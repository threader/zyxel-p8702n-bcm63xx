/*****************************************************************************
 *
 * FILE NAME :      icf_port_http_utils.cpp
 *
 * DESCRIPTION:     This file contains code for utility functions used in the 
 *					MPH module for encoding and decoding HTTP messages.
 *                                    
 * REVISION HISTORY:   
 *
 * Date         Name				  Ref#                Comments
 * --------     ------------		  ----                ---------------
 * 11-Jan-2006  Shadab A. Siddiqui	  ICF LLD          Initial
 * 
 * Copyright (c) 2006, Flextronics Software Systems Ltd.
 ****************************************************************************/

#include <stdlib.h>

#include "icf_port_http_utils.h"
#include "icf_port_prototypes.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif



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
					icf_error_t         *p_ecode)
{
	icf_return_t		ret_val = ICF_SUCCESS;
	icf_uint32_t     buffer_size;
	icf_uint8_t		*p_http_msg;
	icf_header_st	*p_temp_hdr = ICF_NULL;
	icf_string_st	temp_str;

	icf_global_port_info_st  *p_glb_port_info = 
							(icf_global_port_info_st*)p_port_info;

	/* Calculating the total size of the HTTP message to be formulated */
	buffer_size = icf_port_strlen(p_method) + 
				  icf_port_strlen(p_uri) + 
				  icf_port_strlen((icf_uint8_t *)"HTTP/1.1") + 3; /*+3 for 2 " " and 1 "\n"*/

	if ((ICF_NULL != p_authentication_hdr) || 
		(ICF_NULL != p_authorization_hdr))
	{
		buffer_size += icf_port_strlen((icf_uint8_t *)"WWW-Authenticate: ");
		buffer_size += icf_port_strlen(p_authentication_hdr);
		buffer_size += icf_port_strlen((icf_uint8_t *)"Authorization: ");
		buffer_size += icf_port_strlen(p_authorization_hdr);
		buffer_size += 2;
	}

	p_temp_hdr = p_headerlist->hdr;

	while(ICF_NULL != p_temp_hdr)
	{
		buffer_size += p_temp_hdr->hdr_name.str_len;
		buffer_size += 2;
		buffer_size += p_temp_hdr->hdr_value.str_len;
		buffer_size += 1;
		p_temp_hdr = p_temp_hdr->next;
	}

	buffer_size += 1;

	if (body_length > 0)
	{
		icf_port_sprintf((icf_int8_t *)temp_str.str,"%d",body_length);
		buffer_size += icf_port_strlen((icf_uint8_t *)"Content-Length: ");
		buffer_size += icf_port_strlen(temp_str.str);
		buffer_size += 1;
		buffer_size += body_length;
	}

	buffer_size += 1;	/*for NULL termination*/

	/*buffer_size	+= sizeof(all header name) + sizeof(all header values) 
		+ sizeof(msg_pdu) + (2*numberofheaders) + 3;*/

	/* Allocate buffer of size buffer_size to hold the HTTP message; */
	p_http_msg = (icf_uint8_t *) icf_port_memget(p_glb_port_info, buffer_size,
								  ICF_MEM_COMMON,p_ecode);

	if (ICF_NULL != p_http_msg	&& \
		ICF_NULL != p_method	&& \
		ICF_NULL != p_uri)
	{
	/*	Extract method name from array of method_name whose index corresponds 
		to the method enum value;
		Append method name to the buffer;
		Append p_uri to the buffer;
		Append HTTP version to the buffer;
		Append CR and NEWLINE (ie "\r\n") to the buffer;
		Append all pair of header (header with value) CR and NEWLINE 
		(ie "\r\n") to the buffer;
		Append two CR and NEWLINE (ie "\r\n") to the buffer;
		Append p_message to the buffer;
	*/
		icf_port_strcpy(p_http_msg, p_method);
		icf_port_strcat(p_http_msg, (icf_uint8_t *)" ");
		icf_port_strcat(p_http_msg, p_uri);
		icf_port_strcat(p_http_msg, (icf_uint8_t *)" ");
		icf_port_strcat(p_http_msg, (icf_uint8_t *)"HTTP/1.1\n");

		/* header list is not mandatory for all HTTP messages */
		p_temp_hdr = p_headerlist->hdr;

		while(ICF_NULL != p_temp_hdr)
		{
			icf_port_strcat(p_http_msg, p_temp_hdr->hdr_name.str);
			icf_port_strcat(p_http_msg, (icf_uint8_t *)": ");
			icf_port_strcat(p_http_msg, p_temp_hdr->hdr_value.str);
			icf_port_strcat(p_http_msg, (icf_uint8_t *)"\n");

			p_temp_hdr = p_temp_hdr->next;
		}

		if ((ICF_NULL != p_authentication_hdr) || 
			(ICF_NULL != p_authorization_hdr))
		{
			icf_port_strcat(p_http_msg, (icf_uint8_t *)"WWW-Authenticate: ");
			icf_port_strcat(p_http_msg, p_authentication_hdr);
			icf_port_strcat(p_http_msg, (icf_uint8_t *)"\n");
			icf_port_strcat(p_http_msg, (icf_uint8_t *)"Authorization: ");
			icf_port_strcat(p_http_msg, p_authorization_hdr);
			icf_port_strcat(p_http_msg, (icf_uint8_t *)"\n");
		}

		if (body_length > 0)
		{
			icf_port_strcat(p_http_msg, (icf_uint8_t *)"Content-Length: ");
			icf_port_strcat(p_http_msg, temp_str.str);
			icf_port_strcat(p_http_msg, (icf_uint8_t *)"\n");
		}

		icf_port_strcat(p_http_msg, (icf_uint8_t *)"\n");

		/* message body is not mandatory for all HTTP messages */
		if ((ICF_NULL != p_message) && (body_length > 0)) 
		{
			icf_port_strcat(p_http_msg, p_message);
		}

		*p_p_http_msg = p_http_msg;
	}
	else
	{
		if (ICF_NULL != p_http_msg)
		{
			icf_port_memfree(p_port_info, p_p_http_msg, 
								ICF_MEM_COMMON, p_ecode);
		}
		*p_p_http_msg = ICF_NULL;
		ret_val = ICF_FAILURE;
	}

	return ret_val;
}







 

 /****************************************************************************
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
							 icf_uint8_t				*p_resp, 
							 icf_header_list_st		*p_hdr_list,
							 icf_error_t				*p_ecode)
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_uint32_t			headers_len = 0, count = 0, hdr_count = 0;
	icf_uint32_t			hdr_len, payload_length, no_of_nodes = 0;
	icf_header_st			*p_header=ICF_NULL, *iterator;
	icf_uint8_t			*resp = p_resp;
	icf_uint8_t			*p_response = p_resp;
	icf_uint8_t			*temp;
	icf_uint8_t			condition=1;

	icf_global_port_info_st  *p_glb_port_info = 
							(icf_global_port_info_st*)p_port_info;

	if (ICF_NULL != p_hdr_list)
	{
		/* Calculating the HTTP message length till the beginning the message body */
		while (condition==1)
		{
			headers_len++;
			resp++;
			if (resp[0] == '\n' && resp[1] == '\n') 
				break;
		}

		/* Removing the first line of the HTTP message that does not contain any header */
		while (p_response[0] != '\n')
		{
			p_response++;
			count++;
		}

		while (condition==1)
		{
			icf_header_st *temp_node;

			if (p_response[0] == '\n')
			{
				hdr_len = 0;
				count++;
				p_response++;
				resp = p_response;

				while (resp[0] != ':')
				{
					resp++;
					hdr_len++;
				}
				resp++;

				payload_length = ICF_PORT_SIZEOF(icf_header_st);
				temp_node = (icf_header_st *) icf_port_memget(p_glb_port_info, \
											   payload_length,	\
											   ICF_MEM_COMMON,\
											   p_ecode);

				if (ICF_NULL != temp_node)
				{
					temp_node->hdr_name.str_len = (icf_uint16_t)hdr_len;
					icf_port_strncpy((icf_int8_t *) temp_node->hdr_name.str, \
										(const icf_int8_t *) p_response, \
										hdr_len);
					temp_node->hdr_name.str[hdr_len] = '\0';

					if (ICF_FAILURE != icf_port_get_header_val(p_port_info, \
											   p_resp, \
											   (icf_uint8_t *) temp_node->hdr_name.str, \
											   &temp, \
											   p_ecode))
					{
						icf_port_strcpy(temp_node->hdr_value.str, temp);
						temp_node->hdr_value.str_len = (icf_uint16_t)icf_port_strlen(temp);
						temp_node->next = ICF_NULL;

						p_response += (hdr_len + icf_port_strlen(temp));
						count    += (hdr_len + icf_port_strlen(temp));
						hdr_count++;
						no_of_nodes++;

						icf_port_memfree(p_port_info, temp, 
										ICF_MEM_COMMON, p_ecode);
					}
					else
					{
						p_hdr_list->hdr   = p_header;
						ret_val = ICF_FAILURE;
						break;
					}

					if (hdr_count == 1)
					{
						p_header = temp_node;
					}
					else
					{
						iterator = p_header;
						while (condition==1)
						{
							if (icf_port_strcmp(temp_node->hdr_name.str, 
								iterator->hdr_name.str) == 0)
							{
								icf_port_memfree(p_port_info, temp_node, 
										ICF_MEM_COMMON, p_ecode);
								no_of_nodes--;
								break;
							}
							else if (ICF_NULL == iterator->next)
							{
								iterator->next = temp_node;
								break;
							}
							else
							{
								iterator = iterator->next;
							}
						}
					}
				}
				else
				{
					ret_val = ICF_FAILURE;
					if (no_of_nodes > 0)
					{
						p_hdr_list->hdr   = p_header;
						p_hdr_list->count = 0;
					}
					break;
				}
			}
			else
			{
				p_response++;
				count++;
			}
			if (count >= headers_len) 
				break;
		}
	} /*end if (ICF_NULL != p_hdr_list)*/
	else
	{
		ret_val = ICF_FAILURE;
	} 

	if (ret_val != ICF_FAILURE)
	{
		p_hdr_list->hdr   = p_header;
		p_hdr_list->count = no_of_nodes;
	}
	else
	{
		icf_header_st		*temp_node;
		icf_uint32_t		i;

		for (i = no_of_nodes; i > 0; i--)
		{
			temp_node = p_hdr_list->hdr->next;

			icf_port_memfree(p_port_info, p_hdr_list->hdr, 
								ICF_MEM_COMMON, p_ecode);

			p_hdr_list->hdr = temp_node;
		}

		p_hdr_list->count = 0;
		p_hdr_list = ICF_NULL;
	}

	return ret_val;
}







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
								  icf_uint8_t   *p_http_msg,
								  icf_error_t   *p_ecode)
{
	icf_uint32_t    pdu_len, count = 0, len = 0;
	icf_uint32_t	buffer_len = 0;
	icf_uint32_t	buffer_len_count = 0;
	icf_int32_t		chunkLen = -1;
	icf_uint8_t		*p_hdr_val;
	icf_uint8_t		*p_response = p_http_msg;
	icf_return_t	ret_val = ICF_SUCCESS;


	pdu_len = icf_port_strlen(p_response);
	buffer_len = icf_port_strlen(p_response);

	if ((icf_port_strncmp(p_response, (icf_uint8_t *)"HTTP/1.1", 8)) == 0)
	{
		switch((icf_port_getHTTPMessagePreCondition(p_port_info, 
													   p_response,
													   p_ecode)))
		{
			case 0:		/*Responses that carry no body*/
				while(p_response)
				{
					if (p_response[0] == '\n' && p_response[1] == '\n')
					{
						p_response += 2;
						len += 2;
						break;
					}
					else if((p_response[0] == '\r') && (p_response[1] == '\n') && 
							(p_response[2] == '\r') && (p_response[3] == '\n'))
					{
						p_response += 4;	
						len += 4;
						break;
					}
					else
					{
						p_response++;
						len++;
					}
				}

				if (len == pdu_len)
					ret_val = ICF_SUCCESS;
				else
					ret_val = ICF_FAILURE;

				break;

			case 1:		/*Transfer-Encoding*/
				len = 0;
				while(p_response)
				{
					if ((p_response[0] == '\n') && (p_response[1] == '\n') &&	/*For 1st chunk*/
						(chunkLen != 0) && count == 0)
					{
						p_response += 2;	
						len += 2;
						chunkLen = icf_port_getChunkLen(p_port_info,
											   p_response,
											   p_ecode);
						count++;
					}
					else if((p_response[0] == '\r') && (p_response[1] == '\n') &&	/*For 1st chunk*/
							(p_response[2] == '\r') && (p_response[3] == '\n') &&
							(chunkLen != 0) && count == 0)
					{
						p_response += 4;	
						len += 4;
						chunkLen = icf_port_getChunkLen(p_port_info,
											   p_response,
											   p_ecode);
						count++;
					}
					else if ((p_response[0] == '\n') && (chunkLen != 0) && count > 0)	/*Chunks after 1st arrive*/ 
					{																/*after CRLF*/
						p_response++;	
						len++;
						count++;
						if (ICF_TRUE == (count & 1))
						{
							chunkLen = icf_port_getChunkLen(p_port_info,
															p_response, p_ecode);
						}
					}

					else if((p_response[0] == '\r') && (p_response[1] == '\n') && 
							(chunkLen != 0) && count > 0)
					{
						p_response += 2;	
						len += 2;
						count++;
						if (ICF_TRUE == (count & 1))
						{
							chunkLen = icf_port_getChunkLen(p_port_info,
															p_response, p_ecode);
						}
					}
					else
					{
						p_response++;
						len++;
					}

					if(len == pdu_len)
						break;
				}

				if (chunkLen == 0) /*HTTP message ends if the chunk length is 0*/
					ret_val = ICF_SUCCESS;
				else
					ret_val = ICF_FAILURE;
				break;

			case 2:		/*Content-Length*/
				pdu_len = 0;		/*Will increment the length of the message body*/
				if (ICF_FAILURE != icf_port_get_header_val(p_port_info, p_response, 
					(icf_uint8_t *) "Content-Length", &p_hdr_val, p_ecode))
				{
					len = icf_port_atoi((const icf_int8_t *) p_hdr_val);
					icf_port_memfree(p_port_info, p_hdr_val, ICF_MEM_COMMON, p_ecode);

					if (len > 0)
					{
						while(p_response)
						{
							if (p_response[0] == '\n' && p_response[1] == '\n')
							{
								p_response += 2;
								buffer_len_count += 2;
								count++;
							}
							else if((p_response[0] == '\r') && (p_response[1] == '\n') && 
								    (p_response[2] == '\r') && (p_response[3] == '\n'))
							{
								count++;
								p_response += 4;	
								buffer_len_count += 4;
							}
							else
							{
								if (count == 1)
								{
									if ((buffer_len - buffer_len_count) == (len+1))
									{
										ret_val = ICF_SUCCESS;
										break;
									}
									else
									{
										ret_val = ICF_FAILURE;
										break;
									}
								}
								else
								{
									p_response++;
									buffer_len_count++;
									pdu_len++;
								}
							}
						}
					}
				}
				else
				{
					ret_val = ICF_FAILURE;
				}
				break;

			default:
				ret_val = ICF_FAILURE;
				break;
		}	
	}
	else
	{
		ret_val = ICF_FAILURE;
	}

	return ret_val;
}





/*****************************************************************************
 *
 * FUNCTION:        icf_port_getHTTPChunkedMessageBody
 *
 * DESCRIPTION:     This function is used for extracting entire message from 
 *					arrived chunked HTTP message.
 *                  
 * RETURNS:         Character pointer to the message body arrived through 
 *					Transfer-Encoding.
 *
*****************************************************************************/

icf_uint8_t* icf_port_getHTTPChunkedMessageBody(
										 icf_void_t    *p_port_info,
										 icf_uint8_t   *p_response,
										 icf_error_t   *p_ecode)
{
	icf_uint32_t i, pdu_len;
	icf_uint8_t  *chunkedmsg = ICF_NULL ;
	icf_uint32_t count = 0;
	icf_uint32_t chunkedmsgLen = 0;
	icf_uint32_t payload_length;

	icf_global_port_info_st  *p_glb_port_info = 
						(icf_global_port_info_st*)p_port_info;

	pdu_len = icf_port_strlen(p_response);

	for (i=0;i<pdu_len;i++) 
	{
		icf_int32_t chunkLen = 0;
		if (count == 0 && (((p_response[0] == '\n') && (p_response[1] == '\n')) || 
							((p_response[0] == '\r') && (p_response[1] == '\n') && 
							(p_response[2] == '\r') && (p_response[3] == '\n'))))
		{
			if ((p_response[0] == '\n') && (p_response[1] == '\n'))
			{
				p_response += 2;	
				i += 2;
			}
			else if((p_response[0] == '\r') && (p_response[1] == '\n') && 
				    (p_response[2] == '\r') && (p_response[3] == '\n'))
			{
				p_response += 4;	
				i += 4;
			}

			chunkLen = icf_port_getChunkLen(p_port_info,
											   p_response,
											   p_ecode);

			if (chunkLen < 1)	/*First chunk, 0 suggests no message body*/ 
			{					/*and -1 suggests failure in icf_port_getChunkLen()*/
				chunkedmsg = ICF_NULL;
				break;
			}

			chunkedmsgLen += chunkLen;

			while (p_response[0] != '\n')
			{
				p_response++;
				i++;
				if (i>pdu_len)
					break;
			}

			payload_length = 
				ICF_PORT_SIZEOF(icf_uint8_t) * (chunkedmsgLen + 1);
			chunkedmsg = (icf_uint8_t *) icf_port_memget(p_glb_port_info, payload_length,
								  ICF_MEM_COMMON,p_ecode);

			icf_port_strncpy((icf_int8_t *) chunkedmsg, 
								(const icf_int8_t *) (p_response+1), 
								chunkLen);
			p_response += (chunkLen+1);
			count++;
		}
		else if (count > 0)
		{
            icf_uint8_t	*p_temp_http_msg = ICF_NULL;

			if ((p_response[0] == '\n') || ((p_response[0] == '\r') && (p_response[1] == '\n')))
			{
				if (p_response[0] == '\n')
				{
					p_response++;	
					i++;
				}
				else
				{
					p_response += 2;	
					i += 2;
				}

				chunkLen = icf_port_getChunkLen(p_port_info,
											   p_response,
											   p_ecode);
				if (chunkLen == 0)
				{
					chunkedmsg[chunkedmsgLen] = '\0';
					break;
				}

				if (chunkLen < 0)
				{
					if (ICF_NULL != chunkedmsg)
						icf_port_memfree(p_port_info, chunkedmsg, ICF_MEM_COMMON, p_ecode);
					chunkedmsg = ICF_NULL;
					break;
				}

				chunkedmsgLen += chunkLen;

				while (p_response[0] != '\n')
				{
					p_response++;
					i++;
					if (i>pdu_len)
						break;
				}
			}

		/* Start: work around for realloc */
			p_temp_http_msg = (icf_uint8_t*)icf_port_memget(
													p_port_info,
													(chunkedmsgLen - chunkLen),
													ICF_MEM_COMMON,p_ecode);
			if (ICF_NULL == p_temp_http_msg)
			{
				break;
			}
			else
			{
				icf_port_memcpy(p_temp_http_msg, 
					chunkedmsg, 
					(chunkedmsgLen - chunkLen));

				icf_port_memfree(p_port_info, chunkedmsg, ICF_MEM_COMMON, p_ecode);
				chunkedmsg = ICF_NULL;
				chunkedmsg = (icf_uint8_t*)icf_port_memget(
												p_port_info,
												(chunkedmsgLen+1),
												ICF_MEM_COMMON,p_ecode);
				if (ICF_NULL == chunkedmsg)
				{
					break;
				}
				else
				{
					icf_port_memcpy(chunkedmsg, p_temp_http_msg, (chunkedmsgLen - chunkLen));
				}
			}
		/* End: work around for realloc */

			if (ICF_NULL != chunkedmsg)
			{
				icf_port_strncpy(((icf_int8_t *) chunkedmsg+(chunkedmsgLen - chunkLen)), 
									(const icf_int8_t *) (p_response+1), 
									chunkLen);
			}
			else 
				return ICF_NULL;

			p_response += (chunkLen+1);
		}
		else
		{
			p_response++;
		}
	}

	return chunkedmsg;
}







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
										icf_uint8_t   *p_response, 
										icf_error_t   *p_ecode)
{
	icf_return_t  ret_val = ICF_FAILURE;
	/* Checking for a valid Response, which should always start with HTTP */
	if ( ICF_NULL == p_port_info)
	{
	}
	if ((icf_port_strncmp(p_response, (icf_uint8_t *)"HTTP/1.1", 8)) == 0)
	{
		icf_uint32_t i, resp;
		icf_uint8_t respCode[4];

		/*  Extracting the Response code which would always come after the 
		 *  Protocol version (viz HTTP/1.1), which counts to 8 chars */
		for(i = 8; p_response[i] != '\n'; i++)
		{
			/* Response code will always come after Protocol version 
			 * followed by a SPACE character */
			if (p_response[i] == ' ') 
			{
				icf_uint8_t *temp = ICF_NULL;
				i = i + 1;
				temp = (p_response + i);

				/* Response code is always a 3 digit number */
				if (temp[0] >= 48 && temp[0] <= 57 &&
					temp[1] >= 48 && temp[1] <= 57 &&
					temp[2] >= 48 && temp[2] <= 57)
				{
					icf_port_strncpy((icf_int8_t *) respCode, 
										(const icf_int8_t *) temp, 3);  
					respCode[3] = '\0';

					/* Converting the string response code to numeric */
					resp = icf_port_atoi((const icf_int8_t *) respCode);		
					*p_respCode = resp;
					ret_val = ICF_SUCCESS;
					break;
				}
				else
				{
					*p_respCode = 0;
					ret_val = ICF_FAILURE;
					*p_ecode = ICF_CAUSE_INTERNAL_FAILURE;
				}
			}
		}
	}
	else
	{
		ret_val = ICF_FAILURE;
	}
	return ret_val;
}





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
									icf_error_t   *p_ecode)
{
	icf_uint32_t preCondition, headerLength;
	icf_uint8_t *message;
	icf_return_t ret_val = ICF_SUCCESS;


	preCondition = icf_port_getHTTPMessagePreCondition(
													p_port_info, 
													p_response,
													p_ecode);

	switch (preCondition)
	{
		/* Response code as such that message has no body */
		case 0:
			*p_p_msg_body = ICF_NULL;
			*p_body_length = 0;
			break;

		/* Transfer-Encoding header with value != "identity". */
		case 1:
			message = icf_port_getHTTPChunkedMessageBody(p_port_info, 
															p_response,
															p_ecode);
			*p_body_length = icf_port_strlen(message);
			*p_p_msg_body = message;
			break;

		/* Content-Length header */
		case 2:
			if (ICF_FAILURE != icf_port_get_header_val(p_port_info, p_response, 
								(icf_uint8_t *) "Content-Length", &message, p_ecode))
			{
				headerLength = icf_port_atoi((const icf_int8_t *) message);
				icf_port_memfree(p_port_info, 
					message, 
					ICF_MEM_COMMON, 
					p_ecode);
				
				if (headerLength > 0)
				{
					message = icf_port_getMsgChunk(p_port_info, 
						p_response,
						p_ecode);

					*p_body_length = headerLength;
					message[headerLength] = '\0';
					*p_p_msg_body = message;
				}
			}
			else
			{
				*p_body_length = 0;
				*p_p_msg_body = ICF_NULL;
				ret_val = ICF_FAILURE;
			}
			break;

		/* Neither Transfer-Encoding nor Content-Length header is recieved. */
		case 3:
			*p_body_length = 0;
			*p_p_msg_body = ICF_NULL;
			ret_val = ICF_FAILURE;
			break;

		default:
			*p_body_length = 0;
			*p_p_msg_body = ICF_NULL;
			ret_val = ICF_FAILURE;
			break;
	}

	return ret_val;;
}





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
										   icf_error_t   *p_ecode)
{
	icf_uint32_t		respCode;
	icf_uint32_t		pre_condition = 0;
	icf_uint8_t		*p_transEncVal = ICF_NULL;
	icf_uint8_t		*p_contentLenVal = ICF_NULL;
	icf_error_t		error;

	if (ICF_SUCCESS == 
			icf_port_get_http_resp_code (p_port_info, &respCode, 
											p_response, &error))
	{
		if ((respCode < 200) || (respCode == 204) || (respCode == 304))
		{
			pre_condition = 0;	/*No message body along with these responses*/
		}
		else
		{
			icf_port_get_header_val(p_port_info, p_response, 
							(icf_uint8_t *) "Transfer-Encoding", &p_transEncVal, p_ecode);

			if (ICF_NULL != p_transEncVal)
			{
				/* Checking whether Transfer-Encoding headers value is != 
				 * "identity". If the condition is true then we have to 
				 * consider Transfer-Encoding header for message length 
				 * calculation and extracting the message */
				if ((ICF_NULL == 
						icf_port_strstri(p_port_info,
						p_transEncVal, (icf_uint8_t *)"identity", p_ecode)) || 
					(ICF_NULL != 
						icf_port_strstri(p_port_info,
						p_transEncVal, (icf_uint8_t *)"chunked", p_ecode)))  
				{
					pre_condition = 1;	/*Transfer-Encoding to be considered*/
				}
				else if(ICF_FAILURE != 
					icf_port_get_header_val(p_port_info, p_response, 
								(icf_uint8_t *) "Content-Length", &p_contentLenVal, p_ecode))
				{
					pre_condition = 2; /*Content-Length to be considered*/
					icf_port_memfree(p_port_info, 
								p_contentLenVal, 
								ICF_MEM_COMMON, 
								p_ecode);
				}
				icf_port_memfree(p_port_info, 
								p_transEncVal, 
								ICF_MEM_COMMON, 
								p_ecode);
			}
			else if (ICF_FAILURE != 
					icf_port_get_header_val(p_port_info, p_response, 
								(icf_uint8_t *) "Content-Length", &p_contentLenVal, p_ecode))
			{
				pre_condition = 2; /*Content-Length headers to be considered*/

				icf_port_memfree(p_port_info, 
								p_contentLenVal, 
								ICF_MEM_COMMON, 
								p_ecode);
			}
			else	
			{
				/* Both Transfer-Encoding and Content-Length headers are 
				 * not present. */
				pre_condition = 3;	/*Error scenario*/
			}
		}
	}
	else
	{
		pre_condition = 4; /*errorneous condition, illegal response code*/
	}

	return pre_condition;
}





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
							icf_uint8_t	 *p_msg,
							icf_error_t   *p_ecode)
{
	if( ICF_NULL == p_port_info)
	{
	}
	if ( ICF_NULL == p_ecode)
	{
	}
	if (ICF_NULL != p_msg)
	{
		while (ICF_NULL != p_msg)
		{
		/* Message body in a HTTP resp. starts immediately after two 
			consecutive NEWLINE characters */
			if ((p_msg[0] == '\n') && (p_msg[1] == '\n'))
			{
				p_msg += 2;
				break;
			}
			else if((p_msg[0] == '\r') && (p_msg[1] == '\n') && (p_msg[2] == '\r') && (p_msg[3] == '\n'))
			{
				p_msg += 4;
				break;
			}
			else
			{
				p_msg++;
			}
		}
	}

	return p_msg;
}







/*****************************************************************************
 *
 * FUNCTION:        icf_port_get_header_val
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
								icf_error_t   *p_ecode)
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_uint8_t			*p_hdrVal = ICF_NULL;
	icf_uint8_t			*p_hdr = ICF_NULL;
	icf_uint32_t			payload_length, len;
	icf_uint8_t			*p_temp_hdr = ICF_NULL;
	static icf_uint32_t	count = 0;
	static icf_uint8_t	*hdrVal[ICF_MAX_HEADER_OCCURENCE];
	static icf_uint32_t	hdrLen = 0;

	icf_global_port_info_st  *p_glb_port_info = 
        (icf_global_port_info_st*)p_port_info;

	len = icf_port_strlen(p_header)+3;
	p_temp_hdr = (icf_uint8_t *) icf_port_memget(p_port_info, len, ICF_MEM_COMMON, p_ecode);
	
	if (ICF_NULL != p_temp_hdr)
	{
		icf_port_strcpy(p_temp_hdr, (icf_uint8_t *)"\n");
		icf_port_strcat(p_temp_hdr, p_header);
		icf_port_strcat(p_temp_hdr, (icf_uint8_t *)":");

		do 
		{
			p_hdr = icf_port_strstri(p_port_info, 
				p_response, p_temp_hdr, p_ecode);

			p_response = p_hdr;
			if (ICF_NULL != p_hdr)
			{
				icf_uint32_t temphdrLen;
				hdrVal[count] = icf_port_extractHeaderVal(p_port_info, 
					p_hdr,
					p_ecode);
				if (ICF_NULL != hdrVal[count])
				{
					temphdrLen =icf_port_strlen(hdrVal[count]);
					hdrLen += temphdrLen;
					p_response += temphdrLen + (strlen((const icf_int8_t *)p_header)) + 2;
					count++;
				}
				else
				{
					p_hdrVal = ICF_NULL;
					ret_val = ICF_FAILURE;
					break;
				}
			}
			else if(count == 0)  /*header is not present the the HTTP response*/
			{
				p_hdrVal = ICF_NULL;
				ret_val = ICF_FAILURE;
			}
			else if (count > 0)	/*All headers values extracted*/
			{
				icf_uint32_t i;
				hdrLen += count;
				payload_length = (ICF_PORT_SIZEOF(icf_uint8_t) * hdrLen) + 1;
				p_hdrVal = (icf_uint8_t *) icf_port_memget(p_glb_port_info, 
											  payload_length,
											  ICF_MEM_COMMON,
											  p_ecode);	
				if (ICF_NULL != p_hdrVal)
				{
					icf_port_strcpy(p_hdrVal, (icf_uint8_t *)"");
					for(i = 0; i < count; i++)
					{
						if (ICF_NULL != hdrVal[i])
						{
							icf_port_strcat(p_hdrVal, hdrVal[i]);
							if (i != (count - 1))
								icf_port_strcat(p_hdrVal, (icf_uint8_t *)",");

							/* freeing the memory allocated in 
 							 * icf_port_extractHeaderVal() */
							icf_port_memfree(p_port_info, 
								hdrVal[i], 
								ICF_MEM_COMMON, 
								p_ecode);
						}
					}
				}
				else
				{
				/* freeing the memory allocated in 
 				 * icf_port_extractHeaderVal() */
					for(i = 0; i < count; i++)
					{
						icf_port_memfree(p_port_info, 
							hdrVal[i], 
							ICF_MEM_COMMON, 
							p_ecode);
					}
					p_hdrVal = ICF_NULL;
					ret_val = ICF_FAILURE;
				}
				p_hdr = ICF_NULL;
				count = 0;
			}
		}while (ICF_NULL != p_hdr);

		icf_port_memfree(p_port_info, p_temp_hdr, ICF_MEM_COMMON, p_ecode);
	}
	else
	{
		p_hdrVal = ICF_NULL;
		ret_val = ICF_FAILURE;
	}

	*p_header_val = p_hdrVal;
	return ret_val;
}

		





/*****************************************************************************
 *
 * FUNCTION:        icf_port_extractHeaderVal
 *
 * DESCRIPTION:     This function is used for extracting the exact header 
 *					value from the given string.
 *                  
 * RETURNS:         NULL if the passed argument is NULL else the pointer to 
 *					the string containing the header value.
 *
*****************************************************************************/

icf_uint8_t* icf_port_extractHeaderVal(
								 icf_void_t    *p_port_info,
								 icf_uint8_t   *p_hdr,
								 icf_error_t   *p_ecode)		
{
	icf_uint32_t             payload_length;

	icf_global_port_info_st  *p_glb_port_info = 
						(icf_global_port_info_st*)p_port_info;

	/* Checking for the presence of the header, proceed if it is present */
	if (ICF_NULL != p_hdr) 
	{
		icf_uint8_t *p_valstart;
		icf_uint8_t *headerValue = ICF_NULL;
		icf_uint32_t i = 0, val_length = 0;

		while (p_hdr)
		{
			/* Header value always starts after Header name, followed by 
			 * a colon and a space */
			if (p_hdr[i] == ' ') 
			{
				p_hdr++;
				p_valstart = p_hdr;

				/* Header value ends if we encounter a NEWLINE character and 
				 * which is not followed by a SPACE or a HORIZONTAL TAB */
				while (p_hdr[0] != '\n') 
				{
					p_hdr++;
					val_length++;
			
					/* Header value can continue to next line if a NEWLINE 
					 * character is followed by a SPACE or a HORIZONTAL TAB, 
					 * thus this condition jumps the NEWLINE character as
					 * this situation indicates continuation of the header 
					 * value */
					if ((p_hdr[0] == '\n') && ((p_hdr[1] == ' ') || 
						(p_hdr[1] == '\t')))
					{
						p_hdr++;
						val_length++;
					}
				}

				payload_length = 
						(ICF_PORT_SIZEOF(icf_uint8_t) * val_length) + 1;
				headerValue = (icf_uint8_t *) icf_port_memget(p_glb_port_info, 
												 payload_length,
								                 ICF_MEM_COMMON,
												 p_ecode);
				if (ICF_NULL != headerValue)
				{
					icf_port_strncpy((icf_int8_t *) headerValue, 
										(const icf_int8_t *) p_valstart, 
										val_length);
					headerValue[val_length] = '\0';
					return headerValue;
				}
				else
					return ICF_NULL;
			}
			else
			{
				p_hdr++;
			}
		}
	}
	else
	{
		return ICF_NULL;
	}

	return ICF_NULL;
}







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
							icf_error_t   *p_ecode)
{
	icf_int32_t chunk_len = 0;
	icf_uint32_t payload_length, count = 0;
	icf_uint8_t  *temp = ICF_NULL;
	icf_uint8_t  *start = msg;

	icf_global_port_info_st  *p_glb_port_info = 
						(icf_global_port_info_st*)p_port_info;

	while(msg)
	{
		if ((msg[0] >= '0' && msg[0] <= '9') || 
			(msg[0] >= 'a' && msg[0] <= 'f') || 
			(msg[0] >= 'A' && msg[0] <= 'F'))
		{
			count++;
			msg++;
		}
		else
		{
			payload_length = (ICF_PORT_SIZEOF(icf_uint8_t) * count) + 1;
			temp = (icf_uint8_t *) icf_port_memget(p_glb_port_info, payload_length,
									  ICF_MEM_COMMON,p_ecode);
			if (ICF_NULL != temp)
			{
				icf_port_strncpy((icf_int8_t *) temp, 
									(const icf_int8_t *) start, 
									count);
				temp[count] = '\0';
				chunk_len = icf_port_hextoint(p_port_info, temp, p_ecode);
				icf_port_memfree(p_port_info, temp, 
									ICF_MEM_COMMON, p_ecode);
				break;
			}
			else
			{
				chunk_len  = -1;
				*p_ecode   = ICF_ERROR_MEM_ALLOC_FAILURE;
				break;
			}
		}
	}

	return chunk_len;
}




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
						 icf_error_t   *p_ecode)
{
	icf_uint32_t j, len;
	icf_int32_t hex_val= 0;

	p_token=icf_port_tohexupper(p_port_info, p_token, p_ecode);
	if (ICF_NULL == p_token)
	{
		hex_val = -1;
	}
	else
	{
		len = icf_port_strlen(p_token);
		
		for (j=0;j<len;j++)
		{
			if (p_token[j] >= '0' && p_token[j] <= '9')
			{
				hex_val = hex_val * 16 + (p_token[j]-'0');
			}
			else
			{
				hex_val = hex_val * 16 + (p_token[j]-'A' + 10);
			}
		}
		icf_port_memfree(p_port_info, p_token, ICF_MEM_COMMON, p_ecode);
	}
	return hex_val;
}





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
						 icf_error_t   *p_ecode)
{
	icf_uint8_t  *upper;
	icf_uint32_t i, hexLen = 0;

	icf_global_port_info_st  *p_glb_port_info = 
						(icf_global_port_info_st*)p_port_info;

	hexLen = icf_port_strlen(p_token);

	upper = (icf_uint8_t *) icf_port_memget(p_glb_port_info, hexLen+1,
								  ICF_MEM_COMMON,p_ecode);

	if (ICF_NULL != upper)
	{
		for (i=0; i<hexLen; i++)
		{
			if ((p_token[i] >= 'a') &&  (p_token[i] <= 'f'))
			{
				upper[i] = (icf_uint8_t)(p_token[i] - 32);
			}
			else if ((p_token[i] >= '0') &&  (p_token[i] <= '9'))
			{
				upper[i] = p_token[i];
			}
			else if ((p_token[i] >= 'A') &&  (p_token[i] <= 'F'))
			{
				upper[i] = p_token[i];
			}			
			else
			{
				return ICF_NULL;
			}
		}

		upper[hexLen] = '\0';
	}
	else
		upper = ICF_NULL;

	return upper;
}








/***************************************************************************/
/* TEMPORARY CODE gets the connection status */
/***************************************************************************/


icf_uint32_t getConnectionStatus(icf_uint32_t connHandle)
{
	icf_uint32_t connStatus = 1;

	connHandle = connStatus;

	return(connStatus);
}

