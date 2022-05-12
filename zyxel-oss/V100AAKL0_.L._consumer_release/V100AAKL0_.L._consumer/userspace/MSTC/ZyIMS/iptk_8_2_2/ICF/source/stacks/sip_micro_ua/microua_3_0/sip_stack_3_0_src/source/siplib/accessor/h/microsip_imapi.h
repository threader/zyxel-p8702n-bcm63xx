/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************
 ** FUNCTION:
 **	 	This file has all API definitions of   IM Url  related structure
 **
 ******************************************************************************
 **
 ** FILENAME:
 ** 		impp.h
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

#ifndef _IMPPAPI_H_
#define _IMPPAPI_H_

#include "microsip_imurlstruct.h"
#include "microsip_common.h"
#ifndef MAX_IM_URL_SIZE
#define MAX_IM_URL_SIZE 2048
#endif

/***********************************************************************
** Function: sip_getImUrlFromAddrSpec 
** Description: gets the ImUrl from the SipAddrSpec structure
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec								
**		ppImUrl (OUT)	- retrieved ImUrl
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getImUrlFromAddrSpec _ARGS_ ((SipAddrSpec *pAddrSpec,\
	ImUrl **ppImUrl, SipError *pErr));

extern SipBool sip_getPresUrlFromAddrSpec _ARGS_ ((SipAddrSpec *pAddrSpec,\
	ImUrl **ppImUrl, SipError *pErr));



/***********************************************************************
** Function: sip_setImUrlInAddrSpec 
** Description: sets the ImUrl in the SipAddrSpec structure
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec								
**		pImUrl (OUT)	- retrieved ImUrl
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setImUrlInAddrSpec _ARGS_ ((SipAddrSpec *pAddrSpec,\
	ImUrl *pImUrl, SipError *pErr));
extern SipBool sip_setPresUrlInAddrSpec _ARGS_ ((SipAddrSpec *pAddrSpec,\
	ImUrl *pImUrl, SipError *pErr));



/***********************************************************************
** Function: sip_getDispNameFromImUrl 
** Description: gets the Disp Name from the ImUrl structure
** Parameters:
**		pUrl (IN)		- ImUrl
**		ppDispName (OUT)- retrieved Disp Name
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getDispNameFromImUrl _ARGS_((ImUrl *pUrl, \
	SIP_S8bit **ppDispName, SipError *pErr));


#ifdef SIP_PRES
#define sip_getDispNameFromPresUrl         sip_getDispNameFromImUrl
#endif


/***********************************************************************
** Function: sip_setDispNameInImUrl
** Description: sets the DispName in the ImUrl structure
** Parameters:
**		pUrl (IN/OUT)	- ImUrl
**		pDispName (OUT)	- Disp Name to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setDispNameInImUrl _ARGS_ ((ImUrl *pUrl, \
	SIP_S8bit *pDispName, SipError *pErr));

#ifdef SIP_PRES
#define sip_setDispNameInPresUrl           sip_setDispNameInImUrl
#endif


/***********************************************************************
** Function: sip_getUserFromImUrl 
** Description: gets the User Name from the ImUrl structure
** Parameters:
**		pUrl (IN)		- ImUrl
**		ppUser (OUT)	- retrieved User Name
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getUserFromImUrl _ARGS_((ImUrl *pUrl, \
	SIP_S8bit **ppUser, SipError *pErr));


#ifdef SIP_PRES
#define sip_getUserFromPresUrl             sip_getUserFromImUrl
#endif


/***********************************************************************
** Function: sip_setUserInImUrl
** Description: sets the User in the ImUrl structure
** Parameters:
**		pUrl (IN/OUT)	- ImUrl
**		pUser (OUT)		- User Name to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setUserInImUrl _ARGS_ ((ImUrl *pUrl, \
	SIP_S8bit *pUser, SipError *pErr));

#ifdef SIP_PRES
#define sip_setUserInPresUrl               sip_setUserInImUrl
#endif



/***********************************************************************
** Function: sip_getHostFromImUrl 
** Description: gets the Host Name from the ImUrl structure
** Parameters:
**		pUrl (IN)		- ImUrl
**		ppHost (OUT)	- retrieved Host
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getHostFromImUrl _ARGS_((ImUrl *pUrl, \
	SIP_S8bit **ppHost, SipError *pErr));

#ifdef SIP_PRES
#define sip_getHostFromPresUrl             sip_getHostFromImUrl
#endif


/***********************************************************************
** Function: sip_setHostInImUrl
** Description: sets the Host in the ImUrl structure
** Parameters:
**		pUrl (IN/OUT)	- ImUrl
**		pHost (OUT)		- Host to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setHostInImUrl _ARGS_ ((ImUrl *pUrl, \
	SIP_S8bit *pHost, SipError *pErr));

#ifdef SIP_PRES
#define sip_setHostInPresUrl             sip_setHostInImUrl
#endif



/***********************************************************************
** Function: sip_getParamCountFromImUrl
** Description: gets the number of parameters in ImUrl
** Parameters:
**		pImUrl (IN)		- ImUrl 
**		pCount (OUT)	- number of parameters
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getParamCountFromImUrl _ARGS_ ((ImUrl \
	*pImUrl, SIP_U32bit *pCount, SipError *pErr));

#ifdef SIP_PRES
#define sip_getParamCountFromPresUrl       sip_getParamCountFromImUrl
#endif



/*****************************************************************************
** Function: sip_getParamAtIndexFromImUrl
** Description: gets the Param at the specified index in ImUrl
** Parameters:
**	pImUrl (IN)		- ImUrl
**	ppParam(OUT)	- retreived Parameter 
**	dIndex (IN)		- index at which param is to be retieved
**	pErr (OUT)		- Possible Error value (see API ref doc)
******************************************************************************/
extern SipBool sip_getParamAtIndexFromImUrl _ARGS_ ((ImUrl \
	*pImUrl, SipParam **ppParam, SIP_U32bit dIndex, SipError *pErr));

#ifdef SIP_PRES
#define sip_getParamAtIndexFromPresUrl     sip_getParamAtIndexFromImUrl
#endif



/*****************************************************************************
** Function: sip_setParamAtIndexInImUrl
** Description: sets the Param at the specified index in ImUrl
** Parameters:
**	pImUrl (IN/OUT)		- ImUrl	
**	pParam(IN)			- Param to be set
**	dIndex (IN)			- index at which param is set in ImUrl
**	pErr (OUT)			- Possible Error value (see API ref doc)
******************************************************************************/
extern SipBool sip_setParamAtIndexInImUrl _ARGS_ ((ImUrl \
	*pImUrl, SipParam *pParam, SIP_U32bit dIndex, SipError *pErr));


#ifdef SIP_PRES
#define sip_setParamAtIndexInPresUrl       sip_setParamAtIndexInImUrl
#endif


/******************************************************************************
** Function: sip_insertParamAtIndexInImUrl
** Description: inserts the Param at the specified index in ImUrl
** Parameters:
**	pImUrl (IN/OUT)	- ImUrl
**	pParam(IN)		- Param to be inserted
**	dIndex (IN)		- index at which param is inserted in ImUrl
**	pErr (OUT)		- Possible Error value (see API ref doc)
*******************************************************************************/
extern SipBool sip_insertParamAtIndexInImUrl _ARGS_ ((ImUrl \
	*pImUrl, SipParam *pParam, SIP_U32bit dIndex, SipError *pErr));

#ifdef SIP_PRES
#define sip_insertParamAtIndexInPresUrl    sip_insertParamAtIndexInImUrl
#endif


/***********************************************************************
** Function: sip_deleteParamAtIndexInImUrl
** Description: deletes the param at the specified index in ImUrl
** Parameters:
**	pImUrl (IN)		- ImUrl	
**	dIndex (IN)		- index at which param is deleted in ImUrl
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_deleteParamAtIndexInImUrl _ARGS_ ((ImUrl \
	*pImUrl, SIP_U32bit dIndex, SipError *pErr));

#ifdef SIP_PRES
#define sip_deleteParamAtIndexInPresUrl    sip_deleteParamAtIndexInImUrl
#endif


/***********************************************************************
** Function: sip_getRouteCountFromImUrl
** Description: gets the route count from the ImUrl structure
** Parameters:
**	pImUrl (IN)		- ImUrl	
**	pCount (OUT)	- number of ImUrl Routes 
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getRouteCountFromImUrl _ARGS_ ((\
	ImUrl *pImUrl, SIP_U32bit *pCount, SipError *pErr));

#ifdef SIP_PRES
#define sip_getRouteCountFromPresUrl       sip_getRouteCountFromImUrl
#endif


/*****************************************************************************
** Function: sip_getRouteAtIndexFromImUrl
** Description: gets the Route at the specified index in ImUrl
** Parameters:
**	pImUrl (IN)		- ImUrl								
**	ppRoute(OUT)	- retreived AreaSpecifier
**	dIndex (IN)		- index to get Route at
**	pErr (OUT)		- Possible Error value (see API ref doc)
******************************************************************************/
extern SipBool sip_getRouteAtIndexFromImUrl _ARGS_ ((\
	ImUrl *pImUrl, SIP_S8bit **ppRoute, SIP_U32bit \
	dIndex, SipError *pErr));

#ifdef SIP_PRES
#define sip_getRouteAtIndexFromPresUrl     sip_getRouteAtIndexFromImUrl
#endif


/******************************************************************************
** Function: sip_setRouteAtIndexInImUrl
** Description: sets the Route at the specified index in ImUrl
** Parameters:
**	pImUrl (IN/OUT)	- ImUrl								
**	pRoute(IN)		- AreaSpecifier to set
**	dIndex (IN)		- index to set Route at
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setRouteAtIndexInImUrl _ARGS_ ((\
	ImUrl *pImUrl, SIP_S8bit *pRoute, SIP_U32bit\
	dIndex, SipError *pErr));

#ifdef SIP_PRES
#define sip_setRouteAtIndexInPresUrl       sip_setRouteAtIndexInImUrl
#endif


/*******************************************************************************
** Function: sip_insertRouteAtIndexInImUrl
** Description: inserts the Route at the specified index in ImUrl
** Parameters:
**		pImUrl (IN/OUT)	- ImUrl								
**		pRoute(IN)		- Route to insert
**		dIndex (IN)		- index to insert Route at
**		pErr (OUT)		- Possible Error value (see API ref doc)
*******************************************************************************/
extern SipBool sip_insertRouteAtIndexInImUrl _ARGS_ ((\
	ImUrl *pImUrl, SIP_S8bit *pRoute, SIP_U32bit \
	dIndex, SipError *pErr));

#ifdef SIP_PRES
#define sip_insertRouteAtIndexInPresUrl    sip_insertRouteAtIndexInImUrl
#endif


/*******************************************************************************
** Function: sip_deleteRouteAtIndexInImUrl 
** Description: deletes the Route at the specified index in ImUrl
** Parameters:
**		pImUrl (IN/OUT)	- ImUrl
**		dIndex (IN)		- index to delete Route at
**		pErr   (OUT)	- Possible Error value (see API ref doc)
******************************************************************************/
extern SipBool sip_deleteRouteAtIndexInImUrl _ARGS_ ((\
	ImUrl *pImUrl, SIP_U32bit dIndex, SipError *pErr));

#ifdef SIP_PRES
#define sip_deleteRouteAtIndexInPresUrl    sip_deleteRouteAtIndexInImUrl
#endif



#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 


