/* Ensure Names are not mangled by C++ compilers */

#ifndef __SDF_HASH_H__
#define __SDF_HASH_H__

/******************************************************************************
 ** FUNCTION:	
 **		Has the hashtable implementation used in the framework.
 ******************************************************************************
 **
 ** FILENAME:		sdf_hash.h
 **
 ** DESCRIPTION:  	This file contains the implementation of hashtable used
 **					within the framework.
 **
 ** DATE     	NAME          REFERENCE     REASON
 ** ----      	----          ---------     ------
 ** 12/04/01	K. Binu, 	  siphash.h		Creation
 **				Siddharth	  
 **
 ** 25/06/02	Siddharth					Addition of exclusive operation
 **											APIs
 **
 ******************************************************************************
 ** 			COPYRIGHT , Aricent, 2006
 ******************************************************************************/


#include "sdf_common.h"
#include "sdf_struct.h"

#define Sdf_co_createEntry		1
#define Sdf_co_removeEntry		2
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashInit
 ** 
 ** DESCRIPTION: 	This is the function to initialize a new 
 **  				hash table.
 **
 ** PARAMETERS:
 **	 pHash 			(IN/OUT)	: Hash table to be initialized.
 **  fpHashFunc 	(IN)		: Function to be used to hash the key.
 **  fpCompareFunc 	(IN)		: Function to compare the hash keys of entries 
 **								  at time of doing a fetch. If the comparison 
 **								  function :
 **								  returns 0 - the keys that were compared match
 **								  returns 1 - the keys don't match
 **  fpElemFreeFunc (IN)		: Function to invoke to free the 
 **								  element data when the hash entry 
 **								  has be deleted.
 **  fpKeyFreeFunc	(IN)		: Function to invoke to free the 
 **								  element key when the hash entry 
 **								  has be deleted.
 **  numBuckets		(IN)		: number of chains in the hash table.
 **  maxElements	(IN)		: maximum number of elements to be allowed 
 **								  in the hash table.
 **	 pErr			(IN/OUT)	: Error variable returned in case of failure.
 ** 
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashInit _ARGS_((Sdf_st_hash *pHash, \
		Sdf_ty_uaHashFunc fpHashFunc,\
		Sdf_ty_uaHashKeyCompareFunc fpCompareFunc, \
		Sdf_ty_uaHashElementFreeFunc fpElemFreeFunc, \
		Sdf_ty_uaHashKeyFreeFunc fpKeyFreeFunc, \
		Sdf_ty_u32bit numBuckets, Sdf_ty_u32bit maxElements, \
		Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashFree
 ** 
 ** DESCRIPTION: 	This is the function to free members from the hash table. 
 **					It does not free the hash elements, but frees other	member 
 **					variables malloced at the time of Init of the hash table
 **
 ** PARAMETERS:
 **	 pHash 	(IN)	: Hash table to be freed.
 **  pErr	(OUT)	: Error variable returned in case of failure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashFree _ARGS_((Sdf_st_hash *pHash, \
					Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashAdd
 ** 
 ** DESCRIPTION: 	This is the function to add an entry into the hash table.
 **
 ** PARAMETERS:
 **	 pHash 	(IN)	: Hash table to which the entry has
 **					  to be added.
 **  pErr	(OUT)	: Error variable returned in case
 **					  of failure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashAdd _ARGS_((Sdf_st_hash *pHash, \
		void *pElement, void* pKey));


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashFetch
 ** 
 ** DESCRIPTION: 	This is the function to fetch an entry from the hash table.
 **					Sdf_co_null is returned in case the hash table does not 
 **					contain any entries corresponding to the key passed.
 **
 ** PARAMETERS:
 **	 pHash 	(IN)	: Hash table from which the entry has to be
 **					  extracted.
 **  pKey	(IN)	: Key corresponding to the element to be 
 **					  fetched from the hash table.
 **
 ******************************************************************************/
void* sdf_ivk_uaHashFetch _ARGS_((Sdf_st_hash *pHash, \
								void *pKey));


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashRelease
 ** 
 ** DESCRIPTION:	This function should be invoked to
 **					"check in" an element that was obtained
 **					from the hash table. Normally, it would
 **					just decrement reference count for the
 **					element. In case that the element has its
 **					remove flag set, this function frees
 **					the memory too.
 **
 ** PARAMETERS:
 **	 pHash 	(IN)	: Hash table from which the entry has to 
 **					  be released.
 **  pKey	(IN)	: Key corresponding to the element to be 
 **					  released.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashRelease _ARGS_((Sdf_st_hash *pHash, \
								void *pKey));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaHashRemove
 ** 
 ** DESCRIPTION: 	This is the function to remove an entry from the hash 
 **					table. If the element is in use at the time of the remove 
 **					request, then it is marked for removal and memory actually
 **					gets freed only when the other usage releases the entry.
 **
 ** PARAMETERS:
 **	 pHash 	(IN)	: Hash table from which the entry has to be released.
 **  pKey	(IN)	: Key corresponding to the element to be removed.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashRemove _ARGS_((Sdf_st_hash *pHash, void *pKey));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaHashForeach
 ** 
 ** DESCRIPTION: 	This is the function to iterate through
 **					all the elements in the hash table.
 **					Passed function is invoked for each key
 **					and element in the list.
 **					
 **					Note:
 **					----
 **					* Function must return 1 if iteration 
 **					  must continue to the next element.
 **					* Returning 0 stops iterations.
 **
 ** PARAMETERS:
 **	 pHash 			(IN)	: Hash table for which all the entries
 **							  have to be traversed.
 **  fpIteratorFunc	(IN)	: Iterator function to be invoked for each entry.
 **
 ******************************************************************************/
void sdf_ivk_uaHashForeach _ARGS_((Sdf_st_hash *pHash, \
				Sdf_ty_uaHashIteratorFunc fpIteratorFunc));


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashNext
 ** 
 ** DESCRIPTION: 	This is the function to get the element next to an 
 **					iterator in the hash table
 **
 ** PARAMETERS:
 **	 pHash 		(IN)		: Hash table from which the next entry
 **							  has to be retrieved.
 **  pIterator	(IN/OUT)	: Hash iterator for which the next 
 **							  element has to be retrieved.
 **
 ******************************************************************************/
void sdf_ivk_uaHashNext _ARGS_((Sdf_st_hash *pHash, \
				Sdf_st_hashIterator *pIterator));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaHashInitIterator
 **
 ** DESCRIPTION:	Sets the iterator to the first element of the hashtable
 **
 ** PARAMETERS:
 **  pIterator	(IN/OUT)	: Hash iterator to be inited.
 **							
 ******************************************************************************/
void sdf_ivk_uaHashInitIterator _ARGS_((Sdf_st_hash *pHash, \
					Sdf_st_hashIterator *pIterator));


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
 **					doesn't grab any locks and returns NULL.
 **
 **					If no data is found for the key passed and the option
 **					is passed as Sdf_co_createEntry, the function 
 **					creates a new hash entry and stores the data passed as
 **					input in the hash.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHashExclFetch _ARGS_((Sdf_st_hash	*pHash, \
	 Sdf_ty_pvoid pKey, Sdf_ty_pvoid *ppData, Sdf_ty_s8bit dOption));


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaHashExclRelease
 **
 ** DESCRIPTION:	
 **
 ******************************************************************************/
void sdf_ivk_uaHashExclRelease _ARGS_((Sdf_st_hash *pHash, \
	 Sdf_ty_pvoid pKey, Sdf_ty_s8bit dOption, Sdf_ty_pvoid *ppOrigKey, \
	 Sdf_ty_pvoid *ppOrigElem));

/* ELF hash function */
Sdf_ty_u32bit sdf_ivk_uaElfHash _ARGS_((void* name));


/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif
#endif
