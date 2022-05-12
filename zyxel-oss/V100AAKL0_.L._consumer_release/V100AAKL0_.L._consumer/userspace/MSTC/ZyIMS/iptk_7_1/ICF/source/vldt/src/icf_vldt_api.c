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
 * 07-Jan-2008  Amit Sikka         SPR 17945       Support for PUBLISH/MESSAGE
 * 07-Nov-2008  Anurag Khare       SPR 19168       validation for port_to_be_sent_in_from_hdr
 *                                                 in set_call_params API
 *
 * 06-Jan-2009  Vinay Jindal       SPR 19250       validation for added parameter in 
 *                                                 set_service_params API
 * 05-Feb-2009   Rohit Aggarwal  SPR 19316   Changes for INVITE with Replaces
 * 19-Dec-2009  Rohit Aggarwal   SPR 20246  Configuration to strip Route header
 * 29-Mar-2010  Kamal Ashraf                       CSR_1-8389294(Registaration Enh)
 * 09-Apr-2010  Alok Tiwari      SPR 20263         Modified Function:
 *                                                 icf_vldt_icf_options_req_st
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

    /* CSR_1-8389294 changes start */
    if(ICF_SUCCESS == ret_val)
    {  
       /* bit mask check */
       if(p_icf_register_req_st->bit_mask & ICF_REG_REQ_DEREG_SPECIFIC_CONTACT_PRESENT) 
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
    /* CSR_1-8389294 changes end */

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
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *) "body_list:");
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

     }/* End if*/
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
             if((p_icf_notify_cfm_st->response_code < 400) ||  
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

    /*Fix for CSR 1-8223807 : SPR-20362*/
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
                if((p_icf_info_cfm_st->response_code < 400) ||
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
         icf_vldt_port_strcat(p_vldt_err->err_str,"(icf_uint8_t *)reset:");
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
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"proxy_addr:");
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
            /* Check: session_timer should be greater than 90 */
            
            if(p_icf_set_line_param_req_st->session_timer <= 90)
            {
                 ret_val = ICF_FAILURE;
                 p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE; 
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_timer:");
            }/* End if*/
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
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"contact_user_info:");
            }/* End if*/
       }/* End if*/

    }/* End if*/

	/* Fix for CSR 1-8047728: SPR 20246 */
	if ((ICF_SUCCESS == ret_val) && 
        (p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_REMOVE_ROUTE_HDR))
    {
        ret_val = icf_vldt_boolean(
					p_icf_set_line_param_req_st->remove_route_hdr,
					p_vldt_err);
        if (ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(
				p_vldt_err->err_str,
				(icf_uint8_t *)"remove_route_hdr:");
        }
    }

    return(ret_val);
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
            /* Check for value of the icf_simul_call_line_arbitration*/
            ret_val = icf_vldt_boolean(
                      p_icf_app_add_req_st->icf_simul_call_line_arbitration, 
                      p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                (icf_uint8_t *)"icf_simul_call_line_arbitration:");
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
                    /*initialise countto number of deleted media count */
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
                    /*initialise countto number of deleted media count */
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
               icf_vldt_port_strcat(p_vldt_err->err_str, (icf_uint8_t *)"p_conf_reqd_at_transferee:");
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
            /* Check: session_timer should be greater than 90 */

            if(p_icf_set_call_params_req_st->session_timer <= 90)
            {
                 ret_val = ICF_FAILURE;
                 p_vldt_err->ecode =  ICF_CAUSE_INVALID_DURATION_VALUE;
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,
                              (icf_uint8_t *)"session_timer:");
            }/* End if*/
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
            if((p_icf_set_call_params_req_st->update_reqd !=
               ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC) &&
              (p_icf_set_call_params_req_st->update_reqd !=
               ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC) &&
              (p_icf_set_call_params_req_st->update_reqd !=
                ICF_NO_UPDATE ))
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
/*  Fix for SPR 19168 */
   if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_set_call_params_req_st->bit_mask &
                      ICF_SET_PORT_IN_FROM_HDR)
        {

            /* Check for port_to_be_sent_in_from_hdr flag  */
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


	/* Fix for CSR 1-6727350: SPR 19316 */
   	if (ICF_SUCCESS == ret_val)
    {
        if (p_icf_set_call_params_req_st->bit_mask & 
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


