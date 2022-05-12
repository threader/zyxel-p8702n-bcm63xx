/****************************************************************************
*
*     Copyright (c) 2005 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16251 Laguna Canyon Road
*           Irvine, California  92618
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  voipqosapi.c
*
*  PURPOSE:
*
*     API for ATM RX QoS for voice data - give priority to packets with certain dest. ports
*
*  NOTES:
*
****************************************************************************/

#include <env.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sched.h>
#include "voipqos.h"

// register port
void voipqosRegPrioPort( UINT16 port )
{
    int api_fd;
    struct sockaddr_un addr; 
    char msgbuf[500];
    socklen_t addrlen;
    struct CFMAPI_MSGHEADER *header;
    struct VOIPQOSMSG *msg;
    int size;
    int r;
    int pidhttpd=-1;
    FILE *fp;
    struct sched_param param;

    // printf(".......................to register port %d\n", port);

    /* raise httpd priority so the request can get through */
    fp = fopen("/var/run/httpd_pid", "r");
    if (fp) {
        fscanf(fp, "%d", &pidhttpd);
        fclose(fp);
        param.sched_priority = 5;
        sched_setscheduler(pidhttpd, SCHED_RR, &param);
    }
    
    header = (struct CFMAPI_MSGHEADER *) msgbuf;
    msg = (struct VOIPQOSMSG *) (msgbuf + sizeof(struct CFMAPI_MSGHEADER));
    header->type = CFMAPI_APP_VOICEQOS;
    msg ->action = CFMAPI_VOICEQOS_ADDPORT;
    msg ->port = port;
    size = sizeof(struct CFMAPI_MSGHEADER) + sizeof(struct VOIPQOSMSG);

    api_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/var/cfm_socketapi"); 
    addrlen = sizeof(addr);
    r = sendto(api_fd, msgbuf, size, MSG_DONTWAIT, (struct sockaddr *)&addr, addrlen); 
    if (r != size) {
        printf("voipqosapi: sendto returned %d\n", errno);
    }
    close(api_fd);
}


// unregister port
void voipqosUnregPrioPort( UINT16 port )
{
    int api_fd;
    struct sockaddr_un addr; 
    char msgbuf[500];
    socklen_t addrlen;
    struct CFMAPI_MSGHEADER *header;
    struct VOIPQOSMSG *msg;
    int size;
    int r;
    int pidhttpd=-1;
    FILE *fp;
    struct sched_param param;

    // printf(".......................to UNregister port %d\n", port);

    /* raise httpd priority so the request can get through */
    fp = fopen("/var/run/httpd_pid", "r");
    if (fp) {
        fscanf(fp, "%d", &pidhttpd);
        fclose(fp);
        param.sched_priority = 5;
        sched_setscheduler(pidhttpd, SCHED_RR, &param);
    }

    header = (struct CFMAPI_MSGHEADER *) msgbuf;
    msg = (struct VOIPQOSMSG *) (msgbuf + sizeof(struct CFMAPI_MSGHEADER));
    header->type = CFMAPI_APP_VOICEQOS;
    msg ->action = CFMAPI_VOICEQOS_REMOVEPORT;
    msg ->port = port;
    size = sizeof(struct CFMAPI_MSGHEADER) + sizeof(struct VOIPQOSMSG);

    api_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/var/cfm_socketapi"); 
    addrlen = sizeof(addr);
    r = sendto(api_fd, msgbuf, size, MSG_DONTWAIT, (struct sockaddr *)&addr, addrlen); 
    if (r != size) {
        printf("voipqosapi: sendto returned %d\n", errno);
    }
    close(api_fd);
}



// register wan info
void voipqosRegWanInfo( char *wanifc )
{
    int api_fd;
    struct sockaddr_un addr; 
    char msgbuf[500];
    socklen_t addrlen;
    struct CFMAPI_MSGHEADER *header;
    int size;
    int r;
    int pidhttpd=-1;
    FILE *fp;
    struct sched_param param;

    /* raise httpd priority so the request can get through */
    fp = fopen("/var/run/httpd_pid", "r");
    if (fp) {
        fscanf(fp, "%d", &pidhttpd);
        fclose(fp);
        param.sched_priority = 5;
        sched_setscheduler(pidhttpd, SCHED_RR, &param);
    }

    header = (struct CFMAPI_MSGHEADER *) msgbuf;
    header->type = CFMAPI_APP_VOICEQOS_WANINFO;
    strcpy(msgbuf + sizeof(struct CFMAPI_MSGHEADER), wanifc);
    size = sizeof(struct CFMAPI_MSGHEADER) + strlen(wanifc)+1;

    api_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/var/cfm_socketapi"); 
    addrlen = sizeof(addr);
    r = sendto(api_fd, msgbuf, size, MSG_DONTWAIT, (struct sockaddr *)&addr, addrlen); 
    if (r != size) {
        printf("voipqosapi: sendto returned %d\n", errno);
    }
    close(api_fd);
}

