#ifndef __FAP4KE_SWQ_H_INCLUDED__
#define __FAP4KE_SWQ_H_INCLUDED__

/*
<:copyright-BRCM:2007:DUAL/GPL:standard

   Copyright (c) 2007 Broadcom Corporation
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
*/
/* SW Queue Data */


#define SWQ_HOST2FAP_GSO_LOOPBACK_BUDGET 32

#define SWQ_HOST2FAP_GSO_LOOPBACK_Q  0
#define SWQ_HOST2FAP_GSO_LOOPBACK_Q_MSG_SIZE 4   /*words*/
#define SWQ_HOST2FAP_GSO_LOOPBACK_Q_DEPTH 1024

#define SWQ_HOST2FAP_GSO_LOOPBACK_Q_MEM_SIZE  \
        (SWQ_HOST2FAP_GSO_LOOPBACK_Q_MSG_SIZE * SWQ_HOST2FAP_GSO_LOOPBACK_Q_DEPTH)
        

#define SWQ_FAP2HOST_GSO_LOOPBACK_Q  1
#define SWQ_FAP2HOST_GSO_LOOPBACK_Q_MSG_SIZE 2   /*words*/
#define SWQ_FAP2HOST_GSO_LOOPBACK_Q_DEPTH 2048

#define SWQ_FAP2HOST_GSO_LOOPBACK_Q_MEM_SIZE   \
        (SWQ_FAP2HOST_GSO_LOOPBACK_Q_MSG_SIZE * SWQ_FAP2HOST_GSO_LOOPBACK_Q_DEPTH)

#define SWQ_FAP2HOST_WFD_BASE_Q 2
#define SWQ_FAP2HOST_WFD_Q_MSG_SIZE 3   /*words*/
#define SWQ_FAP2HOST_WFD_Q_DEPTH 600    /* This number has been chosen to be same as 
                                           number of enet bds as the WLAN packets were previously 
                                           sent via Ethernet driver. This value can be changed 
                                           if the Q depth turns out low */
#define SWQ_FAP2HOST_WFD_Q_MEM_SIZE   \
        (SWQ_FAP2HOST_WFD_Q_MSG_SIZE * SWQ_FAP2HOST_WFD_Q_DEPTH)

#define SWQ_FAP2HOST_WFD_Q0          SWQ_FAP2HOST_WFD_BASE_Q
#define SWQ_FAP2HOST_WFD_Q0_MSG_SIZE SWQ_FAP2HOST_WFD_Q_MSG_SIZE
#define SWQ_FAP2HOST_WFD_Q0_DEPTH    SWQ_FAP2HOST_WFD_Q_DEPTH

#define SWQ_FAP2HOST_WFD_Q1          (SWQ_FAP2HOST_WFD_BASE_Q + 1)
#define SWQ_FAP2HOST_WFD_Q1_MSG_SIZE SWQ_FAP2HOST_WFD_Q_MSG_SIZE
#define SWQ_FAP2HOST_WFD_Q1_DEPTH    SWQ_FAP2HOST_WFD_Q_DEPTH

#define SWQ_MAX  4

#if 0
typedef struct {
  uint32 gsoLoopBackH2FSwqMem[SWQ_HOST2FAP_GSO_LOOPBACK_Q_MEM_SIZE];
  uint32 gsoLoopBackF2HSwqMem[SWQ_FAP2HOST_GSO_LOOPBACK_Q_MEM_SIZE];
} fap4ke_Sdram_Swq_t;
#endif

typedef struct SWQDataMsg_t
{
    uint32    data[4];   
} SWQDataMsg_t;

typedef struct fap4ke_SWQueue_t{
    struct{
        uint8  dqm:6;         /* assoicated DQM for interrupt mgmt */
        uint8  dqmDir:1;
        uint8  fapId:1;
    };
    uint8  msgSize;    /* size of each message */
    uint16 maxDepth;

    uint16 rsvd;
    uint16 count;    /* number of messages in Queue */

    uint32 *qStart;   /* pointer to the start of the Queue in DDR */
    uint32 *qEnd;     /* pointer to the start of the Queue in DDR */

    uint32 *rdPtr;   /* current read pointer */
    uint32 *wrPtr;   /* current read pointer */

    uint32 dropped; /* number of message dropped due to queue full */
    uint32 processed; /* total number of messages passed */
} fap4ke_SWQueue_t;


#ifdef FAP_4KE
extern inline int swqXmitAvailable4ke(fap4ke_SWQueue_t *swq);
extern inline void swqXmitMsg4ke(fap4ke_SWQueue_t *swq, SWQDataMsg_t *msg);
extern inline int swqRecvAvailable4ke(fap4ke_SWQueue_t *swq);
extern void swqRecvMsg4ke(fap4ke_SWQueue_t *swq, SWQDataMsg_t *msg);
extern void inline swqInit4ke(fap4ke_SWQueue_t *swq, uint32 *qmem, uint32 msgSize,
                                uint32 maxDepth, uint32 dqm, uint8 dqmDir);
extern void swqDump4ke(fap4ke_SWQueue_t *swq);
#endif  

#endif  /* defined(__FAP4KE_SWQ_H_INCLUDED__) */
