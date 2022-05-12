/****************************************************************************
*
*  Copyright (c) 2005-2008 Broadcom Corporation
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
*    Filename: voipAppDefs.h
*
****************************************************************************
*    Description:
*
*      This file contains definitions for VOIP application.  The purpose of
*      this file is to share common definitions used within the context of the
*      VOIP application as well as used outside of its scope but by third
*      party application which should share the same base data.  An example
*      of such third party application is the CFM (configuration manager) one.
*
****************************************************************************/

#ifndef VOIP_APP_DEFS
#define VOIP_APP_DEFS

/*
   RTP audio and data base ports.
*/
#define RTP_AUDIO_BASE_PORT      53456
#define RTP_AUDIO_MAX_PORT       63456
#define RTP_DATA_BASE_PORT       17384

/*
   SIP local port for the VODSL application.

   Note: if the VODSL is to co-exist with a SIP ALG application on the
         router then we need to assign a different local port number for
         VODSL SIP application to avoid conflicts with the SIP ALG application.
*/
#ifdef IP_SIP
#   ifdef EXTERNAL_SIP_ALG
#      define VODSL_SIP_LOCAL_PORT     5058
#   else
#      ifdef DSLX_SIG_TLS
#         define VODSL_SIP_LOCAL_PORT     5061
#      else
#         define VODSL_SIP_LOCAL_PORT     5060
#      endif
#   endif
#endif

#ifdef IP_MGCP
/* Default port number for MGCP client */
#  define VODSL_MGCP_LOCAL_PORT        2427
#endif 

typedef enum VOICE_APP_STATUS
{
   VOICE_APP_DEINITIALIZED = 0,
   VOICE_APP_INITIALIZED,
   VOICE_APP_INIT_IN_PROGRESS,
   VOICE_APP_DEINIT_IN_PROGRESS,
   VOICE_APP_STATUS_UNKNOWN,

} VOICE_APP_STATUS;

#define  VOICE_STATUS_DIR              "/var/voice"
#define  VOICE_STATUS_FILENAME         "status"

#define  VOICE_CONFIG_DIR              "/var/voipCfg"
#define  VOICE_CONFIG_FILENAME         "appConfig"
/* Maximum size of voice configuration data in bytes */
#define  VOICE_CONFIG_LENGTH           4096
/* Maximum number of voice configuration parameters * 
 * and option flags                                 */
#define  VOICE_MAX_CFGPARM             300

/* Max size of command issued via command line */
#define  VOICE_MAX_CLI_SZ              128

#define  VOICE_TIME_NEEDED_FOR_INIT    5
#define  VOICE_TIME_NEEDED_FOR_DEINIT  3
#define  VOICE_TIME_MAX_WAIT           6 

#endif /* VOIP_APP_DEFS */
