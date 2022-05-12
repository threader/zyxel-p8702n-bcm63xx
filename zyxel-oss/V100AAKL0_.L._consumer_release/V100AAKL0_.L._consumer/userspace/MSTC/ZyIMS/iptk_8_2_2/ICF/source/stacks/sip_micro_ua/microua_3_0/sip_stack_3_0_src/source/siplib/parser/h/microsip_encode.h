/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 ** FUNCTION: 	This file contains the prototypes of the message formation APIs	
 **
 ******************************************************************************
 **
 ** FILENAME: 		microsip_encode.h
 **
 ** DESCRIPTION:	This file contains the prototypes of the message formation
 **					APIs.
 **
 **
 ** 	DATE	 	NAME			REFERENCE	 		REASON
 ** 	----	 	----			---------	 		--------
 ** 27-Feb-2002		Siddharth		sipformmesage.h		Initial
 **					Toshniwal		of core SIP Stack
 ** 28-Jul-2009     Tarun Gupta     SPR 19886           Merged CSR 1-7233780
 **
 ******************************************************************************
 **				COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#ifndef __MUSIP_ENCODE_H__
#define __MUSIP_ENCODE_H__

/*****************************************************************
** Function: sip_formMessage
** Description: Forms a text message from a message structure.
** Parameters:
**		s(IN): The message structure that is to be converted to
**			text.
**		options(IN): The options to be used for forming the text
**			message. Multiple options can be ORed. The supported 
**			options are:
**			SIP_OPT_SINGLE: Each header is formed in a separate line.
**				The message will not contain comma separated headers.	
**				Cannot be used with SIP_OPT_COMMASEPARATED.
**			SIP_OPT_COMMASEPARATED: Multiple instances of the same
**				kind of header will appear on the same header line
**				separated by commas.
**				Cannot be used with SIP_OPT_SINGLE.
**			SIP_OPT_SHORTFORM: Single-letter short forms will be used
**				for headers	that have short names.
**				Cannot be used with SIP_OPT_SULLFORM.
**			SIP_OPT_FULLFORM: All hedernames will appear in full.
**				Cannot be used with SIP_OPT_SHORTFORM.
**			SIP_OPT_CLEN: A Content-Length header with the correct
**				length is inserted in the message being formed. If
**				the message structure has a Content-Length header,
**				the function corrects the length field depending on
**				size of the message body.
**			SIP_OPT_REORDERHOP: All hop to hop headers will be placed
**				above the end to end headers.
**			SIP_OPT_AUTHCANONICAL: All headers after the Authorization
**				header will be formed in the canonical form. This
**				will override options SIP_OPT_SHORTFORM and
**				SIP_OPT_COMMASEPARATED from the Authorization header
**				onwards.
**		out(OUT): A preallocated buffer that will contain the formed
**			text message.
**		dLength(OUT): This will contain the length of the message formed.
**		err(OUT): The error code is returned in this of the function 
**			fails.
**
*******************************************************************/
extern SipBool sip_formMessage _ARGS_ ((SipMessage *s, \
	SipOptions *options, SIP_S8bit *out, \
		SIP_U32bit *dLength, SipError *err ));


/*****************************************************************
** Internal functions not used by the application directly
*******************************************************************/
  /* CSR_1-6949679 Changes Starts */
extern SipBool sip_formEachHeader _ARGS_ ((/* CSR_1-6613911 Change Start */ 
   SIP_S8bit	*pEndBuff, /* CSR_1-6613911 Change End */SIP_U32bit cpiter,\
	SipOptions *opt,SipMessage *s, \
		SIP_S8bit *out,SIP_U32bit siplistmap[],SipError *err));

  /* CSR_1-6949679 Changes Ends */ 
  /* CSR_1-6949679 Changes Starts */ 
extern SipBool sip_formSingleHeader _ARGS_ ((/* CSR_1-6613911 Change Start */ SIP_S8bit	*pEndBuff,/* CSR_1-6613911 Change End */\
en_HeaderType hdrtype,SipOptions *popt,\
	SIP_U32bit ndx,en_AdditionMode mode,\
		SipMessage *s, SIP_U16bit insertCRLF, SIP_S8bit *out,SipError *err));
  /* CSR_1-6949679 Changes Ends */

extern SipBool sip_formMimeBody _ARGS_ ((SIP_S8bit *pDummyBuf, \
	SipList mbodyList, SipContentTypeHeader *ctypehdr,SIP_S8bit *out, \
		SIP_U32bit *length,SipError *err ));

extern SipBool sip_formSdpBody _ARGS_((/* CSR_1-6613911 Change Start */ 
   SIP_S8bit	*pEndBuff, /* CSR_1-6613911 Change End */SdpMessage *s, SIP_S8bit *out, \
	SipError *err));

extern SipBool sip_formSingleGeneralHeader _ARGS_((en_HeaderType dType, \
	SIP_U32bit ndx, en_AdditionMode mode, en_HeaderForm form, \
	SipGeneralHeader *g, SIP_S8bit *out, SipError *err));

extern SipBool sip_formSingleResponseHeader _ARGS_(( en_HeaderType dType, \
	SIP_U32bit ndx, en_AdditionMode mode, en_HeaderForm form, \
	SipRespHeader *s, SIP_S8bit *out, SipError *err));

extern SipBool sip_formSingleRequestHeader _ARGS_(( en_HeaderType dType, \
	SIP_U32bit ndx, en_AdditionMode mode, en_HeaderForm form, \
	SipReqHeader *s, SIP_S8bit *out, SipError *err));

extern SipBool sip_formSingleRequest _ARGS_((en_HeaderType dType, \
	SIP_U32bit ndx, en_AdditionMode mode, en_HeaderForm form, SipMessage *s, \
	SIP_S8bit *out, SipError *err));

extern SipBool sip_formSingleResponse _ARGS_((en_HeaderType dType, \
	SIP_U32bit ndx, en_AdditionMode mode, en_HeaderForm form, SipMessage *s, \
	SIP_S8bit *out, SipError *err));

#ifdef SIP_MWI
extern SipBool sip_formMesgSummaryBody _ARGS_((/* CSR_1-6613911 Change Start */SIP_S8bit	*pEndBuff,/* CSR_1-6613911 Change End */ \
            MesgSummaryMessage *pMessageSummaryMsg, \
            SIP_S8bit *pOut, SipError *pError));
#endif

/* Fix for IPTK CSR 1-7233780 */
extern SipBool sip_formSingleIPTKHeader _ARGS_ ((SIP_S8bit	*pEndBuff, en_HeaderType hdrtype,\
	SIP_U32bit ndx,en_AdditionMode mode,\
		SipMessage *s, SIP_U16bit insertCRLF, SIP_S8bit *out,SipError *err));


#endif	/* Prevent multiple inclusions */

#ifdef __cplusplus
}  /* Ensure Names are not mangled by C++ compilers */
#endif
