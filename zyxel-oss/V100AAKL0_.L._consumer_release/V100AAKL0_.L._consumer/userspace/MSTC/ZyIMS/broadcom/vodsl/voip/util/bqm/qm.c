/*****************************************************************************
*  FILENAME: qm.c
*
*  DESCRIPTION:
*  Queue Manager (QM)
*  Provides functions for:
*  - initializing singly-linked list based queues
*  - enqueuing data buffers to tail of QM queues
*  - dequeuing data buffers from the head of QM queues
*
*  NOTE:
*
*             (c) Copyright Broadcom Canada Ltd., 2000
*                        All rights reserved
*****************************************************************************/

/*
************
** INCLUDES
************
*/
#include <env.h>      /* Environment basic types */
#include <log.h>

#include <bm.h>       /* Buffer Manager Global Include */
#include <qm.h>       /* Queue Manager Global Include */

#include <bosCritSect.h>
#include <bosError.h>


/*****************************************************************************
*
*  qmInit
*
*  PURPOSE:
*   Initializes QM queue structure
*
*   PARAMETERS:
*      queueHeader - Pointer to queue header structure
*
*   RETURNS:
*      None
*
*   NOTE:
*
******************************************************************************/
void qmInit( QMQUEUE *queueHeader )
{
   BOS_ASSERT( queueHeader );

   bosCritSectCreate( &queueHeader->critSection );
   /*
   ** Reset the queue pointers
   */
   queueHeader->headPtr = NULL;
   queueHeader->tailPtr = NULL;

   /*
   ** Zero the count of buffers in the queue.
   */
   queueHeader->count = 0;
}

/*****************************************************************************
*
*  qmEnq
*
*  PURPOSE:
*      Adds a buffer to the tail of an exisiting queue
*
*   PARAMETERS:
*      queueHeader - Pointer to queue header structure
*      bufPtr - Pointer to start of data area of buffer
*
*   RETURNS:
*      None
*
*   NOTE:
*      Assumes buffer next pointer is in memory immediately before buffer data
*
******************************************************************************/
void qmEnq( QMQUEUE *queueHeader, void *bufPtr )
{
   BOS_STATUS  status;

   BOS_ASSERT( queueHeader );
   BOS_ASSERT( bufPtr );

   /*
   ** Protect critical section
   */
   status = bosCritSectEnter( &queueHeader->critSection );
   if (status != BOS_STATUS_OK)
   {
      LOG(("ERROR qmEnq: error locking OS"));
      return;
   }

   /*
   ** If   the queue is empty
   ** Then set the head pointer to this new buffer
   ** Else set old tail buffer next pointer to point to new buffer
   */
   if( queueHeader->headPtr == NULL )
   {
      queueHeader->headPtr = bufPtr;
   }
   else
   {
      /* set old tail buffer next pointer to point to new buffer,
      ** assume buffer next pointer occupies memory word immediately
      ** before buffer data
      */
      *( ( void ** )queueHeader->tailPtr - sizeof( void * ) )
         = ( void * )bufPtr;
   }

   /*
   ** Set queue tail pointer to new buffer
   */
   queueHeader->tailPtr = bufPtr;

   /*
   ** set buffer next pointer to NULL, assume next pointer is in memory
   ** immediately before buffer data
   */
   *( ( void ** )bufPtr - sizeof( void * ) ) = NULL;

   /*
   ** Increment the count of number of packets in the queue
   */
   queueHeader->count++;

   /* finished protection */
   status = bosCritSectLeave( &queueHeader->critSection );
   if (status != BOS_STATUS_OK)
   {
      LOG(("ERROR qmEnq: error unlocking OS"));
      return;
   }

} /* qmEnq( ) */

/*****************************************************************************
*
*  qmDeq
*
*  PURPOSE:
*      Dequeues the buffer at the head of the queue
*
*   PARAMETERS:
*      queueHeader - Pointer to queue header structure
*
*   RETURNS:
*      Pointer to data area of dequeued buffer
*
*   NOTE:
*      Returns NULL if queue empty
*
******************************************************************************/
void *qmDeq( QMQUEUE *queueHeader )
{
   void       *bufPtr;
   BOS_STATUS  status;

   BOS_ASSERT( queueHeader );

   /* protect critical section */
   status = bosCritSectEnter( &queueHeader->critSection );
   if ( status != BOS_STATUS_OK )
   {
      LOG(( "ERROR qmDeq: Error locking OS"));
      return NULL;
   }

   /*
   ** If queue empty return NULL
   */
   if ( queueHeader->headPtr == NULL )
   {
      status = bosCritSectLeave( &queueHeader->critSection );
      if ( status != BOS_STATUS_OK )
      {
         LOG(( "ERROR qmDeq: Error unlocking OS"));
      }
      return ( NULL );
   }

   /*
   ** Get pointer to buffer at head of queue
   */
   bufPtr = queueHeader->headPtr;

   /*
   ** If   there is only one buffer in queue,
   ** Then set queue to empty state
   ** Else update queue head pointer
   */
   if( queueHeader->headPtr == queueHeader->tailPtr )
   {
      queueHeader->headPtr = NULL;
      queueHeader->tailPtr = NULL;
   }
   else
   {
      /* update queue head pointer */
      queueHeader->headPtr =  *( ( void ** )bufPtr - sizeof( void * ) );
   }

   BOS_ASSERT(queueHeader->count);

   queueHeader->count--;

   status = bosCritSectLeave( &queueHeader->critSection );
   if ( status != BOS_STATUS_OK )
   {
      LOG(( "ERROR qmDeq: Error unlocking OS"));
   }
   return ( bufPtr );
} /* qmDeq( ) */

/*****************************************************************************
*
*  qmFlush
*
*  PURPOSE:
*      Flushes a queue (frees all buffers)
*
*   PARAMETERS:
*      queueHeader - Pointer to queue header structure
*
*   RETURNS:
*
*   NOTE:
*
******************************************************************************/
void qmFlush( QMQUEUE *queueHeader )
{
   void *bufPtr;

   bufPtr = qmDeq( queueHeader );
   while( bufPtr )
   {
      bmFree( bufPtr );
      bufPtr = qmDeq( queueHeader );
   }
} /* qmFlush( ) */
