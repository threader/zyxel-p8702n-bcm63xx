/*-----------------------------------------------------------------------------
 *
 * File name        : wrp_types.h
 *
 * Purpose          : This file contains the internal classes, CMNLWrapper, 
 *						CGPRSConnector and CNwMonitor definitions.
 *						
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 10-Aug-2005  Irfan Khan						Initial
 *
 *
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/

#ifndef __WRP_TYPES_H
#define __WRP_TYPES_H

#include "icf_port_pltfm_types.h"

/*TYPEDEFS*/

typedef unsigned int		uint32;
typedef unsigned char		uint8;
typedef unsigned short int	uint16;


/**
* WrDataType_et
* Type of data represented by Data_st
*/
typedef enum __WrDataType_et
{
	 EASCIISTRING,	
	 EUNICODESTRING,
	 EHEXSTRING,
	 EBASE64STRING,
	 EUINT8VECTOR,
	 EUINT16VECTOR, 	
 	 EUINT32VECTOR,

	 /* Default corresponding to host byte order */
	 EBITSTRING,
	 
	 /*No data is present */
	 ENULL 

}WrDataType_et;



/*STRUCTURES*/

/**
* Data_st
* Common structure to exchange the data.
*/
typedef struct __Data_st
{
	/*data type format e.g ASCII,etc*/
	WrDataType_et		format; 
	
	/* length of the vector, not the memory */
	uint32				length;	
	
	/* pointer to start of vector */
	void* 				value;	

}Data_st;


/*ENUMS*/
#if 0
/**
* MIMRetVal_et
* Return values
*/
typedef enum __WrRetVal_et
{
	/*Success*/
	WR_ESUCCESS,

	/*Failure*/
	WR_EFAILURE,

	/*For asynchronous request*/
	WR_EAGAIN,

	/*For passing wrong arguements*/
	WR_EUSAGE,

    /*Request pending */
    WR_PENDING
}MIMRetVal_et;



/**
* MIMSimType_et
* SIM Type
*/
typedef enum __MIMSimType_et
{
	EMIM2GSIM,
	EMIM25GSIM,
	EMIM3GUSIM,
	EMIM3GISIM
}MIMSimType_et;


/
























/**
* WrSysErrorCode_et
* System Error Codes
*/
typedef enum __WrSysErrorCode_et 
{
	/*Default value of error code; it may also indicates
	that there was no error since the error code was reset last*/
	ENOERR,  
	
	/*Phone  Error*/	
	EPHONE, 
	
	/*Operation not allowed*/
	ENOTALLOWED, 
	
	/*Operation not supported*/
	ENOTSUPPORTED, 
	
	/*Card Holder Verification required*/
	EPINREQD, 
	
	/*SIM not inserted*/
	ESIMINSERR, 
	
	/*Operation failed at SIM*/
	ESIMFAILURE, 
	
	/*SIM is busy*/
	ESIMBUSY, 
	
	/*SIM is not correct*/
	ESIMWRONG, 
	
	/*Wrong password provided (TBD)*/	
	EPASSWDERR, 
	
	/*Operation failed due to device memory being full*/
	EMEMORYFULL, 
	
	/*Reference ID is in error*/
	EINDEXERR, 
	
	/*Requested data/service not found*/
	ENOTFOUND, 
	
	/*Error detected in memory location*/
	EMEMORYFAILURE, 
	
	/*One or more parameters in error*/
	EINVALIDPARAMS, 
	
	/*Network Service is not available*/
	ENONETWORK, 
	
	/*Operation timed out*/
	ENETWORKTIMEOUT, 
	
	/*Full services not available*/
	ELIMSERVICEONLY, 
	
	/*An unknown system error detected*/
	EUNKOWNERR  

}WrSysErrorCode_et;


/**
* MIMAkaResult_et
* Return status of Authentication and Key Aggreement operation
*/
typedef enum __MIMAkaResult_et
{
	EMIMAKASUCCESSFUL,
	EMIMAKASEQRANGEERR,
	EMIMAKAUNAUTHNWERR,
	EMIMAKASYSTEMERR,
	EMIMAKANULL
}MIMAkaResult_et;


/**
* SMCGprsError_et
* Possible GPRS Errors
*/
typedef enum __SMCGprsError_et
{
	/* Platform call for GPRS service successful */
	EGPRSNOERR,	
	/* non GPRS system error */
	EGPRSSYTEMERR,	

	/* standard values returned by nw/platform */
	EGPRSILLEGALMS,
	EGPRSILLEGALME,
	ESERVICENOTALLOWED,
	EPLMNNOTALLOWED,
	ELOCATIONNOTALLOWED,
	EROAMINGNOTALLOWED,
	EOPTIONNOTSUPPORTED,
	EOPTIONNOTSUBSCRIBED,
	EOPTIONTEMPOUTOFORDER,
	EGPRSAUTHFAILURE,
	EINVALIDMOBILECLASS,
	EUNSPECIFIEDGPRSERR
}SMCGprsError_et;



/**
* SMCTftFilterCmd_et
* TFT Filter commands
*/
typedef enum __SMCTftFilterCmd_et
{
	/*Create a new TFT*/
	ESMCTFTCREATE, 

	/*Delete existing TFT*/
	ESMCTFTDELETE,

	/*Add a packet filter to existing TFT*/
	ESMCTFTFILTERADD, 

	/*Replace a packet filter in existing TFT*/
	ESMCTFTFILTERREP, 

	/*Delete a packet filter from existing TFT*/
	ESMCTFTFILTERDEL, 

	/*TFT Command is not present*/
	ESMCTFTFILTERDEFAULT

}SMCTftFilterCmd_et;


/**
* SMCPdpType_et
* Type of PDP
*/
typedef enum __SMCPdpType_et
{
	 ESMCPDPTYIPV4,
	 ESMCPDPTYIPV6,
	 ESMCPDPTYDEFAULT
}SMCPdpType_et;

/**
* SMCPdpCtxType_et
* Type of PDP Context
*/
typedef enum __SMCPdpCtxType_et
{
	ESMCPDPPRIMARY,
	ESMCPDPSECONDARY
}SMCPdpCtxType_et;


/**
* SMCQosTrClass_et
* Used in Quality of service parameter of PDP context
*/
typedef enum __SMCQosTrClass_et
{
	ESMCQTCCONVERSATIONAL,
	ESMCQTCSTREAMING,
	ESMCQTCINTERACTIVE,
	ESMCQTCBACKGROUND,
	ESMCQTCSUBSCRIBED,
	ESMCQTCDEFAULT
}SMCQosTrClass_et;


/**
* SMCQosDeliveryOrder_et
* Used in Quality of service parameter of PDP context
*/
typedef enum __SMCQosDeliveryOrder_et
{
	ESMCQDONO,
	ESMCQDOYES,
	ESMCQDOSUBSCRIBED,
	ESMCQDODEFAULT 
}SMCQosDeliveryOrder_et;


/**
* SMCPdpOptType_et
* PDP configuration parameter
*/
typedef enum __SMCPdpOptType_et
{
	ESMCPCSCFADDR,
	ESMCIMCNSIG,
	ESMCDNSADDR,
	ESMCPOLICYREJ
}SMCPdpOptType_et;


/**
* SMCQosErrSduDelivery_et
* Used in Quality of service parameter of PDP context
*/
typedef enum __SMCQosErrSduDelivery_et
{
	ESMCQEDNO,
	ESMCQEDYES,
	ESMCQEDNODETECT,
	ESMCQEDSUBSCRIBED,
	ESMCQEDDEFAULT
}SMCQosErrSduDelivery_et;


/**
* SMCDatComp_et
* Data Compression options
*/
typedef enum __SMCDatComp_et
{
	ESMCDCOMPOFF,
	ESMCDCOMPON,
	ESMCDCOMPV42BIS,
	ESMCDCOMPV44,
	ESMCDCOMPDEFAULT
}SMCDatComp_et;


/**
* SMCHdrComp_et
* Header Compression options
*/
typedef enum __SMCHdrComp_et
{
	ESMCHCOMPOFF,
	ESMCHCOMPON,
	ESMCHCOMPRFC1144,
	ESMCHCOMPRFC2507,
	ESMCHCOMPRFC3095,
	ESMCHCOMPDEFAULT
}SMCHdrComp_et;


/**
* NQIDnsReq_et
* DNS Request IP to FQDN or vice-versa
*/
typedef enum __NQIDnsReq_et
{
	ENQIFQDNTOIP,
	ENQIIPTOFQDN
}NQIDnsReq_et;


/**
* NQIDhcpReq_et
* DHCP, SIP Option
*/
typedef enum __NQIDhcpReq_et
{
	ENQIPCSCFFQDN,
	ENQIPCSCFIPV6,
	ENQUIPCSCFFQDNIP
}NQIDhcpReq_et;

/**
* NM_event_et
* Network events
*/
typedef enum __NM_event_et
{
	ENWFailure,
	ENWUp,
	ECurrentNwChange,
	ECurrentNwSuspended,
	EGPRSNotAvailable,
	EGPRSUnattached,
	EGPRSActive,
	EGPRSSuspended,
	EGPRSRegNotAvailable,
	EGPRSRegUnknown,
	EGPRSRegNoService,
	EGPRSRegEmergencyOnly,
	EGPRSRegSearching,
	EGPRSRegBusy,
	EGPRSRegDenied
}Network_event_et;


/*STRUCTURES*/

/**
* Data_st
* Common structure to exchange the data.
*/
typedef struct __Data_st
{
	/*data type format e.g ASCII,etc*/
	WrDataType_et		format; 
	
	/* length of the vector, not the memory */
	uint32				length;	
	
	/* pointer to start of vector */
	void* 				value;	

}Data_st;


/**
* WrTimeval_st
* Time Interval to specify time out.
*/
typedef struct __WrTimeval_st
{
	/*Seconds*/
	uint32 	tv_sec;

	/*Micro seconds*/
	uint32 	tv_usec;

} WrTimeval_st;


/**
* DeviceParameters_st
* Parameters fetched from the device.
*/
typedef struct __DeviceParameters_st
{
	/* mandatory fields*/
	/*SIM Type*/
	MIMSimType_et	simType;
	
	/*IMSI*/
	Data_st 		imsi;
	
	/*Number of APNs*/
	uint32			numApns; 

	/*List of APNs*/
	Data_st*		apnList;	

	/* Optional fields*/
	/*Home network domain name*/
	Data_st			homeNetworkDomain;

	/*Private user id*/
	Data_st			privateUserId; 

	/*Number of public URIs*/
	uint32			numPublicUris; 

	/*List of public URIs*/
	Data_st*		publicUserId;
	
	/*Number of P-CSCF addresses*/
	uint32			numPcscf; 

	/*List of P-CSCF addresses*/
	Data_st*		pcscfAddr;
	
	/*seudonym*/
	Data_st			pseudonym; 
}DeviceParameters_st;


/**
* AkaChallenge_st
* parameters for AKA Challenge
*/
typedef struct __AkaChallenge_st
{
	/*Random number came from NW*/
	Data_st		rand;
	
	/*Parameter used by SIM to authenticate the NW*/
	Data_st		autn;
} AkaChallenge_st;


/**
* AkaResponse_st
* parameters of AKA Response
*/
typedef struct __AkaResponse_st
{
	/*Result of the operation*/
	MIMAkaResult_et 		status;

	/*Response value calculated by SIM*/
	Data_st					res;

	/*IMS Ciphering Key for use in message encryption*/
	Data_st					imsCk;

	/*IMS Integrity Key for use in message authentication*/
	Data_st					imsIk;

	/*Present only if authentication is successful but the authentication 
	 sequence number is out of range*/
	Data_st					auts;

} AkaResponse_st;


/**
* AccessNetwork_st
* P-Network Access information
*/
typedef struct __AccessNetwork_st
{
	/*MIMAccNwType_et*/
	uint8				nwType; 
	
	/*Mobile Country Code*/
	uint16				mcc;

	/*Mobile Network Code*/
	uint16				mnc;

	/*Location Area Code*/
	uint16				lac;

	/*Cell Id*/
	Data_st				cellId;
} AccessNetwork_st;



/**
* MIMAccNwInfoService_st
* Request - Response structure for P-Network Access information
*/
typedef struct _MIMAccNwInfoService_st
{
	AccessNetwork_st*		mimResponse;
	AccessNetwork_st *		mimRequest;
	
	/*Call back data if asynchronous request*/
	void*					mimCallbackData;

	/*Callback function if asynchronous request*/
	void (*mimCallback)(void *resp, void *callbackData);

	/*Timeout*/
	WrTimeval_st			*mimTimeout;

	/*Function to free memory, allocated by wrapper*/
	void (*freeMem)(void* globalCtx, void* mem);

} MIMAccNwInfoService_st;


/**
* MIMAkaService_st
* Request - Response structure for AKA operation
*/
typedef struct __MIMAkaService_st
{
	AkaResponse_st		*mimResponse;
	AkaChallenge_st		*mimRequest;
	
	/*Call back data if asynchronous request*/
	void*					mimCallbackData;

	/*Callback function if asynchronous request*/
	void (*mimCallback)(void *resp, void *callbackData);

	/*Timeout*/
	WrTimeval_st			*mimTimeout;

	/*Function to free memory, allocated by wrapper*/
	void (*freeMem)(void* globalCtx, void* mem);
} MIMAkaService_st;


/**
* MIMPlatformInfoService_st
* Request - Response structure for mimGetDeviceParameters operation
*/
typedef struct __MIMPlatformInfoService_st
{
	DeviceParameters_st			*mimResponse;
	DeviceParameters_st			*mimRequest;

	/*Call back data if asynchronous request*/
	void*					mimCallbackData;

	/*Callback function if asynchronous request*/
	void (*mimCallback)(void *resp, void *callbackData);

	/*Timeout*/
	WrTimeval_st			*mimTimeout;

	/*Function to free memory, allocated by wrapper*/
	void (*freeMem)(void* globalCtx, void* mem);
}MIMPlatformInfoService_st;

/**
* QoSProfile_st
* Quality of service parameters for PDP context
*/

typedef struct __QoSProfile_st
{
	SMCQosTrClass_et		trClass;
	uint32 					maxBitRateUL;
	uint32 					maxBitRateDL;
	uint32 					guaranteedBitRateUL;
	uint32 					guaranteedBitRateDL;
	SMCQosDeliveryOrder_et	deliveryOrder;
	uint32 					maxSduSize;
	uint32					sduErrRatio;
	uint32					resBitErrRatio;
	SMCQosErrSduDelivery_et	errSduDelivery;
	uint32 					transferDelay;
	uint32 					trafficPriority;
} QoSProfile_st;

/**
* TftFilter_st
* TFT Filter parameters
*/
typedef struct __TftFilter_st
{
	SMCTftFilterCmd_et	command;
	uint32				packetFilterId;
	Data_st				srcAddrSubnet;
	uint32				protocolNext;
	Data_st				destPortRange;
	Data_st				srcPortRange;
	Data_st				ipsecSPI;
	Data_st				tosTrafficClassMask;
	Data_st				flowLabel;
	Data_st				authToken;
	uint32				precedence;
} TftFilter_st;

/**
* PdpPriCtxInfo_st
* Primary PDP context information
*/
typedef struct __PdpPriCtxInfo_st
{
	/*Context Id*/
	uint32			contextId;

	/*PDP Type*/
	SMCPdpType_et	pdpType;

	/*APN for the PDP Context*/
	Data_st			apn;

	/*Address associated with PDP context*/
	Data_st			pdpAddr; 
} PdpPriCtxInfo_st;


/**
* PdpSecCtxInfo_st
* Secondary PDP context information
*/
typedef struct __PdpSecCtxInfo_st
{
	/*Context Id*/
	uint32			contextId;

	/*Id of Primary PDP context of this sec. PDP cotnext*/
	uint32			primaryId;

	/*Number of TFT filters*/
	uint32			numTfts;

	/*List of TFT filtees*/
	TftFilter_st*	tftFilterList;
} PdpSecCtxInfo_st;




/**
* PdpOpt_st
* PDP Configuration options
*/
typedef struct __PdpOpt_st
{
	SMCPdpOptType_et	type;
	uint32				length;
	uint8*				value;
} PdpOpt_st;


typedef struct __PdpDefReq_st
{
	SMCPdpCtxType_et	pdpCtxType;
	/* conditional paramters (one and only one of following present)*/
	PdpPriCtxInfo_st*	primary;
	PdpSecCtxInfo_st*	secondary;

	/* Optional Parameters*/
	SMCDatComp_et		dComp;
	SMCHdrComp_et		hComp;
	QoSProfile_st*		qosProfileReq; 
	QoSProfile_st*		minQosReq; 
	uint32				numPdpVarParams;
	PdpOpt_st *			pdpVarParams;
}PdpDefReq_st;


typedef struct __PdpResponse_st
{
	/* mandatory parameters*/
	SMCGprsError_et 	pdpStatusCode; /*Need to change Irfan*/
	uint32				pdpCtxId;
	/* Optional Parameters*/
	QoSProfile_st*		qosProfile;	
	uint32				numPdpVarParams;
	PdpOpt_st *			pdpVarParams;
}PdpResponse_st;


/*to get the address corresponding to PDP context*/
typedef struct _SMCPdpAddrService_st
{
	Data_st*	 pdpAddrResp;

	/* Changed pointer to normal u32 */
	uint32	 pdpCtxId;
	
	/*Call back data if asynchronous request*/
	void*					smcCallbackData;

	/*Callback function if asynchronous request*/
	void (*smcCallback)(void *resp, void *callbackData);

	/*Timeout*/
	WrTimeval_st			*smcTimeout;

	/*Function to free memory, allocated by wrapper*/
	void (*freeMem)(void* globalCtx, void* mem);
}SMCPdpAddrService_st;

/*To define the PDP context*/
typedef struct _SMCPdpDefineService_st
{
	SMCGprsError_et		*cause;
	PdpDefReq_st		*pdpRequest;
	
	/*Call back data if asynchronous request*/
	void*					smcCallbackData;

	/*Callback function if asynchronous request*/
	void (*smcCallback)(void *resp, void *callbackData);

	/*Timeout*/
	WrTimeval_st			*smcTimeout;

	/*Function to free memory, allocated by wrapper*/
	void (*freeMem)(void* globalCtx, void* mem);
} SMCPdpDefineService_st;



/*This data structure is used to pass parameters between
 NM and Wrapper for PDP Activation */
typedef struct _SMCPdpActService_st
{
	SMCGprsError_et pdpResponse; /*need to check irfan*/ 
	PdpDefReq_st *  pdpRequest;
	
	/*Call back data if asynchronous request*/
	void*					smcCallbackData;

	/*Callback function if asynchronous request*/
	void (*smcCallback)(void *resp, void *callbackData);

	/*Timeout*/
	WrTimeval_st			*smcTimeout;

	/*Function to free memory, allocated by wrapper*/
	void (*freeMem)(void* globalCtx, void* mem);
} SMCPdpActService_st;


/*This data structure is used to pass parameters between NM
 and Wrapper for PDP Deactivation */
typedef struct __SMCPdpDeactService_st
{
	PdpResponse_st* pdpResponse;
	uint32  pdpCtxId;
	
	/*Call back data if asynchronous request*/
	void*					smcCallbackData;

	/*Callback function if asynchronous request*/
	void (*smcCallback)(void *resp, void *callbackData);

	/*Timeout*/
	WrTimeval_st			*smcTimeout;

	/*Function to free memory, allocated by wrapper*/
	void (*freeMem)(void* globalCtx, void* mem);
} SMCPdpDeactService_st;




/**
* DhcpPcscfReq_st
* Request of DHCP request for P-CSCF
*/
typedef struct __DhcpPcscfReq_st
{
	NQIDhcpReq_et		pcscfReq;
	/* optional parameters*/
	uint32				numDhcpServer;
	Data_st*			dhcpServerList;
} DhcpPcscfReq_st;


/**
* DhcpPcscfRes_st
* Response of DHCP request
*/
typedef struct __DhcpPcscfRes_st
{
	NQIDhcpReq_et		pcscfReq;
	/* optional parameters*/
	uint32				numPcscf;
	Data_st*			pcscfListIP;
	Data_st*			pcscfListDns;
} DhcpPcscfRes_st;


/**
* DnsLookupReq_st 
* Request structure for DNS lookup
*/
typedef struct __DnsLookupReq_st
{
	NQIDnsReq_et		dnsReq;
	/* optional parameters*/
	/*Number of servers*/
	uint32				numDnsAddr;
	
	/*list of IP addresses*/
	Data_st*			dnsAddrListIP;

	/*list of DNS names*/
	Data_st*			dnsAddrListFqdn;
} DnsLookupReq_st;


/**
* DnsLookupRsp_st 
* Response of DNS lookup
*/
typedef struct __DnsLookupRsp_st
{
	NQIDnsReq_et		dnsReq;
	/* optional parameters*/
	/*Number of servers*/
	uint32				numDnsAddr;
	
	/*list of IP addresses*/
	Data_st*			dnsAddrListIP;

	/*list of DNS names*/
	Data_st*			dnsAddrListFqdn;
} DnsLookupRsp_st;


/**
* NQIDhcpService_st 
* Request - Response structure for DHCP Request
*/
typedef struct __NQIDhcpService_st
{
	DhcpPcscfRes_st*	dhcpResponse;
	DhcpPcscfReq_st*	dhcpReq;
	
	/*Call back data if asynchronous request*/
	void*					nqiCallbackData;

	/*Callback function if asynchronous request*/
	void (*nqiCallback)(void *resp, void *callbackData);

	/*Timeout*/
	WrTimeval_st			*nqiTimeout;

	/*Function to free memory, allocated by wrapper*/
	void (*freeMem)(void* globalCtx, void* mem);
} NQIDhcpService_st;


/**
* NQIDnsService_st 
* Request - Response structure for DNS service
*/
typedef struct __NQIDnsService_st 
{
	DnsLookupRsp_st*	dnsResponse;
	DnsLookupReq_st*	dnsReq;
	
	/*Call back data if asynchronous request*/
	void*					nqiCallbackData;

	/*Callback function if asynchronous request*/
	void (*nqiCallback)(void *resp, void *callbackData);

	/*Timeout*/
	WrTimeval_st			*nqiTimeout;

	/*Function to free memory, allocated by wrapper*/
	void (*freeMem)(void* globalCtx, void* mem);
} NQIDnsService_st;


typedef enum __WrpMsgId_et
{
	EWRPR_INVALID_MSG_ID,
	EWRPR_PRIM_PDP_ACT_RSP, /* Currntly asumerd to be synchronous */
	EWRPR_SEC_PDP_ACT_RSP,
	EWRPR_PDP_DEACT_RSP,
	EWRPR_PDP_DEACT_IND,
	EWRPR_PDP_MODIFY_IND,
	EWRPR_PLATFORM_EVENT_IND,
    EWRPR_IPSEC_SA_CREATE_RSP,
    EWRPR_IPSEC_SA_MODIFY_RSP,
    EWRPR_IPSEC_SA_DELETE_RSP,
    EWRPR_IPSEC_SA_EXPIRY_IND
}WrMsgId_et;

/**
*WrpMsgRsporInd
*structure to notify upper layers
*/

typedef struct
{
	WrMsgId_et 			wrMsgId; /*e.g. ICF_NM_EVENT*/
	void*				rspData; /* void pointer to Actual Response allocated by wrapper*/
	/*Function to free memory, allocated by wrapper*/
	void (*freeMem)(void* globalCtx, void* mem);
}WrMsgRsporInd_st;



typedef  MIMRetVal_et SMCRetVal_et;

typedef  MIMRetVal_et NQIRetVal_et;


/* ************************************************************************** */
/* IP Security related types start */
/* ************************************************************************** */

/* ------------------------------------------------------------ */
/* Enum for return value */
typedef MIMRetVal_et                    IPSecRetVal_et;

/* ------------------------------------------------------------ */
/* Enum for IPSec protocol */ 
typedef enum __IPSecProtocol_et
{
    EIPSECESP,
    EIPSECAH

} IPSecProtocol_et;

/* ------------------------------------------------------------ */
/* Enum for protocol mode */
typedef enum __IPSecPrtMod_et
{
    EIPSECTRANSPORT,
    EIPSECTUNNEL

} IPSecPrtMod_et;

/* ------------------------------------------------------------ */
/* Enum for encryption algorithms */
typedef enum __IPSecEncAlg_et
{
    EIPSECDES,
    EIPSECAES

} IPSecEncAlg_et;

/* ------------------------------------------------------------ */
/* Enum for encryption mode */
typedef enum __IPSecEncMod_et
{
    EIPSECCBC,
    EIPSECECB

} IPSecEncMod_et;

/* ------------------------------------------------------------ */
/* Enum for authentication algorithms */
typedef enum __IPSecHashAlg_et
{
    EIPSECMD5,
    EIPSECSHA

} IPSecHashAlg_et;

/* ------------------------------------------------------------ */
/* Structure for fetching encryption algorithms */
typedef struct __IPSecSaFetchEncAlgo_st
{
    uint16			                    numEncAlgo;
    Data_st			                    *encAlgoList;
    void (*freeMem)(void* globalCtx, void* mem);

} IPSecSaFetchEncAlgo_st;

/* ------------------------------------------------------------ */
/* Structure for fetching authentication algorithms */
typedef struct __IPSecSaFetchAuthAlgo_st
{
    uint16			                    numAuthAlgo;
    Data_st			                    *authAlgoList;
    void (*freeMem)(void* globalCtx, void* mem);

} IPSecSaFetchAuthAlgo_st;

/* ------------------------------------------------------------ */
/* Structure for creating SA for encryption */
typedef struct __IPSecSaCreateEncReq_st
{
    Data_st			                    ueAddr;
    Data_st			                    pcscfAddr;
    IPSecProtocol_et		            protocol;
    IPSecPrtMod_et		                protocolMode;
    uint32			                    spi;
    IPSecEncAlg_et		                encAlg;
    IPSecEncMod_et		                *encMode;
    Data_st			                    encKey;
    uint32			                    *encKeyLife;
    uint32			                    *maskBits;
    uint32			                    saLifetime;
    void (*freeMem)(void* globalCtx, void* mem);

} IPSecSaCreateEncReq_st;

/* ------------------------------------------------------------ */
/* Structure for creating SA for authentication */
typedef struct __IPSecSaCreateAuthReq_st
{
    Data_st			                    ueAddr;
    Data_st			                    pcscfAddr;
    IPSecProtocol_et		            protocol;
    IPSecPrtMod_et		                protocolMode;
    uint32			                    spi;
    IPSecHashAlg_et		                authAlg;
    Data_st			                    authPasswd;
    uint32			                    *maskBits;
    uint32			                    saLifetime;
    void (*freeMem)(void* globalCtx, void* mem);

} IPSecSaCreateAuthReq_st;

/* ------------------------------------------------------------ */
/* Structure for creating SA for encryption and authentication */
typedef struct __IPSecSaCreateEAReq_st
{
    Data_st			                    ueAddr;
    Data_st			                    pcscfAddr;
    IPSecProtocol_et		            protocol;
    IPSecPrtMod_et		                protocolMode;
    uint32			                    spi;
    IPSecEncAlg_et		                encAlg;
    IPSecEncMod_et		                *encMode;
    Data_st			                    encKey;
    uint32			                    *encKeyLife;
    IPSecHashAlg_et		                authAlg;
    Data_st			                    authPasswd;
    uint32			                    *maskBits;
    uint32			                    saLifetime;
    void (*freeMem)(void* globalCtx, void* mem);

} IPSecSaCreateEAReq_st;

/* ------------------------------------------------------------ */
/* Structure for modifying SA */
typedef struct __IPSecSaModifyReq_st
{
    uint32			                    spi;
    uint32			                    lifetime;

} IPSecSaModifyReq_st;

/* ------------------------------------------------------------ */
/* Structure for deleting SA */
typedef struct __IPSecSaDeleteReq_st
{
    uint32			                    spi;

} IPSecSaDeleteReq_st;

/* ------------------------------------------------------------ */
/* Structure for SA create/modify/delete response */
typedef struct __IPSecSaCmdResponse_st
{
    uint32			                    spi;
    IPSecRetVal_et		                result;

} IPSecSaCmdResponse_st;

/* ------------------------------------------------------------ */
/* Structure for SA expiry indication */
typedef struct __IPSecSaExpiryInd_st
{
    uint32			                    spi;

} IPSecSaExpiryInd_st;


/* ************************************************************************** */
/* IP Security related types end */
/* ************************************************************************** */
#endif
 


#endif /*__WRP_TYPES_H*/

