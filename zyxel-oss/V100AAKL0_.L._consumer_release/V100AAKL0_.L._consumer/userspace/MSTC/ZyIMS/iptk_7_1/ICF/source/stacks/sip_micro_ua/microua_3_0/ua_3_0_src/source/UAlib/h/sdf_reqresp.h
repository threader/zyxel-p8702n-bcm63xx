/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************
 ** FUNCTION:
 **             Has function declarations for forming SIP request and
 **				response messages
 ******************************************************************************
 **
 ** FILENAME:  		sdf_reqresp.h
 **
 ** DESCRIPTION:	This file contains function declarations for forming 
 **					SIP request and response messages
 **
 **  DATE        NAME                    REFERENCE           REASON
 **  ----        ----                    ---------          --------
 ** 27-Oct-00	 Anitha										Creation
 **	  			 Prashant									Updated
 **
 ******************************************************************************
 **     		COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#ifndef __SDF_REQRESP_H_
#define __SDF_REQRESP_H_

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaFormRequest
 **
 ** DESCRIPTION: Forms a SIP Request message using the SIP message in the
 **				 input Call Object as the template.
 **
 ** PARAMETERS:
 **		pMethod(IN)			 : The method (INVITE/ACK,etc) of the SIP message 
 **					  	  	   to be generated
 **		pObject(IN/OUT) 	 : The Call Object whose SIP message is used as the
 **						  	   template. The output message is overwritten on 
 **							   the CallObject's current SIP message.
 **		pOverlapTransInfo(IN): The overlapTransInfo structure in case of
 **						 	   a PRACK/COMET. This parameter indicates the 
 **							   overlapTransInfo element for which a Request 
 **							   is to be formed. In all other cases, this
 **							   parameter is NULL.
 **		pError(OUT)			 : Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFormRequest _ARGS_(( \
	const Sdf_ty_s8bit *pMethod, Sdf_st_callObject *pObject, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaUpdateDestinationInfo
 **
 ** DESCRIPTION	: This function updates the DestInfo structure in the 
 **				  callobject so that it reflects the destination IP/port
 ** 			  that the outgoing message is to be sent to
 **
 ** PARAMETERS:
 **		pObject(IN/OUT)	: The call object whose destination info is to 
 **						  be updated
 **             pMessage(IN/OUT)        : The SipMessage itself 
 **		pError(OUT)		: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaUpdateDestinationInfo 
	_ARGS_((Sdf_st_callObject  *pObject,Sdf_st_overlapTransInfo\
	*pOverlapTransInfo, SipMessage *pMessage, \
	 Sdf_st_error *pError));


 /******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaGetDestinationDetails
 **
 ** DESCRIPTION: 	This function forms the target address spec based on
 **                 presence of loose-routing and returns the same as an
 **                 output parameter.
 ** PARAMETERS:
 **		pObject(IN)	: The call object whose destination info is to 
 **						  be updated
 **		pOverlapTransInfo	: The OverlapTransaction str.
 **     pMessage(IN): The SipMessage itself 
 **                       in which the target address spec will be stored.
 **     ppDestDetails(OUT)	: The output parameter containing the AddrSpec or
 **                           the DestInfo.
 **		pError(OUT)	: Error structure in case the function fails
 **
 ******************************************************************************/ Sdf_ty_retVal  sdf_ivk_uaGetDestinationDetails
	_ARGS_((Sdf_st_callObject  *pObject,Sdf_st_overlapTransInfo\
	*pOverlapTransInfo, SipMessage *pMessage, \
	 Sdf_st_destDetails **ppDestDetails, Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaFormResponse
 **
 ** DESCRIPTION	: Forms a SIP Response message for the call object 
 **
 ** PARAMETERS:
 **		dRespCode(IN)	: The Response Code of the SIP message to
 **						  be generated
 ** 	pMethod(IN)		: The method of the request message for which 
 **						  the response is being generated
 **		pObject(IN/OUT) : The Call Object whose SIP message is used as the
 **						  template. The output message is overwritten on 
 **						  the CallObject's current	SIP message
 **		pOverlapTransInfo(IN): The overlapTransInfo structure in case of
 **						  a final response to PRACK/COMET. In all other 
 **						  cases, this parameter is NULL.
 **		dReliableResponse : Boolean indicating whether the response to be
 **							formed should be sent out reliably. This 
 **							should be set to Sdf_co_true for RPR's
 **		pError(OUT)		 : Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFormResponse _ARGS_((Sdf_ty_u16bit dRespCode, \
	const Sdf_ty_s8bit *pMethod, Sdf_st_callObject *pObject, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_ty_bool dReliableResponse,\
	Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaFormResponseMessage
 **
 ** DESCRIPTION	: This function forms a response message from the input request 
 **				  message. General Headers are copied from input msg to output 
 **				  msg. The type of response is dependent on the RespCode passed 
 **				  as a parameter
 **
 ** PARAMETERS:
 **		pInputMessage(IN)	: The input SIP message used as a template
 **		pOutputMessage(OUT) : The output SIP request message which has
 **							  been formed using pInputMessage as a template
 **		dRespCode (IN) 		: The response code
 **		pError(OUT)			: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFormResponseMessage _ARGS_( \
	(Sdf_st_initData  *pInitData, SipMessage *pInputMessage, \
	SipMessage **pOutputMessage,Sdf_ty_u16bit dRespCode, Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaAddStatusCodeSpecificHeaders
 **
 ** DESCRIPTION	: This function forms any additional headers 
 ** 			  that should be added to a response message.
 **
 ** Parameters:
 **		pCallObj(IN)   : The Call Object Structure that contains entire call
 **						 information.
 **		pInitData(IN)  : The InitData structure that contains preferences
 **						 (methods supported, etc)
 **		pMesg(IN/OUT)  : The message structure into which headers have to 
 **						 be added
 **		statuscode (IN): The Response code
 **		pErr (OUT)     : Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaAddStatusCodeSpecificHeaders \
	_ARGS_((Sdf_st_callObject *pCallObj, Sdf_st_initData *pInitData, \
	SipMessage *pMesg, Sdf_ty_u16bit statuscode, Sdf_st_error *pErr));

/* Internal functions follow */
/******************************************************************************
 ** FUNCTION	: sdf_fn_uaGetRequestHeadersFromSipMessage
 **
 ** DESCRIPTION	: This function retrives all the request headers from the 
 ** 			  input SipMessage and fills them into the Output SipMessage
 **
 ** PARAMETERS:
 **		pInputMessage(IN)	: The input SIP message used as a template
 **		ppOutputMessage(OUT): The output SIP message whose request headers 
 **							  have been filled using pInputMessage as a 
 **							  template
 **		pError(OUT)			: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGetRequestHeadersFromSipMessage \
	_ARGS_((SipMessage *pInputMessage, SipMessage **ppOutputMessage, \
	 Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION	: sdf_fn_uaGetResponseHeadersFromSipMessage
 **
 ** DESCRIPTION	: This function retrives all the response headers from the 
 ** 			  input SipMessage and fills them into the Output SipMessage
 **
 ** PARAMETERS:
 **		pInputMessage(IN)	: The input SIP message used as a template
 **		pOutputMessage(OUT) : The output SIP message whose response headers 
 **							  have been filled using pInputMessage as a 
 **							  template
 **		pError(OUT)			: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGetResponseHeadersFromSipMessage 
	_ARGS_((SipMessage *pInputMessage, SipMessage **ppOutputMessage, \
	Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION	: sdf_fn_uaGetGeneralHeadersFromSipMessage
 **
 ** DESCRIPTION	: This function retrives all the general headers from the 
 ** 			  input SipMessage and fills them into the Output SipMessage
 **
 ** PARAMETERS:
 **		pInputMessage(IN)	: The input SIP message used as a template
 **		pOutputMessage(OUT) : The output SIP message whose general headers 
 **							  have been filled using pInputMessage as a 
 **							  template
 **		pError(OUT)			: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGetGeneralHeadersFromSipMessage \
	_ARGS_((SipMessage *pInputMessage, SipMessage **ppOutputMessage, \
	Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaFormRedirectResponse
 **
 ** DESCRIPTION	: Forms a SIP Redirect Response message for the call object 
 **
 ** PARAMETERS:
 **		dRespCode(IN)	: The Response Code of the SIP message to
 **						  be generated
 ** 	pMethod(IN)		: The method of the request message for which 
 **						  the response is being generated
 **		pNewContactList(IN): List of contact headers to include in the response
 **						  being generated.
 **		pObject(IN/OUT) : The Call Object whose SIP message is used as the
 **						  template. The output message is overwritten on 
 **						  the CallObject's current	SIP message
 **		pError(OUT)		 : Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFormRedirectResponse _ARGS_((Sdf_ty_u16bit dRespCode, \
	const Sdf_ty_s8bit *pMethod, const Sdf_ty_slist *pNewContactList, \
	Sdf_st_callObject *pObject, Sdf_st_error *pError));

 
/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaInsertHeaderParam
 **
 ** DESCRIPTION	: Inserts the header paramater with name and list of values
 **				   to the specified header
 **
 ** PARAMETERS:
 **		pSipHeader(IN)	: The Sip header to which the param need to be added.
 **
 ** 	pParamName(IN)	: Name of the header param to be added
 **					
 **		pParamValuesList(IN): List of Param values corresponding to the param
 **							header param to be added.
 **		dInsertIndex(IN): Index at which header param need to be inserted. 
 **						 Optional field. Default value is 0.
 **		pError(OUT)		 : Error structure in case the function fails
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInsertHeaderParam 
	_ARGS_((SipHeader	*pSipHdr, Sdf_ty_s8bit 	*pParamName,
	Sdf_ty_slist *pParamValuesList, Sdf_ty_u32bit dInsertIndex, \
	Sdf_st_error	*pError));


/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaInsertUrlParam
 **
 ** DESCRIPTION	: Inserts the URL parameters to the Address Spec supplied.
 **
 ** PARAMETERS:
 **		pSipAddrSpec(IN) : The Sip Address Spec to which URL param need to 
 **							be added.
 **
 ** 	pParamName(IN)	: URL Param Name
 **					
 **		pParamValuesList(IN): List of URL param values.
 **
 **		dInsertIndex(IN): Index at which URL need to be inserted. Optional
 **						field. Default value is 0.
 **
 **		pError(OUT)		 : Error structure in case the function fails
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInsertUrlParam 
	_ARGS_((SipAddrSpec	*pSipAddrSpec, Sdf_ty_s8bit *pParamName,
	 Sdf_ty_slist  *pParamValuesList, Sdf_ty_u32bit	dInsertIndex,
	 Sdf_st_error  *pError));


/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaEncryptString
 **
 ** DESCRIPTION	: API is used to get the encrypted 32bit length string 
 **					for the input string. This API allocates Memory for
 **					out put string.
 **
 ** PARAMETERS:
 **		pInString(IN) : The Input String 
 **
 ** 	ppOutString(IN)	: 32bit long encrypted string. 
 **
 **		pError(OUT)		 : Error structure in case the function fails
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaEncryptString _ARGS_((Sdf_ty_s8bit *pInString,
	 Sdf_ty_s8bit	**ppOutString, Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaMakePermanentCallObj
 **
 ** DESCRIPTION	: API is used to update the temp callobject with message
 **				  details. It converts the temp call object to permanent 
 **					call object.  Application can store this permanent in its
 **					call object aggregation.
 **
 ** PARAMETERS:
 **		pCallObj(IN) : The Call Object to be converted 
 **
 ** 	pSipMsg(IN)	:  The incoming message with which the call object need
 **						to be updated.
 **		pInitData (IN): The Init Data associated with the permanent call obj.
 **		pError(OUT)		 : Error structure in case the function fails
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaMakePermanentCallObj _ARGS_( \
	(Sdf_st_callObject	*pCallObj, SipMessage	*pSipMsg, \
	 Sdf_st_initData	*pInitData, Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION:		sdf_ivk_uaFormWarningHeader
 **
 ** DESCRIPTION:	This function Forms the Warning Header. The formed 
 **					header is inserted into the processed header list of
 **					the overlap transaction or pTransaction passed to it.
 **
 *****************************************************************************/
Sdf_ty_retVal	sdf_ivk_uaFormWarningHeader
	_ARGS_((Sdf_st_callObject *pCallObj, Sdf_st_transaction	*pTransaction,\
	 Sdf_st_overlapTransInfo	*pOverlapTxn, Sdf_ty_s8bit	*pWarningTxt, \
	 Sdf_ty_u16bit dWarningCode, Sdf_st_error	*pError));

/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaMakeAddrSpecFromConstituents
 **
 ** DESCRIPTION: This function makes a SipAddrSpec structure from 
 **				 the member values passed as parameters.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeAddrSpecFromConstituents 
 	_ARGS_((SipAddrSpec **ppAddrSpec, Sdf_ty_s8bit *pAddr, \
	 Sdf_ty_u32bit dPort, Sdf_ty_s8bit *pScheme, Sdf_st_error *pError));

#endif

#ifdef __cplusplus
}
#endif 
