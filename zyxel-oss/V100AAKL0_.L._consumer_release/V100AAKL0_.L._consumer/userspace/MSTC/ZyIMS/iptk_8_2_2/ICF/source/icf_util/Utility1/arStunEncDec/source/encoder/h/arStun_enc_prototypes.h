/******************************************************************************
*                                    
* FILE NAME    : arStun_enc_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for function prototypes. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Jan-2009    Anuradha Gupta         None          Initial
* 07-Jan-2009    Abbas Haque            None          Added proto types for Stun encoder
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARSTUN_ENC_PROTOTYPES_H__
#define __ARSTUN_ENC_PROTOTYPES_H__

#include "common_types.h"
#include "common_defs.h"
#include "arStunEncDec_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
/****************************************************************************
 * API Name: arStun_encode_U8
 * 
 * Description:    This function shall be  used to encode 1 byte data 
 * INPUT       : data is the data to be encoded .
 *             :p_length  length of data that is to be encoded
 * OUTPUT      :p_dest is the varible to copy encoded data
 *
 * Return :It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.

 ******************************************************************************/

util_void_t
arStun_encode_U8
(
   util_uint8_t     *p_dest,
util_uint8_t     data,
util_uint32_t    *p_length);

/****************************************************************************
 * API Name: util_encode_U16
 * 
 * Description:    This function shall be  used to encode 2 byte data 
 * INPUT       : data is the data to be encoded .
 * OUTPUT      :p_length  length of data that has been encoded. 
 *             :p_dest is the varible to copy encoded data
 *
 * Return :It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.

 ******************************************************************************/

util_void_t
arStun_encode_U16
(
    util_uint8_t    *p_dest,
    util_uint16_t     data,
    util_uint32_t    *p_length);

/****************************************************************************
 * API Name: util_encode_U32
 * 
 * Description:    This function shall be  used to encode 4 byte data 
 * INPUT       : data is the data to be encoded .
 * OUTPUT      :p_length  length of data that has been encoded. 
 *             :p_dest is the varible to copy encoded data
 *
 * Return :It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/

 util_void_t
arStun_encode_U32
(
    util_uint8_t     *p_dest,
    util_uint32_t     data,
    util_uint32_t    *p_length);

/****************************************************************************
 * API Name: arStun_encode_ipv4_addr
 * 
 * Description:    This function shall be  used to encode ip address attribute type 
 * INPUT       : type is the attribute type.
 *             : atr is the IP address 
 * OUTPUT      :p_msg_buffer  buffer conatining encoded error attribute. 
 *
 * Return :
 ******************************************************************************/

util_void_t  
arStun_encode_ipv4_addr 
(
    util_uint8_t          **pp_msg_buffer, 
    util_uint16_t 	  type, 
    arStun_attr_addr_st atr);


/****************************************************************************
 * API Name: arStun_encode_change_req
 * 
 * Description:    This function shall be  used to encode change req attribute type 
 * INPUT       : type is the attribute type.
 *             : atr is the change attribute value.
 * OUTPUT      :p_msg_buffer  buffer conatining encoded error attribute. 
 *
 * Return :
 ******************************************************************************/

util_void_t  
arStun_encode_change_req 
(
    util_uint8_t 		 **pp_msg_buffer, 
    util_uint16_t 	         type, 
    arStun_attr_change_req_t     atr);


/****************************************************************************
 * API Name: arStun_encode_string
 * 
 * Description:    This function shall be  used to encode string attribute type 
 * INPUT       : type is the attribute type.
 *             : str is the string attribute 
 * OUTPUT      :p_msg_buffer  buffer conatining encoded error attribute. 
 *
 * Return :
 ******************************************************************************/

util_void_t  
arStun_encode_string 
(
    util_uint8_t 		   **pp_msg_buffer, 
    util_uint16_t 	          type, 
    arStun_string_st              str);


/****************************************************************************
 * API Name: arStun_encode_unknown_attr
 * 
 * Description:    This function shall be  used to encode unknown attribute type 
 * INPUT       : type is the attribute type.
 *             : str is the unknown attribute 
 * OUTPUT      :p_msg_buffer  buffer conatining encoded error attribute. 
 *
 * Return :
 ******************************************************************************/

util_void_t  
arStun_encode_unknown_attr 
(
    util_uint8_t 		  **pp_msg_buffer, 
    util_uint16_t 	          type, 
    arStun_attr_unknown_st        attr);




/****************************************************************************
 * API Name: arStun_convert_ipaddr_to_data
 * 
 * Description :This function shall be  used to convert ip address into 4 bytes.  
 * INPUT       : ipv4_addr IP address.
 * OUTPUT      :ip_data 4 bytes converted ip ddress. 
 *
 * Return : 
 ******************************************************************************/

util_void_t  
arStun_convert_ipaddr_to_data 
(
    arStun_ipv4_address_st	  ipv4_addr, 
    util_uint32_t 	          *p_ip_data );



/****************************************************************************
 * API Name: arStun_encode_error_attr
 * 
 * Description: This function shall be  used to encode error attribute type 
 * INPUT       : type is the attribute type.
 *             : atr is the error attribute 
 * OUTPUT      :p_msg_buffer  buffer conatining encoded error attribute. 
 *
 * Return :
 ******************************************************************************/

util_void_t  
arStun_encode_error_attr 
(
    util_uint8_t 		  **pp_msg_buffer, 
    util_uint16_t 	          type, 
    arStun_attr_error_st          attr);



/****************************************************************************
 * API Name: arStun_generate_transactionID
 * 
 * Description:    This function shall be  used to generate transaction id 
 * INPUT       : .
 * OUTPUT      :pTransactionID  is transaction id. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/

util_void_t
arStun_generate_transactionID (
      INOUT arStun_uint128_t      *pTransactionID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARSTUN_ENC_PROTOTYPES_H__ */



