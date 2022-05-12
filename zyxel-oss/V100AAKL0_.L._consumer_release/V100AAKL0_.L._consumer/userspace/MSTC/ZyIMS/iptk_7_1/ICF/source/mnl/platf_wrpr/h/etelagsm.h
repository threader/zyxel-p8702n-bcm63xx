/* ETELAGSM.H*/
/**/
/* Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.*/
/**/
/* ETel Advanced Gsm API*/

#if !defined(__ETELAGSM_H__)
#define __ETELAGSM_H__

#if !defined(__ETEL_H__)
#include <ETEL.h>
#endif

#if !defined(__BASICGSM_H__)
#include "ETelBGsm.h"
#endif

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif


const TInt KETelExtAdvancedGsm					=2000;
const TInt KETelMixinAdvPhoneOwnNumber			=KETelExtAdvancedGsm+1;
const TInt KETelMixinAdvPhoneNetwork			=KETelExtAdvancedGsm+2;
const TInt KETelMixinAdvPhoneSecurity			=KETelExtAdvancedGsm+3;
const TInt KETelMixinAdvPhoneAoc				=KETelExtAdvancedGsm+4;
const TInt KETelMixinAdvPhoneFdn				=KETelExtAdvancedGsm+5;
const TInt KETelMixinAdvPhoneAirTimeDuration	=KETelExtAdvancedGsm+6;
const TInt KETelMixinAdvPhoneCallForwarding		=KETelExtAdvancedGsm+7;
const TInt KETelMixinAdvPhoneSuppService		=KETelExtAdvancedGsm+8;
const TInt KETelMixinAdvPhoneLineIdentity		=KETelExtAdvancedGsm+9;
const TInt KETelMixinAdvPhoneCaps				=KETelExtAdvancedGsm+10;
const TInt KETelMixinAdvLineStatus				=KETelExtAdvancedGsm+11;
const TInt KETelMixinAdvLineBarring				=KETelExtAdvancedGsm+12;
const TInt KETelMixinAdvLineForwarding			=KETelExtAdvancedGsm+13;
const TInt KETelMixinAdvLineCallWaitingMode		=KETelExtAdvancedGsm+14;
const TInt KETelMixinAdvLineAutoRetry			=KETelExtAdvancedGsm+15;
const TInt KETelMixinAdvCallControl				=KETelExtAdvancedGsm+16;
const TInt KETelMixinAdvCallAoC					=KETelExtAdvancedGsm+17;
const TInt KETelMixinAdvCallCCBS				=KETelExtAdvancedGsm+18;
const TInt KETelMixinAdvCallAlternatingMode		=KETelExtAdvancedGsm+19;
const TInt KETelMixinAdvCallEmergency			=KETelExtAdvancedGsm+20;
const TInt KEtelMixinAdvPhoneCallBarring		=KETelExtAdvancedGsm+21;
const TInt KEtelMixinAdvPhoneCallWaiting		=KETelExtAdvancedGsm+22;
const TInt KEtelMixinAdvCallInformation			=KETelExtAdvancedGsm+23;
const TInt KETelMixinAdvPhoneDTMF				=KETelExtAdvancedGsm+24;
const TInt KETelMixinAdvPhoneNitz				=KETelExtAdvancedGsm+25;
const TInt KETelMixinAdvPhoneAltLineService		=KETelExtAdvancedGsm+26;
const TInt KETelMixinAdvPhoneCCBS				=KETelExtAdvancedGsm+27;
const TInt KETelMixinAdvPhoneAltCall			=KETelExtAdvancedGsm+28;
const TInt KETelMixinAdvPhoneLanguage			=KETelExtAdvancedGsm+29;

/**/
/* Call Groups*/
/**/
/* Note:*/
/* Requests made on the RCall objects themselves action that call individually, so they map onto a combination of splitting the call*/
/* from the group they occupy and transfer only that call.  Actions placed on the "Call Group" class effect all calls in the relevant group.*/
/**/
class CAdvPtrHolder;
class RGsmCallGroup : public RTelSubSessionBase
	{
public:

	IMPORT_C RGsmCallGroup();

	enum TCallGroup
		{
		ECallGroupMain,
		ECallGroupHold,
		ECallGroupWaiting
		};
	
	IMPORT_C TInt Open(RPhone& aPhone,TCallGroup aGroup);
	IMPORT_C void Close();

/**/
/* Enumeration Calls*/
/**/
	IMPORT_C TInt EnumerateCalls(TInt& aCalls) const;
	IMPORT_C TInt GetCallInfo(TInt aIndex, TName& aCallName) const;

/* Actions*/
/**/
/**/
/* Hang Up the "Main" Call Group*/
/**/
	IMPORT_C TInt HangUp() const;
	IMPORT_C void HangUp(TRequestStatus& aReqStatus) const;
	IMPORT_C void HangUpCancel() const;

/**/
/* Swap - Swap whole "Hold" call group with "Main" call group.*/
/**/
	IMPORT_C TInt Swap() const;
	IMPORT_C void Swap(TRequestStatus& aReqStatus) const;
	IMPORT_C void SwapCancel() const;

/**/
/* Join Call to the Main Call Group*/
/**/
	IMPORT_C TInt Join() const;
	IMPORT_C void Join(TRequestStatus& aReqStatus) const;
	IMPORT_C void JoinCancel() const;

/**/
/* Answer and Hold*/
/**/
	IMPORT_C TInt PutOnHoldAndAnswer() const;
	IMPORT_C void PutOnHoldAndAnswer(TRequestStatus& aReqStatus) const;
	IMPORT_C void PutOnHoldAndAnswerCancel() const;

/**/
/* Answer and Hang Up*/
/**/
	IMPORT_C TInt HangUpAndAnswer() const;
	IMPORT_C void HangUpAndAnswer(TRequestStatus& aReqStatus) const;
	IMPORT_C void HangUpAndAnswerCancel() const;

/**/
/* Notifications*/
/**/
	enum TCallGroupingChangeAction
		{
		ECallGroupingChangeAdd,
		ECallGroupingChangeRemove,
		ECallGroupingChangeTerminated,
		ECallGroupingChangeAllCallsTerminated
		};

	struct TCallGroupingChange
		{
		TCallGroupingChangeAction iAction;
		TName iCallName;
		};

	IMPORT_C void NotifyCallGroupingChange(TRequestStatus& aReqStatus, TCallGroupingChange& aCallGroupingChange) const;
	IMPORT_C void NotifyCallGroupingChangeCancel() const;

/**/
/* Call Group Dynamic Capability Information*/
/**/
	enum TCallGroupCaps
		{
		KCapsCallGroupHangUp=0x00000001,
		KCapsCallGroupSwap=0x00000002,
		KCapsCallGroupJoin=0x00000004,
		KCapsCallGroupHoldAndAnswer=0x00000008,
		KCapsCallGroupHangUpAndAnswer=0x00000010
		};

	struct TCaps
		{
		TUint iCallGroupCaps;
		};

	IMPORT_C TInt GetCallGroupCaps(TCaps& aCaps) const; 
	IMPORT_C void NotifyCallGroupCaps(TRequestStatus& aReqStatus, TCaps& aCaps) const;
	IMPORT_C void NotifyCallGroupCapsCancel() const;

private:
	CAdvPtrHolder* iAdvPtrHolder;
protected:
	IMPORT_C virtual void ConstructL();
	IMPORT_C virtual void Destruct();
	};

/**/
/* Call control*/
/**/

class MAdvGsmCallControl
	{
public:
/**/
/* Call Status*/
/**/
	enum TGsmStatus
		{
		EGsmStatusUnknown,
		EGsmStatusIdle,
		EGsmStatusDialing,
		EGsmStatusRinging,
		EGsmStatusAnswering,
		EGsmStatusConnecting,
		EGsmStatusConnected,
		EGsmStatusDisconnecting,
		EGsmStatusHold,
		EGsmStatusWaitingCcbsRecall,
		EGsmStatusRingingCcbsRecall,
		EGsmStatusWaitingCallModeSwitch
		};

	virtual TInt GetGsmStatus(TGsmStatus& aInfo) const=0;
	virtual void NotifyGsmStatusChange(TRequestStatus& aReqStatus, TGsmStatus& aInfo) const=0;
	virtual void NotifyGsmStatusChangeCancel() const=0;

/**/
/* Call Control Events*/
/**/
	enum TCallEvent
		{
		ELocalHold,
		ELocalResume,
		ELocalDeflectCall,
		ERemoteHold,
		ERemoteResume,
		ERemoteTerminated,
		ENoCallEvent
		};

	virtual void EventNotification(TRequestStatus& aReqStatus, TCallEvent& aEvent) const=0;
	virtual void EventNotificationCancel() const=0;

	virtual TInt Hold() const=0;
	virtual void Hold(TRequestStatus& aReqStatus) const=0;
	virtual void HoldCancel() const=0;
	virtual TInt ToggleHold() const=0;
	virtual void ToggleHold(TRequestStatus& aReqStatus) const=0;
	virtual void ToggleHoldCancel() const=0;
	virtual TInt Resume() const=0;
	virtual void Resume(TRequestStatus& aReqStatus) const=0;
	virtual void ResumeCancel() const=0;
	virtual TInt DeflectCall(const TGsmTelNumber& aNumber) const=0;
	virtual void DeflectCall(TRequestStatus& aReqStatus, const TGsmTelNumber& aNumber) const=0;
	virtual void DeflectCallCancel() const=0;
	virtual TInt GoOneToOne() const=0;
	virtual void GoOneToOne(TRequestStatus& aReqStatus) const=0;
	virtual void GoOneToOneCancel() const=0;
	virtual TInt ExplicitCallTransfer() const=0;
	virtual void ExplicitCallTransfer(TRequestStatus& aReqStatus) const=0;
	virtual void ExplicitCallTransferCancel() const=0;

/**/
/* Dynamic Call Control Capability Information*/
/**/
	enum TCallControlCaps
		{
		KCapsHoldSupported=0x00001000,		/* offset added so that core & */
		KCapsResumeSupported=0x00002000,	/* adv call control caps can be */
		KCapsDeflectSupported=0x00004000,	/* returned in one TUInt*/
		KCapsOneToOneSupported=0x00008000,
		KCapsExplicitTransferSupported=0x00010000,
		KCapsToggleSupported=0x00020000,
		KCapsActivateCCBSSupported=0x00040000,
		KCapsDeactivateCCBSSupported=0x00080000
		};

	enum TCallEventCaps
		{
		KCapsLocalHoldSupported=0x00000001,
		KCapsLocalResumeSupported=0x00000002,
		KCapsLocalDeflectCallSupported=0x00000004,
		KCapsLocalExplicitTransferSupported=0x00000008,
		KCapsRemoteHoldSupported=0x00010000,
		KCapsRemoteResumeSupported=0x00020000,
		KCapsRemoteTerminatedSupported=0x00040000
		};

	struct TCallControlCapsFlags
		{
		TUint iCallControlCaps;
		TUint iCallEventCaps;
		};

	virtual TInt GetGsmCallControlCaps(TCallControlCapsFlags& aCaps) const=0;
	virtual void NotifyGsmCallControlCaps(TRequestStatus& aReqStatus, TCallControlCapsFlags& aCaps) const=0;
	virtual void NotifyGsmCallControlCapsCancel() const=0;
	};

/**/
/* Get information about the call*/
/**/
class MAdvGsmCallInformation
	{
public:
	enum {KMaxDTMFSize = 50};
	enum {KMaxSSPrefixStringSize = 10};

	/* Call information capabilities - can be a combination of static ME caps*/
	/* and dynamic call info*/
	enum TCallInformationCaps
		{
		KCapsStartTimeSupported=0x00000001,
		KCapsRemotePartyInfoSupported=0x00000002,
		KCapsDialledNumberInfoSupported=0x00000004,
		KCapsCallGroupInfoSupported=0x00000008,
		KCapsLastExitCodeSupported=0x00000010,
		KCapsSSPrefixSupported=0x00000020,
		KCapsCallIdSupported=0x00000040,
		KCapsDTMFSupported=0x00000080
		};

	struct TCallInformationCapsFlags
		{
		TUint iCallInformationCaps;
		};

	enum TCallGroupBelongedTo
		{
		ECallInMainGroup,
		ECallInHoldGroup,
		ECallInWaitingGroup,
		ECallNotInGroup
		};

	enum TDirection
		{
		EMobileOriginated,
		EMobileTerminated
		};

	enum TRemotePartyIdentityStatus
		{
		ERemotePartyIdentityUnknown,
		ERemotePartyIdentityAvailable,
		ERemotePartyIdentitySuppressed
		};

	struct TRemotePartyInfo
		{
		TRemotePartyIdentityStatus iRemoteIdentityStatus;
		TBuf<RGsmPhoneBook::KPhoneBookEntrySize> iName;
		TGsmTelNumber iNumber;
		TDirection iDirection;
		};

	struct TDialledNumberInfo
		{
		TBuf<RGsmPhoneBook::KPhoneBookEntrySize> iName;
		TGsmTelNumber iNumber;
		};

	enum TEncryptionStatus
		{
		EEncrypted,
		EUnencrypted,
		EUnknown
		};

	virtual TInt GetCallId(TInt& aId) const=0;
	virtual void GetCallId(TRequestStatus& aReqStatus, TInt& aId) const=0;
	virtual void GetCallIdCancel() const=0;
	virtual void NotifyCallIdChanged(TRequestStatus& aReqStatus, TInt& aId) const=0;
	virtual void NotifyCallIdChangedCancel() const=0;

	virtual TInt GetCallStartTime(TGsmDateTime& aStartTime) const=0;
	virtual void GetCallStartTime(TRequestStatus& aReqStatus, TGsmDateTime& aStartTime) const=0;
	virtual void GetCallStartTimeCancel() const=0;

	virtual TInt GetRemotePartyInfo(TRemotePartyInfo& aRemotePartyInfo) const=0;
	virtual void GetRemotePartyInfo(TRequestStatus& aReqStatus, TRemotePartyInfo& aRemotePartyInfo) const=0;
	virtual void GetRemotePartyInfoCancel() const=0;
	virtual void NotifyRemotePartyInfoChanged(TRequestStatus& aReqStatus, TRemotePartyInfo& aRemotePartyInfo) const=0;
	virtual void NotifyRemotePartyInfoChangedCancel() const=0;

	virtual TInt GetDialledNumberInfo(TDialledNumberInfo& aDialledInfo) const=0;
	virtual void GetDialledNumberInfo(TRequestStatus& aReqStatus, TDialledNumberInfo& aDialledInfo) const=0;
	virtual void GetDialledNumberInfoCancel() const=0;

	virtual TInt GetCallGroup(TCallGroupBelongedTo& aCallGroup) const=0;
	virtual void GetCallGroup(TRequestStatus& aReqStatus, TCallGroupBelongedTo& aCallGroup) const=0;
	virtual void GetCallGroupCancel() const=0;
	virtual void NotifyCallGroupChanged(TRequestStatus& aReqStatus, TCallGroupBelongedTo& aCallGroup) const=0;
	virtual void NotifyCallGroupChangedCancel() const=0;

	virtual TInt GetLastExitCode(TInt& aLastExitCode) const=0;
	virtual void GetLastExitCode(TRequestStatus& aReqStatus, TInt& aLastExitCode) const=0;
	virtual void GetLastExitCodeCancel() const=0;

	virtual TInt GetSSStringPrefix(TDes& aSSString) const=0;
	virtual void GetSSStringPrefix(TRequestStatus& aReqStatus, TDes& aSSString) const=0;
	virtual void GetSSStringPrefixCancel() const=0;

	virtual TInt GetDTMFInDialString(TDes& aDTMFString) const=0;
	virtual void GetDTMFInDialString(TRequestStatus& aReqStatus, TDes& aDTMFString) const=0;
	virtual void GetDTMFInDialStringCancel() const=0;

	virtual TInt GetEmergencyCallFlag(TBool& aEmergencyCallFlag) const=0;
	virtual TInt GetAutoRetryCallFlag(TBool& aAutoRetryCallFlag) const=0;
	virtual TInt GetForwardedCallFlag(TBool& aForwardedCallFlag) const=0;
	virtual TInt GetCcbsActiveFlag(TBool& aCcbsActive) const=0;

	virtual TInt GetCallInformationCaps(TCallInformationCapsFlags& aCaps) const=0;

	virtual TInt GetEncryptionStatus(TEncryptionStatus& aStatus) const=0;
	virtual void GetEncryptionStatus(TRequestStatus& aReqStatus, TEncryptionStatus& aStatus) const=0;
	virtual void GetEncryptionStatusCancel() const=0;
	virtual void NotifyEncryptionStatusChanged(TRequestStatus& aReqStatus, TEncryptionStatus& aStatus) const=0;
	virtual void NotifyEncryptionStatusChangedCancel() const=0;
	};

/**/
/* Advice of Charge (AoC)*/
/**/
class MAdvGsmCallAoC
	{
public:

	struct TCallMeter
		{
		TUint iCallMeterUnits;
		TReal iCallMeter;
		};

	/* Returns just the cost of this particular call, adjusted to chosen currency*/
	/* using the current PUCT setting*/
	virtual TInt GetCallMeter(TCallMeter& aCost) const=0;
	virtual void GetCallMeter(TRequestStatus& status, TCallMeter& aCost) const=0; 
	virtual void GetCallMeterCancel() const=0;
 
	/* Notification for increasing call cost.*/
	virtual void NotifyCallMeterChanged(TRequestStatus& status, TCallMeter& aCost) const=0;
	virtual void NotifyCallMeterChangedCancel() const=0;

	};

/**/
/* Completion of Calls to Busy Subscriber*/
/**/
class MAdvGsmCallCCBS
	{
public:
	virtual TInt ActivateCCBS() const=0;
	virtual void ActivateCCBS(TRequestStatus& aReqStatus) const=0;
	virtual void ActivateCCBSCancel() const=0;

	virtual TInt RejectCCBS() const=0;
	virtual void RejectCCBS(TRequestStatus& aReqStatus) const=0;
	virtual void RejectCCBSCancel() const=0;

	virtual TInt DeactivateCCBS() const=0;
	virtual void DeactivateCCBS(TRequestStatus& aReqStatus) const=0;
	virtual void DeactivateCCBSCancel() const=0;

	virtual TInt AcceptCCBSRecall() const=0;
	virtual void AcceptCCBSRecall(TRequestStatus& aReqStatus) const=0;
	virtual void AcceptCCBSRecallCancel() const=0;

	};

/**/
/* Alternating Call Mode*/
/**/
class MAdvGsmCallAlternatingCallMode
	{
public:

	enum TAltCallMode
		{
		EAltCallModeSingle,
		EAltCallModeVoiceFax,
		EAltCallModeVoiceData,
		EAltCallModeVoiceThenData
		};

	virtual TInt SetAlternatingCallMode(const TAltCallMode aMode) const=0;
	virtual TInt GetAlternatingCallMode(TAltCallMode& aMode) const=0;
	virtual void NotifyAlternatingCallModeChanged(TRequestStatus& aReqStatus, TAltCallMode& aAltCallMode) const=0;
	virtual void NotifyAlternatingCallModeChangedCancel() const=0;

	virtual void WaitForCallModeSwitch(TRequestStatus& aReqStatus) const=0;
	virtual void WaitForCallModeSwitchCancel() const=0;

	virtual TInt SwitchCallMode() const=0;
	virtual void SwitchCallMode(TRequestStatus& aReqStatus) const=0;
	virtual void SwitchCallModeCancel() const=0;

	virtual void NotifyRemoteCallModeSwitch(TRequestStatus& aReqStatus) const=0;
	virtual void NotifyRemoteCallModeSwitchCancel() const=0;
	};

/**/
/* Emergency call*/
/**/
class MAdvGsmCallEmergencyCall
	{
public:
	virtual TInt MakeEmergencyCall() const=0;
	virtual void MakeEmergencyCall(TRequestStatus& aReqStatus) const=0;
	virtual void MakeEmergencyCallCancel() const=0;
	};


/**/
/**/
/* RAdvGsmCall*/
/**/
/**/

class CAdvCallPtrHolder;
class RAdvGsmCall : public RBasicGsmCall, public MAdvGsmCallControl, public MAdvGsmCallInformation, 
					public MAdvGsmCallAoC, public MAdvGsmCallCCBS, 
					public MAdvGsmCallAlternatingCallMode, public MAdvGsmCallEmergencyCall
	{
public:
	IMPORT_C RAdvGsmCall();

/**/
/* Gsm Call Parameters*/
/**/

	struct TTempCug
		{
		TBool	iExplicitInvoke;
		TInt	iCugIndex;
		TBool	iSuppressOA;
		TBool	iSuppressPrefCug;
		};

	enum TTempClir
		{
		EDefault,
		ESendCLI,
		EDontSendCLI
		};
	
	class TCallParams : public RBasicGsmCall::TCallParams
		{
	public:
		IMPORT_C	TCallParams();
		IMPORT_C	TCallParams(const TTempCug& aTempCug, const TTempClir aTempClir);
		TTempCug	iTempCug;
		TTempClir	iTempClir;
		};

	typedef TPckg<TCallParams> TCallParamsPckg;
/**/
/*	General notification information*/
/**/
	
	class TCallInfo : public RBasicGsmCall::TCallInfo
		{
	public:
		TInt iCallId;
		TGsmStatus iGsmStatus;
		TCallEvent iCallEvent;
		TCallGroupBelongedTo iCallGroup;
		TAltCallMode iAltCallMode;
		TBool iEmergencyCall;
		TBool iAutoRetryCall;
		TBool iForwardedCall;
		TInt iLastExitCode;
		TGsmDateTime iCallStartTime;
		TRemotePartyInfo iRemotePartyInfo;
		TDialledNumberInfo iDialledInfo;
		TBuf<KMaxDTMFSize> iDTMF;
		TBuf<KMaxSSPrefixStringSize> iSSString;
		TCallMeter iCallMeter;
		TBool iCcbsActive;
	public:
		IMPORT_C TCallInfo();
		};

	/* Get a snapshot of all the current call information*/
	IMPORT_C TInt GetInfoAdvGsm(TCallInfo& aCallInfo) const;
	IMPORT_C void GetInfoAdvGsm(TRequestStatus& aReqStatus, TCallInfo& aCallInfo) const;
	IMPORT_C void GetInfoAdvGsmCancel() const;

	/* Individual notifications*/
	IMPORT_C TInt GetGsmStatus(TGsmStatus& aCallStatus) const;
	IMPORT_C void NotifyGsmStatusChange(TRequestStatus& aReqStatus, TGsmStatus& aCallStatus) const;
	IMPORT_C void NotifyGsmStatusChangeCancel() const;

	IMPORT_C void EventNotification(TRequestStatus& aReqStatus, TCallEvent& aEvent) const;
	IMPORT_C void EventNotificationCancel() const;

	IMPORT_C TInt Hold() const;
	IMPORT_C void Hold(TRequestStatus& aReqStatus) const;
	IMPORT_C void HoldCancel() const;
	IMPORT_C TInt ToggleHold() const;
	IMPORT_C void ToggleHold(TRequestStatus& aReqStatus) const;
	IMPORT_C void ToggleHoldCancel() const;
	IMPORT_C TInt Resume() const;
	IMPORT_C void Resume(TRequestStatus& aReqStatus) const;
	IMPORT_C void ResumeCancel() const;
	IMPORT_C TInt DeflectCall(const TGsmTelNumber& aNumber) const;
	IMPORT_C void DeflectCall(TRequestStatus& aReqStatus, const TGsmTelNumber& aNumber) const;
	IMPORT_C void DeflectCallCancel() const;
	IMPORT_C TInt GoOneToOne() const;
	IMPORT_C void GoOneToOne(TRequestStatus& aReqStatus) const;
	IMPORT_C void GoOneToOneCancel() const;
	IMPORT_C TInt ExplicitCallTransfer() const;
	IMPORT_C void ExplicitCallTransfer(TRequestStatus& aReqStatus) const;
	IMPORT_C void ExplicitCallTransferCancel() const;
	IMPORT_C TInt GetGsmCallControlCaps(TCallControlCapsFlags& aCaps) const;
	IMPORT_C void NotifyGsmCallControlCaps(TRequestStatus& aReqStatus, TCallControlCapsFlags& aCaps) const;
	IMPORT_C void NotifyGsmCallControlCapsCancel() const;

	IMPORT_C TInt GetCallId(TInt& aId) const;
	IMPORT_C void GetCallId(TRequestStatus& aReqStatus, TInt& aId) const;
	IMPORT_C void GetCallIdCancel() const;
	IMPORT_C void NotifyCallIdChanged(TRequestStatus& aReqStatus, TInt& aId) const;
	IMPORT_C void NotifyCallIdChangedCancel() const;

	IMPORT_C TInt GetCallStartTime(TGsmDateTime& aStartTime) const;
	IMPORT_C void GetCallStartTime(TRequestStatus& aReqStatus, TGsmDateTime& aStartTime) const;
	IMPORT_C void GetCallStartTimeCancel() const;
	
	IMPORT_C TInt GetCallGroup(TCallGroupBelongedTo& aCallGroup) const;
	IMPORT_C void GetCallGroup(TRequestStatus& aReqStatus, TCallGroupBelongedTo& aCallGroup) const;
	IMPORT_C void GetCallGroupCancel() const;
	IMPORT_C void NotifyCallGroupChanged(TRequestStatus& aReqStatus, TCallGroupBelongedTo& aCallGroup) const;
	IMPORT_C void NotifyCallGroupChangedCancel() const;
	
	IMPORT_C TInt GetSSStringPrefix(TDes& aSSString) const;
	IMPORT_C void GetSSStringPrefix(TRequestStatus& aReqStatus, TDes& aSSString) const;
	IMPORT_C void GetSSStringPrefixCancel() const;

	IMPORT_C TInt GetDTMFInDialString(TDes& aDTMFString) const;
	IMPORT_C void GetDTMFInDialString(TRequestStatus& aReqStatus, TDes& aDTMFString) const;
	IMPORT_C void GetDTMFInDialStringCancel() const;

	IMPORT_C TInt GetRemotePartyInfo(TRemotePartyInfo& aRemotePartyInfo) const;
	IMPORT_C void GetRemotePartyInfo(TRequestStatus& aReqStatus, TRemotePartyInfo& aRemotePartyInfo) const;
	IMPORT_C void GetRemotePartyInfoCancel() const;
	IMPORT_C void NotifyRemotePartyInfoChanged(TRequestStatus& aReqStatus, TRemotePartyInfo& aRemotePartyInfo) const;
	IMPORT_C void NotifyRemotePartyInfoChangedCancel() const;
	
	IMPORT_C TInt GetDialledNumberInfo(TDialledNumberInfo& aDialledInfo) const;
	IMPORT_C void GetDialledNumberInfo(TRequestStatus& aReqStatus, TDialledNumberInfo& aDialledInfo) const;
	IMPORT_C void GetDialledNumberInfoCancel() const;

	IMPORT_C TInt GetLastExitCode(TInt& iLastExitCode) const;
	IMPORT_C void GetLastExitCode(TRequestStatus& aReqStatus, TInt& iLastExitCode) const;
	IMPORT_C void GetLastExitCodeCancel() const;

	IMPORT_C TInt GetEmergencyCallFlag(TBool& aEmergencyCallFlag) const;
	IMPORT_C TInt GetAutoRetryCallFlag(TBool& aAutoRetryCallFlag) const;
	IMPORT_C TInt GetForwardedCallFlag(TBool& aForwardedCallFlag) const;
	IMPORT_C TInt GetCcbsActiveFlag(TBool& aCcbsActive) const;

	IMPORT_C TInt GetCallInformationCaps(TCallInformationCapsFlags& aCaps) const;

	IMPORT_C TInt GetEncryptionStatus(TEncryptionStatus& aStatus) const;
	IMPORT_C void GetEncryptionStatus(TRequestStatus& aReqStatus, TEncryptionStatus& aStatus) const;
	IMPORT_C void GetEncryptionStatusCancel() const;
	IMPORT_C void NotifyEncryptionStatusChanged(TRequestStatus& aReqStatus, TEncryptionStatus& aStatus) const;
	IMPORT_C void NotifyEncryptionStatusChangedCancel() const;

	IMPORT_C TInt GetCallMeter(TCallMeter& aCost) const;
	IMPORT_C void GetCallMeter(TRequestStatus& status, TCallMeter& aCost) const; 
	IMPORT_C void GetCallMeterCancel() const;
	IMPORT_C void NotifyCallMeterChanged(TRequestStatus& status, TCallMeter& aCost) const;
	IMPORT_C void NotifyCallMeterChangedCancel() const;

	IMPORT_C TInt ActivateCCBS() const;
	IMPORT_C void ActivateCCBS(TRequestStatus& aReqStatus) const;
	IMPORT_C void ActivateCCBSCancel() const;
	IMPORT_C TInt RejectCCBS() const;
	IMPORT_C void RejectCCBS(TRequestStatus& aReqStatus) const;
	IMPORT_C void RejectCCBSCancel() const;
	IMPORT_C TInt DeactivateCCBS() const;
	IMPORT_C void DeactivateCCBS(TRequestStatus& aReqStatus) const;
	IMPORT_C void DeactivateCCBSCancel() const;
	IMPORT_C TInt AcceptCCBSRecall() const;
	IMPORT_C void AcceptCCBSRecall(TRequestStatus& aReqStatus) const;
	IMPORT_C void AcceptCCBSRecallCancel() const;

	IMPORT_C TInt SetAlternatingCallMode(const TAltCallMode aMode) const;
	IMPORT_C TInt GetAlternatingCallMode(TAltCallMode& aMode) const;
	IMPORT_C void NotifyAlternatingCallModeChanged(TRequestStatus& aReqStatus, TAltCallMode& aAltCallMode) const;
	IMPORT_C void NotifyAlternatingCallModeChangedCancel() const;
	IMPORT_C void WaitForCallModeSwitch(TRequestStatus& aReqStatus) const;
	IMPORT_C void WaitForCallModeSwitchCancel() const;
	IMPORT_C TInt SwitchCallMode() const;
	IMPORT_C void SwitchCallMode(TRequestStatus& aReqStatus) const;
	IMPORT_C void SwitchCallModeCancel() const;
	IMPORT_C void NotifyRemoteCallModeSwitch(TRequestStatus& aReqStatus) const;
	IMPORT_C void NotifyRemoteCallModeSwitchCancel() const;
	
	IMPORT_C TInt MakeEmergencyCall() const;
	IMPORT_C void MakeEmergencyCall(TRequestStatus& aReqStatus) const;
	IMPORT_C void MakeEmergencyCallCancel() const;
	
private:
	CAdvCallPtrHolder* iAdvPtrHolder;
protected:
	IMPORT_C virtual void ConstructL();
	IMPORT_C virtual void Destruct();
	};

/**/
/* Automatic Retry of Dialled Number*/
/**/
class MAdvGsmLineAutoRetry
	{
public:
	virtual void AutoRetry(TRequestStatus& aReqStatus, const TGsmTelNumber& aTelNumber, TName& aNewCallName)const=0;
	virtual void AutoRetryCancel() const=0;
	};

/**/
/* Advanced Line information*/
/**/
class MAdvGsmLineStatusAndCallInfo
	{
public:
	enum TAdvGsmLineStatus
		{
		EGsmStatusUnknown,
		EGsmStatusIdle,
		EGsmStatusDialing,
		EGsmStatusRinging,
		EGsmStatusAnswering,
		EGsmStatusConnecting,
		EGsmStatusConnected,
		EGsmStatusHangingUp,
		EGsmStatusHold
		};

	virtual TInt GetAdvGsmLineStatus(TAdvGsmLineStatus& aReqStatus) const=0;
	virtual void NotifyAdvGsmLineStatusChange(TRequestStatus& aReqStatus,TAdvGsmLineStatus& aInfo) const=0;
	virtual void NotifyAdvGsmLineStatusChangeCancel() const=0;

	struct TAdvGsmCallInfo
		{
		TName iCallName;
		RAdvGsmCall::TGsmStatus iGsmStatus;
		RAdvGsmCall::TCallControlCapsFlags iAdvCallCaps;
		RAdvGsmCall::TAltCallMode iAlternatingCallMode;
		};

	virtual TInt GetAdvGsmCallInfo(const TInt aIndex,TAdvGsmCallInfo& aCallInfo) const=0;

	/* Advanced line caps defined so that client can check if line is voice or voice 2*/
	/* Offset added so that core & adv line caps can be returned in one TUInt */

	enum TAdvGsmLineCaps
		{
		KCapsAuxVoice=0x00000008
		};					 	 

	virtual TInt GetAdvGsmLineCaps(RLine::TCaps& aAdvGsmLineCaps) const=0;

	struct TAdvGsmLineInfo
		{
		TName iLineName;
		TAdvGsmLineStatus iLineStatus;
		RLine::TCaps iAdvLineCaps;
		};

	};

/**/
/* Call Barring on individual lines*/
/**/
class MAdvGsmLineBarring
	{
public:
	enum TBarringMode
		{
		EBarringModeActive,			/* barring program is currently active*/
		EBarringModeNotActive,		/* barring program is provisioned but not active*/
		EBarringModeNotProvisioned,	/* barring program is not provisioned*/
		EBarringModeUnavailable,	/* barring program is not available in serving PLMN*/
		EBarringModeUnknown			/* status of barring program can not be determined*/
		};							/* It could be one of the following*/
									/* 1. Not provisioned*/
									/* 2. Provisioned but not active*/
	enum TBarringType
		{
		EBarringTypeAllOutgoingCall,
		EBarringTypeOutgoingInternational,
		EBarringTypeOutgoingExceptToHomeCountry,
		EBarringTypeAllIncomingCall,
		EBarringTypeIncomingCallWhenRoamingOutsideHomeCountry,
		EBarringTypeIncomingCallNotStoreInTAmemory,
		EBarringTypeIncomingCallNotStoreInMEmemory,
		EBarringTypeIncomingCallNotStoreInSIMmemory,
		EBarringTypeIncomingCallNotStoreInAnymemory,
		EBarringTypeAllBarringServices,
		EBarringTypeAllOutgoingServices,
		EBarringTypeAllIncomingServices
		};

	enum TBarringSetting
		{
		EBarringActivate,
		EBarringDeactivate
		};

	struct TChangeBarringMode
		{
		TBarringSetting iSetting;
		TBuf<KGsmPasswordSize> iPassword;
		};

	enum TBarringModeLocation
		{
		EBarringLocationCache,
		EBarringLocationCachePreferred,
		EBarringLocationNetwork
		};

	virtual TInt GetBarringMode(const TBarringType aType ,TBarringMode& aMode, const TBarringModeLocation aLocation = EBarringLocationCache) const=0;
	virtual void GetBarringMode(TRequestStatus& aReqStatus, const TBarringType aType, TBarringMode& aMode,const TBarringModeLocation aLocation = EBarringLocationCache) const=0;
	virtual void GetBarringModeCancel() const=0;
	virtual TInt SetBarringMode(const TBarringType aType, const TChangeBarringMode& aMode) const=0;
	virtual void SetBarringMode(TRequestStatus& aReqStatus, const TBarringType aType,const TChangeBarringMode& aMode) const=0;
	virtual void SetBarringModeCancel() const=0;
	virtual void BarringModeNotification(TRequestStatus& aReqStatus, TBarringType& aType, TBarringMode& aMode) const=0; 
	virtual void BarringModeNotificationCancel() const=0;

	/**/
	/*	Internal Client/Server struct*/
	/**/
	struct TBarringTypeAndLocation
		{
		TBarringModeLocation iLocation;
		TBarringType iType;
		};

	};

/**/
/* Call Forwarding on individual lines*/
/**/
class MAdvGsmLineForwarding
	{
public:
	enum TCallForwardingReasonGet
		{
		ECallForwardingGetUnconditional,
		ECallForwardingGetTypeBusy,
		ECallForwardingGetTypeNoReply,
		ECallForwardingGetTypeNotReachable,
		ECallForwardingGetTypeUnknown
		};

	enum TCallForwardingReasonSet
		{
		ECallForwardingSetUnconditional,
		ECallForwardingSetTypeBusy,
		ECallForwardingSetTypeNoReply,
		ECallForwardingSetTypeNotReachable,
		ECallForwardingSetTypeAllCalls,
		ECallForwardingSetTypeAllConditionalCases
		};

	enum TCallForwardingStatus
		{
		ECallForwardingStatusActive,		/* CF service is provisioned, registered and active*/
		ECallForwardingStatusNotActive,		/* CF service is provisioned, registered but not active*/
		ECallForwardingStatusNotRegistered,	/* CF service is provisioned but not registered*/
		ECallForwardingStatusNotProvisioned,/* CF serivce is not provisioned*/
		ECallForwardingStatusUnavailable,	/* CF service is not available in serving PLMN*/
		ECallForwardingStatusUnknown		/* CF service status can not be determined.*/
		};									

	enum TCallForwardingAction
		{
		ECallForwardingInterrogation, 
		ECallForwardingRegistration,
		ECallForwardingActivation,
		ECallForwardingDeactivation,
		ECallForwardingErasure
		};

	enum TCallForwardingActive
		{
		ECallForwardingUnconditionalActive,
		ECallForwardingConditionalActive
		};

	enum TForwardingInfoLocation
		{
		EForwardingLocationCache,
		EForwardingLocationCachePreferred,
		EForwardingLocationNetwork
		};
			   
	enum TCallForwardingSetting
		{
		ECallForwardingActivate, 
		ECallForwardingRegister,
		ECallForwardingDeactivate,
		ECallForwardingErase
		};

	struct TCallForwardingChange
		{
		TCallForwardingSetting iSetting;
		TGsmTelNumber iNumber;
		TInt iTimeout;						/* only applicable to CFNRy (= -1 otherwise)*/
		};

	struct TCallForwardingInfo
		{
		TCallForwardingStatus iStatus;
		TCallForwardingAction iAction;
		TGsmTelNumber iNumber;
		TInt iTimeout;						/* only applicable to CFNRy (= -1 otherwise)*/
		};

	virtual TInt GetCallForwardingInfo(const TCallForwardingReasonGet aReason, TCallForwardingInfo& aInfo,const TForwardingInfoLocation aLocation = EForwardingLocationCache) const=0;
	virtual void GetCallForwardingInfo(TRequestStatus& aReqStatus,const TCallForwardingReasonGet aReason, TCallForwardingInfo& aInfo,const TForwardingInfoLocation aLocation = EForwardingLocationCache) const=0;
	virtual void GetCallForwardingInfoCancel() const=0;
	virtual TInt SetCallForwardingStatus(const TCallForwardingReasonSet aReason,const TCallForwardingChange& aInfo) const=0;
	virtual void SetCallForwardingStatus(TRequestStatus& aReqStatus, const TCallForwardingReasonSet aReason,const TCallForwardingChange& aInfo) const=0;
	virtual void SetCallForwardingStatusCancel() const=0;

	virtual void CallForwardingStatusNotification(TRequestStatus& aReqStatus, TCallForwardingReasonGet& aReason, TCallForwardingInfo& aInfo) const=0;
	virtual void CallForwardingStatusNotificationCancel() const=0;

	virtual void CallForwardingActiveNotification(TRequestStatus& aReqStatus, TCallForwardingActive& aActiveService) const=0;
	virtual void CallForwardingActiveNotificationCancel() const=0;

	/**/
	/*	Internal Client/Server structs*/
	/**/
	struct TForwardingReasonAndLocation
		{
		TCallForwardingReasonGet iReason;
		TForwardingInfoLocation iLocation;
		};

	struct TForwardingReasonAndAction
		{
		TCallForwardingReasonGet iReason;
		TCallForwardingAction iAction;
		};

	};


/**/
/* Call Waiting on individual lines*/
/**/

class MAdvGsmLineCallWaitingMode
	{
public:
	enum TCallWaitingMode
		{
		ECallWaitingModeActive,			/* CW service is provisioned and active*/
		ECallWaitingModeNotActive,		/* CW service is provisioned but not active*/
		ECallWaitingModeNotProvisioned, /* CW service is not provisioned*/
		ECallWaitingModeUnavailable,	/* CW service is not available in serving PLMN*/
		ECallWaitingModeUnknown			/* CW service status can not be determined*/
		};								/* It could be one of the following :-*/
										/* 1. Not provisioned*/
										/* 2. Provisioned but not active*/

	enum TCallWaitingSetting
		{
		ECallWaitingActivate,
		ECallWaitingDeactivate
		};

	enum TWaitingModeLocation
		{
		EWaitingLocationCache,
		EWaitingLocationCachePreferred,
		EWaitingLocationNetwork
		};

	virtual TInt GetCallWaitingMode(TCallWaitingMode& aMode,const TWaitingModeLocation aLocation = EWaitingLocationCache) const=0;
	virtual void GetCallWaitingMode(TRequestStatus& aReqStatus,TCallWaitingMode& aMode,const TWaitingModeLocation aLocation = EWaitingLocationCache) const=0;
	virtual void GetCallWaitingModeCancel() const=0;
	virtual TInt SetCallWaitingMode(const TCallWaitingSetting aSetting) const=0;
	virtual void SetCallWaitingMode(TRequestStatus& aReqStatus, const TCallWaitingSetting aSetting) const=0;
	virtual void SetCallWaitingModeCancel() const=0;
	virtual void NotifyCallWaitingModeChange(TRequestStatus& aReqStatus, TCallWaitingMode& aMode) const=0;
	virtual void NotifyCallWaitingModeChangeCancel() const=0;

	};

/**/
/**/
/* RAdvGsmLine*/
/**/
/**/
class CAdvLinePtrHolder;
class RAdvGsmLine : public RLine, public MAdvGsmLineStatusAndCallInfo, public MAdvGsmLineBarring,
					public MAdvGsmLineForwarding, public MAdvGsmLineCallWaitingMode,
					public MAdvGsmLineAutoRetry
	{
public:
	IMPORT_C RAdvGsmLine();

/**/
/*	General notification information*/
/**/
	IMPORT_C void AutoRetry(TRequestStatus& aReqStatus, const TGsmTelNumber& aTelNumber, TName& aNewCallName) const;
	IMPORT_C void AutoRetryCancel() const;

	IMPORT_C TInt GetAdvGsmLineStatus(TAdvGsmLineStatus& aReqStatus) const;
	IMPORT_C void NotifyAdvGsmLineStatusChange(TRequestStatus& aReqStatus, TAdvGsmLineStatus& aInfo) const;
	IMPORT_C void NotifyAdvGsmLineStatusChangeCancel() const;
	IMPORT_C TInt GetAdvGsmCallInfo(const TInt aIndex, TAdvGsmCallInfo& aCallInfo) const;
	IMPORT_C TInt GetAdvGsmLineCaps(TCaps& aAdvGsmLineCaps) const;

	IMPORT_C TInt GetBarringMode(const TBarringType aType ,TBarringMode& aMode, const TBarringModeLocation aLocation = EBarringLocationCache) const;
	IMPORT_C void GetBarringMode(TRequestStatus& aReqStatus, const TBarringType aType, TBarringMode& aMode,const TBarringModeLocation aLocation = EBarringLocationCache) const;
	IMPORT_C void GetBarringModeCancel() const;
	IMPORT_C TInt SetBarringMode(const TBarringType aType,const TChangeBarringMode& aMode) const;
	IMPORT_C void SetBarringMode(TRequestStatus& aReqStatus, const TBarringType aType,const TChangeBarringMode& aMode) const;
	IMPORT_C void SetBarringModeCancel() const;
	IMPORT_C void BarringModeNotification(TRequestStatus& aReqStatus, TBarringType& aType, TBarringMode& aMode) const;
	IMPORT_C void BarringModeNotificationCancel() const;

	IMPORT_C TInt GetCallForwardingInfo(const TCallForwardingReasonGet aReason, TCallForwardingInfo& aInfo,const TForwardingInfoLocation aLocation = EForwardingLocationCache) const;
	IMPORT_C void GetCallForwardingInfo(TRequestStatus& aReqStatus,const TCallForwardingReasonGet aReason, TCallForwardingInfo& aInfo,const TForwardingInfoLocation aLocation = EForwardingLocationCache) const;
	IMPORT_C void GetCallForwardingInfoCancel() const;
	IMPORT_C TInt SetCallForwardingStatus(const TCallForwardingReasonSet aReason,const TCallForwardingChange& aInfo) const;
	IMPORT_C void SetCallForwardingStatus(TRequestStatus& aReqStatus, const TCallForwardingReasonSet aReason,const TCallForwardingChange& aInfo) const;
	IMPORT_C void SetCallForwardingStatusCancel() const;
	IMPORT_C void CallForwardingStatusNotification(TRequestStatus& aReqStatus, TCallForwardingReasonGet& aReason,TCallForwardingInfo& aInfo) const;
	IMPORT_C void CallForwardingStatusNotificationCancel() const;
	IMPORT_C void CallForwardingActiveNotification(TRequestStatus& aReqStatus, TCallForwardingActive& aActiveService) const;
	IMPORT_C void CallForwardingActiveNotificationCancel() const;

	IMPORT_C TInt GetCallWaitingMode(TCallWaitingMode& aMode,const TWaitingModeLocation aLocation = EWaitingLocationCache) const;
	IMPORT_C void GetCallWaitingMode(TRequestStatus& aReqStatus,TCallWaitingMode& aMode,const TWaitingModeLocation aLocation = EWaitingLocationCache) const;
	IMPORT_C void GetCallWaitingModeCancel() const;
	IMPORT_C TInt SetCallWaitingMode(const TCallWaitingSetting aSetting) const;
	IMPORT_C void SetCallWaitingMode(TRequestStatus& aReqStatus, const TCallWaitingSetting aSetting) const;
	IMPORT_C void SetCallWaitingModeCancel() const;
	IMPORT_C void NotifyCallWaitingModeChange(TRequestStatus& aReqStatus, TCallWaitingMode& aMode) const;
	IMPORT_C void NotifyCallWaitingModeChangeCancel() const;	

private:
	CAdvLinePtrHolder* iAdvPtrHolder;
protected:
	IMPORT_C virtual void ConstructL();
	IMPORT_C virtual void Destruct();
	};

/**/
/* Calling/Called Party identification Presentation and Restriction*/
/**/
class MAdvGsmPhoneLineIdentity
	{
public:
/**/
/* CLIP*/
/**/
	enum TClipMode
		{
		EClipModeActive,
		EClipModeNotProvisioned,
		EClipModeUnknown
		};

	virtual TInt GetClipMode(TClipMode& aMode) const=0;
	virtual void GetClipMode(TRequestStatus& aReqStatus, TClipMode& aMode) const=0;
	virtual void GetClipModeCancel() const=0;
	virtual void ClipModeNotification(TRequestStatus& aReqStatus, TClipMode& aMode) const=0;
	virtual void ClipModeNotificationCancel() const=0;
/**/
/* CLIR*/
/**/
	enum TClirMode
		{
		EClirModePermanentRestricted,
		EClirModeTemporaryDefaultRestricted,
		EClirModeTemporaryDefaultAllowed,
		EClirModeNotProvisioned,
		EClirModeUnknown
		};

	virtual TInt GetClirMode(TClirMode& aMode) const=0;
	virtual void GetClirMode(TRequestStatus& aReqStatus, TClirMode& aMode) const=0;
	virtual void GetClirModeCancel() const=0;
	virtual void ClirModeNotification(TRequestStatus& aReqStatus, TClirMode& aMode) const=0;
	virtual void ClirModeNotificationCancel() const=0;
/**/
/* COLP*/
/**/
	enum TColpMode
		{
		EColpModeActive,
		EColpModeNotProvisioned,
		EColpModeUnknown
		};

	virtual TInt GetColpMode(TColpMode& aMode) const=0;
	virtual void GetColpMode(TRequestStatus& aReqStatus, TColpMode& aMode) const=0;
	virtual void GetColpModeCancel() const=0;
	virtual void ColpModeNotification(TRequestStatus& aReqStatus, TColpMode& aMode) const=0;
	virtual void ColpModeNotificationCancel() const=0;

/**/
/* COLR*/
/**/
	enum TColrMode
		{
		EColrModeActive,
		EColrModeNotProvisioned,
		EColrModeUnknown
		};

	virtual TInt GetColrMode(TColrMode& aMode) const=0;
	virtual void GetColrMode(TRequestStatus& aReqStatus, TColrMode& aMode) const=0;
	virtual void GetColrModeCancel() const=0;
	virtual void ColrModeNotification(TRequestStatus& aReqStatus, TColrMode& aMode) const=0;
	virtual void ColrModeNotificationCancel() const=0;
	};

/**/
/* Own Number Information*/
/**/
class MAdvGsmPhoneOwnNumber
	{
public:
	enum
		{
		KSizeOfOwnNumberText=100
		};

	enum TInfoTransferCap
		{
		EITCVoice,
		EITCAuxVoice,
		EITCUDI,
		EITCThreePointOnekHz,
		EITCFax
		};

	enum TBearerService
		{
		EServiceAsynchronousModem,
		EServiceSynchronousModem,
		EServicePADAccess,
		EServicePacketAccess
		};


	enum TBearerCE
		{
		EBearerCETransparent,
		EBearerCENonTransparent
		};

	struct TOwnNumberEntry
		{
		TBuf<KSizeOfOwnNumberText> iText;
		TGsmTelNumber iTelNumber;
		TInfoTransferCap iInfoTransferCap;
		TUint iBearerSpeed;
		TBearerService iBearerService;
		TBearerCE iBearerCE;
		};

	struct TOwnNumberInfo
		{
		TUint iNumOfEntries;
		TUint iMaxNumOfEntries;
		TUint iMaxTelNumberLen;
		TUint iMaxTextLength;
		};

	virtual TInt GetOwnNumberInfo(TOwnNumberInfo& aOwnNumberInfo) const=0;
	virtual void GetOwnNumberInfo(TRequestStatus& aReqStatus, TOwnNumberInfo& aOwnNumberInfo) const=0;
	virtual void GetOwnNumberInfoCancel() const=0;

	virtual TInt GetOwnNumberEntry(const TInt aIndex, TOwnNumberEntry& aEntry) const=0;
	virtual void GetOwnNumberEntry(TRequestStatus& aReqStatus, const TInt aIndex, TOwnNumberEntry& aEntry) const=0;
	virtual void GetOwnNumberEntryCancel() const=0;
	
	virtual TInt SetOwnNumberEntry(const TInt aIndex, const TOwnNumberEntry& aEntry) const=0;
	virtual void SetOwnNumberEntry(TRequestStatus& aReqStatus, const TInt aIndex, const TOwnNumberEntry& aEntry) const=0;
	virtual void SetOwnNumberEntryCancel() const=0;

	virtual void NotifyOwnNumberEntryChanged(TRequestStatus& aReqStatus, TInt& aIndex) const=0;
	virtual void NotifyOwnNumberEntryChangedCancel() const=0;
	};

/**/
/* Network Identity & Time Zone (NITZ)*/
/* */

class MAdvGsmPhoneNitz
	{
public:
	/**/
	/* NITZ information structure*/
	/**/
	struct TNITZInfo
		{
		TBool iTimeValid;
		TDateTime iDateTime;
		TBool iTimeZoneValid;
		TInt iTimeZone;
		TBool iNetworkInfoValid;
		RBasicGsmPhone::TNetworkInfo iNetworkInfo;
		};
	
	/* Get current NITZ if available - return KErrNotFound if none exists*/
	virtual TInt GetCurrentNITZInfo(TNITZInfo& aNITZInfo) const=0;	

	/* Notification of any new NITZ info received from the network*/
	virtual void NotifyNITZInfoChange(TRequestStatus& aReqStatus, TNITZInfo& aNITZInfo) const=0;
	virtual void NotifyNITZInfoChangeCancel() const=0;
	};

/**/
/* Phone-level advanced network information*/
/**/

class CGsmNetworkList;
class MAdvGsmPhoneNetwork
	{
public:

	virtual CGsmNetworkList* RetrievePreferredNetworksLC()=0;
	virtual TInt StorePreferredNetworksL(CGsmNetworkList* aNetworkList) const=0; 
	virtual void StorePreferredNetworksL(TRequestStatus& aReqStatus, CGsmNetworkList* aNetworkList) const=0;
	virtual void StorePreferredNetworksCancel() const=0;
	virtual void NotifyPreferredNetworksChanged(TRequestStatus& aReqStatus) const=0;
	virtual void NotifyPreferredNetworksChangedCancel() const=0;

	virtual TInt GetHomeNetworkInfo(RBasicGsmPhone::TNetworkInfo& aNetworkInfo) const=0;
	virtual void GetHomeNetworkInfo(TRequestStatus& aReqStatus, RBasicGsmPhone::TNetworkInfo& aNetworkInfo) const=0;
	virtual void GetHomeNetworkInfoCancel() const=0;

	/**/
	/*	Interrogation of "Master list" of Networks stored on ME*/
	/**/

	virtual TInt EnumerateMasterListNetworks(TInt& aPhoneNetworks) const=0;
	virtual void EnumerateMasterListNetworks(TRequestStatus& aReqStatus, TInt& aPhoneNetworks) const=0;
	virtual void EnumerateMasterListNetworksCancel() const=0;

	virtual TInt GetMasterListNetworkInfo(const TInt aIndex, RBasicGsmPhone::TNetworkInfo& aNetworkInfo) const=0;
	virtual void GetMasterListNetworkInfo(TRequestStatus& aReqStatus, const TInt aIndex, RBasicGsmPhone::TNetworkInfo& aNetworkInfo) const=0;
	virtual void GetMasterListNetworkInfoCancel() const=0;

	virtual CGsmNetworkList* GetMultipleMasterNetworkInfoLC(const TInt aIndexFrom, const TInt aIndexTo)=0;

/**/
/* Server/TSY internal struct*/
/**/
	struct TReadPositions
		{
		TUint iPosFrom;
		TUint iPosTo;
		};
	
	};

/**/
/* Phone security - Lock settings, status and security events/actions*/
/**/

class MAdvGsmPhoneSecurity
	{
public:
/**/
/* Access Phone Lock Functionality*/
/**/
	enum TLockType
		{
		ELockSIMCard,
		ELockPhoneDevice,	/* A pass phrase that locks the phone*/
		ELockPhoneToSim		/* Phone lock to Sim Card, requires pass phrase to lock/unlock*/
		};

	enum TLockCurrentStatus
		{
		EUnlocked,			/* Lock is now open - user can access functionality governed by this lock*/
		ELocked				/* Lock is now closed - user can not access functionality governed by this lock*/
		};

	enum TLockSetting
		{
		ELockDisabled,		/* Lock is disabled, so its status is always unlocked*/
		ELockEnabled		/* Lock is enabled, ( PIN code or password required to unlock the lock )*/
		};

	struct TLockInfo
		{
		TLockCurrentStatus	iStatus;
		TLockSetting 		iSetting;
		};

	struct TLockChangeSetting
		{
		TLockSetting 	iSetting;
		TGsmPassword 	iPassword;
		};

	virtual TInt GetLockInfo(const TLockType aType,TLockInfo& aLockInfo) const=0;
	virtual void GetLockInfo(TRequestStatus& aStatus, const TLockType aType,TLockInfo& aLockInfo) const=0;
	virtual void GetLockInfoCancel() const=0;

	virtual TInt ChangeLockSetting(const TLockType aType,const TLockChangeSetting& aLockSetting) const=0;
	virtual void ChangeLockSetting(TRequestStatus& aStatus, const TLockType aType,const TLockChangeSetting& aLockSetting) const=0;
	virtual void ChangeLockSettingCancel() const=0;

	virtual void NotifyLockInfoChanged(TRequestStatus& aReqStatus, TLockType& aLockType, TLockInfo& aLockInfo) const=0;
	virtual void NotifyLockInfoChangedCancel() const=0;

/**/
/* Change Password*/
/**/

	enum TSecurityCodeType
		{
		ESecurityCodePin1,
		ESecurityCodePin2,
		ESecurityCodePassPhrase
		};

	virtual TInt ChangePassword(const TSecurityCodeType aType,const TGsmPassword& aOldPassword, const TGsmPassword& aNewPassword) const=0;
	virtual void ChangePassword(TRequestStatus& aReqStatus, const TSecurityCodeType aType, const TGsmPassword& aOldPassword,const TGsmPassword& aNewPassword) const=0;
	virtual void ChangePasswordCancel() const=0;

/**/
/*  Security notifications*/
/**/
	enum TSecurityEvent
		{
		ENoSIMFound,
		ESimTerminated,
		EPin1Required,
		EPuk1Required,
		EPin2Required,
		EPuk2Required,
		EPassPhraseRequired,
		EPin1Verified,
		EPuk1Verified,
		EPin2Verified,
		EPuk2Verified,
		EPassPhraseVerified
		};

	virtual void NotifyOnSecurityEvent(TRequestStatus& aReqStatus, TSecurityEvent& aEvent) const=0;
	virtual void NotifyOnSecurityEventCancel() const=0;

/**/
/* Unlock with Security Code*/
/**/
	virtual TInt SendSecurityCode(const TSecurityCodeType aCodeType, const TGsmPassword& aCode) const=0;
	virtual void SendSecurityCode(TRequestStatus& aReqStatus, const TSecurityCodeType aCodeType, const TGsmPassword& aCode) const=0;
	virtual void SendSecurityCodeCancel() const=0;

	virtual TInt AbortSecurityCode(const TSecurityCodeType aCodeType) const=0;
	virtual void AbortSecurityCode(TRequestStatus& aReqStatus, const TSecurityCodeType aCodeType) const=0;
	virtual void AbortSecurityCodeCancel() const=0;

/**/
/* Unblock (and unlock) with PUK*/
/**/
	enum TUnblockCodeType
		{
		EUnblockCodePuk1,
		EUnblockCodePuk2
		};

	virtual TInt SendUnblockCode(const TUnblockCodeType aCodeType, const TGsmPassword& aUnblockCode, const TGsmPassword& aNewPin) const=0;
	virtual void SendUnblockCode(TRequestStatus& aReqStatus, const TUnblockCodeType aCodeType, const TGsmPassword& aUnblockCode, const TGsmPassword& aNewPin) const=0;
	virtual void SendUnblockCodeCancel() const=0;

	virtual TInt AbortUnblockCode(const TUnblockCodeType aCodeType) const=0;
	virtual void AbortUnblockCode(TRequestStatus& aReqStatus, const TUnblockCodeType aCodeType) const=0;
	virtual void AbortUnblockCodeCancel() const=0;

/**/
/* Dynamic Phone Lock Caps*/
/**/
	enum TPhoneLockCaps
		{
		KCapsLockSim=0x00000001,
		KCapsLockPhoneDevice=0x00000002,
		KCapsLockPhoneToSim=0x00000004
		};

	struct TSecurityCapsFlags
		{
		TUint iPhoneLockCaps;
		};

	virtual TInt GetPhoneLockCaps(TSecurityCapsFlags& aCaps) const=0;
	virtual void NotifyPhoneLockCaps(TRequestStatus& aReqStatus, TSecurityCapsFlags& aCaps) const=0;
	virtual void NotifyPhoneLockCapsCancel() const=0;

/**/
/* Server TSY internal definitions*/
/**/
public:
	struct TChangePassword
		{
		TSecurityCodeType iType;
		TGsmPassword iOldPassword;
		TGsmPassword iNewPassword;
		};

	struct TUnlockPIN
		{
		TUnblockCodeType iPukType;
		TGsmPassword iPuk;
		TGsmPassword iNewPin;
		};

	};

/**/
/* Advice of Charge - information, updates and settings*/
/**/
class MAdvGsmPhoneAoC
	{
public:

	enum TAocType
		{
		EAocTypeUnknown,		/* SIM & ME support AoC but no calls are in*/
								/* progress so network support is unknown*/
		EAocTypeNotAvailable,	/* Either call in progress but network does not support AoC*/
								/* Or SIM/ME does not support AoC*/
		EAocTypeInformation,	/* Call in progress - AoCI currently active*/
		EAocTypeCharging		/* Call in progress - AoCC currently active*/
		};

	struct TPuct
		{
		TBuf<3> iCurrencyName;
		TReal	iPricePerUnit;
		};

	struct TAocSimInfo
		{
		TReal iAcm;
		TUint iAcmUnits;
		TReal iAcmMax;
		TUint iAcmMaxUnits;
		TPuct iPuct;
		};

	struct TAocDynamicInfo
		{
		TAocType iType;
		TReal iCcm;			/* CCM is combined cost of all ongoing calls*/
		TUint iCcmUnits;					
		TReal iRemaining;	/* remaining=ACMmax-ACM*/
		TUint iRemainingUnits;
		};

	enum TAocMeCaps
		{
		KCapsAocInformation=0x00000001,	/* ME supports AoCI*/
		KCapsAocCharging=0x00000002,	/* ME supports AoCC*/
		KCapsCallMeterUnits=0x00000004,	/* ME supports the "unit" version of ACM, CCM and ACMmax*/
		KCapsCallMeterReal=0x00000008   /* ME supports the "real" version of ACM, CCM and ACMmax*/
		};

	struct TAocCapsFlags
		{
		TUint iAocMeCaps;
		};

	/* Resets ACM in the SIM to zero. */
	/* Probably also resets CCM to zero and Remaining to its max level */
	virtual TInt ClearAcm() const=0;
	virtual void ClearAcm(TRequestStatus& aReqStatus) const=0;
	virtual void ClearAcmCancel() const=0;
	
	/* Sets AcmMax in the SIM to the specified value. */
	/* To conform to GSM 11.11, aAcmMax must always be greater than zero */
	virtual TInt SetAcmMax(const TReal aAcmMax) const=0;
	virtual void SetAcmMax(TRequestStatus& aReqStatus, const TReal aAcmMax) const=0;
	virtual TInt SetAcmMax(const TUint aChargingUnits) const=0;
	virtual void SetAcmMax(TRequestStatus& aReqStatus, const TUint aChargingUnits) const=0;
	virtual void SetAcmMaxCancel() const=0;
	
	/* Sets PUCT in the SIM*/
	virtual TInt SetPuct(const TPuct& aPuctSetting) const=0;
	virtual void SetPuct(TRequestStatus& aReqStatus, const TPuct& aPuctSetting) const=0;
	virtual void SetPuctCancel() const=0;
	
	/* Get snapshot of current AoC related data on the SIM */
	virtual TInt GetAocSimInfo(TAocSimInfo& aAocSimInfo) const=0;
	virtual void GetAocSimInfo(TRequestStatus& aReqStatus, TAocSimInfo& aAocSimInfo) const=0;
	virtual void GetAocSimInfoCancel() const=0;
	/* Notification if AoC SIM data is changed by another client or by the TSY/ME*/
	virtual void NotifyAocSimInfoChanged(TRequestStatus& aReqStatus, TAocSimInfo& aAocInfo) const=0;
	virtual void NotifyAocSimInfoChangedCancel() const=0;

	/* Dynamic AoC information that changes during a call */
	virtual TInt GetAocDynamicInfo(TAocDynamicInfo& aAocInfo) const=0;
	virtual void GetAocDynamicInfo(TRequestStatus& aReqStatus, TAocDynamicInfo& aAocInfo) const=0;
	virtual void GetAocDynamicInfoCancel() const=0;
	virtual void NotifyAocDynamicInfoChanged(TRequestStatus& aReqStatus, TAocDynamicInfo& aAocInfo) const=0;
	virtual void NotifyAocDynamicInfoChangedCancel() const=0;

	/* Interrogate AoC capabilities - a summation of ME & SIM Aoc caps */
	virtual TInt GetAocCaps(TAocCapsFlags& aCaps) const=0;
	virtual void GetAocCaps(TRequestStatus& aReqStatus, TAocCapsFlags& aCaps) const=0; 
	virtual void GetAocCapsCancel() const=0;

	/**/
	/*	Server internal structure*/
	/**/
	enum TWhichAcm
		{
		EAcmReal,
		EAcmUnits,
		};

	};

/**/
/* Activate/Deactive Fixed Dialling Numbers*/
/**/
class MAdvGsmPhoneFdn
	{
public:
	enum TFdnModeGet
		{
		EFdnModeNotActive,
		EFdnModeActive,
		EFdnModePermanentlyActive,
		EFdnModeNotSupported,
		EFdnModeUnknown
		};

	enum TFdnModeSet
		{
		EFdnModeActivate,
		EFdnModeDeactivate
		};

	virtual TInt SetFdnMode(const TFdnModeSet aFdnMode) const=0;
	virtual void SetFdnMode(TRequestStatus& aReqStatus, const TFdnModeSet aFdnMode) const=0;
	virtual void SetFdnModeCancel() const=0;
	
	virtual TInt GetFdnMode(TFdnModeGet& aFdnMode) const=0;
	virtual void GetFdnMode(TRequestStatus& aReqStatus, TFdnModeGet& aFdnMode) const=0;
	virtual void GetFdnModeCancel() const=0;

	virtual void NotifyFdnModeChanged(TRequestStatus& aReqStatus, TFdnModeGet& aFdnMode) const=0;
	virtual void NotifyFdnModeChangedCancel() const=0;
	};

/**/
/* Retrieve Total Air Time Duration*/
/**/
class MAdvGsmPhoneAirTimeDuration
	{
public:
	virtual TInt GetAirTimeDuration(TTimeIntervalSeconds& aTime) const=0;
	virtual void GetAirTimeDuration(TRequestStatus& aReqStatus, TTimeIntervalSeconds& aTime) const=0;
	virtual void GetAirTimeDurationCancel() const=0;

	virtual void NotifyAirTimeDurationChange(TRequestStatus& aReqStatus,TTimeIntervalSeconds& aTime) const=0;
	virtual void NotifyAirTimeDurationChangeCancel() const=0;
	};

/**/
/* Call Forwarding on all lines - get/set call forwarding for "all basic services"*/
/**/

class MAdvGsmPhoneCallForwarding
	{
public:	
	virtual TInt GetCallForwardingInfo(const MAdvGsmLineForwarding::TCallForwardingReasonGet aReason, MAdvGsmLineForwarding::TCallForwardingInfo& aInfo,const RAdvGsmLine::TForwardingInfoLocation aLocation = RAdvGsmLine::EForwardingLocationCache) const=0;
	virtual void GetCallForwardingInfo(TRequestStatus& aReqStatus,const MAdvGsmLineForwarding::TCallForwardingReasonGet aReason, MAdvGsmLineForwarding::TCallForwardingInfo& aInfo,const RAdvGsmLine::TForwardingInfoLocation aLocation = RAdvGsmLine::EForwardingLocationCache) const=0;
	virtual void GetCallForwardingInfoCancel() const=0;
	virtual TInt SetCallForwardingInfo(const MAdvGsmLineForwarding::TCallForwardingReasonSet aReason, MAdvGsmLineForwarding::TCallForwardingChange& aInfo) const=0;
	virtual void SetCallForwardingInfo(TRequestStatus& aReqStatus,const MAdvGsmLineForwarding::TCallForwardingReasonSet aReason, MAdvGsmLineForwarding::TCallForwardingChange& aInfo) const=0;
	virtual void SetCallForwardingInfoCancel() const=0;
	virtual void CallForwardingStatusNotification(TRequestStatus& aReqStatus, MAdvGsmLineForwarding::TCallForwardingReasonGet& aReason,MAdvGsmLineForwarding::TCallForwardingInfo& aInfo) const=0;
	virtual void CallForwardingStatusNotificationCancel() const=0;
	};

/**/
/* Call Barring on all lines - get/set call barring for "all basic services"*/
/**/

class MAdvGsmPhoneCallBarring
	{
public:	
	struct TChangeLockPassword
		{
		TBuf<KGsmPasswordSize> iOldPassword;
		TBuf<KGsmPasswordSize> iNewPassword;
		};

	virtual TInt ChangeBarringPassword(const TChangeLockPassword& aPasswords) const=0;
	virtual void ChangeBarringPassword(TRequestStatus& aReqStatus, const TChangeLockPassword& aPasswords) const=0;
	virtual void ChangeBarringPasswordCancel() const=0;

	virtual TInt GetCallBarringMode(const MAdvGsmLineBarring::TBarringType aType, MAdvGsmLineBarring::TBarringMode& aMode, MAdvGsmLineBarring::TBarringModeLocation aLocation = RAdvGsmLine::EBarringLocationCache) const=0;
	virtual void GetCallBarringMode(TRequestStatus& aReqStatus,const MAdvGsmLineBarring::TBarringType aType, MAdvGsmLineBarring::TBarringMode& aMode, MAdvGsmLineBarring::TBarringModeLocation aLocation = RAdvGsmLine::EBarringLocationCache) const=0;
	virtual void GetCallBarringModeCancel() const=0;
	virtual TInt SetCallBarringMode(const MAdvGsmLineBarring::TBarringType aType,const MAdvGsmLineBarring::TChangeBarringMode& aMode) const=0;
	virtual void SetCallBarringMode(TRequestStatus& aReqStatus,const MAdvGsmLineBarring::TBarringType aType,const MAdvGsmLineBarring::TChangeBarringMode& aMode) const=0;
	virtual void SetCallBarringModeCancel() const=0;
	virtual void CallBarringModeNotification(TRequestStatus& aReqStatus, MAdvGsmLineBarring::TBarringType& aType, MAdvGsmLineBarring::TBarringMode& aMode) const=0;
	virtual void CallBarringModeNotificationCancel() const=0;
	};

/**/
/* Call Waiting on all lines - get/set call waiting for "all basic services"*/
/**/

class MAdvGsmPhoneCallWaiting
	{
public:	
	virtual TInt GetCallWaitingMode(MAdvGsmLineCallWaitingMode::TCallWaitingMode& aMode,const MAdvGsmLineCallWaitingMode::TWaitingModeLocation aLocation = RAdvGsmLine::EWaitingLocationCache) const=0;
	virtual void GetCallWaitingMode(TRequestStatus& aReqStatus,MAdvGsmLineCallWaitingMode::TCallWaitingMode& aMode,const MAdvGsmLineCallWaitingMode::TWaitingModeLocation aLocation = RAdvGsmLine::EWaitingLocationCache) const=0;
	virtual void GetCallWaitingModeCancel() const=0;
	virtual TInt SetCallWaitingMode(const MAdvGsmLineCallWaitingMode::TCallWaitingSetting aSetting) const=0;
	virtual void SetCallWaitingMode(TRequestStatus& aReqStatus,const MAdvGsmLineCallWaitingMode::TCallWaitingSetting aSetting) const=0;
	virtual void SetCallWaitingModeCancel() const=0;
	virtual void CallWaitingModeNotification(TRequestStatus& aReqStatus, MAdvGsmLineCallWaitingMode::TCallWaitingMode& aMode) const=0;
	virtual void CallWaitingModeNotificationCancel() const=0;
	};


/**/
/* Supplementary Service Strings*/
/**/

class MAdvGsmPhoneSupplementaryService
	{
public:	
	virtual TInt SendSSString(const TDesC& aString) const=0;
	virtual void SendSSString(TRequestStatus& aReqStatus, const TDesC& aString) const=0;
	virtual void SendSSStringCancel() const=0;
	};

/**/
/* Send and receive DTMF Tones during connected voice calls.*/
/**/
class MAdvGsmPhoneDTMF
	{
public:

	enum TDTMFEvent
		{
		EDTMFEventAutoStart,
		EDTMFEventAutoComplete,
		EDTMFEventManualStart,
		EDTMFEventManualComplete
		};	

	virtual TInt SendDTMFTones(const TDesC& aTones) const=0;
	virtual void SendDTMFTones(TRequestStatus& aReqStatus, const TDesC& aTones) const=0;
	virtual void SendDTMFTonesCancel() const=0;

	virtual TInt ReadDTMFTones(TDes& aTones) const=0;
	virtual void ReadDTMFTones(TRequestStatus& aReqStatus, TDes& aTones) const=0;
	virtual void ReadDTMFTonesCancel() const=0;

	virtual void NotifyDTMFEvent(TRequestStatus& aReqStatus,TDTMFEvent& aEvent) const=0;
	virtual void NotifyDTMFEventCancel() const=0;

	virtual TInt StartDTMFTone(const TChar& aTone) const=0;
	virtual void StartDTMFTone(TRequestStatus& aReqStatus, const TChar& aTone) const=0;
	virtual void StartDTMFToneCancel() const=0;

	virtual TInt StopDTMFTone() const=0;
	virtual void StopDTMFTone(TRequestStatus& aReqStatus) const=0;
	virtual void StopDTMFToneCancel() const=0;

	virtual TInt SkipDTMFPause() const=0;
	virtual void SkipDTMFPause(TRequestStatus& aReqStatus) const=0;
	virtual void SkipDTMFPauseCancel() const=0;

	};

class CGsmEmergencyNumbersList;
class MAdvGsmPhoneEmergencyNumbers
	{
public:
	class TEmergencyNumber
		{
	public:
		void InternalizeL(RReadStream& aStream);
		void ExternalizeL(RWriteStream& aStream) const;
	public:
		IMPORT_C TEmergencyNumber();
	public:
		TGsmTelNumber iTelNumber;
		};

	virtual CGsmEmergencyNumbersList* RetrieveSimEmergencyNumbersLC()=0;
	virtual CGsmEmergencyNumbersList* RetrieveStandardEmergencyNumbersLC() const=0;
	};

/**/
/*  Phone's static capabilities for alternating calls*/
/* */

class MAdvGsmPhoneAltCall
	{
public:
	enum TAlternatingModeCaps
		{
		KCapsCallModeSingle=0x00000001,
		KCapsCallModeVoiceFax=0x00000002,
		KCapsCallModeVoiceData=0x00000004,
		KCapsCallModeVoiceThenData=0x00000008
		};

	struct TAlternatingCallModeCapsFlags
		{
		TUint iAlternatingCallModeCaps;
		};

	virtual TInt GetAlternatingCallModeCaps(TAlternatingCallModeCapsFlags& aCaps) const=0;
	};

/**/
/*  Alternate Line Service - access to the auxiliary voice channel*/
/**/

class MAdvGsmPhoneAlternateLineService
	{
public:
	enum TALSLineGet
		{
		ELinePrimary,		/* ALS is supported by ME & SIM and Primary Line is the current line*/
		ELineAuxiliary,		/* ALS is supported by ME & SIM and Auxiliary Line is the current line*/
		ELineUnknown,		/* ALS is supported by ME & SIM but TSY can not determine line selection*/
		ELineNotSupported	/* ALS is not supported either by ME or SIM or both.*/
		};	

	enum TALSLineSet
		{
		ESelectPrimaryLine,
		ESelectAuxiliaryLine
		};	

	virtual TInt SetALSLine(const TALSLineSet  aLineSetting) const=0;
	virtual void SetALSLine(TRequestStatus& aReqStatus, const TALSLineSet aLineSetting) const=0;
	virtual void SetALSLineCancel() const=0;

	virtual TInt GetALSLine(TALSLineGet&  aLineSetting) const=0;
	virtual void GetALSLine(TRequestStatus& aReqStatus, TALSLineGet&  aLineSetting) const=0;
	virtual void GetALSLineCancel() const=0;

	virtual void NotifyALSLineChange(TRequestStatus& aReqStatus, TALSLineGet&  aLineSetting) const=0;
	virtual void NotifyALSLineChangeCancel() const=0;
	};

/**/
/*  Phone-level CCBS request interrogation & deactivation*/
/**/

class CAdvGsmCcbsRequestList;
class MAdvGsmPhoneCCBS
	{
public:
	enum TCcbsStatus
		{
		ECcbsNotProvisioned,	/* user has not subscribed to CCBS*/
		ECcbsNotActive,			/* user has CCBS provisioned but no CCBS requests are currently active*/
		ECcbsActive,			/* user has CCBS provisioned and there is at least 1 ( and up to 5) active CCBS requests*/
		ECcbsUnknown			/* Phone can not determine the CCBS status	*/
		};

	enum TCcbsStatusLocation
		{
		ECcbsLocationCache,
		ECcbsLocationCachePreferred,
		ECcbsLocationNetwork
		};

	class TCcbsRequest
		{
	public:
		void InternalizeL(RReadStream& aStream);
		void ExternalizeL(RWriteStream& aStream) const;
	public:
		IMPORT_C TCcbsRequest();

		TInt  iCcbsIndex;		/* The index given to the CCBS request by the network*/
		TName iCallName;		/* Name of the call object that represents the CCBS request*/
		TName iLineName;		/* Indication of which basic service (voice, data or fax) this CCBS is applicable to*/
		TGsmTelNumber iDestination;		/* Tel Number of the CCBS request's destination*/
		};

	/* User is deactivating all outstanding CCBS requests.*/
	virtual TInt DeactivateAllCCBS() const=0;
	virtual void DeactivateAllCCBS(TRequestStatus& aReqStatus) const=0;
	virtual void DeactivateAllCCBSCancel() const=0;

	/* CCBS interrogation returns the CCBS status and if the service is active, */
	/* the destination numbers and basic services against which CCBS requests are outstanding*/
	virtual TInt GetCcbsStatus(TCcbsStatus& aCcbsStatus, const TCcbsStatusLocation aLocation = ECcbsLocationCache) const=0;
	virtual void GetCcbsStatus(TRequestStatus& aReqStatus, TCcbsStatus& aCcbsStatus, const TCcbsStatusLocation aLocation = ECcbsLocationCache) const=0;
	virtual void GetCcbsStatusCancel() const=0;

	/* If CCBS status indicates that the service is active, client can get list of */
	/* active CCBS requests using this method.*/

	/* Synchronous version*/
	virtual CAdvGsmCcbsRequestList* RetrieveActiveCCBSRequestsLC()=0;
	};

/**/
/*  Preferred Language lists*/
/**/

class CAdvGsmLanguageList;
class MAdvGsmPhoneLanguage
	{
public:
	struct TCombinedEntries
		{
		TInt iEntryCount;
		TInt iExtEntryCount;
		};

	virtual CAdvGsmLanguageList* RetrievePreferredLanguagesLC()=0;
	
	virtual TInt StorePreferredLanguagesL(CAdvGsmLanguageList* aLanguageList) const=0; 
	virtual void StorePreferredLanguagesL(TRequestStatus& aReqStatus, CAdvGsmLanguageList* aLanguageList) const=0;
	virtual void StorePreferredLanguagesCancel() const=0;
	
	virtual void NotifyPreferredLanguagesChanged(TRequestStatus& aReqStatus) const=0;
	virtual void NotifyPreferredLanguagesChangedCancel() const=0;
	};

/**/
/*  Customer Service Profile & SIM Service Table*/
/**/

class MAdvGsmPhoneSimCapabilities
	{
public:
	enum TCspCallOffering
		{
		KCspCT=0x08,
		KCspCFNRc=0x10,
		KCspCFNRy=0x20,
		KCspCFB=0x40,
		KCspCFU=0x80
		};

	enum TCspCallRestriction
		{
		KCspBICRoam=0x08,
		KCspBAIC=0x10,
		KCspBOICexHC=0x20,
		KCspBOIC=0x40,
		KCspBOAC=0x80
		};

	enum TCspOtherSuppServices
		{
		KCspCUGOA=0x08,
		KCspPrefCUG=0x10,
		KCspAoC=0x20,
		KCspCUG=0x40,
		KCspMPTY=0x80,
		};

	enum TCspCallCompletion
		{
		KCspCCBS=0x20,
		KCspCW=0x40,
		KCspHOLD=0x80,
		};

	enum TCspTeleservices
		{
		KCspValidityPeriod=0x02,
		KCspProtocolID=0x04,
		KCspDelConf=0x08,
		KCspReplyPath=0x10,
		KCspSMCB=0x20,
		KCspSMMO=0x40,
		KCspSMMT=0x80,
		};

	enum TCspCPHSTeleservices
		{
		KCspALS=0x80
		};

	enum TCspCPHSFeatures
		{
		KCspReservedSST=0x80
		};

	enum TCspNumberIdentification
		{
		KCspCLIBlock=0x01,
		KCspCLISend=0x02,
		KCspCOLP=0x10,
		KCspCOLR=0x20,
		KCspCLIP=0x80,
		};

	enum TCspValueAdded
		{
		KCspLanguage=0x01,
		KCspData=0x04,
		KCspFax=0x08,
		KCspSMMOEmail=0x10,
		KCspSMMOPaging=0x20,
		KCspPLMNMode=0x80,
		};

	struct TCspFile
		{
		TUint8	iCallOfferingServices;
		TUint8	iCallRestrictionServices;
		TUint8	iOtherSuppServices;
		TUint8	iCallCompletionServices;
		TUint8  iTeleservices;
		TUint8	iCphsTeleservices;
		TUint8	iCphsFeatures;
		TUint8	iNumberIdentServices;
		TUint8	iPhase2PlusServices;
		TUint8	iValueAddedServices;
		};

	virtual TInt GetCustomerServiceProfile(TCspFile& aCsp) const=0;
	virtual void GetCustomerServiceProfile(TRequestStatus& aReqStatus, TCspFile& aCsp) const=0;
	virtual void GetCustomerServiceProfileCancel() const=0;


	enum TSSTServices1To8
		{
		KSstPin1Disable=0x01,
		KSstADN=0x02,
		KSstFDN=0x04,
		KSstSMS=0x08,
		KSstAoC=0x10,
		KSstCCP=0x20,
		KSstPLMNSelector=0x40
		};

	enum TSSTServices9To16
		{
		KSstMSISDN=0x01,
		KSstExt1=0x02,
		KSstExt2=0x04,
		KSstLND=0x10,
		KSstCBMI=0x20
		};
	
	enum TSSTServices17To24
		{
		KSstSDN=0x02,
		KSstExt3=0x04
		};

	enum TSSTServices25To32
		{
		KSstCBMIRanges=0x20,
		KSstBDN=0x40,
		KSstExt4=0x80
		};

	enum TSSTServices33To40
		{
		KSstSMStatusReports=0x04
		};

	struct TSstFile
		{
		TUint8 iServices1To8;
		TUint8 iServices9To16;
		TUint8 iServices17To24;
		TUint8 iServices25To32;
		TUint8 iServices33To40;
		};

	virtual TInt GetSimServiceTable(TSstFile& aSst) const=0;
	virtual void GetSimServiceTable(TRequestStatus& aReqStatus, TSstFile& aSst) const=0;
	virtual void GetSimServiceTableCancel() const=0;
	};

/**/
/* Advanced GSM Phone Information*/
/**/

class MAdvGsmPhoneInformation
	{
public:
	enum {KMaxImsiLength = 15};

	typedef TBuf<KMaxImsiLength>  TSubscriberId;

	virtual TInt GetSubscriberIdNS(TSubscriberId& aId) const=0;
	virtual void GetSubscriberIdNS(TRequestStatus& aReqStatus, TSubscriberId& aId) const=0;
	virtual void GetSubscriberIdCancel() const=0;
	};
	
/**/
/**/
/* RAdvGsmPhone*/
/**/
/**/
class CAdvPhonePtrHolder;
class RAdvGsmPhone : public RBasicGsmPhone, public MAdvGsmPhoneOwnNumber,
					 public MAdvGsmPhoneNitz, public MAdvGsmPhoneNetwork, 
					 public MAdvGsmPhoneSecurity, public MAdvGsmPhoneAoC, public MAdvGsmPhoneFdn,
					 public MAdvGsmPhoneAirTimeDuration, public MAdvGsmPhoneCallForwarding,
					 public MAdvGsmPhoneCallBarring, public MAdvGsmPhoneCallWaiting,
					 public MAdvGsmPhoneSupplementaryService, public MAdvGsmPhoneLineIdentity,
					 public MAdvGsmPhoneSimCapabilities, public MAdvGsmPhoneDTMF, 
					 public MAdvGsmPhoneEmergencyNumbers, public MAdvGsmPhoneAltCall, public MAdvGsmPhoneLanguage,
					 public MAdvGsmPhoneAlternateLineService, public MAdvGsmPhoneCCBS,
					 public MAdvGsmPhoneInformation
	{
public:
	friend class CAdvGsmRetrievePreferredNetworks;
	friend class CAdvGsmRetrieveStaticNetworks;
	friend class CGsmRetrieveSimEmergencyNumbers;
	friend class CAdvGsmRetrievePrefLanguages;
	friend class CAdvGsmRetrieveActiveCcbs;
	
	IMPORT_C RAdvGsmPhone();

	/* Server/TSY struct*/
	struct TClientId
		{
		TInt iSessionHandle;
		TInt iSubSessionHandle;
		};

	class TPhoneInfo : public RBasicGsmPhone::TPhoneInfo
		{
	public:
		TClipMode iClipMode;
		TClirMode iClirMode;		
		TColpMode iColpMode;
		TColrMode iColrMode;
		TFdnModeGet iFdnMode;
		TAocDynamicInfo iAocDynamicInfo;
	public:
		IMPORT_C TPhoneInfo();
		};

		struct TNumberOfMsgWaiting
		{
		TUint8 iVoiceLine1;		/* Number of voice messages waiting on line 1, CPHS spec compliant and 3GPP generic		*/
		TUint8 iVoiceLine2;		/* Number of voice messages waiting on line 2, CPHS spec compliant*/
		TUint8 iFax;			/* Number of fax messages waiting, CPHS and 3GPP spec compliant*/
		TUint8 iEmail;			/* Number of email messages waiting, 3GPP spec compliant*/
		TUint8 iData;			/* Number of data messages waiting, CPHS spec compliant*/
		TUint8 iOthers;			/* Number of other messages waiting, 3GPP spec compliant*/
		};

	IMPORT_C TInt GetInfoAdvGsm(TPhoneInfo& aPhoneInfo) const;
	IMPORT_C void GetInfoAdvGsm(TRequestStatus& aReqStatus,TPhoneInfo& aPhoneInfo) const;
	IMPORT_C void GetInfoAdvGsmCancel() const;

	IMPORT_C TInt GetAdvGsmLineInfo(const TInt aIndex, RAdvGsmLine::TAdvGsmLineInfo& aLineInfo) const;

	IMPORT_C TInt GetOwnNumberInfo(TOwnNumberInfo& aOwnNumberInfo) const;
	IMPORT_C void GetOwnNumberInfo(TRequestStatus& aReqStatus, TOwnNumberInfo& aOwnNumberInfo) const;
	IMPORT_C void GetOwnNumberInfoCancel() const;
	IMPORT_C TInt GetOwnNumberEntry(const TInt aIndex, TOwnNumberEntry& aEntry) const;
	IMPORT_C void GetOwnNumberEntry(TRequestStatus& aReqStatus, const TInt aIndex, TOwnNumberEntry& aEntry) const;
	IMPORT_C void GetOwnNumberEntryCancel() const;
	IMPORT_C TInt SetOwnNumberEntry(const TInt aIndex, const TOwnNumberEntry& aEntry) const;
	IMPORT_C void SetOwnNumberEntry(TRequestStatus& aReqStatus, const TInt aIndex, const TOwnNumberEntry& aEntry) const;
	IMPORT_C void SetOwnNumberEntryCancel() const;
	IMPORT_C void NotifyOwnNumberEntryChanged(TRequestStatus& aReqStatus, TInt& aIndex) const;
	IMPORT_C void NotifyOwnNumberEntryChangedCancel() const;

	IMPORT_C TInt GetClipMode(TClipMode& aMode) const;
	IMPORT_C void GetClipMode(TRequestStatus& aReqStatus, TClipMode& aMode) const;
	IMPORT_C void GetClipModeCancel() const;
	IMPORT_C void ClipModeNotification(TRequestStatus& aReqStatus, TClipMode& aMode) const;
	IMPORT_C void ClipModeNotificationCancel() const;

	IMPORT_C TInt GetClirMode(TClirMode& aMode) const;
	IMPORT_C void GetClirMode(TRequestStatus& aReqStatus, TClirMode& aMode) const;
	IMPORT_C void GetClirModeCancel() const;
	IMPORT_C void ClirModeNotification(TRequestStatus& aReqStatus, TClirMode& aMode) const;
	IMPORT_C void ClirModeNotificationCancel() const;

	IMPORT_C TInt GetColpMode(TColpMode& aMode) const;
	IMPORT_C void GetColpMode(TRequestStatus& aReqStatus, TColpMode& aMode) const;
	IMPORT_C void GetColpModeCancel() const;
	IMPORT_C void ColpModeNotification(TRequestStatus& aReqStatus, TColpMode& aMode) const;
	IMPORT_C void ColpModeNotificationCancel() const;

	IMPORT_C TInt GetColrMode(TColrMode& aMode) const;
	IMPORT_C void GetColrMode(TRequestStatus& aReqStatus, TColrMode& aMode) const;
	IMPORT_C void GetColrModeCancel() const;
	IMPORT_C void ColrModeNotification(TRequestStatus& aReqStatus, TColrMode& aMode) const;
	IMPORT_C void ColrModeNotificationCancel() const;

	IMPORT_C CGsmNetworkList* RetrievePreferredNetworksLC();
	IMPORT_C TInt StorePreferredNetworksL(CGsmNetworkList* aNetworkList) const; 
	IMPORT_C void StorePreferredNetworksL(TRequestStatus& aReqStatus, CGsmNetworkList* aNetworkList) const;
	IMPORT_C void StorePreferredNetworksCancel() const;
	IMPORT_C void NotifyPreferredNetworksChanged(TRequestStatus& aReqStatus) const;
	IMPORT_C void NotifyPreferredNetworksChangedCancel() const; 
	IMPORT_C TInt GetHomeNetworkInfo(RBasicGsmPhone::TNetworkInfo& aNetworkInfo) const;
	IMPORT_C void GetHomeNetworkInfo(TRequestStatus& aReqStatus, RBasicGsmPhone::TNetworkInfo& aNetworkInfo) const;
	IMPORT_C void GetHomeNetworkInfoCancel() const;

	IMPORT_C TInt EnumerateMasterListNetworks(TInt& aPhoneNetworks) const;
	IMPORT_C void EnumerateMasterListNetworks(TRequestStatus& aReqStatus, TInt& aPhoneNetworks) const;
	IMPORT_C void EnumerateMasterListNetworksCancel() const;

	IMPORT_C TInt GetMasterListNetworkInfo(const TInt aIndex, TNetworkInfo& aNetworkInfo) const;
	IMPORT_C void GetMasterListNetworkInfo(TRequestStatus& aReqStatus, const TInt aIndex, TNetworkInfo& aNetworkInfo) const;
	IMPORT_C void GetMasterListNetworkInfoCancel() const;

	IMPORT_C CGsmNetworkList* GetMultipleMasterNetworkInfoLC(const TInt aIndexFrom, const TInt aIndexTo);

	IMPORT_C TInt GetCurrentNITZInfo(TNITZInfo& aNITZInfo) const;					
	IMPORT_C void NotifyNITZInfoChange(TRequestStatus& aReqStatus, TNITZInfo& aNITZInfo) const;
	IMPORT_C void NotifyNITZInfoChangeCancel() const;

	IMPORT_C TInt GetLockInfo(const TLockType aType,TLockInfo& aLockInfo) const;
	IMPORT_C void GetLockInfo(TRequestStatus& aStatus, const TLockType aType,TLockInfo& aLockInfo) const;
	IMPORT_C void GetLockInfoCancel() const;

	IMPORT_C TInt ChangeLockSetting(const TLockType aType,const TLockChangeSetting& aLockSetting) const;
	IMPORT_C void ChangeLockSetting(TRequestStatus& aStatus, const TLockType aType,const TLockChangeSetting& aLockSetting) const;
	IMPORT_C void ChangeLockSettingCancel() const;

	IMPORT_C void NotifyLockInfoChanged(TRequestStatus& aReqStatus, TLockType& aLockType, TLockInfo& aLockInfo) const;
	IMPORT_C void NotifyLockInfoChangedCancel() const;

	IMPORT_C TInt ChangePassword(const TSecurityCodeType aType,const TGsmPassword& aOldPassword,const TGsmPassword& aNewPassword) const;
	IMPORT_C void ChangePassword(TRequestStatus& aReqStatus,const TSecurityCodeType aType,const TGsmPassword& aOldPassword,const TGsmPassword& aNewPassword) const;
	IMPORT_C void ChangePasswordCancel() const;

	IMPORT_C void NotifyOnSecurityEvent(TRequestStatus& aReqStatus, TSecurityEvent& aEvent) const;
	IMPORT_C void NotifyOnSecurityEventCancel() const;

	IMPORT_C TInt SendSecurityCode(const TSecurityCodeType aCodeType, const TGsmPassword& aCode) const;
	IMPORT_C void SendSecurityCode(TRequestStatus& aReqStatus, const TSecurityCodeType aCodeType, const TGsmPassword& aCode) const;
	IMPORT_C void SendSecurityCodeCancel() const;
	IMPORT_C TInt AbortSecurityCode(const TSecurityCodeType aCodeType) const;
	IMPORT_C void AbortSecurityCode(TRequestStatus& aReqStatus, const TSecurityCodeType aCodeType) const;
	IMPORT_C void AbortSecurityCodeCancel() const;

	IMPORT_C TInt SendUnblockCode(const TUnblockCodeType aCodeType, const TGsmPassword& aUnblockCode, const TGsmPassword& aNewPin) const;
	IMPORT_C void SendUnblockCode(TRequestStatus& aReqStatus, const TUnblockCodeType aCodeType, const TGsmPassword& aUnblockCode, const TGsmPassword& aNewPin) const;
	IMPORT_C void SendUnblockCodeCancel() const;
	IMPORT_C TInt AbortUnblockCode(const TUnblockCodeType aCodeType) const;
	IMPORT_C void AbortUnblockCode(TRequestStatus& aReqStatus, const TUnblockCodeType aCodeType) const;
	IMPORT_C void AbortUnblockCodeCancel() const;

	IMPORT_C TInt GetPhoneLockCaps(TSecurityCapsFlags& aCaps) const;
	IMPORT_C void NotifyPhoneLockCaps(TRequestStatus& aReqStatus, TSecurityCapsFlags& aCaps) const;
	IMPORT_C void NotifyPhoneLockCapsCancel() const;

	IMPORT_C TInt ClearAcm() const;
	IMPORT_C void ClearAcm(TRequestStatus& aReqStatus) const;
	IMPORT_C void ClearAcmCancel() const;
	IMPORT_C TInt SetAcmMax(const TReal aAcmMax) const;
	IMPORT_C void SetAcmMax(TRequestStatus& aReqStatus, const TReal aAcmMax) const;
	IMPORT_C TInt SetAcmMax(const TUint aChargingUnits) const;
	IMPORT_C void SetAcmMax(TRequestStatus& aReqStatus, const TUint aChargingUnits) const;
	IMPORT_C void SetAcmMaxCancel() const;
	IMPORT_C TInt SetPuct(const TPuct& aPuctSetting) const;
	IMPORT_C void SetPuct(TRequestStatus& aReqStatus, const TPuct& aPuctSetting) const;
	IMPORT_C void SetPuctCancel() const;
	IMPORT_C TInt GetAocSimInfo(TAocSimInfo& aAocSimInfo) const;
	IMPORT_C void GetAocSimInfo(TRequestStatus& aReqStatus, TAocSimInfo& aAocSimInfo) const;
	IMPORT_C void GetAocSimInfoCancel() const;
	IMPORT_C void NotifyAocSimInfoChanged(TRequestStatus& aReqStatus, TAocSimInfo& aAocInfo) const;
	IMPORT_C void NotifyAocSimInfoChangedCancel() const;
	IMPORT_C TInt GetAocDynamicInfo(TAocDynamicInfo& aAocInfo) const;
	IMPORT_C void GetAocDynamicInfo(TRequestStatus& aReqStatus, TAocDynamicInfo& aAocInfo) const;
	IMPORT_C void GetAocDynamicInfoCancel() const;
	IMPORT_C void NotifyAocDynamicInfoChanged(TRequestStatus& aReqStatus, TAocDynamicInfo& aAocInfo) const;
	IMPORT_C void NotifyAocDynamicInfoChangedCancel() const;
	IMPORT_C TInt GetAocCaps(TAocCapsFlags& aCaps) const;
	IMPORT_C void GetAocCaps(TRequestStatus& aReqStatus, TAocCapsFlags& aCaps) const; 
	IMPORT_C void GetAocCapsCancel() const;

	IMPORT_C TInt SetFdnMode(const TFdnModeSet aFdnMode) const;
	IMPORT_C void SetFdnMode(TRequestStatus& aReqStatus, const TFdnModeSet aFdnMode) const;
	IMPORT_C void SetFdnModeCancel() const;
	IMPORT_C TInt GetFdnMode(TFdnModeGet& aFdnMode) const;
	IMPORT_C void GetFdnMode(TRequestStatus& aReqStatus, TFdnModeGet& aFdnMode) const;
	IMPORT_C void GetFdnModeCancel() const;
	IMPORT_C void NotifyFdnModeChanged(TRequestStatus& aReqStatus, TFdnModeGet& aFdnMode) const;
	IMPORT_C void NotifyFdnModeChangedCancel() const;

	IMPORT_C TInt GetAirTimeDuration(TTimeIntervalSeconds& aTime) const;
	IMPORT_C void GetAirTimeDuration(TRequestStatus& aReqStatus, TTimeIntervalSeconds& aTime) const;
	IMPORT_C void GetAirTimeDurationCancel() const;
	IMPORT_C void NotifyAirTimeDurationChange(TRequestStatus& aReqStatus,TTimeIntervalSeconds& aTime) const;
	IMPORT_C void NotifyAirTimeDurationChangeCancel() const;

	IMPORT_C TInt GetCallForwardingInfo(const MAdvGsmLineForwarding::TCallForwardingReasonGet aReason, MAdvGsmLineForwarding::TCallForwardingInfo& aInfo,const RAdvGsmLine::TForwardingInfoLocation aLocation = RAdvGsmLine::EForwardingLocationCache) const;
	IMPORT_C void GetCallForwardingInfo(TRequestStatus& aReqStatus,const MAdvGsmLineForwarding::TCallForwardingReasonGet aReason, MAdvGsmLineForwarding::TCallForwardingInfo& aInfo,const RAdvGsmLine::TForwardingInfoLocation aLocation = RAdvGsmLine::EForwardingLocationCache) const;
	IMPORT_C void GetCallForwardingInfoCancel() const;
	IMPORT_C TInt SetCallForwardingInfo(const MAdvGsmLineForwarding::TCallForwardingReasonSet aReason, MAdvGsmLineForwarding::TCallForwardingChange& aInfo) const;
	IMPORT_C void SetCallForwardingInfo(TRequestStatus& aReqStatus,const MAdvGsmLineForwarding::TCallForwardingReasonSet aReason, MAdvGsmLineForwarding::TCallForwardingChange& aInfo) const;
	IMPORT_C void SetCallForwardingInfoCancel() const;
	IMPORT_C void CallForwardingStatusNotification(TRequestStatus& aReqStatus, MAdvGsmLineForwarding::TCallForwardingReasonGet& aReason,MAdvGsmLineForwarding::TCallForwardingInfo& aInfo) const;	
	IMPORT_C void CallForwardingStatusNotificationCancel() const;

	IMPORT_C TInt GetCallBarringMode(const MAdvGsmLineBarring::TBarringType aType, MAdvGsmLineBarring::TBarringMode& aMode, MAdvGsmLineBarring::TBarringModeLocation aLocation = RAdvGsmLine::EBarringLocationCache) const;
	IMPORT_C void GetCallBarringMode(TRequestStatus& aReqStatus,const MAdvGsmLineBarring::TBarringType aType, MAdvGsmLineBarring::TBarringMode& aMode, MAdvGsmLineBarring::TBarringModeLocation aLocation = RAdvGsmLine::EBarringLocationCache) const;
	IMPORT_C void GetCallBarringModeCancel() const;
	IMPORT_C TInt SetCallBarringMode(const MAdvGsmLineBarring::TBarringType aType,const MAdvGsmLineBarring::TChangeBarringMode& aMode) const;
	IMPORT_C void SetCallBarringMode(TRequestStatus& aReqStatus,const MAdvGsmLineBarring::TBarringType aType,const MAdvGsmLineBarring::TChangeBarringMode& aMode) const;
	IMPORT_C void SetCallBarringModeCancel() const;
	IMPORT_C void CallBarringModeNotification(TRequestStatus& aReqStatus, MAdvGsmLineBarring::TBarringType& aType, MAdvGsmLineBarring::TBarringMode& aMode) const;
	IMPORT_C void CallBarringModeNotificationCancel() const;
	IMPORT_C TInt ChangeBarringPassword(const TChangeLockPassword& aPasswords) const;
	IMPORT_C void ChangeBarringPassword(TRequestStatus& aReqStatus, const TChangeLockPassword& aPasswords) const;
	IMPORT_C void ChangeBarringPasswordCancel() const;

	IMPORT_C TInt GetCallWaitingMode(MAdvGsmLineCallWaitingMode::TCallWaitingMode& aMode,const MAdvGsmLineCallWaitingMode::TWaitingModeLocation aLocation = RAdvGsmLine::EWaitingLocationCache) const;
	IMPORT_C void GetCallWaitingMode(TRequestStatus& aReqStatus,MAdvGsmLineCallWaitingMode::TCallWaitingMode& aMode,const MAdvGsmLineCallWaitingMode::TWaitingModeLocation aLocation = RAdvGsmLine::EWaitingLocationCache) const;
	IMPORT_C void GetCallWaitingModeCancel() const;
	IMPORT_C TInt SetCallWaitingMode(const MAdvGsmLineCallWaitingMode::TCallWaitingSetting aSetting) const;
	IMPORT_C void SetCallWaitingMode(TRequestStatus& aReqStatus,const MAdvGsmLineCallWaitingMode::TCallWaitingSetting aSetting) const;
	IMPORT_C void SetCallWaitingModeCancel() const;
	IMPORT_C void CallWaitingModeNotification(TRequestStatus& aReqStatus, MAdvGsmLineCallWaitingMode::TCallWaitingMode& aMode) const;
	IMPORT_C void CallWaitingModeNotificationCancel() const;

	IMPORT_C TInt SendSSString(const TDesC& aString) const;
	IMPORT_C void SendSSString(TRequestStatus& aReqStatus, const TDesC& aString) const;
	IMPORT_C void SendSSStringCancel() const;

	IMPORT_C TInt SendDTMFTones(const TDesC& aTones) const;
	IMPORT_C void SendDTMFTones(TRequestStatus& aReqStatus, const TDesC& aTones) const;
	IMPORT_C void SendDTMFTonesCancel() const;
	IMPORT_C TInt ReadDTMFTones(TDes& aTones) const;
	IMPORT_C void ReadDTMFTones(TRequestStatus& aReqStatus, TDes& aTones) const;
	IMPORT_C void ReadDTMFTonesCancel() const;
	IMPORT_C void NotifyDTMFEvent(TRequestStatus& aReqStatus,TDTMFEvent& aEvent) const;
	IMPORT_C void NotifyDTMFEventCancel() const;
	IMPORT_C TInt StartDTMFTone(const TChar& aTone) const;
	IMPORT_C void StartDTMFTone(TRequestStatus& aReqStatus, const TChar& aTone) const;
	IMPORT_C void StartDTMFToneCancel() const;
	IMPORT_C TInt StopDTMFTone() const;
	IMPORT_C void StopDTMFTone(TRequestStatus& aReqStatus) const;
	IMPORT_C void StopDTMFToneCancel() const;
	IMPORT_C TInt SkipDTMFPause() const;
	IMPORT_C void SkipDTMFPause(TRequestStatus& aReqStatus) const;
	IMPORT_C void SkipDTMFPauseCancel() const;

	IMPORT_C CGsmEmergencyNumbersList* RetrieveSimEmergencyNumbersLC();
	IMPORT_C CGsmEmergencyNumbersList* RetrieveStandardEmergencyNumbersLC() const;

	IMPORT_C TInt GetAlternatingCallModeCaps(TAlternatingCallModeCapsFlags& aCaps) const;

	IMPORT_C TInt SetALSLine(const TALSLineSet  aLineSetting) const;
	IMPORT_C void SetALSLine(TRequestStatus& aReqStatus, const TALSLineSet  aLineSetting) const;
	IMPORT_C void SetALSLineCancel() const;
	IMPORT_C TInt GetALSLine(TALSLineGet&  aLineSetting) const;
	IMPORT_C void GetALSLine(TRequestStatus& aReqStatus, TALSLineGet&  aLineSetting) const;
	IMPORT_C void GetALSLineCancel() const;
	IMPORT_C void NotifyALSLineChange(TRequestStatus& aReqStatus, TALSLineGet&  aLineSetting) const;
	IMPORT_C void NotifyALSLineChangeCancel() const;

	IMPORT_C TInt DeactivateAllCCBS() const;
	IMPORT_C void DeactivateAllCCBS(TRequestStatus& aReqStatus) const;
	IMPORT_C void DeactivateAllCCBSCancel() const;
	IMPORT_C TInt GetCcbsStatus(TCcbsStatus& aCcbsStatus, const TCcbsStatusLocation aLocation = ECcbsLocationCache) const;
	IMPORT_C void GetCcbsStatus(TRequestStatus& aReqStatus, TCcbsStatus& aCcbsStatus, const TCcbsStatusLocation aLocation = ECcbsLocationCache) const;
	IMPORT_C void GetCcbsStatusCancel() const;
	IMPORT_C CAdvGsmCcbsRequestList* RetrieveActiveCCBSRequestsLC();
	
	IMPORT_C CAdvGsmLanguageList* RetrievePreferredLanguagesLC();
	IMPORT_C TInt StorePreferredLanguagesL(CAdvGsmLanguageList* aLanguageList) const; 
	IMPORT_C void StorePreferredLanguagesL(TRequestStatus& aReqStatus, CAdvGsmLanguageList* aLanguageList) const;
	IMPORT_C void StorePreferredLanguagesCancel() const;
	IMPORT_C void NotifyPreferredLanguagesChanged(TRequestStatus& aReqStatus) const;
	IMPORT_C void NotifyPreferredLanguagesChangedCancel() const;

	IMPORT_C TInt GetCustomerServiceProfile(TCspFile& aCsp) const;
	IMPORT_C void GetCustomerServiceProfile(TRequestStatus& aReqStatus, TCspFile& aCsp) const;
	IMPORT_C void GetCustomerServiceProfileCancel() const;
	IMPORT_C TInt GetSimServiceTable(TSstFile& aSst) const;
	IMPORT_C void GetSimServiceTable(TRequestStatus& aReqStatus, TSstFile& aSst) const;
	IMPORT_C void GetSimServiceTableCancel() const;

	IMPORT_C TInt GetSubscriberIdNS(TSubscriberId& aId) const;
	IMPORT_C void GetSubscriberIdNS(TRequestStatus& aReqStatus, TSubscriberId& aId) const;
	IMPORT_C void GetSubscriberIdCancel() const;
	
	IMPORT_C void GetMsgWaitingFlag(TRequestStatus& aReqStatus, TNumberOfMsgWaiting& aFlags) const;
	IMPORT_C TInt GetMsgWaitingFlag(TNumberOfMsgWaiting& aFlags) const;
	IMPORT_C void GetMsgWaitingFlagCancel() const; 
	IMPORT_C void SetMsgWaitingFlag(TRequestStatus& aReqStatus, const TNumberOfMsgWaiting& aFlags) const;
	IMPORT_C TInt SetMsgWaitingFlag(const TNumberOfMsgWaiting& aFlags) const;
	IMPORT_C void SetMsgWaitingFlagCancel() const;
	IMPORT_C void MsgWaitingFlagNotification(TRequestStatus& aReqStatus, TNumberOfMsgWaiting& aFlags) const;
	IMPORT_C void MsgWaitingFlagNotificationCancel() const;

private:
	CAdvPhonePtrHolder* iAdvPtrHolder;
protected:
	IMPORT_C virtual void ConstructL();
	IMPORT_C virtual void Destruct();
protected:
	void AdvPhoneGet(const TInt aIpc,TRequestStatus& aReqStatus,TDes8& aDes1,TDes8& aDes2) const;
	void AdvPhoneGet(const TInt aIpc,TRequestStatus& aReqStatus,TDes16& aDes1,TDes16& aDes2) const;
	void AdvPhoneSet(const TInt aIpc,TRequestStatus& aReqStatus,TDes16& aDes1,TDes16& aDes2) const;
	TInt AdvPhoneCancel(const TInt aIpc,const TInt aIpcToCancel) const;
	};

/**/
/* Asynchronous Master List Network Multiple Read*/
/**/

class CAdvGsmRetrieveStaticNetworks : public CActive
	{
public:
	IMPORT_C static CAdvGsmRetrieveStaticNetworks* NewL(RAdvGsmPhone& aPhone);
	IMPORT_C ~CAdvGsmRetrieveStaticNetworks();
/**/
/* Start the multiple read*/
/**/
	IMPORT_C void StartL(const TInt aIndexFrom, const TInt aIndexTo, TRequestStatus& aReqStatus);
/**/
/* Get the results of the search*/
/**/
	IMPORT_C CGsmNetworkList* ReadResults() const;

protected:
	CAdvGsmRetrieveStaticNetworks(RAdvGsmPhone& aPhone);
	void ConstructL();
private:
/**/
/* CActive pure virtual functions*/
/* */
	virtual void DoCancel();
	virtual void RunL();

private:
	enum {
		EIdle,
		EReading
		} iState;

	RAdvGsmPhone& iPhone;
	TRequestStatus* iUserStatus;
	CGsmNetworkList* iResults;
	CBufFlat* iResultsBuf;
	RAdvGsmPhone::TReadPositions iReadPositions;
	TPtr8 iResultsPtr;
	TPtr8 iReadPtr;
	};



/**/
/* Asynchronous Retrieve Preferred Networks*/
/**/
class CAdvGsmRetrievePreferredNetworks : public CActive
	{
public:
	IMPORT_C static CAdvGsmRetrievePreferredNetworks* NewL(RAdvGsmPhone& aPhone);
	IMPORT_C ~CAdvGsmRetrievePreferredNetworks();
/**/
/* Start the retrieval*/
/**/
	IMPORT_C void Start(TRequestStatus& aReqStatus);
/**/
/* Get the results of the retrieval*/
/**/
	IMPORT_C CGsmNetworkList* RetrieveResults() const;

protected:
	CAdvGsmRetrievePreferredNetworks(RAdvGsmPhone& aPhone);
	void ConstructL();
private:
	virtual void StartPhase2L();
/**/
/* CActive pure virtual functions*/
/* */
	void DoCancel();
	void RunL();

private:
	enum {
		EIdle,
		ERetrievePhase1,
		ERetrievePhase2
		} iState;

	RAdvGsmPhone& iPhone;
	TRequestStatus* iUserStatus;
	TInt iNumOfEntries;
	RAdvGsmPhone::TClientId iId;
	CGsmNetworkList* iResults;
	CBufFlat* iResultsBuf;
	TPtr8 iIdPtr;
	TPtr8 iResultsPtr;
	TPtr8 iReadPtr;
	};

/**/
/* Asynchronous Retrieve SIM Emergency Numbers*/
/**/
class CGsmRetrieveSimEmergencyNumbers : public CActive
	{
public:
	IMPORT_C static CGsmRetrieveSimEmergencyNumbers* NewL(RAdvGsmPhone& aPhone);
	IMPORT_C ~CGsmRetrieveSimEmergencyNumbers();
/**/
/* Start the retrieval*/
/**/
	IMPORT_C void Start(TRequestStatus& aReqStatus);
/**/
/* Get the results of the retrieval*/
/**/
	IMPORT_C CGsmEmergencyNumbersList* RetrieveList() const;

protected:
	CGsmRetrieveSimEmergencyNumbers(RAdvGsmPhone& aPhone);
	void ConstructL();
private:
	virtual void StartPhase2L();
/**/
/* CActive pure virtual functions*/
/* */
	void DoCancel();
	void RunL();

private:
	enum {
		EIdle,
		ERetrievePhase1,
		ERetrievePhase2
		} iState;

	RAdvGsmPhone& iPhone;
	TRequestStatus* iUserStatus;
	TInt iNumOfEntries;
	RAdvGsmPhone::TClientId iId;
	CGsmEmergencyNumbersList* iList;
	CBufFlat* iListBuf;
	TPtr8 iIdPtr;
	TPtr8 iListPtr;
	TPtr8 iReadPtr;
	};

/* Emergency Numbers List class*/
template<class T>
class CGsmList;
class CGsmEmergencyNumbersList : public CBase
	{
public:
	IMPORT_C static CGsmEmergencyNumbersList* NewL(TInt aNoOfEntries);
	IMPORT_C ~CGsmEmergencyNumbersList();
/**/
/* Streaming - for use to pass arrays over IPC*/
/**/
	IMPORT_C CBufFlat* StoreLC();
	IMPORT_C void StoreL(TDes8& aDes);
	IMPORT_C void RestoreL(const TDesC8& aBuf);
	IMPORT_C void AddEmptyEntriesL();

	IMPORT_C TInt AddEntryL(const RAdvGsmPhone::TEmergencyNumber aNumber) const;
	
	IMPORT_C TInt EnumerateEntries(TInt& aNoOfEntries) const;
	
	IMPORT_C TInt GetEntry(const TInt aIndex, RAdvGsmPhone::TEmergencyNumber& aNumber ) const; 
	IMPORT_C TInt GetNextEntry(RAdvGsmPhone::TEmergencyNumber& aNumber) const; 
	IMPORT_C TInt GetPreviousEntry(RAdvGsmPhone::TEmergencyNumber& aNumber) const; 
protected:
	CGsmEmergencyNumbersList(TInt aNoOfEntries);
private:
	void ConstructL();
private:
 	CGsmList<RAdvGsmPhone::TEmergencyNumber>* iList;
	TInt		iNoOfEntries;
	};

/**/
/* Asynchronous Retrieve Active CCBS Requests*/
/**/
class CAdvGsmRetrieveActiveCcbs : public CActive
	{
public:
	IMPORT_C static CAdvGsmRetrieveActiveCcbs* NewL(RAdvGsmPhone& aPhone);
	IMPORT_C ~CAdvGsmRetrieveActiveCcbs();
/**/
/* Start the retrieval*/
/**/
	IMPORT_C void Start(TRequestStatus& aReqStatus);
/**/
/* Get the results of the retrieval*/
/**/
	IMPORT_C CAdvGsmCcbsRequestList* RetrieveList() const;

protected:
	CAdvGsmRetrieveActiveCcbs(RAdvGsmPhone& aPhone);
	void ConstructL();
private:
	virtual void StartPhase2L();
/**/
/* CActive pure virtual functions*/
/* */
	void DoCancel();
	void RunL();

private:
	enum {
		EIdle,
		ERetrievePhase1,
		ERetrievePhase2
		} iState;

	RAdvGsmPhone& iPhone;
	TRequestStatus* iUserStatus;
	TInt iNumOfEntries;
	RAdvGsmPhone::TClientId iId;
	CAdvGsmCcbsRequestList* iList;
	CBufFlat* iListBuf;
	TPtr8 iIdPtr;
	TPtr8 iListPtr;
	TPtr8 iReadPtr;
	};

/* Active CCBS Request List class*/
template<class T>
class CGsmList;
class CAdvGsmCcbsRequestList : public CBase
	{
public:
	IMPORT_C static CAdvGsmCcbsRequestList* NewL(TInt aNoOfEntries);
	IMPORT_C ~CAdvGsmCcbsRequestList();
/**/
/* Streaming - for use to pass arrays over IPC*/
/**/
	IMPORT_C CBufFlat* StoreLC();
	IMPORT_C void StoreL(TDes8& aDes);
	IMPORT_C void RestoreL(const TDesC8& aBuf);
	IMPORT_C void AddEmptyEntriesL();

	IMPORT_C TInt AddEntryL(const RAdvGsmPhone::TCcbsRequest aRequest) const;
	
	IMPORT_C TInt EnumerateEntries(TInt& aNoOfEntries) const;
	
	IMPORT_C TInt GetEntry(const TInt aIndex, RAdvGsmPhone::TCcbsRequest& aRequest) const; 
	IMPORT_C TInt GetNextEntry(RAdvGsmPhone::TCcbsRequest& aRequest) const; 
	IMPORT_C TInt GetPreviousEntry(RAdvGsmPhone::TCcbsRequest& aRequest) const; 
protected:
	CAdvGsmCcbsRequestList(TInt aNoOfEntries);
private:
	void ConstructL();
private:
 	CGsmList<RAdvGsmPhone::TCcbsRequest>* iList;
	TInt		iNoOfEntries;
	};

/**/
/* Asynchronous Retrieve Preferred Languages*/
/**/
class CAdvGsmRetrievePrefLanguages : public CActive
	{
public:
	IMPORT_C static CAdvGsmRetrievePrefLanguages* NewL(RAdvGsmPhone& aPhone);
	IMPORT_C ~CAdvGsmRetrievePrefLanguages();
/**/
/* Start the retrieval*/
/**/
	IMPORT_C void Start(TRequestStatus& aReqStatus);
/**/
/* Get the results of the retrieval*/
/**/
	IMPORT_C CAdvGsmLanguageList* RetrieveList() const;

protected:
	CAdvGsmRetrievePrefLanguages(RAdvGsmPhone& aPhone);
	void ConstructL();
private:
	virtual void StartPhase2L();
/**/
/* CActive pure virtual functions*/
/* */
	void DoCancel();
	void RunL();

private:
	enum {
		EIdle,
		ERetrievePhase1,
		ERetrievePhase2
		} iState;

	RAdvGsmPhone& iPhone;
	TRequestStatus* iUserStatus;
	RAdvGsmPhone::TCombinedEntries iNumOfEntries;
	RAdvGsmPhone::TClientId iId;
	CAdvGsmLanguageList* iList;
	CBufFlat* iListBuf;
	TPtr8 iIdPtr;
	TPtr8 iListPtr;
	TPtr8 iReadPtr;
	};

/* Language List class*/
template<class T>
class CGsmList;
class CAdvGsmLanguageList : public CBase
	{
public:
	IMPORT_C static CAdvGsmLanguageList* NewL(TInt aNoOfEntries, TInt aNoOfExtEntries);
	IMPORT_C ~CAdvGsmLanguageList();
/**/
/* Streaming - for use to pass arrays over IPC*/
/**/
	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;
	IMPORT_C CBufFlat* StoreLC();
	IMPORT_C void StoreL(TDes8& aDes);
	IMPORT_C void RestoreL(const TDesC8& aBuf);
	IMPORT_C void AddEmptyEntriesL();

	IMPORT_C TInt AddEntryL(const TUint8 aLanguageCode) const;
	IMPORT_C TInt AddExtEntryL(const TUint16 aLanguageCode) const;
	
	IMPORT_C TInt EnumerateEntries(TInt& aNoOfEntries, TInt& aNoOfExtEntries) const;
	IMPORT_C TInt GetMaxNumEntries(TInt& aMaxNumber, TInt& aMaxExtNumber) const;
	IMPORT_C TInt SetMaxNumEntries(const TInt aMaxNumber, const TInt aMaxExtNumber) const;
	
	IMPORT_C TInt GetEntry(const TInt aIndex, TUint8& aLanguageCode) const; 
	IMPORT_C TInt GetNextEntry(TUint8& aLanguageCode) const; 
	IMPORT_C TInt GetPreviousEntry(TUint8& aLanguageCode) const; 
	IMPORT_C TInt GetExtEntry(const TInt aIndex, TUint16& aLanguageCode) const; 
	IMPORT_C TInt GetNextExtEntry(TUint16& aLanguageCode) const; 
	IMPORT_C TInt GetPreviousExtEntry(TUint16& aLanguageCode) const; 

	IMPORT_C TInt InsertEntryL(const TInt aIndex, const TUint8 aLanguageCode) const;
	IMPORT_C TInt InsertExtEntryL(const TInt aIndex, const TUint16 aLanguageCode) const;
	IMPORT_C TInt ChangeEntry(const TInt aIndex, const TUint8 aLanguageCode) const;
	IMPORT_C TInt ChangeExtEntry(const TInt aIndex, const TUint16 aLanguageCode) const;
	IMPORT_C TInt DeleteEntry(const TInt aIndex) const;
	IMPORT_C TInt DeleteExtEntry(const TInt aIndex) const;

protected:
	CAdvGsmLanguageList(TInt aNoOfEntries, TInt aNoOfExtEntries);
private:
	void ConstructL();
private:
 	CGsmList<TUint8>* iList;
	CGsmList<TUint16>* iExtList;
	TInt		iNoOfEntries;
	TInt		iNoOfExtEntries;
	};

/**/
/*	RAdvGsmPhoneBook*/
/**/

#define ALL_PHONEBOOKS		_L("AllPhoneBooks")
#define BARRED_PHONEBOOK	_L("BarredNumbers")
#define SERVICE_PHONEBOOK	_L("ServiceNumbers")
#define MAILBOX_PHONEBOOK	_L("MailboxNumbers")


class CAdvGsmPhoneBookRead;
class CAdvGsmPhoneBookSearch;
class RAdvGsmPhoneBook : public RGsmPhoneBook
	{
public:
	friend class CAdvGsmPhoneBookSearch;
	friend class CAdvGsmPhoneBookRead;

	IMPORT_C RAdvGsmPhoneBook();
	
	enum  
		{ KPanicNullResultsPtr,
		  KPanicInvalidResultsSize};
/**/
/* Retrieve position of first empty slot*/
/**/
	IMPORT_C TInt GetFirstEmptySlot(TInt& aIndex) const;
	IMPORT_C void GetFirstEmptySlot(TRequestStatus& aReqStatus, TInt& aIndex);
	IMPORT_C void GetFirstEmptySlotCancel() const;
/**/
/* Determine if slot is unused*/
/**/
	IMPORT_C TInt IsSlotUnused(const TInt aIndex, TBool& aUnused) const;
	IMPORT_C void IsSlotUnused(TRequestStatus& aReqStatus, const TInt aIndex, TBool& aUnused);
	IMPORT_C void IsSlotUnusedCancel() const;

/**/
/* Synchronous Phone Book Multiple Read*/
/**/
	IMPORT_C CGsmPhoneBookResults* MultipleReadLC(const TInt aIndexFrom, const TInt aIndexTo);

/**/
/* Advanced Synchronous Search*/
/**/
	enum TTypeOfSearch
		{
		ESearchTypeBasic,
		ESearchTypeAnyText,
		ESearchTypeByNumber
		};

	enum TOrderOfSearch
		{
		ESearchOrderByIndex,
		ESearchOrderAlphaNumeric
		};

	struct TSearchVariety
		{
		TTypeOfSearch iType;
		TOrderOfSearch iOrder;
		};
		
	IMPORT_C CGsmPhoneBookResults* SearchLC(const TSearchVariety& aSearchVariety, const TSearchString& aSearch);

/**/
/* Server/TSY internal struct*/
/**/
	struct TAdvGsmPhoneBookSearch
		{
		TSearchString iSearchString;
		TUint iNumMatchingEntries;
		TOrderOfSearch iOrder;
		};

	struct TReadPositions
		{
		TUint iPosFrom;
		TUint iPosTo;
		};

protected:
	void PhBookAdvGet(const TInt aIpc,TRequestStatus& aReqStatus,TDes8& aDes1,TDes8& aDes2) const;
	void PhBookAdvGet(const TInt aIpc,TRequestStatus& aReqStatus,TDes16& aDes1,TDes16& aDes2) const;
	TInt PhBookAdvCancel(const TInt aIpc,const TInt aIpcToCancel) const;

private:
	RAdvGsmPhoneBook(const RAdvGsmPhoneBook& aPhoneBook);
	TGsmPhoneBookWrite iWrite;
	TPtr8 iWritePtr;
	TPtr8 iDelete;
	TPtrC8 iDeletePin;
	TPtr8 iFirstEntryPtr;
	TPtr8 iUnusedPosPtr;
	TPtr8 iUnusedPtr;
	TInt iIsSlotUnusedIndex;
	TInt iDeleteSlotIndex;
	};

/**/
/*	Advanced asynchronous search*/
/**/
class CAdvGsmPhoneBookSearch : public CGsmPhoneBookSearch
	{
public:
	IMPORT_C static CAdvGsmPhoneBookSearch* NewL(RAdvGsmPhoneBook& aPhoneBook);
	IMPORT_C ~CAdvGsmPhoneBookSearch();

/**/
/* Start the search*/
/**/
	IMPORT_C void Start(const RAdvGsmPhoneBook::TSearchVariety& aSearchVariety, const RGsmPhoneBook::TSearchString& aSearch,TRequestStatus& aReqStatus);

private:
	virtual void StartPhase2L();
/**/
/* CActive pure virtual functions*/
/* */
	virtual void DoCancel();
	virtual void RunL();

protected:
	CAdvGsmPhoneBookSearch(RAdvGsmPhoneBook& aPhoneBook);
private:
	RAdvGsmPhoneBook::TSearchVariety iSearchVariety;
	RAdvGsmPhoneBook& iAdvPhoneBook;
	RAdvGsmPhoneBook::TAdvGsmPhoneBookSearch iAdvSearch;
	};

/**/
/* Asynchronous Phone Book Multiple Read*/
/**/

class CAdvGsmPhoneBookRead : public CActive
	{
public:
	IMPORT_C static CAdvGsmPhoneBookRead* NewL(RAdvGsmPhoneBook& aPhoneBook);
	IMPORT_C ~CAdvGsmPhoneBookRead();
/**/
/* Start the multiple read*/
/**/
	IMPORT_C void StartL(const TInt aIndexFrom, const TInt aIndexTo, TRequestStatus& aReqStatus);
/**/
/* Get the results of the search*/
/**/
	IMPORT_C CGsmPhoneBookResults* ReadResults() const;

protected:
	CAdvGsmPhoneBookRead(RAdvGsmPhoneBook& aPhoneBook);
	void ConstructL();
private:
/**/
/* CActive pure virtual functions*/
/* */
	virtual void DoCancel();
	virtual void RunL();

private:
	enum {
		EIdle,
		EReading
		} iState;

	RAdvGsmPhoneBook& iPhoneBook;
	TRequestStatus* iUserStatus;
	CGsmPhoneBookResults* iResults;
	CBufFlat* iResultsBuf;
	RAdvGsmPhoneBook::TReadPositions iReadPositions;
	TPtr8 iResultsPtr;
	TPtr8 iReadPtr;
	};

/*////*/
/**/
/* CBS Message Abstraction*/
/**/
/*////*/

class TCbs
	{
public:
/**/
/* Messaging Format*/
/**/
	enum TFormat
		{
		EFormatPdu,
		EFormatText		/* this may be extended in the future*/
		};
/**/
/* Messaging Enumerations*/
/**/
	enum {KCbsSize=82};
	enum {KPduSize=88};		
	enum {KCbsStoreSize=108};		
/**/
/* Message Structures*/
/**/
	typedef TBuf8<KPduSize> TPdu;
	typedef TUint8 TDcs;

	struct TCbm
		{
		TUint16 iCbmSerialNumber;
		TUint16 iCbmMessageId;
		TDcs iDcs;
		TUint iPage;
		TUint iPages;
		TBuf8<KCbsSize> iMessage;
		};

/**/
/* Functionality*/
/**/
	IMPORT_C TCbs();
	IMPORT_C TCbs(const TFormat aFormat);
	IMPORT_C TCbs(const TPdu& aPdu);
	IMPORT_C TCbs(const TCbm& aCbm);

	IMPORT_C TCbs::TFormat Format() const;
	IMPORT_C void SetFormat(const TFormat aFormat);
/**/
/* For Pdu message only*/
/**/
	IMPORT_C const TPdu& PduL() const;
	IMPORT_C void SetPduL(const TPdu& aPdu);
/**/
/* For Text message only*/
/**/
	IMPORT_C void SetTextL(const TCbm& aCbm);

	IMPORT_C const TCbs::TCbm& CellBroadcastL() const;
public:
	TInt iIndex;	
private:
	TFormat iFormat;
	TBuf8<KCbsStoreSize> iBuf;
	};

/* Advanced GSM SMS Messaging class, inheriting from the Basic GSM's RSmsMessaging*/

class CAdvMessagingPtrHolder;
class CAdvGsmCbmiList;
class CAdvGsmSmspList;
class RAdvGsmSmsMessaging : public RSmsMessaging
	{
public:
	friend class CAdvGsmRetrieveCbmiList;
	friend class CAdvGsmRetrieveSmsParameters;

	IMPORT_C RAdvGsmSmsMessaging();

	/* Advanced SMS*/
	
	enum TSmsReportCaps
		{
		KCapsDeliverReportFromME=0x00000001,	/* ME automatically sends a SMS-DELIVER-REPORT*/
		KCapsDeliverReportFromClient=0x00000002,/* Client app can send a SMS-DELIVER-REPORT*/
		KCapsSubmitReportToME=0x00000004,		/* ME can receive and process SMS-SUBMIT-REPORT*/
		KCapsSubmitReportToClient=0x00000008	/* Client can receive and process SMS-SUBMIT-REPORT*/
 		};

	struct TSmsReportCapsFlags
		{
		TUint iSmsReportCaps;
		};

	enum TSmsReportResponsibility
		{
		ESmsReportME,
		ESmsReportClient
		};

	struct TSmsReportSetting
		{
		TSmsReportResponsibility iDeliverReports;
		TSmsReportResponsibility iSubmitReports;
		};

	enum TSubmitReportType
		{
		ESmsSubmitReportAck,
		ESmsSubmitReportNack
		};

	struct TSmsSendResult
		{
		TUint iMsgRef;
		TSubmitReportType iType;
		TSms iSubmitReport;
		};

	/* Capabilities for ME and/or Client responsibility for SMS reports*/
	IMPORT_C TInt GetSmsReportCaps(TSmsReportCapsFlags& aCaps) const;
	IMPORT_C TInt GetSmsReportResponsibility(TSmsReportSetting& aReportSetting) const;
	IMPORT_C TInt SetSmsReportResponsibility(const TSmsReportSetting aReportSetting) const;

	/* Receiving SMS-SUBMIT-REPORTS*/
	IMPORT_C TInt SendAdvSmsMessage(const TSms& aMsg, TSmsSendResult& aResult) const;
	IMPORT_C void SendAdvSmsMessage(TRequestStatus& aStatus, const TSms& aMsg, TSmsSendResult& aResult) const;
	IMPORT_C void SendAdvSmsMessageCancel() const;

	/* Sending SMS-DELIVER-REPORTS*/
	IMPORT_C TInt AckSmsMessage(const TSms& aMsg) const; 
	IMPORT_C TInt AckSmsMessage() const;
	IMPORT_C void AckSmsMessage(TRequestStatus& aStatus, const TSms& aMsg) const; 
	IMPORT_C void AckSmsMessage(TRequestStatus& aStatus) const;
	IMPORT_C void AckSmsMessageCancel() const;
	
	IMPORT_C TInt NackSmsMessage(const TSms& aMsg) const;
	IMPORT_C void NackSmsMessage(TRequestStatus& aStatus, const TSms& aMsg) const;
	IMPORT_C void NackSmsMessageCancel() const;

	IMPORT_C TInt RequestSmsResend() const;
	IMPORT_C void RequestSmsResend(TRequestStatus& aStatus) const;
	IMPORT_C void RequestSmsResendCancel() const;

	/* Advanced version of NotifySmsMessageArrived*/
	IMPORT_C void NotifyAdvSmsMessageArrived(TRequestStatus& aReqStatus, TMsgLocation& aMsgLocation) const;
	IMPORT_C void NotifyAdvSmsMessageArrivedCancel() const;

	/* SMS event notification - inform client of events triggered by other clients*/

	enum TSmsEvent
		{
		ESmsSentInfoAvailableSubmit,		/* MO SMS sent, iSms contains SMS-SUBMIT.*/
		ESmsSentInfoAvailableCommand,		/* MO SMS sent, iSms contains SMS-COMMAND.*/
		ESmsSentInfoAvailableSubmitReport,	/* MO SMS sent, iReport contains SMS-SUBMIT-REPORT*/
		ESmsSentInfoNotAvailable			/* MO SMS sent, no other info available*/
		};

	struct TSmsEventInfo
		{
		TSmsEvent iEventType;
		TSmsSendResult iReport;
		TSms iSms;
		};

	IMPORT_C void NotifySmsEvent(TRequestStatus& aReqStatus, TSmsEventInfo& aEvent) const;
	IMPORT_C void NotifySmsEventCancel() const;

	/* Retrieve & Edit SMS Parameters List*/

	enum { KMaxSmspTextSize=50 };

	enum TSmspValidParameters
		{
		KDestinationIncluded=0x01,
		KServiceCentreAddressIncluded=0x02,
		KProtocolIdIncluded=0x04,
		KDcsIncluded=0x08,
		KValidityPeriodIncluded=0x10
 		};

	class TSmspEntry
	{
	public:
		void InternalizeL(RReadStream& aStream);
		void ExternalizeL(RWriteStream& aStream) const;
	public:
		IMPORT_C TSmspEntry();
	public:
		TBuf<KMaxSmspTextSize> iText;
		TUint8	iValidParams;
		TGsmTelNumber iDestination;
		TGsmTelNumber iServiceCentre;
		TUint8	iProtocolId;
		TUint8	iDcs;
		TUint8	iValidityPeriod;
	};

	IMPORT_C TInt StoreSmsParameterListL(CAdvGsmSmspList* aSmspList) const;
	IMPORT_C void StoreSmsParameterListL(TRequestStatus& aReqStatus, CAdvGsmSmspList* aSmspList) const;
	IMPORT_C void StoreSmsParameterListCancel() const;

	IMPORT_C CAdvGsmSmspList* RetrieveSmsParameterListLC();
	IMPORT_C void NotifySmsParameterListChanged(TRequestStatus& aReqStatus) const;
	IMPORT_C void NotifySmsParameterListChangedCancel() const;

	/* Send and Receive USSD Messages*/

	enum {	KUssdMsgSize=182	};
	typedef TBuf8<KUssdMsgSize> TUssdMsg;

	enum TMOUssdMsgType
		{
		EUssdMOCommand,
		EUssdMOReply
		};

	enum TMTUssdMsgType
		{
		EUssdMTNotify,
		EUssdMTRequest,
		EUssdMTReply
		};

	struct TSendUssdMsg
		{
		TMOUssdMsgType iSendType;
		TCbs::TDcs iDcs;
		TUssdMsg iMsg;
		};

	struct TReceiveUssdMsg
		{
		TMTUssdMsgType iReceiveType;
		TCbs::TDcs iDcs;
		TUssdMsg iMsg;
		};

	IMPORT_C TInt SendUssdMessage(const TSendUssdMsg& aMsg) const;
	IMPORT_C void SendUssdMessage(TRequestStatus& aReqStatus, const TSendUssdMsg& aMsg) const;
	IMPORT_C void SendUssdMessageCancel() const;
	
	IMPORT_C TInt ReadUssdMessage(TReceiveUssdMsg& aMsg) const;
	IMPORT_C void ReadUssdMessage(TRequestStatus& aReqStatus, TReceiveUssdMsg& aMsg) const;
	IMPORT_C void ReadUssdMessageCancel() const;

	/* Receive CBS messages*/

	struct TCombinedIdAndDcs
		{
		TInt iIdCount;
		TInt iDcsCount;
		};

	enum TCbsReceiveMode
		{
		ECbsReceiveEnabled,
		ECbsReceiveDisabled
		};

	IMPORT_C void ReadCbsMessage(TRequestStatus& aReqStatus, TCbs& aMsg) const;
	IMPORT_C void ReadCbsMessageCancel() const;
	IMPORT_C void NotifyCbsMessageArrived(TRequestStatus& aReqStatus) const;
	IMPORT_C void NotifyCbsMessageArrivedCancel() const;

	IMPORT_C TInt GetCbsReceiveMode(TCbsReceiveMode& aMode) const;
	IMPORT_C TInt SetCbsReceiveMode(const TCbsReceiveMode aMode) const;
	IMPORT_C void NotifyCbsReceiveModeChanged(TRequestStatus& aReqStatus, TCbsReceiveMode& aMode) const;
	IMPORT_C void NotifyCbsReceiveModeChangedCancel() const;

	/* Retrieve & store CBMI list*/

	IMPORT_C TInt StoreCbmiListL(CAdvGsmCbmiList* aCbmiList) const;
	IMPORT_C void StoreCbmiListL(TRequestStatus& aReqStatus, CAdvGsmCbmiList* aCbmiList) const;
	IMPORT_C void StoreCbmiListCancel() const;

	IMPORT_C CAdvGsmCbmiList* RetrieveCbmiListLC();
	IMPORT_C void NotifyCbmiListChanged(TRequestStatus& aReqStatus) const;
	IMPORT_C void NotifyCbmiListChangedCancel() const;

	/* Server/TSY structs*/
	struct TClientId
		{
		TInt iSessionHandle;
		TInt iSubSessionHandle;
		};

	enum TWhichSmsAck
		{
		ESmsAckWithPdu,
		ESmsAckWithoutPdu
		};

private:
	CAdvMessagingPtrHolder* iAdvPtrHolder;
protected:
	IMPORT_C void ConstructL();
	IMPORT_C void Destruct();
	};

/**/
/* Asynchronous Retrieve Cbmi List*/
/**/
class CAdvGsmRetrieveCbmiList : public CActive
	{
public:
	IMPORT_C static CAdvGsmRetrieveCbmiList* NewL(RAdvGsmSmsMessaging& aMessaging);
	IMPORT_C ~CAdvGsmRetrieveCbmiList();
/**/
/* Start the retrieval*/
/**/
	IMPORT_C void Start(TRequestStatus& aReqStatus);
/**/
/* Get the results of the retrieval*/
/**/
	IMPORT_C CAdvGsmCbmiList* RetrieveResults() const;

protected:
	CAdvGsmRetrieveCbmiList(RAdvGsmSmsMessaging& aMessaging);
	void ConstructL();
private:
	virtual void StartPhase2L();
/**/
/* CActive pure virtual functions*/
/* */
	void DoCancel();
	void RunL();

private:
	enum {
		EIdle,
		ERetrievePhase1,
		ERetrievePhase2
		} iState;

	RAdvGsmSmsMessaging& iSmsMessaging;
	TRequestStatus* iUserStatus;
	RAdvGsmSmsMessaging::TCombinedIdAndDcs iNumOfEntries;
	RAdvGsmSmsMessaging::TClientId iId;
	CAdvGsmCbmiList* iResults;
	CBufFlat* iResultsBuf;
	TPtr8 iIdPtr;
	TPtr8 iResultsPtr;
	TPtr8 iReadPtr;
	};

/**/
/* Async retrieval of SMS Parameters*/
/**/

class CAdvGsmRetrieveSmsParameters : public CActive
	{
public:
	IMPORT_C static CAdvGsmRetrieveSmsParameters* NewL(RAdvGsmSmsMessaging& aMessaging);
	IMPORT_C ~CAdvGsmRetrieveSmsParameters();
/**/
/* Start the retrieval*/
/**/
	IMPORT_C void Start(TRequestStatus& aStatus);
/**/
/* Get the results of the retrieval*/
/**/
	IMPORT_C CAdvGsmSmspList* RetrieveList() const;

protected:
	IMPORT_C CAdvGsmRetrieveSmsParameters(RAdvGsmSmsMessaging& aMessaging);
	IMPORT_C void ConstructL();

	IMPORT_C virtual void StartPhase2L();
/**/
/* CActive pure virtual functions*/
/* */
	IMPORT_C virtual void DoCancel();
private:
	IMPORT_C virtual void RunL();

protected:
	enum {
		EIdle,
		ERetrievePhase1,
		ERetrievePhase2
		} iState;

	RAdvGsmSmsMessaging& iMessaging;
	TRequestStatus* iUserStatus;
	TInt iNumOfEntries;
	RAdvGsmSmsMessaging::TClientId iId;
	CAdvGsmSmspList* iResults;
	CBufFlat* iResultsBuf;
	TPtr8 iIdPtr;
	TPtr8 iResultsPtr;
	TPtr8 iReadPtr;
	};

/**/
/* Advanced Message Storage class, inheriting from the Basic GSM's RSmsStorage*/
/**/
class CAdvStoragePtrHolder;
class RAdvGsmSmsStorage : public RSmsStorage
	{
public:
	IMPORT_C RAdvGsmSmsStorage();

	/* Advanced SMS storage*/
	IMPORT_C TInt SendMessageFromStorage(const TInt aIndex, TUint& aMsgRef) const;
	IMPORT_C void SendMessageFromStorage(TRequestStatus& aReqStatus, const TInt aIndex, TUint& aMsgRef) const;
	IMPORT_C TInt SendMessageFromStorage(const TInt aIndex, TUint& aMsgRef, const TGsmTelNumber& aTelNumber) const;
	IMPORT_C void SendMessageFromStorage(TRequestStatus& aReqStatus, const TInt aIndex, TUint& aMsgRef, const TGsmTelNumber& aTelNumber) const;
	IMPORT_C void SendMessageFromStorageCancel() const;

	struct TCbsMsgStoreEntry
		{
		TCbs iMsg;
		TStatus iStatus;
		};

	IMPORT_C TInt ReadCbs(const TInt aIndex, TCbsMsgStoreEntry& aMsgEntry) const;
	IMPORT_C void ReadCbs(TRequestStatus& aReqStatus, const TInt aIndex, TCbsMsgStoreEntry& aMsgEntry) const;
	IMPORT_C void ReadCbsCancel() const;

	/* Server/TSY structs*/
	enum TWhichSendFromStorage
		{
		ESendToOldTelNumber,
		ESendToNewTelNumber
		};

	struct TSendFromStorage
		{
		TWhichSendFromStorage iWhichSend;
		TInt iIndex;
		TGsmTelNumber iNumber;
		};

private:
	CAdvStoragePtrHolder* iAdvPtrHolder;
protected:
	IMPORT_C void ConstructL();
	IMPORT_C void Destruct();
	};


/* Cell Broadcast Message Identifier class*/
template<class T>
class CGsmList;

class CAdvGsmCbmiList : public CBase
	{
public:
	IMPORT_C static CAdvGsmCbmiList* NewL(TInt aNoOfIdEntries,TInt aNoOfDcsEntries);
	IMPORT_C ~CAdvGsmCbmiList();
/**/
/* Streaming - for use to pass arrays over IPC*/
/**/
	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;
	IMPORT_C CBufFlat* StoreLC();
	IMPORT_C void StoreL(TDes8& aDes);
	IMPORT_C void RestoreL(const TDesC8& aBuf);
	IMPORT_C void AddEmptyEntriesL();

	enum TCbmiMode {
		ECBMessageAllowed,
		ECBMessageNotAllowed
		} ;

	IMPORT_C TInt GetMode(TCbmiMode& aMode) const;
	IMPORT_C TInt SetMode(const TCbmiMode aMode);

	IMPORT_C TInt AddSingleIdEntryL(const TUint16 aId) const;
	IMPORT_C TInt AddRangeIdEntryL(const TUint16 aStartId, const TUint16 aEndId) const;
	
	IMPORT_C TInt AddSingleDcsEntryL(const TCbs::TDcs& aDcs) const;
	IMPORT_C TInt AddRangeDcsEntryL(const TCbs::TDcs& aStartDcs,const TCbs::TDcs& aEndDcs) const;
	
	IMPORT_C TInt EnumerateEntries(TInt& aNoOfId, TInt& aNoOfDcs) const;
	IMPORT_C TInt GetMaxNumEntries(TInt& aMaxNoOfId, TInt& aMaxNoOfDcs) const;
	IMPORT_C TInt SetMaxNumEntries(TInt aMaxNoOfId, TInt aMaxNoOfDcs) const;
	
	IMPORT_C TInt GetIdEntry(const TInt aIndex, TUint16& aId ) const; 
	IMPORT_C TInt GetNextIdEntry(TUint16& aId) const; 
	IMPORT_C TInt GetPreviousIdEntry(TUint16& aId) const; 
	
	IMPORT_C TInt GetDcsEntry(const TInt aIndex, TCbs::TDcs& aDcs ) const; 
	IMPORT_C TInt GetNextDcsEntry(TCbs::TDcs& aDcs ) const; 
	IMPORT_C TInt GetPreviousDcsEntry(TCbs::TDcs& aDcs ) const; 
	
	IMPORT_C TInt DeleteIdEntry(TUint16 aId) const;
	IMPORT_C TInt DeleteDcsEntry(TCbs::TDcs aDcs) const;

protected:
	CAdvGsmCbmiList(TInt aNoOfIdEntries,TInt aNoOfDcsEntries);
private:
	void ConstructL();
private:
	TInt iNoOfIdEntries;
	TInt iNoOfDcsEntries;
	TCbmiMode	iMode;

	CGsmList<TUint16>* iIdList;
	CGsmList<TCbs::TDcs>* iDcsList;
	};

/**/
/* SMS Parameters List class*/
/**/
template<class T>
class CGsmList;

class CAdvGsmSmspList : public CBase
{
public:
	IMPORT_C static CAdvGsmSmspList* NewL(TInt aNoOfEntries);
	IMPORT_C ~CAdvGsmSmspList ();

	IMPORT_C CBufFlat* StoreLC();
	IMPORT_C void StoreL(TDes8& aDes);
	IMPORT_C void RestoreL(const TDesC8& aBuf);
	IMPORT_C void AddEmptyEntriesL();
/**/
/* Accessing results*/
/* */
	IMPORT_C TInt GetNextEntry(RAdvGsmSmsMessaging::TSmspEntry& aEntry) const;
	IMPORT_C TInt GetPreviousEntry(RAdvGsmSmsMessaging::TSmspEntry& aEntry) const;
	IMPORT_C TInt GetEntry(const TInt aIndex, RAdvGsmSmsMessaging::TSmspEntry& aEntry) const;
/**/
/* Adding an entry */
/**/
	IMPORT_C TInt AddEntryL(const RAdvGsmSmsMessaging::TSmspEntry& aEntry) const;
	IMPORT_C TInt InsertEntryL(const TInt aIndex, const RAdvGsmSmsMessaging::TSmspEntry& aEntry) const;
	IMPORT_C TInt ChangeEntry(const TInt aIndex, const RAdvGsmSmsMessaging::TSmspEntry& aEntry) const;
	IMPORT_C TInt DeleteEntry(const TInt aIndex) const;
/**/
/* Counting entries*/
/**/
	IMPORT_C TInt EnumerateEntries(TInt& aCount) const;
	IMPORT_C TInt GetMaxNumEntries(TInt& aMaxNumber) const;
	IMPORT_C TInt SetMaxNumEntries(TInt aMaxNumber) const;

protected:
	CAdvGsmSmspList(TInt aNoOfEntries);
private:
	void ConstructL();

private:
	TInt iNoOfEntries;
	CGsmList<RAdvGsmSmsMessaging::TSmspEntry>* iList;
};

#endif
