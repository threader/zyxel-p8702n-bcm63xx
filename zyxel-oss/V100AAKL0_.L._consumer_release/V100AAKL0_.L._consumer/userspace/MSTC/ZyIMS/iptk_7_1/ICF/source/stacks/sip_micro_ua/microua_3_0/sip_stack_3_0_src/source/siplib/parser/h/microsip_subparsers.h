/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 ** FUNCTION:
 **             Functions For subparsers

 *********************************************************************
 **
 ** FILENAME:
 ** microsip_subparsers.h
 **
 ** DESCRIPTION:
 ** This file contains function prototypes for subparser functions.
 **
 ** DATE        NAME                    REFERENCE       REASON
 ** ----        ----                    ---------       --------
 ** 15 02 2002  Mahesh Govind	           -		 Initial Creation
 ***
 ** COPYRIGHT , Aricent, 2006
 *********************************************************************/ 

#ifndef __MICROSIP_SUBPARSERS_H__
#define __MICROSIP_SUBPARSERS_H__

#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_free.h"
#include "microsip_init.h"


/***********************************************************************
** FUNCTION: sip_tokenize
** DESCRIPTION: This function tokenizes a given string 
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
extern SIP_S8bit  *sip_tokenize _ARGS_ (( SIP_S8bit *pStart ,SIP_S8bit *pEnd ,\
					SIP_S8bit dLimiter));

/***********************************************************************
** FUNCTION: sip_tokenize_sdp_attr
** DESCRIPTION: This function tokenizes a given string 
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
extern SIP_S8bit  *sip_tokenize_sdp_attr _ARGS_ (( SIP_S8bit *pStart ,SIP_S8bit *pEnd ,\
					SIP_S8bit dLimiter));


/****************************************************************************
** FUNCTION: sip_validateIPv6Addr
** DESCRIPTION: This function Checks whether the given address is
**				a valid IPV6 address or not    
** 
** PARAMETERS:
**				ppStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/
extern SipBool sip_validateIPv6Addr _ARGS_ ((SIP_S8bit *pStart, \
				SIP_S8bit *pEnd, SipError *pErr));
			
/****************************************************************************
** FUNCTION: sip_parseComment
** DESCRIPTION: This function parses and a "Sip" Comment   
** 
** PARAMETERS:
**				ppStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				Flag			- Flag telling whether name=value is a must
**								  headers filled. 
**				pErr(OUT		- Error value if any 
**
** NOTE: This function returns a pointer to a SipPAram structure
****************************************************************************/
extern SipBool sip_parseComment _ARGS_ ((SIP_S8bit *pStart,SIP_S8bit *pEnd, \
				 SIP_S8bit **ppOutComment, SipError *pErr));

/****************************************************************************
** FUNCTION: sip_parseEachpParam
** DESCRIPTION: This function parses and fills a SipParam structure   
** 
** PARAMETERS:
**				ppStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				Flag			- Flag telling whether name=value is a must
**								  headers filled. 
**				pErr(OUT		- Error value if any 
**
** NOTE: This function returns a pointer to a SipPAram structure
****************************************************************************/
extern SipParam * sip_parseEachParam _ARGS_ ((SIP_S8bit *pStart, \
					SIP_S8bit *pEnd, en_SipBoolean Flag, SipError *err));

/****************************************************************************
** FUNCTION: sip_parseSipParam
** DESCRIPTION: This function parses a SipParam and Returns a list of slParam  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				slParam(OUT)	- The sipList which is filled and returned
**				Flag			- Flag telling whether name=value is a must
**								  headers filled. 
**				dLimiter		- The dLimiter by which params are separated
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/
extern SipBool sip_parseSipParam _ARGS_ ((SipList *slParam, SIP_S8bit *pStart, \
	SIP_S8bit *pEnd, en_SipBoolean dFlag, SIP_S8bit dLimiter, \
	SipError *err));


/****************************************************************************
** FUNCTION: sip_parseHostPort
** DESCRIPTION: This function parses the Host and Port  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				ppHost(OUT)		- The Host address
**				ppPort(OUT)		- The port no
**								  headers filled. 
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_parseHostPort _ARGS_ ((SIP_S8bit *pStart, \
				SIP_S8bit *pEnd, SIP_S8bit **ppHost, \
				SIP_U16bit **ppPort, SipError *pErr));

 
/****************************************************************************
** FUNCTION: sip_parseUserAndHostPort
** DESCRIPTION: This function parses the username password host and port 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**				ppHost(OUT)		- The Host address
**				ppPort(OUT)		- The port no
**				ppUser(OUT)		- The user name
**				ppPassword(OUT)	- The password	
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_parseUserAndHostPort _ARGS_((SIP_S8bit *pStart , \
				SIP_S8bit *pEnd,SIP_S8bit **ppUser,SIP_S8bit **ppPassword, \
				SIP_S8bit **ppHost ,SIP_U16bit **ppPort,SipError * pErr));

/****************************************************************************
** FUNCTION: sip_parseSipUrl
** DESCRIPTION: This function parses the SipURL 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				ppOutUrl(OUT)	- The SipUrl structure with relevan values
**								  filled. 
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_parseSipUrl _ARGS_((SipUrl **ppOutUrl,SIP_S8bit *pStart, \
				SIP_S8bit *pEnd,SipError *pErr));

/****************************************************************************
** FUNCTION: sip_parseAddrSpec
** DESCRIPTION: This function parses the Addrspec  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				ppOutAddr(OUT)	- The SipAddrSpec structure with the relevant
**								  headers filled. 
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_parseAddrSpec _ARGS_((SIP_S8bit *pStart,\
		SIP_S8bit * pEnd, SipAddrSpec **ppOutAddr,SipError *pErr));

/***********************************************************************
** FUNCTION: sip_nStrdup
** DESCRIPTION: This function does strdup for n characters 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				n(IN)			- Number of characters
** NOTE: 
************************************************************************/

extern SIP_S8bit* sip_nStrdup _ARGS_ ((SIP_U32bit id,SIP_S8bit *pStart,SIP_S32bit n, SipError *pErr));

/****************************************************************************
** FUNCTION: sip_atoi
** DESCRIPTION: This function parses the Addrspec  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				
** NOTE: 
****************************************************************************/

extern SipBool	sip_atoi _ARGS_ ((SIP_S8bit *pStart, SIP_S8bit *pEnd, SIP_U32bit	*pNum));

/****************************************************************************
** FUNCTION: stripLeadingLWS
** DESCRIPTION: This function parses the Addrspec  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				
** NOTE: 
****************************************************************************/

SIP_S8bit *sip_stripLeadingLWS(SIP_S8bit *pStart, SIP_S8bit *pEnd);

/****************************************************************************
** FUNCTION: stripTrailingLWS
** DESCRIPTION: This function parses the Addrspec  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				
** NOTE: 
****************************************************************************/

SIP_S8bit *sip_stripTrailingLWS(SIP_S8bit *pStart, SIP_S8bit *pEnd);

#ifdef SIP_DATE
SipBool	sip_parseSipDate(SIP_S8bit *pStart,  SIP_S8bit *pEnd,  \
		SipDateStruct **ppDateStruct, SipError *pErr);
#endif /* SIP_DATE */

#endif /*Avoid Multiple inclusions*/

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
 
