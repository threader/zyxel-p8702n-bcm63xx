#ifndef __APP_USER_IPTK_HANDLE_H__
#define __APP_USER_IPTK_HANDLE_H__

#include "switch.h"	

#if HEARTBEAT_MECHANISM_ENABLE
 
#define HEARTBEATCALLIDBASE	0x7ff0

typedef struct heartbeatRec_st{
	uint8	hearbeatOngoing ;
	/*record the ongoing timer Id for the profile*/
	int		timerID;
}heartbeatRec_t;

#endif

/*Enhance Debug log for ZyIMS,Mitrastar,Jason,2013.01.29*/
typedef struct {
	uint32  error_id;	
	char error_str[ICF_MAX_STR_LEN];
}voiceAppICF_Err_ind_t;

typedef struct {
	uint8  record_type;	
	uint8 cause;
	char fqdn[ICF_MAX_STR_LEN];
	char ip[ICF_MAX_STR_LEN];
}voiceAppICF_Dnsmapchg_ind_t;

typedef struct {
	uint16  line_id;	
	uint8   request_type;	
	uint16  error_cause ;
	uint16  response_code;
	time_t  timerec;
}voiceAppICF_Fail_rereg_t;

typedef struct {
	uint8 failreRegNext;
	uint8 errIndNext;
	uint8 DNSMapChgNext;	
	voiceAppICF_Fail_rereg_t	lastReRegErr[VOICEAPP_ICF_MAX_REREG_REC];
	/*ICF_REPORT_ERROR_IND*/
	voiceAppICF_Err_ind_t lastErrInd[VOICEAPP_ICF_MAX_ERR_REC];
	/*DNS_MAPPING_CHANGE_IND*/
	voiceAppICF_Dnsmapchg_ind_t lastDNSMapChgInd[VOICEAPP_ICF_MAX_DNS_REC];	
}voiceAppICFErrRec_t;

bool voiceAppIPTKMsgProcess(uint8 *recvBuf, int recvLen);

#endif  /*__APP_USER_IPTK_HANDLE_H__*/
