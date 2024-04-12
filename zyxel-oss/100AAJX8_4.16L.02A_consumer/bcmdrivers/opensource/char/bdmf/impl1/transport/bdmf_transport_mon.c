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
 * bdmf_transport_mon.c
 *
 * BDMF framework - Transport service. Shell interface
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
#include <bdmf_shell.h>
#include <bdmf_transport.h>

static bdmfmon_enum_val_t connection_type_enum_table[] = {
    { .name="domain_socket", .val=BDMFTS_CONN_DOMAIN_SOCKET},
    { .name="shm", .val=BDMFTS_CONN_SHM},
    { .name="proxy", .val=BDMFTS_CONN_PROXY},
    BDMFMON_ENUM_LAST
};

static bdmfmon_enum_val_t connection_role_enum_table[] = {
    { .name="connect", .val=1},
    { .name="listen",  .val=0},
    BDMFMON_ENUM_LAST
};


static int bdmfts_mon_ping_sent, bdmfts_mon_ping_received, bdmfts_mon_ping_errors;

/* View/Set *this* subsystem
    BDMFMON_MAKE_PARM_ENUM("subsystem", "This subsystem", subsystem_type_table, 0),
*/
static int bdmfts_mon_subsystem(bdmf_session_handle session,
                               const bdmfmon_cmd_parm_t parm[],  uint16_t n_parms)
{
    bdmf_subsystem_t subs;
    int rc;
    if (bdmfmon_parm_is_set(session, 0))
    {
        subs = (bdmf_subsystem_t)parm[0].value.number;
        rc = bdmf_subsystem_set(subs);
    }
    else
    {
        subs = bdmf_subsystem();
        if (subs > BDMF_SUBSYSTEM_LOCAL)
            bdmf_session_print(session, "Local subsystem is %s\n", bdmf_subsystem_name(subs));
        else
            bdmf_session_print(session, "Local subsystem is not set\n");
        rc = 0;
    }
    return rc;
}

/* Connect to subsystem
    BDMFMON_MAKE_PARM_ENUM("subsystem", "Remote subsystem", subsystem_type_enum_table, 0),
    BDMFMON_MAKE_PARM_ENUM("connection_type", "Connection type", connection_type_enum_table, 0),
    BDMFMON_MAKE_PARM_ENUM("role", "Connect/listen", connection_role_enum_table, 0),
    BDMFMON_MAKE_PARM_RANGE_DEFVAL("priority", "priority", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL,
                    BDMFTS_PRTY0, BDMFTS_PRTY_LAST, BDMFTS_PRTY0),
    BDMFMON_MAKE_PARM("address", "address", BDMFMON_PARM_STRING, BDMFMON_PARM_FLAG_OPTIONAL),
*/
static int bdmfts_mon_connect(bdmf_session_handle session,
                               const bdmfmon_cmd_parm_t parm[],  uint16_t n_parms)
{
    bdmf_subsystem_t target_system = (bdmf_subsystem_t)parm[0].value.number;
    bdmfts_conn_type_t conn_type = (bdmfts_conn_type_t)parm[1].value.number;
    int role = (int)parm[2].value.number;
    bdmfts_prty_t priority = (bdmfts_prty_t)parm[3].value.number;
    char *address = (char *)parm[4].value.string;
    bdmfts_session_parm_t session_parm;
    bdmfts_handle bdmfts;
    int rc;

    memset(&session_parm, 0, sizeof(session_parm));
    session_parm.subsystem = target_system;
    session_parm.priority = priority;
    session_parm.conn_type = conn_type;
    session_parm.data = address;
    if (role)
        rc = bdmfts_connect(&session_parm, &bdmfts);
    else
        rc = bdmfts_listen(&session_parm, &bdmfts);
    return rc;
}

/* Disconnect from subsystem
    BDMFMON_MAKE_PARM_ENUM("subsystem", "Remote subsystem", subsystem_type_enum_table, 0),
    BDMFMON_MAKE_PARM_RANGE_DEFVAL("priority", "priority", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL,
                    BDMFTS_PRTY0, BDMFTS_PRTY_LAST, BDMFTS_PRTY0),
*/
static int bdmfts_mon_disconnect(bdmf_session_handle session,
                               const bdmfmon_cmd_parm_t parm[],  uint16_t n_parms)
{
    bdmf_subsystem_t target_system = (bdmf_subsystem_t)parm[0].value.number;
    bdmfts_prty_t priority = (bdmfts_prty_t)parm[1].value.number;
    bdmfts_handle bdmfts;
    if (!(bdmfts=bdmfts_session(target_system, priority)))
        return BDMF_ERR_NOT_CONNECTED;
    bdmfts_disconnect(bdmfts);
    bdmf_session_print(session, "Disconnected from %s:%d",
                    bdmf_subsystem_name(target_system), priority);
    return 0;
}

/* RPC status
*/
static int bdmfts_mon_status(bdmf_session_handle session,
                               const bdmfmon_cmd_parm_t parm[],  uint16_t n_parms)
{
    int i;
    bdmf_session_print(session, "This subsystem is: %s\n", bdmf_subsystem_name(bdmf_subsystem()));
    bdmf_session_print(session, "\nRegistered transport message servers:\n");
    for(i=0; i<BDMFTS_MSGTYPE__NUMBER_OF; i++)
    {
        const bdmfts_server_parm_t *server = bdmfts_server_get(i);
        if (server)
            bdmf_session_print(session, "%d \t%s\n", i, server->name);
    }
    bdmf_session_print(session, "\nCommunication sessions:\n");
    for(i=0; i<BDMF_SUBSYSTEM__NUMBER_OF; i++)
    {
        int j;
        for(j=0; j<=BDMFTS_PRTY_LAST; j++)
        {
            bdmfts_handle bdmfts;
            if ((bdmfts=bdmfts_session(i, j)))
            {
                const bdmfts_session_parm_t *sparm=bdmfts_session_parms(bdmfts);
                bdmf_session_print(session, "%s:%d \t%s\n",
                    bdmf_subsystem_name(i), j,
                    bdmfmon_enum_stringval(connection_type_enum_table, sparm->conn_type));
            }
        }
    }
    bdmf_session_print(session, "Last ping status: sent:%d  received:%d  errors:%d\n",
                        bdmfts_mon_ping_sent, bdmfts_mon_ping_received, bdmfts_mon_ping_errors);
    return 0;
}

#define BDMFTS_PING_SIZE          64
#define BDMFTS_PING_TIMEOUT_US    10000

/* ping subsystem
    BDMFMON_MAKE_PARM_ENUM("subsystem", "Remote subsystem", subsystem_type_enum_table, 0),
    BDMFMON_MAKE_PARM_RANGE_DEFVAL("priority", "priority", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL,
                    BDMFTS_PRTY0, BDMFTS_PRTY_LAST, BDMFTS_PRTY0),
    BDMFMON_MAKE_PARM_DEFVAL("count", "Count", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL, 1),
*/
static int bdmfts_mon_ping(bdmf_session_handle session, const bdmfmon_cmd_parm_t parm[],  uint16_t n_parms)
{
    bdmf_subsystem_t target_system = (bdmf_subsystem_t)parm[0].value.number;
    bdmfts_prty_t priority = (bdmfts_prty_t)parm[1].value.number;
    bdmfmon_number count=parm[2].value.number;
    int verbose=(int)parm[3].value.number;
    int fd=(int)parm[4].value.number;

    bdmfts_handle bdmfts;
    bdmfts_msg_t *msg;
    int i;
    int rc;
    char c;
    int n;
    int d;

    if (!(bdmfts=bdmfts_session(target_system, priority)))
        return BDMF_ERR_NOT_CONNECTED;

    bdmfts_mon_ping_sent = bdmfts_mon_ping_received = bdmfts_mon_ping_errors = 0;
    for(n=0; n<count; n++)
    {
        char *payload;
        msg = bdmfts_msg_alloc(bdmfts, BDMFTS_PING_SIZE, NULL);
        if (!msg)
        {
            bdmf_session_print(session, "PING: Message allocation failed\n");
            rc = BDMF_ERR_NOMEM;
            break;
        }
        payload = bdmfts_msg_data(msg);
        bdmfts_msg_type_set(msg, BDMFTS_MSGTYPE_PING);
        payload[0] = verbose;
        c = '0';
        for(i=1; i<BDMFTS_PING_SIZE; i++)
        {
            payload[i] = c++;
            if (c > 'z')
                c = '0';
        }
        bdmfts_msg_data_length_set(msg, BDMFTS_PING_SIZE);
        if (fd)
            bdmfts_msg_full_duplex_set(msg);

        /* Send message */
        if (verbose)
            bdmf_session_hexdump(session, payload, 0, BDMFTS_PING_SIZE);
        rc = bdmfts_msg_send(bdmfts, msg);
        if (rc)
        {
            bdmf_session_print(session, "PING%d: failed with rc=%d - %s\n",
                    n, rc, bdmf_strerror(rc));
            break;
        }
        else
        {
            if (verbose)
                bdmf_session_print(session, "PING%d: sent %d bytes\n", n, BDMFTS_PING_SIZE);
            ++bdmfts_mon_ping_sent;
        }
        if (fd)
        {
            for(d=0; d<BDMFTS_PING_TIMEOUT_US/10 && (bdmfts_mon_ping_sent != bdmfts_mon_ping_received); d++)
                usleep(10);
        }
    }
    bdmf_session_print(session, "PING: %d packets sent, %d packets received\n",
                        bdmfts_mon_ping_sent, bdmfts_mon_ping_received);

    return rc;
}


/* ping service handler */
static void bdmfts_mon_ping_handler(bdmfts_handle bdmfts, void *server_priv, bdmfts_msg_t *msg)
{
    const bdmfts_session_parm_t *sparm=bdmfts_session_parms(bdmfts);
    int payload_length = bdmfts_msg_data_length(msg);
    char *payload;
    int reply;
    int verbose;
    int fd;

    if (msg->rc)
    {
        ++bdmfts_mon_ping_errors;
        bdmfts_msg_free(bdmfts, msg);
        return;
    }

    reply = bdmfts_msg_is_reply(msg);
    fd = bdmfts_msg_is_full_duplex(msg);
    payload = bdmfts_msg_data(msg);
    verbose = payload[0];
    if (verbose)
    {
        bdmf_session_print(NULL, "%s: received %d bytes from %s:%d. %s\n",
                        reply ? "PONG" : "PING",
                        payload_length, bdmf_subsystem_name(sparm->subsystem),
                        sparm->priority, fd ? "FD" : "One way");
        bdmf_session_hexdump(NULL, payload, 0, payload_length);
    }
    ++bdmfts_mon_ping_received;
    if (reply || !fd)
        bdmfts_msg_free(bdmfts, msg);
    else
    {
        snprintf(payload+1, payload_length-1, "PONG: received %d bytes", payload_length);
        bdmfts_msg_reply(bdmfts, msg);
    }
}

/*
 * Register ping service
 */
static int bdmfts_mon_ping_register(void)
{
    static bdmfts_server_parm_t ping_server = {
        .msg_type = BDMFTS_MSGTYPE_PING,             /**< Message type - used for message de-multiplexing */
        .name = "ping",
        .receive = bdmfts_mon_ping_handler
    };
    int rc;
    rc = bdmfts_server_register(&ping_server);
    if (rc)
    {
        bdmf_session_print(NULL, "Failed to register ping server with transport service. Error %d - %s\n",
                        rc, bdmf_strerror(rc));
    }
    return rc;
}


/* Create transport monitor directory in root_dir
   Returns the "transport" directory handle
*/
bdmfmon_handle_t bdmfts_mon_init(bdmfmon_handle_t root_dir)
{
    bdmfmon_handle_t bdmfts_dir;
    bdmfts_dir = bdmfmon_dir_add(root_dir, "transport", "Transport service",
                             BDMF_ACCESS_GUEST, NULL);

    {
        static bdmfmon_cmd_parm_t parms[]={
            BDMFMON_MAKE_PARM_ENUM("subsystem", "This subsystem", subsystem_type_enum_table, BDMFMON_PARM_FLAG_OPTIONAL),
            BDMFMON_PARM_LIST_TERMINATOR
        };
        bdmfmon_cmd_add(bdmfts_dir, "this", bdmfts_mon_subsystem,
                      "View/Set *this* subsystem",
                      BDMF_ACCESS_ADMIN, NULL, parms);
    }

    {
        static bdmfmon_cmd_parm_t parms[]={
            BDMFMON_MAKE_PARM_ENUM("subsystem", "Remote subsystem", subsystem_type_enum_table, 0),
            BDMFMON_MAKE_PARM_ENUM("connection_type", "Connection type", connection_type_enum_table, 0),
            BDMFMON_MAKE_PARM_ENUM("role", "Connect/listen", connection_role_enum_table, 0),
            BDMFMON_MAKE_PARM_RANGE_DEFVAL("priority", "priority", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL,
                            BDMFTS_PRTY0, BDMFTS_PRTY_LAST, BDMFTS_PRTY0),
            BDMFMON_MAKE_PARM("address", "address", BDMFMON_PARM_STRING, BDMFMON_PARM_FLAG_OPTIONAL),
            BDMFMON_PARM_LIST_TERMINATOR
        };
        bdmfmon_cmd_add(bdmfts_dir, "connect", bdmfts_mon_connect,
                      "Connect to subsystem",
                      BDMF_ACCESS_ADMIN, NULL, parms);
    }

    {
        static bdmfmon_cmd_parm_t parms[]={
            BDMFMON_MAKE_PARM_ENUM("subsystem", "Remote subsystem", subsystem_type_enum_table, 0),
            BDMFMON_MAKE_PARM_RANGE_DEFVAL("priority", "priority", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL,
                            BDMFTS_PRTY0, BDMFTS_PRTY_LAST, BDMFTS_PRTY0),
            BDMFMON_PARM_LIST_TERMINATOR
        };
        bdmfmon_cmd_add(bdmfts_dir, "disconnect", bdmfts_mon_disconnect,
                      "Disconnect from subsystem",
                      BDMF_ACCESS_ADMIN, NULL, parms);
    }

    {
        bdmfmon_cmd_add(bdmfts_dir, "status", bdmfts_mon_status,
                      "Transport service status report",
                      BDMF_ACCESS_GUEST, NULL, NULL);
    }

    {
        static bdmfmon_cmd_parm_t parms[]={
            BDMFMON_MAKE_PARM_ENUM("subsystem", "Remote subsystem", subsystem_type_enum_table, 0),
            BDMFMON_MAKE_PARM_RANGE_DEFVAL("priority", "priority", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL,
                            BDMFTS_PRTY0, BDMFTS_PRTY_LAST, BDMFTS_PRTY0),
            BDMFMON_MAKE_PARM_DEFVAL("count", "Count", BDMFMON_PARM_NUMBER, BDMFMON_PARM_FLAG_OPTIONAL, 1),
            BDMFMON_MAKE_PARM_ENUM("verbose", "Verbose", bdmfmon_enum_bool_table, BDMFMON_PARM_FLAG_OPTIONAL),
            BDMFMON_MAKE_PARM_ENUM("fd", "Full duplex (expect reply)", bdmfmon_enum_bool_table, BDMFMON_PARM_FLAG_OPTIONAL),
            BDMFMON_PARM_LIST_TERMINATOR
        };
        bdmfmon_cmd_add(bdmfts_dir, "ping", bdmfts_mon_ping,
                      "Ping subsystem",
                      BDMF_ACCESS_GUEST, NULL, parms);
    }

    /*
     * Register ping service
     */
    bdmfts_mon_ping_register();

    return bdmfts_dir;
}
