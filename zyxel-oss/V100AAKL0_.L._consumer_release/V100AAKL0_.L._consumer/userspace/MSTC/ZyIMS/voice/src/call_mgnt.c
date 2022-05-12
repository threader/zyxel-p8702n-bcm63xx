#include <stdio.h>
#include <sys/socket.h>
#include "global.h"
#include "switch.h"
#include "event.h"
#include "sys_msg.h"
#include "call_mgnt.h"
#include "Voipccm_sip_itf.h"
#include "phfsm.h"
#include "Phcmd.h"
#include "callTrans.h"
#include "icf_types.h"
#include "voiceApp_main.h"
#include "tr104.h"
#include "dialplan.h"
#ifdef CALLHISTORY_SUPPORT
#include "webCallHistory.h"
#endif
#include "numberingplan.h"
#include "callConf.h"

/*Jaosn , syslog*/
#ifdef VOIP_SYSLOG
#include "config_voip_stats.h"
#include "syslog.h"
#include "voip_syslog.h"
#include <sys/time.h>
#endif

#if CCBS_SUPPORT /*CCBS*/
#include "webDial_fsm.h"
#endif

int ccmConnFsmProc(evtMsg_t*);
static int ccmConnFsmIdle(evtMsg_t*,ccmConn_t*);
static int ccmConnFsmConnOut(evtMsg_t*, ccmConn_t*);
static int ccmConnFsmConnIn(evtMsg_t*, ccmConn_t*);
static int ccmConnFsmConnRdy(evtMsg_t*, ccmConn_t*);
static int ccmConnFsmConnRel(evtMsg_t*, ccmConn_t*);
static int ccmConnFsmInteractive(evtMsg_t*, ccmConn_t*);
static int ccmConnFsmInterConnect(evtMsg_t*, ccmConn_t*);


/*____________________________________________________________________________*/
/*	data declaration                                                                                                          */
/*____________________________________________________________________________*/
/*_____ global variables _______________________________________________*/
int ccmDebug = 0;
ccmConnCb_t ccmConnCb;
associateTable_t ccmAssociateTable[MAX_PORT_ROUTE][MAX_PORT_ROUTE];
#if 0
char callFeatureKey[CALL_FEATURE_NUMBERS][CALL_FEATURE_MAX_KEY_LEN] = 
{
	"****",			/* IVR_EMBEDDED */
	"####", 			/* INTERNAL_CALL */
	"*99#", 			/* ENABLE_FW_UPDATE */
	"#99#", 			/* DISABLE_FW_UPDATE */
	"*98#", 			/* CALL_TRANSFER */
	"*97#", 			/* CALL_PARK */
	"#97#", 			/* CALL_PARK_RETRIEVE */
};

#endif

PhoneFeaturePrefixInfo_t PhoneFeatureTable[CALL_FEATURE_NUMBERS] =
{  /*phone feature Id       		, TR104 Facility  , feature name(for log) */ 
 { CALL_FEATURE_RETURN, 			     "X_ZYXEL_CALL_RETURN",	"Call return"},
 { CALL_FEATURE_ONE_SHOT_CID_DISPLAY,    "CA_ACTIVATE",		"temp CID dislplay"},
 { CALL_FEATURE_ONE_SHOT_CID_HIDDEN,     "X_ZYXEL_CA_DISABLE",	"temp CID hidden"},
 { CALL_FEATURE_CALL_WAITING_ENABLE,     "CW_ACTIVATE",		"Enable Call waiting"},
 { CALL_FEATURE_CALL_WAITING_DISABLE,    "CW_DEACTIVATE"},
 { CALL_FEATURE_IVR,    				 "X_ZYXEL_IVR",			"Enter IVR menu"},
 { CALL_FEATURE_INTERNAL_CALL,		     "X_ZYXEL_INTERNALCALL",	"Make internal call"},
 { CALL_FEATURE_CALL_TRANSFER,   		 "X_ZYXEL_TRANSFER_ACTIVE","Call transfer"},
 { CALL_FEATURE_PARK,					 "X_ZYXEL_CALL_PARK_ACTIVE","Call park"},
 { CALL_FEATURE_PARK_RETRIEVE,			 "X_ZYXEL_CALL_PARK_RETRIEVE","Call Retrieve"},
 { CALL_FEATURE_UNCONDITIONAL_FORWARD_ENABLE	,"CFU_ACTIVATE","Enable CFU"},
 { CALL_FEATURE_UNCONDITIONAL_FORWARD_DISABLE	,"CFU_DEACTIVATE","Diable CFU"},
 { CALL_FEATURE_NOANSWER_FORWARD_ENABLE	,		"CFNR_ACTIVATE","Enable CFNR"},
 { CALL_FEATURE_NOANSWER_FORWARD_DISABLE	,		"CFNR_DEACTIVATE","Disable CFNR"},
 { CALL_FEATURE_BUSY_FORWARD_ENABLE,	"CFB_ACTIVATE",		"Enable CFB"},
 { CALL_FEATURE_BUSY_FORWARD_DISABLE,	"CFB_DEACTIVATE",	"Disable CFB"},
 { CALL_FEATURE_ONE_SHOT_CALL_WAITING_ENABLE,	"X_ZYXEL_ONESHOT_CW_ACTIVATE","temp enable CallWaiting"},
 { CALL_FEATURE_ONE_SHOT_CALL_WAITING_DISABLE,	"X_ZYXEL_ONESHOT_CW_DEACTIVATE","temp disable CallWaiting"},
 { CALL_FEATURE_DISABLE_DND,"X_ZYXEL_DND_DISABLE",	"Disable DND"},
 { CALL_FEATURE_ENABLE_DND,"X_ZYXEL_DND_ENABLE",	"Enable DND"},
 { CALL_FEATURE_CCBS_DEACTIVATE, "CCBS_DEACTIVATE","Call Back Busy Subscriber Deactive"} ,
 { CALL_FEATURE_OUTGOING_SIP, "X_ZYXEL_OUTGOING_SIP", "Select outgoing SIP account"} ,	/* aircheng add */
 
};

#ifdef CALLHISTORY_SUPPORT
extern webCallHistoryBuffer_t *webCallHistory_pool;
#endif


/*______________________________________________________________________________
**  callCtrlInit
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int callCtrlInit(void) {
	initCcmConn();
	ccmIpcInit();	
	return 0;
}

/*______________________________________________________________________________
**  initCcmConn
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void initCcmConn(void) {
	ccmConnCb.fxsCcmConn = NULL;
	ccmConnCb.fxoCcmConn = NULL;
	ccmConnCb.sipCcmConn = NULL;
	ccmConnCb.zyxelCcmConn = NULL;

	ccmAssociateTable[DSP_FXS_PORT][DSP_FXS_PORT] = associateFxs2Fxs;
	ccmAssociateTable[DSP_FXS_PORT][DSP_FXO_PORT] = associateFxs2Fxo;
	ccmAssociateTable[DSP_FXS_PORT][DSP_FXO_LIFELINE] = associateFxs2FxoLife;
	ccmAssociateTable[DSP_FXS_PORT][SIP_PORT] = associateFxs2Sip;
	ccmAssociateTable[DSP_FXS_PORT][ZYXEL_PHONE_PORT] = associateFxs2ZyPhone;
	ccmAssociateTable[DSP_FXS_PORT][ISDN_NT_PORT] = NULL;
	ccmAssociateTable[DSP_FXS_PORT][ISDN_TE_PORT] = NULL;
	ccmAssociateTable[DSP_FXS_PORT][DSP_DECT_PORT] = associateFxs2Fxs;

	ccmAssociateTable[DSP_FXO_PORT][DSP_FXS_PORT] = associateFxo2Fxs;
	ccmAssociateTable[DSP_FXO_PORT][DSP_FXO_PORT] = NULL;
	ccmAssociateTable[DSP_FXO_PORT][DSP_FXO_LIFELINE] = NULL;
	ccmAssociateTable[DSP_FXO_PORT][SIP_PORT] = NULL;
	ccmAssociateTable[DSP_FXO_PORT][ZYXEL_PHONE_PORT] = NULL;
	ccmAssociateTable[DSP_FXO_PORT][ISDN_NT_PORT] = NULL;
	ccmAssociateTable[DSP_FXO_PORT][ISDN_TE_PORT] = NULL;
	ccmAssociateTable[DSP_FXO_PORT][DSP_DECT_PORT] = associateFxo2Fxs;

	ccmAssociateTable[DSP_FXO_LIFELINE][DSP_FXS_PORT] = associateFxoLife2Fxs;
	ccmAssociateTable[DSP_FXO_LIFELINE][DSP_FXO_PORT] = NULL;
	ccmAssociateTable[DSP_FXO_LIFELINE][DSP_FXO_LIFELINE] = NULL;
	ccmAssociateTable[DSP_FXO_LIFELINE][SIP_PORT] = NULL;
	ccmAssociateTable[DSP_FXO_LIFELINE][ZYXEL_PHONE_PORT] = NULL;
	ccmAssociateTable[DSP_FXO_LIFELINE][ISDN_NT_PORT] = NULL;
	ccmAssociateTable[DSP_FXO_LIFELINE][ISDN_TE_PORT] = NULL;
	ccmAssociateTable[DSP_FXO_LIFELINE][DSP_DECT_PORT] = associateFxoLife2Fxs;

	ccmAssociateTable[SIP_PORT][DSP_FXS_PORT] = associateSip2Fxs;
	ccmAssociateTable[SIP_PORT][DSP_FXO_PORT] = NULL;
	ccmAssociateTable[SIP_PORT][DSP_FXO_LIFELINE] = NULL;
	ccmAssociateTable[SIP_PORT][SIP_PORT] = NULL;
	ccmAssociateTable[SIP_PORT][ZYXEL_PHONE_PORT] = associateSip2ZyPhone;
	ccmAssociateTable[SIP_PORT][ISDN_NT_PORT] = NULL;
	ccmAssociateTable[SIP_PORT][ISDN_TE_PORT] = NULL;
	ccmAssociateTable[SIP_PORT][DSP_DECT_PORT] = associateSip2Fxs;

	ccmAssociateTable[ZYXEL_PHONE_PORT][DSP_FXS_PORT] = associateZyPhone2Fxs;
	ccmAssociateTable[ZYXEL_PHONE_PORT][DSP_FXO_PORT] = NULL;
	ccmAssociateTable[ZYXEL_PHONE_PORT][DSP_FXO_LIFELINE] = NULL;
	ccmAssociateTable[ZYXEL_PHONE_PORT][SIP_PORT] = associateZyPhone2Sip;
	ccmAssociateTable[ZYXEL_PHONE_PORT][ZYXEL_PHONE_PORT] = NULL;
	ccmAssociateTable[ZYXEL_PHONE_PORT][ISDN_NT_PORT] = NULL;
	ccmAssociateTable[ZYXEL_PHONE_PORT][ISDN_TE_PORT] = NULL;
	ccmAssociateTable[ZYXEL_PHONE_PORT][DSP_DECT_PORT] = NULL;

	ccmAssociateTable[ISDN_NT_PORT][DSP_FXS_PORT] = NULL;
	ccmAssociateTable[ISDN_NT_PORT][DSP_FXO_PORT] = NULL;
	ccmAssociateTable[ISDN_NT_PORT][DSP_FXO_LIFELINE] = NULL;
	ccmAssociateTable[ISDN_NT_PORT][SIP_PORT] = NULL;
	ccmAssociateTable[ISDN_NT_PORT][ZYXEL_PHONE_PORT] = NULL;
	ccmAssociateTable[ISDN_NT_PORT][ISDN_NT_PORT] = NULL;
	ccmAssociateTable[ISDN_NT_PORT][ISDN_TE_PORT] = NULL;
	ccmAssociateTable[ISDN_NT_PORT][DSP_DECT_PORT] = NULL;

	ccmAssociateTable[ISDN_TE_PORT][DSP_FXS_PORT] = NULL;
	ccmAssociateTable[ISDN_TE_PORT][DSP_FXO_PORT] = NULL;
	ccmAssociateTable[ISDN_TE_PORT][DSP_FXO_LIFELINE] = NULL;
	ccmAssociateTable[ISDN_TE_PORT][SIP_PORT] = NULL;
	ccmAssociateTable[ISDN_TE_PORT][ZYXEL_PHONE_PORT] = NULL;
	ccmAssociateTable[ISDN_TE_PORT][ISDN_NT_PORT] = NULL;
	ccmAssociateTable[ISDN_TE_PORT][ISDN_TE_PORT] = NULL;
	ccmAssociateTable[ISDN_TE_PORT][DSP_DECT_PORT] = NULL;

	ccmAssociateTable[DSP_DECT_PORT][DSP_FXS_PORT] = associateFxs2Fxs;
	ccmAssociateTable[DSP_DECT_PORT][DSP_FXO_PORT] = associateFxs2Fxo;
	ccmAssociateTable[DSP_DECT_PORT][DSP_FXO_LIFELINE] = associateFxs2FxoLife;
	ccmAssociateTable[DSP_DECT_PORT][SIP_PORT] = associateFxs2Sip;
	ccmAssociateTable[DSP_DECT_PORT][ZYXEL_PHONE_PORT] = NULL;
	ccmAssociateTable[DSP_DECT_PORT][ISDN_NT_PORT] = NULL;
	ccmAssociateTable[DSP_DECT_PORT][ISDN_TE_PORT] = NULL;
	ccmAssociateTable[DSP_DECT_PORT][DSP_DECT_PORT] = associateFxs2Fxs;
	return;
}

/*______________________________________________________________________________
**	ccmConnFsmProc
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int ccmConnFsmProc(evtMsg_t *msg) {
	int status = 0;
	uint16 ccmConnEvent = msg->event;
	uint16 ccmConnType = msg->dev_type;
	uint16 ccmChId = msg->chann_id;
	ccmConn_t *ccmConn_p = NULL;	
	sipCCMConvertEntry_t *tmpConvertEntry_p;

	ccmConn_p = findOrigCcmConn(ccmConnType, ccmChId);
	if ( ccmConn_p == NULL ) {
		CCM_FSM_DBG1("ccmConnFsmProc ccmConn_p is NULL,  event=", ccmConnEvent);
		CCM_FSM_DBG2("      type = ", ccmConnType, " chId = ", ccmChId);
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}

	if ( ccmConnEvent == EVT_CCM_REM_REQ ) {
		delete_ccmConn(ccmConn_p->origPortType, ccmConn_p->origPortId);
		goto exit;
	} else if ( ccmConnEvent == EVT_CCM_REG_SUCC ) {
		ccmConn_p->flags |= CALL_CONN_FLAG_REGISTER;		
		tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);		
		goto exit;
	} else if ( ccmConnEvent == EVT_CCM_UNREG_SUCC ) {
		ccmConn_p->flags &= ~CALL_CONN_FLAG_REGISTER;		
		tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);		
		goto exit;
	}
	else if ( ( ccmConnEvent == EVT_CCM_CALL_TRANS_UPDATE ) && ( ccmConn_p->callTransferCB != NULL ) ) {
		callTransferFsmProc(ccmConn_p->callTransferCB, EVT_CCM_CALL_TRANS_UPDATE, NULL, 0);
		goto exit;
	}
#if CCBS_SUPPORT
	else if (ccmConn_p->WebDialCB!= NULL ){
		WebDialFsmProc(ccmConn_p, ccmConnEvent, msg->data_p, msg->data_size);	
	}
#endif

	switch( ccmConn_p->connSt ) {
		case CALL_CONN_ST_IDLE:
			status = ccmConnFsmIdle(msg, ccmConn_p);
			break;
		case CALL_CONN_ST_CONNOUT:
			status = ccmConnFsmConnOut(msg, ccmConn_p);
			break;
		case CALL_CONN_ST_CONNIN:
			status = ccmConnFsmConnIn(msg, ccmConn_p);
			break;
		case CALL_CONN_ST_CONNRDY:
			status = ccmConnFsmConnRdy(msg, ccmConn_p);
			break;
		case CALL_CONN_ST_CONNREL:
			status = ccmConnFsmConnRel(msg, ccmConn_p);
			break;
		case CALL_CONN_ST_INTERACTIVE:
			status = ccmConnFsmInteractive(msg, ccmConn_p);
			break;
		case CALL_CONN_ST_INTERCONN:
			status = ccmConnFsmInterConnect(msg, ccmConn_p);
			break;
		default:
			CCM_FSM_DBG1("ccmConnFsmProc: undefined status=", ccmConn_p->connSt);
			break;
	}

exit:
	return status;

fail:
	CCM_FSM_DBG1("ccmConnFsmProc() failed, reason=", status);
	return status;

} /* ccmConnFsmProc */

/*______________________________________________________________________________
**	ccmConnFsmIdle
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
ccmConnFsmIdle(
	evtMsg_t *msg,
	ccmConn_t *ccmConn_p
) {
	int status = 0;
	uint16 ccmConnEvent = msg->event;
#ifdef VISUAL_MWI
	sipCCMConvertEntry_t * tmpConvertEntry_p = NULL;
#endif
#if CCBS_SUPPORT
sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
#endif
	CCM_FSM_DBG2("ccmConnFsmIdle, origPortId=", ccmConn_p->origPortId, " ccmConnEvent=", ccmConnEvent);

	switch( ccmConnEvent) {
		case EVT_CCM_ASSIGN_REQ: {
				switch ( ccm_associateConn(msg, ccmConn_p) ) {
					case CCM_CONN_TYPE_CALLER_CALLER:
					case CCM_CONN_TYPE_CALLER_CALLEE:						
						if ( ccmConn_p->peerCcmConn != NULL ) {
							eventVoiceCCMSimpleSend(EVT_CCM_DIAL_REQ, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);
						} else {
							CCM_FSM_DBG("peerCcmConn_p == NULL");
						}
						break;						
					case CCM_CONN_TYPE_CALLEE_CALLEE:
						if ( ccmConn_p->peerCcmConn != NULL ) {
							eventVoiceCCMSimpleSend(EVT_CCM_DIAL_IND, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);
						} else {
							CCM_FSM_DBG("peerCcmConn_p == NULL");
						}
						break;
					case CCM_CONN_TYPE_WAIT:
						break;
					case CCM_CONN_TYPE_FAIL:  {
#ifdef CALL_TRANSFER
							ccmConn_t *prevCcmConn=NULL, *prevPeerCcmConn=NULL;
							if((prevCcmConn = findOrigCcmConn(DSP_FXS_PORT, ccmConn_p->dialInform.prevLogicId)) != NULL){
								if(((prevPeerCcmConn = prevCcmConn->peerCcmConn) != NULL) && (prevPeerCcmConn->callTransferCB != NULL)){
									CCM_FSM_DBG("un-Attended call transfer");
									eventVoiceSimpleSend(EVT_CCM_UNATTEND_CALL_TRANS_REQ, prevPeerCcmConn->origFuncId, prevPeerCcmConn->origPortId);
								}
							}
#endif
							eventVoiceSimpleSend(EVT_CCM_ASSIGN_FAIL, ccmConn_p->origFuncId, ccmConn_p->origPortId);
						}
						break;
					default:
						break;
				}
			}
			break;
		case EVT_CCM_CALL_TRANS_IND:{ /* transferee : receive refer request from netwrok */
				ccmConn_p->callFeatures = CALL_FEATURE_TRANSFER_TRANSFEREE;
				switch(ccm_associateConn(msg, ccmConn_p)){
					case CCM_CONN_TYPE_CALLER_CALLER:
						CCM_FSM_DBG("EVT_CCM_CALL_TRANS_IND associate CCM_CONN_TYPE_CALLER_CALLER");
						break;
					default:
						CCM_FSM_DBG("EVT_CCM_CALL_TRANS_IND associate fail");
						break;
				}
			}
			break;
		case EVT_CCM_ATTEND_CALL_TRANS_REPLACE: { /* transfer target : receive invite with replace header */
				ccmConn_p->callFeatures = CALL_FEATURE_TRANSFER_TARGET;
				switch(ccm_associateConn(msg, ccmConn_p)){
					case CCM_CONN_TYPE_CALLEE_CALLEE:
						CCM_FSM_DBG("EVT_CCM_ATTEND_CALL_TRANS_REPLACE associate CCM_CONN_TYPE_CALLEE_CALLEE");
						break;
					default:
						CCM_FSM_DBG("EVT_CCM_ATTEND_CALL_TRANS_REPLACE associate fail");
						break;
				}
			}
			break;
#ifdef VISUAL_MWI
		case EVT_CCM_NEW_VOICE_MESSAGE:
			ccmConn_p->flags |= CALL_CONN_FLAG_VMWI;
			switch ( ccm_associateConn(msg, ccmConn_p) ) {
				case CCM_CONN_TYPE_CALLEE_CALLEE:
					eventVoiceCCMSimpleSend(EVT_CCM_NEW_VOICE_MESSAGE, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);
					break;
				case CCM_CONN_TYPE_FAIL:
					ccmConn_p->connSt = CALL_CONN_ST_IDLE;
					ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
					tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
#ifdef MULTI_SIP_SESSION
					if ( tmpConvertEntry_p->flags == SIP_CCM_TABLE_PERMANENT ) {
						CCM_PRINTF("receive EVT_CCM_NEW_VOICE_MESSAGE , SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
						resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
					} else {
						deleteSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt, SIP_CCM_TABLE_DELETE_CHECK);
					}
#else
					resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
#endif
					ccm_releaseConn(ccmConn_p);
					break;
			}
			break;
		case EVT_CCM_NO_NEW_VOICE_MESSAGE:
			switch ( ccm_associateConn(msg, ccmConn_p) ) {
				case CCM_CONN_TYPE_CALLEE_CALLEE:
					eventVoiceCCMSimpleSend(EVT_CCM_NO_NEW_VOICE_MESSAGE, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);
					break;
				case CCM_CONN_TYPE_FAIL:
					ccmConn_p->connSt = CALL_CONN_ST_IDLE;
					ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;

					tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
#ifdef MULTI_SIP_SESSION
					if ( tmpConvertEntry_p->flags == SIP_CCM_TABLE_PERMANENT ) {
						CCM_PRINTF("receive EVT_CCM_NO_NEW_VOICE_MESSAGE , SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
						resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
					} else {
						deleteSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt, SIP_CCM_TABLE_DELETE_CHECK);
					}
#else
					resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
#endif
					ccm_releaseConn(ccmConn_p);
					break;
			}
			break;
#endif
		case EVT_CCM_REL_REQ:
			ccm_releaseConn(ccmConn_p);
			break;
#if CCBS_SUPPORT
		case EVT_CCM_DISC_ACK:
			if (ccmConn_p->origPortType == SIP_PORT) {
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
				if(tmpConvertEntry_p!=NULL){
				if ( tmpConvertEntry_p->flags == SIP_CCM_TABLE_PERMANENT ) {
						CCM_PRINTF("receive EVT_CCM_DISC_ACK , SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
						resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
					} else {
						deleteSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt, SIP_CCM_TABLE_DELETE_CHECK);
					}
				}
			}
			break;
#endif
		default:
			CCM_FSM_DBG1("undefined ccmConnEvent=", ccmConnEvent);
			break;
	}

	return status;
} /* ccmConnFsmIdle */

/*______________________________________________________________________________
**	ccmConnFsmConnOut
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
ccmConnFsmConnOut(
	evtMsg_t *msg,
	ccmConn_t *ccmConn_p
) {
	int status = 0;
	ccmConn_t *peerCcmConn_p=NULL;
	uint16 ccmConnEvent = msg->event;
#if CCBS_SUPPORT
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
#endif
#ifdef CALL_TRANSFER
	ccmConn_t *prevCcmConn=NULL, *prevPeerCcmConn=NULL, *tmpCcmConn_p = NULL;
	dialInfor_t *dialInform = NULL;
	callTransfer_t *callTransfer_p = NULL;
	int aaa = 0;
#endif

	CCM_FSM_DBG2("ccmConnFsmConnOut, origPortId=", ccmConn_p->origPortId, " ccmConnEvent=", ccmConnEvent);

	peerCcmConn_p = ccmConn_p->peerCcmConn;
	if ( peerCcmConn_p == NULL ) {
		CCM_FSM_DBG("peerCcmConn_p is NULL");
		goto terminate;
	}

	switch( ccmConnEvent) {
#if CCBS_SUPPORT
		case EVT_CCM_DISC_ACK:
			if (ccmConn_p->origPortType == SIP_PORT) {
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
				if(tmpConvertEntry_p!=NULL){
				if ( tmpConvertEntry_p->flags == SIP_CCM_TABLE_PERMANENT ) {
						CCM_PRINTF("receive EVT_CCM_DISC_ACK , SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
						resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
					} else {
						deleteSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt, SIP_CCM_TABLE_DELETE_CHECK);
					}
				}
			}
			break;
#endif
		case EVT_CCM_DIAL_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {
				eventVoiceDataSend(EVT_CCM_ASSIGN_CONF, ccmConn_p->origFuncId, ccmConn_p->origPortId, &peerCcmConn_p->origPortType, sizeof(uint16));
				eventVoiceSimpleSend(EVT_CCM_DIAL_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				
				if ( peerCcmConn_p->origPortType == DSP_FXO_LIFELINE ) {	/* FXO_LIFELINE */
					CCM_FSM_DBG("FXO_LIFELINE shutcut");
					ccmConn_p->connSt = peerCcmConn_p->connSt = CALL_CONN_ST_CONNRDY;
				}
				#if 0 //#ifdef CALL_TRANSFER
				if ( ( peerCcmConn_p->callFeatures & (CCM_CALL_TRANSFER<<8) ) && ( peerCcmConn_p->callTransferCB != NULL ) ) {
					callTransferFsmProc(peerCcmConn_p->callTransferCB, ( (peerCcmConn_p->callFeatures & 0x00FF)<<8)|(EVT_CCM_DIAL_REQ & 0x00FF), NULL, 0);
				}
				#endif
			}
			break;
		case EVT_CCM_ALERT_IND:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) ) {
					eventVoiceSimpleSend(EVT_CCM_ALERT_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
			}
			break;
             case EVT_CCM_SDP_ALERT_IND:
			 if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) ) {
					eventVoiceSimpleSend(EVT_CCM_SDP_ALERT_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
			}	
	             break;
		case EVT_CCM_REL_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {
#ifdef CALL_TRANSFER
				if ( ( peerCcmConn_p->callFeatures & (CCM_CALL_TRANSFER<<8) ) && ( peerCcmConn_p->callTransferCB != NULL ) ) {
					/* Theresa_mod_111506 : if CALL TRANSFER FXO-FXO-SIP, REL_REQ cannot send to SIP, the call will be missed!*/
					callTransferFsmProc(peerCcmConn_p->callTransferCB, ( (peerCcmConn_p->callFeatures & 0x00FF)<<8)|(EVT_CCM_REL_REQ & 0x00FF), NULL, 0);
					/* Because the call Transfer might re-assoicate to the ZyPhone, re-define the peerCcmConn_p again */
					peerCcmConn_p = ccmConn_p->peerCcmConn;
				}
#endif
				eventVoiceSimpleSend(EVT_CCM_REL_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				ccmConn_p->connSt = CALL_CONN_ST_IDLE;
				ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				peerCcmConn_p->connSt = CALL_CONN_ST_CONNREL;
				peerCcmConn_p->connRole = CCM_CONN_ROLE_PEER;
				ccm_releaseConn(ccmConn_p);
			}
			break;
		case EVT_CCM_REJ_IND:
		case EVT_CCM_REL_IND:
		case EVT_CCM_DISC_IND:
		case EVT_CCM_CALL_RESTART:		
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) ) {
				#ifdef CALL_TRANSFER
				if ( ( ccmConn_p->callFeatures & (CCM_CALL_TRANSFER<<8) ) && ( ccmConn_p->callTransferCB != NULL ) ) {
					callTransferFsmProc(ccmConn_p->callTransferCB, ( (ccmConn_p->callFeatures & 0x00FF)<<8)|(EVT_CCM_DISC_IND & 0x00FF), NULL, 0);
					/* Because the call Transfer might re-assoicate to the ZyPhone, re-define the peerCcmConn_p again */
					peerCcmConn_p = ccmConn_p->peerCcmConn;				
					
				}
				#endif
				
				#ifdef CALL_CONFERENCE
				if(peerCcmConn_p->ConfCallCB != NULL){
					ConferenceCallFsmProc(peerCcmConn_p->ConfCallCB, ccmConnEvent, msg->data_p, msg->data_size);
				}
				#endif
#ifdef NORWAY_VOIP_CUSTOMIZATION
				if( ccmConnEvent == EVT_CCM_REJ_IND ) {
					eventVoiceDataSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId, msg->data_p, msg->data_size);	
				} else {
#endif
				eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
#ifdef NORWAY_VOIP_CUSTOMIZATION
				}
#endif
				eventVoiceSimpleSend(EVT_CCM_DISC_ACK, ccmConn_p->origFuncId, ccmConn_p->origPortId);
				ccmConn_p->connSt = CALL_CONN_ST_CONNREL;
				peerCcmConn_p->connSt = CALL_CONN_ST_CONNREL;
				ccm_releaseConn(ccmConn_p);				
			}
			break;
		case EVT_CCM_CONN_IND:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) ) {
				eventVoiceSimpleSend(EVT_CCM_CONN_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				eventVoiceSimpleSend(EVT_CCM_CONN_ACK, ccmConn_p->origFuncId, ccmConn_p->origPortId);
				ccmConn_p->connSt = peerCcmConn_p->connSt = CALL_CONN_ST_CONNRDY;
				#ifdef CENTRALIZE_LOG
				ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_ESTABLISH);
				#endif
				#ifdef CALL_TRANSFER
				if ( ( ccmConn_p->callFeatures & (CCM_CALL_TRANSFER<<8) ) && ( ccmConn_p->callTransferCB != NULL ) ) {
					callTransferFsmProc(ccmConn_p->callTransferCB, ( (ccmConn_p->callFeatures & 0x00FF)<<8)|(EVT_CCM_CONN_IND & 0x00FF), NULL, 0);
				}
				#endif
				#ifdef CALL_CONFERENCE
				if(peerCcmConn_p->ConfCallCB != NULL){
					ConferenceCallFsmProc(peerCcmConn_p->ConfCallCB, ((peerCcmConn_p->callFeatures & 0x00FF)<<8)|(ccmConnEvent & 0x00FF), msg->data_p, msg->data_size);
				}
				#endif
			}
			break;
		case EVT_CCM_ATTEND_CALL_TRANS_REQ: {
				dialInform = (dialInfor_t *)msg->data_p;
				if ( (prevCcmConn = findOrigCcmConn(ccmConn_p->origPortType, dialInform->prevLogicId) ) != NULL ) {
					if ( (prevPeerCcmConn = prevCcmConn->peerCcmConn) != NULL ) {
						if ( prevPeerCcmConn->origPortType == SIP_PORT ) {
							createCallTransferCB(peerCcmConn_p, prevPeerCcmConn, CALL_TRANSFEROR, CALL_TRANSFER_TYPE_ATTENDED);
						}
					}
				}
				callTransfer_p = peerCcmConn_p->callTransferCB;
				callTransfer_p->callTransferStatus = CALL_TRANSFER_ST_SECOND_CALL;
				//eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);
				if ( ccmConn_p->callTransferCB != NULL ) {
					aaa = 1;
					tmpCcmConn_p = ccmConn_p;
				} else if ( peerCcmConn_p->callTransferCB != NULL ) {
					aaa = 2;
					tmpCcmConn_p = peerCcmConn_p;
				}

				if ( aaa != 0 ) {
					callTransferFsmProc(tmpCcmConn_p->callTransferCB, ( (tmpCcmConn_p->callFeatures & 0x00FF)<<8)|(EVT_CCM_REL_REQ & 0x00FF), NULL, 0);
					/* Because the call Transfer might re-assoicate to the ZyPhone, re-define the peerCcmConn_p again */
					peerCcmConn_p = ccmConn_p->peerCcmConn;
				}
			}
			break;
		default:
			CCM_FSM_DBG1("undefined ccmConnEvent=", ccmConnEvent);
			break;
	}

terminate:
	return status;
} /* ccmConnFsmConnOut */

/*______________________________________________________________________________
**	ccmConnFsmConnIn
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
ccmConnFsmConnIn(
	evtMsg_t *msg,
	ccmConn_t *ccmConn_p
) {
	int status = 0, firstLink=0;
	uint16 ccmConnEvent = msg->event;
	ccmConn_t *peerCcmConn_p=NULL, *tmpPeerCcmConn_p=NULL, *tmpCcmConn_p=NULL;
#ifdef VISUAL_MWI
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
#endif
#if CCBS_SUPPORT
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
#endif

	CCM_FSM_DBG2("ccmConnFsmConnIn, origPortId=", ccmConn_p->origPortId, " ccmConnEvent=", ccmConnEvent);

	peerCcmConn_p = ccmConn_p->peerCcmConn;
	if ( peerCcmConn_p == NULL ) {
		CCM_FSM_DBG("peerCcmConn_p is NULL");
		goto terminate;
	}

	switch( ccmConnEvent) {
		case EVT_CCM_DIAL_IND:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {
				while( peerCcmConn_p != NULL ) {
					if ( firstLink == 0 ) {
						eventVoiceDataSend(EVT_CCM_ASSIGN_CONF, ccmConn_p->origFuncId, ccmConn_p->origPortId, &peerCcmConn_p->origPortType, sizeof(uint16));
						firstLink = 1;
					}
					eventVoiceDataSend(EVT_CCM_DIAL_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId, &ccmConn_p->origPortType, sizeof(uint16));
					peerCcmConn_p = peerCcmConn_p->bundleCcmList.bundleCcmNext;
				}
			}
			break;
		case EVT_CCM_REL_IND:
		case EVT_CCM_REJ_IND:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {
				while( peerCcmConn_p != NULL ) {
					eventVoiceSimpleSend(EVT_CCM_REL_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
					eventVoiceSimpleSend(EVT_CCM_DISC_ACK, ccmConn_p->origFuncId, ccmConn_p->origPortId);
					ccmConn_p->connSt = CALL_CONN_ST_IDLE;
					ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
					peerCcmConn_p->connSt = CALL_CONN_ST_IDLE;
					peerCcmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
					peerCcmConn_p = peerCcmConn_p->bundleCcmList.bundleCcmNext;
				}
				ccm_releaseConn(ccmConn_p);
			}
			break;
		case EVT_CCM_DISC_IND:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {
				eventVoiceSimpleSend(EVT_CCM_DISC_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				eventVoiceSimpleSend(EVT_CCM_DISC_ACK, ccmConn_p->origFuncId, ccmConn_p->origPortId);
				ccmConn_p->connSt = CALL_CONN_ST_IDLE;
				ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				peerCcmConn_p->connSt = CALL_CONN_ST_CONNREL;
				peerCcmConn_p->connRole = CCM_CONN_ROLE_ORIGINAL;
				ccm_releaseConn(ccmConn_p);
			}
			break;
		case EVT_CCM_CONN_CONF:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {
				eventVoiceSimpleSend(EVT_CCM_CONN_CONF, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				ccmConn_p->connSt = peerCcmConn_p->connSt = CALL_CONN_ST_CONNRDY;
			}
			#ifdef CENTRALIZE_LOG
			ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_INCALL_ESTABLISH);
			#endif
			break;
		case EVT_CCM_ALERT_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) && (!( ccmConn_p->flags & CALL_CONN_FLAG_ALERT)) ) {
				if ( peerCcmConn_p != NULL ) {
					tmpCcmConn_p = ccmConn_p;
					while ( tmpCcmConn_p != NULL ) {
						tmpCcmConn_p->flags |= CALL_CONN_FLAG_ALERT;
						tmpCcmConn_p = tmpCcmConn_p->bundleCcmList.bundleCcmNext;
					}
					eventVoiceSimpleSend(EVT_CCM_ALERT_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				}
			}
			break;
		case EVT_CCM_REJ_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) ) {
				tmpPeerCcmConn_p = peerCcmConn_p->peerCcmConn;
				if ( ( tmpPeerCcmConn_p == ccmConn_p ) && ( tmpPeerCcmConn_p->bundleCcmList.bundleCcmNext == NULL ) ) {
					/* No other bundleCcm */
					eventVoiceSimpleSend(EVT_CCM_REJ_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
					ccmConn_p->connSt = CALL_CONN_ST_IDLE;
					ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
					peerCcmConn_p->connSt = CALL_CONN_ST_IDLE;
					ccm_releaseConn(ccmConn_p);
				} else {
					ccm_releaseConn(ccmConn_p);
				}
			}
			break;
		case EVT_CCM_CONN_REQ_TO:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) ) {
				eventVoiceSimpleSend(EVT_CCM_DISC_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				ccmConn_p->connSt = CALL_CONN_ST_CONNREL;
				peerCcmConn_p->connSt = CALL_CONN_ST_CONNREL;
				ccm_releaseConn(ccmConn_p);
			}
			break;
		case EVT_CCM_DISC_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) ) {
				eventVoiceSimpleSend(EVT_CCM_DISC_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				ccmConn_p->connSt = CALL_CONN_ST_IDLE;
				ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				peerCcmConn_p->connSt = CALL_CONN_ST_CONNREL;
				peerCcmConn_p->connRole = CCM_CONN_ROLE_PEER;
				ccm_releaseConn(ccmConn_p);
			}
			break;
		case EVT_CCM_CONN_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) ) {
				eventVoiceSimpleSend(EVT_CCM_CONN_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);	
				ccm_releaseOtherChann( ccmConn_p, peerCcmConn_p->peerCcmConn);
			}
			break;
#ifdef VISUAL_MWI
		case EVT_CCM_NEW_VOICE_MESSAGE:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {
				while( peerCcmConn_p != NULL ) {
					eventVoiceDataSend(EVT_PHONE_NEW_VOICE_MESSAGE, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId, &ccmConn_p->origPortType, sizeof(uint16));
					peerCcmConn_p->connSt = CALL_CONN_ST_IDLE;
					peerCcmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
					peerCcmConn_p = peerCcmConn_p->bundleCcmList.bundleCcmNext;
				}
				ccmConn_p->connSt = CALL_CONN_ST_IDLE;
				ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
#ifdef MULTI_SIP_SESSION
				if ( tmpConvertEntry_p->flags == SIP_CCM_TABLE_PERMANENT ) {
					CCM_PRINTF("receive EVT_CCM_NEW_VOICE_MESSAGE , SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
					resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
				} else {
					deleteSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt, SIP_CCM_TABLE_DELETE_CHECK);
				}
#else
				resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
#endif
				ccm_releaseConn(ccmConn_p);
			}
			break;
		case EVT_CCM_NO_NEW_VOICE_MESSAGE:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {
				while( peerCcmConn_p != NULL ) {
					eventVoiceDataSend(EVT_PHONE_NO_NEW_VOICE_MESSAGE, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId, &ccmConn_p->origPortType, sizeof(uint16));
					peerCcmConn_p->connSt = CALL_CONN_ST_IDLE;
					peerCcmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
					peerCcmConn_p = peerCcmConn_p->bundleCcmList.bundleCcmNext;
				}
				ccmConn_p->connSt = CALL_CONN_ST_IDLE;
				ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
#ifdef MULTI_SIP_SESSION
				if ( tmpConvertEntry_p->flags == SIP_CCM_TABLE_PERMANENT ) {
					CCM_PRINTF("receive EVT_CCM_NO_NEW_VOICE_MESSAGE , SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
					resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
				} else {
					deleteSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt, SIP_CCM_TABLE_DELETE_CHECK);
				}
#else
				resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
#endif
				ccm_releaseConn(ccmConn_p);			
			}
		break;
#endif
#if CCBS_SUPPORT
		case EVT_CCM_DISC_ACK:
			if(ccmConn_p->peerCcmConn!=NULL){
				tmpConvertEntry_p = findSipCCMConvertByCallId(peerCcmConn_p->origPortId);
				if(tmpConvertEntry_p!=NULL){
					if ( tmpConvertEntry_p->flags == SIP_CCM_TABLE_PERMANENT ) {
						CCM_PRINTF("receive EVT_CCM_DISC_ACK , SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
						resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
					} else {
						deleteSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt, SIP_CCM_TABLE_DELETE_CHECK);
					}
				}
			}
			break;
#endif
		default:
			CCM_FSM_DBG1("undefined ccmConnEvent=", ccmConnEvent);
			break;
	}

terminate:
	return status;
} /* ccmConnFsmConnIn */

/*______________________________________________________________________________
**	ccmConnFsmConnRdy
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
ccmConnFsmConnRdy(
	evtMsg_t *msg,
	ccmConn_t *ccmConn_p
) {
	int status = 0, callTransFlag=0;
	uint16 ccmConnEvent = msg->event;
	ccmConn_t *peerCcmConn_p=NULL, *tmpCcmConn_p = NULL;
	dialDigitBuf_t *dialDigitBuf = NULL;
#ifdef CALL_TRANSFER
	ccmConn_t *prevCcmConn=NULL, *prevPeerCcmConn=NULL;
	dialInfor_t *dialInform = NULL;
	callTransfer_t *callTransfer_p = NULL;
#endif
#if CCBS_SUPPORT
sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
#endif

	CCM_FSM_DBG2("ccmConnFsmConnRdy, origPortId=", ccmConn_p->origPortId, " ccmConnEvent=", ccmConnEvent);

	peerCcmConn_p = ccmConn_p->peerCcmConn;
	if ( peerCcmConn_p == NULL ) {
		CCM_FSM_DBG("peerCcmConn_p is NULL");
		goto terminate;
	}

	switch( ccmConnEvent) {
#if CCBS_SUPPORT
		case EVT_CCM_REL_IND:
			eventVoiceSimpleSend(EVT_CCM_DISC_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
			break;		
		case EVT_CCM_DISC_ACK:
			eventVoiceSimpleSend(EVT_CCM_DISC_ACK, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);		
			if (ccmConn_p->origPortType == SIP_PORT) {
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
				if(tmpConvertEntry_p!=NULL){
					if ( tmpConvertEntry_p->flags == SIP_CCM_TABLE_PERMANENT ) {
						CCM_PRINTF("receive EVT_CCM_DISC_ACK , SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
						resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
					} else {
						deleteSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt, SIP_CCM_TABLE_DELETE_CHECK);
					}
				}
			}
			
			ccmConn_p->connSt = CALL_CONN_ST_IDLE;
			ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
			if ( peerCcmConn_p->origPortType == DSP_FXO_LIFELINE ) {
				peerCcmConn_p->connSt = CALL_CONN_ST_IDLE;
				peerCcmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
			} else {
				peerCcmConn_p->connSt = CALL_CONN_ST_CONNREL;
				peerCcmConn_p->connRole = CCM_CONN_ROLE_PEER;
			}
			ccm_releaseConn(ccmConn_p);			
			break;
#endif			
		case EVT_CCM_DISC_REQ:
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
				#ifdef CALL_CONFERENCE
				if(ccmConn_p->ConfCallCB != NULL){
					/*Eason, 2009/07/07, to support hold/resume one call without remove conference*/
					if (CONF_CALL_ST_ONE_CALL_ONHOLD != ccmConn_p->ConfCallCB->confcallStatus){
						ConferenceCallFsmProc(ccmConn_p->ConfCallCB, ((ccmConn_p->callFeatures & 0x00FF)<<8)|(ccmConnEvent & 0x00FF), msg->data_p, msg->data_size);
					}
					else{
						/*in CONF_CALL_ST_ONE_CALL_ONHOLD state, DISC_REQ, send EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_REQ
						  * to onHoldCCM to remove conferenceCB
						  */
						CCM_PRINTF("CONF_CALL_ST_ONE_CALL_ONHOLD, send EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_REQ, threeWayCallBaseLogicId = %d\n", ccmConn_p->ConfCallCB->threeWayCallBaseLogicId);
						if(ccmConn_p->ConfCallCB->confcallOnHoldCCM != ccmConn_p){						
							eventVoiceCCMDataSend(EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_REQ, ccmConn_p->ConfCallCB->confcallOnHoldCCM->origFuncId,ccmConn_p->ConfCallCB->confcallOnHoldCCM->origPortId, DSP_FXS_PORT, &(ccmConn_p->ConfCallCB->threeWayCallBaseLogicId), sizeof(uint16));
						}
						else{
							eventVoiceCCMDataSend(EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_REQ, ccmConn_p->ConfCallCB->confcallActiveCCM->origFuncId,ccmConn_p->ConfCallCB->confcallActiveCCM->origPortId, DSP_FXS_PORT, &(ccmConn_p->ConfCallCB->threeWayCallBaseLogicId), sizeof(uint16));
							
						}
					}
				}
				#endif
				#if 0 //#ifdef CALL_TRANSFER
				/* Because callTransferCB only bundle on the SIP CCM */
				if ( ccmConn_p->callTransferCB != NULL ) {
					aaa = 1;
					tmpCcmConn_p = ccmConn_p;
				} else if ( peerCcmConn_p->callTransferCB != NULL ) {
					aaa = 2;
					tmpCcmConn_p = peerCcmConn_p;
				}

				if ( aaa != 0 ) {
					callTransferFsmProc(tmpCcmConn_p->callTransferCB, ( (tmpCcmConn_p->callFeatures & 0x00FF)<<8)|(EVT_CCM_REL_REQ & 0x00FF), NULL, 0);
					/* Because the call Transfer might re-assoicate to the ZyPhone, re-define the peerCcmConn_p again */
					peerCcmConn_p = ccmConn_p->peerCcmConn;
				} else {
				#endif
				eventVoiceSimpleSend(EVT_CCM_DISC_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				ccmConn_p->connSt = CALL_CONN_ST_IDLE;
				ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				if ( peerCcmConn_p->origPortType == DSP_FXO_LIFELINE ) {
					peerCcmConn_p->connSt = CALL_CONN_ST_IDLE;
					peerCcmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				}
				#ifdef FXO_DSPRELAY
				else if(peerCcmConn_p->origPortType == DSP_FXO_PORT){
					peerCcmConn_p->connSt = CALL_CONN_ST_IDLE;
					peerCcmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				} 
				#endif
				else {
					peerCcmConn_p->connSt = CALL_CONN_ST_CONNREL;
					peerCcmConn_p->connRole = CCM_CONN_ROLE_PEER;
				}
				ccm_releaseConn(ccmConn_p);
				#if 0 //#ifdef CALL_TRANSFER
				}
				#endif
			}
			break;
		case EVT_CCM_DISC_IND:
		case EVT_CCM_REJ_IND:
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
				#ifdef CALL_TRANSFER
				if ( ( ccmConn_p->callFeatures & (CCM_CALL_TRANSFER<<8) ) && ( ccmConn_p->callTransferCB != NULL ) ) {
					callTransferFsmProc(ccmConn_p->callTransferCB, ( (ccmConn_p->callFeatures & 0x00FF)<<8)|(EVT_CCM_DISC_IND & 0x00FF), NULL, 0);
					/* Because the call Transfer might re-assoicate to the ZyPhone, re-define the peerCcmConn_p again */
					peerCcmConn_p = ccmConn_p->peerCcmConn;
				}
				#endif
				#ifdef CALL_CONFERENCE
				if(peerCcmConn_p->ConfCallCB != NULL){
					ConferenceCallFsmProc(peerCcmConn_p->ConfCallCB, ((peerCcmConn_p->callFeatures & 0x00FF)<<8)|(ccmConnEvent & 0x00FF), &peerCcmConn_p->origPortId, sizeof(uint16));
					peerCcmConn_p = ccmConn_p->peerCcmConn;
				}
				#endif
				eventVoiceSimpleSend(EVT_CCM_DISC_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				eventVoiceSimpleSend(EVT_CCM_DISC_ACK, ccmConn_p->origFuncId, ccmConn_p->origPortId);
				if ( ccmConn_p->origPortType == SIP_PORT ) {
					ccmConn_p->connSt = CALL_CONN_ST_CONNREL;
				} else {
					ccmConn_p->connSt = CALL_CONN_ST_IDLE;
					ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				}
				peerCcmConn_p->connSt = CALL_CONN_ST_CONNREL;
				peerCcmConn_p->connRole = CCM_CONN_ROLE_ORIGINAL;
				ccm_releaseConn(ccmConn_p);
			}
			break;
		case EVT_CCM_DIAL_DIGIT:
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
				dialDigitBuf = (dialDigitBuf_t *)calloc(1, sizeof(dialDigitBuf_t) );
				zyMemcpy(dialDigitBuf, (dialDigitBuf_t *)msg->data_p, sizeof(dialDigitBuf_t));
				CCM_FSM_DBG1("CCM: digit = ", dialDigitBuf->digitStr);
				eventVoiceDataSend(EVT_CCM_DIAL_DIGIT, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId, dialDigitBuf, sizeof(dialDigitBuf_t) );
				zyFree(dialDigitBuf);
				dialDigitBuf = NULL;
			}
			break;
		case EVT_CCM_ONHOLD_CONF:
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
				eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				#ifdef CALL_TRANSFER
				if ( ccmConn_p->callTransferCB != NULL ) {
					if ( ccmConn_p == ccmConn_p->callTransferCB->callTransferSecondCCM ) {
						callTransferFsmProc(ccmConn_p->callTransferCB, EVT_CT_REL, NULL, 0);
					}
				}
				#endif

				if( (NULL != peerCcmConn_p->ConfCallCB) && (CONF_CALL_ST_ONE_CALL_ONHOLD != peerCcmConn_p->ConfCallCB->confcallStatus )){
					peerCcmConn_p->ConfCallCB->confcallStatus = CONF_CALL_ST_ONE_CALL_ONHOLD;	
					CCM_PRINTF("ConfCall state changed to CONF_CALL_ST_ONE_CALL_ONHOLD\n");
				}
			}
			break;
		case EVT_CCM_UNHELD_REQ:
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
			#if 0 /* CALL_CONFERENCE */ /*Eason,  2009/07/07, don't handle UNHELD_REQ to prevent remove conference, only remove conference when one call leave*/
				if(ccmConn_p->ConfCallCB != NULL){
					ConferenceCallFsmProc(ccmConn_p->ConfCallCB, ccmConnEvent, NULL, 0);
				}else
			#endif
				eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
			}
			break;
		case EVT_CCM_CONFERENCE_IND:
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
			#if 1 /* CALL_CONFERENCE */
				if(ccmConn_p->ConfCallCB != NULL){
					ConferenceCallFsmProc(ccmConn_p->ConfCallCB, ccmConnEvent, NULL, 0);
				}else
			#endif
				eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
			}
			break;
		case EVT_CCM_ONHOLD_REQ: /*Eason, 2009/07/07, don't handle ONHOLD_REQ to prevent remove conference, only remove conference when one call leave*/
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {								
				eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
			}
			break;
		case EVT_CCM_FAX_RELAY:
		case EVT_CCM_MEDIA_CHANGE:
		case EVT_CCM_FAX_T38:
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {			
			#ifdef CALL_CONFERENCE
				if(ccmConn_p->ConfCallCB != NULL){
					ConferenceCallFsmProc(ccmConn_p->ConfCallCB, ((ccmConn_p->callFeatures & 0x00FF)<<8)|(ccmConnEvent & 0x00FF), NULL, 0);
				}
			#endif			
				eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
			}
			break;
		case EVT_CCM_UNHELD_CONF:
		#if 1 /* CALL_CONFERENCE */
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {				
				if(peerCcmConn_p->ConfCallCB != NULL){
					ConferenceCallFsmProc(peerCcmConn_p->ConfCallCB,ccmConnEvent,NULL,0);
					#if 1 /*Eason, 2009/07/07, to support hold/resume one call without remove conference*/
					if( CONF_CALL_ST_ONE_CALL_ONHOLD == peerCcmConn_p->ConfCallCB->confcallStatus){
						/* if one call hold, confcall state changed to CONF_CALL_ST_ONE_CALL_ONHOLD, 
						  * after call resumed, swap confcall OnholdCCM and ActiveCCM and change confcall state to
						  * CONF_CALL_ST_ALL_MEMBER_JOINED
						  **/
						
						CCM_PRINTF("peerCcmConn logicalId = 0x%x ,ActiveCCM logicalId = 0x%x, OnHoldCCM logicalId = 0x%x\n", 
								peerCcmConn_p->origPortId,
								peerCcmConn_p->ConfCallCB->confcallActiveCCM->origPortId,
								peerCcmConn_p->ConfCallCB->confcallOnHoldCCM->origPortId);
						if(peerCcmConn_p != peerCcmConn_p->ConfCallCB->confcallOnHoldCCM){
							swapConferenceCallCB(peerCcmConn_p->ConfCallCB);
							CCM_PRINTF("exit swapConferenceCallCB\n");
							CCM_PRINTF("EVT_CCM_UNHELD_CONF, swap ActiveCCM and OnHoldCCM, ActiveCCM logicalId = 0x%x, OnHoldCCM logicalId = 0x%x\n", 
								peerCcmConn_p->ConfCallCB->confcallActiveCCM->origPortId,
								peerCcmConn_p->ConfCallCB->confcallOnHoldCCM->origPortId);
						}
					}
					eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
					#endif
				}
				else{
					eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				}
			}
		#else
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
				eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
			}
		#endif
			break;
		case EVT_CCM_ATTEND_CALL_TRANS_REQ: {
				dialInform = (dialInfor_t *)msg->data_p;
				if ( (prevCcmConn = findOrigCcmConn(ccmConn_p->origPortType, dialInform->prevLogicId) ) != NULL ) {
					if ( (prevPeerCcmConn = prevCcmConn->peerCcmConn) != NULL ) {
						if ( prevPeerCcmConn->origPortType == SIP_PORT ) {
							#ifdef CENTRALIZE_LOG
							ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_START);
							#endif
							createCallTransferCB(peerCcmConn_p, prevPeerCcmConn, CALL_TRANSFEROR, CALL_TRANSFER_TYPE_ATTENDED);
						}
					}
				}
				callTransfer_p = peerCcmConn_p->callTransferCB;
				callTransfer_p->callTransferStatus = CALL_TRANSFER_ST_SECOND_CALL;
				//eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);
				if ( ccmConn_p->callTransferCB != NULL ) {
					callTransFlag = 1;
					tmpCcmConn_p = ccmConn_p;
				} else if ( peerCcmConn_p->callTransferCB != NULL ) {
					callTransFlag = 2;
					tmpCcmConn_p = peerCcmConn_p;
				}

				if ( callTransFlag != 0 ) {
					#ifdef CENTRALIZE_LOG
					ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_START);
					#endif
					callTransferFsmProc(tmpCcmConn_p->callTransferCB, ( (tmpCcmConn_p->callFeatures & 0x00FF)<<8)|(EVT_CCM_REL_REQ & 0x00FF), NULL, 0);
					/* Because the call Transfer might re-assoicate to the ZyPhone, re-define the peerCcmConn_p again */
					peerCcmConn_p = ccmConn_p->peerCcmConn;
				}
			}
			break;
		case EVT_CCM_CONFERENCE_CONF:
		#if 1 /* CALL_CONFERENCE */
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
				if(peerCcmConn_p->ConfCallCB != NULL){
					ConferenceCallFsmProc(peerCcmConn_p->ConfCallCB,ccmConnEvent,NULL,0);
				}
				else{
					eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				}
			}
		#else
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
				eventVoiceSimpleSend(ccmConnEvent, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
			}
		#endif
			break;
#ifdef CALL_CONFERENCE
		case EVT_CCM_CONFCALL_MEETROOM_REQ:
				if(createConferenceCallCB(ccmConn_p,*(uint16 *)(msg->data_p)) == 0){
					ConferenceCallFsmProc(ccmConn_p->ConfCallCB, ((ccmConn_p->callFeatures & 0x00FF)<<8)|(ccmConnEvent & 0x00FF), NULL, 0);
				}else{
					CCM_FSM_DBG("\r\n Conference Call Request Failed");
				}
			break;
		case EVT_CCM_CONFCALL_MEETROOM_FAIL:
			if(peerCcmConn_p->ConfCallCB != NULL){
				ConferenceCallFsmProc(peerCcmConn_p->ConfCallCB,ccmConnEvent,NULL,0);
			}
			break;
	#if 0
		case EVT_CCM_UNHOLD_FAIL:
			if(peerCcmConn_p->ConfCallCB != NULL){
				ConferenceCallFsmProc(peerCcmConn_p->ConfCallCB, EVT_CCM_3WAYCALL_MEETROOM_DESTROY,NULL,0);
			}
			#ifdef USADATANET
			threeWayCallState=_3WayCallState_None;
			#endif
			break;
	#endif
		case EVT_CCM_CONFCALL_MEETROOM_CONF:
		case EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_CONF:
		case EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_CONF:
			ConferenceCallFsmProc(ccmConn_p->ConfCallCB,ccmConnEvent,msg->data_p, msg->data_size);
			break;
#endif

		default:
			CCM_FSM_DBG1("undefined ccmConnEvent=", ccmConnEvent);
			break;
	}

terminate:
	return status;
} /* ccmConnFsmConnRdy */

/*______________________________________________________________________________
**	ccmConnFsmConnRel
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
ccmConnFsmConnRel(
	evtMsg_t *msg,
	ccmConn_t *ccmConn_p
) {
	int status = 0;
	uint16 ccmConnEvent = msg->event;
#if CCBS_SUPPORT
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
	ccmConn_t *peerCcmConn_p=NULL;
#endif

	CCM_FSM_DBG2("ccmConnFsmIdle, origPortId=", ccmConn_p->origPortId, " ccmConnEvent=", ccmConnEvent);

	switch( ccmConnEvent) {
		case EVT_CCM_REL_REQ:
		case EVT_CCM_DISC_REQ:		
			ccmConn_p->connSt = CALL_CONN_ST_IDLE;
			ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
			break;
		case EVT_CCM_DISC_IND: 			
			eventVoiceSimpleSend(EVT_CCM_DISC_ACK, ccmConn_p->origFuncId, ccmConn_p->origPortId);			
			break;
		case EVT_CCM_DISC_ACK:
#if CCBS_SUPPORT
			if (ccmConn_p->origPortType == SIP_PORT) {
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
				if(tmpConvertEntry_p!=NULL){
					if ( tmpConvertEntry_p->flags == SIP_CCM_TABLE_PERMANENT ) {
						CCM_PRINTF("receive EVT_CCM_DISC_ACK , SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
						resetSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt);
					} else {
						deleteSipCCMConvert(tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->callId, tmpConvertEntry_p->RoleSt, SIP_CCM_TABLE_DELETE_CHECK);
					}
				}
			}
#endif
			ccmConn_p->connSt = CALL_CONN_ST_IDLE;
			ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
			break;
		default:
			CCM_FSM_DBG1("undefined ccmConnEvent=", ccmConnEvent);
			break;
	}

	return status;
} /* ccmConnFsmConnRel */

/*______________________________________________________________________________
**	ccmConnFsmInteractive
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
ccmConnFsmInteractive(
	evtMsg_t *msg,
	ccmConn_t *ccmConn_p
) {
	int status = 0;
	ccmConn_t *peerCcmConn_p=NULL, *tmpCcmConn_p=NULL;
	uint16 ccmConnEvent = msg->event;

	CCM_FSM_DBG2("ccmConnFsmInteractive, origPortId=", ccmConn_p->origPortId, " ccmConnEvent=", ccmConnEvent);

	peerCcmConn_p = ccmConn_p->peerCcmConn;
	if ( peerCcmConn_p == NULL ) {
		CCM_FSM_DBG("peerCcmConn_p is NULL");
		goto terminate;
	}

	switch( ccmConnEvent) {
		case EVT_CCM_DIAL_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {
				eventVoiceDataSend(EVT_CCM_ASSIGN_CONF, ccmConn_p->origFuncId, ccmConn_p->origPortId, &peerCcmConn_p->origPortType, sizeof(uint16));
				while( peerCcmConn_p != NULL ) {
					eventVoiceDataSend(EVT_CCM_DIAL_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId, &ccmConn_p->origPortType, sizeof(uint16));
					peerCcmConn_p = peerCcmConn_p->bundleCcmList.bundleCcmNext;
				}
			}
			break;
		case EVT_CCM_ALERT_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) && (!( ccmConn_p->flags & CALL_CONN_FLAG_ALERT)) ) {
				if ( peerCcmConn_p != NULL ) {
					tmpCcmConn_p = ccmConn_p;
					while ( tmpCcmConn_p != NULL ) {
						tmpCcmConn_p->flags |= CALL_CONN_FLAG_ALERT;
						tmpCcmConn_p = tmpCcmConn_p->bundleCcmList.bundleCcmNext;
					}
					eventVoiceSimpleSend(EVT_CCM_ALERT_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				}
			}
			break;
		case EVT_CCM_CONN_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) ) {
				eventVoiceSimpleSend(EVT_CCM_CONN_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);	
				eventVoiceSimpleSend(EVT_CCM_CONN_CONF, ccmConn_p->origFuncId, ccmConn_p->origPortId);
				ccmConn_p->connSt = peerCcmConn_p->connSt = CALL_CONN_ST_INTERCONN;
				ccm_releaseOtherChann( ccmConn_p, peerCcmConn_p->peerCcmConn);
			}
			break;
		case EVT_CCM_REL_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {
				while( peerCcmConn_p != NULL ) {
					eventVoiceSimpleSend(EVT_CCM_REL_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
					ccmConn_p->connSt = CALL_CONN_ST_IDLE;
					ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
					peerCcmConn_p->connSt = CALL_CONN_ST_IDLE;
					peerCcmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
					peerCcmConn_p = peerCcmConn_p->bundleCcmList.bundleCcmNext;
				}
				ccm_releaseConn(ccmConn_p);
			}
			break;
		case EVT_CCM_DISC_REQ:
			if ( ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn_p->connRole == CCM_CONN_ROLE_PEER ) ) {
				eventVoiceSimpleSend(EVT_CCM_DISC_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				ccmConn_p->connSt = CALL_CONN_ST_IDLE;
				ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				peerCcmConn_p->connSt = CALL_CONN_ST_CONNREL;
				ccm_releaseConn(ccmConn_p);
			}
			break;
		default:
			CCM_FSM_DBG1("undefined ccmConnEvent=", ccmConnEvent);
			break;
	}

terminate:
	return status;
} /* ccmConnFsmInteractive */

/*______________________________________________________________________________
**	ccmConnFsmInterConnect
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
ccmConnFsmInterConnect(
	evtMsg_t *msg,
	ccmConn_t *ccmConn_p
) {
	int status = 0;
	uint16 ccmConnEvent = msg->event;
	ccmConn_t *peerCcmConn_p=NULL;

	CCM_FSM_DBG2("ccmConnFsmInterConnect, origPortId=", ccmConn_p->origPortId, " ccmConnEvent=", ccmConnEvent);

	peerCcmConn_p = ccmConn_p->peerCcmConn;
	if ( peerCcmConn_p == NULL ) {
		CCM_FSM_DBG("peerCcmConn_p is NULL");
		goto terminate;
	}

	switch( ccmConnEvent) {
		case EVT_CCM_DISC_REQ:
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
				eventVoiceSimpleSend(EVT_CCM_DISC_IND, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				eventVoiceSimpleSend(EVT_CCM_DISC_ACK, ccmConn_p->origFuncId, ccmConn_p->origPortId);
				ccmConn_p->connSt = CALL_CONN_ST_IDLE;
				ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
				peerCcmConn_p->connSt = CALL_CONN_ST_CONNREL;
				peerCcmConn_p->connRole = CCM_CONN_ROLE_PEER;
				ccm_releaseConn(ccmConn_p);
			}
			break;
		case EVT_CCM_DIAL_DIGIT:
			if ( ccmConn_p->flags & CALL_CONN_FLAG_ASSIGN ) {
				eventVoiceDataSend(EVT_CCM_DIAL_DIGIT, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId, msg->data_p, msg->data_size );
			}
			break;
		default:
			CCM_FSM_DBG1("undefined ccmConnEvent=", ccmConnEvent);
			break;
	}

terminate:
	return status;
} /* ccmConnFsmInterConnect */

#ifdef CENTRALIZE_LOG
void
ccmCentralizeLog(
	ccmConn_t *ccmConn_p,
	uint16 logsType
) {	
	char peerNumberStr[CALLHISTORY_MAX_CALLNUMBER];
	sipCCMConvertEntry_t *tmpConvertEntry_p=NULL;	
	char			timeStr[CALLHISTORY_MAX_TIME_LENGTH];
#ifdef VOIP_SYSLOG /*Jason , syslog*/
	char			voipsyslogbuf[TEXT_VOIP_SIZE];
	Config_VoIP_Stats_t logstatsbuf;
#endif
	phoneCh_t	*phoneCB = NULL;	
	int outCallIndex = 0, inCallIndex = 0;		
	VoiceProfLineObj_t* pLine = NULL;

	int LineItr = 0;	

	CCM_PRINTF("%s logType %d\n",__FUNCTION__, logsType);
#ifdef VOIP_SYSLOG /*Jason , syslog*/	
	zyMemset(&(voipsyslogbuf), 0, sizeof(voipsyslogbuf));
	zyMemset(&(timeStr), 0, sizeof(timeStr));
	zyMemset(&(logstatsbuf), 0, sizeof(Config_VoIP_Stats_t));
#endif
	switch(logsType){
		case CCM_LOGS_TYPE_OUTCALL_START:
		{		
			zyStrncpy(peerNumberStr, (char *)(&ccmConn_p->dialInform.dialDigitNumber.dial_digits[0]), CALLHISTORY_MAX_CALLNUMBER);			
			if ( ccmConn_p->peerCcmConn->origPortType == SIP_PORT ) {
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->peerCcmConn->origPortId);
				if(NULL != tmpConvertEntry_p)
				{
					#if 1 /*Eason, sipCallStatus*/
					tmpConvertEntry_p->sipCallStatus.status = TR104I_STATE_CALLING;
					zyStrncpy(tmpConvertEntry_p->sipCallStatus.peerNumber, peerNumberStr, WEB_CALLSTATUS_PHONENUM_LENGTH);
					tmpConvertEntry_p->sipCallStatus.callStartTime= time(NULL);					
					#endif
					
				}
				else{
					CCM_PRINTF("sipCallStatus failed\n");
				}
				#ifdef CALLHISTORY_SUPPORT				
				if(NULL != tmpConvertEntry_p){		
					CCM_PRINTF("receive CCM_LOGS_TYPE_OUTCALL_START, SipAccountId = %d\n", tmpConvertEntry_p->SipAccountId);
					LineItr = mapSipAccountIdToLineItr(tmpConvertEntry_p->SipAccountId);
					CCM_PRINTF("ccmCentralizeLog VpItr = %d\n", tmpConvertEntry_p->VpItr);
					CCM_PRINTF("ccmCentralizeLog LineItr = %d\n", LineItr);
					pLine = tr104VoiceLineObjPtrGet(tmpConvertEntry_p->VpItr, LineItr);
					if(NULL != pLine){						
						/*
						** The meaning of ccm webCallHistoryIndex
						** if InCalls
						** then 
						** 0 <= webCallHistoryIndex <MAX_CALL_HISTORY_ENTRY_INCALLS 
						** 
						** if OutCalls
						** then
						** MAX_CALL_HISTORY_ENTRY_INCALLS <=webCallHistoryIndex < MAX_CALL_HISTORY_ENTRY_INCALLS + MAX_CALL_HISTORY_ENTRY_OUTCALLS
						** 
						*/			
						outCallIndex = webCallHistory_pool->outCallIndex;
						ccmConn_p->webCallHistoryIndex = outCallIndex + MAX_CALL_HISTORY_ENTRY_INCALLS;
						ccmConn_p->peerCcmConn->webCallHistoryIndex = ccmConn_p->webCallHistoryIndex;
						zyMemset(&(webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex]), 0, sizeof(spWebCallHistoryEntry_t));
						
						CCM_PRINTF("Outgoing SIP Call Start, webCallHistoryIndex %d, outCallIndex %d\n", ccmConn_p->webCallHistoryIndex, outCallIndex);
					
						
						zyStrncpy(webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryLocalNumber, pLine->directoryNumber, CALLHISTORY_MAX_CALLNUMBER);										
						snprintf(webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryPeerNumber, CALLHISTORY_MAX_CALLNUMBER, "%s", peerNumberStr);			
						CCM_PRINTF("Outgoing Call ,PeerNUmber:%s\n", webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryPeerNumber);
						
						webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].catalogClass = WEB_CALL_HISTORY_CATALOG_DIALED;			
						webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].portType= ccmConn_p->origPortType;
						webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].portNum = find_phonePhyId(ccmConn_p->origPortId);
#ifdef VOIP_SYSLOG /*Jason , syslog*/
						webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].SipAccountId = tmpConvertEntry_p->SipAccountId;
#endif
						/* time */
						webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryStartTime = time(NULL);						
						CCM_PRINTF("Outgoing SIP Call Start, callHistoryStartTime: %d\n", webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryStartTime);
/*Jason , syslog*/
#ifdef VOIP_SYSLOG
						sprintf(voipsyslogbuf, "Start Outgoing Call [%s]",  webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryPeerNumber);
						voipSyslog(LOG_CALL_STATISTICS,LOG_INFO,webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryLocalNumber,webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].portNum,voipsyslogbuf);
#endif
						(webCallHistory_pool->outCallIndex)++;
						if(webCallHistory_pool->outCallIndex >= MAX_CALL_HISTORY_ENTRY_OUTCALLS){
							webCallHistory_pool->outCallIndex = 0;
						}	
												
					}/*NULL!=pLine*/
				}/*NULL!=tmpConvertEntry_p*/
				#endif
			}
		
		} /*case CCM_LOGS_TYPE_OUTCALL_START*/
		break;
		case CCM_LOGS_TYPE_INCALL_START:  
		{		
			if ( ccmConn_p->origPortType == SIP_PORT ) {				
				if ( ccmConn_p->dialInform.dialDigitNumber.current_length != 0 ){
					zyStrncpy(peerNumberStr, (char *)(&ccmConn_p->dialInform.dialDigitNumber.dial_digits[0]), CALLHISTORY_MAX_CALLNUMBER);
				}
				else{
					zyStrcpy(peerNumberStr, "unknown");
				}
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
				if(NULL != tmpConvertEntry_p)
				{					
					#if 1 /*Eason, sipCallStatus*/
					zyStrncpy(tmpConvertEntry_p->sipCallStatus.peerNumber, peerNumberStr, WEB_CALLSTATUS_PHONENUM_LENGTH);
					CCM_PRINTF("peerNumber = %s\n", tmpConvertEntry_p->sipCallStatus.peerNumber);
					tmpConvertEntry_p->sipCallStatus.status = TR104I_STATE_RINGING;
					tmpConvertEntry_p->sipCallStatus.callStartTime= time(NULL);					
					#endif
					
				}
				else{
					CCM_PRINTF("sipCallStatus failed\n");
				}
				#ifdef CALLHISTORY_SUPPORT				
				if ( tmpConvertEntry_p != NULL ) {					
					CCM_PRINTF("receive CCM_LOGS_TYPE_INCALL_START, SipAccountId = %d\n", tmpConvertEntry_p->SipAccountId);
					LineItr = mapSipAccountIdToLineItr(tmpConvertEntry_p->SipAccountId);
					CCM_PRINTF("receive CCM_LOGS_TYPE_INCALL_START, VpItr = %d\n", tmpConvertEntry_p->VpItr);
					CCM_PRINTF("receive CCM_LOGS_TYPE_INCALL_START, LineItr = %d\n", LineItr);
					pLine = tr104VoiceLineObjPtrGet(tmpConvertEntry_p->VpItr, LineItr);

					inCallIndex = webCallHistory_pool->inCallIndex;
					ccmConn_p->webCallHistoryIndex = inCallIndex;
					ccmConn_p->peerCcmConn->webCallHistoryIndex = ccmConn_p->webCallHistoryIndex;
					zyMemset(&(webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex]), 0, sizeof(spWebCallHistoryEntry_t));

					CCM_PRINTF("Incoming SIP Call Start, webCallHistoryIndex %d, inCallIndex %d\n", ccmConn_p->webCallHistoryIndex, inCallIndex);
					
					
					

					snprintf(webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryPeerNumber, CALLHISTORY_MAX_CALLNUMBER, "%s", peerNumberStr);			
					CCM_PRINTF("Incoming Call ,PeerNUmber:%s\n", webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryPeerNumber);

					zyStrncpy(webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryLocalNumber, pLine->directoryNumber, CALLHISTORY_MAX_CALLNUMBER);
					//webCallHistory_pool->webCallHistoryEntryInCalls[ccmConn_p->webCallHistoryIndex].callHistoryPhyId = find_phonePhyId(ccmConn_p->peerCcmConn->origPortId);						
#ifdef VOIP_SYSLOG  /*Jason , syslog*/
					webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].SipAccountId = tmpConvertEntry_p->SipAccountId;
#endif					
					/* time */
					webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryStartTime = time(NULL);
					CCM_PRINTF("Incoming Call, callHistoryStartTime: %d\n", webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryStartTime);					
/*Jason , syslog*/
#ifdef VOIP_SYSLOG					
					sprintf(voipsyslogbuf, "Start Incoming Call [%s]",  webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryPeerNumber);
					voipSyslog(LOG_CALL_STATISTICS,LOG_INFO,webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryLocalNumber,webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].portNum,voipsyslogbuf);
#endif
					
					(webCallHistory_pool->inCallIndex)++;/*For next item*/
					if(webCallHistory_pool->inCallIndex>=MAX_CALL_HISTORY_ENTRY_INCALLS){
						webCallHistory_pool->inCallIndex = 0;
					}					
				}
				#endif		
			}
		} /*case CCM_LOGS_TYPE_INCALL_START*/
		break;
		/* Outgoing Call Established */
		case CCM_LOGS_TYPE_OUTCALL_ESTABLISH:  
		{		
			#ifdef CALLHISTORY_SUPPORT
				CCM_PRINTF("Outgoing Call Established, webCallHistoryIndex %d\n", ccmConn_p->webCallHistoryIndex);
				outCallIndex = ccmConn_p->webCallHistoryIndex - MAX_CALL_HISTORY_ENTRY_INCALLS;
				CCM_PRINTF("Outgoing Call Established, outCallIndex %d\n", outCallIndex);
				if((outCallIndex<0) || (outCallIndex>=MAX_CALL_HISTORY_ENTRY_OUTCALLS)){
					ZyPrintf("outcall index out of range %d\n", outCallIndex);
					return;
				}

				CCM_PRINTF("Outgoing Call Established, LocalNumber:%s\n", webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryLocalNumber);
				CCM_PRINTF("Outgoing Call Established, PeerNUmber:%s\n", webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryPeerNumber);
				
				webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryTalkStartTime = (uint32)time(NULL);
				CCM_PRINTF("Outgoing Call Established, TalkStartTime: %d\n", webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryTalkStartTime);
/*Jason , syslog*/
#ifdef VOIP_SYSLOG				
				sprintf(voipsyslogbuf, "Outgoing Call Established [%s]",  webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryPeerNumber);
				voipSyslog(LOG_CALL_STATISTICS,LOG_INFO,webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryLocalNumber,webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].portNum,voipsyslogbuf);
#endif	
				//dbg_printf("\r\n Call End 1, Rtp up time:%s", timeStr);				
			#endif		
			if ( ccmConn_p->origPortType == SIP_PORT ) {
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
				if(NULL != tmpConvertEntry_p){					
					#if 1 /*Eason, sipCallStatus*/
					tmpConvertEntry_p->sipCallStatus.callStartTime= time(NULL);	
					tmpConvertEntry_p->sipCallStatus.status = TR104I_STATE_INCALL;
					CCM_PRINTF("sipCallStatus SipAccountId %d, callStartTime = %d\n", tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->sipCallStatus.callStartTime);				
					#endif					
				}
				else{
					CCM_PRINTF("sipCallStatus failed\n");
				}
			}
			
		} /*case CCM_LOGS_TYPE_OUTCALL_ESTABLISH*/
		break;
		case CCM_LOGS_TYPE_INCALL_ESTABLISH:
		{
			#ifdef CALLHISTORY_SUPPORT
			CCM_PRINTF("Incoming Call Established, webCallHistoryIndex %d\n", ccmConn_p->webCallHistoryIndex);
			inCallIndex = ccmConn_p->webCallHistoryIndex;
			CCM_PRINTF("Incoming Call Established, inCallIndex %d\n",inCallIndex);
			if(inCallIndex<0 || inCallIndex>=MAX_CALL_HISTORY_ENTRY_INCALLS){
					ZyPrintf("incall index out of range %d\n", inCallIndex);
					return;
			}
			
			if ( ccmConn_p->dialInform.dialDigitNumber.current_length != 0 ) {
				zyStrncpy(peerNumberStr, (char *)(&ccmConn_p->dialInform.dialDigitNumber.dial_digits[0]), CALLHISTORY_MAX_CALLNUMBER);
			}
			else{
				zyStrcpy(peerNumberStr, "unknown");
			}
				

			CCM_PRINTF("Incoming Call Established, LocalNumber:%s\n", webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryLocalNumber);
			CCM_PRINTF("Incoming Call Established, PeerNUmber:%s\n", webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryPeerNumber);

			webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryTalkStartTime = (uint32)time(NULL);
			CCM_PRINTF("Incoming Call Established, TalkStartTime: %d\n", webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryTalkStartTime);
			webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].catalogClass = WEB_CALL_HISTORY_CATALOG_RECEIVED; 
			webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].portType = ccmConn_p->peerCcmConn->origPortType;
			webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].portNum = find_phonePhyId(ccmConn_p->peerCcmConn->origPortId);		
/*Jason , syslog*/
#ifdef VOIP_SYSLOG			
			sprintf(voipsyslogbuf, "Incoming Call Established [%s]",  webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryPeerNumber);
			voipSyslog(LOG_CALL_STATISTICS,LOG_INFO,webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryLocalNumber,webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].portNum,voipsyslogbuf);
#endif	
			#endif
			if ( ccmConn_p->origPortType == SIP_PORT ) {
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
				if(NULL != tmpConvertEntry_p){					
					#if 1 /*Eason, sipCallStatus*/
					tmpConvertEntry_p->sipCallStatus.callStartTime= time(NULL);	
					tmpConvertEntry_p->sipCallStatus.status = TR104I_STATE_INCALL;
					CCM_PRINTF("sipCallStatus SipAccountId %d, callStartTime = %d\n", tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->sipCallStatus.callStartTime);						
					#endif					
				}
				else{
					CCM_PRINTF("sipCallStatus failed\n");
				}
			}
			
		}/*case CCM_LOGS_TYPE_INCALL_ESTABLISH*/
		break;
		case CCM_LOGS_TYPE_END_CALL:
		{
			CCM_PRINTF("Call End, ccmConn_p->webCallHistoryIndex = %d\n", ccmConn_p->webCallHistoryIndex);

			if ( ccmConn_p->origPortType == SIP_PORT ) {
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);		
				if(NULL != tmpConvertEntry_p){					
					#if 1 /*Eason, sipCallStatus*/					
					/*reset sipCallStatus, selfNumber should not clear*/
					tmpConvertEntry_p->sipCallStatus.status=0;
					tmpConvertEntry_p->sipCallStatus.callStartTime=0;
					zyMemset(tmpConvertEntry_p->sipCallStatus.peerNumber, 0, sizeof(tmpConvertEntry_p->sipCallStatus.peerNumber));
					zyMemset(tmpConvertEntry_p->sipCallStatus.codec, 0, sizeof(tmpConvertEntry_p->sipCallStatus.codec));					
					#endif					
				}
				else{
					CCM_PRINTF("sipCallStatus failed\n");
				}
			}
			else if ( (NULL!= ccmConn_p->peerCcmConn) && (ccmConn_p->peerCcmConn->origPortType == SIP_PORT) ) {
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->peerCcmConn->origPortId);
				if(NULL != tmpConvertEntry_p){					
					#if 1 /*Eason, sipCallStatus*/
					/*reset sipCallStatus, selfNumber should not clear*/
					tmpConvertEntry_p->sipCallStatus.status=0;
					tmpConvertEntry_p->sipCallStatus.callStartTime=0;
					zyMemset(tmpConvertEntry_p->sipCallStatus.peerNumber, 0, sizeof(tmpConvertEntry_p->sipCallStatus.peerNumber));
					zyMemset(tmpConvertEntry_p->sipCallStatus.codec, 0, sizeof(tmpConvertEntry_p->sipCallStatus.codec));					
					#endif					
				}
				else{
					CCM_PRINTF("sipCallStatus failed\n");
				}
			}
			#ifdef CALLHISTORY_SUPPORT
			if ((ccmConn_p->webCallHistoryIndex) >= MAX_CALL_HISTORY_ENTRY_INCALLS ) { /*OUTCALLS*/
			
				
					outCallIndex = ccmConn_p->webCallHistoryIndex - MAX_CALL_HISTORY_ENTRY_INCALLS;
					if(outCallIndex >= MAX_CALL_HISTORY_ENTRY_OUTCALLS ){
						ZyPrintf("out of range\n");
						return;
					}				
				
					if(webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryStartTime > 0){
						CCM_PRINTF("Out Call End, LocalNumber:%s\n", webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryLocalNumber);
						CCM_PRINTF("Out Call End, PeerNUmber:%s\n", webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryPeerNumber);
						/*callHistoryConnectType*/
						webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryConnectType = SIP_PORT;

						
						if(webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryTalkStartTime !=0){
							webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryTalkDuration
							= (uint32)time(NULL) - webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryTalkStartTime;
						}else{
							webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryTalkDuration = 0;
						}			
						CCM_PRINTF("Out Call End, Duration: %d\n", webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryTalkDuration);
						webCallHistorySummaryAddEntry(&(webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex]));
					}
	/*Jason syslog*/					
	#ifdef VOIP_SYSLOG					
					sprintf(voipsyslogbuf, "Out Call End [%s],Duration: %d",  webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryPeerNumber,webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryTalkDuration);
					voipSyslog(LOG_CALL_STATISTICS,LOG_INFO,webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryLocalNumber,webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].portNum,voipsyslogbuf);
					#if 1 /*Jason , */
					if(webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryTalkDuration>0){					
						CCM_PRINTF("receive CCM_LOGS_TYPE_END_CALL outCall, SipAccountId = %d\n" , webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].SipAccountId);
						vDspStreamStatsReq(webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].SipAccountId,0,&(logstatsbuf));
						sprintf(voipsyslogbuf,"rx pkts %d,rx bytes %d,lost pkts %d,avg jitter %d,tx pkts %d,tx bytes %d", \
							logstatsbuf.PacketsReceived,logstatsbuf.BytesReceived, \
							logstatsbuf.PacketsLost,logstatsbuf.AverageReceiveInterarrivalJitter,logstatsbuf.PacketsSent,logstatsbuf.BytesSent);
						ZyPrintf("voipsyslogbuf %s\n",voipsyslogbuf);
						voipSyslog(LOG_CALL_STATISTICS,LOG_INFO,webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].callHistoryLocalNumber,webCallHistory_pool->webCallHistoryEntryOutCalls[outCallIndex].portNum,voipsyslogbuf);				
					}
					#endif
	#endif														
			}
			else if ((ccmConn_p->webCallHistoryIndex) >= 0) { /*INCALLS*/				
				
					inCallIndex = ccmConn_p->webCallHistoryIndex;
					if(webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryStartTime >0){
					//dennis_CallHistory								
						CCM_PRINTF("In Call End, LocalNumber:%s\n", webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryLocalNumber);
						CCM_PRINTF("In Call End, PeerNUmber:%s\n", webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryPeerNumber);
						/*callHistoryConnectType*/
						webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryConnectType = SIP_PORT;
						
						if(webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryTalkStartTime !=0){
							webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryTalkDuration
							= (uint32)time(NULL) - webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryTalkStartTime;
						}else{
							webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryTalkDuration = 0;
						}		
						CCM_PRINTF("In Call End, Duration: %d\n", webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryTalkDuration);
						webCallHistorySummaryAddEntry(&(webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex]));						
					}
	/*Jason , syslog*/					
	#ifdef VOIP_SYSLOG					
					sprintf(voipsyslogbuf, "In Call End [%s],Duration: %d",  webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryPeerNumber,webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryTalkDuration);
					voipSyslog(LOG_CALL_STATISTICS,LOG_INFO,webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryLocalNumber,webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].portNum,voipsyslogbuf);
				#if 1 /*Jason , */
					if(webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryTalkDuration>0){
						CCM_PRINTF("receive CCM_LOGS_TYPE_END_CALL inCall, SipAccountId = %d\n" , webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].SipAccountId);
						vDspStreamStatsReq(webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].SipAccountId,0,&(logstatsbuf));									
						sprintf(voipsyslogbuf,"rx pkts %d,rx bytes %d,lost pkts %d,avg jitter %d,tx pkts %d,tx bytes %d", \
							logstatsbuf.PacketsReceived,logstatsbuf.BytesReceived, \
							logstatsbuf.PacketsLost,logstatsbuf.AverageReceiveInterarrivalJitter,logstatsbuf.PacketsSent,logstatsbuf.BytesSent);

						voipSyslog(LOG_CALL_STATISTICS,LOG_INFO,webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].callHistoryLocalNumber,webCallHistory_pool->webCallHistoryEntryInCalls[inCallIndex].portNum,voipsyslogbuf);				
					}
				#endif 	
	#endif							
			}

			ccmConn_p->webCallHistoryIndex = -1;
			if(NULL != ccmConn_p->peerCcmConn ){
				ccmConn_p->peerCcmConn->webCallHistoryIndex = -1;
			}
			#endif
		} /*case CCM_LOGS_TYPE_END_CALL*/
		break;
	}/*switch logsType*/
}

#endif


/*______________________________________________________________________________
**  create_ccmConn
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
ccmConn_t*
create_ccmConn(
	uint8 portType, 
	uint16 chId, 
	uint16  origFuncId,
	uint8 phoneSelect
) {
	ccmConn_t *tmpCcmConn=NULL, **hp = NULL;
	int i_state;	

	switch( portType ) {
		case DSP_FXS_PORT:
			hp = &(ccmConnCb.fxsCcmConn);
			break;
		case DSP_FXO_PORT:
		case DSP_FXO_LIFELINE:
			hp = &(ccmConnCb.fxoCcmConn);
			break;
		case SIP_PORT:
			hp = &(ccmConnCb.sipCcmConn);
			break;
		case ZYXEL_PHONE_PORT:
			hp = &(ccmConnCb.zyxelCcmConn);
			break;
		default:
			ZyPrintf("error portType at create_ccmConn()\n\r", portType);
			return NULL;
	}

	if ( ( tmpCcmConn = (ccmConn_t *)zyMalloc( sizeof(ccmConn_t) ) ) == NULL ) {
		ZyPrintf("alloc ccmConn_t error\n\r");
		return NULL;
	}

	/* Assign the initial value */
	zyMemset(tmpCcmConn, 0, sizeof(ccmConn_t) );
	tmpCcmConn->connSt = CALL_CONN_ST_IDLE;
	tmpCcmConn->origPortId = chId;
	tmpCcmConn->origPortType = portType;
	tmpCcmConn->callFeatures = 0;
	tmpCcmConn->origFuncId = origFuncId;
	tmpCcmConn->SipFxsMap = phoneSelect;
	tmpCcmConn->peerCcmConn = NULL;
	tmpCcmConn->bundleCcmList.bundleCcmNext = NULL;
	tmpCcmConn->bundleCcmList.bundleCcmPrev = NULL;
	tmpCcmConn->callTransferCB = NULL;
	tmpCcmConn->ConfCallCB = NULL;	/* CALL_CONFERENCE */
	tmpCcmConn->phoneFeatureCb = NULL;	
	#ifdef CALLHISTORY_SUPPORT
	tmpCcmConn->webCallHistoryIndex = -1;
	#endif
	constructCallFeatureCb(tmpCcmConn);
	tmpCcmConn->callRouteCb = NULL;	
	constructCallRouteCb(tmpCcmConn);
	/* bind this entry to the header of the link list */
	i_state = dirps();
	tmpCcmConn->prev = NULL;
	tmpCcmConn->next = *hp;
	if(tmpCcmConn->next != NULL) {
		tmpCcmConn->next->prev = tmpCcmConn;
	}
	*hp = tmpCcmConn;
	restore(i_state);

	return tmpCcmConn;
}

/*______________________________________________________________________________
**  findOrigCcmConn
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
ccmConn_t*
findOrigCcmConn(
	uint8 portType, 
	uint16 chId
){
	ccmConn_t *tmpCcmConn=NULL;

	switch( portType ) {
		case DSP_FXS_PORT:
			tmpCcmConn = ccmConnCb.fxsCcmConn;
			break;
		case DSP_FXO_PORT:
		case DSP_FXO_LIFELINE:
			tmpCcmConn = ccmConnCb.fxoCcmConn;
			break;
		case SIP_PORT:
			tmpCcmConn = ccmConnCb.sipCcmConn;
			break;
		case ZYXEL_PHONE_PORT:
			tmpCcmConn = ccmConnCb.zyxelCcmConn;
			break;
		default:
			ZyPrintf("wrong portType at findOrigCcmConn()\n\r", portType);
			return NULL;
	}

	while ( tmpCcmConn != NULL ) {
		if ( tmpCcmConn->origPortId == chId ) {
			return tmpCcmConn;
		} else {
			tmpCcmConn = tmpCcmConn->next;
		}
	}

	ZyPrintf("can't find match CCM \r\n");

	return NULL;
}

/*______________________________________________________________________________
**  delete_ccmConn
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void
delete_ccmConn(
	uint8 portType, 
	uint16 chId
) {
	ccmConn_t *tmpCcmConn=NULL, **hp = NULL, *headCcmConn=NULL;
	int i_state, found=0;

	switch( portType ) {
		case DSP_FXS_PORT:
			hp = &(ccmConnCb.fxsCcmConn);
			break;
		case DSP_FXO_PORT:
		case DSP_FXO_LIFELINE:
			hp = &(ccmConnCb.fxoCcmConn);
			break;
		case SIP_PORT:
			hp = &(ccmConnCb.sipCcmConn);
			break;
		case ZYXEL_PHONE_PORT:
			hp = &(ccmConnCb.zyxelCcmConn);
			break;
		default:
			ZyPrintf("error portType at delete_ccmConn()\n\r", portType);
			return;
	}

	tmpCcmConn = *hp;
	while ( tmpCcmConn != NULL ) {
		if ( tmpCcmConn->origPortId == chId ) {
			found = 1;
			break;
		} else {
			tmpCcmConn = tmpCcmConn->next;
		}
	}

	if ( found == 1 ) {
		/* Remove the entry from the prev/next link list */
		i_state = dirps();
		if ( tmpCcmConn->prev != NULL ) {
			tmpCcmConn->prev->next = tmpCcmConn->next;
		} else {
			*hp = tmpCcmConn->next;
		}
		if ( tmpCcmConn->next != NULL ) {
			tmpCcmConn->next->prev = tmpCcmConn->prev;
		}
		tmpCcmConn->next = NULL;
		tmpCcmConn->prev = NULL;

		/* remove the entry from the bundle/peer link list */
		if ( tmpCcmConn->peerCcmConn != NULL ) {
			if ( tmpCcmConn->peerCcmConn->peerCcmConn != NULL ) {
				headCcmConn = tmpCcmConn->peerCcmConn->peerCcmConn;
				if ( headCcmConn != tmpCcmConn ) {
					tmpCcmConn->bundleCcmList.bundleCcmPrev->bundleCcmList.bundleCcmNext = tmpCcmConn->bundleCcmList.bundleCcmNext;
				} else {
					headCcmConn = tmpCcmConn->bundleCcmList.bundleCcmNext;
				}
			}
		}
		tmpCcmConn->peerCcmConn = NULL;
		tmpCcmConn->bundleCcmList.bundleCcmNext = NULL;
		tmpCcmConn->bundleCcmList.bundleCcmPrev = NULL;
		tmpCcmConn->callTransferCB = NULL;		/* CALL_TRANSFER */
		tmpCcmConn->ConfCallCB = NULL;		/* CALL_CONFERENCE */
		restore(i_state);

		zyFree(tmpCcmConn);
		tmpCcmConn = NULL;
	}

	return;
}

/*______________________________________________________________________________
**  ccm_associateConn
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
ccm_associateConn(
	evtMsg_t *msg,
	ccmConn_t* ccmConn_p
){
	int routeSelect = 0;
	int connType = CCM_CONN_TYPE_FAIL, exitFlag = FALSE, routeIndex = 0, phoneFeature = 0 ;
	dialInfor_t *dialInform = NULL;
	ccmPhoneFeatureCb_t *tmpPhoneFeatureCb = NULL;
	ccmCallRouteCb_t *tmpCallRouteCb = NULL;

	CCM_FSM_DBG1("ccm_associateConn, origPortType=", ccmConn_p->origPortType);

	/*if(msg->data_p !=  NULL ){*/
	if(msg->data_size !=  0 ) {
		dialInform = (dialInfor_t *)msg->data_p;
		zyMemcpy( &(ccmConn_p->dialInform), dialInform, sizeof(dialInfor_t) );
	}

	if (ccmConn_p->phoneFeatureCb != NULL){
		tmpPhoneFeatureCb = ccmConn_p->phoneFeatureCb;
		while( tmpPhoneFeatureCb != NULL) {
			exitFlag = FALSE;
			if(tmpPhoneFeatureCb->funcPtr != NULL){
				tmpPhoneFeatureCb->funcPtr(ccmConn_p, &phoneFeature, &exitFlag);
			}
			if(exitFlag == TRUE) {
				break;
			}
			tmpPhoneFeatureCb = tmpPhoneFeatureCb->next;
		}
	}
	CCM_FSM_DBG1("ccm_associateConn, phoneFeature = ", phoneFeature);	
	
	/* if have phoneFeature , use phoneFeature to check the outgoing rule 
	*/
	if (ccmConn_p->callRouteCb != NULL){
		tmpCallRouteCb = ccmConn_p->callRouteCb;
		while( tmpCallRouteCb != NULL) {
			exitFlag = FALSE;
			if(tmpCallRouteCb->funcPtr != NULL){
				tmpCallRouteCb->funcPtr(ccmConn_p, &routeSelect, phoneFeature, &exitFlag);
			}
			if(exitFlag == TRUE) {
				break;
			}
			tmpCallRouteCb = tmpCallRouteCb->next;
		}
	}
	CCM_FSM_DBG1("ccm_associateConn, routeSelect = ", routeSelect);
	
	for(routeIndex = 0 ; routeIndex < MAX_PORT_ROUTE ; routeIndex++) {
		if(routeSelect & (1<< routeIndex)) {
			CCM_FSM_DBG2("associate from ", ccmConn_p->origPortType, " to ", routeIndex);
			if(ccmAssociateTable[ccmConn_p->origPortType][routeIndex] != NULL) {
				connType = ccmAssociateTable[ccmConn_p->origPortType][routeIndex](ccmConn_p, phoneFeature);
			}
		}
	}

	if ( connType == CCM_CONN_TYPE_FAIL ) {
		ccmConn_p->connType = CCM_CONN_TYPE_FAIL;
		ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
		zyMemset( &(ccmConn_p->dialInform.dialDigitNumber), 0, sizeof(dialDigitNum_t) );
	}

	CCM_FSM_DBG1("ccm_associateConn, connType=", connType);	
	return connType;
}

/*______________________________________________________________________________
**  associateFxs2FxoLife
**
**  descriptions: Assoicate the FXS with FXO-LifeLine CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
associateFxs2FxoLife(
	ccmConn_t* ccmConn_p,
	int fxsTranslate
) {
	ccmConn_t *tmpCcmConn=NULL;
	int connType = CCM_CONN_TYPE_FAIL;

	tmpCcmConn = ccmConnCb.fxoCcmConn;
	while ( tmpCcmConn != NULL ) {
		if ( (tmpCcmConn->connSt == CALL_CONN_ST_IDLE) && (tmpCcmConn->flags & CALL_CONN_FLAG_REGISTER) ) {
			ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_CONNOUT, CCM_CONN_TYPE_CALLER_CALLER);
			connType = CCM_CONN_TYPE_CALLER_CALLER;
			if ( fxsTranslate == 2 ) 	{		/* PSTN LifeLine Relay directly */
				ccmConn_p->flags |= CALL_CONN_FLAG_PREFIX;
				tmpCcmConn->flags |= CALL_CONN_FLAG_PREFIX;
			}
			break;
		}
		tmpCcmConn = tmpCcmConn->next;
	}

	return connType;
}

/*______________________________________________________________________________
**  associateFxs2Sip
**
**  descriptions: Assoicate the FXS with SIP CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
associateFxs2Sip(
	ccmConn_t* ccmConn_p,
	int phoneFeature
) {
	int connType = CCM_CONN_TYPE_FAIL;
	int sipLineId=0;
	ccmConn_t *tmpCcmConn=NULL, *newCcmConn_p = NULL, *prevCcmConn=NULL, *prevPeerCcmConn=NULL;
	sipCCMConvertEntry_t *tmpConvertEntry_p=NULL;
	int firstAvailSipLineId = -1;
#ifdef ZYNOS_CALL_TRANSFER /*Eason, ZyNOS_Transfer*/
	phoneCh_t *phoneCB = NULL;
#endif

	CCM_FSM_DBG("associate FXS to SIP");

	tmpCcmConn = ccmConnCb.sipCcmConn;
	/*move to last ccm in list which is the first created ccm*/
	while ( tmpCcmConn != NULL ) {
		if(tmpCcmConn->next == NULL){
			break;
		}
		tmpCcmConn = tmpCcmConn->next;
	}
	/*search from tail to head, in ccm create time(small origPortId first) order */
	while ( tmpCcmConn != NULL ) {
		tmpConvertEntry_p = findSipCCMConvertByPortId(tmpCcmConn->origPortId);
		if ( tmpConvertEntry_p != NULL ) {
			CCM_PRINTF("associateFxs2Sip > ENTER, SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
			sipLineId = tmpConvertEntry_p->SipAccountId;
                        
			CCM_FSM_DBG1("associateFxs2Sip, sipIndex=", sipLineId);

			/* aircheng add for outgoing sip */
			if ( phoneFeature == CALL_FEATURE_OUTGOING_SIP ) {
				if (ccmConn_p->outgoingSipAccountId == -1){
					CCM_PRINTF("%s: Error!! outgoingSipAccountId = %d\n", __FUNCTION__, ccmConn_p->outgoingSipAccountId);
					break;
				}
				if (ccmConn_p->outgoingSipAccountId == sipLineId) {
					if ( !(tmpCcmConn->flags & CALL_CONN_FLAG_REGISTER) ) {
						return connType;
					}
					if(-1 == firstAvailSipLineId){
						firstAvailSipLineId = sipLineId;
					}
					CCM_FSM_DBG1("tmpCcmConn->connSt = ", tmpCcmConn->connSt);
					if ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) {
						ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_CONNOUT, CCM_CONN_TYPE_CALLER_CALLER);
						connType = CCM_CONN_TYPE_CALLER_CALLER;
						#ifdef CENTRALIZE_LOG
						ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_START);
						#endif
						break;
					}
				}
			}
			else{
				if ( ( ccmConn_p->SipFxsMap & (1<<sipLineId) ) && ( tmpCcmConn->flags & CALL_CONN_FLAG_REGISTER ) ) {
					if(-1 == firstAvailSipLineId){
						firstAvailSipLineId = sipLineId;
					}
					CCM_FSM_DBG1("tmpCcmConn->connSt = ", tmpCcmConn->connSt);
					if ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) {
						ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_CONNOUT, CCM_CONN_TYPE_CALLER_CALLER);
						connType = CCM_CONN_TYPE_CALLER_CALLER;
						#ifdef CENTRALIZE_LOG
						ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_START);
						#endif
						break;
					}
				}
			}
		}
		tmpCcmConn = tmpCcmConn->prev;
	}
#ifdef MULTI_SIP_SESSION
	if ( connType == CCM_CONN_TYPE_FAIL ) {		
		prevCcmConn = findOrigCcmConn(DSP_FXS_PORT, ccmConn_p->dialInform.prevLogicId);
		if(prevCcmConn != NULL) {
			if ( (prevPeerCcmConn = prevCcmConn->peerCcmConn) != NULL ) {
				if ( prevPeerCcmConn->origPortType == SIP_PORT ) {
					tmpConvertEntry_p = findSipCCMConvertByPortId(prevPeerCcmConn->origPortId);
					if ( tmpConvertEntry_p != NULL ) {
						CCM_PRINTF("associateFxs2Sip > ENTER CCM_CONN_TYPE_FAIL, SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
						sipLineId = tmpConvertEntry_p->SipAccountId;
						CCM_FSM_DBG1("ccm_associateConn, firstMatchConvertEntry_p->sipIndex=", sipLineId);
						#ifdef ZYNOS_CALL_TRANSFER /*Eason, ZyNOS_Transfer*/
						{
							int VpItr = 0, LineItr = 0;	
							VoiceProfLineCallingFeaturesObj_t *pLineFeature = NULL;
							VpItr = mapSipAccountIdToVpItr(sipLineId);
							LineItr = mapSipAccountIdToLineItr(sipLineId);	
							CCM_PRINTF("VpItr = %d, LineItf = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
							if(phoneFeature == CALL_FEATURE_CALL_TRANSFER){
								if(pLineFeature->callTransferEnable){
									phoneCB = find_phoneCB(ccmConn_p->origPortId, 0);
									if((NULL != phoneCB) && (phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE)){
										phoneCB->phoneFlags |= PHONE_ATTENDED_TRANSFER;
										CCM_PRINTF("phoneCB logicalId = %d, Attended Tranfer flag turned on\n", phoneCB->logicalId);
									}
								}
								else{
									CCM_PRINTF("Call Transfer disabled for VpItr %d, LineItr %d, Call Transfer feature key not accept here\n", VpItr, LineItr);
									return connType;
								}
							}
							
							if ( (newCcmConn_p = addSipCCMConvert(SIP_CCM_CONVERT_ST_IDLE, SIP_CCM_TABLE_TEMPORARY, sipLineId, 0) ) != NULL ) {
								if ( ccmConn_p->SipFxsMap & (1<<sipLineId) ) {
									if ( newCcmConn_p->connSt == CALL_CONN_ST_IDLE ) {
										connType = CCM_CONN_TYPE_CALLER_CALLER;
										ccm_bundleConn(ccmConn_p, newCcmConn_p, CALL_CONN_ST_CONNOUT, CCM_CONN_TYPE_CALLER_CALLER);
										#ifdef CENTRALIZE_LOG
										ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_START);
										#endif
									}
								}
							}	
						}
						#else
						if ( phoneFeature == CALL_FEATURE_CALL_TRANSFER) {
							createCallTransferCB(ccmConn_p, prevPeerCcmConn, CALL_TRANSFEROR, CALL_TRANSFER_TYPE_UNATTENDED);
						} else {
							if ( (newCcmConn_p = addSipCCMConvert(SIP_CCM_CONVERT_ST_IDLE, SIP_CCM_TABLE_TEMPORARY, sipLineId, 0) ) != NULL ) {
								if ( ccmConn_p->SipFxsMap & (1<<sipLineId) ) {
									if ( newCcmConn_p->connSt == CALL_CONN_ST_IDLE ) {
										connType = CCM_CONN_TYPE_CALLER_CALLER;
										ccm_bundleConn(ccmConn_p, newCcmConn_p, CALL_CONN_ST_CONNOUT, CCM_CONN_TYPE_CALLER_CALLER);
										#ifdef CENTRALIZE_LOG
										ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_START);
										#endif
									}
								}
							}
						}
						#endif
					}
				}
			}
		}
		else{
			CCM_FSM_DBG("MULTI SIP SESSION, use first registered sipCCMConvert");
			if ( (-1!=firstAvailSipLineId) && (newCcmConn_p = addSipCCMConvert(SIP_CCM_CONVERT_ST_IDLE, SIP_CCM_TABLE_TEMPORARY, firstAvailSipLineId, 0) ) != NULL ) {				
				if ( newCcmConn_p->connSt == CALL_CONN_ST_IDLE ) {
					connType = CCM_CONN_TYPE_CALLER_CALLER;
					ccm_bundleConn(ccmConn_p, newCcmConn_p, CALL_CONN_ST_CONNOUT, CCM_CONN_TYPE_CALLER_CALLER);
					#ifdef CENTRALIZE_LOG
					ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_START);
					#endif
				}
				
			}
		}

	}
#endif
	return connType;
}

/*______________________________________________________________________________
**  associateFxs2Sip
**
**  descriptions: Assoicate the SIP with FXS CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
associateSip2Fxs(
	ccmConn_t* ccmConn_p,
	int phoneFeature
) {
	int connType = CCM_CONN_TYPE_FAIL;
	uint16 phonePhyId = 0;
	ccmConn_t *tmpCcmConn=NULL, *newCcmConn_p = NULL, *peerCcmConn_p=NULL, *tmpPeerCcmConn=NULL, *prevCcmConn=NULL, *prevPeerCcmConn=NULL;
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
	int bCallWaiting = TRUE; /*support one shot callwaiting on/off */
	phoneCh_t *phoneCB = NULL, *replacedPhoneCB = NULL;	
	
	CCM_FSM_DBG("associate SIP to FXS");

	if ( phoneFeature == CALL_FEATURE_TRANSFER_TARGET ) {
		if ( (prevCcmConn = findOrigCcmConn(SIP_PORT, ccmConn_p->dialInform.prevLogicId) ) != NULL ) {
			if ( (prevPeerCcmConn = prevCcmConn->peerCcmConn) != NULL ) {
				if ( prevPeerCcmConn->origPortType == DSP_FXS_PORT ) {
					phonePhyId = find_phonePhyId(prevPeerCcmConn->origPortId);
					if ( ( phonePhyId > 0 ) && ( checkLogicPhoneNum(phonePhyId-1) < N_PHONE_LOGIC_NUM ) ) {						
						phoneCB = find_phoneCB(phonePhyId-1, 1);
						if((NULL != phoneCB) && phoneCB->phStatus == PHONE_STATUS_DATA){
							CCM_PRINTF("TRANSFER_TARGET, attended-transfer\n");
							/*need to pick up new phone*/
							newCcmConn_p = createLogicPhoneSwap(phonePhyId-1, PHONE_STATUS_RING, PHONE_LINK_ST_INCOMING);
						}
						else if((NULL != phoneCB) && phoneCB->phStatus == PHONE_STATUS_RING){
							CCM_PRINTF("TRANSFER_TARGET, semi-transfer\n");
							/*need to alert new caller*/
							newCcmConn_p = createLogicPhoneSwap(phonePhyId-1, PHONE_STATUS_IDLE, PHONE_LINK_ST_IDLE);
						}
						else{
							CCM_PRINTF("error!! original phone in wrong state logicalId = %d, phStatus = %d", phoneCB->logicalId, phoneCB->phStatus);
						}
						
						replacedPhoneCB = find_phoneCB(newCcmConn_p->origPortId, 0);
						
						/*Eason, change physical phone to replaced phone, prevent flash ring when original phone remove*/
						if((NULL != phoneCB) && (NULL != replacedPhoneCB)){
							if((phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE)){
								replacedPhoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;
								replacedPhoneCB->phoneFlags |= PHONE_PHYSICAL_REVERSE;
								phoneCB->phoneFlags &= ~(PHONE_PHYSICAL_USAGE);								
								CCM_PRINTF("TRANSFER_TARGET oldphone logicalId = %d, newPhone logicalId = %d change to physical usage\n", phoneCB->logicalId, replacedPhoneCB->logicalId);
							}
						}						
						
					}
				}
			}
		}
		if ( newCcmConn_p != NULL ) {
			if ( newCcmConn_p->connSt == CALL_CONN_ST_IDLE ) {
				ccm_bundleConn(ccmConn_p, newCcmConn_p, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
				connType = CCM_CONN_TYPE_CALLEE_CALLEE;
				
				//zyMemcpy( &(ccmConn_p->dialInform), dialInform, sizeof(dialInfor_t) );
				zyMemcpy( &(newCcmConn_p->dialInform.dialDigitNumber), &(ccmConn_p->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
				newCcmConn_p->dialInform.prevLogicId = prevPeerCcmConn->origPortId;
				if(NULL != phoneCB){
					if(phoneCB->phStatus == PHONE_STATUS_RING){
						CCM_PRINTF("TRANSFER_TARGET, semi-transfer, alert new caller\n");
						eventVoiceCCMSimpleSend(EVT_CCM_DIAL_IND, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);
					}
					else if(phoneCB->phStatus == PHONE_STATUS_DATA){
						CCM_PRINTF("TRANSFER_TARGET, attended-transfer, offhook new call\n");
						if(NULL != replacedPhoneCB){
							enterLogicPhone(replacedPhoneCB, newCcmConn_p);
						}
						else{
							CCM_PRINTF("error!! no replaced phone found\n");
						}
					}
				}
				eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, prevPeerCcmConn->origFuncId, prevPeerCcmConn->origPortId);
				
			}
		} else {
			/* ccw mark: Need to process if the newCcmConn failed to created */
			
		}		
	}else if(phoneFeature == CALL_FEATURE_TRANSFER_TRANSFEREE) {
		if ( (prevCcmConn = findOrigCcmConn(SIP_PORT, ccmConn_p->dialInform.prevLogicId) ) != NULL ) {
			if ( (prevPeerCcmConn = prevCcmConn->peerCcmConn) != NULL ) {
				if ( prevPeerCcmConn->origPortType == DSP_FXS_PORT ) {
					phonePhyId = find_phonePhyId(prevPeerCcmConn->origPortId);
					if ( ( phonePhyId > 0 ) && ( checkLogicPhoneNum(phonePhyId-1) < N_PHONE_LOGIC_NUM ) ) {
						if ( (newCcmConn_p = createLogicPhoneSwap(phonePhyId-1, PHONE_STATUS_DIAL, PHONE_LINK_ST_CALLING) ) != NULL ) {
							if ( newCcmConn_p->connSt == CALL_CONN_ST_IDLE ) {
								ccm_bundleConn(newCcmConn_p, ccmConn_p, CALL_CONN_ST_CONNOUT, CCM_CONN_TYPE_CALLER_CALLER);
								connType = CCM_CONN_TYPE_CALLER_CALLER;
								//memcpy( &(newCcmConn_p->dialInform), &(ccmConn_p->dialInform), sizeof(dialInfor_t) );
								zyMemcpy( &(newCcmConn_p->dialInform.dialDigitNumber), &(ccmConn_p->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
								newCcmConn_p->dialInform.prevLogicId = prevPeerCcmConn->origPortId;
								createCallTransferCB(ccmConn_p, prevCcmConn, CALL_TRANSFEREE, CALL_TRANSFER_TYPE_ATTENDED);
								eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, prevPeerCcmConn->origFuncId, prevPeerCcmConn->origPortId);
								//eventVoiceCCMSimpleSend(EVT_CCM_DIAL_REQ, ORIG_FUNC_CCMCONN, newCcmConn_p->origPortId, newCcmConn_p->origPortType);
							}
						}
					}
				}
			}
		}
	}else {

		tmpCcmConn = ccmConnCb.fxsCcmConn;
		while ( tmpCcmConn != NULL ) {
			phonePhyId = find_phonePhyId(tmpCcmConn->origPortId);			
			if ( (phonePhyId != 0) && ( ccmConn_p->SipFxsMap & (1<<(phonePhyId-1)) ) ) {/* check SIP_FXS mapping */				
				if ( ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) && ( tmpCcmConn->peerCcmConn == NULL ) &&( !( tmpCcmConn->flags & CALL_CONN_FLAG_PREASSIGN ) ) ) {
					if ( ccmConn_p->peerCcmConn == NULL ) {
						peerCcmConn_p = tmpCcmConn;
						tmpPeerCcmConn = peerCcmConn_p;
						connType = CCM_CONN_TYPE_CALLEE_CALLEE;						
						ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
						#ifdef CENTRALIZE_LOG
						ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_INCALL_START);
						#endif
					} else {
						tmpPeerCcmConn = ccmConn_p->peerCcmConn;
						ccm_bundleForkConn(ccmConn_p, tmpCcmConn, tmpPeerCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
						#ifdef CENTRALIZE_LOG
						/* set webCallHistoryIndex of bundleCcm */
						if(NULL != tmpPeerCcmConn->bundleCcmList.bundleCcmNext){
							tmpPeerCcmConn->bundleCcmList.bundleCcmNext->webCallHistoryIndex = tmpPeerCcmConn->webCallHistoryIndex;
						}
						#endif
					}
				} else if ( ( tmpCcmConn->connSt == CALL_CONN_ST_CONNRDY ) || (tmpCcmConn->connSt == CALL_CONN_ST_CONNREL) ){
					if ( checkLogicPhoneNum(phonePhyId-1) < N_PHONE_LOGIC_NUM ) {
						tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
						if(tmpConvertEntry_p != NULL){
							int LineItr = 0;
							CCM_PRINTF("associateFxs2Sip > ENTER CALL_CONN_ST_CONNRDY, SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
							LineItr = mapSipAccountIdToLineItr(tmpConvertEntry_p->SipAccountId);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(tmpConvertEntry_p->VpItr, LineItr);
							/*check the call waiting feature */
							if(pLineFeature->callWaitingEnable){
								bCallWaiting = TRUE;
							}else{
								bCallWaiting = FALSE;
							}
							/* check the on shot call waiting */
							if(tmpCcmConn->dialInform.dialDigitNumber.dialDigitFlags & DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_ENABLE){
								bCallWaiting = TRUE;
							}else if(tmpCcmConn->dialInform.dialDigitNumber.dialDigitFlags & DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_DISABLE){
								bCallWaiting = FALSE;
							}
							
							if(bCallWaiting==TRUE){
								if ( (newCcmConn_p = createLogicPhoneSwap(phonePhyId-1, PHONE_STATUS_IDLE, PHONE_LINK_ST_IDLE) ) != NULL  ) {
									if ( ccmConn_p->peerCcmConn == NULL ) {
										peerCcmConn_p = newCcmConn_p;
										tmpPeerCcmConn = peerCcmConn_p;
										connType = CCM_CONN_TYPE_CALLEE_CALLEE;
										ccm_bundleConn(ccmConn_p, tmpPeerCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
										#ifdef CENTRALIZE_LOG
										ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_INCALL_START);
										#endif
									} else {
										tmpPeerCcmConn = ccmConn_p->peerCcmConn;
										ccm_bundleForkConn(ccmConn_p, newCcmConn_p, tmpPeerCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
									}
								}
							} else {
								CCM_FSM_DBG("Call Waiting disabled");								
							}
						}
					}
				}
			}
			tmpCcmConn = tmpCcmConn->next;
		}
	}

	return connType;
}
/*______________________________________________________________________________
**  associateSip2ZyPhone
**
**  descriptions: Assoicate the SIP with ZyPhone CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int associateSip2ZyPhone(ccmConn_t* ccmConn_p,int phoneFeature)
{
	int connType = CCM_CONN_TYPE_FAIL;
	int conn = connType;
	uint16 phonePhyId = 0;
	ccmConn_t *tmpCcmConn=NULL, *newCcmConn_p = NULL;
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
	CCM_FSM_DBG("associateSip2ZyPhone");
	/* Create the ZyPhone for */
	{
		tmpCcmConn = ccmConnCb.fxsCcmConn;
		while ( tmpCcmConn != NULL ) {
			phonePhyId = find_phonePhyId(tmpCcmConn->origPortId);
			if ( (phonePhyId != 0) && ( ccmConn_p->SipFxsMap & (1<<(phonePhyId-1)) ) ) {/* check SIP_FXS mapping */
				 if ( ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) || (tmpCcmConn->connSt == CALL_CONN_ST_CONNREL) ){
					if ( checkLogicPhoneNum(phonePhyId-1) < N_PHONE_LOGIC_NUM ) {
						tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
						if(tmpConvertEntry_p != NULL){
							int LineItr = 0;
							CCM_PRINTF("associateSip2ZyPhone>ENTER, SipAccountId =%d\n", tmpConvertEntry_p->SipAccountId);
							LineItr = mapSipAccountIdToLineItr(tmpConvertEntry_p->SipAccountId);
							CCM_PRINTF("associateSip2ZyPhone>ENTER, VpItr =%d\n", tmpConvertEntry_p->VpItr);
							CCM_PRINTF("associateSip2ZyPhone>ENTER, LineItr =%d\n", LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(tmpConvertEntry_p->VpItr, LineItr);
							if(pLineFeature->doNotDisturbEnable)
							{
								if ( (newCcmConn_p = create_ZyPhone(ccmConn_p, phoneFeature) ) != NULL ) {
									conn = CCM_CONN_TYPE_CALLEE_CALLEE;
									ccm_bundleConn(ccmConn_p, newCcmConn_p, CALL_CONN_ST_CONNIN, connType);									
					             }
							} else {
								CCM_FSM_DBG("Do not disturb disabled");
							}
						}
					}
				}
			}
			tmpCcmConn = tmpCcmConn->next;
		}
	}
	return conn;
}
/*______________________________________________________________________________
**  associateFxo2Fxs
**
**  descriptions: Assoicate the FXO with FXS CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
associateFxo2Fxs(
	ccmConn_t* ccmConn_p,
	int phoneFeature
) {
	uint16 phonePhyId = 0;
	int connType = CCM_CONN_TYPE_FAIL;
	ccmConn_t *tmpCcmConn=NULL, *newCcmConn_p = NULL, *peerCcmConn_p=NULL, **tmpPeerCcmConn=NULL;

	CCM_FSM_DBG("associate Fxo 2 Fxs");
	tmpCcmConn = ccmConnCb.fxsCcmConn;
	while ( tmpCcmConn != NULL ) {
		phonePhyId = find_phonePhyId(tmpCcmConn->origPortId);

		if ( ccmConn_p->SipFxsMap & (1<<(phonePhyId-1)) ) {
			if ( ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) && ( tmpCcmConn->peerCcmConn == NULL )
				&&( !( tmpCcmConn->flags & CALL_CONN_FLAG_PREASSIGN ) ) ) {
				if ( peerCcmConn_p == NULL ) {
					peerCcmConn_p = tmpCcmConn;
					tmpPeerCcmConn = &peerCcmConn_p;
					connType = CCM_CONN_TYPE_CALLEE_CALLEE;
					ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
					#ifdef CENTRALIZE_LOG//dennis_add
					ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_INCALL_START);
					#endif	
				} else {
					ccm_bundleForkConn(ccmConn_p, tmpCcmConn, tmpPeerCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
				}
			} 		
		}
		tmpCcmConn = tmpCcmConn->next;
	}
CCM_FSM_DBG1("associate Fxo 2 Fxs return type is ",connType);
	return connType;
}

/*______________________________________________________________________________
**  associateFxoLife2Fxs
**
**  descriptions: Assoicate the FXO-LifeLine with FXS CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
associateFxoLife2Fxs(
	ccmConn_t* ccmConn_p,
	int phoneFeature
) {
	uint16 phonePhyId = 0;
	int connType = CCM_CONN_TYPE_FAIL;
	ccmConn_t *tmpCcmConn=NULL, *newCcmConn_p = NULL, *peerCcmConn_p=NULL, *tmpPeerCcmConn=NULL;

	tmpCcmConn = ccmConnCb.fxsCcmConn;
	while ( tmpCcmConn != NULL ) {
		phonePhyId = find_phonePhyId(tmpCcmConn->origPortId);
		if ( ccmConn_p->SipFxsMap & (1<<(phonePhyId-1)) ) {
			if ( ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) && ( tmpCcmConn->peerCcmConn == NULL ) 
				&&( !( tmpCcmConn->flags & CALL_CONN_FLAG_PREASSIGN ) ) ) {
				if ( peerCcmConn_p == NULL ) {
					peerCcmConn_p = tmpCcmConn;
					tmpPeerCcmConn = peerCcmConn_p;
					connType = CCM_CONN_TYPE_CALLEE_CALLEE;
					ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
				} else {
					ccm_bundleForkConn(ccmConn_p, tmpCcmConn, tmpPeerCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
				}
			} else if ( (( tmpCcmConn->connSt == CALL_CONN_ST_CONNRDY ) || (tmpCcmConn->connSt == CALL_CONN_ST_CONNREL) ) && ( phonePhyId != 0 ) ) {
				if ( (newCcmConn_p = createLogicPhoneSwap(phonePhyId-1, PHONE_STATUS_IDLE, PHONE_LINK_ST_IDLE) ) != NULL  ) {
					if ( peerCcmConn_p == NULL ) {
						peerCcmConn_p = newCcmConn_p;
						tmpPeerCcmConn = peerCcmConn_p;
						connType = CCM_CONN_TYPE_CALLEE_CALLEE;
						ccm_bundleConn(ccmConn_p, newCcmConn_p, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
					} else {
						ccm_bundleForkConn(ccmConn_p, newCcmConn_p, tmpPeerCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
					}
				}
			}
		}
		tmpCcmConn = tmpCcmConn->next;
	}

	return connType;
}

/*______________________________________________________________________________
**  associateFxs2Fxs
**
**  descriptions: Assoicate the FXS with FXS CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
associateFxs2Fxs(
	ccmConn_t* ccmConn_p,
	int phoneFeature
) {
	int connType = CCM_CONN_TYPE_FAIL, interCallFeatureLen = 0;
	uint16 phonePhyId1 = 0, phonePhyId2 = 0;
	ccmConn_t *tmpCcmConn=NULL, *newCcmConn_p = NULL, *peerCcmConn_p=NULL, **tmpPeerCcmConn=NULL;

	CCM_FSM_DBG("associate FXS 2 FXS");
	/* Get the start FXS-FXS physical-ID. To avoid the link at the same physical phone */
	phonePhyId1 = find_phonePhyId(ccmConn_p->origPortId);

	tmpCcmConn = ccmConnCb.fxsCcmConn;

	interCallFeatureLen = getCallFeaturekeyLengthByFeatureId(0,CALL_FEATURE_INTERNAL_CALL);
	while ( tmpCcmConn != NULL ) {
		phonePhyId2 = find_phonePhyId(tmpCcmConn->origPortId);
		if ( phonePhyId1 != phonePhyId2 ) {
			if ( ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) && ( tmpCcmConn->peerCcmConn == NULL ) &&( !( tmpCcmConn->flags & CALL_CONN_FLAG_PREASSIGN ) ) ) {
				if ( ccmConn_p->peerCcmConn == NULL ) {
					peerCcmConn_p = tmpCcmConn;
					tmpPeerCcmConn = &peerCcmConn_p;
					connType = CCM_CONN_TYPE_CALLER_CALLEE;
					ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_INTERACTIVE, CCM_CONN_TYPE_CALLER_CALLEE);
					peerCcmConn_p->dialInform.dialDigitNumber.dial_digits[interCallFeatureLen] = phonePhyId1;
					#ifdef CENTRALIZE_LOG
					ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_INCALL_START);
					#endif
				} else {
					tmpPeerCcmConn = ccmConn_p->peerCcmConn;
					ccm_bundleForkConn(ccmConn_p, tmpCcmConn, tmpPeerCcmConn, CALL_CONN_ST_INTERACTIVE, CCM_CONN_TYPE_CALLER_CALLEE);
					tmpCcmConn->dialInform.dialDigitNumber.dial_digits[interCallFeatureLen] = phonePhyId1;
				}
			} else if ( ( tmpCcmConn->connSt == CALL_CONN_ST_CONNRDY ) || (tmpCcmConn->connSt == CALL_CONN_ST_CONNREL) ){
				if ( checkLogicPhoneNum(phonePhyId2-1) < N_PHONE_LOGIC_NUM ) {
					if ( (newCcmConn_p = createLogicPhoneSwap(phonePhyId2-1, PHONE_STATUS_IDLE, PHONE_LINK_ST_IDLE) ) != NULL  ) {
						if ( ccmConn_p->peerCcmConn == NULL ) {
							peerCcmConn_p = newCcmConn_p;
							tmpPeerCcmConn = &peerCcmConn_p;
							connType = CCM_CONN_TYPE_CALLER_CALLEE;
							ccm_bundleConn(ccmConn_p, *tmpPeerCcmConn, CALL_CONN_ST_INTERACTIVE, CCM_CONN_TYPE_CALLER_CALLEE);
						} else {
							tmpPeerCcmConn = ccmConn_p->peerCcmConn;
							ccm_bundleForkConn(ccmConn_p, newCcmConn_p, tmpPeerCcmConn, CALL_CONN_ST_INTERACTIVE, CCM_CONN_TYPE_CALLER_CALLEE);
						}
					}
				}
			}
		}
		tmpCcmConn = tmpCcmConn->next;
	}

	return connType;
}

/*______________________________________________________________________________
**  associateFxs2Fxo
**
**  descriptions: Assoicate the FXS with FXO CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
associateFxs2Fxo(
	ccmConn_t* ccmConn_p,
	int phoneFeature
) {
	int connType = CCM_CONN_TYPE_FAIL;
	ccmConn_t *tmpCcmConn=NULL;
	uint16 phonePhyId = find_phonePhyId(ccmConn_p->origPortId);
	
	tmpCcmConn = ccmConnCb.fxoCcmConn;
	while ( tmpCcmConn != NULL ) {
		CCM_FSM_DBG("associate Fxs2Fxo");
		if ( (tmpCcmConn->connSt == CALL_CONN_ST_IDLE)  ) {
			ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_CONNOUT, CCM_CONN_TYPE_CALLER_CALLER);
			connType = CCM_CONN_TYPE_CALLER_CALLER;
			#ifdef CENTRALIZE_LOG
			ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_START);
			#endif
			ccmConn_p->flags |= CALL_CONN_FLAG_PREFIX;
			tmpCcmConn->flags |= CALL_CONN_FLAG_PREFIX;
			break;
		}
		tmpCcmConn = tmpCcmConn->next;
	}
	return connType;
}

/*______________________________________________________________________________
**  associateFxs2ZyPhone
**
**  descriptions: Assoicate the FXS with FXS CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
associateFxs2ZyPhone(
	ccmConn_t* ccmConn_p,
	int phoneFeature
) {
	int connType = CCM_CONN_TYPE_FAIL;
	ccmConn_t *newCcmConn_p = NULL;

	CCM_FSM_DBG("associate FXS 2 ZyPhone");	
	/* Create the ZyPhone for IVR */
	if ( (newCcmConn_p = create_ZyPhone(ccmConn_p, phoneFeature) ) != NULL ) {
		connType = CCM_CONN_TYPE_CALLER_CALLEE;
		ccm_bundleConn(ccmConn_p, newCcmConn_p, CALL_CONN_ST_INTERACTIVE, CCM_CONN_TYPE_CALLER_CALLEE);
	}		
	return connType;
}

/*______________________________________________________________________________
**  associateZyPhone2FXS
**
**  descriptions: Assoicate the ZyPhone with FXS CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int
associateZyPhone2Fxs(
	ccmConn_t* ccmConn_p,
	int phoneFeature

) 
{
	int connType = CCM_CONN_TYPE_FAIL;
	uint16 phonePhyId = 0;

	uint8 phFeatureLen = 0, fxsTranslate=0;
	ccmConn_t *tmpCcmConn=NULL, *newCcmConn_p = NULL, *peerCcmConn_p=NULL, **tmpPeerCcmConn=NULL;
	VoiceProfLineCallingFeaturesObj_t* lineFeatureObj = NULL;	
	phoneCh_t * FXSPhoneCB = NULL;

	CCM_FSM_DBG("associate ZyPhone 2 FXS");


	tmpCcmConn = ccmConnCb.fxsCcmConn;
	while ( tmpCcmConn != NULL ) {
		phonePhyId = find_phonePhyId(tmpCcmConn->origPortId);
 		CCM_FSM_DBG2("associateZyPhone2Fxs: tmpCcmConn=", tmpCcmConn->origPortId,", SipFxsMap=",ccmConn_p->SipFxsMap);

		if ( (phonePhyId != 0) && ( ccmConn_p->SipFxsMap & (1<<(phonePhyId-1)) ) ) {/* check SIP_FXS mapping */

			if ( ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) && ( tmpCcmConn->peerCcmConn == NULL ) &&( !( tmpCcmConn->flags & CALL_CONN_FLAG_PREASSIGN ) ) ) {
				if ( peerCcmConn_p == NULL ) {
					peerCcmConn_p = tmpCcmConn;
					tmpPeerCcmConn = &peerCcmConn_p;
					 if( phoneFeature==CALL_FEATURE_INTERNAL_CALL){
						connType = CCM_CONN_TYPE_CALLER_CALLEE;
						ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_INTERACTIVE, CCM_CONN_TYPE_CALLER_CALLEE);
					}
					else{
						connType = CCM_CONN_TYPE_CALLEE_CALLEE;
						ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
					}				
					#ifdef CENTRALIZE_LOG
					ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_INCALL_START);
					#endif
				} else {
					ccm_bundleForkConn(ccmConn_p, tmpCcmConn, tmpPeerCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
				}
			} else if ( ( tmpCcmConn->connSt == CALL_CONN_ST_CONNRDY ) || (tmpCcmConn->connSt == CALL_CONN_ST_CONNREL) ){
				FXSPhoneCB = find_phoneCB(phonePhyId , 1);
				lineFeatureObj = getAssociatedLineFeatures(FXSPhoneCB);

				if ( checkLogicPhoneNum(phonePhyId-1) < N_PHONE_LOGIC_NUM ) {
					if( (NULL != lineFeatureObj) && (lineFeatureObj->callWaitingEnable) ){
						if ( (newCcmConn_p = createLogicPhoneSwap(phonePhyId-1, PHONE_STATUS_IDLE, PHONE_LINK_ST_IDLE) ) != NULL  ) {
							if ( peerCcmConn_p == NULL ) {
								peerCcmConn_p = newCcmConn_p;
								tmpPeerCcmConn = &peerCcmConn_p;
								if ( phoneFeature==CALL_FEATURE_INTERNAL_CALL){
									connType = CCM_CONN_TYPE_CALLER_CALLEE;
									ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_INTERACTIVE, CCM_CONN_TYPE_CALLER_CALLEE);
								}
								else{
									connType = CCM_CONN_TYPE_CALLEE_CALLEE;
									ccm_bundleConn(ccmConn_p, *tmpPeerCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
								}
							} else {
								ccm_bundleForkConn(ccmConn_p, newCcmConn_p, tmpPeerCcmConn, CALL_CONN_ST_CONNIN, CCM_CONN_TYPE_CALLEE_CALLEE);
							}
						}
					}
					else{
						CCM_FSM_DBG("Call Waiting disabled\r\n");
					}
				}
			}

		}

		Exit:
		tmpCcmConn = tmpCcmConn->next;
	}

	return connType;
}


/*______________________________________________________________________________
**  associateZyPhone2Sip
**
**  descriptions: Assoicate the ZyPhone with SIP CCM.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
associateZyPhone2Sip(
	ccmConn_t* ccmConn_p,
	int phoneFeature
) 
{
	int connType = CCM_CONN_TYPE_FAIL;
	int sipLineId=0, firstAvailSipLineId = 0;
	
	ccmConn_t *tmpCcmConn=NULL, *newCcmConn_p = NULL;
	sipCCMConvertEntry_t *tmpConvertEntry_p=NULL, *firstMatchConvertEntry_p=NULL;

	CCM_FSM_DBG("associate ZyPhone 2 Sip");
	tmpCcmConn = ccmConnCb.sipCcmConn;
	while ( tmpCcmConn != NULL ) {
		tmpConvertEntry_p = findSipCCMConvertByPortId(tmpCcmConn->origPortId);
		if ( tmpConvertEntry_p != NULL ) {
			CCM_PRINTF("associateZyPhone2Sip > ENTER , SipAccountId = %d\n" , tmpConvertEntry_p->SipAccountId);
			sipLineId = tmpConvertEntry_p->SipAccountId;
			CCM_FSM_DBG1("associateZyPhone2Sip, sipLineId=", sipLineId);
		
			if ( ( ccmConn_p->SipFxsMap & (1<<sipLineId) ) && ( tmpCcmConn->flags & CALL_CONN_FLAG_REGISTER )) {
				if(-1 == firstAvailSipLineId){
					firstAvailSipLineId = sipLineId;
				}	

				CCM_FSM_DBG2("associateZyPhone2Sip,  SipFxsMap=",ccmConn_p->SipFxsMap,", sipLineId=",sipLineId);
				if ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) {
					ccm_bundleConn(ccmConn_p, tmpCcmConn, CALL_CONN_ST_CONNOUT, CCM_CONN_TYPE_CALLER_CALLER);
					connType = CCM_CONN_TYPE_CALLER_CALLER;
					#ifdef CENTRALIZE_LOG
					ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_START);
					#endif
					break;
				}
			}
		}
		tmpCcmConn = tmpCcmConn->next;
	}
#ifdef MULTI_SIP_SESSION
	{
		if ( connType == CCM_CONN_TYPE_FAIL ) {
			if ( (-1!=firstAvailSipLineId) && (newCcmConn_p = addSipCCMConvert(SIP_CCM_CONVERT_ST_IDLE, SIP_CCM_TABLE_TEMPORARY, firstAvailSipLineId, 0) ) != NULL ) {				
				if ( newCcmConn_p->connSt == CALL_CONN_ST_IDLE ) {
					connType = CCM_CONN_TYPE_CALLER_CALLER;
					ccm_bundleConn(ccmConn_p, newCcmConn_p, CALL_CONN_ST_CONNOUT, CCM_CONN_TYPE_CALLER_CALLER);
					#ifdef CENTRALIZE_LOG
					ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_OUTCALL_START);
					#endif
				}
				
			}			
		}
	}
#endif

	return connType;
}

/*______________________________________________________________________________
**  ccm_bundleConn
**
**  descriptions: Bundle two CCM connection.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
ccm_bundleConn(
	ccmConn_t* ccmConn_p,
	ccmConn_t* peerCcmConn_p,
	uint16 connStatus,
	uint8 connType
){
	phoneCh_t *tmpPhoneCB = NULL;
	phoneCh_t *tmpPeerPhoneCB = NULL;
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
	VoiceProfLineVoiceProcessingObj_t *pVoiceProObj = NULL;

	/* Set the connSt and connType on both CCM */
	ccmConn_p->connSt = peerCcmConn_p->connSt = connStatus;
	connType = ccmConn_p->connType = peerCcmConn_p->connType = connType;

	/* Set the ccmConn_p */
	ccmConn_p->connRole = CCM_CONN_ROLE_ORIGINAL;
	ccmConn_p->flags &= ~CALL_CONN_FLAG_PREASSIGN;
	ccmConn_p->flags |= CALL_CONN_FLAG_ASSIGN;
	ccmConn_p->peerCcmConn = peerCcmConn_p;
	if ( ( ccmConn_p->bundleCcmList.bundleCcmNext != NULL ) || ( ccmConn_p->bundleCcmList.bundleCcmPrev != NULL ) ) {
		CCM_FSM_DBG("ccm_bundle() warning - The original ccmConn_p->bundleCcmList is not NULL");
	}
	ccmConn_p->bundleCcmList.bundleCcmNext = NULL;
	ccmConn_p->bundleCcmList.bundleCcmPrev = NULL;

	/* Set the peerCcmConn_p */
	peerCcmConn_p->connRole = CCM_CONN_ROLE_PEER;
	peerCcmConn_p->flags &= ~CALL_CONN_FLAG_PREASSIGN;
	peerCcmConn_p->flags |= CALL_CONN_FLAG_ASSIGN;
	peerCcmConn_p->peerCcmConn = ccmConn_p;
	if ( ( peerCcmConn_p->bundleCcmList.bundleCcmNext != NULL ) || ( peerCcmConn_p->bundleCcmList.bundleCcmPrev != NULL ) ) {
		CCM_FSM_DBG("ccm_bundle() warning - The original peerCcmConn_p->bundleCcmList is not NULL");
	}
	peerCcmConn_p->bundleCcmList.bundleCcmNext = NULL;
	peerCcmConn_p->bundleCcmList.bundleCcmPrev = NULL;
	if ( connType == CALL_CONN_ST_CONNOUT ) {
		zyMemcpy(&(peerCcmConn_p->dialInform.dialDigitNumber), &(ccmConn_p->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
	}

	/* Set the callId & associatedProfileId & associatedSipAccountId to the peer */
	if ( (ccmConn_p->origPortType != SIP_PORT) && (peerCcmConn_p->origPortType == SIP_PORT) ) {
		tmpPhoneCB = find_phoneCB(ccmConn_p->origPortId, 0);
		tmpConvertEntry_p = findSipCCMConvertByPortId(peerCcmConn_p->origPortId);
		if ( (tmpPhoneCB != NULL) && (tmpConvertEntry_p != NULL) ) {
			if (tmpConvertEntry_p->callId == 0) {
				tmpConvertEntry_p->callId = tmpPhoneCB->callId;
			} else {
				tmpPhoneCB->callId = tmpConvertEntry_p->callId;
			}
			tmpPhoneCB->associatedVpItr= tmpConvertEntry_p->VpItr;
			tmpPhoneCB->associatedSipAccountId= tmpConvertEntry_p->SipAccountId;
			int LineItr = 0;
			CCM_PRINTF("ccm_bundleConn>ENTER, SipAccountId = %d\n", tmpPhoneCB->associatedSipAccountId);
			CCM_PRINTF("ccm_bundleConn>ENTER, associatedVpItr = %d\n", tmpPhoneCB->associatedVpItr);
			CCM_PRINTF("ccm_bundleConn>ENTER, LineItr = %d\n", LineItr);
			LineItr = mapSipAccountIdToLineItr(tmpPhoneCB->associatedSipAccountId);
			pVoiceProObj = tr104VoiceLineProcessingObjPtrGet(tmpPhoneCB->associatedVpItr, LineItr);
			if(NULL != pVoiceProObj){
			   #if 0 /* __MSTC__, Klose,merge from P876 feature, Set egress/ingress gain to control the Speaking/Listening volume, 20121107 */
				phSetRxGain(tmpPhoneCB,pVoiceProObj->receiveGain); /*receiveGain in 0.1db unit*/
				phSetTxGain(tmpPhoneCB,pVoiceProObj->transmitGain);/*transmitGain in 0.1db unit*/
				#endif
			}
		} else {
			CCM_FSM_DBG("ccm_bundle() ERROR - tmpPhoneCB or tmpConvertEntry_p is NULL");
		}
	}else if ( (ccmConn_p->origPortType == SIP_PORT) && (peerCcmConn_p->origPortType != SIP_PORT) && (peerCcmConn_p->origPortType != ZYXEL_PHONE_PORT) ) {
		tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
		tmpPeerPhoneCB = find_phoneCB(peerCcmConn_p->origPortId, 0);
		if ( (tmpPeerPhoneCB != NULL) && (tmpConvertEntry_p != NULL) ) {
			tmpPeerPhoneCB->callId = tmpConvertEntry_p->callId;
			tmpPeerPhoneCB->associatedVpItr= tmpConvertEntry_p->VpItr;
			tmpPeerPhoneCB->associatedSipAccountId= tmpConvertEntry_p->SipAccountId;
			int LineItr = 0;
			CCM_PRINTF("ccm_bundleConn>ENTER, SipAccountId = %d\n", tmpPeerPhoneCB->associatedSipAccountId);
			CCM_PRINTF("associateSip2ZyPhone>ENTER, associatedVpItr = %d\n", tmpPeerPhoneCB->associatedVpItr);
			CCM_PRINTF("associateSip2ZyPhone>ENTER, LineItr = %d\n", LineItr);
			LineItr = mapSipAccountIdToLineItr(tmpPeerPhoneCB->associatedSipAccountId);
			pVoiceProObj = tr104VoiceLineProcessingObjPtrGet(tmpPeerPhoneCB->associatedVpItr, LineItr);
			if(NULL != pVoiceProObj){
			   #if 0 /* __MSTC__, Klose,merge from P876 feature, Set egress/ingress gain to control the Speaking/Listening volume, 20121107 */
				phSetRxGain(tmpPeerPhoneCB,pVoiceProObj->receiveGain);
				phSetTxGain(tmpPeerPhoneCB,pVoiceProObj->transmitGain);
				#endif
			}
		} else {
			CCM_FSM_DBG("ccm_bundle() ERROR - tmpPeerPhoneCB or tmpConvertEntry_p is NULL");
		}
	}else if ( (ccmConn_p->origPortType != SIP_PORT) && (peerCcmConn_p->origPortType != SIP_PORT) ) {
		tmpPhoneCB = find_phoneCB(ccmConn_p->origPortId, 0);
		tmpPeerPhoneCB = find_phoneCB(peerCcmConn_p->origPortId, 0);
		if ( (tmpPhoneCB != NULL) && (tmpPeerPhoneCB != NULL) ) {
			if(ccmConn_p->origPortType == DSP_FXO_PORT){
				tmpPhoneCB->callId = tmpPhoneCB->logicalId;				
			}
			CCM_PRINTF("copy dial digits:%s\n", ccmConn_p->dialInform.dialDigitNumber.dial_digits);
			zyMemcpy(&(peerCcmConn_p->dialInform.dialDigitNumber), &(ccmConn_p->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
			zyMemcpy(&(tmpPeerPhoneCB->dialInform.dialDigitNumber), &(ccmConn_p->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
			tmpPeerPhoneCB->callId = tmpPhoneCB->callId;			
		} else {
			CCM_FSM_DBG("ccm_bundle() ERROR - tmpPhoneCB or tmpPeerPhoneCB is NULL");
		}
	}
	
	CCM_FSM_DBG1("ccm_bundle, peerCcmConn=", (uint32)peerCcmConn_p);

	return 0;
}

/*______________________________________________________________________________
**  ccm_bundleForkConn
**
**  descriptions: Bundle other CCMs into the peerCCM link list.
**                                FXS               SIP
**                                peer------->
**                                      <-------peer
**                                  |
**                              bundle1
**                                peer ------>
**                                  |
**                              bundle2
**                                 peer------->
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
ccm_bundleForkConn(
	ccmConn_t* ccmConn_p,
	ccmConn_t* peerCcmConn_p,
	ccmConn_t* prePeerCcmConn_p,
	uint16 connStatus, 
	uint8 connType
){
	phoneCh_t *tmpPhoneCB = NULL;
	phoneCh_t *tmpPeerPhoneCB = NULL;
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;

	/* Check the link list */
	if ( ccmDebug != 0 ) {
		if ( peerCcmConn_p->peerCcmConn != NULL ) {
			dbg_pline_1("Warning: ccm_bundleForkConn(), peerCcmConn_p->peerCcmConn != NULL\n\r");
		}
		if ( peerCcmConn_p->bundleCcmList.bundleCcmPrev != NULL ) {
			dbg_pline_1("Warning: ccm_bundleForkConn(), peerCcmConn_p->bundleCcmPrev != NULL\n\r");
		}
		if ( prePeerCcmConn_p->bundleCcmList.bundleCcmNext != NULL ) {
			dbg_pline_1("Warning: ccm_bundleForkConn(), prePeerCcmConn_p->bundleCcmNext != NULL\n\r");
		}
	}

	/* Bundle a peerCCM link list for ccmConn_p to allow the folking */
	peerCcmConn_p->connType = connType;
	peerCcmConn_p->connSt = connStatus;
	peerCcmConn_p->connRole = CCM_CONN_ROLE_PEER;
	peerCcmConn_p->flags &= ~CALL_CONN_FLAG_PREASSIGN;
	peerCcmConn_p->flags |= CALL_CONN_FLAG_ASSIGN;
	peerCcmConn_p->peerCcmConn = ccmConn_p;
	peerCcmConn_p->bundleCcmList.bundleCcmPrev = prePeerCcmConn_p;
	prePeerCcmConn_p->bundleCcmList.bundleCcmNext = peerCcmConn_p;
	prePeerCcmConn_p = peerCcmConn_p;

	/* Set the callId & associatedProfileId & associatedSipAccountId to the peer */
	if ( (ccmConn_p->origPortType != SIP_PORT) && (peerCcmConn_p->origPortType == SIP_PORT) ) {
		tmpPhoneCB = find_phoneCB(ccmConn_p->origPortId, 0);
		tmpConvertEntry_p = findSipCCMConvertByPortId(peerCcmConn_p->origPortId);
		if ( (tmpPhoneCB != NULL) && (tmpConvertEntry_p != NULL) ) {
			if (tmpConvertEntry_p->callId == 0) {
				tmpConvertEntry_p->callId = tmpPhoneCB->callId;
			} else {
				tmpPhoneCB->callId = tmpConvertEntry_p->callId;
			}
			CCM_PRINTF("ccm_bundleForkConn>ENTER, VpItr = %d\n", tmpConvertEntry_p->VpItr);
			CCM_PRINTF("ccm_bundleForkConn>ENTER, SipAccountId = %d\n", tmpConvertEntry_p->SipAccountId);
			tmpPhoneCB->associatedVpItr= tmpConvertEntry_p->VpItr;
			tmpPhoneCB->associatedSipAccountId= tmpConvertEntry_p->SipAccountId;
		} else {
			CCM_FSM_DBG("ccm_bundle() ERROR - tmpPhoneCB or tmpConvertEntry_p is NULL");
		}
	}else if ( (ccmConn_p->origPortType == SIP_PORT) && (peerCcmConn_p->origPortType != SIP_PORT) ) {
		tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn_p->origPortId);
		tmpPeerPhoneCB = find_phoneCB(peerCcmConn_p->origPortId, 0);
		if ( (tmpPeerPhoneCB != NULL) && (tmpConvertEntry_p != NULL) ) {
			tmpPeerPhoneCB->callId = tmpConvertEntry_p->callId;
			CCM_PRINTF("ccm_bundleForkConn>ENTER, VpItr = %d\n", tmpConvertEntry_p->VpItr);
			CCM_PRINTF("ccm_bundleForkConn>ENTER, SipAccountId = %d\n", tmpConvertEntry_p->SipAccountId);
			tmpPeerPhoneCB->associatedVpItr= tmpConvertEntry_p->VpItr;
			tmpPeerPhoneCB->associatedSipAccountId= tmpConvertEntry_p->SipAccountId;
		} else {
			CCM_FSM_DBG("ccm_bundle() ERROR - tmpPeerPhoneCB or tmpConvertEntry_p is NULL");
		}
	}else if ( (ccmConn_p->origPortType != SIP_PORT) && (peerCcmConn_p->origPortType != SIP_PORT) ) {
		tmpPhoneCB = find_phoneCB(ccmConn_p->origPortId, 0);
		tmpPeerPhoneCB = find_phoneCB(peerCcmConn_p->origPortId, 0);
		if ( (tmpPhoneCB != NULL) && (tmpPeerPhoneCB != NULL) ) {
			tmpPeerPhoneCB->callId = tmpPhoneCB->callId;
		} else {
			CCM_FSM_DBG("ccm_bundle() ERROR - tmpPhoneCB or tmpPeerPhoneCB is NULL");
		}
	}
	
	CCM_FSM_DBG1("ccm_bundleForkConn(), forkCcmConn=", (uint32)prePeerCcmConn_p);

	return 0;
}

/*______________________________________________________________________________
**  ccm_releaseConn
**
**  descriptions: if ccmConn_p has bundleCcmList, change the peerCcmConn_p to the bundleCcmList.
**                     if not, remove all the peerCcmList.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
ccm_releaseConn(
	ccmConn_t* ccmConn_p
){
	ccmConn_t *tmpPeerHeadCcmConn_p = NULL, *prevTmpCcmConn_p = NULL;;
	#ifdef CENTRALIZE_LOG
	//ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_END_CALL);	/* move to the latter, after the bundleCcmList condition */
	#endif

	/* There are other bundleCcmList existed on the ccmConn_p, change the peerCcmConn_p->peerCcmConn_p to this */
	if ( ccmConn_p->bundleCcmList.bundleCcmPrev != NULL ) {
		CCM_FSM_DBG("CCM: ccm_releaseConn() -1");
		ccmConn_p->bundleCcmList.bundleCcmPrev->bundleCcmList.bundleCcmNext = ccmConn_p->bundleCcmList.bundleCcmNext;
		if ( ccmConn_p->bundleCcmList.bundleCcmNext != NULL ) {
			ccmConn_p->bundleCcmList.bundleCcmNext->bundleCcmList.bundleCcmPrev = ccmConn_p->bundleCcmList.bundleCcmPrev;
		}
		ccmConn_p->flags &= ~(CALL_CONN_FLAG_ASSIGN|CALL_CONN_FLAG_PREFIX|CALL_CONN_FLAG_ALERT|CALL_CONN_FLAG_PEERLOCAL|CALL_CONN_FLAG_HOLD);
		ccmConn_p->connType = CCM_CONN_TYPE_FAIL;
		zyMemset( &(ccmConn_p->dialInform), 0, sizeof(dialInfor_t) );
		ccmConn_p->peerCcmConn = NULL;
		ccmConn_p->bundleCcmList.bundleCcmPrev = NULL;
		ccmConn_p->bundleCcmList.bundleCcmNext = NULL;
		return 0;
	}

	if ( ccmConn_p->bundleCcmList.bundleCcmNext != NULL ) {
		CCM_FSM_DBG("CCM: ccm_releaseConn() -2");
		ccmConn_p->bundleCcmList.bundleCcmNext->bundleCcmList.bundleCcmPrev = ccmConn_p->bundleCcmList.bundleCcmPrev;
		if ( ccmConn_p->bundleCcmList.bundleCcmPrev != NULL ) {
			ccmConn_p->bundleCcmList.bundleCcmPrev->bundleCcmList.bundleCcmNext = ccmConn_p->bundleCcmList.bundleCcmNext;
		} else {
			ccmConn_p->peerCcmConn->peerCcmConn = ccmConn_p->bundleCcmList.bundleCcmNext;
		}
#ifdef VISUAL_MWI
		ccmConn_p->flags &= ~(CALL_CONN_FLAG_VMWI);
#endif		
		ccmConn_p->flags &= ~(CALL_CONN_FLAG_ASSIGN|CALL_CONN_FLAG_PREFIX|CALL_CONN_FLAG_ALERT|CALL_CONN_FLAG_PEERLOCAL|CALL_CONN_FLAG_HOLD);
		ccmConn_p->connType = CCM_CONN_TYPE_FAIL;
		zyMemset( &(ccmConn_p->dialInform), 0, sizeof(dialInfor_t) );
		ccmConn_p->peerCcmConn = NULL;
		ccmConn_p->bundleCcmList.bundleCcmPrev = NULL;
		ccmConn_p->bundleCcmList.bundleCcmNext = NULL;
		return 0;
	}
	
	#ifdef CENTRALIZE_LOG
	ccmCentralizeLog(ccmConn_p, CCM_LOGS_TYPE_END_CALL);	/* move from the former */
	#endif

	CCM_FSM_DBG("CCM: ccm_releaseConn() -3");
	tmpPeerHeadCcmConn_p = ccmConn_p->peerCcmConn;
	/* If ccmConn_p->peerCcmConn is NULL, remove the ccmConn_p from the link list. And reset it */
	while ( tmpPeerHeadCcmConn_p != NULL ) {
		prevTmpCcmConn_p = tmpPeerHeadCcmConn_p->bundleCcmList.bundleCcmNext;
		if ( tmpPeerHeadCcmConn_p->bundleCcmList.bundleCcmPrev != NULL ) {
			tmpPeerHeadCcmConn_p->bundleCcmList.bundleCcmPrev->bundleCcmList.bundleCcmNext = tmpPeerHeadCcmConn_p->bundleCcmList.bundleCcmNext;
		}
		if ( tmpPeerHeadCcmConn_p->bundleCcmList.bundleCcmNext != NULL ) {
			tmpPeerHeadCcmConn_p->bundleCcmList.bundleCcmNext->bundleCcmList.bundleCcmPrev = tmpPeerHeadCcmConn_p->bundleCcmList.bundleCcmPrev;
		}
#ifdef VISUAL_MWI
		tmpPeerHeadCcmConn_p->flags &= ~(CALL_CONN_FLAG_VMWI);
#endif		
		tmpPeerHeadCcmConn_p->flags &= ~(CALL_CONN_FLAG_ASSIGN|CALL_CONN_FLAG_PREFIX|CALL_CONN_FLAG_ALERT|CALL_CONN_FLAG_PEERLOCAL|CALL_CONN_FLAG_HOLD);
		tmpPeerHeadCcmConn_p->connType = CCM_CONN_TYPE_FAIL;
		zyMemset( &(tmpPeerHeadCcmConn_p->dialInform), 0, sizeof(dialInfor_t) );
		tmpPeerHeadCcmConn_p->peerCcmConn = NULL;
		tmpPeerHeadCcmConn_p->bundleCcmList.bundleCcmNext =  NULL;
		tmpPeerHeadCcmConn_p->bundleCcmList.bundleCcmPrev = NULL;
		tmpPeerHeadCcmConn_p = prevTmpCcmConn_p;
	}

	ccmConn_p->flags &= ~(CALL_CONN_FLAG_ASSIGN|CALL_CONN_FLAG_PREFIX|CALL_CONN_FLAG_ALERT|CALL_CONN_FLAG_PEERLOCAL|CALL_CONN_FLAG_HOLD);
#ifdef VISUAL_MWI
	ccmConn_p->flags &= ~(CALL_CONN_FLAG_VMWI);
#endif	
	ccmConn_p->connType = CCM_CONN_TYPE_FAIL;
	zyMemset( &(ccmConn_p->dialInform), 0, sizeof(dialInfor_t) );
	ccmConn_p->peerCcmConn = NULL;
	ccmConn_p->bundleCcmList.bundleCcmPrev = NULL;
	ccmConn_p->bundleCcmList.bundleCcmNext = NULL;
	return 0;
}

/*______________________________________________________________________________
**  ccm_releaseOtherChann
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
ccm_releaseOtherChann(
	ccmConn_t* ccmConn_p,
	ccmConn_t* ccmBundleHead
){
	int i_state;
	ccmConn_t *tmpCcmConn_p, *tmpBundleConn_p=NULL, *peerConn_p=NULL;

	tmpCcmConn_p = ccmBundleHead;
	peerConn_p = ccmBundleHead->peerCcmConn;
	CCM_FSM_DBG2("ccm_releaseOtherChann, startId=", ccmBundleHead->origPortId, ", type=", ccmBundleHead->origPortType);
	while ( tmpCcmConn_p != NULL ){
		if ( tmpCcmConn_p != ccmConn_p ) {
			i_state = dirps();
			tmpBundleConn_p = tmpCcmConn_p->bundleCcmList.bundleCcmNext;
			eventVoiceSimpleSend(EVT_CCM_REL_IND, tmpCcmConn_p->origFuncId, tmpCcmConn_p->origPortId);
			ccm_releaseConn(tmpCcmConn_p);
			tmpCcmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
			tmpCcmConn_p->connSt = CALL_CONN_ST_IDLE;
			tmpCcmConn_p = tmpBundleConn_p;
			restore(i_state);
		} else {
			i_state = dirps();
			peerConn_p->peerCcmConn = tmpCcmConn_p;
			if ( tmpCcmConn_p->bundleCcmList.bundleCcmPrev != NULL ) {
				CCM_FSM_DBG("Warning: ccm_releaseOtherChann(), bundleCcmPrev != NULL");
			}
			restore(i_state);
			tmpCcmConn_p = tmpCcmConn_p->bundleCcmList.bundleCcmNext;
		}
	}

	return 0;
}


/*______________________________________________________________________________
**  ccm_change2ZyPhone
**
**  descriptions: Want to delete (such as onHook) the peerCcmConn_p. 
**                     So create the ZyPhone for ccmConn_p to continue the call.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
ccm_change2ZyPhone(
	ccmConn_t* ccmConn_p
){
	ccmConn_t *peerCcmConn_p = NULL, *newCcmConn_p = NULL, *newPeerCcmConn_p = NULL;

	CCM_FSM_DBG("CCM: ccm_change2ZyPhone()");

	peerCcmConn_p = ccmConn_p->peerCcmConn;
	if ( peerCcmConn_p != NULL ) {
		newCcmConn_p = create_ZyPhone(peerCcmConn_p, 0);
		newPeerCcmConn_p = create_ZyPhone(peerCcmConn_p, 0);
		if ( ( newCcmConn_p == NULL ) || ( newPeerCcmConn_p == NULL ) ) {
			CCM_FSM_DBG("Warning: Call Transfer failed to create ZyPhone-1");
		} else {
			/* ZyPhone (newPeerCcmConn_p) bundle with ccmConn_p */
			newPeerCcmConn_p->peerCcmConn = peerCcmConn_p->peerCcmConn;
			newPeerCcmConn_p->callTransferCB = peerCcmConn_p->callTransferCB;
			newPeerCcmConn_p->connSt =  peerCcmConn_p->connSt;
			newPeerCcmConn_p->callFeatures = peerCcmConn_p->callFeatures;
			zyMemcpy( &(newPeerCcmConn_p->dialInform.dialDigitNumber), &(peerCcmConn_p->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
			newPeerCcmConn_p->connRole =  peerCcmConn_p->connRole;
			newPeerCcmConn_p->connType = peerCcmConn_p->connType;
			
			/* ZyPhone (newCcmConn_p) bundle with ccmConn_p */
			newCcmConn_p->peerCcmConn = ccmConn_p->peerCcmConn;
			newCcmConn_p->callTransferCB = NULL;
			newCcmConn_p->connSt =  ccmConn_p->connSt;
			newCcmConn_p->callFeatures = ccmConn_p->callFeatures;
			zyMemcpy( &(newCcmConn_p->dialInform.dialDigitNumber), &(ccmConn_p->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
			newCcmConn_p->connRole =  ccmConn_p->connRole;
			newCcmConn_p->connType = ccmConn_p->connType;

			/* ccmConn_p bundle with newPeerCcmConn_p */;
			ccmConn_p->peerCcmConn = newPeerCcmConn_p;

			/* peerCcmConn_p bundle with newCcmConn_p */
			peerCcmConn_p->peerCcmConn = newCcmConn_p;
			peerCcmConn_p->callTransferCB = NULL;
			peerCcmConn_p->callFeatures = 0;

			/* Change the call back function */
//			eventVoiceDataSend(EVT_CCM_CHANGE_ASSIGN, ccmConn_p->origFuncId, ccmConn_p->origPortId, &newPeerCcmConn_p->origPortType, sizeof(uint16));
//			eventVoiceDataSend(EVT_CCM_CHANGE_ASSIGN, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId, &newCcmConn_p->origPortType, sizeof(uint16));
		}
	} else {
		CCM_FSM_DBG("Warning: Call Transfer failed to create ZyPhone-2");
	}

	return 0;
}

/*______________________________________________________________________________
**  constructCallFeatureCb
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void constructCallFeatureCb(ccmConn_t* ccmConn_p) {

	switch (ccmConn_p->origPortType) {
		case DSP_FXS_PORT:
			/* first add check phone feature */
			createCallFeatureCb(ccmConn_p, (void *)checkNumberingPlanPhoneFeature);
#ifdef SPEED_DIAL
			createCallFeatureCb(ccmConn_p, (void *)checkSpeedDialFeature);
#endif
			#ifdef PSTN_PHONEBOOK
			createCallFeatureCb(ccmConn_p, (void *)checkPSTNPhoneBook);
			#endif
			break;
		case DSP_FXO_PORT:
			break;
		case DSP_FXO_LIFELINE:
			break;
		case SIP_PORT:
			/* first add check phone feature */
			createCallFeatureCb(ccmConn_p, (void *)checkDefaultFeature);
#ifdef DO_NOT_DISTURB
			createCallFeatureCb(ccmConn_p, (void *)checkDNDFeature);
#endif
			break;
		case ZYXEL_PHONE_PORT:
			#if CCBS_SUPPORT
			createCallFeatureCb(ccmConn_p, (void *)checkNumberingPlanPhoneFeature);
#ifdef SPEED_DIAL
			createCallFeatureCb(ccmConn_p, (void *)checkSpeedDialFeature);
#endif
			#endif
			break;
		case ISDN_NT_PORT:
			break;
		case ISDN_TE_PORT:
			break;
		case DSP_DECT_PORT:
			break;
		default:
			ccmConn_p->phoneFeatureCb = NULL;
			break;
	}

	
}

void createCallFeatureCb(ccmConn_t *ccmConn_p, void (*func_p)) {
	ccmPhoneFeatureCb_t *tmpPhoneFeature=NULL;
	int i_state;

	if ( ( tmpPhoneFeature = (ccmPhoneFeatureCb_t *)zyMalloc( sizeof(ccmPhoneFeatureCb_t) ) ) == NULL ) {
		ZyPrintf("alloc ccmPhoneFeatureCb_t error\n\r");
		return;
	}

	zyMemset(tmpPhoneFeature, 0, sizeof(ccmPhoneFeatureCb_t) );
	tmpPhoneFeature->funcPtr = func_p;
	
	i_state = dirps();
	tmpPhoneFeature->prev = NULL;
	tmpPhoneFeature->next = ccmConn_p->phoneFeatureCb;
	if(tmpPhoneFeature->next != NULL) {
		tmpPhoneFeature->next->prev = tmpPhoneFeature;
	}
	ccmConn_p->phoneFeatureCb = tmpPhoneFeature;
	restore(i_state);
	
}


/*______________________________________________________________________________
**  constructCallRouteCb
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void constructCallRouteCb(ccmConn_t* ccmConn_p) {

	switch (ccmConn_p->origPortType) {
		case DSP_FXS_PORT:
			createCallRouteCb(ccmConn_p, (void *)ccmDefaultRoute);
			#ifdef DIAL_PLAN
			createCallRouteCb(ccmConn_p, (void *)ccmDialPlanRoute);
			#endif
			createCallRouteCb(ccmConn_p, (void *)ccmForceToPstn);
			createCallRouteCb(ccmConn_p, (void *)ccmPhoneFeatureRoute);
			break;
		case DSP_FXO_PORT:
			createCallRouteCb(ccmConn_p, (void *)ccmDefaultRoute);
			break;
		case DSP_FXO_LIFELINE:
			break;
		case SIP_PORT:
			createCallRouteCb(ccmConn_p, (void *)ccmDefaultRoute);
#ifdef DO_NOT_DISTURB
			createCallRouteCb(ccmConn_p, (void *)ccmDNDRoute);
#endif
			break;
		case ZYXEL_PHONE_PORT:
			#if CCBS_SUPPORT
			createCallRouteCb(ccmConn_p, (void *)ccmWebDialRoute);
			#endif
			break;
		case ISDN_NT_PORT:
			break;
		case ISDN_TE_PORT:
			break;
		case DSP_DECT_PORT:
			break;
		default:
			ccmConn_p->phoneFeatureCb = NULL;
			break;
	}
}

void createCallRouteCb(ccmConn_t *ccmConn_p, void (*func_p)) {
	ccmCallRouteCb_t *tmpCallRoute=NULL;
	int i_state;

	if ( ( tmpCallRoute = (ccmCallRouteCb_t *)zyMalloc( sizeof(ccmCallRouteCb_t) ) ) == NULL ) {
		ZyPrintf("alloc ccmCallRouteCb_t error\n\r");
		return;
	}

	zyMemset(tmpCallRoute, 0, sizeof(ccmPhoneFeatureCb_t) );
	tmpCallRoute->funcPtr = func_p;
	
	i_state = dirps();
	tmpCallRoute->prev = NULL;
	tmpCallRoute->next = ccmConn_p->callRouteCb;
	if(tmpCallRoute->next != NULL) {
		tmpCallRoute->next->prev = tmpCallRoute;
	}
	ccmConn_p->callRouteCb = tmpCallRoute;
	restore(i_state);
	
}

#ifdef SPEED_DIAL

/*______________________________________________________________________________
**  checkSpeedDialFeature
**
**  descriptions: check dial plan rule in ccm
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void 
checkSpeedDialFeature(ccmConn_t *ccmConn, int *retCode, int *exitFlag)
{
	checkSpeedDialPhBook(&(ccmConn->dialInform.dialDigitNumber));
	CCM_FSM_DBG("checkSpeedDialFeature done");
}

/*______________________________________________________________________________
**  checkSpeedDialPhBook
**
**  descriptions: check if the dial digits belong to the speed dial. If yes, change it to the original numbers.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void
checkSpeedDialPhBook(
	dialDigitNum_t *dialDigitNum_p
){
	int index = 0, phoneBookLen = 0;
	ZyXEL_VoicePhoneBook_t* voicePhBook_p = NULL;
	CCM_FSM_DBG("checkSpeedDialPhBook\n");

	for ( index=0; index<TR104_VPHONE_BOOK_CNT; index++ ) {
		voicePhBook_p = tr104VoicePhoneBookPtrGet(index);
		if(voicePhBook_p==NULL)
			continue;
		phoneBookLen =  zyStrlen(&voicePhBook_p->speedNumber[0]);		
		if ( ( voicePhBook_p->flags & SPEED_DIAL_ACTIVE ) && ( dialDigitNum_p->current_length == phoneBookLen ) ) {
			if ( memcmp(&(voicePhBook_p->speedNumber[0]), &(dialDigitNum_p->dial_digits[0]), phoneBookLen ) == 0 ) {
				zyMemset( dialDigitNum_p, 0, sizeof(dialDigitNum_t) );
				snprintf(&(dialDigitNum_p->dial_digits[0]), MAX_DIAL_STRING_LENGTH-1, "%s" , &(voicePhBook_p->origNumber[0]));
				dialDigitNum_p->current_length = zyStrlen(&(dialDigitNum_p->dial_digits[0]) );
				if (voicePhBook_p->type == SPEED_DIAL_SIP_NONPROXY ) {
					zyMemcpy(&(dialDigitNum_p->force_sipUri[0]), &(voicePhBook_p->forceSipURI[0]), zyStrlen( &(voicePhBook_p->forceSipURI[0])) );
				} else {
					zyMemset(&(dialDigitNum_p->force_sipUri[0]), 0, SIP_DOMAIN_NAME_LEN);
					}
				break;
				}
			}
		}
			
	return;
		}

#endif

#ifdef DO_NOT_DISTURB
/*______________________________________________________________________________
**  checkDNDFeature
**
**  descriptions: check dial plan rule in ccm
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void checkDNDFeature(ccmConn_t *ccmConn, int *retCode, int *exitFlag)
{
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;	
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
	phoneCh_t *phoneCB = NULL;
	ccmConn_t *tmpCcmConn = NULL;
	uint16 phonePhyId = 0;
	
	tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn->origPortId);
	if(tmpConvertEntry_p != NULL){
		int LineItr = 0;
		CCM_PRINTF("checkDNDFeature>ENTER, SipAccountId = %d\n", tmpConvertEntry_p->SipAccountId);
		LineItr = mapSipAccountIdToLineItr(tmpConvertEntry_p->SipAccountId);
		CCM_PRINTF("checkDNDFeature>ENTER, VpItr = %d\n", tmpConvertEntry_p->VpItr);
		CCM_PRINTF("checkDNDFeature>ENTER, LineItr = %d\n", LineItr);
		pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(tmpConvertEntry_p->VpItr, LineItr);
		if(pLineFeature != NULL){
			#if 1
			/*busy forward and DND are conflict feature, decide which feature should be applied to this incoming call*/
			/*check busy forward here in order to have the chance to skip DND check if phone is busy(offhook)*/
			if(pLineFeature->doNotDisturbEnable){
				if(pLineFeature->callForwardOnBusyEnable){
					/*associate fxs phones are all busy -> enter busy forward*/
					tmpCcmConn = ccmConnCb.fxsCcmConn;
					while ( tmpCcmConn != NULL ) {
						phonePhyId = find_phonePhyId(tmpCcmConn->origPortId);
						if ( (phonePhyId != 0) && ( ccmConn->SipFxsMap & (1<<(phonePhyId-1)) ) ) {/* check SIP_FXS mapping */
							if ( ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) && ( tmpCcmConn->peerCcmConn == NULL ) &&( !( tmpCcmConn->flags & CALL_CONN_FLAG_PREASSIGN ) ) ) {
								/*idle fxs phone found -> apply DND*/
								*retCode=CALL_FEATURE_ENABLE_DND;
								CCM_FSM_DBG("DND enabled");
								return;

							}
						}
						tmpCcmConn = tmpCcmConn->next;
					}

					/*associate fxs is onhook -> check DND*/
					ZyPrintf("check idle fxs phones complete\n");
				}
				else{
					*retCode=CALL_FEATURE_ENABLE_DND;
					CCM_FSM_DBG("DND enabled");
					return;
				}	
			}

			
			#else
			if(pLineFeature->doNotDisturbEnable){
				*retCode=CALL_FEATURE_ENABLE_DND;
				CCM_FSM_DBG("DND enabled");
			}
			#endif
		}
	}
	CCM_FSM_DBG("checkDNDFeature done");
}
#endif						

/*______________________________________________________________________________
**  checkDefaultFeature
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void checkDefaultFeature(ccmConn_t *ccmConn, int *retCode, int *exitFlag) {
	switch (ccmConn->origPortType) {
		case DSP_FXS_PORT:
			break;
		case DSP_FXO_PORT:
			break;
		case DSP_FXO_LIFELINE:
			break;
		case SIP_PORT:
			if (ccmConn->callFeatures == CALL_FEATURE_TRANSFER_TRANSFEREE) {
				*retCode = CALL_FEATURE_TRANSFER_TRANSFEREE;
			} else if(ccmConn->callFeatures == CALL_FEATURE_TRANSFER_TARGET){
				*retCode = CALL_FEATURE_TRANSFER_TARGET;
			}
			break;
		case ZYXEL_PHONE_PORT:
			break;
		case ISDN_NT_PORT:
			break;
		case ISDN_TE_PORT:
			break;
		case DSP_DECT_PORT:
			break;
		default:
			ZyPrintf("error ccmConn->origPortType!\r\n");
			break;
	}
	*exitFlag = TRUE;

	CCM_FSM_DBG1("checkDefaultFeature : ", *retCode);
	
}

/*______________________________________________________________________________
**  ccmDefaultRoute
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void ccmDefaultRoute(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag) {
	switch (ccmConn->origPortType) {
		case DSP_FXS_PORT:
			*retCode |= 1<<SIP_PORT;
			break;
		case DSP_FXO_PORT:
			*retCode |= 1<<DSP_FXS_PORT;
			break;
		case DSP_FXO_LIFELINE:
			*retCode |= 1<<DSP_FXS_PORT;
			break;
		case SIP_PORT:
			*retCode |= 1<<DSP_FXS_PORT;
			break;
		case ZYXEL_PHONE_PORT:
			*retCode |= 1<<SIP_PORT;
			break;
		case ISDN_NT_PORT:
			*retCode |= 1<<SIP_PORT;
			break;
		case ISDN_TE_PORT:
			*retCode |= 1<<SIP_PORT;
			break;
		case DSP_DECT_PORT:
			*retCode |= 1<<SIP_PORT;
			break;
		default:
			ZyPrintf("error ccmConn->origPortType!\r\n");
			break;
	}
	*exitFlag = TRUE;

	CCM_FSM_DBG1("ccmDefaultRoute to ", *retCode);
}

#ifdef DIAL_PLAN
//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**  ccmDialPlanRoute
**
**  descriptions: check dial plan rule in ccm
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void 
ccmDialPlanRoute(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag)
{
	
	int gateway = 0;
	sipCCMConvertEntry_t *tmpConvertEntry_p=NULL;
	ccmConn_t *tmpCcmConn=NULL;
	int VpItr = 0;
	uint16 sipLineId=0;

	CCM_FSM_DBG("ccmDialPlanRoute>ENTER");

	CCM_PRINTF("%s, sipFxsMap = 0x%x\n",__FUNCTION__, ccmConn->SipFxsMap);
	tmpCcmConn = ccmConnCb.sipCcmConn;
	while ( tmpCcmConn != NULL ) {
		tmpConvertEntry_p = findSipCCMConvertByPortId(tmpCcmConn->origPortId);
		if ( tmpConvertEntry_p != NULL ) {
			CCM_PRINTF("ccmDialPlanRoute>ENTER, SipAccountId = %d\n", tmpConvertEntry_p->SipAccountId);
			sipLineId = tmpConvertEntry_p->SipAccountId;
			if ( ccmConn->SipFxsMap & (1<<sipLineId) ) {
				gateway = checkDialPlan(tmpConvertEntry_p->VpItr, &(ccmConn->dialInform.dialDigitNumber));	
				CCM_PRINTF("gateway = %d\n", gateway);
			}
		}
		tmpCcmConn = tmpCcmConn->next;
	}

	if( gateway == -1){/*blocked , or error occur*/
		CCM_FSM_DBG("This dial number is blocked number,or error occur");
		*retCode = 0;
		*exitFlag = TRUE;
	}else if( gateway == -2 ){/*do nothing :"no dial plan rule" or "no match rule"*/
		CCM_FSM_DBG(" do nothing ,goto the next check CB!");
	}else{/* find match dial plan rule*/
		*retCode |= 1<<gateway;
		*exitFlag = TRUE;
	}
	
	CCM_FSM_DBG1("ccmDialPlanRoute route to ", *retCode);
}
#endif

/*______________________________________________________________________________
**  ccmForceToPstn
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void ccmForceToPstn(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag) {

	if ( ccmConn->dialInform.dialDigitNumber.dialDigitFlags & DIAL_DIGITAL_FORCE_PSTN ) {
		ccmConn->flags |= CALL_CONN_FLAG_PREFIX;
		*retCode |= 1<<DSP_FXO_PORT;
		*exitFlag = TRUE;
	}

	CCM_FSM_DBG1("ccmForceToPstn route to ", *retCode);
}

/*______________________________________________________________________________
**  ccmPhoneFeatureRoute
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void ccmPhoneFeatureRoute(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag) {

	switch(phoneFeature){
		case CALL_FEATURE_THREE_WAY_CONFERENCE:
		case CALL_FEATURE_TRANSFER_TRANSFEREE:
		case CALL_FEATURE_TRANSFER_TARGET:
			break;
		case CALL_FEATURE_IVR:
		case CALL_FEATURE_ENABLE_WLAN:
		case CALL_FEATURE_DISABLE_WLAN:
		case CALL_FEATURE_CALL_WAITING_ENABLE:
		case CALL_FEATURE_CALL_WAITING_DISABLE:
		case CALL_FEATURE_ENABLE_DND:
		case CALL_FEATURE_DISABLE_DND:
		case CALL_FEATURE_CID_DISPLAY:
		case CALL_FEATURE_CID_HIDDEN:
		#if CCBS_SUPPORT
		case CALL_FEATURE_CCBS_DEACTIVATE:
		#endif
		case CALL_FEATURE_UNCONDITIONAL_FORWARD_ENABLE:
		case CALL_FEATURE_UNCONDITIONAL_FORWARD_DISABLE:
		case CALL_FEATURE_NOANSWER_FORWARD_ENABLE:
		case CALL_FEATURE_NOANSWER_FORWARD_DISABLE:
		case CALL_FEATURE_BUSY_FORWARD_ENABLE:
		case CALL_FEATURE_BUSY_FORWARD_DISABLE:
#ifdef NORWAY_VOIP_CUSTOMIZATION
			if( phoneFeature == CALL_FEATURE_UNCONDITIONAL_FORWARD_ENABLE 
				|| phoneFeature == CALL_FEATURE_UNCONDITIONAL_FORWARD_DISABLE 
				|| phoneFeature == CALL_FEATURE_BUSY_FORWARD_ENABLE
				|| phoneFeature == CALL_FEATURE_BUSY_FORWARD_DISABLE
				|| phoneFeature == CALL_FEATURE_NOANSWER_FORWARD_ENABLE
				|| phoneFeature == CALL_FEATURE_NOANSWER_FORWARD_DISABLE )
            {	
				int len = 0;
                char buf[MAX_DIAL_STRING_LENGTH];
                VoiceProfLineObj_t *pLine = NULL;
				sipCCMConvertEntry_t *tmpConvertEntry_p=NULL;
				int LineItr = 0;
				
				tmpConvertEntry_p = findSipCCMConvertByPortId(ccmConn->outgoingSipAccountId);
			
				if( NULL != tmpConvertEntry_p )
				{
					LineItr = mapSipAccountIdToLineItr(tmpConvertEntry_p->SipAccountId);
					pLine = tr104VoiceLineObjPtrGet(tmpConvertEntry_p->VpItr, LineItr);
				
					if(pLine) {
						memset(buf, 0, sizeof(buf));
						sprintf(buf, "*%s%%23", pLine->directoryNumber);
						len = strlen(buf);
						if((len + ccmConn->dialInform.dialDigitNumber.current_length) < MAX_DIAL_STRING_LENGTH) {
							strcat(ccmConn->dialInform.dialDigitNumber.dial_digits, buf);
							ccmConn->dialInform.dialDigitNumber.current_length += len;
						}
					}
				}
		*retCode |= 1 << SIP_PORT;
                *exitFlag = TRUE;
			}
			else
			{
				*retCode |= 1<<ZYXEL_PHONE_PORT;
				*exitFlag = TRUE;
			}
#else
			*retCode |= 1<<ZYXEL_PHONE_PORT;
			*exitFlag = TRUE;
#endif
			break;
		case CALL_FEATURE_INTERNAL_CALL:
			*retCode |= 1<<DSP_FXS_PORT;
			*exitFlag = TRUE;
			break;
 
		case CALL_FEATURE_CALL_TRANSFER:
			break;
		case CALL_FEATURE_PARK:
		case CALL_FEATURE_PARK_RETRIEVE:
			break;
#ifdef CALL_RETURN_SUPPORT
		case CALL_FEATURE_RETURN:
			if ( ( ccmConn->dialInform.dialDigitNumber.current_length == 0) || !( zyStrcasecmp("anonymous",&(ccmConn->dialInform.dialDigitNumber.dial_digits[0]) ) ))
			{
				*retCode =0;
				*exitFlag = TRUE;				
			}
			break;
#endif
#if 1	/* aircheng add for outgoing sip */
		case CALL_FEATURE_OUTGOING_SIP:
			outgoingSipPhoneFeature(ccmConn);
			*retCode |= (1 << SIP_PORT);
			*exitFlag = TRUE;
			break;
#endif
	}

	CCM_FSM_DBG1("ccmPhoneFeatureRoute route to ", *retCode);
}
#ifdef DO_NOT_DISTURB
/*______________________________________________________________________________
**  ccmDNDRoute
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void ccmDNDRoute(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag) {

	if(phoneFeature==CALL_FEATURE_ENABLE_DND){
			*retCode |= 1<<ZYXEL_PHONE_PORT;
			*exitFlag = TRUE;
		}
	CCM_FSM_DBG1("ccmDNDRoute route to ", *retCode);
	}
#endif

#if CCBS_SUPPORT
/*______________________________________________________________________________
**  ccmWebDialRoute
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void ccmWebDialRoute(ccmConn_t *ccmConn, int *retCode, int phoneFeature, int *exitFlag) {
	phoneCh_t *phoneCB = NULL;
	ccmConn_t *phoneCcmConn = NULL;
	if(zyStrlen(ccmConn->dialInform.dialDigitNumber.dial_digits)!=0){
		if(NULL != ccmConn->WebDialCB){
			ccmConn->SipFxsMap = (uint8)(1 << (ccmConn->WebDialCB->PhonePhyID));
			*retCode |= 1<< SIP_PORT;			
			*exitFlag = TRUE;
		}
	}
	else{ /*WebPhoneDial ZyPhone2 call local phone*/
		if(NULL != ccmConn->WebDialCB){
			phoneCB = find_phoneCB(ccmConn->WebDialCB->PhonePhyID, 1);
			if(NULL!= phoneCB){
				phoneCcmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
			}
			if(NULL != phoneCcmConn){
				ccmConn->SipFxsMap=phoneCcmConn->SipFxsMap;
			}
			else{
				CCM_PRINTF("phone ccmConn not found, sipFxsMap\n");
			}
			
			*retCode |= 1<< DSP_FXS_PORT;
			*exitFlag = TRUE;
		}
	}
	CCM_FSM_DBG1("ccmWebDialRoute route to ", *retCode);
}
#endif

/*______________________________________________________________________________
**  checkPSTNPhoneBook
**
**  descriptions: check if the dial digits belong to the PSTN phoneBook.
**  parameters:
**  local:
**  global:
**  return:  0: Not for LifeLine, 1: LifeLine by dial number, 2: LifeLine relay directly.
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void
checkPSTNPhoneBook(
	ccmConn_t *ccmConn, 
	int *retCode, 
	int *exitFlag
) {
	int fxsTranslate = 0;			/* fxsTranslate:	0: Not for lifeline 			 */
								/*				1: LifeLine by dail number */
								/*				2: LifeLine relay directly	  */
	int index =0, phoneBookLen = 0;
	ZyXEL_VoicePSTN_t* voicePstn_p=NULL;
	uint8 tmpDigits[MAX_DIAL_STRING_LENGTH];
	dialDigitNum_t *dialDigitNum_p = &(ccmConn->dialInform.dialDigitNumber);
	*retCode = -1;
	*exitFlag = FALSE;
	for ( index = 0; index < TR104_VPSTN_CNT; index++) {
		voicePstn_p = tr104VoicePSTNPtrGet(index);
		if ( voicePstn_p->flags & PSTN_PHBOOK_ACTIVE ) {
			CCM_FSM_DBG("checkPSTNPhoneBook, PSTN_PHBOOK_ACTIVE");
			phoneBookLen =	zyStrlen(voicePstn_p->forcePSTNTab);
			if ( voicePstn_p->flags & PSTN_PREFIX_CODE ) {
				CCM_FSM_DBG("checkPSTNPhoneBook, PSTN_PREFIX_CODE");
				CCM_PRINTF("PSTNTab: %s, dial_digits: %s (%d)\n", voicePstn_p->forcePSTNTab, dialDigitNum_p->dial_digits, dialDigitNum_p->current_length);
				/* PSTN Phone Entry has the prefix code */
				if ( memcmp(&(voicePstn_p->forcePSTNTab[0]), &(dialDigitNum_p->dial_digits[0]), phoneBookLen ) == 0 ) {
					if ( dialDigitNum_p->current_length == phoneBookLen ) {
						zyMemset( dialDigitNum_p, 0, sizeof(dialDigitNum_t) );
						dialDigitNum_p->current_length = 0;
						fxsTranslate = 2;
						dialDigitNum_p->dialDigitFlags |= DIAL_DIGITAL_FORCE_PSTN;
						*exitFlag = TRUE;
						break;
					} else {
						zyMemset(&tmpDigits[0], 0, MAX_DIAL_STRING_LENGTH);
						zyMemcpy(&tmpDigits[0], &(dialDigitNum_p->dial_digits[phoneBookLen]), dialDigitNum_p->current_length - phoneBookLen);
						zyMemset( &(dialDigitNum_p->dial_digits[0]), 0, MAX_DIAL_STRING_LENGTH);
						zyMemcpy(&(dialDigitNum_p->dial_digits[0]), &tmpDigits[0], dialDigitNum_p->current_length - phoneBookLen);
						dialDigitNum_p->current_length -= phoneBookLen;
						fxsTranslate = 1;
						dialDigitNum_p->dialDigitFlags |= DIAL_DIGITAL_FORCE_PSTN;
						*exitFlag = TRUE;
						CCM_PRINTF("checkPSTNPhoneBook dial_digits: %s\n", ccmConn->dialInform.dialDigitNumber.dial_digits);
						break;
					}
				}
			} else {
				/* Number match then dial to PSTN */
				if ( ( memcmp(&(voicePstn_p->forcePSTNTab[0]), &(dialDigitNum_p->dial_digits[0]), phoneBookLen) == 0 ) && 
					(dialDigitNum_p->current_length == phoneBookLen) ) {
					fxsTranslate = 1;
					dialDigitNum_p->dialDigitFlags &= DIAL_DIGITAL_FORCE_PSTN;
					*exitFlag = TRUE;
					break;
				}
			}
		}
	}
	CCM_FSM_DBG2("checkPSTNPhoneBook retCode=",*retCode, ", exitFlag = ", *exitFlag);

}


extern sipCCMConvert_t sipCCMConvert;
/*______________________________________________________________________________
**  recordLocalConfPeerLocalPhLogicalId
**
**  descriptions: record phone logical id at phoneCB if the dial digits belong to Local Peer number. 
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
recordLocalConfPeerLocalPhLogicalId(				//Local Peer Call Conference
	uint16	phLogicalId,
	dialDigitNum_t *dialDigitNum_p
) {
	int i = 0, sipLineId = 0, phonenumLen = 0;
	VoiceProfLineObj_t* pLine = NULL;
	ccmConn_t *tmpCcmConn_p=NULL;
	phoneCh_t *selfPhoneCB=NULL, *oppPhoneCB=NULL;
	sipCCMConvertEntry_t *tmpEntry_p = NULL;

	int found=0;
	int LineItr = 0, VpItr=0;

	uint32 NumOfProfile = 0;
	uint32 NumOfLine = 0;

	NumOfProfile = tr104GetNumProfile();
	for(VpItr=0;VpItr<NumOfProfile;VpItr++) {
		NumOfLine = tr104GetVPNumOfLine(VpItr);
		for ( LineItr=0; LineItr<NumOfLine; LineItr++ ) {
			pLine = (VoiceProfLineObj_t*)tr104VoiceLineObjPtrGet(VpItr,LineItr);
			if (pLine == NULL) {
				return 0;
			}
			phonenumLen = zyStrlen(pLine->directoryNumber);
			if((pLine->enable) && (dialDigitNum_p->current_length == phonenumLen)){
				if ( memcmp(pLine->directoryNumber, dialDigitNum_p->dial_digits, phonenumLen ) == 0 ){
					sipLineId = i;
					break;
				}
			}
		}
	}

	if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
		tmpEntry_p = sipCCMConvert.sipCCMConvertEntry;
		while ( tmpEntry_p != NULL ) {
			CCM_PRINTF("recordLocalConfPeerLocalPhLogicalId>ENTER, SipAccountId = %d\n", tmpEntry_p->SipAccountId);
			if ( ( sipLineId == tmpEntry_p->SipAccountId) ){
				if ((tmpCcmConn_p = findOrigCcmConn(SIP_PORT,  tmpEntry_p->sipCcmPortId)) == NULL){
					ZyPrintf("\r\nsipCcmPortId find ccm NULL\r\n");
					continue;
				}
				if ((tmpCcmConn_p->peerCcmConn != NULL) && (tmpCcmConn_p->peerCcmConn->flags & CALL_CONN_FLAG_PEERLOCAL)){
					found=1;
					break;
				

				}


				
			}
			tmpEntry_p = tmpEntry_p->next;
		}
	}

	if(found==0){
		return 0;
	}

	if (tmpCcmConn_p->peerCcmConn != NULL) {
			oppPhoneCB = find_phoneCB(tmpCcmConn_p->peerCcmConn->origPortId, 0);
			if (oppPhoneCB == NULL) {
				ZyPrintf("recordLocalConfPeerLocalPhLogicalId : Can't find opposite phoneCB\n");
				return 0;
			}
	} else{ 
			ZyPrintf("\r\ntmpCcmConn_p->peerCcmConn ==NULL\r\n");	
		return 0;
	}

	if ((selfPhoneCB = find_phoneCB(phLogicalId, 0)) == NULL){
		ZyPrintf("\r\nselfPhoneCB==NULL\r\n");		
		return 0;
	}

	selfPhoneCB->localConfPeerLocalPhLogicalId = oppPhoneCB->logicalId;
	oppPhoneCB->localConfPeerLocalPhLogicalId = selfPhoneCB->logicalId;

	return 0;
}/*recordLocalConfPeerLocalPhLogicalId */

/*______________________________________________________________________________
**  check_associateCCMRegSt
**		SIP      LifeLine     Result
**		Y		Y		1: Still make the call
**		Y		N		1:
**		N		Y		1:
**		N		N		0: Congestion Tone
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
check_associateCCMRegSt(
	ccmConn_t* ccmConn_p
) {
	uint16 sipLineId=0, ret=0;
	ccmConn_t *tmpCcmConn=NULL;
	sipCCMConvertEntry_t *tmpConvertEntry_p=NULL;

	CCM_PRINTF("%s, sipFxsMap = 0x%x\n",__FUNCTION__, ccmConn_p->SipFxsMap);
	/* Find the Idle SIP connect to bundle */
	tmpCcmConn = ccmConnCb.sipCcmConn;
	while ( tmpCcmConn != NULL ) {
		tmpConvertEntry_p = findSipCCMConvertByPortId(tmpCcmConn->origPortId);
		if ( tmpConvertEntry_p != NULL ) {
			CCM_PRINTF("check_associateCCMRegSt>ENTER, SipAccountId %d\n", tmpConvertEntry_p->SipAccountId);
			sipLineId = tmpConvertEntry_p->SipAccountId;
			if ( ccmConn_p->SipFxsMap & (1<<sipLineId) ) {
				if ( tmpCcmConn->flags & CALL_CONN_FLAG_REGISTER ) {
					ret = 1;
					break;
				}
			}
		}
		tmpCcmConn = tmpCcmConn->next;
	}

#if defined(FXO_DSPRELAY) || defined(FXO_LIFELINE) || defined(LIFELINE_AUTODETECT)
	if ( ret == 0 ) {
		/* Find the Idle FXO connect to bundle */
		tmpCcmConn = ccmConnCb.fxoCcmConn;
		while ( tmpCcmConn != NULL ) {
			if ( tmpCcmConn->flags & CALL_CONN_FLAG_REGISTER ) {
				ret = 1;
				break;
			}
			tmpCcmConn = tmpCcmConn->next;
		}
	}
#endif

	return ret;
}

/*______________________________________________________________________________
**  isVoIPinUsed
**
**  descriptions: check if VoIP is in used.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
bool 
isVoIPinUsed(
	void
)
{
	ccmConn_t *tmpCcmConn=NULL;
	
	tmpCcmConn = ccmConnCb.fxsCcmConn;
	while ( tmpCcmConn != NULL ) {
		if(tmpCcmConn->connSt != CALL_CONN_ST_IDLE)
			return TRUE;
		tmpCcmConn = tmpCcmConn->next;
	}
	tmpCcmConn = ccmConnCb.fxoCcmConn;
	while ( tmpCcmConn != NULL ) {
		if(tmpCcmConn->connSt != CALL_CONN_ST_IDLE)
			return TRUE;
		tmpCcmConn = tmpCcmConn->next;
	}
	tmpCcmConn = ccmConnCb.sipCcmConn;
	while ( tmpCcmConn != NULL ) {
		if(tmpCcmConn->connSt != CALL_CONN_ST_IDLE)
			return TRUE;
		tmpCcmConn = tmpCcmConn->next;
	}
#if 0
	tmpCcmConn = ccmConnCb.zyxelCcmConn;
	while ( tmpCcmConn != NULL ) {
		if(tmpCcmConn->connSt != CALL_CONN_ST_IDLE)
			return TRUE;
		tmpCcmConn = tmpCcmConn->next;
	}
#if defined(ISDN_SUPPORT)
	tmpCcmConn = ccmConnCb.isdnCcmConn;
	while ( tmpCcmConn != NULL ) {
		if(tmpCcmConn->connSt != CALL_CONN_ST_IDLE)
			return TRUE;
		tmpCcmConn = tmpCcmConn->next;
	}
#endif
#endif
	return FALSE;
}



