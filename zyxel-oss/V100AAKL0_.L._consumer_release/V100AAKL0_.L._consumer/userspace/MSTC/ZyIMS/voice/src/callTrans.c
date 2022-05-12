#include <stdio.h>
#include "global.h"
#include "event.h"
#include "sys_msg.h"
#include "call_mgnt.h"
#include "callTrans.h"
#include "phfsm.h"


/*______________________________________________________________________________
**  createCallTransferCB
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
createCallTransferCB(
	ccmConn_t *ccmConn_p,
	ccmConn_t *prevPeerCcmConn,
	uint8 callTransferRole,
	uint8 callTransferType
){
	callTransfer_t *callTransfer_p = NULL;

	CCM_FSM_DBG1("createCallTransferCB, ccmConn_p=", (uint32)ccmConn_p);

	/* create the callTransfer control block */
	callTransfer_p = (callTransfer_t *)zyMalloc(sizeof(callTransfer_t));
	zyMemset(callTransfer_p, 0, sizeof(callTransfer_t));
	callTransfer_p->callTransferRole = callTransferRole;
	callTransfer_p->callTransferType = callTransferType;
	callTransfer_p->callTransferStatus = CALL_TRANSFER_ST_IDLE;
	zyMemset( &(callTransfer_p->dialDigitNumber), 0, sizeof(dialDigitNum_t));
	printf("createCallTransferCB dial number = %s\r\n", &ccmConn_p->dialInform.dialDigitNumber.dial_digits[0]);
	zyMemcpy( &(callTransfer_p->dialDigitNumber), &(ccmConn_p->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
	if ( callTransferRole == CALL_TRANSFEROR ) {
		callTransfer_p->callTransferFirstCCM = prevPeerCcmConn;
		if(callTransferType != CALL_TRANSFER_TYPE_UNATTENDED) {
			callTransfer_p->callTransferSecondCCM = ccmConn_p;
			ccmConn_p->callFeatures = CCM_TRANSFER_SECOND_CALL;
		} else {
			callTransfer_p->callTransferSecondCCM = NULL;
		}
		callTransfer_p->callTransferThirdCCM = NULL;
		prevPeerCcmConn->callFeatures = CCM_TRANSFER_FIRST_CALL;	
	} else if ( callTransferRole == CALL_TRANSFEREE ) {
		callTransfer_p->callTransferFirstCCM = prevPeerCcmConn;
		callTransfer_p->callTransferSecondCCM = NULL;
		callTransfer_p->callTransferThirdCCM = ccmConn_p;
		ccmConn_p->callFeatures = CCM_TRANSFER_THIRD_CALL;
		prevPeerCcmConn->callFeatures = CCM_TRANSFER_FIRST_CALL;	
	}
	
	/* Bind the call transfer control block to CCM */
	if(callTransferType != CALL_TRANSFER_TYPE_UNATTENDED) {
		ccmConn_p->callTransferCB = prevPeerCcmConn->callTransferCB = callTransfer_p;
	} else {
		prevPeerCcmConn->callTransferCB = callTransfer_p;
	}

	return 0;
}

/*______________________________________________________________________________
**  removeCallTransferCB
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
removeCallTransferCB(
	callTransfer_t* callTransfer_p
){
	CCM_FSM_DBG("      call-Transfer: removeCallTransferCB");

	if ( callTransfer_p->callTransferFirstCCM != NULL ) {
		callTransfer_p->callTransferFirstCCM->callTransferCB = NULL;
		callTransfer_p->callTransferFirstCCM->callFeatures &= ~CCM_TRANSFER_FIRST_CALL;
	}
	if ( callTransfer_p->callTransferSecondCCM != NULL ) {
		callTransfer_p->callTransferSecondCCM->callTransferCB = NULL;
		callTransfer_p->callTransferSecondCCM->callFeatures &= ~CCM_TRANSFER_SECOND_CALL;
	}
	if ( callTransfer_p->callTransferThirdCCM != NULL ) {
		callTransfer_p->callTransferThirdCCM->callTransferCB = NULL;
		callTransfer_p->callTransferThirdCCM->callFeatures &= ~CCM_TRANSFER_THIRD_CALL;
	}
	zyFree(callTransfer_p);
	callTransfer_p = NULL;

	return;
}

/*______________________________________________________________________________
**  callTransferFsmProc
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
callTransferFsmProc(
	callTransfer_t *callTransfer_p, 
	uint16	callTransEvent,
	void		*data_p,
	uint16	data_size
){
	int status =0;

	CCM_FSM_DBG2("callTransferFsmProc: status=", callTransfer_p->callTransferStatus, " , Role=", callTransfer_p->callTransferRole);

	if ( callTransfer_p->callTransferRole == CALL_TRANSFEROR ) {
		switch( callTransfer_p->callTransferStatus) {
			case CALL_TRANSFER_ST_IDLE:
				status = callTransferorFsmIdle(callTransfer_p, callTransEvent, data_p, data_size);
				break;
			case CALL_TRANSFER_ST_SECOND_CALL:
				status = callTransferorFsmSecond(callTransfer_p, callTransEvent, data_p, data_size);
				break;
			case CALL_TRANSFER_ST_TRANSFER:
				status = callTransferorFsmTransfer(callTransfer_p, callTransEvent, data_p, data_size);
				break;
			default:
				break;
		}
	} else if ( callTransfer_p->callTransferRole == CALL_TRANSFEREE ) {
		switch( callTransfer_p->callTransferStatus) {
			case CALL_TRANSFER_ST_IDLE:
				status = callTransfereeFsmIdle(callTransfer_p, callTransEvent, data_p, data_size);
				break;
			case CALL_TRANSFER_ST_TRANSFER:
				status = callTransfereeFsmTransfer(callTransfer_p, callTransEvent, data_p, data_size);
				break;
			default:
				break;
		}
	}

	return status;

}

/*______________________________________________________________________________
**  callTransferorFsmIdle
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
int callTransferorFsmIdle(callTransfer_t *callTransfer_p, uint16 callTransEvent,void *data_p,uint16 data_size){
	int status =0;
	ccmConn_t *ccmConn_p = NULL, *peerCcmConn_p = NULL;

	CCM_FSM_DBG2("callTransferorFsmIdle: event=", callTransEvent, " ,type=", callTransfer_p->callTransferType);
	#if 1
	switch( callTransEvent ) {		
		case EVT_CT_FIRST_REL:
		case EVT_CT_FIRST_DISC:
			removeCallTransferCB(callTransfer_p);
			break;
		case EVT_CCM_CALL_TRANS_UPDATE:
			break;
		default:
			break;
	}
	#endif
#if 0
	if ( callTransfer_p->callTransferType == CALL_TRANSFER_TYPE_CONSULT ) {
		switch( callTransEvent ) {
			case EVT_CT_SEC_START:
				break;
			case EVT_CT_SEC_OK:
				callTransfer_p->callTransferStatus = CALL_TRANSFER_ST_SECOND_CALL;
				break;
			case EVT_CT_SEC_REL:
				if ( !(callTransfer_p->callTransferFlags & CALL_TRANSFER_SIGNAL_SENDED) ) {
					ccmConn_p = callTransfer_p->callTransferFirstCCM;
					if ( ccmConn_p != NULL ) {
						if ( (peerCcmConn_p = ccmConn_p->peerCcmConn) != NULL ) {
							ccm_change2ZyPhone( ccmConn_p );
							//eventVoiceDataSend(EVT_CCM_UNATTEND_CALL_TRANS_REQ, ccmConn_p->origFuncId, ccmConn_p->origPortId, &callTransfer_p->dialDigitNumber.dial_digits[0], zyStrlen(&callTransfer_p->dialDigitNumber.dial_digits[0]) );
							printf("allTransferFirstCCM : %d , callTransferSecondCCM = %d\r\n", ccmConn_p->origPortId, callTransfer_p->callTransferSecondCCM->origPortId);
							eventVoiceDataSend(EVT_CCM_UNATTEND_CALL_TRANS_REQ, ccmConn_p->origFuncId, ccmConn_p->origPortId, &callTransfer_p->callTransferSecondCCM->origPortId, sizeof(uint16) );
							eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
							callTransfer_p->callTransferFlags |= CALL_TRANSFER_SIGNAL_SENDED;
						}
					}
					callTransfer_p->callTransferSecondCCM->callTransferCB = NULL;
					callTransfer_p->callTransferSecondCCM->callFeatures &= ~CCM_TRANSFER_SECOND_CALL;
					callTransfer_p->callTransferSecondCCM = NULL;
					callTransfer_p->callTransferType = CALL_TRANSFER_TYPE_BLIND;
				}
				break;
			case EVT_CT_SEC_DISC:
				removeCallTransferCB(callTransfer_p);
			default:
				break;
		}
	} else if ( callTransfer_p->callTransferType == CALL_TRANSFER_TYPE_BLIND ) {
		switch( callTransEvent ) {
			case EVT_CT_FIRST_REL:
			case EVT_CT_FIRST_DISC:
				removeCallTransferCB(callTransfer_p);
				break;
			case EVT_CCM_CALL_TRANS_UPDATE:
				break;
			default:
				break;
		}
	}
#endif

	return status;

}

/*______________________________________________________________________________
**  callTransferorFsmSecond
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
callTransferorFsmSecond(
	callTransfer_t *callTransfer_p, 
	uint16	callTransEvent,
	void		*data_p,
	uint16	data_size
){
	int status =0;
	ccmConn_t *firstCcmConn_p = NULL, *firstPeerCcmConn_p = NULL, *secondCcmConn_p = NULL, *secondPeerCcmConn_p = NULL;;

	CCM_FSM_DBG2("callTransferorFsmSecond: event=", callTransEvent, " ,type=", callTransfer_p->callTransferType);
	switch( callTransEvent ) {
		case EVT_CT_SEC_REL:
			if ( (callTransfer_p->callTransferType == CALL_TRANSFER_TYPE_ATTENDED) && ( !(callTransfer_p->callTransferFlags & CALL_TRANSFER_SIGNAL_SENDED) ) ){
				if  ( ( (firstCcmConn_p = callTransfer_p->callTransferFirstCCM) != NULL ) && ( (secondCcmConn_p = callTransfer_p->callTransferSecondCCM) != NULL) &&
					 ( (firstPeerCcmConn_p = firstCcmConn_p->peerCcmConn) != NULL ) && ( (secondPeerCcmConn_p = secondCcmConn_p->peerCcmConn) != NULL ) ) {
						eventVoiceDataSend(EVT_CCM_ATTEND_CALL_TRANS_REQ, firstCcmConn_p->origFuncId, firstCcmConn_p->origPortId, &secondCcmConn_p->origPortId, sizeof(uint16));
						callTransfer_p->callTransferFlags |= CALL_TRANSFER_SIGNAL_SENDED;
				} else {
					CCM_FSM_DBG("Warn: callTransferorFsmSecond() could not find the relative CCM");
					removeCallTransferCB(callTransfer_p);
				}
			}
			break;
		case EVT_CT_REL:
		case EVT_CT_SEC_DISC:
		case EVT_CT_FIRST_REL:
		case EVT_CT_FIRST_DISC:
			removeCallTransferCB(callTransfer_p);
			break;
		case EVT_CCM_CALL_TRANS_UPDATE:
			break;
		default:
			break;
	}

	return status;

}

/*______________________________________________________________________________
**  callTransferorFsmTransfer
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
callTransferorFsmTransfer(
	callTransfer_t *callTransfer_p, 
	uint16	callTransEvent,
	void		*data_p,
	uint16	data_size
){
	int status =0;

	CCM_FSM_DBG2("callTransferorFsmTransfer: event=", callTransEvent, " ,type=", callTransfer_p->callTransferType);
	switch( callTransEvent ) {
		case EVT_CT_FIRST_REL:
		case EVT_CT_FIRST_DISC:
			removeCallTransferCB(callTransfer_p);
			break;
		case EVT_CCM_CALL_TRANS_UPDATE:
			break;
		default:
			break;
	}

	return status;

}

/*______________________________________________________________________________
**  callTransfereeFsmIdle
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
callTransfereeFsmIdle(
	callTransfer_t *callTransfer_p, 
	uint16	callTransEvent,
	void		*data_p,
	uint16	data_size
){
	int status =0;
	ccmConn_t *ccmConn_p = NULL;

	CCM_FSM_DBG2("callTransfereeFsmIdle: event=", callTransEvent, " ,type=", callTransfer_p->callTransferType);
	switch( callTransEvent ) {
		case EVT_CT_THIRD_START:
			break;
		case EVT_CT_THIRD_OK:
			ccmConn_p = callTransfer_p->callTransferFirstCCM;
			if ( ccmConn_p != NULL ) {
				eventVoiceSimpleSend(EVT_CCM_CALL_TRANS_RSP, ccmConn_p->origFuncId, ccmConn_p->origPortId);
			}
			callTransfer_p->callTransferStatus = CALL_TRANSFER_ST_TRANSFER;
			break;
		case EVT_CT_THIRD_REL:
		case EVT_CT_THIRD_DISC:
			ccmConn_p = callTransfer_p->callTransferFirstCCM;
			if ( ccmConn_p != NULL ) {
				eventVoiceSimpleSend(EVT_CCM_CALL_TRANS_RSP, ccmConn_p->origFuncId, ccmConn_p->origPortId);
			}
			removeCallTransferCB(callTransfer_p);
			break;
		default:
			break;
	}

	return status;

}

/*______________________________________________________________________________
**  callTransfereeFsmTransfer
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
callTransfereeFsmTransfer(
	callTransfer_t *callTransfer_p, 
	uint16	callTransEvent,
	void		*data_p,
	uint16	data_size
){
	int status =0;
	ccmConn_t *ccmConn_p = NULL;

	CCM_FSM_DBG2("callTransfereeFsmTransfer: event=", callTransEvent, " ,type=", callTransfer_p->callTransferType);
	switch( callTransEvent ) {
		case EVT_CT_THIRD_DISC:
		case EVT_CT_FIRST_DISC:
			ccmConn_p = callTransfer_p->callTransferFirstCCM;
			if ( ccmConn_p != NULL ) {
				eventVoiceSimpleSend(EVT_CCM_CALL_TRANS_RSP, ccmConn_p->origFuncId, ccmConn_p->origPortId);
			}
			removeCallTransferCB(callTransfer_p);
			break;
		default:
			break;
	}

	return status;

}

