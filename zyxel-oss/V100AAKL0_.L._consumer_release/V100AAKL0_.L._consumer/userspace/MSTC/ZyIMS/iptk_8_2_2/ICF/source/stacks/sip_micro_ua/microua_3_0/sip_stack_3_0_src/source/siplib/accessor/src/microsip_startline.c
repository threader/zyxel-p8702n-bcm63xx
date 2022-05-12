/***********************************************************************
** FUNCTION:
**			Startline Accessor API functionality
************************************************************************
**
** FILENAME:
** microsip_startline.c
**
** DESCRIPTION:
**		This file contains the accessor functions for manipulating the 
**		startline of a SIP message.
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 9/09/2002	Aparna Kuppachi		-			        Initial					
** 29/01/2009   Kamal Ashraf     IPTK Rel 8.1   API 
**                                              sip_setReqLineFromString added
** 13/02/2009   Kamal Ashraf        IPTK Rel 8.1   UT Bug Fixes 
** 05-Jun-2009   Anuradha Gupta         SPR 19737       Klocworks Warning Removal
**
**	COPYRIGHT , Aricent, 2009
***********************************************************************/

#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_init.h"
#include "microsip_startline.h"
#include "microsip_header.h"
#include "microsip_hdrparsers.h"
#include "microsip_accessors.h"


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

SipBool sip_getStatusLineFromSipRespMsg
	(SipMessage *pMsg, SipStatusLine **ppStatusLine, SipError *pErr)
{
	SipRespMessage *response_msg;
	SipStatusLine *temp_line;

	SIPDEBUGFN("Entering function sip_getstatusLineFromSipRespMsg");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if ( (pMsg == SIP_NULL)||(ppStatusLine == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pMsg->dType != SipMessageResponse)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif

	response_msg = (pMsg->u).pResponse;
	if ((response_msg == SIP_NULL))
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	temp_line = response_msg->pStatusLine;
	if (temp_line == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	HSS_LOCKEDINCREF(temp_line->dRefCount);
	*ppStatusLine = temp_line;
		
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_getstatusLineFromSipRespMsg");
	return SipSuccess;
}


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

SipBool sip_setStatusLineInSipRespMsg
	(SipMessage *pMsg, SipStatusLine *pLine, SipError *pErr)
{
	SIPDEBUGFN("Entering function sip_setStatusLineInSipRespMsg");

#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pMsg == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pMsg->dType != SipMessageResponse)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	if ((pMsg->u).pResponse == SIP_NULL)
	{
		if (sip_initSipRespMessage(&((pMsg->u).pResponse), pErr) == SipFail)
			return SipFail;
	}
	if (pLine == SIP_NULL)
	{
		if (((pMsg->u).pResponse)->pStatusLine != SIP_NULL)
			sip_freeSipStatusLine(((pMsg->u).pResponse)->pStatusLine);
		((pMsg->u).pResponse)->pStatusLine = SIP_NULL;	
	}
	else
	{
		if (((pMsg->u).pResponse)->pStatusLine != SIP_NULL)
			sip_freeSipStatusLine(((pMsg->u).pResponse)->pStatusLine);
		HSS_LOCKEDINCREF(pLine->dRefCount);
		((pMsg->u).pResponse)->pStatusLine = pLine;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setStatusLineInSipRespMsg");
	return SipSuccess;
}


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

SipBool sip_getReqLineFromSipReqMsg
	(SipMessage *pMsg, SipReqLine **ppLine, SipError *pErr)
{
	SipReqMessage *request_msg;
	SipReqLine *temp_line;
	SIPDEBUGFN("Entering function sip_getReqLineFromSipReqMsg");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if ( (pMsg == SIP_NULL)||(ppLine == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pMsg->dType != SipMessageRequest)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif

	request_msg = (pMsg->u).pRequest;
	if ((request_msg == SIP_NULL))
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	temp_line = request_msg->pRequestLine;
	if (temp_line == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	HSS_LOCKEDINCREF(temp_line->dRefCount);
	*ppLine = temp_line;
		
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getReqLineFromSipReqMsg");
	return SipSuccess;
}


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

SipBool sip_setReqLineInSipReqMsg
	(SipMessage *pMsg, SipReqLine *pLine, SipError *pErr)
{
	SIPDEBUGFN("Entering function sip_setReqLineInSipReqMsg");

#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pMsg == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pMsg->dType != SipMessageRequest)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	if (pMsg->u.pRequest == SIP_NULL)
	{
		if (sip_initSipReqMessage(&((pMsg->u).pRequest), pErr) == SipFail)
				return SipFail;
	}
	
	if (pLine == SIP_NULL)
	{
		if (((pMsg->u).pRequest)->pRequestLine != SIP_NULL)
			sip_freeSipReqLine(((pMsg->u).pRequest)->pRequestLine);
		((pMsg->u).pRequest)->pRequestLine = SIP_NULL;	
	}
	else
	{
		if (((pMsg->u).pRequest)->pRequestLine != SIP_NULL)
			sip_freeSipReqLine(((pMsg->u).pRequest)->pRequestLine);
		HSS_LOCKEDINCREF(pLine->dRefCount);
		((pMsg->u).pRequest)->pRequestLine = pLine;
	}
			
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setReqLineInSipReqMsg");
	return SipSuccess;
}


/*****************************************************************
**
** FUNCTION:  sip_getAddrSpecFromReqLine
**
** DESCRIPTION: This function retrieves the dAddr-spec field from a
**		SIP Request line
**
***************************************************************/

SipBool sip_getAddrSpecFromReqLine(SipReqLine *pLine, SipAddrSpec \
	**ppRequestUri, SipError *pErr)
{

	SipHeader Hdr;

	SIPDEBUGFN("Entering function sip_getAddrSpecFromReqLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since Reqline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_getAddrSpecFromCommonHdr(&Hdr,ppRequestUri,pErr))
	{
		return SipFail;
	}
	SIPDEBUGFN("Exiting function sip_getAddrSpecFromReqLine");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/*****************************************************************
**
** FUNCTION:  sip_setAddrSpecInReqLine
**
** DESCRIPTION: This function sets the dAddr-spec field in a SIP
**		Request line
**
***************************************************************/
SipBool sip_setAddrSpecInReqLine
	(SipReqLine *pLine, SipAddrSpec *pRequestUri, SipError *pErr)
{
	SipHeader Hdr;

	SIPDEBUGFN("Entering function sip_setAddrSpecInReqLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since Reqline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_setAddrSpecInCommonHdr(&Hdr,pRequestUri,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setAddrSpecInReqLine");
	return SipSuccess;
}

/***********************************************************************
** Function: sip_setMethodInReqLine
** Description:sets the Method in the Sip Status Line
** Parameters:
**			pLine(IN/OUT) 		- Sip Status Line
**			method(IN)			- The method to set
**			pErr(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
SipBool sip_setMethodInReqLine
	(SipReqLine *pLine, SIP_S8bit *pMethod, SipError *pErr)
{
	SipHeader Hdr;

 	SIPDEBUGFN("Entering function sip_setMethodInReqLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since Reqline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_setStr1InCommonHdr(&Hdr,pMethod,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setMethodInReqLine");
        return SipSuccess;
}


SipBool sip_getMethodFromReqLine
	(SipReqLine *pLine, SIP_S8bit **ppMethod, SipError *pErr)
{
	SipHeader Hdr;

 	SIPDEBUGFN("Entering function sip_getMethodFromReqLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since Reqline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_getStr1FromCommonHdr(&Hdr,ppMethod,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getMethodFromReqLine");
 	return SipSuccess;
}


/***********************************************************************
** Function: sip_getVersionFromReqLine
** Description:gets the Version fromthe Sip Request Line
** Parameters:
**			pLine(IN) 			- Sip RequestLine
**			version(OUT)			- Version to retrieve
**			pErr(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
SipBool sip_getVersionFromReqLine
	(SipReqLine *pLine, SIP_S8bit **ppVersion, SipError *pErr)
{
	SipHeader Hdr;

 	SIPDEBUGFN("Entering function sip_getVersioFromReqLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since Reqline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_getStr2FromCommonHdr(&Hdr,ppVersion,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getVersioFromReqLine");
 	return SipSuccess;
}

/***********************************************************************
** Function: sip_setVersionInReqLine
** Description:gets the Version in the Sip Request Line
** Parameters:
**			pLine(IN/OUT) 			- Sip RequestLine
**			version(IN)			- Version to set
**			pErr(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
SipBool sip_setVersionInReqLine
	(SipReqLine *pLine, SIP_S8bit *pVersion, SipError *pErr)
{
	SipHeader Hdr;

 	SIPDEBUGFN("Entering function sip_setVersionInReqLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since Reqline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_setStr2InCommonHdr(&Hdr,pVersion,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setVersionInReqLine");
        return SipSuccess;
}
/***********************************************************************
** Function: sip_getVersionFromStatusLine
** Description:gets the Version field from the Sip Status Line
** Parameters:
**			pLine(IN) 			- Sip Status Line
**			version(OUT)		- The version to retrieve
**			pErr(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
SipBool sip_getVersionFromStatusLine
	(SipStatusLine *pLine, SIP_S8bit **ppVersion, SipError *pErr)
{
	SipHeader Hdr;

 	SIPDEBUGFN("Entering function sip_getVersionFromStatusLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since statusline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_getStr1FromCommonHdr(&Hdr,ppVersion,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getVersionFromStatusLine");
 	return SipSuccess;
}

/***********************************************************************
** Function: sip_setVersionInStatusLine
** Description:sets the Version field in the Sip Status Line
** Parameters:
**			pLine(IN/OUT) 			- Sip Status Line
**			version(IN)			- The version to set
**			pErr(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
SipBool sip_setVersionInStatusLine
	(SipStatusLine *pLine, SIP_S8bit *pVersion, SipError *pErr)
{
	SipHeader Hdr;

 	SIPDEBUGFN("Entering function sip_setVersionInStatusLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since statusline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_setStr1InCommonHdr(&Hdr,pVersion,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setVersionInStatusLine");
        return SipSuccess;
}

/***********************************************************************
** Function: sip_getStatusCodeNumFromStatusLine
** Description:gets the Status code number from the Sip Status Line
** Parameters:
**			pLine(IN) 			- Sip Status Line
**			code_num(OUT)		- The Status code number to retrieve
**			pErr(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
SipBool sip_getStatusCodeNumFromStatusLine
	(SipStatusLine *pLine, SIP_U32bit *pCodeNum, SipError *pErr)
{
	SipHeader Hdr;

 	SIPDEBUGFN("Entering function sip_getStatusCodeNumFromStatusLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since statusline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_getdIntVar1FromCommonHdr(&Hdr,pCodeNum,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getStatusCodeNumFromStatusLine");
	return SipSuccess;
}
/***********************************************************************
** Function: sip_setStatusCodeNumInStatusLine
** Description:sets the Status code number in the Sip Status Line
** Parameters:
**			pLine(IN/OUT) 			- Sip Status Line
**			code_num(IN)		- The Status code number to set
**			pErr(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/

SipBool sip_setStatusCodeNumInStatusLine
	(SipStatusLine *pLine, SIP_U32bit dCodeNum, SipError *pErr)
{
	SipHeader Hdr;

 	SIPDEBUGFN("Entering function sip_setStatusCodeNumInStatusLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since statusline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_setdIntVar1InCommonHdr(&Hdr,dCodeNum,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setStatusCodeNumInStatusLine");
	return SipSuccess;
}


/*****************************************************************
**
** FUNCTION:  sip_getReasonFromStatusLine
**
** DESCRIPTION: This function retrieves the pReason from a SIP status
**		pLine
**
***************************************************************/

SipBool sip_getReasonFromStatusLine
	(SipStatusLine *pLine, SIP_S8bit **ppReason, SipError *pErr)
{
	SipHeader Hdr;

 	SIPDEBUGFN("Entering function sip_getReasonFromStatusLine");
#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since statusline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_getStr2FromCommonHdr(&Hdr,ppReason,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getReasonFromStatusLine");
 	return SipSuccess;
}


/***********************************************************************
** Function: sip_setReasonInStatusLine
** Description:sets the Reason in the Sip Status Line
** Parameters:
**			pLine(IN/OUT) 			- Sip Status Line
**			reason(IN)			- The reason to set
**			pErr(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
SipBool sip_setReasonInStatusLine
	(SipStatusLine *pLine, SIP_S8bit *pReason, SipError *pErr)
{
	SipHeader Hdr;

#ifndef SIP_NO_CHECK
	if(pLine == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
 	SIPDEBUGFN("Entering function sip_setReasonInStatusLine");
	/* Fill a dummy SipHeader and pass it to the common funtion */
	Hdr.dType = SipHdrTypeAny; /* Since statusline is does not come under headers */
	Hdr.pHeader = pLine;

	if(SipFail == sip_setStr2InCommonHdr(&Hdr,pReason,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setReasonInStatusLine");
        return SipSuccess;
}


/*API added for IPTK Rel 8.1(UATK Enhancement)*/
/********************************************************************
**
** FUNCTION:    sip_setReqLineFromString
**
** DESCRIPTION: This function sets the Request Line in a SipMessage
**				 of dType SipMessageRequest.
**
*********************************************************************/
SipBool sip_setReqLineFromString
#ifdef ANSI_PROTO
    (SipMessage *pSipMsg, SIP_S8bit *line, SipError *pErr)
#else
    (pSipMsg, line, pErr)
     SipMessage *pSipMsg;
     SIP_S8bit *line;
     SipError *pErr;
#endif
{
    SipMessage *pSipMessage = SIP_NULL ;
    en_SipMessageType       dReqRespType = SipMessageRequest;
    SIP_S8bit *pBeginIndex = SIP_NULL ;
    SIP_S8bit *pEndIndex   = SIP_NULL ;
    SIPDEBUG("Entering sip_setReqLineFromString");

#ifndef SIP_NO_CHECK
    if (SIP_NULL == pErr)
	    return SipFail;

    if ((SIP_NULL == pSipMsg))
    {
        *pErr = E_INV_PARAM; /*set the value of pErr as invalid parameter*/ 
        return SipFail;
    }

    if(SIP_NULL == line)
    {
        *pErr = E_INV_PARAM; /*set the value of pErr as invalid parameter*/
        return SipFail;
    }
#endif

    /*Get message type from SIP message*/
    if(SipFail == sip_getMessageType(pSipMsg,&dReqRespType,pErr))
    {
        return SipFail;
    }

    /*Initilised a temporary sip message.With the help of this
      temporary sip meaasge we will parse the input string which need to
      set as request uri in sip message*/
    /*Initilisation of temporary sip message can be of two type 
      depending  upon whether the input sip message is of request type
      or response type*/    

    if (SipMessageRequest == dReqRespType)
    { 
        if (SipFail == sip_initSipMessage (&pSipMessage,SipMessageRequest,pErr))
        {
            return SipFail;
        }
    }
    if (SipMessageResponse == dReqRespType)
    { 
        if (SipFail == sip_initSipMessage (&pSipMessage,SipMessageResponse,pErr))
        {
            return SipFail;
        }
    }

    if(SIP_NULL == pSipMessage)
    {
	   *pErr = E_NO_MEM;	
       return SipFail;
    }

    if(SIP_NULL == (pSipMessage->u).pRequest->pRequestLine)
    {
        /*Initialized the request line in SIP message*/
        /* klocworks : in case init request line fails that means it is
         * a no memory error
         */ 
        if(SipFail == sip_initSipReqLine(&(pSipMessage->u).pRequest->pRequestLine,pErr))
		{
			*pErr = E_NO_MEM;
			/*Free the temporary SIP message*/            
			sip_freeSipMessage(pSipMessage); 
			return SipFail;
		}
    }

    pBeginIndex = line ;
    pEndIndex = pBeginIndex + sip_strlen(line) - 1 ;

    /*Parse the request line*/ 
    if (SipFail == sip_parseRequestLine(pBeginIndex,
        pEndIndex, pSipMessage->u.pRequest->pRequestLine, pErr))
    {
        /*Free the temporary SIP message*/            
        sip_freeSipMessage(pSipMessage); 
        return SipFail;
    }
    else
    {
        /*Input string is successfully parsed in temporary sip message.Now free
          the temporary sip message and set the same string in pSipMsg.Before that
          free the already present request line*/

        /*Free the temporary SIP message*/            
        sip_freeSipMessage(pSipMessage); 

        sip_freeSipReqLine((pSipMsg->u).pRequest->pRequestLine);

        /*Initialized the request line in SIP message*/
        if(SipFail == sip_initSipReqLine(&(pSipMsg->u).pRequest->pRequestLine,pErr))
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
        /*set the request line in pSipMsg*/ 
        else if (SipFail == sip_parseRequestLine(pBeginIndex,
                    pEndIndex, pSipMsg->u.pRequest->pRequestLine, pErr))
		{
			*pErr = E_INV_PARAM;
			return SipFail;
		}
    }

    SIPDEBUG("Exiting sip_setReqLineFromString");
    *pErr = E_NO_ERROR; 
    return SipSuccess;

}



