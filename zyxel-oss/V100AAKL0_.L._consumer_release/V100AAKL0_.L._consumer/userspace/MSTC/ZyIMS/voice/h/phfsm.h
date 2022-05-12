#ifndef __PHFSM_H__
#define __PHFSM_H__
#include "call_mgnt.h"
#include "switch.h"
#include "tr104.h"

/*____________________________________________________________________________*/
/*	macro                                                                                                                        */
/*____________________________________________________________________________*/
#define N_PHONE_LOGIC_NUM		2		/* maxmum logic numbers for one physical phone */

/* The definition for the phone hook status */
#define PHONE_PHY_ST_ONHOOK	0
#define PHONE_PHY_ST_OFFHOOK	1


/* The definition for the compressType */
#define DATA_UNKNOWN	0
#define DATA_G711_64_A	(1<<0)
#define DATA_G711_64_MU	(1<<1)
#define DATA_G726_16		(1<<2)
#define DATA_G726_24		(1<<3)
#define DATA_G726_32		(1<<4)
#define DATA_G726_40		(1<<5)
#define DATA_G727_16		(1<<6)
#define DATA_G727_24		(1<<7)
#define DATA_G727_32		(1<<8)
#define DATA_G727_40		(1<<9)
#define DATA_G728_16		(1<<10)
#define DATA_G729B		(1<<11)
#define DATA_G729AB		(1<<12)
#define DATA_G729E		(1<<13)
#define DATA_GSM_EFR		(1<<14)
#define DATA_GSM_FR		(1<<14)
#define DATA_G723_5_3K	(1<<15)
#define DATA_G723_6_3K	(1<<16)
#define DATA_G722_64K	(1<<17)
#define DATA_G722_56K	(1<<18)
#define DATA_G722_48K	(1<<19)
#define DATA_FAX_T38		(1<<20)


/* The definition for the phoneCh->phoneFlags */
#define PHONE_PHYSICAL_USAGE				(1<<0)		/* Physical Phone */
#define PHONE_PHYSICAL_ONHOLDING			(1<<1)		/* Trigger the call-onhold */
#define PHONE_PHYSICAL_ONHOLD			(1<<2)		/* Enter the OnHold status */
#define PHONE_PHYSICAL_UNHOLDING			(1<<3)		/* Trigger the call-unheld */
#define PHONE_PHYSICAL_UNHOLD_RING		(1<<4)		/* Enter the UnHeld status */
#define PHONE_PHYSICAL_HOOK_NOT_CARE	(1<<5)		/* Don't care the Hook Detection */
#define PHONE_PHYSICAL_FXO_NONFLASH		(1<<6)		/* Ignore the flash detect suddenly */
#define PHONE_PHYSICAL_FEATURE_KEY		(1<<7)		/* User has pressed the feature key */
#define PHONE_IVR_FW_UPGRADE				(1<<8)		/* IVR Play FW Upgrade */
#define PHONE_IVR_EMBEDDED				(1<<9)		/* ZyPhone to play out the IVR */
#define PHONE_PHYSICAL_CONFERENCE		(1<<10)		/* Enter the Conference status */
#define PHONE_PHYSICAL_REVERSE			(1<<11)		/* Physical phone which trigger reverse */
#define PHONE_PHYSICAL_AUXILIARY			(1<<12)		/* Physical phone enter auxliary */
#define PHONE_DONOTDISTURB              (1<<13)     /* Do Not Disturb, Incoming calls transfer to ZyPhone */
#define PHONE_PHONE_CONFIG				(1<<14)		/* ZyPhone to start phone config */
#ifdef TERMINAL_PORTABILITY
#define PHONE_TERMINAL_PORTABILITY      (1<<15)     /*Denmark feature, terminal portability*/
#endif
#ifdef ZYNOS_CALL_TRANSFER /*Eason, ZyNOS_Transfer*/
#define PHONE_ATTENDED_TRANSFER      (1<<16)     
#endif
#ifdef CCBS_SUPPORT		/* aircheng add, [SPRID # 100504121] */
#define PHONE_CALLEE_EARLY_MEDIA      (1<<17)     
#endif

/* The definition for the phoneCh->phStatus */
#define PHONE_STATUS_IDLE		0
#define PHONE_STATUS_DIAL		1
#define PHONE_STATUS_RING		2
#define PHONE_STATUS_DATA		3
#define PHONE_STATUS_DROP		4
#define PHONE_STATUS_DISC		5
#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
#define PHONE_STATUS_TEST		6
#endif


/* The definition for the phoneCh->linkSt */
#define PHONE_LINK_ST_IDLE			0
#define PHONE_LINK_ST_DIGIT			1
#define PHONE_LINK_ST_CALLING		2
#define PHONE_LINK_ST_INCOMING	3
#define PHONE_LINK_ST_CONNECT		4
#define PHONE_LINK_ST_WAITING		5
#define PHONE_LINK_ST_ONHOLD		6

/* The definition for the phoneCb->hookSt */
#define PHONE_HOOK_ST_ON			0
#define PHONE_HOOK_ST_OFF			1

/* For Line Polarity  */
#define	LINE_POLARITY_FORWARD	0
#define	LINE_POLARITY_REVERSE		1

/* The definition for the logic phone channel */
#define LOGIC_PHONE_LOWBOUND	10000
#define LOGIC_PHONE_UPBOUND		20000

/* The definition for the return status of the phFsm */
#define ERROR_CCM_CONN_FIND		-1
#define ERROR_NO_USABLE_CHANNEL	-2
#define ERROR_UNDEFINE		-3

/* The definition for the callProTimer.firstDigitTimer */
#define TIMEOUT_FIRST_DIGIT				10
#define TIMEOUT_DIAL_DIGITS				3
#define TIMEOUT_CONN_CONF				30000
#define DELAY_CALLID_START				1400
#define DELAY_CALLID_STOP				2300
#define TIMEOUT_CALLID_RING			3500
#define TIMEOUT_FXS_NONHOOK_DETECT	3000
#define TIMEOUT_FXO_DIAL_LONG_DIGIT	3000
#define DURATION_FXO_DTMF_PAUSE		70
#define DURATION_FXO_DTMF_DIGIT		70
#define TIMEOUT_FXO_LIFELINE_STABLE	100
#ifdef AUXILARY_RELAY
#define TIMEOUT_FXO_CONN_CHECK		100
#else
#define TIMEOUT_FXO_CONN_CHECK		1000
#endif
#define TIMEOUT_FLASH_RING_CHECK		10000
#define TIMEOUT_FXO_NO_FLASH			3000
#define TIMEOUT_DIAL_DIGITS_TE			180
#define CALL_WAITTING_REPEAT_TIME		600		/*0.2 on 0.2 off 0.2 on 4s off*/
#define CALL_WAITTING_REPEAT_ON_TIME	525
#define CALL_WAITTING_REPEAT_OFF_TIME	3500
#define TIMEOUT_WAIT_DTAS_ACK			160		/*ms */
#define MWI_STUTTER_TONE_TIME			2000	/*250ms on 250ms off, repeat 4 times*/

/*____________________________________________________________________________*/
/*	Line events for RFC 1833													   */
/*____________________________________________________________________________*/
#define LINE_EVT_PHONE_OFF_HOOK							64
#define LINE_EVT_PHONE_ON_HOOK							65
#define LINE_EVT_PHONE_DIAL_TONE							66
#define LINE_EVT_PHONE_PABX_INTERNAL_DIAL_TONE			67
#define LINE_EVT_PHONE_SPECIAL_DIAL_TONE					68
#define LINE_EVT_PHONE_SECOND_DIAL_TONE					69
#define LINE_EVT_PHONE_RINGING_TONE						70
#define LINE_EVT_PHONE_SPECIAL_RINGING_TONE				71
#define LINE_EVT_PHONE_BUSY_TONE							72
#define LINE_EVT_PHONE_CONGESTION_TONE					73
#define LINE_EVT_PHONE_SPECIAL_INFORMATION_TONE 		74
#define LINE_EVT_PHONE_COMFORT_TONE						75
#define LINE_EVT_PHONE_HOLD_TONE							76
#define LINE_EVT_PHONE_RECORD_TONE						77
#define LINE_EVT_PHONE_CALLER_WAITING_TONE				78
#define LINE_EVT_PHONE_CALL_WAITING_TONE					79
#define LINE_EVT_PHONE_PAY_TONE							80
#define LINE_EVT_PHONE_POSITIVE_INDICATION_TONE 			81
#define LINE_EVT_PHONE_NEGATIVE_INDICATION_TONE			82
#define LINE_EVT_PHONE_WARNING_TONE 						83
#define LINE_EVT_PHONE_INTRUSION_TONE 					84
#define LINE_EVT_PHONE_CALLING_CARD_SERVICE_TONE		85
#define LINE_EVT_PHONE_PAYPHONE_RECOGNITION_TONE		86
#define LINE_EVT_PHONE_CPE_ALERTING_SIGNAL				87
#define LINE_EVT_PHONE_OFF_HOOK_WARNING_TONE 			88
#define LINE_EVT_PHONE_RING								89

/*____________________________________________________________________________*/
/*	data                                                                                                                           */
/*____________________________________________________________________________*/
#define PH_FSM_DBG(fs)						if (phoneDebug & 1)	{		ZyPrintf((fs));			\
																	ZyPrintf("\n\r"); }
#define PH_FSM_DBG1(fs, v1)				if (phoneDebug & 1)	{	ZyPrintf(fs);	\
																ZyPrintf("0x%x", v1); \
																ZyPrintf("\n\r"); }
#define PH_FSM_DBG2(fs, v1, f2, v2)			if (phoneDebug & 1) {	ZyPrintf(fs);	\
																ZyPrintf("0x%x", v1); \
																ZyPrintf(f2); \
																ZyPrintf("0x%x", v2); \
																ZyPrintf("\n\r"); }
#define PH_FSM_PRINTF	if (phoneDebug & 1)	ZyPrintf

/*____________________________________________________________________________*/
/*	event                                                                                                                         */
/*____________________________________________________________________________*/
#define	EVT_PHONE_FXS_OFFHOOK			EVENT( EVT_VOIP_PHONE, 0x00 )
#define	EVT_PHONE_FXS_ONHOOK			EVENT( EVT_VOIP_PHONE, 0x01 )
#define	EVT_PHONE_FXS_DIGITS			EVENT( EVT_VOIP_PHONE, 0x02 )
#define	EVT_PHONE_DIAL_REQ				EVENT( EVT_VOIP_PHONE, 0x03 )
#define	EVT_PHONE_FLASH				EVENT( EVT_VOIP_PHONE, 0x04 )
#define	EVT_PHONE_POLARITY_REVERSE	EVENT( EVT_VOIP_PHONE, 0x05 )
#define	EVT_PHONE_FXO_RINGING			EVENT( EVT_VOIP_PHONE, 0x06 )
#define	EVT_PHONE_FXO_STOP_RING		EVENT( EVT_VOIP_PHONE, 0x07 )
#define	EVT_PHONE_FXO_DETECT_ONHOOK	EVENT( EVT_VOIP_PHONE, 0x08 )
#define	EVT_PHONE_FXS_SENDCID			EVENT( EVT_VOIP_PHONE, 0x09 )
#define	EVT_PHONE_FXO_DETECTCID		EVENT( EVT_VOIP_PHONE, 0x0A )
#define	EVT_PHONE_FXO_RECVCID			EVENT( EVT_VOIP_PHONE, 0x0B )
#define	EVT_PHONE_DSP_DIGITS			EVENT( EVT_VOIP_PHONE, 0x0C )		/* DSP detect DTMF Tone after the call is established */
#define	EVT_PHONE_FXO_PLUGOUT			EVENT( EVT_VOIP_PHONE, 0x0D )
#define	EVT_PHONE_FAX_DETECT			EVENT( EVT_VOIP_PHONE, 0x0E )
#define	EVT_PHONE_FAX_FINISH			EVENT( EVT_VOIP_PHONE, 0x0F )
#define	EVT_PHONE_REMOVE				EVENT( EVT_VOIP_PHONE, 0x10 )
#define	EVT_PHONE_IVR_MAIN				EVENT( EVT_VOIP_PHONE, 0x11 )		/* EMBEDDED_IVR */
#define	EVT_PHONE_IVR_FUNC				EVENT( EVT_VOIP_PHONE, 0x12 )		/* EMBEDDED_IVR */
#define	EVT_PHONE_ST_CHECK				EVENT( EVT_VOIP_PHONE, 0x13 )		/* Protect for Phone Release Status */
#define	EVT_PHONE_PHONE_CONFIG_MAIN		EVENT( EVT_VOIP_PHONE, 0x14 )
#define	EVT_PHONE_PHONE_CONFIG_FUNC		EVENT( EVT_VOIP_PHONE, 0x15 )
#define	EVT_PEERFXO_BUSYTONE_DETECT	EVENT( EVT_VOIP_PHONE, 0x16)
#define	EVT_IVRPASSWORD_OVER_PLAYCOUNT    EVENT( EVT_VOIP_PHONE, 0x17) 
#define	EVT_PHONE_ISDN_FACILITY		EVENT( EVT_VOIP_PHONE, 0x18 )
#define	EVT_PHONE_ISDN_TE_RINGING		EVENT( EVT_VOIP_PHONE, 0x19 )
#define	EVT_PHONE_ISDN_TE_DISCONNECT	EVENT( EVT_VOIP_PHONE, 0x20)
#ifdef VISUAL_MWI
#define EVT_PHONE_NEW_VOICE_MESSAGE	EVENT( EVT_VOIP_PHONE, 0x22)
#define EVT_PHONE_NO_NEW_VOICE_MESSAGE	EVENT( EVT_VOIP_PHONE, 0x23)
#endif
#define	EVT_PEERFXO_RINGBACKTONE_DETECT	 EVENT( EVT_VOIP_PHONE, 0x24)
#define	EVT_PHONE_PHONECONFIG_TRUE		EVENT( EVT_VOIP_PHONE, 0x25)
#define	EVT_PHONE_PHONECONFIG_FALSE		EVENT( EVT_VOIP_PHONE, 0x26)


/*____________________________________________________________________________*/
/*	type definition                                                                                                             */
/*____________________________________________________________________________*/
typedef struct {
	chann_sig_t sig;		/* channel signal parameters	*/
} phoneCb_t;

typedef struct callProTimer_s
{
	tm_id_t firstDigitTimer;				/* First dial digit timer */
	tm_id_t dialDigitTimer;				/* Dial Digital interval */ 
	tm_id_t connConfTimer;				/* Connection confirm timer */
	tm_id_t fxsCallerIDStartTimer;			/* Delay before sending Caller-ID timer */
	tm_id_t fxsCallerIDStopTimer;			/* Set SLIC to normal after callerID timer */
	tm_id_t fxsCallerIDRingTimer;			/* Ring After sending Caller-ID timer */
	tm_id_t fxsCallerIDTACSecTimer;			/* Second TAS Timer */
	tm_id_t fxsflashRingTimer;				/* Ring after onhooking the phone if other logical channels exists */
	tm_id_t fxsFeatureKeyTimer;			/* FXS Feature Key Timer */
	tm_id_t fxsNonHookDetectTimer;		/* FXS ignore the Hook Event because of the LifeLine */
	tm_id_t fxoDialDigitTimer1;			/* FXO dial digit out timer1 */
	tm_id_t fxoDialDigitTimer2;			/* FXO dial digit out timer2 */
	tm_id_t fxoConnCheckTimer;			/* FXO Connection Check Timer */
	tm_id_t fxoNonFlashCheckTimer;		/* FXO Turn off Flash Check Timer */
	tm_id_t fxsCallWaittingRepeater;		/* Call Waitting Repeat timer */
	tm_id_t fxsCallWaittingRejectTimer;	/* Call Waitting Repeat timer */
	tm_id_t fxsCallerIDRingStopTimer;		/* CallerID Ring Stop Timer */
	tm_id_t fxsCallerIDDTMFToneOnTimer;	/* CallerID DTMF Tone On Timer */
	tm_id_t fxsCallerIDDTMFToneOffTimer;	/* CallerID DTMF Tone Off Timer */
	tm_id_t fxsCIDFSKOffHookWaitACKTimer;	/* CallerID Wait ACK Timer */
	tm_id_t fxsMWIStutterToneTimer;		/* MWI Shutter Tone Timer */
	tm_id_t fxsWaitPhoneACKTimer;		/* OffHook CallerID DTAS ACK Timer */
	tm_id_t fxsOffHookCallerIDTimer;		/* OffHook CallerID Send Digits Start Timer */
	tm_id_t fxsToneStartTimer;			/* FXS Tone Timer */
	tm_id_t fxsRingStartTimer;			/* FXS Ring Timer */
	#ifdef HOWLER_TONE
	tm_id_t fxsOffhookHowlerToneTimer;	/*FXS Howler Tone timer*/
	tm_id_t fxsHowlerTonePeriodicTimer;	/* periodic timer to raise phone volume for Howler Tone*/
	#endif	
	#ifdef TERMINAL_PORTABILITY	
	tm_id_t fxsTerminalPortabilityTimer;
	#endif
#if CCBS_SUPPORT
	tm_id_t fxsWebRepeatDialTimer;		/* WebPhone Repeat dial  timer */
	tm_id_t fxsWebDialStatusTimer;		/* WebPhone dial Status  timer */
	tm_id_t fxsWebDialCheckToneTimer;	/* WebPhone dial FXO MGB_CPTONE_MODE Tone detection Timer */ 
#endif
} callProTimer_t;

#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
typedef struct autoTest_s
{
	uint8 autoTest_digits[MAX_DIAL_STRING_LENGTH];
	tm_id_t fxsAutoTestTimerOne;		/* VoIP Auto Test timer */
	tm_id_t fxsAutoTestTimerTwo;		/* VoIP Auto Test timer */
	bool isFXSONESucc;
	bool isFXSTWOSucc;
} autoTest_t;
#endif

typedef struct{
	uint8 Tone;
	int Times;
}limit_struct;

//Ssu-Ying, support Multiple Profile
typedef struct phoneCh_s
{
	struct phoneCh_s *prev;				/* Linked list pointers 	*/
	struct phoneCh_s *next;
	uint16 physicalId;						/* for voice dsp channel Id */
	uint16 logicalId;
	uint16 callId;
	int associatedVpItr;
	int associatedSipAccountId;
	uint16 phStatus;						/* 0:IDLE, 1:DIAL, 2:RING, 3:DATA, 4:DROP, 5:DISC */
	uint16 linkSt;							/* 0:IDLE, 1:CALLING, 2:INCOMING, 3:CONNECT, 4:WAITING */
	uint16 peerMediaChId;				/* Peer Media ChId */
	uint8 enabled;						/* Enabled: TRUE/FALSE from user config. */
	uint8 dspPortType;					/* 0:FXS, 1:FXO ,ISDN ...etc*/
	uint32 phoneFlags;					/* phoneCh flags */
	dialInfor_t dialInform;					/* Record the dialOut Number */
	callProTimer_t callProTimer;			/* Call Progress Timer */
	uint32 onHookTime;					/* OnHook Time */
	uint8 CallWaitingToneCounter;			/* Call-Waiting Tone Play out Counter */
	uint8 LinePolarity;					/* Record the line polarity *///hchsu add for telefonica 2005/03/23
#ifdef AUXILARY_RELAY
	uint8 fxoOnhookCounter;				/* On hook Counter */
#endif

	limit_struct limit_tone;
	uint16 localConfPeerLocalPhLogicalId;	/* Phone Logical Id with Conference local mixer and local peer */
#ifdef TAIWAN_CID
	uint8 DTMFNotDet;
#endif
#ifdef COMMON_CID_DET
	uint8 CID_detection;
#endif
#ifdef HOWLER_TONE
	uint8 howlerToneSeconds;  /*passing seconds after Howler tone played*/
#endif
} phoneCh_t;

typedef struct voicePhoneCh_s 
{
	uint32	phoneChNum;
	phoneCh_t *phoneChEntry;
#ifdef VISUAL_MWI
	/* Used for only sending VMWI if phone is offhook and receiving Voicemessage notify */
	uint8 *mwiChangeStatus; /* 0: no change 1: new voicemessage 2: voice message removed, PH_FXS_NUM_CHANNELS*/
#endif
} voicePhoneCh_t;

#ifdef CALL_RETURN_SUPPORT
typedef struct {
	uint8 callHistoryLastCallDigit[MAX_DIAL_STRING_LENGTH];
	uint8 current_length;
} CallHistoryLastCall_t;
#endif


typedef struct
{
	uint16		cidType;
	char			month[2];
	char			day[2];
	char			hour[2];
	char			minute[2];
	uint8			digitsFlag;
	char			digits[MAX_DIAL_STRING_LENGTH];
	char			digits_absence;
	uint8			nameFlag;
	char			name[MAX_CNAME_LENGTH];
	char			name_absence;
} callerIDInfo_t;

#ifdef CALL_RETURN_SUPPORT
extern CallHistoryLastCall_t *callHistoryLastCall; /*array size = slic channel size, allocate in phoneInit()*/
#endif

/*____________________________________________________________________________*/
/*	data definition                                                                                                             */
/*____________________________________________________________________________*/
extern voicePhoneCh_t voicePhoneCh;
extern phoneCb_t phoneCb;
extern int phoneDebug;
extern int zyxelTE;
#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
extern autoTest_t VoIP_autoTest;
#endif

phoneCh_t* find_phoneCB(uint16 chanId, uint16 type);
int phoneInit(void);
int phoneFsmProc(evtMsg_t *msg);
static phoneCh_t* create_phoneCB(uint16 physicalId, uint8 PortType);
void reset_PhoneCB(phoneCh_t *phoneCB);
int checkLogicPhoneNum(uint16 physicalId);
phoneCh_t* getNextLogicPhone(phoneCh_t* phoneCB);
VoiceProfLineObj_t* getAssociatedLineObj(phoneCh_t* phoneCB);
VoiceProfObj_t* getAssociatedProfileObj(phoneCh_t* phoneCB);
VoiceProfLineCallingFeaturesObj_t* getAssociatedLineFeatures(phoneCh_t* phoneCB);
void delete_PhoneCB(phoneCh_t *phoneCB, ccmConn_t *ccmConn_p);
void delete_AllPhoneCB(void);
void AdjustPhoneLEDBySipMap(int physicalId, uint32 sipSelect);

#if CCBS_SUPPORT
#define  _copyWebDialLog 		copyWebDialLog
#define  _setWebDialStates_Start 	setWebDialStates_Start()
#define  _setWebDialStates_End 	setWebDialStates_End()
#define  _getWebDialStatus		getWebDialStatus
#define  _WebDialAssociate 		WebDialAssociate

#endif
#endif

