/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/***********************************************************************
** FUNCTION:
**			Prototyoes for Startline Accessor APIs
************************************************************************
**
** FILENAME:
** microsip_startline.h
**
** DESCRIPTION:
**		This file contains the prototypes for the accessor functions 
**		for manipulating the startline of a SIP message.
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 9/09/2002	Aparna Kuppachi		-			        Initial					
** 28/01/2009   Kamal Ashraf                     IPTK Rel 8.1           Protype Added for API
**                                                                      sip_setReqLineFromString
**	COPYRIGHT , Aricent, 2009
***********************************************************************/

#ifndef __MICROSIP_STARTLINE_H__
#define __MICROSIP_STARTLINE_H__

#include "microsip_struct.h"
#include "microsip_common.h"
#include "microsip_portlayer.h"

/****************************************************************************
** FUNCTION: sip_getStatusLineFromSipRespMsg
** DESCRIPTION: This function returns the status line from a SIP response message
** PARAMETERS:
**				pMsg(IN)		- Pointer to the SipMessage 
**				ppStatusLine(OUT)	- The retrieved status line
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_getStatusLineFromSipRespMsg
	(SipMessage *pMsg, SipStatusLine **ppStatusLine, SipError *pErr);


/****************************************************************************
** FUNCTION: sip_setStatusLineInSipRespMsg
** DESCRIPTION: This function sets the status line in a SIP response message
** PARAMETERS:
**				pMsg(IN)		- Pointer to the SipMessage
**				pLine(IN)		- The status line to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_setStatusLineInSipRespMsg
	(SipMessage *pMsg, SipStatusLine *pLine, SipError *pErr);


/****************************************************************************
** FUNCTION: sip_getReqLineFromSipReqMsg
** DESCRIPTION: This function retrieves the request line from a SIP request
**				message.
** PARAMETERS:
**				pMsg(IN)		- Pointer to the SipMessage
**				ppLine(OUT)		- The retrieved request line
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_getReqLineFromSipReqMsg
	(SipMessage *pMsg, SipReqLine **ppLine, SipError *pErr);


/****************************************************************************
** FUNCTION: sip_setReqLineInSipReqMsg
** DESCRIPTION: This function sets the Request Line in a SIP request message
** PARAMETERS:
**				pMsg(IN)		- Pointer to the SipMessage
**				pLine(IN)		- The request line to be set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_setReqLineInSipReqMsg
	(SipMessage *pMsg, SipReqLine *pLine, SipError *pErr);

/*****************************************************************
**
** FUNCTION:  sip_getAddrSpecFromReqLine
**
** DESCRIPTION: This function retrieves the dAddr-spec field from a
**		SIP Request line
**
***************************************************************/
extern SipBool sip_getAddrSpecFromReqLine(SipReqLine *line, SipAddrSpec \
	**ppRequestUri, SipError *err);

/*****************************************************************
**
** FUNCTION:  sip_setAddrSpecInReqLine
**
** DESCRIPTION: This function sets the dAddr-spec field in a SIP
**		Request line
**
***************************************************************/
extern SipBool sip_setAddrSpecInReqLine
	(SipReqLine *line, SipAddrSpec *pRequestUri, SipError *err);

/***********************************************************************
** Function: sip_setMethodInReqLine
** Description:sets the Method in the Sip Status Line
** Parameters:
**			line(IN/OUT) 		- Sip Status Line
**			method(IN)			- The method to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setMethodInReqLine _ARGS_ ((SipReqLine *line, \
		SIP_S8bit *method, SipError *err));


extern SipBool sip_getMethodFromReqLine
	(SipReqLine *line, SIP_S8bit **pMethod, SipError *err);

/***********************************************************************
** Function: sip_getVersionFromReqLine
** Description:gets the Version fromthe Sip Request Line
** Parameters:
**			line(IN) 			- Sip RequestLine
**			version(OUT)			- Version to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_getVersionFromReqLine
	(SipReqLine *line, SIP_S8bit **pVersion, SipError *err);

/***********************************************************************
** Function: sip_setVersionInReqLine
** Description:gets the Version in the Sip Request Line
** Parameters:
**			line(IN/OUT) 			- Sip RequestLine
**			version(IN)			- Version to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setVersionInReqLine
	(SipReqLine *line, SIP_S8bit *pVersion, SipError *err);

/***********************************************************************
** Function: sip_getVersionFromStatusLine
** Description:gets the Version field from the Sip Status Line
** Parameters:
**			line(IN) 			- Sip Status Line
**			version(OUT)		- The version to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_getVersionFromStatusLine
	(SipStatusLine *line, SIP_S8bit **pVersion, SipError *err);

/***********************************************************************
** Function: sip_setVersionInStatusLine
** Description:sets the Version field in the Sip Status Line
** Parameters:
**			line(IN/OUT) 			- Sip Status Line
**			version(IN)			- The version to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setVersionInStatusLine
	(SipStatusLine *line, SIP_S8bit *pVersion, SipError *err);

/***********************************************************************
** Function: sip_getStatusCodeNumFromStatusLine
** Description:gets the Status code number from the Sip Status Line
** Parameters:
**			line(IN) 			- Sip Status Line
**			code_num(OUT)		- The Status code number to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
/* CodeNum Changed to U32bit */ 
extern SipBool sip_getStatusCodeNumFromStatusLine
	(SipStatusLine *line, SIP_U32bit *dCodeNum, SipError *err);

/***********************************************************************
** Function: sip_setStatusCodeNumInStatusLine
** Description:sets the Status code number in the Sip Status Line
** Parameters:
**			line(IN/OUT) 			- Sip Status Line
**			code_num(IN)		- The Status code number to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setStatusCodeNumInStatusLine
	(SipStatusLine *line, SIP_U32bit dCodeNum, SipError *err);

/***********************************************************************
** Function: sip_getReasonFromStatusLine
** Description:gets the Reason in the Sip Status Line
** Parameters:
**			line(IN) 			- Sip Status Line
**			reason(OUT)			- The reason to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_getReasonFromStatusLine
	(SipStatusLine *line, SIP_S8bit **pReason, SipError *err);

/***********************************************************************
** Function: sip_setReasonInStatusLine
** Description:sets the Reason in the Sip Status Line
** Parameters:
**			line(IN/OUT) 			- Sip Status Line
**			reason(IN)			- The reason to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setReasonInStatusLine
	(SipStatusLine *line, SIP_S8bit *pReason, SipError *err);


#endif /* __MICROSIP_STARTLINE_H__ */

/*ProtoType for the API added for IPTK Rel 8.1(UATK Enhancement)*/

/******************************************************************************
** Function: sip_setReqLineFromString
** Description:  sets request line as string in sip message
** Parameters:
** pSipMsg (IN/OUT)		- Sip Message
** line (IN)			- request line as string
** pErr (OUT)			- possible error value 
********************************************************************************/
extern SipBool sip_setReqLineFromString _ARGS_((SipMessage *pSipMsg,\
     SIP_S8bit *line, SipError *pErr));


/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
	}
#endif
