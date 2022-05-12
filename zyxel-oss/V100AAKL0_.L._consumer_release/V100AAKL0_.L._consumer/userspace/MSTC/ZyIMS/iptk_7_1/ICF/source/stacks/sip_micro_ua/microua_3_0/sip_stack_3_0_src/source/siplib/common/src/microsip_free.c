/***********************************************************************
** FUNCTION:
**		  Free Functions for all the stack structures
************************************************************************
**
** FILENAME:
** microsip_free.c
**
** DESCRIPTION:
**		This file contains freeing functions for all the structures used by
**		the stack.
**
** DATE			NAME			REFERENCE      REASON
** ----			----			---------      --------
** 20/02/2002 Aparna Kuppachi 		-		Core stk sipfree.c
**
**	COPYRIGHT , Aricent, 2006
************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "microsip_struct.h"
#include "microsip_free.h"
#include "microsip_bcptfree.h"

#ifdef SIP_THREAD_SAFE
extern synch_id_t	glbLockStatisticsMutex;
#endif

/****************************************************************************
** FUNCTION: sip_freeString
** DESCRIPTION: This function frees a string buffer 
** 
** PARAMETERS:
**				s(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeString 
#ifdef ANSI_PROTO
	(SIP_S8bit *s)
#else
	(s)
	SIP_S8bit *s;
#endif
{
	HSS_FREE(s);
}


/****************************************************************************
** FUNCTION: sip_freeSipTimerKey
** DESCRIPTION: This function frees a SipTimerKey structure 
** 
** PARAMETERS:
**				k(IN/OUT)			- The structure to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipTimerKey
#ifdef ANSI_PROTO
	(SipTimerKey *k)
#else
	(k)
	SipTimerKey *k;
#endif
{
	if (k==SIP_NULL) return;
	HSS_LOCKREF(k->dRefCount);HSS_DECREF(k->dRefCount);
	if(HSS_CHECKREF(k->dRefCount))
	{
		HSS_FREE(k->dCallid);
		HSS_FREE(k->pMethod);
		HSS_FREE(k->pViaBranch);
		if(k->pFromHdr != SIP_NULL)
			sip_freeSipFromHeader(k->pFromHdr);
		if(k->pToHdr != SIP_NULL)
			sip_freeSipToHeader(k->pToHdr);
#ifdef SIP_RPR			
		if(k->pRackHdr != SIP_NULL)
			sip_freeSipRackHeader(k->pRackHdr);
#endif			
		HSS_UNLOCKREF(k->dRefCount);
		HSS_DELETEREF(k->dRefCount);
		HSS_FREE(k);
	}
	else
	{
		HSS_UNLOCKREF(k->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSdpOrigin
** DESCRIPTION: This function frees a SdpOrigin structure 
** 
** PARAMETERS:
**				s(IN/OUT)			- The structure to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSdpOrigin
#ifdef ANSI_PROTO
	(SdpOrigin *s)
#else
	(s)
	SdpOrigin *s;
#endif
{
	if (s == SIP_NULL) 
		return;
	HSS_LOCKREF(s->dRefCount);
	HSS_DECREF(s->dRefCount);
	if (HSS_CHECKREF(s->dRefCount))
	{
		HSS_FREE(s->pUser);
        HSS_FREE(s->pSessionid);
		HSS_FREE(s->pVersion);
		HSS_FREE(s->pNetType);
		HSS_FREE(s->pAddrType);
		HSS_FREE(s->pAddr);
		HSS_UNLOCKREF(s->dRefCount);
		HSS_DELETEREF(s->dRefCount);
		HSS_FREE(s);
	}
	else
	{
		HSS_UNLOCKREF(s->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSdpMedia
** DESCRIPTION: This function frees a SdpMedia structure 
** 
** PARAMETERS:
**				m(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSdpMedia
#ifdef ANSI_PROTO
	(SdpMedia *m)
#else
	(m)
	SdpMedia *m;
#endif
{
	SipError err;

	if (m == SIP_NULL) 
		return;

	HSS_LOCKREF(m->dRefCount);
	HSS_DECREF(m->dRefCount);
	if (HSS_CHECKREF(m->dRefCount))
	{
		HSS_FREE(m->pInformation);
		(void)sip_listDeleteAll(&(m->slConnection), &err);
		(void)sip_listDeleteAll( &(m->slBandwidth), &err);
		HSS_FREE(m->pKey);
		HSS_FREE(m->pFormat);
		HSS_FREE(m->pPortNum);
		HSS_FREE(m->pProtocol);
		(void)sip_listDeleteAll ( &(m->slAttr), &err);
		HSS_FREE (m->pMediaValue);
		(void)sip_listDeleteAll ( &(m->slIncorrectLines), &err);
		HSS_UNLOCKREF(m->dRefCount);
		HSS_DELETEREF(m->dRefCount);
		HSS_FREE(m);
	}
	else
	{
		HSS_UNLOCKREF(m->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSdpAttr
** DESCRIPTION: This function frees a SdpAttr structure 
** 
** PARAMETERS:
**				a(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSdpAttr
#ifdef ANSI_PROTO
	(SdpAttr *a)
#else
	(a)
	SdpAttr *a;
#endif
{
	if (a == SIP_NULL) 
		return;

	HSS_LOCKREF(a->dRefCount);
	HSS_DECREF(a->dRefCount);
	if (HSS_CHECKREF(a->dRefCount))
	{
		HSS_FREE(a->pName);
		HSS_FREE(a->pValue);
		HSS_UNLOCKREF(a->dRefCount);
		HSS_DELETEREF(a->dRefCount);
		HSS_FREE (a);
	}
	else
	{
		HSS_UNLOCKREF(a->dRefCount);
	}


}


/****************************************************************************
** FUNCTION: sip_freeSdpTime
** DESCRIPTION: This function frees a SdpTime structure 
** 
** PARAMETERS:
**				t(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSdpTime
#ifdef ANSI_PROTO
	(SdpTime *t)
#else
	(t)
	SdpTime *t;
#endif
{
	SipError err;

	if (t == SIP_NULL) 
		return;
	HSS_LOCKREF(t->dRefCount);
	HSS_DECREF(t->dRefCount);
	if (HSS_CHECKREF(t->dRefCount))
	{
		HSS_FREE(t->pStart);
		HSS_FREE(t->pStop);
		(void)sip_listDeleteAll ( &(t->slRepeat), &err);
		HSS_FREE(t->pZone);
		HSS_UNLOCKREF(t->dRefCount);
		HSS_DELETEREF(t->dRefCount);
		HSS_FREE (t);
	}
	else
	{
		HSS_UNLOCKREF(t->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSdpConnection
** DESCRIPTION: This function frees a SdpConnection buffer 
** 
** PARAMETERS:
**				c(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSdpConnection
#ifdef ANSI_PROTO
	( SdpConnection *c)
#else
	(c)
 	SdpConnection *c;
#endif
{
	if (c == SIP_NULL) 
		return;
	HSS_LOCKREF(c->dRefCount);
	HSS_DECREF(c->dRefCount);
	if (HSS_CHECKREF(c->dRefCount))
	{
		HSS_FREE(c->pNetType);
		HSS_FREE(c->pAddrType);
		HSS_FREE(c->pAddr);
		HSS_UNLOCKREF(c->dRefCount);
		HSS_DELETEREF(c->dRefCount);
		HSS_FREE(c);
	}
	else
	{
		HSS_UNLOCKREF(c->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSdpMessage
** DESCRIPTION: This function frees a SdpMessage buffer 
** 
** PARAMETERS:
**				m(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSdpMessage
#ifdef ANSI_PROTO
	(SdpMessage *m)
#else
	(m)
	SdpMessage *m;
#endif
{
	SipError err;
	
	if (m == SIP_NULL) 
		return;
	HSS_LOCKREF(m->dRefCount);
	HSS_DECREF(m->dRefCount);
	if (HSS_CHECKREF(m->dRefCount))
	{
		HSS_FREE(m->pVersion);
		sip_freeSdpOrigin(m->pOrigin);
		HSS_FREE(m->pSession);
		HSS_FREE(m->pInformation);
		HSS_FREE(m->pUri);
		(void)sip_listDeleteAll( &(m->slEmail), &err);
		(void)sip_listDeleteAll( &(m->slPhone), &err);
		sip_freeSdpConnection (m->slConnection);
		(void)sip_listDeleteAll( &(m->pBandwidth), &err);
		(void)sip_listDeleteAll( &(m->slTime), &err);
		HSS_FREE(m->pKey);
		(void)sip_listDeleteAll( &(m->slAttr), &err);
		(void)sip_listDeleteAll( &(m->slMedia), &err);
		(void)sip_listDeleteAll( &(m->slIncorrectLines), &err);
		HSS_UNLOCKREF(m->dRefCount);
		HSS_DELETEREF(m->dRefCount);
		HSS_FREE(m);
	}
	else
	{
		HSS_UNLOCKREF(m->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSipParam
** DESCRIPTION: This function frees a SipParam structure 
** 
** PARAMETERS:
**				p(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipParam
#ifdef ANSI_PROTO
	(SipParam *p)
#else
	(p)
	SipParam *p;
#endif
{
	SipError err;
	
	if (p == SIP_NULL) 
		return;
	HSS_LOCKREF(p->dRefCount);
	HSS_DECREF(p->dRefCount);
	if (HSS_CHECKREF(p->dRefCount))
	{
		HSS_FREE(p->pName);
		(void)sip_listDeleteAll( &(p->slValue), &err);
		HSS_UNLOCKREF(p->dRefCount);
		HSS_DELETEREF(p->dRefCount);
		HSS_FREE(p);
	}
	else
	{
		HSS_UNLOCKREF(p->dRefCount);
	}
}

/****************************************************************************
** FUNCTION: sip_freeSipCommonHeader
** DESCRIPTION: This function frees a SipCommonHeader structure 
** 
** PARAMETERS:
**				pHeader(IN)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipCommonHeader
	(SipCommonHeader *pHeader)
{
	SipError err;
	if (pHeader == SIP_NULL) 
		return;
	HSS_LOCKREF(pHeader->dRefCount);
	HSS_DECREF(pHeader->dRefCount);
	if (HSS_CHECKREF(pHeader->dRefCount))
	{
		HSS_FREE(pHeader->pStr1);
		HSS_FREE(pHeader->pStr2);
		sip_freeSipAddrSpec(pHeader->pAddrSpec);
		(void)sip_listDeleteAll(&(pHeader->slParam),&err);
#ifdef SIP_INCREMENTAL_PARSING
		HSS_FREE(pHeader->pBuffer);
#endif
		HSS_UNLOCKREF(pHeader->dRefCount);
		HSS_DELETEREF(pHeader->dRefCount);
		HSS_FREE(pHeader);
	}
	else
	{
		HSS_UNLOCKREF(pHeader->dRefCount);
	}
}

#ifdef SIP_DATE
/****************************************************************************
** FUNCTION: sip_freeSipCommonHeaderWithDate
** DESCRIPTION: This function frees a SipCommonHeader structure 
** 
** PARAMETERS:
**				pHeader(IN)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipCommonHeaderWithDate
	(SipCommonHeader *pHeader)
{
	SipError err;
	if (pHeader == SIP_NULL) 
		return;
	HSS_LOCKREF(pHeader->dRefCount);
	HSS_DECREF(pHeader->dRefCount);
	if (HSS_CHECKREF(pHeader->dRefCount))
	{
		HSS_FREE(pHeader->pStr1);
		HSS_FREE(pHeader->pStr2);
		sip_freeSipAddrSpec(pHeader->pAddrSpec);
		(void)sip_listDeleteAll(&(pHeader->slParam),&err);
		if (pHeader->dIntVar1 == SipExpDate)
			__sip_freeSipDateStruct(pHeader->u.pDate);
#ifdef SIP_INCREMENTAL_PARSING
		HSS_FREE(pHeader->pBuffer);
#endif
		HSS_UNLOCKREF(pHeader->dRefCount);
		HSS_DELETEREF(pHeader->dRefCount);
		HSS_FREE(pHeader);
	}
	else
	{
		HSS_UNLOCKREF(pHeader->dRefCount);
	}
}
#endif

/****************************************************************************
** FUNCTION: sip_freeSipRespHeader
** DESCRIPTION: This function frees a SipRespHeader structure 
** 
** PARAMETERS:
**				h(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipRespHeader
#ifdef ANSI_PROTO
	(SipRespHeader *h)
#else
	(h)
	SipRespHeader *h;
#endif
{
	SipError err;
	
	if (h == SIP_NULL) 
		return;
	HSS_LOCKREF(h->dRefCount);
	HSS_DECREF(h->dRefCount);
	if (HSS_CHECKREF(h->dRefCount))
	{
#ifdef SIP_RPR	
		sip_freeSipRseqHeader((h->pRSeqHdr)) ;
#endif		
		(void)sip_listDeleteAll(&(h->slProxyAuthenticateHdr),&err);
		(void)sip_listDeleteAll(&(h->slUnsupportedHdr), &err);
#ifdef SIP_WARNING		
		(void)sip_listDeleteAll(&(h->slWarningHeader), &err);
#endif	
#ifdef SIP_3GPP
		(void)sip_listDeleteAll(&(h->slPAssociatedUriHdr), &err);
		(void)sip_listDeleteAll(&(h->slServiceRouteHdr), &err);
		sip_freeSipETagHeader((h->pETagHdr)) ;
#endif	
#ifdef SIP_SECURITY
		(void)sip_listDeleteAll(&(h->slSecurityServerHdr), &err);
#endif        
		sip_freeSipMinExpiresHeader((h->pMinExpiresHdr)) ;
		(void)sip_listDeleteAll(&(h->slWwwAuthenticateHdr), &err);
#ifdef SIP_CONGEST
		(void)sip_listDeleteAll(&(h->slAcceptRsrcPriorityHdr), &err);
#endif
		HSS_UNLOCKREF(h->dRefCount);
		HSS_DELETEREF(h->dRefCount);
		HSS_FREE(h);
	}
	else
	{
		HSS_UNLOCKREF(h->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSipUrl
** DESCRIPTION: This function frees a SipUrl buffer 
** 
** PARAMETERS:
**				s(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipUrl
#ifdef ANSI_PROTO
	(SipUrl *u)
#else
	(u)
	SipUrl *u;
#endif
{
	SipError err;
	
	if (u == SIP_NULL) 
		return;
	HSS_LOCKREF(u->dRefCount);
	HSS_DECREF(u->dRefCount);
	if (HSS_CHECKREF(u->dRefCount))
	{ 
		HSS_FREE(u->pUser);
		HSS_FREE(u->pPassword);
		HSS_FREE(u->pHost);
		HSS_FREE(u->dPort);
		(void)sip_listDeleteAll(&(u->slParam), &err);
		HSS_FREE(u->pHeader);
		HSS_UNLOCKREF(u->dRefCount);
		HSS_DELETEREF(u->dRefCount);
		HSS_FREE(u);
	}
	else
	{
		HSS_UNLOCKREF(u->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSipAddrSpec
** DESCRIPTION: This function frees a SipAddrSpec structure 
** 
** PARAMETERS:
**				a(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipAddrSpec
#ifdef ANSI_PROTO
	(SipAddrSpec *a)
#else
	(a)
	SipAddrSpec *a;
#endif
{
	if (a == SIP_NULL) 
		return;
	HSS_LOCKREF(a->dRefCount);
	HSS_DECREF(a->dRefCount);
	if (HSS_CHECKREF(a->dRefCount))
	{ 
		if (a->dType == SipAddrReqUri)
			HSS_FREE(a->u.pUri);
		else	
		if ((a->dType == SipAddrSipUri) || (a->dType == SipAddrSipSUri))
		{
			sip_freeSipUrl(a->u.pSipUrl);
		}
		HSS_UNLOCKREF(a->dRefCount);
		HSS_DELETEREF(a->dRefCount);
		HSS_FREE(a);
	}
	else
	{
		HSS_UNLOCKREF(a->dRefCount);
	}
}

/****************************************************************************
** FUNCTION: sip_freeSipReqHeader
** DESCRIPTION: This function frees a SipReqHeader structure 
** 
** PARAMETERS:
**				s(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipReqHeader
#ifdef ANSI_PROTO
	(SipReqHeader *s)
#else
	(s)
	SipReqHeader *s;
#endif
{
	SipError err;
	
	if (s == SIP_NULL) 
		return;
	HSS_LOCKREF(s->dRefCount);
	HSS_DECREF(s->dRefCount);
	if (HSS_CHECKREF(s->dRefCount))
	{
#ifdef SIP_RPR	
		sip_freeSipRackHeader(s->pRackHdr);
#endif		
		sip_freeSipMaxForwardsHeader(s->pMaxForwardsHdr);
#ifdef SIP_REFER
		sip_freeSipReferToHeader(s->pReferToHdr);
		sip_freeSipReferredByHeader(s->pReferredByHdr);
#endif
#ifdef SIP_IMPP
		sip_freeSipEventHeader(s->pEventHdr);
		sip_impp_freeSipSubscriptionStateHeader(s->pSubscriptionStateHdr);
#endif		
#ifdef SIP_REPLACES
		sip_freeSipReplacesHeader(s->pReplacesHdr);
#endif /* SIP_REPLACES */
#ifdef SIP_3GPP
		sip_freeSipPCalledPartyIdHeader(s->pPCalledPartyIdHdr);
		sip_freeSipJoinHeader(s->pJoinHdr);
		sip_freeSipIfMatchHeader(s->pIfMatchHdr);
		(void)sip_listDeleteAll(&(s->slPVisitedNetworkIdHdr), &err);
		(void)sip_listDeleteAll(&(s->slRequestDispositionHdr), &err);
		(void)sip_listDeleteAll(&(s->slAcceptContactHdr), &err);
		(void)sip_listDeleteAll(&(s->slRejectContactHdr), &err);
#endif	
#ifdef SIP_SECURITY
		(void)sip_listDeleteAll(&(s->slSecurityClientHdr), &err);
		(void)sip_listDeleteAll(&(s->slSecurityVerifyHdr), &err);
#endif        
		(void)sip_listDeleteAll(&(s->slRouteHdr), &err);
		(void)sip_listDeleteAll(&(s->slProxyAuthorizationHdr), &err);
		(void)sip_listDeleteAll(&(s->slAuthorizationHdr), &err);
#ifdef SIP_CONGEST
		(void)sip_listDeleteAll(&(s->slRsrcPriorityHdr), &err);
#endif
		HSS_UNLOCKREF(s->dRefCount);
		HSS_DELETEREF(s->dRefCount);
		HSS_FREE(s);
	}
	else
	{
		HSS_UNLOCKREF(s->dRefCount);
	}
}

/****************************************************************************
** FUNCTION: sip_freeSipGeneralHeader
** DESCRIPTION: This function frees a SipGeneralHeader structure 
** 
** PARAMETERS:
**				g(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipGeneralHeader
#ifdef ANSI_PROTO
	(SipGeneralHeader *g)
#else
	(g)
	SipGeneralHeader *g;
#endif
{
	SipError err;
	
	if (g == SIP_NULL) 
		return;
	HSS_LOCKREF(g->dRefCount);
	HSS_DECREF(g->dRefCount);
	if (HSS_CHECKREF(g->dRefCount))
	{ 
		(void)sip_listDeleteAll(&(g->slContentDispositionHdr), &err);
		(void)sip_listDeleteAll(&(g->slContactHdr), &err);
		(void)sip_listDeleteAll(&(g->slRecordRouteHdr), &err);
		(void)sip_listDeleteAll(&(g->slViaHdr), &err);
		(void)sip_listDeleteAll(&(g->slContentEncodingHdr), &err);
		(void)sip_listDeleteAll(&(g->slUnknownHdr), &err);
		(void)sip_listDeleteAll(&(g->slSupportedHdr), &err);
		(void)sip_listDeleteAll(&(g->slRequireHdr), &err);
		(void)sip_listDeleteAll(&(g->slAllowHdr), &err);
#ifdef SIP_PEMEDIA
		(void)sip_listDeleteAll(&(g->slPEarlyMediaHdr), &err);
#endif
		sip_freeSipRetryAfterHeader(g->pRetryAfterHdr);
		sip_freeSipExpiresHeader(g->pExpiresHdr);
		sip_freeSipCallIdHeader(g->pCallidHdr);
		sip_freeSipCseqHeader(g->pCseqHdr);
		sip_freeSipFromHeader(g->pFromHeader);
		sip_freeSipToHeader(g->pToHdr);
		sip_freeSipContentLengthHeader(g->pContentLengthHdr);
		sip_freeSipContentTypeHeader(g->pContentTypeHdr);
#ifdef SIP_TIMESTAMP
		sip_freeSipTimeStampHeader(g->pTimeStampHdr);
#endif		
		sip_freeSipMimeVersionHeader(g->pMimeVersionHdr);
		(void)sip_listDeleteAll( &(g->slBadHdr), &err);
#ifdef SIP_SESSIONTIMER
        sip_freeSipMinSEHeader(g->pMinSEHdr);
		sip_freeSipSessionExpiresHeader(g->pSessionExpiresHdr);
#endif
#ifdef SIP_PRIVACY
		sip_freeSipPrivacyHeader(g->pPrivacyHdr);
		(void)sip_listDeleteAll( &(g->slPAssertIdHdr), &err);
		(void)sip_listDeleteAll( &(g->slPPreferredIdHdr), &err);
#endif
#ifdef SIP_IMPP
		(void)sip_listDeleteAll( &(g->slAllowEventsHdr), &err);
#endif
		(void)sip_listDeleteAll( &(g->slAcceptHdr), &err);
#ifdef SIP_3GPP
		(void)sip_listDeleteAll(&(g->slPathHdr), &err);
	    sip_freeSipPcfAddrHeader(g->pPcfAddrHdr);		
        sip_freeSipPanInfoHeader(g->pPanInfoHdr);
        sip_freeSipPcVectorHeader(g->pPcVectorHdr);
		(void)sip_listDeleteAll(&(g->slHistoryInfoHdr), &err);
#endif

		
		HSS_UNLOCKREF(g->dRefCount);
		HSS_DELETEREF(g->dRefCount);
		HSS_FREE(g);
	}
	else
	{
		HSS_UNLOCKREF(g->dRefCount);
	}
}

/****************************************************************************
** FUNCTION: sip_freeSipReqMessage
** DESCRIPTION: This function frees a SipReqMessage structure 
** 
** PARAMETERS:
**				r(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipReqMessage
#ifdef ANSI_PROTO
	(SipReqMessage *r)
#else
	(r)
	SipReqMessage *r;
#endif
{
	if (r == SIP_NULL) 
		return;
	HSS_LOCKREF(r->dRefCount);
	HSS_DECREF(r->dRefCount);
	if (HSS_CHECKREF(r->dRefCount))
	{ 
		sip_freeSipReqLine(r->pRequestLine);
		sip_freeSipReqHeader(r->pRequestHdr);
		HSS_UNLOCKREF(r->dRefCount);
		HSS_DELETEREF(r->dRefCount);
		HSS_FREE(r);
	}
	else
	{
		HSS_UNLOCKREF(r->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSipRespMessage
** DESCRIPTION: This function frees a SipRespMessage structure 
** 
** PARAMETERS:
**				r(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipRespMessage
#ifdef ANSI_PROTO
	(SipRespMessage *r)
#else
	(r)
	SipRespMessage *r;
#endif
{
	if (r == SIP_NULL) 
		return;
	HSS_LOCKREF(r->dRefCount);
	HSS_DECREF(r->dRefCount);
	if (HSS_CHECKREF(r->dRefCount))
	{ 
		sip_freeSipStatusLine(r->pStatusLine);
		sip_freeSipRespHeader(r->pResponseHdr);
		HSS_UNLOCKREF(r->dRefCount);
		HSS_DELETEREF(r->dRefCount);
		HSS_FREE(r);
	}
	else
	{
		HSS_UNLOCKREF(r->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSipUnknownMessage
** DESCRIPTION: This function frees a SipUnknownMessage structure 
** 
** PARAMETERS:
**				s(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipUnknownMessage
#ifdef ANSI_PROTO
	(SipUnknownMessage *s)
#else
	(s)
	SipUnknownMessage *s;
#endif
{
	if (s == SIP_NULL) 
		return;
	HSS_LOCKREF(s->dRefCount);
	HSS_DECREF(s->dRefCount);
	if (HSS_CHECKREF(s->dRefCount))
	{ 
		if (s->pBuffer != SIP_NULL)
			HSS_FREE(s->pBuffer);
		HSS_UNLOCKREF(s->dRefCount);
		HSS_DELETEREF(s->dRefCount);
		HSS_FREE(s);
	}
	else
	{
		HSS_UNLOCKREF(s->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSipMsgBody
** DESCRIPTION: This function frees a SipMsgBody structure 
** 
** PARAMETERS:
**				s(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipMsgBody
#ifdef ANSI_PROTO
	(SipMsgBody	*s)
#else
	(s)
	SipMsgBody 	*s;
#endif
{
	if (s == SIP_NULL) 
		return;
	HSS_LOCKREF(s->dRefCount);
	HSS_DECREF(s->dRefCount);
	if (HSS_CHECKREF(s->dRefCount))
	{ 
		switch (s->dType)
		{
			case SipSdpBody:
				sip_freeSdpMessage(s->u.pSdpMessage);
				break;	
			case SipUnknownBody:
				sip_freeSipUnknownMessage(s->u.pUnknownMessage);
				break;	
#ifdef SIP_ISUP				
			case SipIsupBody:
				sip_bcpt_freeIsupMessage(s->u.pIsupMessage);
				break;	
#endif				
#ifdef SIP_MIME_PARSING				
			case SipMultipartMimeBody:
				sip_bcpt_freeMimeMessage(s->u.pMimeMessage);
				break;
#endif				
#ifdef SIP_SIPMSGBODY
			case SipAppSipBody:
				sip_freeSipMessage(s->u.pAppSipMessage);
                break;
#endif				
#ifdef SIP_MWI
            case SipMessageSummaryBody:
				sip_mwi_freeMesgSummaryMessage(s->u.pSummaryMessage);
                break;
#endif
			default :
				break;	
		}
#ifdef SIP_MIME_PARSING				
		sip_bcpt_freeSipMimeHeader(s->pMimeHeader);
#endif		
		HSS_UNLOCKREF(s->dRefCount);
		HSS_DELETEREF(s->dRefCount);
		HSS_FREE(s);
	}
	else
	{
		HSS_UNLOCKREF(s->dRefCount);
	}
}

#ifdef SIP_MWI
/*****************************************************************
** FUNCTION:__sip_freeSummaryLine
**
**
** DESCRIPTION:
*******************************************************************/

void __sip_mwi_freeSummaryLine
#ifdef ANSI_PROTO
        (SIP_Pvoid s)
#else
        (s)
        SIP_Pvoid s;
#endif
{
        sip_mwi_freeSummaryLine((SummaryLine*) s);
}

#endif
/****************************************************************************
** FUNCTION: sip_freeSipMessage
** DESCRIPTION: This function frees a SipMessage structure 
** 
** PARAMETERS:
**				s(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipMessage
#ifdef ANSI_PROTO
	(SipMessage *s)
#else
	(s)
	SipMessage *s;
#endif
{
	SipError err;

	if (s == SIP_NULL) 
		return;
	HSS_LOCKREF(s->dRefCount);
	HSS_DECREF(s->dRefCount);
	if (HSS_CHECKREF(s->dRefCount))
	{ 
		sip_freeSipGeneralHeader(s->pGeneralHdr);
		if (s->dType == SipMessageRequest)
			sip_freeSipReqMessage(s->u.pRequest);
		else
			if (s->dType == SipMessageResponse)
			{
				sip_freeSipRespMessage(s->u.pResponse);
			}
		(void)sip_listDeleteAll(&(s->slOrderInfo), &err);
		(void)sip_listDeleteAll(&(s->slMessageBody), &err);
		HSS_UNLOCKREF(s->dRefCount);
		HSS_DELETEREF(s->dRefCount);
		HSS_FREE(s);
	}
	else
	{
		HSS_UNLOCKREF(s->dRefCount);
	}
}

/****************************************************************************
** FUNCTION: sip_freeSipHeader
** DESCRIPTION: This function frees a SipHeader structure 
** 
** PARAMETERS:
**				h(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipHeader
	(SipHeader *pHdr)
{
	if (pHdr == SIP_NULL) 
		return;

	if((pHdr->dType != SipHdrTypeAny) && \
		(pHdr->pHeader != SIP_NULL))
	{
		sip_freeSipCommonHeader(( SipCommonHeader *)(pHdr->pHeader));
	}
	pHdr->dType=SipHdrTypeAny;

} /* freeSipHeader */


/* These are the functions that will be called INTERNALLY
to associate with sip lists which expect void *.
These are NOT exposed to the pUser
*/

/****************************************************************************
** FUNCTION: __sip_freeString
** DESCRIPTION: This function frees a string buffer 
** 
** PARAMETERS:
**				s(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeString
#ifdef ANSI_PROTO
	(SIP_Pvoid s)
#else
	(s)
	SIP_Pvoid s;
#endif
{
	sip_freeString((SIP_S8bit *)s);
 
}


/****************************************************************************
** FUNCTION: __sip_freeSipHeaderOrderInfo
** DESCRIPTION: This function frees a SipHeaderOrderInfo buffer 
** 
** PARAMETERS:
**				s(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSipHeaderOrderInfo 
#ifdef ANSI_PROTO
	(SIP_Pvoid s)
#else
	(s)
	SIP_Pvoid s;
#endif
{
	SipHeaderOrderInfo *pOrderInfo=SIP_NULL;

	if (s==SIP_NULL) return;
	pOrderInfo=(SipHeaderOrderInfo *)s;

    HSS_LOCKREF(pOrderInfo->dRefCount);HSS_DECREF(pOrderInfo->dRefCount);
	if(HSS_CHECKREF(pOrderInfo->dRefCount))
	{
		HSS_UNLOCKREF(pOrderInfo->dRefCount);
		HSS_DELETEREF(pOrderInfo->dRefCount);
		HSS_FREE(pOrderInfo);
	}
	else
	{
		HSS_UNLOCKREF(pOrderInfo->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: __sip_freeSipMessage
** DESCRIPTION: This function frees a SipMessage structure 
** 
** PARAMETERS:
**				h(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSipMessage
#ifdef ANSI_PROTO
	(SIP_Pvoid s)
#else
	(s)
	SIP_Pvoid s;
#endif
{
	sip_freeSipMessage((SipMessage*) s);
}


/****************************************************************************
** FUNCTION: __sip_freeSdpMedia
** DESCRIPTION: This function frees an SdpMedia structure 
** 
** PARAMETERS:
**				m(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSdpMedia
#ifdef ANSI_PROTO
	(SIP_Pvoid m)
#else
	(m)
	SIP_Pvoid m;
#endif
{
	sip_freeSdpMedia((SdpMedia*) m);
}


/****************************************************************************
** FUNCTION: __sip_freeSdpAttr
** DESCRIPTION: This function frees an SdpAttr buffer 
** 
** PARAMETERS:
**				a(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSdpAttr
#ifdef ANSI_PROTO
	(SIP_Pvoid a)
#else
	(a)
	SIP_Pvoid a;
#endif
{
	sip_freeSdpAttr((SdpAttr*) a);
}


/****************************************************************************
** FUNCTION: __sip_freeSdpTime
** DESCRIPTION: This function frees an SdpTime buffer 
** 
** PARAMETERS:
**				t(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSdpTime 
#ifdef ANSI_PROTO
	(SIP_Pvoid t)
#else
	(t)
	SIP_Pvoid t;
#endif
{
	sip_freeSdpTime ((SdpTime *) t);
}


/****************************************************************************
** FUNCTION: __sip_freeSdpConnection
** DESCRIPTION: This function frees an SdpConnection buffer 
** 
** PARAMETERS:
**				c(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSdpConnection
#ifdef ANSI_PROTO
	(SIP_Pvoid c)
#else
	(c)
	SIP_Pvoid c;
#endif
{
	sip_freeSdpConnection((SdpConnection*) c);
}


/****************************************************************************
** FUNCTION: sip_freeSipParam
** DESCRIPTION: This function frees a SipParam structure 
** 
** PARAMETERS:
**				p(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSipParam
#ifdef ANSI_PROTO
	(SIP_Pvoid p)
#else
	(p)
	SIP_Pvoid p;
#endif
{
	sip_freeSipParam((SipParam *) p);
}


/*SS: Why __function is present. Verify and remove */
/****************************************************************************
** FUNCTION: __sip_freeSipCommonHeader
** DESCRIPTION: This function frees a SipCommonHeader structure 
** 
** PARAMETERS:
**				w(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/
void __sip_freeSipCommonHeader
	(SIP_Pvoid pHeader)
{
	sip_freeSipCommonHeader((SipCommonHeader*) pHeader);
}

#ifdef SIP_DATE
/****************************************************************************
** FUNCTION: __sip_freeSipCommonHeaderWithDate
** DESCRIPTION: This function frees a SipCommonHeader structure 
** 
** PARAMETERS:
**				w(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/
void __sip_freeSipCommonHeaderWithDate
	(SIP_Pvoid pHeader)
{
	sip_freeSipCommonHeaderWithDate((SipCommonHeader*) pHeader);
}
#endif

/****************************************************************************
** FUNCTION: sip_freeSipMsgBody
** DESCRIPTION: This function frees a SipMsgBody structure 
** 
** PARAMETERS:
**				h(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSipMsgBody
#ifdef ANSI_PROTO
	(SIP_Pvoid h)
#else
	(h)
	SIP_Pvoid h;
#endif
{
	sip_freeSipMsgBody((SipMsgBody*) h);
}


/****************************************************************************
** FUNCTION: sip_releaseStack
** DESCRIPTION: This function performs cleanup functions for the stack 
** 
** PARAMETERS: None
**
****************************************************************************/

void sip_releaseStack()
{
	sip_releaseStackPortSpecific();
		
#ifdef SIP_THREAD_SAFE
		fast_free_synch(&glbLockStatisticsMutex);
#endif
}

#ifdef SIP_DATE
/****************************************************************************
** FUNCTION: __sip_freeSipDateStruct
** DESCRIPTION: This function frees a SipDateStruct structure
** 
** PARAMETERS:
**				d(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSipDateStruct
#ifdef ANSI_PROTO
	(SIP_Pvoid d)
#else
	(d)
	SIP_Pvoid d;
#endif
{
	sip_freeSipDateStruct((SipDateStruct*) d);
}


/****************************************************************************
** FUNCTION: sip_freeSipDateFormat
** DESCRIPTION: This function frees a SipDateFormat structure
** 
** PARAMETERS:
**				d(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipDateFormat
#ifdef ANSI_PROTO
	(SipDateFormat *d)
#else
	(d)
	SipDateFormat *d;
#endif
{
	if (d == SIP_NULL) 
		return;
	HSS_LOCKREF(d->dRefCount);
	HSS_DECREF(d->dRefCount);
	if (HSS_CHECKREF(d->dRefCount))
	{
		HSS_UNLOCKREF(d->dRefCount);
		HSS_DELETEREF(d->dRefCount);
		HSS_FREE(d);
	}
	else
	{
		HSS_UNLOCKREF(d->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSipDateStruct
** DESCRIPTION: This function frees a SipDateStruct structure 
** 
** PARAMETERS:
**				d(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipDateStruct
#ifdef ANSI_PROTO
	(SipDateStruct *d)
#else
	(d)
	SipDateStruct *d;
#endif
{
	if (d == SIP_NULL) 
		return;
	HSS_LOCKREF(d->dRefCount);
	HSS_DECREF(d->dRefCount);
	if (HSS_CHECKREF(d->dRefCount))
	{
		__sip_freeSipDateFormat(d->pDate);
		__sip_freeSipTimeFormat(d->pTime);
		HSS_UNLOCKREF(d->dRefCount);
		HSS_DELETEREF(d->dRefCount);
		HSS_FREE(d);
	}
	else
	{
		HSS_UNLOCKREF(d->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: __sip_freeSipDateFormat
** DESCRIPTION: This function frees a SipDateFormat structure
** 
** PARAMETERS:
**				d(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSipDateFormat
#ifdef ANSI_PROTO
	(SIP_Pvoid d)
#else
	(d)
	SIP_Pvoid d;
#endif
{
	sip_freeSipDateFormat((SipDateFormat*) d);
}


/****************************************************************************
** FUNCTION: __sip_freeSipTimeFormat
** DESCRIPTION: This function frees a SipTimeFormat structure 
** 
** PARAMETERS:
**				t(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void __sip_freeSipTimeFormat
#ifdef ANSI_PROTO
	(SIP_Pvoid t)
#else
	(t)
	SIP_Pvoid t;
#endif
{
	sip_freeSipTimeFormat((SipTimeFormat*) t);
}


/****************************************************************************
** FUNCTION: sip_freeSipTimeFormat
** DESCRIPTION: This function frees a SipTimeFormat structure 
** 
** PARAMETERS:
**				t(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipTimeFormat
#ifdef ANSI_PROTO
	(SipTimeFormat *t)
#else
	(t)
	SipTimeFormat *t;
#endif
{
	if (t == SIP_NULL) 
		return;
	HSS_LOCKREF(t->dRefCount);
	HSS_DECREF(t->dRefCount);
	if (HSS_CHECKREF(t->dRefCount))
	{
		HSS_UNLOCKREF(t->dRefCount);
		HSS_DELETEREF(t->dRefCount);
		HSS_FREE(t);
	}
	else
	{
		HSS_UNLOCKREF(t->dRefCount);
	}
}
#endif /* SIP_DATE */



#ifdef SIP_CORE_STACK_COMPATIBLE
/*****************************************************************
** FUNCTION:sip_freeSipContactParam
** DESCRIPTION: This function frees a SipContactParam structure 
** 
** PARAMETERS:
**				c(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipContactParam
#ifdef ANSI_PROTO
	(SipContactParam *c)
#else
	(c)
	SipContactParam *c;
#endif
{
	if (c==SIP_NULL) 
		return;
	HSS_LOCKREF(c->dRefCount);
	HSS_DECREF(c->dRefCount);
	if (HSS_CHECKREF(c->dRefCount))
	{
		switch(c->dType)
		{
			case SipCParamQvalue:
				HSS_FREE(c->u.pQValue);
				break;
			case SipCParamExpires:
				sip_freeSipExpiresStruct(c->u.pExpire);
				break;
			case SipCParamExtension:
				HSS_FREE(c->u.pExtensionAttr);
				break;
			case SipCParamAction:
				HSS_FREE(c->u.pAction);
				break;
#ifdef SIP_3GPP
 			case SipCParamFeature:
				HSS_FREE(c->u.pFeatureAttr);
				break;
#endif

			case SipCParamAny:
				break;
		} /*switch*/
		HSS_UNLOCKREF(c->dRefCount);
		HSS_DELETEREF(c->dRefCount);
		HSS_FREE(c);
	}
	else
	{
		HSS_UNLOCKREF(c->dRefCount);
	}
}


#ifdef SIP_AUTHENTICATE
/****************************************************************************
** FUNCTION: sip_freeSipGenericChallenge
** DESCRIPTION: This function frees a SipGenericChallenge structure 
** 
** PARAMETERS:
**				c(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipGenericChallenge
#ifdef ANSI_PROTO
	(SipGenericChallenge *c)
#else
	(c)
	SipGenericChallenge *c;
#endif
{
	SipError err;
	
	if (c == SIP_NULL) 
		return;

	HSS_LOCKREF(c->dRefCount);
	HSS_DECREF(c->dRefCount);
	if (HSS_CHECKREF(c->dRefCount))
	{
		HSS_FREE(c->pScheme);
		(void)sip_listDeleteAll(&(c->slParam), &err);
		HSS_UNLOCKREF(c->dRefCount);
		HSS_DELETEREF(c->dRefCount);
		HSS_FREE(c);
	}
	else
	{
		HSS_UNLOCKREF(c->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: sip_freeSipGenericCredential
** DESCRIPTION: This function frees a SipGenericCredential structure
** 
** PARAMETERS:
**				c(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipGenericCredential
#ifdef ANSI_PROTO
	(SipGenericCredential *c)
#else
	(c)
	SipGenericCredential *c;
#endif
{
	if (c == SIP_NULL) 
		return;
	HSS_LOCKREF(c->dRefCount);
	HSS_DECREF(c->dRefCount);
	if (HSS_CHECKREF(c->dRefCount))
	{
		if (c->dType == SipCredAuth)
		{
			__sip_freeSipGenericChallenge(c->u.pChallenge);
		}
		else
		if (c->dType == SipCredBasic)
		{
			HSS_FREE(c->u.pBasic);
		}
		HSS_UNLOCKREF(c->dRefCount);
		HSS_DELETEREF(c->dRefCount);
		HSS_FREE(c);
	}
	else
	{
		HSS_UNLOCKREF(c->dRefCount);
	}
}


/****************************************************************************
** FUNCTION: __sip_freeSipGenericChallenge
** DESCRIPTION: This function frees a SipGenericChallenge structure 
** 
** PARAMETERS:
**				c(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/
void __sip_freeSipGenericChallenge
#ifdef ANSI_PROTO
	(SIP_Pvoid c)
#else
	(c)
	SIP_Pvoid c;
#endif
{
	sip_freeSipGenericChallenge ((SipGenericChallenge *) c);
}
#endif
#endif /* SIP_CORE_STACK_COMPATIBLE */


/****************************************************************************
** FUNCTION: __sip_freeSipBadHeader
** DESCRIPTION: This internal function calls a function to free a 
**				SipBadHeader structure.
** 
** PARAMETERS:
**				u(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/
#ifdef  SIP_BADMESSAGE_PARSING
void __sip_freeSipBadHeader
#ifdef ANSI_PROTO
	(SIP_Pvoid u)
#else
	(u)
	SIP_Pvoid u;
#endif
{
	sip_freeSipBadHeader((SipBadHeader *) u);
}


/****************************************************************************
** FUNCTION: sip_freeSipBadHeader
** DESCRIPTION: This function frees a SipBadHeader structure 
** 
** PARAMETERS:
**				u(OUT)			- The buffer to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipBadHeader
#ifdef ANSI_PROTO
	(SipBadHeader *u)
#else
	(u)
	SipBadHeader *u;
#endif
{
	HSS_LOCKREF(u->dRefCount);
	HSS_DECREF(u->dRefCount);
	if (HSS_CHECKREF(u->dRefCount))
	{
		HSS_FREE(u->pName);
		HSS_FREE(u->pBody);
		HSS_UNLOCKREF(u->dRefCount);
		HSS_DELETEREF(u->dRefCount);
		HSS_FREE(u);
	}
	else
	{
		HSS_UNLOCKREF(u->dRefCount);
	}
}

#endif

#ifdef SIP_CORE_STACK_COMPATIBLE
/****************************************************************************
** FUNCTION: sip_freeSipNameValuePair
** DESCRIPTION: This function frees a SipNameValuePair structure.
** 
** PARAMETERS:
**				n(OUT)			- The structure to be freed 
**
** NOTE: 
****************************************************************************/

void sip_freeSipNameValuePair
#ifdef ANSI_PROTO
      (SipNameValuePair *n)
#else
      (n)
      SipNameValuePair *n;
#endif
{
        if (n==SIP_NULL) 
			return;
        HSS_LOCKREF(n->dRefCount);HSS_DECREF(n->dRefCount);
        if (HSS_CHECKREF(n->dRefCount))
        {
			HSS_FREE(n->pName);
			HSS_FREE(n->pValue);
			HSS_UNLOCKREF(n->dRefCount);
			HSS_DELETEREF(n->dRefCount);
			HSS_FREE(n);
        }
        else
        {
            HSS_UNLOCKREF(n->dRefCount);
        }
}


#endif /* SIP_CORE_STACK_COMPATIBLE */
#ifdef SIP_MWI
/*****************************************************************
** FUNCTION:sip_mwi_freeSummaryLine
**
**
** DESCRIPTION:
*******************************************************************/
void sip_mwi_freeSummaryLine
#ifdef ANSI_PROTO
        (SummaryLine *s)
#else
        (s)
        SummaryLine *s;
#endif
{
        if (s==SIP_NULL) return;
        HSS_LOCKREF(s->dRefCount);HSS_DECREF(s->dRefCount);
        if(HSS_CHECKREF(s->dRefCount))
        {
                HSS_FREE(s->pMedia);
                HSS_UNLOCKREF(s->dRefCount);
                HSS_DELETEREF(s->dRefCount);
                HSS_FREE(s);
        }
        else
        {
                HSS_UNLOCKREF(s->dRefCount);
        }
}

/*****************************************************************
** FUNCTION:sip_mwi_freeMesgSummaryMessage
**
**
** DESCRIPTION:
*******************************************************************/

void sip_mwi_freeMesgSummaryMessage
#ifdef ANSI_PROTO
	(MesgSummaryMessage *m)
#else
	(m)
	MesgSummaryMessage *m;
#endif
{
 	SipError err;
        if (m==SIP_NULL) return;
        HSS_LOCKREF(m->dRefCount);HSS_DECREF(m->dRefCount);
        if(HSS_CHECKREF(m->dRefCount))
        {
                (void)sip_listDeleteAll (&(m->slSummaryLine), &err);
                (void)sip_listDeleteAll (&(m->slNameValue), &err);
                sip_freeSipAddrSpec(m->pAddrSpec) ;
                HSS_UNLOCKREF(m->dRefCount);
                HSS_DELETEREF(m->dRefCount);
                HSS_FREE(m);
        }
        else
        {
                HSS_UNLOCKREF(m->dRefCount);
        }
}
#endif

/*******************************************************************************
** FUNCTION: sip_freeSipTimerBuffer
** DESCRIPTION: This function frees teh timer buffer structure
** of the timeout Context
** 
** PARAMETERS:
**      	      	b(IN)	:The SipMessage
** Return Value:
**				  void	
*******************************************************************************/

void sip_freeSipTimerBuffer
#ifdef ANSI_PROTO
	(SipTimerBuffer *b)
#else
	(b)
	SipTimerBuffer *b;
#endif
{
	if (b==SIP_NULL) 
		return;
	HSS_LOCKREF(b->dRefCount);
	HSS_DECREF(b->dRefCount);
	if(HSS_CHECKREF(b->dRefCount))
	{
		if(b->pContext != SIP_NULL)
		{
			if( (b->pContext->dOptions.dOption & SIP_OPT_DIRECTBUFFER) != \
					SIP_OPT_DIRECTBUFFER)
			{
				if(b->pBuffer != SIP_NULL)
					HSS_FREE(b->pBuffer);
			}
		}			
		else
		{
			if(b->pBuffer != SIP_NULL)
				HSS_FREE(b->pBuffer);
		}	
		if(b->pContext != SIP_NULL)
			sip_freeEventContext(b->pContext);

		HSS_UNLOCKREF(b->dRefCount);
		HSS_DELETEREF(b->dRefCount);
		HSS_FREE(b);
	}
	else
	{
		HSS_UNLOCKREF(b->dRefCount);
	}
}

#ifdef SIP_3GPP

void sip_freeMsrpUrl
#ifdef ANSI_PROTO
	(MsrpUrl *pMsrpUrl)
#else
	(pMsrpUrl)
	MsrpUrl *pMsrpUrl;
#endif
{
	
	if (pMsrpUrl == SIP_NULL) 
		return;
	
	/* It will Lock the reference count variable and decrement its value by 1 */
	
	HSS_LOCKREF(pMsrpUrl->dRefCount);
	HSS_DECREF(pMsrpUrl->dRefCount);
	
	/* If reference count value is equal to 0 we have to delete the structure */
             
	if (HSS_CHECKREF(pMsrpUrl->dRefCount))
	{ 
		HSS_FREE(pMsrpUrl->pUser);
		HSS_FREE(pMsrpUrl->pHost);
		HSS_FREE(pMsrpUrl->dPort);
		HSS_FREE(pMsrpUrl->pResource);
		HSS_UNLOCKREF(pMsrpUrl->dRefCount);
		HSS_DELETEREF(pMsrpUrl->dRefCount);
		HSS_FREE(pMsrpUrl);
	}
	else
	{
		HSS_UNLOCKREF(pMsrpUrl->dRefCount);
	}
}

#endif

/*CSR 1-6792719 CSR Change Start*/
void sip_freeSipTranspAddr
#ifdef ANSI_PROTO
	(SipTranspAddr *t)
#else
	(t)
	SipTranspAddr *t;
#endif
{
	if (t==SIP_NULL) 
		return;
	HSS_LOCKREF(t->dRefCount);
	HSS_DECREF(t->dRefCount);
	if(HSS_CHECKREF(t->dRefCount))
	{
				if(t->pHost != SIP_NULL)
					HSS_FREE(t->pHost);
		    if(t->pReceiversIp != SIP_NULL)
        HSS_FREE(t->pReceiversIp);

		HSS_UNLOCKREF(t->dRefCount);
		HSS_DELETEREF(t->dRefCount);
		HSS_FREE(t);
	}
	else
	{
		HSS_UNLOCKREF(t->dRefCount);
	}
}
/*CSR 1-6792719 CSR Change End*/


