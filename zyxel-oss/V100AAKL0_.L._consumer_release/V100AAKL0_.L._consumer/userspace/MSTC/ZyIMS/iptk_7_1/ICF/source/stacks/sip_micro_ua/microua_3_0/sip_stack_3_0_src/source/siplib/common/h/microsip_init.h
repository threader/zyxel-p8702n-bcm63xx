/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 ** FUNCTION:
 **             Init function prototypes for all structures
 **
 ***********************************************************************
 **
 ** FILENAME:
 ** microsip_init.h
 **
 ** DESCRIPTION:
 **
 ** DATE        NAME           REFERENCE       REASON
 ** ----        ----           ---------       --------
 ** 20/02/2002  Aparna	    		-		Initial Creation
 **
 ** COPYRIGHT , Aricent, 2006
 **********************************************************************/

#include "microsip_struct.h"
#include "microsip_free.h"

#ifndef __MICROSIP_INIT_H_
#define __MICROSIP_INIT_H_

#define INIT(x) \
do \
{ \
	x=SIP_NULL;\
} \
while(0)

extern SipBool sip_initSipCommonHeader _ARGS_\
	((SipCommonHeader **ppHeader,SipError *pErr));

#ifdef SIP_DATE
extern SipBool sip_initSipCommonHeaderWithDate _ARGS_\
	((SipCommonHeader **ppHeader,en_ExpiresType dType, SipError *pErr));
#endif
#define sip_initSipToHeader sip_initSipCommonHeader
#define sip_initSipReferredByHeader sip_initSipCommonHeader
#define sip_initSipRouteHeader sip_initSipCommonHeader
#define sip_initSipRecordRouteHeader sip_initSipCommonHeader
#define sip_initReferedByHeader sip_initSipCommonHeader
#define sip_initSipReferToHeader sip_initSipCommonHeader

#ifdef SIP_PEMEDIA
#define sip_initSipPEarlyMediaHeader sip_initSipCommonHeader
#endif

#ifdef SIP_3GPP
#define sip_initSipPathHeader 							sip_initSipCommonHeader
#define sip_initSipPAssociatedUriHeader			sip_initSipCommonHeader
#define sip_initSipPCalledPartyIdHeader			sip_initSipCommonHeader
#define sip_initSipPVisitedNetworkIdHeader	sip_initSipCommonHeader
#define sip_initSipPcfAddrHeader 						sip_initSipCommonHeader
#define sip_initSipPanInfoHeader						sip_initSipCommonHeader
#define sip_initSipPcVectorHeader						sip_initSipCommonHeader
#define sip_initSipServiceRouteHeader 			sip_initSipCommonHeader
#define sip_initSipHistoryInfoHeader 				sip_initSipCommonHeader
#define sip_initSipAcceptContactHeader 			sip_initSipCommonHeader
#define sip_initSipRejectContactHeader 			sip_initSipCommonHeader
#define sip_initSipRequestDispositionHeader sip_initSipCommonHeader
#define sip_initSipJoinHeader 							sip_initSipCommonHeader
#define sip_initSipIfMatchHeader						sip_initSipCommonHeader
#define sip_initSipETagHeader								sip_initSipCommonHeader		
#endif

#ifdef SIP_AUTHENTICATE
#define sip_initSipWwwAuthenticateHeader sip_initSipCommonHeader
#define sip_initSipProxyAuthenticateHeader sip_initSipCommonHeader
#define sip_initSipProxyAuthorizationHeader sip_initSipCommonHeader
#endif

#define sip_initSipSupportedHeader sip_initSipCommonHeader
#define sip_initSipAllowHeader sip_initSipCommonHeader
#define sip_initSipAllowEventsHeader sip_initSipCommonHeader
#define sip_impp_initSipAllowEventsHeader sip_initSipCommonHeader
#define sip_initSipCallIdHeader sip_initSipCommonHeader
#define sip_initSipRequireHeader sip_initSipCommonHeader
#define sip_initSipContentEncodingHeader sip_initSipCommonHeader
#define sip_initSipMimeVersionHeader sip_initSipCommonHeader
#define sip_bcpt_initSipMimeVersionHeader sip_initSipCommonHeader
#ifdef SIP_DATE
#define sip_initSipExpiresHeader sip_initSipCommonHeaderWithDate
#else
#define sip_initSipExpiresHeader sip_initSipCommonHeader
#endif
#define sip_initSipMinExpiresHeader sip_initSipCommonHeader
#define sip_initSipAcceptHeader sip_initSipCommonHeader
#define sip_initSipContentDispositionHeader sip_initSipCommonHeader

#ifdef SIP_SECURITY
#define sip_initSipSecurityClientHeader sip_initSipCommonHeader
#define sip_initSipSecurityServerHeader sip_initSipCommonHeader
#define sip_initSipSecurityVerifyHeader sip_initSipCommonHeader
#endif
#ifdef SIP_PRIVACY
#define sip_initSipPrivacyHeader sip_initSipCommonHeader
#define sip_initSipPAssertIdHeader sip_initSipCommonHeader
#define sip_initSipPPreferredIdHeader sip_initSipCommonHeader
#endif

#ifdef SIP_IMPP
#define sip_initSipEventHeader sip_initSipCommonHeader
#define sip_impp_initSipEventHeader sip_initSipCommonHeader
#define sip_initSipSubscriptionStateHeader sip_initSipCommonHeader
#define sip_impp_initSipSubscriptionStateHeader sip_initSipCommonHeader
#endif /* SIP_IMPP */

#ifdef SIP_REPLACES
#define sip_initSipReplacesHeader sip_initSipCommonHeader
#endif /* SIP_REPLACES */

#ifdef SIP_SESSIONTIMER
#define sip_initSipMinSEHeader			sip_initSipCommonHeader
#define sip_initSipSessionExpiresHeader	sip_initSipCommonHeader
#endif /* SIP_SESSIONTIMER */

#define sip_initSipMaxForwardsHeader	sip_initSipCommonHeader
#define sip_initSipRseqHeader			sip_initSipCommonHeader
#define sip_rpr_initSipRSeqHeader		sip_initSipCommonHeader

#ifdef SIP_CONGEST
#define sip_initSipRsrcPriorityHeader			sip_initSipCommonHeader
#define sip_initSipAcceptRsrcPriorityHeader			sip_initSipCommonHeader
#endif


extern SIP_S8bit* sip_getPart _ARGS_((void ));

extern SipBool sip_initSipTimerKey _ARGS_((SipTimerKey **k,SipError *err));

extern SipBool sip_initSdpOrigin _ARGS_((SdpOrigin **s,SipError *err));

extern SipBool sip_initSdpMedia _ARGS_((SdpMedia **m,SipError *err));

extern SipBool sip_initSdpAttr _ARGS_((SdpAttr **a,SipError *err));

extern SipBool sip_initSdpTime _ARGS_((SdpTime **t,SipError *err));

extern SipBool sip_initSdpConnection _ARGS_((SdpConnection **c,SipError *err));

extern SipBool sip_initSdpMessage _ARGS_((SdpMessage **m,SipError *err));

#ifdef SIP_WARNING
#define sip_initSipWarningHeader      sip_initSipCommonHeader
#endif
#ifdef SIP_AUTHENTICATE
#define sip_initSipAuthorizationHeader sip_initSipCommonHeader
#endif
extern SipBool sip_initSipRespHeader _ARGS_((SipRespHeader **h,SipError *err));

#define sip_initSipContentTypeHeader sip_initSipCommonHeader

#ifdef SIP_DATE
#define sip_initSipRetryAfterHeader 	sip_initSipCommonHeaderWithDate
#else
#define sip_initSipRetryAfterHeader   sip_initSipCommonHeader
#endif /* SIP_DATE */

#define sip_initSipContentLengthHeader sip_initSipCommonHeader
#define sip_initSipViaHeader           sip_initSipCommonHeader

extern SipBool sip_initSipUrl _ARGS_((SipUrl **u,SipError *err));

extern SipBool sip_initSipAddrSpec _ARGS_((SipAddrSpec **a,en_AddrType type,\
	SipError *err));

#ifdef SIP_TIMESTAMP
#define sip_initSipTimeStampHeader    sip_initSipCommonHeader
#endif

#define sip_initSipFromHeader        sip_initSipCommonHeader
extern SipBool sip_initSipContactHeader 
_ARGS_  ((  SipContactHeader **ppHeader, 
		    en_ContactType type,
		    SipError *err));
#define sip_initSipCseqHeader        sip_initSipCommonHeader
extern SipBool sip_initSipReqHeader _ARGS_((SipReqHeader **s,SipError *err));
#define sip_initSipReqLine           sip_initSipCommonHeader
#define sip_initSipStatusLine        sip_initSipCommonHeader
#define sip_initSipUnknownHeader     sip_initSipCommonHeader
#define sip_initSipUnsupportedHeader sip_initSipCommonHeader

extern SipBool sip_initSipGeneralHeader _ARGS_((SipGeneralHeader **g,\
	SipError *err));

extern SipBool sip_initSipReqMessage _ARGS_((SipReqMessage **r,SipError *err));

extern SipBool sip_initSipRespMessage _ARGS_((SipRespMessage **r,SipError *err));

extern SipBool sip_initSipUnknownMessage _ARGS_((SipUnknownMessage **s,\
	SipError *err));

extern SipBool sip_initSipMsgBody _ARGS_((SipMsgBody **s,\
	en_SipMsgBodyType type,SipError *err));

extern SipBool sip_initSipMessage _ARGS_((SipMessage **s,\
	en_SipMessageType type,SipError *err));

extern SipBool sip_initSipHeader _ARGS_((SipHeader **h,\
	en_HeaderType type,SipError *err));

extern SipBool sip_initSipHeaderOrderInfo _ARGS_((SipHeaderOrderInfo **h,\
	SipError *err));

extern SipBool sip_initSipParam _ARGS_((SipParam **p,SipError *err));

extern SipBool sip_initSipAllowHeader _ARGS_((SipAllowHeader **u,\
	SipError *err));

#ifdef SIP_RPR
#define sip_initSipRackHeader       sip_initSipCommonHeader
#define sip_rpr_initSipRAckHeader   sip_initSipCommonHeader 
#endif
#ifdef SIP_DATE
#define sip_initSipExpiresStruct 	sip_initSipCommonHeaderWithDate
#else
#define sip_initSipExpiresStruct    sip_initSipCommonHeader
#endif /* SIP_DATE */

#ifdef SIP_DATE
extern SipBool sip_initSipDateFormat _ARGS_((SipDateFormat **d,SipError *err));

extern SipBool sip_initSipTimeFormat _ARGS_((SipTimeFormat **t,SipError *err));

extern SipBool sip_initSipDateStruct _ARGS_((SipDateStruct **d,SipError *err));
#endif

extern SIP_S8bit *TOKEN_BUFFER_DUP _ARGS_ ((SipTokenBuffer x)) ;

#ifdef SIP_IMPP
#define sip_initSipAllowEventsHeader    sip_initSipCommonHeader
#endif
#ifdef SIP_ISUP
SipBool sip_initIsupMessage
	(IsupMessage **m,SipError *err);
#endif
#ifdef SIP_CORE_STACK_COMPATIBLE
#ifdef SIP_AUTHENTICATE
extern SipBool sip_initSipGenericCredential _ARGS_((SipGenericCredential **c,\
	en_CredentialType type,SipError *err));

extern SipBool sip_initSipGenericChallenge _ARGS_((SipGenericChallenge **c,\
	SipError *err));

extern void sip_freeSipGenericCredential _ARGS_((SipGenericCredential *c));

extern void sip_freeSipGenericChallenge _ARGS_((SipGenericChallenge *c));
#endif
extern SipBool sip_initSipContactParam _ARGS_((SipContactParam **c,\
	en_ContactParamsType type,SipError *err));

extern SipBool sip_initSipNameValuePair _ARGS_((SipNameValuePair **c,\
	SipError *pErr));

#endif /* SIP_CORE_STACK_COMPATIBLE */

#ifdef SIP_MWI
extern SipBool sip_mwi_initMesgSummaryMessage _ARGS_((MesgSummaryMessage **m,SipError *err));

extern SipBool sip_mwi_initSummaryLine _ARGS_((SummaryLine **c,SipError *err));
#endif


#ifdef  SIP_BADMESSAGE_PARSING
extern SipBool sip_initSipBadHeader _ARGS_((SipBadHeader **e,\
	en_HeaderType dType,SipError *err));
#endif	

extern SipBool sip_initStack _ARGS_ ((void));

extern SipBool sip_initSipTimerBuffer _ARGS_((SipTimerBuffer **b, SipError *e));


#ifdef SIP_3GPP
extern SipBool sip_initMsrpUrl _ARGS_((MsrpUrl **ppMsrpUrl, SipError *pErr));
#endif

/* Fix for CSR 1-6792719 refferred from CSR 1-6466704 */
extern SipBool sip_initSipTranspAddr(SipTranspAddr **t,SipError *pErr);

#endif /* __MICROSIP_INIT_H_ */

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 


