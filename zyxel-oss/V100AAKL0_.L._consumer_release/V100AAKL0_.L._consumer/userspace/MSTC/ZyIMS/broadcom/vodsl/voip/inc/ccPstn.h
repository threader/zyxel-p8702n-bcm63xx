/****************************************************************************
*
*  Copyright (c) 2001-2008 Broadcom Corporation
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
*  ccPstn.h
*
*  PURPOSE:
*
*     This file contains global definitions for the PSTN module,
*     which processes events. In case of a dial plan match, the PSTN
*     module seizes the PSTN line and dials the accumulated digits.
*     In case of on-hook event, the PSTN line is released.
*
*  NOTES:
*
****************************************************************************/

#if !defined( CC_PSTN_H )
#define CC_PSTN_H

/* ---- Include Files ---------------------------------------------------- */

#include <endpoint_user.h>
#include <vodslCfg.h>

/* ---- Constants and Types ---------------------------------------------- */

/* NUM_FXO_CHANNELS should be passed by the Makefile
** If NUM_FXO_CHANNELS is not defined, this means that PSTN is not supported. */
#ifndef NUM_FXO_CHANNELS
#define NUM_FXO_CHANNELS      0
#endif

#ifndef PSTN_SUPPORT
#   if (NUM_FXO_CHANNELS > 0)
#      define PSTN_SUPPORT          1
#   else
#      define PSTN_SUPPORT          0
#   endif
#endif

#define PSTN_INC_ROUTING_RULE_SELECT_LINE     "Line"
#define PSTN_INC_ROUTING_RULE_SELECT_VOIP     "Voip"
#define PSTN_INC_ROUTING_RULE_SELECT_AUTO     "Auto"
#define PSTN_CONNECTION_INDEX                 0xCAFE
#define PSTN_CTL_DO_NOT_SIGNAL_LINE           0xBEBE
#define PSTN_CTL_VALIDATE_CLID_RX             0xABBA
#define PSTN_CTL_CLID_RX_INVALID_MARKER       0xBABE
#define PSTN_CTL_3_WAY_CONFERENCE             0xBEEF
#define PSTN_CTL_DO_NOT_DISCONNECT_PRIMARY    0xBAAD

#define  ASCII_OFFSET            48
#define  MAXPSTNDIGITS           64
#define  CC_MAX_COMPILED_DMAP    512


/* --------------Structures and Callbacks----------------- */                                        
typedef enum CC_PSTN_RETCODE
{
   CC_PSTN_SUCCESS = 0,
   CC_PSTN_FAIL,
   CC_PSTN_PREFIX_NOT_MATCHED,
   CC_PSTN_PREFIX_MATCHED,
   CC_PSTN_CANT_SIGNAL,
   CC_PSTN_CALL_ANSWER,
   CC_PSTN_MAX_RETCODE
} CC_PSTN_RETCODE;


typedef enum CC_PSTN_SIG
{
   CC_PSTNSIG_SEIZE_LINE = 0,
   CC_PSTNSIG_RELEASE_LINE,
   CC_PSTNSIG_CALLSETUP,
   CC_PSTNSIG_ANSWER,
   CC_PSTNSIG_CONNECT,
   CC_PSTNSIG_CONNECT_IMMED,
   CC_PSTNSIG_DISCONNECT,
   CC_PSTNSIG_PSTNDISABLE,
   CC_PSTNSIG_PSTNENABLE,
   CC_PSTNSIG_MAX,

} CC_PSTN_SIG;


typedef enum CC_PSTN_EVT
{
   CC_PSTNEVT_NULL = 0,
   CC_PSTNEVT_DTMF0,
   CC_PSTNEVT_DTMF1,
   CC_PSTNEVT_DTMF2,
   CC_PSTNEVT_DTMF3,
   CC_PSTNEVT_DTMF4,
   CC_PSTNEVT_DTMF5,
   CC_PSTNEVT_DTMF6,
   CC_PSTNEVT_DTMF7,
   CC_PSTNEVT_DTMF8,
   CC_PSTNEVT_DTMF9,
   CC_PSTNEVT_DTMFS,
   CC_PSTNEVT_DTMFH,
   CC_PSTNEVT_DTMFA,
   CC_PSTNEVT_DTMFB,
   CC_PSTNEVT_DTMFC,
   CC_PSTNEVT_DTMFD,
   CC_PSTNEVT_OFFHOOK,
   CC_PSTNEVT_ONHOOK,
   CC_PSTNEVT_FLASH,
   CC_PSTNEVT_FAX,
   CC_PSTNEVT_MODEM,
   CC_PSTNEVT_DTMFL,
   CC_PSTNEVT_MEDIA,
   CC_PSTNEVT_TDD,
   CC_PSTNEVT_MAX,
} CC_PSTN_EVT;


typedef enum CC_PSTN_RING_STATE
{
   CC_PSTNRING_NO_RING,
   CC_PSTNRING_PRE_RING_CLIDRX,
   CC_PSTNRING_FIRST_RING,
   CC_PSTNRING_RINGING,

} CC_PSTN_RING_STATE;


typedef enum CC_PSTN_ROUTING_RULE
{
   CC_PSTNROUTING_AUTO,          /* Internal routing rule. */
   CC_PSTNROUTING_FORCED,        /* Internal routing rule. */
   CC_PSTNROUTING_VOIP,          /* External routing rule (route through VoIP). */

} CC_PSTN_ROUTING_RULE;

typedef struct CCDIALBUF
{
   char  digits[MAXPSTNDIGITS];      /* keypad digits */
} CCDIALBUF;

/* PSTN signal callback */
typedef CC_PSTN_RETCODE (*CCPSTN_SIGCB)(int endpt, CC_PSTN_SIG signal,
                                        unsigned int action, int value );

typedef struct CCPSTN_CTRLBLOCK
{
   CCPSTN_SIGCB           signalCallback;
   int                    routedEndpt;
   int                    confCnxId;
   CCDIALBUF              dialBuffer[VODSL_MAX_PHYS_ENDPT-NUM_FXO_CHANNELS];
   char                   dmap[CC_MAX_COMPILED_DMAP];
   int                    isPstnLineSeized;
   CC_PSTN_RING_STATE     ringState;
   VRG_CLIDRX_DATA        clid;
   CC_PSTN_ROUTING_RULE   incRoutingRule;
   int                    incRoutingForcedIx;
   int                    dtmfClidHead;
   int                    dtmfClidTail;
   int                    dtmfClidCollect;

} CCPSTN_CTRLBLOCK;


/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

/****************************************************************************
*
*  ccPstnInit
*
*  PURPOSE:
*     Initialize the PSTN module
*
*  PARAMETERS:
*     signalCallback [IN]  -  callback for sending PSTN signals to the endpoint
*     dialPlan       [IN]  -  dialPlan representing dialstrings, which activate
*                             the switch to PSTN line
*
*  RETURNS:
*     CC_PSTN_RETCODE
*
*  NOTES:
*
***************************************************************************/
CC_PSTN_RETCODE ccPstnInit( CCPSTN_SIGCB signalCallback, char* dialPlan );

/****************************************************************************
*
*  ccPstnDeInit
*
*  PURPOSE:
*     De-Initialize the PSTN module
*
*  PARAMETERS:
*
*  RETURNS:
*     CC_PSTN_RETCODE
*
*  NOTES:
*
***************************************************************************/
CC_PSTN_RETCODE ccPstnDeInit( void );

/****************************************************************************
*
*  ccPstnProcessEvent
*
*  PURPOSE:
*     Process event on an endpoint
*
*  PARAMETERS:
*     endpt    [IN]  -  endpoint on which the event occurred
*     event    [IN]  -  event that occurred
*
*  RETURNS:
*     CC_PSTN_RETCODE
*
*  NOTES:
*     If the event that occurred is a digit, which causes the dial string
*     for that endpoint match the PSTN dialplan, an attempt to seize the
*     PSTN line will occur.
*
*     If the eevent that occurred is an on-hook event, the dial string
*     for that endpoint will be reset and the PSTN line will be released
*     (if that endpoint is routed through the PSTN line)
*
***************************************************************************/
CC_PSTN_RETCODE ccPstnProcessEvent( int endpt, EPEVT event );

/****************************************************************************
*
*  ccPstnLastCallReturn
*
*  PURPOSE:
*     handles last call return event on an FXS endpt when last caller was from PSTN
*
*  PARAMETERS:
*     endpt          -  [IN]  FXS endpoint that is performing the last call return event
*     ds             -  [IN]  dialstring (dial string of last call)
*
*  RETURNS:
*
*  NOTES:
*
***************************************************************************/
void ccPstnLastCallReturn( int endpt, char* ds );

/****************************************************************************
*
*  ccPstnIsDmapMatch
*
*  PURPOSE:
*     Verify if the dialed numbers on the endpoint match the PSTN digit map (dial plan)
*
*  PARAMETERS:
*     endpt          -  [IN]  Endpoint id
*     dialString     -  [IN]  Accumulated dial string*
*
*  RETURNS:
*     TRUE if there is full dialstring match, FALSE otherwise
*
*  NOTES:
*
***************************************************************************/
int ccPstnIsDmapMatch( int endpt, char* dialString );


int ccPstnGetRoutedEndpt( void );
void ccPstnSetRoutedEndpt( int endpt );
void ccPstnClearRoutedEndpt();

int ccPstnValidateRing( EPEVT event );
void ccPstnEndRing( void );
CC_PSTN_RING_STATE ccPstnGetRingState();
void ccPstnClearRingState();

void ccPstnSaveCLID( VRG_CLIDRX_DATA *pCLID );
void ccPstnGetCLID( VRG_CLIDRX_DATA *pCLID );
void ccPstnProcessDtmfClidRx( int data );
void ccPstnSetDtmfClidRx( int head, int tail );

CC_PSTN_ROUTING_RULE ccPstnGetRouting( int *data );
void ccPstnSetRouting( char *type, char *data );

void ccPstnConnect( int cnxId );
void ccPstnDisconnect( int cnxId, int action );
void ccPstnLineRelease( void );
int ccPstnForward( int endpt, char *dialString );

void ccPstnResetDigits( int endpt );

#endif /* CC_PSTN_H */
