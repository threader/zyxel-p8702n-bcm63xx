/***************************************************************************
*
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16251 Laguna Canyon Road
*             Irvine, California  92618
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*  
****************************************************************************
*
*    Description:
*  BM.H - Buffer Manager
*
*  PURPOSE:
*     Provides functions for:
*      - initializing a heap control structure
*      - allocating a data buffer
*      - freeing a data buffer
*
****************************************************************************/

#ifndef	BMMGR_H
#define	BMMGR_H

#include <bm.h>                   /* Hausware Buffer Manager */

#define BMLGPSIZE 512   /* in number of words */
#define BMMGRLGBUFSIZE      (BMLGPSIZE*2)/* in number of bytes */
#define BMLGPNUM 40
#define BMSMPSIZE 32    /* number of words */ 
#define BMMGRSMBUFSIZE      (BMSMPSIZE*2)/* number of bytes */ 
#define BMSMPNUM 100

extern   BMHEAP   bmMgrLargePoolHdr;
extern   BMHEAP   bmMgrSmallPoolHdr;

#define BMMGRLGHEAP  &bmMgrLargePoolHdr
#define BMMGRSMHEAP  &bmMgrSmallPoolHdr

void bmMgrInit( void );

#endif /* BMMGR_H */

