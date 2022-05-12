/****************************************************************************
*
*  FILENAME:   bmMgr.c
*
*  PURPOSE:
*     This file contains functions that implement a general buffer manager
*     which provides a mechanism for the allocation and control of fixed size
*     buffers.
*
*     Each buffer contains a two field header which is used by the buffer
*     manager.  These fields indicate the current status of the buffer -
*     allocated or free - and the link filed for the free pool or message
*     queue that it is in.
*
*     The first field is set to 0xa110 (allocated) or 0xdead (free) to as
*     necessary.  This allows easy visual debugging to determine the status
*     of the buffers.
*
*     The second field is a pointer to the next buffer in the free pool or
*     the message queue.  Queues and pools are NULL terminated.
*
*     Before buffers can be used the buffer manager must be initialized.
*     Initialization is accomplished by making a call to the routine
*     bmfInit().
*
*  NOTE:
*     Copyright (c) Broadcom Canada Ltd., 1995-2000
*     All Rights Reserved
*
****************************************************************************/

/****************************************************************************
** INCLUDES
****************************************************************************/
#include <env.h>                       /* Environment basic types */
#include <bmmgr.h>                     /* Buffer Manager Application include */


/****************************************************************************
** GLOBAL VARIABLE DECLARATIONS
****************************************************************************/

/*
** Buffer Heap Headers
*/
BMHEAP bmMgrLargePoolHdr;
BMHEAP bmMgrSmallPoolHdr;
/*
** buffer pools - note data pools 32bit aligned
*/
UINT32 bmMgrLgPool[ ((BMLGPSIZE + BM_CTL_FIELDS_OVERHEAD_SIZE) * BMLGPNUM )/2 ];
UINT32 bmMgrSmPool[ ((BMSMPSIZE + BM_CTL_FIELDS_OVERHEAD_SIZE) * BMSMPNUM )/2 ];

/****************************************************************************
*
*  bmMgrInit
*
*  PURPOSE:
*     This function initializes the buffer manager.  The buffer manager
*     controls a number of pools that contain buffers and queue.
*
*     Each buffer pool contains a header that specifies the start address of
*     the pool, number of buffers in the pool, size of each buffer, and a
*     head pointer for the queue.
*
*  PARAMETERS:
*     None
*
*  RETURNS:
*     Nothing
*
*  NOTE:
*
****************************************************************************/
void bmMgrInit( void )
{
   /* initialize the buffer pools */

   bmInitHeap( &bmMgrLargePoolHdr, BMLGPNUM, BMLGPSIZE, bmMgrLgPool );
   bmInitHeap( &bmMgrSmallPoolHdr, BMSMPNUM, BMSMPSIZE, bmMgrSmPool );
}


