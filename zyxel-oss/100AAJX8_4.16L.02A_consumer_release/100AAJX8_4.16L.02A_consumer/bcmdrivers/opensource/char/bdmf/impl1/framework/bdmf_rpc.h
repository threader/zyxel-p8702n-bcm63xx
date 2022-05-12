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
 * Data path builder - RPC support header file
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
#ifndef _BDMF_RPC_H_
#define _BDMF_RPC_H_

/** Remote BDMF handle */
typedef long bdmf_remote_handle;

#ifdef BDMF_RPC

#include <bdmf_transport.h>
#include <bdmf_rpc_engine.h>

/** \defgroup bdmf_rpc BDMF Remote Procedure Call service
 * \ingroup ipc
 * Types and APIs in this chapter provide support for BDMF remoting.
 * That is, transparent access to BDMF objects located on another system.
 * @{
 */

/** BDMF RPC function calls
 */
typedef enum
{
    BDMF_RPCF__NONE,

    BDMF_RPCF_TYPE_GET_NEXT,    /**< bdmf_type_get_next() */
    BDMF_RPCF_TYPE_INFO,        /**< bdmf_type_info() */
    BDMF_RPCF_AGGR_GET_NEXT,    /**< bdmf_aggr_get_next() */
    BDMF_RPCF_AGGR_ATTR_INFO,   /**< bdmf_aggr_attr_info() */
    BDMF_RPCF_ATTR_INFO,        /**< bdmf_attr_info() */
    BDMF_RPCF_NEW,              /**< bdmf_new() */
    BDMF_RPCF_DESTROY,          /**< bdmf_destroy() */
    BDMF_RPCF_CONFIGURE,        /**< bdmf_configure() */
    BDMF_RPCF_FIND_GET,         /**< bdmf_find_get() */
    BDMF_RPCF_GET_NEXT,         /**< bdmf_get_next() */
    BDMF_RPCF_GET_NEXT_CHILD,   /**< bdmf_get_next_child() */
    BDMF_RPCF_ATTR_READ,        /**< attr->read() */
    BDMF_RPCF_ATTR_WRITE,       /**< attr->write() */
    BDMF_RPCF_ATTR_VAL_TO_S,    /**< attr->val_to_s() */
    BDMF_RPCF_ATTR_S_TO_VAL,    /**< attr->s_to_val() */
    BDMF_RPCF_ATTR_INDEX_TO_S,  /**< attr->index_to_s() */
    BDMF_RPCF_ATTR_S_TO_INDEX,  /**< attr->s_to_index() */
    BDMF_RPCF_ATTR_FIND,        /**< attr->find() */
    BDMF_RPCF_ATTR_ADD,         /**< attr->add() */
    BDMF_RPCF_ATTR_DELETE,      /**< attr->delete() */
    BDMF_RPCF_ATTR_GET_NEXT,    /**< attr->get_next() */

    BDMF_RPCF__NUMBER_OF
} bdmf_rpc_function_t;

#define BDMF_RPC_TRANSPORT_PRTY     BDMFTS_PRTY0      /**< BDMF RPC transport priority */

/** Get BDMF RPC function name (for tracing)
 * \param[in]   function        BDMF function code
 * \return  function name
 */
const char *bdmf_rpc_function_name(bdmf_rpc_function_t function);

/** Allocate RPC message for use by BDMF RPC sub-system
 * \param[in]   subs            Remote subsystem
 * \param[in]   function        BDMF function code
 * \return  rpc message pointer or NULL
 */
bdmfts_msg_t *bdmf_rpc_msg_alloc(bdmf_subsystem_t subs, bdmf_rpc_function_t function);

/** Release RPC message allocated by bdmf_rpc_msg_alloc
 * \param[in]   subs            Remote subsystem
 * \param[in]   msg             Message to be released
 */
void bdmf_rpc_msg_free(bdmf_subsystem_t subs, bdmfts_msg_t *msg);

/** Make RPC call to BDMF located at remote subsystem
 * \param[in]       subs        Target subsystem
 * \param[in,out]   msg         RPC message
 * \return  0 = OK\n
 *         <0 - communication or remote error
 */
int bdmf_rpc_call(bdmf_subsystem_t subs, bdmfts_msg_t *msg);

/** Register BDMF RPC server
 * \return  0 = OK\n
 *         <0 - error
 */
int bdmf_rpc_server_register(void);


/*
 * Pointer <-> communication handle mapping
 */

/** Store pointer in translation table
 * \param[in]   ptr     Pointer to be stored
 * \return communication handle or 0 in case of error
 */
bdmf_remote_handle bdmf_rpc_ptr_store(void *ptr);

/** Get pointer to BDMF type given its handle
 * \param[in]   handle  Communication handle
 * \return pointer or NULL if not found or doesn't point to struct bdmf_type.
 * Note that the function increments the types's usecount
 */
struct bdmf_type *bdmf_rpc_handle_to_type(bdmf_remote_handle handle);

/** Get pointer to BDMF object given its handle
 * \param[in]   handle  Communication handle
 * \return pointer or NULL if not found or doesn't point to struct bdmf_object.
 * Note that the function increments the object's usecount
 */
struct bdmf_object *bdmf_rpc_handle_to_object(bdmf_remote_handle handle);

/** Remove handle from translation table
 *  \param[in]  handle  Communication handle to be removed
 */
void bdmf_rpc_handle_remove(bdmf_remote_handle handle);

/** @} end of bdmf_rpc group */

#endif /* BDMF_RPC */

#endif /* _BDMF_RPC_H_ */
