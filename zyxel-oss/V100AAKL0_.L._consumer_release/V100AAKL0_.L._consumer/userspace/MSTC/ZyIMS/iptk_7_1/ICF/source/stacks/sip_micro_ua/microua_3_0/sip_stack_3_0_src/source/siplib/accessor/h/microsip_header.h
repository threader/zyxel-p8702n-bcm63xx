
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/***********************************************************************
** FUNCTION:
**			Header Accessor API functionality
************************************************************************
**
** FILENAME:
** microsip_header.h
**
** DESCRIPTION:
**		This file contains the accessor functions for the various SIP
**		headers supported by the microSIP stack.
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 9/09/2002	Aparna Kuppachi		-			Initial					
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/

#ifndef __MICROSIP_HEADER_H__
#define __MICROSIP_HEADER_H__

#include "microsip_struct.h"
#include "microsip_common.h"
#include "microsip_portlayer.h"

/****************************************************************************
** Common header functions:
****************************************************************************/

/* Common header fintions for accessing U32bit data */
extern SipBool sip_getdIntVar1FromCommonHdr _ARGS_ ((SipHeader *pHdr,
            SIP_U32bit  *pIntVar1, 
            SipError    *pErr));
extern SipBool sip_setdIntVar1InCommonHdr _ARGS_ ((SipHeader *pHdr,
            SIP_U32bit  dIntVar1, 
            SipError    *pErr));
extern SipBool sip_getdIntVar2FromCommonHdr _ARGS_ ((SipHeader *pHdr,
            SIP_U32bit  *pIntVar2, 
            SipError    *pErr));
extern SipBool sip_setdIntVar2InCommonHdr _ARGS_ ((SipHeader *pHdr,
            SIP_U32bit  dIntVar2, 
            SipError    *pErr));

/* Common header funtions for accessing string data */
extern SipBool sip_getStr1FromCommonHdr _ARGS_ ((SipHeader *pHdr, 
            SIP_S8bit   **ppStr1, 
            SipError    *pErr));
extern SipBool sip_setStr1InCommonHdr _ARGS_ ((SipHeader *pHdr,
            SIP_S8bit   *pStr1, 
            SipError    *pErr));
extern SipBool sip_getStr2FromCommonHdr _ARGS_ ((SipHeader *pHdr,
            SIP_S8bit   **ppStr2, 
            SipError    *pErr));
extern SipBool sip_setStr2InCommonHdr _ARGS_ ((SipHeader *pHdr,
            SIP_S8bit   *pStr2, 
            SipError    *pErr));

/* Common header funtions for accessing parameter list */
extern SipBool sip_getParamCountFromCommonHdr _ARGS_ ((SipHeader    *pHdr, 
            SIP_U32bit  *pCount, 
            SipError    *pErr));
extern SipBool sip_getParamCountForNameFromCommonHdr _ARGS_ ((SipHeader    *pHdr, 
            SIP_S8bit   *pName,
            SIP_U32bit  *pCount, 
            SipError    *pErr));
extern SipBool sip_getParamAtIndexFromCommonHdr _ARGS_ ((SipHeader  *pHdr,
            SipParam    **ppParam,
            SIP_U32bit  index, 
            SipError    *pErr));
extern SipBool sip_getParamForNameFromCommonHdr _ARGS_ ((SipHeader  *pHdr,
            SIP_S8bit   *pName, 
            SIP_S8bit   **pValue,
            SipError    *pErr));
extern SipBool sip_setParamAtIndexInCommonHdr _ARGS_ ((SipHeader 	*pHdr,
            SipParam    *pParam, 
            SIP_U32bit  index, 
            SipError    *pErr));
extern SipBool sip_setParamForNameInCommonHdr _ARGS_ ((SipHeader  *pHdr,
            SIP_S8bit   *pName, 
            SIP_S8bit   *pValue,
            SipError    *pErr));
extern SipBool sip_insertParamAtIndexInCommonHdr _ARGS_ ((SipHeader *pHdr, 
            SipParam    *pParam,
            SIP_U32bit  index,
            SipError    *pErr));
extern SipBool sip_deleteParamAtIndexInCommonHdr _ARGS_ ((SipHeader *pHdr,
            SIP_U32bit  index,
            SipError    *pErr));
extern SipBool sip_deleteParamForNameInCommonHdr _ARGS_ ((SipHeader *pHdr,
            SIP_S8bit   *pName, 
            SipError    *pErr));
#ifdef SIP_CORE_STACK_COMPATIBLE
extern SipBool sip_getNameValuePairAtIndexFromCommonHdr _ARGS_ ((SipHeader *pHdr,
            SipNameValuePair **ppNameValue,
            SIP_U32bit       index,
            SipError         *pErr));
extern SipBool sip_setNameValuePairAtIndexInCommonHdr _ARGS_ ((SipHeader *pHdr,
            SipNameValuePair *pNameValue,
            SIP_U32bit       dIndex,
            SipError         *pErr));
extern SipBool sip_insertNameValuePairAtIndexInCommonHdr _ARGS_ ((SipHeader *pHdr, 
            SipNameValuePair    *pNameValue, 
            SIP_U32bit          dIndex, 
            SipError            *pErr));

#define sip_deleteNameValuePairAtIndexInCommonHdr  sip_deleteParamAtIndexInCommonHdr
#endif
/* Common header funtions for Address Spec */

extern SipBool sip_getAddrSpecFromCommonHdr _ARGS_ ((SipHeader *pHdr,
            SipAddrSpec **ppAddrSpec,
            SipError    *pErr));
extern SipBool sip_setAddrSpecInCommonHdr _ARGS_ ((SipHeader *pHdr,
            SipAddrSpec *pAddrSpec,
            SipError    *pErr));

/* Common header funtions for accessing date struct */
#ifdef SIP_DATE
extern SipBool sip_getDateFromCommonHdr _ARGS_((SipHeader *hdr,
            SipDateStruct   **ppDate,
            SipError        *pErr)) ;
extern SipBool sip_setDateInCommonHdr _ARGS_(( SipHeader *hdr,
            SipDateStruct   *pDate,
            SipError        *pErr));
#endif
	
/****************************************************************************
** FUNCTION: sip_getLengthFromContentLengthHdr
** DESCRIPTION: This function gets the dLength field from Content-Length Header. 
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Length SipHeader
**				pLength(OUT)	- The retrieved length field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getLengthFromContentLengthHdr sip_getdIntVar1FromCommonHdr

/****************************************************************************
** FUNCTION: sip_setLengthInContentLengthHdr
** DESCRIPTION: This function sets the dLength field in Content-Length Header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Length SipHeader
**				dLength(IN)	- The length field to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setLengthInContentLengthHdr sip_setdIntVar1InCommonHdr

/****************************************************************************
** FUNCTION: sip_getMediaTypeFromContentTypeHdr
** DESCRIPTION: This function gets the media type field from Content-Type Header. 
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Type SipHeader
**				pType(OUT)	- The retrieved media type
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getMediaTypeFromContentTypeHdr sip_getStr1FromCommonHdr

/****************************************************************************
** FUNCTION: sip_setMediaTypeInContentTypeHdr
** DESCRIPTION: This function sets the media type into Content-Type Header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Type SipHeader
**				pType(IN)		- The string to be set as media type
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setMediaTypeInContentTypeHdr sip_setStr1InCommonHdr

/****************************************************************************
** FUNCTION: sip_getParamCountFromContentTypeHdr
** DESCRIPTION: This function gets the number of SipParam structures present 
**				in the ContentTypeHdr. 
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Length SipHeader
**				pLength(OUT)	- The retrieved count
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getParamCountFromContentTypeHdr sip_getParamCountFromCommonHdr

/****************************************************************************
** FUNCTION: sip_getParamAtIndexFromContentTypeHdr
** DESCRIPTION: This function gets the SipParam structure at a specified index 
** in the ContentTypeHdr.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Type SipHeader
**				ppParam(OUT)	- The retrieved SipParam 
**				index(IN)		- The index to retrieve from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getParamAtIndexFromContentTypeHdr sip_getParamAtIndexFromCommonHdr

/****************************************************************************
** FUNCTION: sip_setParamAtIndexInContentTypeHdr
** DESCRIPTION: This function sets the SipParam structure at a specified index
**				in the ContentTypeHdr.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Type SipHeader
**				pParam(In)		- The SipParam to be set 
**				index(IN)		- The index at which to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setParamAtIndexInContentTypeHdr sip_setParamAtIndexInCommonHdr                 

/****************************************************************************
** FUNCTION: sip_insertParamAtIndexInContentTypeHdr
** DESCRIPTION: This function inserts the SipParam structure at a specified 
**				index in the ContentTypeHdr.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Type SipHeader
**				pParam(IN)		- The SipParam to be retrieved
**				index(IN)		- The index at which to insert
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_insertParamAtIndexInContentTypeHdr sip_insertParamAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_deleteParamAtIndexInContentTypeHdr
** DESCRIPTION: This function deletes a SipParam structure at a specified index 
**				in the ContentTypeHdr.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Type SipHeader
**				index(In)		- The index from which to delete
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_deleteParamAtIndexInContentTypeHdr sip_deleteParamAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_getMediaRangeFromAcceptHdr
** DESCRIPTION: This function retrieves the Media Range from a SIP Accept 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Accept SipHeader
**				ppMediaRange(OUT)	- The retrieved MediaRange Parameter
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getMediaRangeFromAcceptHdr sip_getStr1FromCommonHdr


/****************************************************************************
** FUNCTION: sip_setMediaRangeInAcceptHdr
** DESCRIPTION: This function retrieves the Accept Range from a SIP Accept 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Accept SipHeader
**				pMediaRange(IN)	- The MediaRange Parameter to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setMediaRangeInAcceptHdr sip_setStr1InCommonHdr

/****************************************************************************
** FUNCTION: sip_getAcceptRangeFromAcceptHdr
** DESCRIPTION: This function retrieves the Accept Param from a SIP Accept 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Accept SipHeader
**				ppAcceptRange(OUT)	- The retrieved AcceptRange Parameter
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getAcceptRangeFromAcceptHdr sip_getStr2FromCommonHdr

/****************************************************************************
** FUNCTION: sip_setAcceptRangeInAcceptHdr
** DESCRIPTION: This function retrieves the Accept Param from a SIP Accept 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Accept SipHeader
**				pAcceptRange(IN)	- The AcceptRange Parameter to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setAcceptRangeInAcceptHdr sip_setStr2InCommonHdr


/****************************************************************************
** FUNCTION: sip_getDispNameFromContactHdr
** DESCRIPTION: This function retrieves the display pName from a SIP Contact 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Contact SipHeader
**				ppDispName(OUT)	- The retrieved display name field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getDispNameFromContactHdr sip_getStr1FromCommonHdr

/****************************************************************************
** FUNCTION: sip_setDispNameInContactHdr
** DESCRIPTION: This function sets the display pName field in a SIP Contact 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Contact SipHeader
**				pDispName(IN)	- The display name to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setDispNameInContactHdr sip_setStr1InCommonHdr

/****************************************************************************
** FUNCTION: sip_getAddrSpecFromContactHdr
** DESCRIPTION: This function retrieves the dAddr-spec field from a
**				SIP contact header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Contact SipHeader
**				pLength(OUT)	- The retrieved addr-spec
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getAddrSpecFromContactHdr sip_getAddrSpecFromCommonHdr

/****************************************************************************
** FUNCTION: sip_setAddrSpecInContactHdr
** DESCRIPTION: This function sets the dAddr-spec field in a SIP
**				Contact header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Contact SipHeader
**				pLength(OUT)	- The addr-spec to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setAddrSpecInContactHdr sip_setAddrSpecInCommonHdr 

/****************************************************************************
** FUNCTION: sip_getDispNameFromFromHdr
** DESCRIPTION: This function retrieves the display name field
**				from a SIP From header
**				 
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				ppDispName(OUT)	- The retrieved display name
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getDispNameFromFromHdr sip_getStr1FromCommonHdr             

/****************************************************************************
** FUNCTION: sip_setDispNameInFromHdr
** DESCRIPTION: This function sets the display name field in a
**				SIP From header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				pDispName(IN)	- The display name to be set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setDispNameInFromHdr sip_setStr1InCommonHdr

#ifdef SIP_CORE_STACK_COMPATIBLE
/****************************************************************************
** FUNCTION: sip_getTagCountFromFromHdr
** DESCRIPTION: This function gets the number of slTag inserted in a
**				From header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				pCount(OUT)		- The retrieved count field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getTagCountFromFromHdr(pHdr,pCount,pErr) \
            sip_getParamCountForNameFromCommonHdr(pHdr,"tag",pCount,pErr)
#endif
        
/****************************************************************************
** FUNCTION: sip_getTagAtIndexFromFromHdr
** DESCRIPTION: This function retrieves a tag at a specified index
**				in a From header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				ppTag(OUT)	- The retrieved tag field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getTagAtIndexFromFromHdr(pHdr,ppTag,index,pErr) \
            sip_getParamForNameFromCommonHdr(pHdr,"tag",ppTag,pErr)

/****************************************************************************
** FUNCTION: sip_insertTagAtIndexInFromHdr
** DESCRIPTION: This function inserts a tag at a specified index in a From 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				pTag(IN)		- The tag to be inserted
**				index(IN)		- The index at which to insert
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_insertTagAtIndexInFromHdr(pHdr,pTag,index,pErr) \
            sip_setParamForNameInCommonHdr(pHdr,"tag",pTag,pErr)

/****************************************************************************
** FUNCTION: sip_setTagAtIndexInFromHdr
** DESCRIPTION: This function sets a tag at a specified index in a
**				From header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				pTag(IN)		- The tag to be set
**				index(IN)		- The index at which to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setTagAtIndexInFromHdr(pHdr,pTag,index,pErr) \
            sip_setParamForNameInCommonHdr(pHdr,"tag",pTag,pErr)
/*
 * In case of Ftom and To tags, setTagAtIndex and InsertTagAtIndex are
 * treated as equivalent since the stack is transparent to any tag
 * params after the first one.
 */

/****************************************************************************
** FUNCTION: sip_deleteTagAtIndexInFromHdr
** DESCRIPTION: This function deletes a tag at a specified index in a From
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				index(IN)		- The index from which to delete
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_deleteTagAtIndexInFromHdr(pHdr,index,pErr) \
            sip_deleteParamForNameInCommonHdr(pHdr,"tag",pErr)

#define sip_deleteTagAtIndexInToHdr(pHdr,index,pErr) \
            sip_deleteParamForNameInCommonHdr(pHdr,"tag",pErr)

/****************************************************************************
** FUNCTION: sip_getExtensionParamCountFromFromHdr
** DESCRIPTION: This function gets the number of extension-pParam
**				from a From header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				pCount(OUT)		- The retrieved count
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getExtensionParamCountFromFromHdr sip_getParamCountFromCommonHdr

/****************************************************************************
** FUNCTION: sip_getExtensionParamAtIndexFromFromHdr
** DESCRIPTION: This function gets an extension-param at a specified
**				index from a From header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				ppParam(OUT)	- The retrieved extn param
**				index(IN)		- The index from which to retrieve
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getExtensionParamAtIndexFromFromHdr sip_getParamAtIndexFromCommonHdr

/****************************************************************************
** FUNCTION: sip_insertExtensionParamAtIndexInFromHdr
** DESCRIPTION: This function inserts an extension-param at a specified index
**				in a From header.

** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				pParam(IN)		- The param to be inserted
**				index(IN)		- The index at which to insert
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_insertExtensionParamAtIndexInFromHdr sip_insertParamAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_setExtensionParamAtIndexInFromHdr
** DESCRIPTION: This function sets an extension-param at a specified
**				index in a From header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				pParam(IN)		- The param to be set
**				index(IN)		- The index at which to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setExtensionParamAtIndexInFromHdr sip_setParamAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_deleteExtensionParamAtIndexInFromHdr
** DESCRIPTION: This function deletes an extension-param at a specified index 
**				in a From header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				index(IN)		- The index from which to delete
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_deleteExtensionParamAtIndexInFromHdr sip_deleteParamAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_getAddrSpecFromFromHdr
** DESCRIPTION: This function retrieves the addr-spec field from a
**				 From header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				pLength(OUT)	- The retrieved addr-spec
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getAddrSpecFromFromHdr sip_getAddrSpecFromCommonHdr

/****************************************************************************
** FUNCTION: sip_setAddrSpecInFromHdr
** DESCRIPTION: This function sets the addr-spec field in a From header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				pLength(OUT)	- The addr-spec to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setAddrSpecInFromHdr sip_setAddrSpecInCommonHdr

/****************************************************************************
** FUNCTION: sip_getTimeFromTimeStampHdr
** DESCRIPTION: This function gets the slTime from a Timestamp header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				slTime(OUT)		- The retrieved time field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#ifdef SIP_TIMESTAMP			
#define sip_getTimeFromTimeStampHdr sip_getStr1FromCommonHdr
	
/****************************************************************************
** FUNCTION: sip_setTimeInTimeStampHdr
** DESCRIPTION: This function sets the slTime in a TimeStamp header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the From SipHeader
**				pLength(OUT)	- The time field to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setTimeInTimeStampHdr sip_setStr1InCommonHdr

/****************************************************************************
** FUNCTION: sip_getDelayFromTimeStampHdr
** DESCRIPTION: This function retrieves the delay field from a
**				Timestamp header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Timestamp SipHeader
**				pLength(OUT)	- The retrieved delay field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
            
#define sip_getDelayFromTimeStampHdr sip_getStr2FromCommonHdr

/****************************************************************************
** FUNCTION: sip_setDelayInTimeStampHdr
** DESCRIPTION: This function sets the delay field in a Timestamp header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Timestamp SipHeader
**				delay(IN)	- The delay field to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setDelayInTimeStampHdr sip_setStr2InCommonHdr
#endif
/****************************************************************************
** FUNCTION: sip_getTypeFromContactHdr
** DESCRIPTION: This function retrieves the dType of a Contact header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Contact SipHeader
**				dType(OUT)	- The retrieved dType
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getTypeFromContactHdr sip_getdIntVar1FromCommonHdr

/****************************************************************************
** FUNCTION: sip_setTypeInContactHdr
** DESCRIPTION: This function sets the dType of a Contact header 
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Contact SipHeader
**				dType(IN)		- The dType to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setTypeInContactHdr sip_setStr1InCommonHdr
	
/****************************************************************************
** FUNCTION: sip_getDispTypeFromContentDispositionHdr
** DESCRIPTION: This function retrieves the display type from the 
**				Content-Disposition header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Disposition SipHeader
**				ppDispType(OUT)	- The retrieved disp type
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getDispTypeFromContentDispositionHdr sip_getStr1FromCommonHdr

/****************************************************************************
** FUNCTION: sip_setDispTypeInContentDispositionHdr
** DESCRIPTION: This function sets the display type into the Content-Disposition 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Disposition SipHeader
**				pDispType(IN)	- The disp type to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setDispTypeInContentDispositionHdr sip_setStr1InCommonHdr
	
/****************************************************************************
** FUNCTION: sip_getParamCountFromContentDispositionHdr
** DESCRIPTION: This function retrieves the number of parameters from a
**				Content-Disposition header.
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Disposition SipHeader
**				pCount(OUT)		- The count of the number of params
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getParamCountFromContentDispositionHdr sip_getParamCountFromCommonHdr

/****************************************************************************
** FUNCTION: sip_getParamAtIndexFromContentDispositionHdr
** DESCRIPTION: This function retrieves the parameter at a specified
**				 index from a Content-Disposition header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Disposition SipHeader
**				ppParam(OUT)	- The retrieved parameter
**				index(IN)		- The index to retrive from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getParamAtIndexFromContentDispositionHdr sip_getParamAtIndexFromCommonHdr

/****************************************************************************
** FUNCTION: sip_insertParamAtIndexInContentDispositionHdr
** DESCRIPTION: This function inserts a parameter at a specified
**				index in a Content-Disposition header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Disposition SipHeader
**				pParam(IN)		- The parameter to be inserted
**				index(IN)		- The index to insert at
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_insertParamAtIndexInContentDispositionHdr sip_insertParamAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_deleteParamAtIndexInContentDispositionHdr
** DESCRIPTION: This function deletes a parameter at a specified
**				index in a Content-Disposition header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Disposition SipHeader
**				index(IN)		- The index to delete from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_deleteParamAtIndexInContentDispositionHdr sip_deleteParamAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_setParamAtIndexInContentDispositionHdr
** DESCRIPTION: This function sets a parameter at a specified
**				index in a Content-Disposition header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Disposition SipHeader
**				pParam(IN)		- The param to set
**				index(IN)		- The index to set at
**				pErr(OUT)		- Error value if any
**
** NOTE: 
****************************************************************************/

#define sip_setParamAtIndexInContentDispositionHdr sip_setParamAtIndexInCommonHdr

#ifdef SIP_WARNING
/****************************************************************************
** FUNCTION: sip_getCodeFromWarningHdr
** DESCRIPTION: This function gets the dCodeNum field from a Warning header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Warning SipHeader
**				pCodeNum(OUT)	- The retrieved dCodeNum field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

        /* U16bit dCodeNum changed to U32bit */
#define sip_getCodeFromWarningHdr sip_getdIntVar1FromCommonHdr 
	
/****************************************************************************
** FUNCTION: sip_setCodeInWarningHdr
** DESCRIPTION: This function sets the dCodeNum field in a Warning header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Warning SipHeader
**				dCodeNum(IN)	- The dCodeNum to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setCodeInWarningHdr sip_setdIntVar1InCommonHdr

/****************************************************************************
** FUNCTION: sip_getAgentFromWarningHdr
** DESCRIPTION: This function gets the pAgent field from a Warning header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Warning SipHeader
**				ppAgent(OUT)	- The retrieved pAgent field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getAgentFromWarningHdr sip_getStr1FromCommonHdr 

/****************************************************************************
** FUNCTION: sip_setAgentInWarningHdr
** DESCRIPTION: This function sets the pAgent field in a Warning header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Warning SipHeader
**				pAgent(IN)		- The pAgent field to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setAgentInWarningHdr sip_setStr1InCommonHdr
	
/****************************************************************************
** FUNCTION: sip_getTextFromWarningHdr
** DESCRIPTION: This function gets the warning text field from a Warning hdr.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Warning SipHeader
**				ppText(OUT)		- The retrieved warning text
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getTextFromWarningHdr sip_getStr2FromCommonHdr 

/****************************************************************************
** FUNCTION: sip_setTextInWarningHdr
** DESCRIPTION: This function sets the Warning text into a Warning header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Warning SipHeader
**				pText(IN)		- The warning text to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setTextInWarningHdr sip_setStr2InCommonHdr

#endif
#ifdef SIP_SESSIONTIMER
#ifdef SIP_CORE_STACK_COMPATIBLE

/****************************************************************************
** FUNCTION: sip_getNameValuePairCountFromMinSEHdr
** DESCRIPTION: This function retrieves the number of NameValuePairs from an
**				MinSE header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the MinSE SipHeader
**				pCount(OUT)		- The retrieved count
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_getNameValuePairCountFromMinSEHdr sip_getParamCountFromCommonHdr

/****************************************************************************
** FUNCTION: sip_getNameValuePairAtIndexFromMinSEHdr
** DESCRIPTION: This function retrieves NameValuePair at specified index
**				from MinSE Header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the MinSE SipHeader
**				ppNameValue(IN)	- The retreived name-value pair
**				index(IN		- The index to retrieve from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getNameValuePairAtIndexFromMinSEHdr sip_getNameValuePairAtIndexFromCommonHdr

/****************************************************************************
** FUNCTION: sip_setNameValuePairAtIndexInMinSEHdr
** DESCRIPTION: This function sets the NameValuePair at specified index
**				in MinSE header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Warning SipHeader
**				pNameValue(IN)	- The name value pair to set
**				dIndex(IN)		- The index at which to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setNameValuePairAtIndexInMinSEHdr sip_setNameValuePairAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_insertNameValuePairAtIndexInMinSEHdr
** DESCRIPTION: This function inserts NameValuePair at specified index
**				in MinSE Header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the MinSE SipHeader
**				pNameValue(IN)	- The name value pair to insert
**				dIndex(IN)		- The index at which to insert
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_insertNameValuePairAtIndexInMinSEHdr sip_insertNameValuePairAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_deleteNameValuePairAtIndexInMinSEHdr
** DESCRIPTION: This function deletes NameValuePair at specified index
**				in MinSE header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the MinSE SipHeader
**				dIndex(IN)		- The index to delete from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_deleteNameValuePairAtIndexInMinSEHdr sip_deleteParamAtIndexInCommonHdr

/* APIs for Session-Expires header */

#define	sip_getNameValuePairCountFromSessionExpiresHdr \
        	sip_getParamCountFromCommonHdr
#define	sip_getNameValuePairAtIndexFromSessionExpiresHdr \
            sip_getNameValuePairAtIndexFromCommonHdr
#define sip_setNameValuePairAtIndexInSessionExpiresHdr \
            sip_setNameValuePairAtIndexInCommonHdr
#define sip_insertNameValuePairAtIndexInSessionExpiresHdr \
            sip_insertNameValuePairAtIndexInCommonHdr
#define sip_deleteNameValuePairAtIndexInSessionExpiresHdr \
            sip_deleteParamAtIndexInCommonHdr
#endif /* SIP_CORE_STACK_COMPATIBLE */

#endif /* SIP_SESSIONTIMER */

#ifdef SIP_REPLACES
/****************************************************************************
** FUNCTION: sip_getFromTagFromReplacesHdr
** DESCRIPTION: This function retrieves the from-tag field from a Replaces 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				ppFromTag(OUT)	- The retrieved from-tag
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
extern SipBool sip_getFromTagFromReplacesHdr _ARGS_ ((SipHeader *pHdr, \
	SIP_S8bit **ppFromTag, SipError *pErr));

/****************************************************************************
** FUNCTION: sip_setFromTagInReplacesHdr
** DESCRIPTION: This function sets the from-tag in a Replaces header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				pFromTag(IN)	- The from-tag value to be set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setFromTagInReplacesHdr(pHdr,pFromTag,pErr) \
            sip_setParamForNameInCommonHdr(pHdr,"from-tag",pFromTag,pErr) 

/****************************************************************************
** FUNCTION: sip_getToTagFromReplacesHdr
** DESCRIPTION: This function retrieves the to-tag field from a Replaces 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				ppToTag(OUT)	- The retrieved to-tag
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool sip_getToTagFromReplacesHdr _ARGS_
	((SipHeader *pHdr, SIP_S8bit **ppToTag, SipError *pErr));

/****************************************************************************
** FUNCTION: sip_setToTagInReplacesHdr
** DESCRIPTION: This function sets the to-tag in a Replaces header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				pToTag(IN)		- The to-tag value to be set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setToTagInReplacesHdr(pHdr,pToTag,pErr) \
            sip_setParamForNameInCommonHdr(pHdr,"to-tag",pToTag,pErr)

#ifdef SIP_CORE_STACK_COMPATIBLE
/****************************************************************************
** FUNCTION: sip_getNameValuePairCountFromReplacesHdr
** DESCRIPTION: This function retrieves the number of NameValuePairs from an
**				Replaces header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				pCount(OUT)		- The retrieved count
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_getNameValuePairCountFromReplacesHdr \
	sip_getParamCountFromCommonHdr

/****************************************************************************
** FUNCTION: sip_getNameValuePairAtIndexFromReplacesHdr
** DESCRIPTION: This function retrieves NameValuePair at specified index
**				from Replaces Header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				ppNameValue(IN)	- The retreived name-value pair
**				index(IN		- The index to retrieve from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getNameValuePairAtIndexFromReplacesHdr \
    sip_getNameValuePairAtIndexFromCommonHdr

/****************************************************************************
** FUNCTION: sip_setNameValuePairAtIndexInReplacesHdr
** DESCRIPTION: This function sets the NameValuePair at specified index
**				in Replaces header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				pNameValue(IN)	- The name value pair to set
**				dIndex(IN)		- The index at which to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setNameValuePairAtIndexInReplacesHdr \
    sip_setNameValuePairAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_insertNameValuePairAtIndexInReplacesHdr
** DESCRIPTION: This function inserts NameValuePair at specified index
**				in Replaces Header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				pNameValue(IN)	- The name value pair to insert
**				dIndex(IN)		- The index at which to insert
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_insertNameValuePairAtIndexInReplacesHdr \
    sip_insertNameValuePairAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_deleteNameValuePairAtIndexInReplacesHdr
** DESCRIPTION: This function deletes NameValuePair at specified index
**				in Replaces header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				dIndex(IN)		- The index to delete from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_deleteNameValuePairAtIndexInReplacesHdr \
    sip_deleteParamAtIndexInCommonHdr
#endif /* SIP_CORE_STACK_COMPATIBLE */
#endif /* SIP_REPLACES */

/*****************************************************************
**
** FUNCTION:  sip_getValueFromCallIdHdr
**
** DESCRIPTION: This function retrieves the pValue field from a SIP
**		Call-Ifd Header
**
***************************************************************/
#define sip_getValueFromCallIdHdr sip_getStr1FromCommonHdr

/***********************************************************************
** FUNCTION: sip_setValueInCallIdHdr
** DESCRIPTION:sets the Value in the CallId Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip CallId Header
**			value(IN)			- value to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_setValueInCallIdHdr sip_setStr1InCommonHdr

/***********************************************************************
** FUNCTION: sip_getEncodingFromContentEncodingHdr 
** DESCRIPTION: get Encoding from Content Encoding Header
** PARAMETERS:	
**				hdr(IN)			- Sip Content Encoding Header
**				encoding(OUT)	- Content Encoding retrieved 
**				err(OUT)  		- Possible error value (See API ref doc)
************************************************************************/

#define sip_getEncodingFromContentEncodingHdr sip_getStr1FromCommonHdr
/***********************************************************************
** FUNCTION: sip_setEncodingInContentEncodingHdr 
** DESCRIPTION: set Encoding in Content Encoding Header
** PARAMETERS:	
**				hdr(IN/OUT)		- Sip Content Encoding Header
**				encoding(IN)	- Content Encoding to set 
**				err(OUT)  		- Possible error value (See API ref doc)
************************************************************************/
#define  sip_setEncodingInContentEncodingHdr  sip_setStr1InCommonHdr

/******************************************************************************
** FUNCTION: sip_bcpt_getVersionFromMimeVersionHdr  
** DESCRIPTION: get Version from Mime Version Header
** PARAMETERS:
**				pHdr(IN)		- Mime Version Header
**				ppVersion(OUT)	- Version retrieved
**				pErr(OUT)   	- Possible error value (See API ref doc)
********************************************************************************/
#define sip_bcpt_getVersionFromMimeVersionHdr sip_getStr1FromCommonHdr

/***********************************************************************
** FUNCTION: sip_setOptionInSupportedHdr
** DESCRIPTION:sets the Options field in the Sip Supported Header
** PARAMETERS:
**			hdr(IN/OUT) 				- Sip Supported Header
**			pOptions(IN)			- The Options field to set
**			err(OUT)				- Possible error value(see API ref doc)
**
***********************************************************************/
#define sip_setOptionInSupportedHdr sip_setStr1InCommonHdr


/***********************************************************************
** FUNCTION: sip_getOptionFromSupportedHdr
** DESCRIPTION:gets the Options field from the Sip Supported Header
** PARAMETERS:
**			hdr(IN) 				- Sip Supported Header
**			pOptions(OUT)			- The Options field to retrieve
**			err(OUT)				- Possible error value(see API ref doc)
**
************************************************************************/
#define  sip_getOptionFromSupportedHdr sip_getStr1FromCommonHdr

/***********************************************************************
** FUNCTION: sip_setOptionInUnsupportedHdr
** DESCRIPTION:sets the Options field in the Sip Unsupported Header
** PARAMETERS:
**			hdr(IN/OUT) 				- Sip Unsupported Header
**			pOptions(IN)			- The Options field to set
**			err(OUT)				- Possible error value(see API ref doc)
**
***********************************************************************/
#define sip_setOptionInUnsupportedHdr sip_setStr1InCommonHdr


/***********************************************************************
** FUNCTION: sip_getOptionFromUnsupportedHdr
** DESCRIPTION:gets the Options field from the Sip Unsupported Header
** PARAMETERS:
**			hdr(IN) 				- Sip Unsupported Header
**			pOptions(OUT)			- The Options field to retrieve
**			err(OUT)				- Possible error value(see API ref doc)
**
************************************************************************/
#define  sip_getOptionFromUnsupportedHdr sip_getStr1FromCommonHdr

/***********************************************************************
** FUNCTION:  sip_getMethodFromAllowHdr
** DESCRIPTION: Gets the pMethod field pValue from allow pHeader.
** PARAMETERS:
**			hdr(IN)		- Sip Agent Header
**			pMethod(OUT)- The pMethod field  got
**			pErr(OUT)	- Possible Error value (see API ref doc)
**
**********************************************************************/
#define sip_getMethodFromAllowHdr  sip_getStr1FromCommonHdr

/***********************************************************************
** FUNCTION:  sip_setMethodInAllowHdr
** DESCRIPTION: Sets the pMethod field in allow pHeader.
** PARAMETERS:
**			hdr(IN/OUT)		- Sip Agent Header
**			pMethod(IN)		- The pMethod field  set
**			pErr(OUT)		- Possible Error value (see API ref doc)
**
***********************************************************************/
#define sip_setMethodInAllowHdr sip_setStr1InCommonHdr
/***********************************************************************
** FUNCTION:  sip_impp_getEventTypeFromAllowEventsHdr
** DESCRIPTION: Gets the pEventType field from Allow Events pHeader.
** PARAMETERS:
**			hdr(IN)		- Sip Allow Events Header
**			pEventType(OUT)- The pEventType field got
**			pErr(OUT)	- Possible Error value (see API ref doc)
**
**********************************************************************/
#define sip_impp_getEventTypeFromAllowEventsHdr sip_getStr1FromCommonHdr

/***********************************************************************
** FUNCTION:  sip_impp_setEventTypeInAllowEventsHdr
** DESCRIPTION: Sets the pEventType field in Allow Events pHeader.
** PARAMETERS:
**			hdr(IN/OUT)		- Sip Allow Events Header
**			pEventType(IN)		- The pEventType field  set
**			pErr(OUT)		- Possible Error value (see API ref doc)
**
***********************************************************************/
#define sip_impp_setEventTypeInAllowEventsHdr sip_setStr1InCommonHdr
/***********************************************************************
** FUNCTION: sip_getTokenFromRequireHdr
** DESCRIPTION: gets the Tokens from the Require Header
** PARAMETERS:
**			pHdr(IN)			- Sip Require Header
**			token(OUT)			- The Token retrieved
**			pErr(OUT)			- Possible Error value (see API ref doc)
**
************************************************************************/
#define sip_getTokenFromRequireHdr sip_getStr1FromCommonHdr

/***********************************************************************
** FUNCTION: sip_setTokenInRequireHdr
** DESCRIPTION: sets the Tokens in the Require Header
** PARAMETERS:
**			pHdr(IN/OUT)		- Sip Require Header
**			token(IN)			- The Token to set
**			pErr(OUT)			- Possible Error value (see API ref doc)
**
************************************************************************/
#define sip_setTokenInRequireHdr  sip_setStr1InCommonHdr


/*****************************************************************
**
** FUNCTION:  sip_getSchemeFromAuthorizationHdr
**
** DESCRIPTION: This function retrieves the pScheme from a SIP
**		Authorization Header
**
***************************************************************/
#define sip_getSchemeFromAuthorizationHdr sip_getStr1FromCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_setSchemeInAuthorizationHdr
**
** DESCRIPTION: This function sets the pScheme field in a SIP Authorization
**		pHeader
**
***************************************************************/
#define sip_setSchemeInAuthorizationHdr sip_setStr1InCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_getParamCountFromAuthorizationHdr
**
** DESCRIPTION: This function retrieives the number of parametrs
**		from a SIP Authorization pHeader
**
**************************************************************/
#define sip_getParamCountFromAuthorizationHdr sip_getParamCountFromCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_getParamAtIndexFromAuthorizationHdr
**
** DESCRIPTION: This function retrieves a paarmeter at a specified
**		index from a SIP Authorization pHeader
**
***************************************************************/
#define sip_getParamAtIndexFromAuthorizationHdr sip_getParamAtIndexFromCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_insertParamAtIndexInAuthorizationHdr
**
** DESCRIPTION: This function inserts a parameter at a specified
**		in a SIp Authorization pHeader
**
***************************************************************/
#define sip_insertParamAtIndexInAuthorizationHdr sip_insertParamAtIndexInCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_deleteParamAtIndexInAuthorizationHeader
**
** DESCRIPTION: This function deletes a parameter at a specified
**		index in a SIP AuthorizationHeader pHeader
**
***************************************************************/
#define sip_deleteParamAtIndexInAuthorizationHdr sip_deleteParamAtIndexInCommonHdr
	
/*****************************************************************
**
** FUNCTION:  sip_setParamAtIndexInAuthorizationHdr
**
** DESCRIPTION: This function sets a parameter at a specified index
**		in a SIP AuthorizationHeader pHeader
**
***************************************************************/
#define sip_setParamAtIndexInAuthorizationHdr sip_setParamAtIndexInCommonHdr
		
/*****************************************************************
**
** FUNCTION:  sip_getSchemeFromProxyAuthorizationHdr
**
** DESCRIPTION: This function retrieves the pScheme from a SIP
**		ProxyAuthorization Header
**
***************************************************************/
#define sip_getSchemeFromProxyAuthorizationHdr sip_getStr1FromCommonHdr 
/*****************************************************************
**
** FUNCTION:  sip_setSchemeInProxyAuthorizationHdr
**
** DESCRIPTION: This function sets the pScheme field in a SIP ProxyAuthorization
**		pHeader
**
***************************************************************/
#define sip_setSchemeInProxyAuthorizationHdr sip_setStr1InCommonHdr
	
/*****************************************************************
**
** FUNCTION:  sip_getParamCountFromProxyAuthorizationHdr
**
** DESCRIPTION: This function retrieives the number of parametrs
**		from a SIP ProxyAuthorization pHeader
**
**************************************************************/
#define sip_getParamCountFromProxyAuthorizationHdr sip_getParamCountFromCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_getParamAtIndexFromAuthorizationHdr
**
** DESCRIPTION: This function retrieves a paarmeter at a specified
**		index from a SIP ProxyAuthorization pHeader
**
***************************************************************/
#define sip_getParamAtIndexFromProxyAuthorizationHdr \
    sip_getParamAtIndexFromCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_insertParamAtIndexInAuthorizationHdr
**
** DESCRIPTION: This function inserts a parameter at a specified
**		in a SIp ProxyAuthorization pHeader
**
***************************************************************/
#define  sip_insertParamAtIndexInProxyAuthorizationHdr \
    sip_insertParamAtIndexInCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_deleteParamAtIndexInProxyAuthorizationHeader
**
** DESCRIPTION: This function deletes a parameter at a specified
**		index in a SIP ProxyAuthorizationHeader pHeader
**
***************************************************************/
#define sip_deleteParamAtIndexInProxyAuthorizationHdr \
    sip_deleteParamAtIndexInCommonHdr
	
/*****************************************************************
**
** FUNCTION:  sip_setParamAtIndexInProxyAuthorizationHdr
**
** DESCRIPTION: This function sets a parameter at a specified index
**		in a SIP ProxyAuthorizationHeader pHeader
**
***************************************************************/
#define sip_setParamAtIndexInProxyAuthorizationHdr sip_setParamAtIndexInCommonHdr
	
/*****************************************************************
**
** FUNCTION:  sip_getSchemeFromWwwAuthenticateHdr
**
** DESCRIPTION: This function retrieves the pScheme from a SIP
**		WwwAuthenticate Header
**
***************************************************************/
#define sip_getSchemeFromWwwAuthenticateHdr sip_getStr1FromCommonHdr 
/*****************************************************************
**
** FUNCTION:  sip_setSchemeInWwwAuthenticateHdr
**
** DESCRIPTION: This function sets the pScheme field in a SIP WwwAuthenticate
**	pHeader
**
***************************************************************/
#define sip_setSchemeInWwwAuthenticateHdr sip_setStr1InCommonHdr
	
/*****************************************************************
**
** FUNCTION:  sip_getParamCountFromWwwAuthenticateHdr
**
** DESCRIPTION: This function retrieives the number of parametrs
**		from a SIP WwwAuthenticateHdr pHeader
**
**************************************************************/
#define sip_getParamCountFromWwwAuthenticateHdr sip_getParamCountFromCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_getParamAtIndexFromWwwAuthenticateHdr
**
** DESCRIPTION: This function retrieves a paarmeter at a specified
**		index from a SIP WwwAuthenticateHdr
**
***************************************************************/
#define sip_getParamAtIndexFromWwwAuthenticateHdr sip_getParamAtIndexFromCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_insertParamAtIndexInWwwAuthenticateHdr
**
** DESCRIPTION: This function inserts a parameter at a specified
**		in a SIp WwwAuthenticateHdr pHeader
**
***************************************************************/
#define  sip_insertParamAtIndexInWwwAuthenticateHdr \
    sip_insertParamAtIndexInCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_deleteParamAtIndexInWwwAuthenticateHdr
**
** DESCRIPTION: This function deletes a parameter at a specified
**		index in a SIP WwwAuthenticateHdr pHeader
**
***************************************************************/
#define sip_deleteParamAtIndexInWwwAuthenticateHdr \
    sip_deleteParamAtIndexInCommonHdr
	
/*****************************************************************
**
** FUNCTION:  sip_setParamAtIndexInAuthorizationHdr
**
** DESCRIPTION: This function sets a parameter at a specified index
**		in a SIP WwwAuthenticateHdr pHeader
**
***************************************************************/
#define sip_setParamAtIndexInWwwAuthenticateHdr sip_setParamAtIndexInCommonHdr
	
/*****************************************************************
**
** FUNCTION:  sip_getSchemeFromProxyAuthenticateHdr
**
** DESCRIPTION: This function retrieves the pScheme from a SIP
**		ProxyAuthenticate Header
**
***************************************************************/
#define sip_getSchemeFromProxyAuthenticateHdr sip_getStr1FromCommonHdr 
/*****************************************************************
**
** FUNCTION:  sip_setSchemeInProxyAuthenticateHdr
**
** DESCRIPTION: This function sets the pScheme field in a SIP ProxyAuthenticate
**	pHeader
**
***************************************************************/
#define sip_setSchemeInProxyAuthenticateHdr sip_setStr1InCommonHdr
	
/*****************************************************************
**
** FUNCTION:  sip_getParamCountFromProxyAuthenticateHdr
**
** DESCRIPTION: This function retrieives the number of parametrs
**		from a SIP ProxyAuthenticateHdr pHeader
**
**************************************************************/
#define sip_getParamCountFromProxyAuthenticateHdr sip_getParamCountFromCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_getParamAtIndexFromProxyAuthenticateHdr
**
** DESCRIPTION: This function retrieves a paarmeter at a specified
**		index from a SIP ProxyAuthenticateHdr
**
***************************************************************/
#define sip_getParamAtIndexFromProxyAuthenticateHdr \
    sip_getParamAtIndexFromCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_insertParamAtIndexInProxyAuthenticateHdr
**
** DESCRIPTION: This function inserts a parameter at a specified
**		in a SIp ProxyAuthenticateHdr pHeader
**
***************************************************************/
#define  sip_insertParamAtIndexInProxyAuthenticateHdr \
    sip_insertParamAtIndexInCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_deleteParamAtIndexInProxyAuthenticateHdr
**
** DESCRIPTION: This function deletes a parameter at a specified
**		index in a SIP ProxyAuthenticateHdr pHeader
**
***************************************************************/
#define sip_deleteParamAtIndexInProxyAuthenticateHdr \
    sip_deleteParamAtIndexInCommonHdr
	
/*****************************************************************
**
** FUNCTION:  sip_setParamAtIndexInAuthorizationHdr
**
** DESCRIPTION: This function sets a parameter at a specified index
**		in a SIP ProxyAuthenticateHdr pHeader
**
***************************************************************/
#define sip_setParamAtIndexInProxyAuthenticateHdr sip_setParamAtIndexInCommonHdr
	
	
/***********************************************************************
** FUNCTION: sip_getSentByFromViaHdr
** DESCRIPTION:gets the SentBy field from the Sip Via Header
** PARAMETERS:
**			hdr(IN) 			- Sip Via Header
**			sent(OUT)			- The sentBy field which is retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_getSentByFromViaHdr sip_getStr2FromCommonHdr


/***********************************************************************
** FUNCTION: sip_setSentByInViaHdr
** DESCRIPTION:sets the SentBy field in the Sip Via Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip Via Header
**			sent(IN)			- The sentBy field which is set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_setSentByInViaHdr sip_setStr2InCommonHdr

/***********************************************************************
** FUNCTION: sip_getSentProtocolFromViaHdr
** DESCRIPTION:gets the Sent Protocol from the Sip Via Header
** PARAMETERS:
**			hdr(IN) 			- Sip Via Header
**			sentProtocol(OUT)	- The sentProtocol which is retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_getSentProtocolFromViaHdr sip_getStr1FromCommonHdr


/***********************************************************************
** FUNCTION: sip_setSentProtocolInViaHdr
** DESCRIPTION:sets the Sent Protocol in the Sip Via Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip Via Header
**			sentProtocol(IN)	- The sentProtocol to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_setSentProtocolInViaHdr sip_setStr1InCommonHdr


/***********************************************************************
** FUNCTION: sip_getViaParamCountFromViaHdr
** DESCRIPTION:gets the number of Via PARAMETERS from the Sip Via Header
** PARAMETERS:
**			hdr(IN) 			- Sip Via Header
**			count(IN)			- The number of Via PARAMETERS is retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_getViaParamCountFromViaHdr sip_getParamCountFromCommonHdr

/***********************************************************************
** FUNCTION: sip_getViaParamAtIndexFromViaHdr
** DESCRIPTION:gets the Via Parameter at an index from the Sip Via Header
** PARAMETERS:
**			hdr(IN) 			- Sip Via Header
**			pViaParam(OUT)		- The Via Parameter is retrieved
**			index(IN)			- Index at which the Via Parameter is retrieved.
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_getViaParamAtIndexFromViaHdr sip_getParamAtIndexFromCommonHdr

/***********************************************************************
** FUNCTION: sip_insertViaParamAtIndexInViaHdr
** DESCRIPTION:inserts the Via Parameter at an index in the Sip Via Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip Via Header
**			pViaParam(IN)		- The Via Parameter is inserted
**			index(IN)			- Index at which the Via Parameter is inserted
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_insertViaParamAtIndexInViaHdr sip_insertParamAtIndexInCommonHdr


/***********************************************************************
** FUNCTION: sip_setViaParamAtIndexInViaHdr
** DESCRIPTION:sets the Via Parameter at an index in the Sip Via Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip Via Header
**			pViaParam(IN)		- The Via Parameter is set
**			index(IN)			- Index at which the Via Parameter is set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_setViaParamAtIndexInViaHdr sip_setParamAtIndexInCommonHdr


/***********************************************************************
** FUNCTION: sip_deleteViaParamAtIndexInViaHdr
** DESCRIPTION:deletes the Via Parameter at an index in the Sip Via Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip Via Header
**			index(IN)			- Index at which the Via Parameter is deleted
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_deleteViaParamAtIndexInViaHdr sip_deleteParamAtIndexInCommonHdr


/***********************************************************************
** FUNCTION: sip_getCommentFromViaHdr
** DESCRIPTION:gets the Comment from the Sip Via Header
** PARAMETERS:
**			hdr(IN) 			- Sip Via Header
**			Comment(OUT)		- Comment to be retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
/* SS: Defintion not found */
extern SipBool sip_getCommentFromViaHdr _ARGS_ ((SipHeader *hdr, \
		SIP_S8bit **comment, SipError *err));


/***********************************************************************
** FUNCTION: sip_setCommentInViaHdr
** DESCRIPTION:sets the Comment in the Sip Via Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip Via Header
**			Comment(IN)			- Comment to be set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
/* SS: Defintion not found */
extern SipBool sip_setCommentInViaHdr _ARGS_ ((SipHeader *hdr, \
		SIP_S8bit *comment, SipError *err));

#ifdef SIP_REFER
/*****************************************************************
**
** FUNCTION:  sip_getAddrSpecFromReferToHdr
**
** DESCRIPTION: This function retrieves the dAddr-spec field from a
**		SIP ReferTo pHeader
**		ppAddrSpec(OUT): The retrieved AddrSpec
**		hdr(IN)		   : The Sip Header From Which AddrSpec has to be retrieved.
**
***************************************************************/
#define sip_getAddrSpecFromReferToHdr sip_getAddrSpecFromCommonHdr  
/*****************************************************************
**
** FUNCTION:  sip_setAddrSpecInReferToHdr
**
** DESCRIPTION: This function sets the dAddr-spec field in a SIP
**		Record-Route pHeader
**		hdr(OUT)	  : the ReferTo Header To which
**							AddrSpec Has to be set
**		pAddrSpec(OUT): the AddrSpec to be set
**		err			  : SipError	
**
***************************************************************/
#define sip_setAddrSpecInReferToHdr sip_setAddrSpecInCommonHdr 

#endif
/***********************************************************************
** FUNCTION: sip_getSeqNumFromCseqHdr
** DESCRIPTION:get the Sequence number from the Sip CSequence Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip CSequence Header
**			seq_no(OUT)			- The sequencenumber retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_getSeqNumFromCseqHdr sip_getdIntVar1FromCommonHdr  


/***********************************************************************
** FUNCTION: sip_setSeqNumInCseqHdr
** DESCRIPTION:set the Sequence number in the Sip CSequence Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip CSequence Header
**			seq_no(IN)			- The sequencenumber set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_setSeqNumInCseqHdr sip_setdIntVar1InCommonHdr 


/***********************************************************************
** FUNCTION: sip_getMethodFromCseqHdr
** DESCRIPTION:get the Method from the Sip CSequence Header
** PARAMETERS:
**			hdr(IN) 			- Sip CSequence Header
**			method(OUT)			- The method to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_getMethodFromCseqHdr sip_getStr1FromCommonHdr 


/***********************************************************************
** FUNCTION: sip_setMethodInCseqHdr
** DESCRIPTION:set the Method in the Sip CSequence Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip CSequence Header
**			method(IN)			- The method to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_setMethodInCseqHdr sip_setStr1InCommonHdr

/****************************************************************
**
** FUNCTION: sip_getDispTypeFromContentDispositionHdr
**
** DESCRIPTION: This function retrives the display typei field from the
** ContentDisposition pHeader.
** PARAMETERS:
**			hdr(IN) 				- Sip ContentDisposition Header
**			ppDispType(OUT)			- The DispType field to retrieve
**			err(OUT)				- Possible error value(see API ref doc)
**
**
*****************************************************************/
#define sip_getDispTypeFromContentDispositionHdr sip_getStr1FromCommonHdr
/****************************************************************
**
** FUNCTION: sip_setDispTypeInContentDispositionHdr
**
** DESCRIPTION: This function sets the pDispType field in the ContentDisposition
** Header.
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip ContentDisposition Header
**			pDispType(IN)			- The DispType field to retrieve
**			err(OUT)				- Possible error value(see API ref doc)
**
**
*****************************************************************/
#define sip_setDispTypeInContentDispositionHdr sip_setStr1InCommonHdr
/*****************************************************************
**
** FUNCTION:  sip_getParamCountFromContentDispositionHdr
**
** DESCRIPTION: This function retrieives the number of parametrs
**		from a SIP Encryption pHeader
** PARAMETERS:
**			hdr(IN) 			- Sip ContentDisposition Header
**			pParam(OUT)			- The Parameter to retrieve
**			index(IN)			- The index at which the Parameter is retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
***************************************************************/
#define sip_getParamCountFromContentDispositionHdr sip_getParamCountFromCommonHdr 
/*****************************************************************
**
** FUNCTION:  sip_getParamAtIndexFromContentDispositionHdr
**
** DESCRIPTION: This function retrieves a paarmeter at a specified
**		index from a SIP ContentDisposition Header
** PARAMETERS:
**			hdr(IN) 			- Sip ContentDisposition Header
**			ppParam(OUT)		- The Parameter to retrieve
**			index(IN)			- The index at which the Parameter is retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
***************************************************************/
#define sip_getParamAtIndexFromContentDispositionHdr sip_getParamAtIndexFromCommonHdr 
/*****************************************************************
**
** FUNCTION:  sip_insertParamAtIndexInContentDispositionHdr
**
** DESCRIPTION: This function inserts a parameter at a specified
**		in a SIp ContentDisposition pHeader
** PARAMETERS:
**			hdr(IN/OUT) 		- Sip ContentDisposition Header
**			pParam(IN)			- The Parameter to insert
**			index(IN)			- The index at which the Parameter is inserted
**			err(OUT)			- Possible error value(see API ref doc)
**
**
***************************************************************/
#define sip_insertParamAtIndexInContentDispositionHdr sip_insertParamAtIndexInCommonHdr 
/*****************************************************************
**
** FUNCTION:  sip_deleteParamAtIndexInContentDispositionHdr
**
** DESCRIPTION: This function deletes a parameter at a specified
**		index in a SIP ContentDisposition pHeader
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip ContentDisposition Header
**			index(IN)			- The index at which the Parameter is deleted
**			err(OUT)			- Possible error value(see API ref doc)
**
**
***************************************************************/
#define sip_deleteParamAtIndexInContentDispositionHdr sip_deleteParamAtIndexInCommonHdr 
/*****************************************************************
**
** FUNCTION:  sip_setParamAtIndexInContentDispositionHdr
**
** DESCRIPTION: This function sets a parameter at a specified index
**		in a SIP ContentDisposition pHeader
** PARAMETERS:
**			hdr(IN/OUT) 		- Sip ContentDisposition Header
**			pParam(IN)			- The Parameter to set
**			index(IN)			- The index at which the Parameter is set
**			err(OUT)			- Possible error value(see API ref doc)
**
**
***************************************************************/
#define sip_setParamAtIndexInContentDispositionHdr sip_setParamAtIndexInCommonHdr 
	
/****************************************************************
**
** FUNCTION: sip_getEventTypeFromEventHdr
**
** DESCRIPTION: This function retrives the display typei field from the
** Event pHeader.
** PARAMETERS:
**			hdr(IN) 				- Sip Event Header
**			ppDispType(OUT)			- The DispType field to retrieve
**			err(OUT)				- Possible error value(see API ref doc)
**
**
*****************************************************************/

#define sip_getEventTypeFromEventHdr  sip_getStr1FromCommonHdr
/****************************************************************
**
** FUNCTION: sip_setEventTypeInEventHdr
**
** DESCRIPTION: This function sets the pDispType field in the Event
** Header.
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip Event Header
**			pDispType(IN)			- The DispType field to retrieve
**			err(OUT)				- Possible error value(see API ref doc)
**
**
*****************************************************************/
#define sip_setEventTypeInEventHdr  sip_setStr1InCommonHdr
 
/*****************************************************************
**
** FUNCTION:  sip_getParamCountFromEventHdr
**
** DESCRIPTION: This function retrieives the number of parametrs
**		from a SIP Event pHeader
** PARAMETERS:
**			hdr(IN) 			- Sip Event Header
**			pParam(OUT)			- The Parameter to retrieve
**			index(IN)			- The index at which the Parameter is retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
***************************************************************/
#define sip_getParamCountFromEventHdr sip_getParamCountFromCommonHdr

/*****************************************************************
**
** FUNCTION:  sip_getParamAtIndexFromEventHdr
**
** DESCRIPTION: This function retrieves a paarmeter at a specified
**		index from a SIP Event Header
** PARAMETERS:
**			hdr(IN) 			- Sip Event Header
**			ppParam(OUT)		- The Parameter to retrieve
**			index(IN)			- The index at which the Parameter is retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
***************************************************************/
#define sip_getParamAtIndexFromEventHdr sip_getParamAtIndexFromCommonHdr
 
/*****************************************************************
**
** FUNCTION:  sip_insertParamAtIndexInEventHdr
**
** DESCRIPTION: This function inserts a parameter at a specified
**		in a SIp Event pHeader
** PARAMETERS:
**			hdr(IN/OUT) 		- Sip Event Header
**			pParam(IN)			- The Parameter to insert
**			index(IN)			- The index at which the Parameter is inserted
**			err(OUT)			- Possible error value(see API ref doc)
**
**
***************************************************************/
#define sip_insertParamAtIndexInEventHdr sip_insertParamAtIndexInCommonHdr
 
/*****************************************************************
**
** FUNCTION:  sip_deleteParamAtIndexInEventHdr
**
** DESCRIPTION: This function deletes a parameter at a specified
**		index in a SIP Event pHeader
** PARAMETERS:
**			hdr(IN/OUT) 		- Sip Event Header
**			index(IN)			- The index at which the Parameter is deleted
**			err(OUT)			- Possible error value(see API ref doc)
**
**
***************************************************************/
#define sip_deleteParamAtIndexInEventHdr sip_deleteParamAtIndexInCommonHdr
 
/*****************************************************************
**
** FUNCTION:  sip_setParamAtIndexInEvent
**
** DESCRIPTION: This function sets a parameter at a specified index
**		in a SIP Event pHeader
** PARAMETERS:
**			hdr(IN/OUT) 		- Sip Event Header
**			pParam(IN)			- The Parameter to set
**			index(IN)			- The index at which the Parameter is set
**			err(OUT)			- Possible error value(see API ref doc)
**
**
***************************************************************/
#define sip_setParamAtIndexInEvent  sip_setParamAtIndexInCommonHdr

/***********************************************************************
** Function: sip_getMethodFromReqLine
** Description:gets the Method from the Sip Status Line
** Parameters:
**			line(IN) 			- Sip Status Line
**			method(OUT)			- The method to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_getMethodFromReqLine _ARGS_ ((SipReqLine *line, \
		SIP_S8bit **method, SipError *err));

/***********************************************************************
** Function: sip_setMethodInReqLine
** Description:sets the Method in the Sip Status Line
** Parameters:
**			line(IN/OUT) 			- Sip Status Line
**			method(IN)			- The method to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setMethodInReqLine _ARGS_ ((SipReqLine *line, \
		SIP_S8bit *method, SipError *err));
/***********************************************************************
** Function: sip_getAddrSpecFromReqLine
** Description:gets the Sip Addr Spec structure from the Sip Request Line
** Parameters:
**			line(IN) 			- Sip RequestLine
**			pReqUri(OUT)		- Addr Spec structure to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_getAddrSpecFromReqLine _ARGS_ ((SipReqLine *line, \
		SipAddrSpec **ppReqUri, SipError *err));
/***********************************************************************
** Function: sip_setAddrSpecInReqLine
** Description:sets the Sip Addr Spec structure in the Sip Request Line
** Parameters:
**			line(IN/OUT) 			- Sip RequestLine
**			pReqUri(IN)			- Addr Spec structure to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setAddrSpecInReqLine _ARGS_ ((SipReqLine *line, \
		SipAddrSpec *req_uri, SipError *err));


/***********************************************************************
** Function: sip_getVersionFromReqLine
** Description:gets the Version fromthe Sip Request Line
** Parameters:
**			line(IN) 			- Sip RequestLine
**			version(OUT)			- Version to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_getVersionFromReqLine _ARGS_ ((SipReqLine *line, \
		SIP_S8bit **version, SipError *err));


/***********************************************************************
** Function: sip_setVersionInReqLine
** Description:gets the Version in the Sip Request Line
** Parameters:
**			line(IN/OUT) 			- Sip RequestLine
**			version(IN)			- Version to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setVersionInReqLine _ARGS_ ((SipReqLine *line, \
		SIP_S8bit *version, SipError *err));

/***********************************************************************
** Function: sip_getVersionFromStatusLine
** Description:gets the Version field from the Sip Status Line
** Parameters:
**			line(IN) 			- Sip Status Line
**			version(OUT)		- The version to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_getVersionFromStatusLine _ARGS_ ((SipStatusLine *line, \
		SIP_S8bit **version, SipError *err));

/***********************************************************************
** Function: sip_setVersionInStatusLine
** Description:sets the Version field in the Sip Status Line
** Parameters:
**			line(IN/OUT) 			- Sip Status Line
**			version(IN)			- The version to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setVersionInStatusLine _ARGS_ ((SipStatusLine *line, \
		SIP_S8bit *version, SipError *err));
/***********************************************************************
** Function: sip_getStatusCodeNumFromStatusLine
** Description:gets the Status code number from the Sip Status Line
** Parameters:
**			line(IN) 			- Sip Status Line
**			code_num(OUT)		- The Status code number to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_getStatusCodeNumFromStatusLine _ARGS_ ((SipStatusLine *line, \
		SIP_U32bit *code_num, SipError *err));


/***********************************************************************
** Function: sip_setStatusCodeNumInStatusLine
** Description:sets the Status code number in the Sip Status Line
** Parameters:
**			line(IN/OUT) 			- Sip Status Line
**			code_num(IN)		- The Status code number to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setStatusCodeNumInStatusLine _ARGS_ ((SipStatusLine *line, \
		SIP_U32bit code_num, SipError *err));


/***********************************************************************
** Function: sip_getReasonFromStatusLine
** Description:gets the Reason in the Sip Status Line
** Parameters:
**			line(IN) 			- Sip Status Line
**			reason(OUT)			- The reason to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_getReasonFromStatusLine _ARGS_ ((SipStatusLine *line, \
		SIP_S8bit **reason, SipError *err));


/***********************************************************************
** Function: sip_setReasonInStatusLine
** Description:sets the Reason in the Sip Status Line
** Parameters:
**			line(IN/OUT) 			- Sip Status Line
**			reason(IN)			- The reason to set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setReasonInStatusLine _ARGS_ ((SipStatusLine *line, \
		SIP_S8bit *reason, SipError *err));
	
#ifdef SIP_DATE
/***********************************************************************
** Function: sip_getDateFromRetryAfterHdr
** Description: get date field from RetryAfter Header
** Parameters:
**				hdr(IN)		- Sip RetryAfter Header
**				date(OUT) 	- date to retrieve
**				err(OUT)    - Possible error value (See API ref doc)
************************************************************************/
#define sip_getDateFromRetryAfterHdr sip_getDateFromCommonHdr 

/***********************************************************************
** Function: sip_setDateInRetryAfterHdr
** Description: set date in RetryAfter header
** Parameters:
**				hdr(IN/OUT)	- Sip RetryAfter Header
**				date(IN) 	- date to set
**				err(OUT)    - Possible error value (See API ref doc)
************************************************************************/
#define sip_setDateInRetryAfterHdr sip_setDateInCommonHdr 
#endif /* SIP_DATE */

/***********************************************************************
** Function: sip_getDeltaSecondsFromRetryAfterHdr
** Description: get delta seconds field from RetryAfter Header
** Parameters:
**				hdr(IN)			- Sip RetryAfter Header
**				dseconds(OUT) 	- seconds to retrieve
**				err(OUT)    	- Possible error value (See API ref doc)
************************************************************************/
#ifdef SIP_DATE
#define sip_getDeltaSecondsFromRetryAfterHdr sip_getdIntVar1FromCommonHdr
#endif
/***********************************************************************
** Function: sip_setDeltaSecondsInRetryAfterHdr
** Description: set delta seconds in RetryAfter header
** Parameters:
**				hdr(IN/OUT)	- Sip RetryAfter Header
**				dseconds(IN)	- delta seconds to set
**				err(OUT)    - Possible error value (See API ref doc)
************************************************************************/
extern SipBool sip_setDeltaSecondsInRetryAfterHdr _ARGS_(( SipHeader *hdr, \
		SIP_U32bit dseconds, SipError *err));

/***********************************************************************
** Function: sip_getCommentFromRetryAfterHdr
** Description: get Comment field from RetryAfter Header
** Parameters:
**				hdr(IN)			- Sip RetryAfter Header
**				comment(OUT) 	- Comment to retrieve
**				err(OUT)    	- Possible error value (See API ref doc)
************************************************************************/
#define sip_getCommentFromRetryAfterHdr sip_getStr1FromCommonHdr 

/***********************************************************************
** Function: sip_setCommentInRetryAfterHdr
** Description: set Comment field in RetryAfter Header
** Parameters:
**				hdr(IN/OUT)		- Sip RetryAfter Header
**				comment(IN) 	- Comment to set
**				err(OUT)    	- Possible error value (See API ref doc)
************************************************************************/
#define sip_setCommentInRetryAfterHdr sip_setStr1InCommonHdr 

/***********************************************************************
** Function:  sip_getParamCountFromRetryAfterHdr
** Description: Retrieve the number of parameters in a RetryAfter header
** Parameters:
**				pHdr(IN/OUT)		- Sip RetryAfter Header
**				pCount(IN) 		- The number of parameters in RetryAfter header
**				pErr(OUT)    		- Possible error value (See API ref doc)
************************************************************************/
#define sip_getParamCountFromRetryAfterHdr sip_getParamCountFromCommonHdr 

/***********************************************************************
** Function:  sip_getParamAtIndexFromRetryAfterHdr
** Description: Retrieves the param at a specified index from a Retry
		After header
** Parameters:
**				pHdr(IN)		- Sip RetryAfter Header
**				pParam/ppParam(OUT)	_ The retrieved param
**				DIndex(IN) 		- The index of the param to be retrieved
**				pErr(OUT)    		- Possible error value (See API ref doc)
************************************************************************/
#define sip_getParamAtIndexFromRetryAfterHdr sip_getParamAtIndexFromCommonHdr  

/***********************************************************************
** Function:  sip_setParamAtIndexInRetryAfterHdr
** Description: Sets a param at a specified index in a Retry
		After header
** Parameters:
**				pHdr(IN/OUT)		- Sip RetryAfter Header
**				pParam(IN)		_ The param to be set
**				DIndex(IN) 		- The index at which the param to be set
**				pErr(OUT)    		- Possible error value (See API ref doc)
************************************************************************/
#define sip_setParamAtIndexInRetryAfterHdr sip_setParamAtIndexInCommonHdr 

/***********************************************************************
** Function:  sip_insertParamAtIndexInRetryAfterHdr
** Description: Inserts a param at a specified index in a Retry
		After header
** Parameters:
**				pHdr(IN/OUT)		- Sip RetryAfter Header
**				pParam(IN)		_ The param to be inserted
**				DIndex(IN) 		- The index at which the param to be inserted
**				pErr(OUT)    		- Possible error value (See API ref doc)
************************************************************************/
#define sip_insertParamAtIndexInRetryAfterHdr sip_insertParamAtIndexInCommonHdr 

/***********************************************************************
** Function:  sip_deleteParamAtIndexInRetryAfterHdr
** Description: Deletes a param at a specified index in a Retry
		After header
** Parameters:
**				pHdr(IN/OUT)		- Sip RetryAfter Header
**				DIndex(IN) 		- The index at which the param to be deleted
**				pErr(OUT)    		- Possible error value (See API ref doc)
************************************************************************/
#define sip_deleteParamAtIndexInRetryAfterHdr sip_deleteParamAtIndexInCommonHdr 

/* 
 * Definitions for headers that have been #defined to header functions for
 * other headers are defined here.
 */
/* APIs for Max-Forwards header */
#define sip_getHopsFromMaxForwardsHdr	sip_getdIntVar1FromCommonHdr
#define sip_setHopsInMaxForwardsHdr		sip_setdIntVar1InCommonHdr

/* Record-Route header related accessor APIs. */
#define sip_getDispNameFromRecordRouteHdr		sip_getStr1FromCommonHdr
#define sip_setDispNameInRecordRouteHdr			sip_setStr1InCommonHdr
#define sip_getAddrSpecFromRecordRouteHdr		sip_getAddrSpecFromCommonHdr
#define sip_setAddrSpecInRecordRouteHdr			sip_setAddrSpecInCommonHdr
#define sip_getParamCountFromRecordRouteHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromRecordRouteHdr 	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInRecordRouteHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInRecordRouteHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInRecordRouteHdr	sip_deleteParamAtIndexInCommonHdr

#ifdef SIP_3GPP
		/*PATH Header */
#define sip_getDispNameFromPathHdr				sip_getStr1FromCommonHdr
#define sip_setDispNameInPathHdr					sip_setStr1InCommonHdr
#define sip_getAddrSpecFromPathHdr		    sip_getAddrSpecFromCommonHdr
#define sip_setAddrSpecInPathHdr			    sip_setAddrSpecInCommonHdr
#define sip_getParamCountFromPathHdr		  sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromPathHdr	  sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInPathHdr		  sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInPathHdr	  sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInPathHdr	  sip_deleteParamAtIndexInCommonHdr

/* P-Associated-URI Header */
#define sip_getDispNameFromPAssociatedUriHdr		sip_getStr1FromCommonHdr
#define sip_setDispNameInPAssociatedUriHdr			sip_setStr1InCommonHdr
#define sip_getAddrSpecFromPAssociatedUriHdr		sip_getAddrSpecFromCommonHdr
#define sip_setAddrSpecInPAssociatedUriHdr			sip_setAddrSpecInCommonHdr
#define sip_getParamCountFromPAssociatedUriHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromPAssociatedUriHdr	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInPAssociatedUriHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInPAssociatedUriHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInPAssociatedUriHdr	sip_deleteParamAtIndexInCommonHdr
	/* P-Called-Party-ID Header */
#define sip_getDispNameFromPCalledPartyIdHdr		sip_getStr1FromCommonHdr
#define sip_setDispNameInPCalledPartyIdHdr			sip_setStr1InCommonHdr
#define sip_getAddrSpecFromPCalledPartyIdHdr		sip_getAddrSpecFromCommonHdr
#define sip_setAddrSpecInPCalledPartyIdHdr			sip_setAddrSpecInCommonHdr
#define sip_getParamCountFromPCalledPartyIdHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromPCalledPartyIdHdr	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInPCalledPartyIdHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInPCalledPartyIdHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInPCalledPartyIdHdr	sip_deleteParamAtIndexInCommonHdr
	/* P-Visited-Network-ID Header */
#define sip_getDispNameFromPVisitedNetworkIdHdr		 sip_getStr1FromCommonHdr
#define sip_setDispNameInPVisitedNetworkIdHdr		   sip_setStr1InCommonHdr
#define sip_getParamCountFromPVisitedNetworkIdHdr	   sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromPVisitedNetworkIdHdr	   sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInPVisitedNetworkIdHdr	    sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInPVisitedNetworkIdHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInPVisitedNetworkIdHdr	sip_deleteParamAtIndexInCommonHdr

	/* P-Charging-Function-Addresses Header */
#define sip_getDispNameFromPcfAddrHdr		sip_getStr1FromCommonHdr
#define sip_setDispNameInPcfAddrHdr			sip_setStr1InCommonHdr
#define sip_getParamCountFromPcfAddrHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromPcfAddrHdr	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInPcfAddrHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInPcfAddrHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInPcfAddrHdr	sip_deleteParamAtIndexInCommonHdr
	/* Service-Route Header */
#define sip_getDispNameFromServiceRouteHdr		sip_getStr1FromCommonHdr
#define sip_setDispNameInServiceRouteHdr			sip_setStr1InCommonHdr
#define sip_getAddrSpecFromServiceRouteHdr		sip_getAddrSpecFromCommonHdr
#define sip_setAddrSpecInServiceRouteHdr			sip_setAddrSpecInCommonHdr
#define sip_getParamCountFromServiceRouteHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromServiceRouteHdr	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInServiceRouteHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInServiceRouteHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInServiceRouteHdr	sip_deleteParamAtIndexInCommonHdr
	/* P-Charging-Vector */
#define sip_getDispNameFromPcVectorHdr        sip_getStr1FromCommonHdr
#define sip_setDispNameInPcVectorHdr       		sip_setStr1InCommonHdr
#define sip_getAccessValueFromPcVectorHdr  		sip_getSentProtocolFromCommonHdr 
#define sip_setAccessValueInPcVectorHdr    		sip_setSentProtocolInCommonHdr
#define sip_getParamCountFromPcVectorHdr   		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromPcVectorHdr 		sip_getParamAtIndexFromCommonHdr
#define sip_insertParamAtIndexInPcVectorHdr 	sip_insertParamAtIndexInCommonHdr
#define sip_setParamAtIndexInPcVectorHdr		  sip_setParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInPcVectorHdr 	sip_deleteParamAtIndexInCommonHdr
 /* P-Access-Network-Info */
#define sip_getSchemeFromPanInfoHdr   sip_getStr1FromCommonHdr
#define sip_setSchemeInPanInfoHdr     sip_setStr1InCommonHdr
#define sip_getParamCountFromPanInfoHdr    sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromPanInfoHdr  sip_getParamAtIndexFromCommonHdr
#define sip_insertParamAtIndexInPanInfoHdr sip_insertParamAtIndexInCommonHdr
#define sip_setParamAtIndexInPanInfoHdr    sip_setParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInPanInfoHdr sip_deleteParamAtIndexInCommonHdr
	/* History-Info */
#define sip_getDispNameFromHistoryInfoHdr		sip_getStr1FromCommonHdr
#define sip_setDispNameInHistoryInfoHdr			sip_setStr1InCommonHdr
#define sip_getAddrSpecFromHistoryInfoHdr		sip_getAddrSpecFromCommonHdr
#define sip_setAddrSpecInHistoryInfoHdr			sip_setAddrSpecInCommonHdr
#define sip_getParamCountFromHistoryInfoHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromHistoryInfoHdr	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInHistoryInfoHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInHistoryInfoHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInHistoryInfoHdr	sip_deleteParamAtIndexInCommonHdr

	/* Join Header */
#define sip_getCallidFromJoinHdr	   		sip_getStr1FromCommonHdr
#define sip_setCallidInJoinHdr		   		sip_setStr1InCommonHdr
#define sip_getParamCountFromJoinHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromJoinHdr	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInJoinHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInJoinHdr		sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInJoinHdr		sip_deleteParamAtIndexInCommonHdr

	/* AcceptContact Header */
#define sip_getParamCountFromAcceptContactHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromAcceptContactHdr		sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInAcceptContactHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInAcceptContactHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInAcceptContactHdr	sip_deleteParamAtIndexInCommonHdr

	/* RejectContact Header */
#define sip_getParamCountFromRejectContactHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromRejectContactHdr		sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInRejectContactHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInRejectContactHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInRejectContactHdr	sip_deleteParamAtIndexInCommonHdr

	/* Request Disposition Header */
#define sip_getDirectiveFromRequestDispositionHdr sip_getStr1FromCommonHdr
#define sip_setDirectiveInRequestDispositionHdr   sip_setStr1InCommonHdr
	/* SIP-If-Match Header */
#define sip_getEntityTagFromIfMatchHdr sip_getStr1FromCommonHdr
#define sip_setEntityTagInIfMatchHdr   sip_setStr1InCommonHdr
	/* SIP-ETag Header */
#define sip_getEntityTagFromETagHdr sip_getStr1FromCommonHdr
#define sip_setEntityTagInETagHdr   sip_setStr1InCommonHdr

#endif

#ifdef SIP_SECURITY

#define sip_getParamCountFromSecurityClientHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromSecurityClientHdr	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInSecurityClientHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInSecurityClientHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInSecurityClientHdr	sip_deleteParamAtIndexInCommonHdr


#define sip_getParamCountFromSecurityVerifyHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromSecurityVerifyHdr	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInSecurityVerifyHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInSecurityVerifyHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInSecurityVerifyHdr	sip_deleteParamAtIndexInCommonHdr

#define sip_getParamCountFromSecurityServerHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromSecurityServerHdr	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInSecurityServerHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInSecurityServerHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInSecurityServerHdr	sip_deleteParamAtIndexInCommonHdr
#endif

#ifdef SIP_PRIVACY
#define sip_getNameValuePairCountFromPrivacyHdr		sip_getParamCountFromCommonHdr
#define sip_getNameValuePairAtIndexFromPrivacyHdr	sip_getNameValuePairAtIndexFromCommonHdr
#define sip_setNameValuePairAtIndexInPrivacyHdr		sip_setNameValuePairAtIndexInCommonHdr
#define sip_insertNameValuePairAtIndexInPrivacyHdr	sip_insertNameValuePairAtIndexInCommonHdr
#define sip_deleteNameValuePairAtIndexInPrivacyHdr	sip_deleteNameValuePairAtIndexInCommonHdr
#endif
    
/* To header related accessor APIs. */
#define sip_getDispNameFromToHdr				sip_getStr1FromCommonHdr
#define sip_setDispNameInToHdr					sip_setStr1InCommonHdr 
#define sip_getTagCountFromToHdr(pHdr,pCount,pErr) \
                sip_getParamCountForNameFromCommonHdr(pHdr,"tag",pCount,pErr)
#define sip_getTagAtIndexFromToHdr(pHdr,ppTag,index,pErr) \
                sip_getParamForNameFromCommonHdr(pHdr,"tag",ppTag,pErr)
#define sip_insertTagAtIndexInToHdr(pHdr,ppTag,index,pErr) \
                sip_setParamForNameInCommonHdr(pHdr,"tag",ppTag,pErr)
#define sip_setTagAtIndexInToHdr(pHdr,ppTag,index,pErr) \
               sip_setParamForNameInCommonHdr(pHdr,"tag",ppTag,pErr)
#define sip_getExtensionParamCountFromToHdr		sip_getParamCountFromCommonHdr
#define sip_getExtensionParamAtIndexFromToHdr	sip_getParamAtIndexFromCommonHdr
#define sip_insertExtensionParamAtIndexInToHdr	sip_insertParamAtIndexInCommonHdr
#define sip_setExtensionParamAtIndexInToHdr		sip_setParamAtIndexInCommonHdr
#define sip_deleteExtensionParamAtIndexInToHdr	sip_deleteParamAtIndexInCommonHdr
#define sip_getAddrSpecFromToHdr				sip_getAddrSpecFromCommonHdr
#define sip_setAddrSpecInToHdr					sip_setAddrSpecInCommonHdr

#ifdef SIP_PRIVACY
#define sip_getAddrSpecFromPPreferredIdHdr  sip_getAddrSpecFromCommonHdr
#define sip_getAddrSpecFromPAssertIdHdr  sip_getAddrSpecFromCommonHdr
#define sip_setAddrSpecInPPreferredIdHdr					sip_setAddrSpecInCommonHdr
#define sip_setAddrSpecInPAssertIdHdr					sip_setAddrSpecInCommonHdr
#define sip_getDispNameFromPPreferredIdHdr sip_getStr1FromCommonHdr             
#define sip_getDispNameFromPAssertIdHdr sip_getStr1FromCommonHdr             
#define sip_setDispNameInPPreferredIdHdr			sip_setStr1InCommonHdr
#define sip_setDispNameInPAssertIdHdr			sip_setStr1InCommonHdr

#endif
#ifdef SIP_REFER
/* Referred-By header related accessor APIs. */
#define sip_getDispNameFromReferredByHdr		sip_getStr1FromCommonHdr
#define sip_setDispNameInReferredByHdr			sip_setStr1InCommonHdr
#define sip_getReferrerFromReferredByHdr		sip_getAddrSpecFromCommonHdr
#define sip_setReferrerInReferredByHdr			sip_setAddrSpecInCommonHdr
#define sip_getParamCountFromReferredByHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromReferredByHdr	sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInReferredByHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInReferredByHdr	sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInReferredByHdr	sip_deleteParamAtIndexInCommonHdr
/*Refer To Header */
#define sip_getDispNameFromReferToHdr		    sip_getStr1FromCommonHdr
#define sip_setDispNameInReferToHdr			    sip_setStr1InCommonHdr
#define sip_getParamCountFromReferToHdr		    sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromReferToHdr	    sip_getParamAtIndexFromCommonHdr
#define sip_getAddrSpecFromReferToHdr           sip_getAddrSpecFromCommonHdr	
#define sip_setAddrSpecInReferToHdr		        sip_setAddrSpecInCommonHdr 
#define sip_setParamAtIndexInReferToHdr	        sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInReferToHdr	    sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInReferToHdr	    sip_deleteParamAtIndexInCommonHdr


#ifdef SIP_SECURITY

#define sip_getMechanismNameFromSecurityClientHdr sip_getStr1FromCommonHdr
#define sip_setMechanismNameInSecurityClientHdr sip_setStr1InCommonHdr

#define sip_getMechanismNameFromSecurityVerifyHdr sip_getStr1FromCommonHdr
#define sip_setMechanismNameInSecurityVerifyHdr sip_setStr1InCommonHdr

#define sip_getMechanismNameFromSecurityServerHdr sip_getStr1FromCommonHdr
#define sip_setMechanismNameInSecurityServerHdr sip_setStr1InCommonHdr
               
#endif
#ifdef SIP_CORE_STACK_COMPATIBLE
/*********************************************************
** FUNCTION:sip_getMsgIdFromReferredByHdr
**
** DESCRIPTION: This function retrieves the MsgId field
**		from a SIP ReferredBy pHeader
**
**********************************************************/
extern SipBool sip_getMsgIdFromReferredByHdr _ARGS_ ((SipHeader *pHdr,\
	SIP_S8bit **ppMsgId, SipError *pErr));

/*********************************************************
** FUNCTION:sip_setMsgIdInReferredByHdr
**
** DESCRIPTION: This function sets the msgId field in
**		a SIP ReferredBy pHeader
**
**********************************************************/

#define sip_setMsgIdInReferredByHdr(pHdr,pMsgId,pErr) \
               sip_setParamForNameInCommonHdr(pHdr,"cid",pMsgId,pErr)
#endif /* SIP_CORE_STACK_COMPATIBLE */

#endif /* SIP_REFER */

/* Route header related accessor APIs. */
#define sip_getDispNameFromRouteHdr		        sip_getStr1FromCommonHdr
#define sip_setDispNameInRouteHdr			    sip_setStr1InCommonHdr
#define sip_getParamCountFromRouteHdr		    sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromRouteHdr	        sip_getParamAtIndexFromCommonHdr
#define sip_getAddrSpecFromRouteHdr             sip_getAddrSpecFromCommonHdr	
#define sip_setAddrSpecInRouteHdr		        sip_setAddrSpecInCommonHdr 
#define sip_setParamAtIndexInRouteHdr	        sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInRouteHdr	    sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInRouteHdr	    sip_deleteParamAtIndexInCommonHdr

#ifdef SIP_IMPP
/* APIs for Event header */
#define sip_impp_getEventTypeFromEventHdr		sip_getStr1FromCommonHdr
#define sip_impp_setEventTypeInEventHdr			sip_setStr1InCommonHdr
#define sip_impp_getParamCountFromEventHdr		sip_getParamCountFromCommonHdr
#define sip_impp_getParamAtIndexFromEventHdr	sip_getParamAtIndexFromCommonHdr
#define sip_impp_setParamAtIndexInEventHdr		sip_setParamAtIndexInCommonHdr
#define sip_impp_insertParamAtIndexInEventHdr	sip_insertParamAtIndexInCommonHdr
#define sip_impp_deleteParamAtIndexInEventHdr	sip_deleteParamAtIndexInCommonHdr

/* APIs for Subscription-State header */
#define sip_impp_getSubStateFromSubscriptionStateHdr sip_getStr1FromCommonHdr
#define sip_impp_setSubStateInSubscriptionStateHdr   sip_setStr1InCommonHdr
#define sip_impp_getParamCountFromSubscriptionStateHdr \
                sip_getParamCountFromCommonHdr
#define sip_impp_getParamAtIndexFromSubscriptionStateHdr \
	            sip_getParamAtIndexFromCommonHdr
#define sip_impp_insertParamAtIndexInSubscriptionStateHdr \
	            sip_insertParamAtIndexInCommonHdr
#define sip_impp_deleteParamAtIndexInSubscriptionStateHdr \
	            sip_deleteParamAtIndexInCommonHdr
#define sip_impp_setParamAtIndexInSubscriptionStateHdr \
	            sip_setParamAtIndexInCommonHdr
#endif /* SIP_IMPP */

#ifdef SIP_REPLACES
/* APIs for Replaces header */
#define sip_getCallidFromReplacesHdr			sip_getStr1FromCommonHdr
#define sip_setCallidInReplacesHdr				sip_setStr1InCommonHdr
#define sip_getParamCountFromReplacesHdr		sip_getParamCountFromCommonHdr
#define sip_getParamAtIndexFromReplacesHdr		sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInReplacesHdr		sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInReplacesHdr		sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInReplacesHdr		sip_deleteParamAtIndexInCommonHdr
#endif /* SIP_REPLACES */


#ifdef SIP_CONGEST
#define sip_getNamespaceFromRsrcPriorityHdr 		sip_getStr1FromCommonHdr
#define sip_setNamespaceInRsrcPriorityHdr 			sip_setStr1InCommonHdr
#define sip_getPriorityFromRsrcPriorityHdr 			sip_getStr2FromCommonHdr
#define sip_setPriorityInRsrcPriorityHdr 			sip_setStr2InCommonHdr
#define sip_getNamespaceFromAcceptRsrcPriorityHdr	sip_getStr1FromCommonHdr
#define sip_getPriorityFromAcceptRsrcPriorityHdr 	sip_getStr2FromCommonHdr
#define sip_setNamespaceInAcceptRsrcPriorityHdr 	sip_setStr1InCommonHdr
#define sip_setPriorityInAcceptRsrcPriorityHdr 		sip_setStr2InCommonHdr
#endif

#ifdef SIP_SESSIONTIMER
/****************************************************************************
** FUNCTION: sip_getSecondsFromMinSEHdr
** DESCRIPTION: This function retrieves the seconds field from a MinSE Header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Min-SE SipHeader
**				dSec(OUT)		- The retrieved seconds field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_getSecondsFromMinSEHdr	sip_getdIntVar1FromCommonHdr

/****************************************************************************
** FUNCTION: sip_setSecondsInMinSEHdr
** DESCRIPTION: This function sets the seconds field in a MinSE Header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the MinSE SipHeader
**				dSec(IN)		- The seconds to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_setSecondsInMinSEHdr	sip_setdIntVar1InCommonHdr

/****************************************************************************
** FUNCTION: sip_getParamCountFromMinSEHdr
** DESCRIPTION: This function retrieves the number of SipParams from a
**				MinSE header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the MinSE SipHeader
**				pCount(OUT)		- The retrieved count
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getParamCountFromMinSEHdr sip_getParamCountFromCommonHdr

/****************************************************************************
** FUNCTION: sip_getParamAtIndexFromMinSEHdr
** DESCRIPTION: This function retrieves the parameter at a specified
**				 index from a MinSE header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the MinSE SipHeader
**				ppParam(OUT)	- The retrieved parameter
**				index(IN)		- The index to retrive from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getParamAtIndexFromMinSEHdr sip_getParamAtIndexFromCommonHdr

/****************************************************************************
** FUNCTION: sip_insertParamAtIndexInMinSEHdr
** DESCRIPTION: This function inserts a parameter at a specified
**				index in a MinSE header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the MinSE SipHeader
**				pParam(IN)		- The parameter to be inserted
**				index(IN)		- The index to insert at
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_insertParamAtIndexInMinSEHdr sip_insertParamAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_deleteParamAtIndexInMinSEHdr
** DESCRIPTION: This function deletes a parameter at a specified
**				index in a MinSE header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the MinSE SipHeader
**				index(IN)		- The index to delete from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_deleteParamAtIndexInMinSEHdr sip_deleteParamAtIndexInCommonHdr

/****************************************************************************
** FUNCTION: sip_setParamAtIndexInMinSEHdr
** DESCRIPTION: This function sets a parameter at a specified
**				index in a MinSE header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the MinSE SipHeader
**				pParam(IN)		- The param to set
**				index(IN)		- The index to set at
**				pErr(OUT)		- Error value if any
**
** NOTE: 
****************************************************************************/

#define sip_setParamAtIndexInMinSEHdr sip_setParamAtIndexInCommonHdr

/* APIs for Session-Expires header */

#define sip_getSecondsFromSessionExpiresHdr				sip_getdIntVar1FromCommonHdr
#define sip_setSecondsInSessionExpiresHdr				sip_setdIntVar1InCommonHdr
#define	sip_getParamCountFromSessionExpiresHdr \
                sip_getParamCountFromCommonHdr
#define	sip_getParamAtIndexFromSessionExpiresHdr \
                sip_getParamAtIndexFromCommonHdr
#define sip_setParamAtIndexInSessionExpiresHdr \
                sip_setParamAtIndexInCommonHdr
#define sip_insertParamAtIndexInSessionExpiresHdr \
                sip_insertParamAtIndexInCommonHdr
#define sip_deleteParamAtIndexInSessionExpiresHdr \
                sip_deleteParamAtIndexInCommonHdr

#endif /* SIP_SESSIONTIMER */

/****************************************************************************
** Function: sip_rpr_getRespNumFromRSeqHdr 
** Description:  get response number from RSeq header
** Parameters:
**		pHdr (IN)		- RSeq Header
**		pRespNum (OUT)	- retrieved response number
**		pErr (OUT)		- possible error value (see api ref. document)
****************************************************************************/
#define sip_rpr_getRespNumFromRSeqHdr sip_getdIntVar1FromCommonHdr

/****************************************************************************
** Function: sip_rpr_setRespNumInRSeqHdr 
** Description:  set response number in RSeq header
** Parameters:
**		pHdr (IN/OUT)	- RSeq Header
**		respNum (IN)	- response number to set
**		pErr (OUT)		- possible error value (see api ref. document)
****************************************************************************/
#define sip_rpr_setRespNumInRSeqHdr sip_setdIntVar1InCommonHdr

/****************************************************************************
** Function: sip_rpr_getRespNumFromRAckHdr 
** Description:  get Response number from RAck header
** Parameters:
**		pHdr (IN)		- RAck header
**		pRespNum (OUT)	- retrieved response number
**		pErr (OUT)		- possible error value (see api ref. document)
****************************************************************************/
#define sip_rpr_getRespNumFromRAckHdr sip_getdIntVar1FromCommonHdr

/****************************************************************************
** Function: sip_rpr_setRespNumInRAckHdr 
** Description:  set Response number in RAck header
** Parameters:
**		pHdr (IN/OUT) 	- RAck header
**		respNum (IN)	- response number to set
**		pErr (OUT)		- possible error value (see api ref. document)
****************************************************************************/
#define sip_rpr_setRespNumInRAckHdr sip_setdIntVar1InCommonHdr 

/****************************************************************************
** Function: sip_rpr_getCseqNumFromRAckHdr 
** Description:  get CSeq number from RAck header
** Parameters:
**		pHdr (IN)	- Rack header
**		pCseq (OUT)	- retrieved seq. number
**		pErr (OUT)	- possible error value (see api ref. document)
****************************************************************************/

#define sip_rpr_getCseqNumFromRAckHdr sip_getdIntVar2FromCommonHdr

/****************************************************************************
** Function: sip_rpr_setCseqNumInRAckHdr 
** Description:  set CSeq number in RAck header
** Parameters:
**		pHdr (IN/OUT)	- Rack header
**		cSeq (IN)		- seq. number to set
**		pErr (OUT)		- possible error value (see api ref. document)
**
****************************************************************************/
#define sip_rpr_setCseqNumInRAckHdr sip_setdIntVar2InCommonHdr 

/****************************************************************************
** Function: sip_rpr_getMethodFromRAckHdr 
** Description:  get Method value from RAck header
** Parameters:
**		pHdr (IN)		- Rack Header
**		ppMethod (OUT)	-  retrieved header method
**		pErr (OUT)		- possible error value (see api ref. document)
****************************************************************************/
#define sip_rpr_getMethodFromRAckHdr sip_getStr1FromCommonHdr 

/****************************************************************************
** Function: sip_rpr_setMethodInRAckHdr 
** Description:  set Method in RAck header
** Parameters:
**		pHdr (IN/OUT)		- Rack Header
**		pMethod (IN)		- method value to set
**		pErr (OUT)			- possible error value (see api ref. document)
****************************************************************************/
#define sip_rpr_setMethodInRAckHdr sip_setStr1InCommonHdr 

/****************************************************************************
** FUNCTION: sip_getParamAtIndexFromContactHdr
** DESCRIPTION: This function gets a param at a specified
**				index from a Contact header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Contact SipHeader
**				ppParam(OUT)	- The retrieved param
**				index(IN)		- The index from which to retrieve
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_getParamAtIndexFromContactHdr sip_getParamAtIndexFromCommonHdr 

/****************************************************************************
** FUNCTION: sip_insertParamAtIndexInContactHdr
** DESCRIPTION: This function inserts an param at a specified index
**				in a Contact header.

** PARAMETERS:
**				pHdr(IN)		- Pointer to the Contact SipHeader
**				pParam(IN)		- The param to be inserted
**				index(IN)		- The index at which to insert
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_insertParamAtIndexInContactHdr sip_insertParamAtIndexInCommonHdr 

/****************************************************************************
** FUNCTION: sip_setParamAtIndexInContactHdr
** DESCRIPTION: This function sets an param at a specified
**				index in a Contact header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Contact SipHeader
**				pParam(IN)		- The param to be set
**				index(IN)		- The index at which to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_setParamAtIndexInContactHdr sip_setParamAtIndexInCommonHdr 

/****************************************************************************
** FUNCTION: sip_deleteParamAtIndexInContactHdr
** DESCRIPTION: This function deletes a param at a specified index 
**				in a Contact header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Contact SipHeader
**				index(IN)		- The index from which to delete
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_deleteParamAtIndexInContactHdr sip_deleteParamAtIndexInCommonHdr

#ifdef SIP_DATE
/*********************************************************
** FUNCTION:sip_getTypeFromExpiresHdr
**
** DESCRIPTION: This function retrieves the dType of a SIP
**		Expires pHeader
**
**********************************************************/
/* Expires type would be replaced by hash defines */
#define sip_getTypeFromExpiresHdr sip_getdIntVar1FromCommonHdr


/*********************************************************************
** FUNCTION:   sip_getTypeFromRetryAfterHdr
**********************************************************************
**
** DESCRIPTION: Gets the dType pValue from retry after pHeader
**
*********************************************************************/
#define sip_getTypeFromRetryAfterHdr sip_getdIntVar1FromCommonHdr
#endif /* SIP_DATE */

#ifdef SIP_CORE_STACK_COMPATIBLE

/***********************************************************************
** Function: sip_getCredentialsFromAuthorizationHdr 
** Description: sets the Credential type in the Sip Authorisation Header
** Parameters:
**			pHdr(IN/OUT)	- Sip Authorization Header
**			pHtype(IN)		- The header type set
**			pErr(OUT)		- Possible Error value (see API ref doc)
**
************************************************************************/
#ifdef SIP_AUTHENTICATE
extern SipBool sip_getCredentialsFromAuthorizationHdr\
			(SipHeader *pHdr, SipGenericCredential **pCredintial,\
			 SipError *pError);

/***********************************************************************
** FUNCTION: sip_getCredentialsFromProxyAuthorizationHdr 
** Description: gets the Credential from the ProxyAuthorization Header
** Parameters:
**			pHdr(IN)			- Sip ProxyAuthorization Header
**			credentials(OUT)	- The Credentials retrieved
**			pErr(OUT)			- Possible Error value (see API ref doc)
**
************************************************************************/
#define sip_getCredentialsFromProxyAuthorizationHdr \
	sip_getCredentialsFromAuthorizationHdr

/***********************************************************************
** Function: sip_setCredentialsInAuthorizationHdr 
** Description: sets the Credential type in the Sip Authorisation Header
** Parameters:
**			pHdr(IN/OUT)	- Sip Authorization Header
**			pHtype(IN)		- The header type set
**			pErr(OUT)		- Possible Error value (see API ref doc)
**
************************************************************************/
extern SipBool sip_setCredentialsInAuthorizationHdr _ARGS_((SipHeader *hdr,\
		SipGenericCredential *cr, SipError *err));


/***********************************************************************
** FUNCTION: sip_setCredentialsInProxyAuthorizationHdr 
** Description: sets the Credential in the ProxyAuthorization Header
** Parameters:
**			pHdr(IN/OUT)		- Sip ProxyAuthorization Header
**			credentials(IN)		- The Credentials to set
**			pErr(OUT)			- Possible Error value (see API ref doc)
**
************************************************************************/
#define sip_setCredentialsInProxyAuthorizationHdr \
		sip_setCredentialsInAuthorizationHdr


extern SipBool sip_getChallengeFromWwwAuthenticateHdr\
			(SipHeader *pSipWwwAuthenticateHeader,\
			SipGenericChallenge **pChallenge,SipError *pError);

#define sip_getChallengeFromProxyAuthenticateHdr sip_getChallengeFromWwwAuthenticateHdr

/***********************************************************************
** Function: sip_setChallengeInWwwAuthenticateHdr
** Description: set Challenge field in WWWAuthenticate Header
** Parameters:
**				hdr(IN/OUT)		- Sip WWWAuthenticate Header
**				challenge(IN) 	- Challenge to set
**				err(OUT)    	- Possible error value (See API ref doc)
************************************************************************/
extern SipBool sip_setChallengeInWwwAuthenticateHdr _ARGS_(( SipHeader *hdr, \
		SipGenericChallenge *challenge, SipError *err));

#define sip_setChallengeInProxyAuthenticateHdr sip_setChallengeInWwwAuthenticateHdr
			
#endif

/* SS: The logic inside Contact Header is different than other headers. So
 * keeping it as such. Would need to analyse it if to be replaced */
/***********************************************************************
** FUNCTION: sip_getContactParamAtIndexFromContactHdr
** Description:gets the Contact Parameters at an index1 from the Sip Contact Header
** Parameters:
**			hdr(IN) 			- Sip Contact Header
**			conatct_param(OUT)	- ContactParameter to retrieve
**			index1(IN)			- Index at which the Contact Param is retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_getContactParamAtIndexFromContactHdr _ARGS_ ((\
	SipHeader *hdr,SipContactParam **ppContactParam, SIP_U32bit index1, \
		SipError *err));

/***********************************************************************
** FUNCTION: sip_insertContactParamAtIndexInContactHdr
** Description:insert the Contact Parameters at an index1 in the Sip Contact Header
** Parameters:
**			hdr(IN/OUT) 			- Sip Contact Header
**			conatct_param(IN)	- ContactParameter to insert
**			index1(IN)			- Index at which the Contact Parameter is inserted
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_insertContactParamAtIndexInContactHdr _ARGS_ ((SipHeader *hdr, \
	SipContactParam *contact_param, SIP_U32bit index1, SipError *err));




/***********************************************************************
** FUNCTION: sip_deleteContactParamAtIndexInContactHdr
** Description:delete the Contact Parameter at an index1 in the Sip Contact Header
** Parameters:
**			hdr(IN/OUT) 			- Sip Contact Header
**			index1(IN)			- Index at which the Contact Parameter is deleted
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_deleteContactParamAtIndexInContactHdr  sip_deleteParamAtIndexInCommonHdr

/***********************************************************************
** FUNCTION: sip_setExpiresInContactParam 
** Description: sets the Expires in the Contact Parameter
** Parameters:
**			hdr(IN/OUT)			- Sip Contact Parameter
**			pExpires(IN)		- The Expires field to set
**			pErr(OUT)			- Possible Error value (see API ref doc)
**
************************************************************************/
extern SipBool sip_setExpiresInContactParam _ARGS_((SipContactParam *hdr,\
		SipExpiresStruct *expires, SipError *err));

/***********************************************************************
** FUNCTION: sip_setContactParamAtIndexInContactHdr
** Description:set the Contact Parameters at an index1 in the Sip Contact Header
** Parameters:
**			hdr(IN/OUT) 			- Sip Contact Header
**			conatct_param(IN)	- ContactParameter to set
**			index1(IN)			- Index at which the Contact Parameter is set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
extern SipBool sip_setContactParamAtIndexInContactHdr _ARGS_ ((SipHeader *hdr, \
	SipContactParam *contact_param, SIP_U32bit index1, SipError *err));


#ifdef SIP_3GPP
/***********************************************************************
** FUNCTION: sip_getFeatureAttrFromContactParam 
** Description: gets the Feature attribute from the Contact Parameter
** Parameters:
**			hdr(IN)					- Sip Contact Parameter
**			FeatureAttribute(OUT)	- The Feature attribute to retrieve
**			pErr(OUT)				- Possible Error value (see API ref doc)
**
************************************************************************/
extern SipBool sip_getFeatureAttrFromContactParam _ARGS_((SipContactParam *hdr,\
		SIP_S8bit **FeatureAttrribute, SipError *err));

/***********************************************************************
** FUNCTION: sip_setExtensionAttrInContactParam 
** Description: sets the FeatureAttributes in the Contact Parameter
** Parameters:
**			hdr(IN/OUT)				- Sip Contact Parameter
**			FeatureAttribute(IN)	- The Feature attribute to set
**			pErr(OUT)				- Possible Error value (see API ref doc)
**
************************************************************************/
extern SipBool sip_setFeatureAttrInContactParam _ARGS_((SipContactParam *hdr,\
		SIP_S8bit *FeatureAttrribute, SipError *err));
#endif

/***********************************************************************
** Function: sip_getQvalueFromContactParam 
** Description: gets the Q value from the Contact Parameter
** Parameters:
**			hdr(IN)				- Sip Contact Parameter 
**			qvalue(OUT)			- The QValue to retrieve
**			pErr(OUT)			- Possible Error value (see API ref doc)
**
************************************************************************/
extern SipBool sip_getQvalueFromContactParam _ARGS_((SipContactParam *hdr,\
		SIP_S8bit **qvalue, SipError *err));

/***********************************************************************
** Function: sip_setQvalueInContactParam 
** Description: sets the Q value in the Contact Parameter
** Parameters:
**			hdr(IN/OUT)			- Sip Contact Parameter
**			qvalue(IN)			- The QValue to set
**			pErr(OUT)			- Possible Error value (see API ref doc)
**
************************************************************************/
extern SipBool sip_setQvalueInContactParam _ARGS_((SipContactParam *hdr,\
		SIP_S8bit *qvalue, SipError *err));


/********************************************************************
**
** FUNCTION:  validateSipContactParamsType
**
** DESCRIPTION:  This fucntion returns SipSuccess if "dType"
** is one among the defined en_ContactParamsType's; else it returns SipFail.
**
********************************************************************/
extern SipBool validateSipContactParamsType (en_ContactParamsType dType, SipError *err);

/***********************************************************************
** FUNCTION: sip_getChallengeFromCredential 
** Description: get Challenge from Credential
** Parameters:
**		cr (IN)		- Generic Credential
**		ch (OUT)	- retrieved challenge
**		pErr (OUT)	-	Possible Error value (see API ref doc)
************************************************************************/
#ifdef SIP_AUTHENTICATE
extern SipBool sip_getChallengeFromCredential _ARGS_((SipGenericCredential *cr,\
		SipGenericChallenge **ch, SipError *err));

/***********************************************************************
** FUNCTION: sip_setChallengeInCredential 
** Description: set challenge in credential 
** Parameters:
**		cr (IN/OUT)		- Generic Credential
**		ch (IN)			- Challenge token to set
**		pErr (OUT)		-	Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setChallengeInCredential _ARGS_((SipGenericCredential *cr,\
		SipGenericChallenge *ch, SipError *err));

/***********************************************************************
** FUNCTION: sip_getSchemeFromChallenge 
** Description: set scheme in Challenge
** Parameters:
**		ch (IN/OUT)		-  	Generic Challenge
**		sch (OUT)		- 	Scheme to get
**		pErr (OUT)		-	Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getSchemeFromChallenge _ARGS_((SipGenericChallenge *ch,\
		SIP_S8bit **sch, SipError *error));

/***********************************************************************
** FUNCTION: sip_setSchemeInChallenge 
** Description: set scheme in Challenge
** Parameters:
**		ch (IN/OUT)		-  	Generic Challenge
**		sch (IN)		- 	Scheme to set
**		pErr (OUT)		-	Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setSchemeInChallenge _ARGS_((SipGenericChallenge *ch,\
		SIP_S8bit *sch, SipError *error));

extern SipBool sip_getAuthorizationParamAtIndexFromChallenge _ARGS_\
	(( SipGenericChallenge *pSch,\
		SipParam **ppParam,SIP_U32bit dIndex, SipError 	*pErr));


/***********************************************************************
** FUNCTION: sip_insertAuthorizationParamAtIndexInChallenge 
** Description: insert authorization parameter at specified index1 in challenge
** Parameters:
**		sch (IN/OUT)	- Generic Challenge
**		param (IN)		- authorization parameter to insert
**		index1 (IN)		- index1 to insert at
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_insertAuthorizationParamAtIndexInChallenge _ARGS_\
	((SipGenericChallenge *sch,\
		SipParam *param,SIP_U32bit index1,SipError *error));

/***********************************************************************
** FUNCTION: sip_setAuthorizationParamAtIndexInChallenge 
** Description: set authorization parameter at specified index1 in challenge
** Parameters:
**		sch (IN/OUT)	- Generic Challenge
**		param (IN)		- authorization parameter to set
**		index1 (IN)		- index1 to set at
**		pErr (OUT)		- Possible Error value (see API ref doc)
**
************************************************************************/
extern SipBool sip_setAuthorizationParamAtIndexInChallenge _ARGS_((SipGenericChallenge *sch,\
		SipParam *param,SIP_U32bit index1,SipError *error));

/***********************************************************************
** FUNCTION: sip_getAuthorizationParamCountFromChallenge 
** Description: get number of authorization parameters in Challenge
** Parameters:
**		sch (IN)		- Generic Challenge
**		count (OUT)		- retrieved count of authorization parameters
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getAuthorizationParamCountFromChallenge _ARGS_((SipGenericChallenge *sch,\
		SIP_U32bit *count,SipError *error));

/***********************************************************************
** FUNCTION: sip_deleteAuthorizationParamAtIndexInChallenge 
** Description: delete Authorization parameter at specified index1 in challenge
** Parameters:
**		sch (IN/OUT)	- Generic Challenge
**		index1 (IN)		- index1 to delete from
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_deleteAuthorizationParamAtIndexInChallenge _ARGS_\
	((SipGenericChallenge *sch,\
		SIP_U32bit index1,SipError *error));


/***********************************************************************
** FUNCTION: sip_getCredentialTypeFromCredential 
** Description: get type of Credential
** Parameters:
**		cr (IN)		-  Generic Credential
**		type (OUT)	-  retrieved type of credential
**		pErr (OUT)	-	Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getCredentialTypeFromCredential _ARGS_((SipGenericCredential *cr,\
		en_CredentialType *type, SipError *err));

/***********************************************************************
** Function: sip_getBasicFromCredential 
** Description: get Basic token from credential
** Parameters:
**		cr (IN)		- Generic Credential
**		basic (OUT)	- retrieved basic token
**		pErr (OUT)	-	Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_getBasicFromCredential _ARGS_((SipGenericCredential *cr,\
		SIP_S8bit **basic, SipError *err));

/***********************************************************************
** Function: sip_setBasicInCredential 
** Description: set basic token in credentials
** Parameters:
**		cr (IN/OUT) - Generic Credential
**		basic(IN) 	- basic token to set
**		pErr (OUT)	-	Possible Error value (see API ref doc)
************************************************************************/
extern SipBool sip_setBasicInCredential _ARGS_((SipGenericCredential *cr,\
		SIP_S8bit *basic, SipError *err));
#endif		
/*******************************************************************************
** FUNCTION:sip_getTypeFromContactHdr
**
** DESCRIPTION: This function retrieves the dType of a SIP
**		Contact pHeader
** PARAMETERS:
**					hdr	- Sipheader
**				dType	- The contact type.
**  pErr				-	SipError
** ReturnValue 
**				SipBool
**
*******************************************************************************/
#define sip_getTypeFromContactHdr sip_getdIntVar1FromCommonHdr
#endif /* SIP_CORE_STACK_COMPATIBLE */


#ifdef SIP_DATE
#define sip_setDateStructInExpiresHdr sip_getDateFromCommonHdr

/* SS: These three APIs take SipExpiresStruct as argument. Could this be changed */
extern SipBool sip_getTypeFromExpires(SipExpiresStruct *hdr,en_ExpiresType *dType,\
	SipError *err);
extern SipBool sip_getDateStructFromExpires(SipExpiresStruct *hdr, \
		SipDateStruct **date_struct, SipError *err);
extern SipBool sip_setDateStructInExpires(SipExpiresStruct *hdr, \
		SipDateStruct *date_struct, SipError *err);
#endif /* SIP_DATE */


#ifndef SIP_CORE_STACK_COMPATIBLE
/*******************************************************************************
** FUNCTION	  : sip_getParamCountFromContactHdr
**
** DESCRIPTION: This function retrieves the number of contact pParam
**				from a SIP contact pHeader
** PARAMETERS:
**				hdr(IN)		- SipContactHeader
**				count(OUT) 	- Challenge to set
**				err(OUT)   	- Possible error value (See API ref doc)
** Return Value:
**				  SipBool	
*******************************************************************************/

#define sip_getParamCountFromContactHdr sip_getParamCountFromCommonHdr
#else
/*******************************************************************************
** FUNCTION	  : sip_getContactParamsCountFromContactHdr
**
** DESCRIPTION: This function retrieves the number of contact pParam
**				from a SIP contact pHeader
** PARAMETERS:
**				hdr(IN)		- SipContactHeader
**				count(OUT) 	- Challenge to set
**				err(OUT)   	- Possible error value (See API ref doc)
** Return Value:
**				  SipBool	
*******************************************************************************/

#define sip_getContactParamsCountFromContactHdr sip_getParamCountFromCommonHdr

#define sip_getParamCountFromContactHdr sip_getParamCountFromCommonHdr

#ifdef ACCTEST
#define sip_getContactParamCountFromContactHdr sip_getParamCountFromCommonHdr
#endif /* ACCTEST */

#endif /* ifndef SIP_CORE_STACK_COMPATIBLE */


/*****************************************************************
**
** FUNCTION:  sip_setSecondsInExpiresHdr
**
** DESCRIPTION: This function sets the seconds field in a SIP Expires
**		pHeader
**
***************************************************************/
extern SipBool sip_setSecondsInExpiresHdr _ARGS_ ((SipHeader *hdr, \
	SIP_U32bit dSec, SipError *err));
/*#ifdef STK_RELEASE*/
/*****************************************************************
**
** FUNCTION:  sip_getSecondsFromExpiresHdr
**
** DESCRIPTION: This function retrieves the seconds field from a SIP
**		Encryption pHeader
**
***************************************************************/
extern SipBool sip_getSecondsFromExpiresHdr _ARGS_ ((SipHeader *hdr, \
	SIP_U32bit *dSec, SipError *err));
/*#endif*/

/*****************************************************************
**
** FUNCTION:  sip_setDeltaSecondsInMinExpiresHdr
**
** DESCRIPTION: This function retrieves the seconds field from a SIP
**		Encryption pHeader
**
***************************************************************/
#define sip_setDeltaSecondsInMinExpiresHdr sip_setdIntVar1InCommonHdr

/*****************************************************************
**
** FUNCTION:  sip_getDeltaSecondsFromMinExpiresHdr
**
** DESCRIPTION: This function retrieves the seconds field from a SIP
**		Encryption pHeader
**
***************************************************************/
#define sip_getDeltaSecondsFromMinExpiresHdr sip_getdIntVar1FromCommonHdr


#if 0 /* Redefintion. Will remove after confirming */
#ifdef SIP_CORE_STACK_COMPATIBLE
/*****************************************************************
**
** FUNCTION:  sip_getTagCountFromFromHdr
**
** DESCRIPTION: This FUNCTION gets the number of Tag inserted in a
**		SIP From pHeader
** PARAMETERS:
**			hdr(IN) 			- Sip From Header
**			Count(OUT)			- The Count to retrieve
**			err(OUT)			- Possible error value(see API ref doc)
**
**************************************************************/
#define sip_getTagCountFromFromHdr(pHdr,pCount,pErr) \
            sip_getParamCountForNameFromCommonHdr(pHdr,"tag",pCount,pErr)

#define sip_getTagCountFromToHdr(pHdr,pCount,pErr) \
                sip_getTagCountFromFromHdr(pHdr,"tag",pCount,pErr)
/***********************************************************************
** FUNCTION: sip_getTagAtIndexFromFromHdr
** DESCRIPTION:gets the Tag at an index from the Sip From Header
** PARAMETERS:
**			hdr(IN) 			- Sip From Header
**			tag(OUT)			- The Tag to retrieve
**			index(IN)			- The index at which the tag is retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_getTagAtIndexFromFromHdr(pHdr,ppTag,index,pErr) \
            sip_getParamForNameFromCommonHdr(pHdr,"tag",pptag,pErr)

#define sip_getTagAtIndexFromToHdr(pHdr,ppTag,index,pErr) \
            sip_getTagAtIndexFromFromHdr(pHdr,"tag",pptag,pErr)
/***********************************************************************
** FUNCTION: sip_setTagAtIndexInFromHdr
** DESCRIPTION:sets a Tag at an index in the Sip From Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip From Header
**			tag(IN)				- The Tag to set
**			index(IN)			- The index at which the tag is set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_setTagAtIndexInFromHdr(pHdr,pTag,index,pErr) \
            sip_setParamForNameInCommonHdr(pHdr,"tag",pTag,pErr)

#define sip_setTagAtIndexInToHdr(pHdr,pTag,index,pErr) \
            sip_setParamForNameInCommonHdr(pHdr,"tag",pTag,pErr)

/***********************************************************************
** FUNCTION: sip_insertTagAtIndexInFromHdr
** DESCRIPTION:sets a Tag at an index in the Sip From Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip From Header
**			tag(IN)				- The Tag to set
**			index(IN)			- The index at which the tag is set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_insertTagAtIndexInFromHdr(pHdr,pTag,index,pErr) \
            sip_setParamForNameInCommonHdr(pHdr,"tag",pTag,pErr)
            
#define sip_insertTagAtIndexInToHdr(pHdr,pTag,index,pErr) \
            sip_setParamForNameInCommonHdr(pHdr,"tag",pTag,pErr)
/***********************************************************************
** FUNCTION: sip_deleteTagAtIndexInFromHdr
** DESCRIPTION:deletes a Tag at an index in the Sip From Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip From Header
**			index(IN)			- The index at which the tag is deleted
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
#define sip_deleteTagAtIndexInFromHdr(pHdr,index,pErr) \
            sip_deleteParamForNameInCommonHdr(pHdr,"tag",pErr)

#define sip_deleteTagAtIndexInToHdr(pHdr,index,pErr) \
            sip_deleteParamForNameInCommonHdr(pHdr,"tag",pErr)

#endif /* SIP_CORE_STACK_COMPATIBLE */
#endif


/*****************************************************************
**
** FUNCTION:  sip_getNameFromUnknownHdr
**
** DESCRIPTION: This function retrieves the pName from an Unknown
**		SIP pHeader
**
***************************************************************/
#define sip_getNameFromUnknownHdr sip_getStr1FromCommonHdr 

/*****************************************************************
**
** FUNCTION:  sip_setNameInUnknownHdr
**
** DESCRIPTION: This function sets the pName of an Unknown SIP pHeader
**
***************************************************************/
#define sip_setNameInUnknownHdr sip_setStr1InCommonHdr

/*****************************************************************
**
** FUNCTION:  sip_getBodyFromUnknownHdr
**
** DESCRIPTION: This function retrieves the pBody from an Unknown SIP
**		pHeader
**
***************************************************************/
#define sip_getBodyFromUnknownHdr sip_getStr2FromCommonHdr 

/************************************************************************
**
** FUNCTION:  sip_setBodyInUnknownHdr
**
** DESCRIPTION: This function sets the pBody in an Unknown SIP pHeader
**
************************************************************************/
#define sip_setBodyInUnknownHdr sip_setStr2InCommonHdr 


/********************************************************************
**
** FUNCTION:  sip_deleteAllHeaderType
**
** DESCRIPTION: This function deletes all headers of en_HeaderType 
** "dType" from a SipMessage "msg".
**
*********************************************************************/
extern SipBool sip_deleteAllHeaderType _ARGS_ ((SipMessage *msg, \
	en_HeaderType dType, SipError *err));


/******************************************************************
**
** FUNCTION:  deleteAllGeneralHdr
**
** DESCRIPTION: This function deletes all general headers of
**	en_HeaderType "dType".
**
******************************************************************/
extern SipBool deleteAllGeneralHdr _ARGS_ ((SipGeneralHeader *hdr, \
	en_HeaderType dType, SipError *err));


/******************************************************************
**
** FUNCTION:  deleteAllRequestHdr
**
** DESCRIPTION:  This function deletes all request headers of
**	en_HeaderType "dType".
**
******************************************************************/
extern SipBool deleteAllRequestHdr _ARGS_ ((SipReqHeader *hdr, \
	en_HeaderType dType, SipError *err));


/******************************************************************
**
** FUNCTION:  deleteAllResponseHdr
**
** DESCRIPTION: This function deletes all response headers of en_HeaderType "dType".
**
******************************************************************/
extern SipBool deleteAllResponseHdr _ARGS_ ((SipRespHeader *hdr, \
	en_HeaderType dType, SipError *err));


/********************************************************************
**
** FUNCTION:  sip_parseSingleHeader
**
** DESCRIPTION: This function parses a header string to form a SipHeader
**		structure containing the parsed header. This function can only
**		be used to parse known headers.
** 
** PARAMETERS:
**	pHeaderStr(IN) - The string containing the header to be parsed.
**	dType(IN) - The type of of the header string that is to be parsed.
**		For types like Contact or Expires, the Any type should be used.
**		The function will return fail if this type does not match with 
**		the type of the header given in the string.
**	pHeader(OUT) - The header structure which will contain the parsed
**		header on return. This structure should be initialized using
**		the sip_initSipHeader function with header type SipHdrTypeAny.
**	pErr(OUT) - Error indication in case of failure.
**
*********************************************************************/

extern SipBool sip_parseSingleHeader _ARGS_ ((SIP_S8bit *pHeaderStr, \
	en_HeaderType dType, SipHeader *pHeader, SipError *pErr));

/*****************************************************************
**
** FUNCTION:  sip_getBodyFromBadHdr
**
** DESCRIPTION: This function retrieves the body from an Unknown SIP
**		header
**
***************************************************************/
#ifdef  SIP_BADMESSAGE_PARSING
extern SipBool sip_getBodyFromBadHdr _ARGS_ ((SipBadHeader *hdr, SIP_S8bit **pBody, \
			SipError *err));

/*****************************************************************
**
** FUNCTION:  sip_getNameFromBadHdr
**
** DESCRIPTION: This function retrieves the name from an Unknown
**		SIP header
**
***************************************************************/
extern SipBool sip_getNameFromBadHdr _ARGS_ ((SipBadHeader *hdr, \
	SIP_S8bit **pName, SipError *err));

/*****************************************************************
**
** FUNCTION:  sip_getTypeFromBadHdr
**
** DESCRIPTION: This function retrieves the header type from the bad
**		header. This is the type corresponding to the name of the
**		header.
**
***************************************************************/
extern SipBool sip_getTypeFromBadHdr _ARGS_ ((SipBadHeader *hdr, \
	en_HeaderType *pType, SipError *err));

/********************************************************************
**
** FUNCTION:  sip_getBadHeaderCount
**
** DESCRIPTION: This function returns the number of bad headers in the 
**	SIP message. Messages with bad headers are returned when a syntactically
**	incorrect message is passed to sip_decodeMessage with the SIP_OPT_BADMESSAGE
**	option.
** PARAMETERS:
**	pMessage (IN) - The SIP message that is to be examined for the presence of
**		bad headers
**	pCount (OUT) - Returns the number of bad headers in the message.
**	pError (OUT) - Returns the error code in case of failure.
**
*********************************************************************/
extern SipBool sip_getBadHeaderCount _ARGS_ ((SipMessage *pMessage, \
	SIP_U32bit *pCount, SipError *pError));

/********************************************************************
**
** FUNCTION:  sip_getBadHeaderAtIndex
**
** DESCRIPTION: This function returns a bad header from a SIP message
**	Messages with bad headers are returned when a syntactically
**	incorrect message is passed to sip_decodeMessage with the 
**	SIP_OPT_BADMESSAGE option.
** PARAMETERS:
**	pMessage (IN) - The SIP message that is to be examined for the presence of
**		bad headers
**	ppBadHdr (OUT) - Returns the bad header from the message.
**	index (IN) - This index of the bad header to be returned.
**	pError (OUT) - Returns the error code in case of failure.
**
*********************************************************************/
extern SipBool sip_getBadHeaderAtIndex _ARGS_ ((SipMessage *pMessage, \
	SipBadHeader **ppBadHdr, SIP_U32bit index, SipError *pError));
#endif

/* MICROUA begin */
extern void sip_getTypeFromName _ARGS_ ((SIP_S8bit *pName, en_HeaderType *pType, \
	SipError *pError));

/********************************************************************
**
** FUNCTION:  sip_getHeaderAsStringAtIndex
**
** DESCRIPTION:  This function retrieves a SipHeader of en_HeaderType 
** "dType" in exactly the same way as sip_getHeaderAtIndex; 
** the output pHeader here is in the form of a string "hdr".

**
*********************************************************************/
extern SipBool sip_getHeaderAsStringAtIndex _ARGS_ ((SipMessage *s, \
	en_HeaderType dType,  SIP_S8bit **hdr, SIP_U32bit index, SipError *err));


#ifdef SIP_SESSIONTIMER
/*****************************************************************
**
** FUNCTION:  sip_getRefresherFromSessionExpiresHdr
**
** DESCRIPTION: This function retrieves the refresher field from a SIP
**		SessionExpires pHeader
**
***************************************************************/
extern SipBool sip_getRefresherFromSessionExpiresHdr _ARGS_ ((SipHeader *pHdr, \
	SIP_S8bit **ppRefresher, SipError *pErr));


/*****************************************************************
**
** FUNCTION:  sip_setRefresherInSessionExpiresHdr
**
** DESCRIPTION: This function sets the refresher field in  SIP
**		SessionExpires pHeader
**
***************************************************************/
#define sip_setRefresherInSessionExpiresHdr(pHdr,pRefresher,pErr) \
                sip_setParamForNameInCommonHdr(pHdr,"refresher",pRefresher,pErr)

#endif /* SIP_SESSIONTIMER */


/********************************************************************
**
** FUNCTION:  sdp_setSdpOriginFromString
**
** DESCRIPTION: This function sets the "o=" line of the 
**				SDP message (taken as input) from the string provided.
**
*********************************************************************/
extern SipBool sdp_setSdpOriginFromString _ARGS_ ((SdpMessage *pSdpMsg, \
	SIP_S8bit *pStr, SipError *pErr));

/********************************************************************
**
** FUNCTION:  sdp_setSdpConnectionFromString
**
** DESCRIPTION: This function sets the SESSION LEVEL "c=" line of the 
**				SDP message (taken as input) from the string provided.
**
*********************************************************************/
extern SipBool sdp_setSdpConnectionFromString _ARGS_ ((SdpMessage *pSdpMsg, \
	SIP_S8bit *pStrConn, SipError *pErr));


/********************************************************************
**
** FUNCTION:  sdp_insertConnectionFromStringAtIndexInMedia
**
** DESCRIPTION: This function 'inserts' the MEDIA LEVEL "c=" line  
**				(at index: dIndex) from the string provided.
**
*********************************************************************/
SipBool sdp_insertConnectionFromStringAtIndexInMedia
(SdpMedia *pSdpMedia, SIP_S8bit *pStrConn, SIP_U32bit dIndex, SipError *pErr);

SipBool sdp_insertAttrFromStringAtIndexInMedia
(SdpMedia *pSdpMedia, SIP_S8bit *pStr, SIP_U32bit dIndex, SipError *pErr);

SipBool sdp_insertSdpAttrAtIndexFromString
(SdpMessage *pSdpMsg, SIP_S8bit *pStr, SIP_U32bit dIndex, SipError *pErr);

SipBool sip_insertHeaderFromStringAtIndex
(SipMessage *msg, en_HeaderType dType, SIP_S8bit *hdr, SIP_U32bit index, SipError *err);

SipBool sdp_insertSdpTimeAtIndexFromString
(SdpMessage *pSdpMsg, SIP_S8bit *pStr, SIP_U32bit dIndex, SipError *pErr);

SipBool sip_setHeaderFromStringAtIndex
(SipMessage *msg, en_HeaderType dType, SIP_S8bit *hdr, SIP_U32bit index, SipError *err);

SipBool sip_setAddrSpecFromString
(SipAddrSpec **ppAddrSpec, SIP_S8bit *pStr, SipError *pErr);

SipBool sip_checkNameParam _ARGS_((SIP_S8bit ch,SIP_S8bit* pname));

SipBool sip_checkContactFeatureParam _ARGS_ ((SIP_S8bit* pname));

/* MICROUA end */

#endif /* __MICROSIP_HEADER_H__*/

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
	}
#endif
