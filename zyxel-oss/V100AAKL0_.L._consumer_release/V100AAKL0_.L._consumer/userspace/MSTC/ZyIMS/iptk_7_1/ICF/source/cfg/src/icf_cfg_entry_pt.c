/*******************************************************************************
 *
 * FILE NAME :      icf_cfg_entry_pt.c
 *
 * DESCRIPTION:     This file contains the code of the entry point function 
 *                  of the CRM module of the Packet Phone.
 *                                    
 * REVISION HISTORY: 
 * 
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 21-Jan-2005  Shefali Aggarwal	ICF LLD	Initial
 * 15-Mar-2005  Jalaj Negi                      Changed #define for Timer Expiry
 *                                              Indication from MRM
 * 08-Dec-2005  Rohit Aggarwal      ICF 4.0 CDD Coding for IPSec support
 * 12-Jul-2005  Shradha Budhiraja   Rel 2.0 LLD Initial  
 * 1-Aug-2005   Shradha Budhiraja   Rel 2.0     Codew Review Comments
 * 25-Oct-2005  Sumit Gupta         SPR 8146    
 * 02-Feb-2006  Amit Sharma                     CFG merged with ICF 2.0
 * 08-JUN-2007  Alok Tiwari        Rel early 7.0 Code added for register status
 *
 *
 * Copyright (c) 2006, Aricent.
 ******************************************************************************/



#include "icf_internal_api_id.h"
#include "icf_common_interface_types.h"
#include "icf_cfg_prototypes.h"
#include "icf_cfg_macro.h"
#include "icf_cfg_defs.h"
#include "icf_dbm_prototypes.h"
#include "icf_regm_prototypes.h"


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_process_mesg
 *
 * DESCRIPTION:     This is the entry point function for Configuration and 
 *                  Registration handling modules.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_process_mesg(
		INOUT	icf_internal_msg_st	    *p_internal_msg)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_line_id_t                        line_id = 0; 
    icf_crm_pdb_st                       crm_pdb;/* = {0};*/

    ICF_SET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb, ICF_INT_MODULE_CFG)
    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)

    icf_port_memset(&crm_pdb, ICF_NULL,\
                      sizeof(icf_crm_pdb_st));

    /* Copy the global pdb pointer locally in pdb */
    crm_pdb.p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;
	ICF_ASSERT(p_internal_msg->msg_hdr.p_glb_pdb, 
            (ICF_NULL != crm_pdb.p_glb_pdb))

    /* Copy the received internal message pointer locally in pdb */
    crm_pdb.p_internal_msg = p_internal_msg;
	ICF_ASSERT(crm_pdb.p_glb_pdb, (ICF_NULL != crm_pdb.p_internal_msg))
        
    /* Initialize the recv msg payload pointer in the pdb */
    crm_pdb.p_recv_payload = ICF_NULL;
	
	/* By default, response will be sent for the request */
	crm_pdb.resp_to_be_sent = ICF_TRUE;
    crm_pdb.result = ICF_SUCCESS;
    crm_pdb.error_cause = ICF_CAUSE_UNDEFINED;
    crm_pdb.rgm_pdb.num_of_elements = 1;
	/* Initialize config pdb pointer in pdb to null */	
	crm_pdb.p_config_data = ICF_NULL;

    /* Initialize rgm context pointer in crm pdb to null */
    crm_pdb.p_rgm_context = ICF_NULL;
    
#ifdef ICF_MULTILINE
    /* Initialize the value of only_priority_update */
    crm_pdb.only_priority_update = ICF_FALSE;
#endif

#ifdef ICF_CFG_UT
    /* This value is used only for UT Purposes. It will have the return values 
     * for the external functions used by CFG.
     */
    ext_int_ret_val = crm_pdb.p_glb_pdb->p_recv_msg->hdr.call_id;
#endif

    /* Initialize the value of global init complete flag to value recd in msg */
    crm_pdb.global_data_init_complete = crm_pdb.p_glb_pdb->p_glb_data->
        global_data_init_complete;

ICF_CRM_START
#ifdef ICF_NW_ACTIVATE
	/* Initialize netact context pointer in crm pdb to null */
    crm_pdb.p_netact_context = ICF_NULL;
#endif
ICF_CRM_END

    crm_pdb.auto_activated = ICF_FALSE;

	/* Initialize line data pointers in pdb to null */	
	/* Initialize line status flags in pdb */
        
	for (line_id = 0; line_id < ICF_MAX_NUM_OF_LINES; line_id++)
	{
		crm_pdb.p_line_data[line_id] = ICF_NULL;
		crm_pdb.requested_lines[line_id] = ICF_FALSE;
		crm_pdb.lines_present[line_id] = ICF_FALSE;
	}

    /* Initialize other pdb flags */
    crm_pdb.is_valid_param = ICF_TRUE;

	/* Initialize api index flags in pdb */
	for (line_id = 0; line_id < ICF_MAX_NUM_OF_CONFIG_ELEMENTS; line_id++)
	{
		crm_pdb.api_indexes[line_id] = ICF_FALSE;
	}

    /* pointer to error code */
    crm_pdb.p_ecode = &(p_internal_msg->msg_hdr.ecode);
     
	crm_pdb.request_recvd_from_app = ICF_TRUE;
    crm_pdb.p_rgm_glb_data = ICF_NULL;

    /* get the config data pointer from DBM */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(crm_pdb.p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(crm_pdb.p_config_data),
                crm_pdb.p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(crm_pdb.p_glb_pdb)
        return ret_val;
    }

   
	/* get the RGM data pointer from DBM */
	if (ICF_FAILURE == icf_dbm_get_module_glb_data(crm_pdb.p_glb_pdb,
		(icf_int_module_id_t)ICF_INT_MODULE_REGM,
		(icf_void_t *)&(crm_pdb.p_rgm_glb_data),
		crm_pdb.p_ecode))
	{
		ret_val = ICF_FAILURE;
		ICF_FUNCTION_EXIT(crm_pdb.p_glb_pdb)
			return ret_val;
	}
    /* initialize the effective_self_mode to BOTH */
	crm_pdb.effective_self_mode = ICF_TRANSPORT_MODE_BOTH;
    
    /* 
     * This entry point can be called for external Configuration Module 
     * messages or for SSA messages in response to Registration requests.
     */
    ICF_CFG_TRACE(&crm_pdb, ICF_MSG_PROC_RECV_INT_MSG)
    switch (p_internal_msg->msg_hdr.msg_id)
    {
        case ICF_MSG_FROM_MRM:
        {
            switch(crm_pdb.p_glb_pdb->p_recv_msg->hdr.api_id)
            {
                case ICF_TIMER_MSG:
                    ret_val = icf_cfg_timer_expiry(&crm_pdb);
                    break;
                        
                default:
                    /* If the timer expiry API ID is not recd, then the 
                     * API must have been recvd from ext Config Module. Thus CFG
                     * function is called to identify the API and call the 
                     * handler.
                     */
                    ret_val = icf_cfg_msg_handler(&crm_pdb);
                    break;
            }
            break;
        }/* end case */


ICF_CRM_START
#if 0
		case ICF_NM_ACTIVATE_RESP:
            ret_val = icf_cfg_process_activate_resp(&crm_pdb);
            break;
		case ICF_NM_DEACTIVATE_RESP:
            ret_val = icf_cfg_process_deactivate_resp(&crm_pdb);
            break;
#endif
#ifdef ICF_NW_ACTIVATE
		case ICF_NM_CFG_EVENT_IND:
            ret_val = icf_cfg_process_nw_event_ind(&crm_pdb);
            break;
#endif
#ifdef IMS_CLIENT
		case ICF_SIC_SUBSCRIPTION_STATUS:
            ret_val = icf_rgm_process_subscribe_state(&crm_pdb);
            break;
		case ICF_SIC_NOTIFY_IND :

			ret_val = icf_rgm_process_notify(&crm_pdb);
#endif
            break;

ICF_CRM_END		
        case ICF_NW_INC_REG_RESP:
#ifdef ICF_NAT_RPORT_SUPPORT
    ret_val = icf_cfg_handle_nat_binding_chng(&crm_pdb);
    if(ICF_SUCCESS == ret_val)
    {
#endif
        ret_val = icf_rgm_process_nw_response(&crm_pdb);
#ifdef ICF_NAT_RPORT_SUPPORT
    }
#endif
    break;
ICF_CRM_START

#ifdef ICF_IPSEC_ENABLED
        
        case ICF_NM_RGM_SA_CREATE_RESP:
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
        case ICF_NM_RGM_SA_MODIFY_RESP:
#endif
        case ICF_NM_RGM_SA_DELETE_RESP:
        case ICF_NM_RGM_SA_EXPIRY_IND:
            ret_val = icf_rgm_ipsec_process_nm_msg(&crm_pdb);
            break;
        
		case ICF_DELETE_OLD_SA:
			ret_val = icf_rgm_ipsec_delete_old_sa(&crm_pdb);
			break;

#endif /* ICF_IPSEC_ENABLED */
#ifdef ICF_NW_ACTIVATE
        case ICF_NW_NOT_RESPONDING:
            ret_val = icf_cfg_nw_not_responding(&crm_pdb);
            break;
#endif
#if 0
/* ICF shall obtain the Notify indications from SIC module */
#ifdef IMS_CLIENT
		case ICF_SSA_SUBS_NOTIFY_IND:
            ret_val = icf_rgm_process_notify(&crm_pdb);
            break;
#endif
#endif
ICF_CRM_END	
ICF_INIT_START
		case ICF_CFG_INTERNAL_INIT:
				/* No response will be sent for this request */
				crm_pdb.resp_to_be_sent = ICF_FALSE;
				ret_val = icf_cfg_internal_init(&crm_pdb);
				break;

#ifndef ICF_NW_ACTIVATE
#ifdef IMS_CLIENT
		
		case ICF_CFG_GET_NW_INFO:
				/* No response will be sent for this request */
				crm_pdb.resp_to_be_sent = ICF_FALSE;
				/* Copy the recv msg payload pointer in the pdb */
				crm_pdb.p_recv_payload = 
					crm_pdb.p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->payload;

				ret_val = icf_cfg_get_nw_info(&crm_pdb);
				break;
#endif
#endif

#ifdef ICF_NW_ACTIVATE
		case ICF_CFG_AUTO_ACTIVATE:
				/* No response will be sent for this request */
				crm_pdb.resp_to_be_sent = ICF_FALSE;
		        /* Copy the recv msg payload pointer in the pdb */
				crm_pdb.p_recv_payload = 
					crm_pdb.p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->payload;
                crm_pdb.auto_activated = ICF_TRUE;

				ret_val = icf_cfg_network_activate_hdlr(&crm_pdb);
				break;
#endif
ICF_INIT_END
ICF_4_0_CRM_START
		case ICF_CFG_AUTO_REGISTER:
				/* No response will be sent for this request */
				crm_pdb.resp_to_be_sent = ICF_FALSE;
		        /* Copy the recv msg payload pointer in the pdb */
				crm_pdb.p_recv_payload = 
					crm_pdb.p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->payload;

				ret_val = icf_rgm_register_req_hdlr(&crm_pdb);
				break;
ICF_4_0_CRM_END
		case ICF_CFG_MMI_MM_INIT_COMPLETE:
			if ( ICF_FALSE == crm_pdb.global_data_init_complete)
			{
				/* Check whether all the mandatory parameters have been recd or 
				 * not */
				ret_val = icf_cfg_check_init_complete(&crm_pdb);
				ICF_CFG_TRACE(&crm_pdb, ICF_TRACE_PARAM_VALIDITY)
				if(ICF_TRUE == crm_pdb.is_valid_param)
				{
					/* All mandatory parameters have been recd */
					crm_pdb.p_glb_pdb->p_glb_data->global_data_init_complete = 
						ICF_TRUE;
					ICF_PRINT(((icf_uint8_t *) "\n[CFG]: Global data init completed"));
				}
			}
			break;
	
        default:
            /* Any other msg_id is not expected */
            ICF_CFG_TRACE(&crm_pdb, ICF_TRACE_INVALID_MSG_ID_RECD)
            ret_val = ICF_SUCCESS;
            break;

    }/* End switch */
   
    ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)    
    ICF_RESET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_msg_handler
 *
 * DESCRIPTION:     This function checks the API received in a message from the 
 *                  external Configuration Module and invokes appropriate 
 *                  handler.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_msg_handler(
		INOUT	icf_crm_pdb_st      	    *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_api_id_t                         api_id = ICF_API_ID_UNDEFINED;

#if defined ICF_NW_ACTIVATE
	icf_register_req_st						*p_payload = ICF_NULL;
	icf_error_t								*p_ecode = ICF_NULL;
#endif
#if defined ICF_NW_ACTIVATE || defined IMS_CLIENT
	icf_nm_global_st					*p_nm_glb_data = ICF_NULL;
#endif
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    api_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id;
        
    /* get the config data pointer from DBM */
    ret_val = icf_dbm_get_module_glb_data(p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(p_crm_pdb->p_config_data),
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));

#if defined ICF_NW_ACTIVATE || defined IMS_CLIENT
	/* get the nm global data pointer from DBM */
    ret_val = icf_dbm_get_module_glb_data(p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_NM,
                (icf_void_t *)&(p_nm_glb_data),
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));
#endif

    if (ICF_FAILURE == ret_val)
    {
        if ((ICF_CFG_CC_INIT_COMPLETE_REQ == api_id) ||
                (ICF_SM_CC_RESTART_REQ == api_id) ||
                (ICF_SM_CC_DELETE_CONFIG_PARAMS_REQ == api_id))
        {
            /* No response will be sent for this message */
            p_crm_pdb->resp_to_be_sent = ICF_FALSE;
        }
    }
    else if(ICF_CFG_CC_SET_SCALE_PARAMS_REQ != api_id &&
                ICF_FALSE == p_crm_pdb->p_config_data->scale_param_recd)
    {
        /* No other api other then scale param req should be processed
         * until scale-param-api is processed succesfuly.
         * it should be first api in system
         */
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED)

        /* all mandatory parameters have not been received*/
        ICF_CFG_SET_ECODE(p_crm_pdb,ICF_ERROR_INIT_CMPLT)

        ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)
       

        ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_RESP_SENT)        
        
    }
    else
    {    
        /* Copy the recv msg payload pointer in the pdb */
        p_crm_pdb->p_recv_payload = 
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->payload;
	    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
                (ICF_NULL != p_crm_pdb->p_recv_payload))

        /* Check the API Id to invoke corresponding handler */
        switch (api_id)
        {
            case ICF_CFG_CC_SET_SCALE_PARAMS_REQ:
                ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_SCALE_PARAMS_REQ)

                /* check if this is received for the first time or not */
                ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)
                if (ICF_TRUE == p_crm_pdb->p_config_data->scale_param_recd)
                {
                    /* This API has already been received once, send failure */
                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            ICF_CAUSE_SCALE_PARAMS_ALREADY_CONFIGURED)
                    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_RESP_SENT)
                }
                else
                {
                    ret_val = icf_cfg_set_scale_prms_hdlr(p_crm_pdb);
                }
                break;

ICF_CONF_STATUS_API_START
            case ICF_GET_SYSTEM_SETTINGS_REQ:
                
                ret_val = icf_cfg_get_system_settings_req_hdlr(p_crm_pdb);
                p_crm_pdb->resp_to_be_sent = ICF_FALSE;
                break;
                
            case ICF_GET_LINE_SETTINGS_REQ:
                
                ret_val = icf_cfg_get_line_settings_req_hdlr(p_crm_pdb);
                p_crm_pdb->resp_to_be_sent = ICF_FALSE;
                break;

	    case ICF_SET_LINE_PARAM_REQ:

    	  	ret_val = icf_cfg_set_line_params_hdlr(p_crm_pdb);
     	  	p_crm_pdb->resp_to_be_sent = ICF_TRUE;
     	  	break;


ICF_CONF_STATUS_API_END
ICF_CRM_START
#ifdef ICF_NW_ACTIVATE
            case ICF_NETWORK_ACTIVATE_REQ:
                ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_NETWORK_ACTIVATE_REQ)

                ret_val = icf_cfg_network_activate_hdlr(p_crm_pdb);
                p_crm_pdb->resp_to_be_sent = ICF_FALSE;
				
			 /********************************************
			   auto reg with app initiated nw activation
			  ********************************************/

				if ( (ICF_TRUE == p_crm_pdb->p_config_data->auto_register) &&
					 (ICF_FALSE == p_crm_pdb->p_config_data->auto_activate)) 
				{
					ICF_MEMGET(p_crm_pdb->p_glb_pdb, 
						sizeof(icf_msg_st) + sizeof(icf_register_req_st) , 
						ICF_MEM_COMMON,
						p_crm_pdb->p_glb_pdb->p_recv_msg, 
						ICF_RET_ON_FAILURE, 
						p_ecode, 
						ret_val)

					icf_port_memset(p_crm_pdb->p_glb_pdb->p_recv_msg, 0,
								sizeof(icf_msg_st) + sizeof(icf_register_req_st));

					/* memory allocated for p_recv_msg and payload */
					p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id = ICF_REGISTER_REQ;
					p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.version = ICF_VERSION_1_0; 
					p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.source_id = ICF_DUMMY_MODULE_ID;
					p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.destination_id = ICF_MODULE_ICF;
					p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id = (icf_uint16_t)ICF_INVALID_CALL_ID;
					p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id = ICF_DUMMY_APP_ID;
					p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.api_length = 
												sizeof(icf_msg_st) + sizeof(icf_register_req_st);
					p_payload = (icf_register_req_st *)(p_crm_pdb->p_glb_pdb->p_recv_msg->payload); 
					p_payload->bit_mask = ICF_NULL;

					/* deafult line id being filled */
					p_payload->line_id = ICF_USE_LINE1;
					p_payload->header_list.count = ICF_NULL;
					p_payload->tag_list.count = ICF_NULL;

					ret_val = icf_rgm_register_req_hdlr(p_crm_pdb);
					p_crm_pdb->resp_to_be_sent = ICF_FALSE;

					/* free the memory allocated */
					ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
							p_crm_pdb->p_glb_pdb->p_recv_msg,
							ICF_MEM_COMMON,
							p_ecode, 
							ret_val)
		
				}

                break;

            case ICF_NETWORK_DEACTIVATE_REQ:
                ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_NETWORK_DEACTIVATE_REQ)

                ret_val = icf_cfg_network_deactivate_hdlr(p_crm_pdb);
                p_crm_pdb->resp_to_be_sent = ICF_FALSE;
                break;
#endif
            case ICF_APP_ADD_REQ:
                ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_APP_ADD_REQ)

                ret_val = icf_cfg_application_add_hdlr(p_crm_pdb);
                p_crm_pdb->resp_to_be_sent = ICF_FALSE;
                break;

            case ICF_APP_REMOVE_REQ:
                ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_APP_REMOVE_REQ)

                ret_val = icf_cfg_application_remove_hdlr(p_crm_pdb);
                p_crm_pdb->resp_to_be_sent = ICF_FALSE;
                break;
ICF_CRM_END

            case ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ:
                /* REL 5.0 starts */
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
#ifdef ICF_NW_ACTIVATE
 					ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INVALID_API_ID_RECD)
					ICF_CFG_SET_RESPONSE( p_crm_pdb, ICF_FAILURE,
						    		        ICF_CAUSE_INVALID_ACTION)
					ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_RESP_SENT)
#else
					ICF_STAT(p_crm_pdb->p_glb_pdb,
					ICF_STAT_CFG_CC_SET_TRANS_PARAMS_REQ)
					
					ret_val = icf_cfg_set_trnspt_prms_in_db(p_crm_pdb);
#endif
ICF_CHECK_IMS_END
#endif
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_FALSE)
				ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_TRANS_PARAMS_REQ)

                ret_val = icf_cfg_set_trnspt_prms_in_db(p_crm_pdb);
ICF_CHECK_IMS_END
#endif
			/* REL 5.0 ends */
/*
				ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_TRANS_PARAMS_REQ)

                ret_val = icf_cfg_set_trnspt_prms_in_db(p_crm_pdb);*/
                break;

            case ICF_CFG_CC_SET_CALL_PARAMS_REQ:
                ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_CALL_PARAMS_REQ)

                ret_val = icf_cfg_set_call_prms_in_db(p_crm_pdb);
                break;

            case ICF_CFG_CC_SET_SELF_ID_PARAMS_REQ:
				/* REL 5.0 ends */
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
				if ((ICF_PORT_PLTFM_EMIM3GUSIM == p_nm_glb_data->simType) ||
					(ICF_PORT_PLTFM_EMIM3GISIM == p_nm_glb_data->simType))
				{
					ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INVALID_API_ID_RECD)
					ICF_CFG_SET_RESPONSE( p_crm_pdb, ICF_FAILURE,
						  		        ICF_CAUSE_INVALID_ACTION)
					ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_RESP_SENT)
				}
				else
				{
					ICF_STAT(p_crm_pdb->p_glb_pdb,
						    ICF_STAT_CFG_CC_SET_SELF_ID_PARAMS_REQ)

					ret_val = icf_cfg_set_self_id_prms_in_db(p_crm_pdb);
				}
ICF_CHECK_IMS_END
#endif
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_FALSE)	

				ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_SELF_ID_PARAMS_REQ)

                ret_val = icf_cfg_set_self_id_prms_in_db(p_crm_pdb);
ICF_CHECK_IMS_END
#endif
			/* REL 5.0 ends */
                break;

            case ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ:
				/* REL 5.0 starts */
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
				if ((ICF_PORT_PLTFM_EMIM3GUSIM == p_nm_glb_data->simType) ||
					(ICF_PORT_PLTFM_EMIM3GISIM == p_nm_glb_data->simType))
				{
					ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INVALID_API_ID_RECD)
					ICF_CFG_SET_RESPONSE( p_crm_pdb, ICF_FAILURE,
						  		        ICF_CAUSE_INVALID_ACTION)
					ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_RESP_SENT)
				}
				else
				{
					/*To be given by the application*/
					ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_TRANS_PARAMS_REQ)

					ret_val = icf_cfg_set_reg_prms_hdlr(p_crm_pdb);
				}
ICF_CHECK_IMS_END
#endif
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_FALSE)
				ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_TRANS_PARAMS_REQ)

                ret_val = icf_cfg_set_reg_prms_hdlr(p_crm_pdb);
				ICF_CHECK_IMS_END
#endif
			/* REL 5.0 ends */

/*                ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_REG_PARAMS_REQ)

                ret_val = icf_cfg_set_reg_prms_hdlr(p_crm_pdb);*/
                break;
#ifdef ICF_SERVICE_ENABLE

            case ICF_CFG_CC_SET_SERVICE_PARAMS_REQ:
                ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_SERV_PARAMS_REQ)

                ret_val = icf_cfg_set_srvc_prms_in_db(p_crm_pdb);
                break;
#endif

#ifndef ICF_FEATURE_ABANDON
            case ICF_CFG_CC_SET_VMS_PARAMS_REQ:
                ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_VMS_PARAMS_REQ)

                ret_val = icf_cfg_set_vms_prms_in_db(p_crm_pdb);
                break;

            case ICF_CFG_CC_INIT_COMPLETE_REQ:
                ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_INIT_COMPLETE_REQ)

                ret_val = icf_cfg_init_complete_hdlr(p_crm_pdb);
                break;
#endif

            case ICF_REGISTER_REQ:
                ret_val = icf_rgm_register_req_hdlr(p_crm_pdb);
                p_crm_pdb->resp_to_be_sent = ICF_FALSE;
                break;

            case ICF_GET_REGISTER_STATUS_REQ:
                ret_val = icf_cfg_get_register_status_req_hdlr(p_crm_pdb);
                break;

            case ICF_SM_CC_RESTART_REQ:
                ret_val = icf_cfg_restart_req_hdlr(p_crm_pdb);
                break;

/*#endif*/
            case ICF_SM_CC_DELETE_CONFIG_PARAMS_REQ:
#if 0
				/* REL 5.0 starts */
/*#ifdef IMS_CLIENT*/
                ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
				if ((ICF_PORT_PLTFM_EMIM3GUSIM == p_nm_glb_data->simType) ||
					(ICF_PORT_PLTFM_EMIM3GISIM == p_nm_glb_data->simType))
				{
					ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INVALID_API_ID_RECD)
					ICF_CFG_SET_RESPONSE( p_crm_pdb, ICF_FAILURE,
						    		        ICF_CAUSE_INVALID_ACTION)
					ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_RESP_SENT)
				}
				else
				{
					/*To be given by the application*/
					ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_TRANS_PARAMS_REQ)
			        ret_val = icf_cfg_delete_config_hdlr(p_crm_pdb);
				}

				ICF_CHECK_IMS_END
/*#else	*/
				ICF_STAT(p_crm_pdb->p_glb_pdb,
                        ICF_STAT_CFG_CC_SET_TRANS_PARAMS_REQ)

                ret_val = icf_cfg_delete_config_hdlr(p_crm_pdb);
				ICF_CHECK_IMS_END
/*#endif*/
#endif
                ret_val = icf_cfg_delete_config_hdlr(p_crm_pdb);
                break;
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
            case ICF_SET_MGMT_PLANE_AUTH_PARAM:
				icf_mph_process_mesg(p_crm_pdb->p_internal_msg);
                p_crm_pdb->result = p_crm_pdb->p_internal_msg->msg_hdr.status;
                p_crm_pdb->error_cause = p_crm_pdb->p_internal_msg->msg_hdr.ecode;
                break;
ICF_CHECK_IMS_END
#endif
			case ICF_RES_UNAVAIL_IND:
				p_crm_pdb->p_internal_msg->p_msg_data = 
					(icf_uint8_t *)p_crm_pdb->p_internal_msg->
					msg_hdr.p_glb_pdb->p_recv_msg;
				icf_adm_process_mesg(p_crm_pdb->p_internal_msg);
				break;			

            default:
                /* This is an invalid message, raise trace but return success */
                ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INVALID_API_ID_RECD)
                ret_val = ICF_SUCCESS;
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_ACTION)
                ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_RESP_SENT)
                break;

        }/* End switch-case */
    }/* End else */

    /* 
     * CFG will return failure only in case of ipc/memory/timer failures. 
     * In such a case, return common error code if not already set.
     */

    /* Check if this request has a corresponding response message */
    if (ICF_TRUE == p_crm_pdb->resp_to_be_sent)
    {
        /* Send response to external Configuration Module */
        ret_val = icf_cfg_config_prms_resp(p_crm_pdb);
        
        if(ICF_FAILURE == ret_val)
        {
            /* Failed to send response */
           ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_SEND_RESP_FAILURE)
        }
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */
