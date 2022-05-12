#include <switch.h>
#include <global.h>
#include "event.h"
#include "sys_msg.h"
#include "call_mgnt.h"
#include "phfsm.h"

void slicInit(
	void
) {
	
}

void slic_hw_init(
	uint16 phPhyId
) {
	
}
 
int slicStopTone(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStopTone(phoneCB))!=0) {
		printf("slicStopTone error\n");
	}
	return err;
}

int slicStartDialTone(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStartDialTone(phoneCB))!=0) {
		printf("slicStartDialTone error\n");
	}
	return err;	
}

#ifdef DO_NOT_DISTURB
int slicStartSpecialDialTone(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStartSpecialDialTone(phoneCB))!=0) {
		printf("slicStartSpecialDialTone error\n");
	}
	return err;
}
#endif /* DO_NOT_DISTURB */

int slicStartConfirmTone(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStartConfirmTone(phoneCB))!=0) {
		printf("slicStartConfirmTone error\n");
	}
	return err;
}


int slicStartMWITone(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStartMWITone(phoneCB))!=0) {
		printf("slicStartMWITone error\n");
	}
	return err;		
}

int slicStartBusyTone(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStartBusyTone(phoneCB))!=0) {
		printf("slicStartBusyTone error\n");
	}	
	return err;	
}

int slicStartRingBackTone(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStartRingBackTone(phoneCB))!=0) {
		printf("slicStartRingBackTone error\n");
	}
	return err;
}

int slicStartReorderTone(
	phoneCh_t *phoneCB
) {
	
	return 0;
}

int slicStartCongestTone(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStartCongestTone(phoneCB))!=0) {
		printf("slicStartBusyTone error\n");
	}
	return err;	
}

int slicStartBeepTone(
	phoneCh_t *phoneCB
) {	
	return 0;
}

int slicStartPositiveAckTone(
	phoneCh_t *phoneCB
) {	
	int err=-1;
	if ((err=PhoneStartTone(phoneCB, 2 /*EPSIG_CONF*/))!=0) {
		printf("slicStartPositiveAckTone error\n");
	}
	return err; 

	return 0;
}

int slicStartNegativeAckTone(
	phoneCh_t *phoneCB
) {	
	int err=-1;
	if ((err=PhoneStartTone(phoneCB, 29 /*EPSIG_WARN*/))!=0) {
		printf("slicStartNegativeAckTone error\n");
	}
	return err; 

	return 0;
}



int slicStartCallWaitingTone(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStartCallWaitingTone(phoneCB))!=0) {
		printf("slicStartCallWaitingTone error\n");
	}
	return err;	
}

#ifdef SUPPORT_COUNTRY_CODE
int slicStartToneCallBack(
	tm_id_t tmId, 
	void *param
) {
	
	return 0;
}
#endif

int slicPolarityReverse(
	phoneCh_t *phoneCB
) {
	
	return 0;
}

int slicStartRing(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStartRing(phoneCB))!=0) {
		printf("slicStartRing error\n");
	}
	return err;
}

#ifdef SUPPORT_COUNTRY_CODE
int slicStartRingAgain(
	tm_id_t tmId,
	void *param
) {
	
}
#endif

int slicStopRing(
	phoneCh_t *phoneCB
) {
	int err=-1;
	if ((err=PhoneStopRing(phoneCB))!=0) {
		printf("slicStopRing error\n");
	}
	return err;
	
}

void slicSetSLICStatus(
	uint16 chId, 
	uint8 slicSt
) {
	
}

/*______________________________________________________________________________
**  slicSetTxGain
**
**  descriptions:
**  parameters: value in 0.1db unit,  ex. 10 = 1 db
**  local:
**  global:
**  return: 0 on success, -1 on fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int slicSetTxGain(
	phoneCh_t *phoneCB,
	int value
) {
	return PhoneSetTxGain(phoneCB, (value/10));
}

/* __MSTC__, Klose,merge from P876 feature, Set egress/ingress gain to control the Speaking/Listening volume, 20121107 */
int slicSetEptTxGain(
	phoneCh_t *phoneCB,
	int value
) {
	return PhoneSetEptTxGain(phoneCB, (value/10));
}

/*______________________________________________________________________________
**  slicSetRxGain
**
**  descriptions:
**  parameters: value in 0.1db unit,  ex. 10 = 1 db
**  local:
**  global:
**  return: 0 on success, -1 on fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int slicSetRxGain(
	phoneCh_t *phoneCB,
	int value
) {
	return PhoneSetRxGain(phoneCB, (value/10));
}

/* __MSTC__, Klose,merge from P876 feature, Set egress/ingress gain to control the Speaking/Listening volume, 20121107 */
int slicSetEptRxGain(
	phoneCh_t *phoneCB,
	int value
) {
	return PhoneSetEptRxGain(phoneCB, (value/10));
}

int slicHookStatus(
	phoneCh_t *phoneCB
) {
	int hookstatus = 0;
	hookstatus = GetHooKstatus(phoneCB);
	return hookstatus;
}

int slicStartRingingPulseAlertingSignal(
	phoneCh_t*phoneCB
) {
	
	return 0;
}

int slicStartDualPluseAlertingSignal(
	phoneCh_t*phoneCB
) {
	
	return 0;
}


void daaOnhook(phoneCh_t *phoneCB){
	PhonePSTNOnhook(phoneCB);
}

void daaOffhook(phoneCh_t *phoneCB){
	PhonePSTNOffhook(phoneCB);
}

void daaDial(phoneCh_t *phoneCB){
	PhonePSTNOutgoing(phoneCB);
}


