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
* 08-Apr-2008   Amit Sikka                       SPR 18380   Under CSR 1-5838506
* 08-Oct-2008    Puneet Sharma                   CSR 1-6325377
* Copyright 2007, Aricent.
*
*******************************************************************************/
 
#include "icf_vldt_prototypes.h"
#include "icf_vldt_defs.h"

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_address_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  transport address structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_address_st(
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
    /* This varialbe is used to identfiy e164 number */
    icf_boolean_t e164_number = ICF_TRUE;
    /* This varialbe is used to identfiy tel number */
    icf_boolean_t tel_number =  ICF_TRUE;
        
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
            if(p_addr_data[0] == telurl_start || ICF_ISXDIGIT(p_addr_data[0]))
            {
                for(counter = 1; counter < icf_vldt_port_strlen(p_addr_data); counter++)
                {
                    if(!ICF_ISXDIGIT(p_addr_data[counter])) 
                    {
                        /* If this was not a hexadecimal digit. the number cant be 
                        * an E164 Number*/
                        e164_number = ICF_FALSE;
                    
                        if('-' != p_addr_data[counter])
                        {
                            /* If number does not contain [0-f] or '-'
                             *  it is not valid number  */
                            tel_number = ICF_FALSE;
                            break;
                        }
                    }

                }/*End of for loop */

            if(ICF_TRUE == e164_number)
            {
                /* it is an E164 url number */
                addr_type = ICF_ADDRESS_PLAN_E164;
            }
            else if(ICF_TRUE == tel_number)
            {
                /* it is a tel url number */
                addr_type = ICF_ADDRESS_PLAN_TEL_URI;
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
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *) "string_list_invalid_count:");
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
         while( p_ipv6_addr->str[i] != '\0')
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
     
    /* Chck for the correctness of type of transport layer 
       i.e whether tcp,udp or message queue 
    */
    if(p_rt_info->type != ICF_IPC_UDP)
    {
        /* Assign failue to ret_val */
        ret_val = ICF_FAILURE;

        /*Assign Error Code to ecode in p_vldt_err */
        p_vldt_err->ecode = ICF_CAUSE_INCORRECT_TRANSPORT_MODE ;
        
        
    } /* if(p_rt_info->type != ICF_IPC_UDP) */

    
    /* if route info transport layer mode is correct */
    if(ICF_SUCCESS == ret_val) 
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
         else
         {         
              /* Check: Length of string "value" should be equal to 
                 strlen(value)
              */
              if (p_message->length != icf_vldt_port_strlen(p_message->value))
              {
                  /*Assign failure to ret_val*/
                  ret_val = ICF_FAILURE;
                       
                  /*Assign Error Code to ecode in p_vldt_err */

                  p_vldt_err->ecode = ICF_CAUSE_INVALID_MSG_BODY_LEN;
              }
         }
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
             
     if((ICF_MAX_TRANSPORT_TYPE < tmode))
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
				ret_val = icf_vldt_icf_string_st(
								(icf_string_st*)&p_transport_addr->addr.domain, p_vldt_err);
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

     /*CSR 1-6325377*/
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
     if(p_codec_attrib->bit_mask &
            ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT)
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
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str, (icf_uint8_t *)"codec_name:");
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

     /*CSR 1-6325377*/
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
                  IN icf_config_address_st *p_config_addr,
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
    /* This varialbe is used to identfiy e164 number */
    icf_boolean_t e164_number = ICF_TRUE;
    /* This varialbe is used to identfiy tel number */
    icf_boolean_t tel_number =  ICF_TRUE;
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
            if(p_addr_data[0] == telurl_start || ICF_ISXDIGIT(p_addr_data[0]) || (ICF_ISSPECIALDIGIT(p_addr_data[0])))

            {
                
                for(counter = 1; counter < icf_vldt_port_strlen(p_addr_data); counter++)
                {
                    /* A tel URI may have special digits like * and
                    #.We are validating them.(Ref RFC 3966) */
 
                    if(!ICF_ISXDIGIT(p_addr_data[counter])) 
                    {
                        /* If this was not a hexadecimal digit. the number cant be 
                        * an E164 Number*/
                       e164_number = ICF_FALSE;
                    
                       if(('-' != p_addr_data[counter]) &&
                       (!ICF_ISSPECIALDIGIT(p_addr_data[counter])))

                        {
                            /* If number does not contain [0-f] or '-'
                             *  it is not valid number  */
                            tel_number = ICF_FALSE;
                            break;
                        }
                    }

                }/*End of for loop */

            if(ICF_TRUE == e164_number)
            {
                /* it is an E164 url number */
                addr_type = ICF_ADDRESS_PLAN_E164;
            }
            else if(ICF_TRUE == tel_number)
            {
                /* it is a tel url number */
                addr_type = ICF_ADDRESS_PLAN_TEL_URI;
            }

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

    return ret_val;
}        
                      



                 
 
             
              

     
         
         
              


