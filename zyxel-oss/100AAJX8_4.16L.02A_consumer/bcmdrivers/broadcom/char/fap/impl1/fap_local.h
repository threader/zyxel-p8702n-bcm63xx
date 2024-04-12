#ifndef __FAP_LOCAL_H_INCLUDED__
#define __FAP_LOCAL_H_INCLUDED__

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

/*
 *******************************************************************************
 * File Name  : fap_local.h
 *
 * Description: This file contains assorted definitions and prototypes for the
 *              Host MIPs side of the FAP interface for the 6362.
 *
 *******************************************************************************
 */

#include <linux/smp.h> /*To avoid warnings in r4kcache*/
#include <asm/io.h>
#include <asm/addrspace.h>
#include <asm/cpu-info.h>
#include <asm/r4kcache.h>
#include <linux/types.h>

#define FAP_IDX_FMT "[FAP%ld] "

static inline void fap_cacheInvFlush(void *start, void *end, int flush)
{
    uint32_t a, e;
    uint32_t dcLsize = current_cpu_data.dcache.linesz;

    a = (uint32_t)start & ~(dcLsize - 1);
    e = (uint32_t)end & ~(dcLsize - 1);

    while (1)
    {
        if (flush)
            flush_dcache_line(a);
        else
            invalidate_dcache_line(a);

        if (a == e)
            break;

        a += dcLsize;
    }
}

/* Prototypes: fapHwInit.c */
int fap_init4ke(uint32 fapIdx);
void fap_enable4ke(uint32 fapIdx);

/* Prototypes: fap_interrupt.c */
int fapIrq_init(uint32 fapIdx, uint32 registerIrqs);

/******************************************************************************
 * fapIrq_enable: MUST BE CALLED WITH INTERRUPTS DISABLED!
 ******************************************************************************/
#define fapIrq_enable(fapIdx, _irqMask)                                 \
    FAP_HOST_REG_WR(hostRegCntrl(fapIdx)->irq_mips_mask,                \
                    (FAP_HOST_REG_RD(hostRegCntrl(fapIdx)->irq_mips_mask) | (_irqMask)))

/******************************************************************************
 * fapIrq_disable: MUST BE CALLED WITH INTERRUPTS DISABLED!
 ******************************************************************************/
#define fapIrq_disable(fapIdx, _irqMask)                                \
    FAP_HOST_REG_WR(hostRegCntrl(fapIdx)->irq_mips_mask,                \
                    (FAP_HOST_REG_RD(hostRegCntrl(fapIdx)->irq_mips_mask) & ~(_irqMask)))

/* Prototypes: fap_mailBox.c */
void fapMailBox_hostInit(uint32 fapIdx);

/* Prototyps: fapDqm.c */
void fapDqm_hostInit(void);

extern void fap_Updatejiffies(uint32 fapIdx);

#endif  /* defined(__FAP_LOCAL_H_INCLUDED__) */
