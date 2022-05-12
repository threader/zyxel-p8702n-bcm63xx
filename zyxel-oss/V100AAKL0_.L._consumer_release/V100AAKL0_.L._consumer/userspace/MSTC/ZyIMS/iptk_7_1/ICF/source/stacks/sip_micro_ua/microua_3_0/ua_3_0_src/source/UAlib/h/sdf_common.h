/******************************************************************************
 ** FUNCTION:
 **	 	This file has common definitions used by all modules of the toolkit
 ******************************************************************************
 **
 ** FILENAME		: sdf_common.h
 **
 ** DESCRIPTION		: This file has common definitions used by all modules 
 **						of the toolkit
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 24/10/00	Anitha						Creation
 ** 25-Jan-10   Pavneet        SPR 20278    Size of Sdf_co_characterBufferSize
 **                                         has been increased to 128.     
 **
 ******************************************************************************
 **				COPYRIGHT , Aricent, 2006
 ******************************************************************************/


#ifndef __SDF_COMMON_H__
#define __SDF_COMMON_H__
#include "microsip_common.h"
#include "microsip_portlayer.h"
#include "microsip_struct.h"
#include "microsip_list.h"
#include "sdf_basictypes.h"

	
#ifdef ANSI_PROTO
#define _ARGS_(x)      x
#define _CONST_        const
#else
#define _ARGS_(x)      ()
#define _CONST_
#endif

 



#define SDF_PART_ID "1-000-5-0219-0111-14-0400-000"
#define Sdf_co_viaMagicCookie		"z9hG4bK"

 /* Allocate sufficient memory to hold "SIP/2.0/TCP" or "SIP/2.0/UDP" 
  * or "SIP/2.0/SCTP".  So the maximum langth required is around 13 */
#define Sdf_co_ViaSentByLength		13

/*
 * The number of methods (Eg. INVITE, BYE, PRACK, etc)
 * supported by the UA Toolkit
 */
#ifdef SDF_REFER
#define	Sdf_co_noOfReferMethods		1	
#else
#define	Sdf_co_noOfReferMethods		0	
#endif

#ifdef SDF_IM
#define	Sdf_co_noOfImMethods		1	
#else
#define	Sdf_co_noOfImMethods		0	
#endif

#ifdef SIP_IMPP
#define	Sdf_co_noOfImPPMethods		2	
#else
#define	Sdf_co_noOfImPPMethods		0	
#endif	


#define Sdf_co_noOfMethodsSupported 	(10 + Sdf_co_noOfReferMethods + Sdf_co_noOfImMethods + Sdf_co_noOfImPPMethods )


/*
 * The size of the table mapping payload types to the
 * corresponding AVT-profile codecs
 */
#ifdef SDF_UAB2B 
#define Sdf_co_codecTableSize			(35)
#else
#define Sdf_co_codecTableSize			(19)
#endif
/*
 * The size of the table mapping SIP status codes to
 * reason phrases.
 */
#define Sdf_co_scTableSize				(58)


/*
 * Max buffer size
 */
#define Sdf_mc_maxMsgSize				 4000 

/* 
 * Default port for SIP URIs.
 */
#define Sdf_mc_defaultSipPort		5060


#ifdef SDF_TLS

/* 
 * Default port for SIPS URIs.
 */
#define Sdf_mc_defaultSipSPort		5061

/* 
 * Default port for SIPS URIs.
 */
#define Sdf_mc_defaultCipherList	"TLSv1"

/* 
 * Max total length of the string containing the concatenated list of ciphers.
 */
#define Sdf_co_maxCipherListLength  1024

/*
 * Max length of a TLS/SSL cipher-suite name.
 */
#define Sdf_co_maxCipher_suite_NameLength  128

/*
 * Max number of ciphers that can be configured.
 */
#define Sdf_co_maxNumOfCiphers      40

#endif /* SDF_TLS */


/*
 * The expiry time of entries in the Remote Retransmission list. Each
 * entry is kept for Sdf_mc_remoteRetransExpiryTime seconds and purged
 * thereafter.
 */
#define Sdf_mc_remoteRetransExpiryTime		32

/*
 * The spool time for purging expired entries in the Remote Retransmission
 * list. If the UA toolkit is compiled with the SDF_USE_INTERNALTHREADS flag,
 * the remote retransmission manager wakes up every
 * Sdf_mc_remoteRetransSpoolTime seconds and spools the list for expired 
 * entries. If it is not compiled with the SDF_USE_INTERNALTHREADS flag, then
 * the sdf_cbk_uaStartTimer callback is invoked every
 * Sdf_mc_remoteRetransSpoolTime seconds.
 */
#define Sdf_mc_remoteRetransSpoolTime		10

#ifndef SDF_TXN
	
/*
 * The max buckets of the hash table storing Remote Retransmission elements
 */
#define Sdf_mc_remoteRetransHashMaxBuckets	5021

/*
 * The max elements that can be stored in the hash table of Remote
 * Retransmission elements
 */
#define Sdf_mc_remoteRetransHashMaxElements	1000

#endif

#ifdef SDF_TXN
	
/*
 * The duration for which the call will live after reception of a
 * provisional response. If within this duration, a final response 
 * is not received, then the application will be informed of a 
 * timeout in a callback. 
 *
 * This duration is in milli-seconds.
 */
#define Sdf_co_noAnswerTimerDuration	180000

/*
 * The max buckets of the hash table storing Remote Retransmission elements
 */
#define Sdf_co_numTxnHashBuckets		500

/*
 * The max elements that can be stored in the hash table of Remote
 * Retransmission elements
 */
#define Sdf_co_numTxnHashMaxElements	1500
	
#endif
	
/* The max value for initial CSeq. This is used to cross check the 
 * initial CSeq got on a Fresh request*/
#define Sdf_mc_maxCseqValue	2147483647
	
#ifdef SDF_SUBSCRIPTION_LAYER
/*
 * The max buckets of the hash table storing Subscription Objects 
 */
#define Sdf_co_numSLHashBuckets       500

/*
 * The max elements that can be stored in the hash table of Subscription
 * Objects
 */
#define Sdf_co_numSLHashMaxElements   5000

#endif
	
#ifdef SDF_LOOKUP
#define NUMERIC_ONLY 1
#define ALPHANUMERIC_ONLY 2
#endif	
#ifdef SDF_DEP
#define MAX_DEP_OBJECTS 50
#define NUM_HASH_BUCKETS 100
#endif
/*
 * Timer ticks per second on the system clock;
 * needs to be atleast 100 for the timer logic to work properly
 */
 /*
  * Sizes of static arrays
 */ 	
/*Change for Symbian compilation Start*/
#ifndef ICF_PORT_SYMBIAN
#define	 Sdf_co_traceBufferSize			300
/*CSR 8196703 SPR 20278- Size of Sdf_co_characterBufferSize 
should be equal to ICF_MAX_STR_LEN defined in icf_defs.h*/
#define	 Sdf_co_characterBufferSize		128
#define	 Sdf_co_smallBufferSize			400
#define	 Sdf_co_mediumBufferSize		300
#define	 Sdf_co_largeBufferSize 		500
#else
#define	 Sdf_co_smallBufferSize			100
#define	 Sdf_co_traceBufferSize			Sdf_co_smallBufferSize
#define	 Sdf_co_characterBufferSize		 50
#define	 Sdf_co_mediumBufferSize		Sdf_co_smallBufferSize
#define	 Sdf_co_largeBufferSize 		500
#endif
/*Change for Symbian compilation End*/
	

typedef		SIP_RefCount				Sdf_ty_refCount;

typedef		Sdf_ty_u32bit				Sdf_ty_txnIdT;

typedef enum 
{
#define 	Sdf_error_base		SipErrorBase
	/* 
	 * SIP Core Stack Errors 
	 */
	Sdf_en_invalidHeaderError  			=  (Sdf_error_base +0),
	Sdf_en_invalidIndexError			=  (Sdf_error_base +1),
	Sdf_en_noMemoryError				=  (Sdf_error_base +2),  
	Sdf_en_invalidTypeError				=  (Sdf_error_base +3),      
	Sdf_en_stkInvalidTraceLevel			=  (Sdf_error_base +4),
	Sdf_en_stkInvalidTraceType    		=  (Sdf_error_base +5),
	Sdf_en_stkInvalidErrorLevel   		=  (Sdf_error_base +6),
	Sdf_en_stkInvalidStatsType   		=  (Sdf_error_base +7),
	Sdf_en_stkTraceDisabled  			=  (Sdf_error_base +8), 
	Sdf_en_stkErrorDisabled    			=  (Sdf_error_base +9),
	Sdf_en_stkStatsDisabled   	 		=  (Sdf_error_base +10),
	Sdf_en_duplicateEntry 				=  (Sdf_error_base +11),
	Sdf_en_noExistError    				=  (Sdf_error_base +12),
	Sdf_en_systemError        			=  (Sdf_error_base +13),
	Sdf_en_memoryError     				=  (Sdf_error_base +14),
	Sdf_en_invalidParamError			=  (Sdf_error_base +15),
	Sdf_en_noUaError       	  			=  (Sdf_error_base +16),
	Sdf_en_parserError					=  (Sdf_error_base +17),
	Sdf_en_timerDuplicateError			=  (Sdf_error_base +18),
	Sdf_en_timerFull					=  (Sdf_error_base +19),
	Sdf_en_timerDoesNotExist			=  (Sdf_error_base +20),
	Sdf_en_maybeIncomplete				=  (Sdf_error_base +21),
	Sdf_en_incomplete					=  (Sdf_error_base +22),
    /* Enhancement Change Start */
    Sdf_en_sipIncomplete                =  (Sdf_error_base +23),
    /* Enhancement Change End */
    Sdf_en_notImplemented               =  (Sdf_error_base +24),
    Sdf_en_sendFailed                   =  (Sdf_error_base +25),
    Sdf_en_timeOut                      =  (Sdf_error_base +26),
    Sdf_en_hashFail                     =  (Sdf_error_base +27),
    Sdf_en_recvFail                     =  (Sdf_error_base +28),
    Sdf_en_invRange                     =  (Sdf_error_base +29),
    Sdf_en_txnNoExist                   =  (Sdf_error_base +30),
    Sdf_en_txnExists                    =  (Sdf_error_base +31),
    Sdf_en_txnInvState                  =  (Sdf_error_base +32),
    Sdf_en_txnInvMessage                =  (Sdf_error_base +33),
    Sdf_en_bufOverflow                  =  (Sdf_error_base +34),
    Sdf_en_NetworkSentFailure           =  (Sdf_error_base +35),
	Sdf_en_LastCoreStackErrors			=  E_LAST_ERROR,
     


	/* 
	 * Errors for possible incomplete messages (core stack)
	 */
	Sdf_en_versionNotSupported			=  (Sdf_en_LastCoreStackErrors+1),
	Sdf_en_LastIncompMsgErr				=  (Sdf_en_LastCoreStackErrors+2),

	/* 
	 * Network Errors 
	 */
	Sdf_en_socketBindError				=  (Sdf_en_LastIncompMsgErr+1),
	Sdf_en_socketOpenError				=  (Sdf_en_LastIncompMsgErr+2),
	Sdf_en_tcpConnectError				=  (Sdf_en_LastIncompMsgErr+3),
	Sdf_en_destinationInfoError			=  (Sdf_en_LastIncompMsgErr+4),
	Sdf_en_LastNetworkError				=  (Sdf_en_LastIncompMsgErr+4),

	/* 
	 * Configuration data related Errors 
	 */
	Sdf_en_configDataError				=  (Sdf_en_LastNetworkError+1),

	/* 
	 * UA Toolkit structure access related errors
	 */
	Sdf_en_callObjectAccessError 		= 	(Sdf_en_LastNetworkError+2),
	Sdf_en_transpInfoAccessError 		= 	(Sdf_en_LastNetworkError+3),
	Sdf_en_callInfoAccessError 			= 	(Sdf_en_LastNetworkError+4),
	Sdf_en_transactionAccessError		= 	(Sdf_en_LastNetworkError+5),
	Sdf_en_nameInfoAccessError			=	(Sdf_en_LastNetworkError+6),
	Sdf_en_commonInfoAccessError 		=	(Sdf_en_LastNetworkError+7),
	Sdf_en_initDataAccessError	 		=	(Sdf_en_LastNetworkError+8),
	Sdf_en_appDataAccessError			=	(Sdf_en_LastNetworkError+9),
	Sdf_en_overlapTransInfoAccessError	=	(Sdf_en_LastNetworkError+10),
	Sdf_en_headerListAccessError		=	(Sdf_en_LastNetworkError+11),
	Sdf_en_mediaHandlingAccessError		=	(Sdf_en_LastNetworkError+12),
	Sdf_en_sessionParamAccessError		=	(Sdf_en_LastNetworkError+13),
	Sdf_en_noCallExists					=	(Sdf_en_LastNetworkError+14),
	Sdf_en_LastUATKAccessError			=	(Sdf_en_LastNetworkError+14),
	
	/* 
	 * Miscellaneous errors 
	 */
	Sdf_en_noMatchingMediaError			=	(Sdf_en_LastUATKAccessError+1),
	Sdf_en_threadCreationError			=	(Sdf_en_LastUATKAccessError+2),
	Sdf_en_headerManipulationError		=	(Sdf_en_LastUATKAccessError+3),
	Sdf_en_userNotFoundError			=	(Sdf_en_LastUATKAccessError+4),
	Sdf_en_sipstackInitError			=	(Sdf_en_LastUATKAccessError+5),
	Sdf_en_unknownMethodError			=	(Sdf_en_LastUATKAccessError+6),
	Sdf_en_badExtensionsError			=	(Sdf_en_LastUATKAccessError+7),
	Sdf_en_extraHeaderAccessError		=	(Sdf_en_LastUATKAccessError+8),
	Sdf_en_retransKeyError				=	(Sdf_en_LastUATKAccessError+9),
	Sdf_en_attributeConflictError		=	(Sdf_en_LastUATKAccessError+10),
	Sdf_en_tagMismatchError				=	(Sdf_en_LastUATKAccessError+11),
	Sdf_en_forkedRespError				=	(Sdf_en_LastUATKAccessError+12),
	Sdf_en_featureNotImplemented		=	(Sdf_en_LastUATKAccessError+13),
	Sdf_en_contactSchemeMisMatch		=	(Sdf_en_LastUATKAccessError+14),
	Sdf_en_mandatoryHdrMissing      = 	(Sdf_en_LastUATKAccessError+15),
	Sdf_en_InvalidCseqNumber				= (Sdf_en_LastUATKAccessError+16),
	Sdf_en_requestPendingError			=	(Sdf_en_LastUATKAccessError+17),
	Sdf_en_localTagMismatchError		=	(Sdf_en_LastUATKAccessError+18),
	Sdf_en_serverInternalError			=	(Sdf_en_LastUATKAccessError+19),
  Sdf_en_lowSessionTimerIntervalError = (Sdf_en_LastUATKAccessError+20),
   	Sdf_en_cancelTxnMismatch			=	(Sdf_en_LastUATKAccessError+21),
	Sdf_en_LastMiscError			    =   (Sdf_en_LastUATKAccessError+22),


	/* 
	 * Statistics, Trace and Error related errors
	 */
	Sdf_en_invalidStatsType				=	(Sdf_en_LastMiscError+1),
	Sdf_en_invalidErrorLevel			=	(Sdf_en_LastMiscError+2),
	Sdf_en_invalidTraceType				=	(Sdf_en_LastMiscError+3),
	Sdf_en_invalidTraceLevel			=	(Sdf_en_LastMiscError+4),
	Sdf_en_TraceDisabled				=	(Sdf_en_LastMiscError+5),
	Sdf_en_LastStatError				=	(Sdf_en_LastMiscError+5),

	/* 
	 * Call state and Callback related errors 
	 */
	Sdf_en_callStateError  				= 	(Sdf_en_LastStatError+1),
	Sdf_en_callBackError				=   (Sdf_en_LastStatError+2),
	Sdf_en_LastCallStateError			=   (Sdf_en_LastStatError+2),

	/* 
	 * Request/Response/Messagebody errors 
	 */
	Sdf_en_formRequestError				=   (Sdf_en_LastCallStateError+1),
	Sdf_en_formResponseError			=   (Sdf_en_LastCallStateError+2),
	Sdf_en_sdpError						=   (Sdf_en_LastCallStateError+3),
	Sdf_en_msgBodyError					=	(Sdf_en_LastCallStateError+4),	
	Sdf_en_messageBodyNotAllowed		=	(Sdf_en_LastCallStateError+5),	
	Sdf_en_schemeNotAllowed				=	(Sdf_en_LastCallStateError+6),	
	Sdf_en_tagGenerationError			=	(Sdf_en_LastCallStateError+7),	
	Sdf_en_regularTxnPendingError		=	(Sdf_en_LastCallStateError+8),	
	Sdf_en_LastReqRespError				=	(Sdf_en_LastCallStateError+9),	
	
	/* 
	 * Send error 
	 */
	Sdf_en_retransListError				= 	(Sdf_en_LastReqRespError+1),
	Sdf_en_sendNetworkError   			=	(Sdf_en_LastReqRespError+2),

	/* 
	 * Authentication error 
	 */
	Sdf_en_authError   					=	(Sdf_en_LastReqRespError+3),

	/*
	 * Errors due to erroneous headers in SipMessage
	 */
	Sdf_en_protocolMismatchInVia		=	(Sdf_en_LastReqRespError+4),
	Sdf_en_addressMismatchInVia			=	(Sdf_en_LastReqRespError+5),
	/* CSeq Error*/
	Sdf_en_cseqValueExceeded			=	(Sdf_en_LastReqRespError+6),
	Sdf_en_invalidLocalCseq         = (Sdf_en_LastReqRespError+7),
	Sdf_en_invalidRemoteCseq        = (Sdf_en_LastReqRespError+8),

	/* Start transaction failed */
	Sdf_en_transactionFailure			=	(Sdf_en_LastReqRespError+9),

    /* Basic scheme not implemented */	
    Sdf_en_basicSchemeNotImplemented    =	(Sdf_en_LastReqRespError+10),
	Sdf_en_LastGenError					=	(Sdf_en_LastReqRespError+11),


#ifdef SDF_SESSION_TIMER
	/* Invalid Refresher */
	Sdf_en_invalidRefesher 				=	(Sdf_en_LastGenError+1),
	/* Invalid Session Interval */	
	Sdf_en_invalidSessionInterval		=	(Sdf_en_LastGenError+2),
	/* Invalid MinSE */
	Sdf_en_invalidMinSe					=	(Sdf_en_LastGenError+3),
	/* Session timer error */
	Sdf_en_sessionTimerError			=	(Sdf_en_LastGenError+4),
	/* Session Interval too low */
	Sdf_en_lowsessionTimerError			=	(Sdf_en_LastGenError+5),

	Sdf_en_LastSessionTimerError		=	(Sdf_en_LastGenError+5),
#else
	Sdf_en_LastSessionTimerError		=	(Sdf_en_LastGenError),
#endif

#ifdef SDF_HA
	/* Serialization/Deserialization and other HA related Errors */
	Sdf_en_serializeInvalidFieldId		= 	(Sdf_en_LastSessionTimerError+1),
	Sdf_en_serializeExtraArgsReqd		= 	(Sdf_en_LastSessionTimerError+2),
	Sdf_en_invalidObjectType			= 	(Sdf_en_LastSessionTimerError+3),
	Sdf_en_stackSerializationApiFailure =	(Sdf_en_LastSessionTimerError+4),
	Sdf_en_stackDeserializationApiFailure =	(Sdf_en_LastSessionTimerError+5),
	Sdf_en_objectNotInitialzedProperly	=	(Sdf_en_LastSessionTimerError+6),
	Sdf_en_initSerializationModuleFailed = 	(Sdf_en_LastSessionTimerError+7),
	Sdf_en_cannotReconstructInThisState	=	(Sdf_en_LastSessionTimerError+8),
	Sdf_en_dataMissing					=	(Sdf_en_LastSessionTimerError+9),

	Sdf_en_LastHaRelatedError			=	(Sdf_en_LastSessionTimerError+9),
#else
	Sdf_en_LastHaRelatedError			=	(Sdf_en_LastSessionTimerError),
#endif
	/* 
	 * Sdp error 
	 */
	Sdf_en_malformedQosAttribute		= 	(Sdf_en_LastHaRelatedError+1),
	Sdf_en_noMatchingMid				= 	(Sdf_en_LastHaRelatedError+2),
	Sdf_en_missingOriginLineError		= 	(Sdf_en_LastHaRelatedError+3),
	Sdf_en_missingConnectionLineError	= 	(Sdf_en_LastHaRelatedError+4),
	Sdf_en_noMediaLineInAnswer          =   (Sdf_en_LastHaRelatedError+5), 
	Sdf_en_mediaGroupError              =   (Sdf_en_LastHaRelatedError+6),
	Sdf_en_mediaTypeMisMatch            =   (Sdf_en_LastHaRelatedError+7),
	Sdf_en_transportProtocolMisMatch    =   (Sdf_en_LastHaRelatedError+8),
	Sdf_en_codecMismatch                =   (Sdf_en_LastHaRelatedError+9),
	Sdf_en_noSdpError                   =   (Sdf_en_LastHaRelatedError+10),
	Sdf_en_mediaStateTransitionError    =   (Sdf_en_LastHaRelatedError+11),
	Sdf_en_sdpAccessorError				=	(Sdf_en_LastHaRelatedError+12),
	Sdf_en_LastSdpRelatedError			=   (Sdf_en_sdpAccessorError),


	/*Extra Errors*/
	Sdf_en_tooManyViaHdrsInResponse		=	(Sdf_en_LastSdpRelatedError+1),
	Sdf_en_basicHeaderError				=	(Sdf_en_LastSdpRelatedError+2),
	Sdf_en_nonMandatoryHeaderError		=	(Sdf_en_LastSdpRelatedError+3),
	Sdf_en_sdpLineError					=	(Sdf_en_LastSdpRelatedError+4),
	Sdf_en_LastExtraError				=	Sdf_en_sdpLineError,
	

	/*
	 * Service layer related errors
	 */
#ifdef SDF_SERVICE
	Sdf_en_noAssocCallObj				=   (Sdf_en_LastExtraError+1),
	Sdf_en_slDeleteEntryError			=   (Sdf_en_LastExtraError+2),
	Sdf_en_slAddEntryError				=   (Sdf_en_LastExtraError+3),
	Sdf_en_slNoEntryError				=   (Sdf_en_LastExtraError+4),
	Sdf_en_serviceObjectFetchError		=   (Sdf_en_LastExtraError+5),
	Sdf_en_serviceObjectReleaseError	=   (Sdf_en_LastExtraError+6),
	Sdf_en_internalTransferError		=   (Sdf_en_LastExtraError+7),
	Sdf_en_invalidTransferStateError    =   (Sdf_en_LastExtraError+8),
	Sdf_en_LastServiceError				=   (Sdf_en_invalidTransferStateError),
#else
	Sdf_en_LastServiceError				=   (Sdf_en_LastExtraError),
#endif	
#ifdef SDF_TLS
	/*
	 * TLS related errors.
	 */
	Sdf_en_invalidTlsCertificateInfoError = (Sdf_en_LastServiceError+1),
	Sdf_en_invalidTlsCiphersInfoError     = (Sdf_en_LastServiceError+2),
	Sdf_en_DHParamFileError               = (Sdf_en_LastServiceError+3),
	Sdf_en_EGDRandFileError               = (Sdf_en_LastServiceError+4),
	Sdf_en_noSipSUriInContact			  = (Sdf_en_LastServiceError+5),
	Sdf_en_invalidSipSUrlParamError       = (Sdf_en_LastServiceError+6),
	Sdf_en_socketInfoMapError             = (Sdf_en_LastServiceError+7),
	Sdf_en_tlsConnectError                = (Sdf_en_LastServiceError+8),
	Sdf_en_transportSchemeMismatch        = (Sdf_en_LastServiceError+9),
	Sdf_en_LastTlsRelatedError            = (Sdf_en_transportSchemeMismatch),
#else
	Sdf_en_LastTlsRelatedError            = (Sdf_en_LastServiceError),
#endif
#ifdef SDF_REFER
	Sdf_en_noReferToHdrError			=	(Sdf_en_LastTlsRelatedError+1),
	Sdf_en_replacesInNonInviteRequestError	=	(Sdf_en_LastTlsRelatedError+2),
	Sdf_en_badReplacesHeaderError			=	(Sdf_en_LastTlsRelatedError+3),
	Sdf_en_LastReferError				=   	(Sdf_en_badReplacesHeaderError),
#else
	Sdf_en_LastReferError				=   (Sdf_en_LastTlsRelatedError),
#endif
	
#ifdef SDF_SUBSCRIPTION_LAYER
	Sdf_en_uaSLTmrStartErr           = Sdf_en_LastReferError+1,
	Sdf_en_uaSLTmrStopErr            = Sdf_en_LastReferError+2,
	Sdf_en_uaSLTmrRestartErr         = Sdf_en_LastReferError+3,
	Sdf_en_uaSLInvalidOperation      = Sdf_en_LastReferError+4,
	Sdf_en_uaSLInvalidTrigger        = Sdf_en_LastReferError+5,
	Sdf_en_uaSLValidateExpiresCbkErr = Sdf_en_LastReferError+6,
	Sdf_en_uaSLInvalidMsgPassedToSL  = Sdf_en_LastReferError+7,
	Sdf_en_uaSLInvalidTriggerRecvd   = Sdf_en_LastReferError+8,
	Sdf_en_invalidTxnTimeoutErr      = Sdf_en_LastReferError+9,
	Sdf_en_uaSLInvalidTimerExpire    = Sdf_en_LastReferError+10,
	Sdf_en_uaSLInvalidSubsType       = Sdf_en_LastReferError+11,
	Sdf_en_lowExpiresTimerError		   = Sdf_en_LastReferError+12,
	Sdf_en_expiresHdrError		       = Sdf_en_LastReferError+13,
	Sdf_en_eventHdrError		         = Sdf_en_LastReferError+14,
	Sdf_en_subsStateHdrError		     = Sdf_en_LastReferError+15,
	Sdf_en_subsListFail     		     = Sdf_en_LastReferError+16,
#ifdef SDF_HA
	Sdf_en_subsObjectAccessError     = Sdf_en_LastReferError+17,
	Sdf_en_uaSLLastSubsError         = Sdf_en_subsObjectAccessError,
#else
	Sdf_en_uaSLLastSubsError         = Sdf_en_subsStateHdrError,
#endif
#else
	Sdf_en_uaSLLastSubsError         = Sdf_en_LastReferError,
#endif

#ifdef SDF_XML_PARSER
    Sdf_en_uaXMLParseError             = Sdf_en_uaSLLastSubsError+1,
    Sdf_en_uaXMLLastError,               
#else
    Sdf_en_uaXMLLastError =  Sdf_en_uaSLLastSubsError, 
#endif    
    
#ifdef SDF_LOOKUP
	Sdf_en_noMoreDNSRecord		= (Sdf_en_uaXMLLastError+1),
	Sdf_en_invalidDNSrecord		= (Sdf_en_uaXMLLastError+2),
	Sdf_en_dnsTimeout			= (Sdf_en_uaXMLLastError+3),
	Sdf_en_dnsInvState			= (Sdf_en_uaXMLLastError+4),
#ifdef SDF_ASYNC_DNS
	Sdf_en_postQueryRespError   = (Sdf_en_uaXMLLastError+5),
#endif
	Sdf_en_uaLookupLastError,               
#else
	Sdf_en_uaLookupLastError	= (Sdf_en_uaXMLLastError),          
#endif 

#ifdef SDF_DEP 
	Sdf_en_DepStateNotSupported = (Sdf_en_uaLookupLastError +1),
	Sdf_en_uaDepLastError,
#else
	Sdf_en_uaDepLastError = Sdf_en_uaLookupLastError,
#endif
	Sdf_en_LastUatkError
} Sdf_ty_errorCode;

/*
 * Number of error codes defined by the UA toolkit
 */
#define Sdf_co_numErrors 		 Sdf_en_LastUatkError

/*
 * Max length of the error string corresponding to each
 * error number
 */
#define Sdf_mc_errLength 				  200


/*Bit Masks to Disable HandleRequire and IsSupported checks*/
#define	Sdf_mc_disableRequireCheck	(0x1)
#define	Sdf_mc_disableMethodCheck	(0x2)

/*
 * Error structure returned by UA Toolkit APIs. The ErrMsg is a string that
 * gives detailed information of the error in case UA Toolkit is compiled
 * with SDF_ERROR.
 */
typedef struct
{
	Sdf_ty_errorCode 	errCode;
	Sdf_ty_errorCode	stkErrCode;
	Sdf_ty_u32bit		appErrCode;
	Sdf_ty_s8bit		ErrMsg[Sdf_mc_errLength];
} Sdf_st_error;


typedef enum
{
	/* Invalid state transition */
	Sdf_en_invalidStateTransition = 0, 
	/* Idle */
	Sdf_en_idle = 1,
	/* INVITE received */
	Sdf_en_inviteReceived = 2, 
	/* INVITE sent */
	Sdf_en_inviteSent = 3, 
	/* re-INVITE received */
	Sdf_en_reInviteReceived = 4, 
	/* re-INVITE sent */
	Sdf_en_reInviteSent = 5,
	/* Final response to re-INVITE received */
	Sdf_en_reInviteFinalResponseReceived = 6, 
	/* Final response to re-INVITE sent */
	Sdf_en_reInviteFinalResponseSent = 7,
	/* 2xx to INVITE received */
	Sdf_en_finalSuccessReceived = 8,	
	/* 2xx to INVITE sent */
	Sdf_en_finalSuccessSent = 9,		
	/* >= 3xx received	*/
	Sdf_en_finalFailureReceived = 10,
	/* >= 3xx sent	*/
	Sdf_en_finalFailureSent = 11,		
	/* ACK received/sent */
	Sdf_en_callEstablished = 12, 		
	/* BYE received */
	Sdf_en_byeReceived = 13,
	/* BYE sent */
	Sdf_en_byeSent = 14,			
	/* If CANCEL/BYE is received when a final response to INVITE has
	   not been sent */
	Sdf_en_inviteCancelled = 15,
	/* If CANCEL/BYE is received when a final response to reINVITE has
	   not been sent */
	Sdf_en_reInviteCancelled = 16,
	/* Final response received/sent for the BYE */
	Sdf_en_callTerminated = 17,
 	/* For any non INVITE, CANCEL, BYE */
	Sdf_en_transactionReceived = 18,
	/* For any non INVITE,CANCEL, BYE */
	Sdf_en_transactionSent = 19,
	/* Reliable provisional response received */
	Sdf_en_rprReceived = 20,
	/* Reliable provisional response sent */
	Sdf_en_rprSent = 21,
	/* PRACK/COMET received */
	Sdf_en_overlapTransactionReceived = 22,
	/* PRACK/COMET sent */
	Sdf_en_overlapTransactionSent = 23,
	/* Transaction Cancel other than INVITE */
	Sdf_en_transactionCancel=24,

	/*  Provisional Response Sent */
	Sdf_en_provisionalRespSent=25,
	/*  Provisional Response Received */
	Sdf_en_provisionalRespReceived=26,
		
	/*  Provisional Response For ReInvite Received */
	Sdf_en_reInviteprovRespReceived=27,
	/*  Provisional Response For ReInvite Sent */
	Sdf_en_reInviteprovRespSent=28
	/* If any state added below this, update the Sdf_mc_noOfStates also*/
}Sdf_ty_state;

/* ---------------------
 * 		IMPORTANT
 * ---------------------
 * In case of any additions of further states, please modify the sizes of 
 * transition tables accordingly
 */
#define Sdf_mc_noOfStates (Sdf_en_reInviteprovRespSent + 1)

/* This is not a normal call state. This state is used only by HA module to
 * indicate to the standby that the call object has been deleted */
#ifdef SDF_HA
#define Sdf_en_callObjectDeletedState 	(Sdf_mc_noOfStates+1)
#endif

/* 
 * Message type enumeration - Required for the state 
 * transition/CSM tables 
 */
typedef enum
{
	Sdf_en_invite = 0,
	Sdf_en_cancel,
	Sdf_en_ack,
	Sdf_en_bye,
	Sdf_en_finalSuccess,
	Sdf_en_finalFailure,
	Sdf_en_options,
	Sdf_en_register,
	Sdf_en_info,
#ifdef SIP_IMPP
	Sdf_en_subscribe,
	Sdf_en_notify,
#endif
#ifdef SDF_IM
	Sdf_en_message,
#endif
	Sdf_en_unknownRequest,
	Sdf_en_rpr,
	Sdf_en_prack,
	Sdf_en_comet,
	Sdf_en_update,
	Sdf_en_provisionalResp,
#ifdef SDF_REFER
	Sdf_en_refer,
#endif
	Sdf_en_RegularTxn
}Sdf_ty_messageType; 	


/* ---------------------
 * 		IMPORTANT
 * ---------------------
 * In case of any additions of further message types, please modify the 
 * sizes of transition tables accordingly
 */
#define Sdf_mc_noOfMessageTypes (Sdf_en_RegularTxn +1)

/* 
 * Enumeration used to determine whether the state is being changed on 
 * receipt/sending of a SIP Message
 */
typedef enum
{
	/* SIP message is being sent out */
	Sdf_en_msgModeSend = 0,

	/* SIP message was just received for this call */
	Sdf_en_msgModeRecd,

	/* CANCEL received before INVITE was completed */
	Sdf_en_msgModeCancelRecd,

	/* BYE received before INVITE was completed */
	Sdf_en_msgModeByeRecd

} Sdf_ty_msgMode;



#endif
