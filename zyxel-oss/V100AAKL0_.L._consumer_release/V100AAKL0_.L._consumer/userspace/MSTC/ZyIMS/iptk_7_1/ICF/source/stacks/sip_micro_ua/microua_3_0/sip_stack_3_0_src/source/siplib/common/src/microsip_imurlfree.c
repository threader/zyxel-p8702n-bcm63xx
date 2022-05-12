
 /******************************************************************************
 ** FUNCTION:
 **	 This file has the source for the free functions for the
 **	 Instant Messaging and Presence related headers
 **
 ******************************************************************************
 **
 ** FILENAME:
 ** 		imppfree.c
 **
 ** DESCRIPTION:
 **	 
 **
 ** DATE		NAME				REFERENCE	REASON
 ** ----		----				--------	------
 ** 16/4/2001	Subhash Nayak U.	Original
 **
 **
 **	COPYRIGHT , Aricent, 2006
 ******************************************************************************/
#include "microsip_imurlfree.h"
#include "microsip_free.h"
/*****************************************************************
** FUNCTION:sip_freeImUrl
**
**
** DESCRIPTION:
*******************************************************************/
void sip_freeImUrl 
#ifdef ANSI_PROTO
	(ImUrl *pUrl)
#else
 	(pUrl)
	ImUrl *pUrl;
#endif
{
	SipError dErr;

	if (pUrl == SIP_NULL) 
		return;
	HSS_LOCKREF(pUrl->dRefCount);
	HSS_DECREF(pUrl->dRefCount);
	if(HSS_CHECKREF(pUrl->dRefCount))
	{

		HSS_FREE(pUrl->pDispName);
		HSS_FREE(pUrl->pUser);
		HSS_FREE(pUrl->pHost);
		if (pUrl->slRoute.size != 0)
			if(sip_listDeleteAll ( &(pUrl->slRoute), &dErr)==SipFail)
				return;
		if (pUrl->slParams.size != 0)
			if(sip_listDeleteAll ( &(pUrl->slParams), &dErr)==SipFail)
				return;
		HSS_UNLOCKREF(pUrl->dRefCount);
		HSS_DELETEREF(pUrl->dRefCount);
		HSS_FREE(pUrl);
	}
	else
	{
		HSS_UNLOCKREF(pUrl->dRefCount);
	}
}

