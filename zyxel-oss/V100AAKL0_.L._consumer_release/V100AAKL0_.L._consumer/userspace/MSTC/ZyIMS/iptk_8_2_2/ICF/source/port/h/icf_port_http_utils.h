/*******************************************************************************

 *

 * FILE NAME :      icf_port_http_utils.h

 *

 * DESCRIPTION:     This file contains prototypes for utility functions used in  

 *					the MPH module for encoding and decoding HTTP messages.

 *                                    

 * REVISION HISTORY:   

 *

 * Date         Name				  Ref#                Comments

 * --------     ------------		  ----                ---------------

 * 11-Jan-2006  Shadab A. Siddiqui	  ICF LLD          Initial

 * 

 * Copyright (c) 2006, Flextronics Software Systems Ltd.

 ******************************************************************************/



#ifndef __ICF_PORT_HTTP_UTILS_H__

#define __ICF_PORT_HTTP_UTILS_H__



#include "icf_types.h"



#define ICF_MAX_HEADER_OCCURENCE		5





 /* Determine if a C++ compiler is being used.

 * If so, ensure that standard

 * C is used to process the header information.

 *   */



#ifdef __cplusplus



/* Yes, C++ compiler is present.

 * Use standard C.

 * */



extern "C"

{

#endif /* __cplusplus */





/*****************************************************************************  

 *

 *

 * FUNCTION:        icf_port_make_http_req

 *

 * DESCRIPTION:     This function is used for formulating an HTTP request.

 *                  

 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in

 *                  case of failure.

 *

*****************************************************************************/

icf_return_t icf_port_make_http_req(

					icf_void_t          *p_port_info, 

					icf_uint8_t		   *p_method,

					icf_uint8_t         *p_uri, 

					icf_uint8_t         *p_authentication_hdr,

					icf_uint8_t         *p_authorization_hdr,

					icf_header_list_st  *p_headerlist,    

					icf_uint32_t	       body_length,

					icf_uint8_t         *p_message, 

					icf_uint8_t         **p_p_http_msg, 

					icf_error_t         *p_ecode);





/*****************************************************************************  

 *

 * FUNCTION:        icf_port_send_http_req

 *

 * DESCRIPTION:     This function is used sending an HTTP request over the 

 *			  network.

 *                  

 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in

 *                  case of failure.

 *

*****************************************************************************/

icf_return_t icf_port_send_http_req (

					icf_void_t      *p_port_info, 

					icf_uint32_t    connHandle, 			

					icf_uint8_t     *p_http_msg, 

					icf_uint32_t		msg_len,

					icf_error_t     *p_ecode);





/*****************************************************************************   

 *

 * FUNCTION:        icf_port_populate_header_list

 *

 * DESCRIPTION:     This function is used for extracting the headers from a 

 *					HTTP response, create a list of headers and return it.

 *                  

 * RETURNS:         Pointer to the populated header list via the argument 

 *					hdr_list.

 *

*****************************************************************************/

icf_return_t icf_port_populate_header_list(

					icf_void_t				*p_port_info,

					icf_uint8_t				*p_response, 

					icf_header_list_st		*p_hdr_list,

					icf_error_t				*p_ecode);





/*****************************************************************************

 *

 * FUNCTION:        icf_port_isCompleteHTTPResp

 *

 * DESCRIPTION:     This function is used for checking whether the recieved

 *					HTTP message is a complete HTTP message or not.

 *                  

 * RETURNS:         Returns ICF_SUCCESS if it is a complete HTTP message 

 *                  else ICF_FAILURE.

 *

*****************************************************************************/

icf_return_t icf_port_isCompleteHTTPResp(

								  icf_void_t    *p_port_info,

								  icf_uint8_t   *p_response,

								  icf_error_t   *p_ecode);





/*****************************************************************************   

 *

 * FUNCTION:        icf_port_getHTTPChunkedMessageBody

 *

 * DESCRIPTION:     This function is used for extracting entire message from 

 *					arrived chunked HTTP message.

 *                  

 * RETURNS:         Character pointer to the message body arrived with 

 *					Transfer-Encoding header.

 *

*****************************************************************************/

icf_uint8_t* icf_port_getHTTPChunkedMessageBody(

										 icf_void_t    *p_port_info,

										 icf_uint8_t   *p_response,

										 icf_error_t   *p_ecode);





/*****************************************************************************   

 *

 * FUNCTION:        icf_port_get_http_resp_code

 *

 * DESCRIPTION:     Function to extract the Response Code returned by the 

 *					server from the Response PDU.

 *                  

 * RETURNS:         ICF_FAILURE or ICF_SUCCESS and a valid HTTP 

 *					Response Code via p_respCode else 0.

 *

*****************************************************************************/

icf_return_t icf_port_get_http_resp_code (

											icf_void_t    *p_port_info, 

											icf_uint32_t  *p_respCode,	  		

											icf_uint8_t   *http_msg, 

											icf_error_t   *p_ecode);









/*****************************************************************************   

 *

 * FUNCTION:        icf_port_get_http_msg_body

 *

 * DESCRIPTION:     Function to extract message body from the Response PDU.

 *                  

 * RETURNS:         Pointer to the message body else NULL.

 *

*****************************************************************************/

icf_return_t icf_port_get_http_msg_body (

									icf_void_t    *p_port_info,               

									icf_uint8_t   *p_response, 

									icf_uint32_t  *p_body_length, 

									icf_uint8_t   **p_p_msg_body, 

									icf_error_t   *p_ecode);







/*****************************************************************************   

 *

 * FUNCTION:        icf_port_getHTTPMessagePreCondition

 *

 * DESCRIPTION:     Function to decide the condition which helps in 

 *					calculating the message length in the Response.

 *					For details refer section 4.4 of RFC 2616.

 *                  

 * RETURNS:         Valid integer representing a possible condition else 0.

 *

*****************************************************************************/

icf_uint32_t icf_port_getHTTPMessagePreCondition(

										   icf_void_t    *p_port_info,

										   icf_uint8_t   *p_response,

										   icf_error_t   *p_ecode);







/*****************************************************************************   

 *

 * FUNCTION:        icf_port_getMsgChunk

 *

 * DESCRIPTION:     This function is used for extracting entire message from 

 *					arrived HTTP message..

 *                  

 * RETURNS:         Character pointer to the message body arrived with 

 *					Content-Length header.

 *

*****************************************************************************/

icf_uint8_t* icf_port_getMsgChunk(

							icf_void_t    *p_port_info,

							icf_uint8_t	 *msg,

							icf_error_t   *p_ecode);







/*****************************************************************************   

 *

 * FUNCTION:        icf_port_getHTTPHeaderVal

 *

 * DESCRIPTION:     This function is used for extracting header value of a 

 *					given header from the HTTP response. A header can exists 

 *					at multiple places in the entire HTTP response.

 *                  

 * RETURNS:         NULL if the header is not present else the pointer to the 

 *					string containing the header value.

 *

*****************************************************************************/

icf_return_t icf_port_get_header_val (

								icf_void_t    *p_port_info, 

								icf_uint8_t   *p_response, 			

								icf_uint8_t   *p_header, 

								icf_uint8_t   **p_header_val, 

								icf_error_t   *p_ecode);









/*****************************************************************************   

 *

 * FUNCTION:        icf_port_extractHeaderVal

 *

 * DESCRIPTION:     This function is used for extracting the exact header 

 *					value from the given string. It is invoked by the 

 *					icf_port_get_header_val(), thus acts as a supplement 

 *					funtion.

 *                  

 * RETURNS:         NULL if the passed argument is NULL else the pointer to 

 *					the string containing the header value.

 *

*****************************************************************************/

icf_uint8_t* icf_port_extractHeaderVal(

								 icf_void_t    *p_port_info,

								 icf_uint8_t   *p_hdr,

								 icf_error_t   *p_ecode);





/*****************************************************************************   

 *

 * FUNCTION:        icf_port_getChunkLen

 *

 * DESCRIPTION:     This function is used for calculating the length of a 

 *					message chunk.

 *                  

 * RETURNS:         Length of the chunk.

 *

*****************************************************************************/

icf_int32_t icf_port_getChunkLen(

							icf_void_t    *p_port_info,

							icf_uint8_t   *msg,

							icf_error_t   *p_ecode);









/*****************************************************************************   

 *

 * FUNCTION:        icf_port_hextoint

 *

 * DESCRIPTION:     This function is used for converting the hex value of 

 *					chunk into integer.

 *                  

 * RETURNS:         Integer equivalent of the hex chunk length.

 *

*****************************************************************************/

icf_int32_t icf_port_hextoint(

						 icf_void_t    *p_port_info,

						 icf_uint8_t   *p_token,

						 icf_error_t   *p_ecode);







/*****************************************************************************   

 *

 * FUNCTION:        icf_port_tohexupper

 *

 * DESCRIPTION:     This function is used for converting the hex string to an

 *					upper case hex string.

 *                  

 * RETURNS:         Upper case equivalent of the hex string.

 *

*****************************************************************************/

icf_uint8_t* icf_port_tohexupper(

						 icf_void_t    *p_port_info,

						 icf_uint8_t   *p_token,

						 icf_error_t   *p_ecode);















/*****************************************************************************/

/* TEMPORARY CODE gets the connection status */

icf_uint32_t getConnectionStatus(icf_uint32_t connHandle);









#ifdef __cplusplus

}

#endif /* __cplusplus */

#endif /* End of ifndef __ICF_PORT_INTF_PROTOTYPES_H__ */

