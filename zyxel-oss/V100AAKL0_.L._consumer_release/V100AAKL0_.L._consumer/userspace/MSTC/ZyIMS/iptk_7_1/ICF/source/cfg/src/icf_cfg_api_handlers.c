/*******************************************************************************
 *
 * FILE NAME :      icf_cfg_api_handlers.c
 *
 * DESCRIPTION:     This file contains the code of the API handlers in the 
 *                  CFG module of the Packet Phone.
 *                                    
 * REVISION HISTORY:   
 *
 * Date         Name                Ref#                Comments
 * --------     ------------        ----                ---------------
 * 21-Jan-2005  Shefali Aggarwal    ICF LLD				Initial
 * 20-Apr-2005  Shradha Budhiraja                       Change in Deinit
 * 17-May-2005  Shradha Budhiraja   7564                Change of deregister 
 *                                                      func in deinit         
 * 18-May-2005  Shradha Budhiraja   7549                Reinitializing data 
 *                                                      at deinit
 * 19-May-2005  Shradha Budhiraja   7588                Change in init_complete
 *                                                      handler
 * 14-Jun-2005  Shradha Budhiraja   7644                Checking value of 
 *                                                      num_of_calls_to_log 
 *                                                      before invoking 
 *                                                      sm_deinit_call_history
 * 4-Jul-2005   Shradha Budhiraja   TCP Update          Change in SSA Triggers
 * 15-Jul-2005  Shradha Budhiraja   ICF LLD				Release 2.0
 * 22-Jul-2005  Jalaj Negi          SPR - 7824          Added API ID in 
 *                                                      CFG resp API.
 * 22-Jul-2005  Jalaj Negi          SPR - 7825          Filled CFG response
 *                                                      structure as payload 
 *                                                      in CFG response API.
 * 3-Aug-2005   Shradha Budhiraja                       Adding trigger for SSA
 *                                                      to clear context.
 * 06-Dec-2005  Navneet Gupta       ICF 4.0 CDD         Multiple App Support
 * 08-Dec-2005  Rohit Aggarwal      ICF 4.0 CDD         Coding for IPSec support
 * 13-Dec-2005    Jyoti Narula        ICF 4.0			Filling Line data for ICF 4.0:
 *														adding authentication key and 
 *														the password to line data
 * 15-Dec-2005  Puneet Sharma       ICF 4.0 CDD         Copy generic routing info in 
 *														network activation by SM
 * 02-Feb-2006  Amit Sharma                             CFG merged with ICF 2.0
 * 08-Feb-2006  Amit Sharma                             Network PDU Logging API from ICF
 *
 * 06-Jan-2006	Jyoti Narula		SPR 9124			In icf_cfg_network_activate_hdlr()
 *												for AUTO_REGISTER on, REGISTER is triggered.
 * 07-Apr-2006	Anuradha Gupta	ICF 5.0				Call Modify related App ADD changes
 *
 * 02-Apr-2006	Puneet Sharma	ICF5.0 LLD				changes done in the comments 
 *														Multiple Public Users ICF5.0
 *
 * 07-Sep-2006	Amit Sharma	SPR:12681
 * 15-Jan-2007 Abhishek Dhammawat ICF REL 6.3    Additions for Per Line Feature  
 * 26-Feb-2007 Abhishek Dhammawat CSR No: 1-4326706    Added icf_cfg_trim_string     *
 * 08-JUN-2007  Alok Tiwari      Rel early 7.0      Code added for 
                                                    get_register_status
 * 27-Aug-2007  Deepti Goyal     SPR 15997          Line id in crm pdb updated 
 *                                                  in configure confirm API.
 *
 * 7-Sep-2007   Deepti Goyal    SPR 16019           Initialize TCP blocking 
 *                                                  connect variable in glb
 * 6-Dec-2007   Amit Sharma                         6_4 merge to PQT 7_0
 * 12-Dec-2007  Abhishek Dhammawat SPR17771         Modified
 *                                                  icf_cfg_delete_config_hdlr
 * 23-Dec-2007  Abhishek Dhammawat SPR 17855       Modified 
 *                                                 icf_cfg_set_line_params_hdlr 
 * 25-Jun-2008  Rajiv Kumar        SPR - 18722     IPTK not compilable with gcc
 *                                                 version 4.2.3 
 * 07-Nov-2008  Anurag Khare       SPR 19168       To reinit port_to_be_sent_in_from_header
 * 19-Jan-2009  Sumit Manchanda   CSR 1-6792719    IPTK release 7.1.0 compliance for PROTOS 
 * 09-Feb-2009   Rohit Aggarwal  SPR 19316   Changes for INVITE with Replaces
 * 09-Jul-2009  Rohit Aggarwal  SPR 19772   Changes for Big-endian Linux
 * 22-Jul-2009  Anurag Khare    CSR 1-7493306      Changes done if function
 *                                                 icf_cfg_delete_config_hdlr
 * 19-Dec-2009  Rohit Aggarwal   SPR 20246  Configuration to strip Route header
 *
 * Copyright (c) 2009, Aricent.
 ******************************************************************************/


#include "icf_common_interface_types.h"
#include "icf_internal_api_id.h"
#include "icf_cfg_prototypes.h"
#include "icf_cfg_macro.h"
#include "icf_cfg_defs.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_regm_prototypes.h"
#include "icf_regm_prototypes.h"
#include "icf_nm_prototypes.h"
#include "icf_sm_prototypes.h"
/*#include "icf_es_prototypes.h"*/
#include "icf_mmi_defs.h"
#include "icf_mmi_types.h"


/* Single Function externed for sending invalid response directly to the application
 * instead of routing through ADM */
extern icf_return_t icf_send_to_app(icf_glb_pdb_st*    p_glb_pdb,
                icf_msg_st*    p_msg_data,
                icf_void_st*    p_routing_info,
                icf_error_t*    p_ecode);
#ifdef ICF_P_CSCF_DISCOVERY_ENABLED
extern icf_return_t icf_port_pltfm_dhcp_query(
                icf_transport_list_st *p_dhcp_addr,
                icf_transport_list_st *p_dns_addr,
                icf_transport_list_st *p_pcscf_addr);
#endif

/*#ifndef ICF_FEATURE_ABANDON*/


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_scale_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_SET_SCALE_PARAMS_REQ is received. 
 *                  It handles the scalable parameters like max_calls,
 *                  max_lines etc supported by the phone.IPPhone toolkit
 *                  scales its internal data structures based on these values.
 *                   
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in case
 *                  invalid parameters are received, or memget fails, or DBM 
 *                  fails to do initialization, or module's global data cannot 
 *                  be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_scale_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_scale_params_req_st   *p_recv_msg_payload = ICF_NULL;
    icf_dbm_scale_data_init_st           *p_init_param = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SCALABLE_LINE_PRMS_REQ_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SCALABLE_CALL_PRMS_REQ_RECD)
    /*-----------------------------------------------------------------------*/
    /* Validate the data received */
    ret_val = icf_cfg_validate_scale_prms(p_crm_pdb);

    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    if (ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        /* Validation of parameters failed */
        p_crm_pdb->result = ICF_FAILURE;
        ret_val = ICF_SUCCESS;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /*-----------------------------------------------------------------------*/
    /* Copy the external request payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_scale_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    /* Global data of DBM needs to be initialized */
    ICF_MEMGET(
            p_crm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_dbm_scale_data_init_st),
            ICF_MEM_COMMON,
            p_init_param,
            ICF_RET_ON_FAILURE,
            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
            ret_val)

    /* Set the data to be passed to DBM for initializations */
    p_init_param->max_num_of_lines = p_recv_msg_payload->max_lines;
    
    p_init_param->max_num_of_calls = 
        (icf_uint16_t) ((p_recv_msg_payload->max_audio_calls) + 
         (p_recv_msg_payload->max_video_calls) +
         (p_recv_msg_payload->max_service_calls));
    
    p_init_param->max_num_of_registrations = 
        p_recv_msg_payload->max_registration_calls;

    p_init_param->optional_bit_flag = ICF_NULL;

    p_init_param->max_num_of_subm_ctx = p_recv_msg_payload->max_lines;

    p_init_param->max_num_of_service_calls = 
        p_recv_msg_payload->max_service_calls;

    /* Invoke DBM function to init data structures */
    if (ICF_FAILURE == icf_dbm_init_scalable_param(
                            p_crm_pdb->p_glb_pdb,
                            p_init_param,
                            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
        /* Failure returned by DBM */
        ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_init_param,
                ICF_MEM_COMMON,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                ret_val)
        ret_val = ICF_FAILURE;
        ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INSUFFICIENT_RESOURCES)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /* To free the pointer */
    else
    {
        ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_init_param,
                ICF_MEM_COMMON,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                ret_val)
    }

    /* Invoke SM function for num_of_calls_to_log */
#ifdef ICF_STAT_ENABLED
    if(ICF_FAILURE == icf_sm_init_call_history(p_crm_pdb->p_glb_pdb, 
     p_recv_msg_payload->num_of_calls_to_log))
    {
        ret_val = ICF_FAILURE;
        ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INSUFFICIENT_RESOURCES)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }
#endif
    /*-----------------------------------------------------------------------*/
    /* Set the API parameters in the config data */
    p_crm_pdb->p_config_data->max_lines = p_recv_msg_payload->max_lines;

    p_crm_pdb->p_config_data->max_audio_calls = 
        p_recv_msg_payload->max_audio_calls;

    p_crm_pdb->p_config_data->max_video_calls = 
        p_recv_msg_payload->max_video_calls;

    p_crm_pdb->p_config_data->max_service_calls = 
        p_recv_msg_payload->max_service_calls;

    p_crm_pdb->p_config_data->max_registration_req = 
        p_recv_msg_payload->max_registration_calls;

    p_crm_pdb->p_config_data->max_users_per_line = 
        p_recv_msg_payload->max_users_per_line;
    
    p_crm_pdb->p_config_data->num_of_calls_to_log =  
        p_recv_msg_payload->num_of_calls_to_log;

    p_crm_pdb->p_config_data->max_num_of_subm_ctx = 
        (icf_uint8_t)p_recv_msg_payload->max_lines;

    /* 
     * Since all the mandatory config data parameters have been received, set 
     * the init complete flag for config data. This flag will indicate that 
     * scale params API has been received and any other API received after this 
     * can be processed.
     */
    p_crm_pdb->p_config_data->scale_param_recd = ICF_TRUE;

    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SCALABLE_LINE_PRMS_SET)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SCALABLE_CALL_PRMS_SET)
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_trnspt_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ is received.
 *                  This function handles transport related parameters like 
 *                  proxy address, self IP address, self port etc.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 *****************************************************************************/
icf_return_t icf_cfg_set_trnspt_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_glb_data_st                      *p_glb_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the global data pointer locally */
    p_glb_data = p_crm_pdb->p_glb_pdb->p_glb_data;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_glb_data))

    /*
     * If global_data_init_complete is false, it means that the Init Complete
     * Ind has not been received, so this is the first occurrence of this
     * request (initial set request).
     * If global_data_init_complete is true, it means that the Init Complete
     * Ind has already been received, so this is a repeated occurrence of this
     * request (modify request).
     */

    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)


    if (ICF_FALSE == p_glb_data->global_data_init_complete)
    {
        /* Request has been received for the first time 
         * (initial set request) */

        /* Check whether Scalable params have been received or not */
        if (ICF_FALSE == \
                p_crm_pdb->p_config_data->config_data_init_complete)
        {
            /* Scalable params not yet recd, return failure */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED)
            ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_RESP_SENT)
        }
        else
        {
            /* Scalable params already recd, save message data in global db */
            ret_val = icf_cfg_set_trnspt_prms_in_db(p_crm_pdb);
         }

    }/* End if for global_data_init_complete */
    else
    {
        /* Request has been received again (modify request) */

        /* Update message data in global db */
        ret_val = icf_cfg_set_trnspt_prms_in_db(p_crm_pdb);
    }/* End else for global_data_init_complete */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_call_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_SET_CALL_PARAMS_REQ is received.
 *                  This function handles call related parameters like timer 
 *                  values and certain default call settings.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 *****************************************************************************/
icf_return_t icf_cfg_set_call_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_glb_data_st                      *p_glb_data = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the global data pointer locally */
    p_glb_data = p_crm_pdb->p_glb_pdb->p_glb_data;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_glb_data))
    
    /* 
     * If global_data_init_complete is false, it means that the Init Complete 
     * Ind has not been received, so this is the first occurrence of this 
     * request (initial set request).
     * If global_data_init_complete is true, it means that the Init Complete 
     * Ind has already been received, so this is a repeated occurrence of this 
     * request (modify request).
     */
    
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)

    if (ICF_FALSE == p_glb_data->global_data_init_complete)
    {
        /* Request has been received for the first time 
         * (initial set request) */

        /* Check whether Scalable params have been received or not */
        if (ICF_FALSE == \
                p_crm_pdb->p_config_data->config_data_init_complete)
        {
            /* Scalable params not yet recd, return failure */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED)
            ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_RESP_SENT)
        }
        else
        {
            /* Scalable params already recd, save message data in global db */
            ret_val = icf_cfg_set_call_prms_in_db(p_crm_pdb);
         }
    
    }/* End if for global_data_init_complete */
    else
    {
        /* Request has been received again (modify request) */
       
        /* Update message data in global db */ 
        ret_val = icf_cfg_set_call_prms_in_db(p_crm_pdb);
    }/* End else for global_data_init_complete */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_set_reg_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ is received.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in case
 *                  of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_set_reg_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_glb_data_st                      *p_glb_data = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the global data pointer locally */
    p_glb_data = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_glb_data))
    
    /* 
     * If global_data_init_complete is false, it means that the Init Complete 
     * Ind has not been received, so this is the first occurrence of this 
     * request (initial set request).
     * If global_data_init_complete is true, it means that the Init Complete 
     * Ind has already been received, so this is a repeated occurrence of this 
     * request (modify request).
     */
    
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)

    if (ICF_FALSE == p_crm_pdb->global_data_init_complete)
    {
        /* Request has been received for the first time (initial set request) */

        /* Scalable params already recd, save message data in global db */
        ret_val = icf_cfg_set_reg_prms_in_db(p_crm_pdb);
    
    }/* End if for global_data_init_complete */
    else
    {
        /* Request has been received again (modify request) */
        /* Update message data in global db */ 
        ret_val = icf_cfg_set_reg_prms_in_db(p_crm_pdb);
    }/* End else for global_data_init_complete */
    

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_set_self_id_prms_hdlr
 *
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_SET_SELF_ID_PARAMS_REQ is received.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 *****************************************************************************/
icf_return_t icf_cfg_set_self_id_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_glb_data_st                      *p_glb_data = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the global data pointer locally */
    p_glb_data = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_glb_data))
    
    /* 
     * If global_data_init_complete is false, it means that the Init Complete 
     * Ind has not been received, so this is the first occurrence of this 
     * request (initial set request).
     * If global_data_init_complete is true, it means that the Init Complete 
     * Ind has already been received, so this is a repeated occurrence of this 
     * request (modify request).
     */
    
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)
    if (ICF_FALSE == p_glb_data->global_data_init_complete)
    {
        /* Request has been received for the first time 
         * (initial set request) */

        /* Check whether Scalable params have been received or not */
        if (ICF_FALSE == \
                p_crm_pdb->p_config_data->config_data_init_complete)
        {
            /* Scalable params not yet recd, return failure */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED)
            ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_RESP_SENT)
        }
        else
        {
            /* Scalable params already recd, save message data in global db */
            ret_val = icf_cfg_set_self_id_prms_in_db(p_crm_pdb);
         }
    
    }/* End if for global_data_init_complete */
    else
    {
        /* Request has been received again (modify request) */
       
        /* Update message data in global db */ 
        ret_val = icf_cfg_set_self_id_prms_in_db(p_crm_pdb);
    }/* End else for global_data_init_complete */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

#ifdef ICF_SERVICE_ENABLE
/******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_srvc_prms_hdlr
 * 
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_SET_SERVICE_PARAMS_REQ is received.
 *                  This function handles service related parameters like 
 *                  enable/disable service, activate/deactivate service and 
 *                  service related timers.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 *****************************************************************************/
icf_return_t icf_cfg_set_srvc_prms_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_glb_data_st                      *p_glb_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the global data pointer locally */
    p_glb_data = p_crm_pdb->p_glb_pdb->p_glb_data;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_glb_data))

    /*
     * If global_data_init_complete is false, it means that the Init Complete
     * Ind has not been received, so this is the first occurrence of this
     * request (initial set request).
     * If global_data_init_complete is true, it means that the Init Complete
     * Ind has already been received, so this is a repeated occurrence of this
     * request (modify request).
     */

    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)
    if (ICF_FALSE == p_glb_data->global_data_init_complete)
    {
        /* Request has been received for the first time 
         * (initial set request) */

        /* Check whether Scalable params have been received or not */
        if (ICF_FALSE == \
                p_crm_pdb->p_config_data->config_data_init_complete)
        {
            /* Scalable params not yet recd, return failure */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED)
            ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_RESP_SENT)
        } 
        else
        {
            /* Scalable params already recd, save message data in global db */
            ret_val = icf_cfg_set_srvc_prms_in_db(p_crm_pdb);
        }
        
    }/* End if for global_data_init_complete */
    else
    {
        /* Request has been received again (modify request) */

        /* Update message data in global db */
        ret_val = icf_cfg_set_srvc_prms_in_db(p_crm_pdb);
    }/* End else for global_data_init_complete */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

#endif
/******************************************************************************
 *
 * FUNCTION:        icf_cfg_init_complete_hdlr
 *
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_INIT_COMPLETE_REQ is received.
 *                  This function checks that all the mandatory parameters have 
 *                  been received. If yes, it opens network sockets, sends 
 *                  registration requests and makes toolkit ready to handle 
 *                  calls.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched, or if REGM 
 *                  returns failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_init_complete_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_glb_data_st                      *p_glb_data = ICF_NULL;
    icf_void_t                           *p_port_data = ICF_NULL;
    icf_mmi_data_st					     *p_mmi_data = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the global data pointer locally */
    p_glb_data = p_crm_pdb->p_glb_pdb->p_glb_data;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_glb_data))
    
    /* 
     * Check if this is a repeated message. If global data init complete is 
     * true then Init Complete Ind had already been received and processed 
     * earlier, simply ignore this one.
     */
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)

    
    if (ICF_FALSE == p_crm_pdb->global_data_init_complete)
    {
        /* This indication has been received for the first time */
        /* Check whether all the mandatory parameters have been recd or 
         * not */
        ret_val = icf_cfg_check_init_complete(p_crm_pdb);

        ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
        if(ICF_TRUE == p_crm_pdb->is_valid_param)
        {
			ret_val = icf_dbm_get_module_glb_data(p_crm_pdb->p_glb_pdb,
						(icf_module_id_t)(ICF_INT_MODULE_MMI),
						(icf_void_t *)&p_mmi_data,
						p_crm_pdb->p_ecode);
			if ( ICF_SUCCESS == ret_val && 
					ICF_TRUE == p_mmi_data->init_complete )
			{
	            /* All mandatory parameters have been recd */
	            p_crm_pdb->p_glb_pdb->p_glb_data->global_data_init_complete = 
	                ICF_TRUE;
			}
			else
			{	
				p_crm_pdb->p_glb_pdb->p_glb_data->global_data_init_complete = 
					ICF_FALSE;
			}

            ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)

            /* Check the status of VMS Subscription */
            if((ICF_NULL != p_crm_pdb->p_config_data->
                        vms_subscription_status) && (ICF_NULL == 
                            (ICF_CFG_VMS_GRP_SUBSCRIBED & 
                            p_crm_pdb->p_config_data->vms_subscription_status)))
            {
                /* VMS address Exists on line and Susbscription must be
                 * for line */

                ICF_CFG_SET_BITMASK(p_crm_pdb->p_config_data->
                        vms_subscription_status, 
                        ICF_CFG_VMS_LINE_SUBSCRIBED)
            }


            /* Trigger SSA to open network connections with transport mode 
             * as stired global config data.
             */
            if (ICF_FAILURE == icf_cfg_open_nw_interface(
                        p_crm_pdb, p_crm_pdb->p_config_data->self_mode))
            {

                ICF_CFG_SET_ECODE(p_crm_pdb,ICF_ERROR_NW_INTERFACE)
                    ret_val = ICF_FAILURE;
            }
#ifndef ICF_FEATURE_ABANDON
            else if(ICF_FAILURE ==  icf_rgm_init_complete(p_crm_pdb))
            {           
                ICF_CFG_SET_ECODE(p_crm_pdb,ICF_ERROR_RGM_INIT_CMPLT)
                    ret_val = ICF_FAILURE;
            }
#endif
            else if(ICF_FAILURE ==  icf_dbm_get_port_data(
                        p_crm_pdb->p_glb_pdb,&p_port_data,
                        &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
            {           
                ICF_CFG_SET_ECODE(p_crm_pdb,ICF_ERROR_DBM_PORT_DATA)
                    ret_val = ICF_FAILURE;
            }
            else if(ICF_FAILURE == icf_es_init_complete_handler(
                        p_port_data,
                        &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
            {           
                ICF_CFG_SET_ECODE(p_crm_pdb,ICF_ERROR_ES_INIT_CMPLT)
                    ret_val = ICF_FAILURE;
            }
#ifndef ICF_FEATURE_ABANDON
            if(ICF_FAILURE == icf_cfg_mwi_trigger(
                        p_crm_pdb,
                        (icf_uint8_t)ICF_CFG_SUBSCRIBE_ALL))
            {           
                ICF_CFG_SET_ECODE(p_crm_pdb,ICF_ERROR_MWI_INIT_CMPLT)
                    ret_val = ICF_FAILURE;
            }
#endif
        }
    }
    else
    {
        /* Init complete has already been received */
        ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_RECD)
    }
    
    /* No response will be sent for this message */
    p_crm_pdb->resp_to_be_sent = ICF_FALSE;
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* Always return success */
}/* End function */



/******************************************************************************
 *
 * FUNCTION:        icf_cfg_restart_req_hdlr
 *
 * DESCRIPTION:     This function will send an indication to CM to clear all 
 *                  calls and deallocate all call related buffers.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_restart_req_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_app_id_t		 resp_app_id = ICF_NULL;
    icf_uint8_t           app_index=(icf_uint8_t)-1,resp_app_index=(icf_uint8_t)-1;
    icf_app_conf_st      *p_app_conf = ICF_NULL;
    icf_dbm_data_st      *p_dbm_data = ICF_NULL;
    icf_config_data_st   *p_config_data = ICF_NULL;

    if(ICF_NULL == p_config_data)
    {
       ICF_PRINT(((icf_uint8_t *) "p_config_data is null"));

    }

    resp_app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->
                             hdr.app_id;
 
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* on receiving restart request set recd_restart_reinit_event
     * and adm will check this and stop sending messages to applications till 
     * it is reset before sending restart response and indication*/
    p_crm_pdb->p_glb_pdb->p_glb_data->recvd_restart_reinit_event = ICF_TRUE;
    /* 
     * Check whether this request is being received after init complete. 
     * If it is received before init complete, then it will be ignored. 
     */
    
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)
    
    ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_SYSTEM_RESTARTING)
    
   if (ICF_TRUE == p_crm_pdb->global_data_init_complete)
    {
        /* Send call clear indication to CM for all active calls */
        ret_val = icf_cfg_cm_forceful_call_clear(p_crm_pdb);
    }

    /* No response will be sent for this request */
    p_crm_pdb->resp_to_be_sent = ICF_FALSE;

   /* Get DBM data to fetch application context */
    if(ICF_FAILURE == icf_dbm_get_module_glb_data(     
                p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                (icf_void_t *)&p_dbm_data,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
        p_crm_pdb->result = ICF_FAILURE;
        p_crm_pdb->error_cause = ICF_CAUSE_INTERNAL_FAILURE;
    }

    /* Get config_data */
    else if(ICF_FAILURE == icf_dbm_get_module_glb_data(     
                p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_crm_pdb->p_config_data,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
        p_crm_pdb->result = ICF_FAILURE;
        p_crm_pdb->error_cause = ICF_CAUSE_INTERNAL_FAILURE;
    }

    /* before sending response reset recd_restart_reinit_event now adm will start 
     * start sending messages to application*/
    p_crm_pdb->p_glb_pdb->p_glb_data->recvd_restart_reinit_event = ICF_FALSE;

    p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

    for(app_index=0;app_index<p_crm_pdb->p_config_data->max_applications;app_index++)
    {
        if( ICF_TRUE==p_app_conf[app_index].is_valid)
        {
            if(resp_app_id!=p_app_conf[app_index].app_id)
            {
                ret_val = icf_cfg_restart_reinit_ind(p_crm_pdb,
                        &p_app_conf[app_index].routing_info, 
                        p_app_conf[app_index].app_id,
                        ICF_RESTART_IND);
            }
            else 
            {
                resp_app_index =  app_index;
            }
        }
    }
    
    /* A CFM response will be sent for this request */
    ret_val = icf_cfg_restart_reinit_rsp(p_crm_pdb,
		&p_app_conf[resp_app_index].routing_info, 
                         resp_app_id,ICF_RESTART_RESP);
    /* Fix for CSR 1-6792719 
       After the application has issued a restart_req, all
       stats related data should be reset to NULL.
    */
    #ifdef ICF_STAT_ENABLE
        icf_port_memset((p_crm_pdb->p_glb_pdb->p_stats_info),0,
                        ICF_PORT_SIZEOF(icf_stats_st));
        p_crm_pdb->p_glb_pdb->last_reset_time.time_in_secs = ICF_NULL;
        p_crm_pdb->p_glb_pdb->last_reset_time.time_in_millisecs = ICF_NULL;
    #endif

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; 
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_cfg_delete_config_hdlr
 *
 * DESCRIPTION:     This function will invoke a function of RGM to de-register 
 *                  all the users.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_delete_config_hdlr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_error_t                          ecode = ICF_ERROR_NONE;
    icf_line_id_t                       line_index=0;
    icf_uint8_t                        line_status[ICF_MAX_NUM_OF_LINES];
#ifdef ICF_IPV6_ENABLED
    icf_uint16_t    dType = 0;
#endif
#ifdef ICF_SESSION_TIMER
    icf_cfg_ssa_set_min_se min_se_internal_msg;
    icf_internal_msg_st internal_msg;
#endif
    icf_app_id_t		                 resp_app_id = ICF_NULL;
    icf_uint8_t                          app_index=(icf_uint8_t)-1,resp_app_index=(icf_uint8_t)-1;
    icf_app_conf_st                      *p_app_conf = ICF_NULL;
    icf_dbm_data_st                       *p_dbm_data = ICF_NULL;
    icf_user_db_node_st	*p_user_db = ICF_NULL, *p_temp_db = ICF_NULL;
    /*icf_rgm_context_st	*p_rgm_context = ICF_NULL;*/

    resp_app_id = p_crm_pdb->p_glb_pdb->p_recv_msg-> 
                             hdr.app_id;
   
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* on receiving reinititiaize request set recd_restart_reinit_event
     * and adm will check this and stop sending messages to applications till 
     * it is reset before sending reinitialize response and indication*/
    p_crm_pdb->p_glb_pdb->p_glb_data->recvd_restart_reinit_event = ICF_TRUE;
 
    /* To remove unused variable warnings */
    ecode = ecode;    

    ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_SYSTEM_REINITIALISING)    

    /* 
     * Check whether this request is being received after init complete. 
     * If it is received before init complete, then it will be ignored. 
     */
    
    if (ICF_TRUE == p_crm_pdb->global_data_init_complete)
    {
        /* Send call clear indication to CM for all active calls */
        /*        ret_val = icf_cfg_cm_call_clear_ind(p_crm_pdb);*/
        ret_val = icf_cfg_cm_reinit_ind(p_crm_pdb); 

        /*
         * For each line on which a user and registrar are configured,
         * send deregister request to REGM
         */
        ret_val = icf_rgm_forceful_dereg_all_users(p_crm_pdb);

        /* Trigger MWI to unsubscribe all users */
        /*        ret_val = icf_cfg_mwi_trigger(p_crm_pdb, 
                  (icf_uint8_t)ICF_CFG_UNSUBSCRIBE_ALL);*/

        /* Trigger SSA to initiate clean up */ 
        ret_val = icf_cfg_ssa_initiate_cleanup(p_crm_pdb);

        /*Line ID is set to all lines, to close nw server for all lines*/
        p_crm_pdb->line_id = ICF_ALL_LINES;
        /*  Trigger SSA to close network connections */
        ret_val = icf_cfg_close_nw_interface(p_crm_pdb);

#ifdef ICF_DNS_LOOKUP_ENABLED
        /*remove the dns buffer here this will automaticallly stop the DNS timer */
        ret_val = icf_ssa_dns_deinit_dns_buffer(p_crm_pdb->p_glb_pdb);
#endif

#ifdef ICF_STAT_ENABLED
        /* Call the SM deinit call history function to delete all call logs */
        if(0 != p_crm_pdb->p_config_data->num_of_calls_to_log)
        {
            icf_sm_deinit_call_history(p_crm_pdb->p_glb_pdb, &ecode);
        }
#endif

    /* Reset line data */
    for(line_index = 0; line_index < (p_crm_pdb->p_config_data->
                                        max_lines); line_index++)
    {
        line_status[line_index] = ICF_STATUS_LINE_NEW;

	    icf_dbm_fetch_line_cntxt_blk( p_crm_pdb->p_glb_pdb, line_index,
			    &p_crm_pdb->p_line_data[line_index], 
			    p_crm_pdb->p_ecode);
           /*If line data not found then continue for other lines*/
           if (ICF_NULL == p_crm_pdb->p_line_data[line_index])
           {
               continue;
           }
       /* Delete users configured on line */ 
	    p_user_db = p_crm_pdb->p_line_data[line_index]->p_user_db; 
	    while(p_user_db)
	    {
	    	p_temp_db = (icf_user_db_node_st *)p_user_db->p_next;
		    icf_dbm_delete_public_uri_for_line(p_crm_pdb->p_glb_pdb,
				    line_index, &(p_user_db->public_user),p_crm_pdb->p_ecode);
		    p_user_db = p_temp_db;
	    }

#ifdef ICF_NAT_RPORT_SUPPORT
       if(ICF_NULL != p_crm_pdb->p_line_data[line_index]->p_rport_config_data)
       {
            ICF_MEMFREE(
                     p_crm_pdb->p_glb_pdb,
                     p_crm_pdb->p_line_data[line_index]->p_rport_config_data,
                     ICF_MEM_COMMON,
                     p_crm_pdb->p_ecode,
                     ret_val)
       }
       p_crm_pdb->p_line_data[line_index]->p_rport_config_data = ICF_NULL;
#endif

	    /* Initialise line block */   
        icf_cfg_init_line_blk(p_crm_pdb, line_status[line_index],
                line_index);

	    p_crm_pdb->lines_present[line_index] = ICF_TRUE;

          /* Copying init_time parameters from config_data to line data */ 
		    p_crm_pdb->p_line_data[line_index]->proxy_addr.port_num =
			    p_crm_pdb->p_config_data->send_to_network_port;

		    p_crm_pdb->p_line_data[line_index]->registrar_addr.port_num =
			    p_crm_pdb->p_config_data->send_to_network_port;

		    p_crm_pdb->p_line_data[line_index]->transport_mode =
			    p_crm_pdb->p_config_data->self_mode;

		    p_crm_pdb->p_line_data[line_index]->reg_mode =
			    p_crm_pdb->p_config_data->self_mode;

		    if (ICF_NONIMS_AUTH_OPTION ==
				    p_crm_pdb->p_config_data->auth_type)
		    {
			    icf_port_memcpy(
					    (icf_void_t*)&(p_crm_pdb->p_line_data[line_index]->authentication_password),
					    &(p_crm_pdb->p_config_data->auth_password),
					    (icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));

			    if (ICF_NULL != p_crm_pdb->p_config_data->auth_password.str_len)
			    {
				    ICF_CFG_SET_BITMASK(p_crm_pdb->p_line_data[line_index]->line_data_received,ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)
			    }
			    else
			    {
				    ICF_CFG_RESET_BITMASK(p_crm_pdb->p_line_data[line_index]->line_data_received,ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)
			    }
		    }/* password setting for non IMS scenario ends */

    } /* End of Reset line data */


        /* Reset/Set Default values in config data except the parameters initialised at init time */

        /*----------- Transport data ----------------*/

        /* self_ip_address */

#ifdef ICF_IPV6_ENABLED
        if(icf_port_get_family_type(ICF_NULL,
                    p_crm_pdb->p_config_data->self_ip_address.port_num,
                    &dType) == ICF_FAILURE)
        {
            ret_val = ICF_FAILURE;
        }
        if(dType == ICF_PORT_IPV4_FAMILY){
#endif
            p_crm_pdb->p_config_data->self_addr_per_line = ICF_CFG_SELF_ADDR_INV_TYPE; /*0*/

            p_crm_pdb->p_config_data->self_ip_address.addr.addr_type =
                ICF_TRANSPORT_ADDRESS_IPV4_ADDR;

            p_crm_pdb->p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_1 = 
                0;
            p_crm_pdb->p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_2 = 
                0;
            p_crm_pdb->p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_3 =
                0;
            p_crm_pdb->p_config_data->self_ip_address.addr.addr.ipv4_addr.octet_4 = 
                0;
#ifdef ICF_IPV6_ENABLED
        }
        else if(dType == ICF_PORT_IPV6_FAMILY){
            strcpy(p_crm_pdb->p_config_data->self_ip_address.addr.addr.ipv6_addr.str,"\0");
            p_crm_pdb->p_config_data->self_ip_address.addr.addr.ipv6_addr.str_len = 0;
            p_crm_pdb->p_config_data->self_ip_address.addr.addr_type = 
                ICF_TRANSPORT_ADDRESS_IPV6_ADDR;
        }
#endif

        /*------------- Call data -------------------*/

        /* default_settings */
        p_crm_pdb->p_config_data->default_settings = 
            ICF_CFG_CC_CALL_THRU_SIP_PROXY | 
            ICF_CFG_CC_PRIVACY_HEADER_PRESENT;

        /* qos_mark */
        p_crm_pdb->p_config_data->qos_mark = ICF_QOS_MARK_NONE;

        /* setup_timer */
        p_crm_pdb->p_config_data->setup_timer = ICF_CFG_DEF_SETUP_TIMER;

        /* options_timer */
        p_crm_pdb->p_config_data->options_timer = ICF_CFG_DEF_OPTIONS_TIMER;

        /* alerting_timer */
        p_crm_pdb->p_config_data->alerting_timer = 
            ICF_CFG_DEF_ALERTING_TIMER;

        /* release_timer */
        p_crm_pdb->p_config_data->release_timer = ICF_CFG_DEF_RELEASE_TIMER;

        /* modify_media_timer */
        p_crm_pdb->p_config_data->modify_media_timer = 
            ICF_CFG_DEF_MODIFY_MEDIA_TIMER;

        /* registration_resp_timer */
        p_crm_pdb->p_config_data->registration_resp_timer = 
            ICF_CFG_DEF_REG_RESP_TIMER;

        /* registration_retry_timer */
        p_crm_pdb->p_config_data->registration_retry_timer = 
            ICF_CFG_DEF_REG_RETRY_TIMER;

        /* subscription_resp_timer */
        p_crm_pdb->p_config_data->subs_resp_timer = 
            ICF_CFG_DEF_SUBS_RESP_TIMER;

        /* subscription_retry_timer */
        p_crm_pdb->p_config_data->subs_retry_timer = 
            ICF_CFG_DEF_SUBS_RETRY_TIMER;

#ifdef ICF_SESSION_TIMER
        p_crm_pdb->p_config_data->session_timer = ICF_CFG_DEF_SESSION_TIMER_INTERVAL;
        p_crm_pdb->p_config_data->min_se = ICF_CFG_DEF_MIN_SESSION_TIMER_INTERVAL;
        /* By default, session timer will be enabled at run time.*/
        p_crm_pdb->p_config_data->session_timer_enable = ICF_TRUE;

        /* Session refresher is TRUE by default not to disturb existing handling.*/
        p_crm_pdb->p_config_data->session_refresher_reqd = ICF_TRUE;

        /* session_timer_logic is FALSE by default not to disturb
         * existing timer logic.*/
        p_crm_pdb->p_config_data->session_timer_logic = ICF_FALSE;

        min_se_internal_msg.min_se = ICF_CFG_DEF_MIN_SESSION_TIMER_INTERVAL;
        internal_msg.msg_hdr.msg_id = ICF_CFG_SSA_SET_MIN_SE;
        internal_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb;
        internal_msg.msg_hdr.payload_length = 
            ICF_PORT_SIZEOF(min_se_internal_msg);
        internal_msg.msg_hdr.status = ICF_NULL;
        /*SPR17771 : initialised the ecode*/
        internal_msg.msg_hdr.ecode = ICF_ERROR_UNDEFINED;
        internal_msg.p_msg_data = (icf_uint8_t *)&min_se_internal_msg;
        icf_ssa_process_mesg(&internal_msg);
#endif
        p_crm_pdb->p_config_data->dns_configuration = 0;
       
        p_crm_pdb->p_config_data->dns_source_port = 0;
       
        p_crm_pdb->p_config_data->dns_min_retrans = ICF_CFG_DEF_DNS_MIN_RETRANS;
        
        p_crm_pdb->p_config_data->dns_max_retrans = ICF_CFG_DEF_DNS_MAX_RETRANS;

        /*------------------- Service data ------------------------*/
        /* call_hold_option */
        p_crm_pdb->p_config_data->hold_option = 
            ICF_CALL_HOLD_OPT_INV_WTH_MODE_SEND_ONLY;

        /* max_num_of_call_forwarding_hops */
        p_crm_pdb->p_config_data->max_num_of_call_forwarding_hops = 
            ICF_CFG_DEF_NUM_CALL_FWD_HOPS;

        /* max_num_of_calls_in_call_waiting */
        p_crm_pdb->p_config_data->max_num_of_calls_in_call_waiting =
            ICF_CFG_DEF_MAX_CALLS_WAITING;

        /* call_hold_resume_timer */
        p_crm_pdb->p_config_data->call_hold_resume_timer = 
            ICF_CFG_DEF_CHR_TIMER;

        /* reinvite_race_timer */
        p_crm_pdb->p_config_data->reinvite_race_timer = 
            ICF_CFG_DEF_REINVITE_RACE_TIMER;

        /* twc_timer */
        p_crm_pdb->p_config_data->twc_timer = ICF_CFG_DEF_TWC_TIMER;

        /* ctu_timer */
        p_crm_pdb->p_config_data->ctu_timer = ICF_CFG_DEF_CTU_TIMER;

        /* cta_timer */
        p_crm_pdb->p_config_data->cta_timer = ICF_CFG_DEF_CTA_TIMER;

        /* rnw_timer */
        p_crm_pdb->p_config_data->rnw_timer = ICF_CFG_DEF_RNW_TIMER;

        /* cw_alert_timer */
        p_crm_pdb->p_config_data->cw_alert_timer = 
            ICF_CFG_DEF_CW_ALERT_TIMER;

        /* cnd_alert_timer */
        p_crm_pdb->p_config_data->cnd_alert_timer = 
            ICF_CFG_DEF_CND_ALERT_TIMER;

        /* cw_n_cnd_alert_timer */
        p_crm_pdb->p_config_data->cw_n_cnd_alert_timer = 
            ICF_CFG_DEF_CW_N_CND_ALERT_TIMER;

        p_crm_pdb->p_config_data->info_timer =
            ICF_CFG_DEF_INFO_TIMER;

        p_crm_pdb->p_config_data->global_service_subscription_flag = ICF_NULL;

        p_crm_pdb->p_config_data->global_service_activation_flag = ICF_NULL;

#ifndef ICF_FEATURE_ABANDON
        p_crm_pdb->p_config_data->p_group_vms_addr = ICF_NULL;

        p_crm_pdb->p_config_data->vms_subscribe_duration = ICF_CFG_DEF_SUBS_DURATION;

        p_crm_pdb->p_config_data->vms_subscription_status =  ICF_NULL;
#endif

        p_crm_pdb->p_config_data->call_modify_req_method = ICF_CFG_CALL_MODIFY_METHOD_INVITE;

        p_crm_pdb->p_config_data->multiple_answer_enable = ICF_FALSE;

        /* User-Agent Parameter to be used in SIp URI */
        if (ICF_NULL != p_crm_pdb->p_config_data->p_phone_info)
		{
		    /*Fix done for SPR-18722 : CSR-1-6068965*/
            /* free the memory allocated */
            ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
                  p_crm_pdb->p_config_data->p_phone_info,
                  ICF_MEM_COMMON,
                  &(p_crm_pdb->p_internal_msg->msg_hdr.ecode), 
                  ret_val)
        	p_crm_pdb->p_config_data->p_phone_info = ICF_NULL;
		}
		
        p_crm_pdb->p_config_data->global_service_subscription_flag = 0;
        p_crm_pdb->p_config_data->global_service_activation_flag = 0;
        p_crm_pdb->p_config_data->max_num_of_subm_ctx = 0;
        /*  Fix for SPR 19168 */
        /*  Make it true because it is the default behaviour */
        p_crm_pdb->p_config_data->port_to_be_sent_in_from_header = ICF_TRUE;

		/* Fix for CSR 1-6727350: SPR 19316 */
		p_crm_pdb->p_config_data->app_connect_reqd_for_replaces = ICF_FALSE;

#if defined (ICF_TRACE_ENABLE) || defined (ICF_ERROR_ENABLE)
        icf_port_memset(&(p_crm_pdb->p_config_data->trace_info),ICF_NULL,
             sizeof(icf_print_info_st));
#endif

#ifdef ICF_NAT_RPORT_SUPPORT
     {
       icf_rport_config_data_st *p_rport_config_data = 
           &(p_crm_pdb->p_config_data->rport_config_data);

        if (ICF_NULL != p_rport_config_data->p_binding_refresh_timer)
	{

             ICF_STOP_TIMER(p_crm_pdb->p_glb_pdb,
                      p_rport_config_data->binding_refresh_timer_id,
                      p_crm_pdb->p_ecode,ret_val)

             icf_dbm_ret_mem_to_pool(p_crm_pdb->p_glb_pdb,
                     ICF_MEM_TIMER,
                     p_rport_config_data->p_binding_refresh_timer,
                     p_crm_pdb->p_ecode);

             p_rport_config_data->binding_refresh_timer_id=ICF_NULL;

             p_rport_config_data->p_binding_refresh_timer= ICF_NULL;

	}

        icf_port_memset(&(p_crm_pdb->p_config_data->rport_config_data),ICF_NULL,
             sizeof(icf_rport_config_data_st));

        icf_cfg_rport_config_data_init(p_crm_pdb->p_glb_pdb, &p_rport_config_data);
    }
#endif
		
    }/* End if for global_data_init_complete */
   
    /* No response will be sent for this message */
    p_crm_pdb->resp_to_be_sent = ICF_FALSE;
 
    /* before sending response reset recd_restart_reinit_event now adm will start 
     * start sending messages to application*/
    p_crm_pdb->p_glb_pdb->p_glb_data->recvd_restart_reinit_event = ICF_FALSE;
    p_crm_pdb->p_glb_pdb->p_glb_data->global_data_init_complete = ICF_FALSE;

/* Get DBM data to fetch application context */
    if(ICF_FAILURE == icf_dbm_get_module_glb_data(     
                p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                (icf_void_t *)&p_dbm_data,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
        p_crm_pdb->result = ICF_FAILURE;
        p_crm_pdb->error_cause = ICF_CAUSE_INTERNAL_FAILURE;
    }

    p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

    for(app_index=0;app_index<p_crm_pdb->p_config_data->max_applications;app_index++)
    {
        if( ICF_TRUE==p_app_conf[app_index].is_valid)
        {
            if(resp_app_id!=p_app_conf[app_index].app_id)
            {
                ret_val = icf_cfg_restart_reinit_ind(p_crm_pdb,
                        &p_app_conf[app_index].routing_info, 
                        p_app_conf[app_index].app_id,
                        ICF_REINIT_IND);
		/* Remove application context */
		icf_cfg_remove_app_ctxt(p_crm_pdb,p_app_conf[app_index].app_id);
            }
            else 
            {
                resp_app_index =  app_index;
            }
        }
    }
    
    /* A CFM response will be sent for this request */
    ret_val = icf_cfg_restart_reinit_rsp(p_crm_pdb,
		&p_app_conf[resp_app_index].routing_info, 
                         resp_app_id,ICF_REINIT_RESP);

    /* Remove application context */
    icf_cfg_remove_app_ctxt(p_crm_pdb,p_app_conf[resp_app_index].app_id);

    /* Fix for CSR 1-6792719 
       After the application has issued a reinitialise_req, all
       stats related data should be reset to NULL.
    */
    #ifdef ICF_STAT_ENABLE
        icf_port_memset((p_crm_pdb->p_glb_pdb->p_stats_info),0,
                        ICF_PORT_SIZEOF(icf_stats_st));
        p_crm_pdb->p_glb_pdb->last_reset_time.time_in_secs = ICF_NULL;
        p_crm_pdb->p_glb_pdb->last_reset_time.time_in_millisecs = ICF_NULL;
    #endif
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_config_prms_resp
 *
 * DESCRIPTION:     This function sends response to the external Configuration 
 *                  Module for a request message.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in case
 *                  of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_config_prms_resp(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_error_t                          *p_ecode = ICF_ERROR_NONE;
    icf_msg_st                           *p_msg = ICF_NULL;
    icf_uint8_t                          api_len = 0;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the ecode pointer locally */
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_ecode))

    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + \
                 ICF_PORT_SIZEOF(icf_cc_cfg_set_config_params_resp_st);
    /* Allocate memory to the response message strucure */
    ICF_MEMGET(
            p_crm_pdb->p_glb_pdb,
            api_len,
            ICF_MEM_COMMON,
            p_msg,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    
    if(ICF_NULL != p_msg)
    {
		p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_CFG_SET_CONFIG_PARAM_RESP;
        /* API-ID to send */
        p_msg->hdr.api_id = ICF_CC_CFG_SET_CONFIG_PARAM_RESP;

        /*Payload length is size of api header + response structure*/
        p_msg->hdr.api_length = api_len;

        if (ICF_NULL != p_crm_pdb->p_glb_pdb->p_recv_msg)
        {
        	p_msg->hdr.app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;
            p_msg->hdr.call_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;

        	/* Fill the API id for which response is sent */
        	((icf_cc_cfg_set_config_params_resp_st *)&(p_msg->payload[0]))->
            	api_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id;    
		}
        /* SPR 15997 : Line id in configure confirm API*/
        ((icf_cc_cfg_set_config_params_resp_st *)&(p_msg->payload[0]))->
            line_id = p_crm_pdb->line_id;

        /*Result of request received success or failure*/
        ((icf_cc_cfg_set_config_params_resp_st *)&(p_msg->payload[0]))->
            result = p_crm_pdb->result;

        /*error_cause, valid if result is failure*/
        ((icf_cc_cfg_set_config_params_resp_st *)&(p_msg->payload[0]))->
            error_cause = p_crm_pdb->error_cause;     
   
   
        /* Fill the response message */    
/*        icf_cmn_fill_resp_msg(
                p_crm_pdb->p_glb_pdb,
                p_msg,
                (icf_module_id_t)(ICF_MODULE_CONF),
                (icf_api_id_t)(ICF_CC_CFG_SET_CONFIG_PARAM_RESP),
                p_crm_pdb->result,
                p_crm_pdb->error_cause);
*/
        /* Send the response message */
        p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;
        ret_val = icf_adm_process_mesg(p_crm_pdb->p_internal_msg);


        ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_RESP_SENT)

        /* Free the memory allocated to the response message */
        if (ICF_SUCCESS == ret_val)
        {
            /* Free memory */
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
        }
        else
        {
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
            /* restore the return value of ICF_SEND */
            ret_val = ICF_FAILURE;
        }
    }
        
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

ICF_INIT_START
/******************************************************************************
 *
 * FUNCTION:        icf_cfg_internal_init
 *
 * DESCRIPTION:     This function is called when the internal API 
 *                  ICF_CFG_INTERNAL_INIT is received. 
 *                  It handles the scalable parameters like max_calls,
 *                  max_lines etc supported by the phone.IPPhone toolkit
 *                  scales its internal data structures based on these values.
 *                   
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in case
 *                  invalid parameters are received, or memget fails, or DBM 
 *                  fails to do initialization, or module's global data cannot 
 *                  be fetched.
 *
 *****************************************************************************/
icf_return_t icf_cfg_internal_init(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_init_param_st                    *p_recv_msg_payload = ICF_NULL;
    icf_dbm_scale_data_init_st           *p_init_param = ICF_NULL;
ICF_CRM_START
    icf_uint8_t                          app_index = 0;
	icf_line_id_t							 line_index = 0;
    icf_dbm_data_st                      *p_dbm_data = ICF_NULL;
    icf_app_conf_st                      *p_app_conf = ICF_NULL;

#if defined ICF_NW_ACTIVATE || defined IMS_CLIENT
	icf_internal_msg_st					 internal_msg;
#endif
	icf_transport_address_st			 trans_addr;/* = {0};*/
ICF_CRM_END
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
     
	icf_port_memset(&trans_addr,ICF_NULL,sizeof(icf_transport_address_st)); 

#ifndef ICF_FEATURE_ABANDON
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SCALABLE_LINE_PRMS_REQ_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SCALABLE_CALL_PRMS_REQ_RECD)

    /*-----------------------------------------------------------------------*/
    /* Validate the data received */
    ret_val = icf_cfg_validate_scale_prms(p_crm_pdb);

    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    if (ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        p_crm_pdb->result = ICF_FAILURE;
        ret_val = ICF_SUCCESS;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }
#endif

    /*-----------------------------------------------------------------------*/
    /* Copy the external request payload pointer locally */
    p_recv_msg_payload = (icf_init_param_st *)
                            (p_crm_pdb->p_internal_msg->p_msg_data);

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    /* get the config data pointer from DBM */
    ret_val = icf_dbm_get_module_glb_data(p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(p_crm_pdb->p_config_data),
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));


    /*-----------------------------------------------------------------------*/
    
	p_crm_pdb->p_config_data->opt_parms_bitmask = ICF_NULL;

    /* Set the API parameters in the config data */

	/* non scalable params */
	
	/* mandatory params */
	
	p_crm_pdb->p_config_data->auto_scale = 
		(icf_uint8_t) p_recv_msg_payload->auto_scale; 
ICF_4_0_CRM_START
    p_crm_pdb->p_config_data->auto_register = 
        (icf_uint8_t) p_recv_msg_payload->auto_register; 
ICF_4_0_CRM_END

	p_crm_pdb->p_config_data->ims_oprn_flag = 
		(icf_boolean_t) p_recv_msg_payload->operation_mode;

	/* end of mandatory params */

	p_crm_pdb->p_config_data->self_mode = ICF_TRANSPORT_MODE_BOTH;

/* put the dns buffer related config param here */
#ifdef ICF_DNS_LOOKUP_ENABLED
	p_crm_pdb->p_config_data->sip_req_retry = 
		(icf_uint8_t) p_recv_msg_payload->sip_req_retry;
	p_crm_pdb->p_config_data->dns_purge_timer = 
		(icf_uint32_t) p_recv_msg_payload->dns_purge_timer;
#endif



	if(p_crm_pdb->p_config_data->ims_oprn_flag == ICF_OPERATION_MODE_IMS)
	{
		if(p_recv_msg_payload->bitmask & ICF_PLAIN_DIGEST_AUTH_REQUIRED)
		{
/*			ICF_SET_BIT(p_crm_pdb->p_config_data->opt_parms_bitmask,
				ICF_PLAIN_DIGEST_AUTH_REQUIRED);*/
			
			p_crm_pdb->p_config_data->auth_type = ICF_NONIMS_AUTH_OPTION;
			
			p_crm_pdb->p_config_data->auth_password.str_len = 
				(icf_uint8_t) p_recv_msg_payload->auth_password.str_len;
			
			icf_port_strcpy(p_crm_pdb->p_config_data->auth_password.str, 
				p_recv_msg_payload->auth_password.str); 
		}
		else
		{
			p_crm_pdb->p_config_data->auth_type = ICF_IMS_AUTH_OPTION;
		}
	}
	else
	{
		p_crm_pdb->p_config_data->auth_type = ICF_NONIMS_AUTH_OPTION;
	}


    if( p_crm_pdb->p_config_data->ims_oprn_flag == ICF_OPERATION_MODE_IMS)

	{
		if(p_recv_msg_payload->bitmask & ICF_DEF_SIP_PROXY_PORT)
		{
/*			ICF_SET_BIT(p_crm_pdb->p_config_data->opt_parms_bitmask,
											ICF_DEF_SIP_PROXY_PORT);*/
			
			p_crm_pdb->p_config_data->send_to_network_port =
				(icf_uint16_t)p_recv_msg_payload->sip_proxy_port_num;
		}
		else
		{
			p_crm_pdb->p_config_data->send_to_network_port =
				ICF_DEFAULT_SIP_PROXY_PORT;
		}


	}

	if(p_recv_msg_payload->bitmask & ICF_DEF_SIP_SERVER_PORT)
	{
/*		ICF_SET_BIT(p_crm_pdb->p_config_data->opt_parms_bitmask,
											ICF_DEF_SIP_SERVER_PORT);*/
		
		p_crm_pdb->p_config_data->self_ip_address.port_num = 
			(icf_uint16_t)p_recv_msg_payload->sip_serv_port_num;
	}
	else
	{
		p_crm_pdb->p_config_data->self_ip_address.port_num = 
						ICF_DEFAULT_SIP_SERVER_PORT;
	}

	if(p_recv_msg_payload->bitmask & ICF_SIP_COMPACT_HDR)
	{
/*		ICF_SET_BIT(p_crm_pdb->p_config_data->opt_parms_bitmask,
											ICF_SIP_COMPACT_HDR);*/

		p_crm_pdb->p_config_data->sip_compact_hdrs = ICF_TRUE;
	}
	else
	{
		p_crm_pdb->p_config_data->sip_compact_hdrs = ICF_FALSE;
	}

	if(p_recv_msg_payload->bitmask & ICF_SIP_AUTH_CNONCE)
	{
/*		ICF_SET_BIT(p_crm_pdb->p_config_data->opt_parms_bitmask,
											ICF_SIP_AUTH_CNONCE);*/
		
		p_crm_pdb->p_config_data->client_nonce.str_len = 
			(icf_uint8_t) p_recv_msg_payload->c_nonce_string.str_len;
		
		icf_port_strcpy(p_crm_pdb->p_config_data->client_nonce.str, 
			p_recv_msg_payload->c_nonce_string.str); 
	}

    if (ICF_NULL == p_crm_pdb->p_config_data->p_generic_routing_info)
    {
        
        /* allocate memory for routing info */
        ICF_MEMGET(p_crm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_void_st),
            ICF_MEM_COMMON,
            p_crm_pdb->p_config_data->p_generic_routing_info,
            ICF_RET_ON_FAILURE,
            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
            ret_val); 
        
        /* allocate memory for routing info */
        ICF_MEMGET(p_crm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_route_info_st),
            ICF_MEM_COMMON,
            p_crm_pdb->p_config_data->p_generic_routing_info->ptr,
            ICF_RET_ON_FAILURE,
            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
            ret_val);
        icf_port_memcpy(p_crm_pdb->p_config_data->p_generic_routing_info->ptr,
                p_recv_msg_payload->p_generic_rt_info,sizeof(icf_route_info_st));

        p_crm_pdb->p_config_data->p_generic_routing_info->len = sizeof(icf_route_info_st);
    }

#ifdef ICF_QOS_SUPPORT
	if (p_recv_msg_payload->bitmask & ICF_QOS_RES_RESERV_SUPPORTED)
	{
		/*		ICF_SET_BIT(p_crm_pdb->p_config_data->opt_parms_bitmask,
											ICF_QOS_RES_RESERV_REQUIRED);*/
		p_crm_pdb->p_config_data->nw_res_reservation_supported = ICF_TRUE;
	}
	else
	{
		p_crm_pdb->p_config_data->nw_res_reservation_supported = ICF_FALSE;
	}
#endif

	/* scalable params as per #defines */
	if (ICF_TRUE == p_recv_msg_payload->auto_scale)
	{
		p_crm_pdb->p_config_data->max_lines = ICF_DEFAULT_MAX_LINES;
		p_crm_pdb->p_config_data->max_audio_calls = ICF_DEFAULT_MAX_AUDIO_CALLS;
		p_crm_pdb->p_config_data->max_video_calls = ICF_DEFAULT_MAX_VIDEO_CALLS;
		p_crm_pdb->p_config_data->max_service_calls = ICF_DEFAULT_MAX_SERVICE_CALLS;
		p_crm_pdb->p_config_data->max_registration_req = ICF_DEFAULT_MAX_REGISTRATIONS;
		p_crm_pdb->p_config_data->max_users_per_line = ICF_DEFAULT_MAX_URIS;
		p_crm_pdb->p_config_data->num_of_calls_to_log = ICF_DEFAULT_MAX_CALLS_TO_LOG;
		p_crm_pdb->p_config_data->max_applications = ICF_DEFAULT_MAX_APPLICATIONS;

                p_crm_pdb->p_glb_pdb->sys_resource_limit = ICF_DEFAULT_SYS_RESOURCE_LIMIT;

	}

	else
	{
			if( p_recv_msg_payload->p_scalable_info->num_of_max_lines > ICF_MAX_NUM_OF_LINES 
				|| ICF_NULL == p_recv_msg_payload->p_scalable_info->num_of_max_lines)
			{
				p_crm_pdb->p_config_data->max_lines = ICF_DEFAULT_MAX_LINES; 
			}
			else
			{
				p_crm_pdb->p_config_data->max_lines = (icf_uint16_t) p_recv_msg_payload->p_scalable_info->num_of_max_lines;     
			}
						
			if( p_recv_msg_payload->p_scalable_info->num_of_max_reg > ICF_MAX_NUM_OF_REGISTRATION 
			|| ICF_NULL == p_recv_msg_payload->p_scalable_info->num_of_max_reg)
			{
				p_crm_pdb->p_config_data->max_registration_req = ICF_DEFAULT_MAX_REGISTRATIONS; 
			}
			else
			{
				p_crm_pdb->p_config_data->max_registration_req = (icf_uint16_t) p_recv_msg_payload->p_scalable_info->num_of_max_reg;     
			}
		
			if(( p_recv_msg_payload->p_scalable_info->num_of_max_apps > ICF_MAX_NUM_APPLICATION 
				|| p_recv_msg_payload->p_scalable_info->num_of_max_apps < ICF_MIN_NUM_APPLICATION)
				|| ICF_NULL == p_recv_msg_payload->p_scalable_info->num_of_max_apps)
			{
				p_crm_pdb->p_config_data->max_applications = ICF_DEFAULT_MAX_APPLICATIONS; 
			}
			else
			{
				p_crm_pdb->p_config_data->max_applications = (icf_uint8_t) p_recv_msg_payload->p_scalable_info->num_of_max_apps;     
			}
		
			if( (p_recv_msg_payload->p_scalable_info->num_of_max_audio_calls > ICF_MAX_NUM_CALL 
				|| p_recv_msg_payload->p_scalable_info->num_of_max_audio_calls < ICF_MIN_NUM_CALL) 
				|| ICF_NULL == p_recv_msg_payload->p_scalable_info->num_of_max_audio_calls)
			{
				p_crm_pdb->p_config_data->max_audio_calls = ICF_DEFAULT_MAX_AUDIO_CALLS; 
			}
			else
			{
				p_crm_pdb->p_config_data->max_audio_calls = (icf_uint16_t)p_recv_msg_payload->p_scalable_info->num_of_max_audio_calls;     
			}
		
			if(( p_recv_msg_payload->p_scalable_info->num_of_max_video_calls > ICF_MAX_NUM_CALL 
				|| p_recv_msg_payload->p_scalable_info->num_of_max_video_calls < ICF_MIN_NUM_CALL )
				|| ICF_NULL == p_recv_msg_payload->p_scalable_info->num_of_max_video_calls)
			{
				p_crm_pdb->p_config_data->max_video_calls = ICF_DEFAULT_MAX_VIDEO_CALLS; 
			}
			else
			{
				p_crm_pdb->p_config_data->max_video_calls = (icf_uint16_t) p_recv_msg_payload->p_scalable_info->num_of_max_video_calls;     
			}
		
			if(( p_recv_msg_payload->p_scalable_info->num_of_max_service_calls > ICF_MAX_NUM_CALL 
				|| p_recv_msg_payload->p_scalable_info->num_of_max_service_calls < ICF_MIN_NUM_CALL) 
				|| ICF_NULL == p_recv_msg_payload->p_scalable_info->num_of_max_service_calls )
			{
				p_crm_pdb->p_config_data->max_service_calls = ICF_DEFAULT_MAX_SERVICE_CALLS; 
			}
			else
			{
				p_crm_pdb->p_config_data->max_service_calls = (icf_uint16_t) p_recv_msg_payload->p_scalable_info->num_of_max_service_calls;     
			}
			if( (p_recv_msg_payload->p_scalable_info->num_of_max_uri > ICF_MAX_NUM_URI 
				|| p_recv_msg_payload->p_scalable_info->num_of_max_uri < ICF_MIN_NUM_URI) 
				|| ICF_NULL == p_recv_msg_payload->p_scalable_info->num_of_max_uri)
			{
				p_crm_pdb->p_config_data->max_users_per_line = ICF_DEFAULT_MAX_URIS; 
			}
			else
			{
				p_crm_pdb->p_config_data->max_users_per_line = (icf_uint16_t) p_recv_msg_payload->p_scalable_info->num_of_max_uri;     
			}
#if 0 /*Now max call objects will be derived from calls and reg and subs */			
			if (p_crm_pdb->p_config_data->max_audio_calls + 
          				p_crm_pdb->p_config_data->max_video_calls + 
         				p_crm_pdb->p_config_data->max_registration_req > ICF_SSA_MAX_CALL_OBJ)
      		{
				p_crm_pdb->p_config_data->max_audio_calls = ICF_DEFAULT_MAX_CALLS; 
				p_crm_pdb->p_config_data->max_video_calls = ICF_DEFAULT_MAX_CALLS; 
				p_crm_pdb->p_config_data->max_registration_req = ICF_DEFAULT_MAX_REGISTRATIONS; 
      		}
#endif
			
			if( (p_recv_msg_payload->p_scalable_info->num_of_max_call_history > ICF_MAX_NUM_OF_CALLS_TO_LOG 
				|| p_recv_msg_payload->p_scalable_info->num_of_max_call_history < ICF_MIN_NUM_OF_CALLS_TO_LOG)
				|| ICF_NULL == p_recv_msg_payload->p_scalable_info->num_of_max_call_history)
			{
				p_crm_pdb->p_config_data->num_of_calls_to_log = ICF_DEFAULT_MAX_CALLS_TO_LOG; 
			}
			else
			{
				p_crm_pdb->p_config_data->num_of_calls_to_log = (icf_uint8_t) p_recv_msg_payload->p_scalable_info->num_of_max_call_history;     
			}

			if( (p_recv_msg_payload->p_scalable_info->sys_resource_limit > ICF_MAX_SYS_RESOURCE_LIMIT 
				|| p_recv_msg_payload->p_scalable_info->sys_resource_limit < ICF_MIN_SYS_RESOURCE_LIMIT)
				|| ICF_NULL == p_recv_msg_payload->p_scalable_info->sys_resource_limit)
			{
				p_crm_pdb->p_glb_pdb->sys_resource_limit = ICF_DEFAULT_SYS_RESOURCE_LIMIT; 
			}
			else
			{
				p_crm_pdb->p_glb_pdb->sys_resource_limit = p_recv_msg_payload->p_scalable_info->sys_resource_limit;     
			}
	}


    /* Global data of DBM needs to be initialized */
    ICF_MEMGET(
            p_crm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_dbm_scale_data_init_st),
            ICF_MEM_COMMON,
            p_init_param,
            ICF_RET_ON_FAILURE,
            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
            ret_val)

    /* Set the data to be passed to DBM for initializations */
    p_init_param->max_num_of_lines = p_crm_pdb->p_config_data->max_lines;
    
    p_init_param->max_num_of_calls = (icf_uint16_t)((p_crm_pdb->p_config_data->max_audio_calls) +
                                     (p_crm_pdb->p_config_data->max_video_calls) + 
                                     (p_crm_pdb->p_config_data->max_service_calls));
    p_crm_pdb->p_config_data->max_num_of_calls = p_init_param->max_num_of_calls;

   /*Derive max number of SIP call objects*/
    p_crm_pdb->p_config_data->max_sip_call_objs = 
         (icf_uint16_t)(2 * (p_init_param->max_num_of_calls * (1 + ICF_CALL_RATE_PER_MIN)
       + (p_crm_pdb->p_config_data->max_registration_req * 2)));
    /*Reg contexts are multiplied by 2 to cater for subscription contexts*/
 
   /*Derive max number of timer blocks*/
    p_crm_pdb->p_config_data->max_num_of_timers =
         (icf_uint16_t)(((3 * p_init_param->max_num_of_calls) + (2 * (p_crm_pdb->p_config_data->max_registration_req * 2))));

#ifdef ICF_LOAD_STAT
    printf("\nNumber of timers:%d\n", p_crm_pdb->p_config_data->max_num_of_timers);
#endif
    p_init_param->max_num_of_timers = p_crm_pdb->p_config_data->max_num_of_timers;
	
    p_init_param->max_num_of_service_calls = p_crm_pdb->p_config_data->max_service_calls;

    p_init_param->max_num_of_registrations = p_crm_pdb->p_config_data->max_registration_req; 

    p_init_param->optional_bit_flag = ICF_NULL;

	if(p_recv_msg_payload->bitmask & ICF_NW_TRACE_ENABLED)
	{
/*		ICF_SET_BIT(p_crm_pdb->p_config_data->opt_parms_bitmask,
											ICF_NW_TRACE_ENABLED);*/
		
		p_crm_pdb->p_config_data->nw_trace_enabled = ICF_TRUE;
	}
	else
	{
		p_crm_pdb->p_config_data->nw_trace_enabled = ICF_FALSE;
	}

/*NOTIFY_OUT_OF_DIALOG_START*/
	
	if(p_recv_msg_payload->bitmask & ICF_NON_SUBSCRIBE_NOTIFY_ENABLE)
	{		
		p_crm_pdb->p_config_data->non_sub_notify_enable = ICF_TRUE;
	}
	else
	{
		p_crm_pdb->p_config_data->non_sub_notify_enable = ICF_FALSE;
	}
/*NOTIFY_OUT_OF_DIALOG_END*/

    /* Invoke DBM function to init data structures */
    if (ICF_FAILURE == icf_dbm_init_scalable_param(
                            p_crm_pdb->p_glb_pdb,
                            p_init_param,
                            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
        ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_init_param,
                ICF_MEM_COMMON,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                ret_val)
        ret_val = ICF_FAILURE;
        ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INSUFFICIENT_RESOURCES)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /* To free the pointer */
    else
    {
        ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_init_param,
                ICF_MEM_COMMON,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                ret_val)
    }

#ifdef ICF_STAT_ENABLED
    /* Invoke SM function for num_of_calls_to_log */
    if(ICF_FAILURE == icf_sm_init_call_history(p_crm_pdb->p_glb_pdb, 
     p_recv_msg_payload->p_scalable_info->num_of_calls_to_log))
    {
        ret_val = ICF_FAILURE;
        ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INSUFFICIENT_RESOURCES)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }
#endif   

#if defined ICF_NW_ACTIVATE || defined IMS_CLIENT
	/* Now set the scalable parameter in NM ..so that it can allocate memory 
	   based on number of calls
	 */
      internal_msg.p_msg_data = ICF_NULL;
      internal_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb;
      internal_msg.msg_hdr.msg_id = ICF_NM_SCALABLE_PARAMS_REQ;
      icf_nm_process_mesg(&internal_msg);
#endif


    /* 
     * Since all the mandatory config data parameters have been received, set 
     * the init complete flag for config data. This flag will indicate that 
     * scale params API has been received and any other API received 
     * after this can be processed.
     */
    p_crm_pdb->p_config_data->config_data_init_complete = ICF_TRUE;
    p_crm_pdb->p_config_data->scale_param_recd = ICF_TRUE;

ICF_CRM_START
    /* Get DBM data to fetch network access context */
    if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
                                p_crm_pdb->p_glb_pdb,
                                (icf_int_module_id_t const)\
                                ICF_INT_MODULE_DBM,
                                (icf_void_t *)&p_dbm_data,
                                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
		if(ICF_SUCCESS == icf_cfg_check_line_configured(p_crm_pdb,
				(icf_boolean_t)ICF_TRUE))
		{
				for(line_index = 0; line_index < (p_crm_pdb->p_config_data->
					max_lines); line_index++)
				{
					if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
					{                
						icf_port_memcpy(
							(icf_void_t*)&(p_crm_pdb->p_line_data[line_index]->proxy_addr),
							(icf_void_t*)&(trans_addr),
							(icf_uint32_t) ICF_PORT_SIZEOF(icf_transport_address_st));
					
						p_crm_pdb->p_line_data[line_index]->proxy_addr.port_num = 
							p_crm_pdb->p_config_data->send_to_network_port;
						
						icf_port_memcpy(
							(icf_void_t*)&(p_crm_pdb->p_line_data[line_index]->registrar_addr),
							(icf_void_t*)&(trans_addr),
							(icf_uint32_t) ICF_PORT_SIZEOF(icf_transport_address_st));
					
						p_crm_pdb->p_line_data[line_index]->registrar_addr.port_num = 
							p_crm_pdb->p_config_data->send_to_network_port;

						p_crm_pdb->p_line_data[line_index]->transport_mode = 
											p_crm_pdb->p_config_data->self_mode;
						
						p_crm_pdb->p_line_data[line_index]->reg_mode = 
											p_crm_pdb->p_config_data->self_mode;


						if (ICF_NONIMS_AUTH_OPTION ==
										p_crm_pdb->p_config_data->auth_type)
						{
							icf_port_memcpy(
								(icf_void_t*)&(p_crm_pdb->p_line_data[line_index]->authentication_password),
								&(p_crm_pdb->p_config_data->auth_password),
								(icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));

							if (ICF_NULL != p_crm_pdb->p_config_data->auth_password.str_len)
							{											
								ICF_CFG_SET_BITMASK(p_crm_pdb->p_line_data[line_index]->line_data_received,ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)
							}				
							else
							{
								ICF_CFG_RESET_BITMASK(p_crm_pdb->p_line_data[line_index]->line_data_received,ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)
							}
						}/* password setting for non IMS scenario ends */

					}
				}
		}

		p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;
		
		/* Initialise application context */
		
		for(app_index = 0; app_index < p_crm_pdb->\
			p_config_data->max_applications; app_index++)
		{
			icf_port_memset(
				(icf_void_t*)&p_app_conf[app_index],
				(icf_uint8_t)0,
				(icf_uint32_t)
				ICF_PORT_SIZEOF(icf_app_conf_st));
			
			p_app_conf[app_index].is_valid = ICF_FALSE;
		}
    }
    if ((ICF_FAILURE != ret_val) && (ICF_NULL != p_recv_msg_payload->p_tls_init_data))
    {
        ICF_MEMGET(
          p_crm_pdb->p_glb_pdb,
          ICF_PORT_SIZEOF(icf_tls_init_params_st),
          ICF_MEM_COMMON,
          p_crm_pdb->p_config_data->p_tls_init_data,
          ICF_RET_ON_FAILURE,
          &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
          ret_val)

        icf_port_memcpy(p_crm_pdb->p_config_data->p_tls_init_data,
                    p_recv_msg_payload->p_tls_init_data,
                    ICF_PORT_SIZEOF(icf_tls_init_params_st));
    }
    /* If blocking connect is required ,the same is configured in the config data */
    if (ICF_BLOCKING_CONNECT == (p_recv_msg_payload->bitmask & ICF_BLOCKING_CONNECT))
    {
         p_crm_pdb->p_config_data->is_blocking_connect = ICF_TRUE;
    }
    /* Assigning the default value application specific UPDATE handling parameter */
    p_crm_pdb->p_config_data->update_reqd = ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC;

    p_crm_pdb->p_config_data->early_inc_ind_for_all_users = ICF_FALSE; 

ICF_CRM_END
    /* SPR 16019: Initialize TCP blocking connect to false
     * By default, TCP connect will always be non-blocking
     */
    p_crm_pdb->p_config_data->is_tcp_blocking_connect = ICF_FALSE;
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SCALABLE_LINE_PRMS_SET)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SCALABLE_CALL_PRMS_SET)
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_INIT_COMPLETE_STATUS)
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */
ICF_INIT_END




/******************************************************************************
 *
 * FUNCTION:        icf_cfg_get_nw_info
 *
 * DESCRIPTION:     
 *                   
 * RETURNS:         
 *
 *****************************************************************************/
icf_return_t icf_cfg_get_nw_info(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{  

#ifdef NON_IMS_CLIENT
    /*To Remove Warning */
    p_crm_pdb=p_crm_pdb;
#endif 
	return ICF_FAILURE;
}

ICF_CRM_START



/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_application_add_resp
 *
 * DESCRIPTION:     This function sends response to external Application 
 *                  Module for a request message.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in 
 *                  case of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_application_add_resp(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        INOUT   icf_void_st     *p_routing_info,
        IN      icf_app_id_t app_id)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_error_t                          *p_ecode = ICF_ERROR_NONE;
    icf_msg_st                           *p_msg = ICF_NULL;
    icf_uint16_t                         api_len = 0;
    icf_app_add_cfm_st                   *p_payload = ICF_NULL;


    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the ecode pointer locally */
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_ecode))

    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
                    ICF_PORT_SIZEOF(icf_app_add_cfm_st);

    /* Allocate memory to the response message strucure */
    ICF_MEMGET(
            p_crm_pdb->p_glb_pdb,
            api_len,
            ICF_MEM_COMMON,
            p_msg,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    
    if(ICF_NULL != p_msg)
    {
       
        p_payload = (icf_app_add_cfm_st *) p_msg->payload;

		p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_APP_ADD_CFM;
        /* API-ID to send */
        p_msg->hdr.api_id = ICF_APP_ADD_CFM;
        
        /*Payload length is size of api header + response structure*/
        p_msg->hdr.api_length = api_len;

        p_msg->hdr.call_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
        
        /*Result of request received success or failure*/
        p_payload->result = p_crm_pdb->result;

        /*error_cause, valid if result is failure*/
        p_payload->error_cause = p_crm_pdb->error_cause;     

        /* bit_mask indicates the presence of the optional parameters */
        p_payload->bit_mask = 0;

        if (ICF_FAILURE == p_crm_pdb->result)
        {
             p_payload->bit_mask = (icf_uint8_t)(p_payload->bit_mask | ICF_APP_ADD_CFM_ERROR_CAUSE_PRESENT);
        }

        /* Fill the response message */    
/*        icf_cmn_fill_resp_msg(
                p_crm_pdb->p_glb_pdb,
                p_msg,
                (icf_module_id_t)(ICF_MODULE_CONF),
                (icf_api_id_t)(ICF_CC_CFG_SET_CONFIG_PARAM_RESP),
                p_crm_pdb->result,
                p_crm_pdb->error_cause);
*/
        
        /* APP-ID to send */
        p_msg->hdr.app_id = app_id;

        /* Send the response message */
        
        p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;
        if(ICF_INVALID_APP_ID == app_id)
        {
            /* version to send */
            p_msg->hdr.version = ICF_VERSION_1_0;
        
            /* Source module is ICF */
            p_msg->hdr.source_id = ICF_MODULE_ICF;

            /* Destination is the module to which response targetted*/
            p_msg->hdr.destination_id = p_crm_pdb->p_internal_msg->\
                msg_hdr.p_glb_pdb->p_recv_msg->hdr.source_id;

#ifdef ICF_TRACE_ENABLE
            icf_cmn_trace_api_from_api_id(
                   p_crm_pdb->p_glb_pdb,p_msg,
                   p_ecode);
#endif

            /* Application does not exist in app db, send response on 
            destination given in routing_info */
            ret_val = icf_send_to_app(p_crm_pdb->p_glb_pdb,
                p_msg,
                p_routing_info,
                p_ecode);
        }
        else
        {
            ret_val = icf_adm_process_mesg(p_crm_pdb->p_internal_msg);
        }

        ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_RESP_SENT)

        /* Free the memory allocated to the response message */

        if (ICF_SUCCESS == ret_val)
        {
            /* Free memory */
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
        }
        else
        {
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
            /* restore the return value of ICF_SEND */
            ret_val = ICF_FAILURE;
        }
    }
        
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


 /*******************************************************************************
 *
 * FUNCTION:        icf_cfg_application_add_hdlr
 *
 * DESCRIPTION:     This function is called when the API ICF_APP_ADD_REQ 
 *                  is received.
 *                  This function adds application context data. 
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched or response fails.
 *
 ******************************************************************************/
  icf_return_t icf_cfg_application_add_hdlr(
     INOUT   icf_crm_pdb_st *p_crm_pdb)
 {
     icf_return_t         ret_val = ICF_SUCCESS;
     icf_dbm_data_st      *p_dbm_data = ICF_NULL;
     icf_uint32_t         num_apps = 0;
     icf_uint8_t         app_index  = (icf_uint8_t)-1;
     icf_app_add_req_st   *p_payload = ICF_NULL;
     icf_app_conf_st      *p_app_conf = ICF_NULL;
     icf_int_filter_list_st *p_internal_filter_list=ICF_NULL;
     icf_error_t           ecode;
     icf_void_st           *p_routing_info=ICF_NULL;
     icf_app_id_t         app_id=ICF_INVALID_APP_ID;
     icf_boolean_t        app_already_present = ICF_FALSE;
     icf_uint8_t          *p_sptd_ctype = (icf_uint8_t*)"application/sdp";
	 icf_list_st				*p_src_list = ICF_NULL,
                          *p_next_node = ICF_NULL;
	 icf_stream_capab_st	*p_dest_data = ICF_NULL;

	 icf_uint32_t			local_stream =0;
     /*start change for  CSR No: 1-4326706 */
     icf_uint32_t counter =0 ;
     icf_header_name_st *p_current_node = ICF_NULL;
     icf_uint32_t loop_count = 0;
     icf_filter_st *p_curr_node = ICF_NULL;
     icf_string_list_node_st *p_current_nod = ICF_NULL;
     /*end change for  CSR No: 1-4326706 */
     
     ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb);

     ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
				(ICF_NULL != p_crm_pdb->p_glb_pdb->p_glb_data));

     p_payload = (icf_app_add_req_st *)p_crm_pdb->p_recv_payload;

     p_routing_info = &(p_payload->routing_info);

	 ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_crm_pdb->p_recv_payload))
		 
		 
	 ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_APP_ADD_RECD)

         
         icf_port_strcpy(p_crm_pdb->app_name.str,p_payload->app_name.str);


        ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_APP_ADD)
		 
	 /* Get DBM data to fetch application context */
	 if(ICF_FAILURE == icf_dbm_get_module_glb_data(     
			 p_crm_pdb->p_glb_pdb,
			 (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
			 (icf_void_t *)&p_dbm_data,
			 &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
	 {
		 p_crm_pdb->result = ICF_FAILURE;
		 p_crm_pdb->error_cause = ICF_CAUSE_INTERNAL_FAILURE;
	 }
	 /* Get config_data */
	 else if(ICF_FAILURE == icf_dbm_get_module_glb_data(     
		 p_crm_pdb->p_glb_pdb,
		 (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
		 (icf_void_t *)&p_crm_pdb->p_config_data,
		 &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
	 {
		 p_crm_pdb->result = ICF_FAILURE;
		 p_crm_pdb->error_cause = ICF_CAUSE_INTERNAL_FAILURE;
	 }

	 p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

     /*---------------------------------------------------------------------*/
     /*start change for  CSR No: 1-4326706 */
     for(counter = p_payload->header_name_list.count,
         p_current_node = p_payload->header_name_list.hdr;
         counter-- ; p_current_node = p_current_node->next)
       {
        p_current_node->hdr_name.str_len =
         icf_cfg_trim_string(p_current_node->hdr_name.str);
       }
     for(loop_count = p_payload->filter.count,
         p_curr_node = p_payload->filter.p_filter_list;
         loop_count--; p_curr_node =  p_curr_node->p_next)
       {
         p_curr_node->target_header.str_len =
           icf_cfg_trim_string_for_filter_list(p_curr_node->target_header.str);
         p_curr_node->expr_str.str_len =
           icf_cfg_trim_string_for_filter_list(p_curr_node->expr_str.str);  
       }
     for(counter = p_payload->supported_events.count,
          p_current_nod = p_payload->supported_events.p_list;
          counter--; p_current_nod =  p_current_nod->p_next)
       {
        p_current_nod->str.str_len =
          icf_cfg_trim_string(p_current_nod->str.str);
       }
     for(counter = p_payload->supported_methods.count,
         p_current_nod = p_payload->supported_methods.p_list;
         counter--; p_current_nod =  p_current_nod->p_next)
       {
         p_current_nod->str.str_len = icf_cfg_trim_string(p_current_nod->str.str);
       }
     for(counter = p_payload->supported_content_types.count,
          p_current_nod = p_payload->supported_content_types.p_list;
          counter--; p_current_nod =  p_current_nod->p_next)
       {
         p_payload->supported_content_types.p_list->str.str_len =
         icf_cfg_trim_string(p_current_nod->str.str);
       }
     for(counter = p_payload->supported_languages.count,
         p_current_nod = p_payload->supported_languages.p_list;
         counter--; p_current_nod =  p_current_nod->p_next)
       {
         p_current_nod->str.str_len =
         icf_cfg_trim_string(p_current_nod->str.str);
       }
     for(counter = p_payload->supported_encodings.count,
         p_current_nod = p_payload->supported_encodings.p_list;
         counter--; p_current_nod =  p_current_nod->p_next)
      {
         p_current_nod->str.str_len =
         icf_cfg_trim_string(p_current_nod->str.str);
      }   
     /*End change for  CSR No: 1-4326706 */
     /* Validate the request received */
     icf_cfg_validate_request_header(p_crm_pdb);
     
     icf_cfg_validate_app_add(p_crm_pdb);
     
     ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY);
     if (ICF_FALSE == p_crm_pdb->is_valid_param)
     {
         p_crm_pdb->result = ICF_FAILURE;

		/* Fix for CSR 1-7423906: SPR 19772 */
		 if(ICF_SUCCESS == icf_cfg_app_already_present(p_crm_pdb,
				 p_payload->app_name, 
#ifdef ICF_PORT_DEBIAN
				(icf_uint8_t *)&app_index))
#else
				(icf_uint16_t *)&app_index))
#endif
		 {
			 /* Application already present */
			 /* SPR 8664 fix start */
			 
			 /* Send call clear indication to CM for all active calls 
			 * associated with this application */
			 ret_val = icf_cfg_cm_call_clear_ind_wrt_app(
							p_crm_pdb,
							p_app_conf[app_index].app_id);
			 /* SPR 8664 fix end */

			app_already_present = ICF_TRUE;
		 }
     }
	 else if(ICF_FAILURE != p_crm_pdb->result)
	 {
		 /*-----------------------------------------------------------------*/
		/* Fix for CSR 1-7423906: SPR 19772 */
		 if(ICF_FAILURE == icf_cfg_app_already_present(p_crm_pdb,
			 p_payload->app_name, 
#ifdef ICF_PORT_DEBIAN
				(icf_uint8_t *)&app_index))
#else
				(icf_uint16_t *)&app_index))
#endif
		 {
			 /* Application is not already present */
			 
			 icf_cfg_get_num_apps(p_crm_pdb, &num_apps);
			 
			 p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;        
			    if(p_crm_pdb->p_config_data->max_applications == num_apps)
			    {
			        p_crm_pdb->result = ICF_FAILURE;
			        p_crm_pdb->error_cause = ICF_CAUSE_MAX_NUM_APPLICATION_EXCEEDED;
			        if(ICF_TRUE == p_crm_pdb->resp_to_be_sent)
			        {
					    p_crm_pdb->resp_to_be_sent = ICF_FALSE;
					    ret_val = icf_cfg_application_add_resp(p_crm_pdb, 
														p_routing_info, 
														ICF_INVALID_APP_ID);
				    }
				    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb);
				    return ret_val;
			    }
			    else
			    {
				    /* create application record in app_conf */
				    icf_cfg_get_vacant_app_index(p_crm_pdb, (icf_uint8_t*)&app_index);
				 
				    /* Initialize application context */
				    icf_port_memset(
					    (icf_void_t*)&p_app_conf[app_index],
					    (icf_uint8_t)0,
					    (icf_uint32_t)
					    ICF_PORT_SIZEOF(icf_app_conf_st));
				    p_app_conf[app_index].is_valid = ICF_FALSE;
				 
				    /* create application record in app_conf */
				    p_app_conf[app_index].is_valid = ICF_TRUE;
				    p_app_conf[app_index].app_id = app_index + 1;
				    icf_port_strcpy(p_app_conf[app_index].app_name.str, 
					                p_payload->app_name.str); 

				    p_app_conf[app_index].app_name.str_len = p_payload->app_name.str_len;
                }
		 }
		 else
		 {
			 /* Application already present */
			 /* SPR 8664 fix start */
			 
			 /* Send call clear indication to CM for all active calls 
			 * associated with this application */
			 ret_val = icf_cfg_cm_call_clear_ind_wrt_app(
				 p_crm_pdb,
				 p_app_conf[app_index].app_id);
			 /* SPR 8664 fix end */
			 app_already_present = ICF_TRUE;
		 }
		 
		 /* Free filter list if already present */
		 if(ICF_NULL !=p_app_conf[app_index].p_filter_list)
		 {
			 icf_cfg_free_internal_filter_list(p_crm_pdb,
				 p_app_conf[app_index].p_filter_list);
		 }
		 /* Parse the filter list and store it in internal filter structure */         
		 if(ICF_FAILURE == icf_cfg_parse_filter_list(p_crm_pdb,
			 &(p_payload->filter),
			 &p_internal_filter_list,
			 &ecode))			
		 {
			 p_crm_pdb->result = ICF_FAILURE;
			 p_crm_pdb->error_cause = ecode;
		 }
		 else
		 {
             icf_uint32_t count;
             icf_string_list_node_st *p_cur_node;

			 p_app_conf[app_index].p_filter_list = p_internal_filter_list;
			 
			 p_app_conf[app_index].routing_info.len = 
											p_payload->routing_info.len;
			 
			 if(p_payload->routing_info.ptr != ICF_NULL)
			 {
				 if (ICF_NULL != p_app_conf[app_index].routing_info.ptr)
				 {
					 /* free the memory allocated */
					 ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
						 p_app_conf[app_index].routing_info.ptr,
						 ICF_MEM_COMMON,
						 &(p_crm_pdb->p_internal_msg->msg_hdr.ecode), 
						 ret_val)
				 }
				 /* allocate memory for routing info */
				 ICF_MEMGET(p_crm_pdb->p_glb_pdb,
					 ICF_PORT_SIZEOF(icf_route_info_st),
					 ICF_MEM_COMMON,
					 p_app_conf[app_index].routing_info.ptr,
					 ICF_RET_ON_FAILURE,
					 &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
					 ret_val);
				 
				 icf_port_memcpy(
					 (icf_void_t*)(icf_route_info_st *)p_app_conf[app_index].\
						routing_info.ptr,
					 (icf_void_t*)p_payload->routing_info.ptr,
					 (icf_uint32_t) ICF_PORT_SIZEOF(icf_route_info_st));
			 }

             p_app_conf[app_index].app_source_id = p_crm_pdb->p_internal_msg->\
                 msg_hdr.p_glb_pdb->p_recv_msg->hdr.source_id;
			 
			 if (ICF_NULL != p_app_conf[app_index].hdr_list.count)
			 {
			/* Free the header list that is already present
			* This may happen if memory has not been freed 
			* on application remove */
				 icf_cmn_free_header_name_list(
					 p_crm_pdb->p_glb_pdb,
					 &(p_app_conf[app_index].hdr_list),
					 &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));
				 p_app_conf[app_index].hdr_list.count = ICF_NULL;
			 }
		 
			 icf_cmn_clone_header_name_list(
				 p_crm_pdb->p_glb_pdb,
				 &p_payload->header_name_list,
				 &p_app_conf[app_index].hdr_list,
				 &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));
             
             /* Free existing app_profile if exists */
             icf_cfg_free_app_profile(p_crm_pdb,&(p_app_conf[app_index].\
										app_profile));

             /* Check if application supports invite and 
			 *  set the basic methods flag */

             p_app_conf[app_index].app_profile.basic_method_support 
															= ICF_FALSE;

             for(count = p_payload->supported_methods.count, 
				 p_cur_node = p_payload->supported_methods.p_list;
				 count--;p_cur_node = p_cur_node->p_next)
             {
                 if(ICF_NULL != icf_port_strstr((const icf_int8_t *)"INVITE",(const icf_int8_t *) p_cur_node->str.str))
                 {
                     p_app_conf[app_index].app_profile.basic_method_support 
																	= ICF_TRUE;
                     break;
                 }
             }

             /* Assign the new app_profile */
             icf_cfg_move_string_list
				 (&(p_app_conf[app_index].app_profile.supported_methods),
                 &(p_payload->supported_methods));

             icf_cfg_move_string_list
				 (&(p_app_conf[app_index].app_profile.supported_events),
                 &(p_payload->supported_events)); 

			/* Message Body Header Changes Start
			*/
			 /* If Content-Type are not specified in the request then
              * Set supported content type as application/sdp.
			  */
			 if(0 == p_payload->supported_content_types.count)
			 {
				 ICF_MEMGET(p_crm_pdb->p_glb_pdb,
                   ICF_PORT_SIZEOF(icf_string_list_node_st),
                   ICF_MEM_COMMON,
                   p_payload->supported_content_types.p_list,
                   ICF_RET_ON_FAILURE,
                   &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                   ret_val)
				 
				 
                 icf_port_memcpy(
                        &(p_payload->supported_content_types.p_list->str.str),
                        (p_sptd_ctype),icf_port_strlen(p_sptd_ctype)+1);

				 p_payload->supported_content_types.p_list->str.str_len = \
					 (icf_uint16_t) icf_port_strlen(p_sptd_ctype);

				 p_payload->supported_content_types.count++;

			 }
#if 0
			 else
			 {
				 p_list = p_payload->supported_content_types.p_list;
				 do
				 {
					p_temp = p_list;
                    if(0 == icf_port_strcmp(p_list->str.str,"application/sdp"))
					{
						 match_found = ICF_TRUE;
						 break;
					}
					p_list = p_list->p_next;
				}while(ICF_NULL != p_temp->p_next);

				 if(ICF_FALSE == match_found)
				 {
					 ICF_MEMGET(p_crm_pdb->p_glb_pdb,
                   ICF_PORT_SIZEOF(icf_string_list_node_st),
                   ICF_MEM_COMMON,
                   p_temp->p_next,
                   ICF_RET_ON_FAILURE,
                   &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                   ret_val)
				   icf_port_memcpy(
                        &(p_temp->p_next->str.str),(p_sptd_ctype),\
						icf_port_strlen(p_sptd_ctype)+1);

				  p_temp->p_next->str.str_len = icf_port_strlen(p_sptd_ctype);
                  p_payload->supported_content_types.count++;

				 }
			 }
#endif
			 /* Message Body Header Changes Ends
			 */
             icf_cfg_move_string_list
				 (&(p_app_conf[app_index].app_profile.supported_content_types),
                 &(p_payload->supported_content_types));

             icf_cfg_move_string_list
				 (&(p_app_conf[app_index].app_profile.supported_encodings),
                 &(p_payload->supported_encodings));

             icf_cfg_move_string_list
				 (&(p_app_conf[app_index].app_profile.supported_languages),
                 &(p_payload->supported_languages));

             /* tokenize the string lists */
             icf_cmn_tokenize_string_list(p_crm_pdb->p_glb_pdb,
                 &(p_app_conf[app_index].app_profile.supported_methods));
             icf_cmn_tokenize_string_list(p_crm_pdb->p_glb_pdb,
                 &(p_app_conf[app_index].app_profile.supported_events));
             icf_cmn_tokenize_string_list(p_crm_pdb->p_glb_pdb,
                 &(p_app_conf[app_index].app_profile.supported_content_types));
             icf_cmn_tokenize_string_list(p_crm_pdb->p_glb_pdb,
                 &(p_app_conf[app_index].app_profile.supported_encodings));
             icf_cmn_tokenize_string_list(p_crm_pdb->p_glb_pdb,
                 &(p_app_conf[app_index].app_profile.supported_languages));
             
             if(ICF_FAILURE == icf_cfg_validate_supported_method_list(p_crm_pdb,
                                 &(p_app_conf[app_index].app_profile.supported_methods)))
             {
                  p_crm_pdb->result = ICF_FAILURE;
                  p_crm_pdb->error_cause = ICF_CAUSE_UNSUPPORTED_METHOD_IN_LIST;
             }
             else
             {
                  p_crm_pdb->result = ICF_SUCCESS;
             }
                     /*Fix for CSR 1-6792719*/
             /*Adding the check for result in p_crm_pdb*/
             /*This is done as a part of Klockwork warning removal.If there is
              * some failure in handling ICF_APP_ADD_REQ and failure response
              * need to be send , there is no need to populate the media
              * profile in p_app_conf.Also in this scenario 
              * p_payload->p_media_profile will be freed*/
         
/*ICF REL 6.0*/
		    if((ICF_FAILURE != p_crm_pdb->result) &&
               (p_payload->bit_mask & ICF_APP_ADD_MEDIA_PROFILE_PRESENT) && \
			(p_payload->p_media_profile))
			{
				if (p_app_conf[app_index].p_app_media_profile != NULL)
				{
					/*free the complete list */
					icf_cmn_free_stream_capab_list(p_crm_pdb->p_glb_pdb,
				 					p_app_conf[app_index].p_app_media_profile);
                    p_app_conf[app_index].p_app_media_profile = ICF_NULL;
				}


				p_src_list = p_payload->p_media_profile;

				while(ICF_NULL != p_src_list)
				{
					p_dest_data = (icf_stream_capab_st*)p_src_list->p_data;
					
					local_stream = 	((icf_stream_capab_st *)p_src_list->p_data)->stream_type;
					icf_cmn_add_node_to_internal_list
							(p_crm_pdb->p_glb_pdb,
							&p_app_conf[app_index].p_app_media_profile,
							p_dest_data,
							&(local_stream),
							ICF_NULL);


		/*Populating call type in app_conf_st depending upon the stream type*/

					switch(((icf_stream_capab_st *)p_src_list->p_data)\
						->stream_type)
					{
						case 	ICF_STREAM_TYPE_AUDIO:
								p_app_conf[app_index].call_type |=  
														ICF_CALL_TYPE_AUDIO;
								break;

						case 	ICF_STREAM_TYPE_VIDEO:
								p_app_conf[app_index].call_type |= 
														ICF_CALL_TYPE_VIDEO;
								break;

						case 	ICF_STREAM_TYPE_TBCP:
								p_app_conf[app_index].call_type |=  
														ICF_CALL_TYPE_TBCP;
								break;

						case 	ICF_STREAM_TYPE_FAX:
								p_app_conf[app_index].call_type |=  
														ICF_CALL_TYPE_T38;
								break;

						case 	ICF_STREAM_TYPE_INVALID:
								p_app_conf[app_index].call_type |=  
														ICF_CALL_TYPE_INVALID;
								break;

						default:
								p_app_conf[app_index].call_type |=  
														ICF_CALL_TYPE_INVALID;
								break;			
					}

					
					p_src_list = (icf_list_st *)p_src_list->p_next;
				}
                p_src_list = p_payload->p_media_profile;
                for(;p_src_list != ICF_NULL; p_src_list = p_next_node)
                {
                    p_next_node = p_src_list->p_next;
                    icf_cmn_memfree(p_crm_pdb->p_glb_pdb,p_src_list,ICF_MEM_COMMON, p_crm_pdb->p_ecode);
                }
			}

            if(p_payload->icf_simul_call_line_arbitration)
			{
				p_app_conf[app_index].app_profile.\
					icf_simul_call_line_arbitration = ICF_TRUE;
			}
			else
			{
				p_app_conf[app_index].app_profile.\
					icf_simul_call_line_arbitration = ICF_FALSE;
			}
            if (ICF_TRUE == p_payload->merge_req_wo_rem_sdp)
            {
                p_app_conf[app_index].app_profile.merge_req_wo_rem_sdp = 
                     ICF_TRUE;
            }
            else
            {
                p_app_conf[app_index].app_profile.merge_req_wo_rem_sdp = 
                     ICF_FALSE;
            }
		 }
	}
    
	if(ICF_TRUE == p_crm_pdb->resp_to_be_sent)
	{
		p_crm_pdb->resp_to_be_sent = ICF_FALSE;

		if(ICF_FAILURE == p_crm_pdb->result)
		{
            if (p_app_conf[app_index].p_app_media_profile != NULL)
            {
                /*free the complete list */
                icf_cmn_free_stream_capab_list(p_crm_pdb->p_glb_pdb,
                    p_app_conf[app_index].p_app_media_profile);
                p_app_conf[app_index].p_app_media_profile = ICF_NULL;
            }
            
			if (ICF_TRUE == app_already_present)
			{
				icf_cfg_remove_app_ctxt(p_crm_pdb, 
					p_app_conf[app_index].app_id);
			}
            /*Fix For CSR 1-6792719*/
            /*Check whether application has define its preference for media 
              streams, codecs (and other parameters) within a media stream
              using ICF_APP_ADD_REQ API.As IPTK is returning failure for 
              this API (for the scenario when application does not specify any
              method in supported_methods) free the media profile from 
              p_payload before sending the failure response.*/

		    if((p_payload->bit_mask & ICF_APP_ADD_MEDIA_PROFILE_PRESENT) && 
                    (ICF_NULL != p_payload->p_media_profile))
			{
                icf_cmn_free_stream_capab_list_stored_as_icf_list(
                                             p_crm_pdb->p_glb_pdb,
                                             p_payload->p_media_profile);    
            }

			app_id = ICF_INVALID_APP_ID;
			p_routing_info = &(p_payload->routing_info);
		}
		else
		{
			p_routing_info = ICF_NULL;
			app_id = p_app_conf[app_index].app_id;
		}

		ret_val = icf_cfg_application_add_resp(p_crm_pdb,
											p_routing_info,app_id);
	}

	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val; 
}




/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_application_remove_resp
 *
 * DESCRIPTION:     This function sends response to external Application 
 *                  Module for a request message.
 *
 * RETURNS:         ICF_SUCESS in case of success and ICF_FAILURE in 
 *                  case of failure.
 *
 *****************************************************************************/
icf_return_t icf_cfg_application_remove_resp(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_app_id_t app_id)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_error_t                          *p_ecode = ICF_ERROR_NONE;
    icf_msg_st                           *p_msg = ICF_NULL;
    icf_uint16_t                         api_len = 0;
    icf_app_remove_cfm_st                *p_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the ecode pointer locally */
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_ecode))

    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
                    ICF_PORT_SIZEOF(icf_app_remove_cfm_st);

    /* Allocate memory to the response message strucure */
    ICF_MEMGET(
            p_crm_pdb->p_glb_pdb,
            api_len,
            ICF_MEM_COMMON,
            p_msg,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    
    if(ICF_NULL != p_msg)
    {
        
        p_payload = (icf_app_remove_cfm_st *) p_msg->payload;

		p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_APP_REMOVE_CFM;
        
        /* API-ID to send */
        p_msg->hdr.api_id = ICF_APP_REMOVE_CFM;
        
        /*Payload length is size of api header + response structure*/
        p_msg->hdr.api_length = api_len;

        p_msg->hdr.call_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
        
        /*Result of request received success or failure*/
        p_payload->result = p_crm_pdb->result;

        /*error_cause, valid if result is failure*/
        p_payload->error_cause = p_crm_pdb->error_cause;     

        /* APP-ID to send */
        p_msg->hdr.app_id = app_id;

        /* Send the response message */
        p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;
        ret_val = icf_adm_process_mesg(p_crm_pdb->p_internal_msg);
        ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_RESP_SENT)

        /* Free the memory allocated to the response message */
        if (ICF_SUCCESS == ret_val)
        {
            /* Free memory */
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
        }
        else
        {
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
            /* restore the return value of ICF_SEND */
            ret_val = ICF_FAILURE;
        }
    }
        
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_application_remove_hdlr
 *
 * DESCRIPTION:     This function is called when the API ICF_APP_REMOVE_REQ 
 *                  is received.
 *                  This function adds application context data. 
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched or response fails.
 *
 ******************************************************************************/
 icf_return_t icf_cfg_application_remove_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_app_conf_st      *p_app_conf_1 = ICF_NULL;
 	icf_dbm_data_st      *p_dbm_data = ICF_NULL;
    icf_uint8_t           app_index=0; 

    icf_app_conf_st      *p_app_conf = ICF_NULL;
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_crm_pdb->p_glb_pdb->p_glb_data))
    
    /*---------------------------------------------------------------------*/

    /* Validate the request received */
    icf_cfg_validate_request_header(p_crm_pdb);
    
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    if (ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        p_crm_pdb->result = ICF_FAILURE;
        if(ICF_TRUE == p_crm_pdb->resp_to_be_sent)
        {
            p_crm_pdb->resp_to_be_sent = ICF_FALSE;
            ret_val = icf_cfg_application_remove_resp(p_crm_pdb,p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id);
        }
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val; 
    }

    /*----------------------------------------------------------------------*/

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_crm_pdb->p_recv_payload))

    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_APP_REMOVE_RECD)

    /* Get DBM data to fetch network access context */
    if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
                                  p_crm_pdb->p_glb_pdb,
                                  (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                                  (icf_void_t *)&p_dbm_data,
                                  &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
        p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

        if(ICF_SUCCESS == icf_cfg_fetch_app_conf(p_crm_pdb->p_glb_pdb,
                                        p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id, 
                                        &p_app_conf_1,
                                        &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {
            /* SPR 8068 fix start */
            /* Indicate REGM about Application that is being removed */

            icf_rgm_remove_app_cntxt(p_crm_pdb,
                    p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id,
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));
            p_crm_pdb->resp_to_be_sent = ICF_TRUE;
            /* SPR 8068 fix end */

ICF_NW_RECOVERY_START
			/* SPR 8655 fix start */

			/* Send call clear indication to CM for all active calls */
			ret_val = icf_cfg_cm_call_clear_ind_wrt_app(
						p_crm_pdb,p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id);

            /* SPR 8655 fix end */
ICF_NW_RECOVERY_END



            
            icf_cfg_fetch_app_index(p_crm_pdb, p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id, &app_index);


            if (ICF_NULL != p_app_conf[app_index].hdr_list.count)
            {
                /* Free the header list that is already present
                 * This may happen if memory has not been freed on application remove */
                icf_cmn_free_header_name_list(
                    p_crm_pdb->p_glb_pdb,
                    &(p_app_conf[app_index].hdr_list),
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));
                p_app_conf[app_index].hdr_list.count = ICF_NULL;
            }
			/* Free the internal filter list */
            if(ICF_NULL != (p_app_conf[app_index]).p_filter_list)
            {
                icf_cfg_free_internal_filter_list(p_crm_pdb,
                    (p_app_conf[app_index]).p_filter_list);
                (p_app_conf[app_index]).p_filter_list = ICF_NULL;
            }

            /* Free app_profile  */
            icf_cfg_free_app_profile(p_crm_pdb,&(p_app_conf[app_index].app_profile));
			
			/* free the media profile pointer within the app_conf for this application */
			icf_cmn_free_stream_capab_list(p_crm_pdb->p_glb_pdb,
				 					p_app_conf[app_index].p_app_media_profile);
            p_app_conf[app_index].p_app_media_profile = ICF_NULL;

            p_crm_pdb->result = ICF_SUCCESS;

			if(ICF_TRUE == p_crm_pdb->resp_to_be_sent)
			{
				p_crm_pdb->resp_to_be_sent = ICF_FALSE;
				
				ret_val = icf_cfg_application_remove_resp(p_crm_pdb,p_app_conf[app_index].app_id);
			}
			
			
			if(ICF_SUCCESS == p_crm_pdb->result)
			{
			/* fetch app_conf is used while sending response hence is_valid 
				* set to false after sending response */
				p_app_conf[app_index].is_valid = ICF_FALSE;
				
				/* We are freeing the routing info after sending the response 
				as routing info is needed for sending app_remove_resp */
				
				if (ICF_NULL != p_app_conf[app_index].routing_info.ptr)
				{
					/* free the memory allocated */
					ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
						p_app_conf[app_index].routing_info.ptr,
						ICF_MEM_COMMON,
						&(p_crm_pdb->p_internal_msg->msg_hdr.ecode), 
						ret_val)
				}
			}

        }
	}

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; 
}


ICF_CRM_END
ICF_CONF_STATUS_API_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_get_system_settings_req_hdlr
 *
 * DESCRIPTION:     This function is called when Application triggers req 
 *                  to fetch system settings.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_get_system_settings_req_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t                      ret_val = ICF_SUCCESS;
    icf_get_system_settings_req_st   *p_recv_msg_payload = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, \
                (ICF_NULL != p_crm_pdb->p_glb_pdb->p_glb_data))

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, \
            (ICF_NULL != p_crm_pdb->p_recv_payload))

    p_recv_msg_payload = (icf_get_system_settings_req_st *)\
        p_crm_pdb->p_recv_payload;

    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_GET_SYSTEM_SETTINGS_RECD) 

   /*---------------------------------------------------------------------*/

   /* Get config_data */
   if(ICF_SUCCESS == icf_dbm_get_module_glb_data(\
       p_crm_pdb->p_glb_pdb,
       (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
       (icf_void_t *)&p_crm_pdb->p_config_data,
       &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
   {

   /* If the app-id is GENERIC app_id, 
    * replace/refresh the generic_routing_info
    * present in p_crm_pdb->p_config_data->p_generic_routing_info
    * by the routing info freshly received in 
    * get_system_settings api from application */



       if( p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id \
           == ICF_GENERIC_APP_ID)
       {
               if((ICF_NULL != (ICF_GET_SETTINGS_ROUTE_INFO_PRESENT & \
                            p_recv_msg_payload->bit_mask)) &&
              (p_recv_msg_payload->routing_info.ptr != ICF_NULL))
           {
               if (ICF_NULL != p_crm_pdb->p_config_data->\
                   p_generic_routing_info)
               {
                   if (ICF_NULL != p_crm_pdb->p_config_data->\
                        p_generic_routing_info->ptr)
                   {
                       /* free the memory allocated */
                       ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
                           p_crm_pdb->p_config_data->p_generic_routing_info->ptr,
                           ICF_MEM_COMMON,
                           &(p_crm_pdb->p_internal_msg->msg_hdr.ecode), 
                           ret_val)
                   }
                       
                   /* free the memory allocated */
                   ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
                       p_crm_pdb->p_config_data->p_generic_routing_info,
                       ICF_MEM_COMMON,
                       &(p_crm_pdb->p_internal_msg->msg_hdr.ecode), 
                       ret_val)
               }
               
               /* allocate memory for routing info */
               ICF_MEMGET(p_crm_pdb->p_glb_pdb,
                   ICF_PORT_SIZEOF(icf_void_st),
                   ICF_MEM_COMMON,
                   p_crm_pdb->p_config_data->p_generic_routing_info,
                   ICF_RET_ON_FAILURE,
                   &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                   ret_val) 
                   
               /* allocate memory for routing info */
               ICF_MEMGET(p_crm_pdb->p_glb_pdb,
                   ICF_PORT_SIZEOF(icf_route_info_st),
                   ICF_MEM_COMMON,
                   p_crm_pdb->p_config_data->p_generic_routing_info->ptr,
                   ICF_RET_ON_FAILURE,
                   &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                   ret_val)
                   
               p_crm_pdb->p_config_data->p_generic_routing_info->len =\
                   p_recv_msg_payload->routing_info.len;
               
               icf_port_memcpy(
                   (icf_void_t*)(icf_route_info_st *)\
                   p_crm_pdb->p_config_data->p_generic_routing_info->ptr,
                   (icf_void_t*)p_recv_msg_payload->routing_info.ptr,
                   (icf_uint32_t)\
                   ICF_PORT_SIZEOF(icf_route_info_st));

               ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
                      p_recv_msg_payload->routing_info.ptr,
                       ICF_MEM_COMMON,
                       &(p_crm_pdb->p_internal_msg->msg_hdr.ecode), ret_val)

           }
       }
   }
   else
   {
        /* System failure */
   }

   /* Validate the request received */
   icf_cfg_validate_request_header(p_crm_pdb); 

   if(ICF_SUCCESS != icf_cfg_get_app_id_validity(p_crm_pdb->p_glb_pdb,\
                            p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id) )
   {
       if( ICF_GENERIC_APP_ID != p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id)
       {   
           p_crm_pdb->is_valid_param = ICF_FALSE;
           p_crm_pdb->error_cause = ICF_CAUSE_INVALID_APP_ID;
       }
   }

    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    if (ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        p_crm_pdb->result = ICF_FAILURE;
        if(ICF_TRUE == p_crm_pdb->resp_to_be_sent)
        {
            p_crm_pdb->resp_to_be_sent = ICF_FALSE;
            p_crm_pdb->result = ICF_FAILURE;
            ret_val = icf_cfg_send_system_settings_resp(p_crm_pdb); 
        }
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val; 
    }
        
    /*----------------------------------------------------------------------*/

    /* Now, send back the response for the get_system_settings_request */
    
    ret_val = icf_cfg_send_system_settings_resp(p_crm_pdb); 
    

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_send_system_settings_resp
 *
 * DESCRIPTION:     This function is called when ICF wants to send 
 *                  resp to application for get-system-settings API 
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_send_system_settings_resp(
        INOUT   icf_crm_pdb_st                  *p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint32_t                     api_len = 0; 
    icf_msg_st                       *p_msg = ICF_NULL;
    icf_get_system_settings_resp_st  *p_payload = ICF_NULL;
    icf_get_system_settings_req_st   *p_recv_msg_payload = ICF_NULL;
    icf_error_t                        *p_ecode = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, \
                    (ICF_NULL != p_crm_pdb->p_glb_pdb->p_glb_data))
   
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    p_recv_msg_payload = (icf_get_system_settings_req_st *)\
        p_crm_pdb->p_recv_payload;


    api_len = ICF_PORT_SIZEOF(icf_api_header_st) +\
        ICF_PORT_SIZEOF(icf_get_system_settings_resp_st);
    
    /* Allocate memory to the response message strucure */
    ICF_MEMGET(p_crm_pdb->p_glb_pdb,api_len,
        ICF_MEM_COMMON,p_msg,
        ICF_RET_ON_FAILURE,p_ecode,ret_val)
    
    if(ICF_NULL != p_msg)
    { 
        p_payload = (icf_get_system_settings_resp_st *)\
            p_msg->payload;

        icf_port_memset((icf_void_t*)p_payload,
                                ICF_NULL,
                                (icf_uint32_t)
                                ICF_PORT_SIZEOF(icf_get_system_settings_resp_st));

        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_GET_SYSTEM_SETTINGS_RESP;
        p_msg->hdr.api_id = ICF_GET_SYSTEM_SETTINGS_RESP;
        
        p_msg->hdr.app_id = p_crm_pdb->p_glb_pdb->\
                                  p_recv_msg->hdr.app_id; 
        p_msg->hdr.api_length = (icf_uint16_t) api_len;
        p_msg->hdr.call_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
        /*Result of request received success or failure*/
        /* bit_mask indicates the presence of the optional parameters */
        p_payload->bit_mask = 0;  
    }
    
    /* Get config_data */
        if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
            p_crm_pdb->p_glb_pdb,
            (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
            (icf_void_t *)&p_crm_pdb->p_config_data,
            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {

                
            if(ICF_SUCCESS == p_crm_pdb->result) 
            {
                /* Set response to success */
                p_payload->bit_mask |= ICF_API_GET_SYS_STG_RESULT_SUCCESS;

                /** ######### PRACK STATUS ############ */
                if (ICF_NULL != (ICF_REQD_PRACK_ENABLED_STATUS & \
                            p_recv_msg_payload->bit_mask))  
                {
                    if(ICF_NULL != (ICF_CFG_CC_PRACK_REQD & \
                                p_crm_pdb->p_config_data->default_settings)) 

                    {
                        /** From config data ?prack is enabled in system **/
                        p_payload->prack_enabled = ICF_TRUE;

                    }
                    else 
                    {

                        /** From config data -> prack is disabled in system **/
                        p_payload->prack_enabled = ICF_FALSE; 
                    }
                    p_payload->bit_mask |= ICF_REQD_PRACK_ENABLED_STATUS;

                }

                /** ######### AUTHENTICATION STATUS ############ */
                if (ICF_NULL != (ICF_REQD_AUTH_TYPE_STATUS & \
                            p_recv_msg_payload->bit_mask)) 
                { 
                    p_payload->auth_type= p_crm_pdb->p_config_data->auth_type; 
                
                    p_payload->bit_mask |= ICF_REQD_AUTH_TYPE_STATUS;
                }

                /** ######### PRIVACY SUPPORTED FLAG STATUS ############ */
                if (ICF_NULL != (ICF_REQD_PRIVACY_SUPPORTED &\
                            p_recv_msg_payload->bit_mask))  
                { 
                    if(ICF_NULL != (ICF_CFG_CC_PRIVACY_HEADER_PRESENT &\
                                p_crm_pdb->p_config_data->default_settings)) 
                    {
                        /** From config data-> privacy header 
                         * is enabled in system **/
                        p_payload->privacy_supported = ICF_TRUE;  
                    }
                    else 
                    {
                        /** From config data-> privacy header
                         * is disabled in system **/
                        p_payload-> privacy_supported = ICF_FALSE;
                    }
                    p_payload->bit_mask |= ICF_REQD_PRIVACY_SUPPORTED;
                }

                /** ######### MAX LINES VALUE STATUS ############ */
                if (ICF_NULL != (ICF_REQD_MAX_LINES & \
                            p_recv_msg_payload->bit_mask))   
                {  
                        p_payload->max_lines = \
                            p_crm_pdb->p_config_data->max_lines; 
                    
                        p_payload->bit_mask |= ICF_REQD_MAX_LINES;
                }

          /* ########## TIMERS ##################  */

                /* ##### SETUP TIMER ##### */
                if (ICF_NULL != (ICF_REQD_SETUP_TIMER & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the timer to the response API's payload. **/
                    
                    p_payload->setup_timer =\
                            p_crm_pdb->p_config_data->setup_timer;
                    
                    p_payload->bit_mask |= ICF_REQD_SETUP_TIMER;
                }

                /* ##### ALERTING TIMER ##### */
                if (ICF_NULL != (ICF_REQD_ALERTING_TIMER & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the timer to the response API's payload. **/
                    p_payload->alerting_timer =\
                            p_crm_pdb->p_config_data->alerting_timer;
                    
                    p_payload->bit_mask |= ICF_REQD_ALERTING_TIMER;
                }

                /* ##### RELEASE TIMER ##### */
                if (ICF_NULL != (ICF_REQD_RELEASE_TIMER & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the timer to the response API's payload. **/
                    p_payload->release_timer =\
                            p_crm_pdb->p_config_data->release_timer;
                    
                    p_payload->bit_mask |= ICF_REQD_RELEASE_TIMER;
                }

                /* ###### MODIFY_MEDIA TIMER ###### */
                if (ICF_NULL != (ICF_REQD_MODIFY_MEDIA_TIMER & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the timer to the response API's payload. **/
                    p_payload->modify_media_timer =\
                            p_crm_pdb->p_config_data->modify_media_timer;

                    p_payload->bit_mask |= ICF_REQD_MODIFY_MEDIA_TIMER;
                }

                /* ######  REGISTRATION_RESP TIMER ###### */
                if (ICF_NULL != (ICF_REQD_REG_RESP_TIMER & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the timer to the response API's payload. **/
                    p_payload->registration_resp_timer =\
                            p_crm_pdb->p_config_data->\
                            registration_resp_timer;

                    p_payload->bit_mask |= ICF_REQD_REG_RESP_TIMER;
                }

                /* ###### REGISTRATION_RETRY TIMER ###### */
                if (ICF_NULL != (ICF_REQD_REG_RETRY_TIMER & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the timer to the response API's payload. **/
                    p_payload->\
                            registration_retry_timer =\
                            p_crm_pdb->p_config_data->\
                            registration_retry_timer;

                    p_payload->bit_mask |= ICF_REQD_REG_RETRY_TIMER;
                }

                /* #######  SUBSCRIBE_RETRY TIMER ####### */
                if (ICF_NULL != (ICF_REQD_SUBS_RETRY_TIMER & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the timer to the response API's payload. **/
                    p_payload->subs_retry_timer =\
                            p_crm_pdb->p_config_data->subs_retry_timer;

                    p_payload->bit_mask |= ICF_REQD_SUBS_RETRY_TIMER;
                }

                /* ####### SUBSCRIBE_RESP TIMER ####### */
                if (ICF_NULL != (ICF_REQD_SUBS_RESP_TIMER & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the timer to the response API's payload. **/
                    p_payload->subs_resp_timer =\
                            p_crm_pdb->p_config_data->subs_resp_timer;

                    p_payload->bit_mask |= ICF_REQD_SUBS_RESP_TIMER;
                }

                 /* ############## Signal Compression status ############## */
                if (ICF_NULL != (ICF_REQD_SIG_COMP_STATUS & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    p_payload->sig_comp_enabled = \
                        p_crm_pdb->p_config_data-> sigcomp_required; 

                    p_payload->bit_mask |= ICF_REQD_SIG_COMP_STATUS;
                }

                /* ############## Security status ############## */

                /* ############# Self IP address #############  */
                if (ICF_NULL != (ICF_REQD_SELF_IP_ADDRESS & \

                    p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the self ip address structure
                     * to the response API's payload. **/
                    
                    icf_port_memcpy(
                            (icf_void_t*)&(p_payload->self_ip_address),\
                            (icf_void_t*)&(p_crm_pdb->\
                                      p_config_data->self_ip_address),\
                            (icf_uint32_t)\
                            ICF_PORT_SIZEOF(icf_transport_address_st)\
                            );

                    p_payload->bit_mask |= ICF_REQD_SELF_IP_ADDRESS;
                }


                /* ############# Self Transport Mode #############  */
                if (ICF_NULL != (ICF_REQD_SELF_TRANSPORT_MODE & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the self transport mode
                     * to the response API's payload. **/
                    
                    p_payload->self_transport_mode = \
                                   p_crm_pdb->p_config_data->self_mode;
                            
                    p_payload->bit_mask |= ICF_REQD_SELF_TRANSPORT_MODE;
                }

                
                /* ############# Self Port #############  */
                if (ICF_NULL != (ICF_REQD_SELF_PORT & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the self port
                     * to the response API's payload. **/
                    
                    p_payload->self_port = \
                         p_crm_pdb->p_config_data->self_ip_address.port_num;
                            
                    p_payload->bit_mask |= ICF_REQD_SELF_PORT;
                }
                
                /* ############# Call Thru Proxy #############  */
                if (ICF_NULL != (ICF_REQD_CALL_THRU_PROXY_STATUS & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the call thru proxy status from config_data
                    to the response API's payload. **/
                                                         
                    if(ICF_NULL != (p_crm_pdb->p_config_data->default_settings & \
                                        ICF_CFG_CC_CALL_THRU_SIP_PROXY))
                            
                    {
                        p_payload->call_thru_proxy = ICF_TRUE;
                    }
                    else
                    {
                        p_payload->call_thru_proxy = ICF_FALSE;
                    }

                    p_payload->bit_mask |= ICF_REQD_CALL_THRU_PROXY_STATUS;
                }
                
                /* ############# Sip Compact Header #############  */
                if (ICF_NULL != (ICF_REQD_SIP_COMPACT_HDR_STATUS & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the sip compact header from config_data
                    to the response API's payload. **/
                                                         
                    p_payload->sip_compact_header = ICF_TRUE;
                
                    p_payload->bit_mask |= ICF_REQD_SIP_COMPACT_HDR_STATUS;
                }

                if (ICF_NULL != (ICF_REQD_QOS_MARK & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the sip compact header from config_data
                    to the response API's payload. **/
                                                         
                      p_payload->qos_mark = \
                        p_crm_pdb->p_config_data->qos_mark;
                
                    p_payload->bit_mask |= ICF_REQD_QOS_MARK;
                }

                
                /* ############# Session Interval Timer #############  */
#ifdef ICF_SESSION_TIMER
                if (ICF_NULL != (ICF_REQD_SESSION_INTERVAL_TIMER & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the sip compact header from config_data
                    to the response API's payload. **/
                                                       
                      p_payload->session_interval_timer = \
                        p_crm_pdb->p_config_data->session_timer;
                
                    p_payload->bit_mask |= ICF_REQD_SESSION_INTERVAL_TIMER;
                }
                
                /* #############  Interval Timer #############  */
                if (ICF_NULL != (ICF_REQD_MIN_SESSION_INTERVAL_TIMER   & \
                            p_recv_msg_payload->bit_mask))    
                {  
                    /** Copy the sip compact header from config_data
                    to the response API's payload. **/
                                                         
                    p_payload->min_session_interval_timer = \
                        p_crm_pdb->p_config_data->min_se;
                
                    p_payload->bit_mask |= ICF_REQD_MIN_SESSION_INTERVAL_TIMER ;
                }
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
                /* ############# Binding query enable #############  */

                if (ICF_NULL != (ICF_REQD_BINDING_QUERY_ENABLED_STATUS & \
                            p_recv_msg_payload->bit_mask))  
                {
                    
                        p_payload->binding_query_enable= ICF_FALSE;

                }
                    
                /* ############# Binding refresh timer #############  */
                 
                if(ICF_FALSE == p_payload->binding_query_enable)
                {  
                     if (ICF_NULL != (ICF_REQD_BINDING_REFRESH_TIMER & \
                            p_recv_msg_payload->bit_mask))    
                     {  
                                                                         
                         p_payload->binding_refresh_timer = \
                         p_crm_pdb->p_config_data->rport_config_data.binding_refresh_timer;
                
                         p_payload->bit_mask |= ICF_REQD_BINDING_REFRESH_TIMER;
                     }
               }
#endif  /* ICF_NAT_RPORT_SUPPORT  */
            }
        } /* end of config_data */
         
        /* #######  SEND THE RESPONSE TO THE APPLICATION ####### */
                
        /* Send the response message */
        p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;
        ret_val = icf_adm_process_mesg(p_crm_pdb->p_internal_msg);
        ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_RESP_SENT)
        if(ICF_NULL != (p_payload->bit_mask & ICF_REQD_ASSO_URI_LIST))
        {
         icf_cfg_delete_public_uri_list (p_crm_pdb->p_glb_pdb,
                                         &(p_payload->p_uri_list),
                                         p_ecode);
        }
#ifdef ICF_NW_ACTIVATE
        if(ICF_NULL != (p_payload->bit_mask & ICF_REQD_APN_LIST))
        {
         ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
                     p_dst_apn_list,
                     ICF_MEM_COMMON,
                     p_ecode,
                     ret_val)
        }
#endif

        /* Free the memory allocated to the response message */
        if (ICF_SUCCESS == ret_val)
        {
            /* Free memory */
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
        }
        else
        {
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
            /* restore the return value */
            ret_val = ICF_FAILURE;
        }

ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_get_line_settings_req_hdlr
 *
 * DESCRIPTION:     This function is called when Application triggers req 
 *                  to fetch line settings.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_get_line_settings_req_hdlr(
        INOUT   icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t                      ret_val = ICF_SUCCESS;
    icf_get_line_settings_req_st        *p_recv_msg_payload = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, \
                (ICF_NULL != p_crm_pdb->p_glb_pdb->p_glb_data))

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, \
            (ICF_NULL != p_crm_pdb->p_recv_payload))

    p_recv_msg_payload = (icf_get_line_settings_req_st *)p_crm_pdb->p_recv_payload;

    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_GET_LINE_SETTINGS_RECD) 

    /*---------------------------------------------------------------------*/

   /* Get config_data */
   if(ICF_SUCCESS == icf_dbm_get_module_glb_data(\
       p_crm_pdb->p_glb_pdb,
       (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
       (icf_void_t *)&p_crm_pdb->p_config_data,
       &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
   {

   /* If the app-id is GENERIC app_id, 
    * replace/refresh the generic_routing_info
    * present in p_crm_pdb->p_config_data->p_generic_routing_info
    * by the routing info freshly received in 
    * get_system_settings api from application */
       
       if( p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id \
           == ICF_GENERIC_APP_ID)
       {
           
           if((ICF_NULL != (ICF_GET_SETTINGS_ROUTE_INFO_PRESENT & \
                            p_recv_msg_payload->bit_mask)) &&
              (p_recv_msg_payload->routing_info.ptr != ICF_NULL))
           {
               if (ICF_NULL != p_crm_pdb->p_config_data->\
                   p_generic_routing_info)
               {
                   if (ICF_NULL != p_crm_pdb->p_config_data->\
                        p_generic_routing_info->ptr)
                   {
                       /* free the memory allocated */
                       ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
                           p_crm_pdb->p_config_data->p_generic_routing_info->ptr,
                           ICF_MEM_COMMON,
                           &(p_crm_pdb->p_internal_msg->msg_hdr.ecode), 
                           ret_val)
                   }
                   
				   /*Fix done for SPR-18722 : CSR-1-6068965*/
                   /* free the memory allocated */
                   ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
                       p_crm_pdb->p_config_data->p_generic_routing_info,
                       ICF_MEM_COMMON,
                       &(p_crm_pdb->p_internal_msg->msg_hdr.ecode), 
                       ret_val)
               }
               
               /* allocate memory for routing info */
               ICF_MEMGET(p_crm_pdb->p_glb_pdb,
                   ICF_PORT_SIZEOF(icf_void_st),
                   ICF_MEM_COMMON,
                   p_crm_pdb->p_config_data->p_generic_routing_info,
                   ICF_RET_ON_FAILURE,
                   &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                   ret_val) 
                   
               /* allocate memory for routing info */
               ICF_MEMGET(p_crm_pdb->p_glb_pdb,
                   ICF_PORT_SIZEOF(icf_route_info_st),
                   ICF_MEM_COMMON,
                   p_crm_pdb->p_config_data->p_generic_routing_info->ptr,
                   ICF_RET_ON_FAILURE,
                   &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                   ret_val)
                   
               p_crm_pdb->p_config_data->p_generic_routing_info->len =\
                   p_recv_msg_payload->routing_info.len;
               
               icf_port_memcpy(
                   (icf_void_t*)(icf_route_info_st *)\
                   p_crm_pdb->p_config_data->p_generic_routing_info->ptr,
                   (icf_void_t*)p_recv_msg_payload->routing_info.ptr,
                   (icf_uint32_t)\
                   ICF_PORT_SIZEOF(icf_route_info_st));
           }
       }
   }
   else
   {
        /* System failure */
   }


   /* Validate the request received */
   icf_cfg_validate_request_header(p_crm_pdb); 

   if(ICF_SUCCESS != icf_cfg_get_app_id_validity(p_crm_pdb->p_glb_pdb,\
                            p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id) )
   {
       if( ICF_GENERIC_APP_ID != p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id)
       {   
           p_crm_pdb->is_valid_param = ICF_FALSE;
           p_crm_pdb->error_cause = ICF_CAUSE_INVALID_APP_ID;
       }
   }

   /* Fetch Line Data */
   
   if(ICF_SUCCESS == icf_cfg_check_line_configured(p_crm_pdb,
       (icf_boolean_t)ICF_FALSE))
   {
         if ((p_recv_msg_payload->line_id >= p_crm_pdb->p_config_data->max_lines)\
           || (ICF_TRUE != p_crm_pdb->lines_present\
           [p_recv_msg_payload->line_id])
           || (p_crm_pdb->p_line_data\
           [p_recv_msg_payload->line_id] == ICF_NULL))
       {
           p_crm_pdb->is_valid_param = ICF_FALSE;
       }
   }
   else
   {
       p_crm_pdb->is_valid_param = ICF_FALSE;
       p_crm_pdb->error_cause = ICF_CAUSE_INVALID_LINE_ID;
   }

    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    if (ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        p_crm_pdb->result = ICF_FAILURE;
        if(ICF_TRUE == p_crm_pdb->resp_to_be_sent)
        {
            p_crm_pdb->resp_to_be_sent = ICF_FALSE;
            p_crm_pdb->result = ICF_FAILURE;
            ret_val = icf_cfg_send_line_settings_resp(p_crm_pdb); 
        }
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val; 
    }

    /*----------------------------------------------------------------------*/

    /* Now, send back the response for the get_line_settings_request */
    
    ret_val = icf_cfg_send_line_settings_resp(p_crm_pdb);
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_send_line_settings_resp
 *
 * DESCRIPTION:     This function is called when ICF wants to send 
 *                  resp to application for get-line-settings API 
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_send_line_settings_resp(
        INOUT   icf_crm_pdb_st                  *p_crm_pdb)
{
    icf_return_t                      ret_val = ICF_SUCCESS;
    icf_uint32_t                      api_len = 0; 
    icf_msg_st                           *p_msg = ICF_NULL;
    icf_get_line_settings_resp_st   *p_payload = ICF_NULL;
    icf_get_line_settings_req_st   *p_recv_msg_payload = ICF_NULL;
    icf_error_t             *p_ecode = ICF_NULL;

    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, \
        (ICF_NULL != p_crm_pdb->p_glb_pdb->p_glb_data))
    
    p_recv_msg_payload = (icf_get_line_settings_req_st *)\
        p_crm_pdb->p_recv_payload;
   
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    
    api_len = ICF_PORT_SIZEOF(icf_api_header_st) +\
        ICF_PORT_SIZEOF(icf_get_line_settings_resp_st);
    
    /* Allocate memory to the response message strucure */
    ICF_MEMGET(p_crm_pdb->p_glb_pdb,api_len,
        ICF_MEM_COMMON,p_msg,
        ICF_RET_ON_FAILURE,p_ecode,ret_val)
        if(ICF_NULL != p_msg)
        { 
            p_payload = (icf_get_line_settings_resp_st *)\
                p_msg->payload;

			p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_GET_LINE_SETTINGS_RESP;

            p_msg->hdr.api_id = ICF_GET_LINE_SETTINGS_RESP;

            p_msg->hdr.app_id = p_crm_pdb->p_glb_pdb->\
                                  p_recv_msg->hdr.app_id;
            /*Payload length is size of api header + response structure*/
            p_msg->hdr.api_length = (icf_uint16_t)api_len;
            p_msg->hdr.call_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
            /* bit_mask indicates the presence of the optional parameters */
            p_payload->bit_mask = 0;  
            
        }

        /* Get config_data */
        if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
            p_crm_pdb->p_glb_pdb,
            (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
            (icf_void_t *)&p_crm_pdb->p_config_data,
            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {


            if(ICF_SUCCESS == p_crm_pdb->result) 
            {
                p_payload->bit_mask |= ICF_API_GET_LINE_STG_RESULT_SUCCESS;
                
                /* now fill the response structure according to 
                 * paramters asked to send */
                /* ###### Proxy address ######## */
                if (ICF_NULL != (ICF_REQD_PROXY_ADDR & \
                            p_recv_msg_payload->bit_mask))   
                { 
                        
                    /** Copy the proxy address structure to the response API's payload. **/
                    if(p_crm_pdb->p_line_data[p_recv_msg_payload->line_id]->line_data_received
                                                     & ICF_CFG_LINE_DATA_PROXY_RECD)
                    {                    
                          icf_port_memcpy( 
                               (icf_void_t*)&(p_payload->proxy_address), 
                               (icf_void_t*)&(p_crm_pdb->p_line_data\
                               [p_recv_msg_payload->line_id]->proxy_addr),
                               (icf_uint32_t) \
                               ICF_PORT_SIZEOF(icf_transport_address_st));
                    }        
                    p_payload->bit_mask |= ICF_REQD_PROXY_ADDR; 

                }

                /* ##### Registrar address ###### */
                if (ICF_NULL != (ICF_REQD_REGISTRAR_ADDR & \
                            p_recv_msg_payload->bit_mask))    
                { 
                      /** Copy the proxy address structure to the response API's payload. **/
                      if(p_crm_pdb->p_line_data[p_recv_msg_payload->line_id]->line_data_received
                                      & ICF_CFG_LINE_DATA_REGISTRAR_RECD)
                      {                
                           icf_port_memcpy(  
                                    (icf_void_t*)&(p_payload->\
                                                      registrar_address), 
                                    (icf_void_t*)&(p_crm_pdb->p_line_data\
                                    [p_recv_msg_payload->line_id]->\
                                    registrar_addr),
                                    (icf_uint32_t) ICF_PORT_SIZEOF(icf_transport_address_st)
                                    );
                      }
                            p_payload->bit_mask |= ICF_REQD_REGISTRAR_ADDR;
                        
                }

                /* ###### Transport mode ###### */
                if (ICF_NULL != (ICF_REQD_PROXY_TRANSPORT_MODE & \
                            p_recv_msg_payload->bit_mask)) 
                { 
                       
                            /** Copy the proxy address structure to the response API's payload. **/
                            p_payload->proxy_transport_mode = 
                                p_crm_pdb->p_line_data\
                                [p_recv_msg_payload->line_id]->transport_mode;

                            p_payload->bit_mask |= ICF_REQD_PROXY_TRANSPORT_MODE;
                        
                }
                

                /* ######## Self Address List ######## */
                if (ICF_NULL != (ICF_REQD_SELF_ADDR_LIST    & \
                                    p_recv_msg_payload->bit_mask))
                {
                    /* ICF_MUL_PUB - call fn to get user list from user db*/
                    /** Copy the self address list structure to the response API's payload. **/
                    if(p_crm_pdb->p_line_data[p_recv_msg_payload->line_id]->line_data_received
                              & ICF_CFG_LINE_DATA_SELF_ADDR_RECD)
                    {

                         icf_port_memcpy( 
                              (icf_void_t*)&(p_payload->\
                              self_address_list), 
                              (icf_void_t*)(p_crm_pdb->p_line_data\
                              [p_recv_msg_payload->line_id]->\
                              p_user_db),
                              (icf_uint32_t) ICF_PORT_SIZEOF(icf_address_st));
                    }   
                    p_payload->bit_mask |= ICF_REQD_SELF_ADDR_LIST;
                
                }

                /* ######## Self Name ######## */
                if (ICF_NULL != (ICF_REQD_SELF_NAME & \
                                    p_recv_msg_payload->bit_mask))
                {
                    /** Copy the self name structure to the response API's payload. **/
                    icf_port_memcpy( 
                        (icf_void_t*)&(p_payload->\
                        self_name), 
                        (icf_void_t*)&(p_crm_pdb->p_line_data\
                        [p_recv_msg_payload->line_id]->\
                        user_name),
                        (icf_uint32_t) ICF_PORT_SIZEOF(icf_name_st));
                    
                    p_payload->bit_mask |= ICF_REQD_SELF_NAME;
                
                }

                /* ######## Presentation Status ######## */
                if (ICF_NULL != (ICF_REQD_PRESENTATION_STATUS & \
                                    p_recv_msg_payload->bit_mask))
                {
                    /** Copy the Presentation Status to the response API's payload. **/
                    
                     p_payload->presentation_allowed = 
                                p_crm_pdb->p_line_data\
                                [p_recv_msg_payload->line_id]->default_settings;
                    
                    p_payload->bit_mask |= ICF_REQD_PRESENTATION_STATUS;
                
                }

                /* ######## Auth Key ######## */
                if (ICF_NULL != (ICF_REQD_AUTH_KEY & \
                                    p_recv_msg_payload->bit_mask))
                {
                    /** Copy the auth key to the response API's payload. **/
                    if(p_crm_pdb->p_line_data[p_recv_msg_payload->line_id]->line_data_received
                            & ICF_CFG_LINE_DATA_AUTH_KEY_RECD)
                    {
                        icf_port_memcpy( 
                             (icf_void_t*)&(p_payload->\
                             auth_key), 
                             (icf_void_t*)&(p_crm_pdb->p_line_data\
                             [p_recv_msg_payload->line_id]->\
                             authentication_key),
                             (icf_uint32_t) ICF_PORT_SIZEOF(icf_name_st));
                    } 
                    p_payload->bit_mask |= ICF_REQD_AUTH_KEY;
                
                }

                /* ######## Auth Password ######## */
                if (ICF_NULL != (ICF_REQD_AUTH_PASSWD & \
                                    p_recv_msg_payload->bit_mask))
                {
                    /** Copy the auth passwd to the response API's payload. **/
                    if(p_crm_pdb->p_line_data[p_recv_msg_payload->line_id]->line_data_received
                           & ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)
                    {       
                        icf_port_memcpy( 
                            (icf_void_t*)&(p_payload->\
                            auth_password), 
                            (icf_void_t*)&(p_crm_pdb->p_line_data\
                            [p_recv_msg_payload->line_id]->\
                            authentication_password),
                            (icf_uint32_t) ICF_PORT_SIZEOF(icf_name_st));
                    }  
                    p_payload->bit_mask |= ICF_REQD_AUTH_PASSWD;
                
                }
               /* ######## Reg Duration ######## */
                if (ICF_NULL != (ICF_REQD_REG_MODE & \
                                    p_recv_msg_payload->bit_mask))
                {
                    /** Copy the Reg Duration to the response API's payload. **/
                    
                     p_payload->reg_mode = 
                                p_crm_pdb->p_line_data\
                                [p_recv_msg_payload->line_id]->reg_mode;
                    
                    p_payload->bit_mask |= ICF_REQD_REG_MODE;
                
                }
                /* ######## Reg Duration ######## */
                if (ICF_NULL != (ICF_REQD_REG_DURATION & \
                                    p_recv_msg_payload->bit_mask))
                {
                    /** Copy the Reg Duration to the response API's payload. **/
                    
                     p_payload->reg_duration = 
                                p_crm_pdb->p_line_data\
                                [p_recv_msg_payload->line_id]->registration_duration;
                    
                    p_payload->bit_mask |= ICF_REQD_REG_DURATION;
                
                }

                


                if (ICF_NULL != (ICF_REQD_SIP_T1_TIMER & \
                            p_recv_msg_payload->bit_mask))
                {
                    /** Copy the timer to the response API's payload. **/
                    p_payload->sip_T1_timer =\
                             p_crm_pdb->p_line_data\
                             [p_recv_msg_payload->line_id]->sip_timer_t1;

                    p_payload->bit_mask |= ICF_REQD_SIP_T1_TIMER;
                }

                /* ####### SIP B TIMER ####### */
                if (ICF_NULL != (ICF_REQD_SIP_B_TIMER & \
                            p_recv_msg_payload->bit_mask))
                {
                    /** Copy the timer to the response API's payload. **/
                    p_payload->sip_B_timer =\
                            p_crm_pdb->p_line_data\
                            [p_recv_msg_payload->line_id]->sip_timer_invite;

                    p_payload->bit_mask |= ICF_REQD_SIP_B_TIMER;
                }

                /* ####### SIP F TIMER ####### */
                if (ICF_NULL != (ICF_REQD_SIP_F_TIMER & \
                            p_recv_msg_payload->bit_mask))
                {
                    /** Copy the timer to the response API's payload. **/
                    p_payload->sip_F_timer =\
                           p_crm_pdb->p_line_data
                          [p_recv_msg_payload->line_id]->sip_timer_non_invite;

                    p_payload->bit_mask |= ICF_REQD_SIP_F_TIMER;
                }



                /* ##### Route address for Register requests ###### */
                if (ICF_NULL != (ICF_REQD_REG_ROUTE_ADDR & \
                            p_recv_msg_payload->bit_mask))    
                { 

                  /** Copy the proxy address structure to the response API's payload. **/
                  icf_port_memcpy( 
                      (icf_void_t*)&(p_payload->\
                                     reg_route_addr), 
                      (icf_void_t*)&(p_crm_pdb->p_line_data\
                                     [p_recv_msg_payload->line_id]->\
                                     reg_route_addr),
                      (icf_uint32_t) ICF_PORT_SIZEOF(icf_transport_address_st)
                      );

                  p_payload->bit_mask |= ICF_REQD_REG_ROUTE_ADDR;
				}
            } /* if(ICF_SUCCESS == p_crm_pdb->result) */
        } /* end of config_data */

        /* #######  SEND THE RESPONSE TO THE APPLICATION ####### */
                
        /* Send the response message */
        p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;
        ret_val = icf_adm_process_mesg(p_crm_pdb->p_internal_msg);
        ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_RESP_SENT)

        /* Free the memory allocated to the response message */
        if (ICF_SUCCESS == ret_val)
        {
            /* Free memory */
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
        }
        else
        {
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
            /* restore the return value */
            ret_val = ICF_FAILURE;
        }
            
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}
ICF_CONF_STATUS_API_END


/*************************************************************************
 *
 * FUNCTION:        icf_cfg_set_line_params_hdlr
 * 
 * DESCRIPTION:     This function will handle the ICF_SET_LINE_PARAM_REQ
 *                  API send from application..
 *                  
 * RETURNS:         ICF_SUCCESS:        
 *                  ICF_FAILURE: 
 *************************************************************************/
icf_return_t icf_cfg_set_line_params_hdlr 
(
	INOUT	icf_crm_pdb_st	*p_crm_pdb
)
{
	icf_return_t			ret_val			=		ICF_SUCCESS;
	icf_line_data_st		*p_line_data	=		ICF_NULL;
	icf_set_line_param_req_st	*p_recv_msg_payload =	ICF_NULL;
	icf_line_id_t			line_index	=	0;	
 
	ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
   
	p_recv_msg_payload = (icf_set_line_param_req_st *)(p_crm_pdb->p_recv_payload);

	ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
    /* SPR 15997: line_id in ICF_CONFIGURE_CFM_API */
    p_crm_pdb->line_id = p_recv_msg_payload->line_id; 
    /*----------------------------------------------------*/ 
    /* validate the data received */
	ret_val = icf_cfg_validate_line_prms(p_crm_pdb);
    
	/*ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)*/
    if(ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        p_crm_pdb->result = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;            
    }     
     /* Added null check(config_data) for klocwork warning removal */       
     if ((ICF_ALL_LINES == p_recv_msg_payload->line_id) &&
         (ICF_NULL != p_crm_pdb->p_config_data))  
     {
		for (line_index = 0; line_index < (p_crm_pdb->p_config_data->max_lines); line_index++)
		{
        	if(ICF_SUCCESS == 
				icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, line_index,
                    &p_line_data, 
 					p_crm_pdb->p_ecode))
        	{
    			if (p_recv_msg_payload->bitmask &
					ICF_SET_LINE_PRM_SUBS_HS_TIMER)
    			{
					p_line_data->subs_hs_duration = 
						p_recv_msg_payload->subscribe_head_start_timer * 1000; 
				}	
				if (p_recv_msg_payload->bitmask &
					ICF_SET_LINE_PRM_T1_TIMER)
    			{
					p_line_data->sip_timer_t1 = 
						p_recv_msg_payload->sip_timer_t1;
				}	
				if (p_recv_msg_payload->bitmask &
					ICF_SET_LINE_PRM_T2_TIMER)
    			{
					p_line_data->sip_timer_t2 = 
						p_recv_msg_payload->sip_timer_t2;
				}	
				if (p_recv_msg_payload->bitmask &
					ICF_SET_LINE_PRM_INV_TIMER)
    			{
					p_line_data->sip_timer_invite = 
					p_recv_msg_payload->sip_timer_invite; 
				}	
				if (p_recv_msg_payload->bitmask &
					ICF_SET_LINE_PRM_NON_INV_TIMER)
    			{
					p_line_data-> sip_timer_non_invite = 
						p_recv_msg_payload->sip_timer_noninvite; 
				}	
                /* Start : Addition for Per Line Feature */
                if (p_recv_msg_payload->bitmask &
                    ICF_SET_LINE_SIP_DSCP)
                {
                    p_line_data->sip_dscp = 
                    p_recv_msg_payload->sip_dscp;
                }
                if (p_recv_msg_payload->bitmask &
                    ICF_SET_LINE_RTP_DSCP)
                {
                    p_line_data->rtp_dscp = 
                    p_recv_msg_payload->rtp_dscp;
                }
#ifdef ICF_SESSION_TIMER
              if(ICF_TRUE == p_crm_pdb->p_config_data->session_timer_enable)
              {
                if (p_recv_msg_payload->bitmask &
                    ICF_SET_LINE_SESSION_TIMER)
                {
                 /* for cases where min_se is absent in api and line has some value
                 due to prev call/configuration so if validation fails set session_timer 
                 with the default */
                 if(!(p_recv_msg_payload->bitmask & ICF_SET_LINE_MIN_SE) )
                 {
                   if((p_line_data->min_se != ICF_NULL) &&
                      (p_line_data->min_se >  p_recv_msg_payload->session_timer))
                      {
                       p_line_data->session_timer = ICF_CFG_DEF_SESSION_TIMER_INTERVAL;
                      }
                   else
                     {
                      p_line_data->session_timer = p_recv_msg_payload->session_timer;
                     }
                 }
                 else
                 {
                   p_line_data->session_timer = p_recv_msg_payload->session_timer;
                 }
                }
                if (p_recv_msg_payload->bitmask &
                    ICF_SET_LINE_MIN_SE)
                {
                 /* for cases where session_timer is absent in api and line has some value
                 due to prev call/configuration so if validation fails set min_se 
                 with the default */
                  if(!(p_recv_msg_payload->bitmask &ICF_SET_LINE_SESSION_TIMER))
                   {
                     if((p_line_data->session_timer != ICF_NULL) &&
                        (p_recv_msg_payload->min_se > p_line_data->session_timer))
                        {
                         p_line_data->min_se = ICF_CFG_DEF_MIN_SESSION_TIMER_INTERVAL;
                        } 
                     else
                        {
                         p_line_data->min_se = p_recv_msg_payload->min_se;
                        } 
                   }
                  else
                   {
                     p_line_data->min_se = p_recv_msg_payload->min_se;
                   }
                }
                if (p_recv_msg_payload->bitmask &
                    ICF_SET_LINE_SESSION_TIMER_REFRESHER)
                {
                  p_line_data->session_timer_refresher = 
                  p_recv_msg_payload->session_timer_refresher;
                }
         }
#endif

            if (p_recv_msg_payload->bitmask &
                ICF_SET_LINE_AUDIO_CODEC)
            {  
                icf_cmn_clone_codec_attrib_list(p_crm_pdb->p_glb_pdb,&(p_line_data->p_codec_attributes_audio),p_recv_msg_payload->p_codec_attributes_audio);
            }                    
            if (p_recv_msg_payload->bitmask &
                ICF_SET_LINE_VIDEO_CODEC)
            {  
                icf_cmn_clone_codec_attrib_list(p_crm_pdb->p_glb_pdb,&(p_line_data->p_codec_attributes_video),p_recv_msg_payload->p_codec_attributes_video);

            }    
            if (p_recv_msg_payload->bitmask & ICF_SET_LINE_CONTACT_USER_INFO)
            {
                p_line_data->contact_user_info.str_len = 
                    p_recv_msg_payload->contact_user_info.str_len;
                icf_port_strcpy(
                    p_line_data->contact_user_info.str, 
                    p_recv_msg_payload->contact_user_info.str);
            }                
                /* End : Addition for Per Line Feature */
			/* Fix for CSR 1-8047728: SPR 20246 */
			if (p_recv_msg_payload->bitmask & ICF_SET_LINE_REMOVE_ROUTE_HDR)
            {
                p_line_data->remove_route_hdr = 
                    p_recv_msg_payload->remove_route_hdr;
            }
	

	
  			}
    	}
	}
	else	 
	{
    	if(ICF_SUCCESS == 
			icf_dbm_fetch_line_cntxt_blk(
                p_crm_pdb->p_glb_pdb, 
  				p_recv_msg_payload->line_id,
                &p_line_data, 
  				p_crm_pdb->p_ecode))
        {
    		if (p_recv_msg_payload->bitmask &
				ICF_SET_LINE_PRM_SUBS_HS_TIMER)
    		{
				p_line_data->subs_hs_duration = 
					p_recv_msg_payload->subscribe_head_start_timer * 1000; 
			}	
			if (p_recv_msg_payload->bitmask &
				ICF_SET_LINE_PRM_T1_TIMER)
    		{
				p_line_data->sip_timer_t1 = 
					p_recv_msg_payload->sip_timer_t1;
			}	
			if (p_recv_msg_payload->bitmask &
				ICF_SET_LINE_PRM_T2_TIMER)
    		{
				p_line_data->sip_timer_t2 = 
					p_recv_msg_payload->sip_timer_t2;
			}	
			if (p_recv_msg_payload->bitmask &
				ICF_SET_LINE_PRM_INV_TIMER)
    		{
				p_line_data->sip_timer_invite = 
				p_recv_msg_payload->sip_timer_invite; 
			}	
			if (p_recv_msg_payload->bitmask &
				ICF_SET_LINE_PRM_NON_INV_TIMER)
    		{
				p_line_data-> sip_timer_non_invite = 
					p_recv_msg_payload->sip_timer_noninvite; 
			}
            if (p_recv_msg_payload->bitmask &
                ICF_SET_LINE_SIP_DSCP)
            {
              p_line_data->sip_dscp = 
              p_recv_msg_payload->sip_dscp;
            }
           if (p_recv_msg_payload->bitmask & 
               ICF_SET_LINE_RTP_DSCP)
            {
              p_line_data->rtp_dscp = 
              p_recv_msg_payload->rtp_dscp;
            }
#ifdef ICF_SESSION_TIMER
          if ((ICF_NULL != p_crm_pdb->p_config_data) &&
              (ICF_TRUE == p_crm_pdb->p_config_data->session_timer_enable))
          {
            if (p_recv_msg_payload->bitmask &
               ICF_SET_LINE_SESSION_TIMER)
            {
                 if(!(p_recv_msg_payload->bitmask & ICF_SET_LINE_MIN_SE) )
                 {
                   if((p_line_data->min_se != ICF_NULL) &&
                      (p_line_data->min_se >  p_recv_msg_payload->session_timer))
                      {
                       p_line_data->session_timer = ICF_CFG_DEF_SESSION_TIMER_INTERVAL;
                      }
                   else
                     {
                      p_line_data->session_timer = p_recv_msg_payload->session_timer;
                     }
                 }
                 else
                 {
                   p_line_data->session_timer = p_recv_msg_payload->session_timer;
                 }
            }
           if (p_recv_msg_payload->bitmask &
               ICF_SET_LINE_MIN_SE)
            {
                  if(!(p_recv_msg_payload->bitmask &ICF_SET_LINE_SESSION_TIMER))
                   {
                     if((p_line_data->session_timer != ICF_NULL) &&
                        (p_recv_msg_payload->min_se > p_line_data->session_timer))
                        {
                         p_line_data->min_se = ICF_CFG_DEF_MIN_SESSION_TIMER_INTERVAL;
                        } 
                     else
                        {
                         p_line_data->min_se = p_recv_msg_payload->min_se;
                        } 
                   }
                  else
                   {
                     p_line_data->min_se = p_recv_msg_payload->min_se;
                   }  
            }
           if (p_recv_msg_payload->bitmask &
              ICF_SET_LINE_SESSION_TIMER_REFRESHER)
            {
             p_line_data->session_timer_refresher =
             p_recv_msg_payload->session_timer_refresher;
            }
        }
#endif
            if (p_recv_msg_payload->bitmask &
                ICF_SET_LINE_AUDIO_CODEC)
            {  
                icf_cmn_clone_codec_attrib_list(p_crm_pdb->p_glb_pdb,&(p_line_data->p_codec_attributes_audio),p_recv_msg_payload->p_codec_attributes_audio);
            }                    
            if (p_recv_msg_payload->bitmask &
                ICF_SET_LINE_VIDEO_CODEC)
            {  
                icf_cmn_clone_codec_attrib_list(p_crm_pdb->p_glb_pdb,&(p_line_data->p_codec_attributes_video),p_recv_msg_payload->p_codec_attributes_video);

            }   
            if (p_recv_msg_payload->bitmask & ICF_SET_LINE_CONTACT_USER_INFO)
            {
                p_line_data->contact_user_info.str_len = 
                    p_recv_msg_payload->contact_user_info.str_len;
                icf_port_strcpy(
                    p_line_data->contact_user_info.str, 
                    p_recv_msg_payload->contact_user_info.str);
            }       
			/* Fix for CSR 1-8047728: SPR 20246 */
			if (p_recv_msg_payload->bitmask & ICF_SET_LINE_REMOVE_ROUTE_HDR)
            {
                p_line_data->remove_route_hdr = 
                    p_recv_msg_payload->remove_route_hdr;
            }
  		}
	}	
	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;
}
/*************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_line_prms
 * 
 * DESCRIPTION:     This function will validate the ICF_SET_LINE_PARAM_REQ
 *                  API send from application..
 *                  
 * RETURNS:         ICF_SUCCESS:        
 *                  ICF_FAILURE: 
 *************************************************************************/
icf_return_t icf_cfg_validate_line_prms 
(
	INOUT	icf_crm_pdb_st	*p_crm_pdb
)
{

	icf_return_t ret_val = ICF_SUCCESS;
	icf_set_line_param_req_st	*p_recv_msg_payload =	ICF_NULL;

	ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
	
	p_recv_msg_payload = (icf_set_line_param_req_st *)(p_crm_pdb->p_recv_payload);
	ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

	/* Validate line id */
    if((ICF_ALL_LINES != p_recv_msg_payload->line_id) && 
            (p_recv_msg_payload->line_id >= p_crm_pdb->p_config_data->
             max_lines))
    {
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_LINE_ID)
    }
	else if ( ICF_FAILURE == icf_cfg_check_line_configured(p_crm_pdb,ICF_FALSE))
	{
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_LINE_ID)
	}
	else if ( ICF_SET_LINE_PRM_T1_TIMER & p_recv_msg_payload->bitmask &&
			  0 == p_recv_msg_payload->sip_timer_t1)
	{
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)

	} 
    else if ( ICF_SET_LINE_PRM_T2_TIMER & p_recv_msg_payload->bitmask &&
              0 == p_recv_msg_payload->sip_timer_t2)
    {
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
    
    }
    else if ( ICF_SET_LINE_PRM_INV_TIMER & p_recv_msg_payload->bitmask &&
              0 == p_recv_msg_payload->sip_timer_invite)
    {
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
    
    }
    else if ( ICF_SET_LINE_PRM_NON_INV_TIMER & p_recv_msg_payload->bitmask &&
              0 == p_recv_msg_payload->sip_timer_noninvite)
    {
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
    
    }
#ifdef ICF_SESSION_TIMER
  if ((ICF_NULL != p_crm_pdb->p_config_data) &&
      (ICF_TRUE == p_crm_pdb->p_config_data->session_timer_enable))
 {
    if (ICF_NULL != (ICF_SET_LINE_MIN_SE &
                    p_recv_msg_payload->bitmask))
    {
        if(ICF_CFG_DEF_MIN_SESSION_TIMER_INTERVAL >
                p_recv_msg_payload->min_se)
        {
            p_crm_pdb->error_cause = ICF_CAUSE_INVALID_TIMER_VALUE;
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val; /* returns success always */
        }
        if (ICF_NULL != (ICF_SET_LINE_SESSION_TIMER &
                    p_recv_msg_payload->bitmask))
        {
            if(p_recv_msg_payload->min_se >
                    p_recv_msg_payload->session_timer)
            {
                p_crm_pdb->error_cause = ICF_CAUSE_INVALID_TIMER_VALUE;
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val; /* returns success always */
            }
        }
    }
    else if(ICF_NULL != (ICF_SET_LINE_SESSION_TIMER &
                    p_recv_msg_payload->bitmask))
    {
        if( ICF_CFG_DEF_MIN_SESSION_TIMER_INTERVAL >
                p_recv_msg_payload->session_timer)
        {
            p_crm_pdb->error_cause = ICF_CAUSE_INVALID_TIMER_VALUE;
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val; /* returns success always */
        }
    }
 }
#endif


	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;
}
/*************************************************************************
 *
 * FUNCTION:        icf_cfg_trim_string
 * 
 * DESCRIPTION:     This function will remove the single and double quotes
 *                  from input string.
 *                  
 * RETURNS:         length of modified string        
 *************************************************************************/
icf_uint16_t icf_cfg_trim_string(icf_uint8_t   *p_str)
{
 icf_string_st        temp_str;
 icf_uint8_t          temp_index = 0;
 icf_uint8_t          index = 0;
 icf_port_memset(&temp_str.str,ICF_NULL,ICF_MAX_STR_LEN);
 temp_str.str_len = 0;
 while(*(p_str + index))
 {
   if((*(p_str + index) == '"' ) || (*(p_str + index) == 96))
   {
    index++;
   }
   else
   {
     temp_str.str[temp_index] = *(p_str + index);
     temp_index++;
     index++;
   }
  }
  icf_port_strcpy(p_str, temp_str.str);
  temp_str.str_len = (icf_uint16_t) icf_port_strlen(p_str);
  return(temp_str.str_len);
}  



/*************************************************************************
 *
 * FUNCTION:        icf_cfg_get_register_status_req_hdlr
 * 
 * DESCRIPTION:     This function is called when the Application sends
 *                  ICF_GET_REGISTER_STATUS_REQ to IPTK.
 *                  
 * RETURNS:         length of modified string        
 *************************************************************************/


icf_return_t icf_cfg_get_register_status_req_hdlr(INOUT  icf_crm_pdb_st  *p_crm_pdb)
{
    icf_return_t                           ret_val = ICF_SUCCESS;
    icf_line_data_st                       *p_line_data = ICF_NULL;
    icf_line_id_t                          line_id=0;
    icf_uint8_t                            registration_state = ICF_RGM_00_IDLE;
    icf_user_db_node_st                    *p_user_db = ICF_NULL;
    icf_msg_st                             *p_msg = ICF_NULL;
    icf_uint16_t                           api_len = 0;
    icf_get_register_status_resp_st        *p_payload = ICF_NULL;
    icf_get_register_status_req_st         *p_recv_msg_payload =  ICF_NULL;
    icf_user_reg_status_st                 *temp = ICF_NULL;


        ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

        p_recv_msg_payload = (icf_get_register_status_req_st *)
                                    (p_crm_pdb->p_recv_payload);

        line_id = p_recv_msg_payload->line_id;

        api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
                    ICF_PORT_SIZEOF(icf_get_register_status_resp_st);


          /* Allocate memory to the response message strucure */

        ICF_MEMGET(
                   p_crm_pdb->p_glb_pdb,
                   api_len,
                   ICF_MEM_COMMON,
                   p_msg,
                   ICF_RET_ON_FAILURE,
                   p_crm_pdb->p_ecode,
                   ret_val)


       if(ICF_NULL != ret_val)
       {

        p_payload = (icf_get_register_status_resp_st *) p_msg->payload;


        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                                      ICF_GET_REGISTER_STATUS_RESP;

        /* API-ID to send */
        p_msg->hdr.api_id = ICF_GET_REGISTER_STATUS_RESP;
        p_msg->hdr.source_id = ICF_MODULE_ICF;
        p_msg->hdr.destination_id = ICF_MODULE_PA;

        /*Payload length is size of api header + response structure*/
        p_msg->hdr.api_length = api_len;

        p_msg->hdr.app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;
        p_msg->hdr.call_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
            

        /* Filling the payload */

        if(ICF_ALL_LINES == p_recv_msg_payload->line_id)
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

            p_payload->result = ICF_FAILURE;

            p_payload->error_cause = ICF_CAUSE_INVALID_ACTION;
        }


       else if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk (p_crm_pdb->p_glb_pdb,
              line_id,&p_line_data,p_crm_pdb->p_ecode))   /*no get_line needed */
        {

                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

                p_payload->result = ICF_FAILURE;

                p_payload->error_cause = ICF_CAUSE_LINE_NOT_CONFIGURED;

         }/*If the line is not configured*/

        /* Success by this function means that the line exists and 
         * pointer to the line is returned further check if the registar is not
         * present.
         */

        else if ((ICF_NULL == (ICF_CFG_LINE_DATA_REGISTRAR_RECD
                              & p_line_data->line_data_received)))
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

            p_payload->result = ICF_FAILURE;

            p_payload->error_cause = ICF_CAUSE_REGISTRAR_NOT_CONFIGURED;
        }
      
        else
        {

        p_user_db = p_line_data->p_user_db;
        
        if(p_user_db == ICF_NULL)
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

            p_payload->result = ICF_FAILURE;

            p_payload->error_cause = ICF_CAUSE_NO_USER_EXIST_ON_LINE;
         }
        else
        {

          p_payload->result = ICF_SUCCESS;
          p_payload->registrar_address = p_line_data->registrar_addr;
          p_payload->expires_duration = p_line_data->registration_duration;

           while(ICF_NULL != p_user_db)
            {
                /* The response list should include all the users configured 
                 * on a line.In case a user is configured on a line but there
                 * is no reg-context,send status as IDLE.
            	 */
                if (( ICF_FAILURE !=
                     icf_rgm_get_uri_status(p_crm_pdb->p_glb_pdb,
                     &(p_user_db->public_user),
                     p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id,
                     &registration_state,
                     &(p_crm_pdb->error_cause) ) ) ||
                     (ICF_RGM_00_IDLE == registration_state))
                 {

                   /*Fix done for SPR-18722 : CSR-1-6068965*/
                   /* This variable is used to keep the value of user and
                       reg_status and it is assigned to list in payload*/
                   temp = ICF_NULL;

                   ICF_MEMGET(
                   p_crm_pdb->p_glb_pdb,
                   ICF_PORT_SIZEOF(icf_user_reg_status_st),
                   ICF_MEM_COMMON,
                   temp,
                   ICF_RET_ON_FAILURE,
                   p_crm_pdb->p_ecode,
                   ret_val)

                   temp->user =  p_user_db->public_user;
  
                   temp->reg_status = registration_state ;

                   icf_cmn_add_node_to_list(
                    p_crm_pdb->p_glb_pdb, &(p_payload->p_user_reg_status),temp);

                 }
                 p_user_db = p_user_db->p_next;
              }
          }
       }
    }
  
  
     p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;
     ret_val = icf_adm_process_mesg(p_crm_pdb->p_internal_msg);

     /* SPR 17636 : CSR 1-5290722: Free the user reg status list
      */
     icf_cmn_delete_list(p_crm_pdb->p_glb_pdb, &(p_payload->p_user_reg_status),
             ICF_MEM_COMMON);

        /* Free the memory allocated to the response message */
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_msg,
                    ICF_MEM_COMMON,
                    p_crm_pdb->p_ecode,
                    ret_val)

   ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
   return ret_val;

}

/*************************************************************************
 *
 * FUNCTION:        icf_cfg_trim_string_for_filter_list
 * 
 * DESCRIPTION:     This function will remove the single qoutes
 *                  from input string.
 *                  
 * RETURNS:         length of modified string        
 *************************************************************************/
icf_uint16_t icf_cfg_trim_string_for_filter_list(icf_uint8_t   *p_str)
{
 icf_string_st        temp_str;
 icf_uint8_t          temp_index = 0;
 icf_uint8_t          index = 0;
 icf_port_memset(&temp_str.str,ICF_NULL,ICF_MAX_STR_LEN);
 temp_str.str_len = 0;
 while(*(p_str + index))
 {
   if((*(p_str + index) == 96))
   {
    index++;
   }
   else
   {
     temp_str.str[temp_index] = *(p_str + index);
     temp_index++;
     index++;
   }
  }
  icf_port_strcpy(p_str, temp_str.str);
  temp_str.str_len = (icf_uint16_t) icf_port_strlen(p_str);
  return(temp_str.str_len);
}

