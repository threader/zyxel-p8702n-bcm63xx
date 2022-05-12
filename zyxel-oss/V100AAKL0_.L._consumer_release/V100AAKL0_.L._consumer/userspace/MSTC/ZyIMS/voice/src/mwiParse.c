#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include "icf_types.h"
#include "icf_feature_flags.h"
#include "icf_defs.h"
#include "icf_trace_id.h"
#include "icf_trace_error_types.h"
#include "icf_error_id.h"
#include "icf_api.h"
#include "global.h"
#include "call_mgnt.h"
#include "Voipccm_sip_itf.h"
#include "voiceApp_main.h"
#include "voiceApp_iptk_MsgHandler.h"
#include "voiceApp_iptk_api.h"
#include "voiceApp_macro.h"
#include "mwiParse.h"


static char SDP_FIELDS[16][3] =	{ "v=","o=","s=","i=","u=","e=","p=","b=",
				  "c=","r=","z=","t=","k=","a=","m=","\0" };

static char SDP_TOKEN_SEPERATOR[3][2] =	{ ":","/","\0" };

static char SDP_SPACE[3][2] = { " ","\t","\0" }; 

SdpSessMWI
sdpMWINewFromText(
	const char* sdpText
){
	SdpList	lineList = NULL;
	SdpList	lineListIter = NULL;
	SdpSessMWI	sdp = NULL;

	if(!sdpText) {
		return NULL;
	}

	lineList = lineListIter = sdpLineParse(sdpText);

	/* version*/
	if( lineListIter==NULL ) {
		return NULL;
	}

	if( strncmp(lineListIter->pToken, SDP_MWI_STATUS_TOKEN, zyStrlen(SDP_MWI_STATUS_TOKEN)) != 0 ) {
		//TCRPrint("<sdpMWINewFromText()>:\"Messages-Waiting: \" error.\r\n", SIP_DBGMSG_ERR);
		sdpListFree(lineList);	// Yu-Shun modified 11/28/2003
		return NULL;
	} else {
		char* status = lineListIter->pToken+zyStrlen(SDP_MWI_STATUS_TOKEN);
		uint16 enable = 0;

		sdp = sdpMWINew();
		if( strncmp(status, "yes", 3) == 0) {
			enable = 1;
		}
		sdpSessAddMWIBody(sdp, Status, &enable);
	}
	lineListIter = lineListIter->pNext;
	if(!lineListIter) {
		goto sdp_success;
	}

	/* account*/
	if( strncmp(lineListIter->pToken, SDP_MWI_ACCOUNT_TOKEN, zyStrlen(SDP_MWI_ACCOUNT_TOKEN)) == 0 ) {
		char*	status = lineListIter->pToken+zyStrlen(SDP_MWI_ACCOUNT_TOKEN);
		sdpSessAddMWIBody(sdp, Account, status);
		lineListIter = lineListIter->pNext;
		if(!lineListIter) {
			goto sdp_success;
		}
	}

	/* voice*/
	if( strncmp(lineListIter->pToken, SDP_MWI_VOICE_TOKEN, zyStrlen(SDP_MWI_VOICE_TOKEN)) == 0 ) {
		char*	status = lineListIter->pToken+zyStrlen(SDP_MWI_VOICE_TOKEN);
		SdpMessageMWI		pVoice;
		pVoice = sdpMWIMessageParse(status);
		sdpSessAddMWIBody(sdp, Voicemessage, &pVoice);
		lineListIter = lineListIter->pNext;
		if(!lineListIter) {
			goto sdp_success;
		}
	}

	if(strncmp(lineListIter->pToken, SDP_MWI_VOICE_TOKEN2, zyStrlen(SDP_MWI_VOICE_TOKEN)) == 0) {
		char*	status = lineListIter->pToken+zyStrlen(SDP_MWI_VOICE_TOKEN2);
		SdpMessageMWI		pVoice;
		pVoice = sdpMWIMessageParse(status);
		sdpSessAddMWIBody(sdp, Voicemessage, &pVoice);
		lineListIter = lineListIter->pNext;
		if(!lineListIter) {
			goto sdp_success;
		}
	}

sdp_success:
	sdpListFree(lineList);
	return sdp;

sdp_parse_err:
	sdpListFree(lineList);
	sdpMWIFree(sdp);
	return NULL;
}

uint32 sdpMWIGetNewVoiceMailCount(SdpSessMWI sdpMWI)
{
	char buf[128];

	printf("*** [sdpMWIGetNewVoiceMailCount] coming .. \r\n");
	if (sdpMWI->status){
		sprintf(buf,"*** [sdpMWIGetNewVoiceMailCount] MWI new message = %d\r\n",sdpMWI->pVoice.newMessage);
		printf("*****[sdpMWIGetNewVoiceMailCount]*****buf=%s\r\n",buf);
		return sdpMWI->pVoice.newMessage;
	}
	return 0;
}

void sdpListFree(SdpList tokList)
{
	SdpList	pNext = tokList;

	if(!tokList) {
		return;
	}

	while(pNext) {
		tokList = tokList->pNext;

		if(pNext->pToken) {
			zyFree(pNext->pToken);
		}
		zyFree(pNext);

		pNext = tokList;
	}

}

void	sdpMWIFree(SdpSessMWI this_)
{
	if(!this_) {
		return;
	}

	if (this_->pAccount != NULL) {
		zyFree(this_->pAccount);
	}

	zyFree(this_);
	return;
}

SdpList sdpLineParse(const char* pSdpStr)
{
	char*	temp;
	char	*p;
	SdpList	tokList;
	int	i = -1;
	int	j = -1;

	if(!pSdpStr||*pSdpStr=='\0') {
		return NULL;
	}
	
	while( 1 ) {
		i++;
		if( isSDPSpace( (pSdpStr+i) ) ) {
			continue;
		} else {
			break;
		}
	}
	p = (char*)(pSdpStr+i);
	if( *p=='\0' ) {
		return NULL;
	}

	if( i>0 ) {
		tokList = (SdpList)zyMalloc(sizeof(SdpListElm));
		tokList->pToken = strDup(" ");
		tokList->pNext = sdpTokParse(p);

		return tokList;
	}

	while( 1 ) {
		j++;
		if( isCRLF(p+j) ) {
			j += isCRLF(p+j);
			break;
		}	
		if( p[j]=='\0' ) {
			break;
		}
	}

	temp = (char*)zyMalloc(sizeof(char)*(j+1));
	zyStrncpy(temp,p,j);	temp[j] = 0;

	tokList = (SdpList)zyMalloc(sizeof(SdpListElm));
	tokList->pToken = temp;
	tokList->pNext = sdpLineParse(p+j);

	return tokList;
}

int isSDPSpace(const char* field)
{
	int i = 0;

	if(!field) {
		return 0;
	}

	for( i=0; *SDP_SPACE[i]!='\0'; i++) {
		if( strncmp(SDP_SPACE[i],field,zyStrlen(SDP_SPACE[i]))==0 ) {
			return 1;
		}
	}

	return 0;
}

char* strDup(const char* s)
{
	char *p = NULL;
	
	if( s == NULL ) {
		return NULL;
	}
	p = (char*)zyMalloc(zyStrlen(s)+1);
	if( p != NULL){
		zyStrcpy(p,s);
		return p;
	}
	return NULL;
}

SdpList sdpTokParse(const char* pSdpStr)
{
	char*		temp = NULL;
	char*		p;
	SdpList		tokList;
	int 		i = -1;
	int 		j = -1;

	if(!pSdpStr||*pSdpStr=='\0') {
		return NULL;
	}
	if(zyStrlen(pSdpStr)>SDP_LINE_MAX_SIZE) {	// Yu-Shun add 12/8/2003
		return NULL;
	}
	
	while( 1 ) {
		i++;
		if( isSDPSpace( (pSdpStr+i) ) ) {
			continue;
		} else {
			break;
		}
	}
	p = (char*)(pSdpStr+i);
	if( *p=='\0' ) {
		return NULL;
	}

	if( i>0 ) {
		tokList = (SdpList)zyMalloc(sizeof(SdpListElm));
		tokList->pToken = strDup(" ");
		tokList->pNext = sdpTokParse(p);

		return tokList;
	}

	if( isSDPField(p) ) {
		j = 2;
	} 
	else if( isCRLF(p) ) {
		j = isCRLF(p);
	}
	else if( isSDPTokenSep(p) ) {
		j = 1;
	}
	else {
		while(1) {
			j++;
			if( p[j]!='\0' &&
				!isCRLF( (p+j) ) &&
				!isSDPField( (p+j) ) &&
				!isSDPTokenSep( (p+j) ) &&
				!isSDPSpace( (p+j) ) ) {
				continue;
			} else  {
				break;
			}
		}
	}

	temp = (char*)zyMalloc(sizeof(char)*(j+1));
	zyStrncpy(temp,p,j);	temp[j] = 0;

	tokList = (SdpList)zyMalloc(sizeof(SdpListElm));
	tokList->pToken = temp;
	tokList->pNext = sdpTokParse(p+j);
	
	return tokList;
}

int isSDPField(const char* field)
{
	int i = 0;

	if(!field) {
		return 0;
	}

	for( i=0; *SDP_FIELDS[i]!='\0'; i++) {
		if( strncmp(SDP_FIELDS[i],field,zyStrlen(SDP_FIELDS[i]))==0 ) {
			return 1;
		}
	}

	return 0;
}

int isCRLF(const char* str)
{
	if( !str ) {
		return 0;
	}
	if( zyStrlen(str)<1 ) {
		return 0;
	}

	if( zyStrlen(str)>1 && strncmp(SDP_CRLF,str,zyStrlen(SDP_CRLF))==0 ) {
		return 2;
	}
	if( zyStrlen(str)>=1 && ( strncmp("\r",str,1)==0 ||strncmp("\n",str,1)==0 )) {
		return 1;
	}
	
	return 0;
}

int isSDPTokenSep(const char* field)
{
	int i = 0;

	if(!field) {
		return 0;
	}

	for( i=0; *SDP_TOKEN_SEPERATOR[i]!='\0'; i++) {
		if( strncmp(SDP_TOKEN_SEPERATOR[i],field,zyStrlen(SDP_TOKEN_SEPERATOR[i]))==0 ) {
			return 1;
		}
	}

	return 0;
}

SdpSessMWI	sdpMWINew(void)
{
	SdpSessMWI	sdp;

	sdp = (SdpSessMWI)zyMalloc(sizeof(struct sdpSessMWIObj));

	sdp->sdp_flags_ = SDP_MWI_FLAG_NONE;

	sdp->status = 0;
	sdp->pAccount = NULL;
	zyMemset(&(sdp->pVoice), 0, sizeof(SdpMessageMWI));

	return sdp;
}

int sdpSessAddMWIBody(SdpSessMWI sdp, SdpMWIBodyType bodytype, void* str)
{
	if((sdp == NULL) || (str == NULL)) {
		return -1;
	}

	switch(bodytype){
		case Status:
			sdp->status = *(uint16 *)str;
			sdp->sdp_flags_ |= SDP_MWI_FLAG_STATUS;
			break;
		case Account:
			if (sdp->pAccount != NULL) {
				zyFree(sdp->pAccount);
				sdp->pAccount = NULL;
			}
			sdp->pAccount = zyMalloc(zyStrlen((char *)str));
			zyStrcpy(sdp->pAccount, (char *)str);
			sdp->sdp_flags_ |= SDP_MWI_FLAG_ACCOUNT;
			break;
		case Voicemessage:
			sdp->pVoice = *((SdpMessageMWI *)str);
			sdp->sdp_flags_ |= SDP_MWI_FLAG_VOICE;
			break;
		default:
			break;

	}

    return 0;
}

SdpMessageMWI sdpMWIMessageParse(char* pSdpStr)
{
	char*		p;
	int 		i = -1;
	SdpMessageMWI  tempMessage;
	int len = 0;

	zyMemset(&tempMessage, 0, sizeof(SdpMessageMWI));

	if(!pSdpStr || *pSdpStr=='\0') {
		return tempMessage;
	}

	len = zyStrlen(pSdpStr);
	pSdpStr[len] = '\0';

	//new message
	while( pSdpStr[i+1] != '\0' ) {
		i++;
		if( strncmp((pSdpStr+i), "/", 1) != 0) {
			continue;
		} else {
			break;
		}
	}

	p = pSdpStr;
	p[i] = '\0';
	tempMessage.newMessage = atoi(p);

	i++;
	pSdpStr = pSdpStr + i;
	len = len - i;
	if (len <= 0) {
		return tempMessage;
	}

	i = -1;

	//old message
	while( pSdpStr[i+1] != '\0' ) {
		i++;
		if( (strncmp((pSdpStr+i), "(", 1) != 0) && (strncmp((pSdpStr+i), "\n", 1) != 0) && (strncmp((pSdpStr+i), "\r", 1) != 0)) {
			continue;
		} else {
			break;
		}
	}

	p = pSdpStr;
	p[i] = '\0';
	tempMessage.oldMessage = atoi(p);

	i++;
	pSdpStr = pSdpStr + i;
	len = len - i;

	if (len <= 0) {
		return tempMessage;
	}

	i = -1;

	//new ungrent message
	while( pSdpStr[i+1]  != '\0' ) {
		i++;
		if( strncmp((pSdpStr+i), "/", 1) != 0) {
			continue;
		} else {
			break;
		}
	}

	p = pSdpStr;
	p[i] = '\0';
	tempMessage.newUrgent = atoi(p);

	i++;
	pSdpStr = pSdpStr + i;
	len = len - i;

	if (len <= 0) {
		return tempMessage;
	}

	i = -1;

	//new ungrent message
	while( pSdpStr[i+1]  != '\0' ) {
		i++;
		if( strncmp((pSdpStr+i), ")", 1) != 0) {
			continue;
		} else {
			break;
		}
	}

	p = pSdpStr;
	p[i] = '\0';
	tempMessage.oldUrgent = atoi(p);

	return tempMessage;


}

