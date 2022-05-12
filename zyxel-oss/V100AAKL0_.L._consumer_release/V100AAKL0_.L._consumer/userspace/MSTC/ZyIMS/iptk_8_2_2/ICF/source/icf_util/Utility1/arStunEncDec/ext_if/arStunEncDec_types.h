/******************************************************************************
*                                    
* FILE NAME    : arStunEncDec_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the data types. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Jan-2009    Anuradha Gupta         None          Initial
* 07-Jan-2009    Abbas Haque            None          Data structure for stun added 
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARSTUNENCDEC_TYPES_H__
#define __ARSTUNENCDEC_TYPES_H__

#include "arStunEncDec_defs.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
/*This is the stun message header*/  

typedef struct { util_uint8_t octet[16]; }  arStun_uint128_t;

typedef util_uint32_t  arStun_attr_change_req_t;

typedef struct 
{
      util_uint16_t         msg_type;
      util_uint16_t          msg_length;
      arStun_uint128_t      transaction_id;
} arStun_msg_hdr_st;

/*This is the stun message contains the IPV4 address */
typedef struct
{
    util_uint8_t            octet_1;
    util_uint8_t            octet_2;
    util_uint8_t            octet_3;
    util_uint8_t            octet_4;
}arStun_ipv4_address_st;

/*This is the stun message ip port structure*/
typedef struct
{
    arStun_ipv4_address_st   ipv4_address;
    util_uint16_t            port;
    util_uint16_t            padding;
}arStun_ip_port_address_st;

/*This is the stun message IP addrtess structure*/  
typedef struct
{
      util_uint8_t                  padding;
      util_uint8_t                  ipfamily;
      arStun_ip_port_address_st     ipaddr;
} arStun_attr_addr_st;

/*This is the stun message string structure*/
typedef struct        
{
    util_uint16_t  str_len;
    util_uint8_t   str_val[ARSTUN_MAX_STR_LEN];
} arStun_string_st;

/*This is the stun message error structure*/  
typedef struct
{
      util_uint16_t                padding; 
      util_uint8_t                 error_class;
      util_uint8_t                 number;
      arStun_string_st             reason;
} arStun_attr_error_st;


/*This is the stunencdec  error codes */
typedef enum
{
     ARSTUN_ERROR_START = 0,
     ARSTUN_ERROR_INVALID_STUN_MSG_STRUCT,
     ARSTUN_ERROR_INVALID_MSG_BUFFER,
     ARSTUN_ERROR_INVALID_STUN_MSG_HDR,
     ARSTUN_ERROR_BAD_STUN_MSG,
     ARSTUN_ERROR_END
} arStun_ecode_et;;


/*This is the stun message attributes header structure*/
typedef struct
{
     util_uint16_t   attributes_type;
     util_uint16_t   attribute_length;
} arStun_attr_hdr_st;

/*This is the stun message unknown attributes structure*/
 
typedef struct
{
  util_uint16_t  attr_type[ARSTUN_MAX_UNKNOWN_ATTRIBUTES];
  util_uint8_t   num_attributes;
} arStun_attr_unknown_st;



/*This is the stun message buffer structure*/

typedef struct
{
    util_uint16_t msg_len;
    util_uint8_t  msg_val[ARSTUN_MAX_MSG_LEN];
}arStun_msg_buffer_st;

/*This is message integrity structure*/
typedef struct
{
    util_uint8_t     integrity_val[ARSTUN_MSG_INTEGRITY_LEN];
 } arStun_msg_intregity_st;

/*This is the stun message structure*/  
 
typedef struct
{
    util_uint32_t                          msg_attrib_presence;
#define  ARSTUN_MAPPED_ADDRESS_PRESENT       0x0001
#define  ARSTUN_RESPONSE_ADDRESS_PRESENT     0x0002
#define  ARSTUN_CHANGE_REQUEST_PRESENT       0x0004
#define  ARSTUN_SOURCE_ADDRESS_PRESENT       0x0008
#define  ARSTUN_CHANGED_ADDRESS_PRESENT      0x0010
#define  ARSTUN_USER_NAME_PRESENT            0x0020
#define  ARSTUN_ERROR_CODE_PRESENT           0x0040
#define  ARSTUN_UNKNOWN_ATTR_PRESENT         0x0080
#define  ARSTUN_REFLECTED_FROM_PRESENT       0x0100
#define  ARSTUN_PASSWORD_PRESENT             0x0200
#define  ARSTUN_MSG_INTREGITY_PRESENT        0x0400
    arStun_msg_hdr_st                       msg_hdr;
    arStun_attr_addr_st                     mapped_addr;
    arStun_attr_addr_st                     response_addr;
    arStun_attr_change_req_t                change_request;
    arStun_attr_addr_st                     source_addr;
    arStun_attr_addr_st                     changed_addr;
    arStun_string_st                        username;
    arStun_attr_error_st                    error_code;
    arStun_attr_unknown_st                  unknown_attributes;
    arStun_attr_addr_st                     reflected_from;
    arStun_string_st                        password;
    arStun_msg_intregity_st                 msg_intregity;
} arStun_msg_st;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARSTUNENCDEC_TYPES_H__ */



