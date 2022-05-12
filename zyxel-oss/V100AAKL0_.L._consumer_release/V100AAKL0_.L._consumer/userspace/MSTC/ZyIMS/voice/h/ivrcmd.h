/*
   $Id: Ivr.h 1.7.1.1 2007/05/14 10:05:50 mrchung Exp $
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
** $Log: Ivr.h $
** Revision 1.7.1.1  2007/05/14 10:05:50  mrchung
** Add more features of VoIP following the P2602HWNLI-D7A.
** Revision 1.7  2006/07/27 05:43:20  AlexSo
** Make it easier to read if IVRCheckCodec return a false value
** Revision 1.6  2006/07/27 05:10:37  AlexSo
** Default early media ringtone fixed
** Revision 1.5  2006/07/25 09:30:47  AlexSo
** Default early media ringtone
** Revision 1.4  2006/07/19 13:21:40  tommyhsu
** Can't hear IVRPRON_ENTERPASWWORD when enable Trucking 
** Revision 1.3  2006/04/19 06:43:55  Eric_Chen
** Replace FSM and hardware depend related files to phase 2
** Revision 1.11  2006/04/11 11:04:07  ohsieh
** Revision 1.10  2006/03/20 08:56:57  Weicheng
** Fix trunking bug when IVR interval too long
** Revision 1.9  2006/02/08 07:31:20  Weicheng
** Add ringback IVR and password IVR for Trunking 
** Revision 1.8  2005/12/30 12:38:00  Rudi
** Change "NUM_IVRUSR" to "NUM_IVR_UPGRADE"
** Add Compiler Flag "N0_IVRUSR_PATH"
** Revision 1.7  2005/11/24 03:13:49  khung
** add phone config feature
** Revision 1.6  2005/06/30 02:33:17  ohsieh
** Add G723 Support
** Revision 1.5  2005/04/07 08:10:30  ccshen
** Add crc parameter.
** Revision 1.4  2005/03/03 13:59:03  cwchen
** Enhance the IVR to bundle the voipIvrCallBack() with the IVR channel.
** Revision 1.3  2005/02/01 11:34:24  cwchen
** Support EMBEDDED_IVR and enhance the IVR mechanism.
** Revision 1.2  2005/01/10 07:13:48  cwchen
** Enhance the configuration for the language type of the IVR voice
** Revision 1.1  2004/12/15 06:53:50  cwchen
** Initial revision
** Revision 1.2  2004/10/18 08:32:53  yjlee
** Support early media.
** Revision 1.1  2004/10/11 13:42:21  ccshen
** Initial revision
*/
#ifndef _IVR_CMD_H
#define _IVR_CMD_H

#include "ivr_common.h"

typedef struct {
	uint32 	index;
	char 	country[4];
} voiceCountIndex_t;


typedef struct voiceIvrCh_s{
	struct voiceIvrCh_s *prev;
	struct voiceIvrCh_s *next;
	uint16 	logicalId;
	uint16	logicalType;
	uint16	ivrIndex;
	uint16	ivrType;
	uint16	func;
	uint16	reserv;
} voiceIvrCh_t;

typedef struct {
	voiceIvrCh_t *voiceIvrCh_p;
	uint16	ivrNumbers;
} voiceIvrCb_t;

extern voiceIvrCh_t *voiceIvrChCb;



#endif

