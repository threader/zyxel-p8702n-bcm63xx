#ifndef __VOICEAPP_IPTK_MACROS_H__
#define __VOICEAPP_IPTK_MACROS_H__

/************************************************************************************
*All the Macro definitions will go here 
************************************************************************************/

#define VOICEAPP_CALL_TYPE_AUDIO_CALL  0x0001
#define VOICEAPP_CALL_TYPE_FAX_CALL  0x0002
#define IPTKAPP_MODULE_VOICEAPP             107

/* --------------------------------------------------------------------
 *  This macro will take the pointer to the memory and will allocate the 
 *  memory = API header + payload length. the macro will return the pointer
 *  i.e. p_msg to the icf_msg_st srtucture.
 *----------------------------------------------------------------------*/
#define  VOICEAPP_IPTK_MSG_ALLOC(p_msg, size, ret_val)	\
	do \
	{ \
		p_msg = zyMalloc((size + sizeof(icf_api_header_st)));      \
		zyMemset(p_msg,0,(size + sizeof(icf_api_header_st)));\
		if(p_msg==NULL)			\
		{														\
			ret_val = ICF_FAILURE;										\
		}																	\
		else																\
		{																	\
			((icf_api_header_st*)(p_msg))->api_length = size + sizeof(icf_api_header_st);			\
			(p_msg) = (icf_msg_st*)(p_msg);	\
			ret_val = ICF_SUCCESS;												\
		}																\
	}while(0)


/* --------------------------------------------------------------------
 * This macro will take the pointer to the icf_msg_st structure and 
 * will fill all neccessy parameters of the API header
 *--------------------------------------------------------------------*/

#define  VOICEAPP_IPTK_MSG_FILL_HDR(in_p_msg,in_api_id,in_source_id,in_destination_id,in_call_id,in_app_id )\
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

#define VOICEAPP_IPTK_MSG_SEND(p_msg, buf_len,ret_val)\
	{\
	    ret_val = voiceAppIptkIpcSend(p_msg,buf_len);\
	}

/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 * call the function icf_ipc_udp_send to send the udp packet
 *--------------------------------------------------------------------*/
#define VOICEAPP_IPTK_MSG_GET_APP_ID(p_msg ) ((icf_api_header_st*)p_msg)->app_id

/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure
 * and will extract the value of the deatination_id.
 *--------------------------------------------------------------------*/

#define VOICEAPP_IPTK_MSG_GET_DESTINATION(p_msg) ((icf_api_header_st*)p_msg)->destination_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the source id.
 *--------------------------------------------------------------------*/
#define VOICEAPP_IPTK_MSG_GET_SRC(p_msg) ((icf_api_header_st*)p_msg)->source_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the Call_Id
 *--------------------------------------------------------------------*/

#define VOICEAPP_IPTK_MSG_GET_CALL_ID(p_msg) ((icf_api_header_st*)p_msg)->call_id
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the length of the payload
 *--------------------------------------------------------------------*/

#define VOICEAPP_IPTK_MSG_GET_PAYLOAD_LENGTH(p_msg) ((icf_api_header_st*)p_msg)->api_length
	


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the pointer to the payload
 *--------------------------------------------------------------------*/
#define VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg ) ((icf_uint8_t*)(p_msg) + sizeof(icf_api_header_st))


/* --------------------------------------------------------------------
 * This macro will take the pointer to icf_msg_st structure and will 
 *  get the pointer to the API-ID
 *-------------------------------------------------------------------*/
#define VOICEAPP_IPTK_MSG_GET_API(p_msg) ((icf_api_header_st*)p_msg)->api_id


#define VOICEAPP_IPTK_MSG_GET_MSG_FROM_PAYLOAD(p_payload) ((icf_uint8_t*)(p_payload) - sizeof(icf_api_header_st))

#endif /* __VOICEAPP_IPTK_MACROS_H__ */

