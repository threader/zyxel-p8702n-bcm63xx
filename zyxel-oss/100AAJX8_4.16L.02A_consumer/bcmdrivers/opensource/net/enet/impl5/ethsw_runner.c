/*
 Copyright 2007-2010 Broadcom Corp. All Rights Reserved.

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

#define _BCMENET_LOCAL_

#include <linux/types.h>
#include <linux/delay.h>
#include <linux/mii.h>
#include <linux/stddef.h>
#include <linux/ctype.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <board.h>
#include "boardparms.h"
#include <bcm_map_part.h>
#include "bcm_intr.h"
#include "bcmenet.h"
#include "bcmmii.h"
#if defined(CONFIG_BCM96838)
#include "phys_common_drv.h"
#include "egphy_drv.h"
#endif
#include <rdpa_api.h>
#include "bcmswshared.h"
#include "ethsw.h"
#include "ethsw_phy.h"
#include "bcmsw_runner.h"

extern struct semaphore bcm_ethlock_switch_config;

#if defined(SWITCH_REG_SINGLE_SERDES_CNTRL)
enum
{
    SFP_NO_MODULE,
    SFP_FIBER,
    SFP_COPPER,
};

static int sfp_module_type = SFP_NO_MODULE;
void ethsw_sfp_module_phy(int phyId)
{
    u32 val32;

    val32 = *(u32*)SWITCH_SINGLE_SERDES_STAT;
    if ((val32 & SWITCH_REG_SSER_RXSIG_DET) == 0)
    {
        if(sfp_module_type != SFP_NO_MODULE)
        {
            sfp_module_type = SFP_NO_MODULE;
            printk("SFP module unplugged\n");
        }
        return;
    }

    if (sfp_module_type == SFP_NO_MODULE)
    {
#if defined(CONFIG_I2C)
        if (sfp_i2c_phy_read(0, &val32))
        {
            sfp_module_type = SFP_COPPER;


            sfp_i2c_phy_write(0x0, 0x8140);     /* Software reset */

            /* Configure SPF PHY into SGMII mode */
            sfp_i2c_phy_write(0x1b, 0x9084);    /* Enable SGMII mode */
            sfp_i2c_phy_write(0x9, 0x0f00);     /* Advertise 1kBase-T Full/Half-Duplex */
            sfp_i2c_phy_write(0x0, 0x8140);     /* Software reset */
            sfp_i2c_phy_write(0x4, 0x0de1);     /* Adverstize 100/10Base-T Full/Half-Duplex */
            sfp_i2c_phy_write(0x0, 0x9140);     /* Software reset */

            /* Configure Serdes into SGMII mode */
            ethsw_config_serdes_sgmii(phyId);

            printk("Copper SFP Module Plugged in\n");
        }
        else
#endif
        {
            /* Configure Serdes into 1000Base-X mode */
            sfp_module_type = SFP_FIBER;
            ethsw_config_serdes_1kx(phyId);

#if defined(CONFIG_I2C)
            printk("Fibre SFP Module Plugged in\n");
#else
            printk("SFP Module Plugged in\n");
#endif
        }
    }
}
#endif

#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
PHY_STAT ethsw_phyid_stat(int phyId) /* FIXME  - duplicate code; Merge together with ethsw_dma.c */
{
    PHY_STAT ps;
    uint16 v16;
    uint16 ctrl;
    uint16 mii_esr = 0;
    uint16 mii_stat = 0, mii_adv = 0, mii_lpa = 0;
    uint16 mii_gb_ctrl = 0, mii_gb_stat = 0;

    memset(&ps, 0, sizeof(ps));
    if (!IsPhyConnected(phyId))
    {
        ps.lnk = 1;
        ps.fdx = 1;
        if (IsMII(phyId))
            ps.spd100 = 1;
        else
            ps.spd1000 = 1;
        return ps;
    }

    down(&bcm_ethlock_switch_config);

    if((phyId & MAC_IFACE) == MAC_IF_SERDES)
    {
#if defined(SWITCH_REG_SINGLE_SERDES_CNTRL)
        ethsw_sfp_module_phy(phyId);
#endif

        /* Read twice based on ASIC team reqeust */
        ethsw_phy_rreg(phyId, MII_STATUS, &v16);
        ethsw_phy_rreg(phyId, MII_STATUS, &v16);
        ps.lnk = (v16 & MII_STATUS_LINK) != 0;

        if(ps.lnk)
        {
            ethsw_phy_exp_rreg(phyId, MIIEX_DIGITAL_STATUS_1000X, &v16);
            ps.spd1000 = (v16 & MIIEX_SPEED) == MIIEX_SPD1000;
            ps.spd100 = (v16 & MIIEX_SPEED) == MIIEX_SPD100;
            ps.spd10 = (v16 & MIIEX_SPEED) == MIIEX_SPD10;
            ps.fdx = (v16 & MIIEX_DUPLEX) > 0;
        }

        goto end;
    }

    ethsw_phy_rreg(phyId, MII_INTERRUPT, &v16);
    ethsw_phy_rreg(phyId, MII_ASR, &v16);
    BCM_ENET_DEBUG("%s mii_asr (reg 25) 0x%x\n", __FUNCTION__, v16);


    if (!MII_ASR_LINK(v16)) {
        goto end;
    }

    ps.lnk = 1;

    ethsw_phy_rreg(phyId, MII_BMCR, &ctrl);

    if (!MII_ASR_DONE(v16)) {
        ethsw_phy_rreg(phyId, MII_BMCR, &ctrl);
        if (ctrl & BMCR_ANENABLE) {
            up(&bcm_ethlock_switch_config);
            return ps;
        }
        // auto-negotiation disabled
        ps.fdx = (ctrl & BMCR_FULLDPLX) ? 1 : 0;
        if((ctrl & BMCR_SPEED100) && !(ctrl & BMCR_SPEED1000))
            ps.spd100 = 1;
        else if(!(ctrl & BMCR_SPEED100) && (ctrl & BMCR_SPEED1000))
            ps.spd1000 = 1;

        goto end;
    }

    //Auto neg enabled (this end) cases
    ethsw_phy_rreg(phyId, MII_ADVERTISE, &mii_adv);
    ethsw_phy_rreg(phyId, MII_LPA, &mii_lpa);
    ethsw_phy_rreg(phyId, MII_BMSR, &mii_stat);

    BCM_ENET_DEBUG("%s mii_adv 0x%x mii_lpa 0x%x mii_stat 0x%x mii_ctrl 0x%x \n", __FUNCTION__,
           mii_adv, mii_lpa, mii_stat, v16);
    // read 1000mb Phy  registers if supported
    if (mii_stat & BMSR_ESTATEN) { 

        ethsw_phy_rreg(phyId, MII_ESTATUS, &mii_esr);
        if (mii_esr & (1 << 15 | 1 << 14 |
                       ESTATUS_1000_TFULL | ESTATUS_1000_THALF))
            ethsw_phy_rreg(phyId, MII_CTRL1000, &mii_gb_ctrl);
            ethsw_phy_rreg(phyId, MII_STAT1000, &mii_gb_stat);
    }

    mii_adv &= mii_lpa;

    if ((mii_gb_ctrl & ADVERTISE_1000FULL) &&  // 1000mb Adv
            (mii_gb_stat & LPA_1000FULL))
    {
        ps.spd1000 = 1;
        ps.fdx = 1;
    } else if ((mii_gb_ctrl & ADVERTISE_1000HALF) && 
            (mii_gb_stat & LPA_1000HALF))
    {
        ps.spd1000 = 1;
        ps.fdx = 0;
    } else if (mii_adv & ADVERTISE_100FULL) {  // 100mb adv
        ps.spd100 = 1;
        ps.fdx = 1;
    } else if (mii_adv & ADVERTISE_100BASE4) {
        ps.spd100 = 1;
        ps.fdx = 0;
    } else if (mii_adv & ADVERTISE_100HALF) {
        ps.spd100 = 1;
        ps.fdx = 0;
    } else if (mii_adv & ADVERTISE_10FULL) {
        ps.fdx = 1;
    }
end:
    up(&bcm_ethlock_switch_config);
    return ps;
}
#endif /* 963138 || 963148 */

#ifdef CONFIG_BCM96838
PHY_STAT ethsw_phy_stat(int unit, int port, int cb_port) /* FIXME  - very similar code/functionality; Merge together with the above */
{
    PHY_STAT phys;
    PHY_RATE curr_phy_rate;
    int phyId;

    memset(&phys, 0, sizeof(phys));
    phyId = enet_sw_port_to_phyid(0, port);
    if (!IsPhyConnected(phyId))
    {
        // 0xff PHY ID means no PHY on this port.
        phys.lnk = 1;
        phys.fdx = 1;
        if (IsMII(phyId))
            phys.spd100 = 1;
        else
            phys.spd1000 = 1;
        return phys;
    }

    curr_phy_rate = PhyGetLineRateAndDuplex(port);
    phys.lnk = curr_phy_rate < PHY_RATE_LINK_DOWN;
    switch (curr_phy_rate)
    {
    case PHY_RATE_10_FULL:
        phys.fdx = 1;
        break;
    case PHY_RATE_10_HALF:
        break;
    case PHY_RATE_100_FULL:
        phys.fdx = 1;
        phys.spd100 = 1;
        break;
    case PHY_RATE_100_HALF:
        phys.spd100 = 1;
        break;
    case PHY_RATE_1000_FULL:
        phys.spd1000 = 1;
        phys.fdx = 1;
        break;
    case PHY_RATE_1000_HALF:
        phys.spd1000 = 1;
        break;
    case PHY_RATE_LINK_DOWN:
        break;
    default:
        break;
    }
    return phys;
}
#endif

int ethsw_reset_ports(struct net_device *dev)
{
    return 0;
}

int ethsw_add_proc_files(struct net_device *dev)
{
    return 0;
}

int ethsw_del_proc_files(void)
{
    return 0;
}

int ethsw_disable_hw_switching(void)
{
    return 0;
}

void ethsw_dump_page(int page)
{
}

void fast_age_port(uint8_t port, uint8_t age_static)
{
}

int ethsw_counter_collect(uint32_t portmap, int discard)
{
    return 0;
}

void ethsw_port_based_vlan(int port_map, int wan_port_map, int txSoftSwitchingMap)
{
}

void ethsw_get_txrx_imp_port_pkts(unsigned int *tx, unsigned int *rx)
{
}
EXPORT_SYMBOL(ethsw_get_txrx_imp_port_pkts);

int ethsw_phy_intr_ctrl(int port, int on)
{
    return 0;
}
void ethsw_phy_apply_init_bp(void)
{
}
int ethsw_setup_led(void)
{
    return 0;
}
int ethsw_setup_phys(void)
{
    return 0;
}
int ethsw_enable_hw_switching(void)
{
    return 0;
}

/* Code to handle exceptions chip specific cases */
void ethsw_phy_handle_exception_cases(void)
{
}

int bcmeapi_ethsw_dump_mib(int port, int type)
{
    int					rc;
    rdpa_emac_stat_t 	emac_cntrs;
    bdmf_object_handle 	port_obj = NULL;
    rdpa_if             rdpa_port = rdpa_if_lan0 + (rdpa_if)port;

    rc = rdpa_port_get(rdpa_port,&port_obj);
    if ( rc != BDMF_ERR_OK)
    {
        printk("failed to get rdpa port object rc=%d\n",rc);
        return -1;
    }

    rc = rdpa_port_emac_stat_get (port_obj, &emac_cntrs);
    if ( rc != BDMF_ERR_OK)
    {
    	printk("failed to get statistic counters from RDPA layer\n");
    	bdmf_put(port_obj);
    	return -1;
    }

    bdmf_put(port_obj);
    printk("\nRunner Stats : Port# %d\n",port);

    /* Display Tx statistics */
    printk("\n");
    printk("TxUnicastPkts:          %10u \n", (unsigned int)emac_cntrs.tx.packet);
    printk("TxMulticastPkts:        %10u \n", (unsigned int)emac_cntrs.tx.multicast_packet);
    printk("TxBroadcastPkts:        %10u \n", (unsigned int)emac_cntrs.tx.broadcast_packet);
    printk("TxDropPkts:             %10u \n", (unsigned int)emac_cntrs.tx.error);

    /* Display remaining tx stats only if requested */
    if (type) {
        printk("TxBytes:                %10u \n", (unsigned int)emac_cntrs.tx.byte);
        printk("TxFragments:            %10u \n", (unsigned int)emac_cntrs.tx.fragments_frame);
        printk("TxCol:                  %10u \n", (unsigned int)emac_cntrs.tx.total_collision);
        printk("TxSingleCol:            %10u \n", (unsigned int)emac_cntrs.tx.single_collision);
        printk("TxMultipleCol:          %10u \n", (unsigned int)emac_cntrs.tx.multiple_collision);
        printk("TxDeferredTx:           %10u \n", (unsigned int)emac_cntrs.tx.deferral_packet);
        printk("TxLateCol:              %10u \n", (unsigned int)emac_cntrs.tx.late_collision);
        printk("TxExcessiveCol:         %10u \n", (unsigned int)emac_cntrs.tx.excessive_collision);
        printk("TxPausePkts:            %10u \n", (unsigned int)emac_cntrs.tx.pause_control_frame);
        printk("TxExcessivePkts:        %10u \n", (unsigned int)emac_cntrs.tx.excessive_deferral_packet);
        printk("TxJabberFrames:         %10u \n", (unsigned int)emac_cntrs.tx.jabber_frame);
        printk("TxFcsError:             %10u \n", (unsigned int)emac_cntrs.tx.fcs_error);
        printk("TxCtrlFrames:           %10u \n", (unsigned int)emac_cntrs.tx.control_frame);
        printk("TxOverSzFrames:         %10u \n", (unsigned int)emac_cntrs.tx.oversize_frame);
        printk("TxUnderSzFrames:        %10u \n", (unsigned int)emac_cntrs.tx.undersize_frame);
        printk("TxUnderrun:             %10u \n", (unsigned int)emac_cntrs.tx.underrun);
        printk("TxPkts64Octets:         %10u \n", (unsigned int)emac_cntrs.tx.frame_64);
        printk("TxPkts65to127Octets:    %10u \n", (unsigned int)emac_cntrs.tx.frame_65_127);
        printk("TxPkts128to255Octets:   %10u \n", (unsigned int)emac_cntrs.tx.frame_128_255);
        printk("TxPkts256to511Octets:   %10u \n", (unsigned int)emac_cntrs.tx.frame_256_511);
        printk("TxPkts512to1023Octets:  %10u \n", (unsigned int)emac_cntrs.tx.frame_512_1023);
        printk("TxPkts1024to1518Octets: %10u \n", (unsigned int)emac_cntrs.tx.frame_1024_1518);
        printk("TxPkts1519toMTUOctets:  %10u \n", (unsigned int)emac_cntrs.tx.frame_1519_mtu);
    }

    /* Display Rx statistics */
    printk("\n");
    printk("RxUnicastPkts:          %10u \n", (unsigned int)emac_cntrs.rx.packet);
    printk("RxMulticastPkts:        %10u \n", (unsigned int)emac_cntrs.rx.multicast_packet);
    printk("RxBroadcastPkts:        %10u \n", (unsigned int)emac_cntrs.rx.broadcast_packet);

    /* Display remaining rx stats only if requested */
    if (type) {
        printk("RxBytes:                %10u \n", (unsigned int)emac_cntrs.rx.byte);
        printk("RxJabbers:              %10u \n", (unsigned int)emac_cntrs.rx.jabber);
        printk("RxAlignErrs:            %10u \n", (unsigned int)emac_cntrs.rx.alignment_error);
        printk("RxFCSErrs:              %10u \n", (unsigned int)emac_cntrs.rx.fcs_error);
        printk("RxFragments:            %10u \n", (unsigned int)emac_cntrs.rx.fragments);
        printk("RxOversizePkts:         %10u \n", (unsigned int)emac_cntrs.rx.oversize_packet);
        printk("RxUndersizePkts:        %10u \n", (unsigned int)emac_cntrs.rx.undersize_packet);
        printk("RxPausePkts:            %10u \n", (unsigned int)emac_cntrs.rx.pause_control_frame);
        printk("RxOverflow:             %10u \n", (unsigned int)emac_cntrs.rx.overflow);
        printk("RxCtrlPkts:             %10u \n", (unsigned int)emac_cntrs.rx.control_frame);
        printk("RxUnknownOp:            %10u \n", (unsigned int)emac_cntrs.rx.unknown_opcode);
        printk("RxLenError:             %10u \n", (unsigned int)emac_cntrs.rx.frame_length_error);
        printk("RxCodeError:            %10u \n", (unsigned int)emac_cntrs.rx.code_error);
        printk("RxCarrierSenseErr:      %10u \n", (unsigned int)emac_cntrs.rx.carrier_sense_error);
        printk("RxPkts64Octets:         %10u \n", (unsigned int)emac_cntrs.rx.frame_64);
        printk("RxPkts65to127Octets:    %10u \n", (unsigned int)emac_cntrs.rx.frame_65_127);
        printk("RxPkts128to255Octets:   %10u \n", (unsigned int)emac_cntrs.rx.frame_128_255);
        printk("RxPkts256to511Octets:   %10u \n", (unsigned int)emac_cntrs.rx.frame_256_511);
        printk("RxPkts512to1023Octets:  %10u \n", (unsigned int)emac_cntrs.rx.frame_512_1023);
        printk("RxPkts1024to1522Octets: %10u \n", (unsigned int)emac_cntrs.rx.frame_1024_1518);
        printk("RxPkts1523toMTU:        %10u \n", (unsigned int)emac_cntrs.rx.frame_1519_mtu);
    }
    return 0;
}

