/*****************************************************************************
*
*  QM.H - Queue Manager
*
*  PURPOSE:
*    Provides functions for:
*    - initializing singly-linked list based queues
*    - enqueuing data buffers to tail of QM queues
*    - dequeuing data buffers from the head of QM queues
*
*  NOTE:
*
*           (c) Copyright 1999 Broadcom Canada Limited
*                    All rights reserved
*
*****************************************************************************/

#ifndef	QM_H
#define	QM_H

#include <env.h>
#include <bosCritSect.h>

/*
** Structures
*/

typedef struct QMQUEUE
{
	void *headPtr;
	void *tailPtr;
   UINT16 count;
   BOS_CRITSECT critSection;
} QMQUEUE;

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
void qmInit( QMQUEUE *queueHeader );

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
void qmEnq( QMQUEUE *queueHeader, void *bufPtr );

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
void *qmDeq( QMQUEUE *queueHeader );

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
void qmFlush( QMQUEUE *queueHeader );

#endif /* QM_H */
