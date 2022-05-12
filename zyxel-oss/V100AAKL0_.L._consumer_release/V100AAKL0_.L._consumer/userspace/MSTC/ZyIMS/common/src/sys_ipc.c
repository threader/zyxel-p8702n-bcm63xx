#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "global.h"
#include "sys_msg.h"
#include "icf_types.h"
#include "voiceApp_main.h"
#include "voice_dsp_common_api.h"
#include "voice_dsp_utils.h"
#include "mm_macros.h"

/*______________________________________________________________________________
**  eventVoiceSimpleSend
**
**  descriptions:
**	this function is to send a simple cbuf to destination without data
**	and it is a non-blocking call.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
eventVoiceSimpleSend(
	uint16	event,			/* cbuf event	*/
	uint16	dest_fid,		/* destination func-ID */
	uint16	chann_id		/* channel ID */
) {
	evtMsg_t	*msgSend = NULL;			/* cbuf pointer to be sent	*/
	int sendSock;
	struct sockaddr_un addr = {0};

	msgSend = (evtMsg_t*)zyMalloc(sizeof(evtMsg_t));

	if (msgSend == NULL) {
		return -2;
	}
	
	msgSend->event = event;
	msgSend->chann_id = chann_id;
	msgSend->data_size = 0;
	
	sendSock = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(sendSock == -1) {
		zyFree(msgSend);
		return -1;
	}

	zyMemset(&addr, 0, sizeof(addr));
	addr.sun_family = PF_LOCAL;
	switch(dest_fid) {
		case MSG_SEND_TO_CCMCONN:
		case ORIG_FUNC_CCMCONN:
			zyStrncpy(addr.sun_path, CCM_CHANNEL_PATH, sizeof(CCM_CHANNEL_PATH));
			break;
		case MSG_SEND_TO_PHONE:
		case ORIG_FUNC_PHONE:
			zyStrncpy(addr.sun_path, PHONE_CHANNEL_PATH, sizeof(PHONE_CHANNEL_PATH));
			break;
		case MSG_SEND_TO_SIP:
		case ORIG_FUNC_SIP:
			zyStrncpy(addr.sun_path, VOICEAPP_CCM_CHANNEL_PATH, sizeof(VOICEAPP_CCM_CHANNEL_PATH));
			break;
	}
	sendto(sendSock, msgSend, sizeof(evtMsg_t), 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
	
	close(sendSock);
	zyFree(msgSend);
	
	return 0;
} /* eventVoiceSimpleSend */

/*______________________________________________________________________________
**  eventDataSend
**
**  descriptions:
**	this function is to send a simple cbuf to destination with data
**	and it is a non-blocking call.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
eventVoiceDataSend(
	uint16	event,			/* cbuf event	*/
	uint16	dest_fid,		/* destination func-ID */
	uint16	chann_id,		/* channel ID */
	void		*data_p,		/* transmit data pointer	*/
	uint16	data_size		/* transmit data size */
)
{
	evtMsg_t	*msgSend = NULL;			/* cbuf pointer to be sent	*/
	int sendSock;
	struct sockaddr_un addr = {0};

	msgSend = (evtMsg_t*)zyMalloc(sizeof(evtMsg_t));

	if(msgSend == NULL) {
		return -2;
	}
	
	msgSend->event = event;
	msgSend->chann_id = chann_id;
	if (data_size>(MAX_SIZE_MSG_SEND)){
		printf("data_size too large=%x\n", data_size);
		zyFree(msgSend);
		return -1;
	}
	if ( data_p != NULL && data_size != 0)
		zyMemcpy( msgSend->data_p, data_p, data_size );
	msgSend->data_size= data_size;
	
	sendSock = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(sendSock == -1) {
		zyFree(msgSend);
		return -1;
	}

	zyMemset(&addr, 0, sizeof(addr));
	addr.sun_family = PF_LOCAL;
	switch(dest_fid) {
		case MSG_SEND_TO_CCMCONN:
		case ORIG_FUNC_CCMCONN:
			zyStrncpy(addr.sun_path, CCM_CHANNEL_PATH, sizeof(CCM_CHANNEL_PATH));
			break;
		case MSG_SEND_TO_PHONE:
		case ORIG_FUNC_PHONE:
			zyStrncpy(addr.sun_path, PHONE_CHANNEL_PATH, sizeof(PHONE_CHANNEL_PATH));
			break;
		case MSG_SEND_TO_SIP:
		case ORIG_FUNC_SIP:
			zyStrncpy(addr.sun_path, VOICEAPP_CCM_CHANNEL_PATH, sizeof(VOICEAPP_CCM_CHANNEL_PATH));
			break;
	}

	sendto(sendSock, msgSend, sizeof(evtMsg_t), 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
	
	close(sendSock);
	zyFree(msgSend);
	
	return 0;
} /* eventVoiceDataSend */

/*______________________________________________________________________________
**  eventVoiceCCMSimpleSend
**
**  descriptions:
**	this function is to send a simple cbuf to destination without data
**	and it is a non-blocking call.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
eventVoiceCCMSimpleSend(
	uint16	event,			/* cbuf event	*/
	uint16	dest_fid,		/* destination func-ID */
	uint16	chann_id,		/* channel ID */
	uint16	type			/* type */
) {
	evtMsg_t	*msgSend = NULL;			/* cbuf pointer to be sent	*/
	int sendSock;
	struct sockaddr_un addr = {0};

	msgSend = (evtMsg_t*)zyMalloc(sizeof(evtMsg_t));

	if(msgSend == NULL) {
		return -2;
	}
	
	msgSend->event = event;
	msgSend->chann_id = chann_id;
	msgSend->dev_type= type;
	msgSend->data_size = 0;
	
	sendSock = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(sendSock == -1) {
		zyFree(msgSend);
		return -1;
	}

	zyMemset(&addr, 0, sizeof(addr));
	addr.sun_family = PF_LOCAL;
	switch(dest_fid) {
		case MSG_SEND_TO_CCMCONN:
		case ORIG_FUNC_CCMCONN:
			zyStrncpy(addr.sun_path, CCM_CHANNEL_PATH, sizeof(CCM_CHANNEL_PATH));
			break;
		case MSG_SEND_TO_PHONE:
		case ORIG_FUNC_PHONE:
			zyStrncpy(addr.sun_path, PHONE_CHANNEL_PATH, sizeof(PHONE_CHANNEL_PATH));
			break;
		case MSG_SEND_TO_SIP:
		case ORIG_FUNC_SIP:
			zyStrncpy(addr.sun_path, VOICEAPP_CCM_CHANNEL_PATH, sizeof(VOICEAPP_CCM_CHANNEL_PATH));
			break;
	}

	sendto(sendSock, msgSend, sizeof(evtMsg_t), 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
	
	close(sendSock);
	zyFree(msgSend);
	
	return 0;
} /* eventVoiceCCMSimpleSend */

/*______________________________________________________________________________
**  eventVoiceCCMDataSend
**
**  descriptions:
**	this function is to send a simple cbuf to destination with data
**	and it is a non-blocking call.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
eventVoiceCCMDataSend(
	uint16	event,			/* cbuf event	*/
	uint16	dest_fid,		/* destination func-ID */
	uint16	chann_id,		/* channel ID */
	uint16	type,			/* type */
	void		*data_p,		/* transmit data pointer	*/
	uint16	data_size		/* transmit data size */
)
{
	evtMsg_t	*msgSend = NULL;			/* cbuf pointer to be sent	*/
	int sendSock;
	struct sockaddr_un addr = {0};

	msgSend = (evtMsg_t*)zyMalloc(sizeof(evtMsg_t));

	if (msgSend == NULL) {
		return -2;
	}
	
	msgSend->event = event;
	msgSend->chann_id = chann_id;
	msgSend->dev_type= type;
	if (data_size>(MAX_SIZE_MSG_SEND)){
		printf("data_size too large=%x\n", data_size);
		zyFree(msgSend);
		msgSend = NULL;		
		return -1;
	}
	if ( data_p != NULL && data_size != 0) {
		zyMemcpy( msgSend->data_p, data_p, data_size );
	msgSend->data_size= data_size;
	}
	
	sendSock = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(sendSock == -1) {
		zyFree(msgSend);
	msgSend = NULL;	
		return -1;
	}

	zyMemset(&addr, 0, sizeof(addr));
	addr.sun_family = PF_LOCAL;
	switch(dest_fid) {
		case MSG_SEND_TO_CCMCONN:
		case ORIG_FUNC_CCMCONN:
			zyStrncpy(addr.sun_path, CCM_CHANNEL_PATH, sizeof(CCM_CHANNEL_PATH));
			break;
		case MSG_SEND_TO_PHONE:
		case ORIG_FUNC_PHONE:
			zyStrncpy(addr.sun_path, PHONE_CHANNEL_PATH, sizeof(PHONE_CHANNEL_PATH));
			break;
		case MSG_SEND_TO_SIP:
		case ORIG_FUNC_SIP:
			zyStrncpy(addr.sun_path, VOICEAPP_CCM_CHANNEL_PATH, sizeof(VOICEAPP_CCM_CHANNEL_PATH));
			break;
	}

	sendto(sendSock, msgSend, sizeof(evtMsg_t), 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_un));
	
	close(sendSock);
	zyFree(msgSend);
	
	return 0;
} /* eventVoiceCCMDataSend */

/*______________________________________________________________________________
**  eventVoiceDspDataSend
**
**  descriptions:
**	this function is to send a voice dsp request message to MM and receive corresponding response
**  parameters:
**  (input) uint8 *p_input_buf: voice message send to MM
**  (outout) uint8 **p_p_output_buf: voice response message received from MM
**  			if return success, p_p_output_buf should be free outside this function
**  local:
**  global:
**  return: -1 = failue, 0 = success
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
eventVoiceDspDataSend(
	uint8 *p_input_buf, 
	uint8 **p_p_output_buf	
)
{
	struct sockaddr_un addr;
	int ret_val;
	uint8 *buf;
	uint16 recvLen=0;
	uint8 perr = 0;
	voiceMsg_t *p_msg;
	fd_set 	allset;
	int maxfd=0;
	struct timeval timeoutval;
	int timeLimit = 30000;
	int select_ressult = 0;
	
	p_msg=(voiceMsg_t *)p_input_buf;
	
	if(p_msg ==NULL) {
		return -1;
	}

	zyMemset(&addr, 0, sizeof(addr));
	addr.sun_family = PF_LOCAL;
	zyStrncpy(addr.sun_path, MM_CHANNEL_PATH, sizeof(MM_CHANNEL_PATH));
	ret_val=voiceEventIpcSend(globalVoiceApp.voiceApp_mm_fd, (struct sockaddr *)&addr,sizeof(struct sockaddr_un),p_msg);

	if(MM_FAILURE == ret_val){
		return -1;
	}
	
	buf=zyMalloc(MAX_MTU_SIZE);
	FD_ZERO(&allset);
	FD_SET(globalVoiceApp.voiceApp_mm_fd, &allset);
	maxfd = globalVoiceApp.voiceApp_mm_fd;
	timeoutval.tv_sec = timeLimit / 1000;
	timeoutval.tv_usec = ( timeLimit % 1000 ) * 1000;

	select_ressult = select(maxfd+1, &allset, NULL, NULL, &timeoutval);
	if(select_ressult > 0 ){
		if(FD_ISSET(globalVoiceApp.voiceApp_mm_fd, &allset)) {
			recvLen=recvfrom(globalVoiceApp.voiceApp_mm_fd, buf, MAX_MTU_SIZE, 0, NULL, NULL);
		}
	}
	else if(select_ressult == 0 ){/*timeout*/
		printf("%s: ****timeout wait response****, non-consistent state, exit program\n",__FUNCTION__);
		zyFree(buf);
		buf = NULL;
		kill(getpid(), SIGTERM);
	}
	else{ /*socket error*/
		printf("%s: ****error wait response****\n",__FUNCTION__);
		zyFree(buf);
		buf = NULL;		
		kill(getpid(), SIGTERM);
	}

	ret_val=voiceMsgDecode((voiceMsg_t **)p_p_output_buf, buf, recvLen, &perr);	

	zyFree(buf);
	buf = NULL;
	if(MM_FAILURE == ret_val){
		return -1;
	}

	return 0;
} /* eventVoiceDataSend */

