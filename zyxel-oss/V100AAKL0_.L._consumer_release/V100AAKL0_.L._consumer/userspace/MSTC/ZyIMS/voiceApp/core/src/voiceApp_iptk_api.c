#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include "icf_types.h"
#include "icf_feature_flags.h"
#include "icf_defs.h"
#include "icf_trace_id.h"
#include "icf_trace_error_types.h"
#include "icf_api.h"
#include "icf_clib.h"
#include "global.h"
#include "switch.h"
#include "voiceApp_main.h"
#include "voiceApp_iptk_api.h"
#include "voiceApp_macro.h"
#include "Phcmd.h"
#include "tr104.h"
#include "tr104_init.h"
#include "config_sys.h"


#include "switch.h" 
#if HEARTBEAT_MECHANISM_ENABLE			
#include "voiceApp_iptk_MsgHandler.h"
#endif
#if 1 /*Jason ,[SPRID # 090622474]*/
#include "call_mgnt.h"
#endif
int *reg_check_cnt = NULL;
extern tm_id_t reg_status_timer;
extern webSipStatus_t *webSipStatus;
struct hostent* proxyAddrHostent = NULL; /*Note: For voice single-profile only, if for multi-profile, proxyAddrHostent should be array to store proxy server ip for each profile*/
struct hostent* registrarAddrHostent = NULL; /*Note: For voice single-profile only, if for multi-profile, proxyAddrHostent should be array to store proxy server ip for each profile*/

extern int voiceCriticalChangedFlag;

extern uint32 sipAccountNumber;	
extern ZyXEL_VoiceSipAccount_t *ZyXEL_VoiceSipAccount;

#if 1	/*Jason ,[SPRID # 090622474]*/		
oneShotEventRec_t* oneShotEvent = NULL;
#endif

//Ssu-Ying, support Multiple Profile
#if 1
extern int APICFMInitflag;	
extern int IPTKCfgInitTimer;
#endif

pthread_mutex_t mutexVoiceAppIPTK = PTHREAD_MUTEX_INITIALIZER; 
void voiceAppIPTKAcquireMutex(void);
void voiceAppIPTKReleaseMutex(void);
/******************************************************************************
 * voiceAppIPTKAcquireMutex
 *
 * Description: 
 *	Dsp pool function, used to acquire mutex
 *	
 * Parameters:
 * 	Input
 *
 * Return Value: mm_return_t
 *	MM_SUCCESS if success
 *	MM_FAILURE if failure
 *
 *****************************************************************************/
void voiceAppIPTKAcquireMutex(void){	
	VOICEAPP_PRINTF("voiceAppIPTKAcquireMutex \n");
	pthread_mutex_lock(&mutexVoiceAppIPTK);
}

/******************************************************************************
 * voiceAppIPTKReleaseMutex
 *
 * Description: 
 *	Dsp pool function, used to release mutex
 *	
 * Parameters:
 * 	Input
 *
 * Return Value: mm_return_t
 *	MM_SUCCESS if success
 *	MM_FAILURE if failure
 *
 *****************************************************************************/
void voiceAppIPTKReleaseMutex(void){		
	pthread_mutex_unlock(&mutexVoiceAppIPTK);	
	VOICEAPP_PRINTF("voiceAppIPTKReleaseMutex \n");
}



/*************************************************************************
*
*  FUNCTION NAME  : voiceAppIptkIpcSend
*
*  DESCRIPTION    : This is the function that sends the IPC packet to IPTK ES
*
*  RETURNS:       :  Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_uint32_t  voiceAppIptkIpcSend(icf_void_t *p_msg,icf_uint32_t buf_len) {
	icf_uint32_t		payload_length,packet_length;
	icf_uint32_t		bytes_written = 0;
	icf_uint8_t		*p_buf;
	int				sendSock;
	struct sockaddr_un addr;


	if(p_msg ==NULL) {
		return ICF_FAILURE;
	}

	sendSock = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(sendSock == -1) {
		return ICF_FAILURE;
	}
	zyMemset(&addr, 0, sizeof(addr));
	addr.sun_family = PF_LOCAL;
	zyStrncpy(addr.sun_path, ICF_ES_CHANNEL_PATH, sizeof(ICF_ES_CHANNEL_PATH));
	
	packet_length = payload_length = buf_len;
		
	p_buf = ( icf_uint8_t *)p_msg;

	while( packet_length ) {
		if( packet_length > MAX_MTU_SIZE ) {
			packet_length = MAX_MTU_SIZE;
		}


		bytes_written = sendto(sendSock,(icf_void_t*)p_buf,packet_length,0,(struct sockaddr*)&addr,sizeof(struct sockaddr_un));


		if ( packet_length != bytes_written) {
			return (ICF_FAILURE);
		}
		
		p_buf +=packet_length;

		payload_length -= packet_length;

		packet_length = payload_length;	

	}

	close(sendSock);
	return (ICF_SUCCESS);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppFillHdrSendIptk
 *
 * DESCRIPTION   : The function to fill the header of the message and send to ICF
 *
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/
icf_return_t voiceAppFillHdrSendIptk(icf_msg_st *p_msg, icf_api_id_t api_id, icf_call_id_t call_id) {
	icf_uint32_t ret_val = 0;
	icf_uint8_t *p_buffer=NULL;
	icf_uint32_t buf_len=0;
#if defined (IPTK_REL_7_1) || defined (IPTK_REL_8_0) || defined (IPTK_REL_8_2_2)
	icf_vldt_err_st  vldt_err;
#endif
	icf_error_t ecode = 0;
	#define MAX_ICF_INIT_WAITING_COUNT 240
	int ICFMInitWaitingCount=0;

	VOICEAPP_DBG("\n######################################################");
	VOICEAPP_PRINTF("voiceApp -> IPTK : API %d\n", api_id);
#if defined (IPTK_REL_8_2_2)	
	VOICEAPP_PRINTF("voiceApp -> IPTK : API name %s\n\r",iptk_api_id2str(api_id));
#endif
	VOICEAPP_DBG("######################################################");

	/* Every message towards IPTK needs a header.
	* This function fills the values in the header */
	if(ICF_APP_ADD_REQ == api_id) {/* network activate need to be checked */
		VOICEAPP_IPTK_MSG_FILL_HDR(p_msg,api_id,IPTKAPP_MODULE_VOICEAPP, ICF_MODULE_ICF,ICF_INVALID_CALL_ID,ICF_GENERIC_APP_ID);
	} else {
		VOICEAPP_IPTK_MSG_FILL_HDR(p_msg,api_id,IPTKAPP_MODULE_VOICEAPP, ICF_MODULE_ICF,call_id,globalVoiceApp.appId);
	}

	/* Message to be send towards IPTK will be encoded and validated by following
	* function. This is performed by clib library of IPTK which is linked with
	* IPTK Application.
	*/
#if defined (IPTK_REL_7_1) || defined (IPTK_REL_8_0) || defined (IPTK_REL_8_2_2)
	ret_val = icf_encode_msg((icf_msg_st*)p_msg, &p_buffer, &buf_len,&ecode,&vldt_err);
#else
	ret_val = icf_encode_msg((icf_msg_st*)p_msg, &p_buffer, &buf_len,&ecode);
#endif

	if( ret_val !=  ICF_SUCCESS) {
		/* add the trace */
		ZyPrintf("voiceAppFillHdrSendIptk fail and return\r\n");
		return(FALSE);
	}

	VOICEAPP_PRINTF("%s : show APICFMInitflag = %d!!\n" ,__FUNCTION__,APICFMInitflag);			
	while(APICFMInitflag > 7){		
		ICFMInitWaitingCount++;
		usleep(500000);
		if(ICFMInitWaitingCount>MAX_ICF_INIT_WAITING_COUNT){
			voiceAppIPTKAcquireMutex();
			APICFMInitflag = 0;
			ZyPrintf("%s,%d : voiceapp-acif(reset)APICFMInitflag,It's end of waiting Time.!!\n" ,__FUNCTION__,api_id);					
			voiceAppIPTKReleaseMutex();			
		}
	}

	switch (api_id) {   
		case ICF_SET_TRANSPORT_REQ: 				
		case ICF_SET_REGISTRATION_REQ:
		case ICF_SET_SELF_ID_REQ:
		case ICF_SET_SERVICE_PARAMS_REQ:
		case ICF_SET_LINE_PARAM_REQ:
		case ICF_REGISTER_REQ:
			voiceAppIPTKAcquireMutex();
			APICFMInitflag = APICFMInitflag + 1;
			VOICEAPP_PRINTF("%s,%d : voiceapp-acif(inc)APICFMInitflag = %d!!\n" ,__FUNCTION__,api_id,APICFMInitflag);					
			voiceAppIPTKReleaseMutex();
			break;
		default:
			break;
	}

	/* Call the macro to send it to the IPTK */
	VOICEAPP_IPTK_MSG_SEND(p_buffer,buf_len,ret_val);

	/* Error in sending */
	if(ret_val==ICF_FAILURE) {
		icf_free_encoded_msg(p_buffer,&ecode);  /* ICF - free message encoded */
		ZyPrintf("%s : Error in sending!! \n" ,__FUNCTION__);			
		switch (api_id) {   
			case ICF_SET_TRANSPORT_REQ:				
			case ICF_SET_REGISTRATION_REQ:
			case ICF_SET_SELF_ID_REQ:
			case ICF_SET_SERVICE_PARAMS_REQ:
			case ICF_SET_LINE_PARAM_REQ:
			case ICF_REGISTER_REQ:
				voiceAppIPTKAcquireMutex();
				APICFMInitflag = APICFMInitflag - 1;				
				voiceAppIPTKReleaseMutex();			
				ZyPrintf("%s :Restore APICFMInitflag =%d\n" ,__FUNCTION__,APICFMInitflag);				
				break;
			default:
				break;
		}
		return(FALSE);
	}

	/* memory taken by this buffer needs to be freed after sending to IPTK */
	icf_free_encoded_msg(p_buffer,&ecode);   /* ICF - free encoded message*/
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : freeIcfHeaderNameSt
 *
 * DESCRIPTION   : The function is to free icf_header_name_st
 *
 * RETURNS       : void
 *
 ******************************************************************************/
void freeIcfHeaderNameSt(icf_header_name_st *ptr) {
	if( NULL != ptr) {
		if(NULL != ptr->next) {
			freeIcfHeaderNameSt(ptr->next);
			ptr->next = NULL;
		}
		zyFree(ptr);
	}
}

/*******************************************************************************
 *
 * FUNCTION NAME : freeIcfStringListNodeSt
 *
 * DESCRIPTION   : The function is to free icf_string_list_node_st
 *
 * RETURNS       : void
 *
 ******************************************************************************/
void freeIcfStringListNodeSt(icf_string_list_node_st *ptr) {
	if( NULL != ptr) {
		if(NULL != ptr->p_next) {
			freeIcfStringListNodeSt(ptr->p_next);
			ptr->p_next = NULL;
		}
		zyFree(ptr);
		ptr = NULL;
	}
}

/*******************************************************************************
 *
 * FUNCTION NAME : freeIcfListSt
 *
 * DESCRIPTION   : The function is to free icf_list_st
 *
 * RETURNS       : void
 *
 ******************************************************************************/
void freeIcfListSt(icf_list_st *ptr) {
	if( NULL != ptr) {
		if(NULL != ptr->p_next) {
			freeIcfListSt(ptr->p_next);
			ptr->p_next = NULL;
		}
		zyFree(ptr->p_data);
		ptr->p_data = NULL;
		zyFree(ptr);
	}
}

/*******************************************************************************
 *
 * FUNCTION NAME : addHeaderNameList
 *
 * DESCRIPTION   : The function is to add message in p_payload->header_name_list.hdr
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int addHeaderNameList(icf_app_add_req_st *p_payload, char *message){
	icf_header_name_st *now = p_payload->header_name_list.hdr;
	if(now == NULL){
		p_payload->header_name_list.hdr = zyMalloc(sizeof(icf_header_name_st));
		if(NULL == p_payload->header_name_list.hdr) {
			return -1;
		}
		p_payload->header_name_list.hdr->hdr_name.str_len = zyStrlen((char *)message);
		zyStrncpy((char *)p_payload->header_name_list.hdr->hdr_name.str,(char *)message,zyStrlen((char *)message)+1);
		p_payload->header_name_list.hdr->next = NULL;
		p_payload->header_name_list.count = 1;
	}else{
		while(now->next != NULL){
			now = now ->next;
		}
		now->next= zyMalloc(sizeof(icf_header_name_st));
		if(NULL == now->next) {
			return -1;
		}
		now = now ->next;
		now->hdr_name.str_len = zyStrlen((char *)message);
		zyStrncpy((char *)now->hdr_name.str,(char *)message,zyStrlen((char *)message)+1);
		now->next = NULL;
		p_payload->header_name_list.count+=1;

		return 0;
	}

	return 0;
}

/*******************************************************************************
 *
 * FUNCTION NAME : addHeaderList
 *
 * DESCRIPTION   : The function is to add message in p_payload->header_list.hdr
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int addHeaderList(icf_create_call_req_st *p_payload, char *message1, char *message2){
	icf_header_st *now = p_payload->header_list.hdr;
	if(now == NULL){
		p_payload->header_list.hdr = zyMalloc(sizeof(icf_header_st));
		if(NULL == p_payload->header_list.hdr) {
			return -1;
		}
		p_payload->header_list.hdr->hdr_name.str_len = zyStrlen((char *)message1);
		zyStrncpy((char *)p_payload->header_list.hdr->hdr_name.str,(char *)message1,zyStrlen((char *)message1)+1);
		p_payload->header_list.hdr->hdr_value.str_len = zyStrlen((char *)message2);
		zyStrncpy((char *)p_payload->header_list.hdr->hdr_value.str,(char *)message2,zyStrlen((char *)message2)+1);		
		p_payload->header_list.hdr->next = NULL;
		p_payload->header_list.count = 1;
	}else{
		while(now->next != NULL){
			now = now ->next;
		}
		now->next= zyMalloc(sizeof(icf_header_st));
		if(NULL == now->next) {
			return -1;
		}
		now = now ->next;
		now->hdr_name.str_len = zyStrlen((char *)message1);
		zyStrncpy((char *)now->hdr_name.str,(char *)message1,zyStrlen((char *)message1)+1);
		now->hdr_value.str_len = zyStrlen((char *)message2);
		zyStrncpy((char *)now->hdr_value.str,(char *)message2,zyStrlen((char *)message2)+1);
		now->next = NULL;
		p_payload->header_list.count+=1;
	}

	return 0;
}

/*******************************************************************************
 *
 * FUNCTION NAME : addSupportedContentTypes
 *
 * DESCRIPTION   : The function is to add message in p_payload->supported_content_types.p_list
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int addSupportedContentTypes(icf_app_add_req_st *p_payload, char *message){
	icf_string_list_node_st *now = p_payload->supported_content_types.p_list;
	if(now == NULL){
		p_payload->supported_content_types.p_list = zyMalloc(sizeof(icf_string_list_node_st));
		if(NULL == p_payload->supported_content_types.p_list) {
			return -1;
		}
		p_payload->supported_content_types.p_list->str.str_len= zyStrlen((char *)message);
		zyStrncpy((char *)p_payload->supported_content_types.p_list->str.str,(char *)message,zyStrlen((char *)message)+1);
		p_payload->supported_content_types.p_list->p_next = NULL;
		p_payload->supported_content_types.count = 1;
	}else{
		while(now->p_next != NULL){
			now = now ->p_next;
		}
		now->p_next= zyMalloc(sizeof(icf_string_list_node_st));
		if(NULL == now->p_next) {
			return -1;
		}
		now = now ->p_next;
		now->str.str_len = zyStrlen((char *)message);
		zyStrncpy((char *)now->str.str,(char *)message,zyStrlen((char *)message)+1);
		now->p_next = NULL;
		p_payload->supported_content_types.count+=1;
	}

	return 0;
}

/*******************************************************************************
 *
 * FUNCTION NAME : addSupportedMethods
 *
 * DESCRIPTION   : The function is to add message in p_payload->supported_methods.p_list
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int addSupportedMethods(icf_app_add_req_st *p_payload, char *message){
	icf_string_list_node_st *now = p_payload->supported_methods.p_list;
	if(now == NULL){
		p_payload->supported_methods.p_list = zyMalloc(sizeof(icf_string_list_node_st));
		if(NULL == p_payload->supported_methods.p_list) {
			return -1;
		}
		p_payload->supported_methods.p_list->str.str_len= zyStrlen((char *)message);
		zyStrncpy((char *)p_payload->supported_methods.p_list->str.str,(char *)message,zyStrlen((char *)message)+1);
		p_payload->supported_methods.p_list->p_next = NULL;
		p_payload->supported_methods.count = 1;
	}else{
		while(now->p_next != NULL){
			now = now ->p_next;
		}
		now->p_next= zyMalloc(sizeof(icf_string_list_node_st));
		if(NULL == now->p_next) {
			return -1;
		}
		now = now ->p_next;
		now->str.str_len = zyStrlen((char *)message);
		zyStrncpy((char *)now->str.str,(char *)message,zyStrlen((char *)message)+1);
		now->p_next = NULL;
		p_payload->supported_methods.count+=1;
	}

	return 0;
}

#if 1 /*Jason , Content-Enconding*/
/*******************************************************************************
 *
 * FUNCTION NAME : addSupportedEncoding
 *
 * DESCRIPTION   : The function is to add message in p_payload->supported_encodings.p_list
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int addSupportedEncoding(icf_app_add_req_st *p_payload, char *message){
	icf_string_list_node_st *now = p_payload->supported_encodings.p_list;
	if(now == NULL){
		p_payload->supported_encodings.p_list = zyMalloc(sizeof(icf_string_list_node_st));
		if(NULL == p_payload->supported_encodings.p_list) {
			return -1;
		}
		p_payload->supported_encodings.p_list->str.str_len= zyStrlen((char *)message);
		zyStrncpy((char *)p_payload->supported_encodings.p_list->str.str,(char *)message,zyStrlen((char *)message)+1);
		p_payload->supported_encodings.p_list->p_next = NULL;
		p_payload->supported_encodings.count = 1;
	}else{
		while(now->p_next != NULL){
			now = now ->p_next;
		}
		now->p_next= zyMalloc(sizeof(icf_string_list_node_st));
		if(NULL == now->p_next) {
			return -1;
		}
		now = now ->p_next;
		now->str.str_len = zyStrlen((char *)message);
		zyStrncpy((char *)now->str.str,(char *)message,zyStrlen((char *)message)+1);
		now->p_next = NULL;
		p_payload->supported_encodings.count+=1;
	}

	return 0;
}
#endif
/*******************************************************************************
 *
 * FUNCTION NAME : addSupportedEvents
 *
 * DESCRIPTION   : The function is to add message in p_payload->supported_events.p_list
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int addSupportedEvents(icf_app_add_req_st *p_payload, char *message){
	icf_string_list_node_st *now = p_payload->supported_events.p_list;
	if(now == NULL){		
		p_payload->supported_events.p_list = zyMalloc(sizeof(icf_string_list_node_st));
		if(NULL == p_payload->supported_events.p_list) {
			return -1;
		}
		p_payload->supported_events.p_list->str.str_len= zyStrlen((char *)message);
		zyStrncpy((char *)p_payload->supported_events.p_list->str.str,(char *)message,zyStrlen((char *)message)+1);
		p_payload->supported_events.p_list->p_next = NULL;
		p_payload->supported_events.count = 1;
	}else{
		while(now->p_next != NULL){
			now = now ->p_next;
		}
		now->p_next= zyMalloc(sizeof(icf_string_list_node_st));
		if(NULL == now->p_next) {
			return -1;
		}
		now = now ->p_next;
		now->str.str_len = zyStrlen((char *)message);
		zyStrncpy((char *)now->str.str,(char *)message,zyStrlen((char *)message)+1);
		now->p_next = NULL;
		p_payload->supported_events.count+=1;
	}

	return 0;
}

/*******************************************************************************
 *
 * FUNCTION NAME : addAudioStreamData
 *
 * DESCRIPTION   : The function is to add bit_mask, stream_id, & stream_type in head->p_data
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int addAudioStreamData(icf_list_st *head, icf_uint8_t bit_mask, icf_stream_id_t id, icf_media_type_t type){
	icf_list_st *tmp;

	if(NULL == head) {
		return -1;
	}

	tmp = head;
	while(tmp->p_next != NULL) {
		tmp = tmp->p_next;
	}
	tmp->p_next = zyMalloc(sizeof(*tmp));
	if(NULL == tmp->p_next) {
		return -1;
	}
	tmp = tmp->p_next;
	tmp->p_data = zyMalloc(sizeof(icf_stream_capab_st));
	if(NULL == tmp->p_data) {
		return -1;
	}	
	((icf_stream_capab_st*)tmp->p_data)->bit_mask = bit_mask; 
	((icf_stream_capab_st*)tmp->p_data)->stream_id = id; /*Can be generated dynamically is required in future*/
	((icf_stream_capab_st*)tmp->p_data)->stream_type = type;
	tmp->p_next = NULL;

	return 0;
}

/*******************************************************************************
 *
 * FUNCTION NAME : iptkCfgInit
 *
 * DESCRIPTION   : The function is to set configuration towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool iptkCfgInit(int timer_id, /* timerid */
	                             void	*vp) {
                            
	VoiceProfLineObj_t* pLine = NULL;

	int VpItr=0 , LineItr = 0;
	int SipAccountId = 0;
	VOICEAPP_PRINTF("%s > ENTER \n",__FUNCTION__);

	if (FALSE == voiceAppSetCallParam(0)) {
		ZyPrintf("voiceApp > SET_CALL_PARAM_REQ towards IP Phone Toolkit Failed.\n");
		IPTKCfgInitTimer = -1;
		return FALSE;
	}

	for ( SipAccountId=0; SipAccountId<sipAccountNumber; SipAccountId++){	

		VpItr = mapSipAccountIdToVpItr(SipAccountId);
		LineItr = mapSipAccountIdToLineItr(SipAccountId);
		pLine = tr104VoiceLineObjPtrGet(VpItr , LineItr);	

		if(pLine == NULL){
			ZyPrintf("%s > pLine is NULL!\n");	
		}else if(ZyXEL_VoiceSipAccount==NULL){
			ZyPrintf("%s > ZyXEL_VoiceSipAccount is NULL!\n");	
		}

		VOICEAPP_PRINTF("iptkCfgInit > SipAccountId = %d,enable=%d,directoryNumber = %s\n", SipAccountId,ZyXEL_VoiceSipAccount[SipAccountId].enable,pLine->directoryNumber);

		if(ZyXEL_VoiceSipAccount[SipAccountId].enable){

			/* Through this function SET_TRANSPORT_PARAMS API is invoked towards 
			* IP Phone Toolkit.This API is used to configure transport-related
			* parameters such as, proxy address per line, self-IP address, and 
			* the port on which the network (SIP) messages are received.
			*/
			if (FALSE == voiceAppSetTransportParams(SipAccountId)) {
				ZyPrintf("voiceApp > SET_TRANSPORT_PARAMS_REQ towards IP Phone Toolkit Failed.\n");
				IPTKCfgInitTimer = -2;
				return FALSE;
			}

			/* Through this function SET_REGISTRATION_PARAMS_REQ is invoked towards 
			* IP Phone Toolkit.This API is used to configure registration-related
			* parameters, such as, the registrar address, and registration duration.
			* After initial configuration, any of these parameters can be modified 
			* using the same API.
			*/

			if (FALSE == voiceAppSetSelfId(SipAccountId)) {
				ZyPrintf("voiceApp > SET_SELF_ID_REQ towards IP Phone Toolkit Failed.\n");
				IPTKCfgInitTimer = -3;
				return FALSE;
			}


			if (FALSE == voiceAppSetRegistrationParams(SipAccountId)) {
				ZyPrintf("voiceApp > SET_REGISTRATION_PARAMS_REQ towards IP Phone Toolkit Failed.\n");
				IPTKCfgInitTimer = -4;
				return FALSE;
			}

			/* Through this function SET_SELF_ID_REQ is invoked towards 
			* IP Phone Toolkit.This API is used to configure user 
			* identity-related parameters such as, the user name and 
			* address. After initial application configuration, 
			* any of these parameters can be modified using the same API.
			*/

			if (FALSE == voiceAppSetServiceParam(SipAccountId)) {
				ZyPrintf("voiceApp > SET_SERVICE_PARAM_REQ towards IP Phone Toolkit Failed.\n");
				IPTKCfgInitTimer = -5;
				return FALSE;
			}

			if (FALSE == voiceAppSetLineParam(SipAccountId)) {
				ZyPrintf("voiceApp > SET_LINE_PARAM_REQ towards IP Phone Toolkit Failed.\n");
				IPTKCfgInitTimer = -6;
				return FALSE;
			}
			ICFLineInitDone[SipAccountId] = 1;
			VOICEAPP_PRINTF(" %s >Set ICFLineInitDone[%d] to %u\n",__FUNCTION__,SipAccountId ,ICFLineInitDone[SipAccountId]);				
			if(pLine->status != TR104I_STATUS_UNREGISTERED){
				if (FALSE == voiceAppSendRegister(SipAccountId)) {
					ZyPrintf("voiceApp > REGISTER_REQ towards IP Phone Toolkit Failed.\n");
					IPTKCfgInitTimer = -7;
					return FALSE;
				}
			}
		}
	}
	
	IPTKCfgInitTimer = 0;
	VOICEAPP_PRINTF("%s > EXIT \n", __FUNCTION__);	
    return TRUE;
}

/*******************************************************************************
 *
 * FUNCTION NAME : iptkCfgReset
 *
 * DESCRIPTION   : The function is to reset configuration towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool iptkCfgReset(void) {
	VOICEAPP_PRINTF("%s > ENTER\n",__FUNCTION__);
	VoiceProfLineObj_t* pLine = NULL;

	uint16 VpItr=0 , LineItr = 0;
	uint16 SipAccountId = 0;

	VOICEAPP_PRINTF(" %s >Reset ICFLineInitDone to 0\n",__FUNCTION__);		
	zyMemset((void *)ICFLineInitDone, 0, sipAccountNumber*sizeof(uint8));
	
	for ( SipAccountId=0; SipAccountId<sipAccountNumber; SipAccountId++){	

		VpItr = mapSipAccountIdToVpItr(SipAccountId);
		LineItr = mapSipAccountIdToLineItr(SipAccountId);
		pLine = tr104VoiceLineObjPtrGet(VpItr , LineItr);	

		VOICEAPP_PRINTF("iptkCfgReset > ENTER, SipAccountId = %d\n", SipAccountId);
		VOICEAPP_PRINTF("iptkCfgReset > ENTER, VpItr = %d\n", VpItr);
		VOICEAPP_PRINTF("iptkCfgReset > ENTER, LineItr = %d\n", LineItr);
		VOICEAPP_PRINTF(" %s > ICFLineInitDone[%d] is %u.\n",__FUNCTION__,SipAccountId, ICFLineInitDone[SipAccountId]);			
		if(ZyXEL_VoiceSipAccount[SipAccountId].enable) {	
			
			if (FALSE == voiceAppDelTransportParams(SipAccountId)) {
				ZyPrintf("voiceApp > DEL_TRANSPORT_PARAMS_REQ towards IP Phone Toolkit Failed.\n");
				return FALSE;
			}

			if (FALSE == voiceAppDelSelfId(SipAccountId)) {
				ZyPrintf("voiceApp > DEL_SELF_ID_REQ towards IP Phone Toolkit Failed.\n");
				return FALSE;
			}

			if (FALSE == voiceAppDelRegistrationParams(SipAccountId)) {
				ZyPrintf("voiceApp > DEL_REGISTRATION_PARAMS_REQ towards IP Phone Toolkit Failed.\n");
				return FALSE;
			}

			if(pLine->status != TR104I_STATUS_UNREGISTERED){ /*keep unregistered state*/
				//pLine->status = TR104I_STATUS_QUIECCENT;
				VOICEAPP_PRINTF("iptkCfgReset:  Status isn't UNREGISTERED, change it to IDLE\n");				
				pLine->status = TR104I_STATUS_IDLE;
			}

			ZyPrintf("voiceApp >  UNREG_SUCC to CCM\n");
			sipEvtToCCM(SipAccountId, -1, EVT_CCM_UNREG_SUCC, NULL, 0);			
			
			
			
		}
	}

	voipLedOnOff( tr104GetNumOfPhyIntf() );
	
	VOICEAPP_PRINTF("%s > EXIT\n",__FUNCTION__);	
	return TRUE;
}

#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION NAME : iptkTraceInit
 *
 * DESCRIPTION   : The function is to send SET_TRACE_REQ towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool iptkTraceInit(void) {
	/*For reporting of traces to the application API ICF_SET_TRACE_REQ is 
	 *invokes by application towards iptk.This API specify the trace type,
	 *trace criteria and the trace rule and also apecifying the status as 
	 *enabled
	 */

	if (FALSE == voiceAppSetTrace(0)){
		ZyPrintf("voiceApp > SET_TRACE_REQ towards IP Phone Toolkit Failed.\n");
	        return FALSE;
	}
	return TRUE;
}
#endif

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppAddNewApp
 *
 * DESCRIPTION   : The function is to register application with IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppAddNewApp(void) {
	icf_error_t    *p_err = NULL;
	icf_msg_st *p_msg = NULL;
	icf_uint32_t ret_val;
	icf_app_add_req_st *p_payload = NULL;
	int size;
	icf_list_st *index_ptr = NULL;	
	icf_list_st audio_stream_data;
	char	extensions[256+1];
	char* pch;

	VoiceCapSipObj_t* pCapSip = tr104VoiceCapSipObjPtrGet();

	VOICEAPP_DBG("voiceApp -> IPTK : APP_ADD_REQ");

	/* Memory to be allocated for buffer i.e. to be sent towards IPTK */
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_app_add_req_st),ret_val);

	size = sizeof(icf_app_add_req_st);
	if(ret_val==ICF_FAILURE) {
		ZyPrintf("voiceApp:-   MEMORY ALLOC FAIL : APP_ADD_REQ\n");
		return(FALSE);
	}
	
	p_payload=(icf_app_add_req_st * )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

#ifdef IPTK_REL_7_1
	/* Required to be made FALSE if application don't have info regarding line concept */
	p_payload->icf_simul_call_line_arbitration = 0;
#endif

	/* Indicates the presence of optional parameters in the API*/
	p_payload->bit_mask = 0;

	/* Application name, used for tracing purposes */
	zyStrncpy((char *)p_payload->app_name.str, (char *)"ZyIMS voiceApp", zyStrlen((char *)"ZyIMS voiceApp")+1);
	p_payload->app_name.str_len = zyStrlen((char *)"ZyIMS voiceApp");

	/* Contains the IPC information to reach application. */
	p_payload->routing_info.len = sizeof(icf_route_info_st);

	/* allocate memory for icf_route_info_st*/
	p_payload->routing_info.ptr = zyMalloc(sizeof(icf_route_info_st));
	if(NULL == p_payload->routing_info.ptr) {
		icf_free_decoded_msg(p_msg,p_err);	
		p_msg=NULL;
		return(FALSE);
	}

	/* UPD will be used for communication bewteen IPTK App and IPTK */
	((icf_route_info_st*)(p_payload->routing_info.ptr))->type=ICF_IPC_UDP;

	/* addr_type could be domain name or IPV4 address */
	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.ip.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;

	/* IP address is being filled like octet[1] =172, octet[2] = 16 */
	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.ip.addr.ipv4_addr.octet_1 = 192;/*octet_info[0];*/
	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.ip.addr.ipv4_addr.octet_2 = 168;/*octet_info[1];*/
	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.ip.addr.ipv4_addr.octet_3 = 33;/*octet_info[2];*/
	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.ip.addr.ipv4_addr.octet_4 = 33;/*octet_info[3];*/

	/* IPTKAPP Port */
	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.port = 5888;

	/* IP Phone Toolkit uses filter to decide the application corresponding to 
	  * the message obtained from the network and where call_id field is not 
	  * maintained by IP Phone Toolkit.
	  * Currently no filter used & no header attached*/
	p_payload->filter.count = 0;
	p_payload->filter.p_filter_list = NULL;

	/* All the APIs from IP Phone Toolkit to the application includes mandatory
	* parameter and provision for additional headers as desired by application.
	* All the additional headers required by the application are specified in 
	* this field.This field is a List of additional headers that the application
	* wants to receive in the APIs from IP Phone Toolkit if present in the 
	* incoming SIP message.*/

	/*rewrite by Mike,
	*if you wanna add a header,add the function addHeaderNameList(p_payload,"")
	*it use link_list to generate the header
	*/
/** this header will cause T38 failed due to duplicate Content-Type in 200 OK
* refer detailed info from http://172.23.56.144/trac/VoIPTeam/ticket/612
* cindy modify 20100927 */
#if 0
	if( 0 > addHeaderNameList(p_payload, "Content-Type") ) {
		icf_free_decoded_msg(p_msg,p_err);	
		p_msg = NULL;
		return(FALSE);
	}
#endif
	/* Specifies the list of methods, supported by application, to be
	* sent in ?œAllow??header in outgoing requests. If the application
	* does not specify any method, then IP Phone Toolkit will 
	* return failure response to this API.
	*/
	zyStrcpy(extensions, pCapSip->extensions);

	/*rewrite by Mike,
	*if you wanna add a header,add the function addSupportedMethods(p_payload,"")
	*it use link_list to generate the SupportedMethods
	*/
	pch = zyStrtok(extensions, ",\0");
	while(pch != NULL){
		if( 0 > addSupportedMethods(p_payload, pch) ) {
			icf_free_decoded_msg(p_msg,p_err);	
			p_msg =NULL;
			return(FALSE);
		}
		pch = zyStrtok(NULL, ",\0");
	}


	/* Specifies the list of events application supports for 
	* incoming dialog requests. If application does not specify
	* any supported event, then the count should be 0.
	*/

	/*rewrite by Mike,
	*if you wanna add a header,add the function addSupportedEvents(p_payload,"")
	*it use link_list to generate the SupportedEvents
	*/
	if( 0 > addSupportedEvents(p_payload, "message-summary") ) {
		icf_free_decoded_msg(p_msg,p_err);	
		p_msg =NULL;		
		return(FALSE);
	}

	/* Specifies the list of message body content types, 
	* supported by application, to be sent in ?œAccept??header in outgoing requests.
	* If application does not specify any supported content type,
	* then the count should be 0.
	*/

	/*rewrite by Mike,
	*if you wanna add a Content Type,add the function addSupportedContentTypes(p_payload,"")
	*it use link_list to generate the Content Type
	*/
	if( 0 > addSupportedContentTypes(p_payload, "application/simple-message-summary") ) {
		icf_free_decoded_msg(p_msg,p_err);	
		return(FALSE);
	}
	if( 0 > addSupportedContentTypes(p_payload, "application/sdp") ) {
		icf_free_decoded_msg(p_msg,p_err);	
		return(FALSE);
	}
	if( 0 > addSupportedContentTypes(p_payload, "application/dtmf-relay") ) {
		icf_free_decoded_msg(p_msg,p_err);	
		return(FALSE);
	}
	if( 0 > addSupportedContentTypes(p_payload, "application/dtmf") ) {
		icf_free_decoded_msg(p_msg,p_err);			
		return(FALSE);
	}


	/*Specifies the list of message body encoding methods, supported by 
	* application, to be sent in Accept-Encoding header in the
	* outgoing requests. If application does not specify any 
	* supported encoding, then the count should be 0.*/
#if 1 /*Jason , Content-Encoding*/
	p_payload->supported_encodings.count=0;
	if( 0 > addSupportedEncoding(p_payload, "identity") ) {
		icf_free_decoded_msg(p_msg,p_err);			
		return(FALSE);
	}	
#else
	p_payload->supported_encodings.count=0;
#endif
	/* Specifies the list of message body language, supported 
	* by application, to be sent in ?œAccept-Language??header in 
	* outgoing requests. If application does not specify any 
	* supported languages, then the count should be 0.*/
	p_payload->supported_languages.count=0;

	p_payload->rtp_detection_reqd = 0;

	/* This structure below defines the media profile for an application. 
	* The application can define its preference for media streams, 
	* codecs (and there parameters) within a media stream using this API.
	* IP Phone Toolkit will save this information within its database. 
	* The application is expected to define only those codecs in a stream,
	* which have been already configured with MM (by the application).
	* This is a list of the data type, icf_stream_capab_st.
	*/
	p_payload->bit_mask |= ICF_APP_ADD_MEDIA_PROFILE_PRESENT;

	/* Fill info for audio stream */
				 
	/*rewrite by Mike,
	*if you wanna add a AudioStreamDate,add the function addAudioStreamData(p_audio_stream_data, bit_mask, icf_stream_id_t, icf_media_type_t)
	*it use link_list to generate the AudioStreamDate
	*/
	zyMemset(&audio_stream_data, 0, sizeof(audio_stream_data));
	
	if( 0 > addAudioStreamData(&audio_stream_data, 0, VOICEAPP_CALL_TYPE_AUDIO_CALL, ICF_STREAM_TYPE_AUDIO) ) {
		icf_free_decoded_msg(p_msg,p_err);	
		return(FALSE);
	}
	if( 0 > addAudioStreamData(&audio_stream_data, 0, VOICEAPP_CALL_TYPE_FAX_CALL, ICF_STREAM_TYPE_FAX) ) {
		icf_free_decoded_msg(p_msg,p_err);	
		return(FALSE);
	}

	index_ptr = audio_stream_data.p_next;

	p_payload->p_media_profile = index_ptr;

	/* Posting the message to port layer */
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_APP_ADD_REQ, ICF_INVALID_CALL_ID);

	/* In icf_free_decoded_msg  , it doesn't free p_payload->p_media_profile , 
	so we need to do it by ourselves before using icf_free_decoded_msg .*/
	freeIcfListSt(p_payload->p_media_profile);
	p_payload->p_media_profile = NULL;
	icf_free_decoded_msg(p_msg,p_err);	
        p_msg = NULL;
	if(ret_val==ICF_FAILURE){
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppRemoveApp
 *
 * DESCRIPTION   : The function is to remove application from IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppRemoveApp(void) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_app_remove_req_st *p_payload = NULL;

	VOICEAPP_DBG("voiceApp -> IPTK : APP_REMOVE_REQ");
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_app_remove_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : APP_REMOVE_REQ\n");
		return(FALSE);
	}
	
	p_payload=(icf_app_remove_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_APP_REMOVE_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSetTransportParams
 *
 * DESCRIPTION   : The function to fill the proxy address to be added to be send
 *				  towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppSetTransportParams(uint16 SipAccountId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_set_transport_req_st   *p_payload;
	struct hostent* phostent;
	struct      in_addr proxyServerAddr;
	struct      in_addr inAddr;
	char proxyServer[SIP_SERVER_ADDR_LEN+1];
	struct in_addr validateBuf;
	uint8 useOutboundProxy = 0;

	int LineItr = 0, VpItr=0;
	VOICEAPP_DBG1("voiceApp -> IPTK : SET_TRANSPORT_REQ (SipAccountId ", SipAccountId);

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfObj_t* pProf = tr104VoiceProfObjPtrGet(VpItr);
	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);



	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_transport_req_st),ret_val);
		
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : SET_TRANSPORT_PARAM_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_set_transport_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;

	/* Contains, the presence information of optional fields in this structure.*/
	p_payload->bit_mask = ICF_SET_TRP_REQ_PROXY_ADDR;

	/*Indicates the SIP proxy IP address. 
	  * This parameter is configured per line.
	  * This is an optional field. 
	  * IP PHONE TOOLKIT also supports direct routing 
	  * in which case proxy address is optional.
	  */
	p_payload->proxy_addr.line_id = SipAccountId;
	p_payload->proxy_addr.action = ICF_CONFIG_ACTION_ADD;


//support outbound proxy
	if( zyStrcmp("0.0.0.0",pProfSip->outboundProxy) != 0 
		&& zyStrcmp("",pProfSip->outboundProxy) != 0)
	{
		zyStrcpy(proxyServer, pProfSip->outboundProxy);
		p_payload->proxy_addr.transport_addr.port_num = pProfSip->outboundProxyPort;
		VOICEAPP_PRINTF("voiceAppSetTransportParams(): use outbound ProxyServer is %s\n",proxyServer);		
		useOutboundProxy = 1 ;
	}else{
		zyStrcpy(proxyServer, pProfSip->proxyServer);
		p_payload->proxy_addr.transport_addr.port_num = pProfSip->proxyServerPort;	
		VOICEAPP_PRINTF("voiceAppSetTransportParams(): ProxyServer is %s\n",proxyServer);		
	}

	if( zyStrcmp("",proxyServer) != 0 ){
		if( !inet_aton(proxyServer, &validateBuf) ){
			VOICEAPP_PRINTF("voiceAppSetTransportParams(): ProxyServer is domain format\n");
			/* ProxyServer is not ip address format so set domain name format to payload */
			p_payload->proxy_addr.transport_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_DOMAIN_NAME;
			zyStrcpy(p_payload->proxy_addr.transport_addr.addr.addr.domain.str, proxyServer);
			p_payload->proxy_addr.transport_addr.addr.addr.domain.str_len = strlen(proxyServer);	
			#if 1 /*Jason , to support DNS SVR */
			if(pProfSip->ZyXEL_rfc3263support && (!useOutboundProxy))
			{
				p_payload->proxy_addr.transport_addr.port_num=0;
			}
			#endif
			VOICEAPP_PRINTF("voiceAppSetTransportParams(): ProxyServer (%s)\n",p_payload->proxy_addr.transport_addr.addr.addr.domain.str);
			VOICEAPP_PRINTF("voiceAppSetTransportParams(): ProxyServer len (%d)\n",p_payload->proxy_addr.transport_addr.addr.addr.domain.str_len);
		}else{
			/* ProxyServer is ip address format */
	/* IP address is being filled like octet[1] =172, octet[2] = 16 */
			VOICEAPP_PRINTF("voiceAppSetTransportParams(): ProxyServer is ip address format\n");
			p_payload->proxy_addr.transport_addr.addr.addr_type=ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
			p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_1=atoi(zyStrtok(proxyServer,(char *)".\0"));
			p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_2=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_3=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_4=atoi(zyStrtok(NULL,(char *)".\0"));
		}
	}else{
		/* proxyServer ip or domain is not correct to send to iptk */
		ZyPrintf("voiceAppSetTransportParams(): ProxyServer is null\n");
		icf_free_decoded_msg(p_msg,p_err);		
		return FALSE;
	}

	p_payload->proxy_addr.transport_mode = pProfSip->proxyServerTransport;

	/* Denotes the self-IP address.This is an optional field. 
	  * This parameter can be optional if IP Phone Toolkit 
	  * can obtain Self-IP address.
	  */
	if(get_iface_ipv4_address(pProf->ZyXEL_ActiveBoundIfName, &inAddr) != 0) {
		ZyPrintf("voiceApp:-   GET IPADDRESS FAIL : SET_TRANSPORT_PARAM_REQ\n");
		icf_free_decoded_msg(p_msg,p_err);	
		return(FALSE);
	}
	/* IP address is being filled like octet[1] =172, octet[2] = 16 */

	p_payload->bit_mask |= ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE;
	p_payload->self_addr_per_line.line_id=SipAccountId;
	p_payload->self_addr_per_line.action=ICF_CONFIG_ACTION_ADD;
	p_payload->self_addr_per_line.transport_addr.addr.addr_type=ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
	p_payload->self_addr_per_line.transport_addr.addr.addr.ipv4_addr.octet_1=(inAddr.s_addr >> 24) & 0xFF;
	p_payload->self_addr_per_line.transport_addr.addr.addr.ipv4_addr.octet_2=(inAddr.s_addr >> 16) & 0xFF;
	p_payload->self_addr_per_line.transport_addr.addr.addr.ipv4_addr.octet_3=(inAddr.s_addr >> 8) & 0xFF;
	p_payload->self_addr_per_line.transport_addr.addr.addr.ipv4_addr.octet_4=inAddr.s_addr & 0xFF;
	p_payload->self_addr_per_line.transport_addr.port_num = pProfSip->userAgentPort;
	p_payload->self_addr_per_line.transport_mode= pProfSip->userAgentTransport;

	ZyPrintf("  ---> voiceApp to IPTK, transport param...\n");
	ZyPrintf("       Proxy:%s, port:%d, mode:%d\n",
		pProfSip->proxyServer,
		p_payload->proxy_addr.transport_addr.port_num,
		p_payload->proxy_addr.transport_mode);
	if(pProfSip->outboundProxy != NULL){
		ZyPrintf("	  outbound proxy: %s\n",pProfSip->outboundProxy);
	}	
	ZyPrintf("       My IP:%08X, port:%d\n",
		inAddr.s_addr,
		p_payload->self_port);

	/* Posting the message to port layer */
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_SET_TRANSPORT_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	

	if(ret_val==ICF_FAILURE){
		return(FALSE);
	}
		
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppDelTransportParams
 *
 * DESCRIPTION   : The function to delete the transport settings and send
 *				  towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppDelTransportParams(uint16 SipAccountId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_set_transport_req_st   *p_payload;
	struct hostent* phostent;
	struct      in_addr proxyServerAddr;
	struct in_addr validateBuf;	
	struct      in_addr inAddr;
	int LineItr = 0, VpItr=0;
	uint8 useOutboundProxy = 0;

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	VOICEAPP_DBG1("voiceApp -> IPTK : DEL_TRANSPORT_REQ (SipAccountId ", SipAccountId);

	char proxyServer[SIP_SERVER_ADDR_LEN+1];
	VoiceProfObj_t* pProf = tr104VoiceProfObjPtrGet(VpItr);
	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);



	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_transport_req_st),ret_val);
		
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : DEL_TRANSPORT_PARAM_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_set_transport_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;

	/* Contains, the presence information of optional fields in this structure.*/
	p_payload->bit_mask = ICF_SET_TRP_REQ_PROXY_ADDR;

	/*Indicates the SIP proxy IP address. 
	  * This parameter is configured per line.
	  * This is an optional field. 
	  * IP PHONE TOOLKIT also supports direct routing 
	  * in which case proxy address is optional.
	  */
	p_payload->proxy_addr.line_id = SipAccountId;
	p_payload->proxy_addr.action = ICF_CONFIG_ACTION_DELETE;
	

//support outbound 
	if( zyStrcmp("0.0.0.0",pProfSip->outboundProxy) != 0 && zyStrcmp("",pProfSip->outboundProxy) != 0){
		zyStrcpy(proxyServer, pProfSip->outboundProxy);
		p_payload->proxy_addr.transport_addr.port_num = pProfSip->outboundProxyPort;
		VOICEAPP_PRINTF("voiceAppDelTransportParams(): use outbound ProxyServer is %s\n",proxyServer);
		useOutboundProxy = 1;
		
	}else{
		zyStrcpy(proxyServer, pProfSip->proxyServer);
		p_payload->proxy_addr.transport_addr.port_num = pProfSip->proxyServerPort;
		VOICEAPP_PRINTF("voiceAppDelTransportParams(): ProxyServer is %s\n",proxyServer);
	}


	if( zyStrcmp("",proxyServer) != 0 ){
		if( !inet_aton(proxyServer, &validateBuf) ){
			VOICEAPP_PRINTF("voiceAppDelTransportParams(): ProxyServer is domain format\n");
			/* ProxyServer is not ip address format so set domain name format to payload */
			p_payload->proxy_addr.transport_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_DOMAIN_NAME;
			zyStrcpy(p_payload->proxy_addr.transport_addr.addr.addr.domain.str, proxyServer);
			p_payload->proxy_addr.transport_addr.addr.addr.domain.str_len = strlen(proxyServer);

			#if 1 /*Jason , to support DNS SVR */
			if(pProfSip->ZyXEL_rfc3263support && (!useOutboundProxy))
			{
				p_payload->proxy_addr.transport_addr.port_num=0;
			}
			#endif

			VOICEAPP_PRINTF("voiceAppDelTransportParams(): ProxyServer (%s)\n",p_payload->proxy_addr.transport_addr.addr.addr.domain.str);
			VOICEAPP_PRINTF("voiceAppDelTransportParams(): ProxyServer len (%d)\n",p_payload->proxy_addr.transport_addr.addr.addr.domain.str_len);
		}else{
			/* ProxyServer is ip address format */
			VOICEAPP_PRINTF("voiceAppDelTransportParams(): ProxyServer is ip address format\n");
			p_payload->proxy_addr.transport_addr.addr.addr_type=ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
			/* IP address is being filled like octet[1] =172, octet[2] = 16 */
			p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_1=atoi(zyStrtok(proxyServer,(char *)".\0"));
			p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_2=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_3=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_4=atoi(zyStrtok(NULL,(char *)".\0"));
		}
	}else{
		ZyPrintf("%s(): proxyServer format error\n",__FUNCTION__);
		/* proxyServer ip or domain is not correct to send to iptk */
		ZyPrintf("voiceAppDelTransportParams(): ProxyServer is null\n");
		icf_free_decoded_msg(p_msg,p_err);
		return FALSE;
	}

	p_payload->proxy_addr.transport_mode = pProfSip->proxyServerTransport;

	if(get_iface_ipv4_address(pProf->ZyXEL_ActiveBoundIfName, &inAddr) != 0) {
		ZyPrintf("voiceApp:-   GET IPADDRESS FAIL : SET_TRANSPORT_PARAM_REQ\n");
		icf_free_decoded_msg(p_msg,p_err);	
		return(FALSE);
	}
	p_payload->bit_mask |= ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE;
	p_payload->self_addr_per_line.line_id=SipAccountId;
	p_payload->self_addr_per_line.action=ICF_CONFIG_ACTION_DELETE;
	p_payload->self_addr_per_line.transport_addr.addr.addr_type=ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
	p_payload->self_addr_per_line.transport_addr.addr.addr.ipv4_addr.octet_1=(inAddr.s_addr >> 24) & 0xFF;
	p_payload->self_addr_per_line.transport_addr.addr.addr.ipv4_addr.octet_2=(inAddr.s_addr >> 16) & 0xFF;
	p_payload->self_addr_per_line.transport_addr.addr.addr.ipv4_addr.octet_3=(inAddr.s_addr >> 8) & 0xFF;
	p_payload->self_addr_per_line.transport_addr.addr.addr.ipv4_addr.octet_4=inAddr.s_addr & 0xFF;
	p_payload->self_addr_per_line.transport_addr.port_num = pProfSip->userAgentPort;
	p_payload->self_addr_per_line.transport_mode= pProfSip->userAgentTransport;

	ZyPrintf("  ---> voiceApp to IPTK, transport param...\n");
	ZyPrintf("       Proxy:%s, port:%d, mode:%d\n",
		pProfSip->proxyServer,
		p_payload->proxy_addr.transport_addr.port_num,
		p_payload->proxy_addr.transport_mode);

	/* Posting the message to port layer */
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_SET_TRANSPORT_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	
	if(ret_val==ICF_FAILURE){
		return(FALSE);
	}
		
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSetRegistrationParams
 *
 * DESCRIPTION   : The function to fill the registrar address, private password
 *					private user identity and authorization related parameters
 *					to be added	to be send towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppSetRegistrationParams(uint16 SipAccountId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st					 *p_msg;
	icf_return_t				 ret_val;
	icf_set_registration_req_st  *p_payload;
	struct hostent* phostent;
	struct in_addr registrarServerAddr;

	struct hostent* phostent_op ; 
	struct in_addr outboundProxyAddr;

	struct in_addr validateBuf;
	char registrarServer[SIP_REGISTER_ADDR_LEN+1];
	char outboundproxy[SIP_REGISTER_ADDR_LEN+1];
	char userAgentDomain[SIP_DOMAIN_NAME_LEN+1];

	int LineItr = 0, VpItr=0;
	VOICEAPP_DBG1("voiceApp -> IPTK : SET_REGISTRATION_REQ (SipAccountId", SipAccountId);	
	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
	ZyXEL_VoiceCommon_t* pCommon = tr104VoiceCommonObjPtrGet();

		
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_registration_req_st  ),ret_val);

	if(ret_val==ICF_FAILURE){
		ZyPrintf("MEMORY ALLOC FAIL : SET_REGISTRATION_PARAM_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_set_registration_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Specifies the line id for which the registration parameters are
	* applicable. If the value is ICF_ALL_LINES, the parameters 
	* specified in this API is applicable for all lines configured.
	*/
	p_payload->line_id = SipAccountId;

	/* Actions can be add, delete or modify.
	*
	* If action is add then registrar address is mandatory. 
	* Other params are optional.
	*
	* If action is modify, any parameter other then 
	* registrar_address can be present.
	*
	* If action is delete, then no other param is required. 
	* All the registration data for the line is deleted.
	*/
	p_payload->action = ICF_CONFIG_ACTION_ADD;

	/* Contains, the presence information of optional fields in this structure*/
	p_payload->bit_mask=0;
	
	/* Indicates the SIP registrar IP address. This field 
	* contains significant value only when action is add.
	*/
	zyStrcpy(registrarServer, pProfSip->registrarServer);

	VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer is %s\n",registrarServer);
	zyStrcpy(userAgentDomain, pProfSip->userAgentDomain);
	VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): userAgentDomain is %s\n",userAgentDomain);	
	if( zyStrcmp("",registrarServer) != 0 && (zyStrcmp("",userAgentDomain) != 0) ){
		/* use UserAgentDomain in REGISTER Request-URI */		
		if( !inet_aton(userAgentDomain, &validateBuf) ){
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrar_addr is domain format\n");
			/* registrarServer is not ip address format so set domain name format to payload */
			p_payload->registrar_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_DOMAIN_NAME;
			zyStrcpy(p_payload->registrar_addr.addr.addr.domain.str, userAgentDomain);
			p_payload->registrar_addr.addr.addr.domain.str_len = strlen(userAgentDomain);
			#if 1 /*Jason , to support DNS SVR */
			if(pProfSip->ZyXEL_rfc3263support){
				p_payload->registrar_addr.port_num = 0;
			}else{
				if( (pCommon->voipIOPFlags & VOIP_IOP_FLAG_REMOVE_DEFAULT_PORT_IN_REQUEST_URI) &&
					(pProfSip->registrarServerPort == 5060)){
					p_payload->registrar_addr.port_num = 0;	
				}
				else{
					p_payload->registrar_addr.port_num = pProfSip->registrarServerPort;			
				}
			}
			#endif
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrar_addr (%s)\n",p_payload->registrar_addr.addr.addr.domain.str);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrar_addr len (%d)\n",p_payload->registrar_addr.addr.addr.domain.str_len);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrar_addr port (%d)\n", p_payload->registrar_addr.port_num);
		}else{
			/* registrarServer is ip address format */
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrar_addr is ip address format\n");
			p_payload->registrar_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
	/* IP address is being filled like octet[1] =172, octet[2] = 16 */
			p_payload->registrar_addr.addr.addr.ipv4_addr.octet_1=atoi(zyStrtok(userAgentDomain,(char *)".\0"));
			p_payload->registrar_addr.addr.addr.ipv4_addr.octet_2=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->registrar_addr.addr.addr.ipv4_addr.octet_3=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->registrar_addr.addr.addr.ipv4_addr.octet_4=atoi(zyStrtok(NULL,(char *)".\0"));	
			if( (pCommon->voipIOPFlags & VOIP_IOP_FLAG_REMOVE_DEFAULT_PORT_IN_REQUEST_URI) &&
				(pProfSip->registrarServerPort == 5060)){
				p_payload->registrar_addr.port_num = 0;	
			}
			else{
				p_payload->registrar_addr.port_num = pProfSip->registrarServerPort;			
			}
		}
	}else{
		/* registrarServer ip or domain is not correct to send to iptk */
		VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): RegistrarServer or UserAgentDomain is null\n");
		icf_free_decoded_msg(p_msg,p_err);	
		return FALSE ;
	}


	/* Indicates the mode of transport for registration request */
	p_payload->bit_mask |= ICF_SET_REG_REQ_MODE ;
	p_payload->reg_mode = pProfSip->registrarServerTransport;

//support outbound proxy
	zyStrcpy(outboundproxy, pProfSip->outboundProxy);
	VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): outboundproxy is %s\n",outboundproxy);

	if(zyStrcmp("0.0.0.0", outboundproxy) != 0 
		&& zyStrcmp("", outboundproxy) != 0){
		//outbound proxy not empty, support outbound proxy
		p_payload->bit_mask |= ICF_SET_REG_REQ_ROUTE_ADDR ;
		if( !inet_aton(outboundproxy, &validateBuf) ){
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): outbound proxy is domain format\n");
			/* registrarServer is not ip address format so set domain name format to payload */
			p_payload->reg_route_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_DOMAIN_NAME;
			zyStrcpy(p_payload->reg_route_addr.addr.addr.domain.str, outboundproxy);
			p_payload->reg_route_addr.addr.addr.domain.str_len = strlen(outboundproxy);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): outbound proxy (%s)\n",p_payload->reg_route_addr.addr.addr.domain.str);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): outbound proxy len (%d)\n",p_payload->reg_route_addr.addr.addr.domain.str_len);
		}else{
			/* registrarServer is ip address format */
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): outbound proxy is ip address format\n");
			p_payload->reg_route_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
			/* IP address is being filled like octet[1] =172, octet[2] = 16 */
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_1=atoi(zyStrtok(outboundproxy,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_2=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_3=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_4=atoi(zyStrtok(NULL,(char *)".\0"));
		}

		p_payload->reg_route_addr.port_num = pProfSip->outboundProxyPort;

	}else if(zyStrcmp(pProfSip->userAgentDomain,registrarServer) != 0){
		/* UserAgentDomain is diffrent with RegistrarServer, add register route */
		p_payload->bit_mask |= ICF_SET_REG_REQ_ROUTE_ADDR ;
		if( !inet_aton(registrarServer, &validateBuf) ){
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer is domain format\n");
			/* registrarServer is not ip address format so set domain name format to payload */
			p_payload->reg_route_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_DOMAIN_NAME;
			zyStrcpy(p_payload->reg_route_addr.addr.addr.domain.str, registrarServer);
			p_payload->reg_route_addr.addr.addr.domain.str_len = strlen(registrarServer);
			if(pProfSip->ZyXEL_rfc3263support){
				p_payload->reg_route_addr.port_num = 0;
			} else {
				p_payload->reg_route_addr.port_num = pProfSip->registrarServerPort;
			}
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer (%s)\n",p_payload->reg_route_addr.addr.addr.domain.str);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer len (%d)\n",p_payload->reg_route_addr.addr.addr.domain.str_len);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer port (%d)\n", p_payload->reg_route_addr.port_num);
		} else {
			/* registrarServer is ip address format */
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer is ip address format\n");
			p_payload->reg_route_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
			/* IP address is being filled like octet[1] =172, octet[2] = 16 */
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_1=atoi(zyStrtok(registrarServer,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_2=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_3=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_4=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->reg_route_addr.port_num = pProfSip->registrarServerPort;
		}
	}

	/* Indicates the duration of registration in milliseconds. 
	* This parameter is configured per line. The 0xffff 
	* value indicates infinite duration for registration. 
	* Default value of registration_duration is 300000 ms.*/
	p_payload->bit_mask |= ICF_SET_REG_REQ_DURATION;
	p_payload->registration_duration = 1000*pProfSip->registerExpires;
	
	VOICEAPP_PRINTF("  ---> voiceApp to IPTK, register param...\n");
	VOICEAPP_PRINTF("       RegisAddr:%s, port:%d, mode:%d\n",
		pProfSip->registrarServer,
		p_payload->registrar_addr.port_num,
		p_payload->reg_mode);
	VOICEAPP_PRINTF("	  outbound proxy: %s\n",pProfSip->outboundProxy);		
	VOICEAPP_PRINTF("       Regis duration:%d\n",
		p_payload->registration_duration);
	
	ret_val = voiceAppFillHdrSendIptk(p_msg,ICF_SET_REGISTRATION_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppDelRegistrationParams
 *
 * DESCRIPTION   : The function to delete the registration settings and send towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppDelRegistrationParams(uint16 SipAccountId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st					 *p_msg;
	icf_return_t				 ret_val;
	icf_set_registration_req_st  *p_payload;
	struct hostent* phostent;
	struct in_addr registrarServerAddr;

	struct hostent* phostent_op ; 
	struct in_addr outboundProxyAddr;

	char registrarServer[SIP_REGISTER_ADDR_LEN+1];
	char outboundproxy[SIP_REGISTER_ADDR_LEN+1];
	char userAgentDomain[SIP_DOMAIN_NAME_LEN+1];	
	struct in_addr validateBuf;

	int LineItr = 0, VpItr=0;
	VOICEAPP_DBG1("voiceApp -> IPTK : DEL_REGISTRATION_REQ (SipAccountId ", SipAccountId);

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);


		
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_registration_req_st  ),ret_val);

	if(ret_val==ICF_FAILURE){
		ZyPrintf("MEMORY ALLOC FAIL : DEL_REGISTRATION_PARAM_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_set_registration_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Contains, the presence information of optional fields in this structure*/
	p_payload->bit_mask=0;

	/* Specifies the line id for which the registration parameters are
	* applicable. If the value is ICF_ALL_LINES, the parameters 
	* specified in this API is applicable for all lines configured.
	*/
	p_payload->line_id = SipAccountId;

	/* Actions can be add, delete or modify.
	*
	* If action is add then registrar address is mandatory. 
	* Other params are optional.
	*
	* If action is modify, any parameter other then 
	* registrar_address can be present.
	*
	* If action is delete, then no other param is required. 
	* All the registration data for the line is deleted.
	*/
	p_payload->action = ICF_CONFIG_ACTION_DELETE;
	zyStrcpy(registrarServer, pProfSip->registrarServer);

	VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer is %s\n",registrarServer);
	zyStrcpy(userAgentDomain, pProfSip->userAgentDomain);
	VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): userAgentDomain is %s\n",userAgentDomain);	
	if( zyStrcmp("",registrarServer) != 0 && (zyStrcmp("",userAgentDomain) != 0) ){
		/* use UserAgentDomain in REGISTER Request-URI */			
		if( !inet_aton(userAgentDomain, &validateBuf) ){
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrar_addr is domain format\n");
			/* registrarServer is not ip address format so set domain name format to payload */
			p_payload->registrar_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_DOMAIN_NAME;
			zyStrcpy(p_payload->registrar_addr.addr.addr.domain.str, userAgentDomain);
			p_payload->registrar_addr.addr.addr.domain.str_len = strlen(userAgentDomain);
			#if 1 /*Jason , to support DNS SVR */
			if(pProfSip->ZyXEL_rfc3263support){
				p_payload->registrar_addr.port_num = 0;
			}else{
				p_payload->registrar_addr.port_num = pProfSip->registrarServerPort;
			}
			#endif
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrar_addr (%s)\n",p_payload->registrar_addr.addr.addr.domain.str);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrar_addr len (%d)\n",p_payload->registrar_addr.addr.addr.domain.str_len);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrar_addr port (%d)\n", p_payload->registrar_addr.port_num);
		}else{
			/* registrarServer is ip address format */
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrar_addr is ip address format\n");
			p_payload->registrar_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
	/* IP address is being filled like octet[1] =172, octet[2] = 16 */
			p_payload->registrar_addr.addr.addr.ipv4_addr.octet_1=atoi(zyStrtok(userAgentDomain,(char *)".\0"));
			p_payload->registrar_addr.addr.addr.ipv4_addr.octet_2=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->registrar_addr.addr.addr.ipv4_addr.octet_3=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->registrar_addr.addr.addr.ipv4_addr.octet_4=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->registrar_addr.port_num = pProfSip->registrarServerPort;
		}
	}else{
		/* registrarServer ip or domain is not correct to send to iptk */
		VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): RegistrarServer or UserAgentDomain is null\n");
		icf_free_decoded_msg(p_msg,p_err);	
		return FALSE ;
	}
	

	zyStrcpy(outboundproxy, pProfSip->outboundProxy);
	VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): outboundproxy is %s\n",outboundproxy);

	if(zyStrcmp("0.0.0.0", outboundproxy) != 0 
		&& zyStrcmp("", outboundproxy) != 0){
		//outbound proxy not empty, support outbound proxy
		p_payload->bit_mask |= ICF_SET_REG_REQ_ROUTE_ADDR ;
		if( !inet_aton(outboundproxy, &validateBuf) ){
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): outbound proxy is domain format\n");
			/* registrarServer is not ip address format so set domain name format to payload */
			p_payload->reg_route_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_DOMAIN_NAME;
			zyStrcpy(p_payload->reg_route_addr.addr.addr.domain.str, outboundproxy);
			p_payload->reg_route_addr.addr.addr.domain.str_len = strlen(outboundproxy);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): outbound proxy (%s)\n",p_payload->reg_route_addr.addr.addr.domain.str);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): outbound proxy len (%d)\n",p_payload->reg_route_addr.addr.addr.domain.str_len);
		}else{
			/* registrarServer is ip address format */
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): outbound proxy is ip address format\n");
			p_payload->reg_route_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
			/* IP address is being filled like octet[1] =172, octet[2] = 16 */
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_1=atoi(zyStrtok(outboundproxy,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_2=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_3=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_4=atoi(zyStrtok(NULL,(char *)".\0"));
		}

		p_payload->reg_route_addr.port_num = pProfSip->outboundProxyPort;

	}else if(zyStrcmp(pProfSip->userAgentDomain,registrarServer) != 0){
		/* UserAgentDomain is diffrent with RegistrarServer, add register route */
		p_payload->bit_mask |= ICF_SET_REG_REQ_ROUTE_ADDR ;
		if( !inet_aton(registrarServer, &validateBuf) ){
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer is domain format\n");
			/* registrarServer is not ip address format so set domain name format to payload */
			p_payload->reg_route_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_DOMAIN_NAME;
			zyStrcpy(p_payload->reg_route_addr.addr.addr.domain.str, registrarServer);
			p_payload->reg_route_addr.addr.addr.domain.str_len = strlen(registrarServer);
			if(pProfSip->ZyXEL_rfc3263support){
				p_payload->reg_route_addr.port_num = 0;
			} else {
				p_payload->reg_route_addr.port_num = pProfSip->registrarServerPort;
			}
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer (%s)\n",p_payload->reg_route_addr.addr.addr.domain.str);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer len (%d)\n",p_payload->reg_route_addr.addr.addr.domain.str_len);
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer port (%d)\n", p_payload->reg_route_addr.port_num);
		} else {
			/* registrarServer is ip address format */
			VOICEAPP_PRINTF("voiceAppSetRegistrationParams(): registrarServer is ip address format\n");
			p_payload->reg_route_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
			/* IP address is being filled like octet[1] =172, octet[2] = 16 */
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_1=atoi(zyStrtok(registrarServer,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_2=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_3=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->reg_route_addr.addr.addr.ipv4_addr.octet_4=atoi(zyStrtok(NULL,(char *)".\0"));
			p_payload->reg_route_addr.port_num = pProfSip->registrarServerPort;
		}
	}

	
	ret_val = voiceAppFillHdrSendIptk(p_msg,ICF_SET_REGISTRATION_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	VoiceProfLineObj_t *lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineCallingFeaturesObj_t *callfeaturesObj = lineObj->callingFeatures;
	if(NULL != callfeaturesObj) {
		callfeaturesObj->messageWaiting = FALSE;
	}
	setNewVoiceMessage(SipAccountId, FALSE);

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSetSelfId
 *
 * DESCRIPTION   : The function to fill the public address to be added to be send
 *					towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppSetSelfId(uint16 SipAccountId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_set_self_id_req_st *p_payload = NULL;
	char sipUrl[ICF_MAX_STR_LEN];

	int LineItr = 0, VpItr=0;
	VOICEAPP_DBG1("voiceApp -> IPTK : SET_SELF_ID_REQ (SipAccountId ", SipAccountId);

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
	VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineSipObj_t* pLineSip = tr104VoiceLineSipObjPtrGet(VpItr, LineItr);
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);



	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_self_id_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : SET_SELF_ID_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_set_self_id_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Contains the presence information of optional fields in this
	* structure, bitwise.
	*/
	p_payload->bit_mask=0;

	/* Specifies the line id for which the parameters are applicable.
	* The special value of ICF_ALL_LINES signifies that the 
	* specified values are applicable for all lines.
	*/

	p_payload->line_id = SipAccountId;

	/* This field specifies the action for self_addr
	* and self_name field. Action can be add or delete.
	*/
	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_ADDR_ACTION;
	p_payload->action=ICF_CONFIG_ACTION_ADD;

	/* Contains the user name for each line. 
	* These can be same for more than one line.
	*/
	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_NAME ;
	p_payload->self_name.str_len=zyStrlen(pLineFeature->callerIDName);
	zyStrcpy((char *)p_payload->self_name.str, pLineFeature->callerIDName);

	/* Contains self-address for the line. 
	* The same address can be used for more than one line. 
	*/
	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_ADDR;
	if(pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_SIP_URI 
		||pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_SIPS_URI){
	    sprintf(sipUrl, "%s@%s", pLine->directoryNumber, pProfSip->userAgentDomain);
	}else if(pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_TEL_URI){
		sprintf(sipUrl, "+%s", pLine->directoryNumber);
	}
	p_payload->self_addr.addr_val.str_len = zyStrlen((char *)sipUrl); 
	zyStrcpy((char *)p_payload->self_addr.addr_val.str,(char *)sipUrl);

	p_payload->self_addr.addr_type =  pLineSip->ZyXEL_url_type;

	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_DEFAULT_SETTINGS;
#ifdef NORWAY_VOIP_CUSTOMIZATION
	p_payload->default_settings |= ICF_SETTINGS_CONFIG_PRESENTATION_IND;
#else	
	if(pLineFeature->anonymousCallEnable) {
		p_payload->default_settings &= ~ICF_SETTINGS_CONFIG_PRESENTATION_IND;
	} else {
		p_payload->default_settings |= ICF_SETTINGS_CONFIG_PRESENTATION_IND;
	}
#endif
	
	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_AUTH_KEY;
	zyStrcpy((char *)p_payload->authentication_key.str,pLineSip->authUserName);
	p_payload->authentication_key.str_len = zyStrlen(pLineSip->authUserName);

	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_AUTH_PASSWORD;
	zyStrcpy((char *)p_payload->authentication_password.str,pLineSip->authPassword);
	p_payload->authentication_password.str_len = zyStrlen(pLineSip->authPassword);

	VOICEAPP_PRINTF("  ---> voiceApp to IPTK, Self ID param...\n");
	VOICEAPP_PRINTF("       self ID addr type:%d, self ID addr :%s\n",
		p_payload->self_addr.addr_type,
		p_payload->self_addr.addr_val.str);
	VOICEAPP_PRINTF("       self ID name len:%d, self ID name :%s\n",
		p_payload->self_name.str_len,
		p_payload->self_name.str);
	VOICEAPP_PRINTF("       username:%s, password:%s\n",
		p_payload->authentication_key.str,
		p_payload->authentication_password.str);
	VOICEAPP_PRINTF("       realm:%s \n",
		p_payload->configured_realm.str);

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_SET_SELF_ID_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppDelSelfId
 *
 * DESCRIPTION   : The function to delete the public address settings and send towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppDelSelfId(uint16 SipAccountId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_set_self_id_req_st *p_payload = NULL;
	char sipUrl[ICF_MAX_STR_LEN];
	VOICEAPP_DBG1("voiceApp -> IPTK : DEL_SELF_ID_REQ (SipAccountId ", SipAccountId);

	int LineItr = 0, VpItr=0;

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
	VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineSipObj_t* pLineSip = tr104VoiceLineSipObjPtrGet(VpItr, LineItr);
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);



	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_self_id_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : DEL_SELF_ID_PARAM_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_set_self_id_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Contains the presence information of optional fields in this
	* structure, bitwise.
	*/
	p_payload->bit_mask=0;

	/* Specifies the line id for which the parameters are applicable.
	* The special value of ICF_ALL_LINES signifies that the 
	* specified values are applicable for all lines.
	*/

	p_payload->line_id = SipAccountId;

	/* This field specifies the action for self_addr
	* and self_name field. Action can be add or delete.
	*/
	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_ADDR_ACTION;
	p_payload->action=ICF_CONFIG_ACTION_DELETE;

	/* Contains the user name for each line. 
	* These can be same for more than one line.
	*/
	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_NAME ;
	if(pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_SIP_URI 
		||pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_SIPS_URI){
	        sprintf(sipUrl, "%s@%s", pLine->directoryNumber, pProfSip->userAgentDomain);
	}else if(pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_TEL_URI){
		sprintf(sipUrl, "+%s", pLine->directoryNumber);
	}
	p_payload->self_name.str_len=zyStrlen((char *)sipUrl);
	zyStrcpy((char *)p_payload->self_name.str,(char *)sipUrl);

	/* Contains self-address for the line. 
	* The same address can be used for more than one line. 
	*/
	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_ADDR;
	p_payload->self_addr.addr_val.str_len = zyStrlen((char *)sipUrl); 
	zyStrcpy((char *)p_payload->self_addr.addr_val.str,(char *)sipUrl);

	p_payload->self_addr.addr_type =  pLineSip->ZyXEL_url_type;

	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_DEFAULT_SETTINGS;

#ifdef NORWAY_VOIP_CUSTOMIZATION
	p_payload->default_settings |= ICF_SETTINGS_CONFIG_PRESENTATION_IND;
#else
	if(pLineFeature->anonymousCallEnable) {
		p_payload->default_settings &= ~ICF_SETTINGS_CONFIG_PRESENTATION_IND;
	} else {
		p_payload->default_settings |= ICF_SETTINGS_CONFIG_PRESENTATION_IND;
	}
#endif	

	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_AUTH_KEY;
	zyStrcpy((char *)p_payload->authentication_key.str,pLineSip->authUserName);
	p_payload->authentication_key.str_len = zyStrlen(pLineSip->authUserName);

	p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_AUTH_PASSWORD;
	zyStrcpy((char *)p_payload->authentication_password.str,pLineSip->authPassword);
	p_payload->authentication_password.str_len = zyStrlen(pLineSip->authPassword);

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_SET_SELF_ID_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSetCallParam
 *
 * DESCRIPTION   : The function is to fill call-related parameters to be send towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppSetCallParam(uint16 profileId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_set_call_params_req_st *p_payload = NULL;
	VOICEAPP_DBG("voiceApp -> IPTK : SET_CALL_PARAM_REQ");

	ZyXEL_VoiceCommon_t* pCommon = tr104VoiceCommonObjPtrGet();
	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(profileId);
	VoiceProfRtpObj_t* pProfRtp = tr104VoiceProfRtpObjPtrGet(profileId);
	#if SRTP_SUPPORT
	VoiceProfRtpSrtpObj_t* pProfSrtp = tr104VoiceProfRtpSrtpObjPtrGet(profileId);
	#endif

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_call_params_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : SET_CALL_PARAM_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_set_call_params_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Contains the presence information of optional fields in this
	* structure, bitwise.
	*/
	VOICEAPP_PRINTF("voip IOP flags = 0x%x\n", pCommon->voipIOPFlags);

	p_payload->bit_mask=0;

	p_payload->bit_mask |= ICF_SET_CALL_PRM_DEFAULT_SETTINGS ;
	p_payload->default_settings |= ICF_CALL_CONFIG_CALL_THRU_SIP_PROXY;
	p_payload->default_settings |= ICF_CALL_CONFIG_PRIVACY_HEADER_PRESENT;
#if 0 /* ZyXEL Jacky, use INVITE as modify method */
	p_payload->default_settings |= ICF_CALL_CONFIG_CALL_MODIFY_METHOD_UPDATE;
#endif
#if 0 /* ZyXEL Jacky, turn off P-Early-Media header support */
	//eric.chung , early media
	p_payload->default_settings |= ICF_CALL_CONFIG_EARLY_MEDIA_HDR_SUPPORTED;
#endif
	if(pProfSip->ZyXEL_rfc3262support) {
		p_payload->default_settings |= ICF_CALL_CONFIG_PRACK_REQD;
	}

	
	p_payload->bit_mask |= ICF_SET_CALL_PRM_UPDATE_REQD;
	if(pCommon->voipIOPFlags & VOIP_IOP_FLAG_UPDATE_REQD_NO_UPDATE){
		p_payload->update_reqd = ICF_NO_UPDATE;
	}
	else{
		p_payload->update_reqd = ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC;
	}

	p_payload->bit_mask |= ICF_SET_CALL_PRM_PHONE_INFO;
	p_payload->phone_info.str_len = zyStrlen(pCommon->softwareVersion); 
	zyStrcpy((char *)p_payload->phone_info.str, pCommon->softwareVersion);

	p_payload->bit_mask |= ICF_SET_CALL_PRM_SESSION_TIMER_INTERVAL;
	p_payload->session_timer =  pProfSip->inviteExpires;	

	#if 1 /*eason, SPRID#090824353*/
	p_payload->bit_mask |= ICF_SET_CALL_PRM_ALERTING_TIMER;
	p_payload->alerting_timer = 180000;
	#endif

	#if 1 /*eason, release call if no response within 1 second*/
	p_payload->bit_mask |= ICF_SET_CALL_PRM_RELEASE_TIMER;
	p_payload->release_timer = 32000;
	#endif

#if 1 /*Jason , [SPRID # 090618210] [SPRID # 090715106]*/

		p_payload->bit_mask |= ICF_SET_CALL_PRM_QOS_MARK;
		p_payload->qos_mark = (pProfRtp->DSCPMark<<2);

#endif

	p_payload->bit_mask |= ICF_SET_CALL_PRM_MULTIPLE_ANSWER_ENABLE;
#ifdef NORWAY_VOIP_CUSTOMIZATION
	p_payload->multiple_answer_enable = ICF_FALSE;
#else	
	p_payload->multiple_answer_enable = 1;
#endif

#ifdef IPTK_REL_8_0
	p_payload->bit_mask |= ICF_SET_CALL_PRM_ACCESS_NW_INFO;
	p_payload->access_network_info.str_len=zyStrlen("3GPP-UTRAN-TDD; utran-cell-id-3gpp=234151D0FCE11");
	zyStrcpy((char *)p_payload->access_network_info.str, "3GPP-UTRAN-TDD; utran-cell-id-3gpp=234151D0FCE11");
#endif

#if defined (IPTK_REL_7_1) 
	p_payload->bit_mask |= ICF_SET_CALL_PRM_APP_CONNECT_REQD_FOR_REPLACES;
	p_payload->app_connect_reqd_for_replaces = ICF_TRUE;
#elif defined(IPTK_REL_8_2_2)
	p_payload->bit_mask_1 |= ICF_SET_CALL_PRM_APP_CONNECT_REQD_FOR_REPLACES;
	p_payload->app_connect_reqd_for_replaces = ICF_TRUE;
#endif

#if defined(IPTK_REL_8_2_2) /*[SPRID # 090910851]*/
	p_payload->bit_mask |=ICF_SET_CALL_PRM_MAX_ACTIVE_CALLS_PER_LINE;
	p_payload->max_active_calls_per_line = 2;
#endif

#if SRTP_SUPPORT
	VOICEAPP_PRINTF(" srtp enable = %d\n", pProfSrtp->enable);
	if(pProfSrtp->enable){
		p_payload->bit_mask |= ICF_SET_CALL_PRM_MEDIA_TYPE_REQD;
		p_payload->media_transport_type = ICF_TRANSPORT_MODE_RTP_SAVP;
	}
	else{
		p_payload->bit_mask |= ICF_SET_CALL_PRM_MEDIA_TYPE_REQD;
		p_payload->media_transport_type = ICF_TRANSPORT_MODE_RTP_AVP;		
	}
#endif
	VOICEAPP_PRINTF("  ---> voiceApp to IPTK, call param...\n");
	VOICEAPP_PRINTF("       session time:%d\n",
		p_payload->session_timer);
	VOICEAPP_PRINTF("       qos_mark :%d\n",
		p_payload->qos_mark);	
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_SET_CALL_PARAM_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSetServiceParam
 *
 * DESCRIPTION   : The function is to fill service-related parameters to be send towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppSetServiceParam(uint16 SipAccountId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_set_service_params_req_st *p_payload = NULL;
	char sipUrl[ICF_MAX_STR_LEN];

	uint16 LineItr = 0 , VpItr=0;
	VOICEAPP_DBG1("voiceApp -> IPTK : SET_SERVICE_PARAMS_REQ (SipAccountId", SipAccountId);

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfLineCallingFeaturesObj_t* pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);


	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_service_params_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : SET_SERVICE_PARAMS_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_set_service_params_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Contains the presence information of optional fields in this
	  * structure, bitwise.
	  */
	p_payload->bit_mask=0;

	p_payload->bit_mask |= ICF_SET_SER_PRM_SUBSCRIPTION_FLAG ;
	p_payload->bit_mask |= ICF_SET_SER_PRM_ACTIVATION_FLAG ;
	p_payload->service_subscription_flag.line_id = SipAccountId;
	p_payload->service_activation_flag.line_id = SipAccountId;
	
	p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_CALL_HOLD_RESUME;
	p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_HOLD_RESUME;

	if(pLineFeature->callWaitingEnable) 
    {
		p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_CALL_WAITING;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_WAITING;
#if 1 /*Jason , [RD Internal] #340 */
		p_payload->bit_mask |= ICF_SET_SER_PRM_CW_ALERT_TIMER;
		p_payload->cw_alert_timer = 4194304000;/*0xFA000000*/
#endif
	}
	if(pLineFeature->callTransferEnable) {
		p_payload->service_subscription_flag.status |=ICF_SERVICE_SUBS_CALL_TRF_ATTENDED;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_TRF_ATTENDED;
		p_payload->service_subscription_flag.status |=ICF_SERVICE_SUBS_CALL_TRF_UNATTENDED;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_TRF_UNATTENDED;
		p_payload->service_subscription_flag.status |=ICF_SERVICE_SUBS_CALL_TRF_FROM_NW;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_TRF_FROM_NW;
		p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_INVITE_WITH_REPLACE;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_INVITE_WITH_REPLACE;

		p_payload->bit_mask |= ICF_SET_SER_PRM_CONF_REQD_AT_TRANFEREE;
		p_payload->conf_cfm_reqd_at_transferee = ICF_FALSE/*FALSE*/;
	}
	if(pLineFeature->callForwardOnBusyEnable) {
		p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_CALL_FWD_BUSY;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_FWD_BUSY;
	}
	if(pLineFeature->callForwardOnNoAnswerEnable) {
		p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_CALL_FWD_NO_ANSWER;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER;		
	}
	if(pLineFeature->callForwardUnconditionalEnable) {
		p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_CALL_FWD_UNCONDITIONAL;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL;
	}
	/* Conference Call settings */
	p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_THREE_WAY_CALL;
	p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_THREE_WAY_CALL;
#ifdef IPTK_REL_7_1
	p_payload->bit_mask |= ICF_SET_SER_PRM_CONF_FAIL_CALL_CLEAR_NOT_REQD;
	p_payload->conf_fail_call_clear_not_reqd = ICF_TRUE;
#endif

	p_payload->bit_mask |= ICF_SET_SER_PRM_MAX_CALL_WAITING;
	p_payload->max_num_of_calls_in_call_waiting = 2;

	/* Call Forward settings */
if(pLineFeature->callForwardUnconditionalEnable) {	
	p_payload->bit_mask |= ICF_SET_SER_PRM_CFU_ADDR;
	p_payload->addr_for_cfu.line_id = SipAccountId;
	p_payload->addr_for_cfu.address.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
	sprintf(sipUrl, "%s@%s:%d", pLineFeature->callForwardUnconditionalNumber, pProfSip->userAgentDomain, pProfSip->proxyServerPort);
	zyStrcpy(p_payload->addr_for_cfu.address.addr_val.str, (char *)sipUrl);
	p_payload->addr_for_cfu.address.addr_val.str_len = zyStrlen((char *)sipUrl);

}	

if(pLineFeature->callForwardOnBusyEnable) {	
	p_payload->bit_mask |= ICF_SET_SER_PRM_CFB_ADDR;
	p_payload->addr_for_cfb.line_id = SipAccountId;
	p_payload->addr_for_cfb.address.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
	sprintf(sipUrl, "%s@%s:%d", pLineFeature->callForwardOnBusyNumber, pProfSip->userAgentDomain, pProfSip->proxyServerPort);
	zyStrcpy(p_payload->addr_for_cfb.address.addr_val.str, (char *)sipUrl);
	p_payload->addr_for_cfb.address.addr_val.str_len = zyStrlen((char *)sipUrl);

}

if(pLineFeature->callForwardOnNoAnswerEnable) {	
	p_payload->bit_mask |= ICF_SET_SER_PRM_CFD_ADDR;
	p_payload->addr_for_cfd.line_id = SipAccountId;
	p_payload->addr_for_cfd.address.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
	sprintf(sipUrl, "%s@%s:%d", pLineFeature->callForwardOnNoAnswerNumber, pProfSip->userAgentDomain, pProfSip->proxyServerPort);
	zyStrcpy(p_payload->addr_for_cfd.address.addr_val.str, (char *)sipUrl);
	p_payload->addr_for_cfd.address.addr_val.str_len = zyStrlen((char *)sipUrl);

}	

#if 1
	/*cnd_alert_timer:(ms)*/
	p_payload->bit_mask |= ICF_SET_SER_PRM_CND_ALERT_TIMER;
	p_payload->cnd_alert_timer = pLineFeature->callForwardOnNoAnswerRingCount*1000;
#endif	
#if 1 /*Jason , [RD Internal] #340 */
	p_payload->bit_mask |= ICF_SET_SER_PRM_CW_N_CND_ALERT_TIMER;
	p_payload->cw_n_cnd_alert_timer = 4194304000; /*0xFA000000*/
#endif

	VOICEAPP_PRINTF("  ---> voiceApp to IPTK, service param...\n");
	VOICEAPP_PRINTF("  SipAccountId = %d \n",SipAccountId);
	VOICEAPP_PRINTF("       call forward, uncondition, to:%s\n",
		p_payload->addr_for_cfu.address.addr_val.str);
	VOICEAPP_PRINTF("          no answer, to:%s Timer:%d \n",
		p_payload->addr_for_cfd.address.addr_val.str ,p_payload->cnd_alert_timer);
	VOICEAPP_PRINTF("          busy, to:%s\n",
		p_payload->addr_for_cfb.address.addr_val.str);
	VOICEAPP_PRINTF("    bitmask:%08x, serv sub flag:%x, act flag:%x\n",
		p_payload->bit_mask, 
		p_payload->service_subscription_flag.status,
		p_payload->service_activation_flag.status);
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_SET_SERVICE_PARAMS_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}


#if 1	/*Jason ,[SPRID # 090622474]*/		
/*******************************************************************************
 *
 * FUNCTION NAME : setServiceParamForOneShotEvent
 *
 * DESCRIPTION   : The function is to fill service-related parameters to be send towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool setServiceParamForOneShotEvent(uint16 VpItr, uint16 SipAccountId , uint16 dialFlag) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_set_service_params_req_st *p_payload = NULL;
	char sipUrl[ICF_MAX_STR_LEN];
	bool bCallWaiting = 0;
	VOICEAPP_PRINTF("\n voiceAppSetServiceParamForOneShotEvent > ENTER \n");

	int LineItr = 0;
	VOICEAPP_DBG1(" setServiceParamForOneShotEvent> ENTER , SipAccountId %d\n", SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);

	VOICEAPP_DBG1("voiceApp -> IPTK : SET_SERVICE_PARAMS_REQ (SipAccountId ", SipAccountId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_service_params_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : SET_SERVICE_PARAMS_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_set_service_params_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Contains the presence information of optional fields in this
	  * structure, bitwise.
	  */
	p_payload->bit_mask=0;

	p_payload->bit_mask |= ICF_SET_SER_PRM_SUBSCRIPTION_FLAG ;
	p_payload->bit_mask |= ICF_SET_SER_PRM_ACTIVATION_FLAG ;
	p_payload->service_subscription_flag.line_id = SipAccountId;
	p_payload->service_activation_flag.line_id = SipAccountId;
	
	p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_CALL_HOLD_RESUME;
	p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_HOLD_RESUME;

	/*check the call waiting feature */
	if(pLineFeature->callWaitingEnable){
		bCallWaiting = TRUE;
	}else{
		bCallWaiting = FALSE;
	}
	/* check the on shot call waiting */
	if(dialFlag & DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_ENABLE){
		bCallWaiting = TRUE;
	}else if(dialFlag & DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_DISABLE){
		bCallWaiting = FALSE;
	}
	if(bCallWaiting) 
    {
		p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_CALL_WAITING;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_WAITING;
#if 1 /*Jason , [RD Internal] #340 */
		p_payload->bit_mask |= ICF_SET_SER_PRM_CW_ALERT_TIMER;
		p_payload->cw_alert_timer = 4194304000;/*0xFA000000*/
#endif
	}	
	if(pLineFeature->callTransferEnable) {
		p_payload->service_subscription_flag.status |=ICF_SERVICE_SUBS_CALL_TRF_ATTENDED;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_TRF_ATTENDED;
		p_payload->service_subscription_flag.status |=ICF_SERVICE_SUBS_CALL_TRF_UNATTENDED;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_TRF_UNATTENDED;
		p_payload->service_subscription_flag.status |=ICF_SERVICE_SUBS_CALL_TRF_FROM_NW;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_TRF_FROM_NW;
		p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_INVITE_WITH_REPLACE;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_INVITE_WITH_REPLACE;

		p_payload->bit_mask |= ICF_SET_SER_PRM_CONF_REQD_AT_TRANFEREE;
		p_payload->conf_cfm_reqd_at_transferee = ICF_FALSE/*FALSE*/;
	}
	if(pLineFeature->callForwardOnBusyEnable) {
		p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_CALL_FWD_BUSY;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_FWD_BUSY;
	}
	if(pLineFeature->callForwardOnNoAnswerEnable) {
		p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_CALL_FWD_NO_ANSWER;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER;
	}
	if(pLineFeature->callForwardUnconditionalEnable) {
		p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_CALL_FWD_UNCONDITIONAL;
		p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL;
	}
	/* Conference Call settings */
	p_payload->service_subscription_flag.status |= ICF_SERVICE_SUBS_THREE_WAY_CALL;
	p_payload->service_activation_flag.status |= ICF_SERVICE_ACT_THREE_WAY_CALL;
#ifdef IPTK_REL_7_1
	p_payload->bit_mask |= ICF_SET_SER_PRM_CONF_FAIL_CALL_CLEAR_NOT_REQD;
	p_payload->conf_fail_call_clear_not_reqd = ICF_TRUE;
#endif

	p_payload->bit_mask |= ICF_SET_SER_PRM_MAX_CALL_WAITING;
	p_payload->max_num_of_calls_in_call_waiting = 2;

	/* Call Forward settings */
if(pLineFeature->callForwardUnconditionalEnable) {	
	p_payload->bit_mask |= ICF_SET_SER_PRM_CFU_ADDR;
	p_payload->addr_for_cfu.line_id = SipAccountId;
	p_payload->addr_for_cfu.address.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
	sprintf(sipUrl, "%s@%s:%d", pLineFeature->callForwardUnconditionalNumber, pProfSip->userAgentDomain, pProfSip->proxyServerPort);
	zyStrcpy(p_payload->addr_for_cfu.address.addr_val.str, (char *)sipUrl);
	p_payload->addr_for_cfu.address.addr_val.str_len = zyStrlen((char *)sipUrl);

}	

if(pLineFeature->callForwardOnBusyEnable) {	
	p_payload->bit_mask |= ICF_SET_SER_PRM_CFB_ADDR;
	p_payload->addr_for_cfb.line_id = SipAccountId;
	p_payload->addr_for_cfb.address.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
	sprintf(sipUrl, "%s@%s:%d", pLineFeature->callForwardOnBusyNumber, pProfSip->userAgentDomain, pProfSip->proxyServerPort);
	zyStrcpy(p_payload->addr_for_cfb.address.addr_val.str, (char *)sipUrl);
	p_payload->addr_for_cfb.address.addr_val.str_len = zyStrlen((char *)sipUrl);

}

if(pLineFeature->callForwardOnNoAnswerEnable) {	
	p_payload->bit_mask |= ICF_SET_SER_PRM_CFD_ADDR;
	p_payload->addr_for_cfd.line_id = SipAccountId;
	p_payload->addr_for_cfd.address.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
	sprintf(sipUrl, "%s@%s:%d", pLineFeature->callForwardOnNoAnswerNumber, pProfSip->userAgentDomain, pProfSip->proxyServerPort);
	zyStrcpy(p_payload->addr_for_cfd.address.addr_val.str, (char *)sipUrl);
	p_payload->addr_for_cfd.address.addr_val.str_len = zyStrlen((char *)sipUrl);
	
}	

#if 1
	/*cnd_alert_timer:(ms)*/
	p_payload->bit_mask |= ICF_SET_SER_PRM_CND_ALERT_TIMER;
	p_payload->cnd_alert_timer = pLineFeature->callForwardOnNoAnswerRingCount*1000;
#endif	
#if 1 /*Jason , [RD Internal] #340 */
	p_payload->bit_mask |= ICF_SET_SER_PRM_CW_N_CND_ALERT_TIMER;
	p_payload->cw_n_cnd_alert_timer = 4194304000; /*0xFA000000*/
#endif

	VOICEAPP_PRINTF("  ---> voiceApp to IPTK, service param...\n");
	VOICEAPP_PRINTF("  SipAccountId = %d \n",SipAccountId);
	VOICEAPP_PRINTF("       call forward, uncondition, to:%s\n",
		p_payload->addr_for_cfu.address.addr_val.str);
	VOICEAPP_PRINTF("          no answer, to:%s Timer:%d \n",
		p_payload->addr_for_cfd.address.addr_val.str ,p_payload->cnd_alert_timer);
	VOICEAPP_PRINTF("          busy, to:%s\n",
		p_payload->addr_for_cfb.address.addr_val.str);
	VOICEAPP_PRINTF("    bitmask:%08x, serv sub flag:%x, act flag:%x\n",
		p_payload->bit_mask, 
		p_payload->service_subscription_flag.status,
		p_payload->service_activation_flag.status);
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_SET_SERVICE_PARAMS_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : recoverServiceParamForOneShotEvent
 *
 * DESCRIPTION   : The function is to fill service-related parameters to be send towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool recoverServiceParamForOneShotEvent(uint16 SipAccountId) {

	icf_return_t ret_val;
	VOICEAPP_PRINTF("\n recoverServiceParamForOneShotEvent > ENTER\n");
	ret_val=voiceAppSetServiceParam(SipAccountId);
	oneShotEvent[SipAccountId].needRecover = 0;
	oneShotEvent[SipAccountId].OneShotEventFlag = 0;
	if(ret_val==ICF_FAILURE) {
		ZyPrintf("\n recoverServiceParamForOneShotEvent failure \n");
		return(FALSE);
	}
	return(TRUE);
}

#endif
/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSetLineParam
 *
 * DESCRIPTION   : The function is to configure parameters per line towards IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppSetLineParam(uint16 SipAccountId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_set_line_param_req_st *p_payload = NULL;

	int VpItr = 0;
	VOICEAPP_DBG1("voiceApp -> IPTK : SET_LINE_PARAM_REQ (SipAccountId ", SipAccountId);


	VpItr = mapSipAccountIdToVpItr(SipAccountId);

	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
	VoiceProfRtpObj_t* pProfRtp = tr104VoiceProfRtpObjPtrGet(VpItr);
	ZyXEL_VoiceCommon_t* pCommon = tr104VoiceCommonObjPtrGet();


	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_line_param_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : SET_LINE_PARAM_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_set_line_param_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	p_payload->line_id = SipAccountId;
	
	/* Contains the presence information of optional fields in this
	  * structure, bitwise.
	  */
	VOICEAPP_PRINTF("voip IOP flags = 0x%x\n", pCommon->voipIOPFlags);
	p_payload->bitmask=0;

	if( pCommon->voipIOPFlags & VOIP_IOP_FLAG_REMOVE_DEFAULT_PORT_IN_REQUEST_URI ){
		p_payload->bitmask |= ICF_SET_LINE_STRIP_REQ_URI_PARAM;
		p_payload->strip_req_uri_param = ICF_TRUE;
	}

	p_payload->bitmask |= ICF_SET_LINE_MIN_SE;
	p_payload->min_se = pProfSip->ZyXEL_minSE;
#if 1 
	p_payload->bitmask |= ICF_SET_LINE_SESSION_TIMER;
	p_payload->session_timer = pProfSip->inviteExpires;
#endif
	if(pCommon->voipIOPFlags & VOIP_IOP_FLAG_REMOVE_ROUTE_HEADER){
		p_payload->bitmask |= ICF_SET_LINE_REMOVE_ROUTE_HDR;
		p_payload->remove_route_hdr = TRUE;
	    VOICEAPP_PRINTF("ICF_SET_LINE_REMOVE_ROUTE_HDR on");
	}

	p_payload->bitmask |= ICF_SET_LINE_SESSION_TIMER_REFRESHER;
	p_payload->session_timer_refresher = ICF_SESSION_REFRESHER_UAC;	
#if 1 /*Jason , [SPRID # 090618210] [SPRID # 090715106]*/
		p_payload->bitmask |= ICF_SET_LINE_SIP_DSCP;
		p_payload->sip_dscp = (pProfSip->DSCPMark<<2);
		p_payload->bitmask |= ICF_SET_LINE_RTP_DSCP;
		p_payload->rtp_dscp = (pProfRtp->DSCPMark<<2);
#endif

	VOICEAPP_PRINTF("  ---> voiceApp to IPTK, line param...\n");
	VOICEAPP_PRINTF("       min_se:%d, session_timer:%d\n",
		p_payload->min_se,
		p_payload->session_timer);
	
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_SET_LINE_PARAM_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppGetSysSettings
 *
 * DESCRIPTION   : The function is to request for system settings information with IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppGetSysSettings(void) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_get_system_settings_req_st *p_payload = NULL;

	VOICEAPP_DBG("voiceApp -> IPTK : GET_SYSTEM_SETTINGS_REQ");
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_get_system_settings_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : GET_SYSTEM_SETTINGS_REQ\n");
		return(FALSE);
	}
	
	p_payload=(icf_get_system_settings_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;
	p_payload->bit_mask |= 0x3FE7FFF;
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_GET_SYSTEM_SETTINGS_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppGetLineSettings
 *
 * DESCRIPTION   : The function is to request for line settings information with IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppGetLineSettings(uint16 lineId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_get_line_settings_req_st *p_payload = NULL;

	VOICEAPP_DBG1("voiceApp -> IPTK : GET_LINE_SETTINGS_REQ (lineID ", lineId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_get_line_settings_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : GET_LINE_SETTINGS_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_get_line_settings_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	p_payload->bit_mask = 0;
	p_payload->bit_mask |= 0x3FFF;

	p_payload->line_id = lineId;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_GET_LINE_SETTINGS_REQ, ICF_INVALID_CALL_ID);
		
	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
		
	return(TRUE);
}

#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSetTrace
 *
 * DESCRIPTION   : The function is to enable or disable traces, set the trace category, criteria 
 *                           and rule for which traces need to be reported from IPTK.
 * PARAMETERS   : 
 *				(IN) setTraceFlag: It is a bit map parameter. 
 *							bit0 is for icf debug print.
 *							bit1 is for icf trace.
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppSetTrace(uint8 setTraceFlag)
{
	/*In this function we will fill the initilization data in the payload of 
	  *the ICF_SET_TRACE_REQ API structure.
	  */
	#define VOICEAPP_ICF_DBG_TRACE_MASK  0x01
	#define VOICEAPP_ICF_TRACE_MASK  0x02
	
	icf_error_t    *p_err = NULL;
	icf_msg_st			*p_msg;
	icf_return_t		ret_val;
	icf_set_trace_req_st		*p_payload;
	icf_uint8_t 				size;

	VOICEAPP_DBG("voiceApp -> IPTK : SET_TRACE_REQ");
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_set_trace_req_st),ret_val);

	size = sizeof(icf_set_trace_req_st );

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : SET_TRACE_REQ\n");
		return(FALSE);
	}
	p_payload=(icf_set_trace_req_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);

	/*Enable the reporting of traces*/
	p_payload->status = setTraceFlag & VOICEAPP_ICF_TRACE_MASK;

	/*Theses data fields are to filled if specific logs are to be generated.
	  *They change the trace type and filtering criteria.
	  *Type of traces are defined in icf_trace_error_types.h.
	  */
	/*If not specified traces of all types are reported.*/   
	p_payload->bitmask=ICF_TRACE_TYPE_PRESENT;  
	p_payload->trace_type = 0;

	p_payload->bitmask|=ICF_TRACE_CRITERIA_PRESENT;
	p_payload->trace_criteria = 0;/*ICF_TRACE_CRITERIA_UNDEFINED*/
		
	p_payload->bitmask|=ICF_TRACE_DEBUG_TRACE_STATUS_PRESENT;
	p_payload->debug_trace_status = setTraceFlag & VOICEAPP_ICF_DBG_TRACE_MASK;
	/*invoking API towards iptk */
	/* This API is sent by system administration module to enable or disable
	 * the reporting of traces of a specific level or type, during system
	 * runtime. There is no response for this message. */

	VOICEAPP_PRINTF("Set Trace request: dbgTrace/status = [%u/%u] \n",p_payload->debug_trace_status,p_payload->status);
	
	ret_val = voiceAppFillHdrSendIptk(p_msg,ICF_SET_TRACE_REQ,ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("SET_TRACE_REQ FAIL!!\n");		
		return(FALSE);
	}

	return(TRUE);
}
#endif

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppGetRegisterStatus
 *
 * DESCRIPTION   : The function is to request status of registration information for a particular line
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppGetRegisterStatus(uint16 SipAccountId) {
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_get_register_status_req_st *p_payload = NULL;

	VOICEAPP_DBG1("voiceApp -> IPTK : GET_REGISTER_STATUS_REQ (SipAccountId", SipAccountId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_get_register_status_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : GET_REGISTER_STATUS_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_get_register_status_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	p_payload->line_id = SipAccountId;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_GET_REGISTER_STATUS_REQ, ICF_INVALID_CALL_ID);
		
	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : registerStatusTimer
 *
 * DESCRIPTION   : The function is to timer to check registration status
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
//Ssu-Ying, support Multiple Profile
int registerStatusTimer(tm_id_t tm,void * vp){
	int SipAccountId = 0,LineItr = 0, VpItr = 0 ,NumOfLine = 0, NumofProfile = 0;
	int i=0 , timercontinue= 0;
	VoiceProfLineObj_t *lineObj = NULL;	



	VOICEAPP_PRINTF("registerStatusTimer > ENTER \n");

	NumofProfile = tr104GetNumProfile();
	for (VpItr = 0; VpItr < NumofProfile ; VpItr++) {
		NumOfLine = tr104GetVPNumOfLine(VpItr);
		for (LineItr = 0; LineItr < NumOfLine ; LineItr++) {
			lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
			SipAccountId = mapVpItrLineItrToSipAccountId(VpItr, LineItr);
			if(NULL != lineObj){
				if(TR104I_STATUS_UP == lineObj->status){
					VOICEAPP_DBG1("registerStatusTimer, register status Up, stop check timer, SipAccountId ", SipAccountId);
					reg_check_cnt[SipAccountId] = 0;
				}
			#if 1 /*Jason , 090825486*/	
				else if(TR104I_STATUS_IDLE == lineObj->status){
			#else
				else if(TR104I_STATUS_QUIECCENT == lineObj->status){
			#endif
					VOICEAPP_DBG1("registerStatusTimer, register status IDLE, stop check timer, SipAccountId ", SipAccountId);
					reg_check_cnt[SipAccountId] = 0;
				}
				else if(TR104I_STATUS_UNREGISTERED == lineObj->status){
					VOICEAPP_DBG1("registerStatusTimer, register status UNREGISTERED, stop check timer, SipAccountId ", SipAccountId);
					reg_check_cnt[SipAccountId] = 0;
				}
				else if(TR104I_STATUS_DISABLED== lineObj->status){
					VOICEAPP_DBG1("registerStatusTimer, register status Quieccent, stop check timer, SipAccountId ", SipAccountId);
					reg_check_cnt[SipAccountId] = 0;
				}
				else{
					reg_check_cnt[SipAccountId]++;
					VOICEAPP_PRINTF("reg_check_cnt = %d , SipAccountId = %d , lineObj->status = %d\n", reg_check_cnt[SipAccountId] , SipAccountId , lineObj->status);
					if(reg_check_cnt[SipAccountId] <= MAX_REGISTER_STATUS_CHECK_RETRY){
						timercontinue = 1;
						VOICEAPP_PRINTF("timercontinue = %d\n", timercontinue);
						voiceAppGetRegisterStatus(SipAccountId);
					}
				}
			}
		}
	}

	if(!timercontinue) {
		VOICEAPP_PRINTF("%s,reg_status_timer Stop (%d)\n",__FUNCTION__,reg_status_timer);
		timerPeriodicStop(reg_status_timer);
		reg_status_timer = 0;
	}

	return(TRUE);
}
	
/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppStartCheckRegisterStatus
 *
 * DESCRIPTION   : Send ICF_REGISTER_REQ towards IPTK to initiate registration procedure
 *                           on a line for a public-URI
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
//Ssu-Ying, support Multiple Profile
void voiceAppStartCheckRegisterStatus(void)
{
	VOICEAPP_PRINTF("start register status check timer\n");
	voiceAppIPTKAcquireMutex();
	if(reg_status_timer == 0){
		reg_status_timer = timerPeriodicStartS(2000, registerStatusTimer, NULL);
		VOICEAPP_PRINTF("%s,reg_status_timer Star, reg_status_timer = %d \n",__FUNCTION__,reg_status_timer);		
	}else{
		VOICEAPP_PRINTF("%s,Timer exist, reg_status_timer = %d \n",__FUNCTION__,reg_status_timer);
	}
	
	voiceAppIPTKReleaseMutex();

}


/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSendRegister
 *
 * DESCRIPTION   : Send ICF_REGISTER_REQ towards IPTK to initiate registration procedure
 *                           on a line for a public-URI
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppSendRegister(uint16 SipAccountId)
{
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg=NULL;
	icf_return_t ret_val;
	icf_register_req_st  *p_payload=NULL;
	VoiceProfLineObj_t *lineObj = NULL;

	int LineItr = 0, VpItr=0;
	VOICEAPP_DBG1("voiceApp -> IPTK : ICF_REGISTER_REQ (SipAccountId ", SipAccountId);

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);

	char sipUrl[ICF_MAX_STR_LEN]={0};
	VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineSipObj_t* pLineSip = tr104VoiceLineSipObjPtrGet(VpItr, LineItr);


	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_register_req_st),ret_val);

	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : REGISTER_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_register_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;
	p_payload->bit_mask |= ICF_REG_REQ_EXPIRY_PRESENT;
#ifdef NORWAY_VOIP_CUSTOMIZATION
	p_payload->bit_mask |= ICF_REG_REQ_DEREG_SPECIFIC_CONTACT_PRESENT;
	p_payload->dereg_specific_contact = ICF_TRUE;
#endif
	p_payload->line_id = SipAccountId;

	/* This parameter specifies the expiry to be sent in the
	* SIP REGISTER message. If application does not specify this
	* parameter, then previously configured value
	* (using Api ICF_SET_REGISTRATION_REQ) will be used.
	* This parameter is mandatory in case the application needs to 
	* de-register. In this case, the expiry must be set to 0. For 
	* registration, specify a large non-zero value. 
	* The unit of expiry duration is in milli seconds.
	*/
	p_payload->expiry = 1000*pProfSip->registerExpires;


	p_payload->bit_mask |= ICF_REG_REQ_TO_PRESENT ;
	if(pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_SIP_URI ||
		 (pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_SIPS_URI) ){
	    sprintf(sipUrl, "%s@%s", pLine->directoryNumber, pProfSip->userAgentDomain);
	}else if(pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_TEL_URI){
		sprintf(sipUrl, "+%s", pLine->directoryNumber);
	}
	VOICEAPP_PRINTF("sipUrl:%s\n", sipUrl);
	zyStrcpy((char *)((icf_string_st *)p_payload->to.addr_val.str), sipUrl);
	p_payload->to.addr_val.str_len = zyStrlen(sipUrl);
	p_payload->to.addr_type = pLineSip->ZyXEL_url_type;

	VOICEAPP_PRINTF("voiceAppSendRegister(): sipUrl = %s\n", sipUrl);
	VOICEAPP_PRINTF("voiceAppSendRegister(): p_payload->to.addr_val.str = %s\n", p_payload->to.addr_val.str);
	VOICEAPP_PRINTF("  ---> voiceApp to IPTK, send register...\n");
	VOICEAPP_PRINTF("       expire time:%d\n",p_payload->expiry);


	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_REGISTER_REQ,ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
		if(NULL != lineObj){
			lineObj->status = TR104I_STATUS_ERROR;
		}
		return(FALSE);
	}
	lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	if(NULL != lineObj){
		lineObj->status = TR104I_STATUS_REGISTERING;
	}
//Ssu-Ying, support Multiple Profile
	reg_check_cnt[SipAccountId] = 0;
	voiceAppStartCheckRegisterStatus();
	
	return(TRUE);
}


/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSendDeRegister
 *
 * DESCRIPTION   : Send ICF_REGISTER_REQ towards IPTK to initiate deregistration procedure
 *                           on a line for a public-URI
 *
 * RETURNS       : bool
 *
 ******************************************************************************/

bool voiceAppSendDeRegister(uint16 SipAccountId)
{
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_register_req_st  *p_payload;
	VoiceProfLineObj_t *lineObj = NULL;
	VoiceProfLineCallingFeaturesObj_t *callfeaturesObj = NULL;

	int LineItr = 0 , VpItr=0;
	VOICEAPP_DBG1("voiceApp -> IPTK : DEREGISTER_REQ (SipAccountId ", SipAccountId);

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
	char sipUrl[ICF_MAX_STR_LEN];
	VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineSipObj_t* pLineSip = tr104VoiceLineSipObjPtrGet(VpItr, LineItr);


	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_register_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : REGISTER_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_register_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;
	p_payload->bit_mask |= ICF_REG_REQ_EXPIRY_PRESENT;
	p_payload->line_id = SipAccountId;
	
	/* This parameter specifies the expiry to be sent in the
	* SIP REGISTER message. If application does not specify this
	* parameter, then previously configured value
	* (using Api ICF_SET_REGISTRATION_REQ) will be used.
	* This parameter is mandatory in case the application needs to 
	* de-register. In this case, the expiry must be set to 0. For 
	* registration, specify a large non-zero value. 
	* The unit of expiry duration is in milli seconds.
	*/
	p_payload->expiry = 0;

	p_payload->bit_mask |= ICF_REG_REQ_TO_PRESENT ;
	if(pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_SIP_URI ||
		 (pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_SIPS_URI) ){
	    sprintf(sipUrl, "%s@%s", pLine->directoryNumber, pProfSip->userAgentDomain);
	}else if(pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_TEL_URI){
		sprintf(sipUrl, "+%s", pLine->directoryNumber);
	}
	VOICEAPP_PRINTF("sipUrl:%s\n", sipUrl);
	zyStrcpy((char *)((icf_string_st *)p_payload->to.addr_val.str), sipUrl);
	p_payload->to.addr_val.str_len = zyStrlen(sipUrl);
	p_payload->to.addr_type = pLineSip->ZyXEL_url_type;

	VOICEAPP_PRINTF("voiceAppSendRegister(): sipUrl = %s\n", sipUrl);
	VOICEAPP_PRINTF("voiceAppSendRegister(): p_payload->to.addr_val.str = %s\n", p_payload->to.addr_val.str);
	VOICEAPP_PRINTF("  ---> voiceApp to IPTK, send deregister...\n");


	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_REGISTER_REQ,ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);	
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	if(NULL != lineObj){
		lineObj->status = TR104I_STATUS_UNREGISTERING;
	}

	callfeaturesObj = lineObj->callingFeatures;
	if(NULL != callfeaturesObj) {
		callfeaturesObj->messageWaiting = FALSE;
	}
	setNewVoiceMessage(SipAccountId, FALSE);

//Ssu-Ying, support Multiple Profile
	reg_check_cnt[SipAccountId] = 0;
	voiceAppStartCheckRegisterStatus();
	
	return(TRUE);
}

#if 1 /*Eason, replace special character*/
/*******************************************************************************
 *
 * FUNCTION NAME : dialDigitsReplaceChar
 *
 * DESCRIPTION   : Replace dial digit # to %23
 *                          
 *
 * RETURNS       : bool
 *
 ******************************************************************************/

bool dialDigitsReplaceChar(char *dial_digits, char *buf, int len){
	char *p = buf;
	int i=0;
	for(i=0;i<strlen(dial_digits);i++){
		if((p-buf)>=len){ /*out of range*/
			buf[len-1]=0;
			return FALSE;
		}
		
		switch(dial_digits[i]){
			case '#':
				p += sprintf(p, "%%23");
				break;
			default:
				*p=dial_digits[i];
				p++;
				break;
		}		
	}
	*p=0;
	VOICEAPP_PRINTF("%s, replced dial_digits = %s\n", __FUNCTION__, buf);

	return TRUE;

}
#endif

/*Jason ,[SPRID # 090622474]*/	
/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppCreateCallwithCIDflag
 *
 * DESCRIPTION   : Send ICF_CREATE_CALL_REQ towards IPTK to initiate a SIP 'INVITE'
 *                           message towards the remote user. (with caller id hidden flag)
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppCreateCallwithDialflag(uint16 callId, uint16 SipAccountId, uint16 VpItr, char *dial_digits,uint16 dialFlag)
{
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_create_call_req_st  *p_payload;
	char sipUrl[ICF_MAX_STR_LEN];	
	bool	cidflag=0;
	VOICEAPP_DBG1(" voiceAppCreateCallwithDialflag> ENTER , SipAccountId %d\n", SipAccountId);	
    #if 1 /*Eason, replace special character*/
	char replaced_dial_digits[ICF_MAX_STR_LEN]={0};
	ZyXEL_VoiceCommon_t* pCommon = tr104VoiceCommonObjPtrGet();
	#endif

#ifdef NORWAY_VOIP_CUSTOMIZATION
	char *tmpStr2 = NULL;
#endif	
	
	int LineItr = 0;


	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfLineSipObj_t* pLineSip = tr104VoiceLineSipObjPtrGet(VpItr, LineItr);
	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);

#if 1 /*Eason, replace special character*/
	if(pCommon->voipIOPFlags & VOIP_IOP_FLAG_REPLACE_DIGIT){
		if(dialDigitsReplaceChar(dial_digits, replaced_dial_digits, ICF_MAX_STR_LEN)){
			dial_digits = replaced_dial_digits;
		}
		else{

			VOICEAPP_PRINTF("Fail to replace dial digits special characters\n");
		}
	}

#endif


	VOICEAPP_DBG2("voiceApp -> IPTK : CREATE_CALL_REQ (SipAccountId  ", SipAccountId, ", callID ", callId);
	//check calleri id hidden/display
	if(pLineFeature->anonymousCallEnable) {
#ifdef NORWAY_VOIP_CUSTOMIZATION
		cidflag =(dialFlag & DIAL_DIGITAL_ONE_SHOT_CID_DISPLAY)?0:1;
#else		
		cidflag =(dialFlag & DIAL_DIGITAL_ONE_SHOT_CID_DISPLAY)?1:0;						
#endif		
	}else{
		cidflag =(dialFlag & DIAL_DIGITAL_ONE_SHOT_CID_HIDDEN)?1:0;
	}
	VOICEAPP_PRINTF("\n !!! cidflag is ",cidflag);

#ifdef NORWAY_VOIP_CUSTOMIZATION
	if(cidflag){
		tmpStr2 = (char *)zyMalloc(ICF_MAX_STR_LEN);
		zyMemset(tmpStr2, 0, ICF_MAX_STR_LEN);
		sprintf(tmpStr2,"*31*%s", dial_digits);

		zyMemset(dial_digits, 0, ICF_MAX_STR_LEN);
		zyMemcpy(dial_digits, tmpStr2, zyStrlen(tmpStr2));

		zyFree(tmpStr2);
	}
#endif	

	if((dialFlag&DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_ENABLE)
		||(dialFlag&DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_DISABLE))
	{
		ZyPrintf("\n haveOneShotEvent \n");
		oneShotEvent[SipAccountId].needRecover = 1;
		oneShotEvent[SipAccountId].OneShotEventFlag = dialFlag;
		ZyPrintf("\n OneShotEventFlag = %x \n",dialFlag);
		setServiceParamForOneShotEvent(VpItr,SipAccountId,dialFlag);
	}

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_create_call_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : CREATE_CALL_REQ\n");
		return(FALSE);
	}

	/* This API is sent by the application to inform IP PHONE TOOLKIT
	* about the call initiated by the local user. After receiving 
	* this API, IPTK can initiate a SIP ?˜INVITE??message towards 
	* the remote user
	*/
     
	p_payload=(icf_create_call_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */

	p_payload->line_id = SipAccountId;

	p_payload->bit_mask |= ICF_HDR_LIST_PRESENT;
#ifndef NORWAY_VOIP_CUSTOMIZATION
	p_payload->bit_mask |= ICF_TOGGLE_PRES_IND_PRESENT;
#endif
	p_payload->bit_mask |= 0x00000080;
	p_payload->bit_mask |= 0x01000000;

	//p_payload->bit_mask|=ICF_PRIVACY_HDR_FLAG_PRESENT;/* BYE ,ReInvite support privacy*/
	//p_payload->privacy_hdr_all_req=1;
	

	/* Fill the called party address here */

	if(pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_SIP_URI || 
		pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_SIPS_URI ){
		if((pCommon->voipIOPFlags & VOIP_IOP_FLAG_REMOVE_DEFAULT_PORT_IN_REQUEST_URI) &&
			(pProfSip->proxyServerPort == 5060)){			
			snprintf(sipUrl, ICF_MAX_STR_LEN-1, "%s@%s", dial_digits, pProfSip->userAgentDomain);
		}
		else{
			snprintf(sipUrl, ICF_MAX_STR_LEN-1, "%s@%s:%d", dial_digits, pProfSip->userAgentDomain, pProfSip->proxyServerPort);
		}
	}else if(pLineSip->ZyXEL_url_type == ICF_ADDRESS_PLAN_TEL_URI){
		snprintf(sipUrl, ICF_MAX_STR_LEN-1, "+%s", dial_digits);
	}
	VOICEAPP_PRINTF("sipUrl:%s\n", sipUrl);
	zyStrcpy((char *)((icf_string_st *)p_payload->called_party_addr.addr_val.str), sipUrl);
	p_payload->called_party_addr.addr_val.str_len = zyStrlen(sipUrl);
	p_payload->called_party_addr.addr_type = pLineSip->ZyXEL_url_type;

#ifndef NORWAY_VOIP_CUSTOMIZATION	
	//Eric.chung: change the default caller id display (Support one shot caller id dislplay/hidden
	// toggle_presentation_indicator is 0 , follow web configure
	// toggle_presentation_indicator is 1 , inverse web configure (ex:if web is 1 , use 0 , if web is 0 ,use 1)
	// ZyPrintf("\r\n set toggle_presentation_indicator is %d",cidflag);
	p_payload->toggle_presentation_indicator=cidflag;
#endif


	/*rewrite by Mike,
	*if you wanna add a header,add the function addHeaderList(p_payload,"","")
	*it use link_list to generate the header
	*/
	if( 0 > addHeaderList(p_payload, "Accept", "application/dtmf-relay") ) {
		icf_free_decoded_msg(p_msg,p_err);
		return(FALSE);
	}
	if( 0 > addHeaderList(p_payload, "Accept", "application/sdp") ) {
		icf_free_decoded_msg(p_msg,p_err);
		return(FALSE);
	}


	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CREATE_CALL_REQ,callId);
	/*Jason , [SPRID # 090909666] */
	zyStrncpy(webSipStatus[SipAccountId].lastOutgoingNumber, dial_digits, WEB_SIP_STATUS_PHONENUMB_LENGTH);	
	webSipStatus[SipAccountId].lastOutgoingNumber[WEB_SIP_STATUS_PHONENUMB_LENGTH-1]=0;// make last char null	
	VOICEAPP_PRINTF("webSipStatus SipAccountId %d, lastOutgoingNumber = %s\n", SipAccountId, webSipStatus[SipAccountId].lastOutgoingNumber);
	icf_free_decoded_msg(p_msg,p_err);


	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}




/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppLocalAlerted
 *
 * DESCRIPTION   : Send ICF_LOCAL_USER_ALERTED_REQ towards IPTK to initiate 180 SIP 
 *                           message towards the remote user.
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppLocalAlerted(uint16 callId, uint16 SipAccountId, uint16 earlyMediaSuccess)
{
	icf_error_t    *p_err = NULL;

	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_local_user_alerted_req_st  *p_payload;

	/* This API is invoked by the application towards the IP Phone Toolkit
	* in response to the ICF_INCOMING_CALL_IND API. This API indicates 
	* that the application has alerted the local user about the incoming 
	* call. The IPTK initiates 180 SIP message towards the remote user.
	* The application can also invoke this API in response to the 
	* ICF_INCOMING_CALL_MODIFY_IND from IP Phone Toolkit. This feature 
	* is supported for POC application (to add users to an already 
	* established session).
	*/


	int LineItr = 0, VpItr=0;

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfLineCallingFeaturesObj_t* pLineCallingFeatures = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);	/* aircheng modify*/

	VOICEAPP_DBG1("voiceApp -> IPTK : LOCAL_USER_ALERTED_REQ (callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_local_user_alerted_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : LOCAL_USER_ALERTED_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_local_user_alerted_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;

	if(pLineCallingFeatures->ZyXEL_remoteRingbackTone == TR104I_REMOTE_RINGBACK_ACTIVE && earlyMediaSuccess){	/* aircheng modify*/
		p_payload->early_media_reqd = ICF_TRUE;
	} else {
		p_payload->early_media_reqd = ICF_FALSE;
	}

	/* Informs IP Phone Toolkit whether the application or the remote user 
	* should play the ring back tone. This parameter is used by IP Phone
	* Toolkit only if the INVITE received from the network contains the SDP.
	* If the INVITE does not contain SDP, IP Phone Toolkit assumes that 
	* the inband info is not present.
	*/
	if(pLineCallingFeatures->ZyXEL_remoteRingbackTone == TR104I_REMOTE_RINGBACK_ACTIVE && earlyMediaSuccess){	/* aircheng modify*/
		p_payload->inband_info_status = ICF_TRUE;
	} else {
		/* we are not playing the ring back to the n/w */
		p_payload->inband_info_status = ICF_FALSE;
	}
#if 0 /* ZyXEL Jacky, turn off P-Early-Media header support */
	if(pLineSip->ZyXEL_remoteRingbackTone == TR104I_REMOTE_RINGBACK_ACTIVE){
		p_payload->bit_mask = ICF_USER_ALERTED_EARLY_MEDIA_SUPPORTED;
		p_payload->early_mode = ICF_MEDIA_MODE_SEND_RECV;
	}
#endif
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_LOCAL_USER_ALERTED_REQ,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppMediaConnect
 *
 * DESCRIPTION   : Send ICF_MEDIA_CONNECT_REQ towards IPTK to initiate 183 SIP 
 *                           message towards the remote user.
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppMediaConnect(uint16 callId)
{
	icf_error_t    *p_err = NULL;

	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_media_connect_req_st  *p_payload;

	VOICEAPP_DBG1("voiceApp -> IPTK : MEDIA_CONNECT_REQ (callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_media_connect_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : MEDIA_CONNECT_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_media_connect_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;

	p_payload->bit_mask |= ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT;
	p_payload->media_mode = ICF_MEDIA_MODE_SEND_RECV;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_MEDIA_CONNECT_REQ,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppTerminateCall
 *
 * DESCRIPTION   : Send ICF_TERMINATE_CALL_REQ towards IPTK to initiate 
 *                           BYE/CANCEL /4xx/5xx/6xx SIP message towards the remote user
 *                           dependion the call state
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
#if 1 /*Jennifer, Anonymous Call Block*/
bool voiceAppTerminateCall(uint16 callId, uint16 lineId, icf_call_clear_reason_t call_clear_reason, uint16 respcode)
#else
bool voiceAppTerminateCall(uint16 callId, uint16 lineId, icf_call_clear_reason_t call_clear_reason)
#endif
{
	icf_error_t    *p_err = NULL;

	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_terminate_call_req_st  *p_payload;

	/* This API is invoked by the application towards IPTK when 
	* the local user rejects an incoming call or terminates an 
	* ongoing call. This API is also used to indicate any internal
	* failure in the application. This results in a BYE/CANCEL 
	* /4xx/5xx/6xx SIP message towards the remote user depending 
	* on the call state. Any time during an on-going call, this 
	* API will result in a SIP BYE message. If the user has 
	* initiated the call and this API is used before the final 
	* response has arrived, then this will result in a SIP 
	* CANCEL message towards the network.
	*/

	VOICEAPP_DBG2("voiceApp -> IPTK : TERMINATE_CALL_REQ (lineID ", lineId, ", callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_terminate_call_req_st),ret_val);

	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : TERMINATE_CALL_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_terminate_call_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

#if 1 /*Jennifer, Anonymous Call Block*/
	if ((call_clear_reason == ICF_CALL_CLEAR_REASON_UNDEFINED)&& respcode){
		/*anonymousCallBlock-reject anonymous call-433 anonymity disallowed, respcode == 433.*/
		p_payload->bit_mask |= ICF_CALL_CLEAR_RESPONSE_CODE_PRESENT;
		p_payload->response_code = respcode;
	}else{
#endif
	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;
#if 1 /*Jennifer, Anonymous Call Block*/
	}
#endif
	/* fill the call clear reason .
	* Indicates the reason for terminating the call. 
	* IPTK can use this reason to send the appropriate 
	* SIP message to the remote party.*/
	p_payload->call_clear_reason = call_clear_reason;

	p_payload->line_id = lineId;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_TERMINATE_CALL_REQ,callId);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE){
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppTermCallResp
 *
 * DESCRIPTION   : Send ICF_LOCAL_USER_TERMINATE_CALL_RESP towards IPTK in response 
 *                           to the ICF_CALL_TERMINATE_IND API sent by IPTK
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppTermCallResp(uint16 callId) {
	icf_msg_st  *p_msg = NULL;
	icf_return_t ret_val;
	icf_error_t    *p_err = NULL;

	/* This API is invoked by IP Phone Toolkit Module towards the 
	* application when SIP BYE/CANCEL/4xx/5xx/6xx request from 
	* remote user is received or call is released due to some local error
	*/

	VOICEAPP_DBG1("voiceApp -> IPTK : LOCAL_USER_TERMINATE_CALL_RESP (callID ", callId);
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, 0,ret_val);	
	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : LOCAL_USER_TERMINATE_CALL_RESP\n");
		return(FALSE);
	}
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_LOCAL_USER_TERMINATE_CALL_RESP,callId);
	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppConnect
 *
 * DESCRIPTION   : Send ICF_CONNECT_REQ towards IPTK to send a SIP 200 OK message 
 *                           towards the network
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppConnect(uint16 callId)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_connect_req_st  *p_payload;
	icf_error_t    *p_err = NULL;

	/* This API is invoked by the application to IPTK when the local 
	* user accepts the call. On receiving this API, IP Phone Toolkit 
	* sends a SIP 200 OK message towards the network. This message 
	* is sent in response to the INVITE message received earlier for
	* an incoming call.
	*/

	VOICEAPP_DBG1("voiceApp -> IPTK : CONNECT_REQ (callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_connect_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : CONNECT_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_connect_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CONNECT_REQ,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSubscribe
 *
 * DESCRIPTION   : Send ICF_SUBSCRIBE_REQ towards IPTK to send a SIP SUBSCRIBE request 
 *                           towards the network
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppSubscribe(uint16 SipAccountId) {
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_subscribe_req_st *p_payload = NULL;
	char sipUrl[ICF_MAX_STR_LEN];
	icf_error_t    *p_err = NULL;


	int LineItr = 0, VpItr=0;
	VOICEAPP_DBG1("voiceApp -> IPTK : SUBSCRIBE_REQ (SipAccountId ", SipAccountId);

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	
	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
	VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineSipObj_t* pLineSip = tr104VoiceLineSipObjPtrGet(VpItr, LineItr);
	VoiceProfLineCallingFeaturesObj_t *featureObj = NULL;


	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_subscribe_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : SUBSCRIBE_REQ\n");
		return(FALSE);
	}
	if(pProfSip==NULL){
		ZyPrintf("ERROR: pProfSip is NULL!\n");
		return(FALSE);
	}
	if(pLine == NULL){
		ZyPrintf("ERROR: pLine is NULL!\n");
		return(FALSE);
	}else if(pLine->callingFeatures == NULL){
		ZyPrintf("ERROR: pLine->callingFeatures is NULL!\n");
		return(FALSE);
	}else{
		featureObj = pLine->callingFeatures;
	}
	
	if(pLineSip == NULL){
		ZyPrintf("ERROR: pLineSip is NULL!\n");
		return(FALSE);
	}

	p_payload=(icf_subscribe_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Contains the presence information of optional fields in this
	  * structure, bitwise.
	  */
	p_payload->bit_mask=0;
	p_payload->line_id = SipAccountId;
	
	p_payload->bit_mask |= ICF_SUB_REQ_DEST_INFO_PRESENT;
	sprintf(sipUrl, "%s@%s:%d", pLine->directoryNumber, pProfSip->userAgentDomain, pProfSip->proxyServerPort);
	p_payload->destination_info.addr_type = pLineSip->ZyXEL_url_type;
	zyStrcpy((char *)p_payload->destination_info.addr_val.str, (char *)sipUrl);
	p_payload->destination_info.addr_val.str_len = zyStrlen((char *)sipUrl);
	
	p_payload->bit_mask |= ICF_SUB_REQ_EVENT_PRESENT;
	zyStrncpy((char *)p_payload->event_pkg_name.str,(char *)"message-summary",zyStrlen((char *)"message-summary")+1);
	p_payload->event_pkg_name.str_len= zyStrlen((char *)"message-summary");

	if(NULL != featureObj){
		p_payload->duration=featureObj->ZyXEL_mwiExpireTime;
	}else{
		ZyPrintf("ERROR: featureObj is NULL!");
	}

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_SUBSCRIBE_REQ, CALL_ID_SUBSCRIBE);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppUnSubscribe
 *
 * DESCRIPTION   : Send ICF_SUBSCRIBE_REQ towards IPTK to send a SIP SUBSCRIBE request 
 *                           with duration 0 towards the network
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppUnSubscribe(uint16 SipAccountId) {
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_subscribe_req_st *p_payload = NULL;
	char sipUrl[ICF_MAX_STR_LEN];
	icf_error_t    *p_err = NULL;

	int LineItr = 0, VpItr=0;
	VOICEAPP_DBG1("voiceApp -> IPTK : UNSUBSCRIBE_REQ (SipAccountId ", SipAccountId);

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	
	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
	VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineSipObj_t* pLineSip = tr104VoiceLineSipObjPtrGet(VpItr, LineItr);


	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_subscribe_req_st ),ret_val);

	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : SUBSCRIBE_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_subscribe_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Contains the presence information of optional fields in this
	  * structure, bitwise.
	  */
	p_payload->bit_mask=0;
	p_payload->line_id = SipAccountId;
	
	p_payload->bit_mask |= ICF_SUB_REQ_DEST_INFO_PRESENT;
	sprintf(sipUrl, "%s@%s:%d", pLine->directoryNumber, pProfSip->userAgentDomain, pProfSip->proxyServerPort);
	p_payload->destination_info.addr_type = pLineSip->ZyXEL_url_type;
	zyStrcpy((char *)p_payload->destination_info.addr_val.str, (char *)sipUrl);
	p_payload->destination_info.addr_val.str_len = zyStrlen((char *)sipUrl);
	
	p_payload->bit_mask |= ICF_SUB_REQ_EVENT_PRESENT;
	zyStrncpy((char *)p_payload->event_pkg_name.str,(char *)"message-summary",zyStrlen((char *)"message-summary")+1);
	p_payload->event_pkg_name.str_len= zyStrlen((char *)"message-summary");

	p_payload->duration = 0;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_SUBSCRIBE_REQ, CALL_ID_SUBSCRIBE);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppStartFAX
 *
 * DESCRIPTION   : Send ICF_START_FAX_REQ towards IPTK to generate a Re-INVITE request
 *                           on the network to initiate a FAX session
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppStartFAX(uint16 callId, uint16 SipAccountId, uint8 fax_type)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_start_fax_req_st  *p_payload;
	icf_error_t    *p_err = NULL;
	int LineItr = 0, VpItr=0;
	
	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VoiceProfObj_t* pProfSip = tr104VoiceProfObjPtrGet(VpItr);

	VOICEAPP_DBG2("voiceApp -> IPTK : START_FAX_REQ (callID ", callId, ", fax_type ", fax_type);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_start_fax_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : START_FAX_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_start_fax_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bitmask = 0;

	p_payload->fax_type = fax_type;
	if((NULL != pProfSip) && pProfSip->ZyXEL_faxPassThroughPCMA){
		p_payload->fax_pt_codec = ICF_PREF_CODEC_FAX_PT_PCMA;
	}
	else{
		p_payload->fax_pt_codec = ICF_PREF_CODEC_FAX_PT_PCMU;
	}
	

	p_payload->bitmask |= ICF_START_FAX_REQ_METHOD_PRESENT;
	p_payload->fax_method = ICF_FAX_METHOD_REPLACE_STREAM;

	p_payload->bitmask |= ICF_START_FAX_REQ_PT_PARAM_PRESENT;
	p_payload->pt_param = ICF_START_FAX_PT_PARAM_SILENCE_SUPP;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_START_FAX_REQ,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppStopFAX
 *
 * DESCRIPTION   : Send ICF_STOP_FAX_REQ towards IPTK to stop the fax call establishment
 *                           or stop the ongoing FAX session
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppStopFAX(uint16 callId)
{
	icf_msg_st  *p_msg = NULL;
	icf_return_t ret_val;
	icf_error_t    *p_err = NULL;

	VOICEAPP_DBG1("voiceApp -> IPTK : STOP_FAX_REQ (callID ", callId);
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, 0, ret_val);
	if(ret_val==ICF_FAILURE) {
		ZyPrintf("MEMORY ALLOC FAIL : STOP_FAX_REQ\n");
		return(FALSE);
	}
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_STOP_FAX_REQ, callId);
	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppCallHold
 *
 * DESCRIPTION   : Send ICF_CALL_HOLD_REQ towards IPTK to put the currently ongoing call on hold
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppCallHold(uint16 callId)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_call_hold_req_st  *p_payload;
	icf_error_t    *p_err = NULL;
	VOICEAPP_DBG1("voiceApp -> IPTK : CALL_HOLD_REQ (callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_call_hold_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : CALL_HOLD_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_call_hold_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CALL_HOLD_REQ,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppCallResume
 *
 * DESCRIPTION   : Send ICF_CALL_RESUME_REQ towards IPTK to resume the call
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppCallResume(uint16 callId)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_call_resume_req_st  *p_payload;
	icf_error_t    *p_err = NULL;
	VOICEAPP_DBG1("voiceApp -> IPTK : CALL_RESUME_REQ (callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_call_resume_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val){
		ZyPrintf("MEMORY ALLOC FAIL : CALL_RESUME_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_call_resume_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CALL_RESUME_REQ,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppCallModifyCfm
 *
 * DESCRIPTION   : Send ICF_CALL_MODIFY_CFM towards IPTK to accept session updates 
 *                           and respond with 200 OK to the network
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppCallModifyCfm(uint16 callId)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_call_modify_cfm_st  *p_payload;
	icf_error_t    *p_err = NULL;
	VOICEAPP_DBG1("voiceApp -> IPTK : CALL_MODIFY_CFM (callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_call_modify_cfm_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : CALL_MODIFY_CFM\n");
		return(FALSE);
	}

	p_payload=(icf_call_modify_cfm_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;
	p_payload->result = ICF_TRUE;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CALL_MODIFY_CFM,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppInfo
 *
 * DESCRIPTION   : Send ICF_INFO_REQ towards IPTK to send a SIP INFO message
 *                           towards the network
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppInfo(uint16 callId, uint8 *dialDigits)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_info_req_st  *p_payload;
	char tmpChar[1024]={'\0'};
	icf_error_t    *p_err = NULL;
	VOICEAPP_DBG1("voiceApp -> IPTK : INFO_REQ (callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_info_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : INFO_REQ\n");
		return(FALSE);
	}

	p_payload=(icf_info_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	p_payload->bit_mask|=ICF_BODY_LIST_PRESENT;
	p_payload->body_list.count = 1;
	zyStrcpy((icf_int8_t *)p_payload->body_list.content_type.str, "application/dtmf-relay"); 
	p_payload->body_list.content_type.str_len = zyStrlen("application/dtmf-relay");
	p_payload->body_list.body = (icf_msg_body_st*)zyMalloc(sizeof(icf_msg_body_st));
	if(NULL == p_payload->body_list.body) {
		icf_free_decoded_msg(p_msg,p_err);
		return(FALSE);
	}
	p_payload->body_list.body->next = NULL;

	sprintf(tmpChar, "Signal= %s\r\nDuration= 1000\r\n", dialDigits);
	p_payload->body_list.body->value = (icf_uint8_t*)zyMalloc(zyStrlen(tmpChar));
	if(NULL == p_payload->body_list.body->value) {
		icf_free_decoded_msg(p_msg,p_err);
		return(FALSE);
	}
	zyStrcpy((icf_int8_t *)p_payload->body_list.body->value, tmpChar);
	p_payload->body_list.body->length = zyStrlen(tmpChar);
	
	zyStrcpy((icf_int8_t *)p_payload->body_list.body->content_type.str,"application/dtmf-relay"); 
	p_payload->body_list.body->content_type.str_len = zyStrlen("application/dtmf-relay");
	
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_INFO_REQ,callId);
	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppInfoCfm
 *
 * DESCRIPTION   : Send ICF_INFO_CFM towards IPTK in response to ICF_INFO_IND
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppInfoCfm(uint16 callId)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_info_cfm_st  *p_payload;
	icf_error_t    *p_err = NULL;
	VOICEAPP_DBG1("voiceApp -> IPTK : INFO_CFM (callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_info_cfm_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : INFO_CFM\n");
		return(FALSE);
	}

	p_payload=(icf_info_cfm_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask |= ICF_INFO_CFM_RESPONSE_CODE_PRESENT;
	p_payload->result = ICF_TRUE;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_INFO_CFM,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE){
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppNotifyCfm
 *
 * DESCRIPTION   : Send ICF_NOTIFY_CFM towards IPTK in response to ICF_NOTIFY_IND
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppNotifyCfm(uint16 callId)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_notify_cfm_st  *p_payload;
	icf_error_t    *p_err = NULL;
	VOICEAPP_DBG1("voiceApp -> IPTK : NOTIFY_CFM (callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_notify_cfm_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : NOTIFY_CFM\n");
		return(FALSE);
	}

	p_payload=(icf_notify_cfm_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;
	p_payload->result = ICF_TRUE;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_NOTIFY_CFM,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE){
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppUnAttendedTransfer
 *
 * DESCRIPTION   : Send ICF_CALL_TRANSFER_UNATTENDED_REQ towards IPTK to transfer an active call to a third-party
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppUnAttendedTransfer(uint16 callId, uint16 lineId, char *transfer_target_addr)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_call_transfer_unattended_req_st  *p_payload;
	icf_error_t    *p_err = NULL;	
	VOICEAPP_DBG1("voiceApp -> IPTK : ICF_CALL_TRANSFER_UNATTENDED_REQ callID : ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_call_transfer_unattended_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CALL_TRANSFER_UNATTENDED_REQ\n");
		return(FALSE);
	}

	/* This API is sent by the application to inform IP PHONE TOOLKIT
	* about the call initiated by the local user. After receiving 
	* this API, IPTK can initiate a SIP ?˜INVITE??message towards 
	* the remote user
	*/
     
	p_payload=(icf_call_transfer_unattended_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;

	/* Fill the called party address here */
	zyStrcpy((char *)((icf_string_st *)p_payload->transferred_party_addr.addr_val.str), transfer_target_addr);
	p_payload->transferred_party_addr.addr_val.str_len = zyStrlen(transfer_target_addr);
	p_payload->transferred_party_addr.addr_type = 3;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CALL_TRANSFER_UNATTENDED_REQ,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		VOICEAPP_DBG("ICF_CALL_TRANSFER_UNATTENDED_REQ, voiceAppFillHdrSendIptk fail");
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppUnAttendedTransfer
 *
 * DESCRIPTION   : Send ICF_CALL_TRANSFER_ATTENDED_REQ towards IPTK to transfer the
 *                           currently active call to remote party of the held call
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppAttendedTransfer(uint16 callIdB, uint16 callIdC)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_call_transfer_attended_req_st *p_payload;
	icf_error_t    *p_err = NULL;
	VOICEAPP_DBG2("voiceApp -> IPTK : ICF_CALL_TRANSFER_ATTENDED_REQ (callIdB = ", callIdB, ", callIdC = ", callIdC);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_call_transfer_attended_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CALL_TRANSFER_ATTENDED_REQ\n");
		return(FALSE);
	}
     
	p_payload=(icf_call_transfer_attended_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	p_payload->bit_mask = 0;
	p_payload->transferred_party_id = callIdC;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CALL_TRANSFER_ATTENDED_REQ, callIdB);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);

}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppRemoteCallTransferCfm
 *
 * DESCRIPTION   : Send ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM towards IPTK 
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppRemoteCallTransferCfm(uint16 callId)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_remote_call_transfer_initiated_cfm_st *p_payload;
	icf_error_t    *p_err = NULL;
	VOICEAPP_DBG1("voiceApp -> IPTK : ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM (callID ", callId);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_remote_call_transfer_initiated_cfm_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : CALL_MODIFY_CFM\n");
		return(FALSE);
	}

	p_payload = (icf_remote_call_transfer_initiated_cfm_st *)(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;
	p_payload->result = ICF_TRUE;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM,callId);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppConference
 *
 * DESCRIPTION   : Send ICF_CONFERENCE_REQ towards IPTK to place two existing calls on the
 *                           phone in a conference
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppConference(uint16 callIdB, uint16 callIdC)
{
	icf_msg_st  *p_msg;
	icf_return_t ret_val;
	icf_conference_req_st *p_payload;
	icf_error_t    *p_err = NULL;
	VOICEAPP_DBG2("voiceApp -> IPTK : ICF_CONFERENCE_REQ (callIdB  ", callIdB, ", callIdC ", callIdC);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_conference_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CONFERENCE_REQ\n");
		return(FALSE);
	}
     
	p_payload=(icf_conference_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));

	p_payload->bit_mask = 0;
	p_payload->merge_call_id = callIdC;

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CONFERENCE_REQ, callIdB);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

#if HEARTBEAT_MECHANISM_ENABLE
/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppSendOptions
 *
 * DESCRIPTION   : Send ICF_OPTIONS_REQ towards IPTK to send a SIP OPTIONS message
 *                 towards the network
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppSendOptions(uint16 VpItr , uint16 SipAccountId)
{ 
	icf_msg_st  *p_msg = NULL;
	icf_return_t ret_val;
	icf_options_req_st  *p_payload = NULL;
	icf_call_id_t call_id_base = HEARTBEATCALLIDBASE , call_id = 0 ; 
	int LineItr = 0;

	VOICEAPP_DBG1(" voiceAppSendOptions> ENTER , SipAccountId %d\n", SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
	VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	icf_uint8_t sipUrl[ICF_MAX_STR_LEN];
	icf_error_t    *p_err = NULL;	
	call_id = VpItr|call_id_base ;
	VOICEAPP_DBG1("voiceApp -> IPTK : OPTIONS_REQ (VpItr ", VpItr);

	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_options_req_st),ret_val);

	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : OPTIONS_REQ\n");
		return(FALSE);
	}
	if((NULL==pProfSip)||(NULL==pLine)){
		icf_free_decoded_msg(p_msg,p_err);
		return(FALSE);
	}
	p_payload=(icf_options_req_st* )(VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg));
	/* Indicates the presence of optional parameters in the API */
	p_payload->bit_mask = 0;
	p_payload->line_id = SipAccountId;
	VOICEAPP_PRINTF("p_payload->line_id = %d \r\n" , p_payload->line_id);
	p_payload->destination_info.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
	sprintf(sipUrl, "%s@%s:%d", pLine->directoryNumber, pProfSip->userAgentDomain, pProfSip->proxyServerPort);

	zyStrcpy(p_payload->destination_info.addr_val.str, (char *)sipUrl);
	p_payload->destination_info.addr_val.str_len = zyStrlen((char *)sipUrl);
	
	
	VOICEAPP_PRINTF("call_id = %x \r\n" , call_id);
	ret_val = voiceAppFillHdrSendIptk(p_msg,ICF_OPTIONS_REQ,call_id);

	icf_free_decoded_msg(p_msg,p_err);
	
	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}
#endif

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgInit
 *
 * DESCRIPTION   : The function is to set configuration through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgInit(void) {
	VOICEAPP_PRINTF("%s > ENTER \n",__FUNCTION__);

	uint32 SipAccountId = 0;

	for ( SipAccountId=0; SipAccountId<sipAccountNumber; SipAccountId++){	
		if (FALSE == mmCfgSelfIp(SipAccountId)) {
			ZyPrintf("voiceApp > ICF_CFG_SET_SELF_IP_REQ towards IP Phone Toolkit Failed.\n");
			return FALSE;
		}

		if (FALSE == mmCfgPortRange(SipAccountId)) {
			ZyPrintf("voiceApp > ICF_CFG_SET_PORT_RANGE_REQ towards IP Phone Toolkit Failed.\n");
			return FALSE;
		}

		if (FALSE == mmCfgCodecInfo(SipAccountId))
		{
		    ZyPrintf("voiceApp > ICF_CFG_CODEC_INFO_REQ towards IP Phone Toolkit Failed.\n");
		    return FALSE;
		}
	}

/* Not implement yet */
#if 0
	if (FALSE == mmCfgMedia())
	{
	    ZyPrintf("\n\tvoiceApp > ICF_CFG_MEDIA_REQ towards IP Phone Toolkit Failed. \n");
	    return FALSE;
	}

	if (FALSE == mmCfgJbmMgmt())
	{
	    ZyPrintf("\n\tvoiceApp > ICF_CFG_JBM_MGMT_REQ towards IP Phone Toolkit Failed. \n");
	    return FALSE;
	}

	if (FALSE == mmCfgMaxEarpVol())
	{
	    ZyPrintf("\n\tvoiceApp > ICF_CFG_MAX_EARP_VOL_REQ towards IP Phone Toolkit Failed. \n");
	    return FALSE;
	}

	if (FALSE == mmCfgMaxSpeakerVol())
	{
	    ZyPrintf("\n\tvoiceApp > ICF_CFG_MAX_SPEAKER_VOL_REQ towards IP Phone Toolkit Failed. \n");
	    return FALSE;
	}

	if (FALSE == mmCfgDigitTrans())
	{
	    ZyPrintf("\n\tvoiceApp > ICF_CFG_SET_DIGIT_TRANS_REQ towards IP Phone Toolkit Failed. \n");
	    return FALSE;
	}
	
	if (FALSE == mmCfgRemoteVideoDisplay())
	{
	    ZyPrintf("\n\tvoiceApp > ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ towards IP Phone Toolkit Failed. \n");
	    return FALSE;
	}

	if (FALSE == mmCfgPreviewVideoDisplay())
	{
	    ZyPrintf("\n\tvoiceApp > ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ towards IP Phone Toolkit Failed. \n");
	    return FALSE;
	}

	if (FALSE == mmCfgMaxMicGain())
	{
	    ZyPrintf("\n\tvoiceApp > ICF_CFG_MAX_MIC_GAIN_REQ towards IP Phone Toolkit Failed. \n");
	    return FALSE;
	}
#endif
	VOICEAPP_PRINTF("%s > EXIT \n",__FUNCTION__);

	return TRUE;
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgSelfIp
 *
 * DESCRIPTION   : The function to fill the self IP to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgSelfIp(uint32 SipAccountId){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val;
	icf_cfg_set_self_ip_req_st	*p_cfg_self_ip_req = ICF_NULL;
	struct      in_addr inAddr;

	int VpItr = 0;
	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	VoiceProfObj_t * pProf = tr104VoiceProfObjPtrGet(VpItr);

	VOICEAPP_PRINTF("%s -> IPTK : ICF_CFG_SET_SELF_IP_REQ\n",__FUNCTION__);
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_set_self_ip_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_SET_SELF_IP_REQ\n");
		return(FALSE);
	}
	p_cfg_self_ip_req  = (icf_cfg_set_self_ip_req_st*)(p_msg->payload);

	if(get_iface_ipv4_address(pProf->ZyXEL_ActiveBoundIfName, &inAddr) != 0) {
		ZyPrintf("voiceApp:-   GET IPADDRESS FAIL : CFG_SET_SELF_IP_REQ IfNmae %s\n", pProf->ZyXEL_ActiveBoundIfName);
		icf_free_decoded_msg(p_msg,p_err);
		return(FALSE);
	}

	p_cfg_self_ip_req->self_ip_addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
	p_cfg_self_ip_req->SipAccountId = SipAccountId; 
	zyStrcpy(p_cfg_self_ip_req->self_ip_addr.addr_val.str, inet_ntoa (inAddr));
	p_cfg_self_ip_req->self_ip_addr.addr_val.str_len = zyStrlen(p_cfg_self_ip_req->self_ip_addr.addr_val.str);

	VOICEAPP_PRINTF("ICF_CFG_SET_SELF_IP_REQ SipAccountId %d\n ", p_cfg_self_ip_req->SipAccountId);

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_SET_SELF_IP_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}


/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgCodecInfo
 *
 * DESCRIPTION   : The function to fill the icf_config_codec_attrib_st to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgCodecInfo(uint32 SipAccountId){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val = ICF_FAILURE;
	icf_cfg_codec_info_req_st	*p_cfg_codec_info_req = ICF_NULL;
	icf_config_codec_attrib_st *p_config_codec_attrib = ICF_NULL;
	icf_list_st *p_encoded_params = ICF_NULL;
	icf_string_st *str_encoded_param = ICF_NULL;
	VoiceProfLineCodecListObj_t *p_capCodecList = NULL;
	VoiceCapCodecObj_t *p_capCodec = NULL;
	int codec_count = 0, num_of_supported_ptime = 0;
	char *p_ptime_str = NULL;
	char ptime_str[64+1];
	int LineItr = 0, VpItr=0;

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);

	VOICEAPP_DBG("voiceApp -> IPTK : ICF_CFG_CODEC_INFO_REQ");
	
		VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_codec_info_req_st),ret_val);
		if(ICF_FAILURE == ret_val) {
			ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_CODEC_INFO_REQ\n");
			return(FALSE);
		}

		codec_count = 0;
		p_cfg_codec_info_req  = (icf_cfg_codec_info_req_st*)(p_msg->payload);
		p_cfg_codec_info_req->line_id = SipAccountId;

		if(NULL != (p_capCodecList = tr104GetLineCodecByCodecName(VpItr , LineItr , "G.711MuLaw")))
		{
			if(p_capCodecList->priority < ICF_MAX_CODEC_SUPPORTD)
			{
				VOICEAPP_DBG1("PCMU supported (SipAccountId ", SipAccountId);
				p_config_codec_attrib = &p_cfg_codec_info_req->codec_supp[p_capCodecList->priority-1];
				p_config_codec_attrib->enable = p_capCodecList->enable;

				p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
				zyStrcpy(p_config_codec_attrib->codec_name.str, SDP_CODEC_NAME_PCMU);
				p_config_codec_attrib->codec_name.str_len = zyStrlen(p_config_codec_attrib->codec_name.str);

				p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
				p_config_codec_attrib->codec_num = ZYNOS_CODEC_PCMU;

				p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;	

				num_of_supported_ptime = 0;
				if(NULL != (p_capCodec = tr104GetCapCodecByEntryID(p_capCodecList->entryID))){
					zyStrcpy(ptime_str, p_capCodec->packetizationPeriod);
					p_ptime_str = zyStrtok(ptime_str, ",");
					while(NULL != p_ptime_str){
						p_config_codec_attrib->codec_param.audio_param.ptime[num_of_supported_ptime] = atoi(p_ptime_str);
						p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
						num_of_supported_ptime++;
						p_ptime_str = zyStrtok(NULL, ",");
					}
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime = num_of_supported_ptime;
				}
				p_config_codec_attrib->codec_param.audio_param.pref_ptime = atoi(p_capCodecList->packetizationPeriod);

				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
				p_config_codec_attrib->codec_param.audio_param.silence_suppression = p_capCodecList->silenceSuppression;
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
				p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
				p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;

				p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
				p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
				if(NULL == p_config_codec_attrib->p_encoded_params) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}

				p_encoded_params = p_config_codec_attrib->p_encoded_params;					
				p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
				if(NULL == p_encoded_params->p_data) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}
				
				str_encoded_param = (icf_string_st *)p_encoded_params->p_data;
				zyStrcpy(str_encoded_param->str, SDP_CODEC_NAME_PCMU);
				str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
				p_encoded_params->p_next = NULL;

				codec_count++;
			}
		}

		if(NULL != (p_capCodecList = tr104GetLineCodecByCodecName(VpItr , LineItr , "G.711ALaw")))
		{
			if(p_capCodecList->priority < ICF_MAX_CODEC_SUPPORTD){
				VOICEAPP_DBG1("PCMA supported (SipAccountId ", SipAccountId);
				p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[p_capCodecList->priority-1];				
				p_config_codec_attrib->enable = p_capCodecList->enable;

				p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
				zyStrcpy(p_config_codec_attrib->codec_name.str, SDP_CODEC_NAME_PCMA);
				p_config_codec_attrib->codec_name.str_len = zyStrlen(p_config_codec_attrib->codec_name.str);

				p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
				p_config_codec_attrib->codec_num = ZYNOS_CODEC_PCMA;

				p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;

				num_of_supported_ptime = 0;
				if(NULL != (p_capCodec = tr104GetCapCodecByEntryID(p_capCodecList->entryID))){
					zyStrcpy(ptime_str, p_capCodec->packetizationPeriod);
					p_ptime_str = zyStrtok(ptime_str, ",");
					while(NULL != p_ptime_str){
						p_config_codec_attrib->codec_param.audio_param.ptime[num_of_supported_ptime] = atoi(p_ptime_str);
						p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
						num_of_supported_ptime++;
						p_ptime_str = zyStrtok(NULL, ",");
					}
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime = num_of_supported_ptime;
				}
				p_config_codec_attrib->codec_param.audio_param.pref_ptime = atoi(p_capCodecList->packetizationPeriod);
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
				p_config_codec_attrib->codec_param.audio_param.silence_suppression = p_capCodecList->silenceSuppression;
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
				p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
				p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;
				
				p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
				p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
				if(NULL == p_config_codec_attrib->p_encoded_params) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}
				
				p_encoded_params = p_config_codec_attrib->p_encoded_params;					
				p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
				if(NULL == p_encoded_params->p_data) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}
				str_encoded_param = (icf_string_st*)p_encoded_params->p_data;
				zyStrcpy(str_encoded_param->str, SDP_CODEC_NAME_PCMA);
				str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
				p_encoded_params->p_next = NULL;
				codec_count++;	
			}
		}

		if(NULL != (p_capCodecList = tr104GetLineCodecByCodecName(VpItr , LineItr , "G.729a")))
		{
			if(p_capCodecList->priority < ICF_MAX_CODEC_SUPPORTD){
				VOICEAPP_DBG1("G729 supported (SipAccountId ", SipAccountId);
				p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[p_capCodecList->priority-1];					
				p_config_codec_attrib->enable = p_capCodecList->enable;

				p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
				zyStrcpy(p_config_codec_attrib->codec_name.str, SDP_CODEC_NAME_G729);
				p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

				p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
				p_config_codec_attrib->codec_num = ZYNOS_CODEC_G729;

				p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;

				num_of_supported_ptime = 0;
				if(NULL != (p_capCodec = tr104GetCapCodecByEntryID(p_capCodecList->entryID))){
					zyStrcpy(ptime_str, p_capCodec->packetizationPeriod);
					p_ptime_str = zyStrtok(ptime_str, ",");
					while(NULL != p_ptime_str){
						p_config_codec_attrib->codec_param.audio_param.ptime[num_of_supported_ptime] = atoi(p_ptime_str);
						p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
						num_of_supported_ptime++;
						p_ptime_str = zyStrtok(NULL, ",");
					}
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime = num_of_supported_ptime;
				}
				p_config_codec_attrib->codec_param.audio_param.pref_ptime = atoi(p_capCodecList->packetizationPeriod);				
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
				p_config_codec_attrib->codec_param.audio_param.silence_suppression = p_capCodecList->silenceSuppression;
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
				p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
				p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;

				p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
				p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
				if(NULL == p_config_codec_attrib->p_encoded_params) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}

				p_encoded_params = p_config_codec_attrib->p_encoded_params;	
				p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
				if(NULL == p_encoded_params->p_data) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}
				str_encoded_param = (icf_string_st*)p_encoded_params->p_data;
				zyStrcpy(str_encoded_param->str, SDP_CODEC_NAME_G729);
				str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
				p_encoded_params->p_next = NULL;

				codec_count++;
			}
		}

		if(NULL != (p_capCodecList = tr104GetLineCodecByCodecName(VpItr , LineItr , "G.726_16")))
		{
			if(p_capCodecList->priority < ICF_MAX_CODEC_SUPPORTD){
				VOICEAPP_DBG1("G726_16 supported (SipAccountId ", SipAccountId);
				p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[p_capCodecList->priority-1];					
				p_config_codec_attrib->enable = p_capCodecList->enable;

				p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
				zyStrcpy(p_config_codec_attrib->codec_name.str, SDP_CODEC_NAME_G726_16);
				p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

				p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
				p_config_codec_attrib->codec_num = ZYNOS_CODEC_G726_16;

				p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;

				num_of_supported_ptime = 0;
				if(NULL != (p_capCodec = tr104GetCapCodecByEntryID(p_capCodecList->entryID))){
					zyStrcpy(ptime_str, p_capCodec->packetizationPeriod);
					p_ptime_str = zyStrtok(ptime_str, ",");
					while(NULL != p_ptime_str){
						p_config_codec_attrib->codec_param.audio_param.ptime[num_of_supported_ptime] = atoi(p_ptime_str);
						p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
						num_of_supported_ptime++;
						p_ptime_str = zyStrtok(NULL, ",");
					}
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime = num_of_supported_ptime;
				}
				p_config_codec_attrib->codec_param.audio_param.pref_ptime = atoi(p_capCodecList->packetizationPeriod);				
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
				p_config_codec_attrib->codec_param.audio_param.silence_suppression = p_capCodecList->silenceSuppression;
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
				p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
				p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;

				p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
				p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
				if(NULL == p_config_codec_attrib->p_encoded_params) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}

				p_encoded_params = p_config_codec_attrib->p_encoded_params;	
				p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
				if(NULL == p_encoded_params->p_data) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}
				str_encoded_param = (icf_string_st*)p_encoded_params->p_data;
				zyStrcpy(str_encoded_param->str, SDP_CODEC_NAME_G726_16);
				str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
				p_encoded_params->p_next = NULL;

				codec_count++;
			}
		}

		if(NULL != (p_capCodecList = tr104GetLineCodecByCodecName(VpItr , LineItr , "G.726_24")))
		{
			if(p_capCodecList->priority < ICF_MAX_CODEC_SUPPORTD){
				VOICEAPP_DBG1("G726_24 supported (SipAccountId", SipAccountId);
				p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[p_capCodecList->priority-1];					
				p_config_codec_attrib->enable = p_capCodecList->enable;

				p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
				zyStrcpy(p_config_codec_attrib->codec_name.str, SDP_CODEC_NAME_G726_24);
				p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

				p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
				p_config_codec_attrib->codec_num = ZYNOS_CODEC_G726_24;

				p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;

				num_of_supported_ptime = 0;
				if(NULL != (p_capCodec = tr104GetCapCodecByEntryID(p_capCodecList->entryID))){
					zyStrcpy(ptime_str, p_capCodec->packetizationPeriod);
					p_ptime_str = zyStrtok(ptime_str, ",");
					while(NULL != p_ptime_str){
						p_config_codec_attrib->codec_param.audio_param.ptime[num_of_supported_ptime] = atoi(p_ptime_str);
						p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
						num_of_supported_ptime++;
						p_ptime_str = zyStrtok(NULL, ",");
					}
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime = num_of_supported_ptime;
				}
				p_config_codec_attrib->codec_param.audio_param.pref_ptime = atoi(p_capCodecList->packetizationPeriod);				
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
				p_config_codec_attrib->codec_param.audio_param.silence_suppression = p_capCodecList->silenceSuppression;
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
				p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
				p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;

				p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
				p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
				if(NULL == p_config_codec_attrib->p_encoded_params) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}

				p_encoded_params = p_config_codec_attrib->p_encoded_params;	
				p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
				if(NULL == p_encoded_params->p_data) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}
				str_encoded_param = (icf_string_st*)p_encoded_params->p_data;
				zyStrcpy(str_encoded_param->str, SDP_CODEC_NAME_G726_24);
				str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
				p_encoded_params->p_next = NULL;

				codec_count++;
			}
		}

		if(NULL != (p_capCodecList = tr104GetLineCodecByCodecName(VpItr , LineItr , "G.726_32")))
		{
			if(p_capCodecList->priority < ICF_MAX_CODEC_SUPPORTD){
				VOICEAPP_DBG1("G726_32 supported (SipAccountId ", SipAccountId);
				p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[p_capCodecList->priority-1];					
				p_config_codec_attrib->enable = p_capCodecList->enable;

				p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
				zyStrcpy(p_config_codec_attrib->codec_name.str, SDP_CODEC_NAME_G726_32);
				p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

				p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
				p_config_codec_attrib->codec_num = ZYNOS_CODEC_G726_32;

				p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;

				num_of_supported_ptime = 0;
				if(NULL != (p_capCodec = tr104GetCapCodecByEntryID(p_capCodecList->entryID))){
					zyStrcpy(ptime_str, p_capCodec->packetizationPeriod);
					p_ptime_str = zyStrtok(ptime_str, ",");
					while(NULL != p_ptime_str){
						p_config_codec_attrib->codec_param.audio_param.ptime[num_of_supported_ptime] = atoi(p_ptime_str);
						p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
						num_of_supported_ptime++;
						p_ptime_str = zyStrtok(NULL, ",");
					}
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime = num_of_supported_ptime;
				}
				p_config_codec_attrib->codec_param.audio_param.pref_ptime = atoi(p_capCodecList->packetizationPeriod);				
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
				p_config_codec_attrib->codec_param.audio_param.silence_suppression = p_capCodecList->silenceSuppression;
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
				p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
				p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;

				p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
				p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
				if(NULL == p_config_codec_attrib->p_encoded_params) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}

				p_encoded_params = p_config_codec_attrib->p_encoded_params;	
				p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
				if(NULL == p_encoded_params->p_data) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}
				str_encoded_param = (icf_string_st*)p_encoded_params->p_data;
				zyStrcpy(str_encoded_param->str, SDP_CODEC_NAME_G726_32);
				str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
				p_encoded_params->p_next = NULL;

				codec_count++;
			}
		}

		if(NULL != (p_capCodecList = tr104GetLineCodecByCodecName(VpItr , LineItr , "G.726_40")))
		{
			if(p_capCodecList->priority < ICF_MAX_CODEC_SUPPORTD){
				VOICEAPP_DBG1("G726_40 supported (SipAccountId ", SipAccountId);
				p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[p_capCodecList->priority-1];					
				p_config_codec_attrib->enable = p_capCodecList->enable;

				p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
				zyStrcpy(p_config_codec_attrib->codec_name.str, SDP_CODEC_NAME_G726_40);
				p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

				p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
				p_config_codec_attrib->codec_num = ZYNOS_CODEC_G726_40;

				p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;

				num_of_supported_ptime = 0;
				if(NULL != (p_capCodec = tr104GetCapCodecByEntryID(p_capCodecList->entryID))){
					zyStrcpy(ptime_str, p_capCodec->packetizationPeriod);
					p_ptime_str = zyStrtok(ptime_str, ",");
					while(NULL != p_ptime_str){
						p_config_codec_attrib->codec_param.audio_param.ptime[num_of_supported_ptime] = atoi(p_ptime_str);
						p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
						num_of_supported_ptime++;
						p_ptime_str = zyStrtok(NULL, ",");
					}
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime = num_of_supported_ptime;
				}
				p_config_codec_attrib->codec_param.audio_param.pref_ptime = atoi(p_capCodecList->packetizationPeriod);				
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
				p_config_codec_attrib->codec_param.audio_param.silence_suppression = p_capCodecList->silenceSuppression;
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
				p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
				p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;

				p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
				p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
				if(NULL == p_config_codec_attrib->p_encoded_params) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}

				p_encoded_params = p_config_codec_attrib->p_encoded_params;	
				p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
				if(NULL == p_encoded_params->p_data) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}
				str_encoded_param = (icf_string_st*)p_encoded_params->p_data;
				zyStrcpy(str_encoded_param->str, SDP_CODEC_NAME_G726_40);
				str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
				p_encoded_params->p_next = NULL;

				codec_count++;
			}
		}

		if(NULL != (p_capCodecList = tr104GetLineCodecByCodecName(VpItr , LineItr , "G.722")))
		{
			if(p_capCodecList->priority < ICF_MAX_CODEC_SUPPORTD){
				VOICEAPP_DBG1("G722 supported (SipAccountId ", SipAccountId);
				p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[p_capCodecList->priority-1];					
				p_config_codec_attrib->enable = p_capCodecList->enable;

				p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
				zyStrcpy(p_config_codec_attrib->codec_name.str, "G.722");
				p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

				p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
				p_config_codec_attrib->codec_num = ZYNOS_CODEC_G722MODE1;

				p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;

				num_of_supported_ptime = 0;
				if(NULL != (p_capCodec = tr104GetCapCodecByEntryID(p_capCodecList->entryID))){
					zyStrcpy(ptime_str, p_capCodec->packetizationPeriod);
					p_ptime_str = zyStrtok(ptime_str, ",");
					while(NULL != p_ptime_str){
						p_config_codec_attrib->codec_param.audio_param.ptime[num_of_supported_ptime] = atoi(p_ptime_str);
						p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
						num_of_supported_ptime++;
						p_ptime_str = zyStrtok(NULL, ",");
					}
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime = num_of_supported_ptime;
				}
				p_config_codec_attrib->codec_param.audio_param.pref_ptime = atoi(p_capCodecList->packetizationPeriod);				
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
				p_config_codec_attrib->codec_param.audio_param.silence_suppression = p_capCodecList->silenceSuppression;
				p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
				p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
				p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;

				p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
				p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
				if(NULL == p_config_codec_attrib->p_encoded_params) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}

				p_encoded_params = p_config_codec_attrib->p_encoded_params;	
				p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
				if(NULL == p_encoded_params->p_data) {
					icf_free_decoded_msg(p_msg,p_err);
					return(FALSE);
				}
				str_encoded_param = (icf_string_st*)p_encoded_params->p_data;
				zyStrcpy(str_encoded_param->str, "G.722");
				str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
				p_encoded_params->p_next = NULL;

				codec_count++;
			}
		}

		VoiceProfFaxT38Obj_t* pProfFax = tr104VoiceProfFaxT38ObjPtrGet(VpItr);
		if(1 == pProfFax->enable)
		{
			if(NULL != (p_capCodecList = tr104GetLineCodecByCodecName(VpItr , LineItr , "X_5067F0_T38")))
			{
				if(codec_count < ICF_MAX_CODEC_SUPPORTD)
				{
					VOICEAPP_DBG1("T.38 supported (SipAccountId ", SipAccountId);
					p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[codec_count];
					p_config_codec_attrib->enable = p_capCodecList->enable;

					p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_IMAGE;
					zyStrcpy(p_config_codec_attrib->codec_name.str, SDP_CODEC_NAME_T38);
					p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

					p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
					p_config_codec_attrib->codec_num = ZYNOS_CODEC_T38;
							
					p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
					p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
					if(NULL == p_config_codec_attrib->p_encoded_params) {
						return(FALSE);
					}

				#if 0
					/**< cindy set t38 params hard code now */ 
					p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;
					p_config_codec_attrib->codec_param.t38_param.bit_mask |= ICF_FAX_T38_VERSION;
					p_config_codec_attrib->codec_param.t38_param.fax_version = ICF_T38_1998;
					p_config_codec_attrib->codec_param.t38_param.bit_mask |= ICF_FAX_T38_BITRATE;
					p_config_codec_attrib->codec_param.t38_param.bitrate = ICF_T38_14400_BIT_PER_SEC;
					p_config_codec_attrib->codec_param.t38_param.bit_mask |= ICF_FAX_T38_RATE_MANAGEMENT;
					p_config_codec_attrib->codec_param.t38_param.rate_management = ICF_T38_RATE_MANAGEMENT_TRANSFERRED;
					p_config_codec_attrib->codec_param.t38_param.bit_mask |= ICF_FAX_T38_ERR_CORRECTION; 
					p_config_codec_attrib->codec_param.t38_param.err_correction_method = ICF_T38_ERROR_CORR_REDUNDANCY;
					/**< cindy set t38 params hard code now */ 
				#endif

					p_encoded_params = p_config_codec_attrib->p_encoded_params;
					p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
					if(NULL == p_encoded_params->p_data) {
						icf_free_decoded_msg(p_msg,p_err);
						return(FALSE);
					}
					
					str_encoded_param = (icf_string_st *)p_encoded_params->p_data;
					zyStrcpy(str_encoded_param->str, SDP_CODEC_NAME_T38);
					str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
					p_encoded_params->p_next = NULL;

					codec_count++;	
				}
			}
		}

		VoiceProfObj_t* pProfObj = tr104VoiceProfObjPtrGet(VpItr);
		if((TR104I_DTMF_RFC2833 == pProfObj->DTMFMethod) || (TR104I_DTMF_RFC2833 == pProfObj->DTMFMethodG711))
		{	
			#if 0 /*Eason, only need to check dtmf method*/
			if(NULL != (p_capCodecList = tr104GetLineCodecByEntryID(line_index, ZYNOS_CODEC_RFC2833)))
			{
			#endif
				if(codec_count < ICF_MAX_CODEC_SUPPORTD)
				{
					VOICEAPP_DBG1("telephone-event supported (SipAccountId ", SipAccountId);
					p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[codec_count];
					#if 0
					p_config_codec_attrib->enable = p_capCodecList->enable;
					#endif
					p_config_codec_attrib->enable = 1;

					p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
					zyStrcpy(p_config_codec_attrib->codec_name.str, SDP_CODEC_NAME_TELEPHONE_EVENT);
					p_config_codec_attrib->codec_name.str_len = zyStrlen(p_config_codec_attrib->codec_name.str);

					p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
					p_config_codec_attrib->codec_num = ZYNOS_CODEC_RFC2833;

					p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;
					p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
					p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
					p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;
								
					p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
					p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
					if(NULL == p_config_codec_attrib->p_encoded_params) {
						icf_free_decoded_msg(p_msg,p_err);
						return(FALSE);
					}

					p_encoded_params = p_config_codec_attrib->p_encoded_params;	
					p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
					if(NULL == p_encoded_params->p_data) {
						icf_free_decoded_msg(p_msg,p_err);
						return(FALSE);
					}
					
					str_encoded_param = (icf_string_st *)p_encoded_params->p_data;
					zyStrcpy(str_encoded_param->str, SDP_CODEC_NAME_TELEPHONE_EVENT);
					str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
					p_encoded_params->p_next = NULL;

					codec_count++;				
				}
			#if 0				
			}
			#endif
		}

		p_cfg_codec_info_req->count = codec_count;
		if(p_cfg_codec_info_req->count > 0){
			ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_CODEC_INFO_REQ, ICF_INVALID_CALL_ID);
		}

		icf_free_decoded_msg(p_msg,p_err);

		if(ret_val==ICF_FAILURE) {
			return(FALSE);
		}
	
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgPortRange
 *
 * DESCRIPTION   : The function to fill the port range to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgPortRange(uint32 SipAccountId){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val;
	icf_cfg_set_port_range_req_st	*p_cfg_port_rng_req = ICF_NULL;

	int LineItr = 0, VpItr=0;

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	
	VoiceProfRtpObj_t * pProfRtp = tr104VoiceProfRtpObjPtrGet(VpItr);
	VoiceProfRtpRtcpObj_t * pProfRtcp = tr104VoiceProfRtpRtcpObjPtrGet(VpItr);
	#if SRTP_SUPPORT
	VoiceProfRtpSrtpObj_t * pProfSrtp = tr104VoiceProfRtpSrtpObjPtrGet(VpItr);	
	#endif

	VOICEAPP_DBG("voiceApp -> IPTK : ICF_CFG_SET_PORT_RANGE_REQ");

		VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_set_port_range_req_st),ret_val);
		
		if(ICF_FAILURE == ret_val) {
			ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_SET_PORT_RANGE_REQ\n");
			return(FALSE);
		}

		p_cfg_port_rng_req  = (icf_cfg_set_port_range_req_st*)(p_msg->payload);
		p_cfg_port_rng_req->bit_mask = 0; 

		p_cfg_port_rng_req->start_rtp_port = pProfRtp->localPortMin;
		p_cfg_port_rng_req->end_rtp_port = pProfRtp->localPortMax;
		p_cfg_port_rng_req->is_app_rtcp_port_mux = 0;
#if 1 /*ZYXEL_PORTING*/
		p_cfg_port_rng_req->rtcp_enable = pProfRtcp->enable;
		p_cfg_port_rng_req->rtcp_tx_interval = pProfRtcp->txRepeatInterval;
		zyStrcpy(p_cfg_port_rng_req->local_cname, pProfRtcp->localCName);
#endif
		p_cfg_port_rng_req->SipAccountId = SipAccountId; 
		VOICEAPP_PRINTF("ICF_CFG_SET_PORT_RANGE_REQ VpItr %d, SipAccountId %d\n ", VpItr, p_cfg_port_rng_req->SipAccountId);

#if SRTP_SUPPORT
		p_cfg_port_rng_req->bit_mask |= ICF_APP_SRTP_PARAM_PRESENT; 
		p_cfg_port_rng_req->srtp_enable = pProfSrtp->enable;
#define DEFAULT_SRTP_KEY_SIZE 240
		p_cfg_port_rng_req->srtp_encryption_key_size = DEFAULT_SRTP_KEY_SIZE;
		
		if(zyStrlen(pProfSrtp->ZyXEL_CryptoSuiteName)>0){
			zyStrcpy(p_cfg_port_rng_req->srtp_crypto_suite_name, pProfSrtp->ZyXEL_CryptoSuiteName);
			VOICEAPP_PRINTF("srtp_crypto_suite_name = %d\n", p_cfg_port_rng_req->srtp_crypto_suite_name);
		}
		else{
			zyMemset(p_cfg_port_rng_req->srtp_crypto_suite_name, 0 ,sizeof(p_cfg_port_rng_req->srtp_crypto_suite_name));
		}
		
		VOICEAPP_PRINTF("srtp_enable = %d\n", p_cfg_port_rng_req->srtp_enable);
		VOICEAPP_PRINTF("srtp_encryption_key_size = %d\n", p_cfg_port_rng_req->srtp_encryption_key_size);
#endif

		
		if (ICF_TRUE == p_cfg_port_rng_req->is_app_rtcp_port_mux) {
			p_cfg_port_rng_req->bit_mask |= ICF_APP_PORT_RANGE_PRESENT;	
			p_cfg_port_rng_req->start_app_port = 60024;
			p_cfg_port_rng_req->end_app_port = 60034;
		}
			
		ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_SET_PORT_RANGE_REQ, ICF_INVALID_CALL_ID);

		icf_free_decoded_msg(p_msg,p_err);

		if(ret_val==ICF_FAILURE) {
			return(FALSE);
		}
		
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgMedia
 *
 * DESCRIPTION   : The function to fill the media to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgMedia(void){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val;
	icf_cfg_media_req_st	*p_cfg_media_req = ICF_NULL;
	
	VOICEAPP_DBG("voiceApp -> IPTK : ICF_CFG_MEDIA_REQ");
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_media_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_MEDIA_REQ\n");
		return(FALSE);
	}

	p_cfg_media_req  = (icf_cfg_media_req_st*)(p_msg->payload);

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_MEDIA_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgJbmMgmt
 *
 * DESCRIPTION   : The function to fill the jitter buffer to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgJbmMgmt(void){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val;
	icf_cfg_jbm_mgmt_req_st	*p_cfg_jbm_mgmt_req = ICF_NULL;
	
	VOICEAPP_DBG("voiceApp -> IPTK : ICF_CFG_JBM_MGMT_REQ");
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_jbm_mgmt_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_JBM_MGMT_REQ\n");
		return(FALSE);
	}

	p_cfg_jbm_mgmt_req  = (icf_cfg_jbm_mgmt_req_st*)(p_msg->payload);

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_JBM_MGMT_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgMaxEarpVol
 *
 * DESCRIPTION   : The function to fill the max earp volume to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgMaxEarpVol(void){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val;
	icf_cfg_max_earp_vol_req_st	*p_cfg_max_earp_vol_req = ICF_NULL;
	
	VOICEAPP_DBG("voiceApp -> IPTK : ICF_CFG_MAX_EARP_VOL_REQ");
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_max_earp_vol_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_MAX_EARP_VOL_REQ\n");
		return(FALSE);
	}

	p_cfg_max_earp_vol_req  = (icf_cfg_max_earp_vol_req_st*)(p_msg->payload);

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_MAX_EARP_VOL_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgMaxSpeakerVol
 *
 * DESCRIPTION   : The function to fill the max speaker volume to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgMaxSpeakerVol(void){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val;
	icf_cfg_max_speaker_vol_req_st	*p_cfg_max_speaker_vol_req = ICF_NULL;
	
	VOICEAPP_DBG("voiceApp -> IPTK : ICF_CFG_MAX_SPEAKER_VOL_REQ");
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_max_speaker_vol_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_MAX_SPEAKER_VOL_REQ\n");
		return(FALSE);
	}

	p_cfg_max_speaker_vol_req  = (icf_cfg_max_speaker_vol_req_st*)(p_msg->payload);

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_MAX_SPEAKER_VOL_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgDigitTrans
 *
 * DESCRIPTION   : The function to fill the digit trans method to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgDigitTrans(void){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val;
	icf_cfg_set_digit_trans_req_st	*p_cfg_digit_trans_req = ICF_NULL;
	
	VOICEAPP_DBG("voiceApp -> IPTK : ICF_CFG_SET_DIGIT_TRANS_REQ");
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_set_digit_trans_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_SET_DIGIT_TRANS_REQ\n");
		return(FALSE);
	}

	p_cfg_digit_trans_req  = (icf_cfg_set_digit_trans_req_st*)(p_msg->payload);

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_SET_DIGIT_TRANS_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgRemoteVideoDisplay
 *
 * DESCRIPTION   : The function to fill the remote video display to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgRemoteVideoDisplay(void){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val;
	icf_cfg_set_remote_video_display_st	*p_cfg_remote_video_display_req = ICF_NULL;
	
	VOICEAPP_DBG("voiceApp -> IPTK : ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ");
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_set_remote_video_display_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ\n");
		return(FALSE);
	}

	p_cfg_remote_video_display_req  = (icf_cfg_set_remote_video_display_st*)(p_msg->payload);

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgPreviewVideoDisplay
 *
 * DESCRIPTION   : The function to fill the preview video display to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgPreviewVideoDisplay(void){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val;
	icf_cfg_set_preview_video_display_st	*p_cfg_preview_video_display_req = ICF_NULL;
	
	VOICEAPP_DBG("voiceApp -> IPTK : ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ");
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_set_preview_video_display_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ\n");
		return(FALSE);
	}

	p_cfg_preview_video_display_req  = (icf_cfg_set_preview_video_display_st*)(p_msg->payload);

	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}
	return(TRUE);
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmCfgMaxMicGain
 *
 * DESCRIPTION   : The function to fill the max mic gain to be added to be send
 *					through IPTK towards MM
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool mmCfgMaxMicGain(void){
	icf_error_t    *p_err = NULL;
	icf_msg_st  *p_msg;
	icf_uint32_t ret_val;
	icf_cfg_max_mic_gain_req_st	*p_cfg_max_mic_gain_req = ICF_NULL;
	
	VOICEAPP_DBG("voiceApp -> IPTK : ICF_CFG_MAX_MIC_GAIN_REQ");
	
	VOICEAPP_IPTK_MSG_ALLOC(p_msg, sizeof(icf_cfg_max_mic_gain_req_st),ret_val);
	
	if(ICF_FAILURE == ret_val) {
		ZyPrintf("MEMORY ALLOC FAIL : ICF_CFG_MAX_MIC_GAIN_REQ\n");
		return(FALSE);
	}

	p_cfg_max_mic_gain_req  = (icf_cfg_max_mic_gain_req_st*)(p_msg->payload);
	
	ret_val = voiceAppFillHdrSendIptk(p_msg, ICF_CFG_MAX_MIC_GAIN_REQ, ICF_INVALID_CALL_ID);

	icf_free_decoded_msg(p_msg,p_err);

	if(ret_val==ICF_FAILURE){
		return(FALSE);
	}
	
	return(TRUE);
}

