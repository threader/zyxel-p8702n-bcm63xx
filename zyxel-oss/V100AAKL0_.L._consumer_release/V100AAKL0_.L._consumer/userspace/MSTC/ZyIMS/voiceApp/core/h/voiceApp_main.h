#ifndef __APP_USER_H__
#define __APP_USER_H__

void voiceAppTask(void);
bool ccmIpcInit(void);
bool phoneIpcInit(void);
bool voipActivate(void);
bool voipDeactivate(void);
/*Add some debug message in voiceApp for future debugging.,Mitrastar, Jason,20120913*/
void VoiceAppShowStatus(void);

/*____________________________________________________________________________*/
/*	type definition                                                                                                             */
/*____________________________________________________________________________*/
typedef struct voiceApp_s {
	int appId;
	int voiceApp_iptk_fd;
	int voiceApp_ccm_fd;
	int ccm_fd;
	int phone_fd;
	int voiceApp_mm_fd;
	int config_fd;
}voiceApp_t;

#define WEB_SIP_STATUS_PHONENUMB_LENGTH 32+1
typedef struct webSipStatus_s{
	time_t lastRegisteration;
	char lastIncomingNumber[WEB_SIP_STATUS_PHONENUMB_LENGTH];
	char lastOutgoingNumber[WEB_SIP_STATUS_PHONENUMB_LENGTH];
}webSipStatus_t;

typedef struct voiceAppRegTimer_s{
	tm_id_t	regRetryTimer;
	tm_id_t	subscribeTimer;	
}voiceAppRegTimer_t;


/*____________________________________________________________________________*/
/*	data definition                                                                                                             */
/*____________________________________________________________________________*/
/* global socket fd for voiceApp IPC */
voiceApp_t globalVoiceApp;
extern int voiceAppDebug;
/* Redirect Debug message for ZyIMS to setting direction., __P876__, Klose, 20130128 */
extern uint8 log_dest;
extern bool VoIPIsActive;
extern uint8 *ICFLineInitDone;
extern voiceAppRegTimer_t* reg_timer;

#define VOICEAPP_IPTK_CHANNEL_PATH "/var/voiceApp_iptk.chanl"
#define VOICEAPP_CCM_CHANNEL_PATH "/var/voiceApp_ccm.chanl"
#define CCM_CHANNEL_PATH "/var/ccm.chanl"
#define PHONE_CHANNEL_PATH "/var/phone.chanl"
#define ICF_ES_CHANNEL_PATH "/var/iptk_es.chanl"
#define CONFIG_CHANNEL_PATH "/var/config.chanl"
#define VOICEAPP_MM_CHANNEL_PATH "/var/voiceApp_mm.chanl"
#define MM_CHANNEL_PATH "/var/icf_mm.chanl"

#define MAX_MTU_SIZE    20000

#define VOICEAPP_DBG(fs)						if (voiceAppDebug & 1)	{	ZyPrintf((fs));		\
																		ZyPrintf("\n\r"); }
#define VOICEAPP_DBG1(fs, v1)				if (voiceAppDebug & 1)	{	ZyPrintf(fs);			\
																		ZyPrintf("0x%x", v1); \
																		ZyPrintf("\n\r"); }
#define VOICEAPP_DBG2(fs, v1, f2, v2)		if (voiceAppDebug & 1)	{	ZyPrintf(fs);			\
																		ZyPrintf("0x%x", v1); \
																		ZyPrintf(f2); 		\
																		ZyPrintf("0x%x", v2); \
																		ZyPrintf("\n\r"); }
#define VOICEAPP_PRINTF		if (voiceAppDebug & 1)	ZyPrintf

#endif

