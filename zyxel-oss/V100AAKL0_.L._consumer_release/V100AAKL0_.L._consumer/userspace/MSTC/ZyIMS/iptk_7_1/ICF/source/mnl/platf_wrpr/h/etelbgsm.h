/* ETELBGSM.H*/
/**/
/* Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.*/
/**/
/* ETel Basic Gsm API*/

#if !defined(__ETELBGSM_H__)
#define __ETELBGSM_H__

#if !defined(__ETEL_H__)
#include "ETEL.h"
#endif

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__S32MEM_H__)
#include <s32mem.h>
#endif

const TInt KETelExtBasicGsm					=1000;
const TInt KETelMixinBasicPhoneId			=KETelExtBasicGsm+1;
const TInt KETelMixinBasicPhoneBookSupport	=KETelExtBasicGsm+2;
const TInt KETelMixinBasicPhoneSignalAndBer	=KETelExtBasicGsm+3;
const TInt KETelMixinBasicPhoneNetwork		=KETelExtBasicGsm+4;
const TInt KETelMixinBasicPhoneBatteryPower	=KETelExtBasicGsm+5;
const TInt KETelMixinBasicPhoneIndicator	=KETelExtBasicGsm+6;
const TInt KETelMixinBasicPhoneClockAndAlarm=KETelExtBasicGsm+7;


const TUint KGsmMaxTelNumberSize=100;
struct TGsmTelNumber
	{
	TUint iTypeOfNumber;
	TBuf<KGsmMaxTelNumberSize> iTelNumber;
	};

const TUint KGsmPasswordSize=30;			/* Maximum length of longest password*/
typedef TBuf<KGsmPasswordSize> TGsmPassword;

/**/
/**/
/* RBasicGsmCall*/
/**/
/**/
class CBasicPtrHolder;
class RBasicGsmCall : public RCall
	{
public:
	IMPORT_C RBasicGsmCall();
/**/
/* Basic Gsm Call Caps*/
/**/
	enum TBearerCaps
		{
		KCapsBearerSpeedAutobauding=0x00000001,
		KCapsBearerSpeedV21_300=0x00000002,
		KCapsBearerSpeedV22_1200=0x00000004,
		KCapsBearerSpeedV23_1200_75=0x00000008,
		KCapsBearerSpeedV22bis_2400=0x00000010,
		KCapsBearerSpeedV26ter_2400=0x00000020,
		KCapsBearerSpeedV32_4800=0x00000040,
		KCapsBearerSpeedV32_9600=0x00000080,
		KCapsBearerSpeedUnknown_NetworkSpecific=0x00000100,
		KCapsBearerSpeedV32bis_7200=0x00000200,
		KCapsBearerSpeedV32bis_12000=0x00000400,
		KCapsBearerSpeedV32bis_14400=0x00000800,
		KCapsBearerSpeedV34_9600=0x00001000,
		KCapsBearerSpeedV34_12000=0x00002000,
		KCapsBearerSpeedV34_14400=0x00004000,
		KCapsBearerSpeedV110_300=0x00008000,
		KCapsBearerSpeedV110_1200=0x00010000,
		KCapsBearerSpeedV110_2400=0x00020000,
		KCapsBearerSpeedV110_4800=0x00040000,
		KCapsBearerSpeedV110_9600=0x00080000,
		KCapsBearerSpeedV110_12000=0x00100000,
		KCapsBearerSpeedV110_14400=0x00200000,
		KCapsBearerSpeedV120_1200=0x00400000,
		KCapsBearerSpeedV120_2400=0x00800000,
		KCapsBearerSpeedV120_4800=0x01000000,
		KCapsBearerSpeedV120_9600=0x02000000,
		KCapsBearerSpeedV120_14400=0x04000000,
		KCapsBearerSpeedUnknown=0x40000000,
		KCapsBearerSpeedExtensions=0x80000000
		};

	enum
		{
		KCapsBearerNameAsynchronousModem=0x00000001,
		KCapsBearerNameSynchronousModem=0x00000002,
		KCapsBearerNamePADAccessAsynchronous=0x00000004,
		KCapsBearerNamePacketAccessSynchronous=0x00000008,
		KCapsBearerNameDataCircuitAsynchronousRdi=0x00000010,
		KCapsBearerNameDataCircuitSynchronousRdi=0x00000020,
		KCapsBearerNamePADAccessAsynchronousRdi=0x00000040,
		KCapsBearerNamePacketAccessSynchronousRdi=0x00000080,
		KCapsBearerNameUnknown=0x40000000,
		KCapsBearerNameExtensions=0x80000000
		};

	enum
		{
		KCapsBearerCETransparent=0x00000001,
		KCapsBearerCENonTransparent=0x00000002,
		KCapsBearerCETransparentPreferred=0x00000004,
		KCapsBearerCENonTransparentPreferred=0x00000008,
		KCapsBearerCEUnknown=0x40000000,
		KCapsBearerCEExtensions=0x80000000
		};

	struct TRlpParamRanges
		{
		TUint iWsMin;
		TUint iWsMax;
		TUint iMwsMin;
		TUint iMwsMax;
		TUint iT1Min;
		TUint iT1Max;
		TUint iN2Min;
		TUint iN2Max;
		TUint iT4Min;
		TUint iT4Max;
		};

	struct TCaps
		{
		TUint iBearerSpeed;
		TUint iBearerName;
		TUint iBearerCe;
		TRlpParamRanges iRlpParams;
		};

	IMPORT_C virtual TInt GetBasicGsmCaps(TCaps& aCaps) const;
	IMPORT_C virtual void NotifyBasicGsmCaps(TRequestStatus& aStatus, TCaps& aCaps);
	IMPORT_C virtual void NotifyBasicGsmCapsCancel() const;

/**/
/* Basic Gsm Call Parameters*/
/**/
	enum TBearerSpeed
		{
		EBearerSpeedAutobauding,
		EBearerSpeedV21_300,
		EBearerSpeedV22_1200,
		EBearerSpeedV23_1200_75,
		EBearerSpeedV22bis_2400,
		EBearerSpeedV26ter_2400,
		EBearerSpeedV32_4800,
		EBearerSpeedV32_9600,
		EBearerSpeedUnknown_NetworkSpecific,
		EBearerSpeedV32bis_7200,
		EBearerSpeedV32bis_12000,
		EBearerSpeedV32bis_14400,
		EBearerSpeedV34_9600,
		EBearerSpeedV34_12000,
		EBearerSpeedV34_14400,
		EBearerSpeedV34_19200,
		EBearerSpeedV34_28800,
		EBearerSpeedV110_300,
		EBearerSpeedV110_1200,
		EBearerSpeedV110_2400,
		EBearerSpeedV110_4800,
		EBearerSpeedV110_9600,
		EBearerSpeedV110_12000,
		EBearerSpeedV110_14400,
		EBearerSpeedV110_19200,
		EBearerSpeedV110_28800,
		EBearerSpeedV110_38400,
		EBearerSpeedV110_48000,
		EBearerSpeedV110_56000,
		EBearerSpeedV120_1200,
		EBearerSpeedV120_2400,
		EBearerSpeedV120_4800,
		EBearerSpeedV120_9600,
		EBearerSpeedV120_14400,
		EBearerSpeedV120_19200,
		EBearerSpeedV120_28800,
		EBearerSpeedV120_38400,
		EBearerSpeedV120_48000,
		EBearerSpeedV120_56000,
		EBearerSpeedBitTransparent_56000,
		EBearerSpeedBitTransparent_64000
		};

	enum TBearerName
		{
		EBearerNameAsynchronousModem,
		EBearerNameSynchronousModem,
		EBearerNamePADAccessAsynchronous,
		EBearerNamePacketAccessSynchronous,
		EBearerNameDataCircuitAsynchronousRdi,
		EBearerNameDataCircuitSynchronousRdi,
		EBearerNamePADAccessAsynchronousRdi,
		EBearerNamePacketAccessSynchronousRdi
		};

	enum TBearerCE
		{
		EBearerCETransparent,
		EBearerCENonTransparent,
		EBearerCETransparentPreferred,
		EBearerCENonTransparentPreferred
		};

	struct TBearer
		{
		TBearerSpeed iSpeed;
		TBearerName iName;
		TBearerCE iCe;
		};

	struct TRlpParams
		{
		TUint iIws;
		TUint iMws;
		TUint iT1;
		TUint iN2;
		TUint iT4;
		};

	class TCallParams : public RCall::TCallParams
		{
	public:
		IMPORT_C TCallParams();
		IMPORT_C TCallParams(const TBearer& aBearer,const TRlpParams& aRlpParams);
		TBearer iBearer;
		TRlpParams iRlpParams;
		};

	typedef TPckg<TCallParams> TCallParamsPckg;
/**/
/*	General notification information*/
/**/
	class TCallInfo : public RCall::TCallInfo
		{
	public:
		TBearer iBearer;
	public:
		IMPORT_C TCallInfo();
		};

	IMPORT_C virtual TInt GetInfoBasicGsm(TCallInfo& aCallInfo) const;
	
/**/
/*	HSCSD Functionality*/
/**/

/*	HSCSD Capability Information*/
	enum THscsdBearerCaps
		{
		KCapsBearerSpeedV34_19200=0x00000001,
		KCapsBearerSpeedV34_28800=0x00000002,
		KCapsBearerSpeedV110_19200=0x00000004,
		KCapsBearerSpeedV110_28800=0x00000008,
		KCapsBearerSpeedV110_38400=0x00000010,
		KCapsBearerSpeedV110_48000=0x00000020,
		KCapsBearerSpeedV110_56000=0x00000040,
		KCapsBearerSpeedV120_19200=0x00000080,
		KCapsBearerSpeedV120_28800=0x00000100,
		KCapsBearerSpeedV120_38400=0x00000200,
		KCapsBearerSpeedV120_48000=0x00000400,
		KCapsBearerSpeedV120_56000=0x00000800,
		KCapsBearerSpeedBitTransparent_56000=0x00001000,
		KCapsBearerSpeedBitTransparent_64000=0x00002000
		};

	enum TAiurCodingsCaps
		{
		EAiurCodings48=0x00000001,
		EAiurCodings96=0x00000004,
		EAiurCodings144=0x00000008
		};

	struct THscsdCaps
		{
		TUint iHscsdBearerSpeed;		/* Logical OR of THscsdBearerCaps values*/
		TInt iMClass;				/* Detailed in GSM 05.02 V6.4.1 Appendix B*/
		TInt iMaxRxTimeSlots;
		TInt iMaxTxTimeSlots;
		TInt iTotalRxTxTimeSlots;
		TUint iCodingCaps;			/* Logical OR of TAiurCodings values*/
		};

	IMPORT_C virtual TInt GetHscsdCaps(THscsdCaps& aHscsdCaps) const;
	IMPORT_C virtual void NotifyHscsdGsmCaps(TRequestStatus& aStatus, THscsdCaps& aHscsdCaps) const;
	IMPORT_C virtual void NotifyHscsdGsmCapsCancel() const;

/**/
/* Statically Configure HSCSD call parameters*/
/*		(will not work during a call)*/
/**/
	enum TAiur
		{
		EAiurBps9600,
		EAiurBps14400,
		EAiurBps19200,
		EAiurBps28800,
		EAiurBps38400,
		EAiurBps43200,
		EAiurBps57600
		};

	struct THscsdParams
		{
		TAiur iAiur;
		TInt iRequestedTimeSlots;
		TInt iMaxTimeSlots;
		TUint iCodings;			/* Logical OR of TAiurCodings values*/
								/* Note that iCodings=0x0 indicates that all*/
								/* supported codings are acceptable.*/
		};

	IMPORT_C virtual TInt SetHscsdParams(const THscsdParams& aHSCSDParams) const;
	IMPORT_C virtual TInt GetHscsdParams(THscsdParams& aHSCSDParams) const;

/**/
/* Retrieve current call information*/
/**/
	struct THscsdDynamicParams
		{
		TAiur iAiur;
		TInt iRxTimeSlots;
		TInt iTxTimeSlots;
		TUint iCodings; 		/* Logical OR of TAiurCodings values*/
		};

	IMPORT_C virtual TInt GetDynamicHscsdParams(THscsdDynamicParams& aHSCSDParams) const;
	IMPORT_C virtual void NotifyDynamicHscsdParams(TRequestStatus& aStatus, THscsdDynamicParams& aHSCSDParams) const;
	IMPORT_C virtual void NotifyDynamicHscsdParamsCancel() const;

/**/
/* Request a dynamic change of HSCSD parameters*/
/**/
	IMPORT_C virtual TInt SetDynamicHscsdParams(const THscsdDynamicParams& aHSCSDParams) const;
	IMPORT_C virtual void SetDynamicHscsdParams(TRequestStatus& aStatus, const THscsdDynamicParams& aHSCSDParams) const;
	IMPORT_C virtual void SetDynamicHscsdParamsCancel() const;

private:
	CBasicPtrHolder* iBasicPtrHolder;
protected:
	IMPORT_C virtual void ConstructL();
	IMPORT_C virtual void Destruct();
	};

/**/
/**/
/* TGsmDateTime*/
/**/
/**/
class TGsmDateTime : public TDateTime
	{
public:
	IMPORT_C TGsmDateTime();
	IMPORT_C TGsmDateTime(TInt aTimeZone,TInt aYear,TMonth aMonth,TInt aDay,TInt aHour,TInt aMinute, TInt aSecond,TInt aMicroSecond);
	IMPORT_C void SetTimeZone(const TInt aTimeZone);
	IMPORT_C TInt TimeZone() const;
private:
	TInt iTimeZone;
	};

/**/
/* Phone ID Information*/
/**/

class MBasicGsmPhoneId
	{
public:
	enum {	KPhoneManufacturerIdSize=50	};
	enum {	KPhoneModelIdSize=50	};
	enum {	KPhoneRevisionIdSize=50	};
	enum {	KPhoneSerialNumberSize=50	};

	struct TId
		{
		TBuf<KPhoneManufacturerIdSize> iManufacturerId;
		TBuf<KPhoneModelIdSize> iModelId;
		TBuf<KPhoneRevisionIdSize> iRevisionId;
		TBuf<KPhoneSerialNumberSize> iSerialNumber;
		};

	virtual TInt GetGsmPhoneId(TId& aId) const=0;
	virtual void IdFreeSlot1() const=0;
	virtual void IdFreeSlot2() const=0;
	virtual void IdFreeSlot3() const=0;
	virtual void IdFreeSlot4() const=0;
protected:
	MBasicGsmPhoneId();
	};

/**/
/* Phone Book Support*/
/**/
class MBasicGsmPhoneBookSupport
	{
public:
	struct TPhoneBookInfo
		{
		TInt iUsed;
		TInt iTotal;
		TBool iWriteAccess;
		TBool iRestrictedWriteAccess;
		TName iName;
		TInt iMaxNumberLength;
		TInt iMaxTextLength;
		};

	virtual TInt EnumeratePhoneBooks(TInt& aPhoneBookCount) const=0;
	virtual TInt GetPhoneBookInfo(const TInt aIndex, TPhoneBookInfo& aInfo) const=0;
	virtual void GetPhoneBookInfo(TRequestStatus& aStatus, const TInt aIndex, TPhoneBookInfo& aInfo) const=0;
	virtual void GetPhoneBookInfoCancel() const=0;
	
	virtual void PhBookSupportFreeSlot3() const=0;
	virtual void PhBookSupportFreeSlot4() const=0;
protected:
	MBasicGsmPhoneBookSupport();
	};
	
/**/
/* Signal Strength and Bit Error Rate Information*/
/**/
class MBasicGsmPhoneSignalandBer
	{
public:
	virtual TInt GetSignalStrength(TInt32& aSignalStrength) const=0;
	virtual void GetSignalStrength(TRequestStatus& aStatus, TInt32& aSignalStrength)=0;
	virtual void GetSignalStrengthCancel() const=0;
	virtual void SignalStrengthNotification(TRequestStatus& aStatus,TInt32& aSignalStrength)=0;
	virtual void SignalStrengthNotificationCancel() const=0;
	virtual TInt GetBer(TUint& aBer) const=0;
	virtual void GetBer(TRequestStatus& aStatus, TUint& aBer)=0;
	virtual void GetBerCancel() const=0;
	virtual void BerNotification(TRequestStatus& aStatus,TUint& aBer)=0;
	virtual void BerNotificationCancel() const=0;

	enum TSignalStrengthAndBerCaps
		{
		KCapsSignalStrength=0x00000001,
		KCapsSignalBer=0x00000002,
		KCapsSignalStrengthUpdate=0x00000004,
		KCapsSignalBerUpdate=0x00000008
		};

	struct TSignalAndBerCapsFlags
		{
		TUint iSignalCaps;
		};

	virtual TInt GetSignalCaps(TSignalAndBerCapsFlags& aCaps) const=0;
	virtual void NotifySignalCaps(TRequestStatus& aStatus, TSignalAndBerCapsFlags& aCaps)=0;
	virtual void NotifySignalCapsCancel() const=0;

	virtual void SignalFreeSlot1() const=0;
	virtual void SignalFreeSlot2() const=0;
	virtual void SignalFreeSlot3() const=0;
	virtual void SignalFreeSlot4() const=0;

protected:
	MBasicGsmPhoneSignalandBer();
	};

/**/
/*	Network support*/
/**/
class CGsmNetworkList;
class MBasicGsmPhoneNetwork
	{
public:
	enum
		{
		KShortNetworkNameSize=20,
		KLongNetworkNameSize=30
		};

	enum TNetworkStatus
		{
		ENetStatUnknown,
		ENetStatAvailable,
		ENetStatCurrent,
		ENetStatForbidden,
		ENetStatNotApplicable
		};

	class TBscNetworkId	
		{
	public:
		IMPORT_C void InternalizeL(RReadStream& aStream);
		IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
	public:
		TUint iMCC;
		TUint iMNC;
		};

	enum TNetworkSetMode
		{
		ENetworkSetModeAutomatic,
		ENetworkSetModeManual
		};

	class TNetworkInfo
		{
	public:
		void InternalizeL(RReadStream& aStream);
		void ExternalizeL(RWriteStream& aStream) const;
	public:
		IMPORT_C TNetworkInfo();
	public:
		TBscNetworkId iId;
		TNetworkStatus iStatus;
		TBuf<KShortNetworkNameSize> iShortName;
		TBuf<KLongNetworkNameSize> iLongName;
		};

	enum TRegistrationStatus
		{
		ENotRegisteredNotSearching,
		ERegisteredOnHomeNetwork,
		ENotRegisteredSearching,
		ERegistrationDenied,
		EUnknown,
		ERegisteredRoaming
		};

	struct TCurrentNetworkInfo
		{
		TNetworkInfo iNetworkInfo;		
		TUint iLocationAreaCode;
		TUint iCellId;
		};

	virtual TInt EnumerateDetectedNetworks(TInt& aNetworkCnt) const=0;	
	virtual void EnumerateDetectedNetworks(TRequestStatus& aStatus, TInt& aNetworkCnt)=0;
	virtual void EnumerateDetectedNetworksCancel() const=0;
	virtual TInt GetDetectedNetworkInfo(const TInt aIndex,TNetworkInfo& aNetworkInfo) const=0; 
	virtual void GetDetectedNetworkInfo(TRequestStatus& aStatus, const TInt aIndex,TNetworkInfo& aNetworkInfo)=0; 
	virtual void GetDetectedNetworkInfoCancel() const=0; 

	virtual CGsmNetworkList* RetrieveDetectedNetworksLC()=0;

	virtual TInt SetNetworkMode(const TNetworkSetMode aMode) const=0;
	virtual void SetNetworkMode(TRequestStatus& aStatus,const TNetworkSetMode aMode)=0;
	virtual void SetNetworkModeCancel() const=0;
	virtual TInt SetManualNetwork(const TBscNetworkId aNetworkId) const=0;
	virtual void SetManualNetwork(TRequestStatus& aStatus,const TBscNetworkId aNetworkId)=0;
	virtual void SetManualNetworkCancel() const=0;

	virtual TInt GetCurrentNetworkInfo(TCurrentNetworkInfo& aNetworkInfo) const=0;
	virtual void NotifyChangeOfCurrentNetworkNS(TRequestStatus& aStatus,TCurrentNetworkInfo& aNetworkInfo)=0;
	virtual void NotifyChangeOfCurrentNetworkCancel() const=0;

	virtual TInt GetNetworkRegistrationStatus(TRegistrationStatus& aRegistrationStatus) const=0;
	virtual void NotifyChangeOfNetworkRegistrationStatus(TRequestStatus& aStatus,TRegistrationStatus& aRegistrationStatus)=0;
	virtual void NotifyChangeOfNetworkRegistrationStatusCancel() const=0;

	virtual TInt GetNetworkSearchMode(TNetworkSetMode& aMode) const=0;
	virtual void NotifyChangeOfNetworkSearchMode(TRequestStatus& aStatus,TNetworkSetMode& aMode)=0;
	virtual void NotifyChangeOfNetworkSearchModeCancel() const=0;

	enum TNetworkInfoCaps
		{
		KCapsNetworkInfoAvailable=0x00000001,
		KCapsNetworkShortName=0x00000002,
		KCapsNetworkLongName=0x00000004,
		KCapsNetworkCurrentInfoAvailable=0x00000008,
		KCapsNetworkSetModeSupported=0x00000010
		};

	struct TNetworkCapsFlags
		{
		TUint iNetworkInfoCaps;
		};

	virtual TInt GetNetworkInfoCaps(TNetworkCapsFlags& aCaps) const=0;

	virtual void NetworkFreeSlot1() const=0;
	virtual void NetworkFreeSlot2() const=0;
	virtual void NetworkFreeSlot3() const=0;
	virtual void NetworkFreeSlot4() const=0;

	struct TGsmNetworkClientId
		{
		TInt iSessionHandle;
		TInt iSubSessionHandle;
		};

protected:
	MBasicGsmPhoneNetwork();
	};
/**/
/* Battery and Power Information*/
/**/
class MBasicGsmPhoneBatteryAndPower
	{
public:
	enum TBatteryStatus
		{
		EPoweredByBattery,
		EBatteryConnectedButExternallyPowered,
		ENoBatteryConnected,
		EPowerFault
		};

	struct TBatteryInfo
		{
		TBatteryStatus iStatus;
		TUint iChargeLevel;
		};

	virtual TInt GetBatteryInfo(TBatteryInfo& aBatteryInfo) const=0;
	virtual void GetBatteryInfo(TRequestStatus& aStatus, TBatteryInfo& aBatteryInfo)=0;
	virtual void GetBatteryInfoCancel() const=0;
	virtual void BatteryInfoNotification(TRequestStatus& aStatus,TBatteryInfo& aBatteryInfo)=0;
	virtual void BatteryInfoNotificationCancel() const=0;

	enum TBatteryPowerCaps
		{
		KCapsBatteryInfoAvailable=0x00000001,
		KCapsBatteryInfoUpdatesAvailable=0x00000002
		};

	struct TBatteryAndPowerCapsFlags
		{
		TUint iBatteryInfoCaps;
		};

	virtual TInt GetBatteryPowerCaps(TBatteryAndPowerCapsFlags& aCaps) const=0;

	virtual void BatteryFreeSlot1() const=0;
	virtual void BatteryFreeSlot2() const=0;
	virtual void BatteryFreeSlot3() const=0;
	virtual void BatteryFreeSlot4() const=0;

protected:
	MBasicGsmPhoneBatteryAndPower();
	};

/**/
/* Phone Indicator Access*/
/**/
class MBasicGsmPhoneIndicator
	{
public:
	enum TIndicatorType
		{
		EIndBatteryCharge,
		EIndSignalQuality,
		EIndServiceAvail,
		EIndSounderActivity,
		EIndMessageReceived,
		EIndCallInProgress,
		EIndTxActivatedByVoiceActivity,
		EIndRoamingIndicator,
		EIndSmsStorageFull
		};

	struct TIndicatorInfo
		{
		TIndicatorType iIndicator;
		TUint iValue;
		};

	virtual void IndicatorNotification(TRequestStatus& aStatus,TIndicatorInfo& aIndicator)=0;
	virtual void IndicatorNotificationCancel() const=0;
	virtual TInt GetIndicator(const TIndicatorType aIndicator,TUint& aValue) const=0;

	enum TPhoneEquipmentIndicatorCaps
		{
		KCapsEquipmentIndicatorCapture=0x00000001
		};

	struct TIndicatorCapsFlags
		{
		TUint iIndicatorCapture;
		};

	virtual TInt GetIndicatorCaptureCaps(TIndicatorCapsFlags& aCaps) const=0;

	virtual void IndicatorFreeSlot1() const=0;
	virtual void IndicatorFreeSlot2() const=0;
	virtual void IndicatorFreeSlot3() const=0;
	virtual void IndicatorFreeSlot4() const=0;

protected:
	MBasicGsmPhoneIndicator();
	};

/**/
/* Phone Clock and Alarm Access*/
/**/
class MBasicGsmPhoneClockAndAlarm
	{
public:
	enum { KAlarmTextSize=256 };
	struct TAlarm
		{
		TGsmDateTime iDateTime;
		TBuf<KAlarmTextSize> iText;
		};

	virtual TInt ReadClock(TGsmDateTime& aGsmDateTime) const=0;
	virtual void ReadClock(TRequestStatus& aStatus, TGsmDateTime& aGsmDateTime) const=0;
	virtual void ReadClockCancel() const=0;
	virtual TInt SetClock(const TGsmDateTime& aGsmDateTime) const=0;
	virtual void SetClock(TRequestStatus& aStatus, const TGsmDateTime& aGsmDateTime) const=0;
	virtual void SetClockCancel() const=0;

	virtual TInt EnumerateAlarm(TInt& aCount) const=0;
	virtual void EnumerateAlarm(TRequestStatus& aStatus, TInt& aCount) const=0;
	virtual void EnumerateAlarmCancel() const=0;
	virtual TInt GetAlarm(const TInt aIndex,TAlarm& aAlarm) const=0;
	virtual void GetAlarm(TRequestStatus& aStatus, const TInt aIndex, TAlarm& aAlarm) const=0;
	virtual void GetAlarmCancel() const=0;
	virtual TInt SetAlarm(const TInt aIndex,const TAlarm& aAlarm) const=0;
	virtual void SetAlarm(TRequestStatus& aStatus, const TInt aIndex,const TAlarm& aAlarm) const=0;
	virtual void SetAlarmCancel() const=0;
	
	enum TClockCaps
		{
		KCapsClkClockAvailable=0x00000001,
		KCapsClkAlarmAvailable=0x00000002
		};

	struct TClockAndAlarmCapsFlags
		{
		TUint iClockAndAlarmCaps;
		};

	virtual TInt GetClockCaps(TClockAndAlarmCapsFlags& aCaps) const=0;
	virtual void NotifyClockCaps(TRequestStatus& aStatus, TClockAndAlarmCapsFlags& aCaps)=0;
	virtual void NotifyClockCapsCancel() const=0;

	virtual void ClockFreeSlot1() const=0;
	virtual void ClockFreeSlot2() const=0;
	virtual void ClockFreeSlot3() const=0;
	virtual void ClockFreeSlot4() const=0;

protected:
	MBasicGsmPhoneClockAndAlarm();
	};

/**/
/**/
/* RBasicGsmPhone*/
/**/
/**/
class RBasicGsmPhone : public RPhone, public MBasicGsmPhoneId, public MBasicGsmPhoneBookSupport,
					   public MBasicGsmPhoneSignalandBer, public MBasicGsmPhoneNetwork,
					   public MBasicGsmPhoneBatteryAndPower, public MBasicGsmPhoneIndicator,
					   public MBasicGsmPhoneClockAndAlarm
	{
public:
	friend class CBasicGsmRetrieveDetectedNetworks;

	IMPORT_C RBasicGsmPhone();

/**/
/*	General notification information*/
/**/
	class TPhoneInfo : public RPhone::TPhoneInfo
		{
	public:
		TInt32 iSignalStrength;
		TUint iBer;
		TCurrentNetworkInfo iCurrentNetworkInfo;
		TBatteryInfo iBatteryInfo;
		TIndicatorInfo iIndicatorInfo;
		TNetworkSetMode iNetworkSetMode;	
		TRegistrationStatus iNetworkRegistrationStatus;
	public:
		IMPORT_C TPhoneInfo();		
		};

	IMPORT_C TInt GetInfoBasicGsm(TPhoneInfo& aPhoneInfo) const;

	/**/
	/*	These virtual functions implement the pure virtual functions in Mixin classes*/
	/**/
	IMPORT_C virtual TInt GetGsmPhoneId(TId& aId) const;

	IMPORT_C virtual TInt EnumeratePhoneBooks(TInt& aPhoneBookCount) const;
	IMPORT_C virtual TInt GetPhoneBookInfo(const TInt aIndex, TPhoneBookInfo& aInfo) const;
	IMPORT_C virtual void GetPhoneBookInfo(TRequestStatus& aStatus, const TInt aIndex, TPhoneBookInfo& aInfo) const;
	IMPORT_C virtual void GetPhoneBookInfoCancel() const;

	IMPORT_C virtual TInt GetSignalStrength(TInt32& aSignalStrength) const;
	IMPORT_C virtual void GetSignalStrength(TRequestStatus& aStatus, TInt32& aSignalStrength);
	IMPORT_C virtual void GetSignalStrengthCancel() const;
	IMPORT_C virtual void SignalStrengthNotification(TRequestStatus& aStatus,TInt32& aSignalStrength);
	IMPORT_C virtual void SignalStrengthNotificationCancel() const;
	IMPORT_C virtual TInt GetBer(TUint& aBer) const;
	IMPORT_C virtual void GetBer(TRequestStatus& aStatus, TUint& aBer);
	IMPORT_C virtual void GetBerCancel() const;
	IMPORT_C virtual void BerNotification(TRequestStatus& aStatus,TUint& aBer);
	IMPORT_C virtual void BerNotificationCancel() const;
	IMPORT_C virtual TInt GetSignalCaps(TSignalAndBerCapsFlags& aCaps) const;
	IMPORT_C virtual void NotifySignalCaps(TRequestStatus& aStatus, TSignalAndBerCapsFlags& aCaps);
	IMPORT_C virtual void NotifySignalCapsCancel() const;

	IMPORT_C virtual TInt EnumerateDetectedNetworks(TInt& aNetworkCnt) const;
	IMPORT_C virtual void EnumerateDetectedNetworks(TRequestStatus& aStatus, TInt& aNetworkCnt);
	IMPORT_C virtual void EnumerateDetectedNetworksCancel() const;
	IMPORT_C virtual TInt GetDetectedNetworkInfo(const TInt aIndex,TNetworkInfo& aNetworkInfo) const;
	IMPORT_C virtual void GetDetectedNetworkInfo(TRequestStatus& aStatus, const TInt aIndex,TNetworkInfo& aNetworkInfo);
	IMPORT_C virtual void GetDetectedNetworkInfoCancel() const;

	IMPORT_C virtual TInt SetNetworkMode(const TNetworkSetMode aMode) const;
	IMPORT_C virtual void SetNetworkMode(TRequestStatus& aStatus,const TNetworkSetMode aMode);
	IMPORT_C virtual void SetNetworkModeCancel() const;
	IMPORT_C virtual TInt SetManualNetwork(const TBscNetworkId aNetworkId) const;
	IMPORT_C virtual void SetManualNetwork(TRequestStatus& aStatus,const TBscNetworkId aNetworkId);
	IMPORT_C virtual void SetManualNetworkCancel() const;

	IMPORT_C virtual TInt GetCurrentNetworkInfo(TCurrentNetworkInfo& aNetworkInfo) const;
	IMPORT_C virtual void NotifyChangeOfCurrentNetworkNS(TRequestStatus& aStatus,TCurrentNetworkInfo& aNetworkInfo);
	IMPORT_C virtual void NotifyChangeOfCurrentNetworkCancel() const;
	IMPORT_C virtual TInt GetNetworkRegistrationStatus(TRegistrationStatus& aRegistrationStatus) const;
	IMPORT_C virtual void NotifyChangeOfNetworkRegistrationStatus(TRequestStatus& aStatus,TRegistrationStatus& aRegistrationStatus);
	IMPORT_C virtual void NotifyChangeOfNetworkRegistrationStatusCancel() const;
	IMPORT_C virtual TInt GetNetworkSearchMode(TNetworkSetMode& aMode) const;
	IMPORT_C virtual void NotifyChangeOfNetworkSearchMode(TRequestStatus& aStatus,TNetworkSetMode& aMode);
	IMPORT_C virtual void NotifyChangeOfNetworkSearchModeCancel() const;

	IMPORT_C virtual CGsmNetworkList* RetrieveDetectedNetworksLC();

	IMPORT_C virtual TInt GetNetworkInfoCaps(TNetworkCapsFlags& aCaps) const;

	IMPORT_C virtual TInt GetBatteryInfo(TBatteryInfo& aBatteryInfo) const;
	IMPORT_C virtual void GetBatteryInfo(TRequestStatus& aStatus, TBatteryInfo& aBatteryInfo);
	IMPORT_C virtual void GetBatteryInfoCancel() const;
	IMPORT_C virtual void BatteryInfoNotification(TRequestStatus& aStatus,TBatteryInfo& aBatteryInfo);
	IMPORT_C virtual void BatteryInfoNotificationCancel() const;
	IMPORT_C virtual TInt GetBatteryPowerCaps(TBatteryAndPowerCapsFlags& aCaps) const;

	IMPORT_C virtual void IndicatorNotification(TRequestStatus& aStatus,TIndicatorInfo& aIndicator);
	IMPORT_C virtual void IndicatorNotificationCancel() const;
	IMPORT_C virtual TInt GetIndicator(const TIndicatorType aIndicator,TUint& aValue) const;
	IMPORT_C virtual TInt GetIndicatorCaptureCaps(TIndicatorCapsFlags& aCaps) const;

	IMPORT_C virtual TInt ReadClock(TGsmDateTime& aGsmDateTime) const;
	IMPORT_C virtual void ReadClock(TRequestStatus& aStatus, TGsmDateTime& aGsmDateTime) const;
	IMPORT_C virtual void ReadClockCancel() const;
	IMPORT_C virtual TInt SetClock(const TGsmDateTime& aGsmDateTime) const;
	IMPORT_C virtual void SetClock(TRequestStatus& aStatus, const TGsmDateTime& aGsmDateTime) const;
	IMPORT_C virtual void SetClockCancel() const;
	IMPORT_C virtual TInt EnumerateAlarm(TInt& aCount) const;
	IMPORT_C virtual void EnumerateAlarm(TRequestStatus& aStatus, TInt& aCount) const;
	IMPORT_C virtual void EnumerateAlarmCancel() const;
	IMPORT_C virtual TInt GetAlarm(const TInt aIndex,TAlarm& aAlarm) const;
	IMPORT_C virtual void GetAlarm(TRequestStatus& aStatus, const TInt aIndex, TAlarm& aAlarm) const;
	IMPORT_C virtual void GetAlarmCancel() const;
	IMPORT_C virtual TInt SetAlarm(const TInt aIndex,const TAlarm& aAlarm) const;
	IMPORT_C virtual void SetAlarm(TRequestStatus& aStatus, const TInt aIndex,const TAlarm& aAlarm) const;
	IMPORT_C virtual void SetAlarmCancel() const;
	IMPORT_C virtual TInt GetClockCaps(TClockAndAlarmCapsFlags& aCaps) const;
	IMPORT_C virtual void NotifyClockCaps(TRequestStatus& aStatus, TClockAndAlarmCapsFlags& aCaps);
	IMPORT_C virtual void NotifyClockCapsCancel() const;

	/* These are "free slots", to allow subsequent additional functions in Mixins without */
	/* breaking BC.*/

 	IMPORT_C virtual void IdFreeSlot1() const;
	IMPORT_C virtual void IdFreeSlot2() const;
	IMPORT_C virtual void IdFreeSlot3() const;
	IMPORT_C virtual void IdFreeSlot4() const;
	IMPORT_C virtual void PhBookSupportFreeSlot3() const;
	IMPORT_C virtual void PhBookSupportFreeSlot4() const;
 	IMPORT_C virtual void SignalFreeSlot1() const;
	IMPORT_C virtual void SignalFreeSlot2() const;
	IMPORT_C virtual void SignalFreeSlot3() const;
	IMPORT_C virtual void SignalFreeSlot4() const;
	IMPORT_C virtual void NetworkFreeSlot1() const;
	IMPORT_C virtual void NetworkFreeSlot2() const;
	IMPORT_C virtual void NetworkFreeSlot3() const;
	IMPORT_C virtual void NetworkFreeSlot4() const; 
	IMPORT_C virtual void BatteryFreeSlot1() const;
	IMPORT_C virtual void BatteryFreeSlot2() const;
	IMPORT_C virtual void BatteryFreeSlot3() const;
	IMPORT_C virtual void BatteryFreeSlot4() const;
	IMPORT_C virtual void IndicatorFreeSlot1() const;
	IMPORT_C virtual void IndicatorFreeSlot2() const;
	IMPORT_C virtual void IndicatorFreeSlot3() const;
	IMPORT_C virtual void IndicatorFreeSlot4() const;
	IMPORT_C virtual void ClockFreeSlot1() const;
	IMPORT_C virtual void ClockFreeSlot2() const;
	IMPORT_C virtual void ClockFreeSlot3() const;
	IMPORT_C virtual void ClockFreeSlot4() const;
	  
private:
	CBasicPtrHolder* iBasicPtrHolder;
protected:
	IMPORT_C virtual void ConstructL();
	IMPORT_C virtual void Destruct();
protected:
	void BasicPhoneGet(const TInt aIpc,TRequestStatus& aStatus,TDes8& aDes1,TDes8& aDes2) const;
	void BasicPhoneGet(const TInt aIpc,TRequestStatus& aStatus,TDes16& aDes1,TDes16& aDes2) const;
	TInt BasicPhoneCancel(const TInt aIpc,const TInt aIpcToCancel) const;
	};


/**/
/* Async retrieval of network results*/
/**/

class CBasicGsmRetrieveDetectedNetworks : public CActive
	{
public:
	IMPORT_C static CBasicGsmRetrieveDetectedNetworks* NewL(RBasicGsmPhone& aPhone);
	IMPORT_C ~CBasicGsmRetrieveDetectedNetworks();
/**/
/* Start the retrieval*/
/**/
	IMPORT_C void Start(TRequestStatus& aStatus);
/**/
/* Get the results of the retrieval*/
/**/
	IMPORT_C CGsmNetworkList* RetrieveResults() const;

protected:
	CBasicGsmRetrieveDetectedNetworks(RBasicGsmPhone& aPhone);
	void ConstructL();

	virtual void StartPhase2L();
/**/
/* CActive pure virtual functions*/
/* */
	virtual void DoCancel();
private:
	virtual void RunL();

protected:
	enum {
		EIdle,
		ERetrievePhase1,
		ERetrievePhase2
		} iState;

	RBasicGsmPhone& iPhone;
	TRequestStatus* iUserStatus;
	TInt iNumOfEntries;
	RBasicGsmPhone::TGsmNetworkClientId iId;
	CGsmNetworkList* iResults;
	CBufFlat* iResultsBuf;
	TPtr8 iIdPtr;
	TPtr8 iResultsPtr;
	TPtr8 iReadPtr;
	};

class CGsmNetworkList : public CBase
	{
public:
	IMPORT_C static CGsmNetworkList* NewL(TInt aNoOfEntries);
	IMPORT_C ~CGsmNetworkList();
/**/
/* Streaming - for use to pass array over IPC*/
/**/
	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;
	IMPORT_C CBufFlat* StoreLC();
	IMPORT_C void StoreL(TDes8& aDes);
	IMPORT_C void RestoreL(const TDesC8& aBuf);
	IMPORT_C void AddEmptyEntriesL();
/**/
/* Panic constant for browsing list*/
/**/
	enum 
		{ KPanicInvalidResultsIndex };
/**/
/* Accessing results*/
/* */
	IMPORT_C TInt GetNextEntry(RBasicGsmPhone::TNetworkInfo& aEntry);
	IMPORT_C TInt GetPreviousEntry(RBasicGsmPhone::TNetworkInfo& aEntry);
	IMPORT_C TInt GetEntry(const TInt aIndex, RBasicGsmPhone::TNetworkInfo& aEntry);
/**/
/* Adding an entry */
/**/
	IMPORT_C TInt AddEntryL(const RBasicGsmPhone::TNetworkInfo& aEntry);
	IMPORT_C TInt InsertEntryL(const TInt aIndex, const RBasicGsmPhone::TNetworkInfo& aEntry);
	IMPORT_C TInt ChangeEntry(const TInt aIndex, const RBasicGsmPhone::TNetworkInfo& aEntry);
	IMPORT_C TInt DeleteEntry(const TInt aIndex);

/**/
/* Counting entries*/
/**/
	IMPORT_C TInt EnumerateEntries(TInt& aCount);
	IMPORT_C TInt GetMaxNumEntries(TInt& aMaxNumber);
	IMPORT_C TInt SetMaxNumEntries(TInt aMaxNumber);
/**/
/* Reset all entries*/
/**/
	IMPORT_C void Reset();
protected:
	CGsmNetworkList(TInt aNoOfEntries);
private:
	void ConstructL();
private:
	IMPORT_C void GsmNetworkListSlot1();
	IMPORT_C void GsmNetworkListSlot2();

	CArrayFixFlat<RBasicGsmPhone::TNetworkInfo>* iList;
	TInt iCurrentIndex;
	TInt iLastAddedIndex;
	TInt iNoOfEntries;
	TInt iMaxNoOfEntries;
	};


/**/
/**/
/* Phone book*/
/**/
/**/

#define	SIM_FIX_DIALLING_PHONEBOOK		_L("SIMFixDialling")
#define ME_PHONEBOOK					_L("ME")
#define ME_AND_SIM_PHONEBOOK			_L("CombinedMEAndSIM")
#define SIM_PHONEBOOK					_L("SIM")
#define TA_PHONEBOOK					_L("TA")
/**/
#define LAST_DIALLED_PHONEBOOK			_L("DialledCalls")
#define EMERGENCY_PHONEBOOK				_L("EmergencyNumber")
#define MISSED_CALLS_PHONEBOOK			_L("MissedCalls")
#define RECEIVED_CALLS_PHONEBOOK		_L("ReceivedCalls")

class CGsmPhoneBookResults;
class RGsmPhoneBook : public RTelSubSessionBase
	{
friend class CGsmPhoneBookResults;
friend class CGsmPhoneBookSearch;
public:
	IMPORT_C RGsmPhoneBook();
	IMPORT_C TInt Open(RPhone& aPhone, const TDesC& aType);
	IMPORT_C void Close();

/**/
/* Capability Information*/
/**/
	enum
		{
		KCapsWriteAccess=0x00000001,
		KCapsRestrictedWriteAccess=0x00000002,
		KCapsAddressChanged=0x00000004
		};

	struct TCaps
		{
		TUint iCaps;
		};

	IMPORT_C TInt GetCaps(TCaps& aCaps) const;

/**/
/* Phone Book Entry Changed Notifications*/
/**/
	IMPORT_C void EntryChangedNotification(TRequestStatus& aStatus,TInt& aIndex); 
	IMPORT_C void EntryChangedNotificationCancel() const;

/**/
/* Phone book information*/
/**/

	IMPORT_C TInt GetInfo(RBasicGsmPhone::TPhoneBookInfo& aInfo) const;
	IMPORT_C void GetInfo(TRequestStatus& aStatus, RBasicGsmPhone::TPhoneBookInfo& aInfo);
	IMPORT_C void GetInfoCancel() const;

/**/
/* Phone Book Browsing*/
/**/
	enum { KPhoneBookEntrySize=100 };		/* Review this number*/

	class TEntry
		{
	public:
		void InternalizeL(RReadStream& aStream);
		void ExternalizeL(RWriteStream& aStream) const;
	public:
		TGsmTelNumber iTelNumber;
		TBuf<KPhoneBookEntrySize> iText;
		TInt iIndex;
		TName iPhoneBookName;
		};

	IMPORT_C TInt Read(TEntry& aEntry);
	IMPORT_C void Read(TRequestStatus& aStatus, TEntry& aEntry);
	IMPORT_C void ReadCancel() const;
	IMPORT_C TInt ReadFirst(TEntry& aEntry);
	IMPORT_C void ReadFirst(TRequestStatus& aStatus, TEntry& aEntry);
	IMPORT_C void ReadFirstCancel() const;
	IMPORT_C TInt ReadPrevious(TEntry& aEntry);
	IMPORT_C void ReadPrevious(TRequestStatus& aStatus, TEntry& aEntry);
	IMPORT_C void ReadPreviousCancel() const;
	IMPORT_C TInt ReadNext(TEntry& aEntry);
	IMPORT_C void ReadNext(TRequestStatus& aStatus, TEntry& aEntry);
	IMPORT_C void ReadNextCancel() const;

/**/
/* Phone Book Writing/Deleting*/
/**/
	IMPORT_C TInt Write(TEntry& aEntry) const;
	IMPORT_C void Write(TRequestStatus& aStatus, TEntry& aEntry);
	IMPORT_C void WriteCancel() const;
	IMPORT_C TInt Delete(TInt aIndex) const;
	IMPORT_C void Delete(TRequestStatus& aStatus, TInt aIndex);
	IMPORT_C void DeleteCancel() const;

/**/
/* Synchronous Phone Book Search*/
/**/

	typedef TBuf<KPhoneBookEntrySize> TSearchString;

	IMPORT_C CGsmPhoneBookResults* SearchLC(const TSearchString& aSearch);

/**/
/* Server TSY internal definitions*/
/**/

public:
	struct TGsmPhoneBookWrite
		{
		TEntry iEntry;
		TGsmPassword iPin2;
		};
	
	struct TGsmPhoneBookClientId
		{
		TInt iSessionHandle;
		TInt iSubSessionHandle;
		};

	struct TGsmPhoneBookSearch
		{
		TSearchString iSearchString;
		TInt iNumMatchingEntries;
		};
protected:
	void PhBookGet(const TInt aIpc,TRequestStatus& aStatus,TDes8& aDes1,TDes8& aDes2) const;
	void PhBookGet(const TInt aIpc,TRequestStatus& aStatus,TDes16& aDes1,TDes16& aDes2) const;
	TInt PhBookCancel(const TInt aIpc,const TInt aIpcToCancel) const;

private:
	RGsmPhoneBook(const RGsmPhoneBook& aPhoneBook);
	CBasicPtrHolder* iBasicPtrHolder;
	TGsmPhoneBookWrite iWrite;
	TInt iDelete;
	TInt iReadIndex;
protected:
	IMPORT_C virtual void ConstructL();
	IMPORT_C virtual void Destruct();
	};

/**/
/* Storage of results of phone book search or read; used by sync and async searches*/
/**/

class CGsmPhoneBookResults : public CBase
	{
public:
	IMPORT_C static CGsmPhoneBookResults* NewL(TInt aNoOfEntries);
	IMPORT_C ~CGsmPhoneBookResults();
/**/
/* Streaming - for use to pass array over IPC*/
/**/
	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;
	IMPORT_C CBufFlat* StoreLC();
	IMPORT_C void StoreL(TDes8& aDes);
	IMPORT_C void RestoreL(const TDesC8& aBuf);
	IMPORT_C void AddEmptyEntriesL();
/**/
/* Panic constant for browsing list*/
/**/
	enum 
		{ KPanicInvalidResultsIndex };
/**/
/* Accessing results*/
/* */
	IMPORT_C TInt GetNextEntry(RGsmPhoneBook::TEntry& aEntry);
	IMPORT_C TInt GetPreviousEntry(RGsmPhoneBook::TEntry& aEntry);
	IMPORT_C TInt GetEntry(TInt aPosition,RGsmPhoneBook::TEntry& aEntry);
/**/
/* Adding an entry */
/**/
	IMPORT_C TInt AddEntry(const RGsmPhoneBook::TEntry& aEntry);
/**/
/* Counting entries*/
/**/
	IMPORT_C TInt EnumerateEntries(TInt& aCount);
/**/
/* Reset all entries*/
/**/
	IMPORT_C void Reset();
protected:
	CGsmPhoneBookResults(TInt aNoOfEntries);
private:
	void ConstructL();
private:
	CArrayFixFlat<RGsmPhoneBook::TEntry>* iResults;
	TInt iCurrentIndex;
	TInt iLastAddedIndex;
	TInt iNoOfEntries;
	};

/**/
/* Async phone book search*/
/**/

class CGsmPhoneBookSearch : public CActive
	{
public:
	IMPORT_C static CGsmPhoneBookSearch* NewL(RGsmPhoneBook& aPhoneBook);
	IMPORT_C ~CGsmPhoneBookSearch();
/**/
/* Start the search*/
/**/
	IMPORT_C void Start(const RGsmPhoneBook::TSearchString& aSearch,TRequestStatus& aStatus);
/**/
/* Get the results of the search*/
/**/
	IMPORT_C CGsmPhoneBookResults* SearchResults() const;

protected:
	IMPORT_C CGsmPhoneBookSearch(RGsmPhoneBook& aPhoneBook);
	IMPORT_C void ConstructL();

	IMPORT_C virtual void StartPhase2L();
/**/
/* CActive pure virtual functions*/
/* */
	IMPORT_C virtual void DoCancel();
private:
	virtual void RunL();

protected:
	enum {
		EIdle,
		ESearchPhase1,
		ESearchPhase2
		} iState;

	RGsmPhoneBook& iPhoneBook;
	TRequestStatus* iUserStatus;
	RGsmPhoneBook::TGsmPhoneBookSearch iSearch;
	RGsmPhoneBook::TGsmPhoneBookClientId iId;
	TPtr8 iSearchPtr;
	TPtr8 iIdPtr;
	CGsmPhoneBookResults* iResults;
	CBufFlat* iResultsBuf;
	TPtr8 iResultsPtr;
	};

/**/
/**/
/* Sms Message Abstraction*/
/**/
/**/

#define SMS_BROADCAST_MESSAGE_STORE_NAME	_L("BroadcastMessageStorage")
#define SMS_ME_STORE_NAME					_L("MEStorage")
#define SMS_ME_AND_SIM_STORE_NAME			_L("MEAndSIMStorage")
#define SMS_SIM_STORE_NAME					_L("SIMStorage")
#define SMS_TA_STORE_NAME					_L("TAStorage")

class TSms
	{
public:
/**/
/* Messaging Format*/
/**/
	enum TFormat
		{
		EFormatPdu		/* this may be extended in the future*/
		};
/**/
/* Messaging Enumerations*/
/**/

	enum { KPduSize=177			};		/* (160*7)/8 + 25 (header) + 12 (SC address)*/
	enum { KTextSize=160		};		/* */

/**/
/* Message Structures*/
/**/
	typedef TBuf8<KPduSize> TPdu;
/**/
/* Functionality*/
/**/
	IMPORT_C TSms();
	IMPORT_C TSms(const TFormat aFormat);
	IMPORT_C TSms(const TPdu& aPdu);

	IMPORT_C TFormat Format() const;
	IMPORT_C void SetFormat(const TFormat aFormat);
	IMPORT_C TBool UseDefaultSca() const;
	IMPORT_C void SetUseDefaultSca(const TBool& aUseDefaultSca);
/**/
/* For Pdu message only*/
/**/
	IMPORT_C const TPdu& PduL() const;
	IMPORT_C void SetPduL(const TPdu& aPdu);

private:
	TFormat iFormat;
	TBool iUseDefaultSca;
	TPdu iBuf;
	};

/**/
/* RSmsStorage*/
/**/
class RSmsMessaging;
class RSmsStorage : public RTelSubSessionBase
	{
public:
	IMPORT_C RSmsStorage();
	IMPORT_C TInt Open(RSmsMessaging& aMessaging,const TDesC& aStore);
	IMPORT_C void Close();

	struct TInfo
		{
		TUint iUsed;
		TUint iTotal;
		};

	enum TStatus
		{
		EStatusUnread,
		EStatusRead,
		EStatusUnsent,
		EStatusSent
		};

	struct TSmsMsgStoreEntry
		{
		TSms iMsg;
		TStatus iStatus;
		};

	IMPORT_C TInt GetInfo(TInfo& aInfo) const;
	IMPORT_C void GetInfo(TRequestStatus& aStatus, TInfo& aInfo);
	IMPORT_C void GetInfoCancel() const;
	IMPORT_C TInt ReadSms(const TInt aIndex, TSmsMsgStoreEntry& aMsgEntry) const;
	IMPORT_C void ReadSms(TRequestStatus& aStatus, const TInt aIndex, TSmsMsgStoreEntry& aMsgEntry);
	IMPORT_C void ReadSmsCancel() const;
	IMPORT_C TInt Write(TInt& aIndex, const TSmsMsgStoreEntry& aMsg) const;
	IMPORT_C void Write(TRequestStatus& aStatus, TInt& aIndex, const TSmsMsgStoreEntry& aMsg);
	IMPORT_C void WriteCancel() const;
	IMPORT_C TInt Delete(const TInt aIndex) const;
	IMPORT_C void Delete(TRequestStatus& aStatus, const TInt aIndex);
	IMPORT_C void DeleteCancel() const;

	IMPORT_C void WriteNotification(TRequestStatus& aStatus, TInt& aIndex);
	IMPORT_C void WriteNotificationCancel() const;

private:
	CBasicPtrHolder* iBasicPtrHolder;
	TInt iReadIndexValue;
	TInt iDeleteValue;
protected:
	IMPORT_C virtual void ConstructL();
	IMPORT_C virtual void Destruct();
	};

/**/
/* RSmsMessaging*/
/**/
class RSmsMessaging : public RTelSubSessionBase
	{
public:
	IMPORT_C RSmsMessaging();
	IMPORT_C TInt Open(RPhone& aPhone);
	IMPORT_C void Close();

/**/
/* Capability Information*/
/**/
	enum
		{
		KCapsModeCommandPduSupported=0x00000001,
		KCapsModeBlockPduSupported=0x00000002,
		KCapsModeReadWithoutStorage=0x00000004,
		KCapsModeReadWithStorage=0x00000008
		};

	enum
		{
		KCapsTypeRxSmsDeliver=0x00000001,
		KCapsTypeTxSmsSubmit=0x00000002
		};

	struct TCaps
		{
		TUint iSmsMode;
		TUint iSmsMsgTypes;
		};

	IMPORT_C TInt GetCaps(TCaps& aCaps) const;

/**/
/* Messaging Mode*/
/**/
	enum TSmsMode
		{
		EModeCommandPdu,
		EModeBlockPdu
		};

	enum TStorageMode
		{
		EReadWithoutStorage,
		EReadWithStorage
		};

	struct TMode
		{
		TSmsMode iMessagingMode;
		TStorageMode iStorageMode;
		};

	IMPORT_C TInt SetMode(const TMode& aMode) const;
	IMPORT_C void SetMode(TRequestStatus& aStatus, const TMode& aMode) const;
	IMPORT_C void SetModeCancel() const;
	IMPORT_C TInt GetMode(TMode& aMode) const;

	IMPORT_C TInt GetDefaultSCAddress(TGsmTelNumber& aSCAddress) const;
	IMPORT_C void GetDefaultSCAddress(TRequestStatus& aStatus, TGsmTelNumber& aSCAddress) const;
	IMPORT_C void GetDefaultSCAddressCancel() const;
	IMPORT_C TInt SetDefaultSCAddress(const TGsmTelNumber& aSCAddress) const;
	IMPORT_C void SetDefaultSCAddress(TRequestStatus& aStatus, const TGsmTelNumber& aSCAddress);
	IMPORT_C void SetDefaultSCAddressCancel() const;
/**/
/* Message Transmission*/
/**/
	IMPORT_C void SendMessage(TRequestStatus& aStatus, TUint& aMsgRef, TSms& aMsg);
	IMPORT_C void SendMessageCancel() const;

/**/
/* Message Reception*/
/**/
	struct TMsgLocation
		{
		TInt iIndex;
		TName iStore;
		};

	IMPORT_C void ReadSmsMessage(TRequestStatus& aStatus, TMsgLocation& aMsgLocation, TSms& aMsg);
	IMPORT_C void ReadSmsMessageCancel() const;
	IMPORT_C void NotifySmsMessageArrived(TRequestStatus& aStatus) const;
	IMPORT_C void NotifySmsMessageArrivedCancel() const;

/**/
/* Message Storage*/
/**/
	struct TStoreInfo
		{
		TName iName;
		TUint iTotal;
		TUint iUsed;
		TUint32 iFlags;
		};

	enum
		{
		KMessageStoreReadAndDeleted=0x00000001,
		KMessageStoreWriteAndSend=0x00000002,
		KMessageStoreBroadcast=0x00000004
   		};

	IMPORT_C TInt EnumerateMessageStores(TInt& aCount) const;
	IMPORT_C TInt GetMessageStoreInfo(const TInt aIndex,TStoreInfo& aInfo) const;
	IMPORT_C void GetMessageStoreInfo(TRequestStatus& aStatus,const TInt aIndex,TStoreInfo& aInfo) const;
	IMPORT_C void GetMessageStoreInfoCancel() const;

private:
	CBasicPtrHolder* iBasicPtrHolder;
protected:
	IMPORT_C virtual void ConstructL();
	IMPORT_C virtual void Destruct();
	};

#endif
