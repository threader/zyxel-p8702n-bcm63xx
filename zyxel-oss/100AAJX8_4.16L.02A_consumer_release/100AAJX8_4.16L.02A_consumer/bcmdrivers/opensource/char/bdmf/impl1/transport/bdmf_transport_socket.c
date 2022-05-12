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
 * bdmf_rpc_domain_socket.c
 *
 * BDMF framework - RPC service. Domain socket - based communication
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

/* ToDo: the following implementation is intended to work in a single-threaded
 * environment.
 * In result, it can't handle unsolicited messages (indications)
 * This implementation has to be modified so that receive happens in a separate thread
 */

#define _BL_TRANSPORT_SERVICE_

#include <bdmf_transport_internal.h>
#include <bdmf_transport_socket.h>

/* Read message */
static int socket_rx_msg(bdmfts_session_t *bdmfts)
{
    struct bdmfts_socket_msg hdr;
    struct bdmfts_msg *msg=NULL;
    uint16_t payload_length;
    void *payload;
    int n, i;
    int rc;

    /* Read header first. If it is a request - allocate
     * a new message. Otherwise, look for request block
     */
    n = recv(bdmfts->session_data1, &hdr, sizeof(hdr), MSG_WAITALL);
    if (n < sizeof(hdr))
    {
        bdmf_print_error("short header. got %d bytes\n", n);
        goto rx_error;
    }
    payload_length = bdmfts_tmsg_payload_length(&hdr.tmsg);
    if (bdmfts_rx_is_reply(bdmfts, &hdr.tmsg))
    {
        msg = bdmfts_msg_get_by_idx(bdmfts, hdr.msg_idx);
        if (!msg)
        {
            bdmf_print_error("can't find request block %d\n", hdr.msg_idx);
            rc = BDMF_ERR_MSG_ERROR;
            goto alloc_error;
        }
        bdmf_fastlock_lock(&bdmfts->lock);
        TAILQ_REMOVE(&bdmfts->txq_list, msg, queue);
        bdmf_fastlock_unlock(&bdmfts->lock);
        /* Some sanity checks */
        if (payload_length > bdmfts_msg_databuf_size(msg))
            goto msg_error;
    }
    else
    {
        msg = bdmfts_msg_alloc(bdmfts, BDMF_TRANSPORT_PAYLOAD_SIZE, NULL);
        if (!msg)
        {
            bdmf_print_error("msg allocation failed\n");
            rc = BDMF_ERR_NORES;
            goto alloc_error;
        }
        msg->drv_priv = hdr.msg_idx;
    }
    payload = msg->data;
    msg->tmsg = hdr.tmsg;
    /* Read data */
    if (payload_length)
    {
        n = recv(bdmfts->session_data1, payload, payload_length, MSG_WAITALL);
        if (n < payload_length)
        {
            bdmf_print_error("short payload. expected %d got %d bytes\n", payload_length, n);
            goto rx_error;
        }
    }
    bdmfts_notify_rx(bdmfts, msg);
    return 0;

    /*
     * Error handling
     */
rx_error:
    if (msg)
    {
        bdmfts_tmsg_status_set(&msg->tmsg, BDMFTS_STATLCL_COMM_ERROR);
        bdmfts_notify_rx(bdmfts, msg);
    }
    return BDMF_ERR_COMM_FAIL;

alloc_error:
    /* read payload_length from socket in order to sync */
    for(i=0; i<payload_length; i++)
    {
        char c;
        n = recv(bdmfts->session_data1, &c, 1, MSG_WAITALL);
        if (n <= 0)
            return BDMF_ERR_COMM_FAIL;
    }
    /* now send fake reply if necessary to inform the opposite side that
     * there was a problem
     */
    if (bdmfts_rx_is_request(bdmfts, &hdr.tmsg))
    {
        bdmfts_tmsg_status_set(&hdr.tmsg, BDMFTS_STATRMT_NO_RESOURCES);
        bdmfts_tmsg_payload_length_set(&hdr.tmsg, 0);
        send(bdmfts->session_data1, &hdr, sizeof(hdr), 0);
    }
    return rc;

msg_error:
    /* message data error */
    BDMFTS_ERR(bdmfts, "erroneous message ignored\n");
    return BDMF_ERR_MSG_ERROR;
}


/** Open domain-socket based comm session
 */
static int bdmfts_socket_bind_connect(bdmfts_session_t *bdmfts)
{
    struct sockaddr_un sa;
    char *address;
    int len;
    int rc;

    address = bdmfts->parm.data;
    if (bdmfts->session_data1)
    {
        close(bdmfts->session_data1);
        bdmfts->session_data1 = 0;
    }
    if (bdmfts->session_data2)
    {
        close(bdmfts->session_data2);
        bdmfts->session_data2 = 0;
    }
    bdmfts->session_data1 = socket(AF_UNIX, SOCK_STREAM, 0);
    if (bdmfts->session_data1 < 0)
    {
        bdmf_print_error("Can't create domain socket. errno=%d", errno);
        return BDMF_ERR_PARM;
    }
    if (bdmfts->role == BDMFTS_ROLE_SERVER)
    {
        bdmfts->session_data2 = bdmfts->session_data1;
        bdmfts->session_data1 = 0;
        unlink(address);
    }

    memset(&sa, 0, sizeof(sa));
    sa.sun_family = AF_UNIX;  /* local is declared before socket() ^ */
    strcpy(sa.sun_path, address);
    len = strlen(sa.sun_path) + sizeof(sa.sun_family);

    if (bdmfts->role == BDMFTS_ROLE_SERVER)
    {
        rc = bind(bdmfts->session_data2, (struct sockaddr *)&sa, len);
        if (rc < 0)
            return BDMF_ERR_PARM;
        rc = listen(bdmfts->session_data2, 1);
        if (rc < 0)
            return BDMF_ERR_PARM;
    }
    else
    {
        rc = connect(bdmfts->session_data1, (struct sockaddr *)&sa, len);
        /* keep trying if connection is refused or there is no file. Perhaps
         * server is not running yet
         */
        if (!rc)
            bdmfts->connected = 1;
        else if ((errno != ECONNREFUSED) && (errno != ENOENT))
            return BDMF_ERR_PARM;

    }
    return 0;
}


/* Receive handler */
static int socket_rx_thread_handler(void *arg)
{
    bdmfts_session_t *bdmfts=arg;
    struct sockaddr_un remote;
    socklen_t t = sizeof(remote);
    while(!bdmfts->killed)
    {
        if (bdmfts->role == BDMFTS_ROLE_SERVER)
        {
            BDMFTS_INFO(bdmfts, "Waiting for a connection...\n");
            if ((bdmfts->session_data1 = accept(bdmfts->session_data2, (struct sockaddr *)&remote, &t)) == -1)
            {
                perror("accept");
                break;
            }
            bdmfts->connected = 1;
        }

        /* Receive messages */
        if (bdmfts->connected)
        {
            BDMFTS_INFO(bdmfts, "Connected...\n");
            while(socket_rx_msg(bdmfts) != BDMF_ERR_COMM_FAIL)
                ;
            bdmfts->connected = 0;
            bdmfts_notify_disconnect(bdmfts);
            BDMFTS_INFO(bdmfts, "Connection was broken...\n");
        }

        if (bdmfts->role == BDMFTS_ROLE_CLIENT)
        {
            while(bdmfts_socket_bind_connect(bdmfts))
                usleep(100);
        }
        else
        {
            close(bdmfts->session_data1);
            bdmfts->session_data1 = 0;
        }
    }
    return 0;
}

/* send callback */
static int bdmfts_socket_send(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{
    struct bdmfts_socket_msg smsg;
    int n1, n2;

    if (!bdmfts->session_data1 || !bdmfts->connected)
        return BDMF_ERR_NOT_CONNECTED;
    smsg.tmsg = msg->tmsg;
    smsg.msg_idx = msg->drv_priv;
    bdmf_fastlock_lock(&bdmfts->lock);
    n1 = send(bdmfts->session_data1, &smsg, sizeof(smsg), 0);
    n2 = send(bdmfts->session_data1, msg->data, bdmfts_msg_data_length(msg), 0);
    if (n1 < sizeof(smsg) || n2 < bdmfts_msg_data_length(msg))
    {
        bdmf_fastlock_unlock(&bdmfts->lock);
        return BDMF_ERR_COMM_FAIL;
    }
    /* report txdone immediately if reply is not expected */
    if ( bdmfts_tx_is_request(bdmfts, &msg->tmsg) )
    {
        TAILQ_INSERT_TAIL(&bdmfts->txq_list, msg, queue);
        bdmf_fastlock_unlock(&bdmfts->lock);
    }
    else
    {
        bdmf_fastlock_unlock(&bdmfts->lock);
        bdmfts_notify_tx_done(bdmfts, msg);
    }
    return 0;
}

/* allocate message callback */
bdmfts_msg_t *bdmfts_socket_msg_alloc(bdmfts_session_t *bdmfts, uint32_t payload_size, void *payload)
{
    bdmfts_msg_t *msg;
    bdmf_fastlock_lock(&bdmfts->lock);
    msg = bdmfts_msg_get(bdmfts);
    bdmf_fastlock_unlock(&bdmfts->lock);
    if (!msg)
        return NULL;
    msg->is_locally_allocated = 0;
    if (!payload && payload_size)
    {
        payload = bdmf_alloc(payload_size);
        if (!payload)
        {
            bdmf_fastlock_lock(&bdmfts->lock);
            bdmfts_msg_release(bdmfts, msg);
            bdmf_fastlock_unlock(&bdmfts->lock);
            return NULL;
        }
        msg->is_locally_allocated = 1;
    }
    msg->data = payload;
    msg->data_size = payload_size;
    msg->drv_priv = bdmfts_msg_idx(bdmfts, msg);
    return msg;
}

/* release transport message callback */
void bdmfts_socket_msg_free(bdmfts_session_t *bdmfts, bdmfts_msg_t *msg)
{
    if (msg->is_locally_allocated && msg->data)
    {
        bdmf_free(msg->data);
        msg->data = NULL;
    }
    bdmf_fastlock_lock(&bdmfts->lock);
    bdmfts_msg_release(bdmfts, msg);
    bdmf_fastlock_unlock(&bdmfts->lock);
}

/* close domain socket session */
void bdmfts_socket_close(bdmfts_session_t *bdmfts)
{
    bdmfts->connected = 0;
    bdmfts->killed = 0;
    if (bdmfts->rx_thread)
    {
        bdmf_task_destroy(bdmfts->rx_thread);
        bdmfts->rx_thread = 0;
    }
    if (bdmfts->session_data1)
    {
        close(bdmfts->session_data1);
        bdmfts->session_data1 = 0;
    }
    if (bdmfts->session_data2)
    {
        close(bdmfts->session_data2);
        bdmfts->session_data2 = 0;
    }
    /* address in stored session's extra data */
    if (bdmfts->parm.data)
    {
        bdmf_free(bdmfts->parm.data);
        bdmfts->parm.data = NULL;
    }
}

/** Open domain-socket based comm session
 */
int bdmfts_socket_open(bdmfts_session_t *bdmfts)
{
    int rc;
    char *address;

    if (!bdmfts->parm.data)
        return BDMF_ERR_PARM;

    address = bdmf_alloc(strlen((char *)bdmfts->parm.data)+1);
    if (!address)
        return BDMF_ERR_NOMEM;
    strcpy(address, (char *)bdmfts->parm.data);
    bdmfts->parm.data = address;
    rc = bdmfts_socket_bind_connect(bdmfts);
    if (rc)
    {
        close(bdmfts->session_data1);
        bdmf_free(address);
        bdmf_print_error("bind or connect failed. errno=%d - %s\n",
                            errno, strerror(errno));
        return rc;
    }
    bdmfts->msg_send = bdmfts_socket_send;
    bdmfts->msg_alloc = bdmfts_socket_msg_alloc;
    bdmfts->msg_free = bdmfts_socket_msg_free;
    bdmfts->close = bdmfts_socket_close;
    bdmfts->transport_header_size = sizeof(struct bdmfts_socket_msg);

    /* Create receive thread */
    rc = bdmf_task_create("socket_rx", BDMFSYS_DEFAULT_TASK_PRIORITY,
                    BDMFSYS_DEFAULT_TASK_STACK,
                    socket_rx_thread_handler, bdmfts,
                    &bdmfts->rx_thread);
    if (rc)
        goto error_cleanup;

    /* Underlying full-duplex channel is ready. Set-up session */
    return 0;

error_cleanup:
    bdmfts_socket_close(bdmfts);
    return rc;
}


static struct bdmfts_transport socket_transport = {
    .open = bdmfts_socket_open
};

int bdmfts_socket_init(void)
{
    return bdmfts_transport_register(BDMFTS_CONN_DOMAIN_SOCKET, &socket_transport);
}
