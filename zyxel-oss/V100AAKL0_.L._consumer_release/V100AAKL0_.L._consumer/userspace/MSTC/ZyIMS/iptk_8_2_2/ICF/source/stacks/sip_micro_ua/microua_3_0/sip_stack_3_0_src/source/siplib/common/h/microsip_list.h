/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 ** FUNCTION:
 **	 This file contains the protypes of the functions used to manipulate
 **	 SipList structures. It also contains the SipList structure definitions
 ******************************************************************************
 **
 ** FILENAME:
 ** siplist.h
 **
 ** DESCRIPTION:
 **	All modules using SipLists must include this file for the structure definitions
 **	and the function prototypes.
 **
 **
 ** DATE			NAME			REFERENCE		REASON
 ** ----			----			--------		------
 ** 20/03/2002	   Mahesh				--			Initial	
 **
 ** COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#ifndef __SIPLIST__
#define __SIPLIST__

#include <stdio.h> 
#include "microsip_common.h"

typedef struct _SipListElement
{
	SIP_Pvoid pData;
	struct _SipListElement *next;
} SipListElement;

typedef struct
{
	SIP_U32bit size;
	SipListElement *head;
	SipListElement *tail;
	void  (*freefunc)(SIP_Pvoid );
} SipList;

typedef void (*sip_listFuncPtr)(SIP_Pvoid);

/* MICROUA begin */
typedef void (*sip_listFuncPtrWithData)(SIP_Pvoid,SIP_Pvoid);
/* MICROUA end */

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
extern SipBool sip_listInit _ARGS_((SipList *list,sip_listFuncPtr freefunc,\
		SipError *error));
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
extern SipBool sip_listAppend _ARGS_((SipList *list, SIP_Pvoid data, \
	SipError *error));
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
extern SipBool sip_listPrepend _ARGS_((SipList *list, SIP_Pvoid data, \
	SipError *error));
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
extern SipBool sip_listGetAt _ARGS_((SipList *list, SIP_U32bit position,\
	SIP_Pvoid *data, SipError *error));

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
extern SipBool sip_listSetAt _ARGS_((SipList *list, SIP_U32bit position, \
	SIP_Pvoid data, SipError *error));
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
extern SipBool sip_listSizeOf _ARGS_((SipList *list, SIP_U32bit *size, \
	SipError *error));
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
extern SipBool sip_listInsertAt _ARGS_((SipList *list, SIP_U32bit position, \
	SIP_Pvoid data, SipError *error));
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
extern SipBool sip_listDeleteAt _ARGS_((SipList *list, SIP_U32bit position, \
	SipError *error));
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
extern SipBool sip_listForEach _ARGS_((SipList *list, sip_listFuncPtr func, \
	SipError *error));

/* MICROUA begin */
extern SipBool sip_listForEachWithData _ARGS_((SipList *list,\
	sip_listFuncPtrWithData func, SIP_Pvoid *pData, SipError *error));
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
extern SipBool sip_listDeleteAll _ARGS_((SipList *list, SipError *error));
#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

