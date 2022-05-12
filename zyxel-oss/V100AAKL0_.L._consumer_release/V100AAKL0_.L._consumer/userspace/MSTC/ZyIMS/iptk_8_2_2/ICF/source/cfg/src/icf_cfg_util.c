/*******************************************************************************
 *
 * FILE NAME :      icf_cfg_util.c
 *
 * DESCRIPTION:     This file contains the code of the utility functions of 
 *                  the configuration handler(cfg) of the Packet Phone.
 *                                    
 * REVISION HISTORY:   
 *
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 21-Jan-2005  Shefali Aggarwal    ICF LLD  Initial
 * 08-Apr-2005  Shradha Budhiraja   SPR 7363    Set trans param api change
 * 27-apr-2005  Shradha Budhiraja               Sending forceul call clear to
 *                                              MMI and SSA
 * 28-Apr-2005  Shradha Budhiraja   SPR 7464    Change in validate self id. 
 *                                              Wrong index was used.
 * 09-May-2005  Shradha Budhiraja   SPR 7520    Change in set self id
 * 
 * 11-May-2005  Shradha Budhiraja   SPR 7546    Change in delete user
 * 17-May-2005  Shradha Budhiraja   SPR 7564    Change in deregister function
 * 4-Jul-2005   Shradha Budhiraja   TCP Update  Changed triggers to SSA.
 * 12-Jul-2005  Shradha Budhiraja   ICF LLD  Release 2.0
 * 1-Aug-2005   Shradha Budhiraja               Code Review Comments
 * 27-Sep-2005  Aman Aggarwal                   Change in set_vms_params
 * 13-Oct-2005  Aman Aggarwal       SPR 8087    vms subscription status set in
 *                                              set vms params in db.            
 * 11-Nov--2005 Shefali Aggarwal    SPR 8188    Forceful need not be send
 *                                              to SSA by CFG
 * 11-Nov-2005  Shefali Aggarwal    Enh for mode Change in validate function                
 * 12-Nov-2005  Shefali Aggarwal    SPR 8158    Set the call ctx to null before
 *                                              calling SSA to clear.
 * 17-Nov-2005  Aman Aggarwal                   Changes regarding the initializ
 *                                              tion of line_data parameters
 * 06-Jan-2006  Jyoti Narula        SPR 9124    Defining icf_cfg_send_reg()
 *                                              app indicated nw_act 
 *                                              for AUTO_REGISTER on,
 *                                              REGISTER is triggered.
 *
 * 01-Feb-2006   Jyoti Narula       SPR 9079    Changes in icf_cfg_cm_call_
 *                                              clear_ind() to clear all
 *                                              SIC contexts as well
 * 02-Feb-2006  Amit Sharma                     CFG merged with ICF 2.0
 * 28-Feb-2006	Aman Aggarwal					Services merged with IPPPSTK 2.0
 * 02-Apr-2006	Puneet Sharma	ICF5.0 LLD		changes done in the comments 
 *												Multiple Public Users ICF5.0
 * 30-Mar-2006  Anuradha Gupta		ICF 5.0		Call Modify Feature changes
 * 1-Sep-2006   Umang Singh		SPR 12731
 * 7-Sep-2006   Amit Sharma		SPR 12819
 * 17-Oct-2006  Aman Arora      ICF rel 6.1.1   Licensing support for user name validation
 * 13-Jan-2006  Abhishek Dhammawat ICF Rel 6.3 Per line feature - Added for obsolete
 *                                              hold parameter.
 * 08-JUN-2007  Alok Tiwari         Rel early 7.0   Code added for stat collection
 * 13-Jul-2007  Abhishek Dhammawat Rel7.0       Addition for XCONF ftr.
 * 27-Aug-2007  Deepti Goyal    SPR 15997       Line id in ICF_CONFIGURE_CFM API.
 * 7-Sep-2007   Deepti Goyal    SPR 16019       Changes in configuration API 
 *                                              handling.
 * 27-Nov-2007  Abhishek Dhammawat SPR 17553    Modified icf_cfg_cm_reinit_ind
 * 27-Nov-2007  Abhishek Dhammawat SPR 17554    Modified
 * 21-Dec-2007  Amit Sharma        SPR 17433    Server closed when reinit received
 * 22-Dec-2007  Jagmohan Chauhan   SPR 17565    Trace printed for self_transport address 
 * 15-Jan-2008	Shruti Thakral                  Changes for secondary server
 *                                              support in Rel 8.0 
 * 16-JAN-2008  Anurag Khare       ICF Rel 8.0  Changes for DHCP Server
 *  
 * 16-Jan-2008  Kamini Gangwani    Rel 8.0      Modified function icf_cfg_set_call_prms_in_db to 
 *                                              add the data received for access n/w info in 
 *                                              global database.  
 * 17-Jan-2008  Abhishek Dhammawat Rel 8.0      Modified function 
 *                                              icf_cfg_set_call_prms_in_db
 *                                              for precondition feature.
 * 18-Jan-2008  Kamini Gangwani    Rel 8.0      Modified function icf_cfg_set_call_prms_in_db to 
 *                                              add media transport type in global database for 
 *                                              SRTP Support.
 * 24-Jan-2008  Kamini Gangwani    Rel 8.0      Modified function icf_cfg_set_call_prms_in_db
 *                                              for IMS config Support                                              
 * 27-Jan-2008  Rajiv Kumar        ICF 8.0      Registration Procedure
 *                                              Enhancements 
 * 24-Mar-2008  Tarun Gupta     IPTK Rel 8.0    Merged User Param and
 *                                              Dynamic Payload Enhancements
 * 3-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 10-Apr-2008  Alok Tiwari     Spr-18364       Klocwork warning removal
 * 17-Apr-2008  Abhishek Dhammawat SPR18458     Modified
 *                                              icf_cfg_parse_filter_list
 * 28-May-2008  Tarun Gupta     SPR 18585       Modified icf_cfg_cm_call_clear_ind                                               
 * 05-Jun-2008  Tarun Gupta     SPR 18585       Modified icf_cfg_validate_act_flag
 * 07-Jun-2008  Amit Sharma       IPTK 8.0 SRTP LLD      SRTP Enhancements
 * 13-Jun-2008  Tarun Gupta     SPR 18585       Modified icf_cfg_init_line_blk and
 *                                              icf_cfg_set_call_prms_in_db
 * 17-Jun-2008  Tarun Gupta     SPR 18613       Modified icf_cfg_validate_trnspt_prms
 * 18-Jun-2008  Tarun Gupta     SPR 18624       Modified icf_cfg_set_call_prms_in_db
 * 25-Jun-2008 Abhishek Dhammawat IPTK Rel8.0 Modified icf_cfg_init_line_blk
 * 26-Jun-2008  Tarun Gupta     SPR 18616       Added handling for action 'Modify'
 *                                              for registrar address
 * 29-Jun-2008  Amit Sharma     SPR 17030       Enhancement for self_addr cfg
 *                                              with params in user part
 * 07-July-2008 Anurag Khare    SPR 18684       CSR 1-6073561 Merge 
 * 05-Jul-2008  Anuradha Gupta  SPR 18759       Modifed function for initialization
 *                                              of rport config data
 * 08-Jul-2008  Anuradha Gupta  SPR 18783       Modified function icf_cfg_validate_trnspt_prms
 * 09-July-2008 Anurag Khare    SPR 18784       Modified function icf_cfg_validate_act_flag
 * 24-Jul-2008  Tarun Gupta     SPR 18833       Modified icf_cfg_validate_sec_proxy_addr
 * 04-Nov-2008  Tarun Gupta     SPR 19189   Merged Rel 7.1.2 SPR 18880
 * 12-Nov-2008 Abhishek Dhammawat SPR 19195     Modified
 *                                              icf_cfg_set_call_prms_in_db
 * 14-Nov-2008  Rajiv Kumar     SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                              19120)
 * 17-Nov-2008  Rajiv Kumar     SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                              19134)
 * 04-Dec-2008  Tarun Gupta     SPR 19218       Merged CSR 1-6178201
 * 08-Dec-2008  Kamal Ashraf    SPR 19218       Merged CSR 1-5826493 
 * 08-Dec-2008  Rajiv Kumar     SPR 19218       Merged CSR 1-6431768 
 * 11-Dec-2008  Anurag Khare    SPR 19218       CSR 1-6456839 Merged
 * 12-Dec-2008  Kamal Ashraf    SPR 19218       Merged CSR 1-6390927
 * 12-Feb-2009  Tarun Gupta     Rel 8.1 LLD     SDP Tunnelling Enhancements
 * 04-Mar-2009 Saurabh Gupta	SPR 19349       Klocworks fix
 * 27-Mar-2009  Anuradha Gupta  Rel 8.2         Changes for Call Clear Trigger
 * 01-Apr-2009  Kamal Ashraf     Rel 8.2         Klocworks fix
 * 07-Apr-2008  Rajiv Kumar     SPR 19428       Merged CSR 1-6801610 
 * 07-Apr-2008  Anuradha Gupta  SPR 19489       Modified fn icf_cfg_set_reg_prms_in_db 
 * 30-Apr-2009  Tarun Gupta     SPR 19532       Modified icf_cfg_cm_call_clear_ind_wrt_user
 * 2-May-2009 Abhishek Dhammawat SPR 19571      Modified
 *                                              icf_cfg_app_already_present
 * 18-May-2009  Anuradha Gupta   SPR 19672      Modified fn icf_cfg_set_call_prms_in_db
 * 21-May-2009  Anuradha Gupta   SPR 19672      Modified fn icf_cfg_set_call_prms_in_db
 * 25-May-2009  Kamal Ashraf     SPR 19672      SPR 19405 Merged
 * 26-May-2009  Anuradha Gupta   SPR 19672      CSR 1-6667668
 * 26-May-2009  Kamal Ashraf     SPR 19672      CSR_1_7061845 Merged
 * 26-May-2009  Anuradha Gupta   SPR 19672      CSR 1-6727350 Merge 
 * 28-May-2009  Anuradha Gupta   SPR 19672      CSR 1-7047249 Merge 
 * 28-May-2009  Tarun Gupta      Rel 8.2        Async NOTIFY Support
 * 28-May-2009  Ashutosh Mohan   SPR 19672      CSR-1-6976670 Merged
 * 29-May-2009  Kamal Ashraf     SPR 19672      CSR_1_7113139 Merged
 * 01-Jun-2009  Preksha         Rel 8.2         Asynchronus Message enhancements
 * 11-Jun-2009 Ashutosh Mohan   SPR 19737     Changes done for GCC 4.1 warnings removal
 * 22-Jun-2009 Anuradha Gupta    SPR 19752     Modified icf_cfg_set_call_prms_in_db
 * 25-Jul-2009 Abhishek Dhammawat SPR 19793     Modified icf_cfg_set_self_id_prms_in_db
 * 27-Jul-2009  Tarun Gupta      SPR 19886      Merged CSR 1-7009954
 * 02-Aug-2009  Rajiv Kumar      SPR 19886      Merged CSR-1-7423906  
 * 03-Aug-2009  Tarun Gupta      SPR 19886      Merged CSR 1-7334184
 * 06-Aug-2009  Tarun Gupta      SPR 19886      Merged CSR 1-7402396
 * 11-Sep-2009  Rajiv Kumar      SPR 19999      Merged CSR 1-7547597
 * 05-Oct-2009  Tarun Gupta      SPR 20092      Modified icf_cfg_error_cause_to_str
 * 14-Oct-2009 Abhishek Dhammawat SPR 20047     Modified icf_cfg_cm_call_clear_ind_wrt_app_user
 * 14-Jun-2010	Aman Aggarwal	 SPR 20453      Fix for CSR 1-8597937
 * 11-Oct-2010  Sachin Sharma    SPR 20697      Fix for CSR 1-9012191
 * Copyright (c) 2010, Aricent.
 * 
 ******************************************************************************/

#include "icf_cfg_prototypes.h"
#include "icf_common_interface_types.h"
#include "icf_cfg_macro.h"
#include "icf_cfg_defs.h"
#include "icf_dbm_prototypes.h"
#include "icf_regm_prototypes.h"
#include "icf_nm_prototypes.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_regm_prototypes.h"
#include "icf_mmi_types.h"
#include "icf_dbm_defs.h"
#include "icf_port_prototypes.h"

#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif

#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_error_cause_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to an 
 *                  error cause and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_cfg_error_cause_to_str(
        IN icf_error_t error,
        INOUT icf_uint8_t *p_error_str)
{

    switch(error)
    {
#ifndef ICF_FEATURE_ABANDON
        case ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED");
            break;
#endif

        case ICF_CAUSE_MAX_LINES_OUT_OF_RANGE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)"ICF_CAUSE_MAX_LINES_OUT_OF_RANGE");
            break;

        case ICF_CAUSE_INVALID_LINE_ID:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)"ICF_CAUSE_INVALID_LINE_ID");
            break;

        case ICF_CAUSE_MAX_AUDIO_CALLS_OUT_OF_RANGE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_MAX_AUDIO_CALLS_OUT_OF_RANGE");
            break;
        
        case ICF_CAUSE_MAX_REGISTRATION_CALLS_OUT_OF_RANGE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_MAX_REGISTRATION_CALLS_OUT_OF_RANGE");
            break;
        
        case ICF_CAUSE_MAX_USERS_PER_LINE_OUT_OF_RANGE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_MAX_USERS_PER_LINE_OUT_OF_RANGE");
            break;
        
        case ICF_CAUSE_SCALE_PARAMS_ALREADY_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SCALE_PARAMS_ALREADY_CONFIGURED");
            break;
        
        case ICF_CAUSE_INVALID_ACTION:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)"ICF_CAUSE_INVALID_ACTION");
            break;
        
        case ICF_CAUSE_INVALID_ADDRESS_RECEIVED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)"ICF_CAUSE_INVALID_ADDRESS_RECEIVED");
            break;
        
        /*case ICF_CAUSE_MAX_SERVICE_CALLS_OUT_OF_RANGE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_MAX_SERVICE_CALLS_OUT_OF_RANGE");
            break;*/
        
        case ICF_CAUSE_MAX_CALLS_TO_LOG_OUT_OF_RANGE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_MAX_CALLS_TO_LOG_OUT_OF_RANGE");
            break;
        
        /*case ICF_CAUSE_MAX_CALL_FWD_HOPS_OUT_OF_RANGE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_MAX_CALL_FWD_HOPS_OUT_OF_RANGE");
            break;*/
        
        /*case ICF_CAUSE_MAX_CALLS_IN_WAITING_OUT_OF_RANGE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_MAX_CALLS_IN_WAITING_OUT_OF_RANGE");
            break;*/
        
        case ICF_CAUSE_LINE_NOT_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)"ICF_CAUSE_LINE_NOT_CONFIGURED");
            break;

        case ICF_CAUSE_IP_PORT_OUT_OF_RANGE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)"ICF_CAUSE_IP_PORT_OUT_OF_RANGE");
            break;
        
        case ICF_CAUSE_MAX_REGISTRATION_CALLS_EXCEEDED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_MAX_REGISTRATION_CALLS_EXCEEDED");
            break;
        
        case ICF_CAUSE_MAX_USERS_PER_LINE_EXCEEDED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_MAX_USERS_PER_LINE_EXCEEDED");
            break;
        
        case ICF_CAUSE_SELF_ADDR_DOMAIN_NAME_NOT_HANDLED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SELF_ADDR_DOMAIN_NAME_NOT_HANDLED");
            break;
        
        case ICF_CAUSE_INVALID_TIMER_VALUE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_INVALID_TIMER_VALUE");
            break;
        
        case ICF_CAUSE_INSUFFICIENT_RESOURCES:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_INSUFFICIENT_RESOURCES");
            break;
        
        case ICF_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG:            
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG");
            break;

		case ICF_ERROR_SOCKET_BIND:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_ERROR_SOCKET_BIND");
            break;

		case ICF_ERROR_CAUSE_URI_NOT_DEREGISTERED :
			icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_ERROR_CAUSE_URI_NOT_DEREGISTERED");
            break;
				
#ifndef ICF_FEATURE_ABANDON
        case ICF_CAUSE_GROUP_SUBSCRIPTION_NOT_ALLOWED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_GROUP_SUBSCRIPTION_NOT_ALLOWED");
            break;

        case ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED");
            break;
        
        case ICF_CAUSE_SERVICE_NOT_SUBSCRIBED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SERVICE_NOT_SUBSCRIBED");
            break;

        case ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE");
			break;

#endif  
        case ICF_ERROR_DIRECT_ROUTED_CALL_NOT_ALWD:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_ERROR_DIRECT_ROUTED_CALL_NOT_ALWD");
            break;
       
        case ICF_CAUSE_PROXY_ADDR_ALREADY_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_PROXY_ADDR_ALREADY_CONFIGURED");
            break; 

        case ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR");
            break;  
        
        case ICF_CAUSE_DNS_SERVER_ADDRESS_MODIFICATION_NOT_ALLOWED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_DNS_SERVER_ADDRESS_MODIFICATION_NOT_ALLOWED");
            break; 
        
        case ICF_CAUSE_PRIMARY_PROXY_NOT_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_PRIMARY_PROXY_NOT_CONFIGURED");
            break;
        
        case ICF_CAUSE_INCONSISTENT_ACTIONS_FOR_PROXY_CONFIGURATION:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_INCONSISTENT_ACTIONS_FOR_PROXY_CONFIGURATION");
            break;
                   
        case ICF_CAUSE_DHCP_FAILURE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_DHCP_FAILURE");
            break;

        case ICF_CAUSE_SECONDARY_PROXY_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SECONDARY_PROXY_CONFIGURED");
            break;
                    
        case ICF_CAUSE_SECONDARY_PROXY_NOT_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SECONDARY_PROXY_NOT_CONFIGURED");
            break;
                    
        case ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_DNS_SERVER_ADDR:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_DNS_SERVER_ADDR");
            break;
                    
        case ICF_CAUSE_PRIMARY_DNS_SERVER_NOT_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_PRIMARY_DNS_SERVER_NOT_CONFIGURED");
            break;
                    
        case ICF_CAUSE_SECONDARY_PROXY_ADDR_ALREADY_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SECONDARY_PROXY_ADDR_ALREADY_CONFIGURED");
            break;
                    
        case ICF_CAUSE_INVALID_MAX_ACTIVE_CALLS_PER_LINE:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_INVALID_MAX_ACTIVE_CALLS_PER_LINE");
            break;
                    
        case ICF_CAUSE_REG_EVENT_SUBS_ALREADY_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_REG_EVENT_SUBS_ALREADY_CONFIGURED");
            break;

        case ICF_USER_NOT_ADDED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_USER_NOT_ADDED");
            break;
        
        case ICF_CAUSE_SELF_ADDRESS_NOT_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SELF_ADDRESS_NOT_CONFIGURED");
            break;
            
        case ICF_CAUSE_REG_ROUTE_NOT_REQD:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_REG_ROUTE_NOT_REQD");
            break;

        case ICF_CAUSE_REGISTRAR_ALREADY_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_REGISTRAR_ALREADY_CONFIGURED");
            break;

        case ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_REGISTRAR_ADDR:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_REGISTRAR_ADDR");
            break;

        case ICF_CAUSE_SEC_REGISTRAR_ALREADY_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SEC_REGISTRAR_ALREADY_CONFIGURED");
            break;
    
        case ICF_CAUSE_PRIMARY_REGISTRAR_NOT_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_PRIMARY_REGISTRAR_NOT_CONFIGURED");
            break;

        case ICF_CAUSE_INCONSISTENT_ACTIONS_FOR_REGISTRAR_CONFIGURATION:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_INCONSISTENT_ACTIONS_FOR_REGISTRAR_CONFIGURATION");
            break;

        case ICF_CAUSE_SECONDARY_REGISTRAR_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SECONDARY_REGISTRAR_CONFIGURED");
            break;
                    
        case ICF_CAUSE_SECONDARY_REGISTRAR_NOT_CONFIGURED:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SECONDARY_REGISTRAR_NOT_CONFIGURED");
            break;
        
        case ICF_CAUSE_SELF_ADDR_MODIFY_NOT_SUPP:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)
                    "ICF_CAUSE_SELF_ADDR_MODIFY_NOT_SUPP");
            break;

        default:
            icf_port_strcpy(p_error_str,
                    (icf_uint8_t *)"No Error");
            break;

    } /* end of switch-case for error */
} /* end of icf_cfg_error_cause_to_str() */
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_trace
 *
 * DESCRIPTION:     This function checks the trace id and calls the macro for 
 *                  trace.
 *                     
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_void_t icf_cfg_trace(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_trace_id_t           trace_id)
{
    icf_trace_data_st   trace_data=      {{0,0},0,{0},0,{{0,"\0"}},{0,"\0"},0}; 
    icf_return_t        ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    switch(trace_id)
    {
        case ICF_TRACE_SEND_RESP_FAILURE:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
            
        case ICF_TRACE_INVALID_MSG_ID_RECD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = 
                (icf_int32_t)(p_crm_pdb->p_internal_msg->msg_hdr.msg_id);
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_TRACE_INVALID_API_ID_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = 
                (icf_int32_t)(p_crm_pdb->p_internal_msg->msg_hdr.msg_id);
            trace_data.int_val[1] = 
                (icf_int32_t)(p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id);
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_MSG_PROC_RECV_INT_MSG:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = 
                (icf_int32_t)(p_crm_pdb->p_internal_msg->msg_hdr.msg_id);
            if (ICF_NULL != p_crm_pdb->p_glb_pdb->p_recv_msg)
            {
                trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
                trace_data.int_val[1] = 
                    (icf_int32_t)(p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.
                                     api_id);
            }
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_TRACE_INIT_COMPLETE_STATUS:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = 
                p_crm_pdb->p_config_data->scale_param_recd;
            trace_data.int_val[1] = 
                p_crm_pdb->global_data_init_complete;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
        
        case ICF_TRACE_INIT_COMPLETE_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = 
                p_crm_pdb->p_config_data->scale_param_recd;
            trace_data.int_val[1] = 
                p_crm_pdb->global_data_init_complete;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_TRACE_PARAM_VALIDITY:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = p_crm_pdb->is_valid_param;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
                 
            /* Recieved Scalable parameters:max_lines and max_users_per_line*/
/*#ifndef ICF_FEATURE_ABANDON */
        case ICF_CFG_TRACE_SCALABLE_LINE_PRMS_REQ_RECD:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_scale_params_req_st *)p_crm_pdb->
                 p_recv_payload)->max_lines;
            trace_data.int_val[1] = (icf_int32_t)
                 ((icf_cfg_cc_set_scale_params_req_st *)p_crm_pdb->
                  p_recv_payload)->max_users_per_line;
            trace_data.int_val[2] = (icf_int32_t)
                 ((icf_cfg_cc_set_scale_params_req_st *)p_crm_pdb->
                  p_recv_payload)->num_of_calls_to_log;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* Recieved Scalable parameters:max audio, video, 
             * registration & Service calls*/

        case ICF_CFG_TRACE_SCALABLE_CALL_PRMS_REQ_RECD:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] =  ((icf_cfg_cc_set_scale_params_req_st *)
                    p_crm_pdb->p_recv_payload)->max_audio_calls;
            trace_data.int_val[1] =  ((icf_cfg_cc_set_scale_params_req_st *)
                    p_crm_pdb->p_recv_payload)->max_video_calls;
            trace_data.int_val[2] =  ((icf_cfg_cc_set_scale_params_req_st *)
                    p_crm_pdb->p_recv_payload)->max_service_calls;
            trace_data.int_val[3] =  ((icf_cfg_cc_set_scale_params_req_st *)
                    p_crm_pdb->p_recv_payload)->max_registration_calls;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
/*#endif*/
            /* Recieved Transport parameters:presence_flag, Self_transp_mode
             * and Self Transport address' port number */

        case ICF_CFG_TRACE_TRNSPT_PRMS_REQ_RECD:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                 p_recv_payload)->bit_mask;
            if(ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_SLF_PORT & 
                        trace_data.int_val[0]))
            {
                trace_data.int_val[1] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_mode;
            }
            else
            {
                trace_data.int_val[1] = ICF_CFG_INVALID_VALUE;
            }
                
            if(ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_SLF_PORT & 
                        trace_data.int_val[0]))
            {
                trace_data.int_val[2] = (icf_int32_t)
                 ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                  p_recv_payload)->self_port;
            }
            else
            {
                trace_data.int_val[2] = ICF_CFG_INVALID_VALUE;
            }
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* Recieved proxy address parameters:
             * all_lines, line id, action */

        case ICF_CFG_TRACE_PROXY_ADDR_REQ_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                 p_recv_payload)->proxy_addr.line_id;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                 p_recv_payload)->proxy_addr.action;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        /* Recieved secondary proxy address parameters:
             * all_lines, line id, action */

        case ICF_CFG_TRACE_SEC_PROXY_ADDR_REQ_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                 p_recv_payload)->sec_proxy_addr.line_id;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                 p_recv_payload)->sec_proxy_addr.action;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* Recieved Proxy address: Address type, mode and port */
            
        case ICF_CFG_TRACE_PROXY_ADDR_LINE_RECD:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                 p_recv_payload)->proxy_addr.transport_addr.addr.addr_type;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                    p_recv_payload)->proxy_addr.transport_mode;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                    p_recv_payload)->proxy_addr.transport_addr.port_num;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
            
            /* Received Proxy address: IPV4 address */
            
        case ICF_CFG_TRACE_PROXY_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->proxy_addr.transport_addr.addr.
                    addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->proxy_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_1;
                trace_data.int_val[1] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->proxy_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_2;
                trace_data.int_val[2] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->proxy_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_3;
                trace_data.int_val[3] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->proxy_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->proxy_addr.
                    transport_addr[p_crm_pdb->line_id].transport_addr.addr.
                    addr_type)
			{
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        ((icf_cfg_cc_set_transport_params_req_st *)
                         p_crm_pdb->p_recv_payload)->proxy_addr.
                        transport_addr[p_crm_pdb->line_id].transport_addr.
                        addr.addr.ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
			}
#endif


            else if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->proxy_addr.transport_addr.addr.
                    addr_type)
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        ((icf_cfg_cc_set_transport_params_req_st *)
                         p_crm_pdb->p_recv_payload)->proxy_addr.
                        transport_addr.addr.addr.domain.str);
                trace_data.string_val[0].str_len =
                    (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
            }
            break;
            
            /* Received Secondary Proxy address: IPV4 address */
            
        case ICF_CFG_TRACE_SEC_PROXY_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->sec_proxy_addr.transport_addr.addr.
                    addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->sec_proxy_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_1;
                trace_data.int_val[1] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->sec_proxy_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_2;
                trace_data.int_val[2] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->sec_proxy_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_3;
                trace_data.int_val[3] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->sec_proxy_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->sec_proxy_addr.
                    transport_addr[p_crm_pdb->line_id].transport_addr.addr.
                    addr_type)
			{
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        ((icf_cfg_cc_set_transport_params_req_st *)
                         p_crm_pdb->p_recv_payload)->sec_proxy_addr.
                        transport_addr[p_crm_pdb->line_id].transport_addr.
                        addr.addr.ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
			}
#endif


            else if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->sec_proxy_addr.transport_addr.addr.
                    addr_type)
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        ((icf_cfg_cc_set_transport_params_req_st *)
                         p_crm_pdb->p_recv_payload)->sec_proxy_addr.
                        transport_addr.addr.addr.domain.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
            }
            break;
 
            /* Received DNS Server address: IPV4 address */
            
        case ICF_CFG_TRACE_DNS_SERVER_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->dns_server_addr.addr.addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                     dns_server_addr.addr.addr.ipv4_addr.octet_1;
                trace_data.int_val[1] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                     dns_server_addr.addr.addr.ipv4_addr.octet_2;
                trace_data.int_val[2] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                     dns_server_addr.addr.addr.ipv4_addr.octet_3;
                trace_data.int_val[3] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                     dns_server_addr.addr.addr.ipv4_addr.octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->dns_server_addr.addr.addr_type)
			{
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        ((icf_cfg_cc_set_transport_params_req_st *)
                         p_crm_pdb->p_recv_payload)->
                         dns_server_addr.addr.addr.ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
			}
#endif
            break;

            /* Received Secondary DNS Server address: IPV4 address */
            
        case ICF_CFG_TRACE_SEC_DNS_SERVER_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->sec_dns_server_addr.addr.addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                     sec_dns_server_addr.addr.addr.ipv4_addr.octet_1;
                trace_data.int_val[1] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                     sec_dns_server_addr.addr.addr.ipv4_addr.octet_2;
                trace_data.int_val[2] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                     sec_dns_server_addr.addr.addr.ipv4_addr.octet_3;
                trace_data.int_val[3] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                     sec_dns_server_addr.addr.addr.ipv4_addr.octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->sec_dns_server_addr.addr.addr_type)
			{
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        ((icf_cfg_cc_set_transport_params_req_st *)
                         p_crm_pdb->p_recv_payload)->
                         sec_dns_server_addr.addr.addr.ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
			}
#endif
            break;
 
            /* Received Self Transport address: IPV4 address */
            
        case ICF_CFG_TRACE_SELF_TRNSPT_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_transport_address.addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_transport_address.addr.
                    ipv4_addr.octet_1;
                trace_data.int_val[1] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_transport_address.addr.
                    ipv4_addr.octet_2;
                trace_data.int_val[2] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_transport_address.addr.
                    ipv4_addr.octet_3;
                trace_data.int_val[3] = (icf_int32_t)
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_transport_address.addr.
                    ipv4_addr.octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR ==
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_transport_address.addr_type)
            {
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, 
                        ((icf_cfg_cc_set_transport_params_req_st *)
                         p_crm_pdb->p_recv_payload)->self_transport_address.
                        addr.ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
            }
#endif

            else if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
                    ((icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_transport_address.addr_type)
            {
                
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, 
                        ((icf_cfg_cc_set_transport_params_req_st *)
                         p_crm_pdb->p_recv_payload)->self_transport_address.
                        addr.domain.str);
                trace_data.string_val[0].str_len =
                   (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            }

            break;

            /* Received Call Parameters: Presence flag */
            
        case ICF_CFG_TRACE_CALL_PARAMS_RECD:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = ((icf_cfg_cc_set_call_params_req_st *)
                    p_crm_pdb->p_recv_payload)->bit_mask;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_call_params_req_st *)p_crm_pdb->
                 p_recv_payload)->default_settings;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_call_params_req_st *)p_crm_pdb->
                 p_recv_payload)->qos_mark;

            
#ifndef ICF_FEATURE_ABANDON
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str, 
                        ((icf_cfg_cc_set_call_params_req_st *)
                         p_crm_pdb->p_recv_payload)->phone_info.str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
#endif

            break;
            
            /* Received Call Parameters: setup, alerting, release & 
             * modifymedia timers */
            
        case ICF_CFG_TRACE_CALL_TIMERS_RECD:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = ((icf_cfg_cc_set_call_params_req_st *)
                    p_crm_pdb->p_recv_payload)->setup_timer;
            trace_data.int_val[1] = ((icf_cfg_cc_set_call_params_req_st *)
                    p_crm_pdb->p_recv_payload)->alerting_timer;
            trace_data.int_val[2] = ((icf_cfg_cc_set_call_params_req_st *)
                    p_crm_pdb->p_recv_payload)->release_timer;
            trace_data.int_val[3] = ((icf_cfg_cc_set_call_params_req_st *)
                    p_crm_pdb->p_recv_payload)->modify_media_timer;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* Received Call Parameters: registration resp and retry timers */
           
        case ICF_CFG_TRACE_REG_TIMERS_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = ((icf_cfg_cc_set_call_params_req_st *)
                    p_crm_pdb->p_recv_payload)->registration_resp_timer;
            trace_data.int_val[1] = ((icf_cfg_cc_set_call_params_req_st *)
                    p_crm_pdb->p_recv_payload)->registration_retry_timer;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

          
            /* Self Id parameters received: all_lines, line id, action & 
             * priority */
        case ICF_CFG_TRACE_SELF_ID_PRMS_RECD:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                 p_recv_payload)->bit_mask;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                 p_recv_payload)->line_id;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                 p_recv_payload)->action;
            trace_data.int_val[3] = (icf_int32_t)
                ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                 p_recv_payload)->priority;
#if 0      
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_addr.mode.str);    
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
#endif       
            break;
        
           /* self address received  */
            
        case ICF_CFG_TRACE_SELF_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0]= (icf_int32_t)
                ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                 p_recv_payload)->line_id;
           	trace_data.int_val[1]= (icf_int32_t)
                ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                 p_recv_payload)->action;
            trace_data.int_val[2]= (icf_int32_t)
                ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                 p_recv_payload)->self_addr.addr_type;
#if 0
            trace_data.int_val[2]= (icf_int32_t)
                ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                 p_recv_payload)->self_addr.user_param;
            trace_data.int_val[3]= (icf_int32_t)
                ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                 p_recv_payload)->self_addr.transport;
#endif
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_addr.addr_val.str);    
            trace_data.string_val[0].str_len =
                (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
#if 0            
            icf_port_strcpy(trace_data.string_val[1].str,
                    ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_addr.phone_context.str);    
            trace_data.string_val[1].str_len =
                icf_port_strlen(trace_data.string_val[1].str);
#endif
            break;
                      
           /* Seld Id parameter: self name and default settings */
#if 0 /*MERGE_LEFT*/     
        case ICF_CFG_TRACE_SELF_NAME_N_DEF_SET_RECD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0]= (icf_int32_t)
                ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                 p_recv_payload)->default_settings;

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_self_id_params_req_st *)p_crm_pdb->
                     p_recv_payload)->self_name.str);    
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            break;
#endif
            /* Registration parameters: presence flag, all_lines, line_id and 
             * action */
            
        case ICF_CFG_TRACE_REG_PRMS_REQ_RECD:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                 p_recv_payload)->line_id;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                 p_recv_payload)->action;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                 p_recv_payload)->bit_mask;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_CFG_TRACE_SEC_REG_PRMS_REQ_RECD:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                 p_recv_payload)->sec_registrar_addr.line_id;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                 p_recv_payload)->sec_registrar_addr.action;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                 p_recv_payload)->bit_mask;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
            
            /* Registrar address: IPV4 or Domain name if any */
            
        case ICF_CFG_TRACE_REG_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == 
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->registrar_addr.addr.addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->registrar_addr.addr.addr.ipv4_addr.
                    octet_1;
                trace_data.int_val[1] = (icf_int32_t)
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->registrar_addr.addr.addr.ipv4_addr.
                    octet_2;
                trace_data.int_val[2] = (icf_int32_t)
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->registrar_addr.addr.addr.ipv4_addr.
                    octet_3;
                trace_data.int_val[3] = (icf_int32_t)
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->registrar_addr.addr.addr.ipv4_addr.
                    octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == 
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                    registration_param_list[p_crm_pdb->line_id].registrar_addr.
                    addr.addr_type)
            {
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        ((icf_cfg_cc_set_registration_params_req_st *)
                         p_crm_pdb->p_recv_payload)->
                        registration_param_list[p_crm_pdb->line_id].
                        registrar_addr.addr.addr.ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
			}
#endif
            else if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->registrar_addr.addr.addr_type)
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        ((icf_cfg_cc_set_registration_params_req_st *)
                         p_crm_pdb->p_recv_payload)->registrar_addr.addr.addr.
                        domain.str);
                trace_data.string_val[0].str_len =
                     (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            }
            break;
            
            /* Secondary registrar address: IPV4 or Domain name if any */
            
        case ICF_CFG_TRACE_SEC_REG_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == 
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                     sec_registrar_addr.transport_addr.addr.addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                    sec_registrar_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_1;
                trace_data.int_val[1] = (icf_int32_t)
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                    sec_registrar_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_2;
                trace_data.int_val[2] = (icf_int32_t)
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                    sec_registrar_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_3;
                trace_data.int_val[3] = (icf_int32_t)
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                    sec_registrar_addr.transport_addr.addr.addr.
                    ipv4_addr.octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == 
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                    sec_registrar_addr.addr.addr_type)
            {
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        ((icf_cfg_cc_set_registration_params_req_st *)
                         p_crm_pdb->p_recv_payload)->
                        registration_param_list[p_crm_pdb->line_id].
                        sec_registrar_addr.addr.addr.ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
			}
#endif
            else if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
                    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                     p_recv_payload)->
                    sec_registrar_addr.transport_addr.addr.addr_type)
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        ((icf_cfg_cc_set_registration_params_req_st *)
                         p_crm_pdb->p_recv_payload)->
                        sec_registrar_addr.transport_addr.addr.addr.domain.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
            }
            break;

        /* Other Registration parameters: Reg port, Reg mode, 
         * registration duration */

        case ICF_CFG_TRACE_REG_PRMS_RECD:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                 p_recv_payload)->registrar_addr.port_num;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                 p_recv_payload)->reg_mode;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                 p_recv_payload)->registration_duration;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
#if 0 /*MERGE_LEFT*/
	    if(((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
				    p_recv_payload)->registration_param_list[p_crm_pdb->line_id].bit_mask & ICF_CONFIG_RG_PRM_AUTH_KEY)
	    {
		    trace_data.number_of_string_values++;

		    icf_port_strcpy(trace_data.string_val[0].str,
				    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
				     p_recv_payload)->registration_param_list[p_crm_pdb->line_id].
				    authentication_key.str);
		    trace_data.string_val[0].str_len =
			    icf_port_strlen(trace_data.string_val[0].str);
	    }

	    if(((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
				    p_recv_payload)->registration_param_list[p_crm_pdb->line_id].bit_mask & ICF_CONFIG_RG_PRM_AUTH_PASSWORD)
	    {
		    trace_data.number_of_string_values++;

		    icf_port_strcpy(trace_data.string_val[1].str,
				    ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
				     p_recv_payload)->registration_param_list[p_crm_pdb->line_id].
				    authentication_password.str);
		    trace_data.string_val[1].str_len =
			    icf_port_strlen(trace_data.string_val[1].str);
	    }
#endif
	    break;

	    /* Recd Service parameters: bit_mask, call hold option,
	     * max_num_of_call_forwarding_hops, max_num_of_calls_in_call_waiting*/
        
        case ICF_CFG_TRACE_SEC_REG_PRMS_RECD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_registration_params_req_st *)p_crm_pdb->
                 p_recv_payload)->sec_registrar_addr.transport_addr.port_num;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

/*#ifndef ICF_FEATURE_ABANDON*/
	case ICF_CFG_TRACE_SRVC_PRMS_REQ_RECD:
	    trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
	    trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->bit_mask;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->call_hold_option.hold_option;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->max_num_of_call_forwarding_hops;
            trace_data.int_val[3] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->max_num_of_calls_in_call_waiting;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

          /* Received Service parameters: call_hold_resume_timer, 
           * reinvite_race_timer, twc_timer and cw_alert_timer */
        
        case ICF_CFG_TRACE_SRVC_TIMERS_RECD:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->call_hold_resume_timer;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->reinvite_race_timer;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->twc_timer;
            trace_data.int_val[3] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->cw_alert_timer;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

          /* Recd Service parameters: ctu_timer, cta_timer, cnd_alert_timer,
           * cw_n_cnd_alert_timer */
            
        case ICF_CFG_TRACE_CALL_FWD_TIMERS_RECD:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->ctu_timer;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->cta_timer;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->cnd_alert_timer;
            trace_data.int_val[3] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->cw_n_cnd_alert_timer;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
           
             /* Received Service subscription flag */

        case ICF_CFG_TRACE_SRVC_SUBS_FLAG_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->service_subscription_flag.line_id;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->service_subscription_flag.status;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            break;

            /* Received Service activation flag */
            
        case ICF_CFG_TRACE_SRVC_ACT_FLAG_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->service_activation_flag.line_id;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->service_activation_flag.status;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            break;

               /* Received CFU Address */
            
        case ICF_CFG_TRACE_CFU_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfu.line_id;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfu.address.addr_type;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                     p_recv_payload)->addr_for_cfu.address.addr_val.str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
           break; 

            /* Received CFU Address parameters*/
#if 0            
        case ICF_CFG_TRACE_CFU_ADDR_PRMS_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfu.address.user_param;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfu.address.transport;

            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                     p_recv_payload)->addr_for_cfu.address.phone_context.str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            icf_port_strcpy(trace_data.string_val[1].str,
                    ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                     p_recv_payload)->addr_for_cfu.address.mode.str);
            trace_data.string_val[1].str_len =
                icf_port_strlen(trace_data.string_val[1].str);
            
            break;
#endif
            /* Received CFB Address */
            
        case ICF_CFG_TRACE_CFB_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfb.line_id;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfb.address.addr_type;

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                     p_recv_payload)->addr_for_cfb.address.addr_val.str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
           break; 

            /* Received CFB Address parameters*/
#if 0            
        case ICF_CFG_TRACE_CFB_ADDR_PRMS_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfb.address.user_param;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfb.address.transport;

            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                     p_recv_payload)->addr_for_cfb.address.phone_context.str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            icf_port_strcpy(trace_data.string_val[1].str,
                    ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                     p_recv_payload)->addr_for_cfb.address.mode.str);
            trace_data.string_val[1].str_len =
                icf_port_strlen(trace_data.string_val[1].str);
            
            break;
#endif
            /* Received CFD Address */
            
        case ICF_CFG_TRACE_CFD_ADDR_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfd.line_id;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfd.address.addr_type;
            
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                     p_recv_payload)->addr_for_cfd.address.addr_val.str);
            trace_data.string_val[0].str_len =
               (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
           break; 
#if 0
            /* Received CFD Address parameters*/
            
        case ICF_CFG_TRACE_CFD_ADDR_PRMS_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfd.address.user_param;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                 p_recv_payload)->addr_for_cfd.address.transport;

            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                     p_recv_payload)->addr_for_cfd.address.phone_context.str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            icf_port_strcpy(trace_data.string_val[1].str,
                    ((icf_cfg_cc_set_service_params_req_st *)p_crm_pdb->
                     p_recv_payload)->addr_for_cfd.address.mode.str);
            trace_data.string_val[1].str_len =
                icf_port_strlen(trace_data.string_val[1].str);
            
            break;
#endif
#ifndef ICF_FEATURE_ABANDON
            /* VMS Req parameters recd */ 
            
        case ICF_CFG_TRACE_VMS_PRMS_RECD:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                 p_recv_payload)->all_lines;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                 p_recv_payload)->line_id;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                 p_recv_payload)->action;
            trace_data.int_val[3] = (icf_int32_t)
                ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                 p_recv_payload)->vms_account_addr.addr_type;

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                     p_recv_payload)->vms_account_addr.addr_val.str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            break; 

            /* VMS address parameters recd */
               
        case ICF_CFG_TRACE_VMS_ADDR_PRMS_RECD:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                 p_recv_payload)->vms_account_addr.bit_mask;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                 p_recv_payload)->vms_account_addr.user_param;
            trace_data.int_val[2] = (icf_int32_t)
                ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                 p_recv_payload)->vms_account_addr.transport;

            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                     p_recv_payload)->vms_account_addr.phone_context.str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            icf_port_strcpy(trace_data.string_val[1].str,
                    ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                     p_recv_payload)->vms_account_addr.mode.str);
            trace_data.string_val[1].str_len =
                icf_port_strlen(trace_data.string_val[1].str);
            break;

            /* VMS Duration and group account flag recd */
               
        case ICF_CFG_TRACE_VMS_OTHER_PRMS_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)
                ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                 p_recv_payload)->subscribe_duration;
            trace_data.int_val[1] = (icf_int32_t)
                ((icf_cfg_cc_set_vms_params_req_st *)p_crm_pdb->
                 p_recv_payload)->group_account;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT; 

            break;
#endif 
            /* Displaying the Response of an API */
        case ICF_CFG_TRACE_RESP_SENT:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_glb_pdb->
                p_recv_msg->hdr.api_id;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->result;
            trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->error_cause;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cfg_error_cause_to_str(p_crm_pdb->error_cause, 
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);

           break;

        /* Scalable parameters:max_lines and max_users_per_line that are set*/
            
        case ICF_CFG_TRACE_SCALABLE_LINE_PRMS_SET:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_config_data->
                max_lines;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->p_config_data->
                max_users_per_line;
            trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->p_config_data->
                num_of_calls_to_log;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* Scalable parameters:max audio, video, registration & 
             * Service calls that are set */

        case ICF_CFG_TRACE_SCALABLE_CALL_PRMS_SET:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = p_crm_pdb->p_config_data->max_audio_calls;
            trace_data.int_val[1] = p_crm_pdb->p_config_data->max_video_calls;
            trace_data.int_val[2] = p_crm_pdb->p_config_data->max_service_calls;
            trace_data.int_val[3] = p_crm_pdb->p_config_data->
                max_registration_req;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        /* Proxy address's parameters for a particular line: Line Id, transport
         *  mode & port number that are set*/

        case ICF_CFG_TRACE_PROXY_ADDR_LINE_SET:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->line_id;
            /* If line id is ALL_LINES then reset it to line 0
             * so that information is fetched from it, as 
             * p_line_data doesnot have any index corresponding 
             * to ALL_LINES*/
            if (ICF_ALL_LINES == p_crm_pdb->line_id)
            {
                p_crm_pdb->line_id = 0;
            }
            if (ICF_NULL == p_crm_pdb->p_line_data[p_crm_pdb->line_id])
            {
                trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, (icf_uint8_t*)"Proxy address deleted");
            }
            else
            {
                trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->transport_mode;
                trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->proxy_addr.port_num;
                trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            }
            break;
            
           /* Proxy address for the above line IPV4 address that has been set */
                        
        case ICF_CFG_TRACE_PROXY_ADDRESS_SET:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
          
            /* If line id is ALL_LINES then reset it to line 0
             * so that information is fetched from it, as 
             * p_line_data doesnot have any index corresponding 
             * to ALL_LINES*/
            if (ICF_ALL_LINES == p_crm_pdb->line_id)
            {
                p_crm_pdb->line_id = 0;
            }
            if (ICF_NULL == p_crm_pdb->p_line_data[p_crm_pdb->line_id])
            {
                trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, (icf_uint8_t*)"Proxy address deleted");
            }
            else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->proxy_addr.addr.addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->proxy_addr.addr.addr.
                    ipv4_addr.octet_1;
                trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->proxy_addr.addr.addr.
                    ipv4_addr.octet_2;
                trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->proxy_addr.addr.addr.
                    ipv4_addr.octet_3;
                trace_data.int_val[3] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->proxy_addr.addr.addr.
                    ipv4_addr.octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->proxy_addr.addr.addr_type)
            {
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                        p_line_data[p_crm_pdb->line_id]->proxy_addr.addr.addr.
                        ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
			}
#endif            
            else if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->proxy_addr.addr.addr_type)
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                        p_line_data[p_crm_pdb->line_id]->proxy_addr.addr.addr.
                        domain.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
            }
            break;

        /* Secondary proxy address has been set */
                        
        case ICF_CFG_TRACE_SEC_PROXY_ADDRESS_SET:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            /* If line id is ALL_LINES then reset it to line 0
             * so that information is fetched from it, as 
             * p_line_data doesnot have any index corresponding 
             * to ALL_LINES*/
            if (ICF_ALL_LINES == p_crm_pdb->line_id)
            {
                p_crm_pdb->line_id = 0;
            }
            if ((ICF_NULL == p_crm_pdb->p_line_data[p_crm_pdb->line_id]) ||
                (ICF_NULL == p_crm_pdb->p_line_data[p_crm_pdb->line_id]->
                    p_sec_proxy_addr))
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t*)"No secondary proxy address configured\n");
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
            }
            else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->
                p_sec_proxy_addr->addr.addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->
                    p_sec_proxy_addr->addr.addr.
                    ipv4_addr.octet_1;
                trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->
                    p_sec_proxy_addr->addr.addr.
                    ipv4_addr.octet_2;
                trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->
                    p_sec_proxy_addr->addr.addr.
                    ipv4_addr.octet_3;
                trace_data.int_val[3] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->
                    p_sec_proxy_addr->addr.addr.
                    ipv4_addr.octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->
                p_sec_proxy_addr->transport_addr.addr_type)
            {
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                        p_line_data[p_crm_pdb->line_id]->
                        p_sec_proxy_addr->transport_addr.addr.
                        ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
			}
#endif            
            else if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->
                p_sec_proxy_addr->addr.addr_type)
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                        p_line_data[p_crm_pdb->line_id]->
                        p_sec_proxy_addr->addr.addr.
                        domain.str);
                trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
            }
            break;

            /* Self_transport_address that is set */
            
        case ICF_CFG_TRACE_SELF_TRNSPT_ADDR_SET:
#ifdef ICF_IPV6_ENABLED
			if(p_crm_pdb->p_config_data->
				self_ip_address.addr.addr_type == ICF_TRANSPORT_ADDRESS_IPV4_ADDR){
#endif

            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_config_data->
                self_ip_address.addr.addr.ipv4_addr.octet_1;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->p_config_data->
                self_ip_address.addr.addr.ipv4_addr.octet_2;
            trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->p_config_data->
                self_ip_address.addr.addr.ipv4_addr.octet_3;
            trace_data.int_val[3] = (icf_int32_t)p_crm_pdb->p_config_data->
                self_ip_address.addr.addr.ipv4_addr.octet_4;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
#ifdef ICF_IPV6_ENABLED
			}
			else{
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
				icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
					p_config_data->self_ip_address.addr.addr.ipv6_addr.str);
				trace_data.string_val[0].str_len =
					icf_port_strlen(trace_data.string_val[0].str);
			}
#endif
            break;

            /* Self transport mode and port number that are set */

        case ICF_CFG_TRACE_SELF_TRNSPT_MODE_SET:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_config_data->
                self_ip_address.port_num;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->p_config_data->
                self_mode;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* Call Parameters: default_settings and qos_mark that are set*/
#if 0 /*MERGELEFT*/
        case ICF_CFG_TRACE_CALL_PRMS_SET:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_config_data->
                default_settings;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->p_config_data->
                qos_mark;
            
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            if(ICF_NULL != p_crm_pdb->p_config_data->p_phone_info)
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                        p_config_data->p_phone_info->str);
            }
                
            break;
#endif            
            /* Call Parameters: setup, alerting, release & modifymedia timers */
            
        case ICF_CFG_TRACE_CALL_TIMERS_SET:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = p_crm_pdb->p_config_data->setup_timer;
            trace_data.int_val[1] = p_crm_pdb->p_config_data->alerting_timer;
            trace_data.int_val[2] = p_crm_pdb->p_config_data->release_timer;
            trace_data.int_val[3] = p_crm_pdb->p_config_data->
                modify_media_timer;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* Call Parameters: registration resp and retry timers */
           
        case ICF_CFG_TRACE_REG_TIMERS_SET:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = p_crm_pdb->p_config_data->
                registration_resp_timer;
            trace_data.int_val[1] = p_crm_pdb->p_config_data->
                registration_retry_timer;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
          
        /* Indicates maximum number of users on a line have been reached */
            
        case ICF_CFG_TRACE_MAX_NUM_OF_USERS_REACHED:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = p_crm_pdb->line_id;
            trace_data.int_val[1] = icf_dbm_get_public_user_count(p_crm_pdb->p_line_data[p_crm_pdb->line_id]->
                p_user_db);
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

           /* Self_address has been set */
         
        case ICF_CFG_TRACE_SELF_ADDR_SET:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0]= (icf_int32_t)p_crm_pdb->line_id;
            trace_data.int_val[1]= (icf_int32_t)p_crm_pdb->user_id;
            trace_data.int_val[2]= (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->p_user_db->public_user.addr_type;
#ifdef ICF_MULTILINE
            trace_data.int_val[3]= (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->
                p_user_priority_list[p_crm_pdb->user_id];
#endif
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->p_user_db->public_user.addr_val.str);    
            break;

/*#ifndef ICF_FEATURE_ABANDON*/
        case ICF_CFG_TRACE_SELF_ADDR_PRMS_SET:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
#if 0
            trace_data.int_val[0]= (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->
                p_user_addr_list[p_crm_pdb->user_id].user_param;
            trace_data.int_val[1]= (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->
                p_user_addr_list[p_crm_pdb->user_id].transport;
            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->
                    p_user_addr_list[p_crm_pdb->user_id].phone_context.str);    
            icf_port_strcpy(trace_data.string_val[1].str, p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->
                    p_user_addr_list[p_crm_pdb->user_id].mode.str);
#endif
            break;

                 
           /* Self name has been set */

        case ICF_CFG_TRACE_SELF_NAME_SET:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0]= (icf_int32_t)p_crm_pdb->line_id;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->user_name.str);    
            trace_data.string_val[0].str_len =
               (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

            /* Self default settings has been set */
            
        case ICF_CFG_TRACE_SELF_DEFAULT_SETTINGS_SET:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0]= (icf_int32_t)p_crm_pdb->line_id;
            trace_data.int_val[1]= (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->default_settings;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

              /* Line block has been deallocated because there is no 
               * user address, registrar or proxy present */
            
        case ICF_CFG_TRACE_LINE_BLK_DEALLOCATED:       
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0]= (icf_int32_t)p_crm_pdb->line_id;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* Registrar address that has been set */
            
        case ICF_CFG_TRACE_REG_ADDR_SET:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            /* If line id is ALL_LINES then reset it to line 0
             * so that information is fetched from it, as 
             * p_line_data doesnot have any index corresponding 
             * to ALL_LINES*/
            if (ICF_ALL_LINES == p_crm_pdb->line_id)
            {
                p_crm_pdb->line_id = 0;
            }
            if (ICF_NULL == p_crm_pdb->p_line_data[p_crm_pdb->line_id])
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t*)"No registrar address configured\n");
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
            }
            else if (ICF_TRANSPORT_ADDRESS_IPV4_ADDR == 
                    p_crm_pdb->p_line_data[p_crm_pdb->line_id]->
                    registrar_addr.addr.addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->registrar_addr.addr.addr.
                    ipv4_addr.octet_1;
                trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->registrar_addr.addr.addr.
                    ipv4_addr.octet_2;
                trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->registrar_addr.addr.addr.
                    ipv4_addr.octet_3;
                trace_data.int_val[3] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->registrar_addr.addr.addr.
                    ipv4_addr.octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == 
                    p_crm_pdb->p_line_data[p_crm_pdb->line_id]->
                    registrar_addr.addr.addr_type)
            {
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                        p_line_data[p_crm_pdb->line_id]->registrar_addr.addr.
                        addr.ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
            }
#endif
            else if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->registrar_addr.addr.
                    addr_type)
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                        p_line_data[p_crm_pdb->line_id]->registrar_addr.addr.
                        addr.domain.str);
                trace_data.string_val[0].str_len =
                    (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
            }
            break;

            /* Secondary registrar address that has been set */
            
        case ICF_CFG_TRACE_SEC_REG_ADDR_SET:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
           
            /* If line id is ALL_LINES then reset it to line 0
             * so that information is fetched from it, as 
             * p_line_data doesnot have any index corresponding 
             * to ALL_LINES*/
            if (ICF_ALL_LINES == p_crm_pdb->line_id)
            {
                p_crm_pdb->line_id = 0;
            }
            if ((ICF_NULL == p_crm_pdb->p_line_data[p_crm_pdb->line_id]) || 
                (ICF_NULL == p_crm_pdb->p_line_data[p_crm_pdb->line_id]->
                    p_sec_registrar_addr))
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t*)"No secondary registrar address configured\n");
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
            }
            else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == 
                    p_crm_pdb->p_line_data[p_crm_pdb->line_id]->
                    p_sec_registrar_addr->addr.addr_type)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->
                    p_sec_registrar_addr->addr.addr.ipv4_addr.octet_1;
                trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->
                    p_sec_registrar_addr->addr.addr.ipv4_addr.octet_2;
                trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->
                    p_sec_registrar_addr->addr.addr.ipv4_addr.octet_3;
                trace_data.int_val[3] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->
                    p_sec_registrar_addr->addr.addr.ipv4_addr.octet_4;
            }
#ifdef ICF_IPV6_ENABLED
			else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == 
                    p_crm_pdb->p_line_data[p_crm_pdb->line_id]->
                    p_sec_registrar_addr->addr.addr_type)
            {
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                        p_line_data[p_crm_pdb->line_id]->
                        p_sec_registrar_addr->addr.addr.ipv6_addr.str);
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
            }
#endif
            else if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->p_sec_registrar_addr->addr.
                    addr_type)
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                        p_line_data[p_crm_pdb->line_id]->
                        p_sec_registrar_addr->addr.addr.domain.str);
                trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
            }
            break;

            /* Registration parameters: Line Id, Port num, 
             * Registration duration, Authentication key and Password */
            
        case ICF_CFG_TRACE_REG_PRMS_SET:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            if (ICF_ALL_LINES == p_crm_pdb->line_id)
            {
                p_crm_pdb->line_id = 0;
            }
            if (ICF_NULL == p_crm_pdb->p_line_data[p_crm_pdb->line_id])
            {
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t*)"No registrar address configured\n");
                trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);
            }
            else
            {
                trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->line_id;
                trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->registrar_addr.port_num;
                trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->registration_duration;
            
                trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            }
            break;

		/* Service Parameters: call_hold_option, max_num_of_call_forwarding_hops
         *  and max_num_of_calls_in_call_waiting that have been set */
#ifdef ICF_SERVICE_ENABLE            
        case ICF_CFG_TRACE_SRVC_PRMS_SET:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_config_data->
                hold_option;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->p_config_data->
                max_num_of_call_forwarding_hops;
            trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->p_config_data->
                max_num_of_calls_in_call_waiting;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* Service parameters: call_hold_resume_timer, reinvite_race_timer,
           * twc_timer and cw_alert_timer that have been set */
        
        case ICF_CFG_TRACE_SRVC_TIMERS_SET:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_config_data->
                call_hold_resume_timer;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->p_config_data->
                reinvite_race_timer;
            trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->p_config_data->
                twc_timer;
            trace_data.int_val[3] = (icf_int32_t)p_crm_pdb->p_config_data->
                cw_alert_timer;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

          /* Service parameters: ctu_timer, cta_timer, cnd_alert_timer,
           * cw_n_cnd_alert_timer that have been set */
            
        case ICF_CFG_TRACE_CALL_FWD_TIMERS_SET:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_config_data->
                ctu_timer;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->p_config_data->
                cta_timer;
            trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->p_config_data->
                cnd_alert_timer;
            trace_data.int_val[3] = (icf_int32_t)p_crm_pdb->p_config_data->
                cw_n_cnd_alert_timer;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* Service Subscribe parameters that have been set*/
             
        case ICF_CFG_TRACE_SRVC_SUBSCRIBE_PRMS_SET:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_config_data->
                global_service_subscription_flag;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->p_config_data->
                global_service_activation_flag;
            trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->p_config_data->
                subs_resp_timer;
            trace_data.int_val[3] = (icf_int32_t)p_crm_pdb->p_config_data->
                subs_retry_timer;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
            

            /* Service subscription and activation flags that have been set */
            
        case ICF_CFG_TRACE_SRVC_FLAGS_SET:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->line_id;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->service_subscription_flag;
            trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->service_activation_flag;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            break;

            /* CFU Address that has been set */
            
        case ICF_CFG_TRACE_CFU_ADDR_SET:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->line_id;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->addr_for_cfu.addr_type;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->addr_for_cfu.addr_val.str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
           break; 

          
            /* CFB Address that has been set */        
        case ICF_CFG_TRACE_CFB_ADDR_SET:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->line_id;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->addr_for_cfb.addr_type;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->addr_for_cfb.addr_val.str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
           break;


            /* CFD Address that has been set*/           
        case ICF_CFG_TRACE_CFD_ADDR_SET:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->line_id;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->addr_for_cfd.addr_type;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->addr_for_cfd.addr_val.str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
           break;

#endif /*Service Enable*/

#ifndef ICF_FEATURE_ABANDON    
            /* VMS Group parameters set */ 
            
        case ICF_CFG_TRACE_VMS_GRP_ADDR_PRMS_SET:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_config_data->
                p_group_vms_addr->addr_type;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->p_config_data->
                p_group_vms_addr->user_param;
            trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->p_config_data->
                p_group_vms_addr->transport;

            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str, 
                    p_crm_pdb->p_config_data->p_group_vms_addr->addr_val.str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            icf_port_strcpy(trace_data.string_val[1].str, 
                    p_crm_pdb->p_config_data->p_group_vms_addr->
                    phone_context.str);
            trace_data.string_val[1].str_len =
                icf_port_strlen(trace_data.string_val[1].str);
            break;

            /* VMS Group duration and status set */

        case ICF_CFG_TRACE_VMS_GRP_PRMS_SET:
             trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->p_config_data->
                vms_subscribe_duration;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->p_config_data->
                vms_subscription_status;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            /* VMS addr for a line set */

        case ICF_CFG_TRACE_VMS_LINE_ADDR_PRMS_SET:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->p_vms_addr->addr_type;
            trace_data.int_val[1] = (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->p_vms_addr->user_param;
            trace_data.int_val[2] = (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->p_vms_addr->transport;
            
            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str, p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->p_vms_addr->addr_val.str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            icf_port_strcpy(trace_data.string_val[1].str, p_crm_pdb->
                    p_line_data[p_crm_pdb->line_id]->p_vms_addr->phone_context.
                    str);
            trace_data.string_val[1].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            break;

            /* VMS Duration for a line set */
            
        case ICF_CFG_TRACE_VMS_LINE_PRMS_SET:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)p_crm_pdb->
                p_line_data[p_crm_pdb->line_id]->vms_subscribe_duration;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
#endif              
        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

    } /* end of switch-case for trace_id */

    /* Finally, invoke the common macro for trace handling */
    ICF_TRACE(p_crm_pdb->p_glb_pdb, trace_id, &trace_data)

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)

}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_app_trace
 *
 * DESCRIPTION:     This function checks the trace id and calls the macro for
 *                  trace.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_void_t icf_cfg_app_trace(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_trace_id_t           trace_id)
{
    icf_trace_data_st   trace_data;
    icf_trace_criteria_data_st          *p_criteria_data = ICF_NULL;

    icf_return_t                         ret_val = ICF_SUCCESS;
    /* Klocwork Fix Rel 8.2 */ 
    icf_cmn_init_trace_data_st(&trace_data);
    /* Klocwork Fix Rel 8.2 end */
   
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    /* Fix for CSR-1-5249829 */
    ret_val = icf_cmn_check_app_trace(p_crm_pdb->p_glb_pdb,
                                        trace_id,
					p_criteria_data);
    if( ICF_FAILURE == ret_val)
    {
        return;
    }
    else
    {

   if((trace_id != ICF_TRACE_SYSTEM_RESTARTING) || (trace_id != ICF_TRACE_SYSTEM_REINITIALISING))
    {

         p_crm_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

         p_crm_pdb->p_glb_pdb->p_glb_data->app_trace_info.call_id =
                p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
     }
    switch(trace_id)
    {

         case ICF_TRACE_USER_INFO:
    
                 trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
             trace_data.int_val[0] =
                     (icf_uint32_t)(p_crm_pdb->line_id);
             trace_data.int_val[1] =
                      ((icf_cfg_cc_set_self_id_params_req_st *)
                            (p_crm_pdb->p_recv_payload))->action;

             trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
             
             icf_port_strcpy(trace_data.string_val[0].str,
                                    ((icf_cfg_cc_set_self_id_params_req_st *)
                            (p_crm_pdb->p_recv_payload))->self_name.str);

             trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

             icf_port_strcpy(trace_data.string_val[1].str,
                                     ((icf_cfg_cc_set_self_id_params_req_st *)
                            (p_crm_pdb->p_recv_payload))->self_addr.addr_val.str);
                                        
             trace_data.string_val[1].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);

             break;

	case ICF_TRACE_SELF_TRANSPORT_ADDR:
             {
              
              /* If per line address is configured */
              /* Enhanced for SPR 17565 */   
              if (ICF_CFG_SELF_ADDR_PER_LINE ==
                         p_crm_pdb->p_config_data->self_addr_per_line)
              {
               
                   icf_config_transport_address_st *p_local_addr = &(((icf_cfg_cc_set_transport_params_req_st *)
                                       p_crm_pdb->p_recv_payload)->self_addr_per_line);
                   trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
                   trace_data.int_val[0] =
                          (icf_uint32_t)(p_crm_pdb->line_id);

                   if ( p_crm_pdb->line_id < ICF_MAX_NUM_OF_LINES) trace_data.int_val[1] =
                           (icf_uint32_t)(p_crm_pdb->p_line_data[p_crm_pdb->line_id]->self_mode);

                   trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                   if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                                p_local_addr->transport_addr.addr.addr_type)
                   {
                       icf_port_strcpy(trace_data.string_val[0].str,
                          p_local_addr->transport_addr.addr.addr.domain.str);
                   }
                   else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                               p_local_addr->transport_addr.addr.addr_type)
                   {
                       icf_port_sprintf((icf_int8_t *)trace_data.string_val[0].str,
                                  "%d.%d.%d.%d",
                                   p_local_addr->transport_addr.addr.addr.ipv4_addr.octet_1,
                                   p_local_addr->transport_addr.addr.addr.ipv4_addr.octet_2,
                                   p_local_addr->transport_addr.addr.addr.ipv4_addr.octet_3,
                                   p_local_addr->transport_addr.addr.addr.ipv4_addr.octet_4);
                   }

       
              }
              else
              {   
                  icf_transport_addr_st *p_local_addr = &(((icf_cfg_cc_set_transport_params_req_st *)
                                       p_crm_pdb->p_recv_payload)->self_transport_address);

                  trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
                  trace_data.int_val[0] =
                         (icf_uint32_t)(p_crm_pdb->line_id);
                  trace_data.int_val[1] =
                         (icf_uint32_t)(p_crm_pdb->p_config_data->self_mode);

                  trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

                  if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                                        p_local_addr->addr_type)
                  {
                      icf_port_strcpy(trace_data.string_val[0].str,
                                    p_local_addr->addr.domain.str);
                  }
                  else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                                        p_local_addr->addr_type)
                  {
                      icf_port_sprintf((icf_int8_t *)trace_data.string_val[0].str,
                                 "%d.%d.%d.%d",
                                  p_local_addr->addr.ipv4_addr.octet_1,
                                  p_local_addr->addr.ipv4_addr.octet_2,
                                  p_local_addr->addr.ipv4_addr.octet_3,
                                  p_local_addr->addr.ipv4_addr.octet_4);
                  }
             }
    
             trace_data.string_val[0].str_len =
                     (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

             break;

             }

	case ICF_TRACE_PROXY_INFO:
            { 
               icf_cfg_cc_set_transport_params_req_st *p_recv_msg =
                  (icf_cfg_cc_set_transport_params_req_st *)(p_crm_pdb->p_recv_payload);
 
               icf_transport_addr_st *p_local_addr = 
                        &(p_recv_msg->proxy_addr.transport_addr.addr);

             trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
             trace_data.int_val[0] =
                     (icf_uint32_t)(p_crm_pdb->line_id);
             trace_data.int_val[1] =
                     (icf_uint32_t)(p_crm_pdb->p_config_data->self_mode);

              trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

                if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                                p_local_addr->addr_type)
               {
                 icf_port_strcpy(trace_data.string_val[0].str,
                      p_local_addr->addr.domain.str);
               }
               else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                        p_local_addr->addr_type)
               {
                      icf_port_sprintf((icf_int8_t *)trace_data.string_val[0].str,
                             "%d.%d.%d.%d",
                                      p_local_addr->addr.ipv4_addr.octet_1,
                                      p_local_addr->addr.ipv4_addr.octet_2,
                                      p_local_addr->addr.ipv4_addr.octet_3,
                                      p_local_addr->addr.ipv4_addr.octet_4);
                     }

         
             trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);

             break;
         }

    case ICF_TRACE_SEC_PROXY_INFO:
            { 
               icf_cfg_cc_set_transport_params_req_st *p_recv_msg =
                  (icf_cfg_cc_set_transport_params_req_st *)(p_crm_pdb->
                                                             p_recv_payload);
 
               icf_transport_addr_st *p_local_addr = 
                        &(p_recv_msg->sec_proxy_addr.transport_addr.addr);

             /* Print line id and action for secondary proxy being configured */
             trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
             trace_data.int_val[0] =
                     (icf_uint32_t)(p_crm_pdb->line_id);
             trace_data.int_val[1] =
                     (icf_uint32_t)(p_recv_msg->sec_proxy_addr.action);

              trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

                if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                                p_local_addr->addr_type)
               {
                 icf_port_strcpy(trace_data.string_val[0].str,
                      p_local_addr->addr.domain.str);
               }
               else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                        p_local_addr->addr_type)
               {
                      icf_port_sprintf((icf_int8_t *)trace_data.string_val[0].str,
                             "%d.%d.%d.%d",
                                      p_local_addr->addr.ipv4_addr.octet_1,
                                      p_local_addr->addr.ipv4_addr.octet_2,
                                      p_local_addr->addr.ipv4_addr.octet_3,
                                      p_local_addr->addr.ipv4_addr.octet_4);
                     }

         
             trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);

             break;
         } 
      
	case ICF_TRACE_REGISTRAR_INFO:
              {
                  icf_transport_addr_st *p_local_addr = 
                  &(((icf_cfg_cc_set_registration_params_req_st *)(p_crm_pdb->
                          p_recv_payload))->registrar_addr.addr);

                 trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
             trace_data.int_val[0] =
                     (icf_uint32_t)(p_crm_pdb->line_id);
             trace_data.int_val[1] =
                     ((icf_cfg_cc_set_registration_params_req_st *)
                            (p_crm_pdb->p_recv_payload))->action;
             trace_data.int_val[2] =
                            ((icf_cfg_cc_set_registration_params_req_st *)
                               (p_crm_pdb->p_recv_payload))->registration_duration; 
             trace_data.int_val[3] =
                      ((icf_cfg_cc_set_registration_params_req_st *)
                               (p_crm_pdb->p_recv_payload))->register_head_start_timer;

             trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

             if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                                p_local_addr->addr_type)
               {
                 icf_port_strcpy(trace_data.string_val[0].str,
                      p_local_addr->addr.domain.str);
               }
               else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                        p_local_addr->addr_type)
               {
                      icf_port_sprintf((icf_int8_t *)trace_data.string_val[0].str,
                             "%d.%d.%d.%d",
                                      p_local_addr->addr.ipv4_addr.octet_1,
                                      p_local_addr->addr.ipv4_addr.octet_2,
                                      p_local_addr->addr.ipv4_addr.octet_3,
                                      p_local_addr->addr.ipv4_addr.octet_4);
                     }

             trace_data.string_val[0].str_len =
                    icf_port_strlen(trace_data.string_val[0].str);

             break;
          }

    case ICF_TRACE_SEC_REGISTRAR_INFO:
              {
                  icf_transport_addr_st *p_local_addr = 
                  &(((icf_cfg_cc_set_registration_params_req_st *)(p_crm_pdb->
                          p_recv_payload))->
                            sec_registrar_addr.transport_addr.addr);

                 trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
             trace_data.int_val[0] =
                     (icf_uint32_t)(p_crm_pdb->line_id);
             trace_data.int_val[1] =
                     ((icf_cfg_cc_set_registration_params_req_st *)
                            (p_crm_pdb->p_recv_payload))->
                                sec_registrar_addr.action;

             trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

             if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                                p_local_addr->addr_type)
               {
                 icf_port_strcpy(trace_data.string_val[0].str,
                      p_local_addr->addr.domain.str);
               }
               else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                        p_local_addr->addr_type)
               {
                      icf_port_sprintf((icf_int8_t *)trace_data.string_val[0].str,
                             "%d.%d.%d.%d",
                                      p_local_addr->addr.ipv4_addr.octet_1,
                                      p_local_addr->addr.ipv4_addr.octet_2,
                                      p_local_addr->addr.ipv4_addr.octet_3,
                                      p_local_addr->addr.ipv4_addr.octet_4);
                     }

             trace_data.string_val[0].str_len =
                    (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

             break;
          }

	case ICF_TRACE_SYSTEM_RESTARTING:
        case ICF_TRACE_SYSTEM_REINITIALISING:

             trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

             trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

             break;
        case ICF_TRACE_DHCP_SERVER_ALREADY_CONFIGURED:
             
             trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

             trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

             break;
        case ICF_TRACE_PROXY_SERVER_ALREADY_CONFIGURED:

             trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

             trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

             break;
        case ICF_TRACE_DHCP_FAILURE:

             trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

             trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

             break;
        case ICF_TRACE_PCSCF_UNREACHABLE:
             
             trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
             
             icf_port_strcpy(trace_data.string_val[0].str,
                  p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[0].str);
             trace_data.string_val[0].str_len = 
                  p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[0].str_len;

             if (0 != p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[1].str_len)
             {
                 icf_port_strcpy(trace_data.string_val[1].str,
                      p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[1].str);
                 trace_data.string_val[1].str_len = 
                      p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[1].str_len;
                 
                 trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
             }
             else
             {
                 trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
             }
             break;
        case ICF_TRACE_DISCOVERED_PROXY_ADDRESS_FROM_DHCP:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
             
             icf_port_strcpy(trace_data.string_val[0].str,
                  p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[0].str);
             trace_data.string_val[0].str_len = 
                  p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[0].str_len;

             if (0 != p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[1].str_len)
             {
                 icf_port_strcpy(trace_data.string_val[1].str,
                      p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[1].str);
                 trace_data.string_val[1].str_len = 
                      p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[1].str_len;
                 
                 trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
             }
             break;
	default:
           return; 

    } /* end of switch-case for trace_id */

    /* Finally, invoke the common macro for trace handling */
    ICF_APP_TRACE(p_crm_pdb->p_glb_pdb, trace_id, &trace_data, p_criteria_data)
 }
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)

}/* End function */

#endif /* End of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_ERROR_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_error
 *
 * DESCRIPTION:     This function checks the error id and calls the macro for 
 *                  error reporting.
 *                     
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_void_t icf_cfg_error(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_error_t              ecode)
{
#ifdef ICF_TRACE_ENABLE
    
    icf_return_t			 ret_val=ICF_SUCCESS;
#endif 
    icf_error_data_st                    error_data;
    icf_error_t                          *p_ecode = ICF_NULL;

    if(ecode == ecode)
    {
      /* to remove warning */
    } 
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    error_data.num_of_integer_values = ICF_NO_INT_PRESENT;
    error_data.number_of_string_values = ICF_NO_STRING_PRESENT;

    /* Finally, invoke the common macro for error handling */
    ICF_SET_ECODE(p_crm_pdb->p_glb_pdb, &error_data, ecode, p_ecode)
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return;
}/* End function */
#endif /* End of #ifdef ICF_ERROR_ENABLE */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_init_line_blk
 * 
 * DESCRIPTION:     This function sets default values in a newly created line 
 *                  block.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 * 
 *****************************************************************************/
icf_return_t icf_cfg_init_line_blk(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_uint8_t              line_status,
        IN      icf_line_id_t            line_id)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_uint8_t				 i = 0;
    
    if (ICF_STATUS_LINE_NEW == line_status)
    {
        icf_port_memset(
                (icf_void_t*)(p_crm_pdb->p_line_data[line_id]),
                (icf_uint8_t)0,
                (icf_uint32_t)
                ICF_PORT_SIZEOF(icf_line_data_st));
        /* If this line has just been allocated, initialize line data */
        p_crm_pdb->p_line_data[line_id]->line_status = ICF_LINE_STATUS_FREE;
	p_crm_pdb->p_line_data[line_id]->line_id = line_id;
        p_crm_pdb->p_line_data[line_id]->num_of_calls = 0;
        p_crm_pdb->p_line_data[line_id]->num_call_waiting_calls = 0;
        p_crm_pdb->p_line_data[line_id]->p_call_ctx_list = ICF_NULL;

        /* Initialise the transport mode INVALID if User did not fill the mode
          then NAPTR query will be done to find the mode*/
        p_crm_pdb->p_line_data[line_id]->transport_mode = 
                                ICF_TRANSPORT_TYPE_INVALID;
        /*Assign default reg duration in case of non-IMS mode*/
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_FALSE)
        p_crm_pdb->p_line_data[line_id]->registration_duration = 
                                ICF_CFG_DEF_REG_DURATION_NON_IMS_CLIENT;
ICF_CHECK_IMS_END
        /*Assign default reg duration in case of IMS mode*/
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
        p_crm_pdb->p_line_data[line_id]->registration_duration = 
                                ICF_CFG_DEF_REG_DURATION_IMS_CLIENT;
ICF_CHECK_IMS_END
       /* Initialise the reg mode INVALID if User did not fill the mode
          then NAPTR query will be done to find the mode*/
        p_crm_pdb->p_line_data[line_id]->reg_mode = ICF_TRANSPORT_TYPE_INVALID;
        p_crm_pdb->p_line_data[line_id]->service_subscription_flag = 
                                ICF_NULL;
        p_crm_pdb->p_line_data[line_id]->service_activation_flag = ICF_NULL;
        p_crm_pdb->p_line_data[line_id]->line_data_received = ICF_NULL;
        p_crm_pdb->p_line_data[line_id]->default_settings = ICF_NULL;  
        p_crm_pdb->p_line_data[line_id]->default_settings |= 
                                    ICF_SETTINGS_CONFIG_PRESENTATION_IND;
#ifndef ICF_FEATURE_ABANDON
        p_crm_pdb->p_line_data[line_id]->p_vms_addr = ICF_NULL;
        p_crm_pdb->p_line_data[line_id]->vms_subscribe_duration = 
                                ICF_CFG_DEF_SUBS_DURATION;
#endif
#ifdef ICF_SESSION_TIMER
		p_crm_pdb->p_line_data[line_id]->session_timer =
								p_crm_pdb->p_config_data->session_timer;
		p_crm_pdb->p_line_data[line_id]->min_se = 
								p_crm_pdb->p_config_data->min_se;
#endif
        p_crm_pdb->p_line_data[line_id]->ignore_stale_param = 
                               p_crm_pdb->p_config_data->ignore_stale_param;
#ifdef ICF_SESSION_TIMER
        p_crm_pdb->p_line_data[line_id]->session_refresh_method = 
                               p_crm_pdb->p_config_data->session_refresh_method;
#endif
        /*Initialize the line data
	 * SPR 13978 Aman 06-01-07
         */
        for (i = 0; i <= ICF_WIN_TLS_SIP_SERVER; i++)
        {
                p_crm_pdb->p_line_data[line_id]->server_index[i] = 
                    ICF_MAX_NUM_OF_SERVERS;
        }
        
        icf_port_strcpy(
                p_crm_pdb->p_line_data[line_id]->user_name.str,
                (icf_uint8_t *)"\0");

        p_crm_pdb->p_line_data[line_id]->user_name.str_len = ICF_NULL;

		p_crm_pdb->p_line_data[line_id]->subs_hs_duration =
			ICF_CFG_DEF_SUBSCRIPTION_HS_DURATION;
	
		p_crm_pdb->p_line_data[line_id]->reg_hs_duration = 
			ICF_CFG_DEF_REG_HS_DURATION;

	 	p_crm_pdb->p_line_data[line_id]->sip_timer_t1 =
			ICF_CFG_DEF_SIP_TIMER_T1;
	
		p_crm_pdb->p_line_data[line_id]->sip_timer_t2 = 
			ICF_CFG_DEF_SIP_TIMER_T2;

		p_crm_pdb->p_line_data[line_id]->sip_timer_invite =
			ICF_CFG_DEF_SIP_TIMER_INVITE;
	
		p_crm_pdb->p_line_data[line_id]->sip_timer_non_invite = 
			ICF_CFG_DEF_SIP_TIMER_NON_INVITE;

        p_crm_pdb->p_line_data[line_id]->sip_timer_t4 =
			ICF_CFG_DEF_SIP_TIMER_T4;
	
        /* Start: Per Line Feature Additions */
        p_crm_pdb->p_line_data[line_id]->sip_dscp = 
			ICF_CFG_DEF_SIP_DSCP;
        
        p_crm_pdb->p_line_data[line_id]->rtp_dscp = 
			ICF_CFG_DEF_RTP_DSCP;

        p_crm_pdb->p_line_data[line_id]->hold_option = 
            ICF_CALL_HOLD_OPT_INV_WTH_MODE_SEND_ONLY;

        #ifdef ICF_SESSION_TIMER    
        
        p_crm_pdb->p_line_data[line_id]->session_timer_refresher =
            ICF_CFG_DEF_SESSION_TIMER_REFRESHER;
        #endif

        icf_port_memset(&(p_crm_pdb->p_line_data[line_id]->contact_user_info),
            0, ICF_PORT_SIZEOF(icf_string_st));

#ifdef ICF_NAT_RPORT_SUPPORT
       if(ICF_NULL != p_crm_pdb->p_line_data[line_id]->p_rport_config_data)
       {
            ICF_MEMFREE(
                     p_crm_pdb->p_glb_pdb,
                     p_crm_pdb->p_line_data[line_id]->p_rport_config_data,
                     ICF_MEM_COMMON,
                     p_crm_pdb->p_ecode,
                     ret_val)
       }
       p_crm_pdb->p_line_data[line_id]->p_rport_config_data = ICF_NULL;
#endif
       /*Deallocate and initialize the access_network_info header
         pointer*/
       if(ICF_NULL != p_crm_pdb->p_line_data[line_id]->p_access_network_info)
       {
           ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
                  p_crm_pdb->p_line_data[line_id]->p_access_network_info,
                  ICF_MEM_COMMON,
                  p_crm_pdb->p_ecode, 
                  ret_val)
       }    
       /*Changes for CSR 1-7334184 Start */
       p_crm_pdb->p_line_data[line_id]->remove_route_hdr = ICF_FALSE;
       /*Changes for CSR 1-7334184 End */
       
       /*Fix for SPR: 19999 (Merged CSR 1-7547597 : SPR 19898)*/
       p_crm_pdb->p_line_data[line_id]->session_refresh_method_strict_update =
                                                     ICF_FALSE;       
       
       /*Fix for CSR 1-9012191 : SPR 20697)*/
       p_crm_pdb->p_line_data[line_id]->strip_req_uri_param =
                                                     ICF_FALSE;                                                     
    }
    
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_scale_prms
 * 
 * DESCRIPTION:     This function validates the params recieved in the set 
 *                  scale params API.
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_cfg_validate_scale_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_scale_params_req_st   *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* copy the external request payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_scale_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
  
    /* validate max number of lines */ 
    if ((ICF_CFG_MIN_NUM_OF_LINES > p_recv_msg_payload->max_lines) ||
            (ICF_MAX_NUM_OF_LINES < p_recv_msg_payload->max_lines))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_MAX_LINES_OUT_OF_RANGE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
        
    /* validate max number of audio calls */ 
    else if (ICF_CFG_MIN_AUDIO_CALLS > p_recv_msg_payload->max_audio_calls)
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_MAX_AUDIO_CALLS_OUT_OF_RANGE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    
    /* Validate max number of service calls
     * Not required since this comparison always returns false due to datatype 
     */
        
    /* validate max number of users per line */ 
    else if (ICF_CFG_MIN_USERS_PER_LINE > p_recv_msg_payload->
            max_users_per_line || 
            ICF_MAX_NUM_OF_USERS_PER_LINE < p_recv_msg_payload->
            max_users_per_line)
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_MAX_USERS_PER_LINE_OUT_OF_RANGE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
        
   /* validate max number of registration calls
    * Not required, as this value can be more or less than 
    * (users per line * max lines)
    */
    
    /* validate max number of calls to log */ 
    else if (ICF_MAX_NUM_OF_CALLS_TO_LOG < 
            p_recv_msg_payload->num_of_calls_to_log)
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_MAX_CALLS_TO_LOG_OUT_OF_RANGE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_trnspt_prms
 * 
 * DESCRIPTION:     This function will validate the transport parameters.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_trnspt_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                           ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_transport_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_line_id_t                          line_id = 0;
    icf_uint8_t                            line_status[ICF_MAX_NUM_OF_LINES];
    icf_transport_mode_t	               effective_self_mode = 
                                            ICF_TRANSPORT_MODE_BOTH;
        
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_transport_params_req_st *)
        (p_crm_pdb->p_recv_payload);
    
    /*If self transport is set per line as well as sys wide, send
    *failure*/
    if (((ICF_CFG_CC_SET_TRP_PRM_SLF_TRANSPORT_ADDRESS & 
         p_recv_msg_payload->bit_mask) ||
        (ICF_CFG_CC_SET_TRP_PRM_SLF_PORT & p_recv_msg_payload->bit_mask) ||
        (ICF_SET_TRP_REQ_SLF_MODE & p_recv_msg_payload->bit_mask)) &&
        (ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE & p_recv_msg_payload->bit_mask))
     {
        ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INVALID_ACTION)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_RESP_SENT)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
     }
	    
    /* 
     * Initialize the status of all lines to old. This is done only to 
     * prevent any garbage value in the array. This can be removed later if not
     * found necessary.
     */
    for(line_id = 0; line_id < ICF_MAX_NUM_OF_LINES; line_id++)
    {
        line_status[line_id] = ICF_STATUS_LINE_OLD;
    }

     /*If self_addr type is not already set in config_data then set it 
     *from msg_payload*/
    if (ICF_CFG_SELF_ADDR_INV_TYPE == 
         p_crm_pdb->p_config_data->self_addr_per_line)
    {
        if (ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE & p_recv_msg_payload->bit_mask)
        {
            p_crm_pdb->p_config_data->self_addr_per_line = 
               ICF_CFG_SELF_ADDR_PER_LINE;
        }
        else if (ICF_SET_TRP_REQ_SLF_TRANSPORT_ADDRESS & 
                 p_recv_msg_payload->bit_mask)
        {
            p_crm_pdb->p_config_data->self_addr_per_line = 
               ICF_CFG_SELF_ADDR_SYS_WIDE;
        }
    }

    /*Check for bitmask*/
    if (((ICF_CFG_SELF_ADDR_SYS_WIDE == 
         p_crm_pdb->p_config_data->self_addr_per_line) &&
        (ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE & p_recv_msg_payload->bit_mask)) ||
        ((ICF_CFG_SELF_ADDR_PER_LINE == 
         p_crm_pdb->p_config_data->self_addr_per_line) &&
        !(ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE & p_recv_msg_payload->bit_mask)))
    {
        ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INVALID_ACTION)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_RESP_SENT)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    if (ICF_CFG_SELF_ADDR_SYS_WIDE & 
		    p_crm_pdb->p_config_data->self_addr_per_line)
    {

	    /* Validate Action */

		    if (ICF_NULL != (ICF_SET_TRP_REQ_SLF_MODE &
					    p_recv_msg_payload->bit_mask))
		    {
			    effective_self_mode = p_recv_msg_payload->self_mode;
                            /*If mode of operation is IMS and self mode is received as TLS 
                             *then reject request*/
                            if ((ICF_TRANSPORT_TYPE_TLS == effective_self_mode) &&
                                     (ICF_OPERATION_MODE_IMS == p_crm_pdb->p_config_data->ims_oprn_flag))
                            {
                                ICF_CFG_SET_RESPONSE(p_crm_pdb, 
                                        ICF_FAILURE,
                                        ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE)	

                                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                                return ret_val;
                            }
		    }
		    else
		    {
			    effective_self_mode = p_crm_pdb->p_config_data->self_mode;
		    }

	    if(ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_SLF_TRANSPORT_ADDRESS &
				    p_recv_msg_payload->bit_mask))
	    {
		    /* Address type can not be domain name */
		    if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
				    p_recv_msg_payload->self_transport_address.addr_type)
		    {
			    ICF_CFG_SET_RESPONSE(
					    p_crm_pdb,
					    ICF_FAILURE,
					    ICF_CAUSE_SELF_ADDR_DOMAIN_NAME_NOT_HANDLED)
				    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_TRNSPT_ADDR_RECD)
				    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
				    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
				    return ret_val;
		    }
	    }/* End if for presence flag */

	    if(ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_SLF_PORT &
				    p_recv_msg_payload->bit_mask))
	    {
		    /* validate the range of port number */
		    if((ICF_CFG_MIN_PORT_VALUE >
					    p_recv_msg_payload->self_port) &&
				    (ICF_CONFIG_ACTION_DELETE != 
				     p_recv_msg_payload->proxy_addr.action))
		    {
			    ICF_CFG_SET_RESPONSE(
					    p_crm_pdb,
					    ICF_FAILURE,
					    ICF_CAUSE_IP_PORT_OUT_OF_RANGE)
				    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_TRNSPT_PRMS_REQ_RECD)
				    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
				    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
				    return ret_val;
		    }    
	    }/* End if for self port */
    }
    else if (ICF_CFG_SELF_ADDR_PER_LINE == 
				p_crm_pdb->p_config_data->self_addr_per_line)
    {
	    /*Validate self transport address if received per line,
	     *this function sends response to application if validation
	     *failed*/
	    ret_val = icf_cfg_validate_self_addr_per_line(p_crm_pdb);

	    if (ICF_FALSE == p_crm_pdb->is_valid_param)
	    {
		    return ret_val;
	    }
	    effective_self_mode = p_crm_pdb->effective_self_mode;
    }

    if(ICF_NULL != (ICF_SET_TRP_REQ_PROXY_ADDR &
			    p_recv_msg_payload->bit_mask))
    {

        /*  If DHCP server address and Proxy server address, both present in same
            API then reject the request with error_cause as ICF_CAUSE_INVALID_ACTION.
            If DHCP server is already configured earlier by application and proxy 
            server address is received from application then reject the request with 
            error_cause as ICF_CAUSE_DHCP_SERVER_ADDR_ALREADY_CONFIGURED
         */
        /* Check if DHCP server address present in request */
        if(ICF_NULL != (ICF_SET_TRP_REQ_DHCP_SERVER_ADDR & 
                    p_recv_msg_payload->bit_mask))
        {
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_DHCP_AND_PROXY_COMBINED)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;	    
        }/* end if DHCP Server present in request */
        /* Check if DHCP Server already configured */
        else if(ICF_NULL != (p_crm_pdb->p_config_data->p_dhcp_server_addr))
        {
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_DHCP_SERVER_ADDR_ALREADY_CONFIGURED)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_DHCP_SERVER_ALREADY_CONFIGURED)	    
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }/* end if DHCP Server already configured */
        /*If mode of operation is IMS and proxy mode is received as TLS 
         *then reject request*/
        else if ((ICF_TRANSPORT_TYPE_TLS == 
                  p_recv_msg_payload->proxy_addr.transport_mode) &&
                 (ICF_OPERATION_MODE_IMS == p_crm_pdb->p_config_data->ims_oprn_flag))
        {
            ICF_CFG_SET_RESPONSE(p_crm_pdb, 
                    ICF_FAILURE,
                    ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE)	

            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }
        /* Check is added to allow ICF_TRANSPORT_TYPE_INVALID by user in 
           Set_Trans_Param API , When Application gives this it means 
           Application wants to do the NAPTR query */
        else if((effective_self_mode != ICF_TRANSPORT_TYPE_TLS)&&
                (effective_self_mode != ICF_TRANSPORT_MODE_BOTH)
                && (p_recv_msg_payload->proxy_addr.transport_mode != 
                                    ICF_TRANSPORT_MODE_BOTH) && 
                (p_recv_msg_payload->proxy_addr.transport_mode != 
                    effective_self_mode) &&
                 (ICF_TRANSPORT_TYPE_INVALID != 
                        p_recv_msg_payload->proxy_addr.transport_mode))
        {
            ICF_CFG_SET_RESPONSE(p_crm_pdb, 
                    ICF_FAILURE,
                    ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE)	

            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }
        
        else if(ICF_ALL_LINES != p_recv_msg_payload->proxy_addr.line_id)
        {
            line_id = p_recv_msg_payload->proxy_addr.line_id;
            /* Validate the line id */
            if(line_id >= p_crm_pdb->p_config_data->max_lines)
            {
                ICF_CFG_TRACE(p_crm_pdb, 
                        ICF_CFG_TRACE_PROXY_ADDR_REQ_RECD)
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }
            /* Allocate line pointer if action is add. This step is done in 
             * validation, so that if failure occurs, API may be failed from 
             * here itself, and the remaining parameters of self Address and 
             * self mode need not be rolled back.
             */
            else if(ICF_SUCCESS == icf_cfg_check_line_in_db(p_crm_pdb,
                    line_id, line_status,(icf_boolean_t) ICF_TRUE)) 
            {
                /* If primary proxy is being added */
                if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                   proxy_addr.action)
                {
                    /* Check if proxy is already present on the line */
                    if (ICF_NULL != (ICF_CFG_LINE_DATA_PROXY_RECD 
                        & p_crm_pdb->p_line_data[line_id]->line_data_received))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                ICF_FAILURE,
                                ICF_CAUSE_PROXY_ADDR_ALREADY_CONFIGURED)
            
                        ret_val = ICF_SUCCESS;
                        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
		            	return ret_val;
                    }
                    /* If sec. being ADDed (if present in API) on same 
                     * line or ALL_LINES is same as primary present in 
                     * API, then return FAILURE */
                    else if ((ICF_NULL != (ICF_SET_TRP_REQ_SEC_PROXY_ADDR &
                             p_recv_msg_payload->bit_mask)) &&
                            (ICF_CONFIG_ACTION_ADD == 
                             p_recv_msg_payload->sec_proxy_addr.action) &&
                            ((line_id == p_recv_msg_payload->
                              sec_proxy_addr.line_id) ||
                             (ICF_ALL_LINES == p_recv_msg_payload->
                              sec_proxy_addr.line_id)))
                    {
                        /* Check if primary and secondary in the API are same */
                        if (ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                               &(p_recv_msg_payload->sec_proxy_addr.transport_addr),
                               &(p_recv_msg_payload->proxy_addr.transport_addr),
                               p_crm_pdb->p_glb_pdb))
                        {
                            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                            ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                ICF_FAILURE,
                                ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR)
            
                            ret_val = ICF_SUCCESS;
                            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
		            	    return ret_val;
                        }
                    }
                }/* End if (action == ADD) */
                else if (ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->
                         proxy_addr.action)
                {
                    /* If primary is not already configured on line 
                     * and trying to MODIFY primary on line, return 
                     * FAILURE */
                    if (ICF_NULL == (ICF_CFG_LINE_DATA_PROXY_RECD
                        & p_crm_pdb->p_line_data[line_id]->line_data_received))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb,
                            ICF_FAILURE,
                            ICF_CAUSE_PRIMARY_PROXY_NOT_CONFIGURED)
            
                        ret_val = ICF_SUCCESS;
                        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
		            	return ret_val;
                    }
                    /* If same secondary also present in API with action 
                     * ADD or MODIFY for same line_id or ALL_LINES, return
                     * FAILURE */
                    else if ((ICF_NULL != (ICF_SET_TRP_REQ_SEC_PROXY_ADDR &
			            p_recv_msg_payload->bit_mask)) &&
                        ((line_id == p_recv_msg_payload->sec_proxy_addr.line_id) ||
                         ICF_ALL_LINES == p_recv_msg_payload->sec_proxy_addr.line_id) &&
                        ((ICF_CONFIG_ACTION_ADD == 
                          p_recv_msg_payload->sec_proxy_addr.action) || 
                         (ICF_CONFIG_ACTION_MODIFY ==
                          p_recv_msg_payload->sec_proxy_addr.action)))
                    {
                        /* Check if primary and secondary in the API are same */
                        if (ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                               &(p_recv_msg_payload->sec_proxy_addr.transport_addr),
                               &(p_recv_msg_payload->proxy_addr.transport_addr),
                               p_crm_pdb->p_glb_pdb))
                        {
                            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                            ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                ICF_FAILURE,
                                ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR)
            
                            ret_val = ICF_SUCCESS;
                            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
		            	    return ret_val;
                        }
                    }
                    /* Check if secondary proxy not present in API but 
                     * already present on the line, then return error if
                     * the secondary on line is the same as primary */
                    else if ((ICF_NULL == (ICF_SET_TRP_REQ_SEC_PROXY_ADDR &
                             p_recv_msg_payload->bit_mask)) && 
                             (ICF_NULL != (ICF_CFG_LINE_DATA_SEC_PROXY_RECD 
                        & p_crm_pdb->p_line_data[line_id]->line_data_received)))
                    {
                        /* Check if secondary already configured is the same */
                        if (ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                               p_crm_pdb->p_line_data[line_id]->
                                   p_sec_proxy_addr,
                               &(p_recv_msg_payload->proxy_addr.transport_addr),
                               p_crm_pdb->p_glb_pdb))
                        {
                            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                            ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                ICF_FAILURE,
                                ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR)
            
                            ret_val = ICF_SUCCESS;
                            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
		            	    return ret_val;
                        }
                     }                        
                }/* End if (action == MODIFY)*/
                else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                         proxy_addr.action)
                {
                    /* Primary proxy should already be present */
                    if (ICF_NULL == (ICF_CFG_LINE_DATA_PROXY_RECD & 
                       p_crm_pdb->p_line_data[line_id]->line_data_received))
                    {
                       ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                       ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                            ICF_CAUSE_PRIMARY_PROXY_NOT_CONFIGURED)
                       ret_val = ICF_SUCCESS;
                       ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                       return ret_val;
                    }
                    /* FAILURE if secondary present in API with action other 
                     * than DELETE i.e. ADD or MODIFY for the same line_id */
                    else if ((ICF_NULL != (ICF_SET_TRP_REQ_SEC_PROXY_ADDR &
			            p_recv_msg_payload->bit_mask)) && 
                        (line_id == p_recv_msg_payload->sec_proxy_addr.line_id) &&
                        (ICF_CONFIG_ACTION_DELETE != 
                          p_recv_msg_payload->sec_proxy_addr.action))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                         ICF_CAUSE_INCONSISTENT_ACTIONS_FOR_PROXY_CONFIGURATION)
                        ret_val = ICF_SUCCESS;
                        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                        return ret_val;
                    }
                    /* FAILURE if secondary proxy already present on the line */
                    else if ((ICF_NULL == (ICF_SET_TRP_REQ_SEC_PROXY_ADDR &
                                    p_recv_msg_payload->bit_mask)) &&
                             (ICF_NULL != (ICF_CFG_LINE_DATA_SEC_PROXY_RECD 
                        & p_crm_pdb->p_line_data[line_id]->line_data_received)))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                            ICF_CFG_SET_RESPONSE(p_crm_pdb,
                            ICF_FAILURE,
                            ICF_CAUSE_SECONDARY_PROXY_CONFIGURED)
            
                        ret_val = ICF_SUCCESS;
                        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
		        	    return ret_val;
                    }
                }
            }/* End if (SUCCESS == icf_cfg_check_line_in_db) */
        }
        else 
        {
            /* All lines is true. Allocate pointers for the lines if action is 
             * ADD.The reason is  same as above
             */
            /* If for ALL_LINES, action is ADD or MODIFY for 
             * primary proxy, then secondary present in API 
             * should not be the same unless the secondary 
             * is being deleted*/
            if ((ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                        proxy_addr.action) || 
                    (ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->
                     proxy_addr.action))
            {
                if ((ICF_NULL != (ICF_SET_TRP_REQ_SEC_PROXY_ADDR &
                                p_recv_msg_payload->bit_mask)) && 
                        ((ICF_CONFIG_ACTION_ADD == 
                          p_recv_msg_payload->sec_proxy_addr.action) ||
                         (ICF_CONFIG_ACTION_MODIFY == 
                          p_recv_msg_payload->sec_proxy_addr.action)))
                {
                    if (ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                                &(p_recv_msg_payload->sec_proxy_addr.transport_addr),
                                &(p_recv_msg_payload->proxy_addr.transport_addr),
                                p_crm_pdb->p_glb_pdb))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                    ICF_FAILURE,
                                    ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR)

                        ret_val = ICF_SUCCESS;
                        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                        return ret_val;
                    }
                }
            }

            /* for loop is placed in side actions for efficiency.
             * After checking the action for ALL_LINES, a loop 
             * performs that action on the lines upto max lines */
            
            /* If primary proxy is being added */
            if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                    proxy_addr.action)
            {
                for(line_id = 0; 
                    (line_id < p_crm_pdb->p_config_data->max_lines) &&
                    (line_id < ICF_MAX_NUM_OF_LINES); 
                    line_id++)
                {
                    if(ICF_SUCCESS == icf_cfg_check_line_in_db(p_crm_pdb,
                                line_id, line_status,(icf_boolean_t) ICF_TRUE)) 
                    {
                        /* Check if proxy is already present on the line */
                        if (ICF_NULL != (ICF_CFG_LINE_DATA_PROXY_RECD 
                                    & p_crm_pdb->p_line_data[line_id]->line_data_received))
                        {
                            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                            ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                        ICF_FAILURE,
                                        ICF_CAUSE_PROXY_ADDR_ALREADY_CONFIGURED)

                            ret_val = ICF_SUCCESS;
                            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                            return ret_val;
                        }
                    }/* end if ICF_SUCCESS == icf_cfg_check_line_in_db */
                    else
                    {
                        ICF_CFG_SET_RESPONSE(
                                p_crm_pdb,
                                ICF_FAILURE,
                                ICF_CAUSE_INVALID_LINE_ID)
                            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                            return ret_val;
                    }/* end else ICF_FAILURE == icf_cfg_check_line_in_db */
                }/* for loop = ALL_LINES */
            }/* End if (action == ADD) */
            else if (ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->
                    proxy_addr.action)
            {
                for(line_id = 0; 
                    (line_id < p_crm_pdb->p_config_data->max_lines) &&
                    (line_id < ICF_MAX_NUM_OF_LINES);
                    line_id++)
                {
                    if(ICF_SUCCESS == icf_cfg_check_line_in_db(p_crm_pdb,
                                line_id, line_status,(icf_boolean_t) ICF_FALSE)) 
                    {
                        /* Check if primary already configured on line */
                        if (ICF_NULL == (ICF_CFG_LINE_DATA_PROXY_RECD
                            & p_crm_pdb->p_line_data[line_id]->line_data_received))
                        {
                            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                            ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                ICF_FAILURE,
                                ICF_CAUSE_PRIMARY_PROXY_NOT_CONFIGURED)

                            ret_val = ICF_SUCCESS;
                            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                            return ret_val;
                        }
                        /* Check if secondary proxy is received in API with action add or
                         *modify, then check if prinmary and secondary are same or not*/
                        else if ((ICF_NULL != (ICF_SET_TRP_REQ_SEC_PROXY_ADDR & 
                                      p_recv_msg_payload->bit_mask)) && 
                                  (ICF_CONFIG_ACTION_DELETE != 
                                      p_recv_msg_payload->sec_proxy_addr.action))
                        {
                            if (ICF_SUCCESS ==
                                    icf_cmn_compare_transport_addresses(
                                        &(p_recv_msg_payload->sec_proxy_addr.transport_addr),
                                        &(p_recv_msg_payload->
                                            proxy_addr.transport_addr),
                                        p_crm_pdb->p_glb_pdb))
                            {
                                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                                ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                            ICF_FAILURE,
                                            ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR)

                                ret_val = ICF_SUCCESS;
                                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                                return ret_val;
                            }
                        }
                        /*Check if sec proxy is not received in API but present on line 
                         *then return error if same as primary address*/
                        else if ((ICF_NULL == (ICF_SET_TRP_REQ_SEC_PROXY_ADDR & 
                                      p_recv_msg_payload->bit_mask)) &&
                                 (ICF_NULL != (ICF_CFG_LINE_DATA_SEC_PROXY_RECD 
                                    & p_crm_pdb->p_line_data[line_id]->line_data_received)))
                        {
                            /* Check if secondary already configured is same */
                            if (ICF_SUCCESS == 
                                    icf_cmn_compare_transport_addresses(
                                        p_crm_pdb->p_line_data[line_id]->
                                        p_sec_proxy_addr,
                                        &(p_recv_msg_payload->
                                            proxy_addr.transport_addr),
                                        p_crm_pdb->p_glb_pdb))
                            {
                                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                                ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                            ICF_FAILURE,
                                            ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR)

                                ret_val = ICF_SUCCESS;
                                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                                return ret_val;
                            }
                        }
                    }/* end if ICF_SUCCESS == icf_cfg_check_line_in_db */
                    else
                    {
                        ICF_CFG_SET_RESPONSE(
                                p_crm_pdb,
                                ICF_FAILURE,
                                ICF_CAUSE_INVALID_LINE_ID)
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                        return ret_val;
                    }/* end else ICF_FAILURE == icf_cfg_check_line_in_db */
                }/* end for loop - ALL_LINES */
            }/* End if (action == MODIFY)*/
            else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                    proxy_addr.action)
            {
                for(line_id = 0; 
                    (line_id < p_crm_pdb->p_config_data->max_lines) &&
                    (line_id < ICF_MAX_NUM_OF_LINES);
                    line_id++)
                {
                    if(ICF_SUCCESS == icf_cfg_check_line_in_db(p_crm_pdb,
                                line_id, line_status,(icf_boolean_t) ICF_FALSE)) 
                    {
                        /* Primary proxy should already be present */
                        if (ICF_NULL == (ICF_CFG_LINE_DATA_PROXY_RECD & 
                                    p_crm_pdb->p_line_data[line_id]->line_data_received))
                        {
                            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                            ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                        ICF_CAUSE_PRIMARY_PROXY_NOT_CONFIGURED)
                            ret_val = ICF_SUCCESS;
                            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                            return ret_val;
                        }
                        /* if secondary is present in API and action != DELETE
                         * i.e. action is ADD or MODIFY, then return error */
                        else if ((ICF_NULL != (ICF_SET_TRP_REQ_SEC_PROXY_ADDR &
                                        p_recv_msg_payload->bit_mask)) && 
                                (ICF_CONFIG_ACTION_DELETE != 
                                 p_recv_msg_payload->sec_proxy_addr.action))
                        {
                            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                            ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                        ICF_FAILURE,
                                        ICF_CAUSE_INCONSISTENT_ACTIONS_FOR_PROXY_CONFIGURATION)

                            ret_val = ICF_SUCCESS;
                            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                            return ret_val;
                        }
                        /* FAILURE if secondary proxy already present on line and not received in API */
                        else if ((ICF_NULL == (ICF_SET_TRP_REQ_SEC_PROXY_ADDR &
                                        p_recv_msg_payload->bit_mask)) &&
                                 (ICF_NULL != (ICF_CFG_LINE_DATA_SEC_PROXY_RECD 
                                    & p_crm_pdb->p_line_data[line_id]->line_data_received)))
                        {
                            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                                ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                        ICF_FAILURE,
                                        ICF_CAUSE_SECONDARY_PROXY_CONFIGURED)

                            ret_val = ICF_SUCCESS;
                            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                            return ret_val;
                        }
                    }/* end if ICF_SUCCESS == icf_cfg_check_line_in_db */
                    else
                    {
                        ICF_CFG_SET_RESPONSE(
                                p_crm_pdb,
                                ICF_FAILURE,
                                ICF_CAUSE_INVALID_LINE_ID)
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                        return ret_val;
                    }/* end else ICF_FAILURE == icf_cfg_check_line_in_db */
                }/* end for loop ALL_LINES */
            }/* end action == DELETE */
        }/* End else for all_lines */
    }/* End if for bit_mask */


    /*  If DHCP server address is received from application and Proxy server address
     * is already configured on any line then reject the request with error_cause 
     *as ICF_CAUSE_PROXY_SERVER_ALREADY_CONFIGURED  */

    /*  If DHCP Server address is configured in API */
    else if(ICF_NULL != (ICF_SET_TRP_REQ_DHCP_SERVER_ADDR &
                         p_recv_msg_payload->bit_mask))
    {
        /* Proxy should not already be configured on any line */
        for (line_id = 0; line_id < p_crm_pdb->
                p_config_data->max_lines ; line_id++)
        {
            if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk( 
                        p_crm_pdb->p_glb_pdb, line_id,
                        &p_crm_pdb->p_line_data[line_id],
                        p_crm_pdb->p_ecode))
            {
                continue;
            }
            /* Check only primary proxy, as secondary configured only if 
               primary is configured */
            if(ICF_NULL != (ICF_CFG_LINE_DATA_PROXY_RECD & p_crm_pdb->
                        p_line_data[line_id]->line_data_received))
            {
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,	
                        ICF_CAUSE_PROXY_ADDR_ALREADY_CONFIGURED)
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_PROXY_SERVER_ALREADY_CONFIGURED)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }
        }
    }
    
    if(ICF_NULL != (ICF_SET_TRP_REQ_SEC_PROXY_ADDR &
        p_recv_msg_payload->bit_mask))
    {
        /* If primary proxy not present in request */
        if (ICF_NULL == (p_recv_msg_payload->bit_mask & 
            ICF_SET_TRP_REQ_PROXY_ADDR))
        {
            /* Validate secondary proxy address and the corresponding action */
            ret_val = icf_cfg_validate_sec_proxy_addr (p_crm_pdb);
        }
        else /* Primary Proxy present in request */
        {
            /* If primary and secondary are configured for different lines 
             * in the request */
            if (p_recv_msg_payload->proxy_addr.line_id != 
                p_recv_msg_payload->sec_proxy_addr.line_id)
            {
                /* Validate secondary proxy address and the corresponding 
                   action */
                ret_val = icf_cfg_validate_sec_proxy_addr (p_crm_pdb);
            }
            /* fix for SPR 18783 : If primary proxy and secondary proxy both have
             *  been configured and application on ALL LINES then do the validation
             *  for secondary proxy 
             */
            else if(ICF_ALL_LINES == p_recv_msg_payload->sec_proxy_addr.line_id)
            {
                /* Validate secondary proxy address and the corresponding action */
                ret_val = icf_cfg_validate_sec_proxy_addr (p_crm_pdb);
            }
            /* fix for SPR 18613 : If primary proxy has been configured and application
               tries to modify or delete the secondary proxy without adding it first,
               we have to fail the request.
            */
            else
            {
                /* SPR 18783: Validate line id and if line id is valid and 
                 * then perform further validation for action and proxy
                 * address configuration
                 */
                line_id = p_recv_msg_payload->sec_proxy_addr.line_id;

                if(line_id >= p_crm_pdb->p_config_data->max_lines)
                {
                    ICF_CFG_TRACE(p_crm_pdb, 
                         ICF_CFG_TRACE_SEC_PROXY_ADDR_REQ_RECD)
                    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                    ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
                    ret_val = ICF_SUCCESS;
                }
                else if((ICF_NULL == (ICF_CFG_LINE_DATA_SEC_PROXY_RECD & 
                                p_crm_pdb->p_line_data[line_id]->line_data_received)) &&
                    ((ICF_CONFIG_ACTION_DELETE == 
                                 p_recv_msg_payload->sec_proxy_addr.action) ||
                    (ICF_CONFIG_ACTION_MODIFY == 
                                 p_recv_msg_payload->sec_proxy_addr.action)))
                {
                    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                    ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                    ICF_CAUSE_SECONDARY_PROXY_NOT_CONFIGURED)
                    ret_val = ICF_SUCCESS;
                }
            }  
        }
    }/* End if for bit_mask */
    if(ICF_NULL != (ICF_SET_TRP_REQ_DNS_SERVER_ADDR &
        p_recv_msg_payload->bit_mask))
    {
        /*IPTK will not allow modification of Primary DNS server data
         *so rejecting the request*/
        if (ICF_NULL != p_crm_pdb->p_config_data->p_dns_server_addr)
        {
            ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                    ICF_CAUSE_DNS_SERVER_ADDRESS_MODIFICATION_NOT_ALLOWED)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }
        /* Check if secondary is also present in set transport request */
        else if (ICF_NULL != (ICF_SET_TRP_REQ_SEC_DNS_SERVER_ADDR &
          p_recv_msg_payload->bit_mask))
        {
            /*Check if primary and secondary DNS addresses in request are same*/
            if (ICF_SUCCESS == 
               icf_cmn_compare_transport_addresses(&(p_recv_msg_payload->
                 dns_server_addr), &(p_recv_msg_payload->
                 sec_dns_server_addr),
                   p_crm_pdb->p_glb_pdb))
            {
                ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                    ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_DNS_SERVER_ADDR)
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }/* end if (primary and secondary same) */
        }/* end if (secondary DNS Server address present in request) */
        /* Check if secondary DNS Server already configured */
        else if (ICF_NULL != p_crm_pdb->p_config_data->p_sec_dns_server_addr)
        {
            if (ICF_SUCCESS == 
               icf_cmn_compare_transport_addresses(&(p_recv_msg_payload->
                 dns_server_addr), p_crm_pdb->p_config_data->
                 p_sec_dns_server_addr, p_crm_pdb->p_glb_pdb))
            {
                ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                    ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_DNS_SERVER_ADDR)
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }/* end if (primary and secondary same) */
        }/* end if (secondary DNS Server address already configured) */
    }/* End if (bitmask for primary DNS Server address) */

    if(ICF_NULL != (ICF_SET_TRP_REQ_SEC_DNS_SERVER_ADDR &
        p_recv_msg_payload->bit_mask))
    {
        /*IPTK will not allow modification of Secondary DNS server address
         *so rejecting the request*/
        if (ICF_NULL != p_crm_pdb->p_config_data->p_dns_server_addr)
        {
            ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                    ICF_CAUSE_DNS_SERVER_ADDRESS_MODIFICATION_NOT_ALLOWED)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }
        else if ((ICF_NULL == (ICF_SET_TRP_REQ_DNS_SERVER_ADDR &
                    p_recv_msg_payload->bit_mask)) && 
            (ICF_NULL == p_crm_pdb->p_config_data->p_dns_server_addr))
        {
            ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                    ICF_CAUSE_PRIMARY_DNS_SERVER_NOT_CONFIGURED)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }
        /* If primary is already configured and secondary is same as primary*/
        else if ((ICF_NULL == (ICF_SET_TRP_REQ_DNS_SERVER_ADDR &
                           p_recv_msg_payload->bit_mask)) &&
                 (ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                            &(p_recv_msg_payload->sec_dns_server_addr), 
                            p_crm_pdb->p_config_data->p_dns_server_addr,
                            p_crm_pdb->p_glb_pdb)))
        {
            ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_DNS_SERVER_ADDR)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }/* end if (primary and secondary same) */
    }/* End if (bitmask for secondary DNS Server address) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_sec_proxy_addr
 * 
 * DESCRIPTION:     This function will validate the secondary proxy address 
 *                  and the action to be performed on it.
 *                  
 * RETURNS:         ICF_SUCCESS.
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_sec_proxy_addr(
        INOUT   icf_crm_pdb_st          *p_crm_pdb) 
{
    icf_return_t                            ret_val = ICF_SUCCESS; 
    icf_line_id_t                           line_id = 0;
    icf_cfg_cc_set_transport_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_uint8_t                             line_status[ICF_MAX_NUM_OF_LINES];

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    p_recv_msg_payload = (icf_cfg_cc_set_transport_params_req_st *)
        (p_crm_pdb->p_recv_payload);

    line_id = p_recv_msg_payload->sec_proxy_addr.line_id;
    if(ICF_ALL_LINES != line_id)
    {
        /* Validate the line id */
        if(line_id >= p_crm_pdb->p_config_data->max_lines)
        {
            ICF_CFG_TRACE(p_crm_pdb, 
                    ICF_CFG_TRACE_SEC_PROXY_ADDR_REQ_RECD)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INVALID_LINE_ID)
            ret_val = ICF_SUCCESS;
        }
        if (ICF_TRUE == p_crm_pdb->is_valid_param)
        {
            if(ICF_SUCCESS == icf_cfg_check_line_in_db(p_crm_pdb, line_id, 
                        line_status, (icf_boolean_t) ICF_FALSE))
            {
                /* If pointer is returned by DBM i.e line is configured */
				if ( line_id < ICF_MAX_NUM_OF_LINES) {/*klocwork*/
                if (ICF_CONFIG_ACTION_ADD == 
                        p_recv_msg_payload->sec_proxy_addr.action)
                {
                    /* When secondary proxy is being added, primary 
                     * should either be present in the same API or 
                     * should be configured already on that line, 
                     * as secondary can only be added on a line 
                     * which also has primary proxy */
                    
                    /* if primary is present in the same API */
                    if ((line_id < ICF_MAX_NUM_OF_LINES) && ((ICF_NULL != (ICF_SET_TRP_REQ_PROXY_ADDR &
                                p_recv_msg_payload->bit_mask)) &&
                        (ICF_CONFIG_ACTION_ADD == 
                            p_recv_msg_payload->proxy_addr.action) && 
                        ((line_id == p_recv_msg_payload->proxy_addr.line_id) ||
                         (ICF_ALL_LINES == p_recv_msg_payload->proxy_addr.line_id))))
                    {
                        /* Do nothing - Valid behaviour
                         * No need to validate if primary and secondary in
                         * API are same on same/ALL lines. 
                         * Already done in validation of prim. */
                    }
                    /* if primary proxy is not present in API it should be  
                     * present on the line, else return FAILURE */
                    else if (p_crm_pdb->p_line_data[line_id]->line_data_received)
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                    ICF_FAILURE,
                                    ICF_CAUSE_PRIMARY_PROXY_NOT_CONFIGURED)
                        ret_val = ICF_SUCCESS;
                    }
                    /* if secondary proxy is being ADDed it should not 
                     * already be present on the same line */
                    else if (ICF_NULL != (ICF_CFG_LINE_DATA_SEC_PROXY_RECD & 
                                p_crm_pdb->p_line_data[line_id]->line_data_received))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                ICF_CAUSE_SECONDARY_PROXY_ADDR_ALREADY_CONFIGURED)
                        ret_val = ICF_SUCCESS;
                    }
                    /* if primary is already configured on the same line,
                     * then secondary should not be same */
                    else if (ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                                &(p_crm_pdb->p_line_data[line_id]->proxy_addr),
                                &(p_recv_msg_payload->sec_proxy_addr.transport_addr),
                                p_crm_pdb->p_glb_pdb))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                            ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR)
                        ret_val = ICF_SUCCESS;
                    }
                }/* End of action ADD */
                else if (ICF_CONFIG_ACTION_MODIFY == 
                        p_recv_msg_payload->sec_proxy_addr.action)
                {
                    /* if secondary proxy is being modified on a line,
                     * it should already be present on that line, else 
                     * return FAILURE */
                    if (ICF_NULL == (ICF_CFG_LINE_DATA_SEC_PROXY_RECD & 
                                p_crm_pdb->p_line_data[line_id]->line_data_received))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                            ICF_CAUSE_SECONDARY_PROXY_NOT_CONFIGURED)
                        ret_val = ICF_SUCCESS;
                    }
                    /* If secondary proxy is being modified on a line,
                     * it should not be the same as primary proxy already 
                     * configured on that line */
                    else if ((ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                                &(p_crm_pdb->p_line_data[line_id]->proxy_addr),
                                &(p_recv_msg_payload->sec_proxy_addr.transport_addr),
                                p_crm_pdb->p_glb_pdb)))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                    ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR)
                        ret_val = ICF_SUCCESS;
                    }                
                }/* End of action MODIFY */
                else if (ICF_CONFIG_ACTION_DELETE == 
                        p_recv_msg_payload->sec_proxy_addr.action)
                {
                    /* if secondary proxy is being deleted from a line,
                     * it should already be present on that line, else 
                     * return FAILURE */
                    if (ICF_NULL == (ICF_CFG_LINE_DATA_SEC_PROXY_RECD & 
                                p_crm_pdb->p_line_data[line_id]->line_data_received))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                ICF_CAUSE_SECONDARY_PROXY_NOT_CONFIGURED)
                        ret_val = ICF_SUCCESS;
                    }
                }/* End of action DELETE */
				}
            }/* End if(ICF_SUCCESS== icf_cfg_check_line_in_db()*/
        }
        else /* else FAILURE == icf_cfg_check_line_in_db()*/
        {
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INVALID_LINE_ID)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ret_val = ICF_SUCCESS;
        }/* End if(ICF_FAILURE== icf_cfg_check_line_in_db()*/
    }
    else /* all_lines */
    {
        /* for loop is placed in side actions for efficiency.
         * After checking the action for ALL_LINES, a loop 
         * performs that action on the lines upto max lines */
        
        if (ICF_CONFIG_ACTION_ADD == 
                p_recv_msg_payload->sec_proxy_addr.action)
        {
            if(ICF_SUCCESS == 
                    icf_cfg_check_line_configured(p_crm_pdb, (icf_boolean_t)ICF_FALSE))
            {
                for(line_id = 0; (line_id < p_crm_pdb->p_config_data->max_lines) && (line_id < ICF_MAX_NUM_OF_LINES); 
                        line_id++)
                {
                    /* When secondary proxy is being added, primary 
                     * should either be present in the same API or 
                     * should be configured already on that line, 
                     * as secondary can only be added on a line 
                     * which also has primary proxy */
                    
                    /* if primary is present in the same API */
                    if ((ICF_NULL != (ICF_SET_TRP_REQ_PROXY_ADDR &
                                    p_recv_msg_payload->bit_mask)) &&
                            (ICF_CONFIG_ACTION_ADD == 
                             p_recv_msg_payload->proxy_addr.action) &&
                            ((line_id == p_recv_msg_payload->proxy_addr.line_id) || 
                             (ICF_ALL_LINES == p_recv_msg_payload->proxy_addr.line_id)))
                    {
                        /* Do nothing - Valid behaviour
                         * No need to validate if primary and secondary in
                         * API are same on same/ALL lines. 
                         * Already done in validation of prim. */
                    }
                    /* SPR 18833 Fix: In continuation of logic for a single
                       line, the below 'if' has been replaced by 'else if'.
                       We should check for the presence of the primary proxy
                       on a line only if this request does not contain the 
                       primary proxy.
                    */ 
                    
                    /* if primary proxy is not present in API it should be  
                     * present on the line, else return FAILURE */
                    else if (ICF_NULL == (ICF_CFG_LINE_DATA_PROXY_RECD 
                                & p_crm_pdb->p_line_data[line_id]->line_data_received))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                    ICF_FAILURE,
                                    ICF_CAUSE_PRIMARY_PROXY_NOT_CONFIGURED)
                        ret_val = ICF_SUCCESS;
                        break; /* break from for loop in case of failure */
                    }
                    /* if secondary proxy is being ADDed it should not 
                     * already be present on the same line */
                    else if (ICF_NULL != (ICF_CFG_LINE_DATA_SEC_PROXY_RECD & 
                                p_crm_pdb->p_line_data[line_id]->line_data_received))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                    ICF_CAUSE_SECONDARY_PROXY_ADDR_ALREADY_CONFIGURED)
                        ret_val = ICF_SUCCESS;
                        break; /* break from for loop in case of failure */
                    }
                    /* if primary is already configured on the same line,
                     * then secondary should not be same */
                    else if (ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                                &(p_crm_pdb->p_line_data[line_id]->proxy_addr),
                                &(p_recv_msg_payload->sec_proxy_addr.transport_addr),
                                p_crm_pdb->p_glb_pdb))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                    ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR)
                        ret_val = ICF_SUCCESS;
                        break; /* break from for loop in case of failure */
                    }
                }/* end for loop for ALL_LINES */
            }/* end if ICF_SUCCESS == icf_cfg_check_line_in_db */
            else
            {
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ret_val = ICF_SUCCESS;
            }/* end if ICF_FAILURE == icf_cfg_check_line_in_db */
        }/* End of action ADD */
        else if (ICF_CONFIG_ACTION_MODIFY == 
                p_recv_msg_payload->sec_proxy_addr.action)
        {
            if(ICF_SUCCESS == 
                    icf_cfg_check_line_configured(p_crm_pdb, (icf_boolean_t)ICF_FALSE))
            {
                for(line_id = 0; (line_id < p_crm_pdb->p_config_data->max_lines) && (line_id < ICF_MAX_NUM_OF_LINES); 
                    line_id++)
                {
                    /* if secondary proxy is being modified on a line,
                     * it should already be present on that line, else 
                     * return FAILURE */
                    if (ICF_NULL == (ICF_CFG_LINE_DATA_SEC_PROXY_RECD & 
                                p_crm_pdb->p_line_data[line_id]->line_data_received))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                    ICF_CAUSE_SECONDARY_PROXY_NOT_CONFIGURED)
                        ret_val = ICF_SUCCESS;
                        break; /* break from for loop in case of failure */
                    }
                     /* If secondary proxy is being modified on a line,
                     * it should not be the same as primary proxy already 
                     * configured on that line */
                    else if (ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                                &(p_crm_pdb->p_line_data[line_id]->proxy_addr),
                                &(p_recv_msg_payload->sec_proxy_addr.transport_addr),
                                p_crm_pdb->p_glb_pdb))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                    ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_PROXY_ADDR)
                        ret_val = ICF_SUCCESS;
                        break; /* break from for loop in case of failure */ 
                    }  
                }/* end for loop for ALL_LINES */
            }/* end if ICF_SUCCESS == icf_cfg_check_line_in_db */
            else
            {
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ret_val = ICF_SUCCESS;
            }/* end if ICF_FAILURE == icf_cfg_check_line_in_db */
        }/* End of action MODIFY */
        else if (ICF_CONFIG_ACTION_DELETE == 
                p_recv_msg_payload->sec_proxy_addr.action)
        {
            if(ICF_SUCCESS == 
                    icf_cfg_check_line_configured(p_crm_pdb, (icf_boolean_t)ICF_FALSE))
            {
                for(line_id = 0; (line_id < p_crm_pdb->p_config_data->max_lines) && (line_id < ICF_MAX_NUM_OF_LINES); 
                        line_id++)
                {
                    /* if secondary proxy is being deleted from a line,
                     * it should already be present on that line, else 
                     * return FAILURE */
                    if (ICF_NULL == (ICF_CFG_LINE_DATA_SEC_PROXY_RECD & 
                                p_crm_pdb->p_line_data[line_id]->line_data_received))
                    {
                        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                    ICF_CAUSE_SECONDARY_PROXY_NOT_CONFIGURED)
                        ret_val = ICF_SUCCESS;
                        break; /* break from for loop in case of failure */
                    }
                }/* end for loop for ALL_LINES */
            }/* end if ICF_SUCCESS == icf_cfg_check_line_in_db */
            else
            {
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ret_val = ICF_SUCCESS;
            }/* end if ICF_FAILURE == icf_cfg_check_line_in_db */
        }/* End of action DELETE */
    }/* End else for all_lines */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_mode
 * 
 * DESCRIPTION:     This function will check for the self mode.If it is changed 
 *                  from BOTH/UDP->TCP or BOTH/TCP->UDP, it will update the  
 *                  mode in proxy/vms/registrar correspondingly.
 *                  
 * RETURNS:         ICF_SUCCESS.
 *
 ******************************************************************************/

icf_return_t icf_cfg_validate_mode(
        INOUT   icf_crm_pdb_st          *p_crm_pdb) 
{   
    icf_return_t		                   ret_val = ICF_SUCCESS;	            
    icf_line_id_t	                   line_id = 0;
    
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    if (ICF_SUCCESS == icf_cfg_check_line_configured(p_crm_pdb, 
                (icf_boolean_t)ICF_FALSE))
    {
        for(line_id = 0; line_id < ICF_MAX_NUM_OF_LINES; line_id++)
        {

            if(ICF_NULL != p_crm_pdb->p_line_data[line_id])
            {
                /*Check for the proxy over that line.If proxy is there, then check for            
                  the mode */

                if (p_crm_pdb->p_line_data[line_id]->line_data_received &
                        ICF_CFG_LINE_DATA_PROXY_RECD)
                {
                    if(p_crm_pdb->p_config_data->self_mode != 					
                            p_crm_pdb->p_line_data[line_id]->transport_mode)
                    {
                        p_crm_pdb->p_line_data[line_id]->transport_mode = 
                            p_crm_pdb->p_config_data->self_mode;
                    }
                }

                /* Check for the registrar over that line.If its there,check for that
                   mode wrt self_mode and change it to BOTH,if it is not as same as that          
                   of changed self_mode */
                if (p_crm_pdb->p_line_data[line_id]->line_data_received &			                	
                        ICF_CFG_LINE_DATA_REGISTRAR_RECD)	
                {
                    if(p_crm_pdb->p_config_data->self_mode !=	
                            p_crm_pdb->p_line_data[line_id]->reg_mode)
                    {
                        p_crm_pdb->p_line_data[line_id]->reg_mode = 
                            p_crm_pdb->p_config_data->self_mode;
                    }
                }
            }   /*End for the configured line*/
        }/*End of the loop*/
    }
    else /* FAILURE == icf_cfg_check_line_in_db */
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_LINE_ID)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }/* end else for FAILURE == icf_cfg_check_line_in_db */
    ret_val = ICF_SUCCESS;
    return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_update_self_trnspt_in_db
 * 
 * DESCRIPTION:     This function will update the self_transport data received 
 *                  in Set Transport params message into global database.
 *                  
 * RETURNS:         ICF_SUCCESS.
 *
 ******************************************************************************/

icf_return_t icf_cfg_update_self_trnspt_in_db(
        INOUT icf_crm_pdb_st                         *p_crm_pdb,
        IN    icf_transport_addr_st                  *p_transport_addr)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
          
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_TRNSPT_ADDR_RECD)

    icf_port_memcpy(
            (icf_void_t*)&(p_crm_pdb->p_config_data->self_ip_address.addr),
            (icf_void_t*)(p_transport_addr),
            (icf_uint32_t) ICF_PORT_SIZEOF(icf_transport_addr_st));

    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_TRNSPT_ADDR_SET)
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

    
    
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_trnspt_prms_in_db
 * 
 * DESCRIPTION:     This function will add the data received in Set Transport 
 *                  params message into global database.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case settting of proxy data returns failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_trnspt_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_boolean_t                        self_ip_addr_change = ICF_FALSE;
    icf_boolean_t                        self_mode_change = ICF_FALSE;
    icf_boolean_t                        change_mode = ICF_FALSE;
    icf_cfg_cc_set_transport_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_transport_mode_et              mode;
    /* flag to track whether the self IP in CFG data is all 0s */
    icf_boolean_t			              cfg_ip_all_null = ICF_FALSE;
    /* flag to track whether the self IP in recd msg payload is all 0s */
    icf_boolean_t			             msg_ip_all_null = ICF_FALSE; 
    icf_line_id_t                        temp_line_id = 0; 
    icf_boolean_t			             mem_alloc_for_prim_dns = ICF_FALSE;
    icf_boolean_t                        dhcp_query = ICF_FALSE;
    icf_line_id_t                        line_id = 0;
    /* CSR_1_7061845 Merged SPR 19672 Start */
    icf_call_id_t			 call_id_orig = ICF_INVALID_CALL_ID;
    /* CSR_1_7061845 Merged SPR 19672 End */

#ifdef ICF_NAT_RPORT_SUPPORT
    icf_rport_config_data_st             *p_rport_config_data = ICF_NULL;
#endif       
 
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_transport_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
       
    /*-----------------------------------------------------------------------*/
    /* Validate the data received */
    /*AMIT: If it is not first request then check the local flag
     *      for per line self transport configuration, validation failed
     *      if contradiction*/
    ret_val = icf_cfg_validate_trnspt_prms(p_crm_pdb);

    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    if (ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        /* Fill the line id from received message payload */
        if(ICF_NULL != p_recv_msg_payload)
        {
            if(ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_PROXY_ADDR_LIST &
                p_recv_msg_payload->bit_mask))
            {
                p_crm_pdb->line_id = p_recv_msg_payload->proxy_addr.line_id;
            }
            else if(ICF_CFG_SELF_ADDR_PER_LINE ==
                       p_crm_pdb->p_config_data->self_addr_per_line)
            {
                p_crm_pdb->line_id = p_recv_msg_payload->self_addr_per_line.line_id;
            }
        }
        p_crm_pdb->result = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /*-----------------------------------------------------------------------*/

    /* SPR 15997: Set the line id in crm_pdb as follows:
     * If proxy address is present then set line id as specified in 
     * proxy address.
     * Else is self transport address is present then set line id as 
     * specified in transport address.
     */
/*      ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_SELF_TRANSPORT_ADDR) */

    /* If primary DNS Server is present in the API
     * and it has not been allocated memory before,
     * allocate memory to it */
    if ((ICF_NULL != (ICF_SET_TRP_REQ_DNS_SERVER_ADDR & 
            p_recv_msg_payload->bit_mask)) && 
            (ICF_NULL == p_crm_pdb->p_config_data->p_dns_server_addr))
    {
        /* Error cause set and passed on in p_crm_pdb in case 
         * of memory allocation failure rather than using
         * ICF_RET_ON_FAILURE option while doing MEMGET */
        ICF_MEMGET(
                p_crm_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(icf_transport_address_st),
                ICF_MEM_COMMON,
                p_crm_pdb->p_config_data->p_dns_server_addr,
                ICF_DONOT_RET_ON_FAILURE,
                p_crm_pdb->p_ecode,
                ret_val)
        mem_alloc_for_prim_dns = ICF_TRUE;
    }
    if (ICF_FAILURE == ret_val)
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                *p_crm_pdb->p_ecode)
        ICF_CFG_SET_ECODE(p_crm_pdb,*p_crm_pdb->p_ecode)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /* If secondary DNS Server is present in the API
     * and it has not been allocated memory before,
     * allocate memory to it */
    if ((ICF_NULL != (ICF_SET_TRP_REQ_SEC_DNS_SERVER_ADDR & 
            p_recv_msg_payload->bit_mask)) && 
            (ICF_NULL == p_crm_pdb->p_config_data->p_sec_dns_server_addr))
    {
        /* Error cause set and passed on in p_crm_pdb in case 
         * of memory allocation failure rather than using
         * ICF_RET_ON_FAILURE option while doing MEMGET. Also 
         * in case of failure here, memory already allocated 
         * for primary DNS Server needs to be freed*/
        ICF_MEMGET(
                p_crm_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(icf_transport_address_st),
                ICF_MEM_COMMON,
                p_crm_pdb->p_config_data->p_sec_dns_server_addr,
                ICF_DONOT_RET_ON_FAILURE,
                p_crm_pdb->p_ecode,
                ret_val)
    }
    /* If memory allocation for secondary DNS Server fails and primary 
     * DNS Server was allocated memory i.e. primary DNS Server was not 
     * already configured but is being configured in the same API */
    if (ICF_FAILURE == ret_val)
    {
        if (ICF_TRUE == mem_alloc_for_prim_dns)
        {
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_crm_pdb->p_config_data->p_dns_server_addr,
                    ICF_MEM_COMMON,
                    p_crm_pdb->p_ecode,
                    ret_val)
        }
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                *p_crm_pdb->p_ecode)
        ICF_CFG_SET_ECODE(p_crm_pdb,*p_crm_pdb->p_ecode)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

     if(ICF_NULL != (ICF_SET_TRP_REQ_DNS_SERVER_ADDR & 
               p_recv_msg_payload->bit_mask))
     {
         /* No need to check return value of 
          * icf_cmn_copy_transport_address , as
          * both arguments passed to it will always
          * be non-NULL here */
         icf_cmn_copy_transport_address(
             p_crm_pdb->p_config_data->p_dns_server_addr,
             &(p_recv_msg_payload->dns_server_addr),
             p_crm_pdb->p_glb_pdb,p_crm_pdb->p_ecode);
         ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_DNS_SERVER_ADDR_RECD)
     }
     if (ICF_NULL != (ICF_SET_TRP_REQ_SEC_DNS_SERVER_ADDR & 
                 p_recv_msg_payload->bit_mask))
     {
         icf_cmn_copy_transport_address(
                 p_crm_pdb->p_config_data->p_sec_dns_server_addr,
                 &(p_recv_msg_payload->sec_dns_server_addr),
                 p_crm_pdb->p_glb_pdb,p_crm_pdb->p_ecode);
         ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SEC_DNS_SERVER_ADDR_RECD)
     }
 
     if (ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_PROXY_ADDR_LIST & 
                 p_recv_msg_payload->bit_mask))
     {
         p_crm_pdb->line_id = p_recv_msg_payload->proxy_addr.line_id;
     }
     
     if(ICF_CFG_SELF_ADDR_PER_LINE == 
             p_crm_pdb->p_config_data->self_addr_per_line)
     {
         p_crm_pdb->line_id = p_recv_msg_payload->self_addr_per_line.line_id;
     }
     temp_line_id = p_crm_pdb->line_id;
    /*Self transport received per line*/
    if (ICF_CFG_SELF_ADDR_PER_LINE == 
        p_crm_pdb->p_config_data->self_addr_per_line)
    {
         ret_val = icf_cfg_set_self_addr_in_db_per_line(p_crm_pdb);
    }
    else
    {
	    /* Set the self transport data received */
    	if (ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_SLF_TRANSPORT_ADDRESS &
				    p_recv_msg_payload->bit_mask))
        {
            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_TRNSPT_PRMS_REQ_RECD)

            /* If init complete is not done, update the Database */
            if(ICF_FALSE == p_crm_pdb->global_data_init_complete)
            {
                ret_val = icf_cfg_update_self_trnspt_in_db(p_crm_pdb, 
                        &(p_recv_msg_payload->self_transport_address));
            }

            /* if init complete is done */
            else if( 
                    (p_crm_pdb->p_config_data->self_ip_address.addr.addr.
					     ipv4_addr.octet_1 != 
					     p_recv_msg_payload->self_transport_address.addr.
					     ipv4_addr.octet_1) ||
					    (p_crm_pdb->p_config_data->self_ip_address.addr.addr.
					     ipv4_addr.octet_2 != 
					     p_recv_msg_payload->self_transport_address.addr.
					     ipv4_addr.octet_2) ||
					    (p_crm_pdb->p_config_data->self_ip_address.addr.addr.
					     ipv4_addr.octet_3 != 
					     p_recv_msg_payload->self_transport_address.addr.
					     ipv4_addr.octet_3) ||
					    (p_crm_pdb->p_config_data->self_ip_address.addr.addr.
					     ipv4_addr.octet_4 != 
					     p_recv_msg_payload->self_transport_address.addr.
					     ipv4_addr.octet_4)
				    )
            {
				    /* Check if transport params received in API is different 
				     * from existing IP, then set flag to indicate there is a 
				     * change in IP address. */

				    self_ip_addr_change = ICF_TRUE;
                    /* Fix for CSR-1-5278638 */
                    if ((0 == p_crm_pdb->p_config_data->self_ip_address.addr.\
								addr.ipv4_addr.octet_1) &&
						(0 == p_crm_pdb->p_config_data->self_ip_address.addr.\
								addr.ipv4_addr.octet_2) &&
						(0 == p_crm_pdb->p_config_data->self_ip_address.addr.\
								addr.ipv4_addr.octet_3) &&
						(0 == p_crm_pdb->p_config_data->self_ip_address.addr.\
								addr.ipv4_addr.octet_4))
					{
						cfg_ip_all_null = ICF_TRUE;
					}

					if ((0 == p_recv_msg_payload->self_transport_address.\
								addr.ipv4_addr.octet_1) &&
						(0 == p_recv_msg_payload->self_transport_address.\
								addr.ipv4_addr.octet_2) &&
						(0 == p_recv_msg_payload->self_transport_address.\
								addr.ipv4_addr.octet_3) &&
						(0 == p_recv_msg_payload->self_transport_address.\
								addr.ipv4_addr.octet_4))
					{
						msg_ip_all_null = ICF_TRUE;
					}
            }/* end else if */
	    }

	    /* Set the self_port */
	    if(ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_SLF_PORT & 
				    p_recv_msg_payload->bit_mask))
	    {
		    /* If init complete is not done then update the data base */
		    if(ICF_FALSE == p_crm_pdb->global_data_init_complete)
		    {
			    p_crm_pdb->p_config_data->self_ip_address.port_num = 
				    p_recv_msg_payload->self_port;
			    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_TRNSPT_MODE_SET)
		    }

		    /* Else check if port is diff from the  existing port in the
		     * config data then the processing will be same as done when ip addess 
		     * is changed
		     */
		    else if (p_crm_pdb->p_config_data->self_ip_address.port_num != 
				    p_recv_msg_payload->self_port)
		    {
			    self_ip_addr_change = ICF_TRUE;
		    }
	    }

	    /* Set the self transport mode received */
	    if(ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_SLF_MODE &
				    p_recv_msg_payload->bit_mask))
	    {
		    /*This conditions checks the change of self_mode from BOTH/UDP->TCP or            
		      BOTH/TCP->UDP.If that is there then there has to be check over the
		      modes of proxy/registrar/vms*/

            /* Adding extra check to make sure self mode change from UDP/TCP ->
			 * BOTH does not affect proxy_mode
			 */
		    if (((p_crm_pdb->p_config_data->self_mode == 
					ICF_TRANSPORT_MODE_BOTH) && 
				 (p_recv_msg_payload->self_mode != 
					ICF_TRANSPORT_MODE_BOTH)) 
				|| 
				((p_crm_pdb->p_config_data->self_mode != 
					ICF_TRANSPORT_MODE_BOTH) && 
				 (p_recv_msg_payload->self_mode !=
					ICF_TRANSPORT_MODE_BOTH) && 
				 (p_recv_msg_payload->self_mode != 
                                          p_crm_pdb->p_config_data->self_mode)))  
		    {
			    change_mode = ICF_TRUE;
                ICF_PRINT(((icf_uint8_t*)"\n[CFG]: Suspect mode change for outgoing conn\n"));
		    }			


		    if(ICF_FALSE == p_crm_pdb->global_data_init_complete)
		    {
			    /* Save the self mode in the config data structure */
			    p_crm_pdb->p_config_data->self_mode = p_recv_msg_payload->self_mode;
			    if(ICF_TRUE == change_mode)
			    {
				    ret_val = icf_cfg_validate_mode(p_crm_pdb);    
			    }

			    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_TRNSPT_MODE_SET)
		    }
		    /* If mode has changed to both */
		    else if(
                ((ICF_TRANSPORT_TYPE_UDP == 
                  p_crm_pdb->p_config_data->self_mode) || 
                 (ICF_TRANSPORT_TYPE_TCP == 
                  p_crm_pdb->p_config_data->self_mode)) && 
                (ICF_TRANSPORT_MODE_BOTH == 
                 p_recv_msg_payload->self_mode))
            {
			    /* Set local variable to the mode value to be sent to SSA */
			    if(ICF_TRANSPORT_TYPE_UDP == p_crm_pdb->p_config_data->self_mode)
			    {
				    mode = ICF_TRANSPORT_TYPE_TCP;
			    }
			    else
			    {
				    mode = ICF_TRANSPORT_TYPE_UDP;
			    }

			    /* Invoke SSA to open new ports */
			    if (ICF_FAILURE == icf_cfg_open_nw_interface(
						    p_crm_pdb, mode))
			    {
				    ret_val = ICF_FAILURE;
				    ICF_CFG_SET_RESPONSE(
						    p_crm_pdb,
						    ICF_FAILURE,
						    p_crm_pdb->error_cause)
					    ICF_CFG_SET_ECODE(p_crm_pdb,ICF_ERROR_NW_INTERFACE)
			    }
			    else
			    {
				    /* Update the mode in the Database */
				    p_crm_pdb->p_config_data->self_mode = 
					    p_recv_msg_payload->self_mode;
				    ICF_CFG_TRACE(p_crm_pdb, 
						    ICF_CFG_TRACE_SELF_TRNSPT_MODE_SET)
			    }
		    }

		    /* if mode is not changed to both but there is some other change 
		     * then calls will have to be cleared and current sockets will
		     * have to closed and then new sockets will be opened
         */
        else if (p_crm_pdb->p_config_data->self_mode != 
                p_recv_msg_payload->self_mode)
        {
            /*SPR 16019: Save the new moe in effective self mode
             */
            p_crm_pdb->effective_self_mode = p_recv_msg_payload->self_mode;

            /* Set flag to indicate that mode has been changed */
            self_mode_change = ICF_TRUE;
        }
    }
        ICF_PRINT(((icf_uint8_t*)"\n\n[CFG]: Detect self IP or mode change:\n"));
	ICF_PRINT(((icf_uint8_t*)"[CFG]: Self IP address is %s changing\n", 
						self_ip_addr_change?"":"not"));
	ICF_PRINT(((icf_uint8_t*)"[CFG]: Self transport mode is %s changing\n", 
						self_mode_change?"":"not"));
	ICF_PRINT(((icf_uint8_t*)"[CFG]: Currently stored self IP is %s 0.0.0.0\n", 
						cfg_ip_all_null?"":"not"));
	ICF_PRINT(((icf_uint8_t*)"[CFG]: App-request payload self IP is %s 0.0.0.0\n", 
						msg_ip_all_null?"":"not"));

    if(ICF_TRUE == self_ip_addr_change || ICF_TRUE == self_mode_change)
    {
        /* CSR_1_7061845 Merged SPR 19672 Start */
        /* Saving original call_id received in APP config request */
        /* In one of the customer scenarios, it is noticed that the call_id sent
         * by IPTK to APP in ICF_CONFIGURE_CFM for Self IP Change is wrong. This
         * contains the call_id of active call cleared by IPTK while processing 
         * Self IP Change request, instead of having the call_id sent by APP in
         * ICF_SET_TRANSPORT_REQ API. Though this is not seen at our end, following
         * code is being added as a workaround to prevent the issue.
         */
        call_id_orig = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
        /* CSR_1_7061845 Merged SPR 19672 End */

        /* If the self IP address was 0.0.0.0, then it means IPTk was not 
	 * listening on network interface. So there is nothing to be cleared.
	 */
	if (ICF_FALSE == cfg_ip_all_null)
	{
        /* this flag will be used by REGM to forcefully clear contexts */
        p_crm_pdb->forceful_clear = ICF_TRUE;

        /* set the trigger module as CFG */
        p_crm_pdb->cm_trigger_module = ICF_INT_MODULE_CFG;

        /* Send call clear indication to CM */
        ret_val = icf_cfg_cm_call_clear_ind(p_crm_pdb);
        /* Send deregister request to REGM */
        ret_val = icf_rgm_forceful_dereg_all_users(p_crm_pdb);
        /* Invoke SSA to close all connections */
        ret_val = icf_cfg_close_nw_interface(p_crm_pdb);
	}
        /* Fix for CSR 1-1-5337328 */
	/* Transport modes should be updated only when there is a mode change.
	 * It should not be done if only self IP is changing.
	 */
	if (ICF_TRUE == self_mode_change)
	{
        /* SPR 16019: Update Mode*/
        p_crm_pdb->p_config_data->self_mode = p_crm_pdb->effective_self_mode;
        /*If self_mode is changed then the corresponding modes in proxy/regi
          strar/vms has to be updated */
        ret_val = icf_cfg_validate_mode(p_crm_pdb);
        }
        /* Update data in DB */
        if (ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_SLF_TRANSPORT_ADDRESS &
                    p_recv_msg_payload->bit_mask))
        {
            ret_val = icf_cfg_update_self_trnspt_in_db(
                    p_crm_pdb, 
                    &(p_recv_msg_payload->self_transport_address));
        }
        if(ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_SLF_MODE &
                    p_recv_msg_payload->bit_mask))
        {
            /* Save the self mode in the config data structure */
            p_crm_pdb->p_config_data->self_mode = p_recv_msg_payload->self_mode;
            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_TRNSPT_MODE_SET)
        }
        /* Set the self_port */
        if(ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_SLF_PORT & 
                    p_recv_msg_payload->bit_mask))
        {
            p_crm_pdb->p_config_data->self_ip_address.port_num = 
                p_recv_msg_payload->self_port;
            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_TRNSPT_MODE_SET)
        }
        /* If the newly received self IP is 0.0.0.0, it means IPTk needs to
          * close all the sockets and not open any new ones.
	  */

        /* Trigger SSA to open sockets, */
        if ((ICF_FALSE == msg_ip_all_null) &&
	    (ICF_FAILURE == icf_cfg_open_nw_interface(
             p_crm_pdb, p_crm_pdb->p_config_data->self_mode)))
        {
            ret_val = ICF_FAILURE;
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    p_crm_pdb->error_cause)
                                                         
            ICF_CFG_SET_ECODE(p_crm_pdb,ICF_ERROR_NW_INTERFACE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }
        if(ICF_TRUE == self_ip_addr_change)
        {
#ifndef ICF_FEATURE_ABANDON/* For ICF features not supported in ICF */
            /* Send Register request to REGM */
            if(ICF_FAILURE ==  icf_rgm_init_complete(p_crm_pdb))
            {
                ICF_CFG_SET_ECODE(p_crm_pdb,ICF_ERROR_RGM_INIT_CMPLT)
            }
            /* Send subscribe request to MWI for all users */
            if(ICF_FAILURE == icf_cfg_mwi_trigger(p_crm_pdb, 
                    (icf_uint8_t)ICF_CFG_SUBSCRIBE_ALL))
            {
                ICF_CFG_SET_ECODE(p_crm_pdb,ICF_ERROR_MWI_INIT_CMPLT)
            }
#endif /* endif: ICF_FEATURE_ABANDON For ICF features not supported in ICF */
        }
        /* CSR_1_7061845 Merged SPR 19672 Start */
        /* restoring original call_id received in APP config request */
        p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id = call_id_orig;
        /* CSR_1_7061845 Merged SPR 19672 End */
    }
    }

     /* Set the proxy data received. this is done after setting self transport 
     * parameters, so that we don't have to revert the proxies added in case of
     * failure in other set trnspt prms */
    if (ICF_NULL != (ICF_CFG_CC_SET_TRP_PRM_PROXY_ADDR_LIST & 
                p_recv_msg_payload->bit_mask))
    {
        ret_val = icf_cfg_set_trnspt_proxy_addr(p_crm_pdb);

    }
    /* Secondary proxy being configured on line after 
     * primary configuration, if both are present in API */
    if(ICF_NULL != (ICF_SET_TRP_REQ_SEC_PROXY_ADDR & 
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->line_id = p_recv_msg_payload->sec_proxy_addr.line_id;
        if (ICF_FAILURE == icf_cfg_set_trnspt_sec_proxy_addr(p_crm_pdb))
        {
            p_crm_pdb->result = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }
     }

/*Removing the run time check for operation mode(IMS or NON_IMS) from here.
  So as to make it work for both IMS and NON-IMS mode*/
#ifdef NON_IMS_CLIENT
    /* Temporary Fix for ICF NON IMS RELASE 4.3, This will be resolved on 5.0*/
    /* If init complete is not done, and Self IP and Port related configuration has
     * beent received from the Application then open the network channel and
     * this will be treated as Init complete towards ICF*/
      if (ICF_CFG_SELF_ADDR_SYS_WIDE ==
			p_crm_pdb->p_config_data->self_addr_per_line)
	{
		if(ICF_FALSE == p_crm_pdb->p_glb_pdb->p_glb_data->
				global_data_init_complete)
		{
			if((ICF_TRUE == ICF_IS_BIT_SET(p_recv_msg_payload->bit_mask,
							ICF_CFG_CC_SET_TRP_PRM_SLF_TRANSPORT_ADDRESS)) &&
					(ICF_TRUE == ICF_IS_BIT_SET(p_recv_msg_payload->bit_mask,
								    ICF_CFG_CC_SET_TRP_PRM_SLF_MODE)) &&
					(ICF_TRUE == ICF_IS_BIT_SET(p_recv_msg_payload->bit_mask,
								    ICF_CFG_CC_SET_TRP_PRM_SLF_PORT)))
			{
				icf_cfg_check_init_complete(p_crm_pdb);
				if(ICF_TRUE == p_crm_pdb->is_valid_param)
				{
					ret_val = icf_cfg_init_complete_hdlr(p_crm_pdb);
					if(ICF_FAILURE == ret_val)
					{
						p_crm_pdb->result = ICF_FAILURE;
					}
					p_crm_pdb->resp_to_be_sent = ICF_TRUE;
				}
			}
		}
	}
    else if (ICF_CFG_SELF_ADDR_PER_LINE == 
				p_crm_pdb->p_config_data->self_addr_per_line)
	{
		icf_cfg_check_init_complete_for_line(p_crm_pdb);
		if(ICF_TRUE == p_crm_pdb->is_valid_param)
         {
            ret_val = icf_cfg_init_complete_hdlr_for_line(p_crm_pdb);

            if(ICF_FAILURE == ret_val)
            {
               p_crm_pdb->result = ICF_FAILURE;
            }
            p_crm_pdb->resp_to_be_sent = ICF_TRUE;
         }
     }
#endif

#ifdef ICF_NAT_RPORT_SUPPORT

    if (ICF_CFG_SELF_ADDR_PER_LINE !=
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
        p_rport_config_data = &(p_crm_pdb->p_config_data->rport_config_data);
    }

    if (ICF_NULL == p_rport_config_data)
    {
       ret_val = ICF_FAILURE;   
    } 
    else 
    {
        /*  if rport values are still not received only 
            then copy the self into contact other wise 
            dont overwrite the contact because it already
            contatins the rport IP and Address. Fix for 
            CSR 1-5334755.This is the fix not the merge 
            because the code base for 6.2 is very different 
            from MAIN line*/
        if (ICF_FALSE == p_rport_config_data->is_rport_completed)
        {
            icf_port_memcpy((icf_void_t*)&(p_rport_config_data->contact_ip_addr),\
                    (icf_void_t*)&(p_crm_pdb->p_config_data->\
                self_ip_address),
        		(icf_uint32_t) ICF_PORT_SIZEOF(icf_transport_address_st));
    
            p_rport_config_data->contact_ip_addr.port_num\
                = p_crm_pdb->p_config_data->self_ip_address.port_num;
        }
    }

#endif
    if(ICF_ALL_LINES == temp_line_id)
    {
        p_crm_pdb->line_id  = ICF_ALL_LINES;
    }
    ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_SELF_TRANSPORT_ADDR)
     /* if application has set this bit then application wants to
     * configure dns_port from where DNS queries will be issued.
     */
    if (ICF_SET_TRP_REQ_DNS_SRC_PORT & p_recv_msg_payload->bit_mask)
    {
        p_crm_pdb->p_config_data->dns_configuration |= \
                                 ICF_DNS_PORT_CONFIGURED_BY_APP;

        p_crm_pdb->p_config_data->dns_source_port = \
                                 p_recv_msg_payload->dns_source_port;
    }
    if(ICF_NULL != (ICF_SET_TRP_REQ_DHCP_SERVER_ADDR & 
			p_recv_msg_payload->bit_mask))
    {
        if(ICF_NULL == p_crm_pdb->p_config_data->p_dhcp_server_addr)
        {
	    /* Allocate memory for DHCP Server address */
            ICF_MEMGET(
                p_crm_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(icf_transport_address_st),
                ICF_MEM_COMMON,
                p_crm_pdb->p_config_data->p_dhcp_server_addr,
                ICF_DONOT_RET_ON_FAILURE,
                p_crm_pdb->p_ecode,
                ret_val)
            if (ICF_FAILURE == ret_val)
            {
                ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INSUFFICIENT_RESOURCES)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
    
            }
        }

        icf_port_memcpy(
                    (icf_void_t*) (p_crm_pdb->p_config_data->p_dhcp_server_addr),
                    (icf_void_t*) &(p_recv_msg_payload->dhcp_server_addr),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));
    } 
    /* Update DHCP listening port in config_data */
    /* If present in set_transport_req, update with this value */
    if(ICF_NULL != (ICF_SET_TRP_REQ_DHCP_CLIENT_PORT &
                    p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->dhcp_client_port = \
           p_recv_msg_payload->dhcp_client_port;
    }

    /*If DHCP server address is configured and DHCP query is not done yet
     *then check if self address is configured as per line or system wide then mark
     *dhcp_query as TRUE to send DHCP query on netwotk*/
    if ((ICF_NULL != p_crm_pdb->p_config_data->p_dhcp_server_addr) &&
        (ICF_NULL == p_crm_pdb->p_config_data->p_dhcp_resp))
    {
        /*If self address is configured as per line then fetch address from line_data
         *otherwise fetch address from config_data, fetching of address will be done
         *in handle_dhcp_query, here only identified that DHCP query can be done or not*/
        if (ICF_CFG_SELF_ADDR_PER_LINE != 
               p_crm_pdb->p_config_data->self_addr_per_line)
        {
            if ((ICF_TRANSPORT_ADDRESS_INVALID !=
                    p_crm_pdb->p_config_data->self_ip_address.addr.addr_type))
            {
                dhcp_query = ICF_TRUE;
            }
        }
        else
        {
            for (line_id = 0; (line_id < p_crm_pdb->p_config_data->max_lines) && (line_id < ICF_MAX_NUM_OF_LINES); line_id++)
            {
                if (ICF_SUCCESS == icf_cfg_check_line_configured(
                               p_crm_pdb,
                               (icf_boolean_t)ICF_FALSE))
                {
                   if (ICF_NULL != (ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD &
                          p_crm_pdb->p_line_data[line_id]->line_data_received)) 
                   {
                       dhcp_query = ICF_TRUE;
                       p_crm_pdb->line_id = line_id;
                       break;
                   }
                }
            }
        }
    }
    if (ICF_TRUE == dhcp_query)
    {
        /*Send DHCP query to and free DHCP server address in case of
         *any failure occurs or failure response is received from DHCP server*/
        if (ICF_FAILURE == icf_cfg_handle_dhcp_query(p_crm_pdb))
        {
            /* DHCP query failed  delete DHCP server addr from config_data */
            ICF_MEMFREE(
                       p_crm_pdb->p_glb_pdb,
                       p_crm_pdb->p_config_data->p_dhcp_server_addr,
                       ICF_MEM_COMMON,
                       p_crm_pdb->p_ecode,
                       ret_val)
             p_crm_pdb->p_config_data->p_dhcp_server_addr = ICF_NULL;
             ret_val = ICF_FAILURE;
        }
        else
        {
            ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_DISCOVERED_PROXY_ADDRESS_FROM_DHCP)
        }
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_add_proxy_prms_in_db
 * 
 * DESCRIPTION:     This function adds the proxy address received in 
 *                  the API ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 *****************************************************************************/
icf_return_t icf_cfg_add_proxy_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_transport_params_req_st *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_transport_params_req_st *)
    (p_crm_pdb->p_recv_payload);

    /* Fix for SPR 17007: Set transport mode as self mode if both else
     * whatever received.
     */
    /*If transport mode is both or transport mode is invalid and 
     *application has configured proxy as IP address then copy self mode
     *to transport mode*/
    if ((ICF_TRANSPORT_MODE_BOTH == p_recv_msg_payload->
            proxy_addr.transport_mode) ||
        ((ICF_TRANSPORT_TYPE_INVALID == p_recv_msg_payload->
            proxy_addr.transport_mode) &&
         (ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
          p_recv_msg_payload->proxy_addr.transport_addr.addr.addr_type)))
    {
        if (ICF_CFG_SELF_ADDR_PER_LINE !=
                         p_crm_pdb->p_config_data->self_addr_per_line)
        {
            p_crm_pdb->p_line_data[line_id]->transport_mode = p_crm_pdb->
                p_config_data->self_mode;
        }
        else
        {
            p_crm_pdb->p_line_data[line_id]->transport_mode = 
                p_crm_pdb->p_line_data[line_id]->self_mode;
        }
    }
    else
    {
        p_crm_pdb->p_line_data[line_id]->transport_mode = 
            p_recv_msg_payload->proxy_addr.transport_mode;
    }
    /* Fix for SPR 17007 ends */

    icf_cmn_copy_transport_address(
                &(p_crm_pdb->p_line_data[line_id]->proxy_addr),
                &(p_recv_msg_payload->proxy_addr.transport_addr),
                p_crm_pdb->p_glb_pdb,p_crm_pdb->p_ecode);
        /* set the presence flag to indicate that proxy is received on the line */
        ICF_CFG_SET_BITMASK(p_crm_pdb->p_line_data[line_id]->
            line_data_received, ICF_CFG_LINE_DATA_PROXY_RECD)
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
                    
}

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_add_sec_proxy_prms_in_db
 * 
 * DESCRIPTION:     This function adds the secopndary proxy address received in 
 *                  the API ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 *****************************************************************************/
icf_return_t icf_cfg_add_sec_proxy_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t     line_id)
{
    icf_return_t                            ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_transport_params_req_st *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Allocate memory for secondary proxy address in line_data
     * only if not allocated already to avoid memleak */
    if (ICF_NULL == p_crm_pdb->p_line_data[line_id]->p_sec_proxy_addr)
    {
         /* Error cause set and passed on in p_crm_pdb in case 
         * of memory allocation failure rather than using
         * ICF_RET_ON_FAILURE option while doing MEMGET */
        ICF_MEMGET(
            p_crm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_transport_address_st),
            ICF_MEM_COMMON,
            p_crm_pdb->p_line_data[line_id]->p_sec_proxy_addr,
            ICF_DONOT_RET_ON_FAILURE,
            p_crm_pdb->p_ecode,
            ret_val)
    }
    if (ICF_FAILURE == ret_val)
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                *p_crm_pdb->p_ecode)
        ICF_CFG_SET_ECODE(p_crm_pdb,*p_crm_pdb->p_ecode)
    }
    else
    {
        /* Copy the external message payload pointer locally */
        p_recv_msg_payload = (icf_cfg_cc_set_transport_params_req_st *)
            (p_crm_pdb->p_recv_payload);

        /* Transport mode already set in line_data from primary proxy configuration.
           The same transport mode will be used for secondary proxy server as well*/

         icf_cmn_copy_transport_address (
                    p_crm_pdb->p_line_data[line_id]->p_sec_proxy_addr,
                    &(p_recv_msg_payload->sec_proxy_addr.transport_addr),
                    p_crm_pdb->p_glb_pdb,p_crm_pdb->p_ecode);
            /* set the presence flag to indicate that secondary proxy is received 
               on the line */
            ICF_CFG_SET_BITMASK(p_crm_pdb->p_line_data[line_id]->
                        line_data_received, ICF_CFG_LINE_DATA_SEC_PROXY_RECD)
   }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;                  
}

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_del_proxy_prms_in_db
 * 
 * DESCRIPTION:     This function deletes the proxy address received in 
 *                  the API ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_del_proxy_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    p_crm_pdb->line_id = line_id;

    /* set the trigger module as CFG */
    p_crm_pdb->cm_trigger_module = ICF_INT_MODULE_CFG;
    icf_cfg_cm_call_clear_ind_wrt_line(p_crm_pdb);

    p_crm_pdb->p_line_data[line_id]->transport_mode = 
                                          ICF_TRANSPORT_MODE_UDP;

    icf_cmn_init_transport_address(
            &(p_crm_pdb->p_line_data[line_id]->proxy_addr));

    /* reset the presence flag to indicate that proxy is deleted on the line */
    ICF_CFG_RESET_BITMASK(p_crm_pdb->p_line_data[line_id]->
            line_data_received, ICF_CFG_LINE_DATA_PROXY_RECD)

    icf_cfg_delete_line(p_crm_pdb, line_id);

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}        

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_del_sec_proxy_prms_in_db
 * 
 * DESCRIPTION:     This function deletes secondary proxy address received in 
 *                  the API ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_del_sec_proxy_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    p_crm_pdb->line_id = line_id;
    /* Clear call if secondary proxy is active on the line */
    if (ICF_FALSE != p_crm_pdb->p_line_data[line_id]->sec_proxy_server_active)
    {
        /* set the trigger module as CFG */
        p_crm_pdb->cm_trigger_module = ICF_INT_MODULE_CFG;

        icf_cfg_cm_call_clear_ind_wrt_line(p_crm_pdb);
    }

    ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_crm_pdb->p_line_data[line_id]->p_sec_proxy_addr,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)

    /* Reset the presence flag to indicate that secondary proxy has been 
       deleted on the line */
    ICF_CFG_RESET_BITMASK(p_crm_pdb->p_line_data[line_id]->
            line_data_received, ICF_CFG_LINE_DATA_SEC_PROXY_RECD)

    /* The same transport mode is used as is set in primary registrar 
     * configuration, thus no need to update transport mode*/

    /*Now delete line if all other data is already deleted*/
    icf_cfg_delete_line(p_crm_pdb, line_id);

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_trnspt_proxy_addr
 * 
 * DESCRIPTION:     This function will add the proxy address received in Set 
 *                  Transport params message into global database.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_trnspt_proxy_addr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_transport_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_line_id_t                       line_index = 0;
    icf_line_id_t                       line_id = 0;
    icf_uint8_t                         line_status[ICF_MAX_NUM_OF_LINES];

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_transport_params_req_st *)
        (p_crm_pdb->p_recv_payload);
    
    
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_PROXY_ADDR_REQ_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_PROXY_ADDR_LINE_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_PROXY_ADDR_RECD)

    ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_PROXY_INFO)

    if(ICF_ALL_LINES == p_recv_msg_payload->proxy_addr.line_id)
    {
        /* This means that params will be set on all lines i.e. 
         * if a line is not already configured then all lines 
         * upto max_lines in config data will be configured.
         */

        if(ICF_SUCCESS == icf_cfg_check_line_configured(
                            p_crm_pdb,
                            (icf_boolean_t)ICF_TRUE))
        {
            /* Instead of checking p_crm_pdb->lines_present[line_index]
             * icf_cfg_check_line_in_db is being called, because when 
             * primary registrar is configured on ALL_LINES for the first 
             * time, no line is present and registrar would not get 
             * configured at all.
             * 
             * icf_cfg_check_line_in_db is called with last arg. as 
             * ICF_TRUE, so that if the line doesnot exist already 
             * it is now configured */

            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                        max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {                
                if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                        proxy_addr.action)
                {
                    ret_val = icf_cfg_add_proxy_prms_in_db(
                                    p_crm_pdb,
                                    line_index);
                }/* End if action = add ..*/
                
                /* If action is delete, then Check if proxy is present, 
                 * then delete else ignore */

                else if((ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                            proxy_addr.action) && 
                        (ICF_NULL != (p_crm_pdb->p_line_data[line_index]->
                                         line_data_received & 
                                         ICF_CFG_LINE_DATA_PROXY_RECD)))
                {
                    ret_val = icf_cfg_del_proxy_prms_in_db(p_crm_pdb,
                            line_index);

                }/* End if action = delete and proxy is present */
                
                /* If action is modify, Check if proxy is present, 
                 * then modify else ignore */

                else if((ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->
                            proxy_addr.action) && 
                        (ICF_NULL != (p_crm_pdb->p_line_data[line_index]->
                                         line_data_received & 
                                         ICF_CFG_LINE_DATA_PROXY_RECD)))
                {
                    ret_val = icf_cfg_add_proxy_prms_in_db(p_crm_pdb,
                            line_index);

                }/* End if action = modify ..*/
            }/* end loop */
            p_crm_pdb->line_id = p_recv_msg_payload->proxy_addr.line_id;
            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_PROXY_ADDR_LINE_SET)
            p_crm_pdb->line_id = p_recv_msg_payload->proxy_addr.line_id;
            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_PROXY_ADDRESS_SET)
        }/* End if lines configured */
    }/* End if for all_lines */
    else
    {
        /* If all lines field is not true then the proxy address will be set on 
         * the line id requested. */
        
        line_id = p_recv_msg_payload->proxy_addr.line_id;
       
       /* Line data pointer has already been retrieved during validation */ 
        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->proxy_addr.
                action)
        {
            /* Success means line has been allocated */
            ret_val = icf_cfg_add_proxy_prms_in_db(p_crm_pdb, line_id);
        }
        else /* Action is modify/delete */
        {
            if((ICF_SUCCESS == icf_cfg_check_line_in_db(p_crm_pdb, 
                        line_id, line_status, (icf_boolean_t)ICF_FALSE)) && 
						(line_id < ICF_MAX_NUM_OF_LINES))
            {
                /* Action is delete/modify, only fetch line pointer.
                 * Here, failure will mean line does not exist.*/
                if(ICF_NULL != (p_crm_pdb->p_line_data[line_id]->
                            line_data_received & 
                            ICF_CFG_LINE_DATA_PROXY_RECD))
                {
                    /* If proxy exists, means we can now perform delete/modify 
                     * action accordingly or request will be ignored.
                     */
                    if(ICF_CONFIG_ACTION_DELETE ==  p_recv_msg_payload->
                            proxy_addr.action)
                    {
                        ret_val = icf_cfg_del_proxy_prms_in_db(p_crm_pdb, 
                                line_id);
                    }
                    else /* Action is modify */
                    {
                        ret_val = icf_cfg_add_proxy_prms_in_db(p_crm_pdb, 
                                line_id);

                    }/* End else */
                }/* End if(ICF_SUCCESS == ..*/
            }/* End if ICF_SUCCESS */
        }/* End else if action is delete/modify */
        p_crm_pdb->line_id = p_recv_msg_payload->proxy_addr.line_id;
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_PROXY_ADDR_LINE_SET)
        p_crm_pdb->line_id = p_recv_msg_payload->proxy_addr.line_id;
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_PROXY_ADDRESS_SET)
    }/* End else for all_lines */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_trnspt_sec_proxy_addr
 * 
 * DESCRIPTION:     This function will add secondary proxy address received in  
 *                  Set Transport params message into global database.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_trnspt_sec_proxy_addr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                            ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_transport_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_line_id_t                           line_index = 0;
    icf_uint8_t                             line_status[ICF_MAX_NUM_OF_LINES];
    icf_line_id_t                           line_id = 0;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_transport_params_req_st *)
        (p_crm_pdb->p_recv_payload);
    
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SEC_PROXY_ADDR_REQ_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SEC_PROXY_ADDR_RECD)

    ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_SEC_PROXY_INFO)

    if(ICF_ALL_LINES == p_recv_msg_payload->sec_proxy_addr.line_id)
    {
        /* This means that params will be set on all the lines 
         * configured till now. i.e. all the lines on which we have 
         * either proxy or registrar or atleast one user. 
         */
        /* Line pointers have already been fetched during validation.*/
        
        if(ICF_SUCCESS == icf_cfg_check_line_configured(
                            p_crm_pdb,
                            (icf_boolean_t)ICF_TRUE))
        {
            for(line_index = 0; line_index < (p_crm_pdb->p_config_data->
                        max_lines); line_index++)
            {
                /* If action is ADD for secondary proxy server */
                if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                        sec_proxy_addr.action)
                {
                    ret_val = icf_cfg_add_sec_proxy_prms_in_db(
                                    p_crm_pdb, line_index);
                }/* End if action = add */
                
                /* If action is delete */
                else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                            sec_proxy_addr.action)
                {
                    /* Validations have already been made, so secondary proxy
                       must be present */
                    ret_val = icf_cfg_del_sec_proxy_prms_in_db(p_crm_pdb,
                            line_index);
                }/* End if action = delete */
                
                /* If action is modify */
                else if(ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->
                            sec_proxy_addr.action)
                {
                    /* Validations have already been done, so secondary proxy
                       must be present already */
                    ret_val = icf_cfg_add_sec_proxy_prms_in_db(p_crm_pdb,
                            line_index);
                }/* End if action = modify ..*/
            }/* end loop */
            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SEC_PROXY_ADDRESS_SET)
        }/* End if line configured in db */
    }/* End if for all_lines */
    else
    {
        /* If all lines field is not true then the proxy address will be set on 
         * the line id requested. */
        
        line_id = p_recv_msg_payload->sec_proxy_addr.line_id;
       
       /* Line data pointer has already been retrieved during validation */ 
        if(ICF_FAILURE == icf_cfg_check_line_in_db(
                p_crm_pdb,
                line_id,
                line_status,
                (icf_boolean_t)ICF_FALSE))
        {
            ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_LINE_ID)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        }
        /* If action is ADD for secondary proxy */
        else if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->sec_proxy_addr.
                action)
        {
            ret_val = icf_cfg_add_sec_proxy_prms_in_db(p_crm_pdb, line_id);
        }
        /* else if action is DELETE for secondary proxy */
        else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                            sec_proxy_addr.action)
        {
            /* Validations have already been made, so secondary proxy
               must be present */
            ret_val = icf_cfg_del_sec_proxy_prms_in_db(p_crm_pdb,
                      line_id);
        }/* End if action = delete */
            
        /* If action is MODIFY for secondary proxy */
        else if(ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->
                    sec_proxy_addr.action)
        {
           /* Validations have already been done, so secondary proxy
               must be present already */
            ret_val = icf_cfg_add_sec_proxy_prms_in_db(p_crm_pdb,
                    line_id);
        }/* End if action = modify ..*/
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SEC_PROXY_ADDRESS_SET)
    }/* End else for all_lines */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_call_prms
 * 
 * DESCRIPTION:     This function validates the params recieved in the set 
 *                  call params API.
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_cfg_validate_call_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_call_params_req_st    *p_recv_msg_payload = ICF_NULL;
    icf_return_t                         ret_value = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_call_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
  
        
    /* Validate the setup timer */
    if ((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_SETUP_TIMER &
                    p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->setup_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    /* Valiate the value of alerting timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_ALERTING_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->alerting_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    /* Validate the value of release timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_RELEASE_TIMER &
                    p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->release_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    /* Validate the value of modify media timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_MODIFY_MEDIA_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > 
             p_recv_msg_payload->modify_media_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

        /* Validate the value of reg response timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_RG_RSP_TIMER &
                p_recv_msg_payload->bit_mask)) && 
        (ICF_CFG_MIN_TIMER_VALUE > 
         p_recv_msg_payload->registration_resp_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    /* Validate the value of reg retry timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_RG_RETRY_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > 
             p_recv_msg_payload->registration_retry_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
   
    /*Fix Merged for CSR_1_5400409*/
    /* Validate the value of Max_active_calls_per_line */
    else if((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_MAX_ACTIVE_CALLS_PER_LINE &
                           p_recv_msg_payload->bit_mask)) &&
            ((ICF_MAX_NUM_OF_ACTIVE_CALLS_PER_LINE <
                p_recv_msg_payload->max_active_calls_per_line ) ||
            (ICF_ACTIVE_CALLS_PER_LINE_DEFAULT >
                p_recv_msg_payload->max_active_calls_per_line )))
    {

                ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_MAX_ACTIVE_CALLS_PER_LINE)
               ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    
#ifndef ICF_FEATURE_ABANDON    
    /* Validate the value of phone info timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_INFO_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->info_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    /* Validate the value of option timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_OPTIONS_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->options_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    
#endif
#ifdef ICF_SESSION_TIMER
     if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL &
                    p_recv_msg_payload->bit_mask))
    {
        if(ICF_CFG_DEF_MIN_SESSION_TIMER_INTERVAL > 
                p_recv_msg_payload->min_se)
        {
            p_crm_pdb->error_cause = ICF_CAUSE_INVALID_TIMER_VALUE;
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val; /* returns success always */
        }
        if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_SESSION_TIMER_INTERVAL &
                    p_recv_msg_payload->bit_mask))
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
        /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19120)*/
        else
        {
            /*SPR 19120 In case the SET_CALL_PARAM API does have only
             min-se and not session expires value then we will compare
             the min-se value to already configured session expires value
             in the config data.*/
            ICF_CHECK_IF_VAR_IS_NULL(p_crm_pdb->p_glb_pdb,
               p_crm_pdb->p_config_data, ret_value)
            if(ICF_FAILURE == ret_value)
            {
			    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
			    return ret_val; /* returns success always */
            }
            if(p_recv_msg_payload->min_se >
                    p_crm_pdb->p_config_data->session_timer)
            {
			    p_crm_pdb->error_cause = ICF_CAUSE_INVALID_TIMER_VALUE;
			    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
			    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
			    return ret_val; /* returns success always */
            }   
            /*End SPR 19120*/
        }        
	}
	else if(ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_SESSION_TIMER_INTERVAL &
                    p_recv_msg_payload->bit_mask))
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

#endif
      
#ifdef ICF_NAT_RPORT_SUPPORT

    /* Validate the value of binding refresh timer */
    else if ((ICF_FALSE == p_recv_msg_payload->binding_query_enable) &&\
            ((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_BINDING_REFRESH_TIMER &\
                    p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->\
             binding_refresh_timer)))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
#endif 
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_call_prms_in_db
 *
 * DESCRIPTION:     This function will add the data received in Set Call params 
 *                  message into global database.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_call_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_call_params_req_st    *p_recv_msg_payload = ICF_NULL;
    icf_error_t                          *p_ecode = ICF_ERROR_NONE;
    
#ifdef ICF_SESSION_TIMER

    icf_line_id_t                        line_index = 0;
#endif

#ifdef ICF_SESSION_TIMER
	icf_cfg_ssa_set_min_se min_se_internal_msg;
	icf_internal_msg_st internal_msg;
#endif
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

#ifdef ICF_SESSION_TIMER
    icf_port_memset(
            (icf_void_t*)&internal_msg,
            (icf_uint8_t)0,
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_internal_msg_st));
#endif
    
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_call_params_req_st *)
                            (p_crm_pdb->p_recv_payload);

    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    /*-----------------------------------------------------------------------*/
    /* Validate the data received */
    ret_val = icf_cfg_validate_call_prms(p_crm_pdb);

    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    if (ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        p_crm_pdb->result = ICF_FAILURE;
        ret_val = ICF_SUCCESS;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /*-----------------------------------------------------------------------*/
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CALL_PARAMS_RECD)
    
    /* For each optional param received, add the value in global database */
    /* Check for the presence of phone info */
    if ((ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_PHONE_INFO &
                            p_recv_msg_payload->bit_mask)) &&( ICF_NULL == 
                            p_crm_pdb->p_config_data->p_phone_info)) 
    {
        /* Allocate memory for Phone Info */
        ICF_STATIC_MEMGET(p_crm_pdb->p_glb_pdb,
                sizeof(icf_string_st),
                p_crm_pdb->p_config_data->p_phone_info,
                ICF_DONOT_RET_ON_FAILURE,
                p_ecode, ret_val)
            
 
        if(ICF_SUCCESS == ret_val)
        {
            icf_port_strcpy(p_crm_pdb->p_config_data->p_phone_info->str,
                    p_recv_msg_payload->phone_info.str);
        }
        else
        {
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INSUFFICIENT_RESOURCES)
        }
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CALL_PRMS_SET)
    } 
    /* Check for presence of default settings */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_DEFAULT_SETTINGS &
                            p_recv_msg_payload->bit_mask))
    {
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
        /*Rel 8.0:Check if the operation mode is IMS then check if the default 
         *settings in recv_msg_payload is set for call through proxy.
         */   
        if(ICF_CALL_CONFIG_CALL_THRU_SIP_PROXY != 
                              (p_recv_msg_payload->default_settings
                               & ICF_CALL_CONFIG_CALL_THRU_SIP_PROXY))
        {
              ICF_CFG_SET_RESPONSE(
                            p_crm_pdb, 
                            ICF_FAILURE,
                            ICF_ERROR_DIRECT_ROUTED_CALL_NOT_ALWD)
                    ret_val = ICF_FAILURE;
        }
ICF_CHECK_IMS_END
        /* Fix for SPR 18624 : Set default settings in config data only if ret_val is SUCCESS */
        if(ICF_SUCCESS == ret_val)
        {
            /*  CSR 1-6073561 Merge */
            if ((!(ICF_SET_CALL_PRM_STALE_PARAM_NOT_SET &
                     p_recv_msg_payload->bit_mask)) &&
                   (ICF_IGNORE_STALE_PARAM_IN_CHALLENGE & 
                     p_recv_msg_payload->default_settings))
            {
                p_crm_pdb->p_config_data->ignore_stale_param = ICF_TRUE; 
            }
            /*  CSR 1-6073561 Merge */
            else if(!(ICF_SET_CALL_PRM_STALE_PARAM_NOT_SET &
                      p_recv_msg_payload->bit_mask)) 
            {
                p_crm_pdb->p_config_data->ignore_stale_param = ICF_FALSE;
            }
#ifdef ICF_SESSION_TIMER

            /*  CSR 1-6073561 Merge */
            if ((!(ICF_SET_CALL_PRM_SESSION_REFRESH_METHOD_NOT_SET &
			p_recv_msg_payload->bit_mask_1)) &&
                   (ICF_CALL_CONFIG_SESSION_REFRESH_METHOD_UPDATE & 
                p_recv_msg_payload->default_settings))
            {
                p_crm_pdb->p_config_data->session_refresh_method = 
                    ICF_CFG_SESSION_REFRESH_METHOD_UPDATE;
            }
            /*  CSR 1-6073561 Merge */
            else if(!(ICF_SET_CALL_PRM_SESSION_REFRESH_METHOD_NOT_SET &
                      p_recv_msg_payload->bit_mask_1))
            {
                p_crm_pdb->p_config_data->session_refresh_method = 
                    ICF_CFG_SESSION_REFRESH_METHOD_INVITE;
            } 
#endif
            /* Fix for CSR 1-6178201:Type casting removed as both of them is now
		     * uint 16 bit.
		     */
            /*  CSR 1-6073561 Merge */
            p_crm_pdb->p_config_data->default_settings = 
                 p_recv_msg_payload->default_settings;
          
            if(ICF_TRUE == p_crm_pdb->p_config_data->ignore_stale_param)
            {
                p_crm_pdb->p_config_data->default_settings |= 
                              ICF_IGNORE_STALE_PARAM_IN_CHALLENGE;
            }
            else
            {
                p_crm_pdb->p_config_data->default_settings &= 
                             ~(ICF_IGNORE_STALE_PARAM_IN_CHALLENGE);
            }
#ifdef ICF_SESSION_TIMER
            if(ICF_CFG_SESSION_REFRESH_METHOD_UPDATE == 
               p_crm_pdb->p_config_data->session_refresh_method)
            {
                p_crm_pdb->p_config_data->default_settings |= 
                           ICF_CALL_CONFIG_SESSION_REFRESH_METHOD_UPDATE;
            }
            else
            {
                p_crm_pdb->p_config_data->default_settings &= 
                          ~(ICF_CALL_CONFIG_SESSION_REFRESH_METHOD_UPDATE);
            }
#endif

            if (ICF_CALL_CONFIG_CALL_MODIFY_METHOD_UPDATE & 
                p_recv_msg_payload->default_settings)
            {
                p_crm_pdb->p_config_data->call_modify_req_method = 
                    ICF_CFG_CALL_MODIFY_METHOD_UPDATE;
            }
            else
            {
                p_crm_pdb->p_config_data->call_modify_req_method = 
                    ICF_CFG_CALL_MODIFY_METHOD_INVITE;
            }
               
            if (ICF_CALL_CONFIG_FAX_REQUEST_METHOD_UPDATE & 
                p_recv_msg_payload->default_settings)
            {
                p_crm_pdb->p_config_data->fax_req_method = 
                    ICF_CFG_CALL_MODIFY_METHOD_UPDATE;
            }
            else
            {
                p_crm_pdb->p_config_data->fax_req_method = 
                    ICF_CFG_CALL_MODIFY_METHOD_INVITE;
            }

            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CALL_PRMS_SET)
        }
    }        
        
    /* Check for the presence of QOS mark */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_QOS_MARK &
                            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->qos_mark = 
            (icf_uint8_t) p_crm_pdb->p_config_data->qos_mark |
            p_recv_msg_payload->qos_mark;
        
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CALL_PRMS_SET)
    }        
           
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CALL_TIMERS_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_REG_TIMERS_RECD)

    /* Check for presence of setup timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_SETUP_TIMER &
                            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->setup_timer = 
                p_recv_msg_payload->setup_timer;
    }
    
    /* Check for presence of alerting timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_ALERTING_TIMER &
                            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->alerting_timer = 
            p_recv_msg_payload->alerting_timer;
    }

    /* Check for presence of release timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_RELEASE_TIMER &
                            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->release_timer = 
                p_recv_msg_payload->release_timer;
    }
    
    /* Check for presence of modify media timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_MODIFY_MEDIA_TIMER &
                            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->modify_media_timer = 
            p_recv_msg_payload->modify_media_timer;
    }
    
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CALL_TIMERS_SET)

    /* Check for presence of reg response timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_RG_RSP_TIMER &
                            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->registration_resp_timer = 
            p_recv_msg_payload->registration_resp_timer;
    }

    /* Check for the presence of reg retry timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_RG_RETRY_TIMER &
                            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->registration_retry_timer = 
            p_recv_msg_payload->registration_retry_timer;
    }  
    /* Check for the presence of info timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_INFO_TIMER &
                            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->info_timer = p_recv_msg_payload->info_timer;
    }
    /* Check for the presence of option timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_OPTIONS_TIMER &
                            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->options_timer = 
                                p_recv_msg_payload->options_timer;
    }

#ifndef ICF_FEATURE_ABANDON
	/* Check for the presence of resource_resrv_req */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_RG_RES_RSVTN_REQ &
                            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->nw_res_reservation_supported = 
            p_recv_msg_payload->resource_resrv_req;
    }
#endif
#ifdef ICF_SESSION_TIMER

if (ICF_SET_CALL_PRM_SESSION_TIMER_ENABLE & p_recv_msg_payload->bit_mask)
    {
        p_crm_pdb->p_config_data->session_timer_enable = p_recv_msg_payload->session_timer_enable; 
    }
    if (ICF_TRUE == p_crm_pdb->p_config_data->session_timer_enable)
    {
       if (ICF_SET_CALL_PRM_SESSION_TIMER_LOGIC & p_recv_msg_payload->bit_mask)
       {
         /* if this boolean is being enabled then it means we will change the timer 
          * logic according to the new implementation.*/
         if (ICF_TRUE == p_recv_msg_payload->session_timer_logic)
         {
                p_crm_pdb->p_config_data->session_timer_logic = ICF_TRUE;
         }
         else if (ICF_FALSE == p_recv_msg_payload->session_timer_logic)
         {
                p_crm_pdb->p_config_data->session_timer_logic = ICF_FALSE; 
         }
       }  
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_SESSION_TIMER_INTERVAL &
                            p_recv_msg_payload->bit_mask))
    {
		p_crm_pdb->p_config_data->session_timer = p_recv_msg_payload->session_timer;
    }

    /* Check for the presence of reg retry timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL &
                            p_recv_msg_payload->bit_mask))
    {
		p_crm_pdb->p_config_data->min_se = p_recv_msg_payload->min_se;
		
		min_se_internal_msg.min_se = p_recv_msg_payload->min_se;
		
		internal_msg.msg_hdr.msg_id = ICF_CFG_SSA_SET_MIN_SE;
		internal_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb;
		internal_msg.msg_hdr.payload_length = 
			ICF_PORT_SIZEOF(min_se_internal_msg);
		internal_msg.msg_hdr.status = ICF_NULL;
/*                internal_msg.msg_hdr.ecode = ICF_ERROR_NONE;*/
		internal_msg.p_msg_data = (icf_uint8_t *)&min_se_internal_msg;
		icf_ssa_process_mesg(&internal_msg);
    }     

     if (ICF_NULL != (ICF_SET_CALL_PRM_SESSION_REFRESHER_NOT_REQUIRED &
                        p_recv_msg_payload->default_settings))
     {
                /* Control will be here if Application does not want to send refresher
                 * in the outgoing INVITEs.*/
                p_crm_pdb->p_config_data->session_refresher_reqd = ICF_FALSE;
     }
     else
     {
         /*SPR19195: Reset the session_refresher_reqd flag for sending the 
           refresher parameter in the outgoing INVITE. This is required for
          the case when this flag is reset by setting the default settings
          bitmask ICF_SET_CALL_PRM_SESSION_REFRESHER_NOT_REQUIRED and then 
          later on again Application does send the SET_CALL_PARAMS API but
          this bitmask is not set so we should enable the flag.*/
                p_crm_pdb->p_config_data->session_refresher_reqd = ICF_TRUE;
     }
    }   
    /* Start: Changes for per line features additions */
    for(line_index = 0; (line_index <  (p_crm_pdb->p_config_data->max_lines)) &&
						(line_index < ICF_MAX_NUM_OF_LINES); 
        line_index++)
    {
       if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk( p_crm_pdb->p_glb_pdb, line_index,
                                                        &p_crm_pdb->p_line_data[line_index], 
                                                        p_crm_pdb->p_ecode))
        {
#ifdef ICF_SESSION_TIMER
         if (ICF_TRUE == p_crm_pdb->p_config_data->session_timer_enable)
         { 
            if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL &
                          p_recv_msg_payload->bit_mask))
            {
                p_crm_pdb->p_line_data[line_index]->min_se = p_recv_msg_payload->min_se;
            }
            if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_SESSION_TIMER_INTERVAL &
                          p_recv_msg_payload->bit_mask))
            { 
               p_crm_pdb->p_line_data[line_index]->session_timer = 
                        p_recv_msg_payload->session_timer;
            }
            if (ICF_NULL == (ICF_SET_CALL_PRM_SESSION_REFRESHER_NOT_REQUIRED &
                        p_recv_msg_payload->default_settings))
            {
               if (ICF_NULL != (ICF_SET_CALL_PRM_SESSION_REFRESHER &
                                     p_recv_msg_payload->bit_mask))
               {
                   p_crm_pdb->p_line_data[line_index]->session_timer_refresher = 
                                                  p_recv_msg_payload->session_refresher;
               }
            }
         }
#endif 
         if(ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_QOS_MARK & 
                         p_recv_msg_payload->bit_mask))
         {                
          p_crm_pdb->p_line_data[line_index]->rtp_dscp = p_recv_msg_payload->qos_mark;
          p_crm_pdb->p_config_data->qos_mark = p_recv_msg_payload->qos_mark;
         }
         /*  CSR 1-6073561 Merge */
         if(ICF_CFG_CC_SET_CALL_PRM_DEFAULT_SETTINGS & 
			 p_recv_msg_payload->bit_mask)
         {
             /*Changes for CSR_1-5671948*/
             /*  CSR 1-6073561 Merge */
             if ((!(ICF_SET_CALL_PRM_STALE_PARAM_NOT_SET &
                             p_recv_msg_payload->bit_mask)) && 
                  ICF_IGNORE_STALE_PARAM_IN_CHALLENGE & p_recv_msg_payload->default_settings)
             {
                 p_crm_pdb->p_line_data[line_index]->ignore_stale_param = ICF_TRUE;
             }
             if(!(ICF_SET_CALL_PRM_STALE_PARAM_NOT_SET &
                             p_recv_msg_payload->bit_mask))
             {
                 p_crm_pdb->p_line_data[line_index]->ignore_stale_param = ICF_FALSE;
             }
#ifdef ICF_SESSION_TIMER
             if ((!(ICF_SET_CALL_PRM_SESSION_REFRESH_METHOD_NOT_SET &
                             p_recv_msg_payload->bit_mask_1)) &&
                 ICF_CALL_CONFIG_SESSION_REFRESH_METHOD_UPDATE & p_recv_msg_payload->default_settings)
             {
                 p_crm_pdb->p_line_data[line_index]->session_refresh_method = 
                                                   ICF_CFG_SESSION_REFRESH_METHOD_UPDATE;
             }
             else if(!(ICF_SET_CALL_PRM_SESSION_REFRESH_METHOD_NOT_SET &
                             p_recv_msg_payload->bit_mask_1))
             {
                 p_crm_pdb->p_line_data[line_index]->session_refresh_method = 
                                                   ICF_CFG_SESSION_REFRESH_METHOD_INVITE;
             }
#endif
        }
/*Changes for CSR_1-5671948*/
#ifdef ICF_NAT_RPORT_SUPPORT
          if(ICF_NULL != p_crm_pdb->p_line_data[line_index]->p_rport_config_data)
          {
              p_crm_pdb->p_line_data[line_index]->p_rport_config_data->binding_refresh_timer =
                  ICF_CFG_BINDING_REFRESH_TIMER;
          }
#endif
       }
    }
    /* End : changes for per line feature flag */
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
    

    if(ICF_FALSE == p_recv_msg_payload->binding_query_enable)
    {
        /* Get the binding refresh timer value from the configuration API
         * if configured else use the default value
         */
        if(ICF_NULL !=(ICF_CFG_CC_SET_CALL_PRM_BINDING_REFRESH_TIMER &
                    p_recv_msg_payload->bit_mask))
        {
            /* If value is less than 40 secs then set the binding refresh timer
             * value to a default value.
             */
            if(40 > p_recv_msg_payload->binding_refresh_timer)
            {
                p_crm_pdb->p_config_data->rport_config_data.binding_refresh_timer = \
                                          ICF_CFG_BINDING_REFRESH_TIMER;

            }
            else
            {
                p_crm_pdb->p_config_data->rport_config_data.binding_refresh_timer = 
                    p_recv_msg_payload->binding_refresh_timer;
            }
        }
        else
        {
            p_crm_pdb->p_config_data->rport_config_data.binding_refresh_timer =\
                                                     ICF_CFG_BINDING_REFRESH_TIMER;
        }
    }
#endif 

   if(ICF_SET_CALL_PRM_MULTIPLE_ANSWER_ENABLE & p_recv_msg_payload->bit_mask)
    {
	  if(ICF_TRUE == p_recv_msg_payload->multiple_answer_enable)
	  {
		p_crm_pdb->p_config_data->multiple_answer_enable = \
			ICF_TRUE;
	  }
	  else
	  {
		p_crm_pdb->p_config_data->multiple_answer_enable =\
			ICF_FALSE;
	  }
    }
   if (ICF_SET_CALL_PRM_UPDATE_REQD & p_recv_msg_payload->bit_mask)
    {
        /* CSR 1-7402396 : SPR-19822 Fixed*/
        if((ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC == \
                    p_recv_msg_payload->update_reqd) || 
           (ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC == \
                    p_recv_msg_payload->update_reqd) ||
           (ICF_NO_UPDATE == p_recv_msg_payload->update_reqd) ||
           (ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC_COMMIT == \
                    p_recv_msg_payload->update_reqd)||
           (ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC_COMMIT == \
                    p_recv_msg_payload->update_reqd))
        {
           /* Populate application specific update handling parameter */
            p_crm_pdb->p_config_data->update_reqd = p_recv_msg_payload->update_reqd;
        }
     }
   if (ICF_SET_CALL_PRM_EARLY_INC_IND_FOR_ALL_USERS & p_recv_msg_payload->bit_mask)
     {
          if(ICF_TRUE == p_recv_msg_payload->early_inc_ind_for_all_users)
             {
                p_crm_pdb->p_config_data->early_inc_ind_for_all_users = \
                        ICF_TRUE;
             }
           else
             {
                p_crm_pdb->p_config_data->early_inc_ind_for_all_users =\
                        ICF_FALSE;
             }
     }

    /* Check for the presence of DNS Min Retransmission Timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_DNS_MIN_TIMER &
                            p_recv_msg_payload->bit_mask))
    {
          p_crm_pdb->p_config_data->dns_min_retrans = p_recv_msg_payload->dns_min_retrans;
    }		

   /* Check for the presence of DNS Min Retransmission Timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_DNS_MAX_TIMER & 
                            p_recv_msg_payload->bit_mask))
    {
          p_crm_pdb->p_config_data->dns_max_retrans = p_recv_msg_payload->dns_max_retrans;
    }
    /* Check for the presence of max_active_calls_per_line_limit and assign it 
    * into the config data structure field if present */

    /*Fix Merged for CSR_1_5400409*/
    if(ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_MAX_ACTIVE_CALLS_PER_LINE &
                    p_recv_msg_payload->bit_mask))
    {     
        p_crm_pdb->p_config_data->max_active_calls_per_line =
                         p_recv_msg_payload->max_active_calls_per_line;
    }    
    
    /* Check if DHCP response timer configured in request */
    if(ICF_NULL != (ICF_SET_CALL_PRM_DHCP_RESPONSE_TIMER & 
		    p_recv_msg_payload->bit_mask))
    {
        /* Update config_data for DHCP response timer from request */
        p_crm_pdb->p_config_data->dhcp_response_timer = \
                p_recv_msg_payload->dhcp_response_timer;
    }
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_REG_TIMERS_SET)
    
    /* Check for the presence of access_network_info */
    if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_ACCESS_NW_INFO &
                            p_recv_msg_payload->bit_mask))
                            
    {
        /*If Application has set the bitmask for presence of
         * access_network_info header but the value is null then it
         * means application wants to delet the header value*/
         if(0 == icf_port_strcmp(
               (icf_uint8_t *)p_recv_msg_payload->access_network_info.str,
               (icf_uint8_t *)""))
         {
             if (ICF_NULL != p_crm_pdb->p_config_data->p_access_network_info)
             {
                    /* De-allocate memory for Access Network Info */
                     ICF_STATIC_MEMFREE(p_crm_pdb->p_glb_pdb,
                        p_crm_pdb->p_config_data->p_access_network_info,
                        p_ecode, 
                        ret_val)
              }
         }
        else 
          {
            /*If p_access_network_info is already configured earlier, 
              then overwrite its value, do memget only when it is
              first time configured*/
            if (ICF_NULL == p_crm_pdb->p_config_data->p_access_network_info)
            {
               /* Allocate memory for Access Network Info */
               ICF_STATIC_MEMGET(p_crm_pdb->p_glb_pdb,
                  sizeof(icf_string_st),
                  p_crm_pdb->p_config_data->p_access_network_info,
                  ICF_DONOT_RET_ON_FAILURE,
                  p_ecode, ret_val)
            }
            if(ICF_SUCCESS == ret_val)
            {
                icf_port_strcpy(p_crm_pdb->p_config_data->p_access_network_info->str,
                    p_recv_msg_payload->access_network_info.str);
            }
           else
            {
               ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INSUFFICIENT_RESOURCES)
            }
            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CALL_PRMS_SET)
          }      
    }

    /*Start: Rel8.0 Added for precondition support */
#ifdef ICF_QOS_SUPPORT
    if(ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_PRECONDITION_REQD &
                    p_recv_msg_payload->bit_mask))
    {   
        p_crm_pdb->p_config_data->precondition_required =
            p_recv_msg_payload->precondition_required;
        
    }    
#endif
    /*End: Rel8.0 Added for precondition support */

   if (ICF_NULL != (ICF_CFG_CC_SET_CALL_PRM_MEDIA_TYPE_REQD &
                                             p_recv_msg_payload->bit_mask))     
   {
         /* Set the transport type in config data */
            p_crm_pdb->p_config_data->media_transport_type = 
                       p_recv_msg_payload->media_transport_type;
   }

    /*******************ICF 8.0***************/
    /*If in config data, is_reg_triggered flag is TRUE then the value for
      reg_event_subs_reqd will be ignored */
    if (ICF_FALSE == p_crm_pdb->p_config_data->is_reg_triggered)
    {
        if (ICF_NULL != (ICF_SET_CALL_PRM_REG_EVENT_SUBS_REQD &
                    p_recv_msg_payload->bit_mask))
        {
            p_crm_pdb->p_config_data->reg_event_pkg_support_reqd = \
                   p_recv_msg_payload->reg_event_subs_reqd;
            
            /* This boolean is set for the following scenario:
             * Operation mode is Non IMS and application triggers this API
             * a second time.No registration request has been triggered.
             * In such a scenario we have to fail the API with
             * erro cause ICF_CAUSE_REG_EVENT_SUBS_ALREADY_CONFIGURED.
             */
            p_crm_pdb->p_config_data->is_reg_triggered = ICF_TRUE;
        }    
    
    }
    else
    {
        if (ICF_NULL != (ICF_SET_CALL_PRM_REG_EVENT_SUBS_REQD &
                    p_recv_msg_payload->bit_mask))
        {
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_REG_EVENT_SUBS_ALREADY_CONFIGURED)
        }            
    }
    /* The flag allows system level configuration of SRTP Fallback 
       required */
    if (ICF_NULL != (ICF_SET_CALL_PRM_SRTP_FALLBACK_FLAG &
                    p_recv_msg_payload->bit_mask))
    {
            p_crm_pdb->p_config_data->srtp_fallback_reqd  = \
                   p_recv_msg_payload->srtp_fallback_reqd ;

    }
    /*Set SRTP fallback response codes list in config_data
     *Free if application has earlier configured the same and then
     *configure new list*/
    if (ICF_NULL != (ICF_SET_CALL_PRM_SRTP_FALLBACK_RESP_CODES &
                    p_recv_msg_payload->bit_mask))
    {
       if (ICF_NULL != p_crm_pdb->p_config_data->p_srtp_fallback_resp_codes)
       {
           icf_cmn_delete_list(p_crm_pdb->p_glb_pdb, 
                &(p_crm_pdb->p_config_data->p_srtp_fallback_resp_codes),
                ICF_MEM_COMMON);
       }

        p_crm_pdb->p_config_data->p_srtp_fallback_resp_codes = \
              p_recv_msg_payload->p_srtp_fallback_resp_codes ;

        /* SPR 19752: reset the data in NULL so that in case of any
         * other failure, the data can be freed through CLIB
         */ 
        p_recv_msg_payload->p_srtp_fallback_resp_codes  = ICF_NULL;
    }
    
	/* Check for presence of App porting configuration parameter */
    if (ICF_NULL != (ICF_SET_CALL_PRM_APP_PORT_CONFIG & 
					p_recv_msg_payload->bit_mask_1))
    {     
        p_crm_pdb->p_config_data->app_port_config =
                    p_recv_msg_payload->app_port_config;
    }
    
    /*  Fix Merge for CSR 1-6456839 */
    if (ICF_NULL != (ICF_SET_PORT_IN_FROM_HDR &
                            p_recv_msg_payload->bit_mask_1))
    {
          p_crm_pdb->p_config_data->port_to_be_sent_in_from_header = 
              p_recv_msg_payload->port_to_be_sent_in_from_hdr;
    }

    /* Rel 8.1 : SDP Tunnelling Enhancements :
       This indicates that the application wants early session indication for
       incoming INVITE and out of dialog OPTIONS requests.  */
    if (ICF_NULL != (ICF_SET_CALL_PRM_EARLY_INC_SESSION_IND_REQD & 
					 p_recv_msg_payload->bit_mask_1))
    {     
        p_crm_pdb->p_config_data->early_incoming_session_ind_reqd =
                    p_recv_msg_payload->early_incoming_session_ind_reqd;
    }

    /* CSR_1-6347417 fixed:
     * Check for presence of retry_codec_commit_req*/
    if(ICF_NULL != (ICF_SET_CALL_PRM_RETRY_CODEC_COMMIT_REQ &
                                p_recv_msg_payload->bit_mask_1))
    {     
        p_crm_pdb->p_config_data->retry_codec_commit_req =
                    p_recv_msg_payload->retry_codec_commit_req;
    }

    if(ICF_SET_CALL_PRM_UPDATE_FOR_OFFERANSWER_AFTER_CONNECT & 
			p_recv_msg_payload->bit_mask_1)
    {
		p_crm_pdb->p_config_data->update_for_offeranswer_after_connect = 
		p_recv_msg_payload->update_for_offeranswer_after_connect;
	}

    /* Fix for CSR 1-6727350: SPR 19316 */
    if (ICF_NULL != (ICF_SET_CALL_PRM_APP_CONNECT_REQD_FOR_REPLACES &
                            p_recv_msg_payload->bit_mask_1))
    {
          p_crm_pdb->p_config_data->app_connect_reqd_for_replaces = 
				p_recv_msg_payload->app_connect_reqd_for_replaces;
    }

    /* Asynchronous NOTIFY support : Read value of boolean 
       async_notify_support_reqd and save it in config data.
     */
    if(ICF_NULL != (ICF_SET_CALL_PRM_ASYNC_NOTIFY_SUPPORT_REQD & 
         p_recv_msg_payload->bit_mask_1))
    {     
        p_crm_pdb->p_config_data->async_notify_support_reqd =
                    p_recv_msg_payload->async_notify_support_reqd;
    }


    /* Fix for CSR-1-6976670 */
    if(ICF_NULL != (ICF_SET_CALL_PRM_CHECK_REMOTE_CODEC_LIST & 
        p_recv_msg_payload->bit_mask_1))
    {
        p_crm_pdb->p_config_data->check_remote_codec_list = 
        p_recv_msg_payload->check_remote_codec_list;
    }

    /* CSR_1_7113139 Merged SPR 19672 Start */
    /*SPR 19462*/
    if(ICF_NULL != (ICF_SET_CALL_PRM_INC_SDP_VERSION_WHEN_NO_CHANGE & 
                    p_recv_msg_payload->bit_mask_1))
    {
        p_crm_pdb->p_config_data->inc_sdp_version_when_no_change = 
        p_recv_msg_payload->inc_sdp_version_when_no_change;
    }
    /* CSR_1_7113139 Merged SPR 19672 End */

    /* Rel 8.2 : Asynchronus Message Enhancements
       This indicates that the application wants Asynchronus Message Support  */
    if(ICF_NULL != (ICF_SET_CALL_PRM_ASYNC_MESSAGE_SUPPORT_REQD & 
         p_recv_msg_payload->bit_mask_1))
    {     
        p_crm_pdb->p_config_data->async_message_support_reqd =
                    p_recv_msg_payload->async_message_support_reqd;
    }


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* always returns success */
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_self_id_prms
 *
 * DESCRIPTION:     This function will validate the self id parameters in the 
 *                  API. It checks invalid line id, validates that user does not
 *                  already exist and checks if it only priority update.
 *                  
 ******************************************************************************/
icf_return_t icf_cfg_validate_self_id_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_line_id_t                        line_index = 0;
    icf_line_id_t                        line_id = 0;
    icf_line_id_t                         num_of_users[ICF_MAX_NUM_OF_LINES];
    icf_user_db_node_st                  *p_user_db = ICF_NULL;
    icf_boolean_t                        icf_flag = ICF_FALSE;
    icf_uint8_t                          *p_temp_str = ICF_NULL;

        
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Initializing num_of_users to null */
    for(line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; line_index++)
    {
        num_of_users[line_index] = (icf_uint8_t)0;
    }

#ifdef ICF_MULTILINE
    /* Set only_priority_update flag to False */
    p_crm_pdb->only_priority_update = ICF_FALSE;
#endif

    /* copy the external request payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_self_id_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
        
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_ID_PRMS_RECD)
     
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

    /* If address is present */
    else if(ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_ADDR &
                p_recv_msg_payload->bit_mask))
    {
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_ADDR_RECD)

        /* Invoke common function to validate all parameters of 
         * icf_address_st */
         if(ICF_FAILURE == icf_cmn_validate_trnspt_addr(
                     p_crm_pdb->p_glb_pdb, 
                     &(p_recv_msg_payload->self_addr), 
                     &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INVALID_ADDRESS_RECEIVED)
        }
        else
        {
            /*Check if @ present in the addr string, if present then p_temp_str
             *will point to the positon pointed by @, else p_temp_str will point 
             *to addr_val. This is done to check tag parameters only after @ symbol
             *and allow application to configure self address with user parameters*/
            p_temp_str = 
                (icf_uint8_t *)icf_port_strchr(p_recv_msg_payload->self_addr.addr_val.str, '@');

            if (ICF_NULL == p_temp_str)
            {
                p_temp_str = (icf_uint8_t*)&(p_recv_msg_payload->self_addr.addr_val.str);
            }
        }

        if (ICF_FALSE == p_crm_pdb->is_valid_param)
        {
        }
        /* CSR 1-5826493 */
#if 0     
        /* For User Param Support if tags are present in the 
         * Self Address String, then we will simply discard it,
         * with ICF_CAUSE_INVALID_ADDRESS_RECEIVED as
         * error code */
        else if (ICF_NULL != icf_port_strchr(p_temp_str, ';'))
        {
            /* tag values are not allowed in Self Address String*/
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                 ICF_FAILURE,
                                 ICF_CAUSE_INVALID_ADDRESS_RECEIVED)
        }
#endif
        /* If action is add, call the function to validate add action of 
         * the API
         */
        else if(ICF_ALL_LINES == p_recv_msg_payload->line_id)
        {
            if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
            {
                for(line_index = 0; line_index < (p_crm_pdb->p_config_data->
                    max_lines); line_index++)
                {
                    ret_val = icf_cfg_validate_add_self_id_prms(p_crm_pdb,
                            line_index); 
               
                    /* This function always returns success so ret_val is
                     * not checked
                     */
                  
                    if(ICF_FALSE == p_crm_pdb->is_valid_param)
                    {
                        /* Parameters are not valid */
                        break;
                    }                            
                }/* End for */
            }/* End if action is add  */
            /* There is validation for presence of the user on the mentioned
               line in case the action is delete. Further, if there
               are any call/SIC contexts/registration for this user on this
               line then these shall be cleared by CFG*/
            else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
            {
                for(line_index = 0; line_index < (p_crm_pdb->p_config_data->
                    max_lines); line_index++)
                {
                    if( ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line(
                          p_crm_pdb->p_glb_pdb,
                          line_index,
                          &(p_recv_msg_payload->self_addr),
                          ICF_USER_DB_USE_PUBLIC_URIS,
                          &p_user_db,
                          &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))	
                    {
                        icf_flag = ICF_TRUE;
                        break;
                    }
                }
                if(ICF_FALSE == icf_flag)
                {
                    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                    ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_USER_NOT_ADDED)	
                }
            }
            /* SPR 16019: Code removed */

        }/* End if for all_lines */
   
        /* If all_lines is false */
        else
        {
            if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
            {
                line_id = p_recv_msg_payload->line_id;

                /* Call the function to compare the user addresses
                 * The boolean value is set to true to check that
                 * this request is a repeated one or not
                 */
                ret_val = icf_cfg_validate_add_self_id_prms(p_crm_pdb,
                line_id);
                
            }/* if(ICF_CONFIG_ACTION_ADD */

            /* There is validation for presence of the user on the mentioned
               line in case the action is delete. Further, if there
               are any call/SIC contexts/registration for this user on this
               line then these shall be cleared by CFG*/
            else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
            {
                line_id = p_recv_msg_payload->line_id;

                if( ICF_FAILURE == icf_dbm_get_public_uri_node_from_uri_for_line(
                          p_crm_pdb->p_glb_pdb,
                          line_id,
                          &(p_recv_msg_payload->self_addr),
                          ICF_USER_DB_USE_PUBLIC_URIS,
                          &p_user_db,
                          &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))				
                {
                    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                    ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_USER_NOT_ADDED)				
                }
            }
            /* SPR 16019: Code removed */

        }/* End else for all_lines */
     }/* if(ICF_NULL != */           

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb);
    return ret_val;    
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_add_self_id_prms
 *
 * DESCRIPTION:     This function will validate the add action in the self id 
 *                  prms API. It is called by the function to validate the
 *                  prms of this API when ever add action in found in the
 *                  prms received.
 *                  
 ******************************************************************************/
icf_return_t icf_cfg_validate_add_self_id_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_uint8_t          line_status[ICF_MAX_NUM_OF_LINES];
	icf_user_db_node_st	*p_user_db = ICF_NULL;

#ifdef ICF_MULTILINE
    icf_priority_id_t    *p_user_priority = ICF_NULL;
#endif
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    p_recv_msg_payload = (icf_cfg_cc_set_self_id_params_req_st *)
            (p_crm_pdb->p_recv_payload);
    
    /* The entry is made as valid in the pdb */
    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_TRUE)
                    
    /* Fetch the line pointer if the line exists */
    if(ICF_SUCCESS == icf_cfg_check_line_in_db(
                p_crm_pdb,
                line_id,
                line_status,
                (icf_boolean_t)ICF_FALSE))/* no get_line needed */
    {
        /* Success by this function means that the line exists and 
         * pointer to the line is returned
         */ 
         
        /* Check that the same user with same priority is not already present 
         * on the line.
         */
/* Start: Multiple Public Users ICF5.0*/
       p_user_db = p_crm_pdb->p_line_data[line_id]->p_user_db;
        
#ifdef ICF_MULTILINE
        p_user_priority = p_crm_pdb->p_line_data[line_id]->p_user_priority_list;
#endif
        while( ICF_NULL != p_user_db) 


        {
            if(ICF_TRUE == icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
                        &(p_recv_msg_payload->self_addr),
                        &(p_user_db->public_user)))
/* End: Multiple Public Users ICF5.0*/
            {
#ifdef ICF_MULTILINE
                /* If the user is already present, check its priority */
                if((ICF_CFG_CC_SET_SLF_ID_PRM_PRIORITY == 
                        (p_recv_msg_payload->bit_mask &
                        ICF_CFG_CC_SET_SLF_ID_PRM_PRIORITY)) && 
                        p_recv_msg_payload->priority == (*p_user_priority))
                {
                    /* If priority is present in the payload and it is the same
                     * as that of user on the line,then return Failure response.
                     */

                    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            ICF_CAUSE_INVALID_ACTION)
                    break;
                }
                else if ((ICF_CFG_CC_SET_SLF_ID_PRM_PRIORITY == 
                        (p_recv_msg_payload->bit_mask &
                        ICF_CFG_CC_SET_SLF_ID_PRM_PRIORITY)) && 
                        p_recv_msg_payload->priority != (*p_user_priority))
                {
                    /* If priority is present in the payload, but is different 
                     * from that of the user on the line, it means that the 
                     * request is for a change in priority. 
                     * Set flag to indicate this.
                     */
                    p_crm_pdb->only_priority_update = ICF_TRUE;
                }
#endif
                if(ICF_CFG_CC_SET_SLF_ID_PRM_PRIORITY != 
                        (p_recv_msg_payload->bit_mask &
                        ICF_CFG_CC_SET_SLF_ID_PRM_PRIORITY))
                {
                    /* If priority is not present in the payload, user is same
                     * Thus, it invalid action to add user that exists on line*/
                    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            ICF_CAUSE_INVALID_ACTION)
                }
				break;
            }
            else
            {
                /* If user is not present then increment the address 
                 * pointer to check the next address
                 */                                
/* Start: Multiple Public Users ICF5.0*/
					p_user_db = (icf_user_db_node_st *)p_user_db->p_next;
#ifdef ICF_MULTILINE
					p_user_priority++;
#endif
/* End: Multiple Public Users ICF5.0*/
            }
        }/* End while */      
        if((ICF_TRUE == p_crm_pdb->is_valid_param) &&
#ifdef ICF_MULTILINE
                (ICF_FALSE == p_crm_pdb->only_priority_update) &&
#endif
                (p_crm_pdb->p_config_data->max_users_per_line <
                (icf_dbm_get_public_user_count(p_crm_pdb->p_line_data[line_id]->p_user_db)+ 1)))
        {
            /* If it is not a request for only priority update and
             * a user has to be added on the line, then it must not exceed max 
             * users per line */
            
            p_crm_pdb->line_id = line_id;
            
            ICF_CFG_TRACE(p_crm_pdb, 
                    ICF_CFG_TRACE_MAX_NUM_OF_USERS_REACHED)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                
            ICF_CFG_SET_RESPONSE(p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_MAX_USERS_PER_LINE_EXCEEDED)
        }
    }/* End if(ICF_SUCCESS== ..*/


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* Always returns success */
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_del_user_in_db
 *
 * DESCRIPTION:     This function will delete the user from the specified
 *                  line id.
 *
 ******************************************************************************/
icf_return_t icf_cfg_del_user_in_db(
        INOUT icf_crm_pdb_st     *p_crm_pdb,        
        IN    icf_line_id_t      line_id)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_uint8_t          user_index = 0;
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload = ICF_NULL;
	icf_user_db_node_st	*p_user_db = ICF_NULL;
		    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_self_id_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
/* Start: Multiple Public Users ICF5.0*/
	p_user_db = p_crm_pdb->p_line_data[line_id]->p_user_db;
	
	while(ICF_NULL != p_user_db)
    {
        if (ICF_TRUE == icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
                    &(p_user_db->public_user),
                    &(p_recv_msg_payload->self_addr)))
        {
	    p_crm_pdb->line_id = line_id;
            p_crm_pdb->user_id=user_index;  
  	    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_ADDR_SET)

            /* If user is found on the line, delete it */
			/* trigger dbm function for deleting the user node*/
			icf_dbm_delete_public_uri_for_line(p_crm_pdb->p_glb_pdb,
                line_id, &(p_user_db->public_user),p_crm_pdb->p_ecode);

			
            break;
        }
		p_user_db = (icf_user_db_node_st *)p_user_db->p_next;
		user_index++;
    }
      
	/* Fix Merged for CSR 1-6801610: SPR 19265 */
	/* If all the users have been deleted from line, reset the bitmask for 
	 * self address.
	 */
	if (ICF_NULL == p_crm_pdb->p_line_data[line_id]->p_user_db)
	{
		ICF_CFG_RESET_BITMASK(
			p_crm_pdb->p_line_data[line_id]->line_data_received,
			ICF_CFG_LINE_DATA_SELF_ADDR_RECD);
	}
    
/* End: Multiple Public Users ICF5.0*/                            
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)        
    return ret_val; /* Always returns success */
}    
    

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_add_user_in_db
 *
 * DESCRIPTION:     This function will add the user on the specified
 *                  line id.
 *
 ******************************************************************************/
icf_return_t icf_cfg_add_user_in_db(
        INOUT icf_crm_pdb_st     *p_crm_pdb,
        IN    icf_line_id_t      line_id)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_uint8_t          user_index = 0;
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_self_id_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

	ret_val = icf_dbm_add_public_uri_for_line(p_crm_pdb->p_glb_pdb,line_id,
	&(p_recv_msg_payload->self_addr), p_crm_pdb->p_ecode);
 
#ifdef ICF_MULTILINE
    if(ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_PRIORITY & 
            p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_line_data[line_id]->p_user_priority_list[user_index] = 
            p_recv_msg_payload->priority;
    }
    else
    {
        p_crm_pdb->p_line_data[line_id]->p_user_priority_list[user_index] = 255;
    }       
#endif    

    p_crm_pdb->line_id = line_id;
    p_crm_pdb->user_id = user_index;

	if((ICF_SUCCESS == ret_val) && (line_id < ICF_MAX_NUM_OF_LINES))
	{
		ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_ADDR_SET)

		/*trigger dbm function for adding user*/
		ICF_CFG_SET_BITMASK(p_crm_pdb->p_line_data[line_id]->
				line_data_received,ICF_CFG_LINE_DATA_SELF_ADDR_RECD)
	}

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)        
    return ret_val; /* Always returns success */
} 

#ifdef ICF_MULTILINE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_update_priority
 *
 * DESCRIPTION:     This function will update the priority of the user on the 
 *                  line specified without adding a new user.
 *
 ******************************************************************************/
icf_return_t icf_cfg_update_priority(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_line_id_t             line_id)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_uint8_t          user_index = 0;
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_user_db_node_st	*p_user_db = ICF_NULL;
    icf_priority_id_t    *p_user_priority = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_self_id_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    /* Fetch user address and priority lists in local pointers */
    p_user_db = p_crm_pdb->p_line_data[line_id]->p_user_db;
    p_user_priority = p_crm_pdb->p_line_data[line_id]->p_user_priority_list;

    while( ICF_NULL != p_user_db) 

    {
        /* Search for user on the line for which request is recd */
        if(ICF_TRUE == icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
                    &(p_recv_msg_payload->self_addr),
                    p_user_db->public_user))
        {
            /* If user is found, priority needs to be updated */
            *p_user_priority = p_recv_msg_payload->priority;
            break;
        }
        else
        {
            /* If user is not found in the line, move to the next user */
            p_user_db = (icf_user_db_node_st *)p_user_db->p_next;
            p_user_priority++;
        }
		user_index++;
    }/* End while */
   
    p_crm_pdb->line_id = line_id;
    p_crm_pdb->user_id = user_index; 
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_ADDR_SET)

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_delete_line
 *
 * DESCRIPTION:     This function will delete the line if after the user has 
 *                  been deleted, registrar and proxy also don't exist on the 
 *                  line specified.
 *
 ******************************************************************************/
icf_return_t icf_cfg_delete_line(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_line_id_t             line_id)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Check if proxy, registrar and user are not present on the line then 
     * delete the line
     */
    if((ICF_NULL == (ICF_CFG_LINE_DATA_PROXY_RECD &
                    p_crm_pdb->p_line_data[line_id]->line_data_received)) &&
       (ICF_NULL == (ICF_CFG_LINE_DATA_SEC_PROXY_RECD &
                    p_crm_pdb->p_line_data[line_id]->line_data_received)) &&
            (ICF_NULL == (ICF_CFG_LINE_DATA_REGISTRAR_RECD &
                             p_crm_pdb->p_line_data[line_id]->
                             line_data_received)) &&
            (ICF_NULL == (ICF_CFG_LINE_DATA_SEC_REGISTRAR_RECD &
                             p_crm_pdb->p_line_data[line_id]->
                             line_data_received)) &&
	    (ICF_NULL == (ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD &
                             p_crm_pdb->p_line_data[line_id]->
                             line_data_received)) && 
            (ICF_NULL == (ICF_CFG_LINE_DATA_SELF_ADDR_RECD & 
                             p_crm_pdb->p_line_data[line_id]->
                             line_data_received)))
    {
        ret_val = icf_dbm_dealloc_line_blk(
                p_crm_pdb->p_glb_pdb, 
                line_id,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));
        /* this memory has been freed by DBM. So marking in crm_pdb */

        p_crm_pdb->p_line_data[line_id] = ICF_NULL;

        p_crm_pdb->line_id = line_id;
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_LINE_BLK_DEALLOCATED)
        /* SPR 19405 Merged SPR 19672 Start */
        if (ICF_SUCCESS == icf_cfg_check_all_lines_dealloc (p_crm_pdb))
        {
           /*Fix for CSR 1-8597937 SPR 20453.*/
           /*This function shall clear the registration ctxt immediately if all the lines
            * are deleted.
            */
            p_crm_pdb->forceful_clear = ICF_TRUE;

            ret_val = icf_rgm_forceful_dereg_all_users(p_crm_pdb);

           /* Invoke SSA to close all connections */
           ret_val = icf_cfg_close_nw_interface(p_crm_pdb);

           p_crm_pdb->p_glb_pdb->p_glb_data-> global_data_init_complete =
                                                         ICF_FALSE;
        }
        /* SPR 19405 Merged SPR 19672 End */    
    }/* End if */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_self_id_prms_in_db
 *
 * DESCRIPTION:     This function will save the self id parameters in the 
 *                  global databse.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_self_id_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                          ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_self_id_params_req_st  *p_recv_msg_payload = ICF_NULL;
    icf_line_id_t                         line_index = 0;
    icf_line_id_t                         line_id = 0;
    icf_uint8_t                           line_status[ICF_MAX_NUM_OF_LINES];
    icf_rgm_context_st	                  *p_rgm_context = ICF_NULL;
    /*  CSR 1-5599924 Merging */
    icf_user_db_node_st                   *p_user_db = ICF_NULL;

/* waring removal */     
/*#ifdef  ICF_NAT_RPORT_SUPPORT 
    char            *user = ICF_NULL;
    icf_uint8_t     len = 0;
#endif*/
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_self_id_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
       
    /*-----------------------------------------------------------------------*/
    /* Validate the data received */ 
    ret_val = icf_cfg_validate_self_id_prms(p_crm_pdb);
    
    /* This function sets the api_indexes to be processes for the user address
     * to true 
     */
    
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_USER_INFO)
    if(ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        if(ICF_NULL != p_recv_msg_payload)
        {
            p_crm_pdb->line_id = p_recv_msg_payload->line_id;
        }
        p_crm_pdb->result = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }            
    
    /*-----------------------------------------------------------------------*/
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    /* SPR 15997: line_id in ICF_CONFIGURE_CFM_API */
    p_crm_pdb->line_id = p_recv_msg_payload->line_id; 

    /*-----------------------------------------------------------------------*/
    /* Check the presence flag to see if address is present */
    if(ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_ADDR &
                p_recv_msg_payload->bit_mask))
    {
        for(line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; line_index++)
        {
            p_crm_pdb->lines_present[line_index] = ICF_FALSE;
        }
        
        for(line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; line_index++)
        {
            line_status[line_index] = ICF_STATUS_LINE_OLD;
        }

        if(
                (ICF_ALL_LINES == p_recv_msg_payload->line_id) 
#ifdef ICF_MULTILINE 
                && (ICF_FALSE == p_crm_pdb->only_priority_update)
#endif
           )
        {
            /* Get line data pointer for all lines configured till now.
             * If no line is configured then configure all the lines if action 
             * is add. If action is delete or modify ony fetch pointers of
             * configured lines.
             */

/* Start: Multiple Public Users ICF5.0*/
            if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
            {
                if(ICF_FAILURE == icf_cfg_check_line_configured(
                            p_crm_pdb,
                            (icf_boolean_t)ICF_TRUE))
                {
                    ret_val = ICF_FAILURE;
                    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                    return ret_val;
                }
            }
            else
            {
                ret_val = icf_cfg_check_line_configured(p_crm_pdb,
                            (icf_boolean_t)ICF_FALSE);
            }
			/* if action is delete and there is no line configured then we need
			 * to fail the request.
			 */
			if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
			{
				if(ICF_FAILURE == ret_val)
                {
                    ret_val = ICF_FAILURE;
                    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                    return ret_val;
				}
			}
				

            /* Now the pointers needed are taken and data can be set on the 
             * lines 
             */
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                        max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                  /* Check if action is add */
                    if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
                    {
                        /* Add the user on the line. */
                        ret_val = icf_cfg_add_user_in_db(p_crm_pdb, 
                                line_index);
                    }
                   
					if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                                action)
					{
					/* if action is delete on all lines and at least one line is
					 * configured then we need to clear the contexts for this user
					 * and deregister it.
					 */
                        /* set the trigger module as CFG */
                        p_crm_pdb->cm_trigger_module = ICF_INT_MODULE_CFG;

						icf_cfg_cm_call_clear_ind_wrt_user(p_crm_pdb, 
								&(p_recv_msg_payload->self_addr),line_id,
								(icf_boolean_t)ICF_FALSE, p_crm_pdb->p_ecode);
								
						if(ICF_SUCCESS == icf_dbm_fetch_rgn_blk(
									p_crm_pdb->p_glb_pdb,
									&(p_recv_msg_payload->self_addr),
									line_index,
									&p_rgm_context,
									p_crm_pdb->p_ecode))
						{
							icf_cfg_rgm_deregister_user_on_deletion(
								p_crm_pdb,p_rgm_context);
						}
						ret_val = icf_cfg_del_user_in_db(p_crm_pdb, 
									line_index);					
					}
                }/* if(ICF_TRUE == */                    
            }/* End loop */ 
/* End: Multiple Public Users ICF5.0*/			
        }/* End if for all_lines  and ICF_FALSE == only_priority_update..*/
#ifdef ICF_MULTILINE 
        else if((ICF_TRUE == p_recv_msg_payload->all_lines) && 
                (ICF_TRUE == p_crm_pdb->only_priority_update))
        {
            /* Only priority has to be updated */
            for(line_index = 0; line_index < (p_crm_pdb->p_config_data->
                        max_lines); line_index++)
            {
                if(ICF_NULL != p_crm_pdb->p_line_data[line_index])
                {
                    icf_cfg_update_priority(p_crm_pdb, line_index);
                }
            }/* End loop. */
        }/* End else if ICF_TRUE ==.. */
#endif

        else if(
                (ICF_ALL_LINES != p_recv_msg_payload->line_id) 
#ifdef ICF_MULTILINE 
                && (ICF_FALSE == p_crm_pdb->only_priority_update)
#endif
                )
        {
            /* If all lines is false and it is not a priority update request but
             * user has to be added */
            p_crm_pdb->rgm_pdb.all_lines = ICF_FALSE;
            line_id = p_recv_msg_payload->line_id;
/* Start: Multiple Public Users ICF5.0*/        
            /* If action is add, Get line data pointer for line id  */        
            if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
            {     
                ret_val = icf_cfg_check_line_in_db(
                        p_crm_pdb,
                        line_id,
                        line_status,
                        (icf_boolean_t) ICF_TRUE);
                
                if(ICF_SUCCESS == ret_val)
                {
                    /* Add the user on the line, increment the
                     * number of users and set the presence flag
                     */
                    ret_val = icf_cfg_add_user_in_db(p_crm_pdb, line_id);
                }/* End if ICF_SUCCESS == ret_val */
                else
                {
                    /* If failure has been returned by DBM for line blk 
                     * allocation */
                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            ICF_CAUSE_INSUFFICIENT_RESOURCES)
                    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                    return ret_val;
                } 
            }/* End if for action = add */
                 
            /* If action is delete and line is configured */
            else if ((ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
                    && (ICF_SUCCESS == icf_cfg_check_line_in_db(
                            p_crm_pdb,
                            line_id,
                            line_status,
                            (icf_boolean_t) ICF_FALSE)))
            {
                /* set the trigger module as CFG */
                p_crm_pdb->cm_trigger_module = ICF_INT_MODULE_CFG;

				/* If action is delete and if user is registered 
				 * then we have to dereg the user before deleting */
                 icf_cfg_cm_call_clear_ind_wrt_user(p_crm_pdb, 
						&(p_recv_msg_payload->self_addr),line_id,
						(icf_boolean_t)ICF_TRUE,p_crm_pdb->p_ecode);
				ret_val = icf_dbm_fetch_rgn_blk(
						p_crm_pdb->p_glb_pdb,
						&(p_recv_msg_payload->self_addr),
						line_id,
						&p_rgm_context,
						p_crm_pdb->p_ecode);

				if(ICF_SUCCESS == ret_val)
				{
                /*SPR 19793 The deregistration should not be send in the case
                 * when 401 is received for the REGISTER previously the check
                 was that if the registration state is not deregistering send
                 deregister now it is modified that if regm state is other
                 than deregister/awt_auth_resp then send deregister.
                 Added the ICF_RGM_ONCE_REGISTERED check so that deregister
                 may be sent if REGISTRATION is completed once and register 
                 refresh is challenged and then user is deleted*/
				if(1 == p_rgm_context->num_of_lines && 
                   !((ICF_RGM_05_DEREGISTERING == p_rgm_context->rgm_state) || 
                    ((ICF_RGM_06_AWT_AUTH_RESP == 
                          p_rgm_context->rgm_state) && 
                          !(ICF_RGM_ONCE_REGISTERED &
                           p_rgm_context->indicator))))
                {
                    /* SPR 16019: Set the bit mask in rgm_context to indicate that
                     * de-reg generated internally.
                         */
                    /* If de-regsitration is already ongoing then no need to 
                     * send de-register again
                     */
                    p_rgm_context->indicator |= ICF_RGM_INTERNAL_GENERATED_REG;
                    icf_rgm_deregister_user(p_crm_pdb, p_rgm_context);
                }
                else
                {	/* If we have more than one lines using this context then we shall not
                     * deregister the user but update the rgm context and remove the entry 
                     * of this line.
                     */
                    icf_cfg_update_rgm_context_remove_entry(p_crm_pdb,p_rgm_context,line_id);
                }

				}
				icf_cfg_del_user_in_db(p_crm_pdb, 
							line_id);
			}			
/* End: Multiple Public Users ICF5.0*/
        }/* End else for all_lines */
#ifdef ICF_MULTILINE 
        else
        {
            /* all lines is false and only priority has to be updated */
            ret_val = icf_cfg_update_priority(p_crm_pdb, 
                    p_recv_msg_payload->line_id);
        }
#endif
    }/* if(ICF_NULL != */
    
    /*-----------------------------------------------------------------------*/
    /* Check the presence flag to see if name is present */

    if(ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_NAME &
                p_recv_msg_payload->bit_mask))
    {

        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_NAME_N_DEF_SET_RECD)
        
        if(ICF_ALL_LINES == p_recv_msg_payload->line_id)
        {
            for(line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; 
                    line_index++)
            {
                p_crm_pdb->lines_present[line_index] = ICF_FALSE;
            }
            
            ret_val = icf_cfg_check_line_configured(p_crm_pdb,
                    (icf_boolean_t)ICF_FALSE);

            /* Now the pointers needed are taken and data can be set on the 
             * lines 
             */
            
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    /* Check if action is add */
                    if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
                    {
                        icf_port_memcpy(
                                (icf_void_t*)
                                &(p_crm_pdb->p_line_data[line_index]->
                                    user_name),
                                (icf_void_t*)&(p_recv_msg_payload->
                                                  self_name),
                                (icf_uint32_t)
                                ICF_PORT_SIZEOF(icf_name_st));
                        
                        p_crm_pdb->line_id = line_index;
                        ICF_CFG_TRACE(p_crm_pdb, 
                                ICF_CFG_TRACE_SELF_NAME_SET)
                    }/* End if for action = add */
                   
                    /* If action is delete */
                    else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                            action)
                    {
                        p_crm_pdb->p_line_data[line_index]->user_name.str_len = 
                            0;
                        icf_port_strcpy(
                            p_crm_pdb->p_line_data[line_index]->user_name.str,
                            (icf_uint8_t *)"\0");

                        p_crm_pdb->line_id = line_index;
                        ICF_CFG_TRACE(p_crm_pdb, 
                                ICF_CFG_TRACE_SELF_NAME_SET)

                    }/* End if for action = delete */
                }/* if(ICF_TRUE == */                    
            }/* End loop */  
        p_crm_pdb->line_id = ICF_ALL_LINES;              
        }/* End if for all_lines */
        
        else
        {
            line_id = p_recv_msg_payload->line_id;
                
            ret_val = icf_cfg_check_line_in_db(
                    p_crm_pdb,
                    line_id,
                    line_status,
                    (icf_boolean_t) ICF_FALSE);

            if((ICF_SUCCESS == ret_val) && (line_id < ICF_MAX_NUM_OF_LINES))
            {
                if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
                {
                    icf_port_memcpy(
                            (icf_void_t*)&(p_crm_pdb->
                                              p_line_data[line_id]->user_name),
                            (icf_void_t*)&(p_recv_msg_payload->self_name),
                            (icf_uint32_t)
                            ICF_PORT_SIZEOF(icf_name_st));

                    p_crm_pdb->line_id = line_id;
                    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_NAME_SET)
                }/* End if for action = add */

                /* If action is delete */
                else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                        action)
                {

                    p_crm_pdb->p_line_data[line_id]->user_name.str_len = 0;
                    icf_port_strcpy(
                            p_crm_pdb->p_line_data[line_id]->user_name.str,
                            (icf_uint8_t *)"\0");

                    p_crm_pdb->line_id = line_id;
                    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_NAME_SET)
                }/* End if for action = delete */
            }/* End if ICF_SUCCESS == ret_val */
        }/* End else for all_lines */
    }/* if(ICF_NULL != */
    
    /*-----------------------------------------------------------------------*/
    if (ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_DEFAULT_SETTINGS &
                p_recv_msg_payload->bit_mask))
    {
        /* If default settings is present in payload */
        if (ICF_ALL_LINES == p_recv_msg_payload->line_id)
        {
            /* Get line data pointer for all lines configured till now */
            
            for(line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; 
                    line_index++)
            {
                p_crm_pdb->lines_present[line_index] = ICF_FALSE;
            }
            
            ret_val = icf_cfg_check_line_configured(p_crm_pdb,
                    (icf_boolean_t)ICF_FALSE);

            /* Now the pointers needed are taken and data can be set on the 
             * lines 
             */

            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    p_crm_pdb->p_line_data[line_index]->default_settings =
                            p_recv_msg_payload->default_settings;
                    p_crm_pdb->line_id = line_index;
                    ICF_CFG_TRACE(p_crm_pdb, 
                            ICF_CFG_TRACE_SELF_DEFAULT_SETTINGS_SET)
                
                }/* if(ICF_TRUE == */
            }/* End loop */
        p_crm_pdb->line_id = ICF_ALL_LINES; 
        }/* End if for all_lines */
        
        else
        {
            line_id = p_recv_msg_payload->line_id;
                
            ret_val = icf_cfg_check_line_in_db(
                    p_crm_pdb,
                    line_id,
                    line_status,
                    (icf_boolean_t) ICF_FALSE);

            if((ICF_SUCCESS == ret_val) && (line_id < ICF_MAX_NUM_OF_LINES))
            {
                p_crm_pdb->p_line_data[line_id]->default_settings = 
                        p_recv_msg_payload->default_settings; 
                p_crm_pdb->line_id = line_id;
                ICF_CFG_TRACE(p_crm_pdb, 
                        ICF_CFG_TRACE_SELF_DEFAULT_SETTINGS_SET)
            }
        }/* End else for all_lines */
    }/* End If for default_settings */

    /*  CSR 1-5599924 Merging */
    /* Check the presence flag to see if AUTH_PER_USER is present */

    /* Earlier, ICF_USER_DB_AUTH_PRMS_PER_USER bit was set in user-db only if 
     * auth_key & auth_passwd bits were also recd.
     * As a correction to this, even if auth_key & auth_passwd are not 
     * received, this bit should still be stored to decide whether credentials 
     * should be fetched from the line data or not if they are not in user-db.
     * This will help if App configures a user without credentials and does not
     * want to use the line credentials also.
     */
    if ((ICF_NULL != (ICF_SET_SLF_ID_PRM_AUTH_PER_USER & 
			p_recv_msg_payload->bit_mask)) && 
	(ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_ADDR & 
			p_recv_msg_payload->bit_mask)))
    {
        if (ICF_ALL_LINES == p_recv_msg_payload->line_id)
        {
            for(line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; 
                    line_index++)
            {
                p_crm_pdb->lines_present[line_index] = ICF_FALSE;
            }
            
            ret_val = icf_cfg_check_line_configured(p_crm_pdb,
                    (icf_boolean_t)ICF_FALSE);

            /* Now the pointers needed are taken and data can be set on the 
             * user these lines 
             */
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if ((ICF_TRUE == p_crm_pdb->lines_present[line_index]) &&
				    (ICF_SUCCESS == 
						icf_dbm_get_public_uri_node_from_uri_for_line(
						    p_crm_pdb->p_glb_pdb,
						    line_index,
						    &p_recv_msg_payload->self_addr,
						    ICF_USER_DB_USE_PUBLIC_URIS,
						    &p_user_db,
						    p_crm_pdb->p_ecode)))
                {
		    p_user_db->flags |= ICF_USER_DB_AUTH_PRMS_PER_USER;
                }
            }
            p_crm_pdb->line_id = ICF_ALL_LINES;
        }/* End if for all_lines */
        else
        {
            line_id = p_recv_msg_payload->line_id;
                
            if ((ICF_SUCCESS == icf_cfg_check_line_in_db(
				    p_crm_pdb, 
				    line_id, 
				    line_status, 
				    (icf_boolean_t) ICF_FALSE)) && 
		(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line(
				    p_crm_pdb->p_glb_pdb,
				    line_id,
				    &p_recv_msg_payload->self_addr,
				    ICF_USER_DB_USE_PUBLIC_URIS,
				    &p_user_db,
				    p_crm_pdb->p_ecode)))
            {
	        p_user_db->flags |= ICF_USER_DB_AUTH_PRMS_PER_USER;
            }
            p_crm_pdb->line_id = line_id;
        }/* End else for all_lines */
    }/* if(ICF_NULL != AUTH_PER_USER*/

    /* This is to  for the authentication key */
    if(ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_AUTH_KEY &
                p_recv_msg_payload->bit_mask))
    {

        if(ICF_ALL_LINES == p_recv_msg_payload->line_id)
        {
            for(line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; 
                    line_index++)
            {
                p_crm_pdb->lines_present[line_index] = ICF_FALSE;
            }
            
            ret_val = icf_cfg_check_line_configured(p_crm_pdb,
                    (icf_boolean_t)ICF_FALSE);

            /* Now the pointers needed are taken and data can be set on the 
             * lines 
             */
            
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    /* If the PA indicates that we have to add these
                     * creadentials only for the user then we add them 
                     * only in user db and not in line_data*/
                    if(p_recv_msg_payload->bit_mask & 
                            ICF_SET_SLF_ID_PRM_AUTH_PER_USER)
                    {
                        icf_cfg_process_auth_key_for_user(
                                p_crm_pdb->p_glb_pdb,
                                p_recv_msg_payload);
                    }
                    else
                    {
                        /* If the PA indicates that we have to add these
                         * creadentials only for the user then we add them 
                         * only in user db and not in line_data*/
                        if(p_recv_msg_payload->bit_mask & 
                            ICF_SET_SLF_ID_PRM_AUTH_PER_USER)
                        {
                            icf_cfg_process_auth_key_for_user(
                                p_crm_pdb->p_glb_pdb,
                                p_recv_msg_payload);
                        }
                        else
                        {
                            /* Check if action is add */
                            if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
                            {
                                p_crm_pdb->p_line_data[line_index]->
                                    authentication_key.str_len = 
                                    p_recv_msg_payload->authentication_key.str_len;

                                icf_port_strcpy(p_crm_pdb->p_line_data[line_index]->
                                        authentication_key.str,
                                        p_recv_msg_payload->authentication_key.str);

                                ICF_CFG_SET_BITMASK(
                                        p_crm_pdb->p_line_data[line_index]->line_data_received,
                                        ICF_CFG_LINE_DATA_AUTH_KEY_RECD)

                                    p_crm_pdb->line_id = line_index;

                            }/* End if for action = add */

                            /* If action is delete */
                            else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                                    action)
                            {
                                p_crm_pdb->p_line_data[line_index]->
                                    authentication_key.str_len = 0;
                                icf_port_strcpy(
                                        p_crm_pdb->p_line_data[line_index]->authentication_key.str,
                                        (icf_uint8_t *)"\0");

                                p_crm_pdb->line_id = line_index;

                                ICF_CFG_RESET_BITMASK(
                                        p_crm_pdb->p_line_data[line_index]->line_data_received,
                                        ICF_CFG_LINE_DATA_AUTH_KEY_RECD)


                            }/* End if for action = delete */
                        }
                    }/* if(ICF_TRUE == */                    
                }
            }/* End loop */ 
        p_crm_pdb->line_id = ICF_ALL_LINES;                 
        }/* End if for all_lines */
        
        else
        {
            line_id = p_recv_msg_payload->line_id;
                
            ret_val = icf_cfg_check_line_in_db(
                    p_crm_pdb,
                    line_id,
                    line_status,
                    (icf_boolean_t) ICF_FALSE);

            if((ICF_SUCCESS == ret_val) && (line_id < ICF_MAX_NUM_OF_LINES))
            {
                /* If the PA indicates that we have to add these
                 * creadentials only for the user then we add them 
                 * only in user db and not in line_data*/
                if(p_recv_msg_payload->bit_mask & 
                        ICF_SET_SLF_ID_PRM_AUTH_PER_USER)
                {
                    icf_cfg_process_auth_key_for_user(
                            p_crm_pdb->p_glb_pdb,
                            p_recv_msg_payload);
                }
                else
                {
                    if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
                    {
                        p_crm_pdb->p_line_data[line_id]->
                            authentication_key.str_len = 
                            p_recv_msg_payload->authentication_key.str_len;

                        icf_port_strcpy(
                                p_crm_pdb->p_line_data[line_id]->authentication_key.str,
                                p_recv_msg_payload->authentication_key.str);

                        ICF_CFG_SET_BITMASK(
                                p_crm_pdb->p_line_data[line_id]->line_data_received,
                                ICF_CFG_LINE_DATA_AUTH_KEY_RECD)

                         p_crm_pdb->line_id = line_id;

                    }/* End if for action = add */

                    /* If action is delete */
                    else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                            action)
                    {

                        p_crm_pdb->p_line_data[line_id]->authentication_key.str_len = 0;
                        icf_port_strcpy(p_crm_pdb->p_line_data[line_id]->
                                authentication_key.str,
                                (icf_uint8_t *)"\0");

                        p_crm_pdb->line_id = line_id;

                        ICF_CFG_RESET_BITMASK(
                                p_crm_pdb->p_line_data[line_id]->line_data_received,
                                ICF_CFG_LINE_DATA_AUTH_KEY_RECD)

                    }/* End if for action = delete */
                }
            }/* End if ICF_SUCCESS == ret_val */
        }/* End else for all_lines */
    }/* if(ICF_NULL != AUTH_KEY*/


    if(ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_AUTH_PASSWORD &
                p_recv_msg_payload->bit_mask))
    {

        if(ICF_ALL_LINES == p_recv_msg_payload->line_id)
        {
            for(line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; 
                    line_index++)
            {
                p_crm_pdb->lines_present[line_index] = ICF_FALSE;
            }
            
            ret_val = icf_cfg_check_line_configured(p_crm_pdb,
                    (icf_boolean_t)ICF_FALSE);

            /* Now the pointers needed are taken and data can be set on the 
             * lines 
             */
            
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES) ; line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    /* If the PA indicates that we have to add these
                     * creadentials only for the user then we add them 
                     * only in user db and not in line_data*/
                    if(p_recv_msg_payload->bit_mask & 
                            ICF_SET_SLF_ID_PRM_AUTH_PER_USER)
                    {
                        icf_cfg_process_auth_passwd_for_user(
                                p_crm_pdb->p_glb_pdb,
                                p_recv_msg_payload);
                    }
                    else
                    {
                        /* Check if action is add */
                        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
                        {
                            p_crm_pdb->p_line_data[line_index]->
                                authentication_password.str_len = 
                                p_recv_msg_payload->authentication_password.str_len;

                            icf_port_strcpy(p_crm_pdb->p_line_data[line_index]->
                                    authentication_password.str,
                                    p_recv_msg_payload->authentication_password.str);

                            if (ICF_NULL != p_recv_msg_payload->authentication_password.str_len)
                                ICF_CFG_SET_BITMASK(
                                        p_crm_pdb->p_line_data[line_index]->line_data_received,
                                        ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)
                            else
                                ICF_CFG_RESET_BITMASK(
                                        p_crm_pdb->p_line_data[line_index]->line_data_received,
                                        ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)

                                    p_crm_pdb->line_id = line_index;

                        }/* End if for action = add */

                        /* If action is delete */
                        else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                                action)
                        {
                            p_crm_pdb->p_line_data[line_index]->
                                authentication_password.str_len = 0;
                            icf_port_strcpy(p_crm_pdb->p_line_data[line_index]->
                                    authentication_password.str,
                                    (icf_uint8_t *)"\0");

                            p_crm_pdb->line_id = line_index;

                            ICF_CFG_RESET_BITMASK(p_crm_pdb->
                                    p_line_data[line_index]->line_data_received,
                                    ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)


                        }/* End if for action = delete */
                    }
                }/* if(ICF_TRUE == */                    
            }/* End loop */ 
        p_crm_pdb->line_id = ICF_ALL_LINES;               
        }/* End if for all_lines */
        
        else
        {
            line_id = p_recv_msg_payload->line_id;
                
            ret_val = icf_cfg_check_line_in_db(
                    p_crm_pdb,
                    line_id,
                    line_status,
                    (icf_boolean_t) ICF_FALSE);

            if((ICF_SUCCESS == ret_val) && (line_id < ICF_MAX_NUM_OF_LINES))
            {
                /* If the PA indicates that we have to add these
                 * creadentials only for the user then we add them 
                 * only in user db and not in line_data*/
                if(p_recv_msg_payload->bit_mask & 
                        ICF_SET_SLF_ID_PRM_AUTH_PER_USER)
                {
                    icf_cfg_process_auth_passwd_for_user(
                            p_crm_pdb->p_glb_pdb,
                            p_recv_msg_payload);
                }
                else
                {
                    if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
                    {

                        p_crm_pdb->p_line_data[line_id]->
                            authentication_password.str_len = 
                            p_recv_msg_payload->authentication_password.str_len;

                        icf_port_strcpy(
                                p_crm_pdb->p_line_data[line_id]->authentication_password.str,
                                p_recv_msg_payload->authentication_password.str);

                        if (ICF_NULL != p_recv_msg_payload->authentication_password.str_len)
                            ICF_CFG_SET_BITMASK(
                                    p_crm_pdb->p_line_data[line_id]->line_data_received,
                                    ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)
                        else
                            ICF_CFG_RESET_BITMASK(
                                    p_crm_pdb->p_line_data[line_id]->line_data_received,
                                    ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)

                         p_crm_pdb->line_id = line_id;

                    }/* End if for action = add */

                    /* If action is delete */
                    else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                            action)
                    {

                        p_crm_pdb->p_line_data[line_id]->
                            authentication_password.str_len = 0;
                        icf_port_strcpy(p_crm_pdb->p_line_data[line_id]->
                                authentication_password.str,
                            (icf_uint8_t *)"\0");

                        p_crm_pdb->line_id = line_id;

                        ICF_CFG_RESET_BITMASK(
                                p_crm_pdb->p_line_data[line_id]->line_data_received,
                                ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)

                    }/* End if for action = delete */
                }
            }/* End if ICF_SUCCESS == ret_val */
        }/* End else for all_lines */
    }/* if(ICF_NULL != AUTH_PASSWORD*/

    if(ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_REALM &
                p_recv_msg_payload->bit_mask))
    {
        if(ICF_ALL_LINES == p_recv_msg_payload->line_id)
        {
            for(line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; 
                    line_index++)
            {
                p_crm_pdb->lines_present[line_index] = ICF_FALSE;
            }

            ret_val = icf_cfg_check_line_configured(p_crm_pdb,
                    (icf_boolean_t)ICF_FALSE);

            /* Now the pointers needed are taken and data can be set on the lines */
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES) ; line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                  /* If the PA indicates to add realm only for the
                  * user then realm is added only in user db and 
                  * not in line_data*/
                  if(p_recv_msg_payload->bit_mask & 
                        ICF_SET_SLF_ID_PRM_REALM_PER_USER)
                  {
                    icf_cfg_process_realm_for_user(
                              p_crm_pdb->p_glb_pdb,
                              p_recv_msg_payload);
                  }
                  else
                  {
                    /* Check if action is add */
                    if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
                    {
                         p_crm_pdb->p_line_data[line_index]->
                             configured_realm.str_len = 
                        p_recv_msg_payload->configured_realm.str_len;

                        icf_port_strcpy(p_crm_pdb->p_line_data[line_index]->
 	                               		configured_realm.str,
    			                    	p_recv_msg_payload->configured_realm.str);

                        ICF_CFG_SET_BITMASK(
 	                        p_crm_pdb->p_line_data[line_index]->line_data_received,
    	                    ICF_CFG_LINE_DATA_REALM_RECD)

                       p_crm_pdb->line_id = line_index;

                    }/* End if for action = add */
                    /* If action is delete */
					else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
                    {
                        p_crm_pdb->p_line_data[line_index]->
                            configured_realm.str_len = 0;

                        icf_port_strcpy(
                            p_crm_pdb->p_line_data[line_index]->configured_realm.str,
                            (icf_uint8_t *)"\0");

                        p_crm_pdb->line_id = line_index;

                        ICF_CFG_RESET_BITMASK(
                        p_crm_pdb->p_line_data[line_index]->line_data_received,
                        ICF_CFG_LINE_DATA_REALM_RECD)

                    }/* End if for action = delete */
                  }/*End of else for ICF_SET_SLF_ID_PRM_REALM_PER_USER*/  
                }/* if(ICF_TRUE == */                    
            }/* End loop */ 
        p_crm_pdb->line_id = ICF_ALL_LINES;               
        }/* End if for all_lines */
        else
        {
            line_id = p_recv_msg_payload->line_id;

            ret_val = icf_cfg_check_line_in_db(
                    					p_crm_pdb,
					                    line_id,
					                    line_status,
                    					(icf_boolean_t) ICF_FALSE);

            if((ICF_SUCCESS == ret_val) && (line_id < ICF_MAX_NUM_OF_LINES))
            {
             /* If the PA indicates to add realm only for the
              * user then realm is added only in user db and 
              * not in line_data*/
             if(p_recv_msg_payload->bit_mask & 
                 ICF_SET_SLF_ID_PRM_REALM_PER_USER)
             {
                icf_cfg_process_realm_for_user(
                       p_crm_pdb->p_glb_pdb,
                       p_recv_msg_payload);
             }
             else
             {
                if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
                {
                    p_crm_pdb->p_line_data[line_id]->
                       configured_realm.str_len = 
 		                   p_recv_msg_payload->configured_realm.str_len;

                    icf_port_strcpy(
                        p_crm_pdb->p_line_data[line_id]->configured_realm.str,
                        p_recv_msg_payload->configured_realm.str);

                    ICF_CFG_SET_BITMASK(
                    	p_crm_pdb->p_line_data[line_id]->line_data_received,
                    	ICF_CFG_LINE_DATA_REALM_RECD)

                    p_crm_pdb->line_id = line_id;

                }/* End if for action = add */

                /* If action is delete */

                else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                        action)

                {
                    p_crm_pdb->p_line_data[line_id]->
                        configured_realm.str_len = 0;

                    icf_port_strcpy(
                        p_crm_pdb->p_line_data[line_id]->configured_realm.str,
                        (icf_uint8_t *)"\0");

                    p_crm_pdb->line_id = line_id;

                    ICF_CFG_RESET_BITMASK(
                    p_crm_pdb->p_line_data[line_id]->line_data_received,
                    ICF_CFG_LINE_DATA_REALM_RECD)
                        
                }/* End if for action = delete */
              }/*End of else for ICF_SET_SLF_ID_PRM_REALM_PER_USER*/  
            }/* End if ICF_SUCCESS == ret_val */
        }/* End else for all_lines */
    }/* if(ICF_NULL != CONFIGURED_REALM*/


#ifndef ICF_FEATURE_ABANDON
  
    if((ICF_TRUE == p_crm_pdb->global_data_init_complete) && 
            (ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_ADDR & 
                             p_recv_msg_payload->bit_mask)))
    {
        /* If Init complete is done and self address was recd in payload, 
         * Invoke MWI */
        ret_val = icf_cfg_mwi_trigger(p_crm_pdb, 
                (icf_uint8_t)ICF_CFG_SUBSCRIBE_TRIGGER);
    }
#endif
    /* Delete the line now */
    if((ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action) && 
            (ICF_NULL != (ICF_CFG_CC_SET_SLF_ID_PRM_ADDR & 
                             p_recv_msg_payload->bit_mask)))
    {
        if(ICF_ALL_LINES == p_recv_msg_payload->line_id)
        {
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    icf_cfg_delete_line(p_crm_pdb, line_index);
                }
            }
        }
        else
        {
            /* All lines = false */
			if ( line_id < ICF_MAX_NUM_OF_LINES)
            if (ICF_NULL != p_crm_pdb->p_line_data[line_id])
            {
                icf_cfg_delete_line(p_crm_pdb, line_id);
            }
        }
    }

    /* If init complete is not done, and Self IP and Port related configuration has
     * beent received from the Application then open the network channel and
     * this will be treated as Init complete towards ICF*/
     if (ICF_CFG_SELF_ADDR_PER_LINE ==
        p_crm_pdb->p_config_data->self_addr_per_line)
     {
	     icf_cfg_check_init_complete_for_line(p_crm_pdb);
	     if(ICF_TRUE == p_crm_pdb->is_valid_param)
	     {
		     ret_val = icf_cfg_init_complete_hdlr_for_line(p_crm_pdb);

		     if(ICF_FAILURE == ret_val)
		     {
			     p_crm_pdb->result = ICF_FAILURE;
		     }
            p_crm_pdb->resp_to_be_sent = ICF_TRUE;
         }
     }
     else if (ICF_CFG_SELF_ADDR_SYS_WIDE &
		     p_crm_pdb->p_config_data->self_addr_per_line)
     {
	     if(ICF_FALSE == p_crm_pdb->p_glb_pdb->p_glb_data->
			     global_data_init_complete)
	     {
		     if(ICF_TRANSPORT_ADDRESS_INVALID != p_crm_pdb->p_config_data->
				     self_ip_address.addr.addr_type)
		     {
		     ret_val = icf_cfg_init_complete_hdlr(p_crm_pdb);
			     if(ICF_FAILURE == ret_val)
			     {
				     p_crm_pdb->result = ICF_FAILURE;
			     }
			     p_crm_pdb->resp_to_be_sent = ICF_TRUE;
		     }
	     }
     }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;                
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_add_regm_prms
 *
 * DESCRIPTION:     This function validates the ADD action of the data 
 *                  received in the Set reg params API. 
 *                  
 * RETURNS:         ICF_SUCCESS always
 *                  
******************************************************************************/
icf_return_t icf_cfg_validate_add_regm_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      line_status[ICF_MAX_NUM_OF_LINES];
    icf_set_registration_req_st *p_recv_msg_payload = 
                                                    ICF_NULL;
    
    icf_line_data_st  *p_line_data = ICF_NULL;
    icf_transport_mode_t  self_mode = 0;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* copy the external request payload pointer locally */  
    p_recv_msg_payload = (icf_set_registration_req_st *)
        (p_crm_pdb->p_recv_payload);
        
    p_crm_pdb->is_valid_param = ICF_TRUE;

    if (ICF_CFG_SELF_ADDR_PER_LINE ==
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
        if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, line_id,
                    &p_line_data, p_crm_pdb->p_ecode))
        {
            /* This function always returns SUCCESS,
             * thus set valid_params to FALSE */
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
		    ret_val = ICF_SUCCESS;
        }
        else
        {
            self_mode = p_line_data->self_mode;
        }
    }
    else if (ICF_CFG_SELF_ADDR_SYS_WIDE ==
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
         self_mode = p_crm_pdb->p_config_data->self_mode;
    }
    else
    {
        /*If self address is not configured then return failure*/
        /* This function always returns SUCCESS,
        * thus set valid_params to FALSE */
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_SELF_ADDRESS_NOT_CONFIGURED)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ret_val = ICF_SUCCESS;
    }

    /* If valid_params is FALSE, implies some error has occurred */
    if (ICF_FALSE ==  p_crm_pdb->is_valid_param)
    {
    }
    else if ((ICF_NULL != p_recv_msg_payload->registrar_addr.port_num) &&
       (ICF_CFG_MIN_PORT_VALUE > p_recv_msg_payload->registrar_addr.port_num))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_IP_PORT_OUT_OF_RANGE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    /***************ICF 8.0**************************/
    /*If the operation mode is IMS and reg route address is configured set the
      failure response as ICF_CAUSE_REG_ROUTE_NOT_REQD*/
    else if ((ICF_CFG_CC_SET_REG_REQ_ROUTE_ADDR & 
                p_recv_msg_payload->bit_mask) && p_crm_pdb->p_config_data->\
            ims_oprn_flag == ICF_OPERATION_MODE_IMS )
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_REG_ROUTE_NOT_REQD)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ret_val = ICF_SUCCESS;
    }    
    else if ((ICF_CFG_CC_SET_REG_REQ_ROUTE_ADDR & 
                p_recv_msg_payload->bit_mask) && 
			 (ICF_NULL != p_recv_msg_payload->reg_route_addr.port_num) &&
             (ICF_CFG_MIN_PORT_VALUE > 
                p_recv_msg_payload->reg_route_addr.port_num))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_IP_PORT_OUT_OF_RANGE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    else if(((self_mode != ICF_TRANSPORT_MODE_BOTH)
             && (p_recv_msg_payload->reg_mode != ICF_TRANSPORT_MODE_BOTH)) 
	     && (self_mode != ICF_TRANSPORT_TYPE_TLS))
    {
            /*  If user itself gave the transport type INVALID 
                to do naptr query THen allow it*/
	    if((ICF_TRANSPORT_TYPE_INVALID != p_recv_msg_payload->reg_mode) &&
               ((p_recv_msg_payload->reg_mode != self_mode)||
		(ICF_TRANSPORT_TYPE_TLS == p_recv_msg_payload->reg_mode)))
	    {        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
           		ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE);				
	        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ret_val = ICF_SUCCESS;
        }
    }    
    if (ICF_TRUE ==  p_crm_pdb->is_valid_param)
    { 
    	if(ICF_SUCCESS == icf_cfg_check_line_in_db(
			    p_crm_pdb, line_id, line_status,
			    (icf_boolean_t) ICF_TRUE))
	    {
	        /* If pointer is returned by DBM i.e line is configured */
	        /* Check if registrar is already present on the line */
	        if (ICF_NULL != (ICF_CFG_LINE_DATA_REGISTRAR_RECD 
			    & p_crm_pdb->p_line_data[line_id]->line_data_received))
	        {
                /* If the primary already configured on line
                 * is the same as the one in API, then allow,
                 * else raise error for primary reg. already confgd */
                if (ICF_FAILURE == icf_cmn_compare_transport_addresses(
                            &(p_crm_pdb->p_line_data[line_id]->registrar_addr),
                            &(p_recv_msg_payload->registrar_addr),
                            p_crm_pdb->p_glb_pdb))
                {
		            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
		            ICF_CFG_SET_RESPONSE(
			            p_crm_pdb,
			            ICF_FAILURE,
			            ICF_CAUSE_REGISTRAR_ALREADY_CONFIGURED)
		            ret_val = ICF_SUCCESS;
                }
            }
            /* When secondary registrar is also being ADDed on the 
             * same line or ALL_LINES as the primary in the API, 
             * return FAILURE if they are same*/
            else if ((ICF_NULL != (ICF_SET_REG_SEC_REGISTRAR &
                            p_recv_msg_payload->bit_mask)) &&
                    ((ICF_CONFIG_ACTION_ADD == 
                      p_recv_msg_payload->sec_registrar_addr.action)) &&
                    ((line_id == p_recv_msg_payload->sec_registrar_addr.line_id) ||
                     (ICF_ALL_LINES == p_recv_msg_payload->sec_registrar_addr.line_id)))
            {
                /* Check if primary and secondary in the API are same */
                if (ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                            &(p_recv_msg_payload->sec_registrar_addr.transport_addr),
                            &(p_recv_msg_payload->registrar_addr),
                            p_crm_pdb->p_glb_pdb))
                {
                    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                    ICF_CFG_SET_RESPONSE(p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_REGISTRAR_ADDR)
                    ret_val = ICF_SUCCESS;
                }
            }
        }/* End if(ICF_SUCCESS== icf_cfg_check_line_in_db */
        else
        {
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INVALID_LINE_ID)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ret_val = ICF_SUCCESS;
        }/* End if (FAILURE == icf_cfg_check_line_in_db */
    } 
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_add_sec_regm_prms
 *
 * DESCRIPTION:     This function validates the ADD action for secondary 
 *                  registrar in the Set reg params API. 
 *                  
 * RETURNS:         ICF_SUCCESS always
 *                  
******************************************************************************/
icf_return_t icf_cfg_validate_add_sec_regm_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      line_status[ICF_MAX_NUM_OF_LINES];
    icf_set_registration_req_st *p_recv_msg_payload = 
                                                    ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* copy the external request payload pointer locally */  
    p_recv_msg_payload = (icf_set_registration_req_st *)
        (p_crm_pdb->p_recv_payload);
        
    p_crm_pdb->is_valid_param = ICF_TRUE;

    if(ICF_SUCCESS == icf_cfg_check_line_in_db(
                p_crm_pdb, line_id, line_status,
                (icf_boolean_t) ICF_FALSE))
    {
        /*return failure if secondary registrar is already present on line*/
        if (ICF_NULL != (ICF_CFG_LINE_DATA_SEC_REGISTRAR_RECD &
                    p_crm_pdb->p_line_data[line_id]->line_data_received))
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_SEC_REGISTRAR_ALREADY_CONFIGURED)
            ret_val = ICF_SUCCESS;
        }/* end if - secondary already present */
         /* If pointer is returned by DBM i.e line is configured */
        /* If primary is being ADDed on the same line or ALL_LINES
         * in the same API where secondary is being ADDed, validation
         * has already been done to return FAILURE is they are same
         * while validating primary proxy in API, so no need to chk again*/
        else if ((ICF_CONFIG_ACTION_ADD == 
                    p_recv_msg_payload->action) &&
                ((line_id == p_recv_msg_payload->line_id) || 
                 (ICF_ALL_LINES == p_recv_msg_payload->line_id)))
        {
            /* Do nothing - Valid behaviour
             * as primary is being configured on same/ALL lines
             * in the same API as secondary.
             * No need to validate if primary and secondary in
             * API are same on same/ALL lines. 
             * Already done in validation of prim. */
        }
        /* return failure if proxy not present on line */
        else if (ICF_NULL == (ICF_CFG_LINE_DATA_REGISTRAR_RECD & 
                    p_crm_pdb->p_line_data[line_id]->line_data_received))
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_PRIMARY_REGISTRAR_NOT_CONFIGURED)
            ret_val = ICF_SUCCESS;
        }/* end if primary not present */
        /* return failure if primary registrar configured on line
         * is the same as secondary registrar */
        else if (ICF_SUCCESS == icf_cmn_compare_transport_addresses(
                    &(p_crm_pdb->p_line_data[line_id]->registrar_addr),
                    &(p_recv_msg_payload->sec_registrar_addr.transport_addr),
                    p_crm_pdb->p_glb_pdb))
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                        ICF_CAUSE_SAME_PRIMARY_AND_SECONDARY_REGISTRAR_ADDR)
            ret_val = ICF_SUCCESS;
        }/* end else if primary on line same as secondary */
    }/* End if(ICF_SUCCESS== icf_cfg_check_line_in_db */
    else
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_LINE_ID)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ret_val = ICF_SUCCESS;
    }/* End if (FAILURE == icf_cfg_check_line_in_db */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_regm_prms
 *
 * DESCRIPTION:     This function validates the data received in the Set reg 
 *                  params API. 
 *                  
 * RETURNS:         ICF_SUCCESS always
 *                  
******************************************************************************/
icf_return_t icf_cfg_validate_regm_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_set_registration_req_st          *p_recv_msg_payload = 
                                                    ICF_NULL;
    icf_line_id_t                        line_index = 0;
    icf_line_id_t                        line_id = 0; 
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
  
    /* copy the external request payload pointer locally */  
    p_recv_msg_payload = (icf_set_registration_req_st *)
        (p_crm_pdb->p_recv_payload);
     
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_REG_PRMS_REQ_RECD)
    ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_REG_ADDR_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_REG_PRMS_RECD)
    
    if (ICF_ALL_LINES == p_recv_msg_payload->line_id)
    {
        /* Fix for SPR 18616. Validations added for action 'Modify' for primary
           registrar address.
        */
        if(ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->action)
        {
            for(line_index = 0;line_index < (p_crm_pdb->p_config_data->
                        max_lines);line_index++)
            {
                ret_val = icf_cfg_validate_modify_regm_prms(
                                          p_crm_pdb,line_index,(icf_boolean_t)ICF_TRUE);
                if(ICF_FALSE == p_crm_pdb->is_valid_param)
                {
                   /* If the above function has set failure,break */
                   break;
                } 
            }
        }/* end if action is modify */                 

        /* If action is ADD then no registrar address must be present on all 
         * the lines configured till mow. If no line is configured till now
         * then, return success as all lines will be configured now and 
         * registrar will be added on all the lines 
         */
        else if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            for (line_index = 0; line_index < (p_crm_pdb->p_config_data->
                        max_lines); line_index++)
            {
                ret_val = icf_cfg_validate_add_regm_prms(p_crm_pdb,
                        line_index);

                if (ICF_FALSE == p_crm_pdb->is_valid_param)
                {
                    break; /* break from for loop for ALL_LINES on failure */
                }
            }/* for(line_index = .. */ 

            /* where ever failure in validation will occur it will break out 
             * from the loop.If the parameters are valid then set the 
             * all_lines field in the rgm_pdb in the crm_pdb to true.
             * This will be used by RGM in case the API is received after init
             * complete. If the API is received before init complete it
             * will not be used
             */
            if(ICF_TRUE == p_crm_pdb->is_valid_param) 
            {
                p_crm_pdb->rgm_pdb.all_lines = ICF_TRUE;
            }
        }/* if(ICF_CONFIG_ACTION_ADD ==.. */

        else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
        {
            icf_line_data_st *p_line_data = ICF_NULL;
            for (line_index = 0; line_index < (p_crm_pdb->p_config_data->
                        max_lines); line_index++)
            {
                if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                            p_crm_pdb->p_glb_pdb, line_index,
                            &p_line_data, p_crm_pdb->p_ecode))
                {
                    /* Validate the reg params if the line is present
                       in the system */

                    ret_val = icf_cfg_validate_del_regm_prms
                        (p_crm_pdb, line_index);
                    if (ICF_FALSE == p_crm_pdb->is_valid_param)
                    {
                        break;/* break from for loop for ALL_LINES on failure */
                    }
                }/* end if SUCCESS == icf_dbm_fetch_line_cntxt_blk */
            }/* end for ALL_LINES */
            if(ICF_TRUE == p_crm_pdb->is_valid_param) 
            {
                p_crm_pdb->rgm_pdb.all_lines = ICF_TRUE;
            }
        }/* end if for action = MODIFY */
    }/* End if for all_lines */
    else /* not all_lines */
    {
        p_crm_pdb->rgm_pdb.all_lines = ICF_FALSE;
        line_id = p_recv_msg_payload->line_id;

        /* Validate the line id */
        if(line_id > p_crm_pdb->p_config_data->max_lines)
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
            ret_val = ICF_SUCCESS;
        }
        /* SPR 18616 fix, added validations for action 'Modify' for 
           primary registrar address */
        else if(ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->action)
        {
             ret_val = icf_cfg_validate_modify_regm_prms(
                                          p_crm_pdb,line_id,(icf_boolean_t)ICF_TRUE);
        } 
        /* Check the action */
        else if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            ret_val = icf_cfg_validate_add_regm_prms(p_crm_pdb, line_id);

        }/* if(ICF_CONFIG_ACTION_ADD .. */ 
        else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
        {
            ret_val = icf_cfg_validate_del_regm_prms(p_crm_pdb, line_id);
        }
    }/* End else for not all_lines */
    /* Validation of secondary registrar configuration - if present */
    if (ICF_TRUE == p_crm_pdb->is_valid_param)
    {
        if(ICF_NULL !=(p_recv_msg_payload->bit_mask & 
                    ICF_SET_REG_SEC_REGISTRAR))
        {
            /* Secondary configuration on ALL_LINES */
            if (ICF_ALL_LINES == p_recv_msg_payload->sec_registrar_addr.line_id)
            {
                /* Fix for SPR 18616. Validations added for action 'Modify' for secondary
                   registrar address.
                */
                if(ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->action)
                {
                    for(line_index = 0;line_index < (p_crm_pdb->p_config_data->
                        max_lines);line_index++)
                    {
                        ret_val = icf_cfg_validate_modify_regm_prms(
                                          p_crm_pdb,line_index,(icf_boolean_t)ICF_FALSE);
                        if(ICF_FALSE == p_crm_pdb->is_valid_param)
                        {
                            /* If the above function has set failure,break */
                            break;
                        } 
                    }   
                }/* end if action is modify */      
                /* for loop placed inside actions for efficiency.
                 * Once action is checked the corresponding function
                 * is called one by one for all lines upto max lines*/

                else if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                        sec_registrar_addr.action)
                {
                    for (line_index = 0; line_index < (p_crm_pdb->
                                p_config_data->max_lines); line_index++)
                    {
                        ret_val = icf_cfg_validate_add_sec_regm_prms(p_crm_pdb,
                                line_index);

                        if (ICF_FALSE == p_crm_pdb->is_valid_param)
                        {
                            break;
                        }
                    }/* for(line_index = .. */ 

                    /* where ever failure in validation will occur it will 
                     * break out from the loop.If the parameters are valid 
                     * then set the all_lines field in the rgm_pdb in the 
                     * crm_pdb to true.
                     * This will be used by RGM in case API is received after 
                     * init complete. If the API is received before init 
                     * complete it will not be used
                     */
                    if(ICF_TRUE == p_crm_pdb->is_valid_param) 
                    {
                        p_crm_pdb->rgm_pdb.all_lines = ICF_TRUE;
                    }
                }/* end if - action ADD */
                else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                        sec_registrar_addr.action)
                {
                    icf_line_data_st *p_line_data = ICF_NULL;
                    for (line_index = 0; line_index < (p_crm_pdb->
                                p_config_data->max_lines); line_index++)
                    {
                        if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                                    p_crm_pdb->p_glb_pdb, line_index,
                                    &p_line_data, p_crm_pdb->p_ecode))
                        {
                            /* Validate the reg params if the line is present
                               in the system */

                            ret_val = icf_cfg_validate_del_sec_regm_prms(
                                    p_crm_pdb, line_index);
                            if (ICF_FALSE == p_crm_pdb->is_valid_param)
                            {
                                break;
                            }
                        }
                    }/* end for loop - line index */
                    if(ICF_TRUE == p_crm_pdb->is_valid_param) 
                    {
                        p_crm_pdb->rgm_pdb.all_lines = ICF_TRUE;
                    }
                }/* end if - action DELETE */
            }/* end if ALL_LINES */
            
            /* Secondary configuration on one line */
            else /* not ALL_LINES */    
            {
                line_id = p_recv_msg_payload->sec_registrar_addr.line_id;

                /* Validate the line id */
                if(line_id > p_crm_pdb->p_config_data->max_lines)
                {
                    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                    ICF_CFG_SET_RESPONSE(
                                p_crm_pdb,
                                ICF_FAILURE,
                                ICF_CAUSE_INVALID_LINE_ID)
                    ret_val = ICF_SUCCESS;
                }
                /* SPR 18616 fix, added validations for action 'Modify' 
                   for secondary registar address. 
                */
                else if (ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->
                        sec_registrar_addr.action)
                {
                    ret_val = icf_cfg_validate_modify_regm_prms(
                                  p_crm_pdb,line_id,(icf_boolean_t)ICF_FALSE);
                } 
                else if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                        sec_registrar_addr.action)
                {
                    ret_val = icf_cfg_validate_add_sec_regm_prms(p_crm_pdb, 
                            line_id);
                }/* end if - action ADD */
                else if (ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                        sec_registrar_addr.action)
                {
                    ret_val =icf_cfg_validate_del_sec_regm_prms(p_crm_pdb, 
                            line_id);
                }/* end if - action DELETE */
            }/* end else - not ALL_LINES */
        }/* end if - bitmask for secondary registrar */
    }/* end if ICF_TRUE == valid_params */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_validate_del_regm_prms
 * 
 * DESCRIPTION:     This function validates the registrar address to be deleted 
 *                  received in the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_del_regm_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_uint8_t                 line_status;
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_set_registration_req_st *p_recv_msg_payload = 
                                            ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    p_recv_msg_payload = (icf_set_registration_req_st *)
        (p_crm_pdb->p_recv_payload);

    if(ICF_SUCCESS == icf_cfg_check_line_in_db(
                p_crm_pdb, line_id, &line_status,
                (icf_boolean_t) ICF_FALSE))
    {
        /* Primary should already be configured on the line */
        if (ICF_NULL == (ICF_CFG_LINE_DATA_REGISTRAR_RECD 
                    & p_crm_pdb->p_line_data[line_id]->line_data_received))
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_PRIMARY_REGISTRAR_NOT_CONFIGURED)
            ret_val = ICF_SUCCESS;
        }
        /* If secondary registrar is present in API and action 
         * is not DELETE on the same line or ALL_LINES */
        else if ((ICF_NULL != (p_recv_msg_payload->bit_mask & 
                    ICF_SET_REG_SEC_REGISTRAR)) && 
                (ICF_CONFIG_ACTION_DELETE !=
                 p_recv_msg_payload->sec_registrar_addr.action) && 
                ((line_id == p_recv_msg_payload->sec_registrar_addr.line_id) || 
                 (ICF_ALL_LINES == p_recv_msg_payload->sec_registrar_addr.line_id)))
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INCONSISTENT_ACTIONS_FOR_REGISTRAR_CONFIGURATION)
            ret_val = ICF_SUCCESS;
        }
        /* If secondary registrar is not present in API but is 
           already configured on line, because if secondary is 
           present in API with action DELETE too when primary 
           is being deleted this erro should not be raised */
        else if ((ICF_NULL == (p_recv_msg_payload->bit_mask &
                    ICF_SET_REG_SEC_REGISTRAR)) &&
                  (ICF_NULL != (ICF_CFG_LINE_DATA_SEC_REGISTRAR_RECD & 
                    p_crm_pdb->p_line_data[line_id]->line_data_received)))
        {
            /* Return failure - primary cannot be deleted as 
             * secondary is configured */
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_SECONDARY_REGISTRAR_CONFIGURED)
            ret_val = ICF_SUCCESS;
        }/* end if not all_lines */
    }/* end if - SUCCESS - check_line_in_db */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_validate_del_sec_regm_prms
 * 
 * DESCRIPTION:     This function validates secondary registrar address to be 
 *                  deleted received in the API 
 *                  ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_del_sec_regm_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_uint8_t                                 line_status;
    icf_set_registration_req_st   *p_recv_msg_payload = 
                                                    ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    p_recv_msg_payload = (icf_set_registration_req_st *)
        (p_crm_pdb->p_recv_payload);
    if(ICF_SUCCESS == icf_cfg_check_line_in_db(
                p_crm_pdb, line_id, &line_status,
                (icf_boolean_t) ICF_FALSE))
    {
        if (ICF_NULL == (ICF_CFG_LINE_DATA_SEC_REGISTRAR_RECD 
                    & p_crm_pdb->p_line_data[line_id]->line_data_received))
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_SECONDARY_REGISTRAR_NOT_CONFIGURED)
            ret_val = ICF_SUCCESS;
        }
    }/* end if - SUCCESS - check_line_in_db */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_del_regm_prms_in_db
 * 
 * DESCRIPTION:     This function deletes the registrar address received in 
 *                  the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_del_regm_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    icf_port_memset(
            (icf_void_t*)&(p_crm_pdb->p_line_data[line_id]->reg_mode),
            (icf_uint8_t)0,
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_mode_et));
    
    icf_cmn_init_transport_address(
            &(p_crm_pdb->p_line_data[line_id]->registrar_addr));

    icf_port_memset(
            (icf_void_t*)&(p_crm_pdb->p_line_data[line_id]->reg_route_addr),
            (icf_uint8_t)0,
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

    ICF_CFG_RESET_BITMASK(
            p_crm_pdb->p_line_data[line_id]->line_data_received,
            ICF_CFG_LINE_DATA_REGISTRAR_RECD)

    ICF_CFG_RESET_BITMASK(
            p_crm_pdb->p_line_data[line_id]->line_data_received,
            ICF_CFG_LINE_DATA_REG_ROUTE_ADDR_RECD)
                    
    /* The default value of self mode is set after deleting the registar add */
    
    p_crm_pdb->p_line_data[line_id]->reg_mode = 
                                    ICF_TRANSPORT_MODE_BOTH;
    
    /* Check if the registrar address is the last data among proxy, registrar 
     * and user on the line then the line is to be deleted
     */
                    
    icf_cfg_delete_line(p_crm_pdb, line_id);
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}        

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_del_sec_regm_prms_in_db
 * 
 * DESCRIPTION:     This function deletes secondary registrar address received 
 *                  in the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_del_sec_regm_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    p_crm_pdb->line_id = line_id;
    
    ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_crm_pdb->p_line_data[line_id]->p_sec_registrar_addr,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
    
    ICF_CFG_RESET_BITMASK(
            p_crm_pdb->p_line_data[line_id]->line_data_received,
            ICF_CFG_LINE_DATA_SEC_REGISTRAR_RECD)

    /* The same transport mode is used as is set in primary registrar 
     * configuration, thus no need to update transport mode*/
    /*Now delete line if all other parameters are already deleted*/
    icf_cfg_delete_line(p_crm_pdb, line_id);
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} 

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_update_regm_prms_in_db
 * 
 * DESCRIPTION:     This function modifies the registration data received in
 *                  the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_update_regm_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    icf_set_registration_req_st *p_recv_msg_payload = 
                                                    ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_set_registration_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    if(ICF_NULL != (ICF_CFG_CC_SET_REG_HS_DURATION &
                p_recv_msg_payload->bit_mask))
    {
       p_crm_pdb->p_line_data[line_id]-> reg_hs_duration = 
             p_recv_msg_payload->register_head_start_timer;
    }
    
   
    if(ICF_NULL != (ICF_CFG_CC_SET_REG_REQ_DURATION &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_line_data[line_id]->registration_duration = 
            p_recv_msg_payload->registration_duration;
    }

    if(ICF_NULL != (ICF_CFG_CC_SET_REG_REQ_MODE & 
                p_recv_msg_payload->bit_mask))
    {
        /* SPR 16019: If action is not modify then only update
         * the reg_mode. Else save it in effective reg_mode
        */
        if(ICF_CONFIG_ACTION_MODIFY != p_recv_msg_payload->action)
        {
            if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
            {
                 if(ICF_TRANSPORT_MODE_BOTH == p_crm_pdb->p_config_data->self_mode)
                 {
                            p_crm_pdb->p_line_data[line_id]->reg_mode =
                                p_recv_msg_payload->reg_mode;
                            p_crm_pdb->effective_reg_mode = p_crm_pdb->
                                p_line_data[line_id]->reg_mode;
                  }
                  else
                  {
                        if(ICF_TRANSPORT_MODE_BOTH == p_recv_msg_payload->reg_mode)
                        {
                             p_crm_pdb->p_line_data[line_id]->reg_mode =
                                p_crm_pdb->p_config_data->self_mode;
                             p_crm_pdb->effective_reg_mode = p_crm_pdb->
                                p_line_data[line_id]->reg_mode;
                        }
                        else
                        {
                             /* If user wants to do the naptr then aloow
                                INVALID */
                             if((ICF_TRANSPORT_TYPE_INVALID != 
                                          p_recv_msg_payload->reg_mode) &&
                                (p_recv_msg_payload->reg_mode != 
                                          p_crm_pdb->p_config_data->self_mode))
                             {
                                   ICF_CFG_TRACE(p_crm_pdb,ICF_TRACE_PARAM_VALIDITY)
                                   ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                   ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE)
                                   ret_val = ICF_FAILURE;
                             }
                             else 
                             {
                                   p_crm_pdb->p_line_data[line_id]->reg_mode =
                                       p_recv_msg_payload->reg_mode;
                             }
                        }
                  }
            }
            else 
            {
                  p_crm_pdb->p_line_data[line_id]->reg_mode = 
                       p_recv_msg_payload->reg_mode;
            }
        }
        else
        {
            /* Check whether registrar mode is changed or not
             * If changed then set the new mode in configuration and
             * update the line data with new mode once all the transactions
             * on old mode cleared.
             */
            /* Also check for the transport mode. If transport mode is not Both 
             * then send error indication to application stating that the proxy 
             * mode is not changed but registrar mode is changed.
             */
            p_crm_pdb->effective_reg_mode = p_crm_pdb->p_line_data[line_id]->reg_mode;
            if(p_recv_msg_payload->reg_mode !=  p_crm_pdb->\
                    p_line_data[line_id]->reg_mode)
            {
                if(ICF_TRANSPORT_MODE_BOTH == p_crm_pdb->p_config_data->self_mode)
                {
                    if(ICF_TRANSPORT_MODE_BOTH == p_recv_msg_payload->reg_mode)
                    {
                        p_crm_pdb->p_line_data[line_id]->reg_mode = 
                            p_recv_msg_payload->reg_mode;
                        p_crm_pdb->effective_reg_mode = p_crm_pdb->
                            p_line_data[line_id]->reg_mode;
                    }
                    else
                    {
                        p_crm_pdb->effective_reg_mode = p_recv_msg_payload->
                            reg_mode;
                    }
                }
                else
                {
                    if(ICF_TRANSPORT_MODE_BOTH == p_recv_msg_payload->reg_mode && 
                            p_recv_msg_payload->reg_mode == p_crm_pdb->
                            p_config_data->self_mode)
                    {
                        p_crm_pdb->p_line_data[line_id]->reg_mode = 
                            p_recv_msg_payload->reg_mode;
                        p_crm_pdb->effective_reg_mode = p_crm_pdb->
                            p_line_data[line_id]->reg_mode;
                    }
                    else if(ICF_TRANSPORT_MODE_BOTH != p_recv_msg_payload->
                            reg_mode && p_recv_msg_payload->reg_mode != 
                            p_crm_pdb->p_config_data->self_mode)
                    {
                        ICF_CFG_TRACE(p_crm_pdb,ICF_TRACE_PARAM_VALIDITY)
                        ICF_CFG_SET_RESPONSE(p_crm_pdb, ICF_FAILURE,
                                ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE)	
                        ret_val = ICF_FAILURE;

                    }
                }

            }/* End of if block when recvd mode is different from the reg mode.*/
        }/* End of Modify Else */
    }
    if (ICF_NULL != (ICF_CFG_CC_SET_REG_REQ_ROUTE_ADDR & 
		p_recv_msg_payload->bit_mask))
    {
        icf_port_memcpy(
                (icf_void_t*)&(p_crm_pdb->p_line_data[line_id]->reg_route_addr),
            (icf_void_t*)&(p_recv_msg_payload->reg_route_addr),
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

        /* Set the bitmask on the line data to indicate that Route for sending 
         * registrar is received
         */
        /* Add a trace for the received Route address */
        ICF_CFG_SET_BITMASK(p_crm_pdb->p_line_data[line_id]->line_data_received,
                ICF_CFG_LINE_DATA_REG_ROUTE_ADDR_RECD)

    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;        
}        

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_add_regm_prms_in_db
 * 
 * DESCRIPTION:     This function adds the registrar address received in 
 *                  the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_add_regm_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    icf_set_registration_req_st *p_recv_msg_payload = 
                                                    ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_set_registration_req_st *)
                            (p_crm_pdb->p_recv_payload);

    p_crm_pdb->line_id = line_id;

     icf_cmn_copy_transport_address (
                &(p_crm_pdb->p_line_data[line_id]->registrar_addr),
                &(p_recv_msg_payload->registrar_addr),
                p_crm_pdb->p_glb_pdb,p_crm_pdb->p_ecode);
        /* Set the bitmask on the line data to indicate that registrar is received
         */

        ICF_CFG_SET_BITMASK(p_crm_pdb->p_line_data[line_id]->line_data_received,
        ICF_CFG_LINE_DATA_REGISTRAR_RECD)

        icf_cfg_update_regm_prms_in_db(p_crm_pdb, line_id);
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}                

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_add_sec_regm_prms_in_db
 * 
 * DESCRIPTION:     This function adds secondary registrar address received in 
 *                  the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_add_sec_regm_prms_in_db(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id)
{
    icf_return_t                                ret_val = ICF_SUCCESS;
    icf_set_registration_req_st                 *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Allocate memory for secondary registrar in line_data */
    /* Error cause set and passed on in p_crm_pdb in case 
     * of memory allocation failure rather than using
     * ICF_RET_ON_FAILURE option while doing MEMGET */
    ICF_MEMGET(
        p_crm_pdb->p_glb_pdb,
        ICF_PORT_SIZEOF(icf_transport_address_st),
        ICF_MEM_COMMON,
        p_crm_pdb->p_line_data[line_id]->p_sec_registrar_addr,
        ICF_DONOT_RET_ON_FAILURE,
        p_crm_pdb->p_ecode,
        ret_val)
    if (ICF_FAILURE == ret_val)
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                *p_crm_pdb->p_ecode)
        ICF_CFG_SET_ECODE(p_crm_pdb,*p_crm_pdb->p_ecode)
    }
    else
    {
        /* Copy the external message payload pointer locally */
        p_recv_msg_payload = (icf_set_registration_req_st *)
            (p_crm_pdb->p_recv_payload);

        p_crm_pdb->line_id = line_id;

         icf_cmn_copy_transport_address (
                    p_crm_pdb->p_line_data[line_id]->p_sec_registrar_addr,
                    &(p_recv_msg_payload->sec_registrar_addr.transport_addr),
                    p_crm_pdb->p_glb_pdb,p_crm_pdb->p_ecode);
            /* Set the bitmask on the line data to indicate that registrar is received
             */

            ICF_CFG_SET_BITMASK(p_crm_pdb->p_line_data[line_id]->line_data_received,
                    ICF_CFG_LINE_DATA_SEC_REGISTRAR_RECD)
        if (ICF_FAILURE == ret_val)
        {
            ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_crm_pdb->p_line_data[line_id]->p_sec_registrar_addr,
                    ICF_MEM_COMMON,
                    p_crm_pdb->p_ecode,
                    ret_val)
        }
    }
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_set_reg_prms_in_db
 * 
 *
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ is received 
 *                  before init complete.It calls function to validate the 
 *                  data received and sets the data in the line data.
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_reg_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    icf_line_id_t                      line_index = 0;
    icf_line_id_t                      line_id = 0;
    icf_set_registration_req_st        *p_recv_msg_payload = 
                                                    ICF_NULL;
    icf_uint8_t                        line_status[ICF_MAX_NUM_OF_LINES];
	icf_user_db_node_st					*p_user_db = ICF_NULL;
	icf_rgm_context_st					*p_rgm_context = ICF_NULL;
    /* CSR 1-5337328: Following flag indicates whether there is any existing
	 * rgm_ctxt on line */
	icf_boolean_t			atleast_one_rgm_ctxt_on_line = ICF_FALSE;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_set_registration_req_st *)
                            (p_crm_pdb->p_recv_payload);
    /*-----------------------------------------------------------------------*/ 
    /* validate the data received */
    ret_val = icf_cfg_validate_regm_prms(p_crm_pdb);
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    if(ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        if(ICF_NULL != p_recv_msg_payload)
        {
            p_crm_pdb->line_id = p_recv_msg_payload->line_id;
        }
        p_crm_pdb->result = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;            
    }            
                    
    /*-----------------------------------------------------------------------*/ 

    ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_REGISTRAR_INFO)
    /* SPR 15997: line_id in ICF_CONFIGURE_CFM_API */
    p_crm_pdb->line_id = p_recv_msg_payload->line_id;

    for (line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; 
            line_index++)
    {
        line_status[line_index] = ICF_STATUS_LINE_OLD;
    }  
    /* The data received is to be set on all the lines. This function is called
     * before init complete thus there is no need to trigger REGM, only
     * received data will be set on the lines
     */
    
    /* Check if all_lines field is true then first action is to be done 
     * on all the lines configured till now, If no line is configured
     * then all the lines upto max_lines will be configured now and data will
     * be set on all the lines.
     */

    if (ICF_ALL_LINES == p_recv_msg_payload->line_id)
    {
        if ((ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action) && 
                (ICF_SUCCESS == icf_cfg_check_line_configured(
                    p_crm_pdb, (icf_boolean_t)ICF_TRUE)))
        {
            /* Now the pointers to all the lines on which the data is to be set 
             * are received from DBM 
             */
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                        max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    /* Add the registrar address with all the optional 
                     * params received on the line
                     */

                    ret_val = icf_cfg_add_regm_prms_in_db(
                            p_crm_pdb, line_index);
                }
            }
            /* Collection of stats */

            if(ret_val !=ICF_FAILURE)
            {
                ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->
                                stat_register.stats_registrar_addr)
            }
            p_crm_pdb->line_id = p_recv_msg_payload->line_id;
            ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_REG_ADDR_SET)
            p_crm_pdb->line_id = p_recv_msg_payload->line_id;
            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_REG_PRMS_SET)
        }
        /* If dbm returns failure, response will be set in the function 
         * check line configured */

        /* Pointers will only be fetched for delete and modify action */
        else if((ICF_CONFIG_ACTION_ADD != p_recv_msg_payload->action) && 
                (ICF_SUCCESS == icf_cfg_check_line_configured(
                    p_crm_pdb, (icf_boolean_t)ICF_FALSE)))
        {
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                        max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES) ; line_index++)
            {
                /* CSR 1-5337328: initialize flag to false */
					atleast_one_rgm_ctxt_on_line = ICF_FALSE;
                if((ICF_TRUE == p_crm_pdb->lines_present[line_index]) && 
                        (ICF_CONFIG_ACTION_DELETE == 
                         p_recv_msg_payload->action))
                {
					p_user_db = p_crm_pdb->p_line_data[line_index]->p_user_db;

					while(ICF_NULL != p_user_db)
					{
						if(ICF_SUCCESS == icf_dbm_fetch_rgn_blk(
							p_crm_pdb->p_glb_pdb,
							&(p_user_db->public_user),
							line_index,
							&p_rgm_context,
							p_crm_pdb->p_ecode))
						{
							icf_cfg_rgm_deregister_user_on_deletion(
								p_crm_pdb,p_rgm_context);
						}
						p_user_db = p_user_db->p_next;
					}
					/* Remove the registrar address from the line 
					 */
					ret_val = icf_cfg_del_regm_prms_in_db(p_crm_pdb,
							line_index);
                }
                else if((ICF_TRUE == p_crm_pdb->lines_present[line_index]) &&
                        (ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->
                         action))
                {
					p_user_db = p_crm_pdb->p_line_data[line_index]->p_user_db;

                    ret_val = icf_cfg_update_regm_prms_in_db(p_crm_pdb,
                            line_index);
                    if(ICF_FAILURE != ret_val)
                    {
                        while(ICF_NULL != p_user_db)
                        {
                            if(ICF_SUCCESS == icf_dbm_fetch_rgn_blk(
                                        p_crm_pdb->p_glb_pdb,
                                        &(p_user_db->public_user),
                                        line_index,
                                        &p_rgm_context,
                                        p_crm_pdb->p_ecode))
                            {
                                /* set the flag indicating that there is
				 * atleast one valid rgm_context on this 
				 * line
				 */
				atleast_one_rgm_ctxt_on_line = ICF_TRUE;
                /*SPR 19793 The deregistration should not be send in the case
                 * when 401 is received for the REGISTER previously the check
                 was that if the registration state is not deregistering send
                 deregister now it is modified that if regm state is other
                 than deregister/awt_auth_resp then send deregister.*/
                                if((p_crm_pdb->p_line_data[line_index]->
                                            reg_mode != p_crm_pdb->
                                            effective_reg_mode)&&
                                   !((ICF_RGM_05_DEREGISTERING ==
                                          p_rgm_context->rgm_state) ||
                                     ((ICF_RGM_06_AWT_AUTH_RESP == 
                                         p_rgm_context->rgm_state) && 
                                       !(ICF_RGM_ONCE_REGISTERED &
                                           p_rgm_context->indicator))))
                                {
                                    /* SPR 16019: Set the bit mask in rgm_context to indicate that
                                     * de-reg generated internally.
                                     */
                                    /* If de-regsitration is already ongoing then no need to 
                                     * send de-register again
                                     */
                                    p_rgm_context->indicator |= ICF_RGM_INTERNAL_GENERATED_REG;
                                    icf_rgm_deregister_user(p_crm_pdb,p_rgm_context);
                                    p_crm_pdb->p_line_data[line_index]->reg_mode= 
                                        p_crm_pdb->effective_reg_mode; 
                                }
                                else if((p_crm_pdb->p_line_data[line_index]->
                                            registration_duration != 
                                            p_rgm_context->rgm_duration) &&
                                        (ICF_RGM_05_DEREGISTERING != 
                                         p_rgm_context->rgm_state))
                                {
                                    /*Fix for SPR 16334*/
                                    p_rgm_context->rgm_duration = 
                                    p_crm_pdb->p_line_data[line_index]->registration_duration;
                                    p_crm_pdb->p_rgm_context = p_rgm_context;
                                    icf_rgm_register_user(p_crm_pdb, 
                                            p_rgm_context);
                                }
                            }
                            p_user_db = p_user_db->p_next;
                        }
                        /* If there is no rgm_ctxt for any user, then reg_mode in
                         * line-data can be updated at this point itself
                         */
                        if ((ICF_FALSE == atleast_one_rgm_ctxt_on_line) && 
                                (ICF_NULL != (ICF_CFG_CC_SET_REG_REQ_MODE & 
                                              p_recv_msg_payload->bit_mask)))
                        {
                            p_crm_pdb->p_line_data[line_index]->reg_mode = 
                                p_crm_pdb->effective_reg_mode;
                        }
                    }   
                }                    
            }/* End loop */
            p_crm_pdb->line_id = p_recv_msg_payload->line_id;
            ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_REG_ADDR_SET)
            p_crm_pdb->line_id = p_recv_msg_payload->line_id;
            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_REG_PRMS_SET)
        }/* End else if(ICF_SUCCESS == ..*/
    p_crm_pdb->line_id = ICF_ALL_LINES ;
    }/* End if for all_lines */
    else
    {
        line_id = p_recv_msg_payload->line_id;
            
        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action) 
        {
            ret_val = icf_cfg_check_line_in_db(
                    p_crm_pdb,
                    line_id,
                    line_status,
                    (icf_boolean_t) ICF_TRUE);

            if(ICF_SUCCESS == ret_val)
            {
                /* Now the pointer to the line on which data is to be set is 
                 * received, Add the registrar address with all the optional 
                 * params received on the line
                 */
                ret_val = icf_cfg_add_regm_prms_in_db(p_crm_pdb, line_id);
                /* Collection of stats */

                if(ret_val !=ICF_FAILURE)
                {
                    ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->
                               stat_register.stats_registrar_addr)
                }
            }
            else
            {
                ret_val = ICF_SUCCESS;    
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INSUFFICIENT_RESOURCES)
            }
        }/* End if ICF_CONFIG_ACTION_ADD ==.. */
       
       /* For action delete and modify, only pointers of configured lines
        * will be fetched */ 
        else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
        {
            if(ICF_SUCCESS == icf_cfg_check_line_in_db(
                    p_crm_pdb,
                    line_id,
                    line_status,
                    (icf_boolean_t) ICF_FALSE))
            {
				if (line_id < ICF_MAX_NUM_OF_LINES) p_user_db = p_crm_pdb->p_line_data[line_id]->p_user_db;

				while(ICF_NULL != p_user_db)
				{
					if(ICF_SUCCESS == icf_dbm_fetch_rgn_blk(
						p_crm_pdb->p_glb_pdb,
						&(p_user_db->public_user),
						line_id,
						&p_rgm_context,
						p_crm_pdb->p_ecode))
					{
						if(1 == p_rgm_context->num_of_lines)
						{
							icf_cfg_rgm_deregister_user_on_deletion(
								p_crm_pdb,p_rgm_context);
						}
						else
						{
							/* If we have more than one lines using this context then we shall not
							 * deregister the user but update the rgm context and remove the entry 
							 * of this line.
							 */
							icf_cfg_update_rgm_context_remove_entry(p_crm_pdb,p_rgm_context,line_id);
						}
					}
					p_user_db = p_user_db->p_next;
				}
				/* Remove the registrar address from the line 
				 */
				ret_val = icf_cfg_del_regm_prms_in_db(p_crm_pdb,
						line_id);

                /* SPR 19489: The response for Registrar delete should be sent
                 * to application
                 */
                p_crm_pdb->resp_to_be_sent = ICF_TRUE; 
            }
        }            
        else if(ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->action)
        {
            if((line_id < ICF_MAX_NUM_OF_LINES) && (ICF_SUCCESS == icf_cfg_check_line_in_db(
                    p_crm_pdb,
                    line_id,
                    line_status,
                    (icf_boolean_t) ICF_FALSE)))
            {
		p_user_db = p_crm_pdb->p_line_data[line_id]->p_user_db;

                ret_val = icf_cfg_update_regm_prms_in_db(p_crm_pdb,
                        line_id);
                if(ICF_FAILURE != ret_val)
                {
                    while(ICF_NULL != p_user_db)
                    {
                        if(ICF_SUCCESS == icf_dbm_fetch_rgn_blk(
                                    p_crm_pdb->p_glb_pdb,
                                    &(p_user_db->public_user),
                                    line_id,
                                    &p_rgm_context,
                                    p_crm_pdb->p_ecode))
                        {
                             /* set the flag indicating that there is
			                  * atleast one valid rgm_context on this line
			                  */
			            	atleast_one_rgm_ctxt_on_line = ICF_TRUE;
                            /*SPR 19793 The deregistration should not be send in the case
                             * when 401 is received for the REGISTER previously the check
                              was that if the registration state is not deregistering send
                              deregister now it is modified that if regm state is other
                              than deregister/awt_auth_resp then send deregister.*/
                            if((p_crm_pdb->p_line_data[line_id]->reg_mode !=
                                        p_crm_pdb->effective_reg_mode)&&
                                 !((ICF_RGM_05_DEREGISTERING ==
                                          p_rgm_context->rgm_state) ||
                                  ((ICF_RGM_06_AWT_AUTH_RESP == 
                                         p_rgm_context->rgm_state) && 
                                       !(ICF_RGM_ONCE_REGISTERED &
                                           p_rgm_context->indicator))))
                            {
                                /* SPR 16019: Set the bit mask in rgm_context to
                                 * indicate that de-reg generated internally.
                                 */
                                /* If de-regsitration is already ongoing then no
                                 * need to send de-register again
                                 */
                                p_rgm_context->indicator |= ICF_RGM_INTERNAL_GENERATED_REG;
                                icf_rgm_deregister_user(p_crm_pdb,p_rgm_context);
                                p_crm_pdb->p_line_data[line_id]->reg_mode = 
                                    p_crm_pdb->effective_reg_mode; 
                            }

                            else if((p_crm_pdb->p_line_data[line_id]->
                                        registration_duration != 
                                        p_rgm_context->rgm_duration) &&
                                    (ICF_RGM_05_DEREGISTERING != p_rgm_context->
                                     rgm_state))
                            {
                               /*Fix for SPR 16334*/
                                p_rgm_context->rgm_duration = 
                                      p_crm_pdb->p_line_data[line_id]->registration_duration;
                                p_crm_pdb->p_rgm_context = p_rgm_context;
                                icf_rgm_register_user(p_crm_pdb, p_rgm_context);
                            }
                        }
                        p_user_db = p_user_db->p_next;
                    }
                    /* If there is no rgm_ctxt for any user, then reg_mode in
		     * line-data can be updated at this point itself
		     */
	        	if ((ICF_FALSE == atleast_one_rgm_ctxt_on_line) && 
						(ICF_NULL != (ICF_CFG_CC_SET_REG_REQ_MODE & 
							  p_recv_msg_payload->bit_mask)))
			{
				p_crm_pdb->p_line_data[line_id]->reg_mode = 
							p_crm_pdb->effective_reg_mode;
			}
                }
            }
        }
        p_crm_pdb->line_id = p_recv_msg_payload->line_id;
        ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_REG_ADDR_SET)
        p_crm_pdb->line_id = p_recv_msg_payload->line_id;
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_REG_PRMS_SET)
    }/* End else for all_lines */
   
    if (ICF_NULL != (ICF_SET_REG_SEC_REGISTRAR & 
                p_recv_msg_payload->bit_mask))
    {
        ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_SEC_REGISTRAR_INFO)
        p_crm_pdb->line_id = p_recv_msg_payload->sec_registrar_addr.line_id;
        if (ICF_FAILURE == icf_cfg_set_regm_sec_registrar_addr(p_crm_pdb))
        {
            p_crm_pdb->result = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }
    }
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    
}/* End function */

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_set_regm_sec_registrar_addr
 * 
 * DESCRIPTION:     This function will add secondary registrar address received
 *                  in ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_regm_sec_registrar_addr(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                              ret_val = ICF_SUCCESS;
    icf_set_registration_req_st               *p_recv_msg_payload = ICF_NULL;
    icf_line_id_t                             line_index = 0;
    icf_line_id_t                             line_id = 0;
    icf_uint8_t                               line_status[ICF_MAX_NUM_OF_LINES];
    icf_user_db_node_st					      *p_user_db = ICF_NULL;
    icf_rgm_context_st					      *p_rgm_context = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_set_registration_req_st *)
        (p_crm_pdb->p_recv_payload);
    
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SEC_REG_PRMS_REQ_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SEC_REG_ADDR_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SEC_REG_PRMS_RECD)

    /* Line id checked for secondary registrar */
    /* If line_id == ALL_LINES for secondary registrar */
    if (ICF_ALL_LINES == p_recv_msg_payload->sec_registrar_addr.line_id)
    {
        if ((ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                sec_registrar_addr.action) && 
                (ICF_SUCCESS == icf_cfg_check_line_configured(
                    p_crm_pdb, (icf_boolean_t)ICF_TRUE)))
        {
            /* Now the pointers to all the lines on which the data is to be set 
             * are received from DBM 
             */
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                        max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    /* Add secondary registrar address with all the optional 
                     * params received on the line
                     */

                    ret_val = icf_cfg_add_sec_regm_prms_in_db(
                            p_crm_pdb, line_index);
                }
            }
            /* Collection of stats */
            if(ret_val !=ICF_FAILURE)
            {
                ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->
                                stat_register.stats_registrar_addr)
            }
            p_crm_pdb->line_id = ICF_ALL_LINES ;
            if (ICF_FALSE != p_crm_pdb->is_valid_param)
            {
                ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_SEC_REG_ADDR_SET)
            }
        }
        /* If dbm returns failure, response will be set in the function 
         * check line configured */

        /* Pointers will only be fetched for delete action */
        else if((ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                sec_registrar_addr.action) && 
                (ICF_SUCCESS == icf_cfg_check_line_configured(
                    p_crm_pdb, (icf_boolean_t)ICF_FALSE)))
        {
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                        max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index]) 
                {
					p_user_db = p_crm_pdb->p_line_data[line_index]->p_user_db;

					while(ICF_NULL != p_user_db)
					{
						if(ICF_SUCCESS == icf_dbm_fetch_rgn_blk(
							p_crm_pdb->p_glb_pdb,
							&(p_user_db->public_user),
							line_index,
							&p_rgm_context,
							p_crm_pdb->p_ecode))
						{
							icf_cfg_rgm_deregister_user_on_deletion(
								p_crm_pdb,p_rgm_context);
						}
						p_user_db = p_user_db->p_next;
					}
					/* Remove the registrar address from the line 
					 */
					ret_val = icf_cfg_del_sec_regm_prms_in_db(p_crm_pdb,
							line_index);
                }
            }/* End loop */
            p_crm_pdb->line_id = ICF_ALL_LINES ;
            if (ICF_FALSE != p_crm_pdb->is_valid_param)
            {
                ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_SEC_REG_ADDR_SET)
            }
        }/* End else if(ICF_SUCCESS == ..*/
    }/* End if for all_lines */
    /* else specific line_id given for secondary registrar */
    else
    {
        /* Pick the specific line_id for secondary registrar from the API */
        line_id = p_recv_msg_payload->sec_registrar_addr.line_id;
            
        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                sec_registrar_addr.action) 
        {
            ret_val = icf_cfg_check_line_in_db(
                    p_crm_pdb,
                    line_id,
                    line_status,
                    (icf_boolean_t) ICF_TRUE);

            if(ICF_SUCCESS == ret_val)
            {
                /* Now the pointer to the line on which data is to be set is 
                 * received, Add the registrar address with all the optional 
                 * params received on the line
                 */
                ret_val = icf_cfg_add_sec_regm_prms_in_db(p_crm_pdb, line_id);
                /* Collection of stats */

                if(ret_val !=ICF_FAILURE)
                {
                ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->
                               stat_register.stats_registrar_addr)
                }
            }
            else
            {
                ret_val = ICF_SUCCESS;    
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INSUFFICIENT_RESOURCES)
            }
        }/* End if ICF_CONFIG_ACTION_ADD ==.. */
       
       /* For action delete and modify, only pointers of configured lines
        * will be fetched */ 
        else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->
                sec_registrar_addr.action)
        {
            if(ICF_SUCCESS == icf_cfg_check_line_in_db(
                    p_crm_pdb,
                    line_id,
                    line_status,
                    (icf_boolean_t) ICF_FALSE))
            {
				if ( line_id < ICF_MAX_NUM_OF_LINES ) p_user_db = p_crm_pdb->p_line_data[line_id]->p_user_db;

				while(ICF_NULL != p_user_db)
				{
					if(ICF_SUCCESS == icf_dbm_fetch_rgn_blk(
						p_crm_pdb->p_glb_pdb,
						&(p_user_db->public_user),
						line_id,
						&p_rgm_context,
						p_crm_pdb->p_ecode))
					{
						if(1 == p_rgm_context->num_of_lines)
						{
							icf_cfg_rgm_deregister_user_on_deletion(
								p_crm_pdb,p_rgm_context);
						}
						else
						{
							/* If we have more than one lines using this context
                             * then we shall not deregister the user but update
                             * the rgm context and remove the entry of this 
                             * line.
							 */
							icf_cfg_update_rgm_context_remove_entry(p_crm_pdb,
                                    p_rgm_context,line_id);
						}
					}
					p_user_db = p_user_db->p_next;
				}
            }/* End ICF_SUCCESS == icf_cfg_check_line_in_db */
            /* Remove the secondary registrar address from the line 
             */
            ret_val = icf_cfg_del_sec_regm_prms_in_db(p_crm_pdb,
                            line_id);
        }/* End action = DELETE */
        if (ICF_FALSE != p_crm_pdb->is_valid_param)
        {
            ICF_CFG_TRACE(p_crm_pdb,ICF_CFG_TRACE_SEC_REG_ADDR_SET)
        }
    }/* End else for all_lines */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_modify_reg_prms_in_db
 * 
 * DESCRIPTION:     This function is called when the API 
 *                  ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ is received 
 *                  after init complete.It calls function to validate the 
 *                  data received, sets the data in the line data and
 *                  triggers REGM to do the corrsponding action.
 *
 ******************************************************************************/
icf_return_t icf_cfg_modify_reg_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_line_id_t                       line_id = 0;
    icf_line_id_t                       line_index = 0;
    icf_set_registration_req_st *p_recv_msg_payload = 
                                                    ICF_NULL;
    icf_uint8_t                         line_status[ICF_MAX_NUM_OF_LINES];
    icf_boolean_t                       user_present = ICF_FALSE;
    icf_boolean_t                       registrar_present = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
       
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_set_registration_req_st *)
                            (p_crm_pdb->p_recv_payload);
    /*-----------------------------------------------------------------------*/
    /* validate the data received */
    
    ret_val = icf_cfg_validate_regm_prms(p_crm_pdb);
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    if(ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        if(ICF_NULL != p_recv_msg_payload)
        {
            p_crm_pdb->line_id = p_recv_msg_payload->line_id;
        }
        p_crm_pdb->result = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /*-----------------------------------------------------------------------*/
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
    
        
    /* check if all_lines field is true then first action is to be done
     * on all the lines configured till now, If no line is configured
     * then all the lines upto max_lines will be configured now and data will
     * be set on all the lines.
     */

    if(ICF_ALL_LINES == p_recv_msg_payload->line_id)
    {
        p_crm_pdb->rgm_pdb.all_lines = ICF_TRUE;
        
        /* Fetch line data pointer for all the lines configured till
         * now. If no line is configured then all the lines upto
         * max_lines will be configured now in case action is add.
         */
        
        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            if (ICF_FAILURE == icf_cfg_check_line_configured(
                        p_crm_pdb,(icf_boolean_t)ICF_TRUE))
            {
                ret_val = ICF_FAILURE;
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }
        }
        
        /* If action is modify then get the pointers of the lines configured
         * till now and do not configure the lines if no line is configured 
         * till now. The request will be ignored and success response
         * will be send in case no line is configured. Failure is not send 
         * in case of modify action
         */
        
        else
        {
            ret_val = icf_cfg_check_line_configured(
                    p_crm_pdb, (icf_boolean_t)ICF_FALSE);
        }
        
        /* Check if atleast one user is present on the lines. If it is 
         * present then only REGM will be triggred else only data
         * will be set
         */
        
        for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                        max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
        {
            if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
            {
                if(ICF_NULL != (ICF_CFG_LINE_DATA_SELF_ADDR_RECD &
                            p_crm_pdb->p_line_data[line_index]->
                            line_data_received))
                {
                    user_present = ICF_TRUE;
                }
               if(ICF_NULL != (ICF_CFG_LINE_DATA_REGISTRAR_RECD & 
                           p_crm_pdb->p_line_data[line_index]->
                           line_data_received))
               {
                   registrar_present = ICF_TRUE;
               }
            }
        }

        /* Check the action to be done on the line */
        /* If action is ADD */
        if (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                        max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    /* Add the registrar address with all the optional 
                     * params received on the line
                     */
                    ret_val = icf_cfg_add_regm_prms_in_db(p_crm_pdb,
                            line_index);
                }
            }/* End loop */
            
            /* If atleast one user is present then trigger REGM to register 
             * the users with the new registrar
             */
            
            if(ICF_TRUE == user_present)
            {
#ifndef ICF_FEATURE_ABANDON/* For ICF features not supported in ICF */
                ret_val = icf_rgm_modify_registrar_addr(p_crm_pdb);
#endif /* endif: ICF_FEATURE_ABANDON For ICF features not supported in ICF */
                
                /* If REGM returns failure */
                if(ICF_FAILURE == ret_val)
                {
                    /* Rollback i.e. delete the users added on the lines 
                     * and return Failure response */
                    for(line_index = 0; line_index < (p_crm_pdb->
                                p_config_data->max_lines); line_index++)
                    {
                        if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                        {
                            ret_val = icf_cfg_del_regm_prms_in_db(
                                    p_crm_pdb, line_index);
                        }
                    }
                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            p_crm_pdb->error_cause)
                    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                    return ret_val;
                }
            }
        }/* End if action = add */
        
        /* If action is delete */
        else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
        {
            /* if user is present then trigger REGM to de-register the 
             * users
             */
            if((ICF_TRUE == user_present) && (ICF_TRUE == 
                        registrar_present))
            {
#ifndef ICF_FEATURE_ABANDON/* For ICF features not supported in ICF */
                ret_val = icf_rgm_modify_registrar_addr(p_crm_pdb);
#endif /* endif: ICF_FEATURE_ABANDON For ICF features not supported in ICF */

                if(ICF_FAILURE == ret_val)            
                {
                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            p_crm_pdb->error_cause)
                    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                    return ret_val;
                }
                
                /* If REGM returns success then process the API and delete
                 * the registrar
                 */
    
                else
                {            
                    for(line_index = 0; line_index < (p_crm_pdb->p_config_data->
                                max_lines); line_index++)
                    {
                        if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                        {
                            ret_val = icf_cfg_del_regm_prms_in_db(
                                    p_crm_pdb, line_index);
                        }
                    }
                }
            }/* End if ICF_TRUE == user_present..*/
            
            /* If no user is present on the lines then directly delete the 
             * data
             */
            else if(ICF_FALSE == user_present)
            {
                for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                        max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
                {
                    if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                    {
                        ret_val = icf_cfg_del_regm_prms_in_db(
                                p_crm_pdb, line_index);
                    }
                }
            }
        }/* End if action = delete */
                    
        /* If action is modify */
        else if(ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->action)
        {   
            /* If user is present then and duration change is done then
             * trigger REGM to re-register with the new duration
             */
            if((ICF_TRUE == user_present) && 
                    (ICF_TRUE == registrar_present) && 
                    (ICF_NULL != (ICF_CFG_CC_SET_REG_REQ_DURATION & 
                                     p_recv_msg_payload->bit_mask)))
            {
#ifndef ICF_FEATURE_ABANDON/* For ICF features not supported in ICF */
                ret_val = icf_rgm_modify_duration(p_crm_pdb);
#endif /* endif: ICF_FEATURE_ABANDON For ICF features not supported in ICF */
            }

            /* Check if rgm returns failure API will send failure response */
            
            if(ICF_FAILURE == ret_val)
            {                
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        p_crm_pdb->error_cause)
            }
            else
            {
                for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                            max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
                {
                    if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                    {
                        ret_val = icf_cfg_update_regm_prms_in_db(
                                p_crm_pdb, line_index);
                    }
                }/* end for */
            }/* End else */
        }/* End if action = modify */
    }/* End if for all_lines */
    
    else
    {
        p_crm_pdb->rgm_pdb.all_lines = ICF_FALSE;
        
        for(line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; 
                line_index++)
        {
            line_status[line_index] = ICF_STATUS_LINE_OLD;            
        }
        
        line_id = p_recv_msg_payload->line_id;
                
        /* if action is add then get the pointer for the
         * requested line id
         */
        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            if (ICF_FAILURE == icf_cfg_check_line_in_db(
                        p_crm_pdb,
                        line_id,
                        line_status,
                        (icf_boolean_t)ICF_TRUE))
            {
                ret_val = ICF_FAILURE;
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INSUFFICIENT_RESOURCES)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }
        }/* End if action == add */
                
        /* If action is modify or delete then fetch the pointer to the line
         * but if the line is not configured then do not configure
         * the line but ignore the request
         */

        else
        {
            if (ICF_FAILURE == icf_cfg_check_line_in_db(
                        p_crm_pdb,
                        line_id,
                        line_status,
                        (icf_boolean_t)ICF_FALSE))
            {
                /* if the line is not configured */
                ret_val = ICF_SUCCESS;
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }
        }
        /* Check the action */
        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            /* Add the registrar address with all the optional 
             * params received on the line
             */

            ret_val = icf_cfg_add_regm_prms_in_db(
                    p_crm_pdb, line_id);

            /* Check if one user is present on the line then this
             * index of the API will be added in the rgm_pdb
             */

            if((line_id < ICF_MAX_NUM_OF_LINES) && (ICF_NULL != (ICF_CFG_LINE_DATA_SELF_ADDR_RECD &
                        p_crm_pdb->p_line_data[line_id]->
                        line_data_received)))
            {
#ifndef ICF_FEATURE_ABANDON
                ret_val = icf_rgm_modify_registrar_addr(p_crm_pdb);
#endif
                /* If REGM returns failure then data which was added on the 
                 * lines will be deleted (rollback)
                 */

                if(ICF_FAILURE == ret_val)
                {
                    ret_val = icf_cfg_del_regm_prms_in_db(
                            p_crm_pdb, line_id);

                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            p_crm_pdb->error_cause) 

                }/* if(ICF_FAILURE == */  
            }/* End if(ICF_NULL != ..*/
        }/* if(ICF_CONFIG_ACTION_ADD */ 
        else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
        {
                
            /* If action is delete then check if user is present on the 
             * line add its index in the rgm pdb. The data will be 
             * actually deleted after success is returned from 
             * REGM.
             */
			if (line_id < ICF_MAX_NUM_OF_LINES) {
            if(ICF_NULL != (ICF_CFG_LINE_DATA_SELF_ADDR_RECD &
                        p_crm_pdb->p_line_data[line_id]->
                        line_data_received) && 
                    (ICF_NULL != (ICF_CFG_LINE_DATA_REGISTRAR_RECD & 
                                     p_crm_pdb->p_line_data[line_id]->
                                     line_data_received)))
            {
#ifndef ICF_FEATURE_ABANDON/* For ICF features not supported in ICF */
                /* Trigger REGM if atleast one user is present */
		ret_val = icf_rgm_modify_registrar_addr(p_crm_pdb);
#endif /*endif: ICF_FEATURE_ABANDON For ICF features not supported in ICF */
        
                /* If REGM returns success then delete the data to be deleted
                 * from the lines and trigger REGM for modifying the duration if
                 * needed. 
                 */

                if(ICF_SUCCESS == ret_val)
                {
                    /* Remove the registrar address from the line  */
                    ret_val = icf_cfg_del_regm_prms_in_db(
                            p_crm_pdb,
                            line_id);
                }
                else
                {
                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            p_crm_pdb->error_cause) 
                }
            }
            /* If user is not present, and only registrar is present, then 
             * registrar needs to be deleted */
            else if(ICF_NULL == (ICF_CFG_LINE_DATA_SELF_ADDR_RECD &
                        p_crm_pdb->p_line_data[line_id]->
                        line_data_received) && 
                    (ICF_NULL != (ICF_CFG_LINE_DATA_REGISTRAR_RECD & 
                                     p_crm_pdb->p_line_data[line_id]->
                                     line_data_received)))
            {
                    /* Remove the registrar address from the line  */
                    ret_val = icf_cfg_del_regm_prms_in_db(
                            p_crm_pdb, line_id);
            }
			}
        }/* End if action = delete */
                   
        /* After processing the add and delete actions, modify will be 
         * processed. REGM is assumed to return failure in case of 
         * modify action only when a critical failure like MEMGET failure 
         * occurs. */

        /* If action is modfify */
        else if(ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->action)
        {
            /* Check if duration is changed and user and registrar are present
             * on the line on which action is done 
             */
			if (line_id < ICF_MAX_NUM_OF_LINES) 
            if((ICF_NULL != (ICF_CFG_CC_SET_REG_REQ_DURATION &
                         p_recv_msg_payload->bit_mask)) && 
                    (ICF_NULL != (ICF_CFG_LINE_DATA_SELF_ADDR_RECD & 
                                     p_crm_pdb->p_line_data[line_id]->
                                     line_data_received)) && 
                    (ICF_NULL != (ICF_CFG_LINE_DATA_REGISTRAR_RECD & 
                                     p_crm_pdb->p_line_data[line_id]->
                                     line_data_received)))
            {
#ifndef ICF_FEATURE_ABANDON/* For ICF features not supported in ICF */
                /* Trigger REGM */
                ret_val = icf_rgm_modify_duration(p_crm_pdb);
#endif /*endif: ICF_FEATURE_ABANDON For ICF features not supported in ICF */
            }

            /* If rgm returns failure API will send failure response */
            if(ICF_FAILURE == ret_val)
            {
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        p_crm_pdb->error_cause)
            }
            else
            {
                /* Update the data received in the API if REGM 
                 * returns success */
                ret_val = icf_cfg_update_regm_prms_in_db(p_crm_pdb, line_id);
            }                       
        }/* End if action = modify */
    }/* End else for all_lines */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    
}/* End function */
        
#ifdef ICF_SERVICE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_srvc_prms
 * 
 * DESCRIPTION:     This function validates the params recieved in the set 
 *                  service params API.
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_cfg_validate_srvc_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_service_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_boolean_t        line_configured = ICF_FALSE;
    icf_boolean_t        all_lines = ICF_FALSE;    
    icf_uint8_t          line_status[ICF_MAX_NUM_OF_LINES];
    icf_line_id_t          line_index = 0;
    icf_line_id_t        line_id = 0;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_service_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
        
    /* This boolean array is used to mark the line id pointerss needed in
     * all the parameters of the API. This is used to avoid repeated 
     * requests to DBM for the same pointer
     */
        
    for (line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; line_index++)
    {
        p_crm_pdb->requested_lines[line_index] = ICF_FALSE;
    }
    
    if (ICF_NULL != 
            (ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_FLAG & 
             p_recv_msg_payload->bit_mask))
    {
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_SUBS_FLAG_RECD)
        /* If all_lines is true then set the local variable for all_lines true
         */
        if (ICF_ALL_LINES == p_recv_msg_payload->service_subscription_flag.line_id) 
        {
            all_lines = ICF_TRUE;
        }
        else
        {
            line_id = p_recv_msg_payload->service_subscription_flag.line_id;
                                
            /* Validate the line id before indexing the array 
             * p_crm_pdb->requested_lines
             */
            if ((line_id >= p_crm_pdb->p_config_data->max_lines) || (line_id >= ICF_MAX_NUM_OF_LINES))
            {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID) 

                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }
                
            p_crm_pdb->requested_lines[line_id] = ICF_TRUE;
        }/* End else */
    }/* End If for service subscription flag present */
        
    /*-----------------------------------------------------------*/
    /* If service activation flag is present */
    if(ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_ACTIVATION_FLAG & 
             p_recv_msg_payload->bit_mask))
    {
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_ACT_FLAG_RECD)
        if(ICF_ALL_LINES == p_recv_msg_payload->service_activation_flag.line_id)
        {
            all_lines = ICF_TRUE;
        }
        else
        {
            line_id = p_recv_msg_payload->service_activation_flag.line_id;
                
            if ((line_id > p_crm_pdb->p_config_data->max_lines) || (line_id >= ICF_MAX_NUM_OF_LINES))
            {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
                
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }
                
            p_crm_pdb->requested_lines[line_id] = ICF_TRUE;
        }/* End else */
    }/* End If for service activation flag present */

    /*-----------------------------------------------------------*/
    /* If address for CFU is present */
    if(ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CFU_ADDR & 
             p_recv_msg_payload->bit_mask))
    {
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFU_ADDR_RECD)

     /*  ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFU_ADDR_PRMS_RECD)*/

        if(ICF_FAILURE == icf_cmn_validate_trnspt_addr(
                    p_crm_pdb->p_glb_pdb,
                    &(p_recv_msg_payload->addr_for_cfu.address), 
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_ADDRESS_RECEIVED)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
        }
        else if(ICF_ALL_LINES == p_recv_msg_payload->addr_for_cfu.line_id )
        {
            all_lines = ICF_TRUE;            
        }
        else
        {
            line_id = p_recv_msg_payload->addr_for_cfu.line_id;
                    
            if ((line_id >= p_crm_pdb->p_config_data->max_lines) || (line_id >= ICF_MAX_NUM_OF_LINES))
            {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }
            p_crm_pdb->requested_lines[line_id] = ICF_TRUE;
        }/* End else */
    }/* End If address for CFU present */

    /*-----------------------------------------------------------*/
    /* If address for CFB is present */
    if(ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CFB_ADDR & 
             p_recv_msg_payload->bit_mask))
    {
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFB_ADDR_RECD)
        /*ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFB_ADDR_PRMS_RECD)*/
        if(ICF_FAILURE == icf_cmn_validate_trnspt_addr(
                    p_crm_pdb->p_glb_pdb,
                    &(p_recv_msg_payload->addr_for_cfb.address), 
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_ADDRESS_RECEIVED)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
        }
        else if(ICF_ALL_LINES == p_recv_msg_payload->addr_for_cfb.line_id)
        {
            all_lines = ICF_TRUE;            
        }
        else
        {
            line_id = p_recv_msg_payload->addr_for_cfb.line_id;
                    
            if ((line_id >= p_crm_pdb->p_config_data->max_lines) || (line_id >= ICF_MAX_NUM_OF_LINES))
            {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }

            p_crm_pdb->requested_lines[line_id] = ICF_TRUE;
        }/* End else */
    }/* End If address for CFB present */
        
    /*-----------------------------------------------------------*/
    /* If address for CFD is present */
    if(ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CFD_ADDR & 
             p_recv_msg_payload->bit_mask))
    {
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFD_ADDR_RECD)
        /*ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFD_ADDR_PRMS_RECD)*/
        if(ICF_FAILURE == icf_cmn_validate_trnspt_addr(
                    p_crm_pdb->p_glb_pdb,
                    &(p_recv_msg_payload->addr_for_cfd.address), 
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_ADDRESS_RECEIVED)
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
        }
        else if(ICF_ALL_LINES == p_recv_msg_payload->addr_for_cfd.line_id)
        {
            all_lines = ICF_TRUE;            
        }             
        else
        {
            line_id = p_recv_msg_payload->addr_for_cfd.line_id;
                    
            if ((line_id >= p_crm_pdb->p_config_data->max_lines) || (line_id >= ICF_MAX_NUM_OF_LINES))
            {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

                ICF_CFG_SET_RESPONSE( p_crm_pdb,
                        ICF_FAILURE, ICF_CAUSE_INVALID_LINE_ID)

                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
            }
                
            p_crm_pdb->requested_lines[line_id] = ICF_TRUE;
        }/* End else */
    }/* End If address for CFD present */

	if(ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CALL_HOLD_OPTION & 
                p_recv_msg_payload->bit_mask))
    {
        if(ICF_ALL_LINES == p_recv_msg_payload->call_hold_option.line_id) 
        {
            all_lines = ICF_TRUE;
        }
        else
        {
            line_id = p_recv_msg_payload->call_hold_option.line_id;

            if((line_id >= p_crm_pdb->p_config_data->max_lines) || (line_id >= ICF_MAX_NUM_OF_LINES))
            {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

                    ICF_CFG_SET_RESPONSE( p_crm_pdb,
                            ICF_FAILURE, ICF_CAUSE_INVALID_LINE_ID)

                    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                    return ret_val;
            }

            p_crm_pdb->requested_lines[line_id] = ICF_TRUE;      
        }
    }

    for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); 
            line_index++)
    {
        if(ICF_TRUE == p_crm_pdb->requested_lines[line_index] )
        {
            ret_val = icf_cfg_check_line_in_db(
                    p_crm_pdb,
                    line_index,
                    line_status,
                    (icf_boolean_t) ICF_FALSE);
            
            if(ret_val == ICF_FAILURE)
            {
                /* Line is not configured */
                p_crm_pdb->requested_lines[line_index]=ICF_FALSE;
            }
            ret_val = ICF_SUCCESS;
        }
    }
        
        
    /*-----------------------------------------------------------*/
    /*-----------------------------------------------------------*/
    /* If all lines field is set true by any one of the above parameters
     * then pointers to all the lines configured till now will be taken.
     * If there is any invalid request then the function will return 
     * failure before reaching here.
     */
    if (ICF_TRUE == all_lines) 
    {
        for(line_index = 0; (line_index < (p_crm_pdb->p_config_data->max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); 
                line_index++)
        {
            if(ICF_FALSE == p_crm_pdb->requested_lines[line_index] )
            {
                ret_val = icf_cfg_check_line_in_db(
                        p_crm_pdb,
                        line_index,
                        line_status,
                        (icf_boolean_t) ICF_FALSE);
                            
                if(ICF_SUCCESS == ret_val)
                {
                    line_configured = ICF_TRUE;
                    p_crm_pdb->lines_present[line_index] = ICF_TRUE;
                }                    
            }
            else
            {
                line_configured = ICF_TRUE;
                p_crm_pdb->lines_present[line_index] = ICF_TRUE;
            }
        }/* End for line_index */            
        
        if(ICF_FALSE == line_configured)
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_LINE_NOT_CONFIGURED)
            
            ret_val = ICF_SUCCESS;
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }
    }/* End if for all_lines */            

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_srvc_config_prmms
 *
 * DESCRIPTION:     This function will validate the non-line specific data
 *                  of the set service params API.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_srvc_config_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_service_params_req_st *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_PRMS_REQ_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_TIMERS_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CALL_FWD_TIMERS_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_SUBSCRIBE_PRMS_RECD)

    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_service_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    /* In all below validations, the two if conditions have been combined
     * to avoid multiple return statements from between the function */

    /* Validate the value of call hold resume timer value */
    if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CHR_TIMER & 
                 p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > 
             p_recv_msg_payload->call_hold_resume_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    /* Validate the value of reinvite race timer value */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_REINVITE_RACE_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > 
             p_recv_msg_payload->reinvite_race_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    /* Validate the value of three way call timer value */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_TWC_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->twc_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    /* Validate the value of ctu_timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CTU_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->ctu_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
   
    /* Validate the value of cta_timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CTA_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->cta_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    /* Validate the value of rnw timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_RNW_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->rnw_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    
    /* Validate the value of cw_alert_timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CW_ALERT_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->cw_alert_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    
    /* Validate the value of cnd_alert_timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CND_ALERT_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->cnd_alert_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    
    /* Validate the value of cw and cnd_alert_timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CW_N_CND_ALERT_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > 
             p_recv_msg_payload->cw_n_cnd_alert_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    } 
    
    /* Validate the value of subs_resp_timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_SUBS_RESP_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->subs_resp_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

     /* Validate the value of subs_retry_timer */
    else if ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_SUBS_RETRY_TIMER &
                p_recv_msg_payload->bit_mask)) && 
            (ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->subs_retry_timer))
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
#ifndef ICF_FEATURE_ABANDON
    /* Validate the Global activation flags*/
    else if(
            (ICF_NULL != (p_recv_msg_payload->bit_mask & 
                             ICF_CFG_CC_SET_SER_PRM_GLB_ACT_FLAG)) &&
            (ICF_NULL != (p_recv_msg_payload->global_service_activation_flag 
                             & ICF_SERVICE_GLB_ACT_MESSAGE_WAITING_IND)))
    {
        /* If global activation flag is present in the payload and its 
         * value indicates service MWI, we have to check if
         * group VMS address is present */
        
        if(ICF_NULL == p_crm_pdb->p_config_data->p_group_vms_addr)
        {
            /* If Group VMS Address is not present, this service cannot 
             * be activated */

            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
        }
        else if((ICF_TRUE == p_crm_pdb->global_data_init_complete) && 
                (ICF_CFG_VMS_GRP_SUBSCRIBED != 
                 (p_crm_pdb->p_config_data->vms_subscription_status & 
                  ICF_CFG_VMS_GRP_SUBSCRIBED)))
        {
            /* If init complete has been done and MWI has been configured for
             * each line, then Global MWI Service activation cannot be done, 
             * since this will result in change from line to group 
             * configuration for MWI 
             */
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INVALID_ACTION)

            ret_val = ICF_SUCCESS;
        }
    }/* End if global act flag*/   
#endif

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_srvc_prms_in_db
 *
 * DESCRIPTION:     This function will add the data received in Set Service 
 *                  params message into global database. First, it adds any 
 *                  general config data received and then it adds any line 
 *                  specific data received.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_srvc_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_service_params_req_st *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_service_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    /*-----------------------------------------------------------------------*/
    /* Validate the data received */
    ret_val = icf_cfg_validate_srvc_prms(p_crm_pdb);
    if(ICF_TRUE == p_crm_pdb->is_valid_param)
    {
        ret_val = icf_cfg_validate_srvc_config_prms(p_crm_pdb);
    }

    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY)
    if (ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        if(ICF_NULL != p_recv_msg_payload)
        {
            /* Check which service is sepcified in the payload and set the line id
             * for the first subscribed service*/
            if(ICF_NULL !=  (p_recv_msg_payload->bit_mask &
                 ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_FLAG))
            {
                p_crm_pdb->line_id =
                    p_recv_msg_payload->service_subscription_flag.line_id;
            }
            else if(ICF_NULL !=  (p_recv_msg_payload->bit_mask &
                        ICF_CFG_CC_SET_SER_PRM_ACTIVATION_FLAG))
            {
                p_crm_pdb->line_id =
                    p_recv_msg_payload->service_activation_flag.line_id;
            }
            else if(ICF_NULL !=  (p_recv_msg_payload->bit_mask &
                        ICF_CFG_CC_SET_SER_PRM_CALL_HOLD_OPTION))
            {
                p_crm_pdb->line_id =
                    p_recv_msg_payload->call_hold_option.line_id;

            }
            else
            {
                /*Line id not specified in the configuration API
                 */
            }
        }
        p_crm_pdb->result = ICF_FAILURE;
        ret_val = ICF_SUCCESS;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /*-----------------------------------------------------------------------*/
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    /* SPR 15997: Line id in Configure confirm API*/
    /* Check which service is sepcified in the payload and set the line id 
     * for the first subscribed service*/
     if(ICF_NULL !=  (p_recv_msg_payload->bit_mask & 
             ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_FLAG))
     {
         p_crm_pdb->line_id = 
            p_recv_msg_payload->service_subscription_flag.line_id;
     }
     else if(ICF_NULL !=  (p_recv_msg_payload->bit_mask & 
                 ICF_CFG_CC_SET_SER_PRM_ACTIVATION_FLAG))
     {
         p_crm_pdb->line_id = 
            p_recv_msg_payload->service_activation_flag.line_id;
     }
     else if(ICF_NULL !=  (p_recv_msg_payload->bit_mask & 
                 ICF_CFG_CC_SET_SER_PRM_CALL_HOLD_OPTION))
     {
         p_crm_pdb->line_id = 
            p_recv_msg_payload->call_hold_option.line_id;

     }
     else
     {
         /*Line id not specified in the configuration API
          */

     }    
    /* The pointers needed for the line ids are already taken in the 
     * function to validate the data                           
     */
        
    /* Update icf_line_data_st in global database */
    ret_val = icf_cfg_set_srvc_line_prm(p_crm_pdb);

    if (ICF_SUCCESS == ret_val)
    {
    	/* Update the icf_config_data_st with the params received */
	    ret_val = icf_cfg_set_srvc_config_prm(p_crm_pdb);
    }

    /* Invoke MWI for subscription/unsubscription if line prms have been 
     * successfully updated*/

    /* Either of the grp or line subscription flags must be present in payload
     * and init complete must be true to invoke MWI */
#ifndef ICF_FEATURE_ABANDON

    if((ICF_SUCCESS == ret_val) && 
            ((ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_ACTIVATION_FLAG & 
                              p_recv_msg_payload->bit_mask)) || 
             (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_FLAG & 
                              p_recv_msg_payload->bit_mask)) ||
             (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_GLB_SUBS_FLAG &
                              p_recv_msg_payload->bit_mask)) ||
             (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_GLB_ACT_FLAG &
                              p_recv_msg_payload->bit_mask))) &&
            (ICF_TRUE == p_crm_pdb->global_data_init_complete))
    {
        /* Invoke function to trigger MWI */
        ret_val = icf_cfg_mwi_trigger(p_crm_pdb, 
                (icf_uint8_t)ICF_CFG_SUBSCRIBE_TRIGGER);
    }
#endif    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */



/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_srvc_config_prm
 *
 * DESCRIPTION:     This function will save the service param related to 
 *                  config data into global database.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_srvc_config_prm(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_service_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_line_id_t                          line_index = ICF_NULL;
    icf_line_id_t                          line_id = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_service_params_req_st *)
        (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    /* Check for the presence of call hold option */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CALL_HOLD_OPTION &
            p_recv_msg_payload->bit_mask))
    {
        /*p_crm_pdb->p_config_data->hold_option = p_recv_msg_payload->
        call_hold_option;*/
        /* Start: Addition for Obsolete Hold Parameter */
        p_crm_pdb->p_config_data->hold_option = p_recv_msg_payload->
            call_hold_option.hold_option;
        /* Check if all_lines is true */
        if(ICF_ALL_LINES == p_recv_msg_payload->call_hold_option.line_id)
         {
           for (line_index = 0; 
		   		(line_index < (p_crm_pdb->p_config_data->max_lines)) &&
				(line_index < ICF_MAX_NUM_OF_LINES); line_index++)
           {
             if (ICF_TRUE == p_crm_pdb->lines_present[line_index])
             {
               p_crm_pdb->p_line_data[line_index]->hold_option =
               p_recv_msg_payload->call_hold_option.hold_option;
               p_crm_pdb->line_id = line_index;
             }
           }
         }
        else
         {
           /*  Fix for spr 18784 */
           /*  SCENARIO : If call hold option is set for line 0
                          and service activation is done for all lines */
           line_id = p_recv_msg_payload->call_hold_option.line_id;
           /* If line is configured */
           if((line_id < ICF_MAX_NUM_OF_LINES) && (ICF_TRUE == p_crm_pdb->requested_lines[line_id]))
            {
              p_crm_pdb->p_line_data[line_id]->hold_option =
              p_recv_msg_payload->call_hold_option.hold_option;
              p_crm_pdb->line_id = line_id;
            }
         }/* End: Addition for Obsolete Hold Parameter */   
    }  

    /* Check for the presence of max_num_of_call_forwarding_hops */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_MAX_NUM_OF_HOPS &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->max_num_of_call_forwarding_hops = 
            p_recv_msg_payload->max_num_of_call_forwarding_hops;
    }            
                
    /* Check for the presence of max_num_of_calls_in_call_waiting */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_MAX_CALL_WAITING &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->max_num_of_calls_in_call_waiting = 
            p_recv_msg_payload->max_num_of_calls_in_call_waiting;
    }            
                
    /* Check for presence of call hold resume timer value */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CHR_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->call_hold_resume_timer = 
            p_recv_msg_payload->call_hold_resume_timer;
    }

    /* Check for presence of reinvite race timer value */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_REINVITE_RACE_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->reinvite_race_timer = 
            p_recv_msg_payload->reinvite_race_timer;
    }

    /* Check for presence of three way call timer value */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_TWC_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->twc_timer = p_recv_msg_payload->twc_timer;
    } 

    /* Check for presence of ctu_timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CTU_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->ctu_timer = p_recv_msg_payload->ctu_timer;
    }       
   
    /* Check for presence of cta_timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CTA_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->cta_timer = p_recv_msg_payload->cta_timer;    
    } 
    
    /* Check for presence of rnw_timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_RNW_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->rnw_timer = p_recv_msg_payload->rnw_timer; 
    }     
#ifdef ICF_SERVICE_ENABLE    
    /* Check for presence of subscription reqd for xconf */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_REQD_IN_EXT_CONF &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->xconf_subs_duration =
            p_recv_msg_payload->xconf_subs_duration; 
        p_crm_pdb->p_config_data->xconf_subs_reqd = ICF_TRUE;
    }
#endif    
    /* Check for presence of cw_alert_timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CW_ALERT_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->cw_alert_timer = p_recv_msg_payload->
            cw_alert_timer;        
    }     
    
    /* Check for presence of cnd_alert_timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CND_ALERT_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->cnd_alert_timer = p_recv_msg_payload->
            cnd_alert_timer;        
    } 
    
    /* Check for presence of cw and cnd_alert_timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CW_N_CND_ALERT_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->cw_n_cnd_alert_timer = 
            p_recv_msg_payload->cw_n_cnd_alert_timer;        
    } 
   
   /* Check for presence of subscribe resp timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_SUBS_RESP_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->subs_resp_timer = 
            p_recv_msg_payload->subs_resp_timer;        
    }
    /* Check for presence of subscribe retry timer */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_SUBS_RETRY_TIMER &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->subs_retry_timer = 
            p_recv_msg_payload->subs_retry_timer;        
    }

    if (ICF_NULL != (ICF_SET_SER_PRM_CONF_REQD_AT_TRANFEREE &
                p_recv_msg_payload->bit_mask))
    {

        p_crm_pdb->p_config_data->conf_reqd_at_transferee = 
            p_recv_msg_payload->conf_reqd_at_transferee;
    }

    if (ICF_NULL != (ICF_SET_SER_PRM_CONF_CFM_REQD_AT_TRANSFEREE &
                p_recv_msg_payload->bit_mask))
    {
        p_crm_pdb->p_config_data->conf_cfm_reqd_at_transferee = 
            p_recv_msg_payload->conf_cfm_reqd_at_transferee;
    }

    if (ICF_SET_SER_PRM_CONF_FAIL_CALL_CLEAR_NOT_REQD &
					                 p_recv_msg_payload->bit_mask)
	{
			p_crm_pdb->p_config_data->conf_fail_call_clear_not_reqd = 
					   p_recv_msg_payload->conf_fail_call_clear_not_reqd;
	}

    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_PRMS_SET)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_TIMERS_SET)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CALL_FWD_TIMERS_SET)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_SUBSCRIBE_PRMS_SET)

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */




/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_ret_srvc_id
 *
 * DESCRIPTION:     This function will return the bit value of the service
 *
 * RETURNS:         icf_uint16_t
 *
 ******************************************************************************/
icf_uint16_t icf_cfg_ret_srvc_id(
        INOUT   icf_crm_pdb_st          *p_crm_pdb,
        IN      icf_uint16_t            index,
        IN      icf_boolean_t           subscription_request)
{
#ifdef ICF_TRACE_ENABLE
icf_return_t      ret_val = ICF_SUCCESS;
#endif
    icf_uint16_t      ret_srvc_id = 0;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    /* To remove unused parameter warning */
    p_crm_pdb = p_crm_pdb;

    switch (index)
    {
        case ICF_SERVICE_CALL_HOLD_RESUME: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_CALL_HOLD_RESUME;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_CALL_HOLD_RESUME;
            }
            break;

        case ICF_SERVICE_THREE_WAY_CALL: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_THREE_WAY_CALL;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_THREE_WAY_CALL;
            }
            break;

        case ICF_SERVICE_CALL_TRF_ATTENDED: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_CALL_TRF_ATTENDED;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_CALL_TRF_ATTENDED;
            }
            break;

        case ICF_SERVICE_CALL_TRF_UNATTENDED: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_CALL_TRF_UNATTENDED;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_CALL_TRF_UNATTENDED;
            }
            break;

        case ICF_SERVICE_INVITE_WITH_REPLACE: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_INVITE_WITH_REPLACE;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_INVITE_WITH_REPLACE;
            }
            break;

        case ICF_SERVICE_CALL_TRF_FROM_NW: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_CALL_TRF_FROM_NW;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_CALL_TRF_FROM_NW;
            }
            break;

        case ICF_SERVICE_CALL_FWD_UNCONDITIONAL: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_CALL_FWD_UNCONDITIONAL;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL;
            }
            break;

        case ICF_SERVICE_CALL_FWD_BUSY: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_CALL_FWD_BUSY;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_CALL_FWD_BUSY;
            }
            break;

        case ICF_SERVICE_CALL_FWD_NO_ANSWER: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_CALL_FWD_NO_ANSWER;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER;
            }
            break;

        case ICF_SERVICE_CALL_WAITING: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_CALL_WAITING;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_CALL_WAITING;
            }
            break;
#ifndef ICF_FEATURE_ABANDON
        case ICF_SERVICE_MESSAGE_WAITING_IND: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_MESSAGE_WAITING_IND;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_MESSAGE_WAITING_IND;
            }
            break;
#endif
        case ICF_SERVICE_EXTERNAL_CONFERENCE: 
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_EXTERNAL_CONFERENCE;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_EXTERNAL_CONFERENCE;
            }
            break;
         case ICF_SERVICE_JOIN:
            if(ICF_TRUE == subscription_request)
            {
                ret_srvc_id = ICF_SERVICE_SUBS_JOIN_HEADER;
            }
            else
            {
                ret_srvc_id = ICF_SERVICE_ACT_JOIN_HEADER;
            }
            break;
        default:break;
    }
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_srvc_id;
}




/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_act_flag
 *
 * DESCRIPTION:     This function will check if the services that have been 
 *                  requested to be activated have valid data on the lines.
 *                  
 * RETURNS:         ICF_SUCCESS if validation is successful
 *                  ICF_FAILURE if validation fails.
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_act_flag(
         INOUT  icf_crm_pdb_st           *p_crm_pdb, 
         IN     icf_line_id_t             line_id)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_service_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_uint16_t                         status;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_service_params_req_st *)
        (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    status = p_recv_msg_payload->service_activation_flag.status;
 
    if(ICF_NULL != (status & ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL) &&
            (ICF_NULL == icf_port_strlen(p_crm_pdb->p_line_data[line_id]->
                                               addr_for_cfu.addr_val.str)))
    {
        /* If Activation of cfu service is requested and cfu address is not
         * present on line, check if cfu address is present in payload */
        if(!(ICF_CFG_CC_SET_SER_PRM_CFU_ADDR & 
                p_recv_msg_payload->bit_mask)) 
        {
            /* CFU Address is not present in payload */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
            ret_val = ICF_FAILURE;
        }
        else if((ICF_ALL_LINES == p_recv_msg_payload->service_activation_flag.line_id) 
			
                && (ICF_ALL_LINES != p_recv_msg_payload->addr_for_cfu.line_id))
        {
            /* Else if CFU is present in payload, but for a specific line wheras
             * activation flag is for all lines, CFU address will not be present
             * for all ines, Fail API */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
            ret_val = ICF_FAILURE;
        }
        else if((ICF_ALL_LINES != p_recv_msg_payload->service_activation_flag.line_id)
			&&  (line_id != p_recv_msg_payload->addr_for_cfu.line_id))
        {
            /* Else if CFU address is present in payload for a specific line, 
             * but line id is not the same as for activation flag, Fail API */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
            ret_val = ICF_FAILURE;
        }
    }
    else if(ICF_NULL != (status & ICF_SERVICE_ACT_CALL_FWD_BUSY) &&
            (ICF_NULL == icf_port_strlen(p_crm_pdb->p_line_data[line_id]->
                                               addr_for_cfb.addr_val.str)))
    {
        /* If Activation of cfb service is requested and cfb address is not
         * present on line, check if cfb address is present in payload */
        if(!(ICF_CFG_CC_SET_SER_PRM_CFB_ADDR & 
                p_recv_msg_payload->bit_mask)) 
        {
            /* CFB Address is not present in payload */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
            ret_val = ICF_FAILURE;
        }
        else if((ICF_ALL_LINES == p_recv_msg_payload->service_activation_flag.
                    line_id)
                && (ICF_ALL_LINES != p_recv_msg_payload->addr_for_cfb.line_id))
        {
            /* Else if CFB is present in payload, but for a specific line wheras
             * activation flag is for all lines, CFB address will not be present
             * for all ines, Fail API */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
            ret_val = ICF_FAILURE;
        }
        /*  Fix for spr 18784 */
        /*  Scenario : when activatio flag is for all lines.
                       and address for cfb is also for all lines.
                       THen also it is giving failure .*/
        else if((ICF_ALL_LINES != p_recv_msg_payload->service_activation_flag.
                    line_id) &&
                (line_id != p_recv_msg_payload->addr_for_cfb.line_id))
        {
            /* Else if CFB address is present in payload for a specific line, 
             * but line id is not the same as for activation flag, Fail API */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
            ret_val = ICF_FAILURE;
        }
    }
    else if(ICF_NULL != (status & 
                ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER) &&
            (ICF_NULL == icf_port_strlen(p_crm_pdb->p_line_data[line_id]->
                                               addr_for_cfd.addr_val.str)))
    {
        /* If Activation of cfd service is requested and cfd address is not
         * present on line, check if cfd address is present in payload */
        if(!(ICF_CFG_CC_SET_SER_PRM_CFD_ADDR & 
                p_recv_msg_payload->bit_mask)) 
        {
            /* CFD Address is not present in payload */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
            ret_val = ICF_FAILURE;
        }
        /*  Fix for spr 18784 */
        /*  SCENARIO : if activation flag line id 0 and cfd address line id is also 0
            the this check was getting failed previously */
        else if((ICF_ALL_LINES == p_recv_msg_payload->service_activation_flag.line_id)
                && (ICF_ALL_LINES != p_recv_msg_payload->addr_for_cfd.line_id))
        {
            /* Else if CFD is present in payload, but for a specific line wheras
             * activation flag is for all lines, CFD address will not be present
             * for all ines, Fail API */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
            ret_val = ICF_FAILURE;
        }
        else if((ICF_ALL_LINES != p_recv_msg_payload->service_activation_flag.line_id) &&
                (line_id != p_recv_msg_payload->addr_for_cfd.line_id))
        {
            /* Else if CFD address is present in payload for a specific line, 
             * but line id is not the same as for activation flag, Fail API */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
            ret_val = ICF_FAILURE;
        }

    }

#ifndef ICF_FEATURE_ABANDON
    else if(ICF_NULL != (status & ICF_SERVICE_ACT_MESSAGE_WAITING_IND) &&
            (ICF_NULL == p_crm_pdb->p_line_data[line_id]->p_vms_addr))
    {
        /* If MWI is requested to be activated but there is no VMS addr on the
         * line, Fail API */
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED)
        ret_val = ICF_FAILURE;
    }
#endif

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_check_subs_flag
 *
 * DESCRIPTION:     This function will check if service that has to be activated
 *                  has been subscribed or not.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_check_subs_flag(
         INOUT  icf_crm_pdb_st           *p_crm_pdb, 
         IN     icf_line_id_t            line_id, 
         IN     icf_uint16_t             service)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_service_params_req_st *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_service_params_req_st *)
        (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
   	if ( line_id < ICF_MAX_NUM_OF_LINES ) 
	{ 
    /* First check if the service is subscribed on the line data */
    if(ICF_NULL != (service & p_crm_pdb->p_line_data[line_id]->
                            service_subscription_flag))
    {
        /* Set the value in global array. This will be used in case 
         * all validations are a success
         */
         ICF_CFG_SET_SRVC_ACT_BITMASK(act_status[line_id], service)
    }
    else
    {
         ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    /* Check if subscription flag is present in payload */ 
    if(ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_FLAG &
             p_recv_msg_payload->bit_mask))
    {
        if(ICF_ALL_LINES == p_recv_msg_payload->service_subscription_flag.line_id) 
        {
            if(ICF_NULL != (service & p_recv_msg_payload->
                        service_subscription_flag.status))
            {
                /* If srvc subscription flag is present in payload for 
                 * all lines = true and the srvc is being requested for 
                 * subscribe */
                ICF_CFG_SET_SRVC_ACT_BITMASK(act_status[line_id], service)
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_TRUE)
            }
            else
            {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            }
        }
        /* All lines = false, check for the line id specified. */
        else if(line_id == p_recv_msg_payload->service_subscription_flag.
                line_id)
        {
            if(ICF_NULL != (service & p_recv_msg_payload->
                                    service_subscription_flag.status))
            {
                /* If srvc subscription flag is presesnt in payload for 
                 * all lines = false and it is for same line id, and the 
                 * srvc is being requested for subscribe */
                 ICF_CFG_SET_SRVC_ACT_BITMASK(act_status[line_id], service)
                 ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_TRUE)
            }
            else
            {
                 ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            }
        }
    }
	}

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}
                

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_act_flag
 *
 * DESCRIPTION:     This function will the activation flag after checking
 *                  subscription status and address for various services
 *                  
 * RETURNS:         ICF_SUCCESS if validation is successful
 *                  ICF_FAILURE if validation fails.
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_act_flag(
         INOUT  icf_crm_pdb_st           *p_crm_pdb, 
         IN     icf_line_id_t             line_id)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_service_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_uint16_t                         srvc_index = ICF_NULL;
    icf_uint16_t                         status = ICF_NULL;
    icf_uint16_t                         service = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_service_params_req_st *)
        (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
    
    status = p_recv_msg_payload->service_activation_flag.status;

    if (ICF_SUCCESS == icf_cfg_validate_act_flag(
                p_crm_pdb, line_id))
    {
        /* Set the activation flag bits only for the services that are 
         * subscribed */
        for (srvc_index = ICF_SERVICE_CALL_HOLD_RESUME; 
                srvc_index < ICF_MAX_SERVICE; srvc_index++)
        {
            service = icf_cfg_ret_srvc_id(p_crm_pdb, srvc_index, 
                    (icf_boolean_t)ICF_FALSE);
            
            /* If service is being activated */
            if(ICF_NULL != (service & status))
            {
                icf_cfg_check_subs_flag(p_crm_pdb, line_id, service);
                if(ICF_FALSE == p_crm_pdb->is_valid_param)
                {
                    /* If any service is not subscribed, return failure
                     * as srvc needs to be subscribed before activation*/
                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb, 
                            ICF_FAILURE,
                            ICF_CAUSE_SERVICE_NOT_SUBSCRIBED)
                    ret_val = ICF_FAILURE;
                    break;
                }
            }
            /* If service is being deactivated, reset the bit */
            else
            {
                ICF_CFG_RESET_BITMASK(act_status[line_id], service)
            }
        }/* End for(srvc_index ==..*/
    }
    else
    {
        ret_val = ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; 
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_srvc_line_prm
 *
 * DESCRIPTION:     This function will save the service param related to 
 *                  line data into global database.
 *                  
 * RETURNS:         ICF_SUCCESS if validation of line params is successful
 *                  ICF_FAILURE if validation of line params fails
 ******************************************************************************/
icf_return_t icf_cfg_set_srvc_line_prm(
         INOUT  icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_service_params_req_st *p_recv_msg_payload = ICF_NULL;
    icf_line_id_t                        line_index = 0;
    icf_line_id_t                        line_id = 0;
    icf_uint16_t                         srvc_index = 0;
    /*Fix Merged for CSR_1_5400409*/
    icf_uint16_t                         status = 0;
    icf_uint16_t                         service = 0;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_service_params_req_st *)
        (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
    
    /* Check for the presence of all the parameters and then set the values
     * received in the API in the line data.
     */
  
    /*-----------------------------------------------------------------------*/ 
    /* Check for presence of service activation flag 
     * The activation flag is set first, since it can result in failure resp and
     * we do not need to roll back any of the srvc params.
     */
        
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_ACTIVATION_FLAG &
                p_recv_msg_payload->bit_mask))
    {
        /* Check if all_lines is true */
        if(ICF_ALL_LINES == p_recv_msg_payload->service_activation_flag.line_id)
        {
            /* Value in first array element is applicable for all lines 
             * configured till now and the pointers of these lines will be in 
             * the pdb. The status in the line_status field will be true for 
             * the lines whose pointers are available i.e the lines which are 
             * configured till now
             */
            for (line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                        max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if (ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    if(ICF_FAILURE == icf_cfg_set_act_flag(p_crm_pdb, 
                                line_index))
                    {
                        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
			ret_val = ICF_FAILURE;
                        return ret_val;
                    }
                }
            }
            /* If execution reaches this line, it means that none of the 
             * validations failed for any of the lines and now the value in
             * payload can be updated
             */
            for (line_index = 0; 
				(line_index < (p_crm_pdb->p_config_data->max_lines)) &&
				(line_index < ICF_MAX_NUM_OF_LINES) ; line_index++)
            {
                if (ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    p_crm_pdb->p_line_data[line_index]->service_activation_flag 
                        = act_status[line_index];
                }
            }
        }/* End if (ICF_TRUE == .. */        
        else
        {
            line_id = p_recv_msg_payload->service_activation_flag.line_id;
            /* If line is configured */
            if ((line_id < ICF_MAX_NUM_OF_LINES) && (ICF_TRUE == p_crm_pdb->requested_lines[line_id]))
            {
                if(ICF_FAILURE  == icf_cfg_set_act_flag(p_crm_pdb, 
                            line_id))
                {
                    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                    ret_val = ICF_FAILURE;
                    return ret_val;
                }
                else
                {
                    /* Update the line data with act flag */
                    p_crm_pdb->p_line_data[line_id]->service_activation_flag = 
                            act_status[line_id];
                    p_crm_pdb->line_id = line_id;
                    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_FLAGS_SET)
                }
            }/* End if(ICF_TRUE = .. */                         
        }/* End else */
    }/* End if(ICF_NULL != .. */  
   

    /*-----------------------------------------------------------------------*/ 
    /* Check for presence of service subscription flag */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_FLAG &
                p_recv_msg_payload->bit_mask))
    {
        /* Check if all_lines is true */
        if(ICF_ALL_LINES == p_recv_msg_payload->service_subscription_flag.line_id) 
        {
            /* Value in payload is applicable for all lines configured till now 
             * and the pointers of these lines will be in the pdb. The status 
             * in the line_status field will be true for the lines whose 
             * pointers are available i.e the lines which are configured till 
             * now
             */
            status = p_recv_msg_payload->service_subscription_flag.status;

            for (line_index = 0; 
				(line_index < (p_crm_pdb->p_config_data->max_lines)) &&
				(line_index < ICF_MAX_NUM_OF_LINES) ; line_index++)
            {
                if(ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    p_crm_pdb->p_line_data[line_index]->
                        service_subscription_flag = status;

                     /* Deactivate the services which have been unsubscribed */
                    for (srvc_index = ICF_SERVICE_CALL_HOLD_RESUME; 
                            srvc_index < ICF_MAX_SERVICE; srvc_index++)
                    {
                        /* Fetch the bitmask of the service */
                        service = icf_cfg_ret_srvc_id(p_crm_pdb, srvc_index, 
                                (icf_boolean_t)ICF_TRUE);

                        if(ICF_NULL == (service & status))
                        {
                            ICF_CFG_RESET_BITMASK(
                                    p_crm_pdb->p_line_data[line_index]->
                                    service_activation_flag, 
                                    service)
                        }
                    }
                    p_crm_pdb->line_id = line_index;
                    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_FLAGS_SET)
                }
            }/* End for */
        }/* End if (ICF_TRUE == .. */        
        else
        {
            line_id = p_recv_msg_payload->service_subscription_flag.line_id;
            /* If line is configured */
            if((line_id < ICF_MAX_NUM_OF_LINES) && (ICF_TRUE == p_crm_pdb->requested_lines[line_id])) 
            {
                status = p_recv_msg_payload->service_subscription_flag.status;

                p_crm_pdb->p_line_data[line_id]->service_subscription_flag = 
                    status;

                /* Deactivate the services which have been unsubscribed */
                for (srvc_index = ICF_SERVICE_CALL_HOLD_RESUME; 
                        srvc_index < ICF_MAX_SERVICE; srvc_index++)
                {
                    /* Fetch the bitmask of the service */
                    service = icf_cfg_ret_srvc_id(p_crm_pdb, srvc_index, 
                            (icf_boolean_t)ICF_TRUE);

                    if(ICF_NULL == (service & status))
                    {
                        ICF_CFG_RESET_BITMASK(
                                p_crm_pdb->p_line_data[line_id]->
                                service_activation_flag, 
                                service)

                    }
                }
                p_crm_pdb->line_id = line_id;
                ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SRVC_FLAGS_SET)
            }/* End if(ICF_TRUE == .. */
        }/* End else for all_lines */
    }/* End if(ICF_NULL != .. */
            
    /*-----------------------------------------------------------------------*/ 
    /* Check for the presence of addr_for_cfu */
    if(ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CFU_ADDR &
                p_recv_msg_payload->bit_mask))
    {
        if (ICF_ALL_LINES == p_recv_msg_payload->addr_for_cfu.line_id)
        {
            /* Address will be set on all lines configured till now.
             * These are the lines for which the pointers are received 
             */
            for (line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index< ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if (ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    icf_port_memcpy(
                            (icf_void_t*)&(p_crm_pdb->
                                        p_line_data[line_index]->addr_for_cfu),
                            (icf_void_t*)&(p_recv_msg_payload->addr_for_cfu.
                                        address),
                            (icf_uint32_t)
                            ICF_PORT_SIZEOF(icf_address_st));
            
                    p_crm_pdb->line_id = line_index;    
                    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFU_ADDR_SET)
                }                    
            }                    
        }/* End if (ICF_TRUE ==.. */
        else
        {
            /* Add the address received on the line for which the request is 
             * there in the API. The pointer will be present in the pdb.
             */
            
            line_id = p_recv_msg_payload->addr_for_cfu.line_id;

            /* If line is configured */
            if((line_id < ICF_MAX_NUM_OF_LINES) && (ICF_TRUE == p_crm_pdb->requested_lines[line_id]) )
            {
                icf_port_memcpy(
                        (icf_void_t*)&(p_crm_pdb->p_line_data[line_id]->
                                          addr_for_cfu),
                        (icf_void_t*)&(p_recv_msg_payload->addr_for_cfu.
                                          address),
                        (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));

                p_crm_pdb->line_id = line_id;
                ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFU_ADDR_SET)
            }
        }/* End else */        
    }/* End if(ICF_NULL != .. */
#ifdef ICF_SERVICE_ENABLE    
    /*-----------------------------------------------------------------*/
    /* Check for the presence of conference_uri for xconf */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CONFERENCE_URI_PRESENT &
                p_recv_msg_payload->bit_mask))
    {
        if (ICF_ALL_LINES == p_recv_msg_payload->conference_uri.line_id)
        {
            /* Address will be set on all lines configured till now.
             * These are the lines for which the pointers are received 
             */
            for (line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if (ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    icf_port_memcpy(
                      (icf_void_t*)&(p_crm_pdb->p_line_data[line_index]->conf_server_uri),
                      (icf_void_t*)&(p_recv_msg_payload->conference_uri.address),
                      (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));

                    p_crm_pdb->line_id = line_index;
                }                    
            }                    
        }/* End if (ICF_TRUE ==.. */
        else
        {
            /* Add the address received on the line for which the request is 
             * there in the API. The pointer will be present in the pdb.
             */
            
            line_id = p_recv_msg_payload->conference_uri.line_id;
                
            /* If line is configured store the conf server uri coming
               in set service params API in line data*/
            if((line_id < ICF_MAX_NUM_OF_LINES) && (ICF_TRUE == p_crm_pdb->requested_lines[line_id]) )
            {
                icf_port_memcpy(
                (icf_void_t*)&(p_crm_pdb->p_line_data[line_id]->conf_server_uri),
                (icf_void_t*)&(p_recv_msg_payload->conference_uri.address),
                (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));

                p_crm_pdb->line_id = line_id;
            }
        }/* End else */        
    }/* End if(ICF_NULL != .. */
#endif    

    /*-----------------------------------------------------------------------*/ 
    /* Check for the presence of addr_for_cfb */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CFB_ADDR &
                p_recv_msg_payload->bit_mask))
    {
        if (ICF_ALL_LINES == p_recv_msg_payload->addr_for_cfb.line_id)
        {
            /* Address will be set on all lines configured till now.
             * These are the lines for which the pointers are received 
             */
            for (line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if (ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    icf_port_memcpy(
                            (icf_void_t*)&(p_crm_pdb->p_line_data[line_index]
                                              ->addr_for_cfb),
                            (icf_void_t*)&(p_recv_msg_payload->addr_for_cfb.
                                              address),
                            (icf_uint32_t)
                            ICF_PORT_SIZEOF(icf_address_st));

                    p_crm_pdb->line_id = line_index;
                    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFB_ADDR_SET)
                }                    
            }                    
        }/* End if (ICF_TRUE ==.. */
        else
        {
            /* Add the address received on the line for which the request is 
             * there in the API. The pointer will be present in the pdb.
             */
            
            line_id = p_recv_msg_payload->addr_for_cfb.line_id;
                
            /* If line is configured */
            if((line_id < ICF_MAX_NUM_OF_LINES) && (ICF_TRUE == p_crm_pdb->requested_lines[line_id]) )
            {
                icf_port_memcpy(
                        (icf_void_t*)&(p_crm_pdb->p_line_data[line_id]->
                                          addr_for_cfb),
                        (icf_void_t*)&(p_recv_msg_payload->addr_for_cfb.
                                          address),
                        (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));

                p_crm_pdb->line_id = line_id;
                ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFB_ADDR_SET)
            }
        }/* End else */        
    }/* End if(ICF_NULL != .. */

    /*-----------------------------------------------------------------------*/ 
    /* Check for the presence of addr_for_cfd */
    if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CFD_ADDR &
                p_recv_msg_payload->bit_mask))
    {
        if (ICF_ALL_LINES == p_recv_msg_payload->addr_for_cfd.line_id)
        {
            /* Address will be set on all lines configured till now.
             * These are the lines for which the pointers are received 
             */
            for (line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                    max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
            {
                if (ICF_TRUE == p_crm_pdb->lines_present[line_index])
                {
                    icf_port_memcpy(
                            (icf_void_t*)
                            &(p_crm_pdb->p_line_data[line_index]->addr_for_cfd),
                            (icf_void_t*)&(p_recv_msg_payload->addr_for_cfd.
                                              address),
                            (icf_uint32_t)
                            ICF_PORT_SIZEOF(icf_address_st));
				#if 1 /*ZyXEL , Jason , [RD Internal #231]*/
		                /*BL2348_iptk let's noans fwd timer per-line*/
					if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CND_ALERT_TIMER &
			                p_recv_msg_payload->bit_mask))
			   		{
						p_crm_pdb->p_line_data[line_index]->cnd_alert_timer =  p_recv_msg_payload->cnd_alert_timer;
					}
					/*BL2348_iptk end*/
				#endif
                    p_crm_pdb->line_id = line_index;
                    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFD_ADDR_SET)                    
                }                    
            }                    
        }/* End if (ICF_TRUE ==.. */
        else
        {
            /* Add the address received on the line for which the request is 
             * there in the API. The pointer will be present in the pdb.
             */
            
            line_id = p_recv_msg_payload->addr_for_cfd.line_id;
                
            /* If line is configured */
            if((line_id < ICF_MAX_NUM_OF_LINES) && (ICF_TRUE == p_crm_pdb->requested_lines[line_id]) )
            {                
                icf_port_memcpy(
                        (icf_void_t*)&(p_crm_pdb->p_line_data[line_id]->
                                          addr_for_cfd),
                        (icf_void_t*)&(p_recv_msg_payload->addr_for_cfd.
                                          address),
                        (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
				#if 1 /*ZyXEL , Jason , [RD Internal #231]*/			
				/*BL2348_iptk let's noans fwd timer per-line*/
				if (ICF_NULL != (ICF_CFG_CC_SET_SER_PRM_CND_ALERT_TIMER &
		                p_recv_msg_payload->bit_mask))
		   		 {
					p_crm_pdb->p_line_data[line_id]->cnd_alert_timer =  p_recv_msg_payload->cnd_alert_timer;
				}
				/*BL2348_iptk end*/
				#endif
                p_crm_pdb->line_id = line_id;
                ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_CFD_ADDR_SET)
            }
        }/* End else */        
    }/* End if(ICF_NULL != .. */    

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */

#endif /*SERVICE_ENABLE flag ends here*/
    
#ifndef ICF_FEATURE_ABANDON
/****************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_vms_grp_prms
 *
 * DESCRIPTION:     This function validates the vms address in case of group 
 *                  addition.
 *
 * RETURN           ICF_SUCCESS always
 *
 * ****************************************************************************/
icf_return_t icf_cfg_validate_vms_grp_prms(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_vms_params_req_st     *p_recv_msg_payload = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_vms_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
    
    if((ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action) && 
            (ICF_NULL != p_crm_pdb->p_config_data->p_group_vms_addr))
    {
        /* Address already exists */
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_ACTION)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    else if((ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action) || 
            (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action))
    {
        /* If action is delete or add, check subscription status */
        if((ICF_TRUE == p_crm_pdb->global_data_init_complete) && 
                (ICF_NULL != (ICF_CFG_VMS_LINE_SUBSCRIBED &
                    p_crm_pdb->p_config_data->vms_subscription_status)))
        {
            /* If init complete is done and subscription is for line */
            /* Mode cannot be changed to grp after init complete */
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_GROUP_SUBSCRIPTION_NOT_ALLOWED)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        }
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}


/****************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_vms_line_prm
 *
 * DESCRIPTION:     This function validates the vms address in case of per 
 *                  line addition.
 *
 * RETURN:          ICF_SUCCESS if line is configured
 *                  ICF_FAILURE if line is not configured
 *
 ****************************************************************************/
icf_return_t icf_cfg_validate_vms_line_prm(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_line_id_t            line_id)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    icf_uint8_t                        line_status[ICF_MAX_NUM_OF_LINES];
    icf_cfg_cc_set_vms_params_req_st   *p_recv_msg_payload = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_vms_params_req_st *)
                            (p_crm_pdb->p_recv_payload);  
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    /* Invoke function to fetch line pointer. */
    ret_val = icf_cfg_check_line_in_db(p_crm_pdb, 
            line_id, 
            line_status,
            (icf_boolean_t)ICF_FALSE);
    
    if(ICF_FAILURE == ret_val && (ICF_FALSE == 
                p_recv_msg_payload->all_lines))
    {
        /* If line is not allocated or invalid line id */
        if(ICF_STATUS_INVALID_LINE_ID ==  
                p_crm_pdb->p_internal_msg->msg_hdr.ecode)
        {
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INVALID_LINE_ID)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        }
        else if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            /* Line is not configured
             */ 
            ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_LINE_NOT_CONFIGURED)
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        }
        
    }
    else if(ICF_SUCCESS == ret_val)
    {
        /* If line pointer is returned */
        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            if(ICF_NULL != p_crm_pdb->p_line_data[line_id]->p_vms_addr)
            {
                /* VMS addr already exists */ 
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_ACTION)
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            }
            else if((ICF_CFG_VMS_GRP_SUBSCRIBED == 
                        (p_crm_pdb->p_config_data->vms_subscription_status & 
                         ICF_CFG_VMS_GRP_SUBSCRIBED)) && 
                    (ICF_NULL == p_crm_pdb->p_config_data->p_group_vms_addr))
            {
                /* First VMS group addr should be added */
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_ACTION)
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            }
        }

    }
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}
#endif
#ifndef ICF_FEATURE_ABANDON
/****************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_vms_params
 *
* DESCRIPTION:     This function validates the vms prms.
 *
 * RETURN:          ICF_SUCCESS always
 *
 ****************************************************************************/
icf_return_t icf_cfg_validate_vms_params(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_line_id_t                        line_index = 0;
    icf_cfg_cc_set_vms_params_req_st     *p_recv_msg_payload = ICF_NULL;
    icf_boolean_t                        line_configured = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_vms_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_VMS_PRMS_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_VMS_ADDR_PRMS_RECD)
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_VMS_OTHER_PRMS_RECD)

    if(((ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action) || 
            (ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->action)) &&
            ICF_CFG_MIN_TIMER_VALUE > p_recv_msg_payload->subscribe_duration)
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_INVALID_TIMER_VALUE)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
    else if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
    {
        /* If action is add, vms_account_addr will be present in payload. */
        if(ICF_FAILURE == icf_cmn_validate_trnspt_addr(
                    p_crm_pdb->p_glb_pdb,
                    &(p_recv_msg_payload->vms_account_addr), 
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_ADDRESS_RECEIVED)
        }

        else if((p_crm_pdb->p_config_data->self_mode != 
                        ICF_TRANSPORT_MODE_BOTH) 
                && (p_recv_msg_payload->vms_account_addr.transport != 
                        ICF_TRANSPORT_MODE_BOTH))
        {
        	if(p_recv_msg_payload->vms_account_addr.transport != 
                    p_crm_pdb->p_config_data->self_mode)
        	{
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                		ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE)			
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                return ret_val;
    	    }
        }	

    }
    if((ICF_TRUE == p_recv_msg_payload->group_account) && (ICF_TRUE ==
                p_crm_pdb->is_valid_param))
    {
        /* Invoke function to check group validations */
        ret_val = icf_cfg_validate_vms_grp_prms(p_crm_pdb);

    }
    else if((ICF_FALSE == p_recv_msg_payload->group_account) && 
            (ICF_TRUE ==  p_crm_pdb->is_valid_param))
    {  
        /* This is the case when request is to add VMS per line */
        /* Invoke function to check line validations */

        if(ICF_TRUE == p_recv_msg_payload->all_lines)
        {
            for (line_index = 0;line_index < p_crm_pdb->p_config_data-> 
                    max_lines;line_index++)
            {                    
                if(ICF_SUCCESS == 
                        icf_cfg_validate_vms_line_prm(p_crm_pdb, 
                            line_index))
                {
                    /* Line is configured if Success is returned */
                    line_configured = ICF_TRUE;
                
                    /* Line is configured  */
                    p_crm_pdb->lines_present[line_index] = ICF_TRUE;
                }                                             
            }
            if((ICF_FALSE == line_configured) && 
                    (ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action))
            {
                /* If none of the lines are configured, 
                 * VMS cannot be added 
                     */
                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            ICF_CAUSE_LINE_NOT_CONFIGURED)
                    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
            }

        }
        else
        {
            if(ICF_SUCCESS == icf_cfg_validate_vms_line_prm(p_crm_pdb, 
                    p_recv_msg_payload->line_id))
            {
                p_crm_pdb->lines_present[p_recv_msg_payload->line_id] = ICF_TRUE;
            }
        }
    }/* End if VMS_ADDR present */
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}
   

/****************************************************************************
 *
 * FUNCTION:        icf_cfg_set_vms_prms_in_db
 *
 * DESCRIPTION:     This function sets the vms prms. This includes 
 *                   addition/deletion and modification of vms duration.
 *
 * RETURN:          ICF_SUCCESS always
 *
 *****************************************************************************/
icf_return_t icf_cfg_set_vms_prms_in_db(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_vms_params_req_st     *p_recv_msg_payload = ICF_NULL;
    icf_error_t                       *p_ecode = ICF_ERROR_NONE;
    icf_line_id_t                        line_index = 0;
    icf_line_id_t                        line_id = 0;
    icf_cfg_cc_set_vms_params_req_st     *p_recv_payload = ICF_NULL;
    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_vms_params_req_st *)
                            (p_crm_pdb->p_recv_payload);
    
    p_recv_payload = (icf_cfg_cc_set_vms_params_req_st *)
        (p_crm_pdb->p_recv_payload);
    
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))
   
    /* Validate the parameters received */
    ret_val = icf_cfg_validate_vms_params(p_crm_pdb);
    
    if(ICF_FALSE == p_crm_pdb->is_valid_param)
    {
        /* Invalid Data */
        ret_val = ICF_SUCCESS;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }
    
       
    if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
    {
        p_recv_payload->vms_account_addr.addr_type = 
            ICF_ADDRESS_PLAN_UNDEFINED; 
    }

    
    if(ICF_TRUE == p_recv_msg_payload->group_account)
    {
        /* If request is for group account */
        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            /* Copy VMS duration */
            p_crm_pdb->p_config_data->vms_subscribe_duration = 
                p_recv_msg_payload->subscribe_duration;
            
            /* Allocate memory for VMS Group Address */
            ICF_STATIC_MEMGET(p_crm_pdb->p_glb_pdb,
                    sizeof(icf_address_st),
                    p_crm_pdb->p_config_data->p_group_vms_addr,
                    ICF_DONOT_RET_ON_FAILURE,
                    p_ecode, ret_val)

           if(ICF_SUCCESS == ret_val)
           {
               /* Memcpy VMS group address */
               icf_port_memcpy(
                       (icf_void_t*)
                       (p_crm_pdb->p_config_data->p_group_vms_addr),
                       (icf_void_t*)&(p_recv_msg_payload->vms_account_addr),
                       (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st)) ;

               /* Set bitmask to indicate group VMS addr has been added */
               ICF_CFG_SET_BITMASK(p_crm_pdb->p_config_data->
                       vms_subscription_status,
                       ICF_CFG_VMS_GRP_SUBSCRIBED)

               /* Trace for the group VMS data set */                   
               ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_VMS_GRP_PRMS_SET)
               ICF_CFG_TRACE(p_crm_pdb, 
                       ICF_CFG_TRACE_VMS_GRP_ADDR_PRMS_SET)
           }
           else
           {

               ICF_CFG_SET_RESPONSE(
                       p_crm_pdb,
                       ICF_FAILURE,
                       ICF_CAUSE_INSUFFICIENT_RESOURCES)
               ret_val = ICF_FAILURE;
           }

        }
        else if((ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
                && (ICF_NULL != p_crm_pdb->p_config_data->p_group_vms_addr))
        {

            /* This is required to delete the server address by the MWI
             * otherwise there is no way to get that as the server address is 
             * deleted before MWI is called
            * */
            
           icf_port_memcpy((icf_void_t*)
                              &(p_recv_payload->vms_account_addr),
                               (icf_void_t*)
                               (p_crm_pdb->p_config_data->p_group_vms_addr),
                               (icf_uint32_t)
                               ICF_PORT_SIZEOF(icf_address_st)); 
            
            /* If action is delete and Group VMS address is present */
            /* Free memory */
            ICF_STATIC_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_crm_pdb->p_config_data->p_group_vms_addr,
                    p_ecode,
                    ret_val)

            /* Set VMS group address to NULL */
            p_crm_pdb->p_config_data->p_group_vms_addr = ICF_NULL;

            /* Set VMS group duration to Default Value */
            p_crm_pdb->p_config_data->vms_subscribe_duration = 
                ICF_CFG_DEF_SUBS_DURATION;

            if(ICF_FALSE == p_crm_pdb->global_data_init_complete)
            {
                /* Reset bitmask to indicate group account is not present
                 * only if init complete is not done. Once init complete is
                 * done, even if grp account is deleted subscription will 
                 * remain for group.
                 */
                ICF_CFG_RESET_BITMASK(p_crm_pdb->p_config_data->
                        vms_subscription_status, ICF_CFG_VMS_GRP_SUBSCRIBED)
            
                /* The VMS grp parameters has been reset*/    
                ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_VMS_GRP_PRMS_SET)
            }
        }
        else if(ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->action)
        {
            /* If action is modify */
            /* Copy VMS duration */
            p_crm_pdb->p_config_data->vms_subscribe_duration = 
                p_recv_msg_payload->subscribe_duration;
            ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_VMS_GRP_PRMS_SET)
        }
    }
    else
    {
        /* VMS addition is per line */
        if(ICF_TRUE == p_recv_msg_payload->all_lines)
        {
            for (line_index = 0; line_index < p_crm_pdb->p_config_data-> 
                    max_lines; line_index++)
            {
               p_crm_pdb->line_id = line_index;
               if((ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
                  && (ICF_TRUE ==  p_crm_pdb->lines_present[line_index]))
               {
                    /* Pointers have already been fetched for all 
                     * configured lines during validation. VMS address can 
                     * be added
                     */

                    /* Copy duration into line VMS duration */
                    p_crm_pdb->p_line_data[line_index]->vms_subscribe_duration =
                       p_recv_msg_payload->subscribe_duration;
                    ICF_CFG_TRACE(p_crm_pdb,
                            ICF_CFG_TRACE_VMS_LINE_PRMS_SET)

                    /* Allocate memory for VMS Line Address */
                    ICF_STATIC_MEMGET(p_crm_pdb->p_glb_pdb,
                            sizeof(icf_address_st),
                            p_crm_pdb->p_line_data[line_index]->p_vms_addr,
                            ICF_DONOT_RET_ON_FAILURE,
                            p_ecode, ret_val)

                    if(ICF_SUCCESS == ret_val)
                    {
                        /* Memcopy VMS address given in payload to line */
                        icf_port_memcpy(
                               (icf_void_t*)
                               (p_crm_pdb->p_line_data[line_index]->p_vms_addr),
                               (icf_void_t*)
                               &(p_recv_msg_payload->vms_account_addr),
                               (icf_uint32_t)
                               ICF_PORT_SIZEOF(icf_address_st));

                        /* Increment value in bitmask to indicate the number of
                         * lines on which VMS is added 
                         */
                        p_crm_pdb->p_config_data->vms_subscription_status++;
                       
                        /* VMS status has been set */
                        
                       ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_VMS_GRP_PRMS_SET); 

                        /* In case VMS is configured after init complete and gro                                            up subscription is not done*/ 

                     if((ICF_TRUE == p_crm_pdb->global_data_init_complete) &&
                        (ICF_NULL == (ICF_CFG_VMS_GRP_SUBSCRIBED &
                        p_crm_pdb->p_config_data->vms_subscription_status)))
                    {
                        ICF_CFG_SET_BITMASK(p_crm_pdb->p_config_data->
                        vms_subscription_status, 
                        ICF_CFG_VMS_LINE_SUBSCRIBED)
                    }
        
                       
                       ICF_CFG_TRACE(p_crm_pdb, 
                                ICF_CFG_TRACE_VMS_LINE_ADDR_PRMS_SET)
                    }
                    else
                    {
                        ICF_CFG_SET_RESPONSE(
                                p_crm_pdb,
                                ICF_FAILURE,
                                ICF_CAUSE_INSUFFICIENT_RESOURCES)
                        ret_val = ICF_FAILURE;
                    }
                }/* End Action Add*/
                else if((ICF_CONFIG_ACTION_DELETE == 
                        p_recv_msg_payload->action) && 
                        (ICF_TRUE ==  p_crm_pdb->lines_present[line_index])
                         && (ICF_NULL != 
                         p_crm_pdb->p_line_data[line_index]->p_vms_addr))
                {
                
                    icf_port_memcpy((icf_void_t*)
                              &(p_recv_payload->vms_account_addr),
                               (icf_void_t*)
                              (p_crm_pdb->p_line_data[line_index]->p_vms_addr),
                               (icf_uint32_t)
                               ICF_PORT_SIZEOF(icf_address_st)); 

                
                    /* If action is delete and VMS address is present, delete it
                     */
                    /* Free memory */
                    ICF_STATIC_MEMFREE(
                            p_crm_pdb->p_glb_pdb,
                            p_crm_pdb->p_line_data[line_index]->p_vms_addr,
                            p_ecode,
                            ret_val)

                    p_crm_pdb->p_line_data[line_index]->p_vms_addr = 
                    ICF_NULL;

                    /* Decrement value in bitmask to indicate the number of
                     * lines on which VMS is added 
                     */
                    p_crm_pdb->p_config_data->vms_subscription_status--;

                    /* Set VMS group duration to Default Value */
                    p_crm_pdb->p_line_data[line_index]->vms_subscribe_duration =
                        ICF_CFG_DEF_SUBS_DURATION;

                    ICF_CFG_TRACE(p_crm_pdb, 
                            ICF_CFG_TRACE_VMS_LINE_PRMS_SET)
                }
                else if((ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->
                        action) && 
                        (ICF_TRUE ==  p_crm_pdb->lines_present[line_index])                           &&
                        (ICF_NULL != 
                         p_crm_pdb->p_line_data[line_index]->p_vms_addr) )
                {
                    /* Action is modify */
                    /* Copy VMS duration */
                    p_crm_pdb->p_line_data[line_index]->vms_subscribe_duration =
                        p_recv_msg_payload->subscribe_duration;
                    ICF_CFG_TRACE(p_crm_pdb, 
                            ICF_CFG_TRACE_VMS_LINE_PRMS_SET)
                }
            }/* End for */
        }/* End if(ICF_TRUE ==.. */
        else
        {
            /* All_lines = False */
            line_id = p_recv_msg_payload->line_id;
            p_crm_pdb->line_id = line_id;
            /* Pointer for this line has already been fetched 
             * during validation. VMS address can be added/deleted 
             */

            if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
            {
                /* Copy duration into line VMS duration */
                p_crm_pdb->p_line_data[line_id]->vms_subscribe_duration =
                    p_recv_msg_payload->subscribe_duration;
                ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_VMS_LINE_PRMS_SET)
               
                /* Allocate memory for VMS Line Address */
                ICF_STATIC_MEMGET(p_crm_pdb->p_glb_pdb,
                        sizeof(icf_address_st),
                        p_crm_pdb->p_line_data[line_id]->p_vms_addr,
                        ICF_DONOT_RET_ON_FAILURE,
                        p_ecode, ret_val)

                if(ICF_SUCCESS == ret_val)
                {
                    /* Memcopy VMS address given in payload to line */
                    icf_port_memcpy(
                            (icf_void_t*)
                            (p_crm_pdb->p_line_data[line_id]->p_vms_addr), 
                            (icf_void_t*)
                            &(p_recv_msg_payload->vms_account_addr), 
                            (icf_uint32_t)
                            ICF_PORT_SIZEOF(icf_address_st));

                    /* Increment value in bitmask to indicate the number of
                     * lines on which VMS is added 
                     */
                    p_crm_pdb->p_config_data->vms_subscription_status++;
                   
                    /* In case VMS is configured after init complete and gro                        up subscription is not done*/ 

                    if((ICF_TRUE == p_crm_pdb->global_data_init_complete) &&
                        (ICF_NULL == (ICF_CFG_VMS_GRP_SUBSCRIBED &
                        p_crm_pdb->p_config_data->vms_subscription_status)))
                    {
                        ICF_CFG_SET_BITMASK(p_crm_pdb->p_config_data->
                        vms_subscription_status, 
                        ICF_CFG_VMS_LINE_SUBSCRIBED)
                    }
                    

                        
                    ICF_CFG_TRACE(p_crm_pdb, 
                            ICF_CFG_TRACE_VMS_LINE_ADDR_PRMS_SET)
                }
                else
                {
                    ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            ICF_CAUSE_INSUFFICIENT_RESOURCES)
                    ret_val = ICF_FAILURE;
                }
            }
            else if((ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
                    && (ICF_TRUE == p_crm_pdb->lines_present[line_id])
                    && (ICF_NULL != 
                        p_crm_pdb->p_line_data[line_id]->p_vms_addr))
            {
            
            icf_port_memcpy((icf_void_t*)
                              &(p_recv_payload->vms_account_addr),
                               (icf_void_t*)
                               (p_crm_pdb->p_line_data[line_id]->p_vms_addr),
                               (icf_uint32_t)
                               ICF_PORT_SIZEOF(icf_address_st)); 

                /* If action is delete and Address is present, delete it */
                /* Free memory */
                ICF_STATIC_MEMFREE(
                        p_crm_pdb->p_glb_pdb,
                        p_crm_pdb->p_line_data[line_id]->p_vms_addr,
                        p_ecode,
                        ret_val)

                p_crm_pdb->p_line_data[line_id]->p_vms_addr = ICF_NULL;

                /* Decrement value in bitmask to indicate the number of
                 * lines on which VMS is added 
                 */
                p_crm_pdb->p_config_data->vms_subscription_status--;

                /* Set VMS group duration to Default Value */
                p_crm_pdb->p_line_data[line_id]->vms_subscribe_duration = 
                    ICF_CFG_DEF_SUBS_DURATION;
                ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_VMS_LINE_PRMS_SET)
            }
            else if ((ICF_CONFIG_ACTION_MODIFY == p_recv_msg_payload->action)
                      && (ICF_TRUE == p_crm_pdb->lines_present[line_id])  
                      && (ICF_NULL != 
                         p_crm_pdb->p_line_data[line_id]->p_vms_addr))
            {
                /* Action is modify */
                /* Copy VMS duration */
                p_crm_pdb->p_line_data[line_id]->vms_subscribe_duration =
                    p_recv_msg_payload->subscribe_duration;
                ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_VMS_LINE_PRMS_SET)
            }
        }/* End else for all lines = FALSE */
    }/* End else for Line addition*/

    if(ICF_TRUE == p_crm_pdb->global_data_init_complete)
    {
        /* Invoke function to trigger MWI for 
           subscription/unsubscription */
        ret_val = icf_cfg_mwi_trigger(p_crm_pdb, 
                (icf_uint8_t)ICF_CFG_SUBSCRIBE_TRIGGER);
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}

/****************************************************************************
 *
 * FUNCTION:        icf_cfg_mwi_trigger
 *
 * DESCRIPTION:     This function sets the msg id and payload for MWI
 *
 * RETURN:          ICF_SUCCESS always
 *
 ****************************************************************************/
icf_return_t icf_cfg_mwi_trigger(
        INOUT   icf_crm_pdb_st           *p_crm_pdb, 
        IN      icf_uint8_t               msg_id)
{   
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
  
    p_crm_pdb->p_internal_msg->msg_hdr.msg_id = msg_id;
    p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
    p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;
        
    /* Send message to MWI */
    ret_val = icf_mwi_process_mesg(p_crm_pdb->p_internal_msg);
    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}
#endif
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_check_init_complete
 *
 * DESCRIPTION:     This function checks whether all the mandatory parameters 
 *                  have been received once or not.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_check_init_complete(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_line_id_t                        line_id = 0;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* 
     * Check if mandatory parameters have been received for all lines.
     * Currently we are checking that we have atleast one user on
     * any one line
     */
    
    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

    for (line_id = 0; (line_id < p_crm_pdb->p_config_data->max_lines) && (line_id < ICF_MAX_NUM_OF_LINES) ; line_id++)
    {
        /* Get line data pointer from DBM */
        if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                                p_crm_pdb->p_glb_pdb,
                                line_id,
                                &(p_crm_pdb->p_line_data[line_id]),
                                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {
            if(ICF_NULL != 
                    (ICF_CFG_LINE_DATA_SELF_ADDR_RECD & 
                     p_crm_pdb->p_line_data[line_id]->line_data_received))
            {
                ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_TRUE)
                break;
            }
        }
    }/* End for */
                        
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_open_nw_interface
 *
 * DESCRIPTION:     This function triggers SSA to open network sockets.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case SSA fails to open network interface.
 *
 ******************************************************************************/
icf_return_t icf_cfg_open_nw_interface(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_transport_mode_et     self_mode)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_return_t                         ret_val_ssa = ICF_SUCCESS;
    icf_internal_msg_st                  open_nw_msg;
    icf_cfg_open_nw_server_st            *p_open_nw = ICF_NULL;
    icf_line_data_st                    *p_line_data = ICF_NULL;
    icf_line_data_st                    *p_temp_line = ICF_NULL;
    icf_line_id_t                          i = 0, j = 0;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Fill the internal message */
    open_nw_msg.msg_hdr.msg_id = ICF_CFG_OPEN_NW_SERVERS;
    open_nw_msg.msg_hdr.payload_length = sizeof(icf_cfg_open_nw_server_st);
    open_nw_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb;
    open_nw_msg.msg_hdr.ecode = ICF_ERROR_NONE;
    open_nw_msg.msg_hdr.status = ICF_STATUS_SUCCESS;

    /* Allocate memory for payload */
    ICF_MEMGET(
            p_crm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cfg_open_nw_server_st),
            ICF_MEM_COMMON,
            open_nw_msg.p_msg_data,
            ICF_RET_ON_FAILURE,
            &(open_nw_msg.msg_hdr.ecode),
            ret_val)

        
    if(ICF_SUCCESS == ret_val)
    {  
        p_open_nw = (icf_cfg_open_nw_server_st *)(open_nw_msg.p_msg_data);
      
        if (ICF_CFG_SELF_ADDR_PER_LINE == 
                p_crm_pdb->p_config_data->self_addr_per_line)
        {
            /* SOAK FIX : Fetch the line data based on line id only if
             * ICF_CFG_SELF_ADDR_PER_LINE is set in config data .p_line_data
             * is used under the check for ICF_CFG_SELF_ADDR_PER_LINE only
             * .Fetching the line data outside this check will prevent the
             * ICF_ALL_LINES feature to work .The case is one in which we try the
             * set_trans_params with line id value set to 65535 ie
             * ICF_ALL_LINES(a common scenario in load) .In that case the function
             * icf_dbm_fetch_line_cntxt_blk returns failure as it treat it as
             * as invalid line id .The will prevent the opening of network
             * interface .*/

            if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb,
                    p_crm_pdb->line_id,
                    &p_line_data,
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
            {
                ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    open_nw_msg.p_msg_data,
                    ICF_MEM_COMMON,
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                    ret_val)

                return ICF_FAILURE;
            }

        
             /*If same Server is already open then return*/
             for (i = 0; i < p_crm_pdb->p_config_data->max_lines; i++)
             {
                 if (i == p_crm_pdb->line_id)
                     continue;

                 if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb,
                    i,
                    &p_temp_line,
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
                 {
                     if (
                         ((p_line_data->self_addr.addr.addr.ipv4_addr.octet_1 == 
                          p_temp_line->self_addr.addr.addr.ipv4_addr.octet_1) &&
                         (p_line_data->self_addr.addr.addr.ipv4_addr.octet_2 == 
                          p_temp_line->self_addr.addr.addr.ipv4_addr.octet_2) &&
                         (p_line_data->self_addr.addr.addr.ipv4_addr.octet_3 == 
                          p_temp_line->self_addr.addr.addr.ipv4_addr.octet_3) &&
                         (p_line_data->self_addr.addr.addr.ipv4_addr.octet_4 == 
                          p_temp_line->self_addr.addr.addr.ipv4_addr.octet_4)) &&
                         (p_line_data->self_addr.port_num == p_temp_line->self_addr.port_num) &&
                         (ICF_TRUE == p_temp_line->line_data_init_complete))
                     {
                         /*If mode is same then return success else derive the mode*/
                         if (p_line_data->self_mode == p_temp_line->self_mode)
                         {
                              for (j = 0; j <= ICF_WIN_TLS_SIP_SERVER; j++)
                              {
                                  p_line_data->server_index[j] = 
                                     p_temp_line->server_index[j];
                              }
                              p_line_data->line_data_init_complete = ICF_TRUE; 
                              ICF_MEMFREE(
                                p_crm_pdb->p_glb_pdb,
                                open_nw_msg.p_msg_data,
                                ICF_MEM_COMMON,
                                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                                ret_val)
        
                              return ICF_SUCCESS;
                         }
                         else
                         {
                         }
                     }
                 }
             }
             p_open_nw->mode = p_line_data->self_mode;
             p_open_nw->line_id = p_crm_pdb->line_id;
	}
	else
	{
	     p_open_nw->mode = (icf_uint8_t)self_mode;
	}
   }

    /* Send the internal message */
    ret_val_ssa = icf_ssa_process_mesg(&open_nw_msg);
    
    /* Save the error code to be sent in response */
    
	p_crm_pdb->error_cause = open_nw_msg.msg_hdr.ecode;
    
    ICF_MEMFREE(
            p_crm_pdb->p_glb_pdb,
            open_nw_msg.p_msg_data,
            ICF_MEM_COMMON,
            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
            ret_val)
        
    /* This is to avoid the over-writing of the value of the ret_val returned by SSA 
     * and memfree as concern is only with the return of SSA.
     * */    
    ret_val = ret_val_ssa;   
    if ((ICF_CFG_SELF_ADDR_PER_LINE == 
            p_crm_pdb->p_config_data->self_addr_per_line) &&
        (ICF_SUCCESS == ret_val))
    {
             /*If same Server is already open then return*/
             for (i = 0; i < p_crm_pdb->p_config_data->max_lines; i++)
             {
                 if (i == p_crm_pdb->line_id)
                     continue;

                 if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb,
                    i,
                    &p_temp_line,
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
                 {
                     if (
                         ((p_line_data->self_addr.addr.addr.ipv4_addr.octet_1 == 
                          p_temp_line->self_addr.addr.addr.ipv4_addr.octet_1) &&
                         (p_line_data->self_addr.addr.addr.ipv4_addr.octet_2 == 
                          p_temp_line->self_addr.addr.addr.ipv4_addr.octet_2) &&
                         (p_line_data->self_addr.addr.addr.ipv4_addr.octet_3 == 
                          p_temp_line->self_addr.addr.addr.ipv4_addr.octet_3) &&
                         (p_line_data->self_addr.addr.addr.ipv4_addr.octet_4 == 
                          p_temp_line->self_addr.addr.addr.ipv4_addr.octet_4)) &&
                         (p_line_data->self_addr.port_num == p_temp_line->self_addr.port_num) &&
                         (p_line_data->self_mode == p_temp_line->self_mode))
                     {
                          p_temp_line->line_data_init_complete = ICF_TRUE;
                          for (j = 0; j <= ICF_WIN_TLS_SIP_SERVER; j++)
                          {
                               p_temp_line->server_index[j] = 
                                   p_line_data->server_index[j];
                          }
                     }
                 }
             }
    }    
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
        
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_close_nw_interface
 *
 * DESCRIPTION:     This function triggers SSA to close network sockets.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case SSA fails to close network interface.
 *
 ******************************************************************************/
icf_return_t icf_cfg_close_nw_interface(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_internal_msg_st                  int_msg = {{0,0,0,0,0,0},0};
    icf_cfg_close_nw_server_st           close_nw = {0};
    icf_line_id_t                          i = 0, j = 0, k = 0;
    icf_line_data_st                     *p_line_data = ICF_NULL;
    icf_line_data_st                     *p_temp_line = ICF_NULL;
     
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Fill the internal message */
    int_msg.msg_hdr.msg_id = ICF_CFG_CLOSE_NW_SERVERS;
    int_msg.msg_hdr.payload_length = 0;
    int_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb;
    int_msg.msg_hdr.ecode = ICF_ERROR_NONE;
    int_msg.msg_hdr.status = ICF_STATUS_SUCCESS;
    int_msg.p_msg_data = (icf_uint8_t *)&close_nw;

    /* Make the call ctx in the glb pdb to null */
    int_msg.msg_hdr.p_glb_pdb->p_call_ctx = ICF_NULL;

    if (ICF_CFG_SELF_ADDR_PER_LINE == 
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
        /*Check if server to be closed for all lines*/
        if (ICF_ALL_LINES == p_crm_pdb->line_id)
        {
            /*Iterate thru all lines and fetch line block*/
            for (i = 0; i < p_crm_pdb->p_config_data->max_lines; i++)
            {
                if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb,
                    i,
                    &p_line_data,
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
                {
                    continue;
                }
                /*Move to next line if server is not already opened*/
                if (ICF_FALSE == p_line_data->line_data_init_complete)
                {
                     continue;
                }
                /*Now check for lines which has same self IP, mode and port*/
                for (j = (icf_uint16_t)i+1; j < p_crm_pdb->p_config_data->max_lines; j++)
                {
                    if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                        p_crm_pdb->p_glb_pdb,
                        j,
                        &p_temp_line,
                        &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
                    {
                        if ((
                           (p_line_data->self_addr.addr.addr.ipv4_addr.octet_1 ==
                            p_temp_line->self_addr.addr.addr.ipv4_addr.octet_1) &&
                           (p_line_data->self_addr.addr.addr.ipv4_addr.octet_2 ==
                            p_temp_line->self_addr.addr.addr.ipv4_addr.octet_2) &&
                           (p_line_data->self_addr.addr.addr.ipv4_addr.octet_3 ==
                            p_temp_line->self_addr.addr.addr.ipv4_addr.octet_3) &&
                           (p_line_data->self_addr.addr.addr.ipv4_addr.octet_4 ==
                            p_temp_line->self_addr.addr.addr.ipv4_addr.octet_4)) &&
                           (p_line_data->self_addr.port_num == p_temp_line->self_addr.port_num) &&
                            (p_line_data->self_mode == p_temp_line->self_mode))
                         {
                                p_temp_line->line_data_init_complete = ICF_FALSE;
                                for (k = 0; k <= ICF_WIN_TLS_SIP_SERVER; k++)
                                {
                                    p_temp_line->server_index[k] = ICF_MAX_NUM_OF_SERVERS;
                                }
                         }
                    }
                }
                if (ICF_TRUE == p_line_data->line_data_init_complete)
                {
                    /*SPR 17433 index of line id should be sent*/ 
                    close_nw.line_id = i;
                    ret_val = icf_ssa_process_mesg(&int_msg);
                }
            }
        }
        else
        {
            close_nw.line_id = p_crm_pdb->line_id;

            if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                        p_crm_pdb->p_glb_pdb,
                        p_crm_pdb->line_id,
                        &p_line_data,
                        &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
            {
                return ICF_FAILURE;
            }
            /*If same Server is already open then return*/
            for (i = 0; i < p_crm_pdb->p_config_data->max_lines; i++)
            {
                if (i == p_crm_pdb->line_id)
                    continue;
            
                if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                            p_crm_pdb->p_glb_pdb,
                            i,
                            &p_temp_line,
                            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
                {
                    if (
                            ((p_line_data->self_addr.addr.addr.ipv4_addr.octet_1 ==
                              p_temp_line->self_addr.addr.addr.ipv4_addr.octet_1) &&
                             (p_line_data->self_addr.addr.addr.ipv4_addr.octet_2 ==
                              p_temp_line->self_addr.addr.addr.ipv4_addr.octet_2) &&
                             (p_line_data->self_addr.addr.addr.ipv4_addr.octet_3 ==
                              p_temp_line->self_addr.addr.addr.ipv4_addr.octet_3) &&
                             (p_line_data->self_addr.addr.addr.ipv4_addr.octet_4 ==
                              p_temp_line->self_addr.addr.addr.ipv4_addr.octet_4)) &&
                            (p_line_data->self_addr.port_num == p_temp_line->self_addr.port_num) &&
                            (p_line_data->self_mode == p_temp_line->self_mode))
                    {
                        p_temp_line->line_data_init_complete = ICF_TRUE;
                        for (j = 0; j <= ICF_WIN_TLS_SIP_SERVER; j++)
                        {
                            p_line_data->server_index[j] = ICF_MAX_NUM_OF_SERVERS;
                        }
                        return ICF_SUCCESS;
                    }
                }
            }
            /* Send the internal message */
            ret_val = icf_ssa_process_mesg(&int_msg);
        }
    }
    else
    {
        /* Send the internal message */
        ret_val = icf_ssa_process_mesg(&int_msg); 
    }
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	    return ret_val;

}/* End function */
    
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_call_clear_ind
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_internal_msg_st                  sic_msg;
    icf_sic_ctx_st                       *p_sic_ctxt = ICF_NULL;
	icf_sic_clear_ctx_st				 sic_payload;
    icf_uint16_t                         call_index = 1;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    icf_cfg_cm_forceful_call_clear(p_crm_pdb);

    /* SPR 9079: Fix Start */
    /* Clearing the SIC contexts */ 
    
    /* CSR 1-5856190: SPR 18425: Clear all the SIC contexts 
	* before closing the network sockets.
    */ 
	call_index =1;
	while (ICF_SUCCESS == icf_dbm_get_ong_sic_ctx(
                                p_crm_pdb->p_glb_pdb,
                                call_index,
                                &(p_sic_ctxt),
                                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
			/* Clearing the SIC contexts */
			/* to clear contexts for the required call id only */
			sic_msg.msg_hdr.msg_id = ICF_CLEAR_SIC_CTX;
			sic_msg.msg_hdr.ecode = 0; 
			sic_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb; 
			sic_payload.call_id = p_sic_ctxt->call_id;
			sic_payload.app_id = p_sic_ctxt->app_id;
			
			sic_msg.msg_hdr.payload_length = 
				ICF_PORT_SIZEOF(icf_sic_clear_ctx_st);
			sic_msg.p_msg_data = (icf_uint8_t*)(&(sic_payload));
			
			icf_sic_process_mesg(&sic_msg);  
		call_index ++;
	}
        
    /* SPR 9079: Fix End */


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

ICF_NW_RECOVERY_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_call_clear_ind_wrt_app
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls associated with particular application.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind_wrt_app(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_app_id_t				app_id)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_uint16_t                         call_index = 1;
	icf_sic_ctx_st						*p_sic_ctxt = ICF_NULL;
    icf_internal_msg_st                  sic_msg;    
	icf_sic_clear_ctx_st				    sic_payload;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
		
	/* Fetch call ctx from DBM until it returns failure */
	while (ICF_SUCCESS == icf_dbm_get_ong_call_ctx(
			p_crm_pdb->p_glb_pdb,
			call_index,
			&(p_crm_pdb->p_glb_pdb->p_call_ctx),
			&(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
	{
		if(p_crm_pdb->p_glb_pdb->p_call_ctx->app_id == app_id)
        {
            if(ICF_CALL_TYPE_EMERGENCY & 
                    p_crm_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
            {
                /* this is an emergency call and if the trigger is from REGM then do
                 * not cler the call and in case the trigger is from CFG then clear the
                 * call
                 */
                if(ICF_INT_MODULE_REGM == p_crm_pdb->cm_trigger_module)
                {
                    continue;
                } 
            }
            /* Send message to MMI for each active call */
            p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                ICF_FORCEFUL_CALL_CLEAR_IND;
            p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;			
            p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;

            ret_val = icf_mmi_process_mesg(p_crm_pdb->p_internal_msg);

            /* Send message to CM for each active call */
            p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                ICF_FORCEFUL_CALL_CLEAR_IND;
            p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
            p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;

            ret_val = icf_cm_process_mesg(p_crm_pdb->p_internal_msg);
        }
		    /* Increment number of active call counter */
		    call_index++;
		
	}/* End while */
	call_index =1;
	while (ICF_SUCCESS == icf_dbm_get_ong_sic_ctx(
                                p_crm_pdb->p_glb_pdb,
                                call_index,
                                &(p_sic_ctxt),
                                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
		if (app_id == p_sic_ctxt->app_id)
		{
			/* Clearing the SIC contexts */
			/* to clear contexts for the required call id only */
			sic_msg.msg_hdr.msg_id = ICF_CLEAR_SIC_CTX;
			sic_msg.msg_hdr.ecode = 0; 
			sic_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb; 
			sic_payload.call_id = p_sic_ctxt->call_id;
			sic_payload.app_id = app_id;
			/*sic_payload.event_package = 0;*/
			
			sic_msg.msg_hdr.payload_length = 
				ICF_PORT_SIZEOF(icf_sic_clear_ctx_st);
			sic_msg.p_msg_data = (icf_uint8_t*)(&(sic_payload));
			
			icf_sic_process_mesg(&sic_msg);  
		}
	   call_index ++;
	}    

	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;
}

/* Start: Multiple Public Users ICF5.0*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_call_clear_ind_wrt_user
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls associated with particular user.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind_wrt_user(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        INOUT		icf_address_st			*p_rgm_uri,
		IN		icf_line_id_t			line_id,
		IN		icf_boolean_t			line_specific,
		OUT		icf_error_t				*p_ecode)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_uint16_t                         call_index = 1;
	icf_sic_ctx_st						*p_sic_ctxt = ICF_NULL;
    icf_internal_msg_st                  sic_msg;    
	icf_sic_clear_ctx_st				    sic_payload;
	icf_user_db_node_st					*p_user_db = ICF_NULL;
	icf_address_list_node_st				*p_assoc_uri_list = ICF_NULL;
    icf_boolean_t                        is_to_send_forceful_call_clear = ICF_FALSE;
	
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

	ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(p_crm_pdb->p_glb_pdb,
	line_id, p_rgm_uri, ICF_USER_DB_USE_PUBLIC_URIS|ICF_USER_DB_USE_ASSOCIATED_URIS,
	&p_user_db,p_ecode);

	if(ICF_SUCCESS == ret_val)
	{
 		/* Fetch call ctx from DBM until it returns failure */
		while (ICF_SUCCESS == icf_dbm_get_ong_call_ctx(
				p_crm_pdb->p_glb_pdb,
				call_index,
				&(p_crm_pdb->p_glb_pdb->p_call_ctx),
				&(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
		{	
            is_to_send_forceful_call_clear = ICF_FALSE;
			p_assoc_uri_list = p_user_db->p_assoc_uri_list;
			
			/* Case where no Assoc uris were recieved in the response */
			if(ICF_NULL == p_assoc_uri_list)
			{
                if((ICF_FALSE == line_specific)   ||
                    ((ICF_TRUE == line_specific)   &&
                     (line_id == p_crm_pdb->p_glb_pdb->p_call_ctx->line_id)))
                {
                    /* SPR 19532:
                       To clear the calls wrt a user on a line,compare only the
                       username part and not the entire address.
                       
                       This is required to enable call clearing in the following 
                       scenario:

                       User1@a.com is added on line 1.
                       User2@b.com is added in line 2.

                       A back to back call is attempted with User1 calling
                       User2. The outgoing INVITE sent will have the Request URI
                       of the following form:
                       User2@selfIP:self:port

                       Since it is a back to back call, this will be updated 
                       in the call contexts' local_address (in the call context created for
                       the incoming call). If the entire addresses are compared, then the 
                       comparision would be between User2@b.com and User2@selfIP:self:port.
                       This comparision would fail and calls would not be cleared for line 2
                       and User 2. However the line data for line 2 will be freed.
                       On timer expiry, CC would try to access the same line data for
                       line 2 and would result in a reset.
                       
                       To avoid this, the comparision has been done only for the username
                       part. In such a scenario calls for line 2 and User 2 would be cleared.
                    */   
                    
                    if(icf_dbm_util_cmp_username(p_crm_pdb->p_glb_pdb,
                        &(p_crm_pdb->p_glb_pdb->p_call_ctx->local_address),
                        &(p_user_db->public_user)))
                    {
                        is_to_send_forceful_call_clear = ICF_TRUE;
                        
                    }
                    else if((ICF_NULL != p_crm_pdb->p_glb_pdb->p_call_ctx->p_preferred_identity) && 
                          (icf_dbm_util_cmp_username(p_crm_pdb->p_glb_pdb,
                                    p_crm_pdb->p_glb_pdb->p_call_ctx->p_preferred_identity,
                                    &(p_user_db->public_user))
                           )
                         )
                    {
                        is_to_send_forceful_call_clear = ICF_TRUE;
                    }
                }
               
                if(ICF_TRUE == is_to_send_forceful_call_clear)
                {
                    if(ICF_CALL_TYPE_EMERGENCY & 
                            p_crm_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
                    {
                        /* this is an emergency call and if the trigger is from REGM then do
                         * not cler the call and in case the trigger is from CFG then clear
                         * the call
                         */
                        if(ICF_INT_MODULE_REGM == p_crm_pdb->cm_trigger_module)
                        {
                            is_to_send_forceful_call_clear = ICF_FALSE;
                        } 
                    }
                }

                if(ICF_TRUE == is_to_send_forceful_call_clear)
                {
                    /* Send message to MMI for each active call */
                    p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                        ICF_FORCEFUL_CALL_CLEAR_IND;
                    p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;						
                    p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;

                    ret_val = icf_mmi_process_mesg(p_crm_pdb->p_internal_msg);

                    /* Send message to CM for each active call */
                    p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                        ICF_FORCEFUL_CALL_CLEAR_IND;
                    p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;						
                    p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;

                    ret_val = icf_cm_process_mesg(p_crm_pdb->p_internal_msg);
                }
			}
			/* Case where Assoc uris are present and we will have to search from them*/
			else
			{
				while(ICF_NULL != p_assoc_uri_list)
				{
                              /*  CSR 1-5710958 code is removed from here */
                    if((ICF_FALSE == line_specific)   ||
                        ((ICF_TRUE == line_specific)   &&
                        (line_id == p_crm_pdb->p_glb_pdb->p_call_ctx->line_id)))
                    {
                        if(icf_dbm_util_cmp_username(p_crm_pdb->p_glb_pdb,
                            &(p_crm_pdb->p_glb_pdb->p_call_ctx->local_address),
                            &(p_assoc_uri_list->user_addr)))
                        {
                            is_to_send_forceful_call_clear = ICF_TRUE;
                        }
                        else if((ICF_NULL != p_crm_pdb->p_glb_pdb->p_call_ctx->p_preferred_identity) && 
                              (icf_dbm_util_cmp_username(p_crm_pdb->p_glb_pdb,
                                    p_crm_pdb->p_glb_pdb->p_call_ctx->p_preferred_identity,
                                    &(p_assoc_uri_list->user_addr))))
                        {
                            is_to_send_forceful_call_clear = ICF_TRUE;
                        }
                        
                    }

                    if(ICF_TRUE == is_to_send_forceful_call_clear)
                    {
                       if(ICF_CALL_TYPE_EMERGENCY & 
                                p_crm_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
                        {
                            /* this is an emergency call and if the trigger is from REGM then do
                             * not cler the call and in case the trigger is from CFG then clear
                             * the call
                             */
                            if(ICF_INT_MODULE_REGM == p_crm_pdb->cm_trigger_module)
                            {
                                is_to_send_forceful_call_clear = ICF_FALSE;
                            } 
                        }
                    }

                    if(ICF_TRUE == is_to_send_forceful_call_clear)
                    {
                        /* Send message to MMI for each active call */
                        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                            ICF_FORCEFUL_CALL_CLEAR_IND;
                        p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
                        p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;

                        ret_val = icf_mmi_process_mesg(p_crm_pdb->p_internal_msg);

                        /* Send message to CM for each active call */
                        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                            ICF_FORCEFUL_CALL_CLEAR_IND;
                        p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
                        p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;

                        ret_val = icf_cm_process_mesg(p_crm_pdb->p_internal_msg);

                        /* Break out of the loop of assoc URIs as we are looking all the
                         * ongoing context*/
                        break;
                    }
						p_assoc_uri_list = (icf_address_list_node_st *)p_assoc_uri_list->p_next;
				}
			}
			/* Increment number of active call counter */
            
            /*If we have done the forceful call clear for some call context
             * then the value of call_index is a reset to one .This is used to
             * reinitialised our search from start and continue untill failure
             * is returned by function icf_dbm_get_ong_call_ctx. */
             
            if(ICF_TRUE == is_to_send_forceful_call_clear)
            {
                call_index = 1;
            }
            else 
            {    
			    call_index++;
            }    
			
		}/* End while */

		/* reinitiating for SIC contexts */
		call_index =1;

		while (ICF_SUCCESS == icf_dbm_get_ong_sic_ctx(
									p_crm_pdb->p_glb_pdb,
									call_index,
									&(p_sic_ctxt),
									&(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
		{

/*temp check till SIC context initiallisation is fixed also if the context is reg event package it wont be cleared*/
			if((ICF_INVALID_CALL_ID == p_sic_ctxt->call_id)||
				(0 == icf_port_strcmp((icf_uint8_t *)"reg",p_sic_ctxt->event_package.str)))
			{
				/* This SIC context is invalid and is returned only because not initialised properly*/
				call_index++;
				continue;
			}

			p_assoc_uri_list = p_user_db->p_assoc_uri_list;
			
			/* If there are no assoc URIs for the user then we compare with user only*/
			if(ICF_NULL == p_assoc_uri_list)
			{
				if((icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
						p_sic_ctxt->p_user_address,
						&(p_user_db->public_user)))&&
						((ICF_FALSE == line_specific)||
						((ICF_TRUE == line_specific)&&
						(line_id == p_sic_ctxt->line_id))))
				{
					/* Clearing the SIC contexts */
					/* to clear contexts for the required call id only */
					sic_msg.msg_hdr.msg_id = ICF_CLEAR_SIC_CTX;
					sic_msg.msg_hdr.ecode = 0; 
					sic_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb; 
					sic_payload.call_id = p_sic_ctxt->call_id;
					/*sic_payload.event_package = 0;*/
					
					sic_msg.msg_hdr.payload_length = 
						ICF_PORT_SIZEOF(icf_sic_clear_ctx_st);
					sic_msg.p_msg_data = (icf_uint8_t*)(&(sic_payload));
					
					icf_sic_process_mesg(&sic_msg); \
				}		
			
			}
			else
			{	/* Checking for all the assoc URIs if any one matches the user addr in 
				 * SIC context then we shall clear the context and move over to next context*/
				while(ICF_NULL != p_assoc_uri_list)
				{
					if((icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
						p_sic_ctxt->p_user_address,
						&(p_assoc_uri_list->user_addr)))&&
						((ICF_FALSE == line_specific)||
						((ICF_TRUE == line_specific)&&
						(line_id == p_sic_ctxt->line_id))))
					{
						/* Clearing the SIC contexts */
						/* to clear contexts for the required call id only */
						sic_msg.msg_hdr.msg_id = ICF_CLEAR_SIC_CTX;
						sic_msg.msg_hdr.ecode = 0; 
						sic_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb; 
						sic_payload.call_id = p_sic_ctxt->call_id;
						/*sic_payload.event_package = 0;*/
						
						sic_msg.msg_hdr.payload_length = 
							ICF_PORT_SIZEOF(icf_sic_clear_ctx_st);
						sic_msg.p_msg_data = (icf_uint8_t*)(&(sic_payload));
						
						icf_sic_process_mesg(&sic_msg);
              
						/* Out of the loop for all the assoc URIs */
						break;
					}
					p_assoc_uri_list = (icf_address_list_node_st *)p_assoc_uri_list->p_next;
				}
			}
			     call_index ++;
		}
	}

	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_call_clear_ind_wrt_app_user
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls associated with set of particular user and particular
 *					application.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind_wrt_app_user(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        INOUT		icf_address_st			*p_rgm_uri,
		INOUT		icf_app_id_t				app_id,
		OUT		icf_error_t				*p_ecode)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_uint16_t                         call_index = 1;
	icf_sic_ctx_st						*p_sic_ctxt = ICF_NULL;
    icf_internal_msg_st                  sic_msg;    
	icf_sic_clear_ctx_st				    sic_payload;
	icf_user_db_node_st					*p_user_db = ICF_NULL;
	icf_address_list_node_st				*p_assoc_uri_list = ICF_NULL;
    icf_boolean_t                        is_to_send_forceful_call_clear = ICF_FALSE;

    /* To Remove warnings */
    app_id=app_id;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

	ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(p_crm_pdb->p_glb_pdb,
	(icf_line_id_t )p_crm_pdb->line_id, p_rgm_uri,ICF_USER_DB_USE_PUBLIC_URIS|ICF_USER_DB_USE_ASSOCIATED_URIS,
	&p_user_db,p_ecode);

	
	if(ICF_SUCCESS == ret_val)
	{
 		/* Fetch call ctx from DBM until it returns failure */
		while (ICF_SUCCESS == icf_dbm_get_ong_call_ctx(
				p_crm_pdb->p_glb_pdb,
				call_index,
				&(p_crm_pdb->p_glb_pdb->p_call_ctx),
				&(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
		{
                        /*  CSR 1-5710958 Merge */
                        is_to_send_forceful_call_clear = ICF_FALSE;
			p_assoc_uri_list = p_user_db->p_assoc_uri_list;
			/* Case where no Assoc uris were recieved in the response */
			if(ICF_NULL == p_assoc_uri_list)
			{
				if((icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
						&(p_crm_pdb->p_glb_pdb->p_call_ctx->local_address),
						&(p_user_db->public_user))))
                {
                    is_to_send_forceful_call_clear = ICF_TRUE;
                    
                }
                else if((ICF_NULL != p_crm_pdb->p_glb_pdb->p_call_ctx->p_preferred_identity) && 
                        (icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
                                               p_crm_pdb->p_glb_pdb->p_call_ctx->p_preferred_identity,
                                               &(p_user_db->public_user))
                        )
                       )
                {
                    is_to_send_forceful_call_clear = ICF_TRUE;
                }

                if(ICF_TRUE == is_to_send_forceful_call_clear)
                {
                    if(ICF_CALL_TYPE_EMERGENCY & 
                            p_crm_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
                    {
                        /* this is an emergency call and if the trigger is from REGM then do
                         * not cler the call and in case the trigger is from CFG then clear
                         * the call
                         */
                        if(ICF_INT_MODULE_REGM == p_crm_pdb->cm_trigger_module)
                        {
                            is_to_send_forceful_call_clear = ICF_FALSE;
                        } 
                    }
                }

                if(ICF_TRUE == is_to_send_forceful_call_clear)
                {
                    /* Send message to MMI for each active call */
                    p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                        ICF_FORCEFUL_CALL_CLEAR_IND;
                    p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
                    p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                    
                    ret_val = icf_mmi_process_mesg(p_crm_pdb->p_internal_msg);
                    
                    /* Send message to CM for each active call */
                    p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                        ICF_FORCEFUL_CALL_CLEAR_IND;
                    p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
                    p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                    
                    ret_val = icf_cm_process_mesg(p_crm_pdb->p_internal_msg);
                }
			}
			/* Case where Assoc uris are present and we will have to search from them*/
			else
			{
				while(ICF_NULL != p_assoc_uri_list)
                {
                    is_to_send_forceful_call_clear = ICF_FALSE;
                    if((icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
                        &(p_crm_pdb->p_glb_pdb->p_call_ctx->local_address),
                        &(p_assoc_uri_list->user_addr))))
                    {
                        is_to_send_forceful_call_clear = ICF_TRUE;
                    }
                    /*SPR 20047 The call was not getting cleared for the
                     * public URI used in registration if the same is absent in 
                     * P-Associated-URI list when application initiated
                     * deregistration is triggered using the same public URI
                     * for which registration was done.*/
                    else if((icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
						&(p_crm_pdb->p_glb_pdb->p_call_ctx->local_address),
						&(p_user_db->public_user))))
                    {
                        is_to_send_forceful_call_clear = ICF_TRUE;
                    }
                    else if((ICF_NULL != p_crm_pdb->p_glb_pdb->p_call_ctx->p_preferred_identity) && 
                        (icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
                        p_crm_pdb->p_glb_pdb->p_call_ctx->p_preferred_identity,
                        &(p_assoc_uri_list->user_addr))))
                    {
                        is_to_send_forceful_call_clear = ICF_TRUE;
                    }
                   
                    if(ICF_TRUE == is_to_send_forceful_call_clear)
                    {
                       if(ICF_CALL_TYPE_EMERGENCY & 
                                p_crm_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
                        {
                            /* this is an emergency call and if the trigger is from REGM then do
                             * not cler the call and in case the trigger is from CFG then clear
                             * the call
                             */
                            if(ICF_INT_MODULE_REGM == p_crm_pdb->cm_trigger_module)
                            {
                                is_to_send_forceful_call_clear = ICF_FALSE;
                            } 
                        }
                    }

                    if(ICF_TRUE == is_to_send_forceful_call_clear)
                    {	
                        /* Send message to MMI for each active call */
                        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                            ICF_FORCEFUL_CALL_CLEAR_IND;						
                        p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
                        p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                        
                        ret_val = icf_mmi_process_mesg(p_crm_pdb->p_internal_msg);
                        
                        /* Send message to CM for each active call */
                        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                            ICF_FORCEFUL_CALL_CLEAR_IND;						
                        p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
                        p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                        
                        ret_val = icf_cm_process_mesg(p_crm_pdb->p_internal_msg);
                        break;
                    }
                    p_assoc_uri_list = (icf_address_list_node_st *)p_assoc_uri_list->p_next;
                }
			}
			/* Increment number of active call counter */
			call_index++;
			
		}/* End while */

		/* reinitiating for SIC contexts */
		call_index =1;

		while (ICF_SUCCESS == icf_dbm_get_ong_sic_ctx(
									p_crm_pdb->p_glb_pdb,
									call_index,
									&(p_sic_ctxt),
									&(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
		{
                    /*  CSR 1-5710958 Merge */
                    is_to_send_forceful_call_clear = ICF_FALSE;
             /* temp check till SIC context initiallisation is fixed
              * We will not UNSUBSCRIBE or clear the sic context in the following two conditions:
              * 1.Call ID is invalid.
              * 2.Event package is reg
              */

			if((ICF_INVALID_CALL_ID == p_sic_ctxt->call_id) ||
	    	   (0 == icf_port_strcmp(p_sic_ctxt->event_package.str ,(icf_uint8_t *)"reg")))            	  {
	   			/* This SIC context is invalid and is returned only because not initialised properly*/
				call_index++;
				continue;
			}
			
			p_assoc_uri_list = p_user_db->p_assoc_uri_list;
			
			/* If there are no assoc URIs for the user then we compare with user only*/
			if(ICF_NULL == p_assoc_uri_list)
			{
				if((icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
						p_sic_ctxt->p_user_address,
						&(p_user_db->public_user))))
				{
					/* Clearing the SIC contexts */
					/* to clear contexts for the required call id only */
					sic_msg.msg_hdr.msg_id = ICF_CLEAR_SIC_CTX;
					sic_msg.msg_hdr.ecode = 0; 
					sic_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb; 
					sic_payload.call_id = p_sic_ctxt->call_id;
					/*sic_payload.event_package = 0;*/
					
					sic_msg.msg_hdr.payload_length = 
						ICF_PORT_SIZEOF(icf_sic_clear_ctx_st);
					sic_msg.p_msg_data = (icf_uint8_t*)(&(sic_payload));
					
					icf_sic_process_mesg(&sic_msg);
				}		
			
			}
			else
			{	/* Checking for all the assoc URIs if any one matches the user addr in 
				 * SIC context then we shall clear the context and move over to next context*/
				
				while(ICF_NULL != p_assoc_uri_list)
				{
					if((icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
						p_sic_ctxt->p_user_address,
						&(p_assoc_uri_list->user_addr))))
					{
						/* Clearing the SIC contexts */
						/* to clear contexts for the required call id only */
						sic_msg.msg_hdr.msg_id = ICF_CLEAR_SIC_CTX;
						sic_msg.msg_hdr.ecode = 0; 
						sic_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb; 
						sic_payload.call_id = p_sic_ctxt->call_id;
						/*sic_payload.event_package = 0;*/
						
						sic_msg.msg_hdr.payload_length = 
							ICF_PORT_SIZEOF(icf_sic_clear_ctx_st);
						sic_msg.p_msg_data = (icf_uint8_t*)(&(sic_payload));
						
						icf_sic_process_mesg(&sic_msg);
						/* Out of the loop for all the assoc URIs */
						break;
					}
					p_assoc_uri_list = (icf_address_list_node_st *)p_assoc_uri_list->p_next;
				}
			}
			   call_index ++;
		}
	}

	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
	return ret_val;
}
/* End: Multiple Public Users ICF5.0*/


ICF_NW_RECOVERY_END

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_check_line_in_db
 *
 * DESCRIPTION:     It either calls DBM function to fetch(return pointer 
 *                  if line exists) the line pointer or calls the dbm function 
 *                  to get the pointer (return the pointer to the line and
 *                  allocate memory to it if the line does not exists). 
 *                  It also fills in the response to be send in the pdb in 
 *                  case of invalid line id
 * 
 *****************************************************************************/
icf_return_t icf_cfg_check_line_in_db(
        icf_crm_pdb_st             *p_crm_pdb,
        icf_line_id_t              line_id,
        icf_uint8_t                *p_line_status,
        icf_boolean_t              get_line)
{
    icf_return_t                   ret_val=ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    if(ICF_FALSE == get_line)
    {        
        /* If line is not to be explicitly configured, fetch line pointers */
        if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb,
                    line_id,
                    &(p_crm_pdb->p_line_data[line_id]),
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {    
            /* Here line id is also to be validated. If the API has 
             * line id more than that in the max_lines then DBM
             * will return failure will proper ecode.
             */
            if (ICF_STATUS_INVALID_LINE_ID ==
                    p_crm_pdb->p_internal_msg->msg_hdr.ecode)
            {
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
            }

            ret_val = ICF_FAILURE;
        }
    }        
    else 
    {
        /* Line has to be configured if it is not already conf */
        if(ICF_FAILURE == icf_dbm_get_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb,
                    line_id,
                    &(p_crm_pdb->p_line_data[line_id]),
                    (p_line_status + line_id),
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {
            if (ICF_STATUS_INVALID_LINE_ID ==
                    p_crm_pdb->p_internal_msg->msg_hdr.ecode)
            {
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INVALID_LINE_ID)
                ret_val = ICF_FAILURE;
                ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            }                                    

            ret_val = ICF_FAILURE;
        }/* End ICF_FAILURE == .. */

        else
        { 
            icf_cfg_init_line_blk(p_crm_pdb, 
                    *(p_line_status + line_id),line_id);
        }
    }/* End else */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End Function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_check_line_configured
 *
 * DESCRIPTION:     It checks whether any one line has been configured.
 *                  If yes then takes the pointers of the lines configured 
 *                  till now else allocates memeory to all the lines and 
 *                  takes the pointers to all the lines in pdb.
 *         
 *****************************************************************************/
icf_return_t icf_cfg_check_line_configured(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_boolean_t    configure_lines)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_line_id_t        line_index = 0;
    icf_boolean_t        line_configured = ICF_FALSE;
    icf_uint8_t          line_status[ICF_MAX_NUM_OF_LINES];
    icf_uint8_t          dealloc_count = 0;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    for (line_index = 0; line_index < ICF_MAX_NUM_OF_LINES; line_index++)
    {
        line_status[line_index] = ICF_STATUS_LINE_OLD;    
        p_crm_pdb->lines_present[line_index] = ICF_FALSE;    
    }      
        
    /* Loop to max_lines to get the pointers to the lines configured till
     * now 
     */
        
    for (line_index = 0; (line_index < (p_crm_pdb->p_config_data->max_lines)) &&
						(line_index < ICF_MAX_NUM_OF_LINES) ;
            line_index++)
    {
        if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                   p_crm_pdb->p_glb_pdb, 
                   line_index, 
                   &(p_crm_pdb->p_line_data[line_index]), 
                   &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {
            /* Set the boolean flag to indicate that atleast one line is 
             * configured till now
             */
            line_configured = ICF_TRUE;            
            
            /* Set the status of the line to which pointer is taken in the
             * pdb to true 
             */
            p_crm_pdb->lines_present[line_index] = ICF_TRUE;
        }       

    }        
    
    /* if no line is configured till now then all the line upto max_lines will
     * be configured now 
     */
    
    if((ICF_FALSE == line_configured) && (ICF_TRUE == configure_lines))
    {
        for (line_index = 0; (line_index < (p_crm_pdb->p_config_data->
                max_lines)) && (line_index < ICF_MAX_NUM_OF_LINES); line_index++)
        {
            /* Call dbm function to allocate memory to all the lines
             */
            
            ret_val = icf_dbm_get_line_cntxt_blk(
                p_crm_pdb->p_glb_pdb,
                line_index,
                &(p_crm_pdb->p_line_data[line_index]),
                &(line_status[line_index]),
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));
            
            /* If dbm returns failure then the memory alocated to all the 
             * lines till now has to be de-allocated. In this case all the 
             * lines configured are new so there is no need to chcek the
             * status to de-allocate the lines
             */
            
            if(ICF_FAILURE == ret_val)
            {
                for (dealloc_count = 0; dealloc_count < line_index; 
                        dealloc_count++)
                {
                    if(ICF_STATUS_LINE_NEW == line_status[dealloc_count])
                    {
                        ret_val = icf_dbm_dealloc_line_blk(
                            p_crm_pdb->p_glb_pdb,
                            dealloc_count,
                            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));

                        ICF_CFG_TRACE(p_crm_pdb, 
                                ICF_CFG_TRACE_LINE_BLK_DEALLOCATED)
                    }
                }     
                
                ret_val = ICF_FAILURE;
                ICF_CFG_SET_RESPONSE(
                        p_crm_pdb,
                        ICF_FAILURE,
                        ICF_CAUSE_INSUFFICIENT_RESOURCES)
                break;
            }                
            
            else
            {
                icf_cfg_init_line_blk(p_crm_pdb, line_status[line_index],
                        line_index);

                p_crm_pdb->lines_present[line_index] = ICF_TRUE;
            }                
        }/* End loop */
    }/* End if */        
	else if(ICF_FALSE == line_configured)
	{
		ret_val = ICF_FAILURE;
	}
        
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;        
}    
         
/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_ssa_initiate_cleanup
 *
 * DESCRIPTION:     This function triggers SSA to initiate clean up of all its
 *                  context including timers.
 *         
 *****************************************************************************/
icf_return_t icf_cfg_ssa_initiate_cleanup(
        INOUT   icf_crm_pdb_st   *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_internal_msg_st                  clean_nw_msg;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Fill the internal message */
    clean_nw_msg.msg_hdr.msg_id = ICF_CFG_SSA_INITIATE_CLEAN_UP;
    clean_nw_msg.msg_hdr.payload_length = 0;
    clean_nw_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb;
    clean_nw_msg.msg_hdr.ecode = ICF_ERROR_NONE;
    clean_nw_msg.msg_hdr.status = ICF_STATUS_SUCCESS;
    clean_nw_msg.p_msg_data = ICF_NULL;

    /* Make the call ctx in the glb pdb to null */
    clean_nw_msg.msg_hdr.p_glb_pdb->p_call_ctx = ICF_NULL;
    /* Send the internal message */
    ret_val = icf_ssa_process_mesg(&clean_nw_msg);
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
        
}/* End function */


#if defined(IMS_CLIENT) || defined(ICF_NW_ACTIVATE)
/****************************************************************************
 * FUNCTION:        icf_cfg_nm_get_acs_info
 *
 * DESCRIPTION:     This function requests access information from NM 
 *
 * RETURNS:         ICF_TRUE if able to fetch network info
 *                  ICF_FALSE if unable to fetch network info        
 *
 ***************************************************************************/
icf_return_t icf_cfg_nm_get_acs_info(
		INOUT   icf_crm_pdb_st *p_crm_pdb,
		INOUT   icf_internal_msg_st *p_internal_msg)
{
	icf_return_t        ret_val = ICF_SUCCESS;
	icf_nm_global_st    *p_nm_data = ICF_NULL;



	ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

		/*----------------------------------------------------------------------*/


		/* Get NM data from DBM */
		if(ICF_SUCCESS == icf_dbm_get_module_glb_data(
					p_crm_pdb->p_glb_pdb,       
					(icf_module_id_t)ICF_INT_MODULE_NM,
					(icf_void_t*)(&p_nm_data),
					&(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
		{
			p_internal_msg->msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb;
			p_internal_msg->msg_hdr.msg_id = ICF_NM_ACS_INFO;

			((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_nm_ptr = p_nm_data;

#ifdef ICF_CFG_UNIT_TESTING
        ICF_MEMGET(p_crm_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(icf_nm_access_info_st),
                ICF_MEM_COMMON,
                (icf_void_t *)((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo,
                ICF_RET_ON_FAILURE,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                ret_val)

        ICF_MEMGET(p_crm_pdb->p_glb_pdb,
                10*ICF_PORT_SIZEOF(icf_string_st),
                ICF_MEM_COMMON,
                (icf_void_t *)((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->p_public_usr_id,
                ICF_RET_ON_FAILURE,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                ret_val)

        ICF_MEMGET(p_crm_pdb->p_glb_pdb,
                10*ICF_PORT_SIZEOF(icf_string_st),
                ICF_MEM_COMMON,
                (icf_void_t *)((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->p_apn_list,
                ICF_RET_ON_FAILURE,
                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode),
                ret_val)

        icf_port_strcpy(((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->home_nw_dmn.str,"flextronics.home.net"); 
        ((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->home_nw_dmn.str_len = icf_port_strlen("flextronics.home.net");
    
        ((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->nofpbuids = 2;

        icf_port_strcpy(((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->p_public_usr_id[0].str,"sip:PRIYA"); 
        icf_port_strcpy(((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->p_public_usr_id[1].str,"tel:PRIYANKA"); 

        ((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->p_public_usr_id[0].str_len = 9;

        ((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->p_public_usr_id[1].str_len = 12;

        ((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->nofapns = 2;

        icf_port_strcpy(((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->p_apn_list[0].str,"FSS_APN_1"); 
        icf_port_strcpy(((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->p_apn_list[1].str,"APN"); 

        ((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->p_apn_list[0].str_len = 9;

        ((icf_nm_acs_info_st *)(p_internal_msg->p_msg_data))->p_ainfo->p_apn_list[1].str_len = 3;
#endif

        ret_val = icf_nm_process_mesg(p_internal_msg);
    }

	ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return (ret_val);
}/* End function */
#endif /*ICF_NW_ACTIVATE || IMS_CLIENT*/


/****************************************************************************
 * FUNCTION:        icf_cfg_get_num_apps
 *
 * DESCRIPTION:     This function determines the number of valid entries in 
 *                  application context block
 *
 ***************************************************************************/
icf_return_t icf_cfg_get_num_apps(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        OUT     icf_uint32_t *num_apps)
{
     icf_return_t        ret_val = ICF_SUCCESS;
     icf_uint8_t         app_index = 0;
     icf_dbm_data_st     *p_dbm_data = ICF_NULL;
     icf_app_conf_st     *p_app_conf = ICF_NULL;


     ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

     if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
                                p_crm_pdb->p_glb_pdb,
                                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                                (icf_void_t *)&p_dbm_data,
                                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
     {
        p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

	     /* count the number of applications */
         *num_apps = 0;
         for(app_index = 0; app_index < p_crm_pdb->p_config_data->max_applications; app_index++)
         {
            if (p_app_conf[app_index].is_valid == ICF_TRUE)
            {
               *num_apps = *num_apps + 1;
            }
         }
     }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return (ret_val);
}/* End function */

/****************************************************************************
 * FUNCTION:        icf_cfg_get_vacant_app_index
 *
 * DESCRIPTION:     This function determines vacant application context index
 *
 ***************************************************************************/
icf_return_t icf_cfg_get_vacant_app_index(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        OUT     icf_uint8_t *app_index)
{
     icf_return_t        ret_val = ICF_SUCCESS;
     icf_uint8_t         applicatn_index = 0;
     icf_dbm_data_st     *p_dbm_data = ICF_NULL;
     icf_app_conf_st     *p_app_conf = ICF_NULL;
     

     ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

     if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
                                p_crm_pdb->p_glb_pdb,
                                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                                (icf_void_t *)&p_dbm_data,
                                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
     {
        p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

	     /* determine a vacant application context */
         for(applicatn_index = 0; applicatn_index < p_crm_pdb->p_config_data->max_applications; applicatn_index++)
         {
            if (p_app_conf[applicatn_index].is_valid == ICF_FALSE)
            {
               *app_index = applicatn_index;
               break;
            }
         }
     }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return (ret_val);
}/* End function */

/****************************************************************************
 * FUNCTION:        icf_cfg_app_already_present
 *
 * DESCRIPTION:     This function determines if application context is alraedy 
 *                  present
 *
 ***************************************************************************/
/* Fix for SPR: 19886 (Merged CSR-1-7423906)*/
/* Fix for CSR 1-7423906: SPR 19772 */
icf_return_t icf_cfg_app_already_present(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_string_st app_name,
#ifdef ICF_PORT_DEBIAN
        INOUT     icf_uint8_t *app_index)
#else
        INOUT     icf_uint16_t *app_index)
#endif        
{
     icf_return_t        ret_val = ICF_FAILURE;
     icf_uint8_t          applicatn_index = 0;
     icf_dbm_data_st     *p_dbm_data = ICF_NULL;
     icf_app_conf_st     *p_app_conf = ICF_NULL;
     

     ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
     /* Fix for SPR: 19886 (Merged CSR-1-7423906)*/     
     /* Fix for CSR 1-7423906: SPR 19772 */
#ifdef ICF_PORT_DEBIAN
     *app_index = (icf_uint8_t)-1;
#else
     *app_index = (icf_uint16_t)-1;
#endif     

     if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
                                p_crm_pdb->p_glb_pdb,
                                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                                (icf_void_t *)&p_dbm_data,
                                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
     {
        p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

	     /* determine a vacant application context */
         for(applicatn_index = 0; applicatn_index < p_crm_pdb->p_config_data->max_applications; applicatn_index++)
         {
            if (p_app_conf[applicatn_index].is_valid == ICF_TRUE)
            {
               if(icf_port_strcmp(p_app_conf[applicatn_index].app_name.str,app_name.str) == 0)
               {
                   *app_index = applicatn_index;
                   ret_val = ICF_SUCCESS; 
                   break;
               }
            }
         }
     }


     ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
     return ret_val ; 
}/* End function */

/****************************************************************************
 * FUNCTION:        icf_cfg_fetch_app_conf
 *
 * DESCRIPTION:     This function fetches icf_app_conf_st for a given app_id
 *
 ***************************************************************************/
icf_return_t icf_cfg_fetch_app_conf(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN 	  icf_app_id_t app_id, 
        OUT   icf_app_conf_st **p_p_app_conf,
        OUT     icf_error_t *p_ecode)
{
     icf_return_t        ret_val = ICF_SUCCESS;
     icf_uint8_t         applicatn_index = 0;
     icf_dbm_data_st     *p_dbm_data = ICF_NULL;
     icf_config_data_st  *p_config_data = ICF_NULL;
     icf_app_conf_st     *p_app_conf = ICF_NULL;

 
     ICF_FUNCTION_ENTER(p_glb_pdb)
	 ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_p_app_conf))
	 ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_ecode))

     if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
                                    p_glb_pdb,
                                    (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                                    (icf_void_t *)&p_dbm_data,
                                    p_ecode))
     {
        p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

         if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
                                        p_glb_pdb,
                                        (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                                        (icf_void_t *)&p_config_data,
                                        p_ecode))
         {
	         /* determine the application context entry */
             for(applicatn_index = 0; applicatn_index < p_config_data->max_applications; applicatn_index++)
             {
                if (p_app_conf[applicatn_index].is_valid == ICF_TRUE)
                {
                    if (p_app_conf[applicatn_index].app_id == app_id)
                    {
                        *p_p_app_conf = &(p_app_conf[applicatn_index]);
                        return (ret_val);
                    }
                }
             }
         }
     }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ICF_FAILURE;
}/* End function */

/****************************************************************************
 * FUNCTION:        icf_cfg_fetch_app_index
 *
 * DESCRIPTION:     This function fetches app_index for a given app_id
 *                  in Application Context
 *
 ***************************************************************************/
icf_return_t icf_cfg_fetch_app_index(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        IN    icf_app_id_t app_id,
        OUT	  icf_uint8_t *app_index)
{
     icf_return_t        ret_val = ICF_SUCCESS;
     icf_uint8_t         applicatn_index = 0;
     icf_dbm_data_st     *p_dbm_data = ICF_NULL;
     icf_app_conf_st     *p_app_conf = ICF_NULL;

     
     ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

     if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
                                    p_crm_pdb->p_glb_pdb,
                                    (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                                    (icf_void_t *)&p_dbm_data,
                                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
     {
        p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

   	     /* determine the application context entry */
         for(applicatn_index = 0; applicatn_index < p_crm_pdb->p_config_data->max_applications; applicatn_index++)
         {
            if (p_app_conf[applicatn_index].is_valid == ICF_TRUE)
            {
                if (p_app_conf[applicatn_index].app_id == app_id)
                {
                    *app_index = applicatn_index;
                    break;
                }
            }
         }
     }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return (ret_val);
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_app_add
 * 
 * DESCRIPTION:     This function validates the params received in application 
 *                  add API.
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_cfg_validate_app_add(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_app_add_req_st   *p_payload = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_crm_pdb->p_recv_payload))
  
    p_payload = (icf_app_add_req_st *)p_crm_pdb->p_recv_payload;

#ifdef ICF_REL_OLD
    /* Validate call type */
    if((ICF_CALL_TYPE_INVALID != p_payload->call_type)
        && (ICF_CALL_TYPE_AUDIO != p_payload->call_type)
        && (ICF_CALL_TYPE_VIDEO != p_payload->call_type)
        && (ICF_CALL_TYPE_AUDIO_TBCP != p_payload->call_type)
        && (ICF_CALL_TYPE_VIDEO_TBCP != p_payload->call_type)
        && (ICF_CALL_TYPE_AUDIO_VIDEO != p_payload->call_type))
    {
        p_crm_pdb->error_cause = ICF_CAUSE_INVALID_CALL_TYPE;
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }
#endif
    
   /* Validate the list of supported methods - Fix SPR 10087 */
    if (ICF_NULL == p_payload->supported_methods.count)
    {
        p_crm_pdb->error_cause = ICF_CAUSE_EMPTY_SUPP_HDR_LIST;
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_request_header
 * 
 * DESCRIPTION:     This function validates the header for various request APIs 
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *****************************************************************************/
icf_return_t icf_cfg_validate_request_header(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_crm_pdb->p_glb_pdb->p_recv_msg->hdr))
  
    /* Validate version */
    if(ICF_VERSION_1_0 != p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.version)
    {
        p_crm_pdb->error_cause = ICF_CAUSE_INVALID_VERSION_ID;
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    /* Validate destination id */
    if(ICF_MODULE_ICF != p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.destination_id)
    {
        p_crm_pdb->error_cause = ICF_CAUSE_INVALID_DESTINATION_ID;
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; /* returns success always */
}/* End function */

ICF_CRM_END

/*****************************************************************************
 * FUNCTION:        icf_cfg_parse_filter_expression
 *
 * DESCRIPTION:     This function parses the filter expression strings given  
 *                  in the application filter structure by an application in 
 *                  APP_ADD_REQ. The expression string is stored in the filter
 *                  expression structure if the parsing is successful.
 *					The function returns ICF_SUCCESS if the expression 
 *                  string is successfully parsed and stored in the filter
 *                  expression structure passed.
 *
 * Note :           The Syntax of expression string is 
 *                  str (| str)* || str (& str)*
 *                  where 
 *                  | stands for the boolean operator OR
 *                  & stands for the boolean operator AND
 *                  str = "(any_character)*"  
 *                          i.e. str is any character string 
 *                          enclosed in double quotes(")
 *                  For eg. +g.poc.talkburst is represented as 
 *                               "+g.poc.talkburst"
 *                  For representing the double quote character(") in a 
 *                  string, application will write it as "" 
 *                  For eg. "+g.poc.talkburst" is repesented as 
 *                   """+g.poc.talkburst"""
 *                  The expression can only contain either of the & or |. 
 *                  Mixed expressions are currently not supported.
 *
 ******************************************************************************/
icf_return_t 
icf_cfg_parse_filter_expression(INOUT icf_crm_pdb_st *p_crm_pdb,
								INOUT	icf_uint8_t		   *p_expr_str,
								OUT	icf_filter_expr_st  *p_expr,
								OUT	icf_error_t		   *p_ecode)
{
	icf_uint8_t temp_str[ICF_MAX_STR_LEN],*p_ch = ICF_NULL;
	icf_uint8_t state= ICF_PARSE_STATE_LOOKING_FOR_STRING;
	icf_uint8_t i=ICF_NULL,opr = ICF_NULL;
	icf_boolean_t operator_found = ICF_FALSE;
	icf_return_t ret_val=ICF_SUCCESS;
	icf_string_node_st **p_p_last_string_node=ICF_NULL;

    p_p_last_string_node=&(p_expr->p_match_str_list);

    
    /* state can have one of the following values 
	 {IN_STRING, LOOKING_FOR_STRING, LOOKING_FOR_OPERATOR,PARSE_FAILED} */

	
	p_expr->count = 0;
    p_expr->match_all=ICF_FALSE;

    /* Initially the parsing state is ICF_PARSE_STATE_LOOKING_FOR_STRING */
    for(p_ch = p_expr_str; 
	((*p_ch !='\0') && (state !=ICF_PARSE_STATE_PARSE_FAILED)
		&& (ICF_SUCCESS == ret_val));p_ch++)
	{
		switch(state)
		{
		case ICF_PARSE_STATE_LOOKING_FOR_STRING : 
            /* In this state we will look for a double quote 
               denoting the start of a string, when a double quote is found
               the state will be changed to ICF_PARSE_STATE_IN_STRING */
			{
				/* ignore spaces in expression  */
				while(icf_port_isspace(*p_ch)) 
					p_ch++;
				if(*p_ch == '"') /* start of string */
					state = ICF_PARSE_STATE_IN_STRING;
				else 
                    /* If character other than " is found */
					state = ICF_PARSE_STATE_PARSE_FAILED;/* failure */
				break;
			}
		case ICF_PARSE_STATE_IN_STRING:
            /* traverse and store the string till an ending double 
               quote is found. After the string ends, we will look for an
               operator */
			{
				for(i=0;(state == ICF_PARSE_STATE_IN_STRING) && (i<ICF_MAX_STR_LEN);i++,p_ch++)
				{
					if(*p_ch == '\0') 
						/* end of expression in string */
					{
						state = ICF_PARSE_STATE_PARSE_FAILED;
					}
					else   
					{
						/* string end or two quotes in continuation */
						if (*p_ch == '"')
						{
							/* move to next character */
							p_ch++; 
							
							if(*p_ch !='"')
							{
								/* end of current string */ 
								p_ch--; /* push back unprocessed character */
                                temp_str[i] = '\0';
								if(0 == icf_port_strcmp(temp_str,(icf_uint8_t *)""))
									state = ICF_PARSE_STATE_PARSE_FAILED;
								else
									state = ICF_PARSE_STATE_LOOKING_FOR_OPERATOR;
							}
							else
								temp_str[i]=*p_ch;
						}
						else
							temp_str[i] = *p_ch;
						
					}
				}
                p_ch--; /* push back unprocessed character */
				if(state !=ICF_PARSE_STATE_PARSE_FAILED)
				{
					/* make a new node in the string list*/
					ICF_MEMGET(p_crm_pdb->p_glb_pdb,
						ICF_PORT_SIZEOF(icf_string_node_st),
		                ICF_MEM_COMMON,
						(*p_p_last_string_node),
						ICF_DONOT_RET_ON_FAILURE,
						p_ecode,
						ret_val);
                    /*klocwork warning removal*/
                    ICF_KLOCWORK_FALSE_POSITIVE(p_crm_pdb->p_glb_pdb,
                                                   *p_p_last_string_node)

					if(ICF_FAILURE == ret_val || ICF_NULL == *p_p_last_string_node)
						break;
					
					(*p_p_last_string_node)->p_next = ICF_NULL;
					/* increment count */
					p_expr->count++;

					/*  Allocate memory for the string */
					ICF_MEMGET(p_crm_pdb->p_glb_pdb,
						i,
		                ICF_MEM_COMMON,
						((*p_p_last_string_node)->p_str),
						ICF_DONOT_RET_ON_FAILURE,
						p_ecode,
						ret_val);
                    /*klocwork warning removal*/
                    ICF_KLOCWORK_FALSE_POSITIVE(p_crm_pdb->p_glb_pdb,
                                             (*p_p_last_string_node)->p_str)
					if(ICF_FAILURE == ret_val)
						break;
					
					/* store temp_str in the filter structure  */
					icf_port_strcpy((*p_p_last_string_node)->p_str,temp_str);
					
					/* Move the list pointer to the next node */
					p_p_last_string_node=(icf_string_node_st **) &((*p_p_last_string_node)->p_next);
				}
				break;
			}
		case ICF_PARSE_STATE_LOOKING_FOR_OPERATOR: 
            /* Look for an operator ( |, &). As we do not support 
               mixed expressions, we will compare the operator found 
               with the first operator which was found in this 
               expression. If it is the same operator then state is changed to
               LOOKING_FOR_STRING, otherwise FAILURE */
			{
				/* ignore spaces in expression */
				while(icf_port_isspace(*p_ch))
					p_ch++;
				if(*p_ch == '\0')
						break;

				/*  an operator was found earlier */
				if(ICF_TRUE == operator_found)
				{ 
					/* operator is same as before */
					if(*p_ch==opr)  
						state = ICF_PARSE_STATE_LOOKING_FOR_STRING;
					else
						/* unknown character or wrong operator */
						state = ICF_PARSE_STATE_PARSE_FAILED;
				}
				else /* operator not found till now */
				{
					switch(*p_ch)
					{
					case '&' : 
						/* set the operator in the filter structure */
						p_expr->match_all = ICF_TRUE;
					case '|' : 
						opr = *p_ch;
						operator_found = ICF_TRUE;
						state = 
							ICF_PARSE_STATE_LOOKING_FOR_STRING;
						break;
					default : state = ICF_PARSE_STATE_PARSE_FAILED;
						break;
						
					}
				}
				break;
			}
		default: break;
		}
	}
    /* After a successful parse the state of parsing will be 
       ICF_PARSE_STATE_LOOKING_FOR_OPERATOR */
	if( (ICF_PARSE_STATE_LOOKING_FOR_OPERATOR != state ) || (ICF_FAILURE == ret_val) )
	{

		/* Free all the memory */
		icf_cfg_free_filter_expr(p_crm_pdb,
			p_expr);
		ret_val=ICF_FAILURE;
	}
	return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_cfg_parse_filter_list
 *
 * DESCRIPTION:     This function parses the filter list given by an application 
 *                  in APP_ADD_REQ. It converts the application filter_list to
 *                  internal filter list strucutre format.
 *                  
 *					The function returns ICF_SUCCESS if the filter list is 
 *                  successfully parsed and stored in the internal filter list 
 *                  structure passed.
 *
 ******************************************************************************/

icf_return_t icf_cfg_parse_filter_list( INOUT icf_crm_pdb_st      *p_crm_pdb,
										IN		icf_filter_list_st		   *p_app_filter_list,
										INOUT	icf_int_filter_list_st  **p_p_int_filter_list,
										OUT		icf_error_t			   *p_ecode)
{
	icf_int_filter_st **p_p_curr_filter_node=ICF_NULL;
	icf_filter_st	  *p_curr_app_filter_node =ICF_NULL;
	icf_uint8_t i;
    icf_boolean_t validation_success, match_all_expr_allowed;


	icf_return_t ret_val = ICF_SUCCESS;

	
    ICF_MEMGET(p_crm_pdb->p_glb_pdb,
        ICF_PORT_SIZEOF(icf_int_filter_list_st),
        ICF_MEM_COMMON,
        (*p_p_int_filter_list),
        ICF_DONOT_RET_ON_FAILURE,
        p_ecode,
        ret_val);
    
    if(ICF_FAILURE == ret_val)			
    {
        p_crm_pdb->result = ICF_FAILURE;
        p_crm_pdb->error_cause = *p_ecode;
        return ret_val; 
    }

    icf_port_memset(*p_p_int_filter_list,0,ICF_PORT_SIZEOF(icf_int_filter_list_st));

  	(*p_p_int_filter_list)->match_all = p_app_filter_list->match_all;
		
    /*Initialization of list pointers */
	p_p_curr_filter_node = &((*p_p_int_filter_list)->p_filter_list);
	p_curr_app_filter_node = p_app_filter_list->p_filter_list;

	
	for(i=0;i<p_app_filter_list->count && ICF_NULL!=p_curr_app_filter_node;i++)
	{
        validation_success = ICF_TRUE;
        match_all_expr_allowed = ICF_TRUE;

        /* Filter Validations */
        switch(p_curr_app_filter_node->filter_type)
        {
            case ICF_FILTER_BY_HEADER_NAME:
                {
                    if( ICF_EXACT_STRING_MATCH != p_curr_app_filter_node->match_type)
                    {
                        validation_success = ICF_FALSE;
                    }
                }
                break;
            case ICF_FILTER_BY_METHOD_NAME:
                {
                    match_all_expr_allowed = ICF_FALSE;
                    if( ICF_EXACT_STRING_MATCH != p_curr_app_filter_node->match_type)
                    {
                        validation_success = ICF_FALSE;
                    }
                }
                break;
            case ICF_FILTER_BY_HEADER_CONTENT:
                {
                    if(0 == icf_port_strcmp(p_curr_app_filter_node->target_header.str,(icf_uint8_t *)""))
                    {
                        validation_success = ICF_FALSE;
                    }
                }
                break;
            case ICF_FILTER_BY_REQ_URI_HDR_PARAM:
                break;
            default:
                {
                    validation_success = ICF_FALSE;
                }
                break;
        }
        if(ICF_EXACT_STRING_MATCH == p_curr_app_filter_node->match_type)
		{
			match_all_expr_allowed = ICF_FALSE;
		}
		else if(ICF_SUB_STRING_MATCH == p_curr_app_filter_node->match_type)
        {
            
		}
		else
		{
			validation_success = ICF_FALSE;
        }

        if(ICF_FALSE == validation_success)
        {
			ret_val = ICF_FAILURE;
			break;
        }

        /* Alloc Memory for the node */
		ICF_MEMGET(p_crm_pdb->p_glb_pdb,
			ICF_PORT_SIZEOF(icf_int_filter_st),
			ICF_MEM_COMMON,
            (*p_p_curr_filter_node),
            ICF_DONOT_RET_ON_FAILURE,
            p_ecode,
            ret_val);
        
        if(ICF_FAILURE == ret_val)
            break;
            
        icf_port_memset((*p_p_curr_filter_node),
            0,
            ICF_PORT_SIZEOF(icf_int_filter_st));

		(*p_p_curr_filter_node)->p_next = ICF_NULL;
   		
		(*p_p_curr_filter_node)->filter_type = 
			p_curr_app_filter_node->filter_type;
		(*p_p_curr_filter_node)->match_type = 
			p_curr_app_filter_node->match_type;
        /* Target header is only required for filter type header content */
        if(ICF_FILTER_BY_HEADER_CONTENT == p_curr_app_filter_node->filter_type)
        {
            (*p_p_curr_filter_node)->target_header = p_curr_app_filter_node->target_header;
        }
	    /* SPR18458 Moved the incrementation of count before call
           to icf_cfg_parse_filter_expression function so that
           if failure happens in the parse_filter function then 
           also count is incremented.This leads to free of memory
           when icf_cfg_free_internal_filter_list is invoked. */
        (*p_p_int_filter_list)->count++;
        if(ICF_FAILURE == icf_cfg_parse_filter_expression (p_crm_pdb,
			(p_curr_app_filter_node->expr_str).str,
			&((*p_p_curr_filter_node)->filter_expr),
			p_ecode))
		{
			ret_val = ICF_FAILURE;
			break;
		}

        if((ICF_FALSE == match_all_expr_allowed) &&
            (ICF_TRUE == (*p_p_curr_filter_node)->filter_expr.match_all))
		{
			ret_val = ICF_FAILURE;
			break;
		}
        
		/* Advance pointers */
		p_p_curr_filter_node=(icf_int_filter_st ** )&((*p_p_curr_filter_node)->p_next);
		p_curr_app_filter_node = (icf_filter_st *)p_curr_app_filter_node->p_next;
	}

	if( (0 == (*p_p_int_filter_list)->count) || 
		(ICF_FAILURE == ret_val))
	{
	    /* dealloc memory */
		icf_cfg_free_internal_filter_list(p_crm_pdb,
            *p_p_int_filter_list);
        /* top level pointer is also freed in above function */

        (*p_p_int_filter_list) = ICF_NULL;
	}

    if(ICF_FAILURE == ret_val)
    {
		*p_ecode=ICF_CAUSE_FILTER_PARSE_FAILED;
    }

	return ret_val;
}


/*******************************************************************************
 * FUNCTION:        icf_cfg_free_filter_expr
 *
 * DESCRIPTION:     Function to free Filter Expression strucure 
 *
 ******************************************************************************/

icf_return_t icf_cfg_free_filter_expr(INOUT icf_crm_pdb_st     *p_crm_pdb,
									        INOUT icf_filter_expr_st	*p_filter_expr)
{
	icf_string_node_st *p_curr_node;
	icf_string_node_st *p_next_node;
	icf_uint8_t i;
	icf_error_t ecode;
	icf_return_t ret_val = ICF_SUCCESS;

	p_curr_node = p_filter_expr->p_match_str_list;
	
	for(i=0;(i<p_filter_expr->count) && (ICF_NULL != p_curr_node);i++)
	{
		p_next_node = (icf_string_node_st *)p_curr_node->p_next;
		/* Free the string */
		ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
			p_curr_node->p_str,
			ICF_MEM_COMMON,
			&ecode,
			ret_val);
		/* Free the node */
		ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
				p_curr_node,
				ICF_MEM_COMMON,
				&ecode,
				ret_val);
		
		p_curr_node = p_next_node;
	}
	return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_cfg_free_internal_filter_list
 *
 * DESCRIPTION:     Function to free Internal Filter List strucure 
 *
 ******************************************************************************/

icf_return_t icf_cfg_free_internal_filter_list(INOUT icf_crm_pdb_st         *p_crm_pdb,
													 INOUT icf_int_filter_list_st *p_filter_list)
{
    icf_int_filter_st *p_curr_node = ICF_NULL;
    icf_int_filter_st *p_next_node = ICF_NULL;
    icf_uint8_t i = ICF_NULL;
    icf_error_t ecode;
    icf_return_t ret_val = ICF_SUCCESS;

    if (ICF_NULL != p_filter_list)
    {
        p_curr_node = p_filter_list->p_filter_list;
    
        for(i=0;(i<p_filter_list->count)&&(ICF_NULL != p_curr_node);i++)
        {
            p_next_node = (icf_int_filter_st *)p_curr_node->p_next;

            icf_cfg_free_filter_expr(p_crm_pdb,&(p_curr_node->filter_expr));
            ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
                p_curr_node,
                ICF_MEM_COMMON,
                &ecode,
                ret_val);
    
            p_curr_node = p_next_node;
        }
    
        /* free the top level pointer */
        ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
            p_filter_list,
            ICF_MEM_COMMON,
            &ecode,
            ret_val)
    } /* End if(p_filter_list) */

	return ret_val;

}



/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_remove_app_ctxt
 *
 * DESCRIPTION:     This function is called when the same application sends 
 *                  app_add again and the app_add fails.
 *                  This function adds application context data. 
 *                  
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cfg_remove_app_ctxt(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_app_id_t app_id)
{
	
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_app_conf_st      *p_app_conf_1 = ICF_NULL;
 	icf_dbm_data_st      *p_dbm_data = ICF_NULL;
    icf_uint8_t           app_index = 0; 
    icf_app_conf_st      *p_app_conf = ICF_NULL;
    icf_error_t          ecode = ICF_ERROR_NONE;

	/* Get DBM data to fetch network access context */
    if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
                                  p_crm_pdb->p_glb_pdb,
                                  (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                                  (icf_void_t *)&p_dbm_data,
                                  &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
        p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

        if(ICF_SUCCESS == icf_cfg_fetch_app_conf(p_crm_pdb->p_glb_pdb,
                                        app_id, 
                                        &p_app_conf_1,
                                        &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
        {

			/* Indicate REGM about Application that is being removed */
			icf_rgm_remove_app_cntxt(p_crm_pdb,
				app_id,
				&ecode);
		
            /* set the trigger module as CFG */
            p_crm_pdb->cm_trigger_module = ICF_INT_MODULE_CFG;

			/* Send call clear indication to CM for all active calls */
			ret_val = icf_cfg_cm_call_clear_ind_wrt_app(
				p_crm_pdb,
				app_id);

				icf_cfg_fetch_app_index(p_crm_pdb, app_id, &app_index);
				
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

				p_app_conf[app_index].is_valid = ICF_FALSE;
				
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

 return ICF_SUCCESS;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_send_reg
 *
 * DESCRIPTION:     This function is called when the application has activated
 *                  the network. This function will send REGISTER incase the 
 *                    App ID is not GENERIC app ID and the AUTO_REG is ON.
 *                  
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t            icf_cfg_send_reg(
                INOUT        icf_crm_pdb_st            *p_crm_pdb)
{
    icf_app_id_t                app_id = ICF_NULL; 
    icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
    icf_return_t                ret_val = ICF_SUCCESS;
     
    icf_register_req_st            *p_payload = ICF_NULL;
    

        

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    p_glb_pdb = p_crm_pdb->p_glb_pdb;
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb->p_recv_msg))
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_crm_pdb->p_config_data))
    

    app_id = p_glb_pdb->p_recv_msg->hdr.app_id;

    if ((ICF_GENERIC_APP_ID == app_id)
        || (ICF_DUMMY_APP_ID == app_id))
    {
        ret_val = ICF_SUCCESS;
    }

    else if (ICF_TRUE == p_crm_pdb->p_config_data->auto_register)
    {    
        icf_msg_st           *p_temp_recv_msg = p_glb_pdb->p_recv_msg;
        icf_void_t            *p_temp_payload = p_crm_pdb->p_recv_payload;
        
        ICF_MEMGET(p_glb_pdb, 
            sizeof(icf_msg_st) + sizeof(icf_register_req_st) , 
            ICF_MEM_COMMON,
            p_glb_pdb->p_recv_msg, 
            ICF_RET_ON_FAILURE, 
            p_crm_pdb->p_ecode, 
            ret_val)
            
        icf_port_memset(p_glb_pdb->p_recv_msg, 0,
            sizeof(icf_msg_st) + sizeof(icf_register_req_st));
        
        /* memory allocated for p_recv_msg and payload */
        p_glb_pdb->p_recv_msg->hdr.api_id = ICF_REGISTER_REQ;
        p_glb_pdb->p_recv_msg->hdr.version = ICF_VERSION_1_0; 
        p_glb_pdb->p_recv_msg->hdr.source_id = ICF_DUMMY_MODULE_ID;
        p_glb_pdb->p_recv_msg->hdr.destination_id = ICF_MODULE_ICF;
        p_glb_pdb->p_recv_msg->hdr.call_id = (icf_uint16_t )ICF_INVALID_CALL_ID;
        p_glb_pdb->p_recv_msg->hdr.app_id = ICF_DUMMY_APP_ID;
        p_glb_pdb->p_recv_msg->hdr.api_length = sizeof(icf_msg_st) + 
                                                sizeof(icf_register_req_st);
        p_payload = (icf_register_req_st *)(p_glb_pdb->p_recv_msg->payload); 
        p_payload->bit_mask = ICF_NULL;

        /* deafult line id being filled */
        p_payload->line_id = ICF_USE_LINE1;
        p_payload->header_list.count = ICF_NULL;
        p_payload->tag_list.count = ICF_NULL;

        p_crm_pdb->p_recv_payload = p_payload;

        ret_val = icf_rgm_register_req_hdlr(p_crm_pdb);
        /* return type is not checked */

        /* free the memory allocated */
        ICF_MEMFREE(p_glb_pdb, 
                p_glb_pdb->p_recv_msg,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode, 
                ret_val)

        p_glb_pdb->p_recv_msg = p_temp_recv_msg;
        p_crm_pdb->p_recv_payload = p_temp_payload;
    }/* auto_reg selected: ends */
    else
    {/* auto_register not selected */
        ret_val = ICF_SUCCESS;
    }


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_cfg_send_reg() ends here */


 /*******************************************************************************
 *
 * FUNCTION:        icf_cfg_free_app_profile
 *
 * DESCRIPTION:     This function frees an app profile structure 
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cfg_free_app_profile(icf_crm_pdb_st      *p_crm_pdb,
                                            icf_app_profile_st *p_app_profile)
{
    icf_cmn_free_string_list(
        p_crm_pdb->p_glb_pdb,
        &(p_app_profile->supported_methods),
        p_crm_pdb->p_ecode);
    icf_cmn_free_string_list(
        p_crm_pdb->p_glb_pdb,
        &(p_app_profile->supported_events),
        p_crm_pdb->p_ecode);
    icf_cmn_free_string_list(
        p_crm_pdb->p_glb_pdb,
        &(p_app_profile->supported_content_types),
        p_crm_pdb->p_ecode);
    icf_cmn_free_string_list(
        p_crm_pdb->p_glb_pdb,
        &(p_app_profile->supported_encodings),
        p_crm_pdb->p_ecode);
    icf_cmn_free_string_list(
        p_crm_pdb->p_glb_pdb,
        &(p_app_profile->supported_languages),
        p_crm_pdb->p_ecode);
    return ICF_SUCCESS;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_move_string_list
 *
 * DESCRIPTION:     This function moves the contents of p_src to p_dest
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cfg_move_string_list(icf_string_list_st *p_dest,icf_string_list_st *p_src)
{
    p_dest->count = p_src->count;
    p_dest->p_list = p_src->p_list;
    p_src->count = 0;
    p_src->p_list = ICF_NULL;
    return ICF_SUCCESS;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_update_rgm_context_remove_entry
 *
 * DESCRIPTION:     This function updates the rgm context whenever a user/registrar
  *					registration for a line has to be removed.
 *					
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cfg_update_rgm_context_remove_entry(
						INOUT	icf_crm_pdb_st		*p_crm_pdb,
						INOUT	icf_rgm_context_st	*p_rgm_context,
						IN		icf_line_id_t		line_id)
{
	icf_line_id_t icf_line_count,
				icf_temp_count;
    p_crm_pdb = p_crm_pdb;
	
	for(icf_line_count=0;(icf_line_count<p_rgm_context->num_of_lines) && 
						(icf_line_count < ICF_MAX_NUM_OF_LINES);
						icf_line_count++)
	{
		if(line_id == p_rgm_context->rgm_list_of_lines[icf_line_count])
		{
			for(icf_temp_count = icf_line_count;
			(icf_temp_count<(p_rgm_context->num_of_lines)-1) && 
			(icf_temp_count < ICF_MAX_NUM_OF_LINES);
			icf_temp_count++)
			{
				if ( (icf_temp_count + 1) < ICF_MAX_NUM_OF_LINES)
				{
					p_rgm_context->rgm_list_of_lines[icf_temp_count] = 
					p_rgm_context->rgm_list_of_lines[icf_temp_count+1];
				}
			}
			break;
		}
	}
	p_rgm_context->num_of_lines--;
	return ICF_SUCCESS;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_delete_public_uri_list 
 *
 * DESCRIPTION : This function deletes the public uri list passed
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t 
icf_cfg_delete_public_uri_list (
                                icf_glb_pdb_st	*p_glb_pdb,
                                icf_pub_uri_list_node_st **p_p_pub_uri_list,
                                icf_error_t     *p_ecode
                                )
{
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_pub_uri_list_node_st *p_cur_node,*p_next_node;
    
    for(p_cur_node = *p_p_pub_uri_list;ICF_NULL != p_cur_node; p_cur_node = p_next_node)
    {
        p_next_node = p_cur_node->p_next;
        icf_dbm_delete_address_list(p_glb_pdb,
            &(p_cur_node->p_assoc_uri_list),
            p_ecode);
        ICF_MEMFREE(p_glb_pdb,
            p_cur_node,
            ICF_MEM_COMMON,
            p_ecode,
            ret_val)
    }
    return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:			icf_cfg_timer_expiry
 *
 * DESCRIPTION:         This function is called when TIMER_EXPIRY event is  
 *                      received from MRM.It checks the type of timer expired 
 *                      and invokes the respective handler.
 *
 ******************************************************************************/
icf_return_t icf_cfg_timer_expiry(
		        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_data_st                **p_p_timer_data = ICF_NULL;
    icf_return_t                     return_val = ICF_SUCCESS;

    /* SPR 18880 : Added NULL checks */
    if(ICF_NULL == p_crm_pdb->p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)"[CFG] : p_glb_pdb is NULL.\n"))
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) /* function entry trace */

    ICF_CHECK_IF_VAR_IS_NULL(p_crm_pdb->p_glb_pdb,
                             p_crm_pdb->p_glb_pdb->p_recv_msg,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ICF_FAILURE;
    }

    p_p_timer_data = (icf_timer_data_st **)
                    (p_crm_pdb->p_glb_pdb->p_recv_msg->payload);

	if (ICF_NULL != *p_p_timer_data)
	{
			ret_val = icf_rgm_timer_exp(p_crm_pdb);
	}
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*************************************************************************
 *
 * FUNCTION:        icf_cfg_get_asso_uris
 * 
 * DESCRIPTION:     This function returns all associated URIs          
 *                  
 *                  
 *                   
 * RETURNS:         ICF_SUCCESS: In case associated URI is found.       
 *                    ICF_FAILURE: In case no associated URI is found
 *************************************************************************/
icf_return_t    
icf_cfg_get_asso_uris (
    INOUT    icf_glb_pdb_st    *p_glb_pdb,
    INOUT    icf_pub_uri_list_node_st  **p_p_uris,
    INOUT    icf_error_t        *p_ecode)
{

    icf_config_data_st     *p_config_data = ICF_NULL;
    icf_line_data_st       *p_line_blk=ICF_NULL;
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_line_id_t            count;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    *p_p_uris = ICF_NULL;

    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_uris))
    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_ecode))
    
    /* Retreive the pointer for global CFG data*/
    if(ICF_FAILURE ==icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
        (icf_void_t *)&p_config_data,
        p_ecode))
    {
        ret_val = ICF_FAILURE;
    }

    else
    {
        icf_uint32_t num_lines = p_config_data->max_lines;
        /* traverse all the lines*/
        for (count = 0; count < num_lines; count++)
        {
            if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_glb_pdb,
                (icf_line_id_t const)(count),
                &p_line_blk,
                p_ecode))
            {
            /* Check for next line block. It is possible that there
            * may be holes in line data configuration. So leave
                * that. */
                continue;
            }
            else
            {
                icf_dbm_copy_user_list_to_pub_uri_list (
                    p_glb_pdb,
                    p_line_blk->p_user_db,
                    p_p_uris,
                    ICF_USER_DB_USE_ASSOCIATED_URIS,
                    p_ecode
                    );
				while(ICF_NULL != (*p_p_uris))
					p_p_uris = &((*p_p_uris)->p_next);

            }
        }
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val; 
}

/*************************************************************************
 *
 * FUNCTION:        icf_cfg_update_mm_self_ip
 * 
 * DESCRIPTION:              
 *                  
 *                  
 *                   
 * RETURNS:         ICF_SUCCESS:        
 *                    ICF_FAILURE: 
 *************************************************************************/
icf_return_t icf_cfg_update_mm_self_ip(
        INOUT icf_crm_pdb_st                         *p_crm_pdb)
{
	icf_internal_msg_st        internal_msg;/* = {0};*/
	ICF_PRINT(((icf_uint8_t*)"\n[CFG]: Entering Function: icf_cfg_update_mm_self_ip called\n"));
    icf_port_memset(&internal_msg,ICF_NULL,\
                         sizeof(icf_internal_msg_st));
	internal_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb;
	internal_msg.msg_hdr.msg_id = ICF_CONFIG_SELF_IP_REQ; 
    internal_msg.msg_hdr.payload_length = 0;
    internal_msg.p_msg_data = ICF_NULL;
    return icf_mmi_process_mesg(&internal_msg);
}

#ifdef ICF_NAT_RPORT_SUPPORT
/*************************************************************************
 *
 * FUNCTION:        icf_cfg_handle_nat_binding_chng
 * 
 * DESCRIPTION:     This function will handle the NAT binding change event
 *                  Clear all ongoing calls and unsubscribe all users.
 *                  
 * RETURNS:         ICF_SUCCESS:        
 *                  ICF_FAILURE: 
 *************************************************************************/

icf_return_t    icf_cfg_handle_nat_binding_chng
(
 INOUT  icf_crm_pdb_st          *p_crm_pdb
)
{
    
    icf_return_t     	       ret_val = ICF_SUCCESS;
    icf_internal_msg_st        *p_msg = ICF_NULL;	
    icf_rport_config_data_st   *p_rport_config_data = ICF_NULL;
    icf_line_data_st           *p_line_data = ICF_NULL;

     p_crm_pdb->line_id = ((icf_rgm_nw_resp_st *)(p_crm_pdb->
            p_internal_msg->p_msg_data))->p_rgm_context->line_id; 

     if (ICF_CFG_SELF_ADDR_PER_LINE ==
            p_crm_pdb->p_config_data->self_addr_per_line)
    {
        if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, p_crm_pdb->line_id,
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

    if (ICF_NULL == p_rport_config_data)
    {
       return ICF_FAILURE;
    } 

    if(ICF_TRUE == p_rport_config_data->is_rport_reqd)
    {
        if(ICF_TRUE == p_rport_config_data->nat_binding_chng)
        {
            /* Clear all ongoing calls and Unsubscribe all the users.  
             */
            /* Send call clear indication to CM */
   	    
	ICF_MEMGET(
            p_crm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_internal_msg_st),
            ICF_MEM_COMMON,
            p_msg,
            ICF_RET_ON_FAILURE,
            (p_crm_pdb->p_ecode),
            ret_val)
	    
	icf_port_memcpy((icf_void_t*)p_msg,(icf_void_t*)p_crm_pdb->p_internal_msg,\
	(icf_uint32_t)ICF_PORT_SIZEOF(icf_internal_msg_st));

        /* set the trigger module as CFG */
        p_crm_pdb->cm_trigger_module = ICF_INT_MODULE_CFG;

        if(ICF_FAILURE == icf_cfg_cm_call_clear_ind_wrt_line(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
#ifndef ICF_FEATURE_ABANDON
         else if(ICF_FAILURE == icf_cfg_mwi_trigger(p_crm_pdb,\
                        (icf_uint8_t)ICF_CFG_UNSUBSCRIBE_ALL))
         {
              ret_val = ICF_FAILURE;
        }
#endif
         else
         {
                p_crm_pdb->p_internal_msg->p_msg_data =\
                                   (icf_void_t*)(p_msg->p_msg_data);
 
                ICF_MEMFREE(
                        p_crm_pdb->p_glb_pdb,
                        p_msg,
                        ICF_MEM_COMMON,
                        p_crm_pdb->p_ecode,
                        ret_val)

                p_msg = ICF_NULL;
                ret_val = ICF_SUCCESS;
         }
        }
	
    }
    return ret_val;
}
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_validate_self_addr_per_line
 * 
 * DESCRIPTION:     This function will validate the self tranport parmeters
 *                  received per line.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_self_addr_per_line(
      icf_crm_pdb_st    *p_crm_pdb)
{
    icf_return_t                            ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_transport_params_req_st  *p_recv_msg_payload = ICF_NULL;
    icf_line_id_t                           line_id = 0;
    icf_uint8_t                             line_status[ICF_MAX_NUM_OF_LINES];
        
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_transport_params_req_st *)
        (p_crm_pdb->p_recv_payload);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

	/* Validate Action */
    line_id = p_recv_msg_payload->self_addr_per_line.line_id;
    
    p_crm_pdb->effective_self_mode = 
        p_recv_msg_payload->self_addr_per_line.transport_mode;

    /*If mode of operation is IMS and self mode is received as TLS 
     *then reject request*/
    if ((ICF_TRANSPORT_TYPE_TLS == p_crm_pdb->effective_self_mode) &&
        (ICF_OPERATION_MODE_IMS == p_crm_pdb->p_config_data->ims_oprn_flag))
    {
        ICF_CFG_SET_RESPONSE(p_crm_pdb, 
                ICF_FAILURE,
                ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE)	

        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }
     
    /* Address type can not be domain name */
    if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
       p_recv_msg_payload->self_addr_per_line.transport_addr.addr.addr_type)
    {
        ICF_CFG_SET_RESPONSE(
                p_crm_pdb,
                ICF_FAILURE,
                ICF_CAUSE_SELF_ADDR_DOMAIN_NAME_NOT_HANDLED)
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_TRNSPT_ADDR_RECD)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /* validate the range of port number */
    if((ICF_CFG_MIN_PORT_VALUE >
            p_recv_msg_payload->self_addr_per_line.transport_addr.port_num))
    {
        ICF_CFG_SET_RESPONSE(
               p_crm_pdb,
               ICF_FAILURE,
               ICF_CAUSE_IP_PORT_OUT_OF_RANGE)
        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_TRNSPT_PRMS_REQ_RECD)
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }    

    if(ICF_ALL_LINES != line_id)
    {
         /* Validate the line id */
         if((line_id >= p_crm_pdb->p_config_data->max_lines) || (line_id >= ICF_MAX_NUM_OF_LINES))
         {
             ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
             ICF_CFG_SET_RESPONSE(
                       p_crm_pdb,
                       ICF_FAILURE,
                       ICF_CAUSE_INVALID_LINE_ID)
             ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
             return ret_val;
         }
         /* Allocate line pointer if action is add. This step is done in 
          * validation, so that if failure occurs, API may be failed from 
          * here itself, and the remaining parameters of self Address and 
          * self mode need not be rolled back.
          */
         else if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->
                     self_addr_per_line.action) 
         {
             if(ICF_SUCCESS == icf_cfg_check_line_in_db(p_crm_pdb,
                                       line_id, line_status,
                                       (icf_boolean_t) ICF_FALSE))
             {
                 /* If pointer is returned by DBM i.e line is configured */
                 /* Check if proxy is already present on the line */
                 if (ICF_NULL != (ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD 
                     & p_crm_pdb->p_line_data[line_id]->line_data_received))
                 {
                     ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                     ICF_CFG_SET_RESPONSE(p_crm_pdb,
                                         ICF_FAILURE,
                                         ICF_CAUSE_INVALID_ACTION)

                     ret_val = ICF_SUCCESS;
                     ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
     	             return ret_val;
                 }
             }/* End if(ICF_SUCCESS== ..*/

             if(ICF_FAILURE == icf_cfg_check_line_in_db(
                      p_crm_pdb,
                      line_id,
                      line_status,
                      (icf_boolean_t)ICF_TRUE))
             {
                 ICF_CFG_SET_RESPONSE(
                         p_crm_pdb,
                         ICF_FAILURE,
                         ICF_CAUSE_INSUFFICIENT_RESOURCES)
                 ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
                 ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
                 return ret_val;
             }
         }/* End if action = add */
    }
    else 
    {
        /*AMIT: Send failure response, 
         *as ALL_LINES is not permitted in this*/
    }
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_set_self_addr_in_db_per_line
 * 
 * DESCRIPTION:     This function will update the self tranport parmeters
 *                  received per line.
 *                  
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_set_self_addr_in_db_per_line(
      icf_crm_pdb_st    *p_crm_pdb)
{
    icf_return_t                            ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_transport_params_req_st  *p_recv_msg_payload = ICF_NULL;
    icf_line_data_st                        *p_line_data = ICF_NULL;
        
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Copy the external message payload pointer locally */
    p_recv_msg_payload = (icf_cfg_cc_set_transport_params_req_st *)
        (p_crm_pdb->p_recv_payload);

    p_crm_pdb->line_id = p_recv_msg_payload->self_addr_per_line.line_id;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_recv_msg_payload))

    if ((ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                            p_crm_pdb->p_glb_pdb,
                            p_recv_msg_payload->self_addr_per_line.line_id,
                            &p_line_data,
                            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode))) ||
		(p_recv_msg_payload->self_addr_per_line.line_id >= ICF_MAX_NUM_OF_LINES))
    {
        ret_val = ICF_FAILURE;
    }
    /* If init complete is not done, update the Database */
    else if ((ICF_CONFIG_ACTION_ADD == 
           p_recv_msg_payload->self_addr_per_line.action) && 
        !(ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD & 
           p_line_data->line_data_received))
    {
         p_crm_pdb->p_line_data[p_recv_msg_payload->self_addr_per_line.line_id]
                 = p_line_data;
         ret_val = icf_cfg_update_self_trnspt_in_line_db(p_crm_pdb);

         /*p_line_data->line_data_received |= 
             ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD;*/
    }
    else if (ICF_CONFIG_ACTION_ADD == 
             p_recv_msg_payload->self_addr_per_line.action)
    {
        /*Send failure response*/
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_INVALID_ACTION)
        ret_val = ICF_SUCCESS;
    }
    else if ((ICF_TRUE == p_line_data->line_data_init_complete) &&
        (ICF_CONFIG_ACTION_DELETE == 
         p_recv_msg_payload->self_addr_per_line.action))
    {
         p_crm_pdb->p_line_data[p_recv_msg_payload->self_addr_per_line.line_id]
                 = p_line_data;

         /*Delete self transpoert from line, close nw server*/
         p_line_data->line_data_received &= 
              ~ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD;

         /* set the trigger module as CFG */
         p_crm_pdb->cm_trigger_module = ICF_INT_MODULE_CFG;

         /*Clear all the calls on this line*/
         icf_cfg_cm_call_clear_ind_wrt_line(p_crm_pdb);

         /*Deregister all the users from this line*/
         icf_cfg_dereg_for_line(p_crm_pdb);

         /*Close nw servers*/
         icf_cfg_close_nw_interface(p_crm_pdb);
        
         icf_port_memset(
                   (icf_void_t *)&(p_line_data->self_addr),
                   0,
                   (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

         p_line_data->self_mode = 0;
         p_line_data->line_data_init_complete = ICF_FALSE;
    }
    else if (ICF_CONFIG_ACTION_DELETE == 
             p_recv_msg_payload->self_addr_per_line.action)
    {
         p_crm_pdb->p_line_data[p_recv_msg_payload->self_addr_per_line.line_id]
                 = p_line_data;
        /*Delete the self transport from line*/
        p_line_data->line_data_received &= 
             ~ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD;

        icf_port_memset(
                   (icf_void_t *)&(p_line_data->self_addr),
                   0,
                   (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

        p_line_data->self_mode = 0;
    }
    else if (ICF_CONFIG_ACTION_MODIFY ==
         p_recv_msg_payload->self_addr_per_line.action)
    {
         p_crm_pdb->p_line_data[p_recv_msg_payload->self_addr_per_line.line_id]
                 = p_line_data;
         /*Send failure response, as MODIFY is not allowed for self trasnport*/
        /*Send failure response*/
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_CFG_SET_RESPONSE(
                    p_crm_pdb,
                    ICF_FAILURE,
                    ICF_CAUSE_SELF_ADDR_MODIFY_NOT_SUPP)
        ret_val = ICF_SUCCESS;
    }
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_update_self_trnspt_in_line_db
 * 
 * DESCRIPTION:     This function will update the self_transport data received 
 *                  in Set Transport params message into line database.
 *                  
 * RETURNS:         ICF_SUCCESS.
 *
 ******************************************************************************/
icf_return_t icf_cfg_update_self_trnspt_in_line_db(
        INOUT icf_crm_pdb_st                         *p_crm_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_cfg_cc_set_transport_params_req_st 
	                         *p_recv_msg_payload = ICF_NULL;
    icf_line_id_t		 line_id = 0;
    icf_line_data_st             *p_line_data = ICF_NULL;
    icf_line_data_st             *p_temp_line = ICF_NULL;
    icf_line_id_t                  i = 0;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
          
    ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_TRNSPT_ADDR_RECD)
    
    p_recv_msg_payload = 
        (icf_cfg_cc_set_transport_params_req_st *)p_crm_pdb->p_recv_payload;
    
    line_id = p_recv_msg_payload->self_addr_per_line.line_id;
    p_crm_pdb->line_id = line_id;

    if (( line_id < ICF_MAX_NUM_OF_LINES ) && (ICF_NULL != p_crm_pdb->p_line_data[line_id]))
    {
        p_line_data = p_crm_pdb->p_line_data[line_id];
        icf_port_memcpy(
              (icf_void_t *)&(p_crm_pdb->p_line_data[line_id]->self_addr),
              (icf_void_t *)&(p_recv_msg_payload->self_addr_per_line.transport_addr),
              (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

#ifdef ICF_NAT_RPORT_SUPPORT
        /* initialize the RPORT config data on when per line self IP is configured */
        icf_cfg_rport_config_data_init(
               p_crm_pdb->p_glb_pdb,
               &(p_line_data->p_rport_config_data));

        icf_port_memcpy(
               (icf_void_t *)&(p_line_data->p_rport_config_data->contact_ip_addr),\
               (icf_void_t *)&(p_recv_msg_payload->self_addr_per_line.transport_addr),
               (icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));

        p_line_data->p_rport_config_data->contact_ip_addr.port_num = \
               p_recv_msg_payload->self_addr_per_line.transport_addr.port_num;
#endif
        p_crm_pdb->p_line_data[line_id]->self_mode = 
             p_recv_msg_payload->self_addr_per_line.transport_mode;

        p_crm_pdb->p_line_data[line_id]->line_data_received |=
             ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD;

        /*Check if same transport address is confirgured on same line
         *and transport_mode is same or not*/
        for (i = 0; i < p_crm_pdb->p_config_data->max_lines; i++)
        {
           if (i == p_crm_pdb->line_id)
               continue;

           if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
               p_crm_pdb->p_glb_pdb,
               i,
               &p_temp_line,
               &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
           {
               if (
                    ((p_line_data->self_addr.addr.addr.ipv4_addr.octet_1 == 
                     p_temp_line->self_addr.addr.addr.ipv4_addr.octet_1) &&
                    (p_line_data->self_addr.addr.addr.ipv4_addr.octet_2 ==
                     p_temp_line->self_addr.addr.addr.ipv4_addr.octet_2) &&
                    (p_line_data->self_addr.addr.addr.ipv4_addr.octet_3 ==
                     p_temp_line->self_addr.addr.addr.ipv4_addr.octet_3) &&
                    (p_line_data->self_addr.addr.addr.ipv4_addr.octet_4 ==
                     p_temp_line->self_addr.addr.addr.ipv4_addr.octet_4)) &&
                    (p_line_data->self_addr.port_num == 
                       p_temp_line->self_addr.port_num))
               {
                    /*If mode is same then return success else derive the mode*/
                    if (p_line_data->self_mode != p_temp_line->self_mode)
                    {
                         ICF_CFG_SET_RESPONSE(
                            p_crm_pdb,
                            ICF_FAILURE,
                            ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE)

                         icf_port_memset(
                             (icf_void_t *)
                             &(p_crm_pdb->p_line_data[line_id]->self_addr), 0,
                             (icf_uint32_t)
                             ICF_PORT_SIZEOF(icf_transport_address_st));

                         p_crm_pdb->p_line_data[line_id]->self_mode = 0;
                         p_crm_pdb->p_line_data[line_id]->line_data_received &=
                            ~ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD;
                         ret_val = ICF_FAILURE;
                         break;
                    }
                }
            }
        }

        ICF_CFG_TRACE(p_crm_pdb, ICF_CFG_TRACE_SELF_TRNSPT_ADDR_SET)
    }
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_check_init_complete_for_line
 *
 * DESCRIPTION:     This function checks whether all the mandatory parameters
 *                  have been received for a line or not
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case module's global data cannot be fetched.
 *
 ******************************************************************************/
icf_return_t icf_cfg_check_init_complete_for_line(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
   
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /*
     * Check if mandatory parameters have been received for all lines.
     * Currently we are checking that we have atleast one user on
     * any one line
     */
   
    ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)

    /* Get line data pointer from DBM */
    if ( (p_crm_pdb->line_id < ICF_MAX_NUM_OF_LINES) && (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                            p_crm_pdb->p_glb_pdb,
                            p_crm_pdb->line_id,
                            &(p_crm_pdb->p_line_data[p_crm_pdb->line_id]),
                            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode))))
    {
        if((ICF_NULL !=
            (ICF_CFG_LINE_DATA_SELF_ADDR_RECD &
             p_crm_pdb->p_line_data[p_crm_pdb->line_id]->line_data_received)) &&
            (ICF_NULL !=
            (ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD &
             p_crm_pdb->p_line_data[p_crm_pdb->line_id]->line_data_received)))
        {
            ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_TRUE)
        }
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_cfg_init_complete_hdlr_for_line
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
icf_return_t icf_cfg_init_complete_hdlr_for_line(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_glb_data_st                      *p_glb_data = ICF_NULL;
    icf_void_t                           *p_port_data = ICF_NULL;
    icf_mmi_data_st                      *p_mmi_data = ICF_NULL;
    icf_line_data_st                     *p_line_data = ICF_NULL;

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

    ret_val = icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, p_crm_pdb->line_id,
                    &p_line_data, p_crm_pdb->p_ecode);


    if (ICF_FALSE == p_line_data->line_data_init_complete)
    {
        /* This indication has been received for the first time */
        /* Check whether all the mandatory parameters have been recd or
         * not */
        ret_val = icf_cfg_check_init_complete_for_line(p_crm_pdb);

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
                 p_line_data->line_data_init_complete = ICF_TRUE;
 
                 /* Mark the global data init also TRUE*/
                 p_glb_data->global_data_init_complete = ICF_TRUE;
             }
             else
             {     
                p_line_data->line_data_init_complete = ICF_FALSE;
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
                      p_crm_pdb, p_line_data->self_mode))
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


/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_call_clear_ind_wrt_line
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls on a particular line.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_call_clear_ind_wrt_line(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_uint16_t                         call_index = 1;
    icf_internal_msg_st                  sic_msg;
    icf_sic_clear_ctx_st		 sic_payload;
    icf_sic_ctx_st			 *p_sic_ctxt = ICF_NULL;
    
     
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Fetch call ctx from DBM until it returns failure */
    while (ICF_SUCCESS == icf_dbm_get_ong_call_ctx(
                                p_crm_pdb->p_glb_pdb,
                                call_index,
                                &(p_crm_pdb->p_glb_pdb->p_call_ctx),
                                &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
       if (p_crm_pdb->p_glb_pdb->p_call_ctx->line_id == 
           p_crm_pdb->line_id)
       {
           if(ICF_CALL_TYPE_EMERGENCY & 
                   p_crm_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
           {
               /* this is an emergency call and if the trigger is from REGM then do
                * not cler the call and in case the trigger is from CFG then clear the
                * call
                */
              if(ICF_INT_MODULE_REGM == p_crm_pdb->cm_trigger_module)
              {
                  continue;
              } 
           }
           /* Send message to MMI for each active call */
            p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                ICF_FORCEFUL_CALL_CLEAR_IND; 
            p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
            p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;
            
            ret_val = icf_mmi_process_mesg(p_crm_pdb->p_internal_msg);

            /* Send message to CM for each active call */
            p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                ICF_FORCEFUL_CALL_CLEAR_IND; 
            p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
            p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;
            
            ret_val = icf_cm_process_mesg(p_crm_pdb->p_internal_msg);
    
            /* Increment number of active call counter */
            /* call_index++; */
        }
            call_index++;
    }/* End while */

    call_index =1;

    while (ICF_SUCCESS == icf_dbm_get_ong_sic_ctx(
            p_crm_pdb->p_glb_pdb,
            call_index,
            &(p_sic_ctxt),
            &(p_crm_pdb->p_internal_msg->msg_hdr.ecode)))
    {
        /*temp check till SIC context initiallisation is fixed also 
        if the context is reg event package it wont be cleared*/
        if((ICF_INVALID_CALL_ID == p_sic_ctxt->call_id)||
         (0 == icf_port_strcmp((icf_uint8_t*)"reg",p_sic_ctxt->event_package.str)))
        {
            /* This SIC context is invalid and is returned only because not initialised properly*/
            call_index++;
            continue;
        }

        if(p_crm_pdb->line_id == p_sic_ctxt->line_id)
        {
    /* Clearing the SIC contexts */      
            /* to clear contexts for the required call id only */
            sic_msg.msg_hdr.msg_id = ICF_CLEAR_SIC_CTX;
    sic_msg.msg_hdr.ecode = 0; 
    sic_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb; 
            sic_payload.call_id = p_sic_ctxt->call_id;
            /*sic_payload.event_package = 0;*/
					
            sic_msg.msg_hdr.payload_length = 
            ICF_PORT_SIZEOF(icf_sic_clear_ctx_st);
            sic_msg.p_msg_data = (icf_uint8_t*)(&(sic_payload));
    
    icf_sic_process_mesg(&sic_msg);   
        }
        call_index ++;
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_dereg_for_line
 *
 * DESCRIPTION:     This function sends DeRegister request for particular line
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_dereg_for_line(
              icf_crm_pdb_st    *p_crm_pdb)
{
    icf_return_t          ret_val = ICF_SUCCESS;
    icf_rgm_context_st    *p_rgm_context = ICF_NULL;
    icf_user_db_node_st   *p_user_db = ICF_NULL;

    if (p_crm_pdb->line_id < ICF_MAX_NUM_OF_LINES) p_user_db = p_crm_pdb->p_line_data[p_crm_pdb->line_id]->p_user_db;
 
    while (ICF_NULL != p_user_db)
    {
        if(ICF_SUCCESS == icf_dbm_fetch_rgn_blk(
                  p_crm_pdb->p_glb_pdb,
                  &(p_user_db->public_user),
                  p_crm_pdb->line_id,
                  &p_rgm_context,
                  p_crm_pdb->p_ecode))
        {
            if(1 == p_rgm_context->num_of_lines)
            {
                /* SPR 16019: Set the bit mask in rgm_context to indicate that
                 * de-reg generated internally.
                 */
                p_rgm_context->indicator |= ICF_RGM_INTERNAL_GENERATED_REG;
                icf_rgm_deregister_user(p_crm_pdb, p_rgm_context);
                /* Calls and SIC context will be cleared from 
                 *inside this function*/
                 icf_rgm_clear_reg_context(p_crm_pdb, p_rgm_context);
            }
            else
            {	
                /* If we have more than one lines using this context 
                 * then we shall not deregister the user but update 
                 * the rgm context and remove the entry 
    		 * of this line.
    		 */
    		icf_cfg_update_rgm_context_remove_entry(p_crm_pdb,
                        p_rgm_context, p_crm_pdb->line_id);
            }
        }
        p_user_db = p_user_db->p_next;
    }

    return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_cfg_validate_supported_method_list
 * DESCRIPTION:     This function validates whether the all the methods specified
 *                  in app_add_req are supported by ICF
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 ******************************************************************************/
icf_return_t icf_cfg_validate_supported_method_list
(
        INOUT   icf_crm_pdb_st           *p_crm_pdb,
        IN      icf_string_list_st       *p_str_list
)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_string_list_node_st              *p_list = ICF_NULL;
    
    if(ICF_NULL == p_crm_pdb)
    {
        return ICF_FAILURE;
    }
 
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    if(p_str_list)
    {
        p_list = p_str_list->p_list;
    }

    while(p_list)
    {
        if(0 == icf_port_strcmpi(p_list->str.str, (icf_uint8_t *) "INVITE"))
        {}
        else if(0 == icf_port_strcmpi(p_list->str.str, (icf_uint8_t *) "MESSAGE"))
        {}
        else if(0 == icf_port_strcmpi(p_list->str.str, (icf_uint8_t *) "NOTIFY"))
        {}
        else if(0 == icf_port_strcmpi(p_list->str.str, (icf_uint8_t *)"REFER"))
        {}
        else if(0 == icf_port_strcmpi(p_list->str.str, (icf_uint8_t *)"INFO"))
        {}
        else if(0 == icf_port_strcmpi(p_list->str.str, (icf_uint8_t *)"OPTIONS"))
        {}
        else if(0 == icf_port_strcmpi(p_list->str.str, (icf_uint8_t *)"CANCEL"))
        {}
        else if(0 == icf_port_strcmpi(p_list->str.str,(icf_uint8_t *) "ACK"))
        {}
 	    else if(0 == icf_port_strcmpi(p_list->str.str, (icf_uint8_t *)"BYE"))
        {}
        else if(0 == icf_port_strcmpi(p_list->str.str, (icf_uint8_t *)"UPDATE"))
        {}
        else if(0 == icf_port_strcmpi(p_list->str.str, (icf_uint8_t *)"PRACK"))
        {}
        else
        {
            ret_val = ICF_FAILURE;
            break;
        }
        p_list = p_list->p_next;
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/*******************************************************************************
 *
 * FUNCTION:       icf_cfg_restart_reinit_rsp 
 *
 * DESCRIPTION:     This function will send an response to application after 
 *                  processing restart request sent by application
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/

icf_return_t icf_cfg_restart_reinit_rsp(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
        IN      icf_void_st*    p_routing_info,
        IN      icf_app_id_t    app_id,
        IN      icf_api_id_t    api_id)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_error_t                          *p_ecode = ICF_ERROR_NONE;
    icf_msg_st                           *p_msg = ICF_NULL;
    icf_uint16_t                         api_len = 0;

   ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

   if(ICF_NULL == p_routing_info)
   {
       ICF_PRINT(((icf_uint8_t*)"p_routing_info is null"));
   }



    /* Copy the ecode pointer locally */
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_ecode))

    api_len = ICF_PORT_SIZEOF(icf_api_header_st);

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
		p_crm_pdb->p_internal_msg->msg_hdr.msg_id = api_id;

        /* API-ID to send */
        p_msg->hdr.api_id = api_id;
        
        /*Payload length is size of api header + response structure*/
        p_msg->hdr.api_length = api_len;

        p_msg->hdr.call_id = ICF_INVALID_CALL_ID; 

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

}

/*******************************************************************************
 *
 * FUNCTION:       icf_cfg_restart_reinit_ind 
 *
 * DESCRIPTION:     This function will send an indication to other application after 
 *                  processing restart request sent by one application
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_restart_reinit_ind(
        INOUT   icf_crm_pdb_st *p_crm_pdb,
         IN     icf_void_st*    p_routing_info,
        IN      icf_app_id_t    app_id,
        IN      icf_api_id_t    api_id)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_error_t                          *p_ecode = ICF_ERROR_NONE;
    icf_msg_st                           *p_msg = ICF_NULL;
    icf_uint16_t                         api_len = 0;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)


    if(ICF_NULL == p_routing_info)
    {
       ICF_PRINT(((icf_uint8_t*)"p_routing_info is null"));
    }

    /* Copy the ecode pointer locally */
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_ecode))

    api_len = ICF_PORT_SIZEOF(icf_api_header_st);

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
		p_crm_pdb->p_internal_msg->msg_hdr.msg_id = api_id;

        /* API-ID to send */
        p_msg->hdr.api_id = api_id;
        
        /*Payload length is size of api header + response structure*/
        p_msg->hdr.api_length = api_len;

        p_msg->hdr.call_id = ICF_INVALID_CALL_ID; 

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
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_reinit_ind
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls.
 *                  
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_cm_reinit_ind(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19134)*/
    icf_internal_msg_st                  sic_msg = {{0,0,0,0,0,0},0};
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* set the trigger module as CFG */
    p_crm_pdb->cm_trigger_module = ICF_INT_MODULE_CFG;

    /* Send call clear indication to CM for all active calls */
    ret_val = icf_cfg_cm_forceful_call_clear(p_crm_pdb);
    /* SPR 9079: Fix Start */
    /* Clearing the SIC contexts */      
        
    sic_msg.msg_hdr.msg_id = ICF_FORCEFUL_REINIT_IND;
    sic_msg.msg_hdr.ecode = 0; 
    sic_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb; 
    sic_msg.msg_hdr.payload_length = 0;
    sic_msg.p_msg_data = ICF_NULL;
    
    icf_sic_process_mesg(&sic_msg);   


    /* SPR 9079: Fix End */


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


#ifdef ICF_NAT_RPORT_SUPPORT

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_rport_config_data_init
 *
 * DESCRIPTION:     This function initialises the RPORT config data 
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_cfg_rport_config_data_init(
        INOUT   icf_glb_pdb_st                     *p_glb_pdb,
        INOUT   icf_rport_config_data_st           **p_p_rport_config_data)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_error_t                  ecode = ICF_ERROR_NONE;
    icf_rport_config_data_st     *p_rport_config_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    if(ICF_NULL == *p_p_rport_config_data)
    {
         ICF_MEMGET(
            p_glb_pdb,
            ICF_PORT_SIZEOF(icf_rport_config_data_st),
            ICF_MEM_COMMON,
            *p_p_rport_config_data,
            ICF_RET_ON_FAILURE,
            &ecode,
            ret_val)        
    }

    p_rport_config_data = *p_p_rport_config_data;

    p_rport_config_data->is_rport_reqd = ICF_TRUE;
    p_rport_config_data->reg_id=ICF_NULL;
    p_rport_config_data->binding_refresh_timer =\
        ICF_CFG_BINDING_REFRESH_TIMER;

    if(ICF_NULL != p_rport_config_data->binding_refresh_timer_id)
    {
       ICF_STOP_TIMER(p_glb_pdb, p_rport_config_data->binding_refresh_timer_id,
           &ecode,ret_val)

       icf_dbm_ret_mem_to_pool(p_glb_pdb,
                           ICF_MEM_TIMER,
                           p_rport_config_data->p_binding_refresh_timer,
                           &ecode);
    }
    p_rport_config_data->binding_refresh_timer_id = ICF_NULL;
    p_rport_config_data->p_binding_refresh_timer=ICF_NULL;
    icf_port_memset((icf_void_t*)&(p_rport_config_data->contact_ip_addr),\
                    ICF_NULL,(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));
    icf_port_memset((icf_void_t*)&(p_rport_config_data->temp_contact_ip_addr),\
                    ICF_NULL,(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));
    icf_port_memset((icf_void_t*)&(p_rport_config_data->received_addr),\
                    ICF_NULL,(icf_uint32_t)ICF_PORT_SIZEOF(icf_transport_address_st));
    /*p_rport_config_data->ong_dereg_counter = 0;*/
    p_rport_config_data->nat_binding_chng = ICF_FALSE;
    p_rport_config_data->init_reg_duration = ICF_NULL;
    p_rport_config_data->line_id = ICF_NULL;
    icf_port_memset((icf_void_t*)&(p_rport_config_data->user_address),\
        ICF_NULL,(icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
    p_rport_config_data->is_initial_reg = ICF_FALSE;    
    p_rport_config_data->is_rport_completed = ICF_FALSE;
    p_rport_config_data->is_nat_resolved_sys = ICF_FALSE;

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}/* End function */
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_cm_forceful_call_clear 
 *
 * DESCRIPTION:     This function sends forceful call clear to CM for all active
 *                  calls.
 *
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/

icf_return_t icf_cfg_cm_forceful_call_clear(
                                INOUT icf_crm_pdb_st *p_crm_pdb) 
{
    icf_uint16_t                     call_index = 0;
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_dbm_pool_info_st             *p_call_ctx_pool = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    /* Fetch the call ctx pool address*/
    p_call_ctx_pool =  icf_dbm_get_pool_addr(
                       p_crm_pdb->p_glb_pdb,
                       ICF_BLK_TYPE_CALL_CTX);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_call_ctx_pool))

    /* Loop for pool_size times, if the call_id is valid at the 
       call_index then call CM for clearing the call */
    for(call_index =0; call_index < p_call_ctx_pool->pool_size; call_index++)   
    {
        /*When we are sending restart API we need not go by dbm_ong_call_ctx
          function but by incorporating the similar logic here. The need 
          for the same is for readablity and avoiding going in loop if somehow
          call_id is not reset in the blk array of call ctx after deallocation
         */
        if(ICF_INVALID_CALL_ID !=
          (((icf_call_ctx_st *)(p_call_ctx_pool->p_blk_array)) +    
               call_index)->call_id)
        {
            p_crm_pdb->p_glb_pdb->p_call_ctx =
                ((icf_call_ctx_st *)p_call_ctx_pool->p_blk_array) + 
                call_index;

            if(ICF_CALL_TYPE_EMERGENCY & 
                    p_crm_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
            {
                /* this is an emergency call and if the trigger is from REGM then do
                 * not cler the call and in case the trigger is from CFG then clear the
                 * call
                 */
                if(ICF_INT_MODULE_REGM == p_crm_pdb->cm_trigger_module)
                {
                    continue;
                } 
            } 
            /* Send message to MMI for each active call */
            p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                ICF_FORCEFUL_CALL_CLEAR_IND; 
            p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
            p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;

            ret_val = icf_mmi_process_mesg(p_crm_pdb->p_internal_msg);

            /* Send message to CM for each active call */
            p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                ICF_FORCEFUL_CALL_CLEAR_IND; 
            p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 0;
            p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;

            ret_val = icf_cm_process_mesg(p_crm_pdb->p_internal_msg);
        }/* End of if for INVALID call_id check*/
    }    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; 
} 

/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_process_auth_key_for_user
 *
 *  DESCRIPTION : This function adds/deletes the auth data
 *                in the user database of the given of given user.
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t icf_cfg_process_auth_key_for_user 
(
    icf_glb_pdb_st		*p_glb_pdb,
	icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload
)
{
    icf_return_t           ret_val = ICF_FAILURE;
    icf_user_db_node_st    *p_user_db = ICF_NULL;

    icf_error_t            ecode;

    ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(
                                p_glb_pdb,
                                p_recv_msg_payload->line_id,
                                &p_recv_msg_payload->self_addr,
                                ICF_USER_DB_USE_PUBLIC_URIS,
                                &p_user_db,
                                &ecode);

    if(ICF_SUCCESS == ret_val)
    {
        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            p_user_db->flags |= ICF_USER_DB_AUTH_PRMS_PER_USER;
            /* Copy key*/
            if(p_recv_msg_payload->bit_mask & 
                    ICF_CFG_CC_SET_SLF_ID_PRM_AUTH_KEY)
            {
                icf_port_memcpy(&(p_user_db->authentication_key),
                        &p_recv_msg_payload->authentication_key,
                        sizeof(icf_string_st));
                p_user_db->flags |= ICF_USER_DB_AUTH_KEY_RECD;
            }
        }
        else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
        {
            if(p_recv_msg_payload->bit_mask & 
                    ICF_CFG_CC_SET_SLF_ID_PRM_AUTH_KEY)
            {
                p_user_db->authentication_key.str_len = 0;
                icf_port_strcpy(
                        p_user_db->authentication_key.str,
                        (icf_uint8_t *)"\0");

                p_user_db->flags &= ~ICF_USER_DB_AUTH_KEY_RECD;
            }
        }
    }
    return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_process_auth_passwd_for_user
 *
 *  DESCRIPTION : This function adds/deletes the auth passwd
 *                in the user database of the given of given user.
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t icf_cfg_process_auth_passwd_for_user 
(
    icf_glb_pdb_st		*p_glb_pdb,
	icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload
)
{
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_user_db_node_st    *p_user_db = ICF_NULL;
    icf_error_t		   ecode;

    ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(
            p_glb_pdb,
            p_recv_msg_payload->line_id,
            &p_recv_msg_payload->self_addr,
            ICF_USER_DB_USE_PUBLIC_URIS,
            &p_user_db,
            &ecode);

    if(ICF_SUCCESS == ret_val)
    {
        if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
        {
            p_user_db->flags |= ICF_USER_DB_AUTH_PRMS_PER_USER;

            /* Copy Password*/
            if(p_recv_msg_payload->bit_mask &
                    ICF_CFG_CC_SET_SLF_ID_PRM_AUTH_PASSWORD)
            {
                icf_port_memcpy(&(p_user_db->authentication_password),
                        &p_recv_msg_payload->authentication_password,
                        sizeof(icf_string_st));
                p_user_db->flags |= ICF_USER_DB_AUTH_PASSWD_RECD;
            }
        }
        else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
        {
            if(p_recv_msg_payload->bit_mask &
                    ICF_CFG_CC_SET_SLF_ID_PRM_AUTH_PASSWORD)
            {
                p_user_db->authentication_password.str_len = 0;
                icf_port_strcpy(
                        p_user_db->authentication_password.str,
                        (icf_uint8_t *)"\0");

                p_user_db->flags &= ~ICF_USER_DB_AUTH_PASSWD_RECD;
            }
        }
    }
    return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_handle_dhcp_query
 *
 *  DESCRIPTION : This function internally invokes port functions to do 
 * 		  DHCP query. This function do default DHCP query for SIP 
 *  		  server discovery. It checks if DNS server address is 
 *                not configured by application then do DHCP query for DNS
 *		  server discovery also.
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_cfg_handle_dhcp_query
(
 INOUT icf_crm_pdb_st *p_crm_pdb
)
{
    icf_return_t           ret_val = ICF_FAILURE;
    icf_transport_address_st   *p_self_addr = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /*  Incease the stat of DHCP Query */
     ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->
                                        misc_stats.stats_DHCP_QUERY)

    /*If self address is configured as per line then fetch address from line_data
     *otherwise fetch address from config_data*/
    if (ICF_CFG_SELF_ADDR_PER_LINE !=
               p_crm_pdb->p_config_data->self_addr_per_line)
    {
        p_self_addr = &(p_crm_pdb->p_config_data->self_ip_address);
    }
    else
    {
         if (p_crm_pdb->line_id < ICF_MAX_NUM_OF_LINES) p_self_addr = &(p_crm_pdb->p_line_data[p_crm_pdb->line_id]->self_addr);
    }

    /*  This function will actualy do the DHCP Query on the DHCP Server */ 
    if (p_self_addr) ret_val = icf_port_process_dhcp_query (
            &(p_crm_pdb->p_config_data->p_dhcp_resp), 
            p_crm_pdb->p_config_data->p_dhcp_server_addr, 
            p_crm_pdb->p_config_data->dhcp_response_timer,
            p_self_addr,
            p_crm_pdb->p_config_data->dhcp_client_port,
            p_crm_pdb->p_ecode);

    if(ICF_FAILURE == ret_val)
    {
        /*  Incease the stat of DHCP Query Failure */
        ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->
                                        misc_stats.stats_DHCP_FAILURE_QUERY)

       /*  If no specefic error cause is set */
        if(ICF_NULL == *(p_crm_pdb->p_ecode))
        {
            ICF_CFG_SET_RESPONSE(
                  p_crm_pdb,
                  ICF_FAILURE,
                  ICF_CAUSE_DHCP_FAILURE)
        }
        else
        {
            ICF_CFG_SET_RESPONSE(
                  p_crm_pdb,
                  ICF_FAILURE,
                  *(p_crm_pdb->p_ecode))
        }
        ICF_CFG_APP_TRACE(p_crm_pdb, ICF_TRACE_DHCP_FAILURE)

    }
    else 
    {
        icf_line_id_t     line_id = 0;
        icf_uint8_t       line_status[ICF_MAX_NUM_OF_LINES];

        /* Increase the stats for   DHCP Success Query */
        ICF_STATS(p_crm_pdb->p_glb_pdb->p_stats_info->
                                        misc_stats.stats_DHCP_SUCCESS_QUERY)
        /*If proxy address in DHCP response is  received as IP address then
         *configure transport mode on all lines as self_mode*/
        if (ICF_SUCCESS == icf_port_check_if_ip(
                  p_crm_pdb->p_config_data->p_dhcp_resp->proxy_list[0].str))
        {
            for (line_id = 0; 
                 (line_id < p_crm_pdb->p_config_data->max_lines) &&
                 (line_id < ICF_MAX_NUM_OF_LINES);
                 line_id++)
            {
                 if (ICF_SUCCESS == icf_cfg_check_line_in_db(p_crm_pdb,
                     line_id, line_status,(icf_boolean_t) ICF_TRUE))
                 {
                     if (ICF_CFG_SELF_ADDR_PER_LINE ==
                         p_crm_pdb->p_config_data->self_addr_per_line)
                     {
                         p_crm_pdb->p_line_data[line_id]->transport_mode =
                             p_crm_pdb->p_line_data[line_id]->self_mode;
                     }
                     else
                     {
                         p_crm_pdb->p_line_data[line_id]->transport_mode =
                             p_crm_pdb->p_config_data->self_mode;
                     }
                 }
            }
        }
    }
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val; 
}
/*******************************************************************************
 *
 * FUNCTION NAME:   icf_cfg_validate_modify_regm_prms
 * 
 * DESCRIPTION:     This function validates the registrar address to be modified 
 *                  received in the API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ 
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_cfg_validate_modify_regm_prms(
        INOUT   icf_crm_pdb_st   *p_crm_pdb,
        IN      icf_line_id_t    line_id,
        IN      icf_boolean_t    validate_primary)
{
    icf_uint8_t                 line_status;
    icf_set_registration_req_st *p_recv_msg_payload = ICF_NULL;
    /* This boolean is set to true if the modification request in the API 
       has to be rejected.
    */
    icf_boolean_t               set_fail_resp = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    p_recv_msg_payload = (icf_set_registration_req_st *)(p_crm_pdb->p_recv_payload);
   
    if(ICF_SUCCESS == icf_cfg_check_line_in_db(
                          p_crm_pdb,line_id,&line_status,
                          (icf_boolean_t)ICF_FALSE))
    {
        if(ICF_TRUE == validate_primary)
        {
            /* Validate primary registrar address. If the primary registrar
               address is not configured, return failure
            */
            if(ICF_FAILURE == icf_cmn_compare_transport_addresses(
                                  &(p_crm_pdb->p_line_data[line_id]->registrar_addr),
                                  &(p_recv_msg_payload->registrar_addr),
                                  p_crm_pdb->p_glb_pdb))
            {
                /* The application is trying to modify a registrar address 
                   which has not been added.Set failure response.
                */
                set_fail_resp = ICF_TRUE;
            }
        }
        else
        {
            /* Validate secondary registar address. If the secondary registrar
               address is not configured, return failure
             */
            if(ICF_NULL == (ICF_CFG_LINE_DATA_SEC_REGISTRAR_RECD 
                    & p_crm_pdb->p_line_data[line_id]->line_data_received))
            {
                /* The application is trying to modify a registrar address 
                   which has not been added.Set failure response.
                */
                set_fail_resp = ICF_TRUE;
            }
            else
            {
                /* Secondary registrar address has been configured. Verify that the 
                   registrar address received in the modification request is same, if not
                   fail the request.
                */
                if(ICF_FAILURE == icf_cmn_compare_transport_addresses(
                                      p_crm_pdb->p_line_data[line_id]->p_sec_registrar_addr,
                                      &(p_recv_msg_payload->sec_registrar_addr.transport_addr),
                                      p_crm_pdb->p_glb_pdb))
                {
                    /* The application is trying to modify a registrar address 
                       which has not been added.Set failure response.
                     */
                   set_fail_resp = ICF_TRUE;
                }
            } 
        }
    }/* end icf_cfg_check_line_in_db */

    /* Set failure response and fail the request if the boolean has 
       been set to true. 
    */
    if(ICF_TRUE == set_fail_resp)
    {
        ICF_CFG_SET_VALID_PARAM(p_crm_pdb, ICF_FALSE)
        ICF_CFG_SET_RESPONSE(
                         p_crm_pdb,
                         ICF_FAILURE,
                         ICF_CAUSE_INVALID_ACTION)
    }

    return ICF_SUCCESS;
}

/* SPR 19405 Merged SPR 19672 Start */
/*******************************************************************************
 * FUNCTION:        icf_cfg_check_all_lines_dealloc 
 * DESCRIPTION:     This function checks whether all the configurable lines are
 *                  deallocated.
 * RETURNS:         ICF_SUCCESS in case of sucess and ICF_FAILURE
 *                  in case of failure.
 ******************************************************************************/
icf_return_t icf_cfg_check_all_lines_dealloc(
        INOUT   icf_crm_pdb_st           *p_crm_pdb
)
{
    icf_line_id_t 		line_index = 0;
    icf_line_data_st    *p_line_data = ICF_NULL;
    icf_return_t		ret_val = ICF_SUCCESS; 
 
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    while (line_index != p_crm_pdb->p_config_data->max_lines)
	{
		if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
					p_crm_pdb->p_glb_pdb, line_index,
					&p_line_data, p_crm_pdb->p_ecode))
		{
			if((ICF_NULL != (ICF_CFG_LINE_DATA_PROXY_RECD &
							p_line_data->line_data_received)) ||
							(ICF_NULL != (ICF_CFG_LINE_DATA_REGISTRAR_RECD &
							  p_line_data->line_data_received)) ||
							(ICF_NULL != (ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD &
							  p_line_data->line_data_received)) || 
							(ICF_NULL != (ICF_CFG_LINE_DATA_SELF_ADDR_RECD & 
						  p_line_data->line_data_received)))
			{
				ret_val = ICF_FAILURE;
				break;
			}
		}
        else
        {
           p_line_data = ICF_NULL;
        }
		line_index++;
	}

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
}
/* SPR 19405 Merged SPR 19672 End */

/* SPR 19886 : Merged Fix for CSR_1-7009954: SPR-19599*/
/************************************************************************
 *
 * FUNCTION NAME: icf_cfg_process_realm_for_user
 *
 *  DESCRIPTION : This function adds/deletes the realm
 *                in the user database of the given of given user.
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t icf_cfg_process_realm_for_user 
(
    icf_glb_pdb_st		*p_glb_pdb,
    icf_cfg_cc_set_self_id_params_req_st *p_recv_msg_payload
)
{
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_user_db_node_st    *p_user_db = ICF_NULL;
    icf_error_t            ecode = ICF_ERROR_NONE;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(
                            p_glb_pdb,
                            p_recv_msg_payload->line_id,
                            &p_recv_msg_payload->self_addr,
	                        ICF_USER_DB_USE_PUBLIC_URIS,
        	                &p_user_db,
                	        &ecode);

    if(ICF_SUCCESS == ret_val)
    {
       if(ICF_CONFIG_ACTION_ADD == p_recv_msg_payload->action)
       {
           /* Copy realm*/
           icf_port_memcpy(&(p_user_db->configured_realm),
                        &p_recv_msg_payload->configured_realm,
                       sizeof(icf_string_st));
           p_user_db->flags |= ICF_USER_DB_REALM_RECD;
       }
       else if(ICF_CONFIG_ACTION_DELETE == p_recv_msg_payload->action)
	   {
           p_user_db->configured_realm.str_len = 0;
           icf_port_strcpy(
                    p_user_db->configured_realm.str,
                    (icf_uint8_t *)"\0");

           p_user_db->flags &= ~ICF_USER_DB_REALM_RECD;
       }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
