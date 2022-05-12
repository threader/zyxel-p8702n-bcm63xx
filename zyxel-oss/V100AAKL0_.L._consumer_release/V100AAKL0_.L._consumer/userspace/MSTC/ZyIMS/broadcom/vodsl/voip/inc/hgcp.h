/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*    Filename: hgcp.h
*    Creation Date: 2000 16 June 1999 (v0.00)
*    VSS Info:
*        $Revision: 15 $
*        $Date: 10/21/02 12:33p $
*
****************************************************************************
*    Description:
*
*      This file contains the common definitions used by the MGCP interface.
*
****************************************************************************/

#ifndef HGCP_H
#define HGCP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <env.h>
#include <vodslCfg.h>
#include <codec.h>

/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/

#define MGCP_PORT      2427              /* default MGCP IP port */
#define RTP_SECRETSIZE 46
#define RTP_ENCODED_SECRETSIZE 64
#define RTP_ALG_SIZE   5
#define SPI_SIZE       4
#define EMPTY_CIPHER   (-1)
#define DIGIT_STRING_LENGTH 25

#define MGEP_UNKNOWN (-1)

/* Temporary define for interoperability with Metaswitch.
** When this flag is turned on, the patches applied
** for interoperability with Metaswitch are active. */
#define METASWITCH   0

typedef enum
{
   MGE_MGCP_PROTOCOL,
   MGE_NCS_PROTOCOL
} MGE_PROTOCOL;

/* MGCP Response Codes */
typedef enum MGE_STATUS
{
   /* 200-299 indicate successful completion */
   MGERSP_SUCCESS         = 200,          /* executed normally */
   MGERSP_DELCNX          = 250,          /* connection was deleted */
   /* 400-499 indicate transient error conditions */
   MGERSP_TRANSIENT_ERR   = 400,          /* transient error, connection not executed */
   MGERSP_OFFHOOK         = 401,          /* phone already off hook */
   MGERSP_ONHOOK          = 402,          /* phone already on hook */
   MGERSP_TMP_RESOURCES   = 403,          /* insufficient resources (temporary) */
   MGERSP_TMP_BANDWIDTH   = 404,          /* insufficient bandwidth (temporary) */
   MGERSP_RESTARTING      = 405,          /* endpoint is restarting */
   MGERSP_TIMEOUT         = 406,          /* transaction timeout */
   MGERSP_ABORT           = 407,          /* transaction aborted */
   MGERSP_OVERLOAD        = 409,          /* transaction refused due to internal overload */
   MGERSP_NO_ENDPT_AVAIL  = 410,          /* no endpoint available for "any of" wildcard */
   /* 500-599 indicate permanent error conditions */
   MGERSP_ENDPT_UNKNOWN   = 500,          /* endpoint is unknown */
   MGERSP_NOTREADY        = 501,          /* endpoint is not ready */
   MGERSP_RESOURCES       = 502,          /* endpoint has insufficient resources (permanent) */
   MGERSP_FULL_WILDCARD   = 503,          /* "all of" wildcard is not fully supported */
   MGERSP_UNKNOWN_CMD     = 504,          /* unknown or unsupported command */
   MGERSP_UNKNOWN_REMOTE  = 505,          /* unsupported RemoteConnectionDescriptor option */
   MGERSP_OPTION_CONFLICT = 506,          /* conflicting options in Local and Remote descriptors */
   MGERSP_BAD_QUARANTINE  = 508,          /* unsupported quarantine handling */
   MGERSP_BAD_REMOTE      = 509,          /* error in RemoteConnectionDescriptor */
   MGERSP_PROTOCOL_ERR    = 510,          /* protocol error */
   MGERSP_PROTO_EXTENSION = 511,          /* unrecognized protocol extension X+ */
   MGERSP_CANT_DETECT     = 512,          /* endpoint cant detect that event */
   MGERSP_CANT_SIGNAL     = 513,          /* endpoint cant generate that signal */
   MGERSP_CANT_ANNOUNCE   = 514,          /* endpoint cant send that announcement */
   MGERSP_BAD_ID          = 515,          /* incorrect connection ID */
   MGERSP_BAD_CALL        = 516,          /* unknown call ID */
   MGERSP_BAD_MODE        = 517,          /* unsupported or invalid mode */
   MGERSP_BAD_PKG         = 518,          /* unsupported or unknown package */
   MGERSP_NO_DIGITMAP     = 519,          /* gateway does not have a digit map */
   MGERSP_RSIP            = 520,          /* cant execute, restart in progress */
   MGERSP_REDIRECT        = 521,          /* endpoint redirected to another CA */
   MGERSP_BAD_EVT         = 522,          /* no such event or signal */
   MGERSP_BAD_ACTION      = 523,          /* unknown action or illegal combination */
   MGERSP_BAD_FMT         = 524,          /* internal inconsistency in LocalConnectionOptions */
   MGERSP_BAD_EXTENSION   = 525,          /* unknown extension in localconnectionoptions x+ */
   MGERSP_BANDWIDTH       = 526,          /* insufficient bandwidth */
   MGERSP_MISSING_CNX     = 527,          /* missing remote connection descriptor */
   MGERSP_VERSION         = 528,          /* incompatible protocol version */
   MGERSP_HARDWARE_FAILURE= 529,          /* internal hardware failure */
   MGERSP_CAS_ERROR       = 530,          /* CAS signaling protocol error */
   MGERSP_TRUNK_FAILURE   = 531,          /* Trunk facility failure */
   MGERSP_UNSUPPORTED     = 532,          /* unsupported value in localconnectionoptions */
   MGERSP_RSP_TOO_BIG     = 533,          /* response too big */
   MGERSP_CODEC_FAILURE   = 534,          /* codec negotiation failure */
   MGERSP_BAD_PTIME       = 535,          /* Packetization period not supported */
   MGERSP_UNKNOWN_RM      = 536,          /* invalid or unsupported RestartMethod */
   MGERSP_BAD_DIGITMAP    = 537,          /* unknown or unsupport digit map extension */
   MGERSP_PARAM_ERR       = 538,          /* event/signal parameter error */
   MGERSP_CMDPARAM_ERR    = 539,          /* invalid or unsupported command parameter */
   MGERSP_MAX_CONNECTIONS = 540,          /* per-endpoint connection limit exceeded */
   MGERSP_BAD_LOCALOPTION = 541,          /* invalid or unsupported LocalConnectionOptions */


   MGERSP_DUPLICATE_RX    = 999,          /* duplicate unexpired receive transaction */
   MGERSP_ENDPT_DISABLED  = 998           /* message to a disabled endpoint */
} MGE_STATUS;


/* Events detected / Signals requested to the analog line interface */
typedef enum
{
   /*
   ** NOTES:
   **
   ** 1) MGES_CALLWx and MGES_SAS_CASx tones must be in numerical order. Call waiting
   ** tones must be placed before SAS&CAS tones in this list. Also, there must be a
   ** one-to-one mapping between call waiting tones and SAS&CAS tones.
   **
   ** 2) MGES_RINGx and MGES_RINGx_CALLID tones must be in numerical order. Ringing
   ** tones must be placed before ring&callid tones in this list. Also, there must be
   ** a one-to-one mapping between ringing tones and ring&callid tones.
   **
   **
   */
   MGES_NULLEVT,                          /* internal use: Null event */
   MGES_CALLWT,                           /* call waiting tone */
   MGES_CALLW1,                           /* call waiting tone 1 */
   MGES_CALLW2,                           /* call waiting tone 2 */
   MGES_CALLW3,                           /* call waiting tone 3 */
   MGES_CALLW4,                           /* call waiting tone 4 */
   MGES_SAS_CAS,                          /* combined SAS and CAS tone for CLASS2 */
   MGES_SAS_CAS1,                         /* combined SAS and CAS tone for CLASS2 */
   MGES_SAS_CAS2,                         /* combined SAS and CAS tone for CLASS2 */
   MGES_SAS_CAS3,                         /* combined SAS and CAS tone for CLASS2 */
   MGES_SAS_CAS4,                         /* combined SAS and CAS tone for CLASS2 */
   MGES_BUSY,                             /* busy tone */
   MGES_CONF,                             /* confirmation tone */
   MGES_DIAL,                             /* dial tone */
   MGES_MSGW,                             /* message waiting indicator tone */
   MGES_OHWARN,                           /* off-hook warning tone */
   MGES_RINGING,                          /* enable ringer */
   MGES_RING0,                            /* distinctive ringing 0 */
   MGES_RING1,                            /* distinctive ringing 1 */
   MGES_RING2,                            /* distinctive ringing 2 */
   MGES_RING3,                            /* distinctive ringing 3 */
   MGES_RING4,                            /* distinctive ringing 4 */
   MGES_RING5,                            /* distinctive ringing 5 */
   MGES_RING6,                            /* distinctive ringing 6 */
   MGES_RING7,                            /* distinctive ringing 7 */
   MGES_CALLID,                           /* Caller ID message */
   MGES_CALLID_RINGING,                   /* Caller ID Ringing */
   MGES_CALLID_RING0,                     /* Caller ID Ringing 0 */
   MGES_CALLID_RING1,                     /* Caller ID Ringing 1 */
   MGES_CALLID_RING2,                     /* Caller ID Ringing 2 */
   MGES_CALLID_RING3,                     /* Caller ID Ringing 3 */
   MGES_CALLID_RING4,                     /* Caller ID Ringing 4 */
   MGES_CALLID_RING5,                     /* Caller ID Ringing 5 */
   MGES_CALLID_RING6,                     /* Caller ID Ringing 6 */
   MGES_CALLID_RING7,                     /* Caller ID Ringing 7 */
   MGES_REORDER,                          /* reorder tone */
   MGES_SPLASH,                           /* ring splash tone */
   MGES_RINGBACK,                         /* ringback tone */
   MGES_STUTTER,                          /* stutter dial tone */
   MGES_VMWI,                             /* visual message waiting indicator */
   MGES_DTMF0,                            /* DTMF Tone 0 */
   MGES_DTMF1,                            /* DTMF Tone 1 */
   MGES_DTMF2,                            /* DTMF Tone 2 */
   MGES_DTMF3,                            /* DTMF Tone 3 */
   MGES_DTMF4,                            /* DTMF Tone 4 */
   MGES_DTMF5,                            /* DTMF Tone 5 */
   MGES_DTMF6,                            /* DTMF Tone 6 */
   MGES_DTMF7,                            /* DTMF Tone 7 */
   MGES_DTMF8,                            /* DTMF Tone 8 */
   MGES_DTMF9,                            /* DTMF Tone 9 */
   MGES_DTMFS,                            /* DTMF Tone * */
   MGES_DTMFH,                            /* DTMF Tone # */
   MGES_DTMFA,                            /* DTMF Tone A */
   MGES_DTMFB,                            /* DTMF Tone B */
   MGES_DTMFC,                            /* DTMF Tone C */
   MGES_DTMFD,                            /* DTMF Tone D */
   MGES_TIMEOUT,                          /* Timeout event */
   MGES_DTMFX,                            /* DTMF wildcard */
   MGES_CALLCARD,                         /* Calling Card Service tone */
   MGES_PROMPT,                           /* Prompt tone */
   MGES_NETBUSY,                          /* Network busy tone */
   MGES_OFFHOOK,                          /* Off-hook event */
   MGES_ONHOOK,                           /* On-hook event */
   MGES_FLASH,                            /* Hook-flash event */
   MGES_ANSW,                             /* Answer tone detected */
   MGES_FAX,                              /* Fax tone detected */
   MGES_COMPLETE,                         /* Operation complete */
   MGES_MODEM,                            /* Modem detect event */
   MGES_FAILURE,                          /* Operation failure */
   MGES_DTMFL,                            /* DTMF long duration */
   MGES_LONGCNX,                          /* Long duration connection */
   MGES_MEDIA,                            /* Media start */
   MGES_TDD,                              /* Telecom Devices for the Deaf */
   MGES_OSI,                              /* Open switch interval */
   MGES_DTMFDUR,                          /* DTMF Tone with specified duration */
   MGES_DTMFON,                           /* DTMF Tone On/Off signal */
   MGES_CONGEST,                          /* Congestion tone */
   MGES_INTRCPT,                          /* Intercept tone */
   MGES_PREEMPT,                          /* Preempt tone */
   MGES_RECORDER,                         /* Recorder tone */
   MGES_SIT,                              /* SIT tone */
   MGES_HOLD,                             /* Hold tone */
   MGES_ERRTONE,                          /* Error tone */
   MGES_ALERTING,                         /* Alerting tone */
   MGES_LSA,                              /* Line side answer supervision */
   MGES_DISC,                             /* Disconnected state change */
   MGES_DISPLAY1,                         /* Display line 1 */
   MGES_DISPLAY2,                         /* Display line 2 */
   MGES_FKEY1,                            /* Feature key 1 */
   MGES_FKEY2,                            /* Feature key 2 */
   MGES_FKEY3,                            /* Feature key 3 */
   MGES_FKEY4,                            /* Feature key 4 */
   MGES_FKEY5,                            /* Feature key 5 */
   MGES_FKEY6,                            /* Feature key 6 */
   MGES_FKEY7,                            /* Feature key 7 */
   MGES_FKEY8,                            /* Feature key 8 */
   MGES_FKEY9,                            /* Feature key 9 */
   MGES_FKEY10,                           /* Feature key 1 */
   MGES_FKEY11,                           /* Feature key 1 */
   MGES_FKEY12,                           /* Feature key 1 */
   MGES_FKEYSTATE,                        /* Feature key state */
   MGES_FKEYLABEL,                        /* Feature key label */
   MGES_BEEP,                             /* Phone beep tone */
   MGES_HEARTBEAT,                        /* HeartBeat */
   MGES_LOOPOPEN,                         /* FXO Loop Open */
   MGES_LOOPCLOSE,                        /* FXO Loop Close */
   MGES_LOOPFLASH,                        /* FXO Loop Flash */
   MGES_RINGDET,                          /* FXO Ringing Detect */
   MGES_RINGOFF,                          /* FXO Ringing Off */
   MGES_RINGEND,                          /* FXO Ringing End */
   MGES_CLIDRX,                           /* CLID Received */
#if BRCM_VOIP_TO_FXO
   MGES_SETUP,                            /* FXO call setup. */
   MGES_SETUPCOMPLETE,                    /* FXO call setup complete. */
   MGES_RELEASE,                          /* FXO release trunk. */
#endif /* BRCM_VOIP_TO_FXO */
   MGES_LAST
}
MGEEVT;

/* Function key states - parameter to MGES_KEYSTATE signal */
typedef enum MGE_FKEYSTATE
{
   MGEK_DISABLED,                   /* disabled */
   MGEK_ENABLED,                    /* enabled */
   MGEK_IDLE,                       /* idle */
   MGEK_DIALTONE,                   /* dial tone */
   MGEK_CONNECTED,                  /* connected */
   MGEK_DISCONNECTED,               /* disconnected */
   MGEK_RINGING,                    /* ringing */
   MGEK_RINGBACK,                   /* ringback */
   MGEK_HOLDING,                    /* holding */
   MGEK_HELD,                       /* held */
} MGE_FKEYSTATE;



/*
*****************************************************************************
**   MGFEVTCB -- Event CallBack function prototype
**
**   PARAMETERS:
**      endpt - endpoint identifier
**      cxid  - connection identifier (-1 if not used)
**      event - event type code (see MGEEVT)
**      value - event value
**              BR event types - 1 (can be ignored)
**              OO event types - 0 == off. 1 == on
**              TO event types - 0 == stop/off, 1 == start/on
**              String types - (char *) cast to NULL-term string value
**
**   RETURNS:
**      MGE_STATUS - 0 if success, else error code
**
*****************************************************************************
*/
typedef MGE_STATUS
(*MGFEVTCB)(int endpt, int cxid, MGEEVT event, UINT32 value);


/* Connection attributes for Call Control function callback */

typedef enum
{                                 /* Connection command action */
   MGEC_CREATE,                   /* Create new connection on endpoint */
   MGEC_MODIFY,                   /* Modify existing connection */
   MGEC_DELETE                    /* Delete existing connection */
}
MGECONNECT;

typedef struct GWSECINFO
{
   UINT16        sndsecmethod;
   unsigned char sndsecret[RTP_SECRETSIZE];
   int           sndrtpauthalg[RTP_ALG_SIZE];       /* rtp authent algorithm */
   int           sndrtpencryptalg[RTP_ALG_SIZE];    /* rtp encryption algorithm */
   int           sndrtcpauthalg[RTP_ALG_SIZE];      /* rtcp authentication alg */
   int           sndrtcpencryptalg[RTP_ALG_SIZE];   /* rtcp encryption alg */
   unsigned char sndspi[SPI_SIZE];                  /* spi encoded into up to 8 hex chars */
   char          sndbase64sec[RTP_ENCODED_SECRETSIZE + 4];  /* 4 extra bytes are added for decoding */
   UINT16        recvsecmethod;
   unsigned char recvsecret[RTP_SECRETSIZE];
   int           recvrtpauthalg[RTP_ALG_SIZE];      /* rtp authent algorithm */
   int           recvrtpencryptalg[RTP_ALG_SIZE];   /* rtp encryptio algorithm */
   int           recvrtcpauthalg[RTP_ALG_SIZE];     /* rtcp authentication alg */
   int           recvrtcpencryptalg[RTP_ALG_SIZE];  /* rtcp encryption alg */
   unsigned char recvspi[SPI_SIZE];                 /* spi encoded into up to 8 hex chars */
   char          recvbase64sec[RTP_ENCODED_SECRETSIZE + 4]; /* 4 extra bytes are added for decoding */
   int           isOrig;
} GWSECINFO;


/*
** Connection Parameter Block
** Defines connection parameters for RTP
** Note: not all parameters may be known at one time
** e.g. remaddr/remport may be specified in a later MGEC_MODIFY action
** Note: Call Client is responsible for filling in myaddr/myport in response
** to MGEC_CREATE callback when new connection is created
** Note: Multiple codecs may be specified/used on a connection.
** EN: Note: we need two "codec" related Fields: one - for the Local side
**     and another one - for Remote side. The first one is used for composing
**     connection parameters in CRCS (MDCX) response and to properly set the
**     CODEC in the DSP. The second one is used for SEC and dynamic RTP payload
**     type maps.
*/
typedef enum _MGECMODE            /* Connection mode */
{
   MGEM_SNDO,                     /* Send only */
   MGEM_RCVO,                     /* Receive only */
   MGEM_SNDRX,                    /* Send-Receive mode */
   MGEM_CONF,                     /* Send-Receive Conference mode */
   MGEM_INACT,                    /* Inactive */
   MGEM_LOOP,                     /* Network Loopback test */
   MGEM_TEST,                     /* Network Continuity test */
   MGEM_REPLCATE,                 /* Replicate */
   MGEM_EPLOOP,                   /* Endpoint Loopback test */
   MGEM_EPTEST,                   /* Endpoint Continuity test */
   MGEM_MAX
} MGECMODE;


typedef enum _MGEDQOSSTATE         /* DQoS State */
{
   DQOS_PROV   = 1 << 0,           /* NULL state - no reservation or commitment */
   DQOS_ADMT   = 1 << 1,           /* Admitted state - some reservation but no commitment */
   DQOS_ACTIVE = 1 << 2,           /* Active state - some reservation and some commitment */
} MGEDQOSSTATE;

/* The MTA maintains DQoS state on a per connection basis (one state for the
   send direction, and one state for the receive direction).  The state is
   derived from a state machine, which consists of the following uni-directional states:
      1. Provisioned (Null) - no resource reservation exists for the direction
      2. Admitted (Reserve) - resource reservation exists for the direction,
                              but no resources are currently committed
      3. Active (Commit) - resource reservation exists for the direction, and
                           some resources are currently committed
   Depending on the parameters present, the SM will proceed to different states
   and will signal the CM of the necessary changes.  This signaling is
   accomplished using the QoSParmSetTypeUS/DS.  These bit-masks are defined as:
      Bit 0: Provisioned
      Bit 1: Admitted
      Bit 2: Active */
typedef struct _MGZCNXP
{
   enum _MGECMODE mode;                   /* connection mode */
   enum _MGECMODE remotemode;             /* remote connection mode */
   CODECCNXPARMLIST   codecList;              /* list (in ordered preference) of negotiated
                                           * codecs between the local and remote endpoint */
   UINT32         namedPhoneEvts;         /* bit-field of negotiated named telephone events */
   UINT32         bandwidthLoc;           /* local bandwidth in bits/sec rate (0=not specified) */
   UINT32         bandwidthRem;           /* remote bandwidth in bits/sec rate (0=not specified) */
   UINT16         period;                 /* packetization period (msec) */
   UINT16         remoteperiod;           /* remote packetization period (msec) */
   UINT16         echocancel;             /* echo cancellation */
   UINT16         silence;                /* silence suppression */
   UINT32         remaddr;                /* remote dest IP address (0 if unknown) */
   UINT32         myaddr;                 /* local IP address (filled in on return) */
   UINT16         remport;                /* remote dest IP port (0 if unknown) */
   UINT16         myport;                 /* local IP port (filled in on return) */
   UINT8          tos;                    /* IP Type-of-service value */
   GWSECINFO      secInfo;                /* security info */
   UINT32         gateid;                 /* gate id */
   UINT32         resourceid;             /* resource id */
   enum _MGEDQOSSTATE sendstate;          /* DQoS Send State (of the NCS DQoS State Machine) */
   enum _MGEDQOSSTATE recvstate;          /* DQoS Receive State (of the NCS DQoS State Machine) */
   UINT8          QoSParmSetTypeUS;       /* DOCSIS Upstream QoS Parameter Set Type */
   UINT8          QoSParmSetTypeDS;       /* DOCSIS Downstream QoS Parameter Set Type */
   UINT32         rsResvaddr;             /* Reserve destination address */
   UINT16         rsResvport;             /* Reserve destination port */
   SINT16         gain;                   /* endpoint gain control (dB) */
} MGZCNXP;



typedef struct MGZCNXSTATS        /* Connection statistics */
{
   UINT32  txpkts;                /* packets sent */
   UINT32  txbytes;               /* octets sent */
   UINT32  rxpkts;                /* packets received */
   UINT32  rxbytes;               /* octets received */
   UINT32  lost;                  /* packets lost */
   UINT16  jitter;                /* interarrival jitter estimate */
   UINT16  latency;               /* avg tx delay */
} MGZCNXSTATS;

#define  MGZEPCAP_P_RANGE  ((1 << 15))
#define  MGZEPCAP_B_RANGE  ((1 << 15))
#define  MGZEPCAP_PACKAGE_LEN    6

typedef struct MGZEPCAP    /* Endpoint Capabilities */
{
   UINT32   strucSize;     /* Size of Structure: for run-time structure version distinction. */
   UINT16   pCap[2];       /* Packetazation period data (msec). MS-Bit contains "1" if a range is defined */
   UINT8    codecCap[CODEC_MAX_TYPES];   /* Codec caps for the given endpoint */
   UINT32   nteCap;        /* Bit encoded value representing supported named telephone events */
   UINT16   bCap[2];       /* Bandwidth  (in 1000 bits/sec). MS-Bit contains "1" if a range is defined */
   UINT16   eCap;          /* Echo cancelation (0 if NOT supported) */
   UINT16   tCap;          /* Type of Service parameter (0 if NOT supported) */
   UINT16   sCap;          /* Silence Suppression parameter (0 if NOT supported) */
   char     vCap[16][MGZEPCAP_PACKAGE_LEN];  /* Event Packages supported (each char codes a package
                                                name: 'L', 'S', etc). 1st one is default!!! */
   int      mCap[16];      /* Connection Modes. Each char codes the connection mode coded as MGECMODE enum type */
   UINT16   dCap;          /* Dynamic QoS support (0 if NOT supported) */
   UINT16   rtpAuth[16];   /* RTP Authentication Algorithms supported (list) */
   UINT16   rtpEncr[16];   /* RTP Encryption Algorithms supported (list) */
   UINT16   rtcpAuth[16];  /* RTCP Authentication Algorithms supported (list) */
   UINT16   rtcpEncr[16];  /* RTCP Encryption Algorithms supported (list) */
} MGZEPCAP;

typedef struct MGZEPDESCR
{
/**      endptId - reference numeric identifier chosen by caller
**      name    - character string, synchronized with Call Agent configuration
*/
   int      epId;          /* Endpoint ID                                  */
   char     epName[24];    /* Name of the endpoint of the form "aaln/#"    */
   MGZEPCAP epCap;         /* Endpoint capabilities                        */
} MGZEPDESCR;

/*
*****************************************************************************
**   MGFCNXCB -- CallControl Callback function prototype
**
**   PARAMETERS:
**      action - create/modify/delete connection
**      endpt  - endpoint identifier
**      cxid   - connection identifier
**      cnxp   - ptr to connection parameter block
**
**   RETURNS:
**      MGE_STATUS - 0 if success, else error code
**   NOTE:
**        On delete connection, last parameter is a cast to a MGZCNXSTATS*
**        pointer to receive connection statistics (currently not in use)
**
*****************************************************************************
*/
typedef MGE_STATUS
(*MGFCNXCB)(MGECONNECT action, int endpt, int cxid, MGZCNXP *cnxparam);


/*
*****************************************************************************
**   SendPacket callback function prototype
**
**   PARAMETERS:
**      dstdomain - destination domain name or dotted-IP address
**      dstport   - destination port
**      msg       - ASCII message to send (NULL terminated)
**
**   RETURNS:
**      MGE_STATUS - 0 if success, else error code
**
*****************************************************************************
*/
typedef MGE_STATUS
(*MGFSENDCB)( char *dstdomain, UINT16 dstport, char *msg);


/*
*****************************************************************************
**   Connection statistics callback function prototype
**
**   PARAMETERS:
**      cxid      - connection identifier
**      cnxstats  - ptr to connection parameter block
**
**   RETURNS:
**      none
**
*****************************************************************************
*/
typedef void
(*MGFSTATCB)(int cxid, MGZCNXSTATS *cnxstats);


typedef enum
{
   MGEPROV_TdInit,
   MGEPROV_TdMin,
   MGEPROV_TdMax,
   MGEPROV_MWD,
   MGEPROV_TsMax,
   MGEPROV_TLongDuration,
   MGEPROV_RtoInit,
   MGEPROV_RtoMax,
   MGEPROV_Max1,
   MGEPROV_Max2,
   MGEPROV_Max1QEnable,
   MGEPROV_Max2QEnable,
   MGEPROV_TBusyTone,
   MGEPROV_TReorderTone,
   MGEPROV_TDialTone,
   MGEPROV_TMsgWaiting,
   MGEPROV_TStutterDialTone,
   MGEPROV_TOffHookWarningTone,
   MGEPROV_TRinging,
   MGEPROV_TRingback,
   MGEPROV_THist,
   MGEPROV_TCriticalDial,
   MGEPROV_TPartialDial,
   MGEPROV_CaID,
   MGEPROV_CaUdpPort,
   MGEPROV_CallWtgMaxRep,
   MGEPROV_CallWtgDelay,
   MGEPROV_MediaStreamTos,
   MGEPROV_CallSigTos,
   MGEPROV_EndpointEnabled,
   MGEPROV_CallIpAddress
} MGEPROVITEM;

/* MIB access */
typedef enum
{
   MGEPROV_MIB_GET,
   MGEPROV_MIB_SET
} MGEPROVACCESS;

/*
*****************************************************************************
**   Provisioned value callback function prototype
**
**   PARAMETERS:
**      endpt     - endpoint identifier
**      itemId    - id of the requested item
**      itemValue - ptr to user allocated item value
**      length - length of the provisioned item
**      mibAccess - specify whether to retrieve/set provisioned values
**
**   RETURNS:
**      none
**
*****************************************************************************
*/
typedef void
(*MGFPROVCB)(int endpt, MGEPROVITEM itemId, void *itemValue, UINT32 *length, MGEPROVACCESS mibAccess);


/*
*****************************************************************************
**   DNS lookup callback function prototype
**
**   PARAMETERS:
**      hostname     - hostname to to a DNS query on
**      ipAddrList   - list of IP addrs that resolve to hostname
**      numIpAddrs   - number of IP addrs in the list
**      bForceQuery  - force a DNS query instead of relying on the DNS's cache
**
**   RETURNS:
**      none
**
*****************************************************************************
*/
typedef void
(*MGFDNSCB)(char *hostname, UINT32 *ipAddrList, int *numIpAddrs, BOOL bForceQuery);


/* MGCP Configuration definition */

typedef struct MGZCONFIG
{
   char      *gwname;               /* domain name of local gateway */
   char      *agentname[VODSL_MAX_VOIP_ENDPT];  /* domain name of controlling call agent */
   int       protocol;              /* protocol to implement: SGCP, MGCP, MGCP_NCS */
   MGFEVTCB  eventCallBack;         /* incoming event callback function */
   MGFCNXCB  connectCallback;       /* connection control callback function */
   MGFSENDCB sendPacket;            /* outgoing UDP packet callback function */
   MGFSTATCB statisticsCallBack;    /* connection statistics callback function */
   MGFPROVCB provisionCallBack;     /* provisioned values callback function */
   MGFDNSCB  dnsCallBack;           /* dns callback function */
   char      *eid;                  /* equipment id string */
   UINT16   optionbits;             /* bit-field of run-time options */
   CODEC_TYPE prefCodec;            /* Preferred codec */
} MGZCONFIG;

/* Bit definitions for MGZCONFIG.optionbits */
#define MGZCFG_NOWILDRSIP  0x0001   //disallow wildcard in RSIP msg e.g. "RSIP aaln/*" 
#define MGZCFG_IGNOREHOOK  0x0002   /* ignore current hook state when parsing cmds */
#define MGZCFG_LOOPQUARANTINE 0x0004 /* default to loop QuarantineHandling mode */
#define MGZCFG_USE_PREFCODEC 0x0008  /* use preferred codec */


/*
*****************************************************************************
** EXPORTS
*****************************************************************************
*/

/*
*****************************************************************************
**   mgConfig - Configure and initialize the HGCP protocol module
**
**   PARAMETERS:
**      mgzConfigParamp - ptr to config structure
**
**   RETURNS:
**      MGE_STATUS - MGERSP_SUCCESS if success, else error code
**
**   NOTE:
**
*****************************************************************************
*/
extern MGE_STATUS mgConfig( MGZCONFIG *mgzParamp);

/*
*****************************************************************************
**   mgRegisterEndPts - Register EndPoint(s)
**
**   PARAMETERS:
**      epDescr   - Ptr to endpoint descriptor structs each of which contains:
**         epId     - reference numeric identifier chosen by caller
**         epName   - character string, synchronized with Call Agent configuration
**         epCap    - endpoint capabilities
**      numEndpts - Number of endpoint descriptors pointed to by epDescr
**
**   RETURNS:
**      MGE_STATUS - status result code
**
**   NOTE:
**
*****************************************************************************
*/
extern MGE_STATUS mgRegisterEndPts( MGZEPDESCR *epDescr, int numEndpts );

/*
*****************************************************************************
**   mgResetNotify - Notify hgcp about a MTA reset
**
**   PARAMETERS:
**
**   RETURNS:
**
**   NOTE:
**
*****************************************************************************
*/
extern void mgResetNotify( void );

/*
*****************************************************************************
**   mgEndptEnable - Enable an endpoint
**
**   PARAMETERS:
**      endptID - endpoint reference identifier
**
**   RETURNS:
**
**   NOTE:
**
*****************************************************************************
*/
extern void mgEndptEnable( int endptID );

/*
*****************************************************************************
**   mgEndptDisable - Disable an endpoint
**
**   PARAMETERS:
**      endptID - endpoint reference identifier
**
**   RETURNS:
**
**   NOTE:
**
*****************************************************************************
*/
extern void mgEndptDisable( int endptID );

/*
*****************************************************************************
**   mgTimerProc - Timer Process
**
**   PARAMETERS:
**      elapsedtime - timer elasped since last call (milliseconds)
**
**   RETURNS:
**                0 - Nominally
**                1 - When HGCP has completed (ie. after an MTA reset now)
**
**   NOTE:
**
*****************************************************************************
*/
extern int mgTimerProc( int elapsedtime );

/*
*****************************************************************************
**   mgRecvPacket - Accept SGCP/MGCP message from network
**
**   PARAMETERS:
**      srcdomain - domain name (or address in dotted-IP format) of remote system
**      srcport   - port number of remote system
**      msg       - Null-terminated message string (can be multiple lines)
**
**      RETURNS:
**
**      NOTE:
**
*****************************************************************************
*/
extern void mgRecvPacket( char *srcdomain, UINT16 srcport, char *msg);


/*
*****************************************************************************
**   mgSendMsg - Format and send a message to the Call Agent
**
**   PARAMETERS:
**      type    - type of message event
**      endptID - endpoint reference identifier
**      cxID    - connection reference identifier (-1 if none)
**
**   RETURNS:
**
**   NOTE:
**
*****************************************************************************
*/
extern MGE_STATUS mgSendMsg( MGEEVT type, int endptID, int cxID);

/*
*****************************************************************************
**   mgNotify - Notify MGCP of an event
**
**   PARAMETERS:
**      endptID - endpoint reference identifier
**      event   - event type
**      cnxid   - connection ID
**
**   RETURNS:
**      MGE_STATUS
**
**   NOTE:
**
*****************************************************************************
*/
extern MGE_STATUS mgNotify( int endptID, MGEEVT event, int cnxid);

/* For backward compatibility */
#define CODECLIST          CODECCNXLIST
#define CNXCODECLIST       CODECCNXPARMLIST
#define MAX_ENDPT          VODSL_MAX_VOIP_ENDPT
#define MAX_CNX            VODSL_MAX_CNX
#define MAX_CODECS         CODEC_MAX_TYPES

/* Codec Bandwidth (in bits/sec) for various voice encoding formats */
#define CODEC_G711_BW                64000
#define CODEC_G722_MODE1_BW          64000
#define CODEC_G726_32_BW             32000
#define CODEC_G7231_53_BW             5300
#define CODEC_G7231_63_BW             6300
#define CODEC_G728_BW                16000
#define CODEC_G729_BW                 8000
#define CODEC_G729E_BW               11800
#define CODEC_BV32_BW                32000
#define CODEC_BV16_BW                16000
#define CODEC_ILBC_30_BW             13300
#define CODEC_LINEARWB_BW           256000
#define CODEC_GSMAMR_12K_BW          12200
#define CODEC_AMRWB_2385_BW          23850

#ifdef __cplusplus
}
#endif

#endif
