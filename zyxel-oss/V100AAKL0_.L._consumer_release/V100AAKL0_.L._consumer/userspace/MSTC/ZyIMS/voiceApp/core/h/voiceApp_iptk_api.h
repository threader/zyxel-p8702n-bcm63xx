#ifndef __VOICEAPP_IPTK_MAIN_H__
#define __VOICEAPP_IPTK_MAIN_H__

#include "switch.h"	

#ifdef NULL
#undef NULL
#endif
#define NULL                    0L

/* Mitrastar,Jason, 20120822
* The register status checking interval is 2s.
* And the MAX SIP REGISTER transaction is 32s.
* So We set MAX_REGISTER_STATUS_CHECK_RETRY to 17. 
*/
#define MAX_REGISTER_STATUS_CHECK_RETRY 17

#if 1	/*Jason ,[SPRID # 090622474]*/		

typedef struct oneShotEventRec_s
{
	uint8 needRecover;
	uint16 OneShotEventFlag ;
}oneShotEventRec_t;
extern oneShotEventRec_t* oneShotEvent;
bool setServiceParamForOneShotEvent(uint16 VpItr, uint16 SipAccountId ,uint16 dialFlag);
bool recoverServiceParamForOneShotEvent(uint16 SipAccountId);
#endif

icf_uint32_t  voiceAppIptkIpcSend(icf_void_t *p_msg,icf_uint32_t buf_len);
bool iptkCfgInit(int timer_id, void	*vp) ;
bool iptkCfgReset(void);
bool iptkTraceInit(void);
bool mmCfgInit(void);
/* Application setup APIs */
bool voiceAppAddNewApp(void);
bool voiceAppRemoveApp(void);
/* Application configuration APIs */
bool voiceAppSetTransportParams(uint16 SipAccountId);
bool voiceAppDelTransportParams(uint16 SipAccountId);
bool voiceAppSetRegistrationParams(uint16 SipAccountId);
bool voiceAppDelRegistrationParams(uint16 SipAccountId);
bool voiceAppSetSelfId(uint16 SipAccountId);
bool voiceAppDelSelfId(uint16 SipAccountId);
bool voiceAppSetCallParam(uint16 profileId);
bool voiceAppSetServiceParam(uint16 SipAccountId);
bool voiceAppSetLineParam(uint16 SipAccountId);
/* Configuration status APIs */
bool voiceAppGetSysSettings(void);
bool voiceAppGetLineSettings(uint16 lineId);
/* Report trace APIs */
bool voiceAppSetTrace(uint8 setTraceFlag);
/* Status check APIs */
bool voiceAppGetRegisterStatus(uint16 SipAccountId);
/* Control plane APIs */
bool voiceAppSendRegister(uint16 SipAccountId);
bool voiceAppSendDeRegister(uint16 SipAccountId);
/*Jason ,[SPRID # 090622474]*/	
bool voiceAppCreateCallwithDialflag(uint16 callId, uint16 SipAccountId, uint16 VpItr, char *dial_digits,uint16 dialFlag);
bool voiceAppLocalAlerted(uint16 callId, uint16 SipAccountId, uint16 earlyMediaSuccess);
bool voiceAppMediaConnect(uint16 callId);
bool voiceAppTermCallResp(uint16 callId);
bool voiceAppConnect(uint16 callId);
bool voiceAppSubscribe(uint16 SipAccountId);
bool voiceAppUnSubscribe(uint16 SipAccountId);
bool voiceAppStartFAX(uint16 callId, uint16 SipAccountId, uint8 fax_type);
bool voiceAppStopFAX(uint16 callId);
bool voiceAppCallHold(uint16 callId);
bool voiceAppCallResume(uint16 callId);
bool voiceAppCallModifyCfm(uint16 callId);
bool voiceAppInfo(uint16 callId, uint8 *dialDigits);
bool voiceAppInfoCfm(uint16 callId);

#if HEARTBEAT_MECHANISM_ENABLE 
bool voiceAppSendOptions(uint16 VpItr , uint16 SipAccountId);
#endif
bool mmCfgSelfIp(uint32 SipAccountId);
bool mmCfgCodecInfo(uint32 SipAccountId);
bool mmCfgPortRange(uint32 SipAccountId);

bool mmCfgMedia(void);
bool mmCfgJbmMgmt(void);
bool mmCfgMaxEarpVol(void);
bool mmCfgMaxSpeakerVol(void);
bool mmCfgDigitTrans(void);
bool mmCfgRemoteVideoDisplay(void);
bool mmCfgPreviewVideoDisplay(void);
bool mmCfgMaxMicGain(void);
char* ReplaceHashwithASCII(char *Source);
#endif /* __VOICEAPP_IPTK_MAIN_H__ */

