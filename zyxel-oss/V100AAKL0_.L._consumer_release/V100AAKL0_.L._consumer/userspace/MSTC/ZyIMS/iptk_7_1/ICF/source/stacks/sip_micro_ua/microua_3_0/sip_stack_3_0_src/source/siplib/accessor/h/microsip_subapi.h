/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif
/***********************************************************************
** FUNCTION:
**		Accessor API functionality for various sub-structures.
************************************************************************
**
** FILENAME:
** microsip_subapi.c
**
** DESCRIPTION:
**		This file contains the accessor functions for various 
**		sub-structures used by the microSIP stack, such as
**		SipParam, SipAddrSpec, SipUrl, etc.
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 9/09/2002	Aparna Kuppachi		-			Initial					
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/

#ifndef __MICROSIP_SUBAPI_H__
#define __MICROSIP_SUBAPI_H__

#include "microsip_struct.h"
#include "microsip_common.h"
#include "microsip_portlayer.h"

/****************************************************************************
** FUNCTION: sip_getAddrTypeFromAddrSpec
** DESCRIPTION: This function retrieves the dType of a SIP dAddr-spec
** PARAMETERS:
**				pAddrspec(IN)	- Pointer to the address-spec
**				pAddrtpe(OUT)	- The retrieved dType
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_getAddrTypeFromAddrSpec _ARGS_ ((SipAddrSpec *pAddrspec,
	en_AddrType *pAddrtype, SipError *pErr));

/****************************************************************************
** FUNCTION: sip_getUriFromAddrSpec
** DESCRIPTION: This function retrieves the Uri from a addr-spec.
** PARAMETERS:
**				pAddrspec(IN)	- Pointer to the SipAddrSpec
**				ppUri(OUT)		- The retrieved URI
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_getUriFromAddrSpec _ARGS_ ((SipAddrSpec *pAddrspec,
	SIP_S8bit **ppUri, SipError *pErr));


/****************************************************************************
** FUNCTION: sip_setUriInAddrSpec
** DESCRIPTION: This function sets the Uri in a SIP addr-spec.
** PARAMETERS:
**				pAddrSpec(IN)	- Pointer to the SipAddrSpec
**				pUri(In)		- The URI to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_setUriInAddrSpec _ARGS_ ((SipAddrSpec *pAddrspec,
	SIP_S8bit *pUri, SipError *pErr));

/****************************************************************************
** FUNCTION: sip_getUrlFromAddrSpec
** DESCRIPTION: This function retrieves the url from a SIP addr-spec
** PARAMETERS:
**				pAddrSpec(IN)	- Pointer to the SipAddrSpec
**				ppUrl(IN)		- The retrieved URL
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_getUrlFromAddrSpec _ARGS_ ((SipAddrSpec *pAddrspec,
	SipUrl **ppUrl,	SipError *pErr));

/****************************************************************************
** FUNCTION: sip_setUrlInAddrSpec
** DESCRIPTION: This function sets the url field in a SipAddrSpec
** PARAMETERS:
**				pAddrSpec(IN)	- Pointer to the SipAddrSpec
**				pUrl(IN)		- The URL to be set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_setUrlInAddrSpec _ARGS_ ((SipAddrSpec *pAddrspec,
	SipUrl *pUrl, SipError *pErr));

/****************************************************************************
** FUNCTION: sip_getSUrlFromAddrSpec
** DESCRIPTION: This function retrieves the sips url from a SipAddrSpec
** PARAMETERS:
**				pAddrspec(IN)	- Pointer to the SipAddrSpec
**				ppUrl(OUT)		- The retrieved SIPS URL
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_getSUrlFromAddrSpec _ARGS_ ((SipAddrSpec *pAddrspec,
	SipUrl **ppUrl,	SipError *pErr));
	
/****************************************************************************
** FUNCTION: sip_setSUrlInAddrSpec
** DESCRIPTION: This function sets the sips url in a SipAddrSpec
** PARAMETERS:
**				pAddrspec(IN)	- Pointer to the SipAddrSpec
**				pUrl(IN)		- The SIPS URL to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_setSUrlInAddrSpec _ARGS_ ((SipAddrSpec *pAddrspec,
	SipUrl *pUrl, SipError *pErr));


/***************************************************************
** FUNCTION: sip_getUserFromUrl
**
** DESCRIPTION: this function retrieves the pUser from a SIP Url
**
***************************************************************/

extern SipBool sip_getUserFromUrl _ARGS_ (( SipUrl *pUrl, SIP_S8bit **ppUser, \
	SipError *pErr));

/***************************************************************
** FUNCTION: sip_setUserInUrl
**
** DESCRIPTION: this function sets the pUser in a SIP Url
**
***************************************************************/

extern SipBool sip_setUserInUrl _ARGS_ ((SipUrl *pUrl, SIP_S8bit *pUser, \
	SipError *pErr));


/***************************************************************
** FUNCTION: sip_getPasswordFromUrl
**
** DESCRIPTION: This function retrieves the passworxd field from
**		a SIP Url
**
***************************************************************/

extern SipBool sip_getPasswordFromUrl _ARGS_ ((SipUrl *pUrl, \
	SIP_S8bit **ppPassword, SipError *pErr));


/***************************************************************
** FUNCTION: sip_setPasswordInUrl
**
** DESCRIPTION: This function sets the pPassword field in a SIP Url
**
***************************************************************/

extern SipBool sip_setPasswordInUrl _ARGS_ ((SipUrl *pUrl, \
	SIP_S8bit *pPassword, SipError *pErr));

/***************************************************************
** FUNCTION: sip_getHostFromUrl
**
** DESCRIPTION: This function retrieves the pHost field from a SIP
**		Url
**
***************************************************************/

extern SipBool sip_getHostFromUrl _ARGS_ ((SipUrl *pUrl, SIP_S8bit **ppHost,\
	SipError *pErr));

/***************************************************************
** FUNCTION: sip_setHostInUrl
**
** DESCRIPTION: this function sets the pHost field in a SIP Url
**
***************************************************************/

extern SipBool sip_setHostInUrl _ARGS_ ((SipUrl *pUrl, SIP_S8bit *pHost, \
	SipError *pErr));

/***************************************************************
** FUNCTION: sip_getHeaderFromUrl
**
** DESCRIPTION: This function retrieves the pHeader field from a
**		SIP Url
**
***************************************************************/

extern SipBool sip_getHeaderFromUrl _ARGS_ ((SipUrl *pUrl, \
	SIP_S8bit **ppHeader, SipError *pErr));

/***************************************************************
** FUNCTION: sip_setHeaderInUrl
**
** DESCRIPTION: This function sets the pHeader field in a SIP Url
**
***************************************************************/

extern SipBool sip_setHeaderInUrl _ARGS_ ((SipUrl *pUrl, SIP_S8bit *pHeader,\
	SipError *pErr));

/***************************************************************
** FUNCTION: sip_getPortFromUrl
**
** DESCRIPTION: This function retrieves the dPort field from a SIp
**		Url
**
***************************************************************/

extern SipBool sip_getPortFromUrl _ARGS_ ((SipUrl *pUrl, SIP_U16bit *pPort, \
	SipError *pErr));

/***************************************************************
** FUNCTION: sip_setPortInUrl
**
** DESCRIPTION: This function sets the dPort field in a SIP Url
**
***************************************************************/

extern SipBool sip_setPortInUrl _ARGS_ ((SipUrl *pUrl, SIP_U16bit dPort, \
	SipError *pErr));

/***************************************************************
** FUNCTION: sip_getUrlParamCountFromUrl
**
** DESCRIPTION: This function retrieves the number of url-pParam
**		from a SIP Url
**
***************************************************************/

extern SipBool sip_getUrlParamCountFromUrl _ARGS_ ((SipUrl	*pUrl,\
	  SIP_U32bit	*pIndex, SipError *pErr));

/***************************************************************
** FUNCTION: sip_getUrlParamAtIndexFromUrl
**
** DESCRIPTION: This function retrieves a Url-param at a specified
**		index from a Sip Url
**
***************************************************************/

extern SipBool sip_getUrlParamAtIndexFromUrl _ARGS_ ((SipUrl *pUrl, \
	  SipParam **ppParam, SIP_U32bit   dIndex, SipError *pErr));
	  
/***************************************************************
** FUNCTION: sip_setUrlParamAtIndexInUrl
**
** DESCRIPTION: This function sets aurl-param at a specified index
**		in a Sip Url
**
***************************************************************/

extern SipBool sip_setUrlParamAtIndexInUrl _ARGS_ ((SipUrl 	*pUrl, \
	  SipParam	*pParam, SIP_U32bit 	dIndex, SipError *pErr));


/***************************************************************
** FUNCTION: sip_insertUrlParamAtIndexInUrl
**
** DESCRIPTION: This function inserts a url-param at a specified
**		index in a Sip Url
**
***************************************************************/

extern SipBool sip_insertUrlParamAtIndexInUrl _ARGS_ ((SipUrl 	*pUrl,\
	  SipParam	*pParam, SIP_U32bit dIndex, SipError *pErr));

/***************************************************************
** FUNCTION: sip_deleteUrlParamAtIndexInUrl
**
** DESCRIPTION: This function deletes a url-param at a specified
**		index in a Sip Url
**
***************************************************************/

extern SipBool sip_deleteUrlParamAtIndexInUrl _ARGS_ ((SipUrl *pUrl,\
	  SIP_U32bit dIndex, SipError *pErr));

#ifdef SIP_DATE
/***************************************************************
** FUNCTION: sip_getYearFromDateFormat
**
** DESCRIPTION: This function retrieves the dYear from a SIP dDate
**		structure
**
***************************************************************/

extern SipBool sip_getYearFromDateFormat _ARGS_ ((SipDateFormat *pHdr,\
	SIP_U16bit *pYear, SipError *pErr));

/***************************************************************
** FUNCTION: sip_setYearInDateFormat
**
** DESCRIPTION: This function sets the dYear field in a SIP Date
**		structure
**
***************************************************************/

extern SipBool sip_setYearInDateFormat _ARGS_ ((SipDateFormat *pHdr,\
	SIP_U16bit dYear, SipError *pErr));

/***************************************************************
** FUNCTION: sip_setSecInTimeFormat
**
** DESCRIPTION: This function sets the seconds in a SIP Time structure
**
***************************************************************/

extern SipBool sip_setSecInTimeFormat _ARGS_ ((SipTimeFormat *pHdr,\
	SIP_S8bit dSec,	SipError *pErr));
/*****************************************************************
**
** FUNCTION:  sip_setTimeFormatInDateStruct
**
** DESCRIPTION: This function sets the slTime in a SIP Date structure
**
***************************************************************/
extern SipBool sip_setTimeFormatInDateStruct _ARGS_ ((SipDateStruct *dstruct, \
	SipTimeFormat *slTime, SipError *err));

/*********************************************************
** FUNCTION:validateMonth
**
** DESCRIPTION:  This fucntion returns SipSuccess if
** "htype" is one among the defined en_Month types else it
** returns SipFail.
**
**********************************************************/
extern SipBool validateMonth _ARGS_ ((en_Month *htype, \
	SipError *err));

/********************************************************************
**
** FUNCTION:  validateSipDayOfWeek
**
** DESCRIPTION:   This fucntion returns SipSuccess if "dDay" is
** one among the defined en_DayOfWeek types; else it returns SipFail.
**
********************************************************************/

extern SipBool validateSipDayOfWeek _ARGS_ ((en_DayOfWeek dDay, SipError *err));

/********************************************************************
**
** FUNCTION:  validateSipTimeFormat
**
**DESCRIPTION:  This function  validates the pValue of "time_format".
** It returns SipSuccess if dHour is less than 24, minute is less
** than 59 and second is lesss than 59; else it returns SipFail;
**
********************************************************************/

extern SipBool validateSipTimeFormat _ARGS_ ((SipTimeFormat *time_format, \
	SipError *err));

/********************************************************************
**
** FUNCTION:  validateSipDateFormat
**
** DESCRIPTION:  This function validates the values of the
** "date_format" structure fileds.
**
********************************************************************/

extern SipBool validateSipDateFormat _ARGS_ ((SipDateFormat *date_format, \
	SipError *err));

/********************************************************************
**
** FUNCTION:  validateSipDateStruct
**
** DESCRIPTION:  This function validates the field values of 'date_struct"
** structure.
**
********************************************************************/

extern SipBool validateSipDateStruct _ARGS_ ((SipDateStruct *date_struct, \
	SipError *err));

#endif /* SIP_DATE */

#ifdef SIP_CORE_STACK_COMPATIBLE
/*#ifdef STK_RELEASE*/
/********************************************************************
** FUNCTION:sip_getNameFromNameValuePair
**
** DESCRIPTION: This Function gets the Name from NamevaluePair
**
**
**********************************************************************/
extern SipBool sip_getNameFromNameValuePair _ARGS_ 
	(( SipNameValuePair *pNameValue, SIP_S8bit **pName, SipError *err));

/*#endif*/
/********************************************************************
** FUNCTION:sip_setNameInNameValuePair
**
** DESCRIPTION: This Function sets the Name in NamevaluePair
**
**
**********************************************************************/

extern SipBool sip_setNameInNameValuePair _ARGS_ 
	(( SipNameValuePair *pNameValue, SIP_S8bit *pName, SipError *err));

/********************************************************************
** FUNCTION:sip_getValueFromNameValuePair
**
** DESCRIPTION: This Function gets the value from NamevaluePair
**
**
**********************************************************************/
extern SipBool sip_getValueFromNameValuePair _ARGS_
	( (SipNameValuePair *pNameValue, SIP_S8bit **pValue, SipError *err));

/********************************************************************
** FUNCTION:sip_setValueInNameValuePair
**
** DESCRIPTION: This Function sets the value in NamevaluePair
**
**
**********************************************************************/

extern SipBool sip_setValueInNameValuePair _ARGS_
	((SipNameValuePair *pNameValue, SIP_S8bit *pValue, SipError *err));

#endif /* SIP_CORE_STACK_COMPATIBLE */

/********************************************************************
**
** FUNCTION:  validateSipHeaderType
**
** DESCRIPTION:  This fucntion returns SipSuccess if "dType" is one
** among the defined en_HeaderType's else it returns SipFail.
**
********************************************************************/

extern SipBool validateSipHeaderType _ARGS_ ((en_HeaderType dType, SipError *err));

/******************************************************************
**
** FUNCTION:  sip_getTypeFromString
**
** DESCRIPTION: This function retrieves the pHeader dType from a pHeader
**		represented as string
**
******************************************************************/
extern SipBool sip_getTypeFromString _ARGS_ (( SIP_S8bit *str1,\
	   en_HeaderType *dType, SipError *err));


/********************************************************************
**
** FUNCTION:  sip_getHeaderLineCount
**
** DESCRIPTION: This function returns the number of distinct pHeader 
**				lines in the SipMessage "msg".
**
*********************************************************************/

extern SipBool sip_getHeaderLineCount _ARGS_ ((SipMessage *msg, \
	SIP_U32bit *count, SipError *err));

/********************************************************************
**
** FUNCTION:  sip_validateHeaderString
**
** DESCRIPTION: This function returns SipSuccess if the
** string "str1" is having a correct pFormat for any of the headers.
**
********************************************************************/

extern SipBool sip_validateHeaderString _ARGS_ (( SIP_S8bit *str1,\
	   en_HeaderType dType, SipError *err));


#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
        }
#endif
