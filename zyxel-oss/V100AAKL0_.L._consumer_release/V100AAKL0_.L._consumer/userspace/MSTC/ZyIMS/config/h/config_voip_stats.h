/*
   $Id: config_voip_stats.h 0.00.0.1 2008/11/12 14:51 ctpan Exp $
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
 * $Log: config_voip_stats.h $
*/

#ifndef _CONFIG_VoIP_STATS_H_INCLUDE
#define	_CONFIG_VoIP_STATS_H_INCLUDE

#include "phfsm.h"

typedef struct Config_VoIP_Stats_st{
	uint32 RingVolumeStatus;
	uint8 RingMuteStatus;
	char Status[33];
	/*
	"Up", "Initializing", "Registering", "Unregistering", "Error", "Testing", "Quiescent", "Disabled"
	*/
	char CallState[33];
	/*
	"Idle", "Calling", "Ringing", "Connecting", "InCall", "Hold", "Disconnecting"
	*/
	char CallWaitingStatus[257];
	/*
	"Disabled", "Idle", "SecondaryRinging", "SecondaryConnecting", "SecondaryConnected"
	*/
	char TransmitCodec[65];
	char ReceiveCodec[65];
	uint32 TransmitBitRate;
	uint32 ReceiveBitRate;
	uint8 TransmitSilenceSuppression;
	uint8 ReceiveSilenceSuppression;
	uint32 TransmitPacketizationPeriod;
	char SessionStartTime[26];
	uint32 SessionDuration;
	char FarEndIPAddress[33];
	uint32 FarEndUDPPort;
	uint32 LocalUDPPort;
	uint32 PacketsSent;
	uint32 PacketsReceived;
	uint32 BytesSent;
	uint32 BytesReceived;
	uint32 PacketsLost;
	uint32 Overruns;
	uint32 Underruns;
	uint32 IncomingCallsReceived;
	uint32 IncomingCallsAnswered;
	uint32 IncomingCallsConnected;
	uint32 IncomingCallsFailed;
	uint32 OutgoingCallsAttempted;
	uint32 OutgoingCallsAnswered;
	uint32 OutgoingCallsConnected;
	uint32 OutgoingCallsFailed;
	uint32 CallsDropped;
	uint32 TotalCallTime;
	uint32 ServerDownTime;
	uint32 ReceivePacketLossRate;
	uint32 FarEndPacketLossRate;
	uint32 ReceiveInterarrivalJitter;
	uint32 FarEndInterarrivalJitter;
	uint32 RoundTripDelay;
	uint32 AverageReceiveInterarrivalJitter;
	uint32 AverageFarEndInterarrivalJitter;
	uint32 AverageRoundTripDelay;
	char X_ZyXEL_LastRegistration[65];
		/*
		Unregistered, Registering, Registered, Deregistered, Register-Fail
		*/
	char X_ZyXEL_LastIncommingNumber[65];
	char X_ZyXEL_LastOutgoingNumber[65];
	char X_ZyXEL_PeerNumber[65];
}Config_VoIP_Stats_t;



#endif /* _CONFIG_VoIP_STATS_H_INCLUDE */
