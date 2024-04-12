/*
<:copyright-BRCM:2007:proprietary:standard

   Copyright (c) 2007 Broadcom Corporation
   All Rights Reserved

 This program is the proprietary software of Broadcom Corporation and/or its
 licensors, and may only be used, duplicated, modified or distributed pursuant
 to the terms and conditions of a separate, written license agreement executed
 between you and Broadcom (an "Authorized License").  Except as set forth in
 an Authorized License, Broadcom grants no license (express or implied), right
 to use, or waiver of any kind with respect to the Software, and Broadcom
 expressly reserves all rights in and to the Software and all intellectual
 property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
 NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
 BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.

 Except as expressly set forth in the Authorized License,

 1. This program, including its structure, sequence and organization,
    constitutes the valuable trade secrets of Broadcom, and you shall use
    all reasonable efforts to protect the confidentiality thereof, and to
    use this information only in connection with your use of Broadcom
    integrated circuit products.

 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
    ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
    FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
    COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
    TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
    PERFORMANCE OF THE SOFTWARE.

 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
    ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
    INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
    WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
    OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
    SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
    SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
    LIMITED REMEDY.
  :>
*/

#ifndef _BCM_EPON_CFG_H_
#define _BCM_EPON_CFG_H_

#include "bcmtypes.h"

#define GetEponDefHoldOverTime()      1000 // Unit is 1ms
#define GetEponDefHoldOverFlags()     HoldoverNoFlags

#define GetEponDefActivetime()        0  
#define GetEponDefStandbytime()       0

#define GetEponDefPsConfig()          PsConfigNone
#define GetEponDefPsMode()            PsModeInternal

#define GetEponDefShaperIpg()         FALSE

#define GetEponDefLinkNum()           1

#define GetEponDefDnRateMap(a)        PonRateOneG 
#define GetEponDefUpRateMap(a)        PonRateOneG
#define GetEponDefLaserOn(a)          0x0020
#define GetEponDefLaserOff(a)         0x0020
#define GetEponDefTxPolarity(a)       ActiveHi
#define GetEponDefOffTimeOffset(a)    0x0  //xnue:0x5
#define GetEponDefMaxFrameSize(a)     2000
#define GetEponDefPreDraft2dot1(a)    0
#define GetEponDefPowerupTime(a)      0
#define GetEponDefTxOffIdle(a)        1
#define GetEponDefNttReporting(a)     0
#define GetEponDefSchMode()           0
#define GetEponDefIdleTimeOffset()    0
#define GetEponDefFecRx()             0
#define GetEponDefFecTx()             0

#endif //_BCM_EPON_CFG_H_

