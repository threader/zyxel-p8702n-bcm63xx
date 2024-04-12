/************************************************************
 *
 * <:copyright-BRCM:2012:DUAL/GPL:standard
 * 
 *    Copyright (c) 2012 Broadcom Corporation
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
 ************************************************************/

/* This file contains mirrors of Linux specific data types and structures */

#ifndef _FAP4KEOSDEPS_H_
#define _FAP4KEOSDEPS_H_

/* we're including one file from the kernel: the version file
   I'm purposely convoluting the include path though so that
   other people are not tempted to include other kernel files. */

/* This does mean that we have to build the kernel before
   we build this though... TBD: add makefile dependency */

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long s64;
typedef unsigned long u64;

#ifndef BUG
#define BUG(...)          do { } while(0)
#endif /* BUG */
void panic(char *s);

struct net_device {
  unsigned int dev;
};

#define IN
#define OUT

#define true 1
#define false 0

#define virt_to_page(val1) 0
#define SetPageReserved(val1)

#define PAGE_SIZE 1
#define PAGE_MASK 0

#define read_c0_count()     0

#define min_t(type, x, y) ({  \
  type __min1 = (x);  \
  type __min2 = (y);  \
  __min1 < __min2 ? __min1: __min2; })

#define max_t(type, x, y) ({  \
  type __max1 = (x);  \
  type __max2 = (y);  \
  __max1 > __max2 ? __max1: __max2; })

/* WARNING - TBD: look into this... this looks like it could be a problem!!!
   guessing at addresses for now...

   it is used in BCM_PKTDMA_PBUF_FROM_BD, in enetRecvTask.
   */
#define phys_to_virt(address)   ((unsigned long)address - 0x80000000UL + 0)

/* NOTE: these have been modified to match the kernel definitions of the 
   macros.  They no longer match the definitions in bcm_OS_deps.h */
#define CPHYSADDR(val1) ((unsigned long)(val1) & 0x1fffffff)
#define KSEG0ADDR(a)    (CPHYSADDR(a) | 0x80000000)
#define KSEG1ADDR(a)    (CPHYSADDR(a) | 0xa0000000)

#endif /* _FAP4KEOSDEPS_H_ */
