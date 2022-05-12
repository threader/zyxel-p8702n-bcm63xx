/*****************************************************************************
 ** FUNCTION:	
 **					Has the UA Toolkit APIs to make requests or responses.	
 **
 *****************************************************************************
 **
 ** FILENAME: 		sdf_reqresp.c
 **
 ** DESCRIPTION:	Has the UA Toolkit APIs to make requests or responses.
 **  	
 **
 ** DATE      	NAME          REFERENCE      REASON
 ** ----      	----          ---------      ------
 ** 27-Oct-00	Prashant					Creation
 ** 07-Nov-00 	Prashant					Updated
 ** 22/04/03    Anil Kumar     ---          Changes for TLS feature.
 ** 12/05/03     Subbarao		UATK 2.1 Design      RFC 3428 feature added.
 **
 *****************************************************************************
 ** 		COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#include "ALheader.h"

/* Warning 537: Repeated include file */
#ifndef SDF_LINT
#include <time.h>
#include "sdf_free.h"
#include "sdf_list.h"
#include "sdf_common.h"
#include "sdf_struct.h"
#include "sdf_portlayer.h"
#endif
#include "basic.h"
#include "digcalc.h"

#include "sdf_init.h"
#include "sdf_sdpinternal.h"
#include "sdf_reqresp.h"
#include "sdf_sdp.h"
#include "sdf_debug.h"
#include "sdf_accessor.h"
#include "sdf_network.h"
#include "sdf_callbacks.h"
#include "sdf.h"
#include "sdf_internal.h"
#ifdef SDF_IMS
#include "sdf_callapis.h"
#endif

#ifdef SDF_SESSION_TIMER
#include "sdf_sessTimer.h"
#endif
#include "sdf_lookup.h"


/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaAddStatusCodeSpecificHeaders
 **
 ** DESCRIPTION: 	This API forms any additional headers that should 
 **					be added to a response message.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddStatusCodeSpecificHeaders
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_initData *pInitData,
	SipMessage *pMesg,
	Sdf_ty_u16bit statuscode,
	Sdf_st_error *pErr)
#else
	(pCallObj, pInitData, pMesg, statuscode, pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_initData *pInitData;
	SipMessage *pMesg;
	Sdf_ty_u16bit statuscode;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit 	dCount=0;
	SipHeader 		*pHdr = Sdf_co_null;
#ifndef SDF_UAB2B
	Sdf_ty_bool		isRetryAfterPresent = Sdf_co_false;
	Sdf_ty_u32bit 	dDeltaSecs=0;
#endif
    
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_ivk_uaAddStatusCodeSpecificHeaders");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr==Sdf_co_null)
	{
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
			"Invalid parameter passed to the function");
		return Sdf_co_fail;
	}
#ifdef SDF_LINT    
    if(pCallObj==Sdf_co_null)
    {
#ifdef SDF_ERROR		
        sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit*)"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
                "Invalid parameter passed to the function", pErr);
#endif		
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }
#endif     
	if(pMesg==Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
			"Invalid parameter passed to the function", pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(statuscode==0)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
			"Invalid parameter passed to the function", pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	switch(statuscode)
	{
 /* Adding of retry after header is given application scope for B2B.(bcz it is 'MAY' condition according to RFC-3261)*/      
#ifndef SDF_UAB2B
		case 400:
		case 404:
		case 413:
		case 480:
		case 486:
		case 500:
		case 503:
		case 600:
		case 603:
		{
			/* Check the Retry-After is present in slProcess header?
			 * if not then only do below */
            Sdf_st_listIterator dListIterator;
            Sdf_st_headerList 	*pHeaderList = Sdf_co_null;
            (void)sdf_listInitIterator(	\
                    &(pCallObj->pUasTransaction->slProcessedHeaders), \
                    &dListIterator, pErr);
			while (dListIterator.pCurrentElement != Sdf_co_null)
			{
				pHeaderList = (Sdf_st_headerList*)\
					(dListIterator.pCurrentElement->pData);
#if defined(MICROUA) && !defined(SIP_DATE)
				if (pHeaderList->dType == SipHdrTypeRetryAfter)
#else
				if ((pHeaderList->dType == SipHdrTypeRetryAfterSec)\
					||(pHeaderList->dType == SipHdrTypeRetryAfterDate))
#endif
				{
					isRetryAfterPresent = Sdf_co_true;
					break;
				}
                 (void)sdf_listNext(&dListIterator, pErr);
			}
			if( isRetryAfterPresent == Sdf_co_true )
				break;

			/*
			 * Add a retry-after header for these status codes.
			 */
			if ( sdf_fn_uaGetDeltaSecondsForRetryAfter( statuscode, \
				(Sdf_ty_u32bit *)&dDeltaSecs) == Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
					"Failed to get delta seconds for Retry After header", \
					pErr);
#endif				
				return Sdf_co_fail;
			}
#if defined(MICROUA) && !defined(SIP_DATE)
			if(sip_initSipHeader(&pHdr,SipHdrTypeRetryAfter, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
#else
			if(sip_initSipHeader(&pHdr,SipHdrTypeRetryAfterSec, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
#endif
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
					"Could Not Allocate Memory for Retry After Header", pErr);
#endif				
				return Sdf_co_fail;
			}
			if(sip_setDeltaSecondsInRetryAfterHdr(pHdr,  \
				dDeltaSecs,(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
					"Could Not Set the delta seconds filled in Retry "
					"After Header", pErr);
#endif				
				sip_freeSipHeader(pHdr);
                (void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
				return Sdf_co_fail;
			}	
			if(sip_setHeader(pMesg,pHdr,(SipError *)&(pErr->stkErrCode)) \
				==SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
					"Could Not Set the  Retry After Header", pErr);
#endif				
				sip_freeSipHeader(pHdr);

				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
				return Sdf_co_fail;
			}	
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
			break;
		}
#endif
		case 200:
		case 405:
		{
			Sdf_st_headerList	*pAllowHdrList=Sdf_co_null;
			Sdf_ty_u32bit		dIndex=0;
			/*
			 * Add an Allow header(s) for success response and for "method not
			 * allowed" response. Also add supported header(s).
			 */
			if (sdf_fn_uaGetAllowHeaderList(pCallObj, pInitData, \
					&pAllowHdrList,	pErr) == Sdf_co_fail)
			{
				pErr->errCode=Sdf_en_headerListAccessError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaAddStatusCodeSpecificHeaders:" \
					"Error accessing the initData from the call object",pErr);
#endif
				return Sdf_co_fail;
			}
			/*- If ALLOW header is present, then add it to pSipMsg -*/
			if (pAllowHdrList != Sdf_co_null)
			{
				Sdf_st_listIterator dListIterator;
				
			    (void)sdf_listInitIterator(&(pAllowHdrList->slHeaders), \
					&dListIterator, pErr);

				while (dListIterator.pCurrentElement != Sdf_co_null)
				{
					pHdr = (SipHeader *)\
						(dListIterator.pCurrentElement->pData);

					if(sip_insertHeaderAtIndex(pMesg, pHdr, dIndex, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
					{
						pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR							
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit*) "sdf_fn_uaAddAllowHeader(): "
							"Could Not insert Header into SipMessage",pErr);
#endif							
						(void)sdf_ivk_uaFreeHeaderList(pAllowHdrList);
						return Sdf_co_fail;
					}
					dIndex++;
					(void)sdf_listNext(&dListIterator, pErr);
				}
				(void)sdf_ivk_uaFreeHeaderList(pAllowHdrList);
			}

			/* if the response code is 200 and the method is OPTIONS then
			 * insert the Accept, Accept-Encoding, and Accept-Language
			 * This has been done in the switch case 415 
			 */
			if(statuscode !=200)
		 		break;
			else
			{
				Sdf_ty_s8bit *pMethod=Sdf_co_null;

				/*- As per section 13.3.1.4 Rfc3261, para2, 200OK to INVITE
				 * (& OPTIONS) should contain Supported header -*/
		   		 /*w534*/(void)sdf_ivk_uaGetMethodFromSipMessage(pMesg, &pMethod, pErr);

				if ((Sdf_mc_strcasecmp("OPTIONS",pMethod) != 0) || \
					(Sdf_mc_strcasecmp("INVITE",pMethod) != 0))
					if (sdf_fn_uaAddSupportedHdrToSipMsg(pMesg, pInitData, \
								pErr) == Sdf_co_fail)
					{
						pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit*) "sdf_fn_uaAddSupportedHdrToSipMsg"
							"Could Not insert Supported Header into SipMsg",\
							pErr);
#endif
						return Sdf_co_fail;
					}
				
				if(Sdf_mc_strcasecmp("OPTIONS",pMethod) !=0)
					break;
			}
            break;
		}
		case 415:
		{

			Sdf_ty_u32bit dSize1;

			if (pInitData == Sdf_co_null)
				return Sdf_co_fail;

			(void)sdf_listSizeOf(&(pInitData->slExtraHeadersList), &dSize1, pErr);
		   	if(dSize1!=0)
			{
				Sdf_st_configExtraHeaders *pTemp = Sdf_co_null;
				Sdf_st_configExtraHeaders *pAcceptParam = Sdf_co_null;
				Sdf_st_configExtraHeaders *pAcceptEncodingParam = Sdf_co_null;
				Sdf_st_configExtraHeaders *pAcceptLanguageParam = Sdf_co_null;

				Sdf_st_listIterator dListIterator;

				/* 
				 * Get the Accept header from the list
				 */

				(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList), \
					&dListIterator, pErr);

				while (dListIterator.pCurrentElement != Sdf_co_null)
				{
					pTemp =(Sdf_st_configExtraHeaders *)\
							(dListIterator.pCurrentElement->pData);

					if(Sdf_mc_strcasecmp("ACCEPT",pTemp->pParam->pName)==0)
					{
						pAcceptParam=pTemp;
					}
					else if(Sdf_mc_strcasecmp("ACCEPT-ENCODING", \
								pTemp->pParam->pName)==0)
					{
						pAcceptEncodingParam=pTemp;
					}
					else if(Sdf_mc_strcasecmp("ACCEPT-LANGUAGE", \
								pTemp->pParam->pName)==0)
					{
						pAcceptLanguageParam=pTemp;
					}
						
					(void)sdf_listNext(&dListIterator, pErr);
				}

				/*Add the Accpet header into Response if present in the profile
				 */
				if(pAcceptParam !=Sdf_co_null)
				{
					if(sdf_fn_uaInsertHeaderFromProfile(pAcceptParam,\
								pMesg,pErr)==Sdf_co_fail)
					{
#ifdef SDF_ERROR							
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit*)\
							"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
							"Could not Insert Accept header into the resp", \
						   	pErr);
#endif							
						return Sdf_co_fail;
	
					}
				}
			
				/*Add the Accpet-Encoding header into Response 
				 * if present in the profile
				 */
				if(pAcceptEncodingParam !=Sdf_co_null)
				{
					if(sdf_fn_uaInsertHeaderFromProfile(pAcceptEncodingParam,\
								pMesg,pErr)==Sdf_co_fail)
					{
#ifdef SDF_ERROR							
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit*)\
							"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
							"Could not Insert Accept-Encoding header into the"
						   " response", pErr);
#endif							
						return Sdf_co_fail;
	
					}
				}
				/*Add the Accpet-Language header into Response 
				 * if present in the profile
				 */
				if(pAcceptLanguageParam !=Sdf_co_null)
				{
					if(sdf_fn_uaInsertHeaderFromProfile(pAcceptLanguageParam,\
								pMesg,pErr)==Sdf_co_fail)
					{
#ifdef SDF_ERROR							
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit*)\
							"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
							"Could not Insert Accept-Language header into the"
						   " response", pErr);
#endif							
						return Sdf_co_fail;
	
					}
				}
			}
			break;
		}
		case 420:
		{
			Sdf_ty_u32bit size1,size2,IndicateSupported=0;
			Sdf_st_configExtraHeaders *pTemp;
			Sdf_st_listIterator dListIterator;

			if (pInitData == Sdf_co_null)
				return Sdf_co_fail;

			(void)sip_listSizeOf(&(pMesg->pGeneralHdr->slRequireHdr), \
			    &size1,(SipError*)&(pErr->stkErrCode));

			(void)sdf_listSizeOf(&(pInitData->slExtraHeadersList), &size2, \
				pErr);

			(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList), \
				&dListIterator, pErr);

			while (dListIterator.pCurrentElement != Sdf_co_null)
			{
				pTemp =(Sdf_st_configExtraHeaders*)\
					(dListIterator.pCurrentElement->pData);

				if((Sdf_mc_strcasecmp("SUPPORTED",pTemp->pParam->pName)==0)|| \
					(Sdf_mc_strcasecmp("K",pTemp->pParam->pName)==0))
				{
					IndicateSupported = 1;
					break;
				}
				(void)sdf_listNext(&dListIterator, pErr);
			}

			if((size2==0)||(IndicateSupported==0))
			{
				SipHeader dTempVal;
				Sdf_ty_s8bit *pUnsupp,*pToken=Sdf_co_null;
				/*supported header not there in config.txt*/
				/*form Require Header as an Unsupported header*/
				for(dCount=0;dCount<size1;dCount++)
				{
					if (sip_getHeaderAtIndex(pMesg, \
					   SipHdrTypeRequire, &dTempVal,(Sdf_ty_u32bit)dCount,
					   (SipError *)&(pErr->stkErrCode)) == SipFail)
					{
						pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR						
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit*) \
							"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
							"Could Not Get Require Header from Sip Message", \
							pErr);
#endif						
						return Sdf_co_fail;
					}

					if(sip_getTokenFromRequireHdr(&dTempVal,&pUnsupp, \
						(SipError*)&(pErr->stkErrCode))==SipFail)
					{
						pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR						
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit*) \
							"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
							"Could Not Get Token from Require Header", \
							pErr);
#endif						
						return Sdf_co_fail;
					}

					Sdf_mc_strdup(pToken,pUnsupp);

					if(sip_initSipHeader(&pHdr,SipHdrTypeUnsupported, \
						(SipError*)&(pErr->stkErrCode))==SipFail)
					{
						pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR						
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit*) \
							"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
							"Could Not Initialize SipHeader structure", \
							pErr);
#endif						
						return Sdf_co_fail;
					}

					if(sip_setOptionInUnsupportedHdr(pHdr,pToken, \
						(SipError*)&(pErr->stkErrCode))==SipFail)
					{
						pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR						
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit*) \
							"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
							"Could Not Set Option in Unsupported Header", \
							pErr);
#endif						
						return Sdf_co_fail;
					}

					if(sip_insertHeaderAtIndex(pMesg,pHdr,dCount, \
						(SipError*)& (pErr->stkErrCode))==SipFail)
					{
						pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR						
						sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
							(Sdf_ty_s8bit*) \
							"sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
							"Could Not Set Option in Unsupported Header",pErr);
#endif						
					    return Sdf_co_fail;
					}
				}
				pErr->errCode=Sdf_en_noUaError;
				sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
					"Exiting sdf_ivk_uaAddStatusCodeSpecificHeaders");
				return Sdf_co_success;
			}
			break;	
		}
		case 421:
		{
			if((sdf_fn_uaInsertRequireHdr(pMesg, pInitData, pErr)==Sdf_co_fail))
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR						
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*) "sdf_ivk_uaAddStatusCodeSpecificHeaders(): "
					"Failed to Add Require Header",pErr);
#endif						
				return Sdf_co_fail;
			}
			break;
		}
		default:
		{
			pErr->errCode=Sdf_en_noUaError;
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
				"Exiting  sdf_ivk_uaAddStatusCodeSpecificHeaders");
			return Sdf_co_success;
		}
	}
	
	pErr->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exiting  sdf_ivk_uaAddStatusCodeSpecificHeaders");
	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaFormRequest
 **
 ** DESCRIPTION: 	This function forms a request SIP Message depending 
 **					on the input METHOD and fills it into the pSipMessage 
 **					structure in the UAC Transaction or in the overlap 
 **					transaction. 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormRequest
#ifdef ANSI_PROTO 
	(const Sdf_ty_s8bit *pMethod, 
	Sdf_st_callObject *pObject,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	Sdf_st_error *pError)
#else
	(pMethod, pObject, pOverlapTransInfo, pError)
	const Sdf_ty_s8bit *pMethod;
	Sdf_st_callObject *pObject;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_st_error *pError;
#endif
{
	SipMessage 			*pMessage = Sdf_co_null;
	Sdf_ty_messageType 	dMethodType = Sdf_en_unknownRequest;
	Sdf_ty_u16bit 		dRespCode=0;
	Sdf_ty_bool			dRouteCorNeeded = Sdf_co_true;
	Sdf_st_msgInfo		dMessageInfo;

#ifndef SDF_UASSF_ONLY /* LINT fix */
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
	en_AddrType         dContactAddrType = SipAddrSipUri;
#endif /* SDF_UASSF */
	Sdf_ty_bool			dStrictCheck = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering  sdf_ivk_uaFormRequest");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
		return Sdf_co_fail;

	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
			"Invalid parameter passed to the function", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if (pObject->pUacTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
			"UAC Transaction is not initialized in the call object", pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */
	
	/*w534*/(void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMethodType, pError);

	/* 
	 * Fixing the tag problem in To header:
	 * When uac recieves 401/407 with tag in to header the uac 
	 * should not send tag in to header of the subsequent request
	 * accept ACK.
	 * 
	 * Also, this code will set the dCaller member of the call
	 * object key to Sdf_co_true. This is because a request is being
	 * sent on a call object whose state is idle. Hence, this is either
	 * a fresh INVITE, or a fresh OPTIONS/SUBSCRIBE/NOTIFY
	 */
	if (pObject->pCallInfo->dState == Sdf_en_idle) 
	{
		/*This Portion is commented for the following reason
		 * 1. Whenever the User Wishes to form a Fresh Request. He is
		 * supposed to give a Fresh CallObject. If he gives a used
		 * callobject and the To Tag is present. The application will get 
		 * a Request with a To Tag.
		 *
		 * 2. Suppose, we assume the case of deleting the To tag based on
		 * the main call state. We will have problems when app is using
		 * this function to make Regular Transactions with the Existing
		 * CallObject. His intent of using the Exisiting CallObject will be
		 * to get the CallId,CSeq,To Tag etc to be reused. If we remove the
		 * To Tag then we are not serving the purpose.
		 */
		
		/*
		Sdf_ty_u32bit count = 0;

		if(sip_getTagCountFromToHdr(pObject->pCommonInfo->pTo,&count,\
			(SipError *)&(pError->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "\
				"Failed to init get tag count from to header", pError);
#endif		
			pError->errCode=Sdf_en_formRequestError;
			return Sdf_co_fail;
		}

		if (count != 0)
		{
			Sdf_ty_u32bit   	dIndex = 0;

			for(dIndex=0;dIndex<count;dIndex++)
			{
				if(sip_deleteTagAtIndexInToHdr(pObject->pCommonInfo->pTo, \
					dIndex, (SipError *)&(pError->stkErrCode))==SipFail)
				{
#ifdef SDF_ERROR		
					sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError\
						,(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "\
						"Failed to init get tag count from to header", pError);
#endif		
					pError->errCode=Sdf_en_formRequestError;
					return Sdf_co_fail;
				}
			}
		}
		*/

		/*
		 * If the dCaller variable has not yet been set, set it here.
		 * This is because, if a call object is used for SUBSCRIBE/NOTIFY
		 * and the remote party sent the SUBSCRIBE say, the main call state 
		 * will be Sdf_en_idle, but the dCaller would have been set to 
		 * Sdf_en_callee
		 */
		if (pObject->pCommonInfo->pKey->dCaller == Sdf_en_any)
			pObject->pCommonInfo->pKey->dCaller = Sdf_en_caller;
	}

	if(dMethodType == Sdf_en_ack)
	{
		/*w534*/(void)sdf_ivk_uaGetRespCodeFromSipMessage( 
			pObject->pUacTransaction->pSipMsg, &dRespCode, pError);
	}

	dMessageInfo.pMethod 	= Sdf_co_null;
	dMessageInfo.dMsgType 	= dMethodType;
	dMessageInfo.dReqRespType = SipMessageRequest;
	dMessageInfo.dResponse 	= dRespCode;

	if (sip_initSipMessage(&pMessage, SipMessageRequest,(SipError *) \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
			"Failed to init Sip Message", pError);
#endif		
		pError->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	if(sdf_fn_uaFormRequestLine(pObject, pOverlapTransInfo, pMethod, \
			pMessage, dMessageInfo, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
			"Failed to Form Request Line", pError);
#endif		
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}

	if( Sdf_co_fail == sdf_fn_uaFormViaHeader(pObject, pOverlapTransInfo, \
			dMethodType, pMessage, pError))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
			"Failed to Form Request Line", pError);
#endif		
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}
	if(((dMethodType == Sdf_en_ack) && (dRespCode > 299)) || \
		(dMethodType == Sdf_en_cancel) )
	{
		if( Sdf_co_fail == sdf_fn_uaCopyRouteToMessage(pObject, pMessage, \
				   		pError))
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest( ): "
				"Failed to Copy the Invite Route to Processed headers", pError);
#endif		
			pError->errCode=Sdf_en_callObjectAccessError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}
		dRouteCorNeeded = Sdf_co_false;
	}
	/** Setting the Processed Headers */
	if (sdf_fn_uaFormProcessedHeaders(pObject, pOverlapTransInfo, dMessageInfo,\
		pMessage, Sdf_co_false, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest( ): "
			"Failed to form Processed headers", pError);
#endif		
		pError->errCode=Sdf_en_callObjectAccessError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}
	/* For ACK to 2xx Requests, extract the same Require and proxy-Require
	 * headers from persistent list and insert it. */
	if((dMethodType == Sdf_en_ack) && (dRespCode < 300))
	{
		if(sdf_fn_uaFormRequireHdr(pObject, pMessage, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
				"Failed to form Require header", pError);
#endif		
			pError->errCode=Sdf_en_callObjectAccessError;
		}
		/* No need to return failure from here. Flow can continue on
		 * failure of setting require header */
	}
	
	/** Set the From and To headers **/
	if (sdf_fn_uaFormToAndFromHeaders(pObject, pMessage, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
			"Failed to form To and From headers.", pError);
#endif		
		pError->errCode=Sdf_en_formRequestError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}
	/* ACK to Non 2xx shall contain the same To Tag as received in Non 2xx
	 * response. Correct the To Tag*/
	if((dMethodType == Sdf_en_ack) && (dRespCode >= 300))
	{
		SipHeader		dFormedToHeader;
		SipHeader		dRecvdToHeader;
		Sdf_ty_s8bit	*pFormedToTag = Sdf_co_null;
		Sdf_ty_s8bit	*pRecvdToTag = Sdf_co_null;

		if (sip_getHeader(pMessage, SipHdrTypeTo, &dFormedToHeader,\
			(SipError*)	&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
				"Failed to Get the To header From formed Message", pError);
#endif		
			pError->errCode=Sdf_en_formRequestError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}

		if (sip_getHeader(pObject->pUacTransaction->pSipMsg, SipHdrTypeTo,\
				&dRecvdToHeader, (SipError*)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
				"Failed to Get the To header From Received Response", pError);
#endif		
			pError->errCode=Sdf_en_formRequestError;
			sip_freeSipMessage(pMessage);
			sip_freeSipHeader(&dFormedToHeader);
			return Sdf_co_fail;
		}
		/*w534*/(void)sip_getTagAtIndexFromToHdr(&dRecvdToHeader, &pRecvdToTag, \
			0, (SipError*)&(pError->stkErrCode));
		
		/*w534*/(void)sip_getTagAtIndexFromToHdr(&dFormedToHeader, &pFormedToTag, \
			0, (SipError*)&(pError->stkErrCode));

		if(pRecvdToTag!=Sdf_co_null &&\
	       		Sdf_mc_strcmp(pFormedToTag, pRecvdToTag) != 0)
		{
			/*w534*/(void)sip_setTagAtIndexInToHdr(&dFormedToHeader, \
				pRecvdToTag, 0, \
				/*Sdf_mc_strdupCallHandler(pRecvdToTag), 0, \ */
				(SipError *)&(pError->stkErrCode));
		}
        else if(pRecvdToTag==Sdf_co_null && pFormedToTag!=Sdf_co_null)
        {
            (void)sip_deleteTagAtIndexInToHdr(&dFormedToHeader, \
                    0, (SipError *)&(pError->stkErrCode));

        }

		sip_freeSipHeader(&dFormedToHeader);
		sip_freeSipHeader(&dRecvdToHeader);
	}
	/* *  Setting the Call-ID Header * */
	if (sdf_fn_uaFormCallIdHeader(pObject, pMessage, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest( ): "
			"Failed to form Call-Id header", pError);
#endif		
		pError->errCode=Sdf_en_callObjectAccessError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}

	/* *  Setting the CSeq Header * */
	if (sdf_fn_uaFormCseqHeader(pObject, pOverlapTransInfo, pMessage,	\
		pMethod, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest( ): "
			"Failed to form Cseq header", pError);
#endif		
		pError->errCode=Sdf_en_callObjectAccessError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}
	
#ifdef SDF_SESSION_TIMER
	if(sdf_fn_uaAddSessionTimerSpecificHeader(pObject, pMessage, \
			dMethodType, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
			"Failed to add Session-Timer Specific headers in SIP Message",\
			pError);
#endif		
		pError->errCode=Sdf_en_formRequestError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}
#endif	

#ifdef SIP_RPR
	/*
	 * Adding the RAck header in case of PRACK
	 * The Rseq part of the header is retrieved from the 
	 * Sdf_st_overlapTransInfo structure's dRseq member. The Cseq part
	 * of the header is retrieved from the Sdf_st_commonInfo's dLocalCseq 
	 * member
	 */
	if (dMethodType == Sdf_en_prack)
	{
		if(Sdf_co_fail == sdf_fn_uaFormRackHeader(pObject, pOverlapTransInfo,\
				pMessage, pError))
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest( ): "
				"Failed to Form RACK header", pError);
#endif			
			pError->errCode=Sdf_en_formRequestError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}
	}

	/* 
	 * For requests other than PRACK/COMET, set the pRPRMethod to the
	 * method of the request being formed.
	 */
	if ( (dMethodType != Sdf_en_prack) && \
		(dMethodType != Sdf_en_comet) && (dMethodType != Sdf_en_ack)
#ifdef SDF_UAB2B
		&& (dMethodType != Sdf_en_update) && \
		(dMethodType !=	Sdf_en_options)
#endif	
		)
	{
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)&(pObject->pCommonInfo->pRPRMethod), pError);
		pObject->pCommonInfo->pRPRMethod = Sdf_mc_strdupCallHandler(pMethod);
	}
#endif /* SIP_RPR */

#ifdef SDF_TLS
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
if (Sdf_co_fail == sdf_fn_uaGetContactAddrType(pMessage, pObject, \
				&(pObject->pCallInfo->dContactAddrType), SipMessageRequest,\
			   	pError))
#else	

	if (Sdf_co_fail == sdf_fn_uaGetContactAddrType(pMessage, pObject, \
				&dContactAddrType, SipMessageRequest, pError))
#endif	               
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
			"Failed to get Contact header URI type.", pError);
#endif		
		pError->errCode=Sdf_en_callObjectAccessError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}
    
#ifndef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
	pObject->pCallInfo->dContactAddrType = dContactAddrType;
#endif /*end: changes done by subbaroa */	    
#endif /* SDF_TLS */
		
	/*
	 * For BYE and CANCEL requests, we MUST NOT set a Contact header.
	 */
	/*
	 * If it is a REGISTER request, then Contact headers will be added
	 * from the slContact that is passed into MakeRegister. So, do not
	 * add the Contact headers here.
	 */
#ifdef SDF_IM
	if((dMethodType != Sdf_en_bye) && (dMethodType != Sdf_en_cancel) && \
            (dMethodType != Sdf_en_message) && (dMethodType != Sdf_en_register) \
            /*CSR_1-5080393 START*/  
            &&(dMethodType!=Sdf_en_prack)      
            /*CSR_1-5080393 END*/
      )
#else
	if((dMethodType != Sdf_en_bye) && (dMethodType != Sdf_en_cancel) && \
				(dMethodType != Sdf_en_register) \
/*CSR_1-5080393 START*/  
         &&(dMethodType!=Sdf_en_prack)      
/*CSR_1-5080393 END*/
      )
#endif
	{
		Sdf_ty_protocol	dTempTranspType;
		Sdf_st_transportInfo *pDestInfo = Sdf_co_null;

		/*w534*/(void)sdf_ivk_uaGetDestInfoFromTransaction(pObject->pUacTransaction, \
			   pOverlapTransInfo, &pDestInfo, pError);
        if (pDestInfo->dScheme == Sdf_en_protoNone)
        {
           dTempTranspType=pObject->pCommonInfo->dScheme;
        }
        else
        {
           dTempTranspType=pDestInfo->dScheme;
        }

		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);

		/* Check for the Contact header Mandatory/Optional. */
		dStrictCheck = sdf_fn_uaIsContactHdrMandatory(SipMessageRequest, \
				dMethodType, dRespCode);

#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
if (sdf_fn_uaFormContactHeaders(&(pObject->pInitData->slContact),\
			pMessage, dTempTranspType, pObject->pCallInfo->dContactAddrType,\
		   	dStrictCheck, pError) == Sdf_co_fail)
#else

		if (sdf_fn_uaFormContactHeaders(&(pObject->pInitData->slContact),\
				pMessage, dTempTranspType, dContactAddrType, dStrictCheck, \
				pError) == Sdf_co_fail)
#endif                
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
				"Failed to form Contact header", pError);
#endif		
			pError->errCode=Sdf_en_callObjectAccessError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}
	}
	
	/* 
     * Deleting the UnProcessedHeaders that have already been processed by
	 * the Application if Required 
     */ 
	
	if (pOverlapTransInfo == Sdf_co_null)
	{
		if ( sdf_listDeleteAll(\
			&(pObject->pUacTransaction->slUnprocessedHeaders), \
			pError) == Sdf_co_fail) 
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
				"Failed to delete Unknown headers from MsgInfo "
				"of call object", pError);
#endif			
			pError->errCode=Sdf_en_callObjectAccessError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}
	}
	else
	{
		if ( sdf_listDeleteAll(&(pOverlapTransInfo->slUnprocessedHeaders), \
			pError) == Sdf_co_fail) 
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
				"Failed to delete Unknown headers from MsgInfo of "
				"call object", pError);
#endif			
			pError->errCode=Sdf_en_callObjectAccessError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}
	}

	
	/* Remove the last route element from the Route header for the loose routing
	 * case. Since the last route elemet made as a request-uri for loose router
	 * case
	 */
	/* For cancel and ACK to Non 2xx no need to correct the route header
	 * because it contains the same route header as that of its original
	 * Invite */
	if(dRouteCorNeeded == Sdf_co_true )
	{
		if(Sdf_co_fail == sdf_fn_uaCorrectRouteHeader(pObject, pMessage, \
					pError))
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): Failed to Correct"
				"Route Header", pError);
#endif			
			pError->errCode=Sdf_en_callObjectAccessError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}
#ifdef SDF_IM
		/* If Im URI is present in Request verify whether it is allowed or not.
		 * If resolving the IM url fails, then return error*/
		if(sdf_fn_uaCorrectImUrlInRoute(pMessage, pError) != Sdf_co_success)
		{
			sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
				"The scheme in Route or Record Header is not allowed", pError);
#endif			
			return Sdf_co_fail;
		}
#endif
#ifdef SDF_PRES
		/* If presURI is present in Request verify whether it is allowed or not.
		 * If resolving the Pres url fails, then return error*/
		if(sdf_fn_uaCorrectPresUrlInRoute(pMessage, pError) != Sdf_co_success)
		{
			sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "
				"The scheme in Route or Record Header is not allowed", pError);
#endif			
			return Sdf_co_fail;
		}
#endif
	}

	
	/* Stroe the Route set in the commoninfo:
	 * Since Cancel should go with same route as in INVITE
	 * and the route set may change in RPR or 2xx
	 */
	if (dMethodType == Sdf_en_invite)
	{
		if(Sdf_co_fail == sdf_fn_uaStoreRouteHeader(pObject,pMessage,pError))
		{
			sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest( ): "
				"Failed to Store the Route Header into Call Object", pError);
#endif			
			pError->errCode=Sdf_en_commonInfoAccessError;
			return Sdf_co_fail;
		}
	}

/* IPTK Auth Change Start */
/* Code for updating authorization credentials i.e 
   * sdf_fn_uaUpdateAuthorization was present here.
   * It has been removed from here as a part of CSR 1-4878211
   * This has been done to ensure that while credentials are getting
   * recomputed, the message body is a part of the SIP message so that
   * auth-int quality parameter is supported.
   * Now the code for updaing authorization is done by
   * sdf_fn_uaUpdateAuthorization. This function has to be invoked by the
   * application. The usage of sdf_fn_uaUpdateAuthorization has been
   * demonstrated in the hssUA application. */
#if 0
#ifdef SDF_AUTHENTICATE
	/*  For ACK don't change the credentials*/
	if( dMethodType != Sdf_en_ack )
	{
		/* Update the Authorization header with auth-info header*/
		if(Sdf_co_fail == sdf_fn_uaUpdateAuthorization(pObject, pMessage, \
				pMethod,pError))
		{
			sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest( ): "
				"Failed to Update the Authorization Information", pError);
#endif			
			pError->errCode=Sdf_en_commonInfoAccessError;
			return Sdf_co_fail;
		}
	}
#endif
#endif
/* IPTK Auth Change End */
	
	/*
	 * Put the completed SipMessage structure in either the UAC Transaction
	 * or the overlap transaction.
	 */
	if (pOverlapTransInfo == Sdf_co_null)
	{
		sip_freeSipMessage(pObject->pUacTransaction->pSipMsg);
		pObject->pUacTransaction->pSipMsg = pMessage;
	}
	else
	{
		sip_freeSipMessage(pOverlapTransInfo->pSipMsg);
		pOverlapTransInfo->pSipMsg = pMessage;
	}

	pError->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exiting  sdf_ivk_uaFormRequest");
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaFormResponseMessage
 **
 ** DESCRIPTION: 	This function forms a response message from the input 
 **					request message. General Headers are copied from
 ** 				input request msg to output msg. The type of response 
 **					is dependent on the RespCode passed as a parameter
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaFormResponseMessage
#ifdef ANSI_PROTO
	(Sdf_st_initData	*pInitData,
	SipMessage 			*pInputMessage,
	SipMessage 			**ppOutputMessage, 
	Sdf_ty_u16bit 		dRespCode, 
	Sdf_st_error 		*pError)
#else
	(pInitData, pInputMessage, ppOutputMessage, dRespCode, pError)
	Sdf_st_initData	*pInitData;
	SipMessage 		*pInputMessage;
	SipMessage 		**ppOutputMessage;
	Sdf_ty_u16bit 	dRespCode;
	Sdf_st_error 	*pError;
#endif
{
	SipStatusLine 	*pStatLine = Sdf_co_null;
	SipHeader 		dHeader;
	Sdf_ty_u32bit	dIndex=0, count=0;
	Sdf_ty_s8bit 	*pReason = Sdf_co_null;
	Sdf_ty_s8bit 	*pTmpVal = Sdf_co_null;
	Sdf_ty_s8bit	*pToTag = Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering  sdf_ivk_uaFormResponseMessage");

	if (sdf_fn_uaGetReasonFromTable(dRespCode, &pReason, pError) == \
			Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage(): "
			"Failed to get reason for response code", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail; 
	}

	if (sip_initSipMessage(ppOutputMessage, SipMessageResponse, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage(): "
			"Failed to init Sip Message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}


	/* Setting the Status Line */
	if (sip_initSipStatusLine(&pStatLine, (SipError *)&(pError->stkErrCode)) \
		== SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage(): "
			"Failed to init Sip Status line", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}
	if (sip_setReasonInStatusLine(pStatLine, pReason, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage(): "
			"Failed to set reason in Status line", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}
	pTmpVal = 	Sdf_mc_strdupCallHandler("SIP/2.0");

	if (sip_setVersionInStatusLine(pStatLine, pTmpVal, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
			"Failed to set Version in Status Line", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}
	if (sip_setStatusCodeNumInStatusLine(pStatLine,dRespCode, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
			"Failed to set Status code in Status Line", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}
	if (sip_setStatusLineInSipRespMsg (*ppOutputMessage, pStatLine, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
			"Failed to set Status Line in Response Message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}
	/* Free the Local Reference */
	sip_freeSipStatusLine(pStatLine);

		/* Setting the Record-Route header in the output SipMessage */
	/*w534*/(void)sip_getHeaderCount(pInputMessage, SipHdrTypeRecordRoute, &count, \
		(SipError *)&(pError->stkErrCode));

	for (dIndex = 0; dIndex < count; dIndex++)
	{
		if (sip_getHeaderAtIndex(pInputMessage, SipHdrTypeRecordRoute, \
			&dHeader, (Sdf_ty_u32bit)dIndex, (SipError *)&(pError->stkErrCode)) \
			== SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage(): "
				"Failed to get Record route header", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			return Sdf_co_fail;
		}

		if (sip_insertHeaderAtIndex(*ppOutputMessage, &dHeader, \
			(Sdf_ty_u32bit)dIndex, (SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
				"Failed to insert Record route header", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			sip_freeSipHeader(&dHeader);	
			return Sdf_co_fail;
		}
		sip_freeSipHeader(&dHeader);	
	}

	/* * Setting the Via Header * */
	/*w534*/(void)sip_getHeaderCount(pInputMessage, SipHdrTypeVia, &count, \
		(SipError *)&(pError->stkErrCode));

	for (dIndex = 0; dIndex < count; dIndex++)
	{
		if (sip_getHeaderAtIndex(pInputMessage, SipHdrTypeVia, &dHeader, \
			(Sdf_ty_u32bit)dIndex, (SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
				"Failed to get Via header from input message", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			return Sdf_co_fail;
		}
		if (sip_insertHeaderAtIndex(*ppOutputMessage, &dHeader, \
			(Sdf_ty_u32bit)dIndex, (SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
				"Failed to set Via header in output message", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			sip_freeSipHeader(&dHeader);	
			return Sdf_co_fail;
		}
		sip_freeSipHeader(&dHeader);	
	}

	/* Setting the From header in the output SipMessage */
	if (sip_getHeader(pInputMessage, SipHdrTypeFrom, &dHeader, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
			"Failed to get From header from Input Message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(*ppOutputMessage);
		return Sdf_co_fail;
	}
	if (sip_setHeader(*ppOutputMessage, &dHeader,(SipError *)&(\
		pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage(): "
			"Failed to set From header in Outgoing Message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(*ppOutputMessage);
		return Sdf_co_fail;
	}
	sip_freeSipHeader(&dHeader);
		
	/* Setting the To header in the output SipMessage */
	if (sip_getHeader(pInputMessage, SipHdrTypeTo, &dHeader, \
	(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
			"Failed to get To header from Sip Message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(*ppOutputMessage);
		return Sdf_co_fail;
	}
	/* If To Tag is not present in Incoming Sip Message, then insert it */
	/*w534*/(void)sip_getTagAtIndexFromToHdr(&dHeader, &pToTag, 0, \
					(SipError*)&(pError->stkErrCode));

	if (pToTag == Sdf_co_null)
	{
		/*w534*/(void)sdf_fn_uaGenerateGlobalTag( pInitData->pTag, &pToTag);

		/*w534*/(void)sip_insertTagAtIndexInToHdr(&dHeader, \
				pToTag, 0, (SipError *)&(pError->stkErrCode));
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)(&pToTag), pError);
	}
	
	if (sip_setHeader(*ppOutputMessage, &dHeader,(SipError *)&(\
		pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
			"Failed to set To header in Outgoing Message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(*ppOutputMessage);
		return Sdf_co_fail;
	}

	sip_freeSipHeader(&dHeader);


	/* Setting the CallId header in the output SipMessage */
	if (sip_getHeader(pInputMessage, SipHdrTypeCallId, &dHeader, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage(): "
			"Failed to get Call-ID header from input message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(*ppOutputMessage);
		return Sdf_co_fail;
	}
	if (sip_setHeader(*ppOutputMessage, &dHeader, (SipError *)&(\
		pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage(): "
			"Failed to set Call-ID header in output message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(*ppOutputMessage);
		return Sdf_co_fail;
	}
	sip_freeSipHeader(&dHeader);

	/* Setting the Cseq header in the output SipMessage */
	if (sip_getHeader(pInputMessage, SipHdrTypeCseq, &dHeader, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
			"Failed to get Cseq header from input message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(*ppOutputMessage);
		return Sdf_co_fail;
	}
	if (sip_setHeader(*ppOutputMessage, &dHeader, (SipError *)&(\
		pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
			"Failed to set Cseq header in output message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(*ppOutputMessage);
		return Sdf_co_fail;
	}
	sip_freeSipHeader(&dHeader);

#ifdef SIP_TIMESTAMP
	/* Setting the Timestamp header in the output SipMessage */
	if (sip_getHeader(pInputMessage, SipHdrTypeTimestamp, &dHeader, \
		(SipError *)&(pError->stkErrCode)) != SipFail)
	{
		if (sip_setHeader(*ppOutputMessage, &dHeader, (SipError *)&(\
			pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
				"Failed to set timestamp header in output message", \
				pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			sip_freeSipMessage(*ppOutputMessage);
			return Sdf_co_fail;
		}
		sip_freeSipHeader(&dHeader);
	}
#endif /* SIP_TIMESTAMP */	
#ifdef SDF_3GPP
	/* set the Histroty-Info header present in the request into the
	 * response if it is a final response*/
	if(dRespCode>199)
	{
		(void)sip_getHeaderCount(pInputMessage, SipHdrTypeHistoryInfo, &count, \
								 (SipError *)&(pError->stkErrCode));
		for (dIndex = 0; dIndex < count; dIndex++)
		{
		if (sip_getHeaderAtIndex(pInputMessage, SipHdrTypeHistoryInfo,&dHeader,
						(Sdf_ty_u32bit)dIndex, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
						(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage(): "
						"Failed to get History-info header", pError);
#endif			
				pError->errCode=Sdf_en_formResponseError;
				return Sdf_co_fail;
			}

			if (sip_insertHeaderAtIndex(*ppOutputMessage, &dHeader, \
						(Sdf_ty_u32bit)dIndex, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
						(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponseMessage( ): "
						"Failed to insert History-Info header", pError);
#endif			
				pError->errCode=Sdf_en_formResponseError;
				sip_freeSipHeader(&dHeader);	
				return Sdf_co_fail;
			}
			sip_freeSipHeader(&dHeader);	
		}
	}
#endif
	pError->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exiting  sdf_ivk_uaFormResponseMessage");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaFormResponse
 **
 ** DESCRIPTION: 	This function forms a response message and fills it 
 **					into the call object. The type of response msg is 
 **					dependent on the RespCode and the method passed in as
 ** 				parameter
 **
 **					Note (Internal):
 **					The formation of contact headers is after Cseq formation.
 **					Keep it in that order only as Contact formation uses the
 **					method from the Cseq header.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormResponse
#ifdef ANSI_PROTO
	(Sdf_ty_u16bit 			dRespCode, 
	const Sdf_ty_s8bit 		*pMethod,
	Sdf_st_callObject 		*pObject, 
	Sdf_st_overlapTransInfo *pOverlapTransInfo, 
	Sdf_ty_bool 			dReliableResponse,
	Sdf_st_error 			*pError)
#else
	(dRespCode, pMethod, pObject, pOverlapTransInfo, dReliableResponse, pError)
	Sdf_ty_u16bit 			dRespCode;
	const Sdf_ty_s8bit 		*pMethod;
	Sdf_st_callObject 		*pObject;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_ty_bool 			dReliableResponse;
	Sdf_st_error 			*pError;
#endif
{
	SipMessage 				*pMessage = Sdf_co_null;
	SipHeader 				*pHeader = Sdf_co_null, dTempViaHeader;
	SipStatusLine 			*pStatLine = Sdf_co_null;
	Sdf_ty_s8bit 			*pReason = Sdf_co_null;
	Sdf_ty_s8bit			*pTempSentProto = Sdf_co_null;
	Sdf_ty_protocol         dTempTranspType;
	Sdf_st_commonInfo 		*pCommonInfo = Sdf_co_null;
	Sdf_ty_bool				dInviteTransaction = Sdf_co_false;
	Sdf_ty_bool				dStrictCheck = Sdf_co_false;
	Sdf_ty_s8bit			*pTmpVal = Sdf_co_null;
	Sdf_ty_messageType 		dMsgType = Sdf_en_unknownRequest;
	Sdf_st_msgInfo			dMessageInfo;

	(void)dReliableResponse;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaFormResponse");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
		return Sdf_co_fail;

	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Invalid parameter passed to the function", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if (pObject->pUasTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"UAS transaction is not initialized.", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	
#endif	/* End of param validation ifdef */

	(void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, pError);

	dMessageInfo.pMethod = Sdf_co_null;
	dMessageInfo.dMsgType = dMsgType;
	dMessageInfo.dReqRespType = SipMessageResponse;
	dMessageInfo.dResponse = dRespCode;
	/* 
	 * If an RPR is being formed, it should be a response code between
	 * 101 and 199
	 */
	if (dReliableResponse == Sdf_co_true)
	{
		if ( (dRespCode <= 100) || (dRespCode >= 200) )
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Invalid parameter passed to function. RPR is only "
				"for response codes between 101 and 199", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			return Sdf_co_fail;
		}
	}
	
	(void)sdf_fn_uaGetReasonFromTable(dRespCode, &pReason, pError);

		/*got from the global table */
	
	if (sip_initSipMessage(&pMessage, SipMessageResponse, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pReason,pError);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to allocate memory for Sip Message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}
	/* Setting the Status Line */
	if (sip_initSipStatusLine(&pStatLine, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pReason,pError);
		sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to allocate memory for Status Line in SipMessage", \
			pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}	
	if (sip_setReasonInStatusLine(pStatLine, pReason, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pReason,pError);
		sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to set Reason in Status Line of the SipMessage" , pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}	
	pTmpVal = Sdf_mc_strdupCallHandler("SIP/2.0");

	if (sip_setVersionInStatusLine(pStatLine, pTmpVal, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to set Version in Status Line of SipMessage" , pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}	
	if (sip_setStatusCodeNumInStatusLine(pStatLine,dRespCode, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to set Code in Status Line of SipMessage", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}	
	if (sip_setStatusLineInSipRespMsg (pMessage, pStatLine, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to set Status Line in Sip Message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}	

	/* Free the Local Reference */
	sip_freeSipStatusLine(pStatLine);

	/*** Setting the Processed Headers ***/
	if (sdf_fn_uaFormProcessedHeaders(pObject, pOverlapTransInfo, dMessageInfo,\
		pMessage, dReliableResponse, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse(): "
			"Failed to form Processed headers", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}


	/** Set the From and To headers **/
	if (sdf_fn_uaFormToAndFromHeaders(pObject, pMessage, pError) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formResponseError,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse(): "
			"Failed to form To and From headers.", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}

	/* *  Setting the Call-ID Header * */
	if (sdf_fn_uaFormCallIdHeader(pObject, pMessage, pError) == Sdf_co_fail)
	{
		sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse(): "
			"Failed to form Call-Id header", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}

	/* *  Setting the Cseq Header * */
	if (sip_initSipHeader(&pHeader, SipHdrTypeCseq, (SipError *)&(\
		pError->stkErrCode)) == SipFail)
	{
		sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to init Sip header", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}	

	/*
	 * If a final response to a PRACK/COMET is being formed, use
	 * the dCseq in the corresponding Sdf_st_overlapTransInfo structure.
	 * This would have been updated with the Cseq of the PRACK/COMET 
	 * that was received
	 */
	if( ( dMsgType == Sdf_en_invite) ||
	    ( dMsgType == Sdf_en_bye) ||
		( dMsgType == Sdf_en_cancel) ||
		( dMsgType == Sdf_en_ack))
	{
		dInviteTransaction = Sdf_co_true;
	}

#ifdef SIP_RPR	
	if ( ( dMsgType != Sdf_en_prack) && \
	     ( dMsgType != Sdf_en_comet) &&
		 (dInviteTransaction == Sdf_co_true))
#else
	if (( dMsgType != Sdf_en_comet) &&
		 (dInviteTransaction == Sdf_co_true))

#endif /* SIP_RPR */ 
	{
		if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, &pCommonInfo,\
			pError) == Sdf_co_fail)
		{
			sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, 
				Sdf_en_commonInfoAccessError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to get CommomInfo from Call Object", pError);
#endif			
			pError->errCode=Sdf_en_commonInfoAccessError;
			return Sdf_co_fail;
		}	

		if (sip_setSeqNumInCseqHdr(pHeader, pCommonInfo->dRemoteCseq, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
			sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to set CSequence number in CSeq header", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			return Sdf_co_fail;
		}
	
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);

	}
	else
	{		
#ifdef SDF_LINT
		if(pOverlapTransInfo != Sdf_co_null)		
#endif			
			if (sip_setSeqNumInCseqHdr(pHeader, pOverlapTransInfo->dRemoteCseq, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to set CSequence number in CSeq header", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			return Sdf_co_fail;
		}
	}

	pTmpVal = Sdf_co_null;
	pTmpVal = Sdf_mc_strdupCallHandler(pMethod);
	if (sip_setMethodInCseqHdr(pHeader, pTmpVal, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to set method in Cseq header", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}

	if (sip_setHeader(pMessage, pHeader,  \
			(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to set CSeq header in Sip Message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}	
	/* Freeing the Local Reference */
	sip_freeSipHeader(pHeader);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHeader,pError);

#ifdef SDF_SESSION_TIMER	
	if(sdf_fn_uaAddSessionTimerSpecificHeader(pObject,pMessage, dMsgType, \
		pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to set Add Session Timer specific headers"
			"in Sip Message", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}

#endif		

#ifdef SIP_RPR
	/*
	 * Add a Require:100rel header and a Rseq header if an RPR is 
	 * being formed.
	 */
	if (dReliableResponse == Sdf_co_true)
	{
		SipHeader *pRPRHeader;
		Sdf_st_headerList *pAllowHdrList=Sdf_co_null;

		/* Add ALLOW header to RPR's -*/
		/*- Add the list of methods which UATK Supports -*/
		if (sdf_fn_uaGetAllowHeaderList(pObject, pObject->pInitData, \
					&pAllowHdrList, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaFormResponse(): "
					"Failed to get ALLOW Header ",pError);
#endif
			return Sdf_co_fail;
		}
		/*- If ALLOW header is present, then add it to pSipMsg -*/
		if (pAllowHdrList != Sdf_co_null)
		{
			Sdf_ty_u32bit		dIndex = 0;
			Sdf_st_listIterator dListIterator;
			SipHeader 			*pHdr = Sdf_co_null;
			
			(void)sdf_listInitIterator(&(pAllowHdrList->slHeaders), \
				&dListIterator, pError);

			while (dListIterator.pCurrentElement != Sdf_co_null)
			{
				pHdr = (SipHeader *)\
					(dListIterator.pCurrentElement->pData);

				if(sip_insertHeaderAtIndex(pMessage, pHdr, dIndex, \
						(SipError *)&(pError->stkErrCode))==SipFail)
				{
					pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR							
					sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
						(Sdf_ty_s8bit*) "sdf_fn_uaAddAllowHeader(): "
						"Could Not insert Header into SipMessage",pError);
#endif							
					(void)sdf_ivk_uaFreeHeaderList(pAllowHdrList);
					return Sdf_co_fail;
				}
				dIndex++;
				(void)sdf_listNext(&dListIterator, pError);
			}
			(void)sdf_ivk_uaFreeHeaderList(pAllowHdrList);
		}

		/* Set the Require:100 rel header */
		if (sip_initSipHeader(&pRPRHeader, SipHdrTypeRequire, 
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to initialize Require header", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}

		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupCallHandler("100rel");

		if (sip_setTokenInRequireHdr(pRPRHeader, pTmpVal, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to set token in Require header", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			sip_freeSipMessage(pMessage);
			sip_freeSipHeader(pRPRHeader);
			return Sdf_co_fail;
		}

		if (sip_insertHeaderAtIndex(pMessage, pRPRHeader, 0, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to set Require header in Sip Message", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			sip_freeSipMessage(pMessage);
			sip_freeSipHeader(pRPRHeader);
			return Sdf_co_fail;
		}	
		/* Free the local reference */
		sip_freeSipHeader(pRPRHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
			&pRPRHeader, pError);

		/* Require:100rel header set */

		if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, &pCommonInfo, \
			pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to get commonInfo from call object", pError);
#endif			
			pError->errCode=Sdf_en_commonInfoAccessError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}	

		/*
		 * Increment dLocalRseq by 1 and use it in this RPR.
		 * Update the pOverlapTransInfo->dRseq to this value.
		 */
		pCommonInfo->dLocalRseq++;
#ifdef SDF_LINT
		if( pOverlapTransInfo != Sdf_co_null)
#endif		
				pOverlapTransInfo->dRseq = pCommonInfo->dLocalRseq;

		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);

		/* Set the Rseq header */
		if (sip_initSipHeader(&pRPRHeader, SipHdrTypeRSeq, 
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to initialize RSeq header", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			return Sdf_co_fail;
		}

#ifdef SDF_LINT
		if( pOverlapTransInfo != Sdf_co_null)
#endif		
				if (sip_rpr_setRespNumInRSeqHdr(pRPRHeader, pOverlapTransInfo->dRseq,\
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
						sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
						sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
						(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
						"Failed to set RespNum in RSeq header", pError);
#endif			
							pError->errCode=Sdf_en_formResponseError;
						return Sdf_co_fail;
				}
		
		if (sip_setHeader(pMessage, pRPRHeader, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to set RSeq header in SipMessage", pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			return Sdf_co_fail;
		}	
		/* Free the local reference */
		sip_freeSipHeader(pRPRHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
			&pRPRHeader, pError);

		/* Add the new overlapTransInfo structure to the callObject */
		if (sdf_listAppend(&(pObject->slOverlapTransInfo), \
			(Sdf_ty_pvoid) pOverlapTransInfo, pError) == Sdf_co_fail)
		{
			sip_freeSipMessage(pMessage);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to add New Overlap transaction to callObject", \
				pError);
#endif			
			pError->errCode=Sdf_en_formResponseError;
			return Sdf_co_fail;
		}	
	}
#endif /* SIP_RPR */	
/*CSR_1-5080393 START*/
if((
#ifdef SDF_IM
	     (dMsgType != Sdf_en_message)&&  
#endif
         (dMsgType!=Sdf_en_prack)) || ((dMsgType==Sdf_en_prack)&&dRespCode>=300) )
/*CSR_1-5080393 END*/
	{
		/*
		 * Before making the Contact header, find out the transport type
		 * (whether its TCP or UDP). This is required so as to know whether to
		 * add a transport=tcp parameter to the Contact addrspec or not.
		 */
		if (sip_getHeaderAtIndex(pMessage, SipHdrTypeVia, \
			&dTempViaHeader, 0, (SipError*)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse(): "
				"Failed to get Via header at index 0 from the message", pError);
#endif		
			pError->errCode=Sdf_en_formResponseError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;	
		}
		
		if (sip_getSentProtocolFromViaHdr(&dTempViaHeader, &pTempSentProto, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse(): "
				"Failed to get the protocol to be used from the Via header", \
				pError);
#endif		
			pError->errCode=Sdf_en_formResponseError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}

		/* ----
		 * NOTE
		 * ----
		 * If the transport type is not specifically TCP, treat it as
		 * unreliable.
		 */
		if (Sdf_mc_strstr(pTempSentProto, "TCP") != Sdf_co_null)
			dTempTranspType = Sdf_en_protoTcp;
#ifdef SDF_TLS
		else if (Sdf_mc_strstr(pTempSentProto, "TLS") != Sdf_co_null)
			dTempTranspType = Sdf_en_protoTls;
#endif /* SDF_TLS */
		else
			dTempTranspType = Sdf_en_protoUdp;
			
		sip_freeSipHeader(&dTempViaHeader);

		/* Check for the Contact header Mandatory/Optional. */
		dStrictCheck = sdf_fn_uaIsContactHdrMandatory(SipMessageResponse, \
				dMsgType, dRespCode);
		
		/*** Setting Contact Headers ***/
		if (sdf_fn_uaFormContactHeaders(&(pObject->pInitData->slContact), \
			  pMessage, dTempTranspType, pObject->pCallInfo->dContactAddrType, \
			  dStrictCheck,\
			  pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
				"Failed to form Contact header", pError);
#endif		
			pError->errCode=Sdf_en_formResponseError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}
	}	

	/*add any response code specific headers*/
	if((sdf_ivk_uaAddStatusCodeSpecificHeaders( pObject, pObject->pInitData, \
		 pMessage, (Sdf_ty_u16bit)dRespCode,pError)) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse( ): "
			"Failed to add status code specific headers", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		sip_freeSipMessage(pMessage);
		return Sdf_co_fail;
	}	


	
	/* 
     * Deleting the UnProcessedHeaders that have already been processed by
	 * the Application if Required 
     */ 
	
	if (pOverlapTransInfo == Sdf_co_null)
	{
		if ( sdf_listDeleteAll(\
			&(pObject->pUasTransaction->slUnprocessedHeaders), \
			pError) == Sdf_co_fail) 
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse(): "
				"Failed to delete Unknown headers from MsgInfo of "
				"call object", pError);
#endif			
			pError->errCode=Sdf_en_callObjectAccessError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}
	}
	else
	{
		if ( sdf_listDeleteAll(&(pOverlapTransInfo->slUnprocessedHeaders),\
			pError) == Sdf_co_fail) 
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormResponse(): "
				"Failed to delete Unknown headers from MsgInfo of call "
				"object", pError);
#endif			
			pError->errCode=Sdf_en_callObjectAccessError;
			sip_freeSipMessage(pMessage);
			return Sdf_co_fail;
		}
	}

	/*
	 * Store the SipMessage in the UAS transaction structure or in the
	 * overlaptransinfo structure.
	 */
	if (pOverlapTransInfo == Sdf_co_null)
	{
		sip_freeSipMessage(pObject->pUasTransaction->pSipMsg);
		pObject->pUasTransaction->pSipMsg = pMessage;
	}
	else
	{
		sip_freeSipMessage(pOverlapTransInfo->pSipMsg);
		pOverlapTransInfo->pSipMsg = pMessage;
	}

	pError->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaFormResponse");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaUpdateDestinationInfo
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
Sdf_ty_retVal sdf_ivk_uaUpdateDestinationInfo
	(Sdf_st_callObject *pObject,
	Sdf_st_overlapTransInfo *pOverlapTransaction,
	SipMessage *pMessage, 
	Sdf_st_error *pError)
{
	return (sdf_fn_uaHandleLookup (pObject, pOverlapTransaction,
			pMessage, Sdf_co_null,pError));
}

/******************************************************************************
 ** FUNCTION: 		sdf_ivk_uaGetDestinationDetails
 **
 ** DESCRIPTION: 	This function forms the target address spec (based on
 **                 presence of loose-routing) in case of a request or updates
 **                 the DestInfo incase of a response and returns the same as 
 **                 an **                 output parameter.
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
Sdf_ty_retVal sdf_ivk_uaGetDestinationDetails
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	Sdf_st_overlapTransInfo *pOverlapTransaction,
	SipMessage   *pMessage, 
	Sdf_st_destDetails **ppDestDetails,
	Sdf_st_error *pError)
#else
	(pObject,pOverlapTransaction,pMessage,ppDestDetails,pError)
	Sdf_st_callObject *pObject;
	Sdf_st_overlapTransInfo *pOverlapTransaction;
	SipMessage   *pMessage;
	Sdf_st_destDetails **ppDestDetails;
	Sdf_st_error *pError;
#endif
{
	en_SipMessageType		dType;
	Sdf_st_transaction 		*pTransaction=Sdf_co_null;
	Sdf_ty_s8bit	   		*pTmpVal = Sdf_co_null;
	SipAddrSpec  		 	*pAddrSpec=Sdf_co_null;
	SipAddrSpec				*pRouteAddrSpec = Sdf_co_null;
	SipAddrSpec				*pReqLineAddrSpec = Sdf_co_null;
	Sdf_ty_bool   			dIsPresentLr=Sdf_co_false;
	Sdf_ty_u32bit 			dCount=0;
	Sdf_ty_s8bit  			*pTempHost = Sdf_co_null;
	Sdf_ty_s8bit  			*pHost = Sdf_co_null;
	Sdf_st_transportInfo 	*pDestInfo =Sdf_co_null;
	Sdf_ty_s8bit    		dTempAddr[50];
   	SipReqLine 				*pReqLine = Sdf_co_null;
#ifdef SDF_TLS		
		en_AddrType				dSchemeType = SipAddrSipUri;
#endif
	Sdf_ty_s8bit 			*pMethod = Sdf_co_null;
	SipAddrSpec				*pPreloadRouteAddrSpec = Sdf_co_null;
	Sdf_ty_bool			IsMaddrParam=Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_ivk_uaGetDestinationDetails");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetDestinationDetails():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetDestinationDetails( ): "
			"Invalid SipMessage passed to the function", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(ppDestDetails == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetDestinationDetails( ): "
			"ppDestDetails passed to the function is invalid", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End of Param Validation. */

	if (sip_getMessageType(pMessage, &dType, (SipError*)\
		&(pError->stkErrCode)) ==  SipFail)
 	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestinationDetails(): "
			"Failed to get type of SIP Message", pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	if(pOverlapTransaction==Sdf_co_null)
	{
		if(dType==SipMessageRequest)
			pTransaction=pObject->pUacTransaction;
		else
			pTransaction=pObject->pUasTransaction;
	}

	/*
	 * Allocate memory for the structure
	 */
	*ppDestDetails = (Sdf_st_destDetails *) sdf_memget (Sdf_mc_initMemId, \
		sizeof(Sdf_st_destDetails), pError);

	if (dType == SipMessageRequest)
    {
		(*ppDestDetails)->dIsTargetUriSet = Sdf_co_true;

	    /*
	     *	For a Request which is being sent out 
	     *	a) if Route header present in the Message and the top most route is
	     * 		a Loose router, destInfo is set to that IP/port
	     *	b) otherwise, the destInfo is set to IP/Port of the Request-URI
	     */

    	if (sip_getReqLine(pMessage, &pReqLine, \
    			(SipError *)&(pError->stkErrCode)) == SipFail)
    	{
#ifdef SDF_ERROR				
    		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_destinationInfoError,\
    			(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestinationDetails( ): "
    			"Failed to get Request Line from SipMessage",pError);
#endif				
			(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails,pError);
   			pError->errCode=Sdf_en_destinationInfoError;
   			return Sdf_co_fail;
   		}
			
   		if (sip_getAddrSpecFromReqLine(pReqLine, &pReqLineAddrSpec, \
   			(SipError *)&(pError->stkErrCode)) == SipFail)
   		{
   			sip_freeSipReqLine(pReqLine);
#ifdef SDF_ERROR				
   			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_destinationInfoError,\
   				(Sdf_ty_s8bit *)" sdf_ivk_uaGetDestinationDetails( ): "
   				"Failed to get AddrSpec from the Request Line",pError);
#endif				
			(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails,pError);
   			pError->errCode=Sdf_en_destinationInfoError;
   			return Sdf_co_fail;
   		}

   		sip_freeSipReqLine(pReqLine);
#ifdef SDF_TLS			
			dSchemeType = pReqLineAddrSpec->dType;
#endif


	    (void)sip_getHeaderCount(pMessage,SipHdrTypeRoute, &dCount, \
			(SipError *)&(pError->stkErrCode));

	    if(dCount !=0)
	    {
			Sdf_ty_slist 	slRouteHeaderList;
			Sdf_st_headerList *pHeaderList = Sdf_co_null;
			SipHeader *pRouteHeader = Sdf_co_null;


			/* Check whether the top most route entry is lr or not */
			
			/*w534*/(void)sdf_ivk_uaGetHeaderListByCorrelation( \
				&(pObject->slPersistentHeaders), \
				(Sdf_ty_s8bit *)"uat-routeset",	&slRouteHeaderList, pError);

			(void)sdf_listGetAt(&slRouteHeaderList, 0, \
					(Sdf_ty_pvoid *)&pHeaderList, pError);

			(void)sdf_listGetAt(&(pHeaderList->slHeaders), 0, \
					(Sdf_ty_pvoid *)&pRouteHeader, pError);

		    if (sip_getAddrSpecFromRouteHdr(pRouteHeader, &pRouteAddrSpec, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
		    {
		    	pError->errCode = pError->stkErrCode ;
#ifdef SDF_ERROR					
		    	sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
		    		(Sdf_ty_s8bit*)"sdf_ivk_uaGetDestinationDetails( ): "
		    		"Failed to get AddrSpec from Preloaded Info", pError);
#endif	    				
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
				(void)sdf_listDeleteAll(&slRouteHeaderList, pError);
			    return Sdf_co_fail;
		    }
			(void)sdf_listDeleteAll(&slRouteHeaderList, pError);

		    (void)sdf_fn_uaCheckLrParameter(pRouteAddrSpec, &dIsPresentLr,pError);
        }

	  if(dIsPresentLr == Sdf_co_true)
		{
			pAddrSpec = pRouteAddrSpec;
	    	sip_freeSipAddrSpec(pReqLineAddrSpec);
		}
		else
		{
			pAddrSpec = pReqLineAddrSpec;
	    sip_freeSipAddrSpec(pRouteAddrSpec);
		}

		(void)sdf_ivk_uaGetMethodFromSipMessage(pMessage,&pMethod,pError);
	
		if((Sdf_mc_strcmp(pMethod,"CANCEL")==0)||((Sdf_mc_strcmp(pMethod,"ACK")==0)&&(pObject->pCallInfo->dState != Sdf_en_finalSuccessReceived)))
		{
			(void)sdf_listSizeOf(&(pObject->pCommonInfo->slInvRoute),&dCount, pError);
				
			if(dCount!=0)
			{
				SipHeader *pSrcHeader = Sdf_co_null;
				
				(void)sdf_listGetAt(&(pObject->pCommonInfo->slInvRoute),0,
                            (Sdf_ty_pvoid *)&pSrcHeader,pError);
				
				if (sip_getAddrSpecFromRouteHdr(pSrcHeader, &pPreloadRouteAddrSpec, \
						(SipError *)&(pError->stkErrCode)) == SipFail)
				{
						pError->errCode = pError->stkErrCode ;
						(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails,\
						pError);
#ifdef SDF_ERROR					
						sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
																		(Sdf_ty_s8bit*)"sdf_ivk_uaGetDestinationDetails( ): "
																	"Failed to get AddrSpec from Preloaded Info", pError);
#endif	    				
						return Sdf_co_fail;
				}
				dIsPresentLr = Sdf_co_false;

				(void)sdf_fn_uaCheckLrParameter(pPreloadRouteAddrSpec, &dIsPresentLr,
                                        pError);
		
				if(dIsPresentLr == Sdf_co_true)
				{
					sip_freeSipAddrSpec(pAddrSpec);	
					pAddrSpec = pPreloadRouteAddrSpec;
				}
				else
				{
				  sip_freeSipAddrSpec(pPreloadRouteAddrSpec);
				}
			}
		}

#ifdef SDF_LINT			
			if((pAddrSpec != Sdf_co_null)&&
			   (pAddrSpec->dType != SipAddrReqUri))
#else	        
        if((pAddrSpec->dType != SipAddrReqUri))
#endif
		{
			Sdf_ty_u32bit dUrlParamCount = 0;
			SipUrl        *pUrl = Sdf_co_null;

			if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pUrl,\
						pError) == Sdf_co_fail)
			{
				sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR					
				sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_destinationInfoError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestinationDetails( ): "
					"Failed to get Url From Addr Spec",pError);
#endif				
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
				pError->errCode=Sdf_en_destinationInfoError;
				return Sdf_co_fail;
			}

			if (sip_getHostFromUrl(pUrl, &pTempHost,\
				(SipError*)&(pError->stkErrCode)) == SipFail)
			{
				sip_freeSipUrl(pUrl);
				sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR					
				sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_destinationInfoError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestinationDetails( ): "
					"Failed to get Host from Url",pError);
#endif					
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
				pError->errCode=Sdf_en_destinationInfoError;
				return Sdf_co_fail;
			}

			/*
			 * Store the reference so that it can freed if needed.
			 */
			pHost = pTempHost;

			/* 
			 * Check if an maddr param exists in the Request URI. If so,
			 * this request must be sent to the host in the maddr param.
			 * The port in either case is that in the sipurl.
			 * 
			 * Also strip the maddr param at this stage if its present.
			 */
			if (sip_getUrlParamCountFromUrl(pUrl, &dUrlParamCount, \
				(SipError*)&(pError->stkErrCode)) == SipFail)
			{
				sip_freeSipUrl(pUrl);
				sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR					
				sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_destinationInfoError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestinationDetails( ): "
					"Failed to get url param count from Url",pError);
#endif		
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
				pError->errCode=Sdf_en_destinationInfoError;
				return Sdf_co_fail;
			}

			if (dUrlParamCount > 0)
			{
				Sdf_ty_u32bit dIndex;
				for (dIndex = 0; dIndex < dUrlParamCount; dIndex++)
				{
					SipParam *pUrlParam;
					Sdf_ty_s8bit *pName;
					
					if (sip_getUrlParamAtIndexFromUrl(pUrl, &pUrlParam,\
						dIndex, (SipError*)&(pError->stkErrCode)) == SipFail)
					{
						sip_freeSipUrl(pUrl);
						sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR							
						sdf_fn_setError(Sdf_en_minorErrors,\
							Sdf_en_destinationInfoError,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestinationDetails:"\
							"Failed to get url param from Url", pError);
#endif							
						(void)sdf_memfree(Sdf_mc_initMemId, \
								(Sdf_ty_pvoid *)ppDestDetails, pError);
						pError->errCode=Sdf_en_destinationInfoError;
						return Sdf_co_fail;
					}

					if (sip_getNameFromSipParam(pUrlParam, &pName, \
							(SipError*)&(pError->stkErrCode)) == SipFail)
					{
						sip_freeSipParam(pUrlParam);
						sip_freeSipUrl(pUrl);
						sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR							
						sdf_fn_setError(Sdf_en_minorErrors,\
							Sdf_en_destinationInfoError,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestinationDetails:"\
							"Failed to get url param from Url",pError);
#endif							
						(void)sdf_memfree(Sdf_mc_initMemId, \
								(Sdf_ty_pvoid *)ppDestDetails, pError);
						pError->errCode=Sdf_en_destinationInfoError;
						return Sdf_co_fail;
					}

					if (Sdf_mc_strcmp(pName, "transport") == 0)
					{
						Sdf_ty_s8bit *pTempTranspType = Sdf_co_null;
						Sdf_ty_s8bit dSentProto[Sdf_co_characterBufferSize];
						SipHeader dTempViaHeader;

						if (sip_getValueAtIndexFromSipParam(pUrlParam, \
							&pTempTranspType, 0, (SipError*) \
							&(pError->stkErrCode)) == SipFail)
						{
							sip_freeSipParam(pUrlParam);
							sip_freeSipUrl(pUrl);
							sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR								
							sdf_fn_setError(Sdf_en_minorErrors,\
								Sdf_en_destinationInfoError,\
								(Sdf_ty_s8bit *) \
								"sdf_ivk_uaGetDestinationDetails( ): "
								"Failed to get value from Url param", pError);
#endif
							(void)sdf_memfree(Sdf_mc_initMemId, \
									(Sdf_ty_pvoid *)ppDestDetails, pError);
							pError->errCode=Sdf_en_destinationInfoError;
							return Sdf_co_fail;
						}
						/*
						 * If a transport parameter exists, check what
						 * the transport type is and update both the
						 * dest-info as well as the topmost Via of the
						 * request being sent out.
						 */
						if (Sdf_mc_strcasecmp(pTempTranspType, "TCP") == 0)
							    Sdf_mc_strcpy(dSentProto, "SIP/2.0/TCP");
#ifdef SDF_TLS
						else if (Sdf_mc_strcasecmp(pTempTranspType, "TLS") == 0)
							Sdf_mc_strcpy(dSentProto, "SIP/2.0/TLS");
#endif
						else
							Sdf_mc_strcpy(dSentProto, "SIP/2.0/UDP");
						
						/*
						 * Update the sent protocol in the topmost via
						 * header to reflect the transport type set.
						 */
						if (sip_getHeaderAtIndex(pMessage, \
							SipHdrTypeVia, &dTempViaHeader, 0, (SipError*) \
							&(pError->stkErrCode)) == SipFail)
						{
							sip_freeSipParam(pUrlParam);
							sip_freeSipUrl(pUrl);
							sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR								
							sdf_fn_setError(Sdf_en_minorErrors,\
								Sdf_en_destinationInfoError,\
								(Sdf_ty_s8bit *) \
								"sdf_ivk_uaGetDestinationDetails( ): "
								"Failed to get topmost Via header.", pError);
#endif
							(void)sdf_memfree(Sdf_mc_initMemId, \
									(Sdf_ty_pvoid *)ppDestDetails, pError);
							pError->errCode=Sdf_en_destinationInfoError;
							return Sdf_co_fail;
						}
						pTmpVal = Sdf_mc_strdupCallHandler(dSentProto);
						if (sip_setSentProtocolInViaHdr(&dTempViaHeader, \
							pTmpVal, \
							(SipError*)&(pError->stkErrCode)) == SipFail)
						{
							sip_freeSipParam(pUrlParam);
							sip_freeSipUrl(pUrl);
							sip_freeSipAddrSpec(pAddrSpec);
							sip_freeSipHeader(&dTempViaHeader);
#ifdef SDF_ERROR								
							sdf_fn_setError(Sdf_en_minorErrors,\
								Sdf_en_destinationInfoError,\
								(Sdf_ty_s8bit *) \
								"sdf_ivk_uaGetDestinationDetails( ): "
								"Failed to set sent protocol from topmost "
								"Via header.", pError);
#endif
							(void)sdf_memfree(Sdf_mc_initMemId, \
									(Sdf_ty_pvoid *)ppDestDetails, pError);
							pError->errCode=Sdf_en_destinationInfoError;
							return Sdf_co_fail;
						}
						sip_freeSipHeader(&dTempViaHeader);
						
						/*
						 * Via header updations have been done. Change
						 * the destination info scheme now.
						 */
						if (sdf_ivk_uaGetDestInfoFromTransaction(\
								pTransaction,pOverlapTransaction,\
								&pDestInfo, pError) == Sdf_co_fail)
						{
							sip_freeSipParam(pUrlParam);
							sip_freeSipUrl(pUrl);
							sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors,\
								Sdf_en_destinationInfoError,\
								(Sdf_ty_s8bit *) \
								"sdf_ivk_uaGetDestinationDetails( ): "
								"Failed to get Destination Info from the "
								"Call Object",pError);
#endif
							(void)sdf_memfree(Sdf_mc_initMemId, \
									(Sdf_ty_pvoid *)ppDestDetails, pError);
							pError->errCode=Sdf_en_destinationInfoError;
							return Sdf_co_fail;
						}
                        if(Sdf_mc_strcasecmp(pTempTranspType, "TCP") \
								== 0)
							    pDestInfo->dScheme=Sdf_en_protoTcp;
							else
								pDestInfo->dScheme=Sdf_en_protoUdp;
 
						(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
						
					}
					else if (Sdf_mc_strcmp(pName, "maddr") == 0)
					{
						if (sip_getValueAtIndexFromSipParam(pUrlParam, \
							&pTempHost, 0, \
							(SipError*)&(pError->stkErrCode)) == SipFail)
						{
							sip_freeSipParam(pUrlParam);
							sip_freeSipUrl(pUrl);
							sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR								
							sdf_fn_setError(Sdf_en_minorErrors,\
								Sdf_en_destinationInfoError,\
								(Sdf_ty_s8bit *) \
								"sdf_ivk_uaGetDestinationDetails( ): "
								"Failed to get value from Url param", \
								pError);
#endif								
							(void)sdf_memfree(Sdf_mc_initMemId, \
									(Sdf_ty_pvoid *)ppDestDetails, pError);
							pError->errCode=Sdf_en_destinationInfoError;
							return Sdf_co_fail;
						}

						pHost = Sdf_mc_strdupCallHandler(pTempHost);
						IsMaddrParam=Sdf_co_true;
					}
					else if (Sdf_mc_strcmp(pName, "method") == 0)
					{
						/* 
						 * If a method param is present (For eg in a 
						 * Request-URI constructed from a Contact that
						 * had a method-param), it should be stripped
						 * off
						 */
						(void)sip_deleteUrlParamAtIndexInUrl(pUrl, dIndex, \
							(SipError *)&(pError->stkErrCode));

						/*
						 * Handle counters for this loop appropriately
						 * now that one element is reduced.
						 */
						dIndex--;
						dUrlParamCount--;
					}
					sip_freeSipParam(pUrlParam);
				} /* End of the for loop */
			}
			/*
			 * If any header params are present in the URL, remove that
			 * as well (acc. to RFC, method and header params should
			 * not be present in Request-URI.
			 */
                        /* Changes for CSR: 1-5249491*/
                        #ifdef PERF_OPTIMIZATION
			          if(pUrl->pHeader != pUrl->pHeader_temp)
                        #endif
                        /* Changes for CSR: 1-5249491 */

			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
				(pUrl->pHeader),pError);
			/* Free the local reference here. */
			sip_freeSipUrl(pUrl);
	   }
#ifdef SDF_LINT
		/* The check for pAddrSpec is done only to avoid lint warning
			 and the else part of the check is ignored */
		if(pAddrSpec != Sdf_co_null)
		{
			if(sip_initSipAddrSpec(&((*ppDestDetails)->u.pAddrSpec), \
					pAddrSpec->dType, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
				sip_freeSipAddrSpec(pAddrSpec);
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
                          pError);
						pError->errCode = Sdf_en_noMemoryError;
				return Sdf_co_fail;
			}
		}

		if(sip_cloneSipAddrSpec((*ppDestDetails)->u.pAddrSpec,pAddrSpec, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
          sip_freeSipAddrSpec((*ppDestDetails)->u.pAddrSpec);
		  sip_freeSipAddrSpec(pAddrSpec);
	  	  (void)sdf_memfree(Sdf_mc_initMemId, \
	  	  (Sdf_ty_pvoid *)ppDestDetails,pError);
		  return Sdf_co_fail;
		}
		if(IsMaddrParam == Sdf_co_true)
			{
					if (sip_setHostInUrl((*ppDestDetails)->u.pAddrSpec->u.pSipUrl, pHost,\
						(SipError*)&(pError->stkErrCode)) == SipFail)
					{
							sip_freeSipAddrSpec(pAddrSpec);
							sdf_memfree(Sdf_mc_callHandlerMemId, \
								(Sdf_ty_pvoid*)&pHost, pError);
#ifdef SDF_ERROR							
							sdf_fn_setError(Sdf_en_minorErrors,\
								Sdf_en_destinationInfoError,\
								(Sdf_ty_s8bit *) \
								"sdf_ivk_uaGetDestinationDetails( ): "
								"Failed to set host in Url.", \
								pError);
#endif
							sdf_memfree(Sdf_mc_initMemId, \
									(Sdf_ty_pvoid *)ppDestDetails, pError);
							pError->errCode=Sdf_en_destinationInfoError;
							return Sdf_co_fail;
					}
			}
#else
       sip_initSipAddrSpec(&((*ppDestDetails)->u.pAddrSpec), \
			pAddrSpec->dType, (SipError *)&(pError->stkErrCode));
       sip_cloneSipAddrSpec((*ppDestDetails)->u.pAddrSpec,pAddrSpec, \
		(SipError *)&(pError->stkErrCode));

  			if(IsMaddrParam == Sdf_co_true)
			{
					if (sip_setHostInUrl((*ppDestDetails)->u.pAddrSpec->u.pSipUrl, pHost,\
						(SipError*)&(pError->stkErrCode)) == SipFail)
					{
							sip_freeSipAddrSpec(pAddrSpec);
							sdf_memfree(Sdf_mc_callHandlerMemId, \
								(Sdf_ty_pvoid*)&pHost, pError);
#ifdef SDF_ERROR							
							sdf_fn_setError(Sdf_en_minorErrors,\
								Sdf_en_destinationInfoError,\
								(Sdf_ty_s8bit *) \
								"sdf_ivk_uaGetDestinationDetails( ): "
								"Failed to set host in Url.", \
								pError);
#endif
							sdf_memfree(Sdf_mc_initMemId, \
									(Sdf_ty_pvoid *)ppDestDetails, pError);
							pError->errCode=Sdf_en_destinationInfoError;
							return Sdf_co_fail;
					}
			}
		
#endif

		sip_freeSipAddrSpec(pAddrSpec);

#ifndef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/

#ifdef SDF_TLS
		/* If Request URI scheme is SIPS, then irrespective of the Target URI
		 * whether the Top most route header or Request Line, the TLS 
		 * procedures need to be used.  This can be achieved by modifying
		 * the dType of Address spec to SIPS scheme.
		 * */
		if( dSchemeType == SipAddrSipSUri)
			(*ppDestDetails)->u.pAddrSpec->dType = dSchemeType;
#endif
#endif /* End:changes done by subbaroa  */
	}
	else if (dType == SipMessageResponse)
	{
		/* 
		 * For a Response which is being sent out, updating the destination
		 * is done as follows:
		 *  	1. 	If the Via header contains a maddr param, send the
		 *  		response to the IP address it contains and to the port 
		 *  		specified in the 'sentby' portion of the topmost Via
		 * 		2. 	If maddr is absent but the received param is present,
		 * 			then send the response to the port indicated in the
		 * 			sentby portion of the topmost Via (if none given then
		 * 			default = 5060)
		 * 		3.	If neither maddr nor received params are present, the 
		 * 			destInfo is set to the IP/port of the topmost Via Header 
		 */
		SipHeader dTempHeader;
		Sdf_ty_s8bit *pSentProto		 = Sdf_co_null;
		Sdf_ty_s8bit *pSentBy			 = Sdf_co_null;
		Sdf_ty_s8bit *pSentByHostAddress = Sdf_co_null;
		Sdf_ty_s8bit *pSentByPort		 = Sdf_co_null;
		Sdf_ty_s8bit *pMaddrHostAddress  = Sdf_co_null;
		Sdf_ty_s8bit *pReceivedParamHost = Sdf_co_null;

#ifdef SDF_RPORT
		Sdf_ty_s8bit *pRport =Sdf_co_null;
#endif
		Sdf_ty_s8bit *pHostAddress		 = Sdf_co_null; /* final address */
		Sdf_ty_s8bit dTempSentby[Sdf_co_characterBufferSize] = {'\0',};
		SipParam	 *pViaParam;
		Sdf_ty_u32bit dViaParamCount=0, dIndex=0;

		(*ppDestDetails)->dIsTargetUriSet = Sdf_co_false;

		if (sip_getHeaderAtIndex(pMessage, SipHdrTypeVia, \
			&dTempHeader, 0, (SipError*)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_destinationInfoError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateDestinationInfo( ) : "
				"Error Accessing Via Header From Sip Message", pError);
#endif			
			(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
			pError->errCode=Sdf_en_destinationInfoError;
			return Sdf_co_fail;
		}

		if (sip_getSentByFromViaHdr(&dTempHeader, &pSentBy, (SipError *)\
				&(pError->stkErrCode)) == SipFail)
		{		
			pError->errCode = Sdf_en_headerManipulationError;
			sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_destinationInfoError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateDestinationInfo( ) : "
				"Error Accessing  SentBy from Via Header in Sip Message", \
				pError);
#endif			
			(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
			pError->errCode=Sdf_en_destinationInfoError;
			return Sdf_co_fail;
		}
 
/*Change for vxWork Compilation */

/*CSR 1-4788197 start*/		
        Sdf_mc_strncpy(dTempSentby, pSentBy, Sdf_co_characterBufferSize);	
/*CSR 1-4788197 end*/

        /* Strip the '[',']' before resolving the address. */
		if(Sdf_mc_strstr(dTempSentby, "[") != Sdf_co_null)
		{

			pSentByPort = Sdf_mc_strstr(dTempSentby, "]:");
			pSentByHostAddress = dTempSentby+1;
			if(pSentByPort !=Sdf_co_null)
			{
		    	pSentByPort = pSentByPort + 2; /* Get rid of "]:" */
			    pSentByHostAddress[Sdf_mc_strlen(dTempSentby)-\
					Sdf_mc_strlen(pSentByPort)-3] = '\0';
			}
			else
			{
			    pSentByHostAddress[Sdf_mc_strlen(dTempSentby)-2] = '\0';

			}
		}
		else
		{
			pSentByHostAddress = Sdf_mc_strtokr(dTempSentby, ":",&pSentByPort);	
		}

		/*
		 * We have got the host and port parts of the sentby. Now to check
		 * for the maddr and received params.
		 */
		if (sip_getViaParamCountFromViaHdr(&dTempHeader, &dViaParamCount, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateDestinationInfo( ): "
				"Failed to get via param count of topmost Via header.",pError);
#endif			
			(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails,pError);
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipHeader(&dTempHeader);
			return Sdf_co_fail;
		}
		for (dIndex=0; dIndex<dViaParamCount; dIndex++)
		{
			Sdf_ty_s8bit *pParamName  = Sdf_co_null;
			Sdf_ty_s8bit *pParamValue = Sdf_co_null;

			if (sip_getViaParamAtIndexFromViaHdr(&dTempHeader, &pViaParam, \
				(Sdf_ty_u32bit)dIndex, (SipError *)&(pError->stkErrCode)) \
				== SipFail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateDestinationInfo( ): "
					"Failed to get via param from topmost Via header.",pError);
#endif			
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipHeader(&dTempHeader);
				return Sdf_co_fail;
			}
			if (sip_getNameFromSipParam(pViaParam, &pParamName, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateDestinationInfo( ): "
					"Failed to get name from via param.", pError);
#endif			
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipHeader(&dTempHeader);
				sip_freeSipParam(pViaParam);
				return Sdf_co_fail;
			}
			if (sip_getValueAtIndexFromSipParam(pViaParam, &pParamValue, \
				(Sdf_ty_u32bit)0, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
				sip_freeSipParam(pViaParam);
				pError->errCode = pError->stkErrCode;
				if((pError->stkErrCode != Sdf_en_noExistError) && \
	 		       (pError->stkErrCode != Sdf_en_invalidIndexError))
				{
					sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError, \
						(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateDestinationInfo( ): "
						"Failed to get value from via param.", pError);
#endif			
					(void)sdf_memfree(Sdf_mc_initMemId, \
							(Sdf_ty_pvoid *)ppDestDetails, pError);
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				else
				{
					continue;
				}
			}
			if (Sdf_mc_strcmp(pParamName, "maddr") == 0)
			{
				/*
				 * If an maddr param is found, we can break out of the loop
				 * as that gets highest precedence anyways.
				 */
				pMaddrHostAddress = pParamValue;
				sip_freeSipParam(pViaParam);
				break;
			}
			else if (Sdf_mc_strcmp(pParamName, "received") == 0)
			{
				/*
				 * can't break out for received param as a maddr param can
				 * be following this.
				 */
				pReceivedParamHost = pParamValue;
			}
#ifdef SDF_RPORT
			else if (Sdf_mc_strcmp(pParamName, "rport") == 0)
			{
				pRport = pParamValue;
			}
#endif

			sip_freeSipParam(pViaParam);
		}
		
		/*
		 * Get the destination info struct to be updated.
		 */
		if (sdf_ivk_uaGetDestInfoFromTransaction(pTransaction,\
			pOverlapTransaction,&pDestInfo, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateDestinationInfo( ): "
				"Failed to get Destination Info from the Call Object", \
				pError);
#endif			
			(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
			pError->errCode=Sdf_en_callObjectAccessError;
			sip_freeSipHeader(&dTempHeader);
			return Sdf_co_fail;
		}

		/*
		 * Update the port part of it if there is no rport.
		 */

#ifdef SDF_RPORT
		if( pRport == Sdf_co_null)
		{
#endif
			if ( (pSentByPort != Sdf_co_null) && \
				 (Sdf_mc_strcmp(pSentByPort, "") != 0) )
				pDestInfo->dPort = Sdf_mc_atoi(pSentByPort);
			else
			{
#ifdef SDF_TLS	
				Sdf_ty_s8bit *pTempViaSentProto = Sdf_co_null;

			if (sip_getSentProtocolFromViaHdr(&dTempHeader, &pTempViaSentProto,\
					   (SipError*)&(pError->stkErrCode)) == SipFail)
	        {
#ifdef SDF_ERROR
		        sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
			        (Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDestinationInfo(): "
			        "Failed to get sent protocol from Via header",pError);
#endif			
		        pError->errCode=Sdf_en_headerManipulationError;
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
		        sip_freeSipHeader(&dTempHeader);
		        return Sdf_co_fail;
	        }

			if (Sdf_mc_strstr(pTempViaSentProto, "TLS") != Sdf_co_null)
				pDestInfo->dPort = Sdf_mc_defaultSipSPort;
			else
#endif /* SDF_TLS */
				    pDestInfo->dPort = Sdf_mc_defaultSipPort;
			}

#ifdef SDF_RPORT
		}
#endif



		if (sip_getSentProtocolFromViaHdr(&dTempHeader, &pSentProto, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{	
			(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
			sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_destinationInfoError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateDestinationInfo( ): "
				"Failed to get sent protocol from Via header of the request", \
				pError);
#endif			
			(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppDestDetails, \
						pError);
			pError->errCode=Sdf_en_destinationInfoError;
			return Sdf_co_fail;
		}	

		if (pDestInfo->dScheme != Sdf_en_protoNone)
		{
			if ((Sdf_mc_strstr(pSentProto, "TCP") != Sdf_co_null) || \
				(Sdf_mc_strstr(pSentProto, "tcp") != Sdf_co_null))

        		        pDestInfo->dScheme = Sdf_en_protoTcp;
#ifdef SDF_TLS
			else if ((Sdf_mc_strstr(pSentProto, "TLS") != Sdf_co_null) || \
				(Sdf_mc_strstr(pSentProto, "tls") != Sdf_co_null))
		                pDestInfo->dScheme = Sdf_en_protoTls;
                	
#endif
			else	

                		pDestInfo->dScheme = Sdf_en_protoUdp;

		}
		else
		{
			if ((Sdf_mc_strstr(pSentProto, "TCP") != Sdf_co_null) || \
				(Sdf_mc_strstr(pSentProto, "tcp") != Sdf_co_null))

		                pDestInfo->dScheme = Sdf_en_protoTcp;
#ifdef SDF_TLS
			else if ((Sdf_mc_strstr(pSentProto, "TLS") != Sdf_co_null) || \
			         (Sdf_mc_strstr(pSentProto, "tls") != Sdf_co_null))
				pDestInfo->dScheme = Sdf_en_protoTls;
#endif
			else	
			    pDestInfo->dScheme = Sdf_en_protoUdp;
		}

		/*
		 * Now for the host part of the destination info. Free the existing
		 * value and use the rules mentioned above to set the address.
		 */
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
			(pDestInfo->pIp),pError);

		/* 
		 * highest priority to maddr; then to received param and finally
		 * sentby
		 */
		if (pMaddrHostAddress != Sdf_co_null)
			pHostAddress = pMaddrHostAddress;
#ifdef SDF_RPORT
		else if(pRport != Sdf_co_null && pDestInfo->dScheme == Sdf_en_protoUdp)
		{
			pDestInfo->dPort= Sdf_mc_atoi(pRport);
			pHostAddress=pReceivedParamHost;
		}
#endif

		else if (pReceivedParamHost != Sdf_co_null)
			pHostAddress = pReceivedParamHost;
		else
			pHostAddress = pSentByHostAddress;

		/* 
		 * If via-received param is present then '[',']' will NOT be present. 
		 * If SentByHostAddrss is taken, then '[',']' would have been removed
		 * already. So no need to strip them.
		 */
		if (pMaddrHostAddress != Sdf_co_null)
        {
			/* Strip the '[',']' before resolving the address. */
			if(Sdf_mc_strstr(pHostAddress, "]:") != Sdf_co_null)
			{
				/* Strip the '[',']' before resolving the address. */
#ifdef SDF_LINT
				if(pHostAddress != Sdf_co_null)
#endif					
					Sdf_mc_strncpy(dTempAddr, (pHostAddress+1), \
							   Sdf_mc_strlen(pHostAddress)-2);
				dTempAddr[Sdf_mc_strlen(pHostAddress)-2] = '\0';

				pHostAddress = dTempAddr;

			}
   		}

		/* 
		 * Host will not be resolved here, so just store the unresolved host
         * in the pDestInfo->pIp.
		 */
		pDestInfo->pIp = Sdf_mc_strdupCallHandler(pHostAddress);



		/* Store this DestInfo in the output parameter. */
		(*ppDestDetails)->u.pDestInfo = pDestInfo;

		sip_freeSipHeader(&dTempHeader);
	}

	pError->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exiting sdf_ivk_uaGetDestinationDetails");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaFormRedirectResponse
 **
 ** DESCRIPTION	: Forms a SIP Redirect Response message for the call object.
 **				   
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
 Sdf_ty_retVal sdf_ivk_uaFormRedirectResponse 
 #ifdef ANSI_PROTO
	(Sdf_ty_u16bit 			dRespCode, 
	const Sdf_ty_s8bit 		*pMethod,
	const Sdf_ty_slist		*pNewContactList,
	Sdf_st_callObject 		*pObject, 
	Sdf_st_error 			*pError)
#else
	(dRespCode, pMethod, pNewContactList, pObject, pError)
	Sdf_ty_u16bit 			dRespCode;
	const Sdf_ty_s8bit 		*pMethod;
	const Sdf_ty_slist		*pNewContactList;
	Sdf_st_callObject 		*pObject;
	Sdf_st_error 			*pError;
#endif
{
	SipMessage 			*pSipMsg;
	Sdf_ty_bool			dStrictCheck = Sdf_co_false;
	Sdf_ty_messageType 	dMethodType = Sdf_en_unknownRequest;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering " \
		"sdf_ivk_uaFormRedirectResponse");


#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaFormRedirectResponse():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}

	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRedirectResponse( ): "
			"Invalid parameter passed to the function", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
		

	if (pNewContactList == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRedirectResponse( ): "
			"Invalid contact list passed to the function", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if (pObject->pUasTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRedirectResponse( ): "
			"UAS transaction is not initialized.", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	
#endif	/* End of param validation ifdef */

#ifdef SDF_IMS
  (void)sdf_ivk_uaAddExtraRespHeaders(pObject,Sdf_co_null,Sdf_co_null,pError);
#endif
	/* 
	 * Form 3xx Response
	 */
	if (sdf_ivk_uaFormResponse(dRespCode, pMethod, pObject, Sdf_co_null,\
		Sdf_co_false, pError) == Sdf_co_fail)	
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRedirectResponse( ): "
			"Unable to form 3xx response.", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}

	/*
	 * Delete the contact headers from the response just formed as they have 
	 * been picked up from InitData. The Contacts specified in pNewContactList
	 * must be used instead.
	 */
	pSipMsg = pObject->pUasTransaction->pSipMsg;
	/*w534*/(void)sip_deleteAllHeaderType(pSipMsg, SipHdrTypeContactAny, \
		(SipError *)(&pError->stkErrCode));

	/*w534*/(void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMethodType, pError);

	/* Check for the Contact header Mandatory/Optional. */
	dStrictCheck = sdf_fn_uaIsContactHdrMandatory(SipMessageResponse, \
				dMethodType, dRespCode);
	/* 
	 * Form the contact headers using pNewContactList
	 */
	if (sdf_fn_uaFormContactHeaders( (Sdf_ty_slist*)pNewContactList, \
			pSipMsg, Sdf_en_protoUdp, pObject->pCallInfo->dContactAddrType, \
			dStrictCheck, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRedirectResponse( ): "
			"Unable to set the contact headers in response.", pError);
#endif		
		pError->errCode=Sdf_en_formResponseError;
		return Sdf_co_fail;
	}

	pError->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "\
		"sdf_ivk_uaFormRedirectResponse");
	return Sdf_co_success;
}



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
 **		pError(OUT)		 : Error structure in case the function fails
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInsertHeaderParam 
#ifdef ANSI_PROTO
	(SipHeader 			*pSipHdr, 
	 Sdf_ty_s8bit 		*pParamName,
	 Sdf_ty_slist		*pParamValuesList,
	 Sdf_ty_u32bit		dInsertIndex,
	Sdf_st_error 		*pError)
#else
	(pSipHdr, pParamName, pParamValuesList, dInsertIndex, pError)
	SipHeader 			*pSipHdr; 
	Sdf_ty_s8bit 		*pParamName;
	Sdf_ty_slist		*pParamValuesList;
	Sdf_ty_u32bit		dInsertIndex;
	Sdf_st_error 		*pError;
#endif
{
	SipParam 		*pParam;
	Sdf_ty_u32bit	dCount=0;
	Sdf_ty_u32bit	dLoop=0;
	SipList			*pslParam = Sdf_co_null;
	Sdf_ty_s8bit	*pTempValue = Sdf_co_null;
	Sdf_st_error	dLocalErr;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering " \
		"sdf_ivk_uaInsertHeaderParam");


#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaInsertHeaderParam():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}

	if(pSipHdr == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertHeaderParam( ): "
			"Sip header passed is null", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
		

	if (pParamName == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertHeaderParam( ): "
			"Param Name supplied is invalid", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if (pParamValuesList == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertHeaderParam( ): "
			"Invalid Param Value supplied", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	
	if(SipFail == sip_initSipParam(&pParam, (SipError *)&(pError->stkErrCode)))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertHeaderParam( ): "
			"Failed to Init Sip Param structure", pError);
#endif		
		pError->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	pTempValue = Sdf_mc_strdupAccessor(pParamName);

	if(SipFail == sip_setNameInSipParam(pParam, pTempValue, \
		(SipError *)&(pError->stkErrCode)))
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertHeaderParam( ): "
			"Failed to Init Sip Param structure", pError);
#endif		
		(void)sdf_memfree(Sdf_mc_configMemId, (Sdf_ty_pvoid *)&pTempValue, \
						&dLocalErr);
		sip_freeSipParam(pParam);
		return Sdf_co_fail;
	}


	(void)sdf_listSizeOf(pParamValuesList, &dCount, pError);
	
	for( dLoop=0; dLoop < dCount; dLoop++)
	{
		Sdf_ty_s8bit	*pParamValue;

		if(Sdf_co_fail == sdf_listGetAt(pParamValuesList, dLoop,  \
						(Sdf_ty_pvoid *)&pParamValue, pError))
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertHeaderParam( ): "
				"Failed to Get Param value from Input list", pError);
#endif		
			sip_freeSipParam(pParam);
			return Sdf_co_fail;
		}
	
		pTempValue = Sdf_mc_strdupAccessor(pParamValue);

		if(SipFail == sip_insertValueAtIndexInSipParam(pParam,	pTempValue, \
			(Sdf_ty_u32bit)dLoop, (SipError *)&(pError->stkErrCode)))
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertHeaderParam( ): "
				"Failed to Insert Param Value at index", pError);
#endif		
			(void)sdf_memfree(Sdf_mc_configMemId, (Sdf_ty_pvoid *)&pTempValue, \
						&dLocalErr);
			sip_freeSipParam(pParam);
			return Sdf_co_fail;
		}
	}

	switch(pSipHdr->dType)
	{
		case  SipHdrTypeVia:
				pslParam = &(((SipViaHeader *)(pSipHdr->pHeader))->slParam);
				break;

    case  SipHdrTypeEvent:		
				pslParam = &(((SipCommonHeader *)(pSipHdr->pHeader))->slParam);
				break;

		case  SipHdrTypeTo:		
				pslParam = &(((SipToHeader *)(pSipHdr->pHeader))->slParam);
				break;

		case  SipHdrTypeFrom:	
				pslParam = &(((SipFromHeader *)(pSipHdr->pHeader))->slParam);
				break;
#if defined(MICROUA) && !defined(SIP_DATE)
		case SipHdrTypeRetryAfter:
#else
		case  SipHdrTypeRetryAfterDate:
		case  SipHdrTypeRetryAfterSec:
		case  SipHdrTypeRetryAfterAny:
#endif /* MICROUA */				
				pslParam = &(((SipRetryAfterHeader *) \
							(pSipHdr->pHeader))->slParam);
				break;
		case  SipHdrTypeContentType:
				pslParam = &(((SipContentTypeHeader *) \
							(pSipHdr->pHeader))->slParam);
				break;
		case  SipHdrTypeRoute:
				pslParam = &(((SipRouteHeader *)(pSipHdr->pHeader))->slParam);
				break;

#ifdef SIP_REPLACES				
		case  SipHdrTypeReplaces:
				pslParam = &(((SipReplacesHeader *) \
							(pSipHdr->pHeader))->slParam);
				break;
#endif /* SIP_REPLACES */
		case  SipHdrTypeRecordRoute:
				pslParam = &(((SipRecordRouteHeader *) \
							(pSipHdr->pHeader))->slParam);
				break;
#ifdef SDF_REFER
		case  SipHdrTypeReferTo:
				pslParam = &(((SipReferToHeader *) \
								(pSipHdr->pHeader))->slParam);
				break;
		case  SipHdrTypeReferredBy:
				pslParam = &(((SipReferredByHeader *) \
								(pSipHdr->pHeader))->slParam);
				break;
#endif /* SDF_REFER */				

#ifdef SIP_3GPP
		case  SipHdrTypePath:
				pslParam = &(((SipPathHeader *)(pSipHdr->pHeader))->slParam);
				break;
#endif
#if 0
#ifdef SIP_DCS
		case  SipHdrTypeDcsRemotePartyId:
				pslParam =  &(((SipDcsRemotePartyIdHeader *) \
							(pSipHdr->pHeader))->slParams);
				break;

		case  SipHdrTypeDcsRpidPrivacy:
				pslParam = &(((SipDcsRpidPrivacyHeader *) \
							(pSipHdr->pHeader))->slParams);
				break;

		case  SipHdrTypeDcsState:
				pslParam = &(((SipDcsStateHeader *) \
							(pSipHdr->pHeader))->slParams);
				break;
#endif
#endif
/* IPPSTK_CHANGES_START: Cases added for handling Contact Header */
		case  SipHdrTypeContactNormal:
		case  SipHdrTypeContactAny:
		case  SipHdrTypeContactWildCard:
            		pslParam =  &(((SipContactHeader *) \
                	(pSipHdr->pHeader))->slParam);
			break;
/* IPPSTK_CHANGES_END: Cases added for handling Contact Header */
		default:
				pslParam = Sdf_co_null;

	}

	if( pslParam != Sdf_co_null)
	{
		Sdf_ty_u32bit dParamCount = 0;

		(void)sip_listSizeOf(pslParam,&dParamCount,(SipError *)&(pError->stkErrCode));

		if( dInsertIndex > dParamCount )
			dInsertIndex = dParamCount;

		if(SipFail == sip_listInsertAt(pslParam, dInsertIndex, \
				(Sdf_ty_pvoid)pParam, (SipError *)&(pError->stkErrCode)))
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertHeaderParam( ): "
				"Failed to Insert Param Value to header", pError);
#endif		
			sip_freeSipParam(pParam);
			return Sdf_co_fail;
		}
	}
	else
		sip_freeSipParam(pParam);

	/* Don't free the SipParam  pParam at the end in success cases.  
	 * List Insert does not increment the ref count of the pParam. */

	sdf_fn_debug((Sdf_ty_s8bit *) \
			"SDF_DEBUG - Existing sdf_ivk_uaInsertHeaderParam");
	return Sdf_co_success;
}

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
#ifdef ANSI_PROTO
	(SipAddrSpec 		*pSipAddrSpec, 
	 Sdf_ty_s8bit 		*pParamName,
	 Sdf_ty_slist		*pParamValuesList,
	 Sdf_ty_u32bit		dInsertIndex,
	Sdf_st_error 		*pError)
#else
	(pSipAddrSpec, pParamName, pParamValuesList, dInsertIndex, pError)
	SipAddrSpec			*pSipAddrSpec; 
	Sdf_ty_s8bit 		*pParamName;
	Sdf_ty_slist		*pParamValuesList;
	Sdf_ty_u32bit		dInsertIndex;
	Sdf_st_error 		*pError;
#endif
{
	SipUrl *pUrl;
	SipParam 		*pParam;
	Sdf_ty_u32bit	dCount=0;
	Sdf_ty_u32bit	dLoop=0;
	Sdf_ty_u32bit	urlParamCount=0;
	Sdf_st_error	dLocalErr;
	Sdf_ty_s8bit	*pTempValue = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Entering sdf_ivk_uaInsertUrlParam");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaInsertUrlParam(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}

	if(pSipAddrSpec == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertUrlParam( ): "
			"Sip Appress Spec passed is null", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
		

	if (pParamName == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertUrlParam( ): "
			"Param Name supplied is invalid", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if (pParamValuesList == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertUrlParam( ): "
			"Invalid Param Value supplied", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	if(sip_initSipParam(&pParam, (SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertUrlParam( ): "
			"Failed to init Sip Param", pError);
#endif		
		pError->errCode = Sdf_en_memoryError;
		return Sdf_co_fail;
	}
	
	pTempValue = Sdf_mc_strdupAccessor(pParamName);

	if(sip_setNameInSipParam(pParam, pTempValue, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertUrlParam( ): "
			"Failed to set Name into Sip Param", pError);
#endif		
		(void)sdf_memfree(Sdf_mc_configMemId, (Sdf_ty_pvoid *)&pTempValue, \
						&dLocalErr);
		sip_freeSipParam(pParam);
		return Sdf_co_fail;
	}

	(void)sdf_listSizeOf(pParamValuesList, &dCount, pError);
	
	for( dLoop=0; dLoop < dCount; dLoop++)
	{
		Sdf_ty_s8bit	*pParamValue;

		if(sdf_listGetAt(pParamValuesList, dLoop,  \
						(Sdf_ty_pvoid *)&pParamValue, pError) == Sdf_co_fail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertUrlParam( ): "
				"Failed to get the Param Value from Input list", pError);
#endif		
			sip_freeSipParam(pParam);
			return Sdf_co_fail;
		}
	
		pTempValue = Sdf_mc_strdupAccessor(pParamValue);

		if( SipFail == sip_insertValueAtIndexInSipParam(pParam,	pTempValue, \
			(Sdf_ty_u32bit)dLoop,  (SipError *)&(pError->stkErrCode)))
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertUrlParam( ): "
				"Failed to insert the input param value into header", pError);
#endif		
			(void)sdf_memfree(Sdf_mc_configMemId, (Sdf_ty_pvoid *)&pTempValue, \
						&dLocalErr);
			sip_freeSipParam(pParam);
			return Sdf_co_fail;
		}
	}

	if(Sdf_co_fail == sdf_ivk_uaGetUrlFromAddrSpec(pSipAddrSpec, &pUrl, \
				pError))
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertUrlParam( ): "
			"Failed to get URL From Address Spec", pError);
#endif		
		sip_freeSipParam(pParam);
		return Sdf_co_fail;
	}

	(void)sip_getUrlParamCountFromUrl(pUrl, &urlParamCount, \
					(SipError *)&(pError->stkErrCode));

	if( dInsertIndex > urlParamCount )
		dInsertIndex = urlParamCount;

	if( SipFail == sip_insertUrlParamAtIndexInUrl(pUrl, pParam, \
			(Sdf_ty_u32bit)dInsertIndex, (SipError *)&(pError->stkErrCode)))
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertUrlParam( ): "
			"Failed to Insert Param into URL", pError);
#endif		
		sip_freeSipParam(pParam);
		sip_freeSipUrl(pUrl);
		return Sdf_co_fail;
	}
	sip_freeSipParam(pParam);
	sip_freeSipUrl(pUrl);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Existing sdf_ivk_uaInsertUrlParam");
	return Sdf_co_success;
}


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
Sdf_ty_retVal sdf_ivk_uaEncryptString 
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit 		*pInString,
	 Sdf_ty_s8bit		**ppOutString,
	Sdf_st_error 		*pError)
#else
	(pInString, ppOutString, pError)
	Sdf_ty_s8bit 		*pInString;
	Sdf_ty_s8bit		**ppOutString;
	Sdf_st_error 		*pError;
#endif
{
#ifdef SDF_LINT
#ifdef SDF_AUTHENTICATE		
		Sdf_ty_u32bit dLength=0;
#endif
#else
		Sdf_ty_u32bit dLength=0;
#endif		

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Entering sdf_ivk_uaEncryptString");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaEncryptString(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}

	if(pInString == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaEncryptString( ): "
			"Invalid Input String", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(ppOutString == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaEncryptString( ): "
			"Invalid Output String pointer", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */


	/* Allocate 32byte memory for the output string */


	*ppOutString = (Sdf_ty_s8bit *)sdf_memget(0, \
		   		((sizeof(Sdf_ty_s8bit) * 32) + 1), pError);

	if( *ppOutString == Sdf_co_null )
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaEncryptString( ): "
			"Failed to allocate memory for output string", pError);
#endif		
		pError->errCode = Sdf_en_memoryError;
		return Sdf_co_fail;
	}

#ifdef SDF_LINT
#ifdef SDF_AUTHENTICATE
	dLength = Sdf_mc_strlen(pInString);
#endif	
#else	
	dLength = Sdf_mc_strlen(pInString);
#endif	
#ifdef SDF_AUTHENTICATE
	DigestCalcHEntity((Sdf_ty_s8bit *)pInString, dLength, *ppOutString);
#endif /* SDF_AUTHENTICATE */


	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Existing sdf_ivk_uaEncryptString");
	return Sdf_co_success;
}


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

Sdf_ty_retVal sdf_ivk_uaMakePermanentCallObj 
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObj,
	 SipMessage			*pSipMsg,
	 Sdf_st_initData	*pInitData,
	 Sdf_st_error 		*pError)
#else
	(pCallObj, pSipMsg, pInitData, pError)
	 Sdf_st_callObject	*pCallObj;
	 SipMessage			*pSipMsg;
	 Sdf_st_initData	*pInitData;
	 Sdf_st_error 		*pError;
#endif
{
	Sdf_st_msgInfo			dMessageInfo;

	if(sdf_fn_uaGetMessageInfo(pSipMsg,&dMessageInfo,pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaMakePermanentCallObj(): "
			"Failed to get message info from sip message", pError);
#endif		
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(dMessageInfo.dReqRespType != SipMessageResponse)
		return Sdf_co_success;

	if(sdf_ivk_uaInitInitData(&(pCallObj->pInitData), pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaMakePermanentCallObj(): "
			"Failed to Initialise the Init Data", pError);
#endif		
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(sdf_ivk_uaSetInitDataInCallObject(pCallObj, pInitData, pError) != \
			Sdf_co_success)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaMakePermanentCallObj(): "
			"Failed to Set the Init Data", pError);
#endif		
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if(sdf_fn_uaUpdateCallConstants(pCallObj, pSipMsg,\
			dMessageInfo, Sdf_co_true, pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaMakePermanentCallObj(): "
			"Failed to Update the Call Constants in Call Object", pError);
#endif		
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if(sdf_fn_uaUpdateCallObject(pCallObj, Sdf_co_null, \
			pSipMsg, dMessageInfo, pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaMakePermanentCallObj(): "
			"Failed to Update the Call Object with Message Info", pError);
#endif		
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if( dMessageInfo.dReqRespType == SipMessageResponse )
	{
		pCallObj->pCommonInfo->pKey->dCaller = Sdf_en_caller;

		if((dMessageInfo.dResponse >= 200) && (dMessageInfo.dResponse < 300))
			pCallObj->pCallInfo->dState=Sdf_en_finalSuccessReceived;
		else if(dMessageInfo.dResponse >= 300)
			pCallObj->pCallInfo->dState=Sdf_en_finalFailureReceived;
		else
			pCallObj->pCallInfo->dState=Sdf_en_provisionalRespReceived;
	}
	return Sdf_co_success;
}


#ifdef SIP_WARNING
/*****************************************************************************
 ** FUNCTION:		sdf_ivk_uaFormWarningHeader
 **
 ** DESCRIPTION:	This function Forms the Warning Header. The formed 
 **					header is inserted into the processed header list of
 **					the overlap transaction or pTransaction passed to it.
 **
 *****************************************************************************/
Sdf_ty_retVal	sdf_ivk_uaFormWarningHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject			*pCallObj,
	 Sdf_st_transaction			*pTransaction,
	 Sdf_st_overlapTransInfo	*pOverlapTxn,
	 Sdf_ty_s8bit				*pWarningTxt,
	 Sdf_ty_u16bit				dWarningCode,
	 Sdf_st_error 				*pError)
#else
	(pCallObj, pTransaction, pOverlapTxn, pWarningTxt, dWarningCode, pError)
	Sdf_st_callObject			*pCallObj;
	Sdf_st_transaction			*pTransaction;
	Sdf_st_overlapTransInfo		*pOverlapTxn;
	Sdf_ty_s8bit				*pWarningTxt;
	 Sdf_ty_u16bit				dWarningCode;
	Sdf_st_error 				*pError;
#endif
{
	Sdf_st_headerList 		*pHeaderList;
	Sdf_st_transportInfo 	*pTransport;
	SipHeader 				*pHeader = Sdf_co_null;
	Sdf_ty_s8bit 			*pIpAddr=Sdf_co_null;
	Sdf_ty_slist			*pProcessedHeadersList = Sdf_co_null;
	Sdf_st_error			dLocalErr;
	Sdf_ty_s8bit 			dIpAddr[Sdf_co_characterBufferSize]="";

	
	sdf_fn_debug((Sdf_ty_s8bit *)\
			"SDF_DEBUG-Entering sdf_ivk_uaFormWarningHeader");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaFormWarningHeader(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}

	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormWarningHeader( ): "
			"Invalid Call Object passed", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if((pTransaction == Sdf_co_null) && (pOverlapTxn == Sdf_co_null))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormWarningHeader( ): "
			"Invalid Transaction Structure passed", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	
	if ((sdf_ivk_uaInitHeaderList(&pHeaderList, \
		SipHdrTypeWarning, Sdf_co_null, pError)) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormWarningHeader( ): "
			"Failed to Init Header List", pError);
#endif		
		return Sdf_co_fail;
	}

	if ((sip_initSipHeader(&pHeader, SipHdrTypeWarning, \
		(SipError*)&(pError->errCode)))==SipFail)
	{
		(void)sdf_ivk_uaFreeHeaderList(pHeaderList);
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormWarningHeader( ): "
			"Failed to init Warning Sip header", pError);
#endif		
		return Sdf_co_fail;
	}

	if ((sdf_ivk_uaGetTransportInfoFromInitData(pCallObj->pInitData, \
		&pTransport, pError)) == Sdf_co_fail)
	{
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHeader, \
				&dLocalErr);

		(void)sdf_ivk_uaFreeHeaderList(pHeaderList);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormWarningHeader( ): "
			"Failed to get Transport from Init Data", pError);
#endif		
		return Sdf_co_fail;
	}
	
	if(Sdf_mc_strstr(pTransport->pIp,":") !=Sdf_co_null)
		(void)sdf_fn_uaSprintf(dIpAddr, "[%s]", pTransport->pIp);
	else
		(void)sdf_fn_uaSprintf(dIpAddr, "%s", pTransport->pIp);

	Sdf_mc_strdup(pIpAddr, dIpAddr);
	(void)sdf_ivk_uaFreeTransportInfo(pTransport);

	/*
	 * Set the code in the warning header
	 */
	if ((sip_setCodeInWarningHdr(pHeader, dWarningCode, \
		(SipError*)&(pError->errCode))) == SipFail)
	{
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHeader,&dLocalErr);
		(void)sdf_ivk_uaFreeHeaderList(pHeaderList);
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormWarningHeader( ): "
			"Failed to Set the Warning code into header", pError);
#endif		
		return Sdf_co_fail;
	}
	
	/*
	 * Set the agent field in the warning header
	 */
	if ((sip_setAgentInWarningHdr(pHeader, pIpAddr, \
		(SipError*)&(pError->errCode))) == SipFail)
	{
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHeader,&dLocalErr);
		(void)sdf_ivk_uaFreeHeaderList(pHeaderList);
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormWarningHeader( ): "
			"Failed to set the Ip address into warning header", pError);
#endif		
		return Sdf_co_fail;
	}
	
	
	/* 
	 * Set the text in the warning header
	 */
	if ((sip_setTextInWarningHdr(pHeader, Sdf_mc_strdupAccessor(pWarningTxt),\
		(SipError*)&(pError->errCode))) == SipFail)
	{
		sip_freeSipHeader(pHeader);
		(void)sdf_ivk_uaFreeHeaderList(pHeaderList);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHeader,&dLocalErr);
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormWarningHeader( ): "
			"Failed to Set the warning text into warning header", pError);
#endif		
		return Sdf_co_fail;
	}
	
	/* 
	 * Set the Warning header in the Header List structure
	 */
	if ((sdf_listAppend(&(pHeaderList->slHeaders), \
		(Sdf_ty_pvoid)pHeader, pError)) == Sdf_co_fail)
	{
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHeader,&dLocalErr);
		(void)sdf_ivk_uaFreeHeaderList(pHeaderList);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormWarningHeader( ): "
			"Failed to append the formed warning header to header list",pError);
#endif		
		return Sdf_co_fail;
	}
	

	if(pOverlapTxn != Sdf_co_null )
		pProcessedHeadersList = &(pOverlapTxn->slProcessedHeaders);
	else
#ifdef SDF_LINT
		if(pTransaction != Sdf_co_null)
#endif			
			pProcessedHeadersList = &(pTransaction->slProcessedHeaders);
	
	/* 
	 * Set the Header list structure into the AdditionalHeaders
	 * list of the CallInfo structure in the call object 
	 */
	if ((sdf_listAppend(pProcessedHeadersList, \
		(Sdf_ty_pvoid)pHeaderList, pError)) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeHeaderList(pHeaderList);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormWarningHeader( ): "
			"Failed to insert the header list to processed hdr list", pError);
#endif		
		return Sdf_co_fail;
	}

	return Sdf_co_success;
}
#endif /* SIP_WARNING */

/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaMakeAddrSpecFromConstituents
 **
 ** DESCRIPTION: This function makes an addr spec structure from 
 **				 the member values passed as parameters.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeAddrSpecFromConstituents 
#ifdef ANSI_PROTO
 	(SipAddrSpec **ppAddrSpec, Sdf_ty_s8bit *pAddr, Sdf_ty_u32bit dPort, \
	Sdf_ty_s8bit *pScheme, Sdf_st_error *pError)
#else
	(ppAddrSpec, pAddr, dPort, pScheme, pError)
	 SipAddrSpec **ppAddrSpec;
	 Sdf_ty_s8bit *pAddr; 
	 Sdf_ty_u32bit dPort;
	 Sdf_ty_s8bit *pScheme;
	 Sdf_st_error *pError;
#endif
{
	Sdf_ty_s8bit	*pTmpVal = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaMakeAddrSpecFromConstituents");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaMakeAddrSpecFromConstituents(): pErr param passed "
			"is Invalid");
#endif
		return Sdf_co_fail;
	}
	if(pAddr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): " 
			"pAddr passed is Invalid", pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */
	*ppAddrSpec=Sdf_co_null;	

	/* 
	 * Check if it is a SIP/SIPS URL or a uri and fill in appropriate 
	 * structure
	 */
/* Changes for CSR: 1-5249491 */

	if ((pScheme == Sdf_co_null) || \
		((Sdf_mc_strcmp(pScheme, "sip") != 0) && \
		 (Sdf_mc_strcmp(pScheme, "sips") != 0)))
	{
		Sdf_ty_s8bit dTempAddress[Sdf_co_smallBufferSize]="";
		
		/* 
		 * Concatenate protocol:address 
		 */
		Sdf_mc_bzero(dTempAddress, Sdf_co_smallBufferSize);
		if ((pScheme == Sdf_co_null)&&(pAddr!=Sdf_co_null))
			(void)sdf_fn_uaSprintf(dTempAddress, "%s", pAddr);
		else if((pScheme != Sdf_co_null)&&(pAddr!=Sdf_co_null))
			(void)sdf_fn_uaSprintf(dTempAddress, "%s:%s", pScheme, pAddr);

		if (sip_initSipAddrSpec(ppAddrSpec,SipAddrReqUri, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
				"Failed to init an addr spec structure",pError);
#endif
			return Sdf_co_fail;
		}
		pTmpVal = Sdf_mc_strdupCallHandler(dTempAddress);

		if (sip_setUriInAddrSpec(*ppAddrSpec, pTmpVal, \
			(SipError*) &(pError-> stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
			sip_freeSipAddrSpec(*ppAddrSpec);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pError-> errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
				"Failed to set uri in addr spec",pError);
#endif
			return Sdf_co_fail;
		}
	}
else
	{
		Sdf_ty_s8bit *pTempAddr = Sdf_co_null, *pEnd = Sdf_co_null;
		Sdf_ty_u32bit dLength = 0;
	/* Strip leading and trailing LWS */
		pEnd = pAddr +  Sdf_mc_strlen(pAddr);
                          while ((isspace((unsigned)(int)*pAddr)) && (pAddr <= pEnd)) 
      		pAddr++;
		
    		dLength = Sdf_mc_strlen(pAddr);

	 if((Sdf_mc_strncmp(pAddr,"sip:",4) == 0) ||(Sdf_mc_strncmp(pAddr,"sips:",5) == 0))
	   {	
		   	Sdf_ty_s8bit *pTempScheme = Sdf_co_null, *pTemp = Sdf_co_null;

		    pTempScheme = Sdf_mc_strtokr(pAddr,":", &pTemp);
			
			if( Sdf_mc_strcmp(pScheme, pTempScheme) == 0)
			{
					pTempAddr = (SIP_S8bit *)sdf_memget(Sdf_mc_initMemId,dLength + 1, pError);
					if(pTempAddr == Sdf_co_null)
					{
#ifdef SDF_ERROR		
						sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
							(Sdf_ty_s8bit*)"sdf_ivk_uaMakeAddrSpecFromConstituents( ): "
							"Failed to allocate memory for output string", pError);
#endif		
						pError->errCode = Sdf_en_memoryError;
						return Sdf_co_fail;
					}
					Sdf_mc_strcpy(pTempAddr, pScheme);
					Sdf_mc_strcat(pTempAddr, ":");
					Sdf_mc_strcat(pTempAddr, pTemp);
			}else
			    {
					pTempAddr = (SIP_S8bit*)sdf_memget(Sdf_mc_initMemId,dLength + 2, pError);
					if(pTempAddr == Sdf_co_null)
					{
#ifdef SDF_ERROR		
						sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
							(Sdf_ty_s8bit*)"sdf_ivk_uaMakeAddrSpecFromConstituents( ): "
							"Failed to allocate memory for output string", pError);
#endif		
						pError->errCode = Sdf_en_memoryError;
						return Sdf_co_fail;
					}
					Sdf_mc_strcpy(pTempAddr, pScheme);
					Sdf_mc_strcat(pTempAddr, ":");
					Sdf_mc_strcat(pTempAddr, pTemp);
				}
	   }
	  else
		{
			pTempAddr = (SIP_S8bit *) sdf_memget(Sdf_mc_initMemId, dLength + Sdf_mc_strlen(pScheme)+2, pError);
			if(pTempAddr == Sdf_co_null)
			{
#ifdef SDF_ERROR		
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
						(Sdf_ty_s8bit*)"sdf_ivk_uaMakeAddrSpecFromConstituents( ): "
						"Failed to allocate memory for output string", pError);
#endif		
					pError->errCode = Sdf_en_memoryError;
					return Sdf_co_fail;
			}
			Sdf_mc_strcpy(pTempAddr, pScheme);
			Sdf_mc_strcat(pTempAddr, ":");
			Sdf_mc_strcat(pTempAddr, pAddr);
		}

#ifdef SDF_TLS
		if (Sdf_mc_strcmp(pScheme, "sips") == 0)
		{
			if(sip_setAddrSpecFromString(ppAddrSpec,\
						pTempAddr, (SipError *)&(pError->stkErrCode)) \
					== SipFail)
			{
				pError->errCode = pError->stkErrCode;
				sip_freeSipAddrSpec(*ppAddrSpec);
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)&pTempAddr,pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
						"Failed to init Sip Url",pError);
#endif

	return Sdf_co_fail;
			}
		}
		else
#endif /* SDF_TLS */

			if(sip_setAddrSpecFromString(ppAddrSpec,\
						pTempAddr, (SipError *)&(pError->stkErrCode)) \
					== SipFail)
			{
				pError->errCode = pError->stkErrCode;
				sip_freeSipAddrSpec(*ppAddrSpec);
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)&pTempAddr,pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
						"Failed to init Sip Url",pError);
#endif
				return Sdf_co_fail;
			}

		if (dPort != 0)
		{
			if (sip_setPortInUrl((*ppAddrSpec)->u.pSipUrl,(Sdf_ty_u16bit)dPort,(SipError*)\
						&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
				sip_freeSipAddrSpec(*ppAddrSpec);
				(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)&pTempAddr,pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
						"Failed to set port in Url",pError);
#endif
				return Sdf_co_fail;
			}
		}
		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)&pTempAddr,pError);
	}


/* Changes for CSR: 1-5249491 */

/*	if ((pScheme == Sdf_co_null) || \
		((Sdf_mc_strcmp(pScheme, "sip") != 0) && \
		 (Sdf_mc_strcmp(pScheme, "sips") != 0)))
	{
		Sdf_ty_s8bit dTempAddress[Sdf_co_smallBufferSize]="";*/
		
		/* 
		 * Concatenate protocol:address 
		 */
/*		Sdf_mc_bzero(dTempAddress, Sdf_co_smallBufferSize);
		if ((pScheme == Sdf_co_null)&&(pAddr!=Sdf_co_null))
			(void)sdf_fn_uaSprintf(dTempAddress, "%s", pAddr);
		else if((pScheme != Sdf_co_null)&&(pAddr!=Sdf_co_null))
			(void)sdf_fn_uaSprintf(dTempAddress, "%s:%s", pScheme, pAddr);

		if (sip_initSipAddrSpec(ppAddrSpec,SipAddrReqUri, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
				"Failed to init an addr spec structure",pError);
#endif
			return Sdf_co_fail;
		}
		pTmpVal = Sdf_mc_strdupCallHandler(dTempAddress);

		if (sip_setUriInAddrSpec(*ppAddrSpec, pTmpVal, \
			(SipError*) &(pError-> stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
			sip_freeSipAddrSpec(*ppAddrSpec);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pError-> errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
				"Failed to set uri in addr spec",pError);
#endif
			return Sdf_co_fail;
		}
	}
	else
	{
		SipUrl *pUrl;

#ifdef SDF_TLS
		if (Sdf_mc_strcmp(pScheme, "sips") == 0)
		{
			if (sip_initSipAddrSpec(ppAddrSpec,SipAddrSipSUri, \
			    (SipError*)&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
					"Failed to init an addr spec structure",pError);
#endif
				return Sdf_co_fail;
			}
		}
		else*/
/*#endif*/ /* SDF_TLS */
/*		if (sip_initSipAddrSpec(ppAddrSpec,SipAddrSipUri, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
				"Failed to init an addr spec structure",pError);
#endif
			return Sdf_co_fail;
		}
		if (sip_initSipUrl(&pUrl, (SipError*)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
			sip_freeSipAddrSpec(*ppAddrSpec);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
				"Failed to init Sip Url",pError);
#endif
			return Sdf_co_fail;
		}

		if (Sdf_mc_strstr(pAddr, "@") != Sdf_co_null)
		{
			Sdf_ty_s8bit *pTemp, *pLasts=Sdf_co_null, dTempArr[Sdf_co_smallBufferSize]="";

			Sdf_mc_strcpy(dTempArr, pAddr);
			pTemp = Sdf_mc_strtokr(dTempArr, "@",&pLasts);

			pTmpVal = Sdf_co_null;
			pTmpVal = Sdf_mc_strdupCallHandler(pTemp);

			if (sip_setUserInUrl(pUrl, pTmpVal, \
				(SipError*)&(pError->stkErrCode))  == SipFail)
			{
				pError->errCode = pError->stkErrCode;
				sip_freeSipUrl(pUrl);
				sip_freeSipAddrSpec(*ppAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
					"Failed to set user in Url",pError);
#endif
				return Sdf_co_fail;
			}
			pTmpVal = Sdf_co_null;
			pTmpVal = Sdf_mc_strdupCallHandler(pLasts);
			if (sip_setHostInUrl(pUrl, pTmpVal, \
				(SipError*)&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
				sip_freeSipUrl(pUrl);
				sip_freeSipAddrSpec(*ppAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
					"Failed to set host in Url",pError);
#endif
				return Sdf_co_fail;
			}
		}
		else
		{
			pTmpVal = Sdf_co_null;
			pTmpVal = Sdf_mc_strdupCallHandler(pAddr);

			if (sip_setHostInUrl(pUrl, pTmpVal,\
				(SipError*)&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
				sip_freeSipUrl(pUrl);
				sip_freeSipAddrSpec(*ppAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
					"Failed to set host in Url",pError);
#endif
				return Sdf_co_fail;
			}
		}
		if (dPort != 0)
		{
			if (sip_setPortInUrl(pUrl,(Sdf_ty_u16bit)dPort,(SipError*)\
				&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
				sip_freeSipUrl(pUrl);
				sip_freeSipAddrSpec(*ppAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
					"Failed to set port in Url",pError);
#endif
				return Sdf_co_fail;
			}
		}*/

		/* 
		 * Set the URL in the AddrSpec structure 
		 */
	/*	if (sdf_ivk_uaSetUrlInAddrSpec(*ppAddrSpec, pUrl, \
			pError) == Sdf_co_fail)
		{
			sip_freeSipUrl(pUrl);
			sip_freeSipAddrSpec(*ppAddrSpec);
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeAddrSpecFromConstituents(): "
				"Failed to set Url in AddrSpec",pError);
#endif
			return Sdf_co_fail;
		}*/
		/* Free the local reference */
		/*sip_freeSipUrl(pUrl);
	}*/
	
        pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaMakeAddrSpecFromConstituents");
	return Sdf_co_success;
}
