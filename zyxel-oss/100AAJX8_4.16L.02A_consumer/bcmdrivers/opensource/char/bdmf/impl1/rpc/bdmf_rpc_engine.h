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
 * bdmf_rpc.h
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

#ifndef _BDMF_RPC_ENGINE_H_
#define _BDMF_RPC_ENGINE_H_

#include <bdmf_system.h>
#include <bdmf_session.h>
#include <bdmf_transport.h>
#include <bdmf_shell.h>

/** \defgroup rpc Remote Procedure Call service
 * \ingroup ipc
 * Types and APIs in this chapter provide support for RPC
 * @{
 */

/** RPC service module id
 */
typedef enum
{
    BDMF_RPC_MODULE_PING,         /**< Ping */
    BDMF_RPC_MODULE_BDMF,         /**< BDMF */

    BDMF_RPC_MODULE__NUMBER_OF    /**< Must be the last */
} bdmfrpc_module_t;

/** RPC message */
typedef struct bdmfrpc_msg bdmfrpc_msg_t;

/** RPC server parameters structure
 */
typedef struct bdmfrpc_server
{
    const char *name;           /**< Server name */
    bdmfrpc_module_t module;      /**< Unique service module id */
    void *server_priv;          /**< Handle that is passed transparently to handler() */
    bdmf_session_handle session;  /**< Optional printing/logging session handle */
    void (*handler)(bdmfts_handle bdmfts, void *server_priv, bdmfts_msg_t *msg);
} bdmfrpc_server_t;


/** RPC field value type
 */
typedef enum
{
    BDMF_RPC_FIELD_NUMBER,        /**< Number */
    BDMF_RPC_FIELD_STRING,        /**< 0-terminated string */
    BDMF_RPC_FIELD_BUFFER,        /**< buffer */

    BDMF_RPC_FIELD__NUMBER_OF
} bdmfrpc_field_type;


/** RPC number
 */
typedef long bdmfrpc_number;

/** Initialise RPC library
 * \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_module_init(void);


/** Un-initialise RPC library
 */
void bdmfrpc_module_exit(void);


/** Register RPC server
 *  \param[in]  server      Server descriptor
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_server_register(bdmfrpc_server_t *server);


/** Unregister RPC server
 *  \param[in]  server      Server descriptor
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_server_unregister(bdmfrpc_server_t *server);


/** Get RPC service
 *  \param[in]  module      Server id
 *  \param[in]  server      Server descriptor
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_server_get(bdmfrpc_module_t module, const bdmfrpc_server_t **server);


/** Allocate RPC message
 *  \param[in]  bdmfts      Transport session
 *  \param[in]  size        Payload size. 0=Use default
 *  \param[in]  payload     Optional payload buffer. NULL=allocate automatically.
 *  \return
 *     message pointer or NULL if no resources
 */
bdmfts_msg_t *bdmfrpc_msg_alloc(bdmfts_handle bdmfts, uint32_t size, void *payload);


/** Get RPC message header given the transport message
 *  \param[in]  msg         Transport message
 *  \return
 *     RPC header pointer
 */
static inline bdmfrpc_msg_t *bdmfrpc_msg(bdmfts_msg_t *msg)
{
    return bdmfts_msg_data(msg);
}


/** Call remote RPC service
 *  \param[in]  bdmfts        Transport session
 *  \param[in]  msg         RPC message
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_call(bdmfts_handle bdmfts, bdmfts_msg_t *msg);


/** Send indication that doesn't require reply
 *  \param[in]  bdmfts        Transport session
 *  \param[in]  msg         RPC message
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_send(bdmfts_handle bdmfts, bdmfts_msg_t *msg);


/** Reply to RPC call
 *  \param[in]  bdmfts        Transport session
 *  \param[in]  msg         RPC message
 *  \return
 *     0    - OK\n
 *    <0    - error
 */
int bdmfrpc_reply(bdmfts_handle bdmfts, bdmfts_msg_t *msg);


#ifdef BDMF_SHELL
bdmfmon_handle_t bdmfrpc_mon_init(bdmfmon_handle_t root_dir);
#endif

/**  @} */

#include <bdmf_rpc_msg.h>

#endif /* _BDMF_RPC_ENGINE_H_ */
