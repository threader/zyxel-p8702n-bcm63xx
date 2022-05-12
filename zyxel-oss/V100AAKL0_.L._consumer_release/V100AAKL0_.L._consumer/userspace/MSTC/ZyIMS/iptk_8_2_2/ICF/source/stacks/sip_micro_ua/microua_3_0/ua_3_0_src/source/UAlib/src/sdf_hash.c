/******************************************************************************
 ** FUNCTION:	
 **		Has the hashtable implementation used in the UA Toolkit.
 **
 ******************************************************************************
 **
 ** FILENAME: 		sdf_hash.c
 **
 ** DESCRIPTION:	This file contains the implementation of hash table 
 **					used within the UA Toolkit.
 **
 ** DATE     	NAME          REFERENCE     REASON
 ** ----      	----          ---------     ------
 ** 12/04/01	K. Binu, 	  siphash.c		Creation
 **				Siddharth	  
 **
 ** 25/06/02	Siddharth					Addition of exclusive operation
 **											APIs
 ** 12-Jun-09   Anuradha Gupta  SPR 19737   klocoworks warning removal
 **
 ******************************************************************************
 ** 			COPYRIGHT , Aricent, 20069
 ******************************************************************************/

#include "ALheader.h"
#include "sdf_common.h"
#include "sdf_struct.h"
#ifndef SDF_LINT
#include "sdf_sdpstruct.h"
#endif
#include "sdf_portlayer.h"
#include "sdf_hash.h"
#include "sdf_debug.h"

/* klocworks: NULL check for pointers before access */

/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashInit
 ** 
 ** DESCRIPTION: 	This is the function to initialize
 **					a new hash table.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashInit 
#ifdef ANSI_PROTO
	(Sdf_st_hash *pHash, \
	Sdf_ty_uaHashFunc fpHashFunc,\
	Sdf_ty_uaHashKeyCompareFunc fpCompareFunc, \
	Sdf_ty_uaHashElementFreeFunc fpElemFreeFunc, \
	Sdf_ty_uaHashKeyFreeFunc fpKeyFreeFunc, \
	Sdf_ty_u32bit numBuckets, \
	Sdf_ty_u32bit maxElements, \
	Sdf_st_error *pErr)
#else
	(pHash, fpHashFunc, fpCompareFunc, fpElemFreeFunc, \
		fpKeyFreeFunc, numBuckets, maxElements)
	Sdf_st_hash *pHash;
	Sdf_ty_uaHashFunc fpHashFunc; /* function to calculate hash value */
	Sdf_ty_uaHashKeyCompareFunc fpCompareFunc; /* function to compare keys of 2 elements */
	Sdf_ty_uaHashElementFreeFunc fpElemFreeFunc; /* function to free the data being stored */
	Sdf_ty_uaHashKeyFreeFunc fpKeyFreeFunc; /* function to free the key given for an element */
	Sdf_ty_u32bit numBuckets;
	Sdf_ty_u32bit maxElements;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit i=0;
	Sdf_st_error dError;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaHashInit");

	/* Initialize structure variables */
	pHash->fpHashFunc = fpHashFunc;
	pHash->fpCompareFunc = fpCompareFunc;
	pHash->numberOfBuckets = numBuckets;
	pHash->numberOfElements = 0;
	pHash->maxNumberOfElements = maxElements;
	pHash->fpElementFreeFunc = fpElemFreeFunc;
	pHash->fpKeyFreeFunc = fpKeyFreeFunc;
	
	/* Allocate space for buckets */
	pHash->ppHashChains = (Sdf_st_hashElement **) \
	sdf_memget(Sdf_mc_initMemId, sizeof(Sdf_st_hashElement*)*numBuckets,\
			&dError);
	if (pHash->ppHashChains == Sdf_co_null)
	{
		pErr->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

#ifdef SDF_THREAD_SAFE
	/* Allocate space for the mutexes */
	pHash->pBucketSyncObject = (Sdf_ty_syncIdT *) \
		sdf_memget(Sdf_mc_initMemId, sizeof(Sdf_ty_syncIdT)*numBuckets, \
		&dError);
	if (pHash->pBucketSyncObject == Sdf_co_null)
	{
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid *)&(pHash->ppHashChains), \
			&dError);
		pErr->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}
#endif

	/* Initialize the buckets. Also init a mutex for each bucket */
	for(i=0; i<numBuckets; i++)
	{
		pHash->ppHashChains[i] = Sdf_co_null;
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaInitSyncObject(&(pHash->pBucketSyncObject[i]), Sdf_co_false);
#endif
	}
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaInitSyncObject(&(pHash->dHashSyncObject), Sdf_co_false);
#endif
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaHashInit");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashFree
 ** 
 ** DESCRIPTION: 	This is the function to free members from the hash 
 **					table. It does not free the hash elements, but frees 
 **					other member variables malloced at the time of Init 
 **					of the hash table
 **					
 **					NOTE :
 **					=====
 **					This function *must not* be invoked when there are 
 **					threads working on the hash table.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashFree
#ifdef ANSI_PROTO
	(Sdf_st_hash *pHash, Sdf_st_error *pErr) 
#else
	(pHash, pErr)
	Sdf_st_hash *pHash;
	Sdf_st_error pErr;
#endif
{
#ifdef SDF_THREAD_SAFE
	Sdf_ty_u32bit i=0;
#endif

	(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid *)&(pHash->ppHashChains), pErr);
#ifdef SDF_THREAD_SAFE
	for(i=0; i<pHash->numberOfBuckets; i++)
	{
		sdf_fn_uaDestroySyncObject(&(pHash->pBucketSyncObject[i]));
	}
	(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid *)&(pHash->pBucketSyncObject), pErr);
    sdf_fn_uaDestroySyncObject(&(pHash->dHashSyncObject));
#endif
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashAdd
 ** 
 ** DESCRIPTION: 	This is the function to add an entry
 **					into the hash table.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashAdd
#ifdef ANSI_PROTO
	(Sdf_st_hash *pHash, Sdf_ty_pvoid pElement, Sdf_ty_pvoid pKey)
#else
	(pHash, pElement, pKey)
	Sdf_st_hash *pHash; 
	Sdf_ty_pvoid pElement; 
	Sdf_ty_pvoid pKey;
#endif
{
	Sdf_ty_u32bit hashKey=0;
	Sdf_ty_u32bit bucket=0;
	Sdf_st_hashElement* pNewElement = Sdf_co_null;
	Sdf_st_hashElement* pIterator = Sdf_co_null;
	Sdf_st_error dError;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaHashAdd");

#ifdef SDF_PARAMVALIDATION
	if ((pHash == Sdf_co_null) || (pKey == Sdf_co_null))
		return Sdf_co_fail;
#endif	/* End of param validation ifdef */
		
	/* Check if the max number of Elements is getting exceeded */
	if (pHash->numberOfElements == (pHash->maxNumberOfElements))
		return Sdf_co_fail;

	/* Compute hash for the element */
	hashKey = pHash->fpHashFunc(pKey);

	/* Locate the bucket */
	bucket = hashKey % pHash->numberOfBuckets;
	
#ifdef SDF_THREAD_SAFE
	/* Grab lock for the bucket in which 
	   the operation is to be done */
	sdf_fn_uaLockSyncObject(0, &(pHash->pBucketSyncObject[bucket]), 0);
#endif

	/* Ensure that the key is not already present */
	pIterator = pHash->ppHashChains[bucket];
	while(pIterator != Sdf_co_null)
	{
		if(pHash->fpCompareFunc(pIterator->pKey, pKey) == 0)
		{
			/* The key already exists. Return failure here */

#ifdef SDF_THREAD_SAFE
			/* Unlock the mutex for the bucket now */
			sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif	
			return Sdf_co_fail;
		}
		pIterator = pIterator->pNext;
	}

	/* Allocate and initialize element holder */
	pNewElement = (Sdf_st_hashElement *)\
	sdf_memget(Sdf_mc_initMemId, sizeof(Sdf_st_hashElement), &dError);
	
	if (pNewElement == Sdf_co_null)
	{
#ifdef SDF_THREAD_SAFE
		/* Unlock the mutex for the bucket before returning */
		sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif
		return Sdf_co_fail;
	}

	pNewElement->pElement = pElement;
	pNewElement->pKey = pKey;
	pNewElement->dRemove = Sdf_co_false;
	HSS_INITREF(pNewElement->dRefCount);

	/* Push element into the bucket */
	pNewElement->pNext = pHash->ppHashChains[bucket];
	pHash->ppHashChains[bucket] = pNewElement;
#ifdef SDF_THREAD_SAFE
	/* Hash-table level locking */
	sdf_fn_uaLockSyncObject(0, &(pHash->dHashSyncObject), 0);
#endif
	pHash->numberOfElements++;
#ifdef SDF_THREAD_SAFE
	/* Hash-table level unlocking */
	sdf_fn_uaUnlockSyncObject(0, &(pHash->dHashSyncObject));
#endif
#ifdef SDF_THREAD_SAFE
	/* Unlock the mutex for the bucket now */
	sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaHashAdd");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashFetch
 ** 
 ** DESCRIPTION: 	This is the function to fetch an entry
 **					from the hash table.
 **
 ******************************************************************************/
Sdf_ty_pvoid sdf_ivk_uaHashFetch
#ifdef ANSI_PROTO
	(Sdf_st_hash *pHash, Sdf_ty_pvoid pKey)
#else
	(pHash, pKey)
	Sdf_st_hash *pHash;
	Sdf_ty_pvoid pKey;
#endif
{
	Sdf_ty_u32bit hashKey=0;
	Sdf_ty_u32bit bucket=0;
	Sdf_st_hashElement* pIterator = Sdf_co_null;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaHashFetch");

#ifdef SDF_PARAMVALIDATION
	if ((pKey == Sdf_co_null) || (pHash == Sdf_co_null))
		return Sdf_co_null;
#endif	/* End of param validation ifdef */

	/* Compute hash for the element */
	hashKey = pHash->fpHashFunc(pKey);
	
	/* Locate the bucket */
	bucket = hashKey % pHash->numberOfBuckets;
	
#ifdef SDF_THREAD_SAFE
	/* Grab lock for the bucket in which 
	   the operation is to be done */
	sdf_fn_uaLockSyncObject(0, &(pHash->pBucketSyncObject[bucket]), 0);
#endif

	/* Go through chain */
	pIterator = pHash->ppHashChains[bucket];
	
	while(pIterator != Sdf_co_null)
	{
		if(pHash->fpCompareFunc(pIterator->pKey, pKey) == 0)
		{
			/* Increment reference count */
			HSS_LOCKEDINCREF(pIterator->dRefCount);
			break;
		}
		pIterator = pIterator->pNext;
	}
	
#ifdef SDF_THREAD_SAFE
	/* Unlock the mutex for the bucket now */
	sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaHashFetch");

	if (pIterator == Sdf_co_null)
	 	return Sdf_co_null;
	else
		return pIterator->pElement;
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashRemove
 ** 
 ** DESCRIPTION: 	This is the function to remove an entry
 **					from the hash table.
 **
 **					Note:
 **					-----
 **					This function should not be invoked if memory does not 
 **					have to be freed at time of invocation. 
 **					sdf_ivk_uaHashRelease( ) should be called instead.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashRemove
#ifdef ANSI_PROTO
	(Sdf_st_hash *pHash, Sdf_ty_pvoid pKey)
#else
	(pHash, pKey)
	Sdf_st_hash *pHash;
	Sdf_ty_pvoid pKey;
#endif
{
	Sdf_st_hashElement *pTempElement = Sdf_co_null;
	Sdf_ty_u32bit hashKey=0;
	Sdf_ty_u32bit bucket=0;
	Sdf_st_hashElement **ppIterator = Sdf_co_null;
	Sdf_st_error dError;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaHashRemove");

#ifdef SDF_PARAMVALIDATION
	if ((pKey == Sdf_co_null) || (pHash == Sdf_co_null))
		return Sdf_co_fail;
#endif	/* End of param validation ifdef */

	/* Compute hash for the element */
	hashKey = pHash->fpHashFunc(pKey);

	/* Locate the bucket */
	bucket = hashKey % pHash->numberOfBuckets;

#ifdef SDF_THREAD_SAFE
	/* Grab lock for the bucket in which 
	   the operation is to be done */
	sdf_fn_uaLockSyncObject(0, &(pHash->pBucketSyncObject[bucket]), 0);
#endif
	
	/* Go through chain */
	ppIterator = &(pHash->ppHashChains[bucket]);

	while(*ppIterator != Sdf_co_null)
	{
		if(pHash->fpCompareFunc((*ppIterator)->pKey, pKey) == 0)
			break;
		ppIterator = &((*ppIterator)->pNext);
	}
	if(*ppIterator == Sdf_co_null)
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif
		return Sdf_co_fail;
	}

	/* 	check if this hash entry is in use.
		If so just set the remove flag and return */
	HSS_LOCKREF((*ppIterator)->dRefCount);
	HSS_DECREF((*ppIterator)->dRefCount);
	if (HSS_CHECKREF((*ppIterator)->dRefCount))
	{
		pTempElement = *ppIterator;
		*ppIterator = (*ppIterator)->pNext;
		if (pHash->fpElementFreeFunc != Sdf_co_null)
			pHash->fpElementFreeFunc(pTempElement->pElement);
		if (pHash->fpKeyFreeFunc != Sdf_co_null)
			pHash->fpKeyFreeFunc(pTempElement->pKey);
		HSS_UNLOCKREF(pTempElement->dRefCount);
		HSS_DELETEREF(pTempElement->dRefCount);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid *)&pTempElement,&dError);
#ifdef SDF_THREAD_SAFE
	    /* Hash-table level locking */
	    sdf_fn_uaLockSyncObject(0, &(pHash->dHashSyncObject), 0);
#endif
		pHash->numberOfElements--;
#ifdef SDF_THREAD_SAFE
	    /* Hash-table level unlocking */
	    sdf_fn_uaUnlockSyncObject(0, &(pHash->dHashSyncObject));
#endif
	}
	else
	{
		/* 	After decrementing refcount also, it is
			not yet zero. This means the element
			was checked out recently. When its checked
			in the memory will be freed. Set the remove
			flag to true for the moment */
		(*ppIterator)->dRemove = Sdf_co_true;
		HSS_INCREF((*ppIterator)->dRefCount);
		HSS_UNLOCKREF((*ppIterator)->dRefCount);
	}
	

#ifdef SDF_THREAD_SAFE
	/* Unlock the mutex for the bucket now */
	sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaHashRemove");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashRelease
 ** 
 ** DESCRIPTION:	This function should be invoked to "check in" an element 
 **					that was obtained from the hash table. Normally, it would
 **					just decrement reference count for the element. In case 
 **					that the element has its remove flag set, this function 
 **					frees the memory too.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashRelease
#ifdef ANSI_PROTO
(Sdf_st_hash *pHash, Sdf_ty_pvoid pKey)
#else
(pHash, pKey)
Sdf_st_hash *pHash;
Sdf_ty_pvoid pKey;
#endif
{
	Sdf_ty_u32bit hashKey=0;
	Sdf_ty_u32bit bucket=0;
	Sdf_st_hashElement *pTempElement = Sdf_co_null;
	Sdf_st_hashElement **ppIterator = Sdf_co_null;
	Sdf_st_error dError;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaHashRelease");

#ifdef SDF_PARAMVALIDATION
	if ((pKey == Sdf_co_null) || (pHash == Sdf_co_null))
		return Sdf_co_fail;
#endif	/* End of param validation ifdef */

	/* Compute hash for the element */
	hashKey = pHash->fpHashFunc(pKey);

	/* Locate the bucket */
	bucket = hashKey % pHash->numberOfBuckets;

#ifdef SDF_THREAD_SAFE
	/* Grab lock for the bucket in which 
	   the operation is to be done */
	sdf_fn_uaLockSyncObject(0, &(pHash->pBucketSyncObject[bucket]), 0);
#endif
	
	/* Go through chain */
	ppIterator = &(pHash->ppHashChains[bucket]);
	while(*ppIterator != Sdf_co_null)
	{
		if(pHash->fpCompareFunc((*ppIterator)->pKey, pKey) == 0)
			break;
		ppIterator = &((*ppIterator)->pNext);
	}
	if(*ppIterator == Sdf_co_null)
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif
		return Sdf_co_fail;
	}

	/* decrement refcount for the hash element */
	HSS_LOCKREF((*ppIterator)->dRefCount);
	HSS_DECREF((*ppIterator)->dRefCount);
	HSS_DECREF((*ppIterator)->dRefCount);

	/* 	check if the remove flag has been set 
		before the "check in" of the element. Also
		free the memory if the refCount has already
		become zero*/
	if (((*ppIterator)->dRemove == Sdf_co_true)&& \
		(HSS_CHECKREF((*ppIterator)->dRefCount)))
	{
		pTempElement = *ppIterator;
		*ppIterator = (*ppIterator)->pNext;
		if (pHash->fpElementFreeFunc != Sdf_co_null)
			pHash->fpElementFreeFunc(pTempElement->pElement);
		if (pHash->fpKeyFreeFunc != Sdf_co_null)
			pHash->fpKeyFreeFunc(pTempElement->pKey);
		HSS_UNLOCKREF(pTempElement->dRefCount);
		HSS_DELETEREF(pTempElement->dRefCount);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid *)&pTempElement,&dError);
#ifdef SDF_THREAD_SAFE
    	/* Hash-table level locking */
	    sdf_fn_uaLockSyncObject(0, &(pHash->dHashSyncObject), 0);
#endif
		pHash->numberOfElements--;
#ifdef SDF_THREAD_SAFE
    	/* Hash-table level unlocking */
	    sdf_fn_uaUnlockSyncObject(0, &(pHash->dHashSyncObject));
#endif
	}
	else
	{
		HSS_INCREF((*ppIterator)->dRefCount);
		HSS_UNLOCKREF((*ppIterator)->dRefCount);
	}
#ifdef SDF_THREAD_SAFE
	/* Unlock the mutex for the bucket now */
	sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaHashRelease");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashForeach
 ** 
 ** DESCRIPTION: 	This is the function to iterate through all the elements 
 **					in the hash table. Passed function is invoked for each key
 **					and element in the list.
 **
 **					Note:
 **					----
 **					* Function must return 1 if iteration must continue to 
 **					  the next element.
 **					* Returning 0 stops iterations.
 **
 ******************************************************************************/
void sdf_ivk_uaHashForeach 
#ifdef ANSI_PROTO
	(Sdf_st_hash *pHash, Sdf_ty_uaHashIteratorFunc fpIteratorFunc)
#else
	(pHash, fpIteratorFunc)
	Sdf_st_hash *pHash;
	Sdf_ty_uaHashIteratorFunc fpIteratorFunc;
#endif
{
	Sdf_ty_u32bit dIndex=0;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaHashForeach");

#ifdef SDF_PARAMVALIDATION
	if (pHash == Sdf_co_null)
		return;
#endif	/* End of param validation ifdef */

	/* Iterate through all buckets */
	for(dIndex = 0; dIndex < pHash->numberOfBuckets; dIndex++)
	{
		Sdf_st_hashElement *pIterator;
#ifdef SDF_THREAD_SAFE
		/* 	Grab lock for the bucket in which 
	  		the operation is to be done */
		sdf_fn_uaLockSyncObject(0, &(pHash->pBucketSyncObject[dIndex]), 0);
#endif

		/* Iterate through elements in the bucket */
		pIterator=pHash->ppHashChains[dIndex];
		while(pIterator != Sdf_co_null)
		{
			if(fpIteratorFunc(pIterator->pKey, \
				pIterator->pElement) == 0)
			{
				sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
					"sdf_ivk_uaHashForeach");
#ifdef SDF_THREAD_SAFE
				sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[dIndex]));
#endif
				return;
			}
			pIterator = pIterator->pNext;
		}

#ifdef SDF_THREAD_SAFE
		/* Unlock the mutex for the bucket now */
		sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[dIndex]));
#endif
	}
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaHashForeach");
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashNext
 ** 
 ** DESCRIPTION: 	This is the function to get the element
 **					next to an iterator in the hash table
 **
 ******************************************************************************/
void sdf_ivk_uaHashNext 
#ifdef ANSI_PROTO
(Sdf_st_hash *pHash, Sdf_st_hashIterator *pIterator)
#else
(pHash, pIterator)
Sdf_st_hash *pHash;
Sdf_st_hashIterator *pIterator;
#endif
{
	Sdf_st_hashElement *nextElem = Sdf_co_null, *pTempElement = Sdf_co_null;
	Sdf_st_hashElement **ppElement = Sdf_co_null;
	Sdf_ty_pvoid pKey = Sdf_co_null;
	Sdf_st_error dError;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaHashNext");

#ifdef SDF_PARAMVALIDATION
	if ((pHash == Sdf_co_null) || (pIterator == Sdf_co_null))
		return;
#endif	/* End of param validation ifdef */

#ifdef SDF_THREAD_SAFE
	/* Grab mutex for the bucket in which the iterator is currently */
	sdf_fn_uaLockSyncObject(0, &(pHash->\
		pBucketSyncObject[pIterator->currentBucket]), 0);
#endif
	
	/* 	If current element in the iterator points to a 
		null node - keep it null else make it point to 
		next element in the chain. */
	nextElem = (pIterator->pCurrentElement == Sdf_co_null)\
		? Sdf_co_null : pIterator->pCurrentElement->pNext;

	/* 
	 * Check if the current element has to be removed.
	 * If so, do it here before fetching the next element
	 */
	if (pIterator->pCurrentElement != Sdf_co_null)
	{
		HSS_LOCKREF(pIterator->pCurrentElement->dRefCount);
		HSS_DECREF(pIterator->pCurrentElement->dRefCount);
		HSS_DECREF(pIterator->pCurrentElement->dRefCount);

		if (HSS_CHECKREF(pIterator->pCurrentElement->dRefCount) && \
			(pIterator->pCurrentElement->dRemove == Sdf_co_true))
		{
			pKey = pIterator->pCurrentElement->pKey;
			ppElement = &(pHash->ppHashChains[pIterator->currentBucket]);
   	 
			while(*ppElement != Sdf_co_null)
			{
				if(pHash->fpCompareFunc((*ppElement)->pKey, pKey) == 0)
					break;
				ppElement = &((*ppElement)->pNext);
			}
			if(*ppElement != Sdf_co_null)
			{
				pTempElement = *ppElement;
				*ppElement = (*ppElement)->pNext;
				if (pHash->fpElementFreeFunc != Sdf_co_null)
					pHash->fpElementFreeFunc(pTempElement->pElement);
				if (pHash->fpKeyFreeFunc != Sdf_co_null)
					pHash->fpKeyFreeFunc(pTempElement->pKey);
				HSS_UNLOCKREF(pTempElement->dRefCount);	
				HSS_DELETEREF(pTempElement->dRefCount);
				(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid *)&pTempElement, \
					&dError);
#ifdef SDF_THREAD_SAFE
	            /* Hash-table level locking */
            	sdf_fn_uaLockSyncObject(0, &(pHash->dHashSyncObject), 0);
#endif
				pHash->numberOfElements--;
#ifdef SDF_THREAD_SAFE
            	/* Hash-table level unlocking */
            	sdf_fn_uaUnlockSyncObject(0, &(pHash->dHashSyncObject));
#endif
			}
		}
		else
		{
			HSS_INCREF(pIterator->pCurrentElement->dRefCount);
			HSS_UNLOCKREF(pIterator->pCurrentElement->dRefCount);
		}
	}

	if (nextElem != Sdf_co_null)
	{
		/* 	Still in the middle of iteration through a chain. 
			We have already taken the next element. 
			Return now */
		pIterator->pCurrentElement = nextElem;
		HSS_LOCKEDINCREF(pIterator->pCurrentElement->dRefCount);
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockSyncObject(0, &(pHash->\
			pBucketSyncObject[pIterator->currentBucket]));
#endif
		return ;
	}
	
	/*	Since the next element is Null, we have reached the end
		of the chain. Check if its the end of the last chain.
		If so, return */
	if((nextElem == Sdf_co_null) &&\
		(pIterator->currentBucket == pHash->numberOfBuckets-1))
	{
		pIterator->pCurrentElement = nextElem;
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockSyncObject(0, &(pHash->\
			pBucketSyncObject[pIterator->currentBucket]));
#endif
		return;
	}
	
#ifdef SDF_THREAD_SAFE
	/* Unlock the current bucket now */
	sdf_fn_uaUnlockSyncObject(0, &(pHash->\
		pBucketSyncObject[pIterator->currentBucket]));
#endif

	/* 	Find the next non-empty chain and 
		make the iterator point to that */
	pIterator->currentBucket++;
	while (pIterator->currentBucket != pHash->numberOfBuckets-1)
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockSyncObject(0, &(pHash->\
			pBucketSyncObject[pIterator->currentBucket]), 0);
#endif
			
		if (pHash->ppHashChains[pIterator->currentBucket]!=Sdf_co_null)
			break;

#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockSyncObject(0, &(pHash->\
			pBucketSyncObject[pIterator->currentBucket]));
#endif

		pIterator->currentBucket++;
	}

	/* If its the final bucket grab the lock now */
	if (pIterator->currentBucket == pHash->numberOfBuckets-1)
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockSyncObject(0, &(pHash->\
			pBucketSyncObject[pIterator->currentBucket]), 0);
#endif
	}
	
	pIterator->pCurrentElement = \
		pHash->ppHashChains[pIterator->currentBucket];
	
	/* 	Increment reference count of the element being 
		returned unless we reached the end of the final 
		bucket */
	if (pIterator->pCurrentElement != Sdf_co_null)
		HSS_LOCKEDINCREF(pIterator->pCurrentElement->dRefCount);

#ifdef SDF_THREAD_SAFE
	/* Free the lock that was grabbed above */
	sdf_fn_uaUnlockSyncObject(0, &(pHash->\
		pBucketSyncObject[pIterator->currentBucket]));
#endif
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaHashNext");
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashInitIterator
 **
 ** DESCRIPTION:	Sets the iterator to the first element of the hashtable
 **
 ******************************************************************************/
void sdf_ivk_uaHashInitIterator
#ifdef ANSI_PROTO
(Sdf_st_hash *pHash, Sdf_st_hashIterator *pIterator)
#else
(pHash, pIterator)
Sdf_st_hash *pHash;
Sdf_st_hashIterator *pIterator;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaHashInitIterator");

#ifdef SDF_PARAMVALIDATION
	if ((pHash == Sdf_co_null) || (pIterator == Sdf_co_null))
		return;
#endif	/* End of param validation ifdef */

	pIterator->currentBucket = 0;
	while(pIterator->currentBucket <= pHash->numberOfBuckets-1)
	{
#ifdef SDF_THREAD_SAFE
		/* Grab mutex for the bucket in which the iterator is currently */
		sdf_fn_uaLockSyncObject(0, &(pHash->\
			pBucketSyncObject[pIterator->currentBucket]), 0);
#endif
		if(pHash->ppHashChains[pIterator->currentBucket] == Sdf_co_null)
		{
#ifdef SDF_THREAD_SAFE
			/* Unlock the current bucket now */
			sdf_fn_uaUnlockSyncObject(0, &(pHash->\
				pBucketSyncObject[pIterator->currentBucket]));
#endif
			pIterator->currentBucket++;
			continue;
		}
		else
		{
			pIterator->pCurrentElement = pHash->ppHashChains\
				[pIterator->currentBucket];
			HSS_LOCKEDINCREF(pIterator->pCurrentElement->dRefCount);
#ifdef SDF_THREAD_SAFE
			/* Unlock the current bucket now */
			sdf_fn_uaUnlockSyncObject(0, &(pHash->\
				pBucketSyncObject[pIterator->currentBucket]));
#endif
			return;
		}
	}
	pIterator->currentBucket--;
	pIterator->pCurrentElement = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaHashInitIterator");
}	


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaElfHash
 **
 ** DESCRIPTION: 	ELF hash function 
 ** 				Algorithm from Dr. Dobb's Journal
 **
 ** http://www.ddj.com/articles/1996/9604/9604b/9604b.htm?topic=algorithms
 **
 ******************************************************************************/
Sdf_ty_u32bit sdf_ivk_uaElfHash 
#ifdef ANSI_PROTO
(Sdf_ty_pvoid pName)
#else
(pName)
Sdf_ty_pvoid pName;
#endif
{
	Sdf_ty_u32bit h = 0, g=0;
	char *name = Sdf_co_null;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaElfHash");
	if (pName == Sdf_co_null)
		return (Sdf_ty_u32bit)0;
	
	name = (char *)pName;
	while ( *name ) 
	{ 
		h = ( h << 4 ) + *name++; 
		if ( (g = h & 0xF0000000) ) 
			h ^= g >> 24; 
		h &= ~g; 
	}

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaElfHash");
	return h; 
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashExclFetch
 **
 ** DESCRIPTION:	If the element is present, this function fetches the 
 **					element and blocks all further access to the element
 **					(with the given key) till an explicit release is carried
 **					out. This gives the invocation point exclusive access
 **					to the data that is fetched from the hash. 
 **
 **					If no data is found for the key passed and the option
 **					is not passed as Sdf_co_createEntry, the function 
 **					doesn't grab any locks and returns Sdf_co_null.
 **
 **					If no data is found for the key passed and the option
 **					is passed as Sdf_co_createEntry, the function 
 **					creates a new hash entry and stores the data passed as
 **					input in the hash.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashExclFetch
#ifdef ANSI_PROTO
	(Sdf_st_hash	*pHash, 
	 Sdf_ty_pvoid	 pKey, 
	 Sdf_ty_pvoid	*ppData,
	 Sdf_ty_s8bit	 dOption)
#else
	(pHash, pKey, ppData, dOption)
	Sdf_st_hash		*pHash;
	Sdf_ty_pvoid	 pKey;
	Sdf_ty_pvoid	*ppData;
	Sdf_ty_s8bit	 dOption;
#endif
{
	Sdf_ty_u32bit hashKey=0;
	Sdf_ty_u32bit bucket=0;
	Sdf_st_hashElement* pIterator	= Sdf_co_null;
	Sdf_st_hashElement* pNewElement = Sdf_co_null;
	Sdf_st_error dError;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaHashFetchExclusive");

#ifdef SDF_PARAMVALIDATION
	if ((pKey == Sdf_co_null) || (pHash == Sdf_co_null))
		return Sdf_co_fail;

#endif	/* End of param validation ifdef */

	/* Compute hash for the element */
	hashKey = pHash->fpHashFunc(pKey);
	
	/* Locate the bucket */
	bucket = hashKey % pHash->numberOfBuckets;
	
#ifdef SDF_THREAD_SAFE
	/* Grab lock for the bucket in which the operation is to be done. */
	sdf_fn_uaLockSyncObject(0, &(pHash->pBucketSyncObject[bucket]), 0);
#endif

	/* Go through chain */
	pIterator = pHash->ppHashChains[bucket];
	
	while(pIterator != Sdf_co_null)
	{
		if(pHash->fpCompareFunc(pIterator->pKey, pKey) == 0)
		{
			/* Increment reference count */
			HSS_LOCKEDINCREF(pIterator->dRefCount);
			break;
		}
		pIterator = pIterator->pNext;
	}
	
	if (pIterator == Sdf_co_null)
	{
		if (dOption == Sdf_co_createEntry)
		{
			/* Allocate and initialize element holder */
			pNewElement = (Sdf_st_hashElement *)\
				sdf_memget(Sdf_mc_initMemId, sizeof(Sdf_st_hashElement), \
				&dError);
			
			if (pNewElement == Sdf_co_null)
			{
#ifdef SDF_THREAD_SAFE
				/* Unlock the mutex for the bucket before returning */
				sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif
				return Sdf_co_fail;
			}

			pNewElement->pElement = *ppData;
			pNewElement->pKey = pKey;
			pNewElement->dRemove = Sdf_co_false;
			HSS_INITREF(pNewElement->dRefCount);
			HSS_LOCKEDINCREF(pNewElement->dRefCount);

			/* Push element into the bucket */
			pNewElement->pNext = pHash->ppHashChains[bucket];
			pHash->ppHashChains[bucket] = pNewElement;
#ifdef SDF_THREAD_SAFE
        	/* Hash-table level locking */
        	sdf_fn_uaLockSyncObject(0, &(pHash->dHashSyncObject), 0);
#endif
			pHash->numberOfElements++;
#ifdef SDF_THREAD_SAFE
        	/* Hash-table level unlocking */
	        sdf_fn_uaUnlockSyncObject(0, &(pHash->dHashSyncObject));
#endif

			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_ivk_uaHashFetchExclusive");

			return Sdf_co_success;
		}
		else
		{
#ifdef SDF_THREAD_SAFE
			/* Unlock the mutex for the bucket now */
			sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif
			*ppData = Sdf_co_null;

			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_ivk_uaHashFetchExclusive");

		 	return Sdf_co_fail;
		}
	} /* End of "if iterator == Sdf_co_null" */
	else
	{
		*ppData = pIterator->pElement;
			
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_ivk_uaHashFetchExclusive");

		return Sdf_co_success;
	}

} /* End of function */


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashExclRelease
 **
 ** DESCRIPTION:	
 **
 ******************************************************************************/
void sdf_ivk_uaHashExclRelease
#ifdef ANSI_PROTO
	(Sdf_st_hash	*pHash, 
	 Sdf_ty_pvoid	 pKey,
	 Sdf_ty_s8bit	 dOption, 
	 Sdf_ty_pvoid	*ppOrigKey,
	 Sdf_ty_pvoid	*ppOrigElem)
#else
	(pHash, pKey, dOption, ppOrigKey, ppOrigElem)
	 Sdf_st_hash	*pHash;
	 Sdf_ty_pvoid	 pKey;
	 Sdf_ty_s8bit	 dOption;
	 Sdf_ty_pvoid	*ppOrigKey;
	 Sdf_ty_pvoid	*ppOrigElem;
#endif
{
	Sdf_ty_u32bit hashKey=0;
	Sdf_ty_u32bit bucket=0;
	Sdf_st_hashElement	 *pTempElement	= Sdf_co_null;
	Sdf_st_hashElement	**ppIterator	= Sdf_co_null;
	Sdf_st_error dError;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaHashExclRelease");

#ifdef SDF_PARAMVALIDATION
	if ((pKey == Sdf_co_null) || (pHash == Sdf_co_null))
		return;
#endif	/* End of param validation ifdef */

	/* Compute hash for the element */
	hashKey = pHash->fpHashFunc(pKey);

	/* Locate the bucket */
	bucket = hashKey % pHash->numberOfBuckets;

	/* 
	 * NOTE :	Lock is not grabbed here assuming that it was grabbed
	 *			earlier using sdf_ivk_uaHashExclFetch()
	 */

	/* Go through chain */
	ppIterator = &(pHash->ppHashChains[bucket]);

	while (*ppIterator != Sdf_co_null)
	{
		if(pHash->fpCompareFunc((*ppIterator)->pKey, pKey) == 0)
		{
			HSS_LOCKEDDECREF((*ppIterator)->dRefCount);
			break;
		}
		ppIterator = &((*ppIterator)->pNext);
	}

	if (*ppIterator == Sdf_co_null)
	{
		/* Freak case. Could not find any entry with the key provided */
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif
		*ppOrigElem	= Sdf_co_null;

		if(Sdf_co_null != ppOrigKey)
		   *ppOrigKey	= Sdf_co_null;
		return;
	}

	/*
	 * If option set to remove entry, do it now.
	 */
	if (dOption == Sdf_co_removeEntry)
	{
		HSS_LOCKREF((*ppIterator)->dRefCount);
		HSS_DECREF((*ppIterator)->dRefCount);

		if (HSS_CHECKREF((*ppIterator)->dRefCount))
		{
			pTempElement = *ppIterator;
			*ppIterator = (*ppIterator)->pNext;

			*ppOrigElem = (pTempElement->pElement);
			if(Sdf_co_null != ppOrigKey)
			     *ppOrigKey	= (pTempElement->pKey);

			HSS_UNLOCKREF(pTempElement->dRefCount);
			HSS_DELETEREF(pTempElement->dRefCount);
			(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid *)&pTempElement, \
				&dError);
#ifdef SDF_THREAD_SAFE
        	/* Hash-table level locking */
        	sdf_fn_uaLockSyncObject(0, &(pHash->dHashSyncObject), 0);
#endif
			pHash->numberOfElements--;
#ifdef SDF_THREAD_SAFE
        	/* Hash-table level unlocking */
        	sdf_fn_uaUnlockSyncObject(0, &(pHash->dHashSyncObject));
#endif
		}
		else
		{
			(*ppIterator)->dRemove = Sdf_co_true;
			HSS_INCREF((*ppIterator)->dRefCount);
			HSS_UNLOCKREF((*ppIterator)->dRefCount);
		}
	}

	/*
	 * Unlock the bucket for others to get access to the element
	 * if needed.
	 */
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockSyncObject(0, &(pHash->pBucketSyncObject[bucket]));
#endif

	return;	
}
