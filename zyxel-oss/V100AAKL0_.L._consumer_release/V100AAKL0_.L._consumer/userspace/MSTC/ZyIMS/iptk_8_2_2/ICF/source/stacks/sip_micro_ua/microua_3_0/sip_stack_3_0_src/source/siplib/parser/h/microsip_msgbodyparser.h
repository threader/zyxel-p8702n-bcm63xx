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
** 12-Feb-2009  Rajiv Kumar     Rel 8.1         Changes done for handling
**                                              icf_tunnelled_sdp_info_st in
**                                              tunnelled mode.
** 22-Oct-2009  Anurag Khare    SPR 20154       Fix for SPR 20154
** 30-Oct-2009  Rajiv Kumar     SPR 20154       Fix for SPR 20154
**
**
**	COPYRIGHT , Aricent, 2009
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

/* Fix for UATK CSR1-7909949: SPR 20154 Start */
SipBool	sip_parseSdpZone(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SIP_S8bit **pZoneLine, SipError *pErr);
/* Fix for UATK CSR 1-7909949 End */

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

/****************************************************************************
** FUNCTION: sip_parseTunnelledSdpTime
** DESCRIPTION: This function parses the time field of an SDP message.
**              The functionality of this function is similar to 
**              sip_parseSdpTime.The only difference is that repeat field ie
**              r= in sdp is also concatenated in the input string with comma
**              seperated value between repeat field and time field.
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pTimeList(OUT) 	- The pointer to the time list of the SDP 
**								  message into which the parsed time field
**								  will be appended. 
**				pErr(OUT)		- Error value if any 
**
****************************************************************************/
SipBool	sip_parseTunnelledSdpTime(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pTimeList, SipError *pErr);

/* Fix For SPR:20154*/
/****************************************************************************
** FUNCTION: sip_parseTunnelledSdpRepeatField
** DESCRIPTION: This function parses the repeat field of an SDP message
**              Tunnelled mode. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse .
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pRepList(OUT) 	- The pointer to the repeat list of the SDP 
**								  message into which the parsed repeat field
**								  will be appended. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool	sip_parseTunnelledSdpRepeatField(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pRepList, SipError *pErr);

#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif

