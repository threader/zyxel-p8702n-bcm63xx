/* Ensure Names are not mangled by C++ compilers */

/***********************************************************************
 ** FUNCTION:
 **			This file has the freeing functions for all structures
 **
 *********************************************************************
 **
 ** FILENAME		: sdf_free.h
 **
 ** DESCRIPTION		: This file has the freeing functions for all structures
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 27/10/00	Anitha						Creation
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/

#ifndef __SDF_FREE_H_
#define __SDF_FREE_H_

#include "sdf_struct.h"
#ifdef __cplusplus
extern "C" {
#endif 

#ifdef SDF_THREAD_SAFE
extern Sdf_ty_mutexIdT dGlbSdfCallObjectIdGeneratorMutex;
#endif
	
/*********************************************************
** FUNCTION: sdf_fn_uaFreeRemoteRetransKey
**
** DESCRIPTION: This function frees the RemoteRetransKey 
**				structure.
**
** PARAMETERS:
**		pRemoteRetransBuffer(IN): The structure to be freed
**
**********************************************************/
 Sdf_ty_retVal sdf_fn_uaFreeRemoteRetransKey _ARGS_\
	((Sdf_st_remoteRetransKey *pRemoteRetransKey));

/***********************************************************************
** FUNCTION:  __sdf_fn_uaFreeRemoteRetransBuffer
**
** DESCRIPTION: Frees the RemoteRetransBuffer Structure. Used for
**				list operations
**
** PARAMETERS:
**		pRemoteRetransBuffer(IN): void pointer to the structure to be freed
**
************************************************************************/
 void __sdf_fn_uaFreeRemoteRetransBuffer _ARGS_((Sdf_ty_pvoid \
	RemoteRetransBuffer));


/***********************************************************************
** FUNCTION:  sdf_fn_uaFreeRemoteRetransBuffer
**
** DESCRIPTION: Frees the RemoteRetransBuffer Structure
**
** PARAMETERS:
**		pRemoteRetransBuffer(IN): The structure to be freed
**
************************************************************************/
 Sdf_ty_retVal sdf_fn_uaFreeRemoteRetransBuffer _ARGS_\
	((Sdf_st_remoteRetransBuffer *pRemoteRetransBuffer));



#ifdef SDF_APP_CONTROLLED_REMOTE_RETX

/***********************************************************************
** FUNCTION:  sdf_ivk_uaFreeRemoteRetransTable
**
** DESCRIPTION: Frees the RemoteRetrans hash table 
**
** PARAMETERS:
**		pRemoteRetransHash(IN): The structure to be freed
**		pErr (IN/OUT): Error structure
**
************************************************************************/


 Sdf_ty_retVal sdf_ivk_uaFreeRemoteRetransTable _ARGS_\
	((Sdf_st_hash *pRemoteRetransHash, Sdf_st_error *pErr));
#endif




/***********************************************************************
** FUNCTION: __sdf_fn_uaFreeNameInfo
**
** DESCRIPTION: Frees the NameInfo structure. Used for list operations
**
** PARAMETERS:
**		pNameInfo(IN): void pointer to the structure to be freed
**
************************************************************************/
 void __sdf_fn_uaFreeNameInfo _ARGS_((Sdf_ty_pvoid pNameInfo));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaFreeNameInfo
**
** DESCRIPTION: Frees the NameInfo Structure
**
** PARAMETERS:
**		pNameInfo(IN): The structure to be freed
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeNameInfo _ARGS_\
	((Sdf_st_nameInfo *pNameInfo));

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeTransportInfo
**
** DESCRIPTION: This function frees the Transport-Info structure
**              by taking a void pointer.
**
**
**********************************************************/
void __sdf_fn_uaFreeTransportInfo _ARGS_((Sdf_ty_pvoid pTransInfo));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaFreeTransportInfo
**
** DESCRIPTION: Frees the TransportInfo Structure
**
** PARAMETERS:
**		pTranspInfo(IN): The structure to be freed
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeTransportInfo _ARGS_((Sdf_st_transportInfo \
	*pTranspInfo));

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeAppData
**
** DESCRIPTION: This function frees the AppData
**	 			structure
**
** PARAMETERS:
**		pAppData(IN): The structure to be freed
**********************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeAppData _ARGS_((Sdf_st_appData *pAppData));

/***********************************************************************
** FUNCTION: sdf_ivk_uaFreeCallObject
**
** DESCRIPTION: Frees the CallObject Structure
**
** PARAMETERS:
**		ppCallObj(IN): The structure to be freed
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeCallObject _ARGS_((Sdf_st_callObject \
	**ppCallObj));

/***********************************************************************
** FUNCTION: sdf_ivk_uaFreeInitData
**
** DESCRIPTION: Frees the InitData Structure
**
** PARAMETERS:
**		pInitData(IN): The structure to be freed
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeInitData _ARGS_((Sdf_st_initData  \
	*pInitData));

#ifdef SDF_LOOKUP
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaFreeDnsEventContext
**
** DESCRIPTION: This function frees the DNS Event Context structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeDnsEventContext (
    Sdf_st_dnsEventContext *pDnsEventContext);

/***********************************************************************
** Function: __sdf_fn_uaDnsList
** Description: Frees the DNS List structure
**
************************************************************************/
void __sdf_fn_uaFreeDnsList _ARGS_((Sdf_ty_pvoid pvDnsRecord));
#endif

/***********************************************************************
** FUNCTION: sdf_ivk_uaCloseToolkit
**
** DESCRIPTION: Frees data structures and threads (if any) used by the 
**				toolkit
**
** PARAMETERS:
**			none	
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloseToolkit _ARGS_((void));

/***********************************************************************
** FUNCTION: sdf_ivk_uaFreeCommonInfo
**
** DESCRIPTION: Frees the CommonInfo structure
**
** PARAMETERS:
**		pCommonInfo(IN): The structure to be freed
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeCommonInfo _ARGS_((Sdf_st_commonInfo \
	*pCommonInfo));

/***********************************************************************
** FUNCTION: __sdf_fn_uaFreeCallInfo
**
** DESCRIPTION: Frees the CallInfo structure. Used for list operations
**
** PARAMETERS:
**		pCallInfo(IN): void pointer to the structure to be freed
**
************************************************************************/
 void __sdf_fn_uaFreeCallInfo  _ARGS_((Sdf_ty_pvoid pCallInfo));

/***********************************************************************
** FUNCTION: sdf_ivk_uaFreeCallInfo
**
** DESCRIPTION: Frees the CallInfo structure
**
** PARAMETERS:
**		pCallInfo(IN): The structure to be freed
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeCallInfo  _ARGS_((Sdf_st_callInfo \
	*pCallInfo));

/***********************************************************************
** FUNCTION: __sdf_fn_uaFreeHeaderList
**
** DESCRIPTION: Frees the Header List structure. Used for list operations
**
** PARAMETERS:
**		pHeaderList(IN): void pointer to the structure to be freed
**
************************************************************************/
 void __sdf_fn_uaFreeHeaderList _ARGS_((Sdf_ty_pvoid pHeaderList));

/***********************************************************************
** FUNCTION: sdf_ivk_uaFreeHeaderList
**
** DESCRIPTION: Frees the HeaderList structure
**
** PARAMETERS:
**		pHeaderList(IN): The structure to be freed
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeHeaderList _ARGS_((Sdf_st_headerList \
	*pHeaderList));
#ifdef SDF_AUTHENTICATE
/*********************************************************
** FUNCTION: sdf_ivk_uaFreeAuthenticationParams
**
** DESCRIPTION: This function frees the authenticationParams
**	structure.
**
** PARAMETERS:
**		pAuthenticationParams(IN): The structure to be freed
**
**********************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeAuthenticationParams \
	 _ARGS_((Sdf_st_authenticationParams *pAuthenticationParams));

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeAuthenticationParams
**
** DESCRIPTION: This internal function frees the 
**				Sdf_st_authenticationParams structure. 
**				Used for list operations
**
** PARAMETERS:
**		pAuthenticationParams(IN): void pointer to the 
**									structure to be freed
**
**********************************************************/
void __sdf_fn_uaFreeAuthenticationParams \
	_ARGS_((Sdf_ty_pvoid pAuthenticationParams));

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeUacAuthInfo
**
** DESCRIPTION: This function frees the Sdf_st_uacAuthInfo 
**	structure.
**
** PARAMETERS:
**		pUacAuthInfo(IN): The structure to be freed
**
**********************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeUacAuthInfo \
	 _ARGS_((Sdf_st_uacAuthInfo *pUacAuthInfo));

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeUacAuthInfo
**
** DESCRIPTION: This internal function frees the 
**				Sdf_st_uacAuthInfo structure. Used for
**				list operations
**
** PARAMETERS:
**		pUacAuthInfo(IN): void pointer to the 
**							structure to be freed
**
**********************************************************/
void __sdf_fn_uaFreeUacAuthInfo \
	_ARGS_((Sdf_ty_pvoid pUacAuthInfo));

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeUasAuthInfo
**
** DESCRIPTION: This function frees the Sdf_st_uasAuthInfo 
**	structure.
**
** PARAMETERS:
**		pUasAuthInfo(IN): The structure to be freed
**
**********************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeUasAuthInfo \
	 _ARGS_((Sdf_st_uasAuthInfo *pUasAuthInfo));

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeUasAuthInfo
**
** DESCRIPTION: This internal function frees the 
**				Sdf_st_uasAuthInfo structure. Used for
**				list operations
**
** PARAMETERS:
**		pUasAuthInfo(IN): void pointer to the structure to be freed
**
**********************************************************/
void __sdf_fn_uaFreeUasAuthInfo \
	_ARGS_((Sdf_ty_pvoid pUasAuthInfo));
#endif /* SDF_AUTHENTICATE */


/*********************************************************
** FUNCTION: __sdf_fn_uaFreeConfigExtraHeadersList
**
** DESCRIPTION: This function frees the ConfigExtraHeaders 
**				structure. Used for list operations
** PARAMETERS:
**		pConfHdr(IN): void pointer to the structure to be freed
**
**********************************************************/
 void __sdf_fn_uaFreeConfigExtraHeaders _ARGS_((Sdf_ty_pvoid \
	 pConfHdr));

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeConfigExtraHeaders
**
** DESCRIPTION: This function frees the ConfigExtraHeaders
**				structure
**
** PARAMETERS:
**		pConfHdr(IN): The structure to be freed
**
**********************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeConfigExtraHeaders \
	 _ARGS_((Sdf_st_configExtraHeaders *pConfHdr));

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeCompleteHeader
**
** DESCRIPTION: This function frees the SipHeader structure 
**				completely. Used for list operations.
**
** PARAMETERS:
**		pHdr(IN): void pointer to the structure to be freed
**
**********************************************************/
 void __sdf_fn_uaFreeCompleteHeader _ARGS_((Sdf_ty_pvoid pHdr));

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeMsgBody
**
** DESCRIPTION: This function frees the Sdf_st_msgBody
**				structure completely. Used for list operations
**
** PARAMETERS:
**		pMsgBody(IN): void pointer to the structure to be freed
**
**********************************************************/
 void __sdf_fn_uaFreeMsgBody _ARGS_((Sdf_ty_pvoid pMsgBody));

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeMsgBody
**
** DESCRIPTION: This function frees the Sdf_st_msgBody
**			strucure completely. 
**
** PARAMETERS:
**		pMsgBody(IN): The structure to be freed
**
**********************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeMsgBody _ARGS_((Sdf_st_msgBody *pMsgBody));

/***********************************************************************
** FUNCTION: __sdf_fn_uaFreeCallObjectKey
**
** DESCRIPTION: Frees the CallObject Key structure. Used for list
**				operations
**
** PARAMETERS:
**		pCallObjKey(IN): void pointer to the structure to be freed
**
************************************************************************/
 void __sdf_fn_uaFreeCallObjectKey _ARGS_((Sdf_ty_pvoid pCallObjKey));

 /***********************************************************************
** FUNCTION: __sdf_fn_uaFreeCallObject
**
** DESCRIPTION: Frees the CallObject structure. Used for list
**				operations
**
** PARAMETERS:
**		pCallObj(IN): void pointer to the structure to be freed
**
************************************************************************/
 void __sdf_fn_uaFreeCallObject _ARGS_((Sdf_ty_pvoid pCallObj));

/***********************************************************************
** FUNCTION: sdf_ivk_uaFreeCallObjectKey
**
** DESCRIPTION: Frees the CallObject Key structure
**
** PARAMETERS:
**		pCallObjKey(IN): The structure to be freed
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeCallObjectKey \
	_ARGS_((Sdf_st_callObjectKey *pCallObjKey));



/***********************************************************************
** FUNCTION: __sdf_fn_uaFreeOverlapTransInfo
**
** DESCRIPTION: Frees the OverlapTransInfo structure. Used for
**				list operations
**
** PARAMETERS:
**		pOverlapTransInfo(IN): void pointer to the structure to be freed
**
************************************************************************/
 void __sdf_fn_uaFreeOverlapTransInfo \
	_ARGS_((Sdf_ty_pvoid pOverlapTransInfo));

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeOverlapTransInfo
**
** DESCRIPTION: This function frees the Sdf_st_overlapTransInfo
**				structure completely. 
**
** PARAMETERS:
**		pOverlapTransInfo(IN): The structure to be freed
**			
**********************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeOverlapTransInfo \
	_ARGS_((Sdf_st_overlapTransInfo *pOverlapTransInfo));

/***********************************************************************
** FUNCTION: __sdf_fn_uaFreeString
**
** DESCRIPTION: Frees the string passed as a void pointer. Used
**				for list operations.
**
** PARAMETERS:
**		pString(IN): void pointer to the string to be freed
**
************************************************************************/
 void __sdf_fn_uaFreeString _ARGS_((Sdf_ty_pvoid pString));

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeEventContext
**
** DESCRIPTION: This function frees the Sdf_st_eventContext
**	 			structure
**
** PARAMETERS:
**		pEventContext(IN): The structure to be freed
**********************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeEventContext \
 	_ARGS_((Sdf_st_eventContext *pEventContext));

/***********************************************************************
** FUNCTION: sdf_ivk_uaFreeTransaction
**
** DESCRIPTION: Frees the Transaction Structure
**
** PARAMETERS:
**		pTransaction(IN): The structure to be freed
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFreeTransaction _ARGS_((Sdf_st_transaction \
	*pTransaction));

/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaClearTransaction
 **
 ** DESCRIPTION: This function has to be invoked when a transaction that was
 **				 being started has to be aborted. It frees all the contents
 **				 of the UAC transaction object and unlocks the transaction 
 **				 object for use.
 **				 
 **				 Note: 	This function does not decrement the reference count 
 **				 ----	of the transaction object.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaClearTransaction \
 	_ARGS_((Sdf_st_callObject *pCallObject,  \
	 Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	 const Sdf_ty_s8bit *pMethod, \
	 Sdf_ty_transactionType dType));

/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaClearRegularTransaction
 **
 ** DESCRIPTION: This function has to be invoked when a Regular transaction 
 **				 that was being started has to be aborted. 
 **				 It removes the Overlap Transaction from CallObject and
 **				 unlocks the transaction object for use
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaClearRegularTransaction \
 	_ARGS_((Sdf_st_callObject *pCallObject, \
	 		const Sdf_ty_s8bit *pMethod, \
			Sdf_st_overlapTransInfo	*pOverlapTransInfo));

/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaRemoveOverlapInfo
 **
 ** DESCRIPTION: This API is used to remove the overlap info element from  
 ** 			 the list of overlap transactions maintained in call object.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaRemoveOverlapInfo _ARGS_(( \
	Sdf_st_callObject *pCallObj, Sdf_st_overlapTransInfo *pOverlapTransInfo));

/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaFreeDestDetails
 **
 ** DESCRIPTION: Frees the Sdf_st_destDetails structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeDestDetails _ARGS_(( \
	Sdf_st_destDetails *pDestDetails));

#ifdef SDF_TXN
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaFreeTxnContextData
 **
 ** DESCRIPTION: This is the free function for transaction context information 
 **				 that the UA Toolkit stores with the stack. Every time the 
 **				 stack issues a callback for a transaction, we get this 
 **				 context back from the stack.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeTxnContextData _ARGS_(\
	(Sdf_st_txnContextData *pTxnContextData));

/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaFreeNoAnswerTimerContext
 **
 ** DESCRIPTION: This is the free function for transaction context information 
 **				 that the UA Toolkit stores with the stack. Every time the 
 **				 stack issues a callback for a transaction, we get this 
 **				 context back from the stack.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFreeNoAnswerTimerContext _ARGS_((\
	Sdf_st_noAnswerTimerContext *pNoAnswerTimerContext));
#endif
/*****************************************************************************
 ** FUNCTION: 	 __sdf_fn_uaFreeU32bit
 **
 ** DESCRIPTION: This API free's the allocated memory for U32bit 
 **
 ******************************************************************************/
void __sdf_fn_uaFreeU32bit
	_ARGS_((Sdf_ty_pvoid	pU32bit));

#ifdef SDF_TLS

/***********************************************************************
** Function: sdf_ivk_uaFreeSslData
** Description: Frees the SslData structure
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSslData _ARGS_((Sdf_st_sslData *pSslData));

/***********************************************************************
** Function: sdf_ivk_uaFreeSslParams
** Description: Frees the SslParams structure
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSslParams _ARGS_((\
	Sdf_st_sslParams *pSslParams));

#endif /* SDF_TLS */

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

#endif
