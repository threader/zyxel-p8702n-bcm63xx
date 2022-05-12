/**************************************************************************** 
 ** FUNCTION:
 **	 				This file has all the debug and error related functions.
 ****************************************************************************
 **
 ** FILENAME:		sdf_debug.c
 **
 ** DESCRIPTION: 	All UA Toolkit APIs to set error levels, print errors 
 **					and debug statements are defined in this file.
 **
 **
 ** DATE				NAME			REFERENCE		REASON
 ** ----				----			--------		------
 ** 29/09/01			Subhash							Modified ctascii 
 **														related functions to
 **														report multiple calls
 ** 3/03/01				Rajasri,Seshashayi				Added Error functions
 **
 ** 24/10/00			Subhash							Creation
 ** 22/04/03            Anil Kumar         ---          Portlayer related.
 ** 08/06/09            Anuradha Gupta    SPR 19737     Klocworks fix
 **
 ****************************************************************************
 **				COPYRIGHT , Aricent, 2009
 ****************************************************************************/

#include "ALheader.h"
#include "sdf_common.h"
#include "sdf_struct.h"
#ifndef SDF_LINT
#include "sdf_sdpstruct.h"
#endif
#include "sdf_internal.h"
#include "sdf_debug.h"
#include "sdf_portlayer.h"
#include "sdf.h"

/*
 * The TCP socket used to communicate with the ctascii utility
 */
Sdf_ty_socketId dCallFlowSockfd;

/*
 * The error level set in the toolkit
 */
Sdf_ty_uaErrorLevel sdf_sc_dErrorLevel;

/* 
 * The error strings to be displayed for the error codes
 */
#ifdef SDF_ERROR
Sdf_ty_s8bit sdf_sc_dErrorStrings[Sdf_co_numErrors][Sdf_mc_errLength];
#endif

/* klocworks: ABR errors on STRCAT , checks have been added for string length
 */

/***************************************************************************
 ** FUNCTION: 		sdf_ivk_uaInitCallFlow 
 **
 ** DESCRIPTION: 	For tracing the SipMessage flow. This initializes
 **					the ctascii utility to show the SipMessage flow 
 **					from/to the UA
 **
 ***************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitCallFlow
#ifdef ANSI_PROTO
(Sdf_ty_s8bit *pIp,Sdf_ty_u32bit dPort, Sdf_ty_s8bit *pUaOne,\
	Sdf_ty_s8bit *pUaTwo, Sdf_st_error *pErr)
#else
 (pIp,dPort,pUaOne,pUaTwo,pErr)
  Sdf_ty_s8bit *pIp;
  Sdf_ty_u32bit dPort;
  Sdf_ty_s8bit *pUaOne;
  Sdf_ty_s8bit *pUaTwo;
  Sdf_st_error *pErr;
#endif
{
#ifdef SDF_CALLFLOW
#ifdef SDF_IPV6
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	struct addrinfo *pAddrInfo;
	struct addrinfo	Hints;
	Sdf_ty_s32bit	RetVal;
	Sdf_ty_s8bit  lPort[Sdf_co_smallBufferSize];
#endif
	Sdf_ty_bool			dIsIpv6=Sdf_co_false;
#endif
	
	struct sockaddr_in dServaddr;
	Sdf_st_socket      	dSocket;
	Sdf_ty_s8bit       	dTempStr[Sdf_co_smallBufferSize];
	Sdf_ty_s32bit 		dDomain = AF_INET;

		sdf_fn_debug((Sdf_ty_s8bit*)"Entering sdf_ivk_uaInitCallFlow");

#ifdef SDF_IPV6
		if(Sdf_mc_strstr(pIp,":") !=Sdf_co_null)
		{
			dDomain=AF_INET6;
			dIsIpv6=Sdf_co_true;
		}
		else
			dIsIpv6=Sdf_co_false;
#endif
		if ((dCallFlowSockfd = sdf_fn_uaSocketCreate(dDomain, \
			SOCK_STREAM, 0)) <= 0)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_socketOpenError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCallFlow( ):  "
				"Could not open TCP socket", pErr) ;
#endif
			pErr->errCode=Sdf_en_socketOpenError;
			return Sdf_co_fail;
		}

		dSocket.dSockfd = dCallFlowSockfd;
		dSocket.dProtocol = Sdf_en_protoTcp;
		
#ifdef SDF_IPV6			
		if(dIsIpv6 == Sdf_co_true)
		{
			memset(&Hints, 0, sizeof(Hints));
			Hints.ai_flags = AI_NUMERICHOST;
			Hints.ai_family =PF_INET6;
			Hints.ai_socktype = SOCK_STREAM;
			Hints.ai_protocol = IPPROTO_TCP;
			(void)sdf_fn_uaSprintf(lPort, "%hu", dPort);
			RetVal = getaddrinfo(pIp, lPort, &Hints, &pAddrInfo);
			if(RetVal != 0)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError, \
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitCallFlow"
				"(): Failed to resolve IP address into numeric form", pErr);
#endif
				pErr->errCode=Sdf_en_systemError;
				(void)sdf_fn_uaCloseSocket(&dSocket);
				return Sdf_co_fail;
			}
			if (sdf_fn_uaConnectSocket(&dSocket, pAddrInfo->ai_addr, \
				pAddrInfo->ai_addrlen,Sdf_co_null ) == Sdf_co_fail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_tcpConnectError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaInitCallFlow( ): "
					"TCP connect error", pErr);
#endif			
				(void)sdf_fn_uaCloseSocket(&dSocket);
				pErr->errCode=Sdf_en_tcpConnectError;
				freeaddrinfo(pAddrInfo);
				return Sdf_co_fail;
			}
			freeaddrinfo(pAddrInfo);
	}
	else
	{
#endif
		memset((void*)&dServaddr, 0, sizeof(dServaddr));
		dServaddr.sin_family = AF_INET;
		dServaddr.sin_port = Sdf_mc_htons((Sdf_ty_u16bit)dPort);
		dServaddr.sin_addr.s_addr =inet_addr(pIp);

		if (sdf_fn_uaConnectSocket(&dSocket, (Sdf_ty_pvoid)&dServaddr,
			sizeof(dServaddr), Sdf_co_null) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_tcpConnectError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCallFlow( ): "
				"TCP connect error", pErr);
#endif			
			pErr->errCode=Sdf_en_tcpConnectError;
			(void)sdf_fn_uaCloseSocket(&dSocket);
			return Sdf_co_fail;
		}
#ifdef SDF_IPV6
	}
#endif

	/*
	 * Send a message to ctascii to initiate the display with two
	 * lines indicating the local and remote UA
	 */
	if((pUaTwo!=Sdf_co_null)&&(pUaOne!=Sdf_co_null))
		(void)sdf_fn_uaSprintf(dTempStr,"init 2 %s %s \n",pUaOne,pUaTwo);
	
	sdf_ivk_uaDisplayCallFlow(dTempStr);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"Exitting sdf_ivk_uaInitCallFlow");
#else
	/*
	 * To remove compiler warning when SDF_CALLFLOW is disable
	 */
	(void)pIp;
  	(void) dPort;
  	(void)pUaOne;
  	(void)pUaTwo;
	(void)pErr;
#endif	
	return Sdf_co_success;
}


/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaUpdateCallFlow
 **
 ** DESCRIPTION: 	For tracing the SipMessage flow. This uses the ctascii
 **					utility to show the SIP message flow from/to the UA.
 **
 ***************************************************************************/
void sdf_fn_uaUpdateCallFlow 
#ifdef ANSI_PROTO
	(SipMessage *pSipMsg, Sdf_ty_bool dMsgDirection)
#else
	(pSipMsg, dMsgDirection)
	SipMessage *pSipMsg;
	Sdf_ty_bool dMsgDirection;
#endif
{
#ifdef SDF_CALLFLOW
	Sdf_ty_s8bit *pMethod = Sdf_co_null, *pCallId = Sdf_co_null;
	Sdf_ty_s8bit dCtasciiMessage[Sdf_co_smallBufferSize],
				 dTemp[Sdf_co_characterBufferSize];
	Sdf_st_error dErr;
	Sdf_ty_u16bit dCode=0;
	SipHeader dHeader;

	if (pSipMsg == Sdf_co_null)
	{
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaUpdateCallFlow( ): Invalid SipMessage parameter"); 
		return;
	}

	if ((sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, &pMethod, &dErr)) \
		== Sdf_co_fail)
	{
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaUpdateCallFlow( ): Failed to get method from SipMessage"); 
		return;
	}
	if (pSipMsg->dType == SipMessageResponse)
	{
		if ((sdf_ivk_uaGetRespCodeFromSipMessage(pSipMsg, &dCode, &dErr)) \
			== Sdf_co_fail)
		{
			sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
				"sdf_fn_uaUpdateCallFlow( ): Failed to get response code"
				"from SipMessage"); 
			return;
		}
		(void)sdf_fn_uaSprintf (dTemp, "%d response to %s ", dCode, pMethod);
	}
	else
	{
		(void)sdf_fn_uaSprintf(dTemp, "%s ", pMethod);
	}

	/*
	 * Get the call-id from the message
	 */
	if (sip_getHeader(pSipMsg, SipHdrTypeCallId, &dHeader, \
		(SipError *)&(dErr.stkErrCode)) == SipFail)
	{
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaUpdateCallFlow( ): Failed to get Call-Id header"
			"from SipMessage"); 
		return;
	}
	if (sip_getValueFromCallIdHdr(&dHeader, &pCallId,\
		(SipError*) &(dErr.stkErrCode)) == SipFail)
	{
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaUpdateCallFlow( ): Failed to get Call-Id from"
			"Call-ID header"); 
		return;
	}
    /*Codenomicon fixes merge: CSR_1-6714693*/
#if 0
	Sdf_mc_strcat(dTemp, "(i:");
	Sdf_mc_strcat(dTemp, pCallId);
	Sdf_mc_strcat(dTemp, ")");
#endif
    Sdf_mc_strcat(dTemp, "(i:");
	Sdf_mc_strncat(dTemp,pCallId,Sdf_co_characterBufferSize-Sdf_mc_strlen(dTemp)-2);
	Sdf_mc_strncat(dTemp, ")",1);
	/* Free the local reference */
	sip_freeSipHeader(&dHeader);

	/* 
	 * Check if the message was received by the UA Toolkit. Messages 
	 * received are shown in blue at the moment
	 */
	if (dMsgDirection == Sdf_co_true)
	{
		(void)sdf_fn_uaSprintf(dCtasciiMessage, "mesg blue 1 0 %s \n", dTemp);
		sdf_ivk_uaDisplayCallFlow(dCtasciiMessage);
	}
	else
	{
		(void)sdf_fn_uaSprintf(dCtasciiMessage, "mesg red 0 1 %s \n", dTemp);
		sdf_ivk_uaDisplayCallFlow(dCtasciiMessage);
	}
	return;
#else
	(void)dMsgDirection;
	(void)pSipMsg;
	(void)dMsgDirection;
#endif	
}


/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaDisplayCallFlow
 **
 ** DESCRIPTION: For tracing the SipMessage flow. This interprets  
 **				 the input buffer to a graphic display useing the 
 **				 ctascii utility 
 **
 ****************************************************************************/
void sdf_ivk_uaDisplayCallFlow
#ifdef ANSI_PROTO
(Sdf_ty_s8bit *buf)
#else
 (buf )
  Sdf_ty_s8bit *buf;
#endif
{
#ifdef SDF_CALLFLOW    
if (dCallFlowSockfd > 0)                                 
	{	                                                 
     (void)Sdf_mc_displayCallFlow(dCallFlowSockfd,buf);      
	   Sdf_mc_uaTestCancel                
	   write (dCallFlowSockfd, buf, Sdf_mc_strlen(buf));
	   	 Sdf_mc_uaTestCancel                
	}	                                                 
#else                                                    
    (void)buf;                                          
#endif 

}


#ifdef SDF_ERROR 


/****************************************************************************
 ** FUNCTION: 		sdf_fn_setError
 **
 ** DESCRIPTION:	This functions is used to set the error code and 
 **					dump the error message on stderr.
 **
 ****************************************************************************/
void sdf_fn_setError
#ifdef ANSI_PROTO
	(Sdf_ty_uaErrorLevel dErrorLevel,
	Sdf_ty_errorCode dErrorCode,
	const Sdf_ty_s8bit *pStr,
	Sdf_st_error *pErr)
#else
	( dErrorLevel,dErrorCode,pStr,pErr )
	Sdf_ty_uaErrorLevel dErrorLevel;
	Sdf_ty_errorCode dErrorCode;
	const Sdf_ty_s8bit *pStr;
	Sdf_st_error *pErr;
#endif
{
	/* Can put a message to dump saying that error variable has null value */
#ifndef SDF_ERROR
	Sdf_ty_uaErrorLevel dDummyLevel;
	dDummyLevel = dErrorLevel;
#endif
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_fn_setError( ): Invalid parameter passed to function");
		return;
	}	
#endif

	if (dErrorCode < 2000 || dErrorCode >= Sdf_en_LastUatkError )
	{
		pErr->errCode = Sdf_en_invalidTypeError;
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_fn_setError( ): Invalid error type");
		return;	
	}
	pErr->errCode = dErrorCode;

	/*Based on the error code fill the fixed string*/
	Sdf_mc_strcpy(pErr->ErrMsg,(Sdf_ty_s8bit*)sdf_sc_dErrorStrings\
		[dErrorCode-2000]);

	/*Based on the Level Passed add Minor, Major Critical Prefix*/
	switch(dErrorLevel)
	{
		case Sdf_en_minorErrors:
			{
				if(Sdf_mc_errLength > 
						(Sdf_mc_strlen(pErr->ErrMsg) + Sdf_mc_strlen("\nMINOR:")))
				            Sdf_mc_strcat(pErr->ErrMsg,"\nMINOR:");
				break;
			}
		case Sdf_en_majorErrors:
			{
				if(Sdf_mc_errLength > 
						(Sdf_mc_strlen(pErr->ErrMsg) + Sdf_mc_strlen("\nMAJOR:")))
				            Sdf_mc_strcat(pErr->ErrMsg,"\nMAJOR:");
				break;
			}
		case Sdf_en_criticalErrors:
			{
				if(Sdf_mc_errLength > 
						(Sdf_mc_strlen(pErr->ErrMsg) + Sdf_mc_strlen("\nCRITICAL:")))
				             Sdf_mc_strcat(pErr->ErrMsg,"\nCRITICAL:");
				break;
			}
		default:
			break;
	}
		
	if (pStr != Sdf_co_null)
		if(Sdf_mc_errLength > 
						(Sdf_mc_strlen(pErr->ErrMsg) + Sdf_mc_strlen(pStr)))
		          Sdf_mc_strcat(pErr->ErrMsg,pStr);

	if(Sdf_mc_errLength > (Sdf_mc_strlen(pErr->ErrMsg) + 1))
	    Sdf_mc_strcat(pErr->ErrMsg, "\n");	

#ifdef SDF_ERROR
	if (sdf_sc_dErrorLevel == Sdf_en_noErrors)
	{
		pErr->errCode = Sdf_en_noUaError;
		return;
	}
	if (sdf_sc_dErrorLevel & dErrorLevel ) 
	{
		sdf_fn_dumpError((Sdf_ty_s8bit*)pErr->ErrMsg);
	}
#endif
}

/***************************************************************************
 ** FUNCTION: 		sdf_ivk_setErrorLevel
 **
 ** DESCRIPTION:  	This functions is used to set the error level
 **					for the UA Toolkit.
 **
 ***************************************************************************/
Sdf_ty_retVal sdf_ivk_setErrorLevel
#ifdef ANSI_PROTO
	(Sdf_ty_uaErrorLevel dErrLevel,
	Sdf_st_error *pErr)
#else
	(dErrLevel,pErr)
	Sdf_ty_uaErrorLevel dErrLevel;
	Sdf_st_error *pErr;
#endif
{
	sdf_sc_dErrorLevel = dErrLevel;
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/***************************************************************************
 ** FUNCTION: 		sdf_ivk_getErrorLevel
 **
 ** DESCRIPTION:  	This functions is used to get the currently configured
 **					error level	of the UA Toolkit.
 **
 ***************************************************************************/
Sdf_ty_retVal sdf_ivk_getErrorLevel
#ifdef ANSI_PROTO
	(Sdf_ty_uaErrorLevel *pErrLevel,
	Sdf_st_error *pErr)
#else
	(pErrLevel,pErr)
	Sdf_ty_uaErrorLevel *pErrLevel;
	Sdf_st_error *pErr;
#endif
{
	*pErrLevel = sdf_sc_dErrorLevel;
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/***************************************************************************
 ** FUNCTION: 		sdf_fn_initErrorStrings
 **
 ** DESCRIPTION:  	This functions is used to set the Error Strings
 **					corresponding to the error Codes in the UA Toolkit.
 **
 ***************************************************************************/
void sdf_fn_initErrorStrings (void)
{
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_invalidHeaderError - Sdf_error_base], \
		"SDF_ERROR - Invalid header");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_invalidIndexError - Sdf_error_base], \
		"SDF_ERROR - Invalid index");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_noMemoryError - Sdf_error_base], \
		"SDF_ERROR - Out of memory");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_invalidTypeError - Sdf_error_base], \
		"SDF_ERROR - Invalid type");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_stkInvalidTraceLevel - Sdf_error_base], \
		"SDF_ERROR - Invalid trace level (stack)");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_stkInvalidTraceType - Sdf_error_base], \
		"SDF_ERROR - Invalid trace type (stack)");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_stkInvalidErrorLevel - Sdf_error_base], \
		"SDF_ERROR - Invalid error level (stack)");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_stkInvalidStatsType - Sdf_error_base], \
		"SDF_ERROR - Invalid statistics type (stack)");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_stkTraceDisabled - Sdf_error_base], \
		"SDF_ERROR - Trace disabled (stack)");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_stkErrorDisabled - Sdf_error_base], \
		"SDF_ERROR - Error disabled (stack)");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_stkStatsDisabled - Sdf_error_base], \
		"SDF_ERROR - Statistics disabled (stack)");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_duplicateEntry - Sdf_error_base], \
		"SDF_ERROR - Duplicate entry");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_noExistError - Sdf_error_base], \
		"SDF_ERROR - Does not exist");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_systemError - Sdf_error_base], \
		"SDF_ERROR - System error");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_memoryError - Sdf_error_base], \
		"SDF_ERROR - Memory error");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_invalidParamError - Sdf_error_base], \
		"SDF_ERROR - Invalid parameter error");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_noUaError - Sdf_error_base], \
		"SDF_ERROR - No error");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_parserError - Sdf_error_base], \
		"SDF_ERROR - Parser error");

	/* Timer errors */
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_timerDuplicateError - Sdf_error_base], \
		"SDF_ERROR - Timer duplicate");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_timerFull - Sdf_error_base], \
		"SDF_ERROR - Timer full");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_incomplete - Sdf_error_base], \
		"SDF_ERROR - The info is incomplete");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_sendFailed - Sdf_error_base], \
		"SDF_ERROR - Send Failed");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_timeOut - Sdf_error_base], \
		"SDF_ERROR - Time Out");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_hashFail - Sdf_error_base], \
		"SDF_ERROR - Hash Failure happend");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_recvFail - Sdf_error_base], \
		"SDF_ERROR - Receive Failed");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_invRange - Sdf_error_base], \
		"SDF_ERROR - Invalid Range");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_txnNoExist - Sdf_error_base], \
		"SDF_ERROR - No SIP Transaction Exists");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_txnExists - Sdf_error_base], \
		"SDF_ERROR - SIP Transaction Already Exists");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_txnInvState - Sdf_error_base], \
		"SDF_ERROR - Invalid SIP Transaction State");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_txnInvMessage - Sdf_error_base], \
		"SDF_ERROR - Invalid SIP Transaction Message");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_bufOverflow - Sdf_error_base], \
		"SDF_ERROR - Buffer overflow");


	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_timerDoesNotExist - Sdf_error_base], \
		"SDF_ERROR - Timer does not exist");

	/* Errors for possible incomplete messages */
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_maybeIncomplete - Sdf_error_base], \
		"SDF_ERROR - Maybe incomplete SIP Message");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_notImplemented - Sdf_error_base], \
		"SDF_ERROR - Not Implemented");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_versionNotSupported - Sdf_error_base], \
		"SDF_ERROR - Version Not Supported");


	/* Network Errors */
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_socketBindError - Sdf_error_base], \
		"SDF_ERROR - Failed to bind socket");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_socketOpenError - Sdf_error_base], \
		"SDF_ERROR - Failed to create socket");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_tcpConnectError - Sdf_error_base], \
		"SDF_ERROR - Failed to open socket connection");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_destinationInfoError - Sdf_error_base], \
		"SDF_ERROR - Failed to resolve destination information");

	/* Configuration related Errors */
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_configDataError - Sdf_error_base], \
		"SDF_ERROR - Configuration Data");

	/* UA Toolkit structure access Related errors */
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_callObjectAccessError - Sdf_error_base], \
		"SDF_ERROR - Error while accessing call object member");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_transpInfoAccessError - Sdf_error_base], \
		"SDF_ERROR - Error while accessing transport info member");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_callInfoAccessError - Sdf_error_base], \
		"SDF_ERROR - Error while accessing call info member");
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_transactionAccessError - Sdf_error_base], \
		"SDF_ERROR - Error while accessing transaction member");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_nameInfoAccessError - Sdf_error_base], \
		"SDF_ERROR - Error while accessing name info member");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_commonInfoAccessError - Sdf_error_base], \
		"SDF_ERROR - Error while accessing common info member");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_initDataAccessError - Sdf_error_base], \
		"SDF_ERROR - Error while accessing InitData member");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_appDataAccessError - Sdf_error_base], \
		"SDF_ERROR - Error while accessing Application Data");
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_overlapTransInfoAccessError - Sdf_error_base], \
		"SDF_ERROR - Error while accessing overlap transaction info member");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_headerListAccessError - Sdf_error_base], \
		"SDF_ERROR - Error while accessing HeaderList member");

	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_mediaHandlingAccessError-Sdf_error_base],\
		"SDF_ERROR - Error while Handling Media related info");

	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_sessionParamAccessError-Sdf_error_base],\
		"SDF_ERROR - Error while Accessing Session Params");

	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_noCallExists - Sdf_error_base],\
		"SDF_ERROR - No Call Exists");

	/* Miscellaneous errors */
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_noMatchingMediaError - Sdf_error_base], \
		"SDF_ERROR - Could not find matching media");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_threadCreationError - Sdf_error_base], \
		"SDF_ERROR - Failed to create thread");

	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_headerManipulationError - Sdf_error_base], \
		"SDF_ERROR - Failed to manipulate header");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_userNotFoundError - Sdf_error_base], \
		"SDF_ERROR - Invalid Request-URI");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_sipstackInitError - Sdf_error_base], \
		"SDF_ERROR - Failed to initialize stack");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_unknownMethodError - Sdf_error_base], \
		"SDF_ERROR - Unknown method");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_badExtensionsError - Sdf_error_base], \
		"SDF_ERROR - Bad extension");
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_extraHeaderAccessError - Sdf_error_base], \
		"SDF_ERROR - Failed to access extra headers specified in profile");
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_retransKeyError - Sdf_error_base], \
		"SDF_ERROR - Failed to create remote retransmission key");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_attributeConflictError - Sdf_error_base], \
		"SDF_ERROR - Attribute conflicts with an already existing attribute");
	Sdf_mc_strcpy(sdf_sc_dErrorStrings[Sdf_en_tagMismatchError-Sdf_error_base],\
		"SDF_ERROR - Invalid To or From Tag Present in the header");
	Sdf_mc_strcpy(sdf_sc_dErrorStrings[Sdf_en_forkedRespError-Sdf_error_base],\
		"SDF_ERROR - Forked Response Received over the Main dialog");
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_featureNotImplemented - Sdf_error_base], \
		"SDF_ERROR - This feature not implemented");
#ifdef SDF_TLS
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_noSipSUriInContact - Sdf_error_base], \
		"SDF_ERROR - There is no contact with required scheme");
#endif
		/* Statistics, Trace and Error related errors */
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_invalidStatsType - Sdf_error_base], \
		"SDF_ERROR - Invalid statistics type");
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_invalidErrorLevel - Sdf_error_base], \
		"SDF_ERROR - Invalid error level");	
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_invalidTraceType - Sdf_error_base], \
		"SDF_ERROR - Invalid trace type");
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_invalidTraceLevel - Sdf_error_base], \
		"SDF_ERROR - Invalid trace level");
	Sdf_mc_strcpy(sdf_sc_dErrorStrings[Sdf_en_TraceDisabled - Sdf_error_base], \
		"SDF_ERROR - Trace disabled");

		/* Call state and Callback related errors */
	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_callStateError - Sdf_error_base], \
		"SDF_ERROR - Failed to change call state");
	Sdf_mc_strcpy(sdf_sc_dErrorStrings[Sdf_en_callBackError - Sdf_error_base], \
		"SDF_ERROR - Error occured in Callback");

		/* Request/Response/messagebody  errors*/
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_formRequestError - Sdf_error_base], \
		"SDF_ERROR - Failed to Form Request");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_formResponseError - Sdf_error_base], \
		"SDF_ERROR - Failed to Form Response");
	Sdf_mc_strcpy(sdf_sc_dErrorStrings[Sdf_en_sdpError - Sdf_error_base], \
		"SDF_ERROR - Error in SDP Manipulation");
	Sdf_mc_strcpy(sdf_sc_dErrorStrings[Sdf_en_msgBodyError - Sdf_error_base], \
		"SDF_ERROR - Error in message body manipulation");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_messageBodyNotAllowed - Sdf_error_base], \
		"SDF_ERROR - Message Body is not allowed in this Message");

	Sdf_mc_strcpy( \
		sdf_sc_dErrorStrings[Sdf_en_schemeNotAllowed - Sdf_error_base], \
		"SDF_ERROR - The Scheme is not allowd");

	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_tagGenerationError - Sdf_error_base], \
		"SDF_ERROR - Error in Generating the Tag");

	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_regularTxnPendingError - Sdf_error_base], \
		"SDF_ERROR - Already one Regular Transaction Pending");

	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_retransListError - Sdf_error_base], \
		"SDF_ERROR - Error in manipulating remote retransmission list");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_sendNetworkError - Sdf_error_base], \
		"SDF_ERROR - Failed to send to peer entity");

		/* Authorization error */
	Sdf_mc_strcpy(sdf_sc_dErrorStrings[Sdf_en_authError - Sdf_error_base], \
		"SDF_ERROR - Error in authentication");

	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_addressMismatchInVia - Sdf_error_base], \
		"SDF_ERROR - Ip Address mismatch in topmost Via Header");

	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_protocolMismatchInVia - Sdf_error_base], \
		"SDF_ERROR - Protocol mismatch in topmost Via Header");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_cseqValueExceeded - Sdf_error_base], \
		"SDF_ERROR - Maximum CSeq value exceeded");

	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_transactionFailure - Sdf_error_base], \
		"SDF_ERROR - Transaction Failed");
#ifdef SDF_SESSION_TIMER
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_invalidRefesher - Sdf_error_base], \
		"SDF_ERROR - Invalid Refresher");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_invalidSessionInterval - Sdf_error_base], \
		"SDF_ERROR - Invalid Session Interval");
	Sdf_mc_strcpy(sdf_sc_dErrorStrings[Sdf_en_invalidMinSe - Sdf_error_base], \
		"SDF_ERROR - Invalid MinSe");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_sessionTimerError - Sdf_error_base], \
		"SDF_ERROR: session Timer error");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_lowsessionTimerError - Sdf_error_base], \
		"SDF_ERROR - Low Session Timer value");
#endif
#ifdef SDF_HA
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_serializeInvalidFieldId - Sdf_error_base], \
		"SDF_ERROR - Invalid FieldID");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_serializeExtraArgsReqd - Sdf_error_base], \
		"SDF_ERROR - serialisation of extra arguments required");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_invalidObjectType - Sdf_error_base], \
		"SDF_ERROR - Invalid call object type");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_stackSerializationApiFailure - Sdf_error_base], \
		"SDF_ERROR - Error in stack serialisation API processing");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_stackDeserializationApiFailure - Sdf_error_base], \
		"SDF_ERROR - Error in Stack Deserialisation API processiong");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_objectNotInitialzedProperly - Sdf_error_base], \
		"SDF_ERROR - Error in Call Object Initialisation");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_initSerializationModuleFailed - Sdf_error_base], \
		"SDF_ERROR - Initialisation of Serialization Module Failed");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_cannotReconstructInThisState - Sdf_error_base], \
		"SDF_ERROR - Cannot Reconstruct in this state");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_dataMissing - Sdf_error_base], \
		"SDF_ERROR - Data is missing");
#endif
	/*Sdp Related Errors*/
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_noMatchingMid - Sdf_error_base],\
		"SDF_ERROR - No Matching Mid Present");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_malformedQosAttribute - Sdf_error_base],\
		"SDF_ERROR - Malformed Qos Attribute");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_noMatchingMediaError- Sdf_error_base],\
		"SDF_ERROR - No Matching Mid Present");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_missingOriginLineError- Sdf_error_base],\
		"SDF_ERROR - Missing Origin Line");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_missingConnectionLineError- Sdf_error_base],\
		"SDF_ERROR - Missing Connection Line Error");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_noMediaLineInAnswer - Sdf_error_base],\
		"SDF_ERROR - No Media Line in Answer");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_mediaGroupError- Sdf_error_base],\
		"SDF_ERROR - Media Group Error");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_mediaTypeMisMatch- Sdf_error_base],\
		"SDF_ERROR - Media Type Mismatch");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_transportProtocolMisMatch- Sdf_error_base],\
		"SDF_ERROR - Transport Protocol Mismatch");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_codecMismatch- Sdf_error_base],\
		"SDF_ERROR - Codec MisMatch");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_noSdpError- Sdf_error_base],\
		"SDF_ERROR - Sdp Missing");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_mediaStateTransitionError- Sdf_error_base],\
		"SDF_ERROR -  Failed to Change Media State");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_sdpAccessorError- Sdf_error_base],\
		"SDF_ERROR -  Error while accessing MediaHandling member");

	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_tooManyViaHdrsInResponse - Sdf_error_base],\
		"SDF_ERROR - Too many Via headers present");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_basicHeaderError - Sdf_error_base], \
		"SDF_ERROR - Error in Basic Mandatory Headers");
	Sdf_mc_strcpy(\
		sdf_sc_dErrorStrings[Sdf_en_nonMandatoryHeaderError - Sdf_error_base], \
		"SDF_ERROR - Error in Non Mandatory Error");
	Sdf_mc_strcpy(sdf_sc_dErrorStrings[Sdf_en_sdpLineError - Sdf_error_base], \
		"SDF_ERROR - Error in SDP line");
}

#endif 
