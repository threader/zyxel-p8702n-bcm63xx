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
*  voipfeatureapi.c
*
*  PURPOSE:
*
*     API for fire wall for voice data - accept packets with certain dest. ports
*
*  NOTES:
*
****************************************************************************/

#include <env.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sched.h>
#include "voipcallfeature.h"

// add port
void voipCallFeatureStore(UINT16 cmd,  int endpt, void *data , int len)
{
    int api_fd;
    struct sockaddr_un addr; 
    char msgbuf[500];
    socklen_t addrlen;
    struct CFMAPI_MSGHEADER *header;
    struct VOIPCFMSG *msg;
    int size;
    int r;
    int pidhttpd=-1;
    FILE *fp;
    struct sched_param param;


    printf("voipCallFeatureStore...to save call feature \n");

    /* raise httpd priority so the request can get through */
    fp = fopen("/var/run/httpd_pid", "r");
    if (fp) {
        fscanf(fp, "%d", &pidhttpd);
        fclose(fp);
        param.sched_priority = 5;
        sched_setscheduler(pidhttpd, SCHED_RR, &param);
    }
    
    /* fill the msg head and content */
    header = (struct CFMAPI_MSGHEADER *) msgbuf;
    msg = (struct VOIPCFMSG *) (msgbuf + sizeof(struct CFMAPI_MSGHEADER));
    header->type = CFMAPI_APP_VOICECALLFEATURE;
    msg ->action = cmd;
    msg->endpt   = endpt;
    msg->dataLen = (len < sizeof(msg->data) )? len : sizeof(msg->data);
    memcpy(msg->data,  data, msg->dataLen);

    printf("voipCallFeatureStore: msg->data = %x, %x, %x, %x\n", msg->data[0], msg->data[1], msg->data[2], msg->data[3]);

    size = sizeof(struct CFMAPI_MSGHEADER) + sizeof(struct VOIPCFMSG);

    api_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/var/cfm_socketapi");
    addrlen = sizeof(addr);

    r = sendto(api_fd, msgbuf, size, MSG_DONTWAIT, (struct sockaddr *)&addr, addrlen);
    if (r != size) {
        printf("voipcallfeatapi: sendto returned %d\n", errno);
    }
    close(api_fd);
}
