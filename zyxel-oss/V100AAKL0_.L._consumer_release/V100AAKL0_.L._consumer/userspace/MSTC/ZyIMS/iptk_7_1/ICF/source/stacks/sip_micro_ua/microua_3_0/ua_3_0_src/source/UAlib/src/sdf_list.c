/*****************************************************************************
 ** FUNCTION: 
 **			This file has API's to manipulate the locked list structure.
 **			It is essentially a wrapper around the sip_listXXX API's with 
 **			the addition of a locking mechanism to synchronize access to
 **			the list.
 **
 *****************************************************************************
 **
 ** FILENAME		:	sdf_list.c
 **
 ** DESCRIPTION		:	This file has API's to manipulate the locked list 
 **						structure.It is essentially a wrapper around the 
 **						sip_listXXX API's with the addition of a locking 
 **						mechanism to synchronize access to the list.
 **
 **
 ** DATE        NAME				REFERENCE		REASON
 ** ----        ----				---------		--------
 ** 27/09/01	Subhash								Creation
 **
 *****************************************************************************
 **			COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#include "ALheader.h"
#include "sdf_struct.h"
#ifndef SDF_LINT
#include "sdf_list.h"
#endif
#include "sdf_portlayer.h"


/***********************************************************************
** FUNCTION:  sdf_listInit
**
** DESCRIPTION: Initializes a Sdf_ty_slist. (wrapper sip_listInit)
**
************************************************************************/
Sdf_ty_retVal sdf_listInit 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList, 
sip_listFuncPtr pFreefunc, 
Sdf_ty_bool dThreadSafe,
Sdf_st_error *pError)
#else
(pslList, pFreefunc, dThreadSafe,pError)
Sdf_ty_slist *pslList;
sip_listFuncPtr pFreefunc; 
Sdf_ty_bool dThreadSafe;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;

	dRetVal = sip_listInit((SipList *)pslList, pFreefunc, \
		(SipError *)&(pError->stkErrCode));
    if (dRetVal  == SipFail)
		return Sdf_co_fail;
#ifdef SDF_THREAD_SAFE	
	if(dThreadSafe == Sdf_co_true)
	{
		/*if (dRetVal == SipSuccess)*/
			sdf_fn_uaInitSyncObject(&(pslList->dSyncObject), Sdf_co_false);
	}	
#endif	
	pslList->dThreadSafeList=dThreadSafe;
	/*return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);*/
    return Sdf_co_success;
}

/***********************************************************************
** FUNCTION:  sdf_listFree
**
** DESCRIPTION: Frees a Sdf_ty_slist. It does not free the elements in the 
**				list but frees only the Sdf_ty_slist structure members 
**				allocated by sdf_listInit
**
************************************************************************/
Sdf_ty_retVal sdf_listFree 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
Sdf_st_error *pError)
#else
(pslList, pError)
Sdf_ty_slist *pslList;
Sdf_st_error *pError;
#endif
{
	(void) pError;

#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaDestroySyncObject(&(pslList->dSyncObject));
#else
	(void)pslList;
#endif		

	return Sdf_co_success;
}

/***********************************************************************
** FUNCTION:  sdf_listAppend
**
** DESCRIPTION: Appends the pData passed to the locked list. 
**				(wrapper sip_listAppend)
**
************************************************************************/
Sdf_ty_retVal sdf_listAppend 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList, 
Sdf_ty_pvoid pData,
Sdf_st_error *pError)
#else
(pslList, pData, pError)
Sdf_ty_slist *pslList;
Sdf_ty_pvoid pData;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;
#ifdef SDF_LINT
	if(pslList == Sdf_co_null)		
		return Sdf_co_fail;
#endif	
	
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaLockSyncObject(0, &(pslList->dSyncObject), 0);
#endif		
	dRetVal = sip_listAppend((SipList *)pslList, pData, \
		(SipError *)&(pError->stkErrCode));

#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaUnlockSyncObject(0, &(pslList->dSyncObject));
#endif		
	return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);
}

/***********************************************************************
** FUNCTION:  sdf_listPrepend
**
** DESCRIPTION: Prepends the pData passed to the locked list. 
**				(wrapper sip_listPrepend)
**
************************************************************************/
Sdf_ty_retVal sdf_listPrepend 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList, 
Sdf_ty_pvoid pData,
Sdf_st_error *pError)
#else
(pslList, pData, pError)
Sdf_ty_slist *pslList;
Sdf_ty_pvoid pData;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaLockSyncObject(0, &(pslList->dSyncObject), 0);
#endif		
	dRetVal = sip_listPrepend((SipList *)pslList, pData, \
		(SipError *)&(pError->stkErrCode));

#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaUnlockSyncObject(0, &(pslList->dSyncObject));
#endif		
	return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);
}

/***********************************************************************
** FUNCTION:  sdf_listGetAt
**
** DESCRIPTION: Gets the data at the index specified.
**				(wrapper sip_listGetAt)
**
************************************************************************/
Sdf_ty_retVal sdf_listGetAt 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
Sdf_ty_u32bit dPosition, 
Sdf_ty_pvoid *ppData, 
Sdf_st_error *pError)
#else
(pslList, dPosition, ppData, pError)
Sdf_ty_slist *pslList;
Sdf_ty_u32bit dPosition;
Sdf_ty_pvoid *ppData;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaLockSyncObject(0, &(pslList->dSyncObject), 0);
#endif		
	dRetVal = sip_listGetAt((SipList *)pslList, dPosition, ppData, \
		(SipError *)&(pError->stkErrCode));
		
#ifdef SDF_THREAD_SAFE	
		
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaUnlockSyncObject(0, &(pslList->dSyncObject));
#endif		
	return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);
}


/***********************************************************************
** FUNCTION:  sdf_listSetAt
**
** DESCRIPTION: Sets the data at the index specified.
**				(wrapper sip_listSetAt)
**
************************************************************************/
Sdf_ty_retVal sdf_listSetAt 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
Sdf_ty_u32bit dPosition,
Sdf_ty_pvoid pData,
Sdf_st_error *pError)
#else
(pslList, dPosition, pData, pError)
Sdf_ty_slist *pslList;
Sdf_ty_u32bit dPosition;
Sdf_ty_pvoid pData;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;
	
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaLockSyncObject(0, &(pslList->dSyncObject), 0);
#endif		
	dRetVal = sip_listSetAt((SipList *)pslList, dPosition, pData, \
		(SipError *)&(pError->stkErrCode));
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)	
		sdf_fn_uaUnlockSyncObject(0, &(pslList->dSyncObject));
#endif		
	return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);
}

/***********************************************************************
** FUNCTION:  sdf_listSizeOf
**
** DESCRIPTION: Returns the size of the locked list.
**				(wrapper sip_listSizeOf)
**
************************************************************************/
Sdf_ty_retVal sdf_listSizeOf 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
Sdf_ty_u32bit *pSize, 
Sdf_st_error *pError)
#else
(pslList, pSize, pError)
Sdf_ty_slist *pslList;
Sdf_ty_u32bit *pSize;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;
	
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaLockSyncObject(0, &(pslList->dSyncObject), 0);
#endif		
	dRetVal = sip_listSizeOf((SipList *)pslList, pSize, \
		(SipError *)&(pError->stkErrCode));
		
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)	
		sdf_fn_uaUnlockSyncObject(0, &(pslList->dSyncObject));
#endif		
	return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);
}

/***********************************************************************
** FUNCTION:  sdf_listInsertAt
**
** DESCRIPTION: Inserts an element at the specified index.
**				(wrapper sip_listInsertAt)
**
************************************************************************/
Sdf_ty_retVal sdf_listInsertAt 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
Sdf_ty_u32bit dPosition, 
Sdf_ty_pvoid pData, 
Sdf_st_error *pError)
#else
(pslList, dPosition, pData, pError)
Sdf_ty_slist *pslList;
Sdf_ty_u32bit dPosition;
Sdf_ty_pvoid pData;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;
	
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaLockSyncObject(0, &(pslList->dSyncObject), 0);
#endif		
	dRetVal = sip_listInsertAt((SipList *)pslList, dPosition, pData, \
		(SipError *)&(pError->stkErrCode));

#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaUnlockSyncObject(0, &(pslList->dSyncObject));
#endif		
	return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);
}

/***********************************************************************
** FUNCTION:  sdf_listDeleteAt
**
** DESCRIPTION: Deletes the element at the specified index.
**				(wrapper sip_listDeleteAt)
**
************************************************************************/
Sdf_ty_retVal sdf_listDeleteAt 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
Sdf_ty_u32bit dPosition, 
Sdf_st_error *pError)
#else
(pslList, dPosition, pError)
Sdf_ty_slist *pslList;
Sdf_ty_u32bit dPosition;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;
	
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaLockSyncObject(0, &(pslList->dSyncObject), 0);
#endif		
	dRetVal = sip_listDeleteAt((SipList *)pslList, dPosition, \
		(SipError *)&(pError->stkErrCode));
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaUnlockSyncObject(0, &(pslList->dSyncObject));
#endif		
	return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);
}

/***********************************************************************
** FUNCTION:  sdf_listForEach
**
** DESCRIPTION: Iterates through the list and invokes the function passed
**				to it on every element in the list.
**				(wrapper sip_listForEach)
**
************************************************************************/
Sdf_ty_retVal sdf_listForEach 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
sip_listFuncPtr pFunc, 
Sdf_st_error *pError)
#else
(pslList, pFunc, pError)
Sdf_ty_slist *pslList;
sip_listFuncPtr pFunc;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;
	
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaLockSyncObject(0, &(pslList->dSyncObject), 0);
#endif		
	dRetVal = sip_listForEach((SipList *)pslList, pFunc, \
		(SipError *)&(pError->stkErrCode));
		
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaUnlockSyncObject(0, &(pslList->dSyncObject));
#endif		
	return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);
}

/***********************************************************************
** FUNCTION:  sdf_listDeleteAll
**
** DESCRIPTION: Removes all the elements from the list.
**				(wrapper sip_listDeleteAll)
**
************************************************************************/
Sdf_ty_retVal sdf_listDeleteAll 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
Sdf_st_error *pError)
#else
(pslList, pError)
Sdf_ty_slist *pslList;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;
	
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaLockSyncObject(0, &(pslList->dSyncObject), 0);
#endif		
	dRetVal = sip_listDeleteAll((SipList *)pslList, \
		(SipError *)&(pError->stkErrCode));

#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaUnlockSyncObject(0, &(pslList->dSyncObject));
#endif		
	return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);
}


/***********************************************************************
** FUNCTION:  sdf_listForEachWithData
**
** DESCRIPTION: This function iterates through the list and invokes 
**				the function passed to it on every element in the list.
**				(wrapper sip_listForEachWithData)
**
************************************************************************/
Sdf_ty_retVal sdf_listForEachWithData 
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
sip_listFuncPtrWithData pFunc, 
Sdf_ty_pvoid *ppData, 
Sdf_st_error *pError)
#else
(pslList, pFunc, ppData, pError)
Sdf_ty_slist *pslList;
sip_listFuncPtrWithData pFunc;
Sdf_ty_pvoid *ppData;
Sdf_st_error *pError;
#endif
{
	SipBool dRetVal;
	
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaLockSyncObject(0, &(pslList->dSyncObject), 0);
#endif		
	dRetVal = sip_listForEachWithData((SipList *)pslList, pFunc, \
		ppData, (SipError *)&(pError->stkErrCode));
		
#ifdef SDF_THREAD_SAFE	
	if(pslList->dThreadSafeList== Sdf_co_true)
		sdf_fn_uaUnlockSyncObject(0, &(pslList->dSyncObject));
#endif		
	return (dRetVal == SipSuccess ? Sdf_co_success : Sdf_co_fail);
}

#ifndef PERF_OPTIMIZATION
/***********************************************************************
** FUNCTION:  sdf_listInitIterator
**
** DESCRIPTION: Sets the iterator to the first element of the hashtable
**
************************************************************************/
Sdf_ty_retVal sdf_listInitIterator
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
Sdf_st_listIterator *pListIterator,
Sdf_st_error *pError)
#else
(pslList, pListIterator, pError)
Sdf_ty_slist *pslList;
Sdf_st_listIterator *pListIterator;
Sdf_st_error *pError;
#endif
{
	(void) pError;

	pListIterator->pCurrentElement = pslList->head;

	return Sdf_co_success;
}

/***********************************************************************
** FUNCTION:  sdf_listNext
**
** DESCRIPTION: This function gets the element next to the iterator in
**				the list
**
************************************************************************/
Sdf_ty_retVal sdf_listNext
#ifdef ANSI_PROTO
(Sdf_st_listIterator *pListIterator,
Sdf_st_error *pError)
#else
(pListIterator, pError)
Sdf_st_listIterator *pListIterator;
Sdf_st_error *pError;
#endif
{
	(void) pError;

	pListIterator->pCurrentElement = pListIterator->pCurrentElement->next;

	return Sdf_co_success;
}

#else

/***********************************************************************
** FUNCTION:  sdf_listInitIterator
**
** DESCRIPTION: Sets the iterator to the first element of the hashtable
**
************************************************************************/
Sdf_ty_retVal sdf_listInitIterator
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslList,
Sdf_st_listIterator *pListIterator,
Sdf_st_error *pError)
#else
(pslList, pListIterator, pError)
Sdf_ty_slist *pslList;
Sdf_st_listIterator *pListIterator;
Sdf_st_error *pError;
#endif
{
	(void) pError;

	if(pslList->size == 0 )
		pListIterator->	pCurrentElement = NULL;
	else
	{

	pListIterator->pCurrentElement=&(pListIterator->temp);
	pListIterator->pCurrentElement->pData=pslList->pData[0];
	pListIterator->list=pslList;
	pListIterator->cur=0;
	}
	

	return Sdf_co_success;
}

/***********************************************************************
** FUNCTION:  sdf_listNext
**
** DESCRIPTION: This function gets the element next to the iterator in
**				the list
**
************************************************************************/
Sdf_ty_retVal sdf_listNext
#ifdef ANSI_PROTO
(Sdf_st_listIterator *pListIterator,
Sdf_st_error *pError)
#else
(pListIterator, pError)
Sdf_st_listIterator *pListIterator;
Sdf_st_error *pError;
#endif
{
	(void) pError;

	if (pListIterator->cur >= (pListIterator->list->size-1))
		pListIterator->pCurrentElement=NULL;
	else
	{

		pListIterator->pCurrentElement=&(pListIterator->temp);
		pListIterator->cur=pListIterator->cur+1 ;
		pListIterator->pCurrentElement->pData=pListIterator->list->pData[pListIterator->cur];/* ++ added in front of the statement */
	}
	return Sdf_co_success;
}

#endif

