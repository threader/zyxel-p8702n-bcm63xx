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

#ifndef __FAP4KE_FFE_H_INCLUDED__
#define __FAP4KE_FFE_H_INCLUDED__

/*
 *******************************************************************************
 * File Name  : fap4ke_ffe.h
 *
 * Description: This file contains the constants and prototypes needed for the
 *              FFE Driver running on the 4ke.
 *
 *******************************************************************************
 */

// Uncomment to pass all packets directly from rx to tx on FAP
//#define PERFORM_ALL_PACKETS_PASS_THROUGH

#define FFE_DRV_ENET             0
#define FFE_DRV_XTMRT            1

/* FFE Driver Prototypes */
fapRet ffeStartClassify(uint32 channel, unsigned char * pBuf, int len, uint32 dmaFlag, int drvType);


#endif  /* defined(__FAP4KE_FFE_H_INCLUDED__) */
