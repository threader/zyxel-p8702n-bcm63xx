/*
 <:copyright-BRCM:2013:DUAL/GPL:standard
 
    Copyright (c) 2013 Broadcom Corporation
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

#ifndef _PMD_H_
#define _PMD_H_

#include <linux/ioctl.h>
#include "laser.h"

#define PMD_I2C_HEADER 6 /*  Consist: 1 byte - CSR address, 1byte - config reg, 4 byte - register address */

#define PMD_DEV_CLASS   "laser_dev"
#define PMD_BUF_MAX_SIZE 300

typedef struct {
    uint8_t client;
    uint16_t offset; /* is used as message_id when using the messaging system */
    uint16_t len;
    unsigned char *buf;
} pmd_params;

/* IOctl */
#define PMD_IOCTL_SET_PARAMS     _IOW(LASER_IOC_MAGIC, 11, pmd_params)
#define PMD_IOCTL_GET_PARAMS     _IOR(LASER_IOC_MAGIC, 12, pmd_params)
#define PMD_IOCTL_FILE_WRITE     _IOW(LASER_IOC_MAGIC, 13, pmd_params)
#define PMD_IOCTL_FILE_READ      _IOR(LASER_IOC_MAGIC, 14, pmd_params)
#define PMD_IOCTL_SW_DOWNLOAD    _IOW(LASER_IOC_MAGIC, 15, short)
#define PMD_IOCTL_REG_FILE_WRITE _IOW(LASER_IOC_MAGIC, 16, pmd_params)
#define PMD_IOCTL_REG_FILE_READ  _IOR(LASER_IOC_MAGIC, 17, pmd_params)
#define PMD_IOCTL_MSG_WRITE      _IOW(LASER_IOC_MAGIC, 18, pmd_params)
#define PMD_IOCTL_MSG_READ       _IOR(LASER_IOC_MAGIC, 19, pmd_params)

typedef struct
{
    uint32_t alarms;
    uint32_t sff;
}pmd_msg_addr;

#endif /* ! _PMD_H_ */
