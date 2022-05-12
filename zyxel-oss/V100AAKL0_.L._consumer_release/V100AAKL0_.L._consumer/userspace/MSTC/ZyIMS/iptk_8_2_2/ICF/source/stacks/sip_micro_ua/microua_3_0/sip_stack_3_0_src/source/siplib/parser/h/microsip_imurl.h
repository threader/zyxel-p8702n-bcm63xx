/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
** FUNCTION:
**			Header file for ImUrl
************************************************************************
**
** FILENAME:
** microsip_imurl.h
**
** DESCRIPTION:
**		This file contains functions for parsing SIP header elements.
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 15/08/2002	Mahesh Govind		-			Initial					
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/

#include "microsip_struct.h"
#include "microsip_common.h"
#include "microsip_free.h"
#include "microsip_imurlstruct.h"
#include "microsip_subparsers.h"

#ifndef __MICROSIP_IMURL_H_
#define __MICROSIP_IMURL_H_
	
#define MAX_IM_URL_SIZE 1024

/***********************************************************************
** FUNCTION: sip_parseImUrl
** DESCRIPTION: This function parses IM URL 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				pImurl(IN/OUT)	- The Im Url structure
**				pErr(IN)		- SipError
**			
** NOTE:Please note that this function will be called from related API 
************************************************************************/
SipBool sip_parseImUrl _ARGS_ ((SIP_S8bit *pStart , \
	ImUrl *pImurl ,	SipError *pErr));

#ifdef SIP_PRES
#define sip_parsePresUrl                   sip_parseImUrl
#endif

/***********************************************************************
** FUNCTION: sip_parseImRouteList
** DESCRIPTION: This function parses and stores a list of route in IM URL 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				dLimiter(IN)	- The dLimiter 
**
** NOTE: 
************************************************************************/

SipBool sip_parseImRoutelist _ARGS_((SIP_S8bit *pStart ,\
	SIP_S8bit *pEnd , SipList *pList ,SipError *pErr));

#ifdef SIP_PRES
#define sip_parsePresRoutelist             sip_parseImRoutelist
#endif

/***********************************************************************
** FUNCTION: sip_parseImAddr
** DESCRIPTION: This function parses and stores  IM URL addrspec
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				pUser			- The User to be returned by the parser
**				pHost			- The host to be returned by the parser
**
** NOTE: 
************************************************************************/
SipBool sip_parseImAddr _ARGS_ ((SIP_S8bit *pStart ,SIP_S8bit *pEnd ,\
	SIP_S8bit **pUser ,SIP_S8bit **pHost ,SipError *pErr));
#ifdef SIP_PRES
#define sip_parsePresAddr                  sip_parseImAddr
#endif


/***********************************************************************
** FUNCTION: sip_getImUrlFromAddrSpec
**
** DESCRIPTION: gets the ImUrl from the SipAddrSpec structure
**
** PARAMETERS:
**		pAddrSpec (IN)	- SipAddrSpec
**		pImUrl (OUT)	- retrieved ImUrl
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getImUrlFromAddrSpec _ARGS_((\
	SipAddrSpec *pAddrSpec, ImUrl **ppImUrl, SipError *pErr));

#ifdef SIP_PRES
SipBool sip_getPresUrlFromAddrSpec _ARGS_((\
	SipAddrSpec *pAddrSpec, ImUrl **ppImUrl, SipError *pErr));
#endif


/***********************************************************************
** Function: sip_setImUrlInAddrSpec
** Description: sets the ImUrl in the SipAddrSpec structure
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec
**		pImUrl (OUT)	- ImUrl to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setImUrlInAddrSpec _ARGS_ ((SipAddrSpec *pAddrSpec, \
	ImUrl *pImUrl, SipError *pErr));

#ifdef SIP_PRES
SipBool sip_setPresUrlInAddrSpec _ARGS_ ((SipAddrSpec *pAddrSpec, \
	ImUrl *pImUrl, SipError *pErr));
#endif


/***********************************************************************
** Function: sip_isImUrl
** Description: Checks if the Addrspec has a im-url
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_isImUrl _ARGS_ ((SipAddrSpec *pAddrSpec, SipError *pErr));

#ifdef SIP_PRES
SipBool sip_isPresUrl _ARGS_ ((SipAddrSpec *pAddrSpec, SipError *pErr));
#endif

/*****************************************************************
** FUNCTION: sip_formSipStringList
** DESCRIPTION: Converts a list of String to text
** Parameters:
** 		out(OUT)    - output buffer
**		list(IN)       -  the string list to be converted
** 		seperator(IN)	- each element to be sepearated by
**		leadingsep(IN)	- leading seperator
**		pErr (OUT)		- Possible Error value (see API ref doc)
*****************************************************************/

SipBool sip_formSipStringList _ARGS_ ((SIP_S8bit 	*out, SipList 	*list,	SIP_S8bit 	\
	*separator,	SIP_U8bit	leadingsep,	SipError 	*pErr));

/*****************************************************************
** FUNCTION: sip_formSipList
** DESCRIPTION: Converts a list of String to text
** Parameters:
** 		pOut(OUT)    - output buffer
**		pList(IN)       -  the string list to be converted
** 		pSeperator(IN)	- each element to be sepearated by
**		dLeadingsep(IN)	- leading seperator
**		pErr (OUT)		- Possible Error value (see API ref doc)
*****************************************************************/

SipBool sip_formSipList _ARGS_ ((SIP_S8bit 	*pOut, SipList 	*pList, SIP_S8bit\
	*pSeparator,SIP_U8bit	dLeadingsep,SipError 	*pErr));

#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
