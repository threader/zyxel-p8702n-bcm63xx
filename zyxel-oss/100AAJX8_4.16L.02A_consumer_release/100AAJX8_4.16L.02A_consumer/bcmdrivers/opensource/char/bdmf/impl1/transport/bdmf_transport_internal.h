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
 * bdmf_transport_internal.h
 *
 * Inter-processor transport service
 * Internal header file. This header file should only be included
 * by transport service module
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
#ifndef _BL_TRANSPORT_INTERNAL_H_
#define _BL_TRANSPORT_INTERNAL_H_

#ifndef _BL_TRANSPORT_SERVICE_
#error bdmf_transport_internal.h is an internal header file that should only by included by transport service modules
#endif

#include <bdmf_system.h>
#include <bdmf_transport.h>
#include <bdmf_transport_msg.h>

/** Transport role type
 */
typedef enum
{
    BDMFTS_ROLE_CLIENT,               /**< client */
    BDMFTS_ROLE_SERVER,               /**< server */
    BDMFTS_ROLE__NUMBER_OF
} bdmfts_role_t;

/* Transport object */
struct bdmfts_transport;

TAILQ_HEAD(bdmfts_msgq, bdmfts_msg);

/** communication session
 */
typedef struct bdmfts_session
{
    uint32_t magic;
#define BDMFTS_SESS_MAGIC (('b'<<24) | ('l'<<16) | ('t'<<8) | 's')
    char *name;             /**< Session name */
    bdmfts_session_parm_t parm;
    /** Allocate message */
    struct bdmfts_msg *(*msg_alloc)(struct bdmfts_session *session,
                    uint32_t payload_size, void *payload);
    /** Free message */
    void (*msg_free)(struct bdmfts_session *session, struct bdmfts_msg *msg);
    /** Send message */
    int (*msg_send)(struct bdmfts_session *bdmfts, bdmfts_msg_t *msg);
    /** Close session */
    void (*close)(struct bdmfts_session *session);

    bdmfts_role_t role;
    bdmf_task rx_thread;
    long session_data1;
    long session_data2;
    int transport_header_size;
    int killed;
    int connected;
    bdmfts_msg_t *amsg;                 /* Pre-allocated message descriptor array */
    struct bdmfts_session *bdmfts_via;  /* destination leg in endpoint-proxy session */
    struct bdmfts_msgq txq_list;        /* List of messages pending acknowledge */
    struct bdmfts_msgq free_list;       /* free list */
    bdmf_fastlock lock;
} bdmfts_session_t;


/** Transport type
 * transport type registration structure
 */
struct bdmfts_transport
{
    /** Open session */
    int (*open)(bdmfts_session_t *session);
    int extra_size; /**< Number of extra bytes to allocate in session structure */
};

/** Register transport
 */
int bdmfts_transport_register(bdmfts_conn_type_t type, struct bdmfts_transport *transport);

/*
 * Helpers for transport drivers use
 */

/* get free message descriptor from cache.
 * The function must be called under lock
 */
static inline bdmfts_msg_t *bdmfts_msg_get(bdmfts_session_t *bdmfts)
{
    bdmfts_msg_t *msg;
    msg = TAILQ_FIRST(&bdmfts->free_list);
    if (!msg)
        return NULL;
    TAILQ_REMOVE(&bdmfts->free_list, msg, queue);
    msg->is_free = 0;
    return msg;
}

/* return free message descriptor to cache.
 * The function must be called under lock
 */
static inline void bdmfts_msg_release(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{
    assert(!msg->is_free);
    TAILQ_INSERT_TAIL(&bdmfts->free_list, msg, queue);
    msg->is_free = 1;
}

/* get message descriptor given its index
 * The function must be called under lock
 */
static inline bdmfts_msg_t *bdmfts_msg_get_by_idx(bdmfts_session_t *bdmfts, unsigned int idx)
{
    bdmfts_msg_t *msg;
    if (idx >= bdmfts->parm.max_qsize)
        return NULL;
    msg = &bdmfts->amsg[idx];
    if (msg->is_free)
        return NULL;
    return msg;
}

/* get message index. the message must be not free
 */
static inline int bdmfts_msg_idx(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{
    int idx;
    assert(!msg->is_free);
    idx = (msg - bdmfts->amsg);
    assert((unsigned)idx < bdmfts->parm.max_qsize);
    return idx;
}

/*
 * message type helpers
 */
static inline int bdmfts_rx_is_request(bdmfts_session_t *bdmfts, const bdmfts_tmsg_t *tmsg)
{
    if (!bdmfts_tmsg_is_full_duplex(tmsg))
        return 0;
    return (bdmfts_tmsg_destination(tmsg) == bdmf_subsystem())  ||
           (bdmfts_tmsg_source(tmsg) == bdmfts->parm.subsystem);
}

static inline int bdmfts_rx_is_reply(bdmfts_session_t *bdmfts, const bdmfts_tmsg_t *tmsg)
{
    if (!bdmfts_tmsg_is_full_duplex(tmsg))
        return 0;
    return (bdmfts_tmsg_source(tmsg) == bdmf_subsystem()) ||
           (bdmfts_tmsg_destination(tmsg) == bdmfts->parm.subsystem);
}

static inline int bdmfts_tx_is_request(bdmfts_session_t *bdmfts, const bdmfts_tmsg_t *tmsg)
{
    return bdmfts_rx_is_reply(bdmfts, tmsg);
}

static inline int bdmfts_tx_is_reply(bdmfts_session_t *bdmfts, const bdmfts_tmsg_t *tmsg)
{
    return bdmfts_rx_is_request(bdmfts, tmsg);
}


/*
 * Notifications.
 * Called by transport type-specific sublayer to inform generic transport
 * layer about various events
 */

/** Transport type-specific sublayer hands received message
 * over to the generic transport layer
 */
void bdmfts_notify_rx(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg);

/** Notify that transmit is completed and it is safe to release
 * from the transport server point of view.
 */
void bdmfts_notify_tx_done(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg);

/** Notify connect to the generic transport layer
 */
void bdmfts_notify_connect(bdmfts_session_t *bdmfts);

/** Notify disconnect to the generic transport layer
 */
void bdmfts_notify_disconnect(bdmfts_session_t *bdmfts);

/** Map BDMFTS_STAT... to BDMF_ERR_... code
 */
int bdmfts_message_status_to_err(uint16_t msg_status);

#define BDMFTS_PRINT(_bdmfts, _f, _args...) \
    bdmf_print("%s:%d - %s : " _f, __FUNCTION__, __LINE__, _bdmfts->name, ## _args)

#define BDMFTS_INFO(_bdmfts, _f, _args...) \
    bdmf_print("info> %s:%d - %s : " _f, __FUNCTION__, __LINE__, _bdmfts->name, ## _args)

#define BDMFTS_ERR(_bdmfts, _f, _args...) \
    bdmf_print("err> %s:%d - %s : " _f, __FUNCTION__, __LINE__, _bdmfts->name, ## _args)

#endif /* _BL_TRANSPORT_INTERNAL_H_ */
