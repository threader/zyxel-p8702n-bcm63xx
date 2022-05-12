/*****************************************************************************
**
**	FILE NAME:
**		sdf_mempool.c
**
******************************************************************************
**
**	DESCRIPTION:
**		This file contains the implementation of Sdf_st_mempool data structure.
**
**	DATE		NAME				REFERENCE		REASON
**----------------------------------------------------------------------------
**	14Mar2002	Ramu Kandula		HSS:XXXXXXXXX	Original
**
**
**	COPYRIGHT , Aricent, 2006
**
*****************************************************************************/

#include "sdf_mempool.h"
#include "sdf_portlayer.h"


/*****************************************************************************
**
**	FUNCTION : sdf_ivk_uaMempoolInit
**	
******************************************************************************
**
**  DESCRIPTION :
**	initializes the mempool;
**	if pBlk is Sdf_co_null, then this API mallocs the required memory.
**	if pFree is Sdf_co_null, then this API mallocs the required memory.
**
*****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaMempoolInit 
#ifdef ANSI_PROTO
	(Sdf_st_mempool *pMempool, 
	Sdf_ty_u32bit dNumBlks, 
	Sdf_ty_u16bit dBlkSize, 
	Sdf_ty_mempoolFptr	pfnInitBlk,
	Sdf_ty_mempoolFptr	pfnFreeBlk,
	Sdf_ty_pvoid pBlk, 
	Sdf_st_mempoolBlkContainer *pFree) 
#else
	(pMempool, dNumBlks, dBlkSize, pfnInitBlk, pBlk, pFree)
	Sdf_st_mempool *pMempool; /* mempool being initialized */
	Sdf_ty_u32bit dNumBlks; /* number of prealloc blocks in the pool */
	Sdf_ty_u16bit dBlkSize; /* size of each block in the pool */
	Sdf_ty_mempoolFptr	pfnInitBlk;
	Sdf_ty_mempoolFptr	pfnFreeBlk;
	Sdf_ty_pvoid pBlk; 		/* ptr to prealloc block array */
	Sdf_st_mempoolBlkContainer *pFree; /* ptr to blk container array of same
									** size */
#endif
{
	Sdf_ty_u32bit		i;
	Sdf_ty_bool	dIspBlkAlloced;
	Sdf_st_error dError;

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaInitSyncObject(&pMempool->dSyncLock, Sdf_co_false);
#endif	

	dIspBlkAlloced = Sdf_co_false;
	if (pBlk == Sdf_co_null)
	{
		dIspBlkAlloced = Sdf_co_true;
		pBlk = (Sdf_ty_pvoid) sdf_memget (0, dNumBlks * dBlkSize, &dError);
		if (pBlk == Sdf_co_null)
			return Sdf_co_fail;
	}

	if (pFree == Sdf_co_null)
	{
		pFree = (Sdf_st_mempoolBlkContainer*) sdf_memget (0, \
			dNumBlks * sizeof (Sdf_st_mempoolBlkContainer), &dError);
		if (pFree == Sdf_co_null)
		{
			if (dIspBlkAlloced)
				(void)sdf_memfree (0, &pBlk, &dError);
			return Sdf_co_fail;
		}
	}

	pMempool->dNumBlocks = dNumBlks;
	pMempool->dBlkSize = dBlkSize;
	pMempool->dNumAlloc = 0;
	pMempool->pfnInitBlk = pfnInitBlk;
	pMempool->pfnFreeBlk = pfnFreeBlk;
	pMempool->pBlk = pBlk;
	pMempool->pFreelist = pFree;
	pMempool->pExtn = Sdf_co_null;

	/** build up the free list : Free list is maintained as a singly-linked
	*** list with the unused block being used as the header node 
	**/

	/* header node */
	pFree[Sdf_co_mempoolUnusedBlkIndex].dSelfIndex = \
		Sdf_co_mempoolUnusedBlkIndex;
	pFree[Sdf_co_mempoolUnusedBlkIndex].dNextBlkIndex = 1;

	/* list */
	for (i=1; i<=(dNumBlks-1); ++i)
	{
		pFree[i].dFree = Sdf_co_true;
		pFree[i].dSelfIndex = i;
		pFree[i].dNextBlkIndex = i+1;
	}

	/* last node */
	pFree[dNumBlks-1].dFree = Sdf_co_true;
	pFree[dNumBlks-1].dSelfIndex = dNumBlks-1;
	pFree[dNumBlks-1].dNextBlkIndex = Sdf_co_mempoolUnusedBlkIndex;

	/* assign the tail of Free buffer list */
	pMempool->dTailFreeIndex = dNumBlks-1;

	/* Initialize all the preallocated blocks */
	if (pfnInitBlk != Sdf_co_null)
	{
		for (i=1; i<=(dNumBlks-1); ++i)
		{
			(*pfnInitBlk)((Sdf_ty_pvoid)((Sdf_ty_u8bit *)pBlk + \
					(dBlkSize * i)));
		}
	}
	
	return Sdf_co_success;
}

/*****************************************************************************
**
**	FUNCTION :
**		allocates a block from the memory pool
******************************************************************************
**
**  DESCRIPTION :
**	allocates a block from the memory pool;
**	returns pointer to the allocated blk and its index; 
**	returns Sdf_co_null on failure
**
*****************************************************************************/

Sdf_ty_pvoid sdf_ivk_uaMempoolAllocBlk 
#ifdef ANSI_PROTO
	(Sdf_st_mempool	*pMempool,
	Sdf_ty_u32bit 	*pBlkIndex) 
#else
	(pMempool, pBlkIndex)
	Sdf_st_mempool	*pMempool;
	Sdf_ty_u32bit 	*pBlkIndex; /* index of the block allocated */
#endif
{
	Sdf_ty_u32bit				dBlkIndex;
	Sdf_ty_pvoid				*pBlk;
	Sdf_st_mempoolBlkContainer	*pBlkCont;
	Sdf_st_mempoolExtn		*pExtn, *pTmpExtn1, *pTmpExtn2;
	Sdf_ty_u8bit				dNumBlocks;
	Sdf_ty_u8bit				dExtnIndex;
	Sdf_ty_u32bit				dLastIndex;
	Sdf_st_mempoolBlkContainer	*pFreelist;
	Sdf_ty_u32bit				dMask;
	Sdf_st_error 				dError;

#ifdef	SDF_THREAD_SAFE
	sdf_fn_uaLockSyncObject(0, &pMempool->dSyncLock, 0);
#endif	

	/* try to allocate a block from the free list */

	pFreelist = pMempool->pFreelist;
	if (pFreelist[Sdf_co_mempoolUnusedBlkIndex].dNextBlkIndex 
		!= Sdf_co_mempoolUnusedBlkIndex)
	{
		/* block available in the free list */

		dBlkIndex = pFreelist[Sdf_co_mempoolUnusedBlkIndex].dNextBlkIndex;
		pBlk = (Sdf_ty_pvoid*) (((Sdf_ty_u8bit *) pMempool->pBlk) 
				+ (pMempool->dBlkSize * dBlkIndex));
		pBlkCont = &pFreelist[dBlkIndex];
		*pBlkIndex = dBlkIndex;

		pFreelist[Sdf_co_mempoolUnusedBlkIndex].dNextBlkIndex = \
			pBlkCont->dNextBlkIndex;
		pBlkCont->dFree = Sdf_co_false;

#ifdef	SDF_THREAD_SAFE
		sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif	

		pMempool->dNumAlloc ++;
		return pBlk;
	}


	/* try to allocate a block from the extn containers */
	pExtn = pMempool->pExtn;
	while (pExtn != Sdf_co_null)
	{
		dMask = 0x01;
		for (dExtnIndex=0; dExtnIndex<Sdf_co_mempoolNumBlksPerExtn; \
			++dExtnIndex)
		{
			if ( !(pExtn->dAvailFlag & dMask) )
				break;
			else
				dMask = dMask << 1;
		}

		if (dExtnIndex < Sdf_co_mempoolNumBlksPerExtn)
		{
			/* free blk found */
			*pBlkIndex = pExtn->dStartIndex + dExtnIndex;
			pBlk = (Sdf_ty_pvoid*) (((Sdf_ty_u8bit *) pExtn->pBlk)
					+ (pMempool->dBlkSize * dExtnIndex));
			pExtn->dAvailFlag |= dMask;
	
#ifdef SDF_THREAD_SAFE	
			sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif	

			pMempool->dNumAlloc ++;
			return pBlk;	
		}
		else
		{
			/* look in the next extn container */
			pExtn = pExtn->pNextExtn;
		}
	}


	/* create a new extn container and alloc a block from it */

	pExtn = (Sdf_st_mempoolExtn *) sdf_memget (0, \
		sizeof (Sdf_st_mempoolExtn), &dError);
	if (pExtn == Sdf_co_null)
	{
		/* failed to allocate a block */
#ifdef SDF_THREAD_SAFE	
		sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif		
		*pBlkIndex = Sdf_co_mempoolUnusedBlkIndex;
		return Sdf_co_null;
	}

	for (dNumBlocks=Sdf_co_mempoolNumBlksPerExtn; dNumBlocks > 0; \
		dNumBlocks /= 2)
	{
		pExtn->pBlk = (Sdf_ty_pvoid) sdf_memget(0, \
			pMempool->dBlkSize * dNumBlocks, &dError);
		if (pExtn->pBlk != Sdf_co_null)
		{
			pExtn->pBlkCont = (Sdf_st_mempoolBlkContainer *) sdf_memget (0,
					sizeof (Sdf_st_mempoolBlkContainer) * dNumBlocks, &dError);
			if (pExtn->pBlkCont == Sdf_co_null)
			{
				/* release this chunk and try getting a smaller chunk */
				(void)sdf_memfree (0, &pExtn->pBlk, &dError);
			}
			else
			{
				break;
			}
		}
	}

	if (dNumBlocks != 0)
	{
		pExtn->dNumBlocks = dNumBlocks;
		pExtn->dAvailFlag = 0;

		/* scan the extn list and insert the extn container at a place
		** where there is sufficient index space for dNumBlocks
		*/
		pTmpExtn1 = pMempool->pExtn; /* used to traverse the list */
		pTmpExtn2 = Sdf_co_null; /* used to keep track of prev node */
		dLastIndex = pMempool->dNumBlocks-1;
		while (pTmpExtn1 != Sdf_co_null)
		{
			if ((pTmpExtn1->dStartIndex - dLastIndex) >= dNumBlocks)
			{
				/* insert the new container before this node */
				break; 
			}
			else 
			{
				dLastIndex = pTmpExtn1->dStartIndex + 
							pTmpExtn1->dNumBlocks - 1;
				pTmpExtn2 = pTmpExtn1;
				pTmpExtn1 = pTmpExtn1->pNextExtn;
			}
		}

		if (pTmpExtn2 == Sdf_co_null)
		{
			/* 1st node of the list */
			pExtn->pNextExtn = pTmpExtn1;
			pMempool->pExtn = pExtn;
		}
		else
		{
			/* insert in the middle or at the end of the list */
			pExtn->pNextExtn = pTmpExtn2->pNextExtn;
			pTmpExtn2->pNextExtn = pExtn;
		}

		/* initialize the indices of the new extn container */
		pExtn->dStartIndex = dLastIndex + 1;
		for (dExtnIndex=0; dExtnIndex<dNumBlocks; ++dExtnIndex)
		{
			pBlkCont = &pExtn->pBlkCont[dExtnIndex];
			pBlkCont->dSelfIndex = pExtn->dStartIndex + dExtnIndex;
		}

		/* initialize the newly allocated blocks */
		if (pMempool->pfnInitBlk != Sdf_co_null)
		{
			for (dExtnIndex=0; dExtnIndex<dNumBlocks; ++dExtnIndex)
			{
				(*pMempool->pfnInitBlk)((Sdf_ty_pvoid)((Sdf_ty_u8bit *) \
				 				pExtn->pBlk + (pMempool->dBlkSize * \
								dExtnIndex)));
			}
		}

		/* alloc and return a blk */
		pExtn->dAvailFlag |= 0x01;
		*pBlkIndex = pExtn->dStartIndex;
		/* Needs Verification */
		pBlk = (Sdf_ty_pvoid*)pExtn->pBlk;


#ifdef SDF_THREAD_SAFE	
		sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif		

		pMempool->dNumAlloc ++;
		return pBlk;
	}
	else
	{
		/* unable to malloc mem for blks and its containers */
		(void)sdf_memfree (0, (Sdf_ty_pvoid *)&pExtn, &dError);
	}

	/* failed to allocate a block */
#ifdef SDF_THREAD_SAFE	
	sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif	
	*pBlkIndex = Sdf_co_mempoolUnusedBlkIndex;
	return Sdf_co_null;
}

/*****************************************************************************
**
**	FUNCTION :
**		returns the block to the free pool of the mempool
******************************************************************************
**
**  DESCRIPTION :
**	returns the block with specified index to the free pool
**
**	NOTE :
**	ensure that the block is removed from hash table, linked list, etc.
**	before it is returned to the free pool
**
*****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaMempoolFreeBlk 
#ifdef ANSI_PROTO
	(Sdf_st_mempool	*pMempool, 
	Sdf_ty_u32bit	dBlkIndex) 
#else
	(pMempool, dBlkIndex)
	Sdf_st_mempool	*pMempool; 
	Sdf_ty_u32bit	dBlkIndex; /* index of the block to be freed */
#endif
{
	Sdf_st_mempoolExtn		*pExtn, *pTmpExtn;
	Sdf_ty_u8bit					dExtnIndex;
	Sdf_st_error dError;
#ifdef SDF_LINT            
           Sdf_ty_u32bit dTemp = 0;
#endif					 
	
	if (dBlkIndex == Sdf_co_mempoolUnusedBlkIndex)
	{
		/* Attempt to free Header block */
		return Sdf_co_fail;
	}

#ifdef SDF_THREAD_SAFE	
	sdf_fn_uaLockSyncObject(0, &pMempool->dSyncLock, 0);
#endif	

	/* if the blk being freed is a prealloc blk
	** return it to free pool 
	*/
	if (dBlkIndex < pMempool->dNumBlocks)
	{
		if (pMempool->pFreelist[dBlkIndex].dFree == Sdf_co_true)
		{
			/* this block is already free */
#ifdef SDF_THREAD_SAFE	
			sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif			
			return Sdf_co_fail;
		} 

		/* Set the newly free buffer to tail of allocated buffers */
		pMempool->pFreelist[pMempool->dTailFreeIndex].dNextBlkIndex=dBlkIndex;
		pMempool->dTailFreeIndex = dBlkIndex;

		pMempool->pFreelist[dBlkIndex].dNextBlkIndex = \
			Sdf_co_mempoolUnusedBlkIndex;

		pMempool->pFreelist[dBlkIndex].dFree = Sdf_co_true;
		
#ifdef SDF_THREAD_SAFE	
		sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif		

		pMempool->dNumAlloc --;
		return Sdf_co_success;
	}

	/* locate the extn blk to which the blk belongs */
	pExtn = pMempool->pExtn;
	pTmpExtn = Sdf_co_null; /* used to keep track of prev node */
	while (pExtn != Sdf_co_null)
	{
		if ( (pExtn->dStartIndex <= dBlkIndex) &&
			 (dBlkIndex < (pExtn->dStartIndex + pExtn->dNumBlocks)) )
		{
			dExtnIndex = dBlkIndex - pExtn->dStartIndex;
			if ( ! (pExtn->dAvailFlag & (0x01 << dExtnIndex)) )
			{
				/* this block is already free */
#ifdef SDF_THREAD_SAFE	
				sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif				
				return Sdf_co_fail;
			}
			
			/* set the free bit to 0 */
#ifdef SDF_LINT            
           dTemp = (0x01 << dExtnIndex);
           pExtn->dAvailFlag &= ~dTemp; 
#else            
			pExtn->dAvailFlag &= ~(0x01 << dExtnIndex);
#endif            

			/* 
			 * free extn container if no blk of this extn container is 
			 * being used 
			 */
			if ( ! pExtn->dAvailFlag )
			{
				/* 
				 * Invoke pfnFreeBlk for all blocks in this container before 
				 * freeing the container 
				 */
				if (pMempool->pfnFreeBlk != Sdf_co_null)
				{
					Sdf_ty_u8bit i;

					for (i=0; i<pExtn->dNumBlocks; ++i)
					{
						(*pMempool->pfnFreeBlk)((Sdf_ty_pvoid)(\
							(Sdf_ty_u8bit *) pExtn->pBlk + \
							(pMempool->dBlkSize * i)));
					}
				}

				(void)sdf_memfree (0, &pExtn->pBlk, &dError);	
				(void)sdf_memfree (0, (Sdf_ty_pvoid *)&pExtn->pBlkCont, &dError);

				if (pTmpExtn == Sdf_co_null)
				{
					/* delete 1st node in the list */
					pMempool->pExtn = pExtn->pNextExtn;
				}
				else
				{
					/* delete from the middle or end */
					pTmpExtn->pNextExtn = pExtn->pNextExtn;
				}
				(void)sdf_memfree (0, (Sdf_ty_pvoid *)&pExtn, &dError);
			}
			
#ifdef SDF_THREAD_SAFE	
			sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif			

			pMempool->dNumAlloc --;
			return Sdf_co_success;
		}
		else
		{
			/* look in the next extn container */
			pTmpExtn = pExtn;
			pExtn = pExtn->pNextExtn;
		}
	}

	/* blk not found */
#ifdef SDF_THREAD_SAFE	
	sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif	
	return Sdf_co_fail;
}

/*****************************************************************************
**
**	FUNCTION :
**		retrieve a block from the mempool
******************************************************************************
**
**  DESCRIPTION :
**	retrieves the block with specified index from the mempool;
**	returns the pointer to block with the specified index and a pointer to 
**		the block's container if ppBlkCont is not Sdf_co_null;
**	returns Sdf_co_null if a block with the specified index does not exist
**
**	NOTE:
**	take care not to retrieve and use an unallocated block; the entire
**	mempool may get corrupted
**
*****************************************************************************/

Sdf_ty_pvoid sdf_ivk_uaMempoolGetBlk 
#ifdef ANSI_PROTO
	(Sdf_st_mempool	*pMempool, 
	Sdf_ty_u32bit	dBlkIndex, 
	Sdf_st_mempoolBlkContainer **ppBlkCont) 
#else
	(pMempool, dBlkIndex, ppBlkCont)
	Sdf_st_mempool	*pMempool; 
	Sdf_ty_u32bit	dBlkIndex; /* index of the block to be retrieved */
	Sdf_st_mempoolBlkContainer	**ppBlkCont; /* ptr to blk's container */
#endif
{
	Sdf_ty_pvoid			pBlk;
	Sdf_st_mempoolExtn		*pExtn;
	Sdf_ty_u8bit			dExtnIndex;

#ifdef SDF_THREAD_SAFE	
	sdf_fn_uaLockSyncObject(0, &pMempool->dSyncLock, 0);
#endif	

	/* if the blk is a prealloc blk return its address immediately */
	if (dBlkIndex < pMempool->dNumBlocks)
	{
		pBlk = (Sdf_ty_u8bit *) pMempool->pBlk +
				(pMempool->dBlkSize * dBlkIndex);
		if (ppBlkCont != Sdf_co_null)
		{
			*ppBlkCont = &(pMempool->pFreelist[dBlkIndex]);
		}

#ifdef SDF_THREAD_SAFE	
		sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif		

		return pBlk;
	}

	/* locate the extn blk to which the blk belongs */
	pExtn = pMempool->pExtn;
	while (pExtn != Sdf_co_null)
	{
		if ( (pExtn->dStartIndex <= dBlkIndex) &&
			 (dBlkIndex < (pExtn->dStartIndex + pExtn->dNumBlocks)) )
		{
			dExtnIndex = dBlkIndex - pExtn->dStartIndex;
			pBlk = (Sdf_ty_u8bit *) pExtn->pBlk +
					(pMempool->dBlkSize * dExtnIndex);
			if (ppBlkCont != Sdf_co_null)
			{
				*ppBlkCont = &(pExtn->pBlkCont[dExtnIndex]);
			}

#ifdef SDF_THREAD_SAFE	
			sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif			

			return pBlk;
		}
		else
		{
			/* look in the next extn container */
			pExtn = pExtn->pNextExtn;
		}
	}

	/* blk not found */
#ifdef SDF_THREAD_SAFE	
	sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
#endif	
	return Sdf_co_null;
}

/*****************************************************************************
**
**	FUNCTION :
**		Destroy the mempool
******************************************************************************
**
**  DESCRIPTION :
**		Free all resources allocated by this mempool
**
*****************************************************************************/

void  sdf_ivk_uaMempoolDestroy
#ifdef ANSI_PROTO
	(Sdf_st_mempool	*pMempool)
#else
	(pMempool)
	Sdf_st_mempool	*pMempool;
#endif
{
	Sdf_st_mempoolExtn		*pExtn, *pTmpExtn;
	Sdf_ty_u32bit			i;
	Sdf_st_error 			dError;

#ifdef SDF_THREAD_SAFE	
	sdf_fn_uaLockSyncObject(0, &pMempool->dSyncLock, 0);
#endif	

	/* free up all the extension containers */
	pExtn = pMempool->pExtn;
	pTmpExtn = Sdf_co_null; /* used to keep track of prev node */
	while (pExtn != Sdf_co_null)
	{
		/* 
		 * Invoke pfnFreeBlk for all blocks in this extn container before 
		 * freeing the extn container 
		 */
		if (pMempool->pfnFreeBlk != Sdf_co_null)
		{
			for (i=1; i<pExtn->dNumBlocks; ++i)
			{
				(*pMempool->pfnFreeBlk)((Sdf_ty_pvoid)(\
					(Sdf_ty_u8bit *) pExtn->pBlk + \
					(pMempool->dBlkSize * i)));
			}
		}

		pTmpExtn = pExtn;
		pExtn = pExtn->pNextExtn;
		(void)sdf_memfree (0, &pTmpExtn->pBlk, &dError);	
		(void)sdf_memfree (0, (Sdf_ty_pvoid *)&pTmpExtn->pBlkCont, &dError);
	}
	pMempool->pExtn = Sdf_co_null;

	/* 
	 * Invoke pfnFreeBlk for all blocks in main prealloc container
	 */
	if (pMempool->pfnFreeBlk != Sdf_co_null)
	{
		for (i=1; i<pMempool->dNumBlocks; ++i)
		{
			(*pMempool->pfnFreeBlk)((Sdf_ty_u8bit *)(pMempool->pBlk) + \
					(i * pMempool->dBlkSize));
		}
	}

	/* free the main prealloc memory */
	(void)sdf_memfree(0, &pMempool->pBlk, &dError);
	(void)sdf_memfree(0, (Sdf_ty_pvoid *)&pMempool->pFreelist, &dError);
	
#ifdef SDF_THREAD_SAFE	
	sdf_fn_uaUnlockSyncObject(0, &pMempool->dSyncLock);
	sdf_fn_uaDestroySyncObject(&pMempool->dSyncLock);
	
#endif	

	return;
}


/*****************************************************************************
**
**	FUNCTION :
**		compute the % usage of mempool
******************************************************************************
**
**  DESCRIPTION :
**	returns the % of blocks that have been allocated with respect to the
**	number of preallocated blocks in the pool. The returned value may be more
**	than 100 if extension containers have been used in the mempool
**
*****************************************************************************/

Sdf_ty_u16bit sdf_ivk_uaMempoolGetUsage 
#ifdef ANSI_PROTO
	(Sdf_st_mempool	*pMempool)
#else
	(pMempool)
	Sdf_st_mempool	*pMempool;
#endif
{
	return (Sdf_ty_u16bit) ((float)(pMempool->dNumAlloc+1)/ \
		(pMempool->dNumBlocks) * 100);
}

