/*------------------------------------------------------------------------------
 *
 * File name        : icf_vldt_api.c
 *
 * Purpose          : This file contains the funstions to validate 
 *                    the various API' fields in invoked API 
 *                    called by PA towards ICF.
 *
 * Revision History :
 *
 * Date         Name              Ref#            Comments
 * --------     ------------      ----            ---------------
 * 13-JUN-2007  Jagmohan Chauhan   No              Initial
 *              Rishi Saxena
 * 13-DEC-2007  Jagmohan Chauhan   No              SPR Fix 17640
 * 22-DEC-2007  Jagmohan Chauahn   No              Code added for rem_call_transfer_initiated_cfm
 * 14-JAN-2008	Shruti Thakral					   Changes for Secondary Server
 *												   Support in Rel 8.0
 * 16-Jan-2008  Kamini Gangwani    Rel 8.0         String Validation for access_network_info hdr. 
 * 18-Jan-2008  Kamini Gangwani    Rel 8.0         Validation for parameter media_transport_type
 * 21-Jan-2008  Abhishek Dhammawat Rel 8.0         Added precondition handling
 * 21-Jan_2008  Rajiv Kumar        Rel 8.0         Registration Procedure
 *                                                 Enhancements
 * 22-JAN-2008  Anurag Khare       IPTK 8.0        Change for DHCP Server Support
 * 07-MAR-2008  Vipul Varshney     SPR 18122       AKA Enhancement
 * 28-Mar-2008  Tarun Gupta        SPR 18311       Merged Publish-Message Enhancements
 * 07-Jun-2008  Jagmohan Chauhan   IPTK 8.0        SRTP LLD SRTP Enhancements
 * 25-Jun-2008  Abhishek Dhammawat IPTK 8.0        P-Hdr Enhancements
 * 10-Nov-2008  Rajiv Kumar        SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                                 19020)
 * 11-Dec-2008  Anurag Khare       SPR 19218       CSR 1-6456839 Merged
 * 11-Feb-2009  Ashutosh Mohan     Rel8.1          Added code for:
 *                                                 icf_vldt_icf_early_session_cfm_st
 *                                                 Also updated following:
 *                                                 icf_vldt_icf_set_call_params_req_st
 *                                                 icf_vldt_icf_options_req_st
 *                                                 icf_vldt_icf_early_incoming_call_cfm_st
 *                                                 icf_vldt_icf_create_call_req_st
 * 16-Feb-2009  Anurag Khare      Rel 8.1          icf_vldt_icf_call_modify_req_st
 * 18-Mar-2009  Alok Tiwari       Rel 8.1          icf_vldt_icf_register_req_st
 *                                                 has been modified to validate
 *                                                 the field contact_list_required.
 * 31-Mar-2009  Preksha           Rel 8.2          Additions for PRACK Enhancements 
 * 31-Mar-2009  Anuradha Gupta    Rel 8.2          Modified icf_vldt_icf_register_req_st
 * 31-Mar-2009  Alok Tiwari       Rel 8.2          Modified function: 
 *                                                 icf_vldt_icf_register_req_st
 *                                                 icf_vldt_icf_aka_auth_resp_st 
 * 22/04/09    Rajiv Kumar        ICF Rel8.2       RFC 4320 Implementation added.
 * 18-May-2009 Anuradha Gupta     SPR 19672        Modified fn icf_vldt_icf_set_call_params_req_st
 * 21-May-2009 Anuradha Gupta     SPR 19672        Modified fn icf_vldt_icf_set_call_params_req_st
 * 26-May-2009 Anuradha Gupta     SPR 19672        CSR 1-6667668 Merge 
 * 26-May-2009 Anuradha Gupta     SPR 19672        CSR 1-6727350 Merge 
 * 28-May-2009 Ashutosh Mohan     SPR 19672        CSR-1-6976670 Merged
 * 28-May-2009 Tarun Gupta        Rel 8.2          Async NOTIFY Support
 * 29-May-2009 Kamal Ashraf       SPR 19672        CSR_1_7113139 merged
 * 02-Jun-2009 Preksha            Rel 8.2          Asynchronus Message enhancements
 * 11-Jun-2009 Ashutosh Mohan     SPR 19737        Changes done for GCC 4.1 warnings removal
 * 17-Jun-2009 Tarun Gupta        SPR 19742        Modified icf_vldt_icf_generic_msg_req_st,
 *                                                 icf_vldt_icf_generic_msg_cfm_st
 * 03-Aug-2009 Tarun Gupta        SPR 19886        Merged CSR 1-7334184
 * 06-Aug-2009 Tarun Gupta        SPR 19886        Merged CSR 1-7402396
 * 08-Sep-2009 Anuradha Gupta     SPR 19999        Modfied icf_vldt_icf_register_req_st
 * 11-Sep-2009 Rajiv Kumar        SPR 19999        Merged CSR 1-7547597 
 * 29-Apr-2010 Aman Aggarwal      SPR 20391        Fix for CSR 1-8561947
 * 25-may-2010 Preksha            SPR 20412        Merged CSR 1-8389294
 * 29-May-2010 Alok Tiwari        SPR 20412        Merged CSR 1-8223807
 * 11-Oct-2010 Sachin Sharma      SPR 20697        CSR 1-9012191 Fixed. 
 *
 * Copyright (c) 2010, Aricent.
 *---------------------------------------------------------------------------*/

#include "icf_vldt_prototypes.h"   
#include "icf_vldt_defs.h"


/******************************************************************************
*
* FUNCTION:       icf_vldt_icf_register_req_st
*
* DESCRIPTION:    This function is invoked  to validate the 
*                 icf_register_req_st api.
*
*****************************************************************************/ 
icf_return_t icf_vldt_icf_register_req_st(
                          IN icf_uint8_t *p_payload, 
                          INOUT icf_vldt_err_st *p_vldt_err)    
{

    icf_return_t ret_val = ICF_SUCCESS;
   
    /* Initialsing register req local API structure  */
    icf_register_req_st *p_icf_register_req_st = ICF_NULL;
  
    /* Assign payload of register_req API to local API structure */
    p_icf_register_req_st = (icf_register_req_st *)p_payload;       
 
    /* Check line id */

    ret_val = icf_vldt_icf_line_id_t(p_icf_register_req_st->line_id,p_vldt_err,0); /* 1 means that this is a configuration api */

    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }/* end if*/
          

    /* if line id is correct */ 

    if( ICF_SUCCESS == ret_val)
    {
        /* bitmask  check */
        if(p_icf_register_req_st->bit_mask & ICF_REG_REQ_TO_PRESENT)
        {
            /*Check for to(receipeint address) */
            ret_val = icf_vldt_icf_address_st(&p_icf_register_req_st->to,
                                              p_vldt_err);
            if(ICF_FAILURE==ret_val)
            {
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"to:");
            }
        }/* End if*/
    }/* End if*/
     
    /* if address structure of to is correct */
    if( ICF_SUCCESS == ret_val)
    {
         /* bit mask check */
         if(p_icf_register_req_st->bit_mask & ICF_REG_REQ_EXPIRY_PRESENT)  
         {
            /* Check for expiry duration */
            ret_val = icf_vldt_icf_duration_t(p_icf_register_req_st->expiry,
                                                                   p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"expiry:");
            }/* End if*/
         }/* End if*/
    }/* End if*/
    /* if duration is correct */  
    if(ICF_SUCCESS == ret_val)
    {
       
       /* bit mask check */
       if(p_icf_register_req_st->bit_mask & ICF_HDR_LIST_PRESENT)  
       {
            /*Check for header list */
           ret_val = icf_vldt_icf_header_list_st(
                       &p_icf_register_req_st->header_list,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
       }/* End if*/

    }/* End if*/
    /* if header list is correct */
    if(ICF_SUCCESS == ret_val)
    {
        
        /* bit mask check */
        if(p_icf_register_req_st->bit_mask & ICF_TAG_LIST_PRESENT) 
        {
            /*Check for mtag list */
            ret_val = icf_vldt_icf_tag_list_st(
                      &p_icf_register_req_st->tag_list,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/

        }/* End if*/        
    }/* End if*/
 
    /* if tag list is correct */ 
    if(ICF_SUCCESS == ret_val)
    {  
       /* bit mask check */
       if(p_icf_register_req_st->bit_mask & ICF_BODY_LIST_PRESENT) 
       { 
           /*Check for body list */
           ret_val = icf_vldt_icf_msg_body_list_st(
                     &p_icf_register_req_st->body_list,p_vldt_err);
           if(ICF_FAILURE == ret_val)
           {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
           }/* End if*/
       }/* End if*/
    }/* End if*/

    /* if contact_list_required field is correct*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_register_req_st->bit_mask &
                               ICF_REG_REQ_CONTACT_LIST_REQD) 
        { 
            /*Check for contact_list_required */
            ret_val = icf_vldt_boolean(
                p_icf_register_req_st->contact_list_required,
                                        p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                    (icf_uint8_t *)"contact_list_required:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    /* if security-client field is correct*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_register_req_st->bit_mask &
                               ICF_REG_REQ_SECURITY_CLIENT_PRESENT) 
        { 
            /*Check for security-client */
            ret_val = icf_vldt_icf_large_string_st(
                &(p_icf_register_req_st->security_client),
                                              p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                    (icf_uint8_t *)"security_client:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    /* if reg_event_pckg_subs_required field is correct*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_register_req_st->bit_mask &
                        ICF_REG_REQ_REG_EVENT_PCKG_SUBS_FLAG_PRESENT) 
        { 
            /*Check for body list */
            ret_val = icf_vldt_boolean(
                p_icf_register_req_st->reg_event_pckg_subs_required,
                                        p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                    (icf_uint8_t *)"reg_event_pckg_subs_required:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    /*Validation for registration_refresh_required*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_register_req_st->bit_mask &
                               ICF_REG_REQ_REFRESH_REG_FLAG_PRESENT) 
        { 
            /* Check for registration_refresh_required */
            ret_val = icf_vldt_boolean(
                p_icf_register_req_st->registration_refresh_required,
                                        p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                    (icf_uint8_t *)"registration_refresh_required:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

     /* if refresh_ind_reqd field is correct*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_register_req_st->bit_mask &
                        ICF_REG_REQ_REFRESH_INDICATIONS_REQD) 
        { 
            /*Check for boolean flag value */
            ret_val = icf_vldt_boolean(
                p_icf_register_req_st->refresh_ind_reqd,
                                        p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                    (icf_uint8_t *)"refresh_ind_reqd:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {  
        /* bit mask check */
        if(p_icf_register_req_st->bit_mask & 
                ICF_REG_REQ_DEREG_SPECIFIC_CONTACT_PRESENT) 
        { 
            /*Check boolean value for dereg_specific_contact */
            ret_val = icf_vldt_boolean(p_icf_register_req_st->dereg_specific_contact, 
                    p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dereg_specific_contact:");
            }/* End if*/
        }/* End if*/
    }/* End if*/


    return(ret_val);
}
    

/******************************************************************************
*   
* FUNCTION:        icf_vldt_icf_create_call_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_create_call_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_create_call_req_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Assigning and creating a temp node of type icf_list_st*/
    icf_list_st *p_temp_node = ICF_NULL; 
    /* Initialsie structure locally */
    icf_create_call_req_st *p_icf_create_call_req_st = ICF_NULL; 
    /* Assign api payload to local structure */
    p_icf_create_call_req_st = (icf_create_call_req_st *)p_payload; 

    /* check for line id */
    ret_val = icf_vldt_icf_line_id_t(p_icf_create_call_req_st->line_id,
                                                             p_vldt_err,0); /*
    0 means that this is not a configuration api */ 
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }/* End if*/
  

    /* if line id is accurate  */
    if(ICF_SUCCESS == ret_val) 
    {
        
        /* bitmask checking */
        if(p_icf_create_call_req_st->bit_mask & ICF_CALLED_PARTY_NAME_PRESENT) 
        {
           /* Check for called party name */
           ret_val = icf_vldt_icf_string_st(
                     &p_icf_create_call_req_st->called_party_name,p_vldt_err);
           if(ICF_FAILURE == ret_val)
           {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"called_party_name:");
           }/* End if*/
        }/* End if*/
    }/* End if*/
    /* if called party name is correct */
    if(ICF_SUCCESS == ret_val) 
    {
        /* Check for the called party address */
        ret_val = icf_vldt_icf_address_st(
                  &p_icf_create_call_req_st->called_party_addr,p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"called_party_addr:");
        }/* End if*/
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        /* bitmask checking */
        if(p_icf_create_call_req_st->bit_mask & ICF_TOGGLE_PRES_IND_PRESENT) 
        {
            /* cHeck for toggle indicator */
            ret_val=icf_vldt_boolean(
            p_icf_create_call_req_st->toggle_presentation_indicator,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"toggle_presentation_indicator:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        
        /* bitmask checking */
        if(p_icf_create_call_req_st->bit_mask & ICF_QOS_MARK_PRESENT)
        {
            /* check for qos mark */
            /*ret_val= icf_vldt_boolean(p_icf_create_call_req_st->qos_mark, 
                                     p_vldt_err);  */
           
        }/* End if*/
   }/* End if*/
        
    if(ICF_SUCCESS == ret_val)
    {
       
        /*bit mask checking */
        if(p_icf_create_call_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT) 
        {
            /* Check for preferred identity */
            ret_val = icf_vldt_icf_address_st(
                      &p_icf_create_call_req_st->pref_identity,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"pref_identity:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        
        /*bit mask checking */
        if (p_icf_create_call_req_st->bit_mask & ICF_CALLING_PARTY_ADDR) 
        {
            /* check for calling party address */
            ret_val = icf_vldt_icf_address_st(
                   &p_icf_create_call_req_st->calling_party_address,p_vldt_err);
            if(ICF_FAILURE ==ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"calling_party_address:");
            }/* End if*/
        }/* End if*/
     }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        /*bit mask checking */
        if(p_icf_create_call_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
        {
             /* Check for the accuracy of header list */
             ret_val = icf_vldt_icf_header_list_st(
                       &p_icf_create_call_req_st->header_list,p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                  /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
             }/* End if*/
        }/* End if*/
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        
        /*bit mask checking */
        if(p_icf_create_call_req_st->bit_mask & ICF_TAG_LIST_PRESENT)  
        {
            /* Check for accuracy of tag list */
            ret_val = icf_vldt_icf_tag_list_st(
                      &p_icf_create_call_req_st->tag_list,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/  
        }/* End if*/
    }/* End if*/


    if(ICF_SUCCESS == ret_val)
    {
        /*bit mask checking */
        if(p_icf_create_call_req_st->bit_mask & ICF_BODY_LIST_PRESENT) 
        {
            /* Check for the acuracy of body list */
            ret_val = icf_vldt_icf_msg_body_list_st(
                      &p_icf_create_call_req_st->body_list,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        /* Check: p_pref_media_profile should of type icf_list_st and
           each node should be of type icf_stream_capab_st
        */  
         p_temp_node = p_icf_create_call_req_st->p_pref_media_profile;

        /* bit mask  checking */
        if(p_icf_create_call_req_st->bit_mask & ICF_PREF_MEDIA_PROFILE_PRESENT)
        {  
            /* Check for the accuracy of stream list */
            while(p_temp_node != ICF_NULL)
            {
                /*check each node is of type p_stream_capab_st */
                ret_val = icf_vldt_icf_stream_capab_st(
                          p_temp_node->p_data,p_vldt_err);
                if(ICF_FAILURE == ret_val)
                {
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                                  (icf_uint8_t *)"p_pref_media_profile:stream_capab_list:");
                    break;
                } /* End of if(ICF_FAILURE == ret_val) */
                p_temp_node=p_temp_node->p_next;
             } /* End of while */
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {

        /* bitmask checking */
        if(p_icf_create_call_req_st->bit_mask & ICF_PRIVACY_HDR_FLAG_PRESENT) 
        {
            /* Check privacy header required field */
            ret_val = icf_vldt_boolean(
                      p_icf_create_call_req_st->privacy_hdr_all_req,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"privacy_hdr_all_req:");
            }/* End if*/
        }/* End if*/
     }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {

     /* bitmask checking */
     if(p_icf_create_call_req_st->bit_mask & ICF_RESOURCE_RESERV_FLAG_PRESENT) 
     {
          /* Check for the accurtcacy of resource reservation required field */
          ret_val = icf_vldt_boolean(
                    p_icf_create_call_req_st->resource_reservation_reqd, 
                    p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
               /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                 (icf_uint8_t *)"resource_reservation_reqd:");
          }/* End if*/
        }/* End if*/
    }



    if(ICF_SUCCESS == ret_val)
    {

        /*bit mask checking */
        if(p_icf_create_call_req_st->bit_mask & ICF_CALLING_PARTY_NAME)
        {
            /* Check for calling party name */
            ret_val = icf_vldt_icf_string_st(
                     &p_icf_create_call_req_st->calling_party_name,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"calling_party_name:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

   if(ICF_SUCCESS == ret_val)
     {
      
        if(p_icf_create_call_req_st->bit_mask & 
                                     ICF_MEDIA_TYPE_REQD_FLAG_PRESENT)
        {
            ret_val = icf_vldt_icf_media_transport_type_et(
                        p_icf_create_call_req_st->media_transport_type,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"media_transport_type:");
            }/* End if*/
        }
     }
#ifdef ICF_QOS_SUPPORT
    /* Validating precondition_required field for boolean value*/
    if(ICF_SUCCESS == ret_val)
   	 {
   	     /* bitmask checking */
        if(p_icf_create_call_req_st->bit_mask & 
                ICF_PRECONDITION_REQD_FLAG_PRESENT)
        {
       		/* check for precondition_required */
            ret_val = icf_vldt_boolean(
           		p_icf_create_call_req_st->precondition_required,
                p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
           	     /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                           		    (icf_uint8_t *)"precondition_required:");
          	}/* End if*/
       	}/* End if*/
   	 }/* End if*/
#endif    
/* Validating srtp_fallback_reqd field for boolean value*/ 
    if(ICF_SUCCESS == ret_val)
    {  
        /* bitmask checking */
        if(p_icf_create_call_req_st->bit_mask &
                ICF_SRTP_FALLBACK_FLAG_PRESENT)
        {
                /* check for precondition_required */
            ret_val = icf_vldt_boolean(
                        p_icf_create_call_req_st->srtp_fallback_reqd,
                p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                     /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                                            (icf_uint8_t *)"srtp_fallback_reqd:");
            }/* End if*/
        }/* End if*/
    }/* End if*/


/* Validating tunnel_mode field for boolean value*/ 
    if(ICF_SUCCESS == ret_val)
    {  
        /* bitmask checking */
        if(p_icf_create_call_req_st->bit_mask &
                ICF_CREATE_CALL_TUNNEL_MODE_OPERATION_PRESENT)
        {
                /* check for precondition_required */
            ret_val = icf_vldt_boolean(
                        p_icf_create_call_req_st->tunnel_mode,
                p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                     /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                                            (icf_uint8_t *)"tunnel_mode:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

#ifdef ICF_SESSION_TIMER
   if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_create_call_req_st->bit_mask &
                       ICF_CREATE_CALL_SESSION_TIMER_ENABLE_PRESENT)
        {
            /* Check for session timer enable flag  */
            ret_val=icf_vldt_boolean(
                  p_icf_create_call_req_st->session_timer_enable,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"session_timer_enable:");
            }/* End if*/          
        } /*End if */
    } /* End if */
#endif
   if(ICF_SUCCESS == ret_val)
   {
       /* bitmask checking */
       if(p_icf_create_call_req_st->bit_mask &
               ICF_CREATE_CALL_PRACK_VALUE_PRESENT)
       {
           /* check for Prack_value */
           /* This field indicates prack_support value 
              0  No prack neither in supported nor in require 
              1  100rel in supported header
              2  100rel in require header
            */
           if((p_icf_create_call_req_st->prack_value != ICF_NO_PRACK) &&
                   (p_icf_create_call_req_st->prack_value != ICF_PRACK_IN_SUPPORTED) &&
                   (p_icf_create_call_req_st->prack_value != ICF_PRACK_IN_REQUIRE))
           {
               ret_val=ICF_FAILURE;
              /* assign ecode */
              p_vldt_err->ecode = ICF_CAUSE_INVALID_PRACK_VALUE;
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,
                       (icf_uint8_t *)"prack_value:");
           }/* End if*/
       }/* End if*/
   }/* End if*/



    /*Validation for redirect_support_reqd*/
    if(ICF_SUCCESS == ret_val)
    {
        /* bitmask checking */
        if(p_icf_create_call_req_st->bit_mask &
                ICF_CREATE_CALL_REDIRECT_SUPPORT_REQD)
        {
            /* check for redirect_support_reqd */
            ret_val = icf_vldt_boolean(
                        p_icf_create_call_req_st->redirect_support_reqd,
                p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                     /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                                            (icf_uint8_t *)" redirect_support_reqd:");
            }/* End if*/
        }/* End if*/
    }/* End if*/


    return(ret_val);
}

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_terminate_call_req_st
*
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_terminate_call_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_terminate_call_req_st(
                             IN icf_uint8_t *p_payload, 
                             INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* Initialsing local API structure  */
     icf_terminate_call_req_st *p_icf_terminate_call_req_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_terminate_call_req_st = (icf_terminate_call_req_st *)p_payload ;
       
     /* Check liine id & bit mask checking */
     if(p_icf_terminate_call_req_st->bit_mask & ICF_LINE_ID_PRESENT) 
     {
          ret_val = icf_vldt_icf_line_id_t(
                        p_icf_terminate_call_req_st->line_id,  
                        p_vldt_err,0);
          if(ICF_FAILURE == ret_val)
          {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
          }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val) /*if line id is  a correct parameetr */
     {
         /* Check for call clear reason in terminate call request */  
         if((p_icf_terminate_call_req_st->call_clear_reason < ICF_CALL_CLEAR_REASON_UNDEFINED) ||(p_icf_terminate_call_req_st->call_clear_reason > 
                                  ICF_CALL_CLEAR_REASON_UNABLE_TO_CREATE_MEDIA))
         {
              ret_val = ICF_FAILURE;  /*assign failure to ret val */ 
              /* assign ecode */
              p_vldt_err->ecode = ICF_CAUSE_VALUE_NOT_DEFINED;
              /* Add error string to p_vldt_err structure */
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"call_clear_reason:");
         }/* End if*/
     }/* End if*/

     if(ICF_SUCCESS == ret_val)
    {
       
       /* bit mask check */
       if(p_icf_terminate_call_req_st->bit_mask & ICF_HDR_LIST_PRESENT)  
       {
            /*Check for header list */
           ret_val = icf_vldt_icf_header_list_st(
                       &p_icf_terminate_call_req_st->header_list, p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
       }/* End if*/

    }/* End if*/
    /* if header list is correct */
    if(ICF_SUCCESS == ret_val)
    {
        
        /* bit mask check */
        if(p_icf_terminate_call_req_st->bit_mask & ICF_TAG_LIST_PRESENT) 
        {
            /*Check for mtag list */
            ret_val = icf_vldt_icf_tag_list_st(
                      &p_icf_terminate_call_req_st->tag_list,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/

        }/* End if*/        
    }/* End if*/
    /* if tag list is correct */ 
    if(ICF_SUCCESS == ret_val)
    {  
       /* bit mask check */
       if(p_icf_terminate_call_req_st->bit_mask & ICF_BODY_LIST_PRESENT) 
       { 
           /*Check for body list */
           ret_val = icf_vldt_icf_msg_body_list_st(
                     &p_icf_terminate_call_req_st->body_list, p_vldt_err);
           if(ICF_FAILURE == ret_val)
           {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
           }/* End if*/
       }/* End if*/
    }/* End if*/

    return (ret_val);
}

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_connect_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_terminate_call_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_connect_req_st(
                     IN icf_uint8_t *p_payload , 
                     INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* initializing some local variables */
    icf_uint32_t count = 0;
    icf_uint32_t i = 0;
    icf_uint32_t del_count=0;
    icf_list_st *p_temp_node = ICF_NULL;
    /* Initialsing local API structure  */
    icf_connect_req_st *p_icf_connect_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_connect_req_st = (icf_connect_req_st *)p_payload;
    /* Check for privacy indication */
    if(p_icf_connect_req_st->bit_mask & ICF_PRIVACY_IND_PRESENT)
    {
        ret_val=icf_vldt_boolean(p_icf_connect_req_st->privacy_ind,p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"privacy_ind:");
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
         
        /*bit mask checking fro header list */
        if(p_icf_connect_req_st->bit_mask & ICF_HDR_LIST_PRESENT) 
        {
             /* if header list present ,then check for it's accuracy */
             ret_val = icf_vldt_icf_header_list_st(
                       &(p_icf_connect_req_st->header_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
             }/* End if*/
         }/* End if*/
    }/* End if*/   
    if(ICF_SUCCESS == ret_val)
    {

        /*bit mask checking  for tag list*/
        if(p_icf_connect_req_st->bit_mask & ICF_TAG_LIST_PRESENT) 
        {
             /* if tag list present ,then check for it's accuracy */
             ret_val = icf_vldt_icf_tag_list_st(
                       &(p_icf_connect_req_st->tag_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
             }/* End if*/
         }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
         /*bit mask checking for body list*/
         if(p_icf_connect_req_st->bit_mask & ICF_BODY_LIST_PRESENT) 
         {
              /* if body list present ,then check for it's accuracy */
              ret_val = icf_vldt_icf_msg_body_list_st(
                        &(p_icf_connect_req_st->body_list),p_vldt_err);
              if(ICF_FAILURE == ret_val)
              {
                   /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
              }/* End if*/
          }/* End if*/ 
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
           /* Check: p_stream_list should of type icf_list_st and
              each node hould be of type icf_stream_capab_st
           */  
           /* Assign p_icf_connect_req_st->p_stream_list to p_temp_node */
           p_temp_node = p_icf_connect_req_st->p_stream_list;

          /*bit mask checking  for stream list*/
          if(p_icf_connect_req_st->bit_mask & ICF_STREAM_LIST_PRESENT) 
          { 
               /* if body list present ,then check for it's accuracy */
               while(p_temp_node != ICF_NULL)
               {
                  /*check each node is of type p_stream_capab_st */
                  ret_val = icf_vldt_icf_stream_capab_st(
                                   p_temp_node->p_data,p_vldt_err);
                  if(ICF_FAILURE == ret_val)
                  {
                      /* Add error string to p_vldt_err structure */
                      icf_vldt_port_strcat(p_vldt_err->err_str,
                                      (icf_uint8_t *)"p_stream_list:");
                      break;
                  } /* End of if(ICF_FAILURE == ret_val) */
                  p_temp_node=p_temp_node->p_next;
               } /* End of while */
          }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
          /* bit maks checking for deleted media streams */
          if(p_icf_connect_req_st->bit_mask & 
                 ICF_USER_ALERTED_DELETED_STREAM_PRESENT) 
          {
               /* Check for number odf deleted media count */
               if(p_icf_connect_req_st->count > ICF_MAX_STREAM_PER_CALL) 
               {
                    ret_val = ICF_FAILURE; /* Assign failure to ret val */
                    /* Assign ecode  if stream count is out of range*/
                    p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_OUT_OF_RANGE; 
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:");
                }
                else if((p_icf_connect_req_st->count > 0) && 
                        (p_icf_connect_req_st->count < ICF_MAX_STREAM_PER_CALL))
                /*if number of deleted media stream are in valid range */
                {
                    /*initialise countto number of deleted maeida count */
                    count = p_icf_connect_req_st->count; 
                    /* Check for the equality of number of media count 
                       and actual media streams present 
                    */
                    for(i=0 ; i < count ;i++) 
                    {
                        ++del_count;    
                    }
                    
                    /* if del_count is not equal to actual 
                       deleetd media count 
                    */
                    if(del_count != count) 
                    {
                        ret_val = ICF_FAILURE ;/* Assign failure to ret val */ 
                        /* assign ecode */
                        p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_MISMATCH;
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:"); 
                    }/* End if*/

                }/* End else-if*/
                /* if both the above validations are passed, 
                   then check for actual deleted media astreams 
                */
                if( ICF_SUCCESS == ret_val) 
                {
                    /* Determine the validity of each 
                       deleted media stream one by one 
                    */
                    /* In case of failure come out of the loop */
                    for(i=0;((i<p_icf_connect_req_st->count) && 
                                       (ICF_SUCCESS == ret_val)) ; i++) 
                    {
                        ret_val = icf_vldt_icf_stream_list_st(
                             &(p_icf_connect_req_st->deleted_media_streams[i]),
                             p_vldt_err);
                        if(ICF_FAILURE == ret_val)
                        {
                            /* Add error string to p_vldt_err structure */
                            icf_vldt_port_strcat(p_vldt_err->err_str,
                                         (icf_uint8_t *)"deleted_media_streams:");
                            break;
                        }/* End if*/
                    } /* End for */

                }/* End if*/    
         
         }/* End if*/

     }/* End if*/
     return ret_val;
}    



/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_subscribe_req_st
*  
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_subscribe_req_st api.
*      
*****************************************************************************/
icf_return_t icf_vldt_icf_subscribe_req_st(
                          IN icf_uint8_t *p_payload, 
                          INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* initialise local structure for subscribe req api */
    icf_subscribe_req_st *p_icf_subscribe_req_st = ICF_NULL; 
    /* Assign api payload to local structure */
    p_icf_subscribe_req_st = (icf_subscribe_req_st *)p_payload ; 
    /* Check for the line id */
    ret_val = icf_vldt_icf_line_id_t(
              (p_icf_subscribe_req_st->line_id),p_vldt_err,0);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }/* End if*/
    if(ICF_SUCCESS == ret_val) /* if line id is a valid parameter */
    {
        /* Check for destination info present */      
        if(p_icf_subscribe_req_st->bit_mask & ICF_SUB_REQ_DEST_INFO_PRESENT)            
        {
            /* Check for the validity of destination info */ 
            ret_val = icf_vldt_icf_address_st(
                      &(p_icf_subscribe_req_st->destination_info),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {   
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"destination_info:");
            }/* End if*/   
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for event */ 
        if(p_icf_subscribe_req_st->bit_mask & ICF_SUB_REQ_EVENT_PRESENT)
        {
            /* Check if duration is greater than zero */ 
            /* if yes,then validate the event package name */ 
            if(p_icf_subscribe_req_st->duration > 0)
            {
                ret_val = icf_vldt_icf_string_st(
                          &(p_icf_subscribe_req_st->event_pkg_name),p_vldt_err);
                if(ICF_FAILURE == ret_val)
                {
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"event_pkg_name:");
                }/* End if*/ 
            }
            /* Otherwise if duration is zero then no need to check 
               for validation of event package name 
            */ 
            else if(p_icf_subscribe_req_st->duration == 0)
            {
                 ret_val = ICF_SUCCESS;
            }/* End else-if */
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for preferred identity bitmask */
        if(p_icf_subscribe_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
        {
             /* Check the validity of preffered identity */         
             ret_val = icf_vldt_icf_address_st(
                       &(p_icf_subscribe_req_st->pref_identity),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"pref_identity:");
             }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for bitmask of header list */
        if(p_icf_subscribe_req_st->bit_mask &  ICF_HDR_LIST_PRESENT)
        {
            /* Check the validity of the header list */
            ret_val = icf_vldt_icf_header_list_st(
                      &(p_icf_subscribe_req_st->header_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Cehck for the bitmask of tag list */
        if(p_icf_subscribe_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
        {
            /* Check the validity of the tag list */
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_subscribe_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {                 
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmask of body list */   
        if(p_icf_subscribe_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
        {
            /* Check the validity of body list */
            ret_val = icf_vldt_icf_msg_body_list_st(
                      &(p_icf_subscribe_req_st->body_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    return ret_val;
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_publish_req_st
*  
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_publish_req_st api.
*      
*****************************************************************************/
icf_return_t icf_vldt_icf_publish_req_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsing local API structure */
    icf_publish_req_st *p_icf_publish_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_publish_req_st = (icf_publish_req_st *)p_payload ;
    /* Check for the validity of line id */
    ret_val = icf_vldt_icf_line_id_t(
              (p_icf_publish_req_st->line_id),p_vldt_err,0); 
    if(ICF_FAILURE == ret_val)
    {
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }

    if(ICF_SUCCESS == ret_val) /* if line id is a valid parametr */
    {
        /* Check for the validity of destination info */      
        ret_val = icf_vldt_icf_address_st(
                  &(p_icf_publish_req_st->destination_info),p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"destination_info:");
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the duration parameter .if it ise greater than zero,
           then check for event pacakge name 
        */
        if(p_icf_publish_req_st->duration > 0)
        {
            /* Check for event package name's validity */
            ret_val = icf_vldt_icf_string_st(
                      &(p_icf_publish_req_st->event_pkg_name),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"event_pkg_name:");
            }/* End if*/
        }
        else if(p_icf_publish_req_st->duration == 0) 
        /* if durationn is zero  then don't check for event package name */
        {
            ret_val = ICF_SUCCESS;
        }/* End Else-if */ 
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        /* Check  for the bitmask user_address */ 
        if(p_icf_publish_req_st->bit_mask & ICF_USER_ADDRESS_PRESENT)
        {
            /* Check for the validity of user_address */
            ret_val = icf_vldt_icf_address_st(
                      &(p_icf_publish_req_st->user_address),p_vldt_err);
             if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t*)"user_address:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    
    if(ICF_SUCCESS == ret_val)
    {
        /* Check  for the bitmask preferred identity */ 
        if(p_icf_publish_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
        {
            /* Check for the validity of preferred identity */
            ret_val = icf_vldt_icf_address_st(
                      &(p_icf_publish_req_st->pref_identity),p_vldt_err);
             if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"pref_identity:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmask for header list */
        if(p_icf_publish_req_st->bit_mask &  ICF_HDR_LIST_PRESENT)
        {
            /* Check for the validity of header list */
            ret_val = icf_vldt_icf_header_list_st(
                      &(p_icf_publish_req_st->header_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* checkf for the bitmask of tag list */ 
        if(p_icf_publish_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
        {
            /* Check for the validity of tag list */   
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_publish_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bit mask of body list */    
        if(p_icf_publish_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
        {
             /* Check for the validity of body list */   
             ret_val = icf_vldt_icf_msg_body_list_st(
                       &(p_icf_publish_req_st->body_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
             }/* End if*/
         }/* End if*/
    }/* End if*/
    return(ret_val);     
}




/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_message_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_message_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_message_req_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsing local API structure  */
    icf_message_req_st *p_icf_message_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_message_req_st = (icf_message_req_st *)p_payload ;
    /* Check fior the validity of line id */
    ret_val = icf_vldt_icf_line_id_t(
              (p_icf_message_req_st->line_id),p_vldt_err,0);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }/* End if*/
    if(ICF_SUCCESS == ret_val) /* if line id is valid */
    {
        /* Check for the validity of destination info */
        ret_val = icf_vldt_icf_address_st(
                  &(p_icf_message_req_st->destination_info),p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"destination_info:");
        }/* End if*/
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        /* Check  for the bitmask user_address */ 
        if(p_icf_message_req_st->bit_mask & ICF_USER_ADDRESS_PRESENT)
        {
            /* Check for the validity of user_address */
            ret_val = icf_vldt_icf_address_st(
                      &(p_icf_message_req_st->user_address),p_vldt_err);
             if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t*)"user_address:");
            }/* End if*/
        }/* End if*/
     }/* End if*/
    
    if(ICF_SUCCESS == ret_val)
    {  
        /* Check for the bitmask of preferred identity */
        if(p_icf_message_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
        {
            /* check foolr the validity of preferred identity */
            ret_val = icf_vldt_icf_address_st(
                      &(p_icf_message_req_st->pref_identity),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"pref_identity:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bit mask of header list */         
        if(p_icf_message_req_st->bit_mask &  ICF_HDR_LIST_PRESENT)
        {
            /* Check fdor the validity of header list */
            ret_val = icf_vldt_icf_header_list_st(
                      &(p_icf_message_req_st->header_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bit mask of tag list */  
        if(p_icf_message_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
        {
            /* Check for the validity of tag list */ 
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_message_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check  for the bitmask of body list */
         if(p_icf_message_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
         {
             /* check for the validity of body list */
             ret_val = icf_vldt_icf_msg_body_list_st(
                       &(p_icf_message_req_st->body_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
             }/* End if*/
          }/* End if*/
     }/* End if*/
     return(ret_val);
}



/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_conference_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_conference_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_conference_req_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsing local API structure  */
    icf_conference_req_st *p_icf_conference_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_conference_req_st = (icf_conference_req_st*)p_payload;

        /* Check for the bitmask of header list */
        if(p_icf_conference_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
        {
            /* Check for the validity of header list */ 
            ret_val = icf_vldt_icf_header_list_st(
                      &(p_icf_conference_req_st->header_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
        }/* End if*/

    if(ICF_SUCCESS == ret_val) 
    {
        /* Check for the bitmask of tag list */ 
        if(p_icf_conference_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
        {
            /* Check for the validity of tag list */                    
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_conference_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    return(ret_val);
}



/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_refer_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_refer_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_refer_req_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsing local API structure  */
    icf_refer_req_st *p_icf_refer_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_refer_req_st = (icf_refer_req_st *)p_payload ;
    /* Check for the validity of line id */ 
    ret_val = icf_vldt_icf_line_id_t((p_icf_refer_req_st->line_id),p_vldt_err,0);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmask of detination info */          
        if(p_icf_refer_req_st->bit_mask & ICF_REF_REQ_DEST_INFO_PRESENT)  
        {
            /* Check for the destination info's validity */
            ret_val = icf_vldt_icf_address_st(
                      &(p_icf_refer_req_st->destination_info),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"destination_info:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the validity of refer_to parameter validity */
        ret_val = icf_vldt_icf_address_st(
                  &(p_icf_refer_req_st->refer_to),p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {    
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"refer_to:");
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmask of preferred identity */ 
        if(p_icf_refer_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
        {
             /* Check for the validity of preffered idenmtity */
             ret_val = icf_vldt_icf_address_st(
                       &(p_icf_refer_req_st->pref_identity),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"pref_identity:");
             }/* End if*/
         }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmask of header list */   
        if(p_icf_refer_req_st->bit_mask &  ICF_HDR_LIST_PRESENT)
        {
            /* if header lis tpresent,then check for it's validity */ 
            ret_val = icf_vldt_icf_header_list_st(
                      &(p_icf_refer_req_st->header_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    { 
        /* Check for the bitmask of tag list*/                   
        if(p_icf_refer_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
        {
             /* Check for the validity of tag list */    
             ret_val = icf_vldt_icf_tag_list_st(
                       &(p_icf_refer_req_st->tag_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
             }/* End if*/
         }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmask of body list */
        if(p_icf_refer_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
        {
            /* Check for the validity of body list */ 
            ret_val = icf_vldt_icf_msg_body_list_st(
                      &(p_icf_refer_req_st->body_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bit mask of subscritpion required */
        /* bitmask checking */
        if(p_icf_refer_req_st->bit_mask & ICF_REF_REQ_SUBS_REQD_PRESENT) 
        {
            /* Check for subscription required  */
            ret_val=icf_vldt_boolean(p_icf_refer_req_st->subscription_required,
                    p_vldt_err);
            if(ICF_FAILURE == ret_val)             
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"subscription_required:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    return(ret_val);
}

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_call_hold_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_call_hold_req_st api.
*****************************************************************************/
icf_return_t icf_vldt_icf_call_hold_req_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
    icf_call_hold_req_st *p_icf_call_hold_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_call_hold_req_st = (icf_call_hold_req_st *)p_payload ;
    /* Check for the bitmask for header list */
    if(p_icf_call_hold_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        /* if header listpresent ,then check the validity of the header list */
        ret_val = icf_vldt_icf_header_list_st(
                  &(p_icf_call_hold_req_st->header_list),p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
             /* Check for the bit mask of tag list */ 
             if(p_icf_call_hold_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
             {

                 /* if tag list present,then check for the 
                    validity of the tag list  
                 */
                 ret_val = icf_vldt_icf_tag_list_st(
                           &(p_icf_call_hold_req_st->tag_list),p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                      /* Add error string to p_vldt_err structure */
                      icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
                 }/* End if*/
             }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
          /* Chekc for the bitmask of body list */
          if(p_icf_call_hold_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
          {
               /* Check for the validity of body list*/        
               ret_val = icf_vldt_icf_msg_body_list_st(
                         &(p_icf_call_hold_req_st->body_list),p_vldt_err);
               if(ICF_FAILURE == ret_val)
               {
                   /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
               }/* End if*/
          }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check for the bitmask of stream lis t*/
         if(p_icf_call_hold_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
         {
             /* Check fior the validity of stream list */ 
             ret_val = icf_vldt_icf_stream_id_list_st(
                       &(p_icf_call_hold_req_st->stream_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"stream_list:");
             }/* End if*/
         }/* End if*/
      }/* End if*/
      return(ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_call_resume_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_call_resume_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_call_resume_req_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
    icf_call_resume_req_st *p_icf_call_resume_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_call_resume_req_st = (icf_call_resume_req_st *)p_payload ;
    /* Check for the bitmask of header list */
    if(p_icf_call_resume_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        /* Check for the validity of header list */ 
        ret_val = icf_vldt_icf_header_list_st(
                  &(p_icf_call_resume_req_st->header_list),p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmask for tag list */
        if(p_icf_call_resume_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
        {
            /* check for the validity of tag list*/
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_call_resume_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmask for body lis tpresent */ 
        if(p_icf_call_resume_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
        {
            /* Check for the validity of body list */
            ret_val = icf_vldt_icf_msg_body_list_st(
                      &(p_icf_call_resume_req_st->body_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmask of stream list*/
        if(p_icf_call_resume_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
        {
            /* Check fior the validity of stream list */
            ret_val = icf_vldt_icf_stream_id_list_st(
                      &(p_icf_call_resume_req_st->stream_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"stream_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    return(ret_val);
}

/******************************************************************************
*    
* FUNCTION:        icf_vldt_icf_call_transfer_unattended_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_call_transfer_unattended_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_ctu_req_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val  = ICF_SUCCESS;
    /* Initialsie structure locally */
    icf_call_transfer_unattended_req_st *p_icf_ctu_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_ctu_req_st = (icf_call_transfer_unattended_req_st *)p_payload;    
    /* Chck for the bitmask for header list*/
    if(ICF_HDR_LIST_PRESENT == (p_icf_ctu_req_st->bit_mask & ICF_HDR_LIST_PRESENT))
    {
        /* Check for the validity of header list */ 
        ret_val = icf_vldt_icf_header_list_st(
                  &(p_icf_ctu_req_st->header_list),p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
        }/* End if*/
     }/* End if*/
     /* Chck for the bitmask for tag   list*/
     if(ICF_SUCCESS == ret_val)
     {       
         /* Check for the validity of tag  list */
         if(p_icf_ctu_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
         {
             ret_val = icf_vldt_icf_tag_list_st(
                       &(p_icf_ctu_req_st->tag_list),p_vldt_err);
              if(ICF_FAILURE == ret_val)
              {
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
              }/* End if*/
         }/* End if*/
     }/* End if*/
     /* Check for the bitmask for body list present */
     if(ICF_SUCCESS == ret_val)
     {
          /* Check for the validity of body list */    
          if(p_icf_ctu_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
          {
              ret_val = icf_vldt_icf_msg_body_list_st(
                        &(p_icf_ctu_req_st->body_list),p_vldt_err);
              if(ICF_FAILURE == ret_val)
              {
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
              }/* End if*/
          }/* End if*/
     }/* End if*/        
     if(ICF_SUCCESS == ret_val) 
     {      
         /* Chec for the validity of transfeered party address */
         ret_val = icf_vldt_icf_address_st(
                   &(p_icf_ctu_req_st->transferred_party_addr),p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"transferred_party_addr:");
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check for the bitmask for toggle presentaion indicator */  
         /* bitmask checking */
         if(p_icf_ctu_req_st->bit_mask & 
            ICF_CALL_TRANSFER_UNATTENDED_REQ_TOGGLE_PRESENTATION_INDICATOR) 
         {
             /* Check for toggle indicator */
             ret_val = icf_vldt_boolean(
                       p_icf_ctu_req_st->toggle_presentation_indicator,
                       p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"toggle_presentation_indicator:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
        if(p_icf_ctu_req_st->bit_mask &
	    ICF_ADDRESS_ADDNL_URI_PARAMTR_PRES)  
	{
           /* check for uri parameter */
           ret_val = icf_vldt_icf_large_string_st((icf_large_string_st *) p_icf_ctu_req_st->p_uri_parameter,p_vldt_err);
            if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_uri:");
             }					 
         }
     }
     return ret_val;
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_call_transfer_attended_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_call_transfer_attended_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_cta_req_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val  = ICF_SUCCESS;
     /* Initialsie structure locally */
     icf_call_transfer_attended_req_st *p_icf_cta_req_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_cta_req_st = (icf_call_transfer_attended_req_st *)p_payload;

    /* Check for the bitmas kfor header list*/     
    if(p_icf_cta_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        /* Check for the validity of header list */
        ret_val = icf_vldt_icf_header_list_st(
                  &(p_icf_cta_req_st->header_list),p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmas for tag list*/      
        if(p_icf_cta_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
        {
            /* Check for the validity of tag list */
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_cta_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the bitmas for body list*/ 
        if(p_icf_cta_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
        {
            /* Check for the validity of body list */
            ret_val = icf_vldt_icf_msg_body_list_st(
                      &(p_icf_cta_req_st->body_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    return(ret_val);
}



/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_info_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the icf_info_req_st api*
*****************************************************************************/
icf_return_t icf_vldt_icf_info_req_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS; 
    /* Initialsie structure locally */
    icf_info_req_st    *p_icf_info_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_info_req_st    = (icf_info_req_st *)p_payload;
    if(p_icf_info_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        /* Check for the validity of header list */
        ret_val = icf_vldt_icf_header_list_st(
                  &(p_icf_info_req_st->header_list),p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
        }/* End if*/
   }/* End if*/
   if(ICF_SUCCESS == ret_val)
   {
       /* Check for the bitmas for tag list*/
       if(p_icf_info_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
       {
           /* Check for the validity of tag list */
           ret_val = icf_vldt_icf_tag_list_st(
                     &(p_icf_info_req_st->tag_list),p_vldt_err);
           if(ICF_FAILURE == ret_val)
           {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
           }/* End if*/
       }/* End if*/
   }/* End if*/
   if(ICF_SUCCESS == ret_val)
   {
       /* Check for the bitmas for body list*/     
       if(p_icf_info_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
       {
           /* Check for the validity of body list */
           ret_val = icf_vldt_icf_msg_body_list_st(
                     &(p_icf_info_req_st->body_list),p_vldt_err);
           if(ICF_FAILURE == ret_val)
           { 
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
           }/* End if*/
       }/* End if*/
   }/* End if*/
   return(ret_val);
}

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_call_modify_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_call_modify_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_call_modify_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* initializing some local variables */
     icf_uint32_t count = 0;
     icf_uint32_t i = 0;
     icf_uint32_t del_count=0;
     /* Assigning and creating a temp node of type icf_list_st*/
     icf_list_st *p_temp_node = ICF_NULL;
     /* Initialsie structure locally */
     icf_call_modify_req_st  *p_icf_reinvite_req_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_reinvite_req_st =  (icf_call_modify_req_st *)p_payload;
     if(p_icf_reinvite_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
     {
         /* Check for the validity of header list */
         ret_val = icf_vldt_icf_header_list_st(
                   &( p_icf_reinvite_req_st->header_list),p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
         }/* End if*/
     }/* End if*/
      if(ICF_SUCCESS == ret_val)
     {
         /* Check for the bitmas for tag list*/
         if(p_icf_reinvite_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
         {
             /* Check for the validity of tag list */
             ret_val = icf_vldt_icf_tag_list_st(
                       &(p_icf_reinvite_req_st->tag_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check for the bitmas for body list*/
         if(p_icf_reinvite_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
         {
             /* Check for the validity of body list */
             ret_val = icf_vldt_icf_msg_body_list_st(
                       &(p_icf_reinvite_req_st->body_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check: p_media_modify_list should of type icf_list_st and
            each node hould be of type icf_stream_capab_st
         */
         /* Assign p_icf_reinvite_cfm_st->p_media_modify_list to p_temp_node */
          p_temp_node = p_icf_reinvite_req_st->p_media_modify_list;

         /* Check for the bitmas for stream  list*/
         if(p_icf_reinvite_req_st->bit_mask & ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT)
         {
             /* Check for the validity of stream  list */
              while(p_temp_node != ICF_NULL)
              {
                 /*check each node is of type p_stream_capab_st */
                 ret_val = icf_vldt_icf_call_modify_media_data_st(
                           p_temp_node->p_data,p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                     /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_media_modify_list:");
                     break;
                 } /* End of if(ICF_FAILURE == ret_val) */
                 p_temp_node=p_temp_node->p_next;
              } /* End of while */
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
          /* bit maks checking for deleted media streams */
          if(p_icf_reinvite_req_st->bit_mask &
                 ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT)
          {
               /* Check for number odf deleted media count */
               if(p_icf_reinvite_req_st->count > ICF_MAX_STREAM_PER_CALL)
               {
                    ret_val = ICF_FAILURE; /* Assign failure to ret val */
                    /* Assign ecode  if stream count is out of range*/
                    p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_OUT_OF_RANGE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:");
                }/* End if*/
                else if((p_icf_reinvite_req_st->count > 0) &&
                       (p_icf_reinvite_req_st->count < ICF_MAX_STREAM_PER_CALL))
                /*if number of deleted media stream are in valid range */
                {
                    /*initialise countto number of deleted maeida count */
                    count = p_icf_reinvite_req_st->count;
                    /* Check for the equality of number of media count
                       and actual media streams present
                    */
                    for(i=0 ; i < count ;i++)
                    {
                         del_count ++;
                        
                    }/* End for */

                    /* if del_count is not equal to actual
                       deleetd media count
                    */
                    if(del_count != count)
                    {
                        ret_val = ICF_FAILURE ;/* Assign failure to ret val */
                        /* assign ecode */
                        p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_MISMATCH;
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:");
                    }/* End if*/

                }/* End if*/
                /* if both the above validations are passed,
                   then check for actual deleted media astreams
                */
                if( ICF_SUCCESS == ret_val)
                {
                    /* Determine the validity of each
                       deleted media stream one by one
                    */
                    /* In case of failure come out of the loop */
                    for(i=0;((i<p_icf_reinvite_req_st->count) &&
                                       (ICF_SUCCESS == ret_val)) ; i++)
                    {
                        ret_val = icf_vldt_icf_stream_list_st(
                             &(p_icf_reinvite_req_st->deleted_media_streams[i]),
                             p_vldt_err);
                        if(ICF_FAILURE == ret_val)
                        {
                            /* Add error string to p_vldt_err structure */
                            icf_vldt_port_strcat(p_vldt_err->err_str,
                                         (icf_uint8_t *)"delete_media_streams:");
                            break;
                        }/* End if*/
                    }/* End for */

                }/* End if*/

         }/* End if*/
         if(ICF_SUCCESS == ret_val)
         {
             /*  validation of To tag */
             if(ICF_CALL_MOD_REQ_TO_TAG_PRESENT == 
                (p_icf_reinvite_req_st->bit_mask &
                     ICF_CALL_MOD_REQ_TO_TAG_PRESENT))
            {
                ret_val = icf_vldt_icf_string_st(
                          &(p_icf_reinvite_req_st->to_tag),
                         p_vldt_err);
                if(ICF_FAILURE == ret_val)
                {
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"to_tag:");
                }/* End if*/
                
            }
        }
    }/* End if*/    

    if(ICF_SUCCESS == ret_val)
    {
        if(ICF_CALL_MOD_REQ_TUNNELLED_SDP_PRESENT ==
                          (p_icf_reinvite_req_st->bit_mask &
                            ICF_CALL_MOD_REQ_TUNNELLED_SDP_PRESENT))
        {
            ret_val = icf_vldt_boolean(
                         p_icf_reinvite_req_st->tunnelled_sdp_present,
                            p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tunnelled_sdp_present:");
            }/*End if*/
        }
    }

     return(ret_val);
}

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_call_modify_cfm_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_call_modify_cfm_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_call_modify_cfm_st(
                          IN icf_uint8_t *p_payload, 
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     icf_uint8_t count = 0;
     icf_uint8_t  i = 0;
     icf_uint8_t del_count = 0;
     icf_list_st *p_temp_node = ICF_NULL;
     /* Initialsie structure locally */ 
     icf_call_modify_cfm_st  *p_icf_reinvite_cfm_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_reinvite_cfm_st =  (icf_call_modify_cfm_st *)p_payload;
     if(p_icf_reinvite_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT)
     {
         /* Check for the validity of header list */   
         ret_val = icf_vldt_icf_header_list_st(
                   &( p_icf_reinvite_cfm_st->header_list),p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check for the bitmas for tag list*/
         if(p_icf_reinvite_cfm_st->bit_mask & ICF_TAG_LIST_PRESENT)
         {
             /* Check for the validity of tag list */
             ret_val = icf_vldt_icf_tag_list_st(
                       &(p_icf_reinvite_cfm_st->tag_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check for the bitmas for body list*/   
         if(p_icf_reinvite_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
         {
             /* Check for the validity of body list */
             ret_val = icf_vldt_icf_msg_body_list_st(
                       &(p_icf_reinvite_cfm_st->body_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check: p_stream_list should of type icf_list_st and
            each node hould be of type icf_stream_capab_st
         */
         /* Assign p_icf_reinvite_cfm_st->p_stream_list to p_temp_node */
          p_temp_node = p_icf_reinvite_cfm_st->p_stream_list;
  
         /* Check for the bitmas for stream  list*/
         if(p_icf_reinvite_cfm_st->bit_mask & ICF_STREAM_LIST_PRESENT)
         {
             /* Check for the validity of stream  list */
              while(p_temp_node != ICF_NULL)
              {
                 /*check each node is of type p_stream_capab_st */
                 ret_val = icf_vldt_icf_stream_capab_st(
                           p_temp_node->p_data,p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                     /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_stream_list:");
                     break;
                 } /* End of if(ICF_FAILURE == ret_val) */
                 p_temp_node=p_temp_node->p_next;
              } /* End of while */
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check for value of the result  */
         ret_val = icf_vldt_boolean(p_icf_reinvite_cfm_st->result,p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
         }/* End if*/
     }/* End if*/
     /* SPR Fix 17640 . If result is failure the nw_response_code cannot 
        be below 4xx or gretaer than 6xx*/ 
     if((ICF_SUCCESS == ret_val) && (ICF_FAILURE == p_icf_reinvite_cfm_st->
             result))
     {
         if(p_icf_reinvite_cfm_st->bit_mask &
                ICF_CALL_MODIFY_RESPONSE_CODE_PRESENT)
         {
             if(p_icf_reinvite_cfm_st->nw_response_code < 400 ||
                (p_icf_reinvite_cfm_st->nw_response_code >  699))
             {
                 ret_val = ICF_FAILURE; /* Assign failure to ret val */
                 /* Assign ecode  if result is failure and network response code is less than 400 or gretaer than 699*/
                 p_vldt_err->ecode = ICF_CAUSE_INVALID_VALUE_FOR_RESPONSE_CODE;
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"nw_response_code:");
              }
          }
     }

     if((ICF_SUCCESS == ret_val) && (p_icf_reinvite_cfm_st->bit_mask & ICF_CALL_MODIFY_RESPONSE_SIGNALING_MODE_PRESENT)) 
     {
         /* check the value of signaling mode */
         ret_val =  icf_vldt_icf_media_mode_t(p_icf_reinvite_cfm_st->signaling_mode, p_vldt_err);
          if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"media_mode:");
         }
     } 
     if(ICF_SUCCESS == ret_val)
     {
          /* bit maks checking for deleted media streams */
          if(p_icf_reinvite_cfm_st->bit_mask & 
                 ICF_CALL_MODIFY_CFM_REJECTED_STREAM_PRESENT) 
          {
               /* Check for number odf deleted media count */
               if(p_icf_reinvite_cfm_st->count > ICF_MAX_STREAM_PER_CALL) 
               {
                    ret_val = ICF_FAILURE; /* Assign failure to ret val */
                    /* Assign ecode  if stream count is out of range*/
                    p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_OUT_OF_RANGE; 
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:");
                }
                else if((p_icf_reinvite_cfm_st->count > 0) && 
                        (p_icf_reinvite_cfm_st->count < ICF_MAX_STREAM_PER_CALL))
                /*if number of deleted media stream are in valid range */
                {
                    /*initialise countto number of deleted maeida count */
                    count = p_icf_reinvite_cfm_st->count; 
                    /* Check for the equality of number of media count 
                       and actual media streams present 
                    */
                    for(i=0 ; i < count ;i++) 
                    {
                        ++del_count;    
                    }
                    
                    /* if del_count is not equal to actual 
                       deleetd media count 
                    */
                    if(del_count != count) 
                    {
                        ret_val = ICF_FAILURE ;/* Assign failure to ret val */ 
                        /* assign ecode */
                        p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_MISMATCH;
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:"); 
                    }/* End if*/

                }/* End else-if*/
                /* if both the above validations are passed, 
                   then check for actual deleted media astreams 
                */
                if( ICF_SUCCESS == ret_val) 
                {
                    /* Determine the validity of each 
                       deleted media stream one by one 
                    */
                    /* In case of failure come out of the loop */
                    for(i=0;((i<p_icf_reinvite_cfm_st->count) && 
                                       (ICF_SUCCESS == ret_val)) ; i++) 
                    {
                        ret_val = icf_vldt_icf_stream_list_st(
                             &(p_icf_reinvite_cfm_st->deleted_stream_list[i]),
                             p_vldt_err);
                        if(ICF_FAILURE == ret_val)
                        {
                            /* Add error string to p_vldt_err structure */
                            icf_vldt_port_strcat(p_vldt_err->err_str,
                                         (icf_uint8_t *)"deleted_media_streams:");
                            break;
                        }/* End if*/
                    } /* End for */

                }/* End if*/    
         
         }/* End if*/

     }/* End if*/
    
     return(ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_start_fax_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_start_fax_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_start_fax_req_st(
                        IN icf_uint8_t *p_payload, 
                        INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS; 
    /* Initialsie structure locally */
    icf_start_fax_req_st    *p_icf_start_fax_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_start_fax_req_st    = (icf_start_fax_req_st *)p_payload;
    /* Chek for the fax type.It should be in range 0-2 .*/
    if( p_icf_start_fax_req_st->fax_type >3)
    {
        ret_val = ICF_FAILURE;   /* assign failure to ret val */
        p_vldt_err->ecode = ICF_CAUSE_VALUE_NOT_DEFINED ;  /* assign ecode */
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"fax_type:");
    }/* End if*/
    if(ICF_SUCCESS == ret_val) /* if fax type is  a valid parameter */
    {
        /* Chek for fax codec */      
        if((p_icf_start_fax_req_st->fax_pt_codec != ICF_PREF_CODEC_FAX_PT_PCMA)
          && (p_icf_start_fax_req_st->fax_pt_codec != 
             ICF_PREF_CODEC_FAX_PT_PCMU))
        {
            ret_val = ICF_FAILURE; /* assign failure to ret val */
            p_vldt_err->ecode = ICF_CAUSE_VALUE_NOT_DEFINED; /* assign ecode */
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"fax_pt_codec:");
        }/* End if*/
    }/* End if*/      
    if(ICF_SUCCESS == ret_val) /* Check for fax method type 0 means replace audio with fax while 1 means add fax stream with audio */
    {
    	    if(p_icf_start_fax_req_st->bitmask & ICF_START_FAX_REQ_METHOD_PRESENT)
       	    {
	    	if( (p_icf_start_fax_req_st->fax_method != ICF_FAX_METHOD_REPLACE_STREAM) && (p_icf_start_fax_req_st->fax_method != 
			ICF_FAX_METHOD_ADD_STREAM ) )		
		{
			ret_val = ICF_FAILURE; /* assign failure to ret val */
			p_vldt_err->ecode = ICF_CAUSE_VALUE_NOT_DEFINED; /* assign ecode */
			/* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"fax_method:");
        	}/* End if*/
    	}/* End if */					 
    }	
    return(ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_notify_cfm_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_notify_cfm_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_notify_cfm_st(
                          IN icf_uint8_t *p_payload, 
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t  ret_val = ICF_SUCCESS;
     /* Initialsie structure locally */
     icf_notify_cfm_st    *p_icf_notify_cfm_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_notify_cfm_st    = (icf_notify_cfm_st *)p_payload;
     /* Check for value of the result  */
     ret_val = icf_vldt_boolean(p_icf_notify_cfm_st->result,p_vldt_err);
     if(ICF_FAILURE == ret_val)
     {
          /* Add error string to p_vldt_err structure */
          icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
     }/* End if*/

     if(ICF_SUCCESS == ret_val)
     {    
         /* check for the bitmask for response code */      
	 if((ICF_FALSE == p_icf_notify_cfm_st->result) &&
               (p_icf_notify_cfm_st->bit_mask & ICF_NOTIFY_RESP_CODE_PRESENT))
         { 
             /* Check for response code .It's value should be between 400-699*/ 
             /* Fix for SPR 19486: (Implementation of RFC 4320)*/
             /* If the response code is 408 reject the API. 408 response to
              * non invite transaction is not allowed.*/
             if((p_icf_notify_cfm_st->response_code == 408) ||
                     (p_icf_notify_cfm_st->response_code < 400) ||  
                     (p_icf_notify_cfm_st->response_code >  699))
             {
                 ret_val = ICF_FAILURE; /* assign failure to ret val */
                 /* assign ecode */
                 p_vldt_err->ecode = ICF_CAUSE_INVALID_VALUE_FOR_RESPONSE_CODE;
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"response_code:");
             }/* End if*/
         }/* End if*/
   
     }/* End if*/

     /* Asynchronous NOTIFY Support */
    /* check: bit_mask for header_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_notify_cfm_st->bit_mask &  ICF_HDR_LIST_PRESENT)
        {
            /* Check: header_list should be a valid icf_header_list_st*/
            ret_val = icf_vldt_icf_header_list_st(
                      &(p_icf_notify_cfm_st->header_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */   
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for tag_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_notify_cfm_st->bit_mask & ICF_TAG_LIST_PRESENT)
        {
            /* Check: tag_list should be a valid icf_tag_list_st */
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_notify_cfm_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for body_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_notify_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
        {
            /* Check: body_list should be a valid icf_msg_body_list_st */
            ret_val = icf_vldt_icf_msg_body_list_st(
                      &(p_icf_notify_cfm_st->body_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
  
     return(ret_val);
}



/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_options_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_options_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_options_req_st(
                            IN icf_uint8_t *p_payload, 
                            INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
    icf_options_req_st *p_icf_options_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_options_req_st = (icf_options_req_st *)p_payload ;
    /* Check: validate line_id */
    ret_val = icf_vldt_icf_line_id_t(
                  (p_icf_options_req_st->line_id),p_vldt_err,0);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }/* End if*/
    /* check: bit_mask for destination_info for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        /* Check: destination_info should be a valid icf_address_st */
        ret_val = icf_vldt_icf_address_st(
                  &(p_icf_options_req_st->destination_info),p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"destination_info:");
        }/* End if*/        
    }/* End if*/
    /* check: bit_mask for pref_identity for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_options_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
        {
            /* Check: pref_identity should be a valid icf_address_st */
            ret_val = icf_vldt_icf_address_st(
                      &(p_icf_options_req_st->pref_identity),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"pref_identity:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for header_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_options_req_st->bit_mask &  ICF_HDR_LIST_PRESENT)
        {
            /* Check: header_list should be a valid icf_header_list_st*/
            ret_val = icf_vldt_icf_header_list_st(
                      &(p_icf_options_req_st->header_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */   
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for tag_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_options_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
        {
            /* Check: tag_list should be a valid icf_tag_list_st */
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_options_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for body_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_options_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
        {
            /* Check: body_list should be a valid icf_msg_body_list_st */
            ret_val = icf_vldt_icf_msg_body_list_st(
                      &(p_icf_options_req_st->body_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/


     if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_options_req_st->bit_mask &
                       ICF_OPTIONS_TUNNEL_MODE_OPERATION_PRESENT)

        {
            /* Check for session timer logic  flag  */
            ret_val=icf_vldt_boolean(
            p_icf_options_req_st->tunnel_mode,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"tunnel_mode:");
            }/* End if*/
        } /*End if */
    } /* End if */

    /* Fix for SPR 20412: CSR 1-8223807 merged*/
    /* header_name_list should be a validated against
     * icf_vldt_icf_header_name_list_st
     */
    if(ICF_SUCCESS == ret_val)
     {
         ret_val = icf_vldt_icf_header_name_list_st(
                   &(p_icf_options_req_st->remove_hdr_list),p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"remove_hdr_list:");
         }/* End if*/
     }/* End if*/

    return(ret_val);
}



/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_options_cfm_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_options_cfm_st api.
*****************************************************************************/
icf_return_t icf_vldt_icf_options_cfm_st(
                   IN icf_uint8_t *p_payload, 
                   INOUT icf_vldt_err_st *p_vldt_err)
{
        icf_return_t ret_val = ICF_SUCCESS;
        icf_list_st *p_temp_node = ICF_NULL;
        /* initialize local variables */
        icf_uint8_t count = 0,i,del_count=0;
        /* Initialsie structure locally */
        icf_options_cfm_st *p_icf_options_cfm_st = ICF_NULL;
        /* Assign api payload to local structure */
        p_icf_options_cfm_st = (icf_options_cfm_st *)p_payload;
        /*bit mask checking*/
        if(p_icf_options_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT) 
        {
             /* if header list present ,then check for it's accuracy */
             ret_val = icf_vldt_icf_header_list_st(
                       &(p_icf_options_cfm_st->header_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
             }/* End if*/
        }/* End if*/
        if(ICF_SUCCESS == ret_val)
        {
            ret_val = icf_vldt_boolean(p_icf_options_cfm_st->result,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
            }/* End if*/
        }/* End if*/
        if(ICF_SUCCESS == ret_val)
        {
            /* bit mask checking */
            if(p_icf_options_cfm_st->bit_mask & ICF_TAG_LIST_PRESENT)
            {
                 /* if tag list present ,then check for it's accuracy */
                 ret_val = icf_vldt_icf_tag_list_st(
                           &(p_icf_options_cfm_st->tag_list),
                           p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                     /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
                 }/* End if*/
            }/* End if*/
        }/* End if*/
        if(ICF_SUCCESS == ret_val)
        {
            /* bit mask checking */
            if(p_icf_options_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
            {
                 /* if body list present ,then check for it's accuracy */
                 ret_val = icf_vldt_icf_msg_body_list_st(
                           &(p_icf_options_cfm_st->body_list),p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                      /* Add error string to p_vldt_err structure */
                      icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
                 }/* End if*/
            }/* End if*/      
        }/* End if*/
        
        if(ICF_SUCCESS == ret_val)
        {
            /* Check: p_app_sdp_list should of type icf_list_st and
               each node hould be of type icf_stream_capab_st
            */  
            /* Assign p_icf_options_cfm_st->p_app_sdp_list to p_temp_node */
            p_temp_node = p_icf_options_cfm_st->p_app_sdp_list;
            /*bit mask checking */
            if(p_icf_options_cfm_st->bit_mask & ICF_OPTIONS_CFM_MEDIA_PRESENT) 
            {
                 /* if stream list present ,then check for it's accuracy */
                 while(p_temp_node != ICF_NULL)
                 {
                     /*check each node is of type p_stream_capab_st */
                     ret_val = icf_vldt_icf_stream_capab_st(
                               p_temp_node->p_data,p_vldt_err);
                     if(ICF_FAILURE == ret_val)
                     {
                         /* Add error string to p_vldt_err structure */
                         icf_vldt_port_strcat(p_vldt_err->err_str,
                                       (icf_uint8_t *)"p_app_sdp_list:");
                         break;
                     } /* End of if(ICF_FAILURE == ret_val) */
                     p_temp_node=p_temp_node->p_next;
                  } /* End of while */
             }/* End if*/
        }/* End if*/
        if(ICF_SUCCESS == ret_val)
        {
            /* bit maks checking for deleted media streams */
            if(p_icf_options_cfm_st->bit_mask & 
                          ICF_OPTIONS_CFM_DEL_MEDIA_PRESENT) 
            {
                /* Check for number odf deleted media count */
                if(p_icf_options_cfm_st->count > ICF_MAX_STREAM_PER_CALL)
                {

                    ret_val = ICF_FAILURE; /* Assign failure to ret val */
                    /* Assign ecode  if stream count is out of range*/
                    p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_OUT_OF_RANGE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:");
                }
                /*if number of deleted media stream are in valid range */
                else if((p_icf_options_cfm_st->count > 0) && 
                       (p_icf_options_cfm_st->count < ICF_MAX_STREAM_PER_CALL))
                {
                    /*initialise countto number of deleted maeida count */
                    count = p_icf_options_cfm_st->count;
                    /* Check for the equality of number of media count 
                       and actual media streams present 
                    */
                    for(i=0 ; i < count ;i++) 
                    {
                        del_count ++; 
                        
                    }/* end for */
                    /* id del_count is not equal to actual deleetd media count*/
                    if(del_count != count)
                    {
                        /* Assign failure to ret val */
                        ret_val = ICF_FAILURE ;
                        /* assign ecode */
                        p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_MISMATCH ;
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:"); 
                    }/* End if*/

                }/* End if*/
                /* if both the above validations are passed, 
                   then check for actual deleted media astreams 
                */
                if( ICF_SUCCESS == ret_val)
                {
                    /* Determine the validity of each deleted media 
                       stream one by one 
                    */
                    /* In case of failure come out of the loop */
                    for(i=0; ((i < p_icf_options_cfm_st->count) 
                              && (ICF_SUCCESS == ret_val)) ; i++) 
                    {
                        ret_val = icf_vldt_icf_stream_list_st(
                             &(p_icf_options_cfm_st->deleted_media_streams[i]),
                             p_vldt_err);
                        if(ICF_FAILURE == ret_val)
                        {
                            /* Add error string to p_vldt_err structure */
                            icf_vldt_port_strcat(p_vldt_err->err_str,
                                          (icf_uint8_t *)"deleted_media_streams:");
                             break;
                        }/* End if*/
                    }/* end for */
                }/* End if*/    
            }/* End if*/

        }/* End if*/
        
        /* Fix for SPR 19486: (Implementation of RFC 4320)*/
        /* check: bit_mask for response_code for its presence */
        if(ICF_SUCCESS == ret_val)
        {
            if((ICF_FALSE == p_icf_options_cfm_st->result) && 
                    (p_icf_options_cfm_st->bit_mask & ICF_OPTIONS_CFM_FAILURE_RESP_PRESENT))
            {
                /* Check: response_code for its maximum and minimum values */
                /* If the response code is 408 reject the API. 408 response to
                 * non invite transaction is not allowed.*/
                if((p_icf_options_cfm_st->response_code == 408) ||
                        (p_icf_options_cfm_st->response_code < 400) ||
                        (p_icf_options_cfm_st->response_code > 699)) 
                {
                    ret_val = ICF_FAILURE;
                    p_vldt_err->ecode = 
                        ICF_CAUSE_INVALID_VALUE_FOR_RESPONSE_CODE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"response_code:");
                }/* End if*/
            }/* End if*/
        }/* End if*/        

        return(ret_val);
}    


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_info_cfm_st
*       
* DESCRIPTION:     This function is invoked  to validate the icf_info_cfm_st api*
*****************************************************************************/
icf_return_t icf_vldt_icf_info_cfm_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
        icf_return_t  ret_val = ICF_SUCCESS;
        /* Initialsie structure locally */        
        icf_info_cfm_st    *p_icf_info_cfm_st = ICF_NULL;
        /* Assign api payload to local structure */
        p_icf_info_cfm_st    = (icf_info_cfm_st *)p_payload;
        /* Check for value of the result  */
        ret_val = icf_vldt_boolean(p_icf_info_cfm_st->result,p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
        }/* End if*/
        /* check: bit_mask for response_code for its presence */
        if(ICF_SUCCESS == ret_val)
        {
         
	    if((ICF_FALSE == p_icf_info_cfm_st->result) && 
                (p_icf_info_cfm_st->bit_mask & ICF_INFO_CFM_RESPONSE_CODE_PRESENT))
            {
                /* Check: response_code for its maximum and minimum values */
                /* Fix for SPR 19486: (Implementation of RFC 4320)*/
                /* If the response code is 408 reject the API. 408 response to
                 * non invite transaction is not allowed.*/
                if((p_icf_info_cfm_st->response_code == 408) ||
                        (p_icf_info_cfm_st->response_code < 400) ||
                     (p_icf_info_cfm_st->response_code > 699)) 
                {
                    ret_val = ICF_FAILURE;
                    p_vldt_err->ecode = 
                                    ICF_CAUSE_INVALID_VALUE_FOR_RESPONSE_CODE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"response_code:");
                }/* End if*/
           }/* End if*/

        }/* End if*/
  
        if(ICF_SUCCESS == ret_val)
        {

            /*bit mask checking */
            if(p_icf_info_cfm_st->bit_mask & ICF_INFO_CFM_HEADER_LIST_PRESENT) 
            {
                /* if header list present ,then check for it's accuracy */
                ret_val = icf_vldt_icf_header_list_st(
                          &(p_icf_info_cfm_st->header_list),p_vldt_err);
                if(ICF_FAILURE == ret_val)
                {
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
                }/* End if*/
            }/* End if*/
        }/* End if*/
        if(ICF_SUCCESS == ret_val)
        {

            /*bit mask checking */
 
            if(p_icf_info_cfm_st->bit_mask & ICF_INFO_CFM_TAG_LIST_PRESENT) 
            {
                 /* if tag list present ,then check for it's accuracy */
                 ret_val = icf_vldt_icf_tag_list_st(
                           &(p_icf_info_cfm_st->tag_list),p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                      /* Add error string to p_vldt_err structure */
                      icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
                 }/* End if*/
            }/* End if*/
        }/* End if*/
        if(ICF_SUCCESS == ret_val)
        {

            /*bit mask checking */
            if(p_icf_info_cfm_st->bit_mask & ICF_INFO_CFM_BODY_LIST_PRESENT) 
            {
                /* if body list present ,then check for it's accuracy */
                 ret_val = icf_vldt_icf_msg_body_list_st(
                           &(p_icf_info_cfm_st->body_list),p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                     /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
                 }/* End if*/
             }/* End if*/
       }/* End if*/
       return ret_val ;
}
        
              
/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_early_incoming_call_cfm_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_early_incoming_call_cfm_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_early_incoming_call_cfm_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    /* Initialsie structure locally */
    icf_early_incoming_call_cfm_st *p_icf_early_incoming_call_cfm_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_early_incoming_call_cfm_st=(icf_early_incoming_call_cfm_st*)p_payload;
    /* Check for value of the result  */
    ret_val= icf_vldt_boolean(p_icf_early_incoming_call_cfm_st->result,
             p_vldt_err); 
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {

        /* check for line id */
        ret_val = icf_vldt_icf_line_id_t(
                  (p_icf_early_incoming_call_cfm_st->line_id),
                  p_vldt_err,0); 
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
        }/* End if*/
    }/* End if*/

     if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_early_incoming_call_cfm_st->bit_mask &
                       ICF_EARLY_CALL_CFM_TUNNEL_MODE_OPERATION_PRESENT)

        {
            /* Check for session timer logic  flag  */
            ret_val=icf_vldt_boolean(
            p_icf_early_incoming_call_cfm_st->tunnel_mode,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"tunnel_mode:");
            }/* End if*/
        } /*End if */
    } /* End if */




if(ICF_SUCCESS == ret_val)
     {    
         /* check for the bitmask for response code */      
	 if((ICF_FALSE == p_icf_early_incoming_call_cfm_st->result) &&
               (p_icf_early_incoming_call_cfm_st->bit_mask & ICF_EARLY_CALL_CFM_RESPONSE_CODE_PRESENT))
         { 
             /* Check for response code .It's value should be between 400-699*/ 
             if((p_icf_early_incoming_call_cfm_st->response_code < 400) ||  
                (p_icf_early_incoming_call_cfm_st->response_code >  699))
             {
                 ret_val = ICF_FAILURE; /* assign failure to ret val */
                 /* assign ecode */
                 p_vldt_err->ecode = ICF_CAUSE_INVALID_VALUE_FOR_RESPONSE_CODE;
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"response_code:");
             }/* End if*/
         }/* End if*/
   
     }/* End if*/

#ifdef ICF_SESSION_TIMER
    /* check for bitmask of session timer enable */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_early_incoming_call_cfm_st->bit_mask &
                       ICF_EARLY_CALL_CFM_SESSION_TIMER_ENABLE_PRESENT)
        {
            /* Check for session timer enable flag  */
            ret_val=icf_vldt_boolean(
                  p_icf_early_incoming_call_cfm_st->session_timer_enable,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"session_timer_enable:");
            }/* End if*/          
        } /*End if */
    } /* End if */
#endif

    return (ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_get_system_settings_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_get_system_settings_req_st api.
*****************************************************************************/

icf_return_t icf_vldt_icf_get_system_settings_req_st(
                             IN icf_uint8_t *p_payload,
                             INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t  ret_val = ICF_SUCCESS;
     /* Initialsie structure locally */
     icf_get_system_settings_req_st *p_icf_get_system_settings_req_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_get_system_settings_req_st=(icf_get_system_settings_req_st *)p_payload;
     /* check:routing_info.ptr should be validated against
        icf_route_info_st
     */
     if((ICF_SUCCESS == ret_val)&&
       (p_icf_get_system_settings_req_st->bit_mask & 
        ICF_GET_SETTINGS_ROUTE_INFO_PRESENT))
     {
         ret_val = icf_vldt_icf_route_info_st(
                            (icf_route_info_st *)
                            (p_icf_get_system_settings_req_st->routing_info.ptr),
                            p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"routing_info:");
         }/* End if*/
     }/* End if*/
     return(ret_val);
}

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_get_line_settings_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_get_line_settings_req_st api.
*****************************************************************************/
icf_return_t icf_vldt_icf_get_line_settings_req_st(
                             IN icf_uint8_t *p_payload, 
                             INOUT icf_vldt_err_st *p_vldt_err)
{
        icf_return_t  ret_val = ICF_SUCCESS;       
        /* Initialsie structure locally */ 
        icf_get_line_settings_req_st *p_icf_get_line_settings_req_st = ICF_NULL;
        /* Assign api payload to local structure */
        p_icf_get_line_settings_req_st= 
                                      (icf_get_line_settings_req_st *)p_payload;
        /* check for line id */

        ret_val = icf_vldt_icf_line_id_t(
                    (p_icf_get_line_settings_req_st->line_id),
                                                  p_vldt_err,0);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
        }/* End if*/
        /* check:routing_info.ptr should be validated against
           icf_route_info_st
        */ 
        if(ICF_SUCCESS == ret_val)
        {
           if(p_icf_get_line_settings_req_st->bit_mask & ICF_GET_SETTINGS_ROUTE_INFO_PRESENT)
           {
            ret_val = icf_vldt_icf_route_info_st(
                      (icf_route_info_st *)
                      (p_icf_get_line_settings_req_st->routing_info.ptr),
                      p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"routing_info:");
            }/* End if*/

        }/* End if*/
        }/* End if*/
        return ret_val;
}

#ifdef ICF_STAT_ENABLED
/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_get_icf_stats_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_get_icf_stats_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_get_icf_stats_req_st(
                              icf_uint8_t *p_payload, 
                              icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
    icf_get_icf_stats_req_st *p_icf_get_icf_stats_req = ICF_NULL;
    /* Assign api payload to local structure */                                 
     p_icf_get_icf_stats_req = (icf_get_icf_stats_req_st *) p_payload;
     /* Check for value of the result  */
     ret_val=icf_vldt_boolean(p_icf_get_icf_stats_req->reset,p_vldt_err);
     if(ICF_FAILURE == ret_val)
     {
         /* Add error string to p_vldt_err structure */
         icf_vldt_port_strcat(p_vldt_err->err_str,"reset:");
     }/* End if*/
     return ret_val; 
}
#endif

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_set_transport_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_set_transport_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_set_transport_req_st(
                        IN icf_uint8_t *p_payload, 
                        INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
    icf_set_transport_req_st *p_icf_set_transport_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_set_transport_req_st = (icf_set_transport_req_st *)p_payload;
    /* check: bit_mask for self_transport_address for its presence */
    if(p_icf_set_transport_req_st->bit_mask & 
                ICF_SET_TRP_REQ_SLF_TRANSPORT_ADDRESS)
    {
        /* Check: self_transport_address should be a valid
           icf_transport_addr_st
        */
        ret_val = icf_vldt_icf_transport_addr_st(
                  &(p_icf_set_transport_req_st->self_transport_address),
                  p_vldt_err,ICF_TRUE); 
        if(ICF_FAILURE == ret_val)
        {
           /* Add error string to p_vldt_err structure */
           icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"self_transport_address:");
        }/* End if*/	     
       
    }/* End if*/
    /* check: bit_mask for self_mode for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SLF_MODE)
        {
            /* Check: self_mode should be a valid icf_transport_mode_t */
            ret_val = icf_vldt_icf_transport_mode_t(
                      (p_icf_set_transport_req_st->self_mode),p_vldt_err);
           if(ICF_FAILURE == ret_val)
           {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"self_mode:");
           }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for self_port for its presence */
    if(ICF_SUCCESS == ret_val)
    {
         if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SLF_PORT)
         {
             /* Check: self_port should be within the valid prot range */
              ret_val = icf_vldt_port(
                         (p_icf_set_transport_req_st->self_port),
                          p_vldt_err);
              if(ICF_FAILURE == ret_val)
              {
                   /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"self_port:");
              }/* End if*/
         }/* End if*/
    } /* End if*/
    /* check: bit_mask for self_addr_per_line for its presence */
    if(ICF_SUCCESS == ret_val)
    {
         if(p_icf_set_transport_req_st->bit_mask & 
                    ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE)
         {
             /* Check: self_addr_per_line should be valid 
                icf_config_transport_address_st
             */
             ret_val = icf_vldt_icf_config_transport_address_st(
                       &(p_icf_set_transport_req_st->self_addr_per_line),
                       p_vldt_err);
             if(ICF_FAILURE == ret_val)
              {
                   /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"self_addr_per_line:");
              }/* End if*/
         }/* End if*/
    }/* End if*/
    /* check: bit_mask for proxy_addr for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_PROXY_ADDR)
        {
            /* Check: proxy_addr should be a valid icf_config_proxy_address_st
            */
            ret_val = icf_vldt_icf_config_proxy_transport_address_st(
                      &(p_icf_set_transport_req_st->proxy_addr),
                      p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                  /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t*)"proxy_addr:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    /* check: bit_mask for sec_proxy_addr for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_transport_req_st->bit_mask &ICF_SET_TRP_REQ_SEC_PROXY_ADDR)
        {
            /* 	Check: sec_proxy_addr should be a valid 
				icf_config_sec_transport_address_st */
            ret_val = icf_vldt_icf_config_sec_transport_address_st(
                      &(p_icf_set_transport_req_st->sec_proxy_addr),
                      p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                  /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"sec_proxy_addr:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_transport_req_st->bit_mask&ICF_SET_TRP_REQ_DNS_SERVER_ADDR)
        {
           /*  If DNS Server addrtess is domain name then return failure */
            if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
                p_icf_set_transport_req_st->dns_server_addr.addr.addr_type)
            {
                ret_val = ICF_FAILURE;
            }
            if (ICF_SUCCESS == ret_val)
            {
                /* Check: dns_server_addr should be a valid 
                   icf_transport_addr_st */
                ret_val = icf_vldt_icf_transport_addr_st(
                          &(p_icf_set_transport_req_st->dns_server_addr.addr),
                          p_vldt_err,ICF_TRUE);
            }

            if(ICF_FAILURE == ret_val)
            {
                  /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dns_server_addr:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_transport_req_st->bit_mask & 
			ICF_SET_TRP_REQ_SEC_DNS_SERVER_ADDR)
        {
            /*  If DNS Server addrtess is domain name then return failure */
            if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
                p_icf_set_transport_req_st->sec_dns_server_addr.addr.addr_type)
            {
                ret_val = ICF_FAILURE;
            }
            if (ICF_SUCCESS == ret_val)
            {
                /* Check: sec_dns_server_addr should be a valid 
                   icf_transport_addr_st */
                ret_val = icf_vldt_icf_transport_addr_st(
                          &(p_icf_set_transport_req_st->sec_dns_server_addr.addr),
                          p_vldt_err,ICF_TRUE);
            }
            if(ICF_FAILURE == ret_val)
            {
                  /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,
						(icf_uint8_t *)"sec_dns_server_addr:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        /* Check for the range of port which can be used */
        if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_DNS_SRC_PORT)
        {
            ret_val = icf_vldt_port(p_icf_set_transport_req_st->dns_source_port,p_vldt_err);

            if(ICF_FAILURE == ret_val)
            {
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dns_source_port:");
            }
        }
    }
    if(ICF_SUCCESS == ret_val)
    {
        /* Check  for DHCP Server address should be a valid */
        if(ICF_SET_TRP_REQ_DHCP_SERVER_ADDR & p_icf_set_transport_req_st->bit_mask)
        {
           /* Validate the address */
            ret_val = icf_vldt_icf_transport_addr_st(
                             &p_icf_set_transport_req_st->dhcp_server_addr.addr,
                             p_vldt_err,ICF_FALSE);
            
            if (ICF_SUCCESS == ret_val)
            {
                /*  If DHCP address is not IPV4 address then return failure */
                if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR != \
                       p_icf_set_transport_req_st->dhcp_server_addr.addr.addr_type)
                {
                    ret_val = ICF_FAILURE;
                }
            }
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"addr:");
            } /*  End of if(ICF_FAILURE == ret_val) */
            
            /*  Validation of DHCP Server Port number */    
            if (ICF_SUCCESS == ret_val)
            {
                if(ICF_NULL == p_icf_set_transport_req_st->dhcp_server_addr.port_num)
                {
                    ret_val = ICF_FAILURE;
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"port_num:");
                }
             }
       }
       /*  Validation of DHCP Client port */
       if (ICF_NULL != (ICF_SET_TRP_REQ_DHCP_CLIENT_PORT &
                   p_icf_set_transport_req_st->bit_mask))
       {
           if (ICF_NULL == p_icf_set_transport_req_st->dhcp_client_port)
           {
               ret_val = ICF_FAILURE;
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dhcp_client_port:");
           }
       }

    }

    return(ret_val);
}
                
 
/******************************************************************************
*  
* FUNCTION:        icf_vldt_icf_set_self_id_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_set_self_id_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_set_self_id_req_st(
                        IN icf_uint8_t *p_payload, 
                        INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
    icf_set_self_id_req_st  *p_icf_set_self_id_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_set_self_id_req_st = (icf_set_self_id_req_st *)p_payload ;
    /* Check: line_id*/
    ret_val = icf_vldt_icf_line_id_t(
              (p_icf_set_self_id_req_st->line_id),p_vldt_err,1);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }/* End if*/
    /* check: bit_mask for action for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_ADDR_ACTION)
        {
            /* Check: action should be a valid icf_config_action_t */
            ret_val = icf_vldt_icf_config_action_t(
                      (p_icf_set_self_id_req_st->action),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"action:");
            }/* End if*/
        }/* End if*/
   }/* End if*/
   /* check: bit_mask for self_addr for its presence */
   if(ICF_SUCCESS == ret_val)
   {
       if(p_icf_set_self_id_req_st->bit_mask &  ICF_SET_SLF_ID_PRM_ADDR)
       {
            /* Check: self_addr should be validated against icf_address_st.It should not contain a port number */
            /* Fix for set_self_id for SPR 17567 */
            ret_val = icf_vldt_icf_address_with_port_st(
                      &(p_icf_set_self_id_req_st->self_addr),
                      p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"self_addr:");
            }/* End if*/
       }/* End if*/

   }/* End if*/
   /* check: bit_mask for self_name for its presence */
   if(ICF_SUCCESS == ret_val)
   {
       if(p_icf_set_self_id_req_st->bit_mask &    ICF_SET_SLF_ID_PRM_NAME)
       {
           /* Check: self_name should be a valid string */
           ret_val = icf_vldt_icf_string_st(
                     &(p_icf_set_self_id_req_st->self_name),
                     p_vldt_err);
           if(ICF_FAILURE == ret_val)
            { 
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"self_name:");
            }/* End if*/
       }/* End if*/
   }/* End if*/
   /* check: bit_mask for authentication_key for its presence */
   if(ICF_SUCCESS == ret_val)
   {
       if(p_icf_set_self_id_req_st->bit_mask &  ICF_SET_SLF_ID_PRM_AUTH_KEY)
       {
           /* check: authentication_key should be a valid string */
           ret_val = icf_vldt_icf_string_st(
                     &(p_icf_set_self_id_req_st->authentication_key),
                     p_vldt_err);
           if(ICF_FAILURE == ret_val)
           {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"authentication_key:");
           }/* End if*/
       }/* End if*/
   }/* End if*/
   /* check: bit_mask for authentication_password for its presence */
   if(ICF_SUCCESS == ret_val)
   {
       if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_AUTH_PASSWORD)
       {
           /* check: authentication_password should be a valid string */
           ret_val = icf_vldt_icf_string_st(
                     &(p_icf_set_self_id_req_st->authentication_password),
                     p_vldt_err);
           if(ICF_FAILURE == ret_val)
           {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"authentication_password:");
           }/* End if*/
       }/* End if*/
   }/* End if*/
   /* check: bit_mask for configured_realm for its presence */
   if(ICF_SUCCESS == ret_val)
   {
       if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_REALM )
       {
          /* check: configured_Realm should be valid string */
           ret_val = icf_vldt_icf_string_st(
                     &(p_icf_set_self_id_req_st->configured_realm),
                     p_vldt_err);
           if(ICF_FAILURE == ret_val)
           {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"configured_realm:");
           }/* End if*/
       }/* End if*/
   }/* End if*/
   return(ret_val);
}

/******************************************************************************
*   
* FUNCTION:        icf_vldt_icf_set_registration_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_set_registration_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_set_registration_req_st(
                      IN icf_uint8_t *p_payload, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* Initialsie structure locally */
     icf_set_registration_req_st *p_icf_set_registration_req_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_set_registration_req_st = (icf_set_registration_req_st *)p_payload ;
     /* Check validate line_id */
     ret_val = icf_vldt_icf_line_id_t(
               (p_icf_set_registration_req_st->line_id),p_vldt_err,1);
     if(ICF_FAILURE == ret_val)
     {
         /* Add error string to p_vldt_err structure */
         icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
     }/* End if*/
     /* Check: action should be a valid config action */
     if(ICF_SUCCESS == ret_val)
     {
         ret_val = icf_vldt_icf_config_action_t(
                   (p_icf_set_registration_req_st->action),
                   p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"action:");
         }/* End if*/
         else if (ICF_CONFIG_ACTION_DELETE == 
                p_icf_set_registration_req_st->action)
         {
             /*Fix for CSR 1-8561947 SPR 20391.Application can specify the
              *action=delete only to delete the registration parameters. EGT
              *shall not validate for other parameters in this scenario.
              */
             return ret_val;
         }
     }/* End if*/
     /* Check: registrar_addr should be a valid transport address */
     if(ICF_SUCCESS == ret_val)
     {
         ret_val = icf_vldt_icf_transport_addr_reg_st(
                   &(p_icf_set_registration_req_st->registrar_addr),
                   p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"registrar_addr:");
         }/* End if*/
     }/* End if*/

     /*Check:sec_registrar_addr should be a valid secondary transport address */
     if(ICF_SUCCESS == ret_val)
     {
         if(p_icf_set_registration_req_st->bit_mask & ICF_SET_REG_SEC_REGISTRAR)
         {
             ret_val = icf_vldt_icf_config_sec_transport_address_st(
                   &(p_icf_set_registration_req_st->sec_registrar_addr),
                   p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"sec_registrar_addr:");
             }/* End if - FAILURE */
         }/* End if sec. registrar present */
     }/* End if*/

     /* Check: bit_mask for reg_mode,for its presence */
     if(ICF_SUCCESS == ret_val)
     {
         if(p_icf_set_registration_req_st->bit_mask & ICF_SET_REG_REQ_MODE)
         {
             /* Check: reg_mode should be validated against 
                icf_vldt_icf_transport_mode_t
             */
             ret_val = icf_vldt_icf_transport_mode_t(
                       (p_icf_set_registration_req_st->reg_mode),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"reg_mode:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     /* Check: bit_mask for registration_duration,for its presence */
     if(ICF_SUCCESS == ret_val)
     {
          if(p_icf_set_registration_req_st->bit_mask &  
                               ICF_SET_REG_REQ_DURATION)
          {
             ret_val = icf_vldt_icf_duration_t(
                       (p_icf_set_registration_req_st->registration_duration),
                        p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"registration_duration:");
             }/* End if*/
          }/* End if*/
     }/* End if*/
     /* Check: bit_mask for reg_route_addr,for its presence */ 
     if(ICF_SUCCESS == ret_val)
     {
         if((p_icf_set_registration_req_st->bit_mask & 
            ICF_SET_REG_REQ_ROUTE_ADDR) && 
            (p_icf_set_registration_req_st->action ==  ICF_CONFIG_ACTION_ADD))
         {
              ret_val = icf_vldt_icf_transport_addr_reg_st(
                        &(p_icf_set_registration_req_st->reg_route_addr),
                        p_vldt_err);
              if(ICF_FAILURE == ret_val)
              {
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"reg_route_addr:");
              }/* End if*/
         }/* End if*/
     }/* End if*/
     /* Check: bit_mask for register_head_start_timer,for its presence */ 
     if(ICF_SUCCESS == ret_val)
     {
         if(p_icf_set_registration_req_st->bit_mask & ICF_SET_REG_HS_DURATION )
         { 
             /* Check: registration_durtation should be more than 
                register_head_start_timer
             */
             if((p_icf_set_registration_req_st->registration_duration < 
               p_icf_set_registration_req_st->register_head_start_timer))
             {
                    ret_val = ICF_FAILURE;
                    /* Assign ecode */
                    p_vldt_err->ecode =  ICF_CAUSE_INVALID_REGISTER_HEAD_START_TIMER;

                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"register_head_start_timer:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     return (ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_set_line_param_req
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_set_line_param_req api.
*
****************************************************************************/
icf_return_t icf_vldt_icf_set_line_param_req(
                       IN icf_uint8_t *p_payload, 
                       INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
    icf_set_line_param_req_st *p_icf_set_line_param_req_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_set_line_param_req_st = (icf_set_line_param_req_st *)p_payload ;
    /* Check: line_id */
    ret_val = icf_vldt_icf_line_id_t(
              (p_icf_set_line_param_req_st->line_id),p_vldt_err,1);
    if(ICF_FAILURE == ret_val)
    {
         /* Add error string to p_vldt_err structure */
         icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }/* End if*/
     /* Check: bit_mask for sip_timer_t1,for its presence */
     if(ICF_SUCCESS == ret_val)
     {
         if(p_icf_set_line_param_req_st->bitmask &  ICF_SET_LINE_PRM_T1_TIMER)
         {
               /* Check: sip_timer_t1 should be validated agianst 
                  icf_duration_t
               */
               ret_val = icf_vldt_icf_sip_duration_t(
                         (p_icf_set_line_param_req_st->sip_timer_t1),
                          p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                     /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"sip_timer_t1:");
                 }/* End if*/
          }/* End if*/
     }/* End if*/
     /* Check: bit_mask for sip_timer_t2,for its presence */
     if(ICF_SUCCESS == ret_val)
     {
          if(p_icf_set_line_param_req_st->bitmask &  ICF_SET_LINE_PRM_T2_TIMER)
          {
              /* Check: sip_timer_t2 should be validated against 
                 icf_duration_t
              */
              ret_val = icf_vldt_icf_sip_duration_t(
                        (p_icf_set_line_param_req_st->sip_timer_t2),
                         p_vldt_err);
              if(ICF_FAILURE == ret_val)
              { 
                   /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"sip_timer_t2:");
              }/* End if*/
          }/* End if*/
     }	/* End if*/
    /* Check: bit_mask for sip_timer_invite,for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_line_param_req_st->bitmask &  ICF_SET_LINE_PRM_INV_TIMER)
        {
             /* Check: sip_timer_invite should be validated against 
                icf_duration_t
             */
             ret_val = icf_vldt_icf_sip_duration_t(
                       (p_icf_set_line_param_req_st->sip_timer_invite),
                        p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"sip_timer_invite:");
             }/* End if*/
         }/* End if*/
    }/* End if*/
    /* Check: bit_mask for sip_timer_noninvite,for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_line_param_req_st->bitmask & 
                   ICF_SET_LINE_PRM_NON_INV_TIMER)
        {
            /* Check: sip_timer_noninvite should be validated against
               icf_duration_t
            */
            ret_val = icf_vldt_icf_sip_duration_t(
                      (p_icf_set_line_param_req_st->sip_timer_noninvite),
                       p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"sip_timer_noninvite:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
#ifdef ICF_SESSION_TIMER
    /* Check: bit_mask for min_se,for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_MIN_SE )
        {
            /* Check: min_se should be greater than or equal to 90 */
            
            if(p_icf_set_line_param_req_st->min_se < 90)
            {
                 ret_val = ICF_FAILURE;
                 p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE;
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"min_se:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for session_timer,for its presence*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_line_param_req_st->bitmask &  ICF_SET_LINE_SESSION_TIMER)
        {
            /* Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19020)*/
            /* SPR 19020 : 
               If Min-SE is not configured by the application, its default value
               will be 90. In such a scenario, value of Session timer should be
               greater than or equal to 90,
               Else the value of Session Timer should be greater than or equal to
               the the Min SE configured by the application.
            */   
            if(p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_MIN_SE)
            {
                if(p_icf_set_line_param_req_st->session_timer < 
                        p_icf_set_line_param_req_st->min_se)
                {
                    ret_val = ICF_FAILURE;
                    p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_timer:");
                }
            }
            else
            {
                if(p_icf_set_line_param_req_st->session_timer < 90)
                {
                    ret_val = ICF_FAILURE;
                    p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_timer:");
                }
            }            
        }/* End if*/
    }/* End if*/
    /* Check: bit_mask for session_timer_refresher,for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_line_param_req_st->bitmask & 
                 ICF_SET_LINE_SESSION_TIMER_REFRESHER)
        {
            /* Check: session_timer_refresher should have a valid value */
            if((p_icf_set_line_param_req_st->session_timer_refresher != 
               ICF_SESSION_REFRESHER_NONE) &&
              (p_icf_set_line_param_req_st->session_timer_refresher != 
               ICF_SESSION_REFRESHER_UAC) &&
              (p_icf_set_line_param_req_st->session_timer_refresher != 
               ICF_SESSION_REFRESHER_UAS ))
            {
                    ret_val = ICF_FAILURE;
                    /* Assign ecode */
                    p_vldt_err->ecode = ICF_CAUSE_VALUE_NOT_DEFINED ;  
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_timer_refresher:");
            }/* End if*/
       }/* End if*/
    }/* End if*/
#endif
    /* check: bit_mask for p_codec_attributes_video,for its presence*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_AUDIO_CODEC)
        {
        }/* End if*/

    }/* End if*/
    /* check: bit_mask for p_codec_attributes_video,for its presence*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_VIDEO_CODEC)
        {
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for contact_user_info,for its presence */
    
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_line_param_req_st->bitmask & 
                    ICF_SET_LINE_CONTACT_USER_INFO)
        {
            /* Check: contact_user_info should be a valid string */
            ret_val = icf_vldt_icf_string_st(
                      &(p_icf_set_line_param_req_st->contact_user_info),
                      p_vldt_err);

            /*Merged CSR 1-8283140. Fix for CSR 1-8561947. */
            /* Find the character '@' in contact user info string. If '@' is present in
             * conatct user info string, then return failure. 
             */

            if(ICF_FAILURE != ret_val &&
             ICF_NULL != icf_vldt_port_strchr(p_icf_set_line_param_req_st->\
                                                        contact_user_info.str,'@'))
            {
                ret_val = ICF_FAILURE;
            }

            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"contact_user_info:");
            }/* End if*/
       }/* End if*/

    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
      if(ICF_SET_LINE_PRM_ACCESS_NW_INFO &
              p_icf_set_line_param_req_st->bitmask)
      {
               /* Check for access_network_info  */
               ret_val = icf_vldt_allow_null_icf_string_st(
                     &p_icf_set_line_param_req_st->access_network_info,
                     p_vldt_err);     
               if(ICF_FAILURE == ret_val)
               {   
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"access_network_info:");
               }   
      }
     }

    /* Check: bit_mask for sip_timer_t4,for its presence */
     if(ICF_SUCCESS == ret_val)
     {
          if(p_icf_set_line_param_req_st->bitmask &  ICF_SET_LINE_PRM_T4_TIMER)
          {
              /* Check: sip_timer_t4 should be validated against 
                 icf_duration_t
              */
              ret_val = icf_vldt_icf_sip_duration_t(
                        (p_icf_set_line_param_req_st->sip_timer_t4),
                         p_vldt_err);
              if(ICF_FAILURE == ret_val)
              { 
                   /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"sip_timer_t4:");
              }/* End if*/
          }/* End if*/
     }	/* End if*/

     /*Fix for CSR 1-7334184 : SPR 19740 */
    if((ICF_SUCCESS == ret_val) && 
            (p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_REMOVE_ROUTE_HDR))
    {
        ret_val=icf_vldt_boolean(
                p_icf_set_line_param_req_st->remove_route_hdr,p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,
                    (icf_uint8_t *)"remove_route_hdr:");
        }/* End if*/
    }/* End if */

    /*Fix for SPR: 19999 (Merged CSR 1-7547597 : SPR 19898)*/
    if((ICF_SUCCESS == ret_val) && 
            (p_icf_set_line_param_req_st->bitmask &
             ICF_SET_LINE_SESSION_REFRESH_METHOD_STRICT_UPDATE))
    {
        ret_val=icf_vldt_boolean(
                p_icf_set_line_param_req_st->session_refresh_method_strict_update,
                p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,
                    (icf_uint8_t *)"session_refresh_method_strict_update:");
        }/* End if*/
    }/* End if */    

    /*Fix for CSR 1-9012191 : SPR 20697*/
    if((ICF_SUCCESS == ret_val) && 
            (p_icf_set_line_param_req_st->bitmask &
             ICF_SET_LINE_STRIP_REQ_URI_PARAM))
    {
        ret_val=icf_vldt_boolean(
                p_icf_set_line_param_req_st->strip_req_uri_param,
                p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,
                    (icf_uint8_t *)"strip_req_uri_param:");
        }/* End if*/
    }/* End if */    

    return ret_val;
}
                  


/******************************************************************************
*    
* FUNCTION:       icf_vldt_icf_app_add_req
*       
* DESCRIPTION:    This function is invoked  to validate the icf_app_add_req api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_app_add_req_st(
                             IN icf_uint8_t *p_payload,
                             INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* initial some local variables */
     icf_list_st *p_temp_node = ICF_NULL;
     icf_filter_st  *p_filter_list=ICF_NULL;
     icf_uint32_t i;
     icf_uint32_t count = 0;
     /* Initialsie structure locally */
     icf_app_add_req_st *p_icf_app_add_req_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_app_add_req_st = (icf_app_add_req_st *)p_payload ;
     /*  Check: app_name should be valid string */
     ret_val = icf_vldt_icf_string_st(
               &(p_icf_app_add_req_st->app_name),p_vldt_err);
     if(ICF_FAILURE == ret_val)
     {
          /* Add error string to p_vldt_err structure */
          icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"app_name:");
     }/* End if*/
     /* Check: routing_info should be validated against 
        icf_vldt_icf_route_info_st 
     */
     if(ICF_SUCCESS == ret_val)
     {	
         ret_val=icf_vldt_icf_route_info_st(
                 (icf_route_info_st *)(p_icf_app_add_req_st->routing_info.ptr),
                  p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"routing_info:");
         }/* End if*/
     }/* End if*/
     /* header_name_list should be a validated against
        icf_vldt_icf_header_name_list_st
     */
     if(ICF_SUCCESS == ret_val)
     {
         ret_val = icf_vldt_icf_header_name_list_st(
                   &(p_icf_app_add_req_st->header_name_list),p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_name_list:");
         }/* End if*/
      }/* End if*/
      /* Check: supported_methods should be a valid string */
      if(ICF_SUCCESS == ret_val)
      {    
          ret_val = icf_vldt_icf_string_list_st(
                    &(p_icf_app_add_req_st->supported_methods),p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"supported_methods:");
          }/* End if*/
      }/* End if*/
      /* Check: supported_events should be a valid string */
      if(ICF_SUCCESS == ret_val)
      {
          ret_val = icf_vldt_icf_string_list_st(
                    &(p_icf_app_add_req_st->supported_events),p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"supported_events:");
          }/* End if*/
      }/* End if*/
      /* Check: supported_content_types should be a valid string */
      if(ICF_SUCCESS == ret_val)
      {  
          ret_val=icf_vldt_icf_string_list_st(
                  &(p_icf_app_add_req_st->supported_content_types),p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"supported_content_types:");
          }/* End if*/
       }/* End if*/
       /* Check: supported_encodings should be a valid string */
       if(ICF_SUCCESS == ret_val)
       { 
           ret_val = icf_vldt_icf_string_list_st(
                     &(p_icf_app_add_req_st->supported_encodings),p_vldt_err);
           if(ICF_FAILURE == ret_val)
           {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"supported_encodings:");
           }/* End if*/
        }/* End if*/
        /* Check: supported_languages should be a valid string */
        if(ICF_SUCCESS == ret_val)
        {
            ret_val = icf_vldt_icf_string_list_st(
                      &(p_icf_app_add_req_st->supported_languages),p_vldt_err);
            if(ICF_FAILURE == ret_val)
           {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"supported_languages:");
           }/* End if*/


        }/* End if*/    
        if(ICF_SUCCESS == ret_val)
        {
             /* Check: p_media_profile should of type icf_list_st and
                each node hould be of type icf_stream_capab_st
             */  
             /* Assign p_icf_app_add_req_st->p_media_profile to p_temp_node */
            
             p_temp_node = p_icf_app_add_req_st->p_media_profile;

            if(p_icf_app_add_req_st->bit_mask &  
                  ICF_APP_ADD_MEDIA_PROFILE_PRESENT)
            {
                 /* Traversing the p_stream_capab_list_st list */
                 while(p_temp_node != ICF_NULL)
                 {
                     /*check each node is of type p_stream_capab_st */
                     ret_val = icf_vldt_icf_stream_capab_st(
                               p_temp_node->p_data,p_vldt_err);
                     if(ICF_FAILURE == ret_val)
                     {
                         /* Add error string to p_vldt_err structure */
                         icf_vldt_port_strcat(p_vldt_err->err_str,
                                         (icf_uint8_t *)"p_media_profile:");
                         break;
                     }/* End if*/
                     p_temp_node=p_temp_node->p_next;
                 }/* End-while */
            }/* End if*/
       } /* End if*/
       if(ICF_SUCCESS == ret_val)
       {
           /* Check for value of the rtp_detected_reqd */
           ret_val = icf_vldt_boolean(
                     p_icf_app_add_req_st->rtp_detection_reqd,
                     p_vldt_err); 
           if(ICF_FAILURE == ret_val)
           {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"rtp_detection_reqd:");
           }/* End if*/
       }/* End if*/
        if(ICF_SUCCESS == ret_val)
        {
            /* Check for value of the merge_req_wo_rem_sdp*/
            ret_val = icf_vldt_boolean(
                      p_icf_app_add_req_st->merge_req_wo_rem_sdp, p_vldt_err); 
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"merge_req_wo_rem_sdp:");
            }/* End if*/
        }/* End if*/

        if( (ICF_SUCCESS == ret_val) && (p_icf_app_add_req_st->filter.count > 0) )
        {
        /* Check: matach_all should be validated against icf_vldt_boolean */
        if(ICF_SUCCESS == ret_val)
        {

             ret_val= icf_vldt_boolean(p_icf_app_add_req_st->filter.match_all, 
                      p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"match_all:");
             }/* End if*/
        }/* End if*/
        /* Check: count should be equal to no. of nodes in the list*/
        if(ICF_SUCCESS == ret_val)
        {
            for(i=0,(p_filter_list = (p_icf_app_add_req_st->filter.p_filter_list));
                (ICF_NULL!= p_filter_list);i++)
            {
                ++count;
                p_filter_list=p_filter_list->p_next;
            }/* end-for */
            if(count != p_icf_app_add_req_st->filter.count)
            {
                ret_val = ICF_FAILURE ;
                p_vldt_err->ecode = ICF_CAUSE_FILTER_COUNT_MISMATCH ;
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"filter_count:");
            }/* End if*/
       }/* End if*/
       /* Check: match_type should be validated for its valid values */
       if(ICF_SUCCESS == ret_val)
       {
           for(i=0,p_filter_list=p_icf_app_add_req_st->filter.p_filter_list ;
                      i<p_icf_app_add_req_st->filter.count && p_filter_list ;i++)
           {	
                if((p_filter_list->match_type != ICF_EXACT_STRING_MATCH)
                &&(p_filter_list->match_type != ICF_SUB_STRING_MATCH))
                {
                    ret_val = ICF_FAILURE ;
                    p_vldt_err->ecode = ICF_CAUSE_INVALID_VALUE_FOR_MATCH_TYPE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"match_type:");
                    break;
                }/* End if*/
           }/* end-for*/
       }/* End if*/
       /* Check: target_header.str should be validated against icf_string_st */
       if(ICF_SUCCESS == ret_val)
       {
           if(0 != p_icf_app_add_req_st->filter.count)
           {
             for(i=0,p_filter_list=p_icf_app_add_req_st->filter.p_filter_list ;
                    i<p_icf_app_add_req_st->filter.count && p_filter_list ;i++)
             {
                 ret_val = icf_vldt_icf_string_st(
                      &(p_filter_list->target_header),p_vldt_err);
                 p_filter_list=p_filter_list->p_next;

                if(ICF_FAILURE == ret_val)
                {
                 /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"target_header:");
                }/* End if*/
             }/*End of for*/
           }/*End of if*/
       }/* End if*/
       /* Check: expr_str.str should be validated against icf_string_st */
       if(ICF_SUCCESS == ret_val)
       {
          if(0 != p_icf_app_add_req_st->filter.count)
           {            
             for(i=0,p_filter_list=p_icf_app_add_req_st->filter.p_filter_list ;
                    i<p_icf_app_add_req_st->filter.count && p_filter_list ;i++)
             {
                 ret_val = icf_vldt_icf_string_st(
                            &p_filter_list->expr_str,
                            p_vldt_err);
                 p_filter_list=p_filter_list->p_next;

                 if(ICF_FAILURE == ret_val)
                 {
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"expr_str:");
                 }/* End if*/
              }/*End of for*/
            }/*End of if*/
       }/* End if*/
       }
       return ret_val;
}





/******************************************************************************
*
* FUNCTION:       icf_vldt_icf_create_xconf_req_st 
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_create_xconf_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_create_xconf_req_st(
                            IN icf_uint8_t *p_payload, 
                            INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
     icf_create_ext_xconf_req_st *p_icf_create_ext_xconf_req_st = ICF_NULL;
    icf_list_st *p_temp_node = ICF_NULL; 
     
    /* Assign api payload to local structure */
     p_icf_create_ext_xconf_req_st = (icf_create_ext_xconf_req_st *)
                                                                   p_payload ;
    /* Check: validate line_id */
    ret_val = icf_vldt_icf_line_id_t(
                  (p_icf_create_ext_xconf_req_st->line_id),p_vldt_err,0);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }/* End if*/
    
   if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_create_ext_xconf_req_st->bitmask & 
                ICF_CREATE_XCONF_CALLING_PARTY_ADDR_PRESENT)
        {
           /* check for calling party address */
           ret_val = icf_vldt_icf_address_st(
           &p_icf_create_ext_xconf_req_st->calling_party_address,p_vldt_err);
           if(ICF_FAILURE ==ret_val)
           {
              /* Add error string to p_vldt_err structure */
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"calling_party_address:");
            }/* End if*/
        }  
    }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_create_ext_xconf_req_st->bitmask &
                ICF_CREATE_XCONF_CALLING_PARTY_NAME_PRESENT)
        {
           /* Check for calling party name */
           ret_val = icf_vldt_icf_string_st(
         &p_icf_create_ext_xconf_req_st->calling_party_name,p_vldt_err);
           if(ICF_FAILURE == ret_val)
           {
              /* Add error string to p_vldt_err structure */
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"calling_party_name:");
           }/* End if*/
        }  
    }/* End if*/
    
    if(ICF_SUCCESS == ret_val)
    {
        /* bitmask checking */
        if(p_icf_create_ext_xconf_req_st->bitmask &
                ICF_CREATE_XCONF_TOGGLE_PRES_IND_PRESENT ) 
        {
            /* Check for toggle indicator */
            ret_val=icf_vldt_boolean(
            p_icf_create_ext_xconf_req_st->toggle_presentation_indicator,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"toggle_presentation_indicator:");
            }/* End if*/
        }/* End if*/
    }/* End if*/

    /* check: bit_mask for conference uri for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_CONF_URI_PRESENT)
        {
            /* Check: conference uri should be a valid icf_address_st */
            ret_val = icf_vldt_icf_address_st(
                      &(p_icf_create_ext_xconf_req_st->conference_uri),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"conference_uri:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for header_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_create_ext_xconf_req_st->bitmask &  ICF_HDR_LIST_PRESENT)
        {
            /* Check: header_list should be a valid icf_header_list_st*/
            ret_val = icf_vldt_icf_header_list_st(
                      &(p_icf_create_ext_xconf_req_st->header_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */   
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for tag_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_create_ext_xconf_req_st->bitmask & ICF_TAG_LIST_PRESENT)
        {
            /* Check: tag_list should be a valid icf_tag_list_st */
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_create_ext_xconf_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    
    
    if(ICF_SUCCESS == ret_val)
    {
        /* Check: p_pref_media_profile should of type icf_list_st and
           each node should be of type icf_stream_capab_st
        */  
         p_temp_node = p_icf_create_ext_xconf_req_st->p_pref_media_profile;

        /* bit mask  checking */
        if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_PREF_MEDIA_PROFILE_PRESENT)
        {  
            /* Check for the accuracy of stream list */
            while(p_temp_node != ICF_NULL)
            {
                /*check each node is of type p_stream_capab_st */
                ret_val = icf_vldt_icf_stream_capab_st(
                          p_temp_node->p_data,p_vldt_err);
                if(ICF_FAILURE == ret_val)
                {
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                                  (icf_uint8_t *)"p_pref_media_profile:stream_capab_list:");
                    break;
                } /* End of if(ICF_FAILURE == ret_val) */
                p_temp_node=p_temp_node->p_next;
             } /* End of while */
        }/* End if*/
    }/* End if*/
#ifdef ICF_QOS_SUPPORT
    /* Validating the precondition_required for boolean value*/
    if(ICF_SUCCESS == ret_val)
    {
        /* bitmask checking */
        if(p_icf_create_ext_xconf_req_st->bitmask &
                ICF_CREATE_XCONF_PRECONDITION_REQD_PRESENT)
        {
            /* Check for precondition_required */
            ret_val = icf_vldt_boolean(
                p_icf_create_ext_xconf_req_st->precondition_required, 
                p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                        (icf_uint8_t *)"precondition_required:");
            }/* End if*/
         }/* End if*/
     }/* End if*/
#endif    

 /* Validating the srtp_fallback_reqd  for boolean value*/
    if(ICF_SUCCESS == ret_val)
    {
        /* bitmask checking */
        if(p_icf_create_ext_xconf_req_st->bitmask &
                ICF_CREATE_XCONF_SRTP_FALLBACK_REQD_PRESENT)
        {
            /* Check for precondition_required */
            ret_val = icf_vldt_boolean(
                p_icf_create_ext_xconf_req_st->srtp_fallback_reqd,
                p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                        (icf_uint8_t *)"srtp_fallback_reqd:");
            }/* End if*/
         }/* End if*/
     }/* End if*/

    if(ICF_SUCCESS == ret_val)
    {

        if(p_icf_create_ext_xconf_req_st->bitmask &
                                     ICF_CREATE_XCONF_MEDIA_TYPE_PRESENT)
        {
            ret_val = icf_vldt_icf_media_transport_type_et(
                       p_icf_create_ext_xconf_req_st->media_transport_type,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"media_transport_type:");
            }/* End if*/
        }
    }


    return(ret_val);
}

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_add_xconf_party_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_add_xconf_party_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_add_xconf_party_req_st(
                            IN icf_uint8_t *p_payload, 
                            INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
     icf_add_xconf_party_req_st *p_icf_add_xconf_party_req_st = ICF_NULL;
       
    /* Assign api payload to local structure */
     p_icf_add_xconf_party_req_st = (icf_add_xconf_party_req_st *)
                                                                   p_payload ;
     if(p_icf_add_xconf_party_req_st->bitmask &  ICF_HDR_LIST_PRESENT)
     {
          /* Check: header_list should be a valid icf_header_list_st*/
          ret_val = icf_vldt_icf_header_list_st(
                    &(p_icf_add_xconf_party_req_st->header_list),p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */   
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
          }/* End if*/
     }/* End if*/
  
    /* check: bit_mask for tag_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_add_xconf_party_req_st->bitmask & ICF_TAG_LIST_PRESENT)
        {
            /* Check: tag_list should be a valid icf_tag_list_st */
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_add_xconf_party_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    
    
    return(ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_delete_xconf_party_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_delete_xconf_party_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_delete_xconf_party_req_st(
                            IN icf_uint8_t *p_payload, 
                            INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
     icf_delete_xconf_party_req_st *p_icf_delete_xconf_party_req_st = ICF_NULL;
     
    /* Assign api payload to local structure */
     p_icf_delete_xconf_party_req_st = (icf_delete_xconf_party_req_st *)
                                                                   p_payload ;
         
    /* Check: del_party_uri should be a valid icf_address_st */
    ret_val = icf_vldt_icf_address_st(
                  &(p_icf_delete_xconf_party_req_st->del_party_uri),p_vldt_err);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"del_party_uri:");
    }/* End if*/
    
    /* check: bit_mask for header_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_delete_xconf_party_req_st->bitmask &  ICF_HDR_LIST_PRESENT)
        {
            /* Check: header_list should be a valid icf_header_list_st*/
            ret_val = icf_vldt_icf_header_list_st(
                      &(p_icf_delete_xconf_party_req_st->header_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */   
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for tag_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_delete_xconf_party_req_st->bitmask & ICF_TAG_LIST_PRESENT)
        {
            /* Check: tag_list should be a valid icf_tag_list_st */
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_delete_xconf_party_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    
    return(ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_release_xconf_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_release_xconf_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_release_xconf_req_st(
                            IN icf_uint8_t *p_payload, 
                            INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Initialsie structure locally */
     icf_release_xconf_req_st *p_icf_release_xconf_req_st = ICF_NULL;
       
    /* Assign api payload to local structure */
     p_icf_release_xconf_req_st = (icf_release_xconf_req_st *)
                                                                   p_payload ;
     if(p_icf_release_xconf_req_st->bitmask &  ICF_HDR_LIST_PRESENT)
     {
          /* Check: header_list should be a valid icf_header_list_st*/
          ret_val = icf_vldt_icf_header_list_st(
                    &(p_icf_release_xconf_req_st->header_list),p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */   
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
          }/* End if*/
     }/* End if*/
  
    /* check: bit_mask for tag_list for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_release_xconf_req_st->bitmask & ICF_TAG_LIST_PRESENT)
        {
            /* Check: tag_list should be a valid icf_tag_list_st */
            ret_val = icf_vldt_icf_tag_list_st(
                      &(p_icf_release_xconf_req_st->tag_list),p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    
    
    return(ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_local_user_alerted_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                   icf_local_user_alerted_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_local_user_alerted_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* initializing some local variables */
     icf_uint32_t count = 0;
     icf_uint32_t i = 0;
     icf_uint32_t del_count=0;
     /* Assigning and creating a temp node of type icf_list_st*/
     icf_list_st *p_temp_node = ICF_NULL;
     /* Initialsie structure locally */
      icf_local_user_alerted_req_st *p_icf_user_alerted_req_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_user_alerted_req_st =  ( icf_local_user_alerted_req_st*)p_payload;
     if(p_icf_user_alerted_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
     {
         /* Check for the validity of header list */
         ret_val = icf_vldt_icf_header_list_st(
                   &( p_icf_user_alerted_req_st->header_list),p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
         }/* End if*/
     }/* End if*/
      if(ICF_SUCCESS == ret_val)
     {
         /* Check for the bitmas for tag list*/
         if(p_icf_user_alerted_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
         {
             /* Check for the validity of tag list */
             ret_val = icf_vldt_icf_tag_list_st(
                       &(p_icf_user_alerted_req_st->tag_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check for the bitmas for body list*/
         if(p_icf_user_alerted_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
         {
             /* Check for the validity of body list */
             ret_val = icf_vldt_icf_msg_body_list_st(
                       &(p_icf_user_alerted_req_st->body_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     if((ICF_SUCCESS == ret_val) && (p_icf_user_alerted_req_st->bit_mask & ICF_STREAM_LIST_PRESENT) )
     {
         /* Check: p_media_modify_list should of type icf_list_st and
            each node hould be of type icf_stream_capab_st
         */
         /* Assign p_icf_reinvite_cfm_st->p_media_modify_list to p_temp_node */
          p_temp_node = p_icf_user_alerted_req_st->p_stream_list;

         /* Check for the bitmas for stream  list*/
                      /* Check for the validity of stream  list */
              while(p_temp_node != ICF_NULL)
              {
                 /*check each node is of type p_stream_capab_st */
                 ret_val = icf_vldt_icf_stream_capab_st(
                           p_temp_node->p_data,p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                     /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_stream_list:");
                     break;
                 } /* End of if(ICF_FAILURE == ret_val) */
                 p_temp_node=p_temp_node->p_next;
              } /* End of while */
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
          /* bit maks checking for deleted media streams */
          if(p_icf_user_alerted_req_st->bit_mask &
                 ICF_USER_ALERTED_DELETED_STREAM_PRESENT)
          {
               /* Check for number odf deleted media count */
               if(p_icf_user_alerted_req_st->count > ICF_MAX_STREAM_PER_CALL)
               {
                    ret_val = ICF_FAILURE; /* Assign failure to ret val */
                    /* Assign ecode  if stream count is out of range*/
                    p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_OUT_OF_RANGE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:");
                }/* End if*/
                else if((p_icf_user_alerted_req_st->count > 0) &&
                       (p_icf_user_alerted_req_st->count < ICF_MAX_STREAM_PER_CALL))
                /*if number of deleted media stream are in valid range */
                {
                    /*initialise countto number of deleted maeida count */
                    count = p_icf_user_alerted_req_st->count;
                    /* Check for the equality of number of media count
                       and actual media streams present
                    */
                    for(i=0 ; i < count ;i++)
                    {
                         del_count ++;
                        
                    }/* End for */

                    /* if del_count is not equal to actual
                       deleetd media count
                    */
                    if(del_count != count)
                    {
                        ret_val = ICF_FAILURE ;/* Assign failure to ret val */
                        /* assign ecode */
                        p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_MISMATCH;
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:");
                    }/* End if*/

                }/* End if*/
                /* if both the above validations are passed,
                   then check for actual deleted media astreams
                */
                if( ICF_SUCCESS == ret_val)
                {
                    /* Determine the validity of each
                       deleted media stream one by one
                    */
                    /* In case of failure come out of the loop */
                    for(i=0;((i<p_icf_user_alerted_req_st->count) &&
                                       (ICF_SUCCESS == ret_val)) ; i++)
                    {
                        ret_val = icf_vldt_icf_stream_list_st(
                             &(p_icf_user_alerted_req_st->deleted_media_streams[i]),
                             p_vldt_err);
                        if(ICF_FAILURE == ret_val)
                        {
                            /* Add error string to p_vldt_err structure */
                            icf_vldt_port_strcat(p_vldt_err->err_str,
                                         (icf_uint8_t *)"delete_media_streams:");
                            break;
                        }/* End if*/
                    }/* End for */

                }/* End if*/

         }/* End if*/

     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
	if(p_icf_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_EARLY_MEDIA_SUPPORTED)
        {
             ret_val=icf_vldt_boolean(
                                    p_icf_user_alerted_req_st->early_media_reqd,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"early_media_reqd:");
            }/* End if*/
        }/* End if*/
     }   
     if(ICF_SUCCESS == ret_val)
     {
            ret_val=icf_vldt_boolean(
                                    p_icf_user_alerted_req_st->inband_info_status,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"inband_info_status:");
            }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
	if(p_icf_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_ALERT_TIMER_PRESENT)
        {
            if( p_icf_user_alerted_req_st->alert_timer_duration == 0)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"alert_timer_duration:");
            }/* End if*/
        }/* End if*/
     }   
      if((ICF_SUCCESS == ret_val) && (p_icf_user_alerted_req_st->bit_mask & ICF_PRIVACY_IND_PRESENT))
     {
            ret_val=icf_vldt_boolean(
                                    p_icf_user_alerted_req_st->privacy_ind,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"privacy_ind:");
            }/* End if*/
     }/* End if */
     if(ICF_SUCCESS == ret_val)
     {
	if(p_icf_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_MEDIA_MODE_PRESENT)
        {
         /* check the value of media  mode */
             ret_val =  icf_vldt_icf_media_mode_t(p_icf_user_alerted_req_st->media_mode, p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"media_mode:");
             }
        }
     } 
      if((ICF_SUCCESS == ret_val) && (p_icf_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_EARLY_MEDIA_SUPPORTED))
     {
         /* check the value of media  mode */
         ret_val =  icf_vldt_icf_media_mode_t(p_icf_user_alerted_req_st->early_mode, p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"early_mode:");
         }
        
     } 
      if(ICF_SUCCESS == ret_val)
      {
          /* bitmask checking */
          if(p_icf_user_alerted_req_st->bit_mask &
                  ICF_LOCAL_USER_ALERTED_PRACK_VALUE_PRESENT)
          {
              /* check for Prack_value */
              /* This field indicates 180 shall be sent reliable on               non reliable */
              /* This field indicates prack_support value 
                 0  No prack neither in supported nor in require 
                 2  100rel in require header
               */
              if((p_icf_user_alerted_req_st->prack_value != ICF_NO_PRACK) &&
                 (p_icf_user_alerted_req_st->prack_value != ICF_PRACK_IN_REQUIRE))
              {
                  ret_val=ICF_FAILURE;
                  /* assign ecode */
                  p_vldt_err->ecode = ICF_CAUSE_INVALID_PRACK_VALUE;
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,
                          (icf_uint8_t *)"prack_value:");
              }/* End if*/
          }/* End if*/
      }/* End if*/

     return(ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_media_connect_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                   icf_media_connect_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_media_connect_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* initializing some local variables */
     icf_uint32_t count = 0;
     icf_uint32_t i = 0;
     icf_uint32_t del_count=0;
     /* Assigning and creating a temp node of type icf_list_st*/
     icf_list_st *p_temp_node = ICF_NULL;
     /* Initialsie structure locally */
      icf_media_connect_req_st *p_icf_media_connect_req_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_media_connect_req_st =  (icf_media_connect_req_st*)p_payload;
     if(p_icf_media_connect_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
     {
         /* Check for the validity of header list */
         ret_val = icf_vldt_icf_header_list_st(
                   &( p_icf_media_connect_req_st->header_list),p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
         }/* End if*/
     }/* End if*/
      if(ICF_SUCCESS == ret_val)
     {
         /* Check for the bitmas for tag list*/
         if(p_icf_media_connect_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
         {
             /* Check for the validity of tag list */
             ret_val = icf_vldt_icf_tag_list_st(
                       &(p_icf_media_connect_req_st->tag_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
         /* Check for the bitmas for body list*/
         if(p_icf_media_connect_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
         {
             /* Check for the validity of body list */
             ret_val = icf_vldt_icf_msg_body_list_st(
                       &(p_icf_media_connect_req_st->body_list),p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
             }/* End if*/
         }/* End if*/
     }/* End if*/
     if((ICF_SUCCESS == ret_val) && (p_icf_media_connect_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)) 
     {
         /* Check: p_media_modify_list should of type icf_list_st and
            each node hould be of type icf_stream_capab_st
         */
         /* Assign p_icf_reinvite_cfm_st->p_media_modify_list to p_temp_node */
          p_temp_node = p_icf_media_connect_req_st->p_stream_list;

         /* Check for the bitmas for stream  list*/
                      /* Check for the validity of stream  list */
              while(p_temp_node != ICF_NULL)
              {
                 /*check each node is of type p_stream_capab_st */
                 ret_val = icf_vldt_icf_stream_capab_st(
                           p_temp_node->p_data,p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                     /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_stream_list:");
                     break;
                 } /* End of if(ICF_FAILURE == ret_val) */
                 p_temp_node=p_temp_node->p_next;
              } /* End of while */
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
          /* bit maks checking for deleted media streams */
          if(p_icf_media_connect_req_st->bit_mask &
                 ICF_MEDIA_CONNECT_DELETED_STREAM_PRESENT)
          {
               /* Check for number odf deleted media count */
               if(p_icf_media_connect_req_st->count > ICF_MAX_STREAM_PER_CALL)
               {
                    ret_val = ICF_FAILURE; /* Assign failure to ret val */
                    /* Assign ecode  if stream count is out of range*/
                    p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_OUT_OF_RANGE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:");
                }/* End if*/
                else if((p_icf_media_connect_req_st->count > 0) &&
                       (p_icf_media_connect_req_st->count < ICF_MAX_STREAM_PER_CALL))
                /*if number of deleted media stream are in valid range */
                {
                    /*initialise countto number of deleted maeida count */
                    count = p_icf_media_connect_req_st->count;
                    /* Check for the equality of number of media count
                       and actual media streams present
                    */
                    for(i=0 ; i < count ;i++)
                    {
                         del_count ++;
                        
                    }/* End for */

                    /* if del_count is not equal to actual
                       deleetd media count
                    */
                    if(del_count != count)
                    {
                        ret_val = ICF_FAILURE ;/* Assign failure to ret val */
                        /* assign ecode */
                        p_vldt_err->ecode = ICF_CAUSE_STREAM_COUNT_MISMATCH;
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"count:");
                    }/* End if*/

                }/* End if*/
                /* if both the above validations are passed,
                   then check for actual deleted media astreams
                */
                if( ICF_SUCCESS == ret_val)
                {
                    /* Determine the validity of each
                       deleted media stream one by one
                    */
                    /* In case of failure come out of the loop */
                    for(i=0;((i<p_icf_media_connect_req_st->count) &&
                                       (ICF_SUCCESS == ret_val)) ; i++)
                    {
                        ret_val = icf_vldt_icf_stream_list_st(
                             &(p_icf_media_connect_req_st->deleted_media_streams[i]),
                             p_vldt_err);
                        if(ICF_FAILURE == ret_val)
                        {
                            /* Add error string to p_vldt_err structure */
                            icf_vldt_port_strcat(p_vldt_err->err_str,
                                         (icf_uint8_t *)"delete_media_streams:");
                            break;
                        }/* End if*/
                    }/* End for */

                }/* End if*/

         }/* End if*/

     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {
	if(p_icf_media_connect_req_st->bit_mask & ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT)
        {
             ret_val=icf_vldt_icf_media_mode_t(
                                    p_icf_media_connect_req_st->media_mode,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"media_mode:");
            }/* End if*/
        }/* End if*/
     }   
     if((ICF_SUCCESS == ret_val) && (p_icf_media_connect_req_st->bit_mask & ICF_PRIVACY_IND_PRESENT))
     {
            ret_val=icf_vldt_boolean(
                                    p_icf_media_connect_req_st->privacy_ind ,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"privacy_ind:");
            }/* End if*/
     }/* End if*/

     if(ICF_SUCCESS == ret_val)
     {
         /* check the value of send without sdp */
         ret_val =  icf_vldt_boolean(p_icf_media_connect_req_st->send_without_sdp, p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"send_without_sdp:");
         }
        
     } 


     if(ICF_SUCCESS == ret_val)
     {
         /* bitmask checking */
         if(p_icf_media_connect_req_st->bit_mask &
                 ICF_MEDIA_CONNECT_REQ_PRACK_VALUE_PRESENT)
         {
             /* check for Prack_value */
             /* This field indicates 183 shall be sent reliable on non reliable */
             /* This field indicates prack_support value 
                0  No prack neither in supported nor in require 
                2  100rel in require header
              */
             if((p_icf_media_connect_req_st->prack_value != ICF_NO_PRACK) &&
                     (p_icf_media_connect_req_st->prack_value != ICF_PRACK_IN_REQUIRE))
             {
                 ret_val=ICF_FAILURE;
                 /* assign ecode */
                 p_vldt_err->ecode = ICF_CAUSE_INVALID_PRACK_VALUE;
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str, (icf_uint8_t *)"prack_value:");
             }/* End if*/
         }/* End if*/
     }/* End if*/

     return(ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_set_service_params_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_set_service_params_req_st  api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_set_service_params_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* Initialsie structure locally */
      icf_set_service_params_req_st *p_icf_set_service_params_req_st = ICF_NULL;
     /* Assign api payload to local structure */
      p_icf_set_service_params_req_st =  (icf_set_service_params_req_st*)p_payload;
      if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CALL_HOLD_OPTION)
      {
          if((p_icf_set_service_params_req_st->call_hold_option.hold_option < ICF_MIN_CALL_HOLD_OPTION) || (p_icf_set_service_params_req_st->call_hold_option.hold_option > ICF_MAX_CALL_HOLD_OPTION))               
     	  {
               ret_val = ICF_FAILURE;
               /* assign ecode */
               p_vldt_err->ecode = ICF_CAUSE_VALUE_OUT_OF_RANGE;
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"call_hold_option:");
          }
      }
      if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CALL_HOLD_OPTION)
      {
          ret_val = icf_vldt_icf_line_id_t(p_icf_set_service_params_req_st->call_hold_option.line_id,p_vldt_err,1);             if(ICF_FAILURE == ret_val)  
     	  {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"call_hold_option:");
          }
      }
 
      if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CFU_ADDR)
      {
          ret_val = icf_vldt_icf_config_address_st(&p_icf_set_service_params_req_st->addr_for_cfu,p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"addr_for_cfu:");
          }
       			   
      }
      if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CFB_ADDR)
      {
          ret_val = icf_vldt_icf_config_address_st(&p_icf_set_service_params_req_st->addr_for_cfb,p_vldt_err);       
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"addr_for_cfb:");
          }
       			   
      }
      if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CFD_ADDR)
      {
          ret_val = icf_vldt_icf_config_address_st(&p_icf_set_service_params_req_st->addr_for_cfd,p_vldt_err);       
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"addr_for_cfd:");
          }
       			   
      }
      if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CONF_REQD_AT_TRANFEREE)
      {
          ret_val = icf_vldt_boolean(p_icf_set_service_params_req_st->conf_reqd_at_transferee,p_vldt_err);       
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_conf_reqd_at_transferee:");
          }
       			   
      }
      if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CONF_CFM_REQD_AT_TRANSFEREE)
      {
          ret_val = icf_vldt_boolean(p_icf_set_service_params_req_st->conf_cfm_reqd_at_transferee,p_vldt_err);       
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"conf_cfm_reqd_at_transferee:");
          }
       			   
      }
      if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CONFERENCE_URI_PRESENT)
      {
          ret_val = icf_vldt_icf_config_address_st(&p_icf_set_service_params_req_st->conference_uri,p_vldt_err);       
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"conference_uri:");
          }
       			   
      }
 
     if(p_icf_set_service_params_req_st->bit_mask & ICF_SET_SER_PRM_CONF_FAIL_CALL_CLEAR_NOT_REQD)
	  {
			  ret_val = icf_vldt_boolean(p_icf_set_service_params_req_st->conf_fail_call_clear_not_reqd,p_vldt_err);       
			  if(ICF_FAILURE == ret_val)
			  {
					  /* Add error string to p_vldt_err structure */
					  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"conf_fail_call_clear_not_reqd");
			  }

	  }
                       	
     return(ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_set_call_params_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_set_call_params_req_st  api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_set_call_params_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
 
     /*The following is the list of error response codes which can be 
       configured by the application*/
     icf_list_st *p_temp_node = ICF_NULL;

     /* Initialsie structure locally */
      icf_set_call_params_req_st *p_icf_set_call_params_req_st = ICF_NULL;
     /* Assign api payload to local structure */
      p_icf_set_call_params_req_st =  (icf_set_call_params_req_st*)p_payload;
      if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_PHONE_INFO)
      {
               /* Check for phone_info  */
               ret_val = icf_vldt_icf_string_st(
                     &p_icf_set_call_params_req_st->phone_info,p_vldt_err);     
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"phone_info:");
      }

#ifdef ICF_SESSION_TIMER
    /* Check: bit_mask for min_se,for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL )
        {
            /* Check: min_se should be greater than or equal to 90 */

            if(p_icf_set_call_params_req_st->min_se < 90)
            {
                 ret_val = ICF_FAILURE;
                 p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE;
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"min_se:");
            }/* End if*/
        }/* End if*/
    }/* End if*/
    /* check: bit_mask for session_timer,for its presence*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask &  ICF_SET_CALL_PRM_SESSION_TIMER_INTERVAL)
        {
            /* Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19020)*/
            /* SPR 19020 : 
               If Min-SE is not configured by the application, its default value
               will be 90. In such a scenario, value of Session timer should be
               greater than or equal to 90,
               Else the value of Session Timer should be greater than or equal to
               the the Min SE configured by the application.
            */   
            
            if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL)
            {
                if(p_icf_set_call_params_req_st->session_timer < p_icf_set_call_params_req_st->min_se)
                {
                    ret_val = ICF_FAILURE;
                    p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_timer:");
                }
            }
            else
            {
                if(p_icf_set_call_params_req_st->session_timer < 90)
                {
                    ret_val = ICF_FAILURE;
                    p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_timer:");
                }
            }            
        }/* End if*/
    }/* End if*/
    /* Check: bit_mask for session_timer_refresher,for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask &
                 ICF_SET_CALL_PRM_SESSION_REFRESHER)
        {
            /* Check: session_timer_refresher should have a valid value */
            if((p_icf_set_call_params_req_st->session_refresher !=
               ICF_SESSION_REFRESHER_NONE) &&
              (p_icf_set_call_params_req_st->session_refresher !=
               ICF_SESSION_REFRESHER_UAC) &&
              (p_icf_set_call_params_req_st->session_refresher !=
               ICF_SESSION_REFRESHER_UAS ))
            {
                    ret_val = ICF_FAILURE;
                    /* Assign ecode */
                    p_vldt_err->ecode = ICF_CAUSE_VALUE_NOT_DEFINED ;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_refresher:");
            }/* End if*/
       }/* End if*/
    }/* End if*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask &
                       ICF_SET_CALL_PRM_SESSION_TIMER_ENABLE)

        {
            /* Check for session timer enable flag  */
            ret_val=icf_vldt_boolean(
            p_icf_set_call_params_req_st->session_timer_enable,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"session_timer_enable:");
            }/* End if*/          
        } /*End if */
    } /* End if */
     if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask &
                       ICF_SET_CALL_PRM_SESSION_TIMER_LOGIC)

        {
            /* Check for session timer logic  flag  */
            ret_val=icf_vldt_boolean(
            p_icf_set_call_params_req_st->session_timer_logic,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"session_timer_logic:");
            }/* End if*/
        } /*End if */
    } /* End if */
#endif 

    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask &
                      ICF_SET_CALL_PRM_MULTIPLE_ANSWER_ENABLE)

        {
            /* Check for multiple answer enable  flag  */
            ret_val=icf_vldt_boolean(
            p_icf_set_call_params_req_st->multiple_answer_enable,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"multiple_answer_enable:");
            }/* End if*/
        } /*End if */
    } /* End if */   
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask &
                      ICF_SET_CALL_PRM_UPDATE_REQD)

        {
            /* Check for update required  */
            /* Fix for CSR 1-7402396: SPR-19822*/
            if((p_icf_set_call_params_req_st->update_reqd !=
               ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC) &&
              (p_icf_set_call_params_req_st->update_reqd !=
               ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC) &&
              (p_icf_set_call_params_req_st->update_reqd !=
                ICF_NO_UPDATE ) &&
              (p_icf_set_call_params_req_st->update_reqd !=
               ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC_COMMIT) &&
              (p_icf_set_call_params_req_st->update_reqd !=
               ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC_COMMIT))
            {
                    ret_val = ICF_FAILURE;
                    /* Assign ecode */
                    p_vldt_err->ecode = ICF_CAUSE_VALUE_NOT_DEFINED ;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"update_reqd:");
            }/* End if*/
       }/* End if*/
         
    } /* End if */   
     if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask &
                      ICF_SET_CALL_PRM_EARLY_INC_IND_FOR_ALL_USERS)

        {
            /* Check for early_inc_ind_for_all_users   flag  */
            ret_val=icf_vldt_boolean(
            p_icf_set_call_params_req_st->early_inc_ind_for_all_users,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"early_inc_ind_for_all_users:");
            }/* End if*/
        } /*End if */
    } /* End if */
#ifdef ICF_NAT_RPORT_SUPPORT

    if(ICF_SUCCESS == ret_val)
    {
        /* Check for binding_query_enable flag  */
        ret_val=icf_vldt_boolean(
        p_icf_set_call_params_req_st->binding_query_enable,p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,
                            (icf_uint8_t *)"binding_query_enable:");
        }/* End if*/
    } /* End if */

#endif
   
    if(ICF_SUCCESS == ret_val)
    {
      if(p_icf_set_call_params_req_st->bit_mask & 
                         ICF_SET_CALL_PRM_ACCESS_NW_INFO)
      {
               /* Check for access_network_info  */
               ret_val = icf_vldt_allow_null_icf_string_st(
                     &p_icf_set_call_params_req_st->access_network_info,p_vldt_err);     
               if(ICF_FAILURE == ret_val)
               {   
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"access_network_info:");
               }   
      }
     }

    if(ICF_SUCCESS == ret_val)
    {
      if(p_icf_set_call_params_req_st->bit_mask &
                              ICF_CFG_CC_SET_CALL_PRM_MEDIA_TYPE_REQD)
      {
                /* Check for media_transport_type*/
                ret_val = icf_vldt_icf_media_transport_type_et(
                     p_icf_set_call_params_req_st->
                            media_transport_type,p_vldt_err);
                if(ICF_FAILURE == ret_val)
                {
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"media_transport_type:");
                }   
 
      }
    }
#ifdef ICF_QOS_SUPPORT
    /*For validating the value of precondition_required flag*/ 
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask &
                       ICF_SET_CALL_PRM_PRECONDITION_REQD)

        {
            /* Check for precondition_required flag  */
            ret_val = icf_vldt_boolean(
                   p_icf_set_call_params_req_st->precondition_required,
                   p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"precondition_required:");
            }/* End if*/
        } /*End if */
    } /* End if */
#endif    
    /*******************ICF 8.0****************/
    /*Validating the reg_event_subs_reqd field which indicate whether 
      application wants to configure support of reg event package
      subscription*/
      
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask &
                      ICF_SET_CALL_PRM_REG_EVENT_SUBS_REQD)
        {
            /* Check for reg_event_subs_reqd flag  */
            ret_val=icf_vldt_boolean(
            p_icf_set_call_params_req_st->reg_event_subs_reqd,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"reg_event_subs_reqd:");
            }/* End if*/
        } /*End if */
    } /* End if */         
    if (ICF_SUCCESS == ret_val)
    {
        if (ICF_SET_CALL_PRM_DHCP_RESPONSE_TIMER &
                   p_icf_set_call_params_req_st->bit_mask)
        {
            if (ICF_NULL == 
                   p_icf_set_call_params_req_st->dhcp_response_timer)
            {
                ret_val =ICF_FAILURE;
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"dhcp_response_timer:");

            }
        }
    } 
    if (ICF_SUCCESS == ret_val)
    {
        if (ICF_SET_CALL_PRM_SRTP_FALLBACK_FLAG &
                   p_icf_set_call_params_req_st->bit_mask)
        {
            /* Check for srtp_fallback_reqd flag  */
            ret_val=icf_vldt_boolean(
            p_icf_set_call_params_req_st->srtp_fallback_reqd,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"srtp_fallback_reqd:");
            }/* End if*/

        }
    }
    if (ICF_SUCCESS == ret_val)
    {
        if(ICF_SET_CALL_PRM_SRTP_FALLBACK_RESP_CODES &
                  p_icf_set_call_params_req_st->bit_mask)
        {
            p_temp_node = p_icf_set_call_params_req_st->
                              p_srtp_fallback_resp_codes;
        
            while(p_temp_node != ICF_NULL)
            {
                /* check for each response code.It's value 
                   should be between 400 and 699*/ 
                if((*((icf_uint16_t *)(p_temp_node->p_data)) < 400) || 
                     (*((icf_uint16_t *)(p_temp_node->p_data)) > 699))
                {
                    ret_val = ICF_FAILURE; 
                    /*Assign ecode to indicate failure */
                    p_vldt_err->ecode = \
                    ICF_CAUSE_INVALID_VALUE_FOR_RESPONSE_CODE ;

                    icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"p_srtp_fallback_resp_codes:");
                    break; 
                }
                p_temp_node=p_temp_node->p_next;
            }
        }
    }
    /*  Fix Merged for CSR 1-6456839 */
   if(ICF_SUCCESS == ret_val)
   {
       if(p_icf_set_call_params_req_st->bit_mask_1 &
                     ICF_SET_PORT_IN_FROM_HDR)
       {

            /* Check for binding_query_enable flag  */
            ret_val=icf_vldt_boolean(
            p_icf_set_call_params_req_st->port_to_be_sent_in_from_hdr,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"port_to_be_sent_in_from_hdr:");
            }/* End if*/
       }
   } /* End if */


     if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask_1 &
                       ICF_SET_CALL_PRM_EARLY_INC_SESSION_IND_REQD)

        {
            /* Check for session timer logic  flag  */
            ret_val=icf_vldt_boolean(
            p_icf_set_call_params_req_st->early_incoming_session_ind_reqd,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"early_incoming_session_ind_reqd:");
            }/* End if*/
        } /*End if */
    } /* End if */

   if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask_1 &
                       ICF_SET_CALL_PRM_RETRY_CODEC_COMMIT_REQ)

        {
            /* Check for retry_codec_commit_req flag  */
            ret_val=icf_vldt_boolean(
            p_icf_set_call_params_req_st->retry_codec_commit_req,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"retry_codec_commit_req:");
            }/* End if*/
        } /*End if */
    } /* End if */

	 if( ICF_SUCCESS == ret_val)
	 {
		if(p_icf_set_call_params_req_st->bit_mask_1 &
				 ICF_SET_CALL_PRM_UPDATE_FOR_OFFERANSWER_AFTER_CONNECT)

		 {
			 /* Check for update for offeranswer after connect flag  */
			 ret_val=icf_vldt_boolean(
				 p_icf_set_call_params_req_st->update_for_offeranswer_after_connect,
		 					 p_vldt_err);
			 if(ICF_FAILURE == ret_val)
			 {
				 /* Add error string to p_vldt_err structure */
				 icf_vldt_port_strcat(p_vldt_err->err_str,
					 (icf_uint8_t *)"update_for_offeranswer_after_connect:");
			 }/* End if*/
		 } /*End if */
	 } /* End if */

     /* Fix for CSR 1-6727350: SPR 19316 */
    if (ICF_SUCCESS == ret_val)
    {
        if (p_icf_set_call_params_req_st->bit_mask_1 & 
				ICF_SET_CALL_PRM_APP_CONNECT_REQD_FOR_REPLACES)
	{
            /* Check for app_connect_reqd_for_replaces flag */
            ret_val = icf_vldt_boolean(
	        p_icf_set_call_params_req_st->app_connect_reqd_for_replaces,
		p_vldt_err);
            if (ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str, 
                    (icf_uint8_t *)"app_connect_reqd_for_replaces:");
            }
        }
    }

    /* Asynchronous NOTIFY Support */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask_1 &
                       ICF_SET_CALL_PRM_ASYNC_NOTIFY_SUPPORT_REQD)
        {
            ret_val = icf_vldt_boolean(
            p_icf_set_call_params_req_st->async_notify_support_reqd,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"async_notify_support_reqd:");
            }/* End if*/
        } /*End if */
    } /* End if */

    /* Fix for CSR-1-6976670: Validate check_remote_codec_list */

    if( ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask_1 &
            ICF_SET_CALL_PRM_CHECK_REMOTE_CODEC_LIST)

        {
            /* Check for check_remote_codec_list flag  */
            ret_val=icf_vldt_boolean(
            p_icf_set_call_params_req_st->check_remote_codec_list,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                    (icf_uint8_t *)"check_remote_codec_list:");
            }/* End if*/
        } /*End if */
    } /* End if */   


    /* CSR_1_7113139 Merged SPR 19672 Start */ 
    /*SPR 19462*/
    if( ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask_1 &
                      ICF_SET_CALL_PRM_INC_SDP_VERSION_WHEN_NO_CHANGE)

        {
	    /* Check for inc_sdp_version_when_no_change flag  */
            ret_val=icf_vldt_boolean(
                     p_icf_set_call_params_req_st->inc_sdp_version_when_no_change,
                     p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                       (icf_uint8_t *)"inc_sdp_version_when_no_change:");
            }/* End if*/
        } /*End if */
    } /* End if */   
    /* CSR_1_7113139 Merged SPR 19672 End */		

    /* Asynchronous MESSAGE Support */
     if(ICF_SUCCESS == ret_val)
     {
         if(p_icf_set_call_params_req_st->bit_mask_1 & ICF_SET_CALL_PRM_ASYNC_MESSAGE_SUPPORT_REQD)
         {
             /* Check for Asynchronus MESSAGE support Flag  */
             ret_val=icf_vldt_boolean(
                     p_icf_set_call_params_req_st->async_message_support_reqd,p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                         (icf_uint8_t *)"async_message_support_reqd:");
             }/* End if*/
         } /*End if */
     } /* End if */


    return (ret_val);
}


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_get_register_status_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_get_register_status_req_st  api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_get_register_status_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* Initialsie structure locally */
     icf_get_register_status_req_st *p_icf_get_register_status_req_st = ICF_NULL;
     /* Assign api payload to local structure */
      p_icf_get_register_status_req_st =  (icf_get_register_status_req_st*)p_payload;
      
      ret_val = icf_vldt_icf_line_id_t(p_icf_get_register_status_req_st->line_id,p_vldt_err,0); /* 1 means that this is a configuration api */

    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }/* end if*/
    return (ret_val);
}     


/******************************************************************************
*
* FUNCTION:       icf_vldt_icf_aka_auth_resp_st
*
* DESCRIPTION:    This function is invoked to validate the 
*                 icf_vldt_icf_aka_auth_resp_st api.
*
*****************************************************************************/ 
icf_return_t icf_vldt_icf_aka_auth_resp_st (
                          IN icf_uint8_t *p_payload, 
                          INOUT icf_vldt_err_st *p_vldt_err)    
{

    icf_return_t ret_val = ICF_SUCCESS;
   
    /* Initialsing authentication resp local API structure  */
    icf_aka_auth_resp_st *p_icf_aka_auth_resp_st = ICF_NULL;
  
    /* Assign payload of authentication resp API to local API structure */
    p_icf_aka_auth_resp_st = (icf_aka_auth_resp_st*)p_payload;

	/* sip method name check*/
    /* Only Register is supported for AKA challenge*/
	if (ICF_METHOD_REGISTER != p_icf_aka_auth_resp_st->sip_method_name)
	{
		ret_val = ICF_FAILURE;
		p_vldt_err->ecode = ICF_CAUSE_INVALID_METHOD_TYPE;
		icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)
                                         "sip_method_name:");
	}
    	       
 
    /* Check line id */
    if(ICF_SUCCESS == ret_val)
    {
        ret_val = icf_vldt_icf_line_id_t(p_icf_aka_auth_resp_st->line_id , p_vldt_err,0); 
        /* 1 means that this is a configuration api */
    	if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
        }/* end if*/
    }
          

    /* if line id is correct */ 

    if( ICF_SUCCESS == ret_val)
    {
	
	  /*Check for user address */
        ret_val = icf_vldt_icf_address_st(&p_icf_aka_auth_resp_st->user_address, p_vldt_err);                                     
        if(ICF_FAILURE==ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t * )"user address:" );
        }/*End iF*/
       
    }/* End if*/

	/* Check for value of the result  */
	if(ICF_SUCCESS == ret_val)
     {
         
         ret_val = icf_vldt_boolean(p_icf_aka_auth_resp_st->result, p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t * )"result:");
         }/* End if*/
     }/* End if*/

	
    /*check for auth response presence if result is suceess*/
	if(ICF_SUCCESS == ret_val) 
    {
        if(ICF_SUCCESS == p_icf_aka_auth_resp_st->result)
        {
		    if(p_icf_aka_auth_resp_st->bit_mask & ICF_AKA_AUTH_RESPONSE_PRESENT) 
            {
                ret_val = icf_vldt_icf_string_st(&p_icf_aka_auth_resp_st->auth_response , p_vldt_err);
           		if(ICF_FAILURE == ret_val)
           		{
                	/* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *) "auth_response:");
                }
		    }
		    else    /*if bitmask is not set*/
		    {
                ret_val = ICF_FAILURE;
		        p_vldt_err->ecode = ICF_CAUSE_AUTH_RESP_MISSING;
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"auth_response:");
		    }
	    }/* End if*/
    }/* End if*/


    /*check for error_cause presence if result is failure*/
	if(ICF_SUCCESS == ret_val) 
    {
        if(ICF_FAILURE == p_icf_aka_auth_resp_st->result) 
        {
            if(p_icf_aka_auth_resp_st->bit_mask & ICF_AKA_AUTH_RESP_ERROR_PRESENT)
            {
                /* Do nothing */
            }
            else    /*if bitmask for error_cause is not set*/
            {
                ret_val = ICF_FAILURE;
		        p_vldt_err->ecode = ICF_CAUSE_ERROR_CAUSE_MISSING;
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *) " error_cause:");
		    }
	    }/* End if*/
    }


    /*ckeck for auts token if error cause is SQN failure*/
	if(ICF_SUCCESS == ret_val) 
    {
        /*if error_cause is SQN failure and result is ICF_FAILURE*/
        if((ICF_FAILURE == p_icf_aka_auth_resp_st->result) &&
           (ICF_CAUSE_SQN_OUT_OF_RANGE == p_icf_aka_auth_resp_st->error_cause))
        {
		    if(p_icf_aka_auth_resp_st->bit_mask & ICF_AKA_AUTH_AUTS_PRESENT) 
            {
                ret_val = icf_vldt_icf_string_st(& p_icf_aka_auth_resp_st-> auts_token, p_vldt_err);
           		if(ICF_FAILURE == ret_val)
           		{
                	/* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *) " auts_token:");
                }
            }
		    else     /*if bitmask is not set*/
		    {
                ret_val = ICF_FAILURE;
		        p_vldt_err->ecode = ICF_CAUSE_AUTS_TOKEN_MISSING;
			    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"auts_token:");
		    }
        }/* End if*/
    }/* End if*/


    /*check for IK*/
	if(ICF_SUCCESS == ret_val) 
    {
        if(p_icf_aka_auth_resp_st->bit_mask & ICF_AKA_AUTH_IK_PRESENT) 
        {
            ret_val = icf_vldt_icf_string_st(& p_icf_aka_auth_resp_st->Ik, p_vldt_err);
        	if(ICF_FAILURE == ret_val)
       		{
            	/* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *) "Ik:");
            }
		}
     }/* End if*/



	if(ICF_SUCCESS == ret_val) 
    {
        if (p_icf_aka_auth_resp_st->bit_mask & ICF_AKA_AUTH_CK_PRESENT) 
        {
            ret_val = icf_vldt_icf_string_st(& p_icf_aka_auth_resp_st->Ck, p_vldt_err);
           	if(ICF_FAILURE == ret_val)
           	{
            	/* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *) "Ck:");
            }
		}
     }/* End if*/

    /*Validate the correctness of security_association_reqd*/
   	if(ICF_SUCCESS == ret_val) 
    {
        if (p_icf_aka_auth_resp_st->bit_mask &
                     ICF_AKA_AUTH_SECURITY_ASSOCIATION_REQD_PRESENT) 
        {
            ret_val = icf_vldt_boolean(
                        p_icf_aka_auth_resp_st->security_association_reqd,
                                                                p_vldt_err);
           	if(ICF_FAILURE == ret_val)
           	{
            	/* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                            (icf_uint8_t *) "security_association_reqd:");
            }
		}
     }/* End if*/
    return (ret_val);
	
}/* End of function icf_vldt_icf_aka_auth_resp_st*/





/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_msg_resp_st  
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_msg_resp_st  api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_msg_resp_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* Initialsie structure locally */
     icf_msg_resp_st *p_icf_msg_resp_st = ICF_NULL;
     /* Assign api payload to local structure */
      p_icf_msg_resp_st =  (icf_msg_resp_st*)p_payload;
      
      if(p_icf_msg_resp_st->bit_mask & ICF_MSG_RESP_LINE_ID_PRESENT)
      {          
          ret_val = icf_vldt_icf_line_id_t(p_icf_msg_resp_st->line_id,p_vldt_err,0); /* 1 means that this is a configuration api */

          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
          }/* end if*/
      }
      if(ICF_SUCCESS == ret_val)
      {
          /* Check for result  */
            ret_val=icf_vldt_boolean(
            p_icf_msg_resp_st->result,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"result:");
            }/* End if*/
     
       }    
return (ret_val);
}     


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_rem_call_transfer_initiated_cfm_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_rem_call_transfer_initiated_cfm_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_rem_call_transfer_initiated_cfm_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t  ret_val = ICF_SUCCESS;
     /* Initialsie structure locally */
     icf_remote_call_transfer_initiated_cfm_st *p_icf_remote_call_transfer_initiated_cfm_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_remote_call_transfer_initiated_cfm_st  = (icf_remote_call_transfer_initiated_cfm_st *)p_payload;
     /* Check for value of the result  */
     ret_val = icf_vldt_boolean(p_icf_remote_call_transfer_initiated_cfm_st->result,p_vldt_err);
     if(ICF_FAILURE == ret_val)
     {
          /* Add error string to p_vldt_err structure */
          icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
     }/* End if*/
     if(ICF_SUCCESS == ret_val)
     {    
         /* check for the bitmask for response code */
         if((ICF_FALSE == p_icf_remote_call_transfer_initiated_cfm_st->result) && 
             (p_icf_remote_call_transfer_initiated_cfm_st->bit_mask & ICF_CALL_TRANS_INIT_CFM_CODE_PRESENT))
         {
             /* Check for response code .It's value should be between 400-699*/
             if((p_icf_remote_call_transfer_initiated_cfm_st->response_code < 400) ||
                (p_icf_remote_call_transfer_initiated_cfm_st->response_code >  699))
             {
                 ret_val = ICF_FAILURE; /* assign failure to ret val */
                 /* assign ecode */
                 p_vldt_err->ecode = ICF_CAUSE_INVALID_VALUE_FOR_RESPONSE_CODE;
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"response_code:");
             }/* End if*/
         }/* End if*/

     }/* End if*/
     return(ret_val); 
}   

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_early_session_cfm_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_vldt_icf_early_session_cfm_st api.
*
*****************************************************************************/


icf_return_t icf_vldt_icf_early_session_cfm_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    /* Initialsie structure locally */
    icf_early_session_cfm_st *p_icf_early_session_cfm_st = ICF_NULL;
    /* Assign api payload to local structure */
    p_icf_early_session_cfm_st=(icf_early_session_cfm_st*)p_payload;
    /* Check for value of the result  */
    ret_val= icf_vldt_boolean(p_icf_early_session_cfm_st->result,
             p_vldt_err); 
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
    }/* End if*/

    

    /* Validation for presence of Tunnel Mode */


     if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_early_session_cfm_st->bit_mask &
                       ICF_EARLY_SESSION_CFM_TUNNEL_MODE_PRESENT)

        {
            /* Check for session timer logic  flag  */
            ret_val=icf_vldt_boolean(
            p_icf_early_session_cfm_st->tunnel_mode,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"tunnel_mode:");
            }/* End if*/
        } /*End if */
    } /* End if */

    /* Validation for the valus of response code
       The value of response code must lie between 400 & 699 */
        if(ICF_SUCCESS == ret_val)
        {
         
	    if((ICF_FALSE == p_icf_early_session_cfm_st->result) && 
                (p_icf_early_session_cfm_st->bit_mask & ICF_EARLY_SESSION_CFM_RESPONSE_CODE_PRESENT))
            {
                /* Check: response_code for its maximum and minimum values */
                if((p_icf_early_session_cfm_st->response_code < 400) ||
                     (p_icf_early_session_cfm_st->response_code > 699)) 
                {
                    ret_val = ICF_FAILURE;
                    p_vldt_err->ecode = 
                                    ICF_CAUSE_INVALID_VALUE_FOR_RESPONSE_CODE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"response_code:");
                }/* End if*/
           }/* End if*/

        }/* End if*/
return ret_val;
}

#ifdef ICF_SESSION_TIMER
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_session_timer_toggle_req_st
*
* DESCRIPTION:     This Function validates ICF_SESSION_TIMER_TOGGLE_REQ API 
* RETURN:          icf_return_t 
*
******************************************************************************/
icf_return_t icf_vldt_icf_session_timer_toggle_req_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /* Initialsie structure locally */
    icf_session_timer_toggle_req_st *p_icf_session_timer_toggle_param_req_st = ICF_NULL;

    /* Assign api payload to local structure */
    p_icf_session_timer_toggle_param_req_st = (icf_session_timer_toggle_req_st *)p_payload;

    /* Check: bit_mask for min_se,for its presence */
    if(p_icf_session_timer_toggle_param_req_st->bit_mask & ICF_SESS_TOGG_REQ_MINSE_PRESENT)
    {
        /* Check: min_se should be greater than or equal to 90 */
            
        if(p_icf_session_timer_toggle_param_req_st->min_se < 90)
        {
             ret_val = ICF_FAILURE;
             p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE;
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"min_se:");
        }/* End if*/
    }/* End if*/

    /* check: bit_mask for session_timer,for its presence*/
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_session_timer_toggle_param_req_st->bit_mask &  ICF_SESS_TOGG_REQ_TIMER_PRESENT)
        {
            if(p_icf_session_timer_toggle_param_req_st->bit_mask & 
                         ICF_SESS_TOGG_REQ_MINSE_PRESENT)
            {
                if(p_icf_session_timer_toggle_param_req_st->session_timer < 
                        p_icf_session_timer_toggle_param_req_st->min_se)
                {
                    ret_val = ICF_FAILURE;
                    p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_timer:");
                }
            }
            else
            {
                if(p_icf_session_timer_toggle_param_req_st->session_timer < 90)
                {
                    ret_val = ICF_FAILURE;
                    p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_timer:");
                }
            }            
        }/* End if*/
    }/* End if*/

    /* Check: bit_mask for session_timer_refresher,for its presence */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_session_timer_toggle_param_req_st->bit_mask & 
                 ICF_SESS_TOGG_REQ_REFRESHER_PRESENT)
        {
            /* Check: session_timer_refresher should have a valid value */
            if((p_icf_session_timer_toggle_param_req_st->session_refresher != 
               ICF_SESSION_REFRESHER_NONE) &&
              (p_icf_session_timer_toggle_param_req_st->session_refresher != 
               ICF_SESSION_REFRESHER_UAC) &&
              (p_icf_session_timer_toggle_param_req_st->session_refresher != 
               ICF_SESSION_REFRESHER_UAS ))
            {
                    ret_val = ICF_FAILURE;
                    /* Assign ecode */
                    p_vldt_err->ecode = ICF_CAUSE_VALUE_NOT_DEFINED ;  
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_timer_refresher:");
            }/* End if*/
       }/* End if*/
    }/* End if*/
   return ret_val;
}
#endif
/******************************************************************************
*
* FUNCTION:       icf_vldt_icf_generic_msg_req_st
*
* DESCRIPTION:    This function is invoked  to validate the 
*                 icf_generic_msg_req api.
*
*****************************************************************************/ 
icf_return_t icf_vldt_icf_generic_msg_req_st(
                          IN icf_uint8_t *p_payload, 
                          INOUT icf_vldt_err_st *p_vldt_err)    
{

    icf_return_t ret_val = ICF_SUCCESS;
   
    /* Initialsing register req local API structure  */
    icf_generic_msg_req_st *p_icf_generic_msg_req_st = ICF_NULL;
  
    /* Assign payload of send_generic_msg_req API to local API structure */
    p_icf_generic_msg_req_st = (icf_generic_msg_req_st *)p_payload;       


    /* sip method name check*/
    /* Only Refer and Notify are supported */
    ret_val = icf_vldt_icf_string_st(
            &p_icf_generic_msg_req_st->method_name,p_vldt_err);

    if(ICF_SUCCESS == ret_val)
    {
        if((ICF_NULL != icf_vldt_port_strcasecmp((const icf_int8_t *)p_icf_generic_msg_req_st->method_name.str, (const icf_int8_t *)"REFER")) 
                && 
           (ICF_NULL != icf_vldt_port_strcasecmp((const icf_int8_t *)p_icf_generic_msg_req_st->method_name.str, (const icf_int8_t *)"NOTIFY")))
        {
            ret_val = ICF_FAILURE;
            p_vldt_err->ecode = ICF_CAUSE_INVALID_METHOD_TYPE;
        }
    }

    if(ICF_FAILURE == ret_val)
    {
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"method_name:");
    }

    /* if Method Name is correct */  
    if(ICF_SUCCESS == ret_val)
    {

	    /* bit mask check */
	    if(p_icf_generic_msg_req_st->bit_mask & ICF_HDR_LIST_PRESENT)  
	    {
		    /*Check for header list */
		    ret_val = icf_vldt_icf_header_list_st(
				    &p_icf_generic_msg_req_st->header_list,p_vldt_err);
		    if(ICF_FAILURE == ret_val)
		    {
			    /* Add error string to p_vldt_err structure */
			    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
		    }/* End if*/
	    }/* End if*/

    }/* End if*/
    /* if header list is correct */
    if(ICF_SUCCESS == ret_val)
    {

	    /* bit mask check */
	    if(p_icf_generic_msg_req_st->bit_mask & ICF_TAG_LIST_PRESENT) 
	    {
		    /*Check for mtag list */
		    ret_val = icf_vldt_icf_tag_list_st(
				    &p_icf_generic_msg_req_st->tag_list,p_vldt_err);
		    if(ICF_FAILURE == ret_val)
		    {
			    /* Add error string to p_vldt_err structure */
			    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
		    }/* End if*/

	    }/* End if*/        
    }/* End if*/

    /* if tag list is correct */ 
    if(ICF_SUCCESS == ret_val)
    {  
	    /* bit mask check */
	    if(p_icf_generic_msg_req_st->bit_mask & ICF_BODY_LIST_PRESENT) 
	    { 
		    /*Check for body list */
		    ret_val = icf_vldt_icf_msg_body_list_st(
				    &p_icf_generic_msg_req_st->body_list,p_vldt_err);
		    if(ICF_FAILURE == ret_val)
		    {
			    /* Add error string to p_vldt_err structure */
			    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
		    }/* End if*/
	    }/* End if*/
    }/* End if*/

    return(ret_val);


}
/******************************************************************************
*
* FUNCTION:       icf_vldt_icf_generic_msg_cfm_st
*
* DESCRIPTION:    This function is invoked  to validate the 
*                 icf_generic_msg_cfm api.
*
*****************************************************************************/ 
icf_return_t icf_vldt_icf_generic_msg_cfm_st(
                          IN icf_uint8_t *p_payload, 
                          INOUT icf_vldt_err_st *p_vldt_err)    
{

    icf_return_t ret_val = ICF_SUCCESS;
   
    /* Initialsing register req local API structure  */
    icf_generic_msg_cfm_st *p_icf_generic_msg_cfm_st = ICF_NULL;
  
    /* Assign payload of send_generic_msg_req API to local API structure */
    p_icf_generic_msg_cfm_st = (icf_generic_msg_cfm_st *)p_payload;       


    /* sip method name check*/
    /* Only Refer and Notify are supported */
    ret_val = icf_vldt_icf_string_st(
            &p_icf_generic_msg_cfm_st->method_name,p_vldt_err);

    if(ICF_SUCCESS == ret_val)
    {
        if((ICF_NULL != icf_vldt_port_strcasecmp((const icf_int8_t *)p_icf_generic_msg_cfm_st->method_name.str, (const icf_int8_t *)"REFER")) 
                && 
           (ICF_NULL != icf_vldt_port_strcasecmp((const icf_int8_t *)p_icf_generic_msg_cfm_st->method_name.str, (const icf_int8_t *)"NOTIFY")))
        {
            ret_val = ICF_FAILURE;
            p_vldt_err->ecode = ICF_CAUSE_INVALID_METHOD_TYPE;
        }
    }

    if(ICF_FAILURE == ret_val)
    {
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"method_name:");
    }

    /* if Method Name is correct */  
    if( ICF_SUCCESS == ret_val)
    {
	    /* bit mask check */
	    /* Check for Response Code */
        /* Fix for SPR 19486: (Implementation of RFC 4320)*/
        /* If the response code is 408 or less than 199 reject the API.
         * As per RFC 4320 1XX and 408 response to non invite transaction
         * is not allowed.*/
	    if((p_icf_generic_msg_cfm_st->response_code == 408) ||
                (p_icf_generic_msg_cfm_st->response_code < 200) ||
			    (p_icf_generic_msg_cfm_st->response_code >  699))
	    {
		    ret_val = ICF_FAILURE; /* Assign failure to ret val */
		    p_vldt_err->ecode = ICF_CAUSE_INVALID_VALUE_FOR_RESPONSE_CODE;
		    /* Add error string to p_vldt_err structure */
		    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"response_code:");
	    }
    }/* End if*/

    /* if Response Code is correct */  

    if(ICF_SUCCESS == ret_val)
    {

	    /* bit mask check */
	    if(p_icf_generic_msg_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT)  
	    {
		    /*Check for header list */
		    ret_val = icf_vldt_icf_header_list_st(
				    &p_icf_generic_msg_cfm_st->header_list,p_vldt_err);
		    if(ICF_FAILURE == ret_val)
		    {
			    /* Add error string to p_vldt_err structure */
			    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
		    }/* End if*/
	    }/* End if*/

    }/* End if*/
    /* if header list is correct */
    if(ICF_SUCCESS == ret_val)
    {

	    /* bit mask check */
	    if(p_icf_generic_msg_cfm_st->bit_mask & ICF_TAG_LIST_PRESENT) 
	    {
		    /*Check for mtag list */
		    ret_val = icf_vldt_icf_tag_list_st(
				    &p_icf_generic_msg_cfm_st->tag_list,p_vldt_err);
		    if(ICF_FAILURE == ret_val)
		    {
			    /* Add error string to p_vldt_err structure */
			    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
		    }/* End if*/

	    }/* End if*/        
    }/* End if*/

    /* if tag list is correct */ 
    if(ICF_SUCCESS == ret_val)
    {  
	    /* bit mask check */
	    if(p_icf_generic_msg_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT) 
	    { 
		    /*Check for body list */
		    ret_val = icf_vldt_icf_msg_body_list_st(
				    &p_icf_generic_msg_cfm_st->body_list,p_vldt_err);
		    if(ICF_FAILURE == ret_val)
		    {
			    /* Add error string to p_vldt_err structure */
			    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body_list:");
		    }/* End if*/
	    }/* End if*/
    }/* End if*/

    return(ret_val);


}

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_app_confirmation_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_message_cfm_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_app_confirmation_st(
                          IN icf_uint8_t *p_payload, 
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t  ret_val = ICF_SUCCESS;
     /* Initialsie structure locally */
     icf_app_confirmation_st    *p_icf_app_confirmation_st = ICF_NULL;
     /* Assign api payload to local structure */
     p_icf_app_confirmation_st    = (icf_app_confirmation_st *)p_payload;
     /* Check for value of the result  */
     ret_val = icf_vldt_boolean(p_icf_app_confirmation_st->result,p_vldt_err);
     if(ICF_FAILURE == ret_val)
     {
          /* Add error string to p_vldt_err structure */
          icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
     }/* End if*/

     if(ICF_SUCCESS == ret_val)
     {    
         /* check for the bitmask for response code */      
	 if((ICF_FALSE == p_icf_app_confirmation_st->result) &&
               (p_icf_app_confirmation_st->bit_mask & ICF_APP_CONFIRMATION_RESP_CODE_PRESENT))
         { 
             /* Check for response code .It's value should be between 400-699*/ 
             /* If the response code is 408 reject the API. 408 response to
              * non invite transaction is not allowed.*/
             if((p_icf_app_confirmation_st->response_code == 408) ||
                     (p_icf_app_confirmation_st->response_code < 400) ||  
                     (p_icf_app_confirmation_st->response_code >  699))
             {
                 ret_val = ICF_FAILURE; /* assign failure to ret val */
                 /* assign ecode */
                 p_vldt_err->ecode = ICF_CAUSE_INVALID_VALUE_FOR_RESPONSE_CODE;
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"response_code:");
             }/* End if*/
         }/* End if*/
   
     }/* End if*/
   /* if Response Code is correct */  

    if(ICF_SUCCESS == ret_val)
    {
	    /* bit mask check */
	    if(p_icf_app_confirmation_st->bit_mask & ICF_HDR_LIST_PRESENT)  
	    {
		    /*Check for header list */
		    ret_val = icf_vldt_icf_header_list_st(
				    &p_icf_app_confirmation_st->header_list,p_vldt_err);
		    if(ICF_FAILURE == ret_val)
		    {
			    /* Add error string to p_vldt_err structure */
			    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_list:");
		    }/* End if*/
	    }/* End if*/

    }/* End if*/
    /* if header list is correct */
    if(ICF_SUCCESS == ret_val)
    {
	    /* bit mask check */
	    if(p_icf_app_confirmation_st->bit_mask & ICF_TAG_LIST_PRESENT) 
	    {
		    /*Check for mtag list */
		    ret_val = icf_vldt_icf_tag_list_st(
				    &p_icf_app_confirmation_st->tag_list,p_vldt_err);
		    if(ICF_FAILURE == ret_val)
		    {
			    /* Add error string to p_vldt_err structure */
			    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_list:");
		    }/* End if*/

	    }/* End if*/        
    }/* End if*/
 
     return(ret_val);
}


