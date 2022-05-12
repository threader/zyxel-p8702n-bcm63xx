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
 * bdmf_transport_proxy.c
 *
 * Inter-subsystem communication
 * Proxy transport
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

#define _BL_TRANSPORT_SERVICE_

#include <bdmf_transport_internal.h>
#include <bdmf_transport_proxy.h>

/*
 * Endpoint proxy
 */

/* send callback
 * This function is only called at termination points
 * (e.g., proxy transport from EHOST - via CMIPS - PMIPS, located
 * at EHOST or PMIPS, but not CMIPS
 */
static int bdmfts_proxy_msg_send(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{
    bdmfts_session_t *bdmfts_via = (bdmfts_session_t *)bdmfts->bdmfts_via;
    if (!bdmfts_via)
        return BDMF_ERR_COMM_FAIL;
    return bdmfts_msg_send(bdmfts_via, msg);
}

/* allocate message
 * This function is only called at termination points
 * (e.g., proxy transport from EHOST - via CMIPS - PMIPS, located
 * at EHOST or PMIPS, but not CMIPS
 */
static bdmfts_msg_t *bdmfts_proxy_msg_alloc(bdmfts_session_t *bdmfts, uint32_t payload_size, void *payload)
{
    bdmfts_session_t *bdmfts_via = (bdmfts_session_t *)bdmfts->bdmfts_via;
    if (!bdmfts_via)
        return NULL;
    return bdmfts_msg_alloc(bdmfts_via, payload_size, payload);
}

/* release transport message callback.
 * This function is only called at termination points
 * (e.g., proxy transport from EHOST - via CMIPS - PMIPS, located
 * at EHOST or PMIPS, but not CMIPS
 */
static void bdmfts_proxy_msg_free(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{
    bdmfts_session_t *bdmfts_via = (bdmfts_session_t *)bdmfts->bdmfts_via;
    if (bdmfts_via)
        bdmfts_msg_free(bdmfts_via, msg);
}

/* close proxy session */
static void bdmfts_proxy_close(bdmfts_session_t *bdmfts)
{
}

int bdmfts_proxy_open(bdmfts_session_t *bdmfts)
{
    bdmf_subsystem_t subs_via;
    bdmfts_session_t *bdmfts_via;
    /* parm->data contains name of the *via* subsystem */
    if (!bdmfts->parm.data)
    {
        bdmf_print_error("*via* subsystem name is required\n");
        return BDMF_ERR_PARM;
    }
    if (bdmf_subsystem_by_name((char *)bdmfts->parm.data, &subs_via))
    {
        bdmf_print_error("*via* subsystem name is invalid: %s\n", (char *)bdmfts->parm.data);
        return BDMF_ERR_PARM;
    }
    bdmfts_via = bdmfts_session(subs_via, bdmfts->parm.priority);
    if (!bdmfts_via)
    {
        bdmf_print_error("No connection to *via* subsystem: %s\n", (char *)bdmfts->parm.data);
        return BDMF_ERR_NOT_CONNECTED;
    }
    bdmfts->bdmfts_via = bdmfts_via;
    bdmfts->msg_alloc = bdmfts_proxy_msg_alloc;
    bdmfts->msg_free = bdmfts_proxy_msg_free;
    bdmfts->msg_send = bdmfts_proxy_msg_send;
    bdmfts->close = bdmfts_proxy_close;
    return 0;
}

static struct bdmfts_transport proxy_transport = {
    .open = bdmfts_proxy_open,

};

int bdmfts_proxy_init(void)
{
    return bdmfts_transport_register(BDMFTS_CONN_PROXY, &proxy_transport);
}

/*
 * Middle-point proxy
 */

/* receive error handler
 */
static void bdmfts_proxy_rx_error(bdmfts_session_t *bdmfts_from, bdmfts_msg_t *msg)
{
    bdmfts_tmsg_status_set(&msg->tmsg, BDMFTS_STATRMT_COMM_ERROR);
    if (bdmfts_rx_is_request(bdmfts_from, &msg->tmsg))
        bdmfts_msg_reply(bdmfts_from, msg);
    else
        bdmfts_msg_free(bdmfts_from, msg);
}

/* receive handler
 */
void bdmfts_proxy_rx_from(bdmfts_session_t *bdmfts_from, bdmfts_msg_t *msg)
{
    bdmf_subsystem_t dest;
    bdmfts_session_t *bdmfts_to;
    bdmfts_msg_t *msg_to;
    int is_reply;

    is_reply = bdmfts_rx_is_reply(bdmfts_from, &msg->tmsg);
    if (is_reply)
        dest = bdmfts_tmsg_source(&msg->tmsg);
    else
        dest = bdmfts_tmsg_destination(&msg->tmsg);
    bdmfts_to = bdmfts_session(dest, bdmfts_from->parm.priority);
    if (!bdmfts_to)
    {
        bdmfts_proxy_rx_error(bdmfts_from, msg);
        return;
    }

    /* Found target session. Now we need to send message across.
     * If the message is reply - there should already be a request
     * pending on another side.
     */
    if (is_reply)
    {
        msg_to = (bdmfts_msg_t *)msg->user_priv;
        /* a few sanity checks */
        if (!msg_to)
        {
            bdmfts_proxy_rx_error(bdmfts_from, msg);
            return;
        }
        if (msg_to->data_size < bdmfts_tmsg_payload_length(&msg->tmsg))
        {
            bdmfts_tmsg_status_set(&msg_to->tmsg, BDMFTS_STATRMT_COMM_ERROR);
            bdmfts_msg_reply(bdmfts_to, msg_to);
            bdmfts_msg_free(bdmfts_from, msg);
            return;
        }
    }
    else
    {
        msg_to = bdmfts_msg_alloc(bdmfts_to, msg->data_size, NULL);
        if (!msg_to)
        {
            bdmfts_proxy_rx_error(bdmfts_from, msg);
            return;
        }
    }
    /* Copy header and data.
     * ToDo: consider zero copy or DMA
     */
    msg_to->tmsg = msg->tmsg;
    memcpy(msg_to->data, msg->data, bdmfts_tmsg_payload_length(&msg->tmsg));
    if (bdmfts_rx_is_request(bdmfts_from, &msg->tmsg))
        msg_to->user_priv = (long)msg;
    else
    {
        bdmfts_msg_free(bdmfts_from, msg);
        msg_to->user_priv = 0;
    }
    bdmfts_msg_send(bdmfts_to, msg_to);
}

