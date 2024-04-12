/*
* <:copyright-BRCM:2013:GPL/GPL:standard
* 
*    Copyright (c) 2013 Broadcom Corporation
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


/*******************************************************************
 * bdmf_transport_shm.h
 *
 * Inter-subsystem communication
 * Shared memory-based transport service
 *
 * This file is Copyright (c) 2011, Broadlight Communications.
 * This file is licensed under GNU Public License, except that if
 * you have entered in to a signed, written license agreement with
 * Broadlight covering this file, that agreement applies to this
 * file instead of the GNU Public License.
 *
 * This file is free software: you can redistribute and/or modify it
 * under the terms of the GNU Public License, Version 2, as published
 * by the Free Software Foundation, unless a different license
 * applies as provided above.
 *
 * This program is distributed in the hope that it will be useful,
 * but AS-IS and WITHOUT ANY WARRANTY; without even the implied
 * warranties of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 * TITLE or NONINFRINGEMENT. Redistribution, except as permitted by
 * the GNU Public License or another license agreement between you
 * and Broadlight, is prohibited.
 *
 * You should have received a copy of the GNU Public License,
 * Version 2 along with this file; if not, see
 * <http://www.gnu.org/licenses>.
 *
 * Author: Igor Ternovsky
 *******************************************************************/
#ifndef _BL_TRANSPORT_SHM_H_
#define _BL_TRANSPORT_SHM_H_

#define BDMFTS_SHM_DEFAULT_Q_SIZE     64  /**< Default message queue size */

#ifdef BDMF_TRANSPORT_INTERNAL_H

/* transport header */
typedef volatile struct bdmfts_shm_tmsg
{
    struct bdmfts_tmsg hdr;           /**< Transport-independent header */
    uint32_t payload;               /**< Shared payload pointer */
} bdmfts_shm_tmsg_t;

#define BDMFTS_SHM_MIN_DATA_SIZE  2048

#endif

int bdmfts_shm_init(void);

int bdmfts_shm_connect_request(bdmf_config_cmd_resp_t *cmd);

#endif /* _BL_TRANSPORT_SHM_H_ */
