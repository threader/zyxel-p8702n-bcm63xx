/******************************************************************************
*                                    
* FILE NAME    : arStun_dec_util.c                
*                                    
* DESCRIPTION  : Contains the arStun Decoding utility routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Jan-2009    Anuradha Gupta         None          Initial decode utility
* 07-Jan-2009    Shweta Jain            LLD           arStunEncDec 1.0     
*        
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/
#include "common_defs.h"
#include "arStun_dec_prototypes.h"
#include "arStun_dec_macro.h"

/****************************************************************************
 * API Name: arStun_decode_U8
 * 
 * Description:    This function shall be  used to decode 1 byte data 
 *
 * Return : Nothing.
 ******************************************************************************/
util_void_t
arStun_decode_U8
(
   util_uint8_t     **pSrc,
   util_uint8_t     *pOutput)
{
    util_uint8_t *p_temp = *pSrc;
    *pOutput = *p_temp++;
    *pSrc = p_temp;
    return;
}

/****************************************************************************
 * API Name: util_decode_U16
 * 
 * Description:    This function shall be  used to decode 2 byte data 
 *
 * Return : Nothing.
 ******************************************************************************/
util_void_t
arStun_decode_U16
(
    util_uint8_t    **pSrc,
    util_uint16_t   *pOutput)
{
    util_uint16_t   ntohs_data = 0;
    util_uint8_t *p_temp = *pSrc;

    UTIL_MEMCPY(&ntohs_data, p_temp, UTIL_SIZEOF(util_uint16_t));    
    p_temp += UTIL_SIZEOF(util_uint16_t);
    *pOutput = UTIL_NTOHS(ntohs_data);
    *pSrc = p_temp;

    return;
}

/****************************************************************************
 * API Name: util_decode_U32
 * 
 * Description:    This function shall be  used to decode 4 byte data 
 *
 * Return : Nothing.
 ******************************************************************************/
util_void_t
arStun_decode_U32
(
    util_uint8_t     **pSrc,
    util_uint32_t    *pOutput)
{
    util_uint32_t   ntohl_data = 0;
    util_uint8_t *p_temp = *pSrc;

    UTIL_MEMCPY(&ntohl_data, p_temp, UTIL_SIZEOF(util_uint32_t));    
    p_temp += UTIL_SIZEOF(util_uint32_t);
    *pOutput = UTIL_NTOHL(ntohl_data);
    *pSrc = p_temp;

    return;
}

/****************************************************************************
 * API Name: util_decode_data
 * 
 * Description:    This function shall be  used to decode data as per the length
 *
 * Return : Nothing.
 ******************************************************************************/
util_void_t
arStun_decode_data
(
    util_uint8_t     **pSrc,
    util_uint8_t    *pOutput,
    util_uint32_t     length)
{
    util_uint8_t *p_temp = *pSrc;

    UTIL_MEMCPY(pOutput, p_temp, length);    
    p_temp += length;
    *pSrc = p_temp;

    return;
}


/*****************************************************************************
 * FUNCTION :   arStun_decode_attrib_address 
 *
 * Description: This function is used to decode the address related attributes  
 *              of the stun message.
 * INPUT        :pMsgBuffer is buffer containing data.
 *              :attrib_Len is the length of attribute.
 * OUTPUT       :pOutput that will contain address attribute's information.
 *              :pError is error code.
 *             
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/
 util_return_t   
 arStun_decode_attrib_address ( 
               util_uint8_t*              pMsgBuffer, 
               util_uint32_t              attribLen, 
               arStun_attr_addr_st        *pOutput,
               arStun_ecode_et            *pError)

{
     util_return_t   ret_val = UTIL_SUCCESS;

     /*If Attribute length is not equal to length of address attribute, 
     * then failure will be returned */  
     if ( ARSTUN_ATTRIB_ADDR_LEN != attribLen)
     { 
           ARSTUN_PRINT(("Wrong length of address attribute\n"));
           *pError = ARSTUN_ERROR_BAD_STUN_MSG; 
           return UTIL_FAILURE;
     }
     /* Fields of arStun_attr_address_st structure will be populated 
      * by extracting values from the buffer 
      * Padding and ipfamily fields are populated which are of 1 byte each  */
      arStun_decode_U8(&pMsgBuffer , &(pOutput->padding)); 
      arStun_decode_U8(&pMsgBuffer , &(pOutput->ipfamily));
     
      /* Check for family, if it is IPV4 then other fields will be populated 
      * otherwise failure will be returned */
      if (ARSTUN_IPV4_FAMILY == pOutput->ipfamily)
      {
            util_uint32_t ipv4_addr;

           /* Populate the fields of arStun_ip_port_address_st structure from the buffer */
           /* port and address will be populated which are of 2 and 4 bytes respectively */
  
         arStun_decode_U16(&pMsgBuffer , &(pOutput->ipaddr.port)); 
         arStun_decode_U32(&pMsgBuffer , &(ipv4_addr)); 

         /* It wil convert the pOutput->ipaddr.ipv4_address to IPV4 address */
         arStun_convert_data_to_ipaddr(&(pOutput->ipaddr.ipv4_address), &ipv4_addr);
     }  
     else
     {
          ARSTUN_PRINT(("Wrong address family of address attribute\n"));
          *pError = ARSTUN_ERROR_BAD_STUN_MSG;
          ret_val = UTIL_FAILURE;   
     }
     return ret_val;      
  }


/*****************************************************************************
 * FUNCTION :   arStun_decode_attrib_changeRequest 
 *
 * Description: This function is used to decode the change_Request attribute of 
 *              the stun message.
 * INPUT        :pMsgBuffer is buffer containing data.
 *              :attrib_Len is the length of attribute.
 * OUTPUT       :pOutput will contain change_request attribute's information.
 *              :pError is error code.
 *             
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/
 util_return_t   
 arStun_decode_attrib_changeRequest ( 
           util_uint8_t*              pMsgBuffer,
           util_uint32_t              attribLen, 
           arStun_attr_change_req_t   *pOutput, 
           arStun_ecode_et            *pError)
{
     util_return_t   ret_val = UTIL_SUCCESS;

     /*If Attribute length is not equal to length of address attribute, 
      * then failure will be returned */  
     if ( ARSTUN_ATTRIB_CHANGE_REQ_LEN != attribLen )
     {
            ARSTUN_PRINT(("Wrong length of change_request attribute\n"));
            *pError =  ARSTUN_ERROR_BAD_STUN_MSG; 
            ret_val = UTIL_FAILURE;
     }
     /* arStun_attr_change_req_t will be populated by extracting values from the buffer
     * output is populated which is of 4 bytes */
     else
     {
             arStun_decode_U32(&pMsgBuffer , pOutput);
     }
     return ret_val;
}

/*****************************************************************************
 * FUNCTION :   arStun_decode_attrib_error 
 *
 * Description: This function is used to decode the error attribute of 
 *              the stun message.
 * INPUT        :pMsgBuffer is buffer containing data.
 *              :attrib_Len is the length of attribute.
 * OUTPUT       :pOutput will contain error attribute's information.
 *              :pError is error code.
 *             
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/
 util_return_t    
 arStun_decode_attrib_error ( 
         util_uint8_t*             pMsgBuffer, 
         util_uint32_t             attribLen,  
         arStun_attr_error_st      *pOutput,
         arStun_ecode_et           *pError)
{
     util_return_t   ret_val = UTIL_SUCCESS;
     
     /*If Attribute length is greater than size of error attribute structure, 
      * then failure will be returned 
      */  
      if ( UTIL_SIZEOF(arStun_attr_error_st) < attribLen )
      {
         ARSTUN_PRINT(("Wrong length of error attribute\n"));
         *pError =  ARSTUN_ERROR_BAD_STUN_MSG;
         ret_val = UTIL_FAILURE;
      }
      /* Fields of arStun_attr_error_st structure will be populated by 
       * extracting values from the buffer. Pad, error_class and number fields   
       * are populated which are of 2,1 and 1 bytes respectively 
       */
      else
      {
          /* decode the padding value */
          arStun_decode_U16(&pMsgBuffer , &(pOutput->padding));

          /* decode the error class and error number */
          arStun_decode_U8(&pMsgBuffer , &(pOutput->error_class));
          arStun_decode_U8(&pMsgBuffer , &(pOutput->number));

         /* Calculating the length of reason field by substracting the 
         no. of bytes of previous 3 fields */
         pOutput->reason.str_len = (util_uint16_t )(attribLen - 4);

	 if(ARSTUN_MAX_STR_LEN <= pOutput->reason.str_len)
	 {
		 pOutput->reason.str_len = ARSTUN_MAX_STR_LEN-1;
	 }

         if(pOutput->reason.str_len > 0)
         {
              /* Populating the field reason which is of no. of 
               * bytes calculated from previous step */
               arStun_decode_data(&pMsgBuffer , pOutput->reason.str_val,
                    pOutput->reason.str_len);

               pOutput->reason.str_val[pOutput->reason.str_len] = '\0';
         }
      }

      return ret_val;
}

/*****************************************************************************
 * FUNCTION :   arStun_decode_attrib_unknown
 *
 * Description: This function is used to decode the unknown attribute of 
 *              the stun message.
 * INPUT        :pMsgBuffer is buffer containing data.
 *              :attrib_Len is the length of attribute.
 * OUTPUT       :pOutput will contain unknown attribute's information.
 *              :pError is error code.
 *             
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/
 util_return_t     
 arStun_decode_attrib_unknown ( 
            util_uint8_t*              pMsgBuffer, 
            util_uint32_t              attribLen, 
            arStun_attr_unknown_st     *pOutput,
            arStun_ecode_et            *pError)
 {
     util_return_t   ret_val = UTIL_SUCCESS;
     util_uint8_t count=0;

    /*If Attribute length is greater than or equal to size of   
    * unknown attribute structure, then failure will be returned */  
    if ( UTIL_SIZEOF(arStun_attr_unknown_st) <= attribLen )
    {
       ARSTUN_PRINT(("Wrong length of unknown attribute\n"));
       *pError = ARSTUN_ERROR_BAD_STUN_MSG;
       ret_val = UTIL_FAILURE;
    }
    /* the length must be in multiple of 4 */
    else if(0 != (attribLen % 4))
    {
       ARSTUN_PRINT(("Wrong length of unknown attribute: not divisible by 4\n"));
       *pError = ARSTUN_ERROR_BAD_STUN_MSG;
       ret_val = UTIL_FAILURE;
    }
    else
    {
        /* Calculate the no. of attributes and thus populate num_attributes field */
        pOutput->num_attributes = (util_uint16_t )(attribLen / 2 );
      
        /* Populate the field attr_type by running a loop for total no. of attributes */
        for (count=0; count < pOutput->num_attributes; count++)
        {
            arStun_decode_U16(&pMsgBuffer , &(pOutput->attr_type[count]));
        }
    }
    return ret_val;
 }

/*****************************************************************************
 * FUNCTION :   arStun_decode_attrib_string
 *
 * Description: This function is used to decode the string related attributes 
 *              of the stun message.
 * INPUT        :pMsgBuffer is buffer containing data.
 *              :attrib_Len is the length of attribute.
 * OUTPUT       :pOutput will contain string attribute's information.
 *              :pError is error code.
 *             
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/
 util_return_t      
 arStun_decode_attrib_string ( 
            util_uint8_t*             pMsgBuffer, 
            util_uint32_t             attribLen, 
            arStun_string_st          *pOutput,
            arStun_ecode_et           *pError)
 {
     util_return_t   ret_val = UTIL_SUCCESS;

    /*If Attribute length is greater than or equal to    
    ARSTUN_MAX_STR_LEN , then failure will be returned */      
    if( ARSTUN_MAX_STR_LEN <= attribLen )
    {
       ARSTUN_PRINT(("Wrong length of string attribute\n"));
       *pError = ARSTUN_ERROR_BAD_STUN_MSG;
       ret_val = UTIL_FAILURE;
    } 
    /* the length must be in multiple of 4 in order to guarantee
    * alignment of attributes on word boundaries
    */ 
    else if(0 != (attribLen % 4))
    {
       ARSTUN_PRINT(("Wrong length of string attribute: not divisible by 4\n"));
       *pError = ARSTUN_ERROR_BAD_STUN_MSG;
       ret_val = UTIL_FAILURE;
    }
    else
    {
        /*Populating the fields of structure arStun_string_st from the buffer */
        pOutput->str_len = (util_uint16_t )attribLen;
        
        arStun_decode_data(&pMsgBuffer , pOutput->str_val,
                pOutput->str_len);

        pOutput->str_val[pOutput->str_len] = '\0';
    }
     return ret_val;
 }


/*****************************************************************************
 * FUNCTION :   arStun_convert_data_to_ipaddr
 *
 * Description: This function is used to decode the string related attributes 
 *              of the stun message.
 * INPUT        :p_packed_data contain a packed value of 4 byte ip address.
 * OUTPUT       :p_ipv4_addr is ip address  
 *              
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/
 util_return_t     
 arStun_convert_data_to_ipaddr (
              arStun_ipv4_address_st       *p_ipv4_addr,
              util_uint32_t                *p_packed_data)
 {
     /* Populate the fields of arStun_ipv4_address_st structure from 
     * the variable p_packed_data */ 

     p_ipv4_addr->octet_1  = (util_uint8_t)(*p_packed_data >> 24);
     p_ipv4_addr->octet_2  = (util_uint8_t)(*p_packed_data >> 16);
     p_ipv4_addr->octet_3  = (util_uint8_t)(*p_packed_data >> 8);
     p_ipv4_addr->octet_4  = (util_uint8_t)(*p_packed_data);

     return UTIL_SUCCESS;
 }




