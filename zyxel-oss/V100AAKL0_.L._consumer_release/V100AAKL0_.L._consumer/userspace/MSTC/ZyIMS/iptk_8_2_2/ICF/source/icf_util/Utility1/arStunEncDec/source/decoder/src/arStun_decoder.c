/******************************************************************************
*                                    
* FILE NAME    : arStun_decoder.c                
*                                    
* DESCRIPTION  : Contains the arStun decoding routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Jan-2009    Anuradha Gupta         None          Initial
* 08-Jan-2009    Shweta Jain            LLD           arStunEncDec 1.0  
*          
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/
#include "common_defs.h"
#include "common_types.h"
#include "arStunEncDec_api.h"
#include "arStun_dec_prototypes.h"
#include "arStun_dec_macro.h"

/****************************************************************************
 * API Name: arStun_decode_message_header
 * 
 * Description: This API is invoked to decode the STUN messages header and 
 *              message can be binding request, binding response and binding 
 *              error response.
 * INPUT       :pMsgBuffer is a buffer having the data for decoding. 
 * OUTPUT      :pStunMsgHdr is a STUN message Header.
 *             :pError is the error code.    
 *              
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
 util_return_t     arStun_decode_message_header ( 
                   IN          arStun_msg_buffer_st    *pMsgBuffer, 
                   OUT         arStun_msg_hdr_st       *pStunMsgHdr,
                   OUT         arStun_ecode_et         *pError)      

 {
    /* It specifies the result of API */
    util_return_t ret_val = UTIL_SUCCESS;

    /* This will be used for message value contained in buffer pMsgBuffer */ 
    util_uint8_t *p_temp_buffer = UTIL_NULL;

    /* perform Null pointer validations */    
    if(UTIL_NULL == pError)
    {
	    ARSTUN_PRINT(("Validation Failure\n"));
	    ret_val = UTIL_FAILURE;
    }
    else if(UTIL_NULL == pMsgBuffer)
    {
	    ARSTUN_PRINT(("Message Buffer is Invalid\n")); 
	    *pError = ARSTUN_ERROR_INVALID_MSG_BUFFER;
	    ret_val = UTIL_FAILURE;
    }
    else if(UTIL_NULL == pStunMsgHdr)
    {
	    ARSTUN_PRINT(("Invalid STUN Message Header\n"));
	    *pError = ARSTUN_ERROR_INVALID_STUN_MSG_HDR;
	    ret_val = UTIL_FAILURE;
    }
    else
    {
        /* Assigning the address of message val of buffer */
        p_temp_buffer = (util_uint8_t*)pMsgBuffer->msg_val;

        /* Setting the value of message in stun message header */
        UTIL_MEMCPY(pStunMsgHdr, p_temp_buffer, UTIL_SIZEOF(arStun_msg_hdr_st));

        pStunMsgHdr->msg_type = UTIL_NTOHS(pStunMsgHdr->msg_type);
        pStunMsgHdr->msg_length = UTIL_NTOHS(pStunMsgHdr->msg_length);
    }

    return ret_val;
 }

/****************************************************************************
 * API Name: arStun_decode_message
 * 
 * Description: This API is invoked to decode the STUN messages i.e binding 
 *              request, binding response and binding error response.
 * INPUT       :pMsgBuffer is a buffer having the data for decoding. 
 * OUTPUT      :pStunMessage is a STUN message.
 *             :pError is the error code.           
 *              
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/

util_return_t     arStun_decode_message( 
                  IN      arStun_msg_buffer_st   *pMsgBuffer, 
                  OUT     arStun_msg_st          *pStunMessage,
                  OUT     arStun_ecode_et        *pError)
 {
    /* It specifies the result of API */
    util_return_t ret_val = UTIL_SUCCESS;

    /* This will be used for message value contained in buffer pMsgBuffer */  
    util_uint8_t *p_temp_buffer = UTIL_NULL;

    /* msg_size will have message size from buffer */
    util_uint32_t msg_size = UTIL_INIT_VAL;

    /* parsed_data_len will have the length of parsed data */
    util_uint32_t parsed_data_len = UTIL_INIT_VAL;

    /* This variable will contain the message header */ 
    arStun_msg_hdr_st  stun_msg_hdr; 

    /* This variable will contain the info. of attribute */ 
    arStun_attr_hdr_st *pAttrib = UTIL_NULL;

    /* attrib_type is used to store the value of attribute's type */ 
    util_uint16_t attrib_type = UTIL_INIT_VAL;

    /* attrib_len is used to store the value of attribute's length */
    util_uint32_t attrib_len =  UTIL_INIT_VAL; 

    /* perform Null pointer validations */    
    if(UTIL_NULL == pError)
    {
       ARSTUN_PRINT(("Error pointer NULL\n"));
       return UTIL_FAILURE;
    }
    else if(UTIL_NULL == pMsgBuffer)
    {
       ARSTUN_PRINT(("Message Buffer is Invalid\n")); 
       *pError = ARSTUN_ERROR_INVALID_MSG_BUFFER;
       return UTIL_FAILURE;
    }
    else if(UTIL_NULL == pStunMessage)
    {
       ARSTUN_PRINT(("Invalid STUN Message struct\n"));
       *pError = ARSTUN_ERROR_INVALID_STUN_MSG_STRUCT;
       return UTIL_FAILURE;
    }
    /* perform the size validations */
    else if (UTIL_SIZEOF(arStun_msg_hdr_st) > pMsgBuffer->msg_len)
    {
       ARSTUN_PRINT(("Bad STUN Message: Message length is less than header size\n"));
       ARSTUN_PRINT(("Bad STUN Message: Size validation failure\n"));
       *pError = ARSTUN_ERROR_BAD_STUN_MSG;
       return UTIL_FAILURE;
    }
   
    /* decode the message header */
    arStun_decode_message_header(pMsgBuffer,&stun_msg_hdr,pError);

   if (stun_msg_hdr.msg_length + 
            UTIL_SIZEOF(arStun_msg_hdr_st) != pMsgBuffer->msg_len)
   {
       ARSTUN_PRINT(("Bad STUN Message: Mismatch in Message lengths\n"));
       ARSTUN_PRINT(("Bad STUN Message: Size validation failure"));
       *pError = ARSTUN_ERROR_BAD_STUN_MSG;
       return UTIL_FAILURE;
   }

   /* Setting the size of the message */ 
   msg_size  = stun_msg_hdr.msg_length;
   pStunMessage->msg_hdr =  stun_msg_hdr;

   /* take the reference of message buffer */
   p_temp_buffer = (util_uint8_t*)pMsgBuffer->msg_val 
      + UTIL_SIZEOF(arStun_msg_hdr_st);

   pStunMessage->msg_attrib_presence = 0;

   /* Loop goes till the whole message is decoded */
   while ( msg_size > parsed_data_len)
   {	
      pAttrib = (arStun_attr_hdr_st *)p_temp_buffer;
		
      attrib_len = UTIL_NTOHS(pAttrib->attribute_length);
      attrib_type = UTIL_NTOHS(pAttrib->attributes_type);

      if ( (attrib_len + ARSTUN_ATTRIB_HDR_SIZE) > (msg_size - parsed_data_len)) 
      {
         ARSTUN_PRINT(("Bad STUN Message\n"));
         *pError = ARSTUN_ERROR_BAD_STUN_MSG;
         return UTIL_FAILURE;
      }
	
      /* skip the length and type of attribute header */    
      p_temp_buffer += ARSTUN_ATTRIB_HDR_SIZE;
      parsed_data_len += ARSTUN_ATTRIB_HDR_SIZE;
		
      switch ( attrib_type )
      {
         case ARSTUN_ATTRIB_MAPPED_ADDRESS:

            /*Decode the mapped address*/
            if ( UTIL_FAILURE == arStun_decode_attrib_address( 
                        p_temp_buffer,  attrib_len,  
                        &pStunMessage->mapped_addr, pError))
            {
                 ARSTUN_PRINT(("Failed to Decode attribute Mapped Address\n"));
                 return UTIL_FAILURE;
            }
            else
            {
               ARSTUN_PRINT(("Successful to Decode attribute Mapped Address\n"));
               pStunMessage->msg_attrib_presence |= 
                   ARSTUN_MAPPED_ADDRESS_PRESENT;
            }
					
            break;  

         case ARSTUN_ATTRIB_RESPONSE_ADDRESS:
            
            /*Decode the response address address*/
            if ( UTIL_FAILURE == arStun_decode_attrib_address( 
                        p_temp_buffer,  attrib_len,  
                        &pStunMessage->response_addr, pError))
            {
                 ARSTUN_PRINT(("Failed to Decode attribute Response Address\n"));
                 return UTIL_FAILURE;
            }
            else
            {
               ARSTUN_PRINT(("Successful to Decode attribute Response Address\n"));
               pStunMessage->msg_attrib_presence |= 
                   ARSTUN_RESPONSE_ADDRESS_PRESENT;
            }

            break;  
       
         case ARSTUN_ATTRIB_CHANGE_REQUEST:
           
            /*Decode the change request*/
            if ( UTIL_FAILURE == arStun_decode_attrib_changeRequest( 
                        p_temp_buffer,  attrib_len,  
                        &pStunMessage->change_request, pError))
            {
                 ARSTUN_PRINT(("Failed to Decode attribute Change Request\n"));
                 return UTIL_FAILURE;
            }
            else
            {
               ARSTUN_PRINT(("Successful to Decode attribute Change Request\n"));
               pStunMessage->msg_attrib_presence |= 
                   ARSTUN_CHANGE_REQUEST_PRESENT;
            }

            break;
				
         case ARSTUN_ATTRIB_SOURCE_ADDRESS:

            /*Decode the source address*/
            if ( UTIL_FAILURE == arStun_decode_attrib_address( 
                        p_temp_buffer,  attrib_len,  
                        &pStunMessage->source_addr, pError))
            {
                 ARSTUN_PRINT(("Failed to Decode attribute Source Address\n"));
                 return UTIL_FAILURE;
            }
            else
            {
              ARSTUN_PRINT(("Successful to Decode attribute Source Address\n"));
               pStunMessage->msg_attrib_presence |= 
                   ARSTUN_SOURCE_ADDRESS_PRESENT;
            }

            break;  
				
          case ARSTUN_ATTRIB_CHANGED_ADDRESS:
           
            /*Decode the changed address*/
            if ( UTIL_FAILURE == arStun_decode_attrib_address( 
                        p_temp_buffer,  attrib_len,  
                        &pStunMessage->changed_addr, pError))
            {
                 ARSTUN_PRINT(("Failed to Decode attribute Changed Address\n"));
                 return UTIL_FAILURE;
            }
            else
            {
               ARSTUN_PRINT(("Successful to Decode attribute Changed Address\n"));
               pStunMessage->msg_attrib_presence |= 
                   ARSTUN_CHANGED_ADDRESS_PRESENT;
            }

            break;  
				
        case ARSTUN_ATTRIB_USERNAME: 
            
	   /*Decode the user name*/
           if ( UTIL_FAILURE == arStun_decode_attrib_string( 
                        p_temp_buffer,  attrib_len,  
                        &pStunMessage->username, pError))
            {
                 ARSTUN_PRINT(("Failed to Decode attribute UserName\n"));
                 return UTIL_FAILURE;
            }
            else
            {
                   ARSTUN_PRINT(("Successful to Decode attribute UserName\n"));
                   pStunMessage->msg_attrib_presence |= 
                   ARSTUN_USER_NAME_PRESENT;
            }
		
            break;
         				
         case ARSTUN_ATTRIB_ERROR_CODE:
            
	    /*Decode the error code*/
            if ( UTIL_FAILURE == arStun_decode_attrib_error( 
                        p_temp_buffer,  attrib_len,  
                        &pStunMessage->error_code, pError))
            {
                 ARSTUN_PRINT(("Failed to Decode attribute Error Code\n"));
                 return UTIL_FAILURE;
            }
            else
            {
                   ARSTUN_PRINT(("Successful to Decode attribute Error Code\n"));
                   pStunMessage->msg_attrib_presence |= 
                   ARSTUN_ERROR_CODE_PRESENT;
            }
		
            break;
				
         case ARSTUN_ATTRIB_UNKNOWN_ATTRIBUTE:

            /*Decode the unknown attributes*/
            if ( UTIL_FAILURE == arStun_decode_attrib_unknown( 
                        p_temp_buffer,  attrib_len,  
                        &pStunMessage->unknown_attributes, pError))
            {
                 ARSTUN_PRINT(("Failed to Decode attribute Unknown Attribute\n"));
                 return UTIL_FAILURE;
            }
            else
            {
                   ARSTUN_PRINT(("Successful to Decode attribute Unknown Attribute\n"));
                   pStunMessage->msg_attrib_presence |= 
                   ARSTUN_UNKNOWN_ATTR_PRESENT;
            }

            break;
				
         case ARSTUN_ATTRIB_REFLECTED_FROM:
           
           /*Decode the reflected from*/
           if ( UTIL_FAILURE == arStun_decode_attrib_address( 
                        p_temp_buffer,  attrib_len,  
                        &pStunMessage->reflected_from, pError))
            {
                 ARSTUN_PRINT(("Failed to Decode attribute Reflected From\n"));
                 return UTIL_FAILURE;
            }
            else
            {
                   ARSTUN_PRINT(("Successful to Decode attribute Reflected From\n"));
                   pStunMessage->msg_attrib_presence |= 
                   ARSTUN_REFLECTED_FROM_PRESENT;
            }

            break;  
		
         case ARSTUN_ATTRIB_XOR_ONLY:
         case ARSTUN_ATTRIB_MESSAGE_INTEGRITY:
         case ARSTUN_ATTRIB_PASSWORD:
            /* ignore these fields as not required for decoding
             * the stun messages 
             */ 
            break;

         default:
            if ( attrib_type <= 0x7FFF ) 
            {
                 ARSTUN_PRINT(("Invalid Attribute\n"));
                 *pError = ARSTUN_ERROR_BAD_STUN_MSG;
                 return UTIL_FAILURE;
            }
      }
		
      p_temp_buffer += attrib_len;
      parsed_data_len += attrib_len;
   }
    
   if( ARSTUN_BIND_RESP_MSG == pStunMessage->msg_hdr.msg_type )
   {
      if ( !(ARSTUN_MAPPED_ADDRESS_PRESENT & pStunMessage->msg_attrib_presence) ) 
      {
         ARSTUN_PRINT(("Mandatory Attribute Mapped address is not present\n"));
      }
      if ( !(ARSTUN_SOURCE_ADDRESS_PRESENT & pStunMessage->msg_attrib_presence) )   
      {
         ARSTUN_PRINT(("Mandatory Attribute Source address is not present\n"));
      }
      if ( !(ARSTUN_CHANGED_ADDRESS_PRESENT & pStunMessage->msg_attrib_presence) )  
      {
         ARSTUN_PRINT(("Mandatory Attribute Changed address is not present\n"));
      }
    }
  if( ARSTUN_BIND_ERR_RESP_MSG == pStunMessage->msg_hdr.msg_type )
  {
    if ( !(ARSTUN_ERROR_CODE_PRESENT & pStunMessage->msg_attrib_presence) ) 
    {
       ARSTUN_PRINT(("Mandatory Attribute error is not present\n"));
    }

  }
    return ret_val;
}


