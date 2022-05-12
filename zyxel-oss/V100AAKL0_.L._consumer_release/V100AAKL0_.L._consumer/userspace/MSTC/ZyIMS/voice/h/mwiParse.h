#ifndef __MWI_PARSE_H__
#define __MWI_PARSE_H__
#include  "voiceApp_iptk_MsgHandler.h"

#define SDP_LINE_MAX_SIZE 256

typedef struct sdpToken {
	char*			pToken;
	struct sdpToken*	pNext;
} SdpListElm;

typedef SdpListElm*		SdpList;

typedef struct sdpMessageMWI {
	uint32		flag;
	uint32		newMessage;
	uint32		oldMessage;
	uint32		newUrgent;
	uint32		oldUrgent;
} SdpMessageMWI;

/*---------------------------------------------
// SDP session for MWI //2004.6.23 kelly
*/
typedef struct sdpSessMWIObj*	SdpSessMWI; //2004.6.23 kelly
struct sdpSessMWIObj {
	uint16		sdp_flags_;		/* which content exists? */
	/*kelly: need to modify. Maybe new items add as follows */
	uint16		status;			/* yes/no: message exists? */
	char*		pAccount;		/* Voice Mail accounts */
	SdpMessageMWI	pVoice;			/* numbers of new/old message */
};

/*---------------------------------------------
// NOTIFY's SDP message
*/

#define SDP_MWI_STATUS_TOKEN			"Messages-Waiting: "
#define SDP_MWI_ACCOUNT_TOKEN		"Message-Account: "
#define SDP_MWI_VOICE_TOKEN			"Voicemail: "
#define SDP_MWI_VOICE_TOKEN2			"Voice-Message: "

#define SDP_MWI_FLAG_NONE				0x0
#define SDP_MWI_FLAG_STATUS			(1<<0)
#define SDP_MWI_FLAG_ACCOUNT			(1<<1)
#define SDP_MWI_FLAG_VOICE			(1<<2)


/* support MWI fields */
typedef enum {		
	Status = 0,
	Account,
	Voicemessage
} SdpMWIBodyType ;   

#define SDP_CRLF			"\r\n"

/**************************************************************
* sdpLineParse :
*	Parse text into Line list.
*
**************************************************************/
SdpList sdpLineParse(const char* pSdpStr);
/**************************************************************
* sdpTokParse :
*	Parse text into SDP token list.
*
**************************************************************/
SdpList sdpTokParse(const char* pSdpStr);

/* strDup, use malloc() to duplicate string s. 
   User should use free() to deallocate memory. */
char*	strDup(const char* s);

SdpSessMWI	sdpMWINewFromText(const char* sdpText);
SdpMessageMWI sdpMWIMessageParse(char* pSdpStr);
SdpSessMWI	sdpMWINew(void);
uint32 sdpMWIGetNewVoiceMailCount(SdpSessMWI sdpMWI);

void	sdpMWIFree(SdpSessMWI this_);
void sdpListFree(SdpList tokList);

#endif  /*__MWI_PARSE_H__*/

