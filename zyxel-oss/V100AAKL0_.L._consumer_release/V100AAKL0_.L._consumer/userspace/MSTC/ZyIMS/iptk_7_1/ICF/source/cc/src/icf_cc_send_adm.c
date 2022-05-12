/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_send_adm.c
 *
 * Purpose          : This file contains the function to make and send messages
 *                    to ADM.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 13-Dec-04    Pankaj Negi     Non Service LLD Initial
 * 24-Dec-2004  Rohit Aggarwal                  Made definitions blank for
 *                                              compilation purpose
 * 25-Dec-2004  Rohit Aggarwal                  Header file inclusion
 * 14-Jan-2005  Rohit Aggarwal                  Removed insure compilation
 *                                              warnings
 * 07-Feb-2005  Sumit Gupta                     Added new trace for call term 
 *                                              when no ctx found
 * 13-Apr-2005  Sumit Gupta                     Updation for change in diversion
 *                                              data structure in 
 *                                              icf_common_types.h
 * 06-May-2005 Sumit Gupta     SPR 7438         Reset call_clear status PA 
 *                                              cleared after sending incomming 
 *                                              call indication to PA
 * 25-May-2005 Sumit Gupta       SPR 7440       Updated Stats
 * 26-May-2005 Sumit Gupta       SPR 7599        Remote hold indication would be
 *                                               send to PA after sending 
 *                                               connect indication to PA if the
 *                                               media state is remote held
 * 10-Aug-2005	Rohit Aggarwal	ICF				Code changes specific to ICF
 * 17-Aug-2005  Rohit Aggarwal	ICF				To assign p_called_party_id to 
 *												called_party_addr in incoming_call_ind
 * 07-Mar-2006	Anuradha Gupta	None			Added the changes for incoming
 *												Re-invite handling
 * 08-Mar-2006  Aman Aggarwal					ICF merging with ICF2.0
 * 30-Mar-2006  Anuradha Gupta	ICF 5.0				Call Modify Feature changes
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs    Structure changes for multiple
 *                                              m-lines
 * 21-May-2007  Abhishek Dutta                  Changes for call modify cancel
 *                                              request API.                   
 * 31-Jul-2007  Amit Mahajan    SPR 15487       Assign NULL to p_initial_sip_ctx while giving indication to PA.                         
 *
 * 07-May-2008	Deepti Goyal	SPR 18519       The setting of bitmask 
 *                                              ICF_INC_CALL_MOD_WO_SDP removed.
 *
 * 04-Nov-2008  Anurag Khare    CSR 1-6513044/SPR 19190   Modified function
 *                                              icf_cc_send_pa_forwarding_ind 
 * 1-July-2009  Anurag Khare    SPR 19797
 * Copyright (c) 2009, Aricent.
 *---------------------------------------------------------------------------*/

#include "icf_common_prototypes.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_common_types.h"

#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_incoming_call_ind
 *
 * DESCRIPTION:     This function is used to send an incoming call
 *                  indication to the PA
 *
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_incoming_call_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
   icf_return_t                     ret_val = ICF_SUCCESS;
   /*This varialbe is sent to icf_memfree and even if it
    * has value of failure, it does not impact our code */
   icf_return_t                     dummy_retval = ICF_SUCCESS;
   icf_msg_st                       *p_pa_msg = ICF_NULL;
   icf_cc_pa_incoming_call_ind_st   *p_incoming_call_ind = ICF_NULL;
   icf_uint16_t						msg_len = 0;

   ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

   /* Added to send trace to SM module */
   ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_INCOMING_CALL_IND);

   if(ICF_INVALID_CALL_ID == p_cc_pdb->p_call_ctx->replace_initiated_call_id)
   {
        /* We send incoming call indication only if replaced call flag
         * is false. This flag will be set in case we are processing invite
         * with replace header */

        msg_len = ICF_PORT_SIZEOF(icf_api_header_st) +
            ICF_PORT_SIZEOF(icf_cc_pa_incoming_call_ind_st);

        /*Allocate memory for API header strucure */
        ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
                ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

       /* Fill the message header for ICF_CC_PA_INCOMING_CALL_IND */

        p_pa_msg->hdr.api_id = ICF_CC_PA_INCOMING_CALL_IND;
ICF_SESSION_CONTROL_START
		p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
		p_pa_msg->hdr.api_length = msg_len;
ICF_SESSION_CONTROL_END
        p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;

        /* Fill the payload for ICF_CC_PA_INCOMING_CALL_IND */
        p_incoming_call_ind = (icf_cc_pa_incoming_call_ind_st *)
            (p_pa_msg->payload);

		 /* Initialiize transfering party address */
        icf_cmn_init_address(&p_incoming_call_ind->transferring_party_addr);
        /* Initialiize transfering party name */
        icf_cmn_init_string(&p_incoming_call_ind->transferring_party_name);
        /* Initialiize calling party address */
        icf_cmn_init_address(&p_incoming_call_ind->calling_party_addr);
        /* Initialiize calling party name */
        icf_cmn_init_string(&p_incoming_call_ind->calling_party_name);

		/* Initialize the presence flag */
        p_incoming_call_ind->bit_mask = ICF_NULL;

        p_incoming_call_ind->call_id_to_be_joined = 
            p_cc_pdb->p_call_ctx->call_to_be_joined;

		p_incoming_call_ind->p_accepted_stream_list = ICF_NULL;

        p_incoming_call_ind->line_id = p_cc_pdb->p_call_ctx->line_id;

ICF_SESSION_CONTROL_START
		p_incoming_call_ind->call_type = p_cc_pdb->p_call_ctx->call_type;
		p_incoming_call_ind->bit_mask |= ICF_TYPE_OF_CALL_IND_PRESENT;
ICF_SESSION_CONTROL_END


        /*
         * Send the incoming call indication to PA
         * Populate the the calling party number and
         * calling party name in the icf_cc_pa_incoming_call_ind_st
         * structure from the Remote_party_id field present in the call
         * context.This is expected to be filled by the SSA on receiving the
         * INVITE from the network.
         */
        p_incoming_call_ind->bit_mask |=
            ICF_CC_PA_INC_CALL_CLG_PTY_ADDR;


        if(p_cc_pdb->p_call_ctx->common_bitmask &
                ICF_PRIVACY_HDR_PRESENT)
        {

            /* Privacy header is present
             * send address type as restricted and 
             * address value as anonymous 
             */
            p_incoming_call_ind->calling_party_addr.addr_type =
                ICF_ADDRESS_PLAN_RESTRICTED;

            icf_port_strcpy(
                    p_incoming_call_ind->calling_party_addr.addr_val.str,
                    (icf_uint8_t *)"anonymous");

            p_incoming_call_ind->calling_party_addr.addr_val.str_len =
            (icf_uint16_t)icf_port_strlen(p_incoming_call_ind->
                    calling_party_addr.addr_val.str);

            /* Set name as anonymous */
            p_incoming_call_ind->bit_mask |=
                 ICF_CC_PA_INC_CALL_CLG_PTY_NAME;

            icf_port_strcpy(
                    p_incoming_call_ind->calling_party_name.str,
                    (icf_uint8_t *)"anonymous");

            p_incoming_call_ind->calling_party_name.str_len = 
            (icf_uint16_t)icf_port_strlen(p_incoming_call_ind->calling_party_name.str);
            
        }
        else
        {
            
            icf_port_memcpy(&(p_incoming_call_ind->calling_party_addr),
                    &(p_cc_pdb->p_call_ctx->remote_party_addr),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));

            /* CHeck if remote party name is present in call context*/
            if(ICF_REMOTE_NAME_PRESENT & 
                    p_cc_pdb->p_call_ctx->common_bitmask)
            {
                p_incoming_call_ind->bit_mask |=
                    ICF_CC_PA_INC_CALL_CLG_PTY_NAME;

                icf_port_memcpy(&(p_incoming_call_ind->calling_party_name),
                        &(p_cc_pdb->p_call_ctx->remote_party_name),
                        (icf_uint32_t)ICF_PORT_SIZEOF(icf_name_st));
            }
        }

        /*
         *  In case of Invite with refer to header, CC need to indicate
         * transferring party name and address to PA
         */

        if(ICF_NULL != p_cc_pdb->p_call_ctx->p_referred_party_addr)
        {
            p_incoming_call_ind->bit_mask |=
                ICF_CC_PA_INC_CALL_TFR_PTY_ADDR;

            icf_port_memcpy(&(p_incoming_call_ind->transferring_party_addr),
                    (p_cc_pdb->p_call_ctx->p_referred_party_addr),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
        }

        if(ICF_NULL != p_cc_pdb->p_call_ctx->p_referred_party_name)
        {
            p_incoming_call_ind->bit_mask |=
                ICF_CC_PA_INC_CALL_TFR_PTY_NAME;


            icf_port_memcpy(&(p_incoming_call_ind->transferring_party_name),
                    p_cc_pdb->p_call_ctx->p_referred_party_name,               
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_name_st));
        }

        /* Set the call waiting indication in call context */
        p_incoming_call_ind->call_waiting_ind =
            p_cc_pdb->p_call_ctx->call_waiting_call;
ICF_SESSION_CONTROL_START
		if (ICF_TRUE == p_cc_pdb->p_call_ctx->call_waiting_call)
		{
			p_incoming_call_ind->bit_mask |= ICF_CALL_WAITING_IND_PRESENT;
            ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_CALL_WAITING_IND)
		}
ICF_SESSION_CONTROL_END

        /* the originator (calling party) identity is asserted/verified by registrar */
ICF_SESSION_CONTROL_START
		if (ICF_TRUE == p_cc_pdb->p_call_ctx->remote_aserted_address)
		{
			p_incoming_call_ind->bit_mask |= 
				ICF_CALLING_ADD_ASRTD_IND_PRESENT;
		}
ICF_SESSION_CONTROL_END
        p_incoming_call_ind->calling_party_addr_asserted =
            p_cc_pdb->p_call_ctx->remote_aserted_address;

ICF_SESSION_CONTROL_START
		/* copy the local address */
		if (ICF_NULL != p_cc_pdb->p_call_ctx->p_called_party_id)
		{
			p_incoming_call_ind->bit_mask |= ICF_CALLED_PARTY_ADD_PRESENT;

			icf_port_memcpy(
				&(p_incoming_call_ind->called_party_addr),
                (p_cc_pdb->p_call_ctx->p_called_party_id),
                (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));

			ICF_MEMFREE(
				p_cc_pdb->p_glb_pdb,
				p_cc_pdb->p_call_ctx->p_called_party_id,
				ICF_MEM_COMMON,
				p_cc_pdb->p_ecode,
				ret_val)
		}
   	    /*p_called_part_id not received from network,check if alternate address flag set in call
	     context,if yes then copy it to called party address*/
#ifdef ICF_REQ_URI_CALLED_PARTY_ADDR_PARSING         
	    else if(p_cc_pdb->p_call_ctx->common_bitmask & ICF_ALTERNATE_ADDR_FORMAT_POSSIBLE)
            {
		/* p_called_party_id is not present and alternate address is present in call context,
         	copy alternate party addr to called party addr*/
	
          	p_incoming_call_ind->bit_mask |= ICF_CALLED_PARTY_ADD_PRESENT;
          	icf_port_memcpy(&(p_incoming_call_ind->called_party_addr),
                          &(p_cc_pdb->p_call_ctx->alternate_local_address),
                          (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
            }  
#endif            
        else
        { /* p_called_party_id is not present filll local party addr*/
          p_incoming_call_ind->bit_mask |= ICF_CALLED_PARTY_ADD_PRESENT;
          icf_port_memcpy(&(p_incoming_call_ind->called_party_addr),
                          &(p_cc_pdb->p_call_ctx->local_address),
                          (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
        }                   

		/* clone the header list */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_header_list(
									p_cc_pdb->p_glb_pdb,
									&(p_cc_pdb->p_call_ctx->header_list),
									&(p_incoming_call_ind->header_list),
									p_cc_pdb->p_ecode))
			{
				p_incoming_call_ind->bit_mask |= ICF_HDR_LIST_PRESENT;

				/* free the list in call ctx */
				ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
								&(p_cc_pdb->p_call_ctx->header_list),
								p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else{
			p_incoming_call_ind->header_list.hdr = ICF_NULL;
			p_incoming_call_ind->header_list.count = 0;
		}
		/* clone the body list */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_body_list(
									p_cc_pdb->p_glb_pdb,
									&(p_cc_pdb->p_call_ctx->body_list),
									&(p_incoming_call_ind->body_list),
									p_cc_pdb->p_ecode))
			{
				p_incoming_call_ind->bit_mask |= ICF_BODY_LIST_PRESENT;

				/* free the list in call ctx */
				ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
								&(p_cc_pdb->p_call_ctx->body_list),
								p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else{
			p_incoming_call_ind->body_list.body = ICF_NULL;
			p_incoming_call_ind->body_list.count = 0;
		}        
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_cc_create_app_media_data(p_cc_pdb);
            if(ICF_SUCCESS == ret_val)
            {
                /*assign the values in the message data from
                  p_cc_pdb */
                p_incoming_call_ind->p_accepted_stream_list = p_cc_pdb->p_list_1;

                if(p_cc_pdb->count)
                {
                    p_incoming_call_ind->count = p_cc_pdb->count;
                    icf_cc_copy_deleted_list(p_cc_pdb,(icf_stream_list_st *)
                        (p_incoming_call_ind->deleted_stream_list));

                    p_incoming_call_ind->bit_mask |=
                            ICF_REJECTED_MEDIA_STREAM_PRESENT;
                }
            }
        }

		if (ICF_FAILURE == ret_val)
		{
			ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
			return ret_val;
		}
ICF_SESSION_CONTROL_END

        p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
            ICF_CC_PA_INCOMING_CALL_IND;



        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;

        /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

        ICF_CC_TRACE(p_cc_pdb, ICF_PA_INCOMING_CALL_IND)

        /*Call icf_adm_process_mesg(p_internal_msg)*/
        ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

        /*If process msg return fail*/
        if(ICF_FAILURE == ret_val)
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

        }  /* if(ICF_FAILURE == ret_val) */
        else
        {
            /*If incomming call indication was successfullly sent to PA the 
             * reset the PA_CLEAR flag in call_clear_status*/
            ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_PA_CLEARED)           
            
            /*If call waiting indication was successfully given to PA then 
             * update stat ICF_STAT_SUCC_CALL_WAITING*/
            if(ICF_TRUE == p_cc_pdb->p_call_ctx->call_waiting_call)
            {
                ICF_STAT(p_cc_pdb->p_glb_pdb,ICF_STAT_SUCC_CALL_WAITING)
            } 
        }

        /*Deallocate API header structure*/
		icf_cmn_free_header_list(
			p_cc_pdb->p_glb_pdb,
			&(p_incoming_call_ind->header_list),
			p_cc_pdb->p_ecode);
		icf_cmn_free_msg_body_list(
			p_cc_pdb->p_glb_pdb,
			&(p_incoming_call_ind->body_list),
			p_cc_pdb->p_ecode);

        /* Free the acquired memory for the list node */
		icf_cc_delete_app_media_data(p_cc_pdb);


        ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, dummy_retval)
    }

#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==ret_val)
    {
        p_cc_pdb->p_glb_pdb->load_data.inc_call_attempts++;
    }
#endif

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;

}/*End of function icf_cc_send_pa_incoming_call_ind */



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_alert_ind
 *
 * DESCRIPTION:     This function is used to send an alert indication to
 *                  the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_alert_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_cc_pa_alert_user_ind_st      *p_alert_ind = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Added to send trace information to SM */
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_ALERT_TO_APP);

    if(ICF_INVALID_CALL_ID == 
            p_cc_pdb->p_call_ctx->replace_initiated_call_id)
    {
        /* We send incoming call indication only if replaced call flag
         * is false. This flag will be set in case we are processing invite
         * with replace header */


        msg_len = ICF_PORT_SIZEOF(icf_api_header_st) +
            ICF_PORT_SIZEOF(icf_cc_pa_alert_user_ind_st);


        /*Allocate memory for API header strucure */
        ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON,
                p_pa_msg, ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

        /* Fill the message header for ICF_CC_PA_ALERT_USER_IND */

        p_pa_msg->hdr.api_id = ICF_CC_PA_ALERT_USER_IND;
        p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
ICF_SESSION_CONTROL_START
		p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
		p_pa_msg->hdr.api_length = msg_len;
ICF_SESSION_CONTROL_END

        /* Fill the payload for ICF_CC_PA_INCOMING_CALL_IND */
        p_alert_ind = (icf_cc_pa_alert_user_ind_st*)(p_pa_msg->payload);

		p_alert_ind->p_accepted_stream_list = ICF_NULL;

       /*
        * Check whether inband_info(inband_info_status)is present or not
        * This field is present in call context
       */
        p_alert_ind->inband_info_status =
            p_cc_pdb->p_call_ctx->inband_info_present;

ICF_SESSION_CONTROL_START

		/* Initialize the presence flag */
        p_alert_ind->bit_mask = ICF_NULL;

		/* clone the header list */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_header_list(
									p_cc_pdb->p_glb_pdb,
									&(p_cc_pdb->p_call_ctx->header_list),
									&(p_alert_ind->header_list),
									p_cc_pdb->p_ecode))
			{
				p_alert_ind->bit_mask |= ICF_HDR_LIST_PRESENT;

				/* free the list in call ctx */
				ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
								&(p_cc_pdb->p_call_ctx->header_list),
								p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else
		{
			p_alert_ind->header_list.count = 0;
			p_alert_ind->header_list.hdr = ICF_NULL;
		}

		/* clone the body list */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_body_list(
									p_cc_pdb->p_glb_pdb,
									&(p_cc_pdb->p_call_ctx->body_list),
									&(p_alert_ind->body_list),
									p_cc_pdb->p_ecode))
			{
				p_alert_ind->bit_mask |= ICF_BODY_LIST_PRESENT;

				/* free the list in call ctx */
				ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
								&(p_cc_pdb->p_call_ctx->body_list),
								p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else
		{
			p_alert_ind->body_list.count = 0;
			p_alert_ind->body_list.body = ICF_NULL;
		}
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_cc_create_app_media_data(p_cc_pdb);
            if(ICF_SUCCESS == ret_val)
            {
                /*assign the values in the message data from
                  p_cc_pdb */
                p_alert_ind->p_accepted_stream_list = p_cc_pdb->p_list_1;

                if(p_cc_pdb->count)
                {
                    p_alert_ind->count = p_cc_pdb->count;
                    icf_cc_copy_deleted_list(p_cc_pdb,(icf_stream_list_st *)
                       (p_alert_ind->deleted_media_streams));

                    p_alert_ind->bit_mask |=
                        ICF_REM_USER_ALRTED_DELETED_STREAM_PRESENT;
                }
            }
        }

		if (ICF_FAILURE == ret_val)
		{
			ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
			return ICF_FAILURE;
		}
ICF_SESSION_CONTROL_END

        /*Fill header in internal message structure*/
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_ALERT_USER_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id = 
					p_cc_pdb->p_call_ctx->call_id;

        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;

        /*Assign p_pa_msg to p_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

        ICF_CC_TRACE(p_cc_pdb, ICF_PA_ALERT_USER_IND)
#ifdef ICF_TRACE_ENABLE
        p_cc_pdb->p_call_ctx->p_trace_criteria_data->call_type |=
           ICF_TRACE_ALERTED_CALL;
        p_cc_pdb->p_call_ctx->p_trace_criteria_data->bitmask |= 
           ICF_TRACE_CALL_TYPE_PRESENT;
#endif

        /*Call icf_adm_process_mesg(p_internal_msg)*/
        ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

        /*If process msg return fail*/
        if(ICF_FAILURE == ret_val)
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

            ret_val = ICF_FAILURE;

        }  /* if(ICF_FAILURE == ret_val) */

        /*Deallocate API header structure*/
		icf_cmn_free_header_list(
			p_cc_pdb->p_glb_pdb,
			&(p_alert_ind->header_list),
			p_cc_pdb->p_ecode);
		icf_cmn_free_msg_body_list(
			p_cc_pdb->p_glb_pdb,
			&(p_alert_ind->body_list),
			p_cc_pdb->p_ecode);

        /* Free the acquired memory for the list node */
        icf_cc_delete_app_media_data(p_cc_pdb);

        ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, dummy_retval)
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;

}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_media_connect
 *
 * DESCRIPTION:     This function is used to send a connect indiaction
 *                  to the PA on receiving CONNECT from the SSA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_media_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                     msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
ICF_SESSION_CONTROL_START
	icf_media_connected_ind_st		*p_media_ind = ICF_NULL;
ICF_SESSION_CONTROL_END
    

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /* Added to send trace information to SM */
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_MEDIA_CONNECT_TO_APP);

    if(ICF_INVALID_CALL_ID == 
            p_cc_pdb->p_call_ctx->replace_initiated_call_id)
    {
        /* We send incoming call indication only if replaced call flag
         * is false. This flag will be set in case we are processing invite
         * with replace header */

ICF_SESSION_CONTROL_START
        msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF(icf_media_connected_ind_st);
ICF_SESSION_CONTROL_END

        /*Allocate memory for API header strucure */
        ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
                ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

        /* Fill the message header for ICF_CC_PA_CONNECT_IND */

        p_pa_msg->hdr.api_id = ICF_CC_PA_MEDIA_CONNECTED_IND;
        p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
ICF_SESSION_CONTROL_START
		p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
		p_pa_msg->hdr.api_length = msg_len;

		/* Fill the payload for ICF_CC_PA_MEDIA_CONNECTED_IND */
        p_media_ind = (icf_media_connected_ind_st *)(p_pa_msg->payload);

		/* Initialize the presence flag */
        p_media_ind->bit_mask = ICF_NULL;

		p_media_ind->p_accepted_stream_list = ICF_NULL;

		/* Also free body list, this may have been filled by SSA on receiving
		 * media param with sdp from peer */
		if (ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count)
		{
			ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb, 
							&(p_cc_pdb->p_call_ctx->body_list),
							p_cc_pdb->p_ecode);
		}
		
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_cc_create_app_media_data(p_cc_pdb);
            if(ICF_SUCCESS == ret_val)
            {
                /*assign the values in the message data from
                  p_cc_pdb */
                p_media_ind->p_accepted_stream_list = p_cc_pdb->p_list_1;

                if(p_cc_pdb->count)
                {
                    p_media_ind->count = p_cc_pdb->count;
                    icf_cc_copy_deleted_list(p_cc_pdb,(icf_stream_list_st *)
                        (p_media_ind->deleted_media_streams));

                    p_media_ind->bit_mask |= 
                        ICF_MEDIA_CONECT_DELETED_STREAM_PRESENT;
              
                }      
            }
        }

		if (ICF_FAILURE == ret_val)
		{
                       ICF_MEMFREE(p_cc_pdb->p_glb_pdb, 
                                   p_pa_msg, ICF_MEM_COMMON,
                                   p_cc_pdb->p_ecode, dummy_retval) 
                       
			ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
			return ICF_FAILURE;
		}
ICF_SESSION_CONTROL_END

        /*Fill header in internal message structure*/
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
            ICF_CC_PA_MEDIA_CONNECTED_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;


        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;

        /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

        ICF_CC_TRACE(p_cc_pdb, ICF_PA_MEDIA_CONNECT)

        /*Call icf_adm_process_mesg(p_internal_msg)*/
        ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

        /*If process msg return fail*/
        if(ICF_FAILURE == ret_val)
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

            ret_val = ICF_FAILURE;

        }  /* if(ICF_FAILURE == ret_val) */

		icf_cc_delete_app_media_data(p_cc_pdb);

        ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, dummy_retval)

    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_connect_ind
 *
 * DESCRIPTION:     This function is used to send a connect indiaction
 *                  to the PA on receiving CONNECT from the SSA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_connect_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                     msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
ICF_SESSION_CONTROL_START
	icf_connect_ind_st				*p_connect_ind = ICF_NULL;
ICF_SESSION_CONTROL_END

    icf_internal_list_st             *p_stream_sdp_info_node = ICF_NULL;
    icf_stream_sdp_info_st           *p_stream_sdp_info_data = ICF_NULL;     

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

ICF_SESSION_CONTROL_START
        msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF (icf_connect_ind_st);
ICF_SESSION_CONTROL_END
    /*  Changes For XCONF */
    if((p_cc_pdb->p_call_ctx->common_bitmask_2 &
            ICF_CC_EXTERNAL_CONFERENCE_CALL) &&
       (p_cc_pdb->p_call_ctx->common_bitmask_2 &
           ICF_NW_INC_CONNECT_ISFOCUS_FOUND)) 
     {
	     ICF_CC_SET_STATUS_2(p_cc_pdb,
                 ICF_STATUS_EXTERNAL_CONFERENCE_CONNECTED)
         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_CONNECT_DONE)    
     }
    else
     {
        /*Allocate memory for API header strucure */
        ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
                ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

        /* Fill the message header for ICF_CC_PA_CONNECT_IND */

        p_pa_msg->hdr.api_id = ICF_CC_PA_CONNECT_IND;
        p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;

        
ICF_SESSION_CONTROL_START
		p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
		p_pa_msg->hdr.api_length = msg_len;

		/* Fill the payload for ICF_CC_PA_CONNECT_IND */
        p_connect_ind = (icf_connect_ind_st *)(p_pa_msg->payload);

		/* Initialize the presence flag */
        p_connect_ind->bit_mask = ICF_NULL;

		p_connect_ind->p_accepted_stream_list = ICF_NULL;

        if (ICF_TRUE == p_cc_pdb->p_call_ctx->inband_dtmf_supported)
        {
            /*Set dtmf supported flag in connect indication;*/
            p_connect_ind->inband_dtmf_supported = ICF_TRUE;
        }
        else
        {
            /*Set dtmf supported flag to false*/
            p_connect_ind->inband_dtmf_supported = ICF_FALSE;
        }

		/* copy the asserted identity */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->p_asserted_identity))
		{
			icf_port_memcpy(
				(icf_void_t *)&(p_connect_ind->asserted_identity),
				(icf_void_t *)(p_cc_pdb->p_call_ctx->p_asserted_identity),
				(icf_uint8_t)ICF_PORT_SIZEOF(icf_address_st));

			p_connect_ind->bit_mask |= ICF_ASSERTED_ID_PRESENT;

			ICF_MEMFREE(
				p_cc_pdb->p_glb_pdb,
				p_cc_pdb->p_call_ctx->p_asserted_identity,
				ICF_MEM_COMMON,
				p_cc_pdb->p_ecode,
				ret_val)

		}

		/* clone the header list */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_header_list(
									p_cc_pdb->p_glb_pdb,
									&(p_cc_pdb->p_call_ctx->header_list),
									&(p_connect_ind->header_list),
									p_cc_pdb->p_ecode))
			{
				p_connect_ind->bit_mask |= ICF_HDR_LIST_PRESENT;

				/* free the list in call ctx */
				ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
								&(p_cc_pdb->p_call_ctx->header_list),
								p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else{
			p_connect_ind->header_list.count = 0;
			p_connect_ind->header_list.hdr = ICF_NULL;
		}
		/* clone the body list */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_body_list(
									p_cc_pdb->p_glb_pdb,
									&(p_cc_pdb->p_call_ctx->body_list),
									&(p_connect_ind->body_list),
									p_cc_pdb->p_ecode))
			{
				p_connect_ind->bit_mask |= ICF_BODY_LIST_PRESENT;

				/* free the list in call ctx */
				ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
								&(p_cc_pdb->p_call_ctx->body_list),
								p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else{
			p_connect_ind->body_list.body = ICF_NULL;
			p_connect_ind->body_list.count = 0;
		}

        if(ICF_SUCCESS == ret_val)
        {
			ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_FILL_CALL_MODIFY_DATA)
            ret_val = icf_cc_create_app_media_data(p_cc_pdb);
            if(ICF_SUCCESS == ret_val)
            {
                /*assign the values in the message data from
                p_cc_pdb */
                p_connect_ind->p_accepted_stream_list = p_cc_pdb->p_list_1;

                if(p_cc_pdb->count)
                {
                    p_connect_ind->count = p_cc_pdb->count;
                    icf_cc_copy_deleted_list(p_cc_pdb,(icf_stream_list_st *)
                        ((p_connect_ind->deleted_media_streams)));
                    p_connect_ind->bit_mask |=
                        ICF_CONNECT_IND_DELETED_STREAM_PRESENT;
                }
            }
        }

		if (ICF_FAILURE == ret_val)
		{
			ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
			return ICF_FAILURE;
		}
ICF_SESSION_CONTROL_END

        /*Fill header in internal message structure*/
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_CONNECT_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;

        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint8_t)msg_len;

        /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
#ifdef ICF_TRACE_ENABLE
        ICF_CC_TRACE(p_cc_pdb, ICF_PA_CONNECT_IND)
        p_cc_pdb->p_call_ctx->p_trace_criteria_data->call_type |=
                ICF_TRACE_CONNECTED_CALL;
        p_cc_pdb->p_call_ctx->p_trace_criteria_data->bitmask |=
                ICF_TRACE_CALL_TYPE_PRESENT;
#endif

        /*Call icf_adm_process_mesg(p_internal_msg)*/
        ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

        /*If process msg return fail*/
        if(ICF_FAILURE == ret_val)
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

            ret_val = ICF_FAILURE;

        }  /* if(ICF_FAILURE == ret_val) */

        /*Deallocate API header structure*/
		icf_cmn_free_header_list(
			p_cc_pdb->p_glb_pdb,
			&(p_connect_ind->header_list),
			p_cc_pdb->p_ecode);
		icf_cmn_free_msg_body_list(
			p_cc_pdb->p_glb_pdb,
			&(p_connect_ind->body_list),
			p_cc_pdb->p_ecode);

        /* Free the acquired memory for the list node */
		icf_cc_delete_app_media_data(p_cc_pdb);

        ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, dummy_retval)

    /* Set call connect done */
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_CONNECT_DONE)

#ifdef ICF_TRACE_ENABLE
    ICF_CC_APP_TRACE(p_cc_pdb, ICF_TRACE_CALL_CONNECTED)
#endif

    if(ICF_SUCCESS == ret_val)
    {
#if 0
#ifdef ICF_SERVICE_ENABLE
        /*Now that the call is successfully connected send remote hold 
         *indication to PA. If media state is remote held, send remote hold 
         indication to pa*/
        if(ICF_NULL != ICF_CC_CHECK_MEDIA_STATE(p_cc_pdb,
                        ICF_MEDIA_STATE_REMOTE_HELD))
        {
            ret_val = icf_cc_send_pa_remote_hold_ind(p_cc_pdb);
        }
#endif
#endif
    }

	/* Fix corresponding to spr 14476.*/
    p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    while(ICF_NULL != p_stream_sdp_info_node)
    {
        p_stream_sdp_info_data = p_stream_sdp_info_node->p_payload;
        if(ICF_NULL != p_stream_sdp_info_data->p_offered_sdp)
        {
           icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb,
					&(p_stream_sdp_info_data->p_offered_sdp),
					p_cc_pdb->p_ecode);

        }
        p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
	}
	/* End of Fix corresponding to spr 14476.*/

        /* Assign following variable NULL value, once connect ind 
           is given to PA. */ 
        p_cc_pdb->p_call_ctx->p_initial_sip_ctx = ICF_NULL;


#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==ret_val)
    {
        p_cc_pdb->p_glb_pdb->load_data.otg_call_connects++;
        /*Start call duration timer*/
    }
#endif

#ifdef ICF_LOAD_FORCEFUL_CALL_CLEAR
    if(ICF_SUCCESS==ret_val)
    {
        icf_cc_start_load_call_duration_timer(p_cc_pdb);
    }
#endif
}
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_term_ind
 *
 * DESCRIPTION:     This function is used to send a call termination
 *                  indication to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_call_term_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_cc_pa_terminate_call_ind_st  *p_call_term_ind = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
#ifdef ICF_NW_ACTIVATE
    icf_boolean_t                   gprs_status = ICF_TRUE;
#endif

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*Bitmask is set when term_call_ind is first time send to application, from
     *next time this function will return success if bitmask is already set*/
    if (ICF_CC_TERM_CALL_ALREADY_SEND_TO_APP & 
                      p_cc_pdb->p_call_ctx->common_bitmask_2)
    {
        return ICF_SUCCESS;
    }
    else
    {
        p_cc_pdb->p_call_ctx->common_bitmask_2 |= 
                         ICF_CC_TERM_CALL_ALREADY_SEND_TO_APP;
    }

    /*  Changes for REL 7.0 XCONF */
 
    if(ICF_CC_EXTERNAL_CONFERENCE_CALL & p_cc_pdb->p_call_ctx->common_bitmask_2)
    {
	ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, ICF_CALL_CLEAR_STATUS_PA_CLEARED);
	return ICF_SUCCESS;
    }
 
    /* Changes For XCONF End */

  else
  {  
    if(ICF_INVALID_CALL_ID == 
            p_cc_pdb->p_call_ctx->replace_initiated_call_id)
    {

        msg_len = ICF_PORT_SIZEOF(icf_api_header_st) +
            ICF_PORT_SIZEOF(icf_cc_pa_terminate_call_ind_st);


        /*Allocate memory for API header strucure */
        ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
        ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

        /* Fill the message header for ICF_CC_PA_TERMINATE_CALL_IND */

        p_pa_msg->hdr.api_id = ICF_CC_PA_TERMINATE_CALL_IND;
        p_pa_msg->hdr.api_length = msg_len;
        p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
ICF_SESSION_CONTROL_START
	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
ICF_SESSION_CONTROL_END

        /* Fill the payload for ICF_CC_PA_TERMINATE_CALL_IND */
        p_call_term_ind = (icf_cc_pa_terminate_call_ind_st*)
                    (p_pa_msg->payload);

        /* Reset the bitmask */
        p_call_term_ind->bit_mask = ICF_NULL;

#ifdef ICF_NW_ACTIVATE
    if ((ICF_FAILURE != icf_cmn_chk_gprs_status(p_cc_pdb->p_glb_pdb,
            &gprs_status, p_cc_pdb->p_ecode)) &&
                (ICF_FALSE == gprs_status))
    {
        p_call_term_ind->call_clear_reason =
                        ICF_CALL_CLEAR_REASON_GPRS_SUSPENDED;
    
    }
    else
#endif
    {
       p_call_term_ind->call_clear_reason = p_cc_pdb->p_call_ctx->release_cause;
       if(ICF_NULL != p_cc_pdb->p_call_ctx->inc_response_code)
         {
          p_call_term_ind->bit_mask |= ICF_REMOTE_CALL_CLEAR_RESPONSE_CODE_PRESENT;
          p_call_term_ind->response_code = p_cc_pdb->p_call_ctx->inc_response_code;
         } 

        if(ICF_NULL != p_cc_pdb->p_call_ctx->p_response_str)
        {
          icf_port_strcpy(p_call_term_ind->additional_info.str,
                     p_cc_pdb->p_call_ctx->p_response_str);

          p_call_term_ind->additional_info.str_len =
		(icf_uint16_t)icf_port_strlen(p_cc_pdb->p_call_ctx->p_response_str);	
        
          p_call_term_ind->bit_mask |= ICF_ADDNL_INFO_PRESENT;
        }
    
        
       if(ICF_NULL != p_cc_pdb->p_call_ctx->retry_after_duration)
        {
           p_call_term_ind->retry_after_duration = 
                    p_cc_pdb->p_call_ctx->retry_after_duration;
           p_cc_pdb->p_call_ctx->retry_after_duration = ICF_NULL;
           p_call_term_ind->bit_mask |=
                                ICF_RETRY_AFTER_DURATION_PRESENT;
        }

    }

        /*Fill header in internal message structure*/
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_TERMINATE_CALL_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;

        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = msg_len;

        /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

         ICF_CC_TRACE(p_cc_pdb, ICF_PA_CALL_TERM_IND)

            /*Call icf_adm_process_mesg(p_internal_msg)*/
        ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

        /*If process msg return fail*/
        if(ICF_FAILURE == ret_val)
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

            ret_val = ICF_FAILURE;

        }  /* if(ICF_FAILURE == ret_val) */

#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==ret_val)
    {
        switch(p_call_term_ind->call_clear_reason)
        {
            case ICF_CALL_CLEAR_REASON_REMOTE_RELEASED:
            p_cc_pdb->p_glb_pdb->load_data.term_call_ind_bye++;
            break;

            case ICF_CALL_CLEAR_REASON_USER_NOT_FOUND:
            p_cc_pdb->p_glb_pdb->load_data.term_call_ind_404++;
            break;

            case ICF_CALL_CLEAR_REMOTE_BUSY:
            p_cc_pdb->p_glb_pdb->load_data.term_call_ind_486++;
            break;

            default:
            p_cc_pdb->p_glb_pdb->load_data.term_call_ind_default++;
            break;
        }
    }
#endif
    
        /*Deallocate API header structure*/
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, dummy_retval)
    }
  } 
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_term_resp
 *
 * DESCRIPTION:     This function is used to send a call termination
 *                  response to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_call_term_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
    

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    if(ICF_INVALID_CALL_ID == p_cc_pdb->p_call_ctx->replace_initiated_call_id)
    {

    msg_len = ICF_PORT_SIZEOF(icf_api_header_st);


    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
            ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    /* Fill the message header for ICF_CC_PA_TERMINATE_CALL_RESP */

    p_pa_msg->hdr.api_id = ICF_CC_PA_TERMINATE_CALL_RESP;
    p_pa_msg->hdr.api_length = msg_len;
    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
ICF_SESSION_CONTROL_START
	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
ICF_SESSION_CONTROL_END

    /*Fill header in internal message structure*/
    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_TERMINATE_CALL_RESP;
    p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = msg_len;

    /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

    ICF_CC_TRACE(p_cc_pdb, ICF_PA_TERMINATE_CALL_RESP)

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

    /*If process msg return fail*/
    if(ICF_FAILURE == ret_val)
    {
        /*Deallocate API header structure*/
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

        ret_val = ICF_FAILURE;

     }  /* if(ICF_FAILURE == ret_val) */

     /*Deallocate API header structure*/
     ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode, dummy_retval)

    } /* if (replace_initiated_call_id) */
    else
    {
        /* set call state as await connect ack */
        ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_00_IDLE)
    }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_term_resp_invalid_callid
 *
 * DESCRIPTION:     This function is used to send a call termination
 *                  response to the PA when PA sends call_term_req.
 *                  This request will come with an invalid call-id, CC will 
 *                  simply send a call_term_resp with the same call-id as was 
 *                  sent by PA in call_term_req.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_call_term_resp_invalid_callid(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + 1;

    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
            ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    /* Fill the message header for ICF_CC_PA_TERMINATE_CALL_RESP */
    p_pa_msg->hdr.api_id = ICF_CC_PA_TERMINATE_CALL_RESP;
    p_pa_msg->hdr.api_length = ICF_PORT_SIZEOF(icf_api_header_st);
    p_pa_msg->hdr.call_id = p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
    p_pa_msg->hdr.app_id = p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

    /*Fill header in internal message structure*/
    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_TERMINATE_CALL_RESP;
    p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = msg_len;

    /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

     /*Deallocate API header structure*/
     ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode, dummy_retval)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
} /* icf_cc_send_pa_call_term_resp_invalid_callid() */


#ifdef ICF_SERVICE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_forwarding_ind
 *
 * DESCRIPTION:     This function is used to a forwarding indication to PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_forwarding_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    icf_msg_st                         *p_pa_msg = ICF_NULL;
    icf_cc_pa_call_redirection_ind_st  *p_redirection_ind = ICF_NULL;
    icf_uint16_t                        msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
    

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) +
         ICF_PORT_SIZEOF(icf_cc_pa_call_redirection_ind_st);


    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
            ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    /* Fill the message header for ICF_CC_PA_CALL_REDIRECTION_IND */

    p_pa_msg->hdr.api_id = ICF_CC_PA_CALL_REDIRECTION_IND;
    p_pa_msg->hdr.api_length = msg_len;
    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
    p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;

    p_redirection_ind =
        (icf_cc_pa_call_redirection_ind_st *)p_pa_msg->payload;

    /* Iinitalize the presence flag */
    p_redirection_ind->bit_mask = ICF_NULL;
    /*  Fix for SPR 19190 */
    /*  SCENARIO :
     *  INVITE sent 302 receivbed with diversion header
     *  THen Redirected party address should be filled with 
     * contact_addr not from diversion hdr
     */
    /*Check presence of redirected party address */
    if(ICF_NULL != 
            p_cc_pdb->p_call_ctx->p_diversion_header_list->p_contact_addr)
    {
        /*Copy redirected party address */

        p_redirection_ind->bit_mask |=
            ICF_CC_PA_CALL_RDR_IND_PTY_ADDR;
        
        icf_port_memcpy(&(p_redirection_ind->redirected_party_addr),
                (p_cc_pdb->p_call_ctx->p_diversion_header_list->
                    p_contact_addr),
                (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));

        /* If redirected party name is avilable */
        if(ICF_NULL != p_cc_pdb->p_call_ctx->
                p_diversion_header_list->p_contact_name)
        {
            p_redirection_ind->bit_mask |= 
                ICF_CC_PA_CALL_RDR_IND_PTY_NAME;

            icf_port_memcpy(&(p_redirection_ind->redirected_party_name),
                    (p_cc_pdb->p_call_ctx->p_diversion_header_list->
                        p_contact_name),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_name_st));

        }
    }

    /* clone the header list */
    if (ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count)
    {
        if (ICF_SUCCESS == icf_cmn_clone_header_list(
            p_cc_pdb->p_glb_pdb,
            &(p_cc_pdb->p_call_ctx->header_list),
            &(p_redirection_ind->header_list),
            p_cc_pdb->p_ecode))
        {
            p_redirection_ind->bit_mask |= ICF_HDR_LIST_PRESENT;
            
            /* free the list in call ctx */
            ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
                &(p_cc_pdb->p_call_ctx->header_list),
                p_cc_pdb->p_ecode);
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else{
        p_redirection_ind->header_list.hdr = ICF_NULL;
        p_redirection_ind->header_list.count = 0;
    }

    if (ICF_SUCCESS == ret_val)
    {
        /*Fill header in internal message structure*/
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
            ICF_CC_PA_CALL_REDIRECTION_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;
        
        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = msg_len;
        
        /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
        
        ICF_CC_TRACE(p_cc_pdb, ICF_PA_CALL_REDIRECTION_IND)
            
        /*Call icf_adm_process_mesg(p_internal_msg)*/
        ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);
        
        /*If process msg return fail raise a informational trace */
        if(ICF_FAILURE == ret_val)
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
                
            /* Set the stat for unsuccessfull call redirecection */
            ICF_STAT(p_cc_pdb->p_glb_pdb,
            ICF_STAT_UNSUCC_CALL_REDIRECTED)
            
            ret_val = ICF_FAILURE;
            
        }  /* if(ICF_FAILURE == ret_val) */
        else
        {
            /* Set the stat for successfull call redirecection */
            ICF_STAT(p_cc_pdb->p_glb_pdb,
                ICF_STAT_SUCC_CALL_REDIRECTED)
        }
    }

     /*Deallocate API header structure*/
     ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode, dummy_retval)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}
#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_remote_hold_ind
 *
 * DESCRIPTION:     This function is used to send a remote hold indication
 *                  to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_remote_hold_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
    icf_api_ind_st                   *p_api_resp = ICF_NULL;
    icf_glb_pdb_st                   *p_glb_pdb = p_cc_pdb->p_glb_pdb;
    icf_error_t                      *p_ecode = p_cc_pdb->p_ecode;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_api_ind_st);

    /*  Reset remote indication flag */
    /* ANU
    p_cc_pdb->p_call_ctx->remote_mode_change_ind = ICF_FALSE;*/

    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
            ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    /* Fill the message header for ICF_CC_PA_CALL_HOLD_IND */

    p_pa_msg->hdr.api_id = ICF_CC_PA_CALL_HOLD_IND;
    p_pa_msg->hdr.api_length = msg_len;
    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
    p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;

    /*******************************************************************
     * START: Fill app specific headers and body and stream list
     *******************************************************************/

    p_api_resp = (icf_api_ind_st*)&(p_pa_msg->payload[0]);

    /* clone the header list */
    if (ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count)
    {
        if (ICF_SUCCESS == icf_cmn_clone_header_list(
            p_glb_pdb,
            &(p_cc_pdb->p_call_ctx->header_list),
            &(p_api_resp->header_list),
            p_ecode))
        {
            p_api_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
            
            /* free the list in call ctx */
            ret_val = icf_cmn_free_header_list(p_glb_pdb,
                &(p_cc_pdb->p_call_ctx->header_list),
                p_ecode);
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else{
        p_api_resp->header_list.hdr = ICF_NULL;
        p_api_resp->header_list.count = 0;
    }
    /* clone the body list */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
    {
        if (ICF_SUCCESS == icf_cmn_clone_body_list(
            p_glb_pdb,
            &(p_cc_pdb->p_call_ctx->body_list),
            &(p_api_resp->body_list),
            p_ecode))
        {
            p_api_resp->bit_mask |= ICF_BODY_LIST_PRESENT;
            
            /* free the list in call ctx */
            ret_val = icf_cmn_free_msg_body_list(p_glb_pdb,
                &(p_cc_pdb->p_call_ctx->body_list),
                p_ecode);
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else{
        p_api_resp->body_list.body = ICF_NULL;
        p_api_resp->body_list.count = 0;
    }
    /*******************************************************************
     * END: Fill app specific headers and body and stream list
     *******************************************************************/

    if (ICF_SUCCESS == ret_val)
    {
        /*Fill header in internal message structure*/
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_CALL_HOLD_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;
        
        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = msg_len;
        
        /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
        
        ICF_CC_TRACE(p_cc_pdb, ICF_PA_CALL_HOLD_IND)

        /*If process msg return fail*/
        if(ICF_FAILURE == icf_adm_process_mesg(p_cc_pdb->p_internal_msg))
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
                
                ret_val = ICF_FAILURE;
            
        }  /* if(ICF_FAILURE == ret_val) */
    }

    /*Deallocate API header structure*/
    ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
        p_cc_pdb->p_ecode, dummy_retval)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_remote_resume_ind
 *
 * DESCRIPTION:     This function is used to send a remote resume indication
 *                  to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_remote_resume_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
    icf_api_ind_st                   *p_api_resp = ICF_NULL;
    icf_glb_pdb_st                   *p_glb_pdb = p_cc_pdb->p_glb_pdb;
    icf_error_t                      *p_ecode = p_cc_pdb->p_ecode;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_api_ind_st);

    /*  Reset remote indication flag */
    /*p_cc_pdb->p_call_ctx->remote_mode_change_ind = ICF_FALSE;*/

    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
            ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    /* Fill the message header for ICF_CC_PA_CALL_RESUME_IND */

    p_pa_msg->hdr.api_id = ICF_CC_PA_CALL_RESUME_IND;
    p_pa_msg->hdr.api_length = msg_len;
    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
    p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;

    /*******************************************************************
     * START: Fill app specific headers and body and stream list
     *******************************************************************/

    p_api_resp = (icf_api_ind_st*)&(p_pa_msg->payload[0]);

    /* clone the header list */
    if (ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count)
    {
        if (ICF_SUCCESS == icf_cmn_clone_header_list(
            p_glb_pdb,
            &(p_cc_pdb->p_call_ctx->header_list),
            &(p_api_resp->header_list),
            p_ecode))
        {
            p_api_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
            
            /* free the list in call ctx */
            ret_val = icf_cmn_free_header_list(p_glb_pdb,
                &(p_cc_pdb->p_call_ctx->header_list),
                p_ecode);
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else{
        p_api_resp->header_list.hdr = ICF_NULL;
        p_api_resp->header_list.count = 0;
    }
    /* clone the body list */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
    {
        if (ICF_SUCCESS == icf_cmn_clone_body_list(
            p_glb_pdb,
            &(p_cc_pdb->p_call_ctx->body_list),
            &(p_api_resp->body_list),
            p_ecode))
        {
            p_api_resp->bit_mask |= ICF_BODY_LIST_PRESENT;
            
            /* free the list in call ctx */
            ret_val = icf_cmn_free_msg_body_list(p_glb_pdb,
                &(p_cc_pdb->p_call_ctx->body_list),
                p_ecode);
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else{
        p_api_resp->body_list.body = ICF_NULL;
        p_api_resp->body_list.count = 0;
    }
    /*******************************************************************
     * END: Fill app specific headers and body and stream list
     *******************************************************************/

    if (ICF_SUCCESS == ret_val)
    {
        /*Fill header in internal message structure*/
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_CALL_RESUME_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;
        
        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = msg_len;
        
        /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
        
        ICF_CC_TRACE(p_cc_pdb, ICF_PA_CALL_RESUME_IND)
            
        /*Call icf_adm_process_mesg(p_internal_msg)*/
        if(ICF_FAILURE == icf_adm_process_mesg(p_cc_pdb->p_internal_msg))
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
                
                ret_val = ICF_FAILURE;
            
        }  /* if(ICF_FAILURE == ret_val) */
    }
    /* free the list in api resp */
    if(ICF_NULL != p_api_resp->header_list.count)
    {
        ret_val = icf_cmn_free_header_list(p_glb_pdb,
                &(p_api_resp->header_list),
                p_ecode);
    }
    if(ICF_NULL != p_api_resp->body_list.count)
    {
        ret_val = icf_cmn_free_msg_body_list(p_glb_pdb,
                &(p_api_resp->body_list),
                p_ecode);
    }
    if(ICF_NULL != p_api_resp->stream_list.count)
    {
        ret_val = icf_cmn_free_stream_list(p_glb_pdb,
                &(p_api_resp->stream_list),
                p_ecode);
    }

     /*Deallocate API header structure*/
     ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode, dummy_retval)

	     
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}
#endif
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_wo_ctx_call_term
 *
 * DESCRIPTION:     This function is used to send a call termination
 *                  indication to the PA when call ctx is not avilable
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_wo_ctx_call_term(
        INOUT icf_cc_pdb_st *p_cc_pdb,
        IN    icf_call_id_t  call_id,
        IN    icf_call_clear_reason_t release_cause)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_cc_pa_terminate_call_ind_st  *p_call_term_ind = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
     

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) +
                ICF_PORT_SIZEOF(icf_cc_pa_terminate_call_ind_st);


    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
            ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    /* Fill the message header for ICF_CC_PA_TERMINATE_CALL_IND */

    p_pa_msg->hdr.api_id = ICF_CC_PA_TERMINATE_CALL_IND;
    p_pa_msg->hdr.api_length = msg_len;
    p_pa_msg->hdr.call_id = call_id;
    /* dont pickup the app_id from call context as this function 
     * is being called when ICF does not able to get call context
     * from dbm. The app_id is filled from the recv_msg
     */
    p_pa_msg->hdr.app_id = p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

    /* Fill the payload for ICF_CC_PA_TERMINATE_CALL_IND */
    p_call_term_ind = (icf_cc_pa_terminate_call_ind_st*)(p_pa_msg->payload);

    p_call_term_ind->call_clear_reason = release_cause;

    /*Fill header in internal message structure*/
    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_TERMINATE_CALL_IND;
    p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        call_id;

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = msg_len;

    /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

    ICF_CC_TRACE(p_cc_pdb, ICF_PA_CALL_TERM_IND_NO_CTX)

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

     /*Deallocate API header structure*/
     ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode, dummy_retval)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_info_resp
 *
 * DESCRIPTION:     This function is used to send an info response to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_info_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
   icf_return_t                     ret_val = ICF_SUCCESS;
   /*This varialbe is sent to icf_memfree and even if it
    * has value of failure, it does not impact our code */
   icf_return_t                     dummy_retval = ICF_SUCCESS;
   icf_msg_st                       *p_pa_msg = ICF_NULL;
   icf_uint16_t                     msg_len = 0;
   icf_info_resp_st                 *p_info_resp = ICF_NULL;

   ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

   /* We send incoming call indication only if replaced call flag
    * is false. This flag will be set in case we are processing invite
    * with replace header */

    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) +
                ICF_PORT_SIZEOF(icf_info_resp_st);

   /*Allocate memory for API header strucure */
   ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
           ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

       /* Fill the message header for ICF_CC_PA_INCOMING_CALL_IND */

   p_pa_msg->hdr.api_id = ICF_CC_PA_INFO_RESP;
   p_pa_msg->hdr.source_id = ICF_MODULE_ICF;
   p_pa_msg->hdr.destination_id = ICF_MODULE_PA;
   p_pa_msg->hdr.api_length = msg_len;
   p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
   p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
   p_pa_msg->hdr.version = ICF_VERSION_1_0;

   p_info_resp = (icf_info_resp_st *)(p_pa_msg->payload);
   
   if(ICF_NULL != p_cc_pdb->p_call_ctx->app_failure_cause)
   {
       p_info_resp->result = ICF_FAILURE;
       p_info_resp->error_cause = p_cc_pdb->p_call_ctx->app_failure_cause;

       if (ICF_CAUSE_NETWORK_ERROR == p_cc_pdb->p_call_ctx->app_failure_cause)
       {
           p_info_resp->response_code = ((icf_nw_inc_info_resp_st *)
             (p_cc_pdb->p_internal_msg->p_msg_data))->response_code;
       }
       {
           p_info_resp->response_code = 0;
       }
       p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;
   }
   else
   {
       p_info_resp->result = ICF_SUCCESS;
   }
       
   if(ICF_SUCCESS == p_info_resp->result)
   {
		/* clone the header list */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_header_list(
				p_cc_pdb->p_glb_pdb,
				&(p_cc_pdb->p_call_ctx->header_list),
				&(p_info_resp->header_list),
				p_cc_pdb->p_ecode))
			{
				p_info_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
				
				/* free the list in call ctx */
				ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
					&(p_cc_pdb->p_call_ctx->header_list),
					p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else
		{
			p_info_resp->header_list.hdr = ICF_NULL;
			p_info_resp->header_list.count = 0;
		}

		/* clone the body list */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_body_list(
				p_cc_pdb->p_glb_pdb,
				&(p_cc_pdb->p_call_ctx->body_list),
				&(p_info_resp->body_list),
				p_cc_pdb->p_ecode))
			{
				p_info_resp->bit_mask |= ICF_BODY_LIST_PRESENT;
				
				/* free the list in call ctx */
				ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
					&(p_cc_pdb->p_call_ctx->body_list),
					p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else
		{
			p_info_resp->body_list.body = ICF_NULL;
			p_info_resp->body_list.count = 0;
		}
   }

   p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
        ICF_CC_PA_INFO_RESP;
   p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = msg_len;

    /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

	/*Deallocate API header structure*/
	icf_cmn_free_header_list(
		p_cc_pdb->p_glb_pdb,
		&(p_info_resp->header_list),
		p_cc_pdb->p_ecode);
	icf_cmn_free_msg_body_list(
		p_cc_pdb->p_glb_pdb,
		&(p_info_resp->body_list),
		p_cc_pdb->p_ecode);
	ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
		p_cc_pdb->p_ecode, dummy_retval)
    return ret_val;

}


ICF_HANDLE_REINVITE_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_modify_ind
 *
 * DESCRIPTION:     This function is used to send media change indication to
 *                  the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_call_modify_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t                     ret_val = ICF_SUCCESS;
	/*This varialbe is sent to icf_memfree and even if it
    * has value of failure, it does not impact our code */
	icf_return_t                     dummy_retval = ICF_SUCCESS;
	icf_msg_st                       *p_pa_msg = ICF_NULL;
	icf_cc_pa_incoming_call_modify_ind_st	   *p_media_change_ind = ICF_NULL;
	icf_uint16_t						msg_len = 0;
	
	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
	/* Changes For XCONF */
     if((p_cc_pdb->p_call_ctx->common_bitmask_2 &
                 ICF_INC_MEDIA_CHANGE_ISFOCUS_FOUND) && 
        (p_cc_pdb->p_call_ctx->common_bitmask_2 &
         ICF_CC_EXTERNAL_CONFERENCE_CALL))
	{
        	ICF_CC_SET_STATUS_2(p_cc_pdb, ICF_STATUS_EXTERNAL_CONFERENCE_CONNECTED)
	}
	msg_len = ICF_PORT_SIZEOF(icf_api_header_st) +
                       ICF_PORT_SIZEOF(icf_cc_pa_incoming_call_modify_ind_st);
	
	/*Allocate memory for API header strucure */
	ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
		ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)
		
	/* Fill the message header for ICF_CC_PA_INCOMING_CALL_MODIFY_IND */
    p_pa_msg->hdr.api_id = ICF_CC_PA_INCOMING_CALL_MODIFY_IND;

ICF_SESSION_CONTROL_START
	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
	p_pa_msg->hdr.api_length = msg_len;
ICF_SESSION_CONTROL_END

    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
	
	/* Fill the payload for ICF_CC_PA_INCOMING_CALL_MODIFY_IND */
	p_media_change_ind = (icf_cc_pa_incoming_call_modify_ind_st *)
		(p_pa_msg->payload);

	p_media_change_ind->p_active_stream_list = ICF_NULL;
  
    if(ICF_CC_CALL_MODIFY_CFM_NT_REQD == 
        (ICF_CC_CALL_MODIFY_CFM_NT_REQD & p_cc_pdb->common_bitmask))
    {
        p_media_change_ind->confirm_required = ICF_FALSE;
        ICF_CC_RESET_PDB_COMMON_BITMASK(p_cc_pdb,
            ICF_CC_CALL_MODIFY_CFM_NT_REQD)
    }
    else
    {
        p_media_change_ind->confirm_required = ICF_TRUE;
    }

    if ((ICF_FALSE == p_cc_pdb->pa_call_mod_wo_media) ||
            (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                               ICF_CALL_MOD_REQ_WO_SDP)))
    {

        ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,
                                  ICF_CC_FILL_CALL_MODIFY_DATA)

        p_media_change_ind->bit_mask |= 
                  ICF_INC_CALL_ACCEPT_STREAM_LIST_PRESENT;
        ret_val = icf_cc_create_app_media_data(p_cc_pdb);
    }
    else
    {
        /* SPR 18519: The bitmask "ICF_INC_CALL_MOD_WO_SDP"
         * is not used in IPTK Release 7.1.0
         */
		p_cc_pdb->p_list_1 = ICF_NULL;
    }

    if(ICF_SUCCESS == ret_val)
    {
        /*assign the values in the message data from
        p_cc_pdb */
        p_media_change_ind->p_active_stream_list = p_cc_pdb->p_list_1;
        icf_cc_fill_remote_call_hold_status(p_cc_pdb); 

        if(p_cc_pdb->count)
        {
            p_media_change_ind->count = p_cc_pdb->count;
            icf_cc_copy_deleted_list(p_cc_pdb,(icf_stream_list_st *)
                (p_media_change_ind->deleted_stream_list));

            p_media_change_ind->bit_mask |=
                        ICF_INC_CALL_MOD_DELETED_STREAM_PRESENT;
        }
    }

	
	/* clone the header list */
	if ((ICF_SUCCESS == ret_val) && 
		(ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
	{
		if (ICF_SUCCESS == icf_cmn_clone_header_list(
			p_cc_pdb->p_glb_pdb,
			&(p_cc_pdb->p_call_ctx->header_list),
			&(p_media_change_ind->header_list),
			p_cc_pdb->p_ecode))
		{
			p_media_change_ind->bit_mask |= ICF_HDR_LIST_PRESENT;
			
			/* free the list in call ctx */
			ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
				&(p_cc_pdb->p_call_ctx->header_list),
				p_cc_pdb->p_ecode);
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}
	else
	{
		p_media_change_ind->header_list.hdr = ICF_NULL;
		p_media_change_ind->header_list.count = 0;
	}

	/* clone the body list */
	if ((ICF_SUCCESS == ret_val) && 
		(ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
	{
		if (ICF_SUCCESS == icf_cmn_clone_body_list(
			p_cc_pdb->p_glb_pdb,
			&(p_cc_pdb->p_call_ctx->body_list),
			&(p_media_change_ind->body_list),
			p_cc_pdb->p_ecode))
		{
			p_media_change_ind->bit_mask |= ICF_BODY_LIST_PRESENT;
			
			/* free the list in call ctx */
			ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
				&(p_cc_pdb->p_call_ctx->body_list),
				p_cc_pdb->p_ecode);
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}
	else
	{
		p_media_change_ind->body_list.body = ICF_NULL;
		p_media_change_ind->body_list.count = 0;
	}
	if (ICF_FAILURE == ret_val)
	{
                ICF_MEMFREE(p_cc_pdb->p_glb_pdb, 
                            p_pa_msg, 
                            ICF_MEM_COMMON,
		            p_cc_pdb->p_ecode, 
                            dummy_retval)          

		ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
			return ret_val;
	}

ICF_SESSION_CONTROL_END
		
	p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_INCOMING_CALL_MODIFY_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;
	
	p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;
	
	/*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
	p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
	
	ICF_CC_TRACE(p_cc_pdb, ICF_CC_PA_INCOMING_CALL_MODIFY_IND)
		
	/*Call icf_adm_process_mesg(p_internal_msg)*/
	ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);
	
	/*If process msg return fail*/
	if(ICF_FAILURE == ret_val)
	{
		/*Deallocate API header structure*/
		ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
			
	}  /* if(ICF_FAILURE == ret_val) */
	
	
	/*Deallocate API header structure*/
	icf_cmn_free_header_list(
		p_cc_pdb->p_glb_pdb,
		&(p_media_change_ind->header_list),
		p_cc_pdb->p_ecode);
	icf_cmn_free_msg_body_list(
		p_cc_pdb->p_glb_pdb,
		&(p_media_change_ind->body_list),
		p_cc_pdb->p_ecode);

    /* Free the acquired memory for the list node */
	
	icf_cc_delete_app_media_data(p_cc_pdb);
    ICF_CC_RESET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_FILL_CALL_MODIFY_DATA)


	ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
		p_cc_pdb->p_ecode, dummy_retval)
		/*}*/
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
	return ret_val;

}/*End of function icf_cc_send_pa_call_modify_ind */

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_modify_cancel_ind
 *
 * DESCRIPTION:     This function is used to send media change cancel indication to
 *                  the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_call_modify_cancel_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    if(ICF_INVALID_CALL_ID == 
            p_cc_pdb->p_call_ctx->replace_initiated_call_id)
    {

        msg_len = ICF_PORT_SIZEOF(icf_api_header_st);


        /*Allocate memory for API header strucure */
        ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
        ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

        /* Fill the message header for ICF_CC_PA_INCOMING_CALL_MODIFY_CANCEL_IND */

        p_pa_msg->hdr.api_id = ICF_CC_PA_INCOMING_CALL_MODIFY_CANCEL_IND;
        p_pa_msg->hdr.api_length = msg_len;
        p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
ICF_SESSION_CONTROL_START
	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
ICF_SESSION_CONTROL_END


        /*Fill header in internal message structure*/
        p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_INCOMING_CALL_MODIFY_CANCEL_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;

        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = msg_len;

        /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

        ICF_CC_TRACE(p_cc_pdb, ICF_CC_PA_INCOMING_CALL_MODIFY_CANCEL_IND)

            /*Call icf_adm_process_mesg(p_internal_msg)*/
        ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

        /*If process msg return fail*/
        if(ICF_FAILURE == ret_val)
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

            ret_val = ICF_FAILURE;

        }  /* if(ICF_FAILURE == ret_val) */

        /*Deallocate API header structure*/
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, dummy_retval)
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_modify_resp
 *
 * DESCRIPTION:     This function is used to send media change response to
 *                  the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_call_modify_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;

	icf_cc_pa_call_modify_resp_st	   *p_call_modify_resp = ICF_NULL;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /* Added to send trace information to SM */
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_CALL_MODIFY_RESP)

	msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_cc_pa_call_modify_resp_st);
	
	/*Allocate memory for API header strucure */
	ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
		ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    if(ICF_FAILURE == ret_val)
    {
       return ret_val;

    }
	/* Fill the message header for ICF_CC_PA_CALL_MODIFY_RESP */
    p_pa_msg->hdr.api_id = ICF_CC_PA_CALL_MODIFY_RESP;

ICF_SESSION_CONTROL_START
	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
	p_pa_msg->hdr.api_length = msg_len;
ICF_SESSION_CONTROL_END

    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
	
	/* Fill the payload for ICF_CC_PA_CALL_MODIFY_RESP */
	p_call_modify_resp = (icf_cc_pa_call_modify_resp_st *)(p_pa_msg->payload);

	p_call_modify_resp->p_active_stream_list = ICF_NULL;

    ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_FILL_CALL_MODIFY_DATA)

    

	if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(
                    p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL))
	{
		p_call_modify_resp->result = ICF_TRUE;
               /*twc_update handling*/
	if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(
			p_cc_pdb, ICF_CALL_MOD_REQ_WO_SDP))
	{
        
        ret_val = icf_cc_create_app_media_data(p_cc_pdb);
        if(ICF_SUCCESS == ret_val)
        {
            /*assign the values in the message data from
              p_cc_pdb */
            p_call_modify_resp->p_active_stream_list = p_cc_pdb->p_list_1;
            p_call_modify_resp->bit_mask |=
                        ICF_CALL_MOD_ACCEPT_STREAM_LIST_PRESENT;

            if(p_cc_pdb->count)
            {
                p_call_modify_resp->count = p_cc_pdb->count;
                icf_cc_copy_deleted_list(p_cc_pdb,(icf_stream_list_st *)
                   (p_call_modify_resp->deleted_stream_list));

                p_call_modify_resp->bit_mask |=
                        ICF_CALL_MOD_RESP_REJECT_STREAM_PRESENT;
            }
        }
	}
	/*twc_update handling*/
	else
	{
		    /*Reset common_bitmask*/
                    ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP)

                    if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                   ICF_CALL_CONNECT_DONE))
                    {
                       /*This is the case when 200 OK has been received*/
                       /* Return status as signalling done to CM */
                       ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)
                       /* Reset signalling ongoing flag */
                       ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
                    }

	}

ICF_SESSION_CONTROL_START			
		/* clone the header list */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_header_list(
				p_cc_pdb->p_glb_pdb,
				&(p_cc_pdb->p_call_ctx->header_list),
				&(p_call_modify_resp->header_list),
				p_cc_pdb->p_ecode))
			{
				p_call_modify_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
				
				/* free the list in call ctx */
				ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
					&(p_cc_pdb->p_call_ctx->header_list),
					p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else
		{
			p_call_modify_resp->header_list.hdr = ICF_NULL;
			p_call_modify_resp->header_list.count = 0;
		}

		/* clone the body list */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_body_list(
				p_cc_pdb->p_glb_pdb,
				&(p_cc_pdb->p_call_ctx->body_list),
				&(p_call_modify_resp->body_list),
				p_cc_pdb->p_ecode))
			{
				p_call_modify_resp->bit_mask |= ICF_BODY_LIST_PRESENT;
				
				/* free the list in call ctx */
				ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
					&(p_cc_pdb->p_call_ctx->body_list),
					p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else
		{
			p_call_modify_resp->body_list.body = ICF_NULL;
			p_call_modify_resp->body_list.count = 0;
		}

        

		if (ICF_FAILURE == ret_val)
		{
			ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
				return ret_val;
		}

ICF_SESSION_CONTROL_END
	}
	else
	{
		ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

		p_call_modify_resp->result = ICF_FALSE;

		if(ICF_NULL != p_cc_pdb->p_call_ctx->app_failure_cause)
        {
		    p_call_modify_resp->error_cause = p_cc_pdb->p_call_ctx->app_failure_cause;
        }
        else
        {
            p_call_modify_resp->error_cause = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;
        }

		p_call_modify_resp->header_list.hdr = ICF_NULL;
		p_call_modify_resp->header_list.count = 0;

		p_call_modify_resp->body_list.body = ICF_NULL;
		p_call_modify_resp->body_list.count = 0;

        p_call_modify_resp->p_active_stream_list = ICF_NULL;
        p_call_modify_resp->count = 0;

        if(ICF_NULL != p_cc_pdb->p_call_ctx->retry_after_duration)
        {  
            p_call_modify_resp->retry_after_duration = 
                  p_cc_pdb->p_call_ctx->retry_after_duration;
            p_cc_pdb->p_call_ctx->retry_after_duration = ICF_NULL;
           p_call_modify_resp->bit_mask |=
               ICF_RETRY_AFTER_DURATION_PRESENT;
        }

		p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;
	}

	p_cc_pdb->p_call_ctx->call_modify_method_bitmask  &=
		~(ICF_METHOD_CALL_MODIFY_REQ);
	p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_CALL_MODIFY_RESP;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;
	
	p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;
	
	/*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
	p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
	
    /*Reset the bitmask*/
    ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,  ICF_CALL_MOD_CANCEL_REQ)
    /*Reset the bitmask*/
    ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,  ICF_CALL_MOD_CANCEL_REINV_REQ)

	ICF_CC_TRACE(p_cc_pdb, ICF_CC_PA_CALL_MODIFY_RESP)
		
	/*Call icf_adm_process_mesg(p_internal_msg)*/
	ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

	/*If process msg return fail*/
	if(ICF_FAILURE == ret_val)
	{
		/*Deallocate API header structure*/
		ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
			
	}  /* if(ICF_FAILURE == ret_val) */
	
	
	/*Deallocate API header structure*/
	icf_cmn_free_header_list(
		p_cc_pdb->p_glb_pdb,
		&(p_call_modify_resp->header_list),
		p_cc_pdb->p_ecode);
	icf_cmn_free_msg_body_list(
		p_cc_pdb->p_glb_pdb,
		&(p_call_modify_resp->body_list),
		p_cc_pdb->p_ecode);

    /* Free the acquired memory for the list node */
    if(ICF_NULL != p_call_modify_resp->p_active_stream_list)
    {
        icf_cc_delete_app_media_data(p_cc_pdb);
    }
    ICF_CC_RESET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_FILL_CALL_MODIFY_DATA)

	ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
		p_cc_pdb->p_ecode, dummy_retval)
		/*}*/
		
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


ICF_HANDLE_REINVITE_END


#ifdef ICF_FAX_SUPPORT

/******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_start_fax_ind
 *
 * DESCRIPTION:     This function is used to send start fax indication to
 *                  the PA 
 *             
 *
 *****************************************************************************/
icf_return_t icf_cc_send_pa_start_fax_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t                     ret_val = ICF_SUCCESS;
	/*This varialbe is sent to icf_memfree and even if it
    * has value of failure, it does not impact our code */
	icf_return_t                     dummy_retval = ICF_SUCCESS;
	icf_msg_st                       *p_pa_msg = ICF_NULL;
	icf_start_fax_ind_st	         *p_start_fax_ind = ICF_NULL;
	icf_uint16_t					  msg_len = 0;
	
	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
#if ICF_TRACE_ENABLE
    /* Added to send traces to SM module */
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_START_FAX_TO_APP);
#endif
		
	msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_start_fax_ind_st);
	
	/*Allocate memory for API header strucure */
	ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
		ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)
		
	/* Fill the message header for ICF_START_FAX_IND */
    p_pa_msg->hdr.api_id = ICF_START_FAX_IND;

ICF_SESSION_CONTROL_START
	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
	p_pa_msg->hdr.api_length = msg_len;
ICF_SESSION_CONTROL_END

    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
	
	/* Fill the payload for ICF_START_FAX_IND */
	p_start_fax_ind = (icf_start_fax_ind_st *)(p_pa_msg->payload);
	

    p_start_fax_ind->fax_type = p_cc_pdb->p_call_ctx->fax_type;
		
	p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_START_FAX_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;
	
	p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;
	
	/*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
	p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
	
	ICF_CC_TRACE(p_cc_pdb, ICF_START_FAX_IND)
	ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_FAX_CALL_ESTABLISHED)
		
	/*Call icf_adm_process_mesg(p_internal_msg)*/
	ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);
	
	/*If process msg return fail*/
	if(ICF_FAILURE == ret_val)
	{
		/*Deallocate API header structure*/
		ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
			
	}  /* if(ICF_FAILURE == ret_val) */
	
        /* Free the header list */
        if ((ICF_SUCCESS == ret_val) &&
                (ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
        {
             /* free the list in call ctx */
             ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
                           &(p_cc_pdb->p_call_ctx->header_list),
                           p_cc_pdb->p_ecode);
        }

        /* Free the body list */
        if ((ICF_SUCCESS == ret_val) &&
                (ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
        {
             /* free the list in call ctx */
             ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
                           &(p_cc_pdb->p_call_ctx->body_list),
                           p_cc_pdb->p_ecode);
        }	
      
	ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
		p_cc_pdb->p_ecode, dummy_retval)
		
		
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
	return ret_val;

}/*End of function icf_cc_send_pa_start_fax_ind */



/******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_fax_resp
 *
 * DESCRIPTION:     This function is used to send fax resp to
 *                  the PA 
 *             
 *
 *****************************************************************************/
icf_return_t icf_cc_send_pa_fax_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;

	icf_start_fax_resp_st	        *p_start_fax_resp = ICF_NULL;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_start_fax_resp_st);
	
	/*Allocate memory for API header strucure */
	ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
		ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

	/* Fill the message header for ICF_START_FAX_RESP */
    p_pa_msg->hdr.api_id = ICF_START_FAX_RESP;

ICF_SESSION_CONTROL_START
	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
	p_pa_msg->hdr.api_length = msg_len;
ICF_SESSION_CONTROL_END

    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
	
	/* Fill the payload for ICF_START_FAX_RESP */
	p_start_fax_resp = (icf_start_fax_resp_st *)(p_pa_msg->payload);

	if(ICF_NULL == 
        ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL))
	{
		p_start_fax_resp->result = ICF_TRUE;

        p_start_fax_resp->fax_type = p_cc_pdb->p_call_ctx->fax_type;
	ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_FAX_CALL_ESTABLISHED) /*TBD*/
	}
	else
	{
		ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)

		p_start_fax_resp->result = ICF_FALSE;
        
        if(ICF_NULL != p_cc_pdb->p_call_ctx->retry_after_duration)
        {  
            p_start_fax_resp->retry_after_duration = 
                  p_cc_pdb->p_call_ctx->retry_after_duration;
            p_cc_pdb->p_call_ctx->retry_after_duration = ICF_NULL;
            p_start_fax_resp->bit_mask |=
               ICF_RETRY_AFTER_DURATION_PRESENT;
        }

        if(ICF_NULL != p_cc_pdb->p_call_ctx->app_failure_cause)
        {
		    p_start_fax_resp->error_cause = 
               p_cc_pdb->p_call_ctx->app_failure_cause;
        }
        else
        {
            p_start_fax_resp->error_cause = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;
        }

		p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;
	}

	p_cc_pdb->p_call_ctx->call_modify_method_bitmask  &=
		~(ICF_METHOD_START_FAX_REQ);
	p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_START_FAX_RESP;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;
	
	p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;
	
	/*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
	p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
	
	ICF_CC_TRACE(p_cc_pdb, ICF_CC_PA_CALL_MODIFY_RESP)
#if ICF_TRACE_ENABLE
    /* Added to send traces to SM module */
    p_cc_pdb->result= p_start_fax_resp->result;
    p_cc_pdb->resp_code= p_start_fax_resp->error_cause;
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_START_FAX_RESP_TO_APP);
#endif
	/*Call icf_adm_process_mesg(p_internal_msg)*/
	ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

	/*If process msg return fail*/
	if(ICF_FAILURE == ret_val)
	{
		/*Deallocate API header structure*/
		ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
			
	}  /* if(ICF_FAILURE == ret_val) */
       
        /* Free the header list */
        if ((ICF_SUCCESS == ret_val) &&
                (ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
        {
             /* free the list in call ctx */
             ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
                           &(p_cc_pdb->p_call_ctx->header_list),
                           p_cc_pdb->p_ecode);
        }

        /* Free the body list */
        if ((ICF_SUCCESS == ret_val) &&
                (ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
        {
             /* free the list in call ctx */
             ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
                           &(p_cc_pdb->p_call_ctx->body_list),
                           p_cc_pdb->p_ecode);
        }	
	
	/*Deallocate API header structure*/
	ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
		p_cc_pdb->p_ecode, dummy_retval)
		
		
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}





#endif


/*HANDLE_INFO_IND_START*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_info_ind
 *
 * DESCRIPTION:     This function is used to send an info request to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_info_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;

	icf_info_ind_st			*p_info_ind = ICF_NULL;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_info_ind_st);
	
	/*Allocate memory for API header strucure */
	ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
		ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)
		
	/* Fill the message header for ICF_CC_PA_INFO_IND */
    p_pa_msg->hdr.api_id = ICF_CC_PA_INFO_IND;

ICF_SESSION_CONTROL_START
	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
	p_pa_msg->hdr.api_length = msg_len;
ICF_SESSION_CONTROL_END

    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
	
	/* Fill the payload for ICF_CC_PA_INFO_IND */
	p_info_ind = (icf_info_ind_st *)
		(p_pa_msg->payload);
	
	/* clone the header list */
	if ((ICF_SUCCESS == ret_val) && 
		(ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
	{
		if (ICF_SUCCESS == icf_cmn_clone_header_list(
			p_cc_pdb->p_glb_pdb,
			&(p_cc_pdb->p_call_ctx->header_list),
			&(p_info_ind->header_list),
			p_cc_pdb->p_ecode))
		{
			p_info_ind->bit_mask |= ICF_HDR_LIST_PRESENT;
			
			/* free the list in call ctx */
			ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
				&(p_cc_pdb->p_call_ctx->header_list),
				p_cc_pdb->p_ecode);
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}
	else
	{
		p_info_ind->header_list.hdr = ICF_NULL;
		p_info_ind->header_list.count = 0;
	}

	/* clone the body list */
	if ((ICF_SUCCESS == ret_val) && 
		(ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
	{
		if (ICF_SUCCESS == icf_cmn_clone_body_list(
			p_cc_pdb->p_glb_pdb,
			&(p_cc_pdb->p_call_ctx->body_list),
			&(p_info_ind->body_list),
			p_cc_pdb->p_ecode))
		{
			p_info_ind->bit_mask |= ICF_BODY_LIST_PRESENT;
			
			/* free the list in call ctx */
			ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
				&(p_cc_pdb->p_call_ctx->body_list),
				p_cc_pdb->p_ecode);
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}
	else
	{
		p_info_ind->body_list.body = ICF_NULL;
		p_info_ind->body_list.count = 0;
	}
	
	
	if (ICF_FAILURE == ret_val)
	{
		ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
			return ret_val;
	}

ICF_SESSION_CONTROL_END
		
	p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_INFO_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;
	
	p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint8_t)msg_len;
	
	/*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
	p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
	
	ICF_CC_TRACE(p_cc_pdb, ICF_CC_PA_INFO_IND)
		
	/*Call icf_adm_process_mesg(p_internal_msg)*/
	ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);
	
	/*If process msg return fail*/
	if(ICF_FAILURE == ret_val)
	{
		/*Deallocate API header structure*/
		ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
			
	}  /* if(ICF_FAILURE == ret_val) */
	
	
	/*Deallocate API header structure*/
	icf_cmn_free_header_list(
		p_cc_pdb->p_glb_pdb,
		&(p_info_ind->header_list),
		p_cc_pdb->p_ecode);
	icf_cmn_free_msg_body_list(
		p_cc_pdb->p_glb_pdb,
		&(p_info_ind->body_list),
		p_cc_pdb->p_ecode);
	ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
		p_cc_pdb->p_ecode, dummy_retval)

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*HANDLE_INFO_IND_END*/




/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_party_replaced_ind
 *
 * DESCRIPTION:     This function is used to send party replaced indication
 *                  to the PA on receiving Invite with Replaces from SSA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_party_replaced_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                     msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
ICF_SESSION_CONTROL_START
	icf_party_replaced_ind_st		 *p_party_replaced_ind = ICF_NULL;
ICF_SESSION_CONTROL_END
    

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* We send connect indication only if replaced call flag
     * is false. This flag will be set in case we are processing invite
     * with replace header */
    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_party_replaced_ind_st);

    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
            ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    /* Fill the message header for ICF_PARTY_REPLACED_IND */

    p_pa_msg->hdr.api_id = ICF_PARTY_REPLACED_IND;
    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->replace_initiated_call_id;
	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
	p_pa_msg->hdr.api_length = msg_len;

	/* Fill the payload for ICF_PARTY_REPLACED_IND */
    p_party_replaced_ind = (icf_party_replaced_ind_st *)(p_pa_msg->payload);

	/* Initialize the presence flag */
    p_party_replaced_ind->bit_mask = ICF_NULL;

    /* fill the replacing call_id - this is the call_id of the new call */
    p_party_replaced_ind->replacing_call_id = p_cc_pdb->p_call_ctx->call_id;

    /* Initialiize calling party address */
    icf_cmn_init_address(&p_party_replaced_ind->replacing_party_addr);
    /* Initialiize calling party name */
    icf_cmn_init_string(&p_party_replaced_ind->replacing_party_name);

    icf_port_memcpy(&(p_party_replaced_ind->replacing_party_addr),
                    &(p_cc_pdb->p_call_ctx->remote_party_addr),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
    p_party_replaced_ind->bit_mask |=
            ICF_REPLACING_PARTY_ADDR_PRESENT;


    /* CHeck if remote party name is present in call context*/
    if(ICF_REMOTE_NAME_PRESENT & 
        p_cc_pdb->p_call_ctx->common_bitmask)
    {
        p_party_replaced_ind->bit_mask |=
            ICF_PARTY_REPLACED_NAME;
        
        icf_port_memcpy(&(p_party_replaced_ind->replacing_party_name),
            &(p_cc_pdb->p_call_ctx->remote_party_name),
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_name_st));
    }
    /* Added to send trace to SM module */
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_PARTY_REPLACED_IND)

    /* clone the header list */
	if ((ICF_SUCCESS == ret_val) && 
		(ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
	{
		if (ICF_SUCCESS == icf_cmn_clone_header_list(
								p_cc_pdb->p_glb_pdb,
								&(p_cc_pdb->p_call_ctx->header_list),
								&(p_party_replaced_ind->header_list),
								p_cc_pdb->p_ecode))
		{
			p_party_replaced_ind->bit_mask |= ICF_HDR_LIST_PRESENT;

			/* free the list in call ctx */
			ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
							&(p_cc_pdb->p_call_ctx->header_list),
							p_cc_pdb->p_ecode);
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}
	else
    {
		p_party_replaced_ind->header_list.count = 0;
		p_party_replaced_ind->header_list.hdr = ICF_NULL;
	}

	/* clone the body list */
	if ((ICF_SUCCESS == ret_val) && 
		(ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
	{
		if (ICF_SUCCESS == icf_cmn_clone_body_list(
								p_cc_pdb->p_glb_pdb,
								&(p_cc_pdb->p_call_ctx->body_list),
								&(p_party_replaced_ind->body_list),
								p_cc_pdb->p_ecode))
		{
			p_party_replaced_ind->bit_mask |= ICF_BODY_LIST_PRESENT;

			/* free the list in call ctx */
			ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
							&(p_cc_pdb->p_call_ctx->body_list),
							p_cc_pdb->p_ecode);
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}
	else
    {
		p_party_replaced_ind->body_list.body = ICF_NULL;
		p_party_replaced_ind->body_list.count = 0;
	}

	/* clone the stream list */
#if 0 /* TO DO FOR 6.0 */
	if ((ICF_SUCCESS == ret_val) && 
		(ICF_NULL != p_cc_pdb->p_call_ctx->stream_list.count))
	{
		if (ICF_SUCCESS == icf_cmn_clone_stream_list(
								p_cc_pdb->p_glb_pdb,
								&(p_cc_pdb->p_call_ctx->stream_list),
								&(p_party_replaced_ind->stream_list),
								p_cc_pdb->p_ecode))
		{
			p_party_replaced_ind->bit_mask |= ICF_STREAM_LIST_PRESENT;

			/* free the list in call ctx */
			ret_val = icf_cmn_free_stream_list(p_cc_pdb->p_glb_pdb,
							&(p_cc_pdb->p_call_ctx->stream_list),
							p_cc_pdb->p_ecode);
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}
	else
    {
		p_party_replaced_ind->stream_list.count = 0;
	}
#endif

	if (ICF_FAILURE == ret_val)
	{
		ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
		return ICF_FAILURE;
	}
ICF_SESSION_CONTROL_END

    /*Fill header in internal message structure*/
    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_PARTY_REPLACED_IND;
    p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint8_t)msg_len;

    /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

    ICF_CC_TRACE(p_cc_pdb, ICF_PA_CONNECT_IND)

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

    /*If process msg return fail*/
    if(ICF_FAILURE == ret_val)
    {
        /*Deallocate API header structure*/
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

        ret_val = ICF_FAILURE;

    }  /* if(ICF_FAILURE == ret_val) */

    /*Deallocate API header structure*/
	icf_cmn_free_header_list(
		p_cc_pdb->p_glb_pdb,
		&(p_party_replaced_ind->header_list),
		p_cc_pdb->p_ecode);
	icf_cmn_free_msg_body_list(
		p_cc_pdb->p_glb_pdb,
		&(p_party_replaced_ind->body_list),
		p_cc_pdb->p_ecode);
    ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
            p_cc_pdb->p_ecode, dummy_retval)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_cc_send_pa_party_replaced_ind() */



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_connect_resp
 *
 * DESCRIPTION:     This function is used to send a connect response
 *                  to the PA on receiving CONNECT ACK from the SSA for an 
 *                  incoming call.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_connect_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                     msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
ICF_SESSION_CONTROL_START
	icf_connect_resp_st				*p_connect_resp = ICF_NULL;
ICF_SESSION_CONTROL_END
    

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


ICF_SESSION_CONTROL_START
       msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF (icf_connect_resp_st);
ICF_SESSION_CONTROL_END
/*  Changes For XCONF */
    if(p_cc_pdb->p_call_ctx->common_bitmask_2 & ICF_CC_INCOMING_CONFERENCE_REQ)
    {
	    ICF_CC_SET_STATUS_2(p_cc_pdb, 
                ICF_STATUS_INCOMING_EXTERNAL_CONF_CALL)
    }
    else if (ICF_INVALID_CALL_ID != p_cc_pdb->p_call_ctx->call_to_be_joined)
    {
	    ICF_CC_SET_STATUS_2(p_cc_pdb, 
                ICF_STATUS_INVOKE_TWC_FOR_JOIN)
    }
    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
            ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    /* Fill the message header for ICF_CC_PA_CONNECT_RESP */

    p_pa_msg->hdr.api_id = ICF_CC_PA_CONNECT_RESP;
    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;

        
ICF_SESSION_CONTROL_START
	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
	p_pa_msg->hdr.api_length = msg_len;

	/* Fill the payload for ICF_CC_PA_CONNECT_RESP */
    p_connect_resp = (icf_connect_resp_st *)(p_pa_msg->payload);

	/* Initialize the presence flag */
    p_connect_resp->bit_mask = ICF_NULL;

	p_connect_resp->p_accepted_stream_list = ICF_NULL;

	ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_FILL_CALL_MODIFY_DATA)
    ret_val = icf_cc_create_app_media_data(p_cc_pdb);
    if(ICF_SUCCESS == ret_val)
    {
        /*assign the values in the message data from
        p_cc_pdb */
        p_connect_resp->p_accepted_stream_list = p_cc_pdb->p_list_1;

        if(p_cc_pdb->count)
        {
            p_connect_resp->count = p_cc_pdb->count;
            icf_cc_copy_deleted_list(p_cc_pdb,(icf_stream_list_st *)
                ((p_connect_resp->deleted_media_streams)));
            p_connect_resp->bit_mask |=
                ICF_CONNECT_RESP_DELETED_STREAM_PRESENT;
        }
    }
	else
	{
                ICF_MEMFREE(p_cc_pdb->p_glb_pdb, 
                            p_pa_msg, 
                            ICF_MEM_COMMON,
                            p_cc_pdb->p_ecode, 
                            dummy_retval)                

		ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
		return ICF_FAILURE;
	}
ICF_SESSION_CONTROL_END
    
    /* clone the header list */

    if ((ICF_SUCCESS == ret_val) &&
            (ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
    {
            if (ICF_SUCCESS == icf_cmn_clone_header_list(
                                    p_cc_pdb->p_glb_pdb,
                                    &(p_cc_pdb->p_call_ctx->header_list),
                                    &(p_connect_resp->header_list),
                                    p_cc_pdb->p_ecode))
            {
                p_connect_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
                /* free the list in call ctx */
                ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
                                &(p_cc_pdb->p_call_ctx->header_list),
                                p_cc_pdb->p_ecode);
            }
            else
            {
                ret_val = ICF_FAILURE;
            }
    }
    else
    {
            p_connect_resp->header_list.hdr = ICF_NULL;
            p_connect_resp->header_list.count = 0;
    }


    /*Fill header in internal message structure*/
    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_CONNECT_RESP;
    p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;

    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint8_t)msg_len;

    /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

    ICF_CC_TRACE(p_cc_pdb, ICF_PA_CONNECT_IND)
#ifdef ICF_TRACE_ENABLE
    ICF_CC_APP_TRACE(p_cc_pdb, ICF_TRACE_CALL_CONNECTED)
    p_cc_pdb->p_call_ctx->p_trace_criteria_data->call_type |=
               ICF_TRACE_CONNECTED_CALL;
    p_cc_pdb->p_call_ctx->p_trace_criteria_data->bitmask |=
               ICF_TRACE_CALL_TYPE_PRESENT;
#endif

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

    /*If process msg return fail*/
    if(ICF_FAILURE == ret_val)
    {
        /*Deallocate API header structure*/
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

        ret_val = ICF_FAILURE;

    }  /* if(ICF_FAILURE == ret_val) */

    /* Free the acquired memory for the list node */
	icf_cc_delete_app_media_data(p_cc_pdb);

   /* free the list in call ctx */
   icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
                      &(p_connect_resp->header_list),
                       p_cc_pdb->p_ecode);

    ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
            p_cc_pdb->p_ecode, dummy_retval)

#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==ret_val)
    {
        p_cc_pdb->p_glb_pdb->load_data.inc_call_connects++;
    }
#endif
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_early_call_ind
 *
 * DESCRIPTION:     This function is used to send an early call
 *                  indication to the PA
 *
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_early_call_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
   icf_return_t                     ret_val = ICF_SUCCESS;
   /*This varialbe is sent to icf_memfree and even if it
    * has value of failure, it does not impact our code */
   icf_return_t                     dummy_retval = ICF_SUCCESS;
   icf_msg_st                       *p_pa_msg = ICF_NULL;
   icf_early_incoming_call_ind_st		*p_early_call_ind = ICF_NULL;
   icf_uint16_t						msg_len = 0;

   ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
   /* Added to send trace information to SM */
   ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_EARLY_INC_CALL_IND); 

   if(ICF_INVALID_CALL_ID == p_cc_pdb->p_call_ctx->replace_initiated_call_id)
   {
        /* We send incoming call indication only if replaced call flag
         * is false. This flag will be set in case we are processing invite
         * with replace header */

        msg_len = ICF_PORT_SIZEOF(icf_api_header_st) +
            ICF_PORT_SIZEOF(icf_early_incoming_call_ind_st);

        /*Allocate memory for API header strucure */
        ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
                ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

       /* Fill the message header for ICF_CC_PA_INCOMING_CALL_IND */

        p_pa_msg->hdr.api_id = ICF_EARLY_INC_CALL_IND;
ICF_SESSION_CONTROL_START
		p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
		p_pa_msg->hdr.api_length = msg_len;
ICF_SESSION_CONTROL_END
        p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;

        /* Fill the payload for ICF_CC_PA_INCOMING_CALL_IND */
        p_early_call_ind = (icf_early_incoming_call_ind_st *)
            (p_pa_msg->payload);

        /* Initialiize calling party address */
        icf_cmn_init_address(&p_early_call_ind->calling_party_addr);
        /* Initialiize calling party name */
        icf_cmn_init_string(&p_early_call_ind->calling_party_name);
		/* Initialize the presence flag */
        p_early_call_ind->bit_mask = ICF_NULL;

       /*
         * Send the early call indication to PA
         * Populate the the calling party number and
         * calling party name in the icf_early_incoming_call_ind_st
         * structure from the Remote_party_id field present in the call
         * context.This is expected to be filled by the SSA on receiving the
         * INVITE from the network.
         */
        p_early_call_ind->bit_mask |=
            ICF_EARLY_CALL_CLG_PTY_ADDR;


        if(p_cc_pdb->p_call_ctx->common_bitmask &
                ICF_PRIVACY_HDR_PRESENT)
        {

            /* Privacy header is present
             * send address type as restricted and 
             * address value as anonymous 
             */
            p_early_call_ind->calling_party_addr.addr_type =
                ICF_ADDRESS_PLAN_RESTRICTED;

            icf_port_strcpy(
                    p_early_call_ind->calling_party_addr.addr_val.str,
                    (icf_uint8_t *)"anonymous");

            p_early_call_ind->calling_party_addr.addr_val.str_len =
            (icf_uint16_t)icf_port_strlen(p_early_call_ind->
                    calling_party_addr.addr_val.str);

            /* Set name as anonymous */
            p_early_call_ind->bit_mask |=
                 ICF_EARLY_CALL_CLG_PTY_NAME;

            icf_port_strcpy(
                    p_early_call_ind->calling_party_name.str,
                    (icf_uint8_t *)"anonymous");

            p_early_call_ind->calling_party_name.str_len = 
            (icf_uint16_t)icf_port_strlen(p_early_call_ind->calling_party_name.str);
            
        }
        else
        {
            
            icf_port_memcpy(&(p_early_call_ind->calling_party_addr),
                    &(p_cc_pdb->p_call_ctx->remote_party_addr),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));

            /* CHeck if remote party name is present in call context*/
            if(ICF_REMOTE_NAME_PRESENT & 
                    p_cc_pdb->p_call_ctx->common_bitmask)
            {
                p_early_call_ind->bit_mask |=
                    ICF_EARLY_CALL_CLG_PTY_NAME;

                icf_port_memcpy(&(p_early_call_ind->calling_party_name),
                        &(p_cc_pdb->p_call_ctx->remote_party_name),
                        (icf_uint32_t)ICF_PORT_SIZEOF(icf_name_st));
            }
        }

        /*
         *  In case of Invite with refer to header, CC need to indicate
         * transferring party name and address to PA
         */

 
ICF_SESSION_CONTROL_START
		/* copy the local address */
		if (ICF_NULL != p_cc_pdb->p_call_ctx->p_called_party_id)
		{
			p_early_call_ind->bit_mask |= ICF_EARLY_CALL_CALLED_PTY_ADDR;

			icf_port_memcpy(
				&(p_early_call_ind->called_party_addr),
                (p_cc_pdb->p_call_ctx->p_called_party_id),
                (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));

			ICF_MEMFREE(
				p_cc_pdb->p_glb_pdb,
				p_cc_pdb->p_call_ctx->p_called_party_id,
				ICF_MEM_COMMON,
				p_cc_pdb->p_ecode,
				ret_val)
		}
        else
        { 
			/* p_called_party_id is not present filll local party addr*/
			p_early_call_ind->bit_mask |= ICF_EARLY_CALL_CALLED_PTY_ADDR;
			icf_port_memcpy(&(p_early_call_ind->called_party_addr),
							&(p_cc_pdb->p_call_ctx->local_address),
							(icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
		}


/* clone the header list */
if ((ICF_SUCCESS == ret_val) && 
			(ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
		{
			if (ICF_SUCCESS == icf_cmn_clone_header_list(
									p_cc_pdb->p_glb_pdb,
									&(p_cc_pdb->p_call_ctx->header_list),
									&(p_early_call_ind->header_list),
									p_cc_pdb->p_ecode))
			{
				p_early_call_ind->bit_mask |= ICF_HDR_LIST_PRESENT;

				/* free the list in call ctx */
				ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
								&(p_cc_pdb->p_call_ctx->header_list),
								p_cc_pdb->p_ecode);
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}
		else{
			p_early_call_ind->header_list.hdr = ICF_NULL;
			p_early_call_ind->header_list.count = 0;
		}
       
		if (ICF_FAILURE == ret_val)
		{
			ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
			return ret_val;
		}
ICF_SESSION_CONTROL_END

        p_cc_pdb->p_internal_msg->msg_hdr.msg_id =
            ICF_EARLY_INC_CALL_IND;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;

        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;

        /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

        ICF_CC_TRACE(p_cc_pdb, ICF_EARLY_INC_CALL_IND)

        /*Call icf_adm_process_mesg(p_internal_msg)*/
        ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

        /*If process msg return fail*/
        if(ICF_FAILURE == ret_val)
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

        }  /* if(ICF_FAILURE == ret_val) */
        else
        {
            /*If incomming call indication was successfullly sent to PA the 
             * reset the PA_CLEAR flag in call_clear_status*/
            ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_PA_CLEARED)           
            
            /*If call waiting indication was successfully given to PA then 
             * update stat ICF_STAT_SUCC_CALL_WAITING*/
            if(ICF_TRUE == p_cc_pdb->p_call_ctx->call_waiting_call)
            {
                ICF_STAT(p_cc_pdb->p_glb_pdb,ICF_STAT_SUCC_CALL_WAITING)
            } 
        }

        /*Deallocate API header structure*/
		icf_cmn_free_header_list(
			p_cc_pdb->p_glb_pdb,
			&(p_early_call_ind->header_list),
			p_cc_pdb->p_ecode);

        ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, dummy_retval)
    }

#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==ret_val)
    {
        p_cc_pdb->p_glb_pdb->load_data.inc_early_call_attempts++;
    }
#endif

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;

}/*End of function icf_cc_send_pa_early_call_ind */

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_unexpected_msg_ind
 *
 * DESCRIPTION:     This function is used to send an unexpected
 *                  message indication to the PA
 *
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_unexpected_msg_ind(
         INOUT icf_cc_pdb_st *p_cc_pdb)
{
   icf_return_t                     ret_val = ICF_SUCCESS;
   icf_return_t                     dummy_retval = ICF_SUCCESS;
   icf_msg_st                       *p_pa_msg = ICF_NULL;
   icf_unexpected_msg_ind_st   *p_unexpected_msg_ind = ICF_NULL;
   icf_uint16_t                                         msg_len = 0;

   ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

   msg_len = ICF_PORT_SIZEOF(icf_api_header_st) +
            ICF_PORT_SIZEOF(icf_unexpected_msg_ind_st);

          /*Allocate memory for API header strucure */
   ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
                ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

          /* Fill the message header for ICF_UNEXPECTED_MSG_IND */
   p_pa_msg->hdr.api_id = ICF_UNEXPECTED_MSG_IND;
         
          /* Filling the corresponding call id */
   p_pa_msg->hdr.call_id = p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;

ICF_SESSION_CONTROL_START
          /* Filling the corresponding app id */
   p_pa_msg->hdr.app_id = p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;
   p_pa_msg->hdr.api_length = msg_len;
ICF_SESSION_CONTROL_END      
        /* Fill the payload for ICF_UNEXPECTED_MSG_IND */
   p_unexpected_msg_ind = (icf_unexpected_msg_ind_st *)(p_pa_msg->payload);

        /* Filling the error cause */
   p_unexpected_msg_ind->error_cause = ICF_CAUSE_INVALID_ACTION;

     /* Filling the api_id for which this uxexpected msg API is to be sent */
   p_unexpected_msg_ind->api_id = p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.api_id;

       /*Fill header in internal message structure*/
   p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_UNEXPECTED_MSG_IND;
   p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;

   p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint8_t)msg_len;

       /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
   p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

   ICF_CC_TRACE(p_cc_pdb, ICF_UNEXPECTED_MSG_IND)

      /*Call icf_adm_process_mesg(p_internal_msg)*/
   ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

     /*If process msg return fail*/
   if(ICF_FAILURE == ret_val)
    {
        /*Deallocate API header structure*/
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)

        ret_val = ICF_FAILURE;

    }  /* if(ICF_FAILURE == ret_val) */

   ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
            p_cc_pdb->p_ecode, dummy_retval)

   ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
   return ret_val;
}


icf_return_t icf_cc_send_pa_stop_fax_resp(
              INOUT icf_cc_pdb_st *p_cc_pdb)
{
  	icf_return_t                     ret_val = ICF_SUCCESS;
  	icf_return_t                     dummy_retval = ICF_SUCCESS;
  	icf_msg_st                       *p_pa_msg = ICF_NULL;
  	icf_stop_fax_resp_st *p_stop_fax_resp = ICF_NULL;
  	icf_uint16_t                      msg_len = 0;

  	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
  	msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_stop_fax_resp_st);

  	/*Allocate memory for API header strucure */
  	ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
       			ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)
  
	p_pa_msg->hdr.api_id = ICF_STOP_FAX_RESP;
  	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
  	p_pa_msg->hdr.api_length = msg_len;
  	p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
  	p_stop_fax_resp = (icf_stop_fax_resp_st *)(p_pa_msg->payload);
	
	if(ICF_NULL == 
        ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL))
	{
		p_stop_fax_resp->result = ICF_TRUE;
	}
	else
	{
		ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)

		p_stop_fax_resp->result = ICF_FALSE;

        if(ICF_NULL != p_cc_pdb->p_call_ctx->app_failure_cause)
        {
		    p_stop_fax_resp->error_cause = p_cc_pdb->p_call_ctx->app_failure_cause;
        }
		p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;
	}

  
  	p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_STOP_FAX_RESP;
        p_cc_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cc_pdb->p_call_ctx->call_id;
  	p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;
  
	/*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
  	p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
  	ICF_CC_TRACE(p_cc_pdb, ICF_STOP_FAX_RESP)
#if ICF_TRACE_ENABLE
    /* Added to send traces to SM module */
    p_cc_pdb->result = p_stop_fax_resp->result;
    p_cc_pdb->resp_code = p_stop_fax_resp->error_cause;
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STOP_FAX_RESP_TO_APP);
#endif 
  
        if((ICF_FALSE == p_stop_fax_resp->result) && 
           (ICF_NULL != p_cc_pdb->p_call_ctx->retry_after_duration))
        {  
            p_stop_fax_resp->retry_after_duration = 
                  p_cc_pdb->p_call_ctx->retry_after_duration;
            p_cc_pdb->p_call_ctx->retry_after_duration = ICF_NULL;
            p_stop_fax_resp->bit_mask |=
               ICF_RETRY_AFTER_DURATION_PRESENT;
        }

	/*Call icf_adm_process_mesg(p_internal_msg)*/
  	ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);
    ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,  ICF_FAX_CALL_ESTABLISHED);
	p_cc_pdb->p_call_ctx->fax_type = ICF_NULL;
	
	p_cc_pdb->p_call_ctx->call_modify_method_bitmask  &=
		~(ICF_METHOD_STOP_FAX_REQ);

	/*If process msg return fail*/
  	if(ICF_FAILURE == ret_val)
  	{
   		/*Deallocate API header structure*/
   		ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
  	}/* if(ICF_FAILURE == ret_val) */
        
          /* Free the header list */
        if ((ICF_SUCCESS == ret_val) &&
                (ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
        {
             /* free the list in call ctx */
             ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
                           &(p_cc_pdb->p_call_ctx->header_list),
                           p_cc_pdb->p_ecode);
        }

        /* Free the body list */
        if ((ICF_SUCCESS == ret_val) &&
                (ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
        {
             /* free the list in call ctx */
             ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
                           &(p_cc_pdb->p_call_ctx->body_list),
                           p_cc_pdb->p_ecode);
        }
  
   
	ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
      			p_cc_pdb->p_ecode, dummy_retval)
  	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
  	return ret_val;
}  

icf_return_t icf_cc_send_pa_stop_fax_ind(
              INOUT icf_cc_pdb_st *p_cc_pdb)
{
  	icf_return_t                     ret_val = ICF_SUCCESS;
  	icf_return_t                     dummy_retval = ICF_SUCCESS;
  	icf_msg_st                       *p_pa_msg = ICF_NULL;
  	icf_stop_fax_ind_st *p_stop_fax_ind = ICF_NULL;
  	icf_uint16_t                      msg_len = 0;

  	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
  	msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_stop_fax_ind_st);

  	/*Allocate memory for API header strucure */
  	ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
       			ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)
  
	p_pa_msg->hdr.api_id = ICF_STOP_FAX_IND;
  	p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
  	p_pa_msg->hdr.api_length = msg_len;
  	p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
  	p_stop_fax_ind = (icf_stop_fax_ind_st *)(p_pa_msg->payload);
  
	p_stop_fax_ind->fax_type = p_cc_pdb->p_call_ctx->fax_type;
  	p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_STOP_FAX_IND;
  	p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;
  
	/*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
  	p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
  	ICF_CC_TRACE(p_cc_pdb, ICF_STOP_FAX_RESP)

  
	/*Call icf_adm_process_mesg(p_internal_msg)*/
  	ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);
    ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,  ICF_FAX_CALL_ESTABLISHED);
#if ICF_TRACE_ENABLE
    /* Added to send traces to SM module */
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STOP_FAX_TO_APP);
#endif
  
	/*If process msg return fail*/
  	if(ICF_FAILURE == ret_val)
  	{
   		/*Deallocate API header structure*/
   		ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
  	}/* if(ICF_FAILURE == ret_val) */
 
         /* Free the header list */
        if ((ICF_SUCCESS == ret_val) &&
                (ICF_NULL != p_cc_pdb->p_call_ctx->header_list.count))
        {
             /* free the list in call ctx */
             ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
                           &(p_cc_pdb->p_call_ctx->header_list),
                           p_cc_pdb->p_ecode);
        }

        /* Free the body list */
        if ((ICF_SUCCESS == ret_val) &&
                (ICF_NULL != p_cc_pdb->p_call_ctx->body_list.count))
        {
             /* free the list in call ctx */
             ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb,
                           &(p_cc_pdb->p_call_ctx->body_list),
                           p_cc_pdb->p_ecode);
        }
 
	ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
      			p_cc_pdb->p_ecode, dummy_retval)
  	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
  	return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_fax_resp_invalid_callid
 *
 * DESCRIPTION:     This function is used to send a call termination
 *                  response to the PA when PA sends fax_req.
 *                  This request will come with an invalid call-id, CC will 
 *                  simply send a fax_resp with the same call-id as was 
 *                  sent by PA in fax_req.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_fax_resp_invalid_callid(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint16_t                      msg_len = 0;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
	icf_start_fax_resp_st           *p_start_fax_resp = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_start_fax_resp_st);

    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
            ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    /* Fill the message header for ICF_CC_PA_START_FAX_RESP */
    p_pa_msg->hdr.api_id = ICF_CC_PA_START_FAX_RESP;
    p_pa_msg->hdr.api_length = msg_len;
    p_pa_msg->hdr.call_id = p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
    p_pa_msg->hdr.app_id = p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

    /*Fill header in internal message structure*/
    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_START_FAX_RESP;

    /* Fill the payload for ICF_START_FAX_RESP */
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = msg_len;
    
	p_start_fax_resp = (icf_start_fax_resp_st *)(p_pa_msg->payload);
	p_start_fax_resp->result = ICF_FALSE;
	p_start_fax_resp->error_cause = ICF_CAUSE_INVALID_CALL_ID;
	p_start_fax_resp->fax_type = 
		((icf_start_fax_req_st *)(p_cc_pdb->p_glb_pdb->p_recv_msg->payload))->fax_type;

    /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

     /*Deallocate API header structure*/
     ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode, dummy_retval)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
} /* icf_cc_send_pa_fax_resp_invalid_callid() */ 


/*call_mod_cancel*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_modify_cancel_resp
 *
 * DESCRIPTION:     This function is used to send response for cancel of 
 *                  call modify request to PA.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_call_modify_cancel_resp(
              INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_call_modify_cancel_resp_st *p_call_modify_cancel_resp = ICF_NULL;
    icf_uint16_t                      msg_len = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_call_modify_cancel_resp_st);

    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
                ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

    if(ICF_FAILURE != ret_val)
    {
        p_pa_msg->hdr.api_id = ICF_CALL_MODIFY_CANCEL_RESP;
        p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
        p_pa_msg->hdr.api_length = msg_len;
        p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;
        p_call_modify_cancel_resp = (icf_call_modify_cancel_resp_st *)(p_pa_msg->payload);

        if(ICF_NULL ==
            ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL))
        {
            p_call_modify_cancel_resp->result = ICF_TRUE;
        }
        else
        {
            ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

            p_call_modify_cancel_resp->result = ICF_FALSE;

            if(ICF_NULL != p_cc_pdb->p_call_ctx->app_failure_cause)
            {
                p_call_modify_cancel_resp->error_cause = p_cc_pdb->p_call_ctx->app_failure_cause;
            }
            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;
        }


        p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CALL_MODIFY_CANCEL_RESP;
        p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;

        /*Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st*/
        p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_pa_msg;
        ICF_CC_TRACE(p_cc_pdb, ICF_CALL_MODIFY_CANCEL_RESP)

        /*Call icf_adm_process_mesg(p_internal_msg)*/
        ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);
        ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,  ICF_CALL_MOD_CANCEL_REQ);
        ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,  ICF_CALL_MOD_CANCEL_REINV_REQ);

        /*If process msg return fail*/
        if(ICF_FAILURE == ret_val)
        {
            /*Deallocate API header structure*/
            ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
        }/* if(ICF_FAILURE == ret_val) */

        ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode, dummy_retval)
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    }
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_progress_ind
 *
 * DESCRIPTION:     This function is used to send a call progress message 
 *                  indication to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_progress_ind(
        INOUT   icf_cc_pdb_st   *p_cc_pdb,
        IN      icf_uint16_t    response_code)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_msg_st                      *p_pa_msg = ICF_NULL;
    icf_uint16_t                     msg_len = 0;
    icf_return_t                    dummy_retval = ICF_SUCCESS;
  	icf_progress_ind_st 			*p_progress_ind = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    msg_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
				ICF_PORT_SIZEOF(icf_progress_ind_st);

    /* Allocate memory for API header strucure */
    ICF_MEMGET(p_cc_pdb->p_glb_pdb, msg_len, ICF_MEM_COMMON, p_pa_msg,
                ICF_RET_ON_FAILURE, p_cc_pdb->p_ecode, ret_val)

	/* Fill the message header */
    p_pa_msg->hdr.api_id = ICF_PROGRESS_IND;
    p_pa_msg->hdr.app_id = p_cc_pdb->p_call_ctx->app_id;
    p_pa_msg->hdr.api_length = msg_len;
    p_pa_msg->hdr.call_id = p_cc_pdb->p_call_ctx->call_id;

	/* Fill the message payload */
    p_progress_ind = (icf_progress_ind_st *)(p_pa_msg->payload);
    p_progress_ind->response_code = response_code;

    /* Assign p_pa_msg to p_pa_msg_data of icf_internal_msg_st */
    p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_PROGRESS_IND;
    p_cc_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)msg_len;
    p_cc_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_pa_msg;

    /* Call icf_adm_process_mesg(p_internal_msg) */
    ret_val = icf_adm_process_mesg(p_cc_pdb->p_internal_msg);

    if(ICF_FAILURE == ret_val)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_INTERFACE_FAILURE)
    }/* if(ICF_FAILURE == ret_val) */

    ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_pa_msg, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, dummy_retval)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}



