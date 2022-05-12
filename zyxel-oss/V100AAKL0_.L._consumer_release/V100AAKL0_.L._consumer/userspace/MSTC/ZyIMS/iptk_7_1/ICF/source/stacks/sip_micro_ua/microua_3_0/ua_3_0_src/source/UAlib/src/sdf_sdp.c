/****************************************************************************
 ** FUNCTION:	
 **					Has APIs for manipulating/accessing SDP
 **
 ****************************************************************************
 **
 ** FILENAME:		sdf_sdp.c
 **
 ** DESCRIPTION:	Has APIs for manipulating/accessing SDP
 **  	
 **
 ** DATE   	   	  NAME          REFERENCE      REASON
 ** ----    	  ----          ---------      ------
 ** 20-Nov-00	Subhash					  	  Creation
 **
 ** 28-Feb-01	Siddharth					  Changed the MatchMedia
 **											  logic.
 **
 ** 04-Jul-01	Siddharth					  Changed with additional
 **											  handling for overlapping
 **											  transactions.
 **
 ** 21 Nov  02	Elias/Srikanth				  SDP Revamp
 ** 12/05/03     Subbarao		UATK 2.1 Design      RFC 3428 feature added.
 **
 ****************************************************************************
 ** 			COPYRIGHT , Aricent, 2006
 ****************************************************************************/


#include "ALheader.h"
#include "sdf_common.h"
#include "sdf_struct.h"
#include "sdf_internal.h"
#include "sdf_sdpinternal.h"
#include "sdf_reqresp.h"
#include "sdf_sdp.h"
#include "sdf_accessor.h"
#include "sdf_free.h"
#include "sdf_debug.h"
#include "sdf_portlayer.h"
#include "sdf_trace.h"
#include "sdf_init.h"
#include "sdf_sdpinit.h"
#include "sdf_sdpaccessor.h"
#include "sdf_callapis.h"
#include "sdf_tables.h"
#ifndef SDF_LINT
#include "sdf_sdpstruct.h"
#include "sdf_sdpfree.h"
#include "sdf_list.h"
#endif


/*******************************************************************
** FUNCTION: sdp_insertEmailAtIndex_level1
**
** DESCRIPTION: This function insert email line at specified index in 
**				Sdp Message
*******************************************************************/
SipBool sdp_insertEmailAtIndex_level1
#ifdef ANSI_PROTO
	( SdpMessage *msg, SIP_S8bit* pEmail, SIP_U32bit cnt, SipError *err )
#else
	( msg,pEmail,cnt,err)
	  SdpMessage *msg;
	  SIP_S8bit* pEmail;
	  SIP_U32bit *cnt;
	  SipError *err;
#endif
{
	return sdp_insertEmailAtIndex(msg,pEmail,cnt,err);
}

/*******************************************************************
** FUNCTION: sdp_getPhoneCount_level1
**
** DESCRIPTION: This function retrieves the number of slPhone fields
**		in an SDP message structure
**
*******************************************************************/
SipBool sdp_getPhoneCount_level1
#ifdef ANSI_PROTO
	( SdpMessage *msg, SIP_U32bit *cnt, SipError *err )
#else
	( msg,cnt,err)
	  SdpMessage *msg;
	  SIP_U32bit *cnt;
	  SipError *err;
#endif
{
	return sdp_getPhoneCount(msg,cnt,err);
}	
SipBool sdp_getEmailCount_level1
#ifdef ANSI_PROTO
	( SdpMessage *msg, SIP_U32bit *cnt, SipError *err )
#else
	( msg,cnt,err)
	  SdpMessage *msg;
	  SIP_U32bit *cnt;
	  SipError *err;
#endif
{
	return sdp_getEmailCount(msg,cnt,err);
}	
SipBool sdp_getBandwidthCount_level1
#ifdef ANSI_PROTO
	( SdpMessage *msg, SIP_U32bit *cnt, SipError *err )
#else
	( msg,cnt,err)
	  SdpMessage *msg;
	  SIP_U32bit *cnt;
	  SipError *err;
#endif
{
	return sdp_getBandwidthCount(msg,cnt,err);
}	

/*******************************************************************
** FUNCTION: sdp_getBandwidthAtIndex
**
** DESCRIPTION: This function retrieves the pBandwidth field at a
**		specified index in an SDP message structure
**
*******************************************************************/
SipBool sdp_getBandwidthAtIndex_level1
#ifdef ANSI_PROTO
	( SdpMessage *msg, SIP_S8bit **ppBandwidth, SIP_U32bit cnt, SipError *err )
#else
	( msg,ppBandwidth,cnt,err)
	  SdpMessage *msg;
	  SIP_S8bit **ppBandwidth;
	  SIP_U32bit cnt;
	  SipError *err;
#endif
{
	return sdp_getBandwidthAtIndex(msg,ppBandwidth,cnt,err);
}	
SipBool sdp_getPhoneAtIndex_level1
#ifdef ANSI_PROTO
	( SdpMessage *msg, SIP_S8bit **ppPhone, SIP_U32bit cnt, SipError *err )
#else
	( msg,ppPhone,cnt,err)
	  SdpMessage *msg;
	  SIP_S8bit **ppPhone;
	  SIP_U32bit cnt;
	  SipError *err;
#endif
{
	return sdp_getPhoneAtIndex(msg,ppPhone,cnt,err);
}	
SipBool sdp_getEmailAtIndex_level1
#ifdef ANSI_PROTO
	( SdpMessage *msg, SIP_S8bit **ppEmail, SIP_U32bit cnt, SipError *err )
#else
	( msg,ppEmail,cnt,err)
	  SdpMessage *msg;
	  SIP_S8bit **ppEmail;
	  SIP_U32bit cnt;
	  SipError *err;
#endif
{
	return sdp_getEmailAtIndex(msg,ppEmail,cnt,err);
}	


/****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaIsSdpBodyPresent
 **
 ** DESCRIPTION: 	This function returns Sdf_co_success/Sdf_co_fail depending 
 **					on whether the SipMessage contains a SDP message body
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaIsSdpBodyPresent
#ifdef ANSI_PROTO
	(SipMessage *pMessage,
	Sdf_st_error *pError)
#else
	(pMessage, pError)
	SipMessage *pMessage;
	Sdf_st_error *pError;
#endif	
{
	Sdf_ty_u32bit i=0, dCount=0;
	en_SipMsgBodyType dType;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaIsSdpBodyPresent");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
		return Sdf_co_fail;
#endif	/* End of param validation ifdef */

	if(sip_getMsgBodyCount(pMessage, &dCount, \
		(SipError*)&(pError->stkErrCode)) == SipFail) return Sdf_co_fail;
	
	if (dCount == 0)
	{
		pError->errCode = Sdf_en_noExistError;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_ivk_uaIsSdpBodyPresent");
		return Sdf_co_fail;
	}

	for (i = 0; i < dCount; i++)
	{
		if (sip_getMsgBodyTypeAtIndex(pMessage, &dType, i, (SipError*)\
			&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaIsSdpBodyPresent( ): "
				"Failed to get the type of a Message body in the "
				"SIP message", pError);
#endif			
			pError->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		if (dType == SipSdpBody)
		/* Return success if atleast one SdpMsgBody found */
		{
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
				"sdf_ivk_uaIsSdpBodyPresent");
			pError->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}	
	}

	pError->errCode = Sdf_en_noExistError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaIsSdpBodyPresent");
	return Sdf_co_fail;
}

/****************************************************************************
 ** FUNCTION: 		sdf_fn_uaStoreAllMsgBodies
 **
 ** DESCRIPTION: 	This function stores all message bodies present in the 
 **					SIP message into the Call object's MsgBodyList.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_fn_uaStoreAllMsgBodies
#ifdef ANSI_PROTO
	(SipMessage		*pSipMessage,
	Sdf_ty_slist	*pslDestinationList,
	Sdf_st_error 	*pError)
#else
	(pSipMessage, pslDestinationList, pError)
	SipMessage *pSipMessage;
	Sdf_ty_slist *pslDestinationList;
	Sdf_st_error 	  *pError;
#endif	
{
	Sdf_ty_u32bit i=0, dCount=0;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaStoreAllMsgBodies");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR	
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaStoreAllMsgBodies(): "
			"Invalid Error parameter passed to function");
#endif			
		return Sdf_co_fail;
	}
	if (pSipMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaStoreAllMsgBodies( ): "
			"Invalid param passed to function: SipMessage", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pslDestinationList == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaStoreAllMsgBodies( ): "
			"Invalid param passed to function: Destination list", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/* 
	 * Store the msgBodies in the destination now. Storage is
	 * by reference to the original only.
	 */
	if(sip_getMsgBodyCount(pSipMessage, &dCount, (SipError*)\
		&(pError->stkErrCode)) == SipFail) return Sdf_co_fail;

	for (i = 0; i < dCount; i++)
	{
		SipMsgBody *pMsgBody;

		/* Ref count increases now */
		if (sip_getMsgBodyAtIndex(pSipMessage, &pMsgBody, i, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaStoreAllMsgBodies( ): "
				"Failed to get a Message body from the SIP message", pError);
#endif			
			pError->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		if (sdf_listAppend(pslDestinationList, (Sdf_ty_pvoid)pMsgBody, \
			pError) == Sdf_co_fail)
		{
			pError->errCode = Sdf_en_callObjectAccessError;
			sip_freeSipMsgBody(pMsgBody);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaStoreAllMsgBodies( ): "
				"Failed to append a message body to the call object's "
				"list of message bodies", pError);
#endif			
			pError->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}	
		/* 	
		 * no free called deliberately to keep refcount incremented
		 */
	}
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaStoreAllMsgBodies");
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 	sdf_ivk_uaFormSdpMedia
 **
 ** DESCRIPTION: This function forms a SdpMedia using the parameters 
 **				 passed into the function
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormSdpMedia
#ifdef ANSI_PROTO
	 (Sdf_ty_s8bit *pMvalue,
	 Sdf_ty_u32bit dPort,
	 Sdf_ty_u32bit dNumberOfPorts,
	 Sdf_ty_s8bit *pTransport,
	 Sdf_ty_s8bit *pPayLoadList,
	 Sdf_ty_s8bit *pTypeOfTransport,
	 Sdf_ty_slist *slAttributes,
	 Sdf_ty_slist *slConnection,
	 Sdf_ty_s8bit *pBandwidth,
	 Sdf_ty_s8bit *pInfo,
	 Sdf_ty_s8bit *pKey,
	 SdpMedia **pSdpMedia,
	 Sdf_st_error *pErr)
#else
	(pMvalue,dPort, dNumberOfPorts, pTransport, pPayLoadList,\
	 pTypeOfTransport, slAttributes, slConnection, pBandwidth, pInfo, pKey, \
	 pSdpMedia, pErr )
	 Sdf_ty_s8bit *pMvalue;
	 Sdf_ty_u32bit dPort;
	 Sdf_ty_u32bit dNumberOfPorts;
	 Sdf_ty_s8bit *pTransport;
	 Sdf_ty_s8bit *pPayLoadList;
	 Sdf_ty_s8bit *pTypeOfTransport;
	 Sdf_ty_slist *slAttributes;
	 Sdf_ty_slist *slConnection;
	 Sdf_ty_s8bit *pBandwidth;
	 Sdf_ty_s8bit *pInfo;
	 Sdf_ty_s8bit *pKey;
	 SdpMedia **pSdpMedia;
	 Sdf_st_error * pErr;
#endif
{
	Sdf_ty_s8bit 	dUserPayLoads[Sdf_co_smallBufferSize];
   	Sdf_ty_s8bit	*pConvertedPayLoadList = Sdf_co_null;
	Sdf_ty_s8bit	*pPayload = Sdf_co_null; 
	Sdf_ty_s8bit 	dTemp[Sdf_co_smallBufferSize];
	Sdf_ty_u32bit i = 0, j=0, attrindex=0;
	Sdf_ty_s8bit *pLasts = Sdf_co_null;
	Sdf_ty_s8bit	*pTmpVal = Sdf_co_null;
	Sdf_ty_s8bit	*pLocalVal = Sdf_co_null;

	pConvertedPayLoadList=Sdf_co_null;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaFormSdpMedia");
	
	(void)pBandwidth;
#ifdef SDF_PARAMVALIDATION
	/* Error checks in the input parameters */
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;

	if(pPayLoadList == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
			"Invalid param passed to function: payload list", pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif	/* End of param validation ifdef */
	*pSdpMedia=Sdf_co_null;

	if (sip_initSdpMedia(pSdpMedia,(SipError*)&(pErr->stkErrCode)) \
		== SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
			"Failed to initialize a SdpMedia structure", pErr);
#endif		
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}
	if (pMvalue == Sdf_co_null)
	{
		pTmpVal = Sdf_mc_strdupCallHandler("audio");

		if (sdp_setMvalueInMedia((*pSdpMedia), pTmpVal, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
				"Failed to set media value in the SdpMedia structure", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
	}
	else	
	{	
		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupCallHandler(pMvalue);

		if (sdp_setMvalueInMedia((*pSdpMedia), pTmpVal, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
				"Failed to set media value in the SdpMedia structure", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
	}
	if (sdp_setPortInMedia((*pSdpMedia),(Sdf_ty_u16bit)dPort, \
		(SipError*) &(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
			"Failed to set port number in SdpMedia structure", pErr);
#endif		
		pErr->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}
	if (dNumberOfPorts > 1)
	{	
		if (sdp_setNumportInMedia((*pSdpMedia), dNumberOfPorts,\
			(SipError*) &(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): \
			Failed to set number of ports in the SdpMedia structure",\
			pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
	}	
	if (pTransport == Sdf_co_null)
	{
		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupCallHandler("RTP/AVP");


		if (sdp_setProtoInMedia((*pSdpMedia), pTmpVal, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
				"Failed to set transport type in the SdpMedia", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
	}	
	else
	{
		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupCallHandler(pTransport);
		
		if (sdp_setProtoInMedia((*pSdpMedia), pTmpVal, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
				"Failed to set transport type in the SdpMedia", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
	}	
		
	Sdf_mc_strcpy(dUserPayLoads, pPayLoadList);
	pPayload = Sdf_mc_strtokr(dUserPayLoads, " ",&pLasts);	
	for(j=0;j<100; j++)
		dTemp[j] = '\0'; /* Initializing the payload list */
	if (pPayload != Sdf_co_null)
	{
#ifdef SDF_LINT
        for(;;)
#else            
        do
#endif            
		{
			if (!sdf_fn_uaIsNumber(pPayload))
			{
				pLocalVal = sdf_fn_uaMapPayloadFromCodec(pPayload, pErr);
				if(pLocalVal == Sdf_co_null)
				{
					Sdf_mc_strcat(dTemp, pPayload);
				}
				else
				{
					Sdf_mc_strcat(dTemp, pLocalVal);
					(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid *)&pLocalVal, \
						pErr);
				}


			}		
			else
				Sdf_mc_strcat(dTemp, pPayload);

			/* This is to take care of the last codec in the list */
			if ((pPayload = Sdf_mc_strtokr(pLasts, " ",&pLasts))\
				!= Sdf_co_null)
				Sdf_mc_strcat(dTemp, " ");
			else
				break;
#ifdef SDF_LINT
        }
#else            
		} while (1);
#endif        
		/* Copy the payload list from the temporary variable */
		pConvertedPayLoadList = (Sdf_ty_s8bit *) \
		sdf_memget (Sdf_mc_callHandlerMemId, \
			Sdf_mc_strlen(dTemp) + 1, pErr);
#ifdef SDF_LINT
    if (pConvertedPayLoadList == Sdf_co_null)
	{
        /*sip_freeSdpMedia(*pSdpMedia); */
        return Sdf_co_fail;
    }
#endif        
		Sdf_mc_strcpy(pConvertedPayLoadList, dTemp);
	}
	if (sdp_setFormatInMedia((*pSdpMedia), \
		pConvertedPayLoadList,(SipError*)\
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
			"Failed to set format in the SdpMedia structure", pErr);
#endif		
		pErr->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}
	if (slAttributes != Sdf_co_null)
	{
		Sdf_ty_u32bit dSize;
		Sdf_st_listIterator dListIterator;

		(void)sdf_listSizeOf(slAttributes, &dSize, pErr);

		(void)sdf_listInitIterator(slAttributes, &dListIterator, pErr);

		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			Sdf_ty_s8bit *pAttr;

			pAttr = (Sdf_ty_s8bit*)\
				(dListIterator.pCurrentElement->pData);

			if (sdp_insertAttrFromStringAtIndexInMedia((*pSdpMedia), pAttr, \
				attrindex, (SipError*)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
					"Failed to insert Attr in the SdpMedia structure", pErr);
#endif				
				pErr->errCode=Sdf_en_sdpError;
				return Sdf_co_fail;
			}
			(void)sdf_listNext(&dListIterator, pErr);
			attrindex++;
		}		
	}		
	if(slConnection != Sdf_co_null)
	{
		Sdf_st_listIterator dListIterator;

		i = 0;

		(void)sdf_listInitIterator(slConnection, &dListIterator, pErr);
			
		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			Sdf_ty_s8bit *pAttr;

			pAttr = (Sdf_ty_s8bit*)\
				(dListIterator.pCurrentElement->pData);

			if (sdp_insertConnectionFromStringAtIndexInMedia((*pSdpMedia), \
				pAttr, i, (SipError*)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
					"Failed to insert Connection in the SdpMedia structure", \
					pErr);
#endif				
				pErr->errCode=Sdf_en_sdpError;
				return Sdf_co_fail;
			}
			(void)sdf_listNext(&dListIterator, pErr);
			i++;
		}
	}

	/*To Link with the latest stack */
	if(Sdf_co_null!=pBandwidth)
	{
		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupCallHandler(pBandwidth);

		if(SipFail==sdp_insertBandwidthAtIndexInMedia((*pSdpMedia),pTmpVal, \
					(Sdf_ty_u32bit) 0, (SipError*)&(pErr->stkErrCode)))
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
				"Failed to insert Bandwidth in the SdpMedia structure", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
	}
	
	if(Sdf_co_null!=pInfo)
	{
		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupCallHandler(pInfo);

		if (SipFail==sdp_setInfoInMedia((*pSdpMedia),pTmpVal, \
			(SipError*)&(pErr->stkErrCode)))
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
				"Failed to insert Info in the SdpMedia structure", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
	}
		
	if(Sdf_co_null!=pKey)
	{
		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupCallHandler(pKey);

		if(SipFail==sdp_setKeyInMedia((*pSdpMedia),pTmpVal, \
			(SipError*)&(pErr->stkErrCode)))
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
				"Failed to insert Key in the SdpMedia structure", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
	}

	/* Setting the Type of Transfer. eg: a=recvonly */
	if (pTypeOfTransport != Sdf_co_null)
	{
		SdpAttr *pGlobAttr;
		if (sip_initSdpAttr(&pGlobAttr, (SipError*)\
			&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
				"Failed to initialize a SdpAttr structure", pErr);
#endif			
			pErr->errCode=Sdf_en_memoryError;
			return Sdf_co_fail;
		}

		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupCallHandler(pTypeOfTransport);
		
		if (sdp_setNameInAttr(pGlobAttr, pTmpVal, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSdpAttr(pGlobAttr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
				"Failed to get the name from SdpAttr structure", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		if (sdp_insertAttrAtIndexInMedia((*pSdpMedia), pGlobAttr, attrindex, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSdpAttr(pGlobAttr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMedia( ): "
				"Failed to insert Attr in the SdpMedia structure", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		
		/* Freeing the local reference */
		sip_freeSdpAttr(pGlobAttr);
	}
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaFormSdpMedia");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaSetAllMsgBody
 **
 ** DESCRIPTION: 	This function sets all the message bodies from either:
 **
 **					a) the CallObject's MsgBodyList 
 **								OR
 **					b) pOverlapTransInfo's MsgBodyList
 **
 **					into the outgoing SIP message. The former is chosen if 
 **					pOverlapTransInfo is NULL. If this is not the case, the 
 **					latter is chosen.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetAllMsgBody
#ifdef ANSI_PROTO
	(Sdf_st_transaction *pTransaction,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	Sdf_st_error *pError)
#else
	(pTransaction, pOverlapTransInfo, pError)
	Sdf_st_transaction *pTransaction;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_st_error *pError;
#endif	
{
	Sdf_ty_slist *pSrcList = Sdf_co_null;
	SipMessage 	 *pSipMsg = Sdf_co_null;
#ifdef SDF_IM
	Sdf_st_msgInfo			dMessageInfo;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaSetAllMsgBody");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
		return Sdf_co_fail;
#endif	/* End of param validation ifdef */

	/*
	 * Choose the MsgBodyList inside the call object if
	 * overlapTransactionInfo is NULL
	 */
	if (Sdf_co_null == pOverlapTransInfo)
	{
		pSrcList = &(pTransaction->slMsgBodyList);
		pSipMsg = pTransaction->pSipMsg;
	}
	else
	{
		pSrcList = &(pOverlapTransInfo->slMsgBodyList);
		pSipMsg = pOverlapTransInfo->pSipMsg;
	}
#ifdef SDF_IM
	/*w534*/(void)sdf_fn_uaGetMessageInfo(pSipMsg,  &dMessageInfo, pError);

	if( (dMessageInfo.dReqRespType == SipMessageResponse) && \
		(dMessageInfo.dMsgType == Sdf_en_message))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_messageBodyNotAllowed, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetAllMsgBody(): "
			"Message body is not allowed in this message", pError);
#endif		
		pError->errCode = Sdf_en_messageBodyNotAllowed;
		return Sdf_co_fail;
	}
#endif

	/*
	 * copy elements from the msgbodylist into the
	 * SipMessage now
	 */
	if (sdf_ivk_uaSetMsgBodyListInSipMessage(pSrcList, \
		pSipMsg, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetAllMsgBody( ): "
			"Failed to set message body list "
			,pError);
#endif		
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}	

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaSetAllMsgBody");
	return Sdf_co_success;
}	

/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaSetMsgBodyListInSipMessage
 **
 ** DESCRIPTION: 	This function sets the message bodies from the given 
 **					list into the outgoing SIP message. 
 **
 **					Note:
 **					It first removes any previously present MsgBodies in 
 **					the SIP message, and then adds the new list.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetMsgBodyListInSipMessage
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pMsgBodyList,
	SipMessage *pMsg,
	Sdf_st_error *pError)
#else
	(pMsgBodyList, pMsg, pError)
	Sdf_ty_slist *pMsgBodyList;
	SipMessage *pMsg;
	Sdf_st_error *pError;
#endif	
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaSetMsgBodyListInSipMessage");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pError == Sdf_co_null)
		return Sdf_co_fail;
	if (pMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetMsgBodyListInSipMessage( ): "
			"Invalid param passed to function: SipMessage", pError);
#endif		
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/* 
	 * First delete the MsgBodys in the SIP message, then start 
	 * appending new list.
	 */
	if (sip_listDeleteAll(&(pMsg->slMessageBody),(SipError*)&(pError-> \
		stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetMsgBodyListInSipMessage( ): "
			"Failed to delete the existing list of message bodies "
			"in the Sip Message", pError);
#endif		
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}
	
	/* 
	 * Now append the new MsgBodyList to this Sip Message
	 */
	if (sdf_ivk_uaAppendMsgBodyListToSipMessage(pMsgBodyList, pMsg, pError) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetMsgBodyListInSipMessage( ): "
			"Failed toappend the list of message bodies to the "
			"sip message", pError);
#endif		
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetMsgBodyListInSipMessage");
	return Sdf_co_success;
}


/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaAppendMsgBodyListToSipMessage
 **
 ** DESCRIPTION: This function appends the message bodies from the given 
 **				 list into the SIP message. The MsgBodies already present 
 **				 in the SipMessage are left untouched.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAppendMsgBodyListToSipMessage
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pMsgBodyList,
	SipMessage *pMsg,
	Sdf_st_error *pError)
#else
	(pMsgBodyList, pMsg, pError)
	Sdf_ty_slist *pMsgBodyList;
	SipMessage *pMsg;
	Sdf_st_error *pError;
#endif	
{
	Sdf_ty_u32bit	dSize=0;
#ifdef SIP_MIME_PARSING
	Sdf_ty_u32bit dIndex=0;
#ifndef SDF_LINT
    Sdf_ty_u32bit dCount=0;
#endif    
	SipHeader 	*pMimeVersionHdr = Sdf_co_null;
	Sdf_ty_s8bit	*pTmpVal = Sdf_co_null;
	Sdf_ty_s8bit  pMultipart[Sdf_co_smallBufferSize] = \
				"Content-type: multipart/mixed; boundary=";	
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaAppendMsgBodyListToSipMessage");

	(void)sdf_listSizeOf(pMsgBodyList, &dSize, pError);
#ifdef SIP_MIME_PARSING
	if (dSize > 1)
	{
		Sdf_st_listIterator dListIterator;
        SipHeader 		dHeader;

		(void)sdf_listInitIterator(pMsgBodyList, &dListIterator, pError);	
		
		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			SipMsgBody 		*pMsgBody = Sdf_co_null;
			SipMimeHeader 	*pMimeHdr = Sdf_co_null;
			Sdf_ty_bool		dAddContentTypeHeader = Sdf_co_false;
			SipHeader 		*pHeader = Sdf_co_null;
			Sdf_ty_s8bit 	*pContentType = Sdf_co_null;

			pMsgBody = (SipMsgBody*)\
				(dListIterator.pCurrentElement->pData);

			/*
			 * Since more than one message body is being added to the
			 * SipMessage in the form of a multipart/mime message we need
			 * to check if each of these message bodies has a content-type
			 * header. In case it does not, we add it here. 
			 */
			if (sip_bcpt_getMimeHeaderFromMsgBody(pMsgBody, &pMimeHdr, \
				(SipError *)&(pError->stkErrCode)) != SipFail)
			{
				if(sip_initSipHeader(&pHeader,SipHdrTypeAny,\
					(SipError*)&(pError->stkErrCode))==SipFail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ):"
						" Failed to init a content-type header", \
						pError);
#endif				
					pError->errCode=Sdf_en_sdpError;
					return Sdf_co_fail;					
				}

				if (sip_bcpt_getContentTypeFromMimeHdr(pMimeHdr, \
					&pHeader, (SipError *)&(pError->stkErrCode)) \
					!= SipFail)
				{
					/*
					 * A content type header exists in the MIME header. Leave
					 * it as it is.
					 */
				}
				else
				{
					/*
					 * A MIME header is present in the message body, but it
					 * does not have a content type header. Set the flag so
					 * as to add it below.
					 */
					dAddContentTypeHeader = Sdf_co_true;
				}
				sip_bcpt_freeSipMimeHeader(pMimeHdr);
				sip_freeSipHeader(pHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid *)&pHeader, pError);
			}
			else
			{
				/*
				 * A MIME header is absent in the message body. lets make
				 * one now since it is required. Set the flag for adding a
				 * content type header below.
				 */
				if(sip_bcpt_initSipMimeHeader(&pMimeHdr,\
					(SipError*)&(pError->stkErrCode))==SipFail )
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ):"
						" Failed to init a MIME header struct", \
						pError);
#endif				
					pError->errCode=Sdf_en_sdpError;
					return Sdf_co_fail;				
				}
				dAddContentTypeHeader = Sdf_co_true;
			}
			
			if (dAddContentTypeHeader == Sdf_co_true)
			{
				if (sdf_fn_uaGetContentTypeString(pMsgBody, &pContentType, \
					pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ):"
						" Failed to get content type for the message body.", \
						pError);
#endif				
					pError->errCode=Sdf_en_sdpError;
                    /* klockwork fix */
					sip_bcpt_freeSipMimeHeader(pMimeHdr);

					return Sdf_co_fail;
				}
				if(sip_initSipHeader(&pHeader,SipHdrTypeContentType,\
					(SipError*)&(pError->stkErrCode))==SipFail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ):"
						" Failed to init a content-type header", \
						pError);
#endif				
					pError->errCode=Sdf_en_sdpError;
					return Sdf_co_fail;					
				}
				if(sip_setMediaTypeInContentTypeHdr(pHeader, pContentType,\
					(SipError*)&(pError->stkErrCode))==SipFail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ):"
						" Failed to set content-type in the SipHeader", \
						pError);
#endif				
					pError->errCode=Sdf_en_sdpError;
					return Sdf_co_fail;					
				}
				if(sip_bcpt_setContentTypeInMimeHdr(pMimeHdr,pHeader,\
					(SipError*)&(pError->stkErrCode))==SipFail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ):"
						" Failed to set content-type header in the Mime "
						"header", pError);
#endif				
					pError->errCode=Sdf_en_sdpError;
					return Sdf_co_fail;					
				}
				if(sip_bcpt_setMimeHeaderInMsgBody(pMsgBody,pMimeHdr,\
					(SipError*)&(pError->stkErrCode))==SipFail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ):"
						" Failed to set Mime header in the message body.", \
						pError);
#endif				
					pError->errCode=Sdf_en_sdpError;
					return Sdf_co_fail;				
				}
				sip_freeSipHeader(pHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid *)&pHeader, pError);
				/* memleak fix */
				sip_bcpt_freeSipMimeHeader(pMimeHdr);
				/* memleak fix */
			}

			if (sip_insertMsgBodyAtIndex(pMsg, pMsgBody, dIndex, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ): "
					"Failed to insert message body into the SipMessage", \
					pError);
#endif				
				pError->errCode=Sdf_en_sdpError;
				return Sdf_co_fail;
			}
			(void)sdf_listNext(&dListIterator, pError);
			dIndex++;
#ifndef SDF_LINT
			dCount++;
#endif
		} /* End of loop inserting message bodies in the SIP message */


		/* Add a MIME-version header now */
		
		if (sip_initSipHeader(&pMimeVersionHdr, SipHdrTypeMimeVersion, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ): "
					"Failed to init a Meme version header", \
					pError);
#endif				
				return Sdf_co_fail;
		}
		
		pTmpVal = Sdf_mc_strdupCallHandler("1.0");

		if (sip_bcpt_setVersionInMimeVersionHdr(pMimeVersionHdr, pTmpVal, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ): "
					"Failed to set value in Meme version header", \
					pError);
#endif				
				return Sdf_co_fail;
		}

		if (sip_setHeader(pMsg, pMimeVersionHdr, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ): "
					"Failed to set value in Meme version header", \
					pError);
#endif				
				return Sdf_co_fail;
		}

		/* Free local variable */
		sip_freeSipHeader(pMimeVersionHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&pMimeVersionHdr, pError);
        /*
		 * If  content-Type header is already there then don't overwite
		 * otherwise add the Content-Type header
		 */
		if(sip_getHeader(pMsg,SipHdrTypeContentType,&dHeader,\
			(SipError *)&(pError->stkErrCode))==SipFail)
		{
			if( (SipError)(pError->stkErrCode) == E_NO_EXIST)
			{
        		/* Set the ContentType header of the SipMessage to multipart */
				STR_CAT(pMultipart,Sdf_co_boundary);
        		if (sip_setHeaderFromStringAtIndex(pMsg, SipHdrTypeContentType, 
        			pMultipart, 0, (SipError *)&(pError->stkErrCode)) == SipFail)
        		{
#ifdef SDF_ERROR			
		        	sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				        (Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ): "
        				"Failed to set the content type header in the SipMessage", \
        				pError);
#endif			
        			pError->errCode=Sdf_en_sdpError;
        			return Sdf_co_fail;
        		}
			}
			else
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyList"
					"ToSipMessage( ): "
					"Failed to get Content-Type header", pError);
#endif			
				pError->errCode=Sdf_en_sdpError;
				return Sdf_co_fail;
			}
		}
		else
		{
			sip_freeSipHeader(&dHeader);
		}

	}
	else 
#endif	
	if (dSize == 1)
	{
		SipHeader *pHeader = Sdf_co_null;
		SipMsgBody *pMsgBody = Sdf_co_null;
		Sdf_ty_s8bit *pTemp = Sdf_co_null;
		SipHeader dHeader;

		if (sdf_listGetAt(pMsgBodyList, 0, (Sdf_ty_pvoid *)&pMsgBody, \
			pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ): "
				"Failed to get the message body at index 0", pError);
#endif			
			pError->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		/*
		 * If the user has added the mime header, it can be removed at
		 * this stage, as it is not required for a single message body.
		 */
#ifdef SIP_MIME_PARSING
		sip_bcpt_freeSipMimeHeader(pMsgBody->pMimeHeader);
		pMsgBody->pMimeHeader = Sdf_co_null;
#endif		

		if (sip_insertMsgBodyAtIndex(pMsg, pMsgBody, 0, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ): "
				"Failed to insert the message body at index 0", pError);
#endif			
			pError->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
	
		/* 
		 * Fix for content-type begins ---- 
		 * If  content-Type header is already there then don't overwite
		 * otherwise add the Content-Type header
		 */
		if(sip_getHeader(pMsg,SipHdrTypeContentType,&dHeader,\
			(SipError *)&(pError->stkErrCode))==SipFail)
		{
			if( (SipError)(pError->stkErrCode) == E_NO_EXIST)
			{
				if (sdf_fn_uaGetContentTypeString(pMsgBody, &pTemp, \
					pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage():"
					"Failed to get content type string for the message body",\
					pError);
#endif			
					pError->errCode=Sdf_en_sdpError;
					return Sdf_co_fail;
				}
				/* Setting Content Type Header */
				if (sip_initSipHeader(&pHeader, SipHdrTypeContentType, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ):"
						"Failed to initialize header", pError);
#endif			
					pError->errCode=Sdf_en_memoryError;
					(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
						(Sdf_ty_pvoid *)&pTemp, pError);
					return Sdf_co_fail;
				}
				if (sip_setMediaTypeInContentTypeHdr(pHeader, pTemp, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ):"
					"Failed to set media type in content-type header", pError);
#endif			
					pError->errCode=Sdf_en_sdpError;
					sip_freeSipHeader(pHeader);
					(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
						(Sdf_ty_pvoid *)&pHeader, pError);
					(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
						(Sdf_ty_pvoid *)&pTemp, pError);
					return Sdf_co_fail;
				}
				if (sip_setHeader(pMsg, pHeader, \
					(SipError *)&(pError->stkErrCode)) \
					== SipFail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyListToSipMessage( ):"
					"Failed to set header in the SipMessage", pError);
#endif			
					pError->errCode=Sdf_en_sdpError;
					sip_freeSipHeader(pHeader);
					(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
						(Sdf_ty_pvoid *)&pHeader, pError);
					return Sdf_co_fail;
				}
				/* Freeing the local reference */	
				sip_freeSipHeader(pHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid *)&pHeader, pError);
			}
			else
			{
			
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAppendMsgBodyList"
					"ToSipMessage( ): "
					"Failed to get Content-Type header", pError);
#endif			
				pError->errCode=Sdf_en_sdpError;
				return Sdf_co_fail;
			}
		}
		else
		{
			sip_freeSipHeader(&dHeader);
		}
	}

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaAppendMsgBodyListToSipMessage");
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/****************************************************************************
 ** FUNCTION: sdf_ivk_uaMatchMediaFromSdpMessage
 **
 ** DESCRIPTION: This function takes the recd SdpMessage and matches the 
 **				 media details in this SdpMessage against the media details 
 **				 in the initData of the call object.
 **
 ****************************************************************************/
Sdf_ty_matchMediaResult sdf_ivk_uaMatchMediaFromSdpMessage
#ifdef ANSI_PROTO
	(SdpMessage *pIncomingSdpMessage, Sdf_st_initData *pInitData, \
	Sdf_st_mediaHandling *pMediaInfo, SdpMessage **ppReturnSdpMessage,\
	Sdf_ty_slist *slAcceptedMedia,Sdf_st_error *pError)
#else
	(pIncomingSdpMessage, pInitData, pMediaInfo,ppReturnSdpMessage, \
		slAcceptedMedia, pError)
	SdpMessage 		*pIncomingSdpMessage;
	Sdf_st_initData *pInitData;
	Sdf_st_mediaHandling *pMediaInfo;
	SdpMessage 		**ppReturnSdpMessage;
	Sdf_ty_slist	*slAcceptedMedia;
	Sdf_st_error 	*pError;
#endif
{

	Sdf_st_sessionParam  	*pRecdSessionParam = Sdf_co_null;
	Sdf_st_sessionParam  	*pNgtdSessionParam = Sdf_co_null;
	Sdf_ty_matchMediaResult dRetVal;
	Sdf_ty_slist            slMediaStream;
#ifdef SDF_SESSIONCHANGE
	Sdf_st_sessionChange *pSessionChange = Sdf_co_null;
#endif
	*ppReturnSdpMessage = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaMatchMediaFromSdpMessage");

#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null)||(pIncomingSdpMessage == Sdf_co_null)|| \
		(pInitData == Sdf_co_null)|| (pMediaInfo == Sdf_co_null) ||\
		(ppReturnSdpMessage == Sdf_co_null)|| (slAcceptedMedia == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaMatchMediaFromSdpMessage( ): "
			"Invalid Param passed to function", pError);
#endif			
			pError->errCode = Sdf_en_invalidParamError;
		}
		return Sdf_en_matchingNotDone;
	}
#endif	/* End of param validation ifdef */

	if(sdf_ivk_uaInitSessionParam(&pRecdSessionParam, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMediaFromSdpMessage(): "
			"Could not init the Sdf_st_msgBody structure.",pError);
#endif				
		return Sdf_en_matchingNotDone;
	}
    if(sdf_ivk_uaFormSessionParam(pIncomingSdpMessage,  pRecdSessionParam, \
		 Sdf_en_convertRemote, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMediaFromSdpMessage(): "
			"Could not convert the SdpMessage to SessionParam.",pError);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pRecdSessionParam);
		return Sdf_en_matchingNotDone; 
	}

	/* 
	 * Update the LastOffer Recvd field of the media capabilities structure 
	 * if it is an offer
	 */
	if((pMediaInfo->dMediaState == Sdf_en_offerRecv) ||
	   (pMediaInfo->dMediaState == Sdf_en_offerChangeRecv))
	{
		if(sdf_ivk_uaSetLastOfferRecvd(pMediaInfo,
			pRecdSessionParam, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMediaFromSdpMessage(): "
				"Could not get the SessionParam from the sip message.",pError);
#endif
			(void)sdf_ivk_uaFreeSessionParam(pRecdSessionParam);
			return Sdf_en_matchingNotDone;
		}
	}

	/* 
	 * Initialize the Media Stream structures list 
	 */
	if(sdf_listInit(&slMediaStream,__sdf_fn_uaFreeMediaStream,\
			Sdf_co_false,pError) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeSessionParam(pRecdSessionParam);
		return  Sdf_en_matchingNotDone;
	}

    /* Now do the negotiation with the session params */
	dRetVal = sdf_ivk_uaCompareSessionParams(pRecdSessionParam, \
							pInitData->pSessionParam,\
							pMediaInfo, &pNgtdSessionParam,\
							&slMediaStream,
#ifdef SDF_SESSIONCHANGE
							&pSessionChange, 0,
#endif
	                        pError);
	if(dRetVal != Sdf_en_matchingFailed)
	{
		/* 
		 * Form the SDP message
		 */
		if(SipFail == sip_initSdpMessage(ppReturnSdpMessage, \
			(SipError *)&(pError->stkErrCode)))
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMediaFromSdpMessage(): "
				"Could not init the SessionParam structure.",pError);
#endif				
			(void)sdf_ivk_uaFreeSessionParam(pRecdSessionParam);
			(void)sdf_ivk_uaFreeSessionParam(pNgtdSessionParam);
			return Sdf_en_matchingFailed;
		}

		if(sdf_ivk_uaFormSdpMessageFromSessionParam(*ppReturnSdpMessage,\
				pNgtdSessionParam,Sdf_en_convertLocal,pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMediaFromSdpMessage(): "
				"Could not init the SessionParam structure.",pError);
#endif				
			(void)sdf_ivk_uaFreeSessionParam(pRecdSessionParam);
			(void)sdf_ivk_uaFreeSessionParam(pNgtdSessionParam);
			sip_freeSdpMessage(*ppReturnSdpMessage);
			return Sdf_en_matchingFailed;
		}

		/* Now convert the list of media streams to list of sdp media */
		if(sdf_fn_uaConvertMediaStreamListToSdpMediaList(slAcceptedMedia,\
			&slMediaStream, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMediaFromSdpMessage(): "
				"Could not convert the media stream list to"\
				"sdp media structure list"\
				,pError);
#endif				
				(void)sdf_ivk_uaFreeSessionParam(pRecdSessionParam);
				(void)sdf_ivk_uaFreeSessionParam(pNgtdSessionParam);
				sip_freeSdpMessage(*ppReturnSdpMessage);
				return Sdf_en_matchingFailed; 
		}

#ifdef SDF_TRACE
		/* 	Print the m= lines in the negotiated message 
			being returned in case of detailed trace */
		(void)sdf_fn_uaTraceMediaLines(Sdf_co_null, *ppReturnSdpMessage, \
			(Sdf_ty_s8bit *) "negotiated");
#endif
	}
	(void)sdf_ivk_uaFreeSessionParam(pRecdSessionParam);
	(void)sdf_ivk_uaFreeSessionParam(pNgtdSessionParam);
	
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaMatchMediaFromSdpMessage");
	return dRetVal;
}


/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaMatchMedia
 **
 ** DESCRIPTION: This function takes a call object and matches the media 
 **				 details in the Call Object's "incoming" SIP message against 
 **				 the media details in the initData for the call object. The 
 **				 final negotiated body is returned to the application.
 **
 ****************************************************************************/
Sdf_ty_matchMediaResult sdf_ivk_uaMatchMedia
#ifdef ANSI_PROTO
	(Sdf_st_mediaHandling *pMediaInfo,
	SipMessage 	 *pRecdSipMessage,
	Sdf_ty_slist *slAcceptedMedia,
	SdpMessage 	 **ppFormedSdp,
	Sdf_st_error *pError)
#else
	(pMediaInfo, pRecdSipMessage, slAcceptedMedia,ppFormedSdp, 
	 pError)
	Sdf_st_mediaHandling *pMediaInfo;
	SipMessage *pRecdSipMessage;
	Sdf_ty_slist *slAcceptedMedia;
	SdpMessage **ppFormedSdp;
	Sdf_st_error *pError;
#endif	
{
	Sdf_st_sessionParam  	*pRecvdSessionParam = Sdf_co_null;
	Sdf_st_sessionParam  	*pSessionParamLocal = Sdf_co_null;
	Sdf_st_sessionParam  	*pNgtdSessionParam = Sdf_co_null;
	Sdf_ty_matchMediaResult dRetVal;
	Sdf_ty_slist            slMediaStream;
#ifdef SDF_SESSIONCHANGE
	Sdf_st_sessionChange *pSessionChange = Sdf_co_null;
#endif

	*ppFormedSdp = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaMatchMedia");
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null)||(slAcceptedMedia == Sdf_co_null)|| \
		(pRecdSipMessage == Sdf_co_null)||(pMediaInfo == Sdf_co_null))
		
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaMatchMedia( ): "
			"Invalid Param passed to function", pError);
#endif			
			pError->errCode = Sdf_en_invalidParamError;
		}
		return Sdf_en_matchingNotDone;
	}
#endif	/* End of param validation ifdef */

	if(sdf_ivk_uaGetSessionParamFromSipMsg(pRecdSipMessage, 
		&pRecvdSessionParam,Sdf_en_convertRemote, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMedia(): "
			"Could not get the SessionParam from the sip message.",pError);
#endif
		return Sdf_en_matchingNotDone; 
	}

	/* 
	 * Update the LastOffer Recvd field of the media capabilities structure 
	 * if it is an offer
	 */
	if((pMediaInfo->dMediaState == Sdf_en_offerRecv) ||
	   (pMediaInfo->dMediaState == Sdf_en_offerChangeRecv))
	{
		if(sdf_ivk_uaSetLastOfferRecvd(pMediaInfo,
			pRecvdSessionParam, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMedia(): "
				"Could not get the SessionParam from the sip message.",pError);
#endif
			(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);
			return Sdf_en_matchingNotDone; 
		}
	}

    if(sdf_fn_uaGetSessionParamForNgtn(pMediaInfo,&pSessionParamLocal, pError)
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMedia(): "
			"Could not get the local SessionParam.",pError);
#endif
		return Sdf_en_matchingNotDone; 
	}
	/* 
	 * Initialize the Media Stream structures list 
	 */
	if(sdf_listInit(&slMediaStream,__sdf_fn_uaFreeMediaStream,\
			Sdf_co_false,pError) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);
		return  Sdf_en_matchingNotDone;
	}
	
    /* Now do the negotiation with the session params */
	dRetVal = sdf_ivk_uaCompareSessionParams(pRecvdSessionParam, \
							pSessionParamLocal, pMediaInfo, &pNgtdSessionParam,\
							&slMediaStream, 
#ifdef SDF_SESSIONCHANGE
							&pSessionChange, 0,
#endif
	                        pError);
	/*Now Check for the Return Value and Increment Version*/
	if((dRetVal == Sdf_en_matchingSuccessful) &&(
		(pMediaInfo->dMediaState == Sdf_en_offerRecv) ||
		(pMediaInfo->dMediaState == Sdf_en_offerChangeRecv)))
			
	{
		pNgtdSessionParam->dLocalSessionVersion++;
		pSessionParamLocal->dLocalSessionVersion++;
	}

	if(dRetVal != Sdf_en_matchingFailed)
	{
		/* 
		 * Form the Sdp Message
		 */
		if(sip_initSdpMessage(ppFormedSdp, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMedia(): "
				"Could not init the SessionParam structure.",pError);
#endif				
			(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);
			(void)sdf_ivk_uaFreeSessionParam(pSessionParamLocal);
			(void)sdf_ivk_uaFreeSessionParam(pNgtdSessionParam);
			(void)sdf_listDeleteAll(&slMediaStream, pError);
			return Sdf_en_matchingFailed; 
		}

		if(sdf_ivk_uaFormSdpMessageFromSessionParam(*ppFormedSdp, \
			pNgtdSessionParam,Sdf_en_convertLocal,pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMedia(): "
				"Could not init the SessionParam structure.",pError);
#endif				
			(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);
			(void)sdf_ivk_uaFreeSessionParam(pSessionParamLocal);
			(void)sdf_ivk_uaFreeSessionParam(pNgtdSessionParam);
			sip_freeSdpMessage(*ppFormedSdp);
			(void)sdf_listDeleteAll(&slMediaStream, pError);
			return Sdf_en_matchingFailed; 
		}

		/* Now convert the list of media streams to list of sdp media */
		if(sdf_fn_uaConvertMediaStreamListToSdpMediaList(slAcceptedMedia,\
			&slMediaStream, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMatchMedia(): "
			"Could not convert the media stream list to"\
			"sdp media structure list" ,pError);
#endif				
			(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);
			(void)sdf_ivk_uaFreeSessionParam(pSessionParamLocal);
			(void)sdf_ivk_uaFreeSessionParam(pNgtdSessionParam);
			sip_freeSdpMessage(*ppFormedSdp);
			(void)sdf_listDeleteAll(&slMediaStream, pError);
			return Sdf_en_matchingFailed; 
		}

#ifdef SDF_TRACE
		/* 	Print the m= lines in the negotiated message 
			being returned in case of detailed trace */
		(void)sdf_fn_uaTraceMediaLines(Sdf_co_null, *ppFormedSdp, \
			(Sdf_ty_s8bit *) "negotiated");
#endif
	}

	/* Free local variables */
	(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);
	(void)sdf_ivk_uaFreeSessionParam(pSessionParamLocal);
	(void)sdf_ivk_uaFreeSessionParam(pNgtdSessionParam);
	(void)sdf_listDeleteAll(&slMediaStream, pError);
	
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaMatchMedia");
	return dRetVal;
}/* End of function */	

/****************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaSetSdpInOverlapTransaction
 **
 ** DESCRIPTION: This function sets the SDP message in the  
 **		 		 MsgBodyList present in the OverLapTransaction
 **				 object passed to the function. 
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetSdpInOverlapTransaction
#ifdef ANSI_PROTO
	(Sdf_st_overlapTransInfo *pOverlapTransInfo,
    SdpMessage *pMessage,
	Sdf_st_error *pError)
#else
	(pOverlapTransInfo,pMessage, pError)
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
        SdpMessage   *pMessage;
	Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering function "
		"sdf_ivk_uaSetSdpInOverlapTransaction");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
		return Sdf_co_fail;
	if ((pOverlapTransInfo == Sdf_co_null) || (pMessage == Sdf_co_null))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSdpInOverlapTransaction( ): "
			"Invalid parameter passed to function:", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/* 
	 * Add the SDP Message to the msgBodyList in the OverlapTransaction 
	 * object 
	 */
	if (sdf_ivk_uaSetSdpInMsgBodyList(pMessage, \
		&(pOverlapTransInfo->slMsgBodyList), pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_overlapTransInfoAccessError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSdpInOverlapTransaction( ): "
			"Failed to add SDP message to message body list", pError);
#endif		
		pError->errCode=Sdf_en_overlapTransInfoAccessError;
		return Sdf_co_fail;
	}

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaSetSdpInOverlapTransaction");
	return Sdf_co_success;
}

/****************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaSetSdpInTransaction
 **
 ** DESCRIPTION: 	This function sets the SDP message in the  
 **					MsgBodyList present in the transaction
 **					object passed to the function. 
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetSdpInTransaction
#ifdef ANSI_PROTO
	(Sdf_st_transaction *pTransaction,
	SdpMessage *pMessage,
	Sdf_st_error *pError)
#else
	(pTransaction,pMessage, pError)
	Sdf_st_transaction *pTransaction;
	SdpMessage   *pMessage;
	Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering function "
		"sdf_ivk_uaSetSdpInTransaction");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
		return Sdf_co_fail;
	if (pTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSdpInTransaction( ): "
			"Invalid parameter passed to function: transaction object", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/* Add the SDP Message to the msgBodyList in the Transaction object */
	if (sdf_ivk_uaSetSdpInMsgBodyList(pMessage, \
		&(pTransaction->slMsgBodyList), pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionAccessError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSdpInTransaction( ): "
			"Failed to add SDP message to message body list", pError);
#endif		
		pError->errCode=Sdf_en_transactionAccessError;
		return Sdf_co_fail;
	}
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"function sdf_ivk_uaSetSdpInTransaction");
	return Sdf_co_success;
}



/****************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaSetSdpInMsgBodyList
 **
 ** DESCRIPTION: This function sets the SDP message in the  
 **				 MsgBodyList passed to the function. 
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetSdpInMsgBodyList
#ifdef ANSI_PROTO
	(SdpMessage *pMessage,
	Sdf_ty_slist *pMsgBodyList,
	Sdf_st_error *pError)
#else
	(pMessage,pMsgBodyList, pError)
	SdpMessage   *pMessage;
	Sdf_ty_slist *pMsgBodyList;  
	Sdf_st_error *pError;
#endif
{
	SipMsgBody *pMsgBody = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering function "
		"sdf_ivk_uaSetSdpInMsgBodyList");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
		return Sdf_co_fail;
	if (pMsgBodyList == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSdpInMsgBodyList( ): "
			"Invalid parameter passed to function", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */
	
	/* Remove any existing SdpMsgBody's from the slMsgBodyList */
	if (sdf_fn_uaDeleteSdpFromMsgBodyList(pMsgBodyList, \
		pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSdpInMsgBodyList( ): "
			"Failed to delete existing SDP message body", pError);
#endif		
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}

	/*
	 * Form the SipMsgBody with this SdpMessage
	 */
	if ((sip_initSipMsgBody(&pMsgBody, SipSdpBody,(SipError*)&(\
		pError->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSdpInMsgBodyList( ): "
			"Failed to allocate memory for the SIP message body", pError);
#endif		
		pError->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	if ((sip_setSdpInMsgBody(pMsgBody, pMessage,\
		(SipError*)&(pError->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSdpInMsgBodyList( ): "
			"Failed to set the SDP message in the SIP message body", pError);
#endif		
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}
	
	/*
	 * Add the MsgBody to the destination list now
	 */
	if (sdf_listAppend(pMsgBodyList, (Sdf_ty_pvoid) pMsgBody, \
		pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSdpInMsgBodyList( ): "
			"Failed to add the message body to the call object", pError);
#endif		
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}	

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaSetSdpInMsgBodyList");
	return Sdf_co_success;
}


/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaDeleteStreamsFromCall
 **
 ** DESCRIPTION: This function removes a stream from a call leg, either 
 **				 party sets port to zero in the session description for 
 **				 the media line corresponding to this stream when sending 
 **				 a Re-Invite
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaDeleteStreamsFromCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, \
	Sdf_ty_s8bit *pMediaFormats, \
	Sdf_ty_slist *pslMediaIDs,
	Sdf_st_error *pError)
#else
	(pObject,pMediaFormats,pslMediaIDs,pError)
	Sdf_st_callObject *pObject;
	Sdf_ty_s8bit *pMediaFormats;
	Sdf_ty_slist *pslMediaIDs;
	Sdf_st_error *pError;
#endif	
{
	Sdf_st_sessionParam *pSessionParams = Sdf_co_null;
	SipMsgBody *pSipMsgBody=Sdf_co_null;
	SdpMessage *pSdpMessage = Sdf_co_null;
	Sdf_st_transaction *pTransaction = Sdf_co_null;
	Sdf_ty_slist dslMediaIDs;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaDeleteStreamsFromCall");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR	
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaDeleteStreamsFromCall(): "
			"Invalid Error parameter passed to function");
#endif			
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaDeleteStreamsFromCall(): "
			"Invalid parameter Object passed to function",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/*
     * Invoke this API to check media state and verify whether an offer
	 * can be initiated or not. It will take care of incrementing the 
	 * version also.
	 */
	if (sdf_ivk_uaInitiateOffer(&pSessionParams, pObject, Sdf_en_offerChange,\
		pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreamsFromCall( ):  "
			"Initiating Offer for Deleting Streams Failed.",pError);
#endif
		return Sdf_co_fail;
	}

	/*
     * Delete the given streams to the SessionParams.
	 */
	if (sdf_ivk_uaDeleteStreams(pSessionParams, pMediaFormats, pslMediaIDs,\
			   pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreamsFromCall( ):  "
			"Deletion of new Streams Failed.",pError);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
    }

	/*
	 * If codec string is given as input parameter, then convert it to list of 
	 * MIDs before updating Local Session.
	 */
	if (pMediaFormats != Sdf_co_null)
	{
		pslMediaIDs = &dslMediaIDs;
		(void)sdf_listInit(pslMediaIDs, __sip_freeString,Sdf_co_false, pError);

		if (sdf_fn_uaConvertCodecListToMID(pSessionParams, pslMediaIDs, \
				pMediaFormats, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
		    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			    (Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreamsFromCall( ):  "
			    "Conversion of CodecList to MID list Failed.",pError);
#endif
		    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
			(void)sdf_listDeleteAll(pslMediaIDs, pError);
		    return Sdf_co_fail;
		}
	}

	if (pObject->pMediaInfo->pCapabilities != Sdf_co_null)
    {
		/* SPR-22 FIX BY Pankaj */
		/* Update the init Data information with new media streams */
		if(sdf_ivk_uaUpdateLocalSession(pObject->pMediaInfo, pslMediaIDs, \
					Sdf_co_false, pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreamsFromCall( ):  "
				"UPDATE Local Session Failed.",pError);
#endif
			(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
			(void)sdf_listDeleteAll(pslMediaIDs, pError);
			return Sdf_co_fail;
		}
	}
	
	/*  Based on the Mids it is not possible to locate the entry to be
	 *  deleted.  */

	if (pMediaFormats != Sdf_co_null)
	{
		/* 
		 * The pslMediaIDs list was initialised here for updating local session
		 * So delete it. 
		 */
		(void)sdf_listDeleteAll(pslMediaIDs, pError);
		pslMediaIDs = Sdf_co_null;
	}

	/*
     * Convert the SessionParams structure into SdpMessage.
	 */
	if (sip_initSdpMessage(&pSdpMessage, (SipError*)&(pError->stkErrCode))\
		     == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaDeleteStreamsFromCall( ): "
			"Failed to allocate memory for the SDP message", pError);
#endif		
	    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		pError->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
    }

	if (sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMessage, pSessionParams,\
				Sdf_en_convertLocal, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreamsFromCall( ):  "
			"Failed to make SdpMessage from SessionParams",pError);
#endif
	    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
    }
	/*Set LastOfferMade*/
	if(sdf_ivk_uaSetLastOfferMade(pObject->pMediaInfo,pSessionParams,pError) \
			==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreamsFromCall( ):  "
			"Failed to Set Last Offer Made",pError);
#endif
		sip_freeSdpMessage(pSdpMessage);
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
	}


	/* Free the local reference. */
	(void)sdf_ivk_uaFreeSessionParam(pSessionParams);

	/*
     * Form the INVITE by invoking sdf_ivk_uaMakeTransaction.
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"INVITE", pObject,\
			   Sdf_co_null, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreamsFromCall( ):  "
			"Failed to make the transaction",pError);
#endif
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
    }

	/*
	 * Form the SipMsgBody with the SdpMessage just constructed.
	 */
	if ((sip_initSipMsgBody(&pSipMsgBody, SipSdpBody,(SipError*)&(\
		pError->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaDeleteStreamsFromCall( ): "
			"Failed to allocate memory for the SIP message body", pError);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		pError->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	if (sip_setSdpInMsgBody(pSipMsgBody, pSdpMessage,\
			   (SipError*)&pError->stkErrCode) == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaDeleteStreamsFromCall( ): "
			"Failed to set the SDP message in the SIP message body", pError);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
    }
	sip_freeSdpMessage(pSdpMessage);

	/*
	 * Delete all existing message bodies and store the SipMsgBody in the
	 * message body list.
	 */
	if (sdf_ivk_uaGetTransactionFromCallObject(pObject,\
			Sdf_en_uacTransaction,&pTransaction, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaDeleteStreamsFromCall( ): "
			"Failed to get UAC transaction from call object", pError);
#endif			
		sip_freeSipMsgBody(pSipMsgBody);
		pError->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	if (sdf_listDeleteAll(&(pTransaction->slMsgBodyList), pError)\
		   == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreamsFromCall( ):  "
			"Failed to delete the current message bodies.",pError);
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}
	
	if (sdf_listAppend(&(pTransaction->slMsgBodyList), \
				 	(Sdf_ty_pvoid)pSipMsgBody, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreamsFromCall( ):  "
			"Failed to append message body to slMsgBodyList",pError);
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
    }

	/*
     * Store the message body list in the SipMessage.
	 */
	if (sdf_ivk_uaSetAllMsgBody(pTransaction, Sdf_co_null, pError)\
		   == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreamsFromCall( ):  "
			"Failed to set sdp into the SipMessage being made.",pError);
#endif
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}

	/* Free the local references here. */
	(void)sdf_ivk_uaFreeTransaction(pTransaction);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "\
			"sdf_ivk_uaDeleteStreamsFromCall");
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaDeleteStreams
 **
 ** DESCRIPTION: This function removes a stream from a call leg, either 
 **				 party sets port to zero in the session description for 
 **				 the media line corresponding to this stream.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaDeleteStreams
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pSessionParams,
	Sdf_ty_s8bit *pMediaFormats,
	Sdf_ty_slist *pslMediaIDs,
	Sdf_st_error *pError)
#else
	(pSessionParams,pMediaFormats,pslMediaIDs,pError)
	Sdf_st_sessionParam *pSessionParams,
	Sdf_ty_s8bit *pMediaFormats;
	Sdf_ty_slist *pslMediaIDs;
	Sdf_st_error *pError;
#endif
{
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaDeleteStreams");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR	
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaDeleteStreams(): "
			"Invalid Error parameter passed to function");
#endif			
		return Sdf_co_fail;
	}
	if (pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaDeleteStreams(): "
			"Invalid Session Params passed to function",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	if (pMediaFormats != Sdf_co_null)
    {
		Sdf_ty_s8bit *pCodecStr = Sdf_co_null;
		Sdf_ty_s8bit *payload = Sdf_co_null;
        Sdf_ty_s8bit dTempCodecList[Sdf_co_characterBufferSize];
		Sdf_ty_s8bit *pTempCodec = Sdf_co_null;

        Sdf_mc_strcpy(dTempCodecList, pMediaFormats);

        pCodecStr = Sdf_mc_strtokr(dTempCodecList," ",&pTempCodec);

		do
        {
			Sdf_st_mediaStream *pMediaStream = Sdf_co_null;
			Sdf_ty_u8bit       dIndex = 0;
			Sdf_ty_u32bit      codecIndex = 0;

			/*if (!sdf_fn_uaIsNumber(pCodecStr))
				payload = sdf_fn_uaMapPayloadFromCodec(pCodecStr, pErr);
			else
				payload = pCodecStr;*/

			payload = pCodecStr;

			for (dIndex = 0; dIndex < (pSessionParams->dNumMediaStreams); dIndex++)
            {
				Sdf_ty_u32bit dCodecListSize = 0;
				Sdf_st_codec *pCodec = Sdf_co_null;
				Sdf_ty_codecName dCodecName;
				Sdf_ty_bool dStreamToBeDeleted = Sdf_co_false;

				if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
							&pMediaStream, dIndex, pError) == Sdf_co_fail)
                {
#ifdef SDF_ERROR
	                sdf_fn_setError(Sdf_en_majorErrors,\
			           	Sdf_en_sessionParamAccessError,\
   				       	(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreams( ):  "
      				    "Failed to get MediaStream at index from SessionParam",\
					   	pError);
#endif
					pError->errCode=Sdf_en_sessionParamAccessError;
                    return Sdf_co_fail;
                }

				(void)sdf_listSizeOf(&(pMediaStream->slCodecs), &dCodecListSize,\
					           pError);

				for (codecIndex = 0; codecIndex < dCodecListSize; codecIndex++)
                {
					if (sdf_listGetAt(&(pMediaStream->slCodecs), codecIndex,\
						      (Sdf_ty_pvoid *)&pCodec, pError) == Sdf_co_fail)
                    {
#ifdef SDF_ERROR
		                sdf_fn_setError(Sdf_en_majorErrors,\
				           	Sdf_en_sdpAccessorError,\
      				       	(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreams( ):  "
	      				    "Failed to get Codec at index from MediaStream",
			        		pError);
#endif
						(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
                        return Sdf_co_fail;
                    }

					/* Get the codec as name (enumeration). */
					dCodecName = \
						sdf_fn_uaMapCodecNumberToName(Sdf_mc_atoi(payload),\
								pError);

					if (dCodecName != Sdf_en_otherCodec)
                    {
                        if (dCodecName == pCodec->dCodecName)
                        {
							dStreamToBeDeleted = Sdf_co_true;
							break;
                        }
                    }
 #ifdef SDF_UAB2B
                else if(dCodecName == Sdf_en_otherCodec)
                    {
						if (Sdf_mc_strcmp(payload,pCodec->pOtherCodecName) == 0)
                        {
							dStreamToBeDeleted = Sdf_co_true;
							break;
                        }
                    }
                    else
                        dStreamToBeDeleted = Sdf_co_false;
 #else
					else
                    {
						if (Sdf_mc_strcmp(payload,pCodec->pOtherCodecName) == 0)
                        {
							dStreamToBeDeleted = Sdf_co_true;
							break;
                        }
                    }
 #endif                   
                } /* End of for loop of dCodecListSize */

				if (dStreamToBeDeleted == Sdf_co_true)
                {
#ifdef SDF_MEDIAGROUP
					/*Remove this Media Stream from the grouping also*/
					(void)sdf_fn_uaRemoveMediaStreamFromGrouping(dIndex,pSessionParams,pError);
#endif
					/* Set the port to 0 to delete the MediaStream. */
					pMediaStream->dPort = 0;
					/* Also set the number of channels to 0. */
					pMediaStream->dNumPort = 0;
                }

				/* Free the local reference */
				(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
            } /* End of for loop of dNumMediaStreams */

			/* 
			 * Pick up the next payload from the input list
			 */
			pCodecStr = Sdf_mc_strtokr(pTempCodec, " ",&pTempCodec);

        } while (pCodecStr != Sdf_co_null);
    }
	else if (pslMediaIDs != Sdf_co_null)
    {
		Sdf_ty_u32bit dMediaIDListSize = 0;
		Sdf_st_mediaStream *pMediaStream = Sdf_co_null;
		Sdf_ty_u32bit       dMediaIDIndex = 0;
	   	Sdf_ty_u8bit		dMediaStreamIndex = 0;
		Sdf_ty_u8bit        dNumMediaStreams = 0;
        Sdf_ty_s8bit        *pMediaStreamId = Sdf_co_null;
		Sdf_ty_bool			dMatch=Sdf_co_false;

		(void)sdf_listSizeOf(pslMediaIDs, &dMediaIDListSize, pError);

		for (dMediaIDIndex = 0; dMediaIDIndex < dMediaIDListSize;\
			   	            dMediaIDIndex++)
        {
			dMatch=Sdf_co_false;
			if (sdf_listGetAt(pslMediaIDs, dMediaIDIndex,\
					    (Sdf_ty_pvoid *)&pMediaStreamId, pError) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
		        sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
      					(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreams( ):  "
	      				"Failed to get MediastreamId at index from slMediaID",
						pError);
#endif
	        	return Sdf_co_fail;
            }

			dNumMediaStreams = pSessionParams->dNumMediaStreams;

			for (dMediaStreamIndex = 0; dMediaStreamIndex < dNumMediaStreams;\
				   dMediaStreamIndex++)
            {
				if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
					&pMediaStream, dMediaStreamIndex, pError) == Sdf_co_fail)
                {
#ifdef SDF_ERROR
	                sdf_fn_setError(Sdf_en_majorErrors,\
			           	Sdf_en_sessionParamAccessError,\
   				       	(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreams( ):  "
      				    "Failed to get MediaStream at index from SessionParam",\
					   	pError);
#endif
					pError->errCode=Sdf_en_sessionParamAccessError;
                    return Sdf_co_fail;
                }

				/* Check if the MediaStreamIds match. */
				if (Sdf_mc_strcmp(pMediaStream->pMediaStreamId, \
							pMediaStreamId) == 0)
                {
#ifdef SDF_MEDIAGROUP
					/*Remove this Media Stream from the grouping also*/
					(void)sdf_fn_uaRemoveMediaStreamFromGrouping(dMediaStreamIndex,\
							pSessionParams,pError);
#endif
					/* Set the port to 0 to delete the MediaStream. */
					pMediaStream->dPort = 0;
					/* Set Num of ports to 0 to delete the MediaStream. */
					pMediaStream->dNumPort = 0;

					/* Free the local reference */
			        (void)sdf_ivk_uaFreeMediaStream(pMediaStream);
					dMatch=Sdf_co_true;
					break;
                }

                /* Free the local reference */
			    (void)sdf_ivk_uaFreeMediaStream(pMediaStream);
            } /* End of for loop of dNumMediaStreams */
			if(dMatch==Sdf_co_false)
			{
#ifdef SDF_ERROR
		        sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
      					(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteStreams( ):  "
	      				"Failed to Match  MediastreamId ",
						pError);
#endif
	        	return Sdf_co_fail;
            }
        }
    }

    sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "\
			"sdf_ivk_uaDeleteStreams");
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaAddStreamsToCall
 **
 ** DESCRIPTION: 	This function is used to add a stream to an existing call 
 **					leg. Either party appends an additional m line to the 
 **					previous negotiated session description when sending 
 **					a Re-Invite
 **
 **					Note: The user MUST call sdf_ivk_uaStartTransaction prior
 **						  to invoking this API.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddStreamsToCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, 
	Sdf_ty_slist slMediaList,
	Sdf_st_error *pError)
#else	
	(pObject, slMediaList,pError)
	Sdf_st_callObject *pObject;
	Sdf_ty_slist slMediaList;
	Sdf_st_error *pError;
#endif
{
	SipMsgBody *pSipMsgBody=Sdf_co_null;
	SdpMessage *pSdpMessage = Sdf_co_null;
	Sdf_st_transaction *pTransaction = Sdf_co_null;
	Sdf_st_sessionParam *pSessionParams = Sdf_co_null;
	Sdf_ty_u32bit		dListSize=0; 
	Sdf_ty_u32bit		dIndex =0;
	Sdf_st_mediaStream *pMediaStream = Sdf_co_null;


	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaAddStreamsToCall");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR	
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaAddStreamsToCall(): "
			"Invalid Error parameter passed to function");
#endif			
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddStreamsToCall(): "
			"Invalid parameter passed to function",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

    /*
     * Invoke this API to check media state and verify whether an offer
	 * can be initiated or not. It will take care of incrementing the 
	 * version also.
	 */
	if (sdf_ivk_uaInitiateOffer(&pSessionParams, pObject, Sdf_en_offerChange,\
		pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreamsToCall( ):  "
			"Initiating Offer for Adding Streams Failed.",pError);
#endif
		return Sdf_co_fail;
	}

	/* Add the mid's to the input streams if they are not present */
	(void)sdf_listSizeOf(&slMediaList, &dListSize, pError);

	for(dIndex=0; dIndex<dListSize; dIndex++)
	{
		/*w534*/(void)sdf_listGetAt(&slMediaList, dIndex, (Sdf_ty_pvoid *)&pMediaStream, \
					pError);
		if( pMediaStream->pMediaStreamId == Sdf_co_null )
			/*w534*/(void)sdf_fn_uaGenerateMid(&(pMediaStream->pMediaStreamId),pError);
	}

	/*
     * Add the given streams to the SessionParams.
	 */
	if (sdf_ivk_uaAddStreams(pSessionParams, slMediaList, pError)==Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreamsToCall( ):  "
			"Addition of new Streams Failed.",pError);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
    }

	if (pObject->pMediaInfo->pCapabilities != Sdf_co_null)
    {
	    /* Update the init Data information with new media streams */
	    if(sdf_ivk_uaUpdateLocalSession(pObject->pMediaInfo, &slMediaList, \
				Sdf_co_true, pError) == Sdf_co_fail)
			{
					(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
					return Sdf_co_fail;
			}
	}
	
	/*
     * Form the INVITE by invoking sdf_ivk_uaMakeTransaction.
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"INVITE", pObject,\
			   	Sdf_co_null, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreamsToCall( ):  "
			"Failed to make the transaction",pError);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
    }

	/*
     * Convert the SessionParams structure into SdpMessage.
	 */
	if (sip_initSdpMessage(&pSdpMessage, (SipError*)&(pError->stkErrCode))\
		     == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddStreamsToCall( ): "
			"Failed to allocate memory for the SDP message", pError);
#endif		
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		pError->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
    }

	if (sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMessage, pSessionParams,\
				Sdf_en_convertLocal, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreamsToCall( ):  "
			"Failed to make SdpMessage from SessionParams",pError);
#endif
		sip_freeSdpMessage(pSdpMessage);
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
    }
	
	/*Set LastOfferMade*/
	if(sdf_ivk_uaSetLastOfferMade(pObject->pMediaInfo,pSessionParams,pError) \
			==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreamsToCall( ):  "
			"Failed to Set Last Offer Made",pError);
#endif
		sip_freeSdpMessage(pSdpMessage);
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
	}
	/* Free the local reference. */
	(void)sdf_ivk_uaFreeSessionParam(pSessionParams);

	/*
	 * Form the SipMsgBody with the SdpMessage just constructed.
	 */
	if ((sip_initSipMsgBody(&pSipMsgBody, SipSdpBody,(SipError*)&(\
		pError->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddStreamsToCall( ): "
			"Failed to allocate memory for the SIP message body", pError);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		pError->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	if (sip_setSdpInMsgBody(pSipMsgBody, pSdpMessage,\
			   (SipError*)&pError->stkErrCode) == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddStreamsToCall( ): "
			"Failed to set the SDP message in the SIP message body", pError);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
    }
	sip_freeSdpMessage(pSdpMessage);
	/*
	 * Delete all existing message bodies and store the SipMsgBody in the
	 * message body list.
	 */
	if (sdf_ivk_uaGetTransactionFromCallObject(pObject,\
			Sdf_en_uacTransaction,&pTransaction, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddStreamsToCall( ): "
			"Failed to get UAC transaction from call object", pError);
#endif			
		sip_freeSipMsgBody(pSipMsgBody);
		pError->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	if (sdf_listDeleteAll(&(pTransaction->slMsgBodyList), pError) == \
			Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreamsToCall( ):  "
			"Failed to delete the current message bodies.",pError);
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}

	if (sdf_listAppend(&(pTransaction->slMsgBodyList), \
				 	(Sdf_ty_pvoid)pSipMsgBody, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreamsToCall( ):  "
			"Failed to append message body to slMsgBodyList",pError);
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
    }

	/*
     * Store the message body list in the SipMessage.
	 */
	if (sdf_ivk_uaSetAllMsgBody(pTransaction, Sdf_co_null, pError) == \
			Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreamsToCall( ):  "
			"Failed to set sdp into the SipMessage being made.",pError);
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}

	/* Free the local reference. */
	(void)sdf_ivk_uaFreeTransaction(pTransaction);

	pError->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaAddStreamsToCall");
   	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaUpdateMediaStreamInCall
 **
 ** DESCRIPTION: 	This function is used to modify the paramaters of an 
 **					Existing Media Stream.
 **
 **					Note: The user MUST call sdf_ivk_uaStartTransaction prior
 **						  to invoking this API.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaUpdateMediaStreamInCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pObject, 
	 Sdf_st_mediaStream	*pMediaStream,
	 Sdf_st_error 		*pError)
#else	
	(pObject, pMediaStream, pError)
	Sdf_st_callObject 	*pObject;
	Sdf_st_mediaStream 	*pMediaStream;
	Sdf_st_error 		*pError;
#endif
{
	SipMsgBody *pSipMsgBody=Sdf_co_null;
	SdpMessage *pSdpMessage = Sdf_co_null;
	Sdf_st_transaction *pTransaction = Sdf_co_null;
	Sdf_st_sessionParam *pSessionParams = Sdf_co_null;
	Sdf_ty_u8bit		dNumMediaStreams = 0;
	Sdf_ty_u8bit		dIndex = 0;
	Sdf_st_mediaStream 	*pLocalMediaStream = Sdf_co_null;
	Sdf_ty_s8bit		*pMediaStreamId = Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaUpdateMediaStreamInCall");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR	
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaUpdateMediaStreamInCall(): "
			"Invalid Error parameter passed to function");
#endif			
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateMediaStreamInCall(): "
			"Invalid parameter passed to function",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
#endif    
#if 0
#ifdef SDF_LINT
	if (pMediaStream == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateMediaStreamInCall(): "
			"Invalid parameter passed to function",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
#endif
#endif	/* End of param validation ifdef */

    /*
     * Invoke this API to check media state and verify whether an offer
	 * can be initiated or not. It will take care of incrementing the 
	 * version also.
	 */
	if (sdf_ivk_uaInitiateOffer(&pSessionParams, pObject, Sdf_en_offerChange,\
		pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateMediaStreamInCall( ):  "
			"Initiating Offer for Adding Streams Failed.",pError);
#endif
		return Sdf_co_fail;
	}

	if( pMediaStream != Sdf_co_null )
		pMediaStreamId	= pMediaStream->pMediaStreamId;

	if( pMediaStreamId != Sdf_co_null )
	{
		dNumMediaStreams = pSessionParams->dNumMediaStreams;
		
		for(dIndex=0; dIndex < dNumMediaStreams; dIndex++)
		{
			if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
					&pLocalMediaStream, dIndex, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_sessionParamAccessError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateMediaStreamInCall( ):  "
					"Failed to get MediaStream at index from SessionParam",\
					pError);
#endif
				pError->errCode=Sdf_en_sessionParamAccessError;
				return Sdf_co_fail;
			}

			if( Sdf_mc_strcmp(pLocalMediaStream->pMediaStreamId, \
							pMediaStreamId) == 0 )
			{
#ifdef SDF_LINT
				if(pMediaStream != Sdf_co_null)
#endif					
						HSS_LOCKEDINCREF(pMediaStream->dRefCount);
				(void)sdf_ivk_uaFreeMediaStream(\
						pSessionParams->pMediaStreams[dIndex]);
				pSessionParams->pMediaStreams[dIndex] = pMediaStream;

				/* Update the Session info available at Init Data also */
				(void)sdf_ivk_uaFreeMediaStream( \
					pObject->pInitData->pSessionParam->pMediaStreams[dIndex]);

#ifdef SDF_LINT
				if(pMediaStream != Sdf_co_null)
#endif					
						HSS_LOCKEDINCREF(pMediaStream->dRefCount);
				pObject->pInitData->pSessionParam->pMediaStreams[dIndex] = \
							pMediaStream;

				(void)sdf_ivk_uaFreeMediaStream(pLocalMediaStream);
				break;
			}
			(void)sdf_ivk_uaFreeMediaStream(pLocalMediaStream);
		}
	}

	/*
     * Form the INVITE by invoking sdf_ivk_uaMakeTransaction.
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"INVITE", pObject,\
			   	Sdf_co_null, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateMediaStreamInCall( ):  "
			"Failed to make the transaction",pError);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
    }

	if( pMediaStream != Sdf_co_null )
	{
		/*
		 * Convert the SessionParams structure into SdpMessage.
		 */
		if (sip_initSdpMessage(&pSdpMessage, (SipError*)&(pError->stkErrCode))\
				 == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateMediaStreamInCall( ): "
				"Failed to allocate memory for the SDP message", pError);
#endif		
			(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
			pError->errCode=Sdf_en_memoryError;
			return Sdf_co_fail;
		}

		if (sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMessage, \
			pSessionParams, Sdf_en_convertLocal, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateMediaStreamInCall( ):  "
				"Failed to make SdpMessage from SessionParams",pError);
#endif
			sip_freeSdpMessage(pSdpMessage);
			(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
			return Sdf_co_fail;
		}
		/*Set LastOfferMade*/
		if(sdf_ivk_uaSetLastOfferMade(pObject->pMediaInfo,pSessionParams, \
					pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateMediaStreamInCall( ):  "
				"Failed to Set Last Offer Made",pError);
#endif
			sip_freeSdpMessage(pSdpMessage);
			(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
			return Sdf_co_fail;
		}

		/* Free the local reference. */
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);

		/*
		 * Form the SipMsgBody with the SdpMessage just constructed.
		 */
		if ((sip_initSipMsgBody(&pSipMsgBody, SipSdpBody,(SipError*)&(\
			pError->stkErrCode))) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateMediaStreamInCall( ): "
				"Failed to allocate memory for the SIP message body", pError);
#endif		
			sip_freeSdpMessage(pSdpMessage);
			pError->errCode=Sdf_en_memoryError;
			return Sdf_co_fail;
		}

		if (sip_setSdpInMsgBody(pSipMsgBody, pSdpMessage,\
				   (SipError*)&pError->stkErrCode) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateMediaStreamInCall( ): "
				"Failed to set the SDP message in the SIP message body", pError);
#endif		
			sip_freeSdpMessage(pSdpMessage);
			sip_freeSipMsgBody(pSipMsgBody);
			pError->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		sip_freeSdpMessage(pSdpMessage);
		/*
		 * Delete all existing message bodies and store the SipMsgBody in the
		 * message body list.
		 */
		if (sdf_ivk_uaGetTransactionFromCallObject(pObject,\
				Sdf_en_uacTransaction,&pTransaction, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateMediaStreamInCall( ): "
				"Failed to get UAC transaction from call object", pError);
#endif			
			sip_freeSipMsgBody(pSipMsgBody);
			pError->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}

		if (sdf_listDeleteAll(&(pTransaction->slMsgBodyList), pError) == \
				Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateMediaStreamInCall( ):  "
				"Failed to delete the current message bodies.",pError);
#endif
			sip_freeSipMsgBody(pSipMsgBody);
			(void)sdf_ivk_uaFreeTransaction(pTransaction);
			return Sdf_co_fail;
		}

		if (sdf_listAppend(&(pTransaction->slMsgBodyList), \
						(Sdf_ty_pvoid)pSipMsgBody, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateMediaStreamInCall( ):  "
				"Failed to append message body to slMsgBodyList",pError);
#endif
			sip_freeSipMsgBody(pSipMsgBody);
			(void)sdf_ivk_uaFreeTransaction(pTransaction);
			return Sdf_co_fail;
		}

		/*
		 * Store the message body list in the SipMessage.
		 */
		if (sdf_ivk_uaSetAllMsgBody(pTransaction, Sdf_co_null, pError) == \
				Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateMediaStreamInCall( ):  "
				"Failed to set sdp into the SipMessage being made.",pError);
#endif
			sip_freeSipMsgBody(pSipMsgBody);
			(void)sdf_ivk_uaFreeTransaction(pTransaction);
			return Sdf_co_fail;
		}
	}
	else
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);


	/* Free the local reference. */
	(void)sdf_ivk_uaFreeTransaction(pTransaction);

	pError->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaUpdateMediaStreamInCall");
   	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaAddStreams
 **
 ** DESCRIPTION: 	This function is used to add streams to a given session 
 **                 params.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddStreams
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pSessionParams, 
	Sdf_ty_slist slMediaList,
	Sdf_st_error *pError)
#else	
	(pSessionParams, slMediaList, pError)
	Sdf_st_sessionParam *pSessionParams;
	Sdf_ty_slist slMediaList;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_u8bit  dNumNewStreams = 0, dNumCurrentStreams = 0, 
	              dNumTotalStreams = 0, dCurrMediaIndex = 0,
				  dNewMediaIndex = 0, dNumOldStreams=0;
    Sdf_ty_u32bit dNumStreamsToBeAdded = 0;
    Sdf_st_mediaStream *pCurrMediaStream = Sdf_co_null;
    Sdf_st_mediaStream *pNewMediaStream = Sdf_co_null;
	Sdf_ty_u8bit       dIndex = 0;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaAddStreams");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR	
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - sdf_ivk_uaAddStreams(): "
			"Invalid Error parameter passed to function");
#endif			
		return Sdf_co_fail;
	}
	if (pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddStreams(): "
			"Invalid SessionParams parameter passed to function",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	(void)sdf_listSizeOf(&(slMediaList), &dNumStreamsToBeAdded, pError);

	dNumNewStreams = dNumStreamsToBeAdded;
	dNumCurrentStreams = pSessionParams->dNumMediaStreams;
	dNumTotalStreams = dNumCurrentStreams + dNumStreamsToBeAdded;

	dNumOldStreams = dNumCurrentStreams;

	/*
     * First iterate throught the present list of MediaStreams and check if
	 * any slot is empty (port=0). If yes, then replace that MediaStream with
	 * the new MediaStream.
	 */
	for (dCurrMediaIndex = 0; dCurrMediaIndex < dNumCurrentStreams; 
			dCurrMediaIndex++)
    {
        if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
				&pCurrMediaStream, dCurrMediaIndex, pError) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sessionParamAccessError,\
   		    	(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreams( ):  "
   		        "Failed to get MediaStream at index from SessionParam",\
		 	    pError);
#endif
			pError->errCode=Sdf_en_sessionParamAccessError;
            return Sdf_co_fail;
        }

		/* Check if the slot is empty. */
		if (pCurrMediaStream->dPort == 0)
        {
			if (sdf_listGetAt(&(slMediaList), dNewMediaIndex,\
				  (Sdf_ty_pvoid *)&pNewMediaStream, pError) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
	            sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
   		    	    (Sdf_ty_s8bit *)"sdf_ivk_uaAddStreams( ):  "
      	            "Failed to get MediaStream at index from slMediaList",\
		 	        pError);
#endif
				(void)sdf_ivk_uaFreeMediaStream(pCurrMediaStream);
                return Sdf_co_fail;
            }

			HSS_LOCKEDINCREF(pNewMediaStream->dRefCount);
			/*
             * We have found an empty slot, so set the new MediaStream here.
			 * Before setting free the previous Media Stream
			 */
		    (void)sdf_ivk_uaFreeMediaStream(\
					pSessionParams->pMediaStreams[dCurrMediaIndex]);
			pSessionParams->pMediaStreams[dCurrMediaIndex] = pNewMediaStream;

			/* Assign a new MID to the new MediaStream here. */
			if (pNewMediaStream->pMediaStreamId == Sdf_co_null)
				/*w534*/(void)sdf_fn_uaGenerateMid(&(pNewMediaStream->pMediaStreamId),pError);

			dNumStreamsToBeAdded--;
			dNumTotalStreams--;
			dNewMediaIndex++;
        }
		(void)sdf_ivk_uaFreeMediaStream(pCurrMediaStream);

		if (dNumStreamsToBeAdded == 0)
			break;
    }

	if (dNumStreamsToBeAdded != 0)
    {
		/*
         * This means that we couldn't fill all the new Mediastreams in the
		 * existing slots. So, take a new array large enough to store all
		 * the media streams and copy the old as well as new streamns to it.
		 */
		Sdf_st_mediaStream **ppAllMediaStreams = Sdf_co_null;

		/* Allocate for array of MediaStreams Pointers */
		ppAllMediaStreams = (Sdf_st_mediaStream **)sdf_memget(Sdf_mc_sdpMemId,\
							 (sizeof(Sdf_st_mediaStream*) * dNumTotalStreams),\
							 pError);

		if (ppAllMediaStreams == Sdf_co_null)
        {
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
				"Failed to Initialize new MediaStreams array.",pError);
#endif
			pError->errCode=Sdf_en_noMemoryError;
			return Sdf_co_fail;
        }

		/* 
         * First iterate through the list of MediaStreams present in 
		 * pSessionParams and assign them to the new array.
		 */
		for (dIndex = 0; dIndex < dNumCurrentStreams; dIndex++)
        {
			if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
				&pCurrMediaStream, dIndex, pError) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_sessionParamAccessError,\
   		        	(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreams( ): Failed "
					"to get MediaStream at index from SessionParam", pError);
#endif
				(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid *)\
		                       &ppAllMediaStreams, pError);
			    pError->errCode=Sdf_en_sessionParamAccessError;
                return Sdf_co_fail;
            }
			/* Set the media stream. */
			ppAllMediaStreams[dIndex] = pCurrMediaStream;
        }

		/*
         * Now iterate through the remaining MediaStreams in slMediaList
		 * (which couldn't get accomodated in the previous array), and assign
		 * them to the new array i.e. add the new streams at the end.
		 */
		for (dIndex = dNewMediaIndex; dIndex < dNumNewStreams; dIndex++)
        {
		    if (sdf_listGetAt(&(slMediaList), dIndex,\
				(Sdf_ty_pvoid *)&pNewMediaStream, pError) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
	            sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
   		        	(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreams( ):  "
      	            "Failed to get MediaStream at index from slMediaList",\
		 	        pError);
#endif
				(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid *)\
		                       &ppAllMediaStreams, pError);
                return Sdf_co_fail;
            }

			HSS_LOCKEDINCREF(pNewMediaStream->dRefCount);

			/* Set the media stream. */
			ppAllMediaStreams[dNumCurrentStreams] = pNewMediaStream;

			/* Assign a new MID to the new MediaStream here. */
			if (pNewMediaStream->pMediaStreamId == Sdf_co_null)
				/*w534*/(void)sdf_fn_uaGenerateMid(&(pNewMediaStream->pMediaStreamId),pError);

			dNumCurrentStreams++;
		}

		/*
         * All streams added. Now set this new array of MediaStreams in the 
		 * SessionParams str and also update the dNumMediaStreams.
		 */
		if( pSessionParams->pMediaStreams != Sdf_co_null )
		{
			for(dIndex = 0; dIndex < dNumOldStreams; dIndex++)
			 (void)sdf_ivk_uaFreeMediaStream((pSessionParams->pMediaStreams[dIndex]));

			(void)sdf_memfree(Sdf_mc_sdpMemId, \
				(Sdf_ty_pvoid *)&(pSessionParams->pMediaStreams), pError);
		}
		
		pSessionParams->pMediaStreams = ppAllMediaStreams;
		pSessionParams->dNumMediaStreams = dNumTotalStreams;
    }

	pError->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaAddStreams");
   	return Sdf_co_success;
}

#ifdef SIP_ISUP
/****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaGetIsupBodyFromSipMessage 
 **
 ** DESCRIPTION: 	This function is used to get an Isup Body From 
 **					SipMessage.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetIsupBodyFromSipMessage
#ifdef ANSI_PROTO
	(SipMessage 	*pMsg, 
	Sdf_ty_u32bit   dIndex,
	Sdf_ty_s8bit    **ppIsupBody,
	Sdf_ty_u32bit 	*pIsupBodyLength,
	Sdf_st_error    *pError)
#else
	(pMsgBody,dIndex,ppIsupBody,pIsupBodyLength,pError)
	SipMessage 	*pMsg; 
	Sdf_ty_u32bit  dIndex;
	Sdf_ty_s8bit**  ppIsupBody;
	Sdf_ty_u32bit 	*pIsupBodyLength;
	Sdf_st_error *pError;
#endif
{
	SipMsgBody *pMsgBody = Sdf_co_null;
	en_SipMsgBodyType msgbodytype;
	IsupMessage *pIsup = Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaGetIsupBodyFromSipMessage(): ");
	
	*ppIsupBody=Sdf_co_null;

	if(sip_getMsgBodyAtIndex(pMsg,&pMsgBody,dIndex, \
		(SipError*)&(pError->stkErrCode))==SipFail)
	{	
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetIsupBodyFromSipMessage(): "
			"Failed to Get Message Body At Index ", pError);
#endif			
		pError->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}

	if(sip_getMsgBodyTypeAtIndex(pMsg,&msgbodytype,dIndex, \
		(SipError*)&(pError->stkErrCode))==SipFail)
	{	
		sip_freeSipMsgBody(pMsgBody);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetIsupBodyFromSipMessage(): "
			"Failed to Get MessageBody Type ", pError);
#endif			
		pError->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}

	/* If the message body at the specified index is not an 
		ISUP body, return Sdf_co_fail */
	if(msgbodytype != SipIsupBody)
	{
		sip_freeSipMsgBody(pMsgBody);
		return Sdf_co_fail;
	}

	if(sip_bcpt_getIsupFromMsgBody(pMsgBody,&pIsup, \
		(SipError*)&(pError->stkErrCode))==SipFail)
	{	
		sip_freeSipMsgBody(pMsgBody);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetIsupBodyFromSipMessage(): "
			"Failed to Get Isup buffer From MessageBody  ", pError);
#endif			
		pError->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}

	if(sip_bcpt_getLengthFromIsupMessage(pIsup,pIsupBodyLength,\
		(SipError*)&(pError->stkErrCode))==SipFail)
	{
		sip_bcpt_freeIsupMessage(pIsup);
		sip_freeSipMsgBody(pMsgBody);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetIsupBodyFromSipMessage(): "
			"Failed to Get Length Of Isup Message  ", pError);
#endif			
		pError->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}

	if(sip_bcpt_getBodyFromIsupMessage(pIsup, ppIsupBody, \
		(SipError*)&(pError->stkErrCode))==SipFail)
	{
		sip_bcpt_freeIsupMessage(pIsup);
		sip_freeSipMsgBody(pMsgBody);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetIsupBodyFromSipMessage(): "
			"Failed to GetBody From IsupMessage  ", pError);
#endif			
		pError->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}
	sip_freeSipMsgBody(pMsgBody);
	sip_bcpt_freeIsupMessage(pIsup);

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaGetIsupBodyFromSipMessage(): ");
	return Sdf_co_success;	
}

#endif
/****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaGetUnknownBodyFromSipMessage
 **
 ** DESCRIPTION: 	This function is used to get a unknown body buffer 
 **					from a SipMessage. 
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetUnknownBodyFromSipMessage
#ifdef ANSI_PROTO
	(SipMessage 	*pMsg, 
	Sdf_ty_u32bit   dIndex,
	Sdf_ty_s8bit    **ppBuffer,
	Sdf_ty_u32bit 	*pBufLength,
	Sdf_st_error    *pError)
#else
	(pMsg,dIndex,ppBuffer,pBufLength,pError)
	SipMessage 	*pMsg; 
	Sdf_ty_u32bit   dIndex;
	Sdf_ty_s8bit**  ppBuffer;
	Sdf_ty_u32bit 	*pBufLength;
	Sdf_st_error *pError;
#endif
{
	SipMsgBody *pMsgBody = Sdf_co_null;
	en_SipMsgBodyType msgbodytype;
	SipUnknownMessage *pUnknownMessage = Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetUnknownBodyFromSipMessage(): ");

	*ppBuffer=Sdf_co_null;
	if(sip_getMsgBodyAtIndex(pMsg,&pMsgBody,dIndex, \
		(SipError*)&(pError->stkErrCode))==SipFail)
	{	
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetUnknownBodyFromSipMessage(): "
			"Failed to Get Message Body At Index ", pError);
#endif			
		pError->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}

	if(sip_getMsgBodyTypeAtIndex(pMsg,&msgbodytype,dIndex, \
		(SipError*)&(pError->stkErrCode))==SipFail)
	{	
		sip_freeSipMsgBody(pMsgBody);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetUnknownBodyFromSipMessage(): "
			"Failed to Get MessageBody Type ", pError);
#endif			
		pError->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}

	/* If the message body at the specified index is not an 
		Unknown body, return Sdf_co_fail */
	if(msgbodytype != SipUnknownBody)
	{
		sip_freeSipMsgBody(pMsgBody);
		return Sdf_co_fail;
	}

	if(sip_getUnknownFromMsgBody(pMsgBody,&pUnknownMessage, \
		(SipError*)&(pError->stkErrCode))==SipFail)
	{
		sip_freeSipMsgBody(pMsgBody);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetUnknownBodyFromSipMessage(): "
			"Failed to Get UnknownMessage From MessageBody  ", pError);
#endif			
		pError->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}

	if(sip_getLengthFromUnknownMessage(pUnknownMessage,pBufLength,\
		(SipError*)&(pError->stkErrCode))==SipFail)
	{
		sip_freeSipMsgBody(pMsgBody);
		sip_freeSipUnknownMessage(pUnknownMessage);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetUnknownBodyFromSipMessage(): "
			"Failed to Get Length Of unknown Message  ", pError);
#endif			
		pError->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}

	if(sip_getBufferFromUnknownMessage(pUnknownMessage, ppBuffer, \
		(SipError*)&(pError->stkErrCode))==SipFail)
	{
		sip_freeSipMsgBody(pMsgBody);
		sip_freeSipUnknownMessage(pUnknownMessage);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetUnknownBodyFromSipMessage(): "
			"Failed to GetBody From UnknownMessage  ", pError);
#endif			
		pError->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}
	sip_freeSipMsgBody(pMsgBody);
	sip_freeSipUnknownMessage(pUnknownMessage);

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaGetUnknownBodyFromSipMessage(): ");
	return Sdf_co_success;	
}


/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaCreateMessageBodyFromBuffer 
 **
 ** DESCRIPTION: This function creates a message body from the buffer passed.
 **				 Note that this API should not be used to set MIME, SDP or 
 **				 application/sip bodies. The API should be used to set ISUP 
 **				 bodies and other buffers like text/plain, application/mgcp, 
 **				 etc.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCreateMessageBodyFromBuffer
#ifdef ANSI_PROTO
	(SipMsgBody 	**ppMsgBody, 
	Sdf_ty_s8bit	*pContentType,
	Sdf_ty_s8bit	*pBuffer,
	Sdf_ty_u32bit 	pBufLength,
	Sdf_ty_bool		dAddMimeHdr,
	Sdf_st_error 	*pError)
#else
	(ppMsgBody,pContentType,pBuffer,pBufLength,dAddMimeHdr,pError)
	SipMsgBody 		**ppMsgBody; 
	Sdf_ty_s8bit	*pContentType;
	Sdf_ty_s8bit	*pBuffer;
	Sdf_ty_u32bit 	pBufLength;
	Sdf_ty_bool		dAddMimeHdr;
	Sdf_st_error 	*pError;
#endif
{
#ifdef SIP_ISUP
	IsupMessage *pIsupMessage = Sdf_co_null;
#endif	
	
	SipUnknownMessage *pUnknownMessage = Sdf_co_null;
#ifdef SIP_MIME_PARSING	
	SipMimeHeader *pMimeHdr = Sdf_co_null; 
	SipHeader *pHeader = Sdf_co_null;
	Sdf_ty_s8bit	*pTmpVal = Sdf_co_null;
#endif


	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaCreateMessageBodyFromBuffer(): ");

	/*
	 * If the Content-Type is "application/isup", use bcpt ISUP API's
	 * to construct the MessageBody. Else use the UnknownMessageBody API's
	 */
	*ppMsgBody=Sdf_co_null;
#ifdef SIP_ISUP	
	if (Sdf_mc_strcasecmp(pContentType, "application/isup") == 0)
	{
		if (sip_initSipMsgBody(ppMsgBody, SipIsupBody, (SipError*)\
			&(pError->stkErrCode)) ==  SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to initialize a SipMessage Body structure", pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}

		if(sip_bcpt_initIsupMessage(&pIsupMessage,(SipError*)&(\
			pError->stkErrCode)) ==SipFail)
		{
			sip_freeSipMsgBody(*ppMsgBody);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to initialize a Isup Structure body structure", \
				pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}

		if(sip_bcpt_setBodyInIsupMessage(pIsupMessage,(Sdf_ty_s8bit*)pBuffer,\
			(Sdf_ty_u32bit)pBufLength,\
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			sip_freeSipMsgBody(*ppMsgBody);
			sip_bcpt_freeIsupMessage(pIsupMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to Set Value in Isup Structure", pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}

		if(sip_bcpt_setIsupInMsgBody(*ppMsgBody,pIsupMessage,\
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			sip_freeSipMsgBody(*ppMsgBody);
			sip_bcpt_freeIsupMessage(pIsupMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to set ISUP message in a Sip Message Body ", pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}
		/* Free the local reference */
		sip_bcpt_freeIsupMessage(pIsupMessage);
	}
	else
#endif	
	{
		if (sip_initSipMsgBody(ppMsgBody, SipUnknownBody, (SipError*)\
			&(pError->stkErrCode)) ==  SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to initialize a SipMessage Body structure", pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}

		if (sip_initSipUnknownMessage(&pUnknownMessage, \
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			sip_freeSipMsgBody(*ppMsgBody);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to initialize a UnknownMessagebody structure", \
				pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}

		if(sip_setBufferInUnknownMessage(pUnknownMessage, \
			(Sdf_ty_s8bit*)pBuffer, (Sdf_ty_u32bit)pBufLength,\
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			sip_freeSipMsgBody(*ppMsgBody);
			sip_freeSipUnknownMessage(pUnknownMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to Set Value in UnknownMessagebody Structure", \
				pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}

		if(sip_setUnknownInMsgBody(*ppMsgBody,pUnknownMessage,\
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			sip_freeSipMsgBody(*ppMsgBody);
			sip_freeSipUnknownMessage(pUnknownMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to set Unknown message in a Sip Message Body ", \
				pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}
		sip_freeSipUnknownMessage(pUnknownMessage);
	}

	/* Add mime header only if this has been set to Sdf_co_true */
#ifdef SIP_MIME_PARSING
	if (dAddMimeHdr == Sdf_co_true)
	{
		if(sip_bcpt_initSipMimeHeader(&pMimeHdr,\
			(SipError*)&(pError->stkErrCode))==SipFail )
		{
			sip_freeSipMsgBody(*ppMsgBody);
			if (Sdf_mc_strcasecmp(pContentType, "application/isup") == 0)
				sip_bcpt_freeIsupMessage(pIsupMessage);
			else
				sip_freeSipUnknownMessage(pUnknownMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to Initialize a Mime Header ", pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}     	

		if(sip_initSipHeader(&pHeader,SipHdrTypeContentType,\
			(SipError*)&(pError->stkErrCode))==SipFail)
     	{
			sip_freeSipMsgBody(*ppMsgBody);
			if (Sdf_mc_strcasecmp(pContentType, "application/isup") == 0)
				sip_bcpt_freeIsupMessage(pIsupMessage);
			else
				sip_freeSipUnknownMessage(pUnknownMessage);
			sip_bcpt_freeSipMimeHeader(pMimeHdr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to Initialize a ContentType Header ", pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}     	
          	
		pTmpVal = Sdf_mc_strdupCallHandler(pContentType);

		if(sip_setMediaTypeInContentTypeHdr(pHeader, pTmpVal, \
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			sip_freeSipMsgBody(*ppMsgBody);
			if (Sdf_mc_strcasecmp(pContentType, "application/isup") == 0)
				sip_bcpt_freeIsupMessage(pIsupMessage);
			else
				sip_freeSipUnknownMessage(pUnknownMessage);
			sip_bcpt_freeSipMimeHeader(pMimeHdr);
			sip_freeSipHeader(pHeader);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to Set Media Type in  Content Type Header ", pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}

		if(sip_bcpt_setContentTypeInMimeHdr(pMimeHdr,pHeader,\
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			sip_freeSipMsgBody(*ppMsgBody);
			if (Sdf_mc_strcasecmp(pContentType, "application/isup") == 0)
				sip_bcpt_freeIsupMessage(pIsupMessage);
			else
				sip_freeSipUnknownMessage(pUnknownMessage);
			sip_bcpt_freeSipMimeHeader(pMimeHdr);
			sip_freeSipHeader(pHeader);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to Set Content Type in Mime Header ", pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}

		if(sip_bcpt_setMimeHeaderInMsgBody(*ppMsgBody,pMimeHdr,\
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			sip_freeSipMsgBody(*ppMsgBody);
			if (Sdf_mc_strcasecmp(pContentType, "application/isup") == 0)
				sip_bcpt_freeIsupMessage(pIsupMessage);
			else
				sip_freeSipUnknownMessage(pUnknownMessage);
			sip_bcpt_freeSipMimeHeader(pMimeHdr);
			sip_freeSipHeader(pHeader);

#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaCreateMessageBodyFromBuffer(): "
				"Failed to set Mime Header in Message Body ", pError);
#endif				
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
    	}
		/* Free the local reference */
		sip_bcpt_freeSipMimeHeader(pMimeHdr);
		sip_freeSipHeader(pHeader);
	}
#else
	(void)dAddMimeHdr;
	(void)(*pContentType);
#endif /* SIP_MIME_PARSING */	

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaCreateMessageBodyFromBuffer()");
	return Sdf_co_success;	
}



/*********************************************************
** FUNCTION: sdf_ivk_uaStoreSdpBodyAsSessionSdp
**
** DESCRIPTION: This function stores the sdp as the SessionSdp by storing
**				it in the pCurrentSession.
** 
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaStoreSdpBodyAsSessionSdp
#ifdef ANSI_PROTO
	(Sdf_st_mediaHandling *pMediaInfo,
	SdpMessage *pSdpMsg, 
	Sdf_st_error *pError)
#else	
	(pMediaInfo,pSdpMsg,pError)
	Sdf_st_mediaHandling *pMediaInfo;
	SdpMessage *pSdpMsg; 
	Sdf_st_error *pError;
#endif
{
	Sdf_st_sessionParam  *pSessionParam = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaStoreSdpBodyAsSessionSdp");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - \
		sdf_ivk_uaStoreSdpBodyAsSessionSdp: \
		Invalid Error parameter passed to function");
		return Sdf_co_fail;
	}
	if (pSdpMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
		(Sdf_ty_s8bit*)"sdf_ivk_uaStoreSdpBodyAsSessionSdp: "
		"Invalid parameter passed to function",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
	if (pMediaInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
		(Sdf_ty_s8bit*)"sdf_ivk_uaStoreSdpBodyAsSessionSdp: "
		"Invalid Media-Info param passed to function",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}

#endif	

	if(sdf_ivk_uaInitSessionParam(&pSessionParam, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaStoreSdpBodyAsSessionSdp (): "
			"Could not init the Sdf_st_sessionParam structure.",pError);
#endif				
		return Sdf_co_fail;
	}

    if(sdf_ivk_uaFormSessionParam(pSdpMsg,  pSessionParam, \
		 Sdf_en_convertLocal, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaStoreSdpBodyAsSessionSdp (): "
			"Could not convert the SdpMessage to SessionParam.",pError);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pSessionParam);
		return Sdf_co_fail; 
	}

	if(sdf_ivk_uaSetCurrentSession(pMediaInfo, pSessionParam, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaStoreSdpBodyAsSessionSdp (): "
			"Could not set Current Session.",pError);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pSessionParam);
		return Sdf_co_fail; 
	}

	/* Free the local reference. */
	(void)sdf_ivk_uaFreeSessionParam(pSessionParam);

	pError->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaStoreSdpBodyAsSessionSdp ");
   	return Sdf_co_success;
}



/******************************************************************
**
** FUNCTION:  sdf_ivk_uaCompareSessionParams
**
** DESCRIPTION: This API compares two session level parameters
**              and provides the negotiated Session parameter 
**              and the list of media lines that were acceptable.   
******************************************************************/
Sdf_ty_matchMediaResult sdf_ivk_uaCompareSessionParams
#ifdef ANSI_PROTO
   (Sdf_st_sessionParam *pSessionParamNew,\
   Sdf_st_sessionParam *pSessionParamOld,\
   Sdf_st_mediaHandling *pMediaInfo, \
   Sdf_st_sessionParam **ppNegotiatedSessionParam,\
   Sdf_ty_slist *slAcceptedMedia,
#ifdef SDF_SESSIONCHANGE
   Sdf_st_sessionChange **ppSessionChange, Sdf_ty_options dOptions,
#endif
   Sdf_st_error *pError)
#else
   (pSessionParamNew,pSessionParamOld, pMediaInfo,\
    ppNegotiatedSessionParam, slAcceptedMedia,
#ifdef SDF_SESSIONCHANGE
    ppSessionChange, dOptions,
#endif
	pError)
   Sdf_st_sessionParam *pSessionParamNew;\
   Sdf_st_sessionParam *pSessionParamOld;\
   Sdf_st_mediaHandling *pMediaInfo;\
   Sdf_st_sessionParam **ppNegotiatedSessionParam;\
   Sdf_ty_slist *slAcceptedMedia;
#ifdef SDF_SESSIONCHANGE
   Sdf_st_sessionChange **ppSessionChange;
   Sdf_ty_options dOptions;
#endif
   Sdf_st_error *pError;
#endif
{
	Sdf_ty_matchMediaResult dRetVal;

#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) || (pMediaInfo == Sdf_co_null) ||
		(pSessionParamOld == Sdf_co_null) || (pSessionParamNew == Sdf_co_null)
		|| (slAcceptedMedia == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaCompareSessionParams( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_en_matchingFailed; 
	}
#endif	/* End of param validation ifdef */

#ifdef SDF_SESSIONCHANGE
	if(dOptions & SDF_OPT_COMPARE_SESSION)
	{
		if(sdf_ivk_uaInitSessionChange(ppSessionChange,
				pError) == Sdf_co_fail)
		{
			return Sdf_en_matchingFailed;
		}
	}
#endif

	/* 
	 * If the version number is the same then no need to do the
	 * negotiation.. just return the current session it self. This 
	 * may be a refresh request
	 */
	if((pMediaInfo->dMediaState == Sdf_en_offerChangeRecv) && \
       (sdf_fn_uaIsSameVersion(pSessionParamNew, pSessionParamOld, pError) \
                                                   == Sdf_co_success))
	{
		/*Get the Current Session*/
		if(sdf_ivk_uaGetSessionParamsFromMediaInfo(pMediaInfo,\
				ppNegotiatedSessionParam,Sdf_en_currentSession,\
					pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCompareSessionParam(): "
				"Could not Current Session.",pError);
#endif				
			return Sdf_en_matchingFailed; 
		}
		if(sdf_fn_uaFormMediaListFromSessionParam(*ppNegotiatedSessionParam, 
				slAcceptedMedia,pError) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeSessionParam(*ppNegotiatedSessionParam);
			return Sdf_en_matchingFailed;
		}

#ifdef SDF_SESSIONCHANGE
		if(dOptions & SDF_OPT_COMPARE_SESSION)
		{
			(*ppSessionChange)->dSessionChange = Sdf_co_false;
		}
#endif
		/* No need to compute the peer session */ 
		return Sdf_en_matchingSuccessful;
	}

	if(sdf_ivk_uaInitSessionParam(ppNegotiatedSessionParam, pError) == \
			Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCompareSessionParam(): "
			"Could not init the SessionParam structure.",pError);
#endif				
		return Sdf_en_matchingFailed; 
	}

	/*
	 * Clone the recvd Session Param into  the negotiated session param
	 * From here onwards all the comparison will happen with the negotiated
	 * session param only and the information will be directly updated on
	 * this
	 */
	if (sdf_fn_uaCloneSessionParamWithoutMediaGroup(*ppNegotiatedSessionParam, \
		pSessionParamNew, pError) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeSessionParam(*ppNegotiatedSessionParam);
		*ppNegotiatedSessionParam = Sdf_co_null;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaCompareSessionParams( ): "
			"Failed to clone SessionParam", \
			pError);
#endif			
		pError->errCode=Sdf_en_sdpError;
		return  Sdf_en_matchingFailed;
	}

#ifdef SDF_SESSIONCHANGE
	if(dOptions & SDF_OPT_COMPARE_SESSION)
	{
		Sdf_ty_u32bit i;
		/* Now allocate the media change array */
		(*ppSessionChange)->pMediaChange = (Sdf_st_mediaChange *)\
			sdf_memget (Sdf_mc_initMemId,\
			(sizeof(Sdf_st_mediaChange))*\
			(*ppNegotiatedSessionParam)->dNumMediaStreams, 
			pError);

		if((*ppSessionChange)->pMediaChange == Sdf_co_null)
		{
			(void)sdf_ivk_uaFreeSessionParam(*ppNegotiatedSessionParam);
			*ppNegotiatedSessionParam = Sdf_co_null;
			return Sdf_en_matchingFailed;
		}
		/* Initialize the media change array */
		for(i=0; i< (*ppNegotiatedSessionParam)->dNumMediaStreams ; i++)
		{
			if(sdf_ivk_uaInitMediaChange(\
				&((*ppSessionChange)->pMediaChange[i]), pError) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeSessionParam(*ppNegotiatedSessionParam);
				*ppNegotiatedSessionParam = Sdf_co_null;
				return Sdf_en_matchingFailed;
			}
		}

		if(Sdf_fn_uaCheckSessionForChange
				(*ppNegotiatedSessionParam,
			      pSessionParamOld,*ppSessionChange,pError) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeSessionParam(*ppNegotiatedSessionParam);
			*ppNegotiatedSessionParam = Sdf_co_null;
			return Sdf_en_matchingFailed;
		}
	}

	/* Grouping is not handled for session change */
	if(!(dOptions & SDF_OPT_COMPARE_SESSION))
	{
#endif
#ifdef SDF_MEDIAGROUP
		/* Handle the grouping of the media stream if any */
		if(sdf_fn_uaHandleGroupInSessionParam(pSessionParamNew, \
			pSessionParamOld,\
		   *ppNegotiatedSessionParam, pMediaInfo, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			if(pError->errCode == Sdf_en_mediaGroupError)
			{
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaCompareSessionParams( ): "
					"Error while Processing Group Attribute", \
					pError);
			}		
#endif	
			(void)sdf_ivk_uaFreeSessionParam(*ppNegotiatedSessionParam);
			*ppNegotiatedSessionParam = Sdf_co_null;
			return Sdf_en_matchingFailed;
		}
#endif
#ifdef SDF_SESSIONCHANGE
	}
#endif

	/* Now validate the session level direction attribute */
	if(sdf_fn_uaCompareDirectionAttrib(\
		(*ppNegotiatedSessionParam)->dDirectionAttrib,
		pSessionParamOld->dDirectionAttrib, 
#ifdef SDF_SESSIONCHANGE
		(*ppSessionChange), Sdf_co_null, dOptions,
#endif
		pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaCompareSessionParams( ): "
			"Error while Processing Session Level Direction Attribute", \
			pError);
#endif	
        (void)sdf_ivk_uaFreeSessionParam(*ppNegotiatedSessionParam);
		*ppNegotiatedSessionParam = Sdf_co_null;
		return Sdf_en_matchingFailed;
	}

	/* Special Case:- If the peer is provided
	 * with sendrecv and our direction is send
	 * only/recvonly then change the direction
	 * accordingly i.e. recvonly/sendonly
	 * So that while changing the direction for the ngtd 
	 * session the peers view and the ngtd will be
	 * in sync.
	 * e.g 
	 * Recvd:- sendrecv; Local:-recvonly
	 * here it will be updated as sendonly and after
	 * the ngtn the ngtd session will be having
	 * recvonly and the peers view will be having
	 * sendonly
	 *
	 * In the case of inactive in either side (from the
	 * peer or local the resultant ngtd as well as the 
	 * peers view shall be inactive.
	 */
	if((*ppNegotiatedSessionParam)->dDirectionAttrib \
					== Sdf_en_sendrecv)
	{
		if(pSessionParamOld->dDirectionAttrib \
						== Sdf_en_sendonly)
		{
			(*ppNegotiatedSessionParam)->dDirectionAttrib
						= Sdf_en_recvonly;
		}
		else if(pSessionParamOld->dDirectionAttrib \
						== Sdf_en_recvonly)
		{
			(*ppNegotiatedSessionParam)->dDirectionAttrib
						= Sdf_en_sendonly;
		}
	}
	else if(pSessionParamOld->dDirectionAttrib \
					== Sdf_en_inactive)
	{
		(*ppNegotiatedSessionParam)->dDirectionAttrib
					= Sdf_en_inactive;
	}

	/* Compare the media streams */
	dRetVal = sdf_fn_uaCompareMediaStreams(*ppNegotiatedSessionParam,
						pSessionParamOld,pMediaInfo,slAcceptedMedia,
#ifdef SDF_SESSIONCHANGE
						(*ppSessionChange),dOptions,
#endif
						pError);

#ifdef SDF_SESSIONCHANGE
	/* 
	 * Update the peer session if the option has chosen
	 */
	if(dOptions & SDF_OPT_COMPUTE_PEER_SESSION)
	{
		Sdf_ty_u32bit dIndex = 0, dSize = 0;
		Sdf_st_mediaStream *pMediaStream = Sdf_co_null;

		/* If peer session need to be computed -- update the same */
		if(pMediaInfo->pPeersSession != Sdf_co_null)
		{
			(void)sdf_ivk_uaFreeSessionParam(pMediaInfo->pPeersSession);
		}
		if(sdf_ivk_uaInitSessionParam(&(pMediaInfo->pPeersSession), 
				pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCompareSessionParam(): "
				"Could not init the SessionParam structure.",pError);
#endif				
			dRetVal = Sdf_en_matchingFailed;
			pError->errCode=Sdf_en_sdpError;
		}
		if (sdf_fn_uaCloneSessionParamWithoutMediaStream(\
				pMediaInfo->pPeersSession, \
				*ppNegotiatedSessionParam, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaCompareSessionParams( ): "
				"Failed to clone SessionParam", \
				pError);
#endif			
			dRetVal = Sdf_en_matchingFailed;
			pError->errCode=Sdf_en_sdpError;
		}

		/* 
		 * Now Update the peer session with the accepted media
		 * streams ... which provides the peers view of the current
		 * session
		 */
		if (sdf_fn_uaUpdateMediaStreamsInSessionParam(\
				pMediaInfo->pPeersSession, \
				slAcceptedMedia, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaCompareSessionParams( ): "
				"Failed to clone SessionParam", \
				pError);
#endif			
			dRetVal = Sdf_en_matchingFailed;
			pError->errCode=Sdf_en_sdpError;
		}

		/* 
		 * Now remove the rejected streams from the accepted
		 * media stream list
		 */
		(void)sdf_listSizeOf(slAcceptedMedia, &dSize,pError);
		for(dIndex = 0; dIndex < dSize; dIndex++)
		{
			pMediaStream = Sdf_co_null;
			(void)sdf_listGetAt(slAcceptedMedia, dIndex, (Sdf_ty_pvoid*)&pMediaStream, pError);
			if(pMediaStream->dPort == 0)
			{
				(void)sdf_listDeleteAt(slAcceptedMedia, dIndex, pError);
				dIndex--;
				dSize--;
			}

		}
	}
#endif

	if((dRetVal !=  Sdf_en_matchingFailed) && (dRetVal != Sdf_en_noMatchFound))
	{
		/* Update the Session level direction accordingly */
		if(sdf_fn_uaChangeDirectionAttrib(\
			&(*ppNegotiatedSessionParam)->dDirectionAttrib,
			pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaCompareMediaStreams( ): "
				"Error while changing Session Level Direction Attribute", \
				pError);
#endif	
   			(void)sdf_ivk_uaFreeSessionParam(*ppNegotiatedSessionParam);
			*ppNegotiatedSessionParam = Sdf_co_null;
			return Sdf_en_matchingFailed;
		}
		/*
		 * Update the local parameters in the Negotiated SessionParam
	 	 */
		if(Sdf_fn_uaUpdateLocalPropertiesInSessionParam(pMediaInfo, \
			*ppNegotiatedSessionParam, pSessionParamOld, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaCompareMediaStreams( ): "
				"Error while updating Session Level Direction Attribute", \
				pError);
#endif	
   			(void)sdf_ivk_uaFreeSessionParam(*ppNegotiatedSessionParam);
			*ppNegotiatedSessionParam = Sdf_co_null;
			return Sdf_en_matchingFailed;
		}
	}
	/*Now Check for the Return Value and Increment Version*/
	if((dRetVal == Sdf_en_matchingSuccessful) &&(
		(pMediaInfo->dMediaState == Sdf_en_offerRecv) ||
		(pMediaInfo->dMediaState == Sdf_en_offerChangeRecv)))
			
	{
		(*ppNegotiatedSessionParam)->dLocalSessionVersion++;
		pMediaInfo->pCapabilities->dLocalSessionVersion++;
	}
	if(dRetVal ==  Sdf_en_matchingFailed)
	{
		(void)sdf_ivk_uaFreeSessionParam(*ppNegotiatedSessionParam);
		*ppNegotiatedSessionParam = Sdf_co_null;
		return dRetVal;
	}
	return dRetVal;
}


/****************************************************************************
** FUNCTION: 	 sdf_ivk_uaHandleMedia
**
** DESCRIPTION: Top level abstraction for handling the media(SDP 
**              offer/answer). This API checks the sdp and media 
**              state and based on the media state it negotiates 
**              the recvd sdp with the existing capabilities; 
**              and provides the negotiated Session parameter 
**              and the list of media lines that were acceptable.
**
*****************************************************************************/
Sdf_ty_matchMediaResult sdf_ivk_uaHandleMedia 
#ifdef ANSI_PROTO
   (Sdf_st_mediaHandling *pMediaInfo,Sdf_st_overlapTransInfo* pOverlapTransInfo,
	SipMessage *pRecdSipMessage, Sdf_ty_slist *slAcceptedMedia,
	Sdf_st_sessionParam  **ppNgtdSession, 
#ifdef SDF_SESSIONCHANGE
	Sdf_st_sessionChange **ppSessionChange,Sdf_ty_options dOptions,
#endif	
	Sdf_st_error *pError)
#else
   (pMediaInfo, pOverlapTransInfo, pRecdSipMessage, slAcceptedMedia, 
    ppNgtdSession, 
#ifdef SDF_SESSIONCHANGE
	ppSessionChange,dOptions, 
#endif	
	Sdf_st_error *pError)
   Sdf_st_mediaHandling *pMediaInfo;
   Sdf_st_overlapTransInfo* pOverlapTransInfo,
   SipMessage *pRecdSipMessage;
   Sdf_ty_slist *slAcceptedMedia;
   Sdf_st_sessionParam  **ppNgtdSession;
#ifdef SDF_SESSIONCHANGE
   Sdf_st_sessionChange **ppSessionChange;
   Sdf_ty_options dOptions;
#endif	
   Sdf_st_error *pError;
#endif	
{	
	Sdf_st_sessionParam  	*pRecvdSessionParam = Sdf_co_null;
	Sdf_st_sessionParam  	*pSessionParamLocal = Sdf_co_null;
	Sdf_ty_matchMediaResult dRetVal;
#ifdef SDF_SESSIONCHANGE
	Sdf_st_sessionChange *pSessionChange = Sdf_co_null;
	Sdf_st_sessionParam  	*pSessionParamNgtd = Sdf_co_null;
	Sdf_ty_slist            slMediaStream;
#endif
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaHandleMedia");

	(void)pOverlapTransInfo;

#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null)|| \
		(ppNgtdSession == Sdf_co_null)||(slAcceptedMedia == Sdf_co_null)|| \
		(pRecdSipMessage == Sdf_co_null)||(pMediaInfo == Sdf_co_null) 
#ifdef SDF_SESSIONCHANGE
		||(ppSessionChange == Sdf_co_null)
#endif
		)
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaHandleMedia( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_en_matchingNotDone; 
	}
#endif	/* End of param validation ifdef */

    /* 
	 * If the media state change has occured then proceed with the ngtn
	 * else return from here
	 */
	if(pMediaInfo->dMediaStateChange == Sdf_en_noMediaChange)
	{
#ifdef SDF_ERROR				
		sdf_fn_debug((Sdf_ty_s8bit *)\
			"SDF_DEBUG: No media state transition occured"
			" nothing need to be handled for media");
#endif
		pError->errCode = Sdf_en_noUaError;
		return Sdf_en_noMediaStateTransition;
	}

	if(sdf_ivk_uaGetSessionParamFromSipMsg(pRecdSipMessage, \
		&pRecvdSessionParam, Sdf_en_convertRemote,pError) == Sdf_co_fail)
	{

#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleMedia(): "
				"Failed to get session param from Message",pError);
#endif
		return Sdf_en_matchingNotDone; 
	}

	/* 
	 * Update the LastOffer Recvd field of the media capabilities structure 
	 * if it is an offer
	 */
	if((pMediaInfo->dMediaState == Sdf_en_offerRecv) ||
	   (pMediaInfo->dMediaState == Sdf_en_offerChangeRecv))
	{
		if(sdf_ivk_uaSetLastOfferRecvd(pMediaInfo,
			pRecvdSessionParam, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleMedia(): "
				"Could not get the SessionParam from the sip message.",pError);
#endif
			(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);
			return Sdf_en_matchingNotDone; 
		}
	}

    if(sdf_fn_uaGetSessionParamForNgtn(pMediaInfo,&pSessionParamLocal, pError)
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleMedia(): "
			"Could not get the local SessionParam.",pError);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);
		return Sdf_en_matchingNotDone; 
	}

	/* If the application wants to know about the session change then check
	 * whether we have a PeerSession in this callobject's
	 * mediacapabilities. If present then SessionChange need to be
	 * calculated by comparing the existing PeerSession else return NULL
	 */
#ifdef SDF_SESSIONCHANGE
	if(dOptions &  SDF_OPT_COMPARE_SESSION)
	{
		Sdf_ty_bool dComputePeerSession = Sdf_co_false;
		if(pMediaInfo->pPeersSession != Sdf_co_null)
		{
			if(sdf_listInit(&slMediaStream,__sdf_fn_uaFreeMediaStream,\
				Sdf_co_false,pError) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);
				(void)sdf_ivk_uaFreeSessionParam(pSessionParamLocal);
				return  Sdf_en_matchingNotDone;
			}

			/*
			 * Peer session shall be computed only when we do
			 * the negotiation not here. Here we are just looking
			 * at the difference between Peer's previous view and the
			 * current one. So unmask the dOption if it is set to compute
			 * the peer's view of the session. After this comparison we'll
			 * mask it and it 'll be calculated while doing the ngtn.
			 */
			if(dOptions & SDF_OPT_COMPUTE_PEER_SESSION)
			{
				dComputePeerSession = Sdf_co_true;
				dOptions ^= SDF_OPT_COMPUTE_PEER_SESSION;
			}
			
			if(sdf_ivk_uaCompareSessionParams(pRecvdSessionParam, \
							pMediaInfo->pPeersSession, pMediaInfo,\
							&pSessionParamNgtd,\
							&slMediaStream, ppSessionChange, dOptions,\
	                        pError) == Sdf_en_matchingFailed)
			{
				if((pError->errCode == Sdf_en_noMemoryError) || 
					(pError->errCode == Sdf_en_invalidParamError))
				{
					(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);
					(void)sdf_ivk_uaFreeSessionParam(pSessionParamLocal);
					return Sdf_en_matchingFailed;
				}
			}
			/* 
			 * Here we are only interested in the session change - so free
			 * the remaining output parameter
			 */
			(void)sdf_ivk_uaFreeSessionParam(pSessionParamNgtd);
			(void)sdf_listDeleteAll(&slMediaStream, pError);

			/* 
			 * Now mask the options flag if it was set to compute peer's
			 * session
			 */
			if(dComputePeerSession == Sdf_co_true)
			{
				dOptions |= SDF_OPT_COMPUTE_PEER_SESSION;
			}
		}
		else
		{
			*ppSessionChange = Sdf_co_null;
		}
		/* 
		 * If we reach here then Session comparison has already been
		 * or the peers session is not available to see the difference.
		 * In both the cases we need to reset the SDF_OPT_COMPARE_SESSION 
		 * mask from the dOptions and proceed with the negotiation
		 */
		dOptions ^= SDF_OPT_COMPARE_SESSION;
	}
#endif

    /* Now do the negotiation with the session params */
	dRetVal = sdf_ivk_uaCompareSessionParams(pRecvdSessionParam, \
							pSessionParamLocal, pMediaInfo, ppNgtdSession,\
							slAcceptedMedia,
#ifdef SDF_SESSIONCHANGE
							&pSessionChange, dOptions,
#endif
	                        pError);

#ifdef SDF_TRACE
	/*  Print the m= lines in the negotiated message
		being returned in case of detailed trace */
	(void)sdf_fn_uaTraceSdpMessageInSessionParam(Sdf_co_null, *ppNgtdSession, \
		(Sdf_ty_s8bit *)"negotiated", pError);
#endif

	/* 
	 * Release the local references
	 */
	(void)sdf_ivk_uaFreeSessionParam(pSessionParamLocal);
	(void)sdf_ivk_uaFreeSessionParam(pRecvdSessionParam);

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaHandleMedia");
	return dRetVal;
}

/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetLastOfferRecvd
**
** DESCRIPTION: This function sets the LastOffer received from the peer
**				in the media capabilities of the Call Object
**
** PARAMETERS:
**		pMediaInfo(IN/OUT)	: The Media Info structure
**		pLastOffer(IN)	: The offer recvd from the peer (sdf_st_sessionParam)
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetLastOfferRecvd
#ifdef ANSI_PROTO
 	(Sdf_st_mediaHandling *pMediaInfo, Sdf_st_sessionParam *pLastOffer,\
	Sdf_st_error *pError)
#else
	(pMediaInfo, pLastOffer, pError)
	Sdf_st_mediaHandling *pMediaInfo;
	Sdf_st_sessionParam pLastOffer;
	Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null)||(pLastOffer == Sdf_co_null)|| \
		(pMediaInfo == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetLastOfferRecvd():"
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */
	
	if(pMediaInfo->pLastOfferRecd != Sdf_co_null)
	{
		(void)sdf_ivk_uaFreeSessionParam(pMediaInfo->pLastOfferRecd);
	}
	pMediaInfo->pLastOfferRecd = pLastOffer;
	HSS_LOCKEDINCREF(pLastOffer->dRefCount);
    pError->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
        "Exiting sdf_ivk_uaSetLastOfferRecvd():");
    return Sdf_co_success;

}


/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetLastOfferMade
**
** DESCRIPTION: This function sets the LastOffer sent to the peer
**				in the media capabilities of the Call Object
**
** PARAMETERS:
**		pMediaInfo(IN/OUT)	: The Media Info structure
**		pLastOffer(IN)	: The offer sent to the peer (sdf_st_sessionParam)
**		pError(OUT)		: Error structure in case the function fails
***********************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetLastOfferMade
#ifdef ANSI_PROTO
 	(Sdf_st_mediaHandling *pMediaInfo, Sdf_st_sessionParam *pLastOffer,
	Sdf_st_error *pError)
#else
	(pMediaInfo, pLastOffer, pError)
	Sdf_st_mediaHandling *pMediaInfo;
	Sdf_st_sessionParam pLastOffer;
	Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null)||(pLastOffer == Sdf_co_null)|| \
		(pMediaInfo == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetLastOfferSent(): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	if(pMediaInfo->pLastOfferMade != Sdf_co_null)
	{
		(void)sdf_ivk_uaFreeSessionParam(pMediaInfo->pLastOfferMade);
	}
	pMediaInfo->pLastOfferMade = pLastOffer;
	HSS_LOCKEDINCREF(pLastOffer->dRefCount);
    pError->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
        "Exiting sdf_ivk_uaSetLastOfferRecvd():");
    return Sdf_co_success;

}

/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetCurrentSession
**
** DESCRIPTION: This function sets the established current Session 
**
** PARAMETERS:
**		pMediaInfo(IN/OUT)	: The Media Info structure
**		pSession(IN)	: sdf_st_sessionParam - the current session
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetCurrentSession
#ifdef ANSI_PROTO
 	(Sdf_st_mediaHandling *pMediaInfo, Sdf_st_sessionParam *pSession,\
	Sdf_st_error *pError)
#else
	(pMediaInfo, pSession, pError)
	Sdf_st_mediaHandling *pMediaInfo;
	Sdf_st_sessionParam *pSession;
	Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null)||(pSession == Sdf_co_null)|| \
		(pMediaInfo == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetCurrentSession(): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	if(pMediaInfo->pCurrentSession != Sdf_co_null)
	{
		(void)sdf_ivk_uaFreeSessionParam(pMediaInfo->pCurrentSession);
	}
	pMediaInfo->pCurrentSession = pSession;
	HSS_LOCKEDINCREF(pSession->dRefCount);
	
	/*Version Problem Fix: We will Update the Remote Ends SessionId and
	 * Version Number here into the capability.*/

	pMediaInfo->pCapabilities->dRemoteSessionId \
		= pSession->dRemoteSessionId;
	pMediaInfo->pCapabilities->dRemoteSessionVersion\
		= pSession->dRemoteSessionVersion;
		
    pError->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
        "Exiting sdf_ivk_uaSetCurrentSession():");
    return Sdf_co_success;
}
/******************************************************************************
** FUNCTION: sdf_ivk_uaFormSessionParam
**
** DESCRIPTION: The API will convert the incoming SDP message into 
**				Sdf_st_sessionParam Structure. MatchMedia Funciton will invoke 
**				this funciton to get the session param structure from the 
**				incoming message.
**
**				In addition to conversion the API will also do some validations
**				on the incoming SDP mesasage. Such as, Imporper use of fid(group
**				ing two media streams which have the same IP and port) absence 
**				of connection line at both session level and media level, and 
**				also checks for the presence of the o line.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormSessionParam 
#ifdef ANSI_PROTO 
	(SdpMessage 			*pSdpMessage,
	Sdf_st_sessionParam	*pSessionParam,
	Sdf_ty_conversionType	dType,
	Sdf_st_error *pErr)
#else
	(pSdpMessage,pSessionParam,dType,pErr)
	SdpMessage				*pSdpMessage;
	Sdf_st_sessionParam	*pSessionParam;
	Sdf_ty_conversionType	dType;
	Sdf_st_error			*pErr;
#endif
{
	SdpOrigin		*pOrigin=Sdf_co_null;
	Sdf_ty_s8bit	*pOriginVersion=Sdf_co_null;
	Sdf_ty_s8bit	*pOriginSessionId=Sdf_co_null;
	Sdf_ty_slist	slGroupList;
	Sdf_ty_s8bit	*pSessionStr=Sdf_co_null;
	SdpTime			*pSdpTime=Sdf_co_null;
#ifdef SDF_MEDIAGROUP
	Sdf_ty_u32bit	dSize=0;
#endif
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaFormSessionParam");

#ifdef SDF_PARAMVALIDATION
	if((pSdpMessage == Sdf_co_null) || (pSessionParam == Sdf_co_null)
		|| (pErr == Sdf_co_null))
	{
		if (pErr != Sdf_co_null)
		{
			pErr->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Invalid Param passed to function", pErr);
#endif			

		}
		return Sdf_co_fail;
	}
#endif
	
	/*Validate presence of "o" line*/
	if(sdp_getOrigin(pSdpMessage,&pOrigin,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
		if(pErr->stkErrCode==Sdf_en_noExistError)
		{
			pErr->errCode = Sdf_en_missingOriginLineError;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Origin line missing in SDP message", pErr);
#endif
		return Sdf_co_fail;
		}
		else
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Failed to get Origin from SDP message", pErr);
#endif
		return Sdf_co_fail;
		}
	}
	/* Extract Version & SessionId from Origin line */
	if(sdp_getVersionFromOrigin(pOrigin,&pOriginVersion,\
		(SipError*)&pErr->stkErrCode)==SipFail)
	{
			pErr->errCode = Sdf_en_sdpError; 
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Failed to get version from Origin line", pErr);
#endif
		sip_freeSdpOrigin(pOrigin);
		return Sdf_co_fail;
	}
	
	if(sdp_getSessionIdFromOrigin(pOrigin,&pOriginSessionId,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
			pErr->errCode = Sdf_en_sdpError; 
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Failed to get SessionId from Origin line", pErr);
#endif
		sip_freeSdpOrigin(pOrigin);
		return Sdf_co_fail;
	}
	
	/*Store them based on the conversion type*/
	if(dType==Sdf_en_convertLocal)
	{
#ifdef SDF_LINT
		if(pOriginVersion != Sdf_co_null)
#endif
		pSessionParam->dLocalSessionVersion = Sdf_mc_strtoul(pOriginVersion);
		pSessionParam->dLocalSessionId		= Sdf_mc_strtoul(pOriginSessionId);
	}
	else
	{
#ifdef SDF_LINT
		if(pOriginVersion != Sdf_co_null)
#endif 
		pSessionParam->dRemoteSessionVersion = Sdf_mc_strtoul(pOriginVersion);
		pSessionParam->dRemoteSessionId = Sdf_mc_strtoul(pOriginSessionId);
	}
	/*Get user name from origin*/
	if(sdp_getUserFromOrigin(pOrigin,&pSessionStr,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
			pErr->errCode = Sdf_en_sdpError; 
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Failed to get User Name from Origin line", pErr);
#endif
		sip_freeSdpOrigin(pOrigin);
		return Sdf_co_fail;
	}
	
	Sdf_mc_strdup(pSessionParam->pSessionUser,pSessionStr);
	pSessionStr = Sdf_co_null;

	/*Get AddressType From Origin*/
	if(sdp_getAddrTypeFromOrigin(pOrigin,&pSessionStr,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
			pErr->errCode = Sdf_en_sdpError; 
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Failed to get Address from Origin line", pErr);
#endif
		sip_freeSdpOrigin(pOrigin);
		return Sdf_co_fail;
	}

	Sdf_mc_strdup(pSessionParam->pOriginAddrType,pSessionStr);
	pSessionStr = Sdf_co_null;

	/*Get Address From Origin*/
	if(sdp_getAddrFromOrigin(pOrigin,&pSessionStr,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
			pErr->errCode = Sdf_en_sdpError; 
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Failed to get Address from Origin line", pErr);
#endif
		sip_freeSdpOrigin(pOrigin);
		return Sdf_co_fail;
	}

	Sdf_mc_strdup(pSessionParam->pOriginIp,pSessionStr);
	pSessionStr = Sdf_co_null;

	/*Free Local Reference*/
	sip_freeSdpOrigin(pOrigin);

	/*Extract & Store Version from "v=" line*/
	if(sdp_getVersion(pSdpMessage,&pSessionStr,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Failed to get Version from SDP message", pErr);
#endif
		return Sdf_co_fail;
	}
	pSessionParam->dVersion = (Sdf_ty_s8bit)Sdf_mc_strtoul(pSessionStr);
	pSessionStr = Sdf_co_null;
		
	/* Extract Session User s=*/
	if(sdp_getSession(pSdpMessage,&pSessionStr,\
		(SipError*)&pErr->stkErrCode)==SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Failed to get Session from SDP message", pErr);
#endif
		return Sdf_co_fail;
	}
	
	Sdf_mc_strdup(pSessionParam->pSessionName,pSessionStr);
	pSessionStr = Sdf_co_null;
			

	/* Extract Session Info i= line :Optional*/
	if(sdp_getInfo(pSdpMessage,&pSessionStr,
		(SipError*)&pErr->stkErrCode)==SipSuccess)
	{
		Sdf_mc_strdup(pSessionParam->pSessionInfo,pSessionStr);
		pSessionStr = Sdf_co_null;
	}
	else
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
			
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Failed to get Session Informaiton from SDP message", pErr);
#endif
			return Sdf_co_fail;
		}
	}
		
	/* Extract Session Level Connection line :OPTIONAL*/
	if(sdp_getConnection(pSdpMessage,&pSessionParam->pConnection,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
        if(pErr->stkErrCode == Sdf_en_noExistError)
		{
			Sdf_ty_u32bit i,cnt,connectionLines = 0;
			SdpMedia *pTmpMedia;
			Sdf_ty_retVal ConnectionLineflag = Sdf_co_success;	
		
			(void)sdp_getMediaCount(pSdpMessage,&cnt,(SipError*)&(pErr->stkErrCode));
			if(cnt == 0)
				ConnectionLineflag = Sdf_co_fail;
		
			for(i=0;i<cnt;i++)
			{
				(void)sdp_getMediaAtIndex(pSdpMessage,&pTmpMedia, \
						i,(SipError*)&(pErr->stkErrCode));
	
				if(sdp_getConnectionCountFromMedia(pTmpMedia,&connectionLines,\
					(SipError*)&(pErr->stkErrCode))== SipFail)
				{
					ConnectionLineflag = Sdf_co_fail;
					sip_freeSdpMedia(pTmpMedia);	
					break;
				}
				else if(connectionLines==0)
				{
					ConnectionLineflag = Sdf_co_fail;
					sip_freeSdpMedia(pTmpMedia);	
					break;
				}
				
				sip_freeSdpMedia(pTmpMedia);	
			}
			if(ConnectionLineflag == Sdf_co_fail)
			{
				pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
					"Failed to get Session Connection Informaiton"
					"from SDP message", pErr);
#endif
				return Sdf_co_fail;
			}
		}
		else
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Failed to get Session Connection Informaiton"
				"from SDP message", pErr);
#endif
			return Sdf_co_fail;
		}
	}
	/* Extract Emails :Optional*/
	if(sdf_fn_uaGetSessionList(pSdpMessage,&(pSessionParam->slEmail),\
                                sdp_getEmailAtIndex_level1,sdp_getEmailCount_level1,pErr)==Sdf_co_fail)
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Failed to get Email from SDP message", pErr);
#endif
			return Sdf_co_fail;
		}
	}
	/*Extract Phone Details : Optional*/
	if(sdf_fn_uaGetSessionList(pSdpMessage,&(pSessionParam->slPhone),\
	                        sdp_getPhoneAtIndex_level1,sdp_getPhoneCount_level1,pErr)==Sdf_co_fail)
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Failed to get Phone Values from SDP message", pErr);
#endif
			return Sdf_co_fail;
		}
	}
	/*Extract Bandwidth :Optional*/
	if(sdf_fn_uaGetSessionList(pSdpMessage,&(pSessionParam->slBandwidth),\
                        	sdp_getBandwidthAtIndex_level1,sdp_getBandwidthCount_level1,pErr)\
				==Sdf_co_fail)
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Failed to get Bandwidth from SDP message", pErr);
#endif
			return Sdf_co_fail;
		}
	}

	/* Extract URI :Optional*/
	if(sdp_getUri(pSdpMessage,&pSessionStr,\
			(SipError*)&pErr->stkErrCode)==SipSuccess)
	{
		Sdf_mc_strdup(pSessionParam->pUri,pSessionStr);
		pSessionStr = Sdf_co_null;
	}
	else
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Failed to get URI from SDP message", pErr);
#endif
			return Sdf_co_fail;
		}
	}


	/* Extract Encryption Key :Optional*/
	if(sdp_getKey(pSdpMessage,&pSessionStr,\
			(SipError*)&pErr->stkErrCode)==SipSuccess)
	{
		Sdf_mc_strdup(pSessionParam->pEncryptionKey,pSessionStr);
		pSessionStr = Sdf_co_null;
	}
	else
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Failed to get URI from SDP message", pErr);
#endif
			return Sdf_co_fail;
		}
	}

	/* Extract Start and End Session Time :Optional*/
	if(sdp_getTimeAtIndex(pSdpMessage,&pSdpTime,0,\
				(SipError*)&pErr->stkErrCode)==SipSuccess)
	{
		pSessionParam->dSessionStart = Sdf_mc_atoi(pSdpTime->pStart);
		pSessionParam->dSessionEnd  = Sdf_mc_atoi(pSdpTime->pStop);
		pSessionParam->pTimeZoneInfo	= Sdf_mc_strdupSdp(pSdpTime->pZone);
#ifdef SDF_UAB2B
/*SPR-4218 Extract r-line  :Optional*/    
    if(sdf_fn_uaCloneStringList(&(pSessionParam->slRepeatTime),
                               (Sdf_ty_slist*)&(pSdpTime->slRepeat),
									   pErr) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}
#endif
        /*Free Local Reference*/
		sip_freeSdpTime(pSdpTime);
	}
	else
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Failed to get Time from SdpMessage", pErr);
#endif
				return Sdf_co_fail;
		}
	}

	/*Initialize the Group List and pass it to
	 * ExtarctSessionLevelAttribute
	 */ 
	if(sdf_listInit(&slGroupList,__sip_freeSdpAttr,Sdf_co_false,\
		pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Failed to Initialize Attribute List", pErr);
#endif
			return Sdf_co_fail;
	}
	
	/*
	 * Extract session level attribute including
	 * direction attribute. The Group Attributes
	 * will be returned as a list which will be 
	 * processed later by sdf_fn_uaProcessGroups
	 */

	if(sdf_fn_uaExtractSessionLevelAttribute(\
		pSdpMessage,pSessionParam,&slGroupList,pErr)==Sdf_co_fail)
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
				"Failed to Extract Session Attributes", pErr);
#endif
			return Sdf_co_fail;
		}
	}
	/* Extract Media Streams */
	if(sdf_fn_uaExtractMediaStreams(\
		pSdpMessage, pSessionParam, pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;		
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Failed to Extract Media Streams", pErr);
#endif
		return Sdf_co_fail;
	}
#ifdef SDF_MEDIAGROUP
	/*
	 * If there are group attributes,process them
	 */
	if(sdf_listSizeOf(&slGroupList,&dSize,pErr) \
				== Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Failed to Size of Group List", pErr);
#endif
		return Sdf_co_fail;
	}
	if(dSize>0)
	{
		if(sdf_fn_uaProcessGroupings(&slGroupList, \
			pSessionParam,dSize,pErr)==Sdf_co_fail)
		{
			(void)sdf_listDeleteAll(&slGroupList,pErr);
#ifdef SDF_ERROR				
			if(pErr->errCode == Sdf_en_mediaGroupError)
			{
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
					"Invalid Groupings", pErr);
			}
			else
			{
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
					"Failed to Process Media Groupings", pErr);
			}
#endif
			return Sdf_co_fail;
		}
	}
#endif
	/*Delete the Group List*/
	(void)sdf_listDeleteAll(&slGroupList,pErr);
	
	/* Validate the connection Line presence*/
	if (sdf_fn_uaValidateConnectionLines(pSessionParam, pErr)==Sdf_co_false)
	{
		pErr->errCode = Sdf_en_missingConnectionLineError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParam( ): "
			"Connection Line missing at both SessionLevel and MediaLevel", pErr);
#endif
		return Sdf_co_fail;

	}
	
	/*Now that the entire message is Converted to Session Param Structre
	 * hold the reference to the SdpMessage inside the structre
	 */

#ifndef SDF_UAB2B
/*SPR-4217 :B2B doesn't use pSdpMessage  data member and all the processing is done on other data member of sessionParam structure. It is effecting there performance. so kept under flag.  */
    HSS_LOCKEDINCREF(pSdpMessage->dRefCount);
	pSessionParam->pSdpMessage = pSdpMessage;
#endif
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaFormSessionParam");

	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}
/******************************************************************************
** FUNCTION: sdf_ivk_uaFormSdpMessageFromSessionParam
**
** DESCRIPTION: The API will convert the session Param into a SdpMessage
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormSdpMessageFromSessionParam 
#ifdef ANSI_PROTO 
	(SdpMessage 			*pSdpMessage,
	Sdf_st_sessionParam		*pSessionParam,
	Sdf_ty_conversionType	dType,
	Sdf_st_error *pErr)
#else
	(pSdpMessage, pSessionParam, dType, pErr)
	SdpMessage				*pSdpMessage;
	Sdf_st_sessionParam	*pSessionParam;
	Sdf_ty_conversionType	dType;
	Sdf_st_error			*pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaFormSdpMessageFromSessionParam");
	
#ifdef SDF_PARAMVALIDATION
	if((pSdpMessage == Sdf_co_null) || (pSessionParam == Sdf_co_null)
		|| (pErr == Sdf_co_null))
	{
		if (pErr != Sdf_co_null)
		{
			pErr->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMessageFromSessionParam( ): "
			"Invalid Param passed to function", pErr);
#endif			
		}
		return Sdf_co_fail;
	}
#endif

	/*Form the Mandatory lines first v,o,s,c,t*/
	if(sdf_fn_uaFormMandatoryLines(pSdpMessage,pSessionParam,dType,\
						pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMessageFromSessionParam( ): "
			"Failed to Form Mandatory Lines", pErr);
#endif
		return Sdf_co_fail;
	}
	/*Form other fields*/
	if(sdf_fn_uaFormSessionAttributes(pSdpMessage,pSessionParam,\
						pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMessageFromSessionParam( ): "
			"Failed to Form Session Attributes", pErr);
#endif
		return Sdf_co_fail;
	}
	
	/* Form the session level attribute lines. 
	 * This function only forms the 
	 *  slOtherSessionAttributes paramter
 	 */
	if(sdf_fn_uaFormOtherSessionAttributes(pSdpMessage,pSessionParam,\
						pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMessageFromSessionParam( ): "
			"Failed to Form Other Session Attributes", pErr);
#endif
		return Sdf_co_fail;
	}
#ifdef SDF_MEDIAGROUP
	/* Form the group attribute 
	*/
	if(sdf_fn_uaFormGroupAttribute(pSdpMessage,pSessionParam,\
							pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMessageFromSessionParam( ): "
			"Failed to Form Group and Direction Attributes", pErr);
#endif
		return Sdf_co_fail;

	}
#endif
	/* Form the direction attribute 
	*/
	if(sdf_fn_uaUpdateDirectionAttribute(pSdpMessage,pSessionParam,\
							pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMessageFromSessionParam( ): "
			"Failed to Form Group and Direction Attributes", pErr);
#endif
		return Sdf_co_fail;

	}
	/*Form Media Streams*/
	if(sdf_fn_uaFormMediaStreams(pSdpMessage,pSessionParam,\
				pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMessageFromSessionParam( ): "
			"Failed to Form Media Streams", pErr);
#endif
		return Sdf_co_fail;
	}
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaFormSdpMessageFromSessionParam");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;

}
/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaFormSdpMediaFromMediaStream
 **
 ** DESCRIPTION: 	The API Forms a SdpMedia Structure from Sdf_st_mediaStream 
 ** 				Structure
 *****************************************************************************/
Sdf_ty_retVal	sdf_ivk_uaFormSdpMediaFromMediaStream
#ifdef	ANSI_PROTO
	(Sdf_st_mediaStream	*pMediaStream,
	 SdpMedia			**ppSdpMedia,
	 Sdf_st_error		*pErr)
#else
	(pMediaStream,ppSdpMedia,pErr)
	Sdf_st_mediaStream	*pMediaStream;
	SdpMedia			**ppSdpMedia;
	Sdf_st_error		*pErr;
#endif
{
	Sdf_ty_s8bit		dMvalue[Sdf_co_characterBufferSize];
	Sdf_ty_s8bit		dTransport[Sdf_co_characterBufferSize];
	Sdf_ty_u32bit		dAttrIndex=0;
	
	Sdf_ty_slist			slAttribute;
	Sdf_ty_s8bit		*pAttrString=Sdf_co_null;
	Sdf_st_listIterator	dListIterator;
	Sdf_ty_s8bit		dPayLoad[Sdf_co_characterBufferSize];
	Sdf_ty_u32bit		dConnectionIndex=0;
	Sdf_ty_u32bit		dBandwidthIndex=0;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaFormSdpMediaFromMediaStream");

#ifdef SDF_PARAMVALIDATION
	if((pMediaStream == Sdf_co_null) || (ppSdpMedia == Sdf_co_null)
		|| (pErr == Sdf_co_null))
	{
		if (pErr != Sdf_co_null)
		{
			pErr->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream( ): "
			"Invalid Param passed to function", pErr);
#endif			
		}
		return Sdf_co_fail;
	}
#endif

	/*Form M Value*/
	switch(pMediaStream->dMediaType)
	{
		case Sdf_en_audio:
		{
			Sdf_mc_strcpy(dMvalue,"audio");
			break;
		}
		case Sdf_en_video:
		{
			Sdf_mc_strcpy(dMvalue,"video");
			break;
		}
		case Sdf_en_whiteboard:
		{
			Sdf_mc_strcpy(dMvalue,"whiteboard");
			break;
		}
		case Sdf_en_text:
		{
			Sdf_mc_strcpy(dMvalue,"text");
			break;
		}
		case Sdf_en_mediaTypeother:
		{
			Sdf_mc_strcpy(dMvalue,pMediaStream->pOtherMediaType);
			break;
		}
	}
	/*Form Transport*/
	switch(pMediaStream->dTransport)
	{
		case Sdf_en_rtpAvp:
		{
			Sdf_mc_strcpy(dTransport,"RTP/AVP");
			break;
		}
		case Sdf_en_udp:
		{
			Sdf_mc_strcpy(dTransport,"udp");
			break;
		}
		case Sdf_en_tcp:
		{
			Sdf_mc_strcpy(dTransport,"tcp");
			break;
		}
		case Sdf_en_other:
		{
			Sdf_mc_strcpy(dTransport,pMediaStream->pOtherTransportType);
			break;
		}
	}
	/*Initialize Attribute List*/
	(void)sdf_listInit(&slAttribute,__sip_freeString,Sdf_co_false,pErr);
	
	/* For Each Codec form 
	 * 1.rtpmap atribute
	 * 2.fmtp attributes
	 */
	 
	(void)sdf_listInitIterator(&(pMediaStream->slCodecs),&dListIterator,\
								pErr);
	dPayLoad[0]= '\0';
	while(dListIterator.pCurrentElement!= Sdf_co_null)
	{
		Sdf_st_codec	*pCodec;
		Sdf_ty_s8bit	dTemp[Sdf_co_smallBufferSize];
		Sdf_ty_s8bit	dRtpAttribute[Sdf_co_smallBufferSize];

		pCodec = (Sdf_st_codec*)(dListIterator.pCurrentElement->pData);

		if(pCodec->dCodecNum !=-1)
			(void)sdf_fn_uaSprintf(dTemp,"%u",pCodec->dCodecNum);
		else
			(void)sdf_fn_uaSprintf(dTemp,"%s",pCodec->pOtherCodecName);

		Sdf_mc_strcat(dPayLoad,dTemp);
		Sdf_mc_strcat(dPayLoad," ");
		/*Form rtpmap and fmtp only if Port is Non-Zero*/
		if(pMediaStream->dPort !=0)
		{	
			Sdf_ty_bool			dRtpMapPresent = Sdf_co_true;

			/*Form the rtp attribute*/
			Sdf_mc_strcpy(dRtpAttribute,"a=rtpmap:");
		
			if(pCodec->dCodecName!=Sdf_en_otherCodec)
			{
				Sdf_ty_s8bit	dTempCodecString[Sdf_co_smallBufferSize]; 
				
				(void)sdf_fn_uaMapCodecEnumToString(pCodec->dCodecName,\
						dTempCodecString,pErr);
				(void)sdf_fn_uaSprintf(dTemp,"%u %s",pCodec->dCodecNum, \
								dTempCodecString);
			}
#ifdef SDF_UAB2B
        else if(pCodec->dCodecName == Sdf_en_otherCodec)
			{
				if(pCodec->pOtherCodecName == Sdf_co_null)
				{
					pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, \
						pErr->errCode, \
						(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream:"
						"rtpmap missing for dyanmic codec", pErr);
#endif			
					return Sdf_co_fail;
				}
				if(pCodec->dCodecNum != -1)
					(void)sdf_fn_uaSprintf(dTemp,"%u %s",pCodec->dCodecNum, \
							pCodec->pOtherCodecName);
				else
					dRtpMapPresent = Sdf_co_false;
			}
            else
            {
                pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
                sdf_fn_setError(Sdf_en_majorErrors, \
                        pErr->errCode, \
                        (Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream:"
                        "rtpmap missing for dyanmic codec", pErr);
#endif			
                return Sdf_co_fail;
            }

#else
			else
			{
				if(pCodec->pOtherCodecName == Sdf_co_null)
				{
					pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, \
						pErr->errCode, \
						(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream:"
						"rtpmap missing for dyanmic codec", pErr);
#endif			
					return Sdf_co_fail;
				}
				if(pCodec->dCodecNum != -1)
					(void)sdf_fn_uaSprintf(dTemp,"%u %s",pCodec->dCodecNum, \
							pCodec->pOtherCodecName);
				else
					dRtpMapPresent = Sdf_co_false;
			}
#endif            
			/*Now the attribute String is of the form
			 * a=rtpmap:xx: ppp/qqq/r
			 */
			if( dRtpMapPresent == Sdf_co_true )
			{
				Sdf_mc_strcat(dRtpAttribute,dTemp);
				if(pCodec->dEncodingRate !=0)
				{
					Sdf_ty_s8bit dEncode[Sdf_co_smallBufferSize];
					Sdf_mc_strcat(dRtpAttribute,"/");

					(void)sdf_fn_uaSprintf(dEncode,"%d",pCodec->dEncodingRate);
					Sdf_mc_strcat(dRtpAttribute,dEncode);
				}
				if(pCodec->dChannels !=0)
				{
					Sdf_ty_s8bit dChanel[Sdf_co_smallBufferSize];
					Sdf_mc_strcat(dRtpAttribute,"/");

					(void)sdf_fn_uaSprintf(dChanel,"%d",pCodec->dChannels);
					Sdf_mc_strcat(dRtpAttribute,dChanel);
				}
					
			
				pAttrString = Sdf_mc_strdupSdp(dRtpAttribute);
			
				/*Append it to the list of attribute strings*/
				/*w534*/(void)sdf_listAppend(&slAttribute,(Sdf_ty_pvoid)pAttrString,pErr);
			}

			/*Form the fmtp Attribute*/
			if(pCodec->pFmtpString!=Sdf_co_null)
			{
				Sdf_ty_s8bit	dTempFmtp[Sdf_co_smallBufferSize];
				
    		    (void)sdf_fn_uaSprintf(dTempFmtp,"%s%s","a=fmtp:",\
							pCodec->pFmtpString);

				
				pAttrString = Sdf_mc_strdupSdp(dTempFmtp);
				/*Append it to the list of attribute strings*/
				/*w534*/(void)sdf_listAppend(&slAttribute,(Sdf_ty_pvoid)pAttrString,pErr);
			}
		}
		(void)sdf_listNext(&dListIterator,pErr);		
	}
	/*Add the mid attribute to the list irrespective of the Port Value*/
	if(pMediaStream->dMidPresence)
	{
		Sdf_ty_s8bit	dTemp[Sdf_co_characterBufferSize];
		Sdf_mc_strcpy(dTemp,"a=mid:");
		Sdf_mc_strcat(dTemp,pMediaStream->pMediaStreamId);
		pAttrString = Sdf_mc_strdupSdp(dTemp);
		
		/*Append it to the list of attribute strings*/
		/*w534*/(void)sdf_listAppend(&slAttribute,(Sdf_ty_pvoid)pAttrString,pErr);
	}
	/*Now Invoke sdf_ivk_uaFormSdpMedia to form the SdpMedia
	 * Structure
	 */
	if(sdf_ivk_uaFormSdpMedia(dMvalue,pMediaStream->dPort,\
		pMediaStream->dNumPort,dTransport,dPayLoad,Sdf_co_null,\
		&slAttribute,Sdf_co_null,Sdf_co_null,Sdf_co_null,\
		Sdf_co_null,ppSdpMedia,pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, \
			pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream( ): "
				"Failed to form SdpMedia", pErr);
#endif			
		return Sdf_co_fail;	
	}
	/*Delete the Attribute list*/
	(void)sdf_listDeleteAll(&slAttribute,pErr);
	/*Here again add other attributes such as Direction,Qos,Bandwidth etc only
	 * if Port is non-zero
	 */
	/*Form Connection Attribute*/
	(void)sdf_listInitIterator(&(pMediaStream->slConnection),&dListIterator,\
			pErr);
	while(dListIterator.pCurrentElement!= Sdf_co_null)
	{
		SdpConnection	*pConnection=Sdf_co_null;

		pConnection = (SdpConnection*)dListIterator.\
			pCurrentElement->pData;
		if(sdp_insertConnectionAtIndexInMedia(*ppSdpMedia,pConnection,\
					dConnectionIndex,(SipError*)&pErr->stkErrCode)==SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream:"\
					"Failed to Insert Connection", pErr);
#endif			
			return Sdf_co_fail;
		}
		dConnectionIndex++;
		(void)sdf_listNext(&dListIterator,pErr);
	}
	
	if(pMediaStream->dPort !=0 )
	{
		/*Get Count of Attr*/
		if(sdp_getAttrCountFromMedia(*ppSdpMedia,&dAttrIndex,\
					(SipError*)&pErr->stkErrCode)==SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
				pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream( ): "
					"Failed to Attribute Count", pErr);
#endif			
			return Sdf_co_fail;

		}

		/*Now Form slOtherAttribs*/
		(void)sdf_listInitIterator(&(pMediaStream->slOtherAttribs),&dListIterator,\
											pErr);
		while(dListIterator.pCurrentElement!= Sdf_co_null)
		{
			SdpAttr *pSdpAttr=Sdf_co_null;
			
			pSdpAttr = (SdpAttr*)dListIterator.pCurrentElement->pData;

			if(sdp_insertAttrAtIndexInMedia(*ppSdpMedia,pSdpAttr,dAttrIndex,\
						(SipError*)&pErr->stkErrCode)==SipFail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream():"\
						"Failed to Insert SdpAttr", pErr);
#endif			
				return Sdf_co_fail;
			}
			dAttrIndex++;
			(void)sdf_listNext(&dListIterator,pErr);
		}
		if(pMediaStream->dDirectionAttrib != Sdf_en_dirAttribNone)
		{
			SdpAttr 	*pSdpAttr =Sdf_co_null;
			/*Form Direction Attributes*/
			if(sdf_fn_uaFormDirectionAttribute(pMediaStream->dDirectionAttrib,\
						&pSdpAttr,pErr)==Sdf_co_success)
			{
				if(sdp_insertAttrAtIndexInMedia(*ppSdpMedia,pSdpAttr, \
					dAttrIndex, (SipError*)&pErr->stkErrCode)==SipFail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
						(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream:"\
						"Failed to Insert SdpAttr", pErr);
#endif			
					return Sdf_co_fail;
				}
				dAttrIndex++;
				sip_freeSdpAttr(pSdpAttr);
			}
			else
			{
				if(pErr->errCode != Sdf_en_noUaError)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
						(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream:"\
							"Failed to form Direction Attribute", pErr);
#endif			
					return Sdf_co_fail;
				}

			}
		}
		/*Form bandwidth attribute*/
		(void)sdf_listInitIterator(&(pMediaStream->slBandwidth),&dListIterator,\
											pErr);
		while(dListIterator.pCurrentElement!= Sdf_co_null)
		{
			Sdf_ty_s8bit	*pBandwidth=Sdf_co_null,*pTempBandwidth=Sdf_co_null;

			pBandwidth = (Sdf_ty_s8bit*)(dListIterator.\
										pCurrentElement->pData);
			pTempBandwidth = Sdf_mc_strdupSdp(pBandwidth);
			
			if(sdp_insertBandwidthAtIndexInMedia(*ppSdpMedia,pTempBandwidth,\
					dBandwidthIndex,(SipError*)&pErr->stkErrCode)==SipFail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream():"\
						"Failed to Insert bandwidth", pErr);
#endif			
			return Sdf_co_fail;
			}
			dBandwidthIndex++;
			(void)sdf_listNext(&dListIterator,pErr);
		}
#ifdef SDF_QOS
		/*Form Qos Attributes*/
		if(sdf_fn_uaFormQosAttributesFromMediaStream(pMediaStream,*ppSdpMedia,\
				pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream():"\
						"Failed to form Qos Attributes", pErr);
#endif			
			return Sdf_co_fail;
		}
#endif
		/*Form Media Key*/
		if(SipFail==sdp_setKeyInMedia((*ppSdpMedia),\
				Sdf_mc_strdupSdp(pMediaStream->pKey), \
				(SipError*)&(pErr->stkErrCode)))
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream( ): "
				"Failed to insert Key in the SdpMedia structure", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		/*Form Media Info*/
		if(SipFail==sdp_setInfoInMedia((*ppSdpMedia),\
					Sdf_mc_strdupSdp(pMediaStream->pTitle),\
			(SipError*)&(pErr->stkErrCode)))
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream( ): "
				"Failed to insert Media Info in the SdpMedia structure", pErr);
#endif			
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}

	}
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaFormSdpMediaFromMediaStream");

	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaInitiateOffer
 **
 ** DESCRIPTION: 	The API checks if a Offer can be initiated,given the
 **					current state. Based on the type of Offer,the function 
 **					returns the session param structure.
 **					newOffer: Clone of InitData
 **					offerChange: Clone of current Session
 **					refresg		:Reference to current Session
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitiateOffer 
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam	**ppSessionParam,
	 Sdf_st_callObject		*pCallObject,
	 Sdf_ty_offerType	 	 dOfferType,
 	 Sdf_st_error		*pErr)
#else
	(ppSessionParam,pCallObject,dOfferType,pErr)
	Sdf_st_sessionParams	**ppSessionParam;
	Sdf_ty_callObject		*pCallObject;
	Sdf_ty_offerType	 	 dOfferType;
 	Sdf_st_error		*pErr;
#endif
{
	Sdf_ty_mediaState	dState;

#ifdef SDF_PARAMVALIDATION
	if((ppSessionParam == Sdf_co_null) || (pCallObject == Sdf_co_null)
		|| (pErr == Sdf_co_null))
	{
		if (pErr != Sdf_co_null)
		{
			pErr->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInitiateOffer( ): "
			"Invalid Param passed to function", pErr);
#endif			
		}
		return Sdf_co_fail;
	}
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitiateOffer");
	*ppSessionParam = Sdf_co_null;

	/*Lock Call State Mutex*/
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0, &(pCallObject->dCallStateMutex), 0);
#endif

	/*Extract the Media State*/

	dState = pCallObject->pMediaInfo->dMediaState;

	if ((dState == Sdf_en_offerSent) || 
		(dState == Sdf_en_offerRecv) || 
		(dState == Sdf_en_offerChangeSent)|| 
		(dState == Sdf_en_offerChangeRecv))
	{
		/*Unlock Call State Mutex*/
		pErr->errCode = Sdf_en_mediaStateTransitionError;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, \
			pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaInitiateOffer( ): "
				"Invalid Media State Transition ", pErr);
#endif			
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(pCallObject->dCallStateMutex));
#endif
		return Sdf_co_fail;

	}
	else if (dOfferType == Sdf_en_refresh)
	{
		Sdf_st_mediaHandling *pMediaInfo=Sdf_co_null;
		/*Get MediaInfo*/
		if(sdf_ivk_uaGetMediaInfo(pCallObject,&pMediaInfo,\
					pErr)==Sdf_co_fail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
				pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaInitiateOffer( ): "
					"Failed to get Media Info", pErr);
#endif			
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0, &(pCallObject->dCallStateMutex));
#endif
			return Sdf_co_fail;
		}
		/*A Reference to the current session is returned*/
		if(sdf_ivk_uaGetSessionParamsFromMediaInfo(pMediaInfo,ppSessionParam,\
			Sdf_en_currentSession,pErr) == Sdf_co_fail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
				pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaInitiateOffer( ): "
					"Failed to get current Session", pErr);
#endif			
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0, &(pCallObject->dCallStateMutex));
#endif
			return Sdf_co_fail;

		}
		/*Free Local Reference*/
		(void)sdf_ivk_uaFreeMediaHandling(pMediaInfo);
		
		/*Version number is unchanged*/
	}
	else	
	{
		Sdf_st_mediaHandling	*pMediaInfo= Sdf_co_null;
		
		/*Initialize pSessionParam*/
		if(sdf_ivk_uaInitSessionParam(ppSessionParam,pErr)==Sdf_co_fail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
				pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaInitiateOffer( ): "
					"Failed Initialize Session Param", pErr);
#endif			
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0, &(pCallObject->dCallStateMutex));
#endif
			return Sdf_co_fail;
		}
		/*Get MediaInfo*/
		if(sdf_ivk_uaGetMediaInfo(pCallObject,&pMediaInfo,\
					pErr)==Sdf_co_fail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaInitiateOffer( ): "
				"Failed to get Media Info", pErr);
#endif			
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0, &(pCallObject->dCallStateMutex));
#endif
			return Sdf_co_fail;
		}
		if(dOfferType == Sdf_en_newOffer)
		{
			/*Clone the  Session Param with Capability*/
			if(sdf_ivk_uaCloneSessionParam(*ppSessionParam,pMediaInfo->\
				pCapabilities,pErr)==Sdf_co_fail)
			{
				pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaInitiateOffer( ): "
					"Failed to Clone Session param", pErr);
#endif			
#ifdef SDF_THREAD_SAFE
				sdf_fn_uaUnlockMutex(0, &(pCallObject->dCallStateMutex));
#endif
				return Sdf_co_fail;

			}
		}
		else if(dOfferType == Sdf_en_offerChange)
		{
			/*Clone the  Session Param with Current Session*/
			if(sdf_ivk_uaCloneSessionParam(*ppSessionParam,pMediaInfo->\
				pCurrentSession,pErr)==Sdf_co_fail)
			{
				pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaInitiateOffer( ): "
					"Failed to Session param", pErr);
#endif			
#ifdef SDF_THREAD_SAFE
				sdf_fn_uaUnlockMutex(0, &(pCallObject->dCallStateMutex));
#endif
				return Sdf_co_fail;
			}
		}
				
		/* Increment version num in pSessionParam */
		(*ppSessionParam)->dLocalSessionVersion++;

		if (pMediaInfo->pCapabilities != Sdf_co_null)
        {
		    pMediaInfo->pCapabilities->dLocalSessionVersion++;
		}
		
		/*Free Local Reference*/
		(void)sdf_ivk_uaFreeMediaHandling(pMediaInfo);
	}
	/*UnLock Call StateMutex */
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &(pCallObject->dCallStateMutex));
#endif
	/*Set the Intermediate Flag*/
	pCallObject->pMediaInfo->dOfferProgress = Sdf_co_true;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitiateOffer");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaRejectSdp
 **
 ** DESCRIPTION: 	The API will form a SDP message with All ports in the media
 **					stream set to 0 and appending a a=inactive line at the 
 **					session level
 *****************************************************************************/

Sdf_ty_retVal	sdf_ivk_uaRejectSdp
#ifdef ANSI_PROTO
	(SdpMessage		*pIncomingSdpMessage, 
	 SdpMessage 	*pReturnSdpMessage,
	 Sdf_st_error	*pErr)
#else
	(pIncomingSdpMessage,ppReturnSdpMessage,pErr)
	SdpMessage		*pIncomingSdpMessage;
	SdpMessage 		*pReturnSdpMessage;
	Sdf_st_error	*pErr;
#endif
{
	Sdf_ty_u32bit	dSize=0,dCount=0,i=0;

#ifdef SDF_PARAMVALIDATION
	if((pIncomingSdpMessage == Sdf_co_null) ||\
				(pReturnSdpMessage == Sdf_co_null)|| (pErr == Sdf_co_null))
	{
		if (pErr != Sdf_co_null)
		{
			pErr->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaRejectSdp( ): "
			"Invalid Param passed to function", pErr);
#endif			
		}
		return Sdf_co_fail;
	}
#endif
	
	/*Clone the Incoming SdpMessage into ReturnSdpMessage*/
	if(sdp_cloneSdpMessage(pReturnSdpMessage,pIncomingSdpMessage,\
						(SipError*)&pErr->stkErrCode)==SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
		(Sdf_ty_s8bit*)"sdf_ivk_uaRejectSdp( ): "
		"Failed to Clone Sdp Message", pErr);
#endif			
		return Sdf_co_fail;
	}


	/*Iterate through All Media Streams and Set their Ports to 0*/
	
	if(sdp_getMediaCount(pReturnSdpMessage,&dCount, \
							(SipError*)&pErr->stkErrCode) == SipFail)
			return Sdf_co_fail;
	
	for(i=0;i<dCount;i++)
	{
		SdpMedia *pSdpMedia = Sdf_co_null;
		/*Get media Stream*/
		if(sdp_getMediaAtIndex(pReturnSdpMessage,&pSdpMedia,i,\
					(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaRejectSdp( ): "
				"Failed to get Media", pErr);
#endif			
			return Sdf_co_fail;
		}
	

		/*Set Port to 0 */
		if(sdp_setPortInMedia(pSdpMedia,(Sdf_ty_u16bit)0,\
							(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaRejectSdp( ): "
				"Failed to Set port", pErr);
#endif			
			return Sdf_co_fail;
		}
	

		sip_freeSdpMedia(pSdpMedia);
	}
	/*Get Attr Count*/
	 /*w534*/(void)sdp_getAttrCount(pReturnSdpMessage,&dSize, \
							(SipError *)&(pErr->stkErrCode));

	/*Add a=inactive line*/
	if(SipFail == sdp_insertSdpAttrAtIndexFromString(pReturnSdpMessage,\
		(Sdf_ty_s8bit*)"a=inactive", (Sdf_ty_s8bit)dSize, \
		(SipError*)&pErr->stkErrCode))
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaRejectSdp( ): "
			"Failed to Insert inactive Attribute", pErr);
#endif			
		return Sdf_co_fail;
	}
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	sdf_ivk_uaFormSdpMediaStream
 **
 ** DESCRIPTION: This function forms a Sdf_st_mediaStream using the parameters 
 **				 passed into the function
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormSdpMediaStream
#ifdef ANSI_PROTO
	 (Sdf_ty_s8bit *pMvalue,
	  Sdf_ty_u32bit dPort,
	  Sdf_ty_u32bit dNumberOfPorts,
	  Sdf_ty_s8bit *pTransport,
	  Sdf_ty_slist *pslCodecList,
	  Sdf_ty_s8bit *pTypeOfTransport,
	  Sdf_ty_slist *pslConnection,
	  Sdf_st_mediaStream **ppSdpMediaStream,
	  Sdf_st_error *pErr)
#else
	(pMvalue, dPort, dNumberOfPorts, pTransport, pslCodecList,\
	 pTypeOfTransport, pslConnection, ppSdpMediaStream, pErr )
	 Sdf_ty_s8bit 	*pMvalue;
	 Sdf_ty_u32bit 	dPort;
	 Sdf_ty_u32bit 	dNumberOfPorts;
	 Sdf_ty_s8bit 	*pTransport;
	 Sdf_ty_slist 	*pslCodecList;
	 Sdf_ty_s8bit 	*pTypeOfTransport;
	 Sdf_ty_slist 	*pslConnection,
	 Sdf_st_mediaStream **ppSdpMediaStream;
	 Sdf_st_error 	*pErr;
#endif
{
	Sdf_ty_u32bit 		dIndex = 0;
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit *) \
			"SDF_DEBUG-Entering sdf_ivk_uaFormSdpMediaStream");
	
#ifdef SDF_PARAMVALIDATION
	/* Error checks in the input parameters */
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;

	if(pslCodecList == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaStream( ): "
			"Invalid param passed to function: payload list", pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif	/* End of param validation ifdef */
	
	*ppSdpMediaStream = Sdf_co_null;

	if (sdf_ivk_uaInitMediaStream(ppSdpMediaStream, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaStream( ): "
			"Failed to initialize a SdpMedia structure", pErr);
#endif		
		pErr->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	if(Sdf_mc_strcasecmp(pMvalue,"audio")==0)
		(*ppSdpMediaStream)->dMediaType = Sdf_en_audio;
	else if(Sdf_mc_strcasecmp(pMvalue,"video")==0)
		(*ppSdpMediaStream)->dMediaType = Sdf_en_video;
	else if(Sdf_mc_strcasecmp(pMvalue,"whiteboard")==0)
		(*ppSdpMediaStream)->dMediaType = Sdf_en_whiteboard;
	else if(Sdf_mc_strcasecmp(pMvalue,"text")==0)
		(*ppSdpMediaStream)->dMediaType = Sdf_en_text;
	else 
		{
			(*ppSdpMediaStream)->dMediaType = Sdf_en_mediaTypeother;
			Sdf_mc_strdup((*ppSdpMediaStream)->pOtherMediaType, pMvalue);
		}

	if(Sdf_mc_strcasecmp(pTransport,"RTP/AVP")==0)
		(*ppSdpMediaStream)->dTransport = Sdf_en_rtpAvp;
	else if(Sdf_mc_strcmp(pTransport,"udp")==0)
		(*ppSdpMediaStream)->dTransport = Sdf_en_udp;
	else if(Sdf_mc_strcmp(pTransport,"tcp")==0)
		(*ppSdpMediaStream)->dTransport = Sdf_en_tcp;
	else
		{
			(*ppSdpMediaStream)->dTransport = Sdf_en_other;
			Sdf_mc_strdup((*ppSdpMediaStream)->pOtherTransportType, pTransport);
		}
	

	(*ppSdpMediaStream)->dPort 		= dPort;
	(*ppSdpMediaStream)->dNumPort 	= dNumberOfPorts;
	

	dIndex = 0;
	(void)sdf_listInitIterator(pslCodecList, &dListIterator, pErr);
		
	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_codec	*pCodec = Sdf_co_null;

		pCodec = (Sdf_st_codec*)(dListIterator.pCurrentElement->pData);

		HSS_LOCKEDINCREF(pCodec->dRefCount);

		if(sdf_listInsertAt(&((*ppSdpMediaStream)->slCodecs), dIndex, \
			(Sdf_ty_pvoid)pCodec, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaStream( ): "
				"Failed to insert codec into SdpMediaStream", pErr);
#endif				
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
		dIndex++;
	}

	if(pslConnection != Sdf_co_null)
	{
		dIndex = 0;

		(void)sdf_listInitIterator(pslConnection, &dListIterator, pErr);
			
		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			SdpConnection		*pTempConn = Sdf_co_null;

			pTempConn = (SdpConnection *)(dListIterator.pCurrentElement->pData);
			
			HSS_LOCKEDINCREF(pTempConn->dRefCount);

			if(sdf_listInsertAt(&((*ppSdpMediaStream)->slConnection), dIndex, \
				(Sdf_ty_pvoid)pTempConn, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaStream( ): "
					"Failed to insert Connection in the SdpMediaStream", pErr);
#endif				
				pErr->errCode=Sdf_en_sdpError;
				return Sdf_co_fail;
			}
			(void)sdf_listNext(&dListIterator, pErr);
			dIndex++;
		}
	}

	if (pTypeOfTransport != Sdf_co_null)
	{
		if(Sdf_mc_strcasecmp(pTypeOfTransport,"sendonly")==0)
			(*ppSdpMediaStream)->dDirectionAttrib = Sdf_en_sendonly;
		else if(Sdf_mc_strcmp(pTypeOfTransport,"recvonly")==0)
			(*ppSdpMediaStream)->dDirectionAttrib = Sdf_en_recvonly;
		else if(Sdf_mc_strcmp(pTypeOfTransport,"sendrecv")==0)
			(*ppSdpMediaStream)->dDirectionAttrib = Sdf_en_sendrecv;
		else if(Sdf_mc_strcmp(pTypeOfTransport,"inactive")==0)
			(*ppSdpMediaStream)->dDirectionAttrib = Sdf_en_inactive;
		else 
			(*ppSdpMediaStream)->dDirectionAttrib = Sdf_en_dirAttribNone;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)\
			"SDF_DEBUG - Exiting sdf_ivk_uaFormSdpMediaStream");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	sdf_ivk_uaFormCodec
 **
 ** DESCRIPTION: This function forms the Sdf_st_codec structure from 
 **				 either codec number or codec name.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormCodec
#ifdef ANSI_PROTO
	 (Sdf_ty_s8bit  *pCodecName,
	  Sdf_ty_u32bit dEncodingRate,
	  Sdf_ty_u8bit  dChannels,
	  Sdf_ty_u8bit 	dCodecNum,
	  Sdf_st_codec  **ppCodec,
	  Sdf_st_error 	*pErr)
#else
	(pCodecName, dEncodingRate, dChannels, dCodecNum, ppCodec, pErr) 
	 Sdf_ty_s8bit  *pCodecName;
	 Sdf_ty_u32bit dEncodingRate;
	 Sdf_ty_u8bit  dChannels;
	 Sdf_ty_u8bit  dCodecNum;
	 Sdf_st_codec  **ppCodec;
	 Sdf_st_error  *pErr;
#endif
{
	Sdf_ty_u32bit	dIndex = 0;
	
	Sdf_ty_s8bit	dTemp[Sdf_co_smallBufferSize];
	Sdf_ty_s8bit	*pString = Sdf_co_null;
	Sdf_ty_s8bit	*pTempString = Sdf_co_null;
	Sdf_ty_s8bit	*pTempString1 = Sdf_co_null;

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR	
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR-sdf_ivk_uaFormCodec():" \
			"Invalid Error parameter passed to function");
#endif			
		return Sdf_co_fail;
	}
	if(ppCodec == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormCodec(): "
			"Invalid codec argument passed",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
#endif	

	if( (pCodecName == Sdf_co_null) && dCodecNum >= Sdf_co_codecTableSize)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormCodec(): Either codec name or"
			"Codec number should be provided",pErr);
#endif		
		pErr->errCode= Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
#ifdef SDF_UAB2B
    /*TODO - SPR - 3851 Check whether codec number is valid codec*/
    if(pCodecName == Sdf_co_null)
    {
        Sdf_ty_codecName  dTempCodecName;
        dTempCodecName = sdf_gl_dCodecProfile[dCodecNum].dCodecName;
        if (Sdf_en_unassigned == dTempCodecName)
        {
#ifdef SDF_ERROR		
            sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
                    (Sdf_ty_s8bit*)"sdf_ivk_uaFormCodec(): No Codec name"\
                    " provided and codec number provided is unassigned ",pErr);
#endif		
            pErr->errCode= Sdf_en_invalidParamError ;
            return Sdf_co_fail;
        }
    }

#endif

	if( Sdf_co_fail == sdf_ivk_uaInitCodec(ppCodec, pErr))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormCodec(): "
			"Failed To Initialise Codec Structure",pErr);
#endif		
		pErr->errCode= Sdf_en_noMemoryError ;
		return Sdf_co_fail;
	}

	(*ppCodec)->dCodecNum = dCodecNum;

	if( pCodecName != Sdf_co_null )
	{
		Sdf_ty_u32bit i = 0, dLength = 0;

		(void)sdf_fn_uaMapCodecStringToEnum(pCodecName, &((*ppCodec)->dCodecName));
		(*ppCodec)->dChannels 		= dChannels;
		(*ppCodec)->dEncodingRate 	= dEncodingRate;

		(void)sdf_fn_uaSprintf(dTemp,"%s/%d/%d",pCodecName,dEncodingRate,dChannels);

		dLength = Sdf_mc_strlen(dTemp);

		for (i = 0; i < dLength; i++)
			dTemp[i] = Sdf_mc_toupper(dTemp[i]);

		for(dIndex=0; dIndex < Sdf_co_codecTableSize; dIndex++)
		{
			if(Sdf_mc_strstr(sdf_gl_dCodecProfile[dIndex].dCodecString, \
						dTemp) != Sdf_co_null)
			{
				(*ppCodec)->dCodecNum = dIndex;
				break;
			}
		}
		if( dIndex == Sdf_co_codecTableSize )
#ifdef SDF_UAB2B
            /*TODO SPR-3851 Imp*/
        {
            if (dCodecNum >= Sdf_co_codecTableSize)
            {
                (*ppCodec)->dCodecNum = dCodecNum;
            }
            else
            {
                (*ppCodec)->dCodecNum = dIndex;
            }
        }
#else
			(*ppCodec)->dCodecNum = dIndex;
#endif            
	}
	else
	{
		(*ppCodec)->dCodecName = sdf_gl_dCodecProfile[dCodecNum].dCodecName;

		Sdf_mc_strcpy(dTemp, sdf_gl_dCodecProfile[dCodecNum].dCodecString);
		
		pString = Sdf_mc_strtokr(dTemp, "/", &pTempString);

		pCodecName = pString;

		if( pTempString != Sdf_co_null )
		{
			pTempString1 = pTempString;
			pString = Sdf_mc_strtokr(pTempString1, "/", &pTempString);
			(*ppCodec)->dEncodingRate = Sdf_mc_atoi(pString);
		}

		if( pTempString != Sdf_co_null )
		{
			pTempString1 = pTempString;
			pString = Sdf_mc_strtokr(pTempString1, "/", &pTempString);
			(*ppCodec)->dChannels = Sdf_mc_atoi(pString);
		}
	}

	if( (*ppCodec)->dCodecName == Sdf_en_otherCodec )
	{
		Sdf_mc_strdup((*ppCodec)->pOtherCodecName, pCodecName);
	}

	if((*ppCodec)->dChannels == 0)
		(*ppCodec)->dChannels = 1;

	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	sdf_ivk_uaFormMediaStreamFromSdpMedia
 **
 ** DESCRIPTION: This function fills Sdf_st_mediaStream structure from the given
 **				 SdpMedia Structure
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormMediaStreamFromSdpMedia
#ifdef ANSI_PROTO
	(Sdf_st_mediaStream *pMediaStream,
	 SdpMedia			*pSdpMedia,
	 Sdf_st_error		*pErr)
#else
	(Sdf_st_mediaStream *pMediaStream,
	 SdpMedia			*pSdpMedia,
	 Sdf_st_error		*pErr)
#endif
{
	Sdf_ty_s8bit	*pMid=Sdf_co_null,*pKey=Sdf_co_null,*pInfo=Sdf_co_null;
	Sdf_ty_u32bit	dConnectionCount=0;
	Sdf_ty_u32bit	j=0,dBandwidthCount=0;
	Sdf_ty_u32bit	dNumPort=0;


	/*Do Param Validation*/
	
	/*Assign a Mid for this media. This will be overwritten if the
	 * media has a mid in the attribute
	 */
	
	if(sdf_fn_uaGenerateMid(&pMid,pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
			"Failed to generate Mid",pErr);
#endif	
		sip_freeSdpMedia(pSdpMedia);
		return Sdf_co_fail;
	}
	pMediaStream->pMediaStreamId = pMid;
	
	/* Get Media Type */
	if(sdf_fn_uaExtractMediaTypeFromMeida(pSdpMedia,pMediaStream,\
		pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
	sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
		(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
		"Failed to get MediaType from Media",pErr);
#endif	
		sip_freeSdpMedia(pSdpMedia);
		return Sdf_co_fail;
	}
	/* Get Transport type */
	if(sdf_fn_uaExtractTransportTypeFromMedia(pSdpMedia,pMediaStream,\
			pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
	sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
		(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
		"Failed to get MediaType from Media",pErr);
#endif	
		sip_freeSdpMedia(pSdpMedia);
		return Sdf_co_fail;
	}
	
	/* Get port */
	if(sdp_getPortFromMedia(pSdpMedia,&(pMediaStream->dPort),\
		(SipError*)&pErr->stkErrCode)==SipFail)
	{	
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
	sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
		(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
		"Failed to get Port from Media",pErr);
#endif	
		sip_freeSdpMedia(pSdpMedia);
		return Sdf_co_fail;
	}
	
	/*Get Number of ports : OPTIONAL*/
	if(sdp_getNumportFromMedia(pSdpMedia,\
		&dNumPort,(SipError*)&pErr->stkErrCode)==SipFail)
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
			"Failed to get Port from Media",pErr);
#endif	
		sip_freeSdpMedia(pSdpMedia);
		return Sdf_co_fail;
		}
	}
	pMediaStream->dNumPort = dNumPort;
	/*Process codecs */
	if(sdf_fn_uaProcessCodecs(\
		pMediaStream, pSdpMedia, pErr)==Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
			"Failed to Process codecs from Media",pErr);
#endif	
		sip_freeSdpMedia(pSdpMedia);
		return Sdf_co_fail;
	}

	/*Process other Media Attributes*/
	if(sdf_fn_uaProcessMediaAttributes(pMediaStream,pSdpMedia,pErr)==\
				Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
			"Failed to Process Media Attributes",pErr);
#endif			
		sip_freeSdpMedia(pSdpMedia);
		return Sdf_co_fail;
	}
	
	/* Extract Connection lines*/	
	/* Get connection lines */
	if(sdp_getConnectionCountFromMedia(pSdpMedia,&dConnectionCount,\
				(SipError*)&pErr->stkErrCode)==SipFail)
	{	
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
			"Failed to get Connection line Count",pErr);
#endif	
		sip_freeSdpMedia(pSdpMedia);
		return Sdf_co_fail;
	}
	for(j=0;j<dConnectionCount;j++)
	{
		SdpConnection	*pConnection;
		if(sdp_getConnectionAtIndexFromMedia(pSdpMedia,&pConnection,j,\
					(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
			"Failed to get Connection ",pErr);
#endif	
			sip_freeSdpMedia(pSdpMedia);
			return Sdf_co_fail;
		}
		/*Append it to the List*/
		if(sdf_listAppend(&(pMediaStream->slConnection),(Sdf_ty_pvoid)\
			pConnection,pErr)==Sdf_co_fail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
				"Failed to Append Connection to list",pErr);
#endif	
			sip_freeSdpMedia(pSdpMedia);
			return Sdf_co_fail;
		}
	}
	/*Extract Bandwidth lines*/
	/* Get Bandwidth */
	if(sdp_getBandwidthCountFromMedia(pSdpMedia,&dBandwidthCount,\
				(SipError*)&pErr->stkErrCode)==SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
			"Failed to Append Connection to list",pErr);
#endif	
		sip_freeSdpMedia(pSdpMedia);
		return Sdf_co_fail;
	}
	for(j=0;j<dBandwidthCount;j++)
	{
		Sdf_ty_s8bit	*pBandwidth=Sdf_co_null;
		Sdf_ty_s8bit	*pTempBandwidth=Sdf_co_null;
		if(sdp_getBandwidthAtIndexFromMedia(pSdpMedia,&pBandwidth,j,\
					(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
			"Failed to get Bandwidth ",pErr);
#endif	
			sip_freeSdpMedia(pSdpMedia);
			return Sdf_co_fail;
		}
		Sdf_mc_strdup(pTempBandwidth,pBandwidth);
		
		/*Append it to the List*/
		if(sdf_listAppend(&(pMediaStream->slBandwidth),(Sdf_ty_pvoid)\
			pTempBandwidth,pErr)==Sdf_co_fail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
				"Failed to Append Bandwidth to list",pErr);
#endif	
			sip_freeSdpMedia(pSdpMedia);
			return Sdf_co_fail;
		}
	}
	
	/* Get Key */
	if(sdp_getKeyFromMedia(pSdpMedia,&pKey,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
					"Failed to Get Key",pErr);
#endif		
			sip_freeSdpMedia(pSdpMedia);
			return Sdf_co_fail;
		}

	}
	
	Sdf_mc_strdup(pMediaStream->pKey,pKey);
	
	/*Get Title*/
	if(sdp_getInfoFromMedia(pSdpMedia,&pInfo,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
		if(pErr->stkErrCode != Sdf_en_noExistError)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaStreamFromSdpMedia: "
					"Failed to get Title",pErr);
#endif		
			sip_freeSdpMedia(pSdpMedia);
			return Sdf_co_fail;
		}
	}
	Sdf_mc_strdup(pMediaStream->pTitle,pInfo);

	return Sdf_co_success;
}

#ifdef SDF_MEDIAGROUP
/*****************************************************************************
 ** FUNCTION: 	sdf_ivk_uaAddGroupToSession
 **
 ** DESCRIPTION: This function Forms a Group Attribute from the given Inputs
 **				 and adds it to the SessionParam
 **				 
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddGroupToSession
#ifdef ANSI_PROTO
	(Sdf_st_mediaHandling 	*pMediaInfo,
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_ty_groupType		dGroupType,
	 Sdf_ty_s8bit			*pOtherGroupType,
	 Sdf_ty_slist			*pslMid,
	 Sdf_st_error			*pErr)
#else
	(pMediaInfo,pSessionParam,dGroupType,pOtherGroupType,pslMid)
	Sdf_st_mediaHandling *pMediaInfo;
	Sdf_st_sessionParam		*pSessionParam;
	Sdf_ty_groupType		dGroupType;
	Sdf_ty_s8bit			*pOtherGroupType;
	Sdf_ty_list			*pslMid;
	Sdf_st_error		*pErr;
#endif
{

	Sdf_ty_u32bit	dNumNewStreamGroup=0;
	Sdf_st_mediaStreamGroup	*pNewGroup=Sdf_co_null;
	Sdf_st_listIterator		dListIterator;
	Sdf_st_mediaStreamGroup **ppNewMediaStreamGroups = Sdf_co_null;
	Sdf_ty_u32bit		dSize=0, i=0;
	Sdf_ty_u8bit		dIndex=0;

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR	
		sdf_fn_dumpError((Sdf_ty_s8bit *) \
			"SDF_ERROR-sdf_ivk_uaAddGroupToSession():" \
			"Invalid Error parameter passed to function");
#endif			
		return Sdf_co_fail;
	}
	if(pSessionParam == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddGroupToSession(): "
			"Invalid SessionParam argument passed",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
	if((pOtherGroupType == Sdf_co_null) && (dGroupType == Sdf_en_groupOther))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddGroupToSession(): "
			"Invalid Group Type Passed",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
	if(pslMid == Sdf_co_null) 
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddGroupToSession(): "
			"Invalid List Passed",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
#endif
	
	/*Validate the Incoming List*/
	(void)sdf_listSizeOf(pslMid,&dSize,pErr);

	if(dSize == 0)
	{
		pErr->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddGroupToSession(): "
			"Empty List Passed as Argument",pErr);
#endif
		return Sdf_co_fail;
	}
	
	/*Check if a Group Attribute can be added at this State*/
	if((pMediaInfo->dMediaState == Sdf_en_offerRecv) || 
			(pMediaInfo->dMediaState == Sdf_en_offerChangeRecv))
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddGroupToSession(): "
			"Cannot Add Group Attribute in Answer",pErr);
#endif
		return Sdf_co_fail;
	}

	/*Form the New Group Attribute*/
	if(sdf_ivk_uaInitMediaStreamGroup(&pNewGroup,pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddGroupToSession(): "
			"Failed to Initialize new MediaGroup.",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}
	/*Iterate through the List of Mid's and append the corresponding Index 
	 * to the slMediaStreamIndex
	 */
	(void)sdf_listInitIterator(pslMid,&dListIterator,pErr);

	while(dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_ty_s8bit	*pMid=Sdf_co_null;
		Sdf_ty_bool		dMidMatched=Sdf_co_false;

		pMid = (Sdf_ty_s8bit*)dListIterator.pCurrentElement->pData;

		for(i=0;i<pSessionParam->dNumMediaStreams;i++)
		{
			Sdf_st_mediaStream	*pMediaStream=Sdf_co_null;
		
			pMediaStream = pSessionParam->pMediaStreams[i];
		
			if(Sdf_mc_strcmp(pMediaStream->pMediaStreamId,pMid)==0)
			{
				/*Mid Matched. Add Index to the List*/
				Sdf_ty_u32bit	*pIndex=Sdf_co_null;
				pIndex = (Sdf_ty_u32bit*)sdf_memget( \
							Sdf_mc_sdpMemId,sizeof(Sdf_ty_u32bit),  pErr);
				if(pIndex == Sdf_co_null)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAddGroupToSession(): "
						"Failed to Initialize Index.",pErr);
#endif
					(void)sdf_ivk_uaFreeMediaStreamGroup(pNewGroup);
					pErr->errCode=Sdf_en_noMemoryError;
					return Sdf_co_fail;
				}
				*pIndex = i;
				/*w534*/(void)sdf_listAppend(&(pNewGroup->slMediaStreamIndex), \
						(Sdf_ty_pvoid)pIndex,	pErr);
				/*Set GroupType*/
				if(dGroupType == Sdf_en_groupOther)
					pNewGroup->pOtherGroupType = \
							Sdf_mc_strdupSdp(pOtherGroupType);
				else
					pNewGroup->dGroupType = dGroupType;

				dMidMatched = Sdf_co_true;
			}
					
		}
		if(dMidMatched == Sdf_co_false)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMatchingMid,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddGroupToSession(): "
				"No Matching Mid",pErr);
#endif
			(void)sdf_ivk_uaFreeMediaStreamGroup(pNewGroup);
			pErr->errCode=Sdf_en_noMatchingMid;
			return Sdf_co_fail;
		}

		(void)sdf_listNext(&dListIterator,pErr);
	}

	/*New Group Attribute is Formed*/

	dNumNewStreamGroup = pSessionParam->dNumGroups+1;

	/* Allocate for array of MediaGroup Pointers */
	ppNewMediaStreamGroups= (Sdf_st_mediaStreamGroup**) \
		sdf_memget( Sdf_mc_sdpMemId, \
		(sizeof(Sdf_st_mediaStreamGroup*) * dNumNewStreamGroup), pErr);

	if (ppNewMediaStreamGroups == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddGroupToSession(): "
			"Failed to Initialize new MediaGroup array.",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/* 
	 * First iterate through the list of MediaGroup present in 
	 * pSessionParams and assign them to the new array.
	 */
	for (dIndex = 0; dIndex <pSessionParam->dNumGroups; dIndex++)
	{
		Sdf_st_mediaStreamGroup	*pCurrMediaGroup=Sdf_co_null;

		if (sdf_ivk_uaGetMediaStreamGroupAtIndex(pSessionParam,\
			&pCurrMediaGroup, dIndex, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_sessionParamAccessError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddStreams( ): Failed "
				"to get MediaGroup at index from SessionParam", pErr);
#endif
			(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid *)\
						   &ppNewMediaStreamGroups, pErr);
			pErr->errCode=Sdf_en_sessionParamAccessError;
			return Sdf_co_fail;
		}
		/* Set the media stream. */
		ppNewMediaStreamGroups[dIndex] = pCurrMediaGroup;
	}
	/*Add the New Group Attribute*/	
	ppNewMediaStreamGroups[dIndex] = pNewGroup;

	/*New Group Attribute is Formed,Now Validate it*/
	if(sdf_fn_uaValidateGroupings(pSessionParam,ppNewMediaStreamGroups,\
					dNumNewStreamGroup,pErr)==Sdf_co_false)
	{
		for(i=0;i<dNumNewStreamGroup;i++)
		{
			(void)sdf_ivk_uaFreeMediaStreamGroup(ppNewMediaStreamGroups[i]);
		}
		(void)sdf_memfree(Sdf_mc_freeMemId,\
				(Sdf_ty_pvoid*)&(ppNewMediaStreamGroups), \
						(Sdf_st_error*)Sdf_co_null);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_sessionParamAccessError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddGroupToSession( ): Failed "
			"to Add Group to Session", pErr);
#endif
		return Sdf_co_fail;
	}

	/*Free the Old Media Group Strcuture*/
	for(i=0;i<pSessionParam->dNumGroups;i++)
	{
		(void)sdf_ivk_uaFreeMediaStreamGroup(pSessionParam->pMediaStreamGroups[i]);
	}
	/*Free the array of pointers */
	(void)sdf_memfree(Sdf_mc_sdpMemId,(Sdf_ty_pvoid*)&(pSessionParam->pMediaStreamGroups),pErr);

	pSessionParam->dNumGroups = dNumNewStreamGroup;
	pSessionParam->pMediaStreamGroups = ppNewMediaStreamGroups;

	return Sdf_co_success;
	
}
#endif

/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaSetSessionCapability
 **
 ** DESCRIPTION: This function makes a default SDP body using the initData 
 **				 structure of the call object and sets the body formed into 
 **				 the MsgBodyList of the CallObject 
 **
 ****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaSetSessionCapability
#ifdef ANSI_PROTO
	(Sdf_st_mediaHandling *pMediaInfo, 
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_st_error *pError)
#else	
	(pMediaInfo, pSessionParam, pError)
	Sdf_st_mediaHandling *pMediaInfo;
	Sdf_st_sessionParam	*pSessionParam;
	Sdf_st_error *pError;
#endif
{
	(void)pError;
#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR	
		sdf_fn_dumpError((Sdf_ty_s8bit *)
		"SDF_ERROR-sdf_ivk_uaSetSessionCapability():" \
			"Invalid Error parameter passed to function");
#endif			
		return Sdf_co_fail;
	}
	if(pSessionParam == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSessionCapability(): "
			"Invalid SessionParam argument passed",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
	if(pMediaInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError , \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetSessionCapability(): "
			"Invalid Media-Info argument passed",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError ;
		return Sdf_co_fail;
	}
#endif
	HSS_LOCKEDINCREF(pSessionParam->dRefCount);
	pMediaInfo->pCapabilities = pSessionParam;

	return	Sdf_co_success;
		
}
/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaFillDefaultSdpInMsgBodyList
 **
 ** DESCRIPTION: This function makes a SDP body using pMedaInfo->pCapabilities 
 **				 structure in MediaInfo of call object and sets the body formed into 
 **				 the MsgBodyList Passed as Parameter 
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFillDefaultSdpInMsgBodyList
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, 
	Sdf_ty_slist *pMsgBodyList,
	Sdf_st_error *pError)
#else	
	(pObject, pMsgBodyList, pError)
	Sdf_st_mediaHandling *pMediaInfo;
	Sdf_st_callObject *pObject;
	Sdf_ty_slist *pMsgBodyList;
	Sdf_st_error *pError;
#endif
{
	SipMsgBody *pMsgBody = Sdf_co_null;
	Sdf_st_sessionParam		*pSessionParam=Sdf_co_null;
	SdpMessage		*pSdpMessage=Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaFillDefaultSdpInMsgBodyList");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Error checks in the input parameters 
	 */
	if (pError == Sdf_co_null)
		return Sdf_co_fail;
#endif	/* End of param validation ifdef */

	/*Initiate Offer*/
	if(sdf_ivk_uaInitiateOffer(&pSessionParam,pObject,Sdf_en_newOffer,\
				pError)==Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFillDefaultSdpInMsgBodyList(): "
			"Cannot Initiate a Offer at Current Media State", pError);
#endif		
		return Sdf_co_fail;
	}
	
	/*Convert the SessionParam into SdpMessage*/
	if(sip_initSdpMessage(&pSdpMessage,(SipError*)&pError->stkErrCode)==SipFail)
	{
		pError->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFillDefaultSdpInMsgBodyList(): "
			"Failed to Init SdpMessage", pError);
#endif		
		return Sdf_co_fail;
	}
	
	if( Sdf_co_fail == sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMessage, \
			  pSessionParam,Sdf_en_convertLocal, pError))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFillDefaultSdpInMsgBodyList(): "
			"Failed to Init SdpMessage", pError);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}
	/*Set LastOfferMade*/
	if(sdf_ivk_uaSetLastOfferMade(pObject->pMediaInfo,pSessionParam,pError) \
			==Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFillDefaultSdpInMsgBodyList(): "
			"Failed to Set Last Offer Made", pError);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}
	/*Free the Session Param*/
	(void)sdf_ivk_uaFreeSessionParam(pSessionParam);
	if (sip_initSipMsgBody(&pMsgBody, SipSdpBody, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFillDefaultSdpInMsgBodyList(): "
			"Failed to Init SipMsgBody", pError);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}
	if (sip_setSdpInMsgBody(pMsgBody, pSdpMessage, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFillDefaultSdpInMsgBodyList(): "
			"Failed to Set Sdp in MsgBody", pError);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}
	sip_freeSdpMessage(pSdpMessage);

	/* =======================================
		 Remove any existing SdpMsgBody's from 
		 the slMsgBodyList 
	   =======================================*/
	if (sdf_fn_uaDeleteSdpFromMsgBodyList(pMsgBodyList, pError) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFillDefaultSdpInMsgBodyList( ): "
			"Error in deleting SDP message present in the Msg Body List",\
			pError);
#endif		
		pError->errCode=Sdf_en_sdpError;
		sip_freeSipMsgBody(pMsgBody);
		return Sdf_co_fail;
	}
	/* =======================================
		Add this MsgBody into the MsgBodyList 
	   =======================================*/
	if (sdf_listAppend(pMsgBodyList, pMsgBody, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFillDefaultSdpInMsgBodyList( ): "
			"Error while adding the SDP message to the Msg Body List", pError);
#endif		
		pError->errCode=Sdf_en_callObjectAccessError;
		sip_freeSipMsgBody(pMsgBody);
		return Sdf_co_fail;
	}	

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaFillDefaultSdpInMsgBodyList");
	return Sdf_co_success;
}

/****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaFormMediaCapabilityDescription
 **
 ** DESCRIPTION: 	This function forms a SipMsgBody structure from the 
 **					SdpMessage of the initData present in the call object. 
 **					At the end of the function, a SipMsgBody is returned 
 **					which has all the lines of the SdpMessage in the 
 **					initData + other mandatory lines which might have been
 **					skipped in the former. (i.e. v=/o=/c=/s=/t=)
 **
 **					Note:
 **					The initialization for the SipMsgBody struct happens 
 **					inside the function.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormMediaCapabilityDescription
#ifdef ANSI_PROTO
	(Sdf_st_mediaHandling *pMediaInfo, \
	SdpMessage **ppCapabilities, \
	Sdf_st_error *pError)
#else
	(pMediaInfo, ppCapabilities, pError)
	Sdf_st_mediaHandling *pMediaInfo;
	SdpMessage **ppCapabilities;
	Sdf_st_error *pError;
#endif	
{
	Sdf_st_sessionParam	*pCapabilities=Sdf_co_null;
	SdpMessage *pSdpMessage=Sdf_co_null;
	SdpOrigin *pOrigin=Sdf_co_null;
	SdpTime *pSdpTime=Sdf_co_null;
	Sdf_ty_u32bit i,j,dSize,dCount = 0,dRand;
	Sdf_ty_s8bit dTemp[Sdf_co_smallBufferSize];

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Entering "
		" sdf_ivk_uaFormMediaCapabilityDescription");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
		return Sdf_co_fail;
	if (pMediaInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaCapabilityDescription( ): "
			"Invalid param passed to function: pMediaInfo", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif	/* End of param validation ifdef */

	*ppCapabilities=Sdf_co_null;

	if(sdf_ivk_uaGetSessionParamsFromMediaInfo(pMediaInfo,&pCapabilities,\
		Sdf_en_selfCapabilities,pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMediaCapabilityDescription(): "
			"Getting Capability from MediaInfo failed",pError);
#endif
		pError->errCode=Sdf_en_sdpError;
		(void)sdf_ivk_uaFreeMediaHandling(pMediaInfo);
		return Sdf_co_fail;
	}

	/*
     * Convert the SessionParams structure into SdpMessage.
	 */

	/* Initialise the SDP Message */
	if (sip_initSdpMessage(&pSdpMessage, (SipError*)&(pError->stkErrCode))\
		     == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaCapabilityDescription( ): "
			"Failed to allocate memory for the SDP message", pError);
#endif		
	    (void)sdf_ivk_uaFreeSessionParam(pCapabilities);
		pError->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
    }

	if (sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMessage, pCapabilities,\
			Sdf_en_convertLocal, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMediaCapabilityDescription( ):  "
			"Failed to make SdpMessage from SessionParams",pError);
#endif
	    (void)sdf_ivk_uaFreeSessionParam(pCapabilities);
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
    }

	/* Free the local reference. */
	(void)sdf_ivk_uaFreeSessionParam(pCapabilities);

	/*Iterate through All Media Streams and Set their Ports to 0*/
	
	/*w534*/(void)sdp_getMediaCount(pSdpMessage,&dCount, \
							(SipError*)&pError->stkErrCode);
	
	for(i=0;i<dCount;i++)
	{
		SdpMedia *pSdpMedia = Sdf_co_null;

		/*Get media Stream*/
		if(sdp_getMediaAtIndex(pSdpMessage,&pSdpMedia,i,\
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			pError->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaCapabilityDescription( ): "
				"Failed to get Media", pError);
#endif			
			sip_freeSdpMessage(pSdpMessage);
			return Sdf_co_fail;
		}
	
		/*Set Port to 0 */
		if(sdp_setPortInMedia(pSdpMedia,(Sdf_ty_u16bit)0,\
				(SipError*)&(pError->stkErrCode))==SipFail)
		{
			pError->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaCapabilityDescription( ): "
				"Failed to Set port", pError);
#endif			
			sip_freeSdpMedia(pSdpMedia);
			sip_freeSdpMessage(pSdpMessage);
			return Sdf_co_fail;
		}
		
		if(pSdpMedia->pPortNum != Sdf_co_null)
		{
			(void)sdf_memfree(0,(Sdf_ty_pvoid*)&(pSdpMedia->pPortNum),pError);
			pSdpMedia->pPortNum = Sdf_co_null;
		}

		/* Removed the  "a=curr:qos xxxxxxx" lines to ensure that the
		 * outgoing capability contains only desired QoS attibutes
		 */

		/*w534*/(void)sdp_getAttrCountFromMedia(pSdpMedia,&dSize,(SipError*)&(\
				pError->stkErrCode));
		for(j=0; j<dSize;j++)
		{
			SdpAttr *pTempAttr=Sdf_co_null;
			Sdf_ty_s8bit *pAttrName=Sdf_co_null;

			if (sdp_getAttrAtIndexFromMedia(pSdpMedia, &pTempAttr, \
					j, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaCapability"
					"Description:Failed to get Attribute from the"
					" Media",pError);
#endif					
				pError->errCode=Sdf_en_sdpError;
				sip_freeSdpMedia(pSdpMedia);
				sip_freeSdpMessage(pSdpMessage);
				return Sdf_co_fail;
			}

			if(sdp_getNameFromAttr(pTempAttr,&pAttrName, \
					(SipError*)&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode ;
#ifdef SDF_ERROR							
				sdf_fn_setError(Sdf_en_minorErrors, \
					pError->errCode, (Sdf_ty_s8bit*) \
					"sdf_ivk_uaFormMediaCapabilityDescription(): "
					"Failed to Get Name From Attribute",pError);
#endif							
				sip_freeSdpAttr(pTempAttr);
				sip_freeSdpMedia(pSdpMedia);
				sip_freeSdpMessage(pSdpMessage);
				return Sdf_co_fail;
			}
			sip_freeSdpAttr(pTempAttr);
			if(Sdf_mc_strcasecmp(pAttrName,"curr")==0)
			{
				if(sdp_deleteAttrAtIndexInMedia(pSdpMedia,j,\
					(SipError*)&(pError->stkErrCode))== SipFail)
				{

					pError->errCode = pError->stkErrCode ;
#ifdef SDF_ERROR							
					sdf_fn_setError(Sdf_en_minorErrors, \
						pError->errCode, (Sdf_ty_s8bit*) \
						"sdf_ivk_uaFormMediaCapabilityDescription(): "
						"Failed to delete Attr From Media",pError);
#endif							
					sip_freeSdpMedia(pSdpMedia);
					sip_freeSdpMessage(pSdpMessage);
					return Sdf_co_fail;
				}
				j=j-1;
				dSize=dSize-1;
			}
		}

		sip_freeSdpMedia(pSdpMedia);
	}

	/* Set "t=" line in the capability  to "t=0 0". */
	if(sdp_getTimeAtIndex(pSdpMessage,&pSdpTime,(Sdf_ty_u32bit)0,\
		(SipError *)&(pError->stkErrCode))==SipFail)
	{
		pError->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaCapabilityDescription( ): "
				"Failed to get Time From the SDP", pError);
#endif			
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}

	if (sdp_setStartInTime(pSdpTime,Sdf_mc_strdupSdp("0"),\
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaCapabilityDescription( ): "
			"Failed set Start Time", pError);
#endif			
		sip_freeSdpTime(pSdpTime);
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}

	if (sdp_setStopInTime(pSdpTime,Sdf_mc_strdupSdp("0"),\
		(SipError *)&(pError->stkErrCode))==SipFail)
	{
		pError->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaCapabilityDescription( ): "
			"Failed set Stop Time", pError);
#endif			
		sip_freeSdpTime(pSdpTime);
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}
	/* Free the Local Data*/
	sip_freeSdpTime(pSdpTime);

	/* Set the session-ID in the "o=" line different from the previos
	 * offer/answer
	 */
	
	if(sdp_getOrigin(pSdpMessage,&pOrigin,\
		(SipError *)&(pError->stkErrCode))==SipFail)
	{
		pError->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaCapabilityDescription( ): "
				"Failed to get Origin From the SDP", pError);
#endif			
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}
	(void)sdf_fn_uaGenerateRand(&dRand);
	(void)sdf_fn_uaSprintf(dTemp,"%d",dRand);

	if (sdp_setSessionIdInOrigin(pOrigin, Sdf_mc_strdupSdp(dTemp),\
		(SipError*) &(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode ;
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
			(Sdf_ty_s8bit*)"sdf_fn_uaSetOriginInfoInSdp(): "
			"Failed to set session id in Origin Structure",pError);
#endif			
		return Sdf_co_fail;
	}
	sip_freeSdpOrigin(pOrigin);

	/*Get Attr Count*/
	 /*w534*/(void)sdp_getAttrCount(pSdpMessage,&dSize, (SipError *)&(pError->stkErrCode));
	/*Add a=inactive line*/
	if(SipFail == sdp_insertSdpAttrAtIndexFromString(pSdpMessage,\
		(Sdf_ty_s8bit*)"a=inactive", (Sdf_ty_s8bit)dSize, \
		(SipError*)&pError->stkErrCode))
	{
		pError->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMediaCapabilityDescription( ): "
			"Failed to Insert inactive Attribute", pError);
#endif			
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}

	*ppCapabilities=pSdpMessage;

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Exiting "
		"sdf_ivk_uaFormMediaCapabilityDescription");
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaUpdateLocalSession
 **
 ** DESCRIPTION: 
 **				   If dAddStream = Sdf_co_true, then list contains the 
 **				   list of media streams to be added.
 **				   If dAddStream = Sdf_co_false, then list contains the 
 **					list of mids to be deleted.
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaUpdateLocalSession
#ifdef ANSI_PROTO
	(Sdf_st_mediaHandling *pMediaInfo,
	 Sdf_ty_slist		*slMediaList,
	 Sdf_ty_bool		dAddStream,
	 Sdf_st_error		*pError)
#else
	(pMediaInfo, slMediaList, dAddStream, pError)
	 Sdf_st_mediaHandling *pMediaInfo;
	 Sdf_ty_slist		*slMediaList;
	 Sdf_ty_bool		dAddStream;
	 Sdf_st_error		*pError;
#endif
{
	if(dAddStream == Sdf_co_true)
	{
	   /*
        * Add the given streams to the SessionParams.
	    */
		if (sdf_ivk_uaAddStreams(pMediaInfo->pCapabilities, 
			*slMediaList, pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateLocalSession( ):  "
				"Addition of new Streams Failed.",pError);
#endif
			return Sdf_co_fail;
    	}

	}
	else /* Delete stream */
	{
		/*
		 * Delete the given streams from the SessionParams.
		 */
		if (sdf_ivk_uaDeleteStreams(pMediaInfo->pCapabilities, 
				Sdf_co_null, slMediaList, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateLocalSession( ):  "
				"Deletion of new Streams Failed.",pError);
#endif
			return Sdf_co_fail;
		}
	}
	return Sdf_co_success;
}


#ifdef SDF_REFER 
/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetSipfragBodyFromSipMessage
**
** DESCRIPTION: This API will check for the presence of the sipfrag 
**		message body from the sip message and returns SipMessage equivalent
**		of the message body.
**
** PARAMETERS:
**		pInSipMsg(IN)	: The SipMessage which contains Sipfrag body
**		ppOutSipMsg(OUT): The output SipMessage which was obtained by parsing
**							the Sipfrag message body present in pInSipMsg
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSipfragBodyFromSipMessage
#ifdef ANSI_PROTO
	(SipMessage *pInSipMsg,
	 SipMessage **ppOutSipMsg,
	 Sdf_st_error *pErr)
#else
	( pInSipMsg, ppOutSipMsg, pErr )
	 SipMessage *pInSipMsg;
	 SipMessage **ppOutSipMsg;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dCount=0, dIndex;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
			"sdf_ivk_uaGetSipfragBodyFromSipMessage");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaGetSipfragBodyFromSipMessage( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if ((pInSipMsg == Sdf_co_null) || (ppOutSipMsg == Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSipfragBodyFromSipMessage():"
			"Invalid SipMsg passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* end-of-param-validation */
	
	/* Get the total number of Message bodies present in the SipMessage */
	if (SipFail == sip_getMsgBodyCount(pInSipMsg, &dCount, 
				(SipError *)&(pErr->stkErrCode)))
	{
		pErr->errCode = Sdf_en_msgBodyError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaGetSipfragBodyFromSipMessage:"
				"Failed to get MessageBody count from SipMessage", pErr);
#endif
		return Sdf_co_fail;
	}
	/* Iterate through the message body list & fetch message/sipfrag type */
	for (dIndex=0; dIndex<dCount; dIndex++)
	{
		SipMsgBody 		*pMsgBody = Sdf_co_null;
		
		/* Get the message body at current index */
		if (SipFail == sip_getMsgBodyAtIndex(pInSipMsg, &pMsgBody, dIndex, \
				(SipError *)&(pErr->stkErrCode)))
		{
			pErr->errCode = Sdf_en_msgBodyError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit*)"sdf_ivk_uaGetSipfragBodyFromSipMessage:"
					"Failed to get MessageBody from SipMessage", pErr);
#endif
			return Sdf_co_fail;
		}
		/* Check if the MessageBodyType == SipAppSipBody */
		if (pMsgBody->dType == SipAppSipBody)
		{
			/* 
			 * If a message/sipfrag Body is received in any request, 
			 * stack parses the message body into SipMessage structure.
			 * Hence, we need to fetch the message body as a SipMessage
			 * structure and return the same. 
			 */
			if (SipFail == sip_getSipMessageFromMsgBody(pMsgBody, 
						ppOutSipMsg, (SipError*)&(pErr->stkErrCode)))
			{
				sip_freeSipMsgBody(pMsgBody);
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit*)"sdf_ivk_uaGetSipfragBodyFromSipMessage:"
					"Failed to get SipMessage from MessageBody", pErr);
#endif
				return Sdf_co_fail;
			}
			/* We have found a message body */
			sip_freeSipMsgBody(pMsgBody);

			/* Stop iterating & just return */
			return Sdf_co_success;
		}
		/* The current message body is not of type message/sipfrag
		 * Iterate to the next message body */
		sip_freeSipMsgBody(pMsgBody);
	}/* end-of-for */
			
	/* No message body of type message/sipfrag was found */
	pErr->errCode = Sdf_en_noExistError;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_ivk_uaGetSipfragBodyFromSipMessage");
	return Sdf_co_fail;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormAndAddSipfragBodyToSipMessage
**
** DESCRIPTION: This API forms the sipfrag messagebody from the given the input
**              sip message and adds the formed body into the output SipMessage
**              This API also adds the Content-Type header as "message/sipfrag"
**				in the output Sip message
** PARAMETERS:
**		pSipMsg(IN)		: The SipMessage to be set in ppSipMsg's sipfrag body
**		ppSipMsg(OUT)	: The output Sip message
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormAndAddSipfragBodyToSipMessage 
#ifdef ANSI_PROTO
	(SipMessage *pSipMsg, SipMessage **ppSipMsg, Sdf_st_error *pErr)
#else
	(pSipMsg, ppSipMsg, pErr)
	SipMessage *pSipMsg; 
	SipMessage **ppSipMsg;
	Sdf_st_error *pErr;
#endif
{
	SipMsgBody		*pMsgBody = Sdf_co_null;
	Sdf_ty_s8bit	*pContentType = Sdf_co_null;
	SipHeader		*pHeader = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
			"sdf_ivk_uaFormAndAddSipfragBodyToSipMessage");
	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaFormAndAddSipfragBodyToSipMessage( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if ((pSipMsg == Sdf_co_null) || (*ppSipMsg == Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormAndAddSipfragBodyToSipMessage():"
			"Invalid SipMsg passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* end-of-param-validation */
	
	if (sip_initSipMsgBody(&pMsgBody, SipAppSipBody, (SipError*)\
		&(pErr->stkErrCode)) ==  SipFail)
	{
		pErr->errCode = Sdf_en_msgBodyError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormAndAddSipfragBodyToSipMessage( ): "
		 	"sip_initSipMsgBody failed",pErr);
#endif
		return Sdf_co_fail;
	}
	if(sip_setSipMessageInMsgBody(pMsgBody, pSipMsg,
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		sip_freeSipMsgBody(pMsgBody);
		(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pMsgBody,pErr);
		pErr->errCode=Sdf_en_msgBodyError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormAndAddSipfragBodyToSipMessage( ): "
		 	"sip_setSipMessageInMsgBody failed",pErr);
#endif
		return Sdf_co_fail;
	}
	/*
	 * Set the message body in the sip message
	 */
	if (sip_insertMsgBodyAtIndex(*ppSipMsg, pMsgBody, 0, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
		sip_freeSipMsgBody(pMsgBody);
		(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pMsgBody,pErr);
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndAddSipfragBodyToSipMessage( ): "
				"Failed to insert message body into the SipMessage", \
				pErr);
#endif				
		pErr->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}
	/* Free Local Reference */
	sip_freeSipMsgBody(pMsgBody);
	
	/*
	 * Add the content-type as message/sipfrag
	 */
    if(sip_initSipHeader(&pHeader,SipHdrTypeContentType, \
           (SipError*)&(pErr->stkErrCode)) == SipFail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormAndAddSipfragBodyToSipMessage( ): "
		 	"sip_initSipHeader SipHdrTypeContentType failed",pErr);
#endif
		return Sdf_co_fail;
    }     	
    
	Sdf_mc_strdup(pContentType, "message/sipfrag");
	
    if(sip_setMediaTypeInContentTypeHdr(pHeader, \
          pContentType, (SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHeader,pErr);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_sdpError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormAndAddSipfragBodyToSipMessage( ): "
		 	"sip_setMediaTypeInContentTypeHdr failed",pErr);
#endif
		return Sdf_co_fail;
    }
	if (sip_insertHeaderAtIndex(*ppSipMsg, pHeader, 0, \
		(SipError*)&(pErr->stkErrCode))==SipFail)
    {
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHeader,pErr);
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_sdpError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormAndAddSipfragBodyToSipMessage( ): "
		 	"sip_insertHeaderAtIndex in SipMsg failed",pErr);
#endif
		return Sdf_co_fail;
	}
	/* Free Local Reference */
	sip_freeSipHeader(pHeader);
	(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHeader,pErr);

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_ivk_uaFormAndAddSipfragBodyToSipMessage");
	return Sdf_co_success;
}
#endif /*SDF_REFER */

#ifdef SDF_IM
/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg 
 **
 ** DESCRIPTION: This function creates a message body of type text/plain from
 **				the text passed as input and Sets to the sip message passed. 
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg
#ifdef ANSI_PROTO
	(SipMessage *pSipMsg, Sdf_ty_s8bit	*pPlainText, Sdf_st_error *pError)
#else
	(pSipMsg, pPlainText, pError)
	SipMessage 		*pSipMsg; 
	Sdf_ty_s8bit	*pPlainText;
	Sdf_st_error 	*pError;
#endif
{
	SipMsgBody 	 *pMsgBody 		= Sdf_co_null;
	Sdf_ty_s8bit *pContentType 	= Sdf_co_null;
	SipHeader	 *pHeader 		= Sdf_co_null;
	 Sdf_st_msgInfo	dMessageInfo;
	
#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR	
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg(): "
			"Invalid Error parameter passed to function");
#endif			
		return Sdf_co_fail;
	}
	if (pSipMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg(): "
			"Invalid param passed to function: SipMessage", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pPlainText == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg( ): "
			"No text to form the message body", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/*w534*/(void)sdf_fn_uaGetMessageInfo(pSipMsg,  &dMessageInfo, pError);

	if( (dMessageInfo.dReqRespType == SipMessageResponse) && \
		(dMessageInfo.dMsgType == Sdf_en_message))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_messageBodyNotAllowed, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg(): "
			"Message body is not allowed in this message", pError);
#endif		
		pError->errCode = Sdf_en_messageBodyNotAllowed;
		return Sdf_co_fail;
	}

	if(sdf_ivk_uaCreateMessageBodyFromBuffer(&pMsgBody, \
		(Sdf_ty_s8bit *)"text/plain", pPlainText, Sdf_mc_strlen(pPlainText),\
		Sdf_co_false, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg( ): "
		 	"sdf_ivk_uaCreateMessageBodyFromBuffer failed",pError);
#endif
		return Sdf_co_fail;
	}
	/*
	 * Set the message body in the sip message
	 */
	if (sip_insertMsgBodyAtIndex(pSipMsg, pMsgBody, 0, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipMsgBody(pMsgBody);
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg():"\
			"Failed to insert message body into the SipMessage", pError);
#endif				
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}
	sip_freeSipMsgBody(pMsgBody);
	
	/*
	 * Add the content-type as text/plain
	 */
    if(sip_initSipHeader(&pHeader,SipHdrTypeContentType, \
           (SipError*)&(pError->stkErrCode)) == SipFail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg():"\
		 	"sip_initSipHeader SipHdrTypeContentType failed",pError);
#endif
		return Sdf_co_fail;
    }     	
    
	Sdf_mc_strdup(pContentType, "text/plain");
	
    if(sip_setMediaTypeInContentTypeHdr(pHeader, \
          pContentType, (SipError*)&(pError->stkErrCode)) == SipFail)
	{
		(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid *)&pContentType, pError);
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid *)&pHeader, pError);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg():"\
		 	"sip_setMediaTypeInContentTypeHdr failed",pError);
#endif
		return Sdf_co_fail;
    }
	if (sip_setHeader(pSipMsg, pHeader, \
		(SipError*)&(pError->stkErrCode))==SipFail)
    {
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid *)&pHeader, pError);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg():"\
		 	"sip_setHeader in SipMsg failed",pError);
#endif
		return Sdf_co_fail;
	}
	sip_freeSipHeader(pHeader);
	(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid *)&pHeader, pError);
	return Sdf_co_success;
}
#endif


