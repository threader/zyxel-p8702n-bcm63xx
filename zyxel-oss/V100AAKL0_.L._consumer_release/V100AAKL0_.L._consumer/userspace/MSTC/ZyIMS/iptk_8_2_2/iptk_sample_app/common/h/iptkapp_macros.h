/***********************************************************************************
 *
 * File name        : iptkapp_macros.h
 *
 * Purpose          : The files provide the macros to perform various operations
 *					  in iptkapp.	
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 07-Dec-2007  Amit Sikka   					Initial version
 *
 * Copyright (c) 2007 Aricent Inc . All Rights Reserved
 ****************************************************************************************/

#ifndef __IPTKAPP_MACROS_H__
#define __IPTKAPP_MACROS_H__

/***************************************************************************************
*All the include files will go here
**************************************************************************************/
#include "iptkapp_port_wrapper.h"

/*******************************************************/
/*define values for PUBLISH state*/
#define INITIAL 0
#define REFRESH 1
#define MODIFY 2
#define REMOVE 3
/********************************************************/





/************************************************************************************
*All the Macro definitions will go here 
************************************************************************************/

/* These are the default values of call-id and line id to be 
 * filled in APIs towards IPTK*/
#define IPTKAPP_DEFAULT_CALL_ID         265
#define IPTKAPP_DEFAULT_LINE_ID         0

#define IPTKAPP_SUCCESS 1
#define IPTKAPP_FAILURE 0

#define IPTKAPP_TRUE IPTKAPP_SUCCESS
#define IPTKAPP_FALSE IPTKAPP_FAILURE

#define IPTKAPP_MAX_IP_ADDRESS_LEN 50
#define IPTKAPP_MAX_REQUEST_URL_LEN 100
#define IPTKAPP_CONF_FILE_NAME_SIZE 64

#define IPTKAPP_CALL_TYPE_AUDIO_CALL  0x0001
#define IPTKAPP_MODULE_PA             107

#define IPTKAPP_DEFAULT_REGISTRATION_TIMER 0xffff

#define MAX_TOKENS_CONF_FILE               13
#define MAX_MESSAGE_LENGTH              4000



/* --------------------------------------------------------------------
 *  This macro will take the pointer to the memory and will allocate the 
 *  memory = API header + payload length. the macro will return the pointer
 *  i.e. p_msg to the icf_msg_st srtucture.
 *----------------------------------------------------------------------*/
#define  IPTKAPP_MSG_ALLOC(p_msg, size, ret_val, p_error )	\
	do \
	{ \
		p_msg = iptkapp_port_malloc((size + sizeof(icf_api_header_st)));      \
        iptkapp_port_memset(p_msg,0,(size + sizeof(icf_api_header_st)));\
		if(p_msg==IPTKAPP_NULL)			\
		{														\
			ret_val = IPTKAPP_FAILURE;										\
		}																	\
		else																\
		{																	\
			((icf_api_header_st*)(p_msg))->api_length = size + sizeof(icf_api_header_st);			\
			(p_msg) = (icf_msg_st*)(p_msg);	\
			ret_val = IPTKAPP_SUCCESS;												\
		}																\
	}while(0)

			



/* --------------------------------------------------------------------
 * This macro will take the pointer to the icf_msg_st structure and 
 * will fill all neccessy parameters of the API header
 *--------------------------------------------------------------------*/

#define  IPTKAPP_MSG_FILL_HDR(in_p_msg,in_api_id,in_source_id,in_destination_id,in_call_id,in_app_id )\
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
 * call the function iptkapp_ipc_udp_send to send the udp packet
 *--------------------------------------------------------------------*/

#define IPTKAPP_MSG_SEND(p_msg, buf_len,ret_val, p_error )\
	{\
	    ret_val = iptkapp_iptk_ipc_send(p_msg,buf_len, p_error);\
	}
/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 * call the function icf_ipc_udp_send to send the udp packet
 *--------------------------------------------------------------------*/

#define IPTKAPP_MSG_GET_APP_ID(p_msg )\
	    ((icf_api_header_st*)p_msg)->app_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure
 * and will extract the value of the deatination_id.
 *--------------------------------------------------------------------*/

#define IPTKAPP_MSG_GET_DESTINATION(p_msg)\
	   ((icf_api_header_st*)p_msg)->destination_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the source id.
 *--------------------------------------------------------------------*/
#define IPTKAPP_MSG_GET_SRC(p_msg)\
			((icf_api_header_st*)p_msg)->source_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the Call_Id
 *--------------------------------------------------------------------*/

#define IPTKAPP_MSG_GET_CALL_ID(p_msg)\
			((icf_api_header_st*)p_msg)->call_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the length of the payload
 *--------------------------------------------------------------------*/

#define IPTKAPP_MSG_GET_PAYLOAD_LENGTH(p_msg)\
		((icf_api_header_st*)p_msg)->api_length
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the pointer to the payload
 *--------------------------------------------------------------------*/
#define IPTKAPP_MSG_GET_PAYLOAD(p_msg )\
		((icf_uint8_t*)(p_msg) + sizeof(icf_api_header_st))

/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the pointer to the API-ID
 *-------------------------------------------------------------------*/

#define IPTKAPP_MSG_GET_API(p_msg)\
		((icf_api_header_st*)p_msg)->api_id
		
	
#define IPTKAPP_MSG_GET_MSG_FROM_PAYLOAD(p_payload)\
		((icf_uint8_t*)(p_payload) - sizeof(icf_api_header_st))

#endif

