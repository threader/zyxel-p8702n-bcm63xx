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
 * bdmf_transport_msg.h
 *
 * Inter-processor transport service
 * Transport message structure and access functions
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

#ifndef _BL_TRANSPORT_MSG_H_
#define _BL_TRANSPORT_MSG_H_

#include <bdmf_system.h>

/** Transport message type
 * Transport layer de-multiplex incoming messages based
 * on the message type
 */
typedef enum
{
    BDMFTS_MSGTYPE_DONE,             /**< TxDone message - transport internal */
    BDMFTS_MSGTYPE_PING,             /**< Ping message */
    BDMFTS_MSGTYPE_RPC,              /**< RPC service */
    BDMFTS_MSGTYPE_ETHER,            /**< Virtual Ethernet interface */
    BDMFTS_MSGTYPE_UDEF1,
    BDMFTS_MSGTYPE_UDEF2,
    BDMFTS_MSGTYPE_UDEF3,
    BDMFTS_MSGTYPE_UDEF4,
    BDMFTS_MSGTYPE_UDEF5,
    BDMFTS_MSGTYPE_UDEF6,
    BDMFTS_MSGTYPE_UDEF7,
    BDMFTS_MSGTYPE_UDEF8,
    BDMFTS_MSGTYPE__NUMBER_OF
} bdmfts_msgtype_t;

/** Transport-independent part of bdmfts_message header
 * ToDo: define access functions in endian-neutral way
 * Please don't access message fields directly!
 * Use bdmfts_tmsg.. access functions.
 */
typedef struct bdmfts_tmsg
{
    bdmfts_msgtype_t _msg_type:8;     /**< Message type */
    uint16_t _status:4;             /**< Delivery status */
#define BDMFTS_STATLCL_NO_RESOURCES  0x01     /**< Local allocation failure */
#define BDMFTS_STATLCL_COMM_ERROR    0x02     /**< Local communication error */
#define BDMFTS_STATLCL_NO_REQUEST    0x03     /**< Can't find request matching the reply */
#define BDMFTS_STATRMT_NO_RESOURCES  0x09     /**< Remote allocation failure */
#define BDMFTS_STATRMT_COMM_ERROR    0x0a     /**< Remote communication error */
#define BDMFTS_STATRMT_NO_SERVER     0x0b     /**< No server to handle the message */
#define BDMFTS_STATRMT_MSG_ERROR     0x0c     /**< Remote side detected error in message encoding */
    bdmf_subsystem_t _src_subs:2;     /**< Request source subsystem */
    bdmf_subsystem_t _dst_subs:2;     /**< Request destination subsystem */
    int _full_duplex:1;             /**< If set - the message is either request expecting re0ply or a reply */
    int _interrupt:1;               /**< If set - sender should interrupt receiver */
    uint16_t _payload_length:14;    /**< Payload length */
#define BDMFTS_MAX_MESSAGE_SIZE 2048          /**< Max message size including header */
} bdmfts_tmsg_t;

/*
 * Message field access functions
 */
static inline uint16_t bdmfts_tmsg_status(const bdmfts_tmsg_t *msg)
{
    return msg->_status;
}

static inline void bdmfts_tmsg_status_set(bdmfts_tmsg_t *msg, uint16_t status)
{
    msg->_status = status;
}

static inline bdmfts_msgtype_t bdmfts_tmsg_type(const bdmfts_tmsg_t *msg)
{
    return msg->_msg_type;
}

static inline void bdmfts_tmsg_type_set(bdmfts_tmsg_t *msg, bdmfts_msgtype_t msg_type)
{
    msg->_msg_type = msg_type;
}

static inline bdmf_subsystem_t bdmfts_tmsg_source(const bdmfts_tmsg_t *msg)
{
    return msg->_src_subs;
}

static inline void bdmfts_tmsg_source_set(bdmfts_tmsg_t *msg, bdmf_subsystem_t source)
{
    msg->_src_subs = source;
}

static inline bdmf_subsystem_t bdmfts_tmsg_destination(const bdmfts_tmsg_t *msg)
{
    return msg->_dst_subs;
}

static inline void bdmfts_tmsg_destination_set(bdmfts_tmsg_t *msg, bdmf_subsystem_t destination)
{
    msg->_dst_subs = destination;
}

static inline uint16_t bdmfts_tmsg_payload_length(const bdmfts_tmsg_t *msg)
{
    return msg->_payload_length;
}

static inline void bdmfts_tmsg_payload_length_set(bdmfts_tmsg_t *msg, uint16_t length)
{
    msg->_payload_length = length;
}

static inline int bdmfts_tmsg_is_full_duplex(const bdmfts_tmsg_t *msg)
{
    return (msg->_full_duplex!=0);
}

static inline void bdmfts_tmsg_full_duplex_set(bdmfts_tmsg_t *msg, int is_fd)
{
    msg->_full_duplex = is_fd;
}

static inline int bdmfts_tmsg_is_interrupt(const bdmfts_tmsg_t *msg)
{
    return (msg->_interrupt != 0);
}

static inline void bdmfts_tmsg_interrupt_set(bdmfts_tmsg_t *msg, int is_interrupt)
{
    msg->_interrupt = is_interrupt;
}

#endif /* _BL_TRANSPORT_MSG_H_ */
