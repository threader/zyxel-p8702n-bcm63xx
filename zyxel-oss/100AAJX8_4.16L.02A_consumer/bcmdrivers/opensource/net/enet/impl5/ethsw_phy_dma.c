/*
 <:copyright-BRCM:2011:DUAL/GPL:standard
 
    Copyright (c) 2011 Broadcom Corporation
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
#include "bcm_OS_Deps.h"
#include "bcmtypes.h"
#include "bcmmii.h"
#include "ethsw_phy.h"
#include "bcmsw.h"
#include "boardparms.h"
#include "bcmswaccess.h"

extern spinlock_t bcm_ethlock_phy_access;
static DEFINE_MUTEX(bcm_phy_mutex);

/* Note: PHY register access need to wait 50us to complete */
void ethsw_phy_rw_reg(int phy_id, int reg, uint16 *data, int ext_bit, int rd)
{
    uint32 reg_value, read_back, flags;
    int preempt_cnt;

    phy_id &= BCM_PHY_ID_M;
    preempt_cnt = preempt_count();

    if (preempt_cnt == 0) {
        mutex_lock(&bcm_phy_mutex);
    }

    spin_lock_irqsave(&bcm_ethlock_phy_access, flags);
taskRetry:
    reg_value = 0;
    ethsw_wreg(PAGE_CONTROL, REG_MDIO_CTRL_ADDR, (uint8 *)&reg_value, 4);
    reg_value = (ext_bit? REG_MDIO_CTRL_EXT: 0) |
        ((phy_id << REG_MDIO_CTRL_ID_SHIFT) & REG_MDIO_CTRL_ID_MASK) |
        (reg  << REG_MDIO_CTRL_ADDR_SHIFT)|
        (rd? (REG_MDIO_CTRL_READ) : (REG_MDIO_CTRL_WRITE | *data));

    ethsw_wreg(PAGE_CONTROL, REG_MDIO_CTRL_ADDR, (uint8 *)&reg_value, 4);

    if(preempt_cnt == 0){
        spin_unlock_irqrestore(&bcm_ethlock_phy_access, flags);
        msleep(1);
        spin_lock_irqsave(&bcm_ethlock_phy_access, flags);

        /* Now compare the R/W control register contents,
            if it changed, IRS has cut in and changed the register,
            we need to retry the operation. If the register content
            is the same, either no IRS cut in, or even IRS cut in it did the
            same R/W operation, so we can use the result directly in either case. */
        ethsw_rreg(PAGE_CONTROL, REG_MDIO_CTRL_ADDR, (uint8 *)&read_back, 4);
        if (read_back != reg_value)
            goto taskRetry;
    } else {
        udelay(60);
    }

    if (rd) {
        ethsw_rreg(PAGE_CONTROL, REG_MDIO_DATA_ADDR, (uint8 *)data, 2);
    }

    spin_unlock_irqrestore(&bcm_ethlock_phy_access, flags);
    if(preempt_cnt == 0){
        mutex_unlock(&bcm_phy_mutex);
    }
}

MODULE_LICENSE("GPL");

