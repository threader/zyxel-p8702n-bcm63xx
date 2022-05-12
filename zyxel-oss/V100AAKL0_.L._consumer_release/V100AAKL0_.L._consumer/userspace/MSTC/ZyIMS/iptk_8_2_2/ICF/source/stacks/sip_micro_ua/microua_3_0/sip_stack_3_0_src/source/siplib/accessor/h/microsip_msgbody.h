/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/***********************************************************************
** FUNCTION:
**			Prototypes for Message Body Accessor API functionality
************************************************************************
**
** FILENAME:
** microsip_msgbody.h
**
** DESCRIPTION:
**		This file contains the prototypes for the message body accessor
**		functions.
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 9/09/2002	Aparna Kuppachi		-			Initial					
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/

#ifndef __MICROSIP_MSGBODY_H__
#define __MICROSIP_MSGBODY_H__

#include "microsip_struct.h"
#include "microsip_common.h"
#include "microsip_portlayer.h"


typedef enum
{
    SipParamString,
    SipParamIntVar,
    SipSdpParamConnection,
    SipSdpParamOrigin,
    SipSdpParamMedia,
    SipSdpParamTime,
    SipSdpParamAttribute,
    SipSdpWhole,
	SipAppSipMessage,
	SipUnknownWhole,
	SipMsgBodyWhole
}en_SipMsgBodyParamType;


  /* 
  * NOTE:  The fixes for the macros under the SIP_LINT flag 
	* is done to avoid LINT warning: 506 constant boolean value
	*/
 
#ifdef SIP_LINT
#define SIP_GET_PARAM_BY_VAL(pMsg,Param,pOutParam,pErr)  \
     ((pMsg == SIP_NULL)?\
	 (*pErr = E_INV_PARAM,SipFail):\
	 (*pOutParam = (pMsg)->Param,SipSuccess))
#else

#define SIP_GET_PARAM_BY_VAL(pMsg,Param,pOutParam,pErr)  \
    ((pErr==SIP_NULL)?SipFail:\
	(((pMsg == SIP_NULL)||(pOutParam == SIP_NULL))?\
	 (*pErr = E_INV_PARAM,SipFail):\
	 (*pOutParam = (pMsg)->Param,SipSuccess)))
#endif	

#define SIP_SET_PARAM_BY_VAL(pMsg,Param,InParam,pErr)  \
    ((pErr==SIP_NULL)?SipFail:\
	((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
    ((pMsg)->Param = InParam,SipSuccess)))
	

#ifdef SIP_LINT
#define SIP_GET_PARAM_BY_REF(pMsg,pParam,ppOutParam,pErr,dType)  \
    ((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
	(SipFail == sip_getParam((void *)((pMsg)->pParam),(void **)ppOutParam, \
				dType,pErr))?SipFail:SipSuccess)
#else
#define SIP_GET_PARAM_BY_REF(pMsg,pParam,ppOutParam,pErr,dType)  \
    ((pErr==SIP_NULL)?SipFail:\
	((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
	(SipFail == sip_getParam((void *)((pMsg)->pParam),(void **)ppOutParam, \
				dType,pErr))?SipFail:SipSuccess))
#endif
#define SIP_SET_PARAM_BY_REF(pMsg,pParam,pInParam,pErr,dType)  \
    ((pErr==SIP_NULL)?SipFail:\
	((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
	(SipFail == sip_setParam((void *)pInParam,(void **)&((pMsg)->pParam), \
				dType,pErr))?SipFail:SipSuccess))
#ifdef SIP_LINT
#define SIP_GET_LIST_AT_INDEX(pMsg,List,ppData,index,pErr,dType)  \
   	((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
	(SipFail == sip_getListAtIndex(&((pMsg)->List),(void **)(ppData), \
				index,dType,pErr))?SipFail:SipSuccess)
#else
#define SIP_GET_LIST_AT_INDEX(pMsg,List,ppData,index,pErr,dType)  \
    ((pErr==SIP_NULL)?SipFail:\
	((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
	(SipFail == sip_getListAtIndex(&((pMsg)->List),(void **)(ppData), \
				index,dType,pErr))?SipFail:SipSuccess))
#endif
#define SIP_SET_LIST_AT_INDEX(pMsg,List,pData,index,pErr,dType)  \
    ((pErr==SIP_NULL)?SipFail:\
	((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
	(SipFail == sip_setListAtIndex(&((pMsg)->List),(void *)(pData), \
				index,dType,pErr))?SipFail:SipSuccess))

#define SIP_INSERT_LIST_AT_INDEX(pMsg,List,pData,index,pErr,dType)  \
    ((pErr==SIP_NULL)?SipFail:\
	((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
	(SipFail == sip_insertListAtIndex(&((pMsg)->List),(void *)(pData), \
				index, dType, pErr))?SipFail:SipSuccess))

#define SIP_DELETE_LIST_AT_INDEX(pMsg,List,index,pErr)  \
    ((pErr==SIP_NULL)?SipFail:\
	((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
	(SipFail == sip_deleteListAtIndex(&((pMsg)->List),index, \
			  pErr))?SipFail:SipSuccess))
#ifdef SIP_LINT
#define SIP_GET_LIST_COUNT(pMsg,List,pCount,pErr) \
	((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
	(SipFail == sip_getListCount(&((pMsg)->List),pCount, \
				 pErr))?SipFail:SipSuccess)
#else
#define SIP_GET_LIST_COUNT(pMsg,List,pCount,pErr) \
	((pErr==SIP_NULL)?SipFail:\
	((pMsg == SIP_NULL)?(*pErr = E_INV_PARAM,SipFail):\
	(SipFail == sip_getListCount(&((pMsg)->List),pCount, \
				 pErr))?SipFail:SipSuccess))
#endif
/********************************************************************
** FUNCTION:sip_incrementRefCount
**
** DESCRIPTION: This functon increments the refcount of source field 
**
**********************************************************************/
extern SipBool sip_incrementRefCount _ARGS_
	((SIP_Pvoid          pSrc, 
	 en_SipMsgBodyParamType   dType,
	 SipError           *pErr)); 

/********************************************************************
** FUNCTION:sip_freeParam
**
** DESCRIPTION: This functon increments refcount depending on the type
**
**********************************************************************/ 
extern SipBool sip_freeParam _ARGS_
	((SIP_Pvoid          pSrc, 
	 en_SipMsgBodyParamType   dType,
	 SipError           *pErr)); 

/********************************************************************
** FUNCTION:sip_getParam
**
** DESCRIPTION: This functon retrieves the OUT param field from the IN
**				Param field
**
**********************************************************************/

extern SipBool sip_getParam _ARGS_
((   void                *pInParam, 
    void                **ppOutParam, 
    en_SipMsgBodyParamType    dType,
    SipError            *pErr 
));

/********************************************************************
** FUNCTION:sip_setParam
**
** DESCRIPTION: This function sets the OUT Param field from the IN 
**				Param field
**
**********************************************************************/
extern SipBool sip_setParam _ARGS_
((   void                *pInParam, 
    void                **ppOutParam, 
    en_SipMsgBodyParamType    dType,
    SipError            *pErr 
));

/****************************************************************************
** FUNCTION: sip_getListAtIndex
** DESCRIPTION: This function gets the element at an index from SipList 
** 
** PARAMETERS:
**				pList(IN)	    - Pointer to SipList
**              ppData(OUT)     - The retrieved element
**              index(IN)       - Index for which the element is to be retrieved
**              dType           - Type of param that is to be retrieved
**				pErr(OUT)		- Error value if any 
**
****************************************************************************/
SipBool sip_getListAtIndex _ARGS_
    ((SipList          *pList, 
     void             **ppData, 
     SIP_U32bit       index, 
     en_SipMsgBodyParamType dType,
     SipError         *pErr)); 

/********************************************************************
** FUNCTION:sip_setListAtIndex
**
** DESCRIPTION: This function sets the list field at a specified
**		index in a list structure
**
**********************************************************************/

SipBool sip_setListAtIndex _ARGS_
    (( SipList       *pList, 
	  SIP_Pvoid     *pData, 
      SIP_U32bit    index, 
     en_SipMsgBodyParamType dType,
      SipError      *pErr ));

/***************************************************************************
** FUNCTION: sip_insertListAtIndex
**
** DESCRIPTION: This function inserts a list field at a specified
**		index in a list structure
**
****************************************************************************/
SipBool sip_insertListAtIndex _ARGS_
	(( SipList       *pList,
      SIP_Pvoid     pData, 
      SIP_U32bit    index, 
     en_SipMsgBodyParamType dType,
      SipError      *pErr ));

/********************************************************************
** FUNCTION:sip_deleteListAtIndex
**
** DESCRIPTION: This function deletes a list field at a specified
**		index 
**
**********************************************************************/

SipBool sip_deleteListAtIndex _ARGS_
	(( SipList       *pList,
      SIP_U32bit    index, 	  
      SipError      *pErr ));

/********************************************************************
** FUNCTION:sip_getListCount
**
** DESCRIPTION: This function retrieves the List count 
**
**********************************************************************/

SipBool sip_getListCount _ARGS_
	(( SipList       *pList,
      SIP_U32bit    *pCount, 
      SipError      *pErr ));



/****************************************************************************
** FUNCTION: sip_getMsgBodyAtIndex
** DESCRIPTION: This function retrieves the msg body present at a specified
**				index from a SipMessage.
** PARAMETERS:
**				pMsg(IN)		- Pointer to SipMessage 
**				ppMsgBody(OUT)	- The retrieved message body 
**				index(IN)		- The index to retrieve from 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getMsgBodyAtIndex(pMsg,ppMsgBody,index,pErr) \
	SIP_GET_LIST_AT_INDEX(pMsg,slMessageBody,(void **)ppMsgBody,index,\
		pErr,SipMsgBodyWhole)

/****************************************************************************
** FUNCTION: sip_setMsgBodyAtIndex
** DESCRIPTION: This function sets a msg body at a specified index in a 
**				SipMessage. Any message body already existing at that 
**				index gets overwritten.
** 
** PARAMETERS:
**				pMsg(IN)		- Pointer to SipMessage 
**				pMsgBody(IN)	- The msg body to be set 
**				index(IN)		- The index at which to set 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_setMsgBodyAtIndex(pMsg,pMsgBody,index,pErr) \
	SIP_SET_LIST_AT_INDEX((SipMessage *)pMsg,slMessageBody,(void *)pMsgBody,index,\
		pErr,SipMsgBodyWhole)

/****************************************************************************
** FUNCTION: sip_insertMsgBodyAtIndex
** DESCRIPTION: This function inserts a msg body at a specified index in a 
**				SipMessage. 
** 
** PARAMETERS:
**				pMsg(IN)		- Pointer to SipMessage 
**				pMsgBody(IN)	- The msg body to be inserted 
**				index(IN)		- The index at which to insert 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_insertMsgBodyAtIndex(pMsg,pMsgBody,index,pErr) \
	SIP_INSERT_LIST_AT_INDEX((SipMessage *)pMsg,slMessageBody,(void *)pMsgBody,index,\
		pErr,SipMsgBodyWhole)

/****************************************************************************
** FUNCTION: sip_getTypeFromMsgBody
** DESCRIPTION: This function returns the dType of a given msg body 
** 				(Sdp / MultipartMime / Isup /AppSip / Unknown).
** 
** PARAMETERS:
**				pMsgBody(IN)	- Pointer to message body 
**				pType(OUT)		- The retreived dType of msg body 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getTypeFromMsgBody(pMsgBody,pType,pErr)\
	SIP_GET_PARAM_BY_VAL((SipMsgBody *)pMsgBody,dType,pType,pErr)

/****************************************************************************
** FUNCTION: sip_getSipMessageFromMsgBody
** DESCRIPTION: This function retrieves the SIP message element from 
**				the SipMsgBody structure. This function should be used only
**				if the type of the message body is "SipAppSipBody".
** 
** PARAMETERS:
**				pMsgBody(IN)	- Pointer to msg body 
**				ppSipMesg(OUT)	- Pointer to retrieved SipMessage inside the
**								  msg body
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_getSipMessageFromMsgBody(pMsgBody,ppOutSipMsg,pErr) \
	SIP_GET_PARAM_BY_REF((SipMsgBody *)pMsgBody,u.pAppSipMessage,ppOutSipMsg,pErr,SipAppSipMessage)

/****************************************************************************
** FUNCTION: sip_setSipMessageInMsgBody
** DESCRIPTION: This function sets a SIP message element  in
**	the SipMsgBody structure.
** 
** PARAMETERS:
**				pMsgBody(IN)	- Pointer to msg body 
**				pSipMesg(IN)	- Pointer to the SIP message to be set 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_setSipMessageInMsgBody(pMsgBody,pInSipMsg,pErr) \
	SIP_SET_PARAM_BY_REF((SipMsgBody *)pMsgBody,u.pAppSipMessage,pInSipMsg,pErr,SipAppSipMessage)

/****************************************************************************
** FUNCTION: sip_getMsgBodyTypeAtIndex
** DESCRIPTION: This function retreives the dType of the msg body 
**				(Sdp / MultipartMime / Isup /AppSip / Unknown)
**				present at a specified index in a SipMessage.
** 
** PARAMETERS:
**				pMsg(IN)		- Pointer to SipMessage 
**				pType(OUT)		- The retrieved msg body type 
**				index(IN)		- The index of the msg body whose type is to 
**								  be retrieved
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

extern SipBool sip_getMsgBodyTypeAtIndex
(SipMessage *pMsg, en_SipMsgBodyType *pType, SIP_U32bit index, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_getMsgBodyCount
** DESCRIPTION: This function retrieves the number of message bodies in a 
**				SipMessage.
** 
** PARAMETERS:
**				pMsg(IN)		- Pointer to SipMessage 
**				pCount(OUT)		- The count of the number of message bodies 
**								  in pMsg 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_getMsgBodyCount(pMsg,pCount,pErr) \
		SIP_GET_LIST_COUNT((SipMessage*)pMsg,slMessageBody,pCount,pErr) 
#ifdef SIP_MIME_PARSING
/****************************************************************************
** FUNCTION: sip_bcpt_getContentIdFromMimeHdr
** DESCRIPTION: This function gets the value of the ContentId header from 
**				a SipMimeHeader structure.  
** 
** PARAMETERS:
**				pHdr(IN)		- Pointer to SipMimeHeader structure 
**				ppId(OUT)		- The retrieved Content-Id value
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_bcpt_getContentIdFromMimeHdr(pHdr,ppId,pErr)\
	SIP_GET_PARAM_BY_REF((SipMimeHeader *)pHdr,pContentId,ppId,pErr,SipParamString)

/****************************************************************************
** FUNCTION:  sip_bcpt_setContentIdInMimeHdr
** DESCRIPTION: This function sets the Content-Id header in a SipMimeHeader. 
** 
** PARAMETERS:
**				pHdr(IN)		- Pointer to SipMimeHeader 
**				pId(IN)			- The value to be set for Content-Id header 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_bcpt_setContentIdInMimeHdr(pHdr,pId,pErr) \
	SIP_SET_PARAM_BY_REF((SipMimeHeader *)pHdr,pContentId,pId,pErr,SipParamString)

/****************************************************************************
** FUNCTION: sip_bcpt_getContentDescFromMimeHdr
** DESCRIPTION: This function retrieves the value of the Content-Description
**				header from a SipMimeHeader structure.
** 
** PARAMETERS:
**				pHdr(IN)			- Pointer to SipMimeHeader
**				ppDescription(OUT)	- The retrieved Content-Description header
**				pErr(OUT)			- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_bcpt_getContentDescFromMimeHdr(pHdr,ppDescription,pErr)\
	SIP_GET_PARAM_BY_REF((SipMimeHeader *)pHdr,pContentDescription,ppDescription,pErr,SipParamString);

/****************************************************************************
** FUNCTION: sip_bcpt_setContentDescInMimeHdr
** DESCRIPTION: This function sets the Content Description header in
**				a SipMimeHeader structure.
** 
** PARAMETERS:
**				pHdr(IN)		- Pointer to SipMimeHeader 
**				pId(IN)			- The value to be set for Content-Description 
**								  header 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_bcpt_setContentDescInMimeHdr(pHdr,pDescription,pErr)\
	SIP_SET_PARAM_BY_REF((SipMimeHeader *)pHdr,pContentDescription,pDescription,pErr,SipParamString);

/****************************************************************************
** FUNCTION: sip_bcpt_getAdditionalMimeHdrCountFromMimeHdr
** DESCRIPTION: This function gets the number of Additional Mime Headers nodes 
** present in a SipMimeHeader structure.
** 
** PARAMETERS:
**				pHdr(IN)		- Pointer to SipMimeHeader 
**				pCount(OUT)		- The count of the number of additional Mime
**								  headers
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_bcpt_getAdditionalMimeHdrCountFromMimeHdr(pHdr,pCount,pErr)\
	SIP_GET_LIST_COUNT((SipMimeHeader *)pHdr,slAdditionalMimeHeaders,pCount,pErr)

/****************************************************************************
** FUNCTION: sip_bcpt_getAdditionalMimeHdrAtIndexFromMimeHdr
** DESCRIPTION: This function gets the value of the additional Mime header
**				present at a specified index in a SipMimeHeader structure.
**				(Indices start at 0).
** 
** PARAMETERS:
**				pHdr(IN)			- Pointer to SipMimeHeader
**				ppAddMimeHdr(OUT)	- The retrieved Mime header
**				index(IN)			- The index from which to retrieve
**				pErr(OUT)			- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_bcpt_getAdditionalMimeHdrAtIndexFromMimeHdr(pHdr,ppAddMimeHdr,index,pErr)\
	SIP_GET_LIST_AT_INDEX((SipMimeHeader *)pHdr,slAdditionalMimeHeaders,ppAddMimeHdr,index,pErr,SipParamString)

/****************************************************************************
** FUNCTION: sip_bcpt_setAdditionalMimeHdrAtIndexInMimeHdr
** DESCRIPTION: This function sets a string as an additional Mime header
**				at a specified index in a SipMimeHeader.
** 
** PARAMETERS:
**				pHdr(IN)		- Pointer to SipMimeHeader
**				pAddMimeHdr(IN)	- The value to set as the additional Mime header
**				index(IN)		- The index at which to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_bcpt_setAdditionalMimeHdrAtIndexInMimeHdr(pHdr,pAddMimeHdr,index,pErr)\
	SIP_SET_LIST_AT_INDEX((SipMimeHeader *)pHdr,slAdditionalMimeHeaders,pAddMimeHdr,index,pErr,SipParamString)
/****************************************************************************
** FUNCTION: sip_bcpt_insertAdditionalMimeHdrAtIndexInMimeHdr
** DESCRIPTION: This function inserts an additional Mime header at a specified
**				index in a SipMimeHeader structure.
** 
** PARAMETERS:
**				pHdr(IN)		- Pointer to SipMimeHeader
**				pAddMimeHdr(IN)	- The string to set as the additonal Mime header
**				index(IN)		- The index at which to insert
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_bcpt_insertAdditionalMimeHdrAtIndexInMimeHdr(pHdr,pAddMimeHdr,index,pErr)\
	SIP_INSERT_LIST_AT_INDEX((SipMimeHeader *)pHdr,slAdditionalMimeHeaders,pAddMimeHdr,index,pErr,SipParamString)

/****************************************************************************
** FUNCTION: sip_bcpt_deleteAdditionalMimeHdrAtIndexInMimeHdr
** DESCRIPTION: This function deletes the additional Mime header present at 
**				a specified index in a SipMimeHeader.
** 
** PARAMETERS:
**				pHdr(IN)		- Pointer to SipMimeHeader
**				index(IN)		- The index from which to delete
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_bcpt_deleteAdditionalMimeHdrAtIndexInMimeHdr(pHdr,index,pErr)\
	SIP_DELETE_LIST_AT_INDEX((SipMimeHeader *)pHdr,slAdditionalMimeHeaders,index,pErr)

/****************************************************************************
** FUNCTION: sip_bcpt_getMsgBodyCountFromMime
** DESCRIPTION: This function retrieves the count of the number of message
**				bodies present in a Mime message.
** 
** PARAMETERS:
**				pMime(IN)		- Pointer to the Mime message
**				pCount(OUT)		- The retrieved count
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_bcpt_getMsgBodyCountFromMime(pMime,pCount,pErr)\
	SIP_GET_LIST_COUNT((MimeMessage *)pMime,slRecmimeBody,pCount,pErr)

/****************************************************************************
** FUNCTION: sip_bcpt_getMsgBodyAtIndexFromMime
** DESCRIPTION: This function gets the messsage body structure at a specified  
**				index (starting from 0) from a Mime Message.
**
** PARAMETERS:
**				pMime(IN)		- Pointer to the Mime message
**				ppMsgB(OUT)		- The retrieved message body
**				index(IN)		- The index from which to retrieve
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_getMsgBodyAtIndexFromMime
	( MimeMessage 	*pMime,
	  SipMsgBody **ppMsgB,
	  SIP_U32bit 	index, 
	  SipError 	*pErr );

/****************************************************************************
** FUNCTION: sip_bcpt_setMsgBodyAtIndexInMime
** DESCRIPTION: This function sets a message body at a specified
**				index  in a Mime message.
** 
** PARAMETERS:
**				pMime(IN)		- Pointer to Mime message
**				pMsgB(IN)		- The msg body to set
**				index(IN)		- The index at which to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#define sip_bcpt_setMsgBodyAtIndexInMime(pMime,pMsgB,index,pErr)\
	SIP_SET_LIST_AT_INDEX((MimeMessage *)pMime,slRecmimeBody,pMsgB,index,pErr,SipMsgBodyWhole)

/****************************************************************************
** FUNCTION: sip_bcpt_insertMsgBodyAtIndexInMime
** DESCRIPTION: This function inserts the Messsage Body structure at a specified 
**				index in the Mime Message.
** 
** PARAMETERS:
**				pMime(IN)		- Pointer to Mime message
**				pMsgB(IN)		- The message body to insert
**				index(IN)		- The index at which to insert
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_bcpt_insertMsgBodyAtIndexInMime(pMime,pMsgB,index,pErr)\
	SIP_INSERT_LIST_AT_INDEX((MimeMessage *)pMime,slRecmimeBody,pMsgB,index,pErr,SipMsgBodyWhole)

/****************************************************************************
** FUNCTION: sip_bcpt_deleteMsgBodyAtIndexInMime 
** DESCRIPTION: This function deletes a Messsage Body structure at a specified 
**				index in a Mime Message.
** 
** PARAMETERS:
**				pMime(IN)		- Pointer to Mime message
**				index(IN)		- The index from which to delete
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_bcpt_deleteMsgBodyAtIndexInMime(pMime,index,pErr)\
	SIP_DELETE_LIST_AT_INDEX((MimeMessage *)pMime,slRecmimeBody,index,pErr)

/****************************************************************************
** FUNCTION: sip_bcpt_getMsgBodyTypeAtIndexFromMime
** DESCRIPTION: This function gets the Messsage Body structure dType
**				(mime/sdp/unknown/isup/appsip) from a specified index
**				in a Mime message.
**
** PARAMETERS:
**				pMime(IN)		- Pointer to Mime message
**				pType(OUT)		- The retrieved dType
**				index(IN)		- The index from which to retrieve
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_getMsgBodyTypeAtIndexFromMime
	( MimeMessage 		*pMime,
	  en_SipMsgBodyType *pType,
	  SIP_U32bit 		index, 
	  SipError 			*pErr );

#endif
/****************************************************************************
** FUNCTION: sip_bcpt_getIsupFromMsgBody
** DESCRIPTION: This function retrieves the ISUP message body from a SipMsgBody
** 
** PARAMETERS:
**				pMsgBody(IN)	- Pointer to SipMsgBody
**				ppIsup(OUT)		- The retrieved ISUP body 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#ifdef SIP_ISUP
SipBool sip_bcpt_getIsupFromMsgBody
	( SipMsgBody *pMsgBody, IsupMessage **ppIsup, SipError *pErr);
/****************************************************************************
** FUNCTION: sip_bcpt_setIsupInMsgBody
** DESCRIPTION: This function sets an ISUP message body in a SipMsgBody 
**				structure. 
** 
** PARAMETERS:
**				pMsgBody(IN)	- Pointer to SipMsgBody
**				pIsup(IN)		- The retrived message body
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_setIsupInMsgBody
	(SipMsgBody *pMsgBody, IsupMessage *pIsup, SipError *pErr);

#endif
/****************************************************************************
** FUNCTION: sip_bcpt_getMimeFromMsgBody
** DESCRIPTION: This function retrieves the Mime message body from a SipMsgBody
**				structure.
** 
** PARAMETERS:
**				pMsgBody(IN)	- Pointer to SipMsgBody
**				ppMime(OUT)		- The retrieved Mime message
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#ifdef SIP_MIME_PARSING
SipBool sip_bcpt_getMimeFromMsgBody
	( SipMsgBody *pMsgBody, MimeMessage **ppMime, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_setMimeInMsgBody 
** DESCRIPTION: This function sets the Mime message in the SipMsgBody 
**				structure.
** 
** PARAMETERS:
**				pMsgBody(IN)	- Pointer to SipMsgBody
**				pMime(IN)		- The Mime message body to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_setMimeInMsgBody
	( SipMsgBody *pMsgBody, MimeMessage *pMime, SipError *pErr);
#endif
/****************************************************************************
** FUNCTION: sip_bcpt_getLengthFromIsupMessage
** DESCRIPTION: This function retrieves the dLength of an Isup
**		message.
** 
** PARAMETERS:
**				pMsg(IN)		- Pointer to ISUP message
**				pLength(OUT)	- The retrieved length
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#ifdef SIP_ISUP
SipBool sip_bcpt_getLengthFromIsupMessage
	( IsupMessage *pMsg, SIP_U32bit *pLength, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_getBodyFromIsupMessage
** DESCRIPTION: This function gets the value of the IsupMessage. 
** 
** PARAMETERS:
**				pMsg(IN)		- Pointer to Isup message
**				ppBody(OUT)		- The retrieved body
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_getBodyFromIsupMessage
	(IsupMessage *pMsg, SIP_S8bit **ppBody, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_setBodyInIsupMessage
** DESCRIPTION: This function sets the Body field pValue in IsupMessage.
** 
** PARAMETERS:
**				pMsg(IN)		- Pointer to IsupMessage 
**				pBody(IN)		- The string to be set as body 
**				dLength(In)		- The length of the body 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_setBodyInIsupMessage
	(IsupMessage *pMsg, SIP_S8bit *pBody, SIP_U32bit dLength,SipError *pErr);
#endif
#ifdef SIP_MIME_PARSING
/****************************************************************************
** FUNCTION: sip_bcpt_getMimeHeaderFromMsgBody
** DESCRIPTION: This function retrieves the mime pHeader from the SipMsgBody 
**				structure. 
** 
** PARAMETERS:
**				pMsgBody(IN)	- Pointer to SipMsgBody
**				ppMime(OUT)		- The retrieved SipMimeHeader
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_getMimeHeaderFromMsgBody
	(SipMsgBody *pMsgBody, SipMimeHeader **ppMime, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_setMimeHeaderInMsgBody
** DESCRIPTION: This function sets the mime pHeader in SipMsgBody structure. 
** 
** PARAMETERS:
**				pMsgBody(IN)	- Pointer to SipMsgBody
**				pMime(IN)		- The SipMimeHeader structure to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_setMimeHeaderInMsgBody
	(SipMsgBody *pMsgBody, SipMimeHeader *pMime, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_getVersionFromMimeVersionHdr
** DESCRIPTION: This function gets the Version field from Mime-Version Header.
** 
** PARAMETERS:
**				pHdr(IN)		- MimeVersion header
**				ppVersion(OUT)	- The retried version
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_getVersionFromMimeVersionHdr 
	(SipHeader *pHdr, SIP_S8bit **ppVersion, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_setVersionInMimeVersionHdr
** DESCRIPTION: This function sets the Version field in Mime-Version Header.
** 
** PARAMETERS:
**				pHdr(IN)		- The Mime-Version header 
**				pVersion(IN)	- The version string to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_setVersionInMimeVersionHdr 
	(SipHeader *pHdr, SIP_S8bit *pVersion, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_getContentDispositionFromMimeHdr
** DESCRIPTION: This function gets the Content Disposition field from Mime 
**				header.
** 
** PARAMETERS:
**				pMimeHdr(IN)	- Pointer to SipMimeHeader
**				ppContentDisposition(OUT)		- The retrieved value of the
**								  Content-Disposition header
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_getContentDispositionFromMimeHdr
        (SipMimeHeader *pMimeHdr, SipHeader **ppContentDisposition, 
		SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_getContentTypeFromMimeHdr
** DESCRIPTION: This function gets the Content Type field from Mime Header.
** 
** PARAMETERS:
**				pMimeHdr(IN)		- Pointer to SipMimeHeader
**				ppContentType(OUT)	- The retrieved Content-Type header field
**									  value.
**				pErr(OUT)			- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_getContentTypeFromMimeHdr
        (SipMimeHeader *pMimeHdr, SipHeader **ppContentType, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_setContentTypeInMimeHdr
** DESCRIPTION: This function sets the Content Type field in Mime Header.
** 
** PARAMETERS:
**				pMimeHdr(IN)	- Pinter to SipMimeHeader
**				pHdr(IN)		- Pointer to Content-Type header to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_setContentTypeInMimeHdr
        (SipMimeHeader *pMimeHdr, SipHeader *pHdr, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_setContentDispositionInMimeHdr
** DESCRIPTION: This function sets the Content Disposition field in Mime 
**				Header.
** 
** PARAMETERS:
**				pMimeHdr(IN)	- Pointer to Mime header
**				pHdr(IN)		- Pointer to Content-Disposition header to set 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_setContentDispositionInMimeHdr
        (SipMimeHeader *pMimeHdr, SipHeader *pHdr, SipError *pErr);

/****************************************************************************
** FUNCTION: sip_bcpt_getContentTransEncodingFromMimeHdr
** DESCRIPTION: This function gets the Content-Transfer-Encoding field from Mime
**				header.
** 
** PARAMETERS:
**				pMimeHdr(IN)	- Pointer to SipMimeHeader
**				ppEncoding(OUT)	- The retrieved Content-Trans-Encoding header
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_bcpt_getContentTransEncodingFromMimeHdr(pMimeHdr,ppEncoding,pErr) \
	SIP_GET_PARAM_BY_REF((SipMimeHeader *)pMimeHdr,pContentTransEncoding,ppEncoding,pErr,SipParamString)

/****************************************************************************
** FUNCTION: sip_bcpt_setContentTransEncodingInMimeHdr
** DESCRIPTION: This function sets the Content-Transfer-Encoding field in Mime 
**				Header.
** 
** PARAMETERS:
**				pMimeHdr(IN)	- Pointer to SipMimeHeader
**				pEncoding(IN)	- The Content-Transfer-Encoding header to set 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

#define sip_bcpt_setContentTransEncodingInMimeHdr(pMimeHdr,pEncoding,pErr) \
	SIP_SET_PARAM_BY_REF((SipMimeHeader *)pMimeHdr,pContentTransEncoding,pEncoding,pErr,SipParamString)

#endif
/********************************************************************
**
** FUNCTION:  sip_validateSipMsgBodyType
**
**DESCRIPTION:  This fucntion returns SipSuccess if "dType"
** is one among the defined en_MsgBodyType's else it returns SipFail.
**
********************************************************************/
SipBool sip_validateSipMsgBodyType
	(en_SipMsgBodyType dType, SipError *pErr);

/***********************************************************************
** FUNCTION: sdp_getVersion
** DESCRIPTION: get Version field from Sdp Message
** PARAMETERS:
**		msg (IN) 		- Sdp Message
**		version(OUT)	- version retrieved
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getVersion(pMsg,ppOutVersion,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMessage *)pMsg,pVersion,ppOutVersion,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setVersion
** DESCRIPTION: set Version field in Sdp Message
** PARAMETERS:
**		msg (IN/OUT) 	- Sdp Message
**		version(IN) 	- Version to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setVersion(pMsg,pInVersion,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMessage *)pMsg,pVersion,pInVersion,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getOrigin
** DESCRIPTION: get Origin field from Sdp Message
** PARAMETERS:
**		msg (IN) 	- Sdp Message
**		origin(OUT) - origin retrieved
**		err	(OUT)	- Possible error value (see API ref doc)
**
************************************************************************/
#define sdp_getOrigin(pMsg,ppOutOrigin,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMessage *)pMsg,pOrigin,ppOutOrigin,pErr,SipSdpParamOrigin)

/***********************************************************************
** FUNCTION: sdp_setOrigin
** DESCRIPTION: set Origin field in Sdp Message
** PARAMETERS:
**		msg (IN/OUT) 	- Sdp Message
**		origin(IN)		- Origin to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setOrigin(pMsg,pInOrigin,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMessage *)pMsg,pOrigin,pInOrigin,pErr,SipSdpParamOrigin)

/***********************************************************************
** FUNCTION: sdp_getConnection
** DESCRIPTION: get Connection field from Sdp Message
** PARAMETERS:
**		msg (IN) 		- Sdp Message
**		connection(OUT)	- Retrieved connection field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getConnection(pMsg,ppOutConnection,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMessage *)pMsg,slConnection,ppOutConnection,pErr,SipSdpParamConnection)

/***********************************************************************
** FUNCTION: sdp_setConnection
** DESCRIPTION: set connection field in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sp Message
**		connection(IN)	- connection to set
**		err	(OUT)	- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setConnection(pMsg,pInConnection,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMessage *)pMsg,slConnection,pInConnection,pErr,SipSdpParamConnection)

/***********************************************************************
** FUNCTION: sdp_getUserFromOrigin
** DESCRIPTION: get user field from Origin line
** PARAMETERS:
**		msg (IN)		- Sdp Origin line
**		user (OUT)		- retrieved user field
**		err	(OUT)	- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getUserFromOrigin(pOrigin,ppOutUser,pErr) \
	SIP_GET_PARAM_BY_REF((SdpOrigin *)pOrigin,pUser,ppOutUser,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setUserInOrigin
** DESCRIPTION: set user field in origin line
** PARAMETERS:
**		msg (IN/OUT) -	Sdp Origin line
**		user (IN) 	 - 	User value to set
**		err	(OUT)	- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setUserInOrigin(pOrigin,pInUser,pErr) \
	SIP_SET_PARAM_BY_REF((SdpOrigin *)pOrigin,pUser,pInUser,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getSessionIdFromOrigin
** DESCRIPTION: get session identifier from origin field
** PARAMETERS:
**		origin (IN)		- Sdp Origin line
**		id	(OUT)		- Retrieved Session Id field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getSessionIdFromOrigin(pOrigin,ppOutSessID,pErr) \
	SIP_GET_PARAM_BY_REF((SdpOrigin *)pOrigin,pSessionid,ppOutSessID,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setSessionIdInOrigin
** DESCRIPTION: set session identifier in origin field
** PARAMETERS:
**		origin (IN/OUT)		- Sdp Origin line
**		id (IN)				- Session id to set
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setSessionIdInOrigin(pOrigin,pInSessID,pErr) \
	SIP_SET_PARAM_BY_REF((SdpOrigin *)pOrigin,pSessionid,pInSessID,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getVersionFromOrigin
** DESCRIPTION: get version field from Origin line
** PARAMETERS:
**		origin (IN)		- Sdp Origin line
**		version (OUT)	- retrieved version field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getVersionFromOrigin(pOrigin,ppOutVersion,pErr) \
	SIP_GET_PARAM_BY_REF((SdpOrigin *)pOrigin,pVersion,ppOutVersion,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setVersionInOrigin
** DESCRIPTION: set version field in Origin line
** PARAMETERS:
**		origin (IN/OUT)	- Sdp Origin line
**		version (IN)	- Version to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setVersionInOrigin(pOrigin,pInVersion,pErr) \
	SIP_SET_PARAM_BY_REF((SdpOrigin *)pOrigin,pVersion,pInVersion,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getNetTypeFromOrigin
** DESCRIPTION: get Network Type field from Origin line
** PARAMETERS:
**		origin (IN) 		- Sdp Origin line
**		ntype	(OUT)		- retrieved network type
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getNetTypeFromOrigin(pOrigin,ppOutNetType,pErr) \
	SIP_GET_PARAM_BY_REF((SdpOrigin *)pOrigin,pNetType,ppOutNetType,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setNetTypeInOrigin
** DESCRIPTION: set Network Type field in Origin line
** PARAMETERS:
**		origin (IN/OUT)		- Sdp Origin line
**		ntype (IN)			- Network Type to set
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setNetTypeInOrigin(pOrigin,pInNetType,pErr) \
	SIP_SET_PARAM_BY_REF((SdpOrigin *)pOrigin,pNetType,pInNetType,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getAddrFromOrigin
** DESCRIPTION:get Address field from Origin Line
** PARAMETERS:
**		origin 	(IN) 		- Sdp Origin line
**		addr	(OUT)		- retrieved address field
**		err		(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getAddrFromOrigin(pOrigin,ppOutAddr,pErr) \
	SIP_GET_PARAM_BY_REF((SdpOrigin *)pOrigin,pAddr,ppOutAddr,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setAddrInOrigin
** DESCRIPTION: Set address field in Origin Line
** PARAMETERS:
**		origin (IN/OUT)		- Sdp Origin line
**		addr	(IN)		- Address field to set
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setAddrInOrigin(pOrigin,pInAddr,pErr) \
	SIP_SET_PARAM_BY_REF((SdpOrigin *)pOrigin,pAddr,pInAddr,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getAddrTypeFromOrigin
** DESCRIPTION: get address type field from origin line
** PARAMETERS:
**		origin(IN)		- Sdp Origin line
**		addr_type(OUT)	- retrieved address type field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getAddrTypeFromOrigin(pOrigin,ppOutAddrType,pErr) \
	SIP_GET_PARAM_BY_REF((SdpOrigin *)pOrigin,pAddrType,ppOutAddrType,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setAddrTypeInOrigin
** DESCRIPTION: set address type field in origin line
** PARAMETERS:
**		origin (IN/OUT)	- Sdp Origin line
**		addr_type(IN)	- Address Type field to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setAddrTypeInOrigin(pOrigin,pInAddrType,pErr) \
	SIP_SET_PARAM_BY_REF((SdpOrigin *)pOrigin,pAddrType,pInAddrType,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getSession
** DESCRIPTION: get Session line from Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		session(OUT)	- retrieved session line
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getSession(pMsg,ppOutSession,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMessage *)pMsg,pSession,ppOutSession,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setSession
** DESCRIPTION: set Session line in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		session (IN)	- session value to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setSession(pMsg,pInSession,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMessage *)pMsg,pSession,pInSession,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getUri
** DESCRIPTION: get uri line from Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		uri (OUT)		- retrieved uri field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getUri(pMsg,ppOutUri,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMessage *)pMsg,pUri,ppOutUri,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setUri
** DESCRIPTION: set uri line in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		uri (IN)		- Uri to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setUri(pMsg,pInUri,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMessage *)pMsg,pUri,pInUri,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getInfo
** DESCRIPTION: get information line from Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		info (OUT)		- retrieved uri line
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getInfo(pMsg,ppOutInfo,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMessage *)pMsg,pInformation,ppOutInfo,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setInfo
** DESCRIPTION: set information line in Sdp Message (i=)
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		info (IN)		- Information field to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setInfo(pMsg,pInInfo,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMessage *)pMsg,pInformation,pInInfo,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getKey
** DESCRIPTION: get key line from sdp Message (k=)
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		key (OUT)		- Retrieved key value
**		err	(OUT)		- Possible error value (see API ref doc)
**
************************************************************************/
#define sdp_getKey(pMsg,ppOutKey,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMessage *)pMsg,pKey,ppOutKey,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setKey
** DESCRIPTION: set key line in Sdp Message (k=)
** PARAMETERS:
**		msg (IN/OUT)		- Sdp Message
**		key (IN)			- key value to set
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setKey(pMsg,pInKey,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMessage *)pMsg,pKey,pInKey,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getBandwidthCount
** DESCRIPTION: get number of bandwidth lines in Sdp Message (b=)
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		cnt (OUT)		- number of bandwidth lines
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getBandwidthCount(pMsg,pCount,pErr) \
		SIP_GET_LIST_COUNT((SdpMessage *)pMsg,pBandwidth,pCount,pErr)

/***********************************************************************
** FUNCTION: sdp_getBandwidthAtIndex
** DESCRIPTION: get bandwidth line at a specified index from Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		bandwidth(OUT)	- retrieved bandwidh field
**		cnt (IN)		- Index at which to retrieve
************************************************************************/
#define sdp_getBandwidthAtIndex(pMsg,ppBandwidth,index,pErr) \
	    SIP_GET_LIST_AT_INDEX((SdpMessage *)pMsg,pBandwidth,(void **)ppBandwidth,\
				index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setBandwidthAtIndex
** DESCRIPTION:	set bandwidth line at a specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)		- Sdp Message
**		bandwidth(IN)		- bandwidth value to set
** 		cnt (IN)			- Index at which to set
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setBandwidthAtIndex(pMsg,pbdwidth,index,pErr) \
        SIP_SET_LIST_AT_INDEX((SdpMessage *)pMsg,pBandwidth,(SIP_Pvoid)pbdwidth,\
				index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_insertBandwidthAtIndex
** DESCRIPTION:	insert bandwidth line at a specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)		- Sdp Message
**		bandwidth(IN)		- bandwidth value to insert
** 		cnt (IN)			- Index at which to insert
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_insertBandwidthAtIndex(pMsg,pbdwidth,index,pErr) \
        SIP_INSERT_LIST_AT_INDEX((SdpMessage *)pMsg,pBandwidth,(SIP_Pvoid)pbdwidth,\
				index,pErr,SipParamString)
/***********************************************************************
** FUNCTION: sdp_deleteBandwidthAtIndex
** DESCRIPTION:	delete bandwidth line at a specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)		- Sdp Message
** 		cnt (IN)			- Index at which to delete
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_deleteBandwidthAtIndex(pMsg,index,pErr) \
        SIP_DELETE_LIST_AT_INDEX((SdpMessage *)pMsg,pBandwidth,index,pErr)

/***********************************************************************
** FUNCTION: sdp_getPhoneCount
** DESCRIPTION: get number of phone lines in Sdp Message
** PARAMETERS:
**		msg (IN)			- Sdp Message
**		cnt (OUT)			- number of phone lines
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getPhoneCount(pMsg,pCount,pErr) \
		SIP_GET_LIST_COUNT((SdpMessage *)pMsg,slPhone,pCount,pErr)

/***********************************************************************
** FUNCTION: sdp_getPhoneAtIndex
** DESCRIPTION: get phone line at specified index from Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		phone (OUT)		- retrieved phone line
**		cnt (IN)		- specified index to retrieve from
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getPhoneAtIndex(pMsg,ppPhone,index,pErr) \
    SIP_GET_LIST_AT_INDEX((SdpMessage *)pMsg,slPhone,(void **)ppPhone,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setPhoneAtIndex
** DESCRIPTION: set phone line at specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		phone (IN)		- phone line to set
**		cnt (IN)		- specified index to set into
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setPhoneAtIndex(pMsg,pPhone,index,pErr) \
    SIP_SET_LIST_AT_INDEX((SdpMessage *)pMsg,slPhone,(SIP_Pvoid)pPhone,index,pErr,SipParamString)
/***********************************************************************
** FUNCTION: sdp_insertPhoneAtIndex
** DESCRIPTION: insert phone line at specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		phone (IN)		- phone line to insert
**		cnt (IN)		- specified index to insert into
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_insertPhoneAtIndex(pMsg,pPhone,index,pErr) \
    SIP_INSERT_LIST_AT_INDEX((SdpMessage *)pMsg,slPhone,(SIP_Pvoid)pPhone,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_deletePhoneAtIndex
** DESCRIPTION: delete phone line at specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		cnt (IN)		- Index to delete phone line from
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_deletePhoneAtIndex(pMsg,index,pErr) \
    SIP_DELETE_LIST_AT_INDEX((SdpMessage *)pMsg,slPhone,index,pErr)

/***********************************************************************
** FUNCTION: sdp_getEmailCount
** DESCRIPTION: get number of email lines in Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		cnt (OUT)		- number of email lines
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getEmailCount(pMsg,pCount,pErr) \
    SIP_GET_LIST_COUNT((SdpMessage *)pMsg,slEmail,pCount,pErr) 

/***********************************************************************
** FUNCTION: sdp_getEmailAtIndex
** DESCRIPTION: get email line at specified index from Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		email (OUT)		- retrieved email line
**		cnt (IN)		- Index at which to retrieve
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getEmailAtIndex(pMsg,ppEmail,index,pErr) \
    SIP_GET_LIST_AT_INDEX((SdpMessage *)pMsg,slEmail,(void **)ppEmail,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setEmailAtIndex
** DESCRIPTION: set email line at specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		email (IN)		- email line to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setEmailAtIndex(pMsg,pEmail,index,pErr) \
    SIP_SET_LIST_AT_INDEX((SdpMessage *)pMsg,slEmail,(SIP_Pvoid)pEmail,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_insertEmailAtIndex
** DESCRIPTION: insert email line at specified index in Sdp Message
* PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		email (IN)		- email line to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_insertEmailAtIndex(pMsg,pEmail,index,pErr) \
    SIP_INSERT_LIST_AT_INDEX((SdpMessage *)pMsg,slEmail,(SIP_Pvoid)pEmail,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_deleteEmailAtIndex
** DESCRIPTION: delete email line at specified index from Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		cnt (IN)		- specified index to delete from
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_deleteEmailAtIndex(pMsg,index,pErr) \
    SIP_DELETE_LIST_AT_INDEX((SdpMessage *)pMsg,slEmail,index,pErr)

/***********************************************************************
** FUNCTION: sdp_getMediaCount
** DESCRIPTION: get number of media lines from Sdp Message
** PARAMETERS:
**		msg(IN)			- Sdp Message
**		cnt (OUT)		- retrieved number of media lines
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getMediaCount(pMsg,pCount,pErr) \
    SIP_GET_LIST_COUNT((SdpMessage *)pMsg,slMedia,pCount,pErr)

/***********************************************************************
** FUNCTION: sdp_getMediaAtIndex
** DESCRIPTION: get media line from Sdp Message at specified index
** PARAMETERS:
**		msg (IN)	- Sdp Message
**		media (OUT)	- retrieved media line
**		cnt (IN)	- Index to retrieve Media line from
**		err	(OUT)	- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getMediaAtIndex(pMsg,ppMedia,index,pErr)\
    SIP_GET_LIST_AT_INDEX((SdpMessage *)pMsg,slMedia,(void **)ppMedia,index,pErr,SipSdpParamMedia)

/***********************************************************************
** FUNCTION: sdp_setMediaAtIndex
** DESCRIPTION: set media line at specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		media (IN)		- Media line to set
**		cnt (IN)		- Index to set at
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setMediaAtIndex(pMsg,pMedia,index,pErr)\
    SIP_SET_LIST_AT_INDEX((SdpMessage *)pMsg,slMedia,(SIP_Pvoid)pMedia,index,pErr,SipSdpParamMedia)

/***********************************************************************
** FUNCTION: sdp_insertMediaAtIndex
** DESCRIPTION: insert media at specified  index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		media (IN)		- Media line to insert
**		cnt (IN)		- Index to insert at
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_insertMediaAtIndex(pMsg,pMedia,index,pErr)\
    SIP_INSERT_LIST_AT_INDEX((SdpMessage *)pMsg,slMedia,(SIP_Pvoid)pMedia,index,pErr,SipSdpParamMedia)
/***********************************************************************
** FUNCTION: sdp_deleteMediaAtIndex
** DESCRIPTION: delete media at specified index from Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		cnt (IN)		- Index to delete at
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_deleteMediaAtIndex(pMsg,index,pErr)\
    SIP_DELETE_LIST_AT_INDEX((SdpMessage *)pMsg,slMedia,index,pErr)

/***********************************************************************
** FUNCTION: sdp_getTimeCount
** DESCRIPTION: get number of time lines in Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		cnt (OUT)		- retrieved number of times lines
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getTimeCount(pMsg,pCount,pErr) \
    SIP_GET_LIST_COUNT((SdpMessage *)pMsg,slTime,pCount,pErr)

/***********************************************************************
** FUNCTION: sdp_getTimeAtIndex
** DESCRIPTION: get time line at specified index from Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		time (OUT)		- retrieved time line
**		cnt (IN)		- Index to retrieve from
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getTimeAtIndex(pMsg,ppTime,index,pErr)\
    SIP_GET_LIST_AT_INDEX((SdpMessage *)pMsg,slTime,(void **)ppTime,index,pErr,SipSdpParamTime)

/***********************************************************************
** FUNCTION: sdp_setTimeAtIndex
** DESCRIPTION: set time line at specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		time (IN)		- time line to set
**		cnt (IN)		- Index to set at
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setTimeAtIndex(pMsg,pTime,index,pErr)\
    SIP_SET_LIST_AT_INDEX((SdpMessage *)pMsg,slTime,(SIP_Pvoid)pTime,index,pErr,SipSdpParamTime)

/***********************************************************************
** FUNCTION: sdp_insertTimeAtIndex
** DESCRIPTION: insert time line at specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		time (IN)		- time line to insert
**		cnt (IN)		- Index to insert at
**		err	(OUT)		- Possible error value (see API ref doc)
**
************************************************************************/
#define sdp_insertTimeAtIndex(pMsg,pTime,index,pErr)\
    SIP_INSERT_LIST_AT_INDEX((SdpMessage *)pMsg,slTime,(SIP_Pvoid)pTime,index,pErr,SipSdpParamTime)

/***********************************************************************
** FUNCTION: sdp_deleteTimeAtIndex
** DESCRIPTION: delete time line at specified index from Sdp Message
** PARAMETERS:
**		msg (IN/OUT)	- Sdp Message
**		cnt (IN)		- Index to delete at
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_deleteTimeAtIndex(pMsg,index,pErr) \
    SIP_DELETE_LIST_AT_INDEX((SdpMessage *)pMsg,slTime,index,pErr)

/***********************************************************************
** FUNCTION: sdp_getAttrCount
** DESCRIPTION: get number of attribute lines from Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		cnt (OUT)		- retrieved number of attribute lines
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getAttrCount(pMsg,pCount,pErr) \
    SIP_GET_LIST_COUNT((SdpMessage *)pMsg,slAttr,pCount,pErr)

/***********************************************************************
** FUNCTION: sdp_getAttrAtIndex
** DESCRIPTION: get attribute line at specified index from Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		attr(OUT)		- retrieved attribute line
**		cnt (IN)		- Index to retrieve from
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getAttrAtIndex(pMsg,ppAttr,index,pErr)\
    SIP_GET_LIST_AT_INDEX((SdpMessage *)pMsg,slAttr,(void **)ppAttr,index,pErr,SipSdpParamAttribute)

/***********************************************************************
** FUNCTION: sdp_setAttrAtIndex
** DESCRIPTION: set attribute line at specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT) 		- Sdp Message
**		attr (IN)			- attribute line to set
**		cnt (IN)			- index to set into
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setAttrAtIndex(pMsg,pAttr,index,pErr)\
    SIP_SET_LIST_AT_INDEX((SdpMessage *)pMsg,slAttr,(SIP_Pvoid)pAttr,index,pErr,SipSdpParamAttribute)

/***********************************************************************
** FUNCTION: sdp_insertAttrAtIndex
** DESCRIPTION: insert attribute line at specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT) 		- Sdp Message
**		attr (IN)			- attribute line to insert
**		cnt (IN)			- index to insert into
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_insertAttrAtIndex(pMsg,pAttr,index,pErr)\
    SIP_INSERT_LIST_AT_INDEX((SdpMessage *)pMsg,slAttr,(SIP_Pvoid)pAttr,index,pErr,SipSdpParamAttribute)

/***********************************************************************
** FUNCTION: sdp_deleteAttrAtIndex
** DESCRIPTION: delete attribute line at specified index in Sdp Message
** PARAMETERS:
**		msg (IN/OUT) 		- Sdp Message
**		attr (IN)			- attribute line to delete
**		cnt (IN)			- index to delete from
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_deleteAttrAtIndex(pMsg,index,pErr)\
    SIP_DELETE_LIST_AT_INDEX((SdpMessage *)pMsg,slAttr,index,pErr)

/***********************************************************************
** FUNCTION: sdp_getNetTypeFromConnection
** DESCRIPTION: get network type field from connection line
** PARAMETERS:
**	connection (IN)		- Sdp Connection line
**		ntype (OUT)		- retrieved network type field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getNetTypeFromConnection(pConnection,ppOutNetType,pErr) \
	SIP_GET_PARAM_BY_REF((SdpConnection *)pConnection,pNetType,ppOutNetType,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setNetTypeInConnection
** DESCRIPTION: set network type field in connection line
** PARAMETERS:
**		connection (IN/OUT)	- Sdp Connection line
**		ntype (IN)			- network type field to set
**		err	(OUT)			- Possible error value (see API ref doc)

************************************************************************/
#define sdp_setNetTypeInConnection(pConnection,pInNetType,pErr) \
	SIP_SET_PARAM_BY_REF((SdpConnection *)pConnection,pNetType,pInNetType,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getAddrTypeFromConnection
** DESCRIPTION: get address type field from connection line
** PARAMETERS:
**		connection (IN)		- Sdp connection line
**		atype (OUT)			- retrieved address type field
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getAddrTypeFromConnection(pConnection,ppOutAddrType,pErr) \
	SIP_GET_PARAM_BY_REF((SdpConnection *)pConnection,pAddrType,ppOutAddrType,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setAddrTypeInConnection
** DESCRIPTION: set address type field in connection line
** PARAMETERS:
**		connection (IN/OUT)	- Sdp Connection line
**		atype (IN)			- Addres type field to set
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setAddrTypeInConnection(pConnection,pInAddrType,pErr) \
	SIP_SET_PARAM_BY_REF((SdpConnection *)pConnection,pAddrType,pInAddrType,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getStartFromTime
** DESCRIPTION: retrieve start time from time line
** PARAMETERS:
**		time (IN)		- Sdp Time
**		start (OUT)		- retrieved start time
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getStartFromTime(pTime,ppOutStart,pErr) \
	SIP_GET_PARAM_BY_REF((SdpTime *)pTime,pStart,ppOutStart,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setStartInTime
** DESCRIPTION: set start time in time  line
** PARAMETERS:
**		time (IN/OUT)	- Sdp Time
**		start (IN)		- Start time to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setStartInTime(pTime,pInStart,pErr) \
	SIP_SET_PARAM_BY_REF((SdpTime *)pTime,pStart,pInStart,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getStopFromTime
** DESCRIPTION: get stop time from time line
** PARAMETERS:
**		time (IN)		- Sdp Time
**		stop(OUT)		- stop time retrieved
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getStopFromTime(pTime,ppOutStop,pErr) \
	SIP_GET_PARAM_BY_REF((SdpTime *)pTime,pStop,ppOutStop,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setStopInTime
** DESCRIPTION: set stop time in time line
** PARAMETERS:
**		time (IN/OUT)	- Sdp Time
**		stop (IN)		- stop time to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setStopInTime(pTime,pInStop,pErr) \
	SIP_SET_PARAM_BY_REF((SdpTime *)pTime,pStop,pInStop,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getRepeatCountFromTime
** DESCRIPTION: get number of repeat lines in time line (r=)
** PARAMETERS:
**		time (IN)		-  Sdp Time line
**		index (OUT)		- number of repeat lines
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getRepeatCountFromTime(pTime,pCount,pErr)\
    SIP_GET_LIST_COUNT((SdpTime *)pTime,slRepeat,pCount,pErr)

/***********************************************************************
** FUNCTION: sdp_getRepeatAtIndexFromTime
** DESCRIPTION: get repeat line at specified index from Sdp Time line
** PARAMETERS:
**		time (IN)		- Sdp Time
**		repeat (OUT)	- retrieved repeat line
**		index (IN)		- index to retrieve from
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getRepeatAtIndexFromTime(pTime,ppRepeat,index,pErr)\
    SIP_GET_LIST_AT_INDEX((SdpTime *)pTime,slRepeat,ppRepeat,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setRepeatAtIndexInTime
** DESCRIPTION: set repeat line at specified index in Sdp Time line
** PARAMETERS:
**		time (IN/OUT)	- Sdp Time
**		repeat (IN)		- repeat line to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setRepeatAtIndexInTime(pTime,pRepeat,index,pErr)\
    SIP_SET_LIST_AT_INDEX((SdpTime *)pTime,slRepeat,pRepeat,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_insertRepeatAtIndexInTime
** DESCRIPTION: insert repeat line at specified index in Sdp Time line
** PARAMETERS:
**		time (IN/OUT)	- Sdp Time
**		repeat (IN)		- repeat line to insert
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_insertRepeatAtIndexInTime(pTime,pRepeat,index,pErr)\
    SIP_SET_LIST_AT_INDEX((SdpTime *)pTime,slRepeat,pRepeat,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_deleteRepeatAtIndexInTime
** DESCRIPTION: delete repeat line at specified index in Sdp Time line
** PARAMETERS:
**		time (IN/OUT)	- Sdp Time
**		index (IN)		- Index to delte repeat line from
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_deleteRepeatAtIndexInTime(pTime,index,pErr)\
    SIP_DELETE_LIST_AT_INDEX((SdpTime *)pTime,slRepeat,index,pErr)

/***********************************************************************
** FUNCTION: sdp_getZoneFromTime
** DESCRIPTION: retrieve Zone field from Sdp Time line
** PARAMETERS:
**	time (IN)		- Sdp Time line
**	zone (OUT)		- retrieved zone line
**	err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getZoneFromTime(pTime,ppOutZone,pErr) \
	SIP_GET_PARAM_BY_REF((SdpTime *)pTime,pZone,ppOutZone,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setZoneInTime
** DESCRIPTION: set Zone field in Sdp Time line
** PARAMETERS:
**		time (IN)		- Sdp Time line
**		zone (IN)		- zone field to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setZoneInTime(pTime,pInZone,pErr) \
	SIP_SET_PARAM_BY_REF((SdpTime *)pTime,pZone,pInZone,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getAddrFromConnection
** DESCRIPTION: get address field from connection line
** PARAMETERS:
**		connection (IN)		- Sdp Connection line
**		addr (OUT)			- retrieved address field
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getAddrFromConnection(pConnection,ppOutAddr,pErr) \
	SIP_GET_PARAM_BY_REF((SdpConnection *)pConnection,pAddr,ppOutAddr,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setAddrInConnection
** DESCRIPTION: set address field in connection line
** PARAMETERS:
**		connection (IN/OUT)	- Sdp Connection line
**		addr (IN)			- address field to set
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setAddrInConnection(pConnection,pInAddr,pErr) \
	SIP_SET_PARAM_BY_REF((SdpConnection *)pConnection,pAddr,pInAddr,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getNameFromAttr
** DESCRIPTION: get name field from attribute line
** PARAMETERS:
**		attr (IN)		- Sdp Attribute line
**		name (OUT)		- retrieved name field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getNameFromAttr(pAttr,ppOutName,pErr) \
	SIP_GET_PARAM_BY_REF((SdpAttr *)pAttr,pName,ppOutName,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setNameInAttr
** DESCRIPTION: set name field in attribute line
** PARAMETERS:
**		attr (IN/OUT)		- Sdp attribute line
**		name (IN)			- name field to set
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setNameInAttr(pAttr,pInName,pErr) \
	SIP_SET_PARAM_BY_REF((SdpAttr *)pAttr,pName,pInName,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getValueFromAttr
** DESCRIPTION: get value field from attribute line
** PARAMETERS:
**		attr (IN)		- Sdp Attribute line
**		value (OUT)		- value field retrieved
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getValueFromAttr(pAttr,ppOutValue,pErr) \
	SIP_GET_PARAM_BY_REF((SdpAttr *)pAttr,pValue,ppOutValue,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setValueInAttr
** DESCRIPTION: set value field in attribute line
** PARAMETERS:
**		attr (IN/OUT)	- Sdp Attribute line
**		value (IN)		- Sdp value field to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setValueInAttr(pAttr,pInValue,pErr) \
	SIP_SET_PARAM_BY_REF((SdpAttr *)pAttr,pValue,pInValue,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getInfoFromMedia
** DESCRIPTION: get information line from Media line
** PARAMETERS:
**		media (IN)	- Sdp Media line
**		info (OUT)	- retrieved information field
**		err	(OUT)	- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getInfoFromMedia(pMedia,ppOutInfo,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMedia *)pMedia,pInformation,ppOutInfo,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setInfoInMedia
** DESCRIPTION: set information line of media line
** PARAMETERS:
**		media (IN/OUT)	- Sdp Media line
**		info (IN)	- retrieved information field
**		err	(OUT)	- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setInfoInMedia(pMedia,pInInfo,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMedia *)pMedia,pInformation,pInInfo,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getBandwidthCountFromMedia
** DESCRIPTION: get number of bandwidth lines in Sdp Medialine
** PARAMETERS:
**  	pMedia (IN)		- Sdp Media
**	pBWidthCount (OUT)	- number of bandwidth lines
**		pErr	(OUT)	- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getBandwidthCountFromMedia(pMedia,pCount,pErr)\
    SIP_GET_LIST_COUNT((SdpMedia *)pMedia,slBandwidth,pCount,pErr)

/***********************************************************************
** FUNCTION: sdp_getBandwidthAtIndexFromMedia
** DESCRIPTION: get bandwidth line at a specified index from Sdp Media line
** PARAMETERS:
**		pMedia (IN)		- Sdp Media line
**		ppBwidth(OUT)	- retrieved bandwidh field
**		dIndex (IN)		- Index at which to retrieve
**		pErr	(OUT)	- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getBandwidthAtIndexFromMedia(pMedia,ppBandwidth,index,pErr)\
    SIP_GET_LIST_AT_INDEX((SdpMedia *)pMedia,slBandwidth,(void **)ppBandwidth,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setBandwidthAtIndexInMedia
** DESCRIPTION:	set bandwidth line at a specified index in Sdp Media line
** PARAMETERS:
**		pMedia (IN/OUT)		- Sdp Media line
**		pBWidth(IN) 		- bandwidth value to set
** 		dIndex (IN)			- Index at which to set
**		pErr	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setBandwidthAtIndexInMedia(pMedia,pBandwidth,index,pErr)\
    SIP_SET_LIST_AT_INDEX((SdpMedia *)pMedia,slBandwidth,(SIP_Pvoid)pBandwidth,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_insertBandwidthAtIndexInMedia
** DESCRIPTION:	insert bandwidth line at a specified index in Sdp Media line
** PARAMETERS:
**		pMedia (IN/OUT)		- Sdp Media line
**		pBWidth(IN) 		- bandwidth value to insert
** 		dIndex (IN)			- Index at which to insert
**		pErr(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_insertBandwidthAtIndexInMedia(pMedia,pBandwidth,index,pErr)\
    SIP_INSERT_LIST_AT_INDEX((SdpMedia *)pMedia,slBandwidth,(SIP_Pvoid)pBandwidth,index,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_deleteBandwidthAtIndexInMedia
** DESCRIPTION:	delete bandwidth line at a specified index in Sdp Media line
** PARAMETERS:
**		pMedia (IN/OUT)		- Sdp Media line
** 		dIndex  (IN)			- Index at which to delete
**		pErr   (OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_deleteBandwidthAtIndexInMedia(pMedia,index,pErr)\
    SIP_DELETE_LIST_AT_INDEX((SdpMedia *)pMedia,slBandwidth,index,pErr)

/***********************************************************************
** FUNCTION: sdp_getKeyFromMedia
** DESCRIPTION: get key line from Media line
** PARAMETERS:
**		media (IN)		- Sdp Media line
**		key (OUT)		- retrieved key line
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getKeyFromMedia(pMedia,ppOutKey,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMedia *)pMedia,pKey,ppOutKey,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setKeyInMedia
** DESCRIPTION: set key line in media line
** PARAMETERS:
**		media (IN/OUT)		- Sdp Media line
**		key (IN)			- key line to set
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setKeyInMedia(pMedia,pInKey,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMedia *)pMedia,pKey,pInKey,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getMvalueFromMedia
** DESCRIPTION: get media value from media line
** PARAMETERS:
**		media (IN)		- Sdp Media line
**		mvalue (OUT)	- retrieved media value line
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getMvalueFromMedia(pMedia,ppOutMValue,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMedia *)pMedia,pMediaValue,ppOutMValue,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setMvalueInMedia
** DESCRIPTION: set media value in media line
** PARAMETERS:
**		media (IN/OUT)		- Sdp Media line
**		mvalue (IN)			- media value line to set
**		err	(OUT)			- Possible error value (see API ref doc)

************************************************************************/
#define sdp_setMvalueInMedia(pMedia,pInMValue,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMedia *)pMedia,pMediaValue,pInMValue,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getFormatFromMedia
** DESCRIPTION: get format field from Media line
** PARAMETERS:
**		media (IN)		- Sdp Media line
**		format (OUT)	- retrieved Sdp Format field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getFormatFromMedia(pMedia,ppOutFormat,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMedia *)pMedia,pFormat,ppOutFormat,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setFormatInMedia
** DESCRIPTION: set format field in Media line
** PARAMETERS:
**		media (IN/OUT)	- Sdp Media line
**		format (IN)		- format field to set
**		err	(OUT)		- Possible error value (see API ref doc)
**
************************************************************************/
#define sdp_setFormatInMedia(pMedia,pInFormat,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMedia *)pMedia,pFormat,pInFormat,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getProtoFromMedia
** DESCRIPTION: get protocol field from Media line
** PARAMETERS:
**		media (IN)		- Sdp Media
**		proto (OUT)		- retrieved protocol field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getProtoFromMedia(pMedia,ppOutProtocol,pErr) \
	SIP_GET_PARAM_BY_REF((SdpMedia *)pMedia,pProtocol,ppOutProtocol,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_setProtoInMedia
* DESCRIPTION: set protocol field in Media line
** PARAMETERS:
**		media (IN/OUT)	- Sdp Media
**		proto (IN)		- protocol field to set
**		err	(OUT)		- Possible error value (see API ref doc)
**
************************************************************************/
#define sdp_setProtoInMedia(pMedia,pInProtocol,pErr) \
	SIP_SET_PARAM_BY_REF((SdpMedia *)pMedia,pProtocol,pInProtocol,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sdp_getPortFromMedia
** DESCRIPTION: get port field from Media line
** PARAMETERS:
**		media (IN)		- Sdp Media
**		port (OUT)		- port field retrieved
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getPortFromMedia(pMedia,pOutPort,pErr) \
	SIP_GET_PARAM_BY_VAL((SdpMedia *)pMedia,dPort,pOutPort,pErr)

/***********************************************************************
** FUNCTION: sdp_setPortInMedia
** DESCRIPTION: set port field in Media line
** PARAMETERS:
**		media (IN/OUT)	- Sdp Media
**		port (IN)		- port field to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setPortInMedia(pMedia,dInPort,pErr) \
	SIP_SET_PARAM_BY_VAL((SdpMedia *)pMedia,dPort,dInPort,pErr)
/***********************************************************************
** FUNCTION: sdp_getNumportFromMedia
** DESCRIPTION: get number of ports field from media line
** PARAMETERS:
**		media (IN)		- Sdp Media
**		numport (OUT)	- retrieved number of ports  field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
extern SipBool sdp_getNumportFromMedia _ARGS_(( SdpMedia *media,\
		SIP_U32bit *numport, SipError 	*err));

/***********************************************************************
** FUNCTION: sdp_setNumportInMedia
** DESCRIPTION: set number of ports field in media line
** PARAMETERS:
**		media (IN/OUT)	- Sdp Media
**		numport (IN)	- retrieved number of ports  field
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
extern SipBool sdp_setNumportInMedia _ARGS_(( SdpMedia *media,\
		SIP_U32bit numport,SipError *err));

/***********************************************************************
** FUNCTION: sdp_getConnectionCountFromMedia
** DESCRIPTION: get number of connection lines in media line
** PARAMETERS:
**		media (IN)		- Sdp Media
**		index (OUT)		- retrieved number of connection lines
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getConnectionCountFromMedia(pMedia,pCount,pErr)\
    SIP_GET_LIST_COUNT((SdpMedia *)pMedia,slConnection,pCount,pErr)

/***********************************************************************
** FUNCTION: sdp_getConnectionAtIndexFromMedia
** DESCRIPTION: get connection line at specified index from Media line
** PARAMETERS:
**		media (IN)		- Sdp Media
**		connection(OUT)	- retrieved connection line
**		index (IN)		- index to retrieve connection line from
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getConnectionAtIndexFromMedia(pMedia,ppConnection,index,pErr)\
    SIP_GET_LIST_AT_INDEX((SdpMedia *)pMedia,slConnection,(void **)ppConnection,index,pErr,SipSdpParamConnection)

/***********************************************************************
** FUNCTION: sdp_setConnectionAtIndexInMedia
** DESCRIPTION: set connection line at specified index in Media line
** PARAMETERS:
**		media (IN/OUT)	- Sdp Media
**		connection(IN)	- connection line to set
**		index (IN)		- index to set connection in
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setConnectionAtIndexInMedia(pMedia,pConnection,index,pErr)\
    SIP_SET_LIST_AT_INDEX((SdpMedia *)pMedia,slConnection,(SIP_Pvoid)pConnection,index,pErr,SipSdpParamConnection)

/***********************************************************************
** FUNCTION: sdp_insertConnectionAtIndexInMedia
** DESCRIPTION: insert connection line at specified index in Media line
** PARAMETERS:
**		media (IN/OUT)	- Sdp Media
**		connection(IN)	- connection line to insert
**		index (IN)		- index to insert connection in
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_insertConnectionAtIndexInMedia(pMedia,pConnection,index,pErr)\
    SIP_INSERT_LIST_AT_INDEX((SdpMedia *)pMedia,slConnection,(SIP_Pvoid)pConnection,index,pErr,SipSdpParamConnection)

/***********************************************************************
** FUNCTION: sdp_deleteConnectionAtIndexInMedia
** DESCRIPTION: delete connection at specified index in Media line
** PARAMETERS:
**		media (IN/OUT)	- Sdp Media
**		index (IN)		- index to insert connection in
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_deleteConnectionAtIndexInMedia(pMedia,index,pErr)\
    SIP_DELETE_LIST_AT_INDEX((SdpMedia *)pMedia,slConnection,index,pErr)

/***********************************************************************
** FUNCTION: sdp_getAttrCountFromMedia
** DESCRIPTION: get number of attribute lines in media line
** PARAMETERS:
**		media (IN)			- Sdp Media
**		index (OUT)			- number of attribute lines in media
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getAttrCountFromMedia(pMedia,pCount,pErr)\
    SIP_GET_LIST_COUNT((SdpMedia *)pMedia,slAttr,pCount,pErr)

/***********************************************************************
** FUNCTION: sdp_getAttrAtIndexFromMedia
** DESCRIPTION: get attribute line at specifed index from Media line
** PARAMETERS:
**		media (IN/OUT)		- Sdp Media
**		attr (OUT)			- retrieved attribute line
**		index (IN)			- index to retrieve from
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getAttrAtIndexFromMedia(pMedia,ppAttr,index,pErr)\
    SIP_GET_LIST_AT_INDEX((SdpMedia *)pMedia,slAttr,(void **)ppAttr,\
            index,pErr,SipSdpParamAttribute)

/***********************************************************************
** FUNCTION: sdp_setAttrAtIndexInMedia
** DESCRIPTION: set attribute line at specifed index in Media line
** PARAMETERS:
**		media (IN/OUT)		- Sdp Media
**		attr (IN)			- attribute line to set
**		index (IN)			- index to set at
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_setAttrAtIndexInMedia(pMedia,pAttr,index,pErr)\
    SIP_SET_LIST_AT_INDEX((SdpMedia *)pMedia,slAttr,(SIP_Pvoid)pAttr,\
            index,pErr,SipSdpParamAttribute)

/***********************************************************************
** FUNCTION: sdp_insertAttrAtIndexInMedia
** DESCRIPTION: insert attribute line at specifed index in Media line
** PARAMETERS:
**		media (IN/OUT)		- Sdp Media
**		attr (IN)			- attribute line to insert at
**		index (IN)			- index to insert at
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_insertAttrAtIndexInMedia(pMedia,pAttr,index,pErr)\
    SIP_INSERT_LIST_AT_INDEX((SdpMedia *)pMedia,slAttr,(SIP_Pvoid)pAttr,\
            index,pErr,SipSdpParamAttribute)

/***********************************************************************
** FUNCTION: sdp_deleteAttrAtIndexInMedia
** DESCRIPTION:  delete attribute line at specifed index in Media line
** PARAMETERS:
**		media (IN/OUT)		- Sdp Media
**		index (IN)			- index to  delete
**		err	(OUT)			- Possible error value (see API ref doc)
************************************************************************/
#define sdp_deleteAttrAtIndexInMedia(pMedia,index,pErr)\
    SIP_DELETE_LIST_AT_INDEX((SdpMedia *)pMedia,slAttr,index,pErr)

/***********************************************************************
** FUNCTION: sip_getLengthFromUnknownMessage
** DESCRIPTION: get length of unknow message body
** PARAMETERS:
**		msg(IN)		- Unknown message
**		length(OUT)	- retrieved length
**		err	(OUT)	- Possible error value (see API ref doc)
************************************************************************/
#define sip_getLengthFromUnknownMessage(pMsg,pLength,pErr) \
	SIP_GET_PARAM_BY_VAL((SipUnknownMessage *)pMsg,dLength,pLength,pErr)

/***********************************************************************
** FUNCTION: sip_getBufferFromUnknownMessage
** DESCRIPTION: get unknown message body
** PARAMETERS:
**		msg (IN)		- Unknown message
**		buffer (OUT)	- retrieved message buffer
************************************************************************/
#define sip_getBufferFromUnknownMessage(pMsg,ppOutBuffer,pErr) \
	SIP_GET_PARAM_BY_REF((SipUnknownMessage *)pMsg,pBuffer,ppOutBuffer,pErr,SipParamString)

/***********************************************************************
** FUNCTION: sip_setBufferInUnknownMessage
** DESCRIPTION: set unknown message body
** PARAMETERS:
**		msg (IN/OUT)	- unknown message
**		buffer (IN)		- buffer to set
**		length (IN)		- length of buffer
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
extern SipBool sip_setBufferInUnknownMessage _ARGS_(( SipUnknownMessage *msg, \
		SIP_S8bit *buffer, SIP_U32bit length,SipError *err));

/***********************************************************************
** FUNCTION: sip_getMsgBodyType
** DESCRIPTION: get type of message body that is present
** PARAMETERS:
**		body (IN)		- sip message body
**		type (OUT)		- type of message body retrieved
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sip_getMsgBodyType(pMsgBody,pType,pErr)\
	SIP_GET_PARAM_BY_VAL((SipMsgBody *)pMsgBody,dType,pType,pErr) 

/***********************************************************************
** FUNCTION: sip_getUnknownFromMsgBody
** DESCRIPTION: get unknown message body element from sip message body
** PARAMETERS:
**		msg (IN)		- sip message body
**		unknown(OUT)	- unknown message body retrieved
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sip_getUnknownFromMsgBody(pMsgBody,ppOutUnknown,pErr)\
	SIP_GET_PARAM_BY_REF((SipMsgBody *)pMsgBody,u.pUnknownMessage,ppOutUnknown,pErr,SipUnknownWhole)
/***********************************************************************
** FUNCTION: sip_setUnknownInMsgBody
** DESCRIPTION: set unknown message body  in sip message body
** PARAMETERS:
**		msg (IN/OUT)- sip message body
**		unknown(IN)	- unknown message body  to set
**		err	(OUT)	- Possible error value (see API ref doc)
************************************************************************/
#define sip_setUnknownInMsgBody(pMsgBody,pInUnknown,pErr) \
	SIP_SET_PARAM_BY_REF((SipMsgBody *)pMsgBody,u.pUnknownMessage,pInUnknown,pErr,SipUnknownWhole)
/***********************************************************************
** FUNCTION: sip_getSdpFromMsgBody
** DESCRIPTION: get sdp body from sip message body
** PARAMETERS:
**		msg (IN)	- sip message body
**		sdp (OUT)	- retrieved sdp message body
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sip_getSdpFromMsgBody(pMsg,ppOutSdp,pErr) \
	SIP_GET_PARAM_BY_REF((SipMsgBody *)pMsg,u.pSdpMessage,ppOutSdp,pErr,SipSdpWhole)

/***********************************************************************
** FUNCTION: sip_setSdpInMsgBody
** DESCRIPTION: set sdp body in sip message body
** PARAMETERS:
**		msg (IN/OUT)	- sip message body
**		sdp (IN)		- sdp message body to set
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sip_setSdpInMsgBody(pMsg,pInSdp,pErr) \
	SIP_SET_PARAM_BY_REF((SipMsgBody *)pMsg,u.pSdpMessage,pInSdp,pErr,SipSdpWhole)

/***********************************************************************
** FUNCTION: sdp_getIncorrectLineAtIndex
** DESCRIPTION: get incorrect line at specified index from Sdp Message
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		line (OUT)		- retrieved incorrect line (string)
**		cnt (IN)		- Index at which to retrieve
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getIncorrectLineAtIndex(pMsg,ppLine,index,pErr)\
    SIP_GET_LIST_AT_INDEX((SdpMessage *)pMsg,slIncorrectLines,(void **)ppLine,index,pErr,SipParamString)

/********************************************************************
** FUNCTION:sdp_deleteIncorrectLineAtIndex
** DESCRIPTION: This function deletes an incorrect SDP line (allowed by
**				the application during decode) at the specified index in
**				an SDP message structure
** PARAMETERS:
**		msg (IN)		- Sdp Message
**		cnt (IN)		- Index at which to delete
**		err	(OUT)		- Possible error value (see API ref doc)
**********************************************************************/
#define sdp_deleteIncorrectLineAtIndex(pMsg,index,pErr)\
    SIP_DELETE_LIST_AT_INDEX((SdpMessage *)pMsg,slIncorrectLines,index,pErr)
/***********************************************************************
** FUNCTION: sdp_getIncorrectLineAtIndexFromMedia
** DESCRIPTION: get incorrect line at specified index from Sdp Media
** PARAMETERS:
**		media (IN)		- Sdp Media
**		line (OUT)		- retrieved incorrect line (string)
**		cnt (IN)		- Index at which to retrieve
**		err	(OUT)		- Possible error value (see API ref doc)
************************************************************************/
#define sdp_getIncorrectLineAtIndexFromMedia(pMedia,ppLine,index,pErr)\
    SIP_GET_LIST_AT_INDEX((SdpMedia *)pMedia,slIncorrectLines,(void **)ppLine,index,pErr,SipParamString)

/********************************************************************
** FUNCTION:sdp_deleteIncorrectLineAtIndexInMedia
** DESCRIPTION: This function deletes an incorrect SDP line (allowed by
**				the application during decode) at the specified index in
**				an SDP media structure
** PARAMETERS:
**		media (IN)		- Sdp Media
**		cnt (IN)		- Index at which to delete
**		err	(OUT)		- Possible error value (see API ref doc)
**********************************************************************/
#define sdp_deleteIncorrectLineAtIndexInMedia(pMedia,index,pErr)\
    SIP_DELETE_LIST_AT_INDEX((SdpMedia *)pMedia,slIncorrectLines,index,pErr)




#ifdef SIP_MWI
/********************************************************************
** FUNCTION:
** DESCRIPTION: Accessor Functions for MWI
** PARAMETERS:
**		
**********************************************************************/

extern SipBool sip_mwi_getMesgSummaryFromMsgBody   \
        _ARGS_(( SipMsgBody *msg, MesgSummaryMessage **mwi, SipError *err));

extern SipBool sip_mwi_setMesgSummaryInMsgBody  _ARGS_(  \
        ( SipMsgBody *msg, MesgSummaryMessage *mwi, SipError *err));

extern SipBool sip_mwi_getStatusFromMesgSummaryMessage _ARGS_( ( \
	MesgSummaryMessage *pMsgSummary, en_StatusType *pStatusType,\
         SipError *pErr));

extern SipBool sip_mwi_setStatusInMesgSummaryMessage _ARGS_( ( \
        MesgSummaryMessage *pMsgSummary, en_StatusType dStatusType,\
         SipError *pErr));

extern SipBool sip_mwi_getSummaryLineCountFromMesgSummaryMessage _ARGS_( (\
        MesgSummaryMessage *pMsgSummary, SIP_U32bit *count, SipError *err ));

extern SipBool sip_mwi_getSummaryLineAtIndexFromMesgSummaryMessage _ARGS_( (\
         MesgSummaryMessage *pMsgSummary, SummaryLine **slSummaryLine,\
         SIP_U32bit index, SipError *err ));

extern SipBool sip_mwi_setSummaryLineAtIndexInMesgSummaryMessage _ARGS_( (\
         MesgSummaryMessage *pMsgSummary, SummaryLine *slSummaryLine, \
	SIP_U32bit index, SipError *err ));

extern SipBool sip_mwi_insertSummaryLineAtIndexInMesgSummaryMessage _ARGS_( (\
         MesgSummaryMessage *pMsgSummary, SummaryLine *slSummaryLine,\
	 SIP_U32bit index, SipError *err ));

extern SipBool sip_mwi_deleteSummaryLineAtIndexInMesgSummaryMessage _ARGS_( (\
         MesgSummaryMessage *pMsgSummary, SIP_U32bit index, \
	 SipError *err ));

extern SipBool sip_mwi_getNameValuePairCountFromMesgSummaryMessage _ARGS_( (\
         MesgSummaryMessage *pMsgSummary, SIP_U32bit *count, \
	SipError *err ));

extern SipBool sip_mwi_getNameValuePairAtIndexFromMesgSummaryMessage _ARGS_( (\
         MesgSummaryMessage *pMsgSummary, SipNameValuePair **slNameValue,\
         SIP_U32bit index, SipError *err ));

extern SipBool sip_mwi_setNameValuePairAtIndexInMesgSummaryMessage _ARGS_( (\
         MesgSummaryMessage *pMsgSummary, SipNameValuePair *slNameValue,\
	 SIP_U32bit index, SipError *err ));

extern SipBool sip_mwi_insertNameValuePairAtIndexInMesgSummaryMessage _ARGS_( (\
         MesgSummaryMessage *pMsgSummary, SipNameValuePair *slNameValue,\
	 SIP_U32bit index, SipError *err ));

extern SipBool sip_mwi_deleteNameValuePairAtIndexInMesgSummaryMessage _ARGS_( (\
         MesgSummaryMessage *pMsgSummary, SIP_U32bit index, \
	 SipError *err ));

extern SipBool sip_mwi_getMediaFromSummaryLine _ARGS_( (\
         SummaryLine *pSummaryLine, SIP_S8bit **pMedia, SipError *err));

extern SipBool sip_mwi_setMediaInSummaryLine _ARGS_( (\
         SummaryLine *pSummaryLine, SIP_S8bit *pMedia, SipError *err));

/*****Get/set functions for Message-Account header*****/
extern SipBool sip_mwi_getMesgAccountUrlFromMesgSummaryMessage \
    _ARGS_((MesgSummaryMessage *pMsgSummary, SipAddrSpec **ppAddrSpec,\
            SipError *pErr) );

extern SipBool sip_mwi_setMesgAccountUrlInMesgSummaryMessage \
    _ARGS_((MesgSummaryMessage *pMsgSummary, SipAddrSpec *pAddrSpec,\
            SipError *pErr) );
#endif


#endif /* __MICROSIP_MSGBODY_H__ */

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
	}
#endif


















