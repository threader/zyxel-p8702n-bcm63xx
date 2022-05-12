/*****************************************************************************
*
*  BM.H - Buffer Manager
*
*  PURPOSE:
*     Provides functions for:
*      - initializing a heap control structure
*      - allocating a data buffer
*      - freeing a data buffer
*
*  
*  NOTE:
*
*               (c) Copyright 1999 Broadcom Canada Ltd.,
*                      All rights reserved
*
*****************************************************************************/

#ifndef	BM_H
#define	BM_H

#include <env.h>
#include <qm.h>                   /* Hausware Queue Manager */

#define BM_DEBUG  1


/* 
** STRUCTURE:    BMHEAP
** DESCRIPTION:  Buffer Manager Heap header structure 
**
** debugMagicNum     - This field is initialize to a 'magic' number.  If this
**                     field is ever not what we expect, we will know that 
**                     something bad has hppend
** numBuf            - Total number of buffers in this pool.
** sizeData          - Size (in words) of the general data area.
** numFreeBuf        - Number of free buffer.
** lowWaterMark      - Min number of free buffers since heap initialization.
** debugEndTagOffset - Offset to debug end buffer tag, pre-calculate 
**                     to speed BM operations.
** bufStructSize     - Total size of buffer data and control fields.
** freePoolQ         - Queue which hold the free buffer for this pool.
*/
typedef struct 
{
#if BM_DEBUG	            
	UINT16  debugMagicNum;
#endif
	UINT16  numBuf;                                                            
	UINT16  sizeData;                                                          
	UINT16  numFreeBuf;
	UINT16  lowWaterMark;
#if BM_DEBUG	            
	UINT16  debugEndTagOffset;
#endif
   UINT16   bufStructSize;
	QMQUEUE freePoolQ;

} BMHEAP;

/*
** Buffer header structer
*/
typedef struct 
{       
#if  BM_DEBUG
	UINT16  debugStartTag;
#endif 

	BMHEAP  *heapHeader;
	UINT16  *nextPtr;

} BMBUFHEAD;

/*
** Buffer header structer
*/
#if  BM_DEBUG
typedef struct 
{       
	UINT16  debugEndTag;

} BMBUFTAIL;
#endif

/******************
** More CONSTANTS 
******************/

/* 
**  Size (in words) of control fields in BM buffers. 
*/
#if  BM_DEBUG
#define BM_CTL_FIELDS_OVERHEAD_SIZE	  sizeof(BMBUFHEAD) + sizeof(BMBUFTAIL)
#else
#define BM_CTL_FIELDS_OVERHEAD_SIZE	  sizeof(BMBUFHEAD)
#endif

/******************
**    PROTOTYPES 
******************/

void bmInitHeap( BMHEAP *heapHeader, UINT16 numBuf, UINT16 sizeBuf, void *basePtr );
void *bmAlloc( BMHEAP *heapHeader, UINT16 sizeBuf );
void bmFree( void *bufPtr );

#endif /* BM_H */

