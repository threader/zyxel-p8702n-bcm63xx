/*
<:copyright-BRCM:2007:DUAL/GPL:standard

   Copyright (c) 2007 Broadcom Corporation
   All Rights Reserved

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

/***************************************************************************
 * File Name  : xtmcfgdrv.h
 *
 * Description: This file contains the definitions and structures for the
 *              Linux IOCTL interface that used between the user mode
 *              bcmxtmcfg library and the kernel bcmxtmcfg driver.
 ***************************************************************************/

#if !defined(_XTMCFGDRV_H_)
#define _XTMCFGDRV_H_

#if defined(__cplusplus)
extern "C" {
#endif

/* Includes. */
#include <bcmxtmcfg.h>

/* Defines. */
#define XTMCFGDRV_MAJOR         228
#define XTMCFG_INVALID_FIELD    ((UINT32) -1)

#define XTMCFGIOCTL_INITIALIZE \
    _IOWR(XTMCFGDRV_MAJOR, 0, XTMCFGDRV_INITIALIZE)
#define XTMCFGIOCTL_UNINITIALIZE \
    _IOR(XTMCFGDRV_MAJOR, 1, XTMCFGDRV_STATUS_ONLY)
#define XTMCFGIOCTL_GET_TRAFFIC_DESCR_TABLE \
    _IOWR(XTMCFGDRV_MAJOR, 2, XTMCFGDRV_TRAFFIC_DESCR_TABLE)
#define XTMCFGIOCTL_SET_TRAFFIC_DESCR_TABLE \
    _IOWR(XTMCFGDRV_MAJOR, 3, XTMCFGDRV_TRAFFIC_DESCR_TABLE)
#define XTMCFGIOCTL_GET_INTERFACE_CFG \
    _IOWR(XTMCFGDRV_MAJOR, 4, XTMCFGDRV_INTERFACE_CFG)
#define XTMCFGIOCTL_SET_INTERFACE_CFG \
    _IOWR(XTMCFGDRV_MAJOR, 5, XTMCFGDRV_INTERFACE_CFG)
#define XTMCFGIOCTL_GET_CONN_CFG \
    _IOWR(XTMCFGDRV_MAJOR, 6, XTMCFGDRV_CONN_CFG)
#define XTMCFGIOCTL_SET_CONN_CFG \
    _IOWR(XTMCFGDRV_MAJOR, 7, XTMCFGDRV_CONN_CFG)
#define XTMCFGIOCTL_GET_CONN_ADDRS \
    _IOWR(XTMCFGDRV_MAJOR, 8, XTMCFGDRV_CONN_ADDRS)
#define XTMCFGIOCTL_GET_INTERFACE_STATISTICS \
    _IOWR(XTMCFGDRV_MAJOR, 9, XTMCFGDRV_INTERFACE_STATISTICS)
#define XTMCFGIOCTL_SET_INTERFACE_LINK_INFO \
    _IOWR(XTMCFGDRV_MAJOR, 10, XTMCFGDRV_INTERFACE_LINK_INFO)
#define XTMCFGIOCTL_SEND_OAM_CELL \
    _IOWR(XTMCFGDRV_MAJOR, 11, XTMCFGDRV_SEND_OAM_CELL)
#define XTMCFGIOCTL_CREATE_NETWORK_DEVICE \
    _IOWR(XTMCFGDRV_MAJOR, 12, XTMCFGDRV_CREATE_NETWORK_DEVICE)
#define XTMCFGIOCTL_DELETE_NETWORK_DEVICE \
    _IOWR(XTMCFGDRV_MAJOR, 13, XTMCFGDRV_DELETE_NETWORK_DEVICE)
#define XTMCFGIOCTL_REINITIALIZE \
    _IOWR(XTMCFGDRV_MAJOR, 14, XTMCFGDRV_STATUS_ONLY)
#define XTMCFGIOCTL_GET_BONDING_INFO \
    _IOWR(XTMCFGDRV_MAJOR, 15, XTMCFGDRV_BONDING_INFO)
#define XTMCFGIOCTL_SET_DS_PTMBONDING_DEVIATION \
    _IOWR(XTMCFGDRV_MAJOR, 16, XTMCFGDRV_PTMBONDINGCFG)
#define XTMCFGIOCTL_CONFIGURE \
    _IOWR(XTMCFGDRV_MAJOR, 17, XTMCFGDRV_CONFIGURE)

#ifdef MSTC_MODIFY //__MSTC__, Eric, VCAUTOHUNT
#define XTMCFGIOCTL_SEND_VCHUNT_TEST_PATTERN \
	_IOWR(XTMCFGDRV_MAJOR, 18, XTMCFGDRV_VCHUNT_TEST_CFG)
#define XTMCFGIOCTL_GET_VCHUNT_TEST_STATUS \
	_IOWR(XTMCFGDRV_MAJOR, 19, XTMCFGDRV_VCHUNT_TEST_CFG)  
#define XTMCFGIOCTL_SET_VCAUTOHUNT_INFO \
	_IOWR(XTMCFGDRV_MAJOR, 20, XTMCFGDRV_VCAUTOHUNT_INFO) 
#endif //__MSTC__, Eric, VCAUTOHUNT

#ifdef MSTC_MODIFY //__MSTC__, FuChia
#define XTMCFGIOCTL_GET_TXRATE_INFO \
    _IOWR(XTMCFGDRV_MAJOR, 21, XTMCFGDRV_TXRATE_INFO)   
#define MAX_TX_QUEUE 8
#endif //__MSTC__, FuChia


#ifdef MSTC_MODIFY //__MSTC__, Eric, VCAUTOHUNT
#define MAX_XTMCFGDRV_IOCTL_COMMANDS   22
#else
#define MAX_XTMCFGDRV_IOCTL_COMMANDS   18
#endif //__MSTC__, Eric, VCAUTOHUNT
/* Typedefs. */
typedef struct
{
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_STATUS_ONLY, *PXTMCFGDRV_STATUS_ONLY;

typedef struct
{   XTM_INITIALIZATION_PARMS Init;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_INITIALIZE, *PXTMCFGDRV_INITIALIZE;

typedef struct
{   XTM_CONFIGURATION_PARMS Config;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_CONFIGURE, *PXTMCFGDRV_CONFIGURE;

typedef struct
{
    PXTM_TRAFFIC_DESCR_PARM_ENTRY pTrafficDescrTable;
    UINT32 ulTrafficDescrTableSize;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_TRAFFIC_DESCR_TABLE, *PXTMCFGDRV_TRAFFIC_DESCR_TABLE;

typedef struct
{
    UINT32 ulPortId;
    XTM_INTERFACE_CFG InterfaceCfg;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_INTERFACE_CFG, *PXTMCFGDRV_INTERFACE_CFG;

typedef struct
{
    XTM_ADDR ConnAddr;
    XTM_CONN_CFG ConnCfg;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_CONN_CFG, *PXTMCFGDRV_CONN_CFG;

typedef struct
{
    PXTM_ADDR pConnAddrs;
    UINT32 ulNumConns;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_CONN_ADDRS, *PXTMCFGDRV_CONN_ADDRS;

typedef struct
{
    UINT32 ulPortId;
    XTM_INTERFACE_STATS Statistics;
    UINT32 ulReset;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_INTERFACE_STATISTICS, *PXTMCFGDRV_INTERFACE_STATISTICS;

typedef struct
{
    UINT32 ulPortId;
    XTM_INTERFACE_LINK_INFO LinkInfo;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_INTERFACE_LINK_INFO, *PXTMCFGDRV_INTERFACE_LINK_INFO;

typedef struct
{
    XTM_ADDR ConnAddr;
    XTM_OAM_CELL_INFO OamCellInfo;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_SEND_OAM_CELL, *PXTMCFGDRV_SEND_OAM_CELL;

typedef struct
{
    XTM_ADDR ConnAddr;
    char szNetworkDeviceName[NETWORK_DEVICE_NAME_SIZE];
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_CREATE_NETWORK_DEVICE, *PXTMCFGDRV_CREATE_NETWORK_DEVICE;

typedef struct
{
    UINT32 ulDeviation;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_PTMBONDINGCFG, *PXTMCFGDRV_PTMBONDINGCFG;


typedef struct
{
    XTM_ADDR ConnAddr;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_DELETE_NETWORK_DEVICE, *PXTMCFGDRV_DELETE_NETWORK_DEVICE;

typedef struct
{
    XTM_BOND_INFO info ;
    BCMXTM_STATUS bxStatus ;
} XTMCFGDRV_BONDING_INFO, *PXTMCFGDRV_BONDING_INFO;


#ifdef MSTC_MODIFY //__MSTC__, Eric, VCAUTOHUNT

typedef struct
{
    XTM_ADDR ConnAddr;
    XTM_VCHUNT_TEST_CFG vcHuntTestCfg;	
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_VCHUNT_TEST_CFG, *PXTMCFGDRV_VCHUNT_TEST_CFG;

typedef struct
{
    XTM_VCAUTOHUNT_INFO vcAutoHuntInfo;	
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_VCAUTOHUNT_INFO, *PXTMCFGDRV_VCAUTOHUNT_INFO;

#endif //__MSTC__, Eric, VCAUTOHUNT
#ifdef MSTC_MODIFY //__MSTC__, FuChia
typedef struct
{
	XTMRT_TXRATE txRate;
    BCMXTM_STATUS bxStatus;
} XTMCFGDRV_TXRATE_INFO, *PXTMCFGDRV_TXRATE_INFO;
#endif //__MSTC__, FuChia

#if defined(__cplusplus)
}
#endif

#endif // #if !defined(_XTMCFGDRV_H_)
