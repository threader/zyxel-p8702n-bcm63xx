/*********************************************************************************
 ** FUNCTION		: This file has all the Lookup Portlayer related funtions
 **
 *********************************************************************************
 **
 ** FILENAME		: sdf_lookup.c
 **
 ** DESCRIPTION		: This file has all the Lookup related funtions
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 8-JUN-05	R.Parthasarathi				Creation
 ** 7-Feb-09    Alok Tiwari                 IPTK defined functions
 **                                         added as dummy in flag ICF_UATK_PORT
 **                                         undefined
 ** 13-Mar-2009 Rajiv Kumar IPTK Rel8.1     Merging CSR-1-6452321 
 ** 08-Jun-2009 Anuradha Gupta SPR 19737     Klocworks fixing
 ** 11-Jun-2009 Ashutosh Mohan SPR 19737     Changes done for GCC 4.1 warnings removal
 ** 28-Jul-2009 Rajiv kumar    SPR 19896     Resolved stand alone compilation
 **                                          issues of UATK. 
 ** 20-Aug-2009 Anuradha Gupta SPR 19927     GCC warning fixed
 ** 26-Sep-2009 Rajiv kumar    SPR 19991     Common defination for API
 **                                          sdf_ivk_uaGetRecordCountFromUACTransaction
 **                                          and sdf_ivk_uaGetRecordFromUACTransaction
 **                                          for both EGT and SIP Leg. 
 ** 
 **********************************************************************************
 **			COPYRIGHT , Aricent, 2009
 **********************************************************************************/
#include "ALheader.h"
#include "sdf_struct.h"
#include "sdf.h"
#include "sdf_network.h"
#include "sdf_accessor.h"
#include "sdf_init.h"
#include "sdf_internal.h"
#include "sdf_reqresp.h"
#include "sdf_debug.h"
#include "sdf_trace.h"
#include "sdf_hash.h"
#include "sdf_portlayer.h"
#include "sdf_callbacks.h"
#include "sdf_txn.h"
#include "sdf_lookup.h"
#ifdef SDF_LOOKUP
#include "sdf_lookupport.h"
#ifdef UT_STUB
#include "hssua_DNS_UT_lookup.h" 
#include "hssua_DNS_UT_lookupport.h" 
#endif 

/* klocworks : NULL check before pointer access and checking for memory allocation
 * failure and returning from the function
 */ 

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaHandleLookup
 **
 ** DESCRIPTION: This function has the routine to query the DNS server 
 **
 **	PARAMETERS:
 ** 	pObject(IN/OUT)       	    : Call Object to be updated
 **   pOverlapTransaction(IN/OUT) : OverlaptransactionObject to which 
 **										                the elements are updated
 **   pMessage(IN)            		: The decoded SipMessage which is to be processed.
 **   pEventContext(IN/OUT)			  : Transaction Object to which the elements
 **																	   are updated
 **	 	pErr(OUT)										: Error structure in case the function fails

 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHandleLookup( 
	Sdf_st_callObject       *pObject,
	Sdf_st_overlapTransInfo *pOverlapTransaction,
	SipMessage              *pMessage, 
	Sdf_st_eventContext     *pEventContext,		
	Sdf_st_error            *pErr)
{
   	en_SipMessageType  dType;
	Sdf_st_transaction *pTransaction=Sdf_co_null;

sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaHandleLookup");

/* PARAM VALIDATION STARTS */
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaHandleLookup:pErr passed is Invalid");
#endif
		return Sdf_co_fail;

	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleLookup(): "
			"pObject param passed is Invalid",pErr);
#endif		
  		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleLookup(): "
			"SipMessage param passed is Invalid",pErr);
#endif
	pErr->errCode=Sdf_en_invalidParamError; 
		return Sdf_co_fail;
	}
	#endif
/* PARAM VALIDATION ENDS */
	
	/* Dns State change for the Transaction */
  if (sip_getMessageType(pMessage, &dType, (SipError*)\
		&(pErr->stkErrCode)) ==  SipFail)
 	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleLookup(): "
			"Failed to get type of SIP Message", pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
  /* Checking Type of Message */
	if(pOverlapTransaction==Sdf_co_null)
	{
		if(dType==SipMessageRequest)
		{
			Sdf_st_msgInfo			dMessageInfo;
			Sdf_ty_messageType 	   	dMsgType = Sdf_en_unknownRequest;
	
			pTransaction=pObject->pUacTransaction;

			/* Get the Message Type of the SIP message*/   
			if(Sdf_co_success != sdf_fn_uaGetMessageInfo(pMessage, &dMessageInfo,
                                                    pErr))
		 	{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uauaHandleLookup(): "
				"Failed to get message details from  SIP Message",pErr);
#endif	
				pErr->errCode=Sdf_en_headerManipulationError;
			    return Sdf_co_fail;
		    }

			dMsgType     = dMessageInfo.dMsgType;	
            if (dMsgType == Sdf_en_cancel)
            {
				pErr->errCode = Sdf_en_noUaError;
                return Sdf_co_success;
            }
			if (dMsgType == Sdf_en_ack || dMsgType == Sdf_en_bye)
			{	
				(Sdf_ty_void)sdf_listDeleteAll(&(pTransaction->slDnsRecords),pErr); 
				(Sdf_ty_void)sdf_listFree(&(pTransaction->slDnsRecords), pErr);
				if (sdf_listInit(&(pTransaction->slDnsRecords), \
					__sdf_fn_uaFreeDnsList,Sdf_co_false, pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uauaHandleLookup( ):" 
						"Failed to initalize list of DNS SRV IP list",pErr);
#endif
				}
#ifdef SDF_THREAD_SAFE		
				sdf_fn_uaLockMutex(0,  &pTransaction->dTxnDnsStateMutex, 0);  	
#endif
				pTransaction->dTxnDnsState = Sdf_en_Idle;
#ifdef SDF_THREAD_SAFE
				sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif
			}

		}
		else
				pTransaction=pObject->pUasTransaction;
		
		/* Update DNS State */

#ifdef SDF_THREAD_SAFE		
		sdf_fn_uaLockMutex(0,  &pTransaction->dTxnDnsStateMutex, 0);  	
#endif
		if (pTransaction->dTxnDnsState != Sdf_en_Idle)
		{
			pErr->errCode = Sdf_en_dnsInvState;	
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif
			return Sdf_co_fail;
		}
		pTransaction->dTxnDnsState = Sdf_en_dnsQuery;	
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif
	}	
	else
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex, 0);  	
#endif
		if (pOverlapTransaction->dTxnDnsState != Sdf_en_Idle)
		{
			pErr->errCode = Sdf_en_dnsInvState;	
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex);	
#endif		
			return Sdf_co_fail;
		}
		pOverlapTransaction->dTxnDnsState = Sdf_en_dnsQuery;	
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex);	
#endif
	}
#ifdef UT_STUB
UA_DNS_UTP_FnHandleLookup
#endif	
	if (sdf_fn_uaHandleLookup(pObject, 
	   	pOverlapTransaction, pMessage, pEventContext, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleLookup( ): "
			"Failed to update destination info in call object",pErr);
#endif
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaHandleLookup");
	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success; 
}

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaisNumericAddress
 **
 ** DESCRIPTION: This function has the routine to query the DNS server 
 **
 ** PARAMETERS:
 **   pNumStr (IN)    : The given Numeric address as Input.
 **   pOut_type(OUT)  : The type of the Record.
 **   pErr (OUT)      : Error structure in case the function fails
 *****************************************************************************/
Sdf_ty_bool Sdf_ivk_uaisNumericAddress(
Sdf_ty_s8bit *pNumStr, 
Sdf_ty_DnsRecordType *pOut_type,
Sdf_st_error *pErr)
{

	Sdf_ty_s8bit		*pStart    = Sdf_co_null;
	Sdf_ty_s8bit		*pRef      = Sdf_co_null;
	Sdf_ty_s8bit		*pScope    = Sdf_co_null;
	Sdf_ty_s8bit		dchkVal     = ':';
	Sdf_ty_s8bit		deoAddr     = '\0';
	Sdf_ty_bool	  	bipv6       = Sdf_co_false; /* Bool true if IPv6 */
	Sdf_ty_u32bit		dlen        = 0;
	Sdf_ty_u32bit		dhCount     = 0;
	Sdf_ty_u32bit		dhdColon    = 0;
	Sdf_ty_u32bit		dhdFound    = 0;
	Sdf_ty_u32bit		dchkNo      = ALPHANUMERIC_ONLY;
	Sdf_ty_u32bit		doctetVal   = 0;
	Sdf_ty_u32bit		dvalidOctet = Sdf_co_false;
	Sdf_ty_u32bit		dspecial    = 0;  /* for IPv4 Mapped IPv6 Address handling */
	Sdf_ty_u32bit		dnumDots = 0, doctetCount = 0;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaisNumericAddress");
  /* PARAM VALIDATION STARTS */ 
#ifdef SDF_PARAMVALIDATION
if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"Sdf_ivk_uaisNumericAddress() : pErr passed is Invalid");
#endif
		return Sdf_co_false;
	}
	if(Sdf_co_null == pOut_type)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaisNumericAddress() : pOut_type passed is Invalid");
#endif
		return Sdf_co_false;
	}

	if( (Sdf_co_null == pNumStr) || ('\0' == *pNumStr) )
	{
		*pOut_type = Sdf_en_InvalidAddr;
		return Sdf_co_false;
  }

	#endif 
  /* PARAM VALIDATION ENDS */

	/* *pOut_type = 	Sdf_en_SRVrecord; */
	/* Check for special IP addresses. */
	if ((0 == Sdf_mc_strcmp(pNumStr,(Sdf_ty_s8bit *) "0.0.0.0")) || \
      (0 == Sdf_mc_strcmp(pNumStr,(Sdf_ty_s8bit *) "255.255.255.255")) || \
      (0 == Sdf_mc_strcmp(pNumStr,(Sdf_ty_s8bit *) "[::]")))
	{
		*pOut_type = Sdf_en_InvalidAddr;
		return Sdf_co_false;
  }

	if(Sdf_co_null != Sdf_mc_strchr(pNumStr, ':'))
		bipv6 = Sdf_co_true;	/* Means the given address is IPv6 */
	else if (pNumStr[0] > '9')
	{
		/* *pOut_type = 	Sdf_en_SRVrecord; */
		return Sdf_co_false;
	}
	else
		bipv6 = Sdf_co_false;	/* Means the given address is IPv4 */
		
	dlen = Sdf_mc_strlen(pNumStr);
	dlen--;	/*For the use of indexing as like array*/

	if(*pNumStr == '[')
		deoAddr = ']';
	else
		deoAddr = '\0';

	/*Validation of Input parameter.*/
	
	if(bipv6 == Sdf_co_true)
	{
		/* IPv6 Verification Starting */
		/*Checking for proper opening and closing Brackets  */
		if(*pNumStr == '[' && \
		   	(*(pNumStr + dlen) != ']' || *(pNumStr + dlen - 1) == ']'))
		{
			*pOut_type = Sdf_en_InvalidAddr;
			return Sdf_co_false;
		}
	
		if (*pNumStr == ':' && *(pNumStr+1) != ':')
		{
			*pOut_type = Sdf_en_InvalidAddr;
			return Sdf_co_false;
		}


		/* Assign Starting reference */
		if(*pNumStr == '[')
			pRef = pStart = (Sdf_ty_s8bit*)pNumStr + 1;
		else
			pRef = pStart = (Sdf_ty_s8bit*)pNumStr;

		if( (pScope = (Sdf_ty_s8bit *) Sdf_mc_strchr(pNumStr,'.')) != Sdf_co_null)
		{
			/* set flag to handle Ipv4Mapped IPv6 Address */
			dspecial = 1;
		}
	
		if( (pScope = (Sdf_ty_s8bit *) Sdf_mc_strstr(pNumStr,"::")) != Sdf_co_null)
		{
			pScope+=2; /*To Skip the Scope resolution operator */

			if( (Sdf_ty_s8bit *) Sdf_mc_strstr(pScope,"::") != Sdf_co_null)
			{
				*pOut_type = Sdf_en_InvalidAddr;
				return Sdf_co_false;
			}
			else if(*pScope == ':')
			{
				*pOut_type = Sdf_en_InvalidAddr;
				return Sdf_co_false;
			}
			else if(*pScope == deoAddr)
			{
				if (dspecial == 1)
				{
					*pOut_type = Sdf_en_InvalidAddr;
					return Sdf_co_false;
				}
			}
		}

		/* Initialize to zero  */
		dlen = 0;

		/* check for 7 blocks  */
		do
		{
			dvalidOctet = Sdf_co_false;
			doctetVal = 0;

			if(*pStart == ':')
			{
				dhdFound = 1;
				dchkVal = ':';
				dchkNo = ALPHANUMERIC_ONLY;

				if(*pStart != deoAddr && *(pStart+1) == ':')
				{
					pStart++;
					/* Indicates there is a scope resolution */
					dhdColon++;
				}
				else if(*(pStart+1) == deoAddr) 
				{
					*pOut_type = Sdf_en_InvalidAddr;
					return Sdf_co_false;
				}
				else
				{
					/*Counting no. of colons */
					dhCount++;
				}
			}
				
			else if(*pStart == '.' && *pStart != deoAddr)
			{
				dhdFound = 1;
				dchkVal = '.';
				dchkNo = NUMERIC_ONLY;
				if(*(pStart+1) == deoAddr) 
				{
					*pOut_type = Sdf_en_InvalidAddr;
					return Sdf_co_false;
				}
				else
				{
					/*Counting no. of dots */
					dnumDots++;
				}
			}

			if(dhdFound == 1)
			{
				if(dchkNo == NUMERIC_ONLY)
				{
					if(dlen > 3)
					{
						*pOut_type = Sdf_en_InvalidAddr;
						return Sdf_co_false;
					}
					while ( ((*pRef) >= '0') && \
						((*pRef) <= '9') && (dlen-- > 0) )
					{
						doctetVal = (10 * doctetVal) + ((*pRef)-'0');
						pRef++;
					/* To Handle the cases like x.x..x as IP address */
						if(dlen ==0)
							dvalidOctet = Sdf_co_true;
					}

					/* Handle the cases like  x.x..x  as IP address */
					if (Sdf_co_false == dvalidOctet )
					{
						*pOut_type = Sdf_en_InvalidAddr;
						return Sdf_co_false;
					}

					/* Check whether octet value is less than 256 */
					if(doctetVal > 256)
					{
						*pOut_type = Sdf_en_InvalidAddr;
						return Sdf_co_false;
					}
				}
				else
				{
					if(dspecial == 0)
					{
						if(dlen > 4)
						{
							*pOut_type = Sdf_en_InvalidAddr;
							return Sdf_co_false;
						}
						while( dlen-- > 0)
						{
							if(!((*pRef >= 'a' && *pRef <= 'f') || \
								(*pRef >= 'A' && *pRef <= 'F') || \
								(*pRef >= '0' && *pRef <= '9')) \
								 )
							{
							*pOut_type = Sdf_en_InvalidAddr;
							return Sdf_co_false;
							}
							pRef++;
						}
					}
					else
					{
						while(dlen-- > 0)
						{
							if(dchkVal == ':')
							{
								if(!((*pRef >= 'a' && *pRef <= 'f') || \
								(*pRef >= 'A' && *pRef <= 'F') || \
								(*pRef >= '0' && *pRef <= '9')) || dlen >= 4 \
								 )
								{
									*pOut_type = Sdf_en_InvalidAddr;
									return Sdf_co_false;
								}
							}
							pRef++;
						}
					}
				}

				/* reset count */
				dlen = 0;
				/* Skip Colon or dot */
				if(*pRef != deoAddr)
					pRef = pStart + 1;
				dhdFound = 0;
			}
			else
				dlen++;
			if(*pStart == deoAddr)
			{
				dlen = 0;
				dlen = pStart - pRef;
				dhdFound = 1;
			}
			else
				pStart++;
		} while(*pRef != deoAddr);

		if (dnumDots == 3)
		{
			if(dhdColon == 1 && dhCount < 5)
			{
				*pOut_type = Sdf_en_IPv6Address;
				return Sdf_co_true;
			}
			else if (dhdColon == 0 && dhCount == 6)
			{
				*pOut_type = Sdf_en_IPv6Address;
				return Sdf_co_true;
			}
			else
			{
				*pOut_type = Sdf_en_InvalidAddr;
				return Sdf_co_false;
			}
		}
		else if(dhdColon == 0 && dhCount == 7)
		{
			*pOut_type = Sdf_en_IPv6Address;
			return Sdf_co_true;
		}
		else if(dhdColon == 1 && dhCount < 5)
		{
			*pOut_type = Sdf_en_IPv6Address;
			return Sdf_co_true;
		}
		else
		{
			*pOut_type = Sdf_en_InvalidAddr;
			return Sdf_co_false;
		}
	}
	else
	{
		/* IPv4 Address Verification */

		/* 
 		* Loop over all the characters of the input string. Check if 
 		* the characters are numeric or not.
 		*/
		while( '\0' != *pNumStr )
		{
			dvalidOctet = Sdf_co_false;
			doctetVal = 0;
			while ( ((*pNumStr) >= '0') && ((*pNumStr) <= '9') )
			{
				doctetVal = (10 * doctetVal) + ((*pNumStr)-'0');
				pNumStr++;
				/* To Handle the cases like  x.x..x  as IP address */
				dvalidOctet = Sdf_co_true;
			}
			/* Handle the cases like  x.x..x  as IP address */
			if (Sdf_co_false == dvalidOctet )
			{
                /*CSR-1-5666184 changes start*/
				/**pOut_type = Sdf_en_InvalidAddr;*/
                /*CSR-1-5666184 changes end*/
				return Sdf_co_false;
			}
			/* If valid Octet increment the octet count	*/
			doctetCount++;	
			if( ('.' == *pNumStr) && (doctetVal < 256) )
			{
				dnumDots++;
				pNumStr++;
				continue;
			}
			
			else
			{
				break;
			}
		}
   		if( (dnumDots != 3) || ('\0' != *pNumStr) || \
			   	(doctetVal > 255) || (doctetCount != 4))
		{
			/*
			 * Need exactly 3 dots and should not have terminated 
			 * prematurely and last octet should be ok (if parsed)
			 */
            /*CSR-1-5666184 changes start*/
            /**pOut_type = Sdf_en_InvalidAddr;*/
            /*CSR-1-5666184 changes end*/
			return Sdf_co_false;
		}
   		else
		{
			/* All characters in the string are numeric. return true.*/
			*pOut_type = Sdf_en_IPv4Address;
		}
	}
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaisNumsericAddress");
	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_true; 
}

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaGetDnsSrvRecord
 **
 ** DESCRIPTION: This function selects the next DNS SRV record from the 
 **		 transaction
 **
 **	PARAMETERS:
 **   pslDnsRecords(IN)       	:  DNS record list 
 **   ppDnsSrvRecord(OUT)		: Next DNS Srv Record
 **	 	pErr(OUT)				: Error structure in case the function fails

 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetDnsSrvRecord (
	Sdf_ty_slist	   			*pslDnsRecords,
	Sdf_st_dnsSrvRecord      	**ppDnsSrvRecord, 
	Sdf_st_error             	*pErr)
{
	Sdf_ty_u32bit    		dSrvCount;
	Sdf_st_DnsRecord	  	*pDnsRec = Sdf_co_null;		

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG-Entering " 
	   "sdf_ivk_uaDnsSrvRecordFromTransaction");
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetDnsSrvRecordFromTransaction(): pErr passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(ppDnsSrvRecord == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetDnsSrvRecordFromTransaction(): "
			"ppDnsSrvRecord param passed is Invalid",pErr);
#endif
		return Sdf_co_fail;
	}
	
	if (pslDnsRecords == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetDnsSrvRecordFromTransaction(): "
			"pslDnsRecords param passed is Invalid",pErr);
#endif
		return Sdf_co_fail;
	}

#endif
	/* Getting the First SRV record */
	(Sdf_ty_void)sdf_listSizeOf (pslDnsRecords, &dSrvCount, pErr);

	if (dSrvCount == 0)
	{
		pErr->errCode = Sdf_en_noMoreDNSRecord;
		return Sdf_co_fail;
	}
	(Sdf_ty_void)sdf_listGetAt(pslDnsRecords, 0, (Sdf_ty_pvoid *)&pDnsRec, \
	                      pErr);
	
	if (pDnsRec->dDnsRecordType != Sdf_en_SRVrecord)
	{      
		pErr->errCode = Sdf_en_invalidDNSrecord	;
		return Sdf_co_fail;
	}   
	
	*ppDnsSrvRecord = (Sdf_st_dnsSrvRecord *)pDnsRec->pValue;
	 
/*	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
	   "Exiting sdf_ivk_uaGetDnsSrvRecordFromTransaction"); */
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaNextDnsTransaction
 **
 ** DESCRIPTION: This function has the routine to select the next DNS record 
 **
 **	PARAMETERS:
 ** 	pCallObj(IN/OUT)       	: Call object into which the decoded message is set.
 **   pOverlapTransaction(IN) : OverlapTransaction Object to which the elements
 **															are updated
 **	 	pErr(OUT)								: Error structure in case the function fails

 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaNextDnsTransaction( \
	Sdf_st_callObject       *pCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransaction,
	Sdf_st_error            *pErr)
{

	Sdf_ty_pvoid			pContext        = Sdf_co_null;
	Sdf_st_transportInfo	*pDestInfo      = Sdf_co_null;
	Sdf_ty_s8bit			*pLocalBranch   = Sdf_co_null;
	Sdf_ty_s8bit			*pViaBranch     = Sdf_co_null;
    Sdf_ty_u32bit			dTagCount		=	0;
#ifdef UT_STUB	
UA_DNS_UTP_FillDestInfoFromTxnDnsRec
#endif	
sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG-Entering" 
	   "sdf_ivk_uaNextDnsTransaction");

	/* PARAMETER VALIDATION STARTS */
#ifdef SDF_PARAMVALIDATION
if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
	        sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaNextDnsTransaction(): pErr passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaNextDnsTransaction(): "
			"pCallObject param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	
	}
	#endif
	/* PARAMETER VALIDATION ENDS */
	/*   Fix for removing the Tag value in the  TO header starts */
    
	if (pCallObj->pCallInfo->dState == Sdf_en_idle) 
    {
        if((pCallObj)->pUacTransaction != Sdf_co_null)
		{
			(void)sip_getTagCountFromToHdr(pCallObj->pCommonInfo->pTo,
                                         &dTagCount, \
                                         (SipError *)&(pErr->stkErrCode));
          if (dTagCount != 0)
          {
            if(sip_deleteTagAtIndexInToHdr ( \
                  (pCallObj)->pCommonInfo->pTo,\
                  0, (SipError *)&(pErr->stkErrCode)) == SipFail )
              return Sdf_co_fail;
          }

	    }
    }
    /*	 End of Fix for removing the Tag value in the  TO header */

	if (sdf_fn_uaFillDestInfoFromTxnDnsRec (pCallObj, pOverlapTransaction, 
									 pErr) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}
	
	/* Abort the transaction */
	if (pOverlapTransaction != Sdf_co_null)
	{
		/* Overlapped Transaction will be removed inside the Abort
		   Transaction. To avoid the removal, the overlap Transaction	
		   is again added in the list */
		HSS_LOCKEDINCREF(pOverlapTransaction->dRefCount);
		(Sdf_ty_void)sdf_ivk_uaAbortTransaction(pCallObj, \
			pOverlapTransaction->dTxnId, pContext, pErr);
		if (sdf_listAppend(&(pCallObj->slOverlapTransInfo), \
				pOverlapTransaction,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaNextDnsTransaction(): "
					"Failed to insert OverlapTransInfo header into the"
					"Call Object list structure", pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
		}
	}
	else
	{
		if (pCallObj->pUacTransaction != Sdf_co_null)
		{
			(Sdf_ty_void)sdf_ivk_uaAbortTransaction(pCallObj,
				pCallObj->pUacTransaction->dTxnId, pContext,pErr) ;
		}
		else 
		{ 
			(Sdf_ty_void)sdf_ivk_uaAbortTransaction(pCallObj, 
				pCallObj->pUasTransaction->dTxnId, pContext, pErr);
		}
	}

	/* Create the new branch for the transaction */
	if(sdf_fn_uaGenerateViaBranch(pDestInfo, &pLocalBranch) != Sdf_co_success)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaNextDnsTransaction( ): "
			"Failed to Generate Via Branch",pErr);
#endif
		return Sdf_co_fail;
	}
	

	/* Add Mazic Cookie to the Via branch */
	pViaBranch = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_callHandlerMemId, \
			Sdf_mc_strlen(pLocalBranch) + 1 + \
			Sdf_mc_strlen(Sdf_co_viaMagicCookie), \
			(Sdf_st_error *)Sdf_co_null);

    if(Sdf_co_null == pViaBranch)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaNextDnsTransaction( ): "
				"pViaBranch : memory allocation failure",pErr);
#endif

		pErr->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}
	
	(Sdf_ty_void)sdf_fn_uaSprintf(pViaBranch, "%s%s", Sdf_co_viaMagicCookie,	\
					         pLocalBranch);

	(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,  \
			(Sdf_ty_pvoid *)&(pLocalBranch), Sdf_co_null);

	if(pCallObj->pUacTransaction != Sdf_co_null)
	{
		if( pCallObj->pCommonInfo->pKey->pLocalViaBranch != Sdf_co_null)
			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *) \
				&(pCallObj->pCommonInfo->pKey->pLocalViaBranch), \
				Sdf_co_null);

		pCallObj->pCommonInfo->pKey->pLocalViaBranch = \
					pViaBranch;
	}
	else if (pOverlapTransaction != Sdf_co_null )
	{
		if( pOverlapTransaction->pOverlapViaBranch != Sdf_co_null )
			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *) \
				&(pOverlapTransaction->pOverlapViaBranch), Sdf_co_null);

		pOverlapTransaction->pOverlapViaBranch = \
				pViaBranch;
	}

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
	   "Exiting sdf_ivk_uaNextDnsTransaction");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
	
}



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
Sdf_ty_retVal sdf_ivk_uaFillDestInfoFromIpRec( \
	Sdf_ty_slist	   		*pslDnsRecords, 
	Sdf_st_transportInfo	*pDestInfo,
	Sdf_st_error            *pErr)
{
	Sdf_ty_u32bit    		dDnsCount;
	Sdf_st_DnsRecord	  	*pDnsRec = Sdf_co_null;		

	/* PARAMETER VALIDATION STARTS */
#ifdef SDF_PARAMVALIDATION
if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
	        sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaFillDestInfoFromIpRec(): pErr passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if(pDestInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFillDestInfoFromIpRec(): "
			"pDestInfo param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	
	}
	if(pslDnsRecords == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFillDestInfoFromIpRec(): "
			"pslDnsRecords param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	
	}

	#endif
	/* PARAMETER VALIDATION ENDS */

	/* Getting the First A/AAAA record */
	(Sdf_ty_void)sdf_listSizeOf (pslDnsRecords, &dDnsCount, pErr);

	if (dDnsCount == 0)
	{
		pErr->errCode = Sdf_en_noMoreDNSRecord;
		return Sdf_co_fail;
	}

	(Sdf_ty_void)sdf_listGetAt(pslDnsRecords, 0, (Sdf_ty_pvoid *)&pDnsRec, pErr);

	if (!(pDnsRec->dDnsRecordType == Sdf_en_IPv4Address ||
		pDnsRec->dDnsRecordType == Sdf_en_IPv6Address ||
		pDnsRec->dDnsRecordType == Sdf_en_A6record))
	{
		pErr->errCode = Sdf_en_invalidDNSrecord;
		return Sdf_co_fail;
	}
	
	if (pDestInfo->pIp != Sdf_co_null)
	    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
		    (pDestInfo->pIp),pErr);

	/* Merge: IPTK CSR 1-6452321: IPTK SPR 19216: UATK CSR 1-6589034 */
	/* Sdf_mc_strdup(pDestInfo->pIp, (Sdf_ty_s8bit *)pDnsRec->pValue);*/
	pDestInfo->pIp = sdf_memget(0,IPADDRSTRING+sizeof(int)+1,pErr);
    if(Sdf_co_null == pDestInfo->pIp)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFillDestInfoFromIpRec(): "
			"pDestInfo->pIp - memory allocation failure",pErr);
#endif
		pErr->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
    }
	
	memcpy(pDestInfo->pIp,pDnsRec->pValue,IPADDRSTRING+sizeof(int)+1);
    
	if (sdf_listDeleteAt(pslDnsRecords, 0, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_destinationInfoError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaNextDnsIpTransaction():"
			"Failed to Delete the record",pErr);
#endif
	   (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
			    (pDestInfo->pIp),pErr);
		pErr->errCode = Sdf_en_noMoreDNSRecord;
		return Sdf_co_fail;
	}

	return Sdf_co_success;
}

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
Sdf_ty_retVal sdf_ivk_uaFillDestInfoFromSrvRec( \
	Sdf_st_callObject      	*pCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransaction,
	Sdf_ty_slist	   		*pslDnsRecords, 
	Sdf_st_transportInfo	*pDestInfo,
	Sdf_st_error            *pErr)
{
	Sdf_st_dnsSrvRecord 	*pDnsSrvRecord  = Sdf_co_null;
	Sdf_st_transaction 		*pTransaction   = Sdf_co_null;
	Sdf_ty_s8bit 			*pIpRecord      = Sdf_co_null;
	/* un used variable removed*/
	/*Sdf_ty_u32bit 			dCurrentTime    = 0; */ 
	Sdf_ty_u32bit 			dSize		    = 0;
	SipMessage				*pMessage;
	Sdf_st_destDetails 		*pDestDetails = Sdf_co_null;
#if 0    
	SipAddrSpec        		*pAddrSpec=Sdf_co_null;
	SipUrl					*pUrl = Sdf_co_null;
	SipReqLine      		*pReqLine = Sdf_co_null;
#endif

	/* PARAMETER VALIDATION STARTS */
#ifdef SDF_PARAMVALIDATION
if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
	        sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaFillDestInfoFromSrvRec(): pErr passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if(pDestInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFillDestInfoFromSrvRec(): "
			"pDestInfo param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	
	}
	if(pslDnsRecords == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFillDestInfoFromSrvRec(): "
			"pslDnsRecords param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	
	}
if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFillDestInfoFromSrvRec(): "
			"pCallObj param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	
	}

	#endif
	/* PARAMETER VALIDATION ENDS */

/* Get the first SRV record  */
	if (sdf_ivk_uaGetDnsSrvRecord(pslDnsRecords, &pDnsSrvRecord, \
		pErr) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}
	

	/* Getting the First A/AAAA  record */
	if (sdf_listGetAt(&pDnsSrvRecord->slIpRecords, 0, (Sdf_ty_pvoid *)&pIpRecord, \
			pErr) == Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_noMoreDNSRecord;
		return Sdf_co_fail;
	}

	if (pDestInfo->pIp != Sdf_co_null)
	    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
		    (pDestInfo->pIp),pErr);	

	/* Merge: IPTK CSR 1-6452321: IPTK SPR 19216: UATK CSR 1-6589034 */
	/* Sdf_mc_strdup(pDestInfo->pIp,pIpRecord);*/
	pDestInfo->pIp = sdf_memget(0,IPADDRSTRING + sizeof(int) + 1 ,pErr);
	if(Sdf_co_null == pDestInfo->pIp)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFillDestInfoFromSrvRec(): "
			"pDestInfo->pIp - memory allocation failure",pErr);
#endif
		pErr->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
    }

	memcpy(pDestInfo->pIp,pIpRecord, IPADDRSTRING + sizeof(int) + 1 );
    
    pDestInfo->dPort	= pDnsSrvRecord->dPort;

	if (pOverlapTransaction == Sdf_co_null)
	{
		if (pCallObj->pUacTransaction != Sdf_co_null)
			pTransaction = pCallObj->pUacTransaction;
		else
			pTransaction = pCallObj->pUasTransaction;		

		pMessage = pTransaction->pSipMsg;
	}
	else
		pMessage = pOverlapTransaction->pSipMsg;

	if (sdf_ivk_uaGetDestinationDetails(pCallObj, pOverlapTransaction,\
					pMessage, &pDestDetails, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
	    sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
		    (Sdf_ty_s8bit *)"sdf_fn_uaFillDestInfoFromTxnSrvRec: "
		    "Failed to get the target address spec.", pErr);
#endif
		(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
		(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
			    (pDestInfo->pIp),pErr);
				return Sdf_co_fail;
    }
/*  Fix for spr 18998 */
/*  CSR 1-6367020  change start */
#if 0
    if (pDestDetails->dIsTargetUriSet == Sdf_co_true)
        pAddrSpec = pDestDetails->u.pAddrSpec;
	if((pAddrSpec != Sdf_co_null) && (pAddrSpec->dType != SipAddrReqUri))
    {
		if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pUrl,\
    				pErr) == Sdf_co_fail)
	    {
#ifdef SDF_ERROR					
    		sdf_fn_setError(Sdf_en_minorErrors,\
		   		Sdf_en_destinationInfoError,\
   				(Sdf_ty_s8bit *)"sdf_fn_uaFillDestInfoFromTxnSrvRec( ): "
   				"Failed to get Url From Addr Spec",pErr);
#endif					
	    	pErr->errCode=Sdf_en_destinationInfoError;
			(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
	   		(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
			    (pDestInfo->pIp),pErr);
		    return Sdf_co_fail;
	    }

		if (sip_setPortInUrl(pUrl,(Sdf_ty_u16bit)pDnsSrvRecord->dPort, \
		    (SipError *)&(pErr->stkErrCode)) == SipFail)
	    {
			sip_freeSipUrl(pUrl);
	   		(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
			    (pDestInfo->pIp),pErr);
			(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
	    	pErr->errCode=Sdf_en_destinationInfoError;
			return Sdf_co_fail;	
    	}

		if(sip_getReqLineFromSipReqMsg(pMessage, &pReqLine, \
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaFillDestInfoFromTxnSrvRec( ):"
				"Failed to get Request Line from Sip Message",pErr);
#endif
			sip_freeSipUrl(pUrl);
	   		(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
			    (pDestInfo->pIp),pErr);
			(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
                if (sip_setAddrSpecInReqLine(pReqLine,pAddrSpec,(SipError *) &(\
			pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
				"Failed to set AddrSpec in Request Line", pErr);
#endif		
			pErr->errCode=Sdf_en_formRequestError;
	   		(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
			    (pDestInfo->pIp),pErr);
			(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
			sip_freeSipAddrSpec(pAddrSpec);
			sip_freeSipUrl(pUrl);
			sip_freeSipReqLine(pReqLine);	
			return Sdf_co_fail;
		}
		sip_freeSipUrl(pUrl);
		sip_freeSipReqLine(pReqLine);	
	}
#endif
/*  CSR 1-6367020 change End */
	(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);

    /*Removing First A/AAAA record as this is already used */ 
	if (sdf_listDeleteAt(&(pDnsSrvRecord->slIpRecords), 0, pErr) == \
			Sdf_co_fail)
	{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
				"Failed to set AddrSpec in Request Line", pErr);
#endif		
		pErr->errCode = Sdf_en_noMoreDNSRecord;
		return Sdf_co_fail;	
	}
	(Sdf_ty_void)sdf_listSizeOf(&pDnsSrvRecord->slIpRecords,&dSize,pErr);

	if (dSize < 1)
	{
		 /*Delete the corresponding SRV record the list   */
		if (sdf_listDeleteAt(pslDnsRecords, 0, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_fn_uaFillDestInfoFromTxnSrvRec( ): "
				"Failed to  Delete Dns Record From Transaction ", pErr);
#endif		
		}
	} 
	pErr->errCode = Sdf_en_noUaError;	
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaFillDestInfoFromTxnDnsRec
 **
 ** DESCRIPTION: This function has the routine to select the next DNS record 
 **
 **	PARAMETERS:
 ** 	pCallObj(IN/OUT)       	: Call object into which the decoded message is set.
 **   pOverlapTransaction(IN) : OverlapTransaction Object to which the elements
 **															are updated
 **	 	pErr(OUT)								: Error structure in case the function fails
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFillDestInfoFromTxnDnsRec( \
  Sdf_st_callObject       *pCallObj,
  Sdf_st_overlapTransInfo *pOverlapTransaction,
  Sdf_st_error            *pErr)
{
	Sdf_ty_DnsRecordType     dDnsRecordType  = Sdf_en_unKnownAddr;
	Sdf_st_transportInfo	*pDestInfo      = Sdf_co_null; 
	Sdf_st_transaction 		*pTransaction   = Sdf_co_null;
	Sdf_ty_slist	   		*pslDnsRecords  = Sdf_co_null;

	/* Check whether DestInfo needs to be filled or not . Also set the
	   DNS Recordtype, getting the list of the DNS records  */
	if(pOverlapTransaction==Sdf_co_null)
	{
		if (pCallObj->pUacTransaction != Sdf_co_null)
			pTransaction = pCallObj->pUacTransaction;
		else
			pTransaction = pCallObj->pUasTransaction;	

		if(pTransaction == Sdf_co_null)
		{
			pErr->errCode = Sdf_en_noMoreDNSRecord;
		  	return Sdf_co_fail;
		}

#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0,  &pTransaction->dTxnDnsStateMutex, 0);  	
#endif
		if (pTransaction->dTxnDnsState == Sdf_en_dnsFilled || 
			pTransaction->dTxnDnsState == Sdf_en_dnsFirstRecordFilled)
		{
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif
			return Sdf_co_fail;
		}
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif
		dDnsRecordType = pTransaction->dDnsRecordType;
		pslDnsRecords = &(pTransaction->slDnsRecords);
	}	
	else
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex, 0);  	
#endif

		if (pOverlapTransaction->dTxnDnsState == Sdf_en_dnsFilled || 
			pOverlapTransaction->dTxnDnsState == Sdf_en_dnsFirstRecordFilled)
		{
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex);	
#endif
			return Sdf_co_fail;
		}
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex);	
#endif
		dDnsRecordType = pOverlapTransaction->dDnsRecordType;
  		pslDnsRecords = &(pOverlapTransaction->slDnsRecords);
	}

	/* Update the A/AAAA record in pDestInfo->pIp of the transaction  */
	if (sdf_ivk_uaGetDestInfoFromTransaction(pTransaction,\
	  	pOverlapTransaction,&pDestInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,\
			Sdf_en_destinationInfoError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFillDestInfoFromTxnSrvRec():"
			"Failed to get Destination Info from the Call Object",pErr);
#endif
		pErr->errCode=Sdf_en_destinationInfoError;
		return Sdf_co_fail;
	}

    switch (dDnsRecordType)
	{
	case Sdf_en_SRVrecord:
	{
		if (sdf_ivk_uaFillDestInfoFromSrvRec (pCallObj, pOverlapTransaction,
					pslDnsRecords, pDestInfo, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaNextDnsTransaction( ): "
				"Failed to fetch the next transaction",pErr);
#endif

			(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
			
			return Sdf_co_fail;
		}
		break;
	}
	case Sdf_en_IPv4Address:
	case Sdf_en_IPv6Address:
	case Sdf_en_IPAddress:
	{
		if (sdf_ivk_uaFillDestInfoFromIpRec (pslDnsRecords, pDestInfo,pErr) \
				== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaNextDnsTransaction( ): "
				"Failed to fetch the next transaction",pErr);
#endif

			(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
			return Sdf_co_fail;
		}
		break;
	}
	/* For handling Unknown DNS record */	
	case Sdf_en_NAPTRrecord:
	default:
	{
		Sdf_ty_retVal dRetVal  = Sdf_co_fail;

		if (pGlbToolkitData->pFillDestInfoFromUnknownDnsRec != Sdf_co_null)
		{
    		dRetVal = pGlbToolkitData->pFillDestInfoFromUnknownDnsRec( \
				pCallObj, pOverlapTransaction, pslDnsRecords, pDestInfo,
				pErr);	
		}
		if (dRetVal == Sdf_co_fail)
		{
			(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
			return Sdf_co_fail;	
		}
	}	
	}
	/* Update DNS State */
	if(pOverlapTransaction==Sdf_co_null)
	{
		if(pTransaction == Sdf_co_null)
		{
			(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
			return Sdf_co_fail;
		}
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0,  &pTransaction->dTxnDnsStateMutex, 0);  	
#endif
		switch (pTransaction->dTxnDnsState)  
		{
		case Sdf_en_dnsQuery:
			pTransaction->dTxnDnsState = Sdf_en_dnsFirstRecordFilled;	
			break;
		case Sdf_en_dnsIpUsed:
			pTransaction->dTxnDnsState = Sdf_en_dnsFilled;
			break;
		default: ;
		/* To avoid warning, default is added */
		}

#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif
	}	
	else
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex, 0);  	
#endif
		switch (pOverlapTransaction->dTxnDnsState)  
		{
		case Sdf_en_dnsQuery:
			pOverlapTransaction->dTxnDnsState = Sdf_en_dnsFirstRecordFilled;	
			break;
		case Sdf_en_dnsIpUsed:
			pOverlapTransaction->dTxnDnsState = Sdf_en_dnsFilled;	
			break;
		default: ;
		/* To avoid warning, default is added */
		}
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex);	
#endif
	}
	(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:	  sdf_ivk_uaUpdateDnsRecordinTransaction
 **
 ** DESCRIPTION: This function update the DNS record into the transaction
 **
 **	PARAMETERS:
 **   pDnsEventContext(IN)    : Sdf_st_dnsEventContext Structure
 **   bResult                 : Status of the expanded name using ares
 **	 	pErr(OUT)								: Error structure in case the function fails
 *****************************************************************************/
Sdf_ty_retVal  sdf_ivk_uaUpdateDnsRecordinTransaction (
		Sdf_st_dnsEventContext  *pDnsEventContext,
	    Sdf_ty_bool             bResult, 
		Sdf_st_error            *pErr)
{

sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -" 
		  " Entering sdf_ivk_uaSetTo");
	
		/* PARAMETER VALIDATION STARTS */
#ifdef SDF_PARAMVALIDATION	
	
  if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaupdateDnsRecordinTransaction" 
			 ":pErr passed is Invalid");
#endif
		return Sdf_co_fail;
	}
 if(pDnsEventContext == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDnsRecordinTransaction(): "
			"pDnsEventContext param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	
 
#endif
  	/* PARAMETER VALIDATION ENDS */
	/* Update the Txn Dns state machine according to the result */
	if (bResult == Sdf_co_false)
	{
		if (pDnsEventContext->pOverlappedTransaction == Sdf_co_null)
		{

#ifdef SDF_THREAD_SAFE
				sdf_fn_uaLockMutex(0,  &pDnsEventContext->pTransaction->dTxnDnsStateMutex, 0);  	
#endif
				if (pDnsEventContext->pTransaction->dTxnDnsState != Sdf_en_dnsQuery)
				{

#ifdef SDF_THREAD_SAFE
					sdf_fn_uaUnlockMutex(0,  &pDnsEventContext->pTransaction->dTxnDnsStateMutex);	
#endif
					return Sdf_co_fail;
				}
				pDnsEventContext->pTransaction->dTxnDnsState = Sdf_en_dnsFail;	
#ifdef SDF_THREAD_SAFE
				sdf_fn_uaUnlockMutex(0,  &pDnsEventContext->pTransaction->dTxnDnsStateMutex);	
#endif
		}
		else {
#ifdef SDF_THREAD_SAFE
				sdf_fn_uaLockMutex(0,  &pDnsEventContext->pOverlappedTransaction->dTxnDnsStateMutex, 0);  	
#endif
				if (pDnsEventContext->pOverlappedTransaction->dTxnDnsState != Sdf_en_dnsQuery)
				{
/* CSR_1-4829060 Fix */
/* #ifdef SDF_LOOKUP */
#ifdef SDF_THREAD_SAFE
/* CSR_1-4829060 Fix */
					sdf_fn_uaUnlockMutex(0,  &pDnsEventContext->pOverlappedTransaction->dTxnDnsStateMutex);
#endif
					return Sdf_co_fail;
				}
				pDnsEventContext->pOverlappedTransaction->dTxnDnsState = Sdf_en_dnsFail;	
/* CSR_1-4829060 Fix */
/* #ifdef SDF_LOOKUP */
#ifdef SDF_THREAD_SAFE
/* CSR_1-4829060 Fix */

				sdf_fn_uaUnlockMutex(0,  &pDnsEventContext->pOverlappedTransaction->dTxnDnsStateMutex);
#endif
		}	
		return Sdf_co_fail;
	}

	/* Update the TransInfo of the Txn with this IP/port */
	if (sdf_fn_uaFillDestInfoFromTxnDnsRec (pDnsEventContext->pCallObj, 
									pDnsEventContext->pOverlappedTransaction,
									pErr) == Sdf_co_fail)
	{
   	return Sdf_co_fail;
	}
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaUpdateDnsRecordinTransaction");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success; 
}



 /******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaIsDnsQueryState
 **
 ** DESCRIPTION: This function returns whether the Transaction is in 
 **							 DnsQuery State.
 **	
 **	PARAMETERS:
 ** 	pCallObj(IN/OUT)       	: Call object into which the decoded message is set.
 **   pOverlapTransaction(IN) : OverlapTransaction Object to which the elements
 **														    	are updated
 **  	pErr(OUT)								: Error structure in case the function fails
 ******************************************************************************/
 Sdf_ty_bool sdf_ivk_uaIsDnsQueryState
  (Sdf_st_callObject       *pCallObj,
	Sdf_st_overlapTransInfo  *pOverlapTransaction,
	Sdf_st_error             *pErr)

{
		Sdf_st_TxnDnsState		dTxnDnsState;
	  Sdf_st_transaction 		*pTransaction   = Sdf_co_null;
    Sdf_ty_bool						dRet;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -" 
		  " Entering sdf_ivk_uaIsDnsQueryState");
	
/* PARAMETER VALIDATION STARTS */
#ifdef SDF_PARAMVALIDATION
		
		if(pErr == Sdf_co_null)
	  {
#ifdef SDF_ERROR
		  sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
		  	"sdf_ivk_uaDeleteDnsRecordFromTransaction" 
			  ":pErr passed is Invalid");
#endif
		  return Sdf_co_false;
	  }


		if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsDnsQueryState(): "
			"pCallObj param passed is Invalid",pErr);
#endif		
  	pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_false;
	}	
	#endif
	
	if (pOverlapTransaction != Sdf_co_null)
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex, 0);  	
#endif	
		
		dTxnDnsState = pOverlapTransaction->dTxnDnsState;
		
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex);	
#endif		
		
	} 
  else 
	{
	 	if (pCallObj->pUacTransaction != Sdf_co_null)
			pTransaction = pCallObj->pUacTransaction;
		else 
			pTransaction = pCallObj->pUasTransaction;	

		if(pTransaction == Sdf_co_null)
		{				
				return Sdf_co_false;
		}		
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0,  &pTransaction->dTxnDnsStateMutex, 0);  	
#endif	

		dTxnDnsState = pTransaction->dTxnDnsState;	

#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif		
	}
		dRet = (dTxnDnsState == Sdf_en_dnsQuery)?Sdf_co_true:Sdf_co_false;
				
		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
				"Exiting sdf_ivk_uaIsDnsQueryState");
		pErr->errCode = Sdf_en_noUaError;
		return dRet;
}
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
 **	PARAMETERS:
 ** 	pObject(IN/OUT)       	: Call Object to be updated
 **   pOverlapTransaction(IN) :  OverlapTransaction Object to which the elements
 **														    	are updated
 **   pMessage(IN)            : The decoded SipMessage which is to be processed.
 **   pEventContext(IN/OUT)		: Transaction Object to which the elements
 **	 	pErr(OUT)								: Error structure in case the function fails
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleLookup
	(Sdf_st_callObject *pObject,
	Sdf_st_overlapTransInfo *pOverlapTransaction,
	SipMessage				*pMessage, 
	Sdf_st_eventContext     *pEventContext,
	Sdf_st_error *pError)
{
	/* This code works on the SipMessage and not on the CallObject. This is 
	because, at the point of calling this function, the sipmessage has
	already been formed from the CallObject */
   en_SipMessageType   dType;
	Sdf_st_transaction   *pTransaction=Sdf_co_null;
	SipAddrSpec          *pAddrSpec=Sdf_co_null;
	Sdf_st_transportInfo *pDestInfo = Sdf_co_null;
	Sdf_ty_u16bit        dPort, dOldPort;
  Sdf_ty_s8bit         dOldIp[Sdf_co_characterBufferSize]="";
	Sdf_st_destDetails   *pDestDetails = Sdf_co_null;
	Sdf_ty_s8bit         dTempAddr[Sdf_co_characterBufferSize] = {0};
#ifdef SDF_ASYNC_DNS
	Sdf_st_asyncDNSContextInfo *pAsyncDnsContextInfo = Sdf_co_null, \
							   *pAppData = Sdf_co_null;
        Sdf_st_appData 	*pCallObjAppData = Sdf_co_null;

#endif /* SDF_ASYNC_DNS */
	
#ifdef UT_STUB
	UA_DNS_UTP_TempDnsRecordType
#endif

#ifdef SDF_LOOKUP
	Sdf_ty_UrlType 			 dUrlType = Sdf_en_UnKnownUrl;
	Sdf_st_dnsEventContext 	*pDnsEventContext=Sdf_co_null;
	Sdf_ty_DnsRecordType	dDnsRecordType = Sdf_en_unKnownAddr;
	Sdf_ty_bool				dUpdateState =	Sdf_co_false;
#endif

        /* Fix for IPTK CSR 1-6967056: SPR 19351 */
        Sdf_ty_u8bit    *pParam = Sdf_co_null;


	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_fn_uaHandleLookup ");

	(Sdf_ty_void)pEventContext;	

   	if (sip_getMessageType(pMessage, &dType, (SipError*)\
		&(pError->stkErrCode)) ==  SipFail)
 	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaHandleLookup (): "
			"Failed to get type of SIP Message", pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	/* Check for Trancasction Type */
	if(pOverlapTransaction==Sdf_co_null)
	{
		if(dType==SipMessageRequest)
			pTransaction=pObject->pUacTransaction;
		else
			pTransaction=pObject->pUasTransaction;
	if(pTransaction == Sdf_co_null)
				return Sdf_co_fail;
	
	}

	if (sdf_ivk_uaGetDestinationDetails(pObject, pOverlapTransaction,\
					pMessage, &pDestDetails, pError) == Sdf_co_fail)
    {
/*				(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails); */

#ifdef SDF_ERROR
	    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
		    (Sdf_ty_s8bit *)"sdf_fn_uaHandleLookup (): "
		    "Failed to get the target address spec.", pError);
#endif
	    return Sdf_co_fail;
    }

/*   if (pDestDetails->dIsTargetUriSet == Sdf_co_true)
        pAddrSpec = pDestDetails->u.pAddrSpec;
    else
        pDestInfo = pDestDetails->u.pDestInfo;
*/
    if (dType == SipMessageRequest)
    {
	 pAddrSpec = pDestDetails->u.pAddrSpec;

        if((pAddrSpec != Sdf_co_null) && (pAddrSpec->dType == SipAddrReqUri))
/*#SDF_LINT #else
        if((pAddrSpec->dType == SipAddrReqUri))
#endif*/
        {
#ifdef SDF_TEL
			TelUrl 	*pTelUrl = Sdf_co_null;
#endif
#ifdef SDF_IM
			ImUrl	*pImUrl = Sdf_co_null;
#endif
#ifdef SDF_PRES
			PresUrl	*pPresUrl = Sdf_co_null;
#endif
			Sdf_ty_bool		dIsTelUrl = Sdf_co_false;
			Sdf_ty_bool		dIsImUrl  = Sdf_co_false;	
			Sdf_ty_bool		dIsPresUrl  = Sdf_co_false;	

#ifdef SDF_TEL
			if(sip_isTelUrl(pAddrSpec, (SipError*)&(pError->stkErrCode)) \
					==SipSuccess)
				dIsTelUrl = Sdf_co_true;
#endif
#ifdef SDF_IM
			if( dIsTelUrl == Sdf_co_false )
			{
				if(sip_isImUrl(pAddrSpec, (SipError*)&(pError->stkErrCode)) \
					==SipSuccess)
				dIsImUrl = Sdf_co_true;
			}
#endif
#ifdef SDF_PRES
            if((dIsImUrl == Sdf_co_false)&&(dIsTelUrl == Sdf_co_false))
            {
				if(sip_isPresUrl(pAddrSpec, (SipError*)&(pError->stkErrCode)) \
					==SipSuccess)
				dIsPresUrl = Sdf_co_true; 
            }
#endif
			if((dIsTelUrl != Sdf_co_true) && (dIsImUrl != Sdf_co_true)&&
                (dIsPresUrl != Sdf_co_true))
			{
			    (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR					
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_destinationInfoError,\
					(Sdf_ty_s8bit *)" sdf_fn_uaHandleLookup ( ): "
					"Unable to resolve destination as Request-URI", pError);
#endif				
				pError->errCode=Sdf_en_destinationInfoError;
				return Sdf_co_fail;
			}

#ifdef SDF_TEL
			if( dIsTelUrl == Sdf_co_true )
			{
				/*check if it is a Tel Url, if yes then give a callback to the
				 * application else return a Error.
				 */
				if(sip_getTelUrlFromAddrSpec(pAddrSpec,&pTelUrl,\
							(SipError*)&(pError->stkErrCode))==SipFail)
				{
					if((SipError)(pError->stkErrCode)==E_INV_TYPE)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_notImplemented,(Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup (): "
							"From header contains neither a sip url nor"
							"nor a tel url",pError);
#endif
							pError->errCode=Sdf_en_notImplemented;
					}
					else
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,\
							Sdf_en_headerManipulationError,(Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup (): "
							"Failed to get Tel Url from AddrSpec",pError);
#endif
						pError->errCode=Sdf_en_headerManipulationError;
					}
					(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
					return Sdf_co_fail;
				}
				else
				{
#ifdef SDF_LOOKUP
					/*  Event Context Filling  */
		  			if(sdf_ivk_uaInitDnsEventContext (&pDnsEventContext, pError) != Sdf_co_success)
					{
#ifdef SDF_ERROR
			      sdf_fn_setError(Sdf_en_majorErrors,\
									pError->errCode,(Sdf_ty_s8bit *)\
											" sdf_fn_uaHandleLookup (): "
										  "Failed to Initialise DNS Event Context",pError);
#endif
						return Sdf_co_fail;
			
					}
  					pDnsEventContext->pCallObj 		         = pObject;
  					pDnsEventContext->pOverlappedTransaction = pOverlapTransaction;	
	  				pDnsEventContext->dDnsRecordType         = Sdf_en_unKnownAddr;	
					pDnsEventContext->pTransaction			 = pTransaction;
					pDnsEventContext->dProtocol 			 = Sdf_en_protoAny;
					pDnsEventContext->pEventContext			 = pEventContext;

					if(Sdf_co_null != pDnsEventContext->pEventContext)
					   HSS_LOCKEDINCREF(pDnsEventContext->pEventContext->dRefCount); 
					HSS_LOCKEDINCREF(pDnsEventContext->pCallObj->dRefCount);

					/* DNS related details populating in Transaction */
       				if (pOverlapTransaction != Sdf_co_null)
					{
    					pDnsEventContext->pslRecordList = \
							&(pOverlapTransaction->slDnsRecords);
						pOverlapTransaction->dDnsRecordType   = dDnsRecordType;
						HSS_LOCKEDINCREF(pDnsEventContext->pOverlappedTransaction \
							->dRefCount);
					} 
			        else
					{
							if(pTransaction == Sdf_co_null)
								return Sdf_co_fail;

		  		  		pDnsEventContext->pslRecordList = \
							&(pTransaction->slDnsRecords);
		  				pTransaction->dDnsRecordType   = dDnsRecordType;
						HSS_LOCKEDINCREF(pDnsEventContext->pTransaction->dRefCount);
					}
					dUrlType = 	Sdf_en_TelUrl;
				  if (sdf_cbk_uaGetIpFromNonSipUrl(pDnsEventContext,
							(Sdf_ty_pvoid)pTelUrl, dUrlType,	pError) == Sdf_co_fail)
					{
						return Sdf_co_fail;
					}
					/* Free DnsEventContext of the Dns Query */
#ifndef SDF_ASYNC_DNS
					(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
#else
                    if((pObject->pAppData->pData != Sdf_co_null) && \
						*((Sdf_ty_bool*)(pObject->pAppData->pData)) == Sdf_co_true)
					{
						(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
					}
#endif/*SDF_ASYNC_DNS*/
#else
					/* This is a Tel Url. Now Give a callback to the
					 * application and Get the Ip and port*/
					if (sdf_ivk_uaGetDestInfoFromTransaction(pTransaction,\
						pOverlapTransaction,&pDestInfo, pError) == Sdf_co_fail)
					{
						(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
						sip_freeTelUrl(pTelUrl);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,\
							Sdf_en_destinationInfoError,\
							(Sdf_ty_s8bit *)" sdf_fn_uaHandleLookup ():"
							"Failed to get Destination Info from the "
							"Call Object",pError);
#endif
						pError->errCode=Sdf_en_destinationInfoError;
						return Sdf_co_fail;
					}

					/*Issue callback to the application to resolve the Tel-URL*/
					if(sdf_cbk_uaGetIpFromTelUrl(pObject,pTelUrl,\
								pDestInfo,pError)==Sdf_co_fail)
					{
						(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
						sip_freeTelUrl(pTelUrl);
						(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);	
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,\
							Sdf_en_destinationInfoError, (Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup ():"
							"Failed to get Ip From TelUrl",pError);
#endif
						pError->errCode=Sdf_en_destinationInfoError;
						return Sdf_co_fail;
					}
					(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
#endif
					sip_freeTelUrl(pTelUrl);
				}
				(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
			}
#endif
#ifdef SDF_IM
			if( dIsImUrl == Sdf_co_true )
			{
				/*check if it is a IM Url, if yes then give a callback to the
				 * application else return a Error.
				 */
				if(sip_getImUrlFromAddrSpec(pAddrSpec,&pImUrl,\
							(SipError*)&(pError->stkErrCode))==SipFail)
				{
					if((SipError)(pError->stkErrCode)==E_INV_TYPE)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_notImplemented,(Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup (): "
							"From header contains neither a sip url nor"
							"nor a IM url",pError);
#endif
							pError->errCode=Sdf_en_notImplemented;
					}
					else
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,\
							Sdf_en_headerManipulationError,(Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup (): "
							"Failed to get IM Url from AddrSpec",pError);
#endif
						pError->errCode=Sdf_en_headerManipulationError;
					}
					(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
					return Sdf_co_fail;
				}
				else
				{
#ifdef SDF_LOOKUP
					/*  Event Context Filling  */
		  		/*	sdf_ivk_uaInitDnsEventContext (&pDnsEventContext, pError);*/
  			if(sdf_ivk_uaInitDnsEventContext (&pDnsEventContext, pError) != Sdf_co_success)
						{
#ifdef SDF_ERROR
						      sdf_fn_setError(Sdf_en_majorErrors,\
										pError->errCode,(Sdf_ty_s8bit *)\
											" sdf_fn_uaHandleLookup (): "
										  "Failed to Initialise DNS Event Context",pError);
#endif
						return Sdf_co_fail;
			
						}
	
  					pDnsEventContext->pCallObj 		         = pObject;
  					pDnsEventContext->pOverlappedTransaction = pOverlapTransaction;	
	  				pDnsEventContext->dDnsRecordType         = Sdf_en_unKnownAddr;	
					pDnsEventContext->pTransaction			 = pTransaction;
					pDnsEventContext->dProtocol 			 = Sdf_en_protoAny;
					pDnsEventContext->pEventContext			 = pEventContext;

                    if(Sdf_co_null != pDnsEventContext->pEventContext)
					   HSS_LOCKEDINCREF(pDnsEventContext->pEventContext->dRefCount); 
					HSS_LOCKEDINCREF(pDnsEventContext->pCallObj->dRefCount);

					/* DNS related details populating in Transaction */
       				if (pOverlapTransaction != Sdf_co_null)
					{
    					pDnsEventContext->pslRecordList = \
							&(pOverlapTransaction->slDnsRecords);
						pOverlapTransaction->dDnsRecordType   = dDnsRecordType;
						HSS_LOCKEDINCREF(pDnsEventContext->pOverlappedTransaction \
							->dRefCount);
					} 
			        else
					{
							if(pTransaction == Sdf_co_null)
				           return Sdf_co_fail;

		  		  		pDnsEventContext->pslRecordList = \
							&(pTransaction->slDnsRecords);
		  				pTransaction->dDnsRecordType   = dDnsRecordType;
						HSS_LOCKEDINCREF(pDnsEventContext->pTransaction->dRefCount);
					}
					dUrlType = 	Sdf_en_ImUrl;
				    if (sdf_cbk_uaGetIpFromNonSipUrl(pDnsEventContext,
							(Sdf_ty_pvoid)pImUrl, dUrlType,	pError) == Sdf_co_fail)
					{
						return Sdf_co_fail;
					}
					/* Free DnsEventContext of the Dns Query */
#ifndef SDF_ASYNC_DNS
					(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
#else
                    if((pObject->pAppData->pData != Sdf_co_null) && \
						*((Sdf_ty_bool*)(pObject->pAppData->pData)) == Sdf_co_true)
					{
						(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
					}
#endif/*SDF_ASYNC_DNS*/
#else
					/* This is a IM Url. Now Give a callback to the
					 * application and Get the Ip and port*/
					if (sdf_ivk_uaGetDestInfoFromTransaction(pTransaction,\
						pOverlapTransaction,&pDestInfo, pError) == Sdf_co_fail)
					{
						(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
						sip_freeImUrl(pImUrl);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,\
							Sdf_en_destinationInfoError,\
							(Sdf_ty_s8bit *)" sdf_fn_uaHandleLookup ():"
							"Failed to get Destination Info from the "
							"Call Object",pError);
#endif
						pError->errCode=Sdf_en_destinationInfoError;
						return Sdf_co_fail;
					}
					/*Issue callback to the application to resolve the Tel-URL*/
					if(sdf_cbk_uaGetIpFromImUrl(pObject,pImUrl,\
							pDestInfo, pError)==Sdf_co_fail)
					{
						(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
						sip_freeImUrl(pImUrl);
						(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);	
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,\
							Sdf_en_destinationInfoError, (Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup ():"
							"Failed to get Ip From IMUrl",pError);
#endif
						pError->errCode=Sdf_en_destinationInfoError;
						return Sdf_co_fail;
					}
					(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
#endif
					sip_freeImUrl(pImUrl);
				}
				(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
			}
#endif
#ifdef SDF_PRES
			if( dIsPresUrl == Sdf_co_true )
			{
				/*check if it is a Pres Url, if yes then give a callback to the
				 * application else return a Error.
				 */
				if(sip_getPresUrlFromAddrSpec(pAddrSpec,&pPresUrl,\
							(SipError*)&(pError->stkErrCode))==SipFail)
				{
					if((SipError)(pError->stkErrCode)==E_INV_TYPE)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_notImplemented,(Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup (): "
							"From header contains neither a sip url nor"
							"nor a PRES url",pError);
#endif
							pError->errCode=Sdf_en_notImplemented;
					}
					else
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,\
							Sdf_en_headerManipulationError,(Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup (): "
							"Failed to get PRES Url from AddrSpec",pError);
#endif
						pError->errCode=Sdf_en_headerManipulationError;
					}
					(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
					return Sdf_co_fail;
				}
				else
				{
#ifdef SDF_LOOKUP
					/*  Event Context Filling  */
		  		/*	sdf_ivk_uaInitDnsEventContext (&pDnsEventContext, pError);*/
  			if(sdf_ivk_uaInitDnsEventContext (&pDnsEventContext, pError) != Sdf_co_success)
						{
#ifdef SDF_ERROR
						      sdf_fn_setError(Sdf_en_majorErrors,\
										pError->errCode,(Sdf_ty_s8bit *)\
											"sdf_ivk_uaUpdateDestinationInfo(): "
										  "Failed to Initialise DNS Event Context",pError);
#endif
						return Sdf_co_fail;
			
						}
	
  					pDnsEventContext->pCallObj 		         = pObject;
  					pDnsEventContext->pOverlappedTransaction = pOverlapTransaction;	
	  				pDnsEventContext->dDnsRecordType         = Sdf_en_unKnownAddr;	
					pDnsEventContext->pTransaction			 = pTransaction;
					pDnsEventContext->dProtocol 			 = Sdf_en_protoAny;
					pDnsEventContext->pEventContext			 = pEventContext;

					if(Sdf_co_null != pDnsEventContext->pEventContext)
					     HSS_LOCKEDINCREF(pDnsEventContext->pEventContext->dRefCount); 

					HSS_LOCKEDINCREF(pDnsEventContext->pCallObj->dRefCount);

					/* DNS related details populating in Transaction */
       		if (pOverlapTransaction != Sdf_co_null)
					{
    				pDnsEventContext->pslRecordList = \
							&(pOverlapTransaction->slDnsRecords);
						pOverlapTransaction->dDnsRecordType   = dDnsRecordType;
						HSS_LOCKEDINCREF(pDnsEventContext->pOverlappedTransaction \
							->dRefCount);
					} 
			    else
					{
							if(pTransaction == Sdf_co_null)
				           return Sdf_co_fail;

		  		  		pDnsEventContext->pslRecordList = \
							&(pTransaction->slDnsRecords);
		  				pTransaction->dDnsRecordType   = dDnsRecordType;
						HSS_LOCKEDINCREF(pDnsEventContext->pTransaction->dRefCount);
					}
					dUrlType = 	Sdf_en_PresUrl;
				    if (sdf_cbk_uaGetIpFromNonSipUrl(pDnsEventContext,
							(Sdf_ty_pvoid)pPresUrl, dUrlType,	pError) == Sdf_co_fail)
					{
						return Sdf_co_fail;
					}
					/* Free DnsEventContext of the Dns Query */
#ifndef SDF_ASYNC_DNS
					(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
#else
                    if((pObject->pAppData->pData != Sdf_co_null) && \
						*((Sdf_ty_bool*)(pObject->pAppData->pData)) == Sdf_co_true)
					{
						(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
					}
#endif/*SDF_ASYNC_DNS*/
#else
					/* This is a PRES Url. Now Give a callback to the
					 * application and Get the Ip and port*/
					if (sdf_ivk_uaGetDestInfoFromTransaction(pTransaction,\
						pOverlapTransaction,&pDestInfo, pError) == Sdf_co_fail)
					{
						(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
						sip_freePresUrl(pPresUrl);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,\
							Sdf_en_destinationInfoError,\
							(Sdf_ty_s8bit *)" sdf_fn_uaHandleLookup ():"
							"Failed to get Destination Info from the "
							"Call Object",pError);
#endif
						pError->errCode=Sdf_en_destinationInfoError;
						return Sdf_co_fail;
					}
					/*Issue callback to the application to resolve the Tel-URL*/
					if(sdf_cbk_uaGetIpFromPresUrl(pObject,pPresUrl,\
							pDestInfo, pError)==Sdf_co_fail)
					{
						(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
						sip_freePresUrl(pPresUrl);
						(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);	
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,\
							Sdf_en_destinationInfoError, (Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup ():"
							"Failed to get Ip From PresUrl",pError);
#endif
						pError->errCode=Sdf_en_destinationInfoError;
						return Sdf_co_fail;
					}
#endif
					(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
					sip_freePresUrl(pPresUrl);
				}
				(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
			}
#endif            
		}
		else
        {
			SipUrl				*pUrl = Sdf_co_null;
	        Sdf_ty_s8bit       	*pTempHost = Sdf_co_null;
	        Sdf_ty_s8bit      	 *pHost = Sdf_co_null;
#ifdef SDF_LOOKUP
			dUrlType  =	Sdf_en_SipUrl; 
#endif

			if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pUrl,\
		    				pError) == Sdf_co_fail)
		    {
		        (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR					
		    	sdf_fn_setError(Sdf_en_minorErrors,\
		    		Sdf_en_destinationInfoError,\
		    		(Sdf_ty_s8bit *)" sdf_fn_uaHandleLookup ( ): "
		    		"Failed to get Url From Addr Spec",pError);
#endif					
		    	pError->errCode=Sdf_en_destinationInfoError;
			    return Sdf_co_fail;
		    }

		    if (sip_getHostFromUrl(pUrl, &pTempHost,\
			    (SipError*)&(pError->stkErrCode)) == SipFail)
		    {
			    sip_freeSipUrl(pUrl);
		        (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR					
			    sdf_fn_setError(Sdf_en_minorErrors,\
				    Sdf_en_destinationInfoError,\
				    (Sdf_ty_s8bit *)" sdf_fn_uaHandleLookup ( ): "
				    "Failed to get Host from Url",pError);
#endif					
			    pError->errCode=Sdf_en_destinationInfoError;
			    return Sdf_co_fail;
		    }

		    if (sdf_ivk_uaGetDestInfoFromTransaction(pTransaction,\
					pOverlapTransaction,&pDestInfo,pError) == Sdf_co_fail)
		    {
#ifdef SDF_ERROR				
			    sdf_fn_setError(Sdf_en_minorErrors, \
				    Sdf_en_transpInfoAccessError, \
				    (Sdf_ty_s8bit *)"SDF_ERROR - "
				    " sdf_fn_uaHandleLookup ( ): Failed to get "
				    "Destination Info from the Transaction", pError);
#endif					
			    sip_freeSipUrl(pUrl);
		        (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);

			    pError->errCode=Sdf_en_transpInfoAccessError;
			    return Sdf_co_fail;
		    }

			if (sip_getPortFromUrl(pUrl, &dPort, \
			    (SipError *)&(pError->stkErrCode)) == SipFail)
		    {
#ifndef SDF_LOOKUP
			    pError->errCode = Sdf_en_noExistError;
			    if (pError->errCode == Sdf_en_noExistError)
			    {
			    	/* send to default port 5060/5061 */
#ifdef SDF_TLS
					if((pAddrSpec != Sdf_co_null) && (pAddrSpec->dType == \
								SipAddrSipSUri))
			    	    pDestInfo->dPort = Sdf_mc_defaultSipSPort;
					else
#endif
			    	    pDestInfo->dPort = Sdf_mc_defaultSipPort;
			    }
#else
				if (pDestInfo->dScheme == Sdf_en_protoNone)
					dDnsRecordType = Sdf_en_NAPTRrecord;
				else
					dDnsRecordType = Sdf_en_SRVrecord;
#endif 
		    }
		   else
			{
#ifdef SDF_LOOKUP
				dDnsRecordType = Sdf_en_IPAddress;	
#endif
				pDestInfo->dPort = dPort;
			}
#ifdef SDF_TLS
#ifdef SDF_LINT		
			if((pAddrSpec != Sdf_co_null) && (pAddrSpec->dType == \
						SipAddrSipSUri))
#else
			if (pAddrSpec->dType == SipAddrSipSUri)

#endif
			{
                pDestInfo->dScheme = Sdf_en_protoTls;
#ifdef SDF_LOOKUP
				dUrlType 		   =	Sdf_en_SipsUrl; 
#endif
			}
			else
			{
				/*
				 * This is to handle the case in which the final destination is
				 * identified by a SIPS URI and the outbound proxy is a loose
				 * router having SIP URI. In this case also, the message should
				 * go over TLS.
				 */
				SipReqLine 		*pReqLine = Sdf_co_null;

				if (sip_getReqLine(pMessage, &pReqLine, \
						(SipError *)&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_destinationInfoError,\
						(Sdf_ty_s8bit *)" sdf_fn_uaHandleLookup ( ): "
						"Failed to get Request Line from SipMessage",pError);
#endif				
			        sip_freeSipUrl(pUrl);
					(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
			    	(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
					pError->errCode=Sdf_en_destinationInfoError;
					return Sdf_co_fail;
				}
				
				if (sip_getAddrSpecFromReqLine(pReqLine, &pAddrSpec, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_minorErrors, \
						Sdf_en_destinationInfoError,\
						(Sdf_ty_s8bit *)"  sdf_fn_uaHandleLookup ( ): "
						"Failed to get AddrSpec from the Request Line",pError);
#endif				
			        sip_freeSipUrl(pUrl);
					sip_freeSipReqLine(pReqLine);
					(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
			    	(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
					pError->errCode=Sdf_en_destinationInfoError;
					return Sdf_co_fail;
				}

				if (pAddrSpec->dType == SipAddrSipSUri)
				{
                    pDestInfo->dScheme = Sdf_en_protoTls;
#ifdef SDF_LOOKUP										
					dUrlType = 	Sdf_en_SipsUrl; 
#endif
				}

				/* Free the local reference here. */
				sip_freeSipAddrSpec(pAddrSpec);
				sip_freeSipReqLine(pReqLine);
			}
#endif /* SDF_TLS */
		    if(pDestInfo->pIp != Sdf_co_null)
            {
              /*SPR-19314 Fixed*/
              /* CSR_1-5233734 Change Start */
		         Sdf_mc_strncpy(dOldIp, pDestInfo->pIp,
                                 Sdf_co_characterBufferSize-1);
                 dOldIp[Sdf_co_characterBufferSize-1]='\0';
			  /* CSR_1-5233734 Change End   */
            } 
		    dOldPort = pDestInfo->dPort;

		    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
			    (pDestInfo->pIp),pError);

		    pHost = Sdf_mc_strdupCallHandler(pTempHost);

            if( pHost == Sdf_co_null)
            {
                sip_freeSipUrl(pUrl);
		        (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
                pError->errCode = Sdf_en_noMemoryError; 
	    	    return Sdf_co_fail;
            }

             /* Fix for IPTK CSR 1-6967056:*/
             /* If pHost is of the form domain?Params, strip Params */
             pParam = (Sdf_ty_u8bit *)Sdf_mc_strchr(pHost, '?');
             
             if (Sdf_co_null != pParam)
             {
                 *pParam = '\0';
             }

#ifndef SDF_LOOKUP
			if (Sdf_mc_strstr(pHost, "[") != Sdf_co_null)
			{
			    /* Strip the '[',']' before resolving the address. */

                Sdf_mc_strncpy(dTempAddr,(pHost+1), Sdf_mc_strlen(pHost)-2);
		        dTempAddr[Sdf_mc_strlen(pHost)-2] = '\0';
			}
			else
#endif
			{
                /*SPR-19314 Fixed*/
                /* CSR_1-5233734 Change Start */
				Sdf_mc_strncpy(dTempAddr, pHost,
                                 Sdf_co_characterBufferSize-1);
                dTempAddr[Sdf_co_characterBufferSize - 1] = '\0';
				/* CSR_1-5233734 Change End */
/*#ifdef SDF_LOOKUP
				dDnsRecordType = Sdf_en_IPv4Address;
#endif */
			}
#ifndef SDF_LOOKUP
		    if(sdf_fn_uaGetHostIp(dTempAddr,&pDestInfo->pIp,\
							pError)==Sdf_co_fail)
		    {
			    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					    (Sdf_ty_pvoid*)&pHost, pError);
			    sip_freeSipUrl(pUrl);
		        (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
			    (Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);	
#ifdef SDF_ERROR					
			    sdf_fn_setError(Sdf_en_minorErrors,\
				    Sdf_en_destinationInfoError,\
				    (Sdf_ty_s8bit *)" sdf_fn_uaHandleLookup ( ): "
				    "Failed to get the host IP from Destination Info", pError);
#endif					
			    pError->errCode=Sdf_en_destinationInfoError;
			    return Sdf_co_fail;
			}
#else

#ifdef UT_STUB
UA_DNS_UTP_isNumericAddress
#endif
			 if (Sdf_ivk_uaisNumericAddress(dTempAddr,&dDnsRecordType,pError)== \
                                                                Sdf_co_false)
			{
				
				if (dDnsRecordType == Sdf_en_InvalidAddr)	
				{
					(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, \
						(Sdf_ty_pvoid*)&pHost, pError);
					sip_freeSipUrl(pUrl);
			    	    (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
					(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);	
#ifdef SDF_ERROR							
				   		sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_destinationInfoError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaUpdateDestinationInfo( ): "
					  	"Failed to get the host IP from Destination Info", pError);
#endif				
					pError->errCode=Sdf_en_destinationInfoError;
					return Sdf_co_fail;
				} /* End of dDnsRecordType == Sdf_en_InvalidAddr */

				/*  Event Context Filling  */
	  			/*sdf_ivk_uaInitDnsEventContext (&pDnsEventContext, pError);*/
				if(sdf_ivk_uaInitDnsEventContext (&pDnsEventContext, pError) != Sdf_co_success)
				{
#ifdef SDF_ERROR
					      sdf_fn_setError(Sdf_en_majorErrors,\
									pError->errCode,(Sdf_ty_s8bit *)\
									" sdf_fn_uaHandleLookup (): "
								  "Failed to Initialise DNS Event Context",pError);
#endif
						return Sdf_co_fail;
			
						}
						
  				pDnsEventContext->pCallObj 		         = pObject;
  				pDnsEventContext->pOverlappedTransaction = pOverlapTransaction;	
	  			pDnsEventContext->dDnsRecordType         = dDnsRecordType;	
				pDnsEventContext->pTransaction			 = pTransaction;
				pDnsEventContext->dProtocol 			 = pDestInfo->dScheme;
				pDnsEventContext->dUrlType				 = dUrlType;	
				HSS_LOCKEDINCREF(pDnsEventContext->pCallObj->dRefCount);

				/* DNS related details populating in Transaction */
       			if (pOverlapTransaction != Sdf_co_null)
				{
    				 pDnsEventContext->pslRecordList = &(pOverlapTransaction->slDnsRecords);
					 pOverlapTransaction->dDnsRecordType   = dDnsRecordType;
					 HSS_LOCKEDINCREF(pDnsEventContext->pOverlappedTransaction->dRefCount);
				} 
		        else
				{
						if(pTransaction == Sdf_co_null)
			        	return Sdf_co_fail;

		  	  		pDnsEventContext->pslRecordList = &(pTransaction->slDnsRecords);
		  			pTransaction->dDnsRecordType   = dDnsRecordType;
					HSS_LOCKEDINCREF(pDnsEventContext->pTransaction->dRefCount);
				}
#ifdef SDF_ASYNC_DNS				
				if(sdf_fn_uaInitAsyncDnsContextInfo(&pAsyncDnsContextInfo, pError) == \
													Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,\
							pError->errCode,(Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup (): "
							"Failed to Initialise Async DNS Context Info",pError);
#endif
					return Sdf_co_fail;			
				}
				else
				{
					pDnsEventContext->pAppData = pAsyncDnsContextInfo;
					HSS_LOCKEDINCREF(((Sdf_st_asyncDNSContextInfo*)pDnsEventContext-> \
									   pAppData)->dRefCount);
				}
#endif /*SDF_ASYNC_DNS*/

				/* Query the portlayer code */
#ifdef UT_STUB
UA_DNS_UTP_DnsQuery
#endif
				if(sdf_fn_uaDnsQuery(dTempAddr, pDnsEventContext,pError) == Sdf_co_fail)
				{
					(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHost, pError);
					sip_freeSipUrl(pUrl);
					(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
					(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);	
                    /*CSR_1-4904310 Fix Start */
					(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
					/*CSR_1-4904310 Fix End */
#ifdef SDF_ERROR							
				    sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_destinationInfoError,\
    					(Sdf_ty_s8bit *)" sdf_fn_uaHandleLookup (): "
		                "Failed to get the host IP from Destination Info", pError);
#endif					
					pError->errCode=Sdf_en_destinationInfoError;
					return Sdf_co_fail;
				}
#ifdef SDF_ASYNC_DNS
			(Sdf_ty_void)sdf_fn_uaFreeAsyncDnsContextInfo (pDnsEventContext->pAppData);

#endif /*SDF_ASYNC_DNS*/
				/* Free DnsEventContext of the Dns Query */
				(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);

			}
			else
			{
				dUpdateState = Sdf_co_true;	
#ifdef SDF_ASYNC_DNS
				if (sdf_ivk_uaGetAppDataFromCallObject(pObject, \
					&pCallObjAppData, pError) == Sdf_co_fail)
				{
					return Sdf_co_fail;
				}
				if(pCallObjAppData->pData == Sdf_co_null)
				{
					pCallObjAppData->pData = (Sdf_ty_bool*)sdf_memget(Sdf_mc_callHandlerMemId,
						sizeof(Sdf_ty_bool), pError);
					*((Sdf_ty_bool*)(pCallObjAppData->pData)) = Sdf_co_true;
				}
#endif
#ifdef SDF_IPV6
				if (Sdf_mc_strstr(pHost, "[") != Sdf_co_null)
				{
				    /* Strip the '[',']' before resolving the address. */

            	    Sdf_mc_strncpy(dTempAddr,(pHost+1), Sdf_mc_strlen(pHost)-2);
		        	dTempAddr[Sdf_mc_strlen(pHost)-2] = '\0';
				}
#endif
				pDestInfo->pIp = Sdf_mc_strdupCallHandler(dTempAddr);

				if(pDestInfo->dPort == 0)
        {
#ifdef SDF_TLS
					if((pAddrSpec != Sdf_co_null) && (pAddrSpec->dType == \
								SipAddrSipSUri))
          {
			    	 pDestInfo->dPort = Sdf_mc_defaultSipSPort;
          }
					else
#endif
			    	pDestInfo->dPort = Sdf_mc_defaultSipPort;
        }
			}
#endif
#ifndef SDF_ASYNC_DNS
			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHost,pError);
		    if ( Sdf_mc_strcmp(dOldIp, "") != 0)
		    {
			    if ( (Sdf_mc_strcmp(dOldIp, pDestInfo->pIp) != 0) || \
				    (dOldPort != pDestInfo->dPort) )
			    {
				    if (pDestInfo->dScheme != Sdf_en_protoUdp)
				    {
					    if(pOverlapTransaction==Sdf_co_null)
					    {
#ifdef SDF_LINT
                            if ((pTransaction != Sdf_co_null) && \
									(pTransaction->dSocket.dProtocol != \
									Sdf_en_protoUdp))
#else                            
						    if (pTransaction->dSocket.dProtocol != \
									Sdf_en_protoUdp)
#endif                                
						    {
							    (Sdf_ty_void)sdf_fn_uaCloseSocket(&pTransaction->dSocket);
						    }
					    }
					    else
					    {
						    if (pOverlapTransaction->dSocket.dProtocol != \
									Sdf_en_protoUdp)
						    {
							    (Sdf_ty_void)sdf_fn_uaCloseSocket(\
										&pOverlapTransaction->dSocket);
						    }
					    }
				    }
			    }
		    }
		    sip_freeSipUrl(pUrl);
		    (Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);

			/* Free the memory allocated for pDestDetails here. */
		    (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#else
			if (Sdf_ivk_uaisNumericAddress(dTempAddr,&dDnsRecordType,pError)== \
                                                                Sdf_co_false)
			{
				/* Store the pointers that need to be freed in 
				 * Sdf_st_asyncDNSContextInfo. They will be freed
				 * inside sdf_fn_uaQueryRespProcs
				 */
				pAppData = (Sdf_st_asyncDNSContextInfo*) pDnsEventContext->pAppData;
				pAppData->pHost = pHost;
				pAppData->dOldPort = dOldPort;
                /*SPR-19314 Fixed*/
                /* CSR_1-5233734 Change Start */
				Sdf_mc_strncpy(pAppData->dOldIp, dOldIp,
                                     Sdf_co_characterBufferSize-1);
				/* CSR_1-5233734 Change End */
				pAppData->pUrl = pUrl;
				pAppData->pDestInfo = pDestInfo;
				pAppData->pDestDetails = pDestDetails;
				pAppData->dMessageType = dType; 
				pAppData->dUrlType = dUrlType;
			}
			else
			{
			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHost,pError);
		    if ( Sdf_mc_strcmp(dOldIp, "") != 0)
		    {
			    if ( (Sdf_mc_strcmp(dOldIp, pDestInfo->pIp) != 0) || \
				    (dOldPort != pDestInfo->dPort) )
			    {
				    if (pDestInfo->dScheme != Sdf_en_protoUdp)
				    {
					    if(pOverlapTransaction==Sdf_co_null)
					    {
#ifdef SDF_LINT
                            if ((pTransaction != Sdf_co_null) && \
									(pTransaction->dSocket.dProtocol != \
									Sdf_en_protoUdp))
#else                            
						    if (pTransaction->dSocket.dProtocol != \
									Sdf_en_protoUdp)
#endif                                
						    {
							    (Sdf_ty_void)sdf_fn_uaCloseSocket(&pTransaction->dSocket);
						    }
					    }
					    else
					    {
						    if (pOverlapTransaction->dSocket.dProtocol != \
									Sdf_en_protoUdp)
						    {
							    (Sdf_ty_void)sdf_fn_uaCloseSocket(\
										&pOverlapTransaction->dSocket);
						    }
					    }
				    }
			    }
		    }
		    sip_freeSipUrl(pUrl);
		    (Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);

			/* Free the memory allocated for pDestDetails here. */
		    (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
			}

#endif /*SDF_ASYNC_DNS*/
		}
	}
	else if (dType == SipMessageResponse)
    {
		Sdf_ty_s8bit *pHostAddress	 = Sdf_co_null; /* final address */

	    pDestInfo = pDestDetails->u.pDestInfo;
		if(pDestInfo == Sdf_co_null)
		{
   		    (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
		   return Sdf_co_fail;
	    }
        pHostAddress = pDestInfo->pIp;

#ifndef SDF_LOOKUP
		if ((sdf_fn_uaGetHostIp(pHostAddress, &pDestInfo->pIp, pError)) ==\
				Sdf_co_fail)
		{
		    (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_destinationInfoError, \
				(Sdf_ty_s8bit*)" sdf_fn_uaHandleLookup ( ): "
				"Failed to get IP address of the host", pError);
#endif			
			/* 
		     * Free the memory allocated for pDestInfo->pIp inside  
		     * sdf_ivk_uaGetDestinationDetails.
		     */
		    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHostAddress,\
		        		pError);
			pError->errCode=Sdf_en_destinationInfoError;
			return Sdf_co_fail;
		}
		(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHostAddress,\
				pError);
#else /*SDF_LOOKUP*/
		if (Sdf_ivk_uaisNumericAddress(pHostAddress, &dDnsRecordType,pError) == Sdf_co_false)
		{
			if (dDnsRecordType == Sdf_en_InvalidAddr)	
			{
				(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
				/* CSR_1-4789104 */
			    /*(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHostAddress,\
		        		pError);*/
				/* CSR_1-4789104 */
#ifdef SDF_ERROR
			    sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_destinationInfoError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaHandleLookup( ): "
					  	"Failed to get the host IP from Destination Info", pError);
#endif
				pError->errCode = Sdf_en_destinationInfoError;
				return Sdf_co_fail;
			} /* End of dDnsRecordType == Sdf_en_InvalidAddr */

			/*  Event Context Filling  */
	  		/*sdf_ivk_uaInitDnsEventContext (&pDnsEventContext, pError);*/

			if(sdf_ivk_uaInitDnsEventContext (&pDnsEventContext, pError) != Sdf_co_success)
			{
#ifdef SDF_ERROR
			      sdf_fn_setError(Sdf_en_majorErrors,\
						pError->errCode,(Sdf_ty_s8bit *)\
						" sdf_fn_uaHandleLookup (): "
					  "Failed to Initialise DNS Event Context",pError);
#endif
				 			return Sdf_co_fail;
			
			}

  			pDnsEventContext->pCallObj 		         = pObject;
  			pDnsEventContext->pOverlappedTransaction = pOverlapTransaction;	
	  		pDnsEventContext->dDnsRecordType         = dDnsRecordType;	
			pDnsEventContext->pTransaction			 = pTransaction;
			pDnsEventContext->dProtocol 			 = pDestInfo->dScheme;
			HSS_LOCKEDINCREF(pDnsEventContext->pCallObj->dRefCount);

			/* DNS related details populating in Transaction */
       		if (pOverlapTransaction != Sdf_co_null)
			{
    			 pDnsEventContext->pslRecordList = &(pOverlapTransaction->slDnsRecords);
				 pOverlapTransaction->dDnsRecordType   = dDnsRecordType;
				 HSS_LOCKEDINCREF(pDnsEventContext->pOverlappedTransaction->dRefCount);
			} 
		    else
			{
					if(pTransaction == Sdf_co_null)
				          return Sdf_co_fail;

		   		pDnsEventContext->pslRecordList = &(pTransaction->slDnsRecords);
		  		pTransaction->dDnsRecordType   = dDnsRecordType;
				HSS_LOCKEDINCREF(pDnsEventContext->pTransaction->dRefCount);
			}
#ifdef SDF_ASYNC_DNS				
				if(sdf_fn_uaInitAsyncDnsContextInfo(&pAsyncDnsContextInfo, pError) == \
													Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,\
							pError->errCode,(Sdf_ty_s8bit *)\
							" sdf_fn_uaHandleLookup (): "
							"Failed to Initialise Async DNS Context Info",pError);
#endif
					return Sdf_co_fail;			
				}
				else
				{
					pDnsEventContext->pAppData = pAsyncDnsContextInfo;
					HSS_LOCKEDINCREF(((Sdf_st_asyncDNSContextInfo*)pDnsEventContext-> \
									   pAppData)->dRefCount);
				}
#endif /*SDF_ASYNC_DNS*/
				/* Query the portlayer code */
	        if(sdf_fn_uaDnsQuery(dTempAddr, pDnsEventContext,pError) == Sdf_co_fail)
            {
                (Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
                /* CSR_1-4904310 Fix Start */
                (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
                if(pHostAddress != pDestInfo->pIp)
                    /* CSR_1-4904310 Fix End */
                    (Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHostAddress,\
                            pError);
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_minorErrors,\
                        Sdf_en_destinationInfoError,\
                        (Sdf_ty_s8bit *)" sdf_fn_uaHandleLookup ( ): "
                        "Failed to get the host IP from Destination Info", pError);
#endif
                pError->errCode = Sdf_en_destinationInfoError;
                return Sdf_co_fail;
            } /* End of sdf_fn_uaDnsQuery */
					/* 
		 * Free the memory allocated for pDestInfo->pIp inside  
		 * sdf_ivk_uaGetDestinationDetails.
		 */
#ifdef SDF_ASYNC_DNS
		(Sdf_ty_void)sdf_fn_uaFreeAsyncDnsContextInfo (pDnsEventContext->pAppData);

#endif /*SDF_ASYNC_DNS*/
			/* Free DnsEventContext of the Dns Query */
			(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext);
            			/* CSR_1-4904310 Fix Start */
#ifndef SDF_ASYNC_DNS
			if(pHostAddress != pDestInfo->pIp)
			/* CSR_1-4904310 Fix End */	
				(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHostAddress,\
				pError);
#endif
		}
		else 
		{
			dUpdateState = Sdf_co_true;
#ifdef SDF_ASYNC_DNS
				if (sdf_ivk_uaGetAppDataFromCallObject(pObject, \
					&pCallObjAppData, pError) == Sdf_co_fail)
				{
					return Sdf_co_fail;
				}
				if(pCallObjAppData->pData == Sdf_co_null)
				{
					pCallObjAppData->pData = (Sdf_ty_bool*)sdf_memget(Sdf_mc_callHandlerMemId,
						sizeof(Sdf_ty_bool), pError);
					*((Sdf_ty_bool*)(pCallObjAppData->pData)) = Sdf_co_true;
				}
#endif
			if(pDestInfo->dPort == 0)
      {
#ifdef SDF_TLS
				if((pAddrSpec != Sdf_co_null) && (pAddrSpec->dType == \
								SipAddrSipSUri))
        {
	    	 	pDestInfo->dPort = Sdf_mc_defaultSipSPort;
        }
				else
#endif
	    	  pDestInfo->dPort = Sdf_mc_defaultSipPort;
      }
		} /* End of Sdf_ivk_uaisNumericAddress */
#endif

		/* Free the memory allocated for pDestDetails here. */
#ifndef SDF_ASYNC_DNS
		(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
#else
			if (Sdf_ivk_uaisNumericAddress(pHostAddress, &dDnsRecordType,pError) == Sdf_co_false)
			{
				/* Store the pointers that need to be freed in 
				 * Sdf_st_asyncDNSContextInfo. They will be freed
				 * inside sdf_fn_uaQueryRespProcs
				 */
				pAppData = (Sdf_st_asyncDNSContextInfo*) pDnsEventContext->pAppData;
				pAppData->pHost = pHostAddress;
				pAppData->pDestDetails = pDestDetails;
				pAppData->dMessageType = dType; 
				pAppData->dUrlType = dUrlType;
			}
			else
			{
				(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
			}
#endif /*SDF_ASYNC_DNS*/
    }
	else
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_destinationInfoError, \
			(Sdf_ty_s8bit*)" sdf_fn_uaHandleLookup ( ): "
			"Failed to Update destination information", pError);
#endif		
		pError->errCode=Sdf_en_destinationInfoError;
		return Sdf_co_fail;
	}	
#ifdef SDF_LOOKUP
	if (dUpdateState == Sdf_co_true)
	{
		/* Update DNS State */
		if(pOverlapTransaction==Sdf_co_null)
		{
				if(pTransaction == Sdf_co_null)
				    return Sdf_co_fail;

#ifdef SDF_THREAD_SAFE
			sdf_fn_uaLockMutex(0,  &pTransaction->dTxnDnsStateMutex, 0);  	
#endif
			switch (pTransaction->dTxnDnsState)  
			{
			case Sdf_en_dnsQuery:
				pTransaction->dTxnDnsState = Sdf_en_dnsFirstRecordFilled;	
				break;
			case Sdf_en_dnsIpUsed:
				pTransaction->dTxnDnsState = Sdf_en_dnsFilled;
				break;
			default: ;
				/* To avoid warning, default is added */
			}
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0,  &pTransaction->dTxnDnsStateMutex);	
#endif
		}
		else
		{
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaLockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex, 0);  	
#endif
			switch (pOverlapTransaction->dTxnDnsState)  
			{
			case Sdf_en_dnsQuery:
				pOverlapTransaction->dTxnDnsState = Sdf_en_dnsFirstRecordFilled;	
				break;
			case Sdf_en_dnsIpUsed:
				pOverlapTransaction->dTxnDnsState = Sdf_en_dnsFilled;	
				break;
			default: ;
			/* To avoid warning, default is added */
			}
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0,  &pOverlapTransaction->dTxnDnsStateMutex);	
#endif
		}
	}		
#endif
	pError->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exiting  sdf_fn_uaHandleLookup ");
	return Sdf_co_success;
}

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


Sdf_ty_retVal sdf_ivk_uaGetRecordCountFromUACTransaction
(Sdf_st_callObject *pObject,
 Sdf_ty_s32bit *dCount,
 Sdf_st_error *pError)
{
  Sdf_ty_slist            *pslDnsRecords  = Sdf_co_null;
  Sdf_st_transaction *pTransaction=Sdf_co_null;

  sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
      "Entering sdf_ivk_uaGetRecordCountFromUACTransaction ");

  /* PARAM VALIDATION STARTS */
#ifdef SDF_PARAMVALIDATION
  if(pError == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
        "sdf_ivk_uaGetRecordCountFromUACTransaction:pError passed is Invalid");
#endif
    return Sdf_co_fail;

  }
  if(pObject == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
        (Sdf_ty_s8bit *)"sdf_ivk_uaGetRecordCountFromUACTransaction(): "
        "pObject param passed is Invalid",pError);
#endif          
    pError->errCode=Sdf_en_invalidParamError;
    return Sdf_co_fail;
  }
#endif
  /* PARAM VALIDATION ENDS */

  pTransaction = pObject->pUacTransaction;
  pslDnsRecords = &(pTransaction->slDnsRecords);

  (Sdf_ty_void)sdf_listSizeOf(pslDnsRecords, (Sdf_ty_u32bit*)dCount, pError);

  pError->errCode=Sdf_en_noUaError;
  sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
      "Exiting  sdf_ivk_uaGetRecordCountFromUACTransaction");
  return Sdf_co_success;

}


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
    Sdf_st_callObject   *pObject,
    Sdf_ty_s32bit       dIndex,
    Sdf_st_DnsRecord    **ppDnsRecord,
    Sdf_st_error        *pError)
{

    Sdf_st_transaction *pTransaction=Sdf_co_null;



    sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
        "Entering sdf_ivk_uaGetDnsIpRecordFromUACTransaction ");

    /* PARAM VALIDATION STARTS */
#ifdef SDF_PARAMVALIDATION
    if(pError == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
         "sdf_ivk_uaGetDnsIpRecordFromUACTransaction:pError passed is Invalid");
#endif
        return Sdf_co_fail;

    }
    if(pObject == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
            (Sdf_ty_s8bit *)"sdf_ivk_uaGetDnsIpRecordFromUACTransaction(): "
            "pObject param passed is Invalid",pError);
#endif      
        pError->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }

#endif
    /* PARAM VALIDATION ENDS */

    pTransaction = pObject->pUacTransaction;



    if (sdf_listGetAt(&(pTransaction->slDnsRecords), dIndex, \
                (Sdf_ty_pvoid *)ppDnsRecord, pError) == Sdf_co_fail)
    {
        pError->errCode = Sdf_en_noMoreDNSRecord;
        return Sdf_co_fail;
    }


    pError->errCode=Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
        "Exiting  sdf_ivk_uaGetDnsIpRecordFromUACTransaction");
    return Sdf_co_success;

}

#ifdef SDF_ASYNC_DNS
/******************************************************************************
 ** FUNCTION: 		sdf_fn_uaQueryRespProcs
 **
 ** DESCRIPTION: 	This function performs UATK related post DNS query 
 **					procudures
 **					
 **	PARAMETERS:
 **   pDnsEventContext(IN)      : The DNS event context ptr   
 **	  pErr(OUT)					: Error structure in case the function fails
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaQueryRespProcs
	(Sdf_st_dnsEventContext 	*pDnsEventContext,
	 Sdf_st_error				*pError)
{	
	Sdf_st_callObject    *pCallObj = Sdf_co_null;
	en_SipMessageType    dType;
	Sdf_ty_u16bit        dOldPort;
    Sdf_ty_s8bit         dOldIp[Sdf_co_characterBufferSize] = "";
	Sdf_st_destDetails   *pDestDetails = Sdf_co_null;
	SipUrl				 *pUrl         = Sdf_co_null;
	Sdf_ty_s8bit      	 *pHost        = Sdf_co_null;
	Sdf_st_transportInfo *pDestInfo    = Sdf_co_null;
	Sdf_st_eventContext *pEventContext;

	Sdf_st_transaction		*pTransaction = Sdf_co_null;
	Sdf_st_overlapTransInfo *pOverlapTransaction = Sdf_co_null;
	
	Sdf_st_asyncDNSContextInfo *pAppData = ((Sdf_st_asyncDNSContextInfo*) \
											pDnsEventContext->pAppData);
	pCallObj = pDnsEventContext->pCallObj;
	pHost = pAppData->pHost;
	dOldPort = pAppData->dOldPort;
	Sdf_mc_strcpy(dOldIp, pAppData->dOldIp);
	pUrl = pAppData->pUrl;
	pDestInfo = pAppData->pDestInfo;
	pDestDetails = pAppData->pDestDetails;
	dType = pAppData->dMessageType;

	pTransaction = pDnsEventContext->pTransaction;
	pOverlapTransaction = pDnsEventContext->pOverlappedTransaction;
	pEventContext = pDnsEventContext->pEventContext;
	if(pAppData->dUrlType  ==	Sdf_en_SipUrl)
	{
		if (dType == SipMessageRequest)
		{

			(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHost,pError);
			if ( Sdf_mc_strcmp(dOldIp, "") != 0)
			{
				if ( (Sdf_mc_strcmp(dOldIp, pDestInfo->pIp) != 0) || \
					(dOldPort != pDestInfo->dPort) )
				{
					if (pDestInfo->dScheme != Sdf_en_protoUdp)
					{
						if(pOverlapTransaction == Sdf_co_null)
						{
	#ifdef SDF_LINT
							if ((pTransaction != Sdf_co_null) && \
									(pTransaction->dSocket.dProtocol != \
									Sdf_en_protoUdp))
	#else                            
							if (pTransaction->dSocket.dProtocol != \
									Sdf_en_protoUdp)
	#endif                                
							{
								(Sdf_ty_void)sdf_fn_uaCloseSocket(&pTransaction->dSocket);
							}
						}
						else
						{
							if (pOverlapTransaction->dSocket.dProtocol != \
									Sdf_en_protoUdp)
							{
								(Sdf_ty_void)sdf_fn_uaCloseSocket(\
										&pOverlapTransaction->dSocket);
							}
						}
					}
				}
			}
			sip_freeSipUrl(pUrl);
			(Sdf_ty_void)sdf_ivk_uaFreeTransportInfo(pDestInfo);

			(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
		}
		else if(dType == SipMessageResponse) 
		{
				//(Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (pDnsEventContext); needs to be done finally in processaresresponse
				(Sdf_ty_void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHost,\
					pError);
				(Sdf_ty_void)sdf_ivk_uaFreeDestDetails(pDestDetails);
		}
	}

	if(sdf_cbk_uaQueryRespProcs(pCallObj,pEventContext,pOverlapTransaction,dType,pError) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}
	return Sdf_co_success;
}
#endif /* SDF_ASYNC_DNS */


