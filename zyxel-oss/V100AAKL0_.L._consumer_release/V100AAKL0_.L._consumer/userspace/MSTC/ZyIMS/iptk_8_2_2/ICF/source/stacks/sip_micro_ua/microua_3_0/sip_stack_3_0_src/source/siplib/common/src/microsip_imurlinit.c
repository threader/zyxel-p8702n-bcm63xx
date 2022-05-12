
 /******************************************************************************
 ** FUNCTION:
 **	 This file has all the source for init functions for Instant Messaging 
 **	 and Presence Related Headers
 **
 ******************************************************************************
 **
 ** FILENAME:
 ** 		imppinit.c
 **
 ** DESCRIPTION:
 **	 
 **
 ** DATE		NAME				REFERENCE	REASON
 ** ----		----				--------	------
 ** 16/08/2002	Mahesh							Original
 **
 **
 **	COPYRIGHT , Aricent, 2006
 ******************************************************************************/
#include "microsip_imurlinit.h"
#include "microsip_init.h"
/******************************************************************************
**** FUNCTION:	sip_initImUrl
****
****
**** DESCRIPTION:
*******************************************************************************/
SipBool sip_initImUrl(ImUrl **ppIm,SipError *pErr)
{
	*ppIm = (ImUrl *) fast_memget(0, sizeof(ImUrl),pErr);
	if (*ppIm==SIP_NULL)
		return SipFail;
	INIT((*ppIm)->pDispName);
	INIT((*ppIm)->pUser);
	INIT((*ppIm)->pHost);
	(void)sip_listInit(& ((*ppIm)->slRoute),__sip_freeString, pErr);
	(void)sip_listInit(& ((*ppIm)->slParams),__sip_freeSipParam,pErr);
	HSS_INITREF((*ppIm)->dRefCount);
	return SipSuccess;
}

