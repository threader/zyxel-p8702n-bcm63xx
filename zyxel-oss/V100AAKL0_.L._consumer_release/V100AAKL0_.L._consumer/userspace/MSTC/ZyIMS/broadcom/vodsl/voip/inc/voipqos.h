/****************************************************************************
*
*     Copyright (c) 2005 Broadcom Corporation
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
*  voipqos.c
*
*  PURPOSE:
*
*     API for ATM RX QoS for voice data - give priority to packets with certain dest. ports
*
*  NOTES:
*
****************************************************************************/

#ifndef _VOIPQOS_H_
#define _VOIPQOS_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h> 
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "voipSocketMsg.h"

#define CFMAPI_VOICEQOS_ADDPORT         1
#define CFMAPI_VOICEQOS_REMOVEPORT      2

struct VOIPQOSMSG {
    UINT16 action;
    UINT16 port;
};

#ifdef __cplusplus
extern "C" {
#endif

void voipqosRegPrioPort( UINT16 port );
void voipqosUnregPrioPort( UINT16 port );
void voipqosRegWanInfo( char *wanifc );


#ifdef __cplusplus
}
#endif

#endif
