/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************
 ** FUNCTION:
 **	 	This file has all the SIP Txn Related Structures

 ************************************************************************
 **
 ** FILENAME:
 ** siptxnstruct.h
 **
 ** DESCRIPTION:This has structures related to Transaction Layer
 **
 ** DATE		NAME			REFERENCE		REASON
 ** ----		----			--------		------
 ** 12-2-02		Sasidhar P V K 		Initial Version
 **	COPYRIGHT , Aricent, 2006
 *************************************************************************/


#ifndef __SIPTXNSTRUCT_H__
#define __SIPTXNSTRUCT_H__
#include "microsip_struct.h"
/* Used for fast_memget and fast_memfree */
#define TXN_MEM_ID   10

/*For dTimerOption we shall be having the following options*/
#define SIP_OPT_TIMER_T1 		(0x001)
#define SIP_OPT_TIMER_T2		(0x002)
#define SIP_OPT_TIMER_B			(0x004)
#define SIP_OPT_TIMER_C			(0x008)
#define SIP_OPT_TIMER_Cr		(0x010)
#define SIP_OPT_TIMER_D			(0x020)
#define SIP_OPT_TIMER_F			(0x040)
#define SIP_OPT_TIMER_H			(0x080)
#define SIP_OPT_TIMER_I			(0x100)
#define SIP_OPT_TIMER_J			(0x200)
#define SIP_OPT_TIMER_K		   	(0x400)

/*This bitmask will be used to determine which timeOut callbacks is the 
	client interested in receiving??*/
#define SIP_ISSUE_CBKFOR_TIMER_B		(0x001)
#define SIP_ISSUE_CBKFOR_TIMER_C		(0x002)
#define SIP_ISSUE_CBKFOR_TIMER_D		(0x004)
#define SIP_ISSUE_CBKFOR_TIMER_F		(0x008)
#define SIP_ISSUE_CBKFOR_TIMER_H		(0x010)
#define SIP_ISSUE_CBKFOR_TIMER_I		(0x020)
#define SIP_ISSUE_CBKFOR_TIMER_J		(0x040)
#define SIP_ISSUE_CBKFOR_TIMER_K		(0x080)
	
/*This is also the default 	bitmask applied by the TxnLayer*/
#define SIP_ISSUE_CBKFOR_ALLTIMERS		(0x0FF)

/*This macro can be used for specifying
	interest in all timers*/
#define SIP_OPT_TIMERALL		(0xffff)
/*Changes from UATK 2.1  for IMS client Start*/
/*The default values of the timers will be*/
#ifdef IMS_CLIENT
#define SIP_DEFAULT_T1 			2000
#define SIP_DEFAULT_T2 			16000
#define SIP_DEFAULT_B 			128000
#define SIP_DEFAULT_C 			180000
#define SIP_DEFAULT_Cr 			180000
#define SIP_DEFAULT_F_T3		128000
#define SIP_DEFAULT_H 			128000
#define SIP_DEFAULT_I_T4		17000
#define SIP_DEFAULT_K_T4		17000
#define SIP_DEFAULT_J_T3		128000

#elif NON_IMS_CLIENT
#define SIP_DEFAULT_T1 			500
#define SIP_DEFAULT_T2 			4000
#define SIP_DEFAULT_B 			32000
#define SIP_DEFAULT_C 			60000
#define SIP_DEFAULT_Cr 			60000

#define SIP_DEFAULT_F_T3		32000
#define SIP_DEFAULT_H 			32000
#define SIP_DEFAULT_I_T4		5000
#define SIP_DEFAULT_K_T4		5000
#define SIP_DEFAULT_J_T3		32000
#else
#define SIP_DEFAULT_T1 			500
#define SIP_DEFAULT_T2 			4000
#define SIP_DEFAULT_B 			32000
#define SIP_DEFAULT_C 			60000
#define SIP_DEFAULT_Cr 			60000

#define SIP_DEFAULT_F_T3		32000
#define SIP_DEFAULT_H 			32000
#define SIP_DEFAULT_I_T4		5000
#define SIP_DEFAULT_K_T4		5000
#define SIP_DEFAULT_J_T3		32000
#endif


#define SIP_DEFAULT_D_T3		32000
/*Changes from UATK 2.1  for IMS client End*/

#define SIP_TXN_MAX_METHOD_SIZE  100
#define TXNSTATES			21

/*
 * It must be possible for applications to set the timer values once and
 * for these values to become applicable for all of the transactions. For
 * this reason there is a set of global variable provided. In case the
 * application decides to modify the values contained in these then these
 * values are then used for all transactions. However in case for any
 * particular transaction the application decides to modify the timer
 * values by passing these in TxnContext, then the values in the TxnContext
 * override the values in the global variables. These values have been
 * defined in txndecodeintrnl.c and initialised with the default timer
 * values.
 */
extern SIP_U32bit		glbT1;
extern SIP_U32bit		glbT2;
extern SIP_U32bit		glbTimerB;
extern SIP_U32bit		glbTimerC;
extern SIP_U32bit		glbTimerCr;
extern SIP_U32bit		glbTimerD_T3;
extern SIP_U32bit		glbTimerF_T3;
extern SIP_U32bit		glbTimerH;
extern SIP_U32bit		glbTimerI_T4;
extern SIP_U32bit		glbTimerJ_T3;
extern SIP_U32bit		glbTimerK_T4;

/*Purely internal definitions*/

typedef enum
{
	SipTxnMesgSent,
	SipTxnMesgRecvd
}en_SipTxnAPICalled;

typedef enum
{
	SipResponse1xx,
	SipResponseFinal2xx,
	SipResponseFinalNon2xx,
	SipResponseInvalid /* for cases <100 */
}en_SipResponseClass;



typedef enum
{
	SipTxnMethodInvite,
	SipTxnMethodNonInvite,
	SipTxnMethodAck,
	SipTxnMethodPrack
}en_SipMethodType;

/*
Description
This enumeration indicates whether the transaction is a UA transaction or a 
proxy transaction. 
*/	
typedef enum
{
	SipUATypeTxn = 0,
	SipProxyTypeTxn
}en_SipTxnType;


/*
This enumeration indicates whether the transaction is a Invite client,nonInvite client,invite server or a non-invite server type of txn.
*/

typedef enum
{
	SipInviteClient,
	SipNonInviteClient,
	SipInviteServer,
	SipNonInviteServer
}en_SipTxnClass;

/*
This enumeration is returned back by  the sip_txn_decodeMessage() API.
It indicates the result of the transaction handling layer to the application.
If the message received is a retransmission and can be ignored, it is set to
SipIgnorable. If the message received is not matching any transaction object,
it is set to SipStrayMessage. If the message received actually matches the
transaction object and is valid in the current state, it is set to
SipNonIgnorable. 
*/

typedef enum
{
	SipTxnIgnorable,
	SipTxnNonIgnorable,
	SipTxnStrayMessage,
	SipTxnStrayPrack,
	SipTxnConfirmnNeeded,
	SipTxnQueued,
	SipTxnRetrans
}en_SipTxnDecodeResult;


/*
This enumeration is used as a parameter to the sip_sendMessage. 
It indicates whether the message sent needs to fetch a  transaction 
or simply forward it.
*/
				
typedef enum
{
	SipTxnByPass,
	SipTxnNoByPass
}en_SipTxnBypass;

/* This enumeration is used to define various Transaction states in a SIP transaction */
typedef enum
{
	SipTxnInvClientCallingState=0,
	SipTxnInvClientProceedingState,
	SipTxnInvClientFinalResponseRecvState,
	SipTxnInvClientCompletedState,

	SipTxnInvServerProceedingState,
	SipTxnInvServer1xxSentState,	
	SipTxnInvServer2xxForwardedState,
	SipTxnInvServerCompletedState,
	SipTxnInvServerConfirmedState,

	SipTxnNonInvClientTryingState,
	SipTxnNonInvClientProceedingState,
	SipTxnNonInvClientCompletedState,

	SipTxnNonInvServerTryingState,
	SipTxnNonInvServerProceedingState,	
	SipTxnNonInvServerCompletedState,

	SipTxnNonInvClientRPRRecvngState,
	SipTxnNonInvServerRPRSendingState,

	SipTxnNonInvClientRPRCompletedState,
	SipTxnNonInvServerRPRCompletedState,

	SipTxnTerminatedState,

	SipTxnServerBypassState,
	SipTxnDefaultState 
}en_SipTxnState;

/*This structure definition is used to store the default timer values for all the timers defined in the bis-05.*/

typedef struct
{
	SIP_U32bit		dT1;
	SIP_U32bit		dT2;
	SIP_U32bit		dTimerB;
	SIP_U32bit		dTimerC;
	SIP_U32bit		dTimerCr;
	SIP_U32bit		dTimerD_T3;
	SIP_U32bit		dTimerF_T3;
	SIP_U32bit		dTimerH;
	SIP_U32bit		dTimerI_T4;
	SIP_U32bit		dTimerJ_T3;
	SIP_U32bit		dTimerK_T4;
} SipTimeOutValues;



/*
 * Any options that need to be provided to the Txn-Layer and from there on
 * to the core stack need yo be specified inthis structure. The various
 * members are:-
 * 
 * dOption::The usual Stack options like SIP_OPT_CLEN,SIP_OPT_SHORTFORM etc
 * dTimerOption::If the user has specified his own timer values for
 * 				 particular timers then the bitmask of those timers is passed
 * 				 in this member.
 * dTimeoutCbkBitMask::The application might not need the timeout cbks of
 * 					   all timers. The application can indicate the timers 
 * 					   it has interest in using this parameter.
 */
typedef struct
{
	SipOptions	dOption;
	SIP_U32bit	dTimerOption;
	SIP_U32bit	dTimeoutCbkOption;
} SipTxnOption;


/*
The SipTxnContext structure adds on to the SipEventContext structure with
the difference that it has an additional member variable of the type
SipTxnDefaultTimeOutValues that allows the user to specify his own default
values for the various timers. The last variable specifies the kind of txn
that will be created. This value might be needed only when passage of this
information to sip_txn_sendMessage() or sip_txn_decodeMessage() actually
results in a txn being created.
*/

typedef struct
{
	/*A set of configurable values T1,T2,T3 etc */
	SipTimeOutValues timeoutValues;

	/*The value of SipEventContext for SIP_TXN*/
	SipEventContext	*pEventContext;	

	/*The type of txn whether it is of ProxyType or a UA type*/
	en_SipTxnType	dTxnType;		
	
	/*The TxnOptions*/
	SipTxnOption 	txnOption;
} SipTxnContext;

/* This structure definition is used to store the key for a transaction.
	It includes the Call Id, To tag, Method, Via branch, Rseq for RPR 
	and dTimeType indicates the type of the timer used. */

typedef struct
{
	SIP_S8bit* 			pCallid;
	en_SipTxnClass		dTxnClass;
	SIP_U32bit 			dCSeq; 	
	SIP_S8bit*			pMethod;
	SIP_S8bit*			pToTag;
	SIP_S8bit*			pFromTag;
	SIP_S8bit*			pViaBranch;
/*Changes for CSR 1-5930636 starts*/
	/*SIP_U32bit			dViaHdrCount;*/
/*Changes for CSR 1-5930636 end*/
	SIP_U32bit 			dRseq; 	
#ifdef SIP_RPR	
	SipRackHeader*		pRackHdr; 	
#endif	
	en_SipMessageType 	dType;
	SIP_U16bit			dRespCodeNum;
	SipBool				dTagCheck;
	SipBool				dViaBranchCheck;
	SIP_RefCount		dRefCount;
}SipTxnKey;


/*This is a data structure that is passed within the txn layer functions
 it includes the various parameters that would have otherwise needed to be
 sent as separate parameters*/
typedef struct
{
	en_SipMessageType 	dMesgType;/*Request or response*/
	en_SipMethodType  	dMethodType;/*Type of method*/
	en_SipResponseClass dRespClass;/*Class of response 1xx,2xx or other*/
	en_SipTxnClass		dTxnClass;
	SIP_U16bit       	dRespCode;
	
	en_SipTxnAPICalled  dAPICalled;
	SipTxnKey 			*pKeyOfCreatedTxn;
	
	SipMessage 			*pSipMessage;
	SIP_S8bit 			*pMsgBuffer;
	SipTxnContext		*pTxnContext;
	SipTranspAddr 		*pAddr;/*The transport address in case of a message 
									sent*/
	SIP_S8bit 			dTranspType;								
	SIP_U32bit 			dMessageLen;
	SipBool				dMesgSentAtLeastOnce;
	SipBool  			dDirectOption;
	SIP_S8bit 			methodName[SIP_TXN_MAX_METHOD_SIZE];
} SipTxnInformn;


/*The SipTxnBuffer structure definition is the core of the transaction 
	layer implementation. It defines the buffer stored for each transaction 
	maintained. */

typedef struct
{
	/* This is the type of transaction(UA/Proxy) */
	en_SipTxnType		dType;
	
	/* This is the duration of the TimerA/TimerG  which is doubled 
	for each retransmission*/
	SIP_U32bit		dDuration;
	
	/* This is the total duration time from the instance when the
	initial message was sent out */
	SIP_U32bit		dCumulativeDuration;
	
	/* This is the Max duration time for the timer,applicable
	for timers A,E,G */
	SIP_U32bit		dMaxDuration;

	/*The reference to the Txn Key*/
	SipTxnKey 		*pTxnKey;
	
	/* This is the transaction state */
	en_SipTxnState	dTxnState;

	/*This is the class of the Txn*/
	en_SipTxnClass	dTxnClass;	

	/*Code of response 1xx,2xx or other
	this value is essential in determining sitns
	where a client txn receives a final response after
	having had already recvd one, the txn states do not tell
	which class of response was recvd so to distinguish 
	between retransX thsi field becomes essential*/
	SIP_U16bit		 dRespCode;
	
	/* This is the transport type */
	SIP_S8bit 		dTranspType;

	/* This is the transport address */
	SipTranspAddr	*pAddr;
	
	/* This is used in the case of stateless proxy.
	Setting it will bypass the transaction fetch*/
	en_SipTxnBypass dBypassMode;
	
	/* This is the message buffer that needs to be retransmitted*/
	SIP_S8bit		*pBuffer;

	/*An indicator whether this buffer can be freed or not*/
	SipBool 		dDirectOption;
	
	/* This is the Queue used to store the Waiting messages
	for that transaction,of Type SipMessage*/
	SipList			*pslMesgQ;

	/* This is the List to store RPR Txn Keys of Type SipTxnKey */
	SipList			*pslRPRTxnKeyQ;
	
	/* This is the transaction buffer length*/
	SIP_U32bit		dBufferLength;
	
	/*A set of configurable values T1,T2,T3 etc */
	SipTimeOutValues timeoutValues;

	/* This is the SIP Event Context which is returned in indicatetimeOut*/
	SipEventContext	*pEventContext;
	
	/* This is used to set the retranscallback*/
	SipBool			dEnableRetransCallback;
	
	/* This is the handle to store the timer handle returned by
	fast_start_txn_timer*/
	SIP_Pvoid		pTimerHandle;
	
	/* This is used to store the type of timer started in the transaction */
	en_SipTimerType		dTimerStarted;

	/* This is for storing the Retransmissions count */
	SIP_S8bit			dRetransCount;

	/*This is the bitMask that indicates which are the timerTimeOut cbks
		that the application is interested in receiving*/
	SIP_U32bit			dTimeoutCbkBitMask;	
	
	SIP_RefCount		dRefCount;
} SipTxnBuffer;
				
/* This Structure Wraps the Timer Key and Timer Type Wil be Used To pass to\
   fast_startTxnTimer and fast_stopTxnTimer */

typedef struct
{
        en_SipTimerType dTimerStarted;
		SIP_Pvoid		pContextData;
        SipTxnKey*      pKey;
}SipTxnTimeoutData; 

#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif

