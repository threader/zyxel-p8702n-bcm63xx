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
*    Filename: rtp.h
*    Creation Date: 2000 14 June 2000 (v0.00)
*    VSS Info:
*        $Revision: 8 $
*        $Date: 5/10/01 11:28a $
*
****************************************************************************
*    Description:
*
*      This file contains the API definitions used by the RTP.
*
****************************************************************************/

#ifndef _RTP_H_
#define _RTP_H_

/* PLATFORM DEPENDENT INCLUDES / CONFIGURATION BEGIN */

/* Configurations */
#define RTP_TTL            0                /* TTL field is used for multicast only */

#include <stdlib.h>                 /* For rand() */
#include <vodslCfg.h>
#include <endpoint_user.h>
#include <env.h>
#include <udp.h>                    /* UDP API */


/* PLATFORM DEPENDENT INCLUDES / CONFIGURATION END */


/*
*****************************************************************************
** GLOBAL VARIABLES
*****************************************************************************
*/
extern UINT16 minRtpPortNum;
extern UINT16 maxRtpPortNum;

/*
*****************************************************************************
** TYPEDEFS AND CONSTANTS
*****************************************************************************
*/


/*
** RTP Shim function return type
*/
typedef enum
{
   RTP_SUCCESS,
   RTP_NETFAIL,                  /* Network / socket error */
   RTP_NORSC,                    /* RTP stack runs out of resource */
} RTPSTATUS;

typedef struct RTCPCNXSTATS      /* Connection statistics */
{
   /* connection statistics from the local endpoint's perspective */
   UINT32   localPktSent;        /* Local packets sent */
   UINT32   localOctetSent;      /* Local octets sent */
   UINT32   localPktRecvd;       /* Local packets received */
   UINT32   localOctetRecvd;     /* Local octets received */
   SINT32   localPktLost;        /* Local packet lost */
   UINT16   localJitter;         /* Local interarrival jitter estimate (in ms) */
   UINT16   localLatency;        /* Local avg tx delay (in ms) */

   /* connection statistics from the remote endpoint's perspective */
   UINT32   remotePktSent;       /* Remote packets sent */
   UINT32   remoteOctetSent;     /* Remote octets sent */
   SINT32   remotePktLost;       /* Remote packets lost */
   UINT32   remoteJitter;        /* Remote jitter */
}
RTCPCNXSTATS;


/*
**********************************************************************
** FUNCTION:   RTPRECVCB
**
** PURPOSE:    Callback function for receiving RTP packet through the IP stack
**
** PARAMETERS: handle     - handle for the caller
**             pBuffer    - buffer for the RTP packet received
**             length     - length of packet
**
** RETURNS:    Nothing
****************************************************************************
*/
typedef void (*RTPRECVCB)( int handle, void *rtpPacket, int bufSize);


#ifdef __cplusplus
extern "C" {
#endif

/*
*****************************************************************************
** FUNCTION:   rtcpDisable
**
** PURPOSE:    Disable RTCP driver task to send RTCP packets
**
** PARAMETERS: handle       - [IN] RTP session handle
**
** RETURNS:    None
**
** NOTE:       Typically, RTCP statistics is queried after rtpDisable() is
**             called.  The collected statistics queried by calling
**             rtpReportStat().
*****************************************************************************
*/
void rtcpDisable(int handle);


/*
*****************************************************************************
** FUNCTION:   rtcpEnable
**
** PURPOSE:    Enables RTCP driver task to send RTCP packets
**             Enables RTCP driver task to collect statistics
**
** PARAMETERS: handle  - [IN] RTP session handle
**
** RETURNS:    None
**
** NOTE:       The collected statistics queried by calling rtpReportStat().
**             Statistics block is initialized by rtpResetStat().
*****************************************************************************
*/
void rtcpEnable(int handle);


/*
*****************************************************************************
** FUNCTION:   rtpInit
**
** PURPOSE:    Initialize RTP socket interface
**
** PARAMETERS: none
**
** RETURNS:    nothing
**
*****************************************************************************
*/
void rtpInitStack( void );


/*
*****************************************************************************
** FUNCTION:   rtpOpen
**
** PURPOSE:    open a RTP session
**
** PARAMETERS: handle       - [IN] Handle assigned by the RTP stack
**             localaddr    - [IN] RTP address to be opened.  Used only when
**                            multicast is used.  Use 0 for the unicast case.
**             localport    - [IN] RTP (localport) / RTCP (localport + 1) to be
**                            opened / listened for RTP packets
**             recvCallback - [IN] function to call when an RTP packet is received
**
** RETURNS:    RTPSTATUS
**                RTP_SUCCESS
**                RTP_NORSC
**                RTP_NETFAIL
**
** NOTE:       The RTP module assumes that a task context with name "HTSK" is
**             used to send RTP packets.  The name is only needed when pSOS
**             is used.
**             wlocalport is specified in host byte-ordering.
**
*****************************************************************************
*/
RTPSTATUS rtpOpen
(
   int           ifno,
   UINT16        localport,
   IPADDR        localaddr,
   BOS_TASK_ID   sendTaskId,
   BOS_TASK_ID   disableTaskId,
   RTPRECVCB     recvCallback,
   RTPRECVCB     rtcpCallback,
   UINT8         tos,
   BOS_SOCKET   *sharedSendSocket,
   BOS_SOCKET   *sharedDisableSocket,
   int          *handle
);

/*
*****************************************************************************
** FUNCTION:   rtpReportStat
**
** PURPOSE:    This function prepares report statistics to the RTP client
**
** PARAMETERS: handle       - [IN] RTP stack handle
**             rtcpcnxstats - [OUT] statistics (see RTCPCNXSTATS)
**
** RETURNS:    None
*****************************************************************************
*/
void rtpReportStat(int handle, RTCPCNXSTATS *rtcpcnxstats);

/*
*****************************************************************************
** FUNCTION:   rtpResetStat
**
** PURPOSE:    Reset statistics
**
** PARAMETERS: handle - handle assigned by the RTP stack
**
** RETURNS:    none
**
*****************************************************************************
*/
void rtpResetStat(int handle);

/*
*****************************************************************************
** FUNCTION:   rtpSetRTPRemote
**
** PURPOSE:    Set the destination address / port that RTP packet will route to
**
** PARAMETERS: handle       - handle to the rtp session
**             remoteAddr   - remote address in host byte ordering
**             remoteport   - remote port number
**
** RETURNS:    nothing
**
** NOTE:       ttl is used only when remoteAddr is a multicast address
*****************************************************************************
*/
void rtpSetRTPRemote(int handle, UINT32 remoteAddr, UINT16 remoteport);

/*
*****************************************************************************
** FUNCTION:   rtpSetRTCPRemote
**
** PURPOSE:    Set the destination address / port that RTP packet will route to
**
** PARAMETERS: handle       - handle to the rtp session
**             remoteAddr   - remote address in host byte ordering
**             remoteport   - remote port number
**
** RETURNS:    nothing
**
*****************************************************************************
*/
void rtpSetRTCPRemote(int handle, UINT32 remoteAddr, UINT16 remoteport);

void rtpSetT38Remote( int handle, UINT16 remoteport );

/*
*****************************************************************************
** FUNCTION:   rtpClose
**
** PURPOSE:    Close the rtp session
**
** PARAMETERS: handle       - handle to the rtp session
**
** RETURNS:    nothing
**
*****************************************************************************
*/
void rtpClose( int* handle );

/*
*****************************************************************************
** FUNCTION:   rtpUpdateInfo
**
** PURPOSE:    This function prepares and updates current receiver report statistics
**
** PARAMETERS: handle     - [IN] RTP session handle
**             epRtpStats - [IN] RTP stats from DSP to be saved
**
** RETURNS:
**
** NOTE:
*****************************************************************************
*/
void rtpUpdateInfo(int handle, EPZCNXSTATS epRtpStats);


/*
*****************************************************************************
** FUNCTION:   rtpSend
**
** PURPOSE:    Send RTP (not RTCP) packet
**
** PARAMETERS: handle    - handle assigned by the RTP stack
**             rtpPacket - pointer to the RTP packet to be sent
**             length    - length of the RTP packet (i.e. packet + payload)
**             tos       - IPv4 TOS field to set
**
** RETURNS:    nothing
**
** NOTE:       rtpSetRTPRemote and rtpSetRTCPRemote must be called before
**             calling this function.
*****************************************************************************
*/
void rtpSend(int handle, RTPPACKET *rtpPacket, int length, unsigned char tos);


/*
*****************************************************************************
** FUNCTION:   rtpUpdateRecvInfo
**
** PURPOSE:    This function prepares and updates current receiver report statistics
**
** PARAMETERS: handle    - [IN] RTP session handle
**             rtpPacket - [IN] Pointer to the RTP packet to be unpacked
**             length    - [IN] Packet (i.e. header + payload) size in byte
**
** RETURNS:    BOOL - TRUE if the RTP packet is valid, FALSE otherwise
**
** NOTE:       This function must be called right after a RTP packet is received
**             (e.g., right after RTPRECVCB is called)
*****************************************************************************
*/
BOOL rtpUpdateRecvInfo(int handle, RTPPACKET *rtpPacket, UINT16 length);


/*
*****************************************************************************
** FUNCTION:   rtpUpdateSendInfo
**
** PURPOSE:    This function prepares and updates current sender report statistics
**
** PARAMETERS: handle    - [IN] RTP session handle
**             rtpPacket - [IN] Pointer to the packed RTP packet
**             length    - [IN] Packet (i.e. header + payload) size in byte
**
** RETURNS:    None
**
** NOTE:       This function must be called right before the packet is sent
**             (e.g., before rtpSend())
**
**             SSRC is inserted
**             Random offset to timestamp and sequence number is inserted
*****************************************************************************
*/
void rtpUpdateSendInfo(int handle, RTPPACKET *rtpPacket, UINT16 length);

/*
*****************************************************************************
** FUNCTION:   rtpCompareSSRC
**
** PURPOSE:    This function returns TRUE if current SSRC for the 
**             ingress / egress session matches the expected value
**
** PARAMETERS: handle    - [IN] RTP session handle
**             ssrc      - [IN] ssrc to match
**             ingress   - [IN] 0 - egress, 1 - ingress
**
** RETURNS:    None
**
** NOTE:       
*****************************************************************************
*/
BOOL rtpCompareSSRC(int handle, UINT32 ssrc);

/*
*****************************************************************************
** FUNCTION:   rtpSetMediaPortRange
**
** PURPOSE:    This function sets the media port range
**
** PARAMETERS: portNumMin   - [IN] The Min media port number
**             portNumMax   - [IN] The Max media port number
**
** RETURNS:    None
**
** NOTE:       This function must be called at least once before rtpGeMediaPortNums().
**
*****************************************************************************
*/
void rtpSetMediaPortRange(UINT16 portNumMin, UINT16 portNumMax);

/*
*****************************************************************************
** FUNCTION:   rtpGetMediaPortRange
**
** PURPOSE:    This function gets the media port range
**
** PARAMETERS: portNumMin - [OUT] The min media port number
**             portNumMax - [OUT] The max media port number
**
** RETURNS:    None
**
** NOTE:       
**
*****************************************************************************
*/
void rtpGetMediaPortRange(UINT16 * portNumMin, UINT16 * portNumMax);


/*
*****************************************************************************
** FUNCTION:   rtpGetMediaPortNums
**
** PURPOSE:    This function reserves and returns the media port numbers starting from
**             startPortNum to startPortNum + numOfPorts - 1.
**
** PARAMETERS: startPortNum - [OUT] Pointer to the starting media port number
**             numOfPorts   - [IN]  The total reserved contiguous media ports starting 
**                                  from startPortNum.
**
** RETURNS:    RTPSTATUS
**                RTP_SUCCESS
**                RTP_NORSC
**
** NOTE:       This function will create a record to keep the media port numbers 
**             allocated. rtpReleaseMediaPort() must be called to release the resources
**             when those port numbers are not in use.
**
*****************************************************************************
*/
RTPSTATUS rtpGetMediaPortNums(UINT16 *startPortNum, int numOfPorts);

/*
*****************************************************************************
** FUNCTION:   rtpReleaseMediaPortNums
**
** PURPOSE:    This function releases the media port numbers and resouces that
**             used to keep the record of the media port numbers set by 
**             rtpGetMdiaPortNums().
**
** PARAMETERS: startPortNum - [IN] The starting media port number to be released.
**                                  
**
** RETURNS:    None. 
**
** NOTE:       This function must be called to release the resources
**             when media port numbers are not in use.
**
*****************************************************************************
*/
void rtpReleaseMediaPortNums(UINT16 startPortNum);

/*****************************************************************************
** FUNCTION:   rtpCleanupMediaPortNums
**
** PURPOSE:    This function releases all media port numbers and resouces that
**             used to keep the record of the media port numbers set by 
**             rtpGetMdiaPortNums().
**
** PARAMETERS: None.
**                                  
**
** RETURNS:    None. 
**
** NOTE:       This function must be called to release the resources
**             when media port numbers are not in use.
**
*****************************************************************************
*/
void rtpCleanupMediaPortNums(void);

/*
*****************************************************************************
** FUNCTION:   rtcpInitStat
**
** PURPOSE:    This function initializes the RTCP statistics
**
** PARAMETERS: handle - [IN] RTP session handle
**
** RETURNS:    None
**
** NOTE:
*****************************************************************************
*/
void rtcpInitStat(int handle);

/*
 * *****************************************************************************
 * ** FUNCTION:   rtcpSendPacket
 * **
 * ** PURPOSE:    Send the RTCP packet
 * **
 * ** PARAMETERS: handle       - handle to the rtp session
 * **             rtcpPacket   - pointer to rtcp packet
 * **             pktsize      - length of rtcp packet
 * **
 * ** RETURNS:    nothing
 * **
 * *****************************************************************************
 * */
void rtcpSendPacket(int handle, RTPPACKET* rtcpPacket, int pktsize);

/*
*****************************************************************************
** FUNCTION:   rtpDeInitStack
**
** PURPOSE:    This function destroys the critical sections.
**
** PARAMETERS: None
**
** RETURNS:    None
**
** NOTE:
*****************************************************************************
*/
void rtpDeInitStack(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _RTP_H_ */
