
/******************************************************************************
 ** FUNCTION:
 **	 	This file has the sdp related structures
 ****************************************************************************** 
 **
 ** FILENAME		: sdf_sdpstruct.h
 **
 ** DESCRIPTION		: Structures and enumerations for sdp (used for media 
 **                   handling) are defined here. 
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 14/11/02	Elias					    Creation
 ** 18/11/02	Srikanth					Modified Structures
 ** 
 ****************************************************************************** 
 **				COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#ifndef __SDF_SDP_STRUCT_H__
#define __SDF_SDP_STRUCT_H__

#include "sdf_common.h"
#include "sdf_list.h"

/*Constants*/
#define Sdf_mc_networkType	"IN"
#define Sdf_mc_versionSdp		"0"
#define Sdf_mc_noOfEvents		3
#define Sdf_mc_noOfMediaStates	6
#define Sdf_mc_noOfExtraGroups	0

/******************************* 
 * Enumerations                * 
 *******************************/

/* 
 * Media state enumeration used for offer/answer exchange
 */
typedef enum 
{
	Sdf_en_idleMedia=0,  /* Idle */
	Sdf_en_offerSent,       /* Offer made by UATK*/
	Sdf_en_offerRecv,       /* Offer recvd by UATK*/
	Sdf_en_establishedMedia,     /* O/A exchange done */
	Sdf_en_offerChangeSent, /* Counter Offer made */
	Sdf_en_offerChangeRecv,  /* Counter Offer Recvd */ 
	Sdf_en_invalidMediaState
} Sdf_ty_mediaState;


/* 
 * Enum defines the type of the offer
 */
typedef enum 
{
	Sdf_en_newOffer=0,
	Sdf_en_offerChange,
	Sdf_en_refresh
} Sdf_ty_offerType;

/*
 * Enum defines the type of the mute 
 */
typedef enum 
{
	Sdf_en_connectionMute=0,    /* c=0.0.0.0 */
	Sdf_en_holdSelfMedia,     /* recvonly */
	Sdf_en_holdPeerMedia,     /* sendonly */
	Sdf_en_completeMute       /* inactive */
} Sdf_ty_muteType;


/*
 * Enum defines the type of the media 
 */
typedef enum 
{
	Sdf_en_audio=0,
	Sdf_en_video,
	Sdf_en_whiteboard,
	Sdf_en_text,
	Sdf_en_mediaTypeother
} Sdf_ty_mediaType;


/*
 * Enum defines the type of the media transport
 */
typedef enum 
{
	Sdf_en_rtpAvp=0,
	Sdf_en_udp,
	Sdf_en_tcp,
	Sdf_en_other
} Sdf_ty_transport;


/*
 * Enum defines the media direction 
 */
typedef enum 
{
	Sdf_en_dirAttribNone=0, /* No attribute of a particular kind*/	
	/* a=sendonly/recvonly/sendrecv/inactive */
	Sdf_en_sendonly,	
	Sdf_en_recvonly,	
	Sdf_en_sendrecv,	
	Sdf_en_inactive
} Sdf_ty_dirAttrib;


/*
 * Enum defines the static codec name based on the RTP/AVP
 */
typedef enum 
{
	Sdf_en_PCMU=0,		/*  PCMU/8000/1 	*/
	Sdf_en_1016, 		/*  1016/8000/1 	*/
	Sdf_en_G726_32,		/*  G726-32/8000/1 	*/
	Sdf_en_GSM,			/*  GSM/8000/1 		*/
	Sdf_en_G723,		/*  G723/8000/1		*/
	Sdf_en_DVI4,		/*  DVI4/8000/1 	*/
	Sdf_en_DVI4_16,		/*  DVI4/16000/1 	*/
	Sdf_en_LPC,			/*  LPC/8000/1 		*/
	Sdf_en_PCMA,		/*  PCMA/8000/1 	*/
	Sdf_en_G722,		/*  G722/8000/1 	*/
	Sdf_en_L16_2,		/*  L16/44100/2 	*/
	Sdf_en_L16,			/*  L16/44100/1 	*/
	Sdf_en_QCELP,		/*  QCELP/8000/1  	*/
	Sdf_en_CN,			/*  CN/8000/1 		*/
	Sdf_en_MPA,			/*  MPA 			*/
	Sdf_en_G728,		/*  G728/8000/1 	*/
	Sdf_en_DVI4_11,		/*  DVI4/11025/1 	*/
	Sdf_en_DVI4_22,		/*  DVI4/22050/1 	*/
	Sdf_en_G729,		/*  G729/8000/1 	*/
#ifndef SDF_UAB2B  /* SPR- 3851 */	
    Sdf_en_telephoneEvent,	/*telephone-event  */
#endif
#ifdef SDF_UAB2B  /* SPR-3851**/
    Sdf_en_unassigned ,
    Sdf_en_CelB = 25 ,
	Sdf_en_JPEG = 26,
	Sdf_en_H261 = 31,
	Sdf_en_MPV = 32,
	Sdf_en_MP2T = 33,
	Sdf_en_H263 =34 ,	
#endif
	Sdf_en_otherCodec
} Sdf_ty_codecName;


#ifdef SDF_MEDIAGROUP
/*
 * If the streams are grouped, this enumeration 
 * describes the kind of the group.
 */
typedef enum 
{
	Sdf_en_groupOther=0,
	Sdf_en_groupLS, /* Lip Synchronization */
	Sdf_en_groupFID, /* Flow Identification */
	Sdf_en_groupSRF/* Single Reservation Flow */
} Sdf_ty_groupType;
#endif

#ifdef SDF_QOS
/*
 * Enum defines QoS Strength
 */
typedef enum 
{
	Sdf_en_qosStrengthNone=0,
	Sdf_en_mandatory,
	Sdf_en_optional,
	Sdf_en_none,
 	Sdf_en_failure,
	Sdf_en_unknown
} Sdf_ty_qosStrength;


/*
 * Type of the Qos
 */
typedef enum 
{
	Sdf_en_qosTypeNone=0,
	Sdf_en_e2e,
	Sdf_en_segmented
} Sdf_ty_qosType;
#endif


/*
 * The Session parameter conversion type - based on this the corresponding
 * version filed shall be updated while converting the SdpMessage to
 * Session param
 */
typedef enum 
{
	Sdf_en_convertLocal=0,
	Sdf_en_convertRemote
} Sdf_ty_conversionType;

#ifdef SDF_SESSIONCHANGE
/*
 * Enum defines the type of the change happened in the session
 */
typedef enum 
{
	Sdf_en_noChange=0,
	Sdf_en_addition,
	Sdf_en_rejection,
	Sdf_en_mute,
	Sdf_en_otherChange
}Sdf_ty_mediaChangeType;
#endif

/*
 * This enumeration shall be returned from the match media 
 */

typedef enum
{
	Sdf_en_noMediaStateTransition=0,
	Sdf_en_matchingNotDone,
	Sdf_en_matchingSuccessful,
	Sdf_en_matchingFailed,
#ifdef SDF_QOS
	Sdf_en_qosPreconditionsPresent,
#endif
	Sdf_en_noMatchFound
}Sdf_ty_matchMediaResult;

#ifdef SDF_QOS
/*Internal Usage*/
typedef enum
{
	Sdf_en_noneQos=0,
	Sdf_en_currQos,
	Sdf_en_desQos,
	Sdf_en_confQos
}Sdf_ty_reservTypeInfo;
#endif
typedef enum
{
	Sdf_en_request=0,
	Sdf_en_successResponse,
	Sdf_en_failureResponse
}Sdf_ty_eventType;

typedef enum
{
	Sdf_en_lastOfferRecvd=0,
	Sdf_en_lastOfferMade,
	Sdf_en_currentSession,
	Sdf_en_peerSession,
	Sdf_en_selfCapabilities
}Sdf_ty_sessionParamType;

typedef enum
{
	Sdf_en_changeValid=0,
	Sdf_en_noMediaChange
}Sdf_ty_mediaStateChange;

/*****************************
 * Structures                *
 *****************************/

/*
 * Structure containing details for a codec.
 */
typedef struct
{
    Sdf_ty_codecName	dCodecName;

    /* Required to handle dynamic codecs */
	Sdf_ty_s32bit	dCodecNum;

    /* eg: 8000 for PCMU/8000 */
    Sdf_ty_u32bit	dEncodingRate;

    /* eg: 1 for PCMU/8000/1 */
	Sdf_ty_u8bit	dChannels;

	/* Whatever follows "a=fmtp:<codec> " */
    Sdf_ty_s8bit	*pFmtpString;

    /* Required to handle dynamic codecs */
    Sdf_ty_s8bit	*pOtherCodecName;
    
	/* Reference count*/
	Sdf_ty_refCount	dRefCount;
} Sdf_st_codec;

#ifdef SDF_MEDIAGROUP
/* 
 * Media stream group
 */
typedef struct 
{
	/* Type of the group */
	Sdf_ty_groupType		dGroupType;

	/* will be used if the group is neither LS nor FID */
	Sdf_ty_s8bit       *pOtherGroupType;

	/* Indices to the MediaStreams array*/
    Sdf_ty_slist		slMediaStreamIndex;

	Sdf_ty_refCount	dRefCount;
} Sdf_st_mediaStreamGroup;
#endif


#ifdef SDF_QOS
/* 
 * QoS information of the media stream
 */
typedef struct 
{
	/* Type of the Qos Precondition */
	Sdf_ty_s8bit	*pPrecondition;
	
	/* Strength of the precondition */
	Sdf_ty_qosStrength	dQosStrength;

	/*Ref Count*/
	Sdf_ty_refCount	dRefCount;
} Sdf_st_qosInfo;


/*
 * Details of the segmented Qos
 */
typedef struct 
{
	Sdf_st_qosInfo dLocalSend;
	Sdf_st_qosInfo dLocalRecv;
	Sdf_st_qosInfo dRemoteSend;
	Sdf_st_qosInfo dRemoteRecv;
	/*Ref Count*/
	Sdf_ty_refCount	dRefCount;
} Sdf_st_segmentedQosDetails;

/*
 * Details of the end-to-end Qos 
 */

typedef struct 
{
	Sdf_st_qosInfo dE2eSend;
	Sdf_st_qosInfo dE2eRecv;
	/*Ref Count*/
	Sdf_ty_refCount	dRefCount;
} Sdf_st_e2eQosDetails;

/*
 * Represents the Qos attribute
 */
typedef struct 
{
	/* Qos Type */
	Sdf_ty_qosType 		dQosType;

	/* Details about the proposed Qos (e2e/segmented) */
	union
    {
		Sdf_st_segmentedQosDetails	*pSegmentedQos;
		Sdf_st_e2eQosDetails		*pE2eQos;
	}u;
	Sdf_ty_refCount	dRefCount;
}Sdf_st_qosAttrib;

#endif

/*
 * Structure to represent one a= line.
 */
typedef struct 
{
	/*
	 * Used only in case type == Sdf_en_otherAttrib
	 */
	Sdf_ty_s8bit	*pName;
	Sdf_ty_s8bit	*pValue;
	Sdf_ty_refCount	dRefCount;
} Sdf_st_sessionAttrib;


/* 
 * Structure that represents one m= line.
 */
typedef struct
{
	/*
	 * ID that will be used as MID 
 	 * parameter (if needed)
	 */
	Sdf_ty_s8bit	*pMediaStreamId;
	Sdf_ty_bool		dMidPresence;

	/*
	 * Details about the media stream:
	 * type, transport, port and list of codecs
	 */
	Sdf_ty_mediaType	dMediaType; /* audio, video etc */
	Sdf_ty_transport	dTransport;	/* RTP, UDP etc */
	

	/* List of Sdf_st_codec structs */
	Sdf_ty_slist	slCodecs; 

	Sdf_ty_u16bit	dPort;
	Sdf_ty_u16bit	dNumPort;

	Sdf_ty_u16bit	dPacketTime; /* ptime attrib */
	
	/* 
     * Variables to be used for unrecognized 
     * media-type and transport
     */
	Sdf_ty_s8bit	*pOtherMediaType;
	Sdf_ty_s8bit	*pOtherTransportType;

#ifdef SDF_QOS
	/*
	 * QoS details
     */
	Sdf_st_qosAttrib	*pCurrentQos;
	Sdf_st_qosAttrib	*pDesiredQos;
	Sdf_st_qosAttrib	*pConfirmedQos;
#endif

	/* 
     * Direction attrib
	 * If the Qos attrib is set then this will 
     * not be used. This is for the Non Qos Session.
     */ 
    Sdf_ty_dirAttrib	      dDirectionAttrib;
    
	/* 
	 * Stores the prev direction - used for resuming
	 * the muted (held) stream back
	 */
	Sdf_ty_dirAttrib        dPrevDirectionAttrib;

	/*
	 * Other details. 
	 */
	Sdf_ty_slist	 slOtherAttribs;

	Sdf_ty_slist	 slConnection;

	/*
	 * Used to resume the held call if the c=0.0.0.0 is used
	 * to mute
	 */
	Sdf_ty_slist	 slPreviousConnection;
	
	Sdf_ty_slist 	 slBandwidth;
	Sdf_ty_s8bit	*pKey;

   /* i (media title) line */
	Sdf_ty_s8bit	*pTitle;

    /* used for the negotiation of multiple codec in   
     * the stream with multiple streams
     */
    Sdf_ty_bool 	dMatchedAlready;

	Sdf_ty_refCount	dRefCount;
} Sdf_st_mediaStream;


/*
 * Represents the media session.
 */
typedef struct
{
	/* v= line information */
	Sdf_ty_u8bit      dVersion;

	/*
	 * o= and c= line information
	 */
	SdpConnection	*pConnection;

	/*
	 * Used to resume the held media if the c=0.0.0.0 is used
	 * to mute
	 */
	SdpConnection	*pPreviousConnection;

	
	Sdf_ty_s8bit	*pSessionUser;
	Sdf_ty_u32bit	dLocalSessionVersion;
	Sdf_ty_u32bit	dRemoteSessionVersion;
	Sdf_ty_u32bit	dLocalSessionId;
	Sdf_ty_u32bit	dRemoteSessionId;
	Sdf_ty_s8bit	*pOriginIp;
	Sdf_ty_s8bit	*pOriginAddrType;

	/*
	 * s= line for the SDP
	 */
	Sdf_ty_s8bit	*pSessionName;

	/*
	 * Session level a= lines.
	 */
	Sdf_ty_dirAttrib	dDirectionAttrib;
	Sdf_ty_slist	slOtherSessionAttributes;

	/* 
	 * Stores the prev direction - used for resuming
	 * the muted (held) stream back
	 */
	Sdf_ty_dirAttrib     dPrevDirectionAttrib;
	
	/*
	 * Time or orgin and completion of the media.
	 * (corresponds to the t= line)
	 */
	Sdf_ty_u32bit	dSessionStart;
	Sdf_ty_u32bit	dSessionEnd;

#ifdef SDF_UAB2B
  /*SPR-4218 REPEAT_TIME_4218 */
	Sdf_ty_slist	slRepeatTime;
#endif
	/*
	 * Session level i= line (if any)
	 */
	Sdf_ty_s8bit	*pSessionInfo;

    /*
	 * Session level u line
     */
	Sdf_ty_s8bit	*pUri;

	/*
	 * Session level e line
     */
	Sdf_ty_slist	slEmail;

	/*
	 * Session level p line
     */
	Sdf_ty_slist	slPhone;

	/*
	 * Session level b line
     */
	Sdf_ty_slist	slBandwidth;

	/*
	 * Session level z line
     */
	Sdf_ty_s8bit	*pTimeZoneInfo;

	/*
	 * Session level k line
     */
	Sdf_ty_s8bit	*pEncryptionKey;


#ifdef SDF_MEDIAGROUP
	/* 
     * Array of media stream groups (which contains 
     * Sdf_st_mediaStreamGroups) and number of groups 
     * available in the session 
     */
	Sdf_st_mediaStreamGroup	**pMediaStreamGroups;
    Sdf_ty_u8bit	dNumGroups;
#endif

	/* 
     * Array of media streams (which contains 
     * Sdf_st_mediaStream) and number of media streams
     * available in the session
     */
	 Sdf_st_mediaStream      **pMediaStreams;
     Sdf_ty_u8bit      dNumMediaStreams;

	/*
	 * Equivalent SDP message.
	 */

#ifndef SDF_UAB2B
/*SPR-4217 :B2B doesn't use pSdpMessage  data member and all the processing is done on other data member of sessionParam structure. It is effecting there performance. so kept under flag.  */
SdpMessage		*pSdpMessage;
#endif
	Sdf_ty_refCount	dRefCount;

} Sdf_st_sessionParam;


/*
 * Contains all sub-structures for SDP handling.
 */
typedef struct
{

	/* Media state */
    Sdf_ty_mediaState 	dMediaState;

	/*Flag to indicate whether the received/send sdp caused any Media state
	 * transition or not. */
	Sdf_ty_mediaStateChange	dMediaStateChange; 
	
	/*Flag to keep track of Glare Conditions*/

	Sdf_ty_bool			dOfferProgress;

	/*Flag to keep track of Offer/Answers in ProvResp*/
	Sdf_ty_bool			dProvRespWithSdp;
	/*
	 * Complete set of capabilities of this app.
	 * Used f
	 */
	Sdf_st_sessionParam	*pCapabilities;

	/*
	 * The last offer that was sent by this app.
	 */
	Sdf_st_sessionParam	*pLastOfferMade;

	/*
	 * The last offer that was recd by this app.
	 */
	Sdf_st_sessionParam	*pLastOfferRecd;

	/*
	 * The present session parameters as this 
	 * application sees them. The ports are of this
	 * application and the codecs are a subset of the 
	 * offer that was made. The session parameters may 
	 * or may not equal the capabilities exactly.
     */
	Sdf_st_sessionParam	*pCurrentSession;

	/*
	 * PeerSession provides the Peers view of the session
	 * i.e. the peers port # and media diretcion etc on the 
	 * negotiated session param. This structure will be updated
	 * only if the application opts for it while doing the media
	 * negotiation
	 */
	Sdf_st_sessionParam	*pPeersSession;

	Sdf_ty_refCount	dRefCount;
} Sdf_st_mediaHandling;


#ifdef SDF_SESSIONCHANGE
/*
 * Structure gives information about the change 
 * happened in the media stream by comparing two
 * media stream
 */
typedef struct
{
    Sdf_ty_mediaChangeType 	dMediaChangeType;
    Sdf_ty_bool	            dPortChange;
	Sdf_ty_bool 	        dMediaConnectionChange;
	Sdf_ty_bool	            dDirectionChange;
	Sdf_ty_bool	            dCodecChange;
	Sdf_ty_bool	            dOtherAttributeChange;
} Sdf_st_mediaChange;


/*
 * Structure gives information about the change 
 * happened in the session by comparing two
 * session parameters 
 */
typedef struct
{
	Sdf_ty_bool	dSessionChange;
	Sdf_ty_bool	dSessionConnectionChange;
	/*Array of MediaChange Structures*/
	/* 
	 * The size will be the same as the size of the 
	 * number of media streams in the session param
	 */ 
	Sdf_st_mediaChange	*pMediaChange;
	Sdf_ty_refCount	dRefCount;
} Sdf_st_sessionChange;
#endif

/*Structure for storing the mapping between Codec and ENUM
 * eg: PCMU->Sdf_en_PCMU
 */
typedef struct
{
	const Sdf_ty_s8bit	*pCodecString;
	Sdf_ty_codecName	dCodecName;
}Sdf_st_codecMap;
/*
 * Options bitmask used to specify the runtime options
 * for media negotiation 
 */
typedef Sdf_ty_u32bit Sdf_ty_options;


#ifdef SDF_SESSIONCHANGE
/* 
 * The defined bitmasks -- Add the new masks here
 */
#define SDF_OPT_COMPARE_SESSION 0x01 /* Performs negotiation and Session comparison */
#define SDF_OPT_COMPUTE_PEER_SESSION 0x02 /* Computes the peer session along with the ngtn */
#endif

#endif /* __SDF_SDP_STRUCT_H__ */


