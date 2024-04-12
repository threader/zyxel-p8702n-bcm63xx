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
 * bdmf_rpc_msg.h
 *
 * BDMF framework - RPC service. Message access helpers
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

#ifndef _BL_RPC_MSG_H_
#define _BL_RPC_MSG_H_

#include <bdmf_rpc_engine.h>

/** \defgroup rpc_msg Remote Procedure Call service - message encoding
 * \ingroup rpc
 * Types and APIs in this chapter provide support for RPC message encoding/decoding.
 * @{
 */

/** RPC message
 * Please don't access message fields directly.
 * Use access functions bdmfrpc_msg_...
 */
struct bdmfrpc_msg
{
    bdmfrpc_module_t _module:8;   /**< Unique service module id. Must be set */
    uint8_t ffu;
    uint16_t _function;         /**< RPC function code. Must be set */
    int16_t _rc;                /**< Return code */
    uint16_t _area_size;        /**< I/O area size */
    uint16_t _parm_size;        /**< I/O parameter block size */
    uint8_t _io_area[0];        /**< I/O area follows */
};

/** Get message target module
 * \param[in]       msg         RPC message
 * \return target module id
 */
static inline bdmfrpc_module_t bdmfrpc_msg_module(const bdmfrpc_msg_t *msg)
{
    return msg->_module;
}

/** Set message target module
 * \param[in]       msg         RPC message
 * \param[in]       module      Target module
 */
static inline void bdmfrpc_msg_module_set(bdmfrpc_msg_t *msg, bdmfrpc_module_t module)
{
    msg->_module = module;
}

/** Get message function code
 * \param[in]       msg         RPC message
 * \return Function code
 */
static inline uint16_t bdmfrpc_msg_function(const bdmfrpc_msg_t *msg)
{
    return msg->_function;
}

/** Set message function code
 * \param[in]       msg         RPC message
 * \param[in]       function    Function code
 */
static inline void bdmfrpc_msg_function_set(bdmfrpc_msg_t *msg, uint16_t function)
{
    msg->_function = function;
}

/** Get message return code
 * \param[in]       msg         RPC message
 * \return Function code
 */
static inline int16_t bdmfrpc_msg_rc(const bdmfrpc_msg_t *msg)
{
    return msg->_rc;
}

/** Set message return code
 * \param[in]       msg         RPC message
 * \param[in]       rc          Return code
 */
static inline void bdmfrpc_msg_rc_set(bdmfrpc_msg_t *msg, int16_t rc)
{
    msg->_rc = rc;
}

/** Get address of message IO area
 * \param[in]       msg         RPC message
 * \return IO area pointer
 */
static inline uint8_t *bdmfrpc_msg_area(bdmfrpc_msg_t *msg)
{
    return msg->_io_area;
}

/** Get message IO area size
 * \param[in]       msg         RPC message
 * \return IO area size
 */
static inline int16_t bdmfrpc_msg_area_size(const bdmfrpc_msg_t *msg)
{
    return msg->_area_size;
}

/** Set message IO area size
 * \param[in]       msg         RPC message
 * \param[in]       size        IO area size
 */
static inline void bdmfrpc_msg_area_size_set(bdmfrpc_msg_t *msg, int16_t size)
{
    msg->_area_size = size;
}

/** Get message parameter(s) size
 * \param[in]       msg         RPC message
 * \return totat parameters size
 */
static inline int16_t bdmfrpc_msg_parm_size(const bdmfrpc_msg_t *msg)
{
    return msg->_parm_size;
}

/** Set message parameters size
 * \param[in]       msg         RPC message
 * \param[in]       size        parameters size
 */
static inline void bdmfrpc_msg_parm_size_set(bdmfrpc_msg_t *msg, int16_t size)
{
    assert(size <= msg->_area_size);
    msg->_parm_size = size;
}

/** Clear message parameters area
 * \param[in]       msg         RPC message
 */
static inline void bdmfrpc_msg_parm_clear(bdmfrpc_msg_t *msg)
{
    msg->_parm_size = 0;
}


/** Encode number
 * \param[in]       msg         RPC message
 * \param[in]       size        field size
 * \param[in]       value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_encode_number(bdmfrpc_msg_t *msg, uint32_t size, bdmfrpc_number value);


/** Encode number
 * \param[in]       msg         RPC message
 * \param[in]       value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
#define bdmfrpc_encode_num(msg, value)            \
    ({  typeof(value) _v = value;               \
        bdmfrpc_encode_number(msg, sizeof(_v), _v); })


/** Decode number
 * \param[in]       msg         RPC message
 * \param[in,out]   offset      field offset. On output points after the field
 * \param[in,out]   size        expected/received size. if !=0 on input - compared against received
 * \param[out]      value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_decode_number(bdmfrpc_msg_t *msg, uint32_t *offset, uint32_t *size, bdmfrpc_number *value);


/** Decode number
 * \param[in]       msg         RPC message
 * \param[in,out]   offset      field offset. On output points after the field
 * \param[out]      value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
#define bdmfrpc_decode_num(msg, offset, value)                    \
        ({ int _rc; bdmfrpc_number _n; uint32_t _sz=0;            \
           _rc = bdmfrpc_decode_number(msg, offset, &_sz, &_n);   \
           *value = (typeof(*value))_n;                         \
           _rc;                                                 \
        })

/** Encode 0-terminated string
 * \param[in]       msg         RPC message
 * \param[in]       value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_encode_string(bdmfrpc_msg_t *msg, const char *value);


/** Decode 0-terminated string
 * \param[in]       msg         RPC message
 * \param[in,out]   offset      field offset. On output points after the field
 * \param[out]      size        received size
 * \param[out]      value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_decode_string(bdmfrpc_msg_t *msg, uint32_t *offset, uint32_t *size, char **value);


/** Encode buffer
 * \param[in]       msg         RPC message
 * \param[in]       size        buffer size
 * \param[in]       value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_encode_buf(bdmfrpc_msg_t *msg, uint32_t size, const void *value);


/** Decode buffer
 * \param[in]       msg         RPC message
 * \param[in,out]   offset      field offset. On output points after the field
 * \param[out]      size        received size
 * \param[out]      value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_decode_buf(bdmfrpc_msg_t *msg, uint32_t *offset, uint32_t *size, void **value);

/** @} end of rpc_msg group */

#endif /* _BL_RPC_MSG_H_ */

