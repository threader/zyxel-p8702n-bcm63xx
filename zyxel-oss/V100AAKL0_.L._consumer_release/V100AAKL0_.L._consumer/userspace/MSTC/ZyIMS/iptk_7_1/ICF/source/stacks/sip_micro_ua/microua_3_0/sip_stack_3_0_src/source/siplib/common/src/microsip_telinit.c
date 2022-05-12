/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 


/***********************************************************************
 ** FUNCTION:
 **             Has Init Functions For all Structures

 *********************************************************************
 **
 ** FILENAME:
 ** telinit.h
 **
 ** DESCRIPTION:
 ** This file contains code to init all  tel-url structures
 **
 ** DATE        NAME                    REFERENCE              REASON
 ** ----        ----                    ---------              --------
 ** 15-08-2002   Mahesh 								   Initial Creation
 **
 **
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/


#include "microsip_common.h"
#include "microsip_telstruct.h"
#include "microsip_telinit.h"

/*********************************************************************************** FUNCTION:	sip_initTelUrl
****
****
**** DESCRIPTION:
*******************************************************************************/
SipBool sip_initTelUrl
#ifdef ANSI_PROTO
	(TelUrl **ppTel,SipError *pErr)
#else
	(ppTel,pErr)
	TelUrl **ppTel;
	SipError *pErr;
#endif
{
	*ppTel = (TelUrl *) fast_memget(0, sizeof(TelUrl),pErr);
	if (*ppTel==SIP_NULL)
		return SipFail;
	INIT((*ppTel)->pGlobal);
	INIT((*ppTel)->pLocal);
	HSS_INITREF((*ppTel)->dRefCount);
	return SipSuccess;
}

/*********************************************************************************** FUNCTION:	sip_initTelLocalNum
****
****
**** DESCRIPTION:
*******************************************************************************/
SipBool sip_initTelLocalNum 
#ifdef ANSI_PROTO
	(TelLocalNum **ppLocal,SipError *pErr)
#else
	(ppLocal, pErr)
	TelLocalNum **ppLocal;
	SipError *pErr;
#endif
{
	*ppLocal = (TelLocalNum *)fast_memget(0, sizeof(TelLocalNum),0);
	if (*ppLocal == SIP_NULL)
		return SipFail;
	INIT((*ppLocal)->pLocalPhoneDigit);
	INIT((*ppLocal)->pIsdnSubAddr);
	INIT((*ppLocal)->pPostDial);
	(void)sip_listInit(& ((*ppLocal)->slAreaSpecifier),__sip_freeString,pErr);
	(void)sip_listInit(& ((*ppLocal)->slParams),__sip_freeSipParam,pErr);
	HSS_INITREF((*ppLocal)->dRefCount);
	return SipSuccess; 
}

/*********************************************************************************** FUNCTION:	sip_initTelGlobalNum
****
****
**** DESCRIPTION:
*******************************************************************************/
SipBool sip_initTelGlobalNum 
#ifdef ANSI_PROTO
	(TelGlobalNum **ppGlobal,SipError *pErr)
#else
	(ppGlobal, pErr)
	TelGlobalNum **ppGlobal;
	SipError *pErr;
#endif
{
	*ppGlobal = (TelGlobalNum *)fast_memget(0, sizeof(TelGlobalNum),0);
	if (*ppGlobal == SIP_NULL)
		return SipFail;
	INIT((*ppGlobal)->pBaseNo);
	INIT((*ppGlobal)->pIsdnSubAddr);
	INIT((*ppGlobal)->pPostDial);
	(void)sip_listInit(& ((*ppGlobal)->slAreaSpecifier),__sip_freeString, pErr);
	(void)sip_listInit(& ((*ppGlobal)->slParams),__sip_freeSipParam,pErr);
	HSS_INITREF((*ppGlobal)->dRefCount);
	return SipSuccess; 

}
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
