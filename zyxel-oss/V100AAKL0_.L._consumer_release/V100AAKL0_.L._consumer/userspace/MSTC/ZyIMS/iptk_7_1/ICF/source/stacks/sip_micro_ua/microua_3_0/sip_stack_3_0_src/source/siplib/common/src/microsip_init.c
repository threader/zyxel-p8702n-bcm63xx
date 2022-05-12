/***********************************************************************
** FUNCTION:
**		  Init Functions for all the stack structures
************************************************************************
**
** FILENAME:
** microsip_init.c
**
** DESCRIPTION:
**		This file contains init functions for all the structures used by
**		the stack.
**
** DATE			NAME			REFERENCE      REASON
** ----			----			---------      --------
** 20/02/2002 Aparna Kuppachi 		-		Core stk sipinit.c
**											plus optimizations. 
**
**	COPYRIGHT , Aricent, 2006
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "microsip_struct.h"
#include "microsip_free.h"
#include "microsip_init.h"
#include "microsip_statistics.h"
#include "microsip_subapi.h"

#ifdef SIP_THREAD_SAFE
extern synch_id_t glbLockStatisticsMutex;
#endif
SIP_S8bit* hss_sip_part_id = (SIP_S8bit *)SIP_PART_ID;

/****************************************************************************
** FUNCTION: sip_initSipTimerKey
** DESCRIPTION: This function allocates and inits a SipTimerKey structure. 
** 
** PARAMETERS:
**				k(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSipTimerKey
#ifdef ANSI_PROTO
	(SipTimerKey **k,SipError *pErr)
#else
	(k,pErr)
	SipTimerKey **k;
	SipError *pErr;	
#endif
{
	*k=(SipTimerKey *)fast_memget(INIT_MEM_ID, sizeof(SipTimerKey), pErr);
	if (*k == SIP_NULL)
		return SipFail;
	
	*k = (SipTimerKey*)sip_memset((SIP_Pvoid)*k, 0, sizeof(SipTimerKey));		
	HSS_INITREF((*k)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_initSdpOrigin
** DESCRIPTION: This function allocates and inits an SdpOrigin structure. 
** 
** PARAMETERS:
**				s(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSdpOrigin
#ifdef ANSI_PROTO
	(SdpOrigin **s,SipError *pErr)
#else
	(s,pErr)
	SdpOrigin **s;
	SipError *pErr;
#endif
{
	*s=(SdpOrigin *)fast_memget(INIT_MEM_ID, sizeof(SdpOrigin),pErr);
	if (*s == SIP_NULL)
		return SipFail;
	*s=(SdpOrigin*)sip_memset((SIP_Pvoid)*s, 0, sizeof(SdpOrigin));		
	HSS_INITREF((*s)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_initSdpMedia
** DESCRIPTION: This function allocates and inits an SdpMedia structure. 
** 
** PARAMETERS:
**				m(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSdpMedia
#ifdef ANSI_PROTO
	(SdpMedia **m,SipError *pErr)
#else
	(m,pErr)
	SdpMedia **m;
	SipError *pErr;
#endif
{
	*m=(SdpMedia *)fast_memget(INIT_MEM_ID, sizeof(SdpMedia),pErr);
	if (*m == SIP_NULL)
		return SipFail;
	*m=(SdpMedia*)sip_memset((SIP_Pvoid)*m, 0, sizeof(SdpMedia));		
	(void)sip_listInit(& ((*m)->slConnection ),__sip_freeSdpConnection,pErr);
	(void)sip_listInit(& ((*m)->slBandwidth ),__sip_freeString,pErr);
	(void)sip_listInit(& ((*m)->slAttr ),__sip_freeSdpAttr,pErr);
	(void)sip_listInit(& ((*m)->slIncorrectLines ),__sip_freeString,pErr);
	HSS_INITREF((*m)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_initSdpAttr
** DESCRIPTION: This function allocates and inits an SdpAttr structure. 
** 
** PARAMETERS:
**				a(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSdpAttr
#ifdef ANSI_PROTO
	(SdpAttr **a,SipError *pErr)
#else
	(a,pErr)
	SdpAttr **a;
	SipError *pErr;
#endif
{
	*a=(SdpAttr *)fast_memget(INIT_MEM_ID, sizeof(SdpAttr),pErr);
	if (*a == SIP_NULL)
		return SipFail;
	*a = (SdpAttr*)sip_memset((SIP_Pvoid)*a, 0, sizeof(SdpAttr));		
	HSS_INITREF((*a)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_initSdpTime
** DESCRIPTION: This function allocates and inits an SdpTime structure. 
** 
** PARAMETERS:
**				t(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSdpTime 
#ifdef ANSI_PROTO
	(SdpTime **t,SipError *pErr)
#else
	(t,pErr)
	SdpTime **t;
	SipError *pErr;
#endif
{
	*t= (SdpTime *)fast_memget(INIT_MEM_ID, sizeof(SdpTime),pErr);
	if (*t == SIP_NULL)
		return SipFail;
	*t = (SdpTime*)sip_memset((SIP_Pvoid)*t, 0, sizeof(SdpTime));		
	(void)sip_listInit(& ((*t)->slRepeat),__sip_freeString,pErr);
	HSS_INITREF((*t)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_initSdpConnection
** DESCRIPTION: This function allocates and inits an SdpConnection structure. 
** 
** PARAMETERS:
**				c(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSdpConnection
#ifdef ANSI_PROTO
	( SdpConnection **c,SipError *pErr)
#else
	(c,pErr)
	SdpConnection **c;
	SipError *pErr;
#endif
{
	*c = (SdpConnection *)fast_memget(INIT_MEM_ID, sizeof( SdpConnection),pErr);
	if (*c == SIP_NULL)
		return SipFail;
	*c = (SdpConnection*)sip_memset((SIP_Pvoid)*c, 0, sizeof(SdpConnection));		
	HSS_INITREF((*c)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_initSdpMessage
** DESCRIPTION: This function allocates and inits an SdpMessage structure. 
** 
** PARAMETERS:
**				m(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSdpMessage 
#ifdef ANSI_PROTO
	(SdpMessage **m,SipError *pErr)
#else
	(m,pErr)
	SdpMessage **m;
	SipError *pErr;
#endif
{
	*m = (SdpMessage *)fast_memget(INIT_MEM_ID, sizeof(SdpMessage),pErr);
	if (*m == SIP_NULL)
		return SipFail;
	*m = (SdpMessage*)sip_memset((SIP_Pvoid)*m, 0, sizeof(SdpMessage));		
	(void)sip_listInit(& ((*m)->slEmail ),__sip_freeString,pErr);
	(void)sip_listInit(& ((*m)->pBandwidth ),__sip_freeString,pErr);
	(void)sip_listInit(& ((*m)->slPhone ),__sip_freeString,pErr);
	(void)sip_listInit(& ((*m)->slTime ),__sip_freeSdpTime,pErr);
	(void)sip_listInit(& ((*m)->slAttr ),__sip_freeSdpAttr,pErr);
	(void)sip_listInit(& ((*m)->slMedia ),__sip_freeSdpMedia,pErr);
	(void)sip_listInit(& ((*m)->slIncorrectLines ),__sip_freeString,pErr);
	HSS_INITREF((*m)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_initSipParam
** DESCRIPTION: This function allocates and inits a SipParam structure. 
** 
** PARAMETERS:
**				p(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSipParam
#ifdef ANSI_PROTO
	(SipParam **p,SipError *pErr)
#else
	(p,pErr)
	SipParam **p;
	SipError *pErr;	
#endif
{
	*p = ( SipParam * )fast_memget(INIT_MEM_ID, sizeof(SipParam), pErr);
	if (*p == SIP_NULL)
		return SipFail;
	*p = (SipParam*)sip_memset((SIP_Pvoid)*p, 0, sizeof(SipParam));		
	(void)sip_listInit(&((*p)->slValue),__sip_freeString,pErr);
	HSS_INITREF((*p)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_initSipCommonHeader
** DESCRIPTION: This function allocates and inits the SipCommonHeader structure. 
** 
** PARAMETERS:
**				ppHeader(OUT)	- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool sip_initSipCommonHeader
	(SipCommonHeader    **ppHeader,
	 SipError           *pErr)
{
	*ppHeader = (SipCommonHeader *)fast_memget(INIT_MEM_ID, \
		sizeof(SipCommonHeader),pErr);
	if (*ppHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
	*ppHeader = (SipCommonHeader*)sip_memset((SIP_Pvoid)*ppHeader, 0, \
		sizeof(SipCommonHeader));		
    INIT((*ppHeader)->pStr1);
    INIT((*ppHeader)->pStr2);
    INIT((*ppHeader)->pAddrSpec);
	(void)sip_listInit(&((*ppHeader)->slParam),__sip_freeSipParam,pErr);
#ifdef SIP_INCREMENTAL_PARSING	
    INIT((*ppHeader)->pBuffer);
#endif
	HSS_INITREF((*ppHeader)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}
/****************************************************************************
** FUNCTION: sip_initSipRespHeader
** DESCRIPTION: This function allocates and inits a SipRespHeader structure. 
** 
** PARAMETERS:
**				h(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSipRespHeader
#ifdef ANSI_PROTO
	(SipRespHeader **h,SipError *pErr)
#else
	(h,pErr)
	SipRespHeader **h;
	SipError *pErr;
#endif
{
	*h = (SipRespHeader *)fast_memget(INIT_MEM_ID, sizeof(SipRespHeader),pErr);
	if (*h == SIP_NULL)
		return SipFail;
	*h = (SipRespHeader*) sip_memset((SIP_Pvoid)*h, 0, sizeof(SipRespHeader));		
	(void)sip_listInit(& ((*h)->slUnsupportedHdr),__sip_freeSipUnsupportedHeader,pErr);
#ifdef SIP_WARNING
	(void)sip_listInit(& ((*h)->slWarningHeader),__sip_freeSipWarningHeader,pErr);
#endif	
#ifdef SIP_AUTHENTICATE	
	(void)sip_listInit(& ((*h)->slProxyAuthenticateHdr),\
		__sip_freeSipProxyAuthenticateHeader,pErr);
	(void)sip_listInit(& ((*h)->slWwwAuthenticateHdr),\
		__sip_freeSipWwwAuthenticateHeader,pErr);
#endif		
#ifdef SIP_3GPP
	(void)sip_listInit(& ((*h)->slPAssociatedUriHdr),__sip_freeSipRecordRouteHeader,pErr);
    (void)sip_listInit(& ((*h)->slServiceRouteHdr),__sip_freeSipRecordRouteHeader,pErr);

#endif
#ifdef SIP_SECURITY
	(void)sip_listInit(& ((*h)->slSecurityServerHdr),__sip_freeSipSecurityServerHeader,pErr);
#endif  
#ifdef SIP_CONGEST
	(void)sip_listInit(& ((*h)->slAcceptRsrcPriorityHdr),__sip_freeSipAcceptRsrcPriorityHeader,pErr);
#endif
HSS_INITREF((*h)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_initSipUrl
** DESCRIPTION: This function allocates and inits a SipUrl structure. 
** 
** PARAMETERS:
**				u(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSipUrl
#ifdef ANSI_PROTO
	(SipUrl **u,SipError *pErr)
#else
	(u,pErr)
	SipUrl **u;
	SipError *pErr;
#endif
{
	*u = (SipUrl *)fast_memget(INIT_MEM_ID, sizeof(SipUrl),pErr);
	if (*u == SIP_NULL)
		return SipFail;
	*u = (SipUrl*)sip_memset((SIP_Pvoid)*u, 0, sizeof(SipUrl));		
	(void)sip_listInit(& ((*u)->slParam),__sip_freeSipParam,pErr); 
	HSS_INITREF((*u)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_initSipAddrSpec
** DESCRIPTION: This function allocates and inits a SipAddrSpec structure. 
** 
** PARAMETERS:
**				a(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSipAddrSpec
#ifdef ANSI_PROTO
	(SipAddrSpec **a,en_AddrType dType,SipError *pErr)
#else
	(a,dType,pErr)
	SipAddrSpec **a;
	en_AddrType dType;	
	SipError *pErr;
#endif
{
	*a = (SipAddrSpec *)fast_memget(INIT_MEM_ID, sizeof(SipAddrSpec),pErr);
	if (*a == SIP_NULL)
		return SipFail;
	(*a)->dType=dType;/**got to validate**/
	if ((*a)->dType == SipAddrReqUri)
		INIT((*a)->u.pUri);
	else if (((*a)->dType == SipAddrSipUri) || ((*a)->dType == SipAddrSipSUri))
		INIT((*a)->u.pSipUrl);
	HSS_INITREF((*a)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_initSipContactHeader
** DESCRIPTION: This function allocates and inits a SipContactHeader structure. 
** 
** PARAMETERS:
**				ppHeader(OUT)	- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSipContactHeader
	(SipContactHeader **ppHeader,
     en_ContactType   dType, 
     SipError         *pErr)
{
    if(SipFail == sip_initSipCommonHeader(ppHeader,pErr))
    {
        return SipFail;
    }
	(*ppHeader)->dIntVar1 = dType;
	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipReqHeader
** DESCRIPTION: This function allocates and inits a SipReqHeader structure. 
** 
** PARAMETERS:
**				s(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipReqHeader
#ifdef ANSI_PROTO
	(SipReqHeader **s,SipError *pErr)
#else
	(s,pErr)
	SipReqHeader **s;
	SipError *pErr;
#endif
{
	*s = (SipReqHeader *)fast_memget(INIT_MEM_ID, sizeof(SipReqHeader),pErr);
	if (*s == SIP_NULL)
		return SipFail;
	*s = (SipReqHeader*)sip_memset((SIP_Pvoid)*s, 0, sizeof(SipReqHeader));		
	(void)sip_listInit(& ((*s)->slRouteHdr),__sip_freeSipRouteHeader,pErr);
#ifdef SIP_AUTHENTICATE	
	(void)sip_listInit(& ((*s)->slAuthorizationHdr),\
		__sip_freeSipAuthorizationHeader,pErr);
	(void)sip_listInit(& ((*s)->slProxyAuthorizationHdr),\
		__sip_freeSipProxyAuthorizationHeader,pErr);
#endif		
#ifdef SIP_3GPP
	(void)sip_listInit(& ((*s)->slPVisitedNetworkIdHdr),\
		__sip_freeSipRecordRouteHeader,pErr);
	(void)sip_listInit(& ((*s)->slRequestDispositionHdr),\
		__sip_freeSipUnsupportedHeader,pErr);
	(void)sip_listInit(& ((*s)->slAcceptContactHdr),\
		__sip_freeSipRecordRouteHeader,pErr);
	(void)sip_listInit(& ((*s)->slRejectContactHdr),\
		__sip_freeSipRecordRouteHeader,pErr);

#endif
#ifdef SIP_SECURITY
	(void)sip_listInit(& ((*s)->slSecurityClientHdr),\
		__sip_freeSipSecurityClientHeader,pErr);
	(void)sip_listInit(& ((*s)->slSecurityVerifyHdr),\
		__sip_freeSipSecurityVerifyHeader,pErr);

#endif   
#ifdef SIP_CONGEST
(void)sip_listInit(& ((*s)->slRsrcPriorityHdr),\
		__sip_freeSipRsrcPriorityHeader,pErr);
#endif
HSS_INITREF((*s)->dRefCount);/* Initializing the reference count to 1 */

	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipGeneralHeader
** DESCRIPTION: This function allocates and inits a SipGeneralHeader structure. 
** 
** PARAMETERS:
**				g(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipGeneralHeader
#ifdef ANSI_PROTO
	(SipGeneralHeader **g,SipError *pErr)
#else
	(g,pErr)
	SipGeneralHeader **g;
	SipError *pErr;
#endif
{
	*g=(SipGeneralHeader *)fast_memget(INIT_MEM_ID, sizeof(SipGeneralHeader),\
		pErr);
	if (*g == SIP_NULL)
		return SipFail;
	*g =(SipGeneralHeader*) sip_memset((SIP_Pvoid)*g, 0, \
		sizeof(SipGeneralHeader));		
	(void)sip_listInit(& ((*g)->slContactHdr),__sip_freeSipContactHeader,pErr);
	(void)sip_listInit(& ((*g)->slRecordRouteHdr),__sip_freeSipRecordRouteHeader,pErr);
	(void)sip_listInit(& ((*g)->slViaHdr),__sip_freeSipViaHeader,pErr);
	(void)sip_listInit(& ((*g)->slContentEncodingHdr),\
		__sip_freeSipContentEncodingHeader,pErr);
	(void)sip_listInit(& ((*g)->slContentDispositionHdr), \
		__sip_freeSipContentDispositionHeader,pErr);
	(void)sip_listInit(& ((*g)->slUnknownHdr),__sip_freeSipUnknownHeader,pErr);
	(void)sip_listInit(& ((*g)->slRequireHdr),__sip_freeSipRequireHeader,pErr);
	(void)sip_listInit(& ((*g)->slSupportedHdr),__sip_freeSipSupportedHeader,pErr);
	(void)sip_listInit(& ((*g)->slAllowHdr),__sip_freeSipAllowHeader,pErr);
	(void)sip_listInit(& ((*g)->slAcceptHdr),__sip_freeSipAcceptHeader,pErr);

#ifdef SIP_3GPP
	(void)sip_listInit(& ((*g)->slPathHdr),__sip_freeSipRecordRouteHeader,pErr);
	(void)sip_listInit(& ((*g)->slHistoryInfoHdr),__sip_freeSipRecordRouteHeader,pErr);
	
#endif
#ifdef SIP_PRIVACY
	(void)sip_listInit(& ((*g)->slPAssertIdHdr),__sip_freeSipPAssertIdHeader,pErr);
  (void)sip_listInit(& ((*g)->slPPreferredIdHdr),__sip_freeSipPPreferredIdHeader,pErr);
#endif
#ifdef SIP_IMPP
	(void)sip_listInit(& ((*g)->slAllowEventsHdr),__sip_freeSipAllowEventsHeader,pErr);
#endif
#ifdef  SIP_BADMESSAGE_PARSING
	(void)sip_listInit(& ((*g)->slBadHdr),__sip_freeSipBadHeader,pErr);
#endif	
#ifdef SIP_PEMEDIA	
	(void)sip_listInit(& ((*g)->slPEarlyMediaHdr),__sip_freeSipPEarlyMediaHeader,pErr);
#endif
	HSS_INITREF((*g)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipReqMessage
** DESCRIPTION: This function allocates and inits a SipReqMessage structure. 
** 
** PARAMETERS:
**				r(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipReqMessage
#ifdef ANSI_PROTO
	(SipReqMessage **r,SipError *pErr)
#else
	(r,pErr)
	SipReqMessage **r;
	SipError *pErr;
#endif
{
	*r= (SipReqMessage *)fast_memget(INIT_MEM_ID, sizeof(SipReqMessage),pErr);
	if (*r==SIP_NULL)
		return SipFail;
	(void)sip_initSipReqLine(&(*r)->pRequestLine,pErr);
	(void)sip_initSipReqHeader(&(*r)->pRequestHdr,pErr);
	HSS_INITREF((*r)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}

/*********************************************************************************
** FUNCTION: sip_initSipRespMessage
** DESCRIPTION: This function allocates and inits a SipRespMessage structure. 
** 
** PARAMETERS:
**				r(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipRespMessage
#ifdef ANSI_PROTO
	(SipRespMessage **r, SipError *pErr)
#else
	(r,pErr)
	SipRespMessage **r;
	SipError *pErr;
#endif
{
	*r= (SipRespMessage *)fast_memget(INIT_MEM_ID, sizeof(SipRespMessage),pErr);
	if (*r == SIP_NULL)
		return SipFail;
	(void)sip_initSipStatusLine(&(*r)->pStatusLine,pErr);
	(void)sip_initSipRespHeader(&(*r)->pResponseHdr,pErr);
	HSS_INITREF((*r)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipHeaderOrderInfo
** DESCRIPTION: This function allocates and inits a SipHeaderOrderInfo structure. 
** 
** PARAMETERS:
**				order(OUT)		- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipHeaderOrderInfo
#ifdef ANSI_PROTO
	(SipHeaderOrderInfo **order, SipError *pErr)
#else
	(order, pErr)
	SipHeaderOrderInfo **order;
	SipError *pErr;
#endif
{
	*order= (SipHeaderOrderInfo *)fast_memget(INIT_MEM_ID, \
		sizeof(SipHeaderOrderInfo),pErr);
	if (*order == SIP_NULL)
		return SipFail;

	*pErr = E_NO_ERROR;
	HSS_INITREF((*order)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipUnknownMessage
** DESCRIPTION: This function allocates and inits a SipUnknownMessage structure. 
** 
** PARAMETERS:
**				s(OUT)			- The allocated structure 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipUnknownMessage
#ifdef ANSI_PROTO
	(SipUnknownMessage **s,SipError *pErr)
#else
	(s,pErr)
	SipUnknownMessage **s;
	SipError *pErr;
#endif
{
	*s= (SipUnknownMessage *)fast_memget(INIT_MEM_ID, \
		sizeof(SipUnknownMessage),pErr);
	if (*s == SIP_NULL)
		return SipFail;
	INIT( (*s)->pBuffer );
	(*s)->dLength = 0;
	HSS_INITREF((*s)->dRefCount);/* Initializing the reference count to 1 */
	
	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipMsgBody
** DESCRIPTION: This function allocates and inits a SipMsgBody structure. 
** 
** PARAMETERS:
**				s(OUT)			- The allocated structure 
**				dType(IN)		- The message body type
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipMsgBody
#ifdef ANSI_PROTO
	(SipMsgBody **s,en_SipMsgBodyType dType,SipError *pErr)
#else
	(s,dType, pErr)
	SipMsgBody **s;
	en_SipMsgBodyType dType;
	SipError *pErr;
#endif
{
	*s= (SipMsgBody *)fast_memget(INIT_MEM_ID, sizeof(SipMsgBody),pErr);
	if (*s == SIP_NULL)
		return SipFail;
	(*s)->dType=dType;
	switch (dType)
	{
		case SipSdpBody :
			INIT( (*s)->u.pSdpMessage );
			break;
		case SipUnknownBody :
			INIT( (*s)->u.pUnknownMessage);
			break;
#ifdef SIP_ISUP			
		case SipIsupBody :
			INIT( (*s)->u.pIsupMessage);
			break;
#endif			
#ifdef SIP_SIPMSGBODY
		case SipAppSipBody :
			INIT( (*s)->u.pAppSipMessage);
			break;
#endif		
#ifdef SIP_MWI
            case SipMessageSummaryBody :
			INIT( (*s)->u.pSummaryMessage);
			break;
#endif
#ifdef SIP_MIME_PARSING			
		case SipMultipartMimeBody :
			INIT( (*s)->u.pMimeMessage);
			break;
#endif			
		case SipBodyAny :
			/*INIT( (*s)->u.pMimeMessage);*/
			break;
		default :
			*pErr = E_INV_TYPE;
			return SipFail;
	}
#ifdef SIP_MIME_PARSING			
	INIT( (*s)->pMimeHeader);
#endif	
	*pErr = E_NO_ERROR;
	HSS_INITREF((*s)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipMessage
** DESCRIPTION: This function allocates and inits a SipMessage structure. 
** 
** PARAMETERS:
**				s(OUT)			- The allocated structure 
**				dType(IN)		- The SIP message type
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipMessage
#ifdef ANSI_PROTO
	(SipMessage **s,en_SipMessageType dType,SipError *pErr)
#else
	(s,dType, pErr)
	SipMessage **s;
	en_SipMessageType dType;
	SipError *pErr;
#endif
{
	*s= (SipMessage *)fast_memget(INIT_MEM_ID, sizeof(SipMessage),pErr);
	if (*s == SIP_NULL)
		return SipFail;

	(*s)->dType=dType;
	(void)sip_initSipGeneralHeader(&(*s)->pGeneralHdr,pErr);
	if ((*s)->dType == SipMessageRequest)
		(void)sip_initSipReqMessage(&(*s)->u.pRequest,pErr);
	else if ((*s)->dType==SipMessageResponse)
		(void)sip_initSipRespMessage(&(*s)->u.pResponse,pErr);
	
	if (sip_listInit( &((*s)->slOrderInfo), __sip_freeSipHeaderOrderInfo, \
		pErr) == SipFail)
		return SipFail;

	if (sip_listInit( &((*s)->slMessageBody), __sip_freeSipMsgBody, pErr) == \
		SipFail)
		return SipFail;

	HSS_INITREF((*s)->dRefCount);/* Initializing the reference count to 1 */

	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipHeader
** DESCRIPTION: This function allocates and inits a SipHeader structure. 
** 
** PARAMETERS:
**				ppHdr(OUT)		- The allocated structure 
**				dType(IN)		- The header type
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipHeader
	(SipHeader      **ppHdr,
     en_HeaderType  dType,
     SipError       *pErr)
{
	if(dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	*ppHdr= (SipHeader *)fast_memget(INIT_MEM_ID, sizeof(SipHeader),pErr);
	if (*ppHdr == SIP_NULL)
		return SipFail;

	(*ppHdr)->dType=dType;

	/* 
	 * Now init  pHeader; we need to do this as the lower dLevel pHeader 
	 * APIs are never invoked directly by the pUser. If it were invoked, 
	 * the yyySetxxxxx fns would automatically allocate space
	 */
    if(dType == SipHdrTypeContactNormal)
    {
        (void)sip_initSipContactHeader((SipContactHeader **) &(*ppHdr)->pHeader, 
                SipContactNormal, pErr);
    }
    else if(dType == SipHdrTypeContactWildCard)
    {
        (void)sip_initSipContactHeader((SipContactHeader **) &(*ppHdr)->pHeader,
                SipContactWildCard, pErr);
    }
    else if(dType == SipHdrTypeContactAny)
    {
        (void)sip_initSipContactHeader((SipContactHeader **) &(*ppHdr)->pHeader, 
                SipContactAny, pErr);
    }
#ifdef SIP_DATE
    else if((dType == SipHdrTypeExpiresDate)||
            (dType == SipHdrTypeRetryAfterDate))
    {
        (void)sip_initSipCommonHeaderWithDate((SipCommonHeader **) 
                &(*ppHdr)->pHeader, SipExpDate, pErr);
    }else if((dType == SipHdrTypeExpiresAny)||
            (dType == SipHdrTypeRetryAfterAny))
    {
        (void)sip_initSipCommonHeaderWithDate((SipCommonHeader **) 
                &(*ppHdr)->pHeader, SipExpAny, pErr);
    }else if((dType == SipHdrTypeExpiresSec)||
            (dType == SipHdrTypeRetryAfterSec))
    {
        (void)sip_initSipCommonHeaderWithDate((SipCommonHeader **) 
                &(*ppHdr)->pHeader, SipExpSeconds, pErr);
    }
#endif
    else if(dType != SipHdrTypeAny)
    {
        (void)sip_initSipCommonHeader((SipCommonHeader **)&(*ppHdr)->pHeader,pErr);
    }

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_getPart
** DESCRIPTION: This function returns the PART-ID 
** 
** PARAMETERS: None
**
** NOTE: 
*********************************************************************************/

SIP_S8bit *sip_getPart()
{
	return hss_sip_part_id;
}


/*********************************************************************************
** FUNCTION: sip_initStack
** DESCRIPTION: This is the initialization function for the stack. 
** 
** PARAMETERS: None.
**
** NOTE: 
*********************************************************************************/

SipBool sip_initStack(void)
{
#ifdef SIP_STATISTICS
	SipError pError;
#endif
	sip_initStackPortSpecific();

#ifdef SIP_STATISTICS
#ifdef SIP_THREAD_SAFE
	fast_init_synch(&glbLockStatisticsMutex);
#endif
	(void)sip_initStatistics(SIP_STAT_TYPE_API, SIP_STAT_API_ALL,&pError);
	(void)sip_initStatistics(SIP_STAT_TYPE_ERROR, SIP_STAT_ERROR_ALL, &pError);
#endif

	/* These values are in milliseconds */

	SIP_T1 = 500;
	SIP_T2 = 4 * 1000;
	SIP_MAXRETRANS = 10;
	SIP_MAXINVRETRANS = 6;
	return SipSuccess;
}


#ifdef SIP_CORE_STACK_COMPATIBLE
/*********************************************************************************
** FUNCTION: sip_initSipGenericCredential
** DESCRIPTION: This function allocates and inits a SipGenericCredential structure. 
** 
** PARAMETERS:
**				c(OUT)			- The allocated structure
**				dType(IN)		- The credential type
**				pErr(OUT)		- Error value if any
**
** NOTE: 
*********************************************************************************/
#ifdef SIP_AUTHENTICATE
SipBool sip_initSipGenericCredential
#ifdef ANSI_PROTO
	(SipGenericCredential **c,en_CredentialType dType,SipError *pErr)
#else
	(c,dType,pErr)
	SipGenericCredential **c;
	en_CredentialType dType;
	SipError *pErr;
#endif
{
	*c= (SipGenericCredential *)fast_memget(INIT_MEM_ID, \
		sizeof(SipGenericCredential),pErr);
	if (*c == SIP_NULL)
		return SipFail;
	(*c)->dType=dType;
	if ((*c)->dType==SipCredAuth)
	{
		INIT((*c)->u.pChallenge);
	}
	else if ((*c)->dType==SipCredBasic)
	{
		INIT((*c)->u.pBasic);
	}
	else if ((*c)->dType != SipCredAny)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	HSS_INITREF((*c)->dRefCount);
	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipGenericChallenge
** DESCRIPTION: This function allocates and inits a SipGenericChallenge structure. 
** 
** PARAMETERS:
**				c(OUT)			- The allocated structure
**				pErr(OUT)		- Error value if any
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipGenericChallenge
#ifdef ANSI_PROTO
	(SipGenericChallenge **c,SipError *pErr)
#else
	(c,pErr)
	SipGenericChallenge **c;
	SipError *pErr;
#endif
{
	*c= (SipGenericChallenge *)fast_memget(INIT_MEM_ID, \
		sizeof(SipGenericChallenge),pErr);
	if (*c == SIP_NULL)
		return SipFail;
	INIT((*c)->pScheme);
	(void)sip_listInit(& ((*c)->slParam ),__sip_freeSipParam,pErr);
	HSS_INITREF((*c)->dRefCount);
	return SipSuccess;
}

#endif
/*********************************************************************************
** FUNCTION: sip_initSipContactParam
** DESCRIPTION: This function allocates and inits a SipContactParam structure. 
** 
** PARAMETERS:
**				c(OUT)			- The allocated structure
**				dType(IN)		- The Contact Params type
**				pErr(OUT)		- Error value if any
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipContactParam
#ifdef ANSI_PROTO
	(SipContactParam **c,en_ContactParamsType dType,SipError *pErr)
#else
	(c,dType,pErr)
	SipContactParam **c;
	en_ContactParamsType dType;
	SipError *pErr;
#endif
{
#ifndef SIP_NO_CHECK
	if(pErr == SIP_NULL)
		return SipFail;
	if(c == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	
	*c= (SipContactParam *)fast_memget(INIT_MEM_ID, sizeof(SipContactParam),\
		pErr);
	if (*c == SIP_NULL)
		return SipFail;
	(*c)->dType=dType;
	switch ((*c)->dType)
	{
		case SipCParamQvalue:
			INIT((*c)->u.pQValue);
			break;
			
		case SipCParamExpires:
			INIT((*c)->u.pExpire);
			break;

		case SipCParamAction:
			INIT((*c)->u.pAction);
			break;

		case SipCParamExtension:
			INIT((*c)->u.pExtensionAttr);
			break;

#ifdef SIP_3GPP
		case SipCParamFeature:
			INIT((*c)->u.pFeatureAttr);
			break;
#endif		
		case SipCParamAny:
			break;

	} /* end of switch */
	HSS_INITREF((*c)->dRefCount);
	return SipSuccess;
}
#endif /* SIP_CORE_STACK_COMPATIBLE */



/*********************************************************************************
** FUNCTION: sip_initSipBadHeader
** DESCRIPTION: This function allocates and inits a SipBadHeader structure. 
** 
** PARAMETERS:
**				e(OUT)			- The allocated structure
**				dType(IN)		- The header type
**				pErr(OUT)		- Error value if any
**
** NOTE: 
*********************************************************************************/
#ifdef  SIP_BADMESSAGE_PARSING
SipBool sip_initSipBadHeader
#ifdef ANSI_PROTO
	(SipBadHeader **e,en_HeaderType dType,SipError *pErr)
#else
	(e,dType,pErr)
	SipBadHeader **e;
	en_HeaderType dType;
	SipError *pErr;
#endif
{
	*e= (SipBadHeader *)fast_memget(INIT_MEM_ID, sizeof(SipBadHeader),pErr);
	if (*e == SIP_NULL)
		return SipFail;
	INIT((*e)->pName);
	INIT((*e)->pBody);
	(*e)->dType=dType;
	HSS_INITREF(((*e)->dRefCount)); /* Initializing the reference count to 1 */
	return SipSuccess;
}

#endif
/*********************************************************************************
** FUNCTION: sip_initSipTimerBuffer
** DESCRIPTION: This function allocates and inits a SipTimerBuffer structure. 
** 
** PARAMETERS:
**				b(OUT)			- The allocated structure
**				pErr(OUT)		- Error value if any
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipTimerBuffer
#ifdef ANSI_PROTO
	(SipTimerBuffer **b,SipError *pErr)
#else
	(b,pErr)
	SipTimerBuffer **b;
	SipError *pErr;
#endif
{
	*b= (SipTimerBuffer *)fast_memget(INIT_MEM_ID, sizeof(SipTimerBuffer), pErr);
	if (*b == SIP_NULL)
		return SipFail;
	INIT((*b)->pBuffer);
	INIT((*b)->pContext);
	HSS_INITREF((*b)->dRefCount);
	return SipSuccess;
}


#ifdef SIP_DATE
/*********************************************************************************
** FUNCTION: sip_initSipCommonHeaderWithDate
** DESCRIPTION: This function allocates and inits a SipCommonHeader structure. 
** 
** PARAMETERS:
**				ppHeader(OUT)	- The allocated structure
**				dType(IN)		- Specifies whether this is Date/delta-sec
**								  format.
**				pErr(OUT)		- Error value if any
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipCommonHeaderWithDate
	(SipCommonHeader 	**ppHeader,
	 en_ExpiresType 	dType,
	 SipError 			*pErr)
{
	*ppHeader = (SipCommonHeader *)fast_memget(INIT_MEM_ID, \
		sizeof(SipCommonHeader),pErr);
	if (*ppHeader == SIP_NULL)
		return SipFail;
	*ppHeader = (SipCommonHeader*)sip_memset((SIP_Pvoid)*ppHeader, 0, \
		sizeof(SipCommonHeader));		
    INIT((*ppHeader)->pStr1);
    INIT((*ppHeader)->pStr2);
	INIT((*ppHeader)->pAddrSpec);
	(void)sip_listInit(&((*ppHeader)->slParam),__sip_freeSipParam,pErr);
	(*ppHeader)->dIntVar1=dType;
	if (dType == SipExpDate)
		INIT((*ppHeader)->u.pDate);
#ifdef SIP_INCREMENTAL_PARSING	
    INIT((*ppHeader)->pBuffer);
#endif
	HSS_INITREF((*ppHeader)->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}

/*********************************************************************************
** FUNCTION: sip_initSipDateFormat
** DESCRIPTION: This function allocates and inits a SipDateFormat structure. 
** 
** PARAMETERS:
**				d(OUT)			- The allocated structure
**				pErr(OUT)		- Error value if any
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipDateFormat
#ifdef ANSI_PROTO
	(SipDateFormat **d,SipError *pErr)
#else
	(d,pErr)
	SipDateFormat **d;
	SipError *pErr;
#endif
{
	*d= (SipDateFormat *) fast_memget(INIT_MEM_ID, sizeof(SipDateFormat),pErr);
	if (*d == SIP_NULL)
		return SipFail;
	HSS_INITREF((*d)->dRefCount);
	/** No pointers so no init necessary **/
	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipTimeFormat
** DESCRIPTION: This function allocates and inits a SipTimeFormat structure. 
** 
** PARAMETERS:
**				t(OUT)			- The allocated structure
**				pErr(OUT)		- Error value if any
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipTimeFormat
#ifdef ANSI_PROTO
	(SipTimeFormat **t,SipError *pErr)
#else
	(t,pErr)
	SipTimeFormat **t;
	SipError *pErr;
#endif
{
	*t= (SipTimeFormat *) fast_memget(INIT_MEM_ID, sizeof(SipTimeFormat),pErr);
	if (*t == SIP_NULL)
		return SipFail;
	/** No pointers so no init necessary **/
	HSS_INITREF((*t)->dRefCount);
	return SipSuccess;
}


/*********************************************************************************
** FUNCTION: sip_initSipDateStruct
** DESCRIPTION: This function allocates and inits a SipDateStruct structure.
** 
** PARAMETERS:
**				d(OUT)			- The allocated structure
**				pErr(OUT)		- Error value if any
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipDateStruct
#ifdef ANSI_PROTO
	(SipDateStruct **d,SipError *pErr)
#else
	(d,pErr)
	SipDateStruct **d;
	SipError *pErr;
#endif
{
	*d= (SipDateStruct *) fast_memget(INIT_MEM_ID, sizeof(SipDateStruct),pErr);
	if (*d == SIP_NULL)
		return SipFail;
	INIT((*d)->pDate);
	INIT((*d)->pTime);
	HSS_INITREF((*d)->dRefCount);
	return SipSuccess;
}
#endif /* SIP_DATE */



#ifdef SIP_CORE_STACK_COMPATIBLE
/*********************************************************************************
** FUNCTION: sip_initSipNameValuePair
** DESCRIPTION: This function allocates and inits a SipNameValuePair structure. 
** 
** PARAMETERS:
**				c(OUT)			- The allocated structure
**				pErr(OUT)		- Error value if any
**
** NOTE: 
*********************************************************************************/

SipBool sip_initSipNameValuePair
#ifdef ANSI_PROTO
        ( SipNameValuePair **c,SipError *pErr)
#else
        (c, pErr)
        SipNameValuePair **c;
        SipError *pErr;
#endif
{
        *c= (SipNameValuePair *)fast_memget(0, sizeof(SipNameValuePair), pErr);
        if (*c==SIP_NULL)
                return SipFail;
        INIT((*c)->pName);
        INIT((*c)->pValue);
        HSS_INITREF((*c)->dRefCount);
        return SipSuccess;
}
#endif /* SIP_CORE_STACK_COMPATIBLE */
#ifdef SIP_MWI
/************************************************************************
**** FUNCTION:sip_mwi_initSummaryLine
****
****
**** DESCRIPTION:
*************************************************************************/

SipBool sip_mwi_initSummaryLine
#ifdef ANSI_PROTO
        ( SummaryLine **c,SipError *err)
#else
        (c,err)
        SummaryLine **c;
        SipError *err;
#endif
{
        *c= ( SummaryLine *) fast_memget(NON_SPECIFIC_MEM_ID, \
                sizeof( SummaryLine),err);
        if (*c==SIP_NULL)
                return SipFail;
        INIT((*c)->pMedia);
        HSS_INITREF((*c)->dRefCount);
		(*c)->newMessages=0;
		(*c)->oldMessages=0;
		(*c)->newUrgentMessages=0;
		(*c)->oldUrgentMessages=0;
        return SipSuccess;
}

/*****************************************************************************
**** FUNCTION:sip_mwi_initMesgSummaryMessage
****
****
**** DESCRIPTION:
*****************************************************************************/
SipBool sip_mwi_initMesgSummaryMessage
   #ifdef ANSI_PROTO
         (MesgSummaryMessage  **ppParam, SipError *err)
   #else
           (ppParam,err)
           MesgSummaryMessage  **ppParam;
           SipError *err;
   #endif
{
	*ppParam = (MesgSummaryMessage *)fast_memget(NON_SPECIFIC_MEM_ID, \
            sizeof(MesgSummaryMessage),err);
        if ( *ppParam == SIP_NULL )
                   return SipFail;
 	(void)sip_listInit(& ((*ppParam)->slSummaryLine ), \
                       __sip_mwi_freeSummaryLine,err);
#ifdef SIP_CORE_STACK_COMPATIBLE
 	(void)sip_listInit(& ((*ppParam)->slNameValue ), \
                       __sip_freeSipNameValuePair,err);
#endif
	/* (*ppParam)->pAddrSpec = SIP_NULL ; */
	INIT((*ppParam)->pAddrSpec);
  HSS_INITREF((*ppParam)->dRefCount);
  return SipSuccess;
}

#endif

/*********************************************************************************
** FUNCTION: sip_initMsrpUrl
** DESCRIPTION: This function allocates and inits a MsrpUrl structure. 
** 
** PARAMETERS:
**				ppMsrpUrl(OUT)		- The allocated structure
**				pErr(OUT)		- Error value if any
**
** NOTE: Thangarajan 
*********************************************************************************/

#ifdef SIP_3GPP

SipBool sip_initMsrpUrl
#ifdef ANSI_PROTO
	(MsrpUrl **ppMsrpUrl, SipError *pErr)
#else
	(ppMsrpUrl,pErr)
	MsrpUrl **ppMsrpUrl;
	SipError *pErr;
#endif
{
	if ((ppMsrpUrl==SIP_NULL)||(pErr==SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	*ppMsrpUrl = (MsrpUrl *)fast_memget(INIT_MEM_ID, sizeof(MsrpUrl),pErr);
	if (*ppMsrpUrl == SIP_NULL)
		return SipFail;
	*ppMsrpUrl = (MsrpUrl*)sip_memset((SIP_Pvoid)*ppMsrpUrl, 0, sizeof(MsrpUrl));		
	HSS_INITREF((*ppMsrpUrl )->dRefCount);/* Initializing the reference count to 1 */
	return SipSuccess;
}

#endif
/*CSR 1-6792719 CSR Change Start*/
SipBool sip_initSipTranspAddr
#ifdef ANSI_PROTO
	(SipTranspAddr **t,SipError *pErr)
#else
	(t,pErr)
	SipTimerBuffer **t;
	SipError *pErr;
#endif
{
	*t= (SipTranspAddr *)fast_memget(INIT_MEM_ID, sizeof(SipTranspAddr), pErr);
	if (*t == SIP_NULL)
    {
    	return SipFail;
    }
	INIT((*t)->pHost);
	INIT((*t)->pReceiversIp);
	HSS_INITREF((*t)->dRefCount);
	return SipSuccess;
}
/*CSR 1-6792719 CSR Change End*/


