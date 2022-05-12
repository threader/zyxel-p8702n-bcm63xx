/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
** FUNCTION:
**			Prototypes for internal Message Body parsing functions
************************************************************************
**
** FILENAME:
** microsip_msgbodyparser.h
**
** DESCRIPTION:
**
** DATE	 		NAME		 	REFERENCE	 	REASON
** ----	 		----		 	---------	 	--------
** 26/02/2002	Aparna Kuppachi	    -			Initial
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/

#ifndef __MICROSIP_MSGBODYPARSER_H__
#define __MICROSIP_MSGBODYPARSER_H__

#include <ctype.h>
#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_init.h"
#include "microsip_free.h"
#include "microsip_bcptinit.h"
#include "microsip_bcptfree.h"
#include "microsip_subparsers.h"
#include "microsip_trace.h"

#define SIP_SKIPSPACE(a) \
    while( ( *(a) == ' ') || ( *(a) == '\t') ) a++
    

SipBool	sip_parseMessageBody(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipMessage *pSipMesg, SipEventContext *pContext, SipOptions *pOpt,\
	SipError *pErr);

SipBool	sip_parseSdpMessage(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SdpMessage *pSdpMsg, SipOptions *pOpt, SipError *pErr);

SipBool	sip_parseSdpVersion(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SdpMessage *pSdpMsg, SipError *pErr);

SipBool	sip_parseSdpOrigin(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SdpMessage *pSdpMsg, SipError *pErr);

SipBool	sip_parseSdpSessionNameOrSessionInfo(SIP_S8bit *pStart, \
	SIP_S8bit *pEnd, SIP_S8bit **ppNewCurrentPos, SIP_S8bit **ppSessionName, \
	en_SipBoolean isSessionName, SipError *pErr);

SipBool	sip_parseSdpConnectionInfo(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SdpConnection **ppConnInfo, SipError *pErr);

SipBool	sip_parseSdpBandwidthInfo(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pBwList, SipError *pErr);

SipBool	sip_parseSdpTime(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pTimeList, SipError *pErr);

SipBool	sip_parseSdpRepeatField(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pRepList, SipError *pErr);

SipBool	sip_parseSdpAttr(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pAttrList, SipError *pErr);

SipBool	sip_parseSdpKey(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SIP_S8bit **ppKey, SipError *pErr);

SipBool	sip_parseSdpMediaDescription(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	en_SipBoolean connInfoAtSessionLevel, SIP_S8bit **ppNewCurrentPos, \
	SipList *pMediaList, SipError *pErr);
#ifdef SIP_MIME_PARSING
SipBool sip_parseMimeEntity(SIP_U16bit dLevel, SIP_S8bit *pStart, \
	SIP_S8bit *pEnd, SipMessage *pSipMesg, MimeMessage *pMimeMsg, \
	SipEventContext *pContext, SipOptions *pOpt, SipMimeHeader *pMHdr, \
	SipError *pErr);
#endif
SipBool sip_parseSdpAddress(void);

SipBool sip_parseSdpConnectionAddress(void);

SIP_S8bit *sip_glbGetBoundaryVal(void);

void sip_glbSetBoundaryVal(SIP_S8bit *pPtr);

SipBool sip_parseSdpEmailAddrOrPhoneNum(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pEmailList, SipError *pErr);
#ifdef SIP_MIME_PARSING
SipBool sip_parseSingleMimeEntity(SIP_U16bit dLevel, \
	en_SipMsgBodyType contentType, SIP_S8bit *pStartOfBody, \
	SIP_S8bit *pEndOfBody, SipMessage *pSipMesg, MimeMessage *pMimeMsg, \
	SipEventContext *pContext, SipOptions *pOpt, SipMimeHeader *pMimeHdr, \
	SipError *pErr);
#endif	

#ifdef SIP_MWI

SipBool	sip_parseMwiMessage(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	MesgSummaryMessage  *pSummaryMessage, SipOptions *pOpt, SipError *pErr);

SipBool sip_parseMwiSummaryLine(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, MesgSummaryMessage *pSummaryMessage, \
    SIP_U32bit dSummaryIndex,SipError *pErr);

#endif

#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif

