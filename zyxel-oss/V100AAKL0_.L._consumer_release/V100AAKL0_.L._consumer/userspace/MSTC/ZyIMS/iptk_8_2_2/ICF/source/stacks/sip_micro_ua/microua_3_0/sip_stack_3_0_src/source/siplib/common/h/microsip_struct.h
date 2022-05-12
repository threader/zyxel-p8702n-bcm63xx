/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** FUNCTION:
**		This file has all the SIP Related Structures
**
********************************************************************************
**
** FILENAME:
** microsip_struct.h
**
** DESCRIPTION:
**	All primary structures and wrappers for pBasic datatypes are stored here.
**	It is expected that all programs use datatypes defined here.
**
** DATE			NAME			REFERENCE		     REASON
** ----			----			--------		     ------
** 15/2/2002	Mahesh Govind						Initial
**
** COPYRIGHT , Aricent, 2006
********************************************************************************/


#ifndef __MICROSIPSTRUCT_H__
#define __MICROSIPSTRUCT_H__

#include "microsip_common.h"
#include "microsip_list.h"


#include "microsip_portlayer.h"

#define SIP_TCP (0x1)
#define SIP_UDP (0x2)
#define SIP_NORETRANS (0x4)

#define SIP_SCTP (0x8)
#define SIP_TLS (0x10)

/* constants for reforming a message */
#define HEADER_SIZE 1024

#define SIP_OPT_FULLFORM		(0x1)
#define SIP_OPT_SHORTFORM		(0x2)
#define SIP_OPT_COMMASEPARATED	(0x4)
#define SIP_OPT_SINGLE			(0x8)
#define SIP_OPT_REORDERHOP		(0x10)
#define SIP_OPT_AUTHCANONICAL	(0x20)
#define SIP_OPT_CLEN			(0x40)
#define SIP_OPT_RETRANSCALLBACK  (0x80)
#define SIP_OPT_PERMSGRETRANS (0x100)
#define SIP_OPT_DIRECTBUFFER  (0x200)
#define SIP_OPT_PERMSGRETRANSCOUNT (0x400)
/* CSR_1-6949679 Changes Starts */ 
#define SIP_OPT_REMOVEROUTE		(0x800)
/* CSR_1-6949679 Changes Ends */ 

/* constants for decoding a message */
#define SIP_OPT_BADMESSAGE		(0x1)
#define SIP_OPT_NOTIMER         (0x2)
#define SIP_OPT_NOPARSEBODY		(0x4)
#define SIP_OPT_PARTIAL			(0x8)		


#ifndef SIP_LOCKEDREFCOUNT
typedef SIP_U16bit SIP_RefCount	;	/* Reference Count for structures in new version */
#endif /* SIP_LOCKEDREFCOUNT */

#ifdef SIP_LOCKEDREFCOUNT
typedef struct
{
	SIP_U16bit			ref;
	synch_id_t			lock;
} SIP_RefCount;

#endif /* SIP_LOCKEDREFCOUNT */

typedef enum
{
	SipMessageRequest = 0,
	SipMessageResponse,
	SipMessageAny			/* Required for timer pKey matching */
} en_SipMessageType;

typedef enum
{
	SipTimerA_B,	/* For Starting TimerA along With TimerB */
	SipTimerB,
	
	/*
		There are two timer Cs. The reason for having two timers is
		slightly involved. Timer C is started as soon as a PROXY sends
		an INVITE. This timer is reset(restarted) whenever a provisional
		response is received(other than 100 Trying). However when it actually
		fires an indication needs to be given to the application so that
		the application can embark on cancelling the INVITE txn. However the
		txn shud not be removed. Instead another timer Cr is started during
		which the INVITE txn is allowed to complete by way of receipt of a
		487.During the Timer Cr is running any provisional response that is
		received DOES NOT restart the Timer C , these are returned as stray
		The value for both these  timers will be independently configurable. 
		However receipt of timeout callbacks will NOT be independently 
		configurable. In case an application is interested in receiving 
		timeout for TimerC , the appln will *also* receive timeouts for
		Timer Cr as well.
	*/
	SipTimerC,
	SipTimerCr,
	
	SipTimerD,
	SipTimerE_F,	/* For Starting TimerE along With TimerF */
	SipTimerF,
	SipTimerG_H,	/* For Starting TimerG along With TimerH */
	SipTimerH,
	SipTimerI,
	SipTimerJ,
	SipTimerK,
	
	SipTimerForCancelledState,  /*This timer is the timer that is started when
									a txn is cancelled*/
	
	/*
		When a proxy UAS forwards a 2xx the txn needs to be terminated. However 
		this is not done immediately since receipt of retransmissions of INVITEs
		is an issue. We do not want each of these retransmissions to end up
		creating a new txn. For this reason the txn is not immediately removed 
		but retained for a fixed period which shud be sufficient enough to
		buffer up retransmissions of INVITE. For all purposes this value
		shud be same as that of the remote end's TimerB. As far as the stack
		is concerned no separate value will be defined for this timer, its value
		will be the same as the application's configured value of Timer B. 
		Receipt of timeouts of this timer will *not* be configurable. The
		stack will not throw callbacks whenever this timer expires.
	*/
	SipTimerForProxyUAS2xxFwded,

	/*
	 * This timer only serves an an indicator for the last enum.
	 * It is not used anywhere in the stack
	 */
	SipTimerLast
}en_SipTimerType;

typedef enum
{
	Sip_en_headerNameExpected = 0,
	Sip_en_headerBodyExpected
} en_StateofHeaderParsing;

/*Header Types are arranged in alphabetical order.*/

typedef enum
{
	/* General headers start. */
	SipHdrTypeAllow = 0,
#ifdef SIP_IMPP	
	SipHdrTypeAllowEvents,
#endif /* SIP_IMPP */ 	
#ifdef SIP_AUTHENTICATE
	SipHdrTypeAuthorization,
#endif	
	SipHdrTypeCallId,
	SipHdrTypeContactNormal,
	SipHdrTypeContactWildCard,
	SipHdrTypeContactAny,
	SipHdrTypeContentDisposition,
	SipHdrTypeContentEncoding,
	SipHdrTypeContentLength,
	SipHdrTypeContentType,
	SipHdrTypeCseq,
	/* General headers end here. */
#ifdef SIP_IMPP	
	SipHdrTypeEvent,
#endif /* SIP_IMPP */	
#ifdef SIP_DATE
	SipHdrTypeExpiresDate,		
	SipHdrTypeExpiresSec,		
	SipHdrTypeExpiresAny,		
#else
	SipHdrTypeExpires,
#endif /* SIP_DATE */
	SipHdrTypeAccept,
	SipHdrTypeMinExpires,
	SipHdrTypeFrom,
	SipHdrTypeMaxforwards,
	SipHdrTypeMimeVersion,
#ifdef SIP_PRIVACY	
	SipHdrTypePrivacy,      /* Privacy Header */   
	SipHdrTypePPreferredId, /* P-Preferred-Identity */
	SipHdrTypePAssertId,    /* P-Asserted-Identity */
#endif	
#ifdef SIP_SESSIONTIMER
	SipHdrTypeMinSE,
#endif /* SIP_SESSIONTIMER */
#ifdef SIP_3GPP
	SipHdrTypePath,		/*	Path	*/
	SipHdrTypePAssociatedUri, /*3gpp */
  SipHdrTypePCalledPartyId, /*3gpp */
  SipHdrTypePVisitedNetworkId, /*3gpp */
  SipHdrTypePcfAddr, /*3gpp */
 	SipHdrTypePanInfo,   /* 3gpp */
  SipHdrTypePcVector,  /* 3gpp */
  SipHdrTypeServiceRoute, /*3GPP */
	SipHdrTypeHistoryInfo,
	SipHdrTypeRequestDisposition, /*3gpp */
	SipHdrTypeAcceptContact, /*3gpp */
	SipHdrTypeRejectContact, /*3gpp */
	SipHdrTypeJoin, /*3gpp */
	SipHdrTypeIfMatch,
	SipHdrTypeETag,

#endif
#ifdef SIP_AUTHENTICATE
	SipHdrTypeProxyAuthenticate,
	SipHdrTypeProxyauthorization,
#endif	
#ifdef SIP_RPR
	SipHdrTypeRAck,
#endif	
	SipHdrTypeRecordRoute,
#ifdef SIP_REFER	
	SipHdrTypeReferredBy,
	SipHdrTypeReferTo,
#endif /* SIP_REFER */
#ifdef SIP_REPLACES
	SipHdrTypeReplaces,
#endif /* SIP_REPLACES */
	SipHdrTypeRequire,
	SipHdrTypeRoute,
#ifdef SIP_RPR	
	SipHdrTypeRSeq,
#endif
#ifdef SIP_SECURITY
    SipHdrTypeSecurityClient,
   	SipHdrTypeSecurityVerify,
   	SipHdrTypeSecurityServer,
#endif
#ifdef SIP_SESSIONTIMER
	SipHdrTypeSessionExpires,
#endif /* SIP_SESSIONTIMER */
#ifdef SIP_IMPP
	SipHdrTypeSubscriptionState,
#endif /* SIP_IMPP */
	SipHdrTypeSupported,
#ifdef SIP_TIMESTAMP	
	SipHdrTypeTimestamp,
#endif	
	SipHdrTypeTo,
	SipHdrTypeUnsupported,
	SipHdrTypeVia,
#ifdef SIP_WARNING	
	SipHdrTypeWarning,
#endif	
#ifdef SIP_AUTHENTICATE	
	SipHdrTypeWwwAuthenticate,
#endif	
	SipHdrTypeUnknown,
#ifdef SIP_DATE
	SipHdrTypeRetryAfterDate,
	SipHdrTypeRetryAfterSec,
	SipHdrTypeRetryAfterAny,
#else
	SipHdrTypeRetryAfter,
#endif /* SIP_DATE */
#ifdef SIP_PEMEDIA
	SipHdrTypePEarlyMedia,
#endif
#ifdef SIP_CONGEST
	SipHdrTypeRsrcPriority,	
    SipHdrTypeAcceptRsrcPriority,
#endif
	SipHdrTypeAny /* This must always be the last enumeration */
} en_HeaderType;

#define HEADERTYPENUM ((int)SipHdrTypeAny + 1)

typedef enum
{
	SipModeJoin = 0,
	SipModeNew
} en_AdditionMode;

typedef enum
{
	SipFormFull = 0,
	SipFormShort
} en_HeaderForm;

#ifdef SIP_CORE_STACK_COMPATIBLE
typedef enum
{
	SipCredBasic = 0,
	SipCredAuth,
	SipCredAny
} en_CredentialType;
#endif /* SIP_CORE_STACK_COMPATIBLE */

typedef enum	
{	
	SipAddrSipUri = 0, 
	SipAddrSipSUri,
	SipAddrReqUri,
	SipAddrAny
} en_AddrType;

typedef enum
{
	SipContactWildCard = 0,
	SipContactNormal,
	SipContactAny
} en_ContactType;

typedef enum
{
    SipSdpBody = 0,
	SipIsupBody,
	SipMultipartMimeBody,
	SipAppSipBody, 
	SipUnknownBody,
#ifdef SIP_MWI
	SipMessageSummaryBody,
#endif    
	SipBodyAny
} en_SipMsgBodyType;

/*
** Structure Definitions Follow 
*/

typedef struct
{
	SIP_S8bit		*pName;
	SipList 		slValue;	/* of SIP_S8bit * */
	SIP_RefCount	dRefCount;
} SipParam;

typedef struct
{
	SIP_S8bit		dIpv4[16];
	SIP_U16bit		dPort;
	SIP_Pvoid		pData;
	SIP_S8bit		*pHost;
	SIP_U16bit		dSockFd;
	SIP_U16bit 		dReceiversPort;/*rport*/
    SIP_S8bit 		*pReceiversIp;/*rport*/
    
    /* CSR 1-6466704 Change Start */
    SIP_RefCount	dRefCount;
    /* CSR 1-6466704 Change End */    
} SipTranspAddr;

typedef struct
{
	SipTraceLevel	dLevel;
	SipTraceType	dType;
} SipTrace;

typedef struct
{
	SIP_U32bit		dOption;
} SipOptions;

typedef struct 
{
	en_HeaderType	dType;
	SIP_Pvoid		pHeader;
} SipHeader;

typedef struct
{
	SipBool			enable[HEADERTYPENUM];
} SipHdrTypeList;


typedef struct
{
	SipHdrTypeList	*pList;
	SipTranspAddr	*pTranspAddr;
	SIP_S8bit		*pDirectBuffer;
	SIP_Pvoid 		pData;
	SIP_U32bit		dRemainingLength;
	SIP_U32bit		dNextMessageLength;
	SipOptions      dOptions;
#ifndef SIP_TXN_LAYER	
	SIP_U32bit 		dRetransT1;
	SIP_U32bit 		dRetransT2;
	SIP_U16bit 		dMaxRetransCount;
	SIP_U16bit 		dMaxInviteRetransCount; 
#endif /* SIP_TXN_LAYER */
} SipEventContext;

/* SDP Structures */

typedef struct
{
	SIP_S8bit*		pUser;
	SIP_S8bit*		pSessionid;
	SIP_S8bit*		pVersion;
	SIP_S8bit*		pNetType;
	SIP_S8bit*		pAddrType;
	SIP_S8bit		*pAddr;
	SIP_RefCount	dRefCount;
} SdpOrigin;

typedef struct 
{
	SIP_S8bit*      pInformation;
	SipList 		slConnection;		/* of SdpConnection */
	SipList			slBandwidth;
	SIP_S8bit*      pKey;
	SIP_S8bit*		pFormat;
	SIP_U16bit		dPort;
	SIP_U32bit*		pPortNum;
	SIP_S8bit*		pProtocol;
	SipList 		slAttr; 			/* of SdpAttr */
	SIP_S8bit*		pMediaValue;
	SipList         slIncorrectLines;
	SIP_RefCount	dRefCount;
}SdpMedia ;

typedef struct 
{
	SIP_S8bit*		pName;
	SIP_S8bit*		pValue;
	SIP_RefCount	dRefCount;
} SdpAttr;

typedef struct 
{
	SIP_S8bit*		pStart;
	SIP_S8bit*		pStop;
	SipList 		slRepeat;			/* of char * */
	SIP_S8bit*		pZone;
	SIP_RefCount	dRefCount;
} SdpTime;

typedef struct 
{
	SIP_S8bit*		pNetType;
	SIP_S8bit*		pAddrType;
	SIP_S8bit*		pAddr;
	SIP_RefCount	dRefCount;
} SdpConnection;

typedef struct 
{
	SIP_S8bit*		pVersion;
	SdpOrigin*		pOrigin;
	SIP_S8bit*		pSession;
	SIP_S8bit		*pInformation; 
	SIP_S8bit		*pUri; 
	SipList         slEmail; 
	SipList         slPhone; 
	SdpConnection	*slConnection; 
	SipList         pBandwidth;		
	SipList 		slTime; 			/* of SdpTime		*/
	SIP_S8bit		*pKey;
	SipList 		slAttr; 			/* of SdpAttr		*/
	SipList 		slMedia;			/* of SdpMedia		*/
	SipList         slIncorrectLines;   /* incorrect SDP lines */
	SIP_RefCount	dRefCount;
} SdpMessage;


typedef struct 
{
	SIP_S8bit			*pUser;
	SIP_S8bit			*pPassword;
	SIP_S8bit			*pHost;
	SIP_U16bit			*dPort;
	SipList				slParam;		/*  of SipParam */
	SIP_S8bit			*pHeader;
	SIP_RefCount		dRefCount;
} SipUrl;

typedef struct 
{
	en_AddrType			dType;
	union 
	{
		SIP_S8bit		*pUri;
		SipUrl			*pSipUrl;
	} u;
	SIP_RefCount		dRefCount;
} SipAddrSpec;

#ifdef SIP_DATE 
typedef enum
{	SipDayMon = 0,
	SipDayTue,
	SipDayWed,
	SipDayThu,
	SipDayFri,
	SipDaySat,
	SipDaySun,
	SipDayNone
} en_DayOfWeek;

typedef enum
{
	SipMonthJan = 0,
	SipMonthFeb,
	SipMonthMar,
	SipMonthApr,
	SipMonthMay,
	SipMonthJun,
	SipMonthJul,
	SipMonthAug,
	SipMonthSep,
	SipMonthOct,
	SipMonthNov,
	SipMonthDec
} en_Month;

typedef struct
{
	SIP_S8bit			dHour;
	SIP_S8bit			dMin;
	SIP_S8bit			dSec;
	SIP_RefCount		dRefCount;
} SipTimeFormat;

typedef struct
{
	SIP_S8bit			dDay;
	en_Month			dMonth;
	SIP_U16bit			dYear;
	SIP_RefCount		dRefCount;
} SipDateFormat;

typedef struct
{
	en_DayOfWeek		dDow;
	SipDateFormat		*pDate;
	SipTimeFormat		*pTime;
	SIP_RefCount		dRefCount;
} SipDateStruct;

/* Replacing en_ExpiresType */
#define en_ExpiresType SIP_U32bit
#define SipExpDate 0
#define SipExpSeconds 1
#define SipExpAny 2

#endif 
/*
We have a generic Challenge structure this has PGP + any other 
*/
typedef struct
{
        SIP_S8bit        *pStr1;
        SipAddrSpec      *pAddrSpec;
        SipList          slParam;
        SIP_S8bit        *pStr2;
        SIP_U32bit       dIntVar1;
        SIP_U32bit       dIntVar2;
#ifdef SIP_DATE
		union
		{
			SipDateStruct   *pDate;
			SIP_U32bit      dSec;
		} u;
#endif
#ifdef SIP_INCREMENTAL_PARSING
        SIP_S8bit       *pBuffer;
#endif
        SIP_RefCount 	dRefCount;
}SipCommonHeader;

#define SipWarningHeader            SipCommonHeader
#define SipContentDispositionHeader SipCommonHeader
#if defined(SIP_AUTHENTICATE) || defined(SIP_3GPP)
#define SipAuthorizationHeader      SipCommonHeader
#endif
#define SipContentTypeHeader        SipCommonHeader
#define SipContentLengthHeader      SipCommonHeader
#ifdef SIP_AUTHENTICATE
#define SipWwwAuthenticateHeader    SipCommonHeader 
#define SipProxyAuthenticateHeader  SipCommonHeader 
#define SipProxyAuthorizationHeader SipCommonHeader 
#endif
#define SipUnsupportedHeader        SipCommonHeader

#define  SipSupportedHeader         SipCommonHeader
#define  SipMimeVersionHeader       SipCommonHeader 
#define  SipAllowHeader             SipCommonHeader
#define  SipAllowEventsHeader       SipCommonHeader
#define  SipContentEncodingHeader   SipCommonHeader
#define  SipCallIdHeader            SipCommonHeader
#define  SipRequireHeader           SipCommonHeader
#ifdef SIP_RPR
#define  SipRseqHeader              SipCommonHeader
#endif
#define  SipMaxForwardsHeader       SipCommonHeader
#ifdef SIP_SESSIONTIMER
#define  SipMinSEHeader             SipCommonHeader
#define  SipSessionExpiresHeader    SipCommonHeader
#endif /* SIP_SESSIONTIMER */

#ifdef SIP_IMPP
#define  SipEventHeader             SipCommonHeader
#endif /* SIP_IMPP */

#ifdef SIP_REPLACES
#define SipReplacesHeader           SipCommonHeader
#endif /* SIP_REPLACES */

#ifdef SIP_PRIVACY
#define  SipPrivacyHeader						SipCommonHeader
#define  SipPAssertIdHeader					SipCommonHeader
#define  SipPPreferredIdHeader			SipCommonHeader
#endif

#ifdef SIP_IMPP
#define SipSubscriptionStateHeader  SipCommonHeader
#endif /* SIP_IMPP */

#define SipViaHeader                SipCommonHeader
#define SipFromHeader               SipCommonHeader
#define SipToHeader                 SipCommonHeader 
#define SipRouteHeader              SipCommonHeader 
#define SipRecordRouteHeader        SipCommonHeader
#define SipReferredByHeader         SipCommonHeader
#define SipReferToHeader            SipCommonHeader

#ifdef SIP_3GPP
#define SipPathHeader               SipCommonHeader 
#define SipPAssociatedUriHeader     SipCommonHeader
#define SipPCalledPartyIdHeader			SipCommonHeader
#define SipPVisitedNetworkIdHeader	SipCommonHeader
#define SipPcfAddrHeader						SipCommonHeader
#define SipPcVectorHeader						SipCommonHeader
#define SipPanInfoHeader						SipCommonHeader 
#define SipServiceRouteHeader				SipCommonHeader
#define SipHistoryInfoHeader				SipCommonHeader
#define SipRequestDispositionHeader	SipCommonHeader
#define SipAcceptContactHeader 			SipCommonHeader
#define SipRejectContactHeader			SipCommonHeader
#define SipJoinHeader								SipCommonHeader
#define SipIfMatchHeader						SipCommonHeader
#define SipETagHeader								SipCommonHeader
#endif

#define  SipExpiresStruct           SipCommonHeader
#define  SipExpiresHeader           SipCommonHeader
#define  SipMinExpiresHeader           SipCommonHeader
#define  SipAcceptHeader           SipCommonHeader

#ifdef SIP_SECURITY
#define  SipSecurityClientHeader           SipCommonHeader
#define  SipSecurityServerHeader           SipCommonHeader
#define  SipSecurityVerifyHeader           SipCommonHeader
#endif

#define  SipRetryAfterHeader        SipCommonHeader
#define  SipContactHeader           SipCommonHeader
#ifdef SIP_TIMESTAMP
#define  SipTimeStampHeader         SipCommonHeader
#endif

#ifdef SIP_REFER
#define  SipReferToHeader           SipCommonHeader
#endif

#ifdef SIP_RPR
#define  SipRackHeader              SipCommonHeader
#endif
#define  SipCseqHeader				SipCommonHeader
#define  SipUnknownHeader           SipCommonHeader

#define  SipReqLine                 SipCommonHeader
#define  SipStatusLine              SipCommonHeader


#ifdef SIP_PEMEDIA
#define  SipPEarlyMediaHeader           SipCommonHeader
#endif

#ifdef SIP_CONGEST
#define  SipRsrcPriorityHeader              SipCommonHeader
#define  SipAcceptRsrcPriorityHeader        SipCommonHeader 
#endif

	
typedef struct 
{
	SipList 			slProxyAuthenticateHdr;		/* of SipProxyAuthenticateHeader */
	SipList 			slUnsupportedHdr;			/* of SipUnsupportedHeader	*/
#ifdef SIP_WARNING	
	SipList 			slWarningHeader;			/* of SipWarningHeader	*/
#endif	
	SipList 			slWwwAuthenticateHdr;
#ifdef SIP_RPR	
	SipRseqHeader		*pRSeqHdr;  				/* Retrans */
#endif
#ifdef SIP_3GPP
	SipList  		    slPAssociatedUriHdr; /*P-Associated-URI header */
	SipList			    slServiceRouteHdr;	/* Of ServiceRoute Header	*/ 		       
	SipETagHeader		*pETagHdr;					/*SIP-ETag Header */
#endif
	SipMinExpiresHeader			*pMinExpiresHdr;
#ifdef SIP_SECURITY
	SipList  slSecurityServerHdr; /*Security-Server header */
#endif
#ifdef SIP_CONGEST 
	SipList   	slAcceptRsrcPriorityHdr;  /* List of SipAcceptResourcePHeader */
#endif      
	
	SIP_RefCount		dRefCount;
}SipRespHeader ;


#ifdef SIP_CORE_STACK_COMPATIBLE
typedef enum
{
	SipCParamQvalue = 0,
	SipCParamExpires,
	SipCParamAction,
	SipCParamExtension,
#ifdef SIP_3GPP
        SipCParamFeature,
#endif


	SipCParamAny
} en_ContactParamsType;

typedef struct
{
	en_ContactParamsType	dType;
	union
	{
		SIP_S8bit*			pQValue;
		SipExpiresStruct*	pExpire;
		SIP_S8bit*			pAction;
		SIP_S8bit*			pExtensionAttr;
#ifdef SIP_3GPP
		SIP_S8bit*			pFeatureAttr;
#endif

	} u;
	SIP_RefCount			dRefCount;
} SipContactParam;
#endif /* SIP_CORE_STACK_COMPATIBLE */

typedef struct
{
	SipList						slProxyAuthorizationHdr;	
	SipList						slRouteHdr;	
#ifdef SIP_RPR	
	SipRackHeader				*pRackHdr ;
#endif	
	SipList						slAuthorizationHdr;
#ifdef SIP_REFER
	SipReferToHeader			*pReferToHdr;
	SipReferredByHeader			*pReferredByHdr;
#endif /* SIP_REFER */
#ifdef SIP_IMPP
	SipEventHeader				*pEventHdr;
	SipSubscriptionStateHeader	*pSubscriptionStateHdr;
#endif /* SIP_IMPP */	
#ifdef SIP_REPLACES
	SipReplacesHeader			*pReplacesHdr;
#endif /* SIP_REPLACES */
#ifdef SIP_3GPP
	SipPCalledPartyIdHeader      *pPCalledPartyIdHdr; /* P-Called-Party_ID header */
  SipList  					 					 slPVisitedNetworkIdHdr; /*P-Visited-Network-ID header */	 	
	SipList  					  				 slRequestDispositionHdr; /*RequestDisposition header */	  
	SipList  					   				 slAcceptContactHdr; /*AcceptContact header */		
  SipList  					           slRejectContactHdr; /*RejectContact header */
	SipJoinHeader				         *pJoinHdr;  /* Join Header */
	SipIfMatchHeader			       *pIfMatchHdr;  /* SIP-If-Match header */
#endif
#ifdef SIP_SECURITY
	SipList  		slSecurityClientHdr;
	SipList  		slSecurityVerifyHdr;
#endif	
	SipMaxForwardsHeader		*pMaxForwardsHdr;
#ifdef SIP_CONGEST
	SipList   		slRsrcPriorityHdr; /* List of Resource-Priority headers */
#endif
	SIP_RefCount				dRefCount;
} SipReqHeader ;

#ifdef SIP_MIME_PARSING
typedef struct
{
	SipList				slRecmimeBody;	/* of SipMsgBody */
	SIP_RefCount		dRefCount;
} MimeMessage;

typedef struct
{
	SipContentTypeHeader		*pContentType;
	SipContentDispositionHeader *pContentDisposition;
	SIP_S8bit 					*pContentTransEncoding;
	SIP_S8bit					*pContentId;
	SIP_S8bit					*pContentDescription;
	SipList						slAdditionalMimeHeaders;  /* of char * */
	SIP_RefCount				dRefCount;
} SipMimeHeader;
#endif

#ifdef SIP_BADMESSAGE_PARSING
typedef struct
{
	en_HeaderType				dType;
	SIP_S8bit					*pName;
	SIP_S8bit					*pBody;
	SIP_RefCount				dRefCount;
} SipBadHeader;
#endif

typedef struct
{
	SipCallIdHeader				*pCallidHdr;
	SipList						slContactHdr;			/* of ContactHeader		*/
	SipCseqHeader				*pCseqHdr;
	SipExpiresHeader			*pExpiresHdr;
	SipFromHeader				*pFromHeader;
	SipList						slRecordRouteHdr;		/* of RecordRouteHeader */
#ifdef SIP_TIMESTAMP	
	SipTimeStampHeader			*pTimeStampHdr;
#endif	
	SipToHeader					*pToHdr;
	SipList						slViaHdr;				/* of ViaHeader 		*/
	SipContentLengthHeader		*pContentLengthHdr;
	SipContentTypeHeader		*pContentTypeHdr;
	SipMimeVersionHeader		*pMimeVersionHdr;
	SipRetryAfterHeader			*pRetryAfterHdr;
	SipList						slContentEncodingHdr;	/* of ContentEncodingHeade */
	SipList						slUnknownHdr;			/* of SipUnknownHeader */
	SipList						slRequireHdr;
	SipList						slSupportedHdr;
	SipList						slAllowHdr;
	SipList						slContentDispositionHdr;
	SipList						slBadHdr;
#ifdef SIP_IMPP
	SipList						slAllowEventsHdr;		 /* of Allow-Events header */
#endif /* SIP_IMPP */
#ifdef SIP_PRIVACY
	SipPrivacyHeader	*pPrivacyHdr;				/* of Privacy Header */
	SipList 					slPAssertIdHdr;     /* of P-Asserted-Identity Header*/      
	SipList 					slPPreferredIdHdr;  /* of P-Preferred-Identity Header */

#endif
#ifdef SIP_SESSIONTIMER
	SipSessionExpiresHeader		*pSessionExpiresHdr;
	SipMinSEHeader				*pMinSEHdr;
#endif /* SIP_SESSIONTIMER */
#ifdef SIP_3GPP
	SipList									slPathHdr;	/* Of Path Header	*/	
 	SipPcfAddrHeader  			*pPcfAddrHdr;/* Of P-Charging-Function-Addresses */
	SipPanInfoHeader        *pPanInfoHdr;  /* Of P-Access-Network-Info */
	SipPcVectorHeader       *pPcVectorHdr; /* Of P-Charging-Vector */
	SipList									slHistoryInfoHdr;	/* Of HistoryInfo Header	*/

#endif
#ifdef SIP_PEMEDIA
	SipList slPEarlyMediaHdr;
#endif
	SipList						slAcceptHdr;

	SIP_RefCount				dRefCount;
} SipGeneralHeader;

typedef struct
{
	SipReqLine				*pRequestLine;
	SipReqHeader			*pRequestHdr;
	SIP_RefCount			dRefCount;
} SipReqMessage;

typedef struct
{
	SipStatusLine			*pStatusLine;
	SipRespHeader			*pResponseHdr;
	SIP_RefCount			dRefCount;
} SipRespMessage;

typedef struct
{
	en_HeaderType			dType;
	en_HeaderForm			dTextType;
	SIP_U32bit				dNum;	    /* number of dType headers in that line */
	SIP_RefCount			dRefCount;
} SipHeaderOrderInfo;

typedef struct
{
	SIP_U32bit				dLength;
	SIP_S8bit				*pBuffer;
	SIP_RefCount			dRefCount;
} SipUnknownMessage;
#ifdef SIP_ISUP
typedef struct
{
    SIP_U32bit				dLength;
    SIP_S8bit				*pBody;
    SIP_RefCount			dRefCount;
} IsupMessage;

#endif

#ifdef SIP_MWI

typedef enum
{
	SipMsgWaitingNo=0,
	SipMsgWaitingYes
} en_StatusType;

typedef struct
{
	SIP_S8bit*	     pMedia;		/* Media Type */
	SIP_U32bit       newMessages;		/* no of new Messages */
	SIP_U32bit       oldMessages;		/* no of old Messages */
	SIP_U32bit       newUrgentMessages;	/* no of new Urgent Messages */
	SIP_U32bit       oldUrgentMessages;	/* no of old Urgent Messages */
	SIP_RefCount	 dRefCount;
} SummaryLine;


typedef struct
{
	en_StatusType 	dStatus;
	SipList		    slSummaryLine;	/* List of SummaryLine structures */
	SipList		    slNameValue;	/* List of SipNameValuePair structures */
	SipAddrSpec     * pAddrSpec ; /* Message-Account */
	SIP_RefCount	dRefCount;
} MesgSummaryMessage;
#endif

typedef struct
{
	en_SipMessageType		dType;
	SipList					slOrderInfo;	/* of SipHeaderOrderInfo */
	SipList					slMessageBody;	  /* of SipMsgBody	*/
	SipGeneralHeader		*pGeneralHdr;
	union
	{
		SipReqMessage		*pRequest;
		SipRespMessage		*pResponse;
	} u;
	SIP_RefCount			dRefCount;	
} SipMessage;

typedef struct
{
	en_SipMsgBodyType		dType;
#ifdef SIP_MIME_PARSING	
	SipMimeHeader			*pMimeHeader;	/* bcpt ext */
#endif	
	union
	{
		SdpMessage			*pSdpMessage;
		SipUnknownMessage	*pUnknownMessage;
#ifdef SIP_ISUP		
		IsupMessage         *pIsupMessage;          /* bcpt ext */	
#endif	
#ifdef SIP_MWI
        MesgSummaryMessage   *pSummaryMessage;      /* RFC 3842 - MWI */     
#endif	
#ifdef SIP_MIME_PARSING	
		MimeMessage			*pMimeMessage;		/* bcpt ext */
#endif		
#ifdef SIP_SIPMSGBODY
		SipMessage          *pAppSipMessage;        /* REFER draft extension */
#endif		
	}u;
	SIP_RefCount			dRefCount;
} SipMsgBody;


typedef struct
{
	en_SipMessageType		dMatchType;
	SIP_U8bit 				dMatchFlag;
	SIP_S8bit				*dCallid;
	SIP_S8bit				*pMethod;
	SIP_U32bit				dCseq;
#ifdef SIP_RPR	
	SIP_U32bit 				dRseq; 		/* rpr */
	SipRackHeader			*pRackHdr; 	/* rpr */
#endif	
	SipFromHeader			*pFromHdr;
	SipToHeader				*pToHdr;
	SIP_U16bit				dCodeNum;
	SIP_S8bit				*pViaBranch;
	/* The event context in this structure is used to pass the context
	   from sip_decodeMessage to fast_stopTimer */
	SipEventContext			*pContext;
	SIP_RefCount			dRefCount;
} SipTimerKey;

#ifdef SIP_TXN_LAYER
/*The prototype for start/stop timer*/
typedef SIP_U32bit (*TimeoutFuncPtr)(void*,...);
#define sip_txn_timeOutFuncPtr TimeoutFuncPtr
#endif

typedef struct
{
	SIP_U8bit				dRetransCount;
	SIP_U32bit				dDuration;
	SipBool					dInfoReceived;
	SipTranspAddr			dAddr;
	SIP_S8bit				dTranspType;
	SIP_S8bit				*pBuffer;
	SIP_U32bit				dBufferLength;
	SipEventContext			*pContext;
	SIP_U32bit				dRetransT1;
	SIP_U32bit				dRetransT2;
	SIP_U16bit				dMaxRetransCount;
	SIP_U16bit				dMaxInviteRetransCount; 
	SipBool					enableRetransCallback;
	SIP_RefCount			dRefCount;
} SipTimerBuffer;

typedef struct
{
	SIP_S8bit				*pToken;
	SIP_U16bit				dLength;
	SIP_S8bit				dChar;
} SipTokenBuffer;

#ifdef SIP_CORE_STACK_COMPATIBLE
#ifdef SIP_AUTHENTICATE
typedef struct
{
	SIP_S8bit				*pScheme;						
	SipList					slParam; 
	SIP_RefCount			dRefCount;					
} SipGenericChallenge;

typedef struct
{
	en_CredentialType		dType;
	union
	{
		SIP_S8bit			*pBasic;
		SipGenericChallenge	*pChallenge;
	}u;
	SIP_RefCount			dRefCount;
} SipGenericCredential;
#endif
typedef struct
{
	SIP_S8bit*		pName;
	SIP_S8bit*		pValue;
	SIP_RefCount	dRefCount;
}SipNameValuePair;
#endif /* SIP_CORE_STACK_COMPATIBLE */

#ifdef SIP_3GPP

typedef struct
{
          SIP_S8bit      *pUser;
          SIP_S8bit      *pHost;
		  SIP_U16bit	 *dPort;                
          SIP_S8bit      *pResource;
	  SIP_RefCount	 dRefCount;
} MsrpUrl;

#endif



#endif /* __MICROSIPSTRUCT_H__ */

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

