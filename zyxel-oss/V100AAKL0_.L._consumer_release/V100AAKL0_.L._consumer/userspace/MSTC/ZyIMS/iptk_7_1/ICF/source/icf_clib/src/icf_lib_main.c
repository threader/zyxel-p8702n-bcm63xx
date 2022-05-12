/*******************************************************************************
*										
* FILE NAME    :icf_lib_main.c
*										
* DESCRIPTION  :The file contains the encoder and decoder routines of the 
*				library as well as the related free routines
*
*										
* Revision History :				
*				
*    DATE            NAME         REFERENCE       REASON
* --------    ------------        ----         ----------
* 29-JUL-2005   Ram Narayan        None         Initial
* 02-Feb-2006  Amit Sharma                      CFG merged with ICF 2.0
* 08-Feb-2006  Amit Sharma               Network PDU Logging API from ICF
* 07-Mar-2006	Anuradha Gupta		ICF			Added code changes for Re-invite handling
* 30-Mar-2006	Anuradha Gupta		ICF 5.0		Call Modify Feature changes
* 27-June-2006  Raveen Sharma	   ICF 6.0	 Added code changes for list handling
* 03-Jan-2007   Amit Mahajan                     INFO confirmation changes
*
* 26-Mar-2007   Deepti Goyal        ICF 6.4     Changes for media connect req API.
* 08-JUN-2007   Alok Tiwari      Rel early 7.0  Code added for stat and register status 
*
* Copyright 2007, Aricent.   					
*										
*******************************************************************************/


#include "icf_clib.h"
#include "icf_utils.h"
#include "icf_api.h"
#include "icf_error_id.h"
#include "icf_clib_mm.h"
#include "icf_types.h"

/* #define VALIDATION_UT */

/* for warning removal */
extern icf_return_t icf_decode_mm_msg_payload(icf_msg_st*  p_msg, /* Output*/
                            icf_uint8_t*  p_encoded_buf,
                            icf_uint32_t  p_encoded_buf_length,
                            icf_api_id_t  api_id,
                            icf_error_t*  p_ecode);

extern icf_return_t icf_encode_mm_api_payload(icf_msg_st*   p_msg, /* Input*/
                            icf_uint8_t** p_p_encoded_buf, /* Output*/
                            icf_uint32_t* p_encoded_buf_length,
                            icf_error_t*  p_ecode);
/* end of warning removal code */

extern icf_return_t icf_encode_api_payload(icf_msg_st*   p_msg, /* Input*/
                                                        icf_uint8_t** p_p_encoded_buf, /* Output*/
                                                        icf_uint32_t* p_encoded_buf_length,
                                                        icf_error_t*  p_ecode);

extern icf_return_t icf_decode_api_hdr(icf_api_header_st *p_api_hdr, /* Output*/
                                                        icf_uint8_t*  p_encoded_buf,
                                                        icf_uint32_t  encoded_buf_length,
                                                        icf_error_t*  p_ecode);

extern icf_return_t icf_decode_msg_payload(icf_uint8_t*  p_msg, /* Output*/
                                                        icf_uint8_t*  p_encoded_buf,
                                                        icf_uint32_t  encoded_buf_length,
				                        icf_api_id_t  api_id,
                                                        icf_error_t*  p_ecode);

extern icf_return_t icf_pack_api_st
(
    icf_uint8_t     *p_dest,
    icf_uint8_t     *p_src,
    icf_uint32_t    *p_length,
    icf_api_id_t    api_id,
    icf_boolean_t   *p_default 
);

extern icf_return_t icf_unpack_api_st
(
    icf_uint8_t     *p_dest,
    icf_uint8_t     *p_src,
    icf_uint32_t    *p_length,
    icf_api_id_t    api_id,
    icf_boolean_t   *p_default
);


extern icf_return_t icf_vldt_api(
             icf_msg_st *p_msg,
             icf_vldt_err_st *p_vldt_err);


/* 
 * This function takes as input a message of type icf_msg_st. This would
 * read the api id from the api header and the would pack the payload
 * present and would return the encoded payload in p_encoded_buff
 * It would also return the length of encoded buffer through variable 
 * p_encoded_buf_length
 */ 
icf_return_t icf_encode_api_payload(icf_msg_st*   p_msg, /* Input*/
							icf_uint8_t** p_p_encoded_buf, /* Output*/
							icf_uint32_t* p_encoded_buf_length,
							icf_error_t*  p_ecode)

{

	icf_uint32_t  length    = ICF_NULL;
	icf_uint8_t*  p_src     = ICF_NULL;
	icf_uint8_t*  p_dest    = ICF_NULL;
	icf_uint8_t*  p_buff    = ICF_NULL;
    icf_return_t ret_val    = ICF_SUCCESS;
	icf_boolean_t is_default = ICF_FALSE;

	if (!p_msg) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Argument #1 p_msg is NULL\n"));
		*p_ecode = ICF_ERR_NULL_POINTER;
		return ICF_FAILURE;
	}

    ICF_CLIB_TRACE(("\n<======================== ENCODE START ======================>\n"));


	/* Allocate a temporary buffer with the API length of the */
	/* Input Message */

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Getting Memory for p_buff\n"));
	p_buff   = (icf_uint8_t*)ICF_CLIB_MEMGET(ICF_CLIB_MAX_TEMP_ARBI_BUFF_LENGTH)

	if (!p_buff) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure!! (p_buff)\n"));
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


	/* Pack the payload*/
	p_src = p_msg->payload;

	/* Make sure the destination points beyond the API header*/
	p_dest = p_buff;

	ret_val = icf_pack_api_st(p_dest,p_src,&length,p_msg->hdr.api_id, &is_default);

	if(ICF_TRUE == is_default)
    {
        ICF_CLIB_MEMFREE(p_buff);
        ret_val = (icf_uint8_t)icf_encode_mm_api_payload(p_msg, p_p_encoded_buf, p_encoded_buf_length, p_ecode);
        return ret_val;
    }

	if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - ENCODING Failed While Packing!!!\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_ENCODE_FAILED;
        return ret_val;
    }

	*p_encoded_buf_length = length;
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Getting Memory for p_p_encoded_buf\n"));
	*p_p_encoded_buf = (icf_uint8_t*)ICF_CLIB_MEMGET(length);

	if (!*p_p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure! (*p_p_encoded_buf)\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - MEMCopying Payload to encoded buffer\n"));
	ICF_CLIB_MEMCPY(*p_p_encoded_buf, p_dest, length)

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - FREEing p_buff\n"));
	ICF_CLIB_MEMFREE(p_buff)

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Encoded Buffer Length (%d) Bytes\n", *p_encoded_buf_length));
    ICF_CLIB_TRACE(("\n<========================= ENCODE END =======================>\n"));
	
	return ret_val;
}




icf_return_t icf_encode_msg(icf_msg_st*   p_msg, /* Input*/
							icf_uint8_t** p_p_encoded_buf, /* Output*/
							icf_uint32_t* p_encoded_buf_length,
							icf_error_t*  p_ecode,
							icf_vldt_err_st*  p_vldt_err)

{

	icf_uint32_t  length    = ICF_NULL;
	icf_uint8_t*  p_src     = ICF_NULL;
	icf_uint8_t*  p_dest    = ICF_NULL;
	icf_uint8_t*  p_buff    = ICF_NULL;
	icf_uint32_t api_length = ICF_NULL;
        icf_return_t ret_val    = ICF_SUCCESS;
	icf_uint32_t hdr_length = 0;
	icf_uint32_t api_length_index = 0;
	icf_uint16_t api_length_value = 0;
	icf_boolean_t is_default = ICF_FALSE;


	if (!p_msg) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Argument #1 p_msg is NULL\n"));
		*p_ecode = ICF_ERR_NULL_POINTER;
		return ICF_FAILURE;
	}
        else
        {
            ret_val = icf_vldt_api(p_msg,p_vldt_err); 
            if (ICF_FAILURE == ret_val)
            {
                ICF_CLIB_TRACE(("\nFailure in validation \n"));
                ICF_CLIB_TRACE(("\n err_str:%s \n",p_vldt_err->err_str));
                *p_ecode = ICF_CAUSE_VALIDATION_FAILURE;
                return ICF_FAILURE;
            }
#ifdef VALIDATION_UT
            else
            {
                return ICF_SUCCESS;
            } 
#endif
        }

    ICF_CLIB_TRACE(("\n<======================== ENCODE START ======================>\n"));


	api_length = p_msg->hdr.api_length;

    if (api_length <= 0 || api_length >= ICF_CLIB_MAX_API_LENGTH) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Invalid API Length (%d)\n", api_length));
        *p_ecode = ICF_ERR_INVALID_LENGTH;
        return ICF_FAILURE;
    }


	/* Allocate a temporary buffer with the API length of the */
	/* Input Message */

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Getting Memory for p_hdr\n"));
	p_buff   = (icf_uint8_t*)ICF_CLIB_MEMGET(api_length + ICF_CLIB_MAX_TEMP_ARBI_BUFF_LENGTH)

	if (!p_buff) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure!! (p_buff)\n"));
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


	p_src    = (icf_uint8_t*)p_msg;

	/* Pack the header first*/
	ret_val = (icf_uint8_t)icf_pack_icf_api_header_st(p_buff, p_src, &length, &api_length_index);

	hdr_length = length;
		
    if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - ENCODING Failed While Packing API Header!!!\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_ENCODE_FAILED;
        return ret_val;
    }

	/* Pack the rest of the payload*/
	p_src = (icf_uint8_t*)p_msg->payload;

	/* Make sure the destination points beyond the API header*/
	p_dest = p_buff + ICF_API_HEADER_LENGTH;
	ret_val = icf_pack_api_st(p_dest,p_src,&length,p_msg->hdr.api_id, &is_default);

    if(ICF_TRUE == is_default)
    {
        ICF_CLIB_MEMFREE(p_buff);
        /*ret_val = (icf_uint8_t)icf_encode_mm_msg(p_msg, p_p_encoded_buf, &length, p_ecode);*/
        ret_val = (icf_uint8_t)icf_encode_mm_msg(p_msg, p_p_encoded_buf, p_encoded_buf_length, p_ecode);
        return ret_val;
    }
	
    if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - ENCODING Failed While Packing!!!\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_ENCODE_FAILED;
        return ret_val;
    }

	/* The length of the encoded buffer includes
	   the length of API header */
	api_length_value = (icf_uint16_t)(length + hdr_length);

	icf_pack_U16(p_buff + api_length_index, (icf_uint8_t*)&api_length_value, &hdr_length);

	length += ICF_API_HEADER_LENGTH;

	*p_encoded_buf_length = length;
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Getting Memory for p_p_encoded_buf\n"));
	*p_p_encoded_buf = (icf_uint8_t*)ICF_CLIB_MEMGET(length);

	if (!*p_p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure! (*p_p_encoded_buf)\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - MEMCopying API Header to encoded buffer\n"));
	/* Copy the encoded API header first*/
	ICF_CLIB_MEMCPY(*p_p_encoded_buf, p_buff, ICF_API_HEADER_LENGTH)

	/* Copy the rest of the encoded payload, move past the
	   encoded API Header*/
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - MEMCopying Payload to encoded buffer\n"));
	ICF_CLIB_MEMCPY(*p_p_encoded_buf + ICF_API_HEADER_LENGTH, p_dest, length - ICF_API_HEADER_LENGTH)

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - FREEing p_buff\n"));
	ICF_CLIB_MEMFREE(p_buff)

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Encoded Buffer Length (%d) Bytes\n", *p_encoded_buf_length));
    ICF_CLIB_TRACE(("\n<========================= ENCODE END =======================>\n"));
	
	return ret_val;
}




icf_return_t icf_free_encoded_msg(icf_uint8_t* p_encoded_buf, icf_error_t* p_ecode)
{
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_encoded_msg - FREEing encoded Buffer\n"));
	ICF_CLIB_MEMFREE(p_encoded_buf)
    (void)p_ecode;
	return ICF_SUCCESS;
}



/* 
 * The function take the encoded buffer as input and returns api_header
 * present in encoded buffer  as output. This header returned is no more
 * enceoded
 */ 
icf_return_t icf_decode_api_hdr(icf_api_header_st *p_api_hdr, /* Output*/
							icf_uint8_t*  p_encoded_buf,
							icf_uint32_t  encoded_buf_length,
							icf_error_t*  p_ecode)
{

	icf_uint32_t length      = ICF_NULL;
    icf_return_t ret_val     = ICF_SUCCESS;
    /* To remove warning */
    encoded_buf_length=encoded_buf_length;

	if (!p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_api_hdr - Argument #2 p_encoded_buf is NULL\n"));
		*p_ecode = ICF_ERR_NULL_POINTER;
		return ICF_FAILURE;
	}

    /* Unpack the header first*/
	ret_val = (icf_uint8_t)icf_unpack_icf_api_header_st((icf_uint8_t*)p_api_hdr, p_encoded_buf, &length);

    if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_api_hdr - DECODING Failed While Unpacking API Header!!!\n"));
		*p_ecode = ICF_ERR_DECODE_FAILED;
        return ret_val;
    }

	return ret_val;
}

/* 
 * This function would take as input the encoded payload and would copy the
 * decoded payload in the p_msg pointer . Would return failure in case
 * there was come error in decoding
 */ 
icf_return_t icf_decode_msg_payload(icf_uint8_t*  p_msg, /* Output*/
							icf_uint8_t*  p_encoded_buf,
							icf_uint32_t  encoded_buf_length,
                            icf_api_id_t  api_id,
							icf_error_t*  p_ecode)

{

	icf_uint32_t length      = ICF_NULL;
	icf_uint8_t* p_src       = ICF_NULL;
	icf_uint8_t* p_dest      = ICF_NULL;
	icf_uint32_t api_length  = ICF_NULL;
    icf_return_t ret_val     = ICF_SUCCESS;
	icf_boolean_t is_default = ICF_FALSE;

     /* To remove warning */
     api_length=api_length;

	if (!p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg_payload - Argument #2 p_encoded_buf is NULL\n"));
		*p_ecode = ICF_ERR_NULL_POINTER;
		return ICF_FAILURE;
	}

    ICF_CLIB_TRACE(("\n<======================== DECODE START ======================>\n"));

	p_dest = p_msg;

	/* Move past the API header into the buffer*/
	p_src  = p_encoded_buf;
	ret_val = icf_unpack_api_st(p_dest, p_src, &length, api_id, &is_default);

    if(ICF_TRUE == is_default)
    {
        ret_val = (icf_uint8_t)icf_decode_mm_msg_payload((icf_msg_st *)p_msg, p_encoded_buf, encoded_buf_length,api_id,p_ecode);
    }

    if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg_payload - DECODING Failed While Unpacking!!!\n"));
		*p_ecode = ICF_ERR_DECODE_FAILED;
        return ret_val;
    }

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg_payload - Decoded Buffer Length (%d) Bytes\n", length));
    ICF_CLIB_TRACE(("\n<========================= DECODE END =======================>\n"));
	return ret_val;
}



icf_return_t icf_decode_msg(icf_msg_st**  p_p_msg, /* Output*/
							icf_uint8_t*  p_encoded_buf,
							icf_uint32_t  encoded_buf_length,
							icf_error_t*  p_ecode)

{

	icf_uint32_t length      = ICF_NULL;
	icf_uint8_t* p_src       = ICF_NULL;
	icf_uint8_t* p_dest      = ICF_NULL;
	icf_uint32_t api_length  = ICF_NULL;
	icf_uint16_t api_id      = ICF_NULL;
	icf_api_header_st* p_hdr = ICF_NULL;
    icf_return_t ret_val     = ICF_SUCCESS;
	icf_boolean_t is_default = ICF_FALSE;

	if (!p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg - Argument #2 p_encoded_buf is NULL\n"));
		*p_ecode = ICF_ERR_NULL_POINTER;
		return ICF_FAILURE;
	}

    (void)encoded_buf_length;
    
    ICF_CLIB_TRACE(("\n<======================== DECODE START ======================>\n"));

	p_hdr = (icf_api_header_st*)ICF_CLIB_MEMGET(ICF_API_HEADER_LENGTH);

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg - Getting Memory for p_hdr\n"));
	if (!p_hdr) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure()!\n"));
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


	/* Unpack the header first*/
	ret_val = (icf_uint8_t)icf_unpack_icf_api_header_st((icf_uint8_t*)p_hdr, p_encoded_buf, &length);

    if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg - DECODING Failed While Unpacking API Header!!!\n"));
	    ICF_CLIB_MEMFREE(p_hdr)
		*p_ecode = ICF_ERR_DECODE_FAILED;
        return ret_val;
    }

	api_id     = p_hdr->api_id;

	if ((api_id < ICF_EXTERNAL_API_START) && (api_id >ICF_EXTERNAL_API_END))
	{
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg - wrong API id (%d) \n", api_id));
		ICF_CLIB_MEMFREE(p_hdr)
        *p_ecode = ICF_ERR_DECODE_FAILED;
        return ICF_FAILURE;
    }

	api_length = icf_get_api_length(api_id);

	if(0 == api_length)
	{
		ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg - API id (%d) & API Length (%d) Mismatch\n", api_id, api_length));
		ICF_CLIB_MEMFREE(p_hdr)
        *p_ecode = ICF_ERR_INVALID_LENGTH;
        return ICF_FAILURE;
	}

	ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg - Getting Memory for *p_p_msg\n"));
	*p_p_msg = (icf_msg_st*)ICF_CLIB_MEMGET(api_length);

	if (!*p_p_msg) {
		ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure()!\n"));
                ICF_CLIB_MEMFREE(p_hdr)
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}

	p_dest = (*p_p_msg)->payload;

	/* Move past the API header into the buffer*/
	p_src  = p_encoded_buf + ICF_API_HEADER_LENGTH;
	ret_val = icf_unpack_api_st(p_dest, p_src, &length, api_id, &is_default);

    if(ICF_TRUE == is_default)
    {
        ICF_CLIB_MEMFREE(*p_p_msg)
        ret_val = (icf_uint8_t)icf_decode_mm_msg(p_p_msg, p_encoded_buf, encoded_buf_length, p_ecode);
    }


    if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg - DECODING Failed While Unpacking!!!\n"));
	    ICF_CLIB_MEMFREE(*p_p_msg)
		ICF_CLIB_MEMFREE(p_hdr)
		*p_ecode = ICF_ERR_DECODE_FAILED;
        return ret_val;
    }

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg - MEMCopying decoded Header to ICF Message\n"));
	ICF_CLIB_MEMCPY(&((*p_p_msg)->hdr), p_hdr, ICF_API_HEADER_LENGTH);

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg - Freeing Header\n"));
	ICF_CLIB_MEMFREE(p_hdr)

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_msg - Decoded Buffer Length (%d) Bytes\n", length));
    ICF_CLIB_TRACE(("\n<========================= DECODE END =======================>\n"));
	return ret_val;
}




icf_return_t icf_free_decoded_msg(icf_msg_st* p_msg, icf_error_t* p_ecode)
{

    icf_return_t ret_val = ICF_SUCCESS;

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_decoded_msg - FREEing decoded Buffer\n"));
	switch (p_msg->hdr.api_id) {

	/* These API's contain bit masks*/
	case ICF_CREATE_CALL_REQ:			case ICF_LOCAL_USER_ALERTED_REQ:	
	case ICF_CONNECT_REQ:		        case ICF_INCOMING_CALL_IND:	
    case ICF_CONNECT_IND:
	case ICF_REGISTER_REQ:				case ICF_REGISTER_CFM:
	case ICF_CONNECT_RESP:				case ICF_EARLY_INC_CALL_IND:
	case ICF_EARLY_INC_CALL_CFM:
    case ICF_MEDIA_CONNECT_REQ:
	

/*ICF_HANDLE_REINVITE_START*/
	case ICF_INCOMING_CALL_MODIFY_IND:
	case ICF_CALL_MODIFY_CFM:
	case ICF_INCOMING_CALL_MODIFY_CANCEL_IND:
	case ICF_CALL_MODIFY_REQ:	
	case ICF_CALL_MODIFY_RESP:
/*ICF_HANDLE_REINVITE_END*/

	/*ICF REL 6.0*/
	case ICF_MEDIA_CONNECTED_IND:


	/* RN*/
	case ICF_REGISTER_STATUS_IND:				
	case ICF_SUBSCRIBE_REQ:	 
	case ICF_SUBSCRIPTION_STATUS_IND:	case ICF_NOTIFY_IND:
	case ICF_PUBLISH_REQ:				case ICF_PUBLISH_RESP:	   
	case ICF_MESSAGE_REQ:				case ICF_MESSAGE_RESP:
	case ICF_MESSAGE_IND:				case ICF_REFER_REQ:
	case ICF_REFER_RESP:				
	case ICF_GET_SYSTEM_SETTINGS_REQ:	case ICF_GET_SYSTEM_SETTINGS_RESP:
	case ICF_GET_LINE_SETTINGS_REQ:		case ICF_GET_LINE_SETTINGS_RESP:
#ifdef ICF_NW_ACTIVATE
	case ICF_NETWORK_ACTIVATE_REQ:		case ICF_NETWORK_DEACTIVATE_REQ:
#endif
    case ICF_CALL_RESUME_REQ:
    case ICF_CALL_HOLD_REQ:
    case ICF_CALL_RESUME_RESP:
    case ICF_CALL_HOLD_RESP:
    case ICF_CALL_TRANSFER_UNATTENDED_REQ:
	case ICF_CALL_TRANSFER_ATTENDED_REQ:
	case ICF_CALL_TRANSFER_UNATTENDED_RESP:		
    case ICF_CALL_TRANSFER_ATTENDED_RESP:		
    case ICF_REMOTE_CALL_TRANSFER_INITIATED_IND:        
    case ICF_INFO_REQ:
    case ICF_INFO_RESP:
    case ICF_CALL_HOLD_IND:
    case ICF_CALL_RESUME_IND:
    case ICF_CONFERENCE_REQ:
    case ICF_CALL_REDIRECTION_IND:
    /*  Changes for REL 7.0 XCONF */
    case ICF_CREATE_XCONF_REQ:
    case ICF_ADD_XCONF_PARTY_REQ:
    case ICF_DELETE_XCONF_PARTY_REQ:
    case ICF_RELEASE_XCONF_REQ:

    /*   Changes for XCONF end */
    case ICF_NOTIFY_CFM:
	case ICF_INFO_IND:
	case ICF_APP_ADD_CFM:
#ifdef ICF_NW_ACTIVATE
    case ICF_NETWORK_ACTIVATE_CFM:		case ICF_NETWORK_DEACTIVATE_CFM:
#endif
	case ICF_OPTIONS_REQ:				case ICF_OPTIONS_RESP:
/* ICF_6_1_OPTIONS_SUPPORT start */
	case ICF_INC_OPTIONS_IND:			case ICF_OPTIONS_CFM:
/* ICF_6_1_OPTIONS_SUPPORT end */

    case ICF_GET_REGISTER_STATUS_RESP:
    case ICF_APP_ADD_REQ:
    case ICF_SET_LINE_PARAM_REQ:
    case ICF_REMOTE_USER_ALERTED_IND: 
    case ICF_UNEXPECTED_MSG_IND:
    case ICF_REPORT_TRACE_IND:
		ret_val = icf_free_msg(p_msg);
		break;

    case ICF_SET_TRANSPORT_REQ:     
	case ICF_SET_CALL_PARAM_REQ:		case ICF_SET_SELF_ID_REQ:  
	case ICF_TERMINATE_CALL_REQ:		case ICF_TERMINATE_CALL_IND:
    case ICF_SET_REGISTRATION_REQ:		case ICF_CONFIGURE_CFM:
    case ICF_APP_REMOVE_REQ:	  	    case ICF_APP_REMOVE_CFM:
#ifdef ICF_NW_ACTIVATE
	case ICF_EVENT_INDICATION:	
#endif
    case ICF_STOP_FAX_REQ:			case ICF_STOP_FAX_RESP:
	case ICF_STOP_FAX_IND:

   /*  Changes for REL 7.0 XCONF */
    case ICF_CREATE_XCONF_RESP: 
    case ICF_ADD_XCONF_PARTY_RESP:
    case ICF_DELETE_XCONF_PARTY_RESP:
    case ICF_RELEASE_XCONF_RESP:
    case ICF_RELEASE_XCONF_IND:
    /*  Changes End */
    case ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM:
    case ICF_PROGRESS_IND:
		ICF_CLIB_MEMFREE(p_msg);
		break;

    case ICF_CALL_MODIFY_CANCEL_REQ:			case ICF_CALL_MODIFY_CANCEL_RESP:
		ICF_CLIB_MEMFREE(p_msg);
		break;

	case ICF_XCAP_CREATE_REPLACE_RES:
		ICF_CLIB_MEMFREE(((icf_create_replace_res_req_st*)(p_msg->payload))->payload);
		ICF_CLIB_MEMFREE(p_msg);
		break;

	case ICF_SYNCML_SEND_REQ:
		ICF_CLIB_MEMFREE(((icf_syncml_send_req_st*)(p_msg->payload))->payload);
		ICF_CLIB_MEMFREE(p_msg);
		break;

	case ICF_MGMT_PLANE_NW_RESP:
		ICF_CLIB_MEMFREE(((icf_mgmt_plane_nw_resp_st*)(p_msg->payload))->payload);
		ICF_CLIB_MEMFREE(p_msg);
		break;

		
    default:
		/*ret_val = icf_free_msg(p_msg);*/
		ret_val = icf_free_decoded_mm_msg(p_msg,p_ecode);
	    break;
	}

    (void)p_ecode;

	return ret_val;
}

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
)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_uint32_t    length = ICF_NULL;

    switch (api_id) 
    {
    case ICF_MEDIA_CONNECT_REQ:
        ret_val = (icf_uint8_t)icf_pack_icf_media_connect_req_st(p_dest, 
                   p_src,
                   &length);
        break;

	case ICF_APP_ADD_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_app_add_req_st(p_dest, p_src, &length);
		break;	

	case ICF_GET_SYSTEM_SETTINGS_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_get_sys_settings_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_GET_SYSTEM_SETTINGS_RESP:											
		ret_val = (icf_uint8_t)icf_pack_icf_get_sys_settings_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_GET_LINE_SETTINGS_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_get_line_settings_req_st(p_dest, p_src, &length);
		break;
			
	case ICF_GET_LINE_SETTINGS_RESP:											
		ret_val = (icf_uint8_t)icf_pack_icf_get_line_settings_resp_st(p_dest, p_src, &length);
		break;

	case ICF_APP_ADD_CFM:											
		ret_val = (icf_uint8_t)icf_pack_icf_app_add_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_SET_TRANSPORT_REQ:									
		ret_val = (icf_uint8_t)icf_pack_icf_set_transport_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_CALL_PARAM_REQ:								
		ret_val = (icf_uint8_t)icf_pack_icf_set_call_params_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_SELF_ID_REQ:									
		ret_val = (icf_uint8_t)icf_pack_icf_set_self_id_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_REGISTRATION_REQ:								
		ret_val = (icf_uint8_t)icf_pack_icf_set_registration_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_LINE_PARAM_REQ:											
		ret_val = (icf_uint8_t) icf_pack_icf_set_line_param_req_st(p_dest, p_src, &length);
		break;

#if 0 
    case ICF_SET_SCALE_PARAMS_REQ :								
		ret_val = (icf_uint8_t)icf_pack_icf_set_scale_params_req_st(p_dest, p_src, &length);
		break;
#endif


#ifdef ICF_SERVICE_ENABLE
	case ICF_SET_SERVICE_PARAMS_REQ:								
		ret_val = (icf_uint8_t)icf_pack_icf_set_service_params_req_st(p_dest, p_src, &length);
		break;
#endif


/* RN*/
/*	case ICF_CONFIGURE_COMPLETE_REQ:	// Not Done							*/
/*		break;*/

	case ICF_CONFIGURE_CFM:
		ret_val = (icf_uint8_t)icf_pack_icf_configure_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_CREATE_CALL_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_create_call_req_st(p_dest, p_src, &length);
		break;

	case ICF_LOCAL_USER_ALERTED_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_local_user_alerted_req_st(p_dest, p_src, &length);
		break;

	case ICF_TERMINATE_CALL_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_terminate_call_req_st(p_dest, p_src, &length);
		break;

/* RN*/
/*	case ICF_TERMINATE_CALL_RESP:		// Not Done							*/
/*		break;*/

	case ICF_CONNECT_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_connect_req_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_connect_resp_st(p_dest, p_src, &length);
		break;

	case ICF_INCOMING_CALL_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_incoming_call_ind_st(p_dest, p_src, &length);
		break;
	case ICF_EARLY_INC_CALL_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_early_incoming_call_ind_st(p_dest, p_src, &length);
		break;
	case ICF_EARLY_INC_CALL_CFM:
		ret_val = (icf_uint8_t)icf_pack_icf_early_incoming_call_cfm_st(p_dest, p_src, &length);
		break;

/*ICF_HANDLE_REINVITE_START*/
	case ICF_INCOMING_CALL_MODIFY_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_incoming_call_modify_ind_st(p_dest, p_src, &length);
		break;

	case ICF_CALL_MODIFY_CFM:
		ret_val = (icf_uint8_t)icf_pack_icf_call_modify_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_INCOMING_CALL_MODIFY_CANCEL_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_incoming_call_modify_cancel_ind_st(p_dest, p_src, &length);
		break;

	case ICF_CALL_MODIFY_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_call_modify_req_st(p_dest, p_src, &length);
		break;

	case ICF_CALL_MODIFY_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_call_modify_resp_st(p_dest, p_src, &length);
		break;
/*ICF_HANDLE_REINVITE_END*/

	case ICF_REMOTE_USER_ALERTED_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_remote_user_alerted_ind_st(p_dest, p_src, &length);
		break;

	case ICF_TERMINATE_CALL_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_terminate_call_ind_st(p_dest, p_src, &length);
		break;

    case ICF_PROGRESS_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_progress_ind_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_CONNECTED_IND:		/*  Done in Rel 6.0							*/
		ret_val = (icf_uint8_t)icf_pack_icf_media_connected_ind_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_connect_ind_st(p_dest, p_src, &length);
		break;

	case ICF_REGISTER_REQ:										
		ret_val = (icf_uint8_t)icf_pack_icf_register_req_st(p_dest, p_src, &length);
		break;

	case ICF_REGISTER_CFM:										
		ret_val = (icf_uint8_t)icf_pack_icf_register_cfm_st(p_dest, p_src, &length);
		break;
/* RN*/
/*	case ICF_REGISTER_IND:										*/
	case ICF_REGISTER_STATUS_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_register_status_ind_st(p_dest, p_src, &length);
		break;

#ifdef ICF_NW_ACTIVATE
	case ICF_NETWORK_ACTIVATE_REQ:								
		ret_val = (icf_uint8_t)icf_pack_icf_network_activate_req_st(p_dest, p_src, &length);
		break;

	case ICF_EVENT_INDICATION:								
		ret_val = (icf_uint8_t)icf_pack_icf_event_indication_st(p_dest, p_src, &length);
		break;

	case ICF_NETWORK_ACTIVATE_CFM:								
		ret_val = (icf_uint8_t)icf_pack_icf_network_activate_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_NETWORK_DEACTIVATE_REQ:							
		ret_val = (icf_uint8_t)icf_pack_icf_network_deactivate_req_st(p_dest, p_src, &length);
		break;

	case ICF_NETWORK_DEACTIVATE_CFM:								
		ret_val = (icf_uint8_t)icf_pack_icf_network_deactivate_cfm_st(p_dest, p_src, &length);
		break;
#endif

	case ICF_APP_REMOVE_REQ:										
		ret_val = (icf_uint8_t)icf_pack_icf_app_remove_req_st(p_dest, p_src, &length);
		break;

	case ICF_APP_REMOVE_CFM:										
		ret_val = (icf_uint8_t)icf_pack_icf_app_remove_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_SUBSCRIBE_REQ:										
		ret_val = (icf_uint8_t)icf_pack_icf_subscribe_req_st(p_dest, p_src, &length);
		break;

	case ICF_SUBSCRIPTION_STATUS_IND:										
		ret_val = (icf_uint8_t)icf_pack_icf_subscribe_resp_st(p_dest, p_src, &length);
		break;

	case ICF_NOTIFY_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_notify_ind_st(p_dest, p_src, &length);
		break;

    case ICF_NOTIFY_CFM:
		ret_val = (icf_uint8_t)icf_pack_icf_notify_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_PUBLISH_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_publish_req_st(p_dest, p_src, &length);
		break;

	case ICF_PUBLISH_RESP:										
		ret_val = (icf_uint8_t)icf_pack_icf_publish_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MESSAGE_REQ:											
		ret_val = (icf_uint8_t)icf_pack_icf_message_req_st(p_dest, p_src, &length);
		break;

	case ICF_MESSAGE_RESP:										
		ret_val = (icf_uint8_t)icf_pack_icf_message_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MESSAGE_IND:											
		ret_val = (icf_uint8_t)icf_pack_icf_message_ind_st(p_dest, p_src, &length);
		break;

	case ICF_REFER_REQ:	
		ret_val = (icf_uint8_t)icf_pack_icf_refer_req_st(p_dest, p_src, &length);
		break;

	case ICF_REFER_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_refer_resp_st(p_dest, p_src, &length);
		break;

    case ICF_TRACE_NW_BUFF_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_nw_buff_ind_st(p_dest, p_src, &length);
		break;

/*****************************************************************************/
/*************** Management Plane Handler (MPH) APIs Starts here *************/
/*****************************************************************************/
		
	case ICF_SET_MGMT_PLANE_AUTH_PARAM:
		ret_val = (icf_uint8_t)icf_pack_icf_set_mgmt_plane_auth_param_req_st(p_dest, p_src, &length);
		break;
		
	case ICF_CREATE_SECURE_CONN_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_create_secure_conn_req_st(p_dest, p_src, &length);
		break;

	case ICF_CREATE_SECURE_CONN_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_create_secure_conn_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CLOSE_SECURE_CONN_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_close_secure_conn_req_st(p_dest, p_src, &length);
		break;

	case ICF_CLOSE_SECURE_CONN_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_close_secure_conn_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SECURE_CONN_STATUS_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_secure_conn_status_ind_st(p_dest, p_src, &length);
		break;

	case ICF_XCAP_CREATE_REPLACE_RES:
		ret_val = (icf_uint8_t)icf_pack_icf_create_replace_res_req_st(p_dest, p_src, &length);
		break;

	case ICF_XCAP_DELETE_RES:
		ret_val = (icf_uint8_t)icf_pack_icf_delete_res_req_st(p_dest, p_src, &length);
		break;

	case ICF_XCAP_FETCH_RES:
		ret_val = (icf_uint8_t)icf_pack_icf_fetch_res_req_st(p_dest, p_src, &length);
		break;

	case ICF_SYNCML_SEND_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_syncml_send_req_st(p_dest, p_src, &length);
		break;

	case ICF_MGMT_PLANE_API_PROC_FAILURE:
		ret_val = (icf_uint8_t)icf_pack_icf_api_proc_failure_st(p_dest, p_src, &length);
		break;

	case ICF_MGMT_PLANE_NW_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_mgmt_plane_nw_resp_st(p_dest, p_src, &length);
		break;


		
/*****************************************************************************/
/*************** Management Plane Handler (MPH) APIs Ends here ***************/
/*****************************************************************************/

	case ICF_RES_UNAVAIL_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_res_unavail_ind_st(p_dest, p_src, &length);
		break;		
		
#ifdef ICF_SERVICE_ENABLE
        /*   XCONF Changes for REL 7.0*/
	case ICF_CREATE_XCONF_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_create_xconf_req_st(p_dest, p_src, &length); 
		break;
    	case ICF_ADD_XCONF_PARTY_REQ:
 		ret_val = (icf_uint8_t)icf_pack_icf_add_xconf_party_req_st(p_dest, p_src, &length);
 		break;
	case ICF_DELETE_XCONF_PARTY_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_delete_xconf_party_req_st(p_dest, p_src, &length);
		break;
	case ICF_RELEASE_XCONF_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_release_xconf_req_st(p_dest, p_src, &length);
		break;
	case ICF_CREATE_XCONF_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_create_xconf_resp_st(p_dest, p_src, &length);
 		break;
	case ICF_ADD_XCONF_PARTY_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_add_xconf_party_resp_st(p_dest, p_src, &length);
		break;
	case ICF_DELETE_XCONF_PARTY_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_delete_xconf_party_resp_st(p_dest, p_src, &length);
		break;
        case ICF_RELEASE_XCONF_RESP:
                ret_val = (icf_uint8_t)icf_pack_icf_release_xconf_resp_st(p_dest, p_src, &length);
                 break;
 
	case ICF_RELEASE_XCONF_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_release_xconf_ind_st(p_dest, p_src, &length);
		break;
/*  7.0 eND*/
	case ICF_CONFERENCE_REQ:								
		ret_val = (icf_uint8_t)icf_pack_icf_conference_req_st(p_dest, p_src, &length);
		break;
	case ICF_CONFERENCE_RESP:								
		ret_val = (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;
    case ICF_CALL_TRANSFER_UNATTENDED_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_ctu_req_st(p_dest, p_src, &length);
		break;

        case ICF_CALL_TRANSFER_ATTENDED_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_cta_req_st(p_dest, p_src, &length);
		break;

        case ICF_CALL_HOLD_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_call_hold_req_st(p_dest, p_src, &length);
		break;

        case ICF_CALL_RESUME_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_call_resume_req_st(p_dest, p_src, &length);
		break;
        case ICF_CALL_HOLD_RESP:
        case ICF_CALL_RESUME_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_api_mesg_resp_st(p_dest, p_src, &length);
		break;
        case ICF_CALL_TRANSFER_UNATTENDED_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_ctu_resp_st(p_dest, p_src, &length);
		break;
        case ICF_CALL_TRANSFER_ATTENDED_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_cta_resp_st(p_dest, p_src, &length);
		break;
        case ICF_REMOTE_CALL_TRANSFER_INITIATED_IND:
        ret_val = (icf_uint8_t)icf_pack_icf_remote_call_transfer_initiated_ind_st(p_dest, p_src, &length);
		break;
        case ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM:
        ret_val = (icf_uint8_t)icf_pack_icf_remote_call_transfer_initiated_cfm_st(p_dest, p_src, &length);
		break;
#endif
        case ICF_INFO_REQ:
        ret_val = (icf_uint8_t)icf_pack_icf_info_req_st(p_dest, p_src, &length);
		break;

    	case ICF_INFO_RESP:
        ret_val = (icf_uint8_t)icf_pack_icf_info_resp_st(p_dest, p_src, &length);
		break;
   
    	case ICF_INFO_IND:
        ret_val = (icf_uint8_t)icf_pack_icf_info_ind_st(p_dest, p_src, &length);
		break;

        case ICF_INFO_CFM:
        ret_val = (icf_uint8_t)icf_pack_icf_info_cfm_st(p_dest, p_src, &length);
		break; 

	 	case ICF_CALL_HOLD_IND:
    	case ICF_CALL_RESUME_IND:
        ret_val = (icf_uint8_t)icf_pack_icf_api_ind_st(p_dest, p_src, &length);
        break;

        case ICF_CALL_REDIRECTION_IND:
        ret_val = (icf_uint8_t)icf_pack_icf_redirection_ind_st(p_dest, p_src, &length);
		break;

        case ICF_PARTY_REPLACED_IND:
        ret_val = (icf_uint8_t)icf_pack_icf_party_replaced_ind(p_dest, p_src, &length);
		break;  

#ifdef ICF_FAX_SUPPORT
		case ICF_START_FAX_REQ:
        ret_val = (icf_uint8_t)icf_pack_icf_start_fax_req_st(p_dest, p_src, &length);
		break;

		case ICF_START_FAX_RESP:
        ret_val = (icf_uint8_t)icf_pack_icf_start_fax_resp_st(p_dest, p_src, &length);
		break;

		case ICF_START_FAX_IND:
        ret_val = (icf_uint8_t)icf_pack_icf_start_fax_ind_st(p_dest, p_src, &length);
		break;
		case ICF_STOP_FAX_RESP:
        	ret_val = (icf_uint8_t) icf_pack_icf_stop_fax_resp_st(p_dest, p_src, &length);
       	 	break;

		case ICF_STOP_FAX_IND:
        	ret_val = (icf_uint8_t) icf_pack_icf_stop_fax_ind_st(p_dest, p_src, &length);
       	 	break;
#endif

/*call_modify_cancel*/
		case ICF_CALL_MODIFY_CANCEL_RESP:
        	ret_val = (icf_uint8_t) icf_pack_icf_call_mod_cancel_resp_st(p_dest, p_src, &length);
       	 	break;

		case ICF_CALL_FORWARDED_IND:
        ret_val = (icf_uint8_t)icf_pack_icf_call_forwarded_ind_st(p_dest, p_src, &length);
		break;

		case ICF_REMOTE_USER_TERMINATE_CALL_RESP:
			break;

		case ICF_OPTIONS_RESP:
		ret_val = (icf_uint8_t)icf_pack_icf_options_resp_st(p_dest, p_src, &length);
		break;
/* ICF_6_1_OPTIONS_SUPPORT start */
	case ICF_INC_OPTIONS_IND:
		ret_val = (icf_uint8_t)icf_pack_icf_inc_option_ind_st(p_dest, p_src, &length);
		break;

	case ICF_OPTIONS_CFM:
		ret_val = (icf_uint8_t)icf_pack_icf_icf_options_cfm_st(p_dest, p_src, &length);
		break;
	case ICF_OPTIONS_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_options_req_st(p_dest, p_src, &length);
		break;

/* ICF_6_1_OPTIONS_SUPPORT end */

#ifdef ICF_TRACE_ENABLE
	case ICF_SET_TRACE_LEVEL_REQ:
    		ret_val = (icf_uint8_t)icf_pack_icf_trace_level_req_st(p_dest, p_src,
          									&length);
      		break;
    case ICF_REPORT_TRACE_IND:
            ret_val = (icf_uint8_t)icf_pack_icf_report_trace_ind_st(p_dest, p_src, &length);
            break;
     
    case ICF_SET_TRACE_REQ:
            ret_val = (icf_uint8_t)icf_pack_icf_set_trace_req_st(p_dest, p_src, &length);
            break;
#endif

#ifdef ICF_ERROR_ENABLE
	case ICF_SET_ERROR_REPORT_LEVEL_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_error_report_level_req_st(p_dest, p_src,
          									&length);
      		break;

    case ICF_REPORT_ERROR_IND:
        ret_val = (icf_uint8_t)icf_pack_icf_report_error_ind_st(
                    p_dest, p_src, &length);
        break;

#endif
	case ICF_LOCAL_USER_TERMINATE_CALL_RESP:
			break;
        case ICF_RESTART_REQ:
               ret_val = ICF_SUCCESS;
              break;
	case ICF_REINITIALISE_REQ:
	      	ret_val = ICF_SUCCESS;
		break;
        case ICF_REINIT_IND:
               ret_val = ICF_SUCCESS;
              break;
        case ICF_REINIT_RESP:
               ret_val = ICF_SUCCESS;
              break;
        case ICF_RESTART_IND:
               ret_val = ICF_SUCCESS;
              break;
        case ICF_RESTART_RESP:
               ret_val = ICF_SUCCESS;
              break;
              #ifdef ICF_STAT_ENABLE

        case ICF_STATS_REQ:
             ret_val = (icf_uint8_t)icf_pack_icf_stats_req_st(p_dest, p_src, &length);
             break;

        case ICF_STATS_RESP:
             ret_val = (icf_uint8_t)icf_pack_icf_stats_resp_st(p_dest, p_src, &length);
             break;
#endif
        case ICF_GET_REGISTER_STATUS_REQ:
             ret_val = (icf_uint8_t)icf_pack_icf_get_register_status_req_st(p_dest, p_src, &length);
             break;

        case ICF_GET_REGISTER_STATUS_RESP:
             ret_val = (icf_uint8_t)icf_pack_icf_get_register_status_resp_st(p_dest, p_src, &length);
             break;

		case ICF_UNEXPECTED_MSG_IND:
                ret_val = (icf_uint8_t)icf_pack_icf_unexpected_msg_ind_st(p_dest, p_src, &length);
                break;
		default:
            *p_default=ICF_TRUE;

        break;
	}

    *p_length=length;
    return ret_val;
}

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
)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_uint32_t    length = ICF_NULL;

    switch (api_id) 
    {

	case ICF_GET_SYSTEM_SETTINGS_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_get_sys_settings_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_GET_SYSTEM_SETTINGS_RESP:											
		ret_val = (icf_uint8_t)icf_unpack_icf_get_sys_settings_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_GET_LINE_SETTINGS_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_get_line_settings_req_st(p_dest, p_src, &length);
		break;
			
	case ICF_GET_LINE_SETTINGS_RESP:											
		ret_val = (icf_uint8_t)icf_unpack_icf_get_line_settings_resp_st(p_dest, p_src, &length);
		break;

	case ICF_APP_ADD_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_app_add_req_st(p_dest, p_src, &length);
		break;				

	case ICF_APP_ADD_CFM:											
		ret_val = (icf_uint8_t)icf_unpack_icf_app_add_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_SET_TRANSPORT_REQ:									
		ret_val = (icf_uint8_t)icf_unpack_icf_set_transport_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_CALL_PARAM_REQ:								
		ret_val = (icf_uint8_t)icf_unpack_icf_set_call_params_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_SELF_ID_REQ:									
		ret_val = (icf_uint8_t)icf_unpack_icf_set_self_id_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_REGISTRATION_REQ:								
		ret_val = (icf_uint8_t)icf_unpack_icf_set_registration_req_st(p_dest, p_src, &length);
		break;
#if 0
    case ICF_SET_SCALE_PARAMS_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_set_scale_params_req_st(p_dest, p_src, &length);
		break;
#endif

    case ICF_CONFIGURE_CFM:
		ret_val = (icf_uint8_t)icf_unpack_icf_configure_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_CREATE_CALL_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_create_call_req_st(p_dest, p_src, &length);
		break;

	case ICF_LOCAL_USER_ALERTED_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_local_user_alerted_req_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_CONNECT_REQ:
        ret_val = (icf_uint8_t)icf_unpack_icf_media_connect_req_st(p_dest, p_src, &length);
		break;

	case ICF_UNEXPECTED_MSG_IND:
                ret_val = (icf_uint8_t)icf_unpack_icf_unexpected_msg_ind_st(p_dest, p_src, &length);
                break;

	case ICF_TERMINATE_CALL_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_terminate_call_req_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_connect_req_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_RESP:
		ret_val = (icf_uint8_t)icf_unpack_icf_connect_resp_st(p_dest, p_src, &length);
		break;

	case ICF_INCOMING_CALL_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_incoming_call_ind_st(p_dest, p_src, &length);
		break;

	case ICF_EARLY_INC_CALL_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_early_incoming_call_ind_st(p_dest, p_src, &length);
		break;
	case ICF_EARLY_INC_CALL_CFM:
		ret_val = (icf_uint8_t)icf_unpack_icf_early_incoming_call_cfm_st(p_dest, p_src, &length);
		break;

/*ICF_HANDLE_REINVITE_START*/
	case ICF_INCOMING_CALL_MODIFY_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_incoming_call_modify_ind_st(p_dest, p_src, &length);
		break;

	case ICF_CALL_MODIFY_CFM:
		ret_val = (icf_uint8_t)icf_unpack_icf_call_modify_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_INCOMING_CALL_MODIFY_CANCEL_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_incoming_call_modify_cancel_ind_st(p_dest, p_src, &length);
		break;

	case ICF_CALL_MODIFY_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_call_modify_req_st(p_dest, p_src, &length);
		break;

	case ICF_CALL_MODIFY_RESP:
		ret_val = (icf_uint8_t)icf_unpack_icf_call_modify_resp_st(p_dest, p_src, &length);
		break;
/*ICF_HANDLE_REINVITE_END*/

	case ICF_REMOTE_USER_ALERTED_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_remote_user_alerted_ind_st(p_dest, p_src, &length);
		break;

	case ICF_TERMINATE_CALL_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_terminate_call_ind_st(p_dest, p_src, &length);
		break;

    case ICF_PROGRESS_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_progress_ind_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_CONNECTED_IND:		/*  Done in Rel 6.0		*/
		ret_val = (icf_uint8_t)icf_unpack_icf_media_connected_ind_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_connect_ind_st(p_dest, p_src, &length);
		break;

	case ICF_REGISTER_REQ:										
		ret_val = (icf_uint8_t)icf_unpack_icf_register_req_st(p_dest, p_src, &length);
		break;

	case ICF_REGISTER_CFM:										
		ret_val = (icf_uint8_t)icf_unpack_icf_register_cfm_st(p_dest, p_src, &length);
		break;

    case ICF_REGISTER_STATUS_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_register_status_ind_st(p_dest, p_src, &length);
		break;

#ifdef ICF_NW_ACTIVATE
	case ICF_NETWORK_ACTIVATE_REQ:								
		ret_val = (icf_uint8_t)icf_unpack_icf_network_activate_req_st(p_dest, p_src, &length);
		break;

	case ICF_EVENT_INDICATION:								
		ret_val = (icf_uint8_t)icf_unpack_icf_event_indication_st(p_dest, p_src, &length);
		break;

	case ICF_NETWORK_ACTIVATE_CFM:								
		ret_val = (icf_uint8_t)icf_unpack_icf_network_activate_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_NETWORK_DEACTIVATE_REQ:							
		ret_val = (icf_uint8_t)icf_unpack_icf_network_deactivate_req_st(p_dest, p_src, &length);
		break;

	case ICF_NETWORK_DEACTIVATE_CFM:								
		ret_val = (icf_uint8_t)icf_unpack_icf_network_deactivate_cfm_st(p_dest, p_src, &length);
		break;
#endif

	case ICF_APP_REMOVE_REQ:										
		ret_val = (icf_uint8_t)icf_unpack_icf_app_remove_req_st(p_dest, p_src, &length);
		break;

	case ICF_APP_REMOVE_CFM:										
		ret_val = (icf_uint8_t)icf_unpack_icf_app_remove_cfm_st(p_dest, p_src, &length);
		break;

	case ICF_SUBSCRIBE_REQ:										
		ret_val = (icf_uint8_t)icf_unpack_icf_subscribe_req_st(p_dest, p_src, &length);
		break;

	case ICF_SUBSCRIPTION_STATUS_IND:										
		ret_val = (icf_uint8_t)icf_unpack_icf_subscribe_resp_st(p_dest, p_src, &length);
		break;

	case ICF_NOTIFY_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_notify_ind_st(p_dest, p_src, &length);
		break;

/*NOTIFY_OUT_OF_DIALOG_START*/
	case ICF_NOTIFY_CFM:
		ret_val = (icf_uint8_t)icf_unpack_icf_notify_cfm_st(p_dest, p_src, &length);
		break;
/*NOTIFY_OUT_OF_DIALOG_END*/

	case ICF_PUBLISH_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_publish_req_st(p_dest, p_src, &length);
		break;

	case ICF_PUBLISH_RESP:										
		ret_val = (icf_uint8_t)icf_unpack_icf_publish_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MESSAGE_REQ:											
		ret_val = (icf_uint8_t)icf_unpack_icf_message_req_st(p_dest, p_src, &length);
		break;

	case ICF_MESSAGE_RESP:										
		ret_val = (icf_uint8_t)icf_unpack_icf_message_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MESSAGE_IND:											
		ret_val = (icf_uint8_t)icf_unpack_icf_message_ind_st(p_dest, p_src, &length);
		break;

	case ICF_REFER_REQ:	
		ret_val = (icf_uint8_t)icf_unpack_icf_refer_req_st(p_dest, p_src, &length);
		break;

	case ICF_REFER_RESP:
		ret_val = (icf_uint8_t)icf_unpack_icf_refer_resp_st(p_dest, p_src, &length);
		break;        

    case ICF_TRACE_NW_BUFF_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_nw_buff_ind_st(p_dest, p_src, &length);
		break;


#ifdef ICF_SERVICE_ENABLE
	case ICF_CREATE_XCONF_REQ:
                ret_val = (icf_uint8_t)icf_unpack_icf_create_xconf_req_st(p_dest, p_src, &length);
                break;
        case ICF_ADD_XCONF_PARTY_REQ:
                ret_val = (icf_uint8_t)icf_unpack_icf_add_xconf_party_req_st(p_dest, p_src, &length);
                break;
        case ICF_DELETE_XCONF_PARTY_REQ:
                ret_val = (icf_uint8_t)icf_unpack_icf_delete_xconf_party_req_st(p_dest, p_src, &length);
                break;
        case ICF_RELEASE_XCONF_REQ:
                ret_val = (icf_uint8_t)icf_unpack_icf_release_xconf_req_st(p_dest, p_src, &length);
                break;
        case ICF_CREATE_XCONF_RESP:
                ret_val = (icf_uint8_t)icf_unpack_icf_create_xconf_resp_st(p_dest, p_src, &length);
                break;
        case ICF_ADD_XCONF_PARTY_RESP:
                ret_val = (icf_uint8_t)icf_unpack_icf_add_xconf_party_resp_st(p_dest, p_src, &length);
                break;
        case ICF_DELETE_XCONF_PARTY_RESP:
                ret_val = (icf_uint8_t)icf_unpack_icf_delete_xconf_party_resp_st(p_dest, p_src, &length);
                break;
        case ICF_RELEASE_XCONF_IND:
                ret_val = (icf_uint8_t)icf_unpack_icf_release_xconf_ind_st(p_dest, p_src, &length);
                break;
        case ICF_RELEASE_XCONF_RESP:
                ret_val = (icf_uint8_t)icf_unpack_icf_release_xconf_resp_st(p_dest, p_src, &length);
                break;
	case ICF_SET_SERVICE_PARAMS_REQ:								
		ret_val = (icf_uint8_t)icf_unpack_icf_set_service_params_req_st(p_dest, p_src, &length);
		break;

#endif

/*****************************************************************************/
/*************** Management Plane Handler (MPH) APIs Starts here *************/
/*****************************************************************************/
		
	case ICF_SET_MGMT_PLANE_AUTH_PARAM:
		ret_val = (icf_uint8_t)icf_unpack_icf_set_mgmt_plane_auth_param_req_st(p_dest, p_src, &length);
		break;
		
	case ICF_CREATE_SECURE_CONN_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_create_secure_conn_req_st(p_dest, p_src, &length);
		break;

	case ICF_CREATE_SECURE_CONN_RESP:
		ret_val = (icf_uint8_t)icf_unpack_icf_create_secure_conn_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CLOSE_SECURE_CONN_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_close_secure_conn_req_st(p_dest, p_src, &length);
		break;

	case ICF_CLOSE_SECURE_CONN_RESP:
		ret_val = (icf_uint8_t)icf_unpack_icf_close_secure_conn_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SECURE_CONN_STATUS_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_secure_conn_status_ind_st(p_dest, p_src, &length);
		break;

	case ICF_XCAP_CREATE_REPLACE_RES:
		ret_val = (icf_uint8_t)icf_unpack_icf_create_replace_res_req_st(p_dest, p_src, &length);
		break;

	case ICF_XCAP_DELETE_RES:
		ret_val = (icf_uint8_t)icf_unpack_icf_delete_res_req_st(p_dest, p_src, &length);
		break;

	case ICF_XCAP_FETCH_RES:
		ret_val = (icf_uint8_t)icf_unpack_icf_fetch_res_req_st(p_dest, p_src, &length);
		break;

	case ICF_SYNCML_SEND_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_syncml_send_req_st(p_dest, p_src, &length);
		break;

	case ICF_MGMT_PLANE_API_PROC_FAILURE:
		ret_val = (icf_uint8_t)icf_unpack_icf_api_proc_failure_st(p_dest, p_src, &length);
		break;

	case ICF_MGMT_PLANE_NW_RESP:
		ret_val = (icf_uint8_t)icf_unpack_icf_mgmt_plane_nw_resp_st(p_dest, p_src, &length);
		break;
		
/*****************************************************************************/
/*************** Management Plane Handler (MPH) APIs Ends here ***************/
/*****************************************************************************/

	case ICF_RES_UNAVAIL_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_res_unavail_ind_st(p_dest, p_src,&length);
		break;		

#ifdef ICF_SERVICE_ENABLE
	case ICF_CONFERENCE_REQ:								
		ret_val = (icf_uint8_t)icf_unpack_icf_conference_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_CONFERENCE_RESP:								
		ret_val = (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

    case ICF_CALL_TRANSFER_UNATTENDED_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_ctu_req_st(p_dest, p_src, &length);
		break;

    case ICF_CALL_TRANSFER_ATTENDED_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_cta_req_st(p_dest, p_src, &length);
		break;

    case ICF_CALL_HOLD_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_call_hold_req_st(p_dest, p_src, &length);
		break;

    case ICF_CALL_RESUME_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_call_resume_req_st(p_dest, p_src, &length);
		break;
    case ICF_CALL_RESUME_RESP:
    case ICF_CALL_HOLD_RESP:
		ret_val = (icf_uint8_t)icf_unpack_icf_api_mesg_resp_st(p_dest, p_src, &length);
		break;    
	case ICF_CALL_TRANSFER_UNATTENDED_RESP:
		ret_val = (icf_uint8_t)icf_unpack_icf_ctu_resp_st(p_dest, p_src, &length);
		break;
    case ICF_CALL_TRANSFER_ATTENDED_RESP:
		ret_val = (icf_uint8_t)icf_unpack_icf_cta_resp_st(p_dest, p_src, &length);
		break;
    case ICF_REMOTE_CALL_TRANSFER_INITIATED_IND:
        ret_val = (icf_uint8_t)icf_unpack_icf_remote_call_transfer_initiated_ind_st(p_dest, p_src, &length);
		break;
    case ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM:
	ret_val = (icf_uint8_t)icf_unpack_icf_remote_call_transfer_initiated_cfm_st(p_dest,
                p_src, &length);
		break;
#endif
    case ICF_INFO_REQ:
        ret_val = (icf_uint8_t)icf_unpack_icf_info_req_st(p_dest, p_src, &length);
        break;

	case ICF_INFO_RESP:
        ret_val = (icf_uint8_t)icf_unpack_icf_info_resp_st(p_dest, p_src, &length);
		break;

	case ICF_INFO_IND:
        ret_val = (icf_uint8_t)icf_unpack_icf_info_ind_st(p_dest, p_src, &length);
        break;

        case ICF_INFO_CFM:
        ret_val = (icf_uint8_t)icf_unpack_icf_info_cfm_st(p_dest, p_src, &length);
        break;

    case ICF_CALL_HOLD_IND:
    case ICF_CALL_RESUME_IND:
        ret_val = (icf_uint8_t)icf_unpack_icf_api_ind_st(p_dest, p_src, &length);
        break;

    case ICF_CALL_REDIRECTION_IND:
        ret_val = (icf_uint8_t)icf_unpack_icf_redirection_ind_st(p_dest, p_src, &length);
		break;

    case ICF_PARTY_REPLACED_IND:
        ret_val = (icf_uint8_t)icf_unpack_icf_party_replaced_ind_st(p_dest, p_src, &length);
		break;

#ifdef ICF_FAX_SUPPORT
	case ICF_START_FAX_REQ:
        ret_val = (icf_uint8_t)icf_unpack_icf_start_fax_req_st(p_dest, p_src, &length);
		break;

	case ICF_START_FAX_RESP:
        ret_val = (icf_uint8_t)icf_unpack_icf_start_fax_resp_st(p_dest, p_src, &length);
		break;

	case ICF_START_FAX_IND:
        ret_val = (icf_uint8_t)icf_unpack_icf_start_fax_ind_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_FAX_RESP:
        	ret_val = (icf_uint8_t) icf_unpack_icf_stop_fax_resp_st(p_dest, p_src, &length);
        	break;
 
    case ICF_STOP_FAX_IND:
        	ret_val = (icf_uint8_t) icf_unpack_icf_stop_fax_ind_st(p_dest, p_src, &length);
        	break;
       
       case ICF_STOP_FAX_REQ:
                break; 
#endif

/*call_modify_cancel*/
	case ICF_CALL_MODIFY_CANCEL_RESP:
        	ret_val = (icf_uint8_t)icf_unpack_icf_call_mod_cancel_resp_st(p_dest, p_src, &length);
        	break;
	case ICF_CALL_FORWARDED_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_call_forwarded_ind_st(p_dest, p_src, &length);
		break;
	
	case ICF_OPTIONS_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_options_req_st(p_dest, p_src, &length);
		break;

	case ICF_OPTIONS_RESP:
		ret_val = (icf_uint8_t)icf_unpack_icf_options_resp_st(p_dest, p_src, &length);
		break;

/* ICF_6_1_OPTIONS_SUPPORT start */
	case ICF_INC_OPTIONS_IND:
		ret_val = (icf_uint8_t)icf_unpack_icf_inc_option_ind_st(p_dest, p_src, &length);
		break;

	case ICF_OPTIONS_CFM:
		ret_val = (icf_uint8_t)icf_unpack_icf_icf_options_cfm_st(p_dest, p_src, &length);
		break;
#ifdef ICF_TRACE_ENABLE
    case ICF_SET_TRACE_LEVEL_REQ:
    	ret_val = (icf_uint8_t)icf_unpack_icf_trace_level_req_st(p_dest, p_src,
        						  	&length);
		break;
    case ICF_REPORT_TRACE_IND:
            ret_val = (icf_uint8_t)icf_unpack_icf_report_trace_ind_st(p_dest, p_src, &length);
            break;
   
    case ICF_SET_TRACE_REQ:
            ret_val = (icf_uint8_t)icf_unpack_icf_set_trace_req_st(p_dest, p_src, &length);
            break;
#endif

#ifdef ICF_ERROR_ENABLE
    case ICF_SET_ERROR_REPORT_LEVEL_REQ:
	ret_val = (icf_uint8_t)icf_unpack_icf_error_report_level_req_st(p_dest, p_src,
        						  	&length);
      	break;

    case ICF_REPORT_ERROR_IND:
       ret_val = (icf_uint8_t)icf_unpack_icf_report_error_ind_st(
                  p_dest, p_src, &length);
       break;
#endif

	case ICF_SET_LINE_PARAM_REQ:											
		ret_val = (icf_uint8_t) icf_unpack_icf_set_line_param_req_st(p_dest, p_src, &length);
		break;
        #ifdef ICF_STAT_ENABLE

    case ICF_STATS_REQ:
                ret_val = (icf_uint8_t)icf_unpack_icf_stats_req_st(p_dest, p_src, &length);
                break;

    case ICF_STATS_RESP:
                ret_val = (icf_uint8_t)icf_unpack_icf_stats_resp_st(p_dest, p_src, &length);
                break;
#endif
    case ICF_GET_REGISTER_STATUS_REQ:
                ret_val = (icf_uint8_t)icf_unpack_icf_get_register_status_req_st(p_dest, p_src, &length);
                break;

    case ICF_GET_REGISTER_STATUS_RESP:
                ret_val = (icf_uint8_t)icf_unpack_icf_get_register_status_resp_st(p_dest, p_src, &length);
                break;
        case ICF_REINITIALISE_REQ:
		ret_val = ICF_SUCCESS;
		break; 
         case ICF_REINIT_IND:
                ret_val = ICF_SUCCESS;
              break;
        case ICF_REINIT_RESP:
                ret_val = ICF_SUCCESS;
              break;
        case ICF_RESTART_IND:
                ret_val = ICF_SUCCESS;
              break;
        case ICF_RESTART_RESP:
                ret_val = ICF_SUCCESS;
              break;
/* ICF_6_1_OPTIONS_SUPPORT end */

    default:	
        *p_default = ICF_TRUE;
        break;
	}

    *p_length = length;
    return ret_val;
}

