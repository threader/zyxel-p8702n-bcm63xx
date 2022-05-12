/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/


#ifndef __MM_MACROS_H__
#define __MM_MACROS_H__

/***************************************************************************************
*All the include files will go here
**************************************************************************************/
#include "mm_port_wrapper.h"

/************************************************************************************
*All the Macro definitions will go here 
************************************************************************************/
#define MM_SUCCESS ICF_SUCCESS
#define MM_FAILURE ICF_FAILURE
#define MM_ERROR_NONE   0
#define MM_INVALID_SOCKET   -1

#define MM_TRUE MM_SUCCESS
#define MM_FALSE MM_FAILURE


/* --------------------------------------------------------------------
 *  This macro will take the pointer to the memory and will allocate the 
 *  memory = API header + payload length. the macro will return the pointer
 *  i.e. p_msg to the icf_msg_st srtucture.
 *----------------------------------------------------------------------*/
#define  MM_MSG_ALLOC(p_msg, size, ret_val, p_error )	\
	do \
	{ \
		p_msg = zyMalloc((size + sizeof(icf_api_header_st)));      \
        	zyMemset(p_msg,0,(size + sizeof(icf_api_header_st)));\
		if(p_msg==MM_NULL)			\
		{														\
			ret_val = MM_FAILURE;										\
		}																	\
		else																\
		{																	\
			((icf_api_header_st*)(p_msg))->api_length = size + sizeof(icf_api_header_st);			\
			(p_msg) = (icf_msg_st*)(p_msg);	\
			ret_val = MM_SUCCESS;												\
		}																\
	}while(0)

			



/* --------------------------------------------------------------------
 * This macro will take the pointer to the icf_msg_st structure and 
 * will fill all neccessy parameters of the API header
 *--------------------------------------------------------------------*/

#define  MM_MSG_FILL_HDR(in_p_msg,in_api_id,in_source_id,in_destination_id,in_call_id,in_app_id )\
	{\
		((icf_api_header_st*)in_p_msg)->api_id = in_api_id;\
		((icf_api_header_st*)in_p_msg)->version = ICF_VERSION_1_0;\
		((icf_api_header_st*)in_p_msg)->source_id = in_source_id;\
		((icf_api_header_st*)in_p_msg)->destination_id = in_destination_id;\
		((icf_api_header_st*)in_p_msg)->call_id = in_call_id;\
		((icf_api_header_st*)in_p_msg)->app_id = in_app_id;\
	} 


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 * call the function mm_ipc_udp_send to send the udp packet
 *--------------------------------------------------------------------*/

#define MM_MSG_SEND(p_msg, buf_len,ret_val, p_error )\
	{\
	    ret_val = mmIptkIpcSend(p_msg,buf_len, p_error);\
	}
/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 * call the function icf_ipc_udp_send to send the udp packet
 *--------------------------------------------------------------------*/

#define MM_MSG_GET_APP_ID(p_msg )\
	    ((icf_api_header_st*)p_msg)->app_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure
 * and will extract the value of the deatination_id.
 *--------------------------------------------------------------------*/

#define MM_MSG_GET_DESTINATION(p_msg)\
	   ((icf_api_header_st*)p_msg)->destination_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the source id.
 *--------------------------------------------------------------------*/
#define MM_MSG_GET_SRC(p_msg)\
			((icf_api_header_st*)p_msg)->source_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the Call_Id
 *--------------------------------------------------------------------*/

#define MM_MSG_GET_CALL_ID(p_msg)\
			((icf_api_header_st*)p_msg)->call_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the length of the payload
 *--------------------------------------------------------------------*/

#define MM_MSG_GET_PAYLOAD_LENGTH(p_msg)\
		((icf_api_header_st*)p_msg)->api_length
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the pointer to the payload
 *--------------------------------------------------------------------*/
#define MM_MSG_GET_PAYLOAD(p_msg )\
		((icf_uint8_t*)(p_msg) + sizeof(icf_api_header_st))

/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the pointer to the API-ID
 *-------------------------------------------------------------------*/

#define MM_MSG_GET_API(p_msg)\
		((icf_api_header_st*)p_msg)->api_id
		
	
#define MM_MSG_GET_MSG_FROM_PAYLOAD(p_payload)\
		((icf_uint8_t*)(p_payload) - sizeof(icf_api_header_st))

#define MM_MAKE_32BIT_INT(bt3, bt2, bt1, bt0) (((((int)bt3&0xFF)<<24) \
        |((int)bt2&0xFF)<<16) |(((int)bt1&0xFF)<<8) | (bt0 & 0xFF))

#endif

