/*
7   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2013:DUAL/GPL:standard
    
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
/******************************************************************************/
/*                                                                            */
/* File Description:                                                          */
/*                                                                            */
/* This file contains the definitions for Broadcom's 6838 Data path		  	  */
/* initialization sequence											          */
/*                                                                            */
/******************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Include files                                                             */
/*                                                                           */
/*****************************************************************************/
#ifndef __BCM63138_DATA_PATH_INIT_H
#define __BCM63138_DATA_PATH_INIT_H

#include "bl_os_wraper.h"


typedef enum
{
	WAN_TYPE_NONE = 0,
	WAN_TYPE_RGMII = 1,
	WAN_TYPE_DSL = 5
}E_WAN_TYPE;


typedef enum
{
	DPI_RC_OK,
	DPI_RC_ERROR
}E_DPI_RC;

/* TBD. Most/all of these parameters should not be needed. */
typedef struct
{
	E_WAN_TYPE	wanType;
	uint32_t	mtu_size;
	uint32_t	headroom_size;
	uint32_t	car_mode;
	uint32_t	ip_class_method;
	uint32_t	bridge_fc_mode;
}S_DPI_CFG;

uint32_t bcm63138_data_path_init(S_DPI_CFG *pCfg);
uint32_t bcm63138_data_path_go(void);
uint32_t bcm63138_data_path_shutdown(void);
void f_configure_bridge_port_sa_da(void);
void ResetUnresetRdpBlock(void);


#endif //#define __BCM63138_DATA_PATH_INIT_H
