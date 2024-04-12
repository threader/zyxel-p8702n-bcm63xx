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
 * bdmf_transport_proxy.h
 *
 * Inter-subsystem communication
 * Proxy transport service
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
#ifndef _BL_TRANSPORT_PROXY_H_
#define _BL_TRANSPORT_PROXY_H_

#include <bdmf_transport_msg.h>

static inline int bdmfts_is_proxy_msg(bdmfts_msg_t *msg)
{
    bdmf_subsystem_t subs=bdmf_subsystem();
    return (bdmfts_tmsg_source(&msg->tmsg) != subs &&
            bdmfts_tmsg_destination(&msg->tmsg) != subs);
}

/* receive handler
 */
void bdmfts_proxy_rx_from(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg);

/* module init */
int bdmfts_proxy_init(void);

#endif /* _BL_TRANSPORT_PROXY_H_ */
