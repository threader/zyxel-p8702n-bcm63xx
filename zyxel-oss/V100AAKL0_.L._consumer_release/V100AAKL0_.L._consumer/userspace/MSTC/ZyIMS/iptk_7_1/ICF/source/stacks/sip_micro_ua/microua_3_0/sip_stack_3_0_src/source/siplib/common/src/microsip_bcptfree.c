/***********************************************************************
** FUNCTION:
**			Has Free functions for Isup and Mime structures
************************************************************************
**
** FILENAME:
** microsip_bcptfree.c
**
** DESCRIPTION:
**		This file contains functions for freeing Isup and Mime related 
**		structures.	
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 15/02/2002	Aparna Kuppachi		-			Initial					
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/


#include <stdlib.h>
#include "microsip_struct.h"
#include "microsip_free.h"
#include "microsip_bcptfree.h"

#define HSS_FREE(x) \
do \
{ \
	if ((x!=SIP_NULL)) (void)fast_memfree(0,x,SIP_NULL);\
} \
while(0)
/****************************************************************************
** FUNCTION: sip_bcpt_freeSipMimeVersionHeader
** DESCRIPTION: This function frees a SipMimeVersionHeader structure. 
** 
** PARAMETERS:
**				h(IN/OUT)		- The structure to be freed
**
** NOTE: 
****************************************************************************/
#if 0
void sip_bcpt_freeSipMimeVersionHeader
#ifdef ANSI_PROTO
	(SipMimeVersionHeader * h)
#else
	(h)
	SipMimeVersionHeader *h;
#endif
{
	if (h==SIP_NULL) return;
	HSS_LOCKREF(h->dRefCount);HSS_DECREF(h->dRefCount); 
	if(HSS_CHECKREF(h->dRefCount))
	{
		HSS_FREE(h->pString);
		HSS_UNLOCKREF(h->dRefCount);
		HSS_DELETEREF(h->dRefCount);
		HSS_FREE(h);
	}
	else
	{
		HSS_UNLOCKREF(h->dRefCount);
	}
}

#endif

#ifdef SIP_ISUP
/****************************************************************************
** FUNCTION: sip_bcpt_freeIsupMessage
** DESCRIPTION: This function frees an IsupMessage structure. 
** 
** PARAMETERS:
**				m(IN/OUT)		- The structure to be freed
**
** NOTE: 
****************************************************************************/

void sip_bcpt_freeIsupMessage
#ifdef ANSI_PROTO
	(IsupMessage * m)
#else
	(m)
	IsupMessage *m;
#endif
{
	if (m==SIP_NULL) return;

	HSS_LOCKREF(m->dRefCount);HSS_DECREF(m->dRefCount);
	if(HSS_CHECKREF(m->dRefCount))
	{
		HSS_FREE(m->pBody);
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

/****************************************************************************
** FUNCTION: sip_bcpt_freeMimeMessage
** DESCRIPTION: This function frees a MimeMessage structure. 
** 
** PARAMETERS:
**				m(IN/OUT)		- The structure to be freed
**
** NOTE: 
****************************************************************************/
#ifdef SIP_MIME_PARSING
void sip_bcpt_freeMimeMessage
#ifdef ANSI_PROTO
	(MimeMessage * m)
#else
	(m)
	MimeMessage *m;
#endif
{
	SipError temp_err;

	if (m==SIP_NULL) return;
	HSS_LOCKREF(m->dRefCount);HSS_DECREF(m->dRefCount);
	if(HSS_CHECKREF(m->dRefCount))
	{
		(void)sip_listDeleteAll(&(m->slRecmimeBody), &temp_err);
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
** FUNCTION: sip_bcpt_freeSipMimeHeader
** DESCRIPTION: This function frees a SipMimeHeader structure. 
** 
** PARAMETERS:
**				h(IN/OUT)		- The structure to be freed
**
** NOTE: 
****************************************************************************/

void sip_bcpt_freeSipMimeHeader
#ifdef ANSI_PROTO
	(SipMimeHeader * h)
#else
	(h)
	SipMimeHeader *h;
#endif
{
	SipError temp_err;

	if (h==SIP_NULL) return;
	HSS_LOCKREF(h->dRefCount);HSS_DECREF(h->dRefCount);
	if(HSS_CHECKREF(h->dRefCount))
	{
		(void)sip_listDeleteAll(&(h->slAdditionalMimeHeaders), &temp_err);
		HSS_FREE( h->pContentTransEncoding);
		HSS_FREE( h->pContentId );
		HSS_FREE( h->pContentDescription );
		sip_freeSipContentTypeHeader( h->pContentType);
		sip_freeSipContentDispositionHeader( h->pContentDisposition);
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
** FUNCTION: __sip_bcpt_freeMimeMessage
** DESCRIPTION: Invokes sip_bcpt_freeMimeMessage 
** 
** PARAMETERS:
**				m(IN/OUT)		- The structure to be freed
**
** NOTE: 
****************************************************************************/

void __sip_bcpt_freeMimeMessage
#ifdef ANSI_PROTO
	(SIP_Pvoid  m)
#else
	(m)
	SIP_Pvoid m;
#endif
{
	sip_bcpt_freeMimeMessage((MimeMessage*)m);
}
#endif
