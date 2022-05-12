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
*  voipfirewall.h
*
*  PURPOSE:
*
*     API for fire wall for voice data - accept packets with certain dest. ports
*
*  NOTES:
*
****************************************************************************/

#ifndef _VOIPFIREWALL_H_
#define _VOIPFIREWALL_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h> 
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "voipSocketMsg.h"

/* call feature msg */
#define CFG_MAX_CF_NUMBER_STR_LEN           40 /* call feature data size, the size has to match the defination in cfmsocketapi.h */
struct VOIPCFMSG {
    UINT16 action;
    UINT16 endpt;
    UINT16 dataLen;
    UINT8  data[CFG_MAX_CF_NUMBER_STR_LEN];   /* call feature data */
};

/* call feature persist store action */

#define CFMAPI_ACTION_NONE                  0  /* empty action */
#define CFMAPI_VOICE_CF_NUMBER_STORE        1  /* store call forwarding number */
#define CFMAPI_VOICE_CF_BUSY_STORE          2  /* save call forwarding busy feature status */
#define CFMAPI_VOICE_CF_ALL_STORE           3  /* save call forwarding all feaure status */
#define CFMAPI_VOICE_CF_NA_STORE            4  /* save call forwarding no answer feature status */
#define CFMAPI_VOICE_CW_STORE               5  /* save call waiting feature status */

#ifdef __cplusplus
extern "C" {
#endif

void voipCallFeatureStore( UINT16 cmd, int endpt, void *data, int len );

#ifdef __cplusplus
}
#endif

#endif
