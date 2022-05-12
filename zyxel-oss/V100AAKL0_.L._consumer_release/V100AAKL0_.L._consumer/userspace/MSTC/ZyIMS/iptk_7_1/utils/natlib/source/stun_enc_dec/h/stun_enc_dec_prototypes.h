/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_enc_dec_prototypes.h 
 * 
 * DESCRIPTION: This file contains the prototypes of the functions used 
 *              to implement the functionality of STUN ENCODER.
 *              
 * REVISION HISTORY:
 *         Date            Name            Reference       Comments
 * ----------------------------------------------------------------------------
 *     18_Jul-2006    Anuradha Gupta       LLD         Coding
 *	   24-Nov-2006	  Anuradha Gupta       LLD			Coding
 *
 * Copyright 2006, Aricent                        
 * ******************************************************************************/

#ifndef __STUN_ENC_DEC_PROTOTYPES_H__
#define __STUN_ENC_DEC_PROTOTYPES_H__

#include "nat_common_types.h"
#include "stun_enc_dec_types.h"


#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */
    

/*******************************************************************************
 *
 * FUNCTION:        stun_encode_message
 *
 * DESCRIPTION:     This function encodes the stun message
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE                
 *
 ******************************************************************************/
nat_return_t
stun_encode_message( stun_message_st 		stun_message, 
                     nat_octet_string_st	*p_buffer, 
                     const nat_string_st	password,
					nat_error_t		    error,
		     nat_uint8_t 		*p_msg_integrity);

/*******************************************************************************
 *
 * FUNCTION:        stun_decode_message_header
 *
 * DESCRIPTION:     This function decodes the stun message header
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE                
 *
 ******************************************************************************/
nat_return_t
stun_decode_message_header( nat_octet_string_st	*p_buffer, 
	               	        stun_msg_hdr		*p_stun_msg_hdr,
  			                nat_error_t	        error);

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
		             nat_error_t	        *p_error);
		             
/*******************************************************************************
 *
 * FUNCTION:        stun_create_bind_req
 *
 * DESCRIPTION:     This function creates binding request message in encode
 *                  routine format
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE                
 *
 ******************************************************************************/
nat_return_t
stun_create_bind_req(
                     stun_message_st 		*p_stun_message,
	                 nat_string_st	username,
                     nat_boolean_t			change_port,
			         nat_boolean_t			change_ip,
			         nat_ip_port_address_st	*p_response_addr
			   );

/*******************************************************************************
 *
 * FUNCTION:        stun_create_shsec_req
 *
 * DESCRIPTION:     This function creates shared secret request message in 
 *                  encode routine format
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE                
 *
 ******************************************************************************/
nat_return_t
stun_create_shsec_req( stun_message_st 		*p_stun_message);


/*******************************************************************************
 *
 * FUNCTION:        stun_check_msg_integrity
 *
 * DESCRIPTION:     This function checks for Message integrity
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE                
 *
 ******************************************************************************/
nat_return_t 
stun_check_msg_integrity(
			const nat_string_st		password,
			nat_octet_string_st	*p_buffer
			);
 
/*******************************************************************************
 *
 * FUNCTION:        stun_computeHmac
 *
 * DESCRIPTION:     This function is for computing HMAC.
 *                  
 * RETURNS:         nothing               
 *
 ******************************************************************************/    
nat_void_t
stun_computeHmac(nat_uint8_t* hmac, const nat_uint8_t* input, nat_uint32_t length, 
                         const nat_uint8_t* key, nat_uint8_t sizeKey);
   
/*******************************************************************************
 *
 * FUNCTION:        stunRand
 *
 * DESCRIPTION:     Random number generator
 *                  
 * RETURNS:         Random number             
 *
 ******************************************************************************/ 
nat_uint32_t stunRand();

/*
nat_uint64_t stun_get_system_time_secs(); 

void
toHex(const nat_int8_t* buffer, nat_uint8_t bufferSize, nat_int8_t* output);
*/

/*******************************************************************************
 *
 * FUNCTION:        stun_convert_ipaddr_to_data
 *
 * DESCRIPTION:     This function converts IPV4 address to packed uint32 data
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE              
 *
 ******************************************************************************/
nat_return_t stun_convert_ipaddr_to_data(
             const nat_ipv4_address_st ip_addr, nat_uint32_t *p_data);
    
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
             nat_ipv4_address_st *p_ip_addr, nat_uint32_t *p_data);


/*******************************************************************************
 *
 * FUNCTION:        stun_create_binding_err_resp
 *
 * DESCRIPTION:     This function creates binding error response message in encode
 *                  routine format
 *
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE
 *
 ******************************************************************************/
nat_return_t
stun_create_binding_err_resp(nat_octet_string_st **p_buffer,
                             nat_uint128_t         transaction_id,
                             stun_attr_error_st    *error_code,
                         stun_attr_unknown_st  *unknown_attributes,
                             nat_string_st         password);

/*******************************************************************************
 *
 * FUNCTION:        stun_create_binding_resp
 *
 * DESCRIPTION:     This function creates binding response message in encode
 *                  routine format
 *
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE
 *
 ******************************************************************************/
nat_return_t
stun_create_binding_resp(nat_octet_string_st **p_buffer,
                         nat_uint128_t         transaction_id,
                         stun_attr_address_st  *changed_address,
                         stun_attr_address_st  *source_address,
                         stun_attr_address_st  *mapped_address,
                         stun_attr_address_st  *reflected_from,
                         nat_string_st         password);
             
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __STUN_ENC_DEC_PROTOTYPES_H__ */



