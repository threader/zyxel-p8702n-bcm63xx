#include <stdio.h>
#include <switch.h>
#include <global.h>
#include "event.h"
#include "sys_msg.h"
#include "call_mgnt.h"
#include "phfsm.h"
#include "Phcmd.h"
#include "dsp_itf.h"

/*____________________________________________________________________________*/
/*	constant definition                                                                                                       */
/*____________________________________________________________________________*/

/*____________________________________________________________________________*/
/*	type definition                                                                                                             */
/*____________________________________________________________________________*/
#define SUCCESS 	0

/*____________________________________________________________________________*/
/*	global variables                                                                                                             */
/*____________________________________________________________________________*/

/*____________________________________________________________________________*/
/*	Functions                                                                                                                    */
/*____________________________________________________________________________*/
/*______________________________________________________________________________
**  phStopTone
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
phStopTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	PH_FSM_DBG("phStopTone");
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStopTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStopTone(phoneCB);
			break;
#endif
	}
	return err;

} /* phStopTone */

/*______________________________________________________________________________
**  phStartDialTone
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
phStartDialTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartDialTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartDialTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartDialTone */

/*______________________________________________________________________________
**  phStartMWITone
**
**  descriptions: play the Message Waiting Indication Tone
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision: for Sylantro, 0.5s on, 0.05s off, 425Hz
**____________________________________________________________________________*/
int
phStartMWITone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartMWITone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartMWITone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartMWITone */

/*______________________________________________________________________________
**  phStartBusyTone
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
phStartBusyTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	PH_FSM_DBG("phStartBusyTone");
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartBusyTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartBusyTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartBusyTone */

/*______________________________________________________________________________
**  phStartRingBackTone
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
phStartRingBackTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartRingBackTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartRingBackTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartRingBackTone */

/*______________________________________________________________________________
**  phStartReorderTone
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
phStartReorderTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartReorderTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartReorderTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartReorderTone */

/*______________________________________________________________________________
**  phStartCongestTone
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
phStartCongestTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartCongestTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartCongestTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartCongestTone */

/*______________________________________________________________________________
**	phStartPositiveAckTone
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
int
phStartPositiveAckTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartPositiveAckTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartPositiveAckTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartPositiveAckTone */

/*______________________________________________________________________________
**	phStartNegativeAckTone
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
int
phStartNegativeAckTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartNegativeAckTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartNegativeAckTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartNegtiveAckTone */



/*______________________________________________________________________________
**  phStartBeepTone
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
phStartBeepTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartBeepTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartBeepTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartBeepTone */

/*______________________________________________________________________________
**  phStartCallWaitingTone
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
phStartCallWaitingTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartCallWaitingTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartCallWaitingTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartCallWaitingTone */

#ifdef DO_NOT_DISTURB
/*______________________________________________________________________________
**  phStartSpecialDialTone
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
phStartSpecialDialTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartSpecialDialTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartSpecialDialTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartSpecialDialTone */
#endif /* DO_NOT_DISTURB */
/*______________________________________________________________________________
**  phStartConfirmTone
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
phStartConfirmTone(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartConfirmTone(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartConfirmTone(phoneCB);
			break;
#endif
	}
	return err;
}/* phStartSpecialDialTone */

/*______________________________________________________________________________
**  phEnableDtmfDetect
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
phEnableDtmfDetect(
	phoneCh_t *phoneCB
){

	return SUCCESS;
} /* phEnableDtmfDetect */

/*______________________________________________________________________________
**  phDisableDtmfDetect
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
phDisableDtmfDetect(
	phoneCh_t *phoneCB
){

	return SUCCESS;
} /* phDisableDtmfDetect */

/*______________________________________________________________________________
**  phPolarityReverse
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
phPolarityReverse(
	phoneCh_t *phoneCB
){
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicPolarityReverse(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectPolarityReverse(phoneCB);
			break;
#endif
	}
	return err;
} /* phPolarityReverse */

/*______________________________________________________________________________
**  phStartRing
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
phStartRing(
	phoneCh_t *phoneCB
) {
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStartRing(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStartRing(phoneCB);
			break;
#endif
	}
	return err;
}



/*______________________________________________________________________________
**  phSetTxGain
**
**  descriptions:
**  parameters: value in 0.1db unit,  ex. 10 = 1 db
**  local:
**  global:
**  return: TRUE on success, FALSE on fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int phSetTxGain(
	phoneCh_t *phoneCB,
	int value /* 0.1db unit*/
) {
	int ret = 0;
	ret = slicSetTxGain(phoneCB, value);
	if(ret == 0){
		return TRUE;
	}

	return FALSE;
}

/* __MSTC__, Klose,merge from P876 feature, Set egress/ingress gain to control the Speaking/Listening volume, 20121107 */
int phSetEptTxGain(
	phoneCh_t *phoneCB,
	int value /* 0.1db unit*/
) {
	int ret = 0;
	ret = slicSetEptTxGain(phoneCB, value);
	if(ret == 0){
		return TRUE;
	}

	return FALSE;
}


/*______________________________________________________________________________
**  phSetRxGain
**
**  descriptions:
**  parameters: value in 0.1db unit,  ex. 10 = 1 db
**  local:
**  global:
**  return: TRUE on success, FALSE on fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int phSetRxGain(
	phoneCh_t *phoneCB,
	int value /* 0.1db unit*/
) {
	int ret = 0;
	ret = slicSetRxGain(phoneCB,value);
	if(ret == 0){
		return TRUE;
	}

	return FALSE;
}

/* __MSTC__, Klose,merge from P876 feature, Set egress/ingress gain to control the Speaking/Listening volume, 20121107 */
int phSetEptRxGain(
	phoneCh_t *phoneCB,
	int value /* 0.1db unit*/
) {
	int ret = 0;
	ret = slicSetEptRxGain(phoneCB,value);
	if(ret == 0){
		return TRUE;
	}

	return FALSE;
}


/*______________________________________________________________________________
**  phSendCID
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

void phSendCID(
	phoneCh_t *phoneCB
) {

	switch(phoneCB->dspPortType)	{
		case DSP_FXS_PORT:
			vDspSendCID(phoneCB);
			break;			
		case DSP_DECT_PORT:
#if (SUPPORT_DECT == WB_3519)			
			dectSendCID(phoneCB);
#endif
			break;
		default:
			dbg_pline_1("phoneCB->dspPortType error\r\n");
			break;
	}
}

/*______________________________________________________________________________
**  phStopRing
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
phStopRing(
	phoneCh_t *phoneCB
) {
	int err = SUCCESS;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicStopRing(phoneCB);
			break;
#if (SUPPORT_DECT == WB_3519)
		case DSP_DECT_PORT:
			err = dectStopRing(phoneCB);
			break;
#endif
	}
	return err;
}

/*______________________________________________________________________________
**  phSetSLICStatus
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
phSetSLICStatus(
	uint16 chId,
	uint8 slicSt
){
	slicSetSLICStatus(chId, slicSt);
	return;
}

/*______________________________________________________________________________
**  phHookStatus
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
phHookStatus(
	phoneCh_t *phoneCB
){
	int err = 0;
	switch(phoneCB->dspPortType) {
		case DSP_FXS_PORT:
			err = slicHookStatus(phoneCB);
			break;
	}
	return err;
}

/*______________________________________________________________________________
**	phCallWaittingToneCallBack
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
phCallWaittingToneCallBack(
	tm_id_t tmId, 
	void *param 
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)param;

	if(tmpPhoneCB->CallWaitingToneCounter % 2){
		phStopTone(tmpPhoneCB);
		tmpPhoneCB->callProTimer.fxsCallWaittingRepeater = timerStartS(CALL_WAITTING_REPEAT_OFF_TIME, phCallWaittingToneCallBack, tmpPhoneCB);
	}else{
		phStartCallWaitingTone(tmpPhoneCB);
		tmpPhoneCB->callProTimer.fxsCallWaittingRepeater = timerStartS(CALL_WAITTING_REPEAT_ON_TIME, phCallWaittingToneCallBack, tmpPhoneCB);
	}
	tmpPhoneCB->CallWaitingToneCounter++;

	if(tmpPhoneCB->CallWaitingToneCounter >= 4){
		phStopTone(tmpPhoneCB);
		timerStop(tmpPhoneCB->callProTimer.fxsCallWaittingRepeater);
		tmpPhoneCB->callProTimer.fxsCallWaittingRepeater = 0;
		tmpPhoneCB->CallWaitingToneCounter = 0;
	}

	return 0;
}/* phCallWaittingToneCallBack */

/*______________________________________________________________________________
**	phStartCptTone
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
int 
phStartCptTone(
	phoneCh_t*phoneCB, 
	uint8 toneType
) {
	switch(toneType) {
		case LINE_EVT_PHONE_CALL_WAITING_TONE:
#if 1
			phStartCallWaitingTone(phoneCB);
#else
			phoneCB->CallWaitingToneCounter = 0;
			phoneCB->callProTimer.fxsCallWaittingRepeater = timerStartS(CALL_WAITTING_REPEAT_ON_TIME, phCallWaittingToneCallBack, phoneCB);
#endif
			return SUCCESS;
		default:
			return -1;
	}	
}

/* The below: "Support Caller-ID" */
/*______________________________________________________________________________
**	phStartRingingPulseAlertingSignal
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
int
phStartRingingPulseAlertingSignal(
	phoneCh_t*phoneCB
) {

	return 0;
}

/*______________________________________________________________________________
**	phStartDualPluseAlertingSignal
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
int
phStartDualPluseAlertingSignal(
	phoneCh_t*phoneCB
) {
	int err = SUCCESS;
	return err;
}

/*______________________________________________________________________________
**  phGetRingOnInterval
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
phGetRingInterval(
	int type,
	int ringParam
){
	uint16 ringInterval = 0;

	return ringInterval;
}

/*______________________________________________________________________________
**  phPlayDTAS
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
phPlayDTAS(
	tm_id_t timer_id, 
	void *arg
){
	
	return 0;
}

/*______________________________________________________________________________
**  phWaitPhoneACKTimeout
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
phWaitPhoneACKTimeout(			//OffHook CallerID DTAS ACK timeout 
	tm_id_t timer_id, 
	void *arg
){
	return 0;
}

/*______________________________________________________________________________
**  phStopDTASTone
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
static int          
phStopDTASTone(
	tm_id_t timer_id, 
	void *arg
){
	return SUCCESS;
} /* phStopDTASTone */


#if (defined FXO_DSPRELAY) || (defined FXO_LIFELINE)
/*______________________________________________________________________________
**  fxoOffHook
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
fxoOffHook(
	phoneCh_t*phoneCB
){
	daaOffhook(phoneCB);
	return;
}

/*______________________________________________________________________________
**  fxoOnHook
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
fxoOnHook(
	phoneCh_t*phoneCB
){
	daaOnhook(phoneCB);
	return;
}

/*______________________________________________________________________________
**  fxoGenDtmfTone
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
fxoDial(
	phoneCh_t*phoneCB
){
	daaDial(phoneCB);
}


/*______________________________________________________________________________
**  fxoGenDtmfTone
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
fxoGenDtmfTone(
	uint16 chId,
	char dialDigit
){
	return;
}

#ifdef FXO_DSPRELAY
/*______________________________________________________________________________
**  fxofxsChannelConnect
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
fxofxsChannelConnect(
	phoneCh_t *phoneCB,
	int	enable
){
	printf("fxofxsChannelConnect\n");

	if ( enable == 1 ) {
		printf("phoneCB->physicalId=%d\n",phoneCB->physicalId);
		vDspCreateInternalCall(phoneCB);
	}
	return;
}
#endif

#if ( defined(FXO_LIFELINE) || defined(FXO_DSPRELAY) )
/*______________________________________________________________________________
**  fxoLifeLineRelay
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
fxoLifeLineRelay(
	uint16 chId,
	int	enable
){

	return;
}
#endif

/*______________________________________________________________________________
**  fxoOffHookDetect
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
fxoOffHookDetect(
	phoneCh_t *phoneCB
){

	return;
}

/*______________________________________________________________________________
**  fxoOnHookDetect
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
fxoOnHookDetect(
	phoneCh_t *phoneCB
){
	return;
}

/*______________________________________________________________________________
**  fxoDisableHookDetect
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
fxoDisableHookDetect(
	phoneCh_t*phoneCB
){
	return;
}

/*******************************************************************************
* FUNCTION:     fxoCheckLinkStatus
*
* DESCRIPTION: Use to check the hook status after the lifeline switch.
*
*******************************************************************************/
void
fxoCheckLinkStatus(
	phoneCh_t*phoneCB
) {
	return;
}

/*______________________________________________________________________________
**  swapTimeSlot
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
swapTimeSlot(
	uint16 chId1,
	uint16 chId2,
	uint16 type
){
	return;
}
#endif

/* For the Caller-ID Support */
#ifdef AUXILARY_RELAY
/*______________________________________________________________________________
**  fxoLineSt
**
**  descriptions:.
**  parameters:
**  local:
**  global:
**  return: 1, the line voltage is low. There are phone already off-hook.
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
fxoLineSt(
	uint16 chId
){

}

/*______________________________________________________________________________
**  fxoAuxilaryRelay
**
**  descriptions:.
**  parameters:
**  local:
**  global:
**  return: 1, the line voltage is low. There are phone already off-hook.
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
fxoAuxilaryRelay(
	int	enable
){
}

/*______________________________________________________________________________
**  fxoAuxilaryHookStatus
**
**  descriptions:.
**  parameters:
**  local:
**  global:
**  return: 1, the line voltage is low. There are phone already off-hook.
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
fxoAuxilaryHookStatus(
	void
){
}

/*______________________________________________________________________________
**  vDspChanConnect
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
vDspChanConnect(
	uint16 chId
){

	return SUCCESS;
} /* vDspChanConnect */
#endif

/*______________________________________________________________________________
**	map_chId_to_timeslot
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
uint16 map_chId_to_timeslot(uint16 chId)
{
	return (chId+1);
}   

#ifdef LIFELINE_AUTODETECT
int lifelineExistCheck(void){
}

int iflifelineExist(void){
}
#endif

