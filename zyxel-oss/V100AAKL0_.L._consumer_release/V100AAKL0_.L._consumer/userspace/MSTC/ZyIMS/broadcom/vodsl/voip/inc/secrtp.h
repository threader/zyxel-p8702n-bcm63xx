/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16251 Laguna Canyon Road
*             Irvine, California  92618
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: secrtp.h
*    Creation Date: 2 May 2000 (v0.00)
*    VSS Info:
*        $Revision: 2 $
*        $Date: 10/04/01 10:57a $
*
****************************************************************************
*    Description:
*
*      Security APIs
*
****************************************************************************/

#ifndef SECRTP_H
#define SECRTP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <env.h>
#include <hgcp.h>
#include <endpoint_user.h>

#define RTP_INITIALTSLENGTH    4
#define SHA1_KEYSIZE           20
#define MD5_KEYSEZE            16
#define RTP_SEEDSIZE           32
#define SHA1_ITERSIZE          (SHA1_KEYSIZE + RTP_SEEDSIZE)
#define RC4_KEYSIZE            16
#define RTP_INITIMESTAMP       4
#define RTP_MAXKEYSIZE         335    /* (40*8)+12+4-1, 40=max pktsize, 12=maxRTdr, 4=maxMACsize */
#define SPI_SIZE               4
#define MAX_SECRETSIZE         256
#define MAX_REKEY_COUNTER_SIZE 3      /* rekey up to 999 times */
#define RTP_REKEY_SEEDSIZE     32
#define AES_BLOCKSIZE 16
#define AES_KEYLENGTH 16


typedef enum SECRTPPCODES
{
   RTP_AES =   0x51,
   AUTH_NULL = 0x60,
   RTP_MMH_2 = 0x62,
   RTP_MMH_4 = 0x64
} SECRTPCODES;

typedef enum SECRTCPPCODES
{
   RTCP_AES_CBC   = 0x71,
   RTCP_HMAC_SHA1 = 0x81
} SECRTCPPCODES;


/* following functions are called by callclient and rtp */

/*
*****************************************************************************
** FUNCTION:   secRTPInit
**
** PURPOSE:    Initialization
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void secRTPInit( void );


/*
*****************************************************************************
** FUNCTION:   secCreateConnection
**
** PURPOSE:    set security parameters for create connection
**
** PARAMETERS: cxid     - connection id
**             cnxParam - pointer to connection parameters
**
** RETURNS:    none
**
*****************************************************************************
*/
void secCreateConnection(int cxid, MGZCNXP *cnxParam);


/*
*****************************************************************************
** FUNCTION:   secModifyConnection
**
** PURPOSE:    set security parameters for modify connection
**
** PARAMETERS: cxid     - connection id
**             cnxParam - pointer to connection parameters
**
** RETURNS:    none
**
*****************************************************************************
*/
void secModifyConnection(int cxid, MGZCNXP *cnxParam);


/*
*****************************************************************************
** FUNCTION:   secDeletConnection
**
** PURPOSE:    cleanup
**
** PARAMETERS: cxid - connection id
**
** RETURNS:    none
**
*****************************************************************************
*/
void secDeleteConnection(int cxid);


/*
*****************************************************************************
** FUNCTION:   secRTPEncrypt
**
** PURPOSE:    encrypt starting from state
**
** PARAMETERS: cxid      - connection id
**             rtpPacket - pointer to RTP packet
**             pLen      - length of RTP packet
**
** RETURNS:    0 if invalid packet, 1 if valid packet and encrypted properly
**
*****************************************************************************
*/
int secRTPEncrypt(int cxid, RTPPACKET* rtpPacket, int pLen);


/*
*****************************************************************************
** FUNCTION:   secRTPDecrypt
**
** PURPOSE:    decrypt packet starting from state
**
** PARAMETERS: cxid      - connection id
**             rtpPacket - pointer to RTP packet
**             pLen      - length of RTP packet
**
** RETURNS:    none
**
*****************************************************************************
*/
void secRTPDecrypt(int cxid, RTPPACKET* rtpPacket, int pLen);


/*
*****************************************************************************
** FUNCTION:   secRTPSign
**
** PURPOSE:    calculate and add mac to the message
**
** PARAMETERS: cxid      - connection id
**             rtpPacket - pointer to RTP packet
**             msglen    - length of the message
**
** RETURNS:
**
*****************************************************************************
*/
int secRTPSign(int cxid, RTPPACKET* rtpPacket, int msgLen);


/*
*****************************************************************************
** FUNCTION:   secRTPAuthenticate
**
** PURPOSE:    verify signature (mac)
**
** PARAMETERS: cxid      - connection id
**             rtpPacket - pointer to RTP packet
**             msglen    - length of the message
**
** RETURNS:
**
*****************************************************************************
*/
int secRTPAuthenticate(int cxid, RTPPACKET* rtpPacket, int msgLen);


/*
*****************************************************************************
** FUNCTION:   secGetSignatureSize
**
** PURPOSE:    Get the signature size
**
** PARAMETERS: cxid    - connection id
**             sndflag -
**
** RETURNS:    size of the mac key or -1 if not valid
**
*****************************************************************************
*/
int secGetSignatureSize(int cxid, int sndflag, SECRTPCODES rtpAuth);

#ifdef __cplusplus
}
#endif

#endif

