/******************************************************************************
*
* FILE NAME    : icf_vldt_utils.c
*
* DESCRIPTION  : Contains the source code for the utility functions of the 
*                PA validation module
* Revision History :
*
*  DATE            NAME                        REFERENCE       REASON
* ------         ------------                  ---------     ----------
* 22-JUL-2005   Rishi Saxena/Jagmohan chauhan    None          Initial
* 10-Dec-2007   Jagmohan Chauhan                 None        SPR fix 17657 
* 14-JAN-2008	Shruti Thakral								 Changes for Secndry
*															 Server Support in 
*															 Rel 8.0
* 28-Jan-2008   Kamini Gangwani                  Rel 8.0   Added a utility function
*                                                          icf_vldt_icf_media_transport_type_et
* 17-Apr-2008   Tarun Gupta                      SPR 18126 Modified icf_vldt_icf_address_st                                                         
* 29-May-2008   Tarun Gupta                      SPR 18585 Modified icf_vldt_icf_config_address_st
* 06-Jun-2008   Tarun Gupta                      SPR 18585 Modified icf_vldt_icf_codec_attrib_st
* 25-Jun-2008 Abhishek Dhammawat     IPTK 8.0        Added function
*                                              icf_vldt_allow_null_icf_string_st
* 1-Dec-2008  Abhishek Dhammawat     SPR 19218       Merge CSR 1-6214356 
* 09-Dec-2008 Anurag Khare           SPR 19218     CSR 1-6325377 Merged
* 11-Feb-2009 Ashutosh Mohan         IPTK Rel8.1           Added icf_vldt_icf_sdf_address_st & icf_vldt_icf_sdf_string_st
*                                                          icf_vldt_icf_tunnelled_sdp_st 
* 04-Mar-2009 Saurabh Gupta	                     SPR 19349      Klocworks fix
* 13-Mar-2009 Ashutosh Mohan         IPTK Rel8.1     Modified icf_vldt_icf_sdf_address_st & icf_vldt_icf_tunnelled_sdp_st	
* 20-Mar-2009 Ashutosh Mohan         IPTK Rel8.1     Modified
*                                                    icf_vldt_icf_sdf_address_st	
* 04-Apr-2009 Rajiv Kumar            Fix For SPR     Klocwork Warning Fix. 
*                                    19423
* 19-Apr-2009  Abhishek Dhammawat    SPR 19465       modified
*                                                    icf_vldt_icf_tunnelled_sdp_st 
* 20-Apr-2009 Rajiv Kumar            SPR 19482       Fix for CSR-1-7130952
* 21-May-2009 Kamal Ashraf           SPR 19672       CSR_1_7116253 Merged
* 11-Jun-2009 Ashutosh Mohan         SPR 19737       Changes done for GCC 4.1 
*                                                    warnings removal
* 12-Aug-2009 Rajiv Kumar            SPR 19923       Fix for SPR 19923
* 08-Oct-2009 Abhishek Dhammawat     SPR 20063       Merged CSR 1-7118121
* 26-Oct-2009 Pankaj Negi            SPR 20176       Klocwork Warning Fix. 
* 07-May-2010 Aman Aggarwal     SPR 20391       Merged fixes in CSR 1-8561947
* 17-Jun-2010 Aman Aggarwal          SPR 20473       Fix for CSR 1-8648269
*
* Copyright 2010, Aricent.
*
*******************************************************************************/
 
#include "icf_vldt_prototypes.h"
#include "icf_vldt_defs.h"
#include "icf_port_prototypes.h"

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_address_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  transport address structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_address_st(
                  INOUT icf_address_st *p_addr, 
                  INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_address_type_t         addr_type = ICF_ADDRESS_PLAN_UNDEFINED;
    icf_uint8_t *p_addr_data = ICF_NULL;
    
    /* This variable is used to identfiy number of sip uri format */
    icf_uint8_t *p_sipnumber = ICF_NULL;
    /* This variable is used to compare first char iof number as teluri number
     * start with '+' char */
    icf_uint8_t telurl_start = '+';
    /* This variable is used to check presence of @ in string received from PA */
    icf_uint8_t sipuri_num = '@';
    /* This var is used in loops */
    icf_uint8_t counter =0 ;

    /* This varialbe is used to identfiy e164 number */
    icf_boolean_t is_hex_digit = ICF_TRUE;
    /* This varialbe is used to identfiy tel number */
    icf_boolean_t is_separator =  ICF_FALSE;

    /* CSR_1_7116253 Merged SPR 19672 Start */
    icf_uint8_t        temp_addr_str[ICF_MAX_STR_LEN] = "\0",
                       str_count = 0;
    /* CSR_1_7116253 Merged SPR 19672 End */    
    if(ICF_SUCCESS == ret_val) /* if address value is correct */
    {
        /* Checking for the address type */
        /* If application has explicitly specified the address type as undefined,
         * we would infer its type.
         */
        if((ICF_ADDRESS_PLAN_UNDEFINED != p_addr->addr_type) && 
           ((p_addr->addr_type > ICF_MAX_ADDR_TYPE) ||
           (p_addr->addr_type < ICF_MIN_ADDR_TYPE))) /* min-max */
        {
            /* Assign failure if address type is not correct */
            ret_val = ICF_FAILURE;
            /* Assign the ecode */	
            p_vldt_err->ecode = ICF_CAUSE_INVALID_ADDRESS_TYPE;
        }
    }
    if(ICF_SUCCESS == ret_val) 
    {
        /*Checking for null string in address string */
         ret_val = icf_vldt_icf_string_st(
                                   (icf_string_st *)p_addr, p_vldt_err);

            if(ICF_FAILURE == ret_val)
            {
                /*assign error string to vldt structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"addr_val:");
            }
    }
    /* CSR_1_7116253 Merged SPR 19672 Start */
    p_addr_data = (icf_uint8_t *)p_addr->addr_val.str;
    /* CSR_1_7116253 Merged SPR 19672 End */
        
    if(ICF_SUCCESS == ret_val)
    { 
        str_count = 0;

        /* CSR_1_7116253 Merged SPR 19672 Start */
        while(('\0' != *p_addr_data) && ((ICF_MAX_STR_LEN  -1)> str_count))
        {
            if(';' == *p_addr_data)
            {
                temp_addr_str[str_count] = '\0';
                break;
            }
            else
            {
                temp_addr_str[str_count] = *p_addr_data;
            }
            str_count++;
            p_addr_data++;
        }
		temp_addr_str[str_count] = '\0';
  	    p_addr_data = (icf_uint8_t *)temp_addr_str;
	
	/* 
         now p_addr_data has the address before ';' and p_rem has the phone-context 
   	 parameter(if present).If it is not present p_rem will be null */
        /* p_addr_data = (icf_uint8_t *)p_addr->addr_val.str;*/
        /* CSR_1_7116253 Merged SPR 19672 End */
        p_sipnumber = icf_vldt_port_strchr(p_addr_data,sipuri_num);

        /* if @ icf_uint8_t is not found, p_sipnumber will be null and
        * number can be telurl or e164 number */
        if(ICF_NULL == p_sipnumber)
        {
            /* Check if the address starts with a valid hexadecimal digit or '+'
            * If it doesn't it cant be E164 or Tel URI*/

            addr_type = p_addr->addr_type;

            if(p_addr_data[0] == telurl_start || ICF_ISXDIGIT(p_addr_data[0]))
            {
                /* Fix for CSR 1-8648269 SPR 20473.
                 * Change is done such that EGT shall validate for configured
                 * Addr String based on configured Addr Type(for E164 & TEL_URI).
                 * For E164,
                 *  - String must be hexadecimal digits.
                 *  - String must not contain special digits ('*','#') and Visual
                 *   separators ('-','.',')','(').
                 *
                 * For Tel-Uri,
                 *  - String must be hexadecimal digits.
                 *  - String may contains special digits and visual separators.
                 */
                for(counter = 1; counter < str_count; counter++)
                {
                    if(!ICF_ISXDIGIT(p_addr_data[counter])) 
                    {
                        is_hex_digit = ICF_FALSE;
                    }
                    
                    if (ICF_ISSPECIALDIGIT(p_addr_data[counter]) ||
                          ICF_VISUAL_SEPARATOR(p_addr_data[counter]))
                        {
                        is_separator = ICF_TRUE;
                    }

                    if ((p_addr->addr_type == ICF_ADDRESS_PLAN_E164) &&
                        ((ICF_FALSE == is_hex_digit)||(ICF_TRUE == is_separator)))
            {
                          ret_val = ICF_FAILURE;
                          break;
            }
                    else if ((p_addr->addr_type == ICF_ADDRESS_PLAN_TEL_URI) &&
                        (ICF_FALSE == is_hex_digit) && (ICF_FALSE == is_separator))
            {
                          ret_val = ICF_FAILURE;
                          break;
            }

                    is_hex_digit = ICF_TRUE;
                    is_separator = ICF_FALSE;
                }/*End of for loop */

            /* Merged CSR 1-8314017. Fix in CSR 1-8561947 */
            if (ICF_ADDRESS_PLAN_SIP_URI == p_addr->addr_type)
            {
                addr_type = ICF_ADDRESS_PLAN_SIP_URI;
            }
            else if (ICF_ADDRESS_PLAN_SIPS_URI == p_addr->addr_type)
            {
                addr_type = ICF_ADDRESS_PLAN_SIPS_URI;
            }

            if(((ICF_ADDRESS_PLAN_SIP_URI == addr_type)
              ||(ICF_ADDRESS_PLAN_SIPS_URI == addr_type)
              ) && (ICF_FAILURE == icf_vldt_sip_uri_address_string(p_addr_data)))
            {
                ret_val = ICF_FAILURE;

               /*assign error string to vldt structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,
                                               (icf_uint8_t *)"addr_val:");
            }
            }/*End p_addr_data[0] == telurl_start...*/
        } /* End of if*/
        else
        {
            
                if ( ICF_ADDRESS_PLAN_SIPS_URI == p_addr->addr_type)
                {
                    addr_type = ICF_ADDRESS_PLAN_SIPS_URI;
                }
                else
                {
                    addr_type = ICF_ADDRESS_PLAN_SIP_URI;
                }
                /* Fix for CSR 1-7718121 : SPR-20074*/
                if(ICF_FAILURE == icf_vldt_sip_uri_address_string(p_addr_data))
                {
                    ret_val = ICF_FAILURE;
                    /*assign error string to vldt structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                                               (icf_uint8_t *)"addr_val:");
                }
        } /* End else */

         /* if calculated address type is invalid return failure */
         if(ICF_ADDRESS_PLAN_UNDEFINED == addr_type) 
         {
            /* Find the address type
             * if it contain @ and no + in first char then it is SIP URL
             * IF address start with + and no @ then it is TELURL
             * else if it contain alphanumeric values
             * then it is e164 value else return error */
            ret_val = ICF_FAILURE;

        }
        if(ICF_FAILURE == ret_val)
        {
            p_vldt_err->ecode = ICF_CAUSE_INVALID_ADDR_STRING;
        }
    } /* End of ICF_SUCCESS == ret_val */

    /* Handling for undefined address type */
    if ((ICF_FAILURE != ret_val) &&
        (ICF_ADDRESS_PLAN_UNDEFINED == p_addr->addr_type))
    {
        /* Update address type if it is undefined*/
        p_addr->addr_type = addr_type;
    }
 
    /* If the address type determined is not the same as address type
     * passed, set failure.
     */
    if((ICF_FAILURE != ret_val) && 
       (addr_type != p_addr->addr_type))
    {
        ret_val = ICF_FAILURE;
        p_vldt_err->ecode = ICF_CAUSE_INVALID_ADDRESS_TYPE;
    }
    return ret_val;
}        


/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_header_list_st
*
* DESCRIPTION:     This function is invoked to validate header list structure. 
*
******************************************************************************/
icf_return_t icf_vldt_icf_header_list_st(
                      IN icf_header_list_st *p_header, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{

    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint8_t count = 0;
    icf_header_st *p_temp_header = ICF_NULL;

    /* Checck if header list count is equal to zero */
    if(p_header->count == 0)
    {        
        /* Assign failure to ret_val*/
        ret_val = ICF_FAILURE; 
          
        /* Assign the ecode */ 
        p_vldt_err->ecode = ICF_CAUSE_HEADER_LIST_NOT_PRESENT;
    } 
        
    else /* if header list is presnt with count not equal to zero */
    {
        /* Check if number of header list count is equal 
           to the number of header's present 
        */
        p_temp_header = p_header->hdr;
        /* Traverse the header list to count the number 
           of headers present in the list 
        */
        for(count=0; (p_temp_header != ICF_NULL) ; ++count)
        {
             p_temp_header = p_temp_header->next;                       
        }

        /* if number of headers present are not equal 
           to header count,then generate failure 
        */
        if(count != p_header->count)
        {
            ret_val = ICF_FAILURE;
                 
            /* Assign the ecode */
            p_vldt_err->ecode = ICF_CAUSE_HEADER_LIST_COUNT_MISMATCH;
        }

        /* if header list is present with accurate number of header count */
        if(ICF_SUCCESS == ret_val) 
        {
            p_temp_header = p_header->hdr;
            while((p_temp_header) && (ICF_SUCCESS == ret_val))    
            {
                /* Check the accuracy of indivisual header's 
                   present in the header list 
                */
                ret_val = icf_vldt_icf_header_st(p_temp_header ,p_vldt_err);
                p_temp_header = p_temp_header->next;
            }
                
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"hdr:");
            }
 
        }                

    } /* End else */

    return(ret_val);
}
 
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_tag_list_st
*
* DESCRIPTION:     This function is invoked to validate tag list .
*                  We check for null tag list ,and if it is not a 
*                  null list,then we need to check for the accuracy of
*                  tag list count and then we check for the validity of 
*                  each tag in turn.
******************************************************************************/

icf_return_t icf_vldt_icf_tag_list_st(
                      IN icf_tag_list_st *p_tag, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint8_t count = 0;
    icf_tag_st *p_temp_tag = ICF_NULL;

    /* Checck if tag list count is equal to zero. */
    if(p_tag->count == 0)
    {
        /* Assign failure to ret_val*/
        ret_val = ICF_FAILURE;

        /* Assign the ecode */
        p_vldt_err->ecode = ICF_CAUSE_TAG_LIST_NOT_PRESENT;

    }
    else
    {
        p_temp_tag = p_tag->tag;

       /* Traverse the tag list to count the number of tags 
          present in the list 
       */
       for(count=0;(p_temp_tag != ICF_NULL) ; ++count)
       {
          p_temp_tag = p_temp_tag->next ;
       }
       /* if number of tags present are not equal to tag count,
          then generate failure 
       */
       if(count != p_tag->count)
       {
            ret_val = ICF_FAILURE;

            /* Assign the ecode */
            p_vldt_err->ecode = ICF_CAUSE_TAG_LIST_COUNT_MISMATCH;
       }

       /* if tag list is present with accurate number of tag count */
       if(ICF_SUCCESS == ret_val) 
       {
            p_temp_tag = p_tag->tag;
            while((p_temp_tag) && (ICF_SUCCESS == ret_val))
            {
                /* Check the accuracy of indivisual header's 
                   present in the header list 
                */
                ret_val = icf_vldt_icf_tag_st(p_temp_tag ,p_vldt_err);
                p_temp_tag = p_temp_tag->next ; 
            }
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag:");
            }
 
        }
    } /* End else */

    return(ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_msg_body_list_st
*
* DESCRIPTION:     This function is invoked to validate body list .
*                  We check for null body list ,and if it is not a null 
*                  list,then we need to check for the accuracy of
*                  body list count and then we check for the validity 
*                  of each body in turn.
*
******************************************************************************/
icf_return_t icf_vldt_icf_msg_body_list_st(
                      IN icf_msg_body_list_st *p_body, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint8_t count = 0;
    icf_msg_body_st *p_temp_body = ICF_NULL;

    /* Checck if body list count is equal to zero.*/
    if(p_body->count == 0)
    {
        /* Assign failure to ret_val*/
        ret_val = ICF_FAILURE;

        /* Assign the ecode */
        p_vldt_err->ecode = ICF_CAUSE_BODY_LIST_NOT_PRESENT;

    }

    else
    {
  
        p_temp_body = p_body->body;

        /* Traverse the  body  list to count the number of bodies 
           present in the list 
        */
        for(count=0; (p_temp_body != ICF_NULL) ; ++count)
        {
            p_temp_body = p_temp_body->next;
        }

        /*  if number of bodies present are not equal to body count,
            then generate failure 
        */
        if(count != p_body->count)
        {
            ret_val = ICF_FAILURE;
            /* Assign the ecode */
            p_vldt_err->ecode = ICF_CAUSE_BODY_LIST_COUNT_MISMATCH;
        }

        /* if body list is present with accurate number of body  count */
        if(ICF_SUCCESS == ret_val)    
        {
            /* Chck for content type */ 
            ret_val = icf_vldt_icf_string_st(&p_body->content_type,p_vldt_err);
               
            if(ICF_FAILURE == ret_val)
            {
                /*assign error string to vldt structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"content_type:");
            }
       }
       /* if body list is present with accurate number of body  
          count and content type is correct 
       */
       if(ICF_SUCCESS == ret_val) 
       {
           p_temp_body = p_body->body;
           while((p_temp_body) && (ICF_SUCCESS == ret_val))
           {

                /* Check the accuracy of indivisual bodies present in 
                   the body list 
                */
                ret_val = icf_vldt_icf_msg_body_st(p_temp_body ,p_vldt_err);
                p_temp_body = p_temp_body->next;
           }
           if(ICF_FAILURE == ret_val)
           {
               /*assign error string to vldt structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"body:");
           }

       }
   }
   return(ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_header_name_list_st
*
* DESCRIPTION:     This function is invoked to validate header name list.
*                  We check for null header name list ,and if it is not a 
*                  null list,then we need tocheck for the accuracy of 
*                  header name list count and then we check for the 
*                  validity of each header in turn.
******************************************************************************/
 
icf_return_t icf_vldt_icf_header_name_list_st(
                      IN icf_header_name_list_st *p_header_name, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint8_t count = 0;
    icf_header_name_st *p_temp_header_name = ICF_NULL;
    /* Checck if header name list count is greater than  zero */
    if(p_header_name->count >  0)
    {
        p_temp_header_name = p_header_name->hdr;  

        /* Traverse the header name list to count 
           the number of headers present in the list 
        */
        for(count=0; (p_temp_header_name != ICF_NULL); ++count)
        {
            p_temp_header_name = p_temp_header_name->next;
        }

        /* if number of headers present are not equal to 
           header name count,then generate failure 
        */
        if(count != p_header_name->count)
        {
            /* assign failure to ret val */
            ret_val = ICF_FAILURE;  

            /* Assign the ecode */
            p_vldt_err->ecode = ICF_CAUSE_HEADER_NAME_LIST_COUNT_MISMATCH;
        }

        /* if number of header name list count is equal to actual 
           header name present 
        */
        if(ICF_SUCCESS == ret_val) 
        {
            p_temp_header_name = p_header_name->hdr;
            while((p_temp_header_name) && (ICF_SUCCESS == ret_val))
            {
                  
                /* Check the accuracy of indivisual header's present 
                   in the header list 
                */
                ret_val = icf_vldt_icf_header_name_st(
                          p_temp_header_name ,p_vldt_err);
                p_temp_header_name = p_temp_header_name->next;
            }
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_name:");
            }
        }

    }/* End if */
    else if(p_header_name->count == 0)
    {
        ret_val = ICF_SUCCESS;
    }
    else
    {
        ret_val = ICF_FAILURE; /* It is an invalid value less than zero */
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"header_name_list_count:");
    }

    return(ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_string_list_st
*
* DESCRIPTION:     This function is invoked to validate string list .
*                  We check for null string list ,and if it is not a null list,
*                  then we need tocheck for the accuracy of string list count 
*                  and then we check for the validity of each string in turn.
******************************************************************************/
icf_return_t icf_vldt_icf_string_list_st(
                      IN icf_string_list_st *p_string, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{

    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint8_t count = 0;
    icf_string_list_node_st *p_temp_list = ICF_NULL;

    /* Checck if header list count is equal to zero. */
    if(p_string->count == 0)
    {
        /* Assign failure to ret_val*/
        ret_val = ICF_SUCCESS;

    }
    else if(p_string->count > 0)
    {   
        p_temp_list = p_string->p_list;

        /* Traverse the string list to count the number of string's present 
           in the list 
        */
        for(count=0; ((count < p_string->count) && (p_temp_list != ICF_NULL)) ; ++count)
        {
            p_temp_list = p_temp_list->p_next;
        }

        /* if number of headers present are not equal to header count, 
           then generate failure 
        */
        if(count != p_string->count)
        {
            /* assign failure to ret_val */
            ret_val = ICF_FAILURE;

            /* Assign the ecode */
            p_vldt_err->ecode = ICF_CAUSE_STRING_LIST_COUNT_MISMATCH;
        }

        /* if string list is present with correct number of string's */
        if(ICF_SUCCESS == ret_val) 
        {
            p_temp_list = p_string->p_list;
            count = 0;
            while((p_temp_list) && (ICF_SUCCESS == ret_val) && (count < p_string->count))
            {

                /* Check the accuracy of indivisual string's present in 
                   the string list 
                */
                ret_val = icf_vldt_icf_string_list_node_st(
                                     p_temp_list ,p_vldt_err);
                p_temp_list = p_temp_list->p_next;
                count ++;
            }
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_list:");
            }
       }
        
    } /* End else if*/
    else
    {
        ret_val = ICF_FAILURE;
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"string_list_invalid_count:");
    }

    return (ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_ipv6_int_address_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_ipv6_int_address_st structure.
*
******************************************************************************/
icf_return_t icf_vldt_icf_ipv6_int_address_st(
                      IN icf_ipv6_int_address_st *p_ipv6_addr, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{ 

     icf_return_t ret_val = ICF_SUCCESS;
     icf_uint8_t count =0;
     icf_uint8_t i = 0;

     /* Check for validation of IPV6 address */
     ret_val = icf_vldt_icf_string_st(
               (icf_string_st *)p_ipv6_addr, p_vldt_err);
         
     if(ICF_SUCCESS == ret_val)
     {
         while( (i < ICF_MAX_STR_LEN) && ( p_ipv6_addr->str[i] != '\0'))
         {
             if(p_ipv6_addr->str[i] == '.')
             {
                 ++count;
             }
             ++i;
         }
 
         if(count != 5 ) /* if number of dot's are not equa to 5 */
         {
             ret_val = ICF_FAILURE;

             p_vldt_err->ecode = ICF_CAUSE_INVALID_IPV6_ADDR ;
         }
     }
     else
     {
          /* Add error string to p_vldt_err structure */
          icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"str:");
     }

     return (ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_stream_list_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_stream_list_st structure.
******************************************************************************/
icf_return_t icf_vldt_icf_stream_list_st(
                      IN icf_stream_list_st *p_stream, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /* No Check for the stream id */ 
    /* Check for the stream type info */
    ret_val = icf_vldt_icf_stream_type_t(p_stream->stream_type,p_vldt_err);
    if (ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
         icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"stream_type:");
    }
    return (ret_val);
}


/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_stream_type_t
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_stream_type_t structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_stream_type_t(
                      IN icf_media_type_t type, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /* Check for strem media type */
    if((type != ICF_STREAM_TYPE_AUDIO) && (type != ICF_STREAM_TYPE_VIDEO) && (type != ICF_STREAM_TYPE_TBCP) && (type!= ICF_STREAM_TYPE_FAX))
    {
        ret_val = ICF_FAILURE; /* return failure */

        p_vldt_err->ecode = ICF_CAUSE_INVALID_STREAM_TYPE; /* Set ecode */
    }
    
    return (ret_val);
}


/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_call_id_t
*
* DESCRIPTION:     This function is invoked to validate icf_call_id_t structure.
*
*
******************************************************************************/

icf_return_t icf_vldt_icf_call_id_t(
                      IN icf_call_id_t call_id, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    
    icf_return_t ret_val = ICF_SUCCESS;

    /* check for the accuracy of call id .It should be in a valid range */
    if(call_id > ICF_MAX_OTG_CALL_ID_VALUE)
    {

        /* Set ret val to failure */
        ret_val=ICF_FAILURE;

        /* Set ecode */  
        p_vldt_err->ecode = ICF_CAUSE_CALL_ID_OUT_OF_RANGE;
    }

    return (ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_config_action_t
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_config_action_t structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_config_action_t(
                      IN icf_config_action_t c_action, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /*Check for type of action .it has to be in a range from 0 to 2 */
    /* 0 stands for ADD ,1 for MODIFY and 2 for DELETE */
    if((c_action > ICF_MAX_CONFIG_ACTION_VALUE))
    {
        /* Set ret val to failure */
        ret_val=ICF_FAILURE;

        /* Set ecode */
        p_vldt_err->ecode = ICF_CAUSE_INVALID_CONFIG_ACTION;         
    }

    return(ret_val);
}



/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_udp_t
*
* DESCRIPTION:     This function is invoked to validate icf_udp_t structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_udp_st(
                      IN icf_udp_st *p_udp, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS; 

    /* Check for the accuracy of ip address */
    ret_val = icf_vldt_icf_transport_addr_st(&p_udp->ip , p_vldt_err,ICF_FALSE); 
      
    if(ICF_SUCCESS == ret_val) /* if address is correct */
    {
        /* Check for the range of port which can be used */
        ret_val = icf_vldt_port(p_udp->port,p_vldt_err);
        
        if(ICF_FAILURE == ret_val) 
        {
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"port:");      
        }
    }
    else
    {
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"ip:");
    }

    return (ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_config_transport_address_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_config_transport_address_st structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_config_transport_address_st(
                      IN icf_config_transport_address_st *config_trans, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS; 
 
    /* Check for the accuracy of line id */
    ret_val = icf_vldt_icf_line_id_t(config_trans->line_id , p_vldt_err,1); 
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }
    if(ICF_SUCCESS == ret_val) /* if line id is correct */
    {
        /* check for action validity */
        ret_val = icf_vldt_icf_config_action_t(
                  config_trans->action , p_vldt_err) ;
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"action:");
        }
    }

    /* if line id is correct and config action is correct */
    if(ICF_SUCCESS == ret_val)
    {
        ret_val = icf_vldt_icf_transport_addr_reg_st(
                  &config_trans->transport_addr , p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"transport_addr:");
        }
    }

    /* if line id is correct and config action is correct and also 
       transport address is correct 
    */
    if(ICF_SUCCESS == ret_val) 
    {
        /* check for the transport mode parameter */
        ret_val = icf_vldt_icf_transport_mode_t(
                  config_trans->transport_mode , p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"transport_mode:");
        }
    }
    return (ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_audio_profile_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_audio_profile_st structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_audio_profile_st(
                      IN icf_audio_profile_st *p_audio_pro, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{

    icf_return_t ret_val = ICF_SUCCESS;
       
    /* Check for the correctness of pref_time parameter */
    if(p_audio_pro->bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)
    {
        ret_val = icf_vldt_boolean(p_audio_pro->silence_suppression,p_vldt_err);
        if(ICF_FAILURE == ret_val)
        { 
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"silence_suppression:");
        }
    }
    /* Check for the correctness of echo cancellation parameter */
    if((ICF_SUCCESS == ret_val) && (p_audio_pro->bit_mask  & ICF_AUDIO_PROF_ECHO_CANC_PRESENT))
    {
        ret_val = icf_vldt_boolean(p_audio_pro->echo_canc,p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"echo_canc:");
        }
    } 
 

    return(ret_val);
}
  
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_route_info_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_route_info_st structure.
*******************************************************************************/
icf_return_t icf_vldt_icf_route_info_st(
                      IN icf_route_info_st *p_rt_info,
                      INOUT icf_vldt_err_st *p_vldt_err)
{

    icf_return_t ret_val = ICF_SUCCESS; 
    /* Fix for CSR 1-6214356 : SPR 18849 */
	/* Add null check for route_info to handle the scenario when bit_mask for
     * route_info is set but the ptr is null.
     */
	if (ICF_NULL == p_rt_info)
	{
        /* Assign failue to ret_val */
        ret_val = ICF_FAILURE;

        /*Assign Error Code to ecode in p_vldt_err */
        p_vldt_err->ecode = ICF_CAUSE_NULL_STRING;
	} 
    /* Chck for the correctness of type of transport layer 
       i.e whether tcp,udp or message queue 
    */
    /* Fix for CSR 1-7718121 : SPR-20074*/
    else if(p_rt_info->type == ICF_IPC_MSGQ) 
    {
        ret_val = ICF_SUCCESS;
    }
    else if(p_rt_info->type == ICF_IPC_UDP)
    {
        /* check for the validity of ipc type which can be udp,tcp or 
           a message queue 
        */ 
        ret_val = icf_vldt_icf_udp_st(&p_rt_info->ipc.udp,p_vldt_err);
        
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"udp:");
        }
    } /* if(ICF_SUCCESS == ret_val) */
    /* if route info transport layer mode is incorrect */
    else
    {   /* Assign failue to ret_val */
        ret_val = ICF_FAILURE;

        /*Assign Error Code to ecode in p_vldt_err */
        p_vldt_err->ecode = ICF_CAUSE_INCORRECT_TRANSPORT_MODE ;
        
    }

    return (ret_val);
   }

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_amr_params_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_amr_params_st structure.
*
******************************************************************************/
icf_return_t icf_vldt_icf_amr_params_st(
                      IN icf_amr_params_st *p_amr_params, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /*Check for the correctness of  amr mode count */
    if(p_amr_params->amr_mode_count > ICF_MAX_AMR_MODE)
    {

        /* Assign ret_val the failure */ 
        ret_val = ICF_FAILURE;

        /*Assign ecode to indicate failure */ 
        p_vldt_err->ecode = ICF_CAUSE_AMR_MODE_COUNT_OUT_OF_RANGE ;

    }

    if(ICF_SUCCESS == ret_val) /* if mode count is correct */
    {
        /* Check for crc enabled parameter */    
        ret_val = icf_vldt_boolean(p_amr_params->crc_enabled,p_vldt_err);  
        
        if(ICF_FAILURE == ret_val)
        {
            /*Assign ecode to indicate failure */
           icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"crc_enabled:");
        }
    }

    return (ret_val);
}
             



/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_short_string_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_short_string_st structure.
*
******************************************************************************/
icf_return_t icf_vldt_icf_short_string_st(
                      IN icf_short_string_st *p_s_str, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{

    icf_return_t ret_val = ICF_SUCCESS;
    if(ICF_NULL != p_s_str)
    {
        /* string length checking*/

        /* Check: string length should be less then max.string length i.e.
           ICF_MAX_SHORT_STR_LEN and string length should be equal to str_len
        */

         if((p_s_str->str_len > ICF_MAX_SHORT_STR_LEN) || 
            (icf_vldt_port_strlen((icf_uint8_t*)p_s_str->str) != 
             p_s_str->str_len))
         {
             /*Assign failure to ret_val*/
             ret_val = ICF_FAILURE;

             /*Assign Error Code to ecode in p_vldt_err */
             p_vldt_err->ecode = ICF_CAUSE_INVALID_SHORT_STR_LEN;
         }

         /* Check: string length should not be zero*/
         if(ret_val != ICF_FAILURE)
         {
             if(ICF_NULL == p_s_str->str_len)
             {
                 /*Assign failure to ret_val*/
                 ret_val = ICF_FAILURE;

                 /*Assign Error Code to ecode in p_vldt_err */
                 p_vldt_err->ecode = ICF_CAUSE_NULL_SHORT_STRING;
             }
         }
    }
    else
    {
         /*Assign failure to ret_val*/
         ret_val = ICF_FAILURE;

         /*Assign Error Code to ecode in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_NULL_SHORT_STRING;
    }  
    return(ret_val);
}

/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_string_st
*
* DESCRIPTION:     This function is invoked to validate string.
*
**************************************************************************/
icf_return_t icf_vldt_icf_string_st(
             IN icf_string_st *p_str, 
             INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    if(ICF_NULL != p_str)
    {  
        /* 
        Check: string length should be less then max.string length i.e.
              ICF_MAX_STR_LEN and string length should be equal to str_len
        */
        if((p_str->str_len > ICF_MAX_STR_LEN) || 
           (icf_vldt_port_strlen((icf_uint8_t*)p_str->str) != p_str->str_len))
        {
             /*Assign failure to ret_val*/
	     ret_val = ICF_FAILURE;

	     /*Assign Error Code to ecode in p_vldt_err */      
	     p_vldt_err->ecode = ICF_CAUSE_INVALID_STR_LEN;
        }
     
        /* Check: string length should not be zero*/
        if(ret_val != ICF_FAILURE)
        {
             if(ICF_NULL == p_str->str_len)
	     {
      	         /*Assign failure to ret_val*/
                 ret_val = ICF_FAILURE;

                 /*Assign Error Code to ecode in p_vldt_err */
                 p_vldt_err->ecode=ICF_CAUSE_NULL_STRING;
	     }
         }
    }
     else
    {
         /*Assign failure to ret_val*/
         ret_val = ICF_FAILURE;

         /*Assign Error Code to ecode in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_NULL_STRING;
    }

     return(ret_val);
}

/**************************************************************************
*
* FUNCTION:        icf_vldt_allow_null_icf_string_st
*
* DESCRIPTION:     This function is invoked to validate string except 
*                  allowing null value.
* RETURN VALUE:    ICF_SUCCESS or ICF_FAILURE
**************************************************************************/
icf_return_t icf_vldt_allow_null_icf_string_st(
             IN icf_string_st *p_str, 
             INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    if(ICF_NULL != p_str)
    {  
        /*Check: string length should be less then max.
          string length i.e. ICF_MAX_STR_LEN and string length should be
          equal to str_len*/
        if((p_str->str_len > ICF_MAX_STR_LEN) || 
           (icf_vldt_port_strlen((icf_uint8_t*)p_str->str) != p_str->str_len))
        {
             /*Assign failure to ret_val*/
	         ret_val = ICF_FAILURE;

	         /*Assign Error Code to ecode in p_vldt_err */      
	         p_vldt_err->ecode = ICF_CAUSE_INVALID_STR_LEN;
        }
     
    }
     return(ret_val);
}
	 
/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_large_string_st
*
* DESCRIPTION:     This function is invoked to validate large string.
*
**************************************************************************/
icf_return_t icf_vldt_icf_large_string_st(
             IN icf_large_string_st *p_str,
             INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    if(ICF_NULL != p_str)
    {
        /* Check: string length should be less then max.string length i.e.
           ICF_MAX_large_STR_LEN and string length should be equal to str_len
        */
        if((p_str->str_len > ICF_MAX_LARGE_STR_LEN) ||
           (icf_vldt_port_strlen((icf_uint8_t*)p_str->str) != p_str->str_len))
        {
             /*Assign failure to ret_val*/
             ret_val = ICF_FAILURE;

             /*Assign Error Code to ecode in p_vldt_err */
             p_vldt_err->ecode = ICF_CAUSE_INVALID_LARGE_STRING_LEN;
        }
    
        /* Check: string length should not be zero*/
        if(ret_val != ICF_FAILURE)
        {
             if(ICF_NULL == p_str->str_len)
             {
                 /*Assign failure to ret_val*/
                 ret_val = ICF_FAILURE;

                 /*Assign Error Code to ecode in p_vldt_err */
                 p_vldt_err->ecode=ICF_CAUSE_NULL_STRING;
             }
        }
    }
    else
    {
         /*Assign failure to ret_val*/
         ret_val = ICF_FAILURE;

         /*Assign Error Code to ecode in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_NULL_STRING;
    }
 
     return(ret_val);
}

    


 
/*****************************************************************************
*
* FUNCTION:        icf_vldt_icf_line_id_t
*
* DESCRIPTION:     This function is invoked to validate the value of icf_line_id
*                  
*
*****************************************************************************/
icf_return_t icf_vldt_icf_line_id_t(
                        IN icf_line_id_t lid, 
                        INOUT icf_vldt_err_st *p_vldt_err,
                        IN icf_boolean_t is_config_api)
{
    icf_return_t ret_val = ICF_SUCCESS;
    if(is_config_api == 0) /* It is not a configuration API */  
    {
           
       /* Check: icf_line_id_t should not be greater than ICF_MAX_NUM_OF_LINES*/
/* FIX for spr 17944 */
       
      if(ICF_MAX_NUM_OF_LINES <= lid)
      {
         /*Assign Failure to ret_val*/
         ret_val =  ICF_FAILURE;

         /*Assign Error Code to ecode in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_LINE_ID_OUT_OF_RANGE;
     }
   }

   else if(is_config_api == 1) /* It is a configuration API */
   {

/* FIX for spr 17944 */
     if((ICF_MAX_NUM_OF_LINES <= lid) && (ICF_ALL_LINES != lid))
     {
         /*Assign Failure to ret_val*/
	 ret_val =  ICF_FAILURE;

         /*Assign Error Code to ecode in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_LINE_ID_OUT_OF_RANGE;
     }
   }

     return(ret_val);
}


/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_header_st
*
* DESCRIPTION:     This function is invoked to validate icf_header_st.
*                  
*
******************************************************************************/
     
icf_return_t icf_vldt_icf_header_st(
                      IN icf_header_st *p_header, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Check: Header Name should be validated against icf_string_st*/
    ret_val=icf_vldt_icf_string_st(&p_header->hdr_name, p_vldt_err);
    
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"hdr_name:");
    }
    else
    {
        /* Check: Header Value should be validation against icf_large_string_st */
        ret_val = icf_vldt_icf_large_string_st(
                  (icf_large_string_st*)&p_header->hdr_value, p_vldt_err);
        if( ICF_FAILURE == ret_val)
	{
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"hdr_value:");
        }
     }
     return(ret_val);
}


/********************************************************************************
* FUNCTION:        icf_vldt_icf_tag_st
*
* DESCRIPTION:     This function is invoked to validate icf_tag_st.
*                  
*
******************************************************************************/

icf_return_t icf_vldt_icf_tag_st(
                      IN icf_tag_st *p_tag, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val =  ICF_SUCCESS;
    
    /* Check: Tag Type should be from the given values such as 
       ICF_TAG_TYPE_SIP_URI_PARAM, 
       ICF_TAG_TYPE_SIP_URI_HDR_PARAM, ICF_TAG_TYPE_HDR_PARAM (MAX)
    */  
    
    
    if((p_tag->tag_type < ICF_MIN_TAG_TYPE ) || (p_tag->tag_type > ICF_MAX_TAG_TYPE ))
    { 
        /* Assign Failure to ret_val */

        ret_val = ICF_FAILURE;

        /* Assign Error Code to ecode in p_vldt_err */
                   
        p_vldt_err->ecode = ICF_CAUSE_INVALID_TAG_TYPE;
    }
          
    /* Check: Header name should be a validated against icf_string_st */
        
    if(ICF_FAILURE != ret_val)
    {
        ret_val=icf_vldt_icf_string_st(&p_tag->hdr_name, p_vldt_err);

        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"hdr_name:");
        }
        else
        {
            /* Check: Tag Value should be Validated  against icf_string_st */
            ret_val = icf_vldt_icf_string_st(&p_tag->tag_value, p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"tag_value:");
            }

        }
   }
   return (ret_val);
}       

     
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_msg_body_st
*
* DESCRIPTION:This function is invoked to validate icf_msg_body_st.
*
*
******************************************************************************/
      
icf_return_t icf_vldt_icf_msg_body_st(
                      IN icf_msg_body_st *p_message, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;                


     /* Check: content_type should be a valid string */

     ret_val = icf_vldt_icf_string_st(&p_message->content_type,p_vldt_err);

     if(ICF_FAILURE == ret_val)
     {
         /* Add error string to p_vldt_err structure */
         icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"content_type:");
     }

     
     /* Check: msg_body_hdr_list should be a validated against 
        icf_header_list_st 
     */
     if ((ICF_FAILURE != ret_val) && (p_message->msg_body_hdr_list.count > 0))
     {
         ret_val = icf_vldt_icf_header_list_st(
                   (icf_header_list_st *)&p_message->msg_body_hdr_list, 
                                                             p_vldt_err);
         if (ICF_FAILURE == ret_val)
         {
              /* Add error string to p_vldt_err structure */
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"msg_body_hdr_list:");
         }
     }

     /*Check: length should not be equal to 0 */
     if(ICF_FAILURE != ret_val)
     {
       
         if(p_message->length == 0)
         {
             /*Assign failure to ret_val*/
             ret_val=ICF_FAILURE;
                       
             /*Assign Error Code to ecode in p_vldt_err */
             p_vldt_err->ecode = ICF_CAUSE_NULL_MSG_BODY_LEN;
	 }
     /* Merged CSR 1-7951347. Fix for CSR 1-8561947.*/
     /* The code snippet removed which was comparing and validating 
      * the content-length against the actual message body.
     */
     }
     return(ret_val);
} 

    
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_header_name_st
*
* DESCRIPTION:     This function is invoked to validate icf_header_name_st.
*
*
******************************************************************************/
     
icf_return_t icf_vldt_icf_header_name_st(
                      IN icf_header_name_st *p_hdr_name, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* Check: header_name should be a valid String */

     ret_val = icf_vldt_icf_string_st(&p_hdr_name->hdr_name,p_vldt_err);
         
     if(ICF_FAILURE == ret_val)
     {
         /* Add error string to p_vldt_err structure */
         icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"hdr_name:");
     }
        
     return(ret_val);
}
      
      
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_string_list_node_st
*
* DESCRIPTION:     This function is invoked to validate icf_string_list_node_st.
*
*
******************************************************************************/

icf_return_t icf_vldt_icf_string_list_node_st(
                      IN icf_string_list_node_st *p_str_node, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_FAILURE;
    /* Check: str should be a valid String */
    ret_val = icf_vldt_icf_string_st(&p_str_node->str,p_vldt_err);

    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"str:");
    }
        
    return(ret_val);
}
 

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_video_profile_st
*
* DESCRIPTION:     This function is invoked to validate icf_video_profile_st.
*
*
******************************************************************************/
     
icf_return_t icf_vldt_icf_video_profile_st(
                          IN icf_video_profile_st *p_video_profile,
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
        
      
     /* Check: no check on bit_mask */
     /* Check: field encoding_quality is present or not */
     if(p_video_profile->bit_mask & ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT)
     {   
          /* Check: encoding_quality should be with in the range of 0 and 10 */
          if (p_video_profile->encoding_quality > ICF_MAX_ENCODING_QUALITY)
          {
              /*Assign failure to ret_val*/              
              ret_val = ICF_FAILURE;
               
              /*Assign Error Code to ecode in p_vldt_err */
              p_vldt_err->ecode = ICF_CAUSE_ENC_QUAL_OUT_OF_RANGE; 
          }
     }
     /* Check: no check on frame_rate */
     /* Check: no check on bandwidth */
     return(ret_val);
}   
    
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_t38_attrib_param_st
*
* DESCRIPTION:     This function is invoked to validate icf_t38_attrib_param_st.
*
*
******************************************************************************/
     
icf_return_t icf_vldt_icf_t38_attrib_param_st(
                       IN icf_t38_attrib_param_st *p_t38_attrib_param,
                       INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;

     /* Check: bit_mask is not validated */

     /* Check: fax_version should be within the range */
     /* Check: field fax_version is present or not */
     if(p_t38_attrib_param->bit_mask & ICF_FAX_T38_VERSION)
     {
         if (ICF_MAX_FAX_VERSION < p_t38_attrib_param->fax_version)
         {
             /*Assign failure to ret_val*/
             ret_val = ICF_FAILURE;

             /*Assign Error Code to ecode in p_vldt_err */
             p_vldt_err->ecode = ICF_CAUSE_FAX_VER_OUT_OF_RANGE ; 
         }
     }
      
     /* Check: field bit_rate is present or not */
     if((p_t38_attrib_param->bit_mask & 
         ICF_FAX_T38_BITRATE)&&(ICF_FAILURE != ret_val))
     {
   
        /* Check: bit_rate should be within the range and should have the values 
           ICF_T38_2400_BIT_PER_SEC (MIN), ICF_T38_4800_BIT_PER_SEC, 
           ICF_T38_7200_BIT_PER_SEC, ICF_T38_9600_BIT_PER_SEC, 
           ICF_T38_14400_BIT_PER_SEC, ICF_T38_33600_BIT_PER_SEC (MAX) 
        */
        if(( ICF_T38_2400_BIT_PER_SEC != p_t38_attrib_param->bitrate) && 
               (ICF_T38_4800_BIT_PER_SEC != p_t38_attrib_param->bitrate) && 
               (ICF_T38_7200_BIT_PER_SEC != p_t38_attrib_param->bitrate) && 
               (ICF_T38_9600_BIT_PER_SEC !=p_t38_attrib_param->bitrate) && 
               (ICF_T38_14400_BIT_PER_SEC != p_t38_attrib_param->bitrate) && 
               (ICF_T38_33600_BIT_PER_SEC != p_t38_attrib_param->bitrate))
        {
             /*Assign failure to ret_val*/
             ret_val = ICF_FAILURE;

             /*Assign Error Code to ecode in p_vldt_err */
             p_vldt_err->ecode = ICF_CAUSE_INVALID_BITRATE_VALUE;
        }
    }
    /* Check: field rate_management is present or not */
    if((p_t38_attrib_param->bit_mask & ICF_FAX_T38_RATE_MANAGEMENT) &&
       (ICF_FAILURE != ret_val))         
    {

        /* Check: rate_management can have only two valuse 
           ICF_T38_RATE_MANAGEMENT_LOCAL or
           ICF_T38_RATE_MANAGEMENT_TRANSFERRED
        */
        if((ICF_MIN_RATE_MANAGEMENT != p_t38_attrib_param->rate_management) &&
           (ICF_MAX_RATE_MANAGEMENT != p_t38_attrib_param->rate_management))
        {
            /*Assign failure to ret_val*/
            ret_val = ICF_FAILURE;

            /*Assign Error Code to ecode in p_vldt_err */
            p_vldt_err->ecode = ICF_CAUSE_INVALID_RATE_MGMT_VALUE;
        }
    } 
        
    /* Check: udp_buffersize is not validated */
    /* Check: udp_packetsize is not validated */
    
    /* Check: field err_correction_method is present or not */
    if((p_t38_attrib_param->bit_mask & ICF_FAX_T38_ERR_CORRECTION) &&
                    (ICF_FAILURE != ret_val))
    {
    
        /* Check: err_correction_method can have only two valuse 
           ICF_T38_ERROR_CORR_REDUNDANCY or ICF_T38_ERROR_CORR_FEC
        */

        if((ICF_MIN_ERR_CORRECTION_METHOD != 
           p_t38_attrib_param->err_correction_method) && 
          (ICF_MAX_ERR_CORRECTION_METHOD != 
           p_t38_attrib_param->err_correction_method))
        {
             /*Assign failure to ret_val*/
             ret_val = ICF_FAILURE;

             /*Assign Error Code to ecode in p_vldt_err */
             p_vldt_err->ecode = ICF_CAUSE_INVALID_ERROR_CORR_VALUE;
        }
    }
    return(ret_val);
}

     
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_transport_mode_t
*
* DESCRIPTION:     This function is invoked to validate icf_transport_mode_t.
*
*
******************************************************************************/    
icf_return_t icf_vldt_icf_transport_mode_t(
                      IN icf_transport_mode_t tmode, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;

     /* Check: transport_mode can have following values ICF_TRANSPORT_TYPE_UDP, 
        ICF_TRANSPORT_TYPE_TCP, ICF_TRANSPORT_MODE_BOTH, ICF_TRANSPORT_TYPE_TLS
     */
     /*  The check is added if Application wants to do the NAPTR Query for mode
     *   then Application will set the mode as ICF_TRANSPORT_TYPE_INVALID and 
     *   validation should not fail it 
     */        
     if ((ICF_TRANSPORT_TYPE_INVALID != tmode) &&
         (ICF_MAX_TRANSPORT_TYPE < tmode))
     {
         /*Assign failure to ret_val*/
         ret_val = ICF_FAILURE;
  
         /*Assign Error Code to ecode in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_INVALID_TRANSPORT_MODE; 
     }
     return(ret_val);
}  
        
         
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_duration_t
*
* DESCRIPTION:     This function is invoked to validate icf_duration_t.
*
*
******************************************************************************/
 
icf_return_t icf_vldt_icf_duration_t(
                      IN icf_duration_t duration, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
            
 
     /* Check: all icf_duration_st either 0 or in multiples of 1000s */

             
     if ((duration != 0) && (duration % 1000 !=0))
     {
          /*Assign failure to ret_val*/
          ret_val = ICF_FAILURE;
   
          /*Assign Error Code to ecode in p_vldt_err */
          p_vldt_err->ecode = ICF_CAUSE_INVALID_DURATION_VALUE;
      } 
      return(ret_val);
}  


/*******************************************************************************
*
* FUNCTION:        icf_vldt_port
*
* DESCRIPTION:     This function is invoked to validate port numbers.
*
*
******************************************************************************/

icf_return_t icf_vldt_port(
             IN icf_uint16_t port, 
             INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
      

     /* Check: port nos. can have values between ICF_CFG_MIN_PORT_VALUE and 
        ICF_CFG_MAX_PORT_VALUE
     */
 
     /* (ICF_MAX_PORT_VALUE < port) - this check gets validated with 
           limited range of data type */ 
     if (ICF_MIN_PORT_VALUE > port)
     {
          /*Assign failure to ret_val*/
          ret_val = ICF_FAILURE;
   
          /*Assign Error Code to ecode in p_vldt_err */
          p_vldt_err->ecode = ICF_CAUSE_PORT_NUM_OUT_OF_RANGE;
     }
     return(ret_val);
}   
           
/*******************************************************************************
*
* FUNCTION:        icf_vldt_port_reg
*
* DESCRIPTION:     This function is invoked to validate port numbers.In this
*                  case PORT value as 0 is also valid
*
*
******************************************************************************/

icf_return_t icf_vldt_port_reg(
             IN icf_uint16_t port,
             INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;


     /* Check: port nos. can have values between ICF_CFG_MIN_PORT_VALUE and 
        ICF_CFG_MAX_PORT_VALUE
     */

     /* (ICF_MAX_PORT_VALUE < port) - this check gets validated with 
           limited range of data type */
     if (ICF_MIN_PORT_VALUE > port && port != 0)
     {
          /*Assign failure to ret_val*/
          ret_val = ICF_FAILURE;

          /*Assign Error Code to ecode in p_vldt_err */
          p_vldt_err->ecode = ICF_CAUSE_PORT_NUM_OUT_OF_RANGE;
     }
     return(ret_val);
}
 
         
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_transport_addr_st
*
* DESCRIPTION:     This function is invoked to validate icf_transport_addr_st.
*
*
******************************************************************************/
 
icf_return_t icf_vldt_icf_transport_addr_st(
				IN icf_transport_addr_st *p_transport_addr, 
				INOUT icf_vldt_err_st *p_vldt_err,
				IN icf_boolean_t self_ip_null)
{
		icf_return_t ret_val = ICF_SUCCESS;
        icf_uint8_t  *p_domain_addr  = ICF_NULL;
		/* Check: addr_type can have following values only 
		   ICF_TRANSPORT_ADDRESS_DOMAIN_NAME, 
		   ICF_TRANSPORT_ADDRESS_IPV4_ADDR, ICF_TRANSPORT_ADDRESS_IPV6_ADDR
		 */
		if(( ICF_MIN_TRANSPORT_ADDRESS > p_transport_addr->addr_type) ||(ICF_MAX_TRANSPORT_ADDRESS < p_transport_addr->addr_type ))
		{
				/*Assign failure to ret_val*/
				ret_val = ICF_FAILURE;

				/*Assign Error Code to ecode in p_vldt_err */
				p_vldt_err->ecode = ICF_CAUSE_INVALID_ADDR_TYPE;
		}

		/* Check: for domain name */
		if ((ICF_FAILURE != ret_val) && 
						(p_transport_addr->addr_type == ICF_TRANSPORT_ADDRESS_DOMAIN_NAME))
		{
            /* Fix for CSR 1-7718121 : SPR-20074*/
            p_domain_addr = (icf_uint8_t*)p_transport_addr->addr.domain.str;

            if((ICF_NULL != icf_vldt_port_strchr(p_domain_addr, ':'))||
               (ICF_NULL != icf_vldt_port_strchr(p_domain_addr, '@')))
            {
                 ret_val = ICF_FAILURE;
				 p_vldt_err->ecode = ICF_CAUSE_INVALID_ADDR_STRING;
            }
            else
            {
				ret_val = icf_vldt_icf_string_st(
								(icf_string_st*)&p_transport_addr->addr.domain, p_vldt_err);
            }    

            /* Merged CSR 1-8303146. Fix for CSR 1-8561947.*/
            if ( 0 == strcmp((icf_uint8_t *)"0.0.0.0",(icf_uint8_t *) p_domain_addr))
            {
                ret_val = ICF_FAILURE;
            }

			if(ICF_FAILURE == ret_val)
			{
				/* Add error string to p_vldt_err structure */
				icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"domain:");
			}
		}
		/*This check is to handle self_ip_addr as 0.0.0.0 so that 
		 *Application can request IPTK to close all listening network
		 *sockets.
		 */
		if (ICF_FALSE == self_ip_null)
		{
				/*Check: IPV4 address should not be like 0.0.0.0*/
				if ((ICF_FAILURE != ret_val) &&
								(p_transport_addr->addr_type == ICF_TRANSPORT_ADDRESS_IPV4_ADDR))
				{
						ret_val =icf_vldt_icf_ipv4_int_address_st(
										(icf_ipv4_int_address_st *)&p_transport_addr->addr.ipv4_addr,
										p_vldt_err);
						if(ICF_FAILURE == ret_val )
						{
								/* Add error string to p_vldt_err structure */
								icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"ipv4_addr:");
						}
				}
		}

		/* Check: for IPV6 address */

		if ((ICF_FAILURE != ret_val) &&
						(p_transport_addr->addr_type == ICF_TRANSPORT_ADDRESS_IPV6_ADDR))
		{
				ret_val = icf_vldt_icf_ipv6_int_address_st(
								(icf_ipv6_int_address_st *)&p_transport_addr->addr.ipv6_addr,
								p_vldt_err);
				if(ICF_FAILURE == ret_val)
				{
						/* Add error string to p_vldt_err structure */
						icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"ipv6_addr:");
				}

		}

		return(ret_val);
}   
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_ipv4_int_address_st
*
* DESCRIPTION:     This function is invoked to validate icf_ipv4_int_address_st.
*
*
******************************************************************************/

icf_return_t icf_vldt_icf_ipv4_int_address_st(
                          IN icf_ipv4_int_address_st *p_ipv4_addr,
                          INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;

     /* Check: IPV4 address should not be like 0.0.0.0 */
     if((p_ipv4_addr->octet_1 == 0) && (p_ipv4_addr->octet_2 == 0 ) &&
       (p_ipv4_addr->octet_3 == 0) && (p_ipv4_addr->octet_4 == 0))
     {
         ret_val = ICF_FAILURE;
         p_vldt_err->ecode= ICF_CAUSE_INVALID_IPV4_ADDR;
     }
     return(ret_val);
}

    


/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_stream_capab_st
*
* DESCRIPTION:     This function is invoked to validate icf_stream_capab_st.
*
*
******************************************************************************/
 
icf_return_t icf_vldt_icf_stream_capab_st(
                      IN icf_stream_capab_st *p_stream_capab, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* initailize a temporary node of icf_list_st */
     icf_list_st *p_temp_node;
   
     /* Check: no check is for bit_mask */

     /* Check: no check for stream_id */

     /* Check: Stream type can have only following values
              ICF_STREAM_TYPE_AUDIO,
             ICF_STREAM_TYPE_VIDEO, ICF_STREAM_TYPE_TBCP,
             ICF_STREAM_TYPE_FAX
     */

     if(ICF_FAILURE != ret_val)
     {
          ret_val = icf_vldt_icf_stream_type_t(
                    p_stream_capab->stream_type, p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"stream_type:");
          }
     }
      
     /* Check: p_codec_info_list is of type icf_list_st and
             each node of this is of type icf_codec_attrib_st
     */
     p_temp_node = p_stream_capab->p_codec_info_list;
     if( ICF_FAILURE != ret_val)
     {
          while(p_temp_node != ICF_NULL)
          {
               /* Check: p_data is of type icf_codec_attrib_st */
               ret_val = icf_vldt_icf_codec_attrib_st(
                         p_temp_node->p_data,
                         p_stream_capab->stream_type, p_vldt_err);
                     
               if(ICF_FAILURE == ret_val)
               {
                   /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_codec_info_list:");
                                                              
                   break;
               }
               p_temp_node = p_temp_node->p_next;
          }
     }
     
     /* Check: media_stream_param should be of type
            icf_media_stream_param_ut
     */
     
    /*Fix Merged for CSR 1-6325377*/    
     if((ICF_FAILURE != ret_val)&&
        (p_stream_capab->bit_mask &
            ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT)&&
        ((p_stream_capab->stream_type==ICF_STREAM_TYPE_AUDIO)|| 
         (p_stream_capab->stream_type==ICF_STREAM_TYPE_VIDEO)))
     {
                 
        /* if stream type is stream_type is AUDIO then validate audio_profile 
        else validate video_profile
        */
        if(ICF_STREAM_TYPE_AUDIO == p_stream_capab->stream_type)
        {
            /* Check: audio_profile should be a valid icf_audio_profile_st */

            ret_val =icf_vldt_icf_audio_profile_st(
                  &p_stream_capab->media_stream_param.audio_profile,p_vldt_err);
       
            if( ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"audio_profile:");;

            }
        }

        if(ICF_STREAM_TYPE_VIDEO == p_stream_capab->stream_type)
        {
             /* Check: video_profile should be a valid icf_video_profile_st */             
             ret_val = icf_vldt_icf_video_profile_st(
                   &p_stream_capab->media_stream_param.video_profile, p_vldt_err);

             if( ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"video_profile:");
            }
        }

    }       

             
     /* Check : no check is required for p_encoded_attributes */

     /* Check: qos_mark should be of boolean type, can have values 
        ICF_TRUE or ICF_FALSE 
     */
     if(ICF_FAILURE != ret_val)
     {
         ret_val = icf_vldt_boolean(p_stream_capab->qos_mark,p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"qos_mark:");
         }
     } 

     return(ret_val);
}    
   
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_codec_attrib_st
*
* DESCRIPTION:     This function is invoked to validate icf_codec_attrib_st.
*
*
******************************************************************************/
         
icf_return_t icf_vldt_icf_codec_attrib_st(
                    IN icf_codec_attrib_st *p_codec_attrib,
                    INOUT icf_media_type_t media_type,
                    INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* initialize a temp node of type icf_list_st */
     icf_list_st *p_temp_node=ICF_NULL;
        
     /* Check: no check is for bit_mask */

     /* Check: field codec_num is present or not */
     /* Merged CSR 1-7973336. Fix in CSR 1-8561947*/
     /* If the media type is T.38 Fax stream, don't validate the
      * codec number.*/
     if((media_type != ICF_STREAM_TYPE_FAX) &
         (p_codec_attrib->bit_mask &
            ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT))
     {
         /* Check: codec_num should be in the range of 0 to 127 */
     
         if((p_codec_attrib-> codec_num > 127 ))
         {
             ret_val = ICF_FAILURE;
             p_vldt_err->ecode= ICF_CAUSE_INVALID_CODEC_NUM; 
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str, (icf_uint8_t *)"codec_num:");

         }
         else
         {
             /* Check: field codec_name should be validated against 
                icf_short_string_st */
             ret_val=icf_vldt_icf_short_string_st(
                     &p_codec_attrib->codec_name,p_vldt_err);
             if(ICF_FAILURE==ret_val)
             {
                 /* The following check has been added for CSR 1-5945728.
                  * If the codec name is not present, we will return failure
                  * only in case of dynamic codecs.For static codecs, codec name
                  * is not essential and hence we will proceed.
                  */
                 if((ICF_CAUSE_NULL_SHORT_STRING == p_vldt_err->ecode) &&
                     (p_codec_attrib->codec_num >= 96) &&
                     (p_codec_attrib->codec_num <= 127))
                 {
                     /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str, (icf_uint8_t *)"codec_name:");
                 }
                 else
                 {
                     /* Codec name is not present for static codecs.Set success
                      * and proceed.
                      */
                     ret_val = ICF_SUCCESS;
                 }
             }
         }
     }
     /* Check: codec_param is present or not */ 
     if((ICF_FAILURE != ret_val)&&
       (p_codec_attrib->bit_mask & ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT))
     {
          /* Check: codec_params should be of type icf_codec_params_ut */
          
         /* Check for the correctness of audio codec parameters */
          if(media_type == ICF_STREAM_TYPE_AUDIO)
          {
 
              ret_val = icf_vldt_icf_audio_codec_param_st(
              &p_codec_attrib->codec_params.audio_param,p_vldt_err);

              if(ICF_FAILURE == ret_val)
              {
                   /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"audio_param:");
               } /* End of if */
        
           } /* End of  if(media_type == ICF_STREAM_TYPE_AUDIO) */

          /* no check for video_codec_param_st types */
 
           if(media_type == ICF_STREAM_TYPE_FAX) /* if video codec parameters are correct */
           {

               /* Check for the correctness of t38 used in fax parameters */
               ret_val = icf_vldt_icf_t38_attrib_param_st(
                     &p_codec_attrib->codec_params.t38_param,p_vldt_err);
               if(ICF_FAILURE == ret_val)
               {
                   /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"t38_param:");
               }
           } /* End of  if(media_type == ICF_STREAM_TYPE_FAX) */
    } /* End of if((ICF_FAILURE != ret_val)&&
             (p_codec_attrib->bit_mask & ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT)) */
 
    if((ICF_FAILURE !=ret_val) &&
        (p_codec_attrib->bit_mask & ICF_CODEC_ENC_ATTRIB_PRESENT))
    {
         /* Check: p_encoded_attributes is of type icf_list_st and ;

            each node of this is of type icf_string_st 
         */
         p_temp_node = p_codec_attrib->p_encoded_attributes;
         while(p_temp_node != ICF_NULL)
         {
              /* Check: p_data is of type icf_codec_attrib_st */
              ret_val = icf_vldt_icf_string_st
                        (p_temp_node->p_data, 
                        p_vldt_err);
              if(ICF_FAILURE == ret_val)
              {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_data:");
                 break;
              }
              p_temp_node=p_temp_node->p_next;
          }
    }
 
    return (ret_val);
}   


/*******************************************************************************
*
* FUNCTION:       icf_vldt_icf_audio_codec_param_st
*
* DESCRIPTION:    This function is invoked to validate icf_audio_codec_param_st.
*
*
******************************************************************************/

icf_return_t icf_vldt_icf_audio_codec_param_st(
                    IN  icf_audio_codec_param_st *p_audio_codec_param, 
                    INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;


    /* Check: no check is for bit_mask */
    /* Check: no check is for channels */
    /* Check: no check is for sampling_rate */

    /* Check: amr_params should be of type icf_amr_params_st and 
              amr_params is present
    */
    if(p_audio_codec_param->bit_mask & ICF_AUDIO_CODEC_PARAM_AMR_PARAM)
    {
        ret_val = icf_vldt_icf_amr_params_st(&p_audio_codec_param->amr_params, p_vldt_err);
        if( ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"amr_params:");
        }
    }
    if(ICF_SUCCESS == ret_val)
    {
        if(p_audio_codec_param->bit_mask & ICF_AUDIO_CODEC_PARAM_VBD_VAL)
        {
            ret_val = icf_vldt_boolean(p_audio_codec_param->vbd_val, p_vldt_err);
            if( ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"vbd_val:");
            }
        }
    }
   
    return(ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_config_proxy_transport_address_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_config_transport_address_st structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_config_proxy_transport_address_st(
                      IN icf_config_proxy_transport_address_st *config_proxy_trans, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS; 
 
    /* Check for the accuracy of line id */
    ret_val = icf_vldt_icf_line_id_t(config_proxy_trans->line_id , p_vldt_err,1); 
    if(ICF_FAILURE== ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }
    if(ICF_SUCCESS == ret_val) /* if line id is correct */
    {
        /* check for action validity */
        ret_val = icf_vldt_icf_config_action_t(
                  config_proxy_trans->action , p_vldt_err) ;
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"action:");
        }
    }

    /* if line id is correct and config action is correct */
    if(ICF_SUCCESS == ret_val)
    {
        ret_val = icf_vldt_icf_transport_addr_reg_st(
                  &config_proxy_trans->transport_addr , p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"transport_addr:");
        }
    }

    /* if line id is correct and config action is correct and also 
       transport address is correct 
    */
    if(ICF_SUCCESS == ret_val) 
    {
        /* check for the transport mode parameter */
        ret_val = icf_vldt_icf_transport_mode_t(
                  config_proxy_trans->transport_mode , p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"transport_mode:");
        }
    }
    return (ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_config_sec_transport_address_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_config_sec_transport_address_st structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_config_sec_transport_address_st(
	IN 		icf_config_sec_transport_address_st 	*config_sec_trans, 
    INOUT 	icf_vldt_err_st 						*p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS; 
 
    /* Check for the accuracy of line id */
    ret_val = icf_vldt_icf_line_id_t(config_sec_trans->line_id , p_vldt_err,1); 
    if(ICF_FAILURE== ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id:");
    }
    if(ICF_SUCCESS == ret_val) /* if line id is correct */
    {
        /* check for action validity */
        ret_val = icf_vldt_icf_config_action_t(
                  config_sec_trans->action , p_vldt_err) ;
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"action:");
        }
    }

    /* if line id is correct and config action is correct */
    if(ICF_SUCCESS == ret_val)
    {
        ret_val = icf_vldt_icf_transport_addr_reg_st(
                  &config_sec_trans->transport_addr , p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"transport_addr:");
        }
    }

    /* if line id is correct and config action is correct and also 
       transport address is correct 
    */
    return (ret_val);
}

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_call_modify_media_data_st
*
* DESCRIPTION:     This function is invoked to validate icf_call_modify_media_data_st.
*
*
******************************************************************************/
 
icf_return_t icf_vldt_icf_call_modify_media_data_st(
                      IN icf_call_modify_media_data_st *p_call_modify_media_data_st, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* initailize a temporary node of icf_list_st */
     icf_list_st *p_temp_node;
   
     /* Check: no check is for bit_mask */

     /* Check: no check for stream_id */

     /* Check: Stream type can have only following values
              ICF_STREAM_TYPE_AUDIO,
             ICF_STREAM_TYPE_VIDEO, ICF_STREAM_TYPE_TBCP,
             ICF_STREAM_TYPE_FAX
     */

     if(ICF_FAILURE != ret_val)
     {
          ret_val = icf_vldt_icf_stream_type_t(
                    p_call_modify_media_data_st->stream_type, p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
              /* Add error string to p_vldt_err structure */
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"stream_type:");
          }
     }
      
     /* Check: p_codec_info_list is of type icf_list_st and
             each node of this is of type icf_codec_attrib_st
     */
     p_temp_node = p_call_modify_media_data_st->p_codec_info_list;
     if( ICF_FAILURE != ret_val)
     {
          while(p_temp_node != ICF_NULL)
          {
               /* Check: p_data is of type icf_codec_attrib_st */
               ret_val = icf_vldt_icf_codec_attrib_st(
                         p_temp_node->p_data,
                         p_call_modify_media_data_st->stream_type, p_vldt_err);
                     
               if(ICF_FAILURE == ret_val)
               {
                   /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_codec_info_list:");
                                                              
                   break;
               }
               p_temp_node = p_temp_node->p_next;
          }
     }
     
     /* Check: media_stream_param should be of type
            icf_media_stream_param_ut
     */

     /*Fix Merge for CSR 1-6325377*/   

     if((ICF_FAILURE != ret_val)&&
        (p_call_modify_media_data_st->bit_mask & 
            ICF_CALL_MOD_MEDIA_STREAM_PARAM_PRESENT)&& 
        ((p_call_modify_media_data_st->stream_type==ICF_STREAM_TYPE_AUDIO)|| 
         (p_call_modify_media_data_st->stream_type==ICF_STREAM_TYPE_VIDEO)))
     {
                 
        /* if stream type is stream_type is AUDIO then validate audio_profile 
        else validate video_profile
        */
        if(ICF_STREAM_TYPE_AUDIO == p_call_modify_media_data_st->stream_type)
        {
            /* Check: audio_profile should be a valid icf_audio_profile_st */

            ret_val =icf_vldt_icf_audio_profile_st(
                  &p_call_modify_media_data_st->media_stream_param.audio_profile,p_vldt_err);
       
            if( ICF_FAILURE == ret_val)
            {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"audio_profile:");;

            }
        }

        if(ICF_STREAM_TYPE_VIDEO == p_call_modify_media_data_st->stream_type)
        {
             /* Check: video_profile should be a valid icf_video_profile_st */             
             ret_val = icf_vldt_icf_video_profile_st(
                   &p_call_modify_media_data_st->media_stream_param.video_profile, p_vldt_err);

             if( ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"video_profile:");
            }
        }

    }       

             
     /* Check : no check is required for p_encoded_attributes */

     if((ICF_FAILURE != ret_val) && (p_call_modify_media_data_st->bit_mask & ICF_CALL_MOD_MEDIA_MODE_PRESENT))
     {
         ret_val = icf_vldt_icf_media_mode_t(p_call_modify_media_data_st->media_mode,p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"media_mode:");
         }
     } 

     return(ret_val);
}    

/*******************************************************************************
*   
* FUNCTION:        icf_vldt_icf_sip_duration_t 
*   
* DESCRIPTION:     This function is invoked to validate timer values.
*   
*   
******************************************************************************/
 
icf_return_t icf_vldt_icf_sip_duration_t(
                      IN icf_duration_t duration, 
                      INOUT icf_vldt_err_st *p_vldt_err)
{   
     icf_return_t ret_val = ICF_SUCCESS;
            
    
     /* Check: all icf_duration_st either 0 or in multiples of 1000s */
    
             
     if (duration == 0)
     {
          /*Assign failure to ret_val*/
          ret_val = ICF_FAILURE;
    
          /*Assign Error Code to ecode in p_vldt_err */
          p_vldt_err->ecode = ICF_CAUSE_INVALID_DURATION_VALUE;
      } 
      return(ret_val);
}       

/*******************************************************************************
*   
* FUNCTION:        icf_vldt_icf_subs_head_duration_t 
*   
* DESCRIPTION:     This function is invoked to validate subscription head start                    timer values.
*   
*   
******************************************************************************/

icf_return_t icf_vldt_icf_subs_head_duration_t(
                      IN icf_duration_t duration,
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;


     /* Check: The duration timer should not be zero */


     if (duration == 0)
     {
          /*Assign failure to ret_val*/
          ret_val = ICF_FAILURE;

          /*Assign Error Code to ecode in p_vldt_err */
          p_vldt_err->ecode = ICF_CAUSE_INVALID_DURATION_VALUE;
      }
      return(ret_val);
}
  

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_config_address_st
*
* DESCRIPTION:     This function is invoked to validate 
*                   config address structure.
*
******************************************************************************/
             

icf_return_t icf_vldt_icf_config_address_st(
                  INOUT icf_config_address_st *p_config_addr,
                  INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    ret_val = icf_vldt_icf_line_id_t(p_config_addr->line_id ,p_vldt_err,1);
    if(ICF_FAILURE == ret_val)
    {
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"line_id");
    }
    else
    {
        ret_val = icf_vldt_icf_address_st(&p_config_addr->address,p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"address");
        }   
    }
    return ret_val;
}
          
          
/* A new function for SPR 17657 .Specific check for self_address in set_self_id */
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_address_with_port_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  transport address structure for set_self_id .
*
******************************************************************************/

icf_return_t icf_vldt_icf_address_with_port_st(
                  IN icf_address_st *p_addr, 
                  INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_address_type_t         addr_type = ICF_ADDRESS_PLAN_UNDEFINED;
    icf_uint8_t *p_addr_data = ICF_NULL;
    
    /* This variable is used to identfiy number of sip uri format */
    icf_uint8_t *p_sipnumber = ICF_NULL;
    /* This variable is used to compare first char iof number as teluri number
     * start with '+' char */
    icf_uint8_t telurl_start = '+';
    /* This variable is used to check presence of @ in string received from PA */
    icf_uint8_t sipuri_num = '@';
    /* This var is used in loops */
    icf_uint8_t counter =0 ;
    /* This varialbe is used to identify for hexadecimal digit.*/
    icf_boolean_t is_hex_digit = ICF_TRUE;
    /* This varialbe is used to identfiy any visual or special character
    * present in the address string.
    */
    icf_boolean_t is_separator =  ICF_FALSE;
    /* This variable is used to check presence of : in string received from PA */
    icf_uint8_t port = ':';
    /*This variable used to identify whether port is present*/
    icf_uint8_t *p_portpresent = ICF_NULL;
        
    if(ICF_SUCCESS == ret_val) /* if address value is correct */
    {
        /* Checking for the address type */
        if((p_addr->addr_type > ICF_MAX_ADDR_TYPE) ||
           (p_addr->addr_type < ICF_MIN_ADDR_TYPE)) /* min-max */
        {
            /* Assign failure if address type is not correct */
            ret_val = ICF_FAILURE;
            /* Assign the ecode */	
            p_vldt_err->ecode = ICF_CAUSE_INVALID_ADDRESS_TYPE;
        }
    }
    if(ICF_SUCCESS == ret_val) 
    {
        /*Checking for null string in address string */
         ret_val = icf_vldt_icf_string_st(
                                   (icf_string_st *)p_addr, p_vldt_err);

            if(ICF_FAILURE == ret_val)
            {
                /*assign error string to vldt structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"addr_val:");
            }
    }

            
    if(ICF_SUCCESS == ret_val)
    { 
    
        p_addr_data = (icf_uint8_t *)p_addr->addr_val.str;
        p_sipnumber = icf_vldt_port_strchr(p_addr_data,sipuri_num);

        /* if @ icf_uint8_t is not found, p_sipnumber will be null and
        * number can be telurl or e164 number */
        if(ICF_NULL == p_sipnumber)
        {
            /* Check if the address starts with a valid hexadecimal digit or '+'
            * If it doesn't it cant be E164 or Tel URI*/

            addr_type = p_addr->addr_type;

            if(p_addr_data[0] == telurl_start || ICF_ISXDIGIT(p_addr_data[0]))
            {
                 /* Fix for CSR 1-8648269 SPR 20473.
                 * Change is done such that EGT shall validate for configured
                 * Addr String based on configured Addr Type(for E164 & TEL_URI).
                 * For E164,
                 *  - String must be hexadecimal digits.
                 *  - String must not contain special digits ( *, #) and Visual
                 *   separators ('-','.',')','(').
                 * For Tel-Uri,
                 *  - String must be hexadecimal digits.
                 *  - String may contains special digits and visual separators.
                 */
                
                for(counter = 1; counter < icf_vldt_port_strlen(p_addr_data); counter++)
                {
                    if(!ICF_ISXDIGIT(p_addr_data[counter])) 
                    {
                        is_hex_digit = ICF_FALSE;
                    }

                    if (ICF_ISSPECIALDIGIT(p_addr_data[counter]) ||
                          ICF_VISUAL_SEPARATOR(p_addr_data[counter]))
                        {
                        is_separator = ICF_TRUE;
                    }

                    if ((p_addr->addr_type == ICF_ADDRESS_PLAN_E164) &&
                        ((ICF_FALSE == is_hex_digit)||(ICF_TRUE == is_separator)))
            {
                          ret_val = ICF_FAILURE;
                          break;
            }
                    else if ((p_addr->addr_type == ICF_ADDRESS_PLAN_TEL_URI) &&
                         (ICF_FALSE == is_hex_digit) && (ICF_FALSE == is_separator))
            {
                          ret_val = ICF_FAILURE;
                          break;
            }

                    is_hex_digit = ICF_TRUE;
                    is_separator = ICF_FALSE;

                }/*End of for loop */
            }/*End p_addr_data[0] == telurl_start...*/
        } /* End of if*/
        else
        {
            /* THis can be sip uri, if it does not start with +" */
            /* Check for the presence of port.If present then it is invalid address */
      	    p_portpresent = icf_vldt_port_strchr(p_addr_data , port);
            
    	    if(ICF_NULL == p_portpresent)
            {
                if ( ICF_ADDRESS_PLAN_SIPS_URI == p_addr->addr_type)
                {
                    addr_type = ICF_ADDRESS_PLAN_SIPS_URI;
                }
                else
                {
                    addr_type = ICF_ADDRESS_PLAN_SIP_URI;
                }

                /* Merged CSR 1-8314017. Fix in CSR 1-8561947.*/
                if(ICF_FAILURE == icf_vldt_sip_uri_address_string(p_addr_data))
                {
                    ret_val = ICF_FAILURE;
                    /*assign error string to vldt structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,
                                               (icf_uint8_t *)"addr_val:");
                }
            }

             
        } /* End else */

         /* if calculated address type is invalid return failure */
         if(ICF_ADDRESS_PLAN_UNDEFINED == addr_type) 
         {
            /* Find the address type
             * if it contain @ and no + in first char then it is SIP URL
             * IF address start with + and no @ then it is TELURL
             * else if it contain alphanumeric values
             * then it is e164 value else return error */
            ret_val = ICF_FAILURE;

        }
        if(ICF_FAILURE == ret_val)
        {
            p_vldt_err->ecode = ICF_CAUSE_INVALID_ADDR_STRING;
        }
    } /* End of ICF_SUCCESS == ret_val */
    
    /* Fix for SPR: 19923*/
    /* If the address type determined is not the same as address type
     * passed, set failure.*/
    if((ICF_FAILURE != ret_val) && 
            (addr_type != p_addr->addr_type))
    {
        ret_val = ICF_FAILURE;
        p_vldt_err->ecode = ICF_CAUSE_INVALID_ADDRESS_TYPE;
    }

    return ret_val;
} 

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_media_transport_type_et
*
* DESCRIPTION:     This Function validates the media transport type. 
* RETURN:          icf_return_t 
*
******************************************************************************/
icf_return_t icf_vldt_icf_media_transport_type_et(
  IN     icf_media_transport_mode_et   media_transport_type, 
  INOUT  icf_vldt_err_st *p_vldt_err)
{
   icf_return_t ret_val = ICF_SUCCESS;

   if((ICF_MAX_MEDIA_TRANSPORT_TYPE < media_transport_type))
     {
         /*Assign failure to ret_val*/
         ret_val = ICF_FAILURE;
  
         /*Assign Error Code to ecode in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_INVALID_TRANSPORT_MODE; 
     }
     return(ret_val);

}       
                      


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_sdf_string_st
*
* DESCRIPTION:     This function is invoked to validate string.
*
**************************************************************************/
icf_return_t icf_vldt_icf_sdf_string_st(
             IN icf_sdf_string_st *p_str, 
             INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    if(ICF_NULL != p_str)
    {  
        /* 
        Check: string length should be less then max.string length i.e.
              ICF_MAX_STR_LEN and string length should be equal to dStrLen
        */
        if((p_str->dStrLen > SDF_MAXIMUM_STR_LEN) || 
           (icf_vldt_port_strlen((icf_uint8_t*)p_str->dStr) != p_str->dStrLen))
        {
             /*Assign failure to ret_val*/
	     ret_val = ICF_FAILURE;

	     /*Assign Error Code to ecode in p_vldt_err */      
	     p_vldt_err->ecode = ICF_CAUSE_INVALID_STR_LEN;
        }
     
        /* Check: string length should not be zero*/
        if(ret_val != ICF_FAILURE)
        {
             if(ICF_NULL == p_str->dStrLen)
	     {
      	         /*Assign failure to ret_val*/
                 ret_val = ICF_FAILURE;

                 /*Assign Error Code to ecode in p_vldt_err */
                 p_vldt_err->ecode=ICF_CAUSE_NULL_STRING;
	     }
         }
    }
     else
    {
         /*Assign failure to ret_val*/
         ret_val = ICF_FAILURE;

         /*Assign Error Code to ecode in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_NULL_STRING;
    }

     return(ret_val);
}
 
             
              
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_sdf_address_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  transport address structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_sdf_address_st(
                  INOUT icf_sdf_address_st *p_addr, 
                  INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint8_t     index = 0, count = 0;

    if(ICF_SUCCESS == ret_val)
    {
        /* Fix for SPR 19482: CSR-1-7130952*/
        /* If the c line address is of type IPV4 validate whether it is of
         * following form or not:
         * x.x.x.x (3 dots are present in string).Here we are not validating
         * the value of address, we are simply validating the format.*/
        if (SDF_CLINE_ADDRESS_IPV4_ADDR == p_addr->dAddrType)
        {
            /* Rel 8.2 Klocworks fix */
            for ( index = 0; ((index < p_addr->dAddrVal.dStrLen) && 
                          (index < ICF_MAX_STR_LEN -1 )) ; index ++ )
            {
                if ((p_addr->dAddrVal.dStr[index]) == '.')
                {
                    count ++;
                }
            }
            if ( count != 3 )
            { 
                ret_val = ICF_FAILURE;
                /*assign error string to vldt structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dAddrVal:");
            }
        }
        /* Fix for SPR 19482: CSR-1-7130952*/
        /* If the c line address is of type IPV6 validate whether it is of
         * following form or not:
         * x:x:x:x:x:x:x (7 colon are present in string).Here we are not validating
         * the value of address, we are simply validating the format.*/
        else if (SDF_CLINE_ADDRESS_IPV6_ADDR == p_addr->dAddrType)
        {
            /* Rel 8.2 Klocworks fix */
            for ( index = 0; ((index < p_addr->dAddrVal.dStrLen) && 
                          (index < ICF_MAX_STR_LEN -1 )) ; index ++ )
            {
                if ((p_addr->dAddrVal.dStr[index]) == ':')
                {
                    count ++;
                }
            }
            if ( count != 7 )
            { 
                ret_val = ICF_FAILURE;
                /*assign error string to vldt structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dAddrVal:");
            }            
        }
        else
        {
            ret_val = ICF_FAILURE;
            /*assign error string to vldt structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dAddrType:");
        }
    }
    
    if(ICF_SUCCESS == ret_val) 
    {
        /*Checking for null string in address string */
         ret_val = icf_vldt_icf_sdf_string_st(
                                   (icf_sdf_string_st *)(&p_addr->dAddrVal), p_vldt_err);

            if(ICF_FAILURE == ret_val)
            {
                /*assign error string to vldt structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dAddrVal:");
            }
    }

    return ret_val;
}
     
/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_tunnelled_sdp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_vldt_icf_tunnelled_sdp_st
*
**************************************************************************/

icf_return_t icf_vldt_icf_tunnelled_sdp_st(
                                      IN    icf_tunnelled_sdp_info_st *p_sdp ,
                                      INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint32_t i = 0;


    if(ICF_NULL != p_sdp)
    {

        if(p_sdp->med_level_desc[i].bitmask & ICF_TUNNELLED_SDP_SESSION_CLINE_PRESENT)
        {
            ret_val = icf_vldt_icf_sdf_string_st(&p_sdp->session_level_cline.dNetType, p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dNetType:");
            }/* End if*/

            ret_val = icf_vldt_icf_sdf_address_st((icf_sdf_address_st *)&(p_sdp->session_level_cline.dNetworkAddress), p_vldt_err);

            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dNetworkAddress:");
                         
            }/* End if*/
        }
        /*SPR 19465 Reject the API if the number of m lines exceed the maximum
          limit defined by ICF_MAX_NO_OF_M_LINES */
        if(ICF_SUCCESS == ret_val)
        {
            if(ICF_MAX_NO_OF_M_LINES < p_sdp->num_of_m_line)
            {
                p_vldt_err->ecode = ICF_CAUSE_VALUE_OUT_OF_RANGE;
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"num_of_m_line:");
                ret_val = ICF_FAILURE;
            }
        }

        if(ICF_SUCCESS == ret_val)
        {
            for (i = 0; (i < p_sdp->num_of_m_line) && (i < ICF_MAX_NO_OF_M_LINES) ; i++)
            {
                if(p_sdp->med_level_desc[i].bitmask & ICF_TUNNELLED_SDP_SESSION_CLINE_PRESENT)
                {
                    ret_val = icf_vldt_icf_sdf_string_st(&p_sdp->med_level_desc[i].med_level_cline.dNetType, p_vldt_err);
                    if(ICF_FAILURE == ret_val)
                    {
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dNetType:");
                        break;  
                    }/* End if*/
                    
                          
                    ret_val = icf_vldt_icf_sdf_address_st((icf_sdf_address_st *)&(p_sdp->med_level_desc[i].med_level_cline.dNetworkAddress), p_vldt_err);
                    
                    if (ICF_FAILURE == ret_val)
                    {
                
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dNetworkAddress:");
                        break;
              
                    }
                }
             
                    ret_val = icf_vldt_icf_sdf_string_st(&p_sdp->med_level_desc[i].m_line.dMediaType, p_vldt_err);
                    if(ICF_FAILURE == ret_val)
                    { 
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"Media_Type:");
                        break;
                    }/* End if*/
                
                    ret_val = icf_vldt_icf_sdf_string_st(&p_sdp->med_level_desc[i].m_line.dProtocol, p_vldt_err);
                    if(ICF_FAILURE == ret_val)
                    {
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"Protocol:");
                        break;
                    }/* End if*/

                    ret_val = icf_vldt_icf_sdf_string_st(&p_sdp->med_level_desc[i].m_line.dFormat, p_vldt_err);
                    if(ICF_FAILURE == ret_val)
                    {
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"Format:");
                        break;
                    }/* End if*/
                }
            }
            
            /*SPR 19465 Reject the API if the number of encoded string
              lines exceed the maximum limit defined by
              ICF_MAX_NO_OF_ENC_STRING  */
            if(ICF_SUCCESS == ret_val)
            {
                 if(ICF_MAX_NO_OF_ENC_STRING < p_sdp->num_of_encoded_string)
                 {
                      p_vldt_err->ecode = ICF_CAUSE_VALUE_OUT_OF_RANGE;
                      /* Add error string to p_vldt_err structure */
                      icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"num_of_encoded_string:");
                      ret_val = ICF_FAILURE;
                 }
            }
        if(ICF_SUCCESS == ret_val)
        {

            for (i = 0; (i < p_sdp->num_of_encoded_string) && (i < ICF_MAX_NO_OF_ENC_STRING) ; i++)
            {
                ret_val = icf_vldt_icf_sdf_string_st(&p_sdp->encoded_string[i].attr_str, p_vldt_err);
                if(ICF_FAILURE == ret_val)
                {
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"Attr_str:");
                    break;
                }/* End if*/

                if(ICF_SUCCESS == ret_val)
                {  
                    ret_val = icf_vldt_boolean(p_sdp->encoded_string[i].level, p_vldt_err);
                    if(ICF_FAILURE == ret_val)
                    {
                        /* Add error string to p_vldt_err structure */
                        icf_vldt_port_strcat(p_vldt_err->err_str, (icf_uint8_t *)"level:");
                        break;

                    }/* End if*/
                }/* End if*/

                /* Checking whether pos_of_media is greater than number of m lines or not 
                   If it is greater than number of m ,lines then validation fails */  

                if (p_sdp->encoded_string[i].pos_of_media > (p_sdp->num_of_m_line)-1)
                {
                    ret_val = ICF_FAILURE;
                    /* Add error string to p_vldt_err structure */
                    icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"pos_of_media:");
                    break;
                }/* End if*/
            }
        }
     }

return ret_val ;
}    
/*Fix for CSR 1-7718121 : SPR-20074*/
/******************************************************************************
 * Function:     icf_vldt_sip_uri_address_string
 *
 * Description:  This function validates the input string for valid
 *               SIP/SIPS uri address.
 *****************************************************************************/
 
icf_return_t icf_vldt_sip_uri_address_string(INOUT icf_uint8_t *p_addr)
{
    icf_return_t       ret_val = ICF_SUCCESS;
    icf_uint8_t        *p_str1 = ICF_NULL;
    icf_uint8_t        *p_str2 = ICF_NULL;
    icf_uint8_t        *p_temp = ICF_NULL;
    icf_uint16_t       addr_len = 0;
    icf_uint16_t       str1_len = 0;
    icf_uint16_t       str2_len = 0;
    icf_uint16_t       index = 0;
    icf_boolean_t      colon_found = ICF_FALSE;
    icf_uint8_t        temp_addr_str[ICF_MAX_STR_LEN] = "\0" ;

    addr_len = icf_vldt_port_strlen(p_addr);

    /* '@' or ':' should not be the first or last character in address
     *  string.
     */
    if(('@' == *p_addr) || ('@' == p_addr[addr_len-1]) ||
       (':' == *p_addr) || (':' == p_addr[addr_len-1]))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Find the character '@' in address string. If '@' is not present in
         * address string, then return failure. 
         */
        p_temp = icf_vldt_port_strchr(p_addr,'@');

        if(ICF_NULL == p_temp)
        {
            /*Merged CSR 1-8314017. Fix in CSR 1-8561947.*/
			/* Two ':' should not present in address string.
			 */
			for(index = 0;index < addr_len;index++)
			{
				if(':' == p_addr[index])
				{
					if(ICF_FALSE == colon_found)
					{
						colon_found = ICF_TRUE;
					}
					else
					{
            ret_val = ICF_FAILURE;
						break;
					}
				}
			}
        }
        /* If the character '@' is present in the address string, tokenize 
         * the address string in two two tokens by using '@' as delimiter.
         */
        else
        {
             /* Merged CSR 1-8314017. Fix in CSR 1-8561947*/ 
             /* copy the actual string into the temp string.
              * Do not modify the actual string.
              */
             icf_vldt_port_strcpy(temp_addr_str,p_addr);

            /* Second token of the string identified by pointing the 
             * p_temp + 1. P_temp is pointing the first occurence of '@'
             * character in address string.
             */
             p_str2 = p_temp + 1;
             str2_len = icf_vldt_port_strlen(p_str2);

             /* get the first token of the address string.
              */
		 p_str1 = (icf_uint8_t *)icf_vldt_port_strtok((icf_int8_t*)temp_addr_str,"@");
		 /* SPR 20175 -Klcoworks warning fixing */
		 if(ICF_NULL == p_str1)
		 {
			 ret_val = ICF_FAILURE;				 
		 }	 
		 else 
		 {

			 str1_len = icf_vldt_port_strlen(p_str1);

			 /* Colon should not be the first or last character in
			  * any substring (token).
			  */
			 if((':' == *p_str1) || (':' == p_str1[str1_len - 1]) ||
				 (':' == *p_str2) || (':' == p_str2[str2_len - 1]))
			 {
				 ret_val = ICF_FAILURE;
			 }
		 	else
		 	{
				 /* The token-1 that is str1 should not contain the character ':'
			  	*/

				 if(ICF_NULL != icf_vldt_port_strchr(p_str1,':'))
				 {
					 ret_val = ICF_FAILURE;
				 }
				 else
				 {
					 for(index = 0;index < str2_len;index++)
					 {
						 if('@' == p_str2[index])
						 {
							 ret_val = ICF_FAILURE;
						 }
						 else if(':' == p_str2[index])
						 {
							 if(ICF_FALSE == colon_found)
							 {
								 colon_found = ICF_TRUE;
							 }
							 else
							 {
								 ret_val = ICF_FAILURE;
								 break;
							 }
						 }
					 }
				 }
			 }
	              }/* End of klocworks induced else */
        }
    }
    return ret_val;
}


