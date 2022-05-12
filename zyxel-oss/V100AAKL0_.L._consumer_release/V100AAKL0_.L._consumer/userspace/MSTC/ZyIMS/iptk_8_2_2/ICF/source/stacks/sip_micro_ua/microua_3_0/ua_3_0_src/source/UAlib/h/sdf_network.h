/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/***********************************************************************
 ** FUNCTION:
 **			This file has all the network related funtions
 **
 *********************************************************************
 **
 ** FILENAME		: sdf_network.h
 **
 ** DESCRIPTION		: This file has all the network related funtions
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 27/10/00	Subhash						Creation
 ** 29/05/10    Alok Tiwari   SPR 20412     Merged EGT CSR 1-8223807
 **                                         UATK CSR 1-8290042
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/

#ifndef __SDF_NETWORK_H_
#define __SDF_NETWORK_H_

/***********************************************************************
** FUNCTION:  sdf_ivk_uaInitSocket
**
** DESCRIPTION: This function initializes a socket. It returns a sockfd
**				with the initialized socket.
**
** PARAMETERS:
**		pSocketProperties(IN)	: The structure with details for 
**									socket initialization
**		pSockfd(IN/OUT)			: The socket to be initialized
**		pErr(OUT)				: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaInitSocket _ARGS_\
	((Sdf_st_socketProperties *pSocketProperties, Sdf_ty_s32bit *pSockfd, \
	Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION: sdf_ivk_uaSendCallToPeer 
**
** DESCRIPTION: Sends the message across the network to the peer entity
** 				and updates the remote retransmission list. It also changes 
**				the call state of the call. Destination to which
**				the message has to be sent is picked from the common info's
**				destination info member (this has been filled already before
**				invokation of this API inside FormRequest or FormResponse).
**
** PARAMETERS:
**	pObject(IN)		 	: Call Object which contains the SIP message to be sent
**	pSipMsg(IN)		 	: The SipMessage to be sent out.
**
**	dOptions(IN)	 	: The options to be used for forming the text
**							message. Multiple options can be ORed. The 
**							supported. Options are:
**							SIP_OPT_SINGLE: Each header is formed in a 
**								separate line. The message will not contain 
**								comma separated headers. Cannot be used with 
**								SIP_OPT_COMMASEPARATED.
**							SIP_OPT_COMMASEPARATED: Multiple instances of the 
**								same kind of header will appear on the same 
**								header line separated by commas. Cannot be 
**								used with SIP_OPT_SINGLE.
**							SIP_OPT_SHORTFORM: Single-letter short forms will 
**								be used for headers	that have short names.
**								Cannot be used with SIP_OPT_SULLFORM.
**							SIP_OPT_FULLFORM: All headernames will appear in 
**								full. Cannot be used with SIP_OPT_SHORTFORM.
**							SIP_OPT_CLEN: A Content-Length header with the 
**								correct length is inserted in the message 
**								being formed. If the message structure has a 
**								Content-Length header, the function corrects 
**								the length field depending on size of the 
**								message body.
**							SIP_OPT_REORDERHOP: All hop to hop headers will be 
**								placed above the end to end headers.
**							SIP_OPT_AUTHCANONICAL: All headers after the 
**								Authorization header will be formed in the 
**								canonical form. This will override options 
**								SIP_OPT_SHORTFORM and SIP_OPT_COMMASEPARATED 
**								from the Authorization header onwards.
**
**		transptype(IN)	: The transport to be used for transmitting the 
**							SIP text message. Possible values are
**							SDF_NORETRANS: This option disables the stack 
**								retransmission mechanism for the message.
**		pContext(IN)	: The application may pass any information in this
**							structure through its pData (void pointer) 
**							element. This structure is passed to the 
**							application in the Time-out indicate function. 
**							If NULL is passed in this parameter, the Toolkit 
**							creates an event context structure and attaches 
**							the call Object's pAppData pointer as the 
**							context's data.
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 /* Fix for SPR 20412: CSR 1-8223807 merged (UATK CSR 1-8290042)*/
 Sdf_ty_retVal sdf_ivk_uaSendCallToPeer  _ARGS_((Sdf_st_callObject  \
		*pObject, SipMessage *pSipMsg, SipOptions dOptions, \
		Sdf_ty_s8bit transptype, Sdf_st_eventContext *pContext, \
        Sdf_ty_s8bit *headerlist,\
		Sdf_st_error *pError));


/***********************************************************************
** FUNCTION: sdf_ivk_uaSendMessage 
**
** DESCRIPTION: Forms the target address spec and changes the call state
**              assuming that the message will be successfully sent to the
**              network. This API will not try to resolve the target host.
**
** PARAMETERS:
**	pObject(IN)		 	: Call Object which contains the SIP message to be sent
**	pSipMsg(IN)		 	: The SipMessage to be sent out.
**  ppDestDetails(OUT)	: Paramaters which contains the AddressSpec or
**                        DestInfo.
**	pErr(OUT)			: Error structure in case the function fails
**
***********************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSendMessage  _ARGS_((Sdf_st_callObject  \
		*pObject, SipMessage *pSipMsg, Sdf_st_destDetails **ppDestDetails, \
		Sdf_st_error *pError));
 

/***********************************************************************
** FUNCTION:  sdf_fn_uaInitiateConnection
**
** DESCRIPTION: The function is called internally by the toolkit to initiate
**				a new TCP connection at the time of sending a fresh
**				INVITE/REGISTER/OPTIONS or if the connection was broken after
**				the previous transaction
**	
** PARAMETERS:
**		pCallObj(IN) 	: The callobject for which the connection is required
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_fn_uaInitiateConnection _ARGS_((Sdf_st_transaction \
	*pTransaction,Sdf_st_overlapTransInfo *pOverlapTransInfo,\
	Sdf_st_initData *pInitData, Sdf_st_error *pErr));



/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetFdSetToAllTransactions
**
** DESCRIPTION: This API sets the fd_set to all the transaction of the
**				call object ( including overlap transactionsa)
**	
** PARAMETERS:
**		pCallObj(IN) 	: The callobject for which the FD_SET fo all
**							the transaction to be set 
**		pFdSet(IN)		: The fd_set which will be set to all transactions
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetFdSetToAllTransactions _ARGS_((Sdf_st_callObject \
	*pCallObj,Sdf_ty_fdSet *pFdSet,Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetListOfSockFd
**
** DESCRIPTION: This API returns the List of SockFds from  the Call Object
**	
** PARAMETERS:
**		pCallObj(IN) 	: The callobject from which the List of SockFd 
**							will be retrieved 
**		pList(IN/OUT)	: The List where the SockFds will be returned
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetListOfSockFd _ARGS_((Sdf_st_callObject \
	*pCallObj,Sdf_ty_slist *pList,Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetFdIsSetSockFds
**
** DESCRIPTION: This API returns the List of SockFds where actually data has
**					has arrived by doing FD_ISSET on the SockFds of each tran
**					saction associated with the Call Object
**	
** PARAMETERS:
**		pCallObj(IN) 	: The callobject from which the List of SockFd 
**							will be retrieved 
**		pList(IN/OUT)	: The List where the  SockFds will be returned
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetFdIsSetSockFds _ARGS_((Sdf_st_callObject \
	*pCallObj,Sdf_ty_slist *pList,Sdf_ty_fdSet *pFdSet,Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaResetSockFd
**
** DESCRIPTION: This API closes the SockFd map to the particular transation
**				SocketFd and will re-initialize the SockFd to -1
**	
** PARAMETERS:
**		pCallObj(IN) 	: The callobject 
**
**		dSockFd(IN)		: The SockFd for which the mapping SockFd inside the
**							Call object to be closed
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaResetSockFd _ARGS_((Sdf_st_callObject \
	*pCallObj,Sdf_ty_socketId dSockFd,Sdf_st_error *pErr));

	
/***********************************************************************
** FUNCTION:  sdf_ivk_uaCloseAllSocketForCall
**
** DESCRIPTION: This API closes the all the socket associated with the call
**				and will re-initialize the SockFd to -1
**	
** PARAMETERS:
**		pCallObj(IN) 	: The callobject for which all the sockets assocaited 
**							to be close
**
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloseAllSocketsForCall _ARGS_((Sdf_st_callObject \
	*pCallObj,Sdf_st_error *pErr));
	



#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif

