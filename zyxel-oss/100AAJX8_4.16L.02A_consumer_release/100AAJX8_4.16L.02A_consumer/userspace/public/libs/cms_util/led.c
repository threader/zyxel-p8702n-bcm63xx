/***********************************************************************
 *
 *  Copyright (c) 2006-2007  Broadcom Corporation
 *  All Rights Reserved
 *
<:label-BRCM:2012:DUAL/GPL:standard

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
 *
 ************************************************************************/

#include "cms.h"
#include "cms_util.h"
#include "cms_boardioctl.h"

#ifdef BUILD_TMPC4_CUSTOMIZATION
#include "boardparms.h"
#endif

/*
 * See:
 * bcmdrivers/opensource/include/bcm963xx/board.h
 * bcmdrivers/opensource/char/board/bcm963xx/impl1/board.c and bcm63xx_led.c
 */

void cmsLed_setWanConnected(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateOn, NULL);
}


void cmsLed_setWanDisconnected(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateOff, NULL);
}


void cmsLed_setWanFailed(void)
{
#ifdef BUILD_TMPC3_CUSTOMIZATION //Leo, for observa LED customization
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateRedFastBlinkContinues, NULL);
#else
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateFail, NULL);
#endif
}

#ifdef MSTC_MODIFY // __MSTC__, Richard Huang
void cmsLed_setInternetOn(void)
{
#ifdef BUILD_TMPC4_CUSTOMIZATION
   devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_22_AH, kLedStateOn, "");
#else
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedInternetData, kLedStateOn, NULL);
#endif
}

void cmsLed_setInternetOff(void)
{
#ifdef BUILD_TMPC4_CUSTOMIZATION
   devCtl_boardIoctl(BOARD_IOCTL_SET_GPIO, PERSISTENT, "", BP_GPIO_22_AH, kLedStateOff, "");
#else
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedInternetData, kLedStateOff, NULL);
#endif
}

#ifdef BUILD_TMPC3_CUSTOMIZATION //Leo, for observa LED customization
void cmsLed_setInternetFailed(void)
{
   devCtl_boardIoctl(BOARD_IOCTL_LED_CTRL, 0, NULL, kLedWanData, kLedStateRedFastBlinkContinues, NULL);
}
#endif
#endif
