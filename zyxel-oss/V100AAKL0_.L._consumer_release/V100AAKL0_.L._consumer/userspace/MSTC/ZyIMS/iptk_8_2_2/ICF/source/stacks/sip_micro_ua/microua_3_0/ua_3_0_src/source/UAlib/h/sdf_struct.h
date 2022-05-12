/******************************************************************************
 ** FUNCTION:
 **	 	This file has all the UA Toolkit structures
 ****************************************************************************** 
 **
 ** FILENAME		: sdf_struct.h
 **
 ** DESCRIPTION		: All primary structures and wrappers for basic datatypes
 **					  are stored here. It is expected that all programs use
 **					  datatypes defined here
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 24/10/00	Anitha						Creation
 ** 25/09/01	Subhash						Updated with structure 
 **											modifications
 ** 22/04/03    Anil Kumar     ---          Changes for TLS feature.
 ** 12/06/03    Anuradha Gupta  SPR 19737   Klocworks Warning Removal
 ** 31-Aug-2009 Rajiv Kumar     SPR 19999   Merged CSR 1-7562616
 ** 
 ****************************************************************************** 
 **				COPYRIGHT , Aricent, 2009
 ******************************************************************************/

#ifndef __SDF_STRUCT_H__
#define __SDF_STRUCT_H__
#include "microsip_struct.h"
#include "microsip_txnstruct.h"
#include "sdf_common.h"
#include "sdf_sdpstruct.h"


#ifdef SDF_HA
#include "sdf_hacommon.h"
#endif



#ifdef SDF_SUBSCRIPTION_LAYER
typedef struct Sdf_tg_uaSLSubscriptionKey Sdf_st_uaSLSubscriptionKey;
typedef struct Sdf_tg_uaSLSubscriptionObject Sdf_st_uaSLSubscriptionObject;
typedef enum {
    Sdf_en_uaSLInvalidEvent,
    /*--------- NOTIFY callback events----------------------*/
    Sdf_en_uaSLNtfyFailureResp,
    Sdf_en_uaSLNtfySuccessResp,
    Sdf_en_uaSLNtfyActiveRecvd,
    Sdf_en_uaSLNtfyPendingRecvd,
    Sdf_en_uaSLNtfyTermRecvd,
    Sdf_en_uaSLEarlyNtfyActiveRecvd,
    Sdf_en_uaSLEarlyNtfyPendingRecvd,
    Sdf_en_uaSLNtfyFailureWithRetryAfter,
    Sdf_en_uaSLTermNtfyFailureResp,
    Sdf_en_uaSLTermNtfySuccessResp,
    /*---------- Refresh SUBSCRIBE callback events -----------*/
    Sdf_en_uaSLRefreshSubsFailureResp,
    Sdf_en_uaSLRefreshSubsSuccessResp,
    
    /*---------- UNSUBSCRIBE callback events ----------------*/
    Sdf_en_uaSLUnsubscribeFailureResp,
    Sdf_en_uaSLUnsubscribeSuccessResp,
    
    /*---------- Transaction timeout callback events -------*/
    Sdf_en_uaSLNewSubsTxnTimeoutRecvd,
    Sdf_en_uaSLSubsTxnTimeoutRecvd,
    Sdf_en_uaSLUnSubsTxnTimeoutRecvd,
    Sdf_en_uaSLNtfyTxnTimeoutRecvd,
    Sdf_en_uaSLTermNtfyTxnTimeoutRecvd,

    /*-----------SUBSCRIBE response events -----------------*/
    Sdf_en_uaSLNewSubsAcceptRecvd,
    Sdf_en_uaSLNewSubsRejectRecvd,
    Sdf_en_uaSLNewSubsDeferRecvd,

    /*-----------SUBSCRIBE/NOTIFY Challenge-----------------*/
    Sdf_en_uaSLNewSubsChallengeRecvd,
    Sdf_en_uaSLRefreshSubsChallengeRecvd,
    Sdf_en_uaSLNtfyChallengeRecvd

} Sdf_ty_uaSLCbkEvent;
#endif
#ifdef SDF_DEP
typedef enum
{
  Sdf_en_Accept,
  Sdf_en_Defer,
  Sdf_en_Reject,
  Sdf_en_Challenge,
  Sdf_en_Shared,
  Sdf_en_Accept_Virtual
} Sdf_ty_DepCbkRetVal;
typedef struct Sdf_tg_depDetails  Sdf_st_depDetails;
typedef struct Sdf_tg_DepCbkRetVal Sdf_st_DepCbkRetVal;
typedef struct Sdf_tg_DialogUpdate Sdf_st_DialogUpdate;

#endif


#ifdef SDF_LOOKUP
#if defined(ICF_PORT_WINDOWS) || defined(SDF_WINDOWS) || defined(SDF_WINCE)
#define THREAD_SPECIFIC_INDEX DWORD
#endif
#if defined(ICF_PORT_LINUX) || defined(SDF_LINUX) || defined(ICF_PORT_VXWORKS) || defined(SDF_VXWORKS)
#define THREAD_SPECIFIC_INDEX Sdf_ty_s32bit
#endif
typedef THREAD_SPECIFIC_INDEX Sdf_st_ThreadKey;
#endif

/*Change for vxWork Compilation*/ 
/*
#ifdef SDF_LOOKUP
#if defined(SDF_WINDOWS) || defined(SDF_WINCE) || defined(ICF_PORT_WINDOWS)
#define THREAD_SPECIFIC_INDEX DWORD
typedef THREAD_SPECIFIC_INDEX Sdf_st_ThreadKey;
#else
typedef pthread_key_t Sdf_st_ThreadKey;
#endif 
#endif
*/
#define SDF_NORETRANS SIP_NORETRANS

/*
 * State enumeration - 	States for the call itself and other transactions
 * 						that can be progressing along with the call
 */




/*
 * Enumeration used as return value from sdf_ivk_uaDecodeMessage.
 */
typedef enum
{
	/*
	 * The message received was a remote retransmission of a message which
	 * has already been responded to. The UA Toolkit resends the previous
	 * response again. Application can ignore this message if this return
	 * type is given.
	 */
	Sdf_en_remoteRetransmission=0,
	
	/*
	 * This value is returned when validity checks in the function fails. 
	 * Example: error was unallocated or an	invalid param was passed to 
	 * the function
	 */
	Sdf_en_fail,		  

#ifdef SDF_TXN
 	/*
 	 * The received message was a stray SIP message. Examples: a PRACK
 	 * without an associated RPR, an ACK without a final response, 
 	 * or a final response without having sent a request etc.
 	 */
 	Sdf_en_strayMessage,
#endif
	
	/*
	 * If the message was decoded successfully and is not a remote
	 * retransmission, then this value is returned.
	 */
	Sdf_en_success

}Sdf_ty_matchMessageType;

#ifdef SDF_TXN
/*
 * This enumeration includes all values used by the stack in its
 * corresponding enumeration en_SipTimerType. It also adds a value
 * for the "no-answer" timer that is started on reception of a 
 * provisional response.
 */
typedef enum
{
	Sdf_en_timerA_B,	/* For Starting TimerA along With TimerB */
	Sdf_en_timerB,
	Sdf_en_timerC,		/* Never used by UATK */
	Sdf_en_timerCr,		/* Never used by UATK */
	Sdf_en_timerD,
	Sdf_en_timerE_F,	/* For Starting TimerE along With TimerF */
	Sdf_en_timerF,
	Sdf_en_timerG_H,	/* For Starting TimerG along With TimerH */
	Sdf_en_timerH,
	Sdf_en_timerI,
	Sdf_en_timerJ,
	Sdf_en_timerK,
	Sdf_en_timerForCancelledState,  
	Sdf_en_timerForProxyUAS2xxFwded,	/* Never used by UATK */
	Sdf_en_lastStackTimer	= SipTimerLast,
	Sdf_en_timerNoAnswer	= (Sdf_en_lastStackTimer + 1)
} Sdf_ty_txnTimerType;
#endif

typedef enum
{
	Sdf_en_invalidDialogState=0,
	Sdf_en_confirmedDialogState,
	Sdf_en_earlyDialogState
}Sdf_ty_callState;


/*
 * Enumeration used as return type from APIs used to validate a SIP message
 * in various terms. The APIs which use this return value are:
 * --------------------------------------------------------------------------
 *  1. sdf_ivk_uaUpdateCallDetails -  which checks whether the message recd 
 *  									 is allowed in the current state.
 *  2. sdf_ivk_uaValidateUri  	 -	  which validates the Request-URI of the
 *  								  request recd against a given profile
 *  3. sdf_ivk_uaIsSupportedMethod -  which validates the method of the
 *  								  message recd.
 *  4. sdf_ivk_uaHandleRequire	 -    which checks to see if the request
 *  							 	  came with a Require header that is
 *  							 	  not supported.
 * --------------------------------------------------------------------------
 */
typedef enum
{
	/* Message is valid */
	Sdf_en_valid = 0,
	
	/* Invalid message received in the current state */
	Sdf_en_invalid,
	
	/* 
	 * Message received is to be discarded. Reasons include:
	 * 		- lower Cseq request
	 * 		- response recd with topmost Via having a 'sentby' field which
	 * 		  is not same as local host and port.
	 * 		- local retransmission of the final response for a transaction
	 * 		  that was completed.
	 */
	Sdf_en_discarded,

	/* API failed due to some error while processing */
	Sdf_en_failed

} Sdf_ty_messageValidity;	
/* IPTK CML Start */
typedef enum
{
	Sdf_en_validlength = 0,
	Sdf_en_mandatoryhdrlong = 1,
	Sdf_en_optionalhdrlong = 2,
	Sdf_en_msgbodylinelong = 4,
    Sdf_en_reqresplinelong = 8,
    Sdf_en_headernamelong = 16,
    Sdf_en_headervaluelong = 32
} Sdf_ty_IPTKmessageLengthValidity;
/* IPTK CML End */
	
#ifdef SDF_AUTHENTICATE
/*
 * Enumeration used by the authentication module. Application needs to
 * return one of the values listed below from the callbacks:
 * sdf_cbk_uaChallengeUser & sdf_cbk_uaAuthenticateUser.
 */
typedef enum
{
	/*
	 * Request needs to be authenticated.
	 */
	Sdf_en_authReqd =0,

	/*
	 * Request need not be authenticated.
	 */
	Sdf_en_authNotReqd,

	/*
	 * Request has to be denied.
	 */
	Sdf_en_deny,
	
	/*
	 * Used to indicate some application event occured at the time of
	 * retrieval of authentication info. The application can pass any
	 * data from the (authentication related) callback to be used in 
	 * the callback from which authentication began.
	 */
	Sdf_en_appInfo,

	/*
	 * Any API failure at the time of processing the authentication.
	 */
	Sdf_en_error

}Sdf_ty_authResult;
#endif /* SDF_AUTHENTICATE */
/*
 * Enumeration to indicate the status of cancellation at the Caller's end
 */
typedef enum
{
	/* Reset after completion of CANCEL */
	Sdf_en_reset=0,

	/* CANCEL has been sent */
	Sdf_en_cancelSent,

	/* Final response to CANCEL has been received */
	Sdf_en_cancelComplete

} Sdf_ty_callerCancelStatus;

typedef enum
{
	/* UAC transaction structure */
	Sdf_en_uacTransaction = 0,

	/* UAS transaction structure */
	Sdf_en_uasTransaction,

	/* Overlapped transaction structure */
	Sdf_en_overlapTransaction,

	Sdf_en_RegularUacTransaction,
	
	Sdf_en_RegularUasTransaction,

	Sdf_en_InvalidTransaction

} Sdf_ty_transactionType;


/*
 * Enumeration to identify whether this UA is the caller
 * or callee for the call.
 */
typedef enum
{
	Sdf_en_any = 0,

	Sdf_en_caller,

	Sdf_en_callee
	
} Sdf_ty_partyInCall;



/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/*
 * The function to be used to calculate the hash value for the key passed.
 */
typedef Sdf_ty_u32bit (*Sdf_ty_uaHashFunc) (void *pData);

/*
 * The function to be used to compare keys of 2 hash elements.
 */
typedef Sdf_ty_u8bit (*Sdf_ty_uaHashKeyCompareFunc) (void *pKey1,\
					void *pKey2);

/*
 * The function to be given when wanting to iterate through all the
 * elements of the hash table. 
 */
typedef int (*Sdf_ty_uaHashIteratorFunc) (void *pKey, void *pElement);

/*
 * The function to free the data stored in the hash element.
 */
typedef void (*Sdf_ty_uaHashElementFreeFunc) (void *pElement);

/*
 * The function to free the key stored in the hash element.
 */
typedef void (*Sdf_ty_uaHashKeyFreeFunc) (void *pKey);


/*
 * Hash element - The hash table contains chains of this
 * 				  structure.
 */
typedef struct Sdf_tg_uaHashElement
{
	/* 
	 * Data to be stored in the hash table.
	 */
	void*				pElement;

	/* 
	 * Key to be used to retrieve this data later.
	 */
	void*				pKey;

	/* 
	 * The next element in the hash chain. Will be NULL is the current
	 * element is the lat element of the chain.
	 */
	struct Sdf_tg_uaHashElement*	pNext;

	/*
	 * Flag that will be set to true if the API sdf_ivk_uaHashRemove is
	 * unable to free the memory for the hash element at time of
	 * invocation. As soon as the structures refCount reduces to zero, the
	 * memory will be freed if this flag is set.
	 */
	Sdf_ty_bool			dRemove;

	/*
	 * Keep track of how many check-outs of this data have happened. If the
	 * refCount is greater than 1 and sdf_ivk_uaHashRemove is invoked,
	 * memory will not be freed and the dRemove flag will be set.
	 */
	Sdf_ty_refCount		dRefCount;
} Sdf_st_hashElement;

/*
 * Iterator needed to operate on all the hash elements. User will need to
 * do sdf_ivk_uaHashNext() to get the next element from the hash table.
 */
typedef struct
{
	Sdf_st_hashElement 		*pCurrentElement;
	Sdf_ty_u32bit			currentBucket;
} Sdf_st_hashIterator;

/*
 * The actual hash table data structure.
 */
typedef struct
{
	/* function to calculate hash value */
	Sdf_ty_uaHashFunc				fpHashFunc; 
	
	/* function to compare keys of 2 elements */
	Sdf_ty_uaHashKeyCompareFunc	fpCompareFunc; 
	
	/* function to free the data being stored */ 
	Sdf_ty_uaHashElementFreeFunc	fpElementFreeFunc; 
	
	/* function to free the key given for an element */
	Sdf_ty_uaHashKeyFreeFunc		fpKeyFreeFunc; 

	/* Number of buckets and number of elements in the hash */
	Sdf_ty_u32bit 				numberOfBuckets;
	Sdf_ty_u32bit				numberOfElements;
	Sdf_ty_u32bit				maxNumberOfElements;

	/* Resizable array that holds the hash lists */
	Sdf_st_hashElement			**ppHashChains;
	
#ifdef SDF_THREAD_SAFE
	/* Mutex for each bucket of the hash table */
	Sdf_ty_syncIdT 			*pBucketSyncObject;
    Sdf_ty_syncIdT			dHashSyncObject;
#endif
} Sdf_st_hash;

/*
 * Used to return information from the From/To/Contact headers in a
 * convenient format.
 */
typedef struct
{
	Sdf_ty_s8bit		*pName;
	Sdf_ty_s8bit		*pAddress;
	Sdf_ty_s8bit		*pTag;
	Sdf_ty_s8bit		*pScheme;
	Sdf_ty_u16bit 		dPort;
	
	/* Lists of SipParam structures */
	Sdf_ty_slist		slUrlParams;	
	Sdf_ty_slist		slHeaderParams;
	
	Sdf_ty_refCount		dRefCount;
}Sdf_st_nameInfo;

/*
 * Transport protocol. Used in sdf_ivk_uaDecodeMessage
 * to indicate to the UA toolkit the transport protocol on which the
 * incoming SIP message arrived and in the Sdf_st_transportInfo.
 */
typedef enum
{
    Sdf_en_protoNone = 0,
	Sdf_en_protoUdp,
#ifdef SDF_TLS
	Sdf_en_protoTls,
#endif
	Sdf_en_protoTcp
#ifdef SDF_LOOKUP
	,Sdf_en_protoAny
#endif
} Sdf_ty_protocol;

#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
typedef struct
{
	Sdf_ty_protocol	dTransportScheme;
	en_AddrType		dProtocolScheme;
}Sdf_ty_addrType;

#endif /*End:  ssf specific changes done by subbaroa */


/*
 * Used to hold information related to a transport address
 */
typedef struct
{
	/* IP address */
	Sdf_ty_s8bit		*pIp;
	/* Port */
	Sdf_ty_u32bit		dPort;
	/* Scheme; One of "TCP" or "UDP" */
	Sdf_ty_protocol		dScheme;

	Sdf_ty_refCount		dRefCount;

}Sdf_st_transportInfo;


/*
 * Structure used during PRACK/RPR handling.
 */
typedef struct
{
	/* Highest Rseq received by this UA */
	Sdf_ty_u32bit 		dRseq;

	/* Remote Cseq for which this Rseq was received */
	Sdf_ty_u32bit 		dCorrelationCseq;

	Sdf_ty_refCount dRefCount;

} Sdf_st_remoteRseq;

/*
 * Information related to the main call itself.
 */
typedef struct
{
	/* State of the main call (INVITE/OK/ACK/BYE) */
	Sdf_ty_state 	dState;

	Sdf_ty_bool		dInviteInProg;

#ifdef SDF_TLS
	/* 
	 * Whether the call is a secure call (i.e. whether established over TLS) 
	 * or not.
	 */
	Sdf_ty_bool	    dSecureCall;
#endif

	/*
	 * This variable will be used for sending correct Contact headers in the response 
	 * messages.
	 */
    
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
 Sdf_ty_addrType     dContactAddrType;
#else
    en_AddrType     dContactAddrType;
#endif /*End:  ssf specific changes done by subbaroa */

	Sdf_ty_refCount dRefCount;

} Sdf_st_callInfo;

/*
 * Internal structure. Used as the key for each remote retrans entry
 * (maintained by the UA Toolkit) in the hash element.
 */
typedef struct
{
	SipHeader *pFromHeader;
	SipHeader *pToHeader;
	Sdf_ty_s8bit *pCallId;
	Sdf_ty_u32bit dCseqNo;
	Sdf_ty_s8bit *pMethod;
	Sdf_ty_s8bit *pViaBranch;
	en_SipMessageType	dMessageType;
	Sdf_ty_refCount dRefCount;
} Sdf_st_remoteRetransKey;

/*
 * Call object key. Part of every call object. Used to uniquely identify
 * the call object.
 */
typedef struct
{
	SipHeader 		*pLocalAddr;
	SipHeader 		*pRemoteAddr;
	Sdf_ty_s8bit 	*pCallId;
	Sdf_ty_s8bit	*pLocalViaBranch;
	Sdf_ty_s8bit	*pRemoteViaBranch;
	/* 
	 * Enum indicating whether this UA or the 
	 * remote one started the call 
	 */
	Sdf_ty_partyInCall dCaller;
	Sdf_ty_refCount dRefCount;

} Sdf_st_callObjectKey;



typedef enum
{
   	Sdf_en_sdpVersion=0,
	Sdf_en_sdpOrigin,
	Sdf_en_sdpSession,
	Sdf_en_sdpInformation,
	Sdf_en_sdpUri,
	Sdf_en_sdpEmail,			
	Sdf_en_sdpPhone,			
	Sdf_en_sdpConnection,
	Sdf_en_sdpBandwidth,			
	Sdf_en_sdpTime,				
	Sdf_en_sdpKey,
	Sdf_en_sdpAttr,				
	Sdf_en_sdpMedia	
}Sdf_ty_sdpLineType;

/*
 * State maintenance option. In the transaction stateful mode auto-response 
 * generation from the UA is replaced by calls to sdf_cbk_uaTransactionReceived
 */
typedef enum
{
	/* Call-stateful mode */
	Sdf_en_callStateful,

	/* Transaction-stateful mode */
	Sdf_en_transactionStateful

} Sdf_ty_stateMaintenance;

/*
 * Enumeration returned by the API sdf_ivk_uaCompareCallObjectKeys.
 */
typedef enum
{
	/* The keys passed are not matching */
	Sdf_en_notMatching,

	/* 
	 * The keys passed are for 2 messages belonging to the same call-leg.
	 */
	Sdf_en_sameCallLeg,

	/*
	 * The keys passed belong to the same call, but one of them is a new
	 * call-leg in the already existing call.
	 */
	Sdf_en_differentCallLeg,

#ifdef SDF_SUBSCRIPTION_LAYER
	/*
	 * If this value is returned, it indicates that the initial SUBSCRIBE
	 * was forked. This is an in/out value i.e. if set then the check for
	 * forking of SUBSCRIBE is made.
	 */
	Sdf_en_forkedSubscribe,

	/*
	 * If this value is returned, it indicates that this is an early NOTIFY
	 * i.e. a NOTIFY is received before the 2XX to the SUBSCRIBE. This is
	 * an in/out value i.e. if set then the check for early NOTIFY is made.
	 */
	Sdf_en_earlyNotify,
#endif

	/*
	 * The 2 keys passed are such that username of To header in one of the
	 * keys is a subset of username in the other. This is a case of Overlap
	 * dialling.
	 */
	Sdf_en_overlapDialing
			
} Sdf_ty_callLegMatch;

/* 
 * Structure to hold any application specific data/context.
 * This is passed to the application in the relevant UA toolkit
 * and stack callbacks
 */
typedef struct
{
	/* 
	 * void pointer where the application can attach any
	 * data/context that it wants to maintain for the call
	 */
	Sdf_ty_pvoid 		pData;
	Sdf_ty_refCount 	dRefCount;

} Sdf_st_appData;


#ifdef SDF_TLS

/* Enumeration for file format types. */
typedef enum
{
	Sdf_en_fileFormatInvalid=0,
	Sdf_en_fileFormatPEM,
	Sdf_en_fileFormatDER
}Sdf_ty_fileFormat;


/*
 * Structure containing placeholder for all the TLS parammeters that are
 * configured through xml file.
 */
typedef struct
{
	Sdf_ty_s8bit		*pCertFile;
	Sdf_ty_fileFormat	dCertFormat;
	Sdf_ty_s8bit		*pPrivateKeyFile;
	Sdf_ty_s8bit		*pPrivateKeyPasswd;
	Sdf_ty_s8bit		*pCACertDirPath;
        /* CSR_1-5036521*/
	Sdf_ty_s8bit		*pCACertFile;
	/* CSR_1-5036521*/
	Sdf_ty_s8bit		*pCipherSuites;
	Sdf_ty_s8bit		*pDHParamFile;
	Sdf_ty_fileFormat	dDHParamFileFormat;
	Sdf_ty_u8bit		dCertVerifyMode;
    Sdf_ty_refCount     dRefCount;

} Sdf_st_sslParams;


typedef struct
{
	/* 
	 * Void pointer where the application can store any TLS specific
	 * data/context that it wants to store for the profile.
	 */
    Sdf_ty_pvoid        pData;
	Sdf_st_sslParams	*pSslParams;
    Sdf_ty_refCount     dRefCount;

} Sdf_st_sslData;

/*
 * Structure used for mapping TLS cipher names to SSL cipher names.
 */
typedef struct
{
	Sdf_ty_s8bit		pTlsCipherName[Sdf_co_maxCipher_suite_NameLength];
	Sdf_ty_s8bit		pSslCipherName[Sdf_co_maxCipher_suite_NameLength];

} Sdf_st_tlsToSslCipherMap;


#endif /* SDF_TLS */

#ifdef SDF_LOOKUP
/*
 * SRV Records and it's corresponding A/AAAA records are maintained 
 */  
typedef struct sdf_tg_dnsSrvRecord {
	/* 	TTL for the SRV record */
	Sdf_ty_u32bit 		dttl;
	
	/*	Priority of the SRV record */
	Sdf_ty_u16bit 		dPriority;

	/*	Weight of the SRV record	*/
	Sdf_ty_u16bit 		dWeight;
	
	/*	Port number specified for this SRV record */
	Sdf_ty_u16bit 		dPort;

	/*	Transport mechanism needs to be supported	*/
	Sdf_ty_protocol 	protocol;

	/*  The following list is the result of the A/AAAA record */
	Sdf_ty_slist		slIpRecords;

	/*	Destionation address (Domain name)	*/
	Sdf_ty_s8bit		*pTarget;

} Sdf_st_dnsSrvRecord;

typedef enum {

	Sdf_en_unKnownAddr,

	Sdf_en_IPv4Address, /* Ipv4 DNS record  */

	Sdf_en_IPv6Address, /* Ipv6 DNS record */

	Sdf_en_SRVrecord,

	Sdf_en_NAPTRrecord,
	
	Sdf_en_IPAddress, /* This is indicate A/AAAA record */

	Sdf_en_InvalidAddr, /* Invalid Address is provided */

	/* This is the latest Ipv6 record which will not be demonstrated in the
         * UATK application. The application can make use if necessary arises. 
	 */
	Sdf_en_A6record 
} Sdf_ty_DnsRecordType;	

typedef enum {

	Sdf_en_UnKnownUrl,

	Sdf_en_SipUrl, 

#ifdef SDF_TEL
	Sdf_en_TelUrl, 
#endif
#ifdef SDF_IM
	Sdf_en_ImUrl,
#endif
#ifdef SDF_PRES
	Sdf_en_PresUrl,
#endif	

	Sdf_en_SipsUrl 

} Sdf_ty_UrlType;	

typedef struct Sdf_tg_DnsRrecord {

	Sdf_ty_DnsRecordType 		dDnsRecordType;

	/* Contains the Record value */
	Sdf_ty_pvoid  				pValue;
} Sdf_st_DnsRecord;


typedef enum Sdf_tg_CallSendstate
{
	/* Invalid State Transistion */
	Sdf_en_invalidStateTransistion = 0,
	/* DNS query state */
	Sdf_en_dnsquery ,
	/* DNS Next Record */
	Sdf_en_dnsNextRecord,
	/* TCP Connection state */
	Sdf_en_tcpconnect ,
	/* Network send state */
	Sdf_en_sendstate

} Sdf_st_CallSendstate;

typedef enum Sdf_tg_TxnDnsState
{
	/* Invalid State Transistion */
	Sdf_en_invalidDnsStateTransistion = 0,
	/* Idle */
	Sdf_en_Idle = 1,
	/* DNS query state */
	Sdf_en_dnsQuery ,
	/* DNS First IP record filled in DestInfo*/
	Sdf_en_dnsFirstRecordFilled ,
	/* DNS query failed */
	Sdf_en_dnsFail,
	/* DNS query filled in the DestInfo*/
	Sdf_en_dnsFilled,
	/* DNS filled DestInfo is used already */
	Sdf_en_dnsIpUsed

} Sdf_st_TxnDnsState;

#endif

/*
 * Initialization data stored in the call object. Contains all information
 * about the user's profile.
 */
typedef struct
{
	/* The "From" profile to be used in all calls */
	SipHeader					*pFrom;

	/* The unique tag to be used in all calls in the From/To header */
	Sdf_ty_s8bit				*pTag;

	/* List of contact of this user profile */
	Sdf_ty_slist				slContact;

	/* The registrar information (IP/port) */
	Sdf_st_transportInfo		*pRegistrar;

	/* 
	 * The IP/port on which this user profile accepts calls (also used
	 * in inserting Via header in outgoing requests and other validations)
	 */
	Sdf_st_transportInfo		*pTransport;

	/* 
	 * The list of Contact IP/port on which this user profile accepts calls.
	 */
	Sdf_ty_slist                slContactTransport;

	/* 
	 * Sip List of SipMsgBody 
	 */
	Sdf_ty_slist				slMsgBodyList;
	/*
	 * The Session parameter which represents the local
	 * capabilities
	 */
	Sdf_st_sessionParam         *pSessionParam;
	
	/*
	 * List of headers like supported, require, etc. 
	 * The structure is Sdf_st_configExtraHeaders
	 */
	Sdf_ty_slist				slExtraHeadersList; 
	
#ifdef SDF_IMS
	Sdf_ty_slist				slRespHeadersList;
#endif
	/* 
	 * The call-id generator function is passed a pointer to the pAppData 
	 * stored within the particular callObject for which the Call Id is 
	 * being generated 
	 */
	Sdf_ty_s8bit *				(*sdf_fp_uaCallIdGen)(Sdf_st_appData *pAppData);

#ifdef SDF_HA
	Sdf_st_userProfileHAInfo	*pHaInfo;
#endif
	Sdf_ty_slist				slPreLoadedRoute;

#ifdef SDF_RPORT
	Sdf_ty_bool			dEnableRport;
#endif

#ifdef SDF_TLS
	/*
	 * This will be the placeholder for the TLS connection properties for 
	 * a user profile.
	 */
	Sdf_st_sslData              *pSslData;
#endif

	Sdf_ty_refCount				dRefCount;

} Sdf_st_initData;

/* 
 * Structure that contains information common to the entire call object
 * (that is, both the main call and the mid-call transactions).
 */
typedef struct
{
	/* Unique Key identifying the call object */
	Sdf_st_callObjectKey	*pKey;
	
	/* The "From" information in the call */
	SipHeader 			*pFrom;
	
	/* The "To" information in the call */
	SipHeader			*pTo; 
	
	/* The Request URI set by the application */
	SipAddrSpec			*pRequestUri;

	/* List of Contact Headers of the remote end of type SipHeader */
	Sdf_ty_slist		slContact;
	
	/* 
	 * Boolean Variable to identify that if the preloaded Route set is
	 * updated with actual one or not.
	 */
	Sdf_ty_bool			dPreloadedSet;
	
	/* Boolean variable to indicate if SDP was received (or sent) 
	 * in the the INVITE or is expected (or to be sent) in the ACK
	 */
	Sdf_ty_bool			dSdpInInvite;

	/* The remote user's Cseq Number */
	Sdf_ty_u32bit 		dRemoteCseq;
	
	/* This UA's Cseq Number */
	Sdf_ty_u32bit 		dLocalCseq;
	
	/* The Call-ID for this call */
	Sdf_ty_s8bit		*pCallid;     	   
	
	/* Scheme; One of "TCP" or "UDP" */
	Sdf_ty_protocol		dScheme;

	/* Enum to indicate the status of cancellation at the Caller's end */
	Sdf_ty_callerCancelStatus dCallerCancelStatus;

	/* Highest Rseq sent out by this UA */
	Sdf_ty_u32bit 		dLocalRseq;

	/* Highest (Rseq, Cseq) received by this UA */
	Sdf_st_remoteRseq 	dRemoteRseq;

	/* Method for which RPR (if any) is occuring */
	Sdf_ty_s8bit 		*pRPRMethod;

	/* Highest Cseq PRACK/COMET/Regular Txn sent out so far */
	Sdf_ty_u32bit 		dLocalRegCseq;

	/* Highest Cseq PRACK/COMET/Regular Txn received so far */
	Sdf_ty_u32bit 		dRemoteRegCseq;

#ifdef SDF_TXN
	/* 
	 * This flag is set when any timeout happens on this
	 * call object.
	 */
	Sdf_ty_bool			dIsTimeout;

	/*
	 * Key for the "no-answer" timer that is started for the call.
	 */
	Sdf_ty_pvoid		pNoAnswerTimerHandle;
#endif
	/* List of Route Headers that has gone in the outgoing invite
	 */
	Sdf_ty_slist		slInvRoute;

	Sdf_ty_s8bit		*pChallangePasswd;
    
    /*changes for CSR: 1-5210208*/
	/* We were updating the pRequestUri in handleCall at UAS side which is
	 *  not as per the UATK behavior but we have given that fix as a part 
	 *  of CSR 1-2881054. So to keep that fix and also sending BYE from UAS
	 *  side we have added a new member in pCommonInfo pNewRequestUri.  
	 *  This fix is for CSR 1-5210208 */
	SipAddrSpec			*pNewRequestUri;
	/*changes for CSR: 1-5210208*/
    
    /* CSR_1-7588994 Fix Start */
    Sdf_ty_s16bit dReqUriPort;
    /* CSR_1-7588994 Fix End */    
    
	Sdf_ty_refCount		dRefCount;

} Sdf_st_commonInfo;

/* 
 * Structure that contains a list of headers of a particular kind and the 
 * associated type of the header
 */
typedef struct
{
	/* The header type */
	en_HeaderType 			dType;

	/* List of headers */
	Sdf_ty_slist 			slHeaders;

	/* 
	 * Correlation string for the application to associate a 
	 * header list to a string
	 */
	Sdf_ty_s8bit 			*pCorrelation;

	Sdf_ty_refCount			dRefCount;

} Sdf_st_headerList;

/* enum types for the various types of call statistics */
typedef enum
{
	/* No of calls received */
	Sdf_en_callsReceived,

	/* No of calls initiated by remote end and established */
	Sdf_en_remoteCallsEstd,

	/* No of calls initiated by remote end and rejected */
	Sdf_en_remoteCallsRejd,

	/* No of calls initiated by this end */
	Sdf_en_callsInitiated,
	
	/* No of calls initiated by this end and established */
	Sdf_en_callsEstd,

	/* No of calls initiated by this end and rejected */
	Sdf_en_callsRejd,

	/* No of calls terminated by either end */
	Sdf_en_callsTerminated,

	/* No of transactions (INFO/OPTIONS/SUBSCRIBE/NOTIFY) sent by this end*/
	Sdf_en_transactionsSent,

	/* No of transactions (INFO/OPTIONS/SUBSCRIBE/NOTIFY) recd by this end*/
	Sdf_en_transactionsRecv,

	/* Enable all the above statistics */
	Sdf_en_allStats 

}Sdf_ty_callStats;

typedef enum{
	Sdf_en_NewCallObject=0,
	Sdf_en_PermanentCallObject
} Sdf_ty_callObjectType;




/*
 * The structure containing Socket Id.
 */
typedef struct
{
	Sdf_ty_socketId			dSockfd;

    Sdf_ty_protocol	        dProtocol;
}Sdf_st_socket;


/*
 * Structure that will store information specific to a socket descriptor. 
 * Currently, this structure will contain only data related to SSL 
 * (i.e. the SSL/SSL_CTX structure). It can be extended to store any other 
 * data that are specific to a socket.
 */
typedef struct
{
#ifdef SDF_TLS
	/* Field to store the SSL structure defined by OpenSSL. */
    Sdf_ty_pvoid		pSslData;

	/* Field to store the SSL_CTX structure defined by OpenSSL. */
    Sdf_ty_pvoid		pSslCtxData;
#endif

	/* 
	 * The following field has been added just to remove compilation warnings
	 * when SDF_TLS flag is disabled. This should be removed when a new field is
	 * added to this structure outside the SDF_TLS flag.
	 */
	Sdf_ty_u8bit        dDummy;

} Sdf_st_sockInfo;



/* 
 * The transaction structures. The call object contains one such structure
 * for the UAC and the UAS.
 */
typedef struct
{
	/*
	 * Transaction identifier for the structure.
	 */
	Sdf_ty_txnIdT			dTxnId;
	
	/* 
	 * The state of the transaction. This is used only for transactions
	 * like INFO/OPTIONS/REGISTER/SUBSCRIBE/NOTIFY. All call related state
	 * changes are maintained in the Sdf_st_callInfo structure
	 */
	Sdf_ty_state			dTransactionState;

	/* 
	 * Headers such as Via that need to be sent out in the 
	 * next request/response message. This is a list of 
	 * Sdf_st_headerList structure
	 */
	Sdf_ty_slist 			slProcessedHeaders; 

	/* 
	 * Headers that are not understood by the toolkit will be 
	 * inserted into this list for the Application to process if required. 
	 * This is a list of Sdf_st_headerList structure
	 */
	Sdf_ty_slist 			slUnprocessedHeaders; 

	/* 
	 * SipMsgBodies that will be sent out in the next request/response. 
	 * This is a list of SipMsgBody structs
	 */
	Sdf_ty_slist			slMsgBodyList;

	/* Outgoing/Incoming SIP messages will be placed here */
	SipMessage				*pSipMsg;

	/* Last SIP messages (Request only) will be placed here */
	/* We need to verify for storing Response also */
	SipMessage				*pLastSipMsg;
	
	/* Structure that holds the destination information for the peer entity
	in this call */
	Sdf_st_transportInfo	*pDestInfo;
	
	/* Used in case of TCP mode of transmission */
	Sdf_st_socket			dSocket; 

#ifdef SDF_TXN
 	/* 
 	 * The key for the SIP Stack transaction structure corresponding 
 	 * to this SIP transaction.
 	 */
 	SipTxnKey				*pTxnKey;
	/*This stores the Txn Key for the recently sent INV/Re-INV
	 *This is used in sdf_ivk_uaAbortCall API to abort the 
	 *INVITE Transaction
	 */
	SipTxnKey				*pInvTxnKey;
#endif
#ifdef SDF_LOOKUP
	Sdf_ty_DnsRecordType    dDnsRecordType;

  /*  DNS record will be stored in the following list */
	Sdf_ty_slist 			slDnsRecords;

  /* DNS state for the transaction */
	Sdf_st_TxnDnsState		dTxnDnsState;	

#ifdef SDF_THREAD_SAFE
	/*
	 * This is the mutex for synchronizing changes in DNS call state. It is to
	 * protect the DNS state variable from concurrent state updates 
	 */
	Sdf_ty_mutexIdT			dTxnDnsStateMutex;
#endif
#endif

	Sdf_ty_refCount			dRefCount;
	
}Sdf_st_transaction;

#ifdef SDF_SUBSCRIPTION_LAYER
/**********************************************************************
 * ENUMERATION NAME : Sdf_ty_uaSLNotifiedState
 *
 * DESCRIPTION : This enumeration contains the various Notified State
 * (which are informed by NOTIFY message in Subscription-State header).
 **********************************************************************
 */
typedef enum Sdf_tg_uaSLNotifiedState {
  Sdf_en_uaSLInit,
  Sdf_en_uaSLActive,
  Sdf_en_uaSLPending,
  Sdf_en_uaSLTerminated 
} Sdf_ty_uaSLNotifiedState;

/**********************************************************************
 * STRUCT NAME: Sdf_st_uaSLSubsTxnData
 *
 * DESCRIPTION :This denotes the Subscription Transaction Data that is
 *    data stored within the overlap-txn-info.It contains the 
 *    information specific to the subscription.
 **********************************************************************
 */
typedef struct Sdf_tg_uaSLSubsTxnData {
  Sdf_ty_bool                   dIsUnSubs;
  Sdf_ty_u32bit                 dSubsExpire;
  Sdf_ty_uaSLNotifiedState      dSubsStateHdrValue;
  Sdf_st_uaSLSubscriptionObject *pSubsObj; 
  Sdf_ty_bool                   dIsInitialSubscribe;
  Sdf_ty_refCount               dRefCount;
} Sdf_st_uaSLSubsTxnData;

#endif



/* Structure to handle overlap transactions like RPR, PRACK and COMET. */
typedef struct
{
	Sdf_ty_txnIdT			dTxnId;

	Sdf_ty_messageType		dMsgType;

	Sdf_ty_state 			dState;
	Sdf_ty_s8bit			*pOverlapViaBranch;

	/* 
	 * Headers such as Via that need to be sent out in the next 
	 * request/response message. This is a list of 
	 * Sdf_st_headerList structure
	 */
	Sdf_ty_slist 			slProcessedHeaders; 

	/* 
	 * Headers that are not understood by the toolkit will be inserted into 
	 * this list for the Application to process if required. This is a 
	 * list of Sdf_st_headerList structure
	 */
	Sdf_ty_slist 			slUnprocessedHeaders; 

	/* Rseq of the RPR being handled */
	Sdf_ty_u32bit			dRseq;

	/* Cseq of the overlapping transaction sent from UA-T's end */
	Sdf_ty_u32bit			dLocalCseq;

	/* Cseq of the overlapping transaction recd from other end */
	Sdf_ty_u32bit			dRemoteCseq;
	
	/* 
	 * List of SipMsgBody that were received or will be sent 
	 * in this overlap transaction
	 */
	Sdf_ty_slist			slMsgBodyList;

	/*
	 * The last SIP message received/sent out on this overlap
	 * transaction
	 */
	SipMessage				*pSipMsg;

	/* Last SIP messages (Request only) will be placed here */
	/* We need to verify for storing Response also */
	SipMessage				*pLastSipMsg;
	
	/* Structure that holds the destination information for the peer entity
	in this call */
	Sdf_st_transportInfo	*pDestInfo;
	
	/* Used in case of TCP mode of transmission */
	Sdf_st_socket			dSocket; 

#ifdef SDF_TXN
 	/* 
 	 * The keys for the SIP Stack transaction structures corresponding 
 	 * to this SIP transaction. For RPR Transactions, the stack creates
 	 * 2 transaction structures: one for the RPR itself and another for 
 	 * the PRACK request corresponding to the RPR.
 	 */
 	SipTxnKey				*pRprTxnKey;
 	SipTxnKey				*pOLTxnKey;
#endif

	/* Reference count */
	Sdf_ty_refCount		dRefCount;
#ifdef SDF_SUBSCRIPTION_LAYER
	Sdf_st_uaSLSubsTxnData               *pSubsTxnData;
#endif
#ifdef SDF_LOOKUP
  Sdf_ty_DnsRecordType     dDnsRecordType;
  Sdf_ty_slist 				     slDnsRecords;
  /* DNS state for the transaction */
  Sdf_st_TxnDnsState		dTxnDnsState;	

#ifdef SDF_THREAD_SAFE
	/*
	 * This is the mutex for synchronizing changes in DNS call state. It is to
	 * protect the DNS state variable from concurrent state updates 
	 */
	Sdf_ty_mutexIdT			dTxnDnsStateMutex;
#endif
#endif
} Sdf_st_overlapTransInfo;


#ifdef SDF_SUBSCRIPTION_LAYER
typedef enum{
  Sdf_en_dialogNotUsed = 0,
  Sdf_en_dialogOneUse,
  Sdf_en_dialogMultipleUses,
	Sdf_en_dialogErrorOccur
} Sdf_ty_dialogUsage;
#endif

#ifdef SDF_SESSION_TIMER

typedef enum{
	Sdf_en_refresherNone=0,
	Sdf_en_refresherLocal=1,
	Sdf_en_refresherRemote=2
} Sdf_ty_refresher;

typedef enum{
	Sdf_en_applicaton=0,
	Sdf_en_uatoolkit=1
} Sdf_ty_handleEvent;

typedef enum
{
	Sdf_en_refresherInvite = 0,
	Sdf_en_refresherUpdate,
	Sdf_en_refresherInvalid
}Sdf_ty_sessionRefreshRequest;


typedef struct{
	Sdf_ty_refresher 	dRefresher;
	Sdf_ty_s32bit 		dMinSe;
	Sdf_ty_s32bit		dSessionInterval;
/* CSR_1-7033347  START*/    
    Sdf_ty_s32bit	    dOldSessionInterval;
    Sdf_ty_refresher    dOldRefresher; 
    Sdf_ty_s32bit       dOldMinSe; 
/* CSR_1-7033347 END */

/* This member will store the Supported: timer */	
	Sdf_ty_bool			dIsSupportedTimer;
	/* Indicates whether a refresh is sent or not
	 * And which is the request used -*/
	Sdf_ty_sessionRefreshRequest		dRefreshRequest;
	Sdf_ty_pvoid		pTimerId;
	Sdf_ty_refCount		dRefCount;
} Sdf_st_sessionTimerInfo;

/* SessionTimer specific information passed to the startTimer*/

#endif



/* The Call Object */
typedef struct
{
	/* 
	 * Call Object Identified - a unique 32 bit number that is set when the
	 * the call object is initialized. This ID will be used for correlating the 
	 * call object in active and standby. Also, this ID will be used for
	 * maintaining association between various Call Objects for services like
	 * Call Transfer.
	 */
	Sdf_ty_u32bit					dCallObjId;

	/* enum to indicate indicate whether this is a new call object or it is a
	 * permanent/temporary Call Object.
	 * Initiallly a callobject is new and change to permanent only when the 1st
	 * request is formed or when Sdf_ivk_uaHandleCall is called.
	 */
	Sdf_ty_callObjectType		dCallObjectType;

	/* The common information for this call object */
	Sdf_st_commonInfo 		*pCommonInfo;

	/* Information related to the main call */
	Sdf_st_callInfo	 		*pCallInfo;

	/* List of overlap transactions (RPR/PRACK/COMET) are maintained here */
	Sdf_ty_slist 		slOverlapTransInfo;

	/*
	 * Transaction structure to handle outgoing requests (UAC role)
	 */
	Sdf_st_transaction		*pUacTransaction;

	/*
	 * Transaction structure to handle incoming requests (UAS role)
	 */
	Sdf_st_transaction		*pUasTransaction;


#ifdef SDF_THREAD_SAFE
	/*
	 * This is the mutex for synchronizing changes in call state. It is to
	 * protect the call state variable from concurrent state updates 
	 */
	Sdf_ty_mutexIdT			dCallStateMutex;

	/* 
	 * Mutex to protect the Call Object structure.
	 */
	Sdf_ty_mutexIdT 		dCallObjectMutex;
	
#endif

	/* 
	 * List of headers maintained by application of type 
	 * Sdf_st_headerList. The application can maintain any headers
	 * that it needs for future transactions or otherwise. This list
	 * will be used by the toolkit only to maintain the Route set.
	 */
	Sdf_ty_slist 			slPersistentHeaders;

	/* 
	 * List of message bodies maintained by the application 
	 * of type Sdf_st_msgBody. The application can maintain any message
	 * bodies that it needs for future transactions or otherwise. This
	 * list will be used by the toolkit only to maintain the latest SDP
	 * proposed, received and negotiated during the call.
	 */ 
	Sdf_ty_slist 			slPersistentMsgBodyList;

	/* 
	 * Application specific data (context). This will be passed to the 
	 * application in all callbacks including the stack callbacks.
	 */
	Sdf_st_appData			*pAppData;

	/* 
	 * User profile information for this call object.
	 */
	Sdf_st_initData			*pInitData;

#ifdef SDF_SESSION_TIMER
	Sdf_st_sessionTimerInfo	*pSessionTimer;
#endif	

#ifdef SDF_HA
	Sdf_st_callObjectHAInfo	*pHaInfo;
#endif

#ifdef SDF_LOOKUP
        /* State Machine is not used now. It will used in 
           future release */
	Sdf_st_CallSendstate			dCallSendState;
#endif
	/* 
	 * Media Information of this call
	 */
	 Sdf_st_mediaHandling *pMediaInfo;

#ifdef SDF_SUBSCRIPTION_LAYER
  /* Stores the count of the usages of this CallObject i.e. if this CallObject
   * is used in a call & a subscription, it have a value of 2.
   * Only when this count becomes 0, should the CallObject be removed from the
   * aggregation.
   */
  Sdf_ty_u32bit     dDialogUsageCount;

  /* Stores the list of Subscription Objects corresponding to this dialog
   */ 
  Sdf_ty_slist 			slSubsObjList;
#endif
#ifdef SDF_PRIVACY
    /*
    * The bool is to check if the initiated call is a "privacy-call"
    * If it is false then it is not a privacy-call else if it set to true
    * then the call is a privacy-call and the application has to use 
    * anonymous/default from header.
    */
    Sdf_ty_bool  bIsPrivacy;   
#endif
  
#ifdef SDF_DEP
  /* Stores the Dialog Event Package details in the call-object
   * The life of this detail is the life of the call-object and this
   * info gets created whenever there is a subscription to this call.
   */ 
  Sdf_st_depDetails     *pDepDetails;
#endif

	/* Reference counting for the CallObject */	
	Sdf_ty_refCount			dRefCount;

} Sdf_st_callObject;

#ifdef SDF_SESSION_TIMER
typedef struct
{
	Sdf_st_callObject 	*pCallObject;
	/* 
	 * Placeholder for storing pEventContext. This is required 
	 * while sending target refresh request.
	 */
	Sdf_ty_pvoid		pData;
	Sdf_ty_refCount		dRefCount;

	/* any other relevant info for future use*/
	
} Sdf_st_sessionTimerContextInfo;
#endif



/*
 * Internal structure to map payload types to their codec names
 */
typedef struct
{
	Sdf_ty_s8bit 		dCodecString[20];
	Sdf_ty_codecName	dCodecName;
} Sdf_st_avtProfileMap;


/*
 * Internal structures to map status codes to their reason phrases
 */
typedef struct
{
	Sdf_ty_u16bit dStatusCode;
	Sdf_ty_s8bit dReason[40];

} Sdf_st_statusCodeMapping;

/*
 * Internal structure to map an incoming SIP message to the
 * next state and default message to be sent out
 */
typedef struct
{
	Sdf_ty_state dNextState;
    Sdf_ty_s8bit dMsgToBeSent[6];
} Sdf_st_nextCallState;




typedef struct
{
	Sdf_ty_s8bit		*pMethod;
	en_SipMessageType	dReqRespType;
	Sdf_ty_messageType 	dMsgType;
	Sdf_ty_u16bit		dResponse;
  Sdf_ty_u16bit		dummy;
}Sdf_st_msgInfo;

/*
 * The UA Toolkit event context structure. Similar to the stack's
 * SipEventContext structure with additions for the UA Toolkit.
 */
typedef struct
{
	/* SipEventContext members */

	/* 
	 * SipHeader type list - used to pass list of headers with the selective 
	 * parsing option to sip_decodeMessage
	 */
	SipHdrTypeList	*pList;
	SipTranspAddr	*pTranspAddr;
	SIP_S8bit		*pDirectBuffer;
	SIP_Pvoid 		pData;
	SIP_U32bit		dRemainingLength;
	SIP_U32bit		dNextMessageLength;
	SIP_U32bit		dBuffLength;
	SipOptions      dOptions;
#ifndef SDF_TXN	
	SIP_U32bit 		dRetransT1;
	SIP_U32bit 		dRetransT2;
	SIP_U16bit 		dMaxRetransCount;
	SIP_U16bit 		dMaxInviteRetransCount; 
#endif	

#ifdef SDF_SESSION_TIMER
	Sdf_ty_bool		dReinviteTimeout;
#endif	

	/* Additional members defined for UATK */

	/* 
	 * The transport protocol (UDP/TCP/..) on which the incoming
	 * SIP message arrived.
	 */
	Sdf_ty_protocol		dProtocol;

	/*
	 * Index of the preallocated buffer that is used for sending the
	 * message
	 */
	Sdf_ty_u32bit		dPreallocMsgBufIndex;

	/*Variable to skip HandleRequire and IsSupported checks*/
	Sdf_ty_u32bit		dSdfDisabledChecks;

#ifdef SDF_TXN
 	/*
 	 * The structure for configuring timeouts when transaction layer is 
 	 * enabled.
 	 */
 	SipTimeOutValues	dTimeoutValues;
	/*
	 * If the user has specified his own timer values for
 	 * particular timers then the bitmask of those timers is passed
 	 * in this member.
 	 * The following are the masks that has to be used for the corresponding Timers
	 *
	 * dT1			- SIP_OPT_TIMER_T1
	 * dT2			- SIP_OPT_TIMER_T2
	 * dTimerB		- SIP_OPT_TIMER_B
	 * dTimerC		- SIP_OPT_TIMER_C
	 * dTimerCr 	- SIP_OPT_TIMER_Cr
	 * dTimerD_T3   - SIP_OPT_TIMER_D
	 * dTimerF_T3   - SIP_OPT_TIMER_F
	 * dTimerH 	    - SIP_OPT_TIMER_H
	 * dTimerI_T4   - SIP_OPT_TIMER_I
	 * dTimerJ_T3 	- SIP_OPT_TIMER_J
	 * dTimerK_T4   - SIP_OPT_TIMER_K
 	 */
	Sdf_ty_u32bit	dTimerOption;
	/* The application might not need the timeout cbks of
	 * all timers. The application can indicate the timers 
	 * it has interest in using this parameter.
	 * The following are the masks that has to be used for the corresponding Timer CallBacks
	 *
	 * dTimerB		- SIP_ISSUE_CBKFOR_TIMER_B	
	 * dTimerC 		- SIP_ISSUE_CBKFOR_TIMER_C	
	 * dTimerD_T3	- SIP_ISSUE_CBKFOR_TIMER_D
	 * dTimerF_T3 	- SIP_ISSUE_CBKFOR_TIMER_F
	 * dTimerH  	- SIP_ISSUE_CBKFOR_TIMER_H
	 * dTimerD_T3	- SIP_ISSUE_CBKFOR_TIMER_I
	 * dTimerJ_T3	- SIP_ISSUE_CBKFOR_TIMER_J
	 * dTimerK_T4	- SIP_ISSUE_CBKFOR_TIMER_K
	 * This is also the default 	bitmask applied by the TxnLayer
	 * for all timers -  SIP_ISSUE_CBKFOR_ALLTIMERS
	 */
	Sdf_ty_u32bit	dTimeoutCbkOption;
	/*
	 * The duration for which the Toolkit should start a timer waiting 
	 * for a final response. This value (if provided) over-rides the
	 * default value for this transaction alone.
	 */
	Sdf_ty_u32bit		dNoAnswerTimerDuration;
#endif
#ifdef SDF_SUBSCRIPTION_LAYER
   Sdf_st_uaSLSubscriptionObject   *pSubsObj; 

  /* This bool is set by the Notifier to indicate if the Subscription refresh
	 * timer should be restarted when an 'expires' parameter is present in the
	 * NOTIFY message being sent.
	 */
   Sdf_ty_bool                     dIsRestartTmr;
#endif    
	
	/* Reference count */
	Sdf_ty_refCount 	dRefCount;

} Sdf_st_eventContext;


#ifdef SDF_TXN

typedef struct Sdf_tg_noAnswerTimerContext
{
	/* Call object for which the timer was started */
	Sdf_st_callObject	*pRefCallObject;

	/* Event Context that was passed into sdf_ivk_uaUpdateCallDetails() */
	Sdf_st_eventContext	*pEventContext;

	/* Reference count */
	Sdf_ty_refCount		dRefCount;

} Sdf_st_noAnswerTimerContext;


typedef struct Sdf_tg_txnContextData
{
	/* Application's data given in sdf_ivk_uaSendCallToPeer() */
	Sdf_ty_pvoid		pApplicationData;

	/* The call object corresponding to the transaction */
	Sdf_st_callObject	*pRefCallObject;

	/* Enum indicating the transaction type: UAC/UAS/overlapping */
	Sdf_ty_transactionType	dType;

	/* To be used in conjunction with the enum */
	Sdf_ty_pvoid		pTransaction;

	/* Event Context that was passed into sdf_ivk_uaSendCallToPeer() */
	Sdf_st_eventContext	*pEventContext;

	/* Stack function to handle any timer expiry for this transaction */
	TimeoutFuncPtr		pTimeOutFuncPtr;

	/* Reference count */
	Sdf_ty_refCount		dRefCount;

#ifdef SDF_SUBSCRIPTION_LAYER
	/* Subscription Object corresponding to the transaction */
	Sdf_st_uaSLSubscriptionObject *pSubsObj;
#endif

} Sdf_st_txnContextData;

#ifdef SDF_LOOKUP
typedef struct sdf_tg_dnsEventContext {
	Sdf_st_transaction		        *pTransaction;	
	Sdf_st_callObject 	 	        *pCallObj;
	Sdf_st_overlapTransInfo		    *pOverlappedTransaction;
  	Sdf_ty_slist                  	*pslRecordList;
	Sdf_ty_DnsRecordType          	dDnsRecordType;
	Sdf_ty_UrlType					dUrlType;
  	Sdf_ty_protocol  		 		dProtocol; 
	Sdf_st_eventContext          	*pEventContext;
/* Application specific context can be provided in the following
	container */
	Sdf_ty_pvoid					pAppData;
	Sdf_ty_refCount 				dRefCount;	
} Sdf_st_dnsEventContext;

typedef struct Sdf_tg_dnsInitParams {
	/* Filling the destinfo from unknown DNS record and 
     * delete the corresponding record from the list 
	 */
	Sdf_ty_retVal (*pFillDestInfoFromUnknownDnsRec)	(
		Sdf_st_callObject      	*pCallObj,
		Sdf_st_overlapTransInfo *pOverlapTransaction,
		Sdf_ty_slist	   		*pslDnsRecords, 
		Sdf_st_transportInfo	*pDestInfo,
		Sdf_st_error            *pErr);	
	
	/*
	 * Free the unknonwnDnslist thruough this registered
     * callback 
	 */
	Sdf_ty_retVal (*pFreeUnknownDnsList ) (
		Sdf_st_DnsRecord	*pDnsRecord);

} Sdf_st_dnsModuleInitParams;

#endif
/*
 * Initialization parameters for the transaction layer functionality
 * provided in the SIP Stack/Toolkit.
 */
typedef struct Sdf_tg_txnModuleInitParams
{
	/* 
	 * IMPORTANT
	 * ---------
	 * If the app desires UA Toolkit to handle the functionality, it must
	 * pass NULL for these 2 function pointers.
	 */

	/*
	 * Function to fetch (and optionally add) a transaction structure.
	 */
	Sdf_ty_retVal (*pTxnFetchFunc)(Sdf_st_callObject *pCallObj, \
		SipTxnKey *pTxnKey, Sdf_ty_pvoid *ppTxnBuffer, Sdf_ty_s8bit dOpt, \
		Sdf_ty_pvoid pApplicationContext, Sdf_st_error *pErr);

	/*
	 * Function to release (and optionally remove) a transaction structure.
	 */
	Sdf_ty_retVal (*pTxnReleaseFunc) (Sdf_st_callObject	\
		*pCallObj, SipTxnKey *pInSipTxnKey, Sdf_ty_pvoid *ppOutSipTxnKey, \
		Sdf_ty_pvoid *ppTxnBuffer, Sdf_ty_s8bit dOpt, \
		Sdf_ty_pvoid pApplicationContext, Sdf_st_error *pError);

} Sdf_st_txnModuleInitParams;

#ifdef SDF_SUBSCRIPTION_LAYER
typedef struct Sdf_tg_uaSLInitParams {
	/*
	 * Function to allocate and initialize a Subscription Object
	 */
  Sdf_ty_retVal (*fpSLInitSubsObjectFunc) (
           Sdf_st_uaSLSubscriptionObject  **ppSubsObj,
           Sdf_st_eventContext            *pEventContext,
           Sdf_st_error                   *pErr);

	/*
	 * Function to free the memory allocated to the Subscription Object
	 */
  Sdf_ty_retVal (*fpSLFreeSubsObjectFunc) (
           Sdf_st_uaSLSubscriptionObject  *pSubsObj);

} Sdf_st_uaSLModuleInitParams;

#endif /* End of SDF_SUBSCRIPTION_LAYER */
#ifdef SDF_DEP
typedef struct Sdf_tg_uaDepInitParams {
	/*Subscriber call-backs*/

    /* This call-back is used to indicate the application
     * the receipt of New Subscribe Response.
     */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepNewSubscribeRespRecvd)(
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_ty_uaSLCbkEvent             dCbkEvent,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of Refresh Subscribe Response.
     */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepRefreshSubscribeRespRecvd)(
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_ty_uaSLCbkEvent             dCbkEvent,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of Notify(Active/Pending) Request.
     */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepNotifyRecvd)(
    Sdf_st_callObject                **ppCallObj,
    Sdf_st_uaSLSubscriptionObject    *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext              *pEventContext,
    Sdf_ty_uaSLCbkEvent             dCbkEvent,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of Forked Notify Request.
     */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepForkedNotifyRecvd)(
    Sdf_st_callObject               **ppForkCallObj,
    Sdf_st_callObject               **ppOrgCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of Forked 2XX Response.
     */
  
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepForkedSubs2XXRecvd)(
    Sdf_st_callObject               **ppForkCallObj,
    Sdf_st_callObject               **ppOrgCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of response to an UnSubscibe Request.
     */
  
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepUnSubscribeRespRecvd)(
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_ty_uaSLCbkEvent             dCbkEvent,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of Notify(State:Terminated) Request.
     */

  Sdf_ty_retVal (*fp_cbk_subcr_uaDepSubscriptionTerminated)(
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * that the notifier end is free and a call can be initited
     * to him(CCBS).
     */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepStateRequestNotifyRecvd)(
    Sdf_st_callObject                **ppCallObj, 
    Sdf_st_uaSLSubscriptionObject    *pSubsObj,
    Sdf_st_overlapTransInfo          *pOvlpTxnInfo, 
    Sdf_st_eventContext              *pEventContext, 
    Sdf_ty_uaSLCbkEvent              dCbkEvt, 
    Sdf_st_error                     *pErr );

	/* This callback is used to indicate to the application
	 * that the NOTIFY recvd has an unsupported media type */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepUnSupportedMediaNotifyReceived)(
	Sdf_st_callObject                **ppCallObj, 
    Sdf_st_uaSLSubscriptionObject    *pSubsObj,
    Sdf_st_overlapTransInfo          *pOvlpTxnInfo, 
    Sdf_st_eventContext              *pEventContext, 
    Sdf_st_error                     *pErr );

	/* This callback will be thrown to indicate to the application
	 * that the NOTIFY received does not have a message body */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepEmptyNotifyBodyRecvd)(
	Sdf_st_callObject                **ppCallObj, 
    Sdf_st_uaSLSubscriptionObject    *pSubsObj,
    Sdf_st_overlapTransInfo          *pOvlpTxnInfo, 
    Sdf_st_eventContext              *pEventContext, 
    Sdf_st_error                     *pErr );	  

  /* This callback is is used to indicate to the application
   * that the parsing of NOTIFY body has failed */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepParseNotifyFailed)(
	Sdf_st_callObject                **ppCallObj, 
    Sdf_st_uaSLSubscriptionObject    *pSubsObj,
    Sdf_st_overlapTransInfo          *pOvlpTxnInfo, 
    Sdf_st_eventContext              *pEventContext, 
    Sdf_st_error                     *pErr );

	/*End of Subscriber Related Call-Backs*/

  /*Notifier End call-backs*/

  /* This call-back is used to indicate the application 
   * the receipt of Notify Response
   */
  
  Sdf_ty_retVal (*fp_cbk_ntfr_uaDepNotifyRespRecvd)(
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_ty_uaSLCbkEvent             dCbkEvent,
    Sdf_st_error                    *pErr);

  /* This call-back is used to indicate the application 
   * the receipt of Polling Subscription.
   */

  Sdf_ty_retVal (*fp_cbk_ntfr_uaDepPollSubscribeRecvd)( 
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj, 
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext, 
		Sdf_st_DepCbkRetVal             *pDepCbkRetVal,
    Sdf_st_error                    *pErr);

  /* This call-back is used to indicate the application 
   * the receipt of Polling Subscription.
   */

  Sdf_ty_retVal (*fp_cbk_ntfr_uaDepNewSubscribeRecvd)( 
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj, 
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext, 
		Sdf_st_DepCbkRetVal             *pDepCbkRetVal,
    Sdf_st_error                    *pErr);

  /* This call-back is used to indicate the application 
   * the receipt of Un-Subscription.
   */

  Sdf_ty_retVal (*fp_cbk_ntfr_uaDepUnSubscribeRecvd)( 
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj, 
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext, 
		Sdf_st_DepCbkRetVal             *pDepCbkRetVal,
    Sdf_st_error                    *pErr);
  
  /* This call-back is used to indicate the application 
   * the receipt of Refresh-Subscription.
   */

Sdf_ty_retVal (*fp_cbk_ntfr_uaDepRefreshSubscribeRecvd)( 
     Sdf_st_callObject               **ppCallObj,
     Sdf_st_uaSLSubscriptionObject   *pSubsObj, 
     Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
     Sdf_st_eventContext             *pEventContext, 
		 Sdf_st_DepCbkRetVal             *pDepCbkRetVal,
     Sdf_st_error                    *pErr);

/*This callback is issued after the expiry of SubsWaitingTimer.*/
 Sdf_ty_retVal (*fp_cbk_ntfr_uaDepSubsWaitingTimerExpiry)(
    Sdf_st_callObject                *pCallObj,
    Sdf_st_uaSLSubscriptionObject    *pSubsObj, 
    Sdf_ty_pvoid                     pData,
    Sdf_st_error                     *pErr);

/* This callback is issued on expiry of the Subscription Refresh
 * timer.
 */ 
 Sdf_ty_retVal (*fp_cbk_uaDepSubsRefreshTimerExpiry)(
    Sdf_st_callObject                *pCallObj,
    Sdf_st_uaSLSubscriptionObject    *pSubsObj, 
    Sdf_ty_pvoid                     pData,
    Sdf_st_error                     *pErr) ;

/*This callback is issued on the expiry of the 
 * SubsRemovalTimer.
 */ 
 Sdf_ty_retVal (*fp_cbk_subcr_uaDepSubsRemovalTimerExpiry)(
    Sdf_st_callObject                *pCallObj,
    Sdf_st_uaSLSubscriptionObject    *pSubsObj, 
    Sdf_ty_pvoid                     pData,
    Sdf_st_error                     *pErr);

/* This callback is issued when a SUBSCRIBE/NOTIFY client
 * transaction expires.
 */ 
Sdf_ty_retVal (*fp_cbk_uaDepTxnTimeoutRecvd)(
    Sdf_st_callObject                *pCallObj,
    Sdf_st_uaSLSubscriptionObject    *pSubsObj, 
    Sdf_st_overlapTransInfo          *pOvlpTxnInfo, 
    Sdf_ty_txnTimerType              dTimerType,
    Sdf_ty_pvoid                     pData,
    Sdf_ty_uaSLCbkEvent              dCbkEvent,
    Sdf_st_error                     *pError);


} Sdf_st_uaDepModuleInitParams;
#endif
#endif /* End of SDF_TXN */

/* Trace levels */
typedef enum
{
	Sdf_en_briefTraces = 0x0001,
	Sdf_en_detailedTraces = 0x0002,
	Sdf_en_allTraceLevels = 0x0001 | 0x0002 

}Sdf_ty_uaTraceLevel;

/* Trace types */
typedef enum
{
	Sdf_en_noTraces = 0x0000,
	Sdf_en_initTraces = 0x0001,
	Sdf_en_mesgTraces = 0x0002,
	Sdf_en_configTraces = 0x0004,
	Sdf_en_cbksTraces = 0x0008,
	Sdf_en_authTraces = 0x0010,
	Sdf_en_mediaTraces = 0x0020,
	Sdf_en_callStateTraces = 0x0040,
	Sdf_en_unknownHeaderTraces = 0x0080,
	Sdf_en_remoteRetransTraces =0x0100,
	Sdf_en_haTraces = 0x0200,
	Sdf_en_uaAllTraces = 0x0000|0x0001|0x0002|0x0004|0x0008|0x0010|0x0020 \
	|0x0040|0x0080|0x0100|0x0200

}Sdf_ty_traceType;

/*
 * The function to implement the user defined trace function.
 */
typedef void (*Sdf_ty_uaTraceFunc) (Sdf_ty_uaTraceLevel dLevel, \
		Sdf_ty_traceType dType,	const Sdf_ty_s8bit *pStr, \
		Sdf_ty_u32bit dLen,	Sdf_st_error *pErr);

/*
 * The function to implement the user defined debug function.
 */
typedef void (*Sdf_ty_uaDebugFunc) (Sdf_ty_s8bit *str);


/*
 * UA Toolkit initialization paramaters - to be passed to 
 * sdf_ivk_uaInitToolkit() 
 */
typedef struct
{
#ifdef SDF_USE_PREALLOCBUFFERS
	Sdf_ty_u32bit	dNumPreAllocMsgBuffers;
	Sdf_ty_u16bit	dPreAllocBufSize; 
#endif
#ifdef SDF_HA
	Sdf_st_haModuleInitParams	dHaParams;
#endif
#ifdef SDF_TXN
	Sdf_st_txnModuleInitParams	dTxnParams;
#endif

#ifdef SDF_SUBSCRIPTION_LAYER
	Sdf_st_uaSLModuleInitParams dSLParams;
#endif
#ifdef SDF_DEP
	Sdf_st_uaDepModuleInitParams dDepParams;
#endif
#ifdef SDF_LOOKUP
	Sdf_st_dnsModuleInitParams dDnsParams;
#endif
	/* This variable False means only Tag checking will be done for
	 * Dialog identification. 
	 * True means Entire Address Spec is compared for Dialog matching.
	 * Overlap Dialing support is provided only when this variable is True. */

	Sdf_ty_bool		dStrictURICheck;

#ifdef SDF_TLS
	Sdf_ty_s8bit    *pEntropySource;
#endif
	/*
	 * Function to print the trace (optional to be given by the user)
	 */
	Sdf_ty_uaTraceFunc pTraceFunc;

	/* 
	 * Function to print the debug level (optional to be given by the user)
	 */
	Sdf_ty_uaDebugFunc pDebugFunc; 

} Sdf_ty_initToolkitParams;



/* 
 * Internal data structure to maintain remote retransmission.
 */
typedef struct
{
#ifdef SDF_USE_PREALLOCBUFFERS
	/* Event Context with which the message was sent out - This contains a
	 * preallocated formed sip message buffer. we shall use this buffer 
	 * during retransmission instead of forming and storing a new one
	 * for this purpose
	 * */
	Sdf_st_eventContext		*pContext;
#else
 	/* Message that was sent out */
	Sdf_ty_s8bit			*pBuffer;
#endif

  	/* Length of the buffer message */
	Sdf_ty_u32bit			dLength;

	/* Key associated with the element*/
	Sdf_st_remoteRetransKey	*pKey;

	/* Time stamp indicating when it was first sent out */
	Sdf_ty_u32bit 			dTimeStamp;

	/* The destination transport address */
	Sdf_st_transportInfo 	*pDestInfo;

	/* Application data related to this call */
	Sdf_st_appData			*pAppData;

	/* The sockfd (used only in case of TCP) */
	Sdf_ty_socketId 		dSockfd;

	Sdf_ty_refCount			dRefCount;

} Sdf_st_remoteRetransBuffer;

/*
 * Structure containing common authentication parameters that
 * are required both at the UAC and UAS end.
 */
#ifdef SDF_AUTHENTICATE
typedef struct
{
	/* 
	 * The scheme for authentication...Eg. Basic/Digest/PGP
	 */
	Sdf_ty_s8bit *pScheme;

	/* 
	 * The Realm for authentication
	 */
	Sdf_ty_s8bit *pRealm; 

	/* 
	 * The domain for authentication
	 */
	Sdf_ty_s8bit *pDomain;

	/* 
	 * The opaque value, if any
	 */
	Sdf_ty_s8bit *pOpaque;
	
	/* 
	 * "auth" or "auth-int" if quality of protection is desired
	 */
	Sdf_ty_s8bit *pQop;

	/* 
	 * "MD5" or "MD5-sess"
	 */
	Sdf_ty_s8bit *pAlgorithm;

	/* 
	 * The nonce value
	 */
	Sdf_ty_s8bit *pNonce;

	/* 
	 * The stale value, if any
	 */
	Sdf_ty_s8bit *pStale;

	/*
	 * The type of header (Www-Authenticate/Proxy-Authenticate)
	 * in which this authentication challenge was received.
	 * Used at the UAC end to decide whether a WWW-Authorization/
	 * Proxy-Authorization is to be formed.
	 */
	en_HeaderType dHdrType;

	/*
	 * Reference count
	 */
	Sdf_ty_refCount			dRefCount;

} Sdf_st_authenticationParams;

/*
 * Structure containing elements required for the UAC end
 * of authentication
 */
typedef struct
{
	/* 
	 * Username
	 */
	Sdf_ty_s8bit *pUserName;

	/* 
	 * Password for authentication
	 */
	Sdf_ty_s8bit *pPassword;

	/* 
	 * "auth" or "auth-int" if quality of protection is desired
	 */
	Sdf_ty_s8bit *pQop;

	/* 
	 * The Cnonce value if Qop is desired
	 */
	Sdf_ty_s8bit *pCnonce;

	/* 
	 * The nonce-count if Qop is desired
	 */
	Sdf_ty_s8bit *pNonceCount;

	/*
	 * Reference count
	 */
	Sdf_ty_refCount			dRefCount;

} Sdf_st_uacAuthInfo;

/*
 * Structure containing elements required for the UAS end
 * of authentication
 */
typedef struct
{
	/* 
	 * "Basic" or "Digest"
	 */
	Sdf_ty_s8bit *pScheme;

	/*
	 * Username
	 */
	Sdf_ty_s8bit *pUserName;
	
	/* 
	 * Used only in case of Digest scheme
	 */
	Sdf_ty_s8bit *pRealm; 

	/* 
	 * "MD5" or "MD5-sess". Required to computer H(A1).
	 */
	Sdf_ty_s8bit *pAlgorithm;

	/* 
	 * The nonce value received. Required to computer H(A1) only in case 
	 * of MD5-sess algorithm
	 */
	Sdf_ty_s8bit *pNonce;

	/* 
	 * The Cnonce value received. Required to computer H(A1) only in case 
	 * of MD5-sess algorithm
	 */
	Sdf_ty_s8bit *pCnonce;

	/* 
	 * Clear-text password in case of Basic and
	 * H(A1) in case of Digest authentication
	 */
	Sdf_ty_s8bit *pSecret;

	/*
	 * Reference count
	 */
	Sdf_ty_refCount			dRefCount;

} Sdf_st_uasAuthInfo;
#endif /* SDF_AUTHENTICATE */
/*
 * Configuration related extra headers. Used to store information
 * like the Supported/Require/Allow that are supported by a user profile.
 */
typedef struct
{
	SipParam *pParam;

	/* For extensibility, currently used as a flag */
	Sdf_ty_pvoid pData;

}Sdf_st_configExtraHeaders;

/* Type of statistics*/
typedef enum
{
	Sdf_en_call

}Sdf_ty_statsType;

/* Structure for call statistics */
typedef struct
{
 	/* Number of calls received */
	Sdf_ty_u32bit dCallsReceived;

	/* Number of calls received and established */
	Sdf_ty_u32bit dRemoteCallsEstd; 

	/* Number of calls received and rejected*/
	Sdf_ty_u32bit dRemoteCallsRejd;

	/* Number of calls initiated */
	Sdf_ty_u32bit dCallsInitiated;

	/* Number of calls initiated and established */
	Sdf_ty_u32bit dCallsEstd; 		

	/* Number of calls initiated and were rejected */
	Sdf_ty_u32bit dCallsRejd; 		

	/* Number of calls terminated */
	Sdf_ty_u32bit dCallsTerminated; 

	/* Number of transactions sent */
	Sdf_ty_u32bit dTransactionsSent;

	/* Number of transactions received */
	Sdf_ty_u32bit dTransactionsRecv; 

}Sdf_st_callStats;





/* Error levels */
typedef enum
{
	Sdf_en_noErrors = 0x00,
	Sdf_en_minorErrors = 0x01,
	Sdf_en_majorErrors = 0x02,
	Sdf_en_criticalErrors = 0x04,
	Sdf_en_allErrors = 0x01 | 0x02 | 0x04

}Sdf_ty_uaErrorLevel;


typedef struct
{
	/* Sip Message Body */
	SipMsgBody *pMsgBody;

	/* 
	 * Correlation string for the application to associate a 
	 * message body to a string 
	 */
	Sdf_ty_s8bit 		*pCorrelation;

	/* Reference count */
	Sdf_ty_refCount 		dRefCount;

} Sdf_st_msgBody;


/*
 * The toolkit's internal data structure
 */
typedef struct
{
#ifndef SDF_APP_CONTROLLED_REMOTE_RETX
	/* Remote retransmission hash */
	Sdf_st_hash				dRemoteRetransHash;
#endif

	/* 
	 * Enum indicating whether transaction stateful
	 * behaviour is to be applied when no matching
	 * call object is found
	 */
	Sdf_ty_stateMaintenance	dStateMaintenance;

#ifdef SDF_SUBSCRIPTION_LAYER
	/* Hash to hold Watcher Subscription Objects */
	 Sdf_st_hash dSubscriptionHash;

	/*
	 * Function to allocate and initialize a Subscription Object
	 */
  Sdf_ty_retVal (*fpSLInitSubsObjectFunc) (
           Sdf_st_uaSLSubscriptionObject  **ppSubsObj,
           Sdf_st_eventContext            *pEventContext,
           Sdf_st_error                   *pErr);

	/*
	 * Function to free the memory allocated to the Subscription Object
	 */
  Sdf_ty_retVal (*fpSLFreeSubsObjectFunc) (
           Sdf_st_uaSLSubscriptionObject  *pSubsObj);


#endif /* End of SDF_SUBSCRIPTION_LAYER */
#ifdef SDF_LOOKUP
	/* Filling the destinfo from unknown DNS record and 
     * delete the corresponding record from the list 
	 */
	Sdf_ty_retVal (*pFillDestInfoFromUnknownDnsRec)	(
		Sdf_st_callObject      	*pCallObj,
		Sdf_st_overlapTransInfo *pOverlapTransaction,
		Sdf_ty_slist	   		*pslDnsRecords, 
		Sdf_st_transportInfo	*pDestInfo,
		Sdf_st_error            *pErr);	
	
	/*
	 * Free the unknonwnDnslist thruough this registered
     * callback 
	 */
	Sdf_ty_retVal (*pFreeUnknownDnsList ) (
		Sdf_st_DnsRecord	*pDnsRecord);
#endif


#ifdef SDF_TXN
	/*
	 * Functions to fetch (and optionally add) or release (and optionally
	 * remove) a transaction structure.
	 *
	 * These will be set to Sdf_co_null by default. Only in a case that
	 * the application intends to provide explicit implementation of the 
	 * aggregation for the SIP Stack transaction structures, he can
	 * over-ride the UA Toolkit handling. 
	 *
	 * In such a case, these functions to fetch and release the transactions 
	 * are registered with the UA Toolkit at the initialization time. We 
	 * will use these instead of the default handling that we provide.
	 */
	Sdf_ty_retVal (*pTxnFetchFunc)(Sdf_st_callObject *pCallObj, \
		SipTxnKey *pTxnKey, Sdf_ty_pvoid *ppTxnBuffer, Sdf_ty_s8bit dOpt, \
		Sdf_ty_pvoid pApplicationContext, Sdf_st_error *pErr);

	Sdf_ty_retVal (*pTxnReleaseFunc) (Sdf_st_callObject	\
		*pCallObj, SipTxnKey *pInSipTxnKey, Sdf_ty_pvoid *ppOutSipTxnKey, \
		Sdf_ty_pvoid *ppTxnBuffer, Sdf_ty_s8bit dOpt, \
		Sdf_ty_pvoid pApplicationContext, Sdf_st_error *pError);

	/*
	 * Hash table to store the transaction structures. This is used in the
	 * UA Toolkit handling of transaction layer aggregation.
	 */
	Sdf_st_hash		dSipTxnHash;

	/*
	 * All transactions started by the application will wait for a 
	 * final response (after getting a provisional response) for the
	 * following duration. This value is in milli-seconds. 
	 * 
	 * The default value of this variable is the macro
	 * Sdf_co_noAnswerTimerDuration present in sdf_common.h
	 * 
	 * If the application gives a timer value in the event-context as a
	 * parameter to sdf_ivk_uaUpdateCallDetails(), that value shall
	 * over-ride this default for that transaction alone.
	 */
	Sdf_ty_u32bit	dNoAnswerTimerDuration;
#endif
#ifdef SDF_DEP
	/*Subscriber call-backs*/

    /* This call-back is used to indicate the application
     * the receipt of New Subscribe Response.
     */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepNewSubscribeRespRecvd)(
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_ty_uaSLCbkEvent             dCbkEvent,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of Refresh Subscribe Response.
     */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepRefreshSubscribeRespRecvd)(
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_ty_uaSLCbkEvent             dCbkEvent,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of Notify(Active/Pending) Request.
     */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepNotifyRecvd)(
    Sdf_st_callObject                **ppCallObj,
    Sdf_st_uaSLSubscriptionObject    *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext              *pEventContext,
    Sdf_ty_uaSLCbkEvent             dCbkEvent,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of Forked Notify Request.
     */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepForkedNotifyRecvd)(
    Sdf_st_callObject               **ppForkCallObj,
    Sdf_st_callObject               **ppOrgCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of Forked 2XX Response.
     */
  
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepForkedSubs2XXRecvd)(
    Sdf_st_callObject               **ppForkCallObj,
    Sdf_st_callObject               **ppOrgCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of response to an UnSubscibe Request.
     */
  
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepUnSubscribeRespRecvd)(
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_ty_uaSLCbkEvent             dCbkEvent,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * the receipt of Notify(State:Terminated) Request.
     */

  Sdf_ty_retVal (*fp_cbk_subcr_uaDepSubscriptionTerminated)(
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_st_error                    *pErr);

    /* This call-back is used to indicate the application
     * that the notifier end is free and a call can be initited
     * to him(CCBS).
     */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepStateRequestNotifyRecvd)(
    Sdf_st_callObject                **ppCallObj, 
    Sdf_st_uaSLSubscriptionObject    *pSubsObj,
    Sdf_st_overlapTransInfo          *pOvlpTxnInfo, 
    Sdf_st_eventContext              *pEventContext, 
    Sdf_ty_uaSLCbkEvent              dCbkEvt, 
    Sdf_st_error                     *pErr );

	/* This callback is used to indicate to the application
	 * that the NOTIFY recvd has an unsupported media type */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepUnSupportedMediaNotifyReceived)(
	Sdf_st_callObject                **ppCallObj, 
    Sdf_st_uaSLSubscriptionObject    *pSubsObj,
    Sdf_st_overlapTransInfo          *pOvlpTxnInfo, 
    Sdf_st_eventContext              *pEventContext, 
    Sdf_st_error                     *pErr );
		
 	/* This callback is used to indicate to the application
	 * that the NOTIFY received does not have a message body */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepEmptyNotifyBodyRecvd)(
	Sdf_st_callObject                **ppCallObj, 
    Sdf_st_uaSLSubscriptionObject    *pSubsObj,
    Sdf_st_overlapTransInfo          *pOvlpTxnInfo, 
    Sdf_st_eventContext              *pEventContext, 
    Sdf_st_error                     *pErr );	
 

  /* This callback is is used to indicate to the application
   * that the parsing of NOTIFY body has failed */
  Sdf_ty_retVal (*fp_cbk_subcr_uaDepParseNotifyFailed)(
	Sdf_st_callObject                **ppCallObj, 
    Sdf_st_uaSLSubscriptionObject    *pSubsObj,
    Sdf_st_overlapTransInfo          *pOvlpTxnInfo, 
    Sdf_st_eventContext              *pEventContext, 
    Sdf_st_error                     *pErr );		  
		  
  /*End of Subscriber Related Call-Backs*/

  /*Notifier End call-backs*/

  /* This call-back is used to indicate the application 
   * the receipt of Notify Response
   */
  
  Sdf_ty_retVal (*fp_cbk_ntfr_uaDepNotifyRespRecvd)(
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj,
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext,
    Sdf_ty_uaSLCbkEvent             dCbkEvent,
    Sdf_st_error                    *pErr);

  /* This call-back is used to indicate the application 
   * the receipt of Polling Subscription.
   */

  Sdf_ty_retVal (*fp_cbk_ntfr_uaDepPollSubscribeRecvd)( 
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj, 
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext, 
		Sdf_st_DepCbkRetVal             *pDepCbkRetVal,
    Sdf_st_error                    *pErr);

  /* This call-back is used to indicate the application 
   * the receipt of Polling Subscription.
   */

  Sdf_ty_retVal (*fp_cbk_ntfr_uaDepNewSubscribeRecvd)( 
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj, 
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext, 
		Sdf_st_DepCbkRetVal             *pDepCbkRetVal,
    Sdf_st_error                    *pErr);

  /* This call-back is used to indicate the application 
   * the receipt of Un-Subscription.
   */

  Sdf_ty_retVal (*fp_cbk_ntfr_uaDepUnSubscribeRecvd)( 
    Sdf_st_callObject               **ppCallObj,
    Sdf_st_uaSLSubscriptionObject   *pSubsObj, 
    Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
    Sdf_st_eventContext             *pEventContext, 
		Sdf_st_DepCbkRetVal             *pDepCbkRetVal,
    Sdf_st_error                    *pErr);
  
  /* This call-back is used to indicate the application 
   * the receipt of Refresh-Subscription.
   */

Sdf_ty_retVal (*fp_cbk_ntfr_uaDepRefreshSubscribeRecvd)( 
     Sdf_st_callObject               **ppCallObj,
     Sdf_st_uaSLSubscriptionObject   *pSubsObj, 
     Sdf_st_overlapTransInfo         *pOvlpTxnInfo,
     Sdf_st_eventContext             *pEventContext, 
		Sdf_st_DepCbkRetVal             *pDepCbkRetVal,
     Sdf_st_error                    *pErr);

 Sdf_ty_retVal (*fp_cbk_ntfr_uaDepSubsWaitingTimerExpiry)(
    Sdf_st_callObject                *pCallObj,
    Sdf_st_uaSLSubscriptionObject    *pSubsObj, 
    Sdf_ty_pvoid                     pData,
    Sdf_st_error                     *pErr);

 /* This callback is issued on expiry of the Subscription Refresh
 * timer.
 */ 
 Sdf_ty_retVal (*fp_cbk_uaDepSubsRefreshTimerExpiry)(
    Sdf_st_callObject                *pCallObj,
    Sdf_st_uaSLSubscriptionObject    *pSubsObj, 
    Sdf_ty_pvoid                     pData,
    Sdf_st_error                     *pErr) ;

/*This callback is issued on the expiry of the 
 * SubsRemovalTimer.
 */ 
 Sdf_ty_retVal (*fp_cbk_subcr_uaDepSubsRemovalTimerExpiry)(
    Sdf_st_callObject                *pCallObj,
    Sdf_st_uaSLSubscriptionObject    *pSubsObj, 
    Sdf_ty_pvoid                     pData,
    Sdf_st_error                     *pErr);

/* This callback is issued when a SUBSCRIBE/NOTIFY client
 * transaction expires.
 */ 
Sdf_ty_retVal (*fp_cbk_uaDepTxnTimeoutRecvd)(
    Sdf_st_callObject                *pCallObj,
    Sdf_st_uaSLSubscriptionObject    *pSubsObj, 
    Sdf_st_overlapTransInfo          *pOvlpTxnInfo, 
    Sdf_ty_txnTimerType              dTimerType,
    Sdf_ty_pvoid                     pData,
    Sdf_ty_uaSLCbkEvent              dCbkEvent,
    Sdf_st_error                     *pError);
     
#endif    
	/* This variable Flase means only Tag checking will be done for
	 * Dialog identification. 
	 * True means Entire Address Spec is compared for Dialog matching.
	 * Overlap Dialing support is provided only when this variable is True. */

	Sdf_ty_bool		dStrictURICheck;

	Sdf_ty_refCount dRefCount;
	/*
	 * Function to print the trace (optional to be given by the user)
	 */
	Sdf_ty_uaTraceFunc pTraceFunc;

	/* 
	 * Function to print the debug level (optional to be given by the user)
	 */
	Sdf_ty_uaDebugFunc pDebugFunc; 
	
} Sdf_st_toolkitData;

/* 
 * The Sdf_st_netSocketOptions structure is for allowing socket properties 
 * to be set using setsockopt call. The members map directly to the 
 * arguments in the function invocation.
 */
typedef struct
{
	Sdf_ty_s32bit	level;
	Sdf_ty_s32bit	optName;
	Sdf_ty_pvoid	pOptVal;
	Sdf_ty_u32bit	optlen;

}Sdf_st_socketOptions;


typedef enum
{
	Sdf_en_sockUnicast = 1,
	Sdf_en_sockMulticast = 2

} Sdf_ty_socketType;


/* 
 * The Sdf_st_netInterface structure contains all data about any new socket 
 * to be opened by the application.
 */
typedef struct
{
	/* TCP or UDP currently */
	Sdf_ty_s8bit		*pProtocol;
	
	/* Unicast or Multicast */
	Sdf_ty_socketType	dType;
	
	/* Port number to bind */
	Sdf_ty_u16bit 			dPort;		
	
	/* Max pending connections on the socket */
	Sdf_ty_u32bit			dMaxPendConns;	
										
	/* List of Sdf_st_socketOptions to be used in setsockopt */
	Sdf_ty_slist			slOptions;

	/* IP to bind to. If NULL use INADDR_ANY */
	Sdf_ty_s8bit			*pIpAddress;	
										
	/* NULL for unicast	*/
	Sdf_ty_s8bit			*pMulticastGroup;

} Sdf_st_socketProperties;

typedef enum
{
	Sdf_en_invalidTimerType = 0,
	Sdf_en_txnLayerTimer,
	Sdf_en_sessionTimer,
	Sdf_en_dnsTimer,
	Sdf_en_haTimer,
	Sdf_en_remoteRetransTimer,
	Sdf_en_noAnswerTimer,
#ifdef SDF_SUBSCRIPTION_LAYER
	Sdf_en_uaSLSubsRefreshTimer, 
	Sdf_en_uaSLSubsWaitingTimer, 
	Sdf_en_uaSLSubsRemovalTimer,
#endif
	Sdf_en_appTimerBase
} Sdf_ty_timerType;

#if 0
#ifdef SDF_TXN
/*
 * This enumeration includes all values used by the stack in its
 * corresponding enumeration en_SipTimerType. It also adds a value
 * for the "no-answer" timer that is started on reception of a 
 * provisional response.
 */
typedef enum
{
	Sdf_en_timerA_B,	/* For Starting TimerA along With TimerB */
	Sdf_en_timerB,
	Sdf_en_timerC,		/* Never used by UATK */
	Sdf_en_timerCr,		/* Never used by UATK */
	Sdf_en_timerD,
	Sdf_en_timerE_F,	/* For Starting TimerE along With TimerF */
	Sdf_en_timerF,
	Sdf_en_timerG_H,	/* For Starting TimerG along With TimerH */
	Sdf_en_timerH,
	Sdf_en_timerI,
	Sdf_en_timerJ,
	Sdf_en_timerK,
	Sdf_en_timerForCancelledState,  
	Sdf_en_timerForProxyUAS2xxFwded,	/* Never used by UATK */
	Sdf_en_lastStackTimer	= SipTimerLast,
	Sdf_en_timerNoAnswer	= (Sdf_en_lastStackTimer + 1)
} Sdf_ty_txnTimerType;
#endif
#endif
typedef struct
{
	Sdf_st_callObject 		*pCallObj;
	Sdf_st_transaction		*pTransaction;
	Sdf_st_overlapTransInfo	*pOverlapTransaction;
	Sdf_ty_transactionType	dTransactionType;
	Sdf_st_initData			*pInitData;
}Sdf_st_validateInArg;

typedef struct
{
	SipMessage	*pSipMsg;
}Sdf_st_validateOutArg;


typedef struct
{
	Sdf_ty_bool dIsTargetUriSet;
	union
	{
		SipAddrSpec           *pAddrSpec;
	    Sdf_st_transportInfo  *pDestInfo;
    }u;
}Sdf_st_destDetails;

typedef struct
{
	Sdf_ty_fdSet		dSockFdSet;
	Sdf_ty_socketId		dMaxFdVal;
#ifdef SDF_THREAD_SAFE
	Sdf_ty_mutexIdT		dFdsetMutex;
#endif
}Sdf_st_fdSetInfo;

#ifdef SDF_HA
/* This structure specifies the input parameters to sdf_ivk_uaResendMessage()
 * API :
 *		pCallObj [IN]		: contains the call object being reconstructed.
 *		pUacUasTxn [IN]		: contains the uac/uas transaction that is being
 *							  reconstructed. If it is a overlap/regular txn that
 *							  is being reconstructed, then this field will be 
 *							  Sdf_co_null
 *		pOverlapTxn [IN]	: contains the overlap/regular transaction that is 
 *							  being reconstructed. If it is a uac/uas txn that
 *							  is being reconstructed, then this field will be 
 *							  Sdf_co_null
 *		dActionPerformed [IN]: specifies the action being performed.
 *		pSipMsg [IN]		: message to be sent.
 *		dOptions [IN]		: options to use while sending the message.
 *		dTranspType [IN]	: transport type to be used.
 *		pContext [IN]		: this context will be given back to the app in all
 *							  future callbacks related to this transaction.
 */
typedef struct
{
	Sdf_st_callObject 		*pCallObj;
	Sdf_st_transaction 		*pUacUasTxn;
	Sdf_st_overlapTransInfo	*pOverlapTxn;
	Sdf_ty_appActionExpected dActionPerformed;
	SipMessage 				*pSipMsg;
	SipOptions 				dOptions;
	Sdf_ty_s8bit 			dTranspType;
	Sdf_st_eventContext 	*pContext;
} Sdf_st_resendMessageArgs;

#endif

/*
 * The function Pointer for Media State Validity
 */
typedef Sdf_ty_retVal (*Sdf_ty_fptrMediaStateValidationFunc)(SipMessage*,Sdf_st_callObject*,Sdf_st_error*);


/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
#endif
