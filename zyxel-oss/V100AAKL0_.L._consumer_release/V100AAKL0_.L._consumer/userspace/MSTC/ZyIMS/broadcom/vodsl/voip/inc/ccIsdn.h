/****************************************************************************
*
*     Copyright (c) 2001 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16251 Laguna Canyon Road
*           Irvine, California  92618
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  ccIsdn.h
*
*  PURPOSE:
*
*     
*     
*     
*     
*
*  NOTES:
*
****************************************************************************/

#if !defined( CC_ISDN_H )
#define CC_ISDN_H

/* ---- Include Files ---------------------------------------------------- */
#include <endpoint_user.h>

/* ---- Constants and Types ---------------------------------------------- */
bool bIsdnConnected;

typedef enum
{
   CCISDNCMD_INITCALL,
   CCISDNCMD_CONNECT,
   CCISDNCMD_DISCONNECT,
   CCISDNCMD_KEEPALIVE,
   CCISDNCMD_SHUTDOWN,
   CCISDNCMD_UNKNOWN
} CCISDNCMD;

/* ---- Prototypes ------------------------------------------------------- */
BOOL isdnIsDmapMatch( int endpt, char* dialString );
void ccIsdnSendCmd( int endpt, CCISDNCMD cmd, void * params );
void isdnInit ( void );
void isdnDeInit( void );

#endif /* CC_ISDN_H */
