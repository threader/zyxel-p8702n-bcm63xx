/*------------------------------------------------------------------------------
 *
 * File name        : icf_regm_util.c
 *
 * Purpose          : This file contains the utility functions used by
 *                    registration module.
 *
 * Revision History :
 *
 * Date           Name          Ref#                 Comments
 * --------    ------------     ----                 ---------------
 * 22-Dec-2004  Jalaj Negi     Non Service LLD      Initial
 * 26-Apr-2005  Jalaj Negi     SPR 7441             Added Stats
 * 26-Apr-2005  Jalaj Negi     SPR 7361             Added function for starting
 *                                                  timer of duration as
 * 17-Aug-2005    Jyoti Narula   ICF LLD                Coding for ICF :
 *
 * 12-Aug-2005  Saurabh Gupta  ICF_HLD              Changes wrt ICF Rel 3.0.0
 * 23-Sep-2005  Jalaj Negi     SPR - 8036            Fixed corruption in function
 *                                                    icf_regm_copy_uris.
 * 27-Sep-2005  Saurabh Gupta  SPR 8053             Fixed 401 handling in 
 *                                                  re-registration.
 * 28-Sep-2005  Saurabh Gupta  SPR 8062             nonce handling
 * 30-Sep-2005  Saurabh Gupta  SPR 8069             non-ims authentication
 * 07-Dec-2005  Rohit Aggarwal  ICF Rel 4.0 CDD     Coding for IPSec support
 * 08-DEC-2005  Puneet Sharma   ICF 4.0 CDD         Multiple app support
 * 02-Dec-2005    Jyoti Narula    ICF 4.0                new utility functions for ICF 4.0
 * 12-Dec-2005    Ashutos Rath    ICF 4.0                new utility functions parsing regifno
 * 
 * 28-Dec-2005 Jyoti Narula        SPR: 9068: fix        Checking the version number against INVALID value
 *                                9070                checks put for members of registration block
 *                                9071                checks puticf_rgm_update_auth_info for members of contact block
 *                                9067                change in icf_cmn_xmlparser_cbk_node_start()
 *                                                    for parsing of </end_tag>
 *
 * 06-Jan-2006    Jyoti Narula    SPR 8063/ 9043        For AKA all cases handling,
 *                                                    changes made in:
 *                                                    icf_rgm_fetch_nm_info(),
 *                                                    icf_rgm_ims_auth_
 *                                                    process()
 *                                                    icf_rgm_update_
 *                                                    auth_info()
 *
 * 06-Jan-2006    Jyoti Narula    SPR 9048            In icf_rgm_deregister_
 *                                                    user() SIC is no more
 *                                                    triggered to send the 
 *                                                    SUBSCRIBE with 0 in 
 *                                                    Expires.
 *
 * 27-Jan-2006    Jyoti Narula   SPR 9849                In rgm_context
 *                                                    the rgm_user_address 
 *                                                    now stores the value
 *                                                    as used in To for 
 *                                                    REGISTER message.
 *                                                    Changes made in
 *                                                    icf_rgm_update_rgm_cntxt
 *                                                    icf_rgm_exists_uri
 * 02-Apr-2006	Puneet Sharma	ICF5.0 LLD	 changes done in the comments 
 *											 Multiple Public Users ICF5.0
 * 
 * 28-Nov-2006  Deepti Goyal    ICF 6.2             NAT "rport" feature support
 * 28-Dec-2006  Deepti Goyal    SPR 14173           Update rport config data for the
 *                                                  next active registration block 
 *                                                  when the first one is getting 
 *                                                  de-registered.
 * 05-Apr-2007  Rajat Chandna                       Changes for wildcarded user
 *                                                  enhancement.
 * 7-Sep-2007   Deepti Goyal    SPR 16019           Reg-Dereg Enhancement
 * 6-Dec-2007   Amit Sharma     CSR 1-5390722       Merge to PQT 7.0 from 6_4
 * 25-Jun-2008  Rajiv Kumar     SPR - 18722         IPTK not compilable with gcc
 *                                                  version 4.2.3 
 * 25-Jul-2008  Puneet Sharma   merged SPR 18493 from 8.0
 * 23-Jun-2009  Rohit Aggarwal	SPR 19772	Corrected length of local variable 
 *											for storing registrar address
 * 26-Jun-09    Anurag Khare    CSR 1-7437801       Modified function
 *                                                  icf_rgm_get_ctxt_for_pub_uri
 * 02-July-09   Ashutosh Mohan  CSR-1-7423906       Modified icf_rgm_trace
 * 11-Dec-09    Pavneet Bajaj    SPR 20236          Fix for CSR 8070301 
 * 17-Dec-2009 Pavneet Bajaj      SPR 20244         Guard Timer Implementation
 * 21-Dec-2009  Rohit Aggarwal	SPR 20246	Memory leak fix
 * 30-Dec-2009  Pavneet Bajaj   SPR 20236   Fix for CSR 8070301
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_common_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_regm_prototypes.h"
#include "icf_internal_api_id.h"
#include "icf_regm_prototypes.h"
#include "icf_regm_macro.h"
#include "icf_port_macros.h"
/* For accesing the decodebase64 common function */
#include "icf_ssa_common_types.h"

#include "icf_sm_prototypes.h"

/****************************************************************************
 * FUNCTION:            icf_rgm_remove_app_cntxt
 *
 * DESCRIPTION:         This function is used to remove the Application's
 *                        context from the rgm_context. To be used by CFG
 *
 *
 * RETURNS:             
 ***************************************************************************/
icf_return_t    icf_rgm_remove_app_cntxt(
                INOUT    icf_crm_pdb_st    *p_crm_pdb,
                IN        icf_app_id_t        app_id,
                OUT        icf_error_t        *p_ecode)
{
    icf_uint32_t                loop_index = ICF_NULL;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
    
    icf_config_data_st        *p_config_data = ICF_NULL;
    icf_return_t                ret_val = ICF_FAILURE;

    icf_duration_t            default_duration = 
                                ICF_RGM_DEFAULT_DURATION_NON_IMS;
    
    
    /* DBM will return Failure if all Registration blocks are exhausted */
    loop_index = 1;

    /* Get the pointer to p_glb_pdb */
    p_glb_pdb = p_crm_pdb->p_glb_pdb;
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    default_duration = ICF_RGM_DEFAULT_DURATION_IMS_CLIENT;
ICF_CHECK_IMS_END
#endif
                
    /* get the config data pointer from DBM */
    ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)(&p_config_data),
                p_ecode);


    while (ICF_SUCCESS == ret_val &&
                ICF_SUCCESS == icf_dbm_get_used_rgn_blk(
                p_glb_pdb,
                (icf_uint16_t)loop_index,
                &p_rgm_context,
                p_ecode))
    {
        /* While get used registration context : Processing Begins */
        if (ICF_SUCCESS ==icf_rgm_find_n_remove_app(
                p_crm_pdb,app_id,p_rgm_context,p_config_data,p_ecode))
        {
            /* Initialize the rgm_duration with NULL */
            p_rgm_context->rgm_duration = ICF_NULL;
            
            ret_val = icf_rgm_save_smallest_dur(
                    p_glb_pdb, p_rgm_context, p_ecode);

            p_crm_pdb->p_rgm_context = p_rgm_context;

      if((ICF_NULL == 
                p_crm_pdb->p_rgm_context->rgm_duration) &&
                (ICF_IS_BIT_SET(p_crm_pdb->p_rgm_context->indicator,
                ICF_RGM_NFY_ON_PROBATION)))
            {
                 /* Stop the timer */
                 icf_rgm_stop_timer(p_crm_pdb);

                /* rgm_ctx is on probation: set it to UNREGISTERED state*/
                p_crm_pdb->p_rgm_context->rgm_state =
                    ICF_RGM_02_UNREGISTERED;
                
            }

           else if ((ICF_NULL == p_rgm_context->rgm_duration) &&
                    !(p_crm_pdb->p_config_data->auto_register))
           {

                /* None of the applications were valid entries */
                icf_rgm_stop_timer(p_crm_pdb);

                /* SPR 16019: If state is DEREGISTERING then no need to 
                 * de-register again
                 */
                if(ICF_RGM_05_DEREGISTERING != p_rgm_context->rgm_state)
                {
                    /* SPR 16019: Set the bit mask in rgm_context to indicate that
                     * de-reg generated internally.
                     */
                    p_rgm_context->indicator |= ICF_RGM_INTERNAL_GENERATED_REG;
                    /* To send deregister and clear the rgm_context */
                    ret_val = icf_rgm_deregister_user(
                            p_crm_pdb,p_rgm_context);
                }

                /* Invoke CFG 's function to deactivate the network */
                p_crm_pdb->resp_to_be_sent = ICF_FALSE;/*for CFG to not send response*/
                
        /* SPR: 8068: fix start */
                /* ret_val = icf_cfg_process_deactivate_on_dereg(p_crm_pdb); */
        /* SPR: 8068: fix end */
            }            
           else
           {				
               if ((ICF_NULL == p_rgm_context->rgm_duration) &&
                   (p_crm_pdb->p_config_data->auto_register))
               {
                   
                    /* If auto_register then default duration */                    
                    p_rgm_context->rgm_duration = default_duration;
                    
                }

               /* there were some valid application entries or auto_reg ON

 * to send a fresh REGISTER */
                if (ICF_RGM_03_REGISTERED == p_rgm_context->rgm_state)
                {
                    /* Stop Timer */
                    icf_rgm_stop_timer(p_crm_pdb);

                    ret_val = icf_rgm_register_user( p_crm_pdb,p_rgm_context);
                }  
                               

            }/* neither probation, nor deregister: ends */

                      
    
            break;/* App found in this Rgm context hence stop looping
                       * because our design prescribes
                       * one regm_context per application */
        }
         else
            loop_index++;
    
        

    
    }/* While get used registration context : Processing Ends */

    ret_val = ICF_SUCCESS; /* CFG has to get SUCCESS based on application
                               * removal only. */
    return ret_val;

}/* Function icf_rgm_remove_app_cntxt() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_find_n_remove_app
 *
 * DESCRIPTION:         This function is used to find and 
 *                        remove the Application's
 *                        context from the rgm_context. 
 *
 *
 * RETURNS:             
 ***************************************************************************/
icf_return_t     icf_rgm_find_n_remove_app(
                INOUT    icf_crm_pdb_st        *p_crm_pdb,
                IN        icf_app_id_t            app_id,
                INOUT    icf_rgm_context_st    *p_rgm_context,
                INOUT    icf_config_data_st    *p_config_data,
                OUT        icf_error_t            *p_ecode)
{
    icf_uint32_t                loop_app = ICF_NULL;
    icf_return_t                ret_val = ICF_FAILURE;
        icf_boolean_t               prbn_flag = ICF_FALSE;

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_rgm_context))
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_config_data))
            
    /* for each regm context */
    for (loop_app = 0; loop_app < p_config_data->max_applications;
                        loop_app++)
    {
        if (app_id == p_rgm_context->p_app_data_list[
                    loop_app].app_id)
        {
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
			/* To clear this application (and the user) related calls: */
				p_crm_pdb->line_id = p_rgm_context->rgm_list_of_lines[0];

				icf_cfg_cm_call_clear_ind_wrt_app_user(
					p_crm_pdb,
					&(p_rgm_context->rgm_user_address),
					app_id,
					p_crm_pdb->p_ecode);
ICF_CHECK_IMS_END
#endif
                /* App_id matches: Processing Begins */
			
                p_rgm_context->p_app_data_list[
                        loop_app].is_valid = ICF_FALSE;
        
                /* Free the lists if present */
                if (ICF_NULL != p_rgm_context->
                                    p_app_data_list[loop_app].header_list.count)
                {
                    /* free the header list */
                    ret_val = icf_cmn_free_header_list(
                                p_crm_pdb->p_glb_pdb,
                                &(p_rgm_context->p_app_data_list[
                                        loop_app].header_list),
                                p_ecode);
                    p_rgm_context->p_app_data_list[loop_app].
                        header_list.count = ICF_NULL;
                }
        
                /* Free the lists if present */
                if (ICF_NULL != p_rgm_context->
                                    p_app_data_list[loop_app].tag_list.count)
                {
                        
                    /* free the tag list */
                    ret_val = icf_cmn_free_tag_list(
                                p_crm_pdb->p_glb_pdb,
                                &(p_rgm_context->p_app_data_list[
                                        loop_app].tag_list),
                                p_ecode);
                    p_rgm_context->p_app_data_list[loop_app].
                            tag_list.count = ICF_NULL;
                }

                /* Free the lists if present */
                if (ICF_NULL != p_rgm_context->
                                    p_app_data_list[loop_app].body_list.count)
                {
                    /* free the body list */
                    ret_val = icf_cmn_free_msg_body_list(
                                p_crm_pdb->p_glb_pdb,
                                &(p_rgm_context->p_app_data_list[
                                            loop_app].body_list),
                                p_ecode);
                    p_rgm_context->p_app_data_list[loop_app].
                                body_list.count = ICF_NULL;
                }

                p_rgm_context->p_app_data_list[loop_app].bitmask = ICF_NULL;
                p_rgm_context->p_app_data_list[loop_app].
                                        app_indicator = ICF_NULL;

                ret_val = ICF_SUCCESS;/* Out of loop */


            }/* App_id matches: Processing Ends */

 else if ((ICF_TRUE == p_rgm_context->p_app_data_list[
            loop_app].is_valid) &&
            (ICF_IS_BIT_SET(p_rgm_context->p_app_data_list[
            loop_app].app_indicator,
            ICF_RGM_APP_DEREG_ON_PROBATION)))
        {
            /* find the probation status  */
            prbn_flag = ICF_TRUE;
        }

        }/* For loop on app_data_list: ends */

    if ((ICF_FALSE == p_rgm_context->auto_reg_flag) &&
        (ICF_AUTO_REG_PROBATION != p_rgm_context->event_auto_reg) &&
        (ICF_FALSE == prbn_flag) &&
        (ICF_IS_BIT_SET(p_rgm_context->indicator,
                ICF_RGM_NFY_ON_PROBATION)))
    {
        /* reset the ON_PROBATION bit */
        ICF_RESET_BIT(p_rgm_context->indicator,
            ICF_RGM_NFY_ON_PROBATION)
    }

    return ret_val;

}/* Function icf_rgm_find_n_remove_app() ends here*/


/****************************************************************************
 * FUNCTION:            icf_rgm_validate_register_api
 *
 * DESCRIPTION:         This function is used to validate the payload
 *                      of REGISTER_REQ API.
 *
 *
 * RETURNS:             
 ***************************************************************************/
icf_void_t icf_rgm_validate_register_api(
          INOUT icf_crm_pdb_st *p_crm_pdb)
{
    if ((p_crm_pdb->p_config_data->max_lines > ((icf_register_req_st *)
                (p_crm_pdb->p_recv_payload))->line_id))
                
    {
        /* Line_id should be less than max_lines value of config data */
        p_crm_pdb->is_valid_param = ICF_TRUE;
    }
    else
    {
        p_crm_pdb->is_valid_param = ICF_FALSE;

    }



}/* Function icf_rgm_validate_register_api  ends here */


/****************************************************************************
 * FUNCTION:            icf_rgm_parse_string_to_address
 *
 * DESCRIPTION:         This function is used to parse the string
 *                      to address_st type.
 *                        The value before : is compared with TEL and SIP
 *                        to decide the addr_type.   
 *                        The address_st type is undefined
 *                        if the : is not present, 
 *
 *
 * RETURNS:             
 ***************************************************************************/
icf_void_t   icf_rgm_parse_string_to_address(
                IN      icf_uint8_t          *p_address_string,
                INOUT   icf_address_st          *p_address)
{
    icf_uint8_t            *p_temp_val = ICF_NULL;
    icf_uint8_t            temp_type[10];
    icf_uint8_t            loop_index = ICF_NULL;

    p_temp_val = icf_port_strchr (p_address_string, ':');
    if (ICF_NULL == p_temp_val)
    {
        /* Colon(:) not found in the address_string 
         * simply copy the whole string */
        icf_port_strcpy(p_address->addr_val.str, p_address_string);
        p_address->addr_type = ICF_ADDRESS_PLAN_UNDEFINED;

    }
    else
    {/* address string with : Processing Begins */

        /* Copy the URI from next to colon */
        p_temp_val++;
        icf_port_strcpy(p_address->addr_val.str, p_temp_val);

        /* compare the string before : to sip or tel */

        /* Copy the type in temp_type */
        for (loop_index = 0; *p_address_string != ':'; loop_index++, 
                                                p_address_string++)
        {
            temp_type[loop_index] = *p_address_string;
        }
        temp_type[loop_index] = '\0';
        

        if (ICF_NULL == icf_port_strcmp(temp_type, (icf_uint8_t *)"sip"))
        {
            /* address type: SIP */
            p_address->addr_type = ICF_ADDRESS_PLAN_SIP_URI;
        }

        else if (ICF_NULL == icf_port_strcmp(temp_type,  (icf_uint8_t *)"tel"))
        {
            /* address type : TEL */
            p_address->addr_type = ICF_ADDRESS_PLAN_TEL_URI;

        }
        else
        {
            /* address type undefined */
            p_address->addr_type = ICF_ADDRESS_PLAN_UNDEFINED;

        }
        
    }/* address string with : Processing Ends */

       
    p_address->addr_val.str_len = (icf_uint16_t ) icf_port_strlen(p_address->addr_val.str);
    

}/* Function icf_rgm_parse_string_to_address() ends here*/


/******************************************************************************
 *
 * FUNCTION:        icf_rgm_update_rgm_cntxt
 * 
 * DESCRIPTION:     This function updates the regn context with the
 *                  payload of register_request API/
 *                  
 *                   
 * RETURNS:         ICF_SUCCESS: In case regm ctxt is updated.       
 *                    ICF_FAILURE: In case no regm_ctxt is not updated
 ******************************************************************************/
icf_return_t     icf_rgm_update_rgm_cntxt(
                INOUT icf_crm_pdb_st         *p_crm_pdb,                
                IN    icf_address_st         *p_rgm_uri)
                
{
    icf_line_data_st    *p_line_data = ICF_NULL;
    icf_app_id_t            app_id = ICF_NULL;

    icf_boolean_t        app_status = ICF_FALSE;
    icf_rgm_app_ctxt_st    *p_app_data = ICF_NULL;

    icf_boolean_t        line_present = ICF_FALSE;
    icf_uint32_t            loop_index = ICF_NULL;


    icf_return_t         ret_val = ICF_SUCCESS;
    icf_rgm_context_st    *p_rgm_context = ICF_NULL;

    icf_register_req_st        *p_register_req_payload;
#ifdef IMS_CLIENT
    icf_line_id_t             ctr = ICF_NULL;
#endif
    
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
                (ICF_NULL != p_crm_pdb->p_glb_pdb->p_recv_msg))

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
                (ICF_NULL != p_crm_pdb->p_rgm_context))

    p_rgm_context = p_crm_pdb->p_rgm_context;


    /* Copy the app_id from the received api's header */
    app_id = p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;

     /* Get the pointer to the payload received with the API */
    p_register_req_payload = (icf_register_req_st *)
                                    (p_crm_pdb->p_recv_payload);

    
    if (ICF_RGM_00_IDLE == p_rgm_context->rgm_state)
    {
        /* This is a new registration context: Processing Begins */

        /* Copy the rgm_uri as the first element of rgm_uris of 
         * the rgm_context */
        /* memory was given to this array of uris at the time when
         * memory to rgm_context was being allocated. */

	/* During Merge, this change made: because in IMS scenario
	 * also, the rgm_user_address will store the TO/FROM just like
	 * NON IMS scenario. This was SPR 9849 */
        
        p_rgm_context->rgm_user_address.addr_type =
                                       p_rgm_uri->addr_type;

        icf_port_strcpy(p_rgm_context->rgm_user_address.addr_val.str, 
                            p_rgm_uri->addr_val.str);
                        
        p_rgm_context->rgm_user_address.addr_val.str_len =
                                        p_rgm_uri->addr_val.str_len;

        /* line Id being copied into rgm_context */
        /* In IMS mode, the user info needs to be copied onto all the 
         * configured lines */
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb, ICF_TRUE)

        p_rgm_context->num_of_lines = p_crm_pdb->p_config_data->max_lines;
    
        for (ctr = 0; ctr < p_rgm_context->num_of_lines; ctr++)
        {
            p_rgm_context->rgm_list_of_lines[ctr] = ctr;
        }
ICF_CHECK_IMS_END
#endif

#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb, ICF_FALSE)
        p_rgm_context->num_of_lines = 1;
        p_rgm_context->rgm_list_of_lines[0] = 
                        p_register_req_payload->line_id;
ICF_CHECK_IMS_END
#endif

        /* Line data retrival: to get the registrar address */
        if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                p_crm_pdb->p_glb_pdb, p_rgm_context->rgm_list_of_lines[0],
                &p_line_data, p_crm_pdb->p_ecode))
        {
            /* Unable to get the line block for this line id */
            return ICF_FAILURE;

        }
            
        /* copying the registrar details */
        p_rgm_context->list_of_registrar_ip[0].addr_type = 
                            p_line_data->registrar_addr.addr.addr_type;

        if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
                            p_rgm_context->list_of_registrar_ip[0].addr_type)
        {
            /* When registrar is stored as domain name : */
            
            icf_port_strcpy(p_rgm_context->
			    list_of_registrar_ip[0].addr.domain.str,
                    p_line_data->registrar_addr.addr.addr.domain.str);
            p_rgm_context->list_of_registrar_ip[0].addr.domain.str_len =
                    p_line_data->registrar_addr.addr.addr.domain.str_len;
        }

        else if (ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                           p_rgm_context->list_of_registrar_ip[0].addr_type)
        {
            /* When registrar is stored as IPv4 */
            p_rgm_context->list_of_registrar_ip[0].addr.ipv4_addr.octet_1 =
                    p_line_data->registrar_addr.addr.addr.ipv4_addr.octet_1;
            p_rgm_context->list_of_registrar_ip[0].addr.ipv4_addr.octet_2 =
                    p_line_data->registrar_addr.addr.addr.ipv4_addr.octet_2;
            p_rgm_context->list_of_registrar_ip[0].addr.ipv4_addr.octet_3 =
                    p_line_data->registrar_addr.addr.addr.ipv4_addr.octet_3;
            p_rgm_context->list_of_registrar_ip[0].addr.ipv4_addr.octet_4 =
                    p_line_data->registrar_addr.addr.addr.ipv4_addr.octet_4;

            
        }

        else if (ICF_TRANSPORT_ADDRESS_IPV6_ADDR ==
                          p_rgm_context->list_of_registrar_ip[0].addr_type)
        {
            /* When registrar is stored as IPv6 */
            p_rgm_context->list_of_registrar_ip[0].addr.ipv6_addr.str_len =
                    p_line_data->registrar_addr.addr.addr.ipv6_addr.str_len;
            icf_port_strcpy(p_rgm_context->list_of_registrar_ip[0].addr.
                            ipv6_addr.str,
                    p_line_data->registrar_addr.addr.addr.ipv6_addr.str);
                        
        }
        else
        {
            return ICF_FAILURE;
        }

    

        /* Adding the application data to the registration context
        *  Processing Begins */
        if (ICF_DUMMY_APP_ID != app_id)
        {
            
            if (ICF_FALSE == p_rgm_context->p_app_data_list[0].is_valid)
            {
                /* This entry can be used to copy the application data */
                
                if (ICF_FAILURE == icf_rgm_copy_app_details(
			p_crm_pdb->p_glb_pdb,
                    &(p_rgm_context->p_app_data_list[0]),
                    p_register_req_payload,app_id,p_crm_pdb->p_ecode))
                {
                    return ICF_FAILURE;
                }
                
                
                }/* Adding the application data to the registration context
            *  Processing Ends */
        }/* Non DUMMY APP ID case: NON auto register case */
        else
        {
            /* Auto regsiter case: no app context to be added */
        }
   
    }/* This is a new registration context: Processing Ends */

    else
    {
        /* Processing Begins: 
         * This rgm_context is an old one: It will already contain the
         * rgm_uri selected for registration: */

        /* Transport and Registrar related information will already present */

        if (ICF_DUMMY_APP_ID != app_id)
        {/* Non auto register case : begins */
            
            /* Handling the application related 
	     * information in the rgm_context */
            
            app_status = icf_rgm_fetch_app_info(
			    p_crm_pdb,p_rgm_context,
                app_id,&p_app_data);
            
            if (ICF_TRUE == app_status)
            {
                /* Application data already present in the rgm_context */
                /* Simply copy the new values. */
                if (ICF_FAILURE == icf_rgm_copy_app_details(
			p_crm_pdb->p_glb_pdb,
	                p_app_data,
                        p_register_req_payload,app_id,p_crm_pdb->p_ecode))
                {
                    return ICF_FAILURE;
                }
                /* Readying the app_indicator bit for sending the CFM. */
                p_app_data->app_indicator = 0;
                
            }
            
            else
            {
            /* Application not already present: Hence find an empty entry:
                * If no empty entry found: Send the message for failure */
                if (ICF_FAILURE == icf_rgm_get_empty_app(
			p_crm_pdb->p_glb_pdb,
                       p_crm_pdb,p_rgm_context, 
                       &p_app_data, p_crm_pdb->p_ecode))
                {
                    p_crm_pdb->error_cause = 
			    ICF_CAUSE_MAX_NUM_APPLICATION_EXCEEDED;
                    return ICF_FAILURE;
                }
                
                else
                {
                    if (ICF_FAILURE == icf_rgm_copy_app_details(
					    p_crm_pdb->p_glb_pdb,
		                            p_app_data,
                                            p_register_req_payload,app_id,
					    p_crm_pdb->p_ecode))
                    {
                        return ICF_FAILURE;
                    }
                    
                    
                }/* App details copied onto empty app_data member */
                
                 }    /* Processing ends:
            * Application not already present: */
        }/* Non auto register case : ends */

        /* Checking for line_id in the old_rgm_ctxt */
        for(loop_index = 0; loop_index < p_rgm_context->num_of_lines;
                        loop_index++)
        {
            if(p_register_req_payload->line_id ==
                        p_rgm_context->rgm_list_of_lines[loop_index])
            {
                line_present = ICF_TRUE;
                break;
            }
        }/* For loop on lines ends */

        if (ICF_FALSE == line_present)
        {
            p_rgm_context->rgm_list_of_lines[p_rgm_context->num_of_lines] = 
                            p_register_req_payload->line_id;
            p_rgm_context->num_of_lines++;
        }

    }/* Processing Ends: This rgm_context is an old one */

    return ret_val;
    
        
}/* Function icf_rgm_update_rgm_cntxt() Ends here */

#ifdef IMS_CLIENT

/***********************************************************************
 *
 * FUNCTION:        icf_regm_copy_uris
 * 
 * DESCRIPTION:     This function copies the uri from rgm_context
 *                    to uri_list_st
 *                  
 *                   
 * RETURNS:         Nothing
 *************************************************************************/
icf_return_t icf_regm_copy_uris(
            icf_glb_pdb_st        *p_glb_pdb,
            icf_pub_uri_list_node_st **p_p_uri_list,
            icf_rgm_context_st    *p_rgm_context,
            icf_line_id_t         line_id,
            icf_error_t           *p_ecode)
{
    icf_line_id_t i;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_pub_uri_list_node_st **p_p_original_uri_list = p_p_uri_list;
    icf_user_db_node_st *p_node = ICF_NULL;
    icf_address_st *p_user_address = ICF_NULL;

    if(ICF_NULL == (p_rgm_context->indicator & ICF_RGM_IMPLICITLY_REGISTERED))
    {
        p_user_address = &(p_rgm_context->rgm_user_address);
    }
    else
    {
        p_user_address = p_rgm_context->p_registered_pub_uri;
    }


    if(ICF_ALL_LINES == line_id)
    {
        for(i=0;i<p_rgm_context->num_of_lines; i++)
        {
            if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line (
                p_glb_pdb,
                p_rgm_context->rgm_list_of_lines[i],
                p_user_address,
                ICF_USER_DB_USE_PUBLIC_URIS,
                &p_node,
                p_ecode))
            {
                icf_dbm_copy_public_user_node_to_pub_uri_list (
                    p_glb_pdb,
                    p_node,
                    p_p_uri_list,
                    ICF_USER_DB_USE_ASSOCIATED_URIS,
                    p_ecode);
            }
            else
            {
                /*delete the uri list */
                icf_cfg_delete_public_uri_list (p_glb_pdb,
                    p_p_original_uri_list,
                    p_ecode);
                ret_val = ICF_FAILURE;
                break;
            }
            
            /* Get to the last node */
            while(ICF_NULL != *p_p_uri_list)
                p_p_uri_list = (icf_pub_uri_list_node_st **) &((*p_p_uri_list)->p_next);
        }
    }
    else 
    {
        if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line (
            p_glb_pdb,
            line_id,
            p_user_address,
            ICF_USER_DB_USE_PUBLIC_URIS,
            &p_node,
            p_ecode))
        {
            icf_dbm_copy_public_user_node_to_pub_uri_list (
                p_glb_pdb,
                p_node,
                p_p_uri_list,
                ICF_USER_DB_USE_ASSOCIATED_URIS,
                p_ecode);
        }
        else
        {
            /*delete the uri list */
            icf_cfg_delete_public_uri_list (p_glb_pdb,
                p_p_original_uri_list,
                p_ecode);
            ret_val = ICF_FAILURE;
        }
    }

    return ret_val;
}/* Function icf_regm_copy_uris() ends here */
ICF_CRM_END
#endif

/***************************************************************************
 * FUNCTION:            icf_rgm_handle_dereg
 *
 * DESCRIPTION:         This function is used to handle the DEREG
 *                        request made by the application
 *
 *
 * RETURNS:             
 *************************************************************************/
icf_return_t        icf_rgm_handle_dereg(
                INOUT    icf_glb_pdb_st        *p_glb_pdb, 
                INOUT    icf_rgm_context_st    *p_rgm_context, 
                INOUT    icf_crm_pdb_st        *p_crm_pdb, 
                OUT        icf_error_t            *p_ecode)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_app_id_t            app_id  = ICF_NULL;
    
    icf_rgm_app_ctxt_st        *p_app_info = ICF_NULL;
    icf_boolean_t            app_status = ICF_FALSE;

    icf_register_req_st            *p_recv_msg_payload = ICF_NULL;
#ifdef IMS_CLIENT
    icf_duration_t            default_duration = 
                                ICF_RGM_DEFAULT_DURATION_NON_IMS;
#endif
/*SENDING RELATED*/
    icf_rgm_register_resp_st        register_response;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb->p_recv_msg))
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgm_context))

    register_response.options_flag = ICF_NULL;

#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    default_duration = ICF_RGM_DEFAULT_DURATION_IMS_CLIENT;
ICF_CHECK_IMS_END
#endif
    /* Get the pointer to the payload received with the API */
    p_recv_msg_payload = (icf_register_req_st *)
                                    (p_crm_pdb->p_recv_payload);

    /* retrieving the app_id from recd_msg */
    app_id = p_glb_pdb->p_recv_msg->hdr.app_id;
    *p_ecode = *p_ecode;

    /* obtain the Line Id of this application
     * which was stored when the application registered itself */
    app_status = icf_rgm_fetch_app_info(p_crm_pdb,
                            p_rgm_context, app_id, &p_app_info);

    /* Remove its data from the rgm_context 
     * this function will always return SUCCESS, because this
     * rgm_context definitely contains this app_id */
    if (ICF_TRUE == app_status) 
    {
        if(ICF_REG_REQ_EXPIRY_PRESENT == (p_recv_msg_payload->bit_mask & 
                    ICF_REG_REQ_EXPIRY_PRESENT))
        {
            p_app_info->expiry = p_recv_msg_payload->expiry;
        }
        /* To clear this application (and the user) related calls: */	
        icf_cfg_cm_call_clear_ind_wrt_app_user(
                p_crm_pdb,
                &(p_rgm_context->rgm_user_address),
                app_id,
                p_crm_pdb->p_ecode);

        p_rgm_context->rgm_duration = p_app_info->expiry;
        p_crm_pdb->p_rgm_context = p_rgm_context;

        if((0 == p_rgm_context->rgm_duration) &&
                (ICF_IS_BIT_SET(p_rgm_context->indicator,
                                ICF_RGM_NFY_ON_PROBATION)))
        {
            /* Stop the timer */
            icf_rgm_stop_timer(p_crm_pdb);

            /* rgm_ctx is on probation: set it to UNREGISTERED state*/
            p_rgm_context->rgm_state =
                ICF_RGM_02_UNREGISTERED;

        }
        else if (0 == p_rgm_context->rgm_duration) 
        {
#ifdef IMS_CLIENT
                icf_user_db_node_st *p_node = ICF_NULL;
#endif
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
                if(ICF_FAILURE == icf_dbm_get_public_uri_node_from_uri_for_line (p_crm_pdb->p_glb_pdb,
                    p_rgm_context->rgm_list_of_lines[0],
                    &(p_rgm_context->rgm_user_address),
                    ICF_USER_DB_USE_PUBLIC_URIS | ICF_USER_DB_USE_ASSOCIATED_URIS,
                    &p_node,
                    p_crm_pdb->p_ecode))
                {
                    ret_val =ICF_FAILURE;
                }
               /* If autoreg is on and this is not a temporary uri, refresh the registration */
                else if((p_crm_pdb->p_config_data->auto_register)
                    && !ICF_IS_BIT_SET(p_node->flags,ICF_USER_DB_PUBLIC_NODE_TEMPORARY))
                {
                    /* SPR 8944: begin fix */
                    /* If auto_register then default duration */
                    /* Initialize the rgm_duration with NULL */
                    p_rgm_context->rgm_duration = default_duration;
                    p_rgm_context->auto_reg_flag = ICF_TRUE;
                    /* SPR 8944: end fix */
                    
                    /* there were some valid application entries 
                    * to send a fresh REGISTER */
                    
                    ret_val = icf_rgm_register_user( p_crm_pdb,p_rgm_context);
                }
                else
                {
                    icf_rgm_stop_timer(p_crm_pdb);
                    
                    
                    /* None of the applications were valid entries */
                   icf_rgm_update_auth_info(p_crm_pdb);
                   /* To clear all the calls:  */
                   /* To send deregister on the network only
                    * if GPRS is not suspended */


                   ret_val = icf_rgm_deregister_user(
                           p_crm_pdb,p_rgm_context);
                   /*for CFG to not send response*/
                   p_crm_pdb->resp_to_be_sent = ICF_FALSE;
                }

ICF_CHECK_IMS_END
#endif
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
                    icf_rgm_stop_timer(p_crm_pdb);
                    
                    /* None of the applications were valid entries */
                    icf_rgm_update_auth_info(p_crm_pdb);    
                        /* To clear all the calls:  */
                        /*ret_val =  icf_cfg_cm_call_clear_ind(p_crm_pdb);*/
                        
                        
                        /* To send deregister on the network only
                        * if GPRS is not suspended */
                        ret_val = icf_rgm_deregister_user(
                            p_crm_pdb,p_rgm_context);
                        
                        
                        
                        /*for CFG to not send response*/
                        p_crm_pdb->resp_to_be_sent = ICF_FALSE;
ICF_CHECK_IMS_END
#endif

		/* SPR 8068; No need to indicate to the 
		 * CFG(Network manager) to deactivate */
		/* SPR: 8068: fix start */
                /*ret_val = icf_cfg_process_deactivate_on_dereg(p_crm_pdb);*/
		/* SPR: 8068: fix end */		
                               
            }
            else
            {
                /* there were some valid application entries 
                 * to send a fresh REGISTER */
                ret_val = icf_rgm_register_user( p_crm_pdb,p_rgm_context);
                
            }

    }/* app data found in rgm_context: ends */
    else
    {
        /* Application asked for DEREG: send CFM with FAILURE: 
         * as it is not even REGed now */
        register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;        
        register_response.status = ICF_FAILURE;
        register_response.error_cause = ICF_CAUSE_INVALID_REG_REQUEST;
        register_response.options_flag |= ICF_RGM_RESPONSE_CFM;        
        register_response.line_id = p_recv_msg_payload->line_id;
        register_response.p_crm_pdb = p_crm_pdb;

	/*for CFG to not send response*/

        p_crm_pdb->resp_to_be_sent = ICF_FALSE;
        ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                            &register_response, p_crm_pdb->p_ecode);
          
    }

    return ret_val;
    
}/* Function icf_rgm_handle_dereg() ends here */


 /*************************************************************************
 * FUNCTION:            icf_rgm_register_user
 *
 * DESCRIPTION:         This function is called by the functions of RGM in
 * 			 order to send registration requests.
 *
 * RETURNS:             
 *
 ************************************************************************/

icf_return_t icf_rgm_register_user(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_context_st *p_rgm_context)
{
    icf_result_t ret_val = ICF_SUCCESS;
    icf_uint8_t                      *p_msg_data_local;
    icf_rgm_register_resp_st    register_response;
    
    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t  *p_ecode;
    icf_boolean_t   send_reg_resp = ICF_FALSE;
    icf_line_id_t       line_id;
    icf_app_id_t        app_id;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    /* Get the global pdb pointer */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))

    /* Get pointer to ecode */    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);
    line_id = p_crm_pdb->line_id;   
    app_id = p_glb_pdb->p_recv_msg->hdr.app_id;
 
    icf_rgm_stop_timer(p_crm_pdb);

    /* Check whether NULL pointer is not passed */    
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
        
    /* Save p_msg_data */
    p_msg_data_local = p_crm_pdb->p_internal_msg->p_msg_data;

#ifdef ICF_NAT_RPORT_SUPPORT
    if((ICF_RGM_05_DEREGISTERING == p_rgm_context->rgm_state) &&
        (ICF_INITIAL_RPORT_ONGOING != p_rgm_context->rport_status))
    {
        /* Fill CRM PDB Structure */
        p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(p_rgm_context);
        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CLEAR_REG_OBJECT;
        p_crm_pdb->p_internal_msg->msg_hdr.payload_length =
            (icf_uint8_t)(ICF_PORT_SIZEOF(icf_rgm_context_st));
        /* Call SSA to clear Registration Call Object */
        ret_val = icf_ssa_process_mesg(
                (icf_internal_msg_st *)(p_crm_pdb->p_internal_msg));
        if(ICF_RGM_CLEAR_ONGOING_REGISTRATION ==
            (ICF_RGM_CLEAR_ONGOING_REGISTRATION &
                p_rgm_context->indicator))
        {
            /* Send deregister response to application
             */
            register_response.p_crm_pdb = p_crm_pdb;
            register_response.app_id = p_rgm_context->app_id;
            register_response.line_id = p_rgm_context->line_id;
            register_response.options_flag = ICF_NULL;
            register_response.options_flag |= ICF_RGM_FILL_URIS;
            register_response.options_flag |= ICF_RGM_CTXT_COPY;
            register_response.options_flag |= ICF_RGM_RESPONSE_IND;
            register_response.p_rgm_context = p_rgm_context;
            register_response.p_request_payload = ICF_NULL;
            register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
            register_response.status = ICF_SUCCESS;
            ret_val = icf_rgm_send_response(p_glb_pdb,
                    &register_response, p_ecode);
            p_rgm_context->indicator &= ~(ICF_RGM_CLEAR_ONGOING_REGISTRATION);
        }
    }
    else if(ICF_RGM_CLEAR_ONGOING_REGISTRATION ==
            (ICF_RGM_CLEAR_ONGOING_REGISTRATION &
                p_rgm_context->indicator))
    {
        send_reg_resp = ICF_TRUE;
    }

#else
    /* SPR 16019: Forcefully clear ongoing register transaction.
     */
    /* If state is De-Registering then abort the ongoing transaction
     * Free Call Object and then reset the bitmask for Clear Ongoing registr
     * ation if set. New register request will be sent on new SIP
     * Dialogue
     */
    if(ICF_RGM_05_DEREGISTERING == p_rgm_context->rgm_state)
    {
        /* Fill CRM PDB Structure */
        p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(p_rgm_context);
        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CLEAR_REG_OBJECT;
        p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
            (icf_uint16_t)(ICF_PORT_SIZEOF(icf_rgm_context_st));
        /* Call SSA to clear Registration Call Object */
        ret_val = icf_ssa_process_mesg(
                (icf_internal_msg_st *)(p_crm_pdb->p_internal_msg));
        if(ICF_RGM_CLEAR_ONGOING_REGISTRATION == 
            (ICF_RGM_CLEAR_ONGOING_REGISTRATION &
                p_rgm_context->indicator))
        {
            /* Send deregister response to application
             */
            register_response.p_crm_pdb = p_crm_pdb;
            register_response.app_id = p_rgm_context->app_id;
            register_response.line_id = p_rgm_context->line_id;
            register_response.options_flag = ICF_NULL;
            register_response.options_flag |= ICF_RGM_FILL_URIS;
            register_response.options_flag |= ICF_RGM_CTXT_COPY;
            register_response.options_flag |= ICF_RGM_RESPONSE_IND;
            register_response.p_rgm_context = p_rgm_context;
            register_response.p_request_payload = ICF_NULL;
            register_response.request_type = ICF_REQUEST_TYPE_DEREGISTER;
            register_response.status = ICF_SUCCESS;
            ret_val = icf_rgm_send_response(p_glb_pdb,
                    &register_response, p_ecode);
            p_rgm_context->indicator &= ~(ICF_RGM_CLEAR_ONGOING_REGISTRATION);
        }
    }
    else if(ICF_RGM_CLEAR_ONGOING_REGISTRATION == 
            (ICF_RGM_CLEAR_ONGOING_REGISTRATION &
                p_rgm_context->indicator))
    {
        send_reg_resp = ICF_TRUE;
    }
#endif
 
    /* Fill CRM PDB Structure */
    p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(p_rgm_context);
    p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_REG_REQ;
    p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
        (icf_uint16_t)(ICF_PORT_SIZEOF(icf_rgm_context_st));
/*    p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg = ICF_NULL;*/

/*    p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx = ICF_NULL;*/


#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)

    if(ICF_TRUE == icf_rgm_check_for_buffer_condition(p_crm_pdb))
    {
        p_rgm_context->indicator |= ICF_RGM_BUFFER_FOR_REGISTRATION;
        if(0 == p_rgm_context->buffer_id)
		   p_rgm_context->buffer_id = icf_rgm_get_next_buffer_id(p_crm_pdb);
        return ICF_SUCCESS;
    }

#ifdef ICF_IPSEC_ENABLED

    if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
    {
        if (ICF_NULL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params)
        {
            /* Invoke function to assign fresh security parameters */
            ret_val = icf_rgm_ipsec_get_sec_params(
                        p_crm_pdb, &(((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params));
        }
        /* else use the existing local security client parameters */
    } /* End if(ipsec_enabled) */
    
#endif /* ICF_IPSEC_ENABLED */
ICF_CHECK_IMS_END
#endif

    /* Call SSA */
    ret_val = icf_ssa_process_mesg(
            (icf_internal_msg_st *)(p_crm_pdb->p_internal_msg));
    
    /* Restore p_msg_data */
    p_crm_pdb->p_internal_msg->p_msg_data = p_msg_data_local;

    if(ret_val == ICF_SUCCESS)
    {
#ifdef ICF_LOAD_STAT
        p_crm_pdb->p_glb_pdb->load_data.register_req++;
#endif
/*        ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_SENDING_REG_REQUEST)*/

        /* Set State */

        if (p_rgm_context->indicator & ICF_RGM_AUTH_SET)
        {
/* SPR 8053 FIX START */
ICF_CRM_START
            if (ICF_RGM_01_REGISTERING == p_rgm_context->rgm_state ||
                    ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state)
            {
                p_rgm_context->rgm_state = ICF_RGM_04_AUTHORIZING;
                register_response.request_type = ICF_REQUEST_TYPE_AUTHORIZING;
            }
            else
            {
                p_rgm_context->rgm_state = ICF_RGM_01_REGISTERING;
                register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;

            }
ICF_CRM_END
/* SPR 8053 FIX END */
        }
        else
        {
            p_rgm_context->rgm_state = ICF_RGM_01_REGISTERING;
            register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
        }
        /* SPR 16019: Send response for transaction cleared forcefully
         */
        if(ICF_TRUE == send_reg_resp)
        {
            /* Form the register response and send it to App.
             * 
             */
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
            if(ICF_FAILURE == icf_rgm_send_app_resp_on_succ(p_crm_pdb,\
                        &register_response))
            {
                ret_val = ICF_FAILURE;
            }
            send_reg_resp = ICF_FALSE;
            p_rgm_context->app_id = app_id;
            p_rgm_context->line_id = line_id;
            p_rgm_context->p_app_data_list[app_id - 1].
                app_indicator &= ~(ICF_RGM_APP_CFM_SENT);

            /* Send configuration response to application if request is internally
             * generated
             */
        }
        if(ICF_RGM_INTERNAL_GENERATED_REG == (p_rgm_context->indicator & \
                    ICF_RGM_INTERNAL_GENERATED_REG))
        {
            p_crm_pdb->resp_to_be_sent = ICF_TRUE;
        }
/*SPR 20244 Start guard timer */
        icf_rgm_start_timer(
                p_glb_pdb,
                (icf_timer_type_t)(ICF_TIMER_TYPE_RGM_RETRY),
                p_rgm_context,
                p_ecode);

        /* For Trace Purposes Only */
        p_crm_pdb->p_rgm_context = p_rgm_context;
        
        ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_SENDING_REG_REQUEST)
        
        ICF_STAT(p_glb_pdb, ICF_STAT_NUM_OF_REG_REQ_SENT)
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
       ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->reg_ongoing = ICF_TRUE;
ICF_CHECK_IMS_END
#endif

    }
    
    else
    {
        /* Check whether PA was not indicated */
        if (!(p_rgm_context->indicator & ICF_RGM_PA_INDICATED))
        {
            /* PA was not indicated about registration failure */

            /* Send User Registered indication to PA */
            /* this api is not supported in icf */
            /*
            icf_rgm_send_pa_display_ind(
                    (icf_display_id_t)ICF_DISPLAY_USER_NOT_REGISTERED,
                    p_crm_pdb,
                    p_rgm_context);*/

            /* Set the PA_INDICATED bit */
            p_rgm_context->indicator |= ICF_RGM_PA_INDICATED;
        }
        register_response.request_type = ICF_NULL;
        /* For Trace Purposes Only */
        p_crm_pdb->p_rgm_context = p_rgm_context;
        ICF_RGM_SET_ECODE(p_crm_pdb, ICF_UNABLE_TO_SEND_REG_REQ)

		register_response.options_flag = ICF_NULL;
                register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
		register_response.options_flag |= ICF_RGM_CTXT_COPY;            
		register_response.status = ICF_FAILURE;
		register_response.error_cause = ICF_UNABLE_TO_SEND_REG_REQ;
		register_response.p_rgm_context = p_rgm_context;
		register_response.p_request_payload = ICF_NULL;
		register_response.p_crm_pdb = p_crm_pdb;
                register_response.app_id = p_rgm_context->app_id;
                register_response.line_id = p_rgm_context->line_id;
               if(ICF_NULL == register_response.request_type)
               {
                   register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
               }

		ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
										&register_response,
										p_crm_pdb->p_ecode);

#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)            
        p_rgm_context->rgm_state = ICF_RGM_02_UNREGISTERED;

        /* Start REATTEMPT_TIMER */
        ret_val = icf_rgm_start_timer(
                p_glb_pdb,
                (icf_timer_type_t)(ICF_TIMER_TYPE_RGM_REATTEMPT),
                p_rgm_context,
                p_ecode);

ICF_CHECK_IMS_END
#endif
	/* Fix for CSR 1-7423906: SPR 19772 */
    /* Clear the context irrespective of ret_val */
            icf_rgm_clear_reg_context(
                    p_crm_pdb,
                    p_rgm_context);
                        
        }
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_deregister_user
 *
 * DESCRIPTION:         This function is called by the functions of RGM in 
 * 			order to send de-registration requests.
 *
 * RETURNS:             FAILURE: SIC unable to send UN-SUBSCRIBE request
 *
 ***************************************************************************/
icf_return_t icf_rgm_deregister_user(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_context_st *p_rgm_context)
{
    icf_result_t ret_val = ICF_SUCCESS;
    icf_uint8_t                      *p_msg_data_local;

    /* Variables used for Code Area reduction */
    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t  *p_ecode;
    icf_boolean_t               send_reg_resp = ICF_FALSE;
    icf_rgm_register_resp_st	register_response;
    icf_line_id_t               line_id;
    icf_app_id_t                app_id;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        
    /* Get the global pdb pointer */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_glb_pdb))

    /* Get pointer to ecode */    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

    /* Check whether NULL pointer is not passed */    
    ICF_ASSERT(
            p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            (ICF_NULL != p_rgm_context))
    line_id = p_crm_pdb->line_id;    
    app_id = p_glb_pdb->p_recv_msg->hdr.app_id;

    /* get the RGM data pointer from DBM */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(p_glb_pdb,
        (icf_int_module_id_t)ICF_INT_MODULE_REGM,
        (icf_void_t *)&p_crm_pdb->p_rgm_glb_data,
        p_ecode))

    /* putting pointers in crm_pdb */
    p_crm_pdb->p_ecode = p_ecode;
    p_crm_pdb->p_glb_pdb = p_glb_pdb;
    p_crm_pdb->p_rgm_context = p_rgm_context;     
    /* Setting duration to Zero */
    p_rgm_context->rgm_duration = 0;
    /* Fix for CSR_1-8052488_SPR_20244. EGT shall stop refresh timer, if running.
  	 * If the application has deleted the user, then EGT shall send De-Reg on nw.
	 * In that, EGT does not stop refresh timer and start Guard timer for sending
	 * De-Register request. Hence, EGT shall stop refresh timer first and then start
	 * a new guard timer.
     */
	if ( p_rgm_context->timer_id != ICF_TIMER_ID_INVALID)
	{
		icf_rgm_stop_timer(p_crm_pdb);	
	}

    
    /* Save p_msg_data */
    p_msg_data_local = p_crm_pdb->p_internal_msg->p_msg_data;    
    
    /* Fill CRM PDB Structure */
    p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(p_rgm_context);
    p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_REG_REQ;
    p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
        (icf_uint16_t)(ICF_PORT_SIZEOF(icf_rgm_context_st));
/*    p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg = ICF_NULL;*/

/*    p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx = ICF_NULL;*/

    /*CSR 8052488 SPR 20244 While sending the deregistartion request , 
     *  reset the bitmask as it was set while sending the response for
     * for registration to application.
     * This bitmask will be used to ascertain which API (register cfm or register
     * status indication to be given to application on completion of de-registration
     * on network.*/

if(ICF_NULL == app_id)
    {
       p_rgm_context->p_app_data_list[app_id].app_indicator &=
                    ~ICF_RGM_APP_CFM_SENT;
    }
    else
    {
       p_rgm_context->p_app_data_list[app_id - 1].app_indicator &=
                    ~ICF_RGM_APP_CFM_SENT;
    }


#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)

    if(ICF_TRUE == icf_rgm_check_for_buffer_condition(p_crm_pdb))
    {
        p_rgm_context->indicator |= ICF_RGM_BUFFER_FOR_REGISTRATION;
		if(0 == p_rgm_context->buffer_id)
			p_rgm_context->buffer_id = icf_rgm_get_next_buffer_id(p_crm_pdb);
        return ICF_SUCCESS;
    }
ICF_CHECK_IMS_END
#endif
    /* SPR 16019: Check the state of registration, if REGISTERING/AUTHORIZING
     * then set the bitmask to clear the previous register transaction
     */
#ifdef ICF_NAT_RPORT_SUPPORT
    if((ICF_RGM_01_REGISTERING == p_rgm_context->rgm_state ||
           ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state) &&
        (ICF_INITIAL_RPORT_PENDING == p_rgm_context->rport_status))
    {
        p_rgm_context->indicator |= ICF_RGM_CLEAR_ONGOING_REGISTRATION;
        send_reg_resp = ICF_TRUE;
    }
#else
    if(ICF_RGM_01_REGISTERING == p_rgm_context->rgm_state ||
           ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state)
    {
        p_rgm_context->indicator |= ICF_RGM_CLEAR_ONGOING_REGISTRATION;
        if (ICF_RGM_04_AUTHORIZING == p_rgm_context->rgm_state)
        {
            register_response.request_type = ICF_REQUEST_TYPE_AUTHORIZING;
        }
        else if(ICF_RGM_01_REGISTERING == p_rgm_context->rgm_state)
        {
            register_response.request_type = ICF_REQUEST_TYPE_REGISTERING;
        }
        send_reg_resp = ICF_TRUE;
    }
#endif
    /* Call SSA */
    /*Fix for CSR 8070301 SPR 20236*/
    /*ssa should not be triggerred if regm is already in
    deregistering state*/
    if (ICF_RGM_05_DEREGISTERING != p_rgm_context->rgm_state)
    {
    ret_val = icf_ssa_process_mesg(p_crm_pdb->p_internal_msg);
    }
    else
    {
        ret_val = ICF_FAILURE; 
    }
    /* Restore p_msg_data */
    p_crm_pdb->p_internal_msg->p_msg_data = p_msg_data_local;
    
    if(ret_val == ICF_SUCCESS)
    {
/*        ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_SENDING_DEREG_REQUEST)*/
          ICF_RGM_APP_TRACE(p_crm_pdb, ICF_TRACE_DEREG_REQ)

        /* Set State */

        p_rgm_context->rgm_state = ICF_RGM_05_DEREGISTERING;
        /* SPR 16019: Send register response success for the cancelled
         * ongoing transaction
         */
        if(ICF_TRUE == send_reg_resp)
        {
            /* Form the register response and send it to App.
             * 
             */
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
            if(ICF_FAILURE == icf_rgm_send_app_resp_on_succ(p_crm_pdb,\
                        &register_response))
            {
                ret_val = ICF_FAILURE;
            }

            /* Fix for CSR 1-8052488: SPR 20224 */
   			/* While sending the deregistartion request , reset the bitmask as
     		 * it was set while sending the response for registration to 
			 * application. This bitmask will be used to ascertain which API 
			 * (register cfm or register status indication) to be given to 
			 * application on completion of de-registration on network.
			 * Note that for APP initiated dereg, CFM will be sent, whereas 
			 * for Internal initiated dereg, IND will be sent directly.*/

            if (ICF_NULL == app_id) 
    		{
       			p_rgm_context->p_app_data_list[app_id].app_indicator &=
                    ~ICF_RGM_APP_CFM_SENT;
    		}
    		else    
    		{
       			p_rgm_context->p_app_data_list[app_id - 1].app_indicator &=
                    ~ICF_RGM_APP_CFM_SENT;
    		}

            send_reg_resp = ICF_FALSE;
            p_rgm_context->app_id = app_id;
            p_rgm_context->line_id = line_id;
            /* Send configuration response to application if request is internally
             * generated
             */
        }
        if(ICF_RGM_INTERNAL_GENERATED_REG == (p_rgm_context->indicator & \
                    ICF_RGM_INTERNAL_GENERATED_REG))
        {
            p_crm_pdb->resp_to_be_sent = ICF_TRUE;
            
            /* Fix for CSR 1-8052488: SPR 20244 */
			/* For Internally initiated Dereg, INDICATION will be sent */

            if (ICF_NULL == app_id)
            {
                p_rgm_context->p_app_data_list[app_id].app_indicator |= 
					ICF_RGM_APP_CFM_SENT;
            }
            else
            {
                p_rgm_context->p_app_data_list[app_id - 1].app_indicator |=
                    ICF_RGM_APP_CFM_SENT;
        }

        }
/*start guard timer */
            icf_rgm_start_timer(
                    p_glb_pdb,
                    (icf_timer_type_t)(ICF_TIMER_TYPE_RGM_RETRY),
                    p_rgm_context,
                    p_ecode);

            /* For Trace Purposes Only */
            p_crm_pdb->p_rgm_context = p_rgm_context;        
            ICF_RGM_TRACE(p_crm_pdb, ICF_TRACE_SENDING_DEREG_REQUEST)        
            ICF_RGM_APP_TRACE(p_crm_pdb, ICF_TRACE_DEREG_REQ)
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
ICF_4_0_CRM_END
ICF_CHECK_IMS_END        
#endif
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->reg_ongoing = ICF_TRUE;
ICF_CHECK_IMS_END        
#endif
    }
    else
    {
        /* Send register_response failure
        */
        register_response.request_type = ICF_NULL;
        /* For Trace Purposes Only */
        p_crm_pdb->p_rgm_context = p_rgm_context;  
        ICF_RGM_SET_ECODE(p_crm_pdb, ICF_UNABLE_TO_SEND_REG_REQ)
		register_response.options_flag = ICF_NULL;
		register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
		register_response.options_flag |= ICF_RGM_CTXT_COPY;            
		register_response.status = ICF_FAILURE;
		register_response.error_cause = ICF_UNABLE_TO_SEND_REG_REQ;
		register_response.p_rgm_context = p_rgm_context;
		register_response.p_request_payload = ICF_NULL;
		register_response.p_crm_pdb = p_crm_pdb;
        register_response.app_id = p_rgm_context->app_id;
        register_response.line_id = p_rgm_context->line_id;
        if(ICF_NULL == register_response.request_type)
        {
            register_response.request_type = ICF_REQUEST_TYPE_DEREGISTERING;
        }
        ret_val = icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                &register_response,
                p_crm_pdb->p_ecode);      

        /* Clear the context */
        ret_val = icf_rgm_clear_reg_context(
                p_crm_pdb,
                p_rgm_context);
    }
    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
}


/****************************************************************************
 * FUNCTION:            icf_rgm_start_timer
 *
 * DESCRIPTION:         This function is called by the functions of RGM for 
 *                      creating timer structure and starting appropriate timer.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_start_timer(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_timer_type_t timer_type,
        INOUT icf_rgm_context_st *p_rgm_context,
        INOUT icf_error_t *p_ecode)
{
    icf_void_t *p_buff = ICF_NULL;
    icf_timer_data_st *p_timer_data = ICF_NULL;
    icf_duration_t duration= 0;
    icf_config_data_st *p_dbm_config_data = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
	icf_line_data_st *p_line_data	=	ICF_NULL;

#ifdef ICF_IPSEC_ENABLED
    /* current time in seconds */
    icf_time_t                        curr_time = ICF_NULL;
    /* duration in milliseconds for which timer has already been run */
    icf_duration_t                    elapsed_time = ICF_NULL;
    icf_rgm_glb_data_st               *p_rgm_glb_data = ICF_NULL;
#endif /* ICF_IPSEC_ENABLED */
    
    
    /* Get pointer to CFG's global data */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                            p_glb_pdb,
                            (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
                            (icf_void_t **)(&p_dbm_config_data),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;
    }

    switch (timer_type)
    {
        case ICF_TIMER_TYPE_RGM_REATTEMPT:
            /* reattempt is expected to be started
             * only in IMS_CLIENT scenario as AUTO_REG 
             * is supported in only IMS_CLIENT.*/
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
                duration = p_dbm_config_data->registration_retry_timer;
ICF_CHECK_IMS_END
#endif

        /* Hence for non IMS scene we simply return SUCCESS
         * without actually starting the timer */
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
				return ICF_SUCCESS;
ICF_CHECK_IMS_END
#endif
            break;
         
        case ICF_TIMER_TYPE_RGM_RETRY:
            /* SPR 20224 Set the duration for guard timer. This timer has the value of 120
               seconds */
            duration = ICF_REGM_GUARD_TIMER_DURATION;
        break;    

        case ICF_TIMER_TYPE_RGM_REGISTRATION: 
            duration = p_rgm_context->rgm_duration;

			/* Fetch line blk and subtract registion head start     
			 * duration from actual expiry duration */
     		if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
           		p_glb_pdb,p_rgm_context->rgm_list_of_lines[
                 p_rgm_context->num_of_lines -1],
            	(icf_line_data_st **)(&p_line_data),
           		 p_ecode))
			{
            	ret_val = ICF_FAILURE;
                ICF_FUNCTION_EXIT(p_glb_pdb)
                return ret_val;
     		}
 			else 
 			{
				if ((0 != p_line_data->reg_hs_duration) 
					 && (duration > p_line_data->reg_hs_duration))
    			{   
					duration -= p_line_data-> reg_hs_duration;
   				}
				else
				{
					duration = (icf_uint32_t)(duration/2);
				}
			}


#ifdef ICF_IPSEC_ENABLED
            /* get the RGM data pointer from DBM */
            if (ICF_FAILURE == icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_REGM,
                (icf_void_t *)&p_rgm_glb_data,
                p_ecode))
            {
                ret_val = ICF_FAILURE;
                ICF_FUNCTION_EXIT(p_glb_pdb)
                    return ret_val;
            }

          /* the duration will be even shorter in case some fraction of this 
          * timer had already been run in awaiting response from NM */
            if ((ICF_TRUE == p_dbm_config_data->ipsec_enabled) &&
                (ICF_NULL != p_rgm_glb_data->prev_timestamp))
            {
             /* the duration for REGM timer in this case will be equal to the
              * calculated duration minus the timer that has already been 
               * run */
    
              /* get the current system time in seconds */
                ICF_PORT_TIME(ICF_NULL, curr_time, ret_val)

                ICF_PRINT(("\n[REGM]: Returning timestamp = %ld seconds\n",
                    curr_time));
        
                if (ICF_SUCCESS == ret_val)
                {
                    /* get the interval for which timer was run */
                    elapsed_time = 
                        ((icf_duration_t)curr_time - 
                        p_rgm_glb_data->prev_timestamp)*1000;
        
                    ICF_PRINT(("\n[REGM]: REG timer already run for %ld seconds",
                        elapsed_time/1000));

                    /* subtract the interval for which timer was run */
                    duration -= elapsed_time;
        
                    /* reset the prev_timestamp in REGM context */
                    p_rgm_glb_data->prev_timestamp = ICF_NULL;
        
                }
                else
                {
                    ICF_FUNCTION_EXIT(p_glb_pdb)
                    return ret_val;
                }
            } /* End if(ipsec_enabled and prev_timestamp) */
#endif /* ICF_IPSEC_ENABLED */


            break;


#ifdef ICF_IPSEC_ENABLED

        case ICF_TIMER_TYPE_RGM_SA_CREATE:
            duration = p_dbm_config_data->p_ipsec_config_data->sa_create_timer;
            break;

#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
        case ICF_TIMER_TYPE_RGM_SA_MODIFY:
            duration = p_dbm_config_data->p_ipsec_config_data->sa_modify_timer;
            break;
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */

        case ICF_TIMER_TYPE_RGM_SA_DELETE:
            duration = p_dbm_config_data->p_ipsec_config_data->sa_delete_timer;
            break;

#endif /* ICF_IPSEC_ENABLED */

        default:
            ret_val = ICF_FAILURE;
            break;
    }
    
    if (ICF_SUCCESS == ret_val)
    {
        
        /* Allocate a timer block */
        ret_val = icf_dbm_get_mem_from_pool(p_glb_pdb,
                (icf_pool_id_t)(ICF_MEM_TIMER),
                &p_buff,
                p_ecode);

        /* Typecast */
        p_timer_data = (icf_timer_data_st *)(p_buff);
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_rgm_prepare_timer_struct(
                p_glb_pdb,
                timer_type,
                (icf_void_t *)p_rgm_context,
                p_timer_data);

            ICF_START_TIMER(p_glb_pdb,p_timer_data,p_rgm_context->timer_id,
                    duration,p_ecode,ret_val)
                
            /* Pointer should be kept in registration context */    
            if (ICF_SUCCESS == ret_val)
            {
                p_rgm_context->p_timer_buff = p_timer_data;
                
                /* debug traces */
                ICF_PRINT(((icf_uint8_t *)"\n[REGM]: Started timer-type = %d of"\
                    "timer-id = %ld and duration = %ld ms\n", \
                    timer_type, p_rgm_context->timer_id, duration));
            }
            
            else
            {
                /* Free timer block */
                icf_dbm_free_timer_block(
                        p_glb_pdb,
                        (icf_void_t *)(p_rgm_context->p_timer_buff),
                        p_ecode);    
            }
        }
    }
    return ret_val;
}


/****************************************************************************
 * FUNCTION:            icf_rgm_prepare_timer_struct
 *
 * DESCRIPTION:         This function is called by the functions of RGM for 
 *                      preparing timer structures.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_prepare_timer_struct(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN icf_timer_type_t timer_type,
        IN icf_void_t *p_timer_buffer,
        OUT icf_timer_data_st *p_timer_data)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    /* Fill Module ID */
    /* This is set to CFG because MRM checks for module CFG in this field
     * in case of timer expiry.
     */
    p_timer_data->module_id = ICF_INT_MODULE_CFG;

    /* Fill Timer Type */
    p_timer_data->timer_type = timer_type;

    /* Store Payload Pointer */
    p_timer_data->p_timer_buffer = (icf_void_t *)(p_timer_buffer);

    /* Store Global PDB pointer */
    p_timer_data->p_glb_pdb = p_glb_pdb;

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_rgm_start_expires_timer
 *
 * DESCRIPTION:         This function is called by the functions of RGM for 
 *                      creating timer structure and starting timer of the 
 *                      duration mentioned in Expires field of 200 OK to
 *                      registration request.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_start_expires_timer(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_duration_t duration,
        INOUT icf_rgm_context_st *p_rgm_context,
        INOUT icf_error_t *p_ecode)
{
    icf_void_t *p_buff = ICF_NULL;
    icf_timer_data_st *p_timer_data = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
	icf_line_data_st *p_line_data = ICF_NULL;	

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* this is commented out because the duration calculation is
     * different than IMS_CLIENT_2_0 */
    /*if (duration > ICF_RGM_DUR_TO_BE_SUB)
    {
        duration -= ICF_RGM_DUR_TO_BE_SUB;
    }*/

	/* Fetch line blk and subtract registion head start     
	 * duration from actual expiry duration */
    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
    	p_glb_pdb,p_rgm_context->rgm_list_of_lines[
        p_rgm_context->num_of_lines -1],
       	(icf_line_data_st **)(&p_line_data),
		p_ecode))
	{
      	ret_val = ICF_FAILURE;
		ICF_FUNCTION_EXIT(p_glb_pdb)
		return ret_val;
	}
	else 
	{
		if (0 != p_line_data->reg_hs_duration) 
        {
            if(p_line_data->reg_hs_duration < (0.5 * (duration)))
            {
                duration = (icf_uint32_t)(0.5 * (duration));       
            }  
            else if ((p_line_data->reg_hs_duration > (0.5 * (duration)))
                  && (p_line_data->reg_hs_duration < duration))
		     {   
			    duration -= p_line_data-> reg_hs_duration;
		     }
		    else
		     {
			    duration = (icf_uint32_t)(0.5 * duration);
		     }
         }
         else
         {
            duration = (icf_uint32_t)(0.5 * (duration));
         }
	}

    /* Allocate a timer block */
    ret_val = icf_dbm_get_mem_from_pool(p_glb_pdb,
            (icf_pool_id_t)(ICF_MEM_TIMER),
            &p_buff,
            p_ecode);

    /* Typecast */
    p_timer_data = (icf_timer_data_st *)(p_buff);
    
    if (ICF_SUCCESS == ret_val)
    {
        ret_val = icf_rgm_prepare_timer_struct(
                p_glb_pdb,
                (icf_timer_type_t)(ICF_TIMER_TYPE_RGM_REGISTRATION),
                (icf_void_t *)p_rgm_context,
                p_timer_data);

        ICF_START_TIMER(p_glb_pdb,p_timer_data,p_rgm_context->timer_id,
                duration,p_ecode,ret_val)

        /* Pointer should be kept in registration context */    
        if (ICF_SUCCESS == ret_val)
        {
            p_rgm_context->p_timer_buff = p_timer_data;
            /* debug traces */
            ICF_PRINT(((icf_uint8_t *)"\n[REGM]: Started timer-type = %d of"\
                "timer-id = %ld and duration = %ld ms\n", \
                ICF_TIMER_TYPE_RGM_REGISTRATION, p_rgm_context->timer_id, 
                duration));

        }

        else
        {
            /* Free timer block */
            icf_dbm_free_timer_block(
                    p_glb_pdb,
                    (icf_void_t *)(p_rgm_context->p_timer_buff),
                    p_ecode);    
        }
    }

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
}

/**************************************************************************
 * FUNCTION:            icf_rgm_clear_reg_context
 *
 * DESCRIPTION:         This function is called by the functions of RGM in
 * 			order to clear registration context.
 * RETURNS:             
 *
 *************************************************************************/

icf_return_t icf_rgm_clear_reg_context(
        INOUT icf_crm_pdb_st *p_crm_pdb,
        INOUT icf_rgm_context_st *p_rgm_context)
{
    icf_return_t ret_val;
    icf_glb_pdb_st *p_glb_pdb;
    icf_error_t  *p_ecode;

    icf_uint32_t            loop_index = ICF_NULL;
    icf_uint8_t            *p_temp_msg_data = ICF_NULL;
    icf_uint32_t            temp_msg_id = ICF_NULL;
    icf_uint32_t            temp_pay_len = ICF_NULL;
    icf_header_list_st     *p_hdr_list = ICF_NULL;
#ifdef IMS_CLIENT
	icf_user_db_node_st		*p_user_db = ICF_NULL;
        icf_line_id_t			j=1;
#endif

/*  Fix for spr 18493 */
#ifdef ICF_NAT_RPORT_SUPPORT
    icf_line_data_st    *p_line_data = ICF_NULL;
    icf_rport_config_data_st *p_rport_config_data = ICF_NULL;
#endif

	if ( p_rgm_context->timer_id != ICF_TIMER_ID_INVALID)
	{
		icf_rgm_stop_timer(p_crm_pdb);	
	}

    /* Get the global pdb pointer */
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    /* Get pointer to ecode */    
    p_ecode = &(p_crm_pdb->p_internal_msg->msg_hdr.ecode);

ICF_CRM_START
    /* Copy the current msg_data locally */
    if (ICF_NULL != p_crm_pdb->p_internal_msg->p_msg_data)
    {
        p_temp_msg_data = p_crm_pdb->p_internal_msg->p_msg_data;
        temp_msg_id = p_crm_pdb->p_internal_msg->msg_hdr.msg_id;
        temp_pay_len = p_crm_pdb->p_internal_msg->msg_hdr.payload_length;
    }
ICF_CRM_END


    /* Fill CRM PDB Structure */
    p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)(p_rgm_context);
    p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CLEAR_REG_OBJECT;
    p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
        (icf_uint16_t)(ICF_PORT_SIZEOF(icf_rgm_context_st));
/* Start: Multiple Public Users ICF5.0*/
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
	for(j=0;j<p_rgm_context->num_of_lines;j++)
	{
		icf_dbm_get_public_uri_node_from_uri_for_line (
			p_crm_pdb->p_glb_pdb,
			p_rgm_context->rgm_list_of_lines[j],
			&(p_rgm_context->rgm_user_address),
			ICF_USER_DB_USE_PUBLIC_URIS,
			&p_user_db,
			p_crm_pdb->p_ecode);
/* If user was added by the Register API then it has to be removed upon deregister */
		if(p_user_db->flags & ICF_USER_DB_PUBLIC_NODE_TEMPORARY)
		{
			icf_dbm_delete_public_uri_for_line(
				p_crm_pdb->p_glb_pdb,
				p_rgm_context->rgm_list_of_lines[j], 
				&(p_user_db->public_user),
				p_crm_pdb->p_ecode);
		}
	}
	j=1;
	icf_cfg_cm_call_clear_ind_wrt_user(p_crm_pdb, 
		&(p_rgm_context->rgm_user_address), 
		ICF_USE_LINE1,
		(icf_boolean_t)ICF_FALSE,
		p_ecode);
ICF_CHECK_IMS_END
#endif
/* End: Multiple Public Users ICF5.0*/
    
    /* Call SSA to clear Registration Call Object */
    ret_val = icf_ssa_process_mesg(
            (icf_internal_msg_st *)(p_crm_pdb->p_internal_msg));

    /* To free the individual members of the app_data_list */

/*  Fix for SPR 18493 */
/*  If the rgm_context is getting deallocated then
    there is no need of continuing the NAT Timer */ 
#ifdef ICF_NAT_RPORT_SUPPORT
    /* reset the rport status to INITIAL */
    p_rgm_context->rport_status =  ICF_INITIAL_RPORT_INITIAL;

   /*reset the ongoing dereg counter */
   p_rgm_context->ong_dereg_counter = ICF_NULL;

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
    
    if (ICF_NULL == p_rport_config_data)
    {
        return ICF_FAILURE;
    }

    if (ICF_NULL != p_rport_config_data->p_binding_refresh_timer)
    {
        /*  Stop the timer if the rgm_context is getting cleared for the user 
            for which the NAT timer is running */
        if (p_rport_config_data->reg_id == p_rgm_context->reg_id)
        {
            ICF_STOP_TIMER(p_glb_pdb,
                           ((icf_timer_data_st *)(p_rport_config_data->\
                             p_binding_refresh_timer))->timer_id,
                           p_ecode,
                           ret_val)
            ret_val = icf_dbm_free_timer_block(
                                   p_glb_pdb,
                                   (icf_void_t *)(p_rport_config_data->\
                                       p_binding_refresh_timer),
                                   p_ecode);
        
            p_rport_config_data->p_binding_refresh_timer = ICF_NULL;
        }
    }
#endif
     
    /* Re-Initialize the members of this rgn_context with
     * default values */
     p_rgm_context->indicator = ICF_NULL;    
     p_rgm_context->rgm_duration = ICF_NULL;
     p_rgm_context->rgm_state = ICF_RGM_00_IDLE;
     p_rgm_context->num_of_lines = ICF_NULL;
     p_rgm_context->p_ssa_ctxt = ICF_NULL;
     p_rgm_context->p_timer_buff = ICF_NULL;
     
     
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)

     p_rgm_context->retry_count = ICF_NULL;

      /* From IMS rel 2.0 */
     p_rgm_context->p_srv_rte = ICF_NULL;
ICF_CHECK_IMS_END
#endif

     ICF_ASSERT(p_glb_pdb, 
            (ICF_NULL != p_crm_pdb->p_config_data))

ICF_CRM_START
    /* Copy the old msg_data back */
    if (ICF_NULL != p_temp_msg_data)
    {
        p_crm_pdb->p_internal_msg->p_msg_data = p_temp_msg_data;
        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = (icf_uint16_t)temp_msg_id;
        p_crm_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)temp_pay_len;
    }
ICF_CRM_END
   
     for (loop_index = 0; loop_index < 
		     p_crm_pdb->p_config_data->max_applications;
                            loop_index++)
    {
        /*Fix for CSR_1-8052488*/
        /* SPR 20244: Fix for Memory in use in application specific headers*/
         
        if (ICF_TRUE == p_rgm_context->p_app_data_list[loop_index].is_valid)
        {
            p_hdr_list = &(p_rgm_context->p_app_data_list[loop_index].inc_header_list);
            if( p_hdr_list->count)
            {
                /*free the list*/
                icf_cmn_free_header_list(p_crm_pdb->p_glb_pdb,
                        p_hdr_list,
                        p_crm_pdb->p_ecode);
            }
        }
        
        /* Make the application entries ready for Re-use */
        p_rgm_context->p_app_data_list[loop_index].
                            is_valid = ICF_FALSE;
        p_rgm_context->p_app_data_list[loop_index].
                            app_indicator = ICF_NULL;
        p_rgm_context->p_app_data_list[loop_index].
                            bitmask = ICF_NULL;
        
        /* Free the lists if present */
        if (ICF_NULL != p_rgm_context->
                                    p_app_data_list[loop_index].
				    header_list.count)
        {
            /* free the header list */
            ret_val = icf_cmn_free_header_list(
                                p_glb_pdb,
                                &(p_rgm_context->p_app_data_list[
                                        loop_index].header_list),
                                p_ecode);
            p_rgm_context->p_app_data_list[loop_index].
                        header_list.count = ICF_NULL;
        }
        
        /* Free the lists if present */
        if (ICF_NULL != p_rgm_context->
                                    p_app_data_list[loop_index].
				    tag_list.count)
        {
                        
            /* free the tag list */
            ret_val = icf_cmn_free_tag_list(
                                p_glb_pdb,
                                &(p_rgm_context->p_app_data_list[
                                        loop_index].tag_list),
                                p_ecode);
            p_rgm_context->p_app_data_list[loop_index].
                        tag_list.count = ICF_NULL;
        }

        /* Free the lists if present */
        if (ICF_NULL != p_rgm_context->
                                    p_app_data_list[loop_index].
				    body_list.count)
        {
            /* free the body list */
            ret_val = icf_cmn_free_msg_body_list(
                                p_glb_pdb,
                                &(p_rgm_context->p_app_data_list[
                                            loop_index].body_list),
                                p_ecode);
            p_rgm_context->p_app_data_list[loop_index].
                        body_list.count = ICF_NULL;
        }

        ret_val = ICF_SUCCESS;


    }/* For loop: app_data_list: Processing Ends */

            

    if (ICF_NULL != p_rgm_context->p_contact_uri_list)
	{
		/* free the contact list */
		icf_dbm_delete_address_list(p_glb_pdb,
			&(p_rgm_context->p_contact_uri_list),
			p_ecode);
		p_rgm_context->p_contact_uri_list = ICF_NULL;
	}

	/* Fix for CSR 1-8047728: SPR 20246 */
    /* clear contents of auth_data_sent and recd */
    ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
		p_crm_pdb->p_rgm_context->auth_data_sent.p_nonce,
        ICF_MEM_COMMON,
        p_crm_pdb->p_ecode,
		ret_val)
        
    ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
        p_crm_pdb->p_rgm_context->auth_data_recd.p_nonce,
        ICF_MEM_COMMON,
        p_crm_pdb->p_ecode,
		ret_val)
	
#ifdef ICF_TRACE_ENABLE
    if(ICF_NULL != p_crm_pdb->p_rgm_context)
    {

        if (ICF_NULL != p_crm_pdb->p_rgm_context->p_trace_criteria_data)
        {
           ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_crm_pdb->p_rgm_context->p_trace_criteria_data,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
    }
#endif

    /* Call DBM to free Registration block */ 
    if (ICF_FAILURE == icf_dbm_dealloc_rgn_blk(
				p_glb_pdb,
				p_rgm_context,
				p_ecode))
	{
#ifdef ICF_SIGCOMP_SUPPORT
#ifdef ICF_IPSEC_ENABLED
		if (ICF_TRUE == p_crm_pdb->p_config_data->sigcomp_required)
		{
			if (ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled)
			{
				icf_rgm_send_delete_comp_ind_to_sigsa(p_crm_pdb);
			}
		}
#endif
#endif
	}
    ICF_PRINT(((icf_uint8_t *)"\n[REGM]: Registration Context Deallocated\n"));
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
	/* Clear the SIC ctxt if this is the last reg_context being cleared */
	if(ICF_FAILURE == icf_dbm_get_used_rgn_blk(
                p_crm_pdb->p_glb_pdb,
                (icf_uint16_t)j,
                &p_rgm_context,
                p_crm_pdb->p_ecode))
	{
		    ret_val = icf_rgm_send_clear_subs_on_dereg(
                p_crm_pdb,
                p_ecode);
	}
ICF_CHECK_IMS_END
#endif

    return ret_val;
}


/****************************************************************************
 * FUNCTION:            icf_rgm_auth_info_present
 *
 * DESCRIPTION:         This function is called by icf_rgm_01_auth_req_resp
 *                      function of FSM to find whether all authentication
 *                      information for this block is present on some line
 *                      and to place that line at a proper place in the 
 *                      registration block. Line having all Authentication 
 *                      information has to be placed at the end of 
 *                      the registration block because SSA will pick the last
 *                      line in case Authentication is required. 
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_auth_info_present(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    
    icf_int16_t loop_index;
    icf_int16_t loop_index1;
    icf_line_data_st *p_dbm_line_data = ICF_NULL;
    icf_boolean_t auth_info_present_flag = ICF_FALSE;
    icf_line_id_t line_id;
    icf_result_t ret_val;
    icf_rgm_context_st    *p_rgm_context = ICF_NULL;

    p_rgm_context = p_crm_pdb->p_rgm_context;

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_rgm_context))

    
    for (loop_index = (icf_int16_t)(p_rgm_context->num_of_lines - 1); loop_index >= 0;
            loop_index--)
    {
            /* Get pointer to line block */
        ret_val = icf_dbm_fetch_line_cntxt_blk(
                p_crm_pdb->p_glb_pdb,
                p_rgm_context->rgm_list_of_lines[loop_index],       
                &(p_dbm_line_data),
                p_crm_pdb->p_ecode);
        
        if (ICF_SUCCESS == ret_val)
        {
ICF_4_0_CRM_START

            if (p_crm_pdb->p_config_data->auth_type
                                == ICF_NONIMS_AUTH_OPTION)
            {

            /* Check for authentication key and password */
                if (ICF_SUCCESS == icf_cmn_check_if_auth_params_present(
                            p_crm_pdb->p_glb_pdb,
                            &p_rgm_context->rgm_user_address,
                            p_rgm_context->rgm_list_of_lines[loop_index]))
            {
                auth_info_present_flag = ICF_TRUE;
                break;
            }
            }/* NON IMS option */
            else
            {
            /* For REL 4.0: the line data is no more responsible 
             * for holding the auth related info */
            if (p_dbm_line_data->line_data_received & 
                        ICF_CFG_LINE_DATA_AUTH_KEY_RECD)
                
            {
                auth_info_present_flag = ICF_TRUE;
                break;
            }
            }/* IMS option */
            

ICF_4_0_CRM_END

        }
    }/* End of for loop */
    
    if (ICF_TRUE == auth_info_present_flag)
    {
        if (loop_index == (p_rgm_context->num_of_lines - 1))
        {
            /* Line is already in last position */
            ret_val = ICF_SUCCESS;
        }
        else
        {
            /* Line id is not present in the end */
            line_id = p_rgm_context->rgm_list_of_lines[loop_index];
            for (loop_index1 = loop_index; 
                    loop_index1 < (p_rgm_context->num_of_lines - 1);
                    loop_index1++)
            {
                p_rgm_context->rgm_list_of_lines[loop_index1] = 
                    p_rgm_context->rgm_list_of_lines[loop_index1 + 1];
            }
            /* Put line ID in the last place */
            p_rgm_context->rgm_list_of_lines[loop_index1] = line_id;
            ret_val = ICF_SUCCESS;
        }
    }
    else
    {
       /* auth info was not here set the bit-mask for error in crm pdb */
       p_crm_pdb->error_cause = ICF_AUTHENTICATION_INFO_ABSENT;
        ret_val = ICF_FAILURE;
    }
    return ret_val;

}/* Function icf_rgm_auth_info_present() ends here */



#ifdef ICF_TRACE_ENABLE
/**********************************************************************
 *
 * FUNCTION:        icf_rgm_trace
 *
 * DESCRIPTION:     This function is used by RGM for trace handling.
 *
 *********************************************************************/

icf_void_t icf_rgm_trace(
        INOUT icf_crm_pdb_st *p_crm_pdb, 
        IN    icf_trace_id_t trace_id)
{
    icf_trace_data_st trace_data;

#ifndef ICF_REGM_UT_ENABLE
    icf_uint8_t       trace_index;
#ifdef IMS_CLIENT
    icf_line_data_st *p_dbm_line_data = ICF_NULL; 
#endif
#endif
    
    switch(trace_id)
    {
        case ICF_TRACE_SENDING_REG_REQUEST:
        case ICF_TRACE_SENDING_DEREG_REQUEST:
        case ICF_TRACE_DEREG_SUCCESS_RESP:
        case ICF_TRACE_REG_SUCCESS_RESP:
        case ICF_TRACE_DEREG_FAIL_NO_REATT_RESP:
        case ICF_TRACE_REG_FAIL_NO_REATT_RESP:
        case ICF_TRACE_AUTHENTICATION_REQ:
        case ICF_TRACE_DURATION_SHORT:
        case ICF_TRACE_RETRY_TIMER_EXPIRY:
        case ICF_TRACE_REG_TIMER_EXPIRY:
        case ICF_TRACE_REG_REATT_TIMER_EXPIRY:

#ifdef ICF_REGM_UT_ENABLE
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
#endif            
      
#ifndef  ICF_REGM_UT_ENABLE           
            /* For Printing Line Id's */
            for (trace_index = 0; (trace_index < p_crm_pdb->p_rgm_context->
                    num_of_lines) && (trace_index < 4); trace_index++)
            {
                trace_data.int_val[trace_index] =
                    (icf_uint32_t)(p_crm_pdb->p_rgm_context->
                                      rgm_list_of_lines[trace_index]);
            }
            trace_data.num_of_integer_values = trace_index;
#endif
            
            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            
            /* For Printing User Address */
            /*icf_port_strcpy(trace_data.string_val[0].str,
                    p_crm_pdb->p_rgm_context->rgm_user_address.addr_val.str);*/
ICF_CRM_START
#ifdef IMS_CLIENT

            icf_port_strcpy(trace_data.string_val[0].str,
                    p_crm_pdb->p_rgm_context->rgm_user_address.addr_val.str);
ICF_CRM_END

            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            
            /* For Printing Registrar Address */
            /* Get pointer to line block */
            /*icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                    p_crm_pdb->p_rgm_context->rgm_list_of_lines[0],
                    &p_dbm_line_data,
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));*/
ICF_CRM_START
            icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb,
                    p_crm_pdb->p_rgm_context->rgm_list_of_lines[
                            p_crm_pdb->p_rgm_context->num_of_lines - 1],
                    &p_dbm_line_data,
                    p_crm_pdb->p_ecode);
ICF_CRM_END

            if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
                    p_dbm_line_data->registrar_addr.addr.addr_type)
            {
                icf_port_strcpy(trace_data.string_val[1].str,
                    p_dbm_line_data->registrar_addr.addr.addr.domain.str);                
            }
            else if (ICF_TRANSPORT_ADDRESS_IPV4_ADDR == 
                    p_dbm_line_data->registrar_addr.addr.addr_type)
            {
                ICF_SPRINTF(((icf_int8_t *)(trace_data.string_val[1].str),
                            "%d%c%d%c%d%c%d",
                            p_dbm_line_data->registrar_addr.addr.addr.ipv4_addr.
                            octet_1,
                            '.',
                            p_dbm_line_data->registrar_addr.addr.addr.ipv4_addr.
                            octet_2,
                            '.',
                            p_dbm_line_data->registrar_addr.addr.addr.ipv4_addr.
                            octet_3,
                            '.',
                            p_dbm_line_data->registrar_addr.addr.addr.ipv4_addr.
                            octet_4))
            }
#ifdef ICF_IPV6_ENABLED
            else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == 
                p_dbm_line_data->registrar_addr.addr.addr_type){
                icf_port_strcpy(trace_data.string_val[1].str,
                        p_dbm_line_data->registrar_addr.addr.addr.
				ipv6_addr.str);                
            }
#endif
            trace_data.string_val[1].str_len =
                icf_port_strlen(trace_data.string_val[1].str);
            break;
#endif

                         
        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
            
    } /* end of switch-case for trace_id */
    
    /* Finally, invoke the common macro for trace handling */
    ICF_TRACE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb, 
            trace_id, &trace_data)
} /* end of icf_rgm_trace() */
/**********************************************************************
 *
 * FUNCTION:        icf_rgm_app_trace
 *
 * DESCRIPTION:     This function is used by RGM for trace handling.
 *
 *********************************************************************/
 icf_return_t icf_rgm_app_trace(
         INOUT icf_crm_pdb_st *p_crm_pdb,
         IN    icf_trace_id_t trace_id)
 {
     icf_trace_data_st             trace_data = {{0,0},0,{0},0,{{0,"\0"}},{0,"\0"},0}; 
     icf_call_id_t   call_id = ICF_INVALID_CALL_ID;
     icf_return_t    ret_val = ICF_SUCCESS;
     icf_trace_criteria_data_st    *p_criteria_data = ICF_NULL;
     icf_line_data_st        *p_line_data = ICF_NULL;
	/* Fix for CSR 1-7423906: SPR 19772 */
     icf_uint8_t              registrar_addr[ICF_MAX_STR_LEN] = {0};
     if(ICF_NULL == p_crm_pdb->p_rgm_context)
     {
         return ICF_SUCCESS;
     }
     if(ICF_NULL == p_crm_pdb->p_rgm_context->p_trace_criteria_data)
     {
        ICF_MEMGET(p_crm_pdb->p_glb_pdb,
          ICF_PORT_SIZEOF(icf_trace_criteria_data_st),
          ICF_MEM_COMMON,
          p_criteria_data,
          ICF_RET_ON_FAILURE, p_crm_pdb->p_ecode, ret_val)
        p_crm_pdb->p_rgm_context->p_trace_criteria_data = p_criteria_data;   
     }
     else
     {
       p_criteria_data = p_crm_pdb->p_rgm_context->p_trace_criteria_data;
     }
     p_crm_pdb->p_glb_pdb->p_glb_data->app_trace_info.call_id =
                call_id;
     /*fetch the registrar address */           
     if (ICF_FAILURE != icf_dbm_fetch_line_cntxt_blk(
                 p_crm_pdb->p_glb_pdb,
                 p_crm_pdb->p_rgm_context->rgm_list_of_lines[0],
                 &p_line_data, p_crm_pdb->p_ecode))
      {
          if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
               p_line_data->registrar_addr.addr.addr_type)
           {
				/* Fix for CSR 1-7423906: SPR 19772 */
				/* Replace strcpy by strncpy */
               icf_port_strncpy(
                	(icf_int8_t *)registrar_addr,
                	(icf_int8_t *)
							(p_line_data->registrar_addr.addr.addr.domain.str), 
					(ICF_MAX_STR_LEN - 1));
				registrar_addr[ICF_MAX_STR_LEN - 1] = '\0';
           }
          else if (ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                  p_line_data->registrar_addr.addr.addr_type)
           {
              icf_port_sprintf((icf_int8_t *)registrar_addr,"%d.%d.%d.%d:%d",
                   p_line_data->registrar_addr.addr.addr.ipv4_addr.octet_1,
                   p_line_data->registrar_addr.addr.addr.ipv4_addr.octet_2,
                   p_line_data->registrar_addr.addr.addr.ipv4_addr.octet_3,
                   p_line_data->registrar_addr.addr.addr.ipv4_addr.octet_4,
                   p_line_data->registrar_addr.port_num);
           }
      }
      switch(trace_id)
      {
          case ICF_TRACE_REGISTARTION_REQ:
                 p_crm_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                              p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;
                 trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
                 trace_data.int_val[0] =
                       ((icf_register_req_st *)(p_crm_pdb->p_recv_payload))->line_id;
                 trace_data.int_val[1] =
                       ((icf_register_req_st *)(p_crm_pdb->p_recv_payload))->expiry;

                 trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
                 icf_port_strcpy(trace_data.string_val[0].str,registrar_addr);
                 trace_data.string_val[0].str_len =
                       (icf_uint16_t )icf_port_strlen(trace_data.string_val[0].str);

                 icf_port_strcpy(trace_data.string_val[1].str,
                       ((icf_register_req_st *)(p_crm_pdb->p_recv_payload))->
                                to.addr_val.str);
                 trace_data.string_val[1].str_len =
                       (icf_uint16_t ) icf_port_strlen(trace_data.string_val[1].str);

                 p_criteria_data->line_id =
                      ((icf_register_req_st *)(p_crm_pdb->p_recv_payload))->line_id;
                 p_criteria_data->bitmask |= ICF_TRACE_LINE_ID_PRESENT;

                 p_criteria_data->p_calling_party_address =
                      &(p_crm_pdb->p_rgm_context->rgm_user_address);
                 p_criteria_data->bitmask |= ICF_TRACE_CALLING_PARTY_ADDR_PRESENT;

                 break;

         case ICF_TRACE_DEREG_REQ:
                 if((ICF_NULL != p_crm_pdb->p_glb_pdb->p_recv_msg) &&
                      (ICF_GENERIC_APP_ID != p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id))
                  {
                     p_crm_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
                            p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;
                  }
                  trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
                  trace_data.int_val[0] =
                        (icf_uint32_t)p_crm_pdb->line_id;
                  trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
                  icf_port_strcpy(trace_data.string_val[0].str,registrar_addr);
                       trace_data.string_val[0].str_len =
                             (icf_uint16_t ) icf_port_strlen(trace_data.string_val[0].str);
                  if(ICF_NULL != p_crm_pdb->p_recv_payload)
                  {
                      icf_port_strcpy(trace_data.string_val[1].str,
                             ((icf_register_req_st *)(p_crm_pdb->p_recv_payload))->
                             to.addr_val.str);
                      trace_data.string_val[1].str_len =
                           (icf_uint16_t ) icf_port_strlen(trace_data.string_val[1].str);
                  }
                  else
                  {
                       trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                  }
                  break;
         case ICF_TRACE_REREG_REQ:
                 if(ICF_NULL != p_crm_pdb->p_rgm_context)
                 {
                     icf_uint8_t loop_index;
                     for (loop_index = 0; (loop_index < 
                              p_crm_pdb->p_config_data->max_applications); loop_index++)
                     {
                         if (ICF_FALSE == p_crm_pdb->p_rgm_context->
                                 p_app_data_list[loop_index].is_valid)
                          {
                              continue;
                          }
                         else
                          {
                              p_crm_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id
                                = p_crm_pdb->p_rgm_context->p_app_data_list[loop_index].app_id;
                              break;  
                          }
                     }
                 }
                 trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
                 trace_data.int_val[0] =
                    (icf_uint32_t)p_crm_pdb->line_id;
                 trace_data.int_val[1] =
                    (icf_uint32_t)p_crm_pdb->p_rgm_context->rgm_duration;
                 trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
                 icf_port_strcpy(trace_data.string_val[0].str,registrar_addr);
                 trace_data.string_val[0].str_len =
                     (icf_uint16_t ) icf_port_strlen(trace_data.string_val[0].str);
                 icf_port_strcpy(trace_data.string_val[1].str,
                        (p_crm_pdb->p_rgm_context->rgm_user_address.addr_val.str));
                 trace_data.string_val[1].str_len =
                     (icf_uint16_t ) icf_port_strlen(trace_data.string_val[1].str);
                 break;

       case ICF_TRACE_REG_RESP:
                 if(ICF_NULL != p_crm_pdb->p_rgm_context && 
                    ICF_NULL != p_crm_pdb->p_rgm_context->p_app_data_list)
                 {
                     icf_uint8_t loop_index;
                     for (loop_index = 0; (loop_index <
                          p_crm_pdb->p_config_data->max_applications); loop_index++)
                     {
                        if (ICF_FALSE == p_crm_pdb->p_rgm_context->
                                p_app_data_list[loop_index].is_valid)
                         {
                             continue;
                         }
                        else
                         {
                             p_crm_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id
                             = p_crm_pdb->p_rgm_context->p_app_data_list[loop_index].app_id;
                             break;
                         }
                     }
                 }
                 trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
                 trace_data.int_val[0] =
                       (icf_uint32_t)p_crm_pdb->line_id;
                 trace_data.int_val[1] =
                       (icf_uint32_t)p_crm_pdb->result;
                 trace_data.int_val[2] = ((p_crm_pdb->p_rgm_context->rgm_duration)/1000);
                 trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
                 break;

    case ICF_TRACE_REG_TIMER_EXPIRE:
                 if(ICF_NULL != p_crm_pdb->p_rgm_context &&
                    ICF_NULL != p_crm_pdb->p_rgm_context->p_app_data_list)
                 {
                     icf_uint8_t loop_index;
                     for (loop_index = 0; (loop_index <
                           p_crm_pdb->p_config_data->max_applications); loop_index++)
                     {
                         if (ICF_FALSE == p_crm_pdb->p_rgm_context->
                                 p_app_data_list[loop_index].is_valid)
                         {
                            continue;
                         }
                         else
                         {
                            p_crm_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id
                            = p_crm_pdb->p_rgm_context->p_app_data_list[loop_index].app_id;
                            break;
                         }
                     }
                 }
                 trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
                 trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
                 break;
      default:
             trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
             trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
             return ICF_SUCCESS;
      }/* end of switch-case for trace_id */
      /* Fix for CSR-1-5249829 */
      ret_val = icf_cmn_check_app_trace(p_crm_pdb->p_glb_pdb,
                                        trace_id,
					p_criteria_data);
      if( ICF_FAILURE == ret_val)
      {
          return ret_val;
      }
      /* Finally, invoke the common macro for trace handling */
      ICF_APP_TRACE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
              trace_id, &trace_data, p_criteria_data)
      return ret_val;        
 }/* end of icf_rgm_app_trace() */

#endif


#ifdef ICF_ERROR_ENABLE
/*************************************************************************
 *
 * FUNCTION:        icf_rgm_error
 * 
 * DESCRIPTION:     This function is invoked by the macro ICF_RGM_SET_ECODE. 
 *                  It fills in the error data on the basis of ecode id and 
 *                  calls the macro ICF_SET_ECODE to report the error.
 *                   
 * RETURNS:         Nothing.       
 *
 *************************************************************************/

icf_void_t icf_rgm_error(
        INOUT icf_crm_pdb_st            *p_crm_pdb,
        INOUT    icf_error_t               ecode)
{
    icf_error_data_st                error_data;

#ifdef IMS_CLIENT
    icf_uint8_t       error_index;
    icf_line_data_st *p_dbm_line_data = ICF_NULL;    
    icf_boolean_t        rgm_ctxt_null = ICF_TRUE;
#endif
    icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
    
    p_glb_pdb = p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

#ifdef IMS_CLIENT/* 1 */
ICF_CRM_START
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    if (ICF_NULL != p_crm_pdb->p_rgm_context)
    {
        rgm_ctxt_null = ICF_FALSE; 
    }
ICF_CHECK_IMS_END
ICF_CRM_END
#endif/* IMS_CLIENT : 1 */
   
    switch (ecode)
    {
        case ICF_UNABLE_TO_SEND_REG_REQ:
        case ICF_UNABLE_TO_SEND_DEREG_REQ:

/*#ifdef ICF_REGM_UT_ENABLE  
            error_data.num_of_integer_values = ICF_NO_INT_PRESENT;

            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
#endif

#ifndef  ICF_REGM_UT_ENABLE           */ /*for ICF*/


            /* For Printing Line Id's */

#ifdef IMS_CLIENT/* 2*/
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
        if (ICF_TRUE == rgm_ctxt_null)
        {
            error_data.num_of_integer_values = ICF_NULL;
        }
        else
        {
            for (error_index = 0; (error_index < p_crm_pdb->p_rgm_context->
                num_of_lines) && (error_index < 4); error_index++)
            {
                error_data.int_val[error_index] =
                (icf_uint32_t)(p_crm_pdb->p_rgm_context->
                rgm_list_of_lines[error_index]);
            }
            error_data.num_of_integer_values = error_index;
        }
ICF_CHECK_IMS_END
#endif/* IMS_CLIENT : 2 */

    error_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            
            /* For Printing User Address */

#ifdef IMS_CLIENT /* 3 */
ICF_CRM_START
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
       if (ICF_TRUE == rgm_ctxt_null)
        {
            if (ICF_REG_REQ_TO_PRESENT & 
                ((icf_register_req_st *)
                (p_crm_pdb->p_recv_payload))->bit_mask)
            {
                    icf_port_strcpy(error_data.string_val[0].str,
                        ((icf_register_req_st *)p_crm_pdb->p_recv_payload)->to.
                                                            addr_val.str);
            }
            else
            {
                icf_port_strcpy(error_data.string_val[0].str,
                                "Default URI");
            }
                    
            error_data.string_val[0].str_len =
                    icf_port_strlen(error_data.string_val[0].str);
                
        }
        else
        {
/* Start: Multiple Public Users ICF5.0*/
                icf_port_strcpy(error_data.string_val[0].str,
                            p_crm_pdb->p_rgm_context->rgm_user_address.
			    addr_val.str);
                error_data.string_val[0].str_len =
                    icf_port_strlen(error_data.string_val[0].str);
/* End: Multiple Public Users ICF5.0*/
        }
        
ICF_CRM_END
ICF_CHECK_IMS_END
#endif
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
           /* ICF:  */
            icf_port_strcpy(error_data.string_val[0].str,
                    p_crm_pdb->p_rgm_context->rgm_user_address.addr_val.str);

            error_data.string_val[0].str_len =
                (icf_uint16_t ) icf_port_strlen(error_data.string_val[0].str);
ICF_CHECK_IMS_END
#endif /* NON_IMS_CLIENT : 3 */
            
            /* For Printing Registrar Address */

#ifdef IMS_CLIENT/* 4 */
ICF_CRM_START
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
            /* Incase rgm_context is NULL. registrar dummy */
            if (ICF_TRUE == rgm_ctxt_null)
            {
                icf_port_strcpy(error_data.string_val[1].str,
                                "Dummy Registrar");              
            }

            else
            {/* Begin: rgm_context not null */
ICF_CRM_END

            /* ICF: */

            /* Get pointer to line block */
            icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                    p_crm_pdb->p_rgm_context->rgm_list_of_lines[0],
                    &p_dbm_line_data,
                    &(p_crm_pdb->p_internal_msg->msg_hdr.ecode));

            if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
                    p_dbm_line_data->registrar_addr.addr.addr_type)
            {
                icf_port_strcpy(error_data.string_val[1].str,
                      p_dbm_line_data->registrar_addr.addr.addr.domain.str);                
            }
            else if (ICF_TRANSPORT_ADDRESS_IPV4_ADDR == 
                    p_dbm_line_data->registrar_addr.addr.addr_type)
            {
              ICF_SPRINTF(((icf_int8_t *)(error_data.string_val[1].str),
                            "%d%c%d%c%d%c%d",
                        p_dbm_line_data->registrar_addr.addr.addr.ipv4_addr.
                            octet_1,
                            '.',
                        p_dbm_line_data->registrar_addr.addr.addr.ipv4_addr.
                            octet_2,
                            '.',
                        p_dbm_line_data->registrar_addr.addr.addr.ipv4_addr.
                            octet_3,
                            '.',
                        p_dbm_line_data->registrar_addr.addr.addr.ipv4_addr.
                          octet_4))
            }
#ifdef ICF_IPV6_ENABLED
            else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == 
                    p_dbm_line_data->registrar_addr.addr.addr_type)
            {
                icf_port_strcpy(error_data.string_val[1].str,
                        p_dbm_line_data->registrar_addr.addr.addr.ipv6_addr.str);                
            }
#endif
            }/*ICF: ends: rgm_context not null */


            error_data.string_val[1].str_len =
                icf_port_strlen(error_data.string_val[1].str);
            break;
ICF_CHECK_IMS_END
#endif/* IMS_CLIENT : 4 */
            
        default:
            error_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            
            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    }
    
    ICF_SET_ECODE(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb,\
            &error_data,ecode,\
            &ecode) 
}    
#endif /* Error_enable */

/***************************************************************************
 *
 * FUNCTION:        icf_rgm_fetch_ctxt_for_uri
 * 
 * DESCRIPTION:     This function returns pointer to regn context for  
 *                  a user (uri). In case no regn context exists then
 *                  ICF_FAILURE is returned.
 *                   
 * RETURNS:         ICF_SUCCESS: In case regm ctxt is found.       
 *                    ICF_FAILURE: In case no regm_ctxt is found.
 **************************************************************************/

icf_return_t icf_rgm_fetch_ctxt_for_uri(
        INOUT icf_glb_pdb_st         *p_glb_pdb,
        IN    icf_address_st         *p_rgm_uri,
        OUT   icf_rgm_context_st     **p_p_rgn_cntxt_blk,
        OUT   icf_error_t            *p_ecode)
{
    icf_uint32_t            j=0; /* temporary variable */
    icf_config_data_st   *p_config_data = ICF_NULL; 
    icf_rgm_context_st   *p_rgm_context = ICF_NULL; 
    icf_return_t            ret_val = ICF_FAILURE;
#ifdef IMS_CLIENT
    icf_user_db_node_st   *p_node = ICF_NULL;
#endif


    ICF_FUNCTION_ENTER(p_glb_pdb)
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgm_uri))

    *p_p_rgn_cntxt_blk = ICF_NULL; 

    /* Proceed on getting the config data pointer from DBM */
    
    if (ICF_SUCCESS == icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(p_config_data),
                p_ecode))
    {
        icf_boolean_t addr_found = ICF_FALSE;
        /* DBM will return Failure if all Registration blocks are exhausted */
        j=1;
        /* SPR 17636: CSR 1-5390722: Added protective check for p_rgm_context*/
        while ((ICF_SUCCESS == icf_dbm_get_used_rgn_blk(
                p_glb_pdb,
                (icf_uint16_t)j,
                &p_rgm_context,
                p_ecode))  && (ICF_NULL != p_rgm_context))
        {
            if(ICF_TRUE == icf_dbm_util_cmp_addr(p_glb_pdb,
                &(p_rgm_context->rgm_user_address),
                p_rgm_uri)
                )
            {
                /* found */
                addr_found = ICF_TRUE;
            }

#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
       
            /* Get the public id node corresponding to the rgm_user addr */
            if( (ICF_FALSE == addr_found) && (ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line (
                p_glb_pdb,
                ICF_USE_LINE1,
                &(p_rgm_context->rgm_user_address),
                ICF_USER_DB_USE_PUBLIC_URIS, /* searching only public ids */
                &p_node,
                p_ecode)))
            {
                if(ICF_SUCCESS == icf_dbm_search_address_list(
                    p_glb_pdb,
                    p_node->p_assoc_uri_list,
                    (icf_address_st*)p_rgm_uri,
		    ICF_NULL))
                {
                    /* found */
                    addr_found = ICF_TRUE;
                }
            }
ICF_CHECK_IMS_END
#endif
            if(ICF_TRUE == addr_found)
            {
                /* uri found */
                *p_p_rgn_cntxt_blk = p_rgm_context; 
                ret_val = ICF_SUCCESS;
                break; 
            }

            else
            {
                /* CSR 1-5390722 */
                p_rgm_context = ICF_NULL;
                j++;
            }


        }/* While ends here */
    }/* Config data processing ends here */
    
    if (ICF_FAILURE == ret_val)
    {
        ICF_PRINT(((icf_uint8_t *)"\nERROR: Failed to get context for user = %s\n",
            p_rgm_uri->addr_val.str));
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val; 
}

/***************************************************************************
 *
 * FUNCTION:        icf_rgm_get_uri_status
 * 
 * DESCRIPTION:     This function returns the status of URI
 *                  
 *                  
 *                   
 * RETURNS:         ICF_SUCCESS: In case URI status is found.       
 *                    ICF_FAILURE: In case no URI status is found
 **************************************************************************/
icf_return_t    
icf_rgm_get_uri_status (
    INOUT    icf_glb_pdb_st    *p_glb_pdb,
    IN        icf_address_st    *p_uri, 
    IN        icf_app_id_t        app_id, 
    INOUT    icf_uint8_t        *p_state,
    INOUT    icf_error_t        *p_ecode)
{
    icf_return_t            ret_val = ICF_FAILURE;
    icf_rgm_context_st   *p_rgn_cntxt_blk; 


    ICF_FUNCTION_ENTER(p_glb_pdb)
    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_state))
    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_ecode))
    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_uri))
    /* To remove warnings */
     if(app_id==app_id)
     {
     }

    ret_val    = icf_rgm_fetch_ctxt_for_uri(
            p_glb_pdb, 
            p_uri, 
            &p_rgn_cntxt_blk, 
            p_ecode); 

   
    if ( ret_val == ICF_SUCCESS)
    {
        *p_state = p_rgn_cntxt_blk->rgm_state; 
    }

    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val; 
}

/**************************************************************************
 *
 * FUNCTION:        icf_rgm_extract_rand_autn
 * 
 * DESCRIPTION:     This function extracts the RAND and AUTN from nonce
 *                  
 * 
 * RETURNS:         Nothing
 *************************************************************************/
icf_return_t    icf_rgm_extract_rand_autn(
    INOUT    icf_glb_pdb_st        *p_glb_pdb,
    INOUT    icf_uint8_t            *p_nonce,
    INOUT    icf_string_st        *p_rand,
    INOUT    icf_string_st        *p_autn,
    INOUT    icf_error_t            *p_ecode)
{
    icf_uint8_t    *p_temp_nonce = p_nonce;
    icf_uint8_t    *p_temp_rand = p_rand->str;
    icf_uint8_t    *p_temp_autn = p_autn->str;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_return_t dummy_ret_val = ICF_SUCCESS;
    icf_uint32_t    counter = ICF_NULL;
    /* SPR 8062 FIX START */
    icf_uint8_t  *decoded_nonce_str = ICF_NULL;
    icf_uint32_t    decoded_nonce_str_len = 0; 
    /* SPR 8062 FIX END */
    /* function to decode the nonce from base64 */
    
    ICF_ASSERT(p_glb_pdb, 
        (ICF_NULL != p_temp_nonce))
        
    decoded_nonce_str_len = icf_port_strlen(p_temp_nonce);
    
    ICF_MEMGET(p_glb_pdb, 
        (decoded_nonce_str_len + 2), 
        ICF_MEM_COMMON, 
        decoded_nonce_str,
        ICF_RET_ON_FAILURE, p_ecode, ret_val)
        
    ICF_ASSERT(p_glb_pdb, 
        (ICF_NULL != decoded_nonce_str))
        
        
    icf_cmn_ssa_decodeBase64((icf_int8_t *)p_temp_nonce, (icf_int8_t *)decoded_nonce_str);
    
    decoded_nonce_str_len = icf_port_strlen(decoded_nonce_str);
    
    if(32 <= decoded_nonce_str_len)
    {
        for (counter = 0; counter < 16; counter++)
        {
            *p_temp_rand = decoded_nonce_str[counter];
            p_temp_rand++;
        }
        *p_temp_rand = '\0';
        for (/* continuing counter */; counter < 32; counter++)
        {
            *p_temp_autn = decoded_nonce_str[counter];
            p_temp_autn++;            
        }
        *p_temp_autn = '\0';
        p_rand->str_len = (icf_uint16_t ) icf_port_strlen(p_rand->str);
        p_autn->str_len = (icf_uint16_t ) icf_port_strlen(p_autn->str);
        ret_val = ICF_SUCCESS;
    }
    else
    {
        ret_val = ICF_FAILURE;
    }
    
    ICF_MEMFREE(p_glb_pdb,
        decoded_nonce_str, 
        ICF_MEM_COMMON, p_ecode, dummy_ret_val)
    return ret_val;

}/* Function icf_rgm_extract_rand_autn() ends here */



/************************************************************************
 *
 * FUNCTION:        icf_rgm_fetch_app_info
 * 
 * DESCRIPTION:     This function finds whether the application id
 *                    is present in the rgm_context or not
 *                  
 * 
 * RETURNS:         TRUE/FALSE and pointer to app_context
 *************************************************************************/
icf_boolean_t    icf_rgm_fetch_app_info(
            INOUT    icf_crm_pdb_st            *p_crm_pdb,
            INOUT    icf_rgm_context_st        *p_rgm_context,
            IN        icf_app_id_t                app_id,
            INOUT    icf_rgm_app_ctxt_st        **p_p_app_info)
{
    icf_uint32_t        loop_index = ICF_NULL;
    
    icf_boolean_t    app_status = ICF_FALSE;

    for (loop_index = 0; (loop_index < p_crm_pdb->
			    p_config_data->max_applications);
                                            loop_index++)
    {
        if (ICF_FALSE == p_rgm_context->p_app_data_list[
			loop_index].is_valid)
        {
            continue;
        }
        /* For loop: on valid entries of application list: begins */

        if (app_id ==
                    p_rgm_context->p_app_data_list[loop_index].app_id)
        {
            if(ICF_TRUE == icf_dbm_util_cmp_addr(p_crm_pdb->p_glb_pdb,
                &(p_rgm_context->p_app_data_list[loop_index].to),
                &(p_rgm_context->rgm_user_address)))
            {
                
                app_status = ICF_TRUE;
                *p_p_app_info = &(p_rgm_context->p_app_data_list[loop_index]);
                break;
            }

        }
                
    }/* For ends here */

    return app_status;

    
}/* Function icf_rgm_fetch_app_info() ends here */




/************************************************************************
 *
 * FUNCTION:        icf_rgm_make_cfm_hdr
 * 
 * DESCRIPTION:     This function makes the header for REGISTER_CFM
 *                  
 * 
 * RETURNS:         
 *************************************************************************/
icf_void_t    icf_rgm_make_cfm_hdr(
                    INOUT    icf_msg_st    *p_msg_cfm,
                    IN        icf_app_id_t    app_id)
{

    p_msg_cfm->hdr.api_id = ICF_REGISTER_CFM;
    p_msg_cfm->hdr.call_id = ICF_INVALID_CALL_ID;
    p_msg_cfm->hdr.app_id = app_id;
    p_msg_cfm->hdr.api_length = ICF_PORT_SIZEOF(icf_api_header_st) + 
                    ICF_PORT_SIZEOF(icf_register_cfm_st);
                
}/* Function icf_rgm_make_cfm_hdr() ends here */            


/*************************************************************************
 *
 * FUNCTION:        icf_rgm_make_ind_hdr
 * 
 * DESCRIPTION:     This function makes the header for REGISTER_STATUS_IND
 *                  
 * 
 * RETURNS:         
 ************************************************************************/
icf_void_t    icf_rgm_make_ind_hdr(
                    INOUT    icf_msg_st    *p_msg_ind,
                    IN        icf_app_id_t    app_id)
{

    p_msg_ind->hdr.api_id = ICF_REGISTER_STATUS_IND;
    p_msg_ind->hdr.call_id = ICF_INVALID_CALL_ID;
    p_msg_ind->hdr.app_id = app_id;
    p_msg_ind->hdr.api_length = ICF_PORT_SIZEOF(icf_api_header_st) + 
                    ICF_PORT_SIZEOF(icf_register_status_ind_st);
                
}/* Function icf_rgm_make_ind_hdr() ends here */

/*****************************************************************
 * FUNCTION:        icf_rgm_send_to_app
 * 
 * DESCRIPTION:     This function sends response to single application
 * 
 * RETURNS:         SUCCESS/ FAILURE
*******************************************************************/
icf_return_t    icf_rgm_send_to_app(
        INOUT    icf_glb_pdb_st            *p_glb_pdb,
        INOUT    icf_rgm_register_resp_st    *p_register_resp,
        OUT        icf_error_t                *p_ecode)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_return_t            dummy_ret_val = ICF_SUCCESS;
    
    icf_uint32_t            api_len_cfm = ICF_NULL;
    icf_msg_st            *p_msg_cfm = ICF_NULL;
    icf_uint32_t            api_len_ind = ICF_NULL;
    icf_msg_st            *p_msg_ind = ICF_NULL;

    icf_register_cfm_st            *p_register_cfm = ICF_NULL;
    icf_register_status_ind_st        *p_register_ind    = ICF_NULL;

    icf_register_req_st                *p_register_request = ICF_NULL;

    icf_rgm_context_st    *p_rgm_context = ICF_NULL;

    /* SPR 8064: to pass warning header to the application */
    icf_app_conf_st                *p_app_conf = ICF_NULL;
    icf_header_name_list_st        *p_app_hdr_list = ICF_NULL;    
    icf_header_name_st            *p_app_hdr    = ICF_NULL;
    icf_uint32_t                    hdr_index = ICF_NULL;
    icf_header_st                    warning_hdr;
    icf_internal_msg_st            internal_msg;
    icf_header_list_st            *p_header_list = ICF_NULL;
    icf_uint32_t                    loop_app = ICF_NULL;
    icf_config_data_st            *p_config_data = ICF_NULL;
#ifdef ICF_NW_ACTIVATE
    icf_boolean_t                  gprs_status = ICF_TRUE;
#endif
	icf_rgm_glb_data_st			*p_rgm_glb_data = ICF_NULL;

#ifdef IMS_CLIENT            
    icf_line_data_st            *p_line_data = ICF_NULL;
#endif

ICF_4_0_CRM_START
    if (ICF_DUMMY_APP_ID == p_register_resp->app_id)
    {
        /* No response in case of DUMMY app ID */
        return ICF_SUCCESS;
    }
ICF_4_0_CRM_END
    
    p_rgm_context = p_register_resp->p_rgm_context;

    if (p_register_resp->options_flag & ICF_RGM_RESPONSE_CFM)
    {
        api_len_cfm = ICF_PORT_SIZEOF(icf_api_header_st) + 
                    ICF_PORT_SIZEOF(icf_register_cfm_st);
                
        /* Allocate memory to the response message strucure */
        ICF_MEMGET(
                    p_glb_pdb,
                    api_len_cfm,
                    ICF_MEM_COMMON,
                    p_msg_cfm,
                    ICF_RET_ON_FAILURE,
                    p_ecode,
                    ret_val)

        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_msg_cfm))

           
        p_register_cfm = (icf_register_cfm_st *)(p_msg_cfm->payload);
        
        icf_rgm_make_cfm_hdr(p_msg_cfm, p_register_resp->app_id);
           
        p_register_cfm->result = p_register_resp->status;
        p_register_cfm->bit_mask = ICF_NULL;

        if (ICF_FAILURE == p_register_resp->status)
        {
#ifdef ICF_NW_ACTIVATE
            if ((ICF_FAILURE != icf_cmn_chk_gprs_status(
                p_glb_pdb, &gprs_status, p_ecode)) &&
                (ICF_FALSE == gprs_status))
            {
                p_register_cfm->error_cause = 
                    ICF_CAUSE_GPRS_SUSPENDED;
                
            }
            else 
#endif
            {  

                p_register_cfm->error_cause = 
                    p_register_resp->error_cause;
            }
           p_register_cfm->bit_mask |= ICF_REG_CFM_ERROR_CAUSE_PRESENT;
        }

        p_register_cfm->request_type = p_register_resp->request_type;
        p_register_cfm->line_id = p_register_resp->line_id;
        if (ICF_RGM_CLONE_HEADERS & p_register_resp->options_flag)
        {
            /* Cloning header list option selected: begins  */
            if (ICF_NULL == p_rgm_context)
            {
               ICF_MEMFREE(
                        p_glb_pdb,
                        p_msg_cfm,
                        ICF_MEM_COMMON,
                        p_ecode,
                        ret_val) 
               return ICF_FAILURE;
            }

            /* get the pointer to CFG data:  */

            ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)(&(p_config_data)),
                p_ecode);     

        ICF_ASSERT(p_glb_pdb,
                            (ICF_NULL != p_config_data))


            for (loop_app = 0; loop_app < p_config_data->max_applications;
                                                    loop_app++)
            {
                if (ICF_FALSE == p_rgm_context->p_app_data_list[
                            loop_app].is_valid)
                {
                    continue;
                }
                /* Application on Probation need not be sent the response */
                if (ICF_IS_BIT_SET(p_rgm_context->p_app_data_list[
                            loop_app].app_indicator,
                            ICF_RGM_APP_DEREG_ON_PROBATION))
                {
                    continue;
                }
                if (p_register_resp->app_id == p_rgm_context->p_app_data_list[
                            loop_app].app_id )
                {
                    p_header_list = &(p_rgm_context->p_app_data_list[
                            loop_app].inc_header_list);
                    break;
                }
            }/*for loop on app data list ends */


            if ((ICF_NULL != p_header_list) &&
                (ICF_NULL != p_header_list->count))
            {

                if (ICF_SUCCESS == icf_cmn_clone_header_list(
                                        p_glb_pdb,
                                        p_header_list,
                                        &(p_register_cfm->header_list),
                                        p_ecode))
                {
                    p_register_cfm->bit_mask |= ICF_HDR_LIST_PRESENT;                
                    
                }
                else
                {
                    ICF_MEMFREE(
                             p_glb_pdb,
                             p_msg_cfm,
                             ICF_MEM_COMMON,
                             p_ecode,
                             ret_val)
                    return ICF_FAILURE;
                }
            }
            else
                p_register_cfm->header_list.count = 0;
        }/*Cloning header list option selected: ends */
        
        else
            p_register_cfm->header_list.count = 0;



        p_register_cfm->body_list.count = 0;
        /*p_register_cfm->tag_list.count = ICF_NULL;*/


           
        if (ICF_RGM_REQUEST_COPY & p_register_resp->options_flag)
        {
               
           /* Use payload to copy the To/ From */
            p_register_request = (icf_register_req_st *)
                                        p_register_resp->p_request_payload;

            ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_register_request))
            if(ICF_NULL !=  p_register_request)    
            { 
                if (ICF_REG_REQ_TO_PRESENT & p_register_request->bit_mask)
                {
                    icf_port_strcpy(p_register_cfm->to.addr_val.str,
                            p_register_request->to.addr_val.str);
                    p_register_cfm->to.addr_val.str_len = 
                        p_register_request->to.addr_val.str_len;
                    p_register_cfm->to.addr_type = 
                        p_register_request->to.addr_type;
                    p_register_cfm->bit_mask |= ICF_REG_CFM_TO_PRESENT;
                }
            }

        }/* The To/ From copied from Request */
          
           
#ifdef IMS_CLIENT            
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)

     	if ((ICF_NULL != p_rgm_context)&&(ICF_ADDRESS_PLAN_UNDEFINED != 
                p_rgm_context->rgm_user_address.addr_type))
        {

            ICF_ASSERT(p_glb_pdb, 
                            (ICF_NULL != p_register_resp->p_rgm_context))

            p_rgm_context = p_register_resp->p_rgm_context;
              
            /* SPR: 9849 Fix: start */
           /* To given value as rgm_user_address */
           icf_port_strcpy(p_register_cfm->to.addr_val.str,
                                p_rgm_context->rgm_user_address.addr_val.str);
           p_register_cfm->to.addr_val.str_len = icf_port_strlen(
                                p_register_cfm->to.addr_val.str);
           p_register_cfm->to.addr_type = 
                                p_rgm_context->rgm_user_address.addr_type;


           p_register_cfm->bit_mask |= ICF_REG_CFM_TO_PRESENT;
         }
    
         else
         {
            /* Do nothing: No value of To/ From copied into the CFM */

         }
/* If this is the dummy reg cfm for the request without AOR copy the users on this line */
         if(ICF_REQUEST_TYPE_REGISTER_ALL_USERS_ON_LINE == p_register_resp->request_type )
         {
            if ( ICF_FAILURE == 
                icf_dbm_fetch_line_cntxt_blk(
                p_glb_pdb, p_rgm_context->rgm_list_of_lines[0],
                (icf_void_t *)&(p_line_data),
                p_ecode))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                icf_dbm_copy_user_list_to_pub_uri_list (
                    p_glb_pdb,
                    p_line_data->p_user_db,
                    &(p_register_cfm->p_uri_list),
                    ICF_USER_DB_USE_PUBLIC_URIS,
                    p_ecode
                    );
                if(ICF_NULL != p_register_cfm->p_uri_list)
                {
                    p_register_cfm->bit_mask |= ICF_REG_IND_ASSO_URI_PRESENT;
                }
            }         
         }
         else if (ICF_RGM_FILL_URIS & p_register_resp->options_flag)
         {
             if(ICF_FAILURE == icf_regm_copy_uris(p_glb_pdb,
                 &(p_register_cfm->p_uri_list),
                 p_register_resp->p_rgm_context,
                 p_register_resp->line_id,
                 p_ecode))
             {
                 return ICF_FAILURE;
             }
             else if(ICF_NULL != p_register_cfm->p_uri_list)
             {
                 /* Set the Bitmask: */
                 p_register_cfm->bit_mask |= ICF_REG_IND_ASSO_URI_PRESENT;
             }

         }
ICF_CHECK_IMS_END
#endif

#ifdef NON_IMS_CLIENT
/* FOR NON IMS */
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
        if (ICF_RGM_CTXT_COPY & p_register_resp->options_flag)
        {

           p_register_cfm->to.addr_type = 
               p_rgm_context->rgm_user_address.addr_type;
           /* To given value as first element of p_rgm_uris list */
           icf_port_strcpy(p_register_cfm->to.addr_val.str,
                                p_rgm_context->rgm_user_address.addr_val.str);
           p_register_cfm->to.addr_val.str_len = 
                                p_rgm_context->rgm_user_address.addr_val.str_len;
           
           p_register_cfm->bit_mask |= ICF_REG_IND_TO_PRESENT;

         }
ICF_CHECK_IMS_END
#endif
         /* SPR 8064: if Warning received from network: then fetch
         * application data and if specified by application, then send
         * in response */

        if(ICF_RGM_FILL_WARNING & p_register_resp->options_flag)
        {/* Warning header value found in network response: begins */

            if (ICF_SUCCESS == icf_cfg_fetch_app_conf(p_glb_pdb,
                                        p_register_resp->app_id,
                                        &p_app_conf,
                                        p_ecode))
            {
                /* If app_conf found successfully : begins */

                p_app_hdr_list = &(p_app_conf->hdr_list);

                /* point to the first header of the list */
                p_app_hdr = p_app_hdr_list->hdr;
                for (hdr_index = 0; hdr_index < p_app_hdr_list->count;
                    hdr_index++)
                {
                    /* for loop on hdr_list of application */
                    if (ICF_NULL == p_app_hdr) break;

                    /* Comparing for warning header: NOTE that this
                     * is not strcasesmp() */
                    if (!icf_port_strcmp(p_app_hdr->hdr_name.str,(icf_uint8_t *)"Warning"))
                    {
                        /* add Warning header to the CFM payload : Begins*/
                        icf_port_strcpy(warning_hdr.hdr_name.str,
                                        (icf_uint8_t *)"Warning");
                        warning_hdr.hdr_name.str_len =
                                    (icf_uint16_t )icf_port_strlen(warning_hdr.
						    hdr_name.str);

                        icf_port_strcpy(warning_hdr.hdr_value.str,
                                        p_rgm_context->warning_hdr_value.str);
                        warning_hdr.hdr_value.str_len =
                                    (icf_uint16_t )icf_port_strlen(warning_hdr.
						    hdr_value.str);

                        if (ICF_FAILURE == icf_regm_append_hdr_to_list(
                                        p_glb_pdb, &warning_hdr,
                                        &(p_register_cfm->header_list),
                                        p_ecode))
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }
                        else
                        {
                            /* header successfully added to CFM's hdr_list*/
                            p_register_cfm->bit_mask |= 
                                        ICF_HDR_LIST_PRESENT;                                        
                            break;
                        }                

                    }/* add Warning header to the CFM payload : Ends */                    
                    else
                    {
                        /* goto next header */
                        p_app_hdr = p_app_hdr->next;
                    }

                }/* for loop on hdr_list of application : ends*/

            }/* If app_conf found successfully : ends */

            else
            {
                ret_val = ICF_FAILURE;

            }


        }/* Warning header value found in network response: ends */

         
        if (ICF_FAILURE != ret_val) 
        {
            /* Send the response message */
/* MULTIPLE APP FIX START        */

            internal_msg.msg_hdr.p_glb_pdb = p_glb_pdb;
            internal_msg.msg_hdr.msg_id = ICF_REGISTER_CFM;
            internal_msg.p_msg_data = (icf_uint8_t *)p_msg_cfm;
            ret_val = icf_adm_process_mesg(&internal_msg);
/* MULTIPLE APP FIX END*/

        }

		if(ICF_NULL != (p_register_cfm->bit_mask & ICF_REG_IND_ASSO_URI_PRESENT))
		{
			icf_cfg_delete_public_uri_list (
				p_glb_pdb,
				&(p_register_cfm->p_uri_list),
				p_ecode);
		}
        
        /* Free the memory allocated to the response message */
        if (ICF_SUCCESS == ret_val)		
        {
			if(0 != p_register_cfm->header_list.count)
			{
				  dummy_ret_val = icf_cmn_free_header_list(
											p_glb_pdb,
											&(p_register_cfm->header_list),
											p_ecode);
			}
		
            /* Free memory */
            ICF_MEMFREE(
                        p_glb_pdb,
                        p_msg_cfm,
                        ICF_MEM_COMMON,
                        p_ecode,
                        dummy_ret_val)
        }
        else
        {
	        dummy_ret_val = icf_cmn_free_header_list(
                        p_glb_pdb,
                        &(p_register_cfm->header_list),
                        p_ecode);
	
            ICF_MEMFREE(
                        p_glb_pdb,
                        p_msg_cfm,
                        ICF_MEM_COMMON,
                        p_ecode,
                        dummy_ret_val)
                    /* restore the return value of ICF_SEND */
            ret_val = ICF_FAILURE;
        }
    }/* CFM sent to single application */
    else
    {
        api_len_ind = ICF_PORT_SIZEOF(icf_api_header_st) + 
                    ICF_PORT_SIZEOF(icf_register_status_ind_st);


        ICF_MEMGET(
                    p_glb_pdb,
                    api_len_ind,
                    ICF_MEM_COMMON,
                    p_msg_ind,
                    ICF_RET_ON_FAILURE,
                    p_ecode,
                    ret_val)
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_msg_ind))

        p_register_ind = (icf_register_status_ind_st *)(p_msg_ind->payload);
        
        icf_rgm_make_ind_hdr(p_msg_ind, p_register_resp->app_id);
           
        p_register_ind->result = p_register_resp->status;
        p_register_ind->bit_mask = ICF_NULL;

        if (ICF_FAILURE == p_register_resp->status)
        {
#ifdef ICF_NW_ACTIVATE
            if ((ICF_FAILURE != icf_cmn_chk_gprs_status(
                p_glb_pdb, &gprs_status, p_ecode)) &&
                (ICF_FALSE == gprs_status))
            {
                p_register_ind->error_cause = 
                    ICF_CAUSE_GPRS_SUSPENDED;
                
            }
            else 
#endif
            {
                p_register_ind->error_cause = 
                    p_register_resp->error_cause;
            }
           p_register_ind->bit_mask |= ICF_REG_IND_ERROR_CAUSE_PRESENT;
        }

        p_register_ind->request_type = p_register_resp->request_type;
        p_register_ind->line_id = p_register_resp->line_id;            
        p_register_ind->header_list.count = 0;
        /*p_register_ind->body_list.count = ICF_NULL;*/

        /*p_register_ind->tag_list.count = ICF_NULL;*/


           
        if (ICF_RGM_REQUEST_COPY & p_register_resp->options_flag)
        {
            /* Use payload to copy the To/ From */
            p_register_request = (icf_register_req_st *)
                                        p_register_resp->p_request_payload;

            ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_register_request))
           
           if (ICF_REG_REQ_TO_PRESENT & p_register_request->bit_mask)
           {
               icf_port_strcpy(p_register_ind->to.addr_val.str,
                            p_register_request->to.addr_val.str);
               p_register_ind->to.addr_val.str_len = 
                            p_register_request->to.addr_val.str_len;
               p_register_ind->to.addr_type = 
                            p_register_request->to.addr_type;
               p_register_ind->bit_mask |= ICF_REG_IND_TO_PRESENT;
           }
        }/* The To/ From copied from Request */
        else if (ICF_RGM_CTXT_COPY & p_register_resp->options_flag)
        {
           /* SPR: 9849 Fix: Even in IMS scenario */
           /* To/From given value as rgm_user_address */
	       /* This change put during merging */       
        
           p_register_ind->to.addr_type = 
               p_rgm_context->rgm_user_address.addr_type;
           /* To given value as first element of p_rgm_uris list */
           icf_port_strcpy(p_register_ind->to.addr_val.str,
                                p_rgm_context->rgm_user_address.addr_val.str);
           p_register_ind->to.addr_val.str_len = 
                                p_rgm_context->rgm_user_address.addr_val.str_len;
           
           p_register_ind->bit_mask |= ICF_REG_IND_TO_PRESENT;

         }
         else
         {
            /* Do nothing: No value copied in To/ From */
         }
        
         /* SPR 8064: if Warning received from network: then fetch
         * application data and if specified by application, then send
         * in response */

        if(ICF_RGM_FILL_WARNING & p_register_resp->options_flag)
        {/* Warning header value found in network response: begins */

            if (ICF_SUCCESS == icf_cfg_fetch_app_conf(p_glb_pdb,
                                        p_register_resp->app_id,
                                        &p_app_conf,
                                        p_ecode))
            {
                /* If app_conf found successfully : begins */

                p_app_hdr_list = &(p_app_conf->hdr_list);

                /* point to the first header of the list */
                p_app_hdr = p_app_hdr_list->hdr;
                for (hdr_index = 0; hdr_index < p_app_hdr_list->count;
                    hdr_index++)
                {
                    /* for loop on hdr_list of application */
                    if (ICF_NULL == p_app_hdr) break;

                    /* Comparing for warning header: NOTE that this
                     * is not strcasesmp() */
                    if (!icf_port_strcmp(p_app_hdr->hdr_name.str,(icf_uint8_t *)"Warning"))
                    {
                        /* add Warning header to the CFM payload : Begins*/
                        icf_port_strcpy(warning_hdr.hdr_name.str,
                                        (icf_uint8_t *)"Warning");
                        warning_hdr.hdr_name.str_len =
                                    (icf_uint16_t ) icf_port_strlen(
						 warning_hdr.hdr_name.str);

                        icf_port_strcpy(warning_hdr.hdr_value.str,
                                        p_rgm_context->warning_hdr_value.str);
                        warning_hdr.hdr_value.str_len =
                                    (icf_uint16_t )icf_port_strlen(
					    warning_hdr.hdr_value.str);

                        if (ICF_FAILURE == icf_regm_append_hdr_to_list(
                                        p_glb_pdb, &warning_hdr,
                                        &(p_register_ind->header_list),
                                        p_ecode))
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }
                        else
                        {
                            /* header successfully added to IND's hdr_list*/
                            p_register_ind->bit_mask |= 
                                     ICF_HDR_LIST_PRESENT;                                        
                            break;
                        }                

                    }/* add Warning header to the IND payload : Ends */                    

                }/* for loop on hdr_list of application : ends*/

            }/* If app_conf found successfully : ends */

            else
            {
                ret_val = ICF_FAILURE;

            }


        }/* Warning header value found in network response: ends */

#ifdef IMS_CLIENT            
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)

         if ((ICF_SUCCESS == ret_val) &&
             (ICF_RGM_FILL_URIS & p_register_resp->options_flag))
         {
             /* Copy the associated URIs from rgm_context */
             
             if(ICF_FAILURE == icf_regm_copy_uris(p_glb_pdb,
                 &(p_register_ind->p_uri_list),
                 p_rgm_context,
                 p_register_resp->line_id,
                 p_ecode))
             {
                 ret_val = ICF_FAILURE;
             }
             else if(ICF_NULL != p_register_ind->p_uri_list)
             {
             /* Set the Bitmask: */
                 p_register_ind->bit_mask |= ICF_REG_IND_ASSO_URI_PRESENT;
             }
         }
ICF_CHECK_IMS_END
#endif  
/* START: Multiple Public Users ICF5.0 */

       if(ICF_NULL != (ICF_REGISTER_FOR_ALL_USER & p_rgm_context->indicator))
       {
           p_rgm_context->indicator &= ~ICF_REGISTER_FOR_ALL_USER;

           /* get the pointer to RGM global data */
           if(ICF_FAILURE == icf_dbm_get_module_glb_data(
               p_glb_pdb,
               (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
               (icf_void_t **)(&(p_rgm_glb_data)),
               p_ecode))
           {
               ret_val = ICF_FAILURE;
           }

		   p_rgm_glb_data->ongoing_reg_counter--;
       }
/* END: Multiple Public Users ICF5.0 */
      

        if (ICF_FAILURE != ret_val) 
        {
            /* Send the response message */

/* MULTIPLE APP FIX START     */

/*    ICF_SEND(p_glb_pdb, p_msg_ind, ret_val, p_ecode)*/


            internal_msg.msg_hdr.p_glb_pdb = p_glb_pdb;
            internal_msg.msg_hdr.msg_id = ICF_REGISTER_STATUS_IND;
            internal_msg.p_msg_data = (icf_uint8_t *)p_msg_ind;
            ret_val = icf_adm_process_mesg(&internal_msg);
/* MULTIPLE APP FIX END*/

        }
        
	    if(ICF_NULL != (p_register_ind->bit_mask & ICF_REG_IND_ASSO_URI_PRESENT))
		{
		    icf_cfg_delete_public_uri_list (
				p_glb_pdb,
				&(p_register_ind->p_uri_list),
				p_ecode);
			
		}
		
        /* Free the memory allocated to the response message */
        ICF_MEMFREE(
            p_glb_pdb,
            p_msg_ind,
            ICF_MEM_COMMON,
            p_ecode,
            dummy_ret_val)
    }/* IND sent to single application : ends here */
    return ret_val;
    
}/* Function icf_rgm_send_to_app() ends here */

/*************************************************************************
 *
 * FUNCTION:        icf_rgm_send_response
 * 
 * DESCRIPTION:     This function sends the REGISTER response 
 *                  
 * 
 * RETURNS:         SUCCESS/ FAILURE
 **************************************************************************/
icf_return_t        icf_rgm_send_response(
            INOUT    icf_glb_pdb_st            *p_glb_pdb,
            INOUT    icf_rgm_register_resp_st    *p_register_resp,
            INOUT    icf_error_t        *p_ecode)
{

    icf_return_t            ret_val = ICF_SUCCESS;

    icf_rgm_context_st    *p_rgm_context = ICF_NULL;
    icf_uint32_t            loop_app = ICF_NULL;
    icf_crm_pdb_st         *p_crm_pdb  = ICF_NULL;
    icf_uint32_t            max_applications = ICF_NULL;
    /*CSR 8070301 SPR 20236*/
    icf_result_t			crm_pdb_result = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_register_resp))

    
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_register_resp->p_crm_pdb))

    ICF_ASSERT(p_glb_pdb, 
            (ICF_NULL != p_register_resp->p_crm_pdb->p_config_data))
    p_crm_pdb=p_register_resp->p_crm_pdb;
    /* Fix for CSR 1-8070301: SPR 20236 */
	/* Before changing the result in p_crm_pdb for REGM's local
     * usage, take a backup of the value and restore this original
	 * value before returning from this function.
	 */
	crm_pdb_result = p_crm_pdb->result;
    p_crm_pdb->result = p_register_resp->status;
    ICF_RGM_APP_TRACE(p_crm_pdb, ICF_TRACE_REG_RESP)
    
    if (ICF_RGM_SINGLE_APP & p_register_resp->options_flag)
    {
        /* Response to be sent to single application */
        
        /* Return with FAILURE if response type not clearly indicated */
        if (
            ((p_register_resp->options_flag & ICF_RGM_RESPONSE_CFM) &&
                (p_register_resp->options_flag & ICF_RGM_RESPONSE_IND)) ||
            (!(p_register_resp->options_flag & ICF_RGM_RESPONSE_CFM) &&
                !(p_register_resp->options_flag & ICF_RGM_RESPONSE_IND)) 
                    )
        {
            /* Fix for CSR 1-8070301: SPR 20236 */
			/* Restore the value of result in p_crm_pdb */
			p_crm_pdb->result = crm_pdb_result;

            /* Either both set together or both reset together */
            return ICF_FAILURE;
        }
		if (ICF_NULL != p_register_resp->p_rgm_context)
		{
			if(ICF_REGISTER_FOR_ALL_USER & p_register_resp->p_rgm_context->indicator)
			{
				p_register_resp->options_flag &= ~ICF_RGM_RESPONSE_CFM;
				p_register_resp->options_flag |= ICF_RGM_RESPONSE_IND;
			}
		}
        
        ret_val = icf_rgm_send_to_app(p_glb_pdb, p_register_resp, p_ecode);
        
    }/* Single app Handled */

    else
    {
        ICF_ASSERT(p_glb_pdb, 
            (ICF_NULL != p_register_resp->p_rgm_context))

        p_rgm_context = p_register_resp->p_rgm_context;
        
        max_applications = p_register_resp->p_crm_pdb->
                                        p_config_data->max_applications;

        /* to/ From values to be copied from rgm_context*/
        p_register_resp->options_flag |= ICF_RGM_CTXT_COPY;

        for (loop_app = 0; loop_app < max_applications;
                                                    loop_app++)
        {
            if (ICF_FALSE == p_rgm_context->p_app_data_list[
                            loop_app].is_valid)
            {
                continue;
            }
	   /* Application on Probation need not be sent the response */
            if (ICF_IS_BIT_SET(p_rgm_context->p_app_data_list[
                            loop_app].app_indicator,
                            ICF_RGM_APP_DEREG_ON_PROBATION))
            {
                    continue;
            }
	    
            if(ICF_NULL == p_register_resp->app_id)
            {
            /* For each valid app_data entry */
                p_register_resp->app_id = p_rgm_context->p_app_data_list[
                    loop_app].app_id;
                p_register_resp->line_id = p_rgm_context->p_app_data_list[
                    loop_app].line_id;
            }
            if (ICF_RGM_APP_CFM_SENT    &
                        p_rgm_context->p_app_data_list[loop_app].app_indicator)
            {
                p_register_resp->options_flag |= ICF_RGM_RESPONSE_IND;
                ret_val = icf_rgm_send_to_app(p_glb_pdb,
			       	p_register_resp,p_ecode);

                /* get the state of options_flag back to original for next app */
                p_register_resp->options_flag &= ~ICF_RGM_RESPONSE_IND;

                if (ICF_FAILURE == ret_val)
                {
                   /* Fix for CSR 1-8070301: SPR 20236 */
					/* Restore the value of result in p_crm_pdb */
					p_crm_pdb->result = crm_pdb_result; 
                    return ret_val;/*If fails, return*/
                }    
            }
            else
            {
                p_register_resp->options_flag |= ICF_RGM_RESPONSE_CFM;
				if (ICF_NULL != p_register_resp->p_rgm_context)
				{
					if(ICF_REGISTER_FOR_ALL_USER & p_register_resp->p_rgm_context->indicator)
					{
						p_register_resp->options_flag &= ~ICF_RGM_RESPONSE_CFM;
						p_register_resp->options_flag |= ICF_RGM_RESPONSE_IND;
					}
                }

                ret_val = icf_rgm_send_to_app(p_glb_pdb,
			       	p_register_resp,p_ecode);

                p_rgm_context->p_app_data_list[loop_app].app_indicator |=
                                            ICF_RGM_APP_CFM_SENT;

                /* get the state of options_flag back to 
		 * original for next app */
                p_register_resp->options_flag &= ~ICF_RGM_RESPONSE_CFM;

                if (ICF_FAILURE == ret_val) 
                {
                   /* Fix for CSR 1-8070301: SPR 20236 */
					/* Restore the value of result in p_crm_pdb */
					p_crm_pdb->result = crm_pdb_result;
                    return ret_val;/*If fails, return */
                } 

            
            }
        
        }/* For loop ends here */

    }/* All applications of rgm_context handled*/
    /* Fix for CSR 1-8070301: SPR 20236 */
	/* Restore the value of result in p_crm_pdb */
	p_crm_pdb->result = crm_pdb_result;
    return ret_val;


}/* Function icf_rgm_send_response() ends here */


/*********************************************************************
 *
 * FUNCTION:        icf_rgm_copy_app_details
 * 
 * DESCRIPTION:     This function copies the application related details
 *                 from the register_request payload to rgm_app_cxt_st memory
 *                  
 * 
 * RETURNS:         SUCCESS/FAILURE
 *************************************************************************/
icf_return_t    icf_rgm_copy_app_details(
                INOUT    icf_glb_pdb_st        *p_glb_pdb,
                INOUT    icf_rgm_app_ctxt_st   *p_app_data,
                INOUT    icf_register_req_st      *p_register_req_payload,
                IN        icf_app_id_t         app_id,
                OUT        icf_error_t         *p_ecode)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    
    icf_duration_t            default_duration = 
                                ICF_RGM_DEFAULT_DURATION_NON_IMS;

    p_app_data->app_id = app_id;
    default_duration=default_duration;   
    /* Copying the line_id from the api payload */

    p_app_data->line_id = p_register_req_payload->line_id;
#ifdef IMS_CLIENT
    /*Extract the default values of the registration duration when IMS_CLIENT is on*/
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    default_duration = ICF_RGM_DEFAULT_DURATION_IMS_CLIENT;
ICF_CHECK_IMS_END
#endif
    if (ICF_NULL != p_app_data->header_list.count)
    {
        /* Free the header list that is already present
         * This may happen if the application
         * data is already present in the rgm_context */
                        
        ret_val = icf_cmn_free_header_list(
                                p_glb_pdb,
                                &(p_app_data->header_list),
                                p_ecode);
        p_app_data->header_list.count = ICF_NULL;
        p_app_data->bitmask &= ~ICF_HDR_LIST_PRESENT;
        ret_val = ICF_SUCCESS;

    }
    if (ICF_NULL != p_app_data->body_list.count)
    {
        /* Free the body list that is already present
         * This may happen if the application
         * data is already present in the rgm_context */
                        
        ret_val = icf_cmn_free_msg_body_list(
                                p_glb_pdb,
                                &(p_app_data->body_list),
                                p_ecode);
        p_app_data->body_list.count = ICF_NULL;
        p_app_data->bitmask &= ~ICF_BODY_LIST_PRESENT;
        ret_val = ICF_SUCCESS;

    }
    if (ICF_NULL != p_app_data->tag_list.count)
    {
        /* Free the tag list that is already present
         * This may happen if the application
         * data is already present in the rgm_context */
                        
        ret_val = icf_cmn_free_tag_list(
                                p_glb_pdb,
                                &(p_app_data->tag_list),
                                p_ecode);
        p_app_data->tag_list.count = ICF_NULL;
        p_app_data->bitmask &= ~ICF_TAG_LIST_PRESENT;
        p_app_data->bitmask &= ~ICF_TAGS_REFRESHED;
        ret_val = ICF_SUCCESS;

    }

    /* Copy the header list, if present */
    if (ICF_HDR_LIST_PRESENT &
                                    p_register_req_payload->bit_mask)
    {
        if (ICF_FAILURE == icf_cmn_clone_header_list(
                                p_glb_pdb, 
                                &(p_register_req_payload->header_list),
                                &(p_app_data->header_list),
                                p_ecode))
        {
              return ICF_FAILURE;
        }
                
        p_app_data->bitmask |= ICF_HDR_LIST_PRESENT;
            
    }/* header list copying */


    /* Copy the tag list, if present */
    if (ICF_TAG_LIST_PRESENT & p_register_req_payload->bit_mask)
    {
        if (ICF_FAILURE == icf_cmn_clone_tag_list(
                                p_glb_pdb, 
                                &(p_register_req_payload->tag_list),
                                &(p_app_data->tag_list),
                                p_ecode))
        {
            return ICF_FAILURE;
            
        }
        p_app_data->bitmask |= ICF_TAG_LIST_PRESENT;
        p_app_data->bitmask |= ICF_TAGS_REFRESHED;
        
    }/* tag list copying */

    /* Copy the Body list, if present */
    if (ICF_BODY_LIST_PRESENT &
                                    p_register_req_payload->bit_mask)
    {
        if (ICF_FAILURE == icf_cmn_clone_body_list(
                                p_glb_pdb, 
                                &(p_register_req_payload->body_list),
                                &(p_app_data->body_list),
                                p_ecode))
        {
            return ICF_FAILURE;
        }
        
        p_app_data->bitmask |= ICF_BODY_LIST_PRESENT;
        
    }/* Body list copying */

    if(ICF_REG_REQ_TO_PRESENT & p_register_req_payload->bit_mask)
    {
        icf_port_memcpy(&(p_app_data->to),&(p_register_req_payload->to),sizeof(icf_address_st));
    }

    p_app_data->is_valid = ICF_TRUE;

    return ret_val;            
    

}/* Function icf_rgm_copy_app_details() Ends here */


/************************************************************************
 *
 * FUNCTION:        icf_rgm_get_empty_app
 * 
 * DESCRIPTION:     This function returns the pointer to an empty app_data
 *                    entry of the rgm_context
 * 
 * RETURNS:         SUCCESS/FAILURE
 ************************************************************************/
icf_return_t    icf_rgm_get_empty_app(
                INOUT    icf_glb_pdb_st        *p_glb_pdb,
                INOUT    icf_crm_pdb_st        *p_crm_pdb,
                INOUT    icf_rgm_context_st    *p_rgm_context,
                INOUT    icf_rgm_app_ctxt_st    **p_p_app_data,
                OUT        icf_error_t            *p_ecode)
{
    icf_uint32_t            loop_app= ICF_NULL;
    icf_return_t            ret_val = ICF_FAILURE;
    
    /* To remove warning */ 
    p_glb_pdb=p_glb_pdb;
    p_ecode=p_ecode;

    ICF_ASSERT(p_glb_pdb, 
            (ICF_NULL != p_crm_pdb->p_config_data))

    for (loop_app = 0; loop_app < p_crm_pdb->p_config_data->max_applications;
                        loop_app++)
    {
        if (ICF_FALSE == p_rgm_context->p_app_data_list[loop_app].is_valid)
        {
            *p_p_app_data = &(p_rgm_context->p_app_data_list[loop_app]);
            ret_val = ICF_SUCCESS;
            break;

        }

    }/* for loop on app_data_list ends */
    return ret_val;


}/* Function icf_rgm_get_empty_app() ends here */

/*************************************************************************
 *
 * FUNCTION:        icf_rgm_update_line_data
 * 
 * DESCRIPTION:     This function updates the line data on receipt of 200 OK
 * 
 * RETURNS:         SUCCESS/FAILURE
 **************************************************************************/
icf_return_t    icf_rgm_update_line_data(
            INOUT    icf_glb_pdb_st        *p_glb_pdb, 
            INOUT    icf_rgm_context_st    *p_rgm_context,
            INOUT    icf_line_data_st        *p_line_data, 
            INOUT    icf_line_data_st        *p_line_data_with_auth,
            OUT        icf_error_t            *p_ecode)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    /* To remove warnings */
    p_ecode=p_ecode;
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Copying the rgm_duration */
    p_line_data->registration_duration = p_rgm_context->rgm_duration;    


    /* Copying the authorization related info */

    /* Copying the authentication key */
    if (ICF_CFG_LINE_DATA_AUTH_KEY_RECD & 
                        p_line_data_with_auth->line_data_received)
    {
        icf_port_strcpy(p_line_data->authentication_key.str,
                            p_line_data_with_auth->authentication_key.str);
        p_line_data->authentication_key.str_len = 
                    p_line_data_with_auth->authentication_key.str_len;
        p_line_data->line_data_received |= ICF_CFG_LINE_DATA_AUTH_KEY_RECD;
    }

    if (ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD & 
                        p_line_data_with_auth->line_data_received)
    {
        icf_port_strcpy(p_line_data->authentication_password.str,
                            p_line_data_with_auth->authentication_password.str);
        p_line_data->authentication_password.str_len = 
                    p_line_data_with_auth->authentication_password.str_len;
        p_line_data->line_data_received |= ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD;

    }
#ifdef IMS_CLIENT    
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    p_line_data->auth_data_received = p_line_data_with_auth->auth_data_received;
ICF_CHECK_IMS_END
#endif
    /* In rel 4.0 the auth related members have been moved into rgm_ctxt.*/    
    

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;     


}/* Function icf_rgm_update_line_data() ends here */

#ifdef IMS_CLIENT
/**************************************************************************
 *
 * FUNCTION:        icf_rgm_fetch_nm_info
 * 
 * DESCRIPTION:     This function fetches the information from NM
 * 
 * RETURNS:         SUCCESS/FAILURE
 *************************************************************************/
icf_return_t    icf_rgm_fetch_nm_info(
            INOUT    icf_glb_pdb_st            *p_glb_pdb, 
            INOUT    icf_rgm_nm_info_st        *p_rgm_nm_info,    
            OUT        icf_error_t                *p_ecode)
{

    icf_internal_msg_st  internal_msg_nm;
    icf_return_t        ret_val = ICF_SUCCESS;

    icf_nm_global_st         *p_nm_ptr = ICF_NULL;



    if (ICF_RGM_NM_ACS_INFO & p_rgm_nm_info->options_flag)
    {
        /* For ICF 3.0 feature not used in ICF 4.0*/
        ret_val = ICF_FAILURE;

    }/*NM ACS info requested: Processing Ends */

    else if (ICF_RGM_NM_AKA_INFO & p_rgm_nm_info->options_flag)
    {
        icf_nm_aka_info_st    nm_aka_info;
        
        /* Get NM data from DBM */
        if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                        p_glb_pdb, (icf_module_id_t)(ICF_INT_MODULE_NM),
                        (icf_void_t**)(&p_nm_ptr), p_ecode))
        {
            p_rgm_nm_info->status = ICF_FAILURE;
            p_rgm_nm_info->ecode = ICF_NULL;
            return ICF_FAILURE;

        }
        /* Make the internal message for NM */

        internal_msg_nm.msg_hdr.msg_id = ICF_NM_AUTH;
        internal_msg_nm.msg_hdr.p_glb_pdb = p_glb_pdb;

        /* Default return values */
        p_rgm_nm_info->status = ICF_FAILURE;
        p_rgm_nm_info->ecode = ICF_NULL;

	icf_port_memset((icf_void_t *)(&nm_aka_info),0,
               (icf_uint32_t)ICF_PORT_SIZEOF(icf_nm_aka_info_st));
     
        (icf_nm_aka_info_st *)(internal_msg_nm.p_msg_data) =
                                    &nm_aka_info;
        nm_aka_info.p_nm_ptr = p_nm_ptr;



        /* Copy the RAND received from network */
        icf_port_strcpy(nm_aka_info.rand.str,
                                p_rgm_nm_info->rand.str);

        nm_aka_info.rand.str_len =
                                icf_port_strlen(nm_aka_info.rand.str);


        /* Copy the AUTN received from network */
        icf_port_strcpy(nm_aka_info.autn.str,
                            p_rgm_nm_info->autn.str);
        nm_aka_info.autn.str_len =
                                icf_port_strlen(nm_aka_info.autn.str);


        
        
        if (ICF_SUCCESS == icf_nm_process_mesg(&internal_msg_nm))
        {
            /* Depending on the data returned the corresponding ecode 
             * will indicate whether the RES or AUTS has to be used 
             * NM has not specified the errors as indicated in the design doc */
            
	   if ((ICF_NULL != nm_aka_info.res.str_len) 
                        && (ICF_NULL != nm_aka_info.auts.str_len))
            {
                /* this is error scenario and should not occur, 
                   both RES and AUTS cannot be present */
            }
            else if ((ICF_NULL == nm_aka_info.res.str_len) 
                        && (ICF_NULL == nm_aka_info.auts.str_len))
            {
                /* This is MAC failure */
                p_rgm_nm_info->status = ICF_FAILURE;
                /* Because NM has used Wrapper's error value:
                 * here stubbing to ICF_ERROR_CAUSE_NETWORK_UNAUTH */
                p_rgm_nm_info->ecode = ICF_CAUSE_HOME_NETWORK_NOT_AUTH;

            }
            else if (ICF_NULL != nm_aka_info.res.str_len) 
            {
                /* Copy the value of res correctly */
                p_rgm_nm_info->res.str_len = nm_aka_info.res.str_len ;
                icf_port_memcpy(p_rgm_nm_info->res.str, nm_aka_info.res.str,
                    p_rgm_nm_info->res.str_len);
                p_rgm_nm_info->res.str[p_rgm_nm_info->res.str_len] = '\0';

#ifdef IMS_CLIENT
                /* Over here other members that is CK and IK will also be filled. */
            
                icf_port_memcpy((icf_void_t *)(&(p_rgm_nm_info->imsCk)),
                    (icf_void_t *)(&(nm_aka_info.imsCk)),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));
                
                icf_port_memcpy((icf_void_t *)(&(p_rgm_nm_info->imsIk)),
                    (icf_void_t *)(&(nm_aka_info.imsIk)),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));
#endif
        


                p_rgm_nm_info->status = ICF_SUCCESS;
            }
            else
            {

            /* Case 2: This is SQN failure : AUTS to be used */

                p_rgm_nm_info->status = ICF_FAILURE;
                
                p_rgm_nm_info->ecode = ICF_CAUSE_SQN_OUT_OF_RANGE;
                /* Copy the value of auts correctly */
                p_rgm_nm_info->auts.str_len = nm_aka_info.auts.str_len ;
                icf_port_memcpy(p_rgm_nm_info->auts.str,
                                nm_aka_info.auts.str,
                    p_rgm_nm_info->auts.str_len);
                p_rgm_nm_info->auts.str[p_rgm_nm_info->auts.str_len] = '\0';
            }    

            return ICF_SUCCESS;
        }
        else
        {
            return ICF_FAILURE;

        }     
           

    }/* AKA info requested : Processing Ends */

    else
    {
        /* Call not with valid options */
        return ICF_FAILURE;
    }

    return ICF_SUCCESS;
    
}/* Function icf_rgm_fetch_nm_info() ends here */
#endif

/***********************************************************************
 *
 * FUNCTION:        icf_regm_validate_request_header
 * 
 * DESCRIPTION:     This function validates the header for various 
 * 		    request APIs 
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 *************************************************************************/
icf_return_t icf_regm_validate_request_header(
        INOUT   icf_crm_pdb_st           *p_crm_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
	if(ICF_NULL == p_crm_pdb)
    {
       return ICF_FAILURE;
    }

	p_crm_pdb = p_crm_pdb;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
		    (ICF_NULL != p_crm_pdb->p_glb_pdb->p_recv_msg->hdr))
  
    /* Validate version */
    if(ICF_VERSION_1_0 != p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.version)
    {
        p_crm_pdb->error_cause = ICF_CAUSE_INVALID_VERSION_ID;
        p_crm_pdb->is_valid_param = ICF_FALSE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ICF_SUCCESS; /* returns success always */
    }

    /* Validate call id */
    if(ICF_INVALID_CALL_ID != 
		    p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.call_id)
    {
        p_crm_pdb->error_cause = ICF_CAUSE_INVALID_CALL_ID;
        p_crm_pdb->is_valid_param = ICF_FALSE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ICF_SUCCESS; /* returns success always */
    }


    /* Validate destination id */
    if(ICF_MODULE_ICF != 
		    p_crm_pdb->p_glb_pdb->p_recv_msg->hdr.destination_id)
    {
        p_crm_pdb->error_cause = ICF_CAUSE_INVALID_DESTINATION_ID;
        p_crm_pdb->is_valid_param = ICF_FALSE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ICF_SUCCESS; /* returns success always */
    }

	ret_val = ret_val;

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ICF_SUCCESS; /* returns success always */
}/* End function */

/************************************************************************
 *
 * FUNCTION:        icf_regm_append_hdr_to_list
 * 
 * DESCRIPTION:     This function appends a icf_header_st  
 *                    to icf_header_list_st;
 *                  
 * RETURNS:         ICF_SUCCESS always
 * 
 ***********************************************************************/
icf_return_t icf_regm_append_hdr_to_list(
            INOUT    icf_glb_pdb_st            *p_glb_pdb,
            INOUT    icf_header_st            *p_header,
            INOUT    icf_header_list_st        *p_hdr_list,
            INOUT    icf_error_t                *p_ecode)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_uint32_t        hdr_index = ICF_NULL;
    icf_header_st    *p_hdr;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_hdr_list))
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_header))

    if (0 < p_hdr_list->count)
    {
        /* Non null count of headers already present in hdr_list */
        p_hdr = p_hdr_list->hdr;
        for (hdr_index = 0; hdr_index < p_hdr_list->count; hdr_index++,
                                p_hdr = p_hdr->next);

        ICF_MEMGET(
                    p_glb_pdb,
                    ICF_PORT_SIZEOF(icf_header_st),        
                    ICF_MEM_COMMON,
                    p_hdr,
                    ICF_RET_ON_FAILURE,
                    p_ecode,
                    ret_val)

        p_hdr->next = ICF_NULL;
    }
    else
    {
        /* this is the first header in the list */
        ICF_MEMGET(
                    p_glb_pdb,
                    ICF_PORT_SIZEOF(icf_header_st),        
                    ICF_MEM_COMMON,
                    p_hdr_list->hdr,
                    ICF_RET_ON_FAILURE,
                    p_ecode,
                    ret_val)

        p_hdr = p_hdr_list->hdr;
        p_hdr->next = ICF_NULL;

    }

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_hdr))

    icf_port_strcpy(p_hdr->hdr_name.str,p_header->hdr_name.str);
    icf_port_strcpy(p_hdr->hdr_value.str,p_header->hdr_value.str);
    p_hdr->hdr_name.str_len =
                (icf_uint16_t )icf_port_strlen(p_hdr->hdr_name.str);
    p_hdr->hdr_value.str_len =
                (icf_uint16_t ) icf_port_strlen(p_hdr->hdr_value.str);

    p_hdr_list->count++;



    return ret_val;

}/* Function icf_regm_append_hdr_to_list() ends here */
        
/***********************************************************************
 * FUNCTION:         icf_rgm_process_subscribe_state
 *
 * DESCRIPTION:      This function handles subscription state for reg event.
 *
 *
 * RETURNS:             
 *
 **********************************************************************/
icf_return_t icf_rgm_process_subscribe_state(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_sic_subs_state_st  *p_subs_state; 

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 

    p_subs_state = (icf_sic_subs_state_st*)
        (p_crm_pdb->p_internal_msg->p_msg_data);
    

    switch( p_subs_state->subs_state )
    {
        case ICF_SUBS_SUBSCRIBED:            
                    break;
        case ICF_SUBS_FAIL_RETRY:
                    break;
        case ICF_SUBS_FAIL_NO_RETRY:
                    break;
        case ICF_SUBS_UNSUBSCRIBED:
                    break;
    }

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;


}/* Function icf_rgm_process_subscribe_state() ends here */



ICF_CRM_END

ICF_4_0_CRM_START

/************************************************************************
 * FUNCTION:            icf_rgm_check_line_data
 *
 * DESCRIPTION:        This function checks the line data for
 *                     line id received in the payload for REGISTER_REQ API.
 *                     Failure with error_cause is returned in crm_pdb. 
 *
 *
 * RETURNS:             
 *
 ********************************************************************/
 icf_return_t icf_rgm_check_line_data(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_register_req_st        *p_recv_msg_payload = ICF_NULL; 
    icf_line_data_st        *p_line_data = ICF_NULL;
	icf_user_db_node_st	*p_user_db = ICF_NULL;
    
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 
        
        /* Get the pointer to the payload received with the API */
        p_recv_msg_payload = (icf_register_req_st *)
        (p_crm_pdb->p_recv_payload);
    
    
    
    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
        p_crm_pdb->p_glb_pdb, p_recv_msg_payload->line_id,
        &p_line_data, p_crm_pdb->p_ecode))
    {
        /* Unable to get the line block for this line id */
        
        p_crm_pdb->error_cause = ICF_CAUSE_INVALID_LINE_ID;
        ret_val = ICF_FAILURE;
	}
    
    else if (!(ICF_CFG_LINE_DATA_REGISTRAR_RECD & 
			    p_line_data->line_data_received))
    {
        p_crm_pdb->error_cause = ICF_CAUSE_NO_HOME_NETWORK;
        ret_val = ICF_FAILURE;
        
    }/* Line data not configured with REGISTRAR */
    
    else if(!(ICF_CFG_LINE_DATA_SELF_ADDR_RECD & 
			    p_line_data->line_data_received))
        
    {
        /* Self URI not present in Line da:*/
        p_crm_pdb->error_cause = ICF_CAUSE_NO_PUBLIC_URI;
        ret_val = ICF_FAILURE;
        
    }/* Public URI not availale from NM: Ends here  */
             
#ifdef IMS_CLIENT 
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE) 
    else if (!(ICF_CFG_LINE_DATA_AUTH_KEY_RECD & 
			    p_line_data->line_data_received))
    {
        p_crm_pdb->error_cause = ICF_CAUSE_NO_PRIVATE_URI;
        ret_val = ICF_FAILURE;
        
    }/* No private URI from NM : ends here */
ICF_CHECK_IMS_END
#endif
    else if (ICF_REG_REQ_TO_PRESENT & p_recv_msg_payload->bit_mask)
	{
/* Start: Multiple Public Users ICF5.0*/
		ret_val = icf_dbm_get_public_uri_node_from_uri_for_line(p_crm_pdb->p_glb_pdb,
					p_recv_msg_payload->line_id, 
					&(p_recv_msg_payload->to),
					ICF_USER_DB_USE_PUBLIC_URIS|ICF_USER_DB_USE_ASSOCIATED_URIS, 
					&p_user_db,
					p_crm_pdb->p_ecode);
		
		/* if the user going for registering is not configured,
		 *  then send failure*/
		if(ICF_FAILURE == ret_val)
		{
			 p_crm_pdb->error_cause = ICF_CAUSE_NO_PUBLIC_URI;
		}
               /*If wildcarded user is configured,
                *then register request is not sent for it */
                  if(!icf_port_strncmp((icf_uint8_t *)p_recv_msg_payload->to.addr_val.str, \
                                     (icf_uint8_t *)ICF_WILDCARD_USER, icf_port_strlen((icf_uint8_t *)ICF_WILDCARD_USER)))
                          {
                           ret_val = ICF_FAILURE;
                           p_crm_pdb->error_cause = ICF_CAUSE_WILDCARDED_URI;
                          } 

/* End: Multiple Public Users ICF5.0*/
	}
	/* function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb) 
    
    return ret_val;        
}/* Function icf_rgm_check_line_data(p_crm_pdb) ends here */


/**************************************************************************
 *
 * FUNCTION:        icf_rgm_get_ctxt_for_pub_uri
 * 
 * DESCRIPTION:     This function returns pointer to regn context for  
 *                  a public user id. In case no regn context exists then
 *                  pointer to empty ctxt is returned.
 *                   
 * RETURNS:         ICF_SUCCESS: In case regm ctxt is found.       
 *                  ICF_FAILURE: In case no regm_ctxt is found.
 **************************************************************************/
icf_return_t    icf_rgm_get_ctxt_for_pub_uri(
            INOUT icf_glb_pdb_st        *p_glb_pdb,
            IN	  icf_line_id_t			line_id,
            INOUT    icf_address_st        *p_pub_uri,
            OUT   icf_rgm_context_st    **p_p_rgn_cntxt_blk,
            OUT   icf_error_t			*p_ecode)
        
{
    icf_uint32_t            index_context = ICF_NULL; /* temporary
							       variable */
    icf_config_data_st   *p_config_data = ICF_NULL; 
    icf_rgm_context_st   *p_rgm_context = ICF_NULL; 
    icf_return_t            ret_val = ICF_FAILURE;

    icf_line_data_st        *p_line_data = ICF_NULL,
							*p_line_data_temp = ICF_NULL;
    
	icf_boolean_t			match_found = ICF_FALSE;
#ifdef IMS_CLIENT
	icf_user_db_node_st		*p_user_db = ICF_NULL;
#endif
        
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgm_uri))
        
    *p_p_rgn_cntxt_blk = ICF_NULL; 
    
    /* Proceed on getting the config data pointer from DBM */
    
    ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
        (icf_int_module_id_t)ICF_INT_MODULE_CFG,
        (icf_void_t *)&(p_config_data),
        p_ecode);
	
	icf_dbm_fetch_line_cntxt_blk(
                p_glb_pdb,
                line_id,       
                &(p_line_data),
                p_ecode);
    
    ret_val = ICF_FAILURE;
    /* DBM will return Failure if all Registration blocks are exhausted */
    
    index_context = 1;

    while (ICF_SUCCESS == icf_dbm_get_used_rgn_blk(
        p_glb_pdb,
        (icf_uint16_t)index_context,
        &p_rgm_context,
        p_ecode))
    {
        /*  Fix for CSR 1-7437801 */
        /* Here we are checking whether the Public user for which new
         * registration context need to be allocated is same as the
         * rgm_user_address in already allocated registration context .
         * Now the function icf_dbm_cmp_addr_in_aor will do the strict Uri
         * checking which mean User1@aricent.com and User1@aricent1.com are
         * treated as differnt users and hence the variable match_found is 
         * not set to ICF_TRUE which result in the allocation of new
         * registration context.This is different from our previous Philosphy
         * in which we treat User1@aricent.com and User1@aricent1.com as same
         * user and hence no new registration context is allocated .*/
        if(ICF_TRUE == icf_dbm_cmp_addr_in_aor(
        p_glb_pdb,p_pub_uri,&(p_rgm_context->rgm_user_address)))
        {
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
			icf_dbm_fetch_line_cntxt_blk(
                p_glb_pdb,
                p_rgm_context->rgm_list_of_lines[0],       
                &(p_line_data_temp),
                p_ecode);

			if(ICF_TRUE == icf_dbm_util_cmp_trans_addr(p_glb_pdb,
				&p_line_data->registrar_addr,&p_line_data_temp->registrar_addr))
			{					
				match_found = ICF_TRUE;
			}
ICF_CHECK_IMS_END
#endif


#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
			match_found = ICF_TRUE;
ICF_CHECK_IMS_END
#endif

        }
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
            if( (ICF_FALSE == match_found) && 
				(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line (
                p_glb_pdb,
                ICF_USE_LINE1,
                &(p_rgm_context->rgm_user_address),
                ICF_USER_DB_USE_PUBLIC_URIS, /* searching only public ids */
                &p_user_db,
                p_ecode)))
            {
                if(ICF_SUCCESS == icf_dbm_search_address_list(
                    p_glb_pdb,
                    p_user_db->p_assoc_uri_list,
                    p_pub_uri,
					ICF_NULL))
                {
                    /* found */
                    match_found = ICF_TRUE;
                }
            }

ICF_CHECK_IMS_END
#endif
		if(ICF_TRUE == match_found)
		{
			*p_p_rgn_cntxt_blk = p_rgm_context; 
            ret_val = ICF_SUCCESS;
			break;
		}
        index_context++;

    }/* While ends here */

    
    if ( ret_val == ICF_FAILURE)
    {
		/* means no regm ctxt exists for this URI */
		/* find an empty ctxt and return it. */
        ret_val = icf_rgm_make_rgm_context(p_glb_pdb, p_p_rgn_cntxt_blk, p_ecode);
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val; 
}/* Function icf_rgm_get_ctxt_for_pub_uri() ends here */

/************************************************************************
 *
 * FUNCTION:      icf_rgm_init_auth_data
 * 
 * DESCRIPTION:   This function initializes the authorization header 
 *                related data in rgm_context. 
 *               For NON IMS only the nonce_count is made NULL
 *               as no Auth hdr sent. For IMS scenario, the 
 *               registrar on line data is used to set uri, realm and scheme
 *               is made as Digest.
 *                   
 * RETURNS:      ICF_SUCCESS: data initialized according to 
 *               choice: IMS or non IMS
 *               ICF_FAILURE: system failure: line data fetching/ 
 *               conversion of 
 *               transport_addr to string failed
 *
 **************************************************************************/

icf_return_t    icf_rgm_init_auth_data(
            INOUT icf_crm_pdb_st            *p_crm_pdb)
{
    icf_line_data_st *p_line_data = ICF_NULL;    
    icf_result_t ret_val = ICF_SUCCESS;

    icf_rgm_context_st    *p_rgm_context = ICF_NULL;
    icf_string_st        reg_string;
    
    icf_return_t            dummy_ret_val = ICF_SUCCESS;

    /* Function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    p_rgm_context = p_crm_pdb->p_rgm_context;

    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
		    (ICF_NULL != p_rgm_context))
    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
		    (ICF_NULL != p_crm_pdb->p_config_data))

    
     /* Get pointer to last-line-in-rgm-context data block */
     if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                p_crm_pdb->p_glb_pdb,
                p_rgm_context->rgm_list_of_lines[
		p_rgm_context->num_of_lines - 1],       
                &(p_line_data),
                p_crm_pdb->p_ecode))
     {
         /* Line data fetching success: begins  */

         /*Fix done for SPR-18722 : CSR-1-6068965*/
         /* clear contents of auth_data_sent and recd */
         ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
		                        p_rgm_context->auth_data_sent.p_nonce,
                                ICF_MEM_COMMON,
                                p_crm_pdb->p_ecode, dummy_ret_val)
        
        ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
                            p_rgm_context->auth_data_recd.p_nonce,
                                ICF_MEM_COMMON,
                                p_crm_pdb->p_ecode, dummy_ret_val)
         icf_port_memset(
                (icf_void_t*)&(p_rgm_context->auth_data_recd),
                (icf_uint8_t)ICF_NULL,
                (icf_uint32_t) ICF_PORT_SIZEOF(
						 icf_rgm_auth_data_st));


            icf_port_memset(
                (icf_void_t*)&(p_rgm_context->auth_data_sent),
                (icf_uint8_t)ICF_NULL,
                (icf_uint32_t) ICF_PORT_SIZEOF(
						 icf_rgm_auth_data_st));

          
            /* making auth_retry_count as NULL */
            p_rgm_context->auth_retry_count = ICF_NULL;


         if (p_crm_pdb->p_config_data->auth_type
                                == ICF_NONIMS_AUTH_OPTION)
         {
             /* Nonce count made NULL; 
	      * to be used with QoP: to count the number of
              * REGISTER messages sent with challenge-response */

             p_rgm_context->auth_data_sent.nonce_count = ICF_NULL;
                          
         }
         else
         {
             /* IMS OPTION */
             if(ICF_SUCCESS == icf_cmn_convert_transport_addr_to_string(
                 &(p_line_data->registrar_addr.addr), (reg_string.str)))
             {
                 /* converting registrar addr to string: success: begins */

                 /* Set the value in realm */
                 icf_port_memcpy(
                        (icf_void_t *)(&(p_rgm_context->
					    auth_data_sent.realm)),
                        (icf_void_t *)(&reg_string),
                        ICF_PORT_SIZEOF(icf_string_st));

                 /* Set the value for URI */
                 icf_port_strcpy(p_rgm_context->auth_data_sent.uri.str,
                                (icf_uint8_t *)"sip:");
                 icf_port_strcat(p_rgm_context->auth_data_sent.uri.str,
                                reg_string.str);
                 p_rgm_context->auth_data_sent.uri.str_len = 
                                (icf_uint16_t ) icf_port_strlen(
						p_rgm_context->auth_data_sent.uri.str);

                 /* Set the value for Scheme */
                 icf_port_strcpy(p_rgm_context->auth_data_sent.scheme.str,
                                        (icf_uint8_t *)"Digest");

                 /* Set the bitmask */
                 p_rgm_context->auth_data_sent.auth_data_present |=
                                    ICF_AUTH_SCHEME_PRESENT | 
                                    ICF_AUTH_REALM_PRESENT  |
                                    ICF_AUTH_URI_PRESENT    ;

             }/* converting registrar addr to string: success: ends */
             else
             {
                 /* converting registrar addr to string: failure */
                 ret_val = ICF_FAILURE;
             }

         }/* IMS Option ends */

     }/* Line data fetching success: ends */

     else
     {
         /* line data fetching fails */
         ret_val = ICF_FAILURE;
     }
        

    /* Function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_init_auth_data() ends here */


/************************************************************************
 *
 * FUNCTION:        icf_rgm_process_auth_data()
 * 
 * DESCRIPTION:     This function processes the authorization data by
 *                    invoking appropriate functions for diff scenarios
 *                   
 * RETURNS:         ICF_SUCCESS: 
 *                    ICF_FAILURE: 
 *                                    
 *
 *************************************************************************/
icf_return_t    icf_rgm_process_auth_data(
            INOUT icf_crm_pdb_st            *p_crm_pdb)
{
    icf_result_t ret_val = ICF_SUCCESS;

    icf_rgm_context_st    *p_rgm_context = ICF_NULL;
    icf_boolean_t         ims_auth = ICF_TRUE;   

    /* Function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    p_rgm_context = p_crm_pdb->p_rgm_context;

    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
		    (ICF_NULL != p_rgm_context))
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
		    (ICF_NULL != p_crm_pdb->p_config_data))

    if (p_crm_pdb->p_config_data->auth_type
                                == ICF_NONIMS_AUTH_OPTION)
    {
        ret_val = icf_rgm_non_ims_auth_process(p_crm_pdb);
        
        ims_auth = ICF_FALSE;
    }
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
    if ( ICF_TRUE == ims_auth)
    {
        ret_val = icf_rgm_ims_auth_process(p_crm_pdb);

    }
ICF_CHECK_IMS_END
#endif

    /* Function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)

    return ret_val;

}/* Function icf_rgm_process_auth_data() ends here */

/*************************************************************
 *
 * FUNCTION:        icf_rgm_non_ims_auth_process
 * 
 * DESCRIPTION:     This function processes the non IMS 
 *                  authorization related
 *                  information present in auth_data_recd/sent. 
 *                   
 * RETURNS:         ICF_SUCCESS: 
 *                  ICF_FAILURE:  *
 ***********************************************************************/

icf_return_t    icf_rgm_non_ims_auth_process(
            INOUT icf_crm_pdb_st            *p_crm_pdb)
{
    icf_result_t ret_val = ICF_SUCCESS;

    icf_rgm_context_st    *p_rgm_context = ICF_NULL;
    icf_uint32_t            temp_nc = ICF_NULL;
    icf_uint32_t            temp_len = ICF_NULL;
    icf_return_t            dummy_ret_val = ICF_SUCCESS;
	icf_uint32_t            new_cnonce_int = ICF_NULL;
	icf_uint32_t            rgm_duration;

	rgm_duration = ICF_RGM_MAX_DURATION;

    /* Function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    p_rgm_context = p_crm_pdb->p_rgm_context;

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
		    (ICF_NULL != p_rgm_context))
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
		    (ICF_NULL != p_crm_pdb->p_config_data))

    if ((ICF_NULL != icf_port_strcasecmp(
		(const icf_int8_t *) p_rgm_context->auth_data_recd.scheme.str,
               (const icf_int8_t *) "Digest")) &&
            (ICF_NULL != icf_port_strcasecmp(
		(const icf_int8_t *) p_rgm_context->auth_data_recd.scheme.str,
               (const icf_int8_t *)"Basic")))
    {
            ret_val = ICF_FAILURE;
            p_crm_pdb->error_cause = 
		    ICF_CAUSE_SCHEME_NOT_SUPPORTED;

    }
    if ((ICF_SUCCESS == ret_val) &&
            (ICF_NULL == icf_port_strcasecmp(
	    (const icf_int8_t *)  p_rgm_context->auth_data_recd.scheme.str,
            (const icf_int8_t *) "Digest")))
    {
        if (!
            (
            (ICF_NULL == icf_port_strcasecmp(
                            (const icf_int8_t *) p_rgm_context->auth_data_recd.algo.str,(const icf_int8_t *)"MD5")) ||
              (ICF_NULL == icf_port_strcasecmp(
                            (const icf_int8_t *) p_rgm_context->auth_data_recd.algo.str,(const icf_int8_t *)"md5")) ||
              (ICF_NULL == icf_port_strcmpi( 
                            (icf_uint8_t *) p_rgm_context->auth_data_recd.algo.str,(icf_uint8_t *)"MD5-sess"))
                            ))
        {
            ret_val = ICF_FAILURE;
            p_crm_pdb->error_cause = ICF_CAUSE_ALGO_NOT_SUPPORTED;
        }
    }/* Allowed Algo with digest scheme is MD5*/

/* Check Received Relm with configured realm */
    if (ICF_SUCCESS == ret_val)
    {
        icf_uint32_t                loop_lines = ICF_NULL;
        icf_line_data_st            *p_line_data = ICF_NULL;
        
        /* Last line data captured: */
        for (loop_lines = 0; loop_lines < p_rgm_context->num_of_lines; loop_lines++)
        {
            if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                p_crm_pdb->p_glb_pdb,p_rgm_context->
                rgm_list_of_lines[loop_lines],
                (icf_line_data_st **)(&p_line_data),
                p_crm_pdb->p_ecode))
            {
                if(ICF_TRUE == ICF_IS_BIT_SET(
                    p_line_data->line_data_received, ICF_CFG_LINE_DATA_REALM_RECD))
                {
                    if(0 == icf_port_strcmp(p_line_data->configured_realm.str, 
                                       p_rgm_context->auth_data_recd.realm.str))
                    {
                        /* We will traverse all the lines in the REGM_CTXT 
                           until we recieve the line where the realm recieved 
                           is same as the configured realm */
                        ret_val = ICF_SUCCESS;
                        p_crm_pdb->error_cause = ICF_ERROR_NONE;
                        break;
                    }
                    else
                    {
                        ret_val = ICF_FAILURE;
                        p_crm_pdb->error_cause = ICF_CAUSE_REALM_MISMATCH;
                    }
                }
            }
        }
    }


    if (ICF_SUCCESS == ret_val)
    {/* scheme/algo support begins */            
       if (ICF_NULL == p_rgm_context->auth_data_sent.nonce_count)
		{
			temp_nc = 1;
		}
		else
		{ 
        	temp_nc = p_rgm_context->auth_data_sent.nonce_count;
		}
        /*Fix done for SPR-18722 : CSR-1-6068965*/
        /* Clear old contents of auth_data_sent */
        ICF_MEMFREE(p_crm_pdb->p_glb_pdb,
                        p_rgm_context->auth_data_sent.p_nonce,
                           ICF_MEM_COMMON,
                                p_crm_pdb->p_ecode, dummy_ret_val)
        icf_port_memset(
            (icf_void_t*)&(p_rgm_context->auth_data_sent),
            (icf_uint8_t)ICF_NULL,
            (icf_uint32_t) ICF_PORT_SIZEOF(icf_rgm_auth_data_st));
        
        /* Copy the value of nonce */
        temp_len = p_rgm_context->auth_data_recd.nonce_len;

        ICF_MEMGET(p_crm_pdb->p_glb_pdb, 
                        temp_len+1,
                        ICF_MEM_COMMON, 
                        (p_rgm_context->auth_data_sent.p_nonce),
                        ICF_RET_ON_FAILURE, p_crm_pdb->p_ecode, ret_val)
                        
        ICF_ASSERT(p_ssa_pdb->p_glb_pdb, 
                        (ICF_NULL !=
			 p_rgm_context->auth_data_sent.p_nonce))

        icf_port_strcpy(p_rgm_context->auth_data_sent.p_nonce,
                                    p_rgm_context->auth_data_recd.p_nonce);
        p_rgm_context->auth_data_sent.nonce_len = temp_len;
        
        /* Copy the value of scheme */
        icf_port_memcpy((icf_void_t*)(&(
			p_rgm_context->auth_data_sent.scheme)),
            (icf_void_t *)(&(p_rgm_context->auth_data_recd.scheme)),
            ICF_PORT_SIZEOF(icf_string_st));
        
        /* Copy the value of algo */                
        icf_port_memcpy((icf_void_t*)(&(
			p_rgm_context->auth_data_sent.algo)),
            (icf_void_t *)(&(p_rgm_context->auth_data_recd.algo)),
            ICF_PORT_SIZEOF(icf_string_st));
        
        /* Copy the realm value */
        icf_port_memcpy((icf_void_t*)(&(
		p_rgm_context->auth_data_sent.realm)),
            (icf_void_t *)(&(p_rgm_context->auth_data_recd.realm)),
            ICF_PORT_SIZEOF(icf_string_st));
        
        /* Set the value for URI */
        icf_port_strcpy(p_rgm_context->auth_data_sent.uri.str,
            (icf_uint8_t *)"sip:");
        icf_port_strcat(p_rgm_context->auth_data_sent.uri.str,
            p_rgm_context->auth_data_sent.realm.str);
        p_rgm_context->auth_data_sent.uri.str_len = 
            (icf_uint16_t) icf_port_strlen(p_rgm_context->auth_data_sent.uri.str);
        
        /* Copy the Opaque value */
        icf_port_memcpy((icf_void_t*)(&(
			p_rgm_context->auth_data_sent.opaque)),
            (icf_void_t *)(&(p_rgm_context->auth_data_recd.opaque)),
            ICF_PORT_SIZEOF(icf_string_st));

        /* Setting the bitmask same as auth_data_recd */
        p_rgm_context->auth_data_sent.auth_data_present =
            p_rgm_context->auth_data_recd.auth_data_present;

        p_rgm_context->auth_data_sent.auth_data_present |=
                    ICF_AUTH_URI_PRESENT;
                
	 if (ICF_AUTH_QOP_PRESENT & 
                p_rgm_context->auth_data_sent.auth_data_present)
        {
            p_rgm_context->auth_data_sent.nonce_count = temp_nc;
            p_rgm_context->auth_data_sent.auth_data_present |=                    
                    ICF_AUTH_NC_PRESENT ;
            
            /* QoP */
            if (ICF_NULL != p_rgm_context->auth_data_recd.qop.str_len)
            {
            	icf_port_strcpy(p_rgm_context->auth_data_sent.qop.str,
                	p_rgm_context->auth_data_recd.qop.str);
            }
            else
            {
            	icf_port_strcpy(p_rgm_context->auth_data_sent.qop.str,
                	(icf_uint8_t *)"auth");
            }
            p_rgm_context->auth_data_sent.qop.str_len =
                (icf_uint16_t)icf_port_strlen(p_rgm_context->auth_data_sent.qop.str);
           
			/* Client nonce will be generated afresh for each challenge. 
             * For the first challenge, it will be generated here in REGM. 
             * For subsequent authentications, it will be generated by SSA 
             * and copied in p_rgm_context->auth_data_sent.cnonce
             */
            if ((ICF_NULL == 
					p_rgm_context->auth_data_sent.cnonce.str_len) &&
				(ICF_NULL == 
					(ICF_RGM_ONCE_REGISTERED & p_rgm_context->indicator)))
            {
        		/* random value */
        		new_cnonce_int = (icf_uint32_t)icf_port_random(
                                	(icf_uint16_t)1,
                                	(icf_uint16_t)rgm_duration)*rgm_duration;

                icf_port_sprintf((icf_int8_t *) p_rgm_context->auth_data_sent.cnonce.str,
					"%u", new_cnonce_int); 
                p_rgm_context->auth_data_sent.cnonce.str_len =
                    (icf_uint16_t)icf_port_strlen(p_rgm_context->auth_data_sent.cnonce.str);
                p_rgm_context->auth_data_sent.auth_data_present |=
                    ICF_AUTH_CNONCE_PRESENT ;
            }
			else
			{
				icf_port_memcpy(
					&p_rgm_context->auth_data_sent.cnonce,
					&p_rgm_context->client_nonce,
					ICF_PORT_SIZEOF(icf_string_st));
                p_rgm_context->auth_data_sent.auth_data_present |=
                    ICF_AUTH_CNONCE_PRESENT;
			}

        }/* QoP related parameters filled*/
      
        
    }/* scheme/algo support ends */
        
    /* Function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)

    return ret_val;    

}/* Function icf_rgm_non_ims_auth_process() ends here */
#ifdef IMS_CLIENT
/**********************************************************************
 *
 * FUNCTION:      icf_rgm_ims_auth_process
 * 
 * DESCRIPTION:   This function processes the IMS authorization related
 *                information present in auth_data_recd/sent. No processing
 *                is done if the sent_nonce == recd_nonce, 
 *                   
 * RETURNS:       ICF_SUCCESS: RES or AUTS filled in auth_data_sent
 *                ICF_FAILURE: scheme/algo not supported
 *                nonce not proper
 *                MAC failure
 *                system failure from NM/AKA running: error_cause NULL
 *                                    
 *
 ****************************************************************************/
icf_return_t    icf_rgm_ims_auth_process(
            INOUT icf_crm_pdb_st            *p_crm_pdb)

{
    icf_result_t ret_val = ICF_SUCCESS;

    icf_rgm_context_st    *p_rgm_context = ICF_NULL;

    icf_rgm_nm_info_st*    p_rgm_nm_info = ICF_NULL;
    icf_uint32_t            temp_len = ICF_NULL;
    icf_return_t            dummy_ret_val = ICF_SUCCESS;
    icf_uint8_t            nonce_status = ICF_RGM_INVALID_CHALLENGE;

    
    /* Function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Get local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;

    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
            (ICF_NULL != p_rgm_context))
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
        (ICF_NULL != p_crm_pdb->p_config_data))

    ICF_MEMGET(p_crm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_rgm_nm_info_st),
            ICF_MEM_COMMON, 
            (icf_void_t*)(p_rgm_nm_info),
            ICF_RET_ON_FAILURE, 
            p_crm_pdb->p_ecode, ret_val)
        
    icf_port_memset((icf_void_t *)(p_rgm_nm_info),0,
        ICF_PORT_SIZEOF(icf_rgm_nm_info_st));
    /* Fetch Data from NM */
    p_rgm_nm_info->options_flag = ICF_NULL;
    p_rgm_nm_info->options_flag |= ICF_RGM_NM_AKA_INFO;
    
    p_rgm_nm_info->p_access_info =    ICF_NULL;
    
    p_rgm_nm_info->p_crm_pdb = p_crm_pdb;
    p_rgm_nm_info->p_rgm_context = p_rgm_context;
    p_rgm_nm_info->ecode = ICF_NULL;
    p_rgm_nm_info->status = ICF_FAILURE;
    

    if (ICF_NULL != icf_port_strcasecmp(
            p_rgm_context->auth_data_recd.scheme.str,
            "Digest")) 
    {
        ret_val = ICF_FAILURE;
        p_crm_pdb->error_cause = ICF_CAUSE_SCHEME_NOT_SUPPORTED;
        
    }
    if ((ICF_SUCCESS == ret_val) &&
            (ICF_NULL != icf_port_strcasecmp(
                p_rgm_context->auth_data_recd.algo.str,
            "AKAv1-MD5")))
    {
        ret_val = ICF_FAILURE;
        p_crm_pdb->error_cause = ICF_CAUSE_ALGO_NOT_SUPPORTED;
    }
    

    if (ICF_SUCCESS == ret_val)
    {/* scheme/algo support begins */            
        if (p_rgm_context->auth_data_sent.p_nonce)
        {
            if (p_rgm_context->auth_data_recd.p_nonce)
            {
                if (ICF_NULL == icf_port_strcmp(
                       p_rgm_context->auth_data_recd.p_nonce,
                        p_rgm_context->auth_data_sent.p_nonce))
                {
                    nonce_status = ICF_RGM_SAME_NONCE;
                }/* recd_nonce == sent_nonce */            
                else
                {
                    nonce_status = ICF_RGM_FRESH_NONCE ;

                }/* recd_nonce != sent_nonce */
            }/* non null recd_nonce*/
            else
            {
                nonce_status = ICF_RGM_INVALID_CHALLENGE;
            }
        }/* Non null nonce present in auth_sent */
        else 
        {
            if (p_rgm_context->auth_data_recd.p_nonce)
            {
                nonce_status = ICF_RGM_FIRST_NONCE;            
            }
            else
            {
                nonce_status = ICF_RGM_INVALID_CHALLENGE;
            }
        }/* auth_sent nonce is NULL */

        if (ICF_RGM_INVALID_CHALLENGE == nonce_status)
        {
            ret_val = ICF_FAILURE;
        }
        else if (ICF_RGM_FIRST_NONCE == nonce_status ||
                        ICF_RGM_FRESH_NONCE == nonce_status)
        {
            /* Clear old contents of auth_data_sent */

            ICF_MEMFREE(p_crm_pdb->p_glb_pdb, (icf_uint8_t *)
                (p_rgm_context->auth_data_sent.p_nonce),
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode, dummy_ret_val)

            icf_port_memset(
                (icf_void_t*)&(p_rgm_context->auth_data_sent),
                (icf_uint8_t)ICF_NULL,
                (icf_uint32_t) 
                ICF_PORT_SIZEOF(icf_rgm_auth_data_st));
            
            
            
            /* Copy the value of nonce */
            temp_len = p_rgm_context->auth_data_recd.nonce_len;
            
            ICF_MEMGET(p_crm_pdb->p_glb_pdb, 
                temp_len+1,
                ICF_MEM_COMMON, (icf_void_t*)\
                ((p_rgm_context->auth_data_sent.p_nonce)),
                ICF_RET_ON_FAILURE, p_crm_pdb->p_ecode, ret_val)
                
            ICF_ASSERT(p_ssa_pdb->p_glb_pdb, 
                (ICF_NULL != p_rgm_context->auth_data_sent.p_nonce))
                
            icf_port_strcpy(p_rgm_context->auth_data_sent.p_nonce,
                p_rgm_context->auth_data_recd.p_nonce);
            p_rgm_context->auth_data_sent.nonce_len = temp_len;
            
            /* Copy the value of scheme */
            icf_port_memcpy((icf_void_t*)
                        (&(p_rgm_context->auth_data_sent.scheme)),
                (icf_void_t *)(&(p_rgm_context->auth_data_recd.scheme)),
                ICF_PORT_SIZEOF(icf_string_st));
            
            /* Copy the value of algo */                
            icf_port_memcpy((icf_void_t*)
                    (&(p_rgm_context->auth_data_sent.algo)),
                (icf_void_t *)(&(p_rgm_context->auth_data_recd.algo)),
                ICF_PORT_SIZEOF(icf_string_st));
            
            /* Copy the realm value */
            icf_port_memcpy((icf_void_t*)
                    (&(p_rgm_context->auth_data_sent.realm)),
                (icf_void_t *)(&(p_rgm_context->auth_data_recd.realm)),
                ICF_PORT_SIZEOF(icf_string_st));
            
            /* Set the value for URI */
            icf_port_strcpy(p_rgm_context->auth_data_sent.uri.str,
                "sip:");
            icf_port_strcat(p_rgm_context->auth_data_sent.uri.str,
                p_rgm_context->auth_data_sent.realm.str);
            p_rgm_context->auth_data_sent.uri.str_len = 
                icf_port_strlen(p_rgm_context->auth_data_sent.uri.str);
            
            /* Setting the bitmask for auth_data_sent */            
            
            p_rgm_context->auth_data_sent.auth_data_present |=
                        ICF_AUTH_URI_PRESENT        |
                        ICF_AUTH_SCHEME_PRESENT    |
                        ICF_AUTH_ALGO_PRESENT    |
                        ICF_AUTH_REALM_PRESENT    |
                        ICF_AUTH_NONCE_PRESENT;

            /* New challenge begins */
            if (ICF_FAILURE == icf_rgm_extract_rand_autn(
                p_crm_pdb->p_glb_pdb,
                p_rgm_context->auth_data_recd.p_nonce,
                &(p_rgm_nm_info->rand), 
                &(p_rgm_nm_info->autn),
                p_crm_pdb->p_ecode))
            {
            /* Invoke function to extract the RAND and AUTN from
                * Nonce/ Challenge received  */

                /* Nonce len smaller than expected */
                ret_val = ICF_FAILURE;
                p_crm_pdb->error_cause = 
                        ICF_CAUSE_PROPER_NONCE_NOT_RECD;
                
            }
            else if (ICF_FAILURE == icf_rgm_fetch_nm_info(
                    p_crm_pdb->p_glb_pdb,
                    p_rgm_nm_info, p_crm_pdb->p_ecode))
            {     
                
            /* Invoke NM to calculate the response for the nonce
             * that has been received: FAILURE received when
             * none of RES/AUTS or
             * MAC failure detected: AKA processing failed:*/
                
        
                ret_val = ICF_FAILURE;    
                p_crm_pdb->error_cause = 
                    ICF_CAUSE_INSUFFICIENT_RESOURCES;
                    
            }/* NM failed : Processing Ends */
            
            else
            {
                /* Fetching data from NM succesful: Processing Begins */
                if (ICF_CAUSE_SQN_OUT_OF_RANGE == p_rgm_nm_info->ecode)
                {
                    /* NM returned SUCCESS but AUTS to be used */
                    p_rgm_context->auth_data_sent.auts.str_len = 
                            p_rgm_nm_info->auts.str_len;
                    icf_port_strcpy(p_rgm_context->auth_data_sent.auts.str,
                                    p_rgm_nm_info->auts.str);
                    p_rgm_context->auth_data_sent.auth_data_present |=
                                            ICF_AUTH_AUTS_PRESENT;
                    p_rgm_context->auth_data_sent.auth_data_present &=
                                                ~ICF_AUTH_RES_PRESENT;
                    p_crm_pdb->error_cause = ICF_NULL;
                }
                else if (ICF_CAUSE_HOME_NETWORK_NOT_AUTH ==
                        p_rgm_nm_info->ecode)
                {
                    p_rgm_context->auth_data_sent.auth_data_present &=
                                                ~ICF_AUTH_RES_PRESENT;
                    p_rgm_context->auth_data_sent.auth_data_present &=
                                            ~ICF_AUTH_AUTS_PRESENT;
                    p_crm_pdb->error_cause = ICF_CAUSE_HOME_NETWORK_NOT_AUTH;

                }
                else

                {
                    p_rgm_context->auth_data_sent.res.str_len = 
                                p_rgm_nm_info->res.str_len;
                    icf_port_strcpy(p_rgm_context->auth_data_sent.res.str,
                                    p_rgm_nm_info->res.str);
                    p_rgm_context->auth_data_sent.auth_data_present |=
                                            ICF_AUTH_RES_PRESENT;
                    p_rgm_context->auth_data_sent.auth_data_present &=
                                            ~ICF_AUTH_AUTS_PRESENT;
                    p_crm_pdb->error_cause = ICF_NULL;
                
                    icf_port_memcpy((icf_void_t *)(&(p_rgm_context->enc_key)),
                        (icf_void_t *)(&(p_rgm_nm_info->imsCk)),
                        (icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));
                    
                    icf_port_memcpy((icf_void_t *)(&(p_rgm_context->auth_passwd)),
                        (icf_void_t *)(&(p_rgm_nm_info->imsIk)),
                        (icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));
                }
            }/* Fetching data from NM succesful: Processing Ends */
        }/* New challenge processing ends*/
        else
        {
            /* SAME_NONCE status: nothing to be done */
            ret_val = ICF_SUCCESS;
        }
            
    }/* scheme/algo support ends */

    /* Memfree rgm_nm_info  */
    ICF_MEMFREE(p_crm_pdb->p_glb_pdb, (icf_uint8_t *)
                    (p_rgm_nm_info),
                    ICF_MEM_COMMON,
                    p_crm_pdb->p_ecode, dummy_ret_val)

        /* Function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)

    return ret_val;


}/* Function icf_rgm_ims_auth_process() ends here  */
#endif
/*******************************************************************
 *
 * FUNCTION:    icf_rgm_update_auth_info
 * 
 * DESCRIPTION: This function updates auth_info for rgm_context kept 
 *              in crm_pdb.
 *              If the rgm_context is fresh then the auth_data is 
 *              initialized. Incase it is an old rgm_context, 
 *              then sent_auth_data is 
 *              updated for the new set of recd_auth_data. 
 *              Also the auth_info_present() function is invoked to
 *              make the
 *              last line of the array in rgm_context.
 *              The AUTH_SET bit is set for according to the 
 *              scenario: IMS or NON IMS
 *                  
 * RETURNS:     ICF_SUCCESS: .
 *              ICF_FAILURE: init_auth or process_auth() functions
 *               set the error_cause. NULL in system error.
 ************************************************************************/
icf_return_t    icf_rgm_update_auth_info(
            INOUT icf_crm_pdb_st            *p_crm_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_uint32_t                res_flag = ICF_NULL;
    icf_uint32_t                auts_flag = ICF_NULL;
    icf_uint32_t                qop_flag = ICF_NULL;
    icf_uint32_t                nonims_flag = ICF_NULL;
    icf_uint32_t                nonopaque_flag = ICF_NULL;
    icf_uint32_t                nonOpaqueQop_flag = ICF_NULL;
    icf_uint32_t                cnonceQopNonOpaque = ICF_NULL;
    icf_uint32_t                cnonceQop_flag = ICF_NULL;

    /* Function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                    (ICF_NULL != p_crm_pdb->p_rgm_context))
            
    p_rgm_context = p_crm_pdb->p_rgm_context;


    if (ICF_FAILURE == icf_rgm_auth_info_present(p_crm_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {/* Auth_info_present SUCCESS: begins :
        last line has the auth_key: IMS and auth_key/passwd: NON IMS */

        /* reset the AUTH_SET bit. */
        p_rgm_context->indicator &= ~ICF_RGM_AUTH_SET;
        
        if (p_rgm_context->auth_data_recd.auth_data_present & 
                ICF_AUTH_SCHEME_PRESENT)
        {
            /* SCHEME is present: challenge from network is there : */
            /* challenge processing: begins */

            p_crm_pdb->error_cause = ICF_NULL;

            if (ICF_FAILURE == icf_rgm_process_auth_data(p_crm_pdb))
            {/* Failure while processing auth_data: begins */
                ret_val = ICF_FAILURE;
            }/* Failure while processing auth_data: ends */
            else
            {
                /* To set the bit in indicator to be used by SSA */
                if (p_crm_pdb->p_config_data->auth_type
                                == ICF_NONIMS_AUTH_OPTION)
                {
                    /* The auth related members with no QoP */
                    nonims_flag = (ICF_AUTH_NONCE_PRESENT |
                            ICF_AUTH_SCHEME_PRESENT |
                            ICF_AUTH_ALGO_PRESENT    |                    
                            ICF_AUTH_REALM_PRESENT   |
                            ICF_AUTH_OPAQUE_PRESENT  |
                            ICF_AUTH_URI_PRESENT);

                    /* Since Opaque is not mandatory */
                    nonopaque_flag = (ICF_AUTH_NONCE_PRESENT |
                            ICF_AUTH_SCHEME_PRESENT |
                            ICF_AUTH_ALGO_PRESENT    |                    
                            ICF_AUTH_REALM_PRESENT   |                            
                            ICF_AUTH_URI_PRESENT);


                    /* The auth related members with yes QoP */
                    qop_flag = (ICF_AUTH_NONCE_PRESENT |
                            ICF_AUTH_SCHEME_PRESENT |
                            ICF_AUTH_ALGO_PRESENT    |                    
                            ICF_AUTH_REALM_PRESENT   |
                            ICF_AUTH_OPAQUE_PRESENT  |
                            ICF_AUTH_URI_PRESENT     |
                            ICF_AUTH_QOP_PRESENT     |
                            ICF_AUTH_NC_PRESENT);
                    /* The auth related members with yes QoP: 
                        yes client nonce */
                    cnonceQop_flag = (ICF_AUTH_NONCE_PRESENT |
                            ICF_AUTH_SCHEME_PRESENT |
                            ICF_AUTH_ALGO_PRESENT    |                    
                            ICF_AUTH_REALM_PRESENT   |
                            ICF_AUTH_OPAQUE_PRESENT  |
                            ICF_AUTH_URI_PRESENT     |
                            ICF_AUTH_QOP_PRESENT     |
                            ICF_AUTH_NC_PRESENT      |
                            ICF_AUTH_CNONCE_PRESENT);

                    /* Since Opaque is not mandatory : qop option*/
                    nonOpaqueQop_flag = (ICF_AUTH_NONCE_PRESENT |
                            ICF_AUTH_SCHEME_PRESENT |
                            ICF_AUTH_ALGO_PRESENT    |                    
                            ICF_AUTH_REALM_PRESENT   |                            
                            ICF_AUTH_URI_PRESENT     |
                            ICF_AUTH_QOP_PRESENT     |
                            ICF_AUTH_NC_PRESENT);

                    /* The auth related members with yes QoP: 
                        yes client nonce, noOpaque */
                    cnonceQopNonOpaque = (ICF_AUTH_NONCE_PRESENT |
                            ICF_AUTH_SCHEME_PRESENT |
                            ICF_AUTH_ALGO_PRESENT    |                    
                            ICF_AUTH_REALM_PRESENT   |
                            ICF_AUTH_URI_PRESENT     |
                            ICF_AUTH_QOP_PRESENT     |
                            ICF_AUTH_NC_PRESENT      |
                            ICF_AUTH_CNONCE_PRESENT);

                    if ((nonims_flag ==
                        p_rgm_context->auth_data_sent.auth_data_present)||
                        (qop_flag == 
                        p_rgm_context->auth_data_sent.auth_data_present) ||
                        (nonopaque_flag ==
                        p_rgm_context->auth_data_sent.auth_data_present) ||
                        (nonOpaqueQop_flag ==
                        p_rgm_context->auth_data_sent.auth_data_present) ||
                        (cnonceQop_flag ==
                        p_rgm_context->auth_data_sent.auth_data_present) ||
                        (cnonceQopNonOpaque ==
                         p_rgm_context->auth_data_sent.auth_data_present))                            
                    {
                        
                        p_rgm_context->indicator |= 
                            ICF_RGM_AUTH_SET;
                        
                    }
                }/* Non IMS option ends */
                else
                {
                    res_flag = ICF_AUTH_NONCE_PRESENT |
                            ICF_AUTH_SCHEME_PRESENT |
                            ICF_AUTH_ALGO_PRESENT    |
                            ICF_AUTH_REALM_PRESENT   |
                            ICF_AUTH_RES_PRESENT        |
                            ICF_AUTH_URI_PRESENT;
                    
                    auts_flag = ICF_AUTH_NONCE_PRESENT |
                            ICF_AUTH_SCHEME_PRESENT |
                            ICF_AUTH_ALGO_PRESENT    |
                            ICF_AUTH_REALM_PRESENT   |
                            ICF_AUTH_AUTS_PRESENT    |
                            ICF_AUTH_URI_PRESENT;

                    if (res_flag == p_rgm_context->
                            auth_data_sent.auth_data_present) 
                    {
                        p_rgm_context->indicator |= 
                                ICF_RGM_AUTH_SET;

                    }
                            
                    else if (auts_flag == p_rgm_context->
                            auth_data_sent.auth_data_present)
                    {
                        p_rgm_context->indicator |= ICF_RGM_AUTH_SET;
                    }
                            
                }/* IMS option ends */

            }/* bit setting for SSA: ends */

        }/* challenge processing: ends */

        else
        {/* auth_data_recd is empty: so fresh auth params to be put */
            /* fresh auth params: begin */

            if (ICF_FAILURE == icf_rgm_init_auth_data(p_crm_pdb))
            {
                /* system error: basic auth 
                 * related parameters not configured */
                ret_val = ICF_FAILURE;
            }
            else            
            {
                /* Nothing to be done */
                
            }

        }/* fresh auth params: end */

    }/* Auth_info_present SUCCESS: ends */

    /* Function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_update_auth_info(p_crm_pdb) ends here */



#ifdef IMS_CLIENT

/**********************************************************************

 *
 * FUNCTION:        icf_rgm_exists_uri
 * 
 * DESCRIPTION:     This function checks whether the address_st parameter 
 *                    passed is present in rgm_context or not.
 *                  This function can only be used internally by
 *                    REGM because the uri is also compared with
 *                    rgm_user_address. To maitain the URI used in TO/From
 *                    of REGISTER message
 *                    
 * RETURNS:         ICF_SUCCESS: .
 *                    ICF_FAILURE: 
 **************************************************************************/
icf_return_t    icf_rgm_exists_uri(
            INOUT icf_glb_pdb_st            *p_glb_pdb,
            INOUT icf_rgm_context_st        *p_rgm_context,
            INOUT icf_address_st            *p_uri,
            OUT   icf_boolean_t             *p_status,
            INOUT icf_error_t               *p_ecode)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_user_db_node_st *p_node = ICF_NULL;

    /* Function entry trace */
    ICF_FUNCTION_ENTER(p_glb_pdb)

    *p_status = ICF_FALSE;
    if(ICF_TRUE == icf_dbm_util_cmp_addr(p_glb_pdb,
        &(p_rgm_context->rgm_user_address),
        p_uri)
        )
    {
        /* found */
        *p_status = ICF_TRUE;
    }

    /* Get the public id node corresponding to the rgm_user addr */
    else if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line (
        p_glb_pdb,
        ICF_USE_LINE1,
        &(p_rgm_context->rgm_user_address),
        ICF_USER_DB_USE_PUBLIC_URIS, /* searching only public ids */
        &p_node,
        p_ecode))
    {
        if(ICF_SUCCESS == icf_dbm_search_address_list(
            p_glb_pdb,
            p_node->p_assoc_uri_list,
            p_uri,
	    ICF_NULL))
        {
            /* found */
            *p_status = ICF_TRUE;
        }
    }

    /* Function exit trace */
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}/* icf_rgm_exists_uri() : Ends here  */    


#endif
/****************************************************************
 *
 * FUNCTION:        icf_rgm_update_line_info_on_succ
 * 
 * DESCRIPTION:     This function updates the lines' data on 200
 
 *                   
 * RETURNS:         ICF_SUCCESS: .
 *                    ICF_FAILURE: 
 *******************************************************************/
icf_return_t    icf_rgm_update_line_info_on_succ(
            INOUT icf_glb_pdb_st            *p_glb_pdb,
            INOUT icf_rgm_context_st        *p_rgm_context,
            INOUT icf_error_t            *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;

    icf_uint32_t                loop_lines = ICF_NULL;

    icf_line_data_st            *p_line_data = ICF_NULL;
    icf_line_data_st            *p_line_data_with_auth = ICF_NULL;

    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    
     if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_glb_pdb,p_rgm_context->rgm_list_of_lines[
                            p_rgm_context->num_of_lines -1],
                    (icf_line_data_st **)(&p_line_data_with_auth),
                    p_ecode))
     {
         ret_val = ICF_FAILURE;

     }
     else
     {     /* Last line data captured: */
        for (loop_lines = 0; loop_lines < p_rgm_context->num_of_lines;
                                                    loop_lines++)
        {
            if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                            p_glb_pdb,p_rgm_context->
                                rgm_list_of_lines[loop_lines],
                            (icf_line_data_st **)(&p_line_data),
                            p_ecode))
            {
                /* Not to break if the line data is deleted 
                 * without removing the 
                 * line id from rgm_ctxt: Which is not happening as line data
                 * is deleted only after deregistration */
                ret_val = ICF_FAILURE;
                break;
            }
            else    
            {
                ret_val = icf_rgm_update_line_data(p_glb_pdb, p_rgm_context,
                                    p_line_data, p_line_data_with_auth,p_ecode);
                if (ICF_FAILURE == ret_val)
                    break;
            }
        }/* Loop on lines ends */
     }/* Last line data captured: ends*/

         /* Function exit trace */
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;

}/* icf_rgm_update_line_info_on_succ() ends here */

#ifdef IMS_CLIENT
/************************************************************************
 *
 * FUNCTION:        icf_rgm_send_subs_on_succ
 * 
 * DESCRIPTION:     This function triggers SIC to send SUBSCRIBE on 200 OK
 
 *                   
 * RETURNS:         ICF_SUCCESS: .
 *                    ICF_FAILURE: 
 **************************************************************************/
icf_return_t    icf_rgm_send_subs_on_succ(
            INOUT icf_crm_pdb_st       *p_crm_pdb,
            INOUT icf_error_t            *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;

    icf_sic_subs_req_st		sub_req; 
    icf_internal_msg_st		sic_msg;
	icf_address_st           *p_default_uri;

    icf_port_memset((icf_void_t*)&sub_req, 0, 
        ICF_PORT_SIZEOF(icf_sic_subs_req_st));

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* send a trigger to SIC module to send SUBSCRIBE */
        
        sub_req.app_id = ICF_DUMMY_APP_ID ; 
        /* logic to assign call-id may be modified */
        sub_req.call_id =  ICF_REG_EVENT_SUBS_CALL_ID; 
/* Start: Multiple Public Users ICF5.0*/  
		if(ICF_SUCCESS == icf_dbm_get_default_uri_for_line(p_crm_pdb->p_glb_pdb,
		ICF_USE_LINE1, &p_default_uri,p_ecode))
		{
		
			icf_port_memcpy(
				&(sub_req.destination_info),
				p_default_uri,
				sizeof(icf_address_st) );

	/*SPR # 8944 Fix Start */
			sub_req.duration = 600000; 
	/*SPR # 8944 Fix End */
	/*        sub_req.event_package = icf_EVENT_PKG_REG; */

			icf_port_strcpy( sub_req.event_package.str, "reg" );

			sub_req.header_list.count = 0; 
			sub_req.line_id = ICF_USE_LINE1;
    
			ICF_MEMGET(p_crm_pdb->p_glb_pdb, sizeof(icf_reg_event_subs_ctx_st), 
				ICF_MEM_COMMON, 
				(((icf_rgm_glb_data_st *)p_crm_pdb->p_rgm_glb_data)->p_reg_event_subs_ctx), 
				ICF_RET_ON_FAILURE, p_ecode, ret_val)

			sub_req.p_key = ((icf_rgm_glb_data_st *)p_crm_pdb->p_rgm_glb_data)->p_reg_event_subs_ctx; 
			icf_port_memcpy(
				&(sub_req.pref_identity),
				p_default_uri,
				sizeof(icf_address_st) );
			sub_req.tag_list.count = 0; 

			sic_msg.msg_hdr.msg_id = ICF_SIC_SUBSCRIBE_REQ;
			sic_msg.msg_hdr.ecode = 0; 
			sic_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb; 
			sic_msg.msg_hdr.payload_length = ICF_PORT_SIZEOF(
					icf_sic_subs_req_st); 
			sic_msg.msg_hdr.status = ICF_NULL; 
			sic_msg.p_msg_data = (icf_uint8_t*)&sub_req;

			ret_val = icf_sic_process_mesg(&sic_msg);
	/* End: Multiple Public Users ICF5.0*/
		}

		/* Function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* icf_rgm_send_subs_on_succ()*/


/***********************************************************************
 *
 * FUNCTION:        icf_rgm_send_clear_subs_on_dereg
 * 
 * DESCRIPTION:     This function triggers SIC to clear context on De-reg.
 
 *                   
 * RETURNS:         ICF_SUCCESS: .
 *                    ICF_FAILURE: 
 ************************************************************************/
icf_return_t    icf_rgm_send_clear_subs_on_dereg(
			INOUT icf_crm_pdb_st         *p_crm_pdb,
            INOUT icf_error_t            *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;

    icf_sic_clear_ctx_st		sub_req; 
    icf_internal_msg_st			sic_msg;

    icf_port_memset((icf_void_t*)&sub_req, 0, 
            ICF_PORT_SIZEOF(icf_sic_clear_ctx_st));

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* send a trigger to SIC module to send SUBSCRIBE */
        
    sub_req.app_id = ICF_DUMMY_APP_ID; 
    /* logic to assign call-id may be modified */
    sub_req.call_id =  ICF_REG_EVENT_SUBS_CALL_ID; 
    icf_port_strcpy( sub_req.event_package.str, "reg" );
    sub_req.p_key = ICF_NULL; 

    sic_msg.msg_hdr.msg_id = ICF_CLEAR_SIC_CTX;
    sic_msg.msg_hdr.ecode = 0; 
    sic_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb; 
    sic_msg.msg_hdr.payload_length = ICF_PORT_SIZEOF(
            icf_sic_clear_ctx_st); 
    sic_msg.msg_hdr.status = ICF_NULL; 
    sic_msg.p_msg_data = (icf_uint8_t*)&sub_req;

    icf_sic_process_mesg(&sic_msg);
#ifndef ICF_5_UT
	ICF_MEMFREE(p_crm_pdb->p_glb_pdb, 
            ((icf_rgm_glb_data_st *)p_crm_pdb->p_rgm_glb_data)->p_reg_event_subs_ctx,
            ICF_MEM_COMMON,
            p_ecode, ret_val)
#endif
             /* Function exit trace */
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* icf_rgm_send_clear_subs_on_dereg()*/

#endif
/************************************************************************
 *
 * FUNCTION:        icf_rgm_send_app_resp_on_succ
 * 
 * DESCRIPTION:     This function send CFM to app on 200
 
 *                   
 * RETURNS:         ICF_SUCCESS: .
 *                    ICF_FAILURE: 
 *************************************************************************/
icf_return_t icf_rgm_send_app_resp_on_succ(
            INOUT    icf_crm_pdb_st                *p_crm_pdb,
            INOUT    icf_rgm_register_resp_st      *p_register_response)
{

    icf_rgm_context_st                *p_rgm_context = ICF_NULL;
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_boolean_t                    valid_app = ICF_FALSE;

    

    icf_uint32_t                app_index = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

        p_register_response->status = ICF_SUCCESS;
        p_register_response->error_cause = ICF_NULL;
        p_register_response->request_type = ICF_NULL;
        /*for CFG to not send response*/

        p_crm_pdb->resp_to_be_sent = ICF_FALSE;
        p_rgm_context = p_crm_pdb->p_rgm_context;

        for (app_index = 0; app_index < 
                p_crm_pdb->p_config_data->max_applications;
                app_index++)
        {
            /* Response will be sent only to those applications that
            * are valid entries and they have not been given the CFM yet */

            if (ICF_FALSE == p_rgm_context->p_app_data_list[
                    app_index].is_valid)
            {
                continue;
            }
            
            /* Application on Probation need not be sent the CFM */
            if (ICF_IS_BIT_SET(p_rgm_context->p_app_data_list[
                            app_index].app_indicator,
                            ICF_RGM_APP_DEREG_ON_PROBATION))
            {
                    continue;
            }
            valid_app = ICF_TRUE;
            if (ICF_RGM_APP_CFM_SENT & 
                p_rgm_context->p_app_data_list[app_index].app_indicator)
            {
                continue;
            }
            p_register_response->options_flag |= ICF_RGM_RESPONSE_CFM;
            p_register_response->options_flag |= ICF_RGM_SINGLE_APP;
            if(ICF_NULL == p_register_response->app_id)
            {
                p_register_response->app_id = 
                    p_rgm_context->p_app_data_list[app_index].app_id;
                p_register_response->line_id = 
                    p_rgm_context->p_app_data_list[app_index].line_id;
            }


        







                p_register_response->options_flag |= ICF_RGM_CLONE_HEADERS;

            
        


            if (ICF_FAILURE == icf_rgm_send_response(
                    p_crm_pdb->p_glb_pdb,




                                p_register_response, p_crm_pdb->p_ecode))
            {
                ret_val = ICF_FAILURE;
                break;
            }

            /* Donot send further CFMs to this app. */
            p_rgm_context->
                    p_app_data_list[app_index].app_indicator |=
                                            ICF_RGM_APP_CFM_SENT;
                
        
        }/* For loop on applications ends here */
                /* Function exit trace */

        if (ICF_FALSE == valid_app &&
                    p_crm_pdb->p_config_data->auto_register)
        {
            ICF_PRINT(((icf_uint8_t *)"\n Auto Register Succeeded!! \n"));
        }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_send_app_resp_on_succ() ends here */


/*******************************************************************
 * FUNCTION:        icf_rgm_chk_app_details
 *
 * DESCRIPTION:     This function is checks whether the application
 *                  is added to an activated network and also it is 
 *                  a valid app_id.
 *
 * RETURNS:         FAILURE: 
 *                  SUCCESS:
 *******************************************************************/
icf_return_t        icf_rgm_chk_app_details(
                        INOUT    icf_crm_pdb_st        *p_crm_pdb,
                        INOUT    icf_app_id_t            app_id)
{
    icf_return_t        ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    if (ICF_DUMMY_APP_ID != app_id)
    {        
        if (ICF_FALSE == 
             icf_cfg_get_app_id_validity(p_crm_pdb->p_glb_pdb,app_id))
        {
            ret_val = ICF_FAILURE;
            p_crm_pdb->error_cause = ICF_CAUSE_INVALID_APP_ID;
        }
    }
#ifdef ICF_NW_ACTIVATE
    if (ICF_SUCCESS == ret_val)
    {
        if (ICF_FALSE == 
            icf_cfg_get_network_status(p_crm_pdb->p_glb_pdb,app_id))
        {
            ret_val = ICF_FAILURE;
            p_crm_pdb->error_cause = ICF_CAUSE_NW_CURRENTLY_NOT_AVAILABLE;
        }
    }
#endif
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;


}/* Function icf_rgm_chk_app_details() ends here */

#ifdef IMS_CLIENT
/************************************************************************
 * FUNCTION:        icf_rgm_parse_notify_body
 *
 * DESCRIPTION:     This function parse message body obtained
 *                  in notify message for reg event package
 *
 *************************************************************************/
icf_return_t icf_rgm_parse_notify_body(
            INOUT    icf_glb_pdb_st        *p_glb_pdb,
            INOUT    icf_rgm_reginfo_st    *p_reg_info,
            INOUT   icf_uint8_t            *p_buffer,
/* SPR 9044: fix_start*/
            INOUT    icf_uint32_t            buf_length,
/* SPR 9044: fix_end */
            OUT        icf_error_t            *p_ecode)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_rgm_glb_parse_reginfo_st        *p_glb_parse_reginfo = ICF_NULL;
        
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_glb_pdb) 

    icf_rgm_init_glb_parse_reginfo_block(p_glb_pdb,p_reg_info,
                                                &p_glb_parse_reginfo,p_ecode);
/* SPR 9044: fix_start*/
    /* Buffer length of the XML body 
    count = icf_port_strlen(p_buffer);*/
/* SPR 9044: fix_end*/

    p_buffer[buf_length] = '\0';
    

    if(ICF_FAILURE ==
        icf_cmn_parse_xml_body(p_buffer,buf_length,
            (icf_void_t*)(p_glb_parse_reginfo),p_ecode))
    {
        
        /* Free the buffer */
        icf_rgm_free_glb_parse_reginfo(p_glb_pdb,
                    p_glb_parse_reginfo,p_ecode);
        ret_val = ICF_FAILURE;

    }
    else
    {
        /* parsing succeded */
        
        ret_val = ICF_SUCCESS;
        
        /* We just need to free the memory taken by global reg info 
         * parse data: As for success case all memory taken by  temporary
         * memberes have been shifted to the parsed lists. 
         * In the calling function
         * all the lists of the regn blocks are deleted along 
         * with the including
         * contact lists */
           
        ICF_MEMFREE(p_glb_pdb, 
                    (icf_uint8_t *)p_glb_parse_reginfo,
                    ICF_MEM_COMMON,
                    p_ecode, ret_val)
    }
        
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;    

}/* Function icf_rgm_parse_notify_body() ends here */




/*************************************************************************
 *
 * FUNCTION:     icf_rgm_validate_rgm_info_version_state
 *
 * DESCRIPTION:  This is a utility function that validates the version
 *              number recd in NOTIFy with the one stored in rgm_context.
 *    
 * RETURNS:     Success: received version is > stored version
 *              Nothing stored, 0 received.
 *              Faiure: if 0 not received when no old reg_info stored.
 *               received version <= stored version 
 ***************************************************************************/
icf_return_t icf_rgm_validate_rgm_info_version_state(
        INOUT    icf_crm_pdb_st            *p_crm_pdb,
        INOUT    icf_rgm_reginfo_st        *p_reg_info)
{
    icf_return_t				ret_val = ICF_SUCCESS;
	icf_reg_event_subs_ctx_st	*p_reg_event_subs_ctx = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

/* SPR: 9068: fix starts */
    if (ICF_INVALID_REGINFO_VERSION == p_reg_info->version)
    {
        ret_val = ICF_FAILURE;
    }
	if (ICF_SUCCESS == ret_val)
    {
/* Start: Multiple Public Users ICF5.0*/
		p_reg_event_subs_ctx = ((icf_rgm_glb_data_st *)p_crm_pdb->p_rgm_glb_data)->p_reg_event_subs_ctx;
        
        if (!(ICF_RGM_NOTIFY_ONCE_RECD & p_reg_event_subs_ctx->indicator) && 
            (ICF_NULL != p_reg_info->version))
        {
            ret_val = ICF_FAILURE;
            
        }
        else if ((ICF_RGM_NOTIFY_ONCE_RECD & 
                p_reg_event_subs_ctx->indicator) &&
            (p_reg_event_subs_ctx->reg_info_version >= 
                    p_reg_info->version))
        {
            ret_val = ICF_FAILURE;
            
        }
/* End: Multiple Public Users ICF5.0*/
	}
/* SPR: 9068: fix ends */

    if (ICF_SUCCESS == ret_val)
    {
        /* Version valid: now check state: begin */
        if (ICF_RGM_NFY_REGINFO_STATE_INVALID == p_reg_info->reginfo_state)
        {
            ret_val = ICF_FAILURE;

        }
        else
        {
            ret_val = ICF_SUCCESS;            

        }

    }/* Version valid: now check state: ends */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_validate_rgm_info_version() ends here */

/**********************************************************************
 *
 * FUNCTION:    icf_rgm_handle_notify_regn_blk
 *
 * DESCRIPTION: This is a utility function that handles the regn_block
 *              obtained from the NOTIFY: reg_info for 
 *              the rgm_context present in crm_pdb. Note that the regn_blk is
 *              obtained after having matched the aor, or the id.
 *                    
 *    
 * RETURNS:     Success: 
 *              Faiure: 
 *************************************************************************/


icf_return_t  icf_rgm_handle_notify_regn_blk(
            INOUT    icf_crm_pdb_st                *p_crm_pdb,
            INOUT    icf_rgm_regn_block_st        *p_regn_blk)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                            (ICF_NULL != p_rgm_context))

    switch (p_regn_blk->state)
    {
        case ICF_RGM_NFY_REG_STATE_INIT:
            ret_val = icf_rgm_nfy_regn_init(p_crm_pdb, p_regn_blk);
            break;

        case ICF_RGM_NFY_REG_STATE_ACTIVE:
            ret_val = icf_rgm_nfy_regn_active(p_crm_pdb, p_regn_blk);
            break;

        case ICF_RGM_NFY_REG_STATE_TERMINATED:
            ret_val = icf_rgm_nfy_regn_terminated(p_crm_pdb, p_regn_blk);
            break;

        default:
            ret_val = ICF_FAILURE;
            break;

    }/* Switch case on state of regn_blk: ends */
    
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_handle_notify_regn_blk() ends here */



/************************************************************************
 *
 * FUNCTION:        icf_rgm_nfy_regn_init
 *
 * DESCRIPTION:     This is a utility function that handles the regn_block
 *                    obtained from the NOTIFY: when the state is INIT
 *                    
 *    
 * RETURNS:            Success: 
 *                             
 *
 *                    Faiure:                         
 *************************************************************************/
icf_return_t  icf_rgm_nfy_regn_init(
            INOUT    icf_crm_pdb_st                *p_crm_pdb, 
            INOUT    icf_rgm_regn_block_st        *p_regn_blk)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_rgm_context_st       *p_rgm_context = ICF_NULL;
	icf_reg_event_subs_ctx_st			*p_reg_event_subs_ctx = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
/* Start: Multiple Public Users ICF5.0*/
    p_reg_event_subs_ctx = ((icf_rgm_glb_data_st *)p_crm_pdb->p_rgm_glb_data)->p_reg_event_subs_ctx;
	
	ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                            (ICF_NULL != p_rgm_context))


    
    if (ICF_RGM_03_REGISTERED == p_rgm_context->rgm_state)
    {
        if (ICF_RGM_NOTIFY_ONCE_RECD & p_reg_event_subs_ctx->indicator)

        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            p_reg_event_subs_ctx->indicator |= ICF_RGM_NOTIFY_ONCE_RECD;
            icf_port_strcpy(p_rgm_context->notify_reg_id.str,
                                p_regn_blk->p_id);
            p_rgm_context->notify_reg_id.str_len = 
                                        icf_port_strlen(
                                        p_rgm_context->notify_reg_id.str);
        }
    }/* state == REGISTERED */
    else
    {
        ret_val = ICF_FAILURE;
    }
/* End: Multiple Public Users ICF5.0*/

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;


}/* Function icf_rgm_nfy_regn_init() ends here */

/***********************************************************************
 *
 * FUNCTION:        icf_rgm_nfy_regn_active
 *
 * DESCRIPTION:     This is a utility function that handles the regn_block
 *                    obtained from the NOTIFY: when the state is ACTIVE
 *                    
 *    
 * RETURNS:            Success: 
 *                             
 *
 *                    Faiure:                         
 *************************************************************************/
icf_return_t  icf_rgm_nfy_regn_active(
            INOUT    icf_crm_pdb_st                *p_crm_pdb, 
            INOUT    icf_rgm_regn_block_st        *p_regn_blk)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_rgm_contact_blk_st    *p_temp_contact_blk = ICF_NULL;
    icf_uint32_t                index_contact_blk = ICF_NULL;
    icf_boolean_t            valid_contact_found = ICF_FALSE;
	icf_reg_event_subs_ctx_st			*p_reg_event_subs_ctx = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
	p_reg_event_subs_ctx = ((icf_rgm_glb_data_st *)p_crm_pdb->p_rgm_glb_data)->p_reg_event_subs_ctx;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                            (ICF_NULL != p_rgm_context))
    
    /* Setting the initial values to be used by called functions */
    p_crm_pdb->new_retry_value = ICF_NULL;
    p_crm_pdb->recd_con_app_id = ICF_APP_ID_INVALID;
    p_crm_pdb->recd_con_event = ICF_RGM_NFY_CON_EVT_INVALID;
    p_crm_pdb->shortened_expiry = ICF_NULL;
	

    if (ICF_RGM_NOTIFY_ONCE_RECD & p_reg_event_subs_ctx->indicator)
    {
        if (p_regn_blk->contact_list.p_contact_blk)
        {
            p_temp_contact_blk = p_regn_blk->contact_list.p_contact_blk;
        }
        for (index_contact_blk = 0; 
                index_contact_blk < p_regn_blk->contact_list.count;
                index_contact_blk++)
        {
            if (!p_temp_contact_blk)
            {
                break;
            }
/* SPR: 9071: fix start */
                if (!(p_temp_contact_blk->p_contact_uri)||
                    !(p_temp_contact_blk->p_id)||
                    (ICF_RGM_NFY_CON_STATE_INVALID == 
                            p_temp_contact_blk->state))
                {
                    p_temp_contact_blk = p_temp_contact_blk->p_next;
                    continue;
                }
/* SPR: 9071: fix end */

            else
            {
                valid_contact_found = ICF_TRUE;
                
                switch (p_temp_contact_blk->state)
                {
                                    
                case ICF_RGM_NFY_CON_STATE_ACTIVE:
                    ret_val = icf_rgm_nfy_contact_active(
                            p_crm_pdb, p_temp_contact_blk);
                    break;
                    
                case ICF_RGM_NFY_CON_STATE_TERMINATED:
                    ret_val = icf_rgm_nfy_contact_terminated(
                        p_crm_pdb, p_temp_contact_blk);
                    break;                    
                default:
                    break;
                    
                }/* Switch case on state of contact_blk: ends */


                /* get the next contact element */
                p_temp_contact_blk = p_temp_contact_blk->p_next;
            }
        }/* for loop on contact list ends */    
        if (ICF_FALSE == valid_contact_found)
        {
            ret_val = ICF_FAILURE;
        }        
        else if (ICF_FAILURE == icf_rgm_ctx_handled_after_nfy(
                                        p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }/* post-all_contacts_processed_for nofity handling:  */    

    }/* old NOTIFY ID stored */
    else
    {
        ret_val = ICF_FAILURE;
    }


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_nfy_regn_active() ends here */


/**************************************************************************
 *
 * FUNCTION:        icf_rgm_nfy_regn_terminated
 *
 * DESCRIPTION:     This is a utility function that handles the regn_block
 *                    obtained from the NOTIFY: when the state is TERMINATED
 *                    
 *    
 * RETURNS:            Success: 
 *                             
 *
 *                    Faiure:                         
 ****************************************************************************/
icf_return_t  icf_rgm_nfy_regn_terminated(
            INOUT    icf_crm_pdb_st                *p_crm_pdb, 
            INOUT    icf_rgm_regn_block_st        *p_regn_blk)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_void_t                *p_buff = ICF_NULL;
    icf_rgm_glb_data_st        *p_rgm_glb_data = ICF_NULL;
	icf_reg_event_subs_ctx_st			*p_reg_event_subs_ctx = ICF_NULL;
    
   /* To remove warning */
   p_regn_blk=p_regn_blk;
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
	p_reg_event_subs_ctx = ((icf_rgm_glb_data_st *)p_crm_pdb->p_rgm_glb_data)->p_reg_event_subs_ctx;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                            (ICF_NULL != p_rgm_context))

    if (ICF_RGM_NOTIFY_ONCE_RECD & p_reg_event_subs_ctx->indicator)
    {
        /* Call DBM to get the pointer to the RGM global data in p_buff */
        ret_val = icf_dbm_get_module_glb_data(
                p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                &p_buff,
                p_crm_pdb->p_ecode);     

        ICF_ASSERT(
                p_crm_pdb->p_glb_pdb,
            (ICF_NULL != p_buff))
        
        /* Typecast into global data structure */
        p_rgm_glb_data = (icf_rgm_glb_data_st *)(p_buff);

        /* Set values to indicate the FSM handler 
         * about the state=terminated being received in 
         * <aor> element and not <contact> element */
        p_crm_pdb->recd_con_event = ICF_RGM_NFY_CON_EVT_INVALID;
        p_crm_pdb->recd_con_app_id = ICF_APP_ID_INVALID;

        /* Invoke the FSM handler with the TERMINATED event */
        /* Trigger FSM */
        ret_val = p_rgm_glb_data->icf_rgm_state_machine[
            p_rgm_context->rgm_state][
            ICF_RGM_NOTIFY_TERMINATED](p_crm_pdb);
    
    }
    else
    {
        ret_val = ICF_FAILURE;
    }


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_nfy_regn_terminated() ends here */


/**************************************************************************
 *
 * FUNCTION:      icf_rgm_nfy_contact_active
 *
 * DESCRIPTION:  This is a utility function that handles the contact_block
 *               obtained from the NOTIFY: when the contact_state is ACTIVE
 *    
 * RETURNS:      Success:
 *               Faiure:
 ***************************************************************************/


icf_return_t  icf_rgm_nfy_contact_active(
            INOUT    icf_crm_pdb_st                *p_crm_pdb, 
            INOUT    icf_rgm_contact_blk_st        *p_contact_blk)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_app_id_t                temp_app_id = ICF_NULL;
    icf_rgm_app_ctxt_st        *p_temp_app_ctx = ICF_NULL;
    icf_void_t                *p_buff = ICF_NULL;
    icf_rgm_glb_data_st        *p_rgm_glb_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                            (ICF_NULL != p_rgm_context))

    
    if (ICF_FAILURE != icf_rgm_nfy_get_app_id(
            p_crm_pdb, p_contact_blk, &temp_app_id))
    {
        /* If app present in rgm_context and event is SHORTENED 
         * the FSM handler will be called */
        if ((ICF_APP_ID_INVALID != temp_app_id)&&
            (ICF_RGM_NFY_CON_EVT_SHORTENED == p_contact_blk->event) &&
              (ICF_TRUE == icf_rgm_fetch_app_info(p_crm_pdb,p_rgm_context,
                               temp_app_id, &p_temp_app_ctx)))
        {
            if (!(ICF_RGM_NFY_CON_EXPIRES_PRESENT & p_contact_blk->bitmask))
            {/* event shortened but no expires value */
                ret_val = ICF_FAILURE;
            }
            else
            {
                /* store in milliseconds the expires 
                 * value obtained from NOTIFY  */
                p_crm_pdb->shortened_expiry = (p_contact_blk->expires * 1000);


                /* Call DBM to get the pointer to the 
                 * RGM global data in p_buff */
                ret_val = icf_dbm_get_module_glb_data(
                    p_crm_pdb->p_glb_pdb,
                    (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                    &p_buff,
                    p_crm_pdb->p_ecode);
                


                ICF_ASSERT(
                    p_crm_pdb->p_glb_pdb,
                    (ICF_NULL != p_buff))
                    
                    /* Typecast into global data structure */
                    p_rgm_glb_data = (icf_rgm_glb_data_st *)(p_buff);
                
                /* Invooke the FSM handler with the 
                ICF_RGM_NOTIFY_SHORTENED event */
                /* Trigger FSM */
                ret_val = p_rgm_glb_data->icf_rgm_state_machine[
                    p_rgm_context->rgm_state][
                    ICF_RGM_NOTIFY_SHORTENED](p_crm_pdb);
            }/* shortened with expires found: ends */

        }/* shortened and app found : ends */
        else
        {
            /* IP mismatch; or event != shortened or ;
            app not found in rgm_Context*/
            ret_val = ICF_SUCCESS;
        }

    }/* get_app_id returns SUCCESS: ends  */
    else
    {
        ret_val = ICF_FAILURE;
    }


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_nfy_contact_active() ends here */

/************************************************************************
 *
 * FUNCTION:     icf_rgm_nfy_contact_terminated
 *
 * DESCRIPTION:   This is a utility function that handles the contact_blk
 *              obtained from the NOTIFY: when the contact_state is TERMINATED
 *                    
 *    
 * RETURNS:      Success: 
 *               Faiure:  
 **************************************************************************/


icf_return_t  icf_rgm_nfy_contact_terminated(
            INOUT    icf_crm_pdb_st                *p_crm_pdb, 
            INOUT    icf_rgm_contact_blk_st        *p_contact_blk)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_app_id_t                temp_app_id;
    icf_void_t                *p_buff = ICF_NULL;
    icf_rgm_glb_data_st        *p_rgm_glb_data = ICF_NULL;
   


    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb,
                            (ICF_NULL != p_rgm_context))
    

    p_crm_pdb->recd_con_event = ICF_RGM_NFY_CON_EVT_INVALID;
    p_crm_pdb->recd_con_app_id = ICF_APP_ID_INVALID;

    if (ICF_RGM_03_REGISTERED != p_rgm_context->rgm_state)
        return ICF_FAILURE;
    
    if (ICF_FAILURE != icf_rgm_nfy_get_app_id(p_crm_pdb, 
        p_contact_blk, &temp_app_id))
    {
        if (ICF_APP_ID_INVALID ==
                temp_app_id)
        {
                ret_val = ICF_FAILURE;
        }
        else if ((ICF_DUMMY_APP_ID == temp_app_id) &&
                (ICF_TRUE != p_rgm_context->auto_reg_flag))
        {
            ret_val = ICF_FAILURE;
        }
        else if ((ICF_RGM_NFY_CON_EVT_PROBATION ==
                  p_contact_blk->event) &&
                  !(ICF_RGM_NFY_CON_RETRY_AFTER_PRESENT &
                  p_contact_blk->bitmask))
        {
            /* probation event cannot come without
                 * retry_after */
            ret_val = ICF_FAILURE;
        }
        else if (ICF_RGM_NFY_CON_EVT_INVALID !=
                p_contact_blk->event)
        {            
            if (ICF_RGM_NFY_CON_RETRY_AFTER_PRESENT &
                  p_contact_blk->bitmask)
            {                
                p_crm_pdb->new_retry_value =
                        (p_contact_blk->retry_after * 1000);
            }

            p_crm_pdb->recd_con_app_id = temp_app_id;            
            p_crm_pdb->recd_con_event =
                        p_contact_blk->event;
            /* Call DBM to get the pointer to the RGM global data
               in p_buff */
            ret_val = icf_dbm_get_module_glb_data(
                    p_crm_pdb->p_glb_pdb,
                    (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                    &p_buff,
                    p_crm_pdb->p_ecode);     
                
/* Start: Multiple Public Users ICF5.0*/               
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)

		p_crm_pdb->line_id = p_rgm_context->rgm_list_of_lines[0];
		
		icf_cfg_cm_call_clear_ind_wrt_app_user(
			p_crm_pdb,
			&(p_crm_pdb->p_rgm_context->rgm_user_address),
			temp_app_id,
			p_crm_pdb->p_ecode);
ICF_CHECK_IMS_END
#endif
/* End: Multiple Public Users ICF5.0*/                    

            ICF_ASSERT(
                    p_crm_pdb->p_glb_pdb,
                    (ICF_NULL != p_buff))
                    
            /* Typecast into global data structure */
            p_rgm_glb_data = (icf_rgm_glb_data_st *)(p_buff);
                
                    
                
            /* Invoke the FSM handler with the TERMINATED event */
            /* Trigger FSM */
            ret_val = p_rgm_glb_data->icf_rgm_state_machine[
                    p_rgm_context->rgm_state][
                    ICF_RGM_NOTIFY_TERMINATED](p_crm_pdb);                

        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }/* app_id extracted from contact: ends */    
    else
    {
        ret_val = ICF_FAILURE;
    }


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_nfy_contact_terminated() ends here */

/**************************************************************
 *
 * FUNCTION:        icf_rgm_nfy_get_app_id
 *
 * DESCRIPTION:     This is a utility function that extracts 
                    the app_id from
 *                  the contact_blk element: uri attribute
 *                    
 *    
 * RETURNS:        Success: 
 *                 Faiure: 
 *****************************************************************/

icf_return_t  icf_rgm_nfy_get_app_id(
            INOUT    icf_crm_pdb_st                *p_crm_pdb, 
            INOUT    icf_rgm_contact_blk_st        *p_contact_blk,
            OUT        icf_app_id_t                    *p_app_id)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_uint8_t                temp_ip[ICF_MAX_STR_LEN] = {'\0'};
    icf_uint8_t                self_ip[ICF_MAX_STR_LEN] = {'\0'};
    icf_uint8_t                temp_user[ICF_MAX_STR_LEN] = {'\0'},
								*p_temp,
								temp_port[ICF_MAX_STR_LEN] = {'\0'};

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_contact_blk))
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, (ICF_NULL != p_app_id))
    

    *p_app_id = ICF_APP_ID_INVALID ;/* initialize the app id. */
/* SPR 9045: fix start */
    if (!p_contact_blk->p_contact_uri)
    {
        ret_val = ICF_FAILURE;
    }
    else
	{
		icf_port_strcpy(temp_user, p_contact_blk->p_contact_uri->
            addr_val.str);

		p_temp = icf_port_strchr(temp_user,':');

		if(ICF_NULL != p_temp)
		{
			icf_port_sscanf(p_temp,":%s",temp_port);
		
			*p_temp = '\0';
		}
			
		p_temp = icf_port_strchr(temp_user,'@');

		if(ICF_NULL != p_temp)
		{
			icf_port_sscanf(p_temp,"@%s",temp_ip);
		
			*p_temp = '\0';
		}
		p_temp = icf_port_strchr(temp_user,'_');

		if (ICF_NULL >= icf_port_sscanf(p_temp,"_%d",p_app_id))
		{
			ret_val = ICF_FAILURE;

		}
	}
/* SPR 9045: fix end*/

    if ((ICF_FAILURE != ret_val) &&
                (ICF_FAILURE != 
                icf_cmn_convert_transport_addr_to_string(
        &(p_crm_pdb->p_config_data->self_ip_address.addr),
        self_ip)))
    {
        if (!icf_port_strcmp(temp_ip, self_ip))
        {
            ret_val = ICF_SUCCESS;/* ip matches so app id can be sent. */
        }
        else
        {
            *p_app_id = 0xFFFF;/* Invalid app id 
                            returned incase of different IP. */
            ret_val = ICF_SUCCESS;/* selfip not same as contact's ip */
        }

    }
    else
    {
        ret_val = ICF_FAILURE;/* unable to convert self ip into string */
    }    


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;

}/* Function icf_rgm_nfy_get_app_id() ends here */



/**************************************************************************
 *
 * FUNCTION:       icf_rgm_free_regn_list
 *
 * DESCRIPTION:    This is a utility function that frees the regn_list
 *                 maintained in the reginfo  data structure. It is
 *                 upto the calling function to free the input ptr if needed.
 *
 ****************************************************************************/
icf_return_t icf_rgm_free_regn_list(
        INOUT    icf_glb_pdb_st            *p_glb_pdb,
        INOUT    icf_rgm_regn_list_st        *p_regn_list,
        OUT        icf_error_t                *p_ecode)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_rgm_regn_block_st    *p_regn = ICF_NULL, *p_next = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_regn = p_regn_list->p_regn_block;
    
    /* free the elements in the list */
    while (ICF_NULL != p_regn)
    {
        /* free the contact list of this regn_block */
        icf_rgm_free_contact_list(p_glb_pdb, 
                        &(p_regn->contact_list),p_ecode);

        p_next = p_regn->p_next;        

        /* Free other members of regn_blk*/
        ICF_MEMFREE(p_glb_pdb, p_regn->p_aor, 
            ICF_MEM_COMMON, p_ecode, ret_val)
        ICF_MEMFREE(p_glb_pdb, p_regn->p_id, 
            ICF_MEM_COMMON, p_ecode, ret_val)

        /* Free the regn_block */
        ICF_MEMFREE(p_glb_pdb, p_regn, 
            ICF_MEM_COMMON, p_ecode, ret_val)
        p_regn = p_next;
    }

    /* reset the elements in the list */
    p_regn_list->count = ICF_NULL;
    p_regn_list->p_regn_block = ICF_NULL;

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* icf_rgm_free_regn_list() */


/**************************************************************************
 *
 * FUNCTION:      icf_rgm_free_contact_list(
 *
 * DESCRIPTION:   This is a utility function that frees the contact_list
 *                maintained in the regn(NOTIFY related registration
 *                subelement) data structure. It is
 *                upto the calling function to free the input ptr if needed.
 *
 ***************************************************************************/
icf_return_t icf_rgm_free_contact_list(
        INOUT    icf_glb_pdb_st            *p_glb_pdb,
        INOUT    icf_rgm_contact_list_st    *p_contact_list,
        OUT        icf_error_t                *p_ecode)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_rgm_contact_blk_st  *p_contact = ICF_NULL, *p_next = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_contact = p_contact_list->p_contact_blk;
    
    
    /* free the elements in the list */
    while (ICF_NULL != p_contact)
    {
        p_next = p_contact->p_next;        
                
        /* free other members of contact_blk */
        ICF_MEMFREE(p_glb_pdb, p_contact->p_call_id, 
            ICF_MEM_COMMON, p_ecode, ret_val)
        ICF_MEMFREE(p_glb_pdb, p_contact->p_contact_uri, 
            ICF_MEM_COMMON, p_ecode, ret_val)
        ICF_MEMFREE(p_glb_pdb, p_contact->
            p_display_name_element, ICF_MEM_COMMON, p_ecode, ret_val)
        ICF_MEMFREE(p_glb_pdb, p_contact->p_id, 
            ICF_MEM_COMMON, p_ecode, ret_val)
        ICF_MEMFREE(p_glb_pdb, p_contact->p_opt_param, 
            ICF_MEM_COMMON, p_ecode, ret_val)
        ICF_MEMFREE(p_glb_pdb, p_contact->p_relative_priority, 
            ICF_MEM_COMMON, p_ecode, ret_val)
        
        /* free the contact block */
        ICF_MEMFREE(p_glb_pdb, p_contact, 
            ICF_MEM_COMMON, p_ecode, ret_val)

        p_contact = p_next;
    }

    /* reset the elements in the list */
    p_contact_list->count = ICF_NULL;
    p_contact_list->p_contact_blk = ICF_NULL;

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* icf_rgm_free_contact_list() */

/******************************************************************************
 *      UTILITY FUNCTIONS FOR FILLING REGINFO
 ******************************************************************************/

/***************************************************************
 * FUNCTION:  icf_rgm_init_address
 *
 * DESCRIPTION: This function Gets Memory required for icf_address_st
 *              and Initialise it properly
 **************************************************************/ 
icf_return_t icf_rgm_init_address(
                                   INOUT  icf_glb_pdb_st             *p_glb_pdb,
                                   OUT   icf_address_st **p_p_Aor,
                                   INOUT icf_error_t     *p_ecode)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    ICF_MEMGET(p_glb_pdb, sizeof(icf_address_st), 
        ICF_MEM_COMMON, 
        (*p_p_Aor), 
        ICF_RET_ON_FAILURE, p_ecode, ret_val)

    icf_port_memset(*p_p_Aor,0,sizeof(icf_address_st));
    return ret_val;

}/* function icf_rgm_init_address() ends */

/******************************************************************************
 * FUNCTION:  icf_rgm_init_regnblock
 *
 * DESCRIPTION: This function Gets Memory required for icf_rgm_regn_block_st
 *              and Initialise it properly
 *****************************************************************************/ 
icf_return_t icf_rgm_init_regnblock(
                               INOUT  icf_glb_pdb_st             *p_glb_pdb,
                               OUT   icf_rgm_regn_block_st    **p_p_regn_block,
                               INOUT icf_error_t              *p_ecode)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    ICF_MEMGET(p_glb_pdb, sizeof(icf_rgm_regn_block_st), 
        ICF_MEM_COMMON, 
        (*p_p_regn_block), 
        ICF_RET_ON_FAILURE, p_ecode, ret_val)

    icf_port_memset(*p_p_regn_block,0,sizeof(icf_rgm_regn_block_st));
    return ret_val;
}/* function icf_rgm_init_regnblock() ends */

/******************************************************************************
 * FUNCTION:  icf_rgm_init_contact_blk
 *
 * DESCRIPTION: This function Gets Memory required for icf_rgm_contact_blk_st
 *              and Initialise it properly
 *****************************************************************************/ 
icf_return_t icf_rgm_init_contact_blk(
                              INOUT  icf_glb_pdb_st       *p_glb_pdb,
                              OUT icf_rgm_contact_blk_st **p_p_contact_block,
                              INOUT icf_error_t     *p_ecode)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    ICF_MEMGET(p_glb_pdb, sizeof(icf_rgm_contact_blk_st), 
        ICF_MEM_COMMON, 
        (*p_p_contact_block), 
        ICF_RET_ON_FAILURE, p_ecode, ret_val)

    icf_port_memset(*p_p_contact_block,0,sizeof(icf_rgm_contact_blk_st));
    return ret_val;

}/* function icf_rgm_init_contact_blk() ends */

/******************************************************************************
 * FUNCTION:  icf_rgm_init_glb_parse_reginfo_block
 *
 * DESCRIPTION: This function Gets Memory required for 
 *              icf_rgm_glb_parse_reginfo_st
 *              and Initialise it properly
 *****************************************************************************/ 
icf_return_t icf_rgm_init_glb_parse_reginfo_block(
                     INOUT  icf_glb_pdb_st                *p_glb_pdb,
                     IN  icf_rgm_reginfo_st            *p_reginfo,
                     OUT icf_rgm_glb_parse_reginfo_st  **p_p_glb_parse_reginfo,
                     INOUT icf_error_t        *p_ecode)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    /* Allocate memory to the user_addr_list */
    ICF_MEMGET(p_glb_pdb, sizeof(icf_rgm_glb_parse_reginfo_st), 
                ICF_MEM_COMMON, 
                (*p_p_glb_parse_reginfo), 
                ICF_RET_ON_FAILURE, p_ecode, ret_val)
    icf_port_memset(*p_p_glb_parse_reginfo,0,
                    sizeof(icf_rgm_glb_parse_reginfo_st));
    (*p_p_glb_parse_reginfo)->p_glb_pdb = (icf_glb_pdb_st*)p_glb_pdb;
    (*p_p_glb_parse_reginfo)->p_actual_rgm_reg_info = 
                                    (icf_rgm_reginfo_st*)p_reginfo;
    return ret_val;

}/* Function icf_rgm_init_glb_parse_reginfo_block() ends */


/******************************************************************************
 * FUNCTION:  icf_rgm_free_contact_block
 *
 * DESCRIPTION: This function frees the Contact Block
 *              
 *****************************************************************************/ 
icf_return_t icf_rgm_free_contact_block(
                               INOUT  icf_glb_pdb_st            *p_glb_pdb,
                               INOUT icf_rgm_contact_blk_st  *p_contact_block,
                               INOUT icf_error_t             *p_ecode)
{   
    icf_return_t         ret_val = ICF_SUCCESS;

    if(ICF_NULL != p_contact_block->p_id)
    {
        icf_port_strfree(p_contact_block->p_id);
    }
    if(ICF_NULL != p_contact_block->p_contact_uri)
    {
        ICF_MEMFREE(p_glb_pdb,
                p_contact_block->p_contact_uri,
              ICF_MEM_COMMON,p_ecode,ret_val)
    }
    if(ICF_NULL != p_contact_block->p_display_name_element)
    {
        icf_port_strfree(p_contact_block->p_display_name_element);
    }
    if(ICF_NULL != p_contact_block->p_relative_priority)
    {
        icf_port_strfree(p_contact_block->p_relative_priority);
    }
    if(ICF_NULL != p_contact_block->p_call_id)
    {
        icf_port_strfree(p_contact_block->p_call_id);
    }
    /* Free the memory allocated to the node */
    ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_contact_block
        , ICF_MEM_COMMON,
        p_ecode, ret_val)
     return ICF_SUCCESS;
}



/******************************************************************************
 * FUNCTION:  icf_rgm_free_regn_block
 *
 * DESCRIPTION: This function frees the Registration Block
 *              
 *****************************************************************************/ 
icf_return_t icf_rgm_free_regn_block(
                             INOUT  icf_glb_pdb_st           *p_glb_pdb,
                             INOUT icf_rgm_regn_block_st  *p_regn_block,
                             INOUT icf_error_t            *p_ecode)
{   
    icf_return_t         ret_val = ICF_SUCCESS;

    /* First Free the Registation Blocks of the Rgm List*/
    icf_rgm_contact_blk_st     *p_contact_block =
                                    p_regn_block->contact_list.p_contact_blk;
   
    ICF_MEMFREE(p_glb_pdb, p_regn_block->p_aor, ICF_MEM_COMMON, p_ecode, ret_val)
   
    if(ICF_NULL != p_regn_block->p_id)
    {
        icf_port_strfree(p_regn_block->p_id);
    }

    while(ICF_NULL != p_contact_block)
    {
        icf_rgm_contact_blk_st     *p_next_contact_block =
                                        p_contact_block->p_next;
        icf_rgm_free_contact_block(p_glb_pdb,p_contact_block,p_ecode);
        p_contact_block = p_next_contact_block;
    }
    /* Free the memory allocated to the node */
    ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_regn_block
        , ICF_MEM_COMMON,
        p_ecode, ret_val)
    return ICF_SUCCESS;
}
/******************************************************************************
 * FUNCTION:  icf_rgm_free_reginfo
 *
 * DESCRIPTION: This function frees the  Registration Info block 
 *
 *****************************************************************************/ 
icf_return_t icf_rgm_free_reginfo(
                               INOUT  icf_glb_pdb_st       *p_glb_pdb,
                               INOUT icf_rgm_reginfo_st  *p_reginfo,
                               INOUT icf_error_t       *p_ecode)
{   
    icf_return_t         ret_val = ICF_SUCCESS;

    /* First Free the Registation Blocks of the Rgm List*/
    icf_rgm_regn_block_st     *p_regn_block =
                                    p_reginfo->registration_list.p_regn_block;
    while(ICF_NULL != p_regn_block)
    {
        icf_rgm_regn_block_st     *p_next_regn_block =
                            p_regn_block->p_next;
        icf_rgm_free_regn_block(p_glb_pdb,p_regn_block,p_ecode);
        p_regn_block = p_next_regn_block;
    }
    /* Free the memory allocated to the node */
    ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_regn_block
        , ICF_MEM_COMMON,
        p_ecode, ret_val)
    return ICF_SUCCESS;
}


/******************************************************************************
 * FUNCTION:  icf_rgm_free_glb_parse_reginfo
 *
 * DESCRIPTION: This function frees the Global Parse Registration Info block 
 *
 *****************************************************************************/ 
icf_return_t icf_rgm_free_glb_parse_reginfo(
                INOUT  icf_glb_pdb_st                  *p_glb_pdb,
                INOUT icf_rgm_glb_parse_reginfo_st  *p_glb_parse_reginfo,
                INOUT icf_error_t       *p_ecode)
{   
    icf_return_t         ret_val = ICF_SUCCESS;

     /* If temporary contact block is not null then free it*/
    if(ICF_NULL != p_glb_parse_reginfo->p_temp_contact_block)
    {
        icf_rgm_free_contact_block(p_glb_pdb,
            p_glb_parse_reginfo->p_temp_contact_block,p_ecode);
    }
    /* If temporary registration block is not null then free it*/
    if(ICF_NULL != p_glb_parse_reginfo->p_temp_regn_block)
    {
        icf_rgm_free_regn_block(p_glb_pdb,
            p_glb_parse_reginfo->p_temp_regn_block,p_ecode);
    }
    /* Free the Actual Registration info */
    icf_rgm_free_reginfo(p_glb_pdb,
                            p_glb_parse_reginfo->p_actual_rgm_reg_info,p_ecode);
    /* Free the memory allocated to the node */
    ICF_MEMFREE(p_glb_pdb, (icf_uint8_t *)p_glb_parse_reginfo
        , ICF_MEM_COMMON,
        p_ecode, ret_val)
    return ICF_SUCCESS;

}/* function icf_rgm_free_glb_parse_reginfo() ends */



/******************************************************************************
 * FUNCTION:  icf_rgm_fill_reginfo_attribute
 *
 * DESCRIPTION: This function Fills RegInfo Attributes 
 *              
 *****************************************************************************/ 
icf_return_t icf_rgm_fill_reginfo_attribute(
                                IN  icf_glb_pdb_st       *p_glb_pdb,
                                INOUT icf_rgm_reginfo_st *p_rgm_reg_info,
                                IN    icf_uint8_t *p_attribute,
                                IN    icf_uint8_t *p_Value,
                                INOUT icf_error_t *p_ecode)
{
    icf_return_t         ret_val = ICF_SUCCESS;
     /* To remove warnings */
     p_glb_pdb=p_glb_pdb;
     p_ecode=p_ecode;
    if(0 == icf_port_strcmp("version",p_attribute))
    {
        p_rgm_reg_info->version = icf_port_atoi(p_Value);
    }
    else if(0 == icf_port_strcmp("state",p_attribute))
    {
        if(0 == icf_port_strcmp("full",p_Value))
        {
            p_rgm_reg_info->reginfo_state = ICF_RGM_NFY_REGINFO_STATE_FULL;
        }
        else if (0 == icf_port_strcmp("partial",p_Value))
        {
            p_rgm_reg_info->reginfo_state = ICF_RGM_NFY_REGINFO_STATE_PARTIAL;
        }
        else
        {
            p_rgm_reg_info->reginfo_state = ICF_RGM_NFY_REGINFO_STATE_INVALID;
        }
    }
    else
    {
        /* not handled currently */
    }
    
    return ret_val;
    
}/* function icf_rgm_fill_reginfo_attribute() ends */

/******************************************************************************
 * FUNCTION:  icf_rgm_fill_regn_blk_attribute
 *
 * DESCRIPTION: This function Fills Registration Block Attributes to
 *              icf_rgm_regn_block_st
 *****************************************************************************/
icf_return_t icf_rgm_fill_regn_blk_attribute(
                            INOUT  icf_glb_pdb_st       *p_glb_pdb,
                            INOUT   icf_rgm_regn_block_st    *p_regn_block,
                            IN        icf_uint8_t *p_attribute,
                            IN        icf_uint8_t *p_Value,
                            INOUT     icf_error_t *p_ecode)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    if(0 == icf_port_strcmp("aor",p_attribute))
    {
        /* Assumption Only one AoR is takend care of */
        /* Allocate memory for storing the Address of record */
        if(ICF_NULL == p_regn_block->p_aor)
        {
            if(ICF_FAILURE == 
                icf_rgm_init_address(p_glb_pdb,&(p_regn_block->p_aor),
                                                p_ecode))
            {
                ret_val = ICF_FAILURE;
            }
            else 
            {
                /* call function to convert string to address_st */
                icf_rgm_parse_string_to_address(p_Value,p_regn_block->p_aor);
                

                /*icf_port_strcpy(p_regn_block->p_aor->addr_val.str,p_Value);
                p_regn_block->p_aor->addr_val.str_len = 
                    icf_port_strlen(p_Value);
                p_regn_block->p_aor->addr_type = ICF_ADDRESS_PLAN_SIP_URI;*/
            }

        }
    }
    else if(0 == icf_port_strcmp("id",p_attribute))
    {
        /* Assumption Only one ID is takend care of */
        if(ICF_NULL == p_regn_block->p_id)
        {
            p_regn_block->p_id = icf_port_strdup(p_Value);
        }

    }
    else if(0 == icf_port_strcmp("state",p_attribute))
    {
        if(0 == icf_port_strcmp("init",p_Value))
        {
            p_regn_block->state = ICF_RGM_NFY_REG_STATE_INIT;
        }
        else if(0 == icf_port_strcmp("active",p_Value))
        {
            p_regn_block->state = ICF_RGM_NFY_REG_STATE_ACTIVE;
        }
        else if (0 == icf_port_strcmp("terminated",p_Value))
        {
            p_regn_block->state = ICF_RGM_NFY_REG_STATE_TERMINATED;
        }
        else
        {
            p_regn_block->state = ICF_RGM_NFY_REG_STATE_INVALID;
        }
    }
    else
    {
        /* not handled currently */
    }
    
    return ret_val;

}/* Function icf_rgm_fill_regn_blk_attribute() ends */

/******************************************************************************
 * FUNCTION:  icf_rgm_fill_contact_blk_attribute
 *
 * DESCRIPTION: This function Fills Contact Block Attributes to 
 *              icf_rgm_contact_blk_st
 *              
 *****************************************************************************/
icf_return_t icf_rgm_fill_contact_blk_attribute(
                            IN  icf_glb_pdb_st       *p_glb_pdb,
                            INOUT   icf_rgm_contact_blk_st *p_contact_block,
                            IN      icf_uint8_t *p_attribute,
                            IN      icf_uint8_t *p_Value,
                            INOUT   icf_error_t *p_ecode)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    /* To remove warning */
    p_glb_pdb=p_glb_pdb;
    p_ecode=p_ecode;
    
    if(0 == icf_port_strcmp("id",p_attribute))
    {
        /* Assumption Only one ID is takend care of */
        if(ICF_NULL == p_contact_block->p_id)
        {
            p_contact_block->p_id = icf_port_strdup(p_Value);
        }

    }
    else if(0 == icf_port_strcmp("state",p_attribute))
    {
        if(0 == icf_port_strcmp("active",p_Value))
        {
            p_contact_block->state = ICF_RGM_NFY_CON_STATE_ACTIVE;
        }
        else if (0 == icf_port_strcmp("terminated",p_Value))
        {
            p_contact_block->state = ICF_RGM_NFY_CON_STATE_TERMINATED;
        }
        else
        {
            p_contact_block->state = ICF_RGM_NFY_CON_STATE_INVALID;
        }
    }
    else if(0 == icf_port_strcmp("event",p_attribute))
    {
        if(0 == icf_port_strcmp("registered",p_Value))
        {
            p_contact_block->event = ICF_RGM_NFY_CON_EVT_REGISTERED;
        }
        else if (0 == icf_port_strcmp("created",p_Value))
        {
            p_contact_block->event = ICF_RGM_NFY_CON_EVT_CREATED;
        }
        else if (0 == icf_port_strcmp("refreshed",p_Value))
        {
            p_contact_block->event = ICF_RGM_NFY_CON_EVT_REFRESHED;
        }
        else if (0 == icf_port_strcmp("shortened",p_Value))
        {
            p_contact_block->event = ICF_RGM_NFY_CON_EVT_SHORTENED;
        }
        else if (0 == icf_port_strcmp("expired",p_Value))
        {
            p_contact_block->event = ICF_RGM_NFY_CON_EVT_EXPIRED;
        }
        else if (0 == icf_port_strcmp("deactivated",p_Value))
        {
            p_contact_block->event = ICF_RGM_NFY_CON_EVT_DEACTIVATED;
        }
        else if (0 == icf_port_strcmp("probation",p_Value))
        {
            p_contact_block->event = ICF_RGM_NFY_CON_EVT_PROBATION;
        }
        else if (0 == icf_port_strcmp("unregistered",p_Value))
        {
            p_contact_block->event = ICF_RGM_NFY_CON_EVT_UNREGISTERED;
        }
        else if (0 == icf_port_strcmp("rejected",p_Value))
        {
            p_contact_block->event = ICF_RGM_NFY_CON_EVT_REJECTED;
        }
        else
        {
            p_contact_block->event = ICF_RGM_NFY_CON_EVT_INVALID;
        }
    }
    else if(0 == icf_port_strcmp("expires",p_attribute))
    {
        ICF_SET_BIT(p_contact_block->bitmask,
                        ICF_RGM_NFY_CON_EXPIRES_PRESENT);
        p_contact_block->expires = ICF_PORT_ATOL(p_Value);
    }
    else if(0 == icf_port_strcmp("duration-registered",p_attribute))
    {
        ICF_SET_BIT(p_contact_block->bitmask,
                        ICF_RGM_NFY_CON_DURATION_PRESENT);
        p_contact_block->duration_registered = ICF_PORT_ATOL(p_Value);

    }
    else if(0 == icf_port_strcmp("retry-after",p_attribute))
    {
        ICF_SET_BIT(p_contact_block->bitmask,
                        ICF_RGM_NFY_CON_RETRY_AFTER_PRESENT);
        p_contact_block->retry_after = ICF_PORT_ATOL(p_Value);

    }
    else if(0 == icf_port_strcmp("q",p_attribute))
    {
        ICF_SET_BIT(p_contact_block->bitmask,
                        ICF_RGM_NFY_CON_PRIORITY_PRESENT);
        if(ICF_NULL == p_contact_block->p_relative_priority)
        {
            p_contact_block->p_relative_priority = icf_port_strdup(p_Value);
        }

    }
    else if(0 == icf_port_strcmp("cseq",p_attribute))
    {
        ICF_SET_BIT(p_contact_block->bitmask,
                        ICF_RGM_NFY_CON_CSEQ_PRESENT);
        p_contact_block->cseq = ICF_PORT_ATOL(p_Value);
    }
    else if(0 == icf_port_strcmp("callid",p_attribute))
    {
        ICF_SET_BIT(p_contact_block->bitmask,
                        ICF_RGM_NFY_CON_CALL_ID_PRESENT);
        if(ICF_NULL == p_contact_block->p_call_id)
        {
            p_contact_block->p_call_id = icf_port_strdup(p_Value);
        }

    }
    else if(0 == icf_port_strcmp("display-name",p_attribute))
    {
        ICF_SET_BIT(p_contact_block->bitmask,
                        ICF_RGM_NFY_CON_DISPLAY_NAME_PRESENT);
        if(ICF_NULL == p_contact_block->p_display_name_element)
        {
            p_contact_block->p_display_name_element = 
                             icf_port_strdup(p_Value);
        }
    }
    else
    {
        /* currntly Not handled */
    }

    return ret_val;

}/* function icf_rgm_fill_contact_blk_attribute() ends */


/******************************************************************************
 * FUNCTION:  icf_regm_append_regnblk_to_regnblk_list
 *
 * DESCRIPTION: This function Fills Regisration Block 
 *              icf_rgm_contact_blk_st to Registration List
 *              
 *****************************************************************************/
icf_return_t    icf_regm_append_regnblk_to_regnblk_list(
                      INOUT  icf_rgm_regn_block_st             *p_regn_block,
                      INOUT  icf_rgm_regn_list_st           *p_regn_list,
                      INOUT  icf_error_t                 *p_ecode)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_rgm_regn_block_st **p_p_temp_regn_blk =
                                 &(p_regn_list->p_regn_block);
    while(ICF_NULL != *p_p_temp_regn_blk)
    {
        /*p_p_temp_regn_blk = &((icf_rgm_regn_block_st*)(*p_p_temp_regn_blk)->p_next);*/

        p_p_temp_regn_blk = &((*p_p_temp_regn_blk)->p_next);
    }
    *p_p_temp_regn_blk = p_regn_block;
    p_regn_list->count++;
        

    return ret_val;
    p_ecode = p_ecode;

}/* Function icf_regm_append_regnblk_to_regnblk_list() ends */

/******************************************************************************
 * FUNCTION:  icf_regm_append_contactblk_to_contact_list
 *
 * DESCRIPTION: This function appends the Contact Block  
 *              icf_rgm_contact_blk_st to Contact List 
 *              icf_rgm_contact_list_st
 *              
 *****************************************************************************/
icf_return_t    icf_regm_append_contactblk_to_contact_list(
                      INOUT    icf_rgm_contact_blk_st        *p_contact_blk,
                      INOUT    icf_rgm_contact_list_st      *p_contact_list,
                      INOUT    icf_error_t                  *p_ecode)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_rgm_contact_blk_st **p_p_temp_contactblk =
                                 &(p_contact_list->p_contact_blk);
    while(ICF_NULL != *p_p_temp_contactblk)
    {
        /*p_p_temp_contactblk = &((icf_rgm_contact_blk_st*)(*p_p_temp_contactblk)->p_next);*/

        p_p_temp_contactblk = &((*p_p_temp_contactblk)->p_next);
    }
    *p_p_temp_contactblk = p_contact_blk;
    p_contact_list->count++;
        

    return ret_val;
    p_ecode = p_ecode;

}/* Function icf_regm_append_contactblk_to_contact_list() ends */

/******************************************************************
 *  Implementation Of call Back Function Of XML Parser for RegInfo
 ******************************************************************/

/******************************************************************************
 * FUNCTION:  icf_cmn_xmlparser_cbk_node_attribute
 *
 * DESCRIPTION: This function is the call back function of XML Parser
 *              which provides Attrbute and value of the attribute 
 *              for a Node
 *              
 *****************************************************************************/
icf_return_t icf_cmn_xmlparser_cbk_node_attribute(
                                    INOUT    icf_void_t  *p_app_ctxt,
                                    IN        icf_uint8_t *p_nodeName,
                                    IN        icf_uint8_t *p_attribute,
                                    IN        icf_uint8_t *p_Value,
                                    INOUT    icf_error_t *p_ecode)
{
    icf_rgm_glb_parse_reginfo_st  *p_glb_reginfo = 
                                (icf_rgm_glb_parse_reginfo_st*)p_app_ctxt;

    icf_return_t         ret_val = ICF_SUCCESS;

    ICF_PRINT(("\t\t%s=%s\n",p_attribute,p_Value));
    
    if((0 == icf_port_strcmp("reginfo",p_nodeName)) &&
       (ICF_TRUE == 
        ICF_IS_BIT_SET(p_glb_reginfo->bit_mask,
                ICF_RGM_NFY_XML_PARSE_REGINFO_START)))
    {
        ret_val = icf_rgm_fill_reginfo_attribute(p_glb_reginfo->p_glb_pdb,
                        p_glb_reginfo->p_actual_rgm_reg_info,
                        p_attribute,p_Value,p_ecode);
    }
    else if(0 == icf_port_strcmp("registration",p_nodeName))
    {
        
        if(ICF_TRUE == 
             ICF_IS_BIT_SET(p_glb_reginfo->bit_mask,\
                    ICF_RGM_NFY_XML_PARSE_REGNBLOCK_START) && 
            (ICF_NULL != p_glb_reginfo->p_temp_regn_block) )
        {
            ret_val = icf_rgm_fill_regn_blk_attribute(
                        p_glb_reginfo->p_glb_pdb,
                        p_glb_reginfo->p_temp_regn_block,
                        p_attribute,p_Value,p_ecode);
        }
    }
    else if(0 == icf_port_strcmp("contact",p_nodeName))
           
    {
        if(ICF_TRUE == 
             ICF_IS_BIT_SET(p_glb_reginfo->bit_mask,\
                    ICF_RGM_NFY_XML_PARSE_CONTACTBLOCK_START) && 
            (ICF_NULL != p_glb_reginfo->p_temp_regn_block) )
        {
            ret_val = icf_rgm_fill_contact_blk_attribute(
                            p_glb_reginfo->p_glb_pdb,
                            p_glb_reginfo->p_temp_contact_block,
                            p_attribute,p_Value,p_ecode);
        }

    }
    else if(0 == icf_port_strcmp("uri",p_nodeName))
    {
        /* URI Related Attributes ignored */
    }
    else
    {
        /* Dont do any thing */
    }

    return ret_val;
}/* function icf_cmn_xmlparser_cbk_node_attribute() ends */

/******************************************************************************
 * FUNCTION:  icf_cmn_xmlparser_cbk_node_tagdata
 *
 * DESCRIPTION: This function is the call back function of XML Parser
 *              which provides the TagData of a Node
 *              
 *****************************************************************************/
icf_return_t    icf_cmn_xmlparser_cbk_node_tagdata(
                                    INOUT icf_void_t     *p_app_ctxt,
                                    IN      icf_uint8_t    *p_nodeName,
                                    IN      icf_uint8_t    *p_tagData,
                                    INOUT icf_error_t     *p_ecode)
{
       icf_rgm_glb_parse_reginfo_st  *p_glb_reginfo = 
                                (icf_rgm_glb_parse_reginfo_st*)p_app_ctxt;

   /* To remove warning */
   p_ecode=p_ecode; 
    ICF_PRINT(("TagData:%s\n",p_tagData));
    if(0 == icf_port_strcmp("uri",p_nodeName))
    {
        if(ICF_TRUE == 
             ICF_IS_BIT_SET(p_glb_reginfo->bit_mask,
                (ICF_RGM_NFY_XML_PARSE_CONTACTBLOCK_START |
                ICF_RGM_NFY_XML_PARSE_URIBLOCK_START)))
        {
            /* call function to convert string to address_st */

            icf_rgm_parse_string_to_address(p_tagData,
                p_glb_reginfo->p_temp_contact_block->p_contact_uri);
                
            
            /*icf_address_st *p_uri =
                p_glb_reginfo->p_temp_contact_block->p_contact_uri;
            icf_port_strcpy(
                p_uri->addr_val.str,p_tagData);
            p_uri->addr_val.str_len = 
                icf_port_strlen(p_tagData);
            p_uri->addr_type = ICF_ADDRESS_PLAN_SIP_URI;*/

        }
    }
    else
    {
        /* Others are not handled*/
    }

    return ICF_SUCCESS;
    
}/* Function icf_cmn_xmlparser_cbk_node_tagdata() ends */

/******************************************************************************
 * FUNCTION:  icf_cmn_xmlparser_cbk_node_start
 *
 * DESCRIPTION: This function is the call back function of XML Parser
 *              which provides the indication for strating of parsing
 *              for a Node
 *****************************************************************************/
icf_return_t icf_cmn_xmlparser_cbk_node_start(
                                    INOUT  icf_void_t  *p_app_ctxt,
                                    IN       icf_uint8_t *p_nodeName,
                                    INOUT  icf_error_t     *p_ecode)
{
    icf_rgm_glb_parse_reginfo_st  *p_glb_reginfo = 
                                (icf_rgm_glb_parse_reginfo_st*)p_app_ctxt;
    if(0 == icf_port_strcmp("reginfo",p_nodeName)) 
    {
        if (ICF_FALSE == 
            ICF_IS_BIT_SET(p_glb_reginfo->bit_mask,
            ICF_RGM_NFY_XML_PARSE_REGINFO_START))
        {
            ICF_SET_BIT(p_glb_reginfo->bit_mask,ICF_RGM_NFY_XML_PARSE_REGINFO_START)
        }
        else
        {
            return ICF_FAILURE;
        }
    }
    else if(0 == icf_port_strcmp("registration",p_nodeName))
    {
        if (ICF_FALSE == 
            ICF_IS_BIT_SET(p_glb_reginfo->bit_mask,
            ICF_RGM_NFY_XML_PARSE_REGNBLOCK_START))
        {
            ICF_SET_BIT(p_glb_reginfo->bit_mask,
                ICF_RGM_NFY_XML_PARSE_REGNBLOCK_START)
                /* Allocate memory for temporary Registration block and store to
                * glb_regInfo*/
                icf_rgm_init_regnblock(p_glb_reginfo->p_glb_pdb,
                &p_glb_reginfo->p_temp_regn_block,p_ecode);
        }
        else
        {
            return ICF_FAILURE;
        }        
        
    }
    else if (0 == icf_port_strcmp("contact",p_nodeName))
    {
        if  (ICF_FALSE == 
            ICF_IS_BIT_SET(p_glb_reginfo->bit_mask,
            ICF_RGM_NFY_XML_PARSE_CONTACTBLOCK_START))
        {
            ICF_SET_BIT(p_glb_reginfo->bit_mask,ICF_RGM_NFY_XML_PARSE_CONTACTBLOCK_START)
                /* Allocate memory for temporary Contact block */
                icf_rgm_init_contact_blk(p_glb_reginfo->p_glb_pdb,
                &p_glb_reginfo->p_temp_contact_block,p_ecode);
            
        }
        else
        {
            return ICF_FAILURE;
        }        
        
    }
    else if(0 == icf_port_strcmp("uri",p_nodeName))        
    {
        /* This Uri Parameter is associated with contact block.
         * If temporary Conact Block is allocated then fill this uri
         * to Contact_uri of temporary Contact block only if it is
         * not filled yet
         */

         if ((ICF_FALSE == 
                ICF_IS_BIT_SET(p_glb_reginfo->bit_mask,
                    ICF_RGM_NFY_XML_PARSE_URIBLOCK_START)) &&
             (ICF_TRUE == 
             ICF_IS_BIT_SET(p_glb_reginfo->bit_mask,
                ICF_RGM_NFY_XML_PARSE_CONTACTBLOCK_START)) &&
            (ICF_NULL != p_glb_reginfo->p_temp_contact_block) &&
            (ICF_NULL == p_glb_reginfo->p_temp_contact_block->p_contact_uri))
         {
            /* Allocate memory for Contact Uri */
            icf_rgm_init_address(p_glb_reginfo->p_glb_pdb,
                &p_glb_reginfo->p_temp_contact_block->p_contact_uri,p_ecode);
            ICF_SET_BIT(p_glb_reginfo->bit_mask,
                ICF_RGM_NFY_XML_PARSE_URIBLOCK_START)
         }
         else
         {
             return ICF_FAILURE;
         }

    }
    else
    {
        /* Not Required*/
    }
    ICF_PRINT(("\n\nPARSING START FOR NODE: %s \t0x%x\n",p_nodeName,p_glb_reginfo->bit_mask));

    return ICF_SUCCESS;
    
}/* function icf_cmn_xmlparser_cbk_node_start() ends */

/******************************************************************************
 * FUNCTION:  icf_cmn_xmlparser_cbk_node_end
 *
 * DESCRIPTION: This function is the call back function of XML Parser
 *              which provides the indication for ending of parsing
 *              for a Node
 *****************************************************************************/
icf_return_t icf_cmn_xmlparser_cbk_node_end(
                                    INOUT  icf_void_t  *p_app_ctxt,
                                    IN       icf_uint8_t *p_nodeName,
                                    INOUT  icf_error_t *p_ecode)
{
    icf_rgm_glb_parse_reginfo_st  *p_glb_reginfo = 
                                (icf_rgm_glb_parse_reginfo_st*)p_app_ctxt;
    

    if(0 == icf_port_strcmp("reginfo",p_nodeName))
    {
        ICF_RESET_BIT(p_glb_reginfo->bit_mask,ICF_RGM_NFY_XML_PARSE_REGINFO_START)
    }
    else if(0 == icf_port_strcmp("registration",p_nodeName))
    {
         /* Append the temporary Registration block to the Regn list 
          * of RegInfo Block 
          */
        icf_regm_append_regnblk_to_regnblk_list(
            p_glb_reginfo->p_temp_regn_block,
            &(p_glb_reginfo->p_actual_rgm_reg_info->registration_list),p_ecode);
        p_glb_reginfo->p_temp_regn_block = ICF_NULL;
        ICF_RESET_BIT(p_glb_reginfo->bit_mask,ICF_RGM_NFY_XML_PARSE_REGNBLOCK_START)
    }
    else if(0 == icf_port_strcmp("contact",p_nodeName))
    {
        /* Append the temporary Contact to the Contact list 
         * of temporary Registration block 
         */
        icf_regm_append_contactblk_to_contact_list(
            p_glb_reginfo->p_temp_contact_block,
            &(p_glb_reginfo->p_temp_regn_block->contact_list),p_ecode);
        p_glb_reginfo->p_temp_contact_block = ICF_NULL;
        ICF_RESET_BIT(p_glb_reginfo->bit_mask,ICF_RGM_NFY_XML_PARSE_CONTACTBLOCK_START)
    }
    else if(0 == icf_port_strcmp("uri",p_nodeName))
    {
        ICF_RESET_BIT(p_glb_reginfo->bit_mask,ICF_RGM_NFY_XML_PARSE_URIBLOCK_START)
    }
    else
    {
        /* Dont do any thing we r not doing anything */
    }
    ICF_PRINT(("\nPARSING END FOR NODE: /%s> \t0x%x\n",p_nodeName,p_glb_reginfo->bit_mask));

    return ICF_SUCCESS;
    
}/* Function icf_cmn_xmlparser_cbk_node_end() ends here */


/****************************************************************************
 * FUNCTION:            icf_rgm_nfy_con_probation
 *
 * DESCRIPTION:         This function is invoked when valid
 *                      contact with state=terminated and
 *                      event=probation is received in REGISTERED state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_nfy_con_probation(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_rgm_register_resp_st    register_response;
    icf_rgm_app_ctxt_st      *p_temp_app_ctx = ICF_NULL;
    icf_app_id_t             temp_app_id = ICF_NULL;
        
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

    /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
                    (ICF_NULL != p_rgm_context))

    if (ICF_DUMMY_APP_ID == p_crm_pdb->recd_con_app_id)
    {
        if ((ICF_TRUE == p_crm_pdb->p_config_data->auto_register) &&
                (ICF_TRUE == p_rgm_context->auto_reg_flag))
        {
            p_rgm_context->auto_reg_flag = ICF_FALSE;
            p_rgm_context->indicator |=
                    ICF_RGM_NFY_ON_PROBATION;
            p_rgm_context->event_auto_reg = 0;
            p_rgm_context->event_auto_reg =
                    ICF_AUTO_REG_PROBATION;

            if (p_crm_pdb->new_retry_value > 
                p_rgm_context->nfy_retry_timer_value)
            {/* store the new value of retry_after */
                p_rgm_context->nfy_retry_timer_value =
                    p_crm_pdb->new_retry_value;
                
            }
        }/* auto_reg scenario*/
    }/* Contact had DUMMY_APP_ID in the uri */

    else
    {
        temp_app_id = p_crm_pdb->recd_con_app_id;
        /* Inform the app if present in rgm_context: */         
         if (ICF_TRUE == icf_rgm_fetch_app_info(
                        p_crm_pdb,p_rgm_context,
                          temp_app_id, &p_temp_app_ctx))
         {
             /* indicate the app of network initiated dereg*/
             register_response.p_crm_pdb = p_crm_pdb;
             register_response.app_id = temp_app_id;
             register_response.line_id = p_temp_app_ctx->line_id;
             register_response.options_flag = ICF_NULL;
             /*use rgm_ctxt for To/From*/

             register_response.options_flag |= ICF_RGM_CTXT_COPY;
             register_response.p_rgm_context = p_rgm_context;
             register_response.p_request_payload = ICF_NULL;
             /* Network initiated DEREG happened */
             register_response.request_type = ICF_NULL;;
             register_response.status = ICF_FAILURE;
             register_response.error_cause = ICF_CAUSE_NW_PROBATION;
             /*for CFG to not send response*/
              p_crm_pdb->resp_to_be_sent = ICF_FALSE;
              ret_val = icf_rgm_send_to_app(p_crm_pdb->p_glb_pdb,
                    &register_response, p_crm_pdb->p_ecode);

              /* Set the value for Probation in app_indicator */
              p_temp_app_ctx->app_indicator |= 
                    ICF_RGM_APP_DEREG_ON_PROBATION;

              /* value set in indicator bitmask so that retry_after
               * timer can be started once all contacts are processed */
              p_rgm_context->indicator |=
                    ICF_RGM_NFY_ON_PROBATION;             
          
    
              if (p_crm_pdb->new_retry_value > 
                  p_rgm_context->nfy_retry_timer_value)
              {/* store the new value of retry_after */
                  p_rgm_context->nfy_retry_timer_value =
                      p_crm_pdb->new_retry_value;                  
                  
              }

            }/* app_found in rgm_ctxt: ends */
    }/* non-dummy-app-id contact: ends */    


        
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    

}/* Function icf_rgm_nfy_con_probation() ends here */


/****************************************************************************
 * FUNCTION:            icf_rgm_nfy_con_expired
 *
 * DESCRIPTION:         This function is invoked when valid
 *                      contact with state=terminated and
 *                      event=expired is received in REGISTERED state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_nfy_con_expired(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_rgm_register_resp_st    register_response;
    icf_rgm_app_ctxt_st      *p_temp_app_ctx = ICF_NULL;
    icf_app_id_t             temp_app_id = ICF_NULL;
        
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

    /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
                    (ICF_NULL != p_rgm_context))

    
    if (ICF_DUMMY_APP_ID == p_crm_pdb->recd_con_app_id)
    {
        if ((ICF_TRUE == p_crm_pdb->p_config_data->auto_register) &&
                (ICF_TRUE == p_rgm_context->auto_reg_flag))
        {
            p_rgm_context->auto_reg_flag = ICF_FALSE;
            p_rgm_context->event_auto_reg = 0;
            p_rgm_context->event_auto_reg =
                    ICF_AUTO_REG_EXPIRED;
           
        }/* auto_reg scenario*/
    }/* Contact had DUMMY_APP_ID in the uri */

    else
    {
        temp_app_id = p_crm_pdb->recd_con_app_id;
        /* Inform the app if present in rgm_context: */         
         if (ICF_TRUE == icf_rgm_fetch_app_info(
                        p_crm_pdb,p_rgm_context,
                          temp_app_id, &p_temp_app_ctx))
         {
             /* indicate the app of network initiated dereg*/
             register_response.p_crm_pdb = p_crm_pdb;
             register_response.app_id = temp_app_id;
             register_response.line_id = p_temp_app_ctx->line_id;
             register_response.options_flag = ICF_NULL;
             /*use rgm_ctxt for To/From*/

             register_response.options_flag |= ICF_RGM_CTXT_COPY;
             register_response.p_rgm_context = p_rgm_context;
             register_response.p_request_payload = ICF_NULL;
             /* Network initiated DEREG happened */
             register_response.request_type = ICF_NULL;;
             register_response.status = ICF_FAILURE;
             register_response.error_cause = ICF_CAUSE_NW_EXPIRATION;
             /*for CFG to not send response*/
              p_crm_pdb->resp_to_be_sent = ICF_FALSE;
              ret_val = icf_rgm_send_to_app(p_crm_pdb->p_glb_pdb,
                  &register_response, p_crm_pdb->p_ecode);
              
              /* Remove app from rgm_context */
              icf_rgm_find_n_remove_app(
                  p_crm_pdb,temp_app_id,p_rgm_context,
                  p_crm_pdb->p_config_data,p_crm_pdb->p_ecode);    
                }/* app_found in rgm_ctxt: ends */
    }/* non-dummy-app-id contact: ends */    

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    
}/* Function icf_rgm_nfy_con_expired() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_nfy_con_deactivated
 *
 * DESCRIPTION:         This function is invoked when valid
 *                      contact with state=terminated and
 *                      event=deactivated is received in REGISTERED state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_nfy_con_deactivated(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_rgm_register_resp_st    register_response;
    icf_rgm_app_ctxt_st      *p_temp_app_ctx = ICF_NULL;
    icf_app_id_t             temp_app_id = ICF_NULL;
        
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

       /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
                    (ICF_NULL != p_rgm_context))

    if (ICF_DUMMY_APP_ID == p_crm_pdb->recd_con_app_id)
    {
        if ((ICF_TRUE == p_crm_pdb->p_config_data->auto_register) &&
                (ICF_TRUE == p_rgm_context->auto_reg_flag))
        {
            p_rgm_context->indicator |=
                    ICF_RGM_NFY_ON_DEACTIVATION;

            p_rgm_context->event_auto_reg = 0;
            p_rgm_context->event_auto_reg =
                    ICF_AUTO_REG_DEACTIVATED;
            
        }/* auto_reg scenario*/
    }/* Contact had DUMMY_APP_ID in the uri */

    else
    {
        temp_app_id = p_crm_pdb->recd_con_app_id;
        /* Inform the app if present in rgm_context: */         
         if (ICF_TRUE == icf_rgm_fetch_app_info(
                        p_crm_pdb,p_rgm_context,
                          temp_app_id, &p_temp_app_ctx))
         {
             /* indicate the app of network initiated dereg*/
             register_response.p_crm_pdb = p_crm_pdb;
             register_response.app_id = temp_app_id;
             register_response.line_id = p_temp_app_ctx->line_id;
             register_response.options_flag = ICF_NULL;
             /*use rgm_ctxt for To/From*/

             register_response.options_flag |= ICF_RGM_CTXT_COPY;
             register_response.p_rgm_context = p_rgm_context;
             register_response.p_request_payload = ICF_NULL;
             /* Network initiated DEREG happened */
             register_response.request_type = ICF_NULL;;
             register_response.status = ICF_FAILURE;
             register_response.error_cause = ICF_CAUSE_NW_DEACTIVATED;
             /*for CFG to not send response*/
              p_crm_pdb->resp_to_be_sent = ICF_FALSE;
              ret_val = icf_rgm_send_to_app(p_crm_pdb->p_glb_pdb,
                  &register_response, p_crm_pdb->p_ecode);          

              /* reset the CFM bit so that success can be conveyed */                  
              p_temp_app_ctx->app_indicator &= 
                        ~ICF_RGM_APP_CFM_SENT;

              /* value set in indicator bitmask so that REGISTER can be
               * sent once all contacts are processed */
              p_rgm_context->indicator |=
                    ICF_RGM_NFY_ON_DEACTIVATION;
         }
              
    }/* non-dummy-app-id contact: ends */    


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    
}/* Function icf_rgm_nfy_con_deactivated() ends here */


/****************************************************************************
 * FUNCTION:            icf_rgm_nfy_con_rejected
 *
 * DESCRIPTION:         This function is invoked when valid
 *                      contact with state=terminated and
 *                      event=rejected is received in REGISTERED state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_nfy_con_rejected(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_rgm_register_resp_st    register_response;
    icf_rgm_app_ctxt_st      *p_temp_app_ctx = ICF_NULL;
    icf_app_id_t             temp_app_id = ICF_NULL;
        
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

    /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
                    (ICF_NULL != p_rgm_context))


     if (ICF_DUMMY_APP_ID == p_crm_pdb->recd_con_app_id)
    {
        if ((ICF_TRUE == p_crm_pdb->p_config_data->auto_register) &&
                (ICF_TRUE == p_rgm_context->auto_reg_flag))
        {
            p_rgm_context->auto_reg_flag = ICF_FALSE;
            p_rgm_context->event_auto_reg = 0;
            p_rgm_context->event_auto_reg =
                    ICF_AUTO_REG_REJECTED;
        }/* auto_reg scenario*/
    }/* Contact had DUMMY_APP_ID in the uri */

    else
    {
        temp_app_id = p_crm_pdb->recd_con_app_id;
        /* Inform the app if present in rgm_context: */         
         if (ICF_TRUE == icf_rgm_fetch_app_info(
                        p_crm_pdb,p_rgm_context,
                          temp_app_id, &p_temp_app_ctx))
         {
             /* indicate the app of network initiated dereg*/
             register_response.p_crm_pdb = p_crm_pdb;
             register_response.app_id = temp_app_id;
             register_response.line_id = p_temp_app_ctx->line_id;
             register_response.options_flag = ICF_NULL;
             /*use rgm_ctxt for To/From*/

             register_response.options_flag |= ICF_RGM_CTXT_COPY;
             register_response.p_rgm_context = p_rgm_context;
             register_response.p_request_payload = ICF_NULL;
             /* Network initiated DEREG happened */
             register_response.request_type = ICF_NULL;;
             register_response.status = ICF_FAILURE;
             register_response.error_cause = ICF_CAUSE_NW_REJECTED;
             /*for CFG to not send response*/
              p_crm_pdb->resp_to_be_sent = ICF_FALSE;
              ret_val = icf_rgm_send_to_app(p_crm_pdb->p_glb_pdb,
                  &register_response, p_crm_pdb->p_ecode);
              
              /* Remove app from rgm_context */
              icf_rgm_find_n_remove_app(
                  p_crm_pdb,temp_app_id,p_rgm_context,
                  p_crm_pdb->p_config_data,p_crm_pdb->p_ecode);    
                }/* app_found in rgm_ctxt: ends */
    }/* non-dummy-app-id contact: ends */    

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    
}/* Function icf_rgm_nfy_con_rejected() ends here */


/****************************************************************************
 * FUNCTION:            icf_rgm_nfy_con_unregistered
 *
 * DESCRIPTION:         This function is invoked when valid
 *                      contact with state=terminated and
 *                      event=unregistered is received in REGISTERED state.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_nfy_con_unregistered(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_rgm_context_st        *p_rgm_context = ICF_NULL;
    icf_rgm_register_resp_st    register_response;
    icf_rgm_app_ctxt_st      *p_temp_app_ctx = ICF_NULL;
    icf_app_id_t             temp_app_id = ICF_NULL;
        
    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

     /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
                    (ICF_NULL != p_rgm_context))


    if (ICF_DUMMY_APP_ID == p_crm_pdb->recd_con_app_id)
    {
        if ((ICF_TRUE == p_crm_pdb->p_config_data->auto_register) &&
                (ICF_TRUE == p_rgm_context->auto_reg_flag))
        {
            p_rgm_context->auto_reg_flag = ICF_FALSE;
            p_rgm_context->event_auto_reg = 0;
            p_rgm_context->event_auto_reg =
                    ICF_AUTO_REG_UNREGISTERED;
           
        }/* auto_reg scenario*/
    }/* Contact had DUMMY_APP_ID in the uri */

    else
    {
        temp_app_id = p_crm_pdb->recd_con_app_id;
        /* Inform the app if present in rgm_context: */         
         if (ICF_TRUE == icf_rgm_fetch_app_info(
                        p_crm_pdb,p_rgm_context,
                          temp_app_id, &p_temp_app_ctx))
         {
             /* indicate the app of network initiated dereg*/
             register_response.p_crm_pdb = p_crm_pdb;
             register_response.app_id = temp_app_id;
             register_response.line_id = p_temp_app_ctx->line_id;
             register_response.options_flag = ICF_NULL;
             /*use rgm_ctxt for To/From*/

             register_response.options_flag |= ICF_RGM_CTXT_COPY;
             register_response.p_rgm_context = p_rgm_context;
             register_response.p_request_payload = ICF_NULL;
             /* Network initiated DEREG happened */
             register_response.request_type = ICF_NULL;;
             register_response.status = ICF_FAILURE;
             register_response.error_cause = ICF_CAUSE_NW_UNREGISTERED;
             /*for CFG to not send response*/
              p_crm_pdb->resp_to_be_sent = ICF_FALSE;
              ret_val = icf_rgm_send_to_app(p_crm_pdb->p_glb_pdb,
                  &register_response, p_crm_pdb->p_ecode);
              
              /* Remove app from rgm_context */
              icf_rgm_find_n_remove_app(
                  p_crm_pdb,temp_app_id,p_rgm_context,
                  p_crm_pdb->p_config_data,p_crm_pdb->p_ecode);    
                }/* app_found in rgm_ctxt: ends */
    }/* non-dummy-app-id contact: ends */    

    

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    
}/* Function icf_rgm_nfy_con_unregistered() ends here */


/****************************************************************************
 * FUNCTION:            icf_rgm_ctx_handled_after_nfy
 *
 * DESCRIPTION:         This function is invoked after all
 *                      contacts of the NOTIFY are handled.
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_ctx_handled_after_nfy(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_rgm_context_st          *p_rgm_context = ICF_NULL;
    icf_void_t                  *p_buff = ICF_NULL;
    icf_timer_data_st           *p_timer_data = ICF_NULL;
    icf_timer_type_t             timer_type = ICF_NULL;
    icf_uint32_t                 duration = ICF_NULL;
    icf_uint32_t                 loop_app = ICF_NULL;
    icf_boolean_t                all_con_probation = ICF_FALSE;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

     /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
                    (ICF_NULL != p_rgm_context))

    if (ICF_RGM_NFY_ON_PROBATION &
            p_rgm_context->indicator)
    {        
       /* stop the currently running probation timer */
       if (ICF_TIMER_ID_INVALID != p_rgm_context->prbn_timer_id)
       {
           ICF_STOP_TIMER(
               p_crm_pdb->p_glb_pdb,
               p_rgm_context->prbn_timer_id,
               p_crm_pdb->p_ecode,
               ret_val)
               
               /* free the memory allocated to any existing timer data block */
               if ((ICF_SUCCESS == ret_val) &&
                   (ICF_NULL != p_rgm_context->p_timer_buff))
               {
                   
                   ret_val = icf_dbm_free_timer_block(
                       p_crm_pdb->p_glb_pdb,
                       p_crm_pdb->p_rgm_context->p_prbn_timer_buff,
                       p_crm_pdb->p_ecode);
                   
                   /* reset timer buffer pointer in rgm_context */
                   p_rgm_context->p_prbn_timer_buff = ICF_NULL;
                   
                   /* reset the timer id in rgm_context */
                   p_rgm_context->prbn_timer_id = ICF_TIMER_ID_INVALID;
               } /* End if(ret_val && p_timer_buff) */
               
       } /* End if(valid prbn_timer_id) */

    /* start the retry_after timer with new value */
       timer_type = ICF_TIMER_TYPE_PROBATION;
       duration = p_rgm_context->nfy_retry_timer_value;

        /* Allocate a timer block */
        ret_val = icf_dbm_get_mem_from_pool(p_crm_pdb->p_glb_pdb,
                (icf_pool_id_t)(ICF_MEM_TIMER),
                &p_buff,
                p_crm_pdb->p_ecode);

        /* Typecast */
        p_timer_data = (icf_timer_data_st *)(p_buff);
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_rgm_prepare_timer_struct(
                p_crm_pdb->p_glb_pdb,
                timer_type,
                (icf_void_t *)p_rgm_context,
                p_timer_data);

            ICF_START_TIMER(
                p_crm_pdb->p_glb_pdb,p_timer_data,
                    p_rgm_context->prbn_timer_id,
                    duration,p_crm_pdb->p_ecode,ret_val)
                
            /* Pointer should be kept in registration context */    
            if (ICF_SUCCESS == ret_val)
            {
                p_rgm_context->p_prbn_timer_buff = p_timer_data;
                
                /* debug traces */
                ICF_PRINT(("\n[REGM]: Started timer-type = %d of \
                    timer-id = %ld and duration = %ld ms\n", \
                    timer_type, p_rgm_context->prbn_timer_id, duration));
                
                    /* Change state to UNREGISTERED if all contacts 
                * are in PROBATION  */
                if ((ICF_FALSE == p_rgm_context->auto_reg_flag) &&
                    (ICF_AUTO_REG_PROBATION ==
                                p_rgm_context->event_auto_reg))
                {
                    all_con_probation = ICF_TRUE;
                    for (loop_app = 0; loop_app < 
                        p_crm_pdb->p_config_data->max_applications;
                    loop_app++)
                    {
                        if (ICF_FALSE == 
                            p_rgm_context->p_app_data_list[loop_app].is_valid)
                        {
                            continue;
                        }
                        if (!(p_rgm_context->p_app_data_list[loop_app].
                            app_indicator & 
                            ICF_RGM_APP_DEREG_ON_PROBATION))
                        {
                            all_con_probation = ICF_FALSE;
                            break;                        
                        }
                    }/* For loop on the applications ends here */
                    if ((ICF_TRUE == all_con_probation) &&
                        (ICF_RGM_03_REGISTERED == 
                        p_rgm_context->rgm_state))
                    {
                        /* Stop registration timer */
                         icf_rgm_stop_timer(p_crm_pdb);
                        /* Set state as UNREGISTERED */
                         p_rgm_context->rgm_state =
                                ICF_RGM_02_UNREGISTERED;
                    }
                    
                }/* auto_reg_flag: false ends */

                
            }/* probn_timer_started_success: ends here */
            
            else
            {
                /* Free timer block */
                icf_dbm_free_timer_block(
                        p_crm_pdb->p_glb_pdb,
                        (icf_void_t *)(p_rgm_context->p_prbn_timer_buff),
                        p_crm_pdb->p_ecode);    
            }

        }
        
    }/* Probation set in the indicator flag: ends here  */
    
    if (ICF_RGM_NFY_ON_DEACTIVATION &
             p_rgm_context->indicator)
    {
        /* send_register */
        if (ICF_FAILURE == icf_rgm_save_smallest_dur(
            p_crm_pdb->p_glb_pdb,
            p_crm_pdb->p_rgm_context,p_crm_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        else if (ICF_NULL !=
                        p_rgm_context->rgm_duration)
        {
            ret_val = icf_rgm_register_user( p_crm_pdb,p_rgm_context);
        }
    }/* Some contact was received with deactivated event. */
           


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    


}/* Function icf_rgm_ctx_handled_after_nfy() ends here */

/****************************************************************************
 * FUNCTION:            icf_rgm_verify_ctx_status
 *
 * DESCRIPTION:         This function is invoked after NOTIFY msg
 *                      body has been processed.
 *                      It verifies the state of all rgm_ctx' to see
 *                      if all contacts have been terminated with
 *                      no further REGISTER needed to be sent on the
 *                      network, in which case rgm_ctx' will be cleared
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_verify_ctx_status(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t                ret_val = ICF_FAILURE;
    icf_rgm_context_st          *p_rgm_context = ICF_NULL;
    icf_boolean_t               valid_app_present = ICF_FALSE;
    icf_uint32_t                loop_app = ICF_NULL;
	icf_uint16_t					j=1;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

	while (ICF_SUCCESS == icf_dbm_get_used_rgn_blk(
                p_crm_pdb->p_glb_pdb,
                (icf_uint16_t)j,
                &p_rgm_context,
                p_crm_pdb->p_ecode))
	{
		/* see if there is any valid entries */
		p_crm_pdb->p_rgm_context = p_rgm_context;
		valid_app_present = ICF_FALSE;
		for (loop_app = 0; loop_app < 
					p_crm_pdb->p_config_data->max_applications;
						loop_app++)
		{
			if (ICF_FALSE == 
				p_rgm_context->p_app_data_list[loop_app].is_valid)
			{
				continue;
			}
			else
			{
				valid_app_present = ICF_TRUE;
				break;
			}
		}/* For loop on the applications ends here */

		if ((ICF_FALSE == valid_app_present) &&
			(ICF_FALSE == p_rgm_context->auto_reg_flag) &&
			(ICF_AUTO_REG_PROBATION != p_rgm_context->event_auto_reg) &&
			(ICF_AUTO_REG_DEACTIVATED != p_rgm_context->event_auto_reg))
		{
			/* all contacts have been terminated and no further
			 * REGISTER will be sent on the network. Hence rgm_ctx will
			 * be cleared */

			/* Stop the timer */
			icf_rgm_stop_timer(p_crm_pdb);

			/* Clear the context */
			icf_rgm_clear_reg_context(
						p_crm_pdb,
						p_rgm_context);

			p_crm_pdb->p_rgm_context = ICF_NULL;

		}
		else
		{
			ret_val = ICF_SUCCESS;
			j++;
		}
	}

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    

}/* Function icf_rgm_verify_ctx_status() ends here */




ICF_4_0_CRM_END
#endif
/****************************************************************************
 * FUNCTION:            icf_rgm_refresh_register
 *
 * DESCRIPTION:         This function is invoked after 200
 *                      is received in AUTHORIZING or REGISTERING
 *                      or DEREGISTERING state. 
 *                       Incase probation timer expired while rgm_context
 *                      was waiting for response, 
 *                      a refresh REGISTER should be sent 
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_refresh_register(
        INOUT icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_rgm_context_st          *p_rgm_context = ICF_NULL;
    icf_boolean_t                send_register   = ICF_FALSE;
    icf_boolean_t                valid_app_present = ICF_FALSE;
    icf_uint32_t                 loop_app = ICF_NULL;
    
    icf_duration_t            default_duration = 
                                ICF_RGM_DEFAULT_DURATION_NON_IMS;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb) 

     /* local pointer to rgm_context */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, 
                    (ICF_NULL != p_rgm_context))
#ifdef IMS_CLIENT
    /*Extract the default duration of the registration based on the IMS and NON-IMS*/
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
    default_duration = ICF_RGM_DEFAULT_DURATION_IMS_CLIENT;
ICF_CHECK_IMS_END
#endif
    if (!(ICF_RGM_REFRESH_REGISTER_ON_PRBN &
            p_rgm_context->indicator))
                
    {
        ret_val = ICF_SUCCESS;
    }
    else
    {
    /* reset the probation bit value for app_entries also.
        * also reset the CFM bit so that success may be conveyed. */
        for (loop_app = 0; loop_app < p_crm_pdb->p_config_data->max_applications;
        loop_app++)
        {
            if (ICF_FALSE == 
                p_rgm_context->p_app_data_list[loop_app].is_valid)
            {
                continue;
            }
            if (p_rgm_context->p_app_data_list[loop_app].app_indicator & 
                ICF_RGM_APP_DEREG_ON_PROBATION)
            {
                p_rgm_context->p_app_data_list[loop_app].app_indicator &=
                    ~ICF_RGM_APP_DEREG_ON_PROBATION;
                
                p_rgm_context->p_app_data_list[loop_app].app_indicator &=
                    ~ICF_RGM_APP_CFM_SENT;
                
            }
        }/* For loop on the applications ends here */


        /* see if there is any valid entries */
        valid_app_present = ICF_FALSE;
        for (loop_app = 0; loop_app < 
                p_crm_pdb->p_config_data->max_applications;
                loop_app++)
        {
            if (ICF_FALSE == 
                p_rgm_context->p_app_data_list[loop_app].is_valid)
            {
                continue;
            }
            else
            {
                valid_app_present = ICF_TRUE;
                break;
            }
        }/* For loop on the applications ends here */

        if (ICF_TRUE == valid_app_present ||
                ICF_TRUE == p_rgm_context->auto_reg_flag)                
        {
            /*(valid_app_entry) or auto_reg_flag TRUE
            then send_register*/
            /* send a new register */
            send_register = ICF_TRUE;
            ret_val = ICF_SUCCESS;                   
            
        }/* REGISTER needs to be sent: ends here */
        else
        {
            /*clear the rgm-context*/
#ifdef ICF_IPSEC_ENABLED
            
            /* Clear all contexts */
            
            if (ICF_TRUE == 
                    p_crm_pdb->p_config_data->ipsec_enabled)
            {
                /* Delete any existing SAs */
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = 
                        ICF_RGM_IPSEC_SA_ALL;
                
                ret_val = icf_rgm_ipsec_sa_clean_up(
                    p_crm_pdb, 
                    ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP);
            } /* End if (ipsec_enabled) */
            else
            {
#endif /* ICF_IPSEC_ENABLED */
                
                /* Clear registration context */
                ret_val = icf_rgm_clear_reg_context(
                    p_crm_pdb,
                    p_rgm_context);            
                
                /* Ask Aashish */
                ret_val = ICF_SUCCESS;
                
#ifdef ICF_IPSEC_ENABLED
            } /* End else(ipsec_enabled) */
#endif /* ICF_IPSEC_ENABLED */
        }/* no valid entry present: ends here */            

    }/* indicator: REFRESH_REGISTER: ends here */ 
    
    /* reset the REFRESH_REGISTER value in indicator */
    p_rgm_context->indicator &=
                ~ICF_RGM_REFRESH_REGISTER_ON_PRBN;

    /* Sending REGISTER */
    if ((ICF_SUCCESS == ret_val) &&
        (ICF_TRUE == send_register))
    {
        /* send_register */
        if (ICF_FAILURE == icf_rgm_save_smallest_dur(
            p_crm_pdb->p_glb_pdb,
            p_crm_pdb->p_rgm_context,p_crm_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        else if ((ICF_NULL ==
                        p_rgm_context->rgm_duration) &&
                        (ICF_TRUE ==
                            p_rgm_context->auto_reg_flag))
        {
            p_rgm_context->rgm_duration = 
                default_duration;
            ret_val = icf_rgm_register_user(
                        p_crm_pdb,p_rgm_context);

        }
        else if (ICF_NULL !=
                        p_rgm_context->rgm_duration)
        {
            ret_val = icf_rgm_register_user( p_crm_pdb,p_rgm_context);
        }
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;    


}/* Function icf_rgm_refresh_register() ends here */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_stop_timer
 *
 * DESCRIPTION:     This is a utility function that will stop the currently 
 *                  running timer and free the timer block. It does not need the
 *                  timer-id for the timer to be stopped, since currently REGM 
 *                  runs only one timer at a time.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_stop_timer(
        INOUT   icf_crm_pdb_st        *p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    if (ICF_TIMER_ID_INVALID != p_crm_pdb->p_rgm_context->timer_id)
    {
        /* debug traces */
        ICF_PRINT(((icf_uint8_t *)"\n[REGM]: Stopping timer with timer-id = %ld\n",
            p_crm_pdb->p_rgm_context->timer_id));
        
        /* stop the currently running timer */
        ICF_STOP_TIMER(
            p_crm_pdb->p_glb_pdb,
            p_crm_pdb->p_rgm_context->timer_id,
            p_crm_pdb->p_ecode,
            ret_val)
            
        /* free the memory allocated to any existing timer data block */
        if ((ICF_SUCCESS == ret_val) &&
            (ICF_NULL != p_crm_pdb->p_rgm_context->p_timer_buff))
        {
            
            ret_val = icf_dbm_free_timer_block(
                            p_crm_pdb->p_glb_pdb,
                            p_crm_pdb->p_rgm_context->p_timer_buff,
                            p_crm_pdb->p_ecode);
            
            /* reset timer buffer pointer in rgm_context */
            p_crm_pdb->p_rgm_context->p_timer_buff = ICF_NULL;
            
            /* reset the timer id in rgm_context */
            p_crm_pdb->p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;   
        
        } /* End if(ret_val && p_timer_buff) */
    } /* End if(timer_id) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_stop_timer() */


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_get_timestamp
 *
 * DESCRIPTION:     This is a utility function that will return the current 
 *                  system time in milliseconds.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_get_timestamp(
        INOUT   icf_crm_pdb_st        *p_crm_pdb,
        OUT        icf_time_t            *p_time)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_time_t                        curr_time = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* get the current system time in seconds */
    ICF_PORT_TIME(ICF_NULL, curr_time, ret_val)
    
    if (ICF_SUCCESS == ret_val)
    {
        /* return the current time in milliseconds */
        *p_time = curr_time * 1000;
    }
    
    /* debug traces */
    ICF_PRINT(("\n[REGM]: Returning timestamp = %ld ms\n", *p_time));
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_get_timestamp() */
#endif

#ifdef IMS_CLIENT
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_get_next_buffer_id
 *
 * DESCRIPTION:     This is a utility function that returns the next available
 *                  buffer id.
 *                  
 *
 * RETURNS:         icf_uint8_t
 *
 ******************************************************************************/
icf_uint8_t icf_rgm_get_next_buffer_id(icf_crm_pdb_st *p_crm_pdb)
{
    icf_uint8_t buf_id = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->last_buffer_id;
    if(buf_id == p_crm_pdb->p_config_data->max_registration_req)
        buf_id = 1;
    else
        buf_id ++;

    ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->last_buffer_id = buf_id;
    return buf_id;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_check_for_buffer_condition
 *
 * DESCRIPTION:     This is a utility function that checks for buffering condition
 *                  
 *                  
 *
 * RETURNS:         ICF_TRUE if the current registration procedure should be 
 *                  buffered. ICF_FALSE otherwise
 *
 ******************************************************************************/

icf_boolean_t icf_rgm_check_for_buffer_condition(icf_crm_pdb_st *p_crm_pdb)
{
    /* Registration event buffering */
    if (

        (ICF_TRUE == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->reg_ongoing)
#ifdef ICF_IPSEC_ENABLED
        || (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_sa_resp_awaiting)
        || ((ICF_TRUE == p_crm_pdb->p_config_data->ipsec_enabled) &&
           (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context) &&
           (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context))
#endif /* ICF_IPSEC_ENABLED */        
        )
    {
        return ICF_TRUE;
    }
    return ICF_FALSE;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_process_buffer
 *
 * DESCRIPTION:     This is a utility function that processes the buffered list
 *                  and invokes the next buffered registration block
 *                  
 *
 * RETURNS:         icf_return_t 
 *
 ******************************************************************************/
icf_return_t icf_process_buffer(icf_crm_pdb_st *p_crm_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_rgm_context_st *p_buffered_rgn_block = ICF_NULL;
	icf_rgm_glb_data_st *p_rgm_data=ICF_NULL;
    icf_uint8_t current_buf_id;

    current_buf_id = p_crm_pdb->p_rgm_context->buffer_id;
    p_crm_pdb->p_rgm_context->buffer_id = 0;

	ret_val = icf_dbm_get_module_glb_data(     
                p_crm_pdb->p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_REGM,
                (icf_void_t *)&p_rgm_data,
                p_crm_pdb->p_ecode);
   
    /* Current blocks processing is finished, reset it's buffer id*/
    
	if(p_rgm_data->last_buffer_id == current_buf_id)
	{
		/* This was the last block in the buffered list */
		/* reset the last buffered id */
		p_rgm_data->last_buffer_id = 0;
		return ICF_SUCCESS;
	}

    if(ICF_TRUE == icf_rgm_check_for_buffer_condition(p_crm_pdb))
    {
       return ICF_SUCCESS;
    }

    ret_val = icf_dbm_get_next_buffered_reg_ctxt(p_crm_pdb->p_glb_pdb,
        current_buf_id,
        &p_buffered_rgn_block,
        p_crm_pdb->p_ecode);
   
    /* Current blocks processing is finished, reset it's buffer id*/
    if(ICF_NULL != p_buffered_rgn_block)
    {
        icf_rgm_context_st *p_old_reg_ctxt = p_crm_pdb->p_rgm_context;
        p_crm_pdb->p_rgm_context = p_buffered_rgn_block;
        if(0 == p_buffered_rgn_block->rgm_duration)
        {
            ret_val = icf_rgm_deregister_user(p_crm_pdb,p_buffered_rgn_block);
        }
        else
        {
            ret_val = icf_rgm_register_user(p_crm_pdb,p_buffered_rgn_block);
        }
        p_crm_pdb->p_rgm_context = p_old_reg_ctxt;
    }
    return ret_val;
}
#endif

#ifdef ICF_SIGCOMP_SUPPORT
/****************************************************************************
 * FUNCTION:           icf_rgm_send_create_comp_req_to_sigsa
 *
 * DESCRIPTION:        This routine Sends CREATE_COMP_REQUEST TO
 *                     SIGSA for Creating the compartment for sending
 *                     Compressed Message
 *****************************************************************************/
icf_return_t icf_rgm_send_create_comp_req_to_sigsa(
        INOUT  icf_crm_pdb_st   *p_crm_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_internal_msg_st      temp_internal_msg;
    icf_create_comp_req_st   create_comp_req;

    temp_internal_msg.msg_hdr.msg_id = ICF_CREATE_COMP_REQ;
    temp_internal_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb;
    temp_internal_msg.msg_hdr.payload_length = 
                                ICF_PORT_SIZEOF(icf_create_comp_req_st);
    temp_internal_msg.msg_hdr.status = 0;
	create_comp_req.p_rgm_ctxt = ICF_NULL;

	temp_internal_msg.p_msg_data = (icf_uint8_t*)&create_comp_req;

    ret_val = icf_sigsa_process_mesg(&temp_internal_msg);
    return ret_val;

}


/****************************************************************************
 * FUNCTION:           icf_rgm_send_delete_comp_ind_to_sigsa
 *
 * DESCRIPTION:        This routine Sends ICF_DELETE_COMP_IND TO
 *                     SIGSA for deleting the compartment 
 *****************************************************************************/
icf_return_t icf_rgm_send_delete_comp_ind_to_sigsa(
        INOUT  icf_crm_pdb_st   *p_crm_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_internal_msg_st      temp_internal_msg;
    icf_delete_comp_ind_st   delete_comp_ind;

    temp_internal_msg.msg_hdr.msg_id = ICF_DELETE_COMP_IND;
    temp_internal_msg.msg_hdr.p_glb_pdb = p_crm_pdb->p_glb_pdb;
    temp_internal_msg.msg_hdr.payload_length = 
                                ICF_PORT_SIZEOF(icf_delete_comp_ind_st);
    temp_internal_msg.msg_hdr.status = 0;

	delete_comp_ind.p_rgm_ctxt = ICF_NULL;

    temp_internal_msg.p_msg_data = (icf_uint8_t*)&delete_comp_ind;

    ret_val = icf_sigsa_process_mesg(&temp_internal_msg);

    return ret_val;
}
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_make_rgm_context
 *
 * DESCRIPTION:     This is a utility function that makes a new rrm_context for a 
 *					user
 *                  
 *
 * RETURNS:         icf_return_t 
 *
 ******************************************************************************/
icf_return_t icf_rgm_make_rgm_context(
						INOUT	icf_glb_pdb_st		*p_glb_pdb,
						OUT		icf_rgm_context_st	**p_p_rgn_cntxt_blk,
						OUT		icf_error_t			*p_ecode)
{
    icf_return_t            ret_val = ICF_FAILURE;
    icf_uint32_t            loop_index = ICF_NULL;
    icf_return_t            dummy_ret_val = ICF_SUCCESS;
	icf_config_data_st   *p_config_data = ICF_NULL;

	ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
        (icf_int_module_id_t)ICF_INT_MODULE_CFG,
        (icf_void_t *)&(p_config_data),
        p_ecode);
    
    /* now allocate a new regm_cntxt */
    ret_val = icf_dbm_alloc_rgn_blk (p_glb_pdb, 
		p_p_rgn_cntxt_blk, p_ecode); 
    
    if (ICF_SUCCESS == ret_val)
    {
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
        /* The retry count is being made NULL */
        (*p_p_rgn_cntxt_blk)->retry_count = ICF_NULL;
ICF_CHECK_IMS_END
#endif            
        /* making auth_retry_count as NULL */

        (*p_p_rgn_cntxt_blk)->auth_retry_count = ICF_NULL;

        
        /* Initialize the indicator mask with NULL */
        (*p_p_rgn_cntxt_blk)->indicator = ICF_NULL;
        
        /* Initialize the rgm_state*/
        (*p_p_rgn_cntxt_blk)->rgm_state = ICF_RGM_00_IDLE;

ICF_4_0_CRM_START
        /*Fix done for SPR-18722 : CSR-1-6068965*/
        /* Initializing the auth_data structures with NULL .*/

        ICF_MEMFREE(p_glb_pdb,
                        (*p_p_rgn_cntxt_blk)->auth_data_sent.p_nonce,
                            ICF_MEM_COMMON,
                            p_ecode, dummy_ret_val)

        ICF_MEMFREE(p_glb_pdb,
                        (*p_p_rgn_cntxt_blk)->auth_data_recd.p_nonce,
                            ICF_MEM_COMMON,
                            p_ecode, dummy_ret_val)

        icf_port_memset(
            (icf_void_t*)&((*p_p_rgn_cntxt_blk)->auth_data_recd),
            (icf_uint8_t)ICF_NULL,
            (icf_uint32_t) ICF_PORT_SIZEOF(icf_rgm_auth_data_st));


        icf_port_memset(
            (icf_void_t*)&((*p_p_rgn_cntxt_blk)->auth_data_sent),
            (icf_uint8_t)ICF_NULL,
            (icf_uint32_t) ICF_PORT_SIZEOF(icf_rgm_auth_data_st));

        

ICF_4_0_CRM_END

        for (loop_index = 0; loop_index < p_config_data->max_applications;
        loop_index++)
        {
            /* Make the application entries ready for use */
            (*p_p_rgn_cntxt_blk)->p_app_data_list[loop_index].
                is_valid = ICF_FALSE;
            
            (*p_p_rgn_cntxt_blk)->p_app_data_list[loop_index].
                app_indicator = ICF_NULL;
            
            /* Making the list counts as NULL */
            ((*p_p_rgn_cntxt_blk)->p_app_data_list[loop_index].
                header_list.count)
                = ICF_NULL;
#if 0
    /*Rizvi: Will be tested and added later*/

            /* Making the list counts as NULL */
            ((*p_p_rgn_cntxt_blk)->p_app_data_list[loop_index].
                inc_header_list.count)
                = ICF_NULL;
#endif


            ((*p_p_rgn_cntxt_blk)->p_app_data_list[loop_index].
                tag_list.count)
                = ICF_NULL;
            ((*p_p_rgn_cntxt_blk)->p_app_data_list[loop_index].
                body_list.count)
                = ICF_NULL;
            
            
            ret_val = ICF_SUCCESS;
            
            
        }/* For loop: app_data_list: Processing Ends */
        
    }/* Unused Registration context found */
    else
    {
        *p_ecode = ICF_RGN_CTX_BLK_EXHAUSTED;
    }
	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cfg_rgm_deregister_user_on_deletion
 *
 * DESCRIPTION:     This is a utility function that is trigerred from cfg if a user/registrar
 *					is deleted from a line and this is the last user on this line
 *                  
 *
 * RETURNS:         icf_return_t 
 *
 ******************************************************************************/
icf_return_t icf_cfg_rgm_deregister_user_on_deletion(
						INOUT	icf_crm_pdb_st		*p_crm_pdb,
						OUT		icf_rgm_context_st	*p_rgm_context)
{
	icf_return_t                ret_val = ICF_SUCCESS;
    /* SPR 16019: Set the bit mask in rgm_context to indicate that
     * de-reg generated internally.
     */
    if(ICF_RGM_05_DEREGISTERING != p_rgm_context->rgm_state)
    {
        p_rgm_context->indicator |= ICF_RGM_INTERNAL_GENERATED_REG;

        icf_rgm_deregister_user(p_crm_pdb, p_rgm_context);
    }

	return ret_val;
}

#ifdef IMS_CLIENT
icf_return_t icf_rgm_clear_buffered_requests_for_user(icf_crm_pdb_st  *p_crm_pdb,
                                                      icf_address_st  *p_user_addr,
                                                      icf_error_t     *p_ecode)
{
    icf_rgm_context_st *p_buffered_rgm_block = ICF_NULL;
    icf_uint8_t removed_buffer_id,temp_buffer_id;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_rgm_app_ctxt_st   *p_app_data_src,*p_app_data_dest;
	icf_rgm_register_resp_st    register_response;


    p_app_data_src=p_app_data_dest =ICF_NULL;



    /* call dbm function to find the corresponding buffered block */
    if(ICF_FAILURE == icf_dbm_get_buffered_block_by_address(p_crm_pdb->p_glb_pdb,
        p_user_addr,
        &p_buffered_rgm_block,
        p_ecode))
    {
        return ICF_FAILURE;
    }
    if(ICF_NULL == p_buffered_rgm_block)
    {
        return ICF_SUCCESS;
    }

    p_buffered_rgm_block->p_registered_pub_uri = &(p_crm_pdb->p_rgm_context->rgm_user_address);
    register_response.p_crm_pdb = p_crm_pdb;
    register_response.p_rgm_context = p_buffered_rgm_block;
    register_response.options_flag = ICF_NULL;
    register_response.options_flag |= ICF_RGM_RESPONSE_CFM;
	register_response.options_flag |= ICF_RGM_FILL_URIS;

	register_response.line_id = p_buffered_rgm_block->rgm_list_of_lines[0];
	register_response.request_type = ICF_NULL;
	register_response.status = ICF_FAILURE;
	register_response.error_cause = ICF_CAUSE_IMPLICITLY_REGISTERED;

    register_response.p_rgm_context->indicator |= ICF_RGM_IMPLICITLY_REGISTERED;

    icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                            &register_response, p_ecode);
	/********************/

    removed_buffer_id = p_buffered_rgm_block->buffer_id;

    /* delete the buffered rgm_context */
    icf_rgm_clear_reg_context(p_crm_pdb,p_buffered_rgm_block);

    p_buffered_rgm_block = ICF_NULL;
    while(1)
    {
        icf_dbm_get_next_buffered_reg_ctxt(p_crm_pdb->p_glb_pdb,
                                                removed_buffer_id,
                                                &p_buffered_rgm_block,
                                                p_crm_pdb->p_ecode);
        if(ICF_NULL == p_buffered_rgm_block)
        {
            break;
        }
        if(p_buffered_rgm_block->buffer_id == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->last_buffer_id)
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->last_buffer_id = 
                p_buffered_rgm_block->buffer_id = removed_buffer_id;
            break;
        }
        else
        {
            temp_buffer_id = removed_buffer_id;
            removed_buffer_id = p_buffered_rgm_block->buffer_id;
            p_buffered_rgm_block->buffer_id = removed_buffer_id;
        }
    }
    return ret_val;
}
#endif

#ifdef ICF_NAT_RPORT_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_update_nat_binding_refresh_blk
 *
 * DESCRIPTION:     This is a utility function that is trigerred to update the 
 *                  binding refresh timer data in the global config data.
 *                  this will get the next active registration context block and
 *                  the new binding refresh timer will be started on this newly
 *                  obtained active registration block.
 *
 * RETURNS:         icf_return_t 
 *
 ******************************************************************************/

icf_return_t  icf_rgm_update_nat_binding_refresh_blk(
                INOUT    icf_glb_pdb_st        *p_glb_pdb, 
                INOUT    icf_crm_pdb_st        *p_crm_pdb, 
                OUT      icf_error_t            *p_ecode)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_uint16_t                 index = ICF_NULL;
    icf_rgm_context_st          *p_rgm_context = ICF_NULL;
    icf_rport_config_data_st    *p_rport_config_data = ICF_NULL;
    icf_line_data_st		*p_line_data = ICF_NULL;
 
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
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
       ret_val = ICF_FAILURE; 
    }
    else
    {
      while (ICF_SUCCESS == icf_dbm_get_used_rgn_blk(\
                p_glb_pdb,(icf_uint16_t)index,&p_rgm_context,
                p_ecode))
      {
         if(p_rgm_context->reg_id == p_rport_config_data->reg_id)
         {
            /* get the next used rgn block*/
            index++;
         }
         else
         {
            /* Another active used rgn block found.
             * Update this rgn block for reg id and break.
             */
            p_rgm_context->reg_id = p_rport_config_data->reg_id;
            if( p_rgm_context->rgm_duration != 0)
            {
                p_rport_config_data->init_reg_duration =\
                                                    p_rgm_context->rgm_duration;
            }

            p_rport_config_data->line_id = p_rgm_context->rgm_list_of_lines[0];

            icf_port_memcpy((icf_void_t*)&(p_rport_config_data->\
                        user_address),(icf_void_t*)&(p_rgm_context->\
                            rgm_user_address),(icf_uint32_t)\
                    ICF_PORT_SIZEOF(icf_address_st));
            break;
         }

       }/* While ends here */
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

icf_void_t icf_rgm_set_rport_status( 
                INOUT    icf_crm_pdb_st        *p_crm_pdb) 
{
    
    icf_rport_config_data_st    *p_rport_config_data=ICF_NULL;
    icf_line_data_st            *p_line_data = ICF_NULL;
    icf_user_db_node_st         *p_user_db = ICF_NULL;
    icf_boolean_t               match_found = ICF_FALSE;
    icf_rgm_context_st          *p_temp_rgm_context = ICF_NULL;
    
    if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_crm_pdb->p_glb_pdb, p_crm_pdb->p_rgm_context->line_id,
                    &p_line_data, p_crm_pdb->p_ecode))
     {
          return;
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

    p_user_db = p_line_data->p_user_db;
    while (ICF_NULL != p_user_db)
    {

	if (ICF_SUCCESS == icf_rgm_get_ctxt_for_pub_uri(p_crm_pdb->p_glb_pdb,
				p_crm_pdb->p_rgm_context->line_id,
				&(p_user_db->public_user),
				&p_temp_rgm_context, 
				p_crm_pdb->p_ecode))
	{
              if(ICF_TRUE == p_rport_config_data->is_rport_reqd &&
                  ICF_INITIAL_RPORT_INITIAL != p_temp_rgm_context->rport_status)
              {
                  match_found = ICF_TRUE;
                  break;
              }
                    
        }
        p_user_db = p_user_db->p_next;
     }
    
     if(ICF_TRUE == match_found)
     {
         p_crm_pdb->p_rgm_context->rport_status = ICF_INITIAL_RPORT_COMPLETED;
     }
     else
     {
          p_crm_pdb->p_rgm_context->rport_status = ICF_INITIAL_RPORT_INITIAL;
     }

     return;
} 

#endif
