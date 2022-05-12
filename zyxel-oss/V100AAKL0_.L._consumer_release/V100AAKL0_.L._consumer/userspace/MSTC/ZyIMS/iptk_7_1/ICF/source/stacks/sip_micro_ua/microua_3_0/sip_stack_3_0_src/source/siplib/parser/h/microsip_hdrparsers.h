/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
** FUNCTION:
**			Prototypes for internal header parsing functions
************************************************************************
**
** FILENAME:
** microsip_hdrparser.h
**
** DESCRIPTION:
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 15/02/2002	Aparna Kuppachi		-			Initial					
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/

#ifndef __MICROSIP_HDRPARSER_H__
#define __MICROSIP_HDRPARSER_H__

#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_trace.h"


#ifdef SIP_TIMESTAMP			
SipBool	sip_parseTimeStampHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);
#endif
SipBool	sip_parseContentTypeHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);

SipBool	sip_parseCSeqHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);

SipBool	sip_parseContentDispositionHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);
#ifdef SIP_PRIVACY
SipBool	sip_parsePrivacyHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);
#endif

SipBool	sip_parseFromHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);

SipBool	sip_parseStringHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);
#ifdef SIP_WARNING
SipBool	sip_parseWarningHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);

SipBool	sip_parseWarningValue(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipWarningHeader **ppWarningHdr, SipError *pErr);
#endif	
#ifdef SIP_RPR
SipBool	sip_parseRAckHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);
#endif
#if defined(SIP_AUTHENTICATE) || defined(SIP_3GPP)
SipBool	sip_parseAuthHeaders(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);
#endif
SipBool	sip_parseRequestLine(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipReqLine *pReqLine, SipError *pErr);

SipBool	sip_parseSipVersion(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SIP_S8bit **ppVersion, SipError *pErr);

SipBool	sip_parseStatusLine(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipStatusLine *pStatusLine, SipError *pErr);

SipBool	sip_parseContentLengthHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);

SipBool	sip_parseViaHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);

SipBool	sip_parseContactHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);

SipBool	sip_parseMimeVersionHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);

SipBool	sip_parseUnknownHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
			SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);
	
SipBool	sip_parseReferToHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
			SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);

SipBool	sip_parseRetryAfterHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
			SipHeader *pOutHdr,  en_HeaderType dType, SipError *pErr);

SipBool	sip_parseExpiresHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);
#ifdef SIP_SECURITY

#define sip_parseSecurityClientHeader					sip_parseContentDispositionHeader
#define sip_parseSecurityVerifyHeader					sip_parseContentDispositionHeader
#define sip_parseSecurityServerHeader					sip_parseContentDispositionHeader

#endif

#ifdef SIP_3GPP
SipBool	sip_parse3GPPHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd,
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);

#define sip_parsePathHeader					sip_parseFromHeader
#define sip_parsePAssociatedUriHeader		sip_parseFromHeader
#define sip_parsePCalledPartyIdHeader		sip_parseFromHeader
#define sip_parsePVisitedNetworkIdHeader	sip_parse3GPPHeader
#define sip_parsePcfAddrHeader				sip_parse3GPPHeader
#define sip_parsePanInfoHeaders             sip_parse3GPPHeader
#define sip_parsePcVectorHeader             sip_parse3GPPHeader
#define sip_parseServiceRouteHeader			sip_parseFromHeader		
#define sip_parseHistoryInfoHeader			sip_parseFromHeader
#define sip_parseRequestDispositionHeader	sip_parseStringHeader 
#define	sip_parseAcceptContactHeader 		sip_parse3GPPHeader
#define sip_parseRejectContactHeader 		sip_parse3GPPHeader
#define	sip_parseJoinHeader 				sip_parse3GPPHeader			
#define sip_parseIfMatchHeader				sip_parseStringHeader
#define sip_parseETagHeader					sip_parseStringHeader
#endif

#define sip_parseMinExpiresHeader	sip_parseExpiresHeader 
#define	sip_parseAcceptHeader 	sip_parseContentTypeHeader

#ifdef SIP_PEMEDIA
#define sip_parsePEarlyMediaHeader	sip_parseStringHeader
#endif

#ifdef SIP_PRIVACY
#define sip_parsePAssertIdHeader				sip_parseFromHeader
#define sip_parsePPreferredIdHeader			sip_parseFromHeader

#endif

#ifdef SIP_CONGEST
SipBool	sip_parseResourcePriorityHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr);
#endif

#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif
