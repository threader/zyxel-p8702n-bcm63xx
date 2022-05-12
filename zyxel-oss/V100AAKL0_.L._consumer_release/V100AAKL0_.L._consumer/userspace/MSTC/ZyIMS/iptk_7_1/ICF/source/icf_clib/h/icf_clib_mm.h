/*******************************************************************************
*										
* FILE NAME    :icf_clib_mm.h
*										
* DESCRIPTION  :The wrapper header file to the client library
*										
* Revision History :				
*				
*   DATE             NAME               REFERENCE              REASON
* -----------    ---------------      -------------      ------------------
* 05-JUL-2005     Raveen Sharma	       ICF REL 6.0		       Initial
*   										
* Copyright 2006, Aricent.   					
*										
*******************************************************************************/



#ifndef __ICF_CLIB_MM_H__
#define __ICF_CLIB_MM_H__



/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

#include "icf_mm_types.h"


	
/****************************************************************************
* This function is used to encode icf_msg_st into a byte buffer  
* The pointer to input buffer is provided as an argument.          
* The memory associated with the input buffer is released by ICF.  
* The memory allocated by the ICF for encoded_buffer(output) has   
* to be released by the Application.								
*****************************************************************************/

icf_return_t icf_encode_mm_msg(icf_msg_st* p_msg, /*Input*/
							icf_uint8_t** p_p_encoded_buf,
							icf_uint32_t* p_encoded_buf_length,
							icf_error_t* p_ecode);







/****************************************************************************
* This function is used to convert the byte buffer received from the		
* ICF to appropriate icf_message_type (User API).							
* The memory for icf_message_st is allocated by ICF which has to be freed	
* by the Application.														
* The encoded buffer (p_encoded_buf) is freed by ICF after decoding.		
*****************************************************************************/
icf_return_t icf_decode_mm_msg(icf_msg_st**  p_p_msg, /*Output*/
							icf_uint8_t*  p_encoded_buf,
							icf_uint32_t  p_encoded_buf_length,
							icf_error_t*  p_ecode);





/******************************************************************************
* this function frees the memory allocated to icf_message_st in the function 
* icf_decode_message                                                         
*****************************************************************************/

icf_return_t icf_free_decoded_mm_msg(icf_msg_st* p_msg, icf_error_t* p_ecode);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /*__ICF_CLIB_MM_H__*/ 
