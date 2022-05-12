/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
** FUNCTION:
**          Header File for Tel Url Parsing facility
************************************************************************
**
** FILENAME:
** microsip_telparser.h
**
** DESCRIPTION:
**		This file contains functions for parsing tel: URL
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 23/08/2002	Mahesh Govind		-			Initial					
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/
#ifndef __MICROSIPTELPARSER_H__
#define __MICROSIPTELPARSER_H__

#include "microsip_telstruct.h"
#include "microsip_struct.h"
#include "microsip_common.h"
#include "microsip_encodeinternal.h"

/***********************************************************************
** FUNCTION: sip_parseTelUrl
** DESCRIPTION: This function does the parsing of telurl 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**              pEnd(IN)        - The end point of the input buffer
**				TelUrl(IN/Out)	- TelUrl structure returned
** NOTE: 
************************************************************************/

SipBool sip_parseTelUrl _ARGS_ ((SIP_S8bit	*pStart,	\
	TelUrl	*pTelUrl,SipError	*pErr));
/***********************************************************************
** FUNCTION: sip_validatePhoneNumber
**
** DESCRIPTION: This function does the validation of a phone number 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- Number of characters .The end point of 
**								- input buffer
** NOTE: 
************************************************************************/

SipBool sip_validatePhoneNumber _ARGS_((SIP_S8bit *pStart , SIP_S8bit *pEnd ,\
					SipError *pErr));
/**********************************************************************
** FUNCTION: sip_getTelUrlFromAddrSpec
**
** DESCRIPTION: gets the TelUrl from the SipAddrSpec structure
** PARAMETERS:
**		pAddrSpec (IN)	- SipAddrSpec
**		pTelUrl (OUT)	- retrieved TelUrl
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getTelUrlFromAddrSpec _ARGS_((
	SipAddrSpec *pAddrSpec, TelUrl **ppTelUrl, SipError *pErr));
/***********************************************************************
** FUNCTION: sip_isTelUrl
**
** DESCRIPTION: Checks if the Addrspec has a tel-url
**
** PARAMETERS:
**		pAddrSpec (IN)	- SipAddrSpec
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_isTelUrl _ARGS_((SipAddrSpec *pAddrSpec, SipError *pErr));
/***********************************************************************
** Function: sip_setTelUrlInAddrSpec
** Description: sets the TelUrl in the SipAddrSpec structure
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec
**		pTelUrl (OUT)	- TelUrl to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setTelUrlInAddrSpec _ARGS_((SipAddrSpec *pAddrSpec, TelUrl *pTelUrl,\
	SipError *pErr));
/*****************************************************************
** FUNCTION: sip_formTelGlobalNum
** DESCRIPTION: Converts TelGlobalNum to text
** Parameters:
** 		pGlobal(IN)    - The TelGlobalNum to be used
**		out(OUT)       - The output buffer
**		pErr (OUT)		- Possible Error value (see API ref doc)
*****************************************************************/

SipBool sip_formTelGlobalNum(TelGlobalNum *pGlobal,SIP_S8bit* out,SipError *pErr);

/*****************************************************************
** FUNCTION: sip_formTelLocalNum
** DESCRIPTION: Converts a list of TelLocalNum to text
** Parameters:
** 		pLocal(IN)    - The TelLocalNum to be used
**		out(OUT)       - The output buffer
**		pErr (OUT)		- Possible Error value (see API ref doc)
*****************************************************************/

SipBool sip_formTelLocalNum _ARGS_ ((TelLocalNum *pLocal,SIP_S8bit* out,SipError *pErr));
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

SipBool sip_formSipStringList _ARGS_ ((SIP_S8bit 	*out, SipList 	*list,SIP_S8bit 	\
		*separator,	SIP_U8bit	leadingsep,	SipError 	*pErr));

/*****************************************************************
** FUNCTION: sip_formEscapedSipParamList
** DESCRIPTION: Converts a list of Param to text with escaping certain
**                  charaters
** Parameters:
** 		out(OUT)    - output buffer
**		list(IN)       -  the param list to be converted
** 		seperator(IN)	- each element to be sepearated by
**		leadingsep(IN)	- leading seperator
**		pErr (OUT)		- Possible Error value (see API ref doc)
**
** DESCRIPTION: Converts a list of SipParams to text
*****************************************************************/

SipBool sip_formEscapedSipParamList _ARGS_ ((SIP_S8bit 	*out, SipList 	*list,	SIP_S8bit \
	*separator,	SIP_U8bit	leadingsep,	SipError 	*err));

/* Ensure Names are not mangled by C++ compilers */
#endif
#ifdef __cplusplus
}
#endif 

