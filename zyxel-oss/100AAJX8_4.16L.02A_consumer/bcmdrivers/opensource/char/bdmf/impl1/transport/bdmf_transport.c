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
 * bdmf_transport.c
 *
 * Inter-processor transport service
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
#ifdef BDMF_TRANSPORT_SOCKET
#include <bdmf_transport_socket.h>
#endif
#ifdef BDMF_TRANSPORT_SHM
#include <bdmf_transport_shm.h>
#endif
#ifdef BDMF_TRANSPORT_PROXY
#include <bdmf_transport_proxy.h>
#endif

static struct bdmfts_transport *bdmfts_transport_dev[BDMFTS_CONN__NUMBER_OF];

/* Message servers */
struct bdmfts_server
{
    struct bdmfts_server *next;
    bdmfts_server_parm_t parm;
};
static struct bdmfts_server *bdmfts_servers[BDMFTS_MSGTYPE__NUMBER_OF];

/* Sessions */
static bdmfts_session_t *bdmfts_sessions[BDMF_SUBSYSTEM__NUMBER_OF][BDMFTS_PRTY_LAST+1];


/** Initialise transport service module
 * \return  0 - OK\n
 *         <0 - error code
 */
int bdmfts_module_init(void)
{
#ifdef BDMF_TRANSPORT_SOCKET
    bdmfts_socket_init();
#endif
#ifdef BDMF_TRANSPORT_SHM
    bdmfts_shm_init();
#endif
#ifdef BDMF_TRANSPORT_PROXY
    bdmfts_proxy_init();
#endif
#ifdef BDMF_SHELL
    bdmfts_mon_init(NULL);
#endif
    return 0;
}


/** Un-initialise transport service module
 */
void bdmfts_module_exit(void);

/** Register transport
 */
int bdmfts_transport_register(bdmfts_conn_type_t type, struct bdmfts_transport *transport)
{
    if (bdmfts_transport_dev[type])
        return BDMF_ERR_ALREADY;
    bdmfts_transport_dev[type] = transport;
    return 0;
}

/** Register transport message server
 * Message server can be registered per message type.
 * Transport service de-multiplexes messages based on the message type
 * and delivers to the appropriate server.
 * \param[in]   server      Server registration parameters. Must NOT be allocated on the stack
 * \return  0 - OK\n
 *         <0 - error code
 */
int bdmfts_server_register(bdmfts_server_parm_t *server)
{
    struct bdmfts_server *bdmftserv;
    if (!server || !server->name)
        return BDMF_ERR_PARM;
    if (bdmfts_servers[server->msg_type])
        return BDMF_ERR_ALREADY;

    bdmftserv = bdmf_calloc(sizeof(struct bdmfts_server));
    if (!bdmftserv)
        return BDMF_ERR_NOMEM;
    bdmftserv->parm = *server;
    bdmftserv->next = bdmfts_servers[server->msg_type];
    bdmfts_servers[server->msg_type] = bdmftserv;
    return 0;
}

/** Unregister transport message server
 * \param[in]   server      Server registration parameters. Must NOT be allocated on the stack
 * \return  0 - OK\n
 *         <0 - error code
 */
int bdmfts_server_unregister(bdmfts_server_parm_t *server)
{
    struct bdmfts_server *bdmftserv, *prev=NULL;
    if (!server || !server->name)
        return BDMF_ERR_PARM;
    /* look for the server */
    bdmftserv = bdmfts_servers[server->msg_type];
    while(bdmftserv && (bdmftserv->parm.receive != server->receive) )
    {
        prev = bdmftserv;
        bdmftserv = bdmftserv->next;
    }
    if (!bdmftserv)
        return BDMF_ERR_NODEV;
    if (prev)
        prev->next = bdmftserv->next;
    else
        bdmfts_servers[server->msg_type] = bdmftserv->next;
    bdmf_free(bdmftserv);
    return 0;
}

/** Get transport message server parameters
 * \param[in]   msg_type    Message type
 * \return message server block or NULL if server is not registered for the message type
 */
const bdmfts_server_parm_t *bdmfts_server_get(bdmfts_msgtype_t msg_type)
{
    if (!bdmfts_servers[msg_type])
        return NULL;
    return &bdmfts_servers[msg_type]->parm;
}


/** Release transport session context */
static void _bdmfts_session_free(bdmfts_session_t *bdmfts)
{
    if (bdmfts->amsg)
        bdmf_free(bdmfts->amsg);
    bdmf_free(bdmfts);
}

/* connect or listen for connection */
static int _bdmfts_connect_or_listen(bdmfts_session_parm_t *session_parm, bdmfts_handle *pbdmfts,
    bdmfts_role_t role)
{
    struct bdmfts_transport *transport;
    bdmfts_session_t *bdmfts;
    char name[32];
    int rc;
    int i;

    if (!session_parm || !pbdmfts)
        return BDMF_ERR_PARM;
    if (bdmf_subsystem()==BDMF_SUBSYSTEM_LOCAL)
        return BDMF_ERR_NO_LOCAL_SUBS;
    if (bdmfts_sessions[session_parm->subsystem][session_parm->priority])
        return BDMF_ERR_ALREADY;
    if (!bdmfts_transport_dev[session_parm->conn_type])
        return BDMF_ERR_NOT_SUPPORTED;
    transport = bdmfts_transport_dev[session_parm->conn_type];
    snprintf(name, sizeof(name), "%s:%d",
        bdmf_subsystem_name(session_parm->subsystem), session_parm->priority);
    bdmfts = bdmf_calloc(sizeof(bdmfts_session_t)+
                    strlen(name) + 1 + transport->extra_size);
    if (!bdmfts)
        return BDMF_ERR_NOMEM;
    bdmfts->magic = BDMFTS_SESS_MAGIC;
    bdmfts->role = role;
    bdmfts->parm = *session_parm;
    bdmfts->name = (char *)bdmfts + sizeof(bdmfts_session_t) + transport->extra_size;
    strcpy(bdmfts->name, name);
    if (!bdmfts->parm.max_qsize)
        bdmfts->parm.max_qsize = BDMFTS_DEFAULT_MAX_QSIZE;
    bdmfts->amsg = bdmf_calloc(bdmfts->parm.max_qsize * sizeof(bdmfts_msg_t));
    if (!bdmfts->amsg)
    {
        _bdmfts_session_free(bdmfts);
        return BDMF_ERR_NOMEM;
    }
    TAILQ_INIT(&bdmfts->txq_list);
    TAILQ_INIT(&bdmfts->free_list);
    for(i=0; i<bdmfts->parm.max_qsize; i++)
    {
        bdmf_mutex_init(&bdmfts->amsg[i].lock);
        /* Initial state is "locked" */
        bdmf_mutex_lock(&bdmfts->amsg[i].lock);
        /* Put on free list */
        bdmfts_msg_release(bdmfts, &bdmfts->amsg[i]);
    }
    bdmf_fastlock_init(&bdmfts->lock);
    rc = transport->open(bdmfts);
    if (rc)
    {
        BDMFTS_ERR(bdmfts, "failed. rc=%d - %s\n", rc, bdmf_strerror(rc));
        _bdmfts_session_free(bdmfts);
        return rc;
    }
    bdmfts_sessions[session_parm->subsystem][session_parm->priority] = bdmfts;
    *pbdmfts = bdmfts;
    return rc;
}

/** Connect transport session
 * \param[in]   session_parm    Transport session parameters
 * \param[out]  pbdmfts          Session handle
 * \return  0 - OK\n
 *         <0 - error code
 */
int bdmfts_connect(bdmfts_session_parm_t *session_parm, bdmfts_handle *pbdmfts)
{
    return _bdmfts_connect_or_listen(session_parm, pbdmfts, BDMFTS_ROLE_CLIENT);
}


/** Wait for incoming connection
 * \param[in]   session_parm    Transport session parameters
 * \param[out]  pbdmfts           Session handle
 * \return  0 - OK\n
 *         <0 - error code
 */
int bdmfts_listen(bdmfts_session_parm_t *session_parm, bdmfts_handle *pbdmfts)
{
    return _bdmfts_connect_or_listen(session_parm, pbdmfts, BDMFTS_ROLE_SERVER);
}


/** Disconnect transport session
 * \param[in]   bdmfts           Session handle returned by bdmfts_connect().
 *                              The session handle becomes invalid following this call.
 */
void bdmfts_disconnect(bdmfts_handle bdmfts)
{
    assert(bdmfts->magic==BDMFTS_SESS_MAGIC);

    bdmfts_sessions[bdmfts->parm.subsystem][bdmfts->parm.priority] = NULL;

    bdmfts->close(bdmfts);

    /* Report to all requesters awaiting reply */
    bdmfts_notify_disconnect(bdmfts);

    /* Release everything */
    if (bdmfts->amsg)
        bdmf_free(bdmfts->amsg);

    bdmf_free(bdmfts);
}


/** Get session handle for transport connection with the given subsystem at given priority
 * \param[in]   subsystem       Target subsystem
 * \param[in]   priority        Transport priority
 * \return  bdmfts session handle or NULL if not connected
 */
bdmfts_handle bdmfts_session(bdmf_subsystem_t subsystem, bdmfts_prty_t priority)
{
    return bdmfts_sessions[subsystem][priority];
}


/** Get session parameters
 * \param[in]   bdmfts            Transport session handle
 * \return session parameter structure
 */
const bdmfts_session_parm_t *bdmfts_session_parms(bdmfts_handle bdmfts)
{
    assert(bdmfts->magic==BDMFTS_SESS_MAGIC);
    return &bdmfts->parm;
}

/** Allocate transport message
 * \param[in]   bdmfts            Transport session handle
 * \param[in]   payload_size    Message payload size
 * \param[in]   payload         Message payload pointer. If set, the function will try to use zero-copy,
 *                              depending on the underlying transport mechanism. If not set,
 *                              message payload is allocated internally.
 * \return  message block pointer if success\n
 *          NULL if failure
 */
bdmfts_msg_t *bdmfts_msg_alloc(bdmfts_handle bdmfts, uint32_t payload_size, void *payload)
{
    bdmfts_msg_t *msg;
    assert(bdmfts->magic==BDMFTS_SESS_MAGIC);
    msg = bdmfts->msg_alloc(bdmfts, payload_size, payload);
    if (!msg)
        return NULL;
    *(uint32_t *)&msg->tmsg = 0;
    bdmfts_tmsg_source_set(&msg->tmsg, bdmf_subsystem());
    bdmfts_tmsg_destination_set(&msg->tmsg, bdmfts->parm.subsystem);
    return msg;
}


/** Send request and expect for reply. Reply is notified asynchronously via callback
 * \param[in]   bdmfts        Transport session
 * \param[in]   msg         Message block
 * Notes:
 * non-zero return code indicates delivery failure.
 * per-message type txdone callback is always called eventually,
 * regardless on bdmfts_msg_send outcome.
 * \return  0 - OK\n
 *         <0 - delivery error
 */
int bdmfts_msg_send(bdmfts_handle bdmfts, bdmfts_msg_t *msg)
{
    int rc;
    assert(bdmfts->magic==BDMFTS_SESS_MAGIC);
    rc = bdmfts->msg_send(bdmfts, msg);
    if (rc)
    {
        msg->rc = rc;
        if (bdmfts_tx_is_request(bdmfts, &msg->tmsg))
            bdmfts_notify_rx(bdmfts, msg);
        else
            bdmfts_notify_tx_done(bdmfts, msg);
    }
    return rc;
}

/** Send reply
 * \param[in]   bdmfts        Session handle passed to the server in its "receive" callback
 * \param[in]   msg         Message passed to the "receive" callback, possibly modified
 * per-message type txdone callback is always called eventually,
 * regardless on the delivery outcome.
 * \return  0 - OK\n
 *         <0 - delivery error
 */
int bdmfts_msg_reply(bdmfts_handle bdmfts, bdmfts_msg_t *msg)
{
    assert(bdmfts_tmsg_is_full_duplex(&msg->tmsg));
    assert(bdmfts_tmsg_destination(&msg->tmsg)==bdmf_subsystem());
    return bdmfts_msg_send(bdmfts, msg);
}

/** Release message block
 * \param[in]   bdmfts        Transport session handle
 * \param[in]   msg         Message block
 */
void bdmfts_msg_free(bdmfts_handle bdmfts, bdmfts_msg_t *msg)
{
    assert(bdmfts->magic==BDMFTS_SESS_MAGIC);
    bdmfts->msg_free(bdmfts, msg);
}

/*
 * Transport sub-layers support functions
 */

/** Tx Done - called by transport service when tx is finished
 * and it is safe to deallocate message.
 * Returns 1 if deallocated
 */
static inline struct bdmfts_server *_bdmfts_server_get(int msg_type)
{
    if ((unsigned)msg_type >= BDMFTS_MSGTYPE__NUMBER_OF)
        return NULL;
    return bdmfts_servers[msg_type];
}

/** Notify that transmit is completed and it is safe to release
 * from the transport server point of view.
 */
void bdmfts_notify_tx_done(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{
    struct bdmfts_server *server = _bdmfts_server_get(bdmfts_tmsg_type(&msg->tmsg));
    if (!server || !server->parm.txdone)
        bdmfts_msg_free(bdmfts, msg);
    else
        server->parm.txdone(bdmfts, server->parm.server_priv, msg);
}

/** Notify rx message to the transport layer
 */
void bdmfts_notify_rx(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{
    int msg_type;
    struct bdmfts_server *server;

#ifdef BDMF_TRANSPORT_PROXY
    if (bdmfts_is_proxy_msg(msg))
    {
        bdmfts_proxy_rx_from(bdmfts, msg);
        return;
    }
#endif
    msg_type = bdmfts_tmsg_type(&msg->tmsg);
    server = _bdmfts_server_get(msg_type);
    if (!server || !server->parm.receive)
    {
        BDMFTS_INFO(bdmfts, "No server for message type %d. Message ignored\n", msg_type);
        bdmfts_tmsg_status_set(&msg->tmsg, BDMFTS_STATRMT_NO_SERVER);
        bdmfts_msg_free(bdmfts, msg);
    }
    msg->rc = bdmfts_message_status_to_err(bdmfts_tmsg_status(&msg->tmsg));
    server->parm.receive(bdmfts, server->parm.server_priv, msg);
}

/** Notify disconnect to the transport layer
 */
void bdmfts_notify_disconnect(bdmfts_session_t *bdmfts)
{
    bdmfts_msg_t *msg, *tmp_msg;
    bdmf_fastlock_lock(&bdmfts->lock);
    TAILQ_FOREACH_SAFE(msg, &bdmfts->txq_list, queue, tmp_msg)
    {
        bdmfts_tmsg_status_set(&msg->tmsg, BDMFTS_STATLCL_COMM_ERROR);
        TAILQ_REMOVE(&bdmfts->txq_list, msg, queue);
        bdmf_fastlock_unlock(&bdmfts->lock);
        msg->rc = BDMF_ERR_COMM_FAIL;
        if (bdmfts_tx_is_request(bdmfts, &msg->tmsg))
        {
#ifdef BDMF_TRANSPORT_PROXY
            if (bdmfts_is_proxy_msg(msg))
                bdmfts_proxy_rx_from(bdmfts, msg);
            else
#endif
            bdmfts_notify_rx(bdmfts, msg);
        }
        else
            bdmfts_notify_tx_done(bdmfts, msg);
        bdmf_fastlock_lock(&bdmfts->lock);
    }
    bdmf_fastlock_unlock(&bdmfts->lock);
}

/** Map BDMFTS_STAT... to BDMF_ERR_... code
 */
int bdmfts_message_status_to_err(uint16_t msg_status)
{
    switch(msg_status)
    {
    case 0: return 0;
    case BDMFTS_STATLCL_NO_RESOURCES: return BDMF_ERR_NOMEM;
    case BDMFTS_STATLCL_COMM_ERROR: return BDMF_ERR_COMM_FAIL;
    case BDMFTS_STATLCL_NO_REQUEST: return BDMF_ERR_MSG_ERROR;
    case BDMFTS_STATRMT_COMM_ERROR: return BDMF_ERR_COMM_FAIL;
    case BDMFTS_STATRMT_NO_RESOURCES: return BDMF_ERR_RMT_NO_RESOURCES;
    case BDMFTS_STATRMT_NO_SERVER: return BDMF_ERR_NO_MSG_SERVER;
    case BDMFTS_STATRMT_MSG_ERROR: return BDMF_ERR_RMT_MSG_ERROR;
    default: return BDMF_ERR_MSG_ERROR;
    }
}
