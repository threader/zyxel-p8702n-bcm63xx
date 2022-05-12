/*
 Copyright 2004-2010 Broadcom Corp. All Rights Reserved.

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
#ifndef _ETHSW_H_
#define _ETHSW_H_

#include "bcmenet_common.h"
#include "bcmmii.h"
#if !defined(RDPA_PLATFORM)
#include "ethsw_dma.h"
#else
#include "ethsw_runner.h"
#endif


/****************************************************************************
    Prototypes
****************************************************************************/

int ethsw_setup_led(void);
int ethsw_reset_ports(struct net_device *dev);
int ethsw_set_mac(int logical_port, PHY_STAT ps);
int ethsw_phy_intr_ctrl(int port, int on);
PHY_STAT ethsw_phyid_stat(int phyId);
PHY_STAT ethsw_phy_stat(int unit, int port, int cb_port);
void ethsw_switch_power_off(void* context);
void ethsw_init_config(int unit, uint32_t port_map,  int wanPort);
int ethsw_setup_phys(void);
void ethsw_phy_handle_exception_cases(void); /* Code to handle exceptions and chip specific cases */
void ethsw_phy_apply_init_bp(void);
int ethsw_add_proc_files(struct net_device *dev);
int ethsw_del_proc_files(void);
int ethsw_enable_sar_port(void);
int ethsw_disable_sar_port(void);
int ethsw_save_port_state(void);
int ethsw_restore_port_state(void);
void ethsw_port_based_vlan(int port_map, int wan_port_bit, int txSoftSwitchingMap);
int ethsw_enable_hw_switching(void);
int ethsw_disable_hw_switching(void);
int ethsw_set_hw_switching(uint32 state);
int ethsw_get_hw_switching_state(void);
int ethsw_phy_pll_up(int ephy_and_gphy);
uint32 ethsw_ephy_auto_power_down_wakeup(void);
uint32 ethsw_ephy_auto_power_down_sleep(void);

void ethsw_phyport_wreg(int port, int reg, uint16 *data);
void ethsw_phyport_rreg(int port, int reg, uint16 *data);
#if defined(GPHY_EEE_1000BASE_T_DEF) || defined(CONFIG_BCM_EXT_SWITCH)
void ethsw_phyport_c45_rreg(int log_port, int regg, int regr, uint16 *pdata16);
void ethsw_phyport_c45_wreg(int log_port, int regg, int regr, uint16 *pdata16);
#endif

void ethsw_phy_advertise_all(uint32 phy_id);
void ethsw_isolate_phy(int phyId, int isolate);
void ethsw_phy_config(void);
void ethsw_init_table(BcmEnet_devctrl *pDevCtrl);
int  bcmeapi_ioctl_ethsw_arl_access(struct ethswctl_data *e);
int bcmeapi_ioctl_ethsw_port_mirror_set(struct ethswctl_data *e);
int bcmeapi_ioctl_ethsw_port_mirror_get(struct ethswctl_data *e);
#if defined(CONFIG_BCM_SWITCH_PORT_TRUNK_SUPPORT)
int bcmeapi_ioctl_ethsw_port_trunk_set(struct ethswctl_data *e);
int bcmeapi_ioctl_ethsw_port_trunk_get(struct ethswctl_data *e);
#endif /* CONFIG_BCM_SWITCH_PORT_TRUNK_SUPPORT */
#if defined(CONFIG_BCM96828) && !defined(CONFIG_EPON_HGU)
void saveEthPortToRxIudmaConfig(uint8 port, uint8 iudma);
int restoreEthPortToRxIudmaConfig(uint8 port);
int enet_learning_ctrl(uint32_t portMask, uint8_t enable);
int bcm_fun_enet_drv_handler(void *ptr);
int epon_uni_to_uni_ctrl(unsigned int portMap, int val);
#endif

#endif /* _ETHSW_H_ */
