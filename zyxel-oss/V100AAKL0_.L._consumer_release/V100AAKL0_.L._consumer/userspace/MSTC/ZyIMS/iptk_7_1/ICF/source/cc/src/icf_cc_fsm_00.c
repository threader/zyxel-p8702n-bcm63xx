/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_00.c
 *
 * Purpose          : This file contains the event handlers in ICF_CC_00_IDLE
 *                    state.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 14-Dec-2004  Umang Singh     Non Service LLD Initial
 * 21-Dec-2004  Rohit Aggarwal                  Addition of traces
 * 24-Dec-2004  Rohit Aggarwal                  Compilation fixes
 * 24-Feb-2004  Sumit Gupta                     Changes for CFW and CW service 
 *                                              implementation
 * 25-Feb-2004  Sumit Gupta                     Updated the generic error 
 *                                              handling
 * 20-Apr-2005  Sumit Gupta      SPR 7366       Set status as 
 *                                              ICF_STATUS_CONTINUE in return
 *                                              path for FORCEFUL_CLEAR and 
 *                                              Generic Error
 *
 * 04-May-2005  Pankaj Negi      SPR 7491    
 * 06-May-2005  Sumit Gupta      SPR 7438      Reset call clear status NW 
 *                                             cleared when setup received from 
 *                                             SSA and reset call clear status
 *                                             PA when PA setup req is received
 * 10-May-2005 Sumit Gupta       SPR 7533      Would not send reject from SSA 
 *                           
 setup this would be done on 
 *                                             receiving generic_error
 * 17-May-2005 Sumit Gupta       SPR 7570      Updated function 
 *                                               icf_cc_pa_setup_req
 *  07-Jul-2005 Sumit Gupta                     Updated the check for CC_CLEARED
 *                                              flag
 * 10-Aug-2005	Rohit Aggarwal	ICF				Code changes specific to ICF
 * 06-Jan-2006	Jyoti Narula	SPR 9125		In the icf_cc_00_pa_setup_req * 
 * 08-Mar-2006  Aman Aggarwal					ICF merging with ICF2.0
 * 02-Apr-2006	Puneet Sharma	ICF5.0 LLD		changes done in the comments 
 *												Multiple Public Users ICF5.0
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs    Structure changes for multiple
 *                                              m-lines
 * 01-Sep-2006	Amit Sharma     SPR:12714
 * 03-Jan-2007  Amit Mahajan                    INFO confirmation changes.
 * 02-Feb-2007  Umang Singh     SPR 14438
 * 04-Apr_2007  Rajat Chandna                   Changes for wildcarded user
 *                                              enhancement.
 *
 * 27-Apr-2007  Kamini Gangwani SPR 14917       Mode change to INACTIVE for outgoing/incoming  *                                              call.
 * 21-Jun-2007  Tarun Gupta     SPR 15149       Change in function
 *                                              icf_cc_00_clear_call:   
 *                                              Stopped info cfm timer in case 
 *                                              of forceful call clear
 *
 * 31-Dec-2008  Vinay Jindal    SPR 19248       In case of Invite with replaces, 
 *                                              set mm_applied_mode as sendrecv
 * 09-Feb-2009   Rohit Aggarwal  SPR 19316   Changes for INVITE with Replaces
 * 13-Jul-2009   Anurag Khare    CSR 1-7505190  Changes done in function
 *                                              icf_cc_00_pa_setup_req
 * 
 * Copyright (c) 2009, Aricent.
 *---------------------------------------------------------------------------*/


#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_dbm_prototypes.h"
#include "icf_port_prototypes.h"
ICF_SESSION_CONTROL_START
#include "icf_regm_defs.h"
#include "icf_regm_prototypes.h"
#include "icf_dbm_regm_prototypes.h"
ICF_SESSION_CONTROL_END


#include "icf_internal_api_id.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif
/******************************************************************************
 *
 * FUNCTION:        icf_cc_00_pa_setup_req
 *
 * DESCRIPTION:     This function is invoked when the CC is
 *                  in IDLE state and it receives a setup
 *                  request from PA
 *                  Current State: ICF_CC_00_IDLE
 *                  Current Event: ICF_CC_PA_SETUP_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_00_pa_setup_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_pa_cc_create_call_req_st *p_call_setup = ICF_NULL;
	icf_boolean_t call_thru_proxy = ICF_TRUE;

	icf_line_data_st *p_line_data = ICF_NULL;
    icf_uint8_t      param_val[5] = {0,0,0,0,0};

#ifdef ICF_QOS_SUPPORT
    icf_config_data_st  *p_config_data = ICF_NULL;
#endif
	icf_user_db_node_st			*p_user_db = ICF_NULL;
    icf_internal_list_st	     *p_stream_sdp_info_node = ICF_NULL;
    icf_stream_sdp_info_st           *p_stream_sdp_data = ICF_NULL;
    icf_boolean_t                    atleast_one_hit = ICF_FALSE;
    icf_config_data_st           *p_cfg_data = ICF_NULL;
    icf_line_id_t		 line_id;
    icf_user_db_node_st          *p_usr_db_node = ICF_NULL;
    icf_boolean_t                 wildcard_usr_on_line = ICF_FALSE;
    /* This will be populated according to create_call or create_xconf
       request bitmask coming from PA */
    icf_uint32_t           request_bitmask = 0;
    icf_address_st         request_calling_party_address = {{0, "\0"},0};
    icf_string_st          request_calling_party_name = {0, "\0"};
    icf_boolean_t          request_toggle_pres_ind = ICF_FALSE; 
#ifdef ICF_SESSION_TIMER
    icf_session_timer_data_st      *p_session_timer_data = ICF_NULL;
#endif
 
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SETUP_RECVD_FRM_PA)

    ICF_ASSERT(p_cc_pdb->p_glb_pdb,p_cc_pdb->p_glb_pdb->p_recv_msg)

	if ( ICF_FAILURE ==
			icf_dbm_hash_add_call_ctxt(p_cc_pdb->p_glb_pdb,p_cc_pdb->p_call_ctx->call_id,
								p_cc_pdb->p_call_ctx,p_cc_pdb->p_ecode))
	{
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ICF_FAILURE;
	}

	/* Fix for CSR 1-6727350: SPR 19316 */
	/* For handling INVITE with Replaces, reset NW_CLR flag.
	 * For handling PA setup request, reset PA_CLR flag.
	 */
	/* Condition of ICF_TRUE == p_cc_pdb->p_config_data->app_connect_reqd_for_replaces
 	 * has been removed beacuse if it is a replace init call then 
	 * ICF_CALL_CLEAR_STATUS_NW_CLEARED must be reset, irrespective of the value
	 * of p_cc_pdb->p_config_data->app_connect_reqd_for_replaces.
	 */
	
    if (ICF_INVALID_CALL_ID != 
			p_cc_pdb->p_call_ctx->replace_initiated_call_id)
	{
    	/* Reset the NW_CLEAR flag in call_clear_status in call context*/
    	ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb, ICF_CALL_CLEAR_STATUS_NW_CLEARED)
	}
    	/*Reset the PA_CLEAR flag in call_clear_status*/
    	ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb, ICF_CALL_CLEAR_STATUS_PA_CLEARED)

    if (ICF_FAILURE == icf_dbm_get_module_glb_data(p_cc_pdb->p_glb_pdb, 
               ICF_INT_MODULE_CFG,
               (icf_void_t *)&p_cfg_data, p_cc_pdb->p_ecode))
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ICF_FAILURE;
    }   
     /*SPR 13978 Aman 03-01-07*/
       /* Check added for XCONF REL 7.0 */
    if(ICF_FALSE == p_cc_pdb->refer_init_call && 
	   (!(ICF_CC_EXTERNAL_CONFERENCE_CALL &
          p_cc_pdb->p_call_ctx->common_bitmask_2)))
    {
       line_id = ((icf_pa_cc_create_call_req_st*)p_cc_pdb->p_glb_pdb->p_recv_msg->payload)
                                                             ->line_id;
       request_bitmask = ((icf_pa_cc_create_call_req_st *) \
           (p_cc_pdb->p_glb_pdb->p_recv_msg->payload))->bit_mask;
       request_calling_party_address =((icf_pa_cc_create_call_req_st *) \
           (p_cc_pdb->p_glb_pdb->p_recv_msg->payload))->calling_party_address;
       request_calling_party_name =((icf_pa_cc_create_call_req_st *) \
           (p_cc_pdb->p_glb_pdb->p_recv_msg->payload))->calling_party_name;
       request_toggle_pres_ind =((icf_pa_cc_create_call_req_st *) \
           (p_cc_pdb->p_glb_pdb->p_recv_msg->payload))->toggle_presentation_indicator;
    }
    else
    {
       line_id = p_cc_pdb->p_call_ctx->line_id;
       if(ICF_CC_EXTERNAL_CONFERENCE_CALL &
            p_cc_pdb->p_call_ctx->common_bitmask_2)
       {
         request_bitmask = ((icf_pa_cc_create_ext_xconf_req_st *) \
             (p_cc_pdb->p_glb_pdb->p_recv_msg->payload))->bitmask;
         request_calling_party_address =((icf_pa_cc_create_ext_xconf_req_st*) \
             (p_cc_pdb->p_glb_pdb->p_recv_msg->payload))->calling_party_address;
         request_calling_party_name =((icf_pa_cc_create_ext_xconf_req_st *) \
             (p_cc_pdb->p_glb_pdb->p_recv_msg->payload))->calling_party_name;
         request_toggle_pres_ind =((icf_pa_cc_create_ext_xconf_req_st *) \
           (p_cc_pdb->p_glb_pdb->p_recv_msg->payload))->toggle_presentation_indicator;
       }
       else
       {    
            ICF_CC_APP_TRACE(p_cc_pdb, ICF_TRACE_CALL_INITIATED_FOR_INC_REFER) 
       }         
    }
     if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
             p_cc_pdb->p_glb_pdb, line_id,
                &p_line_data, p_cc_pdb->p_ecode))
     {
         ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
         return ICF_FAILURE;
     }
    /*SPR 13978 Aman 03-01-07*/
     if(ICF_FAILURE == icf_cmn_validate_open_nw_server(p_cc_pdb->p_glb_pdb,line_id))
     {    /* set release cause and reason for failure as license expiry */
	     ICF_CC_SET_ECODE(p_cc_pdb, ICF_ECODE_GENERIC_ERROR)
		     ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)

		     ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
				     ICF_CALL_CLEAR_REASON_OPEN_NW_SERVER_FAILED)

		     /* set the line ID and the APP ID in the cc_pdb_ctx, so that the error 
		      * message can be routed to the correct PA */
         p_cc_pdb->p_call_ctx->app_id =
                p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

         p_cc_pdb->p_call_ctx->line_id =
                ((icf_pa_cc_create_call_req_st *)
                    p_cc_pdb->p_glb_pdb->p_recv_msg->payload)->line_id ;
         p_cc_pdb->p_call_ctx->call_id =
                        p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.call_id ;

                ret_val = ICF_FAILURE;
    }
    else if((ICF_FALSE == p_cc_pdb->refer_init_call) && 
	        (!(ICF_CC_EXTERNAL_CONFERENCE_CALL & 
               p_cc_pdb->p_call_ctx->common_bitmask_2)))
    {
	/* IN case call is not due to service initiation req
        * we need to get call context else these field will be
        * filled by Refer slp */

        p_call_setup = (icf_pa_cc_create_call_req_st*)
                p_cc_pdb->p_glb_pdb->p_recv_msg->payload;
	/*Copying pointer of pref media profile received into call ctx.*/
	p_cc_pdb->p_call_ctx->p_pref_media_profile=p_call_setup->p_pref_media_profile;
	p_call_setup->p_pref_media_profile = ICF_NULL;
        /* copy the app_id in received message into call ctx */
        p_cc_pdb->p_call_ctx->app_id = 
                p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

        if (ICF_CFG_SELF_ADDR_PER_LINE & p_cfg_data->self_addr_per_line)
        {
            p_call_setup = (icf_pa_cc_create_call_req_st*)
                    p_cc_pdb->p_glb_pdb->p_recv_msg->payload;

            if (ICF_FALSE == p_line_data->line_data_init_complete)
            {
                /*
                 * Init Complete has not occurred, therefore return failure
                 */
                ICF_CC_SET_ECODE(p_cc_pdb, ICF_MSG_PROC_INIT_NOT_COMPLETE)
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return (ICF_FAILURE);
            }
        }
#ifndef ICF_UNIT_TEST
        else
        {
             /* Check whether init complete has occurred or not */
             if (ICF_FALSE == p_cc_pdb->p_glb_pdb->p_glb_data->
                              global_data_init_complete)
             {
                 /*
                  * Init Complete has not occurred, therefore return failure
                  */
                 ICF_CC_SET_ECODE(p_cc_pdb, ICF_MSG_PROC_INIT_NOT_COMPLETE)
                 ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                 return (ICF_FAILURE);
             }
        }
#endif
	   
	if( ICF_FAILURE == icf_cmn_validate_called_party_addr(
                 p_cc_pdb->p_glb_pdb,
                 &(p_call_setup->called_party_addr),
                 p_cc_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
        }

        /* 
         * Check if mode is present in called party address and if it is present
         * then validate it against the proxy mode
         */
        else if(ICF_TRUE == icf_cmn_check_if_param_present(
            &(p_call_setup->called_party_addr.addr_val),(icf_uint8_t*) "transport", param_val, 
            (icf_uint8_t*)"To", &(p_call_setup->tag_list)))
        {
            call_thru_proxy = icf_dbm_chk_ogn_call_thru_proxy(
                    p_cc_pdb->p_glb_pdb,        
                    p_cc_pdb->p_ecode);

            if(ICF_FAILURE == icf_dbm_validate_mode_ogn_call(
                    p_cc_pdb->p_glb_pdb,        
                    param_val,
                    p_call_setup->line_id,
                    call_thru_proxy,
                    p_cc_pdb->p_ecode))
            {
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                        ICF_CALL_CLEAR_REASON_INCONSISTENT_TRANSPORT_MODE)
                ret_val = ICF_FAILURE;
            }
        }

        /* If the address validation was successfull the copy the 
         * called_party_addr to remote_party_addr in call context*/
        if(ICF_SUCCESS == ret_val)
        {
            icf_port_memcpy(
                    (icf_void_t*)(&p_cc_pdb->p_call_ctx->remote_party_addr),
                    (icf_void_t*)(&p_call_setup->called_party_addr),
                    (icf_uint32_t)sizeof(icf_address_st));
        }



ICF_SESSION_CONTROL_START
		/* validate the app_id */
		if (ICF_FAILURE == icf_cfg_get_app_id_validity(
                                    p_cc_pdb->p_glb_pdb,
                                    p_cc_pdb->p_call_ctx->app_id))
		{
			ICF_CC_SET_ECODE(p_cc_pdb, ICF_ECODE_GENERIC_ERROR)

			ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, 
				ICF_REL_TYPE_INTERNAL_INITIATED)

			ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
				ICF_CAUSE_INVALID_APP_ID)
		
			return ICF_FAILURE;
		}

#ifdef ICF_NW_ACTIVATE
		/* validate that the network is not down */
		if (ICF_FAILURE == icf_cfg_get_network_status(
                                    p_cc_pdb->p_glb_pdb,
                                    p_cc_pdb->p_call_ctx->app_id))
		{
			ICF_CC_SET_ECODE(p_cc_pdb, ICF_ECODE_GENERIC_ERROR)

			ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, 
				ICF_REL_TYPE_INTERNAL_INITIATED)

			ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
				ICF_CAUSE_NW_CURRENTLY_NOT_AVAILABLE)
		
			return ICF_FAILURE;
		}
#endif

ICF_SESSION_CONTROL_END
        /* For outgoing call we need to  validate the parameters in
         * called party address*/
        
        if(ICF_FAILURE == ret_val) 
        {
            ICF_CC_SET_ECODE(p_cc_pdb,ICF_ERROR_INVALID_TRNSPT_ADDR)
        }
    }  
    if((ICF_FALSE == p_cc_pdb->refer_init_call) && 
       (ICF_SUCCESS == ret_val)) 
    {   
       /*attaching line to CC for XCONF request*/ 
       /* If validation is succesful. we will ask dbm to validate line id */
        if(ICF_FAILURE == icf_dbm_validate_line_for_ogn_call(
                p_cc_pdb->p_glb_pdb, line_id,
                p_cc_pdb->p_ecode))
        {
                /* For all failures before call create, release the call */
                ret_val = ICF_FAILURE;
                /* Set this release cause if no other cause is set
                 * The macro takes care no release cause is overwritten*/
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
                    ICF_CALL_CLEAR_REASON_LINE_BUSY)
        }
        if ((ICF_SUCCESS == ret_val) && (ICF_NULL != p_line_data->p_user_db))
        {
            if(ICF_FAILURE == icf_dbm_attach_cc_to_line(
                p_cc_pdb->p_glb_pdb, line_id,
                p_cc_pdb->p_call_ctx,p_cc_pdb->p_ecode))
            {
                /* For all failures before call create, release the call */
                ICF_SESSION_CONTROL_START
                ICF_CC_SET_ECODE(p_cc_pdb, ICF_ECODE_GENERIC_ERROR)
                    
                ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, 
                ICF_REL_TYPE_INTERNAL_INITIATED)
                
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
                ICF_CALL_CLEAR_REASON_USER_NOT_REGISTERED)
                ICF_SESSION_CONTROL_END
                
                ret_val = ICF_FAILURE;
            }
         }
         else if (ICF_SUCCESS == ret_val)
         {
             if(ICF_FAILURE == icf_dbm_attach_cc_to_line_wo_user(
                p_cc_pdb->p_glb_pdb, line_id,
                p_cc_pdb->p_call_ctx,p_cc_pdb->p_ecode))
             {
                 ret_val = ICF_FAILURE;
             }
         }
        if (ICF_FAILURE == ret_val)
        {/*Do nothing*/
        }
        /*If calling party address present then fill it in local address*/
        else if (ICF_CALLING_PARTY_ADDR & request_bitmask)
        {
            
                ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(
				p_cc_pdb->p_glb_pdb,
			    line_id,
				&(request_calling_party_address),
				ICF_USER_DB_USE_PUBLIC_URIS | 
                        ICF_USER_DB_USE_ASSOCIATED_URIS, /*so that searching done in assoc uris also*/
				&p_user_db,
				p_cc_pdb->p_ecode);

                if (ICF_FAILURE == ret_val)
                {
                    /*User not exist on line*/
                    /* regm_ctx does not exist, or user is not registered */
                    for(p_usr_db_node = p_line_data->p_user_db;
                        ICF_NULL != p_usr_db_node;
                        p_usr_db_node = p_usr_db_node->p_next)
                       {
                         if ( icf_port_strncmp(p_usr_db_node->public_user.addr_val.str, \
                                  (icf_uint8_t*)ICF_WILDCARD_USER, icf_port_strlen((icf_uint8_t*)ICF_WILDCARD_USER)) == 0)
                           {
                             ret_val = ICF_SUCCESS;
                             wildcard_usr_on_line = ICF_TRUE;
                             break;
                           }

                       }
                    if(ICF_FALSE == wildcard_usr_on_line)
                      {
                        ICF_CC_SET_ECODE(p_cc_pdb, ICF_ECODE_GENERIC_ERROR)
                    
                        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, 
                        ICF_REL_TYPE_INTERNAL_INITIATED)
                        
                        ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
                        ICF_CALL_CLEAR_REASON_USER_NOT_FOUND)
                   
                        ret_val = ICF_FAILURE;
                      }
                }
            
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)	
            /* if contact_uri_list is null that means the user is not registered*/
            if ((ICF_FAILURE == ret_val) || 
                ((ICF_SUCCESS == ret_val) && 
                (ICF_NULL == p_user_db->p_contact_uri_list)))
                /* End: Multiple Public Users ICF5.0 */
            {
                /* regm_ctx does not exist, or user is not registered */
                ICF_CC_SET_ECODE(p_cc_pdb, ICF_ECODE_GENERIC_ERROR)
                    
                ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, 
                ICF_REL_TYPE_INTERNAL_INITIATED)
                    
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
                ICF_CALL_CLEAR_REASON_USER_NOT_REGISTERED)
                   
                ret_val = ICF_FAILURE;
            }
ICF_CHECK_IMS_END
#endif

            if (ICF_FAILURE != ret_val)
            {
                icf_port_memcpy(( icf_void_t*)&(p_cc_pdb->p_call_ctx->local_address),
                    ( icf_void_t*)&(request_calling_party_address),
                    (icf_uint32_t)sizeof(icf_address_st));
                
                if (ICF_CALLING_PARTY_NAME & request_bitmask)
                {
                    icf_port_memcpy(( icf_void_t*)&(p_cc_pdb->p_call_ctx->local_name),
                        ( icf_void_t*)&(request_calling_party_name),
                        (icf_uint32_t)sizeof(icf_string_st));
                    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_LOCAL_NAME_PRESENT)
                }
            }        
        }
        else if (ICF_NULL != p_line_data->p_user_db)
        {      /* If only wildcaeded user is conf. on line */
               if (!icf_port_strncmp(p_line_data->p_user_db->public_user.addr_val.str, \
                                     (icf_uint8_t*)ICF_WILDCARD_USER, icf_port_strlen((icf_uint8_t*)ICF_WILDCARD_USER)) &&
                                     (ICF_NULL == p_line_data->p_user_db->p_next))
                 {
                   ret_val = icf_cc_set_annonymous_local_addr(p_cc_pdb);
                 }
               else
                 { 
                    /* if a user and wildcaeded user are conf. on line 
                     * and user is conf. first */
                    if (icf_port_strncmp(p_line_data->p_user_db->public_user.addr_val.str, \
                                     (icf_uint8_t*)ICF_WILDCARD_USER, icf_port_strlen((icf_uint8_t*)ICF_WILDCARD_USER)))
                       {
                         
                         icf_port_memcpy(( icf_void_t*)&(p_cc_pdb->p_call_ctx->local_address),
                         ( icf_void_t*)&(p_line_data->p_user_db->public_user),
                         (icf_uint32_t)sizeof(icf_address_st));
                         if(ICF_NULL != p_line_data->user_name.str)
                           {
                             icf_port_strcpy(p_cc_pdb->p_call_ctx->local_name.str,
                                              p_line_data->user_name.str);
                             p_cc_pdb->p_call_ctx->local_name.str_len =
                                              p_line_data->user_name.str_len;
                             ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_LOCAL_NAME_PRESENT)
 
                           }
                       }
                     /* user and wildcarded user are conf,and wildcarded
                      * is configured first */
                     else
                       {
                         icf_port_memcpy(( icf_void_t*)&(p_cc_pdb->p_call_ctx->local_address),
                                 ( icf_void_t*)&(p_line_data->p_user_db->p_next->public_user),
                                 (icf_uint32_t)sizeof(icf_address_st));
                         if(ICF_NULL != p_line_data->user_name.str)
                           {
                             icf_port_strcpy(p_cc_pdb->p_call_ctx->local_name.str,
                                              p_line_data->user_name.str);
                             p_cc_pdb->p_call_ctx->local_name.str_len =
                                              p_line_data->user_name.str_len; 
                           }
                       }
                 }
        }


     	if((ICF_PA_CC_CREATE_CALL_REQ_TOGGLE_PRESENTATION_INDICATOR
                        & request_bitmask) &&
                   (ICF_TRUE == request_toggle_pres_ind))
		{
             p_cc_pdb->p_call_ctx->common_bitmask =
                    p_cc_pdb->p_call_ctx->common_bitmask ^ ICF_TOGGLE_PI;
        }
    }

    /* Fetch the line context block for this line_id */
	   if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
							p_cc_pdb->p_glb_pdb, 
							p_cc_pdb->p_call_ctx->line_id,
							&p_line_data, 
							p_cc_pdb->p_ecode))
	     {
				 ret_val = ICF_FAILURE;
				 ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
				 return ret_val;
		 }

#ifdef ICF_SESSION_TIMER
             if (ICF_TRUE == p_cc_pdb->p_call_ctx->session_timer_enable)
             {
	             ICF_MEMGET(p_cc_pdb->p_glb_pdb,
	                        ICF_PORT_SIZEOF(icf_session_timer_data_st),
		                    ICF_MEM_COMMON,
            		        p_session_timer_data,
        		            ICF_RET_ON_FAILURE,
		                    p_cc_pdb->p_ecode,
		                    ret_val)       

                 if ((ICF_NULL != p_session_timer_data) && 
                     (ICF_NULL != p_line_data))
                 {
                    p_session_timer_data->session_timer = p_line_data->session_timer;
                    p_session_timer_data->min_se = p_line_data->min_se;

                    p_session_timer_data->session_refresher_method = 
                                          p_cfg_data->call_modify_req_method;

                    p_session_timer_data->session_timer_refresher = 
                                          p_line_data->session_timer_refresher;

                    p_session_timer_data->session_refresher_reqd = 
                                          p_cfg_data->session_refresher_reqd;

                    p_session_timer_data->session_timer_logic = 
                                          p_cfg_data->session_timer_logic;

                    p_cc_pdb->p_call_ctx->p_session_timer_data = p_session_timer_data;
                 }
                 else
                 {
                    if (ICF_NULL != p_session_timer_data)
                    {
                        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                                    p_session_timer_data,
                    				ICF_MEM_COMMON,
                                    p_cc_pdb->p_ecode,
				                    ret_val)
                    }  
                    return ICF_FAILURE;
                 }   
             }
#endif



    if((ICF_FALSE == p_cc_pdb->refer_init_call) && 
	        (!(ICF_CC_EXTERNAL_CONFERENCE_CALL & 
               p_cc_pdb->p_call_ctx->common_bitmask_2)))
    {    
        /* Now just update call context with valid parameters */        
        if(ICF_SUCCESS == ret_val)
        {
            if (ICF_NULL == p_call_setup)
            {
               return ICF_FAILURE;
            }  
            if(ICF_PA_CC_CREATE_CALL_REQ_CALLED_PARTY_NAME &
                p_call_setup->bit_mask )
            {
            /* In case remote party name is present, copy it in call ctx
                * and set the bitmask for same in call context */
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_REMOTE_NAME_PRESENT)
                    
                    icf_port_memcpy(
                    ( icf_void_t*)&(p_cc_pdb->p_call_ctx->remote_party_name),
                    ( icf_void_t*)&(p_call_setup->called_party_name),
                    (icf_uint32_t)sizeof(icf_name_st));
            }

            /*Add this to dialed call list. We wouldn't enter call  orher than 
             *those initiated throulgh create call request in dailed call list*/
#ifdef ICF_STAT_ENABLED
             icf_sm_set_dialed_call(p_cc_pdb->p_glb_pdb);
#endif

			             /* Now if bit ICF_TOGGLE_PI is set, we will give valid address
             * to remote else donot display address */
            if(ICF_TOGGLE_PI & p_cc_pdb->p_call_ctx->common_bitmask)
            {
                ICF_CC_TRACE(p_cc_pdb, ICF_TRANSPORT_ADDRESS)
            }
            else
            {
                /* Fill address type as sip url and address as annonymous call 
                 */
                icf_cc_set_annonymous_local_addr(p_cc_pdb);

ICF_SESSION_CONTROL_START
				/* privacy header needs to be sent out */
				p_cc_pdb->p_call_ctx->send_privacy_hdr = ICF_TRUE;

                /* privacy header in other requests within this dialog */
                if ((ICF_PRIVACY_HDR_FLAG_PRESENT & p_call_setup->bit_mask) &&
                    (ICF_TRUE == p_call_setup->privacy_hdr_all_req))
                {
                    p_cc_pdb->p_call_ctx->common_bitmask |= ICF_PRIVACY_HDR_ALL_REQ;
                }

ICF_SESSION_CONTROL_END
            }

            if(ICF_PA_CC_CREATE_CALL_REQ_QOS_MARK & 
                    p_call_setup->bit_mask)
            {
                /* PA can overwrite default value of qos flag */
                p_cc_pdb->p_call_ctx->qos_mark = p_call_setup->qos_mark;
            }
            else
            {
                p_cc_pdb->p_call_ctx->qos_mark =0;
            }

#ifdef ICF_QOS_SUPPORT
			/* Get pointer to config data from DBM */
			ret_val = icf_dbm_get_module_glb_data(
					p_cc_pdb->p_glb_pdb,
					(icf_int_module_id_t)ICF_INT_MODULE_CFG,
					(icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

			ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data));

			if (ICF_SUCCESS == ret_val)
			{
				if (ICF_TRUE == p_config_data->nw_res_reservation_supported)
				{
					/* check if user has requested resource reservation
					* in create_call_req API */

					/*p_call_setup->bit_mask |= ICF_PA_CC_CREATE_CALL_REQ_RES_RESERV_REQD;*/
					/*p_call_setup->resource_reservation_reqd = ICF_TRUE;*/

					if((ICF_PA_CC_CREATE_CALL_REQ_RES_RESERV_REQD &
							p_call_setup->bit_mask) && 
						(ICF_TRUE == p_call_setup->resource_reservation_reqd))
					{
						/* set the bitmask in call context */
						ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_NW_RES_RESERV_REQD)
					}
				}
			}
#endif
ICF_SESSION_CONTROL_START
			/* If preferred id is received from app, verify that it is
			 * registered, else return failure.
			 * If it is not received from app, get it from DBM
			 */
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
			/* copy the preferred identity into call ctx */
			if (ICF_PREF_IDENTITY_PRESENT & p_call_setup->bit_mask)
			{
/* Start: Multiple Public Users ICF5.0 */							
				/* return failure if the pref_id is not registered and 
				 * unbarred */
		
			ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(
						p_cc_pdb->p_glb_pdb,
						p_call_setup->line_id,
						&(p_call_setup->pref_identity),
						ICF_USER_DB_USE_ASSOCIATED_URIS, /*so that searching done in assoc uris also*/
						&p_user_db,
						p_cc_pdb->p_ecode);

			/* if contact_uri_list is null that means the user is not registered*/
			if ((ICF_FAILURE == ret_val) || 
				((ICF_SUCCESS == ret_val) && 
				(ICF_NULL == p_user_db->p_contact_uri_list)))
/* End: Multiple Public Users ICF5.0 */
				{
					/* regm_ctx does not exist, or user is not registered */
					ICF_CC_SET_ECODE(p_cc_pdb, ICF_ECODE_GENERIC_ERROR)

					ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, 
						ICF_REL_TYPE_INTERNAL_INITIATED)

					ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
						ICF_CALL_CLEAR_REASON_USER_NOT_REGISTERED)

					ret_val = ICF_FAILURE;
				}
				else
				{
					ICF_MEMGET(p_cc_pdb->p_glb_pdb,
						ICF_PORT_SIZEOF(icf_address_st),
						ICF_MEM_COMMON,
						(icf_void_t *)(p_cc_pdb->p_call_ctx->p_preferred_identity),
						ICF_RET_ON_FAILURE,
						p_cc_pdb->p_ecode,
						ret_val)
					
					icf_port_memcpy(
						(icf_void_t *)(p_cc_pdb->p_call_ctx->p_preferred_identity),
						(icf_void_t *)&(p_call_setup->pref_identity),
						(icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
				}
			} /* if (pref_id) */
			else /* get pref_id from DBM */
			{
				ICF_MEMGET(p_cc_pdb->p_glb_pdb,
					ICF_PORT_SIZEOF(icf_address_st),
					ICF_MEM_COMMON,
					(icf_void_t *)(p_cc_pdb->p_call_ctx->p_preferred_identity),
					ICF_RET_ON_FAILURE,
					p_cc_pdb->p_ecode,
					ret_val)
					
				ret_val = icf_dbm_get_preferred_identity(
							p_cc_pdb->p_glb_pdb,
                            p_cc_pdb->p_call_ctx->line_id,
                            p_cc_pdb->p_call_ctx->p_preferred_identity,
							p_cc_pdb->p_ecode);

				if (ICF_FAILURE == ret_val)
				{
					ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, 
						ICF_REL_TYPE_INTERNAL_INITIATED)

					ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
						ICF_CALL_CLEAR_REASON_USER_NOT_REGISTERED)
				}
			} /* end if (pref_id) */
ICF_CHECK_IMS_END
#endif

#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
			/* copy the preferred identity into call ctx */
			if (ICF_PREF_IDENTITY_PRESENT & p_call_setup->bit_mask)
			{
                 ICF_MEMGET(
                       p_cc_pdb->p_glb_pdb,
                       ICF_PORT_SIZEOF(icf_address_st),
                       ICF_MEM_COMMON,
                       p_cc_pdb->p_call_ctx->p_preferred_identity,
                       ICF_RET_ON_FAILURE,
                       p_cc_pdb->p_ecode,
                       ret_val)
                           
                 icf_port_strcpy(
                       p_cc_pdb->p_call_ctx->p_preferred_identity->addr_val.str,
                       p_call_setup->pref_identity.addr_val.str);

                 p_cc_pdb->p_call_ctx->p_preferred_identity->addr_val.str_len = (icf_uint16_t) 
                       icf_port_strlen(p_cc_pdb->p_call_ctx->
                            p_preferred_identity->addr_val.str);
                           
                 p_cc_pdb->p_call_ctx->p_preferred_identity->addr_type = 
                        p_call_setup->pref_identity.addr_type;
                 /* Fix for SPR 17099*/
                  p_cc_pdb->p_call_ctx->common_bitmask_2 |= ICF_PREFERRED_ID_HDR_REQD;
                 /* end fix for SPR 17099*/

#if 0
/* Start: Multiple Public Users ICF5.0 */							
				/* return failure if user not present on line */
		
				ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(
						p_cc_pdb->p_glb_pdb,
						p_call_setup->line_id,
						&(p_call_setup->pref_identity),
						ICF_USER_DB_USE_PUBLIC_URIS,
						&p_user_db,
						p_cc_pdb->p_ecode);
#endif
			}
ICF_CHECK_IMS_END
#endif
			if (ICF_SUCCESS == ret_val)
			{
				/* clone the header list into call ctx, if received from app */
				if (ICF_HDR_LIST_PRESENT & p_call_setup->bit_mask)
				{
					ret_val = icf_cmn_clone_header_list(
								p_cc_pdb->p_glb_pdb,
								&(p_call_setup->header_list),
								&(p_cc_pdb->p_call_ctx->header_list),
								p_cc_pdb->p_ecode);
				}

				/* clone the tag list into call ctx, if received from app */
				if ((ICF_SUCCESS == ret_val) && 
					(ICF_TAG_LIST_PRESENT & p_call_setup->bit_mask))
				{
					ret_val = icf_cmn_clone_tag_list(
								p_cc_pdb->p_glb_pdb,
								&(p_call_setup->tag_list),
								&(p_cc_pdb->p_call_ctx->tag_list),
								p_cc_pdb->p_ecode);
				}

				/* clone the body list into call ctx, if received from app */
				if ((ICF_SUCCESS == ret_val) && 
					(ICF_BODY_LIST_PRESENT & p_call_setup->bit_mask))
				{
					ret_val = icf_cmn_clone_body_list(
								p_cc_pdb->p_glb_pdb,
								&(p_call_setup->body_list),
								&(p_cc_pdb->p_call_ctx->body_list),
								p_cc_pdb->p_ecode);
				}
			}
ICF_SESSION_CONTROL_END

        }
    }

	else if(ICF_TRUE == icf_cmn_check_if_param_present(
        &(p_cc_pdb->p_call_ctx->remote_party_addr.addr_val), (icf_uint8_t*)"transport", param_val,
        (icf_uint8_t*)"To", ICF_NULL))
    {
        call_thru_proxy = icf_dbm_chk_ogn_call_thru_proxy(
                p_cc_pdb->p_glb_pdb,        
                p_cc_pdb->p_ecode);

        if(ICF_FAILURE == icf_dbm_validate_mode_ogn_call(
                    p_cc_pdb->p_glb_pdb,        
                    param_val,
                    p_cc_pdb->p_call_ctx->line_id,
                    call_thru_proxy,
                    p_cc_pdb->p_ecode))
        {
            ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                    ICF_CALL_CLEAR_REASON_INCONSISTENT_TRANSPORT_MODE)
                ret_val = ICF_FAILURE;
        }
    }

    if(ICF_SUCCESS == ret_val)
    {
#ifdef ICF_TRACE_ENABLE
	if(ICF_FAILURE == icf_cc_create_criteria_data(p_cc_pdb))
	{
	    ret_val = ICF_FAILURE;
        }
	else
	{
	    p_cc_pdb->p_call_ctx->p_trace_criteria_data->call_direction =
			ICF_CALL_DIRECTION_OUTGOING;
	   p_cc_pdb->p_call_ctx->p_trace_criteria_data->bitmask |=
		ICF_TRACE_CALL_DIR_PRESENT;

	   if(ICF_TRANSPORT_TYPE_UDP == p_line_data->transport_mode)
	   {
		p_cc_pdb->p_call_ctx->
			p_trace_criteria_data->call_transport_mode =
				ICF_CALL_TRANSPORT_MODE_UDP;
	   }
	   else if(ICF_TRANSPORT_TYPE_TCP == p_line_data->transport_mode)
           {
                p_cc_pdb->p_call_ctx->
                        p_trace_criteria_data->call_transport_mode =
                                ICF_CALL_TRANSPORT_MODE_TCP;
           }
	   else if(ICF_TRANSPORT_TYPE_TLS == p_line_data->transport_mode)
           {
                p_cc_pdb->p_call_ctx->
                        p_trace_criteria_data->call_transport_mode =
                                ICF_CALL_TRANSPORT_MODE_TLS;
           }
	   else
	   {
		p_cc_pdb->p_call_ctx->
                        p_trace_criteria_data->call_transport_mode =
                                ICF_CALL_TRANSPORT_MODE_UDP;
	   }
	   p_cc_pdb->p_call_ctx->p_trace_criteria_data->bitmask |=
		ICF_TRACE_TRANSPORT_MODE_PRESENT;
	}

	ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_CREATE_CALL_REQ)
        icf_cmn_trace_msg(p_cc_pdb->p_glb_pdb,p_cc_pdb->p_ecode); 
#endif

        /* Set ICF_INT_MODULE_CC in signalling ongoing flag */
        ICF_CC_SET_SIG_ONGOING(p_cc_pdb)


        /*
         * Start the setup timer
         */
        if (ICF_FAILURE == icf_cc_start_setup_timer(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* else if (ICF_FAILURE == ipp... */
		/* Fix for CSR 1-6727350: SPR 19316 */
		/* Since incoming INVITE with Replaces is received by CC as 
		 * PA setup request in this function, send 100 Trying from here if APP
		 * connect is required.
		 */
        else if ((ICF_INVALID_CALL_ID != 
					p_cc_pdb->p_call_ctx->replace_initiated_call_id) &&
				 (ICF_TRUE ==
					p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
				 (ICF_FAILURE == icf_cc_send_ssa_setup_resp(p_cc_pdb)))
        {
            ret_val = ICF_FAILURE;
        }
        /*
         * Send CMS to the MMI
         */
        else 
        {
            ret_val = icf_cc_populate_local_sdp(p_cc_pdb);
            
            if (ICF_NULL != (p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
            {
                /* Clear call bit is set.*/
                ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, 
                    ICF_REL_TYPE_INTERNAL_INITIATED);
                
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
                    ICF_CALL_CLEAR_REASON_INVALID_STREAM);
                
                ret_val = icf_cc_00_clear_call(p_cc_pdb);
                
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                    return ret_val;
            }
            p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
            while (ICF_NULL != p_stream_sdp_info_node)
            {
              p_stream_sdp_data = (icf_stream_sdp_info_st *)p_stream_sdp_info_node->p_payload;
              if (ICF_MEDIA_STREAM_ACTIVE == (ICF_MEDIA_STREAM_ACTIVE 
                                       & p_stream_sdp_data->stream_status))
              {
                  atleast_one_hit = ICF_TRUE;
              }
              p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
           }
           if (ICF_FALSE == atleast_one_hit)
           {
             ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                 ICF_CALL_CLEAR_REASON_INVALID_STREAM)

             ret_val = icf_cc_00_generic_err(p_cc_pdb);
             ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
             return ret_val;
           }

            /*  For call created by refer service, call hold is poissible and
            * mode should be set to call hold only */
            if(p_cc_pdb->p_call_ctx->media_state & ICF_MEDIA_STATE_LOCAL_HELD)
            {
                
                icf_cc_set_mm_and_app_mode(p_cc_pdb, 
                    ICF_MEDIA_MODE_INACTIVE);
            }
            else if( ICF_INVALID_CALL_ID == 
                p_cc_pdb->p_call_ctx->replace_initiated_call_id)
            {
                icf_cc_set_mm_and_app_mode(p_cc_pdb, 
                    ICF_MEDIA_MODE_RECV_ONLY);
            }
            else
            {
				/* Fix for CSR 1-6727350: SPR 19316 */
				/* For INVITE with Replaces, media mode in CMS request should
				 * be set as follows:
				 *  (1) When APP Connect is not reqd and INVITE recd with SDP,
						media mode should be "sendrecv".
				 *  (2) When APP Connect is not reqd and INVITE recd wo SDP,
						media mode should be "inactive".
				 *  (3) When APP Connect is reqd and INVITE recd with SDP,
						media mode should be "sendonly".
				 *  (4) When APP Connect is reqd and INVITE recd wo SDP,
						media mode should be "inactive".
				 */
                if ((ICF_FALSE == 
					  p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
					(ICF_CALL_MEDIA_SETUP_TYPE == ICF_CC_CHECK_COMMON_BITMASK(
					  p_cc_pdb, ICF_CALL_MEDIA_SETUP_TYPE)))
                {
                     icf_cc_set_mm_and_app_mode(p_cc_pdb, 
                         ICF_MEDIA_MODE_SEND_RECV);
                }
                else if ((ICF_FALSE == 
					  p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
					(ICF_CALL_MEDIA_SETUP_TYPE != ICF_CC_CHECK_COMMON_BITMASK(
					  p_cc_pdb, ICF_CALL_MEDIA_SETUP_TYPE)))
                {
                     icf_cc_set_mm_and_app_mode(p_cc_pdb, 
                         ICF_MEDIA_MODE_INACTIVE);
                }
                else if ((ICF_TRUE == 
					  p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
					(ICF_CALL_MEDIA_SETUP_TYPE == ICF_CC_CHECK_COMMON_BITMASK(
					  p_cc_pdb, ICF_CALL_MEDIA_SETUP_TYPE)))
                {
                     icf_cc_set_mm_and_app_mode(p_cc_pdb, 
                         ICF_MEDIA_MODE_SEND_ONLY);
                }
                else if ((ICF_TRUE == 
					  p_cc_pdb->p_config_data->app_connect_reqd_for_replaces) &&
					(ICF_CALL_MEDIA_SETUP_TYPE != ICF_CC_CHECK_COMMON_BITMASK(
					  p_cc_pdb, ICF_CALL_MEDIA_SETUP_TYPE)))
                {
                     icf_cc_set_mm_and_app_mode(p_cc_pdb, 
                         ICF_MEDIA_MODE_INACTIVE);
                }
            }
            
        }
		if (ICF_FAILURE != ret_val)
		{
			if (ICF_FAILURE == icf_cc_send_cms_req(p_cc_pdb))
			{
				ret_val = ICF_FAILURE;
			} /* else if (ICF_FAILURE == ipp... */
			else
			{  
				ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CMS_REQ_SENT)
				/*
				 * Update the states in the call context
				 */
				ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_01_AWT_CMS_RESP)
			} /* else */
		}
        if(ICF_FAILURE == ret_val)
        {
            /* Mark NW cleared bit as no need to send any indication to NW */
            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_NW_CLEARED)         

        }
    }
    
#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==ret_val)
    {
        p_cc_pdb->p_glb_pdb->load_data.otg_call_attempts++;
    }
#endif
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_00_ssa_setup_req
 *
 * DESCRIPTION:     This function is invoked when the CC is
 *                  in IDLE state and it receives a setup
 *                  request from SSA
 *                  Current State: ICF_CC_00_IDLE
 *                  Current Event: ICF_CC_SSA_SETUP_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_00_ssa_setup_req (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      trigger_cause = ICF_CAUSE_LINE_FREE;
    icf_uint32_t     status = ICF_STATUS_CONTINUE;
    icf_app_conf_st  *p_app_conf = ICF_NULL;
    icf_boolean_t    icf_simul_call_line_arbitration = ICF_FALSE;
    icf_internal_list_st	     *p_stream_sdp_info_node = ICF_NULL;
    icf_stream_sdp_info_st           *p_stream_sdp_data = ICF_NULL;
    icf_boolean_t                    atleast_one_hit = ICF_FALSE;
    icf_line_data_st    *p_line_data = ICF_NULL;
    icf_config_data_st  *p_config_data = ICF_NULL;
    icf_boolean_t				early_indication = ICF_FALSE;
#ifdef ICF_SESSION_TIMER
    icf_internal_msg_st internal_msg; 
    icf_session_timer_data_st      *p_session_timer_data = ICF_NULL;
#endif    


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /* Set the call direction flag */
    p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_DIR;

    /* Reset the NW_CLEAR flag in call_clear_status in call context*/
    ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
            ICF_CALL_CLEAR_STATUS_NW_CLEARED)

     /* If this is a new call request from NW and not a Call waiting call
     */
#ifdef ICF_SESSION_TIMER
     internal_msg.msg_hdr.msg_id = ICF_SSA_SESSION_TIMER_GET;
     internal_msg.msg_hdr.payload_length = ICF_NULL;
     internal_msg.p_msg_data = ICF_NULL;
     internal_msg.msg_hdr.p_glb_pdb = p_cc_pdb->p_glb_pdb; 
     internal_msg.msg_hdr.status = ICF_NULL; 
     internal_msg.msg_hdr.ecode = ICF_NULL;
#endif    

    if((ICF_FAILURE != ret_val) && (ICF_TRUE != p_cc_pdb->p_call_ctx->call_waiting_call))
    {
        
        if(ICF_FAILURE == 
                icf_dbm_get_line_for_inc_call(p_cc_pdb->p_glb_pdb,
					(icf_boolean_t)(ICF_FALSE),
                    &(p_cc_pdb->p_call_ctx->local_address),
                    &(p_cc_pdb->p_call_ctx->line_id),
                    &status,
                    p_cc_pdb->p_ecode))
        {

            /*
             * Fill the release cause as ICF_REL_CAUSE_USER_BUSY
             */
            ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                   ICF_CALL_CLEAR_REASON_USER_BUSY) 

            /*
             * Send call clear request to SSA to reject this call
             */
            status = ICF_STATUS_CALL_R_SRV_TERMINATE;
            ret_val = ICF_FAILURE;

        }/* end of if for ICF_FAILURE == icf_dbm_get_line_for_inc_call..*/
        
		else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                                p_cc_pdb->p_glb_pdb,
                                (icf_module_id_t)(ICF_INT_MODULE_CFG),
                                (icf_void_t *)&p_config_data,
                                p_cc_pdb->p_ecode))
        {
             ret_val = ICF_FAILURE;
        }
#ifdef ICF_REQ_URI_CALLED_PARTY_ADDR_PARSING
        /* In case the exact userinfo received in req-uri of incoming
         * INVITE did not match any of the configured user addresses,
         * there is still a possibility that the req-uri contained
         * phone-context parameter. In such a case, we should also try to
         * match the reconstructed address with the configured users.
         */
        else if ((ICF_NO_USER_ON_LINE == status) &&
                 (ICF_NULL != (ICF_ALTERNATE_ADDR_FORMAT_POSSIBLE &
                                p_cc_pdb->p_call_ctx->common_bitmask)))
        {
            ICF_PRINT(("\n[CC]: Received called party address not configured."\
                        "\n[CC]: Check if alternate address is configured\n"));
            if (ICF_FAILURE == icf_dbm_get_line_for_inc_call(
                            p_cc_pdb->p_glb_pdb,
                            (icf_boolean_t)(ICF_FALSE),
                            &(p_cc_pdb->p_call_ctx->alternate_local_address),
                            &(p_cc_pdb->p_call_ctx->line_id),
                            &status,
                            p_cc_pdb->p_ecode))
            {
                /*
                 * Fill the release cause as ICF_REL_CAUSE_USER_BUSY
                 */
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                    ICF_CALL_CLEAR_REASON_USER_BUSY)

                /*
                 * Send call clear request to SSA to reject this call
                 */
                status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                ret_val = ICF_FAILURE;
            }
        }
        ICF_PRINT(("\n[CC]: icf_dbm_get_line_for_inc_call returned line_id "\
                        "= %d and status = %x\n", \
                        p_cc_pdb->p_call_ctx->line_id, status));

        if (ICF_FAILURE == ret_val)
        {
            /* do nothing */
            ret_val = ICF_FAILURE;
        }
#endif /* ifdef ICF_REQ_URI_CALLED_PARTY_ADDR_PARSING */
        else if(((ICF_NO_USER_ON_LINE == status) && 
                (ICF_SUCCESS == icf_cc_is_any_line_without_user(p_cc_pdb,
                                                     p_cc_pdb->p_ecode)))
                ||
                ((ICF_TRUE == p_config_data->early_inc_ind_for_all_users) &&
                 (ICF_NO_USER_ON_LINE != status)))
        {
            /* give early call indication here */
            if(ICF_FAILURE == icf_cc_send_pa_early_call_ind(p_cc_pdb))
			{
				ret_val = ICF_FAILURE;				
			}
			else
			{
				ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_20_AWT_EARLY_IND_CFM);
				ret_val = ICF_SUCCESS;
				early_indication = ICF_TRUE;
			}


        }
        else if(ICF_INVALID_LINE_ID == p_cc_pdb->p_call_ctx->line_id ||
               ICF_NO_USER_ON_LINE == status )
        {
            
            /*
             * If DBM has returned line id as ICF_LINE_ID_INVALID means user 
             * is not found on any line. Fill the release cause as 
             * ICF_CALL_CLEAR_REASON_USER_NOT_FOUND
             */
            

			ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                    ICF_CALL_CLEAR_REASON_USER_NOT_FOUND)

            status = ICF_STATUS_CALL_R_SRV_TERMINATE;
            
            ret_val = ICF_FAILURE;

        }

	/*	else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                                p_cc_pdb->p_glb_pdb,
                                (icf_module_id_t)(ICF_INT_MODULE_CFG),
                                (icf_void_t *)&p_config_data,
                                p_cc_pdb->p_ecode))
        {
             ret_val = ICF_FAILURE;
        }
        */
        else if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_cc_pdb->p_glb_pdb, p_cc_pdb->p_call_ctx->line_id,
                    &p_line_data, p_cc_pdb->p_ecode))
        {
             ret_val = ICF_FAILURE;
        }
        /*If self transport is not received for line, clear the call*/
        else if ((ICF_CFG_SELF_ADDR_PER_LINE == 
                  p_config_data->self_addr_per_line ) && 
                 (ICF_FALSE == p_line_data->line_data_init_complete))
        {
            ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                    ICF_CALL_CLEAR_REASON_USER_NOT_FOUND)

            status = ICF_STATUS_CALL_R_SRV_TERMINATE;

            ret_val = ICF_FAILURE;
        }
        else
        { 
            /*
             * Check if the status returned by DBM is ICF_LINE_STATUS_FREE ie 
            * the line is IDLE.Set triggering cause as ICF_CAUSE_LINE_FREE */
            if(ICF_LINE_STATUS_FREE == status)
            {
                trigger_cause = (icf_uint8_t)ICF_CAUSE_LINE_FREE;
            }
            /*
             * Check if the status returned by DBM is ICF_LINE_STATUS_BUSY ie
             * there are few calls going on the line. Set triggering cause as 
             * ICF_CAUSE_LINE_BUSY
             */
            else if(ICF_LINE_STATUS_BUSY == status)
            {
                    /* In case app wants ICF's internal line arbitration mechanism and does
                     * not specify line_id for simultaneous calls, this function will simply 
                     * skip validation of line_id & allow MAX_CALLS on single line */
                    if (ICF_TRUE == icf_simul_call_line_arbitration)
                    {
                        /* App wants to run simultaneous calls on single line.
                         * So present all incoming calls on line-0 */
                        p_cc_pdb->p_call_ctx->line_id = 0;
                        status = ICF_STATUS_CONTINUE;
                    }
                    else
                    {
                        /* do normal line-based validations */
                        trigger_cause = (icf_uint8_t)ICF_CAUSE_LINE_BUSY;
                    }
            }
            /*
             * Call DBM's function to check service activated on line
             */ 
            ret_val = icf_dbm_chk_line_service(
                    p_cc_pdb->p_glb_pdb,
                    trigger_cause,
                    p_cc_pdb->p_call_ctx->line_id,
                    &status,
                    p_cc_pdb->p_ecode);

            /*
             * If the service status is not successfully returned by DBM or 
             * DBM has reurned ICF_STATUS_CALL_R_SRV_TERMINATE in status 
             * which means the call needs to be cleared then mark release cause 
             * as ICF_REL_CAUSE_USER_BUSY and send reject to SSA
             */
            /* This failure handling for services needs to be done only if 
             * app-configured line arbitration is set */
            if (ICF_TRUE == icf_simul_call_line_arbitration)
            {
                /* reset the status to run simultaneous calls on same line */
                status = ICF_STATUS_CONTINUE;
            }
            else if (ICF_FAILURE == ret_val || 
                        ICF_STATUS_CALL_R_SRV_TERMINATE == status)
            {
                if (ICF_FAILURE !=ret_val && trigger_cause == ICF_CAUSE_LINE_BUSY &&
                    (ICF_SERVICE_ACT_CALL_WAITING & p_line_data->service_activation_flag))
                {
                   p_line_data->num_call_waiting_calls++;
                }
                /*
                 * Fill the release cause as ICF_REL_CAUSE_USER_BUSY
                 */
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                       ICF_CALL_CLEAR_REASON_USER_BUSY) 

                /*
                 * Send call clear request to SSA to reject this call
                 */
                status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                ret_val = ICF_FAILURE;

            }
            else
            {
                /*
                 * Attach the call context to the line given by DBM in 
                 * get line for incoming call
                 */
                if(ICF_FAILURE == icf_dbm_attach_cc_to_line(
                            p_cc_pdb->p_glb_pdb, p_cc_pdb->p_call_ctx->line_id,
                            p_cc_pdb->p_call_ctx,p_cc_pdb->p_ecode))
                {
                    /* For all failures before call create, release the call */
                    ret_val = ICF_FAILURE;
                }
                /*
                 * If the status returned by DBM is not ICF_STATUS_CONTINUE 
                 * means some service invocation is required and call context 
                 * was successfully attached to line. Return to CM with status 
                 * returned by DBM set in internal message 
                 */
                if(ICF_STATUS_CONTINUE != status 
                        && 
                        ICF_FAILURE != ret_val)
                {
                    /*
                     * Copy the status into internal message status to return to
                     * CM
                     */
                     p_cc_pdb->p_internal_msg->msg_hdr.status = status;

                    /*This is the case where in some service needs to be 
                     * triggered by CM for the call*/
                    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SRVC_TRIG_STAT_RET)
                        
                    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) 
                    return ret_val;
                }
            }/*End Else*/
        }/*End Else*/
    }/*End if (ICF_TRUE != p_cc_pdb->p_call_ctx->call_waiting_call.. */

#ifdef ICF_SESSION_TIMER
    if ((ret_val != ICF_FAILURE) && \
        (ICF_TRUE == p_cc_pdb->p_call_ctx->session_timer_enable))
    {
                 if (ICF_NULL != p_session_timer_data)
                 {
                      ICF_PRINT(((icf_uint8_t*)"\nSession timer data is already there.\n"))
                      ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                                             ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE)
                      ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */  
                      return ICF_FAILURE;
                 }
                 else
                 {
                    /*   Fetching the line data */
                    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                         p_cc_pdb->p_glb_pdb, p_cc_pdb->p_call_ctx->line_id,
                         &p_line_data, p_cc_pdb->p_ecode))
                     {
			                return ICF_FAILURE;		
                     }    
	                ICF_MEMGET(p_cc_pdb->p_glb_pdb,
	                        ICF_PORT_SIZEOF(icf_session_timer_data_st),
		                    ICF_MEM_COMMON,
            		        p_session_timer_data,
        		            ICF_RET_ON_FAILURE,
		                    p_cc_pdb->p_ecode,
		                    ret_val)       

                    if ((ICF_NULL != p_session_timer_data) && 
                        (ICF_NULL != p_line_data))
                    {
                      p_session_timer_data->session_timer = p_line_data->session_timer;

                      p_session_timer_data->min_se = p_line_data->min_se;

                      p_session_timer_data->session_timer_refresher = 
                                          p_line_data->session_timer_refresher;

                      if (ICF_NULL == p_config_data)
                      {
                          if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                                p_cc_pdb->p_glb_pdb,
                                (icf_module_id_t)(ICF_INT_MODULE_CFG),
                                (icf_void_t *)&p_config_data,
                                p_cc_pdb->p_ecode))
                          {
                              ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                                          p_session_timer_data,
                                          ICF_MEM_COMMON,
                                          p_cc_pdb->p_ecode,
				          ret_val) 

                              return ICF_FAILURE;
                          } 
                      }
                      p_session_timer_data->session_refresher_reqd = 
                                          p_config_data->session_refresher_reqd;

                      p_session_timer_data->session_refresher_method = 
                                          p_config_data->call_modify_req_method;

                      p_session_timer_data->session_timer_logic = 
                                          p_config_data->session_timer_logic;

                      p_cc_pdb->p_call_ctx->p_session_timer_data = p_session_timer_data;
                   }
                   else
                   {
                      if (ICF_NULL != p_session_timer_data)
                      {
                        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                                    p_session_timer_data,
                    				ICF_MEM_COMMON,
                                    p_cc_pdb->p_ecode,
				                    ret_val)
                      }
                      ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                                             ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE)
                      ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */  
                      return ICF_FAILURE;
                   }
                 }
                 if (ICF_FAILURE == icf_ssa_process_mesg(&internal_msg))
                 {
                    status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                    ret_val = ICF_FAILURE;
                 }
    }   
   
#endif     

     /*
      * Start the setup timer
      */
    if (ICF_SUCCESS == ret_val)
    { 
    /* Fetch the app_conf for this app */
        if (ICF_SUCCESS == icf_cfg_fetch_app_conf(
                    p_cc_pdb->p_glb_pdb, 
                    p_cc_pdb->p_call_ctx->app_id, 
                    &p_app_conf,
                    p_cc_pdb->p_ecode))
        {
            icf_simul_call_line_arbitration = 
                p_app_conf->app_profile.icf_simul_call_line_arbitration;
        }
        else
        {
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return (ret_val);
        }


        if(ICF_FALSE == early_indication)
        {
            if (ICF_FAILURE == icf_cc_check_recvd_media_with_profile(p_cc_pdb))
            {
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                        ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE)

                    status = ICF_STATUS_CALL_R_SRV_TERMINATE;

                ret_val = ICF_FAILURE;
            }
            p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
            while (ICF_NULL != p_stream_sdp_info_node)
            {
                p_stream_sdp_data = (icf_stream_sdp_info_st *)p_stream_sdp_info_node->p_payload;
                if (ICF_MEDIA_STREAM_ACTIVE == (ICF_MEDIA_STREAM_ACTIVE 
                            & p_stream_sdp_data->stream_status))
                {
                    atleast_one_hit = ICF_TRUE;
                }
                p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
            }
            if (ICF_FALSE == atleast_one_hit)
            {
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                        ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE)

                    ret_val = icf_cc_00_generic_err(p_cc_pdb);
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                    return ret_val;
            }
        }
    }

    if(ICF_SUCCESS == ret_val)
    {
        ICF_CC_SET_SIG_ONGOING(p_cc_pdb)
        
        if(ICF_FALSE == p_cc_pdb->p_call_ctx->call_waiting_call)
        {
            icf_cc_set_mm_and_app_mode(p_cc_pdb, ICF_MEDIA_MODE_SEND_ONLY);
        }
        else
        {
          /* for a call waiting call, we will make the mode inactive */
		    icf_cc_set_mm_and_app_mode(p_cc_pdb, ICF_MEDIA_MODE_INACTIVE);
        } 

#ifdef ICF_TRACE_ENABLE
        if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_MEDIA_SETUP_TYPE))
        { 
           ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_REMOTE_SDP_OFFER)
        }
#endif
        if(ICF_FAILURE == icf_cc_start_setup_timer(p_cc_pdb))
        {
			status = ICF_STATUS_CALL_R_SRV_TERMINATE;
            ret_val = ICF_FAILURE;
        }
        /*
         * Send setup resp to the SSA
         */
        else if(early_indication == ICF_TRUE)
		{
			/* if early indication is true no need to do the CMS and 
			* no need of state change */
		}
        else if(ICF_FAILURE == icf_cc_send_ssa_setup_resp(p_cc_pdb))
        {
			status = ICF_STATUS_CALL_R_SRV_TERMINATE;
            ret_val = ICF_FAILURE;
        }
        else if(early_indication == ICF_TRUE)
		{
			/* if early indication is true no need to do the CMS and 
			* no need of state change */
		}
		/*
         * Send CMS to the MMI
         */
        else if(ICF_FAILURE == icf_cc_send_cms_req(p_cc_pdb))
        {
			status = ICF_STATUS_CALL_R_SRV_TERMINATE;
            ret_val = ICF_FAILURE;
        }
        else
        {
            /*
             * Update the states in the call context
             */
         /* Pankaj  icf_cc_set_local_sdp_media_mode_from_rem_mode(p_cc_pdb);*/

            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_01_AWT_CMS_RESP)
        }
    }

    if(ICF_FAILURE == ret_val)
    {
         /* Mark PA cleared bit as no need to send any indication to PA */
         ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                 ICF_CALL_CLEAR_STATUS_PA_CLEARED)
         /* if status is ICF_STATUS_CALL_R_SRV_TERMINATE return 
             success as call will be cleared by CM */
         if(ICF_STATUS_CALL_R_SRV_TERMINATE == status)
         {
                ICF_CC_SET_STATUS(p_cc_pdb,
                         ICF_STATUS_CALL_R_SRV_TERMINATE)
                ret_val = ICF_SUCCESS;
         }         
    }

     ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
     return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_00_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is in IDLE state
 *                  and CM ask it to clear call context. Once CM is sure that
 *                  call has been cleared in CC and SC, it will invoke this
 *                  API to clear call context. This function will delete call
 *                  context and detach line id
 *                  Current State: ICF_CC_00_IDLE
 *                  Current Event: ICF_CC_GENERIC_CALL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_00_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Set the call direction flag */


    /* No request has been sent to MMI, so mark this as cleared */
    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_MM_CLEARED)

    /*Stop the current timer if any*/
    icf_cc_stop_current_timer(p_cc_pdb);

    /* Stop info timer */
    icf_cc_stop_info_timer(p_cc_pdb);

    /* stop INFO confirmation timer.*/
    icf_cc_stop_info_cfm_timer(p_cc_pdb); 

    /* Depending on call direction we will clear SSA. if outgoing call 
     * no need to send any request to SSA else send call reject to SSA */
    if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
    {
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_NW_CLEARED)
    }

    /*
     *  Check if SSA has been cleared. If not it would be an incomming call then
     * send reject to SSA
     */
    if(ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_NW_CLEARED))
    {
        ret_val = icf_cc_send_ssa_reject(p_cc_pdb);
        /*
         * We wouldn't be waiting for reject ack and would mark NW as clear
         */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_NW_CLEARED)
    }
    
    /* Call the DBM function to decrease the call waiting count on line
     * This DBM function would additionally validate the call waiting could 
     * to be greater than 0 before decrementing it
	 */    
	icf_dbm_decrease_call_waiting(
		p_cc_pdb->p_glb_pdb,
		p_cc_pdb->p_call_ctx->line_id,
		p_cc_pdb->p_ecode);
    
    /*Set this release cause if no other cause is set
     *The macro takes care no release cause is overwritten*/
    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_REL_CAUSE_SERV_INT_ERR)

    /* Check if PA has been cleared */
    if(ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_PA_CLEARED))
    {
        if (ICF_NULL == ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
                    ICF_REL_TYPE_PA_INITIATED))
        {
            /* Send Call Terminate Request to PA */
            ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);
        }
        else
        {
            /* Send Call Terminate Response to PA */
            ret_val = icf_cc_send_pa_call_term_resp(p_cc_pdb);
        }

        /*
         * Now that all the three interfaces have been cleared, invoke the PA 
         * clear handler to do the call context clean up
         */
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_cc_pa_clear_handler(p_cc_pdb);
        }
    }

     icf_cc_clear_div_info(p_cc_pdb);
    /*
     * Perform call context clean up activity
     */
    if (ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                   ICF_CALL_CLEAR_STATUS_CC_CLEARED))
    {
        icf_cc_clear_call_context(p_cc_pdb);
        p_cc_pdb->p_call_ctx->app_id = ICF_APP_ID_INVALID;
    }
     
    /* Detach the call context from line */
    icf_dbm_detach_cc_from_line(p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->line_id,
                p_cc_pdb->p_call_ctx,
                p_cc_pdb->p_ecode);

    
    /* Free all call context memory*/
    icf_dbm_dealloc_call_cntxt_blk(p_cc_pdb->p_glb_pdb,
                                      p_cc_pdb->p_call_ctx->call_id,
                                      p_cc_pdb->p_ecode);


    /*Set status as continue */
    p_cc_pdb->p_internal_msg->msg_hdr.status = ICF_STATUS_CONTINUE;
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}   

/******************************************************************************
 *
 * FUNCTION:        icf_cc_00_clear_call
 *
 * DESCRIPTION:     This function is invoked when the CC is in IDLE state
 *                  and CM ask it to clear call context. Once CM is sure that
 *                  call has been cleared in CC and SC, it will invoke this
 *                  API to clear call context. This function will delete call
 *                  context and detach line id
 *                  Current State: ICF_CC_00_IDLE
 *                  Current Event: ICF_CC_CALL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_00_clear_call (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* reset the pointer to service context */
    p_cc_pdb->p_call_ctx->p_service_ctx = ICF_NULL;

    /*Stop the current timer if any*/
    icf_cc_stop_current_timer(p_cc_pdb);

	/* Stop info timer */
    icf_cc_stop_info_timer(p_cc_pdb);

    /* stop INFO confirmation timer.*/
    icf_cc_stop_info_cfm_timer(p_cc_pdb);

    /* If outgoing call, send call terminate to PA, if PA not cleared
     * if Pa initated set, send call terminate ack to PA */

    /*Set this release cause if no other cause is set
     *The macro takes care no release cause is overwritten*/
    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
            ICF_CALL_CLEAR_REASON_SYSTEM_ERROR)
    
    if(ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_PA_CLEARED))
    {
        /* If PA interface is not cleared, check if we have got any 
         * call terminate req from PA or not */

        if (ICF_NULL == ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb,
                    ICF_REL_TYPE_PA_INITIATED))
        {
            /* Send Call Terminate Request to PA */
            ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);
        }
        else
        {
            /* Send Call Terminate Response to PA */
            ret_val = icf_cc_send_pa_call_term_resp(p_cc_pdb);
        }
    }

    /* Send forcefull call clear indication to ssa */
    icf_cc_send_ssa_force_clear(p_cc_pdb);
 
    /* Call the DBM function to decrease the call waiting count on line
     * This DBM function would additionally validate the call waiting could 
     * to be greater than 0 before decrementing it
	 */    
	icf_dbm_decrease_call_waiting(
		p_cc_pdb->p_glb_pdb,
		p_cc_pdb->p_call_ctx->line_id,
		p_cc_pdb->p_ecode); 

    if (ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                   ICF_CALL_CLEAR_STATUS_CC_CLEARED))
    { 
        icf_cc_clear_call_context(p_cc_pdb);
    }

    /* Detach the call context from line */
    icf_dbm_detach_cc_from_line(p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->line_id,
                p_cc_pdb->p_call_ctx,
                p_cc_pdb->p_ecode);

    
    /* Free all call context memory*/
    icf_dbm_dealloc_call_cntxt_blk(p_cc_pdb->p_glb_pdb,
                                      p_cc_pdb->p_call_ctx->call_id,
                                      p_cc_pdb->p_ecode);
  
    /*Set status as continue */
    p_cc_pdb->p_internal_msg->msg_hdr.status = ICF_STATUS_CONTINUE;
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}  


