/*****************************************************************************
**
**	FILE NAME:
**		sdf_mempool.h
**
******************************************************************************
**
**	DESCRIPTION:
**		This file contains the data structure definitions and interface 
** 	function prototypes memory pool management data structure
**
**	DATE		NAME				REFERENCE		REASON
**----------------------------------------------------------------------------
**	14Mar2002	Ramu Kandula 		HSS:XXXXXXXXX	Original
**
**
**	COPYRIGHT , Aricent, 2006
**
*****************************************************************************/

#ifndef	__SDF_MEMPOOL_H__
#define	__SDF_MEMPOOL_H__

#include "sdf_common.h"

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Sdf_st_mempoolBlkContainer
{
	Sdf_ty_u32bit	dSelfIndex;
	Sdf_ty_bool		dFree;
	Sdf_ty_u32bit	dNextBlkIndex; 
} Sdf_st_mempoolBlkContainer;

/* Each extension container maintains upto 32 blocks. The availability 
** status of each block is maintained by means of a bitmap. The entire 
** extension container is freed when no block is being used
*/
typedef struct Sdf_st_mempoolExtn
{
	Sdf_ty_u8bit		dNumBlocks; /* number of blocks allocated */
	Sdf_ty_u32bit		dStartIndex;
	Sdf_ty_u32bit		dAvailFlag;	/* bit set to 1 if blk is used; 
							** lsb corresponds to 1st blk 
							*/
	Sdf_ty_pvoid		pBlk; /* block array; try to alloc 32 blks;
							** if not possible, then try 16 and so on */
	Sdf_st_mempoolBlkContainer	*pBlkCont; /* containers for blocks */
	struct 	Sdf_st_mempoolExtn	*pNextExtn; /* ptr to next extn container */
} Sdf_st_mempoolExtn;

typedef struct Sdf_st_mempool
{
	Sdf_ty_u32bit			dNumBlocks; /* number of prealloc blocks */
	Sdf_ty_u16bit			dBlkSize; /* size of each block in the pool */
	Sdf_ty_u32bit			dNumAlloc; /* number of blks currently alloc'ed */
	Sdf_ty_pvoid			pBlk; /* ptr to prealloc block array */	
	Sdf_st_mempoolBlkContainer	*pFreelist; /* ptr to prealloc blk container 
										 ** array of the same size */
	void					(*pfnInitBlk)(Sdf_ty_pvoid); /* block init func */
	void					(*pfnFreeBlk)(Sdf_ty_pvoid); /* block free func */
	Sdf_ty_u32bit           dTailFreeIndex; /* Index of the Tail of Free Buffer List. */
	Sdf_st_mempoolExtn		*pExtn; /* mempool extension container list */
#ifdef SDF_THREAD_SAFE
	Sdf_ty_syncIdT			dSyncLock;
#endif	
} Sdf_st_mempool;


/****************************************************************************
**
**	#defs for common data structs
**
****************************************************************************/

#define Sdf_co_mempoolUnusedBlkIndex	0x00
#define Sdf_co_mempoolNumBlksPerExtn	32 	/* must be a power of 2; max = 32 */


/****************************************************************************
**
**	Interface for Sdf_st_mempool type
**
****************************************************************************/

/*	sdf_ivk_uaMempoolInit ()
**	initializes the mempool; 
** 	if pBlk is Sdf_co_null, then sdf_ivk_uaMempoolInit mallocs 
**	the required memory.
** 	if pFree is Sdf_co_null, then sdf_ivk_uaMempoolInit mallocs 
**	the required memory.
*/


typedef void (*Sdf_ty_mempoolFptr) (Sdf_ty_pvoid);

Sdf_ty_retVal sdf_ivk_uaMempoolInit _ARGS_
((	Sdf_st_mempool	*pMempool,/* mempool being initialized */
	Sdf_ty_u32bit		dNumBlks,/* number of prealloc blocks in the pool */	
	Sdf_ty_u16bit		dBlkSize, /* size of each block in the pool */
	Sdf_ty_mempoolFptr	pfnInitBlk,/* func to initialize block 
									when it is created */
	Sdf_ty_mempoolFptr	pfnFreeBlk,/*Func to Free Block when its container is
									being destroyed */
	Sdf_ty_pvoid	pBlk,		/* ptr to prealloc block array */
	Sdf_st_mempoolBlkContainer	 *pFree )); 
	
/* 	sdf_ivk_uaMempoolAllocBlk ()
** 	returns pointer to alloc blk and its index; returns Sdf_co_null on failure
*/
Sdf_ty_pvoid sdf_ivk_uaMempoolAllocBlk _ARGS_
((	Sdf_st_mempool	*pMempool,
	Sdf_ty_u32bit	*pIndex )); 

/* 	sdf_ivk_uaMempoolFreeBlk ()
** 	returns the block with specified index to the free pool
**	WARN: ensure that the block is removed from hash table / linked list
**		  before it is returned to the free pool
*/
Sdf_ty_retVal sdf_ivk_uaMempoolFreeBlk _ARGS_
((	Sdf_st_mempool	*pMempool,
	Sdf_ty_u32bit	dIndex )); 	/* dIndex of the block to be freed */

/* 	sdf_ivk_uaMempoolGetBlk ()
**	returns the pointer to block with the specified index; if ppBlkCont
**		is not null, then the ptr to the block's container is returned in 
**		*ppBlkCont
** 	returns Sdf_co_null if a block with the specified index does not exist
**	WARN: take care not to retrieve and use an unallocated block; the entire
**		  mempool may get corrupted 
*/
Sdf_ty_pvoid sdf_ivk_uaMempoolGetBlk _ARGS_
(( 	Sdf_st_mempool	*pMempool,
	Sdf_ty_u32bit	dIndex,	/* index of the block to be retrieved */
	Sdf_st_mempoolBlkContainer	**ppBlkCont)); /* ptr to retrieved blks 
							* container */

/* sdf_ivk_uaMempoolDestroy
** frees up all the resources allocated by this mempool
*/
void sdf_ivk_uaMempoolDestroy _ARGS_
((	Sdf_st_mempool *pMempool));


/* 	sdf_ivk_uaMempoolGetUsage ()
**	returns the % usage of the mempool
*/
Sdf_ty_u16bit sdf_ivk_uaMempoolGetUsage _ARGS_
(( 	Sdf_st_mempool	*pMempool ));



#ifdef __cplusplus
}
#endif
#endif /* __SDF_MEMPOOL_H__ */
