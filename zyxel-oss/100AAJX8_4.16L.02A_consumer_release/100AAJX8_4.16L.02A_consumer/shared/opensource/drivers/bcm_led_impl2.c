/*
 * <:copyright-BRCM:2014:DUAL/GPL:standard
 * 
 *    Copyright (c) 2014 Broadcom Corporation
 *    All Rights Reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation (the "GPL").
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * 
 * A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 * writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * :>
 */

#include "boardparms.h"
#include "bcm_led.h"
#include "bcm_pinmux.h"

#ifdef _CFE_
#include "lib_types.h"
#include "lib_printf.h"
#include "lib_string.h"
#include "bcm_map.h"
#define printk  printf
#else // Linux
#include <linux/kernel.h>
#include <linux/module.h>
#include <bcm_map_part.h>
#include <linux/string.h>
#endif

// #define BCM_LED_DEBUG 1

/*
  These are low level functions that can be called from CFE or from the Linux board driver
  The Linux board driver handles any necessary locking so these functions should not be called
  directly from elsewhere.
*/

/* Currently, this manages only the power LED in 60333. This LED works
 * in a different way than the regular ones: it's driven by the AON and
 * it's not selectable.
 * I kept the signature of these functions to preserve a coherent API
 * with impl1, but for now the "num" parameter in bcm_led_driver_set and
 * bcm_led_driver_toggle is ignored.
 */

/* These are defined the same as in
 * cfe/cfe/board/bcm63xx_ram/include/bcm63xx_util.h
 */
enum {
    LED_OFF,
    LED_ON
};

/* Keeps the current state of the power LED: [LED_ON/LED_OFF] */
static unsigned short bcm_led_driver_state;

/* **********************************************************************
 *   BSTIWrite(addr, data)
 *
 *   Writes <data> in BSTI address <addr> and waits for the
 *   transaction to complete
 *
 *   Input parameters:
 *       Address to write to
 *       Data to write
 *
 *   Return value:
 *       nothing
 ************************************************************************ */
static void BSTIWrite(unsigned int addr, unsigned short data)
{
    unsigned int bstictrl = 0;
    unsigned int finish = 0;

    bstictrl = (data & BSTI_SER_CTRL_WR_DATA_MASK);
    bstictrl |= ((addr << BSTI_SER_CTRL_ADDR_SHIFT) & BSTI_SER_CTRL_ADDR_MASK);
    bstictrl |= ((BSTI_WRITE_OP << BSTI_SER_CTRL_CMD_SHIFT)
                                  & BSTI_SER_CTRL_CMD_MASK);
    bstictrl |= ((BSTI_START_OP << BSTI_SER_CTRL_START_SHIFT)
                                  & BSTI_SER_CTRL_START_MASK);
    BSTI->ser_ctrl = bstictrl;

    do
    {
        finish = (BSTI->ser_ctrl & BSTI_SER_CTRL_START_MASK)
                                >> BSTI_SER_CTRL_START_SHIFT;
    } while (finish == BSTI_START_OP);
}

void bcm_led_driver_set(unsigned short num, unsigned short state)
{
#ifdef BCM_LED_DEBUG
   printk("LED %x set state %d\n",num,state);
#endif
   if (state != bcm_led_driver_state) {
      bcm_led_driver_state = state;
      if (state == LED_ON) {
          BSTIWrite(AON_REGISTERS_LED_EN, 1);
      } else {
          BSTIWrite(AON_REGISTERS_LED_EN, 0);
      }
   }
}

void bcm_led_driver_toggle(unsigned short num)
{
    if (bcm_led_driver_state == LED_ON) {
        bcm_led_driver_set(num, LED_OFF);
    } else {
        bcm_led_driver_set(num, LED_ON);
    }
}

void bcm_common_led_init(void) {
    /* Disable Power LED */
    BSTIWrite(AON_REGISTERS_LED_EN, 0);

    /* Configure LED duty cycle: 100% of time ON when led is enabled */
    BSTIWrite(AON_REGISTERS_LED0_ON, 0xFFFF);  /* LED on (lower half-word) */
    BSTIWrite(AON_REGISTERS_LED1_ON, 0xFFFF);  /* LED on (upper half-word) */
    BSTIWrite(AON_REGISTERS_LED0_OFF, 0);      /* LED off (lower half-word) */
    BSTIWrite(AON_REGISTERS_LED1_OFF, 0);      /* LED off (upper half-word) */
}

void bcm_common_led_setAllSoftLedsOff(void)
{
    bcm_led_driver_set(0, LED_OFF);
}
