
/* 
** THIS FILE IS INTERNALLY USED BY THE STACK
** THE USER SHOULD NOT INCLUDE THIS FILE DIRECTLY
**/

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 


/**************************************************************************
** FUNCTION:
**  This file contains the INTERNAL prototypes of parser clone functions
**
***************************************************************************
**
** FILENAME:
**  microsip_clone.h
**
** DESCRIPTION
**
**
**  DATE            NAME             REFERENCE			REASON
**	----			----			 ---------			------
** march 18 2002   Mahesh			Core Stack			Initial
**
**
** COPYRIGHT , Aricent, 2006
***************************************************************************/

#ifndef __MICROSIP_CLONEINTRNL_H_
#define __MICROSIP_CLONEINTRNL_H_
#include "microsip_struct.h"

/*******************************************************************************
** FUNCTION: __sipParser_cloneSipParam
** DESCRIPTION: This function copies the contents of a SIP param
**		structure into another SIP param structure
** 
** PARAMETERS:
**      	      pDest(OUT)			:The destination sipparam
**				  pSource(IN)			:the Sipparam to be cloned
**				  pError(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

extern SipBool __sipParser_cloneSipParam _ARGS_((SipParam *pDest, \
	SipParam *pSource, SipError *pErr));
/*******************************************************************************
** FUNCTION: __validateSipAddrSpecType
** DESCRIPTION: This function returns SipSuccess if "Type" is 
** among the defined en_AddType's ; else it returns SipFail;
** 
** PARAMETERS:
**      	      dType(IN)				:The adderspec to be validated
**				  pError(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern SipBool __validateSipAddrSpecType _ARGS_((en_AddrType dType, \
		SipError *err));
/*******************************************************************************
** FUNCTION: sip_cloneSipUrl
** DESCRIPTION:  This function makes a deep copy of the 
** fileds from the Url structures "source" to "dest".
** 
** PARAMETERS:
**      	      to(OUT)			:The destination sipurl
**				  from(IN)			:The source sipusrl
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

extern SipBool __sipParser_cloneSipUrl _ARGS_((SipUrl *to, SipUrl *from, \
		SipError *err));

/*******************************************************************************
** FUNCTION:  __sipParser_cloneSipAddrSpec
** DESCRIPTION:   This function makes a deep copy of the fileds 
** from the AddrSpec structures "source" to "dest".
** 
** PARAMETERS:
**      	      dest(OUT)			:The destination SipAddrSpec
**				  source(IN)		:The source SipAddrSpec
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

extern SipBool __sipParser_cloneSipAddrSpec _ARGS_((SipAddrSpec *dest, \
	SipAddrSpec *source, SipError *err));
#define __sip_cloneSipAddrSpec 	__sipParser_cloneSipAddrSpec
/*******************************************************************************
** FUNCTION:  __sipParser_cloneSipParamList
** DESCRIPTION:  This function makes a deep copy of a 
** SipList of SipParam from the "source" to "dest".
**
** PARAMETERS:
**      	      dest(OUT)			:The destination SipList
**				  source(IN)		:The source SipList
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

extern SipBool __sipParser_cloneSipParamList _ARGS_((SipList *dest, \
	SipList *source, SipError *err));

/*******************************************************************************
** FUNCTION:  __sipParser_cloneSipCommonHeader
**
** DESCRIPTION: This function makes a deep copy of the fileds 
** from the CommonHeader structures "source" to "dest".
**
** PARAMETERS:
**      	      dest(OUT)			:The destination SipCommonHeader
**				  source(IN)		:The source SipCommonHeader
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern SipBool __sipParser_cloneSipCommonHeader _ARGS_ ((SipCommonHeader *pDest, 
			SipCommonHeader *pSource, 
			SipError *pErr));

/*******************************************************************************
** FUNCTION:  __sipParser_cloneSipFromHeader
**
** DESCRIPTION: This function makes a deep copy of the fileds 
** from the FromHeader structures "source" to "dest".
**
** PARAMETERS:
**      	      dest(OUT)			:The destination SipFromHeader
**				  source(IN)		:The source SipFromHeader
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
#define __sipParser_cloneSipFromHeader __sipParser_cloneSipCommonHeader
#define __sipParser_cloneSipToHeader   __sipParser_cloneSipCommonHeader

#define validateSipAddrSpecType  __validateSipAddrSpecType
#endif /* __MICROSIP_CLONEINTRNL_H_ */

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

