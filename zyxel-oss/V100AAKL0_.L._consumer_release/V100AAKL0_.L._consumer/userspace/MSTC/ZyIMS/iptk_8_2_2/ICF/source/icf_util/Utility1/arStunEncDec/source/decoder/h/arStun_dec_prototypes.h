/******************************************************************************
*                                    
* FILE NAME    : arStun_dec_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for function prototypes. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERDECE       REASON        
* ------         ------------           ---------     ----------
* 05-Jan-2009    Anuradha Gupta         None          Initial
* 07-Jan-2009    Shweta Jain            LLD           arStunEncDec 1.0     
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARSTUN_DEC_PROTOTYPES_H__
#define __ARSTUN_DEC_PROTOTYPES_H__

#include "common_types.h"
#include "arStunEncDec_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

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
   util_uint8_t     *pOutput);

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
    util_uint16_t   *pOutput);

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
    util_uint32_t    *pOutput);
   

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
    util_uint32_t     length);

/*****************************************************************************
 * FUNCTION :   arStun_decode_attrib_address 
 *
 * Description: This function is used to decode the address related attributes  
 *              of the stun message.
 *              
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/

 util_return_t   arStun_decode_attrib_address ( 
                                  util_uint8_t*              msg_buffer, 
                                  util_uint32_t              attrib_Len, 
                                  arStun_attr_addr_st        *pOutput,
                                  arStun_ecode_et            *pError);



/*****************************************************************************
 * FUNCTION :   arStun_decode_attrib_changeRequest 
 *
 * Description: This function is used to decode the change_Request attribute of 
 *              the stun message.
 *              
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/

 util_return_t   arStun_decode_attrib_changeRequest ( 
                                    util_uint8_t*              msg_buffer,
                                    util_uint32_t              attrib_Len, 
                                    arStun_attr_change_req_t   *pOutput, 
                                    arStun_ecode_et            *pError);



/*****************************************************************************
 * FUNCTION :   arStun_decode_attrib_error 
 *
 * Description: This function is used to decode the error attribute of 
 *              the stun message.
 *              
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/

 util_return_t    arStun_decode_attrib_error ( 
                               util_uint8_t*             msg_buffer, 
                               util_uint32_t             attrib_Len,  
                               arStun_attr_error_st      *pOutput,
                               arStun_ecode_et           *pError);

/*****************************************************************************
 * FUNCTION :   arStun_decode_attrib_unknown
 *
 * Description: This function is used to decode the unknown attribute of 
 *              the stun message.
 *              
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/

 util_return_t     arStun_decode_attrib_unknown ( 
                                util_uint8_t*              msg_buffer, 
                                util_uint32_t              attrib_Len, 
                                arStun_attr_unknown_st     *pOutput,
                                arStun_ecode_et            *pError);


/*****************************************************************************
 * FUNCTION :   arStun_decode_attrib_string
 *
 * Description: This function is used to decode the string related attributes 
 *              of the stun message.
 *              
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/

 util_return_t      arStun_decode_attrib_string ( 
                                    util_uint8_t*             msg_buffer, 
                                    util_uint32_t             attrib_Len, 
                                    arStun_string_st          *pOutput,
                                    arStun_ecode_et           *pError);


/*****************************************************************************
 * FUNCTION :   arStun_convert_data_to_ipaddr
 *
 * Description: This function is used to decode the string related attributes 
 *              of the stun message.
 *              
 * Return:      It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ********************************************************************************/

 util_return_t     arStun_convert_data_to_ipaddr (
                             arStun_ipv4_address_st       *p_ipv4_addr,
                             util_uint32_t                *p_packed_data);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARSTUN_DEC_PROTOTYPES_H__ */



