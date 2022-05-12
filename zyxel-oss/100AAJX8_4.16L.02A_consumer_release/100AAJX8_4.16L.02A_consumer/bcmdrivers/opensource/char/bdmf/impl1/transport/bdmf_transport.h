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
 * bdmf_transport.h
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
#ifndef _BL_TRANSPORT_H_
#define _BL_TRANSPORT_H_

#include <bdmf_system.h>
#include <bdmf_platform.h>
#include <bdmf_transport_msg.h>

/** \defgroup ipc Interprocessor communication
 * Types and APIs in this group provide inter-processor communication services
 */

/** \defgroup transport Low-level transport service
 * \ingroup ipc
 * Types and APIs in this chapter provide low-level transport services.
 * Higher-level services (e.g., RPC) are built on top of the low-level transport services.
 * Transport services support communication between cores of a multi-core system and also
 * with external processor(s).
 * @{
 */

/* Transport message payload size */
#define BDMF_TRANSPORT_PAYLOAD_SIZE    8192

/** Transport connection type
 */
typedef enum
{
    BDMFTS_CONN_DOMAIN_SOCKET,        /**< Domain socket */
    BDMFTS_CONN_SHM,                  /**< Shared memory area */
    BDMFTS_CONN_PROXY,                /**< Proxy transport */
    BDMFTS_CONN__NUMBER_OF
} bdmfts_conn_type_t;

/** Transport priority
 */
typedef enum
{
    BDMFTS_PRTY0,                     /**< Priority 0 - lowest */
    BDMFTS_PRTY1,                     /**< Priority 1 */
    BDMFTS_PRTY2,                     /**< Priority 2 */
    BDMFTS_PRTY3,                     /**< Priority 3 - highest */
    BDMFTS_PRTY_LAST = BDMFTS_PRTY3,
    BDMFTS_PRTY__NUMBER_OF
} bdmfts_prty_t;

/** Transport session handle
 */
typedef struct bdmfts_session *bdmfts_handle;

/** Transport session parameters
 */
typedef struct bdmfts_session_parm
{
    void *user_priv;                /**< Handle that is passed transparently to handler() */
    bdmf_subsystem_t subsystem;       /**< Target subsystem */
    bdmfts_conn_type_t conn_type;     /**< Connection type */
    bdmfts_prty_t priority;           /**< Connection priority */
    void *data;                     /**< Address in string form or other data */
    int max_qsize;                  /**< Max number of message queue entries */
#define BDMFTS_DEFAULT_MAX_QSIZE   64     /**< Default max message queue size */
} bdmfts_session_parm_t;

/** Transport message descriptor
 * Note that the following structure is for local use only.
 * It is not what is actually transferred on the line.
 */
typedef struct bdmfts_msg
{
    bdmfts_tmsg_t tmsg;             /**< Transport header */
    void *data;                     /**< Local data pointer */
    long user_priv;                 /**< For application use */
    long drv_priv;                  /**< For driver use */
    bdmf_mutex lock;                /**< Semaphore for FD service use. Locked initially */
    TAILQ_ENTRY(bdmfts_msg) queue;  /**< Message queue */
    long timestamp;                 /**< Timestamp when message has transmitted */
    uint32_t data_size;             /**< Data size */
    uint32_t tmsg_id;               /**< Transport block id - for internal use */
    short int rc;                   /**< Error code */
    char is_free;                   /**< 1=descriptor is free */
    char is_locally_allocated;      /**< 1=payload is locally allocated */
} bdmfts_msg_t;

/** Parameters of server utilizing transport service
 */
typedef struct bdmfts_server_parm
{
    bdmfts_msgtype_t msg_type;        /**< Message type - used for message de-multiplexing */
    const char *name;               /**< Transport user name */
    void (*receive)(bdmfts_handle bdmfts, void *server_priv, bdmfts_msg_t *msg);  /**< Receive callback */
    void (*txdone)(bdmfts_handle bdmfts, void *server_priv, bdmfts_msg_t *msg);   /**< Done callback */
    void (*connect)(bdmfts_handle bdmfts, void *server_priv);                   /**< Optional "connect" indication */
    void (*disconnect)(bdmfts_handle bdmfts, void *server_priv);                /**< Optional "disconnect" indication */
    uint32_t flags;                 /**< Server flags */
    void *server_priv;              /**< Server's private data */
} bdmfts_server_parm_t;

/** Initialise transport service module
 * \return  0 - OK\n
 *         <0 - error code
 */
int bdmfts_module_init(void);

/** Un-initialise transport service module
 */
void bdmfts_module_exit(void);

/*
 * Transport session control
 */

/** Register transport message server
 * Message server can be registered per message type.
 * Transport service de-multiplexes messages based on the message type
 * and delivers to the appropriate server.
 * \param[in]   server      Server registration parameters. Must NOT be allocated on the stack
 * \return  0 - OK\n
 *         <0 - error code
 */
int bdmfts_server_register(bdmfts_server_parm_t *server);

/** Unregister transport message server
 * \param[in]   server      Server registration parameters. Must NOT be allocated on the stack
 * \return  0 - OK\n
 *         <0 - error code
 */
int bdmfts_server_unregister(bdmfts_server_parm_t *server);

/** Get transport message server parameters
 * \param[in]   msg_type    Message type
 * \return message server block or NULL if server is not registered for the message type
 */
const bdmfts_server_parm_t *bdmfts_server_get(bdmfts_msgtype_t msg_type);

/** Connect transport session
 * \param[in]   session_parm    Transport session parameters
 * \param[out]  pbdmfts           Session handle
 * \return  0 - OK\n
 *         <0 - error code
 */
int bdmfts_connect(bdmfts_session_parm_t *session_parm, bdmfts_handle *pbdmfts);

/** Wait for incoming connection
 * \param[in]   session_parm    Transport session parameters
 * \param[out]  pbdmfts           Session handle
 * \return  0 - OK\n
 *         <0 - error code
 */
int bdmfts_listen(bdmfts_session_parm_t *session_parm, bdmfts_handle *pbdmfts);

/** Disconnect transport session
 * \param[in]   bdmfts            Session handle returned by bdmfts_connect() or bdmfts_listen().
 *                              The session handle becomes invalid following this call.
 */
void bdmfts_disconnect(bdmfts_handle bdmfts);

/** Get session handle for transport connection with the given subsystem at given priority
 * \param[in]   subsystem       Target subsystem
 * \param[in]   priority        Transport priority
 * \return  bdmfts session handle or NULL if not connected
 */
bdmfts_handle bdmfts_session(bdmf_subsystem_t subsystem, bdmfts_prty_t priority);

/** Get session parameters
 * \param[in]   bdmfts            Transport session handle
 * \return session parameter structure
 */
const bdmfts_session_parm_t *bdmfts_session_parms(bdmfts_handle bdmfts);


/*
 * Message field access
 */

/** Get message type
 * \param[in]   msg             Message block
 * \return message type
 */
static inline bdmfts_msgtype_t bdmfts_msg_type(const bdmfts_msg_t *msg)
{
    return bdmfts_tmsg_type(&msg->tmsg);
}

/** Set message type
 * \param[in]   msg             Message block
 * \param[in]   msg_type        Message type
 */
static inline void bdmfts_msg_type_set(bdmfts_msg_t *msg, bdmfts_msgtype_t msg_type)
{
    bdmfts_tmsg_type_set(&msg->tmsg, msg_type);
}

/** Get message payload
 * \param[in]   msg             Message block
 * \return payload pointer
 */
static inline void *bdmfts_msg_data(const bdmfts_msg_t *msg)
{
    return msg->data;
}

/** Get message data length
 * \param[in]   msg             Message block
 * \return payload length
 */
static inline uint32_t bdmfts_msg_data_length(const bdmfts_msg_t *msg)
{
    return bdmfts_tmsg_payload_length(&msg->tmsg);
}

/** Set message data length
 * \param[in]   msg             Message block
 * \param[in]   length          Payload length
 */
static inline void bdmfts_msg_data_length_set(bdmfts_msg_t *msg, uint32_t length)
{
    assert(length <= msg->data_size);
    bdmfts_tmsg_payload_length_set(&msg->tmsg, length);
}

/** Get message data buffer size
 * \param[in]   msg             Message block
 * \return data buffer size
 */
static inline uint32_t bdmfts_msg_databuf_size(const bdmfts_msg_t *msg)
{
    return msg->data_size;
}

/** Get message sender
 * \param[in]   msg             Message block
 * \returns sender's subsystem
 */
static inline bdmf_subsystem_t bdmfts_msg_sender(const bdmfts_msg_t *msg)
{
    return bdmfts_tmsg_source(&msg->tmsg);
}

/** Is message a reply
 * \param[in]   msg             Message block
 * \returns true(!=0) if message is a reply
 */
static inline int bdmfts_msg_is_reply(const bdmfts_msg_t *msg)
{
    return (bdmfts_tmsg_is_full_duplex(&msg->tmsg) &&
            bdmfts_tmsg_source(&msg->tmsg) == bdmf_subsystem());
}

/** Is message a request
 * \param[in]   msg             Message block
 * \returns true(!=0) if message is a request
 */
static inline int bdmfts_msg_is_request(const bdmfts_msg_t *msg)
{
    return (bdmfts_tmsg_is_full_duplex(&msg->tmsg) &&
            bdmfts_tmsg_destination(&msg->tmsg) == bdmf_subsystem());
}

/** Is message a full-duplex (request or reply)
 * \param[in]   msg             Message block
 * \returns true(!=0) if message is full-duplex
 */
static inline int bdmfts_msg_is_full_duplex(const bdmfts_msg_t *msg)
{
    return bdmfts_tmsg_is_full_duplex(&msg->tmsg);
}

/** Set "full duplex" in message header
 * \param[in]   msg             Message block
 */
static inline void bdmfts_msg_full_duplex_set(bdmfts_msg_t *msg)
{
    bdmfts_tmsg_full_duplex_set(&msg->tmsg, 1);
}


/*
 * Transport services
 */

/** Allocate transport message
 * \param[in]   bdmfts            Transport session handle
 * \param[in]   payload_size    Message payload size
 * \param[in]   payload         Message payload pointer. If set, the function will try to use zero-copy,
 *                              depending on the underlying transport mechanism. If not set,
 *                              message payload is allocated internally.
 * \return  message block pointer if success\n
 *          NULL if failure
 */
bdmfts_msg_t *bdmfts_msg_alloc(bdmfts_handle bdmfts, uint32_t payload_size, void *payload);

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
int bdmfts_msg_send(bdmfts_handle bdmfts, bdmfts_msg_t *msg);

/** Send reply
 * \param[in]   bdmfts        Session handle passed to the server in its "receive" callback
 * \param[in]   msg         Message passed to the "receive" callback, possibly modified
 * per-message type txdone callback is always called eventually,
 * regardless on the delivery outcome.
 * \return  0 - OK\n
 *         <0 - delivery error
 */
int bdmfts_msg_reply(bdmfts_handle bdmfts, bdmfts_msg_t *msg);

/** Release message block
 * \param[in]   bdmfts        Transport session handle
 * \param[in]   msg         Message block
 */
void bdmfts_msg_free(bdmfts_handle bdmfts, bdmfts_msg_t *msg);


#ifdef BDMF_SHELL
bdmfmon_handle_t bdmfts_mon_init(bdmfmon_handle_t root_dir);
#endif

/** @} end of transport group */

#endif /* _BL_TRANSPORT_H_ */
