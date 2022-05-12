/****************************************************************************
*
*     Copyright (c) 2006 Broadcom Corporation
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
*  voipSocketMsg.h
*
*  PURPOSE:
*
*     Message definitions that used to communicate between VODSL and CFM.
*     Make sure those definitions are matched with cfmsocketapi.h.
*
*  NOTES:
*
****************************************************************************/

#ifndef _VOIPSOCKETMSG_H_
#define _VOIPSOCKETMSG_H_

#if !defined(BCMTYPES_H)
//typedef unsigned short UINT16;
#endif

struct CFMAPI_MSGHEADER {
    UINT16 type;
};


/* none action */
#define CFMAPI_ACTION_NONE              0  /* no action */

/* MSG Type */
#define CFMAPI_APP_VOICEQOS             1
#define CFMAPI_APP_VOICEQOS_WANINFO     2
#define CFMAPI_APP_VOICEFIREWALL        3
#define CFMAPI_APP_VOICECALLFEATURE     4  /* message type  defination */

#endif

