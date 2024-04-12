/*
    Copyright 2000-2010 Broadcom Corporation
    
    <:label-BRCM:2011:DUAL/GPL:standard
    
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
*/                      

/**************************************************************************
 * File Name  : bcmnetlink.h
 *
 * Description: This file defines broadcom specific netlink message types
 ***************************************************************************/
#ifndef _BCMNETLINK_H
#define _BCMNETLINK_H

#include<linux/netlink.h>

#ifndef NETLINK_BRCM_MONITOR
#define NETLINK_BRCM_MONITOR 25
#endif

#ifndef NETLINK_BRCM_EPON
#define NETLINK_BRCM_EPON 26
#endif

/* message types exchanged using NETLINK_BRCM_MONITOR */
#define MSG_NETLINK_BRCM_WAKEUP_MONITOR_TASK 0X1000

#define MSG_NETLINK_BRCM_LINK_STATUS_CHANGED 0X2000

#define MSG_NETLINK_BRCM_LINK_TRAFFIC_TYPE_MISMATCH 0X4000

#define MSG_NETLINK_BRCM_SAVE_DSL_CFG		0X8000	/* Why the previous defined values are like bitmap???, just follow */
#define MSG_ID_BRCM_SAVE_DSL_CFG_ALL		0
#define MSG_ID_BRCM_SAVE_DSL_PREFERRED_LINE	1

#define MSG_NETLINK_BRCM_CALLBACK_DSL_DRV	0X0100

extern void kerSysSendtoMonitorTask(int msgType, char *msgData, int msgDataLen);

#ifdef MSTC_IPV6 // __MSTC__, KuanJung. also define in kernel/linux/net/ipv6/addrconf.c
#define MSG_NETLINK_BRCM_IPV6_SLAAC_ADDIP 0X5000
#endif // __MSTC__, KuanJung. also define in kernel/linux/net/ipv6/addrconf.c
#ifdef MSTC_IPV6 // __MSTC__, DingRuei, also define in kernel/linux/net/ipv6/ndisc.c
#define MSG_NETLINK_BRCM_IPV6_DEFAULT_ROUTER 0X6000
#endif

#ifdef MSTC_MODIFY//__MSTC__, FuChia
#define MSG_NETLINK_MTS_REBOOT             0X3000
#define MSG_NETLINK_MTS_RESET_DEFAULT      0X7000
#endif

#if defined(BUILD_MSTC_DSL_2492GNAU_B3BC) || defined(BUILD_NORWAY_CUSTOMIZATION) 
#define MSG_EXTSW_LED_OFF 0X9000 // NORWAY_BTN_LED_OFF
#define MSG_EXTSW_WLAN_LED_CTL 0Xb000 // NORWAY_WLAN_LED_CTL
#define MSG_NETLINK_BRCM_IP_UPDATE 0X0080
#define MSG_EXTSW_10MSWLED_En 0Xa000 // NORWAY_10MSWLED_En
#endif

#endif /*_BCMNETLINK_H */
