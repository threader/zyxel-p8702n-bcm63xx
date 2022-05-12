/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_enc_dec_types.h 
 * 
 * DESCRIPTION: This file contains the C structures used in the  
 *		    STUN TLV decoder 
 *              
 * REVISION HISTORY:
 *         Date            Name            Reference       Comments
 * ----------------------------------------------------------------------------
 *     18_Jul-2006    Anuradha Gupta       LLD         Coding
 *	   24-Nov-2006	  Anuradha Gupta       LLD			Coding
 *
 * Copyright 2006, Aricent                     
 ******************************************************************************/

#ifndef __STUN_ENC_DEC_TYPES_H__
#define __STUN_ENC_DEC_TYPES_H__

#include "nat_common_types.h"
#include "nat_common_defs.h"
#include "nat_types.h"

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */
  

/* if you change this version, change in makefile too */
#define NAT_STUN_VERSION "0.96"

#define NAT_STUN_MAX_STRING 			256
#define NAT_STUN_MAX_UNKNOWN_ATTRIBUTES 	8
#define NAT_STUN_MAX_MESSAGE_SIZE 		2048

#define NAT_STUN_PORT 				3478


#define NAT_STUN_HMAC_UNKNOWN 		0x00,
#define NAT_STUN_HMAC_OK 			0x01
#define NAT_STUN_HMAC_BAD_USRNAME		0x02
#define NAT_STUN_HMAC_UNKNOWN_USRNAME 	0x03
#define NAT_STUN_HMAC_FAILED	 		0x04

/* define macros to hold a stun address */
#define IPV4FAMILY          0x01
#define IPV6FAMILY          0x02    

/* define  flags  */
#define CHANGE_IPFLAG      0x04
#define CHANGE_PORTFLAG    0x02

/* define  stun attribute */
#define MAPPED_ADDRESS      0x0001
#define RESPONSE_ADDRESS    0x0002
#define CHANGE_REQUEST      0x0003
#define SOURCE_ADDRESS      0x0004
#define CHANGED_ADDRESS     0x0005
#define USERNAME            0x0006
#define PASSWORD            0x0007
#define MESSAGE_INTEGRITY   0x0008
#define ERROR_CODE          0x0009
#define UNKNOWN_ATTRIBUTE   0x000A
#define REFLECTED_FROM      0x000B
#define XOR_MAPPED_ADDRESS  0x8020
#define XOR_ONLY            0x0021
#define SERVER_NAME         0x8022
#define SECONDARY_ADDRESS   0x8050 /* Non standard extention */

/* define types for a stun message */
typedef enum
{
        BindRequestMsg = 0x0001,
        BindResponseMsg = 0x0101,
        BindErrorResponseMsg = 0x0111,
        SharedSecretRequestMsg = 0x0002,
        SharedSecretResponseMsg = 0x0102,
        SharedSecretErrorResponseMsg = 0x0112
} stun_mesg_type;

typedef struct 
{
      nat_uint16_t  msg_type;
      nat_uint16_t  msg_length;
      nat_uint128_t transaction_id;
} stun_msg_hdr;


typedef struct
{
      nat_uint16_t attr_type;
      nat_uint16_t attr_length;
} stun_attr_hdr;


typedef struct
{
      nat_uint32_t value;
} stun_attr_change_req_st;

typedef struct
{
      nat_uint8_t	 		pad;
      nat_uint8_t  		family;
	nat_ip_port_address_st 	addr;
} stun_attr_address_st;

typedef struct
{
      nat_uint16_t pad; 
      nat_uint8_t  error_class;
      nat_uint8_t  number;
      nat_string_st reason;
} stun_attr_error_st;

typedef struct
{
      nat_uint16_t attr_type[NAT_STUN_MAX_UNKNOWN_ATTRIBUTES];
      nat_uint16_t num_attributes;
} stun_attr_unknown_st;


typedef struct
{
      nat_uint8_t	 hash[NAT_STUN_HMAC_DIGEST_LENGTH+1];
} stun_attr_integrity_st;

typedef struct
{
      stun_msg_hdr 		msg_hdr;
	
      nat_boolean_t		hasMappedAddress;
      stun_attr_address_st    mapped_address;
	
      nat_boolean_t		hasResponseAddress;
      stun_attr_address_st    response_address;
	
      nat_boolean_t		hasChangeRequest;
      stun_attr_change_req_st 	change_request;
	
      nat_boolean_t		hasSourceAddress;
      stun_attr_address_st 	source_address;
	
      nat_boolean_t		hasChangedAddress;
      stun_attr_address_st 	changed_address;
	
      nat_boolean_t		hasUsername;
      nat_string_st 		username;
	
      nat_boolean_t		hasPassword;
      nat_string_st 		password;
	
      nat_boolean_t		hasMessageIntegrity;
      stun_attr_integrity_st	message_integrity;
	
      nat_boolean_t		hasErrorCode;
      stun_attr_error_st 		error_code;
	
      nat_boolean_t		hasUnknownAttributes;
      stun_attr_unknown_st 	unknown_attributes;
	
      nat_boolean_t		hasReflectedFrom;
      stun_attr_address_st 	reflected_from;

      nat_boolean_t		hasXorMappedAddress;
      stun_attr_address_st  	xor_mapped_address;
	
      nat_boolean_t		xorOnly;

      nat_boolean_t		hasServerName;
      nat_string_st 		server_name;
      
      nat_boolean_t		hasSecondaryAddress;
      stun_attr_address_st 	secondary_address;
} stun_message_st; 




#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __STUN_ENC_DEC_TYPES_H__ */



