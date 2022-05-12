/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 ** FUNCTION:
 **             Has Free Function Declarations For all Structures
 **
 ******************************************************************************
 **
 ** FILENAME:
 ** microsip_free.h
 **
 ** DESCRIPTION:
 **
 ** DATE        NAME                    REFERENCE       REASON
 ** ----        ----                    ---------       --------
 ** 27/02/2002 Mahesh Govind         Initial Creation		-
 **
 ** COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#ifndef __MICROSIP_FREE_H_
#define __MICROSIP_FREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "microsip_struct.h"

#define HSS_FREE(x) \
do \
{ \
	if ((x!=SIP_NULL)) (void)fast_memfree(0,x,SIP_NULL);\
} \
while(0)

#ifdef SIP_DATE
extern void sip_freeSipCommonHeaderWithDate _ARGS_((SipCommonHeader *pHeader));
extern void __sip_freeSipCommonHeaderWithDate _ARGS_((SIP_Pvoid pHeader));
#endif
extern void sip_freeSipCommonHeader _ARGS_((SipCommonHeader *pHeader));
extern void __sip_freeSipCommonHeader _ARGS_((SIP_Pvoid pHeader));

#ifdef SIP_AUTHENTICATE
#define  sip_freeSipProxyAuthenticateHeader sip_freeSipCommonHeader
#define  sip_freeSipProxyAuthorizationHeader sip_freeSipCommonHeader
#define  sip_freeSipWwwAuthenticateHeader sip_freeSipCommonHeader
#endif

#ifdef SIP_3_0
#define sip_freeSipPEarlyMediaHeader   sip_freeSipCommonHeader
#endif


#ifdef SIP_3GPP
#define	sip_freeSipPathHeader  							sip_freeSipCommonHeader
#define sip_freeSipPAssociatedUriHeader			sip_freeSipCommonHeader
#define sip_freeSipPCalledPartyIdHeader			sip_freeSipCommonHeader
#define sip_freeSipPVisitedNetworkIdHeader	sip_freeSipCommonHeader
#define sip_freeSipPcfAddrHeader 						sip_freeSipCommonHeader
#define sip_freeSipPanInfoHeader						sip_freeSipCommonHeader
#define sip_freeSipPcVectorHeader						sip_freeSipCommonHeader
#define	sip_freeSipServiceRouteHeader  			sip_freeSipCommonHeader
#define	sip_freeSipHistoryInfoHeader  			sip_freeSipCommonHeader
#define sip_freeSipAcceptContactHeader 			sip_freeSipCommonHeader
#define sip_freeSipRejectContactHeader 			sip_freeSipCommonHeader
#define sip_freeSipRequestDispositionHeader sip_freeSipCommonHeader
#define sip_freeSipJoinHeader 							sip_freeSipCommonHeader
#define sip_freeSipIfMatchHeader						sip_freeSipCommonHeader 
#define sip_freeSipETagHeader								sip_freeSipCommonHeader 

#endif

#define	sip_freeSipToHeader  sip_freeSipCommonHeader
#define	sip_freeSipReferredByHeader  sip_freeSipCommonHeader
#define	sip_freeSipReferToHeader  sip_freeSipCommonHeader
#define	sip_freeSipRouteHeader  sip_freeSipCommonHeader
#define	sip_freeSipRecordRouteHeader  sip_freeSipCommonHeader
#define	sip_freeSipSupportedHeader  sip_freeSipCommonHeader
#define	sip_freeSipAllowHeader  sip_freeSipCommonHeader
#define	sip_freeSipContentEncodingHeader  sip_freeSipCommonHeader
#define	sip_freeSipRequireHeader  sip_freeSipCommonHeader
#define	sip_freeSipMimeVersionHeader  sip_freeSipCommonHeader
#define	sip_freeSipCallIdHeader  sip_freeSipCommonHeader
#ifdef SIP_DATE
#define	sip_freeSipExpiresHeader   sip_freeSipCommonHeaderWithDate
#define	sip_freeSipExpiresStruct   sip_freeSipCommonHeaderWithDate
#else
#define	sip_freeSipExpiresHeader   sip_freeSipCommonHeader
#define	sip_freeSipExpiresStruct   sip_freeSipCommonHeader
#endif
#define	sip_freeSipAcceptHeader   sip_freeSipCommonHeader
#define	sip_freeSipMinExpiresHeader   sip_freeSipCommonHeader
#define	sip_freeSipMinExpiresStruct   sip_freeSipCommonHeader
#define	sip_freeSipRseqHeader   sip_freeSipCommonHeader

#ifdef SIP_SECURITY
#define	sip_freeSipSecurityClientHeader   sip_freeSipCommonHeader
#define	sip_freeSipSecurityServerHeader   sip_freeSipCommonHeader
#define	sip_freeSipSecurityVerifyHeader   sip_freeSipCommonHeader

#define	__sip_freeSipSecurityClientHeader   __sip_freeSipCommonHeader
#define	__sip_freeSipSecurityServerHeader   __sip_freeSipCommonHeader
#define	__sip_freeSipSecurityVerifyHeader   __sip_freeSipCommonHeader

#endif


#ifdef SIP_PRIVACY
#define	sip_freeSipPrivacyHeader  		sip_freeSipCommonHeader
#define	sip_freeSipPAssertIdHeader  	sip_freeSipCommonHeader
#define	sip_freeSipPPreferredIdHeader sip_freeSipCommonHeader
#endif

#ifdef SIP_RPR
#define sip_rpr_freeSipRSeqHeader sip_freeSipCommonHeader
#endif
#define	sip_freeSipMaxForwardsHeader   sip_freeSipCommonHeader
#ifdef SIP_SESSIONTIMER
#define	sip_freeSipMinSEHeader   sip_freeSipCommonHeader
#define	sip_freeSipSessionExpiresHeader   sip_freeSipCommonHeader
#endif

#ifdef SIP_IMPP
#define	sip_freeSipAllowEventsHeader  sip_freeSipCommonHeader
#define	sip_impp_freeSipAllowEventsHeader  sip_freeSipCommonHeader
#endif


#define  __sip_freeSipProxyAuthenticateHeader __sip_freeSipCommonHeader
#define  __sip_freeSipProxyAuthorizationHeader __sip_freeSipCommonHeader
#define  __sip_freeSipWwwAuthenticateHeader __sip_freeSipCommonHeader
#define	__sip_freeSipToHeader  __sip_freeSipCommonHeader
#define	__sip_freeSipReferredByHeader  __sip_freeSipCommonHeader
#define	__sip_freeSipReferToHeader  __sip_freeSipCommonHeader
#define	__sip_freeSipRouteHeader  __sip_freeSipCommonHeader
#define	__sip_freeSipRecordRouteHeader  __sip_freeSipCommonHeader
#define	__sip_freeSipSupportedHeader  __sip_freeSipCommonHeader
#define	__sip_freeSipAllowHeader  __sip_freeSipCommonHeader
#define	__sip_freeSipContentEncodingHeader  __sip_freeSipCommonHeader
#define	__sip_freeSipAllowEventsHeader  __sip_freeSipCommonHeader
#ifdef SIP_PRIVACY
#define	__sip_freeSipPrivacyHeader  		__sip_freeSipCommonHeader
#define	__sip_freeSipPAssertIdHeader  	__sip_freeSipCommonHeader
#define	__sip_freeSipPPreferredIdHeader __sip_freeSipCommonHeader
#endif
#ifdef SIP_PEMEDIA
#define __sip_freeSipPEarlyMediaHeader __sip_freeSipCommonHeader
#endif

#ifdef SIP_CONGEST
#define	sip_freeSipRsrcPriorityHeader  sip_freeSipCommonHeader
#define	sip_freeSipAcceptRsrcPriorityHeader  sip_freeSipCommonHeader
#define	__sip_freeSipAcceptRsrcPriorityHeader __sip_freeSipCommonHeader
#define	__sip_freeSipRsrcPriorityHeader __sip_freeSipCommonHeader
#endif

extern void sip_freeString _ARGS_((SIP_S8bit *s));

extern void sip_freeSipTimerKey _ARGS_((SipTimerKey *k));

extern void sip_freeSdpOrigin _ARGS_((SdpOrigin *s));

extern void sip_freeSdpMedia _ARGS_((SdpMedia *m));

extern void sip_freeSdpAttr _ARGS_((SdpAttr *a));

extern void sip_freeSdpTime _ARGS_((SdpTime *t));

extern void sip_freeSdpConnection _ARGS_(( SdpConnection *c));

extern void sip_freeSdpMessage _ARGS_((SdpMessage *m));

extern void sip_freeSipParam _ARGS_((SipParam *c));
#ifdef SIP_WARNING
#define sip_freeSipWarningHeader    sip_freeSipCommonHeader	
#endif
#ifdef SIP_AUTHENTICATE
#define sip_freeSipAuthorizationHeader     sip_freeSipCommonHeader	
#define sip_freeSipProxyAuthenticateHeader sip_freeSipCommonHeader	
#endif
#ifdef SIP_DATE
#define	sip_freeSipRetryAfterHeader   sip_freeSipCommonHeaderWithDate
#else
#define	sip_freeSipRetryAfterHeader   sip_freeSipCommonHeader
#endif
#define sip_freeSipContentTypeHeader     sip_freeSipCommonHeader	
#define sip_freeSipContentLengthHeader     sip_freeSipCommonHeader	
#ifdef SIP_RPR
#define sip_freeSipRackHeader     sip_freeSipCommonHeader	
#define sip_rpr_freeSipRAckHeader     sip_freeSipCommonHeader	
#endif
#define sip_freeSipViaHeader     sip_freeSipCommonHeader	
#define sip_freeSipToHeader     sip_freeSipCommonHeader	

extern void sip_freeSipRespHeader _ARGS_((SipRespHeader *h));

extern void sip_freeSipUrl _ARGS_((SipUrl *u));

extern void sip_freeSipAddrSpec _ARGS_((SipAddrSpec *a));

#ifdef SIP_TIMESTAMP
#define sip_freeSipTimeStampHeader     sip_freeSipCommonHeader	
#endif
#define sip_freeSipRequireHeader     sip_freeSipCommonHeader	
#define sip_freeSipFromHeader     sip_freeSipCommonHeader	
#define sip_freeSipContactHeader     sip_freeSipCommonHeader	
#define sip_freeSipCseqHeader     sip_freeSipCommonHeader	
#define sip_freeSipCallIdHeader     sip_freeSipCommonHeader	
#define sip_freeSipUnsupportedHeader     sip_freeSipCommonHeader	
#define sip_freeSipUnknownHeader     sip_freeSipCommonHeader	
#define sip_freeSipReqLine     sip_freeSipCommonHeader	
#define sip_freeSipStatusLine     sip_freeSipCommonHeader	

extern void sip_freeSipReqHeader _ARGS_((SipReqHeader *s));

extern void sip_freeSipGeneralHeader _ARGS_((SipGeneralHeader *g));

extern void sip_freeSipReqMessage _ARGS_((SipReqMessage *r));

extern void sip_freeSipRespMessage _ARGS_((SipRespMessage *r));

extern void sip_freeSipMessage _ARGS_((SipMessage *s));

extern void sip_freeSipUnknownMessage _ARGS_((SipUnknownMessage *s));

extern void sip_freeSipMsgBody _ARGS_((SipMsgBody *s));

extern void sip_freeSipHeader _ARGS_((SipHeader *h));

#ifdef SIP_DATE
extern void sip_freeSipDateStruct _ARGS_((SipDateStruct *d));

extern void sip_freeSipDateFormat _ARGS_((SipDateFormat *d));

extern void sip_freeSipTimeFormat _ARGS_((SipTimeFormat *t));
#endif /* SIP_DATE */


/* These are declarations for the INTERNAL siplist functions */

extern void __sip_freeString _ARGS_((SIP_Pvoid s));

extern void __sip_freeSipTimerKey _ARGS_((SIP_Pvoid k));

extern void __sip_freeSdpOrigin _ARGS_((SIP_Pvoid s));

extern void __sip_freeSdpMedia _ARGS_((SIP_Pvoid m));

extern void __sip_freeSdpAttr _ARGS_((SIP_Pvoid a));

extern void __sip_freeSdpTime _ARGS_((SIP_Pvoid t));

extern void __sip_freeSdpConnection _ARGS_((SIP_Pvoid c));

extern void __sip_freeSdpMessage _ARGS_((SIP_Pvoid m));

extern void __sip_freeSipParam _ARGS_((SIP_Pvoid c));

extern void __sip_freeSipGenericChallenge _ARGS_((SIP_Pvoid c));
#ifdef SIP_WARNING
#define __sip_freeSipWarningHeader __sip_freeSipCommonHeader
#endif
#ifdef SIP_DATE
extern void __sip_freeSipDateFormat _ARGS_((SIP_Pvoid d));

extern void __sip_freeSipTimeFormat _ARGS_((SIP_Pvoid t));

extern void __sip_freeSipDateStruct _ARGS_((SIP_Pvoid d));
#endif /* SIP_DATE */

#ifdef SIP_DATE
#define	__sip_freeSipRetryAfterHeader   __sip_freeSipCommonHeaderWithDate
#else
#define __sip_freeSipRetryAfterHeader __sip_freeSipCommonHeader
#endif
#define __sip_freeSipAuthorizationHeader __sip_freeSipCommonHeader
#define __sip_freeSipContentTypeHeader __sip_freeSipCommonHeader
#define __sip_freeSipAcceptHeader __sip_freeSipCommonHeader
#define __sip_freeSipContentEncodingHeader __sip_freeSipCommonHeader
#define __sip_freeSipContentLengthHeader __sip_freeSipCommonHeader
#define __sip_freeSipViaHeader __sip_freeSipCommonHeader
#define __sip_freeSipToHeader __sip_freeSipCommonHeader

extern void __sip_freeSipGenericCredential _ARGS_((SIP_Pvoid c));

extern void __sip_freeSipRespHeader _ARGS_((SIP_Pvoid h));

extern void __sip_freeSipUrl _ARGS_((SIP_Pvoid u));

extern void __sip_freeSipAddrSpec _ARGS_((SIP_Pvoid a));

#ifdef SIP_TIMESTAMP
#define __sip_freeSipTimeStampHeader __sip_freeSipCommonHeader
#endif

#define __sip_freeSipRouteHeader __sip_freeSipCommonHeader
#define __sip_freeSipRecordRouteHeader __sip_freeSipCommonHeader
#define __sip_freeSipRequireHeader __sip_freeSipCommonHeader
#define __sip_freeSipFromHeader __sip_freeSipCommonHeader
#define __sip_freeSipContactHeader __sip_freeSipCommonHeader
#define __sip_freeSipCseqHeader __sip_freeSipCommonHeader
#define __sip_freeSipCallIdHeader __sip_freeSipCommonHeader
#define __sip_freeSipUnsupportedHeader __sip_freeSipCommonHeader
#define __sip_freeSipUnknownHeader __sip_freeSipCommonHeader
#define __sip_freeSipReqLine __sip_freeSipCommonHeader
#define __sip_freeSipStatusLine __sip_freeSipCommonHeader
#define __sip_freeSipWwwAuthenticateHeader __sip_freeSipCommonHeader

#define sip_freeSipContentDispositionHeader sip_freeSipCommonHeader
#define __sip_freeSipContentDispositionHeader __sip_freeSipCommonHeader

extern void __sip_freeSipContactParam _ARGS_((SIP_Pvoid c));

extern void __sip_freeSipReqHeader _ARGS_((SIP_Pvoid s));

extern void __sip_freeSipGeneralHeader _ARGS_((SIP_Pvoid g));

extern void __sip_freeSipReqMessage _ARGS_((SIP_Pvoid r));

extern void __sip_freeSipRespMessage _ARGS_((SIP_Pvoid r));

extern void __sip_freeSipHeaderOrderInfo _ARGS_((SIP_Pvoid h));

extern void __sip_freeSipMsgBody _ARGS_((SIP_Pvoid s));

#ifdef SIP_IMPP
#define sip_impp_freeSipSubscriptionStateHeader  sip_freeSipCommonHeader
#define sip_freeSipEventHeader sip_freeSipCommonHeader
#define  __sip_freeSipEventHeader __sip_freeSipCommonHeader

#define sip_impp_freeSipEventHeader sip_freeSipCommonHeader

#define sip_freeSipAllowEventsHeader sip_freeSipCommonHeader
#define __sip_freeSipAllowEventsHeader __sip_freeSipCommonHeader
#endif /* SIP_IMPP */

#ifdef SIP_REPLACES
#define sip_freeSipReplacesHeader sip_freeSipCommonHeader
#define __sip_freeSipReplacesHeader __sip_freeSipCommonHeader
#endif /* SIP_REPLACES */

extern void sip_releaseStack _ARGS_ ((void));

#ifdef SIP_CORE_STACK_COMPATIBLE
#ifdef SIP_AUTHENTICATE
extern void sip_freeSipGenericChallenge _ARGS_((SipGenericChallenge *c));

extern void sip_freeSipGenericCredential _ARGS_((SipGenericCredential *c));
#endif
/*****************************************************************
** FUNCTION:sip_freeSipContactParam
**
**
** DESCRIPTION:
*******************************************************************/

extern void sip_freeSipContactParam(SipContactParam *c);

/****************************************************************************
** FUNCTION: sip_freeSipNameValuePair
** DESCRIPTION: This function frees a SipNameValuePair structure.
** 
** PARAMETERS:
**				n(OUT)			- The structure to be freed 
**
** NOTE: 
****************************************************************************/

extern void sip_freeSipNameValuePair _ARGS_ ((SipNameValuePair *n));

/*#ifdef STK_RELEASE*/
extern void __sip_freeSipNameValuePair _ARGS_ ((SIP_Pvoid n));
/*#endif*/

#endif /* SIP_CORE_STACK_COMPATIBLE */


#ifdef SIP_MWI
extern void sip_mwi_freeSummaryLine _ARGS_((SummaryLine *s));

extern void sip_mwi_freeMesgSummaryMessage _ARGS_((MesgSummaryMessage *m));

extern void __sip_mwi_freeSummaryLine _ARGS_((SIP_Pvoid s));
#endif

#ifdef SIP_3GPP
extern void sip_freeMsrpUrl _ARGS_ ((MsrpUrl *pMsrpUrl));
#endif
extern void __sip_freeSipMessage _ARGS_((SIP_Pvoid s));
#ifdef  SIP_BADMESSAGE_PARSING
extern void sip_freeSipBadHeader _ARGS_((SipBadHeader *u));

extern void __sip_freeSipBadHeader _ARGS_((SIP_Pvoid u));
#endif
#define sip_bcpt_freeSipMimeVersionHeader sip_freeSipCommonHeader
extern void sip_freeSipTimerBuffer(SipTimerBuffer *buf);
extern void sip_freeEventContext(SipEventContext *pContext);
/* CSR 1-6466704 Change Start */
extern void sip_freeSipTranspAddr(SipTranspAddr *t);
/* CSR 1-6466704 Change End */
#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif



