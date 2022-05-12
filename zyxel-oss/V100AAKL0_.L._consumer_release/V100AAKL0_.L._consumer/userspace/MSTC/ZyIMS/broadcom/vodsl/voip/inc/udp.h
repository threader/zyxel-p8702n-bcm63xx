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
*    Filename: udp.h
*    Creation Date: 2000 29 June 1999 (v0.00)
*    VSS Info:
*        $Revision: 7 $
*        $Date: 5/03/01 12:21p $
*
****************************************************************************
*    Description:
*
*      Provides a set of functions for UDP communication using sockets.
*
****************************************************************************/

#ifndef _UDP_H
#define _UDP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <net.h>
#include <bosSocket.h>

/*
*****************************************************************************
** TYPEDEFS AND CONSTANTS
*****************************************************************************
*/

#define  SOCKET_ERROR  (-1)

/*
** Udp socket information
*/
typedef struct UDPSOCKET
{
   BOS_SOCKET handle;                     /* handle of the socket */
   BOS_SOCKET sendHandle;                 /* Handle of the send socket */
   BOS_SOCKET recvHandle;                 /* Handle of the recv socket */
   UINT16 destport;
   UINT16 localport;
   IPADDR localIpAddr;
   UINT16 fromport;
   unsigned long destaddr;
   unsigned long fromaddr;
} UDPSOCKET;


/*
*****************************************************************************
** FUNCTION:   udpMcastOpen
**
** PURPOSE:    Open an multicast socket for UDP
**
** PARAMETERS: localAddr  - local multicast address of socket
**             localPort  - local multicast port of socket
**             bBlockMode - blocking or non-blocking mode of seocket
**
** RETURNS:    socket descriptor if successful, otherwise SOCKET_ERROR (-1)
**
** NOTE:       Currently, bBlockMode is not used, and the UDP port / address
**             is always blocked.
**
*****************************************************************************
*/
int udpMcastOpen(UINT32 localAddr, UINT16 localPort, int bBlockMode);

/*
*****************************************************************************
** FUNCTION:   udpOpen
**
** PURPOSE:    Open UDP socket
**
** PARAMETERS: localPort  - local port of socket
**             bBlockMode - blocking or non-blocking mode of seocket
**
** RETURNS:    socket descriptor if successful, otherwise SOCKET_ERROR (-1)
**
*****************************************************************************
*/
int udpOpen(UINT16 localPort, int bBlockMode);

/*
*****************************************************************************
** FUNCTION:   udpRawOpen
**
** PURPOSE:    Open a RAW UDP socket for sending, normal for receiving.
**
** PARAMETERS: localPort  - local port of socket
**             bBlockMode - blocking or non-blocking mode of seocket
**             rx_d - (O) receive socket
**             tx_d - (O) send socket
**
** RETURNS:    udpRawOpen handle , if fail SOCKET_ERROR (-1)
**
*****************************************************************************
*/
int udpRawOpen(int localPort, int bBlockMode, BOS_SOCKET *rx_d, BOS_SOCKET *tx_d);

/*
*****************************************************************************
** FUNCTION:   udpRecv
**
** PURPOSE:    Receive UDP socket
**
** PARAMETERS: udpSock - udp socket port
**             buf     - pointer to receive buffer
**             length  - length of receive buffer
**             remAddr - pointer to remote udp socket address
**             remPort - pointer to remote udp socket port
**
** RETURNS:    size of receive buffer if data available, otherwise 0
**
*****************************************************************************
*/
int   udpRecv  (int udpSock, char* buf, int length, unsigned long* remAddr, UINT16* remPort );

/*
*****************************************************************************
** FUNCTION:   udpSend
**
** PURPOSE:    Send UDP socket
**
** PARAMETERS: udpSock - udp socket port
**             buf     - pointer to transmit buffer
**             length  - length of transmit buffer
**             remAddr - remote udp socket address
**             remPort - remote udp socket port
**
** RETURNS:    size of transmit buffer if successful, otherwise SOCKET_ERROR (-1)
**
*****************************************************************************
*/
int   udpSend  (int udpSock, char* buf, int length, unsigned long remAddr, UINT16 remPort);

/*
*****************************************************************************
** FUNCTION:   udpRawSend
**
** PURPOSE:    Send RAW UDP socket
**
** PARAMETERS: udpSock - udp socket port
**             buf     - pointer to transmit buffer
**             length  - length of transmit buffer
**             remAddr - remote udp socket address
**             remPort - remote udp socket port
**             tos - type of service
**
** RETURNS:    size of transmit buffer if successful,
**             otherwise SOCKET_ERROR (-1)
** NOTE:          tos field
**                bit 7 ~ bit 5 = precedence (0 = normal, 7 = extremely high)
**                bit 4 = D bit (minimize delay)
**                bit 3 = T bit (maximize throughput)
**                bit 2 = R bit (maximize reliability)
**                bit 1 = C bit (minimize transmission cost)
**                bit 0 = not used
**
*****************************************************************************
*/
int udpRawSend(int udpSock, char* buf, int length, unsigned long remAddr,
               UINT16 remPort, unsigned char tos);

/*
*****************************************************************************
** FUNCTION:   udpRawSetSendHandle
**
** PURPOSE:    set send handle for Raw socket
**
** PARAMETERS:  udpRawOpen handle
**              tx_d - send socket descriptor
**
** RETURNS:    0 - success
**             -1 - failure
**
*****************************************************************************
*/
int udpRawSetSendHandle(int rawHdl, BOS_SOCKET tx_d);

/*
*****************************************************************************
** FUNCTION:   udpClose
**
** PURPOSE:    Close UDP socket
**
** PARAMETERS: sockDesc - udp socket description
**
** RETURNS:    nothing
**
*****************************************************************************
*/
void  udpClose (int sockDesc);

/*
*****************************************************************************
** FUNCTION:   udpMcastClose
**
** PURPOSE:    Leave the specified multicast group and close the UDP socket
**
** PARAMETERS: sockDesc - udp socket description to close
**
**
** RETURNS:    nothing
**
** NOTES:      Leave multicast group before the socket is closed
**
*****************************************************************************
*/
void udpMcastClose( int sockDesc, UINT32 localAddr );

#ifdef __cplusplus
    }
#endif

#endif /* _UDP_H */

