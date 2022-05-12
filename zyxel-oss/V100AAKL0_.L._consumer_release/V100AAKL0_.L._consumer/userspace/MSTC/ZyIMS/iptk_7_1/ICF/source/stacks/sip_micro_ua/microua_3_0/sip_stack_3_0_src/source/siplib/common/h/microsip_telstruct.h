/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 


/******************************************************************************* ** FUNCTION:
 **	 	This file has all the Structures related tel-url

 ******************************************************************************* **
 ** FILENAME:
 ** telstruct.h
 **
 ** DESCRIPTION:
 **	All primary structures are stored here
 **
 ** DATE			NAME			REFERENCE		REASON
 ** ----			----			---------		------
 ** 4Jan01			Rajasri							Initial Creation
*******************************************************************************/

#ifndef __TELSTRUCT_H__
#define __TELSTRUCT_H__

#include "microsip_common.h"
#include "microsip_struct.h"

typedef struct
{
	SIP_S8bit *pBaseNo;
	SIP_S8bit *pIsdnSubAddr;
	SIP_S8bit *pPostDial;
	SipList   slAreaSpecifier; /* of SIP_S8bit* */
	SipList   slParams;		  /* holds service-provider and future extensions */
	SIP_RefCount  dRefCount;/* Reference Count for structures in new version */
} TelGlobalNum;

typedef struct
 {
	SIP_S8bit *pLocalPhoneDigit;
	SIP_S8bit *pIsdnSubAddr;
	SIP_S8bit *pPostDial;
	SipList   slAreaSpecifier;  /* of SIP_S8bit* */
	SipList    slParams;	/* holds service-provider and future extensions */
	SIP_RefCount dRefCount; /* Reference Count for structures in new version */
}TelLocalNum;

typedef struct
{
	TelGlobalNum *pGlobal;
	TelLocalNum *pLocal;
	SIP_RefCount dRefCount;/* Reference Count for structures in new version */
}TelUrl;

typedef struct
{
	SipList	*pGCList;
	SipError *pError;
	TelUrl *pTelUrl;
} SipTelParserParam;

#endif /* __TELSTRUCT */
 
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

