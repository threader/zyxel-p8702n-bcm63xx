/*------------------------------------------------------------------------------
*
* File name        : icf_vldt_main.c
*
* Purpose          : This file contains the functions to validate the various 
*                    API called by PA towards ICF.
*
* Revision History :
*
* Date         Name              Ref#            Comments
* --------     ------------      ----            ---------------
* 13-JUN-2007  Jagmohan Chauhan   No              Initial
                Rishi Saxena
* 07-MAR-2008  Vipul Varshney   SPR 18122       AKA Enhancement
* 05-Jun-2008  Tarun Gupta      SPR 18585       Modified icf_vldt_api
* 30-Jun-2008  Tarun Gupta      SPR 18579       Modified icf_vldt_api
* 18-Feb-2009  Anurag Khare     Rel 8.1         Options Enahcemenst Changes
* 04-Mar-2009 Saurabh Gupta	                     SPR 19349      Klocworks fix		
* 26-May-2009  Preksha          Rel 8.2         Asynchronus Message enhancements
* 11-Jun-2009 Ashutosh Mohan    SPR 19737       Changes done for GCC 4.1 warnings removal
* 07-May-2010 Aman Aggarwal     SPR 20391       Merged fixes in CSR 1-8561947
* Copyright (c) 2010, Aricent.
*---------------------------------------------------------------------------*/


#include "icf_vldt_prototypes.h"
#include "icf_vldt_defs.h"

/*******************************************************************************
*
* FUNCTION:        icf_vldt_api
*
* DESCRIPTION:     This function is invoked to validate each API field 
*                  depending on the api-id 
*
******************************************************************************/
 
icf_return_t icf_vldt_api(
             icf_msg_st *p_msg,
             icf_vldt_err_st *p_vldt_err)
{
    icf_uint8_t*  p_payload = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint16_t str_length=0;   

    /*If encode function is invoked from ICF then this argument
     *will be NULL*/
    if (ICF_NULL == p_vldt_err)
    {
         return ICF_SUCCESS;
    }
    /* the message payload*/
    p_payload = p_msg->payload;
        
    /* Validate the Payload according to API Types*/
    switch (p_msg->hdr.api_id) {
       
    case ICF_REGISTER_REQ:
         /* Validate Payload for icf_register_req_st */ 
         ret_val = icf_vldt_icf_register_req_st(p_payload,p_vldt_err);
         break;     

    case ICF_CREATE_CALL_REQ:
         /* Validate call_id in API header */
         ret_val = icf_vldt_icf_call_id_t(p_msg->hdr.call_id,p_vldt_err);
         if (ICF_SUCCESS == ret_val)
         {
             /* Validate Payload for icf_create_call_req_st */
             ret_val = icf_vldt_icf_create_call_req_st(p_payload,p_vldt_err);
         }
         else
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"call_id in API header:");
         }
         break;

    case ICF_TERMINATE_CALL_REQ:
         /* Validate Payload for icf_terminate_call_req_st */
         ret_val = icf_vldt_icf_terminate_call_req_st(p_payload,p_vldt_err);
         break;

    case ICF_CONNECT_REQ:
         /* Validate Payload for icf_connect_req_st */
         ret_val  = icf_vldt_icf_connect_req_st(p_payload,p_vldt_err);
         break;

    case ICF_SUBSCRIBE_REQ:
         /* Validate Payload for icf_subscribe_req_st*/
         ret_val = icf_vldt_icf_subscribe_req_st(p_payload,p_vldt_err);
         break;

    case ICF_PUBLISH_REQ:
         /* Validate Payload foricf_publish_req_st */
         ret_val = icf_vldt_icf_publish_req_st(p_payload,p_vldt_err);
         break;

    case ICF_MESSAGE_REQ:
         /* Validate Payload for icf_message_req_st*/
         ret_val = icf_vldt_icf_message_req_st(p_payload,p_vldt_err);
         break;

    case ICF_REFER_REQ:
         /* Validate Payload foricf_refer_req_st */
         ret_val = icf_vldt_icf_refer_req_st(p_payload,p_vldt_err);
         break;

    case ICF_CALL_HOLD_REQ:
         /* Validate Payload for icf_call_hold_req_st*/
         ret_val = icf_vldt_icf_call_hold_req_st(p_payload,p_vldt_err);   
         break;

    case ICF_CALL_RESUME_REQ:
         /* Validate Payload for icf_call_resume_req_st */
         ret_val = icf_vldt_icf_call_resume_req_st(p_payload,p_vldt_err);
         break;

    case ICF_CONFERENCE_REQ:
         /* Validate Payload for icf_conference_req_st */
         ret_val = icf_vldt_icf_conference_req_st(p_payload,p_vldt_err);
         break;

    case ICF_CALL_TRANSFER_UNATTENDED_REQ:
         /* Validate Payload for icf_ctu_req_st*/
         ret_val = icf_vldt_icf_ctu_req_st(p_payload,p_vldt_err);
         break;

    case ICF_CALL_TRANSFER_ATTENDED_REQ:
         /* Validate Payload for icf_cta_req_st*/
         ret_val = icf_vldt_icf_cta_req_st(p_payload,p_vldt_err);
         break;

    case ICF_INFO_REQ:
         /* Validate Payload for icf_info_req_st */
         ret_val = icf_vldt_icf_info_req_st(p_payload,p_vldt_err);
         break;

    case ICF_CALL_MODIFY_REQ:
         /* Validate Payload for icf_call_modify_req_st*/
         ret_val = icf_vldt_icf_call_modify_req_st(p_payload,p_vldt_err);
         break;

    case ICF_CALL_MODIFY_CFM:
         /* Validate Payload for icf_call_modify_cfm_st */
         ret_val = icf_vldt_icf_call_modify_cfm_st(p_payload,p_vldt_err);
         break;

    case ICF_START_FAX_REQ:
         /* Validate Payload for icf_start_fax_req_st*/
         ret_val = icf_vldt_icf_start_fax_req_st(p_payload,p_vldt_err);
         break;

    case ICF_NOTIFY_CFM:
         /* Validate Payload for icf_notify_cfm_st */
         ret_val = icf_vldt_icf_notify_cfm_st(p_payload,p_vldt_err);
         break;

    case ICF_OPTIONS_REQ:
         /* Validate Payload for icf_options_req_st*/
         ret_val = icf_vldt_icf_options_req_st(p_payload,p_vldt_err);
         break;

    case ICF_OPTIONS_CFM:
         /* Validate Payload for icf_options_cfm_st */
         ret_val = icf_vldt_icf_options_cfm_st(p_payload,p_vldt_err);
         break;

    case ICF_INFO_CFM:
         /* Validate Payload for icf_info_cfm_st*/
         ret_val = icf_vldt_icf_info_cfm_st(p_payload,p_vldt_err);
         break;

    case ICF_EARLY_INC_CALL_CFM:
         /* Validate Payload for icf_info_cfm_st*/
         ret_val = icf_vldt_icf_early_incoming_call_cfm_st(p_payload,p_vldt_err);
         break;

    case ICF_APP_ADD_REQ:
         /* Validate Payload for icf_app_add_req_st */
         ret_val = icf_vldt_icf_app_add_req_st(p_payload,p_vldt_err);
         break;

    case ICF_SET_SELF_ID_REQ:
         /* Validate Payload for icf_set_self_id_req_st*/
         ret_val = icf_vldt_icf_set_self_id_req_st(p_payload,p_vldt_err);
         break;

    case ICF_SET_REGISTRATION_REQ:
         /* Validate Payload for icf_set_registration_req_st*/
         ret_val = icf_vldt_icf_set_registration_req_st(p_payload,p_vldt_err);
         break;

    case ICF_SET_LINE_PARAM_REQ:
         /* Validate Payload for icf_set_registration_req_st*/
         ret_val = icf_vldt_icf_set_line_param_req(p_payload,p_vldt_err);
         break;

    case ICF_GET_SYSTEM_SETTINGS_REQ:
         /* Validate Payload for icf_get_sys_settings_req_st*/
         ret_val = icf_vldt_icf_get_system_settings_req_st(p_payload,p_vldt_err);
         break;

    case ICF_GET_LINE_SETTINGS_REQ:
         /* Validate Payload for icf_get_line_settings_req_st*/
         ret_val = icf_vldt_icf_get_line_settings_req_st(p_payload,p_vldt_err);
         break;
 
#ifdef ICF_STAT_ENABLED
    case ICF_GET_ICF_STATS_REQ:
         /* Validate Payload for icf_get_icf_stats_req_st*/
         ret_val = icf_vldt_icf_get_icf_stats_req_st(p_payload,p_vldt_err);
         break;
#endif

    case ICF_SET_TRANSPORT_REQ:
         /* Validate Payload for icf_set_transport_req_st*/
         ret_val = icf_vldt_icf_set_transport_req_st(p_payload,p_vldt_err);
         break;
 
   case ICF_CREATE_XCONF_REQ:
        /*  Validate Payload for icf_create_xconf_req_st */
        ret_val = icf_vldt_icf_create_xconf_req_st(p_payload,p_vldt_err);
        break;

   case ICF_ADD_XCONF_PARTY_REQ:
        /*  Validate Payload for icf_add_xconf_party_req_st */
        ret_val = icf_vldt_icf_add_xconf_party_req_st(p_payload,p_vldt_err);
        break;

   case ICF_DELETE_XCONF_PARTY_REQ:
        /*  Validate Payload for icf_delete_xconf_party_req_st */
        ret_val = icf_vldt_icf_delete_xconf_party_req_st(p_payload,p_vldt_err);
        break;

   case ICF_RELEASE_XCONF_REQ:
       /*  Validate Payload for icf_release_xconf_req_st */
       ret_val = icf_vldt_icf_release_xconf_req_st(p_payload,p_vldt_err);
       break;

   case  ICF_LOCAL_USER_ALERTED_REQ:
       /* validation for local user alerted */
       ret_val = icf_vldt_icf_local_user_alerted_req_st(p_payload,p_vldt_err);
       break;

   case ICF_MEDIA_CONNECT_REQ :
       /* validation for media connect req */
       ret_val = icf_vldt_icf_media_connect_req_st(p_payload,p_vldt_err);
       break; 

   case ICF_SET_SERVICE_PARAMS_REQ :
       /*validation for set service params req */
       ret_val = icf_vldt_icf_set_service_params_req_st(p_payload,p_vldt_err);
       break; 

   case ICF_SET_CALL_PARAM_REQ:
       /*validation for set_call_params req */
       ret_val =  icf_vldt_icf_set_call_params_req_st(p_payload,p_vldt_err);
       break;

   case ICF_GET_REGISTER_STATUS_REQ:
        /*validation for get_register_status req */
       ret_val =  icf_vldt_icf_get_register_status_req_st(p_payload,p_vldt_err);
       break;

   case ICF_AKA_AUTH_RESP:
        /*validation for aka_auth_resp_st */
       ret_val = icf_vldt_icf_aka_auth_resp_st(p_payload,p_vldt_err);
       break;
       
       /* CSR 1-5830831: SPR 18375 */
   case ICF_LOCAL_USER_TERMINATE_CALL_RESP:
       /* No need to validate this API as no payload present */
       break;
  
   case ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM:
       ret_val = icf_vldt_icf_rem_call_transfer_initiated_cfm_st(p_payload,p_vldt_err);
       break;
   
   case ICF_EARLY_SESSION_CFM:
       ret_val = icf_vldt_icf_early_session_cfm_st(p_payload,p_vldt_err);
       break;

#ifdef ICF_SESSION_TIMER
   case ICF_SESSION_TIMER_TOGGLE_REQ:
       ret_val = icf_vldt_icf_session_timer_toggle_req_st(p_payload,p_vldt_err);
       break;
#endif
   case ICF_GENERIC_MSG_REQ:
       ret_val = icf_vldt_icf_generic_msg_req_st(p_payload,p_vldt_err);
       break;

   case ICF_GENERIC_MSG_CFM:
       ret_val = icf_vldt_icf_generic_msg_cfm_st(p_payload,p_vldt_err);
       break;
   case ICF_MESSAGE_CFM:
       ret_val = icf_vldt_icf_app_confirmation_st(p_payload,p_vldt_err);
       break;

   /* Merged CSR 1-7931204. Fix for CSR 1-8561947. */ 
   case ICF_STOP_FAX_REQ:
   case ICF_CALL_MODIFY_CANCEL_REQ:
   case ICF_APP_REMOVE_REQ:
   case ICF_RESTART_REQ:
   case ICF_REINITIALISE_REQ:
   case ICF_SET_TRACE_REQ:
   case ICF_SET_ERROR_REPORT_LEVEL_REQ:
   case ICF_CHECK_MODULE_STATUS_REQ:
       break;

   default :  ret_val = icf_vldt_mm_api(p_msg ,p_vldt_err);
              /* In case it is not an application API but MM API 
                 then call this function 
              */
    }

    if(ICF_FAILURE == ret_val) /* If return value is a failure */
    {
        str_length= icf_vldt_port_strlen(p_vldt_err->err_str); 
        if (((str_length -1) > 0) && ((str_length - 1) < ICF_MAX_STR_LEN)) 
        {
            p_vldt_err->err_str[str_length - 1]='\0';
        }
    }
    return ret_val;
} /* End of icf_vldt_api function */

