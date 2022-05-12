/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 


/**************************************************************************
** FUNCTION:
**  This file contains the INTERNAL prototypes of parser clone functions
**
***************************************************************************
**
** FILENAME:
**  microsip_clone.h
**
** DESCRIPTION
**
**
**  DATE            NAME             REFERENCE			REASON
**	----			----			 ---------			------
** march 18 2002   Mahesh			Core Stack			Initial
**
**
** COPYRIGHT , Aricent, 2006
***************************************************************************/

#ifndef __MICROSIP_PARSER_CLONE_H_
#define __MICROSIP_PARSER_CLONE_H_
#include "microsip_struct.h"	
#include "microsip_cloneintrnl.h"
#include "microsip_bcptfree.h"
#include "microsip_bcptinit.h"
#ifdef SIP_TEL
#include "microsip_telstruct.h"
#include "microsip_telinit.h"
#endif
#ifdef SIP_IMURL
#include "microsip_imurlstruct.h"
#include "microsip_imurlinit.h"
#endif

/*******************************************************************************
** FUNCTION:sip_cloneSipCommonHeader
** DESCRIPTION:makes a deep copy of the source SipCommonHeader to the destination 
** SipCommonHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipCommonHeader 
**			pSource(IN)		- Source SipCommonHeader 
**			pErr(OUT)		- error value 
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipCommonHeader _ARGS_ 
    ((SipCommonHeader *pDest, 
      SipCommonHeader *pSource, 
      SipError        *pErr));
	
/*******************************************************************************
** FUNCTION: sip_cloneSipCommonHeaderList
** DESCRIPTION:makes a deep copy of the source SipList of Common header 
**             to the destination  SipList of Common Header List
** PARAMETERS:
**          pDest(OUT)      - Destination SipList
**          pSource(IN)     - Source SipList
**          pErr(OUT)       - error value 
**
** Return Values:
**          SipFail
**          SipSuccess
*******************************************************************************/
SipBool sip_cloneSipCommonHeaderList _ARGS_
    ((SipList *pDest, 
      SipList *pSource, 
      SipError *pErr));

#ifdef SIP_DATE
/*******************************************************************************
** FUNCTION:sip_cloneSipCommonHeaderWithDate
** DESCRIPTION:makes a deep copy of the source SipCommonHeader to the destination 
** SipCommonHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipCommonHeader 
**			pSource(IN)		- Source SipCommonHeader 
**			pErr(OUT)		- error value 
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipCommonHeaderWithDate _ARGS_ 
    ((SipCommonHeader *pDest, 
      SipCommonHeader *pSource, 
      SipError        *pErr));
#endif


#ifdef SIP_PEMEDIA
/*******************************************************************************
** FUNCTION:__sip_cloneSipPEarlyMediaHeader
** DESCRIPTION:makes a deep copy of the source SipPEarlyMediaHeader to the 
** destination  SipPEarlyMediaHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipPEarlyMediaHeader 
**			pSource(IN)		- Source SipPEarlyMediaHeader
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipPEarlyHeader sip_cloneSipCommonHeader

/*******************************************************************************
** FUNCTION:__sip_cloneSipPEarlyMediaList
** DESCRIPTION:makes a deep copy of the source SipPEarlyMediaList to the destination SipPEarlyMediaList 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipPEarlyMediaHeaderList     sip_cloneSipCommonHeaderList

#endif


/*******************************************************************************
** FUNCTION:sip_cloneSipStatusLine 
** DESCRIPTION:makes a deep copy of the source Status Line to the 
** destination Status Line
** PARAMETERS:
**			pDest(OUT)		- Destination Status Line
**			pSource(IN)		- Source Status Line
**			pErr(OUT)		- possible error value (see API ref doc)
**
*******************************************************************************/
#define sip_cloneSipStatusLine sip_cloneSipCommonHeader

/*******************************************************************************
** FUNCTION:sip_cloneSipReqLine 
** DESCRIPTION:makes a deep copy of the source Request Line to the 
** destination Request Line
** PARAMETERS:
**			pDest(OUT)		- Destination Request Line
**			pSource(IN)		- Source Request Line
**			pErr(OUT)		- possible error value (see API ref doc)
** Return Values:
**			SipFail
**			SipSuccess
**
*******************************************************************************/
#define sip_cloneSipReqLine sip_cloneSipCommonHeader

/*******************************************************************************
** FUNCTION:sip_cloneSipAddrSpec 
** DESCRIPTION:makes a deep copy of the source AddrSpec to the destination
**  AddrSpec
** PARAMETERS:
**			pDest(OUT)		- Destination AddrSpec
**			pSource(IN)		- Source AddrSpec
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipAddrSpec _ARGS_ ((SipAddrSpec *pDest, \
		SipAddrSpec *pSource, SipError *pErr));

#ifdef SIP_DATE
/*******************************************************************************
** FUNCTION:__sip_cloneSipDateStruct
** DESCRIPTION:makes a deep copy of the source SipDateStruct to the destination 
** SipDateStruct 
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateStruct 
**			pSource(IN)		- Source SipDateStruct 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSipDateStruct _ARGS_((SipDateStruct * dest, \
	SipDateStruct *src, SipError * err));
/*******************************************************************************
** FUNCTION:__sip_cloneSipDateFormat
** DESCRIPTION:makes a deep copy of the source SipDateFormat to the destination 
** SipDateFormat
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateFormat 
**			pSource(IN)		- Source SipDateFormat 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSipDateFormat _ARGS_((SipDateFormat * dest, \
	SipDateFormat *src, SipError * err));
/*******************************************************************************
** FUNCTION:__sip_cloneSipTimeFormat 
** DESCRIPTION:makes a deep copy of the source SipTimeFormat to the destination 
** SipTimeFormat
** PARAMETERS:
**			pDest(OUT)		- Destination SipTimeFormat 
**			pSource(IN)		- Source SipTimeFormat
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSipTimeFormat _ARGS_((SipTimeFormat * dest, \
	SipTimeFormat *src, SipError * err));
#endif

#ifdef SIP_CORE_STACK_COMPATIBLE
/*******************************************************************************
** FUNCTION:sip_cloneSipContactParam 
** DESCRIPTION:makes a deep copy of the source Contact Parameter to the 
**	destination Contact Parameter
** PARAMETERS:
**			pDest(OUT)		- Destination Contact Parameter Line
**			pSource(IN)		- Source Contact Parameter Line
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipContactParam _ARGS_ ((SipContactParam *pDest, \
		SipContactParam *pSource, SipError *pErr));

#endif		
/*******************************************************************************
** FUNCTION:sip_cloneSipExpiresStruct 
** DESCRIPTION:makes a deep copy of the source Expires struct to the 
**	destination Expires struct
** PARAMETERS:
**			pDest(OUT)		- Destination Expires Struct Line
**			pSource(IN)		- Source Expires Struct Line
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define sip_cloneSipExpiresStruct sip_cloneSipCommonHeader

/*******************************************************************************
** FUNCTION:sip_cloneSipMinExpiresStruct 
** DESCRIPTION:makes a deep copy of the source Min Expires struct to the 
**	destination Min Expires struct
** PARAMETERS:
**			pDest(OUT)		- Destination Expires Struct Line
**			pSource(IN)		- Source Expires Struct Line
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define sip_cloneSipMinExpiresStruct sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:sip_cloneSipStringList 
** DESCRIPTION:makes a deep copy of the source String LIst to the destination 
** String List
** PARAMETERS:
**			pDest(OUT)		- Destination String List
**			pSource(IN)		- Source String List
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipStringList _ARGS_ ((SipList *pDest, \
		SipList *pSource, SipError *pErr));
#ifdef SIP_CORE_STACK_COMPATIBLE

/*******************************************************************************                               
** FUNCTION:sip_cloneSipGenericChallenge 
** DESCRIPTION:makes a deep copy of the source Generic Challenge to the 
** destination Generic Challenge
** PARAMETERS:
**			pDest(OUT)		- Destination Generic Challenge
**			pSource(IN)		- Source Generic Challenge
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_AUTHENTICATE
extern SipBool sip_cloneSipGenericChallenge _ARGS_ ((SipGenericChallenge *pDest\
	,SipGenericChallenge *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sip_cloneSipGenericCredential 
** DESCRIPTION:makes a deep copy of the source GenericCredential to the 
** destination GenericCredential
** PARAMETERS:
**			pDest(OUT)		- Destination GenericCredential
**			pSource(IN)		- Source GenericCredential
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipGenericCredential _ARGS_ ((SipGenericCredential *pDest, \
		SipGenericCredential *pSource, SipError *pErr));
#endif
#endif
/*******************************************************************************
** FUNCTION:sip_cloneSipUrl 
** DESCRIPTION:makes a deep copy of the source SipUrl to the destination SipUrl
** PARAMETERS:
**			pDest(OUT)		- Destination SipUrl 
**			pSource(IN)		- Source SipUrl
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipUrl _ARGS_ ((SipUrl *pDest, \
		SipUrl *pSource, SipError *pErr));
#ifdef SIP_DATE
/*******************************************************************************
** FUNCTION:sip_cloneSipDateFormat 
** DESCRIPTION:makes a deep copy of the source SipDateFormat to the 
**	destination SipDateFormat 
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateFormat 
**			pSource(IN)		- Source SipDateFormat
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipDateFormat _ARGS_ ((SipDateFormat *pDest, \
		SipDateFormat *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sip_cloneSipTimeFormat 
** DESCRIPTION:makes a deep copy of the source SipTimeFormat to the destination SipTimeFormat
** PARAMETERS:
**			pDest(OUT)		- Destination SipTimeFormat 
**			pSource(IN)		- Source SipTimeFormat 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipTimeFormat _ARGS_ ((SipTimeFormat *pDest, \
		SipTimeFormat *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sip_cloneSipDateStruct 
** DESCRIPTION:makes a deep copy of the source SipDateStruct to the destination ** SipDateStruct 
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateStruct 
**			pSource(IN)		- Source SipDateStruct 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipDateStruct _ARGS_ ((SipDateStruct *pDest, \
		SipDateStruct *pSource, SipError *pErr));
#endif
/*******************************************************************************
** FUNCTION:sdp_cloneSdpTime 
** DESCRIPTION:makes a deep copy of the source SdpTime to the destination  
**	SdpTime 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpTime 
**			pSource(IN)		- Source SdpTime 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sdp_cloneSdpTime _ARGS_ ((SdpTime *ppDest, \
		SdpTime *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sdp_cloneSdpMedia 
** DESCRIPTION:makes a deep copy of the source SdpMedia to the destination 
**	SdpMedia 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpMedia 
**			pSource(IN)		- Source SdpMedia 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sdp_cloneSdpMedia _ARGS_ ((SdpMedia *ppDest, \
		SdpMedia *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sdp_cloneSdpConnection 
** DESCRIPTION:makes a deep copy of the source SdpConnection to the destination
**  SdpConnection 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpConnection 
**			pSource(IN)		- Source SdpConnection 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sdp_cloneSdpConnection _ARGS_ ((SdpConnection *ppDest, \
		SdpConnection *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sdp_cloneSdpAttr 
** DESCRIPTION:makes a deep copy of the source SdpAttributes to the 
** destination SdpAttributes 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpAttributes 
**			pSource(IN)		- Source SdpAttributes 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sdp_cloneSdpAttr _ARGS_ ((SdpAttr *ppDest,\
		SdpAttr *pSource, SipError *pErr));

#define sip_cloneSdpMessage sdp_cloneSdpMessage
/*******************************************************************************
** FUNCTION:sdp_cloneSdpMessage 
** DESCRIPTION:makes a deep copy of the source SdpMessage to the destination SdpMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpMessage 
**			pSource(IN)		- Source SdpMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sdp_cloneSdpMessage _ARGS_ ((SdpMessage *pDest, \
		SdpMessage *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sdp_cloneSdpOrigin 
** DESCRIPTION:makes a deep copy of the source SdpOrigin to the destination 
** SdpOrigin 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpOrigin 
**			pSource(IN)		- Source SdpOrigin 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sdp_cloneSdpOrigin _ARGS_ ((SdpOrigin *pDest, \
		SdpOrigin *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sip_cloneSipUnknownMessage 
** DESCRIPTION:makes a deep copy of the source SipUnknownMessage to the 
** destination SipUnknownMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipUnknownMessage 
**			pSource(IN)		- Source SipUnknownMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipUnknownMessage _ARGS_ ((SipUnknownMessage *pDest, \
		SipUnknownMessage *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sip_cloneSipMsgBody 
** DESCRIPTION:makes a deep copy of the source SipMessageBody to the 
** destination SipMessageBody 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMessageBody 
**			pSource(IN)		- Source SipMessageBody 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipMsgBody _ARGS_ ((SipMsgBody *pDest, \
		SipMsgBody *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sip_cloneSipParam 
** DESCRIPTION:makes a deep copy of the source SipParam to the destination 
** SipParam 
** PARAMETERS:
**			pDest(OUT)		- Destination SipParam 
**			pSource(IN)		- Source SipParam 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipParam _ARGS_ ((SipParam *pDest, \
		SipParam *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sip_cloneSipHeader 
** DESCRIPTION:makes a deep copy of the source SipHeader to the destination 
** SipHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipHeader 
**			pSource(IN)		- Source SipHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipHeader _ARGS_ ((SipHeader *pDest, \
		SipHeader *pSource, SipError *pErr));

/*******************************************************************************
** FUNCTION:sip_cloneSipMessage 
** DESCRIPTION:makes a deep copy of the source SipMessage to the destination 
** SipMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMessage 
**			pSource(IN)		- Source SipMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipMessage _ARGS_ ((SipMessage *pDest, \
		SipMessage *pSource, SipError *pErr));

#ifdef SIP_MIME_PARSING
/*******************************************************************************
** FUNCTION:sip_cloneSipMimeHeader 
** DESCRIPTION:makes a deep copy of the source SipMimeHeader to the destination
** SipMimeHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMimeHeader 
**			pSource(IN)		- Source SipMimeHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneSipMimeHeader _ARGS_((SipMimeHeader *pDest, \
		SipMimeHeader *pSource, SipError *pErr));
#endif
/*******************************************************************************
** FUNCTION:sip_cloneMimeMessage 
** DESCRIPTION:makes a deep copy of the source SipMimeMessage to the
** destination SipMimeMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMimeMessage 
**			pSource(IN)		- Source SipMimeMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_MIME_PARSING
extern SipBool sip_cloneMimeMessage _ARGS_((MimeMessage *pDest, \
		MimeMessage *pSource, SipError *pErr));
#endif	
#ifdef SIP_MWI
/*****************************************************************
** Function:sip_mwi_cloneMesgSummaryMessage
** Description:makes a deep copy of the source MesgSummaryMessage to \
the destination MesgSummaryMessage
** Parameters:
**                      pDest(OUT)   - Destination MesgSummaryMessage
**                      pSource(IN)  - Source MesgSummaryMessage
**                      pErr(OUT) - possible error value (see APIref doc)
**
******************************************************************/
SipBool sip_mwi_cloneMesgSummaryMessage _ARGS_ (( \
            MesgSummaryMessage *pDest, MesgSummaryMessage *pSource,\
            SipError *pErr));

/*****************************************************************
** Function:sip_mwi_cloneSummaryLine
** Description:makes a deep copy of the source SummaryLine \
to the destination SummaryLine
** Parameters:
**                      pDest(OUT) - Destination SummaryLine
**                      pSource(IN)- Source SummaryLine
**                      pErr(OUT) - possible error value (see APIref doc)
**
******************************************************************/
SipBool sip_mwi_cloneSummaryLine _ARGS_ ((SummaryLine *pDest, \
            SummaryLine *pSource, SipError *pErr));


SipBool __sip_mwi_cloneMesgSummaryMessage _ARGS_ (( \
            MesgSummaryMessage *pDest, MesgSummaryMessage *pSource, \
            SipError *pErr));

SipBool __sip_mwi_cloneSummaryLine _ARGS_ ((SummaryLine *pDest, \
            SummaryLine *pSource, SipError *pErr));
#endif
	
/*******************************************************************************
** FUNCTION:sip_cloneSipNameValuePair
** DESCRIPTION:makes a deep copy of the source NameValuePair to the 
** destination NameValuePair 
** PARAMETERS:
**                      pDest(OUT)              - Destination NameValuePair
**                      pSource(IN)             - Source NameValuePair
**                      pErr(OUT)               - possible error value 
**													(see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_ISUP
/*******************************************************************************
** FUNCTION:sip_cloneIsupMessage 
** DESCRIPTION:makes a deep copy of the source IsupMessage to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination IsupMessage 
**			pSource(IN)		- Source IsupMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool sip_cloneIsupMessage _ARGS_((IsupMessage *pDest, \
		IsupMessage *pSource, SipError *pErr));
#endif
#ifdef   SIP_BADMESSAGE_PARSING
/*******************************************************************************
** FUNCTION:__sip_cloneSipBadHeader 
** DESCRIPTION:makes a deep copy of the source SipBadHeader to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipBadHeader
**			pSource(IN)		- Source SipBadHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipBadHeader _ARGS_ ((SipBadHeader *dest,\
	 SipBadHeader *source, SipError *err));
 
#endif

/*******************************************************************************
** FUNCTION:sip_cloneSipFromHeader 
** DESCRIPTION:makes a deep copy of the source SipFromHeader to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipFromHeader
**			pSource(IN)		- Source SipFromHeader
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

#define __sip_cloneSipFromHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:sip_cloneSipToHeader
** DESCRIPTION:makes a deep copy of the source SipToHeader to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipToHeader 
**			pSource(IN)		- Source SipToHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipToHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:sip_cloneRouteHeader 
** DESCRIPTION:makes a deep copy of the source SipRouteHeader to the destination
** SipRouteHeader  
** PARAMETERS:
**			pDest(OUT)		- Destination SipRouteHeader 
**			pSource(IN)		- Source SipRouteHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipRouteHeader sip_cloneSipCommonHeader
#ifdef SIP_3GPP
#define __sip_cloneSipPathHeader 							  sip_cloneSipCommonHeader
#define __sip_cloneSipPAssociatedUriHeader  	  sip_cloneSipCommonHeader
#define __sip_cloneSipPCalledPartyIdHeader		  sip_cloneSipCommonHeader
#define __sip_cloneSipPVisitedNetworkIdHeader	  sip_cloneSipCommonHeader
#define __sip_cloneSipPcfAddrHeader						  sip_cloneSipCommonHeader
#define __sip_cloneSipPanInfoHeader						  sip_cloneSipCommonHeader
#define __sip_cloneSipPcVectorHeader					  sip_cloneSipCommonHeader		
#define __sip_cloneSipServiceRouteHeader        sip_cloneSipCommonHeader
#define __sip_cloneSipHistoryInfoHeader         sip_cloneSipCommonHeader
#define __sip_cloneSipAcceptContatHeader 			  sip_cloneSipCommonHeader
#define __sip_cloneSipRejectContactHeader 		  sip_cloneSipCommonHeader
#define __sip_cloneSipRequestDispositionHeader  sip_cloneSipCommonHeader
#define __sip_cloneSipJoinHeader 								sip_cloneSipCommonHeader
#define __sip_cloneSipIfMatchHeader							sip_cloneSipCommonHeader
#define __sip_cloneSipETagHeader								sip_cloneSipCommonHeader
#endif

#ifdef SIP_SECURITY
#define __sip_cloneSipSecurityClientHeader          sip_cloneSipCommonHeader
#define __sip_cloneSipSecurityServerHeader          sip_cloneSipCommonHeader
#define __sip_cloneSipSecurityVerifyHeader          sip_cloneSipCommonHeader

#define __sip_cloneSipSecurityClientHeaderList  	sip_cloneSipCommonHeaderList
#define __sip_cloneSipSecurityVerifyHeaderList  	sip_cloneSipCommonHeaderList
#define __sip_cloneSipSecurityServerHeaderList  	sip_cloneSipCommonHeaderList
#endif
/*******************************************************************************
** FUNCTION:sip_cloneRecordRouteHeader 
** DESCRIPTION:makes a deep copy of the source SipRecordRouteHeader to the destination 
** SipRecordRouteHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipRecordRouteHeader 
**			pSource(IN)		- Source SipRecordRouteHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipRecordRouteHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipReferredByHeader
** DESCRIPTION:makes a deep copy of the source SipReferredByHeader to the 
**	destination  SipRecordRouteHeader
**  
** PARAMETERS:
**			pDest(OUT)		- Destination SipRecordRouteHeader 
**			pSource(IN)		- Source SipRecordRouteHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipReferredByHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipRseqHeader 
** DESCRIPTION:makes a deep copy of the source SipRseqHeader to the destination 
** SipRseqHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipRseqHeader 
**			pSource(IN)		- Source SipRseqHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipRseqHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneIsupMessage
** DESCRIPTION:makes a deep copy of the source SipSupportedHeader to the 
**	destination  SipSupportedHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipSupportedHeader 
**			pSource(IN)		- Source SipSupportedHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipSupportedHeader  sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipMimeVersionHeader
DESCRIPTION:makes a deep copy of the source SipMimeVersionHeader
**  to the destination SipMimeVersionHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMimeVersionHeader 
**			pSource(IN)		- Source SipMimeVersionHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipMimeVersionHeader  sip_cloneSipCommonHeader
#define __sip_bcpt_cloneSipMimeVersionHeader  sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipAllowHeader
** DESCRIPTION:makes a deep copy of the source SipAllowHeader to the destination** SipAllowHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipAllowHeader 
**			pSource(IN)		- Source SipAllowHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipAllowHeader  sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipAllowEventsHeader
** DESCRIPTION:makes a deep copy of the source SipAllowEventsHeader
** to the destination SipAllowEventsHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipAllowEventsHeader 
**			pSource(IN)		- Source SipAllowEventsHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipAllowEventsHeader  sip_cloneSipCommonHeader
#define __sip_impp_cloneSipAllowEventsHeader  sip_cloneSipCommonHeader

 #ifdef SIP_PRIVACY
/*******************************************************************************
** FUNCTION:__sip_cloneSipPrivacyHeader
** DESCRIPTION:makes a deep copy of the source SipPrivacyHeader to the destination** SipPrivacyHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipPrivacyHeader 
**			pSource(IN)		- Source SipPrivacyHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipPrivacyHeader  sip_cloneSipCommonHeader

/*******************************************************************************
** FUNCTION:__sip_cloneSipPAssertIdHeader
** DESCRIPTION:makes a deep copy of the source SipPAssertIdHeader to the destination** SipPAssertIdHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipPAssertIdHeader 
**			pSource(IN)		- Source SipPAssertIdHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipPAssertIdHeader  sip_cloneSipCommonHeader
#define __sip_cloneSipPAssertIdHeaderList  	sip_cloneSipCommonHeaderList

/*******************************************************************************
** FUNCTION:__sip_cloneSipPPreferredIdHeader
** DESCRIPTION:makes a deep copy of the source SipPPreferredIdHeader to the destination** SipPPreferredIdHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipPPreferredIdHeader 
**			pSource(IN)		- Source SipPPreferredIdHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipPPreferredIdHeader  sip_cloneSipCommonHeader
#define __sip_cloneSipPPreferredIdHeaderList 	sip_cloneSipCommonHeaderList
#endif

/*******************************************************************************
** FUNCTION:__sip_cloneSipContentEncodingHeaderList
** DESCRIPTION:makes a deep copy of the source SipContentEncodingHeader to 
** the destination SipContentEncodingHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipContentEncodingHeader 
**			pSource(IN)		- Source SipContentEncodingHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipContentEncodingHeader  sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipCallIdHeader
** DESCRIPTION:makes a deep copy of the source SipCallIdHeader to the 
** destination SipCallIdHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipCallIdHeader 
**			pSource(IN)		- Source SipCallIdHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipCallIdHeader  sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipRequireHeader
** DESCRIPTION:makes a deep copy of the source SipRequireHeader to the 
** destination SipRequireHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipRequireHeader 
**			pSource(IN)		- Source SipRequireHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipRequireHeader  sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipParam
** DESCRIPTION:makes a deep copy of the source SipParam to the destination 
** SipParam
** PARAMETERS:
**			pDest(OUT)		- Destination SipParam 
**			pSource(IN)		- Source SipParam 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipParam			__sipParser_cloneSipParam
/*******************************************************************************
** FUNCTION:__sip_cloneSipUrl
** DESCRIPTION:makes a deep copy of the source SipUrl to the destination 
** SipUrl
** PARAMETERS:
**			pDest(OUT)		- Destination SipUrl 
**			pSource(IN)		- Source SipUrl 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipUrl     __sipParser_cloneSipUrl
/*******************************************************************************
** FUNCTION:__sip_cloneAddrSpec
** DESCRIPTION:makes a deep copy of the source SipAddrSpec to the destination 
** SipAddrSpec 
** PARAMETERS:
**			pDest(OUT)		- Destination SipAddrSpec 
**			pSource(IN)		- Source SipAddrSpec 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneAddrSpec	__sipParser_cloneSipAddrSpec
/*******************************************************************************
** FUNCTION:__sip_cloneSipParamList
** DESCRIPTION:makes a deep copy of the source SipList of Sipparams to the 
** destination SipList 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList of SipParams  
**			pSource(IN)		- Source SipList of SipParams  
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipParamList  __sipParser_cloneSipParamList

/*******************************************************************************
** FUNCTION:__sip_cloneSipCseqHeader
** DESCRIPTION:makes a deep copy of the source SipCseqHeader to the destination 
** SipCseqHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipCseqHeader 
**			pSource(IN)		- Source SipCseqHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

#define __sip_cloneSipCseqHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipContentLengthHeader
** DESCRIPTION:makes a deep copy of the source SipContentLengthHeader to the 
** destination  SipContentLengthHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination  SipContentLengthHeader 
**			pSource(IN)		- Source  SipContentLengthHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipContentLengthHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipContentTypeHeader
** DESCRIPTION:makes a deep copy of the source SipContentTypeHeader to the 
** destination  SipContentTypeHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipContentTypeHeader 
**			pSource(IN)		- Source SipContentTypeHeader
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipContentTypeHeader sip_cloneSipCommonHeader

/*******************************************************************************
** FUNCTION:__sip_cloneSipAcceptHeader
** DESCRIPTION:makes a deep copy of the source SipAcceptHeader to the 
** destination  SipAcceptHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipAcceptHeader 
**			pSource(IN)		- Source SipAcceptHeader
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipAcceptHeader sip_cloneSipAcceptHeader

/*******************************************************************************
** FUNCTION:__sip_cloneSipUnknownHeader
** DESCRIPTION:makes a deep copy of the source SipUnknownHeader to the 
** destination SipUnknownHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipUnknownHeader 
**			pSource(IN)		- Source SipUnknownHeader
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

#define __sip_cloneSipUnknownHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipExpiresHeader
** DESCRIPTION:makes a deep copy of the source SipExpiresHeader to the 
** destination  SipExpiresHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipExpiresHeader 
**			pSource(IN)		- Source SipExpiresHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_DATE
#define __sip_cloneSipExpiresHeader sip_cloneSipCommonHeaderWithDate
#else
#define __sip_cloneSipExpiresHeader sip_cloneSipCommonHeader
#endif
/*******************************************************************************
** FUNCTION:__sip_cloneSipMinExpiresHeader
** DESCRIPTION:makes a deep copy of the source SipMinExpiresHeader to the 
** destination  SipMinExpiresHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMinExpiresHeader 
**			pSource(IN)		- Source SipMinExpiresHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipMinExpiresHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipViaHeader
** DESCRIPTION:makes a deep copy of the source SipViaHeader to the destination 
** SipViaHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipViaHeader 
**			pSource(IN)		- Source SipViaHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

#define __sip_cloneSipViaHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipContactHeader
** DESCRIPTION:makes a deep copy of the source SipContactHeader to the 
** destination SipContactHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipContactHeader 
**			pSource(IN)		- Source SipContactHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

#define __sip_cloneSipContactHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipRecordRouteHeader
** DESCRIPTION:makes a deep copy of the source SipRecordRouteHeader to the 
** destination  SipRecordRouteHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipRecordRouteHeader 
**			pSource(IN)		- Source SipRecordRouteHeader
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipRecordRouteHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipStatusLine
** DESCRIPTION:makes a deep copy of the source SipStatusLine to the destination 
** SipStatusLine
** PARAMETERS:
**			pDest(OUT)		- Destination SipStatusLine 
**			pSource(IN)		- Source SipStatusLine 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipStatusLine sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipReqLine
** DESCRIPTION:makes a deep copy of the source SipReqLine to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipReqLine 
**			pSource(IN)		- Source SipReqLine
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipReqLine sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipWarningHeader
** DESCRIPTION:makes a deep copy of the source SipWarningHeader to the 
** destination  SipWarningHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipWarningHeader 
**			pSource(IN)		- Source SipWarningHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_WARNING
#define __sip_cloneSipWarningHeader sip_cloneSipCommonHeader
#endif	
/*******************************************************************************
** FUNCTION:__sip_cloneSipRetryAfterHeader
** DESCRIPTION:makes a deep copy of the source SipRetryAfterHeader to the 
** destination  SipRetryAfterHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipRetryAfterHeader 
**			pSource(IN)		- Source SipRetryAfterHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

#ifndef SIP_DATE
#define __sip_cloneSipRetryAfterHeader sip_cloneSipCommonHeader
#else
#define __sip_cloneSipRetryAfterHeader sip_cloneSipCommonHeaderWithDate
#endif

/*******************************************************************************
** FUNCTION:__sip_cloneSipProxyAuthenticateHeader
** DESCRIPTION:makes a deep copy of the source SipProxyAuthenticateHeader to
** the destination  SipProxyAuthenticateHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipProxyAuthenticateHeader 
**			pSource(IN)		- Source SipProxyAuthenticateHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipProxyAuthenticateHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipWwwAuthenticateHeader
** DESCRIPTION:makes a deep copy of the source SipWwwAuthenticateHeader to the 
** destination  SipWwwAuthenticateHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipWwwAuthenticateHeader 
**			pSource(IN)		- Source SipWwwAuthenticateHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipWwwAuthenticateHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipProxyAuthorizationHeader
** DESCRIPTION:makes a deep copy of the source SipProxyAuthorizationHeader to 
** the  destination  ProxyAuthorizationHeaderder 
** PARAMETERS:
**			pDest(OUT)		- Destination ProxyAuthorizationHeaderder 
**			pSource(IN)		- Source ProxyAuthorizationHeaderder 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipProxyAuthorizationHeader  sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipReferToHeader
** DESCRIPTION:makes a deep copy of the source SipReferToHeader to the 
** destination SipReferToHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipReferToHeader 
**			pSource(IN)		- Source SipReferToHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_REFER
#define __sip_cloneSipReferToHeader sip_cloneSipCommonHeader
#endif
/*******************************************************************************
** FUNCTION:__sip_cloneSipAuthorizationHeader
** DESCRIPTION:makes a deep copy of the source SipAuthorizationHeader to the 
**destination SipAuthorizationHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipAuthorizationHeader 
**			pSource(IN)		- Source SipAuthorizationHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_AUTHENTICATE
#define __sip_cloneSipAuthorizationHeader sip_cloneSipCommonHeader
#endif		
/*******************************************************************************
** FUNCTION:__sip_cloneSipContentDispositionHeader
** DESCRIPTION:makes a deep copy of the source SipContentDispositionHeader to 
** the destination SipContentDispositionHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipContentDispositionHeader
**			pSource(IN)		- Source SipContentDispositionHeader
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipContentDispositionHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipAddrSpec
** DESCRIPTION:makes a deep copy of the source SipAddrSpec to the destination 
** SipAddrSpec 
** PARAMETERS:
**			pDest(OUT)		- Destination SipAddrSpec 
**			pSource(IN)		- Source SipAddrSpec 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSipAddrSpec _ARGS_ ( (SipAddrSpec *dest, \
	SipAddrSpec *source, SipError *err));
/*******************************************************************************
** FUNCTION:__sip_cloneSipViaParam
** DESCRIPTION:makes a deep copy of the source SipParam to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination IsupMessage 
**			pSource(IN)		- Source IsupMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSipViaParam _ARGS_ ( (SipParam *dest, \
	SipParam *source, SipError *err));
 #ifdef SIP_CORE_STACK_COMPATIBLE
/*******************************************************************************
** FUNCTION:__sip_cloneSipContactParam
** DESCRIPTION:makes a deep copy of the source SipContactParam to the 
** destination SipContactParam 
** PARAMETERS:
**			pDest(OUT)		- Destination SipContactParam 
**			pSource(IN)		- Source SipContactParam
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSipContactParam _ARGS_ ( (SipContactParam *dest, \
	SipContactParam *source, SipError *err));
#endif /* SIP_CORE_STACK_COMPATIBLE */

/*******************************************************************************
** FUNCTION:__sip_cloneSipExpiresStruct
** DESCRIPTION:makes a deep copy of the source SipExpiresStruct to the 
** destination SipExpiresStruct 
** PARAMETERS:
**			pDest(OUT)		- Destination SipExpiresStruct 
**			pSource(IN)		- Source SipExpiresStruct
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_DATE
#define __sip_cloneSipExpiresStruct sip_cloneSipCommonHeaderWithDate
#else
#define __sip_cloneSipExpiresStruct sip_cloneSipCommonHeader
#endif
/*******************************************************************************
** FUNCTION:__sip_cloneSipMinExpiresStruct
** DESCRIPTION:makes a deep copy of the source SipMinExpiresStruct to the 
** destination SipMinExpiresStruct 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMinExpiresStruct 
**			pSource(IN)		- Source SipMinExpiresStruct
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipMinExpiresStruct sip_cloneSipCommonHeader

#ifdef SIP_CORE_STACK_COMPATIBLE
/*******************************************************************************
** FUNCTION:__sip_cloneChallenge
** DESCRIPTION:makes a deep copy of the source SipGenericChallenge to the 
** destination SipGenericChallenge 
** PARAMETERS:
**			pDest(OUT)		- Destination SipGenericChallenge 
**			pSource(IN)		- Source SipGenericChallenge 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_AUTHENTICATE
#define __sip_cloneChallenge __sip_cloneSipChallenge
/*******************************************************************************
** FUNCTION:__sip_cloneCredential
** DESCRIPTION:makes a deep copy of the source SipGenericCredential to the 
** destination  SipGenericCredential 
** PARAMETERS:
**			pDest(OUT)		- Destination SipGenericCredential 
**			pSource(IN)		- Source SipGenericCredential 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneCredential _ARGS_((SipGenericCredential *to,\
	SipGenericCredential *from,SipError *err));
#endif	
#endif
#ifdef SIP_TIMESTAMP			
/*******************************************************************************
** FUNCTION:__sip_cloneSipTimeStampHeader
** DESCRIPTION:makes a deep copy of the source SipTimeStampHeader to the
** destination SipTimeStampHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipTimeStampHeader 
**			pSource(IN)		- Source SipTimeStampHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipTimeStampHeader sip_cloneSipCommonHeader
#endif	
/*******************************************************************************
** FUNCTION:__sip_cloneSipUrlParam
** DESCRIPTION:makes a deep copy of the source SipParam to the destination 
** Sipparam
** PARAMETERS:
**			pDest(OUT)		- Destination SipParam 
**			pSource(IN)		- Source SipParam 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

extern  SipBool __sip_cloneSipUrlParam _ARGS_((SipParam *to,SipParam *from, \
	SipError *err));
/*******************************************************************************
** FUNCTION:__sip_cloneSipUrl
** DESCRIPTION:makes a deep copy of the source SipUrl to the destination 
** SipUrl
** PARAMETERS:
**			pDest(OUT)		- Destination SipUrl
**			pSource(IN)		- Source SipUrl 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSipUrl _ARGS_((SipUrl *to,SipUrl *from,SipError *err));

#ifdef SIP_DATE
/*******************************************************************************
** FUNCTION:__sip_cloneDateStruct
** DESCRIPTION:makes a deep copy of the source SipDateStruct to the destination 
** SipDateStruct 
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateStruct
**			pSource(IN)		- Source SipDateStruct
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneDateStruct _ARGS_((SipDateStruct * dest, \
	SipDateStruct *src,SipError *err));
#endif

#ifdef  SIP_CORE_STACK_COMPATIBLE

/*******************************************************************************
** FUNCTION:__sip_cloneSipChallenge
** DESCRIPTION:makes a deep copy of the source SipGenericChallenge to the 
** destination SipGenericChallenge 
** PARAMETERS:
**			pDest(OUT)		- Destination SipGenericChallenge 
**			pSource(IN)		- Source SipGenericChallenge 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_AUTHENTICATE
extern  SipBool __sip_cloneSipChallenge _ARGS_((SipGenericChallenge * \
	dest, SipGenericChallenge *src,SipError *err));
#endif	
#endif 
#ifdef SIP_DATE
/*******************************************************************************
** FUNCTION: __sip_cloneDateFormat
** DESCRIPTION:makes a deep copy of the source SipDateFormat to the destination 
** SipDateFormat
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateFormat 
**			pSource(IN)		- Source SipDateFormat
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneDateFormat _ARGS_((SipDateFormat *dest, \
	SipDateFormat *src,SipError *err));
/*******************************************************************************
** FUNCTION:__sip_cloneTimeFormat
** DESCRIPTION:makes a deep copy of the source SipTimeFormat to the destination 
** SiptimeFormat
** PARAMETERS:
**			pDest(OUT)		- Destination SiptimeFormat 
**			pSource(IN)		- Source SipTimeFormat
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneTimeFormat _ARGS_((SipTimeFormat * dest, \
	SipTimeFormat *src,SipError *err));
#endif
/*******************************************************************************
** FUNCTION:__sip_cloneSdpTime
** DESCRIPTION:makes a deep copy of the source SdpTime to the destination 
** SdpTime 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpTime 
**			pSource(IN)		- Source SdpTime 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSdpTime _ARGS_(( SdpTime 	*dest, SdpTime 	\
	*src, SipError 		*err));
/*******************************************************************************
** FUNCTION:__sip_cloneSdpMedia
** DESCRIPTION:makes a deep copy of the source SdpMedia to the destination 
** SdpMedia
** PARAMETERS:
**			pDest(OUT)		- Destination SdpMedia 
**			pSource(IN)		- Source SdpMedia 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSdpMedia _ARGS_(( SdpMedia 	*dest, SdpMedia 	\
	*src, SipError 		*err)) ;
/*******************************************************************************
** FUNCTION:__sip_cloneSdpMessage
** DESCRIPTION:makes a deep copy of the source SdpMessage to the destination 
** SdpMessage
** PARAMETERS:
**			pDest(OUT)		- Destination SdpMessage 
**			pSource(IN)		- Source SdpMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSdpMessage _ARGS_((SdpMessage * dest, SdpMessage \
	*src, SipError *err));
/*******************************************************************************
** FUNCTION:__sip_cloneSdpOrigin
** DESCRIPTION:makes a deep copy of the source SdpOrigin to the destination 
** SdpOrigin
** PARAMETERS:
**			pDest(OUT)		- Destination SdpOrigin 
**			pSource(IN)		- Source SdpOrigin
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSdpOrigin _ARGS_((SdpOrigin * dest, SdpOrigin *src, \
	SipError *err));
/*******************************************************************************
** FUNCTION:__sip_cloneSdpConnection
** DESCRIPTION:makes a deep copy of the source SdpConnection to the destination 
** SdpConnection 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpConnection 
**			pSource(IN)		- Source SdpConnection 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSdpConnection _ARGS_((SdpConnection * dest, \
	SdpConnection *src, SipError *err)) ;
/*******************************************************************************
** FUNCTION:__sip_cloneSdpAttr
** DESCRIPTION:makes a deep copy of the source SdpAttr to the destination 
** SdpAttr
** PARAMETERS:
**			pDest(OUT)		- Destination SdpAttr 
**			pSource(IN)		- Source SdpAttr
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern  SipBool __sip_cloneSdpAttr _ARGS_((SdpAttr * dest, SdpAttr *src, \
	SipError *err));
/*******************************************************************************
** FUNCTION:__sip_cloneSipUnknownMessage
** DESCRIPTION:makes a deep copy of the source SipUnknownMessage to the 
** destination  SipUnknownMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipUnknownMessage 
**			pSource(IN)		- Source SipUnknownMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipUnknownMessage _ARGS_((SipUnknownMessage *dest, \
	SipUnknownMessage *src, SipError *err));
/*******************************************************************************
** FUNCTION:__sip_cloneSipMsgBody
** DESCRIPTION:makes a deep copy of the source SipMsgBody to the destination 
** SipMsgBody 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMsgBody 
**			pSource(IN)		- Source SipMsgBody 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipMsgBody _ARGS_((SipMsgBody *dest, SipMsgBody *src,\
	SipError *err));

/*******************************************************************************
** FUNCTION:__sip_cloneSipUnsupportedHeader
** DESCRIPTION:makes a deep copy of the source SipUnsupportedHeader to the 
** destination SipUnsupportedHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipUnsupportedHeader
**			pSource(IN)		- Source SipUnsupportedHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipUnsupportedHeader sip_cloneSipCommonHeader
/*******************************************************************************
** FUNCTION:__sip_cloneSipMessage
** DESCRIPTION:makes a deep copy of the source SipMessage to the destination 
** SipMessage
** PARAMETERS:
**			pDest(OUT)		- Destination SipMessage 
**			pSource(IN)		- Source SipMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipMessage _ARGS_ ((SipMessage *pDest, SipMessage \
	*pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION:__sip_cloneSipOrderInfoList
** DESCRIPTION:makes a deep copy of the source SipList of Header order Info to 
** the destination destination list of header order info 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipOrderInfoList _ARGS_ ((SipList *pDest, SipList \
	*pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION:__sip_cloneMessageBodyList
** DESCRIPTION:makes a deep copy of the source SipList of Message Body to the 
** destination SipList of message body
** PARAMETERS:
**			pDest(OUT)		- Destination SipList 
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipMessageBodyList _ARGS_ ((SipList *pDest, SipList \
	*pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION:__sip_cloneSipContactHeaderList
** DESCRIPTION:makes a deep copy of the source SipList of Contact header to the destination  SipList of Contact Header List
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
/* GENERAL HEADERS */
#define __sip_cloneSipContactHeaderList  	sip_cloneSipCommonHeaderList
/*******************************************************************************
** FUNCTION:__sip_cloneSipAcceptHeader
** DESCRIPTION:makes a deep copy of the source SipList of Accept header to the destination  SipList of Accept Header List
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
/* GENERAL HEADERS */
#define __sip_cloneSipAcceptHeaderList  	sip_cloneSipCommonHeaderList
#ifdef SIP_3GPP
#define __sip_cloneSipPathHeaderList  							sip_cloneSipCommonHeaderList
#define __sip_cloneSipPAssociatedUriHeaderList  		sip_cloneSipCommonHeaderList
#define __sip_cloneSipPVisitedNetworkIdHeaderList		sip_cloneSipCommonHeaderList
#define __sip_cloneSipServiceRouteHeaderList  			sip_cloneSipCommonHeaderList
#define __sip_cloneSipHistoryInfoHeaderList  				sip_cloneSipCommonHeaderList
#define __sip_cloneSipRequestDispositionHeaderList	sip_cloneSipCommonHeaderList		
#define __sip_cloneSipAcceptContactHeaderList				sip_cloneSipCommonHeaderList
#define __sip_cloneSipRejectContactHeaderList				sip_cloneSipCommonHeaderList
#endif
/*******************************************************************************
** FUNCTION:__sip_cloneSipRecordRouteHeaderList
** DESCRIPTION:makes a deep copy of the source SipList of record route headers 
** to the destination SipList 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

#define __sip_cloneSipRecordRouteHeaderList     sip_cloneSipCommonHeaderList
/*******************************************************************************
** FUNCTION:__sip_cloneSipViaHeaderList
** DESCRIPTION:makes a deep copy of the source SipList of Via Headers to 
** destination List 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipViaHeaderList     sip_cloneSipCommonHeaderList
/*******************************************************************************
** FUNCTION:__sip_cloneSipContentEncodingList
** DESCRIPTION:makes a deep copy of the source SipContentEncodingList to the destination SipContentEncodingList 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipContentEncodingHeaderList     sip_cloneSipCommonHeaderList
/*******************************************************************************
** FUNCTION:__sip_cloneSipUnknownHeaderList
** DESCRIPTION:makes a deep copy of the source SipList of unknown headers to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination IsupMessage 
**			pSource(IN)		- Source IsupMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipUnknownHeaderList     sip_cloneSipCommonHeaderList
/*******************************************************************************
** FUNCTION:__sip_cloneSipSupportedHeaderList
** DESCRIPTION:makes a deep copy of the source SipList to the destination 
** SipList
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipSupportedHeaderList     sip_cloneSipCommonHeaderList
/*******************************************************************************
** FUNCTION:__sip_cloneSipContentDispositionHeaderList
** DESCRIPTION:makes a deep copy of the source SipContentDispositionHeader list
** to the destination List 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipContentDispositionHeaderList     sip_cloneSipCommonHeaderList

/*******************************************************************************
** FUNCTION: __sip_cloneSipProxyAuthorizationHeaderList
** DESCRIPTION:makes a deep copy of the source SipList to the destination 
** SipList
** PARAMETERS
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_AUTHENTICATE
#define __sip_cloneSipProxyAuthorizationHeaderList sip_cloneSipCommonHeaderList
extern SipBool __sip_cloneSipProxyAuthorizationHeaderList _ARGS_ ((SipList \
	*pDest, SipList *pSource, SipError *pErr));
#endif	
/*******************************************************************************
** FUNCTION:__sip_cloneSipRouteHeaderList
** DESCRIPTION:makes a deep copy of the source SipList to the destination  List
** 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipRouteHeaderList sip_cloneSipCommonHeaderList
/*******************************************************************************
** FUNCTION:__sip_cloneSipRequireHeaderList
** DESCRIPTION:makes a deep copy of the source SipList to the destination 
** SipList
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipRequireHeaderList sip_cloneSipCommonHeaderList
/*******************************************************************************
** FUNCTION:__sip_cloneSipWwwAuthenticateHeaderList
** DESCRIPTION:makes a deep copy of the source SipList to the destination 
** SipList
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_AUTHENTICATE
#define __sip_cloneSipWwwAuthenticateHeaderList sip_cloneSipCommonHeaderList
#endif
/*******************************************************************************
** FUNCTION:__sip_cloneSipAllowHeaderList
** DESCRIPTION:makes a deep copy of the source SipAllowHeaderList to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination IsupMessage 
**			pSource(IN)		- Source IsupMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
/* RESPONSE HEADER */
#define __sip_cloneSipAllowHeaderList sip_cloneSipCommonHeaderList


/*******************************************************************************
** FUNCTION:__sip_cloneSipProxyAuthenticateHeaderList
** DESCRIPTION:makes a deep copy of the source ProxyAuthenticateHeaderList 
**to the destination SipList 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_AUTHENTICATE
#define __sip_cloneSipProxyAuthenticateHeaderList sip_cloneSipCommonHeaderList
#endif	
/*******************************************************************************
** FUNCTION:__sip_cloneSipUnsupportedHeaderList
** DESCRIPTION:makes a deep copy of the source SipList of unsupported header 
** to destination List 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipUnsupportedHeaderList sip_cloneSipCommonHeaderList
#ifdef SIP_WARNING	
/*******************************************************************************
** FUNCTION:__sip_cloneSipWarningHeaderList
** DESCRIPTION:makes a deep copy of the source SipList of Warning headers to 
** the destination List 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipWarningHeaderList sip_cloneSipCommonHeaderList
#endif
/* COPYING ALL HEADERS */
/*******************************************************************************
** FUNCTION:__sip_cloneSipGeneralHeader
** DESCRIPTION:makes a deep copy of the source SipGeneralHeader to the 
** destination SipGeneralHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipGeneralHeader 
**			pSource(IN)		- Source SipGeneralHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipGeneralHeader _ARGS_ ((SipGeneralHeader *pDest, \
	SipGeneralHeader *pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION:__sip_cloneSipReqheader
** DESCRIPTION:makes a deep copy of the source SipReqHeader to the destination 
** SipReqHeader
** PARAMETERS:
**			pDest(OUT)		- Destination SipReqHeader 
**			pSource(IN)		- Source SipReqHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipReqHeader _ARGS_ ((SipReqHeader *pDest, \
	SipReqHeader *pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION:__sip_cloneSipRespHeader
** DESCRIPTION:makes a deep copy of the source SipRespHeader to the destination 
** SipRespHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipRespHeader 
**			pSource(IN)		- Source SipRespHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipRespHeader _ARGS_ ((SipRespHeader *pDest, \
	SipRespHeader *pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION:__sip_cloneSipReqMessage
** DESCRIPTION:makes a deep copy of the source SipReqMessage to the destination 
** SipReqMessage
** PARAMETERS:
**			pDest(OUT)		- Destination SipReqMessage
**			pSource(IN)		- Source SipReqMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
/* COPYING MESSAGES */
extern SipBool __sip_cloneSipReqMessage _ARGS_ ((SipReqMessage *pDest, \
	SipReqMessage *pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION:__sip_cloneSipRespMessage
** DESCRIPTION:makes a deep copy of the source SipRespMessage to the 
** destination SipRespMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipRespMessage
**			pSource(IN)		- Source SipRespMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipRespMessage _ARGS_ ((SipRespMessage *pDest, \
	SipRespMessage *pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION: __sip_cloneSipHeaderOrderInfo
** DESCRIPTION:makes a deep copy of the source SipHeaderOrderInfo to the 
** destination  SipHeaderOrderInfo 
** PARAMETERS:
**			pDest(OUT)		- Destination SipHeaderOrderInfo 
**			pSource(IN)		- Source SipHeaderOrderInfo 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

extern SipBool __sip_cloneSipHeaderOrderInfo _ARGS_ ((SipHeaderOrderInfo \
	*pDest, SipHeaderOrderInfo *pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION: __sip_cloneSipAuthorizationHeaderList
** DESCRIPTION:makes a deep copy of the source SipList of Authorization headers to the destination SipList 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList of Auth Headers
**			pSource(IN)		- Source SipList of Auth Headers
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#if defined(SIP_AUTHENTICATE) || defined(SIP_3GPP)
#define __sip_cloneSipAuthorizationHeaderList sip_cloneSipCommonHeaderList
/*******************************************************************************
** FUNCTION: __sip_cloneSipAuthorizationHeader
** DESCRIPTION:makes a deep copy of the source SipAuthorizationHeader to the 
** destination SipAuthorizationHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipAuthorizationHeader 
**			pSource(IN)		- Source SipAuthorizationHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipAuthorizationHeader sip_cloneSipCommonHeader
#endif	
/*******************************************************************************
** FUNCTION: __sip_cloneSipStringList
** DESCRIPTION:makes a deep copy of the source SipListof Stringt o the 
** destination SipList of String
** PARAMETERS:
**			pDest(OUT)		- Destination SipList (of strings)
**			pSource(IN)		- Source SipList (of strings) 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool __sip_cloneSipStringList(SipList *dest, SipList *source, \
	SipError *err);
/*******************************************************************************
** FUNCTION:sip_bcpt_cloneIsupMessage
** DESCRIPTION:makes a deep copy of the source IsupMessage to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination IsupMessage 
**			pSource(IN)		- Source IsupMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_ISUP
extern SipBool sip_bcpt_cloneIsupMessage(IsupMessage *pDest, IsupMessage \
	*pSource, SipError *pErr);
#endif	
/*******************************************************************************
** FUNCTION:validateSipContactType
** DESCRIPTION:   This fucntion returns SipSuccess if "dType" is
** one among the defined en_ContactType's; else it returns SipFail.
** PARAMETERS:
**			dType(IN)		- Contact Type to be validated
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern SipBool validateSipContactType(en_ContactType dType, SipError *err);
/*******************************************************************************
** FUNCTION:__sip_cloneIsupMessage
** DESCRIPTION:makes a deep copy of the source IsupMessage to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination IsupMessage 
**			pSource(IN)		- Source IsupMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#define __sip_cloneSipAllowEventsHeaderList sip_cloneSipCommonHeaderList
/*******************************************************************************
** FUNCTION:sip_bcpt_cloneMimeMessage
** DESCRIPTION:makes a deep copy of the source MimeMessage to the destination 
** MimeMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination MimeMessage 
**			pSource(IN)		- Source MimeMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

#ifdef SIP_MIME_PARSING
extern SipBool sip_bcpt_cloneMimeMessage _ARGS_((MimeMessage *pDest,\
	MimeMessage *pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION:sip_bcpt_cloneSipMimeHeader
** DESCRIPTION:makes a deep copy of the source SipMimeHeader to the destination 
** SipMimeHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMimeHeader 
**			pSource(IN)		- Source SipMimeHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
extern 	SipBool sip_bcpt_cloneSipMimeHeader _ARGS_((SipMimeHeader *pDest, \
	SipMimeHeader *pSource, SipError *pErr));
#endif	
/*******************************************************************************
** FUNCTION:__sip_rpr_cloneSipRAckHeader
** DESCRIPTION:makes a deep copy of the source SipRackHeader to the destination 
** SipRackHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipRackHeader 
**			pSource(IN)		- Source SipRackHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_RPR
#define __sip_rpr_cloneSipRAckHeader sip_cloneSipCommonHeader
#endif	
/*******************************************************************************
** FUNCTION:__sip_cloneSipRseqHeader
** DESCRIPTION:makes a deep copy of the source SipRseqHeader to the destination 
** SipRseqHeader
** PARAMETERS:
**			pDest(OUT)		- Destination SipRseqHeader 
**			pSource(IN)		- Source SipRseqHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_RPR
#define __sip_rpr_cloneSipRSeqHeader sip_cloneSipCommonHeader
#endif	
/*******************************************************************************
** FUNCTION:__sip_bcpt_cloneSipMimeHeader
** DESCRIPTION:makes a deep copy of the source SipMimeHeader to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMimeHeader 
**			pSource(IN)		- Source SipMimeHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_MIME_PARSING
extern SipBool __sip_bcpt_cloneSipMimeHeader _ARGS_((SipMimeHeader *pDest, \
	SipMimeHeader *pSource, SipError *pErr));
#endif	
/*******************************************************************************
** FUNCTION:__sip_cloneSipEventHeader
** DESCRIPTION:makes a deep copy of the source SipEventHeader to the destination
** SipEventHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipEventHeader 
**			pSource(IN)		- Source SipEventHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_IMPP
#define __sip_cloneSipEventHeader sip_cloneSipCommonHeader
#define  __sip_impp_cloneSipEventHeader   sip_cloneSipCommonHeader 	
#endif
#ifdef SIP_TEL
/*********************************************************
** FUNCTION:sip_cloneTelUrl
**
** DESCRIPTION:  This function makes a deep copy of the fileds from 
**	the  TelUrl structures "from" to "to".
** Parameters:
**	to (OUT)		- TelUrl								
**	from (IN)		- TelUrl which has to be cloned
**	pErr (OUT)		- Possible Error value (see API ref doc)
**
**********************************************************/

extern SipBool sip_cloneTelUrl _ARGS_ ((TelUrl *to,\
	TelUrl *from, SipError *err));
/*********************************************************
** FUNCTION:__sip_cloneTelGlobalNum
**
** DESCRIPTION:  This function makes a deep copy of the fileds from 
**	the  TelGlobalNum structures "from" to "to".
**
**********************************************************/
extern SipBool __sip_cloneTelGlobalNum _ARGS_ ((TelGlobalNum *to, \
	TelGlobalNum *from, SipError *pErr));

/*********************************************************
** FUNCTION:__sip_cloneTelLocalNum
**
** DESCRIPTION:  This function makes a deep copy of the fileds from 
**	the  TelLocalNum structures "from" to "to".
**
**********************************************************/
extern SipBool __sip_cloneTelLocalNum _ARGS_ ((TelLocalNum *to, \
	TelLocalNum *from, SipError *pErr));
/*********************************************************
** FUNCTION:__sip_cloneTelUrl
**
** DESCRIPTION:  This function makes a deep copy of the fileds from
**	the  TelUrl structures "from" to "to".
** Parameters:
**	to (OUT)		- TelUrl
**	from (IN)		- TelUrl which has to be cloned
**	pErr (OUT)		- Possible Error value (see API ref doc)
**
**
**********************************************************/
SipBool __sip_cloneTelUrl _ARGS_ ((TelUrl *to, TelUrl *from,\
	SipError *pErr));
	
#endif
#ifdef SIP_IMURL
/*********************************************************
** FUNCTION:sip_cloneImUrl
**
** DESCRIPTION:  This function makes a deep copy of the fileds from 
**	the ImUrl structures "from" to "to".
** Parameters:
**	to (OUT)		- ImUrl
**	from (IN)		- ImUrl which has to be cloned
**	pErr (OUT)		- Possible Error value (see API ref doc)
**
**********************************************************/

extern SipBool sip_cloneImUrl _ARGS_ ((ImUrl *pTo,\
	ImUrl *pFrom, SipError *pErr));

#define sip_clonePresUrl sip_cloneImUrl

#endif

#ifdef SIP_REPLACES
#define __sip_cloneSipReplacesHeader sip_cloneSipCommonHeader
#endif
#ifdef SIP_SESSIONTIMER
#define __sip_cloneSipSessionExpiresHeader sip_cloneSipCommonHeader
#define __sip_cloneSipMinSEHeader sip_cloneSipCommonHeader
#endif

#ifdef SIP_IMPP
#define __sip_cloneSipSubscriptionStateHeader sip_cloneSipCommonHeader
#define __sip_impp_cloneSipSubscriptionStateHeader sip_cloneSipCommonHeader
#endif

#define __sip_cloneSipMaxForwardsHeader  sip_cloneSipCommonHeader



/******************************************************************
**
** FUNCTION:  __sipParser_cloneSipStringList
**
** DESCRIPTION:  This function makes a deep copy of a 
** SipList of SIP_S8bit *  from the "source" to "dest".
**
******************************************************************/
SipBool __sipParser_cloneSipStringList _ARGS_ ((SipList *dest, \
	SipList *source, SipError *err));

#ifdef SIP_DATE
/********************************************************************
**
** FUNCTION:  validateSipExpiresType
**
** DESCRIPTION:  This function validates the pValue of "dType"
**
********************************************************************/

extern SipBool validateSipExpiresType _ARGS_ ((en_ExpiresType dType, \
	SipError *err));
#endif

#ifdef SIP_CORE_STACK_COMPATIBLE
/*****************************************************************************
** FUNCTION:__sip_cloneSipNameValuePair
**
** DESCRIPTION:
**
**
******************************************************************************/
extern SipBool __sip_cloneSipNameValuePair _ARGS_
      ((SipNameValuePair * dest, SipNameValuePair *src, SipError *err));

extern SipBool sip_cloneSipNameValuePair _ARGS_
        ((SipNameValuePair *pDest, SipNameValuePair *pSource, SipError *pErr));


#ifdef SIP_3GPP

/*******************************************************************************
** FUNCTION:__sipParser_cloneMsrpUrl
** DESCRIPTION:makes a deep copy of the source MsrpUrl to the destination 
** 
** PARAMETERS:
**			pDest(OUT)		- Destination MsrpUrl 
**			pSource(IN)		- Source MsrpUrl
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

extern SipBool __sipParser_cloneMsrpUrl (MsrpUrl *to,MsrpUrl *from,SipError *err);

/*******************************************************************************
** FUNCTION:__sipParser_cloneMsrpUrl
** DESCRIPTION:makes a deep copy of the source MsrpUrl to the destination 
** 
** PARAMETERS:
**			pDest(OUT)		- Destination MsrpUrl 
**			pSource(IN)		- Source MsrpUrl
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

extern SipBool sip_cloneMsrpUrl (MsrpUrl *pDest, MsrpUrl *pSource, SipError *pErr);


#endif




#endif /* SIP_CORE_STACK_COMPATIBLE */


#ifdef SIP_CONGEST
#define __sip_cloneSipRsrcPriorityHeader sip_cloneSipCommonHeader
#define __sip_cloneSipAcceptRsrcPriorityHeader sip_cloneSipCommonHeader
#define __sip_cloneSipRsrcPriorityHeaderList sip_cloneSipCommonHeaderList
#define __sip_cloneSipAcceptRsrcPriorityHeaderList sip_cloneSipCommonHeaderList
#endif

#endif /* __MICROSIP_PARSER_CLONE_H_ */

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

