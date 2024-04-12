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
 * bdmf_rpc.c
 *
 * BDMF framework - RPC service
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

#include <bdmf_rpc_engine.h>
#include <bdmf_transport.h>

static bdmfrpc_server_t *bdmfrpc_servers[BDMF_RPC_MODULE__NUMBER_OF];

/* rpc message handler */
static void rpc_rx_handler(bdmfts_handle bdmfts, void *server_priv, bdmfts_msg_t *msg)
{
    bdmfrpc_module_t module;
    bdmfrpc_server_t *server;
    bdmfrpc_msg_t *rmsg=bdmfrpc_msg(msg);

    /* if it is a reply unlock process that waits for it */
    if (bdmfts_msg_is_reply(msg))
    {
        if (bdmfts_msg_sender(msg)==bdmf_subsystem())
        {
            bdmf_mutex_unlock(&msg->lock);
        }
        else
        {
            bdmf_print_error("Unexpected RPC reply. Request sender %s\n",
                bdmf_subsystem_name(bdmfts_msg_sender(msg)));
            bdmfts_tmsg_status_set(&msg->tmsg, BDMFTS_STATRMT_MSG_ERROR);
            bdmfts_msg_free(bdmfts, msg);
        }
        return;
    }

    /* sanity checks */
    module = bdmfrpc_msg_module(rmsg);
    if ((module >= BDMF_RPC_MODULE__NUMBER_OF) ||
        (bdmfts_msg_data_length(msg) < bdmfrpc_msg_parm_size(rmsg)))
    {
        bdmf_print_error("Malformatted RPC message: module=%d dl=%d ps=%d\n",
            module, bdmfts_msg_data_length(msg), bdmfrpc_msg_parm_size(rmsg));
        bdmfts_tmsg_status_set(&msg->tmsg, BDMFTS_STATRMT_MSG_ERROR);
        bdmfts_msg_free(bdmfts, msg);
        return;
    }

    /* Find server */
    server = bdmfrpc_servers[module];
    if (!server)
    {
        bdmf_print_error("No RPC server: module=%d\n", module);
        bdmfrpc_msg_rc_set(rmsg, BDMF_ERR_RPC_NO_SERVER);
        bdmfts_msg_free(bdmfts, msg);
        return;
    }

    /* Pass over to the server */
    server->handler(bdmfts, server->server_priv, msg);

    /* send reply to the sender */
    bdmfrpc_reply(bdmfts, msg);
}

/* tx completed handler */
static void rpc_txdone_handler(bdmfts_handle bdmfts, void *server_priv, bdmfts_msg_t *msg)
{
    /* send failure requires special handling.
     * some process can be blocked in rpc_call waiting for reply.
     */
    if (msg->rc && bdmfts_msg_is_request(msg))
    {
        bdmf_mutex_unlock(&msg->lock);
        return;
    }
    bdmfts_msg_free(bdmfts, msg);
    return;
}

static bdmfts_server_parm_t rpc_server = {
    .msg_type = BDMFTS_MSGTYPE_RPC,     /**< Message type - used for message de-multiplexing */
    .name = "rpc",
    .receive = rpc_rx_handler,
    .txdone = rpc_txdone_handler
};

/** Initialise RPC library
 * \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_module_init(void)
{
    int rc;
    rc = bdmfts_server_register(&rpc_server);
    if (rc)
    {
        bdmf_session_print(NULL, "Failed to register RPC server with transport service. Error %d - %s\n",
                        rc, bdmf_strerror(rc));
        return rc;
    }
#ifdef BDMF_SHELL
    bdmfrpc_mon_init(NULL);
#endif
    return 0;
}


/** Un-initialise RPC library
 */
void bdmfrpc_module_exit(void)
{
    int i;
    /* Unregister all services */
    for(i=0; i<BDMF_RPC_MODULE__NUMBER_OF; i++)
    {
        if (bdmfrpc_servers[i])
            bdmfrpc_server_unregister(bdmfrpc_servers[i]);
    }
}


/** Register RPC server
 *  \param[in]  server      Server descriptor
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_server_register(bdmfrpc_server_t *server)
{
    if (bdmfrpc_servers[server->module])
        return BDMF_ERR_ALREADY;
    bdmfrpc_servers[server->module] = server;
    return 0;
}


/** Unregister RPC server
 *  \param[in]  server      Server descriptor
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_server_unregister(bdmfrpc_server_t *server)
{
    if (bdmfrpc_servers[server->module] != server)
        return BDMF_ERR_PARM;
    bdmfrpc_servers[server->module] = NULL;
    return 0;
}


/** Get RPC service
 *  \param[in]  module      Server id
 *  \param[in]  server      Server descriptor
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_server_get(bdmfrpc_module_t module, const bdmfrpc_server_t **server)
{
    if (!bdmfrpc_servers[module])
        return BDMF_ERR_NOENT;
    *server = bdmfrpc_servers[module];
    return 0;
}

/** Allocate RPC message
 *  \param[in]  bdmfts      Transport session
 *  \param[in]  size        Payload size. 0=use default
 *  \param[in]  payload     Optional payload buffer. NULL=allocate automatically.
 *  \return
 *     message pointer or NULL if no resources
 */
bdmfts_msg_t *bdmfrpc_msg_alloc(bdmfts_handle bdmfts, uint32_t size, void *payload)
{
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    if (!size)
        size = BDMF_TRANSPORT_PAYLOAD_SIZE - sizeof(bdmfrpc_msg_t);
    msg = bdmfts_msg_alloc(bdmfts, sizeof(bdmfrpc_msg_t)+size, payload);
    if (!msg)
        return NULL;
    bdmfts_msg_type_set(msg, BDMFTS_MSGTYPE_RPC);
    rmsg=bdmfrpc_msg(msg);
    memset(rmsg, 0, sizeof(bdmfrpc_msg_t));
    bdmfrpc_msg_area_size_set(rmsg, size);
    return msg;
}


/** Call remote RPC service
 *  \param[in]  bdmfts        Transport session
 *  \param[in]  msg        RPC message
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_call(bdmfts_handle bdmfts, bdmfts_msg_t *msg)
{
    bdmfrpc_msg_t *rmsg=bdmfrpc_msg(msg);
    bdmfts_msg_data_length_set(msg, sizeof(bdmfrpc_msg_t)+bdmfrpc_msg_parm_size(rmsg));
    bdmfts_msg_full_duplex_set(msg);
    bdmfts_msg_send(bdmfts, msg);
    /* Wait for reply */
    bdmf_mutex_lock(&msg->lock);
    return msg->rc;
}


/** Send indication that doesn't require reply
 *  \param[in]  bdmfts        Transport session
 *  \param[in]  msg        RPC message
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_send(bdmfts_handle bdmfts, bdmfts_msg_t *msg)
{
    bdmfrpc_msg_t *rmsg=bdmfrpc_msg(msg);
    bdmfts_msg_data_length_set(msg, sizeof(bdmfrpc_msg_t)+bdmfrpc_msg_parm_size(rmsg));
    return bdmfts_msg_send(bdmfts, msg);
}


/** Reply to RPC call
 *  \param[in]  msg        RPC message
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_reply(bdmfts_handle bdmfts, bdmfts_msg_t *msg)
{
    bdmfrpc_msg_t *rmsg=bdmfrpc_msg(msg);
    bdmfts_msg_data_length_set(msg, sizeof(bdmfrpc_msg_t)+bdmfrpc_msg_parm_size(rmsg));
    return bdmfts_msg_reply(bdmfts, msg);
}
