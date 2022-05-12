/******************************************************************************
 ** FUNCTION:
 **	 This file contains the implementations of the functions used to 
 **	 manipulate SipList structures. 
 ******************************************************************************
 **
 ** FILENAME:
 ** siplist.c
 **
 ** DESCRIPTION:
 **	This file implements all functions required to perform operations
 **     on the SipLists	
 **	
 **
 ** DATE	   NAME			REFERENCE		REASON
 ** ----	   ----			--------		------
 ** 20/03/02	   Mahesh				--			Original
 ** 05-Jun-2009   Anuradha Gupta         SPR 19737       Klocworks Warning Removal
 **
 **	COPYRIGHT , Aricent, 2009
 ******************************************************************************/

#include "microsip_portlayer.h"
#include "microsip_list.h"



/*******************************************************************************
** FUNCTION: sip_listInit
** DESCRIPTION: This function initializes the list structure passed to it.
** It sets the head and tail pointers to SIP_NULL and the size to zero.
** This funct also set the free function pointer in the list structure.
** Needs to be called before any other operations are performed on a list.
** Need not be called after DeleteAll before reusing the deleted list.
** 
** PARAMETERS:
**      	      *list(IN)			:The List which has to be initialized
**				  freefunc(IN)		:The function pointer to function which 
**									  frees the list element.
**				  error(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_listInit
#ifdef ANSI_PROTO
	( SipList 		*list,
	  sip_listFuncPtr freefunc,
	  SipError 		*error )
#else
	( list, freefunc, error)
	  SipList 		*list;
	  sip_listFuncPtr freefunc;
	  SipError 		*error;  
#endif
{
	 if(list==SIP_NULL) 
	 {
		  *error = E_NO_EXIST;
		  return SipFail;
	 }
	 list->tail = list->head = SIP_NULL;
	 list->size = 0;
	 /*
	  *The function  to be called when 
	  * when the list is freed
	  */
	 list->freefunc = freefunc;
	 *error = E_NO_ERROR;

	 return SipSuccess;
}

/*******************************************************************************
** FUNCTION: sip_listAppend
** DESCRIPTION:This function appends the pData passed to the siplist. 
** 		The list maintains  a pointer to the pData passed and does not copy the 
**		pData. Hence pData must not be freed after it has been appended to 
**		a siplist.
** 
** PARAMETERS:
**      	      *list(IN)			:The List which a node has to be appended
**				  pDatac(IN)		:The data to be appended
**				  error(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_listAppend
#ifdef ANSI_PROTO
	( SipList 		*list,
	  void 			*pData,
	  SipError 		*error )
#else
	( list, pData, error )
	  SipList 		*list;
	  void 			*pData;
	  SipError 		*error;
#endif
{
	 SipListElement *tmp, *it;

	 if(list==SIP_NULL) 
	 {
		  *error = E_NO_EXIST;
		  return SipFail;
	 }

	tmp = (SipListElement *)fast_memget(0,sizeof(SipListElement),error); 
	 if(tmp==SIP_NULL)
	 { 
		  *error = E_NO_MEM;
		  return SipFail;
	 }
	 tmp->pData = pData;
	 tmp->next=SIP_NULL;
		 
	 if(list->head==SIP_NULL)
	 {
		  list->head = list->tail = tmp;
		  list->size++;
		  *error = E_NO_ERROR;
		  return SipSuccess;
	 }
	 it = list->tail;
	 it->next = tmp;
	 list->tail = tmp;
	 /*
	  * Incrementing the list size
	  */
	 list->size++;
	  *error = E_NO_ERROR;
	 return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_listPrepend
** DESCRIPTION:This function prepends the pData passed to the siplist.
** The list maintains  a pointer to the pData passed and does not copy
** the pData. Hence pData must not be freed after it has been prepended 
** to a siplist.
** 
** PARAMETERS:
**      	      *list(IN)			:The List which a node has to be prepended
**				  pDatac(IN)		:The data to be prepended
**				  error(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_listPrepend
#ifdef ANSI_PROTO
	( SipList 		*list,
	  void 			*pData, 
	  SipError 		*error )
#else
	( list, pData, error )
	  SipList 		*list;
	  void 			*pData;
	  SipError 		*error;
#endif
{
	 SipListElement *tmp;
	 
	 tmp = (SipListElement *)fast_memget(0,sizeof(SipListElement),error); 
	 if(tmp==SIP_NULL)
	 { 
		 *error = E_NO_MEM;
		 return SipFail;
	 }
	 if(list==SIP_NULL) 
	 {
		 *error = E_NO_EXIST;
                 fast_memfree(0, tmp, SIP_NULL);
		 return SipFail;
	 }
	 tmp->pData = pData;
	 tmp->next = list->head;
	 if(list->tail==SIP_NULL) 
		list->tail = list->head;
	 list->head = tmp;
	 list->size++;

	  *error = E_NO_ERROR;
	 return SipSuccess;
}

/*******************************************************************************
** FUNCTION: sip_listGetAt
** DESCRIPTION:This function gets the pData at the index specified. 
** The pointer to the pData is returned by the function. It must not be freed.
** Indexing begins from 0. 
** 
** PARAMETERS:
**      	      *list(IN)			:The List which a node has to be retrieved
**				position(IN)		:The position from which pData has to be
** 												retrieved
**				  pData(IN)			:The data retrieved
**				  error(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool sip_listGetAt
#ifdef ANSI_PROTO
	( SipList 		*list,
	  SIP_U32bit 		position,
	  SIP_Pvoid 		*pData, 
	  SipError 		*error )
#else
	( list, position,pData, error )
	  SipList 		*list;
	  SIP_U32bit 		position;
	  SIP_Pvoid		*pData;
	  SipError 		*error;
#endif
{
		 SipListElement *it;
		 SIP_U32bit counter=0;

		 if(list==SIP_NULL) 
		 {
		  	*error = E_NO_EXIST;
		 	 return SipFail;
		 }
		 if(list->head == SIP_NULL)
		{
			*error = E_NO_EXIST;
			return SipFail;
		}

		 for(it=list->head;(it!=SIP_NULL)&&(counter!=position);\
		 	it=it->next,counter++)
		 {
			/*
			 * Go to the position 
			 */
		 }
		 if((it==SIP_NULL)||(counter!=position)) 
		 {
		  	*error = E_INV_INDEX;
			  return SipFail;
		 }
		 else 
		 {
			  *pData = it->pData;
			  *error = E_NO_ERROR;
			  return SipSuccess;
		 }
}


/*******************************************************************************
** FUNCTION: sip_listSetAt
** DESCRIPTION:This function sets the pData at the index specified. 
** 	It frees up pData that exists at the index before the set is invoked.
** 	The pData pointer passed to this function must not be freed.
** 	Index begins at 0.
** 
** PARAMETERS:
**      	      *list(IN)			:The List to which a node to be set
**				position(IN)		:The position to which pData has to be
** 												set
**				  pData(IN)			:The data set
**				  error(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool sip_listSetAt
#ifdef ANSI_PROTO
	( SipList 		*list,
	  SIP_U32bit 		position,
	  void 			*pData, 
	  SipError 		*error )
#else
	( list, position, pData, error )
	  SipList 		*list;
	  SIP_U32bit 		position;
	  void 			*pData;
	  SipError 		*error;
#endif
{
		 SipListElement *it;
		 SIP_U32bit counter=0;

		 if(list==SIP_NULL) 
		 {
		  	*error = E_NO_EXIST;
		  	return SipFail;
		 }
		 for(it=list->head;(it!=SIP_NULL)&&(counter!=position);\
		 	it=it->next,counter++)
		 {
		 
		 }
		 if((it==SIP_NULL)||(counter!=position)) 
		 {
			*error = E_INV_INDEX;
			return SipFail;
		 }
		 else 
		 {
			  if(list->freefunc!=SIP_NULL) list->freefunc(it->pData);
				  it->pData = pData;
			  *error = E_NO_ERROR;
			  return SipSuccess;
		 }
}


/*******************************************************************************
** FUNCTION: sip_listSizeOf
** DESCRIPTION:This function returns the number of elements in the list.
** 
** PARAMETERS:
**      	      	*list(IN)			:The List whose size to be found
**					size(OUT)			:The size retrieved
**				  	error(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_listSizeOf
#ifdef ANSI_PROTO
	( SipList 		*list, 
	  SIP_U32bit 		*size, 
	  SipError 		*error )
#else
 	( list, size, error )
	  SipList 		*list; 
	  SIP_U32bit 		*size; 
	  SipError 		*error; 
#endif
{
	 if (list==SIP_NULL) 
	 {
		  *size = 0;
		  *error = E_NO_EXIST;
		  return SipFail;
	 }

	*size = list->size;
	 *error = E_NO_ERROR;
	 return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_listInsertAt
** DESCRIPTION:This function inserts pData at the index specified. 
 ** The pData passed is placed at the index given to the function.
 ** The pData pointer passed to this function must not be freed.
 ** Index begins at zero.
** 
** PARAMETERS:
**      	      	*list(IN)			:The SipList 
**					Position(OUT)		:The loaction at which insertion has to ** 										  be one
**					pData(IN)			:The data to be inserted
**				  	error(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_listInsertAt
#ifdef ANSI_PROTO
	( SipList 		*list, 
	  SIP_U32bit 		position, 
	  void 			*pData, 
	  SipError 		*error )
#else
	( list, position, pData, error )
	  SipList 		*list;
	  SIP_U32bit 		position;
	  void 			*pData;
	  SipError 		*error;
#endif
{
		 SipListElement *tmp, *it;
		 SIP_U32bit counter=0;

		 if(list==SIP_NULL) 
		 {
		 	 *error = E_NO_EXIST;
		  	return SipFail;
		 }

		 if(position>(list->size)) 
		 {
		  	*error = E_INV_INDEX;
		  	return SipFail;
		 }
		 if(position==list->size) 
		 	return sip_listAppend(list,pData,error);
		 if(position==0) 
		 	return sip_listPrepend(list,pData,error);
		 for(it=list->head;(it!=SIP_NULL)&&(counter!=position-1);\
		 	it=it->next,counter++)
		 {
		 
		 }
		 tmp = (SipListElement *)fast_memget(0,sizeof(SipListElement),error); 
		 if(tmp==SIP_NULL) 
		 {
			*error = E_NO_MEM;
			return SipFail;
		 }
		 tmp->pData = pData;
		 if(SIP_NULL != it)
		 {
			 tmp->next = it->next;
			 it->next = tmp; 
			 list->size++;
		 }
		 else
		 {
			 *error = E_NO_EXIST;
			 (void)fast_memfree(0,tmp,SIP_NULL);
			 return SipFail;
		 }
			 
		 *error = E_NO_ERROR;
		 
		 return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_listDeleteAt
** DESCRIPTION:This function removes the element at the index specified.
** It also frees up the memory allocated to the pData using the free function
** supplied in sip_listInit.
** Index begins at zero.
** 
** PARAMETERS:
**      	      	*list(IN)			:The SipList 
**					Position(OUT)		:The loaction at which deletion has to 
** 										  be one
**				  	error(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_listDeleteAt
#ifdef ANSI_PROTO
	( SipList 		*list, 
	  SIP_U32bit 		position, 
	  SipError 		*error )
#else
 	( list, position, error )
	  SipList 		*list; 
	  SIP_U32bit 		position; 
	  SipError 		*error; 
#endif
{
	 SipListElement *tmp = SIP_NULL, *it = SIP_NULL;
	 SIP_U32bit counter=0;

	 if(list==SIP_NULL) 
	 {
		*error = E_NO_EXIST;
		return SipFail;
	 }

 	if(list->size==0)
	{
 	 	*error = E_INV_INDEX;
  		return SipFail;
	}

	 if(position>(list->size-1))
	 {
		*error = E_INV_INDEX;
		return SipFail;
	 }
	 if (position==0) 
	 {
		  tmp = list->head;
		  list->head = list->head->next;
		  if (list->head==SIP_NULL) 
			list->tail=SIP_NULL;
		  if (tmp!=SIP_NULL)
		  {
				if(list->freefunc!=SIP_NULL)
					if (tmp->pData!=SIP_NULL) 
						list->freefunc(tmp->pData);
				(void)fast_memfree(0,tmp,SIP_NULL);
		  }
		  list->size--;

		  *error = E_NO_ERROR;
		  return SipSuccess; 
	 }
	 for(it=list->head;(it!=SIP_NULL)&&(counter!=position-1);it=it->next,counter++)
	 {}

         if(SIP_NULL != it)
	    tmp = it->next;

	 if ((SIP_NULL != tmp) && (SIP_NULL != it))
	 {
		 it->next = tmp->next;
		 if(list->freefunc!=SIP_NULL)
			 if (tmp->pData!=SIP_NULL) 
				 list->freefunc(tmp->pData);
		 (void)fast_memfree(0,tmp,SIP_NULL);
	 }

	 if (tmp==list->tail)
		list->tail = it;
	 list->size--;
	 *error = E_NO_ERROR;

	 return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_listForEach
** DESCRIPTION:This function iterates through the list and invokes the function
 ** passed to it on every element in the list.
** 
** PARAMETERS:
**      	      	*list(IN)			:The SipList 
**					func(OUT)			:The function pointer of function 
**										  which has to be operated over each 
**										  element 
**				  	error(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_listForEach
#ifdef ANSI_PROTO
	( SipList 		*list,
	  sip_listFuncPtr func,
	  SipError 		*error )
#else
 	( list, func, error )
	  SipList 		*list;
	  sip_listFuncPtr func;
	  SipError 		*error; 
#endif
{
	 SipListElement *it;

	 if (list==SIP_NULL) 
	 {
		*error = E_NO_EXIST;
		return SipFail;
	 }

	 for (it=list->head;it!=SIP_NULL;it=it->next)
		func(it->pData);

	  *error = E_NO_ERROR;
	  return SipSuccess;
}


/* MICROUA begin */
/******************************************************************************
 ** FUNCTION: sip_listForEachWithData
 ******************************************************************************
 ** This function iterates through the list and invokes the function
 ** passed to it on every element in the list.
 *****************************************************************************/

SipBool sip_listForEachWithData
#ifdef ANSI_PROTO
	( SipList 		*list,
	  sip_listFuncPtrWithData func,
	  SIP_Pvoid           *pData,
	  SipError 		*error )
#else
 	( list, func, pData, error )
	  SipList 		*list;
	  sip_listFuncPtrWithData func;
	  SIP_Pvoid           *pData;
	  SipError 		*error; 
#endif
{
 SipListElement *it;

 if(list==SIP_NULL) 
 {
  *error = E_NO_EXIST;
  return SipFail;
 }

 for(it=list->head;it!=SIP_NULL;it=it->next)
  func(it->pData,pData);
  *error = E_NO_ERROR;
  return SipSuccess;
}
/* MICROUA end */



/*******************************************************************************
** FUNCTION: sip_listDeleteAll
** DESCRIPTION: This function removes all elements from the list.
** It also frees up the memory allocated to the pData using the free function
** supplied in sip_listInit.
** The list can be reused without an Init after this funcion is called.
** 
** PARAMETERS:
**      	      	*list(IN)			:The SipList 
**					error(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_listDeleteAll
#ifdef ANSI_PROTO
	( SipList 		*list,
	  SipError 		*error )
#else
 	( list,error )
	  SipList 		*list;
	  SipError 		*error; 
#endif
{
	 if (list==SIP_NULL) 
	 {
		*error = E_NO_EXIST;
		return SipFail;
	 }

	 while(list->size!=0)
		if(sip_listDeleteAt(list,0,error)!=SipSuccess) 
			return SipFail;

	*error = E_NO_ERROR;
	return SipSuccess;
}

