/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 ** FUNCTION: 		Prototypes for Header level encoding APIs
 **
 *****************************************************************************
 **
 ** FILENAME: 		microsip_hdrencoders.h
 **
 ** DESCRIPTION:	
 **
 **
 ** 	DATE	 	NAME			REFERENCE	 		REASON
 ** 	----	 	----			---------	 		--------
 ** 27-Feb-2002		Siddharth		sipformmesage.h		Initial
 **					Toshniwal		of SIP Stack
 **
 ****************************************************************************
 **				COPYRIGHT , Aricent, 2006
 ****************************************************************************/

#ifndef __MUSIP_HDRENCODERS_H__
#define __MUSIP_HDRENCODERS_H__

#include "microsip_common.h"
#include "microsip_struct.h"


/*****************************************************************
** Function: sip_formAddrSpec
** Description: Forms an address specification text from an AddrSpec
**		structure.
** Parameters:
**		pDummyBuf(IN): For future use
**		out(OUT): A preallocated buffer that will contain the 
**			Addr-Spec string.
**		aspec(IN): The AddrSpec structure to be converted to string.
**		err(OUT): Contains the error code if the function fails.
**
*******************************************************************/
extern SipBool sip_formAddrSpec _ARGS_ ((SIP_S8bit *pDummyBuf, \
	SIP_S8bit 	*out, SipAddrSpec 	*aspec, SipError 	*err));



/*
 * Internal functions used by the stack for header encoding 
 */

extern SipBool sip_formSipParamList _ARGS_((SIP_S8bit 	*out, SipList *list, \
	SIP_S8bit *separator,SIP_U8bit leadingsep,SipError *err));

extern SipBool sip_formFromFamily _ARGS_((SIP_S8bit *pOut, SipMessage *pSipMsg,\
	SIP_U32bit dIndex, en_AdditionMode dMode, en_HeaderForm dForm, \
	en_HeaderType dType, SipError *pError));

extern SipBool sip_formStringFamily _ARGS_((SIP_S8bit *pOut, \
	SipMessage *pSipMsg, SIP_U32bit dIndex, en_AdditionMode dMode, \
	en_HeaderForm dForm, en_HeaderType dType, SipError *pError));

extern SipBool sip_formSdpConnection _ARGS_((SIP_S8bit *pOut, \
	SdpConnection *pSdpConnection, SipError *pError));
	
extern SipBool sip_formListOfAttributes _ARGS_((SIP_S8bit *pOut, \
	SipList *pslAttribList, SipError *pError));

extern SipBool sip_formListOfBandwidthVal _ARGS_((SIP_S8bit *pOut, \
	SipList *pslBandwidth, SipError *pError));
	
extern SipBool sip_formAuthFamily _ARGS_((SIP_S8bit *pOut, SipMessage *pSipMsg,\
	 SIP_U32bit dIndex, en_AdditionMode dMode, en_HeaderForm dForm, \
	en_HeaderType dType, SipError *pError));

#ifdef SIP_3GPP
extern SipBool sip_form3GPPHeader _ARGS_((SIP_S8bit *pOut, SipMessage *pSipMsg,\
	SIP_U32bit dIndex, en_AdditionMode dMode, en_HeaderForm dForm, \
	en_HeaderType dType, SipError *pError));

#endif
#ifdef SIP_DATE
extern SipBool sip_formDateStruct (SIP_S8bit **out, SipDateStruct *dt,\
	SipError *err);
#endif /* SIP_DATE */

#endif	/* Prevent multiple inclusions */

#ifdef __cplusplus
}  /* Ensure Names are not mangled by C++ compilers */
#endif
