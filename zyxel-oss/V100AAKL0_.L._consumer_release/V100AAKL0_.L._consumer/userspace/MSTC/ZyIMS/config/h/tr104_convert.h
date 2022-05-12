/*
   $Id: tr104_convert.h 0.00.0.1 2008/11/12 14:51 ctpan Exp $
*/
/************************************************************************
 *
 *	Copyright (C) 2008 ZyXEL Communications, Corp.
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
 * $Log: tr104_convert.h $
*/

#ifndef _TR104CONVERT_H_INCLUDE
#define	_TR104CONVERT_H_INCLUDE


int tr104ConvertEnable(char * str);
int tr104ConvertPassthrough(char * str);
int tr104ConvertTransport(char * str);
int tr104ConvertDTMFMethod(char * str);
int tr104ConvertInBoundAuth(char* str);
int tr104ConvertTCFMethod(char* str);
int tr104ConvertLineStatus(char* str);
int tr104ConvertCallState(char* str);
int tr104ConvertRegType(char* str);
int tr104ConvertURLType(char* str);
int tr104ConvertMixMode(char* str);
int tr104ConvertRemoteRingback(char* str);
int tr104ConvertDialMethod(char* str);
int tr104ConvertCallState(char* str);
int tr104ConvertCWStatus(char* str);
int tr104ConvertConfCallStatus(char* str);
int tr104ConvertPhyIntfType(char* str);
int tr104ConvertPortsType(char* str);
int tr104ConvertDialingType(char* str);
int tr104ConvertDialToneType(char* str);
int tr104ConvertReceiveToneType(char* str);
int tr104ConvertCallerIDType(char* str);
int tr104ConvertCallerIDPayloadType(char* str);
int tr104ConvertTASType(char* str);
int tr104ConvertPhoneBookFlag(char* str);
int tr104ConvertPhoneBookType(char* str);
int tr104ConvertLoggingLevel(char* str);
int tr104ConvertCodecEntryID(int entryId );
int  tr104ConvertDialPlan(char* result  , char* original );
#endif

