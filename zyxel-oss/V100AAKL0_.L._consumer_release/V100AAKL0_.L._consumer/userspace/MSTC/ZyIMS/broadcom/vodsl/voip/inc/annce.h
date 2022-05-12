/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: annce.h
*
****************************************************************************
*    Description:
*
*     Define types for annoucement data
****************************************************************************/
#ifndef ANNCE_H
#define ANNCE_H

typedef struct ANNCEINFO
{
   int eptCodec;
   UINT8 pktPeriod;
   UINT8 rtpCode;
   UINT32 length;
   const char *bufp;
} ANNCEINFO;

/* The index of the announceInfo array below to use for each announcement reason.
 * Add reasons before NUM_ANNCERSN. */
typedef enum
{
   ANNCERSN_DISABLED = 0,
   ANNCERSN_DISCONNECTED,
   ANNCERSN_NOT_OPERATIONAL,
   NUM_ANNCERSN        /* Be sure it's the last */
} ANNCERSN;

typedef enum
{
   ANNCECMD_PLAY,
   ANNCECMD_STOP
} ANNCECMD;

#endif
