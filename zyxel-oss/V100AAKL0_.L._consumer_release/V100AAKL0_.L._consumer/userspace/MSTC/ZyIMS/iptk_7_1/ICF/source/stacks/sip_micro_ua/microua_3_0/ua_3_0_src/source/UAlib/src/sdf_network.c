 /** FUNCTION		: This file has all the network related funtions
 **
 *****************************************************************************
 **
 ** FILENAME		: sdf_network.c
 **
 ** DESCRIPTION		: This file has all the network related funtions
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 27-Oct-00	Subhash						Creation
 ** 19-Jul-02	Siddharth					Added transaction layer 
 **											integration.
 ** 22/04/03    Anil Kumar     ---          Changes for TLS feature.
 ** 09/04/10    Alok Tiwari  CSR 1-8223807  UATK CSR 1-8290042 Merged
 ** 20-Apr-2010  Rohit Aggarwal  SPR 20381  Added null check for hdr list
 **
 *****************************************************************************
 **				COPYRIGHT , Aricent, 2010
 *****************************************************************************/


#include "ALheader.h"
#include "sdf_struct.h"
#include "sdf.h"
#include "sdf_network.h"
#include "sdf_accessor.h"
#include "sdf_init.h"
#ifndef SDF_LINT
#include "sdf_free.h"
#include "sdf_list.h"
#endif
#include "sdf_internal.h"
#include "sdf_sdpinternal.h"
#include "sdf_reqresp.h"
#include "sdf_debug.h"
#include "sdf_trace.h"
#include "sdf_hash.h"
#include "sdf_portlayer.h"
#include "sdf_callbacks.h"
#include "sdf_sdp.h"
#include "sdf_sdpaccessor.h"
#include "sdf_sdpfree.h"

#ifdef SDF_SESSION_TIMER
#include "sdf_sessTimer.h"
#endif

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaSendCallToPeer
 **
 ** DESCRIPTION: Sends the message across the network to the peer entity
 ** 			 and updates the remote retransmission list. It also changes 
 **				 the call state of the call. Destination to which
 **				 the message has to be sent is picked from the common info's
 **				 destination info member (this has been filled already before
 **				 invokation of this API inside FormRequest or FormResponse).
 **
 *****************************************************************************/
/* CSR_1-8290042 changes start */
Sdf_ty_retVal sdf_ivk_uaSendCallToPeer
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	SipMessage *pSipMsg,
	SipOptions dOptions,
	Sdf_ty_s8bit transptype,
	Sdf_st_eventContext *pContext,
    Sdf_ty_s8bit *headerlist,
	Sdf_st_error *pError)
#else 	
	(pObject, pSipMsg, dOptions, transptype, pContext, pError)
	Sdf_st_callObject *pObject;
	SipMessage *pSipMsg;
	SipOptions dOptions;
	Sdf_ty_s8bit transptype;
	Sdf_st_eventContext *pContext;
    Sdf_ty_s8bit *headerlist;
	Sdf_st_error *pError;
#endif
/* CSR_1-8290042 changes end */
{
   /* CSR_1-8290042 changes start */
    Sdf_ty_s8bit *tempstr;
    en_HeaderType dHeaderType;
    Sdf_ty_u32bit dIndex = 0;
    Sdf_ty_u32bit dCount = 0;
    SipMessage *pClonedMsg;
   /* CSR_1-8290042 changes end */

	Sdf_st_transaction	 *pTransaction	= Sdf_co_null;
#ifndef SDF_UASSF
	SipTranspAddr		 *pTranspaddr	= Sdf_co_null;
	Sdf_st_eventContext  *pEventContext	= Sdf_co_null;
	Sdf_st_transportInfo *pDestInfo		= Sdf_co_null;
	Sdf_st_socket		  *pSocket; 
	/*CSR 1-6712729 Change Start*/ 
	/*Sdf_st_error		  dLocalErr;*/
	/*CSR 1-6712729 Change End*/
#ifdef SDF_USE_PREALLOCBUFFERS
	Sdf_ty_u32bit dLength;
#endif
#endif

#ifdef SDF_LINT
#ifndef SDF_UASSF    
    Sdf_ty_messageType	  dMethodType	= Sdf_en_unknownRequest;
#endif    
#else
    Sdf_ty_messageType	  dMethodType	= Sdf_en_unknownRequest;
#endif    

   	en_SipMessageType     dReqRespType;

	Sdf_st_overlapTransInfo *pOverlapTransInfo = Sdf_co_null;
	Sdf_st_msgInfo		  dMessageInfo;

	(void)transptype;
	(void)dOptions;
	(void)pContext;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSendCallToPeer\n");

   /* CSR_1-8290042 changes start */

   if(sip_initSipMessage(&pClonedMsg, \
		 	pSipMsg->dType, (SipError *)&(pError->stkErrCode)) ==SipFail)
   {
#ifdef SDF_ERROR
       sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *) "Initializing SipMessage failed ",pError);
#endif
       pError->errCode=Sdf_en_headerManipulationError;
       return Sdf_co_fail;
   }

    
   if(sip_cloneSipMessage(pClonedMsg, pSipMsg,\
		(SipError *)&(pError->stkErrCode))==SipFail)
   {
#ifdef SDF_ERROR
      sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
            (Sdf_ty_s8bit *)"Cloning SipMessage failed ",pError);
#endif
      pError->errCode=Sdf_en_headerManipulationError;
      sip_freeSipMessage(pClonedMsg);
      return Sdf_co_fail;
   }

	/* Start fix for CSR 1-8516341: SPR 20381 */
	if (headerlist != NULL)
	{
	/* End fix for CSR 1-8516341: SPR 20381 */
   tempstr = Sdf_mc_strtok(headerlist,",");
   
   if(tempstr != NULL)
   {
      sip_getTypeFromName(tempstr,&dHeaderType,(SipError *)&\
		(pError->stkErrCode));  
      (void)sip_getHeaderCount(pClonedMsg,dHeaderType, &dCount, \
			(SipError *)&(pError->stkErrCode));
      for(dIndex = 0; dIndex < dCount; dIndex++)
      {    
         sip_deleteHeaderAtIndex(pClonedMsg,dHeaderType,0,(SipError *)&\
		                           (pError->stkErrCode));
      }
       
   }        

   while(1)
   {
       tempstr = Sdf_mc_strtok(NULL,",");
        
       if(tempstr == NULL)
           break;       
        
       sip_getTypeFromName(tempstr,&dHeaderType,(SipError *)&\
		(pError->stkErrCode));  
       (void)sip_getHeaderCount(pClonedMsg,dHeaderType, &dCount, \
    	(SipError *)&(pError->stkErrCode));
       for(dIndex = 0; dIndex < dCount; dIndex++)
       {    
         sip_deleteHeaderAtIndex(pClonedMsg,dHeaderType,0,(SipError *)&\
		                           (pError->stkErrCode));
       }

   }
	/* Start fix for CSR 1-8516341: SPR 20381 */
	}
	/* End fix for CSR 1-8516341: SPR 20381 */
        
  /*  CSR_1-8290042 changes end */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaSendCallToPeer( ): Invalid Error parameter");
#endif
        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
	}	

	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
			" Call object parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
	}	
#endif

	if( Sdf_co_success != sdf_fn_uaGetMessageInfo( pSipMsg, &dMessageInfo, \
		pError))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
			"Error in getting Message Info SIP Message",pError);
#endif
        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
	}

#ifdef SDF_LINT
#ifndef SDF_UASSF   
	dMethodType   = dMessageInfo.dMsgType;		
#endif /* End of ifndef SDF_UASSF */
#else /* Else of SDF_LINT */
	dMethodType   = dMessageInfo.dMsgType;	
#endif /* End of SDF_LINT */

	dReqRespType  = dMessageInfo.dReqRespType;

#ifdef SDF_RPORT
  /* Add the rport param in the via header */
	if ((pObject->pInitData != Sdf_co_null) && 
      (pObject->pInitData->dEnableRport == Sdf_co_true))
	{
		if(dReqRespType == SipMessageRequest)
		{
			if ( sdf_ivk_uaAddViaRportParam(pSipMsg, pError) != Sdf_en_success )
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer(): "
					"Addition of rport in via header failed",pError);
#endif
               /* CSR_1-8290042 changes start */
                sip_freeSipMessage(pClonedMsg);
               /* CSR_1-8290042 changes end */
				return Sdf_co_fail;
			}
		}
	}
#endif /* End of SDF_RPORT */

	if (sdf_fn_uaDoSendMessageProcs(pObject, pSipMsg, &pOverlapTransInfo,\
			dMessageInfo, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
			"sdf_fn_uaDoSendMessageProcs Failed ",pError);
#endif
        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
    }

	if(dReqRespType==SipMessageRequest)
		pTransaction=pObject->pUacTransaction;
	else
		pTransaction=pObject->pUasTransaction;

#ifdef SDF_LOOKUP
	/* Update the DNS state */
	if(pOverlapTransInfo==Sdf_co_null)
	{
		if (pTransaction == Sdf_co_null)
		{
            /* CSR_1-8290042 changes start */
            sip_freeSipMessage(pClonedMsg);
            /* CSR_1-8290042 changes end */
			return Sdf_co_fail;
		}	
      
        /*Fix for CSR 1-8492091*/
        if(((dReqRespType==SipMessageRequest) && (dMethodType != Sdf_en_cancel))\
                || (dReqRespType==SipMessageResponse) )
        { 
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0,  &pTransaction->dTxnDnsStateMutex, 0);  	
#endif
		if (!(pTransaction->dTxnDnsState ==Sdf_en_dnsIpUsed || 
			pTransaction->dTxnDnsState == Sdf_en_dnsFirstRecordFilled ||
			pTransaction->dTxnDnsState == Sdf_en_dnsFilled))
		{
			pError->errCode = Sdf_en_dnsInvState;
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif
            /* CSR_1-8290042 changes start */
            sip_freeSipMessage(pClonedMsg);
            /* CSR_1-8290042 changes end */
			return Sdf_co_fail;
		}
		pTransaction->dTxnDnsState = Sdf_en_dnsIpUsed;	
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif
	}	
	}	
	else
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0,  &pOverlapTransInfo->dTxnDnsStateMutex, 0);  	
#endif
		if (!(pOverlapTransInfo->dTxnDnsState == Sdf_en_dnsIpUsed || 
			pOverlapTransInfo->dTxnDnsState == Sdf_en_dnsFirstRecordFilled ||
			pOverlapTransInfo->dTxnDnsState == Sdf_en_dnsFilled))
		{
			pError->errCode = Sdf_en_dnsInvState;
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0,  &pOverlapTransInfo->dTxnDnsStateMutex);	
#endif
           /* CSR_1-8290042 changes start */
            sip_freeSipMessage(pClonedMsg);
           /* CSR_1-8290042 changes end */
			return Sdf_co_fail;
		}
		pOverlapTransInfo->dTxnDnsState = Sdf_en_dnsIpUsed;	
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,  &pOverlapTransInfo->dTxnDnsStateMutex);	
#endif
	}
#else
	/*
	 * Update the pDestInfo structure of the call object with the details
	 * from the current SipMessage
	 */
	if (sdf_ivk_uaUpdateDestinationInfo(pObject, pOverlapTransInfo,
		pSipMsg, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
			"Failed to update destination info in call object",pError);
#endif
		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
	}
#endif /* End of SDF_LOOKUP */

#ifndef SDF_UASSF
	/* 
	 * If it is to be sent on a TCP channel, initiate a new 
	 * connection if an active socket is not available
	 */
	if(sdf_fn_uaEstablishChannel(pObject, pOverlapTransInfo, dReqRespType, \
			dMethodType, pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
			"Failed to Establish Channel",pError);
#endif
		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
	}

	/*
	 * Start filling the SipTranspAddr structure.
	 * Socket is filled below after setting dSdpInInvite
	 * appropriately
	 */
	if (sdf_ivk_uaGetDestInfoFromTransaction(pTransaction, pOverlapTransInfo,\
			&pDestInfo, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_callObjectAccessError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
			"Error getting DestInfo from Transaction Object",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
        (void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
	}
	/*CSR 1-6792719 CSR Change Start*/	
	/*pTranspaddr = (SipTranspAddr *) sdf_memget(Sdf_mc_callHandlerMemId, \
		sizeof(SipTranspAddr), pError);*/
        if (sip_initSipTranspAddr(&pTranspaddr,(SipError*)&(pError->stkErrCode \
                      ))== SipFail)
        {
       	    return SipFail;
	}	
	/*CSR 1-6792719 CSR Change End */

#ifdef SDF_RPORT
  pTranspaddr->dReceiversPort = 0;
  /*CSR 1-6792719 CSR Change Start*/	
  /*sdf_mc_init(pTranspaddr->pReceiversIp)*/;
  /*CSR 1-6792719 CSR Change End */
#endif /* SDF_RPORT */
  
	pTranspaddr->pHost = Sdf_mc_strdupCallHandler(pDestInfo->pIp);
	pTranspaddr->dPort = pDestInfo->dPort;

	/*
	 * Set the socket file descriptor for the call in pTranspaddr
	 */
	if(pOverlapTransInfo==Sdf_co_null)
		pSocket = &pTransaction->dSocket;
	else
		pSocket = &pOverlapTransInfo->dSocket;

	pTranspaddr->dSockFd = pSocket->dSockfd;

	if (sdf_ivk_uaInitEventContext(&pEventContext, pError) \
			== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ):"
			"Failed to allocate memory for EventContext structure",pError);
#endif
		pError->errCode=Sdf_en_noMemoryError;
		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
	        /*CSR 1-6792719 CSR Change Start */
        	(void)sip_freeSipTranspAddr(pTranspaddr);
		/* (void)sdf_memfree(Sdf_mc_appMemId, (Sdf_ty_pvoid *)&pTranspaddr->pHost, \
			&dLocalErr);
		(void)sdf_memfree(Sdf_mc_appMemId, (Sdf_ty_pvoid *)&pTranspaddr, &dLocalErr); */
	        /*CSR 1-6792719 CSR Change End */
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
	}
	if (pContext == Sdf_co_null)
	{
		/* As application invoked this API with NULL EventContext,
		 * application don't want to keep any pData. So set pData to null */

		pEventContext->pData = Sdf_co_null;
		pEventContext->dRemainingLength = 0;
	}
	else
	{
#ifdef SDF_RPORT
	  if(dReqRespType == SipMessageResponse) 
	  {
      if (pContext->pTranspAddr != Sdf_co_null)  
      {
	  	  pTranspaddr->dReceiversPort = pContext->pTranspAddr->dReceiversPort;
		    pTranspaddr->pReceiversIp = pContext->pTranspAddr->pReceiversIp;
      }
	  }
#endif 
		/*Clone the values from the existing pContext to the new one  */
		/*w534*/ (void)sdf_ivk_uaCloneEventContext(pEventContext, pContext, pError);
	}

#ifdef SDF_USE_PREALLOCBUFFERS
	/*
	 * Get a preallocated buffer for use by sip_sendMessage and note the
	 * index in event context
	 */
#ifndef SDF_APPCONTROLLEDMEMPOOL
	pEventContext->pDirectBuffer = (Sdf_ty_s8bit*)sdf_ivk_uaMempoolAllocBlk(\
		&dGlbPreallocMsgBufferPool, &pEventContext->dPreallocMsgBufIndex);
#else
	{
		Sdf_st_mempool *pPreallocMsgBuffer;
		(void)sdf_cbk_uaGetDirectBufMemPool( &pPreallocMsgBuffer, pEventContext, \
				pError);

		pEventContext->pDirectBuffer=(Sdf_ty_s8bit*)sdf_ivk_uaMempoolAllocBlk(\
			pPreallocMsgBuffer, &pEventContext->dPreallocMsgBufIndex);
	}
#endif

	if (pEventContext->pDirectBuffer == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "\
			"Could not allocate a buffer for passing to sip_sendMessage()",\
			pError);
#endif
		pError->errCode=Sdf_en_noMemoryError;
		(void)sdf_ivk_uaFreeEventContext(pEventContext);

		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
	        /*CSR 1-6792719 CSR Change Start */
        	(void)sip_freeSipTranspAddr(pTranspaddr);
		/*(void)sdf_memfree(Sdf_mc_appMemId, \
					(Sdf_ty_pvoid *)&pTranspaddr->pHost, pError);
		(void)sdf_memfree(Sdf_mc_appMemId, (Sdf_ty_pvoid *)&pTranspaddr, pError);*/
	        /*CSR 1-6792719 CSR Change End */
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail ;
	}

	/* 
	 * Form the buffer and Set the options flag to cause sip_sendMessage to
	 * use this preallocated buffer instead of allocating a new buffer on
	 * its own
	 */
	if(sip_formMessage( pSipMsg, &dOptions, pEventContext->pDirectBuffer, \
		&dLength, (SipError*)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sendNetworkError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
			"Failed to form the SIP message buffer.", pError);
#endif
		pError->errCode = Sdf_en_sendNetworkError;
		(void)sdf_ivk_uaFreeEventContext(pEventContext);

		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
	        /*CSR 1-6792719 CSR Change Start*/
       	 	(void)sip_freeSipTranspAddr(pTranspaddr);
		/*(void)sdf_memfree(Sdf_mc_appMemId, \
					(Sdf_ty_pvoid *)&pTranspaddr->pHost, pError);
		(void)sdf_memfree(Sdf_mc_appMemId, (Sdf_ty_pvoid *)&pTranspaddr, pError);*/ 
	        /*CSR 1-6792719 CSR Change End*/

        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */

		return Sdf_co_fail ;
	}

	dOptions.dOption |= SIP_OPT_DIRECTBUFFER; 
	pEventContext->dOptions.dOption |= SIP_OPT_DIRECTBUFFER;
	pEventContext->dBuffLength = dLength;

	/* 
	 * NON-TXN LAYER HANDLING OF EVENT-CONTEXT
	 * ---------------------------------------
	 * pEventContext will get freed inside sip_sendMessage but we still
	 * need to store a reference to the event context for remote
	 * retransmission. So we need to increment the reference count here and
	 * release it after adding the remote retransmission entry
	 *
	 *
	 * TXN LAYER HANDLING OF EVENT-CONTEXT
	 * -----------------------------------
	 * The event context is stored in the stack alone as a part of the
	 * Txn information. We do not store this event context in any UA
	 * Data structure. So, we need not increment the refCount
	 * of the event context here at all, since it becomes one again 
	 * by the time we quit from this API. 
	 *
	 */
#ifndef SDF_TXN
	HSS_LOCKEDINCREF(pEventContext->dRefCount);
#endif

#endif
	/* 
	 * Checking the protocol using which the message is to be sent. 
	 * Currently only UDP/TCP/TLS
	 */
	if (pDestInfo->dScheme == Sdf_en_protoTcp)
		transptype = transptype | SIP_TCP;
#ifdef SDF_TLS
	else if(pDestInfo->dScheme == Sdf_en_protoTls)
		transptype = transptype | SIP_TLS;
#endif
	else
		transptype = transptype | SIP_UDP;

    /* CSR_1-8290042 changes start */
	if( Sdf_co_success != sdf_fn_uaSendSipMsgOverNetwork(pObject, \
			pTransaction, pOverlapTransInfo, pClonedMsg, dReqRespType, \
			pEventContext, &dOptions, pTranspaddr, transptype, pError))
    /*  CSR_1-8290042 changes end */
	{		
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
			"Unable to Send message over network", pError);
#endif
		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);

#if defined SDF_USE_PREALLOCBUFFERS && !defined(SDF_TXN)
		(void)sdf_ivk_uaFreeEventContext(pEventContext);
#endif

	/* CSR 1-6792719 Change Start */        
/*#ifndef SDF_TXN
		(void)sdf_memfree(Sdf_mc_appMemId, (Sdf_ty_pvoid *)&pTranspaddr->pHost,\
			&dLocalErr);
		(void)sdf_memfree(Sdf_mc_appMemId, (Sdf_ty_pvoid *)&pTranspaddr, &dLocalErr);
#endif*/
	   	(void)sip_freeSipTranspAddr(pTranspaddr);
	/* CSR 1-6792719 Change End */        
		(void)sdf_ivk_uaFreeEventContext(pEventContext);
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
        /* CSR_1-8290042 changes start */
        sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
	}
	/* CSR 1-6792719 Change Start */
   	(void)sip_freeSipTranspAddr(pTranspaddr);
	
/*#ifndef SDF_TXN
	(void)sdf_memfree(Sdf_mc_appMemId, (Sdf_ty_pvoid *)&pTranspaddr->pHost, pError);
	(void)sdf_memfree(Sdf_mc_appMemId, (Sdf_ty_pvoid *)&pTranspaddr, pError);

#endif*/
	/* CSR 1-6792719 Change End */        
#if 0
	/* CERT:This free statement should be present if SipTxnByPass is being
	 *  passed as argument to sip_txn_sendMessage in sdf_internal.c */
	sdf_memfree(Sdf_mc_appMemId, (Sdf_ty_pvoid *)&pTranspaddr, pError);
#endif

	/*
	 * Populate the remote retransmission list with the new message sent out.
	 * This is done only in the case of ACK messages and >2XX responses in case
	 * of UDP and only ACK messages in case of TCP
	 */

#ifndef SDF_TXN

	if(sdf_fn_uaPopulateRREntry( pObject, pSipMsg, pDestInfo, pSocket->dSockfd,\
			dOptions, pEventContext, dMessageInfo, pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
			"Error in Updating the Remote Retransmission Entry"
			"retransmision list",pError);
#endif
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
        /* CSR_1-8290042 changes start */
         sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
	}
#endif

	/* Free the local reference here. */
	(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);

#if defined SDF_USE_PREALLOCBUFFERS && !defined(SDF_TXN)
	/* 
	 * The reference to pEventContext is no longer needed here as it has been
	 * added to the remote retransmission entry
	 */
	(void)sdf_ivk_uaFreeEventContext(pEventContext);
#endif

	/*
     * Message sent successfully. Now call this API to update the call state
	 * and also to update the Session Timer. 
	 */
	if (sdf_fn_uaMessageSentNtfn(pObject, pOverlapTransInfo,\
				pSipMsg, dMessageInfo, pContext, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
			"Error in Updating the call state.", pError);
#endif
		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
        /* CSR_1-8290042 changes start */
         sip_freeSipMessage(pClonedMsg);
        /*  CSR_1-8290042 changes end */
		return Sdf_co_fail;
    }
#endif
    if(pOverlapTransInfo!=Sdf_co_null)
    {
    	if((pOverlapTransInfo->dMsgType== Sdf_en_prack)&& \
		   (pOverlapTransInfo->dSocket.dSockfd== \
		    pObject->pUasTransaction->dSocket.dSockfd))	
		{	
     		pOverlapTransInfo->dSocket.dProtocol = Sdf_en_protoNone;
	    	pOverlapTransInfo->dSocket.dSockfd= Sdf_co_InvalidSocketFd;
		}
	}
	(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSendCallToPeer\n");
	pError->errCode = Sdf_en_noUaError;

#ifdef SDF_LINT
  (void)pTransaction;
#endif

    /* CSR_1-8290042 changes start */
    sip_freeSipMessage(pClonedMsg);
    /* CSR_1-8290042 changes end */

	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaSendMessage
 **
 ** DESCRIPTION: Forms the target address spec and changes the call state
 **              assuming that the message will be successfully sent to the
 **              network. This API will not try to resolve the target host.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSendMessage
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	SipMessage   *pMessage, 
	Sdf_st_destDetails  **ppDestDetails,
	Sdf_st_error *pError)
#else
	(pObject,pMessage,ppDestDetails,pError)
	Sdf_st_callObject *pObject;
	SipMessage   *pMessage;
	Sdf_st_destDetails  **ppDestDetails;
	Sdf_st_error *pError;
#endif
{
	Sdf_st_overlapTransInfo  *pOverlapTransInfo = Sdf_co_null;
	Sdf_st_msgInfo	dMessageInfo;
	/*Sdf_ty_bool			     dSdpInInvite;*/

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSendMessage\n");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaSendMessage( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}	
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendMessage( ): "
			" Call object parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendMessage( ): "
			" SipMessage parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (ppDestDetails == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendMessage( ): "
			" ppDestDetails parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif  /* End of Param Validation. */

	/*w534*/ (void)sdf_fn_uaGetMessageInfo( pMessage, &dMessageInfo, pError);


	if (sdf_fn_uaDoSendMessageProcs(pObject, pMessage, &pOverlapTransInfo,\
			dMessageInfo, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendMessage( ): "
			"sdf_fn_uaDoSendMessageProcs Failed ",pError);
#endif
		return Sdf_co_fail;
    }

	/*dSdpInInvite = pObject->pCommonInfo->dSdpInInvite;*/

	if (sdf_ivk_uaGetDestinationDetails(pObject, pOverlapTransInfo, pMessage,\
			   ppDestDetails, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
	    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
		    (Sdf_ty_s8bit *)"sdf_ivk_uaSendMessage(): "
		    "Failed to get the target address spec.", pError);
#endif
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(pObject->dCallStateMutex));
#endif
	    (void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
	    return Sdf_co_fail;
    }

	/*
     * Assuming that the message will be sent successfully, call this API to 
	 * update the call state and also to update the Session Timer. 
	 */
	if (sdf_fn_uaMessageSentNtfn(pObject, pOverlapTransInfo,\
				pMessage, dMessageInfo, Sdf_co_null, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSendMessage( ): "
			"Error in Updating the call state.", pError);
#endif
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(pObject->dCallStateMutex));
#endif
		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
		(void)sdf_ivk_uaFreeDestDetails(*ppDestDetails);
		return Sdf_co_fail;
	}

	/* Free the local reference here. */
	(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSendMessage\n");
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

