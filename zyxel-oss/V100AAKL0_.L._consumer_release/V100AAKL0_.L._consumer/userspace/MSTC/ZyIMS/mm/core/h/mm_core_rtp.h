/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/


#ifndef __MM_CORE_RTP_H__
#define __MM_CORE_RTP_H__

#include "global.h"
#include "icf_types.h"
#if SRTP_SUPPORT
#include "srtp.h"
#endif


/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

#define RTP_VERSION    2

#define RTP_SEQ_MOD (1<<16)
#define RTP_MAX_SDES 255      /* maximum text length for SDES */

#define RTP_MAX_DROPOUT		3000
#define RTP_MAX_MISORDER		100
#define RTP_MIN_SEQUENTIAL	2
#define RTP_PORT_NUM		4
#define MAX_SSRC_CNT		15
#define RTP_DATA_TOS		5
#define RANDOM_SEED		0x76549812
#define ROTATE_SEED			0x00000028
#define RTCP_SR_INDEX		0
#define RTCP_RR_INDEX		1
#define RTCP_SDES_INDEX	2
#define RTCP_BYE_INDEX		3
#define RTCP_APP_INDEX		4
#define RTCP_XR_INDEX		5
#define RTP_FIXED_HEADER_BYTE_LEN	12
#define RTCP_COMMON_HEADER_BYTE_LEN	4
#define RTCP_SR_HDR_BYTE_LEN	28	/* SR default length (common + ssrc... ) */
#define RTCP_RR_HDR_BYTE_LEN	8	/* RR default length (common + ssrc) */
#define RTCP_SDES_HDR_BYTE_LEN	4	/* SDES default length (common) */
#define RTCP_BYE_HDR_BYTE_LEN	4	/* BYE default length (common) */
#define RTCP_XR_HDR_BYTE_LEN	8	/* XR default length (common + ssrc) */
#define REPORT_BLOCK_BYTE_SIZE	24

#define SECS_BETWEEN_1900_1970	2208988800u
#define SECOND_FROM_1900	0x83AA7E80 /* Seconds from 1900 01/01 to 1970 01/01 */
#define NTP_FRACTION		0x00419A02 /* Around 200 pico second */

#define RTP_BIG_ENDIAN	1
#define MM_RTP_MAKE_64BTO32B(a,b)  (((a) << 16) | ((b) >> 16))

#define RTP_BCM_RTCP_DRIVER_TASK	1
/* MSTC porting by Andylin, bind interface , Iproute2 */
#define MAX_IFNAME_LEN			16

typedef void (*RTPCALLBACK)(icf_call_id_t call_id, icf_stream_id_t stream_id, icf_media_type_t	stream_type, 
			icf_uint32_t pload_type, icf_uint8_t *InMessage, icf_uint16_t MessageLen, icf_uint32_t TimeStamp, void* handle);

/************************************************************************
*All the typedefs should go here
************************************************************************/
typedef enum {
    RTCP_SR   = 200,
    RTCP_RR   = 201,
    RTCP_SDES = 202,
    RTCP_BYE  = 203,
    RTCP_APP  = 204,
    RTCP_XR = 207
} rtcp_type_t;

typedef enum {
    RTCP_SDES_END   = 0,
    RTCP_SDES_CNAME = 1,
    RTCP_SDES_NAME  = 2,
    RTCP_SDES_EMAIL = 3,
    RTCP_SDES_PHONE = 4,
    RTCP_SDES_LOC   = 5,
    RTCP_SDES_TOOL  = 6,
    RTCP_SDES_NOTE  = 7,
    RTCP_SDES_PRIV  = 8
} rtcp_sdes_type_t;

#ifndef SRTP_H /*conflict definition in srtp.h*/
/*
 * RTCP common header word
 */
typedef struct {
    unsigned int version:2;   /* protocol version */
    unsigned int p:1;         /* padding flag */
    unsigned int count:5;     /* varies by packet type */
    unsigned int pt:8;        /* RTCP packet type */
    unsigned int length:16;   /* pkt len in words, w/o this word */
} rtcp_common_t;
#endif

typedef struct {
    uint32 ssrc;             /* data source being reported */
    unsigned int fraction:8;  /* fraction lost since last SR/RR */
    int lost:24;              /* cumul. no. pkts lost (signed!) */
    uint32 last_seq;         /* extended last seq. no. received */
    uint32 jitter;           /* interarrival jitter */
    uint32 lsr;              /* last SR packet from this source */
    uint32 dlsr;             /* delay since last SR packet */
} rtcp_rr_t;

/*
 * SDES item
 */
typedef struct {
    uint8 type;              /* type of item (rtcp_sdes_type_t) */
    uint8 length;            /* length of item (in octets) */
    char data[1];             /* text, not null-terminated */
} rtcp_sdes_item_t;


typedef struct {
	uint8 bt;             /* Block Type */
	uint8 ts;			/* Type Specific */
	uint16 block_length;	/* Block Length */
	union {
		/* Loss RLE Report Block */
		struct {
			uint32 ssrc;     /* Receiver generating this report */
			uint16 begin_seq;
			uint16 end_seq;
			uint16 chunk[1]; /* variable-length list */
		} loss_RLE;
		/* Duplicate RLE Report Block */
		struct {
			uint32 ssrc;     /* Receiver generating this report */
			uint16 begin_seq;
			uint16 end_seq;
			uint16 chunk[1]; /* variable-length list */
		} dup_RLE;
		/* Packet Receipt Times Report Block */
		struct {
			uint32 ssrc;     /* Receiver generating this report */
			uint16 begin_seq;
			uint16 end_seq;
			uint32 rcv_time[1]; /* variable-length list */
		} pkt_rcv_time;
		/* Receiver Reference Time Report Block */
		struct {
			uint32 NTPTimeStamp_Int;
			uint32 NTPTimeStamp_Frac;
		} rcv_ref_time;
		/* DLRR Report Block */
		struct {
			uint32 ssrc;
			uint32 lrr;
			uint32 dlrr;
		} dlrr;
	} report_block;
} rtcp_xr_t;

/*
 * One RTCP packet
 */
typedef struct {
    rtcp_common_t common;     /* common header */
    union {
        /* sender report (SR) */
        struct {
            uint32 ssrc;     /* sender generating this report */
            uint32 ntp_sec;  /* NTP timestamp */
            uint32 ntp_frac;
            uint32 rtp_ts;   /* RTP timestamp */
            uint32 psent;    /* packets sent */
            uint32 osent;    /* octets sent */ 
            rtcp_rr_t rr[1];  /* variable-length list */
        } sr;

        /* reception report (RR) */
        struct {
            uint32 ssrc;     /* receiver generating this report */
            rtcp_rr_t rr[1];  /* variable-length list */
        } rr;

        /* source description (SDES) */
        struct rtcp_sdes {
            uint32 src;      /* first SSRC/CSRC */
            rtcp_sdes_item_t item[1]; /* list of SDES items */
        } sdes;

        /* BYE */
        struct {
            uint32 src[1];   /* list of sources */
            /* can't express trailing text for reason */
        } bye;

	/* XR */
	struct {
		uint32 ssrc;
		rtcp_xr_t xrblock[1]; /* variable-length list */
	} xr;
    } r;
} rtcp_t;



/*  RTP header format:

     0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |V=2|P|X|  CC   |M|     PT      |       sequence number         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                           timestamp                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           synchronization source (SSRC) identifier            |
   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
   |            contributing source (CSRC) identifiers             |
   |                             ....                              |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
 
typedef struct {
#ifdef RTP_BIG_ENDIAN
    unsigned int version:2;   /* protocol version */
    unsigned int p:1;         /* padding flag */
    unsigned int x:1;         /* header extension flag */
    unsigned int cc:4;        /* CSRC count */
    unsigned int m:1;         /* marker bit */
    unsigned int pt:7;        /* payload type */
#else
    unsigned int cc:4;        /* CSRC count */
    unsigned int x:1;         /* header extension flag */
    unsigned int p:1;         /* padding flag */
    unsigned int version:2;   /* protocol version */
    unsigned int pt:7;        /* payload type */
    unsigned int m:1;         /* marker bit */
#endif
    unsigned int seq:16;      /* sequence number */
    uint32 ts;               /* timestamp */
    uint32 ssrc;             /* synchronization source */
} rtp_header_t;

typedef struct {
    uint16 ext_type;         /* defined by profile */
    uint16 len;              /* extension length in 32-bit word */
} rtp_header_ext_t;

/*
* Per-source state information
*/
typedef struct {
   uint16 max_seq;        /* highest seq. number seen */
   uint32 cycles;         /* shifted count of seq. number cycles */
   uint32 base_seq;       /* base seq number */
   uint32 bad_seq;        /* last 'bad' seq number + 1 */
   uint32 probation;      /* sequ. packets till source is valid */
   uint32 received;       /* packets received */
   uint32 expected_prior; /* packet expected at last interval */
   uint32 received_prior; /* packet received at last interval */
   uint32 transit;        /* relative trans time for prev pkt */
   uint32 jitter;         /* estimated jitter */

 	uint32	last_recv_lsr;
 	uint32	roundtrip_delay;
	struct timeval last_recv_sr_time;   /* time at which last SR was received  */
	uint32 lsr;              /* last SR packet from this source */
	uint32 dlsr;             /* delay since last SR packet */
#if 1 /*Jason , syslog*/
	struct timeval last_arrival_time; /* last rtp packet arrival time */
	uint32 last_ts; /* last rtp timestamp*/
	uint32 totalJitter;
	uint32 totalrtt;
#endif
} rtp_rr_t;


typedef struct udpSocket_s {
   	icf_int32_t s;
	icf_uint32_t	local_addr;
	icf_uint16_t	local_port;
	icf_uint32_t	remote_addr;
	icf_uint16_t	remote_port;
}udpSocket_t;


typedef struct rtcpStats_s
{
	uint8	rxPeddingOctetCnt;	 /* padding octet */
	uint16     rxCSRCOctetCnt;       /* CSRC count  */
  	uint16     rxExtenOctetCnt;		/* header extension length */

	uint8	cname[64+1];
	uint8	cname_len; 
	uint32	tx_last_rtp_pktcnt;
  	uint32	tx_rtp_pkt_cnt;
  	uint32	tx_rtp_octet_cnt;
  	uint32	tx_rtp_pkt_failcnt;
  	uint32	tx_rtcp_cnt;
  	uint32	tx_rtcp_failcnt;

  	uint32	rx_rtp_pkt_cnt;
  	uint32	rx_rtp_octet_cnt;
  	uint32	rx_rtcp_pkt_cnt;

	uint8	rx_rtcp_sr_recvd;	/* Sender Report received */
	uint8	rx_rtcp_rr_recvd;	/* Receiver Report received */
	uint8	rx_rtcp_bye_recvd;	/* BYE received */
	uint8	tx_rtcp_sr_sent;		/* Sender Report sent */
	uint8	tx_rtcp_rr_sent;		/* Receiver Report sent */
	uint8	tx_rtcp_bye_sent;
#if 1 /*Jason , rtp statistic*/
	uint8	remoteLossRate;
	uint32	remoteJitter;
	uint32	totalRemoteJitter;
	uint32	totalRemoteCnt;
#endif
	rtp_rr_t	rtp_rr;
} rtcpStats_t;

typedef struct rtcpTimer_s{
	uint32      		interval;
	uint32      		tm_intv;     			
	tm_id_t			tm_id;	
	uint32 			rty_count;
} rtcpTimer_t;


typedef struct rtpTable_s{

	#if 1 /* MSTC porting by Andylin, bind interface , Iproute2 */
	uint8 ifname[MAX_IFNAME_LEN];
	#endif
       icf_call_id_t	call_id;
	icf_stream_id_t  	stream_id;		/* stream-id on which RTP is received */
	icf_media_type_t	stream_type;
        icf_line_id_t	line_id;

	/* Session Info */
	icf_uint32_t	ssrc;				/* SSRC */
	icf_uint16_t	seq;				/* sequence number */
	icf_uint32_t	remote_ssrc;				/* Remote SSRC */
	udpSocket_t rtp_socket;
	udpSocket_t rtcp_socket;
	icf_uint8_t	qos_mark;
	icf_uint32_t  tx_timestamp;		/* timestamp for RTP data packet */
	
	/* Media Info */
	icf_uint8_t	transport_mode;		/*UDP/TCP/RTP_AVP or UDPTL*/
	icf_uint8_t	media_mode;		/* INVALID:0/INACTIVE:1/SEND_ONLY:2/RECV_ONLY:3/SEND_RECV:4 */

	icf_uint16_t	bandwidth;
	icf_uint8_t	pref_ptime;	
	icf_boolean_t	silence_suppression;
	icf_boolean_t	echo_canc;	

	/* RTCP info*/
	icf_uint8_t	rtcp_enabled;	
	rtcpStats_t	rtcp_stats;
	rtcpTimer_t	rtcp_tx_timer;

#if 1 /*Jason , rtp statistic*/
	icf_uint32_t initTimeStamp;
#endif
#if SRTP_SUPPORT
	srtp_t srtpSendSession;
	srtp_policy_t srtpSendPolicy;
	srtp_t srtpRecvSession;
	srtp_policy_t srtpRecvPolicy;
#endif
	/* DSP CallBack function */
	RTPCALLBACK	voipRtpCallBack;
	RTPCALLBACK	voipRtcpCallBack;

	struct rtpTable_s *next;	/* next active port */
}rtpTable_t;

typedef struct rtpTablePool_s{
	struct rtpTable_s  	*front;		/*rtp_table_t*/
	struct rtpTable_s  	*rear;		/*rtp_table_t*/
	icf_uint32_t       count;
}rtpTablePool_t;	

#if 1 /*Jason , rtp statistic*/
typedef struct{
	icf_uint32_t total_tx_pkt_cnt; /* total rtp packets sent */
	icf_uint32_t total_tx_oct_cnt; /* total rtp bytes sent */
	icf_uint32_t total_rx_pkt_cnt; /* total rtp packets received */
	icf_uint32_t total_rx_oct_cnt; /* total rtp bytes received */
	icf_uint32_t total_call_time;  /* total call session time */
//for VoIP syslog , record only one stream.	
	icf_uint32_t tx_pkt_cnt; /*  rtp packets sent */
	icf_uint32_t tx_oct_cnt; /*  rtp bytes sent */
	icf_uint32_t rx_pkt_cnt; /*  rtp packets received */
	icf_uint32_t rx_oct_cnt; /*  rtp bytes received */
	icf_uint32_t avgjitter;  /*  rtp jitter */
	icf_uint32_t pkt_lost;  /* packet lost*/
	
}SipLineStats_t;
#endif

void rtpTask(void);

void rtcpTask(void);

static icf_int32_t rtpTaskCreate( void );

static icf_int32_t rtcpTaskCreate( void );

static void rtpTableInit(rtpTable_t *table);

static icf_int32_t rtpTableListInit(void);

static icf_uint32_t rtpSocketOpen(udpSocket_t	*p_soket);

static rtpTable_t *rtpTableAlloc(void);

static icf_int32_t rtpTableDelete(
       icf_call_id_t call_id,
	icf_stream_id_t stream_id,
	icf_media_type_t	stream_type
);

static icf_int32_t rtpTableFree(rtpTable_t *bp);

static icf_int32_t  rtpTableInsert(rtpTable_t *table);

rtpTable_t *rtpTableGetById(
       icf_call_id_t	call_id,
	icf_stream_id_t  	stream_id,
	icf_media_type_t	stream_type
);

static icf_int32_t rtpRxProc( 
       rtpTable_t *table,
	void *packet, 
	icf_int32_t length 
);
icf_int32_t rtcpRxProc( 
       rtpTable_t *table,
	void *packet, 
	icf_int32_t length 
);

//Ssu-Ying, support Multiple Profile
static icf_uint16_t rtpPortSearchInUseLinkList(
	icf_line_id_t SipAccountId, 
	icf_uint32_t rtpPortPoolSize
);

#ifdef NORWAY_VOIP_CUSTOMIZATION
static icf_uint16_t rtpPortSearchInnetstat(
	icf_uint16_t rtpPort
);

static icf_uint16_t rtpPortSearchInconntrack(
	icf_uint16_t rtpPort,
	char *filename
);
#endif

static icf_uint16_t rtpPortPoolAllocate(
	icf_line_id_t SipAccountId, 
	icf_media_type_t	stream_type
) ;

static icf_int32_t rtpPortPoolRelease(
	icf_uint16_t port,
	icf_media_type_t	stream_type
) ;

mm_return_t rtpInit(void);

void rtpSend(
	icf_call_id_t call_id, 
	icf_stream_id_t	stream_id, 
	icf_media_type_t	stream_type,
	uint16 logicalId,
	icf_uint32_t pload_type, 
	icf_uint8_t *packet, 
	icf_uint16_t len, 
	icf_uint32_t timestamp, 
	void* handle
);

icf_uint16_t rtcpSdesItem(
	icf_uint8_t type, 
	char *data, 
	char *string);

void rtcpModifySdes(
	rtpTable_t *table,
	icf_uint8_t	*p_msg
);

void rtcpParseRR(
	rtpTable_t *table,
	rtcp_rr_t *p_rr,
	struct timeval recv_time_tv
);

void rtcpPacketParse(
	rtpTable_t *table,
	icf_uint8_t *packet, 
	icf_uint16_t len
);

void rtcpCommonHeader(
 	rtcp_common_t *p_common,
 	int type,
	int count, 
	int bytes_len
);

void rtcpInitSeq(
	rtp_rr_t *src,
	icf_uint16_t seq
);

int rtcpUpdateSeq(
	rtp_rr_t *src,
	icf_uint16_t seq
);

void rtcpJitter(
	rtcpStats_t *rtcp_stats, 
	icf_uint32_t timestamp
);

void rtcpGetEnable(
	rtpTable_t *table,
	uint16 line_id
);

void rtcpGetLocalCname(
	rtcpStats_t	*rtcp_stats,
	uint16 line_id
);

void rtcpGetTxInterval(
	rtpTable_t *table,
	uint16 line_id
	
);

void rtcpPktFractionlost(
	rtp_rr_t *src, 
	icf_uint32_t* input_fraction, 
	icf_uint32_t* input_lost);

icf_uint16_t rtcpWriteSR( 
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
);

icf_uint16_t rtcpWriteRR( 
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
);

icf_uint16_t rtcpWriteApp( 
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
);

icf_uint16_t rtcpWriteXR(
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
);

icf_uint16_t rtcpWriteSdes( 
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
);

icf_uint16_t rtcpWriteBye(
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
);

double rtcpInterval(
	int rtcpTxTime, 
	int initial
);

int rtcpOnExpire(
	tm_id_t	timer_id,
	void	*bp
);

void rtcpEnableTxTimer(
	rtpTable_t *table
);

void rtcpDisableTxTimer(
	rtpTable_t *table
);

void rtcpStoreRtpStats(
	rtpTable_t *table, 
	uint8 *p_rtp_packet, 
	uint16 length
);

void rtcpStoreRtpInfo(
	rtpTable_t *table,
	void *packet, 
	icf_int32_t length 
);

void rtcpSendPacket(
	icf_call_id_t call_id, 
	icf_stream_id_t	stream_id, 
	icf_media_type_t	stream_type,
	uint16 logicalId,
	icf_uint32_t pload_type, 
	icf_uint8_t *packet, 
	icf_uint16_t len, 
	icf_uint32_t timestamp, 
	void* handle
);

void rtcpSend(
	rtpTable_t *table,
	icf_uint8_t event,
	icf_uint8_t *packet, 
	icf_uint16_t len
);

icf_uint32_t udpSend(
	icf_int32_t	socket, 
	icf_uint8_t	*packet, 
	icf_uint16_t	len,
	icf_uint16_t	remote_port,
	icf_uint32_t	remote_addr
);


void SipLineStatsResetAll(void);


#ifdef DUMP_RTCP_PKT
static void dispRTCPPacket(
	unsigned char* packetBuffer, 
	int bufferLength);

void rtpTableDisplay(void);
#endif



/* All memory related macros will go here */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif 

