#ifndef __CALL_MGNT_INCLUDE__
#define __CALL_MGNT_INCLUDE__

#include "event.h"
#include "sys_msg.h"
#include "tr104_object.h"

/*____________________________________________________________________________*/
/*	macro                                                                                                                         */
/*____________________________________________________________________________*/
#ifdef CENTRALIZE_LOG
/* The definition for the CCM centralize log */
#define CCM_LOGS_TYPE_OUTCALL_START			0
#define CCM_LOGS_TYPE_INCALL_START			1
#define CCM_LOGS_TYPE_OUTCALL_ESTABLISH		2
#define CCM_LOGS_TYPE_INCALL_ESTABLISH		3
#define CCM_LOGS_TYPE_END_CALL				4
#define CCM_LOGS_TYPE_ISDN_CCBS_START		5
#define CCM_LOGS_TYPE_ISDN_CCBS_ESTABLISH	6
#endif
/* The definition for the call feature number */
/* for PHONE CONFIG 
if  add call feature define here , please also check
1. modify the CALL_FEATURE_NUMBERS value
2. modify the call feature table for TR104  
*/
#define CALL_FEATURE_IVR					0
#define CALL_FEATURE_INTERNAL_CALL		1
#define CALL_FEATURE_CID_DISPLAY	2		/*enable the caller id for outgoing call*/
#define	CALL_FEATURE_CID_HIDDEN		3		/*hidden the caller id for outgoing call*/
#define CALL_FEATURE_CALL_TRANSFER		4
#define CALL_FEATURE_PARK				5  /* CALL_PARK_PICKUP */
#define CALL_FEATURE_PARK_RETRIEVE		6  /* End - CALL_PARK_PICKUP */
#define CALL_FEATURE_RETURN			7		/* redial the last incoming call */
#define CALL_FEATURE_CALL_WAITING_ENABLE		8	/* enable call waiting */
#define CALL_FEATURE_CALL_WAITING_DISABLE		9	/* disable call waiting*/
#define CALL_FEATURE_UNCONDITIONAL_FORWARD_ENABLE	10	/*active call forwarding unconditional */
#define CALL_FEATURE_UNCONDITIONAL_FORWARD_DISABLE	11	/*deactive call forwarding unconditional */
#define CALL_FEATURE_NOANSWER_FORWARD_ENABLE		12	/*active call forwarding no answer */
#define CALL_FEATURE_NOANSWER_FORWARD_DISABLE		13	/*deactive call forwarding no answer */
#define CALL_FEATURE_BUSY_FORWARD_ENABLE			14	/*active call forwarding on busy */
#define CALL_FEATURE_BUSY_FORWARD_DISABLE			15	/*deactive call forwarding on busy */
#define CALL_FEATURE_ONE_SHOT_CID_HIDDEN			16
#define CALL_FEATURE_ONE_SHOT_CID_DISPLAY			17
#define CALL_FEATURE_ONE_SHOT_CALL_WAITING_DISABLE	18
#define CALL_FEATURE_ONE_SHOT_CALL_WAITING_ENABLE	19
#define CALL_FEATURE_ENABLE_WLAN					20	/* enable wlan */
#define CALL_FEATURE_DISABLE_WLAN					21	/* disable wlan */
#define CALL_FEATURE_CALL_PICKUP					22	/* answer someone else¡¦s telephone call. */
#define CALL_FEATURE_ENABLE_DND						23	/* ENABLE DO NOT DISTURB	*/
#define CALL_FEATURE_DISABLE_DND					24	/* DISABLE DO NOT DISTURB	*/
#define CALL_FEATURE_CCBS_DEACTIVATE		25	/* CCBS deactivate*/
#define CALL_FEATURE_OUTGOING_SIP			26	/* Use specific SIP for outgoing call, aircheng add */
#define CALL_FEATURE_NUMBERS			27

//Other feature
#define CALL_FEATURE_THREE_WAY_CONFERENCE	CALL_FEATURE_NUMBERS+1
#define CALL_FEATURE_TRANSFER_TRANSFEREE	CALL_FEATURE_NUMBERS+2
#define CALL_FEATURE_TRANSFER_TARGET	CALL_FEATURE_NUMBERS+3

#define CALL_FEATURE_MAX_KEY_LEN		42	/*follow TR104, length is 42 */

/* The definition for the voiceDspCh->dspPortType & ccmConn->origPortType & ccmConn->peerPortTye*/
#define DSP_FXS_PORT			0
#define DSP_FXO_PORT			1
#define DSP_FXO_LIFELINE		2
#define SIP_PORT				3
#define ZYXEL_PHONE_PORT		4
#define ISDN_NT_PORT			5
#define ISDN_TE_PORT			6
#define DSP_DECT_PORT		7
#define MAX_PORT_ROUTE		8


/* The definition for the ccmConn_t->connStatus */
#define CALL_CONN_ST_IDLE				0
#define CALL_CONN_ST_CONNOUT			1
#define CALL_CONN_ST_CONNIN				2
#define CALL_CONN_ST_CONNRDY			3
#define CALL_CONN_ST_CONNREL			4
#define CALL_CONN_ST_INTERACTIVE		5
#define CALL_CONN_ST_INTERCONN			6

/* The definition for the ccmConn_t->flags */
#define CALL_CONN_FLAG_PREASSIGN		(1<<0)
#define CALL_CONN_FLAG_ASSIGN			(1<<1)
#define CALL_CONN_FLAG_PREFIX			(1<<2)
#define CALL_CONN_FLAG_ALERT			(1<<3)
#define CALL_CONN_FLAG_REGISTER			(1<<4)
#define CALL_CONN_FLAG_PEERLOCAL		(1<<5)  //Local Peer Call Conference
#define CALL_CONN_FLAG_CALLPARK			(1<<6)  /* CALL_PARK_PICKUP */
#define CALL_CONN_FLAG_CALLPICKUP		(1<<7)  /* End - CALL_PARK_PICKUP */
#ifdef VISUAL_MWI
#define CALL_CONN_FLAG_VMWI			(1<<8)
#endif
#if 1 /*Jason , [SPRID # 090624637]*/ 
#define CALL_CONN_FLAG_HOLD		(1<<9)
#endif
#if CCBS_SUPPORT
#define CALL_CONN_FLAG_WEBDIAL		(1<<10)	
#endif
/* The definition for the ccmConn_t->connType*/
#define CCM_CONN_TYPE_FAIL				0
#define CCM_CONN_TYPE_CALLER_CALLER	1
#define CCM_CONN_TYPE_CALLEE_CALLEE		2
#define CCM_CONN_TYPE_CALLER_CALLEE		3
#define CCM_CONN_TYPE_CALLEE_CALLER		4
#define CCM_CONN_TYPE_WAIT				5  /* CALL_PARK_PICKUP */
#define CCM_CONN_TYPE_NO_CALL			6  /* End - CALL_PARK_PICKUP */

/* The definition for the ccmConn_t->connRole */
#define CCM_CONN_ROLE_UNDEF				0
#define CCM_CONN_ROLE_ORIGINAL			1
#define CCM_CONN_ROLE_PEER				2


#define CCM_CALL_TRANSFER			0x01
#define CCM_CONFERENCE_CALL			0x02


/* The definition for the dialDigits_t */
#define MAX_DIAL_STRING_LENGTH	(128+1)		/* The same as "POTS_PORT_NAME_LEN" at spt.h */
#define MAX_CNAME_LENGTH			50			/* The same as "GG_CID_MAX_CNAME_LENGTH" */

/* The definition for the dialInform.dialDigitNumber.dialDigitFlags */
#define DIAL_DIGITAL_FORCE_PSTN		(1<<0)
#define DIAL_DIGITAL_CALLERID_HIDDEN	(1<<1)
#define DIAL_DIGITAL_ONE_SHOT_CID_HIDDEN		(1<<2)
#define DIAL_DIGITAL_ONE_SHOT_CID_DISPLAY		(1<<3)
#define DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_ENABLE	(1<<4)
#define	DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_DISABLE	(1<<5)



/*____________________________________________________________________________*/
/*	event                                                                                                                          */
/*____________________________________________________________________________*/
#define	EVT_CCM_ASSIGN_REQ						EVENT( EVT_VOIP_CCM, 0x00)
#define	EVT_CCM_ASSIGN_CONF						EVENT( EVT_VOIP_CCM, 0x01)
#define	EVT_CCM_ASSIGN_IND							EVENT( EVT_VOIP_CCM, 0x02)
#define	EVT_CCM_ASSIGN_FAIL						EVENT( EVT_VOIP_CCM, 0x03)
#define	EVT_CCM_DIAL_REQ							EVENT( EVT_VOIP_CCM, 0x04)
#define	EVT_CCM_DIAL_IND							EVENT( EVT_VOIP_CCM, 0x05)
#define	EVT_CCM_REL_REQ							EVENT( EVT_VOIP_CCM, 0x06)
#define	EVT_CCM_REL_IND							EVENT( EVT_VOIP_CCM, 0x07)
#define	EVT_CCM_REJ_REQ							EVENT( EVT_VOIP_CCM, 0x08)
#define	EVT_CCM_REJ_IND							EVENT( EVT_VOIP_CCM, 0x09)
#define	EVT_CCM_ALERT_REQ							EVENT( EVT_VOIP_CCM, 0x0A)
#define	EVT_CCM_ALERT_IND							EVENT( EVT_VOIP_CCM, 0x0B)
#define	EVT_CCM_CONN_REQ							EVENT( EVT_VOIP_CCM, 0x0C)
#define	EVT_CCM_CONN_IND							EVENT( EVT_VOIP_CCM, 0x0D)
#define	EVT_CCM_CONN_ACK							EVENT( EVT_VOIP_CCM, 0x0E)
#define	EVT_CCM_CONN_CONF							EVENT( EVT_VOIP_CCM, 0x0F)
#define	EVT_CCM_CONN_REQ_TO						EVENT( EVT_VOIP_CCM, 0x10)
#define	EVT_CCM_DISC_REQ							EVENT( EVT_VOIP_CCM, 0x11)
#define	EVT_CCM_DISC_IND							EVENT( EVT_VOIP_CCM, 0x12)
#define	EVT_CCM_DISC_ACK							EVENT( EVT_VOIP_CCM, 0x13)
#define	EVT_CCM_REM_REQ							EVENT( EVT_VOIP_CCM, 0x14)
#define	EVT_CCM_DIAL_DIGIT							EVENT( EVT_VOIP_CCM, 0x15)
#define	EVT_CCM_REG_SUCC							EVENT( EVT_VOIP_CCM, 0x16)
#define	EVT_CCM_UNREG_SUCC						EVENT( EVT_VOIP_CCM, 0x17)
#define	EVT_CCM_MWI								EVENT( EVT_VOIP_CCM, 0x18)
#define	EVT_CCM_NO_MWI							EVENT( EVT_VOIP_CCM, 0x19)
#define	EVT_CCM_FAX_RELAY							EVENT( EVT_VOIP_CCM, 0x1A)
#define	EVT_CCM_MEDIA_CHANGE						EVENT( EVT_VOIP_CCM, 0x1B)
#define	EVT_CCM_ONHOLD_REQ						EVENT( EVT_VOIP_CCM, 0x1C)
#define	EVT_CCM_ONHOLD_CONF						EVENT( EVT_VOIP_CCM, 0x1D)
#define	EVT_CCM_UNHELD_REQ						EVENT( EVT_VOIP_CCM, 0x1F)
#define	EVT_CCM_UNHELD_CONF						EVENT( EVT_VOIP_CCM, 0x20)
#define	EVT_CCM_UNHELD_IND						EVENT( EVT_VOIP_CCM, 0x21)
#define	EVT_CCM_UNATTEND_CALL_TRANS_REQ			EVENT( EVT_VOIP_CCM, 0x22)
#define	EVT_CCM_ATTEND_CALL_TRANS_REQ			EVENT( EVT_VOIP_CCM, 0x23)
#define	EVT_CCM_CALL_TRANS_IND					EVENT( EVT_VOIP_CCM, 0x24)
#define	EVT_CCM_CALL_TRANS_RSP					EVENT( EVT_VOIP_CCM, 0x25)			/* Send out the result */
#define	EVT_CCM_ATTEND_CALL_TRANS_REPLACE		EVENT( EVT_VOIP_CCM, 0x26)			/* For the Transfer-Target */
#define	EVT_CCM_CALL_TRANS_UPDATE					EVENT( EVT_VOIP_CCM, 0x27)			/* Receive the result */
#define	EVT_PHONE_ZYXEL_ONHOOK					EVENT( EVT_VOIP_CCM, 0x28)
#define	EVT_PHONE_ZYXEL_REMOVE					EVENT( EVT_VOIP_CCM, 0x29)			/* Send the ZyPHONE remove event */
#define	EVT_CCM_CHANGE_ASSIGN						EVENT( EVT_VOIP_CCM, 0x2A)
#define	EVT_CCM_FAX_T38							EVENT( EVT_VOIP_CCM, 0x2B)
#define	EVT_CCM_CONFCALL_MEETROOM_REQ			EVENT( EVT_VOIP_CCM, 0x2C)
#define	EVT_CCM_CONFCALL_MEETROOM_CONF			EVENT( EVT_VOIP_CCM, 0x2D)
#define	EVT_CCM_CONFCALL_MEETROOM_FAIL			EVENT( EVT_VOIP_CCM, 0x2E)
#define	EVT_CCM_CONFCALL_JOIN_MEMBER_REQ			EVENT( EVT_VOIP_CCM, 0x2F)
#define	EVT_CCM_CONFCALL_JOIN_MEMBER_CONF		EVENT( EVT_VOIP_CCM, 0x30)
#define	EVT_CCM_3WAYCALL_REMOVE_BASEPHONE		EVENT( EVT_VOIP_CCM, 0x31)
#define	EVT_CCM_3WAYCALL_REMOVE_NEWPHONE		EVENT( EVT_VOIP_CCM, 0x32)
#define	EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_REQ	EVENT( EVT_VOIP_CCM, 0x33)
#define	EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_REQ		EVENT( EVT_VOIP_CCM, 0x34)
#define	EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_CONF	EVENT( EVT_VOIP_CCM, 0x35)
#define	EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_CONF	EVENT( EVT_VOIP_CCM, 0x36)
#define	EVT_CCM_3WAYCALL_MEETROOM_DESTROY						EVENT( EVT_VOIP_CCM, 0x37)
#define	EVT_CCM_3WAYCALL_SWITCH_ACTIVE_PHONE					EVENT( EVT_VOIP_CCM, 0x38)
#define	EVT_CCM_CALL_PARK							EVENT( EVT_VOIP_CCM, 0x39)			/* CALL_PARK_PICKUP */
#define	EVT_CCM_CALL_RESTART						EVENT( EVT_VOIP_CCM, 0x3A)
#define	EVT_CCM_SIPTOPSTN							EVENT( EVT_VOIP_CCM, 0x3B) //SIP->PSTN
#define	EVT_CCM_SDP_ALERT_IND						EVENT( EVT_VOIP_CCM, 0x3C)
#define	EVT_CCM_CONFERENCE_IND					EVENT( EVT_VOIP_CCM, 0x3D)
#define	EVT_CCM_CONFERENCE_CONF					EVENT( EVT_VOIP_CCM, 0x3E)
#ifdef VISUAL_MWI
#define EVT_CCM_NEW_VOICE_MESSAGE		EVENT( EVT_VOIP_CCM, 0x3F)
#define EVT_CCM_NO_NEW_VOICE_MESSAGE	EVENT( EVT_VOIP_CCM, 0x40)
#endif
#if CCBS_SUPPORT
#define	EVT_CCM_WEBDIAL_CHECK			EVENT( EVT_VOIP_CCM, 0x57)
#define	EVT_CCM_WEBDIAL_CONF				EVENT( EVT_VOIP_CCM, 0x58)
#define	EVT_CCM_WEBDIAL_REMOVE			EVENT( EVT_VOIP_CCM, 0x59)
#define	EVT_CCM_WEBDIAL_FAIL				EVENT( EVT_VOIP_CCM, 0x5A)
#define	EVT_CCM_WEBDIAL_REDIAL			EVENT( EVT_VOIP_CCM, 0x5B)
#define	EVT_CCM_WEBDIAL_REDIAL_STOP		EVENT( EVT_VOIP_CCM, 0x5C)
#endif


/*____________________________________________________________________________*/
/*	Data declaration                                                                                                          */
/*____________________________________________________________________________*/
#define CCM_FSM_DBG(fs)						if (ccmDebug & 1)	{	dbg_pline_1((fs));			\
																dbg_pline_1("\n\r"); }
#define CCM_FSM_DBG1(fs, v1)				if (ccmDebug & 1)	{	dbg_plinel_1((fs), (v1));	\
																dbg_pline_1("\n\r"); }
#define CCM_FSM_DBG2(fs, v1, f2, v2)			if (ccmDebug & 1) {	dbg_plinel_1((fs), (v1));	\
																dbg_plinel_1((f2), (v2));	\
																dbg_pline_1("\n\r"); }
#define CCM_FSM_DBG3(fs, v1, f2, v2, f3, v3)	if (ccmDebug & 1) {	dbg_plinel_1((fs), (v1));	\
																dbg_plinel_1((f2), (v2));	\
																dbg_plinel_1((f3), (v3));	\
																dbg_pline_1("\n\r"); }
#define CCM_PRINTF	if (ccmDebug & 1)	ZyPrintf

extern char callFeatureKey[CALL_FEATURE_NUMBERS][CALL_FEATURE_MAX_KEY_LEN];


/*____________________________________________________________________________*/
/*	type definition                                                                                                             */
/*____________________________________________________________________________*/
typedef struct dialDigitNum_s
{
	uint8 dial_digits[MAX_DIAL_STRING_LENGTH];
	char force_sipUri[SIP_DOMAIN_NAME_LEN];
	uint16 dial_durations[MAX_DIAL_STRING_LENGTH];
	uint32 dial_timeStampOffsets[MAX_DIAL_STRING_LENGTH];
	uint8 current_length;		/* The dial_digits real length */
	uint8 dialout_length;		/* For the FXO Dialout counter */
	uint16 dialDigitFlags;		/* Dial Digit Flags */
	char dateTime[8];		/* date/time */
	char cname[MAX_CNAME_LENGTH];
} dialDigitNum_t;

typedef struct dialInfor_s
{
	dialDigitNum_t	dialDigitNumber;
	uint16 prevLogicId;		/* From which prevLogicId */
} dialInfor_t;

typedef struct dialDigitBuf_s{
	uint8 digitStr;
	uint8 volume;
	uint16 duration;
	uint32 timeStampOffset;
}dialDigitBuf_t;

typedef struct bundleCcm_s
{
	struct ccmConn_s *bundleCcmPrev;
	struct ccmConn_s *bundleCcmNext;
} bundleCcm_t;

typedef struct ccmConn_s
{
	struct ccmConn_s *prev; 				/* Linked list pointers 	*/
	struct ccmConn_s *next;
	struct ccmConn_s *peerCcmConn;		/* peer ccmConn Linked List pointers*/
	struct bundleCcm_s bundleCcmList;		/* Other bundle ccmConn Link List pointers */
	struct callTransfer_s *callTransferCB;	/* Call transfer control block */
	struct conferencecall_s *ConfCallCB;	/* Call conference control block */	/* CALL_CONFERENCE */
	struct ccmPhoneFeatureCb_s *phoneFeatureCb;
	struct ccmCallRouteCb_s *callRouteCb;
	#if CCBS_SUPPORT
	struct webdial_s *WebDialCB;			/*Web Phone Dial block*/
	#endif
	uint16 connSt;						/* connection status */
	uint16 origPortId;						/* original Port-Id */
	uint16 origPortType;					/* original Port-Type */
	uint16 callFeatures;					/* CallFeatures */
	uint16 origFuncId;					/* original Signal */
	uint32 flags;
	dialInfor_t dialInform;					/* Record the dialOut Number */
	uint8   connRole;						/* 0:Not define, 1:original, 2:peer */
	uint8   connType;
	uint8   SipFxsMap;					/* Phone index*/
	#ifdef CALLHISTORY_SUPPORT
	uint16 webCallHistoryIndex;	/*[0, MAX_CALL_HISTORY_ENTRY_INCALLS-1] - in calls index range , [MAX_CALL_HISTORY_ENTRY_INCALLS, (MAX_CALL_HISTORY_ENTRY_INCALLS+MAX_CALL_HISTORY_ENTRY_OUTCALLS)-1] - out call index range*/	
	#endif
	#if 1	/* aircheng add for outgoing sip */
	int outgoingSipAccountId;				/* For outgoing sip phone feature to record outgoing call through specific sip account */
	#endif
} ccmConn_t;

typedef struct ccmPhoneFeatureCb_s
{
	struct ccmPhoneFeatureCb_s *prev;
	struct ccmPhoneFeatureCb_s *next;
	void (*funcPtr)(ccmConn_t *ccmConn, int *retCode, int *exitFlag);
}ccmPhoneFeatureCb_t;

typedef struct ccmCallRouteCb_s
{
	struct ccmCallRouteCb_s *prev;
	struct ccmCallRouteCb_s *next;
	void (*funcPtr)(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag);
}ccmCallRouteCb_t;

typedef struct ccmConnCb_s
{
	ccmConn_t* fxsCcmConn;				/* FXS CCM Conn */
	ccmConn_t* fxoCcmConn;				/* FXO CCM Conn */
	ccmConn_t* sipCcmConn;				/* SIP CCM Conn */
	ccmConn_t* zyxelCcmConn;			/* ZYXEL_PHONE CCM Conn */
} ccmConnCb_t;

#define FAXValue 0xBABEFACE
typedef struct T38Info_s
{
	uint32 FAXFlag;
	uint16 MaxRate;
	uint16 RateMgnt;
} T38Info_t;

typedef int (*associateTable_t)(ccmConn_t *ccmConn_p, int phoneFeature);

#define ERROR_CCM_CONN_FIND		-1

#define WEB_CALLSTATUS_PHONENUM_LENGTH 32+1
#define WEB_CALLSTATUS_CODEC_LENGTH 10+1

#if 1
typedef struct sipCallStatus_s{
	uint8 status;
	time_t callStartTime;
	char selfNumber[WEB_CALLSTATUS_PHONENUM_LENGTH];
	char peerNumber[WEB_CALLSTATUS_PHONENUM_LENGTH];
	char codec[WEB_CALLSTATUS_CODEC_LENGTH];	
}sipCallStatus_t;
#define MAX_SIP_CALL_STATUS 4
#endif
/*____________________________________________________________________________*/
/*	Data declaration                                                                                                          */
/*____________________________________________________________________________*/
extern ccmConnCb_t ccmConnCb;
extern int ccmDebug;

/*____________________________________________________________________________*/
/*	function                                                                                                                      */
/*____________________________________________________________________________*/
int callCtrlInit(void);
void initCcmConn(void);
int ccm_associateConn(evtMsg_t*, ccmConn_t*);
int associateFxs2FxoLife(ccmConn_t* ccmConn_p, int fxsTranslate);
int associateFxs2Sip(ccmConn_t* ccmConn_p, int phoneFeature);
int associateFxs2Fxo(ccmConn_t* ccmConn_p, int phoneFeature);
int associateSip2Fxs(ccmConn_t* ccmConn_p, int phoneFeature);
int associateFxoLife2Fxs(ccmConn_t* ccmConn_p, int phoneFeature);
int associateFxo2Fxs(ccmConn_t* ccmConn_p, int phoneFeature);
int associateFxs2Fxs(ccmConn_t* ccmConn_p, int phoneFeature);
int associateFxs2ZyPhone(ccmConn_t* ccmConn_p, int phoneFeature);
int associateSip2ZyPhone(ccmConn_t* ccmConn_p,int phoneFeature);
int associateZyPhone2Fxs(ccmConn_t* ccmConn_p,int phoneFeature);
int associateZyPhone2Sip(ccmConn_t* ccmConn_p,int phoneFeature);
int ccm_bundleConn(ccmConn_t* ccmConn_p, ccmConn_t* peerCcmConn_p, uint16 connStatus, uint8 connType);
int ccm_bundleForkConn(ccmConn_t* ccmConn_p, ccmConn_t* peerCcmConn_p, ccmConn_t* prePeerCcmConn_p, uint16 connStatus, uint8 connType);
int ccm_releaseConn(ccmConn_t* ccmConn_p);
int ccm_releaseOtherChann(ccmConn_t* ccmConn_p, ccmConn_t* ccmBundleHead);
int createPhoneFeatureCB(ccmConn_t*, dialInfor_t*, int);
void constructCallFeatureCb(ccmConn_t* ccmConn_p);
void createCallFeatureCb(ccmConn_t *ccmConn_p, void (*func_p));
void constructCallRouteCb(ccmConn_t* ccmConn_p);
void createCallRouteCb(ccmConn_t *ccmConn_p, void (*func_p));
#ifdef DIAL_PLAN
void ccmDialPlanRoute(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag);
#endif
void checkPSTNPhoneBook(	ccmConn_t *ccmConn, int *retCode, int *exitFlag);
void checkPhoneFeature(ccmConn_t *ccmConn, int *retCode, int *exitFlag);
#ifdef SPEED_DIAL
void checkSpeedDialFeature(ccmConn_t *ccmConn, int *retCode, int *exitFlag);
void checkSpeedDialPhBook(dialDigitNum_t *dialDigitNum_p);
#endif
void checkDefaultFeature(ccmConn_t *ccmConn, int *retCode, int *exitFlag);
void ccmDefaultRoute(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag);
void ccmForceToPstn(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag);
void ccmPhoneFeatureRoute(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag);
#ifdef CENTRALIZE_LOG
void ccmCentralizeLog(ccmConn_t *ccmConn_p, uint16 logsType);
#endif
#ifdef DO_NOT_DISTURB
void checkDNDFeature(ccmConn_t *ccmConn, int *retCode, int *exitFlag);
void ccmDNDRoute(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag) ;
#endif
#if CCBS_SUPPORT
void ccmWebDialRoute(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag) ;
#endif
ccmConn_t* create_ccmConn(uint8 portType, uint16 chId, uint16  origFuncId, uint8 phoneSelect);
ccmConn_t* findOrigCcmConn(uint8 portType, uint16 chId);
void delete_ccmConn(uint8 portType, uint16 chId);
int ccm_change2ZyPhone(ccmConn_t* ccmConn_p);
int recordLocalConfPeerLocalPhLogicalId(uint16 phLogicalId,dialDigitNum_t *dialDigitNum_p);
bool isVoIPinUsed(void);

#endif
