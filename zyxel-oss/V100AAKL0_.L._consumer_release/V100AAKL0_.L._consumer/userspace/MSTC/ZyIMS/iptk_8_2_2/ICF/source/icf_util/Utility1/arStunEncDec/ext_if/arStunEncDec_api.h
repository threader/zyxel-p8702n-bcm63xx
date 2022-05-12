/******************************************************************************
*                                    
* FILE NAME    : arStunEncDec_api.h                
*                                    
* DESCRIPTION  : Contains the definitions for the interfaces. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Jan-2009    Anuradha Gupta           None          Initial
* 07-Jan-2009    Abbas Haque              None          Stun API added
*                                                                
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARSTUNENCDEC_API_H__
#define __ARSTUNENCDEC_API_H__

#include "common_types.h"
#include "common_defs.h"
#include "arStunEncDec_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
 /****************************************************************************
 * API Name: arStun_create_binding_req
 * 
 * Description: This API is invoked by the application create a STUN 
 *               :binding request message structure. 
 *		 :The message structure will be used for encoding the binding request message.
 * INPUT       :arStun_string_st is the user name.
 *             :change_port_req is the change port req.
 *             :change_ip_req is the change ip req.
 *             :pResponseAddr is the response address.
 * OUTPUT      :arStun_msg_st  is stun message. 
 *             :pError is the error code. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
util_return_t
arStun_create_binding_req(
             OUT arStun_msg_st             *pStunMessage,
             IN  arStun_string_st          *pUsername,
             IN  util_boolean_t            isChangePortReq,
             IN  util_boolean_t            isChangeIpReq,
             IN  arStun_ip_port_address_st *pResponseAddr,
             OUT arStun_ecode_et           *pError);


 /****************************************************************************
 * API Name: arStun_create_binding_resp
 * 
 * Description: This API is invoked to to create a STUN binding response structure. 
 *      	:The message structure will be used for encoding the success binding 
 *              :response message.
 * INPUT       :pTransactionID is the transaction id.
 *             :pChanged_address is the change address.
 *             :pSource_address is the source address.
 *             :pMapped_address is the mapped address.
 *             :pReflected_from is the reflected from address.
 * OUTPUT      :arStun_msg_st  is stun message. 
 *             :pError is the error code. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
util_return_t
arStun_create_binding_resp(
          OUT    arStun_msg_st          *pStunMessage,
          IN     arStun_uint128_t       *pTransactionID,
          IN     arStun_attr_addr_st    *pChangedAddress,
          IN     arStun_attr_addr_st    *pSourceAddress,
          IN     arStun_attr_addr_st    *pMappedAddress,
          IN     arStun_attr_addr_st    *pReflectedFrom,
          OUT    arStun_ecode_et        *pError);


/****************************************************************************
 * API Name: arStun_create_binding_err_resp
 * 
 * Description: This API is invoked to create the STUN Binding Error response message.
 * INPUT       :pTransactionID is the transcation id.
 *             :pError_code is the error response in binding message.
 *             :pUnknown_attribs is the unknown attributes.
 * OUTPUT      :pStunMessage  is stun message. 
 *             :pError is the error code. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
util_return_t
arStun_create_binding_err_resp(
     OUT arStun_msg_st                *pStunMessage,
     IN   arStun_uint128_t            *pTransactionID,
     IN   arStun_attr_error_st        *pErrorCode,
     IN   arStun_attr_unknown_st      *pUnknownAttribs,
     OUT  arStun_ecode_et             *pError);


/****************************************************************************
 * API Name: arStun_encode_message
 * 
 * Description: This API is invoked to encode the stun message.
 * INPUT       :pStunMessage is the stun message.
 * OUTPUT      :pMsgBuffer  is encoded message. 
 *             :pError is the error code. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
util_return_t
arStun_encode_message(
           IN   arStun_msg_st         *pStunMessage,
           IN   arStun_string_st      *pPassword, 
           OUT  arStun_msg_buffer_st   *pMsgBuffer, 
           OUT  arStun_ecode_et       *pError);

/****************************************************************************
 * API Name: arStun_decode_message
 * 
 * Description: This API is invoked to decode the stun message.
 * INPUT       :pMsgBuffer is the message buffer.
 * OUTPUT      :pStun_message  is stun message. 
 *             :pError is the error code. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
util_return_t
arStun_decode_message( 
           IN   arStun_msg_buffer_st      *pMsgBuffer, 
           OUT  arStun_msg_st            *pStunMessage,
           OUT  arStun_ecode_et          *pError);

/****************************************************************************
 * API Name: arStun_decode_message_header
 * 
 * Description: This API is invoked to decode the message header
 * INPUT       :pMsgBuffer is the message buffer.
 * OUTPUT      :pStun_msg_hdr  is stun message header. 
 *             :pError is the error code. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
util_return_t 
arStun_decode_message_header( 
                 IN  arStun_msg_buffer_st  *pMsgBuffer, 
                 OUT arStun_msg_hdr_st     *pStunMsgHdr,
                 OUT arStun_ecode_et       *pError);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARSTUNENCDEC_API_H__ */



