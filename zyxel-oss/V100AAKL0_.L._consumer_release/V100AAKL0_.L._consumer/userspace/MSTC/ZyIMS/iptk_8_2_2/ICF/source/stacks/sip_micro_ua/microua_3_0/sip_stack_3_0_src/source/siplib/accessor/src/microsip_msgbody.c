/***********************************************************************
** FUNCTION:
**			Message Body Accessor API functionality
************************************************************************
**
** FILENAME:
** microsip_msgbody.c
**
** DESCRIPTION:
**		This file contains the accessor functions for message bodies.
**		This includes APIs for manipulating the MIME headers present
**		inside a MIME message.
**
** DATE	 	    NAME			 REFERENCE	 	REASON
** ----	 	    ----			 ---------	 	--------
** 9/09/2002	 Aparna Kuppachi		-			Initial
** 05-Jun-2009   Anuradha Gupta         SPR 19735       Klocworks Warning Removal
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/

#include "microsip_free.h"
#include "microsip_bcptfree.h"
#include "microsip_init.h"
#include "microsip_msgbody.h"

#ifdef SS_0 /* To be removed after verification */
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

SipBool sip_getMsgBodyAtIndex
(SipMessage *pMsg, SipMsgBody **ppMsgBody, SIP_U32bit index, SipError *pErr)
{
	SipMsgBody *body_from_list;

#ifndef SIP_NO_CHECK
	/* Validate parameters */
	if	(pErr==SIP_NULL) 
		return SipFail;

	if ((pMsg==SIP_NULL)||(ppMsgBody==SIP_NULL)) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* retrieve message pBody from list */
	if (sip_listGetAt(&(pMsg->slMessageBody), index, \
		(SIP_Pvoid *)&(body_from_list), pErr) == SipFail)
		return SipFail;
	/* clone message pBody into dest parameter */
	*ppMsgBody = body_from_list;
	HSS_LOCKEDINCREF((*ppMsgBody)->dRefCount);

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


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

SipBool sip_setMsgBodyAtIndex
(SipMessage *pMsg, SipMsgBody *pMsgBody, SIP_U32bit index, SipError *pErr)
{
	SipMsgBody *body_in_list;

#ifndef SIP_NO_CHECK
	/* Validate parameters */
	if (pErr==SIP_NULL) 
		return SipFail;

	if ((pMsg==SIP_NULL)||(pMsgBody==SIP_NULL)) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	
	/* Validate pBody dType */
	switch (pMsgBody->dType)
	{
		case SipSdpBody:
		case SipUnknownBody:
		case SipMultipartMimeBody:   /* bcpt ext */
		case SipIsupBody:		  	/* bcpt ext */
#ifdef SIP_MWI
        case SipMessageSummaryBody:
#endif
		case SipAppSipBody:
			break;
		default:
			*pErr = E_INV_PARAM;
			return SipFail;
	}
#endif

	/* clone pBody */
	body_in_list=pMsgBody;
	HSS_LOCKEDINCREF(body_in_list->dRefCount);
	/* set message pBody in list */
	if (sip_listSetAt(&(pMsg->slMessageBody), index, \
		(SIP_Pvoid)body_in_list, pErr) == SipFail)
	{
		sip_freeSipMsgBody(body_in_list);
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


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

SipBool sip_insertMsgBodyAtIndex
(SipMessage *pMsg, SipMsgBody *pMsgBody, SIP_U32bit index, SipError *pErr)
{
	SipMsgBody *body_in_list;

#ifndef SIP_NO_CHECK
	/* Validate parameters */
	if (pErr==SIP_NULL) 
		return SipFail;

	if ((pMsg==SIP_NULL)||(pMsgBody==SIP_NULL)) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	
	/* Validate pBody dType */
	switch (pMsgBody->dType)
	{
		case SipSdpBody:
		case SipUnknownBody:
		case SipIsupBody:                     /* bcpt ext */
		case SipMultipartMimeBody:			  /* bcpt ext */
#ifdef SIP_MWI
        case SipMessageSummaryBody:
#endif
		case SipAppSipBody:
			break;
		default:
			*pErr = E_INV_PARAM;
			return SipFail;
	}
#endif
	
	/* clone pBody */
	body_in_list=pMsgBody;
	HSS_LOCKEDINCREF(body_in_list->dRefCount);
	/* insert message pBody in list */
	if (sip_listInsertAt(&(pMsg->slMessageBody), index, \
		(SIP_Pvoid)body_in_list, pErr) == SipFail)
	{
		sip_freeSipMsgBody(body_in_list);
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	return SipSuccess;
}



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

SipBool sip_getTypeFromMsgBody
(SipMsgBody *pMsgBody, en_SipMsgBodyType *pType, SipError *pErr)
{
#ifndef SIP_NO_CHECK	
	/* Validate parameters */
	if (pErr==SIP_NULL) 
		return SipFail;

	if ((pMsgBody==SIP_NULL)||(pType==SIP_NULL)) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* copy message dType into dest parameter*/
	*pType = pMsgBody->dType;

	*pErr = E_NO_ERROR;
	return SipSuccess;
}
#endif

/****************************************************************************
** FUNCTION: sip_getMsgBodyTypeAtIndex
** DESCRIPTION: This function retreives the dType of the msg body 
**				(Sdp / MultipartMime / Isup /AppSip / Unknown)
**				present at a specified index in a SipMessage.
** 
** PARAMETERS:
**				pMsg(IN)		- Pointer to SipMessage 
**				pType(OUT)		- The retrieved msg body type 
**				dIndex(IN)		- The index of the msg body whose type is to 
**								  be retrieved
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getMsgBodyTypeAtIndex
(SipMessage *pMsg, en_SipMsgBodyType *pType, SIP_U32bit dIndex, SipError *pErr)
{
	SipMsgBody *body_from_list;

	/* Validate parameters */
#ifndef SIP_NO_CHECK
	if (pErr==SIP_NULL) 
		return SipFail;

	if ((pMsg==SIP_NULL)||(pType==SIP_NULL)) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	/* retrieve message pBody from list */
	if (sip_listGetAt(&(pMsg->slMessageBody), dIndex, (SIP_Pvoid *) \
		&(body_from_list), pErr) == SipFail)
		return SipFail;
	/* copy message dType into dest parameter*/
	*pType = body_from_list->dType;

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

#ifdef SIP_MIME_PARSING
/****************************************************************************
** FUNCTION: sip_bcpt_getMsgBodyAtIndexFromMime
** DESCRIPTION: This function gets the messsage body structure at a specified  
**				index (starting from 0) from a Mime Message.
**
** PARAMETERS:
**				pMime(IN)		- Pointer to the Mime message
**				ppMsgB(OUT)		- The retrieved message body
**				dIndex(IN)		- The index from which to retrieve
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_getMsgBodyAtIndexFromMime
	( MimeMessage 	*pMime,
	  SipMsgBody **ppMsgB,
	  SIP_U32bit 	dIndex, 
	  SipError 	*pErr )
{
	SIP_Pvoid 	pElementFromList;

	SIPDEBUGFN ( "Entering getMsgBodyAtIndexFromMime");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if ((pMime == SIP_NULL) || (ppMsgB == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listGetAt(&(pMime->slRecmimeBody), dIndex, &pElementFromList, \
		pErr) == SipFail)
		return SipFail;
	
	if (pElementFromList == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	*ppMsgB = (SipMsgBody *) pElementFromList;
	HSS_LOCKEDINCREF((*ppMsgB)->dRefCount);

	SIPDEBUGFN ( "Exitting getMsgBodyAtIndexFromMime");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
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
	(en_SipMsgBodyType dType, SipError *pErr)
{
	SIPDEBUGFN("Entering function validateSipMsgBodyType");
	switch (dType)
	{
		case 	SipSdpBody				:
		case	SipIsupBody				:
		case	SipMultipartMimeBody	:
		case	SipUnknownBody			:
		case	SipBodyAny				:
#ifdef SIP_MWI
        case SipMessageSummaryBody      :
#endif
		case	SipAppSipBody			:
						break;
		default			:*pErr = E_INV_TYPE;
					 return SipFail;

	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function validateSipMsgBodyType");
	return SipSuccess;

}

#ifdef SIP_MIME_PARSING

/****************************************************************************
** FUNCTION: sip_bcpt_getMsgBodyTypeAtIndexFromMime
** DESCRIPTION: This function gets the Messsage Body structure dType
**				(mime/sdp/unknown/isup/appsip) from a specified index
**				in a Mime message.
**
** PARAMETERS:
**				pMime(IN)		- Pointer to Mime message
**				pType(OUT)		- The retrieved dType
**				dIndex(IN)		- The index from which to retrieve
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_bcpt_getMsgBodyTypeAtIndexFromMime
	( MimeMessage 		*pMime,
	  en_SipMsgBodyType *pType,
	  SIP_U32bit 		dIndex, 
	  SipError 			*pErr )
{
	SIP_Pvoid 	pElementFromList;

	SIPDEBUGFN ( "Entering getMsgBodyTypeAtIndexFromMime");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if ((pMime == SIP_NULL) || (pType == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif	
	if (sip_listGetAt(&(pMime->slRecmimeBody), dIndex, &pElementFromList, \
		pErr) == SipFail)
		return SipFail;
	
	if (pElementFromList == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	*pType = ((SipMsgBody*)(pElementFromList))->dType;

	SIPDEBUGFN("Exitting getMsgBodyTypeAtIndexFromMime");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

#endif
#ifdef SIP_ISUP
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

SipBool sip_bcpt_getIsupFromMsgBody
	( SipMsgBody *pMsgBody, IsupMessage **ppIsup, SipError *pErr)
{
	IsupMessage *pTempIsup;
	SIPDEBUGFN("Entering getIsupFromMsgBody\n"); 
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
	{
		return SipFail;
	}

	if ((pMsgBody == SIP_NULL) || (ppIsup == SIP_NULL))
	{
		*pErr = E_INV_PARAM; 
		return SipFail;
	}
	if (pMsgBody->dType != SipIsupBody)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif 
	if (pMsgBody->u.pIsupMessage == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	pTempIsup = pMsgBody->u.pIsupMessage;

	HSS_LOCKEDINCREF(pTempIsup->dRefCount);
	*ppIsup = pTempIsup;
		
	SIPDEBUGFN("Leaving getIsupFromMsgBody"); 
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


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
	(SipMsgBody *pMsgBody, IsupMessage *pIsup, SipError *pErr)
{
	SIPDEBUGFN("Entering sip_bcpt_setIsupInBody\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
	{
		return SipFail;
	}

	if (pMsgBody == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	
	if (pMsgBody->dType!=SipIsupBody)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	if (pMsgBody->u.pIsupMessage != SIP_NULL)
		sip_bcpt_freeIsupMessage( pMsgBody->u.pIsupMessage);
	
	if (pIsup==SIP_NULL)
	{
		pMsgBody->u.pIsupMessage = SIP_NULL;
	}
	else
	{	
		HSS_LOCKEDINCREF(pIsup->dRefCount);
		pMsgBody->u.pIsupMessage = pIsup;
	}	
	SIPDEBUGFN("Leaving sip_bcpt_setIsupBody\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
#endif

#ifdef SIP_MIME_PARSING
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

SipBool sip_bcpt_getMimeFromMsgBody
	( SipMsgBody *pMsgBody, MimeMessage **ppMime, SipError *pErr)
{
	MimeMessage *pTempMime = SIP_NULL;

	SIPDEBUGFN("Entering sip_bcpt_getMimeFromMsgBody\n"); 
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
	{
		return SipFail;
	}

	if ((pMsgBody == SIP_NULL) || (ppMime == SIP_NULL))
	{
		*pErr = E_INV_PARAM; 
		return SipFail;
	}
	
	if (pMsgBody->dType != SipMultipartMimeBody)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	if (pMsgBody->u.pMimeMessage == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	pTempMime = pMsgBody->u.pMimeMessage;
	HSS_LOCKEDINCREF(pTempMime->dRefCount);
	*ppMime = pTempMime;

	SIPDEBUGFN("Leaving sip_bcpt_getMimeFromMsgBody"); 
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


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
	( SipMsgBody *pMsgBody, MimeMessage *pMime, SipError *pErr)
{
	
	SIPDEBUGFN("Entering sip_bcpt_setMimeInBody\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
	{
		return SipFail;
	}

	if (pMsgBody == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	
	if (pMsgBody->dType!=SipMultipartMimeBody)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	if (pMsgBody->u.pMimeMessage != SIP_NULL)
		sip_bcpt_freeMimeMessage( pMsgBody->u.pMimeMessage);

	if (pMime==SIP_NULL)
	{
		pMsgBody->u.pMimeMessage = SIP_NULL;
	}
	else
	{	
		HSS_LOCKEDINCREF(pMime->dRefCount);
		pMsgBody->u.pMimeMessage = pMime;
	}	

	SIPDEBUGFN("Leaving sip_bcpt_setMimeBody\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
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
	( IsupMessage *pMsg, SIP_U32bit *pLength, SipError *pErr)
{
	SIPDEBUGFN ( "Entering getlengthFromIsupMessage");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if (pMsg == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pLength == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif	
	*pLength = pMsg->dLength; 
	
	SIPDEBUGFN ( "Exiting getlengthFromIsupMessage");
	
	*pErr = E_NO_ERROR;
	return SipSuccess;

}


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
	(IsupMessage *pMsg, SIP_S8bit **ppBody, SipError *pErr)
{
	SIPDEBUGFN ( "Entering getBodyFromIsupMessage");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if (pMsg == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	
	if (pMsg->pBody == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}	
#endif
	*ppBody = pMsg->pBody;
	SIPDEBUGFN("Exiting getBodyFromIsupMessage");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}


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
	(IsupMessage *pMsg, SIP_S8bit *pBody, SIP_U32bit dLength,SipError *pErr)
{

	SIPDEBUGFN ( "Entering setBodyInIsupMessage");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if (pMsg == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pMsg->pBody != SIP_NULL) 
	{
		 if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(pMsg->pBody)), \
			pErr) == SipFail)
			return SipFail;
	}
#endif
	pMsg->pBody = pBody;
	pMsg->dLength = dLength;	

	SIPDEBUGFN("Exiting setBodyInIsupMessage");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}
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
	(SipMsgBody *pMsgBody, SipMimeHeader **ppMime, SipError *pErr)
{
	SipMimeHeader *pTempMime = SIP_NULL;

	SIPDEBUGFN("Entering getMimeHeaderFromMsgBody\n"); 
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
	{
		return SipFail;
	}

	if ((pMsgBody == SIP_NULL) || (ppMime == SIP_NULL))
	{
		*pErr = E_INV_PARAM; 
		return SipFail;
	}
#endif	
	if (pMsgBody->pMimeHeader == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	pTempMime = pMsgBody->pMimeHeader;
	HSS_LOCKEDINCREF(pTempMime->dRefCount);
	*ppMime = pTempMime;
	SIPDEBUGFN("Leaving getMimeHeaderFromMsgBody"); 
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


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
	(SipMsgBody *pMsgBody, SipMimeHeader *pMime, SipError *pErr)
{
	
	SIPDEBUGFN("Entering sip_bcpt_setMimeHeaderInBody\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
	{
		return SipFail;
	}

	if (pMsgBody == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (pMsgBody->pMimeHeader != SIP_NULL)
		sip_bcpt_freeSipMimeHeader( pMsgBody->pMimeHeader);
		
	if (pMime==SIP_NULL)
	{
		pMsgBody->pMimeHeader = SIP_NULL;
	}
	else
	{	
		HSS_LOCKEDINCREF(pMime->dRefCount);
		pMsgBody->pMimeHeader = pMime;	
	}	

	SIPDEBUGFN("Leaving sip_bcpt_setMimeHeaderinMsgBody\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
#endif


#ifdef SIP_MIME_PARSING

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
	(SipHeader *pHdr, SIP_S8bit *pVersion, SipError *pErr)
{
	SIP_S8bit *pVer;
	SIPDEBUGFN("Entering function sip_setVersionInMimeVersionHdr");
#ifndef SIP_NO_CHECK
    if (pErr == SIP_NULL)
          return SipFail;

    if (pHdr == SIP_NULL)
    {
         *pErr = E_INV_PARAM;
          return SipFail;
    }

    if (pHdr->pHeader == SIP_NULL)
    {
        *pErr = E_INV_HEADER;
         return SipFail;
    }
    if (pHdr->dType != SipHdrTypeMimeVersion)
    {
        *pErr = E_INV_TYPE;
         return SipFail;
    }
#endif

	pVer = ((SipMimeVersionHeader *)(pHdr->pHeader))->pStr1;

	if (pVer != SIP_NULL)
    {
        if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&pVer), pErr) == SipFail)
            return SipFail;
    }

	((SipMimeVersionHeader *)(pHdr->pHeader))->pStr1 = pVersion;

	*pErr  = E_NO_ERROR;
     SIPDEBUGFN("Exitting function sip_setVersionInMimeVersionHdr");
     return SipSuccess;
}


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
		SipError *pErr)
{
        SIPDEBUGFN("Entering function sip_getContentDispositionFromMimeHdr");
#ifndef SIP_NO_CHECK
        if (pErr == SIP_NULL)
                return SipFail;

        if (pMimeHdr == SIP_NULL)
        {
                *pErr = E_INV_PARAM;
                return SipFail;
        }
        if (ppContentDisposition == SIP_NULL)
        {
       		 *pErr = E_INV_PARAM;
             return SipFail;
        }
#endif
	if (pMimeHdr->pContentDisposition == SIP_NULL)
        {
                *pErr = E_NO_EXIST;
                return SipFail;
        }

		(* ppContentDisposition)->dType = SipHdrTypeContentDisposition; 
		HSS_LOCKEDINCREF(pMimeHdr->pContentDisposition->dRefCount);
		((*ppContentDisposition)->pHeader) = \
			(SIP_Pvoid ) pMimeHdr->pContentDisposition;
		
        *pErr = E_NO_ERROR;
        SIPDEBUGFN("Exitting function sip_getContentDispositionFromMimeHdr");
        return SipSuccess;
}


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
        (SipMimeHeader *pMimeHdr, SipHeader **ppContentType, SipError *pErr)
{
    SIPDEBUGFN("Entering function sip_getContentTypeFromMimeHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
			return SipFail;

	if (pMimeHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (ppContentType == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
		
	if (pMimeHdr->pContentType == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	(* ppContentType)->dType = SipHdrTypeContentType; 
	HSS_LOCKEDINCREF(pMimeHdr->pContentType->dRefCount);
	((*ppContentType)->pHeader) = (SIP_Pvoid ) pMimeHdr->pContentType;
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getContentTypeFromMimeHdr");
	return SipSuccess;
}


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
        (SipMimeHeader *pMimeHdr, SipHeader *pHdr, SipError *pErr)
{
	SipContentTypeHeader *pContentType;

    SIPDEBUGFN("Entering function sip_setContentTypeInMimeHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if (pMimeHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	
	if (pHdr!=SIP_NULL)
	{	
		if (pHdr->dType != SipHdrTypeContentType)
		{
			*pErr = E_INV_HEADER;
			return SipFail;
		}
	}	
#endif
	
	if (pMimeHdr->pContentType != SIP_NULL) 
		sip_freeSipContentTypeHeader(pMimeHdr->pContentType);

	if (pHdr==SIP_NULL)
	{
		pMimeHdr->pContentType = SIP_NULL;
	}
	else
	{	
		pContentType = (SipContentTypeHeader *) (pHdr->pHeader);
		if (pContentType==SIP_NULL)
		{
			pMimeHdr->pContentType = SIP_NULL;
		}
		else
		{	
			HSS_LOCKEDINCREF(pContentType->dRefCount);
			pMimeHdr->pContentType = pContentType;
		}
	}	
	*pErr  = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setContentTypeInMimeHdr");
	return SipSuccess;
}


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
        (SipMimeHeader *pMimeHdr, SipHeader *pHdr, SipError *pErr)
{
	SipContentDispositionHeader *pContentDisposition;
    SIPDEBUGFN("Entering function sip_setContentDispositionInMimeHdr");
#ifndef SIP_NO_CHECK
    if (pErr == SIP_NULL)
       return SipFail;

	if (pMimeHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pHdr->dType != SipHdrTypeContentDisposition)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif
	
	if (pMimeHdr->pContentDisposition != SIP_NULL) 
		sip_freeSipContentDispositionHeader(pMimeHdr->pContentDisposition);

	pContentDisposition = (SipContentDispositionHeader *) ( pHdr->pHeader);

	HSS_LOCKEDINCREF(pContentDisposition->dRefCount);
	pMimeHdr->pContentDisposition = pContentDisposition;
	
	*pErr  = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setContentDispositionInMimeHdr");
	return SipSuccess;
}


#endif

/****************************************************************************
** FUNCTION: sip_getListAtIndex
** DESCRIPTION: This function gets the element at an index from SipList 
** 
** PARAMETERS:
**				pList(IN)	    - Pointer to SipList
**              ppData(OUT)     - The retrieved element
**              dIndex(IN)       - Index for which the element is to be retrieved
**              dType           - Type of param that is to be retrieved
**				pErr(OUT)		- Error value if any 
**
****************************************************************************/
SipBool sip_getListAtIndex
    (SipList                *pList, 
     void                   **ppData, 
     SIP_U32bit             dIndex, 
     en_SipMsgBodyParamType dType,
     SipError               *pErr) 
{
    SIP_Pvoid  pBody = SIP_NULL;
	SIPDEBUGFN("Enterring sip_getListAtIndex\n");
#ifndef SIP_NO_CHECK
    /* Validate parameters */
    if	(pErr==SIP_NULL) 
        return SipFail;

    if	(pList==SIP_NULL) 
    {
        *pErr = E_INV_PARAM;
        return SipFail;
    }

    if (ppData==SIP_NULL) 
    {
        *pErr = E_INV_PARAM;
        return SipFail;
    }
#endif
    /* retrieve message pBody from list */
    if (sip_listGetAt(pList, dIndex, &pBody, pErr) == SipFail)
        return SipFail;

	if ( pBody == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

    *ppData = pBody;

    if(SipFail == sip_incrementRefCount(pBody,dType,pErr))
	{
		return SipFail;
	}
	SIPDEBUGFN("Exiting sip_getListAtIndex\n");
    *pErr = E_NO_ERROR;
    return SipSuccess;
}

/********************************************************************
** FUNCTION:sip_setListAtIndex
**
** DESCRIPTION: This function sets the list field at a specified
**		index in an list structure
**
**********************************************************************/

SipBool sip_setListAtIndex
    ( SipList               *pList, 
	  SIP_Pvoid             *pData, 
      SIP_U32bit            dIndex, 
	  en_SipMsgBodyParamType dType,
      SipError              *pErr )
{

	SIPDEBUGFN("Enterring sip_setListAtIndex\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pList == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if((dType == SipMsgBodyWhole)&&\
			(pData == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listSetAt(pList,dIndex,pData,pErr) == SipFail)
	{
        (void)sip_freeParam(pData,dType,pErr);
		return SipFail;
	}
    if(SipFail == sip_incrementRefCount(pData,dType,pErr))
	{
		return SipFail;
	}
	SIPDEBUGFN("Exitting sip_setListAtIndex\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***************************************************************************
** FUNCTION: sip_insertListAtIndex
**
** DESCRIPTION: This function inserts a list field at a specified
**		index
**
****************************************************************************/
SipBool sip_insertListAtIndex
	( SipList               *pList,
      SIP_Pvoid             pData, 
      SIP_U32bit            dIndex, 
	  en_SipMsgBodyParamType dType,
      SipError              *pErr )
{
	SIPDEBUGFN("Entering sip_insertListAtIndex\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pList == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listInsertAt(pList,dIndex,pData, pErr) == SipFail)
	{
		return SipFail;
	}

    if(SipFail == sip_incrementRefCount(pData,dType,pErr))
	{
		return SipFail;
	}
	SIPDEBUGFN("Exitting sip_insertListAtIndex\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/********************************************************************
** FUNCTION:sip_deleteListAtIndex
**
** DESCRIPTION: This function deletes a list field at a specified
**		index in an SDP message structure
**
**********************************************************************/

SipBool sip_deleteListAtIndex
	( SipList       *pList,
      SIP_U32bit    dIndex, 
      SipError      *pErr )
{

	SIPDEBUGFN("Entering sip_deleteListAtIndex\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL)
		return SipFail;

	if (pList == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listDeleteAt(pList, dIndex, pErr) == SipFail)
		return SipFail;

	SIPDEBUGFN("Exitting sip_deleteListAtIndex\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/********************************************************************
** FUNCTION:sip_getListCount
**
** DESCRIPTION: This function retrieves the List count from an
**		SDP message structure
**
**********************************************************************/

SipBool sip_getListCount
	( SipList       *pList,
      SIP_U32bit    *pCount, 
      SipError      *pErr )
{
	SIPDEBUGFN("Entering sip_getListCount\n");
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL )
		return SipFail;

	if( (pList == SIP_NULL) || ( pCount == SIP_NULL ) )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (sip_listSizeOf(pList, pCount , pErr) == SipFail )
	{
		return SipFail;
	}

	SIPDEBUGFN("Exitting sip_getListCount\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/********************************************************************
** FUNCTION:sip_incrementRefCount
**
** DESCRIPTION: This functon increments refcount depending on the type
**
**********************************************************************/

SipBool sip_incrementRefCount
	(SIP_Pvoid          pSrc, 
	 en_SipMsgBodyParamType   dType,
	 SipError           *pErr) 

{
	SIPDEBUGFN("Entering sip_incrementRefCount\n");
	if(pSrc == SIP_NULL)
		return SipSuccess;

    switch(dType)
    {
        case SipSdpParamConnection:
            HSS_LOCKEDINCREF(((SdpConnection*)pSrc)->dRefCount);
            break;
        case SipSdpParamOrigin:
            HSS_LOCKEDINCREF(((SdpOrigin*)pSrc)->dRefCount);
            break;
        case SipSdpParamMedia:
            HSS_LOCKEDINCREF(((SdpMedia*)pSrc)->dRefCount);
            break;
        case SipSdpWhole:
            HSS_LOCKEDINCREF(((SdpMessage*)pSrc)->dRefCount);
            break;
        case SipMsgBodyWhole:
            HSS_LOCKEDINCREF(((SipMsgBody*)pSrc)->dRefCount);
            break;
        case SipUnknownWhole:
            HSS_LOCKEDINCREF(((SipUnknownMessage*)pSrc)->dRefCount);
            break;
        case SipAppSipMessage:
            HSS_LOCKEDINCREF(((SipMessage*)pSrc)->dRefCount);
            break;
		case SipSdpParamTime:
            HSS_LOCKEDINCREF(((SdpTime*)pSrc)->dRefCount);
			break;
		case SipSdpParamAttribute:
            HSS_LOCKEDINCREF(((SdpAttr*)pSrc)->dRefCount);
			break;
        default:
            /* For all other params RefCount need not be incremented */
            break;
    }
	SIPDEBUGFN("Exiting sip_incrementRefCount\n");
	*pErr = E_NO_ERROR;
    return SipSuccess;
}

/********************************************************************
** FUNCTION:sip_freeParam
**
** DESCRIPTION: This functon increments refcount depending on the type
**
**********************************************************************/ 
SipBool sip_freeParam
	(SIP_Pvoid                pSrc, 
	 en_SipMsgBodyParamType   dType,
	 SipError                 *pErr) 

{
	SIPDEBUGFN("Entering sip_freeParam\n");
	if(pSrc == SIP_NULL)
		return SipSuccess;

    switch(dType)
    {
        case SipParamString:
			if(SipFail == fast_memfree(ACCESSOR_MEM_ID,
						(SIP_S8bit *)pSrc,
						pErr))
				return SipFail;
            break;
        case SipSdpParamConnection:
			sip_freeSdpConnection((SdpConnection *)pSrc);
            break;
        case SipSdpParamOrigin:
			sip_freeSdpOrigin((SdpOrigin *)pSrc);
            break;
        case SipSdpParamMedia:
			sip_freeSdpMedia((SdpMedia *)pSrc);
            break;
        case SipSdpWhole:
			sip_freeSdpMessage((SdpMessage *)pSrc);
            break;
        case SipMsgBodyWhole:
			sip_freeSipMsgBody((SipMsgBody *)pSrc);
            break;
        case SipUnknownWhole:
            sip_freeSipUnknownMessage((SipUnknownMessage *)pSrc);
            break;
        case SipAppSipMessage:
            sip_freeSipMessage((SipMessage *)pSrc);
            break;
		case SipSdpParamTime:
			sip_freeSdpTime((SdpTime *)pSrc);
			break;
		case SipSdpParamAttribute:
			sip_freeSdpAttr((SdpAttr *)pSrc);
			break;
        default:
            /* For all other params, no need to free */
            break;
    }
	SIPDEBUGFN("Exiting sip_freeParam\n");
	*pErr = E_NO_ERROR;
    return SipSuccess;
}

/********************************************************************
** FUNCTION:sip_getParam
**
** DESCRIPTION: This functon derives the out param from the IN param
**
**********************************************************************/

SipBool sip_getParam
(   void                *pInParam, 
    void                **ppOutParam, 
    en_SipMsgBodyParamType    dType,
    SipError            *pErr 
)
{
	SIPDEBUGFN("Entering sip_getParam\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (ppOutParam==SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	
	if( pInParam == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
#endif
    /* Increment the refcount */
    if(SipFail == sip_incrementRefCount(pInParam,dType,pErr))
	{
		return SipFail;
	}
	*ppOutParam = pInParam;

	SIPDEBUGFN("Exitting sip_getParam\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}
/********************************************************************
** FUNCTION:sip_setParam
**
** DESCRIPTION: This function sets the Param field in the IN param field
**              to OUT param
**
**********************************************************************/
SipBool sip_setParam
(   void                *pInParam, 
    void                **ppOutParam, 
    en_SipMsgBodyParamType    dType,
    SipError            *pErr 
)
{
	SIPDEBUGFN("Entering sip_setParam\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
#endif
	if ( ppOutParam != SIP_NULL )
	{
		if ( sip_freeParam(*ppOutParam, dType, pErr) == SipFail)
			return SipFail;
		/* Set the value */
		*ppOutParam = pInParam;
	}

    /* Increment the refcount */
    if(SipFail == sip_incrementRefCount(pInParam,dType,pErr))
	{
		return SipFail;
	}
    
	SIPDEBUGFN("Exitting sip_setParam\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/*********************************************************************
** FUNCTION:  sdp_getNumportFromMedia
**
**********************************************************************
**
** DESCRIPTION: Gets the pPortNum field pValue from sdp slMedia
** structure.
**
*********************************************************************/

SipBool sdp_getNumportFromMedia
#ifdef ANSI_PROTO
	( SdpMedia 	*slMedia,
	  SIP_U32bit 	*pPortNum,
	  SipError 	*err)
#else
	( slMedia, pPortNum, err )
	  SdpMedia *slMedia;
	  SIP_U32bit *pPortNum;
	  SipError *err;
#endif
{
	SIP_U32bit * temp_numport;

	SIPDEBUGFN ( "Entering getNumportFromMedia");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL )
		return SipFail;

	if ( (slMedia == SIP_NULL) || (pPortNum == SIP_NULL) )
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	temp_numport = slMedia->pPortNum;
	if ( temp_numport == SIP_NULL)
	{
		*err = E_NO_EXIST;
		return SipFail;
	}

	*pPortNum = *temp_numport;
	SIPDEBUGFN ( "Exitting getNumportFromMedia");
	*err = E_NO_ERROR;
	return SipSuccess;

}

/*********************************************************************
** FUNCTION:  sdp_setNumportInMedia
**
**********************************************************************
**
** DESCRIPTION: Sets the pPortNum field pValue in sdp slMedia structure.
**
*********************************************************************/
SipBool sdp_setNumportInMedia
#ifdef ANSI_PROTO
	( SdpMedia *slMedia,
	  SIP_U32bit pPortNum,
	  SipError *err)
#else
	( slMedia, pPortNum, err )
	  SdpMedia *slMedia;
	  SIP_U32bit pPortNum;
	  SipError *err;
#endif
{
	SIP_U32bit *temp_numport;
	  SipError local_err;

	SIPDEBUGFN ( "Entering setNumportInMedia");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL )
		return SipFail;

	if ( slMedia == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif

	temp_numport = (SIP_U32bit*)fast_memget(ACCESSOR_MEM_ID, \
		sizeof(SIP_U32bit), err);
	if ( temp_numport == SIP_NULL)
		return SipFail;

	if( slMedia->pPortNum != SIP_NULL )
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(slMedia->pPortNum)),\
			err) == SipFail )
		{
			(void)sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid *)(&temp_numport),&local_err);
			return SipFail;
		}
	}

	*temp_numport = pPortNum;
	slMedia->pPortNum  = temp_numport;

	SIPDEBUGFN ( "Exitting setNumportInMedia");

	*err = E_NO_ERROR;
	return SipSuccess;
}

/*****************************************************************
**
** FUNCTION:  sip_setBufferInUnknownMessage
**
** DESCRIPTION: This function sets th unknoen message pBuffer in an
**		Unknown message
**
******************************************************************/
SipBool sip_setBufferInUnknownMessage
	( SipUnknownMessage *msg, SIP_S8bit *pBuffer, SIP_U32bit dLength,SipError *err)
{
	SIPDEBUGFN ( "Entering setBuferInUnknownMessage");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL )
		return SipFail;

	if ( msg == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	if ( msg->pBuffer !=SIP_NULL )
		(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(msg->pBuffer)), err);

	msg->dLength = dLength;
	msg->pBuffer = pBuffer;


	SIPDEBUGFN ( "Exiting setBufferInUnknownMessage");

	*err = E_NO_ERROR;
	return SipSuccess;
}

#ifdef SIP_MWI

/********************************************************************
** FUNCTION:sip_mwi_getMesgSummaryFromMsgBody
**
** DESCRIPTION: This Function retrieves Message Summary From Message Body
**
**********************************************************************/
SipBool sip_mwi_getMesgSummaryFromMsgBody
#ifdef ANSI_PROTO
	( SipMsgBody *pMsg, MesgSummaryMessage **ppMwi, SipError *pErr)
#else
	( pMsg, ppMwi, pErr )
	  SipMsgBody *pMsg;
	  MesgSummaryMessage **ppMwi;
	  SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_mwi_getMesgSummaryFromMsgBody\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
	{
		return SipFail;
	}
	if ( ( pMsg == SIP_NULL ) || ( ppMwi == SIP_NULL ) )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if ( pMsg->dType != SipMessageSummaryBody )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	if ( pMsg->u.pSummaryMessage == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	HSS_LOCKEDINCREF((pMsg->u.pSummaryMessage)->dRefCount);
	*ppMwi = (pMsg->u).pSummaryMessage;

	SIPDEBUGFN("exiting sip_mwi_getMesgSummaryFromMsgBody ");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/********************************************************************
** FUNCTION:sip_mwi_setMesgSummaryInMsgBody
**
** DESCRIPTION:This function sets the Message Summary in Msg Body
**
**********************************************************************/

SipBool sip_mwi_setMesgSummaryInMsgBody
#ifdef ANSI_PROTO
	( SipMsgBody *pMsg, MesgSummaryMessage *pMwi, SipError *pErr)
#else
	( pMsg, pMwi, pErr )
	  SipMsgBody *pMsg;
	  MesgSummaryMessage *pMwi;
	  SipError *pErr;
#endif
{
	MesgSummaryMessage	*pTempMwi=SIP_NULL;

	SIPDEBUGFN("Entering sip_mwi_setMesgSummaryInMsgBody\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
	{
		return SipFail;
	}

	if (pMsg == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if( pMsg->dType!=SipMessageSummaryBody )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	
	if (pMwi!=SIP_NULL)	
	{	
		HSS_LOCKEDINCREF(pMwi->dRefCount);
		pTempMwi = pMwi;
	}	
	if ( pMsg->u.pSummaryMessage != SIP_NULL)
		sip_mwi_freeMesgSummaryMessage( pMsg->u.pSummaryMessage);

	pMsg->u.pSummaryMessage = pTempMwi;

	SIPDEBUGFN("Leaving sip_mwi_setMesgSummaryInMsgBody\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/********************************************************************
** FUNCTION: sip_mwi_getStatusFromMesgSummaryMessage
**
** DESCRIPTION: This functon retrieves the status field from the
**		Mesg Summary Body
**
**********************************************************************/
SipBool sip_mwi_getStatusFromMesgSummaryMessage
#ifdef ANSI_PROTO
	(MesgSummaryMessage *pMsgSummary, en_StatusType *pStatusType,
	 SipError *pErr)
#else
	( pMsgSummary, pStatusType, pErr )
	MesgSummaryMessage *pMsgSummary;
	en_StatusType* pStatusType;
	SipError *pErr;

#endif
{
	SIPDEBUGFN("Entering sip_mwi_getStatusFromMesgSummaryMessage\n");
#ifndef SIP_NO_CHECK

	if ( pErr == SIP_NULL )
		return SipFail;

	if (( pMsgSummary == SIP_NULL) || ( pStatusType == SIP_NULL ))

	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	*pStatusType  =  pMsgSummary->dStatus;
	SIPDEBUGFN("Exiting sip_mwi_getStatusFromMesgSummaryMessage\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;

}
/********************************************************************
** FUNCTION: sip_mwi_setStatusInMesgSummaryMessage
**
** DESCRIPTION: This functon sets the status field in the
**		Mesg Summary Body
**
**********************************************************************/
SipBool sip_mwi_setStatusInMesgSummaryMessage
#ifdef ANSI_PROTO
	(MesgSummaryMessage *pMsgSummary, en_StatusType dStatusType,
	 SipError *pErr)
#else
	( pMsgSummary, dStatusType, pErr )
	MesgSummaryMessage *pMsgSummary;
	en_StatusType dStatusType;
	SipError *pErr;

#endif
{
	SIPDEBUGFN("Entering sip_mwi_setStatusInMesgSummaryMessage\n");
	if(( dStatusType != SipMsgWaitingYes ) && \
		( dStatusType != SipMsgWaitingNo ))
	  {
		*pErr = E_INV_TYPE;
		return SipFail;
	  }
#ifndef SIP_NO_CHECK

	if ( pErr == SIP_NULL )
		return SipFail;

	if ( pMsgSummary == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pMsgSummary->dStatus = dStatusType;
	SIPDEBUGFN("Exiting sip_mwi_setStatusInMesgSummaryMessage\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/********************************************************************
** FUNCTION:sip_mwi_getSummaryLineCountFromMesgSummaryMessage
**
** DESCRIPTION: This function retrieves the number of SummaryLines from an
**		Message Summary Body
**
**********************************************************************/

SipBool sip_mwi_getSummaryLineCountFromMesgSummaryMessage
#ifdef ANSI_PROTO
	( MesgSummaryMessage *pMsgSummary, SIP_U32bit *pIndex, SipError *pErr )
#else
	( pMsgSummary,pIndex,pErr)
	  MesgSummaryMessage *pMsgSummary;
	  SIP_U32bit *pIndex;
	  SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_mwi_getSummaryLineCountFromMesgSummaryMessage");
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL )
		return SipFail;

	if (( pMsgSummary == SIP_NULL) || ( pIndex == SIP_NULL ))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if(sip_listSizeOf(&(pMsgSummary->slSummaryLine), pIndex, pErr) ==\
	 SipFail )
	{
		return SipFail;
	}

	SIPDEBUGFN("Exitting sip_mwi_getSummaryLineCountFromMesgSummaryMessage\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/*******************************************************************
** FUNCTION: sip_mwi_getSummaryLineAtIndexFromMesgSummaryMessage
**
** DESCRIPTION: This function retrieves Summary Line at specified index
**		from Mesg Summary Message
**
*******************************************************************/
SipBool sip_mwi_getSummaryLineAtIndexFromMesgSummaryMessage
#ifdef ANSI_PROTO
	( MesgSummaryMessage *pMsgSummary, SummaryLine **slSummaryLine,\
	 SIP_U32bit dIndex, SipError *pErr )
#else
	( pMsgSummary,slSummaryLine,dIndex,pErr)
	  MesgSummaryMessage *pMsgSummary;
	  SummaryLine **slSummaryLine;
	  SIP_U32bit dIndex;
	  SipError *pErr;
#endif
{
	SIP_Pvoid element_from_list=SIP_NULL;
	SIPDEBUGFN("Entering " \
            "sip_mwi_getSummaryLineAtIndexFromMesgSummaryMessage\n");

#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (( pMsgSummary == SIP_NULL) || ( slSummaryLine == SIP_NULL ))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listGetAt(&(pMsgSummary->slSummaryLine), dIndex,\
		 &element_from_list, pErr) == SipFail)
		return SipFail;

	if ( element_from_list == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	HSS_LOCKEDINCREF((( SummaryLine* ) element_from_list)->dRefCount);
	*slSummaryLine = ( SummaryLine* ) element_from_list;

	SIPDEBUGFN("Exitting " \
            "sip_mwi_getSummaryLineAtIndexFromMesgSummaryMessage\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/*******************************************************************
** FUNCTION: sip_mwi_setSummaryLineAtIndexInMesgSummaryMessage
**
** DESCRIPTION: This function sets Summary Line at specified index
**		In Mesg Summary Message
**
**
*******************************************************************/
SipBool sip_mwi_setSummaryLineAtIndexInMesgSummaryMessage
#ifdef ANSI_PROTO
	( MesgSummaryMessage *pMsgSummary, SummaryLine *slSummaryLine,\
	 SIP_U32bit dIndex, SipError *pErr )
#else
	( pMsgSummary,slSummaryLine,dIndex,pErr)
	  MesgSummaryMessage *pMsgSummary;
	  SummaryLine *slSummaryLine;
	  SIP_U32bit dIndex;
	  SipError *pErr;
#endif
{

	SummaryLine* element_in_list=SIP_NULL;

	SIPDEBUGFN("Entering sip_mwi_setSummaryLineAtIndexInMesgSummaryMessage\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pMsgSummary == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (slSummaryLine == SIP_NULL)
		element_in_list = SIP_NULL;
	else
	{
		element_in_list = slSummaryLine;
		HSS_LOCKEDINCREF((( SummaryLine* ) element_in_list)->dRefCount);
	}

	if( sip_listSetAt( &(pMsgSummary->slSummaryLine), dIndex,\
	 (SIP_Pvoid) element_in_list, pErr) == SipFail)
	{
#ifndef SIP_BY_REFERENCE
		__sip_mwi_freeSummaryLine(element_in_list);
#endif
		return SipFail;
	}

	SIPDEBUGFN("Exiting sip_mwi_setSummaryLineAtIndexInMesgSummaryMessage\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/*******************************************************************
** FUNCTION: sip_mwi_insertSummaryLineAtIndexInMesgSummaryMessage
**
** DESCRIPTION: This function Inserts Summary Line at specified index
**		In Mesg Summary Message
**
**
*******************************************************************/
SipBool sip_mwi_insertSummaryLineAtIndexInMesgSummaryMessage
#ifdef ANSI_PROTO
	( MesgSummaryMessage *pMsgSummary, SummaryLine *slSummaryLine,\
	 SIP_U32bit dIndex, SipError *pErr )
#else
	( pMsgSummary,slSummaryLine,dIndex,pErr)
	  MesgSummaryMessage *pMsgSummary;
	  SummaryLine *slSummaryLine;
	  SIP_U32bit dIndex;
	  SipError *pErr;
#endif
{

	SummaryLine* element_in_list=SIP_NULL;

	SIPDEBUGFN("Entering " \
            "sip_mwi_insertSummaryLineAtIndexInMesgSummaryMessage\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pMsgSummary == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (slSummaryLine == SIP_NULL)
		element_in_list = SIP_NULL;
	else
	{
		element_in_list = slSummaryLine;
		HSS_LOCKEDINCREF((( SummaryLine* ) element_in_list)->dRefCount);
	}

	if( sip_listInsertAt( &(pMsgSummary->slSummaryLine), dIndex,\
	 (SIP_Pvoid) element_in_list, pErr) == SipFail)
	{
		return SipFail;
	}

	SIPDEBUGFN("Exiting " \
            "sip_mwi_insertSummaryLineAtIndexInMesgSummaryMessage\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/********************************************************************
** FUNCTION:sip_mwi_deleteSummaryLineAtIndexInMesgSummaryMessage
**
** DESCRIPTION: This function deletes Summary Line at specified index
**		In Mesg Summary Message
**
**
**********************************************************************/

SipBool sip_mwi_deleteSummaryLineAtIndexInMesgSummaryMessage
#ifdef ANSI_PROTO
	( MesgSummaryMessage *pMsgSummary, SIP_U32bit dIndex, SipError *pErr )
#else
	( pMsgSummary,dIndex,pErr)
	  MesgSummaryMessage *pMsgSummary;
	  SIP_U32bit dIndex;
	  SipError *pErr;
#endif
{

	SIPDEBUGFN("Entering " \
            "sip_mwi_deleteSummaryLineAtIndexInMesgSummaryMessage\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pMsgSummary == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listDeleteAt( &(pMsgSummary->slSummaryLine), dIndex, pErr) ==\
	 SipFail)
		return SipFail;

	SIPDEBUGFN("Exitting " \
            "sip_mwi_deleteSummaryLineAtIndexInMesgSummaryMessage\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/********************************************************************
** FUNCTION:sip_mwi_getNameValuePairCountFromMesgSummaryMessage
**
** DESCRIPTION: This function retrieves the number of NameValuePairs from an
**		Message Summary Body
**
**
**********************************************************************/

SipBool sip_mwi_getNameValuePairCountFromMesgSummaryMessage
#ifdef ANSI_PROTO
	( MesgSummaryMessage *pMsgSummary, SIP_U32bit *pIndex, SipError *pErr )
#else
	( pMsgSummary,pIndex,pErr)
	  MesgSummaryMessage *pMsgSummary;
	  SIP_U32bit *pIndex;
	  SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering " \
            "sip_mwi_getNameValuePairCountFromMesgSummaryMessage \n");
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL )
		return SipFail;

	if (( pMsgSummary == SIP_NULL) || ( pIndex == SIP_NULL ))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listSizeOf(&(pMsgSummary->slNameValue), pIndex , pErr) == SipFail )
	{
		return SipFail;
	}

	SIPDEBUGFN("Exitting " \
            "sip_mwi_getNameValuePairCountFromMesgSummaryMessage\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/*******************************************************************
** FUNCTION: sip_mwi_getNameValuePairAtIndexFromMesgSummaryMessage
**
** DESCRIPTION: This function retrieves NameValuePair at specified index
**		from Mesg Summary Message
**
*******************************************************************/

SipBool sip_mwi_getNameValuePairAtIndexFromMesgSummaryMessage
#ifdef ANSI_PROTO
	( MesgSummaryMessage *pMsgSummary, SipNameValuePair **slNameValue,\
	 SIP_U32bit dIndex, SipError *pErr )
#else
	( pMsgSummary,slNameValue,dIndex,pErr)
	  MesgSummaryMessage *pMsgSummary;
	  SipNameValuePair **slNameValue;
	  SIP_U32bit dIndex;
	  SipError *pErr;
#endif
{
	SIP_Pvoid element_from_list=SIP_NULL;
	SIPDEBUGFN("Entering " \
            "sip_mwi_getNameValuePairAtIndexFromMesgSummaryMessage\n");

#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (( pMsgSummary == SIP_NULL) || ( slNameValue == SIP_NULL ))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listGetAt(&(pMsgSummary->slNameValue), dIndex,\
		 &element_from_list, pErr) == SipFail)
		return SipFail;

	if ( element_from_list == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	HSS_LOCKEDINCREF((( SipNameValuePair* ) element_from_list)->dRefCount);
	*slNameValue = ( SipNameValuePair* ) element_from_list;

	SIPDEBUGFN("Exitting " \
            "sip_mwi_getNameValuePairAtIndexFromMesgSummaryMessage\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/*******************************************************************
** FUNCTION: sip_mwi_setNameValuePairAtIndexInMesgSummaryMessage
**
** DESCRIPTION: This function sets NameValuePair at specified index
**		in Mesg Summary Message
**
**
*******************************************************************/
SipBool sip_mwi_setNameValuePairAtIndexInMesgSummaryMessage
#ifdef ANSI_PROTO
	( MesgSummaryMessage *pMsgSummary, SipNameValuePair *slNameValue,\
	 SIP_U32bit dIndex, SipError *pErr )
#else
	( pMsgSummary,slNameValue,dIndex,pErr)
	  MesgSummaryMessage *pMsgSummary;
	  SipNameValuePair *slNameValue;
	  SIP_U32bit dIndex;
	  SipError *pErr;
#endif
{

	SipNameValuePair* element_in_list=SIP_NULL;

	SIPDEBUGFN("Entering " \
            "sip_mwi_setNameValuePairAtIndexInMesgSummaryMessage\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pMsgSummary == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (slNameValue == SIP_NULL)
		element_in_list = SIP_NULL;
	else
	{
		element_in_list = slNameValue;
		HSS_LOCKEDINCREF((( SipNameValuePair* ) element_in_list)->\
		dRefCount);
	}

	if( sip_listSetAt( &(pMsgSummary->slNameValue), dIndex,   \
		 (SIP_Pvoid) element_in_list, pErr) == SipFail)
	{
		return SipFail;
	}

	SIPDEBUGFN("Exiting " \
            "sip_mwi_setNameValuePairAtIndexInMesgSummaryMessage\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/*******************************************************************
** FUNCTION: sip_mwi_insertNameValuePairAtIndexInMesgSummaryMessage
**
** DESCRIPTION: This function inserts NameValuePair at specified index
**		in Mesg Summary Message
**
**
*******************************************************************/
SipBool sip_mwi_insertNameValuePairAtIndexInMesgSummaryMessage
#ifdef ANSI_PROTO
	( MesgSummaryMessage *pMsgSummary, SipNameValuePair *slNameValue,\
	 SIP_U32bit dIndex, SipError *pErr )
#else
	( pMsgSummary,slNameValue,dIndex,pErr)
	  MesgSummaryMessage *pMsgSummary;
	  SipNameValuePair *slNameValue;
	  SIP_U32bit dIndex;
	  SipError *pErr;
#endif
{

	SipNameValuePair* element_in_list=SIP_NULL;

	SIPDEBUGFN("Entering " \
            "sip_mwi_insertNameValuePairAtIndexInMesgSummaryMessage\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pMsgSummary == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (slNameValue == SIP_NULL)
		element_in_list = SIP_NULL;
	else
	{
		element_in_list = slNameValue;
		HSS_LOCKEDINCREF((( SipNameValuePair* ) element_in_list)->\
		dRefCount);
	}

	if( sip_listInsertAt( &(pMsgSummary->slNameValue), dIndex,   \
		 (SIP_Pvoid) element_in_list, pErr) == SipFail)
	{
		return SipFail;
	}

	SIPDEBUGFN("Exiting " \
            "sip_mwi_insertNameValuePairAtIndexInMesgSummaryMessage\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/********************************************************************
** FUNCTION:sip_mwi_deleteNameValuePairAtIndexInMesgSummaryMessage
**
** DESCRIPTION: This function deletes NameValuePair at specified index
**		in Mesg Summary Message
**
**
**********************************************************************/

SipBool sip_mwi_deleteNameValuePairAtIndexInMesgSummaryMessage
#ifdef ANSI_PROTO
	( MesgSummaryMessage *pMsgSummary, SIP_U32bit dIndex, SipError *pErr )
#else
	( pMsgSummary,dIndex,pErr)
	  MesgSummaryMessage *pMsgSummary;
	  SIP_U32bit dIndex;
	  SipError *pErr;
#endif
{

	SIPDEBUGFN("Entering " \
            "sip_mwi_deleteNameValuePairAtIndexInMesgSummaryMessage\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pMsgSummary == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listDeleteAt( &(pMsgSummary->slNameValue), dIndex, pErr) == \
	SipFail)
		return SipFail;

	SIPDEBUGFN("Exitting " \
            "sip_mwi_deleteNameValuePairAtIndexInMesgSummaryMessage\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/********************************************************************
** FUNCTION:sip_mwi_getMediaFromSummaryLine
**
** DESCRIPTION: This Function gets the media type from Summary Line
**
**
**********************************************************************/
SipBool sip_mwi_getMediaFromSummaryLine
#ifdef ANSI_PROTO
	( SummaryLine *pSummaryLine, SIP_S8bit **ppMedia, SipError *pErr)
#else
	( pSummaryLine, ppMedia, pErr )
	  SummaryLine *pSummaryLine;
	  SIP_S8bit **ppMedia;
	  SipError *pErr;
#endif
{
	SIP_S8bit *pTempMedia=SIP_NULL;
	SIPDEBUGFN("Entering sip_mwi_getMediaFromSummaryLine\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (( pSummaryLine == SIP_NULL)||(ppMedia==SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pTempMedia = pSummaryLine->pMedia;

	if( pTempMedia == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	*ppMedia = pTempMedia;

	SIPDEBUGFN("Exitting sip_mwi_getMediaFromSummaryLine\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/********************************************************************
** FUNCTION:sip_mwi_setMediaInSummaryLine
**
** DESCRIPTION: This Function sets the media type in Summary Line
**
**
**********************************************************************/

SipBool sip_mwi_setMediaInSummaryLine
#ifdef ANSI_PROTO
	( SummaryLine *pSummaryLine, SIP_S8bit *pMedia, SipError *pErr)
#else
	( pSummaryLine, pMedia, pErr )
	  SummaryLine *pSummaryLine;
	  SIP_S8bit *pMedia;
	  SipError *pErr;
#endif
{

    SIP_S8bit *pTempMedia=SIP_NULL;
	SIPDEBUGFN("Entering sip_mwi_setMediaInSummaryLine\n");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pSummaryLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( pMedia == SIP_NULL)
		pTempMedia = SIP_NULL;
	else
		pTempMedia = pMedia;

	if ( pSummaryLine->pMedia != SIP_NULL )
	{
		if ( fast_memfree(ACCESSOR_MEM_ID, pSummaryLine->pMedia,\
		 pErr) == SipFail)
			return SipFail;
	}
        pSummaryLine->pMedia = pTempMedia;
	SIPDEBUGFN("Exiting sip_mwi_setMediaInSummaryLine\n");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/********************************************************************
** FUNCTION: sip_mwi_getMesgAccountUrlFromMesgSummaryMessage
**
** DESCRIPTION: This functon retrieves the message account url from the
**		Mesg Summary Body
**
**********************************************************************/
SipBool sip_mwi_getMesgAccountUrlFromMesgSummaryMessage
#ifdef ANSI_PROTO
	(MesgSummaryMessage *pMsgSummary, SipAddrSpec **ppAddrSpec,
	 SipError *pErr)
#else
	( pMsgSummary, ppAddrSpec, pErr )
	MesgSummaryMessage *pMsgSummary;
	SipAddrSpec **ppAddrSpec;
	SipError *pErr;
#endif
{
	SipAddrSpec *pTempAddrSpec=SIP_NULL;

	SIPDEBUGFN("Entering " \
            "function sip_mwi_getMesgAccountUrlFromMesgSummaryMessage");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL)
		return SipFail;
	if(  ppAddrSpec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pMsgSummary == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
 	pTempAddrSpec = pMsgSummary->pAddrSpec ;
 	if (pTempAddrSpec == SIP_NULL)
 	{
 		*pErr = E_NO_EXIST;
 		return SipFail;
 	}

	HSS_LOCKEDINCREF(pTempAddrSpec->dRefCount);
	*ppAddrSpec = pTempAddrSpec;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting " \
            "function sip_mwi_getMesgAccountUrlFromMesgSummaryMessage");
	return SipSuccess;

}

/********************************************************************
** FUNCTION: sip_mwi_setMesgAccountUrlInMesgSummaryMessage
**
** DESCRIPTION: This functon set the message account url in the
**		Mesg Summary Body
**
**********************************************************************/
SipBool sip_mwi_setMesgAccountUrlInMesgSummaryMessage
#ifdef ANSI_PROTO
	(MesgSummaryMessage *pMsgSummary, SipAddrSpec *pAddrSpec,
	 SipError *pErr)
#else
	( pMsgSummary, pAddrSpec, pErr )
	MesgSummaryMessage *pMsgSummary;
	SipAddrSpec *pAddrSpec;
	SipError *pErr;
#endif
{
 	SIPDEBUGFN("Entering " \
            "function sip_mwi_setMesgAccountUrlInMesgSummaryMessage");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL)
		return SipFail;

	if( pMsgSummary == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
  if (pAddrSpec == SIP_NULL)
	{
		sip_freeSipAddrSpec(pMsgSummary->pAddrSpec);
		pMsgSummary->pAddrSpec=SIP_NULL ;
	}
	else
	{
		HSS_LOCKEDINCREF(pAddrSpec->dRefCount);
		sip_freeSipAddrSpec(pMsgSummary->pAddrSpec);
		pMsgSummary->pAddrSpec = pAddrSpec;
 	}

	*pErr = E_NO_ERROR;
 	SIPDEBUGFN("Exiting  " \
            "function sip_mwi_setMesgAccountUrlInMesgSummaryMessage");
	return SipSuccess;
}

#endif


