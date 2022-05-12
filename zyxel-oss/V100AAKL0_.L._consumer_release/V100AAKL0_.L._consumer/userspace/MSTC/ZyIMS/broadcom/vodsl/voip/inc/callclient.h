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
*    Filename: callclient.h
*    Creation Date: 2000 27 June 1999 (v0.00)
*    VSS Info:
*        $Revision: 8 $
*        $Date: 6/12/01 11:24a $
*
****************************************************************************
*    Description:
*
*      This file contains the prototypes for the Call Client Interface
*
****************************************************************************/

#ifndef _CALLCLIENT_H
#define _CALLCLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <voipLocale.h>
#include <env.h>
#include <ccPstn.h>

#define MAX_HOST_STR_LEN         80
#define MAX_IPADDR_STR_LEN       20
#define MAX_IFNAME_STR_LEN       20
#define MAX_CODEC_STR_LEN        8
#define MAX_PSWD_STR_LEN         16
#define MAX_COUNTRY_STR_LEN      16
#define MAX_DIALPLAN_LEN         128
#define MAX_EXT_STR_LEN          16
#define MAX_LOCALENAME_STR_LEN   4
#define MAX_CODECNAME_STR_LEN    6
#define MAX_PSTN_ROUTEDATA       256


#if defined(IP_MGCP)

#define MGCP_MAX_GWNAME_STR_LEN      40
#define MGCP_MAX_TRANSPORT_STR_LEN   12

/* Event that can be published by the call client to the outside world */
typedef enum CC_PUBEVT
{
   CC_PUBEVT_ENDPT_INITIALIZED,
   CC_PUBEVT_ENDPT_DEINITIALIZED,
   CC_PUBEVT_MAX
} CC_PUBEVT;

/* Call back function used by call manager to publish events to outside world */
typedef void (*publishEventCallback)( int endpt, CC_PUBEVT event );

typedef struct MGCP_STARTUP_PARMS
{
   char     caIpAddrStr[MAX_IPADDR_STR_LEN]; /* Call Agent IP address string */
   char     gwName[MGCP_MAX_GWNAME_STR_LEN]; /* Gateway name */
   char     ifAddrStr[MAX_IFNAME_STR_LEN];   /* Interface address (eth0, nas25, etc.) */
   char     prefCodecStr[MAX_CODEC_STR_LEN]; /* Preferred codec (g711, g726, etc.) */
   char     countryStr[MAX_CODEC_STR_LEN];   /* Country string (USA, CHN, etc.) */
   int      countryCode;                     /* Country code for VRG */
#if PSTN_SUPPORT
   char     pstnRouteRule[MAX_CODEC_STR_LEN];/* PSTN routing rule for incoming calls */
   char     pstnRouteData[MAX_PSTN_ROUTEDATA];  /* PSTN routing data for incoming calls */
   char     pstnDialPlan[MAX_DIALPLAN_LEN];  /* PSTN dial plan */
#endif /* PSTN_SUPPORT */

   publishEventCallback    publishEventCB;

} MGCP_STARTUP_PARMS;

#endif

/*
*****************************************************************************
** EXTERNAL FUNCTION PROTOTYPES
*****************************************************************************
*/

#if defined(IP_MGCP)
/*
*****************************************************************************
** FUNCTION:   callClient
**
** PURPOSE:    This function is used to initialize and run the MGCP call client
**
** PARAMETERS: Priority of the main "CallClient" task. If it's zero,
**             the priority of the calling (current) task will be taken.
**
** RETURNS:    TaskID if OK,  0 - otherwise.
**
** NOTE:
*****************************************************************************
*/
UINT32   callClient( MGCP_STARTUP_PARMS* startupParms );

#elif ( defined(IP_H323) || defined(IP_SIP) )
/*
*****************************************************************************
** FUNCTION:   callClient
**
** PURPOSE:    This function is used to initialize and run the H323 call client
**
** PARAMETERS:
**
** RETURNS:    TaskID if OK,  0 - otherwise.
**
** NOTE:
*****************************************************************************
*/
UINT32 callClient( void );
#endif

/*
*****************************************************************************
** FUNCTION:   PrintVersionInfo
**
** PURPOSE:    Printing MTA CallClient version info
**
** PARAMETERS: none
**
** RETURNS:    NONE
**
** NOTE:
*****************************************************************************
*/
void     PrintVersionInfo(void);

#ifdef __cplusplus
    }
#endif

#endif  /* _CALLCLIENT_H  */

