/*
   $Id: Ivr.c 1.14.1.2 2007/05/24 11:35:53 mrchung Exp $
*/
/************************************************************************
 *
 *	Copyright (C) 2003-2005 ZyXEL Communications, Corp.
 *	All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Corp. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * ZyXEL Communications, Corp.
 *
 *************************************************************************/
/*
** $Log: Ivr.c $
** Revision 1.14.1.2  2007/05/24 11:35:53  mrchung
** [Bug Fix] (VoIP) 1. Can't enter IVR, exception occures
**                           2. Press flash key for second dial, exception occures.
** Revision 1.14.1.1  2007/05/14 10:04:37  mrchung
** Add more features of VoIP following the P2602HWNLI-D7A.
** Revision 1.14  2006/10/04 10:34:49  eric.chung
** fix support ISDN complier fail.
** Revision 1.13  2006/10/02 12:02:35  BlueLan
** g.726
** Revision 1.12  2006/09/25 01:53:17  Weicheng
** Support GNU compiler and Danube platform
** Revision 1.11  2006/07/27 05:43:24  AlexSo
** Make it easier to read if IVRCheckCodec return a false value
** Revision 1.10  2006/07/27 05:10:30  AlexSo
** Default early media ringtone fixed
** Revision 1.9  2006/07/25 09:30:51  AlexSo
** Default early media ringtone
** Revision 1.8  2006/07/21 09:35:01  AlexSo
** Fix: Early Media can't send out ringtone with g.711u codec
** Revision 1.7  2006/07/11 12:58:32  tommyhsu
** 1.Fix Dect phone can't play ivr when device finish the Auto-provision
** Revision 1.6  2006/06/28 07:41:06  kehuang
** Add P2602HW, P2602HWL, P2602R support
** Revision 1.5  2006/06/15 09:52:43Z  eric.chung
** add ISDN support
** Revision 1.4  2006/06/07 02:48:04  tommyhsu
** DECT can't play IVR over 3 times
** Revision 1.3  2006/04/28 08:16:46  Eric_Chen
** Some bugs fix with dsp pool and common APIs integration
** Revision 1.2  2006/04/19 08:13:47  Eric_Chen
** Replace FSM and hardware depend related files to phase 2
** Revision 1.14  2006/04/18 13:41:29  Eric_Chen
** Add DSP Pool with PMC platform
** Revision 1.13  2006/03/20 09:01:26  Weicheng
** Fix trunking bug when IVR interval too long
** Revision 1.12  2005/12/30 13:00:51  Rudi
** Change "NUM_IVRUSR" to "NUM_IVR_UPGRADE"
** Add Compiler Flag "N0_IVRUSR_PATH"
** Revision 1.11  2005/08/15 07:00:19  Rudi
** Erase Unusable IVR User during IVR Init and add Error Log 
** Revision 1.10  2005/06/30 02:33:31  ohsieh
** Add G723 Support
** Revision 1.9  2005/06/07 07:59:11  cwchen
** Separate the definition for the different IVR into atuoFWUpdate.h.
** Revision 1.8  2005/05/09 14:41:03  Rudi
** Update IVR to Standard (OBM) version - all English voice prompts
** Revision 1.7  2005/05/09 06:24:39  cwchen
** Remove some un-used message.
** Revision 1.6  2005/04/09 11:56:15  ccshen
** Change check and erase user part ivr to first time using ivr.
** Revision 1.5  2005/04/07 08:11:29  ccshen
** Add crc check mechanism and get ivrusr part to .h file function
** Revision 1.4  2005/03/03 14:13:10  cwchen
** Enhance the IVR to bundle the voipIvrCallBack() with the IVR channel.
** Revision 1.3  2005/02/01 11:33:05  cwchen
** Support EMBEDDED_IVR. And enhance the IVR mechanism.
** Revision 1.2  2005/01/10 09:19:22  cwchen
** Enhance the configuration for the language type of the IVR voice
** Revision 1.1  2004/12/15 06:54:00  cwchen
** Initial revision
** Revision 1.1  2004/10/11 13:39:53  ccshen
** Initial revision
*/
#include "global.h"

#include "call_mgnt.h"
#include "phfsm.h"
#include "ivrcmd.h"
#include "event.h"
#include <sys/time.h>



voiceIvrCb_t voiceIvrCb;


voiceIvrCh_t *voiceIvrChCb=NULL;
int ivrDebug=1;

//extern int RtpSendToDsp(uint16 chId, uint32 payload_type, uint8 *InMessage, uint16 MessageLen, uint32 TimeStamp, void* handle);


//int ivrInit(void);
voiceIvrCh_t* createVoiceIvrCh(
	uint16 logicalId,
	uint16 logicalType	
);
int deleteVoiceIvrCh(	voiceIvrCh_t* tmpVoiceIvrCh_p);
voiceIvrCh_t * findVoiceIvrCh(uint16 logicalId,	uint16 logicalType);
int ivrCmdPlayStart(voiceIvrCh_t *tmpVoiceIvrCh_p,	uint16 ivrIndex,	uint16 ivrType,	uint32 timeInterval);

//int voiceIvrChList(void);
//int ivrGetVoiceRecord(char **buf, uint32 *length);
//uint8 IVRCheckCodec(uint16 ivrIndex,uint16 ivrType);

/*______________________________________________________________________________
**	createVoiceIvrCh
**	descriptions: create the new voice IvrCh
**	parameters:
**	local:
**	global:
**	return: 0
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

voiceIvrCh_t*
createVoiceIvrCh(
	uint16 logicalId,
	uint16 logicalType
){
	int i_state;
	voiceIvrCh_t *tmpVoiceIvrCh_p = NULL, *newVoiceIvrCh_p = NULL, **hp = NULL;

	IVR_FSM_DBG("createVoiceIvrCh()");

	if ( (  newVoiceIvrCh_p = (voiceIvrCh_t *)calloc(1, sizeof(voiceIvrCh_t) ) ) == NULL ) {
		printf("allocate voiceIvrCh failed\n\r");
		return NULL;
	}

	/* Assign the initial value */
	newVoiceIvrCh_p->logicalId = logicalId;
	newVoiceIvrCh_p->logicalType = logicalType;
	//newVoiceIvrCh_p->voipIvrCallBack = voipIvrCallBack;
	newVoiceIvrCh_p->prev = NULL;
	newVoiceIvrCh_p->next = NULL;
	//newVoiceIvrCh_p->per.assemPakcet = NULL;
	//newVoiceIvrCh_p->per.packet = NULL;

	/* add the new entry to the link list */
	if ( voiceIvrCb.voiceIvrCh_p != NULL ) {
		tmpVoiceIvrCh_p = voiceIvrCb.voiceIvrCh_p;
		while ( tmpVoiceIvrCh_p != NULL ) {
			if ( newVoiceIvrCh_p->logicalId < tmpVoiceIvrCh_p->logicalId ) {
				tmpVoiceIvrCh_p = tmpVoiceIvrCh_p->prev;
				break;
			}
			if ( tmpVoiceIvrCh_p ->next != NULL ) {
				tmpVoiceIvrCh_p = tmpVoiceIvrCh_p->next;
			} else {
				break;
			}
		}
	}

	if ( tmpVoiceIvrCh_p == NULL ) {
		hp = &(voiceIvrCb.voiceIvrCh_p);
		i_state = dirps();
		newVoiceIvrCh_p->prev = NULL;
		newVoiceIvrCh_p->next = *hp;
		if(newVoiceIvrCh_p->next != NULL)
			newVoiceIvrCh_p->next->prev = newVoiceIvrCh_p;
		*hp = newVoiceIvrCh_p;
		restore(i_state);
	} else {
		i_state = dirps();
		newVoiceIvrCh_p->next = tmpVoiceIvrCh_p->next;
		newVoiceIvrCh_p->prev = tmpVoiceIvrCh_p;
		if(newVoiceIvrCh_p->prev != NULL)
			newVoiceIvrCh_p->prev->next = newVoiceIvrCh_p;
		if(newVoiceIvrCh_p->next != NULL)
			newVoiceIvrCh_p->next->prev = newVoiceIvrCh_p;
		restore(i_state);
	}

	/* Adjust the ivrChannel Numbers */
	voiceIvrCb.ivrNumbers +=1;

	return newVoiceIvrCh_p;
} /* createVoiceIvrCh */

/*______________________________________________________________________________
**	deleteVoiceIvrCh
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
deleteVoiceIvrCh(
	voiceIvrCh_t* tmpVoiceIvrCh_p
) {
	int i_state;

	IVR_FSM_DBG("deleteVoiceIvrCh()");

	if ( tmpVoiceIvrCh_p != NULL ) {
		IVR_FSM_DBG1("deleteVoiceIvrCh - tmpVoiceIvrCh_p=", (unsigned long)tmpVoiceIvrCh_p);
		/* Remove the tmpEntry_p from the link list */
		i_state = dirps();
		if ( tmpVoiceIvrCh_p->prev != NULL ) {
			tmpVoiceIvrCh_p->prev->next = tmpVoiceIvrCh_p->next;
		} else {
			voiceIvrCb.voiceIvrCh_p = tmpVoiceIvrCh_p->next;
		}
		if ( tmpVoiceIvrCh_p->next != NULL ) {
			tmpVoiceIvrCh_p->next->prev = tmpVoiceIvrCh_p->prev;
		}
		tmpVoiceIvrCh_p->next = NULL;
		tmpVoiceIvrCh_p->prev = NULL;
		restore(i_state);

		voiceIvrCb.ivrNumbers -= 1;
		zyFree(tmpVoiceIvrCh_p);
		tmpVoiceIvrCh_p = NULL;
	}

	return 0;
}/* deleteVoiceIvrCh */

/*______________________________________________________________________________
**	findVoiceIvrCh
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

voiceIvrCh_t *
findVoiceIvrCh(
	uint16 logicalId,
	uint16 logicalType
) {
	int found = 0;
	voiceIvrCh_t *tmpVoiceIvrCh_p = NULL;
	IVR_FSM_DBG1("findVoiceIvrCh() logicalId = ",logicalId);

	if ( voiceIvrCb.voiceIvrCh_p != NULL ) {
		tmpVoiceIvrCh_p = voiceIvrCb.voiceIvrCh_p;
		while ( tmpVoiceIvrCh_p != NULL ) {
			if ( ( tmpVoiceIvrCh_p->logicalId == logicalId ) && ( tmpVoiceIvrCh_p->logicalType == logicalType ) ) {
				found =1;
				return tmpVoiceIvrCh_p;
			}
			tmpVoiceIvrCh_p = tmpVoiceIvrCh_p->next;
		}
	}
	IVR_FSM_DBG("findVoiceIvrCh() return NULL");
	return NULL;
}

/*______________________________________________________________________________
**	ivrPlayVoiceStart
**	descriptions:
**	parameters: 	timeInterval=0 means one time play,  
**				timeInterval!=0 means periodically play.
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int 
ivrCmdPlayStart(
	voiceIvrCh_t *tmpVoiceIvrCh_p,
	uint16 ivrIndex,
	uint16 ivrType, 
	uint32 timeInterval
){
		
	phoneCh_t *phoneCB=NULL;
	int err = 0;
	
	IVR_FSM_DBG2("Enter ivrCmdPlayStart, ivrType: ", ivrType, " ,ivrIndex: ", ivrIndex);
	
	phoneCB = find_phoneCB(tmpVoiceIvrCh_p->logicalId, 0);
	if(TYPE_IVR_SYS_DEFAULT == ivrType){
		ivrIndex += IVRSYS_OFFSET;
	}
	err = vDspIVRFunc(phoneCB, 1, 2, IVR_FUNC_ACTION_START, ivrIndex);
	return err;

}

/*______________________________________________________________________________
**	ivrCmdStartRecord
**	descriptions: Prepare to start record voice. allocate one space for voice element
**	parameters:
**	local:
**	global:
**	return: TRUE: success. FALSE: failed
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int 
ivrCmdStartRecord(
	voiceIvrCh_t *tmpVoiceIvrCh_p, 
	uint16 ivrIndex
){
	phoneCh_t *phoneCB=NULL;
	int err = 0;
	phoneCB = find_phoneCB(tmpVoiceIvrCh_p->logicalId, 0);
	tmpVoiceIvrCh_p->ivrIndex = ivrIndex;
	err = vDspIVRFunc(phoneCB, 1, 1, IVR_FUNC_ACTION_START, ivrIndex);
	return err;
}

/*______________________________________________________________________________
**	ivrCmdStopRecord
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return: TRUE:success ; FALSE :failed.
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

void 
ivrCmdStopRecord(
	voiceIvrCh_t *tmpVoiceIvrCh_p
){
	phoneCh_t *phoneCB=NULL;
	int err = 0;
	phoneCB = find_phoneCB(tmpVoiceIvrCh_p->logicalId, 0);
	err = vDspIVRFunc(phoneCB, 1, 1, IVR_FUNC_ACTION_STOP_SAVE, tmpVoiceIvrCh_p->ivrIndex); 
	return;
}

/*______________________________________________________________________________
**	ivrCmdHaltRecord
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void 
ivrCmdHaltRecord(
	voiceIvrCh_t *tmpVoiceIvrCh_p
){
	
	phoneCh_t *phoneCB=NULL;
	int err = 0;
	phoneCB = find_phoneCB(tmpVoiceIvrCh_p->logicalId, 0);
	err = vDspIVRFunc(phoneCB, 1, 1, IVR_FUNC_ACTION_STOP, tmpVoiceIvrCh_p->ivrIndex);
	
	return;
}

/*______________________________________________________________________________
**	ivrCmdDelRecord
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
ivrCmdDelRecord(
	uint16 index
){	
	int err = 0;	
	err = vDspIVRFunc(NULL, 1, 3, 0, index);
	return err;
}

/*______________________________________________________________________________
**	ivrCmdPlayVoiceByKeyPress
**	descriptions:
**	parameters: 	index start from 0
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int 
ivrCmdPlayVoiceByKeyPress(
	voiceIvrCh_t *tmpVoiceIvrCh_p,
	uint16 index
) {
	uint16 ret= 0;	
	phoneCh_t *phoneCB=NULL;
	phoneCB = find_phoneCB(tmpVoiceIvrCh_p->logicalId, 0);
	ret = vDspIVRFunc(phoneCB, 1, 2, IVR_FUNC_ACTION_START, index); /*02XX,  play item XX*/
	return ret;
}  /*ivrCmdPlayVoiceByKeyPress*/

