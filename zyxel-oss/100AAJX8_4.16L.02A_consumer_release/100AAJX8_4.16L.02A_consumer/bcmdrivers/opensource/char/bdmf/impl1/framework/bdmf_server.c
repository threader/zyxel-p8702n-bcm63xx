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
 * bdmf_server.c
 *
 * Data path builder - object server
 * Allows remote access to the local BDMF objects and types
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

#include <bdmf_dev.h>
#include <bdmf_rpc.h>



/* Server control structure */
static struct bdmf_rpc_server
{
    bdmf_trace_level_t trace_level;
} bdmf_rpc_server;

#define BDMF_RPCS_TRACE_ERR(server, fmt, args...)                   \
    do {                                                            \
        if (server->trace_level >= bdmf_trace_level_error)          \
            bdmf_trace("RPCS ERR: %s#%d: " fmt, __FUNCTION__, __LINE__, ## args);\
    } while(0)

#define BDMF_RPCS_TRACE_INFO(server, fmt, args...)                  \
    do {                                                            \
        if (server->trace_level >= bdmf_trace_level_info)           \
            bdmf_trace("RPCS INF: %s#%d: " fmt, __FUNCTION__, __LINE__, ## args);\
    } while(0)

#ifdef BDMF_DEBUG

#define BDMF_RPCS_TRACE_DBG(server, fmt, args...)                  \
    do {                                                           \
        if (server->trace_level >= bdmf_trace_level_debug)         \
            bdmf_trace("RPCS DBG: %s#%d: " fmt, __FUNCTION__, __LINE__, ## args);\
    } while(0)

#else /* #ifdef BDMF_DEBUG */

#define BDMF_RPCS_TRACE_DBG(fmt, args...)

#endif /* #ifdef BDMF_DEBUG */

#define BDMF_RPCS_TRACE_COND(rc, server, fmt, args...)               \
    do {                                                            \
        if (-rc >= -BDMF_ERR_RMT_NO_RESOURCES)\
            BDMF_RPCS_TRACE_ERR(server, "status:%s " fmt, bdmf_strerror(rc), ## args);\
        else \
            BDMF_RPCS_TRACE_DBG(server, "success. bdmf rc=%s(%d) " fmt, bdmf_strerror(rc), rc, ## args);\
    } while(0)

/* RPC handler declaration */
#define BDMF_RPC_HANDLER(name)  \
    static int name (bdmfts_handle bdmfts, struct bdmf_rpc_server *server, bdmfts_msg_t *msg, bdmfrpc_msg_t *rmsg)

/*
 * RPC function handlers
 */

BDMF_RPC_HANDLER(bdmf_rpcs_type_get_next)
{
    bdmf_remote_handle rh;
    bdmf_type_handle lh;
    bdmf_subsystem_t sender = bdmfts_msg_sender(msg);
    uint32_t offset=0;
    int rc;
    rc = bdmfrpc_decode_num(rmsg, &offset, &rh);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;

    /* Map communication handle to local pointer */
    if (rh)
    {
        lh = bdmf_rpc_handle_to_type(rh); /* also increments usecount. bdmf_type_get_next will decrement it */
        if (!lh)
            return BDMF_ERR_RPC_NO_OBJ;
    }
    else
        lh = NULL;
    lh = bdmf_type_get_next(lh);

    /* Don't report back types created on the requester's subsystem */
    while(lh && lh->subsystem==sender)
        lh = bdmf_type_get_next(lh);

    bdmfrpc_msg_parm_clear(rmsg);
    if (lh)
    {
        rh = lh->comm_handle;
        bdmf_type_put(lh); /* restore usecount incremented by bdmf_type_get_next */
    }
    else
        rh = 0;
    bdmfrpc_encode_num(rmsg, rh);
    return 0;
}

BDMF_RPC_HANDLER(bdmf_rpcs_type_info)
{
    bdmf_remote_handle rh;
    bdmf_type_handle lh;
    bdmf_type_info_t info;
    uint32_t offset=0;
    int rc;

    rc = bdmfrpc_decode_num(rmsg, &offset, &rh);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;

    /* Map communication handle to local pointer */
    lh = bdmf_rpc_handle_to_type(rh);
    if (!lh)
        return BDMF_ERR_RPC_NO_OBJ;

    /* Get type info and encode */
    rc = bdmf_type_info(lh, &info);
    bdmf_type_put(lh); /* restore usecount incremented by bdmf_rpc_handle_to_type */
    if (rc)
        return rc;

    bdmfrpc_msg_parm_clear(rmsg);
    rc = bdmfrpc_encode_num(rmsg, info.n_attrs);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, info.n_objects);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, info.location);
    rc = rc ? rc : bdmfrpc_encode_string(rmsg, info.name);
    rc = rc ? rc : bdmfrpc_encode_string(rmsg, info.help);
    return rc;
}


static int bdmf_rcps_encode_attr_info(struct bdmf_attr *attr, bdmfrpc_msg_t *rmsg)
{
    int rc;

    rc = bdmfrpc_encode_num(rmsg, attr->type);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, attr->flags);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, attr->size);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, attr->array_size);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, attr->offset);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, attr->min_val);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, attr->max_val);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, attr->index_type);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, attr->index_size);
    rc = rc ? rc : bdmfrpc_encode_string(rmsg, attr->name);
    rc = rc ? rc : bdmfrpc_encode_string(rmsg, attr->help ? attr->help : "");
    switch(attr->type)
    {
        case bdmf_attr_enum:
        case bdmf_attr_enum_mask:
        {
            const bdmf_attr_enum_table_t *enum_table = attr->ts.enum_table;
            const bdmf_attr_enum_val_t *enum_val = &enum_table->values[0];
            int nvals = 0;
            /* Count enum values */
            while(enum_val && enum_val->name)
            {
                if ((!attr->min_val && !attr->max_val) ||
                    (enum_val->val >= attr->min_val && enum_val->val >= attr->max_val))
                {
                    ++nvals;
                }
                ++enum_val;
            }
            rc = rc ? rc : bdmfrpc_encode_num(rmsg, nvals);
            rc = rc ? rc : bdmfrpc_encode_string(rmsg, enum_table->help ? enum_table->help : "");
            rc = rc ? rc : bdmfrpc_encode_string(rmsg, enum_table->type_name ? enum_table->type_name : "");
            /* Encode table */
            enum_val = &enum_table->values[0];
            while(!rc && enum_val && enum_val->name)
            {
                if ((!attr->min_val && !attr->max_val) ||
                    (enum_val->val >= attr->min_val && enum_val->val >= attr->max_val))
                {
                    rc = rc ? rc : bdmfrpc_encode_string(rmsg, enum_val->name);
                    rc = rc ? rc : bdmfrpc_encode_num(rmsg, enum_val->val);
                }
                ++enum_val;
            }
            break;
        }

        default:
            rc = rc ? rc : bdmfrpc_encode_string(rmsg, attr->ts.format ? attr->ts.format : "");
    }

    switch(attr->index_type)
    {
        case bdmf_attr_enum:
        case bdmf_attr_enum_mask:
        {
            const bdmf_attr_enum_table_t *enum_table = attr->index_ts.enum_table;
            const bdmf_attr_enum_val_t *enum_val = &enum_table->values[0];
            /* Count enum values */
            while(enum_val && enum_val->name)
                ++enum_val;
            rc = rc ? rc : bdmfrpc_encode_num(rmsg, enum_val - &enum_table->values[0]);
            rc = rc ? rc : bdmfrpc_encode_string(rmsg, enum_table->help);
            rc = rc ? rc : bdmfrpc_encode_string(rmsg, enum_table->type_name);
            /* Encode table */
            enum_val = &enum_table->values[0];
            while(!rc && enum_val && enum_val->name)
            {
                rc = rc ? rc : bdmfrpc_encode_string(rmsg, enum_val->name);
                rc = rc ? rc : bdmfrpc_encode_num(rmsg, enum_val->val);
                ++enum_val;
            }
            break;
        }

        default:
            rc = rc ? rc : bdmfrpc_encode_string(rmsg, attr->index_ts.format ? attr->index_ts.format : "");
    }

    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_attr_info)
{
    bdmf_remote_handle rh;
    bdmf_type_handle lh;
    struct bdmf_attr *attr;
    uint32_t attr_index;
    uint32_t offset=0;
    int rc;

    rc = bdmfrpc_decode_num(rmsg, &offset, &rh);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &attr_index);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;

    /* Map communication handle to local pointer */
    lh = bdmf_rpc_handle_to_type(rh);
    if (!lh)
        return BDMF_ERR_RPC_NO_OBJ;

    if (attr_index >= lh->nattrs)
        return BDMF_ERR_PARM;
    attr = bdmf_aid_to_attr(lh, attr_index);

    bdmfrpc_msg_parm_clear(rmsg);
    rc = bdmf_rcps_encode_attr_info(attr, rmsg);
    bdmf_type_put(lh); /* restore usecount incremented by bdmf_rpc_handle_to_type */

    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_new)
{
    bdmf_remote_handle rdrv, rowner;
    bdmf_type_handle drv;
    bdmf_object_handle owner=NULL;
    struct bdmf_object *mo;
    char *attr;
    uint32_t offset=0;
    uint32_t sz;
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_string } */
    rc = bdmfrpc_decode_num(rmsg, &offset, &rdrv);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &rowner);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &attr);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;
    if (!sz)
        attr = NULL;

    /* Map communication handle to local pointer */
    drv = bdmf_rpc_handle_to_type(rdrv);
    if (!drv)
        return BDMF_ERR_RPC_NO_OBJ;
    if (rowner && !(owner=bdmf_rpc_handle_to_object(rowner)))
        return BDMF_ERR_RPC_NO_OBJ;

    rc = bdmf_new_and_configure(drv, owner, attr, &mo);
    bdmf_type_put(drv);   /* restore usecount incremented by bdmf_rpc_handle_to_type */
    if (owner)
        bdmf_put(owner); /* restore usecount incremented by bdmf_rpc_handle_to_object */
    if (rc)
        return rc;

    /* Encode reply: { object_handle, object_state, object_name } */
    bdmfrpc_msg_parm_clear(rmsg);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, mo->comm_handle);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, mo->state);
    rc = rc ? rc : bdmfrpc_encode_string(rmsg, mo->name);
    if (rc)
        bdmf_destroy(mo);

    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_configure)
{
    bdmf_remote_handle rmo;
    struct bdmf_object *mo;
    char *attr;
    uint32_t offset=0;
    uint32_t sz;
    int rc;

    /* msg: { obj_handle, attribute_string } */
    rc = bdmfrpc_decode_num(rmsg, &offset, &rmo);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &attr);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;
    if (!sz)
        attr = NULL;

    /* Map communication handle to local pointer */
    mo = bdmf_rpc_handle_to_object(rmo);
    if (!mo)
        return BDMF_ERR_RPC_NO_OBJ;

    rc = bdmf_configure(mo, attr);
    bdmf_put(mo); /* restore usecount incremented by bdmf_rpc_handle_to_object */
    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_destroy)
{
    bdmf_remote_handle rmo;
    struct bdmf_object *mo;
    uint32_t offset=0;
    int rc;

    /* msg: { obj_handle } */
    rc = bdmfrpc_decode_num(rmsg, &offset, &rmo);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;

    /* Map communication handle to local pointer */
    mo = bdmf_rpc_handle_to_object(rmo);
    if (!mo)
        return BDMF_ERR_RPC_NO_OBJ;

    rc = bdmf_destroy(mo);
    bdmf_put(mo); /* restore usecount incremented by bdmf_rpc_handle_to_object */
    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_find_get)
{
    bdmf_remote_handle rdrv, rowner;
    bdmf_type_handle drv;
    bdmf_object_handle owner=NULL;
    struct bdmf_object *mo;
    char *attr;
    uint32_t offset=0;
    uint32_t sz;
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_string } */
    rc = bdmfrpc_decode_num(rmsg, &offset, &rdrv);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &rowner);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &attr);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;
    if (!sz)
        attr = NULL;

    /* Map communication handle to local pointer */
    drv = bdmf_rpc_handle_to_type(rdrv);
    if (!drv)
        return BDMF_ERR_RPC_NO_OBJ;
    if (rowner && !(owner=bdmf_rpc_handle_to_object(rowner)))
        return BDMF_ERR_RPC_NO_OBJ;

    rc = bdmf_find_get(drv, owner, attr, &mo);
    bdmf_type_put(drv);  /* restore usecount incremented by bdmf_rpc_handle_to_type */
    if (owner)
        bdmf_put(owner); /* restore usecount incremented by bdmf_rpc_handle_to_object */
    if (rc)
        return rc;

    /* Encode reply: { object_handle, object_state, object_name } */
    bdmfrpc_msg_parm_clear(rmsg);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, mo->comm_handle);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, mo->state);
    rc = rc ? rc : bdmfrpc_encode_string(rmsg, mo->name);
    /* we don't want to hold mo because communication can fail and
     * mo will hang forever */
    bdmf_put(mo);

    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_get_next)
{
    bdmf_remote_handle rdrv, rcur;
    bdmf_type_handle drv;
    struct bdmf_object *mo=NULL;
    char *attr;
    uint32_t offset=0;
    uint32_t sz;
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_string } */
    rc = bdmfrpc_decode_num(rmsg, &offset, &rdrv);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &rcur);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &attr);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;
    if (!sz)
        attr = NULL;

    /* Map communication handle to local pointer */
    drv = bdmf_rpc_handle_to_type(rdrv);
    if (!drv)
        return BDMF_ERR_RPC_NO_OBJ;
    if (rcur)
    {
        if (!(mo=bdmf_rpc_handle_to_object(rcur)))
        {
            bdmf_type_put(drv);   /* restore usecount incremented by bdmf_rpc_handle_to_type */
            return BDMF_ERR_RPC_NO_OBJ;
        }
    }

    mo = bdmf_get_next(drv, mo, attr);
    bdmf_type_put(drv);   /* restore usecount incremented by bdmf_rpc_handle_to_type */

    /* Encode reply: { object_handle, object_state, object_name } */
    bdmfrpc_msg_parm_clear(rmsg);
    if (mo)
    {
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, mo->comm_handle);
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, mo->state);
        rc = rc ? rc : bdmfrpc_encode_string(rmsg, mo->name);
        /* we don't want to hold mo because communication can fail and
         * mo will hang forever */
        bdmf_put(mo);
    }
    else
    {
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, 0);
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, bdmf_state_inactive);
        rc = rc ? rc : bdmfrpc_encode_string(rmsg, "");
    }

    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_get_next_child)
{
    bdmf_remote_handle rown, rcur;
    bdmf_object_handle owner;
    struct bdmf_object *mo=NULL;
    char *type;
    uint32_t offset=0;
    uint32_t sz;
    int rc;

    /* msg: { mo_handle, type, prev_handle } */
    rc = bdmfrpc_decode_num(rmsg, &offset, &rown);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &type);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &rcur);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;
    if (sz <= 1)
        type = NULL;

    /* Map communication handle to local pointer */
    owner = bdmf_rpc_handle_to_object(rown);
    if (!owner)
        return BDMF_ERR_RPC_NO_OBJ;
    if (rcur)
    {
        if (!(mo=bdmf_rpc_handle_to_object(rcur)))
        {
            bdmf_put(owner);   /* restore usecount incremented by bdmf_rpc_handle_to_object */
            return BDMF_ERR_RPC_NO_OBJ;
        }
    }

    mo = bdmf_get_next_child(owner, type, mo);
    bdmf_put(owner);   /* restore usecount incremented by bdmf_rpc_handle_to_object */

    /* Encode reply: { object_handle, object_state, object_name } */
    bdmfrpc_msg_parm_clear(rmsg);
    if (mo)
    {
        rc = rc ? rc : bdmfrpc_encode_string(rmsg, mo->drv->name);
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, mo->comm_handle);
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, mo->state);
        rc = rc ? rc : bdmfrpc_encode_string(rmsg, mo->name);
        /* we don't want to hold mo because communication can fail and
         * mo will hang forever */
        bdmf_put(mo);
    }
    else
    {
        rc = rc ? rc : bdmfrpc_encode_string(rmsg, "");
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, 0);
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, bdmf_state_inactive);
        rc = rc ? rc : bdmfrpc_encode_string(rmsg, "");
    }

    return rc;
}


static int bdmf_rpcs_parse_attr_cb_msg(bdmfrpc_msg_t *rmsg, uint32_t *offset,
                struct bdmf_object **pmo, bdmf_attr_id *paid,
                bdmf_index *pindex)
{
    bdmf_remote_handle rmo;
    struct bdmf_object *mo;
    struct bdmf_attr *attr;
    void *index_buf;
    uint32_t index_buf_sz=0;
    uint16_t aid;
    bdmf_index index;
    int rc = 0;

    *offset=0;
    /* msg: { obj_handle, attr_index, array_index } */
    rc = bdmfrpc_decode_num(rmsg, offset, &rmo);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, offset, &aid);
    if (rc)
        return rc;
    mo = bdmf_rpc_handle_to_object(rmo);
    if (!mo)
        return BDMF_ERR_RPC_NO_OBJ;
    if (aid >= mo->drv->nattrs)
    {
        rc = BDMF_ERR_INTERNAL;
        goto parse_attr_done;
    }
    attr = &mo->drv->aattr[aid];

    /* Numeric indexes are passed by value, other - by address */
    if (bdmf_attr_type_is_numeric(attr->index_type) || !pindex)
        rc = rc ? rc : bdmfrpc_decode_num(rmsg, offset, &index);
    else
    {
        rc = rc ? rc : bdmfrpc_decode_buf(rmsg, offset, &index_buf_sz, &index_buf);
        index = (bdmf_index)index_buf;
    }

    if (!rc)
    {
        *pmo = mo;
        *paid = aid;
        if (pindex)
            *pindex = index;
    }

parse_attr_done:
    bdmf_put(mo);   /* restore usecount incremented by bdmf_rpc_handle_to_object */
    return rc;
}

#define BDMF_RPCS_MAX_ATTR_SIZE     512

BDMF_RPC_HANDLER(bdmf_rpcs_attr_read)
{
    struct bdmf_object *mo;
    bdmf_attr_id aid;
    uint8_t buffer[BDMF_RPCS_MAX_ATTR_SIZE];
    bdmf_index index;
    uint32_t offset;
    uint32_t sz=sizeof(buffer);
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_string } */
    rc = bdmf_rpcs_parse_attr_cb_msg(rmsg, &offset, &mo, &aid, &index);
    rc = rc  ? rc : bdmf_attrelem_get_as_buf(mo, aid, index, buffer, sz);
    /* Encode reply: { attribute_value } */
    bdmfrpc_msg_parm_clear(rmsg);
    rc = rc < 0 ? rc : bdmfrpc_encode_buf(rmsg, sz, buffer);
    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_attr_write)
{
    struct bdmf_object *mo;
    bdmf_attr_id aid;
    void *buf;
    bdmf_index index;
    uint32_t offset;
    uint32_t sz=0;
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_string, attr_value } */
    rc = bdmf_rpcs_parse_attr_cb_msg(rmsg, &offset, &mo, &aid, &index);
    rc = rc ? rc : bdmfrpc_decode_buf(rmsg, &offset, &sz, &buf);
    rc = rc ? rc : bdmf_attrelem_set_as_buf(mo, aid, index, buf, sz);
    /* Encode reply: size */
    bdmfrpc_msg_parm_clear(rmsg);
    bdmfrpc_encode_num(rmsg, rc);
    return rc;
}

static int _bdmf_rpcs_attr_val_to_s_helper(bdmfrpc_msg_t *rmsg, int is_index)
{
    struct bdmf_object *mo;
    bdmf_attr_id aid;
    void *buf;
    char sval[BDMF_RPCS_MAX_ATTR_SIZE]="";
    bdmf_index index;
    uint32_t offset;
    uint32_t sz=0;
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_string } */
    rc = bdmf_rpcs_parse_attr_cb_msg(rmsg, &offset, &mo, &aid, &index);
    rc = rc ? rc : bdmfrpc_decode_buf(rmsg, &offset, &sz, &buf);
    if (!rc)
    {
        struct bdmf_attr *attr = &mo->drv->aattr[aid];
        if (is_index)
            rc = attr->index_to_s(mo, attr, buf, sval, sizeof(sval) - 1);
        else
            rc = attr->val_to_s(mo, attr, buf, sval, sizeof(sval) - 1);
    }
    /* Encode reply: { attribute_value } */
    bdmfrpc_msg_parm_clear(rmsg);
    rc = rc ? rc : bdmfrpc_encode_string(rmsg, sval);
    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_attr_val_to_s)
{
    return _bdmf_rpcs_attr_val_to_s_helper(rmsg, 0);
}

BDMF_RPC_HANDLER(bdmf_rpcs_attr_index_to_s)
{
    return _bdmf_rpcs_attr_val_to_s_helper(rmsg, 1);
}

static int _bdmf_rpcs_attr_s_to_val_helper(bdmfrpc_msg_t *rmsg, int is_index)
{
    struct bdmf_object *mo;
    bdmf_attr_id aid;
    char *sval;
    uint32_t offset;
    uint32_t sz=0;
    uint8_t buffer[BDMF_RPCS_MAX_ATTR_SIZE];
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_string, attr_value } */
    rc = bdmf_rpcs_parse_attr_cb_msg(rmsg, &offset, &mo, &aid, NULL);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &sval);
    if (!rc)
    {
        struct bdmf_attr *attr = &mo->drv->aattr[aid];
        if (attr->size > sizeof(buffer) || attr->index_size > sizeof(buffer))
            rc = BDMF_ERR_RPC_SIZE_ERROR;
        if (is_index)
        {
            sz = attr->index_size;
            rc = rc ? rc : attr->s_to_index(mo, attr, sval, buffer, sz);
        }
        else
        {
            sz = attr->size;
            rc = rc ? rc : attr->s_to_val(mo, attr, sval, buffer, sz);
        }
    }
    /* Encode reply: none */
    bdmfrpc_msg_parm_clear(rmsg);
    rc = rc ? rc : bdmfrpc_encode_buf(rmsg, sz, buffer);
    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_attr_s_to_val)
{
    return _bdmf_rpcs_attr_s_to_val_helper(rmsg, 0);
}

BDMF_RPC_HANDLER(bdmf_rpcs_attr_s_to_index)
{
    return _bdmf_rpcs_attr_s_to_val_helper(rmsg, 1);
}

BDMF_RPC_HANDLER(bdmf_rpcs_attr_find)
{
    struct bdmf_object *mo;
    bdmf_attr_id aid;
    void *buf;
    uint32_t offset;
    uint32_t sz=0;
    struct bdmf_attr *attr = NULL;
    bdmf_index index;
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_value } */
    rc = bdmf_rpcs_parse_attr_cb_msg(rmsg, &offset, &mo, &aid, &index);
    rc = rc ? rc : bdmfrpc_decode_buf(rmsg, &offset, &sz, &buf);
    if (rc)
        return rc;
    attr = &mo->drv->aattr[aid];
    if (!attr->find)
        rc = BDMF_ERR_NOT_SUPPORTED;
    else
        rc = attr->find(mo, attr, &index, buf, sz);
    /* Encode reply: { attribute_index } */
    bdmfrpc_msg_parm_clear(rmsg);
    /* Numeric indexes are passed by value, other - by address */
    if (bdmf_attr_type_is_numeric(attr->index_type))
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, index);
    else
        rc = rc ? rc : bdmfrpc_encode_buf(rmsg, attr->index_size, (void *)index);
    rc = rc ? rc : bdmfrpc_encode_buf(rmsg, sz, buf);
    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_attr_add)
{
    struct bdmf_object *mo;
    bdmf_attr_id aid;
    void *buf;
    uint32_t offset;
    uint32_t sz=0;
    struct bdmf_attr *attr = NULL;
    bdmf_index index;
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_value } */
    rc = bdmf_rpcs_parse_attr_cb_msg(rmsg, &offset, &mo, &aid, &index);
    rc = rc ? rc : bdmfrpc_decode_buf(rmsg, &offset, &sz, &buf);
    if (rc)
        return rc;
    attr = &mo->drv->aattr[aid];
    if (!attr->add)
        rc = BDMF_ERR_NOT_SUPPORTED;
    else
        rc = attr->add(mo, attr, &index, buf, sz);

    /* Encode reply: { attribute_index } */
    bdmfrpc_msg_parm_clear(rmsg);
    /* Numeric indexes are passed by value, other - by address */
    if (bdmf_attr_type_is_numeric(attr->index_type))
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, index);
    else
        rc = rc ? rc : bdmfrpc_encode_buf(rmsg, attr->index_size, (void *)index);
    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_attr_delete)
{
    struct bdmf_object *mo;
    bdmf_attr_id aid;
    uint32_t offset;
    struct bdmf_attr *attr = NULL;
    bdmf_index index;
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_value } */
    rc = bdmf_rpcs_parse_attr_cb_msg(rmsg, &offset, &mo, &aid, &index);
    if (rc)
        return rc;
    attr = &mo->drv->aattr[aid];
    if (!attr->delete)
        rc = BDMF_ERR_NOT_SUPPORTED;
    else
        rc = attr->delete(mo, attr, index);

    /* Encode reply:  */
    bdmfrpc_msg_parm_clear(rmsg);

    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_attr_get_next)
{
    struct bdmf_object *mo;
    bdmf_attr_id aid;
    bdmf_index index;
    uint32_t offset;
    struct bdmf_attr *attr;
    int rc;

    /* msg: { drv_handle, owner_handle, attribute_index } */
    rc = bdmf_rpcs_parse_attr_cb_msg(rmsg, &offset, &mo, &aid, &index);
    rc = rc ? rc : bdmf_attrelem_get_next(mo, aid, &index);
    /* Encode reply: { attribute_index } */
    bdmfrpc_msg_parm_clear(rmsg);
    if (rc < 0)
        return rc;
    attr = &mo->drv->aattr[aid];
    if (bdmf_attr_type_is_numeric(attr->index_type))
        rc = rc ? rc : bdmfrpc_encode_num(rmsg, index);
    else
        rc = rc ? rc : bdmfrpc_encode_buf(rmsg, attr->index_size, (void *)index);
    return rc;
}

static int bdmf_rcps_encode_aggr_info(const struct bdmf_aggr_type *ag, bdmfrpc_msg_t *rmsg)
{
    int rc;
    struct bdmf_attr *a;

    if (!ag)
    {
        rc = bdmfrpc_encode_string(rmsg, "");
        return rc;
    }
    rc = bdmfrpc_encode_string(rmsg, ag->name);
    rc = rc ? rc : bdmfrpc_encode_string(rmsg, ag->help);
    rc = rc ? rc : bdmfrpc_encode_string(rmsg, ag->struct_name);

    a = ag->fields;
    while(a && a->name)
        ++a;

    rc = rc ? rc : bdmfrpc_encode_num(rmsg, a - ag->fields);
    a = ag->fields;
    while(a && a->name && !rc)
    {
        rc = rc ? rc : bdmf_rcps_encode_attr_info(a, rmsg);
        ++a;
    }
    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_aggr_get_next)
{
    char *name;
    uint32_t sz=0;
    struct bdmf_aggr_type *ag = NULL;
    uint32_t offset=0;
    int rc;

    /* msg: { prev_name } */
    rc = bdmfrpc_decode_string(rmsg, &offset, &sz, &name);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;

    /* Find prev */
    if (strlen(name))
    {
        rc = bdmf_attr_aggregate_type_find(name, &ag);
        if (rc)
            return rc;
    }
    ag = (struct bdmf_aggr_type *)bdmf_attr_aggregate_type_get_next(ag);
    bdmfrpc_msg_parm_clear(rmsg);
    rc = bdmf_rcps_encode_aggr_info(ag, rmsg);
    if (ag)
        bdmf_attr_aggr_type_put(ag);
    return rc;
}

BDMF_RPC_HANDLER(bdmf_rpcs_aggr_info)
{
    char *name;
    uint32_t sz=0;
    struct bdmf_aggr_type *ag = NULL;
    uint32_t offset=0;
    int rc;

    /* msg: { name } */
    rc = bdmfrpc_decode_string(rmsg, &offset, &sz, &name);
    if (rc)
        return BDMF_ERR_RPC_MSG_ERROR;

    /* Find prev */
    rc = bdmf_attr_aggregate_type_find(name, &ag);
    if (rc)
        return rc;

    bdmfrpc_msg_parm_clear(rmsg);
    rc = bdmf_rcps_encode_aggr_info(ag, rmsg);
    bdmf_attr_aggr_type_put(ag);
    return rc;
}

/* BDMF RPC service handler */
static void bdmf_rpc_handler(bdmfts_handle bdmfts, void *server_priv, bdmfts_msg_t *msg)
{
    bdmfrpc_msg_t *rmsg = bdmfrpc_msg(msg);
    bdmf_rpc_function_t func = bdmfrpc_msg_function(rmsg);
    struct bdmf_rpc_server *server = (struct bdmf_rpc_server *)server_priv;
    int rc;
    BDMF_RPCS_TRACE_DBG(server, "-->function %d - %s\n", func, bdmf_rpc_function_name(func));
    switch(func)
    {
    case BDMF_RPCF_ATTR_READ:
        rc = bdmf_rpcs_attr_read(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_ATTR_WRITE:
        rc = bdmf_rpcs_attr_write(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_ATTR_VAL_TO_S:
        rc = bdmf_rpcs_attr_val_to_s(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_ATTR_S_TO_VAL:
        rc = bdmf_rpcs_attr_s_to_val(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_ATTR_INDEX_TO_S:
        rc = bdmf_rpcs_attr_index_to_s(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_ATTR_S_TO_INDEX:
        rc = bdmf_rpcs_attr_s_to_index(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_ATTR_FIND:
        rc = bdmf_rpcs_attr_find(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_ATTR_ADD:    /**< bdmf_attrelem_add() */
        rc = bdmf_rpcs_attr_add(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_ATTR_DELETE:    /**< bdmf_attrelem_delete() */
        rc = bdmf_rpcs_attr_delete(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_ATTR_GET_NEXT:
        rc = bdmf_rpcs_attr_get_next(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_CONFIGURE:
        rc = bdmf_rpcs_configure(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_NEW:
        rc = bdmf_rpcs_new(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_FIND_GET:
        rc = bdmf_rpcs_find_get(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_GET_NEXT:
        rc = bdmf_rpcs_get_next(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_GET_NEXT_CHILD:
        rc = bdmf_rpcs_get_next_child(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_DESTROY:
        rc = bdmf_rpcs_destroy(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_TYPE_GET_NEXT:
        rc = bdmf_rpcs_type_get_next(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_TYPE_INFO:
        rc = bdmf_rpcs_type_info(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_ATTR_INFO:
        rc = bdmf_rpcs_attr_info(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_AGGR_GET_NEXT:    /**< bdmf_aggr_get_next() */
        rc = bdmf_rpcs_aggr_get_next(bdmfts, server, msg, rmsg);
        break;
    case BDMF_RPCF_AGGR_ATTR_INFO:   /**< bdmf_aggr_attr_info() */
        rc = bdmf_rpcs_aggr_info(bdmfts, server, msg, rmsg);
        break;
    default:
        rc = BDMF_ERR_RPC_FUNC;
    }
    bdmfrpc_msg_rc_set(rmsg, rc);
    BDMF_RPCS_TRACE_COND(rc, server, "-->function %d\n", func);
}

static bdmfrpc_server_t bdmf_rpc_server_dev = {
    .name = "bdmf",
    .module = BDMF_RPC_MODULE_BDMF,
    .handler = bdmf_rpc_handler,
    .server_priv = &bdmf_rpc_server
};

/*
 * Register RPC service
 */
int bdmf_rpc_server_register(void)
{
    int rc;
    rc = bdmfrpc_server_register(&bdmf_rpc_server_dev);
    if (rc)
    {
        bdmf_session_print(NULL, "Failed to register ping RPC server. Error %d - %s\n",
                        rc, bdmf_strerror(rc));
    }
    bdmf_rpc_server.trace_level = bdmf_global_trace_level;
    return rc;
}
