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
 * bdmf_rpc_binary.c
 *
 * BDMF framework - RPC service.
 * Binary formatter
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

#include <bdmf_system.h>
#include <bdmf_session.h>
#include <bdmf_rpc_engine.h>

#define BDMFRPC_BINARY_FIELD_MAGIC        'f'
#define BDMFRPC_BINARY_FIELD_PREFIX_SIZE  4
#define BDMFRPC_BINARY_MAX_FIELD_SIZE     256


static inline int bdmfrpc_encode_type_size(bdmfrpc_msg_t *msg, bdmfrpc_field_type type, uint32_t size)
{
    uint32_t o = bdmfrpc_msg_parm_size(msg);
    if (o + size + BDMFRPC_BINARY_FIELD_PREFIX_SIZE > bdmfrpc_msg_area_size(msg))
        return BDMF_ERR_RPC_AREA_FULL;
    msg->_io_area[o++] = BDMFRPC_BINARY_FIELD_MAGIC;
    msg->_io_area[o++] = (uint8_t)type;
    msg->_io_area[o++] = (size >> 8) & 0xff;
    msg->_io_area[o++] = size & 0xff;
    return o;
}


static inline int bdmfrpc_decode_type_size(bdmfrpc_msg_t *msg, uint32_t *offset, bdmfrpc_field_type *type, uint32_t *size)
{
    uint8_t magic, utype;
    uint16_t sz;
    uint32_t o = *offset;

    magic = msg->_io_area[o++];
    utype = msg->_io_area[o++];
    sz =  (uint16_t)msg->_io_area[o++] << 8;
    sz |= msg->_io_area[o++];

    if ((magic != BDMFRPC_BINARY_FIELD_MAGIC) ||
        (utype >= BDMF_RPC_FIELD__NUMBER_OF)  ||
        (o + sz > bdmfrpc_msg_area_size(msg)))
        return BDMF_ERR_RPC_DECODE_ERROR;

    *size = sz;
    *type = (bdmfrpc_field_type)utype;
    *offset = o;

    return 0;
}


/** Encode number
 * \param[in]       msg         RPC message
 * \param[in]       size        field size
 * \param[in]       value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_encode_number(bdmfrpc_msg_t *msg, uint32_t size, bdmfrpc_number value)
{
    int o;
    o = bdmfrpc_encode_type_size(msg, BDMF_RPC_FIELD_NUMBER, size);
    if (o < 0)
        return o;
    switch(size)
    {
    case 4:
        msg->_io_area[o++] = (uint8_t)(value >> 24);
        msg->_io_area[o++] = (uint8_t)(value >> 16);
    case 2:
        msg->_io_area[o++] = (uint8_t)(value >> 8);
    case 1:
        msg->_io_area[o++] = (uint8_t)value;
        break;
    default:
        return BDMF_ERR_RPC_SIZE_ERROR;
    }
    bdmfrpc_msg_parm_size_set(msg, o);
    return 0;
}


/** Decode number
 * \param[in]       msg         RPC message
 * \param[in,out]   offset      field offset. On output points after the field
 * \param[in,out]   size        expected/received size. if !=0 on input - compared against received
 * \param[out]      value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_decode_number(bdmfrpc_msg_t *msg, uint32_t *offset, uint32_t *size, bdmfrpc_number *value)
{
    uint32_t o=*offset;
    uint32_t sz;
    bdmfrpc_field_type type;
    bdmfrpc_number val=0;
    int rc;
    if ((rc=bdmfrpc_decode_type_size(msg, &o, &type, &sz)))
        return rc;
    if (type != BDMF_RPC_FIELD_NUMBER)
        return BDMF_ERR_RPC_DECODE_ERROR;
    if (*size && *size!=sz)
        return BDMF_ERR_RPC_SIZE_ERROR;
    switch(sz)
    {
    case 8:
        val  = msg->_io_area[o++]; val <<= 8;
        val |= msg->_io_area[o++]; val <<= 8;
        val |= msg->_io_area[o++]; val <<= 8;
        val |= msg->_io_area[o++]; val <<= 8;
    case 4:
        val |= msg->_io_area[o++]; val <<= 8;
        val |= msg->_io_area[o++]; val <<= 8;
    case 2:
        val |= msg->_io_area[o++]; val <<= 8;
    case 1:
        val |= msg->_io_area[o++];
        break;
    default:
        return BDMF_ERR_RPC_SIZE_ERROR;
    }
    *offset = o;
    *size = sz;
    *value = val;
    return 0;
}


/** Encode 0-terminated string
 * \param[in]       msg         RPC message
 * \param[in]       value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_encode_string(bdmfrpc_msg_t *msg, const char *value)
{
    uint32_t size=strlen(value) + 1;
    int o;
    o = bdmfrpc_encode_type_size(msg, BDMF_RPC_FIELD_STRING, size);
    if (o < 0)
        return o;
    memcpy(&msg->_io_area[o], value, size);
    bdmfrpc_msg_parm_size_set(msg, o+size);
    return 0;
}


/** Decode 0-terminated string
 * \param[in]       msg         RPC message
 * \param[in,out]   offset      field offset. On output points after the field
 * \param[out]      size        received size.
 * \param[out]      value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_decode_string(bdmfrpc_msg_t *msg, uint32_t *offset, uint32_t *size, char **value)
{
    uint32_t o=*offset;
    uint32_t sz;
    bdmfrpc_field_type type;
    int rc;
    if ((rc=bdmfrpc_decode_type_size(msg, &o, &type, &sz)))
        return rc;
    if (type != BDMF_RPC_FIELD_STRING)
        return BDMF_ERR_RPC_DECODE_ERROR;
    *value = (char *)&msg->_io_area[o];
    *offset = o + sz;
    *size = sz;
    return 0;
}


/** Encode buffer
 * \param[in]       msg         RPC message
 * \param[in]       size        buffer size
 * \param[in]       value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_encode_buf(bdmfrpc_msg_t *msg, uint32_t size, const void *value)
{
    int o;
    o = bdmfrpc_encode_type_size(msg, BDMF_RPC_FIELD_BUFFER, size);
    if (o < 0)
        return o;
    memcpy(&msg->_io_area[o], value, size);
    bdmfrpc_msg_parm_size_set(msg, o+size);
    return 0;
}


/** Decode buffer
 * \param[in]       msg         RPC message
 * \param[in,out]   offset      field offset. On output points after the field
 * \param[out]      size        received size
 * \param[out]      value       field value
 * \return
 *      0   - OK\n
 *     <0   - error
 */
int bdmfrpc_decode_buf(bdmfrpc_msg_t *msg, uint32_t *offset, uint32_t *size, void **value)
{
    uint32_t o=*offset;
    uint32_t sz;
    bdmfrpc_field_type type;
    int rc;
    if ((rc=bdmfrpc_decode_type_size(msg, &o, &type, &sz)))
        return rc;
    if (type != BDMF_RPC_FIELD_BUFFER)
        return BDMF_ERR_RPC_DECODE_ERROR;
    *value = (char *)&msg->_io_area[o];
    *offset = o + sz;
    *size = sz;
    return 0;
}
