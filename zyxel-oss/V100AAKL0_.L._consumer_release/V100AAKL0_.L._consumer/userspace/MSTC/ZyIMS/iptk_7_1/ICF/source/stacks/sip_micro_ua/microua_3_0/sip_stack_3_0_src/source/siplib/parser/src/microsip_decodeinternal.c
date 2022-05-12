/*****************************************************************************
 ** FUNCTION: 		Functions for use at the time of decode only.
 **
 *****************************************************************************
 **
 ** FILENAME: 		microsip_decodeinternal.c
 **
 ** DESCRIPTION:	This file contains function prototypes for the 
 **					routines invoked from the parsers for specific tasks
 **					such as determinging header type from header name etc.
 **					The functions of these files are internal to the
 **					stack and not exposed to the application developer.
 **
 ** DATE        	NAME			REFERENCE		REASON
 ** ----        	----			---------		--------
 ** 24-Feb-2002  	Siddharth		-				Initial Creation
 **					Toshniwal
 **
 *****************************************************************************
 ** 			COPYRIGHT , Aricent, 2006
 *****************************************************************************/ 

#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_free.h"
#include "microsip_init.h"
#include "microsip_subparsers.h"
#include "microsip_decodeinternal.h"
#include "microsip_trace.h"
#include "microsip_accessors.h"

/*****************************************************************************
 ** FUNCTION: 		sip_getHeaderTypeFromString 
 **
 ** DESCRIPTION:	This function returns the header type from the string
 ** PARAMETERS:
 **		pStart(IN): 	The input String containing the name of the header
 **					return	value -  The header type that is returned
 **	
 **		pStart(IN):The start Index
 **		pEnd(IN):  The end Index	
 **		ReturnValue:
 **		en_headerform(OUT):The Header form(Short Form /Full Form)
 **
 **
 **		
 ****************************************************************************/

en_HeaderType sip_getHeaderTypeFromString(SIP_S8bit* pStart , \
	SIP_S8bit *pEnd,en_HeaderForm *form)
{
	    (void)pEnd;

		/*
		 * For each header supported by the stack, get the header type enum
		 * beased on the headername strign passed to this fucntion.
		 */

	    switch(pStart[0])
        {
                case 'A':
                case 'a':
                        if(sip_strcasecmp((SIP_S8bit*)"Allow",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeAllow;
								
                        }
#ifdef SIP_IMPP
                        else if(sip_strcasecmp((SIP_S8bit*)"Allow-Events",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeAllowEvents;
                        }
#endif						
#ifdef SIP_AUTHENTICATE
                        else if(sip_strcasecmp((SIP_S8bit*)"Authorization",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeAuthorization;
                        }
#endif					
#ifdef SIP_3GPP
                        else if(sip_strcasecmp((SIP_S8bit*)"Accept-Contact",\
																pStart)==0)
                        {
																*form =SipFormFull ;
                                return SipHdrTypeAcceptContact;
                        }
						else if(sip_strcasecmp((SIP_S8bit*)"a",\
																pStart)==0)
                        {
																	*form =SipFormShort ;
                                return SipHdrTypeAcceptContact;
                        }
#endif					
                        else if(sip_strcasecmp((SIP_S8bit*)"Accept",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeAccept;
                        }						
                        else
                                return SipHdrTypeUnknown;
#ifdef SIP_CONGEST
						else if(sip_strcasecmp((SIP_S8bit*)"Accept-Resource-Priority",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeAcceptRsrcPriority;
                        }
#endif		
                case 'b':
                case 'B':
#ifdef SIP_REFER				
                        if(sip_strcasecmp((SIP_S8bit*)"b",pStart)==0)
                        {
								*form =SipFormShort ;
                                return SipHdrTypeReferredBy;
                        }
#endif						
                        break;

                case 'c':
                case 'C':
                        if(sip_strcasecmp((SIP_S8bit*)"Call-id",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeCallId;
                        }
                        else if(sip_strcasecmp((SIP_S8bit*)"Contact",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeContactAny;
                        }
                        else if(sip_strcasecmp((SIP_S8bit*)"Content-Encoding",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeContentEncoding;
                        }
						else if(sip_strcasecmp((SIP_S8bit*)\
							"Content-Disposition",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeContentDisposition;
                        }

                        else if(sip_strcasecmp((SIP_S8bit*)"Content-Length",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeContentLength;
                        }

						else if((sip_strcasecmp((SIP_S8bit*)"Content-Type",\
							pStart)==0))
						
						{
								*form =SipFormFull;
                                return SipHdrTypeContentType;
                        }
                        else if((sip_strcasecmp((SIP_S8bit*)"c",pStart)==0))
						{
								*form =SipFormFull ;
                                return SipHdrTypeContentType;
						}
					    else if(sip_strcasecmp((SIP_S8bit*)"CSeq",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeCseq;
                        }
                        else
                                return SipHdrTypeUnknown;
								case 'D':
								case 'd':
#ifdef SIP_3GPP

														if((sip_strcasecmp((SIP_S8bit*)"d",pStart)==0))
														{
																*form =SipFormShort ;
                                return SipHdrTypeRequestDisposition;
														}
														else
#endif
																return SipHdrTypeUnknown;	
																
                case 'E':
                case 'e':
                        if(sip_strcasecmp((SIP_S8bit*)"Expires",pStart)==0)
                        {
								*form =SipFormFull ;
#ifdef SIP_DATE
                                return SipHdrTypeExpiresAny;
#else
								return SipHdrTypeExpires;
#endif /* SIP_DATE */
                        }
                        else if(sip_strcasecmp((SIP_S8bit*)"e",pStart)==0)
                        {
								*form =SipFormShort ;
                                return SipHdrTypeContentEncoding;
                        }
#ifdef SIP_IMPP						
                       else if(sip_strcasecmp((SIP_S8bit*)"Event",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeEvent;
								
                        }
#endif						
                        else
                                return SipHdrTypeUnknown;
                case 'F':
                case 'f':

						if(sip_strcasecmp((SIP_S8bit*)"From",pStart) == 0)
						{
							*form =SipFormFull;
						}
						else if(sip_strcasecmp((SIP_S8bit*)"f",pStart) == 0)
						{
							*form =SipFormShort;
						}
						else
						{
							return SipHdrTypeUnknown;
						}
                          return SipHdrTypeFrom;
                        
#ifdef SIP_3GPP
				case 'H':

						if(sip_strcasecmp((SIP_S8bit*)"History-Info",pStart)==0)
						{
								*form =SipFormFull ;
                               return SipHdrTypeHistoryInfo;
				        }

					    else 

                        return SipHdrTypeUnknown;
	
#endif						
				case 'I':
                case 'i':
                        if(sip_strcasecmp((SIP_S8bit*)"i",pStart)==0)
                        {
								*form =SipFormShort ;
                                return SipHdrTypeCallId;
                        }
                        else
                                return SipHdrTypeUnknown;
				case 'J':
                case 'j':
#ifdef SIP_3GPP
														if(sip_strcasecmp((SIP_S8bit*)"Join",pStart)==0)
														{
																*form =SipFormFull ;
                                return SipHdrTypeJoin;
										        }

												    else if(sip_strcasecmp((SIP_S8bit*)"j",pStart)==0)
														{
																*form =SipFormShort ;
                                return SipHdrTypeRejectContact;
										        }
												    else 
#endif
                                return SipHdrTypeUnknown;

																
                case 'K':
                case 'k':
                        if(sip_strcasecmp((SIP_S8bit*)"k",pStart)==0)
                        {
								*form =SipFormShort ;
                                return SipHdrTypeSupported;
                        }
                        else
                                return SipHdrTypeUnknown;
        case 'l':
                case 'L':
                        if(sip_strcasecmp((SIP_S8bit*)"l",pStart)==0)
                        {		
								*form =SipFormShort ;
                                return SipHdrTypeContentLength;
                        }
                        else
                                return SipHdrTypeUnknown;
                case 'M':
                case 'm':
                        if(sip_strcasecmp((SIP_S8bit*)"Mime-Version",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeMimeVersion;
                        }
                        else if(sip_strcasecmp((SIP_S8bit*)"m",pStart)==0)
                        {
								*form =SipFormShort ;
                                return SipHdrTypeContactAny;
                        }
						else if(sip_strcasecmp((SIP_S8bit*)"Max-Forwards",pStart)==0)
                        {
								*form =SipFormFull;
                                return SipHdrTypeMaxforwards;
                        }
#ifdef SIP_SESSIONTIMER
						else if(sip_strcasecmp((SIP_S8bit*)"Min-SE",pStart)==0)
                        {
								*form =SipFormFull;
                                return SipHdrTypeMinSE;
                        }
#endif /* SIP_SESSIONTIMER */
						else if(sip_strcasecmp((SIP_S8bit*)"Min-Expires",pStart)==0)
                        {
								*form =SipFormFull;
                                return SipHdrTypeMinExpires;
                        }
                        else
                                return SipHdrTypeUnknown;
#ifdef SIP_IMPP
			case 'O':
			case 'o':
					if (sip_strcasecmp((SIP_S8bit*)"o",pStart)==0)
					{		
							*form =SipFormShort ;
							return SipHdrTypeEvent;
					}
					else
							return SipHdrTypeUnknown;
#endif /* SIP_IMPP */
                case 'P':
                case 'p':
#ifdef SIP_3GPP
														if(sip_strcasecmp((SIP_S8bit*)"Path",\
																pStart)==0)
																{
																		*form =SipFormFull ;
																		return SipHdrTypePath;
																}
														else if(sip_strcasecmp((SIP_S8bit*)"P-Associated-URI",pStart)==0)
																{
																		*form =SipFormFull;
												            return SipHdrTypePAssociatedUri;
																}
														else if(sip_strcasecmp((SIP_S8bit*)"P-Called-Party-ID",pStart)==0)
												        {
																		*form =SipFormFull;
												            return SipHdrTypePCalledPartyId;
																}
				                   	else if(sip_strcasecmp((SIP_S8bit*)"P-Visited-Network-ID",pStart)==0)
								                {
																		*form =SipFormFull;
		                                return SipHdrTypePVisitedNetworkId;
						                    }
														else if(sip_strcasecmp((SIP_S8bit*)"P-Access-Network-Info",pStart)==0)
																{
																		*form =SipFormFull;
												            return SipHdrTypePanInfo;
																}
														else if(sip_strcasecmp((SIP_S8bit*)"P-Charging-Function-Addresses",pStart)==0)
												        {
																		*form =SipFormFull;
												            return SipHdrTypePcfAddr;
																}
														else if(sip_strcasecmp((SIP_S8bit*)"P-Charging-Vector",pStart)==0)
												        {
																		*form =SipFormFull;
												            return SipHdrTypePcVector;
								                }
				
#endif
#ifdef SIP_PRIVACY
														if(sip_strcasecmp((SIP_S8bit*)"Privacy",\
																pStart)==0)
														{
																*form =SipFormFull ;
                                return SipHdrTypePrivacy;
														}
														else if(sip_strcasecmp((SIP_S8bit*)"P-Preferred-Identity",\
																pStart)==0)
														{
																*form =SipFormFull ;
                                return SipHdrTypePPreferredId;
														}
														else if(sip_strcasecmp((SIP_S8bit*)"P-Asserted-Identity",\
																pStart)==0)
														{
																*form =SipFormFull ;
                                return SipHdrTypePAssertId;
														}

#endif														
#ifdef SIP_PEMEDIA
						if(sip_strcasecmp((SIP_S8bit*)"P-Early-Media",pStart)==0)
                        {
		       											*form =SipFormFull;
                                return SipHdrTypePEarlyMedia;
                        }
#endif
#ifdef SIP_AUTHENTICATE						
                        if(sip_strcasecmp((SIP_S8bit*)"Proxy-Authenticate",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeProxyAuthenticate;
                        }
                        else if(sip_strcasecmp((SIP_S8bit*)\
							"Proxy-Authorization",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeProxyauthorization;
                        }
                        else
#endif						
                                return SipHdrTypeUnknown;
                case 'R':
                case 'r':
#ifdef SIP_RPR				
                        if(sip_strcasecmp((SIP_S8bit*)"RAck",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeRAck;
                        }
                        else
#endif						
						if(sip_strcasecmp((SIP_S8bit*)"Record-Route",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeRecordRoute;
                        }
                        else if(sip_strcasecmp((SIP_S8bit*)"Require",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeRequire;
                        }
#ifdef SIP_REPLACES
                        else if (sip_strcasecmp((SIP_S8bit*)"Replaces",\
							pStart) == 0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeReplaces;
                        }
#endif /* SIP_REPLACES */

                        else if(sip_strcasecmp((SIP_S8bit*)"Route",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeRoute;
                        }
#ifdef SIP_RPR						
                        else if(sip_strcasecmp((SIP_S8bit*)"RSeq",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeRSeq;
                        }
#endif					
#ifdef SIP_3GPP	
												else if(sip_strcasecmp((SIP_S8bit*)"Reject-Contact",\
															pStart)==0)
                        {
																*form =SipFormFull ;
                                return SipHdrTypeRejectContact;
                        }
					
                        else if(sip_strcasecmp((SIP_S8bit*)"Request-Disposition",\
															pStart)==0)
                        {
																*form =SipFormFull ;
                                return SipHdrTypeRequestDisposition;
                        }
#endif						
												
                        else if(sip_strcasecmp((SIP_S8bit*)"Retry-After",\
															pStart)==0)
                        {
																*form =SipFormFull ;
#ifdef SIP_DATE
                                return SipHdrTypeRetryAfterAny;
#else
								return SipHdrTypeRetryAfter;
#endif /* SIP_DATE */
                        }
#ifdef SIP_REFER						
                        else if((sip_strcasecmp((SIP_S8bit*)"Refer-To",pStart))\
							== 0)
                                        
                        {
								*form =SipFormFull ;
                                return SipHdrTypeReferTo;
                        }
						else if((sip_strcasecmp((SIP_S8bit*)"r",pStart)) == 0)
                                        
                        {
								*form =SipFormShort;
                                return SipHdrTypeReferTo;
                        }
#endif						
#ifdef SIP_REFER						
                        else if(sip_strcasecmp((SIP_S8bit*)"Referred-By",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeReferredBy;
                        }
#endif						
#ifdef SIP_CONGEST
						else if(sip_strcasecmp((SIP_S8bit*)"Resource-Priority",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeRsrcPriority;
                        }
#endif
                        else
                                return SipHdrTypeUnknown;
                case 'S':
                case 's':
                        if(sip_strcasecmp((SIP_S8bit*)"Supported",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeSupported;
                        }
#ifdef SIP_SESSIONTIMER
						else if(sip_strcasecmp((SIP_S8bit*)"Session-Expires",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeSessionExpires;
                        }
#endif
#ifdef SIP_IMPP
						else if(sip_strcasecmp((SIP_S8bit*)"Subscription-State",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeSubscriptionState;
                        }
#endif
#ifdef SIP_3GPP
						else if(sip_strcasecmp((SIP_S8bit*)"Service-Route",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeServiceRoute;
                        }
						else if(sip_strcasecmp((SIP_S8bit*)"SIP-If-Match",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeIfMatch;
                        }
						else if(sip_strcasecmp((SIP_S8bit*)"SIP-ETag",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeETag;
                        }
#endif
#ifdef SIP_SECURITY
                        else if(sip_strcasecmp((SIP_S8bit*)"Security-Client",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeSecurityClient;
                        }
                        else if(sip_strcasecmp((SIP_S8bit*)"Security-Verify",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeSecurityVerify;
                        }
                        else if(sip_strcasecmp((SIP_S8bit*)"Security-Server",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeSecurityServer;
                        }                        
#endif                        
                        else
                                return SipHdrTypeUnknown;
                case 'T':
                case 't':
#ifdef SIP_TIMESTAMP			
                        if(sip_strcasecmp((SIP_S8bit*)"Timestamp",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeTimestamp;
                        }
                        else 
#endif						
						if((sip_strcasecmp((SIP_S8bit*)"To",pStart)==0))
                        {
								*form =SipFormFull ;
                                return SipHdrTypeTo;
                        }
                        else if((sip_strcasecmp((SIP_S8bit*)"t",pStart)==0))
						{
                                return SipHdrTypeTo;
						}
						else
							return SipHdrTypeUnknown;
                case 'U':
                case 'u':
                        if(sip_strcasecmp((SIP_S8bit*)"Unsupported",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeUnsupported;
                        }
#ifdef SIP_IMPP
						else if (sip_strcasecmp((SIP_S8bit*)"u", pStart)==0)
						{
                                return SipHdrTypeAllowEvents;
						}
#endif /* SIP_IMPP */
                        else
                                return SipHdrTypeUnknown;
                case 'V':
                case 'v':

						if(sip_strcasecmp((SIP_S8bit*)"Via",pStart) == 0)
						{
							*form =SipFormFull;
						}
						else if(sip_strcasecmp((SIP_S8bit*)"v",pStart)== 0)
						{
							*form =SipFormShort;
						}
						else
						{
							return SipHdrTypeUnknown;
						}
                        return SipHdrTypeVia;
	
                case 'W':
                case 'w':
#ifdef SIP_WARNING				
                        if(sip_strcasecmp((SIP_S8bit*)"Warning",pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeWarning;
                        }
						else
#endif						
#ifdef SIP_AUTHENTICATE						
                         if(sip_strcasecmp((SIP_S8bit*)"WWW-Authenticate",\
							pStart)==0)
                        {
								*form =SipFormFull ;
                                return SipHdrTypeWwwAuthenticate;
                        }
                        else
#endif						
                                return SipHdrTypeUnknown;
#ifdef SIP_SESSIONTIMER
				case 'x':
						if(sip_strcasecmp((SIP_S8bit*)"x",pStart)==0)
                        {
								*form =SipFormShort;
                                return SipHdrTypeSessionExpires;
                        }
						else 
								return SipHdrTypeUnknown;
#endif /* SIP_SESSIONTIMER */

                default:
                        return SipHdrTypeUnknown;
        }

	
		return SipHdrTypeUnknown;	
}



/*****************************************************************************
 ** FUNCTION: 		sip_checkMandatory 
 **
 ** DESCRIPTION:	This function checks whether mandatory 
 ** 				headers are present in  a sipmessage
 ** PARAMETERS:
 **		pSipMessage(IN): The sipMessage to be checked
 **		Return Value:
 **				SipBool
 **
 ****************************************************************************/
 
SipBool sip_checkMandatory
#ifdef ANSI_PROTO
	(SipMessage *pSipMessage)
#else
	(s)
	SipMessage *pSipMessage;
#endif
{
	SipError	error = E_NO_ERROR;
	SIP_U32bit	count;

	/*
	 * If message is a request then check for Prack first
	 */
#ifdef SIP_RPR	
	if (pSipMessage->dType == SipMessageRequest)
    {
		if ((sip_strcmp(pSipMessage->u.pRequest->pRequestLine->pStr1,\
			(SIP_S8bit *)"PRACK")\
             == 0) && (pSipMessage->u.pRequest->pRequestHdr->pRackHdr \
			 == SIP_NULL))
		{		   
			sip_error(SIP_Minor,"Prack Method With out RAck Header\n");
			return SipFail;
		}		
	 }
#endif	 

	/*
	 * Check for the foll. mandatory headers: From, To, Call-ID, CSeq, Via.
	 * We dont check for Max-Forwards even tho' it is mandated by RFC3261
	 * in order to parse some of the torture test messages.
	 */

	/* First get the size of Via header list from SipMessage. */
	if (SipFail == sip_listSizeOf(&(pSipMessage->pGeneralHdr->slViaHdr), \
		&count, &error))
	{
		return SipFail;
	}

	if ((pSipMessage->pGeneralHdr->pCallidHdr == SIP_NULL)\
	    || (pSipMessage->pGeneralHdr->pCseqHdr == SIP_NULL)\
	    || (pSipMessage->pGeneralHdr->pFromHeader == SIP_NULL)\
	    || (pSipMessage->pGeneralHdr->pToHdr == SIP_NULL)\
		|| (count == 0))
	{
		sip_error(SIP_Minor,"Message does not contain Mandatory Headers\n");
		return SipFail;
	}

	return SipSuccess;
}


/*****************************************************************************
 ** FUNCTION: 		sip_checkEachhdrType
 **
 ** DESCRIPTION:	This function checks whether a particular header is allowed
 **					in a SipMessage
 ** PARAMETERS:
 **		pSipMessage(IN): The sipMessage to be checked
 **		dType     (OUT): The sip header type
 **		pError (IN/OUT): The SipError
 **
 **		Return Values
 **				SipBool
 **
 **		
 ****************************************************************************/

SipBool sip_checkEachHdrType(SipMessage *pSipMessage,en_HeaderType dType,\
	SipError *pError)
{
	en_SipMessageType dMsgType;

	switch(dType)
	{
		/*
		 * First general headers are checked
		 */
		case SipHdrTypeCallId:
		case SipHdrTypeContactAny:
		case SipHdrTypeContactWildCard:
		case SipHdrTypeContactNormal:
#ifdef SIP_DATE
		case SipHdrTypeExpiresAny:
		case SipHdrTypeExpiresSec:
		case SipHdrTypeExpiresDate:
#else
		case SipHdrTypeExpires:
#endif /* SIP_DATE */
		case SipHdrTypeFrom:
		case SipHdrTypeRecordRoute:
#ifdef SIP_3GPP
		case SipHdrTypePath:
		case SipHdrTypePcfAddr: 
		case SipHdrTypePanInfo:   
		case SipHdrTypePcVector:  
		case SipHdrTypeHistoryInfo:
#endif
#ifdef SIP_PRIVACY	
		case SipHdrTypePrivacy:
		case SipHdrTypePPreferredId:
		case SipHdrTypePAssertId:
#endif	

#ifdef SIP_TIMESTAMP			
		case SipHdrTypeTimestamp:
#endif		
		case SipHdrTypeTo:
		case SipHdrTypeVia:
		case SipHdrTypeContentLength:
		case SipHdrTypeContentType:
		case SipHdrTypeMimeVersion:
		case SipHdrTypeContentEncoding:
#ifdef SIP_PEMEDIA
			case SipHdrTypePEarlyMedia:
#endif
		case SipHdrTypeUnknown:
		case SipHdrTypeRequire:
		case SipHdrTypeSupported:
#ifdef SIP_DATE
		case SipHdrTypeRetryAfterAny:
		case SipHdrTypeRetryAfterSec:
		case SipHdrTypeRetryAfterDate:
#else
		case SipHdrTypeRetryAfter:
#endif /* SIP_DATE */
		case SipHdrTypeAllow:
#ifdef SIP_IMPP		
		case SipHdrTypeAllowEvents:
#endif		
		case SipHdrTypeContentDisposition:
		case SipHdrTypeCseq:
#ifdef SIP_SESSIONTIMER
		case SipHdrTypeSessionExpires:
		case SipHdrTypeMinSE:
#endif
		case SipHdrTypeAccept:
			return SipSuccess;
		default:;

	}
	if (SipFail == sip_getMessageType(pSipMessage,&dMsgType,pError))
		return SipFail;
	if (dMsgType == SipMessageRequest)
	{
		switch(dType)	
		{
			/*
			 * Request headers are checked
			 */
#ifdef SIP_AUTHENTICATE			
			case SipHdrTypeProxyauthorization:
			case SipHdrTypeAuthorization:
#endif			
			case SipHdrTypeRoute:
#ifdef SIP_RPR			
			case SipHdrTypeRAck:
#endif 			
			case SipHdrTypeMaxforwards:
#ifdef SIP_REFER			
			case SipHdrTypeReferTo:
			case SipHdrTypeReferredBy:
#endif			
#ifdef SIP_IMPP			
			case SipHdrTypeEvent:
			case SipHdrTypeSubscriptionState:
#endif			
#ifdef SIP_REPLACES			
			case SipHdrTypeReplaces:
#endif /* SIP_REPLACES */
#ifdef SIP_3GPP
			case SipHdrTypePCalledPartyId: 
			case SipHdrTypePVisitedNetworkId: 
			case SipHdrTypeAcceptContact:
			case SipHdrTypeRejectContact:
			case SipHdrTypeRequestDisposition:
			case SipHdrTypeJoin:
			case SipHdrTypeIfMatch:

#endif		
#ifdef SIP_SECURITY
            case SipHdrTypeSecurityClient:
            case SipHdrTypeSecurityVerify:
#endif               
#ifdef SIP_CONGEST
			case SipHdrTypeRsrcPriority:
#endif
				return SipSuccess;
			default:;
		}
	
	}else if(dMsgType == SipMessageResponse)
	{
	
		switch(dType)	
		{
		/*
		 * Response headers are checked
		 */
#ifdef SIP_AUTHENTICATE		
			case SipHdrTypeProxyAuthenticate:
			case SipHdrTypeWwwAuthenticate:
#endif			
			case SipHdrTypeUnsupported:
#ifdef SIP_WARNING			
			case SipHdrTypeWarning:
#endif			
#ifdef SIP_RPR			
			case SipHdrTypeRSeq:
#endif			
#ifdef SIP_3GPP
			case SipHdrTypePAssociatedUri: 
		    case SipHdrTypeServiceRoute: 
			case SipHdrTypeETag:
#endif			
			case SipHdrTypeMinExpires:
#ifdef SIP_SECURITY
            case SipHdrTypeSecurityServer:
#endif                
#ifdef SIP_CONGEST
			case SipHdrTypeAcceptRsrcPriority:
#endif
		 return SipSuccess;
			default:;
		
		}
	}
	/*
	 * No need to print this error because 
	 * we will inore this class of headers
	 */
	/*sip_error(SIP_Minor,\
		"\n\nThere is  a wrong Class of header in this message\n\n");*/

	/*
	 * Coming here means a header is not matched in any class and there is an
	 * error
	 */
	*pError = E_INV_HEADER;
	return SipFail;
}


