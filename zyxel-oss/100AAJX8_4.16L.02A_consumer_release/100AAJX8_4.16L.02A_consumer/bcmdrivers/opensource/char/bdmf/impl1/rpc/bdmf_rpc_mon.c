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
 * bdmf_rpc_mon.c
 *
 * BDMF framework - RPC service. Shell interface
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
#include <bdmf_platform.h>
#include <bdmf_rpc_engine.h>

/* RPC status
*/
static int bdmfrpc_mon_status(bdmf_session_handle session,
                             const bdmfmon_cmd_parm_t parm[],  uint16_t n_parms)
{
    int i;
    bdmf_session_print(session, "Registered RPC servers:\n");
    for(i=0; i<BDMF_RPC_MODULE__NUMBER_OF; i++)
    {
        const bdmfrpc_server_t *server;
        if (!bdmfrpc_server_get(i, &server))
        {
            bdmf_session_print(session, "%d \t%s\n", i, server->name);
        }
    }
    return 0;
}

#define BDMFRPC_PING_SIZE  64

/* ping subsystem
    BDMFMON_MAKE_PARM_ENUM("subsystem", "Remote subsystem", subsystem_type_enum_table, 0),
    BDMFMON_MAKE_PARM_RANGE_DEFVAL("priority", "priority", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL,
                    BDMFTS_PRTY0, BDMFTS_PRTY3, BDMFTS_PRTY0),
    BDMFMON_MAKE_PARM_DEFVAL("count", "Count", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL, 1),
*/
static int bdmfrpc_mon_ping(bdmf_session_handle session, const bdmfmon_cmd_parm_t parm[],  uint16_t n_parms)
{
    bdmf_subsystem_t target_system = (bdmf_subsystem_t)parm[0].value.number;
    bdmfts_prty_t priority = (bdmfts_prty_t)parm[1].value.number;
    int count = (int)parm[2].value.number;
    int verbose = (int)parm[3].value.number;

    bdmfts_handle bdmfts;
    bdmfts_msg_t *msg;
    bdmfrpc_msg_t *rmsg;
    static int ping_number;
    char sbuf[32];
    int i;
    int rc = 0;

    if (!(bdmfts=bdmfts_session(target_system, priority)))
        return BDMF_ERR_NOT_CONNECTED;
    for (i=0; i<count; i++)
    {
        msg = bdmfrpc_msg_alloc(bdmfts, BDMFRPC_PING_SIZE, NULL);
        if (!msg)
        {
            bdmf_session_print(session, "RPCPING: Message allocation failed\n");
            rc = BDMF_ERR_NOMEM;
            break;
        }
        rmsg = bdmfrpc_msg(msg);
        /* Encode 3 parameters: ping number, ping number as string and verbose */
        bdmfrpc_encode_num(rmsg, ++ping_number);
        snprintf(sbuf, sizeof(sbuf), "Ping number as string: %d", ping_number);
        bdmfrpc_encode_string(rmsg, sbuf);
        bdmfrpc_encode_num(rmsg, verbose);
        rc = bdmfrpc_call(bdmfts, msg);
        if (rc)
            bdmf_session_print(session, "RPCPING: rpc_call() failed. rc=%d - %s\n", rc, bdmf_strerror(rc));
        else
        {
            int pong_number;
            char *pong_string;
            uint32_t s, o=0;
            bdmfrpc_decode_num(rmsg, &o, &pong_number);
            bdmfrpc_decode_string(rmsg, &o, &s, &pong_string);
            if (verbose)
                bdmf_session_print(session, "RPCPONG: rc=%d %d %s\n",
                    bdmfrpc_msg_rc(rmsg), pong_number, pong_string);
        }
        bdmfts_msg_free(bdmfts, msg);
    }
    bdmf_session_print(session, "RPCPING: %d calls made\n", i);
    return rc;
}


/* ping service handler */
static void bdmfrpc_mon_ping_handler(bdmfts_handle bdmfts, void *server_priv, bdmfts_msg_t *msg)
{
    bdmfrpc_msg_t *rmsg=bdmfrpc_msg(msg);
    static int pong_number;
    char sbuf[32];
    int ping_number;
    char *ping_string;
    int verbose;

    uint32_t s, o=0;
    bdmfrpc_decode_num(rmsg, &o, &ping_number);
    bdmfrpc_decode_string(rmsg, &o, &s, &ping_string);
    bdmfrpc_decode_num(rmsg, &o, &verbose);
    if (verbose)
        bdmf_session_print(NULL, "Received RPCPING: %d %s\n", ping_number, ping_string);

    /* Prepare PONG */
    bdmfrpc_msg_parm_size_set(rmsg, 0);
    bdmfrpc_encode_num(rmsg, ++pong_number);
    snprintf(sbuf, sizeof(sbuf), "Pong number as string: %d", pong_number);
    bdmfrpc_encode_string(rmsg, sbuf);
}

/*
 * Register ping service
 */
static int bdmfrpc_mon_ping_register(void)
{
    static bdmfrpc_server_t ping_server = {
                    .name = "ping",
                    .module = BDMF_RPC_MODULE_PING,
                    .handler = bdmfrpc_mon_ping_handler
    };
    int rc;
    rc = bdmfrpc_server_register(&ping_server);
    if (rc)
    {
        bdmf_session_print(NULL, "Failed to register ping RPC server. Error %d - %s\n",
                        rc, bdmf_strerror(rc));
    }
    return rc;
}


/* Create rpc monitor directory in root_dir
   Returns the "rpc" directory handle
*/
bdmfmon_handle_t bdmfrpc_mon_init(bdmfmon_handle_t root_dir)
{
    bdmfmon_handle_t rpc_dir;
    rpc_dir = bdmfmon_dir_add(root_dir, "rpc", "Remote Procedure Call service",
                             BDMF_ACCESS_GUEST, NULL);

    {
        bdmfmon_cmd_add(rpc_dir, "status", bdmfrpc_mon_status,
                      "RPC status report",
                      BDMF_ACCESS_GUEST, NULL, NULL);
    }

    {
        static bdmfmon_cmd_parm_t parms[]={
            BDMFMON_MAKE_PARM_ENUM("subsystem", "Remote subsystem", subsystem_type_enum_table, 0),
            BDMFMON_MAKE_PARM_RANGE_DEFVAL("priority", "priority", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL,
                            BDMFTS_PRTY0, BDMFTS_PRTY_LAST, BDMFTS_PRTY0),
            BDMFMON_MAKE_PARM_DEFVAL("count", "Count", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL, 1),
            BDMFMON_MAKE_PARM_ENUM("verbose", "Verbose", bdmfmon_enum_bool_table, BDMFMON_PARM_FLAG_OPTIONAL),
            BDMFMON_PARM_LIST_TERMINATOR
        };
        bdmfmon_cmd_add(rpc_dir, "ping", bdmfrpc_mon_ping,
                      "Ping subsystem",
                      BDMF_ACCESS_GUEST, NULL, parms);
    }

    /*
     * Register ping service
     */
    bdmfrpc_mon_ping_register();

    return rpc_dir;
}
