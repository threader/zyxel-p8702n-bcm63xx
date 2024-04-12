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
 * bdmf_proxy.c
 *
 * Data path builder - object proxy
 * Allows local access to remote BDMF objects and types
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
#include <bdmf_proxy.h>

static struct bdmf_type *bdmf_root[BDMF_SUBSYSTEM__NUMBER_OF];

/*
 * Make RPC parameter area helpers
 */


#define bdmf_make_rpc_msg_1nparm(_subs, _func, _p1, _err)               \
    ({                                                            \
        bdmfts_msg_t *_msg = bdmf_rpc_msg_alloc(_subs, _func);    \
        if (!_msg) return _err;                                   \
        bdmfrpc_encode_num(bdmfrpc_msg(_msg), _p1);               \
        _msg;                                                     \
    })

#define bdmf_make_rpc_msg_1sparm(_subs, _func, _p1, _err)         \
    ({                                                            \
        bdmfts_msg_t *_msg = bdmf_rpc_msg_alloc(_subs, _func);    \
        if (!_msg) return _err;                                   \
        bdmfrpc_encode_string(bdmfrpc_msg(_msg), _p1);            \
        _msg;                                                     \
    })



/*
 * callback planted in proxy object type
 */

/* type->pre_destroy
 */
static void bdmfproxy_pre_destroy_cb( bdmf_object_handle mo)
{
    bdmfts_msg_t *msg;
    bdmf_subsystem_t subs = mo->drv->subsystem;
    if (!mo->comm_handle)
        return;
    msg = bdmf_make_rpc_msg_1nparm(subs, BDMF_RPCF_DESTROY, mo->comm_handle, );
    bdmf_rpc_call(subs, msg);
    bdmf_rpc_msg_free(subs, msg);
    mo->comm_handle = 0;
}


/** Find managed object
 */
static int bdmfproxy_find_get_cb(bdmf_type_handle drv,
                  bdmf_object_handle owner, const char *discr,
                  bdmf_object_handle *pmo)
{
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    bdmf_subsystem_t subs = drv->subsystem;
    uint32_t offset = 0;
    uint32_t sz;
    bdmf_remote_handle rmo;
    bdmf_object_state state;
    char *name;
    int rc;
    /* -->msg: { drv_handle, owner_handle, discriminator } */
    msg = bdmf_make_rpc_msg_1nparm(subs, BDMF_RPCF_FIND_GET, drv->comm_handle, BDMF_ERR_NOMEM);
    rmsg = bdmfrpc_msg(msg);
    rc = bdmfrpc_encode_num(rmsg, owner ? owner->comm_handle : 0);
    rc = bdmfrpc_encode_string(rmsg, discr);
    rc = rc ? rc : bdmf_rpc_call(subs, msg);
    /* <--msg: { mo_handle, mo_state, mo_name } */
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &rmo);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &state);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &name);
    if (!rc)
    {
        struct bdmf_object *mo;

        /* Not found ? */
        if (!rmo)
        {
            *pmo = NULL;
            goto find_get_done;
        }
        /* Allocate temporary object */
        mo = bdmf_object_alloc(drv);
        if (!mo)
        {
            rc = BDMF_ERR_NOMEM;
            goto find_get_done;
        }
        strncpy(mo->name, name, sizeof(mo->name));
        mo->state = state;
        mo->comm_handle = rmo;
        mo->usecount = 0; /* bdmf_find_get will set =1, next "put" will destroy it */
        *pmo = mo;
    }
find_get_done:
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}


/** Managed object iterator.
 * Get next managed object.
 */
static bdmf_object_handle bdmfproxy_get_next_cb(bdmf_type_handle drv,
                                 bdmf_object_handle mo, const char *filter)
{
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    bdmf_subsystem_t subs = drv->subsystem;
    uint32_t offset = 0;
    uint32_t sz;
    bdmf_remote_handle rmo;
    bdmf_object_state state;
    char *name;
    int rc;

    /* -->msg: { drv_handle, cur_handle, filter } */
    msg = bdmf_make_rpc_msg_1nparm(subs, BDMF_RPCF_GET_NEXT, drv->comm_handle, NULL);
    rmsg = bdmfrpc_msg(msg);
    rc = bdmfrpc_encode_num(rmsg, mo ? mo->comm_handle : 0);
    rc = bdmfrpc_encode_string(rmsg, filter ? filter : "");
    rc = rc ? rc : bdmf_rpc_call(subs, msg);
    /* <--msg: { mo_handle, mo_state, mo_name } */
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &rmo);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &state);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &name);
    mo = NULL;
    if (!rc)
    {
        /* Not found ? */
        if (!rmo)
            goto get_next_done;

        /* Allocate temporary object */
        mo = bdmf_object_alloc(drv);
        if (!mo)
            goto get_next_done;

        strncpy(mo->name, name, sizeof(mo->name));
        mo->state = state;
        mo->comm_handle = rmo;
        mo->usecount = 0; /* bdmf_get_next will set =1, next "put" will destroy it */
    }
get_next_done:
    bdmf_rpc_msg_free(subs, msg);
    return mo;
}


/** Child iterator.
 */
bdmf_object_handle bdmfproxy_get_next_child(bdmf_object_handle owner,
    const char *type, bdmf_object_handle mo)
{
    bdmf_subsystem_t subs = owner->drv->subsystem;
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    uint32_t offset = 0;
    uint32_t sz;
    char *drv_name;
    bdmf_remote_handle rmo;
    bdmf_object_state state;
    bdmf_object_handle child = NULL;
    char *name;
    int rc;

    /* -->msg: { obj_handle, type } */
    msg = bdmf_make_rpc_msg_1nparm(subs, BDMF_RPCF_GET_NEXT_CHILD, owner->comm_handle, NULL);
    rmsg = bdmfrpc_msg(msg);
    rc = bdmfrpc_encode_string(rmsg, type ? type : "");
    rc = bdmfrpc_encode_num(rmsg, mo ? mo->comm_handle : 0);
    rc = rc ? rc : bdmf_rpc_call(subs, msg);
    /* <--msg: { drv_name, mo_handle, mo_state, mo_name } */
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &drv_name);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &rmo);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &state);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &name);
    if (!rc && rmo)
    {
        bdmf_type_handle drv;
        /* Find driver */
        rc = bdmf_type_find_get(drv_name, &drv);
        if (rc)
            goto get_next_child_done;
        /* Allocate temporary object */
        child = bdmf_object_alloc(drv);
        if (!child)
            goto get_next_child_done;

        strncpy(child->name, name, sizeof(mo->name));
        child->state = state;
        child->comm_handle = rmo;
        child->usecount = 1;
    }
get_next_child_done:
    bdmf_rpc_msg_free(subs, msg);
    if (mo)
        bdmf_put(mo);
    return child;

}


/*
 * Attribute access callbacks
 */


static int bdmfproxy_make_attr_msg(struct bdmf_object *mo, struct bdmf_attr *ad, bdmf_index index,
    bdmf_rpc_function_t function, bdmfts_msg_t **pmsg)
{
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    bdmf_subsystem_t subs = mo->drv->subsystem;
    uint16_t attr_index = ad - mo->drv->aattr;
    int rc;

    if (attr_index > mo->drv->nattrs)
    {
        BDMF_TRACE_ERR("RPC error. Can't compute attribute index\n");
        return BDMF_ERR_INTERNAL;
    }
    /* -->msg: { obj_handle, attr_index, array_index } */
    msg = bdmf_make_rpc_msg_1nparm(subs, function, mo->comm_handle, BDMF_ERR_NOMEM);
    rmsg = bdmfrpc_msg(msg);
    rc = bdmfrpc_encode_num(rmsg, attr_index);
    rc = rc ? rc : bdmfrpc_encode_num(rmsg, index);  /* array index */
    *pmsg = msg;
    return 0;
}

/** read attribute callback: value is in native format */
static int bdmfproxy_attr_read_cb(struct bdmf_object *mo,
            struct bdmf_attr *ad, bdmf_index index, void *val, uint32_t size)
{
    bdmf_subsystem_t subs = mo->drv->subsystem;
    bdmfts_msg_t *msg=NULL;
    bdmfrpc_msg_t *rmsg;
    void *buf;
    uint32_t offset=0, sz;
    int rc;
    rc = bdmfproxy_make_attr_msg(mo, ad, index, BDMF_RPCF_ATTR_READ, &msg);
    if (rc)
        return rc;
    rmsg = bdmfrpc_msg(msg);
    rc = bdmf_rpc_call(subs, msg);
    rc = rc ? rc : bdmfrpc_decode_buf(rmsg, &offset, &sz, &buf);
    if (!rc)
    {
        if (size > sz)
            size = sz;
        memcpy(val, buf, size);
        rc = size;
    }
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}

/** Optional write callback: value is in native format */
static int bdmfproxy_attr_write_cb(struct bdmf_object *mo,
             struct bdmf_attr *ad, bdmf_index index, const void *val, uint32_t size)
{
    bdmf_subsystem_t subs = mo->drv->subsystem;
    bdmfts_msg_t *msg=NULL;
    bdmfrpc_msg_t *rmsg;
    uint32_t offset=0;
    int rc;

    rc = bdmfproxy_make_attr_msg(mo, ad, index, BDMF_RPCF_ATTR_WRITE, &msg);
    if (rc)
        return rc;
    rmsg = bdmfrpc_msg(msg);
    rc = bdmfrpc_encode_buf(rmsg, size, val);
    rc = rc ? rc : bdmf_rpc_call(subs, msg);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &size);
    if (!rc)
        rc = size;
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}

/* Convert string to value/index */
static int _bdmf_proxy_attr_s_to_val_helper(struct bdmf_object *mo, struct bdmf_attr *ad, const char *sbuf,
    void *val, uint32_t size, bdmf_rpc_function_t func)
{
    bdmf_subsystem_t subs = mo->drv->subsystem;
    bdmfts_msg_t *msg=NULL;
    bdmfrpc_msg_t *rmsg;
    void *buf;
    uint32_t offset=0, sz=0;
    int rc;

    rc = bdmfproxy_make_attr_msg(mo, ad, 0, func, &msg);
    if (rc)
        return rc;
    rmsg = bdmfrpc_msg(msg);
    rc = bdmfrpc_encode_string(rmsg, sbuf);
    rc = rc ? rc : bdmf_rpc_call(subs, msg);
    rc = rc ? rc : bdmfrpc_decode_buf(rmsg, &offset, &sz, &buf);
    if (!rc)
    {
        if (size < sz)
            rc = BDMF_ERR_OVERFLOW;
        else
            memcpy(val, buf, sz);
    }
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}

/* Convert value/index to string */
static int _bdmf_proxy_attr_val_to_s_helper(struct bdmf_object *mo, struct bdmf_attr *ad, const void *val, char *sbuf,
    uint32_t size, uint32_t val_size, bdmf_rpc_function_t func)
{
    bdmf_subsystem_t subs = mo->drv->subsystem;
    bdmfts_msg_t *msg=NULL;
    bdmfrpc_msg_t *rmsg;
    char *buf;
    uint32_t offset=0, sz=0;
    int rc;
    rc = bdmfproxy_make_attr_msg(mo, ad, 0, func, &msg);
    if (rc)
        return rc;
    rmsg = bdmfrpc_msg(msg);
    rc = bdmfrpc_encode_buf(rmsg, val_size, val);
    rc = rc ? rc : bdmf_rpc_call(subs, msg);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &buf);
    if (!rc)
    {
        if (size < sz)
            rc = BDMF_ERR_OVERFLOW;
        else
            strcpy(sbuf, buf);
    }
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}

/** Optional callback that converts value from string to internal format.
 * returns 0 on success or error code <0
 */
static int bdmfproxy_attr_s_to_val_cb(struct bdmf_object *mo, struct bdmf_attr *ad, const char *sbuf, void *val, uint32_t size)
{
    return _bdmf_proxy_attr_s_to_val_helper(mo, ad, sbuf, val, size, BDMF_RPCF_ATTR_S_TO_VAL);
}

/** Optional callback that converts index from string to internal format.
 * returns 0 on success or error code <0
 */
static int bdmfproxy_attr_s_to_index_cb(struct bdmf_object *mo, struct bdmf_attr *ad, const char *sbuf, void *val, uint32_t size)
{
    return _bdmf_proxy_attr_s_to_val_helper(mo, ad, sbuf, val, size, BDMF_RPCF_ATTR_S_TO_INDEX);
}

/** Optional callback that converts value from internal to string format.
 * returns 0 on success or error code <0
 */
static int bdmfproxy_attr_val_to_s_cb(struct bdmf_object *mo, struct bdmf_attr *ad, const void *val, char *sbuf, uint32_t size)
{
    return _bdmf_proxy_attr_val_to_s_helper(mo, ad, val, sbuf, size, ad->size, BDMF_RPCF_ATTR_VAL_TO_S);
}

/** Optional callback that converts index from internal to string format.
 * returns 0 on success or error code <0
 */
static int bdmfproxy_attr_index_to_s_cb(struct bdmf_object *mo, struct bdmf_attr *ad, const void *val, char *sbuf, uint32_t size)
{
    return _bdmf_proxy_attr_val_to_s_helper(mo, ad, val, sbuf, size, ad->index_size, BDMF_RPCF_ATTR_INDEX_TO_S);
}

///* Optional "add" callback */
//static int bdmfproxy_attr_add_cb(struct bdmf_object *mo, struct bdmf_attr *ad, bdmf_index *index, const void *val, uint32_t size)
//{
//    return BDMF_ERR_NOT_SUPPORTED;
//}
//
///** Optional delete callback: takes attr index to be deleted */
//static int bdmfproxy_attr_delete_cb(struct bdmf_object *mo, struct bdmf_attr *ad, bdmf_index index)
//{
//    return BDMF_ERR_NOT_SUPPORTED;
//}

/** Optional get_next callback: next "filled" array index is set in *index.
 * returns 0=OK, BDMF_ERR_NOENT-no more,other error codes
 */
static int bdmfproxy_attr_get_next_cb(struct bdmf_object *mo, struct bdmf_attr *ad, bdmf_index *index)
{
    bdmf_subsystem_t subs = mo->drv->subsystem;
    bdmfts_msg_t *msg=NULL;
    bdmfrpc_msg_t *rmsg;
    void *buf;
    uint32_t offset=0, sz=0;
    int rc;

    rc = bdmfproxy_make_attr_msg(mo, ad, bdmf_attr_type_is_numeric(ad->index_type) ? *index : (bdmf_index)index,
        BDMF_RPCF_ATTR_GET_NEXT, &msg);
    if (rc)
        return rc;
    rc = bdmf_rpc_call(subs, msg);
    rmsg = bdmfrpc_msg(msg);
    if (bdmf_attr_type_is_numeric(ad->index_type))
        rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, index);
    else
    {
        rc = rc ? rc : bdmfrpc_decode_buf(rmsg, &offset, &sz, &buf);
        if (!rc)
        {
            if (sz == ad->index_size)
                memcpy(index, buf, sz);
            else
            {
                BDMF_TRACE_ERR("RPC error. Invalid index size for attribute %s. Expected %d, got %d\n",
                    ad->name, (int)ad->index_size, (int)sz);
                rc = BDMF_ERR_RPC_SIZE_ERROR;
            }
        }
    }
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}

/** Optional find callback: looks up attribute array entry.
 * Updates *index and can update *val as well.
 * returns 0=OK, BDMF_ERR_NOENT-not found,other error codes
 */
static int bdmfproxy_attr_find_cb(struct bdmf_object *mo, struct bdmf_attr *ad, bdmf_index *index, void *val, uint32_t size)
{
    bdmf_subsystem_t subs = mo->drv->subsystem;
    bdmfts_msg_t *msg=NULL;
    bdmfrpc_msg_t *rmsg;
    void *buf;
    uint32_t offset=0, sz=0;
    int rc;

    rc = bdmfproxy_make_attr_msg(mo, ad, bdmf_attr_type_is_numeric(ad->index_type) ? *index : (bdmf_index)index,
        BDMF_RPCF_ATTR_FIND, &msg);
    if (rc)
        return rc;
    rmsg = bdmfrpc_msg(msg);
    rc = bdmfrpc_encode_buf(rmsg, size, val);
    rc = rc ? rc : bdmf_rpc_call(subs, msg);
    if (bdmf_attr_type_is_numeric(ad->index_type))
        rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, index);
    else
    {
        rc = rc ? rc : bdmfrpc_decode_buf(rmsg, &offset, &sz, &buf);
        if (!rc)
        {
            if (sz == ad->index_size)
                memcpy(index, buf, sz);
            else
            {
                BDMF_TRACE_ERR("RPC error. Invalid index size for attribute %s. Expected %d, got %d\n",
                    ad->name, (int)ad->index_size, (int)sz);
                rc = BDMF_ERR_RPC_SIZE_ERROR;
            }
        }
    }
    rc = rc ? rc : bdmfrpc_decode_buf(rmsg, &offset, &sz, &buf);
    if (!rc)
    {
        if (sz == size)
            memcpy(val, buf, sz);
        else
        {
            BDMF_TRACE_ERR("RPC error. Invalid value size for attribute %s. Expected %d, got %d\n",
                ad->name, (int)size, (int)sz);
            rc = BDMF_ERR_RPC_SIZE_ERROR;
        }
    }
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}

/*
 * Remote type management
 */

/** Managed object type iterator.
 * The function returns the first or the next registered object type handle.
 *
 * The handle returned by bdmf_type_get_next function must be
 * released by passing it as a parameter to bdmf_type_get_next or bdmf_type_put()
 *
 * \param[in]   subs        Remote subsystem id
 * \param[in]   remote_drv  previous type handle. NULL=get first
 * \return
 *     next type handle or NULL if end of list is reached.\n
 */
static bdmf_remote_handle bdmfproxy_type_get_next(bdmf_subsystem_t subs,
                bdmf_remote_handle remote_drv)
{
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    uint32_t offset = 0;
    int rc;
    msg = bdmf_make_rpc_msg_1nparm(subs, BDMF_RPCF_TYPE_GET_NEXT, remote_drv, 0);
    rmsg = bdmfrpc_msg(msg);
    rc = bdmf_rpc_call(subs, msg);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &remote_drv);
    bdmf_rpc_msg_free(subs, msg);
    if (rc)
    {
        BDMF_TRACE_ERR("RPC error %d - %s\n", rc, bdmf_strerror(rc));
        return 0;
    }
    return remote_drv;
}


/** Get managed object type info
 *
 * \param[in]   subs        Remote subsystem id
 * \param[in]   remote_drv  Managed object type handle
 * \param[out]  info        Managed object type info
 * \return
 *     0          - OK\n
 *    <0          - error\n
 */
static int bdmfproxy_type_info(bdmf_subsystem_t subs,
                bdmf_remote_handle remote_drv, bdmf_type_info_t *info)
{
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    char *name, *help;
    uint32_t offset = 0;
    uint32_t sz;
    int rc;

    msg = bdmf_make_rpc_msg_1nparm(subs, BDMF_RPCF_TYPE_INFO, remote_drv, BDMF_ERR_NOMEM);
    rmsg = bdmfrpc_msg(msg);
    rc = bdmf_rpc_call(subs, msg);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &info->n_attrs);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &info->n_objects);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &info->location);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &name);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &help);
    if (rc)
    {
        bdmf_rpc_msg_free(subs, msg);
        BDMF_TRACE_ERR("RPC error %d - %s\n", rc, bdmf_strerror(rc));
        return rc;
    }
    strncpy(info->name, name, sizeof(info->name));
    strncpy(info->help, help, sizeof(info->help));
    bdmf_rpc_msg_free(subs, msg);
    return 0;
}


static int _bdmf_decode_attr_type_info(bdmfrpc_msg_t *rmsg, struct bdmf_attr *attr, char *info_area,
    int is_index, uint32_t *offset)
{
    int rc = 0;
    bdmf_attr_type_t type = is_index ? attr->index_type : attr->type;
    uint32_t sz=0;
    char *s;

    switch(type)
    {
        case bdmf_attr_enum:
        case bdmf_attr_enum_mask:
        {
            /* Fetch enum table */
            int num_of_enums;
            rc = bdmfrpc_decode_num(rmsg, offset, &num_of_enums);
            if (!rc && num_of_enums)
            {
                int i;
                bdmf_attr_enum_table_t *enum_table;
                enum_table = bdmf_calloc((num_of_enums+1) * sizeof(bdmf_attr_enum_val_t) +
                    sizeof(bdmf_attr_enum_table_t));
                if (!enum_table)
                    return BDMF_ERR_NOMEM;
                rc = rc ? rc : bdmfrpc_decode_string(rmsg, offset, &sz, &s);
                enum_table->help = attr->name + (s - info_area);
                rc = rc ? rc : bdmfrpc_decode_string(rmsg, offset, &sz, &s);
                enum_table->type_name = attr->name + (s - info_area);
                for(i=0; i<num_of_enums && !rc; i++)
                {
                    rc = rc ? rc : bdmfrpc_decode_string(rmsg, offset, &sz, &s);
                    enum_table->values[i].name = attr->name + (s - info_area);
                    rc = rc ? rc : bdmfrpc_decode_num(rmsg, offset, &enum_table->values[i].val);
                }
                if (is_index)
                    attr->index_ts.enum_table = enum_table;
                else
                    attr->ts.enum_table = enum_table;
            }
            break;
        }

        default:
        {
            /* Fetch format */
            rc = rc ? rc : bdmfrpc_decode_string(rmsg, offset, &sz, &s);
            if (is_index)
                attr->index_ts.format = attr->name + (s - info_area);
            else
                attr->ts.format = attr->name + (s - info_area);
            break;
        }
    }

    return rc;
}


static int _bdmf_proxy_decode_attr(bdmfrpc_msg_t *rmsg, uint32_t *poffset, struct bdmf_attr *attr)
{
    uint32_t area_size;
    uint32_t name_offset;
    uint32_t sz=0;
    char *name;
    char *s;
    int rc;

    rc = bdmfrpc_decode_num(rmsg, poffset, &attr->type);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, poffset, &attr->flags);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, poffset, &attr->size);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, poffset, &attr->array_size);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, poffset, &attr->offset);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, poffset, &attr->min_val);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, poffset, &attr->max_val);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, poffset, &attr->index_type);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, poffset, &attr->index_size);

    /* now comes a tricky part. There is a lot of pointers in attribute structure.
     * Instead of allocating all of them dynamically, we just clone RPC message's
     * parameter area
     */
    /* Name */
    name_offset = *poffset;
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, poffset, &sz, &name);
    if (rc)
        return rc;
    area_size = bdmfrpc_msg_parm_size(rmsg) - name_offset;
    attr->name = (char *)bdmf_calloc(area_size);
    if (!attr->name)
        return BDMF_ERR_NOMEM;
    memcpy((char *)attr->name, name, area_size);

    /* Description */
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, poffset, &sz, &s);
    attr->help = attr->name + (s - name);
    /* Additional type and index_type info */
    rc = rc ? rc : _bdmf_decode_attr_type_info(rmsg, attr, name, 0, poffset);
    rc = rc ? rc : _bdmf_decode_attr_type_info(rmsg, attr, name, 1, poffset);

    return rc;
}

/** Get attribute info by index
 * \param[in]   subs        Remote subsystem id
 * \param[in]   remote_drv  Managed object type handle
 * \param[in]   index       Attribute index
 * \param[out]  attr        Attribute
 * \return
 *     0        - OK\n
 *     BDMF_ERR_NOENT  - no attribute with such index\n
 *     BDMF_ERR_PERM   - no permission\n
 *     BDMF_ERR_PARM  - error in parameters
 */
static int bdmf_remote_attr_by_index(bdmf_subsystem_t subs,
                bdmf_remote_handle remote_drv, uint32_t index, struct bdmf_attr *attr)
{
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    uint32_t offset = 0;
    int rc;

    memset(attr, 0, sizeof(*attr));
    msg = bdmf_make_rpc_msg_1nparm(subs, BDMF_RPCF_ATTR_INFO, remote_drv, BDMF_ERR_NOMEM);
    rmsg = bdmfrpc_msg(msg);
    bdmfrpc_encode_num(rmsg, index);
    rc = bdmf_rpc_call(subs, msg);
    rc = rc ? rc : _bdmf_proxy_decode_attr(rmsg, &offset, attr);
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}

/** Release attribute bits */
static void bdmfproxy_attr_release(struct bdmf_attr *attr)
{
    if ((attr->type==bdmf_attr_enum || attr->type==bdmf_attr_enum_mask) && attr->ts.enum_table)
    {
        bdmf_free((void *)attr->ts.enum_table);
        attr->ts.enum_table = NULL;
    }
    else if ((attr->type==bdmf_attr_aggregate) && attr->aggr_type)
    {
        bdmf_attr_aggr_type_put(attr->aggr_type);
        attr->aggr_type = NULL;
    }
    if ((attr->index_type==bdmf_attr_enum || attr->index_type==bdmf_attr_enum_mask) && attr->index_ts.enum_table)
    {
        bdmf_free((void *)attr->index_ts.enum_table);
        attr->index_ts.enum_table = NULL;
    }
    else if ((attr->index_type==bdmf_attr_aggregate) && attr->index_aggr_type)
    {
        bdmf_attr_aggr_type_put(attr->index_aggr_type);
        attr->index_aggr_type = NULL;
    }
    if (attr->name)
    {
        bdmf_free((char *)attr->name);
        attr->name = NULL;
    }
}

/** Release aggregate type
 * \param[in]   ag
 */
static void bdmfproxy_aggr_release(struct bdmf_aggr_type *ag)
{
    /* Release attributes if any */
    if (ag->fields)
    {
        struct bdmf_attr *attr = &ag->fields[0];
        while(attr->name)
        {
            bdmfproxy_attr_release(attr);
            ++attr;
        }
    }

    /* Release type */
    bdmf_free(ag);
}

/** Release proxy type
 * \param[in]   proxy_type
 */
static void bdmfproxy_type_release(struct bdmf_type *proxy_type)
{
    /* Release attributes if any */
    if (proxy_type->aattr)
    {
        struct bdmf_attr *attr = &proxy_type->aattr[0];
        while(attr->name)
        {
            bdmfproxy_attr_release(attr);
            ++attr;
        }
        bdmf_free(proxy_type->aattr);
    }

    /* Release type */
    bdmf_free(proxy_type);
}


static int bdmfproxy_decode_aggr_info(bdmfrpc_msg_t *rmsg, struct bdmf_aggr_type **ag)
{
    uint32_t offset = 0;
    uint32_t sz;
    struct bdmf_aggr_type *aggr;
    char *name, *help, *struct_name;
    uint32_t n_attrs;
    uint32_t aggr_size;
    int i;
    int rc;

    *ag = NULL;
    rc = bdmfrpc_decode_string(rmsg, &offset, &sz, &name);
    /* End of list is indicated by empty name (sz==1) */
    if (!rc && sz <= 1)
        return BDMF_ERR_NO_MORE;
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &help);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &struct_name);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &n_attrs);
    if (rc)
        return rc;

    /* Allocate aggregate and read attributes */
    aggr_size = sizeof(struct bdmf_aggr_type) +
        strlen(name) + strlen(help) + strlen(struct_name) + 3 +
        (n_attrs + 1) * sizeof(struct bdmf_attr);
    aggr_size = (aggr_size + sizeof(void *) - 1) & ~(sizeof(void *) - 1);
    aggr = bdmf_calloc(aggr_size);
    if (!aggr)
        return BDMF_ERR_NOMEM;

    aggr->name = (char *)aggr + sizeof(struct bdmf_aggr_type);
    strcpy((char *)aggr->name, name);
    aggr->help = aggr->name + strlen(name) + 1;
    strcpy((char *)aggr->help, help);
    aggr->struct_name = aggr->help + strlen(help) + 1;
    strcpy((char *)aggr->struct_name, struct_name);
    aggr->fields = (struct bdmf_attr *)(((long)aggr->struct_name + strlen(struct_name) + sizeof(void *)) &
        ~(sizeof(void *) - 1));

    for(i=0; i<n_attrs && !rc; i++)
        rc = _bdmf_proxy_decode_attr(rmsg, &offset, &aggr->fields[i]);

    if (rc)
    {
        bdmfproxy_aggr_release(aggr);
        return rc;
    }
    *ag = aggr;

    return rc;
}

/** Aggregate type iterator */
static int bdmfproxy_aggr_get_next(bdmf_subsystem_t subs, struct bdmf_aggr_type **ag)
{
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    int rc;

    msg = bdmf_make_rpc_msg_1sparm(subs, BDMF_RPCF_AGGR_GET_NEXT, *ag ? (*ag)->name : "", BDMF_ERR_NOMEM);
    rmsg = bdmfrpc_msg(msg);
    *ag = NULL;
    rc = bdmf_rpc_call(subs, msg);
    rc = rc ? rc : bdmfproxy_decode_aggr_info(rmsg, ag);
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}

/** Connect to BDMF running at remote system
 *  \param[in]  target_system   Remote system id
 * \return
 *     0 - OK\n
 *     error code otherwise
 */
int bdmfproxy_connect(bdmf_subsystem_t target_system)
{
    bdmf_remote_handle remote_type = 0;
    struct bdmf_type *proxy_root = NULL;
    struct bdmf_type *proxy_type = NULL;
    struct bdmf_aggr_type *ag = NULL;
    int rc = 0;

    /* Read list of object types.
     * For each object read description and attributes.
     * Create the appropriate proxy objects
     */
    if (bdmf_root[target_system])
        return BDMF_ERR_ALREADY;

    /* Scan remote aggregates */
    while(!rc && !(rc = bdmfproxy_aggr_get_next(target_system, &ag)))
    {
        ag->subsystem = target_system;
        rc = bdmf_attr_aggregate_type_register(ag);
    }
    if (rc == BDMF_ERR_NO_MORE)
        rc = 0;
    if (rc)
        goto connect_exit;

    /* Scan remote types */
    while((remote_type = bdmfproxy_type_get_next(target_system, remote_type)))
    {
        bdmf_type_info_t info;
        struct bdmf_attr *aattr=NULL;
        proxy_type = NULL;

        /* Read type info */
        rc = bdmfproxy_type_info(target_system, remote_type, &info);
        if (rc)
            break;

        /* Ignore types located on the local system (cross-connection case) */
        if (info.location == bdmf_subsystem())
            continue;

        /* Allocate proxy type */
        proxy_type = bdmf_calloc(sizeof(struct bdmf_type)+strlen(info.name)+strlen(info.help)+2);
        if (!proxy_type)
        {
            BDMF_TRACE_ERR("Can't allocate attributes for proxy type %s\n", info.name);
            rc = BDMF_ERR_NOMEM;
            break;
        }
        proxy_type->subsystem = target_system;
        proxy_type->comm_handle = remote_type;
        proxy_type->name = (char *)proxy_type + sizeof(struct bdmf_type);
        strcpy((char *)proxy_type->name, info.name);
        proxy_type->description = proxy_type->name + strlen(proxy_type->name) + 1;
        strcpy((char *)proxy_type->description, info.help);

        /* Special callbacks */
        proxy_type->pre_destroy = bdmfproxy_pre_destroy_cb;
        proxy_type->get = bdmfproxy_find_get_cb;
        proxy_type->get_next = bdmfproxy_get_next_cb;

        /* Read attribute info */
        if (info.n_attrs)
        {
            int i;
            aattr = bdmf_calloc((info.n_attrs+1) * sizeof(struct bdmf_attr));
            if (!aattr)
            {
                BDMF_TRACE_ERR("Can't allocate attributes for proxy type %s\n", info.name);
                rc = BDMF_ERR_NOMEM;
                break;
            }
            proxy_type->aattr = aattr;
            for(i=0; i<info.n_attrs; i++)
            {
                rc = bdmf_remote_attr_by_index(target_system, remote_type, i, &aattr[i]);
                if (rc)
                    break;

                /* Set access callbacks */
                aattr[i].read = bdmfproxy_attr_read_cb;
                aattr[i].write = bdmfproxy_attr_write_cb;
                aattr[i].val_to_s = bdmfproxy_attr_val_to_s_cb;
                aattr[i].s_to_val = bdmfproxy_attr_s_to_val_cb;
                aattr[i].index_to_s = bdmfproxy_attr_index_to_s_cb;
                aattr[i].s_to_index = bdmfproxy_attr_s_to_index_cb;
                aattr[i].find = bdmfproxy_attr_find_cb;
                aattr[i].get_next = bdmfproxy_attr_get_next_cb;
            }
        }

        /* Register */
        if (!proxy_root)
            proxy_root = proxy_type;
        rc = rc ? rc : bdmf_type_register_ext(proxy_root, proxy_type);
        if (rc)
        {
            BDMF_TRACE_ERR("Can't register proxy type %s\n", info.name);
            break;
        }
    }

connect_exit:
    if (rc)
    {
        if (proxy_type)
            bdmfproxy_type_release(proxy_type);
        if (ag)
            bdmfproxy_aggr_release(ag);
        bdmfproxy_disconnect(target_system);
    }

    return rc;
}


/** Disconnect from BDMF running at remote system
 *  \param[in]  target_system   Remote system id
 * \return
 *     0 - OK\n
 *     error code otherwise
 */
int bdmfproxy_disconnect(bdmf_subsystem_t target_system)
{
    struct bdmf_type *drv = NULL;
    struct bdmf_aggr_type *ag = NULL;

    /* Remove all types */
    do
    {
        struct bdmf_type *next = bdmf_type_get_next(drv);
        if (drv && drv->subsystem == target_system)
            bdmf_type_unregister(drv);
        drv = next;
    } while(drv);

    /* Remove all aggregate types */
    do
    {
        struct bdmf_aggr_type *next = (struct bdmf_aggr_type *)bdmf_attr_aggregate_type_get_next(ag);
        if (ag && ag->subsystem == target_system)
            bdmf_attr_aggregate_type_unregister(ag);
        ag = next;
    } while(ag);

    return 0;
}

/*
 * Remote object management
 */


/** Create a new managed object of the specified type
 *
 * \param[in]   mo      Local managed object handle
 * \param[in]   attr    An ASCII string of attribute values to be set for the new object.\n
 *                      The string is comma-delimited list in format name=value.\n
 *                      See attr format description in bdmf_configure()
 * \return
 *     0      - OK \n
 *    <0      - error
 */
int bdmfproxy_new(struct bdmf_object *mo, const char *attr)
{
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    bdmf_subsystem_t subs = mo->drv->subsystem;
    uint32_t offset = 0;
    uint32_t sz;
    char *name;
    int rc;
    /* -->msg: { drv_handle, owner_handle, attr_string } */
    msg = bdmf_make_rpc_msg_1nparm(subs, BDMF_RPCF_NEW, mo->drv->comm_handle, BDMF_ERR_NOMEM);
    rmsg = bdmfrpc_msg(msg);
    rc = bdmfrpc_encode_num(rmsg, mo->owner ? mo->owner->comm_handle : 0);
    rc = bdmfrpc_encode_string(rmsg, attr);
    rc = rc ? rc : bdmf_rpc_call(subs, msg);
    /* <--msg: { mo_handle, mo_state, mo_name } */
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &mo->comm_handle);
    rc = rc ? rc : bdmfrpc_decode_num(rmsg, &offset, &mo->state);
    rc = rc ? rc : bdmfrpc_decode_string(rmsg, &offset, &sz, &name);
    if (!rc)
        strncpy(mo->name, name, sizeof(mo->name));
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}


/** Set a number of attributes in a single call.
 * see description in bdmf_configure()
 * \param[in]   mo      Managed object handle
 * \param[in]   set     Comma delimited list of name=value pairs
 * \return
 *     0 - OK\n
 *     error code otherwise
 */
int bdmfproxy_configure(bdmf_object_handle mo, const char *set)
{
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    bdmf_subsystem_t subs = mo->drv->subsystem;
    int rc;
    /* -->msg: { obj_handle, attr_string } */
    msg = bdmf_make_rpc_msg_1nparm(subs, BDMF_RPCF_CONFIGURE, mo->comm_handle, BDMF_ERR_NOMEM);
    rmsg = bdmfrpc_msg(msg);
    rc = bdmfrpc_encode_string(rmsg, set);
    rc = rc ? rc: bdmf_rpc_call(subs, msg);
    /* <--msg: rc */
    bdmf_rpc_msg_free(subs, msg);
    return rc;
}
