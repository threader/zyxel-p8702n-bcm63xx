/***********************************************************************
** FUNCTION:
**			Has Init functions for Isup and Mime structures
************************************************************************
**
** FILENAME:
** microsip_bcptinit.c
**
** DESCRIPTION:
**		This file contains functions for initing Isup and Mime related 
**		structures.	
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 15/02/2002	Aparna Kuppachi		-			Initial					
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/


#include "microsip_free.h"
#include "microsip_init.h"
#include "microsip_bcptfree.h"
#include "microsip_bcptinit.h"

#define INIT(x) \
do \
{ \
	x=SIP_NULL;\
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


/****************************************************************************
** FUNCTION: sip_initIsupMessage
** DESCRIPTION: This function allocates and inits an IsupMessage structure. 
** 
** PARAMETERS:
**				m(OUT)			- The allocated structure 
**				err(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#ifdef SIP_ISUP
SipBool sip_initIsupMessage
#ifdef ANSI_PROTO
	(IsupMessage **m,SipError *err)
#else
	(m,err)
	IsupMessage **m;
	SipError *err;	
#endif
{
	*m = (IsupMessage *) fast_memget(INIT_MEM_ID, sizeof(IsupMessage), err);
	if (*m == SIP_NULL)
		return SipFail;
	INIT((*m)->pBody);
	(*m)->dLength = 0;
	HSS_INITREF((*m)->dRefCount);
	return SipSuccess;
}
#endif

#ifdef SIP_MIME_PARSING
/****************************************************************************
** FUNCTION: sip_initMimeMessage
** DESCRIPTION: This function allocates and inits a MimeMessage structure. 
** 
** PARAMETERS:
**				m(OUT)			- The allocated structure 
**				err(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool sip_initMimeMessage
#ifdef ANSI_PROTO
	(MimeMessage **m,SipError *err)
#else
	(m,err)
	MimeMessage **m;
	SipError *err;	
#endif
{
	*m=(MimeMessage *) fast_memget(INIT_MEM_ID, sizeof(MimeMessage), err);
	if (*m == SIP_NULL)
		return SipFail;
	if (sip_listInit(&((*m)->slRecmimeBody), __sip_freeSipMsgBody,err) == \
		SipFail)
		return SipFail;
	HSS_INITREF((*m)->dRefCount);
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_initSipMimeHeader
** DESCRIPTION: This function allocates and inits a SipMimeHeader structure. 
** 
** PARAMETERS:
**				h(OUT)			- The allocated structure 
**				err(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_initSipMimeHeader
#ifdef ANSI_PROTO
	(SipMimeHeader **h,SipError *err)
#else
	(h,err)
	SipMimeHeader **h;
	SipError *err;	
#endif
{
	*h=(SipMimeHeader *) fast_memget(INIT_MEM_ID, sizeof(SipMimeHeader), err);
	if (*h == SIP_NULL)
		return SipFail;
	INIT((*h)->pContentType);
	INIT((*h)->pContentTransEncoding);
	INIT((*h)->pContentId);
	INIT((*h)->pContentDescription);
	INIT((*h)->pContentDisposition);
	if (sip_listInit(&((*h)->slAdditionalMimeHeaders), __sip_freeString,err) ==\
		SipFail)
		return SipFail;
	HSS_INITREF((*h)->dRefCount);
	return SipSuccess;
}
#endif
