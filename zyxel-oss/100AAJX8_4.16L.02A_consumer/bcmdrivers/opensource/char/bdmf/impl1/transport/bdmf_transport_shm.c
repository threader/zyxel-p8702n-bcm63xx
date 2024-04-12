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
 * bdmf_transport_shm.c
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

#define _BL_TRANSPORT_SERVICE_

#include <bdmf_transport_internal.h>
#include <bdmf_transport_shm.h>

/* Queue descriptor in local memory */
struct bdmfts_shm_qd
{
    bdmfts_session_t *bdmfts;       /* my session */
    void *shm_descr;            /* Queue descriptor in shared memory:
                                   uint16_t c2s_write
                                   uint16_t s2c_write
                                   size * bdmfts_shm_tmsg descriptors followed by
                                   (size+1) * uint16_t - c2s indexes
                                   (size+1) * uint16_t - s2c indexes
                                 */

    bdmfts_shm_tmsg_t *atmsgd;    /* Transport message descriptors array */
    volatile uint16_t *ptx_write;/* pointer of tx write index in the shared area. Owned by *us" */
    volatile uint16_t *prx_write;/* pointer of rx write index in the shared area. Owned by the peer */
    volatile uint16_t *arx;     /* rx message index ring */
    volatile uint16_t *atx;     /* tx message index ring */
    int rx_read_idx;            /* read index: c2s ring (server), s2c (client) */
    int tx_write_idx;           /* write index: c2s (client), s2c (server). has the same value as *tx_write */
    int rx_ring_size;           /* read ring size: c2s ring (server), s2c (client) */
    int tx_ring_size;           /* write ring size: c2s (client), s2c (server) */
    int shbuf_size;             /* shared area size */
    int tmsg_size;              /* tmsg entry size */
    int qsize;                  /* Number of descriptors in atmsgd array */
    int my_irq;                 /* my IRQ */
    int peer_irq;
    int my_session;             /* Index in assessions array */
    bdmf_mutex rx_lock;        /* rx thread lock */
};

#define BDMFTS_SHM_MAX_SESSIONS   16
static struct bdmfts_shm_qd *asessions[BDMFTS_SHM_MAX_SESSIONS];
#define BDMFTS_SHM_MSGQ_IRQ_BASE  1   /* 1st allocated to communication irq in a group of IRQs allocated to a subsystem */

int bdmfts_shm_connect_request(bdmf_config_cmd_resp_t *cmd)
{
    bdmfts_session_parm_t session_parm;
    bdmfts_handle bdmfts;
    int rc;
    memset(&session_parm, 0, sizeof(session_parm));
    session_parm.subsystem = cmd->client;
    session_parm.priority = cmd->x.msgq_init.id;
    session_parm.conn_type = BDMFTS_CONN_SHM;
    session_parm.max_qsize = cmd->x.msgq_init.size;
    session_parm.data = (void *)cmd;
    rc = bdmfts_listen(&session_parm, &bdmfts);
    return rc;
}


int bdmfts_shm_send_connect_and_wait(bdmfts_session_t *bdmfts, bdmf_config_cmd_resp_t *cmd)
{
    cmd->cmd = BDMF_CONFIG_MSG_QUEUE;
    cmd->error = 0;
    cmd->x.msgq_init.id = bdmfts->parm.priority;
    if (!bdmfts->parm.max_qsize)
        bdmfts->parm.max_qsize = BDMFTS_SHM_DEFAULT_Q_SIZE;
    cmd->x.msgq_init.size = bdmfts->parm.max_qsize;
    cmd->x.msgq_init.entry_size = sizeof(bdmfts_shm_tmsg_t);
    cmd->client = bdmf_subsystem();
    cmd->cmd_resp = 1;
    bdmf_print("SHM: waiting for server to connect\n");
    /* ToDo: redefine */
    bdmf_irq_raise(bdmfts->parm.subsystem, BDMF_IRQ_CONFIG_AREA);
    while(cmd->cmd_resp == 1)
        usleep(1000);
    return 0;
}

/* allocate shared payload */
static void *bdmfts_shm_msg_payload_alloc(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg, uint32_t payload_size)
{
#ifdef BDMF_SYSTEM_SIM
    bdmfts_shm_tmsg_t *tmsg = (bdmfts_shm_tmsg_t *)msg->drv_priv;
    char shm_fname[32];
    assert(!msg->data);
    if (payload_size < BDMFTS_SHM_MIN_DATA_SIZE)
        payload_size = BDMFTS_SHM_MIN_DATA_SIZE;
    snprintf(shm_fname, sizeof(shm_fname), "/%d", tmsg->payload);
    msg->data = bdmf_mmap(shm_fname, payload_size);
    msg->data_size = payload_size;
    if (!msg->data)
        return NULL;
#else
#error SHM: platform is not supported
#endif
    return msg->data;
}

/* release shared payload */
static void bdmfts_shm_msg_payload_free(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{
#ifdef BDMF_SYSTEM_SIM
    if (msg->data)
    {
        munmap(msg->data, msg->data_size);
        msg->data = NULL;
    }
#else
#error SHM: platform is not supported
#endif
}

/* send callback */
static int bdmfts_shm_send(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{
    struct bdmfts_shm_qd *qd = (struct bdmfts_shm_qd *)((long)bdmfts + sizeof(*bdmfts));
    bdmfts_shm_tmsg_t *tmsg = &qd->atmsgd[msg->tmsg_id];

    if (!bdmfts->connected)
        return BDMF_ERR_NOT_CONNECTED;

    /* Copy header */
    tmsg->hdr = msg->tmsg;
    bdmf_fastlock_lock(&bdmfts->lock);
    list_add_tail(&msg->queue, &bdmfts->txq_list);
    qd->atx[qd->tx_write_idx++] = msg->tmsg_id;
    if (qd->tx_write_idx == qd->tx_ring_size)
        qd->tx_write_idx = 0;
    *qd->ptx_write = qd->tx_write_idx;
    /* ToDo: check: Unmap remote message payload */
    if (!msg->is_locally_allocated)
        bdmfts_shm_msg_payload_free(bdmfts, msg);
    bdmf_fastlock_unlock(&bdmfts->lock);

    /* Send interrupt to peer */
    bdmf_irq_raise(bdmfts->parm.subsystem, qd->peer_irq);

    return 0;
}

/* get transport message header */
static inline bdmfts_shm_tmsg_t *_bdmfts_shm_tmsg(struct bdmfts_shm_qd *qd, uint16_t idx)
{
    bdmfts_shm_tmsg_t *tmsg = (bdmfts_shm_tmsg_t *)((long)qd->atmsgd + idx*qd->tmsg_size);
    return tmsg;
}

/* allocate message */
bdmfts_msg_t *bdmfts_shm_msg_alloc(bdmfts_session_t *bdmfts, uint32_t payload_size, void *payload)
{
    bdmfts_msg_t *msg;

    bdmf_fastlock_lock(&bdmfts->lock);
    msg = bdmfts_msg_get(bdmfts);
    bdmf_fastlock_unlock(&bdmfts->lock);
    if (!msg)
        return NULL;

    if (payload_size && !bdmfts_shm_msg_payload_alloc(bdmfts, msg, payload_size))
    {
        bdmf_fastlock_lock(&bdmfts->lock);
        bdmfts_msg_release(bdmfts, msg);
        bdmf_fastlock_unlock(&bdmfts->lock);
        return NULL;
    }

    return msg;
}

/* release transport message callback */
void bdmfts_shm_msg_free(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{

    bdmfts_shm_msg_payload_free(bdmfts, msg);

    /* If message was locally allocated - return descriptor to the free cache.
     * Otherwise, return it to the owner
     */
    bdmf_fastlock_lock(&bdmfts->lock);
    if (!msg->is_locally_allocated)
    {
        bdmf_fastlock_unlock(&bdmfts->lock);
        bdmfts_msg_type_set(msg, BDMFTS_MSGTYPE_DONE);
        bdmfts_shm_send(bdmfts, msg);
        return;
    }
    /* Local release */
    bdmfts_msg_release(bdmfts, msg);
    bdmf_fastlock_unlock(&bdmfts->lock);
}

/* receive handler
 */
static void _bdmfts_shm_recv(struct bdmfts_shm_qd *qd, uint16_t msg_index)
{
    bdmfts_session_t *bdmfts = qd->bdmfts;
    bdmfts_msg_t *msg = &bdmfts->amsg[msg_index];
    bdmfts_shm_tmsg_t *tmsg=(bdmfts_shm_tmsg_t *)msg->drv_priv;
    uint32_t payload_size;

    /* cache message header */
    msg->tmsg = tmsg->hdr;

    /*
     * sanity checks
     */
    /* make sure that the received message is not in the free cache */
    assert(!msg->is_free);
    bdmf_fastlock_lock(&bdmfts->lock);
    list_del_init(&msg->queue);
    bdmf_fastlock_unlock(&bdmfts->lock);

    /* handle "done" message */
    if (bdmfts_msg_type(msg) == BDMFTS_MSGTYPE_DONE)
    {
        bdmfts_notify_tx_done(qd->bdmfts, msg);
        return;
    }

    /* Read data */
    payload_size = bdmfts_msg_data_length(msg);
    if (payload_size)
    {
        if (!msg->data && !bdmfts_shm_msg_payload_alloc(bdmfts, msg, payload_size))
            goto rx_error;
    }

    bdmfts_notify_rx(qd->bdmfts, msg);
    return;

    /*
     * Error handling
     */
rx_error:
    bdmfts_tmsg_status_set(&msg->tmsg, BDMFTS_STATLCL_COMM_ERROR);
    bdmfts_notify_rx(bdmfts, msg);
}

inline static void bdmfts_shm_handle_rx_queue(struct bdmfts_shm_qd *qd)
{
    //bdmf_print("%s: read=%u write=%u\n", __FUNCTION__, qd->rx_read_idx, *qd->prx_write);
    /* Get all queued messages */
    while(qd->rx_read_idx != *qd->prx_write)
    {
        uint16_t qd_idx=qd->arx[qd->rx_read_idx];
        assert(qd_idx < qd->qsize);
        //bdmf_print("%s: go message @ %u. qdidx=%u\n", __FUNCTION__, qd->rx_read_idx, qd_idx);
        if (++qd->rx_read_idx >= qd->rx_ring_size)
            qd->rx_read_idx = 0;
        _bdmfts_shm_recv(qd, qd_idx);
    }
}

/* ISR
 */
static int bdmfts_shm_isr(int irq, void *data)
{
    struct bdmfts_shm_qd *qd = data;
    bdmf_task_kick(qd->rx_lock);
    return 0;
}


/* Receive handler */
static void bdmfts_shm_rx_thread_handler(void *arg)
{
    bdmfts_session_t *bdmfts=arg;
    struct bdmfts_shm_qd *qd = (struct bdmfts_shm_qd *)((long)bdmfts + sizeof(*bdmfts));
    do
    {
        bdmfts_shm_handle_rx_queue(qd);
        bdmf_task_wait(qd->rx_lock);
    } while(!bdmfts->killed);
}

/* close domain socket session */
void bdmfts_shm_close(bdmfts_session_t *bdmfts)
{
    struct bdmfts_shm_qd *qd = (struct bdmfts_shm_qd *)((long)bdmfts + sizeof(*bdmfts));

    /* ToDo: Release all payloads */

    if (qd->my_irq)
    {
        bdmf_irq_free(qd->my_irq, bdmfts_shm_isr, qd);
        qd->my_irq = 0;
    }
    if (bdmfts->rx_thread)
    {
        bdmf_task_destroy(bdmfts->rx_thread);
        bdmfts->rx_thread = 0;
    }

#ifdef BDMF_SYSTEM_SIM
    if (qd->shm_descr)
    {
        munmap(qd->shm_descr, qd->shbuf_size);
        qd->shm_descr = NULL;
    }
#else
#error Platform is not supported
    /* Release and unmap here */
#endif

    asessions[qd->my_session] = NULL;
}


int bdmfts_shm_open(bdmfts_session_t *bdmfts)
{
    bdmf_config_cmd_resp_t *cmd_resp=bdmfts->parm.data;
    struct bdmfts_shm_qd *qd = (struct bdmfts_shm_qd *)((long)bdmfts + sizeof(*bdmfts));
    bdmf_subsystem_t client;
    bdmf_subsystem_t server;
    int peer_free_start, peer_free_end;
    volatile uint16_t *c2s, *s2c;
    uint32_t shbuf_size;
    int i;
    int rc=0;

    /* server:
     * - wait for "create queue" request in the configuration area
     * - allocate message descriptor, request and reply arrays in the shared area
     * - allocate auxiliary info array
     * - write shared array base addresses into the configuration area
     * - write ready into the config area
     * - wait for signal indicating that new message has arrived
     *
     * client:
     * - write "create queue" request in the configuration area
     * - poll configuration area until ready
     *
     */

    if (!cmd_resp)
    {
        /* Connection is always initiated on the client side.
         * if we are on the client side - send configuration request
         * and wait for reply.
         * On the server side - wait for request
         */
        bdmf_platform_config_area_t *config;
        if (bdmfts->role == BDMFTS_ROLE_SERVER)
        {
            bdmf_print_error("SHM: connection must be initiated by the client. Use \"wait\" command\n");
            return BDMF_ERR_NOT_SUPPORTED;
        }
        config = bdmf_platform_config_get(bdmfts->parm.subsystem);
        if (!config)
            return BDMF_ERR_INTERNAL;
        cmd_resp = &config->cmd_resp;
        rc = bdmfts_shm_send_connect_and_wait(bdmfts, cmd_resp);
        if (rc)
            return rc;
    }

    /* Find free session slot */
    bdmf_fastlock_lock(&bdmfts->lock);
    for(i=0; i<BDMFTS_SHM_MAX_SESSIONS; i++)
    {
        if (!asessions[i])
        {
            asessions[i] = qd;
            break;
        }
    }
    bdmf_fastlock_unlock(&bdmfts->lock);
    if (i == BDMFTS_SHM_MAX_SESSIONS)
        return BDMF_ERR_TOO_MANY;

    qd->my_session = i;
    qd->qsize = cmd_resp->x.msgq_init.size;
    qd->peer_irq = bdmf_subsystem_irq_base(bdmf_subsystem()) + bdmfts->parm.priority + BDMFTS_SHM_MSGQ_IRQ_BASE;
    qd->my_irq = bdmf_subsystem_irq_base(bdmfts->parm.subsystem) + bdmfts->parm.priority + BDMFTS_SHM_MSGQ_IRQ_BASE;

    if (bdmfts->role == BDMFTS_ROLE_CLIENT)
    {
        client = bdmf_subsystem();
        server = bdmfts->parm.subsystem;
    }
    else
    {
        client = bdmfts->parm.subsystem;
        server = bdmf_subsystem();
    }

    shbuf_size = 2 * sizeof(uint16_t) + (qd->qsize + 1) * 2 * sizeof(uint16_t) +
                    qd->qsize * cmd_resp->x.msgq_init.entry_size;
    qd->shbuf_size = shbuf_size;
#ifdef BDMF_SYSTEM_SIM
    {
        char shm_filename[64];
        /* sim-specific:
         * - open/create/truncate shared memory object
         * - perform memory mapping
         */
        snprintf(shm_filename, sizeof(shm_filename), "/msgd_c%d_s%d_p%d",
            client, server, bdmfts->parm.priority);
        qd->shm_descr = bdmf_mmap(shm_filename, shbuf_size);
    }
#else
#error Platform is not supported
    /* Set qd->amsgd, qd->c2s and qd->s2c here. Map addresses in the local address space */
#endif

    if (cmd_resp)
        memset(qd->shm_descr, 0, shbuf_size);
    qd->atmsgd = (bdmfts_shm_tmsg_t *)((long)qd->shm_descr + 2*sizeof(uint16_t));
    c2s = (void *)((long)qd->atmsgd + qd->qsize * cmd_resp->x.msgq_init.entry_size);
    s2c = (void *)((long)c2s + sizeof(uint16_t)*(qd->qsize+1));
    if (bdmfts->role == BDMFTS_ROLE_CLIENT)
    {
        qd->ptx_write = (uint16_t *)qd->shm_descr;
        qd->prx_write = (uint16_t *)((long)qd->shm_descr + sizeof(uint16_t));
        qd->atx = c2s;
        qd->arx = s2c;
        peer_free_start = qd->qsize / 2;
        peer_free_end = qd->qsize-1;
    }
    else
    {
        qd->prx_write = (uint16_t *)qd->shm_descr;
        qd->ptx_write = (uint16_t *)((long)qd->shm_descr + sizeof(uint16_t));
        qd->atx = s2c;
        qd->arx = c2s;
        peer_free_start = 0;
        peer_free_end = qd->qsize / 2-1;
    }

    /* Take all messages from free list. Messages
     * belonging to the peer will be left dangling and
     * messages belonging to *this* will be returned to the free list
     */
    for (i=0; i<qd->qsize; i++)
        bdmfts_msg_get(bdmfts);

    /* Init local message descriptor array */
    qd->tmsg_size = cmd_resp->x.msgq_init.entry_size;
    for (i=0; i<qd->qsize; i++)
    {
        bdmfts_shm_tmsg_t *tmsg = (bdmfts_shm_tmsg_t *)((long)qd->atmsgd + qd->tmsg_size * i);
        bdmfts->amsg[i].tmsg_id = i;
        bdmfts->amsg[i].drv_priv = (long)tmsg;
        /* return "our" messages to the free list */
        if (i < peer_free_start || i > peer_free_end)
        {
            bdmfts_msg_release(bdmfts, &bdmfts->amsg[i]);
            bdmfts->amsg[i].is_locally_allocated = 1;
        }
#ifdef BDMF_SYSTEM_SIM
        if (bdmfts->role == BDMFTS_ROLE_CLIENT)
            tmsg->payload = (server << 28) | (rand() & 0x0fffffff);
#endif
    }

    /* All good. Finish init */
    qd->rx_ring_size = qd->qsize+1;
    qd->tx_ring_size = qd->qsize+1;
    bdmfts->msg_send = bdmfts_shm_send;
    bdmfts->msg_alloc = bdmfts_shm_msg_alloc;
    bdmfts->msg_free = bdmfts_shm_msg_free;
    bdmfts->close = bdmfts_shm_close;
    bdmfts->transport_header_size = sizeof(bdmfts_shm_tmsg_t);
    qd->bdmfts = bdmfts;

    /* Create receive thread */
    bdmf_fastlock_init(&qd->rx_lock);
    rc = bdmf_task_create("shm_rx", BDMFSYS_DEFAULT_TASK_PRIORITY,
                    BDMFSYS_DEFAULT_TASK_STACK,
                    bdmfts_shm_rx_thread_handler, bdmfts,
                    &bdmfts->rx_thread);
    if (rc)
        goto error_cleanup;

    /* Almost done. Connect ISR */
    rc = bdmf_irq_connect(qd->my_irq, bdmfts_shm_isr, qd);
    if (rc)
        goto error_cleanup;

    bdmfts->connected = 1;
    return 0;

error_cleanup:
    bdmfts_shm_close(bdmfts);
    return rc;
}

static struct bdmfts_transport shm_transport = {
    .open = bdmfts_shm_open,
    .extra_size = sizeof(struct bdmfts_shm_qd)
};

int bdmfts_shm_init(void)
{
    return bdmfts_transport_register(BDMFTS_CONN_SHM, &shm_transport);
}

