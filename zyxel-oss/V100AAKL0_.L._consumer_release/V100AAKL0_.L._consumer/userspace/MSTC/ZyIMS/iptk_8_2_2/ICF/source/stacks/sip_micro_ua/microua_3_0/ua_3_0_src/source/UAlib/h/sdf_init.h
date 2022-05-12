
/******************************************************************************
 ** FUNCTION:
 **			This file has the initialization functions for all structures
 **
 ******************************************************************************
 **
 ** FILENAME		: sdf_init.h
 **
 ** DESCRIPTION		: This file has the initialization functions for 
 **						all structures
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 27/10/00	Anitha						Creation
 **
 ******************************************************************************
 **				COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#ifndef __SDF_INIT_H_
#define __SDF_INIT_H_

#include "sdf_struct.h"
#include "sdf_free.h"

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/***********************************************************************
** FUNCTION:  sdf_getPart
**
** DESCRIPTION: This returns the product id for the SIP UA Toolkit
**
** PARAMETERS:
**		none
**
************************************************************************/
 Sdf_ty_s8bit *sdf_getPart _ARGS_((void));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaInitInitData
**
** DESCRIPTION: This is the initialization for the Init Data structure
**				It allocates memory for the structure and initializes 
**				the member variables to NULL.
**
** PARAMETERS:
**		ppInitData(IN/OUT)	: Input structure
**		pErr(OUT)			: Error structure in case the function fails
**
*********************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitInitData _ARGS_((Sdf_st_initData\
	**ppInitData,Sdf_st_error *pErr));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitRemoteRetransKey
**
** DESCRIPTION: This function initializes the remote retransmission
**		key structure and sets the member variables to NULL
**
** PARAMETERS:
**		ppKey(IN/OUT)	: Input structure
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitRemoteRetransKey _ARGS_((\
	Sdf_st_remoteRetransKey **ppKey ,Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaInitRemoteRetransBuffer
**
** DESCRIPTION: This is the initialization for the retransmission
**				buffer structure. It allocates memory for the structure 
**				and initializes the member variables to NULL.
**
** PARAMETERS:
**		ppObject(IN/OUT)	: Input structure
**		pErr(OUT)		: Error structure in case the function fails
**
**********************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitRemoteRetransBuffer _ARGS_((\
	Sdf_st_remoteRetransBuffer **ppObject,Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaInitToolkit
**
** DESCRIPTION: This is the initialization for the toolkit and must
**				be invoked by the application before invoking any other
**				toolkit API.
**
** PARAMETERS:
**		pErr(OUT)		: Error structure in case the function fails
**
**********************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitToolkit  _ARGS_((\
 	Sdf_ty_initToolkitParams *pInitParams,Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaInitCall
**
** DESCRIPTION: This is the initialization for the call object structure
** 				It allocates memory for the structure and initializes 
**				the member variables to NULL. 
**
** PARAMETERS:
**		ppCallObj(IN/OUT)	: Input structure
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitCall  _ARGS_((Sdf_st_callObject \
			**ppCallObj,Sdf_st_error *pErr));
#ifdef SDF_AUTHENTICATE

/******************************************************************
** FUNCTION:  sdf_ivk_uaInitAuthenticationParams
**
** DESCRIPTION: This function initializes the authenticationParams structure
**
** PARAMETERS:
**		ppAuthenticationParams(IN/OUT)	: Input structure
**		pErr(OUT)						: Error structure in case the 
**											function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitAuthenticationParams \
	 _ARGS_((Sdf_st_authenticationParams **ppAuthenticationParams, \
		Sdf_st_error *pErr));


/******************************************************************
** FUNCTION:  sdf_ivk_uaInitUacAuthInfo
**
** DESCRIPTION: This function initializes the Sdf_st_uacAuthInfo structure
**
** PARAMETERS:
**		ppUacAuthInfo(IN/OUT)			: Input structure
**		pErr(OUT)						: Error structure in case the 
**											function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitUacAuthInfo _ARGS_\
	((Sdf_st_uacAuthInfo **ppUacAuthInfo, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaInitUasAuthInfo
**
** DESCRIPTION: This function initializes the Sdf_st_uasAuthInfo structure
**
** PARAMETERS:
**		ppUasAuthInfo(IN/OUT)			: Input structure
**		pErr(OUT)						: Error structure in case the 
**											function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitUasAuthInfo _ARGS_\
	((Sdf_st_uasAuthInfo **ppUasAuthInfo, Sdf_st_error *pErr));
#endif /* SDF_AUTHENTICATE */


/******************************************************************
** FUNCTION:  sdf_ivk_uaInitTransportInfo
**
** DESCRIPTION: This function initializes the Transport info structure
**
** PARAMETERS:
**		ppTranspInfo(IN/OUT): Input structure
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitTransportInfo _ARGS_((Sdf_st_transportInfo \
	**ppTranspInfo,Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaInitNameInfo
**
** DESCRIPTION: This function initializes the name info structure
**
** PARAMETERS:
**		ppNameInfo(IN/OUT)	: Input structure
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitNameInfo  _ARGS_((Sdf_st_nameInfo \
	**ppNameInfo,Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaInitCommonInfo
**
** DESCRIPTION: This function initializes the CommonInfo structure
**
** PARAMETERS:
**		ppCommonInfo(IN/OUT): Input structure
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitCommonInfo  _ARGS_((Sdf_st_commonInfo \
	**ppCommonInfo,Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaInitHeaderList
**
** DESCRIPTION: This function initializes the header list  structure
**
** PARAMETERS:
**		ppHeaderList(IN/OUT): Input structure
**		dType(IN)			: The type of headers that will be stored 
**								in the list
**		pCorrelation(IN)	: Correlation string for the header list
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitHeaderList  _ARGS_((Sdf_st_headerList \
	**ppHeaderList, en_HeaderType dType, Sdf_ty_s8bit *pCorrelation, \
	Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaInitMsgBody
**
** DESCRIPTION: This function initializes the Sdf_st_msgbody structure
**
** PARAMETERS:
**		ppMsgBody(IN/OUT)	: Input structure
**		pCorrelation(IN)	: Correlation string for the structure
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitMsgBody  _ARGS_((Sdf_st_msgBody \
	**ppMsgBody, Sdf_ty_s8bit *pCorrelation, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaInitCallInfo
**
** DESCRIPTION: This function initializes the CallInfo structure
**
** PARAMETERS:
**		ppCallInfo(IN/OUT)	: Input structure
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitCallInfo  _ARGS_((Sdf_st_callInfo \
	**ppCallInfo,Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaStartTransaction
**
** DESCRIPTION: This function initializes the pUacTransaction 
**				structure present in the call object.
**
** PARAMETERS:
**		pObject(IN/OUT)		: Input call object structure
**		pMethod(IN)			: Method of the request to be sent 
**							  out as the new transaction.
**		pErr(OUT)			: Error structure in case the 
**							  function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaStartTransaction  \
 	_ARGS_((Sdf_st_callObject *pObject, 
	Sdf_st_overlapTransInfo	**ppOverlapTransInfo,const Sdf_ty_s8bit *pMethod, \
	Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_fn_uaIsTransactionAllowed
**
** DESCRIPTION: This function checks whether the transaction is allowed  
**				to start or not.
**
** PARAMETERS:
**		pObject(IN/OUT)		: Input call object structure
**		pMethod(IN)			: Method of the request to be sent 
**							  out as the new transaction.
**		dState(IN)			: current call state
**
******************************************************************/
Sdf_ty_bool sdf_fn_uaIsTransactionAllowed _ARGS_((Sdf_st_callObject *pObject,\
		Sdf_ty_messageType 	dMsgType, Sdf_ty_state dState));
 
/******************************************************************
** FUNCTION:  sdf_ivk_uaStartRegularTransaction
**
** DESCRIPTION: This function initializes the one Overlaptransaction 
**				structure and appends it to the call object.
**
** PARAMETERS:
**		pObject(IN/OUT)		: Input call object structure
**		pOverlapTransInfo	: Output structure returned by this function.	
**		pMethod(IN)			: Method of the request to be sent 
**							  out as the new transaction.
**		pErr(OUT)			: Error structure in case the 
**							  function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaStartRegularTransaction  \
 	_ARGS_((Sdf_st_callObject *pObject, \
	Sdf_st_overlapTransInfo **pOverlapTransInfo, \
	const Sdf_ty_s8bit *pMethod, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_fn_uaGetUniqueTxnId
**
** DESCRIPTION: This function is used to generate one unique id for 
**				overlap transaction structure.
**
******************************************************************/

Sdf_ty_u32bit  sdf_fn_uaGetUniqueTxnId _ARGS_ ((void));

 
/******************************************************************
** FUNCTION:  sdf_ivk_uaInitAppData
**
** DESCRIPTION: This function initializes the application context
**				data (AppData) structure. The pData pointer is set 
**				to NULL and the reference count is initialized to 1.
**
** PARAMETERS:
**		ppAppData(IN/OUT)	: Input structure
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitAppData _ARGS_\
	((Sdf_st_appData **ppAppData,Sdf_st_error *pErr));

#ifdef SDF_LOOKUP
/******************************************************************
** FUNCTION:  sdf_ivk_uaInitDnsSrvRecord
**
** DESCRIPTION: This function initializes the DNS SRV record structure.
**
** PARAMETERS:
**	ppDnsSrvRecord(IN/OUT)	: Input structure
**	pErr(OUT)		: Error structure in case the function fails
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitDnsSrvRecord _ARGS_ ((Sdf_st_dnsSrvRecord **ppdnsSrvRecord, Sdf_st_error *pErr));
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitDnsEventContext
**
** DESCRIPTION: This function init the DNS Event Context record
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitDnsEventContext _ARGS_ ((Sdf_st_dnsEventContext \
		 **ppdnsEventContext, Sdf_st_error *pErr));
#endif 
/******************************************************************
** FUNCTION:  sdf_ivk_uaInitConfigExtraHeaders
**
** DESCRIPTION: This function initializes the configExtraHeaders
**				structure
**
** PARAMETERS:
**		ppConfHdrs(IN/OUT)	: Input structure
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitConfigExtraHeaders _ARGS_\
	((Sdf_st_configExtraHeaders **ppConfHdrs,Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaInitCallObjectKey
**
** DESCRIPTION: This function initializes the Call Object Key structure
**
** PARAMETERS:
**		ppCallObjKey(IN/OUT): Input structure
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitCallObjectKey _ARGS_\
	((Sdf_st_callObjectKey **ppCallObjKey,Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaInitOverlapTransInfo
**
** DESCRIPTION: This function initializes the OverlapTransInfo structure
**
** PARAMETERS:
**		ppOverlapTransInfo(IN/OUT)	: Input structure
**		pErr(OUT)					: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitOverlapTransInfo _ARGS_\
	((Sdf_st_overlapTransInfo **ppOverlapTransInfo ,Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaInitEventContext
**
** DESCRIPTION: This function initializes the Sdf_st_eventContext 
**				structure
**
** PARAMETERS:
**		ppEventContext(IN/OUT)	: Input structure
**		pErr(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitEventContext _ARGS_\
	((Sdf_st_eventContext **ppEventContext, Sdf_st_error *pErr));


/******************************************************************
** FUNCTION:  sdf_ivk_uaInitTransaction
**
** DESCRIPTION: This function initializes the Sdf_st_transaction 
**				structure
**
** PARAMETERS:
**		ppTransaction(IN/OUT)	: Input structure
**		pErr(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitTransaction _ARGS_\
	((Sdf_st_transaction **ppTransaction, Sdf_st_error *pErr));



/******************************************************************
** Function:  sdf_ivk_uaInitRemoteRetransHandling
** Description: This function initializes a UATKs remote retrans tables.
**				If 'app controlled' mode is chosen while compiling, a 
**				callback is issued so that the application can initialize the
**				remote retransmission aggregations maintained by it. Otherwise
**				UATK's own remote retransmission aggregation is initialized.
**				'app controlled' mode is used mainly when the application has
**				several message processing threads and it intends to have 
**				thread-specific remote retransmission handlers. 
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitRemoteRetransHandling _ARGS_\
	((Sdf_st_error *pErr));

#ifdef SDF_APP_CONTROLLED_REMOTE_RETX
/******************************************************************
** Function:  sdf_ivk_uaInitRemoteRetransTable
** Description: This function is available only when UATK is compiled with
**				SDF_APP_CONTROLLED_REMOTE_RETX. Application has to use this
**				to initialize remote retransmission entry hash table. 
**				
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitRemoteRetransTable _ARGS_\
	((Sdf_st_hash *pRemoteRetransHash, Sdf_st_error *pErr));
#endif
	
#ifdef SDF_TXN
/*****************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaInitTxnContextData
 **
 ** DESCRIPTION: This is the initialization function for transaction context
 **				 information that the UA Toolkit stores with the stack. Every
 **				 time the stack issues a callback for a transaction, we get
 **				 this context back from the stack.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitTxnContextData \
	_ARGS_((Sdf_st_txnContextData **ppTxnContextData, Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaInitNoAnswerTimerContext
 **
 ** DESCRIPTION: This is the initialization function for context information
 **				 that the UA Toolkit stores with the application while 
 **				 starting a timer waiting for a final response (after getting
 **				 a provisional response).
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitNoAnswerTimerContext _ARGS_((\
	Sdf_st_noAnswerTimerContext **ppNoAnswerTimerContext, \
	Sdf_st_error *pErr));

#endif

#ifdef SDF_TLS

/******************************************************************
** Function:  sdf_ivk_uaInitSslData
** Description: This function initializes the SslData structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSslData _ARGS_((\
    Sdf_st_sslData **ppSslData, Sdf_st_error *pErr));

/******************************************************************
** Function:  sdf_ivk_uaInitSslParams
** Description: This function initializes the SslParam structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSslParams _ARGS_((\
    Sdf_st_sslParams **ppSslParams, Sdf_st_error *pErr));

#endif /* SDF_TLS */

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

#endif
