/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/***********************************************************************
 ** FUNCTION:
 **			This file has all the accessor API's to retrieve/clone 
 **			various structures in the UA toolkit
 *********************************************************************
 **
 ** FILENAME		: sdf_accessor.h
 **
 ** DESCRIPTION		: This file has all the accessor API's to retrieve/clone 
 **					  various structures in the UA toolkit
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 06/12/00	Subhash						Creation
 ** 25/11/02    Elias/Srikanth				UATK 2.0 Media Handling
 ** 22/04/03    Anil Kumar        ---       Changes for TLS feature.
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/

#ifndef __SDF_ACCESSOR_H_
#define __SDF_ACCESSOR_H_

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetTransactionFromCallObject
**
** DESCRIPTION: This function returns a pointer to the appropriate
**				transaction sub-structure of the call object.
**
** PARAMETERS:
**		pObject(IN)			: The callobject structure
**      dType(IN)			: Transaction Type
**		ppTransaction(OUT)	: The transaction object retrieved
**		pError(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetTransactionFromCallObject \
 	_ARGS_((Sdf_st_callObject *pObject, Sdf_ty_transactionType dType, \
	Sdf_st_transaction **ppTransaction, Sdf_st_error *pError));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetTransactionInCallObject
**
** DESCRIPTION: This function sets the transaction structure into the
**				 appropriate (UAC/UAS) transaction of the Call Object
**
** PARAMETERS:
**		pObject(IN/OUT)		: The callobject structure
**      dType(IN)			: Transaction Type
**		ppTransaction(IN)	: The transaction structure that to be set into 
**							  the Call Object
**		pError(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetTransactionInCallObject\
 	_ARGS_((Sdf_st_callObject *pObject, Sdf_ty_transactionType dType, \
	Sdf_st_transaction *ppTransaction, Sdf_st_error *pError));
	

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetFromInfoFromCommonInfo
**
** DESCRIPTION: This function returns the From information from the
**				CommonInfo structure
**
** PARAMETERS:
**		pCommonInfo(IN)	: The CommonInfo structure 
**		ppFrom(OUT) : The From information retrieved
**		pError(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetFromInfoFromCommonInfo _ARGS_\
	((Sdf_st_commonInfo *pCommonInfo, Sdf_st_nameInfo **ppFrom, \
	 Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetToInfoFromCommonInfo
**
** DESCRIPTION: This function returns the To information from the
**				CommonInfo structure
**
** PARAMETERS:
**		pCommonInfo(IN)	: The CommonInfo structure 
**		ppTo(OUT) 		: The To information retrieved
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetToInfoFromCommonInfo _ARGS_\
	((Sdf_st_commonInfo *pCommonInfo, Sdf_st_nameInfo **ppTo, \
	Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetCommonInfoFromCallObject
**
** DESCRIPTION: This function returns the CommonInfo Structure from the
**				Call Object Element
**
** PARAMETERS:
**		pObject(IN)			: The call object structure
**		ppCommonInfo(OUT) 	: The commonInfo structure retrieved
**		pError(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetCommonInfoFromCallObject  _ARGS_\
	((Sdf_st_callObject *pObject, Sdf_st_commonInfo **ppCommonInfo, \
	Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetDestInfoFromTransaction
**
** DESCRIPTION: This function returns the Destination information from the
**				Transaction Info structure. If pOverlapTransInfo is null then
**				It will retrived from pTransaction structure otherwise from
**				pOverlapTransInfo structure
**
** PARAMETERS:
**		pTransaction(IN)		: The UAC or UAS transaction structure 
**		pOverlapTransInfo(IN)	: The OverlapTransaction Info structure 
**		ppDestInfo(OUT) 		: The Destination information retrieved
**		pError(OUT)				: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetDestInfoFromTransaction _ARGS_\
	((Sdf_st_transaction *pTransaction,Sdf_st_overlapTransInfo \
	*pOverlapTransInfo, Sdf_st_transportInfo **ppDestInfo, \
	 Sdf_st_error *pError));



/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetFromInfoFromCallObject
**
** DESCRIPTION: A higher level API that directly retrieves 
**				the From Info from the call object
**
** PARAMETERS:
**		pObject(IN)	: The CallObject 
**		ppFrom(OUT) 	: The From information retrieved
**		pError(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetFromInfoFromCallObject _ARGS_\
	((Sdf_st_callObject *pObject, Sdf_st_nameInfo **ppFrom,  \
	Sdf_st_error *pError));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetToInfoFromCallObject
**
** DESCRIPTION: A higher level API that directly retrieves 
**				the To Info from the call object
**
** PARAMETERS:
**		pObject(IN)	: The CallObject 
**		ppTo(OUT) 	: The To information retrieved
**		pError(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetToInfoFromCallObject _ARGS_\
	((Sdf_st_callObject *pObject, Sdf_st_nameInfo **ppTo, \
	Sdf_st_error *pError));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetCallInfoFromCallObject
**
** DESCRIPTION: This function retrieves the CallInfo structure from
**				the call object
**
** PARAMETERS:
**		pObject(IN)		: The CallObject
**		ppCallInfo(OUT)	: The CallInfo structure retrieved
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetCallInfoFromCallObject \
	 _ARGS_((Sdf_st_callObject *pObject, Sdf_st_callInfo **ppCallInfo, \
	Sdf_st_error *pError));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetAppDataFromCallObject
**
** DESCRIPTION: This function returns the application context data 
**				structure (AppData) from the Call Object
**
** PARAMETERS:
**		pObject(IN)		: The Call Object
**		ppAppData(OUT) 	: The AppData structure retrieved
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetAppDataFromCallObject \
	 _ARGS_((Sdf_st_callObject *pObject, Sdf_st_appData **ppAppData, \
	Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetInitDataFromCallObject
**
** DESCRIPTION: This function returns the call profile (InitData) 
**				structure from the Call Object
**
** PARAMETERS:
**		pObject(IN)		: The Call Object
**		ppInitData(OUT) : The InitData structure retrieved
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetInitDataFromCallObject \
	 _ARGS_((Sdf_st_callObject *pObject, Sdf_st_initData **ppInitData, \
	Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetInitDataInCallObject
**
** DESCRIPTION: This function sets the call profile (InitData) structure 
**				in the Call Object
**
** PARAMETERS:
**		pObject(IN)		: The CallObject
**		pInitData(IN) 	: The call profile to be set into the callObject
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetInitDataInCallObject \
	 _ARGS_((Sdf_st_callObject *pObject, Sdf_st_initData *pInitData, \
	Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetFromInfoFromInitData
**
** DESCRIPTION: This function returns the From information from the
**				call profile (InitData) structure
**
** PARAMETERS:
**		pInitData(IN) : The initData structure
**		ppFrom(OUT) : The From information retrieved
**		pError(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetFromInfoFromInitData 
 	_ARGS_((Sdf_st_initData *pInitData, Sdf_st_nameInfo **ppFrom, \
	Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetRegistrarInfoFromInitData
**
** DESCRIPTION: This function returns the Registrar information from the
**				initData structure
**
** PARAMETERS:
**		pInitData(IN)		: The initData structure
**		ppRegistrar(OUT) 	: The Registrar information retrieved
**		pError(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetRegistrarInfoFromInitData 
 	_ARGS_((Sdf_st_initData *pInitData, Sdf_st_transportInfo **ppRegistrar, \
	Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetTransportInfoFromGlobalData
**
** DESCRIPTION: This function returns the Transport Info from the
**				call profile (InitData) structure
**
** PARAMETERS:
**		pInitData(IN)   	: The InitData structure.	
**		ppTransport(OUT) 	: The Transport information retrieved
**		pError(OUT)	    	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetTransportInfoFromInitData \
	 _ARGS_((Sdf_st_initData *pInitData,Sdf_st_transportInfo **ppTransport, \
	Sdf_st_error *pError));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaCloneNameInfo
**
** DESCRIPTION: This function makes a deep copy of the NameInfo structure
**				from the source object to the destination object
**
** PARAMETERS:
**		pDestInfo(OUT)	: Destination structure to which copy is made
**		pSrcInfo(IN) 	: Source structure for the clone operation
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneNameInfo _ARGS_((Sdf_st_nameInfo \
	 *pDestInfo, Sdf_st_nameInfo *pSrcInfo, Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaCloneTransportInfo
**
** DESCRIPTION: This function makes a deep copy of the TransportInfo structure
**				from the source object to the destination object
**
** PARAMETERS:
**		pDestInfo(OUT)	: Destination structure to which copy is made
**		pSrcInfo(IN) 	: Source structure for the clone operation
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneTransportInfo 
 	_ARGS_((Sdf_st_transportInfo *pDestInfo, Sdf_st_transportInfo *pSrcInfo, \
	 Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaCloneCommonInfo
**
** DESCRIPTION: This function makes a deep copy of the CommonInfo structure
**				from the source object to the destination object
**
** PARAMETERS:
**		pDestInfo(OUT)	: Destination structure to which copy is made
**		pSrcInfo(IN) 	: Source structure for the clone operation
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneCommonInfo  _ARGS_((Sdf_st_commonInfo \
	*pDestInfo,Sdf_st_commonInfo *pSrcInfo, Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaCloneCallObject
**
** DESCRIPTION: This function makes a deep copy of the CallObject structure
**				from the source object to the destination object
**
** PARAMETERS:
**		pDestInfo(OUT)	: Destination structure to which copy is made
**		pSrcInfo(IN) 	: Source structure for the clone operation
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneCallObject \
 	_ARGS_((Sdf_st_callObject *pDestInfo, Sdf_st_callObject *pSrcInfo, \
     Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaGetHeaderListByType
**
** DESCRIPTION: This function returns a list of Sdf_st_headerList structure
**				corresponding to the given SipHeader type
**
** PARAMETERS:
**		slHeaderList(IN)			: The list of headers to be searched
**		dType(IN) 					: The type of SipHeader being searched
**		pslMatchedHeaderList(OUT)	: The result of the search. A list of
**										Sdf_st_headerList structures.
**		pError(OUT)					: Error structure in case the function fails
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetHeaderListByType \
	_ARGS_((Sdf_ty_slist slHeaderList, en_HeaderType dType,\
	Sdf_ty_slist *pslMatchedHeaderList, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaGetHeaderListByCorrelation
**
** DESCRIPTION: This function returns a list of Sdf_st_headerList structure
**				corresponding to the given correlation string
**
** PARAMETERS:
**		pslHeaderList(IN)		: The list of headers to be searched
**		pCorrelation(IN) 		: The correlation string to retrieve the 
**									matching headerList elements
**		pslMatchedHeaderList(OUT): The result of the search. A list of
**									Sdf_st_headerList structures.
**		pError(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetHeaderListByCorrelation \
 	_ARGS_((Sdf_ty_slist *pslHeaderList, Sdf_ty_s8bit *pCorrelation, 
	Sdf_ty_slist *pslMatchedHeaderList, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaGetMsgBody
**
** DESCRIPTION: This function returns a list of Sdf_st_msgBody structure
**				corresponding to the given correlation string
**
** PARAMETERS:
**		pObject(IN)                 : The Call Object from which the message
**                                    body is to be retrieved.
**		pCorrelation(IN) 			: The correlation string to retrieve the 
**										matching message body
**		pslMatchedMsgBodyList(OUT)	: The result of the search. A list of
**										Sdf_st_msgBody structures.
**		pError(OUT)					: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetMsgBody \
 	_ARGS_((Sdf_st_callObject *pObject, Sdf_ty_s8bit *pCorrelation, \
	Sdf_ty_slist *pslMatchedMsgBodyList, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaDeleteMsgBodyByCorrelation
**
** DESCRIPTION:  This function deletes the Sdf_st_msgBody structure in
**				 the list where an entry corresponding to the given
**				 correlation string is located
**
** PARAMETERS:
**		pslMsgBodyList(IN)	: The list of Sdf_st_msgBody elements to 
**								be searched
**		pCorrelation(IN) 	: The correlation string to delete the 
**								matching Sdf_st_msgBody structure
**		pError(OUT)	: 		Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaDeleteMsgBodyByCorrelation \
 	_ARGS_((Sdf_ty_slist *pslMsgBodyList, Sdf_ty_s8bit *pCorrelation, \
	Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaMoveHeaderListByType
**
** DESCRIPTION: This function moves a headerList structure identified
**				by the type from the source list to the destination list. The
**				structure is removed from the source list.
**
** PARAMETERS:
**		slSrcHeaderList(IN)		: The list from which the header is to be moved
**		slDestHeaderList(IN)	: The list to which the header is to be moved
**		dType(IN) 				: The header type of the headerList structure
**		pError(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaMoveHeaderListByType \
 	_ARGS_((Sdf_ty_slist *pslSrcHeaderList, Sdf_ty_slist *pslDestHeaderList, \
	en_HeaderType dType, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaMoveHeaderListByCorrelation
**
** DESCRIPTION: This function moves a headerList structure identified
**				by the correlation string from the source list to the 
**				destination list. The structure is removed from the source list.
**
** PARAMETERS:
**		slSrcHeaderList(IN)		: The list from which the header is to be moved
**		slDestHeaderList(IN)	: The list to which the header is to be moved
**		pCorrelation(IN) 		: The correlation string
**		pError(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaMoveHeaderListByCorrelation \
 	_ARGS_((Sdf_ty_slist *pslSrcHeaderList, Sdf_ty_slist *pslDestHeaderList, \
	Sdf_ty_s8bit *pCorrelation, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaCopyHeaderListByType
**
** DESCRIPTION: This function copies a headerList structure identified
**				by the type from the source list to the destination list.
**
** PARAMETERS:
**		slSrcHeaderList(IN)		: The list from which the header is to be
**									copied
**		slDestHeaderList(IN)	: The list to which the header is to be copied
**		dType(IN) 				: The header type of the headerList structure
**		pError(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCopyHeaderListByType \
 	_ARGS_((Sdf_ty_slist *pslSrcHeaderList, Sdf_ty_slist *pslDestHeaderList, \
	en_HeaderType dType, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaCopyHeaderListByCorrelation
**
** DESCRIPTION: This function copies a headerList structure identified
**				by the correlation string from the source list to the 
**				destination list.
**
** PARAMETERS:
**		slSrcHeaderList(IN)		: The list from which the header is to be copied
**		slDestHeaderList(IN)	: The list to which the header is to be copied
**		pCorrelation(IN) 		: The correlation string
**		pError(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCopyHeaderListByCorrelation \
 	_ARGS_((Sdf_ty_slist *pslSrcHeaderList, Sdf_ty_slist *pslDestHeaderList, \
	Sdf_ty_s8bit *pCorrelation, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaCloneHeaderList
**
** DESCRIPTION: This function makes a deep copy of the Sdf_st_headerList
**				structure from the source to the destination
**
** PARAMETERS:
**		pDestInfo(OUT)	: Destination structure to which copy is made
**		pSrcInfo(IN) 	: Source structure for the clone operation
**		pError(OUT)		: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneHeaderList \
 	_ARGS_((Sdf_st_headerList *pDestInfo, Sdf_st_headerList *pSrcInfo, \
	Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaCopyMsgBodyToPersistent
**
** DESCRIPTION: This function copies a SipMsgBody structure from the
**				source list to the destination list (PersistentMsgBodyList)
**				The correlation string to be set is passed as a parameter
**				to the function.
**
** PARAMETERS:
**		pslSrcMsgBodyList(IN)	: The list from which the msgbody is to be 
**									copied. A list of type SipMsgBody.
**		pslDestMsgBodyList(IN)	: The list to which the msgbody is to be copied.
**									A list of type Sdf_st_msgBody.
**		pCorrelation(IN) 		: The correlation string to be set for the
**									Sdf_st_msgBody
**		pErr(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCopyMsgBodyToPersistent 
 	_ARGS_((Sdf_ty_slist *pslSrcMsgBodyList, \
	Sdf_ty_slist *pslDestMsgBodyList, Sdf_ty_s8bit *pCorrelation, \
	Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaCopyMsgBodyFromPersistent
**
** DESCRIPTION: This function copies message bodies from persistent list
**				by the correlation string to the destination list
**
** PARAMETERS:
**		pslSrcMsgBodyList(IN)	: The list from which the msgbody is to 
**									be copied. A list of type Sdf_st_msgBody
**		pslDestMsgBodyList(IN)	: The list to which the msgbody is to be copied.
**									A list of type SipMsgBody.
**		pCorrelation(IN) 		: The correlation string used to search the
**									MsgBody
**		pErr(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCopyMsgBodyFromPersistent \
 	_ARGS_((Sdf_ty_slist *pslSrcMsgBodyList, \
	Sdf_ty_slist *pslDestMsgBodyList, Sdf_ty_s8bit *pCorrelation, \
	Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaCloneMsgBodyFromPersistent
**
** DESCRIPTION: This function copies message bodies from persistent list
**				by the correlation string to the destination list
**
** PARAMETERS:
**		pslSrcMsgBodyList(IN)	: The list from which the msgbody is to 
**									be copied. A list of type Sdf_st_msgBody
**		pslDestMsgBodyList(IN)	: The list to which the msgbody is to be copied.
**									A list of type SipMsgBody.
**		pCorrelation(IN) 		: The correlation string used to search the
**									MsgBody
**		pErr(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneMsgBodyFromPersistent \
 	_ARGS_((Sdf_ty_slist *pslSrcMsgBodyList, \
	Sdf_ty_slist *pslDestMsgBodyList, Sdf_ty_s8bit *pCorrelation, \
	Sdf_st_error *pErr));

	

/******************************************************************
** FUNCTION:  sdf_ivk_uaMoveMsgBodyToPersistent
**
** DESCRIPTION:  This function moves Sdf_st_msgBody structures identified
**		by the correlation string from the source list to the 
**		destination list. 
**
** PARAMETERS:
**		pslSrcMsgBodyList(IN)	: The list from which the msgbody is to 
**									be moved. A list of type SipMsgBody
**		pslDestMsgBodyList(IN)	: The list to which the msgbody is to be moved.
**									A list of type Sdf_st_msgBody
**		pCorrelation(IN) 		: The correlation string to set while inserting
**									the new	Sdf_st_msgBody elements.
**		pErr(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaMoveMsgBodyToPersistent \
 	_ARGS_((Sdf_ty_slist *pslSrcMsgBodyList, \
	Sdf_ty_slist *pslDestMsgBodyList, \
	Sdf_ty_s8bit *pCorrelation, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaMoveMsgBodyFromPersistent
**
** DESCRIPTION:  This function moves message bodies from persistent list
**		by the correlation string to the destination list
**
** PARAMETERS:
**		pslSrcMsgBodyList(IN)	: The list from which the msgbody is to be 
**									moved. A list of type Sdf_st_msgBody
**		pslDestMsgBodyList(IN)	: The list to which the msgbody is to be moved.
**									A list of type SipMsgBody.
**		pCorrelation(IN) 		: The correlation string to retrieve the 
**									Msgbodies to be moved.
**							
**		pErr(OUT)	: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaMoveMsgBodyFromPersistent \
 	_ARGS_((Sdf_ty_slist *pslSrcMsgBodyList, \
	Sdf_ty_slist *pslDestMsgBodyList,\
	Sdf_ty_s8bit *pCorrelation, Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetCallObjectKeyFromCallObject
**
** DESCRIPTION: This function returns a pointer to the key structure 
**				associated with the callObject
**
** PARAMETERS:
**		pObject(IN)				: The Call Object
**		ppCallObjectKey(OUT) 	: The call object key retrieved
**		pErr(OUT)				: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal  sdf_ivk_uaGetCallObjectKeyFromCallObject \
 	_ARGS_((Sdf_st_callObject *pObject, \
	Sdf_st_callObjectKey **ppCallObjectKey, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaCloneCallObjectKey
**
** DESCRIPTION: This function copies the 
**				call-object key structure from src to dest
**
** PARAMETERS:
**		pDestInfo(OUT)	: Destination structure to which copy is made
**		pSrcInfo(IN) 	: Source structure for the clone operation
**		pErr(OUT)		: Error structure in case the function fails
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneCallObjectKey _ARGS_\
	((Sdf_st_callObjectKey *pDestInfo, Sdf_st_callObjectKey *pSrcInfo, \
	Sdf_st_error *pErr));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaCloneOverlapTransInfo
**
** DESCRIPTION: This function copies the 
**				OverlapTransactionInfo structure from src to dest
**
** PARAMETERS:
**		pDestObj(OUT)	: Destination structure to which copy is made
**		pSrcObj(IN) 	: Source structure for the clone operation
**		pErr(OUT)		: Error structure in case the function fails
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneOverlapTransInfo _ARGS_\
	((Sdf_st_overlapTransInfo *pDestObj, Sdf_st_overlapTransInfo *pSrcObj,\
	Sdf_st_error *pErr));


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaCloneMsgBody
**
** DESCRIPTION: This function copies the 
**				Sdf_st_msgBody structure from src to dest
**
** PARAMETERS:
**		pDestObj(OUT)	: Destination structure to which copy is made
**		pSrcObj(IN) 	: Source structure for the clone operation
**		pErr(OUT)		: Error structure in case the function fails
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMsgBody \
	_ARGS_((Sdf_st_msgBody *pDestObj, Sdf_st_msgBody *pSrcObj, \
	Sdf_st_error *pErr));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaCloneConfigExtraHeaders
**
** DESCRIPTION: This function copies the 
**				the config extra headers structure from src to dest
**
** PARAMETERS:
**		pDestObj(OUT)	: Destination structure to which copy is made
**		pSrcObj(IN) 	: Source structure for the clone operation
**		pErr(OUT)		: Error structure in case the function fails
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneConfigExtraHeaders _ARGS_\
	((Sdf_st_configExtraHeaders *pDestObj, Sdf_st_configExtraHeaders *pSrcObj,
	Sdf_st_error *pErr));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaCloneInitData
**
** DESCRIPTION: This function copies the InitData structure from 
**				source to destination
**
** PARAMETERS:
**		pDestObj(OUT)	: Destination structure to which copy is made
**		pSrcObj(IN) 	: Source structure for the clone operation
**		pErr(OUT)		: Error structure in case the function fails
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneInitData _ARGS_\
	((Sdf_st_initData *pDestObj, Sdf_st_initData *pSrcObj, Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetStateMaintenance
**
** DESCRIPTION: This API returns the type of state maintenance (call/
**				transaction statefulness) set in the toolkit.
**
** PARAMETERS:
**		pStateMaintenance(OUT)	: Boolean indicating transaction/call 
**									statefulness
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetStateMaintenance _ARGS_\
	((Sdf_ty_stateMaintenance *pStateMaintenance, Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetStateMaintenance
**
** DESCRIPTION: This API sets whether call/transaction stateful behaviour
**				is to be applied if no matching call object is found for
**				the call. If this API is not invoked, call stateful is
**				assumed. In the transaction stateful mode, the UA toolkit
**				will not automatically respond to bad requests. Instead it
**				will issue the sdf_cbk_uaTransactionReceived callback to the
**				application.
**
** PARAMETERS:
**		dStateMaintenance(IN)	: Boolean indicating transaction/call 
**									statefulness
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetStateMaintenance _ARGS_\
	((Sdf_ty_stateMaintenance dStateMaintenance, Sdf_st_error *pErr));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaCloneTransaction
**
** DESCRIPTION: This function copies the 
**				Sdf_st_transaction structure from src to dest
**
** PARAMETERS:
**		pDestInfo(OUT)	: Destination structure to which copy is made
**		pSrcInfo(IN) 	: Source structure for the clone operation
**		pErr(OUT)		: Error structure in case the function fails
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCloneTransaction _ARGS_\
	((Sdf_st_transaction *pDestInfo, Sdf_st_transaction *pSrcInfo, \
	Sdf_st_error *pErr));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaGetContactInfoFromInitData
**
** DESCRIPTION: This function returns the Contact information from the
**				InitData structure
**
** PARAMETERS:
**		pInitData(IN)		: The InitData structure 
**		pContactList(OUT) 	: The Contact information retrieved
**		pError(OUT)			: Error structure in case the function fails
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetContactInfoFromInitData _ARGS_\
	((Sdf_st_initData *pInitData, Sdf_ty_slist *pContactList, \
	 Sdf_st_error *pError));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaGetCseqInCallObject
**
** DESCRIPTION: This function returns the current local Cseq number from the
**				Call Object structure
**
** PARAMETERS:
**		pCallObject(IN)		: The Call object structure 
**		pCseq(OUT) 			: The local Cseq that has been retrieved
**		pError(OUT)			: Error structure in case the function fails
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetCseqInCallObject _ARGS_\
	((Sdf_st_callObject *pCallObject, Sdf_ty_u32bit *pCseq, \
	 Sdf_st_error *pError));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaSetCseqInCallObject
**
** DESCRIPTION: This function sets the local Cseq number in Call Object 
**				to the specified value.
**
** PARAMETERS:
**		pCallObject(IN)		: The Call object structure 
**		dCseq(IN) 			: The Cseq number
**		pError(OUT)			: Error structure in case the function fails
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetCseqInCallObject _ARGS_\
	((Sdf_st_callObject *pCallObject, Sdf_ty_u32bit dCseq, \
	 Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaAddheader
 **
 ** DESCRIPTION: This function will form a header from the string passed
 **				 to this API and will insert in the header list
 **
 ******************************************************************************/
  Sdf_ty_retVal  sdf_ivk_uaAddheader _ARGS_((\
  	Sdf_ty_slist *pList, Sdf_ty_s8bit  *pHeadervalue, Sdf_st_error  *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaExtractHeaderList
 **
 ** DESCRIPTION: This function Extracts a header from the slProcessed header list
 **
 ******************************************************************************/

Sdf_ty_retVal  sdf_ivk_uaExtractHeaderList
	_ARGS_((Sdf_ty_slist *pList, en_HeaderType  dHdrType, \
	Sdf_st_headerList **ppExtractedHdrList, Sdf_st_error  *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaDeleteHeaderListByCorrelation
**
** DESCRIPTION: This function Deletes all Sdf_st_headerList structures
**				corresponding to the given correlation string
**
** PARAMETERS:
**		pslHeaderList(IN)		: The list of headers to be searched
**		pCorrelation(IN) 		: The correlation string to retrieve the 
**									matching headerList elements
**		pError(OUT)				: Error structure in case the function fails
**
******************************************************************/
 Sdf_ty_retVal sdf_ivk_uaDeleteHeaderListByCorrelation \
 	_ARGS_((Sdf_ty_slist *pslHeaderList, const Sdf_ty_s8bit *pCorrelation, 
	Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneTxnToOverLapTxn
 **
 ** DESCRIPTION: This function copies the 
 **				 UAS/UAC Transaction structure to Overlap Transaction 
 **					Structure
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneTxnToOverLapTxn	\
	_ARGS_((Sdf_st_overlapTransInfo *pDestTransaction,	\
	Sdf_st_transaction *pSrcTransaction, Sdf_st_error *pErr));

/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaRetrieveOverlapTransInfoByCseq
 **
 ** DESCRIPTION: This API retrieves an OverlapTransInfo element within the 
 ** 			 call object that corresponds to the Cseq passed.
 **
 ******************************************************************************/
Sdf_ty_retVal	sdf_ivk_uaRetrieveOverlapTransInfoByCseq _ARGS_((\
	Sdf_ty_u32bit	dCseqNum, Sdf_ty_msgMode		 dMsgMode,
	en_SipMessageType dType, Sdf_st_callObject *pCallObj,
	Sdf_st_overlapTransInfo **ppTransInfo, Sdf_st_error *pError));


#ifdef SDF_REFER
/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormCallObjectKeyFromReplacesHeader
**
** DESCRIPTION: This API forms a call-Object-Key structure with the
**		information present in the Replaces header. It fills the
**		Call-Id, From-Tag & To-Tags of the CallObjectKey structure. The 
**		other fields of key are just initialised & returned to application.
**
** PARAMETERS:
**		pSipMsg(IN)		: The SipMessage that contains Replaces header
**		ppCallObjectKey(OUT): Call-object key 
**		pErr(OUT)		: Error structure in case the function fails
**
*****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormCallObjectKeyFromReplacesHeader (SipMessage \
		*pSipMsg, Sdf_st_callObjectKey **ppCallObjectKey, Sdf_st_error *pErr);

/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormReplacesHeader
**
** DESCRIPTION: This API constructs a Replaces header. The Call-Id, From-tag
**		To-tag are taken from Call-Object, if they are not provided as
**		arguments.
**
** PARAMETERS:
**		pCallObj(IN)	: call object from which Call-id, from-tag, to-tag
**							are retrieved, if not provided as argumetns
**		pCallId(IN)		: Call-Id of call to be replaced
**		pToTag(IN)		: To-Tag of call to be replaced
**		pFromTag(IN)	: From-Tag of call to be replaced
**		pReplacesHdr(OUT): The Replaces header 
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormReplacesHeader (Sdf_st_callObject *pCallObj, \
		Sdf_ty_s8bit *pCallId, Sdf_ty_s8bit *pToTag, Sdf_ty_s8bit *pFromTag, \
		SipHeader **ppReplacesHdr, Sdf_st_error *pErr);


/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormURLStringFromReplacesHeader
**
** DESCRIPTION: This API converts Replaces header into a string. Core stack
**		API's are invoked for converting the header to string. Note that
**		special characters are escaped in the returned string.
**
** PARAMETERS:
**		pReplacesHdr(IN): Replaces header which has to be converted to string
**		ppUrlString(OUT): Escaped string equivalent of Replaces header
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormURLStringFromReplacesHeader	(SipHeader \
		*pReplacesHdr, Sdf_ty_s8bit **ppUrlString, Sdf_st_error *pErr);


/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetReplacesHeaderFromReferTo
**
** DESCRIPTION: This API will extract any Replaces Header, which is 
**		embedded as URL parameter in the Refer-To header and will 
**		return a SipReplacesHeader structure.
**
** PARAMETERS:
**		pReferToHdr(IN)	: Refer-To header which contains escaped Replaces hdr
**		pReplacesHdr(OUT):Replaces header constructed out of Refer-To
**							header param
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetReplacesHeaderFromReferTo (SipReferToHeader \
		*pReferToHdr, SipReplacesHeader **ppReplacesHdr, Sdf_st_error *pErr);

/*****************************************************************************
** FUNCTION: sdf_ivk_uaInsertReferredByHeader
**
** DESCRIPTION: This API will check the pReferredBy header parameter, 
**		if this is NULL, then It will form a default referredby header 
**		from the From Information of the InitData.
**
** PARAMETERS:
**		pCallObj(IN)		: call object from which ReferredBy header will
**								be constructed (out of FROM info), if it is
**								not specified in pReferredBy input argument.
**		pSipMsg(OUT)		: The SipMessage in which the header has to be set
**		pReferredByHdr(IN)	: Referred-BY header to be set in SipMessage
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInsertReferredByHeader (Sdf_st_callObject *pCallObj, \
		SipMessage *pSipMsg, SipHeader *pReferredByHdr, 
		Sdf_st_error *pErr);
/*****************************************************************************
 ** FUNCTION	: sdf_ivk_uaSetEarlyonlyInReplacesHdr
 **
 ** DESCRIPTION	: Inserts the "early-only" paramater in Replaces hedaer
 **				    
 **     PARAMETERS:
 **		pSipHeader(IN)	: Replaces header to which the param need to be added.
 **
 ** 	pError(OUT)		 : Error structure in case the function fails
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetEarlyonlyInReplacesHdr 
	(SipHeader 			*pReplacesHeader, 
	 Sdf_st_error 		*pError);

#endif	/* SDF_REFER */
#ifdef SDF_CONGEST
/*****************************************************************************
** FUNCTION: sdf_ivk_uaInsertRsrcPriorityHeader
**
** DESCRIPTION: This API takes Resource-Priority header in the form of a
**              string and inserts it into the passed SipMessage.
**
******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertRsrcPriorityHeader(Sdf_st_callObject
        *pCallObj, \
        SipMessage *pSipMsg,Sdf_ty_s8bit *pRsrcPriorityStr,Sdf_st_error *pErr);
 
/*****************************************************************************
** FUNCTION: sdf_ivk_uaInsertAcceptRsrcPriorityHeader
**
** DESCRIPTION: This API takes Accept-Resource-Priority header in the form of a
**              string and inserts it into the passed SipMessage.
**
******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertAcceptRsrcPriorityHeader (SipMessage *pSipMsg, \
	 Sdf_ty_s8bit *pAcceptRsrcPriorityStr, \
	 Sdf_st_error *pErr);


#endif
/******************************************************************************
** FUNCTION: sdf_ivk_uaGetUrlFromAddrSpec
**
** DESCRIPTION: This function retrieves the URL from a SIP AddrSpec.
**              It first checks if the URL scheme and calls the corresponding
**              stack API based on the scheme.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetUrlFromAddrSpec _ARGS_((SipAddrSpec  *pAddrSpec, \
	 SipUrl **ppUrl, Sdf_st_error *pError));

/******************************************************************************
** FUNCTION: sdf_ivk_uaSetUrlInAddrSpec
**
** DESCRIPTION: This function sets the URL field in SIP AddrSpec.
**              It first checks if the URL scheme and calls the corresponding
**              stack API based on the scheme.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetUrlInAddrSpec _ARGS_((SipAddrSpec  *pAddrSpec, \
	 SipUrl *pUrl, Sdf_st_error *pError));

/******************************************************************
** Function:  sdf_ivk_uaCloneEventContext
** Description: This function Makes a deep copy of the EventContext
**				structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneEventContext
_ARGS_((Sdf_st_eventContext	*pDestObj, 
Sdf_st_eventContext 	*pSrcObj, 
Sdf_st_error 			*pErr));

#ifdef SDF_TLS

/******************************************************************************
** FUNCTION: sdf_ivk_uaGetSslDataFromInitData
**
** DESCRIPTION: This function returns the SSL data from the InitData.
**
** PARAMETERS:
**		pInitData(IN)		: The InitData structure 
**		ppSslData(OUT) 		: The SSL data that has been retrieved
**		pError(OUT)			: Error structure in case the function fails
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSslDataFromInitData _ARGS_((\
	 Sdf_st_initData *pInitData, Sdf_st_sslData **ppSslData, \
	 Sdf_st_error *pError));

/******************************************************************************
** FUNCTION: sdf_ivk_uaSetSslDataInInitData
**
** DESCRIPTION: This function sets the SSL data in the InitData.
**
** PARAMETERS:
**		pInitData(IN)		: The InitData structure 
**		ppSslData(OUT) 		: The SSL data that has to be set.
**		pError(OUT)			: Error structure in case the function fails
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetSslDataInInitData _ARGS_((\
	 Sdf_st_initData *pInitData, Sdf_st_sslData *pSslData, \
	 Sdf_st_error *pError));

#endif /* SDF_TLS */
/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetDestTransportInTransaction
**
** DESCRIPTION: Sets the destination transport information (IP/port) for
**				the call. This may change during the call due to presence 
**				of Contact headers and Record-Routes.
**
** PARAMETERS:
** 		pTransaction(IN/OUT)		: Transaction Object to which the elements
**										are updated
** 		pOverlapTransInfo(IN/OUT)	: OverlaptransactionObject to which 
**										to which the elements are updated
**	 	pAddr(IN)		: Destination IP address for the call
**	 	dPort(IN)		: Destination port for the call
**	 	pScheme(IN)		: Protocol used (Eg. tcp, udp)
**	 	pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetDestTransportInTransaction _ARGS_(\
	(Sdf_st_transaction *pTransaction,Sdf_st_overlapTransInfo\
		*pOverlapTransInfo,Sdf_ty_s8bit *pAddr, Sdf_ty_u32bit\
		dPort, Sdf_ty_s8bit *pScheme, Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetDestTransport
**
** DESCRIPTION: Sets the destination transport information (IP/port) for
**				the call. This may change during the call due to presence 
**				of Contact headers and Record-Routes.
**
** PARAMETERS:
** 		pObject(IN/OUT)	: Call Object to which the elements
**							are updated
**	 	pAddr(IN)		: Destination IP address for the call
**	 	dPort(IN)		: Destination port for the call
**	 	pScheme(IN)		: Protocol used (Eg. tcp, udp)
**	 	pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetDestTransport _ARGS_(\
	(Sdf_st_callObject *pObject,Sdf_ty_s8bit *pAddr, Sdf_ty_u32bit\
		dPort, Sdf_ty_s8bit *pScheme, Sdf_st_error *pErr));

#ifdef SDF_3GPP

/*******************************************************************
** FUNCTION: sdf_ivk_uaInsertHistoryInfoHeader
**
** DESCRIPTION: This API constructs and inserts the History-Info header into the
**              Message passed.The AddrSpec and Index are used to form the header.
**
** PARAMETERS:
**		pCallObj   (IN)   :Hist-Info header will be added in the persistant
**                          hdr list of this CallObject 
**		pCallObj(IN)	: The CallObject containing the message
**		pSipMsg   (IN) :sipMessage to which the Header to added
**      pAddrSpec  (IN): AddressSpec to be used to form History-Info hdr
**      pHistoryIndex (IN): Index to be used to form the History-Info hdr
**		pErr(OUT)		: Error structure in case the function fails
**
*******************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertHistoryInfoHeader	(Sdf_st_callObject *pCallObj, \
SipMessage *pSipMsg,SipAddrSpec *pAddrSpec,Sdf_ty_s8bit *pHistoryIndex,Sdf_st_error *pErr);

Sdf_ty_retVal sdf_ivk_uaAddHistoryInfo(Sdf_st_callObject *pCallObj, \
SipMessage *pSipMsg,SipAddrSpec *pAddrSpec,Sdf_ty_s8bit *pHistoryIndex,Sdf_st_error *pErr);
Sdf_ty_retVal sdf_ivk_uaAddHeaderParamToLastHistoryInfo(SipMessage
        *pSipMsg,Sdf_ty_s8bit *pHeaderParam,Sdf_st_error *pErr);
#endif /*end of SDF_3GPP*/

#ifdef SDF_PEMEDIA
/********************************************************************
** FUNCTION: sdf_ivk_uaInsertPEarlyMediaHeader
**
** DESCRIPTION: This API constructs a p-early-Media  header and sets 
**           into the message passed.The direction is used to form the header.
**
** PARAMETERS:
**		pCallObj(IN)	: The CallObject containing the message
**      pSipMsg   (IN) :sipMessage to which the Header to added
**		pDirection (IN): direction to be used to form the Resource-Priority hdr
**		pppEarlyMediaHdr(OUT)	: The Resource-priority header to be formed
**		pErr(OUT)		: Error structure in case the function fails
**
********************************************************************/
Sdf_ty_retVal sdf_ivk_uaInsertPEarlyMediaHeader(Sdf_st_callObject
        *pCallObj,SipMessage  *pSipMsg,Sdf_ty_s8bit *pDirection,Sdf_st_error *pErr);
#endif
#ifdef SDF_CONF /* SDF_CONF */
/*****************************************************************************
 ** FUNCTION	: sdf_ivk_uaSetIsfocusInContactHeader
 **
 ** DESCRIPTION	: Inserts the "isfocus" paramater in Contact header
 **				    
 **     PARAMETERS:
 **		pSipHeader(IN)	: Contact header to which the param need to be added.
 **
 ** 	pError(OUT)		 : Error structure in case the function fails
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetIsfocusInContactHeader	(SipHeader *pContactHeader,Sdf_st_error	*pError);
#endif


#ifdef SDF_RPORT


/*****************************************************************************
** FUNCTION: sdf_ivk_uaAddViaRportParam
**
** DESCRIPTION: 
*****************************************************************************/


Sdf_ty_matchMessageType sdf_ivk_uaAddViaRportParam
	( SipMessage  	*pMsg,
	  Sdf_st_error *pErr);


/*****************************************************************************
** FUNCTION: sdf_ivk_uaRemoveViaRportParam
**
** DESCRIPTION: 
*****************************************************************************/


Sdf_ty_matchMessageType sdf_ivk_uaRemoveViaRportParam(SipMessage *pMsg, Sdf_st_error * pErr);


#endif

#ifdef SDF_PRIVACY
/*****************************************************************************
** FUNCTION: sdf_ivk_uaAddPrivacyHeader
**
** DESCRIPTION: 
*****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertPrivacyHeader _ARGS_ (\
	( Sdf_st_callObject	*pCallObject,SipMessage  	*pMsg, \
    Sdf_ty_s8bit	*pPrivacyHdrStr, Sdf_st_error *pErr));

/*****************************************************************************
** FUNCTION: sdf_ivk_uaInsertPPreferredIdHdr
**
** DESCRIPTION: 
*****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertPPreferredIdHdr _ARGS_ (\
	( Sdf_st_callObject	*pCallObject,SipMessage  	*pSipMsg,	\
    Sdf_ty_s8bit	*pDispName,	SipAddrSpec *pAddrSpec, Sdf_st_error *pErr));

#endif


#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

