/*****************************************************************************
*
*  bm.c - Buffer Manager
*
*  DESCRIPTION:
*    Provides functions for:
*    - initializing a heap control structure
*    - allocating a fixed data buffer
*    - freeing a data buffer
*
*
*  NOTES:
*                    ================================
*                    Initial state of a 3 buffer pool
*                       (no allocated buffers)
*                    ================================
*
* freeTailPtr -----------------------------+
* freeHeadPtr                              |
*   |                                      |
*   |     +-----------+     +-----------+  |  +-----------+  }  BMBUFHEAD
*   |     |  0xDEAD   |     |  0xDEAD   |  |  |  0xDEAD   |   }   debugStartTag
*   |     +-----------+     +-----------+  |  +-----------+   }
*   |     |  BMHEAP * |     |  BMHEAP * |  |  |  BMHEAP * |   }   heapHeader
*   |     +-----------+     +-----------+  |  +-----------+   }
*   +---->| nextPtr * |---->| nextPtr * |--+->|   NULL    |   }   nextPtr
*         +-----------+     +-----------+     +-----------+  }
*         |           |     |           |     |           |
*         |           |     |           |     |           |
*         |  General  |     |  General  |     |  General  |
*         |   Data    |     |   Data    |     |   Data    |
*         ~           ~     ~           ~     ~           ~
*         |           |     |           |     |           |
*         |           |     |           |     |           |
*         +-----------+     +-----------+     +-----------+  }  BMBUFTAIL
*         +  0xDEAD   |     +  0xDEAD   |     +  0xDEAD   |   }   debugEndTag
*         +-----------+     +-----------+     +-----------+  }
*
*
*                    ================================
*                    Initial state of a 3 buffer pool
*                         (1 allocated buffers)
*                    =================================
*
* freeTailPtr -----------------------------+
* freeHeadPtr -----------+                 |
*                        |                 |
*         +-----------+  |  +-----------+  |  +-----------+  }  BMBUFHEAD
*         |  0xA110   |  |  |  0xDEAD   |  |  |  0xDEAD   |   }   debugStartTag
*         +-----------+  |  +-----------+  |  +-----------+   }
*         |  BMHEAP * |  |  |  BMHEAP * |  |  |  BMHEAP * |   }   heapHeader
*         +-----------+  |  +-----------+  |  +-----------+   }
*         | Don't Care|  +->| nextPtr * |--+->|   NULL    |   }   nextPtr
*         +-----------+     +-----------+     +-----------+  }
*         |           |     |           |     |           |
*         |           |     |           |     |           |
*         |  General  |     |  General  |     |  General  |
*         |   Data    |     |   Data    |     |   Data    |
*         ~           ~     ~           ~     ~           ~
*         |           |     |           |     |           |
*         |           |     |           |     |           |
*         +-----------+     +-----------+     +-----------+  }  BMBUFTAIL
*         +  0xA110   |     +  0xDEAD   |     +  0xDEAD   |   }   debugEndTag
*         +-----------+     +-----------+     +-----------+  }
*
*
*            (c) Copyright Broadcom Canada Ltd., 1999
*                        All rights reserved
*
*
****************************************************************************/

/*
********************
** INCLUDES
********************
*/
#include <env.h>        /* Environment basic types */
#include <string.h>     /* ANSI 'C' String routines */
#include <bm.h>       /* Buffer Manager Global Include */
#include <bcmos.h>

/*
** debug tag values for sanity checks
*/
#define BM_HEAP_HEADER_DEBUG_MAGIC_NUM  0xABCD
#define BM_BUF_DEBUG_TAG_SIZE           (sizeof(UINT16))
#define BM_BUF_DEBUG_FREE_TAG           0xDEAD
#define BM_BUF_DEBUG_ALLOC_TAG          0xA110

/*****************************************************************************
*
* bmInitHeap
*
* PURPOSE:
*   Initializes a heap control structure and strings together heap buffers into
*   free pool.
*   The memory pointed to by basePtr is divided up in 'numBuf' buffers.
*   Each buffer has 'sizeBuf' bytes available for general data usage.  The data
*   area is preceeded by a BMBUFHEAD struct and followed BMBUFTAIL struct.
*
* PARAMETERS:
*   heapHeader - Pointer to heap header structure
*   numBuf     - number of buffers in heap
*   sizebuf    - size of buffers (in words) in heap
*   basePtr    - pointer to start of heap
*
* OUTPUTS:
*   Returns    - none
*   heapHeader - the BMHEAP structure pointed to by heapHeader is initialized
*
******************************************************************************/
void bmInitHeap( BMHEAP *heapHeader, UINT16 numBuf, UINT16 sizeBuf, void *basePtr )
{
   UINT16      i;
   BMBUFHEAD   *bufPtr;

#if BM_DEBUG
   void  *dataPtr;
   UINT16 *debugEndTagPtr;

   BCMOS_ASSERT( heapHeader );

   /*
   ** Set heap structure debug magic number to magic value.
   */
   heapHeader->debugMagicNum = BM_HEAP_HEADER_DEBUG_MAGIC_NUM;
#endif

   /*
   ** Set heap header values.
   */
   heapHeader->numBuf            = numBuf;
   heapHeader->sizeData          = sizeBuf;
   heapHeader->numFreeBuf        = numBuf;
   heapHeader->lowWaterMark      = numBuf;
   heapHeader->debugEndTagOffset = sizeBuf +
                                   BM_CTL_FIELDS_OVERHEAD_SIZE -
                                   sizeof(BMBUFTAIL);
   heapHeader->bufStructSize     = sizeBuf + BM_CTL_FIELDS_OVERHEAD_SIZE;

   /*
   ** Initialize QM free pool queue.
   */

   qmInit( &heapHeader->freePoolQ );

   /*
   ** Set pointer to first buffer to start of heap.
   */
   bufPtr = (BMBUFHEAD *)basePtr;

   /*
   ** initialize buffers for entire heap one buffer at a time
   **   string together all data buffers into free pool using QM
   */
   for( i=1; i <= numBuf; i++ )
   {
      /*
      ** Set pointer to point to heap structure
      */
      bufPtr->heapHeader = heapHeader;

#if  BM_DEBUG
      /*
      ** Set buffer debug start tag to "free" state.
      */
      bufPtr->debugStartTag = BM_BUF_DEBUG_FREE_TAG;

      /*
      ** Initialize data memory to all 0's
      */
      dataPtr = (UINT16 *)bufPtr + sizeof( BMBUFHEAD );
      memset( dataPtr, 0x00, (sizeBuf*2));

      /*
      ** set buffer debug end tag to "free" state
      */
      debugEndTagPtr  = (UINT16 *)bufPtr + heapHeader->debugEndTagOffset;
      *debugEndTagPtr = BM_BUF_DEBUG_FREE_TAG;
#endif

      /*
      ** Enqueue buffer into free pool queue.
      ** Pass pointer to DATA area of BM buffer, QM assumes next pointer
      ** resides in memory bytes immediately before start of buffer data.
      */
      qmEnq( &heapHeader->freePoolQ, (UINT16 *)bufPtr + sizeof(BMBUFHEAD) );

      /*
      ** Advance buffer pointer to next buffer start.
      */
      bufPtr = (BMBUFHEAD *)( (UINT16 *)bufPtr + heapHeader->bufStructSize );



   }
}

/*****************************************************************************
*
* bmAlloc
*
* PURPOSE:
*   Allocates a data buffer from a specified heap
*
* PARAMETERS:
*   heapHeader - Pointer to heap header structure
*   size - Requested size of buffer in bytes
*
* RETURNS:
*   Pointer to data buffer
*
* NOTE:
*   Returns NULL if unable to allocate buffer. Size parameter is not
*   required it is only used as a double check.
*
******************************************************************************/
void *bmAlloc( BMHEAP *heapHeader, UINT16 b_size )
{
   void        *dataPtr;
#if BM_DEBUG
   BMBUFHEAD   *bufPtr;
   UINT16      *debugEndTagPtr;
#endif
   UINT16 size = b_size/2;

   /*
   ** Ensure buffer is large enough for requested size
   */
   BCMOS_ASSERT( size <= heapHeader->sizeData );

   /*
   ** Dequeue head free pool buffer
   */
   dataPtr = qmDeq( &heapHeader->freePoolQ );

   /*
   ** If the free pool queue is empty return NULL
   */
   if( dataPtr == NULL )
   {
      return ( NULL );
   }

#if BM_DEBUG
   /*
   ** The pointer returned by qmDeq points to DATA area of BM buffer.
   ** Change it to point to the start (debug start tag) of BM buffer.
   */
   bufPtr = (BMBUFHEAD *)( (UINT16 *)dataPtr - sizeof(BMBUFHEAD) );

   /*
   ** calculate pointer to debug end tag
   */
   debugEndTagPtr = (UINT16 *)bufPtr + heapHeader->debugEndTagOffset;

   /*
   ** assert if debug start and end tags are not both indicating free state
   */
   //printf( "0x%x\n", bufPtr );
   BCMOS_ASSERT( (bufPtr->debugStartTag == BM_BUF_DEBUG_FREE_TAG) &&
          (*debugEndTagPtr == BM_BUF_DEBUG_FREE_TAG) );

   /*
   ** set buffer next free pointer to NULL
   */
   bufPtr->nextPtr = NULL;

   /*
   ** Set debug start and end tags to allocated state
   */
   bufPtr->debugStartTag = BM_BUF_DEBUG_ALLOC_TAG;
   *debugEndTagPtr       = BM_BUF_DEBUG_ALLOC_TAG;

   /*
   ** Update low-water mark.
   */
   heapHeader->numFreeBuf--;
   if( heapHeader->numFreeBuf < heapHeader->lowWaterMark )
   {
      heapHeader->lowWaterMark = heapHeader->numFreeBuf;
   }
#endif

   /*
   ** Return pointer to the buffer data
   */
   return( dataPtr );
}

/*****************************************************************************
*
* bmFree
*
* PURPOSE:
*   Frees a data buffer from a specified heap and returns it to the free pool
*
* PARAMETERS:
*   bufPtr - Pointer to BM buffer
*
* OUTPUTS:
*   Return - nothing
*
******************************************************************************/
void bmFree( void *bufPtr )
{
   BMBUFHEAD    *bufStructPtr;

#if BM_DEBUG
   UINT16   *debugEndTagPtr;

   BCMOS_ASSERT( bufPtr );
#else
   if( bufPtr == NULL )
   {
      return;
   }
#endif

   //printf( "free 0x%x\n", bufPtr );
   /*
   ** bufPtr points to DATA area of BM buffer, we need to go backward to
   ** reference the start of the entire BM buffer structure.
   */
   bufStructPtr = (BMBUFHEAD *)( (UINT16 *)bufPtr - sizeof(BMBUFHEAD) );

#if BM_DEBUG
   /*
   ** Make sure we reference a valid heap header structure
   */
   BCMOS_ASSERT( bufStructPtr->heapHeader->debugMagicNum ==
           BM_HEAP_HEADER_DEBUG_MAGIC_NUM );

   /*
   ** Calculate pointer to debug end tag.
   */
   debugEndTagPtr = (UINT16 *)bufStructPtr +
                     bufStructPtr->heapHeader->debugEndTagOffset;

   /*
   ** Assert if debug start and end tags are not indicating allocated state.
   */
   BCMOS_ASSERT( (bufStructPtr->debugStartTag == BM_BUF_DEBUG_ALLOC_TAG) &&
          (*debugEndTagPtr == BM_BUF_DEBUG_ALLOC_TAG) );

   /*
   ** - Set debug start and end tags to free state
   ** - Update low-water mark info.
   */
   bufStructPtr->debugStartTag = BM_BUF_DEBUG_FREE_TAG;
   *debugEndTagPtr             = BM_BUF_DEBUG_FREE_TAG;
   bufStructPtr->heapHeader->numFreeBuf++;
#endif

   /*
   ** Queue buffer as head buffer in free pool queue.
   */
   qmEnq( &bufStructPtr->heapHeader->freePoolQ, bufPtr );
}


