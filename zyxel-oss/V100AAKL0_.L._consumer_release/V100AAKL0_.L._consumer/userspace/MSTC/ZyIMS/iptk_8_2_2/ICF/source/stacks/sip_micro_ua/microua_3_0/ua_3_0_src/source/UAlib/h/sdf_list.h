/***********************************************************************
 ** FUNCTION: 
 **			This file has API's to manipulate the locked list structure.
 **			It is essentially a wrapper around the sip_listXXX API's with 
 **			the addition of a locking mechanism to synchronize access to
 **			the list.
 **
 *********************************************************************
 **
 ** FILENAME		: sdf_list.h
 **
 ** DESCRIPTION		: This file has API's to manipulate the locked list 
 **						structure.It is essentially a wrapper around the 
 **						sip_listXXX API's with the addition of a locking 
 **						mechanism to synchronize access to the list.
 **
 **
 ** DATE        NAME				REFERENCE		REASON
 ** ----        ----				---------		--------
 ** 27/09/01	Subhash								Creation
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/

#ifndef __SDF_UALIST_H_
#define __SDF_UALIST_H_

#include "microsip_list.h"
#ifdef PERF_OPTIMIZATION

typedef struct
{
 Sdf_ty_u32bit size;
 Sdf_ty_u32bit MAX;
 Sdf_ty_pvoid *pData;
 Sdf_ty_pvoid temp[MAX_LIST_ELEMENTS];
 void  (*freefunc)(Sdf_ty_pvoid );
#ifdef SDF_THREAD_SAFE	
	/* Mutex to allow synchronized access to the list */
	Sdf_ty_syncIdT dSyncObject;
#endif	
	
	/* Boolean indicating whether the list is thread safe or not */
	Sdf_ty_bool dThreadSafeList;
} Sdf_ty_slist;

#else
/* Thread safe singly linked list */
typedef struct
{
	Sdf_ty_u32bit size;
 	SipListElement *head;
	SipListElement *tail;
	void (*freefunc)(Sdf_ty_pvoid);
 	
#ifdef SDF_THREAD_SAFE	
	/* Mutex to allow synchronized access to the list */
	Sdf_ty_syncIdT dSyncObject;
#endif	
	
	/* Boolean indicating whether the list is thread safe or not */
	Sdf_ty_bool dThreadSafeList;

} Sdf_ty_slist;

#endif
/*
 * Iterator needed to operate on all the list elements. User will need to
 * do sdf_ivk_uaListNext() to get the next element from the list.
 */
#ifdef PERF_OPTIMIZATION 

typedef struct
{
	SipListElement *pCurrentElement;
	SipListElement temp;
	Sdf_ty_slist *list;
	Sdf_ty_u32bit cur;

} Sdf_st_listIterator;

#else
typedef struct
{
	SipListElement *pCurrentElement;

} Sdf_st_listIterator;
#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 
/***********************************************************************
** FUNCTION:  sdf_listInit
**
** DESCRIPTION: Initializes a Sdf_ty_slist. (wrapper sip_listInit)
**
************************************************************************/
Sdf_ty_retVal sdf_listInit _ARGS_ ((Sdf_ty_slist *pList, \
	sip_listFuncPtr pFreefunc,Sdf_ty_bool dThreadSafe,Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listFree
**
** DESCRIPTION: Frees a Sdf_ty_slist. It does not free the elements in the 
**				list but frees only the Sdf_ty_slist structure members 
**				allocated by sdf_listInit
**
************************************************************************/
Sdf_ty_retVal sdf_listFree _ARGS_ ((Sdf_ty_slist *pList, \
	Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listAppend
**
** DESCRIPTION: Appends the pData passed to the locked list. 
**				(wrapper sip_listAppend)
**
************************************************************************/
Sdf_ty_retVal sdf_listAppend _ARGS_ ((Sdf_ty_slist *pList, Sdf_ty_pvoid pData, \
	Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listPrepend
**
** DESCRIPTION: Prepends the pData passed to the locked list. 
**				(wrapper sip_listPrepend)
**
************************************************************************/
Sdf_ty_retVal sdf_listPrepend _ARGS_ ((Sdf_ty_slist *pList, \
	Sdf_ty_pvoid pData, Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listGetAt
**
** DESCRIPTION: Gets the data at the index specified.
**				(wrapper sip_listGetAt)
**
************************************************************************/
Sdf_ty_retVal sdf_listGetAt _ARGS_ ((Sdf_ty_slist *pList, \
	Sdf_ty_u32bit dPosition, Sdf_ty_pvoid *ppData, Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listSetAt
**
** DESCRIPTION: Sets the data at the index specified.
**				(wrapper sip_listSetAt)
**
************************************************************************/
Sdf_ty_retVal sdf_listSetAt _ARGS_ ((Sdf_ty_slist *pList, \
	Sdf_ty_u32bit dPosition, Sdf_ty_pvoid pData, Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listSizeOf
**
** DESCRIPTION: Returns the size of the locked list.
**				(wrapper sip_listSizeOf)
**
************************************************************************/
Sdf_ty_retVal sdf_listSizeOf _ARGS_ ((Sdf_ty_slist *pList, \
	Sdf_ty_u32bit *pSize, Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listInsertAt
**
** DESCRIPTION: Inserts an element at the specified index.
**				(wrapper sip_listInsertAt)
**
************************************************************************/
Sdf_ty_retVal sdf_listInsertAt _ARGS_ ((Sdf_ty_slist *pList, \
	Sdf_ty_u32bit dPosition, Sdf_ty_pvoid pData, Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listDeleteAt
**
** DESCRIPTION: Deletes the element at the specified index.
**				(wrapper sip_listDeleteAt)
**
************************************************************************/
Sdf_ty_retVal sdf_listDeleteAt _ARGS_ ((Sdf_ty_slist *pList, \
	Sdf_ty_u32bit dPosition, Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listForEach
**
** DESCRIPTION: Iterates through the list and invokes the function passed
**				to it on every element in the list.
**				(wrapper sip_listForEach)
**
************************************************************************/
Sdf_ty_retVal sdf_listForEach _ARGS_ ((Sdf_ty_slist *pList, \
	sip_listFuncPtr pFunc, Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listDeleteAll
**
** DESCRIPTION: Removes all the elements from the list.
**				(wrapper sip_listDeleteAll)
**
************************************************************************/
Sdf_ty_retVal sdf_listDeleteAll _ARGS_ ((Sdf_ty_slist *pList,  \
	Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_listForEachWithData
**
** DESCRIPTION: This function iterates through the list and invokes 
**				the function passed to it on every element in the list.
**				(wrapper sip_listForEachWithData)
**
************************************************************************/
Sdf_ty_retVal sdf_listForEachWithData _ARGS_ ((Sdf_ty_slist *pList,\
	sip_listFuncPtrWithData pFunc, Sdf_ty_pvoid *ppData, Sdf_st_error *pError));


/***********************************************************************
** FUNCTION:  sdf_listInitIterator
**
** DESCRIPTION: Sets the iterator to the first element of the hashtable
**
************************************************************************/
Sdf_ty_retVal sdf_listInitIterator _ARGS_ ((Sdf_ty_slist *pslList, \
	Sdf_st_listIterator *pListIterator, Sdf_st_error *pError));


/***********************************************************************
** FUNCTION:  sdf_listNext
**
** DESCRIPTION: This function gets the element next to the iterator in
**				the list
**
************************************************************************/
Sdf_ty_retVal sdf_listNext _ARGS_ ((Sdf_st_listIterator *pListIterator, \
	Sdf_st_error *pError));


/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
#endif
