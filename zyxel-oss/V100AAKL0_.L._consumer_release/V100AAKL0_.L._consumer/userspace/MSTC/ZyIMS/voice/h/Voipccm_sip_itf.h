#ifndef __VOIPCCM_SIP_ITF_INCLUDE__
#define __VOIPCCM_SIP_ITF_INCLUDE__

#include "switch.h"
/*_____________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% type definition
%%____________________________________________________________________________*/
/* The definition for the SIP debug usage */
#define SIP_FSM_DBG(fs)				if (sipDebug & 1) {		dbg_pline_1((fs));			\
														dbg_pline_1("\n\r"); }
#define SIP_FSM_DBG1(fs, v1)			if (sipDebug & 1) {		dbg_plinel_1((fs), (v1));	\
														dbg_pline_1("\n\r"); }
#define SIP_FSM_DBG2(fs, v1, f2, v2)	if (sipDebug & 1) {		dbg_plinel_1((fs), (v1));	\
														dbg_plinel_1((f2), (v2));	\
														dbg_pline_1("\n\r"); }

#define SIP_CCM_CONVERT_ST_IDLE		0
#define SIP_CCM_CONVERT_ST_CALLER		1
#define SIP_CCM_CONVERT_ST_CALLEE		2

#define SIP_CCM_CONVERT_PORTID_UPBOUND		3000
#define SIP_CCM_CONVERT_PORTID_LOWBOUND		2000

#define SIP_CCM_TABLE_TEMPORARY		0
#define SIP_CCM_TABLE_PERMANENT		1

#define SIP_CCM_TABLE_DELETE_CHECK     0
#define SIP_CCM_TABLE_DELETE_NOT_CHECK     1

/*_____________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% data definition
%%____________________________________________________________________________*/
typedef struct sipCCMConvertEntry_s {
	struct sipCCMConvertEntry_s *prev;
	struct sipCCMConvertEntry_s *next;
	int	sipCcmPortId;
	int	RoleSt;
	int	flags;
	int	callId;
	int	SipAccountId;
	int	VpItr;
	#if 1 /*Eason, sipCallStatus*/
	sipCallStatus_t sipCallStatus;
	#endif
} sipCCMConvertEntry_t;

typedef struct sipCCMConvert_s {
	uint32	sipCCMConvertNum;
	sipCCMConvertEntry_t *sipCCMConvertEntry;
} sipCCMConvert_t;

extern int sipDebug;
/*_____________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% function definition
%%____________________________________________________________________________*/
void sipEvtToCCM(int SipAccountId, int callId, int eventType, void *msg, int length);
void CCMEvtToSip(evtMsg_t *cbp);
void initSipCCMConvert(void);
void deleteSipCCMConvert(int SipAccountId, int callId, int type, int option); 
void deleteAllSipCCMConvert(void);
ccmConn_t* addSipCCMConvert(int RoleStatus, int flags, int SipAccountId, int callId);
sipCCMConvertEntry_t *findSipCCMConvertByCallId(int callId);
sipCCMConvertEntry_t *findPermanentSipCCMConvertByLineId(int SipAccountId);
sipCCMConvertEntry_t *findSipCCMConvertByPortId(uint16 sipCcmPortId);
sipCCMConvertEntry_t *changeSipCCMConvert(int SipAccountId, int callId, int type);
void resetSipCCMConvert(int SipAccountId, int callId, int type);
void ccmDiscAckToSIP(int SipAccountId, int callId);
/*Jaosn , syslog*/
#ifdef VOIP_SYSLOG
void sipEvtToVoIPLOG( int callId, void *msg,int length);
#endif
	
#endif

