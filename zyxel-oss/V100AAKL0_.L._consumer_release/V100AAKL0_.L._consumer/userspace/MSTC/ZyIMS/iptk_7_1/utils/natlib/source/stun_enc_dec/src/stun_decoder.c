/******************************************************************************
 * 
 * FILE NAME:   stun_decoder.c
 * 
 * DESCRIPTION: This file contains the decoding routines of STUN Messages
 *              
 * REVISION HISTORY:    
 *        Date            Name                Reference       Comments
 * ------------------------------------------------------------------------------
 *  21-Nov-2006     Anuradha Gupta          HLD             Coding
 *
 ******************************************************************************/ 
 
#include "stun_enc_dec_types.h"
#include "nat_port_pltfm_prototypes.h"
#include "stun_enc_dec_prototypes.h"
#include "nat_error_id.h"
#include "nat_types.h"


static nat_boolean_t 
stunParseAtrAddress( nat_int8_t* body, nat_uint32_t hdrLen,  stun_attr_address_st *p_result )
{
   if ( hdrLen != 8 )
   {
      return NAT_FALSE;
   }
   p_result->pad = *body++;
   p_result->family = *body++;
   if (p_result->family == IPV4FAMILY)
   {
      nat_uint16_t nport;
      nat_uint32_t naddr;
      nat_uint32_t addr;


      nat_port_memcpy(&nport, body, 2); body+=2;
      p_result->addr.port = ntohs(nport);
		
      
      nat_port_memcpy(&naddr, body, 4); body+=4;
      addr = ntohl(naddr);
      stun_convert_data_to_ipaddr(&(p_result->addr.ipv4_addr), &addr);
      return NAT_TRUE;
   }
   else if (p_result->family == IPV6FAMILY)
   {
       /* not supported */
   }
   else
   {
       /* unknown type */
   }
	
   return NAT_FALSE;
}

static nat_boolean_t 
stunParseAtrChangeRequest( nat_int8_t* body, nat_uint32_t hdrLen,  stun_attr_change_req_st result )
{
   if ( hdrLen != 4 )
   {
      return NAT_FALSE;
   }
   else
   {
      nat_port_memcpy(&result.value, body, 4);
      result.value = ntohl(result.value);
      return NAT_TRUE;
   }
}

static nat_boolean_t 
stunParseAtrError( nat_int8_t* body, nat_uint32_t hdrLen,  stun_attr_error_st *p_result )
{

   if ( hdrLen > sizeof(stun_attr_error_st) )
   {
      return NAT_FALSE;
   }
   else
   {
      nat_port_memcpy(&p_result->pad, body, 2); body+=2;
      p_result->pad = ntohs(p_result->pad);
      p_result->error_class = *body++;
      p_result->number = *body++;
		
      p_result->reason.str_len = (nat_uint16_t )(hdrLen - 4);
      if(p_result->reason.str_len -1 >= NAT_MAX_STR_LEN)
      {
	  p_result->reason.str_len = NAT_MAX_STR_LEN -2 ;
      }
      nat_port_memcpy(&p_result->reason.str, body, p_result->reason.str_len);
      p_result->reason.str[p_result->reason.str_len +1] = '\0';
      return NAT_TRUE;
   }
}

static nat_boolean_t 
stunParseAtrUnknown( nat_int8_t* body, nat_uint32_t hdrLen,  stun_attr_unknown_st result )
{
   nat_uint8_t i=0;

   if ( hdrLen >= sizeof(result) )
   {
      return NAT_FALSE;
   }
   else
   {
      if (hdrLen % 4 != 0) return NAT_FALSE;
      result.num_attributes = (nat_uint16_t )(hdrLen / 4);
      for (i=0; i<result.num_attributes; i++)
      {
         nat_port_memcpy(&result.attr_type[i], body, 2); body+=2;
         result.attr_type[i] = ntohs(result.attr_type[i]);
      }
      return NAT_TRUE;
   }
}


static nat_boolean_t 
stunParseAtrString( nat_int8_t* body, nat_uint32_t hdrLen,  nat_string_st *p_result )
{
   if ( hdrLen >= NAT_MAX_OCTET_LEN || hdrLen >= NAT_MAX_STR_LEN)
   {
      return NAT_FALSE;
   }
   else if(hdrLen >= NAT_MAX_STR_LEN)
   {
	return NAT_FALSE;
   }
   else
   {
      if (hdrLen % 4 != 0)
      {
         return NAT_FALSE;
      }
		
      p_result->str_len = (nat_uint16_t )hdrLen;
      nat_port_memcpy(&(p_result->str), body, hdrLen);
      p_result->str[hdrLen] = '\0';
      return NAT_TRUE;
   }
}


static nat_boolean_t
stunParseAtrIntegrity( nat_int8_t* body, nat_uint32_t hdrLen,  stun_attr_integrity_st *p_result )
{
   if ( hdrLen != 20)
   {
      return NAT_FALSE;
   }
   else
   {
      nat_port_memcpy(&(p_result->hash), body, hdrLen);
      p_result->hash[hdrLen] = '\0';
      return NAT_TRUE;
   }
}

/*******************************************************************************
 *
 * FUNCTION:        stun_convert_data_to_ipaddr
 *
 * DESCRIPTION:     This function converts packed uint32 data to IPV4 address.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE              
 *
 ******************************************************************************/  
nat_return_t stun_convert_data_to_ipaddr(
             nat_ipv4_address_st *p_ip_addr, nat_uint32_t *p_data)
{
    p_ip_addr->octet_1    = (nat_uint8_t)(*p_data >> 24);
    p_ip_addr->octet_2    = (nat_uint8_t)(*p_data >> 16);
    p_ip_addr->octet_3    = (nat_uint8_t)(*p_data >> 8);
    p_ip_addr->octet_4    = (nat_uint8_t)(*p_data);

    return NAT_RETURN_SUCCESS;
}


/*******************************************************************************
 *
 * FUNCTION:        stun_decode_message_header
 *
 * DESCRIPTION:     This function decodes the stun message header
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE                
 *
 ******************************************************************************/
nat_return_t stun_decode_message_header( nat_octet_string_st	*p_buffer, 
	               	        stun_msg_hdr		*p_stun_msg_hdr,
  			                nat_error_t	        error)
{

    nat_return_t ret_val = NAT_RETURN_SUCCESS;
	nat_uint8_t *p_temp_buffer = p_buffer->str;
	error = error; 
    nat_port_memcpy(p_stun_msg_hdr, p_temp_buffer, nat_port_sizeof(stun_msg_hdr));
    p_stun_msg_hdr->msg_type = ntohs(p_stun_msg_hdr->msg_type);
    p_stun_msg_hdr->msg_length = ntohs(p_stun_msg_hdr->msg_length);

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_decode_message
 *
 * DESCRIPTION:     This function decodes the stun message
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE                
 *
 ******************************************************************************/
nat_return_t
stun_decode_message( nat_octet_string_st	*p_buffer, 
			         const nat_string_st	password, 	
	                 stun_message_st 	    *p_stun_message,
		             nat_error_t	        *p_error)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
	

    nat_uint8_t *p_temp_buffer = p_buffer->str + nat_port_sizeof(stun_msg_hdr);

    nat_uint32_t size = p_stun_message->msg_hdr.msg_length;
#ifdef NAT_PORT_WINDOWS
	nat_string_st pass = password;
	pass = pass;
#endif
	
   if (nat_port_sizeof(stun_msg_hdr) > p_buffer->octet_len)
   {
      *p_error = NAT_ERR_BAD_MSG_RECVD;
      return NAT_RETURN_FAILURE;
   }
	
   if (size + nat_port_sizeof(stun_msg_hdr) != p_buffer->octet_len)
   {
      *p_error = NAT_ERR_BAD_MSG_RECVD;
      return NAT_RETURN_FAILURE;
   }
	

   while ( size > 0 )
   {	
      stun_attr_hdr* attr = (stun_attr_hdr *)p_temp_buffer;
		
      nat_uint32_t attrLen = ntohs(attr->attr_length);
      nat_uint32_t atrType = (nat_uint32_t)ntohs(attr->attr_type);

	NAT_PRINT(("\n attrLen %d attrType %d size %d",attrLen,atrType,size));
		
      if ( attrLen + 4 > size ) 
      {
         *p_error = NAT_ERR_DECODE_FAILED;
         return NAT_RETURN_FAILURE;
      }
		
      p_temp_buffer += 4; // skip the length and type in attribute header
      size -= 4;
		
      switch ( atrType )
      {
         case MAPPED_ADDRESS:
            p_stun_message->hasMappedAddress = NAT_TRUE;
            if ( stunParseAtrAddress( (nat_int8_t *) p_temp_buffer,  attrLen,  
                    &p_stun_message->mapped_address)== NAT_FALSE )
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
					
            break;  

         case RESPONSE_ADDRESS:
            p_stun_message->hasResponseAddress = NAT_TRUE;
            if ( stunParseAtrAddress(  (nat_int8_t *)p_temp_buffer,  attrLen,  
                    &p_stun_message->response_address )== NAT_FALSE )
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
            break;  
				
         case CHANGE_REQUEST:
            p_stun_message->hasChangeRequest = NAT_TRUE;
            if (stunParseAtrChangeRequest((nat_int8_t *) p_temp_buffer, attrLen, 
                    p_stun_message->change_request) == NAT_FALSE)
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
            break;
				
         case SOURCE_ADDRESS:
            p_stun_message->hasSourceAddress = NAT_TRUE;
            if ( stunParseAtrAddress(  (nat_int8_t *)p_temp_buffer,  attrLen,  
                    &p_stun_message->source_address )== NAT_FALSE )
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
            break;  
				
         case CHANGED_ADDRESS:
            p_stun_message->hasChangedAddress = NAT_TRUE;
            if ( stunParseAtrAddress(  (nat_int8_t *)p_temp_buffer,  attrLen,  
                    &p_stun_message->changed_address )== NAT_FALSE )
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
            break;  
				
         case USERNAME: 
            p_stun_message->hasUsername = NAT_TRUE;
            if (stunParseAtrString( (nat_int8_t *)p_temp_buffer, attrLen, 
                    &(p_stun_message->username)) == NAT_FALSE)
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
					
            break;
				
         case PASSWORD: 
            p_stun_message->hasPassword = NAT_TRUE;
            if (stunParseAtrString( (nat_int8_t *)p_temp_buffer, attrLen, 
                    &(p_stun_message->password)) == NAT_FALSE)
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
            break;
				
         case MESSAGE_INTEGRITY:
            p_stun_message->hasMessageIntegrity = NAT_TRUE; //break
            if (stunParseAtrIntegrity( (nat_int8_t *)p_temp_buffer, attrLen, 
                    &(p_stun_message->message_integrity)) == NAT_FALSE)
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
		p_stun_message->message_integrity.hash[NAT_STUN_HMAC_DIGEST_LENGTH]='\0';
            }
					
					
            // read the current HMAC
            // look up the password given the user of given the transaction id 
            // compute the HMAC on the buffer
            // decide if they match or not
            break;
				
         case ERROR_CODE:
            p_stun_message->hasErrorCode = NAT_TRUE;
            if (stunParseAtrError((nat_int8_t *)p_temp_buffer, attrLen, 
                    &(p_stun_message->error_code)) == NAT_FALSE)
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
					
            break;
				
         case UNKNOWN_ATTRIBUTE:
            p_stun_message->hasUnknownAttributes = NAT_TRUE;
            if (stunParseAtrUnknown((nat_int8_t *)p_temp_buffer, attrLen, 
                    p_stun_message->unknown_attributes) == NAT_FALSE)
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
            break;
				
         case REFLECTED_FROM:
            p_stun_message->hasReflectedFrom = NAT_TRUE;
            if ( stunParseAtrAddress(  (nat_int8_t *)p_temp_buffer,  attrLen,  
                    &p_stun_message->reflected_from ) == NAT_FALSE )
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
            break;  
				
         case XOR_MAPPED_ADDRESS:
            p_stun_message->hasXorMappedAddress = NAT_TRUE;
            if ( stunParseAtrAddress(  (nat_int8_t *)p_temp_buffer,  attrLen,  
                    &p_stun_message->xor_mapped_address ) == NAT_FALSE )
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
            break;  

         case XOR_ONLY:
            p_stun_message->xorOnly = NAT_TRUE;
            
            break;  
				
         case SERVER_NAME: 
            p_stun_message->hasServerName = NAT_TRUE;
            if (stunParseAtrString( (nat_int8_t *)p_temp_buffer, attrLen, 
                    &(p_stun_message->server_name)) == NAT_FALSE)
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
            break;
				
         case SECONDARY_ADDRESS:
            p_stun_message->hasSecondaryAddress = NAT_TRUE;
            if ( stunParseAtrAddress( (nat_int8_t *) p_temp_buffer,  attrLen,  
                    &p_stun_message->secondary_address ) == NAT_FALSE )
            {
                *p_error = NAT_ERR_DECODE_FAILED;
                return NAT_RETURN_FAILURE;
            }
            else
            {
               /* ADD PRINTING TRACE */
            }
            break;  
					
         default:
            if ( atrType <= 0x7FFF ) 
            {
               return NAT_RETURN_FAILURE;
            }
      }
		
      p_temp_buffer += attrLen;
      size -= attrLen;
   }
    
    
    return ret_val;
}
