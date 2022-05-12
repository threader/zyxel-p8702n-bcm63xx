
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 
/**************************************************************
 ** FUNCTION:
 **	 	This file has the prototype definitions for the tel-url
 **		APIs
 *************************************************************
 **
 ** FILENAME:
 ** telapi.h
 **
 ** DESCRIPTION:
 **	
 **	 
 ** DATE			NAME			REFERENCE		REASON
 ** ----			----			--------		------
 ** 4Jan01 		    Rajasri			--				Initial Creation	
****************************************************************/

#ifndef __TEL_API_H_
#define __TEL_API_H_

#include "microsip_telstruct.h"
#include "microsip_common.h"

#define MAX_TEL_URL_SIZE 2048

/***********************************************************************
** Function: sip_isTelUrl 
** Description: Checks if the Addrspec has a tel-url
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec								
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_isTelUrl _ARGS_ ((SipAddrSpec *pAddrSpec, \
	SipError *pErr));

/***********************************************************************
** Function: sip_getTelUrlFromAddrSpec 
** Description: gets the TelUrl from the SipAddrSpec structure
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec								
**		ppTelUrl (OUT)	- retrieved TelUrl
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getTelUrlFromAddrSpec _ARGS_ ((SipAddrSpec *pAddrSpec,\
	TelUrl **ppTelUrl, SipError *pErr));

/***********************************************************************
** Function: sip_setTelUrlInAddrSpec 
** Description: sets the TelUrl in the SipAddrSpec structure
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec								
**		pTelUrl (OUT)	- retrieved TelUrl
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setTelUrlInAddrSpec _ARGS_ ((SipAddrSpec *pAddrSpec,\
	TelUrl *pTelUrl, SipError *pErr));


/***********************************************************************
** Function: sip_getGlobalNumFromTelUrl 
** Description: gets the global number from the TelUrl structure
** Parameters:
**		pUrl (IN)		- TelUrl								
**		ppGlobal (OUT)	- retrieved global number
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getGlobalNumFromTelUrl _ARGS_((TelUrl *pUrl, \
	TelGlobalNum **ppGlobal, SipError *pErr));

/***********************************************************************
** Function: sip_setGlobalNumInTelUrl
** Description: sets the global number in the TelUrl structure
** Parameters:
**		pUrl (IN/OUT)	- TelUrl								
**		pGlobal (OUT)	- Global number to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setGlobalNumInTelUrl _ARGS_ ((TelUrl *pUrl, \
	TelGlobalNum *pGlobal, SipError *pErr));

/***********************************************************************
** Function: sip_getLocalNumFromTelUrl
** Description: gets the local url from the TelUrl structure
** Parameters:
**		pUrl (IN)		- TelUrl								
**		ppLocal (OUT)	- retrieved local number
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getLocalNumFromTelUrl _ARGS_ ((TelUrl *pUrl,\
	TelLocalNum **ppLocal, SipError *pErr));

/***********************************************************************
** Function: sip_setLocalNumInTelUrl
** Description: sets the local url from the TelUrl structure
** Parameters:
**		pUrl (IN/OUT)	- TelUrl								
**		pLocal (OUT)	- local number to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setLocalNumInTelUrl _ARGS_ ((TelUrl *pUrl,TelLocalNum \
	*pLocal, SipError *pErr));

/**********************************/
/*apis for TelLocalNum*/
/*******************************/
/***********************************************************************
** Function: sip_getLocalPhoneDigitFromTelLocalNum
** Description: gets the LocalPhoneDigits from the TelLocalNum structure
** Parameters:
**		pLocal (IN)		- TelLocalNum								
**		ppPhone (OUT)	- retrieved local phone digit
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getLocalPhoneDigitFromTelLocalNum _ARGS_ (( \
	TelLocalNum *pLocal, SIP_S8bit **ppPhone, SipError *pErr));

/***********************************************************************
** Function: sip_setLocalPhoneDigitInTelLocalNum
** Description: sets the LocalPhoneDigits in the TelLocalNum structure
** Parameters:
**		pLocal (IN/OUT)		- TelLocalNum								
**		pPhoneNum (IN)		- LocalPhoneDigit to be set
**		pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setLocalPhoneDigitInTelLocalNum _ARGS_ ((TelLocalNum *pLocal,\
	SIP_S8bit *pPhoneNum, SipError *pErr));

/***********************************************************************
** Function: sip_getIsdnNoFromTelLocalNum
** Description: gets the isdn sub address from the TelLocalNum structure
** Parameters:
**		pLocal (IN)		- TelLocalNum								
**		ppIsdnno (OUT)	- retrieved isdn sub address 
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getIsdnNoFromTelLocalNum _ARGS_ ((TelLocalNum *pLocal,\
	SIP_S8bit **ppIsdnno, SipError *pErr));

/***********************************************************************
** Function: sip_setIsdnNoInTelLocalNum
** Description: sets the isdn no in the TelLocalNum structure
** Parameters:
**		pLocal (IN/OUT)		- TelLocalNum								
**		pIsdnno (IN)	- isdn no to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setIsdnNoInTelLocalNum _ARGS_ ((TelLocalNum *pLocal,\
	SIP_S8bit *pIsdnno, SipError *pErr));

/***********************************************************************
** Function: sip_getPostdialFromTelLocalNum 
** Description: gets the post dial from the TelLocaLNum structure
** Parameters:
**		pLocal (IN)		- TelLocalNum								
**		ppPostd (OUT)	- retreived post dial field
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getPostdialFromTelLocalNum _ARGS_ ((TelLocalNum *pLocal,\
	SIP_S8bit **ppPostd, SipError *pErr));

/***********************************************************************
** Function: sip_setPostdialInTelLocalNum
** Description: sets the post dial in   the TelLocalNum structure
** Parameters:
**		pLocal (IN/OUT)		- TelLocalNum								
**		ppPostd (OUT)	- retreived post dial field
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setPostdialInTelLocalNum _ARGS_ ((TelLocalNum *pLocal,\
	SIP_S8bit *pPostd, SipError *pErr));


/***********************************************************************
** Function: sip_getAreaSpecifierCountFromTelLocalNum
** Description: gets the number of AreaSpecifiers in the TelLocalNum structure
** Parameters:
**		pLocal (IN)		- TelLocalNum								
**		pCount (OUT)	- number of AreaSpecifier fields
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getAreaSpecifierCountFromTelLocalNum _ARGS_ ((TelLocalNum \
	*pLocal, SIP_U32bit *pCount, SipError *pErr));

/***********************************************************************
** Function: sip_getAreaSpecifierAtIndexFromTelLocalNum 
** Description: gets the AreaSpecifier field at the index from TelLocalNum
** Parameters:
**	pLocal (IN)			- TelLocalNum								
**	ppAreaSpecifier(OUT)- retreived AreaSpecifier 
**	dIndex (IN)			- index at which AreaSpecifier is to be retrieved
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getAreaSpecifierAtIndexFromTelLocalNum _ARGS_ ((\
	TelLocalNum* pLocal, SIP_S8bit **ppAreaSpecifier, SIP_U32bit dIndex,\
	 SipError *pErr));

/***********************************************************************
** Function: sip_setAreaSpecifierAtIndexFromTelLocalNum 
** Description: sets the AreaSpecifier field at the index in TelLocalNum
** Parameters:
**	pLocal (IN/OUT)		- TelLocalNum								
**	pAreaSpecifier(IN)	- AreaSpecifier to be set
**	dIndex (IN)			- index at which AreaSpecifier is to be set
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setAreaSpecifierAtIndexInTelLocalNum _ARGS_ ((TelLocalNum* \
	pLocal, SIP_S8bit *pAreaSpecifier, SIP_U32bit dIndex, \
	SipError *pErr));

/***********************************************************************
** Function: sip_insertAreaSpecifierAtIndexInTelLocalNum
** Description: inserts the AreaSpecifier field at the index in TelLocalNum
** Parameters:
**	pLocal (IN/OUT)		- TelLocalNum								
**	pAreaSpecifier(IN)	- AreaSpecifier to be inserted
**	dIndex (IN)			- index at which AreaSpecifier is to be inserted
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_insertAreaSpecifierAtIndexInTelLocalNum _ARGS_ (\
	(TelLocalNum *pLocal, SIP_S8bit *pAreaSpecifier, SIP_U32bit dIndex,\
	 SipError *pErr));

/***********************************************************************
** Function: sip_deleteAreaSpecifierAtIndexInTelLocalNum
** Description: deletes the AreaSpecifier field at the index in TelLocalNum
** Parameters:
**	pLocal (IN/OUT)		- TelLocalNum								
**	pAreaSpecifier(IN)	- AreaSpecifier to be deleted
**	dIndex (IN)			- index at which AreaSpecifier is to be deleted
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_deleteAreaSpecifierAtIndexInTelLocalNum _ARGS_ (\
	(TelLocalNum *pLocal, SIP_U32bit dIndex, SipError *err));

/***********************************************************************
** Function: sip_getParamCountFromTelLocalNum
** Description: gets the number of parameters in TelLocalNum structure
**             (service provider and future extension are treated as parameters)
** Parameters:
**		pLocal (IN)		- TelLocalNum								
**		pCount(OUT)		- number of parameters
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getParamCountFromTelLocalNum _ARGS_ ((TelLocalNum *pLocal,\
	SIP_U32bit *pCount, SipError *pErr));

/***********************************************************************
** Function: sip_getParamAtIndexFromTelLocalNum
** Description: gets the parameter at the index from the TelLocalNum structure
**             (service provider and future extension are treated as parameters)
** Parameters:
**		pLocal (IN)		- TelLocalNum								
**		ppParam (OUT)	- retrieved parameter
**		dIndex (IN)		- index at which parameter is to be retreived
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getParamAtIndexFromTelLocalNum _ARGS_ ((TelLocalNum *pLocal,\
	SipParam **ppParam, SIP_U32bit dIndex, SipError *pErr));

/***********************************************************************
** Function: sip_setParamAtIndexInTelLocalNum
** Description: sets the parameter at the index in the TelLocalNum structure
**             (service provider and future extension are treated as parameters)
** Parameters:
**		pLocal (IN/OUT)	- TelLocalNum								
**		pParam (IN)	- parameter to set
**		dIndex (IN)		- index at which parameter is to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setParamAtIndexInTelLocalNum _ARGS_ ((TelLocalNum \
*pLocal, SipParam *pParam, SIP_U32bit dIndex, SipError *pErr));

/***********************************************************************
** Function: sip_insertParamAtIndexInTelLocalNum 
** Description: inserts the parameter at the index in the TelLocalNum structure
**             (service provider and future extension are treated as parameters)
** Parameters:
**		pLocal (IN/OUT)		- TelLocalNum								
**		pParam (IN)	- parameter to inserted
**		dIndex (IN)		- index at which parameter is to be inserted
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_insertParamAtIndexInTelLocalNum _ARGS_ ((TelLocalNum\
 *pLocal, SipParam *pParam, SIP_U32bit dIndex, SipError *pErr));

/***********************************************************************
** Function: sip_deleteParamAtIndexInTelLocalNum
** Description: deletes the parameter at the index in the TelLocalNum structure
**             (service provider and future extension are treated as parameters)
** Parameters:
**		pLocal (IN/OUT)	- TelLocalNum								
**		dIndex (IN)		- index at which parameter is to be deleted
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_deleteParamAtIndexInTelLocalNum _ARGS_ ((TelLocalNum *local,\
	SIP_U32bit dIndex, SipError *pErr));

/**********************************/
/*apis for TelGlobalNum*/
/*******************************/
/***********************************************************************
** Function: sip_getBaseNoFromTelGlobalNum
** Description: gets the  base number from TelGlobalNum structure
** Parameters:
**		pGlobal	(IN)- TelGlobalNum								
**		ppBase (IN)		- retreived Base number  
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getBaseNoFromTelGlobalNum _ARGS_ ((TelGlobalNum *pGlobal,\
	SIP_S8bit **ppBase, SipError *pErr));

/***********************************************************************
** Function: sip_setBaseNoInTelGlobalNum
** Description: sets the base number in TelGlobalNum structure
** Parameters:
**		pGlobal	(IN/OUT)- TelGlobalNum								
**		pBase (IN)		- Base number to set 
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setBaseNoInTelGlobalNum _ARGS_ ((TelGlobalNum *pGlobal,\
	SIP_S8bit *pBase, SipError *pErr));

/***********************************************************************
** Function: sip_getIsdnNoFromTelGlobalNum
** Description: gets the isdn number from TelGlobalNum 
** Parameters:
**		pGlobal	(IN)	- TelGlobalNum								
**		ppIsdnno (IN)	- retreived isdn number 
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getIsdnNoFromTelGlobalNum _ARGS_ ((TelGlobalNum *pGlobal,\
	SIP_S8bit **ppIsdnno, SipError *pErr));

/***********************************************************************
** Function: sip_setIsdnNoInTelGlobalNum
** Description: sets the isdn number in TelGlobalNum 
** Parameters:
**		pGlobal (IN/OUT)- TelGlobalNum								
**		pIsdnno(IN)		- Isdn number to set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setIsdnNoInTelGlobalNum _ARGS_ ((TelGlobalNum *pGlobal,\
	SIP_S8bit *pIsdnno, SipError *pErr));

/***********************************************************************
** Function: sip_getPostdialFromTelGlobalNum
** Description: gets the PostDial field from TelGlobalNum 
** Parameters:
**		pGlobal (IN)	- TelGlobalNum								
**		ppPostd(OUT)	- retreived PostDial fiels
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getPostdialFromTelGlobalNum _ARGS_ ((TelGlobalNum *pGlobal,\
	SIP_S8bit **ppPostd, SipError *pErr));

/***********************************************************************
** Function: sip_setPostdialInTelGlobalNum
** Description: sets the PostDial field in TelGlobalNum 
** Parameters:
**		pGlobal (IN/OUT)- TelGlobalNum								
**		pPostd(IN)		- PostDial to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setPostdialInTelGlobalNum _ARGS_ ((TelGlobalNum *pGlobal,\
	SIP_S8bit *pPostd,SipError *pErr));

/***********************************************************************
** Function: sip_getParamCountFromTelGlobalNum
** Description: gets the number of parameters in TelGlobalNum 
**             (service provider and future extension are treated as parameters)
** Parameters:
**		pGlobal (IN)	- TelGlobalNum								
**		pCount (OUT)	- number of parameters
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getParamCountFromTelGlobalNum _ARGS_ ((TelGlobalNum \
	*pGlobal, SIP_U32bit *pCount, SipError *pErr));

/*****************************************************************************
** Function: sip_getParamAtIndexInTelGlobalNum
** Description: gets the Param at the specified index in TelGlobalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**	pGlobal (IN)	- TelGlobalNum	
**	ppParam(OUT)	- retreived Parameter 
**	dIndex (IN)		- index at which param is to be retieved
**	pErr (OUT)		- Possible Error value (see API ref doc)
******************************************************************************/
extern SipBool sip_getParamAtIndexFromTelGlobalNum _ARGS_ ((TelGlobalNum \
	*global, SipParam **ppParam, SIP_U32bit dIndex, SipError *pErr));

/*****************************************************************************
** Function: sip_setParamAtIndexInTelGlobalNum
** Description: sets the Param at the specified index in TelGlobalNum
**            (service provider and future extension are treated as parameters)
** Parameters:
**	pGlobal (IN/OUT)	- TelGlobalNum	
**	pParam(IN)			- Param to be set
**	dIndex (IN)			- index at which param is set in Tel Global number
**	pErr (OUT)			- Possible Error value (see API ref doc)
******************************************************************************/
extern SipBool sip_setParamAtIndexInTelGlobalNum _ARGS_ ((TelGlobalNum \
	*pGlobal, SipParam *pParam, SIP_U32bit dIndex, SipError *pErr));

/******************************************************************************
** Function: sip_insertParamAtIndexInTelGlobalNum
** Description: inserts the Param at the specified index in TelGlobalNum 
**             (service provider and future extension are treated as parameters)
** Parameters:
**	pGlobal (IN/OUT)- TelGlobalNum	
**	pParam(IN)		- Param to be inserted
**	dIndex (IN)		- index at which param is inserted in Tel Global number
**	pErr (OUT)		- Possible Error value (see API ref doc)
*******************************************************************************/
extern SipBool sip_insertParamAtIndexInTelGlobalNum _ARGS_ ((TelGlobalNum \
	*pGlobal, SipParam *pParam, SIP_U32bit dIndex, SipError *pErr));

/***********************************************************************
** Function: sip_deleteParamAtIndexInTelGlobalNum
** Description: deletes the param at the specified index in TelGlobalNum
**            (service provider and future extension are treated as parameters)
** Parameters:
**	pGlobal (IN)	- TelGlobalNum	
**	dIndex (IN)		- index at which param is deleted in Tel Global number
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_deleteParamAtIndexInTelGlobalNum _ARGS_ ((TelGlobalNum \
	*pGlobal, SIP_U32bit dIndex, SipError *pErr));

/***********************************************************************
** Function: sip_getAreaSpecifierCountFromTelGlobalNum
** Description: gets the scheme from the TelUrl structure
** Parameters:
**	pGlobal (IN)	- TelGlobalNum	
**	pCount (OUT)	- number of Tel AreaSpecifiers 
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getAreaSpecifierCountFromTelGlobalNum _ARGS_ ((\
	TelGlobalNum *pGlobal, SIP_U32bit *pCount, SipError *pErr));

/*****************************************************************************
** Function: sip_getAreaSpecifierAtIndexFromTelGlobalNum
** Description: gets the AreaSpecifier at the specified index in TelGlobalNum
** Parameters:
**	pGlobal (IN)			- TelGlobalNum								
**	ppAreaSpecifier(OUT)	- retreived AreaSpecifier
**	dIndex (IN)				- index to get AreaSpecifier at
**	pErr (OUT)				- Possible Error value (see API ref doc)
******************************************************************************/
extern SipBool sip_getAreaSpecifierAtIndexFromTelGlobalNum _ARGS_ ((\
	TelGlobalNum *pGlobal, SIP_S8bit **ppAreaSpecifier, SIP_U32bit \
	dIndex, SipError *pErr));

/******************************************************************************
** Function: sip_setAreaSpecifierAtIndexInTelGlobalNum
** Description: sets the AreaSpecifier at the specified index in TelGlobalNum
** Parameters:
**	pGlobal (IN/OUT)		- TelGlobalNum								
**	pAreaSpecifier(IN)		- AreaSpecifier to set
**	dIndex (IN)				- index to set AreaSpecifier at
**	pErr (OUT)				- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setAreaSpecifierAtIndexInTelGlobalNum _ARGS_ ((\
	TelGlobalNum *pGlobal, SIP_S8bit *pAreaSpecifier, SIP_U32bit\
	dIndex, SipError *pErr));

/*******************************************************************************
** Function: sip_insertAreaSpecifierAtIndexInTelGlobalNum
** Description: inserts the AreaSpecifier at the specified index in TelGlobalNum
** Parameters:
**		pGlobal (IN/OUT)		- TelGlobalNum								
**		pAreaSpecifier(IN)		- AreaSpecifier to insert
**		dIndex (IN)				- index to insert AreaSpecifier at
**		pErr (OUT)				- Possible Error value (see API ref doc)
*******************************************************************************/
extern SipBool sip_insertAreaSpecifierAtIndexInTelGlobalNum _ARGS_ ((\
	TelGlobalNum *pGlobal, SIP_S8bit *pAreaSpecifier, SIP_U32bit \
	dIndex, SipError *pErr));

/*******************************************************************************
** Function: sip_deleteAreaSpecifierAtIndexInTelGlobalNum 
** Description: deletes the AreaSpecifier at the specified index in TelGlobalNum
** Parameters:
**		pGlobal (IN/OUT)	- TelGlobalNum
**		dIndex (IN)			- index to delete AreaSpecifier at
**		pErr   (OUT)		- Possible Error value (see API ref doc)
******************************************************************************/
extern SipBool sip_deleteAreaSpecifierAtIndexInTelGlobalNum _ARGS_ ((\
	TelGlobalNum *pGlobal, SIP_U32bit dIndex, SipError *pErr));

/*******************************************************************************
** Function:  sip_compareTelUri
** Description: does the deep comparision of TelURL's
** Parameters:
**		pErr   (OUT)		- Possible Error value (see API ref doc)
******************************************************************************/

extern SipBool  sip_compareTelUri _ARGS_((TelUrl *pTel1, \
 							TelUrl *pTel2, SipError *pError));

#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif
