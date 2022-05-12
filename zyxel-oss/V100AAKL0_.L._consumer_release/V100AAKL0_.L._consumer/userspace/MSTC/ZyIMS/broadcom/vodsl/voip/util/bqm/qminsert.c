/*****************************************************************************
*
*  qminsert.c
*
*  DESCRIPTION:
*
*
*  NOTE:
*
*          (c) Copyright 1999 Broadcom Canada Limited
*                      All rights reserved
*
*****************************************************************************/

/*
****************************************************************************
** COMPILER PRAGMAS
****************************************************************************
*/

#include <env.h>      /* Hausware Environment basic types */
#include <log.h>

#include <bm.h>       /* Hausware Buffer Manager Global Include */
#include <qm.h>       /* Hausware Queue Manager Global Include */
#include <bcmos.h>

#include <bosCritSect.h>

/*****************************************************************************
*
*  qmInsert
*
*  PURPOSE:
*      Inserts a buffer into an exisiting queue.  The buffer is inserted
*      following the buffer that is specified as the insertion point.
*
*   PARAMETERS:
*      queueHeader - Pointer to queue header structure
*      bufPtr - Pointer to start of data area of buffer to insert
*      insertPtr - Pointer to buffer that inserted buffer will be linked to
*
*   RETURNS:
*      None
*
*   NOTE:
*      Assumes buffer next pointer is in memory immediately before buffer data.
*      Do not use on an empty queue - insertPtr should not be NULL.
*
******************************************************************************/
void qmInsert( QMQUEUE *queueHeader, void *bufPtr, void *insertPtr )
{
   void       *tempPtr;
   BOS_STATUS  status;

   BOS_CRITSECT critSection;

   BCMOS_ASSERT( queueHeader );
   BCMOS_ASSERT( bufPtr );
   BCMOS_ASSERT( insertPtr );
   BCMOS_ASSERT( queueHeader->headPtr );

   /* protect critical section */
   bosCritSectCreate( &critSection );
   status = bosCritSectEnter( &critSection );
   if ( status != BOS_STATUS_OK )
   {
      LOG(("ERROR qmInsert: Error locking OS"));
      return;
   }

   /*
   ** save link from insertion point
   */
   tempPtr = *(( void ** )insertPtr - sizeof( void * ));

   /*
   ** set link at insertion point to the new buffer
   */
   *(( void ** )insertPtr - sizeof( void * )) = bufPtr;

   /*
   ** set the link of the new buffer to that formally used by insertion
   ** point buffer
   */
   *(( void ** )bufPtr - sizeof( void * )) = tempPtr;

   /*
   ** Increment the count of number of packets in the queue
   */
   queueHeader->count++;

   status = bosCritSectLeave( &critSection );
   if ( status != BOS_STATUS_OK )
   {
      LOG(("ERROR qmInsert: Error unlocking OS"));
      return;
   }
   bosCritSectDestroy( &critSection );
} /* qmInsert( ) */
