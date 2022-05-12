/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/***********************************************************************
 ** FUNCTION:
 **             Has Free Function Declarations For all tel-url Structures

 *********************************************************************
 **
 ** FILENAME:
 ** telfree.c
 **
 ** DESCRIPTION:
 ** This file contains code to free all structures
 **
 ** DATE        NAME                    REFERENCE               REASON
 ** ----        ----                    ---------              --------
 ** 4Jan01  	Rajasri       		                    Initial Creation
 **
 **
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/


#include "microsip_telstruct.h"
#include "microsip_telfree.h"
#include "microsip_free.h"


/*****************************************************************
** FUNCTION:sip_freeTelLocalNum
**
**
** DESCRIPTION:
*******************************************************************/
void sip_freeTelLocalNum 
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal)
#else
 	(pLocal)
	TelLocalNum *pLocal;
#endif
{
	SipError dErr;

	if (pLocal == SIP_NULL)
		return;
	HSS_LOCKREF(pLocal->dRefCount);HSS_DECREF(pLocal->dRefCount);
	if(HSS_CHECKREF(pLocal->dRefCount))
	{	
		HSS_FREE(pLocal->pLocalPhoneDigit);
		HSS_FREE(pLocal->pIsdnSubAddr);
		HSS_FREE(pLocal->pPostDial);
		(void)sip_listDeleteAll ( &(pLocal->slAreaSpecifier), &dErr);
		(void)sip_listDeleteAll ( &(pLocal->slParams), &dErr);
		HSS_UNLOCKREF(pLocal->dRefCount);
		HSS_DELETEREF(pLocal->dRefCount);
		HSS_FREE(pLocal);
	}
	else
	{
		HSS_UNLOCKREF(pLocal->dRefCount);
	}
}

/*****************************************************************
** FUNCTION:sip_freeTelGlobalNum
**
**
** DESCRIPTION:
*******************************************************************/
void sip_freeTelGlobalNum 
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal)
#else
 	(pGlobal)
	TelGlobalNum *pGlobal;
#endif
{
	SipError dErr;

	if (pGlobal == SIP_NULL)
		return;
	HSS_LOCKREF(pGlobal->dRefCount);HSS_DECREF(pGlobal->dRefCount);
	if(HSS_CHECKREF(pGlobal->dRefCount))
	{
		HSS_FREE(pGlobal->pBaseNo);
		HSS_FREE(pGlobal->pIsdnSubAddr);
		HSS_FREE(pGlobal->pPostDial);
		if (pGlobal->slAreaSpecifier.size != 0)
			(void)sip_listDeleteAll ( &(pGlobal->slAreaSpecifier), &dErr);
		if (pGlobal->slParams.size != 0)
			(void)sip_listDeleteAll ( &(pGlobal->slParams), &dErr);
		HSS_UNLOCKREF(pGlobal->dRefCount);
		HSS_DELETEREF(pGlobal->dRefCount);
		HSS_FREE(pGlobal);
	}
	else
	{
		HSS_UNLOCKREF(pGlobal->dRefCount);
	}
}







/*****************************************************************
** FUNCTION:sip_freeTelUrl
**
**
** DESCRIPTION:
*******************************************************************/
void sip_freeTelUrl 
#ifdef ANSI_PROTO
	(TelUrl *pUrl)
#else
 	(pUrl)
	TelUrl *pUrl;
#endif
{
	if (pUrl == SIP_NULL) 
		return;
	HSS_LOCKREF(pUrl->dRefCount);HSS_DECREF(pUrl->dRefCount);
	if(HSS_CHECKREF(pUrl->dRefCount))
	{
		if (pUrl->pGlobal != SIP_NULL)
			sip_freeTelGlobalNum(pUrl->pGlobal);
		if (pUrl->pLocal != SIP_NULL)
			sip_freeTelLocalNum(pUrl->pLocal);
		HSS_UNLOCKREF(pUrl->dRefCount);
		HSS_DELETEREF(pUrl->dRefCount);
		HSS_FREE(pUrl);
	}
	else
	{
		HSS_UNLOCKREF(pUrl->dRefCount);
	}
}



/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

