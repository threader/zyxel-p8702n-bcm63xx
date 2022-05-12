/*******************************************************************************
*    										
* FILE NAME    :icf_clib.h
*										
* DESCRIPTION  :The wrapper header file to the client library
*										
* Revision History :				
*				
*    DATE            NAME         REFERENCE       REASON
* --------    ------------        ----         ----------
* 8-JUL-2005   Ram Narayan        None         Initial
* 9-Jul-2009   Anuradha Gupta     SPR 18948    Modified prototype for
*                                                icf_encode_api_payload  										
* Copyright 2009, Aricent.   					
*										
*******************************************************************************/



#ifndef __ICF_CLIB_H__
#define __ICF_CLIB_H__



/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "icf_types.h"




/* A version of malloc to provide the memory nedded by the application */
#define ICF_CLIB_MEMGET(n) icf_memget(n);

/* A version of free to release the memory allocated using icf_memget */
#define ICF_CLIB_MEMFREE(ptr) icf_memfree(ptr);

/* A wrapper around the standard C library memcpy function */
#define ICF_CLIB_MEMCPY(dst, src, n) icf_memcpy(dst, src, n);



/* These are the porting function  to be implemented by the client */
icf_void_t* icf_memget(icf_uint32_t size);

icf_void_t icf_memfree(icf_void_t* ptr);

icf_void_t* icf_memcpy(icf_void_t* dst, icf_void_t* src, icf_uint32_t len);



/****************************************************************************
*	This function is used to encode icf_msg_st into a byte buffer  
* The pointer to input buffer is provided as an argument.          
* The memory associated with the input buffer is released by ICF.  
* The memory allocated by the ICF for encoded_buffer(output) has   
* to be released by the Application.								
*****************************************************************************/

icf_return_t icf_encode_msg(icf_msg_st* p_msg, /*Input*/
							icf_uint8_t** p_p_encoded_buf,
							icf_uint32_t* p_encoded_buf_length,
							icf_error_t* p_ecode,
                                                        icf_vldt_err_st*  p_vldt_err);






/****************************************************************************
* This function releases the memory associated with the encoded buffer. 
*****************************************************************************/ 

icf_return_t icf_free_encoded_msg(icf_uint8_t* p_encoded_buf, icf_error_t* p_ecode);





/****************************************************************************
* This function is used to convert the byte buffer received from the		
* ICF to appropriate icf_message_type (User API).							
* The memory for icf_message_st is allocated by ICF which has to be freed	
* by the Application.														
* The encoded buffer (p_encoded_buf) is freed by ICF after decoding.		
*****************************************************************************/
icf_return_t icf_decode_msg(icf_msg_st**  p_p_msg, /*Output*/
							icf_uint8_t*  p_encoded_buf,
							icf_uint32_t  p_encoded_buf_length,
							icf_error_t*  p_ecode);


/* SPR 18948 :
 * This function would take as input the message and would copy the
 * encoded buffer (only the message payload part)
 * in the p_encoded_buf . Would return failure in case
 * there was come error in encoding. This function will also call the 
 * VLDT functions for validating the message. VLDT error will be 
 * populated in the p_vldt_err string.
 */
icf_return_t icf_encode_api_payload(icf_msg_st*   p_msg, /*Input*/
							icf_uint8_t** p_p_encoded_buf, /*Output*/
							icf_uint32_t* p_encoded_buf_length,
							icf_error_t*  p_ecode,
                            icf_vldt_err_st*  p_vldt_err);
/* 
 * The function take the encoded buffer as input and returns api_header
 * present in encoded buffer  as output. This header returned is no more
 * enceoded
 */ 
icf_return_t icf_decode_api_hdr(icf_api_header_st *p_api_hdr, /*Output*/
							icf_uint8_t*  p_encoded_buf,
							icf_uint32_t  encoded_buf_length,
							icf_error_t*  p_ecode);

/* 
 * This function would take as input the encoded payload and would copy the
 * decoded payload in the p_msg pointer . Would return failure in case
 * there was come error in decoding
 */ 
icf_return_t icf_decode_msg_payload(icf_uint8_t*  p_msg, /*Output*/
							icf_uint8_t*  p_encoded_buf,
							icf_uint32_t  encoded_buf_length,
							icf_api_id_t  api_id,
							icf_error_t*  p_ecode);



/******************************************************************************
* this function frees the memory allocated to icf_message_st in the function 
* icf_decode_message                                                         
*****************************************************************************/

icf_return_t icf_free_decoded_msg(icf_msg_st* p_msg, icf_error_t* p_ecode);



/****************************************************************************
 * FUNCTION:            icf_pack_api_st
 *
 * DESCRIPTION:         This function is called by the encoding functions of
 *                      CLIB. This is used as a common function for packing
 *                      the payload of the API depending upon the api_id
 ***************************************************************************/
icf_return_t icf_pack_api_st
(
    icf_uint8_t     *p_dest,
    icf_uint8_t     *p_src,
    icf_uint32_t    *p_length,
    icf_api_id_t    api_id,
    icf_boolean_t   *p_default 
);

/****************************************************************************
 * FUNCTION:            icf_unpack_api_st
 *
 * DESCRIPTION:         This function is called by the decoding functions of
 *                      CLIB. This is used as a common function for unpacking
 *                      the payload of the API depending upon the api_id
 ***************************************************************************/
icf_return_t icf_unpack_api_st
(
    icf_uint8_t     *p_dest,
    icf_uint8_t     *p_src,
    icf_uint32_t    *p_length,
    icf_api_id_t    api_id,
    icf_boolean_t   *p_default
);

icf_return_t icf_decode_mm_msg_payload(icf_msg_st*  p_msg, // Output
							icf_uint8_t*  p_encoded_buf,
							icf_uint32_t  p_encoded_buf_length,
							icf_api_id_t  api_id,
							icf_error_t*  p_ecode);

icf_return_t icf_encode_mm_api_payload(icf_msg_st*   p_msg, // Input
							icf_uint8_t** p_p_encoded_buf, // Output
							icf_uint32_t* p_encoded_buf_length,
							icf_error_t*  p_ecode);


#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /*__ICF_LIB_H__*/ 

