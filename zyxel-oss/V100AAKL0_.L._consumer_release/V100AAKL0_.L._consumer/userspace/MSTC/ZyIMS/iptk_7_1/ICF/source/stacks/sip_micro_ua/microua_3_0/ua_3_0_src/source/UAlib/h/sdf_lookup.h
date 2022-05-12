/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************************
 ** FUNCTION    : This file has all the Lookup  related funtions
 **
 *********************************************************************************
 **
 ** FILENAME    : sdf_lookup.h
 **
 ** DESCRIPTION    : This file has all the Lookup related funtions
 **
 ** DATE    NAME    REFERENCE    REASON
 ** ----    ----    ---------    --------
 ** 8-JUN-05  R.Parthasarathi          Creation
 **
 **********************************************************************************
 **      COPYRIGHT , Aricent, 2006
 **********************************************************************************/
#ifdef SDF_LOOKUP

/*****************************************************************************
 ** FUNCTION:   sdf_ivk_uaHandleLookup
 **
 ** DESCRIPTION: This function has the routine to query the DNS server 
 **
 *****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaHandleLookup (
    Sdf_st_callObject         *pObject,
    Sdf_st_overlapTransInfo   *pOverlapTransaction,
    SipMessage                *pMessage, 
    Sdf_st_eventContext     *pEventContext,
	Sdf_st_error              *pErr);


/*****************************************************************************
 ** FUNCTION:   sdf_ivk_uaisNumericAddress
 **
 ** DESCRIPTION: This function has the routine to find whether the give value
 **     is IP address or Domain Name 
 **
 *****************************************************************************/
Sdf_ty_bool Sdf_ivk_uaisNumericAddress (
    Sdf_ty_s8bit           *pNumStr,
    Sdf_ty_DnsRecordType   *pOut_type,
    Sdf_st_error           *pErr);

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaFillDestInfoFromSrvRec
 **
 ** DESCRIPTION: This function has the routine to move the SRV record
 ** 			  in the DNS list to the Destinfo structure
 **	PARAMETERS:
 ** 	pCallObj(IN/OUT)       	: Call Object to be updated
 **  	pOverlapTransaction(IN) : OverlaptransactionObject to which 
 **										                the elements are updated
 **   	pslDnsRecords(IN) 		: List containing IP records 
 **		pDestInfo (OUT)     	: Fill the IP address in the pDestInfo 
 **	 	pErr(OUT)			    : Error structure in case the function fails

 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFillDestInfoFromSrvRec _ARGS_(( \
	Sdf_st_callObject      	*pCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransaction,
	Sdf_ty_slist	   		*pslDnsRecords, 
	Sdf_st_transportInfo	*pDestInfo,
	Sdf_st_error            *pErr));
/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaGetDnsSrvRecord
 **
 ** DESCRIPTION: This function selects the next DNS SRV record from the 
 **		 transaction
 **
 **	PARAMETERS:
 **   pslDnsRecords(IN)       	: DNS record list 
 **   ppDnsSrvRecord(OUT)		: First DNS Srv Record
 **		pErr(OUT)				: Error structure in case the function fails

 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetDnsSrvRecord _ARGS_((
	Sdf_ty_slist	   			*pslDnsRecords,
	Sdf_st_dnsSrvRecord      	**ppDnsSrvRecord, 
	Sdf_st_error             	*pErr));
/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaDeleteFirstDnsRecord
 **
 ** DESCRIPTION: This function delete the first DNS record from the 
 **		 transaction
 **
 **	PARAMETERS:
 **   	pslDnsRecords(IN) 		: List containing IP records 
 **	 	pErr(OUT)				: Error structure in case the function fails
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaDeleteFirstDnsRecord _ARGS_ ((Sdf_ty_slist	*pslDnsRecords,
	            Sdf_st_error    *pErr));


/*****************************************************************************
 ** FUNCTION:	  Sdf_ivk_uaUpdateDnsRecordinTransaction
 **
 ** DESCRIPTION: This function update the DNS record into the transaction
 **
 *****************************************************************************/
Sdf_ty_retVal  sdf_ivk_uaUpdateDnsRecordinTransaction (
		Sdf_st_dnsEventContext  *pDnsEventContext,
	    Sdf_ty_bool             dResult, 
		Sdf_st_error            *pErr);

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaNextDnsTransaction
 **
 ** DESCRIPTION: This function has the routine to select the next DNS record 
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaNextDnsTransaction( \
  Sdf_st_callObject       *pCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransaction,
	Sdf_st_error            *pErr);

/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaFillDestInfoFromTxnDnsRec
 **
 ** DESCRIPTION: This function has the routine to move the Dns record
 **				 in the transaction to the Destinfo structure	
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFillDestInfoFromTxnDnsRec _ARGS_(( \
  Sdf_st_callObject       *pCallObj,
  Sdf_st_overlapTransInfo *pOverlapTransaction,
  Sdf_st_error            *pErr));

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaFillDestInfoFromIpRec
 **
 ** DESCRIPTION: This function has the routine to move the IP record
 ** 			 in the list to the Destinfo structure and removes in 
 **              the list
 **	PARAMETERS:
 **   	pslDnsRecords(IN) 	: List containing IP records 
 **		pDestInfo (OUT)     : Fill the IP address in the pDestInfo
 **	 	pErr(OUT)			: Error structure in case the function fails
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFillDestInfoFromIpRec _ARGS_(( \
	Sdf_ty_slist	   		*pslDnsRecords, 
	Sdf_st_transportInfo	*pDestInfo,
	Sdf_st_error            *pErr));

 /******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaIsDnsQueryState
 **
 ** DESCRIPTION: 	
 **					
 ******************************************************************************/
Sdf_ty_bool sdf_ivk_uaIsDnsQueryState( \
  Sdf_st_callObject        *pCallObj,
	Sdf_st_overlapTransInfo  *pOverlapTransaction,
	Sdf_st_error             *pErr);

/****************************************************************************
 ** FUNCTION: sdf_cbk_uaGetIpFromNonSipUrl
 **
 ** DESCRIPTION: This Callback resolves a Non Sip URl into a valid IP and port.
 **				 It fills up the pDestInfo structure passed to it.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaGetIpFromNonSipUrl _ARGS_(( \
   Sdf_st_dnsEventContext  *pDnsEventContext, 
	 Sdf_ty_void            *pUrl,
	 Sdf_ty_UrlType         dUrlType,	
	 Sdf_st_error           *pErr));

/******************************************************************************
 ** FUNCTION:           sdf_ivk_uaGetRecordCountFromUACTransaction
 **
 ** DESCRIPTION: This function returns the IpRecord Count From UAC Transaction 
 **     
 **     PARAMETERS:
 **      pObject (IN/OUT)            : Call object to get the pslDnsRecords.
 **   dCount(IN/OUT)          : The size of the list
 **       pError(OUT)                                   : Error structure in case the function fails
 ******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaGetRecordCountFromUACTransaction(
	Sdf_st_callObject 		*pObject,
 	Sdf_ty_s32bit 			*dCount,
 	Sdf_st_error 			*pError);

/******************************************************************************
 ** FUNCTION:           sdf_ivk_uaGetRecordFromUACTransaction
 **
 ** DESCRIPTION: This function returns the IpRecord form the list of IpRecords
 **                From UAC Transaction 
 **     
 **     PARAMETERS:
 **      pObject (IN/OUT)            : Call object to get the pslDnsRecords.
 **   dTemp(IN)               : The List Index
 **   pTranspaddr(IN/OUT)     :The Transpaddr structure which is updated by the 
 **                             IP address in this API.
 **       pError(OUT)                                   : Error structure in case the function fails
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetRecordFromUACTransaction(
	Sdf_st_callObject 	*pObject,
 	Sdf_ty_s32bit 		dIndex,
	Sdf_st_DnsRecord	**ppDnsRecord,
 	Sdf_st_error 		*pError);
#ifdef SDF_ASYNC_DNS
/******************************************************************************
 ** FUNCTION: 		sdf_fn_uaQueryRespProcs
 **
 ** DESCRIPTION: 	This function performs UATK related post DNS query 
 **					procudures
 **					
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaQueryRespProcs _ARGS_(( \
	 Sdf_st_dnsEventContext 	*pDnsEventContext,
	 Sdf_st_error				*pError));

#endif /*SDF_ASYNC_DNS*/

#endif


 /******************************************************************************
 ** FUNCTION: 		sdf_fn_uaHandleLookup
 **
 ** DESCRIPTION: 	This function updates the DestInfo structure in the 
 **					callobject so that it reflects the destination IP/port
 ** 				that the outgoing message is to be sent to.
 **
 **					Notes:
 **					
 **					- honoring & stripping off the maddr param from the 
 **					  req-uri of the request message being sent out happens 
 **					  here.
 **					- honoring maddr & received params of the Via header 
 **					  (for routing responses) is also done here
 **					
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleLookup _ARGS_ ((Sdf_st_callObject *pObject,
	Sdf_st_overlapTransInfo *pOverlapTransaction,
	SipMessage				*pMessage, 
	Sdf_st_eventContext     *pEventContext,
	Sdf_st_error *pError));
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

