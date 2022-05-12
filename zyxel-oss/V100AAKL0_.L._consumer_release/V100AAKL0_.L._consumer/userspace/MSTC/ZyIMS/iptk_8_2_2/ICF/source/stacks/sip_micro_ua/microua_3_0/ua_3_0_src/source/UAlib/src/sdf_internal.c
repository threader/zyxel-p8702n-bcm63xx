/*****************************************************************************
 ** FUNCTION:	
 **			Has all the internal functions of the UA Toolkit.	
 *****************************************************************************
 **
 ** FILENAME: 		sdf_internal.c
 **
 ** DESCRIPTION:	Has all the internal functions of the UA Toolkit
 **  	
 ** DATE      	 NAME          REFERENCE      		  REASON
 ** ----      	 ----          ---------      		  ------
 ** 27-Oct-00	 Prashant							 Creation
 **			 	 Subhash
 ** 07-Nov-00  	 Prashant							 Updated
 ** 25-Dec-00  	 Ashok         IsValidCseq     		 Updated
 ** 28-Mar-01  	 Ashok         IsValidUriSchemePort  Updated
 ** 04-May-01	 Siddharth	   						 Changed storage of remote 
 **												 	 retrans elements to be in 
 **												 	 a hash table instead of 
 **													 list.
 ** 19-Sep-01  	 Subhash							 Added support for 
 **													 SUBSCRIBE and NOTIFY
 ** 22/04/03     Anil Kumar        ---               Changes for TLS feature.
 ** 12/05/03     Subbarao		UATK 2.1 Design      RFC 3428 feature added.
 **
 ** 18/06/2008   Jagmohan Chauhan         CSR SI_Vol18_2_1_9 Merge
 **                                       Issue with IPTK - SCS Interop 
 **                                       IPTK terminates the call on getting 200 OK 
 ** 12-Dec-2008  Anurag Khare             SPR 18850 Merge 
 ** 30-JAN-2009  Kamal Ashraf             SPR 19294 Merge
 ** 27-May-2009  Kamal Ashraf   SPR 19267            CSR_1_7108373 Merged
 ** 08-Jun-2009  Anuradha Gupta  SPR 19737    Klocworks fix
 ** 17-Aug-2009  Anuradha Gupta  SPR 19927    Klocworks warning removal
 ** 31-Aug-2009  Rajiv Kumar     SPR 19999    Merged CSR 1-7429815
 ** 31-Aug-2009  Rajiv Kumar     SPR 19999    Merged CSR 1-7562616  
 ** 02-Sep-2009  Rajiv Kumar     SPR 19999    Merged CSR 1-7658351
 ** 04-Sep-2009  Preksha         SPR 19999    Merged SPR 19805/CSR 1-7574892
 ** 08-Sep-2009  Rajiv Kumar     SPR 19999    Merged CSR 1-7586127
 ** 05-Oct-2009 Abhishek Dhammawat SPR 20063  Merged IPTK CSR 1-7771967
 ** 26-Oct-2009  Anuradha Gupta  SPR 20156    CSR 1-7927993 fix
 ** 29-May-2010  Alok Tiwari     SPR 20412    Merged EGT CSR 1-8223807
 **                                           UATK CSR 1-8290042
 **
 *****************************************************************************
 ** 			COPYRIGHT , Aricent, 2010
 *****************************************************************************/


/* ----------------------------------------------- 
 				System header files 					
   ----------------------------------------------- */
#include <ctype.h>
#include <stdlib.h>

/* -----------------------------------------------
  		SIP Stack and UA Toolkit header files		
   ----------------------------------------------- */
#include "ALheader.h"
#include "sdf_struct.h"

#ifndef SDF_LINT
#include "sdf_common.h"
#include "sdf_sdpstruct.h"
#include "sdf_portlayer.h"
#include "sdf_list.h"
#include "sdf_sdpfree.h"
#endif

#include "sdf_internal.h"
#include "sdf.h"
#include "sdf_reqresp.h"
#include "sdf_debug.h"
#include "sdf_sdp.h"
#include "sdf_tables.h"
#include "sdf_free.h"
#include "sdf_init.h"
#include "sdf_hash.h"
#include "sdf_accessor.h"
#include "sdf_network.h"
#include "sdf_callbacks.h"
#include "sdf_statistics.h"
#include "sdf_trace.h"
#include "sdf_authorization.h"
#ifdef SDF_SESSION_TIMER
#include "sdf_sessTimer.h"
#endif   
#include "sdf_sdpinternal.h"
#include "sdf_sdpinit.h"
#include "sdf_sdpaccessor.h"
#include "sdf_txn.h"

#ifdef SDF_SUBSCRIPTION_LAYER
#include <sdf_subslayerstruct.h>
#include <sdf_subslayercbk.h>
#include <sdf_subslayerutils.h>
#endif


#ifdef SDF_DEP
#include "sdf_deputil.h"
#endif
/*
 * Table mapping the status codes to reason phrases.
 * Any updations in this table size should get reflected in
 * Sdf_co_scTableSize value
 */
Sdf_st_statusCodeMapping sdf_gl_dCodeReasonMapping[] = {
	{100,"Trying"},
	{180,"Ringing"},
	{181,"Call Is Being Forwarded"},
	{182,"Queued"},
	{183,"Session Progress"},
	{200,"OK"},
	{202,"Accepted"},
	{300,"Multiple Choices"},
	{301,"Moved Permanently"},
	{302,"Moved Temporarily"},
	{305,"Use Proxy"},
	{380,"Alternative Service"},
	{400,"Bad Request"},
	{401,"Unauthorized"},
	{402,"Payment Required"},
	{403,"Forbidden"},
	{404,"Not Found"},
	{405,"Method Not Allowed"},
	{406,"Not Acceptable"},
	{407,"Proxy Authentication Required"},
	{408,"Request Timeout"},
	{409,"Conflict"},
	{410,"Gone"},
	{411,"Length Required"},
	{413,"Request Entity Too Long"},
	{414,"Request-URI Too Long"},
	{415,"Unsupported Media Type"},
	{416,"Unsupported URI Scheme"},
    {417,"Unknown Resource-Priority"},
	{420,"Bad Extension"},
	{421,"Extension Required"},
	{422,"Session Interval Too Small"},
	{423,"Interval Too Brief"},
	{429,"Provide Referrer Identity"},
    /*UATK CSR 1-7806133 fix*/
    {433,"Anonymity Disallowed"},
	{480,"Temporarily Unavailable"},
	{481,"Call Leg/Transaction Does Not Exist"},
	{482,"Loop Detected"},
	{483,"Too Many Hops"},
	{484,"Address Incomplete"},
	{485,"Ambiguous"},
	{486,"Busy Here"},
	{487,"Request Terminated"},
	{488,"Not Acceptable Here"},
	{489,"Bad Event"},
	{491,"Request Pending"},
	{493,"Undecipherable"},
	{500,"Server Internal Error"},
	{501,"Not Implemented"},
	{502,"Bad Gateway"},
	{503,"Service Unavailable"},
	{504,"Server Time-out"},
	{505,"Version Not Supported"},
	{513,"Message Too Large"},
	{580,"Precondition Failure"},
	{600,"Busy Everywhere"},
	{603,"Decline"},
	{604,"Does Not Exist Anywhere"},
	{606,"Not Acceptable"}
};


#ifdef SDF_TLS

/*
 * Table mapping the TLS cipher suites to SSL cipher suites.
 */
Sdf_st_tlsToSslCipherMap sdf_gl_dCipherMapping[Sdf_co_maxNumOfCiphers] = {
	{{"TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA"},{"EDH-RSA-DES-CBC3-SHA"}},
	{{"TLS_DHE_DSS_WITH_3DES_EDE_CBC_SHA"},{"EDH-DSS-DES-CBC3-SHA"}},
	{{"TLS_RSA_WITH_3DES_EDE_CBC_SHA"},{"DES-CBC3-SHA"}},
	{{"TLS_DHE_DSS_EXPORT1024_WITH_RC4_56_SHA"},{"EXP1024-DHE-DSS-RC4-SHA"}},
	{{"TLS_DHE_DSS_WITH_RC4_128_SHA"},{"DHE-DSS-RC4-SHA"}},
	{{"TLS_RSA_WITH_IDEA_CBC_SHA"},{"IDEA-CBC-SHA"}},
	{{"TLS_RSA_WITH_RC4_128_SHA"},{"RC4-SHA"}},
	{{"TLS_RSA_EXPORT1024_WITH_RC4_56_SHA"},{"EXP1024-RC4-SHA"}},
	{{"TLS_RSA_EXPORT_WITH_RC4_40_MD5"},{"EXP1024-RC4-MD5"}},
	{{"TLS_RSA_WITH_RC4_128_MD5"},{"RC4-MD5"}},
	{{"TLS_DHE_DSS_EXPORT1024_WITH_DES_CBC_SHA"},{"EXP1024-DHE-DSS-DES-CBC-SHA"}},
	{{"TLS_RSA_EXPORT1024_WITH_DES_CBC_SHA"},{"EXP1024-DES-CBC-SHA"}},
	{{"TLS_DHE_RSA_EXPORT_WITH_DES40_CBC_SHA"},{"EXP-EDH-RSA-DES-CBC-SHA"}},
	{{"TLS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA"},{"EXP-EDH-DSS-DES-CBC-SHA"}},
	{{"TLS_DHE_RSA_WITH_DES_CBC_SHA"},{"EDH-RSA-DES-CBC-SHA"}},
	{{"TLS_DHE_DSS_EXPORT_WITH_DES40_CBC_SHA"},{"EDH-DSS-DES-CBC-SHA"}},
	{{"TLS_RSA_EXPORT_WITH_DES40_CBC_SHA"},{"EXP-DES-CBC-SHA"}},
	{{"TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5"},{"EXP-RC2-CBC-MD5"}},
	{{"TLS_RSA_EXPORT_WITH_RC4_40_MD5"},{"EXP-RC4-MD5"}},
	{{"TLS_RSA_WITH_DES_CBC_SHA"},{"DES-CBC-SHA"}},
	{{"TLS_RSA_EXPORT_WITH_RC2_CBC_40_MD5"},{"EXP1024-RC2-CBC-MD5"}},
	{{"TLS1_TXT_RSA_EXPORT1024_WITH_RC2_CBC_56"},{"EXP1024-RC2-CBC-MD5"}},
	{{"TLS1_TXT_RSA_EXPORT1024_WITH_RC4_56_MD5"},{"EXP1024-RC4-MD5"}},
	{{"TLS1_TXT_RSA_WITH_AES_128_SHA"},{"AES128-SHA"}},
	{{"TLS_RSA_WITH_AES_128_CBC_SHA"},{"AES128-SHA"}},
	{{"TLS1_TXT_DHE_DSS_WITH_AES_128_SHA"},{"DHE-DSS-AES128-SHA"}},
	{{"TLS_DHE_DSS_WITH_AES_128_CBC_SHA"},{"DHE-DSS-AES128-SHA"}},
	{{"TLS1_TXT_DHE_RSA_WITH_AES_128_SHA"},{"DHE-RSA-AES128-SHA"}},
	{{"TLS_DHE_RSA_WITH_AES_128_CBC_SHA"},{"DHE-RSA-AES128-SHA"}},
	{{"TLS1_TXT_RSA_WITH_AES_256_SHA"},{"AES256-SHA"}},
	{{"TLS_RSA_WITH_AES_256_CBC_SHA"},{"AES256-SHA"}},
	{{"TLS1_TXT_DHE_DSS_WITH_AES_256_SHA"},{"DHE-DSS-AES256-SHA"}},
	{{"TLS_DHE_DSS_WITH_AES_256_CBC_SHA"},{"DHE-DSS-AES256-SHA"}},
	{{"TLS1_TXT_DHE_RSA_WITH_AES_256_SHA"},{"DHE-RSA-AES256-SHA"}},
	{{"TLS_DHE_RSA_WITH_AES_256_CBC_SHA"},{"DHE-RSA-AES256-SHA"}}
};

#endif /* SDF_TLS */


/* 
 * CSM Table for state transitions based on incoming SIP messages:
 * Rows 	=> Current state
 * Columns 	=> Message Received
 * Element 	=> Next state 
 */
Sdf_st_nextCallState dCsmNxtStateOnMsgRecd[Sdf_mc_noOfMessageTypes][Sdf_mc_noOfStates];

/* 
 * CSM Table for state transitions based on outgoing SIP messages:
 * Rows 	=> Current state
 * Columns	=> Message Sent 
 * Element 	=> Next state 
 */
Sdf_ty_state dCsmNxtStateOnMsgSent[Sdf_mc_noOfMessageTypes][Sdf_mc_noOfStates];

/* klocworks : NULL checks for pointers before access, checking memeory
 * allocatoin failures and returning failure
 */ 

/***********************************************************************
** DATA: dGlbSdfCallObjectIdGeneratorMutex 
**		 dGlbSdfNextCallObjectId
**
** DESCRIPTION: dGlbSdfCallObjectIdGeneratorMutex is the Mutex used to ensure 
**				that Call Object Id generation is done in a critical section. 
**				The next call object Id available for allocation is stored in 
**				dGlbSdfNextCallObjectId
**
************************************************************************/
#ifdef SDF_THREAD_SAFE
Sdf_ty_mutexIdT dGlbSdfCallObjectIdGeneratorMutex;
#endif
Sdf_ty_u32bit dGlbSdfNextCallObjectId;

#ifdef SIP_RPR
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaIsMessageRPR
 **
 ** DESCRIPTION: This function finds whether the response message
 **				 is an Reliable Provisional Response. 
 **
 ** RETURN VALUE: If 0 - Message is a RPR
 **				  If 1 - Message is not a RPR
 **
 ******************************************************************************/
Sdf_ty_u8bit sdf_fn_uaIsMessageRPR
#ifdef ANSI_PROTO
	(SipMessage *pMessage,
	Sdf_st_error *pError)
#else
	(pMessage, pError)
	SipMessage *pMessage;
	Sdf_st_error *pError;
#endif
{
	en_SipMessageType type;
	Sdf_ty_u32bit dCount = 0;
	SipStatusLine *pStatusLine=Sdf_co_null;
/* CodeNum changed to U32bit in MicroSip */
	Sdf_ty_u32bit dStatusCode=0;
	Sdf_ty_bool		isRseqPresent = Sdf_co_false;
	Sdf_ty_bool		isReqHdrPresent = Sdf_co_false;
	Sdf_ty_u32bit size1,ii;
	Sdf_ty_s8bit *pTemp;
	Sdf_ty_slist slRequiredList;
    SipHeader dTempHeader;
#ifdef SDF_LINT
	Sdf_ty_u32bit dRseq=0;
#else
	Sdf_ty_u32bit dRseq;
#endif


	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_fn_uaIsMessageRPR");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pError == Sdf_co_null)
		return 1;
	if (pMessage == Sdf_co_null)
		return 1;
#endif

	/* 
	 * If message is not of type response, return 1
	 */
	if (sip_getMessageType(pMessage, &type, (SipError*) &(pError->stkErrCode)) \
		== SipFail)
	{
#ifdef SDF_ERROR 
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaIsMessageRPR(): "
			"Failed to get message type",pError);
#endif
		return 1;
	}
	if (type != SipMessageResponse)
	{
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
			"sdf_fn_uaIsMessageRPR");
		return 1;
	}

	if (sip_getStatusLineFromSipRespMsg(pMessage, &pStatusLine, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
			"sdf_fn_uaIsMessageRPR");
		return 1;
	}

	if (sip_getStatusCodeNumFromStatusLine(pStatusLine, &dStatusCode, \
		(SipError*)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipStatusLine(pStatusLine);
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
			"sdf_fn_uaIsMessageRPR");
		return 1;
	}
	sip_freeSipStatusLine(pStatusLine);

	if ((dStatusCode > 199) || (dStatusCode <= 100))
	{
		/*
		 * Message has a status code greater than 199. It is a final
		 * response. Return 1 here.
		 */
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
			"sdf_fn_uaIsMessageRPR");
		return 1;
	}

	/*
	 * If message is provisional response, but does not contain a RSeq header,
	 * return 1.
	 */
	if (sip_getHeaderCount(pMessage, SipHdrTypeRSeq, &dCount, \
		(SipError*) &(pError->stkErrCode)) == SipFail)
	{
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
			"sdf_fn_uaIsMessageRPR");
		return 1;
	}
    if( dCount > 0 )
    {
        if (sip_getHeader(pMessage, SipHdrTypeRSeq,\
                    &dTempHeader, (SipError*)&(pError->stkErrCode)) \
                == SipFail)
        {
            pError->errCode = pError->stkErrCode;

#ifdef SDF_ERROR				
            sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
                    (Sdf_ty_s8bit *)"sdf_fn_uaIsMessageRPR( ): "
                    "Failed to get Rseq header",pError);
#endif				
            return Sdf_co_fail;
        }

        if (sip_rpr_getRespNumFromRSeqHdr(&dTempHeader,\
                    &dRseq,(SipError *) &(pError->stkErrCode)) \
                == SipFail)
        {
            pError->errCode = pError->stkErrCode;

#ifdef SDF_ERROR				
            sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
                    (Sdf_ty_s8bit *)"sdf_fn_uaIsMessageRPR( ): "
                    "Failed to get rseq num from Rseq header",pError);
#endif				
            sip_freeSipHeader(&dTempHeader);
            return Sdf_co_fail;
        }
        sip_freeSipHeader(&dTempHeader);
    }
	
	if((dCount > 0) && (dRseq !=0 ))
		isRseqPresent = Sdf_co_true;

	if (sdf_ivk_uaReturnRequiredList( pMessage, &slRequiredList, pError) \
						== Sdf_co_fail)
	{
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
			"sdf_fn_uaIsMessageRPR");
		return 1;
	}  
	   
	if(sdf_listSizeOf(&slRequiredList, &size1, pError) == Sdf_co_fail)
	{
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
			"sdf_fn_uaIsMessageRPR");
		(void)sdf_listDeleteAll(&slRequiredList, pError);
		return 1;
	}

	for(ii=0;ii<size1;ii++)
   	{
		if (sdf_listGetAt(&(slRequiredList), ii, (Sdf_ty_pvoid *)&pTemp, \
				pError) == Sdf_co_fail)
		{
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
				"sdf_fn_uaIsMessageRPR");
			(void)sdf_listDeleteAll(&slRequiredList, pError);
			return 1;
		}
		if(Sdf_mc_strcasecmp(pTemp,"100rel")==0)
		{
			isReqHdrPresent = Sdf_co_true;
			break;
		}
	}
	(void)sdf_listDeleteAll(&slRequiredList, pError);
	
	if( (isReqHdrPresent == Sdf_co_true) &&
		(isRseqPresent	== Sdf_co_true))
	{
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Exiting sdf_fn_uaIsMessageRPR");
		return 0;
	}	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_fn_uaIsMessageRPR");
	return 1;
}
#endif

#ifdef SDF_SESSION_TIMER
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUpdateSessionTimerInfo
 **
 ** DESCRIPTION: This function Updates the SessionTimerInfo
 **				 strucure of the CallObject  
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateSessionTimerInfo
#ifdef ANSI_PROTO
(Sdf_st_callObject *pCallObj,
SipMessage *pMessage,
Sdf_st_error *pErr)
#else
(pCallObj, pMessage, pErr)
Sdf_st_callObject *pCallObj;
SipMessage *pMessage;
Sdf_st_error *pErr;
#endif
{
	SipHeader dTempHeader;
	Sdf_st_sessionTimerInfo *pSessionTimerInfo;
	en_SipMessageType	dType; 
	Sdf_ty_bool dIsTimer=Sdf_co_false; 
	/* CSR 1-6360207 changes start */
	Sdf_ty_u16bit dRespCode;
	/* CSR 1-6360207 changes end */    

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaUpdateSessionTimerInfo");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;
	if(pMessage == Sdf_co_null)
		return Sdf_co_fail;
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionTimerInfo( ): "
			"Invalid Param: pCallObject",pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */
	/* 
	 * Get SessionTimerInfo from the call object.
	 */
	if (sdf_ivk_uaGetSessionTimerInfoFromCallObject(pCallObj, \
		&pSessionTimerInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionTimerInfo( ): "
			"Failed to get SessionTimer info from the call object", \
			pErr);
#endif				
		return Sdf_co_fail;
	}
	if(sip_getMessageType(pMessage,&dType, \
			(SipError*)&(pErr->stkErrCode))==SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionTimerInfo( ): "
			"Failed to get Message Type from the Sip Messsage", \
			pErr);
#endif
        /*Codenomicon fix merge: CSR_1-6714693*/
        (void)sdf_ivk_uaFreeSessionTimerInfo(pSessionTimerInfo);				
		return Sdf_co_fail;
	}
	/* CSR 1-6360207 changes start */
	/* get the response code if the message is response */
	if(dType==SipMessageResponse)
	{
			if (sdf_ivk_uaGetRespCodeFromSipMessage(pMessage, \
									&dRespCode, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
								(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionTimerInfo(): "
								"Getting RespCode from SipMessage failed",pErr);
#endif		
				return Sdf_co_fail;
		}
	}
	/* CSR 1-6360207 changes start */

	/* 	
	 * Gets the Session-Expires header from the SipMessage and sets the
	 * Session Interval and refresher to the  the call
	 * object
	 */
 	if (sip_getHeader(pMessage,SipHdrTypeSessionExpires, &dTempHeader, \
		(SipError*)&(pErr->stkErrCode)) != SipFail)
	{
		Sdf_ty_u32bit dSec;
		Sdf_ty_s8bit *dRefresher;
	 	if(sip_getSecondsFromSessionExpiresHdr(&dTempHeader,&dSec,\
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
			sip_freeSipHeader(&dTempHeader);	
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->stkErrCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionTimerInfo( ): "
				"Failed to get Secconds from Session-Expires header",pErr);
#endif
            /*Codenomicon fix merge: CSR_1-6714693*/			
            (void)sdf_ivk_uaFreeSessionTimerInfo(pSessionTimerInfo);	
			return Sdf_co_fail;
		}
		if(sip_getRefresherFromSessionExpiresHdr(&dTempHeader,&dRefresher,\
			(SipError*)&(pErr->stkErrCode))==SipFail)
		{
			if((SipError)(pErr->stkErrCode) !=E_NO_EXIST)
			{
				pErr->errCode = pErr->stkErrCode;
				sip_freeSipHeader(&dTempHeader);	
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionTimerInfo( ): "
					"Failed to get Refresher from Session-Expires header",pErr);
#endif
                /*Codenomicon fix merge: CSR_1-6714693*/			
                (void)sdf_ivk_uaFreeSessionTimerInfo(pSessionTimerInfo);	
				return Sdf_co_fail;
			}
			else
			/* CSR_1-6360207 Fix Start */
            {
                if(Sdf_en_refresherLocal == pCallObj->pSessionTimer->dRefresher)
                    Sdf_mc_strdup(dRefresher, "uac");
                else if(Sdf_en_refresherRemote == pCallObj->pSessionTimer->dRefresher)
                    /* CSR 1-6455932 changes start */				
                {
                    if ((dType == SipMessageResponse) && ((dRespCode >= 200) && (dRespCode < 300))) 
                        Sdf_mc_strdup(dRefresher, "uac"); 
                    else 
                        /* CSR 1-6455932 changes end */
                        Sdf_mc_strdup(dRefresher, "uas"); 
                    /* CSR 1-6455932 changes start */		
                }
                /* CSR 1-6455932 changes end */
                else
                    /* CSR_1-6360207 Fix End */ 
                    dRefresher=Sdf_co_null;
                /* CSR_1-6360207 Fix Start */ 
            }
            /* CSR_1-6360207 Fix End */ 
        }

        /* CSR_1-7033347 START*/
        /* take a back-up of the SessionTimer related Inforamtion to restore it
         * back in case of failure responses */
         pSessionTimerInfo->dOldSessionInterval = pSessionTimerInfo->dSessionInterval;
		 pSessionTimerInfo->dOldRefresher = pSessionTimerInfo->dRefresher;
         pSessionTimerInfo->dOldMinSe = pSessionTimerInfo->dMinSe;
        /* CSR_1-7033347 END*/


        /* 
		 * Update Session Interval  field of the
		 * SessionTimerInfo structure
		 */
		pSessionTimerInfo->dSessionInterval=dSec;

		/* 
		 * Update Refrresher field of the
		 * SessionTimerInfo structure
		 */
		if(dType==SipMessageResponse)
		{
			if(Sdf_mc_strcasecmp(dRefresher,"uac")==0)
				pSessionTimerInfo->dRefresher=Sdf_en_refresherLocal;
			else if(Sdf_mc_strcasecmp(dRefresher,"uas")==0)
				pSessionTimerInfo->dRefresher=Sdf_en_refresherRemote;
			else
				pSessionTimerInfo->dRefresher=Sdf_en_refresherNone;
		}
		else if(dType==SipMessageRequest)
		{
			if(Sdf_mc_strcasecmp(dRefresher,"uac")==0)
				pSessionTimerInfo->dRefresher=Sdf_en_refresherRemote;
			else if(Sdf_mc_strcasecmp(dRefresher,"uas")==0)
				pSessionTimerInfo->dRefresher=Sdf_en_refresherLocal;
			else
				pSessionTimerInfo->dRefresher=Sdf_en_refresherNone;
		}
		sip_freeSipHeader(&dTempHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)(&dRefresher), pErr);
	}
    /* CSR_1-5736356 BEGIN */
    /* 
     *  Change done to set the sesssion refresher to Local in
     * case it was set to be remote(UAS) in the request, but the 200
     * response idn't had a Session-Expires heaader.
     */
    /* CSR 1-6360207 BEGIN*/
    /* An additional check where the INVITE contains SE header without refresher but 
     * 200 OK did not contain SE header. In this case the refresher is set to uac. 
     * the check for Minse is to know if the INVITE contained SE header */ 
	else if((SipMessageResponse == dType) && 
					((pSessionTimerInfo->dRefresher == Sdf_en_refresherRemote) 
					 || ((pSessionTimerInfo->dSessionInterval >= sdf_fn_uaGetGlbMinSe()) 
							 && (pSessionTimerInfo->dRefresher == Sdf_en_refresherNone)))) 
    {    
        if((dRespCode >= 200) && (dRespCode < 300))
        {
            pSessionTimerInfo->dRefresher=Sdf_en_refresherLocal;
        }
    }
   	/* CSR_1-6360207 END*/ 
    /* CSR_1-5736356 END*/
	/* 	
	 * Gets the MinSE header from the SipMessage and sets the
	 * MinSE filed of the SessionTimerInfo structure of the call
	 * object
	 */

    /*Work-around for CSR 1-7249352 starts*/

	if( (dType ==SipMessageRequest) || \
			((dType==SipMessageResponse) && (dRespCode==422)) )
	{
		/*Work-around for CSR 1-7249352 ends*/

		if (sip_getHeader(pMessage,SipHdrTypeMinSE, &dTempHeader, \
			(SipError*)&(pErr->stkErrCode)) != SipFail)
		{
			Sdf_ty_u32bit dSec;
	 		if(sip_getSecondsFromMinSEHdr(&dTempHeader,&dSec,\
				(SipError*)&(pErr->stkErrCode)) == SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
				sip_freeSipHeader(&dTempHeader);	
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionTimerInfo( ): "
				"Failed to get Secconds from MinSe header",pErr);
#endif		
                /*Codenomicon fixes merge: CSR_1-6714693*/
            	(void)sdf_ivk_uaFreeSessionTimerInfo(pSessionTimerInfo);	
				return Sdf_co_fail;
			}
		/* 
		 * Update MinSe  field of the
		 * SessionTimerInfo structure
		 */
		pSessionTimerInfo->dMinSe=dSec;
		sip_freeSipHeader(&dTempHeader);	
		}
		/*Work-around for CSR 1-7249352 starts*/
	}	
	/*Work-around for CSR 1-7249352 ends*/

	if(sdf_fn_uaIsSupportedSessionTimer(pCallObj,pMessage,\
				&dIsTimer,pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_extraHeaderAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionTimerInfo(): "
			"Getting Supported:Session Timer from CallObject Error",pErr);
#endif
		pErr->errCode=Sdf_en_extraHeaderAccessError;
        /*Codenomicon fix merge: CSR_1-6714693*/			
        (void)sdf_ivk_uaFreeSessionTimerInfo(pSessionTimerInfo);
		return Sdf_co_fail;
	}

	pSessionTimerInfo->dIsSupportedTimer=dIsTimer;
	/*
	 * If Session-Timer is supported by the peer entity, then the request
	 * should come with Session-Expires > Min-SE. Otherwise, we should
	 * increase the value of Session-Expires equal to Min-SE
	 */
	if ((dIsTimer == Sdf_co_false) && \
			(pSessionTimerInfo->dMinSe > pSessionTimerInfo->dSessionInterval))
	{
		pSessionTimerInfo->dSessionInterval = pSessionTimerInfo->dMinSe;
	}
	(void)sdf_ivk_uaFreeSessionTimerInfo(pSessionTimerInfo);
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaUpdateSessionTimerInfo");
	return Sdf_co_success;

}
#endif



/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetReasonFromTable
 **
 ** DESCRIPTION: This function sets the Reason from the CodeReasonMapping 
 **				 table corresponding to the Response code input to the 
 **				 function
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetReasonFromTable
#ifdef ANSI_PROTO
	(Sdf_ty_u16bit dRespCode,
	Sdf_ty_s8bit **ppReason,
	Sdf_st_error *pError)
#else
	(dRespCode, ppReason, pError)
	Sdf_ty_u16bit dRespCode;
	Sdf_ty_s8bit **ppReason;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_u32bit dIndex=0;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_fn_uaGetReasonFromTable");
	
#ifdef SDF_PARAMVALIDATION

	if (pError == Sdf_co_null)
		return Sdf_co_fail;

#endif
	*ppReason=Sdf_co_null;

	for (dIndex = 0; dIndex < Sdf_co_scTableSize; dIndex++)
	{
		if (dRespCode == sdf_gl_dCodeReasonMapping[dIndex].dStatusCode)
		{
			*ppReason =(Sdf_ty_s8bit *)sdf_memget(0, \
				Sdf_mc_strlen(sdf_gl_dCodeReasonMapping[dIndex].dReason)+1, \
				pError);
			if(Sdf_co_null == *ppReason)
			{
				pError->errCode  = Sdf_en_noMemoryError;
				return Sdf_co_fail;
			}

			Sdf_mc_strcpy(*ppReason, sdf_gl_dCodeReasonMapping[dIndex].dReason);
			pError->errCode = Sdf_en_noUaError;
	        sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_fn_uaGetReasonFromTable");
			return Sdf_co_success;
		}
	}
    /*UATK CSR 1-7806133 fix*/
	*ppReason = Sdf_mc_strdupCallHandler("Unknown Response code");
	
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaGetReasonFromTable");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaIsNumber
 **
 ** DESCRIPTION: This function takes in a string and returns true 
 **				 if it is an integer
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaIsNumber 
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *pStr)
#else
	(pStr)
	Sdf_ty_s8bit *pStr;
#endif	
{
	Sdf_ty_u32bit dIndex=0, dLength=0;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaIsNumber");

#ifdef SDF_PARAMVALIDATION
	if (pStr == Sdf_co_null)
		return Sdf_co_fail;
#endif

	dLength = Sdf_mc_strlen(pStr);

	for (dIndex = 0; dIndex<dLength; dIndex++)
	{
/*Change from UATK 2.1 Start IMS client branch*/
#ifdef SDF_WINDOWS
		if (!((pStr[dIndex] <= '9') && (pStr[dIndex] >= '0')))
		{
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
				"Exiting sdf_fn_uaIsNumber");
			return Sdf_co_fail;
		}
#else		
		if (!isdigit((Sdf_ty_u8bit)pStr[dIndex]))
		{
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
				"Exiting sdf_fn_uaIsNumber");
			return Sdf_co_fail;
		}
#endif
/*Change from UATK 2.1 End*/		
	}
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_fn_uaIsNumber");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetAddrSpecAsString
 **
 ** DESCRIPTION: This function gets the addrspec as a string element. The 
 **				 header types that are valid inputs are From and To header. 
 **				 Passing any other header type will return error as of now.
 **
 ******************************************************************************/
Sdf_ty_retVal  sdf_fn_uaGetAddrSpecAsString
#ifdef ANSI_PROTO
	(SipMessage *pMsg,
    en_HeaderType dHeaderType, 
	Sdf_ty_s8bit **ppAddrSpecStr,
	Sdf_st_error *pErr)
#else
	(pMsg,dHeaderType,ppAddrSpecStr,pErr)
	SipMessage *pMsg;
	en_HeaderType dHeaderType;
	Sdf_ty_s8bit **ppAddrSpecStr;
	Sdf_st_error *pErr;
#endif	
{
	SipHeader hdr;
	SipAddrSpec *pAddrSpec = Sdf_co_null;
	Sdf_ty_s8bit dOut[Sdf_co_largeBufferSize];
#if !defined MICROUA && defined SDF_LINT
	Sdf_ty_s8bit *pStr = Sdf_co_null;
#endif
	dOut[0] ='\0';
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaGetAddrSpecAsString");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;
	if (ppAddrSpecStr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGetAddrSpecAsString( ): "
			"Invalid param value: addrspec string",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */
	*ppAddrSpecStr =Sdf_co_null;

	if ((dHeaderType != SipHdrTypeFrom) && (dHeaderType != SipHdrTypeTo))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGetAddrSpecAsString( ): "
			"Invalid param value: header type",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	/*
	 * If the header is a From header, get addrspec from the from header.
	 */
	if(sip_getHeader(pMsg,dHeaderType,&hdr,(SipError*)&(\
		pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGetAddrSpecAsString(): "
			"sip_getHeader failed",pErr);
#endif			
		return Sdf_co_fail;
	}
	if(dHeaderType == SipHdrTypeFrom)
	{
		if(sip_getAddrSpecFromFromHdr(&hdr,&pAddrSpec,(SipError*)\
			&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
			sip_freeSipHeader(&hdr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaGetAddrSpecAsString(): "
				"sip_getAddrSpecFromFromHdr failed",pErr);
#endif				
			return Sdf_co_fail;
		}
	}	

	/*
	 * If the header is a To header, get addrspec from the To header.
	 */
	 if(dHeaderType == SipHdrTypeTo)
	 {
		if(sip_getAddrSpecFromToHdr(&hdr,&pAddrSpec,(SipError*)
			&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
			sip_freeSipHeader(&hdr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaGetAddrSpecAsString(): "
				"sip_getAddrSpecFromToHdr failed",pErr);
#endif				
			return Sdf_co_fail;
		}
	}	

	/*
	 * This gets the addr spec retrieved as a string
	 */
	if(sip_formAddrSpec(Sdf_co_null,(Sdf_ty_s8bit*)dOut,\
				pAddrSpec,(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
		sip_freeSipHeader(&hdr);
		sip_freeSipAddrSpec(pAddrSpec);
#if !defined MICROUA && defined SDF_LINT
  (void)sdf_memfree(0,(Sdf_ty_pvoid *)&pStr,pErr);
#endif        
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGetAddrSpecAsString(): "
			"sip_formAddrSpec failed",pErr);
#endif			
		return Sdf_co_fail;
	}
#if !defined MICROUA && defined SDF_LINT
   Sdf_mc_strcpy(dOut,pStr); 
  (void)sdf_memfree(0,(Sdf_ty_pvoid *)&pStr,pErr);
#endif 
        
	*ppAddrSpecStr = Sdf_mc_strdupRemoteRetransMngr(dOut);
	sip_freeSipAddrSpec(pAddrSpec);
	sip_freeSipHeader(&hdr);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaGetAddrSpecAsString");
	return Sdf_co_success;
}


#ifdef SDF_HA 

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGenerateRetransKey
 **
 ** DESCRIPTION: This function generates a key for the retransmission element. 
 **				 Stored in the key are the From, To headers, CSeq, method and 
 **				 callid.It also stores the message Type.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGenerateRetransKey
#ifdef ANSI_PROTO
	(SipMessage *pMsg,
    Sdf_st_remoteRetransKey *pKey, 
	Sdf_st_error *pErr)
#else
	(pMsg,pKey,pErr)
	SipMessage *pMsg;
	Sdf_st_remoteRetransKey *pKey;
	Sdf_st_error *pErr;
#endif	
{
	SipHeader header;
	Sdf_ty_s8bit *pCSeqMethod = Sdf_co_null, *pCallIdValue= Sdf_co_null;
	en_SipMessageType	dType; 
	SipHeader		 	ViaHeader;
	Sdf_ty_s8bit		*pViaBranch = Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaGenerateRetransKey");
	
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;
	if(pKey == Sdf_co_null)
		return Sdf_co_fail;
		
#endif	/* End of param validation ifdef */

	/* 
	 * Get the From Header 
	 */
	if(sip_getHeader(pMsg, SipHdrTypeFrom, (pKey)->pFromHeader, (SipError*)\
		&(pErr->stkErrCode)) == SipFail)
	{	
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateRetransKey(): "
			"failed to get From header",pErr);
#endif			
		return Sdf_co_fail;
	}

	/* 
	 * Get the To Header 
	 */
	if(sip_getHeader(pMsg, SipHdrTypeTo, (pKey)->pToHeader, (SipError*)\
		&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateRetransKey(): "
			"failed to get To header",pErr);
#endif			
		return Sdf_co_fail;
	}

	/* 
	 * Get the CallID header.
	 */
	if(sip_getHeader(pMsg,SipHdrTypeCallId,&header,\
		(SipError*) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateRetransKey(): "
			"failed to get Callid header",pErr);
#endif			
		return Sdf_co_fail;
	}
	if(sip_getValueFromCallIdHdr(&header, &pCallIdValue,\
		(SipError*) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateRetransKey(): "
			"failed to get value from Callid header",pErr);
#endif			
		return Sdf_co_fail;
	}
	Sdf_mc_strdup((pKey)->pCallId,pCallIdValue);
	sip_freeSipHeader(&header);

	/* 
	 * Get CSeq header. Retrieve method and cseq from it.
	 */
	if(sip_getHeader(pMsg,SipHdrTypeCseq,&header,\
		(SipError*) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateRetransKey(): "
			"failed to get CSeq header",pErr);
#endif			
		return Sdf_co_fail;
	}
	if(sip_getMethodFromCseqHdr(&header, &pCSeqMethod,\
		(SipError*) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateRetransKey(): "
			"failed to get method from CSeq header",pErr);
#endif			
		return Sdf_co_fail;
	}
	if (Sdf_mc_strcmp(pCSeqMethod, "ACK") == 0)
		Sdf_mc_strdup((pKey)->pMethod,"INVITE");
	else
		Sdf_mc_strdup((pKey)->pMethod,pCSeqMethod);

	if(sip_getSeqNumFromCseqHdr(&header, &((pKey)->dCseqNo), (SipError*)\
		&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateRetransKey(): "
			"failed to get sequence number from CSeq header",pErr);
#endif			
		return Sdf_co_fail;
	}
	/*Get Type of message */
	if(sip_getMessageType(pMsg,&dType,(SipError*)&(pErr->stkErrCode))==SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateRetransKey(): "
			"failed to get Message type",pErr);
#endif			
		return Sdf_co_fail;
	}
	/*Add type of Message into the key*/
	pKey->dMessageType=dType;
	if(sip_getHeader(pMsg, SipHdrTypeVia, &ViaHeader,\
				(SipError*)&(pErr->stkErrCode)) == SipSuccess)
	{
		if(sdf_fn_GetBranchFromViaHdr( &ViaHeader, \
			&pViaBranch, pErr) != Sdf_co_success)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaGenerateRetransKey(): "
				"Failed to get Via Branch from Via header",pErr);
#endif
			pViaBranch = Sdf_co_null;
		}
	}
	else
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateRetransKey(): "
			"Failed to get Via header from SIP message",pErr);
#endif
	}

	if( pKey->pViaBranch != Sdf_co_null )
	{
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)&(pKey->pViaBranch), pErr);
	}

	if( pViaBranch != Sdf_co_null )
		Sdf_mc_strdup((pKey)->pViaBranch, pViaBranch);
	else
		pKey->pViaBranch = Sdf_co_null;

	sip_freeSipHeader(&ViaHeader);
	sip_freeSipHeader(&header);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting"
		 " sdf_fn_uaGenerateRetransKey");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaInsertIntoRemoteRetransList
 **
 ** DESCRIPTION: This function forms a sdf_st_remoteRetransBuffer struct and 
 **				 inserts it into the UA Toolkit's remote retrans list
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaInsertIntoRemoteRetransList 
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObj,
	Sdf_st_transportInfo *pDestInfo,
	Sdf_ty_socketId dSockfd,
	SipMessage *pSipMsg,
    SipOptions dOptions, 
	Sdf_st_eventContext	*pContext,
	Sdf_st_error *pErr)
#else
	(pObj,pDestInfo,dSockfd,pSipMsg,dOptions, pContext, pErr)
	Sdf_st_callObject *pObj;
	Sdf_st_transportInfo *pDestInfo;
	Sdf_ty_socketId dSockfd;
	SipMessage *pSipMsg;
	SipOptions dOptions;
	Sdf_st_eventContext	*pContext;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dTimeStamp=0;
	Sdf_st_hash				*pRemoteRetransHash;
	Sdf_st_remoteRetransBuffer	*pRetransElem = Sdf_co_null;
#ifndef SDF_USE_PREALLOCBUFFERS
	Sdf_ty_u32bit dLength=0;
	Sdf_ty_s8bit dTempBuffer[Sdf_mc_maxMsgSize];
#ifdef SDF_LINT	
	Sdf_ty_s8bit *pdTempBuffer = Sdf_co_null;
#endif	
#endif
	Sdf_st_remoteRetransKey *pTempKey = Sdf_co_null;
	Sdf_st_commonInfo *pCommonInfo = Sdf_co_null;
#ifdef SDF_TRACE	
	Sdf_ty_s8bit trace[Sdf_co_traceBufferSize];
	Sdf_ty_uaTraceLevel dTraceLevel;
#endif	

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaInsertIntoRemoteRetransList");

	/* Dummy statement to avoid compiler warning */
	(void)dOptions; 
	(void)pContext;

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;

	if(pObj == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"Invalid param: pCallObject",pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pDestInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"Invalid param: pDestInfo",pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#ifdef SDF_USE_PREALLOCBUFFERS
	if (pContext == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"Invalid param: pContext",pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
#endif	/* End of param validation ifdef */

#ifdef SDF_APP_CONTROLLED_REMOTE_RETX
	if (sdf_cbk_uaGetRemoteRetransHashTable(&pRemoteRetransHash, pContext, \
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callBackError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"Callback sdf_cbk_uaGetRemoteRetransHashTable failed",pErr);
#endif
		pErr->errCode=Sdf_en_callBackError;
		return Sdf_co_fail;
	}

	if (pRemoteRetransHash == Sdf_co_null)
	{
		/* App intends to take complete control of the remote retransmission
		 * logic and it does not require any support from UATK. So we return
		 * without doing anything.
		 */
		return Sdf_co_success;
	}

#else
	pRemoteRetransHash = &(pGlbToolkitData->dRemoteRetransHash);
#endif
	
	/*
	 * Initialize a buffer to add to the hash table.
	 */
	if (sdf_ivk_uaInitRemoteRetransBuffer(&pRetransElem ,pErr) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"failed to init RemoteRetransBuffer",pErr);
#endif			
		return Sdf_co_fail;
	}

	/*
	 * Generate a key to be used to index into this buffer. 
	 */
	if(sdf_fn_uaGenerateRetransKey(pSipMsg, (pRetransElem->pKey),\
		pErr) == Sdf_co_fail)
	{
		(void)sdf_fn_uaFreeRemoteRetransBuffer(pRetransElem);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"failed to generate RemoteRetrans key",pErr);
#endif			
		pErr->errCode = Sdf_en_retransKeyError;
		return Sdf_co_fail;
	}


#ifdef SDF_USE_PREALLOCBUFFERS
	/* Store a reference to the event context and the buffer length; 
	 * Note that the buffer is contained in the event context itself 
	 */
	HSS_LOCKEDINCREF(pContext->dRefCount);
	pRetransElem->pContext = pContext;
	pRetransElem->dLength = Sdf_mc_strlen(pContext->pDirectBuffer);
#else
	/*
	 * Store the SIP message buffer (being sent out) in the remote
	 * retrans buffer that has just been initialized. Use the stack's
	 * message formation API for the same.
	 */
	dTempBuffer[0] = '\0';
	sip_memset(dTempBuffer, 0, Sdf_mc_maxMsgSize);
	if(sip_formMessage( pSipMsg,&dOptions, dTempBuffer,\
		 &dLength, (SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = Sdf_en_memoryError;
		(void)sdf_fn_uaFreeRemoteRetransBuffer(pRetransElem);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"failed to form Sip message",pErr);
#endif			
		return Sdf_co_fail;
	}
	pRetransElem->pBuffer = (Sdf_ty_s8bit *) \
		sdf_memget(Sdf_mc_remoteRetransMngrMemId, dLength, pErr);
	if (pRetransElem->pBuffer == Sdf_co_null)
	{
		(void)sdf_fn_uaFreeRemoteRetransBuffer(pRetransElem);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"Failed to allocate memory for buffer of retransmision "
			"element", pErr);
#endif			
		pErr->errCode = Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	/* FIX for ISUP */
	/* Sdf_mc_strncpy(pRetransElem->pBuffer, dTempBuffer, dLength);	*/
#ifdef SDF_LINT	
	pdTempBuffer = dTempBuffer;
	Sdf_mc_memcpy(pRetransElem->pBuffer, pdTempBuffer, dLength);	
#else
	Sdf_mc_memcpy(pRetransElem->pBuffer, dTempBuffer, dLength);	
#endif
	
	pRetransElem->dLength = dLength;
#endif

	/* 
	 * Put the timestamp before adding this retransbuffer.
	 */
	if ( sdf_fn_uaGetCurrentTimeInSecs(&dTimeStamp) \
		== Sdf_co_fail)
	{
		(void)sdf_fn_uaFreeRemoteRetransBuffer(pRetransElem);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"failed to get current time in seconds",pErr);
#endif			
		return Sdf_co_fail;
	}
	pRetransElem->dTimeStamp = dTimeStamp;


	/*
	 * Fill the destination info within the timer buffer.
	 */
	if (sdf_ivk_uaCloneTransportInfo(pRetransElem->pDestInfo, pDestInfo, \
		pErr) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
		(void)sdf_fn_uaFreeRemoteRetransBuffer(pRetransElem);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"Failed to clone transport info structure",pErr);
#endif			
		return Sdf_co_fail;
	}

	/* 
	 * Store a reference to the appData structure of the call object.
	 * This is required whenever a remote retransmission occurs. The
	 * Remote retransmission manager calls sip_sendToNetwork passing
	 * the application context data to it.
	 */
	if (sdf_ivk_uaGetAppDataFromCallObject(pObj, \
		&(pRetransElem->pAppData), pErr) == Sdf_co_fail)
	{	
		(void)sdf_fn_uaFreeRemoteRetransBuffer(pRetransElem);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_appDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList( ): "
			"Failed to get AppData from call object\n",pErr);
#endif			
		pErr->errCode=Sdf_en_appDataAccessError;
		return Sdf_co_fail;
	}
	/*
	 * Add the Socket descriptor to the retrans Key
	 */
	pRetransElem->dSockfd = dSockfd;

	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
	
	/*
	 * Add the retrans buffer to the hash table of retrans buffers here.
	 */
	 pTempKey = pRetransElem->pKey;
	 HSS_LOCKEDINCREF(pTempKey->dRefCount);
	 if ((sdf_ivk_uaHashAdd(pRemoteRetransHash, (Sdf_ty_pvoid)pRetransElem, \
		(Sdf_ty_pvoid)pTempKey)) == Sdf_co_fail)
	 {
		(void)sdf_fn_uaFreeRemoteRetransBuffer(pRetransElem);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
			"Failed to add element in remote retransmission hash table.",\
			pErr);
#endif			
		return Sdf_co_fail;
	 }
	 
#ifdef SDF_TRACE			
	/* 
	 * Print trace indicating the message inserted into the buffer.
	 */
	(void)sdf_ivk_uaGetTraceLevel(&dTraceLevel, pErr);
	if (sdf_ivk_uaGetCommonInfoFromCallObject(pObj, \
			&pCommonInfo, pErr) == Sdf_co_fail)
		return Sdf_co_fail;

	if (dTraceLevel == Sdf_en_briefTraces)
	{
		(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
			"Call: %s - "
			"Added buffer to Remote Retransmission List.\n", \
			pCommonInfo->pKey->pCallId);
		sdf_fn_trace(Sdf_en_briefTraces, Sdf_en_remoteRetransTraces, \
			(Sdf_ty_s8bit *)trace, 0, pErr);
	}
	else
	{
		Sdf_ty_s8bit *pMethod;
		en_SipMessageType dReqRespType;

		if(sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, &pMethod, \
			pErr) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
			return Sdf_co_fail;
		}
		if (sip_getMessageType(pSipMsg, &dReqRespType, \
			(SipError*)&(pErr->stkErrCode)) ==  SipFail)
		{
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
				"failed to get Message type",pErr);
#endif			
			return Sdf_co_fail;
		}
		if(dReqRespType == SipMessageResponse)
		{
			Sdf_ty_u16bit dRespCode;
			if (sdf_ivk_uaGetRespCodeFromSipMessage(pSipMsg, \
				&dRespCode, pErr) == Sdf_co_fail)
				return Sdf_co_fail;

			(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
				"Call: %s - Added %d to %s to "
				"Remote Retransmission List.\n", pCommonInfo->pKey->pCallId,\
				dRespCode, pMethod);

			sdf_fn_trace(Sdf_en_detailedTraces, \
				Sdf_en_remoteRetransTraces, (Sdf_ty_s8bit *)trace, \
				0, pErr);
		}
		else
		{
			(void)sdf_fn_uaSnprintf(trace, Sdf_co_traceBufferSize,\
				"Call: %s - Added %s to Remote "
				"Retransmission List.\n", pCommonInfo->pKey->pCallId, \
				pMethod);

			sdf_fn_trace(Sdf_en_allTraceLevels, \
				Sdf_en_remoteRetransTraces, (Sdf_ty_s8bit *)trace, \
				0, pErr);
		}
	}
	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#endif			

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaInsertIntoRemoteRetransList");
	return Sdf_co_success;
}

#endif /* SDF_HA */


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCloneSlExtraHeadersList
 **
 ** DESCRIPTION: This function clones a list of Extra headers.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCloneSlExtraHeadersList
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pDest,
	Sdf_ty_slist *pSrc,
	Sdf_st_error *pErr)
#else
	(pDest, pSrc, pErr)
	Sdf_ty_slist *pDest;
	Sdf_ty_slist *pSrc;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaCloneSlExtraHeadersList");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;
	if (pSrc == Sdf_co_null)
	{
		return Sdf_co_fail;
	}
	if (pDest == Sdf_co_null)
	{
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/* 
	 * Delete the previously existing members of the destination list.
	 */
	if (sdf_listDeleteAll(pDest ,pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCloneSlExtraHeadersList( ): "
			"Could not delete the destination list before cloning", \
			pErr);
#endif			
		return Sdf_co_fail;
	}

	(void)sdf_listInitIterator(pSrc, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_configExtraHeaders *pTemp,*pClone;

		pTemp = (Sdf_st_configExtraHeaders*)\
			(dListIterator.pCurrentElement->pData);

		if (sdf_ivk_uaInitConfigExtraHeaders(&pClone,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCloneSlExtraHeadersList( ): "
				"Could not init a extra header structure",pErr);
#endif				
			return Sdf_co_fail;
		}
		if(sip_cloneSipParam(pClone->pParam,pTemp->pParam, \
			(SipError *)&(pErr->stkErrCode)) ==SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCloneSlExtraHeadersList( ): "
				"Could not clone a SipParam",pErr);
#endif				
			return Sdf_co_fail;
		}
		if(pTemp->pData!=Sdf_co_null)
		{
			Sdf_mc_strdup(pClone->pData,(Sdf_ty_s8bit*)pTemp->pData);
		}

		/*
		 * Append the cloned structure to the destination list
		 */
		if(sdf_listAppend(pDest,pClone,pErr) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeConfigExtraHeaders(pClone);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCloneSlExtraHeadersList( ): "
				"Could not append a cloned extra header to the "
				"destination list",pErr);
#endif				
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}	

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaCloneSlExtraHeadersList");
	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaInitCsmTable
 **
 ** DESCRIPTION: This function initializes the CSM transition tables
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitCsmTable
#ifdef ANSI_PROTO
	(void)
#else
	(void)
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaInitCsmTable");

	/* 
	 * Note that in the transition tables, only valid transitions 
	 * are assigned. Invalid table elements imply invalid state 
	 * transition 
	 */

	/* 
	 * Filling in Transitions in the dCsmNxtStateOnMsgRecd table 
	 */

	/* Transitions & Message Sent when INVITE is received . */
	dCsmNxtStateOnMsgRecd[Sdf_en_invite][Sdf_en_idle].\
		dNextState = Sdf_en_inviteReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_invite][Sdf_en_idle].\
		dMsgToBeSent, "200");

	dCsmNxtStateOnMsgRecd[Sdf_en_invite][Sdf_en_callEstablished].\
		dNextState = Sdf_en_reInviteReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_invite][Sdf_en_callEstablished].\
		dMsgToBeSent, "200");


	/* Transitions & Message Sent when CANCEL is received . */
#ifdef SDF_UASSF
	dCsmNxtStateOnMsgRecd[Sdf_en_cancel][Sdf_en_inviteReceived].\
		dNextState = Sdf_en_inviteCancelled;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_cancel] \
			[Sdf_en_inviteReceived].dMsgToBeSent, "200");
#endif

	dCsmNxtStateOnMsgRecd[Sdf_en_cancel][Sdf_en_provisionalRespSent].\
		dNextState = Sdf_en_inviteCancelled;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_cancel] \
			[Sdf_en_provisionalRespSent].dMsgToBeSent, "200");

#ifdef SDF_UASSF
	dCsmNxtStateOnMsgRecd[Sdf_en_cancel][Sdf_en_reInviteReceived].\
		dNextState = Sdf_en_reInviteCancelled;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_cancel]\
		[Sdf_en_reInviteReceived].dMsgToBeSent, "200");
#endif

	dCsmNxtStateOnMsgRecd[Sdf_en_cancel][Sdf_en_reInviteprovRespSent].\
		dNextState = Sdf_en_reInviteCancelled;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_cancel]\
		[Sdf_en_reInviteprovRespSent].dMsgToBeSent, "200");
	
	dCsmNxtStateOnMsgRecd[Sdf_en_cancel][Sdf_en_finalSuccessSent].\
		dNextState = Sdf_en_finalSuccessSent;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_cancel]\
		[Sdf_en_finalSuccessSent].dMsgToBeSent, "200");

	dCsmNxtStateOnMsgRecd[Sdf_en_cancel][Sdf_en_finalFailureSent].\
		dNextState = Sdf_en_finalFailureSent;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_cancel]\
		[Sdf_en_finalFailureSent].dMsgToBeSent, "200");

	dCsmNxtStateOnMsgRecd[Sdf_en_cancel][Sdf_en_reInviteFinalResponseSent].\
		dNextState = Sdf_en_reInviteFinalResponseSent;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_cancel]\
		[Sdf_en_reInviteFinalResponseSent].dMsgToBeSent, "200");

	dCsmNxtStateOnMsgRecd[Sdf_en_cancel][Sdf_en_callEstablished].\
		dNextState = Sdf_en_callEstablished;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_cancel]\
		[Sdf_en_callEstablished].dMsgToBeSent, "200");

	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_reInviteprovRespSent].\
 		dNextState = Sdf_en_byeReceived;
 	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye]\
 			[Sdf_en_reInviteprovRespSent].dMsgToBeSent, "200");
 	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_reInviteReceived].\
 		dNextState = Sdf_en_byeReceived;
 	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye]\
 			[Sdf_en_reInviteReceived].dMsgToBeSent, "200");


	/* Transitions & Message Sent when ACK is received  */
	dCsmNxtStateOnMsgRecd[Sdf_en_ack][Sdf_en_finalSuccessSent].
		dNextState= Sdf_en_callEstablished;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_ack][Sdf_en_finalSuccessSent].\
		dMsgToBeSent, "");

	dCsmNxtStateOnMsgRecd[Sdf_en_ack][Sdf_en_byeSent].
		dNextState= Sdf_en_byeSent;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_ack][Sdf_en_byeSent].\
		dMsgToBeSent, "");

	
	dCsmNxtStateOnMsgRecd[Sdf_en_ack][Sdf_en_finalFailureSent].\
		dNextState = Sdf_en_idle;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_ack][Sdf_en_finalFailureSent].\
		dMsgToBeSent, "");
	
	dCsmNxtStateOnMsgRecd[Sdf_en_ack][Sdf_en_callEstablished].\
		dNextState = Sdf_en_callEstablished;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_ack][Sdf_en_callEstablished].\
		dMsgToBeSent, "");
	
	dCsmNxtStateOnMsgRecd[Sdf_en_ack][Sdf_en_reInviteFinalResponseSent].\
		dNextState = Sdf_en_callEstablished;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_ack]\
		[Sdf_en_reInviteFinalResponseSent].dMsgToBeSent, "");

	/* Transitions & Message Sent when BYE is received  */

	/* Below state is to handle the case of accepting the BYE from UAS when 
	 * 2xx timeout */
	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_finalSuccessReceived].\
		dNextState = Sdf_en_byeReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye]\
		[Sdf_en_finalSuccessReceived].dMsgToBeSent, "200");

	
	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_callEstablished].\
		dNextState = Sdf_en_byeReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_callEstablished].\
		dMsgToBeSent, "200");
	
	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_byeSent].\
		dNextState = Sdf_en_byeReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_byeSent].\
		dMsgToBeSent, "200");
	
	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_provisionalRespSent].\
		dNextState = Sdf_en_inviteCancelled;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye] \
			[Sdf_en_provisionalRespSent].dMsgToBeSent, "200");
	
	/*   Race Condition for the Bye Received with the Reinvite or Reinvite provisional response */ 
	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_reInviteSent].\
		dNextState = Sdf_en_byeReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye] \
			[Sdf_en_provisionalRespSent].dMsgToBeSent, "200");

	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_reInviteprovRespReceived].\
		dNextState = Sdf_en_byeReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye] \
			[Sdf_en_provisionalRespSent].dMsgToBeSent, "200");
	
	
	/* The BYE received after sending the final response has no effect */
	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_finalSuccessSent].\
		dNextState = Sdf_en_finalSuccessSent;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_finalSuccessSent].\
		dMsgToBeSent, "200");
	
	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_finalFailureSent].\
		dNextState = Sdf_en_finalFailureSent;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_finalFailureSent].\
		dMsgToBeSent, "200");

	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_reInviteFinalResponseSent].\
		dNextState = Sdf_en_reInviteFinalResponseSent;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye]\
		[Sdf_en_reInviteFinalResponseSent].dMsgToBeSent, "200");

	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_reInviteFinalResponseReceived].\
		dNextState = Sdf_en_byeReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye]\
		[Sdf_en_reInviteFinalResponseReceived].dMsgToBeSent, "200");

	dCsmNxtStateOnMsgRecd[Sdf_en_bye][Sdf_en_inviteCancelled].\
		dNextState = Sdf_en_inviteCancelled;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_bye]\
		[Sdf_en_inviteCancelled].dMsgToBeSent, "200");

	dCsmNxtStateOnMsgRecd[Sdf_en_cancel][Sdf_en_inviteCancelled].\
		dNextState = Sdf_en_inviteCancelled;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_cancel]\
		[Sdf_en_inviteCancelled].dMsgToBeSent, "200");

	/*  Transitions & messages Send when provisional Response received*/
	dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp][Sdf_en_inviteSent].\
		dNextState = Sdf_en_provisionalRespReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp]\
		[Sdf_en_inviteSent].dMsgToBeSent,"");
	

	dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp][Sdf_en_reInviteSent].\
		dNextState = Sdf_en_reInviteprovRespReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp]\
		[Sdf_en_reInviteSent].dMsgToBeSent, "");

	dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp]\
			[Sdf_en_reInviteprovRespReceived].\
		dNextState = Sdf_en_reInviteprovRespReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp]\
		[Sdf_en_reInviteprovRespReceived].dMsgToBeSent,"");

	dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp]\
		[Sdf_en_provisionalRespReceived].\
		dNextState = Sdf_en_provisionalRespReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp]\
		[Sdf_en_provisionalRespReceived].dMsgToBeSent,"");


	/*  Transitions & Message Sent when final success 
		response is received . */
	dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess][Sdf_en_provisionalRespReceived].\
		dNextState = Sdf_en_finalSuccessReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess]\
		[Sdf_en_provisionalRespReceived].dMsgToBeSent,"ACK");

	dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess][Sdf_en_inviteSent].\
		dNextState = Sdf_en_finalSuccessReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess]\
		[Sdf_en_inviteSent].dMsgToBeSent,"ACK");

	dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess][Sdf_en_byeSent].\
		dNextState = Sdf_en_callTerminated;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess][Sdf_en_byeSent].\
		dMsgToBeSent, "");

	dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess][Sdf_en_transactionSent].\
		dNextState = Sdf_en_idle;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess]\
		[Sdf_en_transactionSent].dMsgToBeSent, "");

	dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess][Sdf_en_reInviteSent].\
		dNextState = Sdf_en_reInviteFinalResponseReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess]\
		[Sdf_en_reInviteSent].dMsgToBeSent, "ACK");


	dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess][Sdf_en_overlapTransactionSent].\
		dNextState = Sdf_en_idle;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess]\
		[Sdf_en_overlapTransactionSent].dMsgToBeSent, "");

	/*  Transitions & Message Sent when final failure 
		response is received . */

	dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure][Sdf_en_provisionalRespReceived].\
		dNextState = Sdf_en_finalFailureReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure]\
		[Sdf_en_provisionalRespReceived].dMsgToBeSent,"ACK");


	dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess] \
		[Sdf_en_reInviteprovRespReceived].\
		dNextState = Sdf_en_reInviteFinalResponseReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess]\
		[Sdf_en_reInviteprovRespReceived].dMsgToBeSent, "ACK");

	dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure] \
		[Sdf_en_reInviteprovRespReceived].\
		dNextState = Sdf_en_reInviteFinalResponseReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure]\
		[Sdf_en_reInviteprovRespReceived].dMsgToBeSent, "ACK");

	dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure] \
		[Sdf_en_reInviteprovRespReceived].\
		dNextState = Sdf_en_reInviteFinalResponseReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure]\
		[Sdf_en_reInviteprovRespReceived].dMsgToBeSent, "ACK");

	dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure][Sdf_en_inviteSent].\
		dNextState = Sdf_en_finalFailureReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure]\
		[Sdf_en_inviteSent].dMsgToBeSent,"ACK");

	dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure][Sdf_en_byeSent].\
		dNextState = Sdf_en_callEstablished;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure][Sdf_en_byeSent].\
		dMsgToBeSent, "");

	dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure][Sdf_en_transactionSent].\
		dNextState = Sdf_en_idle;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure]\
		[Sdf_en_transactionSent].dMsgToBeSent, "");

	dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure][Sdf_en_reInviteSent].\
		dNextState = Sdf_en_reInviteFinalResponseReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure]\
		[Sdf_en_reInviteSent].dMsgToBeSent, "ACK");


	dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure][Sdf_en_overlapTransactionSent].\
		dNextState = Sdf_en_idle;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure]\
		[Sdf_en_overlapTransactionSent].dMsgToBeSent, "");

	/* Transitions & Message Sent when OPTIONS is received */
	dCsmNxtStateOnMsgRecd[Sdf_en_RegularTxn][Sdf_en_idle].\
		dNextState = Sdf_en_transactionReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_RegularTxn][Sdf_en_idle].\
		dMsgToBeSent, "200");

	/* Transitions & Message Sent when REGISTER is received */
	dCsmNxtStateOnMsgRecd[Sdf_en_register][Sdf_en_idle].\
		dNextState = Sdf_en_transactionReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_register][Sdf_en_idle].\
		dMsgToBeSent, "501");

	/* 
	 * Transitions & Message Sent when an Unknown method is received.
	 * These are transitions when a method that is supported by the 
	 * application (besides INVITE/BYE and other well-known methods) 
	 * but not by the UA Toolkit, is received. As a default 
	 * implementation, they are treated like transactions. 
	 */
	dCsmNxtStateOnMsgRecd[Sdf_en_unknownRequest][Sdf_en_idle].\
		dNextState = Sdf_en_transactionReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_unknownRequest][Sdf_en_idle].\
		dMsgToBeSent, "200");

#ifdef SIP_RPR
	/* Transitions when an RPR is received */
	dCsmNxtStateOnMsgRecd[Sdf_en_rpr][Sdf_en_idle].\
		dNextState = Sdf_en_rprReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_rpr][Sdf_en_idle].\
		dMsgToBeSent, "PRACK");

	/* Transitions when a PRACK is received */
	dCsmNxtStateOnMsgRecd[Sdf_en_prack][Sdf_en_rprSent].\
		dNextState = Sdf_en_overlapTransactionReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_prack][Sdf_en_rprSent].\
		dMsgToBeSent, "200");
#endif

	/* Transitions when a COMET is received */
	dCsmNxtStateOnMsgRecd[Sdf_en_comet][Sdf_en_idle].\
		dNextState = Sdf_en_overlapTransactionReceived;
	Sdf_mc_strcpy(dCsmNxtStateOnMsgRecd[Sdf_en_comet][Sdf_en_idle].\
		dMsgToBeSent, "200");
	
	
	/* 
	 * Filling in Transitions in the dCsmNxtStateOnMsgSent table.
	 * Unfilled transition table elements (Eg. while sending CANCEL 
	 * to INVITE) mean that there will not be a state transition 
	 * for that event 
	 */

	/* Transitions when INVITE is sent . */
	dCsmNxtStateOnMsgSent[Sdf_en_invite][Sdf_en_idle] = Sdf_en_inviteSent;
	dCsmNxtStateOnMsgSent[Sdf_en_invite][Sdf_en_callEstablished] = \
		Sdf_en_reInviteSent;

	dCsmNxtStateOnMsgSent[Sdf_en_provisionalResp][Sdf_en_inviteReceived] = \
		Sdf_en_provisionalRespSent;

	dCsmNxtStateOnMsgSent[Sdf_en_provisionalResp][Sdf_en_provisionalRespSent]=\
		Sdf_en_provisionalRespSent;
	
	/* Transitions when ACK is sent . */
	dCsmNxtStateOnMsgSent[Sdf_en_ack][Sdf_en_finalSuccessReceived] = \
		Sdf_en_callEstablished;
	dCsmNxtStateOnMsgSent[Sdf_en_ack][Sdf_en_finalFailureReceived] = \
		Sdf_en_idle;

	dCsmNxtStateOnMsgSent[Sdf_en_ack][Sdf_en_reInviteFinalResponseReceived] = \
		Sdf_en_callEstablished;

	/* Transitions when BYE is sent . */
	dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_finalSuccessSent] =Sdf_en_byeSent;
	dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_callEstablished] = Sdf_en_byeSent;
    dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_finalSuccessReceived] = Sdf_en_byeSent;

/*CSR starts 4566600*/
    dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_finalSuccessReceived] = Sdf_en_byeSent;
	dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_reInviteFinalResponseReceived] = Sdf_en_byeSent;
	dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_reInviteprovRespSent] = Sdf_en_byeSent;
	dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_reInviteFinalResponseSent] = Sdf_en_byeSent;
/*CSR ends  4566600*/	/* Treat this as cancel only as it is send on provisional resp recvd */
	/* Don't change the state for this */
	dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_provisionalRespReceived] = \
		Sdf_en_provisionalRespReceived;
	dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_idle] = Sdf_en_byeSent;

	/* Transitions when final success response is sent . */
	dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_inviteReceived] = \
		Sdf_en_finalSuccessSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_provisionalRespSent] = \
		Sdf_en_finalSuccessSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_reInviteprovRespSent] = \
		Sdf_en_reInviteFinalResponseSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_reInviteprovRespSent] = \
		Sdf_en_reInviteFinalResponseSent;

	dCsmNxtStateOnMsgSent[Sdf_en_provisionalResp] \
		[Sdf_en_reInviteReceived] = Sdf_en_reInviteprovRespSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_inviteCancelled] = \
		Sdf_en_finalSuccessSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_reInviteReceived] = \
		Sdf_en_reInviteFinalResponseSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_reInviteCancelled] = \
		Sdf_en_reInviteFinalResponseSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_byeReceived] = \
		Sdf_en_callTerminated;

	dCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_byeReceived] = \
		Sdf_en_callEstablished;

	dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_transactionReceived] = \
		Sdf_en_idle;

	dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_transactionCancel] = \
		Sdf_en_idle;

	dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess]\
		[Sdf_en_overlapTransactionReceived] = Sdf_en_idle;

	/* Transitions when final failure response is sent . */
	dCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_inviteReceived] = \
		Sdf_en_finalFailureSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_provisionalRespSent] = \
		Sdf_en_finalFailureSent;
	
	dCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_reInviteReceived] = \
		Sdf_en_reInviteFinalResponseSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_inviteCancelled] = \
		Sdf_en_finalFailureSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_reInviteCancelled] = \
		Sdf_en_reInviteFinalResponseSent;

	dCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_transactionReceived] = \
		Sdf_en_idle;

	dCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_transactionCancel] = \
		Sdf_en_idle;

	dCsmNxtStateOnMsgSent[Sdf_en_finalFailure]\
		[Sdf_en_overlapTransactionReceived] = Sdf_en_idle;

	/* Transitions when OPTIONS is sent . */
	dCsmNxtStateOnMsgSent[Sdf_en_RegularTxn][Sdf_en_idle] = \
						Sdf_en_transactionSent;

	/* Transitions when REGISTER is sent . */
	dCsmNxtStateOnMsgSent[Sdf_en_register][Sdf_en_idle]=Sdf_en_transactionSent;


	/* 
	 * Transitions when an unknown method is sent.
	 * These are transitions when a method that is supported by 
	 * the application (besides INVITE/BYE and other well-known 
	 * methods) but not by the UA Toolkit is sent. As a default 
	 * implementation, they are treated like transactions.
	 */
	dCsmNxtStateOnMsgSent[Sdf_en_unknownRequest][Sdf_en_idle] = \
		Sdf_en_transactionSent;
#ifdef SIP_RPR
	/* Transitions when RPR is sent . */
	dCsmNxtStateOnMsgSent[Sdf_en_rpr][Sdf_en_idle] = Sdf_en_rprSent;
		
	/* Transitions when PRACK is sent . */
	dCsmNxtStateOnMsgSent[Sdf_en_prack][Sdf_en_rprReceived] = \
		Sdf_en_overlapTransactionSent;
#endif

	/* Transitions when COMET is sent . */
	dCsmNxtStateOnMsgSent[Sdf_en_comet][Sdf_en_idle] = \
		Sdf_en_overlapTransactionSent;

	dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_reInviteprovRespReceived] = \
 		Sdf_en_byeSent;
 	dCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_reInviteSent] = \
 		Sdf_en_byeSent;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_fn_uaInitCsmTable");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUpdateCallConstants
 **
 ** DESCRIPTION: This function updates the From, To, Callid and 
 **				 CallObjectKey of the call object. 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateCallConstants
#ifdef ANSI_PROTO
(Sdf_st_callObject 	*pCallObj,
SipMessage 			*pMessage,
Sdf_st_msgInfo		dMessageInfo,
Sdf_ty_bool			dIsIncMsg,
Sdf_st_error 		*pErr)
#else
(pCallObj, pMessage, dMessageInfo, dIsIncMsg, pErr)
Sdf_st_callObject 	*pCallObj;
SipMessage 			*pMessage;
Sdf_st_msgInfo		dMessageInfo;
Sdf_ty_bool			dIsIncMsg;
Sdf_st_error 		*pErr;
#endif
{
	SipHeader 		dTempHeader;
	en_HeaderType	dHdrType = SipHdrTypeAny;
        /******** Merge For SPR 19294 for forking CSR-5572165 Start**********/
        /* Auth+RPR+Fork Change Start */
        SipHeader dCseqHeader;
        Sdf_ty_u32bit dCseq = 0;
        /* Auth+RPR+Fork Change End */
        /******** Merge For SPR 19294 for forking CSR-5572165 End**********/
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaUpdateCallConstants");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;
	if(pMessage == Sdf_co_null)
		return Sdf_co_fail;
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallConstants( ): "
			"Invalid Param: pCallObject",pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/* 	
	 * Gets the CallId from the SipMessage and sets in the call
	 * object
	 */
 	if (sip_getHeader(pMessage,SipHdrTypeCallId, &dTempHeader, \
		(SipError*)&(pErr->stkErrCode)) != SipFail)
	{
		Sdf_ty_s8bit *pCallId;
		Sdf_st_commonInfo *pCommonInfo;
	 	if(sip_getValueFromCallIdHdr(&dTempHeader,&pCallId,\
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&dTempHeader);	
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallConstants( ): "
				"Failed to get value from call-id header",pErr);
#endif				
			return Sdf_co_fail;
		}

		/* 
		 * Get common info.
		 */
		if (sdf_ivk_uaGetCommonInfoFromCallObject(pCallObj, \
			&pCommonInfo, pErr) == Sdf_co_fail)
		{
			sip_freeSipHeader(&dTempHeader);	
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallConstants( ): "
				"Failed to get common info from the call object", \
				pErr);
#endif				
			return Sdf_co_fail;
		}
		/* 
		 * Update the Callid field 
		 */
		if ( (pCommonInfo->pCallid == Sdf_co_null) || \
			(Sdf_mc_strcmp(pCommonInfo->pCallid, pCallId) != 0) )
		{
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
				&(pCommonInfo->pCallid),pErr);
			Sdf_mc_strdup(pCommonInfo->pCallid, pCallId);	
		}

		/* 
		 * Update the Callid in the CallObjectKey 
		 */
		if ( (pCommonInfo->pKey->pCallId == Sdf_co_null) || \
			(Sdf_mc_strcmp(pCommonInfo->pKey->pCallId, pCallId) != 0) )
		{
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
				&(pCommonInfo->pKey->pCallId),pErr);
			Sdf_mc_strdup(pCommonInfo->pKey->pCallId, pCallId);	
		}

		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
		sip_freeSipHeader(&dTempHeader);	
	}

	/* 
	 * Update the From header in the CallObject common info
	 */
	sip_freeSipHeader(pCallObj->pCommonInfo->pFrom);
	if (sip_getHeader(pMessage, SipHdrTypeFrom, \
		pCallObj->pCommonInfo->pFrom, (SipError*)&(pErr->stkErrCode)) \
		== SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
		(Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallConstants( ): "
			"Failed to get From header from SipMessage",pErr);
#endif			
		return Sdf_co_fail;
	}

	/* 
	 * Update the From header in the CallObjectKey.
	 */
		
	sip_freeSipHeader(pCallObj->pCommonInfo->pKey->pLocalAddr);
	
	if(dMessageInfo.dReqRespType == SipMessageRequest )
	{
		if( dIsIncMsg == Sdf_co_true )
			dHdrType = SipHdrTypeTo;
		else
			dHdrType = SipHdrTypeFrom;
	}
	else
	{
		if( dIsIncMsg == Sdf_co_true )
			dHdrType = SipHdrTypeFrom;
		else
			dHdrType = SipHdrTypeTo;
	}

        /******** Merge For SPR 19294 for forking CSR-5572165 Start**********/     
        
        /* Auth+RPR+Fork Start */
        /* Updating CSeq based on the message received */
        if (sip_getHeader(pMessage, SipHdrTypeCseq,\
            &dCseqHeader, (SipError*)&(pErr->stkErrCode)) == SipFail)
        { 
            pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
                (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallConstants( ): "
                "Failed to get Cseq header",pErr);
#endif		
            return Sdf_co_fail;
        }
        if (sip_getSeqNumFromCseqHdr(&dCseqHeader,&dCseq, \
            (SipError *) &(pErr->stkErrCode)) == SipFail)
        {
            pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
                (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallConstants( ): "
                "Failed to get cseq num from Cseq header",pErr);
#endif		
            sip_freeSipHeader(&dCseqHeader);
            return Sdf_co_fail;
        }
        pCallObj->pCommonInfo->dLocalCseq = dCseq;
        sip_freeSipHeader(&dCseqHeader);
        /*pCallObj->pCommonInfo->dLocalCseq = pMessage->pGeneralHdr->pCseqHdr->dIntVar1;*/
        /* Auth+RPR+Fork End */
        
        /******** Merge For SPR 19294 for forking CSR-5572165 End**********/        

	if (sip_getHeader(pMessage, dHdrType, 			\
			pCallObj->pCommonInfo->pKey->pLocalAddr, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
		(Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallConstants( ): "
			"Failed to get From header from SipMessage",pErr);
#endif			
		return Sdf_co_fail;
	}

	/*
	 * Gets the To Header from the SipMessage. Free the existing To
	 * header before getting the new one.
	 */
	sip_freeSipHeader(pCallObj->pCommonInfo->pTo);

	if (sip_getHeader(pMessage, SipHdrTypeTo, pCallObj->pCommonInfo->pTo, \
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallConstants( ): "
			"Failed to get To header from SipMessage",pErr);
#endif			
		return Sdf_co_fail;
	}

	/* 
	 * Update the To header in the CallObjectKey. Free the existing
	 * header first and then get the new one. 
	 */
	sip_freeSipHeader(pCallObj->pCommonInfo->pKey->pRemoteAddr);

	if(dMessageInfo.dReqRespType == SipMessageRequest)
	{
		if( dIsIncMsg == Sdf_co_true )
			dHdrType = SipHdrTypeFrom;
		else
			dHdrType = SipHdrTypeTo;
	}
	else
	{
		if( dIsIncMsg == Sdf_co_true )
			dHdrType = SipHdrTypeTo;
		else
			dHdrType = SipHdrTypeFrom;
	}

	if (sip_getHeader(pMessage, dHdrType, \
		pCallObj->pCommonInfo->pKey->pRemoteAddr, \
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallConstants( ): "
			"Failed to get To header from SipMessage",pErr);
#endif			
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaUpdateCallConstants");
	return Sdf_co_success;

}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUpdateCallObject
 **
 ** DESCRIPTION: This function fills in the cseq, processed headers list, 
 **				 unprocessedheaders list into the call object from the sip 
 **				 message in the call object.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateCallObject
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	SipMessage *pSipMsg,
	Sdf_st_msgInfo	dMessageInfo,
	Sdf_st_error *pErr)
#else
	(pCallObj,pOverlapTransInfo,pSipMsg, dMessageInfo, pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	SipMessage *pSipMsg;
	Sdf_st_msgInfo	dMessageInfo;
	Sdf_st_error *pErr;
#endif
{


    Sdf_ty_u32bit 			dIndex=0;
    Sdf_ty_u32bit 			dRouteSize = 0;
    en_SipMessageType 		dMessageType;
    Sdf_ty_slist			*pProcessedHeadersList = Sdf_co_null;
    Sdf_ty_slist			*pUnprocessedHeadersList = Sdf_co_null;
    Sdf_ty_slist			slRouteHeaderList;
    Sdf_st_transaction		*pTransaction = Sdf_co_null;
    Sdf_ty_u16bit 			dRespCode=0;
#ifdef SDF_TRACE
    Sdf_ty_traceType		dTraceType;
    Sdf_ty_s8bit 			trace[Sdf_co_traceBufferSize];
#endif
    Sdf_ty_s8bit			*pTmpVal = Sdf_co_null;
    Sdf_ty_bool             dIsInviteConfirmed = Sdf_co_false;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
            "Entering sdf_fn_uaUpdateCallObject");

#ifdef SDF_PARAMVALIDATION
    if(pErr == Sdf_co_null)
        return Sdf_co_fail;

    if(pCallObj == Sdf_co_null)
    {
        pErr->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR		
        sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
                (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                "Invalid param: pCallObject",pErr);
#endif			
        return Sdf_co_fail;
    }
#endif	/* End of param validation ifdef */

    /* 
     * Retrieve the type of the message - request/response 
     */
    dMessageType = dMessageInfo.dReqRespType;
    dRespCode	 = dMessageInfo.dResponse;

    /*
     * If this is a response, then update only the To tag
     * in the call object if it has not been set as yet. 
     * (Since call object key is just a reference to the
     * call object's To header, this will automatically get updated). The
     * From, CallId and other fields have already been set at the time of
     * sending out the request.
     * NOTE : The To tag updation should only be done for a >=2xx response.
     * Only RPR's with a different tag cause formation of different call
     * legs. As we dont support multiple call legs, for now we will update
     * the To tag for any incoming SipMessageResponse.
     */
    if (dMessageType == SipMessageResponse)
    { 
        if((!((dMessageInfo.dMsgType==Sdf_en_cancel)&&(dRespCode==200)))&&\
                (dRespCode>100) )
        {
            Sdf_ty_s8bit *pCallObjectTag = Sdf_co_null; 
            Sdf_ty_s8bit *pSipMsgTag = Sdf_co_null;
            SipHeader dTempHeader;

            /*w534*/ (void)sip_getTagAtIndexFromToHdr(pCallObj->pCommonInfo->pTo, \
                    &pCallObjectTag, 0, (SipError*)&(pErr->stkErrCode));

            if (sip_getHeader(pSipMsg, SipHdrTypeTo, &dTempHeader, \
                        (SipError *)&(pErr->stkErrCode)) == SipFail)
            {
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_minorErrors, \
                        Sdf_en_headerManipulationError, \
                        (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject( ): "
                        "Failed to get To header from SIP message",pErr);
#endif
                pErr->errCode=Sdf_en_headerManipulationError;
                return Sdf_co_fail;
            }

            /*w534*/ (void)sip_getTagAtIndexFromToHdr(&dTempHeader, \
                    &pSipMsgTag, 0, (SipError*)&(pErr->stkErrCode));

            sip_freeSipHeader(&dTempHeader);

            if ((pCallObjectTag == Sdf_co_null) && (pSipMsgTag != Sdf_co_null))
            {
                pTmpVal = 	pSipMsgTag;

                if (sip_insertTagAtIndexInToHdr(pCallObj->pCommonInfo->pTo, \
                            pTmpVal, 0, (SipError *)&(pErr->stkErrCode)) == SipFail)
                {
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_minorErrors, \
                            Sdf_en_headerManipulationError, \
                            (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject( ): "
                            "Failed to insert Tag in Call Object's To header",pErr);
#endif
                    pErr->errCode=Sdf_en_headerManipulationError;
                    return Sdf_co_fail;
                }
            }	
        }
    } 
    else 
    {
        /* 
         * It is a request that has been received. If the From header has
         * a tag, and
         *
         * a) if the original call was from us and the To tag in the call
         *    object is NULL, then we need to update it with the recvd tag
         *
         * b) if the original call was from the other end, and the From tag
         *    in the call object is NULL, then we need to update it with
         *    the recvd tag
         */
        Sdf_ty_retVal	dVerifyCaller = Sdf_co_fail;
        Sdf_ty_s8bit *pSipMsgTag = Sdf_co_null;
        SipHeader dTempHeader;

        if (sip_getHeader(pSipMsg, SipHdrTypeFrom, &dTempHeader, \
                    (SipError *)&(pErr->stkErrCode)) == SipFail)
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_minorErrors, \
                    Sdf_en_headerManipulationError, \
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject( ): "
                    "Failed to get To header from SIP message",pErr);
#endif
            pErr->errCode=Sdf_en_headerManipulationError;
            return Sdf_co_fail;
        }

        /*w534*/ (void)sip_getTagAtIndexFromFromHdr(&dTempHeader, \
                &pSipMsgTag, 0, (SipError*)&(pErr->stkErrCode));

        /* Free the local reference */
        sip_freeSipHeader(&dTempHeader);

        if (pSipMsgTag != Sdf_co_null)
        {
            Sdf_ty_s8bit *pCallObjectTag = Sdf_co_null; 

            dVerifyCaller = sdf_fn_uaVerifyCaller(pCallObj, pErr);

            if (dVerifyCaller == Sdf_co_success)
            {
                /*w534*/ (void)sip_getTagAtIndexFromToHdr(pCallObj->pCommonInfo->pTo, \
                        &pCallObjectTag, 0, (SipError*)&(pErr->stkErrCode));
            }
            else
            {
                /*w534*/ (void)sip_getTagAtIndexFromFromHdr(pCallObj->pCommonInfo->pFrom, \
                        &pCallObjectTag, 0, (SipError*)&(pErr->stkErrCode));
            }

            if (pCallObjectTag == Sdf_co_null)
            {
                pTmpVal = Sdf_co_null;
                pTmpVal = 	pSipMsgTag;
                if (dVerifyCaller == Sdf_co_success)
                {
                    if (sip_insertTagAtIndexInToHdr(pCallObj->pCommonInfo->pTo,\
                                pTmpVal,0, (SipError *)&(pErr->stkErrCode)) == SipFail)
                    {
#ifdef SDF_ERROR
                        sdf_fn_setError(Sdf_en_minorErrors, \
                                Sdf_en_headerManipulationError, \
                                (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject( ): "
                                "Failed to insert Tag in Call Object's To info", \
                                pErr);
#endif
                        pErr->errCode=Sdf_en_headerManipulationError;
                        return Sdf_co_fail;
                    }
                }
                else
                {
                    if (sip_insertTagAtIndexInFromHdr( \
                                pCallObj->pCommonInfo->pFrom, pTmpVal, 0, \
                                (SipError *)&(pErr->stkErrCode)) == SipFail)
                    {
#ifdef SDF_ERROR
                        sdf_fn_setError(Sdf_en_minorErrors, \
                                Sdf_en_headerManipulationError, \
                                (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject( ): "
                                "Failed to insert Tag in Call Object's From info",\
                                pErr);
#endif
                        pErr->errCode=Sdf_en_headerManipulationError;
                        return Sdf_co_fail;
                    }
                }
            }
        }
    } /* End of tag updation */

    /*
     * Update remote party's sequence numbers in the Callobject's
     * pCommonInfo structure. Sequence numbers to be updated are
     * a) dRemoteCseq - The highest Cseq received so far.
     * b) dRemoteRseq - The highest (Rseq, Cseq) received so far.
     * c) dRemoteRegCseq - The highest Cseq PRACK received so far.
     */
    if (sdf_fn_uaUpdateSeqNumbersInCallObj(pCallObj, pOverlapTransInfo, \
                pSipMsg, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR		
        sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                "Failed to update cseq for the call",pErr);
#endif			
        return Sdf_co_fail;
    }

    /* 
     * Updating the ProcessedHeaderlist begins here. First check if
     * the processed headers list of the overlap transinfo has to be
     * used or that of the call object is to be updated. Get count of
     * the appropriate list.
     */
    if (pOverlapTransInfo == Sdf_co_null)
    {
        if (dMessageType == SipMessageRequest)
        {
            Sdf_ty_s8bit	*pMsgMethod = Sdf_co_null;
            (void)sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, \
                    &pMsgMethod, pErr);
            if( Sdf_mc_strcmp(pMsgMethod, "INVITE") == 0)
            {
                sip_freeSipTxnKey(pCallObj->pUasTransaction->pInvTxnKey);
                pCallObj->pUasTransaction->pInvTxnKey = Sdf_co_null;
                pCallObj->pUasTransaction->pInvTxnKey=pCallObj->\
                                                      pUasTransaction->pTxnKey;
                if(pCallObj->pUasTransaction->pInvTxnKey != Sdf_co_null)
                    HSS_LOCKEDINCREF(pCallObj->pUasTransaction->pTxnKey->dRefCount);
            }

            if (sdf_ivk_uaGetTransactionFromCallObject(pCallObj, \
                        Sdf_en_uasTransaction, &pTransaction, pErr) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_minorErrors, \
                        Sdf_en_callObjectAccessError,\
                        (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                        "Failed to get UAS transaction object from call object", \
                        pErr);
#endif			
                pErr->errCode=Sdf_en_callObjectAccessError;
                return Sdf_co_fail;
            }
        }
        else
        {
            if (sdf_ivk_uaGetTransactionFromCallObject(pCallObj, \
                        Sdf_en_uacTransaction, &pTransaction, pErr) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_minorErrors, \
                        Sdf_en_callObjectAccessError,\
                        (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                        "Failed to get UAC transaction object from call object", \
                        pErr);
#endif			
                pErr->errCode=Sdf_en_callObjectAccessError;
                return Sdf_co_fail;
            }
        }
        pProcessedHeadersList = &(pTransaction->slProcessedHeaders);
        pUnprocessedHeadersList = &(pTransaction->slUnprocessedHeaders);
    }
    else
    {
        pProcessedHeadersList = &(pOverlapTransInfo->slProcessedHeaders);
        pUnprocessedHeadersList = &(pOverlapTransInfo->slUnprocessedHeaders);
    }

    /* 	
     * Delete all the ProcessedHeaders
     */
    (void)sdf_listDeleteAll(pProcessedHeadersList,pErr);
#ifdef SDF_LOOKUP
    /* Initialize the DNS state to idle for the Lookup of the UAS transacion*/
    if (dMessageType == SipMessageRequest)
    {
        if (pOverlapTransInfo == Sdf_co_null)
        {
#ifdef SDF_LINT
            if (pTransaction == Sdf_co_null)
                return Sdf_co_fail;
#endif
#ifdef SDF_THREAD_SAFE		
            sdf_fn_uaLockMutex(0,  &(pTransaction->dTxnDnsStateMutex), 0);  	
#endif
            pTransaction->dTxnDnsState = Sdf_en_Idle;
#ifdef SDF_THREAD_SAFE
            sdf_fn_uaUnlockMutex(0,  &(pTransaction->dTxnDnsStateMutex));	
#endif
        }
    }
#endif
    if (dMessageType == SipMessageRequest)
    {
        /* ----------------------
         * VIA HEADER(S) HANDLING
         * ----------------------
         * If any via headers exist in the SipMessage add them into the
         * ProcessedHeaderlist 
         */
        if (sdf_fn_uaAddHdrListToProcessedHdrsInCallObj(pTransaction, \
                    pOverlapTransInfo, pSipMsg, SipHdrTypeVia, pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                    "Failed to add Via header to list of processed headers", pErr);
#endif	
            (void)sdf_ivk_uaFreeTransaction(pTransaction);
            return Sdf_co_fail;
        }

        /* -------------------------------
         * RECORD ROUTE HEADER(S) HANDLING 
         * -------------------------------
         * If any record route headers exist in the request message, 
         * copy them into the processedheader list. This is because
         * they must be sent out in the next response.
         */
        if (sdf_fn_uaAddHdrListToProcessedHdrsInCallObj(pTransaction, \
                    pOverlapTransInfo, pSipMsg, SipHdrTypeRecordRoute,pErr) ==	\
                Sdf_co_fail)
        {
#ifdef SDF_ERROR			
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                    "Failed to construct route set",pErr);
#endif
            (void)sdf_ivk_uaFreeTransaction(pTransaction);
            return Sdf_co_fail;
        }

#ifdef SIP_TIMESTAMP
        /* -------------------------
         * TIMESTAMP HEADER HANDLING
         * -------------------------
         * If any timestamp Headers exist in the SipMessage add them into the
         * ProcessedHeaderlist 
         */
        if (sdf_fn_uaAddHdrListToProcessedHdrsInCallObj(pTransaction, \
                    pOverlapTransInfo, pSipMsg, SipHdrTypeTimestamp, pErr) ==	\
                Sdf_co_fail)
        {
#ifdef SDF_ERROR			
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                    "Failed to add Timestamp header to list of processed "
                    "headers", pErr);
#endif				
            (void)sdf_ivk_uaFreeTransaction(pTransaction);
            return Sdf_co_fail;
        }
#endif /* SIP_TIMESTAMP */

#ifdef SDF_AUTHENTICATE
        /* ----------------------
         * AUTHORIZATION HANDLING 
         * ----------------------
         * If any authorization headers exist in the request message, 
         * copy them into the processedheader list 
         */

        if (sdf_fn_uaAddAuthHdrListToCallObj(pTransaction, pOverlapTransInfo, \
                    pSipMsg, pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR			
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                    "Failed to add authorization header to list of "
                    "processed headers", pErr);
#endif				
            (void)sdf_ivk_uaFreeTransaction(pTransaction);
            return Sdf_co_fail;
        }
#endif /* SDF_AUTHENTICATE */
#ifdef SDF_3GPP
        /* -------------------------
         * History-Info HEADER HANDLING
         * -------------------------
         * If any History-Info Headers exist in the SipMessage add them into the
         * ProcessedHeaderlist 
         */
        if (sdf_fn_uaAddHdrListToProcessedHdrsInCallObj(pTransaction, \
                    pOverlapTransInfo, pSipMsg, SipHdrTypeHistoryInfo, pErr) ==	\
                Sdf_co_fail)
        {
#ifdef SDF_ERROR			
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                    "Failed to add History-Info header to list of processed "
                    "headers", pErr);
#endif				
            (void)sdf_ivk_uaFreeTransaction(pTransaction);
            return Sdf_co_fail;
        }
#endif /* SDF_3GPP */

#ifndef SDF_UASSF
        /* 
         * Setting the variable dSdpInInvite. This is necessary
         * to keep track of whether the INVITE contained SDP or
         * whether it is expected in the ACK. The function does 
         * nothing to the variable if the message is not an INVITE
         */
        if (sdf_fn_uaCheckSdpInInvite(pCallObj, dMessageInfo, pSipMsg, \
                    pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR			
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                    "Failed to check for presence of SDP in the message",\
                    pErr);
#endif				
            (void)sdf_ivk_uaFreeTransaction(pTransaction);
            return Sdf_co_fail;
        }
#endif /* SDF_UASSF */

    } /* End of the If SipMessageRequest Statement */
#ifdef SDF_AUTHENTICATE
    else
    {

        /* --------------------------------------------
         * WWW-AUTHENTICATE/PROXY-AUTHENTICATE HANDLING
         * --------------------------------------------
         * If the response code is 401 or 407 and if the Scheme 
         * used for authentication is Basic/Digest, the above 
         * header is put into the ProcessedHeaderlist. Else it 
         * is put into the UnprocessedHeaders list 
         */

        if ( (dRespCode == 401) || (dRespCode == 407) )
        {
            if (sdf_fn_uaAddAuthenticateHdrListToCallObj(pTransaction,\
                        pOverlapTransInfo, pSipMsg, SipHdrTypeWwwAuthenticate, pErr) \
                    == Sdf_co_fail)
            {
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                        (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                        "Failed to add Www-Auth header to ProcessedHeaders",\
                        pErr);
#endif
                (void)sdf_ivk_uaFreeTransaction(pTransaction);
                return Sdf_co_fail;
            }
            if (sdf_fn_uaAddAuthenticateHdrListToCallObj(pTransaction,\
                        pOverlapTransInfo, pSipMsg, SipHdrTypeProxyAuthenticate, pErr) \
                    == Sdf_co_fail)
            {
#ifdef SDF_ERROR				
                sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                        (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                        "Failed to add Www-Auth header to ProcessedHeaders",\
                        pErr);
#endif					
                (void)sdf_ivk_uaFreeTransaction(pTransaction);
                return Sdf_co_fail;
            }
        }
    } /* End of If SipMessageResponse condition */
#endif /* SDF_AUTHENTICATE */	

    /* Free the local reference */
    (void)sdf_ivk_uaFreeTransaction(pTransaction);


    /* ----------------------
     * ROUTE HEADERS HANDLING
     * ----------------------
     * If Route Headers are still not part of the ProcessedHeaders,
     * add it if record route headers exist in the SipMessage. 
     * Note that if pOverlapTransInfo is not null, then	the Route 
     * Headers should be created for this particular overlap
     * transcation. 
     */
    if (sdf_ivk_uaGetHeaderListByCorrelation(&(pCallObj->slPersistentHeaders), \
                (Sdf_ty_s8bit *)"uat-routeset", &slRouteHeaderList,\
                pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
                (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                "Failed to get Route header list by correlation uat-routeset",pErr);
#endif				
        return Sdf_co_fail;
    }
    (void)sdf_listSizeOf(&slRouteHeaderList, &dRouteSize, pErr);
    (void)sdf_listDeleteAll(&slRouteHeaderList,pErr);

    /* Route set should be re-computed for a 2xx response for the INVITE */
    if (( dRespCode == 200 ) && (dMessageInfo.dMsgType == Sdf_en_invite)  &&
            ( pCallObj->pCallInfo->dState  == Sdf_en_finalSuccessReceived ))
    {
        dIsInviteConfirmed = Sdf_co_true;
        (void)sdf_ivk_uaDeleteHeaderListByCorrelation( \
                &(pCallObj->slPersistentHeaders), "uat-routeset",pErr);

        pCallObj->pCommonInfo->dPreloadedSet = Sdf_co_false;
    }

    if (dRouteSize == 0) 
    {
        if (sdf_fn_uaCreateRoutePathInCallObj(pCallObj, dMessageInfo, pSipMsg, \
                    pErr)== Sdf_co_fail)
        {
#ifdef SDF_ERROR			
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                    "Failed to add route headers to ProcessedHeaders",pErr);
#endif				
            return Sdf_co_fail;
        }
    } /* End of the Route If Statement */
    else
    {
        /* 
         * Route Headers already exist in the PersistenHeaders list.
         * Update only the contact header if required 
         */
        if(dIsInviteConfirmed == Sdf_co_false)
        {
            if (sdf_fn_uaUpdateRoutePathInCallObj(pCallObj, dMessageInfo, pSipMsg, \
                        pErr)== Sdf_co_fail)
            {
#ifdef SDF_ERROR			
                sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                        (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                        "Failed to add route headers to ProcessedHeaders",pErr);
#endif				
                return Sdf_co_fail;
            }

        }
        else 
        {
            if (sdf_fn_uaCreateRoutePathInCallObj(pCallObj, dMessageInfo, pSipMsg, \
                        pErr)== Sdf_co_fail)
            {
#ifdef SDF_ERROR			
                sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                        (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                        "Failed to add route headers to ProcessedHeaders",pErr);
#endif				
                return Sdf_co_fail;
            }
        }
    }
    /* 
     * Check if there are any contact headers in the sipmessage	received. 
     * If so, then, delete the entire list of ContactHeaders in the 
     * CallObject and replace them with the contact headers in the sip 
     * message received 
     */
    if (sdf_fn_uaUpdateContactListInCallObj(pCallObj, pSipMsg, pErr) \
            == Sdf_co_fail)
    {
#ifdef SDF_ERROR		
        sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                "Failed to update the Contact list in the call object", pErr);
#endif			
        return Sdf_co_fail;
    }

#ifdef SDF_TRACE
    if ((sdf_ivk_uaGetTraceType(&dTraceType, pErr)) == Sdf_co_fail)
    {
        return Sdf_co_fail;
    }
#endif	




    /* 
     * The rest of the headers existent in the SipMessage are inserted 
     * into the UnprocessedHeaders list in the call object
     * Assuming that the enumeration SipHdrType starts from zero.
     */
    for(dIndex=0; dIndex<SipHdrTypeAny; dIndex++)
    {
        Sdf_st_headerList 	*pHeader;
        Sdf_ty_u32bit		dCount=0, dLIndex=0;

        if ((dIndex == SipHdrTypeContactNormal) || \
                (dIndex == SipHdrTypeContactWildCard) || \
                (dIndex == SipHdrTypeContactAny) || \
                (dIndex == SipHdrTypeRoute) || \
                (dIndex == SipHdrTypeRecordRoute) || \
                (dIndex == SipHdrTypeVia) || \
                (dIndex == SipHdrTypeFrom)|| \
                (dIndex == SipHdrTypeTo) || \
                (dIndex == SipHdrTypeCseq) || \
                (dIndex == SipHdrTypeCallId) || \
                (dIndex == SipHdrTypeContentType) || \
                (dIndex == SipHdrTypeContentLength) || \
                (dIndex == SipHdrTypeMaxforwards) || \
                (dIndex == SipHdrTypeAllow) || \
                (dIndex == SipHdrTypeSupported))
            continue;
#ifdef SDF_AUTHENTICATE			
        if ((dIndex == SipHdrTypeProxyAuthenticate) || \
                (dIndex == SipHdrTypeWwwAuthenticate) || \
                (dIndex == SipHdrTypeAuthorization))
            continue;	
#endif			
#ifdef SDF_SESSION_TIMER
        if((dIndex==SipHdrTypeMinSE) ||\
                (dIndex==SipHdrTypeSessionExpires))
            continue;
#endif

        /*w534*/ (void)sip_getHeaderCount(pSipMsg,(en_HeaderType)dIndex,\
                &dCount, (SipError *)&(pErr->stkErrCode));

        if (dCount == 0)
            continue;

        if (sdf_ivk_uaInitHeaderList(&pHeader, \
                    (en_HeaderType)dIndex, Sdf_co_null, pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR			
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                    "Failed to init the header list structure",pErr);
#endif				
            return Sdf_co_fail;
        }	
        for (dLIndex = 0; dLIndex < dCount; dLIndex++)
        {
            SipHeader *pDestHeader = Sdf_co_null;

            if (sip_initSipHeader(&pDestHeader, SipHdrTypeAny,  \
                        (SipError *)&(pErr->stkErrCode)) == SipFail)
            {
                pErr->errCode = pErr->stkErrCode;
                (void)sdf_ivk_uaFreeHeaderList(pHeader);
#ifdef SDF_ERROR				
                sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                        (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                        "Failed to init a SipHeader structure",pErr);
#endif					
                return Sdf_co_fail;
            }

            pDestHeader->dType = (en_HeaderType)dIndex;

            if (sip_getHeaderAtIndex(pSipMsg, \
                        (en_HeaderType)dIndex, pDestHeader, dLIndex, \
                        (SipError *)&(pErr->stkErrCode)) == SipFail)
            {
                (void)sdf_ivk_uaFreeHeaderList(pHeader);
                pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR				
                sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                        (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                        "Failed to get header from the received message", \
                        pErr);
#endif					
                /* klockwork fix */
                sip_freeSipHeader(pDestHeader);
                (void)sdf_memfree(Sdf_mc_callHandlerMemId, \
                        (Sdf_ty_pvoid *)&pDestHeader, pErr);
                return Sdf_co_fail;
            }

#ifdef SDF_TRACE
            /* 	
             *  Code block between ifdef to endif only for unknown 
             *  header traces 
             */
            if (dTraceType&Sdf_en_unknownHeaderTraces)
            {
                Sdf_ty_s8bit *pTempHeaderString = Sdf_co_null;

                /*w534*/ (void)sip_getHeaderAsStringAtIndex(pSipMsg, \
                        (en_HeaderType)dIndex, &pTempHeaderString, dLIndex, \
                        (SipError *)&(pErr->stkErrCode)); 
                if (Sdf_co_null != pTempHeaderString)
                {
				    Sdf_ty_u32bit dFixedLen = 
						Sdf_mc_strlen("Uknown header passed to toolkit is:");	
					
                    Sdf_mc_strcpy(trace,"Uknown header passed to toolkit is:");
                    Sdf_mc_strncat(trace, pTempHeaderString,
							(Sdf_co_traceBufferSize - dFixedLen - 1));

                    sdf_fn_trace(Sdf_en_allTraceLevels, \
                            Sdf_en_unknownHeaderTraces, trace, 0, pErr);
                    (void)sdf_memfree(Sdf_mc_callHandlerMemId, \
                            (Sdf_ty_pvoid *)&pTempHeaderString, pErr);
                }
            }
#endif

            /*
             * Insert the cloned header into the list of headers.
             */
            if (sdf_listAppend(&(pHeader->slHeaders), \
                        pDestHeader,pErr) == Sdf_co_fail)
            {
                sip_freeSipHeader(pDestHeader);
                (void)sdf_ivk_uaFreeHeaderList(pHeader);
#ifdef SDF_ERROR				
                sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                        (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                        "Failed to insert a SipHeader into the header list "
                        "structure", pErr);
#endif					
                return Sdf_co_fail;
            }
        }

        /* 
         * Append the header list structure to the list of unprocessed 
         * headers.
         */
        if (sdf_listAppend(pUnprocessedHeadersList, pHeader, pErr) == \
                Sdf_co_fail)
        {
            (void)sdf_ivk_uaFreeHeaderList(pHeader);
#ifdef SDF_ERROR				
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateCallObject(): "
                    "Failed to add a header list structure to the "
                    "UnprocessedHeaders list", pErr);
#endif					
            return Sdf_co_fail;
        }	
    }

    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
            "sdf_fn_uaUpdateCallObject");
    return Sdf_co_success;
}

#ifndef SDF_TXN
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCheckRetransmission
 **
 ** DESCRIPTION: This function checks if the received message is actually a 
 **				 retransmission. If so, it sends back the response that it 
 **				 sent to this request the first time.
 **
 ******************************************************************************/
Sdf_ty_matchMessageType sdf_fn_uaCheckRetransmission
#ifdef ANSI_PROTO
	(SipMessage *pMsg, Sdf_st_eventContext *pContext, Sdf_st_error *pErr)
#else
	(pMsg, pContext, pErr)
	SipMessage *pMsg;
	Sdf_st_eventContext *pContext;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_hash				*pRemoteRetransHash;
	Sdf_st_remoteRetransKey *pNewKey = Sdf_co_null;
#ifndef SDF_LINT
	SipOptions options;
#endif
	SipTranspAddr dTranspaddr;
	Sdf_st_remoteRetransBuffer *pTempRetransBuffer = Sdf_co_null;
#ifdef SDF_TRACE
	Sdf_ty_s8bit trace[Sdf_co_traceBufferSize];
#endif	
	(void)pContext;


	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaCheckRetransmission \n");	

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr == Sdf_co_null)
		return Sdf_en_fail;
	if (pMsg== Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckRetransmission( ): "
			"Invalid Param - Input Message is NULL", pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}	
#endif	/* End of param validation ifdef */

#ifdef SDF_APP_CONTROLLED_REMOTE_RETX
	if (sdf_cbk_uaGetRemoteRetransHashTable(&pRemoteRetransHash, pContext, \
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callBackError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckRetransmission(): "
			"Callback sdf_cbk_uaGetRemoteRetransHashTable failed",pErr);
#endif
		pErr->errCode=Sdf_en_callBackError;
		return Sdf_en_fail;
	}

	if (pRemoteRetransHash == Sdf_co_null)
	{
		/* App intends to take complete control of the remote retransmission
		 * logic and it does not require any support from UATK. So we return
		 * without checking here. The calling function would proceed as though
		 * it were a fresh message for which no remote retrans entry was present
		 */
		return Sdf_en_success;
	}

#else
	pRemoteRetransHash = &(pGlbToolkitData->dRemoteRetransHash);
#endif

	/*
	 * To check for retransmissions, generate the retransmission 
	 * key from the sip message within the new call object received.
	 */
	if(sdf_ivk_uaInitRemoteRetransKey(&pNewKey, pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckRetransmission( ): "
			"Failed to initialize retransmisson key",pErr);
#endif			
		return Sdf_en_fail;
	}
	if(sdf_fn_uaGenerateRetransKey(pMsg, pNewKey, \
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckRetransmission( ): "
			"Failed to generate retransmisson key",pErr);
#endif			
		return Sdf_en_fail;
	}

	/*
	 * Do a HashFetch here and see if it returns NULL. If it returns 
	 * a non-NULL value, it is a remote retransmission. Handle it here.
	 */
	pTempRetransBuffer =(Sdf_st_remoteRetransBuffer *)sdf_ivk_uaHashFetch(\
		pRemoteRetransHash, (Sdf_ty_pvoid)pNewKey);
	 
	/* 
	 * Found the matching Response for the request. Send this to the
	 * remote end.
	 */
	if (pTempRetransBuffer != Sdf_co_null)
	{
		Sdf_ty_s8bit *pBuffer;

#ifdef SDF_USE_PREALLOCBUFFERS
		pBuffer = pTempRetransBuffer->pContext->pDirectBuffer;
#else
		pBuffer = pTempRetransBuffer->pBuffer;
#endif

		dTranspaddr.pHost = Sdf_mc_strdupCallHandler(\
				                 pTempRetransBuffer->pDestInfo->pIp);
		dTranspaddr.dPort = pTempRetransBuffer->pDestInfo->dPort;
		dTranspaddr.dSockFd = pTempRetransBuffer-> dSockfd;

		/* 
		 * Increment the reference count of the AppData structure
		 */
		HSS_LOCKEDINCREF(pTempRetransBuffer->pAppData->dRefCount);
		dTranspaddr.pData = pTempRetransBuffer->pAppData;	

#ifndef SDF_LINT
		/*w550*/ options.dOption = SIP_NORETRANS | SIP_OPT_CLEN | SIP_OPT_FULLFORM ;
#endif

		if( pTempRetransBuffer->pDestInfo->dScheme == Sdf_en_protoUdp)
		{
#ifdef SDF_TRACE						
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize, "Call: %s"
				"- Retransmitted SIP Message (UDP) - Handled by Remote"
				"Retransmission Manager\n",pNewKey->pCallId);

			sdf_fn_trace(Sdf_en_allTraceLevels, \
					Sdf_en_remoteRetransTraces, trace, 0, pErr);

			sdf_fn_trace(Sdf_en_detailedTraces, \
				Sdf_en_remoteRetransTraces,\
				pBuffer, pTempRetransBuffer->dLength, pErr);
#endif						
			if (sip_sendToNetwork( pBuffer, pTempRetransBuffer->dLength, \
				&dTranspaddr, SIP_UDP, \
				(SipError*)&(pErr->stkErrCode)) ==SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
				(void)sdf_ivk_uaFreeAppData((Sdf_st_appData *)dTranspaddr.pData);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCheckRetransmission( ): "
					"Failed to Send to Network over UDP", pErr);
#endif
				(void)sdf_memfree(Sdf_mc_appMemId, \
					(Sdf_ty_pvoid *)&dTranspaddr.pHost, pErr);
				return Sdf_en_fail;
			}
		}
		else
		{
#ifdef SDF_TRACE						
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,"Call: %s"
				"- Retransmitted SIP Message (TCP) - Handled by Remote"
				"Retransmission Manager\n",	pNewKey->pCallId);

			sdf_fn_trace(Sdf_en_allTraceLevels, \
					Sdf_en_remoteRetransTraces, trace, 0, pErr);

			sdf_fn_trace(Sdf_en_detailedTraces, \
				Sdf_en_remoteRetransTraces,\
				pBuffer, \
				pTempRetransBuffer->dLength, pErr);
#endif						

			if(sip_sendToNetwork( pBuffer, pTempRetransBuffer->dLength, \
				&dTranspaddr, SIP_TCP, \
				(SipError*)&(pErr->stkErrCode)) ==SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
				(void)sdf_ivk_uaFreeAppData((Sdf_st_appData *)dTranspaddr.pData);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCheckRetransmission( ): "
					"Failed to Send to Network over TCP", pErr);
#endif
				(void)sdf_memfree(Sdf_mc_appMemId, \
					(Sdf_ty_pvoid *)&dTranspaddr.pHost, pErr);
				return Sdf_en_fail;
			}
		}
		/* 
		 * This will decrement the reference count of the AppData
		 * structure.
		 */
		(void)sdf_ivk_uaFreeAppData((Sdf_st_appData *)dTranspaddr.pData);

		/* Free the memory allocated for pHost. */
		(void)sdf_memfree(Sdf_mc_appMemId, \
					(Sdf_ty_pvoid *)&dTranspaddr.pHost, pErr);

		/*w534*/ (void)sdf_ivk_uaHashRelease(pRemoteRetransHash, (Sdf_ty_pvoid)pNewKey);

		(void)sdf_fn_uaFreeRemoteRetransKey(pNewKey);
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Exiting sdf_fn_uaCheckRetransmission");
		return Sdf_en_remoteRetransmission;
	}

	/* 
	 * If we reach here, there is no entry for this request in the
	 * retrans buffer of this call object
	 */
	if (pNewKey != Sdf_co_null)
	{
		(void)sdf_fn_uaFreeRemoteRetransKey(pNewKey);
	}
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - \
	Exiting sdf_fn_uaCheckRetransmission");
	return Sdf_en_success;
}
#endif

#ifdef SDF_USE_INTERNALTHREADS
/******************************************************************************
 ** FUNCTION: sdf_fn_uaRemoteRetransManager
 **
 ** DESCRIPTION: This function spools the global remote retransmission list 
 **				 and deletes entries that have expired (after configured time
 **				 specifed in the macro: Sdf_mc_remoteRetransExpiryTime)
 **
 ******************************************************************************/
Sdf_ty_fnreturn sdf_fn_uaRemoteRetransManager
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid ptr)
#else
	(ptr)
	Sdf_ty_pvoid ptr;
#endif
{
	/* 
	 * Allow for this thread to be cancelled asynchronously at any time.
	 */
	Sdf_st_error dError;
	/*w534*/ (void)sdf_fn_uaSetThreadCancelState(Sdf_mc_uaCancelThread, Sdf_co_null);

	for(;;)
	{
		/* Sleep for the spool time */

		sdf_fn_uaSleep(Sdf_mc_remoteRetransSpoolTime,0);

		/* 
		 * Spool through remoteRetrans list and purge expired entries
		 */
		/*w534*/ (void)sdf_ivk_uaPurgeExpiredRemoteRetransEntries(Sdf_en_remoteRetransTimer,\
			Sdf_co_null,&dError);

	} /* End of for loop */
#ifndef SDF_LINT
	return (Sdf_ty_fnreturn)ptr;
#endif    
}
#endif


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFormToHeader
 **
 ** DESCRIPTION: This function forms the To header with the informtion in 
 **				 the Call Object.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormToHeader
#ifdef ANSI_PROTO 
	(Sdf_st_callObject *pObject, SipMessage *pMessage, Sdf_st_error *pError)
#else
	(pObject, pMessage, pError)
	Sdf_st_callObject *pObject;
	SipMessage *pMessage;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_s8bit *pTag = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaFormToHeader");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks.
	 */
	if(pError == Sdf_co_null)
		return Sdf_co_fail;
	if(pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormToHeader( ): "
			"Invalid param: SipMessage", pError);
#endif			
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormToHeader( ): "
			"Invalid param: Call Object", pError);
#endif			
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */


	/* 
	 * If a response is being formed, add a tag (if absent) in the
	 * To header.
	 */
	if (pMessage->dType == SipMessageResponse)
	{
		/*w534*/ (void)sip_getTagAtIndexFromToHdr(pObject->pCommonInfo->pTo, &pTag, \
			0, (SipError*)&(pError->stkErrCode));

		if (pTag == Sdf_co_null)
		{
			Sdf_st_initData *pInitData;
			Sdf_ty_s8bit	*pVarToTag = Sdf_co_null;

			if (sdf_ivk_uaGetInitDataFromCallObject(pObject, &pInitData, \
				pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_callObjectAccessError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToHeader( ): "
					"Failed to get user profile from call object",pError);
#endif					
				pError->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
/*SPR-4627 : #ifndef SDF_UAB2B_2543 flag is added to give backup
             * compatibility for 2543 to B2B. In 2543 there is no concept of
             * TOtags and From tag */

#ifndef SDF_UAB2B_2543

			if (sdf_fn_uaGenerateGlobalTag( pInitData->pTag, &pVarToTag) \
					== Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_tagGenerationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToHeader( ): "
					"Failed to generate variable portion of the tag",pError);
#endif					
				pError->errCode=Sdf_en_tagGenerationError;
				(void)sdf_ivk_uaFreeInitData(pInitData);
				return Sdf_co_fail;
			}

			if (sip_insertTagAtIndexInToHdr(pObject->pCommonInfo->pTo, \
				pVarToTag, 0, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToHeader( ): "
					"Failed to insert Tag in To header",pError);
#endif					
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)(&pVarToTag), pError);
				(void)sdf_ivk_uaFreeInitData(pInitData);
				return Sdf_co_fail;
			}
 #endif      /*spr-4627 end giving backup compatibility for rfc 2543 to B2B*/

			/* Free the local reference */
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)(&pVarToTag), pError);
			(void)sdf_ivk_uaFreeInitData(pInitData);
		}	
	}
	
	/*
	 * Set the To header in the sip message structure.
	 */
	if (sip_setHeader(pMessage, pObject->pCommonInfo->pTo, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormToHeader( ): "
			"Failed to set To header in Sip Message", pError);
#endif			
		return Sdf_co_fail;
	}

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_fn_uaFormToHeader");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFormFromHeader
 **
 ** DESCRIPTION: This function forms the From header with the information 
 **				 in the Call Object 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormFromHeader
#ifdef ANSI_PROTO 
	(Sdf_st_callObject *pObject, SipMessage *pMessage, Sdf_st_error *pError)
#else
	(pObject, pMessage, pError)
	Sdf_st_callObject *pObject;
	SipMessage *pMessage;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_s8bit *pTag = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaFormFromHeader");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pError == Sdf_co_null)
		return Sdf_co_fail;
	if(pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormFromHeader( ): "
			"Invalid param: Sip Message",pError);
#endif			
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormFromHeader( ): "
			"Invalid param: Call Object",pError);
#endif			
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/* 
	 * If a request is being formed, add a tag (if absent) in the From header 
	 */
	if (pMessage->dType == SipMessageRequest)
	{
		/*w534*/ (void)sip_getTagAtIndexFromFromHdr(pObject->pCommonInfo->pFrom, &pTag, \
			0, (SipError*)&(pError->stkErrCode));

		if (pTag == Sdf_co_null)
		{
			Sdf_st_initData *pInitData;
			Sdf_ty_s8bit	*pVarFromTag = Sdf_co_null;

			if (sdf_ivk_uaGetInitDataFromCallObject(pObject, &pInitData, \
				pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_callObjectAccessError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormFromHeader( ): "
					"Failed to get user profile from call object",pError);
#endif					
				pError->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
 /*spr-4627 :#ifndef SDF_UAB2B_2543 flag is added to give backup
             * compatibility for 2543 to B2B. In 2543 there is no concept of
             * TOtags and From tag */

#ifndef SDF_UAB2B_2543
        if (sdf_fn_uaGenerateGlobalTag( pInitData->pTag, &pVarFromTag) \
					== Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_tagGenerationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormFromHeader( ): "
					"Failed to generate variable portion of the tag",pError);
#endif					
				pError->errCode=Sdf_en_tagGenerationError;
				(void)sdf_ivk_uaFreeInitData(pInitData);
				return Sdf_co_fail;
			}

			if (sip_insertTagAtIndexInFromHdr(pObject->pCommonInfo->pFrom, \
				pVarFromTag, 0, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaFormFromHeader( ): "
					"Failed to insert Tag in From header",pError);
#endif					
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)(&pVarFromTag), pError);
				(void)sdf_ivk_uaFreeInitData(pInitData);
				return Sdf_co_fail;
			}
#endif /*spr-4627 end giving backup compatibility for rfc 2543 to B2B*/            
			/* Free the local reference */
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)(&pVarFromTag), pError);
			(void)sdf_ivk_uaFreeInitData(pInitData);
		}
	}

	/*
	 * Set the From header into the SipMessage being formed.
	 */
	if (sip_setHeader(pMessage, pObject->pCommonInfo->pFrom, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormFromHeader( ): "
			"Failed to set from header in Sip Message",pError);
#endif			
		return Sdf_co_fail;
	}	

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaFormFromHeader");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFormCallIdHeader
 **
 ** DESCRIPTION: This function forms the CallID header with the information 
 **				 in the Call Object 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormCallIdHeader
#ifdef ANSI_PROTO 
	(Sdf_st_callObject *pObject, SipMessage *pMessage, Sdf_st_error *pError)
#else
	(pObject, pMessage, pError)
	Sdf_st_callObject *pObject;
	SipMessage *pMessage;
	Sdf_st_error *pError;
#endif
{
	SipHeader *pHeader = Sdf_co_null;
	Sdf_st_commonInfo *pCommonInfo = Sdf_co_null;
	Sdf_ty_s8bit	  *pTmpVal = Sdf_co_null;
	Sdf_st_error	  dLocalErr;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaFormCallIdHeader");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pError == Sdf_co_null)
		return Sdf_co_fail;
	if(pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormCallIdHeader( ): "
			"Invalid Sip Message",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormCallIdHeader( ): "
			"Invalid Call Object",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */


	/*
	 * Get the common info header and retrieve the call-id from that.
	 * Form the call-id header using that value.
	 */
	if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, \
		&pCommonInfo, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormCallIdHeader( ): "
			"Failed to get Common Info from Call Object",pError);
#endif		
		return Sdf_co_fail;
	}	
	if ((sip_initSipHeader(&pHeader, SipHdrTypeCallId, \
		(SipError*)&(pError->stkErrCode))) == SipFail)
	{
		pError->errCode = pError->stkErrCode;
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormCallIdHeader( ): "
			"Failed to init CallId header",pError);
#endif		
		return Sdf_co_fail;
	}
	pTmpVal = 	Sdf_mc_strdupCallHandler(pCommonInfo->pCallid);
	
	if (sip_setValueInCallIdHdr(pHeader, pTmpVal, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&pHeader, &dLocalErr);
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormCallIdHeader( ): "
			"Failed to set CallId",pError);
#endif		
		return Sdf_co_fail;
	}	
	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);

	/*
	 * Set the header in the SIP message
	 */
	if (sip_setHeader(pMessage, pHeader,  \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&pHeader, &dLocalErr);
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormCallIdHeader( ): "
			"Failed to set CallId header in Sip message",pError);
#endif		
		return Sdf_co_fail;
	}	

	/* 
	 * Freeing the Local Reference
	 */
	sip_freeSipHeader(pHeader);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
		(Sdf_ty_pvoid *)&pHeader, pError);

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaFormCallIdHeader");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFormContactHeaders
 **
 ** DESCRIPTION: This function forms the Contact header with the information 
 **				 in the ContactList and sets this value in the SIP Message.
 **
 ******************************************************************************/
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/

Sdf_ty_retVal sdf_fn_uaFormContactHeaders
#ifdef ANSI_PROTO 
	(Sdf_ty_slist *pContactList, SipMessage *pMessage,
	Sdf_ty_protocol dTranspType, Sdf_ty_addrType dContactAddrType,
	Sdf_ty_bool dStrictCheck,
	Sdf_st_error *pError)
#else
	(pContactList, pMessage, dTranspType, dContactAddrType,dStrictCheck, pError)
	Sdf_ty_slist *pContactList;
	SipMessage   *pMessage;
	Sdf_ty_protocol dTranspType;
	Sdf_ty_addrType  dContactAddrType;
	Sdf_ty_bool  dStrictCheck;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_u32bit 		dIndex=0;
	Sdf_st_listIterator dListIterator;
	Sdf_ty_bool         dCorrectContactFound = Sdf_co_false;
	SipListElement		*pCurElmt = Sdf_co_null;
	SipHeader 	*pSrcHeader  = Sdf_co_null;
	SipHeader	*pDestHeader = Sdf_co_null;

	(void) dContactAddrType;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaFormContactHeaders");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pError == Sdf_co_null)
		return Sdf_co_fail;

	if(pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormContactHeaders( ): "
			"Invalid Sip message",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pContactList == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormContactHeaders( ): "
			"Invalid Contact List",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */
	
	/*
	 * Get the count of list of the contact headers for self from he
	 * initdata in the call object. Clone the topmost Contact and use
	 * reference counting to insert the rest into the message.
	 */
	(void)sdf_listInitIterator(pContactList, &dListIterator, pError);

	/* save the first element of the list. To be used incase not matching
	 * contact is found.*/
	pCurElmt = dListIterator.pCurrentElement;


	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SipAddrSpec		*pAddrSpec = Sdf_co_null;
		Sdf_ty_protocol dTranspParam = Sdf_en_protoNone;

		pSrcHeader = (SipHeader*)(dListIterator.pCurrentElement->pData);

		(void)sip_getAddrSpecFromContactHdr(pSrcHeader, &pAddrSpec, \
				(SipError *)&(pError->stkErrCode));

		(void)sdf_fn_uaGetTransportParam( pAddrSpec, &dTranspParam, pError);

#ifdef SDF_TLS
		if(dContactAddrType.dTransportScheme == Sdf_en_protoTls)
		{
			if( pAddrSpec->dType == SipAddrSipSUri )
			{
				if(dContactAddrType.dProtocolScheme == SipAddrSipSUri)
					dCorrectContactFound = Sdf_co_true;
			}
			else if (pAddrSpec->dType == SipAddrSipUri)
			{
				if(dTranspParam == Sdf_en_protoTls)
					dCorrectContactFound = Sdf_co_true;
			}
			if(dCorrectContactFound != Sdf_co_true)
			{
				(void)sdf_listNext(&dListIterator, pError);
				sip_freeSipAddrSpec(pAddrSpec);
				continue;
			}
		}
		else
#endif
		if(dTranspType == Sdf_en_protoUdp)
		{
			if((dTranspParam == Sdf_en_protoNone) || \
					(dTranspParam == Sdf_en_protoUdp))
				dCorrectContactFound = Sdf_co_true;

			if(dCorrectContactFound != Sdf_co_true)
			{
				(void)sdf_listNext(&dListIterator, pError);
				sip_freeSipAddrSpec(pAddrSpec);
				continue;
			}
		}
		else
			dCorrectContactFound = Sdf_co_true;

		/* End of the while loop */
		if (Sdf_co_true == dCorrectContactFound)
		{
			sip_freeSipAddrSpec(pAddrSpec);
			break;
		}
	}

	if (dCorrectContactFound != Sdf_co_true )
	{
		if(Sdf_co_true == dStrictCheck)
		{
			if (Sdf_co_null != pCurElmt)
			{
				/* we need to pick the local contact formation from what is
				 * configured as the first Port and IP*/
				pSrcHeader = (SipHeader*)(pCurElmt->pData);
			}
			else
			{
				pError->errCode = Sdf_en_contactSchemeMisMatch;
				return Sdf_co_fail;
			}
		}
		else
		{
			pError->errCode = Sdf_en_noUaError;
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaFormContactHeaders");
			return Sdf_co_success;
		}
	}
	
	/* here we shall add the header as picked from the scheme that matches
	 * the one in the outgoing Route Headre or the Request URI. But if not
	 * such contact is available then we shall insert the one that is
	 * default (first configured in our case) provided that it is mandatory
	 * to insert the contact header and no matching contact is found.*/
	if (sip_initSipHeader(&pDestHeader, SipHdrTypeAny, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaFormContactHeaders( ): "
				"Failed to init dest header",pError);
#endif			
		return Sdf_co_fail;
	}

	pDestHeader->dType = pSrcHeader->dType;
	pDestHeader->pHeader = pSrcHeader->pHeader;
	HSS_LOCKEDINCREF(((SipContactHeader *)(pDestHeader->pHeader))->dRefCount);
	/* 
	 * Insert the header in the SipMessage.
	 */
	(void)sip_insertHeaderAtIndex(pMessage, pDestHeader, \
			(Sdf_ty_u32bit)dIndex,(SipError *)&(pError->stkErrCode));

	sip_freeSipHeader(pDestHeader);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pDestHeader,pError);

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaFormContactHeaders");
	return Sdf_co_success;
}


#else

Sdf_ty_retVal sdf_fn_uaFormContactHeaders
#ifdef ANSI_PROTO 
	(Sdf_ty_slist *pContactList, SipMessage *pMessage,
	Sdf_ty_protocol dTranspType, en_AddrType dContactAddrType,
	Sdf_ty_bool dStrictCheck,
	Sdf_st_error *pError)
#else
	(pContactList, pMessage, dTranspType, dContactAddrType,dStrictCheck, pError)
	Sdf_ty_slist *pContactList;
	SipMessage   *pMessage;
	Sdf_ty_protocol dTranspType;
	en_AddrType  dContactAddrType;
	Sdf_ty_bool  dStrictCheck;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_u32bit 		dIndex=0;
	Sdf_st_listIterator dListIterator;
	Sdf_st_error		dLocalErr;
	Sdf_ty_bool         dCorrectContactFound = Sdf_co_false;
	en_AddrType         dAddrType;

	(void)dTranspType;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaFormContactHeaders");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pError == Sdf_co_null)
		return Sdf_co_fail;

	if(pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormContactHeaders( ): "
			"Invalid Sip message",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pContactList == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormContactHeaders( ): "
			"Invalid Contact List",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */
	
	/*
	 * Get the count of list of the contact headers for self from he
	 * initdata in the call object. Clone the topmost Contact and use
	 * reference counting to insert the rest into the message.
	 */
	(void)sdf_listInitIterator(pContactList, &dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SipHeader 	*pSrcHeader  = Sdf_co_null;
		SipHeader	*pDestHeader = Sdf_co_null;
		SipAddrSpec	*pAddrSpec = Sdf_co_null;

		pSrcHeader = (SipHeader*)\
			(dListIterator.pCurrentElement->pData);

		/*w534*/ (void)sip_getAddrSpecFromContactHdr(pSrcHeader, &pAddrSpec, \
				(SipError *)&(pError->stkErrCode));

		dAddrType = pAddrSpec->dType;
		sip_freeSipAddrSpec(pAddrSpec);

		if(dContactAddrType == SipAddrSipUri)
		{
			if(dAddrType != SipAddrSipUri)
			{
				(void)sdf_listNext(&dListIterator, pError);
				continue;
			}
			dCorrectContactFound = Sdf_co_true;
		}
#ifdef SDF_TLS
		else if(dContactAddrType == SipAddrSipSUri)
		{
			if(dAddrType != SipAddrSipSUri)
			{
				(void)sdf_listNext(&dListIterator, pError);
				continue;
			}
			dCorrectContactFound = Sdf_co_true;
		}
#endif
		else
			dCorrectContactFound = Sdf_co_true;


		if (sip_initSipHeader(&pDestHeader, SipHdrTypeAny, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaFormContactHeaders( ): "
				"Failed to init dest header",pError);
#endif			
			return Sdf_co_fail;
		}
	
		pDestHeader->dType = pSrcHeader->dType;
		pDestHeader->pHeader = pSrcHeader->pHeader;
		HSS_LOCKEDINCREF(((SipContactHeader *)(pDestHeader->pHeader))->dRefCount);
		/* 
		 * Insert the header in the SipMessage.
		 */
		if (sip_insertHeaderAtIndex(pMessage, pDestHeader, \
			(Sdf_ty_u32bit)dIndex,(SipError *)&(pError->stkErrCode))==SipFail)
		{
			pError->errCode = pError->stkErrCode;
			sip_freeSipHeader(pDestHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid *)&pDestHeader, &dLocalErr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaFormContactHeaders( ): "
				"Failed to insert Contact header in Sip message", \
				pError);
#endif			
			return Sdf_co_fail;
		}	

		sip_freeSipHeader(pDestHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pDestHeader,pError);
		/* FIX FOR SPR 3971 */
    /*   break;         */
    /* New code for SPR 3971 START */
    (void)sdf_listNext(&dListIterator, pError);
		dIndex++;
    /* New code for SPR 3971 END   */
	} /* End of the while loop */

	if(dCorrectContactFound != Sdf_co_true && Sdf_co_true == dStrictCheck)
	{
		pError->errCode = Sdf_en_contactSchemeMisMatch;
		return Sdf_co_fail;
	}

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaFormContactHeaders");
	return Sdf_co_success;
}
#endif
#ifdef SDF_UASSF_ONLY
/*SPR-4629: subba change will come in to picture. specific to ssf*/
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetTransportParam
 **
 ** DESCRIPTION:  Subbarao written this code for ssf.
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetTransportParam
(SipAddrSpec	*pAddrSpec, Sdf_ty_protocol *pTranspType, Sdf_st_error *pError)
{
	SipUrl			*pSipUrl=Sdf_co_null;
	Sdf_ty_u32bit	dUrlParamCount=0;
	*pTranspType =  Sdf_en_protoNone;

	(void)sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pSipUrl, pError);

	(void)sip_getUrlParamCountFromUrl(pSipUrl, &dUrlParamCount, \
		(SipError*)&(pError->stkErrCode));

	if (dUrlParamCount > 0)
	{
		Sdf_ty_u32bit dIndex;

		for (dIndex = 0; dIndex < dUrlParamCount; dIndex++)
		{
			SipParam 		*pUrlParam = Sdf_co_null;
			Sdf_ty_s8bit	*pName = Sdf_co_null;

			(void)sip_getUrlParamAtIndexFromUrl(pSipUrl, &pUrlParam,\
				dIndex, (SipError*)&(pError->stkErrCode));

			(void)sip_getNameFromSipParam(pUrlParam, &pName, \
				(SipError*)&(pError->stkErrCode));
	
			if (Sdf_mc_strcmp(pName, "transport") == 0)
			{
			    Sdf_ty_s8bit *pTempTranspType = Sdf_co_null;

				(void)sip_getValueAtIndexFromSipParam(pUrlParam, \
					&pTempTranspType, 0, (SipError*) \
					&(pError->stkErrCode));
		
				if (Sdf_mc_strcasecmp(pTempTranspType, "TCP") == 0)
						*pTranspType = Sdf_en_protoTcp;
#ifdef SDF_TLS
				else if(Sdf_mc_strcasecmp(pTempTranspType, "TLS") == 0)
						*pTranspType = Sdf_en_protoTls;
#endif
				else if(Sdf_mc_strcasecmp(pTempTranspType, "UDP") == 0)		
						*pTranspType = Sdf_en_protoUdp;
				else		
					*pTranspType =  Sdf_en_protoNone;
			}
			sip_freeSipParam(pUrlParam);
		}
	}
	sip_freeSipUrl(pSipUrl);
	return Sdf_co_success;
}

#endif 
/* End of subba added function:  sdf_fn_uaGetTransportParam */

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFormProcessedHeaders
 **
 ** DESCRIPTION: This function forms the Processed headers with the 
 **				 information in the Call Object and sets this value in the 
 **				 SIP Message. 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormProcessedHeaders
#ifdef ANSI_PROTO 
	(Sdf_st_callObject 		*pObject, 
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	Sdf_st_msgInfo	dMessageInfo, 
	SipMessage 		*pMessage,
	Sdf_ty_bool 	dReliableResponse, 
	Sdf_st_error 	*pError)
#else
	(pObject, pOverlapTransInfo, dMessageInfo, pMessage, 
	 dReliableResponse, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_st_msgInfo	dMessageInfo, 
	SipMessage *pMessage;
	Sdf_ty_bool 	dReliableResponse;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_slist	*pSlProcessedHeaders = Sdf_co_null;
	Sdf_ty_bool		dRouteFound = Sdf_co_false;
	en_SipMessageType dMessageType;
	Sdf_st_listIterator dListIterator;
	Sdf_ty_messageType 	dMsgType	= Sdf_en_unknownRequest;
	Sdf_st_error		dLocalErr;
	Sdf_st_headerList *pProxyReqHeaderList=Sdf_co_null;
	Sdf_st_headerList *pRouteHeaderList=Sdf_co_null;
	Sdf_st_headerList *pRecordRouteHeaderList=Sdf_co_null;
	Sdf_st_headerList *pViaHeaderList=Sdf_co_null;
	Sdf_ty_u16bit dRespCode=0;
	Sdf_st_headerList *pHeaderList = Sdf_co_null;
	Sdf_ty_u32bit 	dRouteListSize = 0;
	Sdf_ty_u32bit	dRouteSize=0;
        Sdf_ty_u32bit	dIndex = 0;
#ifdef SDF_CONGEST
    Sdf_st_headerList   *pRsrcPriorityHeaderList = Sdf_co_null;    
#endif
#ifdef SDF_3GPP
    Sdf_st_headerList   *pHistoryInfoHeaderList  = Sdf_co_null;    
#endif
#ifdef SDF_PEMEDIA
    Sdf_st_headerList   *pPEMediaHeaderList      = Sdf_co_null;    
#endif
#ifdef SDF_PRIVACY
	Sdf_st_headerList   *pPrivacyHeaderList      = Sdf_co_null;  
	Sdf_st_headerList   *pPprefferedIdHeaderList = Sdf_co_null;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaFormProcessedHeaders");
	
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pError == Sdf_co_null)
		return Sdf_co_fail;
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): "
			"Invalid Param: call object",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): "
			"Invalid Param: sip message",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	dMsgType = dMessageInfo.dMsgType;
	
	if ( dMsgType == Sdf_en_ack)
		/*w534*/ (void)sdf_ivk_uaGetRespCodeFromSipMessage(pObject->pUacTransaction->pSipMsg,\
			  &dRespCode, pError);
	
	dMessageType = dMessageInfo.dReqRespType;

	/*
	 * Select which list to operate upon, depending on whether the
	 * overlaptransinfo is NULL or not.
	 */
	if (pOverlapTransInfo == Sdf_co_null)
	{
		Sdf_st_transaction *pTransaction = Sdf_co_null;
		if (dMessageType == SipMessageRequest)
		{
			if (sdf_ivk_uaGetTransactionFromCallObject(pObject, \
				Sdf_en_uacTransaction, &pTransaction, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_callObjectAccessError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): "
					"Failed to get UAC transaction from call object",pError);
#endif			
				pError->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
		}
		else
		{
			if (sdf_ivk_uaGetTransactionFromCallObject(pObject, \
				Sdf_en_uasTransaction, &pTransaction, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_callObjectAccessError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): "
					"Failed to get UAS transaction from call object",pError);
#endif			
				pError->errCode=Sdf_en_callObjectAccessError;
		        return Sdf_co_fail;
			}
		}
		pSlProcessedHeaders = &(pTransaction->slProcessedHeaders);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
	}
	else
		pSlProcessedHeaders = &(pOverlapTransInfo->slProcessedHeaders);
		

	if(Sdf_co_null == pSlProcessedHeaders)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors, \
				Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): "
				"Failed to get pSlProcessedHeaders from transaction",pError);
#endif
		return Sdf_co_fail;
	}

	/*
	 * Insert the Route, Record-Route and Proxy-Require header first
	 */
	(void)sdf_listInitIterator(pSlProcessedHeaders, &dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		pHeaderList=(Sdf_st_headerList*)(dListIterator.pCurrentElement->pData);

		if (pHeaderList->dType == SipHdrTypeRoute)
		{
			/* an ACK request is being formed for a non-200 Response, 
			 * the Route  header MUST NOT be inserted into the message
			 */

			if ((dMsgType == Sdf_en_ack) && (dRespCode > 299))
			{
				(void)sdf_listNext(&dListIterator, pError);
				continue;
			}

			pRouteHeaderList=pHeaderList;
			dRouteFound = Sdf_co_true;
		}
		else if	(pHeaderList->dType == SipHdrTypeRecordRoute) 
			pRecordRouteHeaderList=pHeaderList;
		else if	(pHeaderList->dType == SipHdrTypeVia)
			pViaHeaderList=pHeaderList;
#ifdef SDF_CONGEST
        else if (pHeaderList->dType == SipHdrTypeRsrcPriority)
			pRsrcPriorityHeaderList=pHeaderList;
#endif        
#ifdef SDF_3GPP
		else if (pHeaderList->dType == SipHdrTypeHistoryInfo)
			pHistoryInfoHeaderList=pHeaderList;
#endif
#ifdef SDF_PEMEDIA
        else if (pHeaderList->dType == SipHdrTypePEarlyMedia)
			pPEMediaHeaderList=pHeaderList;
#endif
#ifdef SDF_PRIVACY
        else if (pHeaderList->dType == SipHdrTypePrivacy)
            pPrivacyHeaderList=pHeaderList;
        else if (pHeaderList->dType == SipHdrTypePPreferredId)
            pPprefferedIdHeaderList=pHeaderList;
#endif    
		(void)sdf_listNext(&dListIterator, pError);
	}

	/* Handle Order of headers in the Request Message */

	if (dMessageType == SipMessageRequest)
	{
#ifdef SDF_AUTHENTICATE	
		Sdf_ty_slist    slAuthenticateList;
        Sdf_ty_u32bit   dAuthSize = 0; 
#endif		
		SipHeader *pHdr = Sdf_co_null;
		Sdf_ty_u32bit dHops=0;

		/* If Route found then insert the Route header into the sipmessage
		 */
		if(dRouteFound == Sdf_co_true)
		{
			if(sdf_fn_uaInsertHeaderFromList(pMessage,pRouteHeaderList,\
				pError) == Sdf_co_fail )
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders(): "
					"Failed to Insert Header List from List", pError);
#endif				
				return Sdf_co_fail;
			}
		}
		/* else take it from the persistent list of the call object stroed with
		 * correlation string uat-routeset
		 */
		else if(dMsgType != Sdf_en_cancel)
		{
			Sdf_ty_slist 	slRouteHeaderList;

			/* 
			 * This if block handles the case of normal SIP Request messages
			 * and PRACK/COMET requests
			 */
			if((dMsgType != Sdf_en_ack) || (dRespCode/100 == 2))
			{
				/*
				 * Add the Route headers if a Route set is present in the 
				 * Persistent Headers list
				 */
				if (sdf_ivk_uaGetHeaderListByCorrelation(&(pObject->\
					slPersistentHeaders),(Sdf_ty_s8bit *) "uat-routeset", \
					&slRouteHeaderList, pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
						(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders(): "
						"Failed to get Route header list by correlation "
						"uat-routeset", pError);
#endif				
					return Sdf_co_fail;
				}

				(void)sdf_listSizeOf(&slRouteHeaderList, &dRouteSize, pError);

				if( dRouteSize > 0 )
				{
					/*
					 * Get the uat-routeset HeaderList structure
					 */
					if (sdf_listGetAt(&slRouteHeaderList, 0, \
						(Sdf_ty_pvoid *)&pHeaderList, pError) == Sdf_co_fail)
					{
#ifdef SDF_ERROR				
						sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
							(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): "
							"Failed to get Route Header List from" \
						   	"Persistant Headers",pError);
#endif				
						(void)sdf_listDeleteAll(&slRouteHeaderList,pError);
						return Sdf_co_fail;
					}
					(void)sdf_listSizeOf(&(pHeaderList->slHeaders), &dRouteListSize, \
						pError);
				}
				/* One route entry means it contains only the Remote Target
				 * URI or contact address present.  No Route headers are 
				 * present. */
				if ((dRouteListSize > 1) && (pHeaderList != Sdf_co_null))
				{
					/*
					 * Set the Route headers into the SipMessage
					 */
					for (dIndex = 0; dIndex < dRouteListSize; dIndex++)
					{
						SipHeader *pRouteHeader;

						if (sdf_listGetAt(&(pHeaderList->slHeaders), dIndex, \
							(Sdf_ty_pvoid *)&pRouteHeader, pError) == \
							Sdf_co_fail)
						{
#ifdef SDF_ERROR				
							sdf_fn_setError(Sdf_en_minorErrors, \
								pError->errCode, (Sdf_ty_s8bit *)\
								"sdf_fn_uaFormProcessedHeaders( ): "
								"Failed to get Route header from uat-routeset",\
								pError);
#endif					
							(void)sdf_listDeleteAll(&slRouteHeaderList,pError);
							return Sdf_co_fail;
						}	

						if (sip_insertHeaderAtIndex(pMessage, pRouteHeader, \
							dIndex, (SipError *)&(pError->stkErrCode)) == \
							SipFail)
						{
							pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR							
							sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
							(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders(): "
								"Failed to insert Route header from "
								"uat-routeset into SipMessage", pError);
#endif							
							(void)sdf_listDeleteAll(&slRouteHeaderList,pError);
							return Sdf_co_fail;
						}
					} /* End of for loop inserting Route headers */
				} /* End of "if uat-routeset exists */
				(void)sdf_listDeleteAll(&slRouteHeaderList,pError);
			} /* End of "if dMsgType != ACK" */
		}/* End of "if dRouteFound == False" and method is not cancel */

		/* Insert Proxy-Require Header */
		if(pProxyReqHeaderList !=Sdf_co_null)
			/*w534*/ (void)sdf_fn_uaInsertHeaderFromList(pMessage,pProxyReqHeaderList,\
				pError);
		/* insert the Max-Forward header */
		if(sip_initSipHeader(&pHdr,SipHdrTypeMaxforwards, \
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR						
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,(Sdf_ty_s8bit*) \
				"sdf_fn_uaFormProcessedHeaders(): "
				"Could Not Initialize SipHeader structure", pError);
#endif						
			return Sdf_co_fail;
		}

		/* Get the number of Hops for the Max-forward header */
		(void)sdf_fn_uaGetNumberOfHopsForMaxForward(pObject,&dHops,pError);
		/* Check for Boundary values.
		 * As per RFC3261: MaxFwds should be b/n 0-255 -*/
		if (dHops > 255)
			dHops = 255;

		/* Set the number of hops in the max-forward header
		 */
		if(sip_setHopsInMaxForwardsHdr(pHdr,dHops, \
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR						
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,(Sdf_ty_s8bit*) \
				"sdf_fn_uaFormProcessedHeaders(): "
				"Could Not Set Hops in Max-Forward Header", pError);
#endif						
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr, \
									&dLocalErr);
			return Sdf_co_fail;
		}

		/* Insert the Max-Forward header into the outgoing message
		 */
		if(sip_insertHeaderAtIndex(pMessage,pHdr,0, \
			(SipError*)& (pError->stkErrCode))==SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR						
			sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
				(Sdf_ty_s8bit*) "sdf_fn_uaFormProcessedHeaders(): "
				"Could Not insert Max-Forward header into the "
				"SipMessage",pError);
#endif						
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr, \
									&dLocalErr);
			return Sdf_co_fail;
		}
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr, &dLocalErr);
#ifdef SDF_AUTHENTICATE

		/* Add SipHdrTypeProxyauthorization and SipHdrTypeAuthorization 
         * headers into message.  They are extracted from persistent 
         * header of callObject */ 
		/*w534*/ (void)sdf_ivk_uaGetHeaderListByCorrelation(&(pObject->slPersistentHeaders),\
			(Sdf_ty_s8bit *) "uat-authenticate", &slAuthenticateList, pError); 
		(void)sdf_listSizeOf(&slAuthenticateList, &dAuthSize,  &dLocalErr); 

		if( dAuthSize > 0) 
		{ 
			Sdf_ty_u32bit      dAuthListSize = 0; 

		   /*  Get the uat-authenticate HeaderList structure */ 
			(void)sdf_listGetAt(&slAuthenticateList, 0,  \
						   (Sdf_ty_pvoid *)&pHeaderList, pError); 
			(void)sdf_listSizeOf(&(pHeaderList->slHeaders), &dAuthListSize, pError); 

		   /* Set the Authenticate headers into the SipMessage */ 
			for (dIndex = 0; dIndex < dAuthListSize; dIndex++) 
			{ 
				SipHeader *pAuthHeader = Sdf_co_null; 

				(void)sdf_listGetAt(&(pHeaderList->slHeaders), dIndex, \
					   (Sdf_ty_pvoid *)&pAuthHeader, pError); 
                /* CSR 1-6665689 Changes Start */
                /*Fix merged from CSR-1-4878211 for CSR-1-6179101*/
				/* IPTK Auth Change Start */
				if(dMsgType != Sdf_en_ack)
				{
					SipGenericCredential 		*pAuthCredential=Sdf_co_null;
					SipGenericChallenge			*pAuthChallenge=Sdf_co_null;	
					Sdf_ty_u32bit						dAuthCount=0;
					Sdf_ty_u32bit 					dIndex1=0;
					Sdf_ty_s8bit 						*pScheme=Sdf_co_null;
					
					sip_getCredentialsFromAuthorizationHdr(pAuthHeader,\
													&pAuthCredential,(SipError*) &(pError->stkErrCode));
					sip_getChallengeFromCredential(pAuthCredential,\
													&pAuthChallenge,(SipError*)&(pError->stkErrCode));
					sip_getSchemeFromChallenge(pAuthChallenge, &pScheme,\
													(SipError*)&(pError->stkErrCode));
					
					if((Sdf_mc_strcasecmp(pScheme,"Digest") == 0))
					{
						sip_getAuthorizationParamCountFromChallenge(pAuthChallenge,\
													&dAuthCount,(SipError*)&(pError->stkErrCode));

					/*
				 	 * Iterate through the parameters in the  Authorization header and
				 	 *  extract the relevant parameters into pUacAuthInfo and pAuthParams						 */

						for (dIndex1=0; dIndex1 < dAuthCount; dIndex1++)
						{
							Sdf_ty_s8bit *pName, *pValue;
							SipParam *pParam;
							Sdf_ty_u32bit dHex;
							Sdf_ty_s8bit *pNewNC = Sdf_co_null;

							pParam=Sdf_co_null;
					
							if (sip_getAuthorizationParamAtIndexFromChallenge(pAuthChallenge,\
								&pParam, dIndex1,(SipError *) &(pError->stkErrCode)) == SipFail)
							{
								sip_freeSipGenericChallenge(pAuthChallenge);
								sip_freeSipHeader(pAuthHeader);
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_majorErrors,\
															 	Sdf_en_headerManipulationError,\
								(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
								"Error Getting the param from Authenticate Challenge", pError);
#endif
								pError->errCode=Sdf_en_headerManipulationError;
								return Sdf_co_fail;
							}
							if (sip_getNameFromSipParam(pParam, &pName, \
			 					(SipError *)&(pError->stkErrCode)) == SipFail)
							{
								sip_freeSipGenericChallenge(pAuthChallenge);
								sip_freeSipParam(pParam);
								sip_freeSipHeader(pAuthHeader);
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_majorErrors,\
															 	Sdf_en_headerManipulationError,\
								(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
								"Error Getting the param from Authenticate Challenge", pError);
#endif
								pError->errCode=Sdf_en_headerManipulationError;
								return Sdf_co_fail;
							}
							if (Sdf_mc_strcasecmp(pName, "nc") == 0)
							{
								
								if (sip_getValueAtIndexFromSipParam(pParam, &pValue, 0,\
								(SipError *)&(pError->stkErrCode)) == SipFail)
								{
									sip_freeSipGenericChallenge(pAuthChallenge);
									sip_freeSipParam(pParam);
									sip_freeSipHeader(pAuthHeader);
#ifdef SDF_ERROR
									sdf_fn_setError(Sdf_en_majorErrors,\
																 	Sdf_en_headerManipulationError,\
									(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
									"Error Getting the param from Authenticate Challenge", pError);
#endif
									pError->errCode=Sdf_en_headerManipulationError;
									return Sdf_co_fail;
								}
								
								pNewNC = (Sdf_ty_s8bit *) sdf_memget (Sdf_mc_callHandlerMemId, \
		Sdf_mc_strlen(pValue)+ 1, pError);

								if(Sdf_co_null == pNewNC)
								{
									sip_freeSipGenericChallenge(pAuthChallenge);
									sip_freeSipParam(pParam);
									sip_freeSipHeader(pAuthHeader);
									pError->errCode=Sdf_en_noMemoryError;
									return Sdf_co_fail;
								}
								
								Sdf_mc_strcpy(pNewNC,pValue);
							
								sscanf(pNewNC,"%x",&dHex);
								dHex = dHex+1;
								sprintf(pNewNC,"%.8x",dHex);
								sip_setValueAtIndexInSipParam(pParam,pNewNC,0,\
																(SipError *)&(pError->stkErrCode));
								sip_setAuthorizationParamAtIndexInChallenge(pAuthChallenge,\
								                pParam, dIndex1,(SipError *) &(pError->stkErrCode));

							}
							if (Sdf_mc_strcasecmp(pName, "cnonce") == 0)
							{
								if (sip_getValueAtIndexFromSipParam(pParam, &pValue, 0,\
								(SipError *)&(pError->stkErrCode)) == SipFail)
								{
									sip_freeSipGenericChallenge(pAuthChallenge);
									sip_freeSipParam(pParam);
									sip_freeSipHeader(pAuthHeader);
#ifdef SDF_ERROR
									sdf_fn_setError(Sdf_en_majorErrors,\
																 	Sdf_en_headerManipulationError,\
									(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
									"Error Getting the param from Authenticate Challenge", pError);
#endif
									pError->errCode=Sdf_en_headerManipulationError;
									return Sdf_co_fail;
								}
							}
							sip_freeSipParam(pParam);
						}
					}
					sip_setChallengeInCredential(pAuthCredential,\
													pAuthChallenge,(SipError*)&(pError->stkErrCode));
					sip_setCredentialsInAuthorizationHdr(pAuthHeader,\
													pAuthCredential,(SipError*) &(pError->stkErrCode));
					sip_freeSipGenericChallenge(pAuthChallenge);
					sip_freeSipGenericCredential(pAuthCredential); 
					
				}
				/* IPTK Auth Change End */
                /* CSR 1-6665689 Changes end */

				if (sip_insertHeaderAtIndex(pMessage, pAuthHeader, \
					   dIndex, (SipError *)&(pError->stkErrCode)) == SipFail) 
				{ 
					pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR 
					sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): " 
						   "Failed to insert Auth header from " 
						   "uat-authenticate into SipMessage", pError); 
#endif 
	   				(void)sdf_listDeleteAll(&slAuthenticateList,pError);
					return Sdf_co_fail; 
				} 
			} 
		} 

	   (void)sdf_listDeleteAll(&slAuthenticateList,pError);
	   /*w534*/ (void)sdf_ivk_uaGetHeaderListByCorrelation(&(pObject->slPersistentHeaders),\
			   (Sdf_ty_s8bit *) "uat-proxyauth", &slAuthenticateList, pError); 
	   (void)sdf_listSizeOf(&slAuthenticateList, &dAuthSize,  pError); 

		if( dAuthSize > 0) 
		{ 
		   Sdf_ty_u32bit      dAuthListSize = 0;

		   /*  Get the uat-proxyauth HeaderList structure */ 
		   (void)sdf_listGetAt(&slAuthenticateList, 0,  \
						   (Sdf_ty_pvoid *)&pHeaderList, pError); 
		   (void)sdf_listSizeOf(&(pHeaderList->slHeaders), &dAuthListSize,\
				pError); 

		   /* Set the Authenticate headers into the SipMessage */ 
			for (dIndex = 0; dIndex < dAuthListSize; dIndex++) 
			{ 
			   SipHeader *pAuthHeader = Sdf_co_null; 

			   (void)sdf_listGetAt(&(pHeaderList->slHeaders), dIndex, \
					   (Sdf_ty_pvoid *)&pAuthHeader, pError);

/* IPTK Auth Change Start */
				if(dMsgType != Sdf_en_ack)
				{
					SipGenericCredential 		*pAuthCredential=Sdf_co_null;
					SipGenericChallenge			*pAuthChallenge=Sdf_co_null;	
					Sdf_ty_u32bit						dAuthCount=0;
					Sdf_ty_u32bit 					dIndex1=0;
					Sdf_ty_s8bit 						*pScheme=Sdf_co_null;
					
					sip_getCredentialsFromAuthorizationHdr(pAuthHeader,\
													&pAuthCredential,(SipError*) &(pError->stkErrCode));
					sip_getChallengeFromCredential(pAuthCredential,\
													&pAuthChallenge,(SipError*)&(pError->stkErrCode));
					sip_getSchemeFromChallenge(pAuthChallenge, &pScheme,\
													(SipError*)&(pError->stkErrCode));
					
					if((Sdf_mc_strcasecmp(pScheme,"Digest") == 0))
					{
						sip_getAuthorizationParamCountFromChallenge(pAuthChallenge,\
													&dAuthCount,(SipError*)&(pError->stkErrCode));

					/*
				 	 * Iterate through the parameters in the  Authorization header and
				 	 *  extract the relevant parameters into pUacAuthInfo and pAuthParams						 */

						for (dIndex1=0; dIndex1 < dAuthCount; dIndex1++)
						{
							Sdf_ty_s8bit *pName, *pValue;
							SipParam *pParam;
							Sdf_ty_u32bit dHex;
							Sdf_ty_s8bit *pNewNC = Sdf_co_null;

							pParam=Sdf_co_null;
					
							if (sip_getAuthorizationParamAtIndexFromChallenge(pAuthChallenge,\
								&pParam, dIndex1,(SipError *) &(pError->stkErrCode)) == SipFail)
							{
								sip_freeSipGenericChallenge(pAuthChallenge);
								sip_freeSipHeader(pAuthHeader);
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_majorErrors,\
															 	Sdf_en_headerManipulationError,\
								(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
								"Error Getting the param from Authenticate Challenge", pError);
#endif
								pError->errCode=Sdf_en_headerManipulationError;
								return Sdf_co_fail;
							}
							if (sip_getNameFromSipParam(pParam, &pName, \
			 					(SipError *)&(pError->stkErrCode)) == SipFail)
							{
								sip_freeSipGenericChallenge(pAuthChallenge);
								sip_freeSipParam(pParam);
								sip_freeSipHeader(pAuthHeader);
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_majorErrors,\
															 	Sdf_en_headerManipulationError,\
								(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
								"Error Getting the param from Authenticate Challenge", pError);
#endif
								pError->errCode=Sdf_en_headerManipulationError;
								return Sdf_co_fail;
							}
							if (Sdf_mc_strcasecmp(pName, "nc") == 0)
							{
								
								if (sip_getValueAtIndexFromSipParam(pParam, &pValue, 0,\
								(SipError *)&(pError->stkErrCode)) == SipFail)
								{
									sip_freeSipGenericChallenge(pAuthChallenge);
									sip_freeSipParam(pParam);
									sip_freeSipHeader(pAuthHeader);
#ifdef SDF_ERROR
									sdf_fn_setError(Sdf_en_majorErrors,\
																 	Sdf_en_headerManipulationError,\
									(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
									"Error Getting the param from Authenticate Challenge", pError);
#endif
									pError->errCode=Sdf_en_headerManipulationError;
									return Sdf_co_fail;
								}
								
								pNewNC = (Sdf_ty_s8bit *) sdf_memget (Sdf_mc_callHandlerMemId, \
		Sdf_mc_strlen(pValue)+ 1, pError);

								if(Sdf_co_null == pNewNC)
								{
									sip_freeSipGenericChallenge(pAuthChallenge);
									sip_freeSipParam(pParam);
									sip_freeSipHeader(pAuthHeader);
									pError->errCode=Sdf_en_noMemoryError;
									return Sdf_co_fail;
								}
								
								Sdf_mc_strcpy(pNewNC,pValue);
							
								sscanf(pNewNC,"%x",&dHex);
								dHex = dHex+1;
								sprintf(pNewNC,"%.8x",dHex);
								sip_setValueAtIndexInSipParam(pParam,pNewNC,0,\
																(SipError *)&(pError->stkErrCode));
								sip_setAuthorizationParamAtIndexInChallenge(pAuthChallenge,\
								                pParam, dIndex1,(SipError *) &(pError->stkErrCode));

							}
							if (Sdf_mc_strcasecmp(pName, "cnonce") == 0)
							{
								if (sip_getValueAtIndexFromSipParam(pParam, &pValue, 0,\
								(SipError *)&(pError->stkErrCode)) == SipFail)
								{
									sip_freeSipGenericChallenge(pAuthChallenge);
									sip_freeSipParam(pParam);
									sip_freeSipHeader(pAuthHeader);
#ifdef SDF_ERROR
									sdf_fn_setError(Sdf_en_majorErrors,\
																 	Sdf_en_headerManipulationError,\
									(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
									"Error Getting the param from Authenticate Challenge", pError);
#endif
									pError->errCode=Sdf_en_headerManipulationError;
									return Sdf_co_fail;
								}
								printf("\n\ncnonce=%s\n\n",pValue);
							}
							sip_freeSipParam(pParam);
						}
					}
					sip_setChallengeInCredential(pAuthCredential,\
													pAuthChallenge,(SipError*)&(pError->stkErrCode));
					sip_setCredentialsInAuthorizationHdr(pAuthHeader,\
													pAuthCredential,(SipError*) &(pError->stkErrCode));
					sip_freeSipGenericChallenge(pAuthChallenge);
					sip_freeSipGenericCredential(pAuthCredential); 
					
				}
				/* IPTK Auth Change End */
			   

			   if (sip_insertHeaderAtIndex(pMessage, pAuthHeader, \
				   dIndex, (SipError *)&(pError->stkErrCode)) == SipFail) 
				{ 
					pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR 
				   sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				   (Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): " 
					   "Failed to insert Auth header from " 
					   "uat-proxyauth into SipMessage", pError); 
#endif 
	   				(void)sdf_listDeleteAll(&slAuthenticateList,pError);
				   return Sdf_co_fail; 
				} 
			} 
		} 
	   (void)sdf_listDeleteAll(&slAuthenticateList,pError);
#endif /* SDF_AUTHENTICATE */
#ifdef SDF_PRIVACY
     if(pPrivacyHeaderList !=Sdf_co_null)
       (void)sdf_fn_uaInsertHeaderFromList(pMessage,pPrivacyHeaderList,\
                                           pError);
     if(pPprefferedIdHeaderList !=Sdf_co_null)
       (void)sdf_fn_uaInsertHeaderFromList(pMessage, \
                                           pPprefferedIdHeaderList,pError);
#endif
#ifdef SDF_CONGEST
     if(pRsrcPriorityHeaderList !=Sdf_co_null)
       (void)sdf_fn_uaInsertHeaderFromList(pMessage,pRsrcPriorityHeaderList,\
				pError);
#endif
#ifdef SDF_3GPP
     if(pHistoryInfoHeaderList !=Sdf_co_null)
       (void)sdf_fn_uaInsertHeaderFromList(pMessage,pHistoryInfoHeaderList,\
				pError);
#endif        
#ifdef SDF_PEMEDIA
     if(pPEMediaHeaderList !=Sdf_co_null)
       (void)sdf_fn_uaInsertHeaderFromList(pMessage,pPEMediaHeaderList,\
				pError);
#endif
	}
	/* Handle Order of headers in the Response Message */
	else
	{
		/*
		 * The following if block handles the case of RPR's wherein we need to
		 * pick up the Via and Record-Route headers from the UAS transaction
		 * structure.
		 */
		if (dReliableResponse == Sdf_co_true)
		{
			(void)sdf_listInitIterator(&(pObject->pUasTransaction->\
				slProcessedHeaders),&dListIterator, pError);
			while (dListIterator.pCurrentElement != Sdf_co_null)
			{
				pHeaderList = (Sdf_st_headerList*)\
					(dListIterator.pCurrentElement->pData);
				if (pHeaderList->dType == SipHdrTypeRecordRoute) 
					pRecordRouteHeaderList = pHeaderList;
				else if (pHeaderList->dType == SipHdrTypeVia)
					pViaHeaderList = pHeaderList;
				(void)sdf_listNext(&dListIterator, pError);
			} /* End of iterating through HeaderList structures */
		}
		/* Insert the Via Header First */
		if(pViaHeaderList !=Sdf_co_null)
			/*w534*/ (void)sdf_fn_uaInsertHeaderFromList(pMessage,pViaHeaderList,\
				pError);

		/* Insert Record Route header */
        if( dMessageInfo.dResponse != 100)
		{
			if(pRecordRouteHeaderList !=Sdf_co_null)
			/*w534*/ (void)sdf_fn_uaInsertHeaderFromList(pMessage,pRecordRouteHeaderList,\
				pError);
		}
	}

	/*
	 * Operate on elements of the selected "processed headers" list
	 */
	(void)sdf_listInitIterator(pSlProcessedHeaders, &dListIterator, pError);


	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_ty_u32bit dJindex, dListLength;

		pHeaderList = (Sdf_st_headerList*)\
			(dListIterator.pCurrentElement->pData);

		(void)sdf_listSizeOf(&(pHeaderList->slHeaders), &dListLength, pError);

		/* The If statements below check if : 
		 * 	the list retrieved is a list of Route Headers,
		 * 	Record-Route headers, Proxy-Require headers,
		 * 	Via Headers, Pr0xy-Authorization or WWW-Authoraization header
		 * 	then ignore those headers. Those headers had been proceessed
		 * 	earlier
		 */

#ifdef SDF_AUTHENTICATE
		if (pMessage->dType == SipMessageRequest)
		{
			if ((pHeaderList->dType == SipHdrTypeWwwAuthenticate) || \
				(pHeaderList->dType == SipHdrTypeProxyAuthenticate) )
			{
				(void)sdf_listNext(&dListIterator, pError);
				continue;
			}

		}
#endif
#ifdef SDF_CONGEST
        if (pMessage->dType == SipMessageRequest)
        {
            if(pHeaderList->dType == SipHdrTypeRsrcPriority)
            {
                (void)sdf_listNext(&dListIterator, pError);
                continue;
            }
        }
#endif
#ifdef SDF_3GPP
        if (pMessage->dType == SipMessageRequest)
        {
			if(pHeaderList->dType == SipHdrTypeHistoryInfo)
			{
                (void)sdf_listNext(&dListIterator, pError);
                continue;
            }
        }
#endif
#ifdef SDF_PEMEDIA
        if (pMessage->dType == SipMessageRequest)
        {
            if(pHeaderList->dType == SipHdrTypePEarlyMedia)
            {
                (void)sdf_listNext(&dListIterator, pError);
                continue;
            }
        }
#endif

#ifdef SDF_PRIVACY
        if (pMessage->dType == SipMessageRequest)
        {
            if((pHeaderList->dType == SipHdrTypePrivacy) || \
                    (pHeaderList->dType == SipHdrTypePPreferredId))
            {
                (void)sdf_listNext(&dListIterator, pError);
                continue;
            }
        }
#endif
#ifdef SDF_AUTHENTICATE
		if ( (pHeaderList->dType == SipHdrTypeRoute) ||\
			(pHeaderList->dType == SipHdrTypeRecordRoute) || \
			(pHeaderList->dType == SipHdrTypeVia) || \
			(pHeaderList->dType == SipHdrTypeProxyauthorization) ||\
			(pHeaderList->dType == SipHdrTypeAuthorization))
#else
		if ( (pHeaderList->dType == SipHdrTypeRoute) ||\
			(pHeaderList->dType == SipHdrTypeRecordRoute) || \
			(pHeaderList->dType == SipHdrTypeVia))
#endif /* SDF_AUTHENTICATE */
		{
			(void)sdf_listNext(&dListIterator, pError);
			continue;

		}
			
		for(dJindex=0; dJindex<dListLength; dJindex++)
		{
			SipHeader *pElement;

			if (sdf_listGetAt(&(pHeaderList->slHeaders), dJindex, \
				(Sdf_ty_pvoid *)&pElement, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): "
					"Failed to get header from list within Processed header "
					"structure",pError);
#endif					
				return Sdf_co_fail;
			}	
			switch (pHeaderList->dType)
			{
				case SipHdrTypeContactAny:
#if defined(MICROUA) && !defined(SIP_DATE)
#else
				case SipHdrTypeExpiresAny:
				case SipHdrTypeRetryAfterAny:
#endif
#ifdef SDF_3GPP                
				case SipHdrTypeHistoryInfo:
				{
                    if (dMessageInfo.dResponse > 199)
                    {
                        if (sip_insertHeaderAtIndex(pMessage, pElement, \
                                    dJindex, (SipError *)&(pError->stkErrCode)) == SipFail)
                        {
                            pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR							
                            sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
                                    (Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): "
                                    "Failed to insert header into SipMessage", \
                                    pError);
#endif							
                            return Sdf_co_fail;
                        }
                    }
                    break;
                }
#endif                 
				case SipHdrTypeAny:
				{
#ifdef SDF_ERROR							
					sdf_fn_setError(Sdf_en_minorErrors, \
						Sdf_en_invalidTypeError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): "
						"Invalid header type \"any\" in HeaderList", pError);
#endif							
					pError->errCode=Sdf_en_invalidTypeError;
					return Sdf_co_fail;
				}
				default:
				{
					if (sip_insertHeaderAtIndex(pMessage, pElement, \
						dJindex, (SipError *)&(pError->stkErrCode)) == SipFail)
					{
						pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR							
						sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
						(Sdf_ty_s8bit *)"sdf_fn_uaFormProcessedHeaders( ): "
							"Failed to insert header into SipMessage", \
							pError);
#endif							
						return Sdf_co_fail;
					}
					break;
				}
			} /* End of Switch Statement */	
		} /* End of inner for loop looping through header elements */
		(void)sdf_listNext(&dListIterator, pError);
	} /* End of outer for loop looping through siplists of headers */	


	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaFormProcessedHeaders");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaIsValidCseq
 **
 ** DESCRIPTION: Checks if the Cseq in the SipMessage is higher than that 
 **				 in the callobject. Returns Sdf_en_valid if Cseq in SipMessage
 **				 is higher. Returns Sdf_en_invalid if Cseq is invalid. Returns 
 **				 Sdf_en_discarded if the message is to be discarded (Eg. a 
 **				 retransmitted INVITE has come when the original INVITE has 
 **				 not yet been responded to) Returns Sdf_en_failed if an API 
 **				 fails.
 **
 ******************************************************************************/
Sdf_ty_messageValidity sdf_fn_uaIsValidCseq
#ifdef ANSI_PROTO
	(Sdf_st_callObject 	*pCallObj,
	SipMessage 			*pMsg,
	Sdf_ty_s32bit 		*pDisplayCallFlow,
	Sdf_ty_messageType	dMethodType,
	Sdf_ty_u16bit 		*pRespCode,
	Sdf_ty_bool	  		*pIsSameCseq,
	Sdf_st_error 		*pError)
#else	
	(pCallObj, pMsg, pDisplayCallFlow, dMsgType, pRespCode, pIsSameCseq, pError)
	Sdf_st_callObject	*pCallObj;
	SipMessage 			*pMsg;
	Sdf_ty_s32bit 		*pDisplayCallFlow;
	Sdf_ty_messageType 	dMethodType;
	Sdf_ty_u16bit 		*pRespCode;
	Sdf_ty_bool	  		*pIsSameCseq;
	Sdf_st_error 		*pError;
#endif
{
	SipHeader pMsgHeader;
	Sdf_ty_u32bit dSeqNum=0;
	Sdf_ty_u32bit maxCseq=0;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaIsValidCseq");	

	*pRespCode=400;

	/*
	 * Get the Cseq number from the received request
	 */
	if (sip_getHeader(pMsg, SipHdrTypeCseq, &pMsgHeader, \
		(SipError *) &(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIsValidCseq( ): "
			"Failed to get  CSeq header from Sip Message",pError);	
#endif		
		return Sdf_en_failed;
	}

	if (sip_getSeqNumFromCseqHdr(&pMsgHeader, &dSeqNum,(SipError *) \
		&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode;
		sip_freeSipHeader(&pMsgHeader);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIsValidCseq( ): "
			"Failed to get Sequence number from CSeq header from Sip Message",\
			pError);	
#endif		
		return Sdf_en_failed;
	}
	sip_freeSipHeader(&pMsgHeader);

	/* 
	 * If a retransmitted ACK of lower Cseq appears after a fresh
	 * INVITE of higher Cseq has been recvd (For example after the first
	 * INVITE was rejected with a 401/407) it can be dropped 
	 */
	/*
 	* For an ACK, if the Cseq is lower than the dRemoteCseq,
	* it can be dropped. This may happen if the ACK to a 401
	* appears after a fresh INVITE with higher Cseq has been
	* received by the F/W. 
	*/

	if( (dMethodType == Sdf_en_ack) && \
		((pCallObj->pCallInfo->dState == Sdf_en_inviteReceived) || \
		 (pCallObj->pCommonInfo->dRemoteCseq > dSeqNum)))
	{
		/* Display on the call flow in this case. */
		*pDisplayCallFlow = 1;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_fn_uaIsValidCseq");
		return Sdf_en_discarded;
	}	

	/* 
	 * Check if the Cseq is higher. Note that for ACK and CANCEL, the Cseq 
	 * will be same as that in initial INVITE.
	 */
	if ((dMethodType == Sdf_en_ack) || (dMethodType == Sdf_en_cancel))
	{
		/* compare CSEQs of call object and sip message */
		if ( pCallObj->pCommonInfo->dRemoteCseq == dSeqNum )
		{
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
				"Exiting sdf_fn_uaIsValidCseq");	
			return Sdf_en_valid;
		}	
		else if (dSeqNum > pCallObj->pCommonInfo->dRemoteCseq)
		{
			*pRespCode = 400;
			sdf_fn_debug((Sdf_ty_s8bit*) \
					"SDF_DEBUG-Exiting sdf_fn_uaIsValidCseq");	
			return Sdf_en_invalid;
		}
		else
		{
			*pRespCode = 500;
			sdf_fn_debug((Sdf_ty_s8bit*) \
					"SDF_DEBUG-Exiting sdf_fn_uaIsValidCseq");	
			return Sdf_en_invalid;
		}
	}

	/* 
	 * Checks for PRACK (overlapping transactions) begin here.
	 */
#ifdef SIP_RPR	
	if (dMethodType == Sdf_en_prack)
	{
		/*
		 * To match a PRACK to a RPR the following are necessary:
		 * -----------------------------------------------------
		 *	 a. The Call-ID in PRACK matches the Call-ID in RPR
		 *	 b. The From in PRACK matches the From in RPR, including the tag
		 *	 c. The To in PRACK matches the To in RPR, including the tag
		 *	 d. The method in the RAck of PRACK matches the method in the 
		 *		CSeq of RPR
		 *	 e. The CSeq-num in the RAck matches the CSeq number in RPR
		 *	 f. The response-num in the RAck matches the RSeq value in RPR
		 *
		 * Matching
		 * --------
		 * 	 a, b and c and f are ensured before this API invocation itself.
		 *	 d and e are specfic only to PRACK and are handled below
		 */
		SipHeader dHeader;
		Sdf_ty_u32bit dRackCseq;
		Sdf_ty_s8bit *pRackMethod = Sdf_co_null;
		/*
		 * Get the method, response-num and CseqNum from the RAck header
		 */
		if (sip_getHeader(pMsg, SipHdrTypeRAck, &dHeader, \
			(SipError *) &(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaIsValidCseq( ): "
				"Failed to get  CSeq header from Sip Message",pError);	
#endif				
			return Sdf_en_failed;
		}
		if (sip_rpr_getCseqNumFromRAckHdr(&dHeader, &dRackCseq, \
			(SipError *) &(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
			sip_freeSipHeader(&dHeader);
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaIsValidCseq( ): "
				"Failed to get Cseq from RAck header",pError);	
#endif				
			return Sdf_en_failed;
		}
		if (sip_rpr_getMethodFromRAckHdr(&dHeader, &pRackMethod, \
			(SipError *) &(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
			sip_freeSipHeader(&dHeader);
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaIsValidCseq( ): "
				"Failed to get method from RAck header",pError);	
#endif				
			return Sdf_en_failed;
		}
		sip_freeSipHeader(&dHeader);
		
		/*
		 * Check 'e': Cseq check.
		 */
		if (pCallObj->pCommonInfo->dRemoteCseq != dRackCseq)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaIsValidCseq( ): "
				"Cseq of PRACK not matching RPR Cseq",pError);	
#endif				
			*pRespCode = 481;
			return Sdf_en_invalid;
		}
		
		/*
		 * Check 'd': Method in RAck header.
		 */
		if ((Sdf_mc_strcmp(pCallObj->pCommonInfo->pRPRMethod, \
			pRackMethod)) != 0)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaIsValidCseq( ): "
				"Method in the RAck header does not match method of "
				"RPR sent",pError);	
#endif					
			*pRespCode = 481;
			return Sdf_en_invalid;
		}
		return Sdf_en_valid;
	}
#endif
	/* 
	 * 	a) 	if Cseq is higher, it should be responded with 
	 * 		a 400 Bad Request.
	 * 	b) 	if Cseq is the same, it is a retransmission. 
	 * 		So it can be dropped.
	 * 	c) 	if Cseq is the lower, it should be responsed with
	 * 		a 500 Response. 
	 */
	
		/* 
	 * If an INVITE is received in the inviteReceived/reInviteReceived/
	 * inviteCancelled/reInviteCancelled state:
	 *
	 * 	a) 	if Cseq is higher, it should be responded with 
	 * 		a 400 Bad Request.
	 * 	b) 	if Cseq is the same, it is a retransmission. 
	 * 		So it can be dropped.
	 */

	if (((pCallObj->pCallInfo->dState == Sdf_en_inviteReceived) || \
		(pCallObj->pCallInfo->dState == Sdf_en_reInviteReceived) || \
		(pCallObj->pCallInfo->dState == Sdf_en_inviteCancelled) || \
		(pCallObj->pCallInfo->dState == Sdf_en_reInviteCancelled) || \
		(pCallObj->pCallInfo->dState == Sdf_en_finalSuccessSent) || \
		(pCallObj->pCallInfo->dState == Sdf_en_finalFailureSent) || \
		(pCallObj->pCallInfo->dState == Sdf_en_reInviteprovRespSent) || \
		(pCallObj->pCallInfo->dState == Sdf_en_reInviteFinalResponseSent)) && \
		(dMethodType == Sdf_en_invite))
	{
		/* 
		 * compare CSEQs of call object and sip message 
		 * pMethod has the method of the Cseq
		 */
		if ( pCallObj->pCommonInfo->dRemoteCseq == dSeqNum )
		{
			sdf_fn_debug((Sdf_ty_s8bit*)\
					"SDF_DEBUG - Exiting sdf_fn_uaIsValidCseq");	
			*pIsSameCseq = Sdf_co_true;
			return Sdf_en_discarded;
		}
		else
		{
            /* Start: CSR 1-4914419*/
			/*if (dSeqNum > pCallObj->pCommonInfo->dRemoteCseq)
				*pRespCode = 400;
			else if( dSeqNum < pCallObj->pCommonInfo->dRemoteCseq )*/
            /* End : CSR 1-4914419*/
				*pRespCode = 500;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaIsValidCseq ( ): "
				"Received message with invalid CSeq number",pError);	
#endif			
			return Sdf_en_invalid;
		}
	}


	/*
	 *  Check if Cseq is greater than { greatest amongst 
	 *	dRemoteCseq and dRprCseq }
	 */
	if (pCallObj->pCommonInfo->dRemoteCseq >= \
					pCallObj->pCommonInfo->dRemoteRegCseq)
		maxCseq = pCallObj->pCommonInfo->dRemoteCseq;
	else
		maxCseq = pCallObj->pCommonInfo->dRemoteRegCseq;
	

	if ( maxCseq < dSeqNum )
	{
		pError->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_fn_uaIsValidCseq");	
		return Sdf_en_valid;
	}
	else if(maxCseq > dSeqNum )
	{
		/* maxCseq > dSeqNum */
		*pRespCode = 500;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIsValidCseq( ): "
			"Received invalid Cseq",pError);	
#endif			
		return Sdf_en_invalid;
	}
	else if(maxCseq == dSeqNum)
	{
		*pRespCode = 400;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIsValidCseq( ): "
			"Received Duplicate Cseq",pError);	
#endif			
		*pIsSameCseq = Sdf_co_true;
		return Sdf_en_invalid;
	}


	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_fn_uaIsValidCseq");	
	return Sdf_en_valid;
}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaVerifyCaller
 **
 ** DESCRIPTION: This function is used to check within a call who the 
 **				 caller is and who the callee is for that particular call. 
 **				 This info is needed in the case while forming the To and 
 **				 From headers for the BYE request.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaVerifyCaller
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, 
	Sdf_st_error *pError)
#else
	(pObject, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaVerifyCaller");
	pError->errCode = Sdf_en_noUaError;

	if (pObject->pCommonInfo->pKey->dCaller == Sdf_en_caller)
	{
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_fn_uaVerifyCaller");
		return Sdf_co_success;
	}
	else
	{
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_fn_uaVerifyCaller");
		return Sdf_co_fail;
	}
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUpdateSeqNumbersInCallObj
 **
 ** DESCRIPTION: Updates remote party's sequence numbers in the Callobject's 
 **				 pCommonInfo structure. 
 **
 **				 Sequence numbers to be updated are
 **				 a) dRemoteCseq - The highest Cseq received so far.
 ** 			 b) dRemoteRseq - The highest (Rseq, Cseq) received so far.
 **				 c) dRemoteRegCseq - The highest Cseq PRACK/COMET 
 **			 		received so far.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateSeqNumbersInCallObj
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	SipMessage *pSipMsg,
	Sdf_st_error *pErr)
#else
	(pCallObj, pOverlapTransInfo, pSipMsg, pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	SipMessage *pSipMsg;
	Sdf_st_error *pErr;
#endif
{
	/*
	 * -----------------------------------------------------------------
	 * If a PRACK/COMET is received, update
	 * 		a) dRemoteRegCseq
	 * 		b) dCseq of the retrieved Sdf_st_overlapTransInfo structure.
	 * -----------------------------------------------------------------
	 * If an RPR is received, update
	 * 		a) dRemoteRseq
	 * 		b) dRseq of the retrieved Sdf_st_overlapTransInfo structure.
	 * -----------------------------------------------------------------
	 */
	en_SipMessageType dMessageType;
	SipHeader dCseqHeader;
	Sdf_ty_u32bit dCseq;
	Sdf_ty_s8bit *pMethod;
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
	Sdf_ty_messageType 	dMsgType = Sdf_en_unknownRequest;
	Sdf_st_msgInfo			dMessageInfo;


	(void)pOverlapTransInfo;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaUpdateSeqNumbersInCallObj");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSeqNumbersInCallObj( ): "
			"Invalid Call Object",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/*w534*/ (void)sdf_fn_uaGetMessageInfo( pSipMsg, &dMessageInfo, pErr);
	
	dTransactionType = sdf_fn_getTransactionType( pSipMsg, dMessageInfo, \
		   		Sdf_co_true, pErr);
	dMessageType = dMessageInfo.dReqRespType;
	/*
	 * Retrieve Cseq and method in the Cseq header from SipMessage
	 */
	if (sip_getHeader(pSipMsg, SipHdrTypeCseq,\
		&dCseqHeader, (SipError*)&(pErr->stkErrCode)) == SipFail)
	{ 
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSeqNumbersInCallObj( ): "
			"Failed to get Cseq header",pErr);
#endif		
		return Sdf_co_fail;
	}
	if (sip_getSeqNumFromCseqHdr(&dCseqHeader,&dCseq, \
		(SipError *) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSeqNumbersInCallObj( ): "
			"Failed to get cseq num from Cseq header",pErr);
#endif		
		sip_freeSipHeader(&dCseqHeader);
		return Sdf_co_fail;
	}
	if (sip_getMethodFromCseqHdr(&dCseqHeader, &pMethod, \
		(SipError *) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSeqNumbersInCallObj( ): "
			"Failed to get cseq method from Cseq header",pErr);
#endif		
		sip_freeSipHeader(&dCseqHeader);
		return Sdf_co_fail;
	}
	sip_freeSipHeader(&dCseqHeader);

	/*w534*/ (void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, pErr);
	
	/*
	 * Check if message is a request.
	 */
	if (dMessageType == SipMessageRequest)
	{
		Sdf_st_commonInfo *pCommInfo;

		if (sdf_ivk_uaGetCommonInfoFromCallObject(\
			pCallObj, &pCommInfo, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSeqNumbersInCallObj( ): "
				"Failed to get common info from call object",pErr);
#endif			
			return Sdf_co_fail;
		}
		/*
	 	 * If a PRACK/COMET is received, update
		 * a) dRemoteRegCseq
		 * b) dCseq of the retrieved Sdf_st_overlapTransInfo structure.
		 */
	
#ifdef SIP_RPR		
		if ( (dMsgType != Sdf_en_prack) && \
			( dMsgType != Sdf_en_comet) &&
			(dTransactionType != Sdf_en_RegularUasTransaction))
#else
		if (( dMsgType != Sdf_en_comet) &&
			(dTransactionType != Sdf_en_RegularUasTransaction))
#endif
		{
			pCommInfo->dRemoteCseq = dCseq;

			/* 
			 * Storing the method of the request recd in the common
			 * info. This will be used later so that the PRACK's RAck 
			 * header method can be tallied against the method of 
			 * the RPR. Do this for all requests other than ACK.
			 */
#ifdef SIP_RPR			
			if ( dMsgType != Sdf_en_ack)
			{
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&(pCommInfo->pRPRMethod), pErr);
				pCommInfo->pRPRMethod = Sdf_mc_strdupCallHandler(pMethod);
			}
#endif			
		}
		else
		{
			pCommInfo->dRemoteRegCseq = dCseq;
			/*
			 * pOverlapTransInfo will be NULL only if we receive a
			 * stray PRACK/COMET (which does not match to any existing
			 * call). HandleCall calls UpdateCallObject which tries to
			 * updateCseq numbers for the call object.
			 */
			if (pOverlapTransInfo != Sdf_co_null)
				pOverlapTransInfo->dRemoteCseq = dCseq;
		}

		(void)sdf_ivk_uaFreeCommonInfo(pCommInfo);

		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_fn_uaUpdateSeqNumbersInCallObj");
		return Sdf_co_success;
	} /* End of "if request" */
	else
	{
#ifdef SIP_RPR	
		SipHeader dTempHeader;
		Sdf_st_commonInfo *pCommInfo;
		Sdf_ty_u32bit dRseq;

		/* 
		 * Only if it is an RPR, update
	 	 * a) dRemoteRseq
		 * b) dRseq of the retrieved Sdf_st_overlapTransInfo structure.
		 * c) pRPRMethod (so that it can be used to construct PRACK
		 */
		if (sdf_fn_uaIsMessageRPR(pSipMsg, pErr) == 0)
		{
			if (sip_getHeader(pSipMsg, SipHdrTypeRSeq,\
				&dTempHeader, (SipError*)&(pErr->stkErrCode)) \
				== SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSeqNumbersInCallObj( ): "
					"Failed to get Rseq header",pErr);
#endif				
				return Sdf_co_fail;
			}
			if (sip_rpr_getRespNumFromRSeqHdr(&dTempHeader,\
				&dRseq,(SipError *) &(pErr->stkErrCode)) \
				== SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSeqNumbersInCallObj( ): "
					"Failed to get rseq num from Rseq header",pErr);
#endif				
				sip_freeSipHeader(&dTempHeader);
				return Sdf_co_fail;
			}

			if (sdf_ivk_uaGetCommonInfoFromCallObject(\
				pCallObj, &pCommInfo, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSeqNumbersInCallObj( ): "
					"Failed to get common info from call object",pErr);
#endif				
				(void)sdf_ivk_uaFreeCommonInfo(pCommInfo);
				sip_freeSipHeader(&dTempHeader);
				return Sdf_co_fail;
			}

			(pCommInfo->dRemoteRseq).dRseq = dRseq;
			(pCommInfo->dRemoteRseq).dCorrelationCseq = dCseq;
			if (pOverlapTransInfo != Sdf_co_null)
				pOverlapTransInfo->dRseq = dRseq;

			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)&(pCommInfo->pRPRMethod), pErr);
			pCommInfo->pRPRMethod = Sdf_mc_strdupCallHandler(pMethod);
			
			(void)sdf_ivk_uaFreeCommonInfo(pCommInfo);

			/* Free the local reference */
			sip_freeSipHeader(&dTempHeader);

			pErr->errCode = Sdf_en_noUaError;
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
				"Exiting sdf_fn_uaUpdateSeqNumbersInCallObj");
			return Sdf_co_success;
		} /* End of "if message is an RPR */
#endif		
	} /* End of "if not request */
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaUpdateSeqNumbersInCallObj");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaAddHdrListToProcessedHdrsInCallObj
 **
 ** DESCRIPTION: This function adds the list of headers in the SipMessage 
 **				 to the Processed headers list of the call object.
 **
 ******************************************************************************/
Sdf_ty_retVal  sdf_fn_uaAddHdrListToProcessedHdrsInCallObj
#ifdef ANSI_PROTO
	(Sdf_st_transaction *pTransaction,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	SipMessage *pSipMsg,
	en_HeaderType dHdrType,
	Sdf_st_error *pErr)
#else
	(pTransaction, pOverlapTransInfo, pSipMsg, dHdrType, pErr)
	Sdf_st_transaction *pTransaction;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	SipMessage *pSipMsg;
	en_HeaderType dHdrType;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dListSize=0, dIndex=0;
	Sdf_st_headerList *pTempElement = Sdf_co_null;
	Sdf_ty_slist *pProcessedHeaderList=Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaAddHdrListToProcessedHdrsInCallObj");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;
   
#endif	/* End of param validation ifdef */

	/*
	 * Get count to see if the header is present in the SipMessage at all
	 */
	/*w534*/ (void)sip_getHeaderCount(pSipMsg, dHdrType, &dListSize,\
		(SipError *)&(pErr->stkErrCode));

	if (dListSize > 0)
	{
		/* 
		 * Header is present. Initialize a new headerlist element 
		 */
		/* Initialise the header list with dHdrType not with SipHdrTypeVia
		 */
		if (sdf_ivk_uaInitHeaderList(&pTempElement, \
			dHdrType, Sdf_co_null, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaAddHdrListToProcessedHdrsInCallObj( ): "
				"Failed to init a headerlist structure",pErr);
#endif
			return Sdf_co_fail;
		}
	}
#ifdef SDF_LINT
  else
	{		
	  	pErr->errCode = Sdf_en_noUaError;
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - " 
				"Exiting sdf_fn_uaAddHdrListToProcessedHdrsInCallObj");
			return Sdf_co_success;
	}	
#endif
		
	for(dIndex=0; dIndex<dListSize; dIndex++)
	{
		SipHeader *pDestHeader = Sdf_co_null;

		if (sip_initSipHeader(&pDestHeader, SipHdrTypeAny, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaAddHdrListToProcessedHdrsInCallObj():"
				"Failed to init dest header",pErr);
#endif			
			(void)sdf_ivk_uaFreeHeaderList(pTempElement);
			return Sdf_co_fail;
		}
		pDestHeader->dType = dHdrType;

		if (sip_getHeaderAtIndex(pSipMsg, dHdrType , pDestHeader, \
			dIndex, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaAddHdrListToProcessedHdrsInCallObj():"
				"Failed to get header from sipmessage",pErr);
#endif			
			(void)sdf_ivk_uaFreeHeaderList(pTempElement);
			return Sdf_co_fail;
		}

		/*
		 * Insert the cloned header to the headerlist structure.
		 */
     
    if (sdf_listAppend(&(pTempElement->slHeaders), \
			(Sdf_ty_pvoid)(pDestHeader), pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaAddHdrListToProcessedHdrsInCallObj( ): "
				"Failed to insert a header into the headerlist",pErr);
#endif			
			sip_freeSipHeader(pDestHeader);
			(void)sdf_ivk_uaFreeHeaderList(pTempElement);
			return Sdf_co_fail;
		}
	} /* End of the for loop for the list of headers */


	/*
	 * Decide headerlist structure should go into which processed headers
	 * list: that of the overlaptransinfo or that of the call object.
	 */
#ifdef SDF_LINT
	if(pOverlapTransInfo == Sdf_co_null)
	{		
    if (pTransaction != Sdf_co_null)
		  pProcessedHeaderList = &(pTransaction->slProcessedHeaders);
	}	
	else
	{		
    if (pOverlapTransInfo != Sdf_co_null)
		  pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);
	}	
#else
	if(pOverlapTransInfo == Sdf_co_null)
		pProcessedHeaderList = &(pTransaction->slProcessedHeaders);
	else
		pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);
#endif

	/*
	 * Only if a headerlist element was created and filled, append it to
	 * the processed headers list.
	 */
	if (dListSize > 0)
	{
		if (sdf_listAppend(pProcessedHeaderList, pTempElement,pErr) == \
				Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaAddHdrListToProcessedHdrsInCallObj:"
				"Failed to append a headerlist into the messageinfo",pErr);
#endif			
			(void)sdf_ivk_uaFreeHeaderList(pTempElement);
			return Sdf_co_fail;
		}
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - " 
		"Exiting sdf_fn_uaAddHdrListToProcessedHdrsInCallObj");
	return Sdf_co_success;
}

#ifdef SDF_AUTHENTICATE
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaAddAuthHdrListToCallObj
 **
 ** DESCRIPTION: This function adds the list of authorization headers in the 
 **				 SipMessage to the Processed headers list of call object
 **
 ******************************************************************************/
Sdf_ty_retVal  sdf_fn_uaAddAuthHdrListToCallObj
#ifdef ANSI_PROTO
	(Sdf_st_transaction *pTransaction,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	SipMessage *pSipMsg,
	Sdf_st_error *pErr)
#else
	(pTransaction, pOverlapTransInfo, pSipMsg, pErr)
	Sdf_st_transaction *pTransaction;
	Sdf_st_msgInfo *pOverlapTransInfo;
	SipMessage *pSipMsg;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dListSize=0, dIndex=0;
	Sdf_st_headerList *pTempElement = Sdf_co_null;
	Sdf_ty_s8bit *pScheme = Sdf_co_null;
	SipGenericChallenge *pChallenge = Sdf_co_null;
	Sdf_ty_slist *pProcessedHeaderList = Sdf_co_null; 
	Sdf_ty_slist *pUnprocessedHeaderList = Sdf_co_null;
	Sdf_st_error	dLocalErr;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaAddAuthHdrListToCallObj");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pErr == Sdf_co_null)
		return Sdf_co_fail;
#endif	/* End of param validation ifdef */
	
	/* ----------------------------------------------------------
	 * 		AUTHORIZATION handling 
	 * ----------------------------------------------------------
	 * IF any Authorization headers exist in the request message, 
	 * copy them into 
	 * 		a) The ProcessedHeaders list if scheme = Basic/Digest
	 * 		b) The UnprocessedHeaders list otherwise.
	 */

	/*w534*/ (void)sip_getHeaderCount(pSipMsg, SipHdrTypeAuthorization, \
		&dListSize, (SipError*)&(pErr->stkErrCode));

	if (dListSize > 0)
	{
		/* 
		 * Initialize a new element and appened it to the list.
		 */
		if (sdf_ivk_uaInitHeaderList(&pTempElement, \
			SipHdrTypeAuthorization, Sdf_co_null, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthHdrListToCallObj( ): "
				"Failed to init headerlist of authorization headers",pErr);
#endif			
			return Sdf_co_fail;
		}
	}
#ifdef SDF_LINT
  else
	{
    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaAddAuthHdrListToCallObj");
    return Sdf_co_success;
	}
#endif

	for(dIndex=0; dIndex<dListSize; dIndex++)
	{
		SipHeader *pDestHeader = Sdf_co_null;

		if (sip_initSipHeader(&pDestHeader,SipHdrTypeAny,\
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
			(void)sdf_ivk_uaFreeHeaderList(pTempElement);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthHdrListToCallObj( ): "
				"Failed to init a Authorization header",pErr);
#endif			
			return Sdf_co_fail;
		}

		pDestHeader->dType = SipHdrTypeAuthorization;

		if (sip_getHeaderAtIndex(pSipMsg, SipHdrTypeAuthorization, \
			pDestHeader, dIndex, (SipError *)&(pErr->stkErrCode)) \
			== SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthHdrListToCallObj( ): "
				"Failed to get authorization header from SipMessage",pErr);
#endif			
			(void)sdf_ivk_uaFreeHeaderList(pTempElement);
			return Sdf_co_fail;
		}
		if (dIndex == 0)
		{
			/* 
			 * Pick the scheme (Basic/Digest/etc) from the first 
			 * Authorization header
			 */
			SipGenericCredential *pGenericCredential;
			en_CredentialType dCredType;

			if (sip_getCredentialsFromAuthorizationHdr(\
				pDestHeader, &pGenericCredential, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
				sip_freeSipHeader(pDestHeader);
				(void)sdf_ivk_uaFreeHeaderList(pTempElement);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthHdrListToCallObj( ): "
					"Failed to get credentials from authorization header", \
					pErr);
#endif				
				return Sdf_co_fail;
			}
			if (sip_getCredentialTypeFromCredential(pGenericCredential, \
				&dCredType, (SipError *)&(pErr->stkErrCode)) == SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
				sip_freeSipGenericCredential(pGenericCredential);
				sip_freeSipHeader(pDestHeader);
				(void)sdf_ivk_uaFreeHeaderList(pTempElement);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthHdrListToCallObj( ): "
					"Failed to get credential type from credentials",pErr);
#endif				
				return Sdf_co_fail;
			}
			if (dCredType == SipCredBasic)
			{
				pScheme = Sdf_mc_strdupCallHandler("Basic");
			}
			else
			{
				Sdf_ty_s8bit *pTempScheme;
				if (sip_getChallengeFromCredential(\
					pGenericCredential, &pChallenge, \
					(SipError *)&(pErr->stkErrCode)) == SipFail)
				{
					pErr->errCode = pErr->stkErrCode;
					sip_freeSipGenericCredential(pGenericCredential);
					sip_freeSipHeader(pDestHeader);
					(void)sdf_ivk_uaFreeHeaderList(pTempElement);
#ifdef SDF_ERROR					
					sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
						(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthHdrListToCallObj( ): "
						"Failed to get challenge from credentials",pErr);
#endif					
					return Sdf_co_fail;
				}

				/* 	
				 * Get the scheme used in authorization (Basic/Digest) 
				 */
				if (sip_getSchemeFromChallenge(pChallenge,&pTempScheme,\
					(SipError*)&(pErr->stkErrCode)) == SipFail)
				{
					pErr->errCode = pErr->stkErrCode;
					sip_freeSipGenericChallenge(pChallenge);
					sip_freeSipGenericCredential(pGenericCredential);
					sip_freeSipHeader(pDestHeader);
					(void)sdf_ivk_uaFreeHeaderList(pTempElement);
#ifdef SDF_ERROR					
					sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
						(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthHdrListToCallObj( ): "
						"Failed to get scheme from challenge",pErr);
#endif					
					return Sdf_co_fail;
				}
				pScheme = Sdf_mc_strdupCallHandler(pTempScheme);
				/* Freeing the Local Reference */
				sip_freeSipGenericChallenge(pChallenge);
			}
			sip_freeSipGenericCredential(pGenericCredential);
		}

		if (sdf_listAppend(&(pTempElement->slHeaders), \
			(Sdf_ty_pvoid)(pDestHeader),pErr) == Sdf_co_fail)
		{
			sip_freeSipHeader(pDestHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pScheme,\
				&dLocalErr);
			(void)sdf_ivk_uaFreeHeaderList(pTempElement);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthHdrListToCallObj( ): "
				"Failed to add Authorization header to headerlist",pErr);
#endif			
			return Sdf_co_fail;
		}
	} /* End of the for loop  */

	/*
	 * Choose the headerlist that is a part of the overlaptransinfo
	 * if the pOverlapTransInfo variable is not NULL. Else API to work on
	 * the call object's headerlist.
	 */
#ifdef SDF_LINT
	if(pOverlapTransInfo == Sdf_co_null)
	{		
			if(pTransaction != Sdf_co_null)
			{
				pProcessedHeaderList = &(pTransaction->slProcessedHeaders);
				pUnprocessedHeaderList = &(pTransaction->slUnprocessedHeaders);
			}
	}		
	else
	{		
			if(pOverlapTransInfo != Sdf_co_null)
			{
				pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);
				pUnprocessedHeaderList = &(pOverlapTransInfo->slUnprocessedHeaders);
			}
	}		
#else	
	if(pOverlapTransInfo == Sdf_co_null)
	{
		pProcessedHeaderList = &(pTransaction->slProcessedHeaders);
		pUnprocessedHeaderList = &(pTransaction->slUnprocessedHeaders);
	}
	else
	{
		pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);
		pUnprocessedHeaderList = &(pOverlapTransInfo->slUnprocessedHeaders);
	}
#endif	
	/* 	
	 * Now the headerList element is ready. Depending on the scheme, it 
	 * has to be appended to ProcessedHeaders/UnprocessedHeaders.
	 */
	if (dListSize > 0)
	{
		if ((Sdf_mc_strcasecmp(pScheme, "Basic") == 0) ||\
			(Sdf_mc_strcasecmp(pScheme, "Digest") == 0) )
		{
			if (sdf_listAppend(pProcessedHeaderList, \
				pTempElement,pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthHdrListToCallObj( ): "
					"Failed to add auth headerlist to Processed headers", \
					pErr);
#endif				
				(void)sdf_ivk_uaFreeHeaderList(pTempElement);
				return Sdf_co_fail;
			}
		}
		else
		{
			if (sdf_listAppend(pUnprocessedHeaderList, pTempElement, \
					pErr)== Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthHdrListToCallObj( ): "
					"Failed to add auth headerlist to UnprocessedHeaders", \
					pErr);
#endif				
				(void)sdf_ivk_uaFreeHeaderList(pTempElement);
				return Sdf_co_fail;
			}
		}
	}
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pScheme,\
		pErr);
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaAddAuthHdrListToCallObj");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaAddAuthenticateHdrListToCallObj
 **
 ** DESCRIPTION: This function adds the list of headers in the SipMessage 
 **				 to the ProcessedHeaderlist of either the overlaptransinfo 
 **				 or the call object (former if the overlaptransinfo is not 
 **				 NULL and latter if it is NULL). Valid header types as input 
 **				 to this function are SipHdrTypeProxyAuthenticate and 
 **				 SipHdrTypeWwwAuthenticate
 **
 ******************************************************************************/
Sdf_ty_retVal  sdf_fn_uaAddAuthenticateHdrListToCallObj
#ifdef ANSI_PROTO
	(Sdf_st_transaction *pTransaction,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	SipMessage *pSipMsg,
	en_HeaderType dHdrType,
	Sdf_st_error *pErr)
#else
	(pTransaction, pOverlapTransInfo, pSipMsg, dHdrType, pErr)
	Sdf_st_transaction *pTransaction;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	SipMessage *pSipMsg;
	en_HeaderType dHdrType;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dListSize=0, dIndex=0;
	Sdf_st_headerList *pProcessedHeadersElement = Sdf_co_null;
	Sdf_st_headerList *pUnProcessedHeadersElement = Sdf_co_null;
	SipGenericChallenge *pChallenge = Sdf_co_null;
	Sdf_ty_slist *pProcessedHeaderList=Sdf_co_null,\
		*pUnprocessedHeaderList=Sdf_co_null;
	Sdf_st_error	 dLocalErr;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaAddAuthenticateHdrListToCallObj");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pErr == Sdf_co_null)
		return Sdf_co_fail;
#endif	/* End of param validation ifdef */

	/* 
	 * Get the count of the proxy-authenticate or www-authenticate
	 * header.
	 */
	/*w534*/ (void)sip_getHeaderCount(pSipMsg, dHdrType, \
		&dListSize,(SipError *)&(pErr->stkErrCode));
	
	if (dListSize > 0)
	{
		/* 
		 * If size is non-zero, then the header was recd in the SipMessage.
		 * Initialize headerlist elements to store the headers
		 */
		if (sdf_ivk_uaInitHeaderList(&pProcessedHeadersElement, \
			dHdrType, Sdf_co_null, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthenticateHdrListToCallObj( ): "
				"Failed to Initialize HeaderList for ProcessedHeaders",pErr);
#endif		
			return Sdf_co_fail;
		}
		if (sdf_ivk_uaInitHeaderList(&pUnProcessedHeadersElement, \
			dHdrType, Sdf_co_null, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthenticateHdrListToCallObj( ): "
				"Failed to Initialize HeaderList for UnProcessedHeaders",pErr);
#endif		
			return Sdf_co_fail;
		}
	}
	else
	{
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Exiting sdf_fn_uaAddAuthenticateHdrListToCallObj");
		return Sdf_co_success;
	}

	for(dIndex=0; dIndex<dListSize; dIndex++)
	{
		SipHeader *pDestHeader;
		Sdf_ty_s8bit *pScheme = Sdf_co_null;

		/*
		 * Get the authenticate header that was recd from the SipMessage.
		 */
		if (sip_initSipHeader(&pDestHeader, SipHdrTypeAny, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthenticateHdrListToCallObj( ): "
				"Failed to initialize SipHeader",pErr);
#endif		
			pErr->errCode=Sdf_en_headerManipulationError;
			(void)sdf_ivk_uaFreeHeaderList(pProcessedHeadersElement);
			(void)sdf_ivk_uaFreeHeaderList(pUnProcessedHeadersElement);
			return Sdf_co_fail;
		}

		pDestHeader->dType = dHdrType;

		if (sip_getHeaderAtIndex(pSipMsg, dHdrType, pDestHeader, \
			dIndex, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthenticateHdrListToCallObj( ): "
				"Failed to get header at index from Incoming SIP message",pErr);
#endif		
			pErr->errCode=Sdf_en_headerManipulationError;
			sip_freeSipHeader(pDestHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)&pDestHeader,&dLocalErr);
			(void)sdf_ivk_uaFreeHeaderList(pProcessedHeadersElement);
			(void)sdf_ivk_uaFreeHeaderList(pUnProcessedHeadersElement);
			return Sdf_co_fail;
		}

		/* 
		 * Pick the challenge. If it is Basic or Digest, append this to the
		 * pProcessedHeadersElement. Else append it to the
		 * pUnProcessedHeadersElement 
		 */
		if (SipHdrTypeWwwAuthenticate == dHdrType)
		{
			if (sip_getChallengeFromWwwAuthenticateHdr(pDestHeader, \
				&pChallenge, (SipError *)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthenticateHdrListToCallObj()"
					": Failed to get Challenge from WWW-Authenticate",pErr);
#endif		
				pErr->errCode=Sdf_en_headerManipulationError;
				sip_freeSipHeader(pDestHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
					&pDestHeader,&dLocalErr);
				(void)sdf_ivk_uaFreeHeaderList(pProcessedHeadersElement);
				(void)sdf_ivk_uaFreeHeaderList(pUnProcessedHeadersElement);
				return Sdf_co_fail;
			}
		}
		else if (SipHdrTypeProxyAuthenticate == dHdrType)
		{
			if (sip_getChallengeFromProxyAuthenticateHdr(pDestHeader, \
				&pChallenge,(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthenticateHdrListToCallObj()"
					": Failed to get Challenge from Proxy-Authenticate",pErr);
#endif		
				pErr->errCode=Sdf_en_headerManipulationError;
				sip_freeSipHeader(pDestHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
					&pDestHeader,&dLocalErr);
				(void)sdf_ivk_uaFreeHeaderList(pProcessedHeadersElement);
				(void)sdf_ivk_uaFreeHeaderList(pUnProcessedHeadersElement);
				return Sdf_co_fail;
			}
		}

		/* 
		 * Get the scheme for authentication (Basic/Digest)
		 */
		if (sip_getSchemeFromChallenge(pChallenge, &pScheme, \
			(SipError *)&(pErr->stkErrCode))== SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthenticateHdrListToCallObj()"
				": Failed to get scheme from challenge",pErr);
#endif		
			pErr->errCode=Sdf_en_headerManipulationError;
			sip_freeSipHeader(pDestHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pDestHeader,\
					&dLocalErr);
			(void)sdf_ivk_uaFreeHeaderList(pProcessedHeadersElement);
			(void)sdf_ivk_uaFreeHeaderList(pUnProcessedHeadersElement);
			return Sdf_co_fail;
		}
		/* 
		 * If scheme is Basic or Digest, then the header is
		 * added to the pProcessedHeadersElement. If not, it is added to
		 * the pUnProcessedHeadersElement.
		 */
		if ((Sdf_mc_strcasecmp(pScheme, "Basic") == 0) ||\
			(Sdf_mc_strcasecmp(pScheme, "Digest") == 0) )
		{
			if (sdf_listAppend(&(pProcessedHeadersElement->slHeaders), \
				(Sdf_ty_pvoid)(pDestHeader),pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthenticateHdrListToCallObj()"
					": Failed to append SipHeader to ProcessedHeader list",pErr);
#endif		
				pErr->errCode=Sdf_en_headerManipulationError;
				sip_freeSipHeader(pDestHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&pDestHeader,&dLocalErr);
				sip_freeSipHeader(pDestHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&pDestHeader, &dLocalErr);
				(void)sdf_ivk_uaFreeHeaderList(pProcessedHeadersElement);
				(void)sdf_ivk_uaFreeHeaderList(pUnProcessedHeadersElement);
				return Sdf_co_fail;
			}
		}
		else
		{
			if (sdf_listAppend(&(pUnProcessedHeadersElement->slHeaders), \
				(Sdf_ty_pvoid)(pDestHeader),pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaAddAuthenticateHdrListToCallObj()"
					": Failed to append SipHeader to UnProcessedHeader",pErr);
#endif		
				pErr->errCode=Sdf_en_headerManipulationError;
				sip_freeSipHeader(pDestHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&pDestHeader,&dLocalErr);
				sip_freeSipHeader(pDestHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&pDestHeader, &dLocalErr);
				(void)sdf_ivk_uaFreeHeaderList(pProcessedHeadersElement);
				(void)sdf_ivk_uaFreeHeaderList(pUnProcessedHeadersElement);
				return Sdf_co_fail;
			}
		}
	}

	/* Freeing the Local Reference */
	sip_freeSipGenericChallenge(pChallenge);
	/* 
	 * Now the headerList elements are ready. Append them to the respective
	 * lists. 
	 */
#ifdef SDF_LINT
	if(pOverlapTransInfo == Sdf_co_null)
	{		
			if(pTransaction != Sdf_co_null)
			{
				pProcessedHeaderList = &(pTransaction->slProcessedHeaders);
				pUnprocessedHeaderList = &(pTransaction->slUnprocessedHeaders);
			}
	}		
	else
	{		
			if(pOverlapTransInfo != Sdf_co_null)
			{
				pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);
				pUnprocessedHeaderList = &(pOverlapTransInfo->slUnprocessedHeaders);
			}
	}		
#else	
	if(pOverlapTransInfo == Sdf_co_null)
	{
		pProcessedHeaderList = &(pTransaction->slProcessedHeaders);
		pUnprocessedHeaderList = &(pTransaction->slUnprocessedHeaders);
	}
	else
	{
		pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);
		pUnprocessedHeaderList = &(pOverlapTransInfo->slUnprocessedHeaders);
	}
#endif
	dListSize = 0;
	(void)sdf_listSizeOf(&(pProcessedHeadersElement->slHeaders), &dListSize, pErr);

	if (dListSize == 0)
	{
		/*
		 * No Processed Headers. Free the pProcessedHeadersElement
		 */
		(void)sdf_ivk_uaFreeHeaderList(pProcessedHeadersElement);
	}
	else
	{
		if (sdf_listAppend(pProcessedHeaderList, \
			pProcessedHeadersElement, pErr) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeHeaderList(pProcessedHeadersElement);
			(void)sdf_ivk_uaFreeHeaderList(pUnProcessedHeadersElement);
			return Sdf_co_fail;
		}
	}

	dListSize = 0;

	(void)sdf_listSizeOf(&(pUnProcessedHeadersElement->slHeaders), &dListSize, \
		pErr);

	if (dListSize == 0)
	{
		/*
		 * No UnProcessed Headers. Free the pUnProcessedHeadersElement
		 */
		(void)sdf_ivk_uaFreeHeaderList(pUnProcessedHeadersElement);
	}
	else
	{
		if (sdf_listAppend(pUnprocessedHeaderList, pUnProcessedHeadersElement, \
			pErr) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeHeaderList(pProcessedHeadersElement);
			(void)sdf_ivk_uaFreeHeaderList(pUnProcessedHeadersElement);
			return Sdf_co_fail;
		}
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaAddAuthenticateHdrListToCallObj");
	return Sdf_co_success;
}
#endif /* SDF_AUTHENTICATE */


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCreateRoutePathInCallObj
 **
 ** DESCRIPTION: This function creates the Route Header list from the 
 **				 incoming SipMessage and adds it to the Processed Headers list 
 **				 of the call object/Overlaptransaction 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCreateRoutePathInCallObj 
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_msgInfo		dMessageInfo,
	SipMessage 	 *pSipMessage,
	Sdf_st_error *pErr)
#else
	(pCallObj, dMessageInfo, pSipMessage, pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_msgInfo	 dMessageInfo;
	SipMessage 	 	*pSipMessage;
	Sdf_st_error	*pErr;
#endif
{
	Sdf_st_headerList 	*pRouteList = Sdf_co_null;
	Sdf_ty_u32bit 		dCount=0;

  Sdf_ty_s16bit		dIndex=0;

	en_SipMessageType 	dMessageType;
	SipHeader 			dTempHeader;
	Sdf_ty_messageType 	dMethodType;
	SipHeader			*pRouteHeader = Sdf_co_null;
	SipAddrSpec 		*pAddrSpec = Sdf_co_null;
	Sdf_ty_s8bit 		*pDispName = Sdf_co_null;
	Sdf_ty_bool			dMethodAllowed = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaCreateRoutePathInCallObj");
	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
		return Sdf_co_fail;

	if (pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCreateRoutePathInCallObj( ): "
			"Invalid param value: Call Object",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	dMethodType = dMessageInfo.dMsgType;

    if(dMethodType == Sdf_en_invite || dMessageInfo.dMsgType == Sdf_en_update)
		dMethodAllowed = Sdf_co_true;
#ifdef SIP_IMPP
  /* SUBSCRIBE & NOTIFY are target refresh requests. Pls refer the following
   * links:
   * http://www.sipwg.org/show_bug.cgi?id=699
   * http://www1.ietf.org/mail-archive/web/sip/current/msg06960.html
   */
	else if ((dMethodType == Sdf_en_subscribe) || 
           (dMethodType == Sdf_en_notify))
		dMethodAllowed = Sdf_co_true;
#endif
#ifdef SDF_REFER
	else if (dMethodType == Sdf_en_refer)
		dMethodAllowed = Sdf_co_true;
#endif
	
	if(dMethodAllowed == Sdf_co_true)
	{
		if (sdf_ivk_uaInitHeaderList(&pRouteList, \
			SipHdrTypeRoute, (Sdf_ty_s8bit *)"uat-routeset", pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaCreateRoutePathInCallObj( ): "
				"Error initialising Header",pErr);
#endif			
			return Sdf_co_fail;
		}
		
		/*w534*/ (void)sip_getHeaderCount(pSipMessage, SipHdrTypeRecordRoute,\
			&dCount,(SipError *)&(pErr->stkErrCode));

		if (dCount != 0)
		{
			dMessageType = dMessageInfo.dReqRespType;
			/* 
			 * Reverse the record routes into the route list only if a 
			 * Response Message is received (UAC end). Else, copy the 
			 * Record Routes into Routes as they are (UAS end).
			 */
			if (dMessageType == SipMessageResponse) 
			{
				Sdf_ty_bool dCreateRoute=Sdf_co_false;
				Sdf_ty_u16bit dRespCode;

				dRespCode = dMessageInfo.dResponse;

				if ((dRespCode >= 200) && (dRespCode < 300))
					dCreateRoute = Sdf_co_true;	

				if (sdf_fn_uaIsMessageRPR(pSipMessage, pErr) == 0)
					dCreateRoute = Sdf_co_true;	
				else if((dRespCode > 100) && (dRespCode < 200))
					pCallObj->pCommonInfo->dPreloadedSet=Sdf_co_true;


				if(dCreateRoute ==Sdf_co_true)
				{
					for(dIndex=dCount-1; dIndex>=0; dIndex--)
					{
						SipHeader *pDestHeader;

						if (sip_getHeaderAtIndex(pSipMessage, \
							SipHdrTypeRecordRoute, &dTempHeader, \
							dIndex,(SipError *)&(pErr->stkErrCode)) == SipFail) 
						{
							pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR					
							sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
								(Sdf_ty_s8bit *) \
								"sdf_fn_uaCreateRoutePathInCallObj( ): "
								"Error getting Record Route header",pErr);
#endif					
							(void)sdf_ivk_uaFreeHeaderList(pRouteList);
							return Sdf_co_fail;
						}
						if (sip_initSipHeader(&pDestHeader, SipHdrTypeRoute, \
							(SipError *)&(pErr->stkErrCode)) == SipFail)
						{
							pErr->errCode = pErr->stkErrCode;
							sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR					
							sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
								(Sdf_ty_s8bit *) \
								"sdf_fn_uaCreateRoutePathInCallObj( ): "
								"Error initialising Header",pErr);
#endif					
							(void)sdf_ivk_uaFreeHeaderList(pRouteList);
							return Sdf_co_fail;
						}
						dTempHeader.dType = SipHdrTypeRoute;

						if (sip_cloneSipHeader(pDestHeader, &dTempHeader, \
							(SipError *)&(pErr->stkErrCode)) == SipFail)
						{
							pErr->errCode = pErr->stkErrCode;
							sip_freeSipHeader(&dTempHeader);
							sip_freeSipHeader(pDestHeader);
							(void)sdf_ivk_uaFreeHeaderList(pRouteList);
#ifdef SDF_ERROR					
							sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
								(Sdf_ty_s8bit *) \
								"sdf_fn_uaCreateRoutePathInCallObj( ): "
								"Error cloning Header",pErr);
#endif					
							return Sdf_co_fail;
						}
						if (sdf_listInsertAt(&(pRouteList->slHeaders), \
							(dCount-1)-dIndex, (Sdf_ty_pvoid)pDestHeader, \
							pErr) == Sdf_co_fail)
						{
							sip_freeSipHeader(&dTempHeader);
							sip_freeSipHeader(pDestHeader);
							(void)sdf_ivk_uaFreeHeaderList(pRouteList);
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
								(Sdf_ty_s8bit *) \
								"sdf_fn_uaCreateRoutePathInCallObj( ): "
								"Error inserting Header",pErr);
#endif					
							return Sdf_co_fail;
						}
						/* Free the local reference */
						sip_freeSipHeader(&dTempHeader);
					}
				}
			}	
			else
			{
				for(dIndex=0; dIndex< (Sdf_ty_s32bit)dCount; dIndex++)
				{
					SipHeader *pDestHeader;

					if (sip_getHeaderAtIndex(pSipMessage, \
						SipHdrTypeRecordRoute, &dTempHeader, dIndex,\
						(SipError *)&(pErr->stkErrCode)) == SipFail)
					{
						pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit *)\
							"sdf_fn_uaCreateRoutePathInCallObj( ): "
							"Error getting Header",pErr);
#endif					
						(void)sdf_ivk_uaFreeHeaderList(pRouteList);
						return Sdf_co_fail;
					}
					if (sip_initSipHeader(&pDestHeader, SipHdrTypeRoute, \
						(SipError *)&(pErr->stkErrCode)) == SipFail)
					{	
						pErr->errCode = pErr->stkErrCode;
						sip_freeSipHeader(&dTempHeader);
						(void)sdf_ivk_uaFreeHeaderList(pRouteList);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit *)\
							"sdf_fn_uaCreateRoutePathInCallObj( ): "
							"Error initing Header",pErr);
#endif					
						return Sdf_co_fail;
					}
					dTempHeader.dType = SipHdrTypeRoute;

					if (sip_cloneSipHeader(pDestHeader, &dTempHeader, \
						(SipError *)&(pErr->stkErrCode)) == SipFail)
					{
						pErr->errCode = pErr->stkErrCode;
						sip_freeSipHeader(&dTempHeader);
						sip_freeSipHeader(pDestHeader);
						(void)sdf_ivk_uaFreeHeaderList(pRouteList);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit *)\
							"sdf_fn_uaCreateRoutePathInCallObj( ): "
							"Error cloning Header",pErr);
#endif					
						return Sdf_co_fail;
					}
					if (sdf_listAppend(&(pRouteList->slHeaders), \
						(Sdf_ty_pvoid)pDestHeader, pErr) == Sdf_co_fail)
					{
						sip_freeSipHeader(&dTempHeader);
						sip_freeSipHeader(pDestHeader);
						(void)sdf_ivk_uaFreeHeaderList(pRouteList);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit *)\
							"sdf_fn_uaCreateRoutePathInCallObj( ): "
							"Error inserting Header",pErr);
#endif					
						return Sdf_co_fail;
					}
					sip_freeSipHeader(&dTempHeader);
				}
			}
		}
		/* Append the remote party conatct or From as the last entry 
		 * to the route set
		 */
		if (sip_getHeaderAtIndex(pSipMessage, SipHdrTypeContactAny, \
			&dTempHeader, 0, (SipError *)&(pErr->stkErrCode)) != SipFail)
		{
			/*w534*/ (void)sip_getAddrSpecFromContactHdr(&dTempHeader, \
				&pAddrSpec, (SipError *)&(pErr->stkErrCode));
			
			/*w534*/ (void)sip_getDispNameFromContactHdr(&dTempHeader,\
				&pDispName, (SipError *)&(pErr->stkErrCode));
		}
		else
		{
			/* If No Contact is present then set the Request URI sent in 
			 * its corresponding Invite Request. Which can be got from the
			 * stored Request URI present in CommonInfo.
			 */
			if ((pErr->stkErrCode == Sdf_en_noExistError) ||\
				(pErr->stkErrCode == Sdf_en_invalidIndexError))
			{
				/*w534*/ (void)sip_getHeader(pSipMessage, SipHdrTypeFrom, \
					&dTempHeader, (SipError *)&(pErr->stkErrCode));

				/*w534*/ (void)sip_getDispNameFromFromHdr(&dTempHeader,\
					&pDispName, (SipError *)&(pErr->stkErrCode));

				if( pCallObj->pCommonInfo->pRequestUri == Sdf_co_null )
				{
					/*w534*/ (void)sip_getAddrSpecFromFromHdr(&dTempHeader, \
						&pAddrSpec, (SipError *)&(pErr->stkErrCode));
				}
				else
				{
					pAddrSpec = pCallObj->pCommonInfo->pRequestUri;
					HSS_LOCKEDINCREF(pAddrSpec->dRefCount);
				}	
			}
			else
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCreateRoutePathInCallObj( ): "
					"Error getting Conatct Info from the SipMessage",pErr);
#endif					
				sip_freeSipHeader(&dTempHeader);
				(void)sdf_ivk_uaFreeHeaderList(pRouteList);
				return Sdf_co_fail;
			}
		}

		sip_freeSipHeader(&dTempHeader);
			
		if (sip_initSipHeader(&pRouteHeader, SipHdrTypeRoute, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
			(void)sdf_ivk_uaFreeHeaderList(pRouteList);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaCreateRoutePathInCallObj( ): "
				"Error initing Header",pErr);
#endif				
			return Sdf_co_fail;
		}

		if (sip_setAddrSpecInRouteHdr(pRouteHeader, pAddrSpec, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaCreateRoutePathInCallObj( ): "
				"Error setting AddrSpec in Route Hdr",pErr);
#endif				
			(void)sdf_ivk_uaFreeHeaderList(pRouteList);
            /* klock tool error fix */
		sip_freeSipHeader(pRouteHeader);
		(void)sdf_memfree(0,(Sdf_ty_pvoid *)&pRouteHeader, pErr);

			return Sdf_co_fail;
		}
			
		if (pDispName != Sdf_co_null)
		{
			/*w534*/ (void)sip_setDispNameInRouteHdr(pRouteHeader, \
				Sdf_mc_strdupCallHandler(pDispName),\
				(SipError *)&(pErr->stkErrCode));
		}

		/* Appended the contact header into the list of Route headers */
		sip_freeSipAddrSpec(pAddrSpec);

		/*w534*/ (void)sdf_listAppend(&(pRouteList->slHeaders), pRouteHeader, pErr);

		if ((sdf_listAppend(&(pCallObj->slPersistentHeaders), \
			(Sdf_ty_pvoid)pRouteList, pErr)) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaCreateRoutePathInCallObj( ): "
				"Error appending RouteHdr list to Persistent Header list "
				"of call object.",pErr);
#endif				
			(void)sdf_ivk_uaFreeHeaderList(pRouteList);
			return Sdf_co_fail;
		}
	}
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaCreateRoutePathInCallObj");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUpdateRoutePathInCallObj
 **
 ** DESCRIPTION: This function updates the Route Header list from the 
 **				 incoming SipMessage in the processed headers list 
 **				 of the call object. 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateRoutePathInCallObj 
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_msgInfo dMessageInfo,
	SipMessage	 *pSipMessage,
	Sdf_st_error *pErr)
#else
	(pCallObj, dMessageInfo, pSipMessage, pErr)
	Sdf_st_callObject 	*pCallObj;
	Sdf_st_msgInfo		dMessageInfo;
	SipMessage			*pSipMessage;
	Sdf_st_error 		*pErr;
#endif
{
	Sdf_ty_bool		  dIsRefreshRequest = Sdf_co_false;
	Sdf_ty_bool		  dMethodAllowed = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaUpdateRoutePathInCallObj");

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
		return Sdf_co_fail;

	if (pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateRoutePathInCallObj( ): "
			"Invalid param value: Call Object",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/* Update route only for fresh requests
	 * and responses to fresh requests. */
	/* 
	 * Check if update is allowed. The update is allowed only if the message
	 * is an INVITE of a 2xx to INVITE
	 */

    if( dMessageInfo.dMsgType == Sdf_en_invite || dMessageInfo.dMsgType == Sdf_en_update)
		dMethodAllowed = Sdf_co_true;
#ifdef SDF_REFER
	else if(dMessageInfo.dMsgType == Sdf_en_refer)
		dMethodAllowed = Sdf_co_true;
#endif
#ifdef SIP_IMPP
  /* SUBSCRIBE & NOTIFY are target refresh requests. Pls refer the following
   * links:
   * http://www.sipwg.org/show_bug.cgi?id=699
   * http://www1.ietf.org/mail-archive/web/sip/current/msg06960.html
   */
	else if ((dMessageInfo.dMsgType == Sdf_en_subscribe) || 
           (dMessageInfo.dMsgType == Sdf_en_notify))
		dMethodAllowed = Sdf_co_true;
#endif

	if ( dMethodAllowed == Sdf_co_true )
	{
		if (dMessageInfo.dReqRespType == SipMessageRequest)
		{
			dIsRefreshRequest = Sdf_co_true;	
		}
		else
		{
			if ((dMessageInfo.dResponse >= 200) && \
				(dMessageInfo.dResponse < 300))
				dIsRefreshRequest = Sdf_co_true;	

#ifdef SIP_RPR
			if (sdf_fn_uaIsMessageRPR(pSipMessage, pErr) == 0)
				dIsRefreshRequest = Sdf_co_true;	
#endif				
		}
	}

	if (dIsRefreshRequest != Sdf_co_true)
	{
		/* 
		 * This message does not correspond to a refresh request. So the 
		 * route set need not be updated 
		 */
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaUpdateRoutePathInCallObj");
		return Sdf_co_success;
	}

	if( pCallObj->pCommonInfo->dPreloadedSet == Sdf_co_true)
	{
		/* This indicates that the existing route set is only the preloaded
		 * one.  So delete the preloaded one and form fresh Route set  */

		/*w534*/ (void)sdf_ivk_uaDeleteHeaderListByCorrelation( \
				&(pCallObj->slPersistentHeaders), "uat-routeset",pErr);

		if (sdf_fn_uaCreateRoutePathInCallObj(pCallObj, dMessageInfo, \
			pSipMessage, pErr)== Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateRoutePathInCallObj(): "
				"Failed to add route headers to ProcessedHeaders",pErr);
#endif				
			return Sdf_co_fail;
		}
		pCallObj->pCommonInfo->dPreloadedSet = Sdf_co_false;
	}
	else
	{
		/* This is an established Route Set.  Only Last Entry which indicates
		 * the Contact Address can be changed */
		if (sdf_fn_uaUpdateContactInRouteSet(pCallObj, pSipMessage, \
				pErr)== Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateRoutePathInCallObj(): "
				"Failed to Update the Route Header to ProcessedHeaders",pErr);
#endif				
			return Sdf_co_fail;
		}
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaUpdateRoutePathInCallObj");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUpdateContactListInCallObj
 **
 ** DESCRIPTION: This function updates the Contact List of the CallObject
 **				 with the information in the incoming SipMessage 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateContactListInCallObj
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	SipMessage *pSipMsg,
	Sdf_st_error *pErr)
#else
	(pCallObj, pSipMsg, pErr)
	Sdf_st_callObject *pCallObj;
	SipMessage *pSipMsg;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit size=0;
	Sdf_ty_u32bit dIndex=0;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaUpdateContactListInCallObj");

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
		return Sdf_co_fail;

	if (pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactListInCallObj( ): "
			"Invalid param value: Call Object",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */


	/*w534*/ (void)sip_getHeaderCount(pSipMsg, SipHdrTypeContactAny,\
		&size,(SipError *)&(pErr->stkErrCode));

	if (size != 0)
	{
		Sdf_st_commonInfo *pCommonInfo;
		if (sdf_ivk_uaGetCommonInfoFromCallObject(pCallObj,\
			&pCommonInfo, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactListInCallObj( ): "
				"Error getting commonInfo from CallObject",pErr);
#endif			
			return Sdf_co_fail;
		}
        /* CSR_1-7588994 Fix Start */
#ifdef SIP_RPR
        if(0 == sdf_fn_uaIsMessageRPR(pSipMsg, pErr))
            pCommonInfo->dReqUriPort = 0;
        else
        {
#endif
            if(SipMessageResponse == pSipMsg->dType)
            {
                if((199 < pSipMsg->u.pResponse->pStatusLine->dIntVar1) && 
                        (200 >= pSipMsg->u.pResponse->pStatusLine->dIntVar1))
                    pCommonInfo->dReqUriPort = 0;
            }
#ifdef SIP_RPR
        }
#endif
        /* CSR_1-7588994 Fix End */        
		if ((sdf_listDeleteAll(&(pCommonInfo->slContact), \
			pErr)) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactListInCallObj( ): "
				"Error deleting contact Hdr list from commonInfo",pErr);
#endif			
			return Sdf_co_fail;
		}
		for(dIndex=0; dIndex<size; dIndex++)
		{
			SipHeader *pDestHeader;

			if (sip_initSipHeader(&pDestHeader, SipHdrTypeAny, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
				(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
				sip_freeSipHeader(pDestHeader);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactListInCallObj(): "
					"Failed to init a SipHeader structure",pErr);
#endif				
				return Sdf_co_fail;
			}
			if (sip_getHeaderAtIndex(pSipMsg, SipHdrTypeContactAny, \
				pDestHeader, dIndex, (SipError *)&(pErr->stkErrCode))==SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
				(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
				sip_freeSipHeader(pDestHeader);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactListInCallObj( ): "
					"Error getting Contact Header",pErr);
#endif				
				return Sdf_co_fail;
			}
			if ((sdf_listAppend(&(pCommonInfo->slContact), pDestHeader, \
				pErr)) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactListInCallObj( ): "
					"Error appending to Conact list in Common Info",pErr);
#endif				
				return Sdf_co_fail;
			}
#ifdef SDF_DEP
			/* In case of DEP, we should post the contact change for
			 * the call-object ONLY if it is changed(this is to be done
			 * only in the case of receipt of a Re-INVITE or UPDATE
			 * method).For this purpose we should take a diff of the
			 * received contact as compared to the earlier contact
			 * header(which will be stored in DepDetails of the
			 * call-object).This will be done only for the *FIRST*
			 * contact in the remote targets list,as we post only for
			 * the change in single contact.
			 */
			if(dIndex == 0)
			{
				Sdf_ty_bool isPost = Sdf_co_false;
				if(sdf_fn_uaDepIsContactChange(pCallObj,pSipMsg,&isPost,pDestHeader,pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
							(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactListInCallObj( ): "
							"Error in posting Contact Change for DEP",pErr);
#endif
					return Sdf_co_fail;
				}

				if(isPost == Sdf_co_true)
				{
					/* This means contact has changed.So post
					 * this using sdf_fn_uaDepPostContactChange
					 */
					if(sdf_fn_uaDepPostContactChange(pCallObj,pSipMsg,pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR				
						sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
								(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactListInCallObj( ): "
								"Error in posting Contact Change for DEP",pErr);
#endif			
						return Sdf_co_fail;
					}
				}/*End of if isPost == true*/ 

			}/*End of dIndex = 0*/

#endif
		}
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaUpdateContactListInCallObj");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_remoteRetransHash
 **
 ** DESCRIPTION: Caclulates the hash value for a given key used in the remote 
 **				 retransmissions hash table.
 **
 ******************************************************************************/
Sdf_ty_u32bit sdf_fn_remoteRetransHash
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pKey)
#else
	(pKey)
	Sdf_ty_pvoid pKey;
#endif
{
	Sdf_ty_u32bit retval=0;
	Sdf_st_remoteRetransKey *pRetransKey = (Sdf_st_remoteRetransKey *)pKey;

	retval = sdf_ivk_uaElfHash((Sdf_ty_pvoid)(pRetransKey->pCallId));
	retval += pRetransKey->dCseqNo;
	return retval;
}


/******************************************************************************
 ** FUNCTION:  	sdf_fn_remoteRetransKeyCompare
 **
 ** DESCRIPTION: Compare two keys (of elements stored in the remote
 **				 retransmission hash table).
 **
 ******************************************************************************/
Sdf_ty_u8bit sdf_fn_remoteRetransKeyCompare
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pKey1, Sdf_ty_pvoid pKey2)
#else
	(pKey1, pKey2)
	Sdf_ty_pvoid pKey1;
	Sdf_ty_pvoid pKey2;
#endif
{
	SipError error;
	Sdf_st_error sdfError;
	Sdf_st_remoteRetransKey *pRetransKey1 = (Sdf_st_remoteRetransKey *)pKey1;
	Sdf_st_remoteRetransKey *pRetransKey2 = (Sdf_st_remoteRetransKey *)pKey2;
	Sdf_ty_s8bit *pFromTag1=Sdf_co_null, *pFromTag2 = Sdf_co_null;
	Sdf_ty_s8bit *pToTag1=Sdf_co_null, *pToTag2 = Sdf_co_null;
	SipAddrSpec *pFromAddrSpec1 = Sdf_co_null, *pFromAddrSpec2 = Sdf_co_null;
	SipAddrSpec *pToAddrSpec1 = Sdf_co_null, *pToAddrSpec2 = Sdf_co_null;

#ifdef SDF_PARAMVALIDATION
	if ((pKey1 == Sdf_co_null) || (pKey2 == Sdf_co_null))
		return 1;
#endif	/* End of param validation ifdef */

	if (pKey1 == pKey2)
		return 0;

	/* Frist match CSeq - the least expensive of 'em all */
	if(pRetransKey1->dCseqNo != pRetransKey2->dCseqNo)
		return 1;

	if(Sdf_mc_strcmp(pRetransKey1->pMethod, pRetransKey2->pMethod)!=0)
		return 1;
	
	/* Call-ID next */
	if(Sdf_mc_strcmp(pRetransKey1->pCallId, pRetransKey2->pCallId)!=0)
		return 1;

	if( pRetransKey1->pViaBranch != Sdf_co_null)
	{
		if( pRetransKey2->pViaBranch != Sdf_co_null )
		{
			if(Sdf_mc_strcmp(pRetransKey1->pViaBranch, \
				pRetransKey2->pViaBranch)!=0)
				return 1;
		}
		else
			return 1;
	}
	else if ( pRetransKey2->pViaBranch != Sdf_co_null )
		return 1;

	/* Now match From and To headers */
	/*w534*/ (void)sip_getAddrSpecFromToHdr(pRetransKey1->pToHeader, &pToAddrSpec1, &error);
	/*w534*/ (void)sip_getAddrSpecFromToHdr(pRetransKey2->pToHeader, &pToAddrSpec2, &error);

	if(sdf_fn_uaCompareSipAddrSpec(pToAddrSpec1, pToAddrSpec2, \
		Sdf_co_true, Sdf_co_false, &sdfError) != 0)
	{
		sip_freeSipAddrSpec(pToAddrSpec1);
		sip_freeSipAddrSpec(pToAddrSpec2);
		return 1;
	}
	sip_freeSipAddrSpec(pToAddrSpec1);
	sip_freeSipAddrSpec(pToAddrSpec2);

	/*w534*/ (void)sip_getAddrSpecFromFromHdr(pRetransKey1->pFromHeader, &pFromAddrSpec1,\
		&error);
	/*w534*/ (void)sip_getAddrSpecFromFromHdr(pRetransKey2->pFromHeader, &pFromAddrSpec2,\
		&error);
	
	if(sdf_fn_uaCompareSipAddrSpec(pFromAddrSpec1, pFromAddrSpec2, \
		Sdf_co_true, Sdf_co_false, &sdfError) != 0)
	{
		sip_freeSipAddrSpec(pFromAddrSpec1);
		sip_freeSipAddrSpec(pFromAddrSpec2);
		return 1;
	}
	sip_freeSipAddrSpec(pFromAddrSpec1);
	sip_freeSipAddrSpec(pFromAddrSpec2);

	/* Last thing - Match the tags */
	if(sip_getTagAtIndexFromToHdr(pRetransKey1->pToHeader, &pToTag1, 0,\
		&error)==SipFail)
		pToTag1 = Sdf_co_null;
	if(sip_getTagAtIndexFromToHdr(pRetransKey2->pToHeader, &pToTag2, 0,\
		&error)==SipFail)
		pToTag2 = Sdf_co_null;
	/* The stored To header's tag is pToTag1. This might have a value
	   but the one being compared to might not. This case is valid */
	if(pToTag2!=Sdf_co_null)
	{
		if(pToTag1 == Sdf_co_null)
			return 1;
		if(Sdf_mc_strcmp(pToTag1, pToTag2)!=0)
			return 1;
	}
	/* For the from header the matching is strict */
	if(sip_getTagAtIndexFromFromHdr(pRetransKey1->pFromHeader, &pFromTag1, 0,\
		&error)==SipFail)
		pFromTag1 = Sdf_co_null;
	if(sip_getTagAtIndexFromFromHdr(pRetransKey2->pFromHeader, &pFromTag2, 0,\
		&error)==SipFail)
		pFromTag2 = Sdf_co_null;
	if(((pFromTag1==Sdf_co_null)&&(pFromTag2!=Sdf_co_null))\
		||((pFromTag2==Sdf_co_null)&&(pFromTag1!=Sdf_co_null)))
		return 1;

	if(pFromTag1!=Sdf_co_null)
		if(Sdf_mc_strcmp(pFromTag1, pFromTag2)!=0)
		return 1;

	/*Now check for the Message Type*/
	if(pRetransKey1->dMessageType==pRetransKey2->dMessageType)
		return 1;
		
	return 0;
}


/******************************************************************************
 ** FUNCTION:  	sdf_fn_remoteRetransElementFree
 **
 ** DESCRIPTION: Function to free an element from the remote retransmission 
 **				 hash table.
 **
 ******************************************************************************/
void sdf_fn_remoteRetransElementFree
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pElement)
#else
	(pElement)
	Sdf_ty_pvoid pElement;
#endif
{
	(void)sdf_fn_uaFreeRemoteRetransBuffer(\
		(Sdf_st_remoteRetransBuffer *)pElement);
}


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_remoteRetransKeyFree
 **
 ** DESCRIPTION: Function to free a key (for an element in the remote 
 **				 retransmission hash table).
 **
 ******************************************************************************/
void sdf_fn_remoteRetransKeyFree
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pKey)
#else
	(pKey)
	Sdf_ty_pvoid pKey;
#endif
{
	(void)sdf_fn_uaFreeRemoteRetransKey((Sdf_st_remoteRetransKey *)pKey);
}


/******************************************************************************
 ** FUNCTION: 	 	sdf_fn_uaCompareSipAddrSpec
 **
 ** DESCRIPTION: 	This function is used to compare two addrspecs according
 **				 	to the sip-url comparision rules
 **	 
 **	RETURN VALUES: 	0 - The address spec structures match.
 **					1 - The address spec structures don't match because 
 **						of some mismatch 
 **					2 - One of the address spec structures contains a user
 **						part which is a subset of the other (overlap dialling)
 **
 ******************************************************************************/
Sdf_ty_s8bit sdf_fn_uaCompareSipAddrSpec 
#ifdef ANSI_PROTO	
	(SipAddrSpec *pAddrSpec1, 
	SipAddrSpec *pAddrSpec2,
	Sdf_ty_bool dCompleteMatch, 
	Sdf_ty_bool dPortLeniancy,
	Sdf_st_error *pError)
#else
	(pAddrSpec1, pAddrSpec2, dCompleteMatch, dPortLeniancy, pError)
	SipAddrSpec *pAddrSpec1;
	SipAddrSpec *pAddrSpec2;
	Sdf_ty_bool dCompleteMatch;
	Sdf_ty_bool dPortLeniancy;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_u16bit dPort1=0, dPort2=0;
	en_AddrType dType1=SipAddrAny, dType2=SipAddrAny;
	Sdf_ty_s8bit *pUri1 = Sdf_co_null, *pUri2 = Sdf_co_null;
	Sdf_ty_u32bit dIndex = 0, dJindex = 0;
#ifdef SDF_TEL
	TelUrl *pTelUrl1 = Sdf_co_null;
	TelUrl *pTelUrl2 = Sdf_co_null;
#endif
#ifdef SDF_IM
	ImUrl	*pImUrl1 = Sdf_co_null;
	ImUrl	*pImUrl2 = Sdf_co_null;
#endif
#ifdef SDF_PRES
	PresUrl	*pPresUrl1 = Sdf_co_null;
	PresUrl	*pPresUrl2 = Sdf_co_null;
#endif

	/*w534*/ (void)sip_getAddrTypeFromAddrSpec(pAddrSpec1, &dType1,(SipError *)\
		&(pError->stkErrCode));
	/*w534*/ (void)sip_getAddrTypeFromAddrSpec(pAddrSpec2, &dType2,\
		(SipError *)&(pError->stkErrCode));

#ifdef SDF_TLS
	if((dType1 == SipAddrSipUri) || (dType1 == SipAddrSipSUri))
#else
	if(dType1 == SipAddrSipUri)
#endif
	{
		/* Address1 is a SIP URL. */
		SipUrl *pUrl1, *pUrl2;
		Sdf_ty_s8bit dUserAndHostComparisonResult = 0;

		/* Check is Address2 is a SIP URL too. */
		if(dType2 != dType1)
			return 1;

		dUserAndHostComparisonResult = sdf_fn_uaCompareUserAndHost(\
			pAddrSpec1, pAddrSpec2, pError);
		
		if (dUserAndHostComparisonResult != 0)
		{
			return dUserAndHostComparisonResult;
		}
		
		/* Compare port fields now */
		/*w534*/ (void)sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec1, &pUrl1, pError);
		/*w534*/ (void)sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec2, &pUrl2, pError);
	
		/* Port comparison */
		if (dPortLeniancy == Sdf_co_true)
		{
			/* 
			 * Leniancy of port switched on. This means that if the first 
			 * URL does not have a port, we can ignore the comparision. 
			 * This is used in order to provide leniancy for the Request-URI. 
			 * If we get a request with R-URI sip:a@b.com, but the profile 
			 * indicates <sip:a@b.com:3421>, we should still consider it to
			 * match. This is from Bakeoff tests, where we found that
			 * proxies may not rewrite the Request-URI to contain the port
			 * detail as well. The fact that it came to the interface
			 * (port) on which the application was listening suffices.
			 *
			 * This flag is used only from sdf_ivk_uaValidateUri, after
			 * passing the Request-URI addrspec as the first parameter.
			 */
			if(pUrl1->dPort != Sdf_co_null)
				dPort1 = *(pUrl1->dPort);
		}
		else
		{
			if(pUrl1->dPort == Sdf_co_null)
			{
#ifdef SDF_TLS
				if (dType1 == SipAddrSipSUri)
				    dPort1 = Sdf_mc_defaultSipSPort;
				else
#endif
				    dPort1 = Sdf_mc_defaultSipPort;
			}
			else
				dPort1 = *(pUrl1->dPort);
		}

		if(pUrl2->dPort == Sdf_co_null)
		{
#ifdef SDF_TLS
			if (dType2 == SipAddrSipSUri)
				dPort2 = Sdf_mc_defaultSipSPort;
			else
#endif
				dPort2 = Sdf_mc_defaultSipPort;
		}
		else
			dPort2 = *(pUrl2->dPort);

		if(dPort1 != dPort2)
		{
			if (dPortLeniancy == Sdf_co_true)
			{
				if (dPort1 != 0)
				{
					/* Free the local variables */
					sip_freeSipUrl(pUrl1);
					sip_freeSipUrl(pUrl2);
					return 1;
				}
			}
			else
			{
				/* Free the local variables */
				sip_freeSipUrl(pUrl1);
				sip_freeSipUrl(pUrl2);
				return 1;
			}
		}

		if (dCompleteMatch == Sdf_co_true)
		{
			Sdf_ty_u32bit dInerParamCount,dOuterParamCount;
			SipUrl	*pOuterUrl=Sdf_co_null,*pInerUrl=Sdf_co_null;

			/*
			 * Complete matching including URL parameters.
			 * Start comparing URL parameters. 
			 *
			 * NOTE :
			 * -> Only URL parameters that are present in both URL's 
			 * 		are taken for the comparision.
			 * -> Paramnames are case insensitive
			 * -> Paramvalues are case sensitive
			 * -> Characters other than those in the "reserved" and 
			 * 		"unsafe" sets are equivalent to their "% HEX HEX" encoding.
			 */
			Sdf_ty_u32bit dUrlParamListSize1 = 0, dUrlParamListSize2 = 0;

			/*w534*/ (void)sip_getUrlParamCountFromUrl(pUrl1, &dUrlParamListSize1, \
				(SipError *)&(pError->stkErrCode));

			/*w534*/ (void)sip_getUrlParamCountFromUrl(pUrl2, &dUrlParamListSize2, \
				(SipError *)&(pError->stkErrCode));


			if( dUrlParamListSize2 != dUrlParamListSize1 )
			{
				sip_freeSipUrl(pUrl1);
				sip_freeSipUrl(pUrl2);
				return 1;
			}

			dOuterParamCount=dUrlParamListSize2;
			dInerParamCount=dUrlParamListSize1;
			pOuterUrl = pUrl2;
			pInerUrl = pUrl1;

			/*
			 * Iterate through each param of pUrl1
			 */
			for (dIndex = 0; dIndex < dOuterParamCount; dIndex++)
			{
				SipParam *pParam1 = Sdf_co_null;
				Sdf_ty_s8bit *pName1 = Sdf_co_null, *pValue1 = Sdf_co_null;
				Sdf_ty_s8bit dHexConvertedName1[Sdf_co_smallBufferSize];
				Sdf_ty_s8bit dHexConvertedValue1[Sdf_co_smallBufferSize];
				Sdf_ty_bool dExactMatch=Sdf_co_false;

				/*w534*/ (void)sip_getUrlParamAtIndexFromUrl(pOuterUrl, &pParam1, \
					dIndex, (SipError *)&(pError->stkErrCode));

				/*w534*/ (void)sip_getNameFromSipParam(pParam1, &pName1, \
					(SipError *)&(pError->stkErrCode));

				/*w534*/ (void)sip_getValueAtIndexFromSipParam(pParam1, &pValue1, \
					0, (SipError *)&(pError->stkErrCode));

				/*CSR 1-7574892 change start*/ 

                if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pName1, dHexConvertedName1,\
                            Sdf_co_smallBufferSize))
                {
                    sip_freeSipParam(pParam1);
                    sip_freeSipUrl(pUrl1);
                    sip_freeSipUrl(pUrl2);

                    return 1;
                }
                if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pValue1, dHexConvertedValue1,\
                            Sdf_co_smallBufferSize))
                {
                    sip_freeSipParam(pParam1);
                    sip_freeSipUrl(pUrl1);
                    sip_freeSipUrl(pUrl2);

                    return 1;
                }
                /*CSR 1-7574892 change end*/
				/*
				 * Now iterate through each param of pUrl2 searching
				 * for a param with the same name
				 */
				for (dJindex = 0; dJindex < dInerParamCount; dJindex++)
				{
					SipParam *pParam2 = Sdf_co_null;
					Sdf_ty_s8bit *pName2 = Sdf_co_null, *pValue2 = Sdf_co_null;
					Sdf_ty_s8bit dHexConvertedName2[Sdf_co_smallBufferSize];
					Sdf_ty_s8bit dHexConvertedValue2[Sdf_co_smallBufferSize];
					
					/*w534*/ (void)sip_getUrlParamAtIndexFromUrl(pInerUrl, &pParam2, \
						dJindex, (SipError *)&(pError->stkErrCode));

					/*w534*/ (void)sip_getNameFromSipParam(pParam2, &pName2, \
						(SipError *)&(pError->stkErrCode));
                    /*CSR 1-7574892 change start*/ 
                    if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pName2, dHexConvertedName2,\
                                Sdf_co_smallBufferSize))
                    {
                        sip_freeSipParam(pParam1);
                        sip_freeSipParam(pParam2);
                        sip_freeSipUrl(pUrl1);
                        sip_freeSipUrl(pUrl2);
                        return 1;
                    }
                    /*CSR 1-7574892 change end*/

					if (Sdf_mc_strcasecmp(dHexConvertedName1, \
						dHexConvertedName2) == 0)
					{
						/*
						 * Matching names found
						 */
						/*w534*/ (void)sip_getValueAtIndexFromSipParam(pParam2, &pValue2, \
							0, (SipError *)&(pError->stkErrCode));

                        /*CSR 1-7574892 change start*/ 
                        if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pValue2, \
                                    dHexConvertedValue2,Sdf_co_smallBufferSize))
                        {
                            sip_freeSipParam(pParam1);
                            sip_freeSipParam(pParam2);
                            sip_freeSipUrl(pUrl1);
                            sip_freeSipUrl(pUrl2);
                            return 1;
                        }
                        /*CSR 1-7574892 change end*/


						if ( (dHexConvertedValue1[0] != '\0') && \
							(dHexConvertedValue2[0] != '\0') && \
							(Sdf_mc_strcmp(dHexConvertedValue1, \
							dHexConvertedValue2) == 0) )
						{
							/*
							 * Param name and param values match. Break
							 * out and pick up the next param
							 */
							sip_freeSipParam(pParam2);
							dExactMatch=Sdf_co_true;
							break;
						}
						else if ( (dHexConvertedValue1[0] == '\0') && \
							(dHexConvertedValue2[0] == '\0') )
						{
							/*
							 * Both param values are NULL. Break out
							 * and pick up the next param
							 */
							dExactMatch=Sdf_co_true;
							sip_freeSipParam(pParam2);

							break;
						}
						else
						{
							/*
							 * A url-parameter name matched, but the values
							 * did not match. Return 1.
							 */
							sip_freeSipParam(pParam1);
							sip_freeSipParam(pParam2);
							sip_freeSipUrl(pUrl1);
							sip_freeSipUrl(pUrl2);

							return 1;
						}
					}
					/* Free the local reference */
					sip_freeSipParam(pParam2);
				}
				/* Free the local reference */
				sip_freeSipParam(pParam1);

				/* If no match found then return failure */
				if(dExactMatch==Sdf_co_false)
				{
					sip_freeSipUrl(pUrl1);
					sip_freeSipUrl(pUrl2);
					return 1;
				}
			}
		}
		/*
		 * All validations completed successfully.
		 * Return 0 (success)
		 */

		/* Free the local reference */
		sip_freeSipUrl(pUrl1);
		sip_freeSipUrl(pUrl2);

		return 0;
	}
	/* Only user@host:port matching */
	else if(dType1 == SipAddrReqUri)
	{
		/* Address1 is a non SIP URL.
		Check if address2 is a non SIP URL too. */

		if(dType2 != SipAddrReqUri)
			return 1;

		/* compare the address strings in full ignoring case */
		/*w534*/ (void)sip_getUriFromAddrSpec(pAddrSpec1, &pUri1, (SipError *)\
			&(pError->stkErrCode));
		/*w534*/ (void)sip_getUriFromAddrSpec(pAddrSpec2, &pUri2,(SipError *)\
			&(pError->stkErrCode));

#ifdef SDF_TEL
		if(sip_isTelUrl(pAddrSpec1,	(SipError*)&(pError->stkErrCode)) \
								!= SipFail)
		{
			if(sip_getTelUrlFromAddrSpec(pAddrSpec1, &pTelUrl1,\
					(SipError*)&(pError->stkErrCode))==SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipAddrSpec( ): "
					"Error in getting Tel url from addrspec", pError);
#endif		
				return 1;
			}

		}
		if(sip_isTelUrl(pAddrSpec2,(SipError*)&(pError->stkErrCode)) != SipFail)
		{
			if(sip_getTelUrlFromAddrSpec(pAddrSpec2, &pTelUrl2,\
					(SipError*)&(pError->stkErrCode))==SipFail)
			{
				sip_freeTelUrl (pTelUrl1);
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipAddrSpec( ): "
					"Error in getting Tel url from addrspec", pError);
#endif		
				return 1;
			}
				
		}
		if( (pTelUrl1 != Sdf_co_null) && ( pTelUrl2 != Sdf_co_null))
		{
			if(sdf_fn_uaCompareTelUri( pTelUrl1, pTelUrl2, pError) == 0)
			{
				sip_freeTelUrl (pTelUrl1);
				sip_freeTelUrl (pTelUrl2);
				return 0;
			}
		}
		if( pTelUrl1 != Sdf_co_null )
			sip_freeTelUrl (pTelUrl1);

		if( pTelUrl2 != Sdf_co_null )
			sip_freeTelUrl (pTelUrl2);
#endif		
#ifdef SDF_IM
		if(sip_isImUrl(pAddrSpec1,(SipError*)&(pError->stkErrCode)) != SipFail)
		{
			if(sip_getImUrlFromAddrSpec(pAddrSpec1, &pImUrl1,\
					(SipError*)&(pError->stkErrCode))==SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipAddrSpec( ): "
					"Error in getting Im url from addrspec", pError);
#endif		
				return 1;
			}

		}
		if(sip_isImUrl(pAddrSpec2,(SipError*)&(pError->stkErrCode)) != SipFail)
		{
			if(sip_getImUrlFromAddrSpec(pAddrSpec2, &pImUrl2,\
					(SipError*)&(pError->stkErrCode))==SipFail)
			{
				sip_freeImUrl (pImUrl1);
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipAddrSpec( ): "
					"Error in getting Im url from addrspec", pError);
#endif		
				return 1;
			}
		}
		if( (pImUrl1 != Sdf_co_null) && ( pImUrl2 != Sdf_co_null))
		{
			if(sdf_fn_uaCompareImUrl(pImUrl1, pImUrl2, pError) == 0)
			{
				sip_freeImUrl (pImUrl1);
				sip_freeImUrl (pImUrl2);
				return 0;
			}
		}
		if( pImUrl1 != Sdf_co_null )
			sip_freeImUrl (pImUrl1);

		if( pImUrl2 != Sdf_co_null )
			sip_freeImUrl (pImUrl2);
#endif		
#ifdef SDF_PRES
	   if(sip_isPresUrl(pAddrSpec1,(SipError*)&(pError->stkErrCode)) != SipFail)
		{
			if(sip_getPresUrlFromAddrSpec(pAddrSpec1, &pPresUrl1,\
					(SipError*)&(pError->stkErrCode))==SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipAddrSpec( ): "
				"Error in getting Pres url from addrspec", pError);
#endif		
				return 1;
			}

		}
	  if(sip_isPresUrl(pAddrSpec2,(SipError*)&(pError->stkErrCode)) != SipFail)
		{
			if(sip_getPresUrlFromAddrSpec(pAddrSpec2, &pPresUrl2,\
					(SipError*)&(pError->stkErrCode))==SipFail)
			{
				sip_freePresUrl (pPresUrl1);
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipAddrSpec( ): "
					"Error in getting Pres url from addrspec", pError);
#endif		
				return 1;
			}
		}
		if( (pPresUrl1 != Sdf_co_null) && ( pPresUrl2 != Sdf_co_null))
		{
			if(sdf_fn_uaComparePresUrl(pPresUrl1, pPresUrl2, pError) == 0)
			{
				sip_freePresUrl (pPresUrl1);
				sip_freePresUrl (pPresUrl2);
				return 0;
			}
		}
		if( pPresUrl1 != Sdf_co_null )
			sip_freePresUrl (pPresUrl1);

		if( pPresUrl2 != Sdf_co_null )
			sip_freePresUrl (pPresUrl2);
#endif
		if(Sdf_mc_strcasecmp(pUri1, pUri2)!=0)
			return 1;
		else
			return 0;
	}
	else
	{
		return 1;
	}
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCompareSipHeaderFromTo
 **
 ** DESCRIPTION: This function is used to compare two headers. Only From/To 
 **				 headers may be passed to this API.
 **	 
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCompareSipHeaderFromTo
#ifdef ANSI_PROTO
	(SipHeader *pNewHeader, 
	SipHeader *pExistHeader, 
	Sdf_ty_callLegMatch *pCallLegMatch,
	Sdf_ty_bool		dToTagLenient,
	Sdf_st_error *pError)
#else
	(pNewHeader, pExistHeader, pCallLegMatch, dToTagLenient, pError)
	SipHeader *pNewHeader;
	SipHeader *pExistHeader;
	Sdf_ty_callLegMatch *pCallLegMatch;
	Sdf_ty_bool		dToTagLenient;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_s8bit *pNewTag=Sdf_co_null, *pExistingTag=Sdf_co_null;
	Sdf_ty_bool	 dIsToTagCmp = Sdf_co_false;

#ifdef SDF_SUBSCRIPTION_LAYER
	Sdf_ty_callLegMatch dCallLegMatch        = Sdf_en_notMatching;
	Sdf_ty_bool	        dIsCheckForkedSubs   = Sdf_co_false;
#endif

#ifdef SDF_PARAMVALIDATION
	if ((pNewHeader == Sdf_co_null)||(pExistHeader == Sdf_co_null)|| \
		(pCallLegMatch == Sdf_co_null) || (pError == Sdf_co_null))
	{
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

#ifdef SDF_SUBSCRIPTION_LAYER
	if (*pCallLegMatch == Sdf_en_forkedSubscribe)
	{
		dIsCheckForkedSubs = Sdf_co_true;
	}
#endif


	/* Compare the Tags which are sufficient for RFC 3261 complient UAs */
	if (pNewHeader->dType == SipHdrTypeFrom)
	{
		if (sip_getTagAtIndexFromFromHdr(pNewHeader, &pNewTag, 0, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
			if ((pError->errCode != Sdf_en_noExistError) &&
				(pError->errCode != Sdf_en_invalidIndexError))
			{
#ifdef SDF_ERROR					
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipHeaderFromTo(): "
					"Failed to get Tag at Index 0 from FromHdr",pError);
#endif					
				return Sdf_co_fail;
			}
		}
	}
	else
	{
		if (sip_getTagAtIndexFromToHdr(pNewHeader, &pNewTag, 0, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
			if ((pError->errCode != Sdf_en_noExistError) &&
				(pError->errCode != Sdf_en_invalidIndexError))
			{
#ifdef SDF_ERROR					
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipHeaderFromTo(): "
					"Failed to get Tag at Index 0 from ToHdr",pError);
#endif					
				return Sdf_co_fail;
			}
		}
	}

	if (pExistHeader->dType == SipHdrTypeFrom)
	{
		if (sip_getTagAtIndexFromFromHdr(pExistHeader, &pExistingTag, 0, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
			if ((pError->errCode != Sdf_en_noExistError) &&
				(pError->errCode != Sdf_en_invalidIndexError))
			{
#ifdef SDF_ERROR					
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipHeaderFromTo(): "
					"Failed to get Tag at Index 0 from FromHdr",pError);
#endif					
				return Sdf_co_fail;
			}
		}
	}
	else
	{
		if (sip_getTagAtIndexFromToHdr(pExistHeader, &pExistingTag, 0, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
			if ((pError->errCode != Sdf_en_noExistError) &&
				(pError->errCode != Sdf_en_invalidIndexError))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipHeaderFromTo(): "
					"Failed to get Tag at Index 0 from ToHdr",pError);
#endif					
				return Sdf_co_fail;
			}
		}
	}

	if ( (pExistHeader->dType == SipHdrTypeTo) && \
		 (pNewHeader->dType == SipHdrTypeTo))
			dIsToTagCmp = Sdf_co_true;
	/* 
	 * pNewTag is the tag of the new key. 
	 * pExistingTag is the tag of the existing key.
	 * If pExistingTag is NULL but pNewTag is present, that is fine.
	 * But if pNewTag is NULL and pExistingTag is not, then the remote
	 * end has gobbled the tag. We return Sdf_en_notMatching 
	 * in this case
	 */
	if(pNewTag != Sdf_co_null)
	{
		if(pExistingTag != Sdf_co_null)
		{
			if (Sdf_mc_strcmp(pNewTag, pExistingTag) != 0)
			{
				/*if((dIsToTagCmp == Sdf_co_true) && \
					(dToTagLenient == Sdf_co_true))
					*pCallLegMatch = Sdf_en_sameCallLeg;
				else
					*pCallLegMatch = Sdf_en_notMatching;*/

                                 /*ARICENT CSR_1-5021862*/
				if(dIsToTagCmp == Sdf_co_true)                              
				{
					if(dToTagLenient == Sdf_co_true)
					{
						*pCallLegMatch = Sdf_en_sameCallLeg;
					}
					else
					{
						*pCallLegMatch = Sdf_en_differentCallLeg;
					}
				}
				else
				{
					*pCallLegMatch = Sdf_en_notMatching;
				}
				/*ARICENT CSR_1-5021862*/

			}
			else
				*pCallLegMatch = Sdf_en_sameCallLeg;
		}
		else
		{
			/* Fix for CSR 1-1316815
			 * Leniency should be present only for the To-To pair. For all
			 * other pairs (i.e. From-From, From-To, To-From) it should
			 * map to NotMatching.
			 */
			if ((pNewHeader->dType == SipHdrTypeTo) && \
				 (pExistHeader->dType == SipHdrTypeTo))
				*pCallLegMatch = Sdf_en_sameCallLeg;
			else
				*pCallLegMatch = Sdf_en_notMatching;
		}
	}
	else
	{
		if(pExistingTag != Sdf_co_null)
		{
			/* Fix for CSR 1-1316815
			 * Leniency should be present only for the To-To pair and that
			 * too if the dToTagLenient flag is set (i.e. to provide
			 * leniency for special cases). 
			 */
			if (((pNewHeader->dType == SipHdrTypeTo) && \
				 (pExistHeader->dType == SipHdrTypeTo)) && \
				(dToTagLenient == Sdf_co_true))
				*pCallLegMatch = Sdf_en_sameCallLeg;
			else
				*pCallLegMatch = Sdf_en_notMatching;
		}
		else
			*pCallLegMatch = Sdf_en_sameCallLeg;
	}

#ifdef SDF_SUBSCRIPTION_LAYER
	/* The following code is added to check for two scenarios:
	 * 1)	Early NOTIFy i.e. a NOTIFY arriving before any other response/NOTIFY.
	 * In case of Early NOTIFY, the existing CallObject would not have a tag 
	 * whereas the NewCallObject would have one.
   * 
	 * 2)	A NOTIFY arriving because of forking of the SUBSCRIBE. This check has 
	 * to be prompted by the application and we use the 'dIsCheckForkedSubs' 
	 * bool for this purpose.
   *
	 * NOTE: In case of NOTIFYs from different Notifiers (as a result of forked 
	 * SUBSCRIBE), it is most likely that the HdrType of the RemoteAddr in the 
	 * CallObject is the same as the HdrType of the RemoteAddr got from the 
	 * NOTIFY (i.e. its From-header). So, the check for Early NOTIFY will not 
	 * pass & so we've to explicitly check for the bool, 'dIsCheckForkedSubs'.
	 */
	if ((*pCallLegMatch == Sdf_en_notMatching) &&
			((dIsCheckForkedSubs  == Sdf_co_true) ||
			 ((pNewHeader->dType   == SipHdrTypeFrom) && 
				(pExistHeader->dType == SipHdrTypeTo))))
	{
		if (pNewTag != Sdf_co_null)
		{
			if ( (pExistingTag != Sdf_co_null) && 
						(dIsCheckForkedSubs == Sdf_co_true) )
			{
				/* If To-tag of the existing CallObject doesn't match that of the
					existing CallObject, then it could be either: NOTIFY received for a
					forked SUBSCRIBE (OR) NOTIFY outside any subscription. In either 
					case, we will return the partially matched CallObject and let the 
					application find out the rest.*/
				*pCallLegMatch = Sdf_en_forkedSubscribe;
			}
			else if (pExistingTag == Sdf_co_null)
			{
				/* If To-tag is not present in the existing CallObject, then it 
					could be a case of either: Early NOTIFY (OR) NOTIFY received for a 
					forked SUBSCRIBE. We will treat this as an early NOTIFY. Again, 
					return the partially matching CallObject, application should figure 
					out the rest.*/
				*pCallLegMatch = Sdf_en_earlyNotify;
			} /* End of pExistingTag == Sdf_co_null */
		} /* End of pNewTag != Sdf_co_null */
	} /* End of (isNotifyCheck) && (Sdf_en_notMatching) */
#endif

	/* If Global ToolKit variable is set to Strict URI checking, then
	 * include the total URI comparision.  Else return the above result itslef
	 * */

	if( pGlbToolkitData->dStrictURICheck == Sdf_co_false )
		return Sdf_co_success;
	else
	{
		/* Compare the entire URI */
		Sdf_ty_bool		dIsTagsMatched = Sdf_co_false;
		
#ifdef SDF_SUBSCRIPTION_LAYER
		/* Store the value in pCallLegMatch in case so that the original value 
		 * can be returned to the Application.
		 */
		if ((*pCallLegMatch == Sdf_en_sameCallLeg) ||
				(*pCallLegMatch == Sdf_en_forkedSubscribe) ||
				(*pCallLegMatch == Sdf_en_earlyNotify))
    {
			dIsTagsMatched  = Sdf_co_true;
      dCallLegMatch   = *pCallLegMatch;
    }
#else
		if( *pCallLegMatch == Sdf_en_sameCallLeg )
			dIsTagsMatched = Sdf_co_true;
#endif

		if(sdf_fn_uaCompareFullURI(pNewHeader, pExistHeader, dIsTagsMatched, \
				pCallLegMatch, pError) != Sdf_co_success )
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipHeaderFromTo(): "
				"Failed to get AddrSpec from FromHdr",pError);
#endif			
			return Sdf_co_fail;
		}
#ifdef SDF_SUBSCRIPTION_LAYER
		if ((*pCallLegMatch     == Sdf_en_sameCallLeg) &&
				(dIsCheckForkedSubs == Sdf_co_true))
			*pCallLegMatch = dCallLegMatch;
#endif
		return Sdf_co_success;
	}
}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGenerateCallLegKey
 **
 ** DESCRIPTION: This function is used to generate the call leg key (stored 
 **				 in the common info) from a SipMessage.
 **	 
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGenerateCallLegKey 
#ifdef ANSI_PROTO
	(SipMessage *pMsg, Sdf_st_callObjectKey **ppKey, Sdf_st_error *pErr)
#else
	(pMsg, ppKey, pErr)
	SipMessage *pMsg;
	Sdf_st_callObjectKey **ppKey;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_s8bit *pTemp = Sdf_co_null;
	SipHeader dHeader;
	Sdf_ty_s8bit *pViaBranch = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaGenerateCallLegKey");

#ifdef SDF_PARAMVALIDATION

	if (pErr == Sdf_co_null)
		return Sdf_co_fail;
	if (ppKey == Sdf_co_null)
		return Sdf_co_fail;
	if (pMsg == Sdf_co_null)
		return Sdf_co_fail;
		
#endif	/* End of param validation ifdef */

	sip_freeSipHeader((*ppKey)->pLocalAddr);	
	
	if ((sip_getHeader(pMsg, SipHdrTypeFrom, \
		(*ppKey)->pLocalAddr, (SipError *)&(pErr->stkErrCode))) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateCallLegKey( ): "
			"Failed to construct the key (from header)",pErr);
#endif		
		return Sdf_co_fail;
	}

	sip_freeSipHeader((*ppKey)->pRemoteAddr);	

	if ((sip_getHeader(pMsg, SipHdrTypeTo, \
		(*ppKey)->pRemoteAddr, (SipError *)&(pErr->stkErrCode))) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateCallLegKey( ): "
			"Failed to construct the key (to header)",pErr);
#endif		
		return Sdf_co_fail;
	}

	if ((sip_getHeader(pMsg, SipHdrTypeCallId, \
		&dHeader, (SipError *)&(pErr->stkErrCode))) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateCallLegKey( ): "
			"Failed to construct the key (callid)",pErr);
#endif		
		return Sdf_co_fail;
	}
	if ((sip_getValueFromCallIdHdr(&dHeader, &pTemp, \
		(SipError *)&(pErr->stkErrCode))) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateCallLegKey( ): "
			"Failed to construct the key (callid)",pErr);
#endif		
		return Sdf_co_fail;
	}
	(*ppKey)->pCallId = Sdf_mc_strdupCallHandler(pTemp);

	sip_freeSipHeader(&dHeader);

	/* Store the Via Branch into the Key */
	if ((sip_getHeader(pMsg, SipHdrTypeVia, \
		&dHeader, (SipError *)&(pErr->stkErrCode))) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGenerateCallLegKey( ): "
			"Failed to get Via header from message)",pErr);
#endif		
			pViaBranch = Sdf_co_null;
	}
	else
	{
		if(sdf_fn_GetBranchFromViaHdr(&dHeader, &pViaBranch, \
				pErr) != Sdf_co_success)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaGenerateCallLegKey( ): "
				"Failed to get Via Branch from Via hdr)",pErr);
#endif		
			pViaBranch = Sdf_co_null;
		}
	}

	if( (*ppKey)->pLocalViaBranch != Sdf_co_null )
	 	(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&((*ppKey)->pLocalViaBranch), pErr);

	if( pViaBranch != Sdf_co_null )
		(*ppKey)->pLocalViaBranch = Sdf_mc_strdupCallHandler(pViaBranch);
	else
		(*ppKey)->pLocalViaBranch = Sdf_co_null;

	sip_freeSipHeader(&dHeader);
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaGenerateCallLegKey");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaRetrieveAddrSpecFromTopmostRoute
 **
 ** DESCRIPTION: This function is used to retrieve the addrspec from the 
 **				 topmost Route header. This is used in FormRequest to set 
 **				 the Request-URI. It first checks the header list of the 
 **				 overlap transinfo structure, and if that does not have a
 **				 Route list, it checks the callObject's processed header list.
 **				 
 **				 Note:
 **				 This function returns success even if a Route header was 
 **				 not found in the call object.
 **	 
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaRetrieveAddrSpecFromTopmostRoute
#ifdef ANSI_PROTO
	(Sdf_st_callObject 		*pObject, 
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	SipAddrSpec 			**ppAddrSpec,
	Sdf_st_error 			*pError)
#else
	(pObject, pOverlapTransInfo, ppAddrSpec, pError)
	Sdf_st_callObject 		*pObject;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	SipAddrSpec 			**ppAddrSpec;
	Sdf_st_error 			*pError;
#endif
{
	Sdf_ty_slist 		slMatchedHeaderList;
	Sdf_st_headerList 	*pHeaderList = Sdf_co_null;
	SipHeader 			*pElement = Sdf_co_null;
	Sdf_ty_u32bit		size=0;
	Sdf_ty_bool 		dRouteFound = Sdf_co_false;
	Sdf_st_error		dLocalErr;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaRetrieveAddrSpecFromTopmostRoute");

	*ppAddrSpec=Sdf_co_null;

	/* 
	 * First check if this is for an overlap transaction. If so,
	 * then use the Route set present in the pOverlapTransInfo
	 * if any.
	 */
	if (pOverlapTransInfo != Sdf_co_null)
	{
		Sdf_st_listIterator dListIterator;

		(void)sdf_listInitIterator(&(pOverlapTransInfo->slProcessedHeaders), \
			&dListIterator, pError);

		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			pHeaderList = (Sdf_st_headerList*)\
				(dListIterator.pCurrentElement->pData);
			
			if (pHeaderList->dType == SipHdrTypeRoute)
			{
				dRouteFound = Sdf_co_true;
				break;
			}
			(void)sdf_listNext(&dListIterator, pError);
		}
	}

	/*
	 * If this is for the main call OR it is for an overlap transaction
	 * but there was no route set in the pOverlapTransInfo, then we use
	 * the "uat-routeset" from the call object's persistent header list.
	 */
	if ( (pOverlapTransInfo == Sdf_co_null) || (dRouteFound == Sdf_co_false) )
	{
		if (sdf_ivk_uaGetHeaderListByCorrelation( \
			&(pObject->slPersistentHeaders),(Sdf_ty_s8bit *) "uat-routeset", \
			&slMatchedHeaderList, pError) == Sdf_co_fail) 
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaRetrieveAddrSpecFromTopmostRoute(): "
				"Failed to get Route header list by correlation "
				"uat-routeset", pError);
#endif				
			return Sdf_co_fail;
		}

		(void)sdf_listSizeOf(&slMatchedHeaderList, &size, pError);

		if (size == 0)
		{
			pError->errCode = Sdf_en_noUaError;
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
				"Exiting sdf_fn_uaRetrieveAddrSpecFromTopmostRoute");
			(void)sdf_listDeleteAll(&slMatchedHeaderList,&dLocalErr);
			return Sdf_co_success;
		}
		
		if (sdf_listGetAt(&(slMatchedHeaderList), 0, \
			(Sdf_ty_pvoid *)&pHeaderList,pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaRetrieveAddrSpecFromTopmostRoute(): "
				"Failed to get Route HeaderList structure", pError);
#endif					
			(void)sdf_listDeleteAll(&slMatchedHeaderList, &dLocalErr);
			return Sdf_co_fail;
		}
	}
	
#ifdef SDF_LINT
	if(pHeaderList != Sdf_co_null)
#endif			
		if (sdf_listGetAt(&(pHeaderList->slHeaders), 0, \
				(Sdf_ty_pvoid *)&pElement,	pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaRetrieveAddrSpecFromTopmostRoute(): "
				"Failed to get Route header list", pError);
#endif				
				if ((pOverlapTransInfo == Sdf_co_null) || (dRouteFound == Sdf_co_false))
						(void)sdf_listDeleteAll(&slMatchedHeaderList, &dLocalErr);
				return Sdf_co_fail;
		}

	if (sip_getAddrSpecFromRouteHdr(pElement, ppAddrSpec, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaRetrieveAddrSpecFromTopmostRoute(): "
			"Failed to get AddrSpec form Route header", pError);
#endif				
		if ((pOverlapTransInfo == Sdf_co_null) || (dRouteFound == Sdf_co_false))
			(void)sdf_listDeleteAll(&slMatchedHeaderList, &dLocalErr);
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}

	if ( (pOverlapTransInfo == Sdf_co_null) || (dRouteFound == Sdf_co_false) )
		(void)sdf_listDeleteAll(&slMatchedHeaderList, pError);

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaRetrieveAddrSpecFromTopmostRoute");
	return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: sdf_fn_uaGetMethodNameAsEnum
 **
 ** DESCRIPTION:  This function returns the enum value for a  method 
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetMethodNameAsEnum
#ifdef ANSI_PROTO
	(const Sdf_ty_s8bit *pMethod, 
	Sdf_ty_messageType *pType,	
	Sdf_st_error *pErr)
#else	
	(pMethod, pType,pErr)
	const Sdf_ty_s8bit *pMethod;
	Sdf_ty_messageType *pType;	
	Sdf_st_error *pErr;
#endif	
{
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
		return Sdf_co_fail;
	if (pMethod == Sdf_co_null)
	{
		pErr->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pType == Sdf_co_null)
	{
		pErr->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	switch (pMethod[0])
	{
		case 'A':
			if (Sdf_mc_strcmp(pMethod,"ACK") == 0)
				*pType = Sdf_en_ack;
			else
				*pType = Sdf_en_unknownRequest;
			break;

		case 'B':
			if (Sdf_mc_strcmp(pMethod,"BYE") == 0)
				*pType = Sdf_en_bye;
			else
				*pType = Sdf_en_unknownRequest;
			break;

		case 'C':
			if (Sdf_mc_strcmp(pMethod,"CANCEL") == 0)
				*pType = Sdf_en_cancel;
			else if (Sdf_mc_strcmp(pMethod,"COMET") == 0)
				*pType = Sdf_en_comet;
			else
				*pType = Sdf_en_unknownRequest;
			break;

		case 'I':
			if (Sdf_mc_strcmp(pMethod,"INVITE") == 0)
				*pType = Sdf_en_invite;
			else if (Sdf_mc_strcmp(pMethod,"INFO") == 0)
				*pType = Sdf_en_info;
			else
				*pType = Sdf_en_unknownRequest;
			break;

#ifdef SDF_IM
		case 'M':
			if (Sdf_mc_strcmp(pMethod,"MESSAGE") == 0)
				*pType = Sdf_en_message;
			else
				*pType = Sdf_en_unknownRequest;
			break;
#endif
#ifdef SIP_IMPP
		case 'N':
			if (Sdf_mc_strcmp(pMethod,"NOTIFY") == 0)
				*pType = Sdf_en_notify;
			else
				*pType = Sdf_en_unknownRequest;
			break;

		case 'S':
			if (Sdf_mc_strcmp(pMethod,"SUBSCRIBE") == 0)
				*pType = Sdf_en_subscribe;
			else
				*pType = Sdf_en_unknownRequest;
			break;
#endif

		case 'O':
			if (Sdf_mc_strcmp(pMethod,"OPTIONS") == 0)
				*pType = Sdf_en_options;
			else
				*pType = Sdf_en_unknownRequest;
			break;
#ifdef SIP_RPR
		case 'P':
			if (Sdf_mc_strcmp(pMethod,"PRACK") == 0)
				*pType = Sdf_en_prack;
			else
				*pType = Sdf_en_unknownRequest;
			break;
#endif

		case 'R':
			if (Sdf_mc_strcmp(pMethod,"REGISTER") == 0)
				*pType = Sdf_en_register;
#ifdef SDF_REFER 
			else if (Sdf_mc_strcmp(pMethod,"REFER") == 0)
				*pType = Sdf_en_refer;
#endif
			else
				*pType = Sdf_en_unknownRequest;
			break;


		case 'U':
			if (Sdf_mc_strcmp(pMethod,"UPDATE") == 0)
				*pType = Sdf_en_update;
			else
				*pType = Sdf_en_unknownRequest;
			break;

		default:
			*pType = Sdf_en_unknownRequest;
	}
	
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCheckSdpInInvite
 **
 ** DESCRIPTION: This function sets the variable dSdpInInvite inside the 
 **				 commonInfo structure if a INVITE request is being sent or 
 **				 recd. with a SDP body.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaCheckSdpInInvite
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, 
	 Sdf_st_msgInfo	dMessageInfo, 
	 SipMessage *pMessage, 
	 Sdf_st_error *pError)
#else
	(pObject, dMessageInfo, pMessage, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_msgInfo	dMessageInfo; 
	SipMessage *pMessage;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_u32bit dMsgBodyCount=0, dIndex=0;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaCheckSdpInInvite");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
		return Sdf_co_fail;
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckSdpInInvite( ): "
			"Invalid param value: Call Object",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */
	
	/* If it is a INVITE request do further processing,
	   else quit here itself without changing the variable */
	if (dMessageInfo.dMsgType == Sdf_en_invite)
	{
		Sdf_st_commonInfo *pCommonInfo;

		/*w534*/ (void)sip_getMsgBodyCount(pMessage, &dMsgBodyCount, \
			(SipError *)&(pError->stkErrCode));

		for (dIndex=0; dIndex<dMsgBodyCount; dIndex++)
		{
			en_SipMsgBodyType dType;

			if ((sip_getMsgBodyTypeAtIndex(pMessage, &dType, dIndex, \
				(SipError *)&(pError->stkErrCode))) == SipFail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_commonInfoAccessError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCheckSdpInInvite( ): "
					"Could not get type of message body from the message", \
					pError);
#endif				
				pError->errCode=Sdf_en_commonInfoAccessError;
				return Sdf_co_fail;
			}
			if (dType == SipSdpBody)
				break;
		}
		if ((sdf_ivk_uaGetCommonInfoFromCallObject(pObject,\
			&pCommonInfo, pError)) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_commonInfoAccessError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCheckSdpInInvite( ): "
					"Could not get common info from the call object",pError);
#endif				
			pError->errCode=Sdf_en_commonInfoAccessError;
			return Sdf_co_fail;
		}
		if (dIndex == dMsgBodyCount)
			pCommonInfo->dSdpInInvite = Sdf_co_false;
		else
			pCommonInfo->dSdpInInvite = Sdf_co_true;
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
	}	

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaCheckSdpInInvite");
	return Sdf_co_success;
}

#ifdef SDF_TRACE
/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaPrintStateChangeTrace
 **
 ** DESCRIPTION: This function prints a trace indicating the state
 **				 change. Used by sdf_ivk_uaChangeCallState internally
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaPrintStateChangeTrace
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *pCallid,
	Sdf_ty_state dCurrState,
	Sdf_ty_state dNextState,
	Sdf_ty_s8bit *pTypeOfState,
	Sdf_st_error *pError)
#else
	(pCallid, dCurrState, dNextState, pTypeOfState, pError)
	Sdf_ty_s8bit *pCallid;
	Sdf_ty_state dCurrState;
	Sdf_ty_state dNextState;
	Sdf_ty_s8bit *pTypeOfState;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize]="";
	Sdf_ty_s8bit pCurrState[Sdf_co_characterBufferSize]="",
	pNextState[Sdf_co_characterBufferSize]="";

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaPrintStateChangeTrace");

	/*w534*/ (void)sdf_fn_getStateName((Sdf_ty_state)dCurrState, pCurrState,Sdf_co_characterBufferSize,\
		pError);
	/*w534*/ (void)sdf_fn_getStateName((Sdf_ty_state)dNextState, pNextState,Sdf_co_characterBufferSize,\
		pError);
	(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,"Call : %s - "
		"Changing %s state from %s to %s", pCallid, pTypeOfState,\
		pCurrState, pNextState);
	sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_callStateTraces, Trace, 0, pError);

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaPrintStateChangeTrace");
	return Sdf_co_success;
}
#endif


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaPerformHeaderListCopyByType
 **
 ** DESCRIPTION: This function copies a headerlist from src to dest (by 
 **				 reference) and then deletes the headerlist from the source
 **				 if the flag dDeleteAfterCopy is passed as Sdf_co_true.
 **
 **				 Note:
 **				 The FIRST match of the type same as dType is copied (and
 **				 deleted from source, if that's desired).
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaPerformHeaderListCopyByType
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pslSrcHeaderList, 
	 Sdf_ty_slist *pslDestHeaderList,
	 en_HeaderType dType, 
	 Sdf_ty_bool dDeleteAfterCopy,
	 Sdf_st_error *pErr)
#else
	(pslSrcHeaderList, pslDestHeaderList, dType, dDeleteAfterCopy, pErr)
	 Sdf_ty_slist *pslSrcHeaderList;
	 Sdf_ty_slist *pslDestHeaderList;
	 en_HeaderType dType;
	 Sdf_ty_bool dDeleteAfterCopy;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit srcListSize=0, iterator=0;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaPerformHeaderListCopyByType");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaPerformHeaderListCopyByType() : Error param passed is"
			" invalid ");
#endif
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	(void)sdf_listSizeOf(pslSrcHeaderList, &srcListSize, pErr);

	/*
	 * Iterate through the source list looking for dType and move the
	 * corresponding HeaderList to the destination list.
	 */
	for(iterator = 0; iterator<srcListSize; iterator++)
	{
		Sdf_st_headerList *pHeaderList;

		if (sdf_listGetAt(pslSrcHeaderList, iterator, \
			(Sdf_ty_pvoid *)&pHeaderList,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaPerformHeaderListCopyByType(): "
				"Getting HeaderList at index from source list failed",pErr);
#endif
			return Sdf_co_fail;
		}

		if(pHeaderList->dType == dType)
		{
			/* 
			 * Header type matches. Remove from source list and append to 
			 * destination list
			 */
			if(sdf_listAppend(pslDestHeaderList, (Sdf_ty_pvoid)pHeaderList, \
				pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaPerformHeaderListCopyByType(): "
					"Failed to append HeaderList to destination list",pErr);
#endif
				return Sdf_co_fail;
			}

			/*
			 * Increment reference count as sdf_listAppend does not do it
			 * automatically
			 */
			HSS_LOCKEDINCREF(pHeaderList->dRefCount);

			/* 
			 * Delete the headerlist from the source list if the flag
			 * dDeleteAfterCopy has been passed as true.
			 */
			if (dDeleteAfterCopy == Sdf_co_true)
			{
				if(sdf_listDeleteAt(pslSrcHeaderList, iterator, \
					pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
						(Sdf_ty_s8bit *)"sdf_fn_uaPerformHeaderListCopyByType:"
						"Error in deleting HeaderList from source list",pErr);
#endif
					return Sdf_co_fail;
				}
			}

			/* 
			 * All done. Break out of the iterator loop
			 */
			break;
		}
	}

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaPerformHeaderListCopyByType");
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaPerformHeaderListCopyByCorrelation
 **
 ** DESCRIPTION: This function copies a headerlist from src to dest (by 
 **				 reference) and then deletes the headerlist from the source
 **				 if the flag dDeleteAfterCopy is passed as Sdf_co_true.
 **
 **				 Note:
 **				 The FIRST match with the correlation same as the correlation
 **				 passed to this API is copied (and deleted from source, if 
 **				 that's desired).
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaPerformHeaderListCopyByCorrelation
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pslSrcHeaderList, 
	Sdf_ty_slist  *pslDestHeaderList, 
	Sdf_ty_s8bit  *pCorrelation, 
	Sdf_ty_bool   dDeleteAfterCopy,
	Sdf_st_error  *pErr)
#else
	(pslSrcHeaderList, pslDestHeaderList, pCorrelation, dDeleteAfterCopy, pErr)
	Sdf_ty_slist *pslSrcHeaderList;
	Sdf_ty_slist *pslDestHeaderList;
	Sdf_ty_s8bit *pCorrelation;
	Sdf_ty_bool  dDeleteAfterCopy,
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit srcListSize=0, iterator=0;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		"sdf_fn_uaPerformHeaderListCopyByCorrelation");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaPerformHeaderListCopyByCorrelation() : Error param "
			"passed is invalid.");
#endif
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */
	
	(void)sdf_listSizeOf(pslSrcHeaderList, &srcListSize, pErr);

	/*
	 * Iterate through the source list looking for pCorrelation and move the
	 * corresponding HeaderList to the destination list.
	 */
	for(iterator = 0; iterator<srcListSize; iterator++)
	{
		Sdf_st_headerList *pHeaderList;

		if(sdf_listGetAt(pslSrcHeaderList, iterator,\
			(Sdf_ty_pvoid *)&pHeaderList, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaPerformHeaderListCopyByCorrelation:"
				"Getting HeaderList at index from source header list failed",\
				pErr);
#endif
			return Sdf_co_fail;
		}

		if (Sdf_mc_strcasecmp(pHeaderList->pCorrelation, pCorrelation) == 0)
		{
			/* 
			 * Correlation string matches. Remove from source list and 
			 * append to destination list 
			 */
			if(sdf_listAppend(pslDestHeaderList, (Sdf_ty_pvoid) pHeaderList, \
				pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaPerformHeaderListCopyByCorrelation(): "
					"Failed to append HeaderList to destination list",pErr);
#endif
				return Sdf_co_fail;
			}

			/*
			 * Increment the reference count as sdf_listAppend does not do this
			 * automatically
			 */
			HSS_LOCKEDINCREF(pHeaderList->dRefCount);

			/* 
			 * Delete the headerlist from the source list if the flag
			 * dDeleteAfterCopy has been passed as true.
			 */
			if (dDeleteAfterCopy == Sdf_co_true)
			{
				if(sdf_listDeleteAt(pslSrcHeaderList, iterator, \
					pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaPerformHeaderListCopyByCorrelation(): "
					"Failed to delete element at index from source list",pErr);
#endif
					return Sdf_co_fail;
				}
			}

			/* 
			 * All done. Break out of the iterator loop
			 */
			break;
		}
	}

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_fn_uaPerformHeaderListCopyByCorrelation");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaChangeCallStateRequest
 **
 ** DESCRIPTION: This function changes the call state of the call object
 **					for incoming/outgoing SIP request messages
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaChangeCallStateRequest
#ifdef ANSI_PROTO
(Sdf_st_callObject *pObject,
 Sdf_st_overlapTransInfo *pOverlapTransInfo,
 SipMessage *pSipMsg,
 Sdf_ty_msgMode dMode,
 Sdf_st_error *pErr)
#else
 (pObject, pOverlapTransInfo, pSipMsg, dMode, pErr)
 Sdf_st_callObject *pObject;
 Sdf_st_overlapTransInfo *pOverlapTransInfo;
 SipMessage *pSipMsg;
 Sdf_ty_msgMode dMode;
  Sdf_st_error *pErr;
#endif
{
	Sdf_ty_s8bit *pMethod = Sdf_co_null;
    Sdf_ty_messageType dMsgType = Sdf_en_unknownRequest;
	Sdf_st_callInfo *pCallInfo = Sdf_co_null;
    Sdf_ty_messageType dUpdatedMsgType = Sdf_en_unknownRequest;

	(void)pOverlapTransInfo;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_fn_uaChangeCallStateRequest");

	/*
	 * Figure out the message type baseed on dMode and the method of the 
	 * SIP request. For Sdf_en_msgModeCancelRecd and Sdf_en_msgModeByeRecd, 
	 * the message is not updated in the call object. Hence, force the dMsgType
	 * to Sdf_en_cancel and Sdf_en_bye respectively.
	 */
	if (dMode == Sdf_en_msgModeCancelRecd)
		dMsgType = Sdf_en_cancel;
	else if (dMode == Sdf_en_msgModeByeRecd)
		dMsgType = Sdf_en_bye;
	else	
	{
		if (sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, \
			&pMethod, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateRequest(): "
				"Getting Method from SipMessage failed",pErr);
#endif
			return Sdf_co_fail;
		}
		
		/*
		 * Set the dMsgType depending on the request method received.
		 */
		/*w534*/ (void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, pErr);
	}

	if (sdf_ivk_uaGetCallInfoFromCallObject(pObject, \
				&pCallInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,
			Sdf_en_callObjectAccessError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateRequest(): "
			"Getting CallInfo from CallObject failed",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	/* 
	 * For Sdf_en_msgModeCancelRecd and Sdf_en_msgModeByeRecd, set
	 * the mode to Sdf_en_msgModeRecd
	 */
	if ((dMode == Sdf_en_msgModeCancelRecd) || (dMode == Sdf_en_msgModeByeRecd))
		dMode = Sdf_en_msgModeRecd;

	/* 
	 * If mode == Sdf_en_msgModeSend, look up the transition table
	 * mapping states to messages sent; else look up the transition
	 * table mapping states to messages received.
	 */
	if (dMode == Sdf_en_msgModeSend)
	{
		if ((dMsgType == Sdf_en_invite) || \
			(dMsgType == Sdf_en_ack) || \
			(dMsgType == Sdf_en_bye) || \
			(dMsgType == Sdf_en_cancel) )
		{
			if((pObject->pUasTransaction->dTransactionState\
					!=Sdf_en_idle) && (dMsgType==Sdf_en_cancel))
			{
#ifdef SDF_STATISTICS
				if (sdf_fn_uaUpdateStatistics(dMode,pObject->pUasTransaction\
					->dTransactionState,dMsgType, pErr)\
					== Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_invalidStatsType,\
						(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateRequest(): "
						"Failed to update statistics", pErr);
#endif
					pErr->errCode=Sdf_en_invalidStatsType;
					(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
					return Sdf_co_fail;
				}
#endif
#ifdef SDF_TRACE
				(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo->\
					pKey->pCallId,pObject->pUasTransaction->dTransactionState, \
					(Sdf_ty_state)dCsmNxtStateOnMsgRecd[dMsgType]\
					[pObject->pUasTransaction->dTransactionState].dNextState,\
					(Sdf_ty_s8bit *) "call", pErr);
#endif
				pObject->pUasTransaction->dTransactionState=\
					dCsmNxtStateOnMsgRecd[dMsgType]\
					[pObject->pUasTransaction->dTransactionState].dNextState;	
			}
			else
			{
				/*
				 * If the message sent and the current state map to a
				 * valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgSent[dMsgType][pCallInfo->dState] \
					!= Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_STATISTICS
					if (sdf_fn_uaUpdateStatistics(dMode, pCallInfo->dState,\
						dMsgType, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,
							Sdf_en_invalidStatsType,\
							(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateRequest():"
							"Failed to update statistics", pErr);
#endif
						pErr->errCode=Sdf_en_invalidStatsType;
						(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
						return Sdf_co_fail;
					}
#endif
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo->\
						pKey->pCallId, pCallInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgSent[dMsgType]\
						[pCallInfo->dState],(Sdf_ty_s8bit *) "call", pErr);
#endif
					pCallInfo->dState = (Sdf_ty_state)\
						dCsmNxtStateOnMsgSent[dMsgType][pCallInfo->dState];	
				}
			}
		}
		else
		{
			dUpdatedMsgType = dMsgType;
			if( pOverlapTransInfo == Sdf_co_null )
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,
					Sdf_en_transactionFailure,\
					(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateRequest(): "
					"Failed to To get the Overlap Transaction", pErr);
#endif
				pErr->errCode=Sdf_en_transactionFailure;
				(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
				return Sdf_co_fail;
			}
#ifdef SIP_RPR
			if((dMsgType != Sdf_en_prack) && (dMsgType != Sdf_en_comet))
#else
			if(dMsgType != Sdf_en_comet)
#endif
				dUpdatedMsgType = Sdf_en_RegularTxn;

			if (dCsmNxtStateOnMsgSent[dUpdatedMsgType]\
					[pOverlapTransInfo->dState] != \
				   	Sdf_en_invalidStateTransition)
			{
#ifdef SDF_STATISTICS
				if (sdf_fn_uaUpdateStatistics(dMode, pOverlapTransInfo->dState,\
					dUpdatedMsgType, pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidStatsType,\
						(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateRequest():"
						"Failed to update statistics", pErr);
#endif
					pErr->errCode=Sdf_en_invalidStatsType;
					(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
					return Sdf_co_fail;
				}
#endif

#ifdef SDF_TRACE
				(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo->\
					pKey->pCallId, pOverlapTransInfo->dState, \
					(Sdf_ty_state)dCsmNxtStateOnMsgSent[dUpdatedMsgType]\
					[pOverlapTransInfo->dState], \
					(Sdf_ty_s8bit *)"overlap transaction", pErr);
#endif
				pOverlapTransInfo->dState = (Sdf_ty_state)\
					dCsmNxtStateOnMsgSent[dUpdatedMsgType]\
					[pOverlapTransInfo->dState];	
			}
		}
	} /* End of if block for Sdf_en_msgModeSend */	
	else
	/*
	 * Handling for messages received (Sdf_en_msgModeRecd)
	 */
	{
		if ((dMsgType == Sdf_en_invite) || \
			(dMsgType == Sdf_en_ack) || \
			(dMsgType == Sdf_en_bye) || \
			(dMsgType == Sdf_en_cancel) )
		{
			if((pObject->pUasTransaction->dTransactionState\
					!=Sdf_en_idle) && (dMsgType==Sdf_en_cancel))
			{
#ifdef SDF_STATISTICS
				if (sdf_fn_uaUpdateStatistics(dMode,pObject->pUasTransaction\
					->dTransactionState,dMsgType, pErr)\
					== Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_invalidStatsType,\
						(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateRequest(): "
						"Failed to update statistics", pErr);
#endif
					pErr->errCode=Sdf_en_invalidStatsType;
					(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
					return Sdf_co_fail;
				}
#endif
#ifdef SDF_TRACE
				(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo->\
					pKey->pCallId,pObject->pUasTransaction->dTransactionState, \
					(Sdf_ty_state)dCsmNxtStateOnMsgRecd[dMsgType]\
					[pObject->pUasTransaction->dTransactionState].dNextState,\
					(Sdf_ty_s8bit *) "call", pErr);
#endif
				pObject->pUasTransaction->dTransactionState=\
					dCsmNxtStateOnMsgRecd[dMsgType]\
					[pObject->pUasTransaction->dTransactionState].dNextState;	
			}
			else
			{
				/*
				 * If the message received and the current state map to a
				 * valid state transition, then change the state.
				 */
				if ( dCsmNxtStateOnMsgRecd[dMsgType][pCallInfo->dState].\
					dNextState != Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_STATISTICS
					if (sdf_fn_uaUpdateStatistics(dMode, pCallInfo->dState,\
						dMsgType, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,
							Sdf_en_invalidStatsType,\
							(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateRequest(): "
							"Failed to update statistics", pErr);
#endif
						pErr->errCode=Sdf_en_invalidStatsType;
						(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
						return Sdf_co_fail;
					}
#endif

#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo->\
						pKey->pCallId, pCallInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgRecd[dMsgType]\
						[pCallInfo->dState].dNextState,(Sdf_ty_s8bit *)\
						"call", pErr);
#endif

#ifdef SDF_UASSF
					{
						Sdf_ty_state dState;

						/* If we receive a BYE in 200 OK received state
						 * (either for INVITE or re-INVITE), we 
						 * need to abort the transaction as we will not be 
						 * able to send ACK and the transaction at stack will 
						 * be hanging */
						dState = pCallInfo->dState;
						if ((dMsgType == Sdf_en_bye) && 	\
							((pCallInfo->dState == \
							  Sdf_en_finalSuccessReceived) || \
							 (pCallInfo->dState == \
							  Sdf_en_reInviteFinalResponseReceived)))
						{
							/*w534*/ (void)sdf_ivk_uaAbortTransaction (pObject,\
								pObject->pUacTransaction->dTxnId,Sdf_co_null, \
								pErr);
						}
						pCallInfo->dState =dCsmNxtStateOnMsgRecd[dMsgType]\
							[dState].dNextState;
					}
#else
					pCallInfo->dState =dCsmNxtStateOnMsgRecd[dMsgType]\
						[pCallInfo->dState].dNextState;
#endif /* SDF_UASSF */
				}
			}
		}
		else	
		{
			dUpdatedMsgType = dMsgType;
			/*
			 * If the message received and the current state map to a
			 * valid state transition, then change the state.
			 */
			if( pOverlapTransInfo == Sdf_co_null )
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure,\
					(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateRequest(): "
					"Failed to get Overlap Transaction structure", pErr);
#endif
					pErr->errCode=Sdf_en_transactionFailure;
					(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
					return Sdf_co_fail;
			}
#ifdef SIP_RPR
			if((dMsgType != Sdf_en_prack) && (dMsgType != Sdf_en_comet))
#else
			if(dMsgType != Sdf_en_comet)
#endif
				dUpdatedMsgType = Sdf_en_RegularTxn;
				
			if (dCsmNxtStateOnMsgRecd[dUpdatedMsgType][pOverlapTransInfo->\
				dState].dNextState != Sdf_en_invalidStateTransition)
			{
#ifdef SDF_STATISTICS
				if (sdf_fn_uaUpdateStatistics(dMode, pOverlapTransInfo->dState,\
					dUpdatedMsgType, pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidStatsType,\
						(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateRequest():"
						"Failed to update statistics", pErr);
#endif
					pErr->errCode=Sdf_en_invalidStatsType;
					(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
					return Sdf_co_fail;
				}
#endif

#ifdef SDF_TRACE
				(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo->\
					pKey->pCallId, pOverlapTransInfo->dState, \
					(Sdf_ty_state)dCsmNxtStateOnMsgRecd[dUpdatedMsgType]\
					[pOverlapTransInfo->dState].dNextState, \
					(Sdf_ty_s8bit *)"overlap transaction", pErr);
#endif
				pOverlapTransInfo->dState = (Sdf_ty_state)\
					dCsmNxtStateOnMsgRecd[dUpdatedMsgType]\
					[pOverlapTransInfo->dState].dNextState;
			}
		}
	}	
	(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_fn_uaChangeCallStateRequest");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaChangeCallStateResponse
 **
 ** DESCRIPTION: This function changes the call state of the call object
 **					for incoming/outgoing SIP response messages
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaChangeCallStateResponse
#ifdef ANSI_PROTO
(Sdf_st_callObject *pObject,
 Sdf_st_overlapTransInfo *pOverlapTransInfo,
 SipMessage *pSipMsg,
 Sdf_ty_msgMode dMode,
 Sdf_st_error *pErr)
#else
 (pObject, pOverlapTransInfo, pSipMsg, dMode, pErr)
 Sdf_st_callObject *pObject;
 Sdf_st_overlapTransInfo *pOverlapTransInfo;
 SipMessage *pSipMsg;
 Sdf_ty_msgMode dMode;
 Sdf_st_error *pErr;
#endif
{ 
	Sdf_ty_u16bit dRespCode=0;
	Sdf_ty_s8bit *pMethod = Sdf_co_null;
	Sdf_ty_messageType dMethodType = Sdf_en_unknownRequest;
	Sdf_st_callInfo *pCallInfo = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_fn_uaChangeCallStateResponse");

	/*
	 * Retrieve the response code and method from the SipMessage
	 */
	if (sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, &pMethod, pErr) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateResponse(): "
			"Getting Method from SipMessage failed",pErr);
#endif
		return Sdf_co_fail;
	}

	/*w534*/ (void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMethodType, pErr);

	if (sdf_ivk_uaGetRespCodeFromSipMessage(pSipMsg, &dRespCode, pErr) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateResponse(): "
			"Getting RespCode from SipMessage failed",pErr);
#endif
		return Sdf_co_fail;
	}


	if (sdf_ivk_uaGetCallInfoFromCallObject(\
		pObject, &pCallInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateResponse(): "
			"Getting CallInfo from CallObject failed", pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}
 
    
	if ( (dRespCode>=100) && (dRespCode<200) &&(dMethodType==Sdf_en_invite))
	{
		if (pOverlapTransInfo == Sdf_co_null)
		{
			/* On receiving the 1xx change the state to
			 * provisional Response received or sent depending upon
			 * dMode */
			if (dMode == Sdf_en_msgModeSend) 
			{
				/*
				 * If the message sent and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgSent[Sdf_en_provisionalResp]\
					[pCallInfo->dState] != Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_STATISTICS
					if (sdf_fn_uaUpdateStatistics(dMode, pCallInfo->dState,\
						Sdf_en_provisionalResp, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,
							Sdf_en_invalidStatsType,\
							(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateResponse():"
							" Failed to update statistics", pErr);
#endif
						pErr->errCode=Sdf_en_invalidStatsType;
						(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
						return Sdf_co_fail;
					}
#endif
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pCallInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgSent\
						[Sdf_en_provisionalResp][pCallInfo->dState], \
						(Sdf_ty_s8bit *)"call", pErr);
#endif
					pCallInfo->dState = (Sdf_ty_state)\
						dCsmNxtStateOnMsgSent[Sdf_en_provisionalResp]\
						[pCallInfo->dState];
				}
			}
			/*  Handling for messages received (Sdf_en_msgModeRecd) */
			else if (dMode == Sdf_en_msgModeRecd)
			{
				/*  Event: Receiving 1xx to INVITE */
				/*
				 * If the message received and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp]\
					[pCallInfo->dState].dNextState != \
					Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_STATISTICS
					if (sdf_fn_uaUpdateStatistics(dMode, pCallInfo->dState,\
						Sdf_en_provisionalResp, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,
							Sdf_en_invalidStatsType,\
							(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateResponse:"
							" Failed to update statistics", pErr);
#endif
						pErr->errCode=Sdf_en_invalidStatsType;
						(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
						return Sdf_co_fail;
					}
#endif
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pCallInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgRecd\
						[Sdf_en_provisionalResp][pCallInfo->dState].dNextState\
						, (Sdf_ty_s8bit *)"call", pErr);
#endif
					pCallInfo->dState = dCsmNxtStateOnMsgRecd\
						[Sdf_en_provisionalResp][pCallInfo->\
						dState].dNextState;
				}	
			}
		}
#ifdef SIP_RPR		
		else
		{
			/* If method is Invite then this is RPR.  We need to change 
			 * the main call state to provisional response received */
			if (dMode == Sdf_en_msgModeSend)
			{
				/*
				 * If the message sent and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgSent[Sdf_en_provisionalResp]\
						[pCallInfo->dState] != Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_STATISTICS
					if (sdf_fn_uaUpdateStatistics(dMode, \
						pCallInfo->dState, Sdf_en_provisionalResp, \
						pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,
							Sdf_en_invalidStatsType, (Sdf_ty_s8bit *) \
							"sdf_fn_uaChangeCallStateResponse():"
							" Failed to update statistics", pErr);
#endif
						pErr->errCode=Sdf_en_invalidStatsType;
						(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
						return Sdf_co_fail;
					}
#endif
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace( \
						pObject->pCommonInfo->pKey->pCallId, \
						pCallInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgSent\
						[Sdf_en_provisionalResp][pCallInfo->dState], \
						(Sdf_ty_s8bit *)"call", pErr);
#endif
					pCallInfo->dState = (Sdf_ty_state)\
					   dCsmNxtStateOnMsgSent[Sdf_en_provisionalResp]\
					   [pCallInfo->dState];
				}
			}
			/*  Handling for messages received (Sdf_en_msgModeRecd) */
			else if (dMode == Sdf_en_msgModeRecd)
			{
				 /* If the message received and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp]\
					[pCallInfo->dState].dNextState != \
					Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_STATISTICS
					if (sdf_fn_uaUpdateStatistics(dMode,pCallInfo->dState,\
						Sdf_en_provisionalResp, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,
							Sdf_en_invalidStatsType, (Sdf_ty_s8bit *)\
							"sdf_fn_uaChangeCallStateResponse():"
							" Failed to update statistics", pErr);
#endif
						pErr->errCode=Sdf_en_invalidStatsType;
						(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
						return Sdf_co_fail;
					}
#endif
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace( \
						pObject->pCommonInfo->pKey->pCallId, \
						pCallInfo->dState, (Sdf_ty_state) \
						dCsmNxtStateOnMsgRecd[Sdf_en_provisionalResp]\
						[pCallInfo->dState].dNextState, \
						(Sdf_ty_s8bit *)"call", pErr);
#endif
					pCallInfo->dState = dCsmNxtStateOnMsgRecd\
						[Sdf_en_provisionalResp][pCallInfo->dState].dNextState;
				}	
			}
			if (dMode == Sdf_en_msgModeSend)
			{
				/*
				 * If the message sent and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgSent[Sdf_en_rpr]\
					[pOverlapTransInfo->dState] != \
					Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pOverlapTransInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgSent[Sdf_en_rpr]\
						[pOverlapTransInfo->dState], \
						(Sdf_ty_s8bit *)"overlap transaction", pErr);
#endif
					pOverlapTransInfo->dState = (Sdf_ty_state)\
						dCsmNxtStateOnMsgSent[Sdf_en_rpr]\
						[pOverlapTransInfo->dState];
				}
			} /* End of if block handling Sdf_en_msgModeSend */
			else
			/*
			 * Handling for messages received (Sdf_en_msgModeRecd)
			 */
			{
				/*
				 * If the message received and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgRecd[Sdf_en_rpr]\
					[pOverlapTransInfo->dState].dNextState != \
					Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pOverlapTransInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgRecd[Sdf_en_rpr]\
						[pOverlapTransInfo->dState].dNextState, \
						(Sdf_ty_s8bit *)"overlap transaction", pErr);
#endif
					pOverlapTransInfo->dState = dCsmNxtStateOnMsgRecd\
						[Sdf_en_rpr][pOverlapTransInfo->\
						dState].dNextState;
				}	
			}
		}
#endif /* SIP_RPR */
    } /* End of if block handling 1xx responses */
	else if ( (dRespCode >= 200) && (dRespCode <= 299) )
	{
		/* 
		 * If mode == Sdf_en_msgModeSend, look up the transition table
		 * mapping states to messages sent; else look up the transition
		 * table mapping states to messages received.
		 */
		if (dMode == Sdf_en_msgModeSend)
		{
			if ((dMethodType == Sdf_en_invite) || \
				(dMethodType == Sdf_en_ack) || \
				(dMethodType == Sdf_en_bye) || \
				(dMethodType == Sdf_en_cancel) )
			{
				/*
				 * Event: Sending 2xx to INVITE/BYE/CANCEL
				 */
				
				/*
				 * If the message sent and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess]\
					[pCallInfo->dState] != Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_STATISTICS
					if (sdf_fn_uaUpdateStatistics(dMode, pCallInfo->dState,\
						Sdf_en_finalSuccess, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,
							Sdf_en_invalidStatsType,\
							(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateResponse():"
							" Failed to update statistics", pErr);
#endif
						pErr->errCode=Sdf_en_invalidStatsType;
						(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
						return Sdf_co_fail;
					}
#endif
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pCallInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgSent\
						[Sdf_en_finalSuccess][pCallInfo->dState], \
						(Sdf_ty_s8bit *)"call", pErr);
#endif
					pCallInfo->dState = (Sdf_ty_state)\
						dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess]\
						[pCallInfo->dState];
				}
			}
			else if(pOverlapTransInfo != Sdf_co_null)
			{
				/*
				 * Event: Sending 2xx to PRACK/COMET/RegularTransaction
				 */

				/*
				 * If the message sent and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess]\
					[pOverlapTransInfo->dState] != \
					Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pOverlapTransInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgSent\
						[Sdf_en_finalSuccess]\
						[pOverlapTransInfo->dState], \
						(Sdf_ty_s8bit *)"overlap transaction", pErr);
#endif
					pOverlapTransInfo->dState = (Sdf_ty_state)\
						dCsmNxtStateOnMsgSent[Sdf_en_finalSuccess]\
						[pOverlapTransInfo->dState];
				}
			}
		} /* End of if block handling Sdf_en_msgModeSend */	
		else		
		/*
		 * Handling for messages received (Sdf_en_msgModeRecd)
		 */
		{
			if ((dMethodType == Sdf_en_invite) || \
				(dMethodType == Sdf_en_ack) || \
				(dMethodType == Sdf_en_bye) || \
				(dMethodType == Sdf_en_cancel) )
			{
				/*
				 * Event: Receiving 2xx to INVITE/BYE/CANCEL
				 */

				/*
				 * If the message received and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess]\
					[pCallInfo->dState].dNextState != \
					Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_STATISTICS
					if (sdf_fn_uaUpdateStatistics(dMode, pCallInfo->dState,\
						Sdf_en_finalSuccess, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,
							Sdf_en_invalidStatsType,\
							(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateResponse:"
							" Failed to update statistics", pErr);
#endif
						pErr->errCode=Sdf_en_invalidStatsType;
						(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
						return Sdf_co_fail;
					}
#endif
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pCallInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgRecd\
						[Sdf_en_finalSuccess][pCallInfo->dState].dNextState\
						, (Sdf_ty_s8bit *)"call", pErr);
#endif
					pCallInfo->dState = dCsmNxtStateOnMsgRecd\
						[Sdf_en_finalSuccess][pCallInfo->\
						dState].dNextState;
				}	
			}
			else if(pOverlapTransInfo != Sdf_co_null)
			{
                /*
				 * Event: Receiving 2xx to PRACK/COMET/RegularTransaction
				 */
				/*
				 * If the message received and the current state map 
				 * to a valid state transition, then change the state.
				 */

				if (dCsmNxtStateOnMsgRecd[Sdf_en_finalSuccess]\
					[pOverlapTransInfo->dState].dNextState != \
					Sdf_en_invalidStateTransition)
				{
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pOverlapTransInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgRecd\
						[Sdf_en_finalSuccess]\
						[pOverlapTransInfo->dState].dNextState, \
						(Sdf_ty_s8bit *)"overlap transaction", pErr);
#endif
					pOverlapTransInfo->dState = dCsmNxtStateOnMsgRecd\
						[Sdf_en_finalSuccess][pOverlapTransInfo->\
						dState].dNextState;
				}	
			}
		}	
	} /* End of if block handling 2xx responses */
	else if (dRespCode >= 300)	
	{
		/* 
		 * If mode == Sdf_en_msgModeSend, look up the transition table
		 * mapping states to messages sent; else look up the transition
		 * table mapping states to messages received.
		 */
		if (dMode == Sdf_en_msgModeSend)
		{
			if ((dMethodType == Sdf_en_invite) || \
				(dMethodType == Sdf_en_ack) || \
				(dMethodType == Sdf_en_bye) || \
				(dMethodType == Sdf_en_cancel))
			{
				/*
				 * Event: Sending 3xx/4xx/5xx/6xx to INVITE/BYE/CANCEL
				 */

				/*
				 * If the message sent and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgSent[Sdf_en_finalFailure]\
					[pCallInfo->dState] != \
					Sdf_en_invalidStateTransition)
				{
#ifdef SDF_STATISTICS
					if (sdf_fn_uaUpdateStatistics(dMode, pCallInfo->dState,\
						Sdf_en_finalFailure, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,
							Sdf_en_invalidStatsType,\
							(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateResponse:"
							" Failed to update statistics", pErr);
#endif
						pErr->errCode=Sdf_en_invalidStatsType;
						(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
						return Sdf_co_fail;
					}
#endif
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pCallInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgSent\
						[Sdf_en_finalFailure][pCallInfo->dState], \
						(Sdf_ty_s8bit *)"call", pErr);
#endif	
					pCallInfo->dState = (Sdf_ty_state)\
						dCsmNxtStateOnMsgSent\
						[Sdf_en_finalFailure][pCallInfo->dState];
				}	
			}
			else if(pOverlapTransInfo != Sdf_co_null)
			{
				/*
				 * Event: Sending 3xx/4xx/5xx/6xx to PRACK/COMET/RegularTxn
				 */
				/*
				 * If the message sent and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgSent[Sdf_en_finalFailure]\
					[pOverlapTransInfo->dState] != \
					Sdf_en_invalidStateTransition)
				{
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pOverlapTransInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgSent\
						[Sdf_en_finalFailure]\
						[pOverlapTransInfo->dState], \
						(Sdf_ty_s8bit *)"overlap transaction", pErr);
#endif	
					pOverlapTransInfo->dState = (Sdf_ty_state)\
						dCsmNxtStateOnMsgSent[Sdf_en_finalFailure]\
						[pOverlapTransInfo->dState];
				}	
			}
		} /* End of if block handling Sdf_en_msgModeSend */
		else		
		/*
		 * Handling for messages received (Sdf_en_msgModeRecd)
		 */
		{
			if ((dMethodType == Sdf_en_invite) || \
				(dMethodType == Sdf_en_ack) || \
				(dMethodType == Sdf_en_bye) || \
				(dMethodType == Sdf_en_cancel))
			{
				/*
				 * Event: Receiving 3xx/4xx/5xx/6xx to INVITE/BYE/CANCEL
				 */

				/*
				 * If the message received and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure]\
					[pCallInfo->dState].dNextState != \
					Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_STATISTICS
					if (sdf_fn_uaUpdateStatistics(dMode, pCallInfo->dState,\
						Sdf_en_finalFailure, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,
							Sdf_en_invalidStatsType,\
							(Sdf_ty_s8bit *)"sdf_fn_uaChangeCallStateResponse:"
							" Failed to update statistics", pErr);
#endif
						pErr->errCode=Sdf_en_invalidStatsType;
						(void)sdf_ivk_uaFreeCallInfo(pCallInfo);
						return Sdf_co_fail;
					}
#endif
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pCallInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgRecd\
						[Sdf_en_finalFailure][pCallInfo->dState].\
						dNextState,(Sdf_ty_s8bit *) "call", pErr);
#endif	
					pCallInfo->dState = dCsmNxtStateOnMsgRecd\
						[Sdf_en_finalFailure][pCallInfo->dState].\
						dNextState;
				}
			}
			else if(pOverlapTransInfo != Sdf_co_null)
			{
				/*
				 * Event: Receiving 3xx/4xx/5xx/6xx to PRACK/COMET/RegularTxn
				 */
				/*
				 * If the message received and the current state map 
				 * to a valid state transition, then change the state.
				 */
				if (dCsmNxtStateOnMsgRecd[Sdf_en_finalFailure]\
					[pOverlapTransInfo->dState].dNextState != \
					Sdf_en_invalidStateTransition)
				{	
#ifdef SDF_TRACE
					(void)sdf_fn_uaPrintStateChangeTrace(pObject->pCommonInfo\
						->pKey->pCallId, pOverlapTransInfo->dState, \
						(Sdf_ty_state)dCsmNxtStateOnMsgRecd\
						[Sdf_en_finalFailure]\
						[pOverlapTransInfo->dState].dNextState, \
						(Sdf_ty_s8bit *)"overlap transaction", pErr);
#endif	
					pOverlapTransInfo->dState = dCsmNxtStateOnMsgRecd\
						[Sdf_en_finalFailure][pOverlapTransInfo->\
						dState].dNextState;
				}
			}
		}
	} /* End of if block handling 3xx,4xx,5xx,6xx responses */	

	(void)sdf_ivk_uaFreeCallInfo(pCallInfo);

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_fn_uaChangeCallStateResponse");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCloneSipHeadersList
 **
 ** DESCRIPTION: This function clones a list of SipHeader structures from
 **					the source list to the destination list
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaCloneSipHeadersList
#ifdef ANSI_PROTO
(Sdf_ty_slist *pDestList,
 Sdf_ty_slist *pSrcList,
 Sdf_st_error *pErr)
#else
 (pDestList, pSrcList pErr);
Sdf_ty_slist *pDestList;
Sdf_ty_slist *pSrcList;
Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_fn_uaCloneSipHeadersList");

	(void)sdf_listInitIterator(pSrcList, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SipHeader *pSrcHeader, *pDestHeader;

		pSrcHeader = (SipHeader*)\
			(dListIterator.pCurrentElement->pData);

		if (sip_initSipHeader(&pDestHeader, pSrcHeader->dType, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCloneSipHeadersList():  "
				"Failed to  initialize SipHeader",pErr);	
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if (sip_cloneSipHeader(pDestHeader, pSrcHeader, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCloneSipHeadersList():  "
				"Failed to clone SipHeader",pErr);	
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if (sdf_listAppend(pDestList, pDestHeader, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCloneSipHeadersList():  "
				"Failed to append SipHeader to destination list",pErr);	
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_fn_uaCloneSipHeadersList");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaConvertHexEncodedString
 **
 ** DESCRIPTION: This function takes a string as input and converts any
 **					characters encoded using % HEX HEX to their equivalent
 **					character
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaConvertHexEncodedString
#ifdef ANSI_PROTO
(Sdf_ty_s8bit *pInput, Sdf_ty_s8bit *pOutput,Sdf_ty_u32bit dLen)
#else
(pInput, pOutput,dLen)
Sdf_ty_s8bit *pInput;
Sdf_ty_s8bit *pOutput;
Sdf_ty_u32bit dLen;
#endif
{
	Sdf_ty_s8bit ch;
	Sdf_ty_u32bit dIndex = 0, dJindex = 0, len;
    /*CSR 1-7574892 change start*/
    Sdf_ty_u8bit asc=0;
    Sdf_ty_bool    isFirstHexFound = Sdf_co_false, isSecondHexFound = Sdf_co_false;
    /*CSR 1-7574892 change end*/


	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_fn_uaConvertHexEncodedString");

	pOutput[0] = '\0';

	if (pInput == Sdf_co_null)
		return Sdf_co_success;

	len = Sdf_mc_strlen(pInput);

	if(len > dLen)
			len=dLen;

	for (dIndex = 0; dIndex < len; dIndex++)
	{
		if (pInput[dIndex] != '%')
		{
			pOutput[dJindex] = pInput[dIndex];
		}
		else
        {
            /*CSR 1-7574892 change start*/
            for(asc =48;asc<58;asc++)
            {
                if (dIndex+1<=len)
                {
                    if (pInput[dIndex+1] == asc) 
                    {
                        isFirstHexFound = Sdf_co_true;
                    }
                }
                if (dIndex+2<=len)
                {
                    if (pInput[dIndex+2] == asc)
                    {
                        isSecondHexFound = Sdf_co_true;
                    }
                }
            }
            if (isFirstHexFound == Sdf_co_false)
            {
                for(asc =65;asc<71;asc++)
                {
                    if (dIndex+1<=len)
                    {

                        if (pInput[dIndex+1] == asc)
                        { 
                            isFirstHexFound = Sdf_co_true;
                        }
                    }

                }
                for(asc =97;asc<103;asc++)
                {
                    if (dIndex+1<=len)
                    {

                        if (pInput[dIndex+1] == asc)
                        { 
                            isFirstHexFound = Sdf_co_true;
                        }
                    }

                }

            }
            if (isSecondHexFound == Sdf_co_false)
            {
                for(asc =65;asc<71;asc++)
                {
                    if (dIndex+2<=len)
                    {
                        if (pInput[dIndex+2] == asc)
                        { 
                            isSecondHexFound = Sdf_co_true;
                        }
                    }

                }
                for(asc =97;asc<103;asc++)
                {
                    if (dIndex+2<=len)
                    {
                        if (pInput[dIndex+2] == asc)
                        { 
                            isSecondHexFound = Sdf_co_true;
                        }
                    }

                }

            }

            if ((isFirstHexFound == Sdf_co_false)|| \
                    (isSecondHexFound == Sdf_co_false))
                return Sdf_co_fail;       
            /*CSR 1-7574892 change end*/

            ch = (pInput[dIndex+1] >= 'A' ? \
                    ((pInput[dIndex+1] & 0xdf)-'A')+10 : (pInput[dIndex+1]-'0'));
            ch *= 16;
            ch += (pInput[dIndex+2] >= 'A' ? \
                    ((pInput[dIndex+2] & 0xdf)-'A')+10 : (pInput[dIndex+2]-'0'));
            pOutput[dJindex] = ch;
            dIndex+=2;
        }
		dJindex++;
	}
	pOutput[dJindex] = '\0';

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_fn_uaConvertHexEncodedString");

	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 	sdf_fn_uaCompareUserAndHost
 **
 ** DESCRIPTION: 	This function takes 2 addr spec structures and compares 
 **					the user and host part of their URLs
 **
 ** RETURN VALUES:	0 - The host name and user name match
 **					1 - The host name and user name don't match.
 **
 *****************************************************************************/
Sdf_ty_s8bit	sdf_fn_uaCompareUserAndHost
#ifdef ANSI_PROTO
	(SipAddrSpec *pAddrSpec1, SipAddrSpec* pAddrSpec2, Sdf_st_error *pError)
#else
	(pAddrSpec1, pAddrSpec2, pError)
	SipAddrSpec 	*pAddrSpec1;
	SipAddrSpec 	*pAddrSpec2;
	Sdf_st_error 	*pError;
#endif
{
	SipUrl *pUrl1 = Sdf_co_null, *pUrl2 = Sdf_co_null;
	Sdf_ty_s8bit dHost1[Sdf_co_smallBufferSize], dHost2[Sdf_co_smallBufferSize];
	Sdf_ty_s8bit dUser1[Sdf_co_smallBufferSize], dUser2[Sdf_co_smallBufferSize];

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_fn_uaCompareUserAndHost");
		
	if ((pAddrSpec1 == Sdf_co_null) || (pAddrSpec2 == Sdf_co_null) \
		|| (pError == Sdf_co_null))
		return 1;

	/* Compare address fields now */
	/*w534*/ (void)sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec1, &pUrl1, pError);
	/*w534*/ (void)sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec2, &pUrl2, pError);

	/* Host field comparison - case insensitive */
    /*CSR 1-7574892 change start*/
    if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pUrl1->pHost, dHost1,Sdf_co_smallBufferSize))
    {
        sip_freeSipUrl(pUrl1);
        sip_freeSipUrl(pUrl2);
        return 1;
    }
    if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pUrl2->pHost, dHost2,Sdf_co_smallBufferSize))
    {
        sip_freeSipUrl(pUrl1);
        sip_freeSipUrl(pUrl2);
        return 1;

    }
    /*CSR 1-7574892 change end*/


	if (Sdf_mc_strcasecmp(dHost1, dHost2) != 0)
	{
		sip_freeSipUrl(pUrl1);
		sip_freeSipUrl(pUrl2);
		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_fn_uaCompareUserAndHost");
		return 1;
	}

	/* User field comparison - case sensitive */
     /*CSR 1-7574892 change start*/
    if (Sdf_co_fail==sdf_fn_uaConvertHexEncodedString(pUrl1->pUser, dUser1,Sdf_co_smallBufferSize))
    {
        sip_freeSipUrl(pUrl1);
        sip_freeSipUrl(pUrl2);

        return 1;

    }
    if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pUrl2->pUser, dUser2,Sdf_co_smallBufferSize))
    {
        sip_freeSipUrl(pUrl1);
        sip_freeSipUrl(pUrl2);
        return 1;

    }
    /*CSR 1-7574892 change end*/


	if ( (dUser1[0] != '\0') && (dUser2[0] != '\0') )
	{
		/* User filed exists in both URLs, compare them */
		if(Sdf_mc_strcmp(dUser1, dUser2) != 0)
		{
			/* 
			 * User field's don't match exactly. Check for
			 * overlap dialing
			 */
			if ( (Sdf_mc_strstr(dUser1, dUser2) != Sdf_co_null) \
				|| (Sdf_mc_strstr(dUser1, dUser2) != Sdf_co_null) )
			{
				/* Free the local variables */
				sip_freeSipUrl(pUrl1);
				sip_freeSipUrl(pUrl2);
				return 2;
			}
			/* Free the local variables */
			sip_freeSipUrl(pUrl1);
			sip_freeSipUrl(pUrl2);
			sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
				"Exiting sdf_fn_uaCompareUserAndHost");
			return 1;
		}
	}
	else if( (pUrl1->pUser == Sdf_co_null) && \
		(pUrl2->pUser == Sdf_co_null) )
	{
		/* No user field in either. Continue. */
		;
	}
	else
	{
		/* One of the URLs has a username while the other doesn't.
		Not a match. */
		sip_freeSipUrl(pUrl1);
		sip_freeSipUrl(pUrl2);
		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_fn_uaCompareUserAndHost");
		return 1;
	}
	sip_freeSipUrl(pUrl1);
	sip_freeSipUrl(pUrl2);
	
	/* 
	 * return success.
	 */
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"sdf_fn_uaCompareUserAndHost");
	return 0;
}




/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaIssueTransactionCallback
 **
 ** DESCRIPTION: This function issues the sdf_cbk_uaTransactionReceived
 **				 callback to the application. Used to remove code
 ** 			 duplication.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaIssueTransactionCallback
#ifdef ANSI_PROTO
(Sdf_st_callObject **ppCallObj,
 Sdf_st_eventContext *pEventContext,
 Sdf_st_overlapTransInfo *pOverlapTransInfo,
 Sdf_st_error *pErr)
#else
 (pCallObj, pEventContext, pOverlapTransInfo, pErr)
 Sdf_st_callObject **ppCallObj;
 Sdf_st_eventContext *pEventContext;
 Sdf_st_overlapTransInfo *pOverlapTransInfo;
 Sdf_st_error *pErr;
#endif
{ 
	Sdf_ty_retVal dCbkRetVal = Sdf_co_success;
#ifdef SDF_SUBSCRIPTION_LAYER
	Sdf_ty_bool    dSubsFlag       = Sdf_co_false;
  Sdf_ty_bool    dIsErrorHandled = Sdf_co_false;
  Sdf_st_msgInfo dMessageInfo    = {Sdf_co_null, SipMessageAny, 
                                    Sdf_en_RegularTxn, 0, 0};
#endif

#ifdef SDF_TRACE
	Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
#endif
	Sdf_ty_slist		*pslMessageBody = Sdf_co_null;
	SipMessage			*pSipMessage = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_fn_uaIssueTransactionCallback");
	
#ifdef SDF_THREAD_SAFE
	/* Lock the dCallStateMutex  before changing the call state */
	sdf_fn_uaLockMutex(0, &((*ppCallObj)->dCallStateMutex), 0);
#endif
	/*
	 * Change the call state of the call object
	 */
	if( pOverlapTransInfo == Sdf_co_null )
		pSipMessage = (*ppCallObj)->pUasTransaction->pSipMsg;
	else
		pSipMessage = pOverlapTransInfo->pSipMsg;
	
	if (sdf_ivk_uaChangeCallState((*ppCallObj), pOverlapTransInfo, \
		pSipMessage, Sdf_en_msgModeRecd, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callStateError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIssueTransactionCallback(): "
			"Changing call state failed",pErr);
#endif
		pErr->errCode=Sdf_en_callStateError;
#ifdef SDF_THREAD_SAFE
		/* Unlock the dCallStateMutex */
		sdf_fn_uaUnlockMutex(0, &((*ppCallObj)->dCallStateMutex));
#endif
		return Sdf_co_fail;
	}

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &((*ppCallObj)->dCallStateMutex));
#endif

	/* 
	 * Store all the message bodies present in the message into the
	 * slMsgBodyList of the call object
	 */
	if( pOverlapTransInfo != Sdf_co_null )
		pslMessageBody = &(pOverlapTransInfo->slMsgBodyList);
	else
		pslMessageBody = &((*ppCallObj)->pUasTransaction->slMsgBodyList);

	if (sdf_fn_uaStoreAllMsgBodies(pSipMessage, pslMessageBody, pErr) == \
			Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIssueTransactionCallback(): "
			"Storing all message bodies in call object failed",pErr);
#endif
		/* Restore the state */
		(*ppCallObj)->pUasTransaction->dTransactionState = Sdf_en_idle;
		return Sdf_co_fail;
	}

#ifdef SDF_TRACE
	(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,"Call : %s -  "
		"Transaction received; "
		"Issuing sdf_cbk_uaTransactionReceived to application",\
		(*ppCallObj)->pCommonInfo->pKey->pCallId);
	sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_cbksTraces,Trace,0,pErr);
#endif

#ifdef SDF_SUBSCRIPTION_LAYER
  /* SubscriptionLayer handler MUST be called BEFORE issuing the 
	 * TransactionReceived callback because we need to process all non-REFER 
	 * SUBSCRIBE and NOTIFY requests & responses in sdf_fn_uaSLHandler. If 
	 * the message is not handled by SubsLayer then it will be handled by
	 * TransactionReceived callback. First check if is a non-REFER SUBSCRIBE/
	 * NOTIFY request.
	 */
  if (Sdf_co_success != sdf_fn_uaGetMessageInfo(pSipMessage, &dMessageInfo, \
                                                pErr))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_cbk_uaTransactionReceived(): "
			"Getting Message Info from SipMessage failed",pErr);
#endif
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
        /*
         * Callobject state is passed as paramter to ismessageforsubslayer
         * fn. 
         */
  if (sdf_fn_uaSLIsMessageForSubsLayer(&dMessageInfo, pOverlapTransInfo,
                                           &dSubsFlag, 
                                           (*ppCallObj)->pCallInfo->dState,
                                            pErr) != Sdf_co_success)
  {
    /* If a mandatory header in SUBSCRIBE/NOTIFY is missing, thrown 
  	 * UnexpectedEvent callback so that Application can handle it 
		 * appropriately.
     */
    dIsErrorHandled = Sdf_co_true;
    dCbkRetVal      = sdf_cbk_uaSLUnexpectedEvent (ppCallObj, Sdf_co_null, 
                                 pOverlapTransInfo, pEventContext, 
                                 Sdf_en_uaSLErrorInMessage, pErr);
  }

  if (dSubsFlag == Sdf_co_true)
	  dCbkRetVal = sdf_fn_uaSLHandler (ppCallObj, pOverlapTransInfo, 
	                            		 pEventContext, pErr);
	/* Above handler will set the dSubsFlag to true if it is handling the 
	 * message*/
	if ((dSubsFlag == Sdf_co_true) || (dIsErrorHandled == Sdf_co_true))
	{
#ifdef SDF_ERROR
    if (dCbkRetVal != Sdf_co_success)
      sdf_fn_dumpError((Sdf_ty_s8bit *)\
  		"sdf_fn_uaIssueTransactionCallback(): " "sdf_fn_uaSLHandler failed");
#endif
	  sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
	    	"Exiting sdf_fn_uaIssueTransactionCallback");
		return dCbkRetVal;
  }
#endif

	/* 
	 * Issue the Transaction Received callback to the application
	 */
	dCbkRetVal = sdf_cbk_uaTransactionReceived(ppCallObj, \
		pEventContext, pOverlapTransInfo, pErr);

	if (dCbkRetVal == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callBackError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIssueTransactionCallback(): "
			"Callback sdf_cbk_uaTransactionReceived failed",pErr);
#endif
		pErr->errCode=Sdf_en_callBackError;
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_fn_uaIssueTransactionCallback");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFormToAndFromHeaders
 **
 ** DESCRIPTION: This function is used to make the From and To headers and 
 **				 add the same to the SipMessage passed as input. Internally
 **				 the function adds a From tag to the callobject if it is 
 **				 the first request being made in this call-leg (i.e. we are 
 **				 originating a new request) or adds a To tag to the call 
 **				 object if it is the first response of a call leg that is 
 **				 being accepted.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormToAndFromHeaders
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, SipMessage *pMessage, Sdf_st_error *pError)
#else
	(pObject, pMessage, pError)
	Sdf_st_callObject 	*pObject;
	SipMessage 			*pMessage;
	Sdf_st_error		*pError;
#endif
{
	SipHeader dCallObjectFromHeader, dCallObjectToHeader;
	Sdf_ty_retVal dCaller;
	SipHeader       *pToHdr = Sdf_co_null;
	Sdf_st_error	dLocalErr;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_fn_uaFormToAndFromHeaders");


#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pError == Sdf_co_null)
		return Sdf_co_fail;

	if(pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
			"Invalid param: Sip Message",pError);
#endif			
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
			"Invalid param: Call Object",pError);
#endif			
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	dCaller = sdf_fn_uaVerifyCaller(pObject, pError);
	
	if (((dCaller == Sdf_co_success)&&(pMessage->dType == SipMessageRequest))|| 
		((dCaller != Sdf_co_success)&&(pMessage->dType == SipMessageResponse)))
	{
		/* ---------------------------
		 * Don't change From and To if:
		 * ---------------------------
		 * 1. We were the caller and are making a new request. From will be 
		 *    me and that's the way it should be for the new request.
		 *
		 * 2. We were the callee and are answering a request. To header will be
		 *    me and that's also right. 
		 */
		dCallObjectFromHeader.dType = SipHdrTypeFrom;
		dCallObjectFromHeader.pHeader = pObject->pCommonInfo->pFrom->pHeader;
		dCallObjectToHeader.dType = SipHdrTypeTo;
		dCallObjectToHeader.pHeader = pObject->pCommonInfo->pTo->pHeader;
	}
	else
	{
		/* ---------------------
		 * Swap From and To if:
		 * ---------------------
		 * 1. We were the caller and are responding to a request from the
		 *    other end. To should be us in this case.
		 * 2. We were not the caller and are making a new request. From
		 *    should be us in this case.
		 */
		dCallObjectFromHeader.dType = SipHdrTypeFrom;
		dCallObjectFromHeader.pHeader = pObject->pCommonInfo->pTo->pHeader;
		dCallObjectToHeader.dType = SipHdrTypeTo;
		dCallObjectToHeader.pHeader = pObject->pCommonInfo->pFrom->pHeader;
	}

	/* 
	 * If a request is being formed, add a tag (if absent) in the From header 
	 */
	if ((pMessage->dType == SipMessageRequest) && (dCaller == Sdf_co_success))
	{
		Sdf_ty_s8bit *pTag = Sdf_co_null;

		/*w534*/ (void)sip_getTagAtIndexFromFromHdr(&dCallObjectFromHeader, &pTag, \
			0, (SipError*)&(pError->stkErrCode));

		if (pTag == Sdf_co_null)
		{
			Sdf_st_initData *pInitData;
			Sdf_ty_s8bit	*pVarFromTag = Sdf_co_null;

			if (sdf_ivk_uaGetInitDataFromCallObject(pObject, &pInitData, \
				pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_callObjectAccessError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
					"Failed to get user profile from call object",pError);
#endif					
				pError->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
 /*spr-4627 :#ifndef SDF_UAB2B_2543 flag is added to give backup
             * compatibility for 2543 to B2B. In 2543 there is no concept of
             * TOtags and From tag */

#ifndef SDF_UAB2B_2543

			if (sdf_fn_uaGenerateGlobalTag( pInitData->pTag, &pVarFromTag) \
					== Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_tagGenerationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
					"Failed to generate variable portion of the tag",pError);
#endif					
				pError->errCode=Sdf_en_tagGenerationError;
				(void)sdf_ivk_uaFreeInitData(pInitData);
				return Sdf_co_fail;
			}

			if (sip_insertTagAtIndexInFromHdr(&dCallObjectFromHeader, \
				pVarFromTag, 0, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
					"Failed to insert Tag in From header",pError);
#endif
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)(&pVarFromTag), pError);
				(void)sdf_ivk_uaFreeInitData(pInitData);
				return Sdf_co_fail;
			}
#endif     /*spr-4627 end giving backup compatibility for rfc 2543 to B2B*/ 

			/* Free the local reference */
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)(&pVarFromTag), pError);
			(void)sdf_ivk_uaFreeInitData(pInitData);
		}
	}

    /* Fix for SPR: 19999(Merged CSR-1-7429815)*/
    /* CSR_1-7430668 Fix Start */

    /* The table present in Section 19.1.2 of RFC 3261 mentions 
     * that port number MUST NOT be included in From and To headers
     *
     * For better interop,
     *
     * (1) Port numbers can be permitted in From / To headers of 
     * incoming requests and responses i.e UATK will not throw an error
     * if any incoming request contained port in FROM or To URI.
     *
     * (2) For outgoing responses, From / To headers are formed from the
     * request being responded to. So if the request had a port in From
     * / To URI, then the response to that request will contain the it.
     *
     * (3) For outgoing requests, dCaller is true then port in To URI
     * will be removed by the below piece of code.
     *
     * (By mentioning the port number as zero at the time of invocation
     * of sdf_ivk_uaSetDefaultFrom, it can be ensured that port number 
     * is not present in From URI)
     */
    if ((pMessage->dType == SipMessageRequest) && (Sdf_en_caller == pObject->pCommonInfo->pKey->dCaller))
    {
        if((Sdf_co_null != dCallObjectToHeader.pHeader) &&\
                (Sdf_co_null != ((SipToHeader *)(dCallObjectToHeader.pHeader))->\
                 pAddrSpec) &&  (Sdf_co_null != ((SipToHeader *)\
                         (dCallObjectToHeader.pHeader))->pAddrSpec->u.pSipUrl))
        {
            /* CSR_1-7588994 Fix Start */
            if ((pMessage->u.pRequest->pRequestLine->pAddrSpec->dType == SipAddrSipUri) || 
                    (pMessage->u.pRequest->pRequestLine->pAddrSpec->dType == SipAddrSipSUri))
            {
                if(Sdf_co_null != pMessage->u.pRequest->pRequestLine->pAddrSpec->u.pSipUrl->dPort)
                    pObject->pCommonInfo->dReqUriPort = 
                        *(pMessage->u.pRequest->pRequestLine->pAddrSpec->u.pSipUrl->dPort);

                /* CSR_1-7588994 Fix End */
                if ((((SipToHeader *)(dCallObjectToHeader.pHeader))->pAddrSpec->dType == SipAddrSipUri)
                        || (((SipCommonHeader *)(dCallObjectToHeader.pHeader))->\
                            pAddrSpec->dType == SipAddrSipSUri))
                {
                    sdf_memfree(Sdf_mc_callHandlerMemId,((Sdf_ty_pvoid *)&\
                                (((SipToHeader *)(dCallObjectToHeader.pHeader))->\
                                 pAddrSpec->u.pSipUrl->dPort)),pError);
                    ((SipToHeader *)(dCallObjectToHeader.pHeader))->pAddrSpec->\
                        u.pSipUrl->dPort = Sdf_co_null;
                }
            }
        }
    }
    /* CSR_1-7430668 Fix End */

    /* CANCEL_TO_TAG_REMOVAL fix begins */ 
    /* Code added to suppress To Tag. This is because CANCEL should have 
	* the same To as the Request that is getting cancelled. If re-INVITe 
	* is being cancelled, the tag should be there. If the initial INVITE 
	* is being cancelled, then this tag should be removed. */ 
    /* Check the state. If the state is "InviteSent". remove the tag. 
     * Else, don't remove. */ 
	if ( (pObject->pCallInfo->dState == Sdf_en_inviteSent)  ||
		 (pObject->pCallInfo->dState == Sdf_en_provisionalRespReceived)) 
	{ 
	   Sdf_ty_u32bit dCount = 0; 
	   Sdf_ty_s8bit *pMethod = Sdf_co_null; 
	   Sdf_ty_s8bit *pTmpTag = Sdf_co_null; 
	   SipReqLine      *pReqLine = Sdf_co_null; 

	   /*w534*/ (void)sip_getReqLineFromSipReqMsg (pMessage, &pReqLine,\
					   (SipError *)&(pError->stkErrCode)); 
	   /*w534*/ (void)sip_getMethodFromReqLine (pReqLine, &pMethod,   \
					   (SipError *)&(pError->stkErrCode)); 

	   sip_freeSipReqLine (pReqLine); 

	   /*w534*/ (void)sip_getTagAtIndexFromToHdr(&dCallObjectToHeader, &pTmpTag, \
			   0, (SipError*)&(pError->stkErrCode)); 

	   if ((pTmpTag != Sdf_co_null) && \
			   (Sdf_mc_strcmp(pMethod, "CANCEL") == 0)) 
		{ 
			if (sip_initSipHeader (&pToHdr, SipHdrTypeTo, \
				   (SipError *)&(pError->stkErrCode)) == SipFail) 
			{ 
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR 
			   sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				   (Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): " 
				   "Failed to init Sip header",pError);
#endif 
			   return Sdf_co_fail;
			}

			if (sip_cloneSipHeader (pToHdr, &dCallObjectToHeader, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR 
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
					 "Failed to clone Sip header",pError);
#endif
				sip_freeSipHeader (pToHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				   (Sdf_ty_pvoid *)&pToHdr, &dLocalErr);
				return Sdf_co_fail;
			}

			if(sip_getTagCountFromToHdr(pToHdr,&dCount,\
				(SipError *)&(pError->stkErrCode))==SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR 
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
					"Failed to get Tag dCount from To header",pError);
#endif 
				sip_freeSipHeader (pToHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid *)&pToHdr, &dLocalErr);
				 return Sdf_co_fail;
			}

			if (dCount != 0)
			{
				Sdf_ty_u32bit dIndex;
				for(dIndex=0;dIndex<dCount;dIndex++)
				{
					if(sip_deleteTagAtIndexInToHdr(pToHdr,dIndex,\
						(SipError *)&(pError->stkErrCode))==SipFail)
					{
						pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR 
						sdf_fn_setError(Sdf_en_minorErrors,\
							pError->errCode,(Sdf_ty_s8bit *)\
							"sdf_fn_uaFormToAndFromHeaders( ):"
							" Failed to delete Tag from"
							" To header",pError);
#endif 
						sip_freeSipHeader (pToHdr);
						(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid *)&pToHdr, &dLocalErr);
						return Sdf_co_fail;
					}
				}
			}
		   dCallObjectToHeader.pHeader = pToHdr->pHeader;
		}
	}
   /* CANCEL_TO_TAG_REMOVAL fix ends */ 


	/*
	 * Set the From header into the SipMessage being formed.
	 */
	if (sip_setHeader(pMessage, &dCallObjectFromHeader, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
			"Failed to set From header in Sip Message",pError);
#endif			
		return Sdf_co_fail;
	}	

	/* 
	 * If a response is being formed, add a tag (if absent) in the
	 * To header.
	 */
	if ((pMessage->dType == SipMessageResponse) && (dCaller == Sdf_co_fail))
	{
		Sdf_ty_s8bit *pTag = Sdf_co_null;

		/*w534*/ (void)sip_getTagAtIndexFromToHdr(&dCallObjectToHeader, &pTag, \
			0, (SipError*)&(pError->stkErrCode));

		if (pTag == Sdf_co_null)
		{
			Sdf_st_initData *pInitData;
			Sdf_ty_s8bit	*pVarToTag = Sdf_co_null;

			if (sdf_ivk_uaGetInitDataFromCallObject(pObject, &pInitData, \
				pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_callObjectAccessError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
					"Failed to get user profile from call object",pError);
#endif					
				pError->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
/*SPR-4627 : #ifndef SDF_UAB2B_2543 flag is added to give backup
             * compatibility for 2543 to B2B. In 2543 there is no concept of
             * TOtags and From tag */
#ifndef SDF_UAB2B_2543
			if (sdf_fn_uaGenerateGlobalTag( pInitData->pTag, &pVarToTag) \
					== Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_tagGenerationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToHeader( ): "
					"Failed to generate variable portion of the tag",pError);
#endif					
				pError->errCode=Sdf_en_tagGenerationError;
				(void)sdf_ivk_uaFreeInitData(pInitData);
				return Sdf_co_fail;
			}
			if (sip_insertTagAtIndexInToHdr(&dCallObjectToHeader, \
				pVarToTag, 0, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
					"Failed to insert Tag in To header",pError);
#endif
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)(&pVarToTag), pError);
				(void)sdf_ivk_uaFreeInitData(pInitData);
				return Sdf_co_fail;
			}
#endif /*spr-4627 end giving backup compatibility for rfc 2543 to B2B*/
      
			/* Free the local reference */
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)(&pVarToTag), pError);
			(void)sdf_ivk_uaFreeInitData(pInitData);
		}	
	}
	
	/*
	 * Set the To header in the SIP message structure.
	 */
	if (sip_setHeader(pMessage, &dCallObjectToHeader, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormToAndFromHeaders( ): "
			"Failed to set To header in Sip Message", pError);
#endif			
		return Sdf_co_fail;
	}

    if (pToHdr != Sdf_co_null)
    {
        sip_freeSipHeader (pToHdr);
        (void)sdf_memfree(Sdf_mc_callHandlerMemId, \
           (Sdf_ty_pvoid *)&pToHdr, pError);
     }

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaFormToAndFromHeaders");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaCheckMessageValidity
**
** DESCRIPTION: Checks if the message received will cause a valid state
**	transition for the call from it's present state. Also does certain
**	validations (like incrementing remote Cseq) and responds with SIP 
**	error responses if the message is invalid.
**
******************************************************************************/

Sdf_ty_messageValidity sdf_fn_uaCheckMessageValidity
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pOriginalCallObj,
	 Sdf_st_callObject	*pNewCallObj,
	 Sdf_st_msgInfo		dMessageInfo,
	 SipMessage			*pIncomingMsg,
	 Sdf_ty_transactionType	dTxnType,
	 Sdf_st_eventContext	*pEventContext,
	 Sdf_st_error 		*pErr)
#else
	(pOriginalCallObj, pNewCallObj, dMessageInfo, pIncomingMsg, 
	 dTxnType, pEventContext, pErr)
	 Sdf_st_callObject	*pOriginalCallObj;
	 Sdf_st_callObject	*pNewCallObj;
	 Sdf_st_msgInfo		dMessageInfo,
	 SipMessage			*pIncomingMsg;
	 Sdf_ty_transactionType	dTxnType;
	 Sdf_st_eventContext	*pEventContext;
	 Sdf_st_error 		*pErr;
#endif
{
	en_SipMessageType dType;
	Sdf_ty_messageType dMsgType=Sdf_en_unknownRequest;
	Sdf_st_overlapTransInfo *pOverlapTransInfo = Sdf_co_null;
	SipMessage				*pTempMessage = Sdf_co_null;
	Sdf_ty_bool 	dLoopDetected = Sdf_co_false;
	Sdf_ty_retVal	retVal;
	Sdf_ty_bool		disInvalid = Sdf_co_false;
	Sdf_ty_u16bit	dRespCode = 0;
#ifndef SDF_LINT
    Sdf_st_socket	*pSockfd = Sdf_co_null;
	Sdf_ty_s8bit *pMethod = Sdf_co_null;    
#endif    
	Sdf_ty_bool		dIsCseqSame = Sdf_co_false;
	Sdf_ty_messageValidity dValidity;
    SipOptions 			dOptions;

	(void)pEventContext;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaCheckMessageValidity ");

	dType = dMessageInfo.dReqRespType;
#ifndef SDF_LINT
    pMethod = dMessageInfo.pMethod;
#endif    
	dMsgType = dMessageInfo.dMsgType;


	switch (dType)
	{
		case SipMessageRequest:
		{
			Sdf_ty_s32bit dDisplayCallFlow = 0;

			dRespCode = 400;
			/* Cancel to Non-Invites are rejected with 405 response.
			 * The Cseq number present in dRemoteCseq field of the 
			 * common info always indicates the sequence number received
			 * in latest INVITE request.  If cancel Cseq number doesn't
			 * match with that of the dRemoteCseq then the cancel is meant
			 * for cancelling Non-Invite request. So we can reject it */

			if (dMsgType == Sdf_en_cancel) 
			{
				SipHeader dHeader;
				Sdf_ty_u32bit dCseq=0;

				if (sip_getHeader(pIncomingMsg, SipHdrTypeCseq, &dHeader, \
					(SipError *)&(pErr->stkErrCode)) == SipFail)
				{
					pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
						(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity(): "
						"Failed to get Cseq header from incoming "
						"message.", pErr);
#endif
					return Sdf_en_failed;
				}
				if (sip_getSeqNumFromCseqHdr(&dHeader, &dCseq, \
					(SipError *)&(pErr->stkErrCode)) == SipFail)
				{
					pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
						(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity(): "
						"Failed to get seq number from Cseq header.", pErr);
#endif
					sip_freeSipHeader(&dHeader);
					return Sdf_en_failed;
				}
				sip_freeSipHeader(&dHeader);

				if( dCseq != pOriginalCallObj->pCommonInfo->dRemoteCseq )
				{
					/* Send pOriginalCallObj->pInitData instead of
					 * pNewCallObj->pInitData to know which other
					 * method supported by the UATK
					 */
					if (sdf_ivk_uaRejectRequest(pNewCallObj, \
						pOriginalCallObj->pInitData, 405, \
						&pOverlapTransInfo, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
							(Sdf_ty_s8bit*)"sdf_fn_uaCheckMessageValidity()"
							": Failed to form 405 response for CANCEL"
							" to a Non-Invite.",pErr);
#endif	
						return Sdf_en_failed;
					}
			
					if( pOverlapTransInfo != Sdf_co_null )
						pTempMessage = pOverlapTransInfo->pSipMsg;
					else
						pTempMessage = pNewCallObj->pUasTransaction->pSipMsg;

					pErr->errCode = Sdf_en_InvalidCseqNumber;
					sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
						"Exiting sdf_fn_uaCheckMessageValidity ");
					return Sdf_en_failed;
				}
			}

#ifdef SIP_RPR
			if (dMsgType == Sdf_en_prack)
			{
				/*
				 * To match a PRACK to a RPR the following are necessary:
				 * -----------------------------------------------------
				 *	 a. The Call-ID in PRACK matches the Call-ID in RPR
				 *	 b. The From in PRACK matches the From in RPR, including 
				 *	 	the tag
				 *	 c. The To in PRACK matches the To in RPR, including 
				 *	 	the tag
				 *	 d. The method in the RAck of PRACK matches the method 
				 *	 	in the CSeq of RPR
				 *	 e. The CSeq-num in the RAck matches the CSeq number 
				 *	 	in RPR
				 *	 f. The response-num in the RAck matches the RSeq value 
				 *	 in RPR
				 *
				 * Matching
				 * --------
				 * 	 a, b and c are already ensured before this point
				 *	 f is ensured in 
				 *	 sdf_ivk_uaRetrieveMatchingOverlapTransInfo()
				 *	 d and e are specific only to PRACK and are handled in
				 *	 sdf_fn_uaIsValidCseq
				 */

				/*
				 * Get the overlapTransInfo that matches this message.
				 */
				retVal = sdf_ivk_uaRetrieveMatchingOverlapTransInfo(\
					pIncomingMsg, pOriginalCallObj, &pOverlapTransInfo, \
					Sdf_en_msgModeRecd, pErr);

				if( retVal == Sdf_co_fail )	
				{
					if (pErr->errCode != Sdf_en_noExistError)
					{
#ifdef SDF_ERROR				
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_overlapTransInfoAccessError, \
							(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity( ): "
							"Failed to get matching OverlapTransInfo",pErr);	
#endif				
						sdf_fn_uaUpdateCallFlow (pIncomingMsg, Sdf_co_true);

						return Sdf_en_failed;
					}
					else
					{
#ifdef SDF_ERROR				
						sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
							(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity( ): "
							"Invalid PRACK request recd",pErr);	
#endif				
						sdf_fn_uaUpdateCallFlow (pIncomingMsg, Sdf_co_true);

						/* Received Stray PRACK.
						 * Form and send a 481 response message */

						if (sdf_ivk_uaRejectRequest(pNewCallObj, \
							pOriginalCallObj->pInitData, 481, \
							&pOverlapTransInfo, pErr) == Sdf_co_fail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
								(Sdf_ty_s8bit*)"sdf_fn_uaCheckMessageValidity()"
								": Failed to form 4xx response",pErr);
#endif
							return Sdf_en_failed;
						}
				

						pTempMessage = pOverlapTransInfo->pSipMsg;

	
						sdf_fn_debug((Sdf_ty_s8bit*) \
								"SDF_DEBUG - Exiting "\
                                "sdf_fn_uaCheckMessageValidity");
						pErr->errCode = Sdf_en_localTagMismatchError;	
						return Sdf_en_failed;
					}
				}
				if (pOverlapTransInfo == Sdf_co_null)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_overlapTransInfoAccessError, \
						(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity( ): "
						"Overlap transaction not found",pErr);	
#endif			
					return Sdf_en_failed;
				}
			}
#endif			

			/* 
			 * If a BYE is received in the byeReceived state, it is a
			 * retransmission. So it can be dropped
	 		*/
			if ((pOriginalCallObj->pCallInfo->dState == Sdf_en_byeReceived) && \
				(dMsgType == Sdf_en_bye) )
			{
				sdf_fn_debug((Sdf_ty_s8bit*) \
						"SDF_DEBUG - Exiting sdf_fn_uaCheckMessageValidity");	
				pErr->errCode = Sdf_en_noUaError;
				(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
				return Sdf_en_discarded;
			}

			/* For Regular Transactions, check whether this transaction is
			 * already received.*/
			if ((dMsgType == Sdf_en_info) || \
				(dMsgType == Sdf_en_register) || \
				(dMsgType == Sdf_en_options) ||
#ifdef SIP_IMPP
				(dMsgType == Sdf_en_subscribe) || \
				(dMsgType == Sdf_en_notify) ||
#endif
				(dMsgType == Sdf_en_update) || \
				(dMsgType == Sdf_en_unknownRequest))
			{
				Sdf_st_transaction	*pTransaction = Sdf_co_null;
				Sdf_st_overlapTransInfo	*pOverTxn = Sdf_co_null;

				if(sdf_ivk_uaGetTransactionForSipMsg(pOriginalCallObj, \
					pIncomingMsg, &pTransaction,  &pOverTxn, \
					pErr) == Sdf_co_success)
				{
					if(pOverTxn != Sdf_co_null)
					{
						if(pOverTxn->dState == Sdf_en_transactionReceived)
						{
							/*
							 * The message can be discarded. This is probably 
							 * because a callback has been issued to the user
							 * for the request, and a retransmission of the 
							 * same has been received.
							 * */
							sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
								"Exiting sdf_fn_uaCheckMessageValidity");

							/* 
							 * Display the incoming message on ctascii 
							 */
							if (dDisplayCallFlow == 1)
								sdf_fn_uaUpdateCallFlow (pIncomingMsg, \
										Sdf_co_true);

							pErr->errCode = Sdf_en_noUaError;
							(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
							(void)sdf_ivk_uaFreeOverlapTransInfo(pOverTxn);

							return Sdf_en_discarded;
						}
						(void)sdf_ivk_uaFreeOverlapTransInfo(pOverTxn);
					}
				}
			}
			/* 
			 * Compare the current Cseq number with the last Cseq received 
			 * from the remote end.
			 */
			dValidity = sdf_fn_uaIsValidCseq(pOriginalCallObj, \
				pIncomingMsg, &dDisplayCallFlow, dMsgType, &dRespCode, \
			   	&dIsCseqSame, pErr);

			switch (dValidity)
			{
				case Sdf_en_discarded :
				{
					/*
					 * The message can be discarded. This is probably because a
					 * callback has been issued to the user for the request,
					 * and a retransmission of the same has been received.
					 */
					sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
						"Exiting sdf_fn_uaCheckMessageValidity");

					/* 
					 * Display the incoming message on ctascii 
					 */
					if (dDisplayCallFlow == 1)
						sdf_fn_uaUpdateCallFlow (pIncomingMsg, Sdf_co_true);

					pErr->errCode = Sdf_en_noUaError;
					(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);

					return Sdf_en_discarded;
				}

				case Sdf_en_valid :	
				{
					/*
					 * The cseq validity check succeeded. Continue executing 
					 * this function 
					 */
					sdf_fn_uaUpdateCallFlow (pIncomingMsg, Sdf_co_true);
					break;
				}

				case Sdf_en_invalid :
				{
					/*
					 * The request is invalid in the current state OR the Cseq
					 * has not incremented.
					 */
					sdf_fn_uaUpdateCallFlow (pIncomingMsg, Sdf_co_true);
					(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);

					
					/*
					 * Check if Sdf_en_invalid was returned for an ACK.
					 * ACK's should not be responded to as a rule.
					 */

					if (dMsgType == Sdf_en_ack)
					{
						sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
							"Exiting sdf_fn_uaCheckMessageValidity");
						pErr->errCode = Sdf_en_noUaError;
						return Sdf_en_invalid;
					}

					if(dIsCseqSame == Sdf_co_true )
					{
						Sdf_ty_bool IsToTagPresent = Sdf_co_false;
						(void)sdf_fn_uaUpdateToTag(pNewCallObj,pIncomingMsg,&IsToTagPresent,pErr);	
									
						if(IsToTagPresent == Sdf_co_false)
							dLoopDetected = sdf_fn_uaCheckRequestLoop(pIncomingMsg,\
										pOriginalCallObj, pNewCallObj, dMessageInfo, pErr);

						if(dLoopDetected == Sdf_co_true)
							dRespCode = 482;
					}

					retVal = sdf_ivk_uaRejectRequest(pNewCallObj, \
							pOriginalCallObj->pInitData, dRespCode, \
							&pOverlapTransInfo, pErr);

					if( retVal == Sdf_co_fail )
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
							(Sdf_ty_s8bit*)"sdf_fn_uaCheckMessageValidity()"
							": Failed to form 4xx response",pErr);
#endif
						return Sdf_en_failed;
					}
#ifndef SDF_LINT		
					pSockfd = Sdf_co_null;
#endif
					if( pOverlapTransInfo != Sdf_co_null )
						pTempMessage = pOverlapTransInfo->pSipMsg;

					else
					{
						pTempMessage = pNewCallObj->pUasTransaction->pSipMsg;
#ifndef SDF_LINT                        
						pSockfd = \
							&(pNewCallObj->pUasTransaction->dSocket);
#endif                        
					}
					if( dRespCode == 500 )
					{
						SipStatusLine *pStatusLine=Sdf_co_null;
						Sdf_ty_s8bit  *pReason = Sdf_co_null;
                        SipHeader dRetry;
						
						/*w534*/ (void)sip_getStatusLineFromSipRespMsg(pTempMessage, \
							&pStatusLine, (SipError *)&(pErr->stkErrCode));

						pReason = Sdf_mc_strdupCallHandler("Server Internal Error");
                        if(SipFail != sip_getHeaderAtIndex(pTempMessage,\
								SipHdrTypeRetryAfterAny,&dRetry,0,\
										(SipError *)&(pErr->stkErrCode)))
						{
			
							(void)sip_deleteHeaderAtIndex(pTempMessage,\
										SipHdrTypeRetryAfterAny,0,\
										(SipError *)&(pErr->stkErrCode));
							sip_freeSipHeader(&dRetry);
						}
						/*w534*/ (void)sip_setReasonInStatusLine(pStatusLine, pReason, \
							(SipError *)&(pErr->stkErrCode));
						sip_freeSipStatusLine(pStatusLine);
					}

					sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
						"Exiting sdf_fn_uaCheckMessageValidity ");

					pErr->errCode = Sdf_en_serverInternalError;
					return Sdf_en_failed;
				}
				case Sdf_en_failed :
				{
					/*
					 * An internal API of the sdf_fn_uaIsValidCseq failed.
					 */
					sdf_fn_uaUpdateCallFlow (pIncomingMsg, Sdf_co_true);

					(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);

#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_noUaError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity(): "
						"Cseq Validity check failed",pErr);
#endif
					pErr->errCode=Sdf_en_noUaError;
					return Sdf_en_failed;
				}
				default :
						break;
			}
#ifdef SIP_RPR

			if (dMsgType != Sdf_en_prack)
#endif
				(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
#ifdef SIP_RPR

			if (dMsgType == Sdf_en_prack)
			{
				/* 
				 * For PRACKs': so far we have checked that the
				 * From, To, Call-ID, Cseq and Rseq of the PRACK
				 * to match some RPR sent out. State check to
				 * be done here.
				 */

				/*
				 * If state anything other than rprSent, a PRACK 
				 * reception is invalid
				 */
#ifdef SDF_LINT
				if (pOverlapTransInfo == Sdf_co_null)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_overlapTransInfoAccessError, \
						(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity( ): "
						"Overlap transaction not found",pErr);	
#endif			
					return Sdf_en_failed;
				}
#endif
                
				if (pOverlapTransInfo->dState != Sdf_en_rprSent)
				{
					sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
						"Exiting sdf_fn_uaCheckMessageValidity ");

					(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
					pErr->errCode = Sdf_en_noUaError;
					return Sdf_en_invalid;
				}
				(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
			}
			/* 
			 * All other methods should map into the block below.
			 * Since COMET should not use the state table and requires
			 * no special handling for Validity checks, the else
			 * block reads "if not COMET".
			 */
			else
#endif			
			if ((dTxnType == Sdf_en_uacTransaction) || \
					(dTxnType == Sdf_en_uasTransaction))
			{
				Sdf_st_callInfo *pOrigCallInfo;
				if (sdf_ivk_uaGetCallInfoFromCallObject(pOriginalCallObj, \
					&pOrigCallInfo, pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, \
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity(): "
						"Getting CallInfo from CallObject failed",pErr);
#endif
					pErr->errCode=Sdf_en_callObjectAccessError;
					return Sdf_en_failed;
				}

				/*
				 * If we get an INVITE in the Sdf_en_reInviteSent state, we
				 * should send a 491 Request Pending Error 
				 */
				if((dMsgType == Sdf_en_invite) &&
				  ((pOrigCallInfo->dState == Sdf_en_reInviteSent) ||
                   (pOriginalCallObj->pMediaInfo->dMediaState == Sdf_en_offerSent) ||
				   (pOriginalCallObj->pMediaInfo->dMediaState == Sdf_en_offerChangeSent)  ||

				   (pOrigCallInfo->dState == Sdf_en_reInviteprovRespReceived) ||
				   (pOriginalCallObj->pCallInfo->dInviteInProg == Sdf_co_true)||
				   (pOriginalCallObj->pMediaInfo->dOfferProgress==Sdf_co_true)))
				{
					disInvalid	= Sdf_co_true;
					dRespCode	= 491;
				}
				else if ( \
					dCsmNxtStateOnMsgRecd[dMsgType][pOrigCallInfo->dState].\
					dNextState == Sdf_en_invalidStateTransition)
				{
					disInvalid	= Sdf_co_true;
					dRespCode	= 481;
				}
				if( disInvalid == Sdf_co_true )
				{
					if( dMsgType != Sdf_en_ack )
					{
						if (sdf_ivk_uaRejectRequest(pNewCallObj, \
							pOriginalCallObj->pInitData, dRespCode, \
							&pOverlapTransInfo, pErr) == Sdf_co_fail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
								(Sdf_ty_s8bit*)"sdf_fn_uaCheckMessageValidity()"
								": Failed to form 491/500 response",pErr);
#endif
							return Sdf_en_failed;
						}
						pTempMessage = pNewCallObj->pUasTransaction->pSipMsg;
#ifndef SDF_LINT
                        pSockfd = &(pNewCallObj->pUasTransaction->dSocket);
#endif

		
					}
					(void)sdf_ivk_uaFreeCallInfo(pOrigCallInfo);
					sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
						"Exiting sdf_fn_uaCheckMessageValidity ");

					pErr->errCode = Sdf_en_requestPendingError;
					return Sdf_en_failed;
				}
				(void)sdf_ivk_uaFreeCallInfo(pOrigCallInfo);	
			}

			if (dMsgType == Sdf_en_update)
			{
/* CSR_1-6042790 Change Start */
/* #ifdef SDF_SESSION_TIMER
				
				SipHeader 		dTempHeader;
				
#endif*/
/* CSR_1-6042790 Change Start */
                
				if (sdf_fn_uaCallState(pOriginalCallObj) == \
						Sdf_en_invalidDialogState)
				{
						if (sdf_ivk_uaRejectRequest(pNewCallObj, \
							pOriginalCallObj->pInitData, 481, \
							&pOverlapTransInfo, pErr) == Sdf_co_fail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
								(Sdf_ty_s8bit*)"sdf_fn_uaCheckMessageValidity:"
								"Failed to form 481 response", pErr);
#endif
							return Sdf_en_failed;
						}

						dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
						pTempMessage = pOverlapTransInfo->pSipMsg;
#ifndef SDF_LINT
						pSockfd = &(pNewCallObj->pUasTransaction->dSocket);
#endif

                        /* Fix for SPR 20412: CSR 1-8223807 merged (UATK CSR 1-8290042)*/
						if(sdf_ivk_uaSendCallToPeer(pNewCallObj, pTempMessage, \
							dOptions, 0, pEventContext, Sdf_co_null, pErr) == Sdf_co_fail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
								(Sdf_ty_s8bit*)"sdf_fn_uaCheckMessageValidity"
								"(): Sending response to peer entity failed", \
								pErr);
#endif
							return Sdf_en_failed;
						}
						pErr->errCode = Sdf_en_callStateError;
						return Sdf_en_invalid;
				}

				
#ifndef SDF_UASSF
				if (sdf_ivk_uaIsSdpBodyPresent(pIncomingMsg, pErr) == \
						Sdf_co_success)
				{
					/* If we have received an UPDATE request when we have
					 * already sent an UPDATE, the received UPDATE should
					 * be rejected with 491 response */
					if ((pOriginalCallObj->pMediaInfo->dMediaState == \
							Sdf_en_offerSent) || \
						(pOriginalCallObj->pMediaInfo->dMediaState == \
						 	Sdf_en_offerChangeSent))
					{
							dRespCode = 491;
					}
					/* If we have received an UPDATE request when we are
					 * already processing another UPDATE, the 2nd UPDATE should
					 * be rejected with 500 response */
					else if ((pOriginalCallObj->pMediaInfo->dMediaState == \
							Sdf_en_offerRecv) || \
						(pOriginalCallObj->pMediaInfo->dMediaState == \
						 	Sdf_en_offerChangeRecv))
					{
							dRespCode = 500;
					}
					/* Check if the request has to be rejected */
					if ((dRespCode == 491) || (dRespCode == 500))
					{
						if (sdf_ivk_uaRejectRequest(pNewCallObj, \
							pOriginalCallObj->pInitData, dRespCode, \
							&pOverlapTransInfo, pErr) == Sdf_co_fail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
								(Sdf_ty_s8bit*)"sdf_fn_uaCheckMessageValidity:"
								"Failed to form 491/500 response", pErr);
#endif
							return Sdf_en_failed;
						}
						/* If response being sent is 500, then add a
						 * Retry-After header */
						if (dRespCode == 500)
						{
#if defined(MICROUA) && !defined(SIP_DATE)
							if (sip_setHeaderFromStringAtIndex(\
								pOverlapTransInfo->pSipMsg, \
								SipHdrTypeRetryAfter, \
								(Sdf_ty_s8bit *)"Retry-After: 10", 0, \
								(SipError *)&(pErr->stkErrCode)) == SipFail)
#else
							if (sip_setHeaderFromStringAtIndex(\
								pOverlapTransInfo->pSipMsg, \
								SipHdrTypeRetryAfterSec, \
								(Sdf_ty_s8bit *)"Retry-After: 10", 0, \
								(SipError *)&(pErr->stkErrCode)) == SipFail)
#endif
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									pErr->errCode, (Sdf_ty_s8bit*)\
									"sdf_fn_uaCheckMessageValidity:"
									"Failed to form 491/500 response", pErr);
#endif
							}
						}
						pTempMessage = pOverlapTransInfo->pSipMsg;
#ifndef SDF_LINT
                        pSockfd = &(pNewCallObj->pUasTransaction->dSocket);
#endif

	
						pErr->errCode = Sdf_en_regularTxnPendingError;;
						return Sdf_en_invalid;
					}
				}
#endif /* SDF_UASSF */
/* CSR_1-6042790 Change Start */
#if 0
/* CSR_1-6042790 Change End */
#ifdef SDF_SESSION_TIMER
				/*- Check if we have sent a refresh request -*/
				if ((pOriginalCallObj->pSessionTimer->dRefreshRequest
						!= Sdf_en_refresherInvalid) && \
						(sip_getHeader(pIncomingMsg, SipHdrTypeSessionExpires,\
						 &dTempHeader, (SipError*)&(pErr->stkErrCode)) 
						 != SipFail))
				{
					sip_freeSipHeader(&dTempHeader);
#ifdef SDF_TRACE
					sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_callStateTraces,\
						(Sdf_ty_s8bit*)"sdf_fn_uaCheckMessageValidity():"
						" Another request with Session-Expires header"
						" is being processed. Rejecting this req...",0,pErr);
#endif
					/*
					 * An UPDATE/INVITE has come to update Session
					 * Timer, while it is already being updated by
					 * another re-INVITE or UPDATE. Reject it with 500
					 */
					if (sdf_ivk_uaRejectRequest(pNewCallObj, \
							pOriginalCallObj->pInitData, 500, \
							&pOverlapTransInfo, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
							(Sdf_ty_s8bit*)"sdf_fn_uaCheckMessageValidity():"
							"Failed to form 500 response", pErr);
#endif
						return Sdf_en_invalid;
					}

					pErr->errCode = Sdf_en_serverInternalError;
					return Sdf_en_failed;
				}
#endif
/* CSR_1-6042790 Change Start */
#endif
/* CSR_1-6042790 Change End */
			}
			break;	
		}
		case SipMessageResponse:
		{
			SipHeader header;
			Sdf_ty_u32bit dSeqNo;
            
#ifndef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
			dValidity = sdf_fn_uaValidateViaAddr(pOriginalCallObj,  \
								pIncomingMsg, pErr);
			
			if( dValidity != Sdf_en_valid )
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity(): "
					"Error In validating the Via address",pErr);
#endif
				return dValidity;
			}
	
#endif /*Subba's modification end The Via header validation function in the UATK is commented.  This function is	resulting the problem because of wrong via header comparisons.  So for the SSF release 	i commented this validation.	After proper fix to this problem, this comment can be removed.
	The code which perform the Via header validation is kept under  #ifndef SDF_UASSF flag 	As this flag gets enabled for SSF releases, this portion of code gets stripped off. */	
						
			if(sip_getHeader(pIncomingMsg, SipHdrTypeCseq, \
				&header, (SipError*)&(pErr->stkErrCode)) == SipFail)
			{
				sdf_fn_uaUpdateCallFlow (pIncomingMsg, Sdf_co_true);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity(): "
					"Failed to get Cseq header from incoming SIP message",\
					pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}

			if(sip_getSeqNumFromCseqHdr(&header, &dSeqNo,(SipError*)\
				&(pErr->stkErrCode)) == SipFail)
			{
				sdf_fn_uaUpdateCallFlow (pIncomingMsg, Sdf_co_true);
				sip_freeSipHeader(&header);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity(): "
					"Failed to get number in Cseq header",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}
			sip_freeSipHeader(&header);


			/* 
			 * If the Cseq of the Incoming Response is greater than 
			 * the Local Cseq then the message should be discarded
			 */ 
			if ((dTxnType == Sdf_en_uacTransaction) || \
				(dTxnType == Sdf_en_uasTransaction)) 
			{
				if ((dSeqNo > pOriginalCallObj->pCommonInfo->dLocalCseq)) 
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_invalidParamError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity(): "
						"SIP Response message received is invalid since Cseq "
						"is higher than last Cseq request sent for this call",\
						pErr);
#endif
					pErr->errCode=Sdf_en_invalidParamError;
					return Sdf_en_invalid;
				}
			}	
			
			dRespCode = dMessageInfo.dResponse;

			/*
			 * If it is a final response to a INVITE, and the Cseq is
			 * exactly the same as dLocalCseq, then it is a remote
			 * retransmission, and hence can be dropped.
			 */
			
						/* 
			 * If a 2xx is received in the finalSuccessReceived state OR 
			 * a >= 3xx is received in the finalFailureReceived state OR 
			 * any kind of final response is received in the 
			 * reInviteFinalResponseReceived state,
			 * it is a retransmission.  So it can be dropped 
			 */
			if ( ((pOriginalCallObj->pCallInfo->dState == \
						Sdf_en_finalSuccessReceived) && \
						((dRespCode/100) == 2) ) || \
				 ((pOriginalCallObj->pCallInfo->dState == \
						Sdf_en_callEstablished) &&  \
						(dRespCode >= 200) ) || \
				 ((pOriginalCallObj->pCallInfo->dState == \
						Sdf_en_finalFailureReceived) &&  \
						(dRespCode >= 300) ) || \
				 ((pOriginalCallObj->pCallInfo->dState == \
						Sdf_en_reInviteFinalResponseReceived) &&  \
						(dRespCode >= 200) ))
			{
				/*
				 * If it is a final response to a INVITE, and the Cseq is
				 * exactly the same as dLocalCseq, then it is a remote
				 * retransmission, and hence can be dropped.
				 */
				if ( (dSeqNo == pOriginalCallObj->pCommonInfo->dLocalCseq) \
					&& (dMsgType == Sdf_en_invite) )
				{
					sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
						"Exiting sdf_fn_uaCheckMessageValidity");

					pErr->errCode = Sdf_en_noUaError;
					return Sdf_en_discarded;
				}
			}

			sdf_fn_uaUpdateCallFlow (pIncomingMsg, Sdf_co_true);

			dValidity = sdf_fn_uaValidateResponseState( pOriginalCallObj, \
							pIncomingMsg, dMessageInfo, pErr);
	
			if( dValidity != Sdf_en_valid )
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCheckMessageValidity(): "
					"Error In validating the State ",pErr);
#endif
				return dValidity;
			}
			break;
		}
		case SipMessageAny:
			break;
	}
	
	pErr->errCode = Sdf_en_noUaError;	

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaCheckMessageValidity ");	

	/* 
	 * If it reached here, this is a valid state transition 
	 */
	return Sdf_en_valid;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaCheckRequestLoop
**
** DESCRIPTION: Checks if the message received is looped back message or
**				not. If Incoming request has same To, From, CallId and
**				CSeq received previously and Via branch differs then
**				that message is treated as looped back message
******************************************************************************/
Sdf_ty_bool sdf_fn_uaCheckRequestLoop
#ifdef ANSI_PROTO
	(SipMessage			*pIncomingMsg,
	 Sdf_st_callObject	*pOrgCallObj,
	 Sdf_st_callObject	*pNewCallObj,
	 Sdf_st_msgInfo		dMessageInfo,
	 Sdf_st_error 		*pErr)
#else
	(pIncomingMsg, pOrgCallObj, pNewCallObj, dMessageInfo, pErr)
	 SipMessage			*pIncomingMsg;
	 Sdf_st_callObject 	*pOrgCallObj;
	 Sdf_st_callObject 	*pNewCallObj;
	 Sdf_st_msgInfo		dMessageInfo;
	 Sdf_st_error 		*pErr;
#endif
{
	SipHeader			dHeader;
	Sdf_ty_s8bit		*pViaBranch = Sdf_co_null;
	Sdf_ty_s8bit		*pLocalViaBranch = Sdf_co_null;
	Sdf_ty_u32bit 		dSeqNum=0;
	Sdf_ty_u32bit 		dLocalSeqNum=0;
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
	Sdf_st_overlapTransInfo *pOverlapTransInfo = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaCheckRequestLoop ");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaCheckRequestLoop(): pErr param passed is Invalid");
#endif
		return Sdf_co_false;
	}
	if (pOrgCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckRequestLoop(): "
			"pOrgCallObj param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_false;
	}	

	if (pNewCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckRequestLoop(): "
			"pNewCallObj param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_false;
	}	
#endif

	(void)pNewCallObj;

	if(dMessageInfo.dMsgType == Sdf_en_ack)
		return Sdf_co_false;

	if(dMessageInfo.dReqRespType != SipMessageRequest)
		return Sdf_co_false;

	dTransactionType = sdf_fn_getTransactionType( pIncomingMsg, \
							dMessageInfo, Sdf_co_true, pErr);

	if( dTransactionType == Sdf_en_RegularUasTransaction) 
	{
		/* Get the corresponding overlap transaction structure and
		 * from it extract the existing via branch */
		if (sdf_ivk_uaRetrieveMatchingOverlapTransInfo(pIncomingMsg, \
			pOrgCallObj, &pOverlapTransInfo, Sdf_en_msgModeRecd, \
			pErr) == Sdf_co_fail)
		{
			/* There ia no loop here as this the 1st message received with
			 * this CSeq */
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_false;
		}
		pLocalViaBranch = pOverlapTransInfo->pOverlapViaBranch;
		dLocalSeqNum = pOrgCallObj->pCommonInfo->dRemoteRegCseq;
	}
	else
	{
		pLocalViaBranch = pOrgCallObj->pCommonInfo->pKey->pRemoteViaBranch;
		dLocalSeqNum = pOrgCallObj->pCommonInfo->dRemoteCseq;
	}		


	if (sip_getHeader(pIncomingMsg, SipHdrTypeCseq, &dHeader, \
		(SipError *) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckRequestLoop( ): "
			"Failed to get  CSeq header from Sip Message",pErr);	
#endif		
		return Sdf_co_false;
	}
	if (sip_getSeqNumFromCseqHdr(&dHeader, &dSeqNum,(SipError *) \
		&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
		sip_freeSipHeader(&dHeader);
		if( pOverlapTransInfo != Sdf_co_null )
			(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckRequestLoop( ): "
			"Failed to get Sequence number from CSeq header from Sip Message",\
			pErr);	
#endif		
		return Sdf_co_false;
	}
	sip_freeSipHeader(&dHeader);

	if( dLocalSeqNum != dSeqNum )
	{
		if( pOverlapTransInfo != Sdf_co_null )
			(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
		return Sdf_co_false;
	}
	if (sip_getHeader(pIncomingMsg, SipHdrTypeVia, &dHeader, \
		(SipError *) &(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckRequestLoop(): "
			"Error Accessing Via Header From Sip Message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		if( pOverlapTransInfo != Sdf_co_null )
			(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
		return Sdf_co_false;
	}

	if(sdf_fn_GetBranchFromViaHdr(&dHeader,&pViaBranch, pErr) != Sdf_co_success)
	{		
		sip_freeSipHeader(&dHeader);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError, \
		(Sdf_ty_s8bit *)"sdf_fn_uaCheckRequestLoop(): "
		"Error accessing ViaBranch from Via Header in Sip Message",pErr);
#endif
		if( pOverlapTransInfo != Sdf_co_null )
			(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_false;
	}

	if((pViaBranch == Sdf_co_null)  && (pLocalViaBranch == Sdf_co_null))
	{
		sip_freeSipHeader(&dHeader);
		if( pOverlapTransInfo != Sdf_co_null )
			(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
		return Sdf_co_false;
	}

	if((pViaBranch != Sdf_co_null) && (pLocalViaBranch != Sdf_co_null))
	{
		if (Sdf_mc_strcmp(pViaBranch, pLocalViaBranch) != 0)
		{
			sip_freeSipHeader(&dHeader);
			if(pOverlapTransInfo != Sdf_co_null)
				(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
			return Sdf_co_true;
		}
	}
	else
	{
		sip_freeSipHeader(&dHeader);
		if(pOverlapTransInfo != Sdf_co_null)
			(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
		return Sdf_co_true;
	}
	sip_freeSipHeader(&dHeader);

	if(pOverlapTransInfo != Sdf_co_null)
		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
	
	return Sdf_co_false;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaRestoreOldState
**
** DESCRIPTION: Internal function to restore state if an error condition
**				occurs after change of call state.
**
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaRestoreOldState
#ifdef SDF_THREAD_SAFE
#ifdef ANSI_PROTO
(Sdf_ty_state *pState,
Sdf_ty_mutexIdT *pMutex, 
Sdf_ty_state dOldState)
#else
(pState, pMutex, dOldState)
Sdf_ty_state *pState;
Sdf_ty_mutexIdT *pMutex;
Sdf_ty_state dOldState;
#endif
#else
#ifdef ANSI_PROTO
(Sdf_ty_state *pState,
Sdf_ty_state dOldState)
#else
(pState, dOldState)
Sdf_ty_state *pState;
Sdf_ty_state dOldState;
#endif
#endif
{
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0, pMutex, 0);
#endif

	*pState = dOldState;

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, pMutex);
#endif
	return Sdf_co_success;
}
/*****************************************************************************
** FUNCTION: sdf_fn_uaRestoreOldMediaState
**
** DESCRIPTION: Internal function to restore Media state if an error condition
**				occurs after change of  Media state.
**
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaRestoreOldMediaState
#ifdef ANSI_PROTO
(Sdf_st_callObject  *pCallObject, 
Sdf_ty_mediaState	dOldMediaState)
#else
(pCallObject, dOldMediaState)
Sdf_st_callObject	*pCallObject;
Sdf_ty_state dOldMediaState;
#endif
{

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0,&(pCallObject->dCallStateMutex),0);
#endif

	pCallObject->pMediaInfo->dMediaState = dOldMediaState;

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0,&(pCallObject->dCallStateMutex));
#endif
	return Sdf_co_success;
}


/******************************************************************************
** FUNCTION: sdf_fn_uaGetHeaderListAsStrings
**
** DESCRIPTION: This function returns the list of headers as 
**	            a list of strings.
**
*******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetHeaderListAsStrings 
#ifdef ANSI_PROTO 
	(SipMessage	 *pSipMsg,
	en_HeaderType dType,
	Sdf_ty_slist *pslStrList,
	Sdf_st_error *pErr)
#else
	(pSipMsg, dType, pStrList, pErr )
	SipMessage	 *pSipMsg;
	en_HeaderType dType;
	Sdf_ty_slist *pslStrList;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dNumOfHdrs = 0;
	Sdf_ty_u32bit iter = 0;
	Sdf_ty_s8bit *pString = Sdf_co_null;
	Sdf_st_error	dLocalErr;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaGetHeaderListAsStrings");
	
	if(sdf_listInit(pslStrList, __sdf_fn_uaFreeString, \
		Sdf_co_false, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode , \
			(Sdf_ty_s8bit *)"sdf_fn_uaGetHeaderListAsStrings():  "
			"Failed to  initiliaze list of strings",pErr);	
#endif
		return Sdf_co_fail;
	}	

	/*w534*/ (void)sip_getHeaderCount(pSipMsg, dType, &dNumOfHdrs, \
			(SipError *)&(pErr->stkErrCode));

	if (dNumOfHdrs == 0)
	{
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
			"sdf_fn_uaGetHeaderListAsStrings");
		pErr->errCode = Sdf_en_noUaError;
		return Sdf_co_success;
	}

	while (iter < dNumOfHdrs)
	{
		SipHeader 	 dSipHeader;
		Sdf_ty_s8bit *pTemp = Sdf_co_null;
		
		if(sip_getHeaderAtIndex(pSipMsg, dType, &dSipHeader, iter, \
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaGetHeaderListAsStrings(): " 
				"Failed to get header from list ",pErr);
#endif
			return Sdf_co_fail;
		}
		if (dType == SipHdrTypeRequire)
		{
			if (sip_getTokenFromRequireHdr(&dSipHeader, &pTemp, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaGetHeaderListAsStrings(): " 
					"Failed to get token from Require header ",pErr);
#endif
				sip_freeSipHeader(&dSipHeader);
				return Sdf_co_fail;
			}
		}
		else if (dType == SipHdrTypeSupported)
		{
			if (sip_getOptionFromSupportedHdr(&dSipHeader, &pTemp, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
                                 /*changes for UATK CSR 1-5215238
				  coress to ALC 1-CSR 5212616*/
				if(pErr->stkErrCode == E_NO_EXIST)
				{
					iter++;
					sip_freeSipHeader(&dSipHeader);
					continue;
				}
				/*changes for UATK CSR 1-5215238
				  coress to ALC 1-CSR 5212616*/

				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaGetHeaderListAsStrings(): " 
					"Failed to get Option from Supported header ",pErr);
#endif
				sip_freeSipHeader(&dSipHeader);
				return Sdf_co_fail;
			}
		}
		
		/* Free local reference */
		sip_freeSipHeader(&dSipHeader);

		pString = Sdf_mc_strdupCallHandler(pTemp);

		if (sdf_listAppend(pslStrList, pString, pErr) == Sdf_co_fail)
		{
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pString, \
					&dLocalErr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaGetHeaderListAsStrings(): " 
				"Failed to append to string list ",pErr);
#endif
			return Sdf_co_fail;
		}
		iter++;
	}
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_fn_uaGetHeaderListAsStrings");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************************
** FUNCTION: sdf_fn_uaCompareSipHeaders
**
** DESCRIPTION: This is an internal function used by 
** 				sdf_ivk_uaCompareCallObjectKeys to compare two keys strictly
**				on basis of {(From:From)(To:To)} OR {(From:To)(To:From)}
**
*******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCompareSipHeaders
#ifdef ANSI_PROTO 
	(Sdf_st_callObjectKey *pNewKey,
	Sdf_st_callObjectKey *pExistingKey,
	Sdf_ty_callLegMatch *pCallLegMatch,
	Sdf_ty_bool			dToTagLenient,
	Sdf_st_error *pErr)
#else
	(pNewKey, pExistingKey, dExactMatch, pCallLegMatch, dToTagLenient, pErr)
	Sdf_st_callObjectKey *pNewKey;
	Sdf_st_callObjectKey *pExistingKey;
	Sdf_ty_callLegMatch *pCallLegMatch;
	Sdf_ty_bool			dToTagLenient;
	Sdf_st_error *pErr;
#endif
{
	SipHeader *pHeader1 = Sdf_co_null, *pHeader2 = Sdf_co_null;
	SipHeader *pHeader3 = Sdf_co_null, *pHeader4 = Sdf_co_null;
#ifdef SDF_SUBSCRIPTION_LAYER
	Sdf_ty_bool	 dIsCheckForkedSubs = Sdf_co_false;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaCompareSipHeaders");

#ifdef SDF_SUBSCRIPTION_LAYER
	if (*pCallLegMatch == Sdf_en_forkedSubscribe)
	{
		dIsCheckForkedSubs = Sdf_co_true;
        *pCallLegMatch = Sdf_en_notMatching;
	}
#endif

	pHeader1 = pNewKey->pLocalAddr;
	pHeader2 = pExistingKey->pLocalAddr;
	pHeader3 = pNewKey->pRemoteAddr;
	pHeader4 = pExistingKey->pRemoteAddr;
		
	/* First compare pHeader1 and pHeader2 */
	/*w534*/ (void)sdf_fn_uaCompareSipHeaderFromTo(pHeader1, pHeader2, pCallLegMatch, \
			dToTagLenient, pErr);

	if (*pCallLegMatch == Sdf_en_sameCallLeg)
	{
		/* 
		 * First pair of headers matched. Now go in for matching the second
		 * set of headers viz. pHeader3 and pHeader4 and return back 
		 * whatever this comparision returns.
		 */
#ifdef SDF_SUBSCRIPTION_LAYER
		 /* If the check for forking was set the first time, reset it here.  */
	  if (dIsCheckForkedSubs == Sdf_co_true)
      *pCallLegMatch = Sdf_en_forkedSubscribe;
#endif
	  (void)sdf_fn_uaCompareSipHeaderFromTo(pHeader3, pHeader4, \
			pCallLegMatch, dToTagLenient, pErr);
		
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_fn_uaCompareSipHeaders");

		/*Fix from UATK 2.2 Start*/
		/* IPPSTK:
		 * If NOTIFY was received before 200 ok of a subscribe
		 * Request then we need an error code for SSA to send 
		 * 200 ok for the Notify
		 */
		if (*pCallLegMatch == Sdf_en_notMatching)
		{
			pErr->errCode = Sdf_en_tagMismatchError;
		}
		else
		{
			pErr->errCode = Sdf_en_noUaError;
		}
		/*Fix from UATK 2.2 End*/
		
		return Sdf_co_success;
	}
	else
	{
		/* 
		 * pCallLegMatch is already set at this stage. Just return.
		 */
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_fn_uaCompareSipHeaders");

		pErr->errCode = Sdf_en_noUaError;

		return Sdf_co_success;
	}
}
#ifdef SDF_TEL
/******************************************************************************
** FUNCTION: sdf_fn_uaCompareSipTelAddrSpec
**
** DESCRIPTION: This is an internal function used by 
** 				sdf_ivk_uaValidateUri to compare a Tel Url and a sip Url which
**				has a tel url embedded in the sip url.it constructs a temporary 				Addrspec and calls sdf_fn_uaCompareSipAddrSpec. 
*******************************************************************************/
Sdf_ty_s8bit sdf_fn_uaCompareSipTelAddrSpec 
#ifdef ANSI_PROTO	
(SipAddrSpec *pReqAddrSpec, 
 SipAddrSpec *pLocalAddrSpec,
 Sdf_st_error *pError)
#else
(pReqAddrSpec, pLocalAddrSpec,pError)
 SipAddrSpec *pReqAddrSpec;
 SipAddrSpec *pLocalAddrSpec;
 Sdf_st_error *pError;
#endif
{
	Sdf_ty_s8bit	*pUser;
	Sdf_ty_s8bit	dTempUser[Sdf_co_smallBufferSize]="";
	SipAddrSpec		*pTempAddrSpec;
	SipUrl			*pUrl;
	Sdf_ty_u32bit	dCount=0,dIndex=0;
	SipParam 		*pParam=Sdf_co_null;
	SIP_S8bit		*pName=Sdf_co_null;
	Sdf_ty_bool		dPhone=Sdf_co_false;

	Sdf_ty_s8bit *pMethod;

	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
	"sdf_fn_uaCompareSipTelAddrSpec");


	/* Check the type of the pReqAddrSpec
	 * If the pReqAddrSpec is not SipAddrSipUri type then return fail
	 */
#ifdef SDF_TLS
	if((pReqAddrSpec->dType != SipAddrSipUri ) &&
			(pReqAddrSpec->dType != SipAddrSipSUri ))
#else
	if(pReqAddrSpec->dType != SipAddrSipUri )
#endif
			return 1;

	if(sdf_ivk_uaGetUrlFromAddrSpec(pReqAddrSpec,&pUrl,pError)==Sdf_co_fail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipTelAddrSpec(): "
			"Failed to get Url from AddrSpec ",pError);
#endif			
			return 1;
	}

	/* Check whether user=phone is present or not in the SipUrl
	 * if not present then return 1 because the sip url has not tel url 
	 * embedded in the sip url
	 * else
	 * sip url has tel url embedded in the sip url
	 */

	/* Get the count of the Url parameteres
	 */

	if(sip_getUrlParamCountFromUrl(pUrl,&dCount,(SipError*)&(pError->\
			stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipTelAddrSpec( ): "
			"Failed to get Url Count from Url",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return 1;
	}

	/* Iterate through the whole url paramter list to find the user=phone
	 */

	for(dIndex=0;dIndex<dCount;dIndex++)
	{
		if(sip_getUrlParamAtIndexFromUrl(pUrl,&pParam,dIndex,\
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipTelAddrSpec( ): "
				"Failed to get Param from Url",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return 1;
		}
		if(sip_getNameFromSipParam(pParam,&pName,(SipError*)&(pError->\
			stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipTelAddrSpec( ): "
				"Failed to get Param Name from SipParam",pError);
#endif
			/* memory leak fix */
			sip_freeSipParam(pParam);
			/* memory leak fix */
			pError->errCode=Sdf_en_headerManipulationError;
			return 1;
		}
		if(Sdf_mc_strcasecmp(pName,"user")==0)
		{
			/*Retrived the valuse */
			if(sip_getValueAtIndexFromSipParam(pParam,&pMethod,0,\
			(SipError*)&(pError->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipTelAddrSpec( ): "
					"Failed to get Param Value from SipParam",pError);
#endif
				/* memory leak fix */
				sip_freeSipParam(pParam);
				/* memory leak fix */
				pError->errCode=Sdf_en_headerManipulationError;
				return 1;
			}
			if(Sdf_mc_strcasecmp(pMethod,"phone")==0)
			{
				dPhone=Sdf_co_true;

				/* memory leak fix */
				sip_freeSipParam(pParam);
				/* memory leak fix */
				
				break;
			}
		}
		sip_freeSipParam(pParam);
	}

	/* Return 1 if user=phone not found
	 */	
	if(dPhone != Sdf_co_true)
	{
		sip_freeSipUrl(pUrl);
		return 1;
	}
	 
	if(sip_getUserFromUrl(pUrl,&pUser,\
			(SipError*)&(pError->stkErrCode))==SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipTelAddrSpec(): "
			"Failed to get User from AddrSpec ",pError);
#endif				
		sip_freeSipUrl(pUrl);
		return 1;
	}
	/*free the Url*/
	sip_freeSipUrl(pUrl);
	
	/*make temporary AddrSpec for Comparison*/
	Sdf_mc_strcat(dTempUser,"tel:");
	Sdf_mc_strcat(dTempUser,pUser);
	
	if(sip_setAddrSpecFromString(&pTempAddrSpec,\
					 dTempUser, (SipError *)&(pError->stkErrCode))==SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCompareSipTelAddrSpec(): "
			"Failed to make AddrSpec from  String",pError);
#endif				
		return 1;
	}
	
	/*Now Compare these two addrspecs*/
	if (sdf_fn_uaCompareSipAddrSpec(pLocalAddrSpec,pTempAddrSpec,\
				Sdf_co_false, Sdf_co_true, pError)== 0)
	{
		/*Success*/
		sip_freeSipAddrSpec(pTempAddrSpec);
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaCompareSipTelAddrSpec");
		return 0;
	}
	else
	{
		/*failure*/
		sip_freeSipAddrSpec(pTempAddrSpec);
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaCompareSipTelAddrSpec");
		return 1;
	}

}

/******************************************************************************
** FUNCTION: sdf_fn_uaCompareTelUri
**
** DESCRIPTION: This function is used to compare two Tel Urls
*******************************************************************************/
Sdf_ty_s8bit sdf_fn_uaCompareTelUri 
#ifdef ANSI_PROTO	
(TelUrl		 *pTel1, 
 TelUrl		 *pTel2,
 Sdf_st_error *pError)
#else
(pTel1, pTel2, pError)
 TelUrl *pTel1;
 TelUrl *pTel2;
 Sdf_st_error *pError;
#endif
{
		
	(void)pError; 

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaCompareTelUri(): pErr param passed is Invalid");
#endif
		return -1;
	}
	if(pTel1 == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCompareTelUri(): "
			"Tel url passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return 1;
	}
	if(pTel2 == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCompareTelUri(): "
			"Tel url passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return 1;
	}
#endif
	
	/* Compare the Global field of the Tel-URL
	 */

	/* If the Global filed is present one of the tel-url structure and the
	 * other tel-url structure do not have Global filed 
	 * then return 1(failure)
	 * Otherwies if the Global field is presents in both the tel-url structure
	 * then Compare each filead of the Global Tel Url Structure
	 */
	if( pTel1->pGlobal != Sdf_co_null )
	{
		Sdf_ty_u32bit 	dGlobalAreaSpecifier1=0;
		Sdf_ty_u32bit	dGlobalAreaSpecifier2=0;
		Sdf_ty_u32bit	dIndex=0;
		Sdf_ty_u32bit	dJindex=0;
		
		if(pTel2->pGlobal == Sdf_co_null )
			return 1;

		/* Compare the Base numerber of the Global phone number
		 */

		if( pTel1->pGlobal->pBaseNo == Sdf_co_null )
		{
			if( pTel2->pGlobal->pBaseNo != Sdf_co_null )
				return 1;
		}
		else
		{
			if( pTel2->pGlobal->pBaseNo == Sdf_co_null )
			return 1;
		}
		if( ( pTel1->pGlobal->pBaseNo != Sdf_co_null ) &&
			( pTel2->pGlobal->pBaseNo != Sdf_co_null ))
		{

			if( Sdf_mc_strcmp( pTel1->pGlobal->pBaseNo, \
								pTel2->pGlobal->pBaseNo) != 0)
				return 1;
		}

		/*	Compare the Isdn part of the Global Phone number
		 */

		if( pTel1->pGlobal->pIsdnSubAddr == Sdf_co_null )
		{
			if( pTel2->pGlobal->pIsdnSubAddr != Sdf_co_null )
				return 1;
		}
		else
		{
			if( pTel2->pGlobal->pIsdnSubAddr == Sdf_co_null )
				return 1;
		}

		if( ( pTel1->pGlobal->pIsdnSubAddr != Sdf_co_null ) &&
			( pTel2->pGlobal->pIsdnSubAddr != Sdf_co_null ))
		{
	
			if( Sdf_mc_strcmp( pTel1->pGlobal->pIsdnSubAddr, \
								pTel2->pGlobal->pIsdnSubAddr) != 0)
				return 1;
		}

		/* Compare the Post dial number of the Global tel-url structure
		 */

		if( pTel1->pGlobal->pPostDial == Sdf_co_null )
		{
			if( pTel2->pGlobal->pPostDial != Sdf_co_null )
				return 1;
		}
		else
		{
			if( pTel2->pGlobal->pPostDial == Sdf_co_null )
				return 1;
		}

		if( ( pTel1->pGlobal->pPostDial != Sdf_co_null ) &&
			( pTel2->pGlobal->pPostDial != Sdf_co_null ))
		{
	
			if( Sdf_mc_strcmp( pTel1->pGlobal->pPostDial, \
								pTel2->pGlobal->pPostDial) != 0)
				return 1;
		}

		/* Compare the Area Specifier
		 */
		if(sip_getAreaSpecifierCountFromTelGlobalNum(pTel1->pGlobal,\
			&dGlobalAreaSpecifier1,(SipError *)&(pError->stkErrCode)) == \
			SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareTelUri( ): "
				"Failed to get Area Specifier count from Tel Url",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return 1;
		}
		if(sip_getAreaSpecifierCountFromTelGlobalNum(pTel2->pGlobal,\
			&dGlobalAreaSpecifier2,(SipError *)&(pError->stkErrCode)) == \
			SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareTelUri( ): "
				"Failed to get Area Specifier count from Tel Url",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return 1;
		}
		if(dGlobalAreaSpecifier2 != dGlobalAreaSpecifier1)
			return 1;
		
		/*Compare each of the area specifer
		 */

		for(dIndex=0; dIndex<dGlobalAreaSpecifier1;dIndex++)
		{
			Sdf_ty_s8bit *pGloblaSpecifier1=Sdf_co_null;
			Sdf_ty_bool	dAreaSpecifierMatch=Sdf_co_false;

			if(sip_getAreaSpecifierAtIndexFromTelGlobalNum( pTel1->pGlobal,\
				&pGloblaSpecifier1, dIndex, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareTelUri( ): "
					"Failed to get Area Specifier from Tel Url",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return 1;
			}

			for(dJindex=0; dJindex<dGlobalAreaSpecifier2;dJindex++)
			{
				Sdf_ty_s8bit *pGloblaSpecifier2=Sdf_co_null;
				if(sip_getAreaSpecifierAtIndexFromTelGlobalNum(pTel2->pGlobal,\
					&pGloblaSpecifier2, dJindex, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError, \
						(Sdf_ty_s8bit *)"sdf_fn_uaCompareTelUri( ): "
						"Failed to get Area Specifier from Tel Url",pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return 1;
				}

				if(Sdf_mc_strcasecmp(pGloblaSpecifier2,pGloblaSpecifier1) ==0)
				{
					dAreaSpecifierMatch=Sdf_co_true;
					break;	
				}
			}
			if(dAreaSpecifierMatch !=Sdf_co_true)
				return 1;
		}
	}
	else 
	{	
		if( pTel2->pGlobal != Sdf_co_null )
			return 1;
	}

	/* Compare the Local phone number part of the Tel-Url Structure
	 */

	/* If the Local filed is present one of the tel-url structure and the
	 * other tel-url structure do not have Local filed 
	 * then return 1(failure)
	 * Otherwies if the Local field is presents in both the tel-url structure
	 * then Compare each filead of the Global Tel Url Structure
	 */


	if( pTel1->pLocal != Sdf_co_null )
	{
		Sdf_ty_u32bit 	dLocalAreaSpecifier1=0;
		Sdf_ty_u32bit	dLocalAreaSpecifier2=0;
		Sdf_ty_u32bit	dIndex=0;
		Sdf_ty_u32bit	dJindex=0;

		if(pTel2->pLocal == Sdf_co_null )
			return 1;

		/* Compare the Local Phone digit
		 */

		if( pTel1->pLocal->pLocalPhoneDigit == Sdf_co_null )
		{
			if( pTel2->pLocal->pLocalPhoneDigit != Sdf_co_null )
				return 1;
		}
		if( ( pTel1->pLocal->pLocalPhoneDigit != Sdf_co_null ) &&
			( pTel2->pLocal->pLocalPhoneDigit != Sdf_co_null ))
		{

			if( Sdf_mc_strcmp( pTel1->pLocal->pLocalPhoneDigit, \
								pTel2->pLocal->pLocalPhoneDigit) != 0)
				return 1;
		}

		/*	Compare the Isdn part of the Global Phone number
		 */

		if( pTel1->pLocal->pIsdnSubAddr == Sdf_co_null )
		{
			if( pTel2->pLocal->pIsdnSubAddr != Sdf_co_null )
				return 1;
		}

		if( ( pTel1->pLocal->pIsdnSubAddr != Sdf_co_null ) &&
			( pTel2->pLocal->pIsdnSubAddr != Sdf_co_null ))
		{
	
			if( Sdf_mc_strcmp( pTel1->pLocal->pIsdnSubAddr, \
								pTel2->pLocal->pIsdnSubAddr) != 0)
				return 1;
		}
		
		/* Compare the Post dial number of the Global tel-url structure
		 */

		if( pTel1->pLocal->pPostDial == Sdf_co_null )
		{
			if( pTel2->pLocal->pPostDial != Sdf_co_null )
				return 1;
		}

		if( ( pTel1->pLocal->pPostDial != Sdf_co_null ) &&
			( pTel2->pLocal->pPostDial != Sdf_co_null ))
		{
	
			if( Sdf_mc_strcmp( pTel1->pLocal->pPostDial, \
								pTel2->pLocal->pPostDial) != 0)
				return 1;
		}


		/* Compare the Area Specifier
		 */
		if(sip_getAreaSpecifierCountFromTelLocalNum(pTel1->pLocal,\
			&dLocalAreaSpecifier1,(SipError *)&(pError->stkErrCode)) == \
			SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareTelUri( ): "
				"Failed to get Area Specifier count from Tel Url",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return 1;
		}
		if(sip_getAreaSpecifierCountFromTelLocalNum(pTel2->pLocal,\
			&dLocalAreaSpecifier2,(SipError *)&(pError->stkErrCode)) == \
			SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareTelUri( ): "
				"Failed to get Area Specifier count from Tel Url",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return 1;
		}
		if(dLocalAreaSpecifier2 != dLocalAreaSpecifier1)
			return 1;
		
		/*Compare each of the area specifer
		 */

		for(dIndex=0; dIndex<dLocalAreaSpecifier1;dIndex++)
		{
			Sdf_ty_s8bit *pGloblaSpecifier1=Sdf_co_null;
			Sdf_ty_bool	dAreaSpecifierMatch=Sdf_co_false;
			if(sip_getAreaSpecifierAtIndexFromTelLocalNum(pTel1->pLocal,\
				&pGloblaSpecifier1,dIndex,(SipError *)&(pError->stkErrCode))==\
				SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCompareTelUri( ): "
					"Failed to get Area Specifier from Tel Url",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return 1;
			}

			for(dJindex=0; dJindex<dLocalAreaSpecifier2;dJindex++)
			{
				Sdf_ty_s8bit *pGloblaSpecifier2=Sdf_co_null;
				if(sip_getAreaSpecifierAtIndexFromTelLocalNum(pTel2->pLocal,\
					&pGloblaSpecifier2, dJindex, \
					(SipError	*)&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError, \
						(Sdf_ty_s8bit *)"sdf_fn_uaCompareTelUri( ): "
						"Failed to get Area Specifier from Tel Url",pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return 1;
				}
				if(Sdf_mc_strcasecmp(pGloblaSpecifier2,pGloblaSpecifier1) ==0)
				{
					dAreaSpecifierMatch=Sdf_co_true;
					break;	
				}
			}
			if(dAreaSpecifierMatch !=Sdf_co_true)
				return 1;
		}
	}
	else 
	{	
		if( pTel2->pLocal != Sdf_co_null )
			return 1;
	}
	return 0;
}
#endif

#ifdef SDF_IM
/******************************************************************************
** FUNCTION: sdf_fn_uaCompareImUrl
**
** DESCRIPTION: This function is used to compare two Im URLs 
*******************************************************************************/
Sdf_ty_s8bit sdf_fn_uaCompareImUrl 
#ifdef ANSI_PROTO	
(ImUrl	 *pImUrl1, 
 ImUrl	 *pImUrl2,
 Sdf_st_error *pError)
#else
(pImUrl1, pImUrl2, pError)
 ImUrl *pImUrl1;
 ImUrl *pImUrl2;
 Sdf_st_error *pError;
#endif
{
	Sdf_ty_s8bit	*pUserName1 = Sdf_co_null;
	Sdf_ty_s8bit	*pUserName2 = Sdf_co_null;
	Sdf_ty_s8bit	*pHostName1 = Sdf_co_null;
	Sdf_ty_s8bit	*pHostName2 = Sdf_co_null;
	Sdf_ty_u32bit	dIndex = 0;
	Sdf_ty_u32bit	dLoop = 0;
	Sdf_ty_u32bit	dRouteCount1 = 0;
	Sdf_ty_u32bit	dRouteCount2 = 0;
	Sdf_ty_u32bit	dParamCount1 = 0;
	Sdf_ty_u32bit	dParamCount2 = 0;
	

	(void)pError; 

	/* Compare User Part of the IM URL */
	if(sip_getUserFromImUrl(pImUrl1, &pUserName1, \
		(SipError*)&(pError->stkErrCode))  != SipSuccess )
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}

	if(sip_getUserFromImUrl(pImUrl2, &pUserName2, \
		(SipError*)&(pError->stkErrCode))  != SipSuccess )
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}
	if( Sdf_mc_strcasecmp(pUserName1, pUserName2) != 0)
		return 1;

	/* Compare host Part of the IM URL */
	if(sip_getHostFromImUrl(pImUrl1, &pHostName1, \
		(SipError*)&(pError->stkErrCode))  != SipSuccess )
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}

	if(sip_getHostFromImUrl(pImUrl2, &pHostName2, \
		(SipError*)&(pError->stkErrCode))  != SipSuccess )
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}
	if( Sdf_mc_strcasecmp(pHostName1, pHostName2) != 0)
		return 1;

	
	/* Compare the Route set of the IM URL */
	if(sip_getRouteCountFromImUrl(pImUrl1, &dRouteCount1, \
		(SipError*)&(pError->stkErrCode)) != SipSuccess)
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}
	if(sip_getRouteCountFromImUrl(pImUrl2, &dRouteCount2, \
		(SipError*)&(pError->stkErrCode)) != SipSuccess)
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}
	if( dRouteCount2 != dRouteCount1)
		return 1;
			
	for( dIndex=0; dIndex < dRouteCount1; dIndex++)
	{
		Sdf_ty_s8bit	*pRoute1 = Sdf_co_null;
		Sdf_ty_s8bit	*pRoute2 = Sdf_co_null;

		if(sip_getRouteAtIndexFromImUrl(pImUrl1, &pRoute1, dIndex,
			(SipError*)&(pError->stkErrCode)) != SipSuccess)
		{
			pError->errCode = pError->stkErrCode;
			return -1;
		}
		if(sip_getRouteAtIndexFromImUrl(pImUrl2, &pRoute2, dIndex,
			(SipError*)&(pError->stkErrCode)) != SipSuccess)
		{
			pError->errCode = pError->stkErrCode;
			return -1;
		}
		if( Sdf_mc_strcasecmp(pRoute1, pRoute2) != 0)
			return 1;
	}
	
	/*
	 * Complete matching including URL parameters.
	 * Start comparing URL parameters. 
	 *
	 * NOTE :
	 * -> Only URL parameters that are present in both URL's 
	 * 		are taken for the comparision.
	 * -> Paramnames are case insensitive
	 * -> Paramvalues are case sensitive
	 * -> Characters other than those in the "reserved" and 
	 * 		"unsafe" sets are equivalent to their "% HEX HEX" encoding.
	 */
	/*w534*/ (void)sip_getParamCountFromImUrl(pImUrl1, &dParamCount1, \
		(SipError *)&(pError->stkErrCode));

	/*w534*/ (void)sip_getParamCountFromImUrl(pImUrl2, &dParamCount2, \
		(SipError *)&(pError->stkErrCode));

	if( dParamCount1 != dParamCount2 )
		return 1;

	/*
	 * Iterate through each param of pUrl1
	 */
	for (dIndex = 0; dIndex < dParamCount1; dIndex++)
	{
		SipParam 		*pParam1 = Sdf_co_null;
		Sdf_ty_s8bit 	*pName1 = Sdf_co_null;
		Sdf_ty_s8bit	*pValue1 = Sdf_co_null;
		Sdf_ty_s8bit 	dHexConvertedName1[Sdf_co_smallBufferSize];
		Sdf_ty_s8bit 	dHexConvertedValue1[Sdf_co_smallBufferSize];
		Sdf_ty_bool 	dExactMatch=Sdf_co_false;

		/*w534*/ (void)sip_getParamAtIndexFromImUrl(pImUrl1, &pParam1, \
			dIndex, (SipError *)&(pError->stkErrCode));

		/*w534*/ (void)sip_getNameFromSipParam(pParam1, &pName1, \
			(SipError *)&(pError->stkErrCode));

		/*w534*/ (void)sip_getValueAtIndexFromSipParam(pParam1, &pValue1, \
			0, (SipError *)&(pError->stkErrCode));

        /*CSR 1-7574892 change start*/
        if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pName1, dHexConvertedName1,\
                    Sdf_co_smallBufferSize))
        {
            sip_freeSipParam(pParam1);

            return 1;
        }
        if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pValue1, dHexConvertedValue1,\
                    Sdf_co_smallBufferSize))
        {
            sip_freeSipParam(pParam1);
            return 1;
        }
        /*CSR 1-7574892 change start*/

		/*
		 * Now iterate through each param of pUrl2 searching
		 * for a param with the same name
		 */
		for (dLoop = 0; dLoop < dParamCount1; dLoop++)
		{
			SipParam 		*pParam2	= Sdf_co_null;
			Sdf_ty_s8bit 	*pName2		= Sdf_co_null;
			Sdf_ty_s8bit	*pValue2	= Sdf_co_null;
			Sdf_ty_s8bit dHexConvertedName2[Sdf_co_smallBufferSize];
			Sdf_ty_s8bit dHexConvertedValue2[Sdf_co_smallBufferSize];
			
			/*w534*/ (void)sip_getParamAtIndexFromImUrl(pImUrl2, &pParam2, \
				dLoop, (SipError *)&(pError->stkErrCode));

			/*w534*/ (void)sip_getNameFromSipParam(pParam2, &pName2, \
				(SipError *)&(pError->stkErrCode));

            /*CSR 1-7574892 change start*/
            if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pName2, dHexConvertedName2,\
                        Sdf_co_smallBufferSize))
            {
                sip_freeSipParam(pParam1);
                sip_freeSipParam(pParam2);
                return 1;
            }
            /*CSR 1-7574892 change end*/


			if (Sdf_mc_strcasecmp(dHexConvertedName1,dHexConvertedName2) == 0)
			{
				/*
				 * Matching names found
				 */
				/*w534*/ (void)sip_getValueAtIndexFromSipParam(pParam2, &pValue2, \
					0, (SipError *)&(pError->stkErrCode));

                /*CSR 1-7574892 change start*/
                if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pValue2, dHexConvertedValue2,\
                            Sdf_co_smallBufferSize))
                {
                    sip_freeSipParam(pParam1);
                    sip_freeSipParam(pParam2);

                    return 1;
                }
                /*CSR 1-7574892 change end*/

				if ((dHexConvertedValue1[0] != '\0') && \
					(dHexConvertedValue2[0] != '\0') && \
					(Sdf_mc_strcmp(dHexConvertedValue1,dHexConvertedValue2)==0))
				{
					/*
					 * Param name and param values match. Break
					 * out and pick up the next param
					 */
					sip_freeSipParam(pParam2);
					dExactMatch=Sdf_co_true;
					break;
				}
				else if ( (dHexConvertedValue1[0] == '\0') && \
					(dHexConvertedValue2[0] == '\0') )
				{
					/*
					 * Both param values are NULL. Break out
					 * and pick up the next param
					 */
					dExactMatch=Sdf_co_true;
					sip_freeSipParam(pParam2);
					break;
				}
				else
				{
					/*
					 * A url-parameter name matched, but the values
					 * did not match. Return 1.
					 */
					sip_freeSipParam(pParam1);
					sip_freeSipParam(pParam2);
					return 1;
				}
			}
			sip_freeSipParam(pParam2);
		}
		sip_freeSipParam(pParam1);

		/* If no match found then return failure */
		if(dExactMatch==Sdf_co_false)
			return 1;
	}

	return 0;
}
#endif

#ifdef SDF_PRES
/******************************************************************************
** FUNCTION: sdf_fn_uaComparePresUrl
**
** DESCRIPTION: This function is used to compare two Im URLs 
*******************************************************************************/
Sdf_ty_s8bit sdf_fn_uaComparePresUrl 
#ifdef ANSI_PROTO	
(PresUrl	 *pPresUrl1, 
 PresUrl	 *pPresUrl2,
 Sdf_st_error *pError)
#else
(pPresUrl1, pPresUrl2, pError)
 PresUrl *pPresUrl1;
 PresUrl *pPresUrl2;
 Sdf_st_error *pError;
#endif
{
	Sdf_ty_s8bit	*pUserName1 = Sdf_co_null;
	Sdf_ty_s8bit	*pUserName2 = Sdf_co_null;
	Sdf_ty_s8bit	*pHostName1 = Sdf_co_null;
	Sdf_ty_s8bit	*pHostName2 = Sdf_co_null;
	Sdf_ty_u32bit	dIndex = 0;
	Sdf_ty_u32bit	dLoop = 0;
	Sdf_ty_u32bit	dRouteCount1 = 0;
	Sdf_ty_u32bit	dRouteCount2 = 0;
	Sdf_ty_u32bit	dParamCount1 = 0;
	Sdf_ty_u32bit	dParamCount2 = 0;
	

	(void)pError; 

	/* Compare User Part of the IM URL */
	if(sip_getUserFromPresUrl(pPresUrl1, &pUserName1, \
		(SipError*)&(pError->stkErrCode))  != SipSuccess )
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}

	if(sip_getUserFromPresUrl(pPresUrl2, &pUserName2, \
		(SipError*)&(pError->stkErrCode))  != SipSuccess )
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}
	if( Sdf_mc_strcasecmp(pUserName1, pUserName2) != 0)
		return 1;

	/* Compare host Part of the IM URL */
	if(sip_getHostFromPresUrl(pPresUrl1, &pHostName1, \
		(SipError*)&(pError->stkErrCode))  != SipSuccess )
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}

	if(sip_getHostFromPresUrl(pPresUrl2, &pHostName2, \
		(SipError*)&(pError->stkErrCode))  != SipSuccess )
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}
	if( Sdf_mc_strcasecmp(pHostName1, pHostName2) != 0)
		return 1;

	
	/* Compare the Route set of the IM URL */
	if(sip_getRouteCountFromPresUrl(pPresUrl1, &dRouteCount1, \
		(SipError*)&(pError->stkErrCode)) != SipSuccess)
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}
	if(sip_getRouteCountFromPresUrl(pPresUrl2, &dRouteCount2, \
		(SipError*)&(pError->stkErrCode)) != SipSuccess)
	{
		pError->errCode = pError->stkErrCode;
		return -1;
	}
	if( dRouteCount2 != dRouteCount1)
		return 1;
			
	for( dIndex=0; dIndex < dRouteCount1; dIndex++)
	{
		Sdf_ty_s8bit	*pRoute1 = Sdf_co_null;
		Sdf_ty_s8bit	*pRoute2 = Sdf_co_null;

		if(sip_getRouteAtIndexFromPresUrl(pPresUrl1, &pRoute1, dIndex,
			(SipError*)&(pError->stkErrCode)) != SipSuccess)
		{
			pError->errCode = pError->stkErrCode;
			return -1;
		}
		if(sip_getRouteAtIndexFromPresUrl(pPresUrl2, &pRoute2, dIndex,
			(SipError*)&(pError->stkErrCode)) != SipSuccess)
		{
			pError->errCode = pError->stkErrCode;
			return -1;
		}
		if( Sdf_mc_strcasecmp(pRoute1, pRoute2) != 0)
			return 1;
	}
	
	/*
	 * Complete matching including URL parameters.
	 * Start comparing URL parameters. 
	 *
	 * NOTE :
	 * -> Only URL parameters that are present in both URL's 
	 * 		are taken for the comparision.
	 * -> Paramnames are case insensitive
	 * -> Paramvalues are case sensitive
	 * -> Characters other than those in the "reserved" and 
	 * 		"unsafe" sets are equivalent to their "% HEX HEX" encoding.
	 */
	(void)sip_getParamCountFromPresUrl(pPresUrl1, &dParamCount1, \
		(SipError *)&(pError->stkErrCode));

	(void)sip_getParamCountFromPresUrl(pPresUrl2, &dParamCount2, \
		(SipError *)&(pError->stkErrCode));

	if( dParamCount1 != dParamCount2 )
		return 1;

	/*
	 * Iterate through each param of pUrl1
	 */
	for (dIndex = 0; dIndex < dParamCount1; dIndex++)
	{
		SipParam 		*pParam1 = Sdf_co_null;
		Sdf_ty_s8bit 	*pName1 = Sdf_co_null;
		Sdf_ty_s8bit	*pValue1 = Sdf_co_null;
		Sdf_ty_s8bit 	dHexConvertedName1[Sdf_co_smallBufferSize];
		Sdf_ty_s8bit 	dHexConvertedValue1[Sdf_co_smallBufferSize];
		Sdf_ty_bool 	dExactMatch=Sdf_co_false;

		(void)sip_getParamAtIndexFromPresUrl(pPresUrl1, &pParam1, \
			dIndex, (SipError *)&(pError->stkErrCode));

		(void)sip_getNameFromSipParam(pParam1, &pName1, \
			(SipError *)&(pError->stkErrCode));

		(void)sip_getValueAtIndexFromSipParam(pParam1, &pValue1, \
			0, (SipError *)&(pError->stkErrCode));

        /*CSR 1-7574892 change start*/
        if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pName1, dHexConvertedName1,\
                    Sdf_co_smallBufferSize))
        {
            sip_freeSipParam(pParam1);

            return 1;
        }
        if (Sdf_co_fail ==sdf_fn_uaConvertHexEncodedString(pValue1, dHexConvertedValue1,\
                    Sdf_co_smallBufferSize))
        {
            sip_freeSipParam(pParam1);
            return 1;
        }
        /*CSR 1-7574892 change end*/

		/*
		 * Now iterate through each param of pUrl2 searching
		 * for a param with the same name
		 */
		for (dLoop = 0; dLoop < dParamCount1; dLoop++)
		{
			SipParam 		*pParam2	= Sdf_co_null;
			Sdf_ty_s8bit 	*pName2		= Sdf_co_null;
			Sdf_ty_s8bit	*pValue2	= Sdf_co_null;
			Sdf_ty_s8bit dHexConvertedName2[Sdf_co_smallBufferSize];
			Sdf_ty_s8bit dHexConvertedValue2[Sdf_co_smallBufferSize];
			
			(void)sip_getParamAtIndexFromPresUrl(pPresUrl2, &pParam2, \
				dLoop, (SipError *)&(pError->stkErrCode));

			(void)sip_getNameFromSipParam(pParam2, &pName2, \
				(SipError *)&(pError->stkErrCode));

            /*CSR 1-7574892 change start*/
            if ( Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pName2, dHexConvertedName2,\
                        Sdf_co_smallBufferSize))
            {
                sip_freeSipParam(pParam1);
                sip_freeSipParam(pParam2);

                return 1;
            }
            /*CSR 1-7574892 change end*/


			if (Sdf_mc_strcasecmp(dHexConvertedName1,dHexConvertedName2) == 0)
			{
				/*
				 * Matching names found
				 */
				(void)sip_getValueAtIndexFromSipParam(pParam2, &pValue2, \
					0, (SipError *)&(pError->stkErrCode));

                /*CSR 1-7574892 change start*/
                if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pValue2, dHexConvertedValue2,\
                            Sdf_co_smallBufferSize))
                {
                    sip_freeSipParam(pParam1);
                    sip_freeSipParam(pParam2);

                    return 1;
                }
                /*CSR 1-7574892 change end*/

				if ((dHexConvertedValue1[0] != '\0') && \
					(dHexConvertedValue2[0] != '\0') && \
					(Sdf_mc_strcmp(dHexConvertedValue1,dHexConvertedValue2)==0))
				{
					/*
					 * Param name and param values match. Break
					 * out and pick up the next param
					 */
					sip_freeSipParam(pParam2);
					dExactMatch=Sdf_co_true;
					break;
				}
				else if ( (dHexConvertedValue1[0] == '\0') && \
					(dHexConvertedValue2[0] == '\0') )
				{
					/*
					 * Both param values are NULL. Break out
					 * and pick up the next param
					 */
					dExactMatch=Sdf_co_true;
					sip_freeSipParam(pParam2);
					break;
				}
				else
				{
					/*
					 * A url-parameter name matched, but the values
					 * did not match. Return 1.
					 */
					sip_freeSipParam(pParam1);
					sip_freeSipParam(pParam2);
					return 1;
				}
			}
			sip_freeSipParam(pParam2);
		}
		sip_freeSipParam(pParam1);

		/* If no match found then return failure */
		if(dExactMatch==Sdf_co_false)
			return 1;
	}

	return 0;
}
#endif

/*****************************************************************************
** FUNCTION: sdf_fn_GetBranchFromViaHdr
**
** DESCRIPTION: To Get the Via branch from the via header.
**
******************************************************************************/

Sdf_ty_retVal sdf_fn_GetBranchFromViaHdr
#ifdef ANSI_PROTO
	(SipHeader 	  *pHeader,
	 Sdf_ty_s8bit **ppBranch,
	 Sdf_st_error *pErr)
#else
	(pHeader,ppBranch,pErr)
	SipHeader	 *pHeader;
	Sdf_ty_s8bit **ppBranch;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dIndex, size;
	SipViaHeader *pViaHeader = (SipViaHeader *)(pHeader->pHeader);

	if(sip_listSizeOf(&(pViaHeader->slParam), &size, \
		(SipError *)&(pErr->stkErrCode)) == SipFail)
		return Sdf_co_fail;

	dIndex = 0;
	while (dIndex < size)
	{
		SipParam *pParam;
		
		if(sip_listGetAt(&(pViaHeader->slParam), dIndex, \
			((SIP_Pvoid *) &pParam), (SipError *)&(pErr->stkErrCode))==SipFail)
			return Sdf_co_fail;

		if(pParam->pName != Sdf_co_null)
		{
			/* changes for CSR_1-5436128 Starts */
			if(Sdf_mc_strcasecmp(pParam->pName,"branch")==0)
			/* changes for CSR_1-5436128 Ends */            
			{
				SIP_S8bit *pBranch;

				if(sip_listGetAt(&(pParam->slValue),0,((SIP_Pvoid *) &pBranch),\
					(SipError *)&(pErr->stkErrCode) ) != SipFail)
				{
					*ppBranch = pBranch;
					return Sdf_co_success;
				}
			}
		}
		dIndex++;
	}
	return Sdf_co_fail;
}

/*****************************************************************************
** FUNCTION: sdf_fn_InsertBranchFromViaHdr
**
** DESCRIPTION: To Insert new Via branch into the Via Header
**
******************************************************************************/

Sdf_ty_retVal sdf_fn_InsertBranchFromViaHdr
#ifdef ANSI_PROTO
	(SipHeader *pViaHeader,
	 Sdf_ty_s8bit *pBranch,
	 Sdf_st_error *pErr)
#else
	(pViaHeader,pBranch,pErr)
	SipHeader *pViaHeader;
	Sdf_ty_s8bit *pBranch;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_slist	slBranchList;

	if(sdf_listInit(&slBranchList, __sdf_fn_uaFreeString, Sdf_co_false, \
					pErr) == Sdf_co_fail)
		return Sdf_co_fail;
		
	if(sdf_listInsertAt(&slBranchList, 0, (Sdf_ty_pvoid)pBranch, \
			pErr) ==	Sdf_co_fail)
		return Sdf_co_fail;

	if( sdf_ivk_uaInsertHeaderParam(pViaHeader, (Sdf_ty_s8bit *)"branch", \
			&slBranchList, 0, pErr) == Sdf_co_fail)
		return Sdf_co_fail;

	(void)sdf_listDeleteAll(&slBranchList, pErr);

	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaRejectRequest
 **
 ** DESCRIPTION: This function has to be invoked to send a failure response to 
 **				 a recd request if any validation prior to 
 **				 sdf_ivk_uaUpdateCallDetails fails. The call object passed to 
 **				 this API is the temporary call object returned by 
 **				 sdf_ivk_uaDecodeMessage().
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaRejectRequest
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj, Sdf_st_initData *pInitData, \
	 Sdf_ty_u16bit dRespCode, SipMessage **ppSipMsg, Sdf_ty_transactionType \
	 transType, Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	 Sdf_st_error *pError)
#else
	(pCallObj, pInitData, dRespCode, ppSipMsg, pError)
	Sdf_st_callObject *pCallObj;
	Sdf_st_initData *pInitData;
	Sdf_ty_u16bit dRespCode;
	SipMessage **ppSipMsg;
	Sdf_st_error *pError;
#endif
{
	SipMessage *pOutMsg = Sdf_co_null;
	Sdf_ty_bool dUAResponseToOverlapTrans = Sdf_co_false;
	Sdf_ty_messageType dMethodType = Sdf_en_unknownRequest;
	SipHeader 	  dHeader;
	Sdf_ty_u32bit dTagCount = 0;
	SipMessage *pSipMsg=Sdf_co_null;
	Sdf_st_msgInfo	dMessageInfo;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaRejectRequest");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaRejectRequest(): pError param passed is invalid");
#endif
		return Sdf_co_fail;
	}

	if (pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaRejectRequest( ): "
			"Invalid call object passed as parameter ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of parameter validations */


	if( (transType==Sdf_en_overlapTransaction) ||
		(transType == Sdf_en_RegularUasTransaction))
		pSipMsg=pOverlapTransInfo->pSipMsg;

	else if(transType==Sdf_en_uasTransaction)
		pSipMsg=pCallObj->pUasTransaction->pSipMsg;

	else if(transType == Sdf_en_uacTransaction)
		pSipMsg=pCallObj->pUacTransaction->pSipMsg;

    /*resolving klocworks warning*/
    if(Sdf_co_null == pSipMsg)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
              (Sdf_ty_s8bit *)"sdf_fn_uaRejectRequest(): "
              "Invalid Sip message passed as parameter",pError);
#endif
        pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	 /*w534*/ (void)sdf_fn_uaGetMessageInfo(pSipMsg, &dMessageInfo, pError);

	if (sdf_fn_uaUpdateCallObject(pCallObj, pOverlapTransInfo, \
		pSipMsg, dMessageInfo, pError) == Sdf_co_fail)
	{	
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaRejectRequest(): "
			"Updation of CallObject details failed",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}
	/*
	 * Don't change call state if we are about to form a failure response
	 * to a stray PRACK/COMET
	 */
	dMethodType = dMessageInfo.dMsgType;
#ifdef SIP_RPR
	if ((dMethodType == Sdf_en_prack) || \
		(dMethodType == Sdf_en_comet) )
#else
	if (dMethodType == Sdf_en_comet )
#endif
		dUAResponseToOverlapTrans = Sdf_co_true;
	
	if (dUAResponseToOverlapTrans != Sdf_co_true)
	{
		if (sdf_ivk_uaChangeCallState(pCallObj, pOverlapTransInfo, \
			pSipMsg, Sdf_en_msgModeRecd, \
			pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callStateError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaRejectRequest(): "
				"Changing call state failed", pError);
#endif
			pError->errCode=Sdf_en_callStateError;
			return Sdf_co_fail;
		}
	}

	/*
	 * For the response for the request that was recd.
	 */
	if (sip_getHeader(pSipMsg, \
		SipHdrTypeTo, &dHeader, \
		(SipError *)&(pError->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *) "sdf_fn_uaRejectRequest( ):"
			"Failed to get the To header from incoming SipMessage", pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	/*w534*/ (void)sip_getTagCountFromToHdr(&dHeader, &dTagCount, \
		(SipError *)&(pError->stkErrCode));
	if (dTagCount == 0)
	{
		Sdf_ty_s8bit	*pVarToTag = Sdf_co_null;
/*#SPR-4627: ifndef SDF_UAB2B_2543 flag is added to give backup
 * compatibility for 2543 to B2B. In 2543 there is no concept of
 * TOtags and From tag */

#ifndef SDF_UAB2B_2543

		if (sdf_fn_uaGenerateGlobalTag( pInitData->pTag, &pVarToTag) \
				== Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, \
				Sdf_en_tagGenerationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaRejectRequest( ): "
				"Failed to generate variable portion of the tag",pError);
#endif					
			pError->errCode=Sdf_en_tagGenerationError;
			return Sdf_co_fail;
		}

		if (sip_insertTagAtIndexInToHdr(&dHeader, \
			pVarToTag, 0, (SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaRejectRequest( ): "
				"Failed to insert Tag in To header",pError);
#endif	
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)(&pVarToTag), pError);
			sip_freeSipHeader(&dHeader);			
			return Sdf_co_fail;
		}
#endif /*spr-4627 end giving backup compatibility for rfc 2543 to B2B*/
         
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)(&pVarToTag), pError);
	}
	sip_freeSipHeader(&dHeader);			

	if (sdf_ivk_uaFormResponseMessage(pInitData, pSipMsg, &pOutMsg, \
		dRespCode, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
			(Sdf_ty_s8bit *) "sdf_fn_uaRejectRequest( ):"
			"Failed to form a SIP failure response.",pError);
#endif
		return Sdf_co_fail;
	}
	
	/*
	 * Add any headers that are particular to this status code.
	 */
	if((sdf_ivk_uaAddStatusCodeSpecificHeaders(pCallObj, pInitData, \
		pOutMsg, dRespCode, pError)) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaRejectRequest(): "
			"Failed to add Status Code Specific headers", pError);	
#endif
        /* klock tool error fix */
		sip_freeSipMessage(pOutMsg);
		return Sdf_co_fail;
	}

	/*
	 * Over-write the request recd with the response just made.
	 */
	sip_freeSipMessage(*ppSipMsg);
	*ppSipMsg = pOutMsg;

	/* Update the Overlap Info structure with the Cseq from message */
	if( pOverlapTransInfo != Sdf_co_null )
	{
		SipHeader 			pMsgHeader;
		Sdf_ty_u32bit 		dSeqNum=0;

		/*w534*/ (void)sip_getHeader( pOutMsg, SipHdrTypeCseq, \
			&pMsgHeader, (SipError *) &(pError->stkErrCode));
		
		/*w534*/ (void)sip_getSeqNumFromCseqHdr(&pMsgHeader, &dSeqNum, \
				(SipError *)&(pError->stkErrCode));

		sip_freeSipHeader(&pMsgHeader);

		if( pOverlapTransInfo->dRemoteCseq == 0 )
			pOverlapTransInfo->dRemoteCseq = dSeqNum;
	}

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_fn_uaRejectRequest");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_getTransactionType
**
** DESCRIPTION: Based on the Incoming message it decides what kind of 
**				Transaction this belogns.
**
******************************************************************************/
Sdf_ty_transactionType sdf_fn_getTransactionType
#ifdef ANSI_PROTO
	(SipMessage		*pMessage,
	 Sdf_st_msgInfo	dMessageInfo,
	 Sdf_ty_bool	dIsIncoming,
	 Sdf_st_error   *pError)
#else
	(pMessage, dMessageInfo, dIsIncoming, pError)
	(SipMessage		*pMessage;
	 Sdf_st_msgInfo	dMessageInfo;
	 Sdf_ty_bool	dIsIncoming;
	 Sdf_st_error   *pError)
#endif
{
#ifdef SIP_RPR
	if( pMessage != Sdf_co_null )
	{
		if( dMessageInfo.dReqRespType == SipMessageResponse )
		{
			if( sdf_fn_uaIsMessageRPR(pMessage, pError) == 0)
				return Sdf_en_overlapTransaction;
		}
	}

#endif
#ifdef SIP_RPR
	if((dMessageInfo.dMsgType == Sdf_en_prack) || \
	   (dMessageInfo.dMsgType == Sdf_en_comet))
#else
	if(dMessageInfo.dMsgType == Sdf_en_comet)
#endif
		return Sdf_en_overlapTransaction;

	if( (dMessageInfo.dMsgType == Sdf_en_invite)	||
		(dMessageInfo.dMsgType == Sdf_en_cancel)	||
		(dMessageInfo.dMsgType == Sdf_en_ack)	||
		(dMessageInfo.dMsgType == Sdf_en_bye)	)
	{
		if(dMessageInfo.dReqRespType == SipMessageResponse)
		{
			if(dIsIncoming == Sdf_co_true)
				return Sdf_en_uacTransaction;
			else
				return Sdf_en_uasTransaction;
		}
		else
		{
			if(dIsIncoming == Sdf_co_true)
				return Sdf_en_uasTransaction;
			else
				return Sdf_en_uacTransaction;
		}
	}

	/* All other methods are treated as regular transactions */

	if(dMessageInfo.dReqRespType == SipMessageResponse)
	{
		if(dIsIncoming == Sdf_co_true)
			return Sdf_en_RegularUacTransaction;
		else
			return Sdf_en_RegularUasTransaction;
	}
	else if(dMessageInfo.dReqRespType == SipMessageRequest)
	{
		if(dIsIncoming == Sdf_co_true)
			return Sdf_en_RegularUasTransaction;
		else
			return Sdf_en_RegularUacTransaction;
	}
	return Sdf_en_InvalidTransaction;
}

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSetOverlapInfoCancelled
 **
 ** DESCRIPTION: This function is used to make all the overlap transactions 
 **				 state to cancelled state. So that all the processing 
 **	             transactions over this terminated call object will get 4xx 
 **				 response.
 **
 ******************************************************************************/
Sdf_ty_retVal	sdf_fn_uaSetOverlapInfoCancelled
#ifdef ANSI_PROTO
	( Sdf_st_callObject *pCallObj, Sdf_st_error *pError)
#else	
	(pCallObj, pError)
	Sdf_st_callObject 	*pCallObj;
	Sdf_st_error 		*pError;
#endif
{
	Sdf_st_listIterator dListIterator;
	Sdf_st_overlapTransInfo *pTransInfo;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Entering "
			"sdf_ivk_uaSetOverlapInfoCancelled");
	/*
	 * Now iterate through call objects's OverlapTransInfo elements
     * and get the relevant one
     */
	(void)sdf_listInitIterator(&(pCallObj->slOverlapTransInfo), \
		&dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		pTransInfo = (Sdf_st_overlapTransInfo*)\
			(dListIterator.pCurrentElement->pData);
		/* Set the transaction state to Cancelled */
		pTransInfo->dState = Sdf_en_transactionCancel;

		(void)sdf_listNext(&dListIterator, pError);
	}

	pError->errCode = Sdf_en_noExistError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetOverlapInfoCancelled");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaCheckToTagValidity
**
** DESCRIPTION: Checks if the message received has valid To tag value. If 
**				Original call object has To tag and incoming request has no
**				To tag then this function returns Sdf_co_fail. 
**				If the invalid message is an Request then this function
**				rejects the request with 481 response. 

*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaCheckToTagValidity
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pOrgCallObj,
	 Sdf_st_callObject *pNewCallObj,
	 Sdf_st_msgInfo		dMessageInfo,
	 Sdf_st_eventContext *pEventContext,
	 Sdf_st_error *pErr)
#else
	(pOrgCallObj, pNewCallObj, dMessageInfo, pEventContext, pErr)
	 Sdf_st_callObject *pOrgCallObj;
	 Sdf_st_callObject *pNewCallObj;
	 Sdf_st_msgInfo		dMessageInfo;
	 Sdf_st_eventContext *pEventContext;
	 Sdf_st_error *pErr;
#endif
{
	SipHeader			*pExistRemoteHdr = Sdf_co_null;
	SipHeader			*pNewRemoteHdr	= Sdf_co_null;
	Sdf_ty_s8bit		*pExistRemoteTag = Sdf_co_null;
	Sdf_ty_s8bit		*pNewRemoteTag = Sdf_co_null;
	SipHeader			*pExistLocalHdr = Sdf_co_null;
	SipHeader			*pNewLocalHdr	= Sdf_co_null;
	Sdf_ty_s8bit		*pExistLocalTag = Sdf_co_null;
	Sdf_ty_s8bit		*pNewLocalTag = Sdf_co_null;
	Sdf_ty_bool			dIsInvalid = Sdf_co_false;
	Sdf_ty_bool			dLocalTagMismatch = Sdf_co_false;
	Sdf_ty_bool			dRemoteTagMismatch = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaCheckToTagValidity ");

	/* For Cancel and Non 2xx Response to Cancel, relax the To Tag
	 * Validation. As these are hop by hop responses, they can have
	 * different To Tags */
	
	if(dMessageInfo.dMsgType == Sdf_en_cancel) 
		return Sdf_co_success;

	if(dMessageInfo.dReqRespType == SipMessageResponse)
	{
		if((dMessageInfo.dMsgType == Sdf_en_invite) && \
			(dMessageInfo.dResponse > 299))
			return Sdf_co_success;
	}

	pExistRemoteHdr = pOrgCallObj->pCommonInfo->pKey->pRemoteAddr;
	pExistLocalHdr  = pOrgCallObj->pCommonInfo->pKey->pLocalAddr;

	if( pExistRemoteHdr->dType == SipHdrTypeTo )
		/*w534*/ (void)sip_getTagAtIndexFromToHdr(pExistRemoteHdr, &pExistRemoteTag, 0, \
				(SipError *)&(pErr->stkErrCode));
	else
		/*w534*/ (void)sip_getTagAtIndexFromFromHdr(pExistRemoteHdr, &pExistRemoteTag, 0, \
				(SipError *)&(pErr->stkErrCode));

	if( pExistLocalHdr->dType == SipHdrTypeTo )
		/*w534*/ (void)sip_getTagAtIndexFromToHdr(pExistLocalHdr, &pExistLocalTag, 0, \
				(SipError *)&(pErr->stkErrCode));
	else
		/*w534*/ (void)sip_getTagAtIndexFromFromHdr(pExistLocalHdr, &pExistLocalTag, 0, \
				(SipError *)&(pErr->stkErrCode));

	pNewRemoteHdr = pNewCallObj->pCommonInfo->pKey->pRemoteAddr;
	pNewLocalHdr  = pNewCallObj->pCommonInfo->pKey->pLocalAddr;

	if( pNewRemoteHdr->dType == SipHdrTypeTo )
		/*w534*/ (void)sip_getTagAtIndexFromToHdr(pNewRemoteHdr, &pNewRemoteTag, 0, \
				(SipError *)&(pErr->stkErrCode));
	else
		/*w534*/ (void)sip_getTagAtIndexFromFromHdr(pNewRemoteHdr, &pNewRemoteTag, 0, \
				(SipError *)&(pErr->stkErrCode));

	if( pNewLocalHdr->dType == SipHdrTypeTo )
		/*w534*/ (void)sip_getTagAtIndexFromToHdr(pNewLocalHdr, &pNewLocalTag, 0, \
				(SipError *)&(pErr->stkErrCode));
	else
		/*w534*/ (void)sip_getTagAtIndexFromFromHdr(pNewLocalHdr, &pNewLocalTag, 0, \
				(SipError *)&(pErr->stkErrCode));
	
	/* Check the Local Tag Validation */

	if((pExistLocalTag != Sdf_co_null) && ( pNewLocalTag == Sdf_co_null))
	{
		dIsInvalid = Sdf_co_true;
		dLocalTagMismatch = Sdf_co_true;
	}
	else if((pExistLocalTag != Sdf_co_null) && ( pNewLocalTag != Sdf_co_null))
	{
		if(Sdf_mc_strcmp(pExistLocalTag, pNewLocalTag) != 0)
		{
			dIsInvalid = Sdf_co_true;
			dLocalTagMismatch = Sdf_co_true;
		}
	}

	if((pExistRemoteTag != Sdf_co_null) && (pNewRemoteTag == Sdf_co_null))
	{
		dIsInvalid = Sdf_co_true;
		dRemoteTagMismatch = Sdf_co_true;
	}
	else if((pExistRemoteTag != Sdf_co_null) && (pNewRemoteTag != Sdf_co_null))
	{
		if(Sdf_mc_strcmp(pExistRemoteTag, pNewRemoteTag) != 0)
		{
			dIsInvalid = Sdf_co_true;
			dRemoteTagMismatch = Sdf_co_true;
		}
	}

	if(dIsInvalid == Sdf_co_true) 
	{
		pErr->errCode = Sdf_en_tagMismatchError;

		if(dMessageInfo.dReqRespType == SipMessageRequest)
		{
			if(dMessageInfo.dMsgType != Sdf_en_invite )
			{
			SipMessage	*pTempMessage = Sdf_co_null;
			Sdf_st_overlapTransInfo	*pOverlapTransInfo = Sdf_co_null;
#ifndef SDF_LINT
            Sdf_st_socket	*pSockfd = Sdf_co_null;
#endif			
			if (sdf_ivk_uaRejectRequest(pNewCallObj, \
				 pOrgCallObj->pInitData, 481, \
				 &pOverlapTransInfo, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_fn_uaCheckToTagValidity()"
					": Failed to form 481 response", pErr);
#endif
				return Sdf_co_fail;
			}

			if( pOverlapTransInfo != Sdf_co_null )
				pTempMessage = pOverlapTransInfo->pSipMsg;
			else
			{
				pTempMessage = pNewCallObj->pUasTransaction->pSipMsg;
#ifndef SDF_LINT
                pSockfd = &(pNewCallObj->pUasTransaction->dSocket);
#endif                
			}

			/* If Local tag is not matching, then set the currect local tag
			 * to this failed response */
			if( dLocalTagMismatch == Sdf_co_true )
			{
				SipHeader		dHeader;
				Sdf_ty_s8bit	*pTmpTagValue = Sdf_co_null;
				
				/*w534*/ (void)sip_getHeader(pTempMessage, SipHdrTypeTo, &dHeader,\
					(SipError*)	&(pErr->stkErrCode));
		
				pTmpTagValue = pExistLocalTag;
				/*w534*/ (void)sip_setTagAtIndexInToHdr(&dHeader, pTmpTagValue, 0, \
						(SipError *)&(pErr->stkErrCode));

				sip_freeSipHeader(&dHeader);
			}

				pErr->errCode = Sdf_en_localTagMismatchError;
				sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
						"Exiting sdf_fn_uaCheckToTagValidity");
				return Sdf_co_fail;
			}
		}
		else if(dMessageInfo.dReqRespType == SipMessageResponse)
		{
			/*
			 * This if condition was modified to add the check for 
			 * Sdf_en_finalSuccessReceived state also - SPR 2572.
			 */
			if( (dRemoteTagMismatch == Sdf_co_true) && \
				(dMessageInfo.dMsgType == Sdf_en_invite) &&
				((pOrgCallObj->pCallInfo->dState == Sdf_en_inviteSent) || \
				 (pOrgCallObj->pCallInfo->dState == \
				 	Sdf_en_provisionalRespReceived) || \
				 (pOrgCallObj->pCallInfo->dState == \
				 	Sdf_en_finalSuccessReceived) || \
				 (pOrgCallObj->pCallInfo->dState == Sdf_en_callEstablished)) )
			{
				if(sdf_fn_uaHandleForkedResponse(pNewCallObj, \
					pOrgCallObj, pEventContext, pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
						(Sdf_ty_s8bit*)"sdf_fn_uaCheckToTagValidity() :" \
						"Handling the Forked response failed",pErr);
#endif
				}
				pErr->errCode = Sdf_en_forkedRespError;
			}
			return Sdf_co_fail;
		}
		else
			return Sdf_co_fail;
	}
	return Sdf_co_success;
}

/***********************************************************************
** FUNCTION: sdf_fn_uaIsCancelMatching
**
** DESCRIPTION: This function matches the CANCEL UAS transaction
**		in the pNewCallObj with the INVITE UAS transaction
**		in pExistingCallObj based on the transaction
**		matching procedures described in Section 17.2.3
**		of RFC 3261 for a CANCEL request.
**		It returns Sdf_co_true if the CANCEL matches
**		the INVITE and Sdf_co_false otherwise.
**
************************************************************************/
Sdf_ty_bool sdf_fn_uaIsCancelMatching
#ifdef ANSI_PROTO
	(Sdf_st_callObject 	*pNewCallObj,
	Sdf_st_callObject 	*pExistingCallObj,
	Sdf_st_error *pErr)
#else
	(pNewCallObj,pExisitingCallObj,pErr)
	Sdf_st_callObject 	*pNewCallObj;
	Sdf_st_callObject 	*pExistingCallObj;
	Sdf_st_error 		*pErr;
#endif
{
	Sdf_st_commonInfo	*pNewCommonInfo = Sdf_co_null; 
	Sdf_st_commonInfo	*pExistingCommonInfo = Sdf_co_null;

	SipMessage		*pNewSipMsg = Sdf_co_null;
	SipMessage		*pExistingSipMsg = Sdf_co_null;

	SIP_S8bit		*pNewViaBranch = Sdf_co_null;
	SIP_S8bit		*pExistingViaBranch = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_fn_uaIsCancelMatching");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */

	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaIsCancelMatching():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_false;
	}

	if ((pNewCallObj) == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIsCancelMatching(): "
			"pNewCallObj passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_false;
	}	

	if (pExistingCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIsCancelMatching(): "
			"pExisitingCallObj passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_false;
	}	
#endif	/* End of param validation ifdef */

	pNewCommonInfo = pNewCallObj->pCommonInfo;
	pExistingCommonInfo = pExistingCallObj->pCommonInfo;

	pNewSipMsg = pNewCallObj->pUasTransaction->pSipMsg;
	pExistingSipMsg = pExistingCallObj->pUasTransaction->pSipMsg;

	pNewViaBranch = pNewCommonInfo->pKey->pRemoteViaBranch;
	pExistingViaBranch = pExistingCommonInfo->pKey->pRemoteViaBranch;

	pErr->errCode = Sdf_en_noUaError;

	if( (pNewViaBranch != Sdf_co_null ) && \
		(Sdf_mc_strncmp(pNewViaBranch, "z9hG4bK", 7) == 0))
	{
		SipHeader	dNewViaHdr, dExistingViaHdr;

		Sdf_ty_s8bit 	*pNewViaSentBy = Sdf_co_null;
		Sdf_ty_s8bit 	*pExistingViaSentBy = Sdf_co_null;

		if(pExistingViaBranch == Sdf_co_null)
		{
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_fn_uaIsCancelMatching");
			return Sdf_co_false;
		}

		if( Sdf_mc_strcmp(pNewViaBranch, pExistingViaBranch) != 0)
		{
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_fn_uaIsCancelMatching");
			return Sdf_co_false;
		}

		/* Compare sent-by value in top Via */
		if ((sip_getHeader(pNewSipMsg, SipHdrTypeVia, \
			&dNewViaHdr, (SipError *)&(pErr->stkErrCode))) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaIsCancelMatching( ): "
				"Failed to get Via header from message)",pErr);
#endif		
			pErr->errCode = pErr->stkErrCode;
			return Sdf_co_false;
		}

		if ((sip_getHeader(pExistingSipMsg, SipHdrTypeVia, \
			&dExistingViaHdr, (SipError *)&(pErr->stkErrCode))) == SipFail)
		{
			sip_freeSipHeader(&dNewViaHdr);
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaIsCancelMatching( ): "
				"Failed to get Via header from message)",pErr);
#endif		
			pErr->errCode = pErr->stkErrCode;
			return Sdf_co_false;
		}

		if (sip_getSentByFromViaHdr(&dNewViaHdr, &pNewViaSentBy, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&dNewViaHdr);
			sip_freeSipHeader(&dExistingViaHdr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaIsCancelMatching(): "
					"Failed to get sentby from Via header",pErr);
#endif			
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_false;
		}

		if (sip_getSentByFromViaHdr(&dExistingViaHdr, &pExistingViaSentBy, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&dNewViaHdr);
			sip_freeSipHeader(&dExistingViaHdr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaIsCancelMatching(): "
				"Failed to get sentby from Via header",pErr);
#endif			
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_false;
		}

		if (Sdf_mc_strcasecmp(pNewViaSentBy, pExistingViaSentBy) != 0)
		{
			sip_freeSipHeader(&dNewViaHdr);
			sip_freeSipHeader(&dExistingViaHdr);

			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_fn_uaIsCancelMatching");
			return Sdf_co_false;
		}

		sip_freeSipHeader(&dNewViaHdr);
		sip_freeSipHeader(&dExistingViaHdr);
	}
	else
	{
		Sdf_ty_s8bit		*pNewViaStr = Sdf_co_null;
		Sdf_ty_s8bit		*pExistingViaStr = Sdf_co_null;

		Sdf_ty_s8bit		*pTag1 = Sdf_co_null;
		Sdf_ty_s8bit		*pTag2 = Sdf_co_null;

		SipHeader		dNewCseqHeader, dExistingCseqHeader;
		Sdf_ty_u32bit		dNewCseqNo = 0, dExistingCseqNo = 0;

		SipAddrSpec		*pRequestAddrSpec = Sdf_co_null;
		SipReqLine		*pRequest = Sdf_co_null;

		/* compare Request-URI */
		if(sip_getReqLineFromSipReqMsg(pNewSipMsg, \
			&pRequest, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaIsCancelMatching( ):"
				"Failed to get Request Line from Sip Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_false;
		}

		if ( sip_getAddrSpecFromReqLine(pRequest,&pRequestAddrSpec, \
			(SipError *)&(pErr->stkErrCode)) == SipFail ) 
                {
			sip_freeSipReqLine(pRequest); 
#ifdef SDF_ERROR 
                        sdf_fn_setError(Sdf_en_minorErrors, \
       	                        Sdf_en_headerManipulationError,\
               	                (Sdf_ty_s8bit *) "sdf_fn_uaIsCancelMatching( ):"\
                       	        "Failed to getAddrSpec from  Request Line",pErr); 
#endif 
                        pErr->errCode=Sdf_en_headerManipulationError; 
       	                /* Free already accessed ReqLine */ 
               	        sip_freeSipReqLine(pRequest) ; 
                       	return Sdf_co_false; 
                 } 

        /* Changes for CSR: 1-5210208 */
		if (sdf_fn_uaCompareSipAddrSpec(pRequestAddrSpec, \
			pExistingCommonInfo->pNewRequestUri, \
			Sdf_co_true, Sdf_co_false, pErr) != 0)
        /* Changes for CSR: 1-5210208 */
		{
			sip_freeSipAddrSpec(pRequestAddrSpec); 
			sip_freeSipReqLine(pRequest);

			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_fn_uaIsCancelMatching");
			return Sdf_co_false;
		}

		sip_freeSipAddrSpec(pRequestAddrSpec); 
		sip_freeSipReqLine(pRequest);

		/* compare From tag */ 
		if (sip_getTagAtIndexFromFromHdr(pNewCommonInfo->pFrom, &pTag1, 0, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			if (sip_getTagAtIndexFromFromHdr(pExistingCommonInfo->pFrom, &pTag2, 0, \
				(SipError *)&(pErr->stkErrCode)) != SipFail)
			{
				sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
					"Exiting sdf_fn_uaIsCancelMatching");
				return Sdf_co_false;
			}
		}
		else
		{
			if (sip_getTagAtIndexFromFromHdr(pExistingCommonInfo->pFrom, &pTag2, 0, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
				sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
					"Exiting sdf_fn_uaIsCancelMatching");
				return Sdf_co_false;
			}
			else if (Sdf_mc_strcmp(pTag1, pTag2) != 0)
			{
				sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
					"Exiting sdf_fn_uaIsCancelMatching");
				return Sdf_co_false;
			}
        }
        /*Fix for SPR: 19999 (Merged CSR 1-7586127 : SPR-19988)*/
        /* CSR_1-7595231 Change Start */
        pTag1 = Sdf_co_null;
        pTag2 = Sdf_co_null;
        /* CSR_1-7595231 Change End */
        /* CANCEL should not have To tag */
		if (sip_getTagAtIndexFromToHdr(pNewCommonInfo->pTo, &pTag1, 0, \
			(SipError *)&(pErr->stkErrCode)) != SipFail)
		{
            /* CSR_1-7595231 Change Start */
            if(Sdf_co_null != pTag1)
            {
            /* CSR_1-7595231 Change End */            
                sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
                        "Exiting sdf_fn_uaIsCancelMatching");
                return Sdf_co_false;
            /* CSR_1-7595231 Change Start */
            }
            /* CSR_1-7595231 Change End */                
        }

		/* compare Call-ID */
		if (Sdf_mc_strcmp(pNewCommonInfo->pKey->pCallId, \
			pExistingCommonInfo->pKey->pCallId) != 0)
		{
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_fn_uaIsCancelMatching");
			return Sdf_co_false;
		}

		/* compare CSeq */
		if(sip_getHeader(pNewSipMsg, SipHdrTypeCseq, &dNewCseqHeader,\
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaIsCancelMatching( ): "
				"Failed to get Cseq header from message)",pErr);
#endif		
			pErr->errCode = pErr->stkErrCode;
			return Sdf_co_false;
		}

		(void)sip_getSeqNumFromCseqHdr(&dNewCseqHeader, &dNewCseqNo, \
				(SipError*)&(pErr->stkErrCode));

		sip_freeSipHeader(&dNewCseqHeader);

		if(sip_getHeader(pExistingSipMsg, SipHdrTypeCseq, &dExistingCseqHeader,\
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&dNewCseqHeader);
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaIsCancelMatching( ): "
				"Failed to get Cseq header from message)",pErr);
#endif		
			pErr->errCode = pErr->stkErrCode;
			return Sdf_co_false;
		}

		(void)sip_getSeqNumFromCseqHdr(&dExistingCseqHeader, &dExistingCseqNo, \
				(SipError*)&(pErr->stkErrCode));

		sip_freeSipHeader(&dExistingCseqHeader);

		if (dNewCseqNo != dExistingCseqNo)
		{
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_fn_uaIsCancelMatching");
			return Sdf_co_false;
		}

		/* compare top Via header field */
		 (void)sip_getHeaderAsStringAtIndex(pNewSipMsg, \
			SipHdrTypeVia,  &pNewViaStr, 0, \
			(SipError*)&(pErr->stkErrCode));

		 (void)sip_getHeaderAsStringAtIndex(pExistingSipMsg, \
			SipHdrTypeVia,  &pExistingViaStr, 0, \
			(SipError*)&(pErr->stkErrCode));

		if (Sdf_mc_strcasecmp(pNewViaStr, pExistingViaStr) != 0)
		{
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&( \
				pNewViaStr), pErr);

			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&( \
				pExistingViaStr), pErr);

			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_fn_uaIsCancelMatching");
			return Sdf_co_false;
		}

		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&( \
			pNewViaStr), pErr);

		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&( \
			pExistingViaStr), pErr);
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaIsCancelMatching");
	return Sdf_co_true;
}
/*****************************************************************************
** FUNCTION: sdf_fn_uaSipDecode
**
** DESCRIPTION: This function includes the code to create context and invoking
**				SIP stack function to decode the message.
*****************************************************************************/
Sdf_ty_matchMessageType sdf_fn_uaSipDecode
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObj,
	 Sdf_ty_s8bit 		*pBuffer,
	 SipMessage 		**ppMsg,
	 SipOptions  		*pOptions,
	 Sdf_ty_u32bit 		dLength,
	 Sdf_ty_s8bit 		**ppNextBuffer,
	 Sdf_st_eventContext *pEventContext,
	 Sdf_st_error *pErr)
#else
	(pCallObj, pBuffer, ppMsg, pOptions, dLength, ppNextBuffer, \
	 pEventContext, pErr)
	 Sdf_st_callObject	*pCallObj;
	 Sdf_ty_s8bit *pBuffer;
	 SipMessage	   **ppMsg;
	 SipOptions    *pOptions;
	 Sdf_ty_u32bit dLength;
	 Sdf_ty_s8bit **ppNextBuffer;
	 Sdf_st_eventContext *pEventContext;
	 Sdf_st_error *pErr;
#endif
{
	en_SipMessageType dReqRespType = SipMessageRequest;
	Sdf_st_transaction	*pTransaction = Sdf_co_null;
	Sdf_ty_transactionType transType;
	Sdf_ty_matchMessageType dType = Sdf_en_success;
    /*ARICENT CSR_15006112*/
    Sdf_ty_bool dIsMsgIncomplete = Sdf_co_false;
   /*ARICENT CSR_15006112 */

#ifdef SDF_TXN
	SipTxnContext			*pSipTxnContext	= Sdf_co_null;
	SipTxnKey				*pSipTxnKey		= Sdf_co_null;
	Sdf_st_txnContextData	*pTxnContextData = Sdf_co_null;
	en_SipTxnDecodeResult	dTxnResult		= SipTxnNonIgnorable;
#endif


#ifndef SDF_TXN
	/*
	 * NON TRANSACTION LAYER HANDLING
	 * ------------------------------
	 * Invoke the core stack parser to decode the message. The parsed
	 * SipMessage structure is set into pMsg.
	 */
	if(sip_decodeMessage(pBuffer, ppMsg, pOptions,\
		dLength, ppNextBuffer, (SipEventContext *)pEventContext, \
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_parserError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSipDecode(): "
			"Decoding the SIP message failed",pErr);
#endif
		if((ppMsg != SIP_NULL)&&(*ppMsg != SIP_NULL))
		{
			sip_freeSipMessage(*ppMsg);
		}
		
		pErr->errCode=Sdf_en_parserError;
		return Sdf_en_fail;
	}

#else /* Else for ifndef SDF_TXN */

	/* 
	 * TXN LAYER HANDLING
	 * ------------------
	 * Store the user-passed options and set the Transaction type for the
	 * proper stack handling.
	 */
	if (sip_txn_initSipTxnContext(&pSipTxnContext, \
		(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSipDecode(): "
			"Failed to initialize the transaction context structure.",pErr);
#endif
		return Sdf_en_fail;
	}
	pSipTxnContext->txnOption.dOption		= *pOptions;
	pSipTxnContext->dTxnType				=  SipUATypeTxn;

	/*
	 * Store a reference to the call object. This is needed for issuing the
	 * sdf_cbk_uaFetchSipTransaction() callback.
	 */
	if (sdf_ivk_uaInitTxnContextData(&pTxnContextData, pErr) == Sdf_co_fail)
	{
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid *)&pSipTxnContext, pErr);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSipDecode(): "
			"Failed to initialize the transaction context data.", pErr);
#endif
		pErr->errCode = Sdf_en_noMemoryError;
		return Sdf_en_fail;
	}

	HSS_LOCKEDINCREF(pCallObj->dRefCount);
	pTxnContextData->pRefCallObject		= pCallObj;
	HSS_LOCKEDINCREF(pEventContext->dRefCount);
	pTxnContextData->pEventContext 		= pEventContext;
	pTxnContextData->pApplicationData	= pEventContext->pData;
	pSipTxnContext->pEventContext		= (SipEventContext *)pEventContext;
	pSipTxnContext->timeoutValues		= pEventContext->dTimeoutValues;

	/* 
	 * NOTE:
	 * We are over-writing the data part of the user-provided data in
	 * the event context. We shall restore this immediately after decode.
	 */
	pEventContext->pData		= (Sdf_ty_pvoid) pTxnContextData;
	
	/*
	 * Call the parser now.
	 */
	if (sip_txn_decodeMessage(pBuffer, ppMsg, pSipTxnContext,\
		dLength, ppNextBuffer, &dTxnResult, &pSipTxnKey, \
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		Sdf_ty_matchMessageType dFlag = Sdf_en_fail;
                /* Enhancement Change Start */
		Sdf_ty_bool dIsHdrIncomplete = Sdf_co_false;
		/* Enhancement Change End */
    
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_parserError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSipDecode(): "
			"Decoding the SIP message failed",pErr);
#endif
       if((pErr->stkErrCode == Sdf_en_incomplete) ||
               ( pErr->stkErrCode == Sdf_en_sipIncomplete))
       {
            /* Enhancement Change Start */
            if(pErr->stkErrCode == Sdf_en_incomplete)
                /* Enhancment Change End */
                /* CSR_1-5006112 Change Start */
                dIsMsgIncomplete = Sdf_co_true;
            /* CSR_1-5006112 Change End */
            /* Enhancement Change Start */
            else
                dIsHdrIncomplete = Sdf_co_true;
            /* Enhancement Change End */
            if (sip_getMessageType(*ppMsg, &dReqRespType, (SipError*)\
         &(pErr->stkErrCode)) ==  SipFail) 
	{    
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSipDecode(): "
			"Failed to get type of SIP Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
            /*CSR No.:1-5139236 Enhancement Change Start */
            if(dIsMsgIncomplete == Sdf_co_true)
            {
                pErr->stkErrCode = Sdf_en_incomplete;
            }
            /*CSR No.:1-5139236 Enhancement Change End*/
            return Sdf_en_fail;
        }
		if ((dReqRespType == SipMessageRequest) &&\
		 	(pEventContext->dProtocol == Sdf_en_protoUdp))
		{								
			dFlag = Sdf_en_success;			
	 		(void)sip_txn_handleTxn(*ppMsg,pSipTxnContext,&dTxnResult,\
				&pSipTxnKey,(SipError*)&(pErr->stkErrCode));
			if(pCallObj->pUasTransaction->pTxnKey != Sdf_co_null)
			   sip_freeSipTxnKey(pCallObj->pUasTransaction->pTxnKey);
			pCallObj->pUasTransaction->pTxnKey = pSipTxnKey;
	 	}
	 }

		/*
		 * Restore the data part of the event context.
		 */
		pEventContext->pData = pTxnContextData->pApplicationData;

		/*
		 * Free the transaction context data created for the parsing.
		 */
		pTxnContextData->pApplicationData = Sdf_co_null;
		(void)sdf_ivk_uaFreeTxnContextData(pTxnContextData);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&pSipTxnContext, pErr);

/*CSR No.:1-5139236 Enhancement Change Start */
#if 0
This code is already written above few lines.
/*CSR No.:1-5139236 Enhancement Change End*/
		if (sip_getMessageType(*ppMsg, &dReqRespType, &error)\
		==  SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSipDecode(): "
			"Failed to get type of SIP Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
            if(dIsMsgIncomplete == Sdf_co_true)
            {
                pErr->stkErrCode = Sdf_en_incomplete;
            }
			return Sdf_en_fail;
		}
/*CSR No.:1-5139236 Enhancement Change Start */
#endif
/*CSR No.:1-5139236 Enhancement Change End*/

        /*ARICENT CSR_15006112 */
        /* Enhancement Change Start */
        /* Freeing of SIP Message if a fragmented message has come over TCP */
        if(((dIsMsgIncomplete == Sdf_co_true) ||(dIsHdrIncomplete == Sdf_co_true) )&& \
            (pEventContext->dProtocol != Sdf_en_protoUdp))
            /* Enhancement Change End */		
        {
                sip_freeSipMessage(*ppMsg);
	    }
        /* For all the other 3 cases namely
		1) Incomplete header in SIP Message Over UDP
		2) Complete Message Over UDP
		3) Complete Message Over TCP
		control comes here */
        else if(dReqRespType == SipMessageRequest)
	    {
		    pCallObj->pUasTransaction->pSipMsg =*ppMsg;
	    }
        else
        {
            pCallObj->pUacTransaction->pSipMsg =*ppMsg;
        }

        if(dIsMsgIncomplete == Sdf_co_true)
			pErr->stkErrCode = Sdf_en_incomplete;
        /*ARICENT CSR_15006112 */
        /* Enhancement Change Start */
        if(dIsHdrIncomplete == Sdf_co_true)
        {
            pErr->stkErrCode = Sdf_en_sipIncomplete;
            (void)sdf_memfree(Sdf_mc_callHandlerMemId, \
                 (Sdf_ty_pvoid *)&pSipTxnContext, pErr);
        }
        /* Enhancement Change End */           
	pErr->errCode=Sdf_en_parserError;
	return Sdf_en_fail;
	}
	
	/* NOTE (contd from above): The event context data is restored here.  */
	pEventContext->pData = pTxnContextData->pApplicationData;
	
	/* Free the transaction context data.  */
	pTxnContextData->pApplicationData = Sdf_co_null;
	(void)sdf_ivk_uaFreeTxnContextData(pTxnContextData);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pSipTxnContext, pErr);

#endif /* End - ifdef SDF_TXN */
		
	if (sip_getMessageType(*ppMsg, &dReqRespType, (SipError*)\
			&(pErr->stkErrCode)) ==  SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSipDecode(): "
			"Failed to get type of SIP Message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
            if((dIsMsgIncomplete == Sdf_co_true) &&  \
                (pEventContext->dProtocol != Sdf_en_protoUdp))
             {
	        sip_freeSipMessage(*ppMsg);
	     }
            else if(dReqRespType == SipMessageRequest)
	          {
	 	     pCallObj->pUasTransaction->pSipMsg =*ppMsg;
	          }
            else
               {
	           pCallObj->pUacTransaction->pSipMsg =*ppMsg;
               }

            if(dIsMsgIncomplete == Sdf_co_true)
              pErr->stkErrCode = Sdf_en_incomplete;       
              return Sdf_en_fail;
	}

	/* If any SipMessage present in the Transaction Structures
	 * free them before overwriting them */
	if( pCallObj->pUacTransaction->pSipMsg != Sdf_co_null )
	{
		sip_freeSipMessage(pCallObj->pUacTransaction->pSipMsg);
		pCallObj->pUacTransaction->pSipMsg = Sdf_co_null;
	}

	if( pCallObj->pUasTransaction->pSipMsg != Sdf_co_null )
	{
		sip_freeSipMessage(pCallObj->pUasTransaction->pSipMsg);
		pCallObj->pUasTransaction->pSipMsg = Sdf_co_null;
	}

	if (dReqRespType == SipMessageRequest)
		transType = Sdf_en_uasTransaction;
	else
		transType = Sdf_en_uacTransaction;


	if (sdf_ivk_uaGetTransactionFromCallObject(pCallObj, transType, \
		&pTransaction, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSipDecode(): "
			"Failed to get UAS structure from call object",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
        if(dIsMsgIncomplete == Sdf_co_true)
        {
            pErr->stkErrCode = Sdf_en_incomplete;
        }
		return Sdf_en_fail;
	}
#ifdef SDF_TXN
	sip_freeSipTxnKey(pTransaction->pTxnKey);
	pTransaction->pTxnKey = pSipTxnKey;
#endif


#ifdef SDF_TXN
	switch (dTxnResult)
	{
		case SipTxnNonIgnorable:
		{ 
			/* Success case: new transaction */
			pTransaction->dSocket.dSockfd = pEventContext->pTranspAddr->dSockFd;
			pTransaction->dSocket.dProtocol = pEventContext->dProtocol;
			break;
		}
		case SipTxnIgnorable:
		{
			/* Ignorable (recd message is a remote retransmission) */
			dType = Sdf_en_remoteRetransmission;
			break;
		}
		case SipTxnStrayMessage:
		case SipTxnStrayPrack:
		{
			/*
			 * Stray response recd or a PRACK received for which there is
			 * no matching RPR transaction.
			 */
			dType = Sdf_en_strayMessage;
			break;
		}
		case SipTxnRetrans:
		case SipTxnConfirmnNeeded:
		case SipTxnQueued:
		{
			/* 
			 * Queued or confirmation needed are not returned for UA Type
			 * transactions. These can be ignored in the case-switch.
			 * Same for SipTxnRetrans: This is for stateless proxy application 
			 * using the core stack.
			 */
			break;
		}
	}
#endif
	
	pTransaction->pSipMsg = *ppMsg;
	(void)sdf_ivk_uaFreeTransaction(pTransaction);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_fn_uaSipDecode");
	return dType;
}

#ifdef SIP_BADMESSAGE_PARSING
/*****************************************************************************
** FUNCTION: sdf_fn_uaHandleBadHeaders
**
** DESCRIPTION: This function includes the code to handle the bad headers.
*****************************************************************************/
Sdf_ty_matchMessageType sdf_fn_uaHandleBadHeaders
#ifdef ANSI_PROTO
	( SipMessage  	*pMsg,
	  Sdf_ty_slist	*pslNonMandatoryBadHeaders,
	  Sdf_ty_bool	*pSdpLineError,
	  Sdf_st_error *pErr)
#else
	(pMsg, pNonMandatoryError, pSdpLineError, pErr)
	 SipMessage		*pMsg;
	 Sdf_ty_slist	*pslNonMandatoryBadHeaders,
	 Sdf_ty_bool	*pSdpLineError;
	 Sdf_st_error 	*pErr;
#endif
{
	Sdf_ty_u32bit dCount = 0, dIndex = 0;	
	Sdf_ty_u32bit 	dMsgBodyCount	= 0;
	en_SipMsgBodyType	dMsgBodyType;
#ifdef SDF_LINT
	if(pMsg == Sdf_co_null)
		return Sdf_en_fail;
#endif

	if(sip_getBadHeaderCount(pMsg, &dCount, \
		(SipError*)	&(pErr->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,\
			Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaHandleBadHeaders(): "
			"Failed to get the count of the Bad header",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}
	for(dIndex=0;dIndex<dCount;dIndex++)
	{
		SipBadHeader	*pBadHeader = Sdf_co_null;
		SIP_S8bit 		*pStr = Sdf_co_null;

		if(sip_getBadHeaderAtIndex(pMsg, &pBadHeader,\
			dIndex, (SipError*)&(pErr->stkErrCode))==SipFail) 
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleBadHeaders(): "
				"Failed to get the Bad header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
		if(sip_getNameFromBadHdr(pBadHeader, &pStr,\
			(SipError*)&(pErr->stkErrCode))==SipFail)
		{
			sip_freeSipBadHeader(pBadHeader);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleBadHeaders(): "
				"Failed to get the Bad header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
		if((Sdf_mc_strcasecmp(pStr,"From")==0) || \
			(Sdf_mc_strcasecmp(pStr,"To")==0) || \
			(Sdf_mc_strcasecmp(pStr,"Via")==0) || \
			(Sdf_mc_strcasecmp(pStr,"Call-Id")==0) || \
			(Sdf_mc_strcasecmp(pStr,"Contact")==0) || \
			(Sdf_mc_strcasecmp(pStr,"Cseq")==0) || \
			(Sdf_mc_strcasecmp(pStr,"F")==0) || \
			(Sdf_mc_strcasecmp(pStr,"T")==0) || \
			(Sdf_mc_strcasecmp(pStr,"V")==0) || \
			(Sdf_mc_strcasecmp(pStr,"i")==0) || \
			(Sdf_mc_strcasecmp(pStr,"m")==0))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_basicHeaderError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleBadHeaders(): "
				"Error in Basic header",pErr);
#endif
			sip_freeSipBadHeader(pBadHeader);
			pErr->errCode=Sdf_en_basicHeaderError;
			return Sdf_en_fail;
		}
		else
		{
			/*Add it to the List of Non-Mandatory BadHeaders*/
			if(sdf_listAppend(pslNonMandatoryBadHeaders,pBadHeader,\
					pErr)==Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_basicHeaderError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaHandleBadHeaders(): "
					"Failed to append to Bad header List",pErr);
#endif
				return	Sdf_en_fail;
			}
			continue;
		}
	}
	if(sip_listSizeOf(&(pMsg->slMessageBody),&dMsgBodyCount,\
			(SipError*)&(pErr->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,\
			Sdf_en_msgBodyError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaHandleBadHeaders(): "
			"Failed to get size of list ",pErr);
#endif		
		pErr->errCode=Sdf_en_msgBodyError;
		return Sdf_en_fail;
	}

	for (dIndex = 0; dIndex < dMsgBodyCount; dIndex++)
	{
		SipMsgBody *pTempMsgBody=Sdf_co_null;

		if (sip_listGetAt(&(pMsg->slMessageBody), dIndex, \
			(Sdf_ty_pvoid *) &pTempMsgBody,\
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaHandleBadHeaders(): "
				"Failed to get Message Body from MsgBody List",pErr);
#endif		 
			pErr->errCode=Sdf_en_msgBodyError ;
			return Sdf_en_fail;
		}
		if (sip_getMsgBodyType(pTempMsgBody, &dMsgBodyType, (SipError*)\
			&(pErr->stkErrCode))  == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaHandleBadHeaders(): "
				"Failed to get Message Body Type",pErr);
#endif		 
			pErr->errCode=Sdf_en_msgBodyError;
			return Sdf_en_fail;
		}

		if (dMsgBodyType == SipSdpBody)
		{
			Sdf_ty_u32bit dMediaCount=0,dLIndex=0;
			SdpMedia 	*pMedia=Sdf_co_null;
			
			Sdf_ty_u32bit dIncorrectCount=0;

			if (sip_listSizeOf(\
				&(pTempMsgBody->u.pSdpMessage->slIncorrectLines), \
				&dIncorrectCount, (SipError*)&(pErr->stkErrCode)) \
				== SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_msgBodyError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaHandleBadHeaders(): "
				"Failed to get Size of Incorrect Line From Sdp Message", \
				pErr);
#endif
				pErr->errCode=Sdf_en_msgBodyError;
				return Sdf_en_fail;
			}
			if(dIncorrectCount > 0 )
			{				
				*pSdpLineError=Sdf_co_true;
				break;
			}
			if(sdp_getMediaCount(pTempMsgBody->u.pSdpMessage,\
				&dMediaCount,(SipError*)\
				&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_msgBodyError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaHandleBadHeaders(): "
					"Failed to get Media Count From Sdp Body ",pErr);
#endif
				pErr->errCode=Sdf_en_msgBodyError;
				return Sdf_en_fail;
			}
			for(dLIndex=0;dLIndex < dMediaCount; dLIndex++)
			{
				Sdf_ty_u32bit	dIncorrectCountMedia=0;
				if(sdp_getMediaAtIndex(pTempMsgBody->u.pSdpMessage,\
					&pMedia,dLIndex,(SipError*)\
					&(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,\
						Sdf_en_msgBodyError, \
						(Sdf_ty_s8bit *)"sdf_fn_uaHandleBadHeaders(): "
						"Failed to get Media From SDP Body",pErr);
#endif
					pErr->errCode=Sdf_en_msgBodyError;
					return Sdf_en_fail;
				}
				if (sip_listSizeOf(&(pMedia->slIncorrectLines),\
						&dIncorrectCountMedia,(SipError*)\
						&(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,\
						Sdf_en_msgBodyError, \
						(Sdf_ty_s8bit *)"sdf_fn_uaHandleBadHeaders(): "
						"Failed to get size of"
						"Incorrect header From Media", pErr);
#endif
					pErr->errCode=Sdf_en_msgBodyError;
					return Sdf_en_fail;
				}
				sip_freeSdpMedia(pMedia);
				if(dIncorrectCountMedia > 0 )
				{				
					*pSdpLineError=Sdf_co_true;
					break;
				}
			} /* Media Count Loop Ends */
		} /* End of if Sdp */
	} /* End of MsgBody Count */

	if(sip_checkMandatory(pMsg)==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_parserError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaHandleBadHeaders(): "
			"Mandatory header missing in the SIP message",pErr);
#endif
		pErr->errCode=Sdf_en_parserError;
		return Sdf_en_fail;
	}
	return Sdf_en_success;
}
#endif /* SIP_BADMESSAGE_PARSING */


/*****************************************************************************
** FUNCTION: sdf_fn_uaExtractKeyInfo
**
** DESCRIPTION: This function extracts the Basic Key information from the 
**				Incoming message and stores into the Call Object's key field
*****************************************************************************/
Sdf_ty_matchMessageType sdf_fn_uaExtractKeyInfo
#ifdef ANSI_PROTO
	( Sdf_st_callObject	*pCallObj, 
	  SipMessage  	*pMsg,
	  en_SipMessageType dReqRespType,
	  Sdf_st_error *pErr)
#else
	(pCallObj, pMsg, dReqRespType, pErr)
	 Sdf_st_callObject	*pCallObj;
	 SipMessage		*pMsg;
	 en_SipMessageType dReqRespType;
	 Sdf_st_error 	*pErr;
#endif
{
	SipHeader 			dTempHeader;
	Sdf_ty_s8bit 		*callId	= Sdf_co_null;
	Sdf_st_commonInfo 	*pCommonInfo	= Sdf_co_null;
	Sdf_ty_s8bit		*pViaBranch		= Sdf_co_null;
	en_HeaderType		dHdrType = SipHdrTypeAny;

	if (sip_getHeader(pMsg, SipHdrTypeCallId, &dTempHeader,\
		(SipError*)	&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaExtractKeyInfo(): "
			"Failed to get Callid header from SIP message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}

	if(sip_getValueFromCallIdHdr(&dTempHeader, &callId, \
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaExtractKeyInfo(): "
			"Failed to get value from Call-Id header",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		sip_freeSipHeader(&dTempHeader);
		return Sdf_en_fail;
	}

	sip_freeSipHeader(&dTempHeader);

	if (sdf_ivk_uaGetCommonInfoFromCallObject(pCallObj, &pCommonInfo, \
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaExtractKeyInfo(): "
			"Getting CommonInfo from CallObject failed",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_en_fail;
	}

	pCommonInfo->pCallid = Sdf_mc_strdupCallHandler(callId);

	if( dReqRespType == SipMessageRequest )
		dHdrType = SipHdrTypeTo;
	else
		dHdrType = SipHdrTypeFrom;

	if (sip_getHeader(pMsg, dHdrType, pCommonInfo->pKey->pLocalAddr,  \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaExtractKeyInfo(): "
			"Failed to get From header from SipMessage",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}

	if( dReqRespType == SipMessageRequest )
		dHdrType = SipHdrTypeFrom;
	else
		dHdrType = SipHdrTypeTo;

	if (sip_getHeader(pMsg, dHdrType, pCommonInfo->pKey->pRemoteAddr, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaExtractKeyInfo(): "
			"Failed to get To header from SipMessage",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}
	pCommonInfo->pKey->pCallId =Sdf_mc_strdupCallHandler(callId);

	/* This addition was done as part of fix to CERT test case 291. 
	 * When CANCEL is received for a failed INVITE, we are trying to access
	 * pFrom & pTo information from pCommonInfo (which is not set until
	 * sdf_fn_uaUpdateCallConstants is called) */
	sip_freeSipHeader(pCallObj->pCommonInfo->pFrom);
	if (sip_getHeader(pMsg, SipHdrTypeFrom, pCommonInfo->pFrom, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaExtractKeyInfo(): "
			"Failed to get From header from SipMessage",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}
	sip_freeSipHeader(pCallObj->pCommonInfo->pTo);
	if (sip_getHeader(pMsg, SipHdrTypeTo, pCommonInfo->pTo, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaExtractKeyInfo(): "
			"Failed to get From header from SipMessage",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}

	if(sip_getHeader(pMsg, SipHdrTypeVia, &dTempHeader,\
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaExtractKeyInfo(): "
			"Failed to get Via header from SIP message",pErr);
#endif
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}

	if(sdf_fn_GetBranchFromViaHdr(&dTempHeader,	&pViaBranch, \
			pErr) != Sdf_co_success)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaExtractKeyInfo(): "
			"Failed to get Via Branch from Via header",pErr);
#endif
		pViaBranch = Sdf_co_null;
	}

	if( pViaBranch != Sdf_co_null )
	{
		if( dReqRespType == SipMessageResponse )
			pCommonInfo->pKey->pLocalViaBranch = \
							Sdf_mc_strdupAccessor(pViaBranch);
		else
			pCommonInfo->pKey->pRemoteViaBranch = \
							Sdf_mc_strdupAccessor(pViaBranch);
	}
	sip_freeSipHeader(&dTempHeader);
	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
	return Sdf_en_success;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaCorrectCseq
**
** DESCRIPTION: If the method present in Cseq is different from that present in
**				request URI, then correct the method in Cseq.
*****************************************************************************/
Sdf_ty_matchMessageType sdf_fn_uaCorrectCseq
#ifdef ANSI_PROTO
	( Sdf_ty_s8bit *pMethod, 
	  SipMessage  	*pMsg,
	  Sdf_st_error *pErr)
#else
	(pMethod, pMsg, pErr)
	 Sdf_ty_s8bit	*pMethod;
	 SipMessage		*pMsg;
	 Sdf_st_error 	*pErr;
#endif
{
	Sdf_ty_s8bit *pCseqMethod = Sdf_co_null;
	SipHeader header;
	
	/* 
	 * Correct the Cseq method if it is different from 
	 * the Req-URI method
	 */
	
	if(sip_getHeader(pMsg, SipHdrTypeCseq,&header,\
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCorrectCseq(): "
			"Failed to get Cseq header from SIP message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}
	if(sip_getMethodFromCseqHdr(&header,&pCseqMethod,(SipError*)\
		&(pErr->stkErrCode)) == SipFail)
	{
		sip_freeSipHeader(&header);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,\
			Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCorrectCseq(): "
			"Failed to get method from Cseq header",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}
	if (Sdf_mc_strcmp(pMethod, pCseqMethod) != 0)
	{
		Sdf_ty_s8bit	*pTmpMethod = Sdf_co_null;

		pTmpMethod = Sdf_mc_strdupCallHandler(pMethod);

		if (sip_setMethodInCseqHdr(&header, pTmpMethod, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&header);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectCseq(): "
				"Failed to set method in Cseq header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
	}
	sip_freeSipHeader(&header);
	return Sdf_en_success;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaValidateMessage
**
** DESCRIPTION: After decoding the message, do some basic validations on 
**				the decoded message. This includes the validating the Version,
**				Address Type of To address etc.
*****************************************************************************/
Sdf_ty_matchMessageType sdf_fn_uaValidateMessage
#ifdef ANSI_PROTO
	( SipMessage  	*pMsg,
	  en_SipMessageType dReqRespType,
	  Sdf_ty_messageType dMethodType,
	  Sdf_st_error *pErr)
#else
	(pMsg, dReqRespType, dMethodType, pErr)
	SipMessage		*pMsg;
	en_SipMessageType dReqRespType;
	Sdf_ty_messageType dMethodType;
	Sdf_st_error 	*pErr;
#endif
{
	SipHeader		dTempHeader;
	SipAddrSpec		*pAddrSpec 		= Sdf_co_null;
	Sdf_ty_s8bit 	*pSipVersion	= Sdf_co_null;
	SipStatusLine	*pStatusLine	= Sdf_co_null;
	Sdf_ty_u32bit	dViaHdrCount	= 0;
	Sdf_ty_u32bit 	dHdrCount	= 0;
    Sdf_ty_u32bit 	dValidUrlCount	= 0;
	Sdf_ty_u32bit dStatusCode=0;
	Sdf_ty_u32bit 	dIndex		= 0;
	Sdf_ty_bool		dIsTelUrl = Sdf_co_false;
	Sdf_ty_bool		dIsImUrl  =	Sdf_co_false;
	Sdf_ty_bool		dIsPresUrl  =	Sdf_co_false;
    Sdf_ty_bool		dIsUnknownUrl = Sdf_co_false;

	if (dReqRespType == SipMessageRequest)
	{
		if (sip_getHeader(pMsg, SipHdrTypeFrom, &dTempHeader, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get From header from SipMessage",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
		if (sip_getAddrSpecFromFromHdr(&dTempHeader, &pAddrSpec, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get AddrSpec from From header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}

/* As per the B2B requirement when To header URI scheme is other than that of sip/sips/tel uatk is giving a decode failure(Ex. To: <http:hssua@10.203.225.197:37710> ). So to solve this problem following code is not executed for B2B . According to rfc 3261 The To header field MAY contain a SIP or SIPS URI, but it may also make use of other URI schemes (the tel URL (RFC 2806 [9]), for example). 
*/
#ifndef SDF_UAB2B 
        
#ifdef SDF_TLS
	    if((pAddrSpec->dType != SipAddrSipUri ) &&
			    (pAddrSpec->dType != SipAddrSipSUri ))
#else
	    if(pAddrSpec->dType != SipAddrSipUri )
#endif
		{
#ifdef SDF_TEL
			if(sip_isTelUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
			   	==SipSuccess)
				dIsTelUrl = Sdf_co_true;
#endif
#ifdef SDF_IM
			if( dIsTelUrl != Sdf_co_true )
			{
				if(sip_isImUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
				   	==SipSuccess)
					dIsImUrl = Sdf_co_true;
			}
#endif
#ifdef SDF_PRES
			if((dIsTelUrl != Sdf_co_true)&&(dIsImUrl != Sdf_co_true))
			{
				if(sip_isPresUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
				   	==SipSuccess)
					dIsPresUrl = Sdf_co_true;
			}
#endif

			if((dIsTelUrl != Sdf_co_true) && (dIsImUrl != Sdf_co_true)&&
                (dIsPresUrl != Sdf_co_true))
			{
				sip_freeSipHeader(&dTempHeader);
				sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_notImplemented, \
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
					"From header contains Unsupported URL Scheme",pErr);
#endif
				pErr->errCode=Sdf_en_notImplemented;
				return Sdf_en_fail;
			}
		}
#endif /* End: when B2B flag is enabled then validation of TO header with URI scheems tel/pres/http etc are skipeed*/
        
		sip_freeSipHeader(&dTempHeader);
		sip_freeSipAddrSpec(pAddrSpec);

		/* Check whether the Uri format in To header is supported */
		if (sip_getHeader(pMsg, SipHdrTypeTo, &dTempHeader, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get To header from SipMessage",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
		if (sip_getAddrSpecFromToHdr(&dTempHeader, &pAddrSpec, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get AddrSpec from To header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
#ifdef SDF_TLS
	    if((pAddrSpec->dType != SipAddrSipUri ) &&
			    (pAddrSpec->dType != SipAddrSipSUri ))
#else
	    if(pAddrSpec->dType != SipAddrSipUri )
#endif
		{
#ifdef SDF_TEL
			if(sip_isTelUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
			   	==SipSuccess)
				dIsTelUrl = Sdf_co_true;
#endif
#ifdef SDF_IM
			if( dIsTelUrl != Sdf_co_true )
			{
				if(sip_isImUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
				   	==SipSuccess)
					dIsImUrl = Sdf_co_true;
			}
#endif
#ifdef SDF_PRES
			if((dIsTelUrl != Sdf_co_true)&&(dIsImUrl != Sdf_co_true))
			{
				if(sip_isPresUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
				   	==SipSuccess)
					dIsPresUrl = Sdf_co_true;
			}
#endif
			if((dIsTelUrl != Sdf_co_true) && (dIsImUrl != Sdf_co_true)&&
                (dIsPresUrl != Sdf_co_true))
			{
				sip_freeSipHeader(&dTempHeader);
				sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_notImplemented, \
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
					"To header contains Unsupported URL Scheme",pErr);
#endif
				pErr->errCode=Sdf_en_notImplemented;
				return Sdf_en_fail;
			}
		}
		sip_freeSipHeader(&dTempHeader);
		sip_freeSipAddrSpec(pAddrSpec);

#ifdef SDF_REFER
		/* Check if Refer-To header is present in REFER request or not.
		 * If not, we need to return error code to application */
		if (dMethodType == Sdf_en_refer)
		{
			Sdf_ty_messageValidity dValidity=Sdf_en_invalid;
			
			if (Sdf_co_fail == sdf_fn_uaValidateRefer(pMsg,&dValidity,pErr))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
					"sdf_fn_uaValidateRefer() failed", pErr);
#endif
				return Sdf_en_fail;
			}
			if (Sdf_en_valid != dValidity)
			{
				pErr->errCode = Sdf_en_noReferToHdrError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
					"Failed to Validate REFER request(Missing Refer-To)",pErr);
#endif
				return Sdf_en_fail;
			}
		}
		
		/* Check if any non-INVITE request has come with "Replaces:" header.
		 * If so, reject it with 400
		 * PS: ACK cannot be rejected */
		if ((dMethodType != Sdf_en_invite) && (dMethodType != Sdf_en_ack) &&\
				(Sdf_co_true == sdf_fn_uaIsHeaderPresent(pMsg, \
				SipHdrTypeReplaces,pErr)))
		{
			pErr->errCode = Sdf_en_replacesInNonInviteRequestError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
					"non-INVITE request contains Replaces header", pErr);
#endif
			return Sdf_en_fail;
		}
#endif
/***check for contacts in contact header for request messages except register***/
 		if(dMethodType != Sdf_en_register)
	  	{
	  
			dHdrCount = 0;
			/*w534*/ (void)sip_getHeaderCount(pMsg, SipHdrTypeContactAny, &dHdrCount,\
				(SipError*)&(pErr->stkErrCode));
			dValidUrlCount = dHdrCount;	
			for(dIndex = 0; dIndex < dHdrCount; dIndex++ )
			{
				if (sip_getHeaderAtIndex(pMsg, SipHdrTypeContactAny, \
					&dTempHeader, dIndex, (SipError*)&(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError, \
						(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
						"Failed to get Contact header at index from SipMessage",pErr);
#endif
					pErr->errCode=Sdf_en_headerManipulationError;
					return Sdf_en_fail;
				}

				/* 
				 * Check whether the contact header type is wildcard contact
				 * header 
				 */
				
				/* 
				 * If yes, the check whether it is a SipURL or not
				 * can't be done. Hence, continue.
					 */
				if (dTempHeader.dType != SipHdrTypeContactWildCard)
				{
					if (sip_getAddrSpecFromContactHdr(&dTempHeader, &pAddrSpec, \
						(SipError*)&(pErr->stkErrCode)) == SipFail)
					{
						sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_headerManipulationError, \
							(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
							"Failed to get AddrSpec from Contact header",pErr);
#endif
						pErr->errCode=Sdf_en_headerManipulationError;
						return Sdf_en_fail;
					}
#ifdef SDF_TLS
					if((pAddrSpec->dType != SipAddrSipUri ) &&
							(pAddrSpec->dType != SipAddrSipSUri ))
#else
					if(pAddrSpec->dType != SipAddrSipUri )
#endif
					{
#ifdef SDF_TEL
						if(sip_isTelUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
							==SipSuccess)
							dIsTelUrl = Sdf_co_true;
#endif
#ifdef SDF_IM
						if( dIsTelUrl != Sdf_co_true )
						{
							if(sip_isImUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
								==SipSuccess)
								dIsImUrl = Sdf_co_true;
						}
#endif
#ifdef SDF_PRES
                        if((dIsTelUrl != Sdf_co_true)&&(dIsImUrl != Sdf_co_true))
                        {
                            if(sip_isPresUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
                                ==SipSuccess)
                                dIsPresUrl = Sdf_co_true;
                        }
#endif
                    
						if((dIsTelUrl != Sdf_co_true) && (dIsImUrl != Sdf_co_true) &&(dIsPresUrl != Sdf_co_true))
						{
						
							dValidUrlCount = dValidUrlCount - 1;
							dIsUnknownUrl = Sdf_co_true;
						}
					}
					sip_freeSipAddrSpec(pAddrSpec);
				}
				else
				{
                     /* CSR_1-6665445 changes starts */
                    /* CSR 1-6099289 Changes Start */
                    if( dMethodType != Sdf_en_register )
                   /* CSR 1-6099289 Changes End */
                   /* CSR_1-6665445 changes ends */
                    {
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_basicHeaderError, \
							(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
							"WildCard Contact header in non-Register "
							"Request message. Parsing Failed",pErr);
#endif
						sip_freeSipHeader(&dTempHeader);
						pErr->errCode=Sdf_en_basicHeaderError;
						return Sdf_en_fail;
             /* CSR_1-6665445 changes starts */
            /* CSR_1-6099289 changes starts */
                    }
            /* CSR_1-6665445 changes ends */
            /* CSR_1-6099289 changes ends */
				}
				sip_freeSipHeader(&dTempHeader);
			 }/*--- end of for loop ----*/
			/*----check for atleast one valid url is there -----*/
			if((dValidUrlCount == 0 ) && (dIsUnknownUrl == Sdf_co_true ))
			{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_notImplemented, \
								(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
								"Contact header contains Unsupported URL Scheme",pErr);
#endif
				pErr->errCode=Sdf_en_notImplemented;
				return Sdf_en_fail;
			
			}
 		 }

        
        
	}	
	else if (dReqRespType == SipMessageResponse)
	{
		/*
		* Check if the version in Status Line is SIP/2.0. 
		*/
		if (sip_getStatusLine(pMsg, &pStatusLine, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get Status Line from SipMessage",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
		if (sip_getVersionFromStatusLine(pStatusLine, &pSipVersion, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaValidateMessage( ):"
				"Failed to get SIP Version from Status Line",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			sip_freeSipStatusLine(pStatusLine);
			return Sdf_en_fail;
		}

		if (Sdf_mc_strcasecmp(pSipVersion, "SIP/2.0") != 0)
		{
			sip_freeSipStatusLine(pStatusLine);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_versionNotSupported,\
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage( ): "
				"SIP Version not supported",pErr);
#endif
			pErr->errCode = Sdf_en_versionNotSupported;
			return Sdf_en_fail;
		}
        /*Fix done for CSR-1-6073699*/ 
        /*Populate the dStatusCode from status line*/
        if (sip_getStatusCodeNumFromStatusLine(pStatusLine, &dStatusCode, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipStatusLine(pStatusLine);
			sdf_fn_debug((Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage( ):"
					"Failed to get SIP statuscodenumber from Status Line");
			pErr->errCode=Sdf_en_headerManipulationError;

			return Sdf_en_fail;
		}

		sip_freeSipStatusLine(pStatusLine);

		/* Check that there is only one Via header in the response */
		/*w534*/ (void)sip_getHeaderCount(pMsg, SipHdrTypeVia, &dViaHdrCount,\
			(SipError*)&(pErr->stkErrCode));

		if (dViaHdrCount > 1)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_tooManyViaHdrsInResponse,\
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage( ): "
				"Too many Via headers in Response message",pErr);
#endif
			pErr->errCode=Sdf_en_tooManyViaHdrsInResponse;
			return Sdf_en_fail;
		}
        /***check for contacts in contact header for 2xx responses	***/
		if ((dStatusCode > 199 ) && (dStatusCode < 300 ))
		{
			if(dMethodType != Sdf_en_register)
			{
		  
				dHdrCount = 0;
			/*w534*/ (void)	sip_getHeaderCount(pMsg, SipHdrTypeContactAny, &dHdrCount,\
					(SipError*)&(pErr->stkErrCode));
				dValidUrlCount = dHdrCount;	
				for(dIndex = 0; dIndex < dHdrCount; dIndex++ )
				{
					if (sip_getHeaderAtIndex(pMsg, SipHdrTypeContactAny, \
						&dTempHeader, dIndex, (SipError*)&(pErr->stkErrCode)) == SipFail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_headerManipulationError, \
							(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
							"Failed to get Contact header at index from SipMessage",pErr);
#endif
						pErr->errCode=Sdf_en_headerManipulationError;
						return Sdf_en_fail;
					}

					/* 
					 * Check whether the contact header type is wildcard contact
					 * header 
					 */
					
					/* 
					 * If yes, the check whether it is a SipURL or not
					 * can't be done. Hence, continue.
						 */
					if (dTempHeader.dType != SipHdrTypeContactWildCard)
					{
						if (sip_getAddrSpecFromContactHdr(&dTempHeader, &pAddrSpec, \
							(SipError*)&(pErr->stkErrCode)) == SipFail)
						{
							sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors, \
								Sdf_en_headerManipulationError, \
								(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
								"Failed to get AddrSpec from Contact header",pErr);
#endif
							pErr->errCode=Sdf_en_headerManipulationError;
							return Sdf_en_fail;
						}
#ifdef SDF_TLS
						if((pAddrSpec->dType != SipAddrSipUri ) &&
								(pAddrSpec->dType != SipAddrSipSUri ))
#else
						if(pAddrSpec->dType != SipAddrSipUri )
#endif
						{
#ifdef SDF_TEL
							if(sip_isTelUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
								==SipSuccess)
								dIsTelUrl = Sdf_co_true;
#endif
#ifdef SDF_IM
							if( dIsTelUrl != Sdf_co_true )
							{
								if(sip_isImUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
									==SipSuccess)
									dIsImUrl = Sdf_co_true;
							}
#endif
#ifdef SDF_PRES
                            if((dIsTelUrl != Sdf_co_true)&&(dIsImUrl != Sdf_co_true))
                            {
                                if(sip_isPresUrl(pAddrSpec,(SipError*)&(pErr->stkErrCode)) \
                                    ==SipSuccess)
                                    dIsPresUrl = Sdf_co_true;
                            }
#endif
                            
							if((dIsTelUrl != Sdf_co_true) && (dIsImUrl != Sdf_co_true)&&(dIsPresUrl != Sdf_co_true))
							{
								dValidUrlCount = dValidUrlCount - 1;
								dIsUnknownUrl = Sdf_co_true;
							}
						}
						sip_freeSipAddrSpec(pAddrSpec);
					}
					else
					{
                         /* CSR_1-6665445 changes ends */
                        /* CSR 1-6099289 Changes Start */
                        if( dMethodType != Sdf_en_register )
                        /* CSR 1-6099289 Changes End */
                         /* CSR_1-6665445 changes ends */    
			            {
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_basicHeaderError, \
								(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
								"WildCard Contact header in non-Register "
								"Request message. Parsing Failed",pErr);
#endif
							sip_freeSipHeader(&dTempHeader);
							pErr->errCode=Sdf_en_basicHeaderError;
							return Sdf_en_fail;
            /* CSR_1-6665445 changes starts */
            /* CSR_1-6099289 changes starts */
                        }
            /* CSR_1-6665445 changes ends */
            /* CSR_1-6099289 changes ends */ 
					}
					sip_freeSipHeader(&dTempHeader);
				}/*--- end of for loop ----*/
				/*----check for atleast one valid url is there -----*/
				if((dValidUrlCount == 0 ) && (dIsUnknownUrl == Sdf_co_true ))
				{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_notImplemented, \
									(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
									"Contact header contains Unsupported URL Scheme",pErr);
#endif
					pErr->errCode=Sdf_en_notImplemented;
					return Sdf_en_fail;
				
				}
	 		}
            /*Fix done for CSR-1-6073699*/
            /*This is done to avoid the decode fail when 200 OK to REGISTER
             *request received with Contact header having value as "*"
             *(WildCarded).This is a common interop issue in which the
             *Registrar sends the value "*" in Contact Header of 200 OK response. */
	
#if 0 
  		/*---check for 2xx  having wildcard for register request---*/
			else 
			{
				dHdrCount = 0;
				/*w534*/ (void)sip_getHeaderCount(pMsg, SipHdrTypeContactAny, &dHdrCount,\
					(SipError*)&(pErr->stkErrCode));
				dValidUrlCount = dHdrCount;	
				for(dIndex = 0; dIndex < dHdrCount; dIndex++ )
				{
					if (sip_getHeaderAtIndex(pMsg, SipHdrTypeContactAny, \
						&dTempHeader, dIndex, (SipError*)&(pErr->stkErrCode)) == SipFail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_headerManipulationError, \
							(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
							"Failed to get Contact header at index from SipMessage",pErr);
#endif
						pErr->errCode=Sdf_en_headerManipulationError;
						return Sdf_en_fail;
					}
					if (dTempHeader.dType == SipHdrTypeContactWildCard)
					{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_basicHeaderError, \
								(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
								"WildCard Contact header in non-Register "
								"Request message. Parsing Failed",pErr);
#endif
							sip_freeSipHeader(&dTempHeader);
							pErr->errCode=Sdf_en_basicHeaderError;
							return Sdf_en_fail;
	
					}
					sip_freeSipHeader(&dTempHeader);

				}

			}
#endif
	
  		}
			/*---check for all responses except 2xx  having wildcard ---*/
		else 
		{
				dHdrCount = 0;
				/*w534*/ (void)sip_getHeaderCount(pMsg, SipHdrTypeContactAny, &dHdrCount,\
					(SipError*)&(pErr->stkErrCode));
				dValidUrlCount = dHdrCount;	
				for(dIndex = 0; dIndex < dHdrCount; dIndex++ )
				{
					if (sip_getHeaderAtIndex(pMsg, SipHdrTypeContactAny, \
						&dTempHeader, dIndex, (SipError*)&(pErr->stkErrCode)) == SipFail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_headerManipulationError, \
							(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
							"Failed to get Contact header at index from SipMessage",pErr);
#endif
						pErr->errCode=Sdf_en_headerManipulationError;
						return Sdf_en_fail;
					}
					if (dTempHeader.dType == SipHdrTypeContactWildCard)
					{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_basicHeaderError, \
								(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
								"WildCard Contact header in non-Register "
								"Request message. Parsing Failed",pErr);
#endif
							sip_freeSipHeader(&dTempHeader);
							pErr->errCode=Sdf_en_basicHeaderError;
							return Sdf_en_fail;
	
					}
					sip_freeSipHeader(&dTempHeader);

				}

		}
		

	}
#ifdef SDF_IM
	/*w534*/ (void)sip_getHeaderCount(pMsg, SipHdrTypeRecordRoute, &dHdrCount,\
		(SipError*)&(pErr->stkErrCode));

	for(dIndex = 0; dIndex < dHdrCount; dIndex++ )
	{
		if (sip_getHeaderAtIndex(pMsg, SipHdrTypeRecordRoute, \
			&dTempHeader, dIndex, (SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get Record Route header at index from SipMessage",\
				pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}

		if (sip_getAddrSpecFromRecordRouteHdr(&dTempHeader, &pAddrSpec, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get AddrSpec from Record Route header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
		if( pAddrSpec->dType == SipAddrReqUri)
		{
			if( sip_isImUrl(pAddrSpec, (SipError*)&(pErr->stkErrCode)) == \
					SipSuccess)
			{
				sip_freeSipHeader(&dTempHeader);
				sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_schemeNotAllowed, \
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
					"Record Router Contains Im URI",pErr);
#endif
				pErr->errCode=Sdf_en_schemeNotAllowed;
				return Sdf_en_fail;
			}
		}
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipHeader(&dTempHeader);
	} 
	dHdrCount = 0;
	/*w534*/ (void)sip_getHeaderCount(pMsg, SipHdrTypeRoute, &dHdrCount,\
		(SipError*)&(pErr->stkErrCode));

	for(dIndex = 0; dIndex < dHdrCount; dIndex++ )
	{
		if (sip_getHeaderAtIndex(pMsg, SipHdrTypeRoute, \
			&dTempHeader, dIndex, (SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get Route header at index from SipMessage", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}

		if (sip_getAddrSpecFromRouteHdr(&dTempHeader, &pAddrSpec, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get AddrSpec from Routed header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
		if( pAddrSpec->dType == SipAddrReqUri)
		{
			if( sip_isImUrl(pAddrSpec, (SipError*)&(pErr->stkErrCode)) == \
					SipSuccess)
			{
				sip_freeSipHeader(&dTempHeader);
				sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_schemeNotAllowed, \
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
					"Router Contains Im URI",pErr);
#endif
				pErr->errCode=Sdf_en_schemeNotAllowed;
				return Sdf_en_fail;
			}
		}
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipHeader(&dTempHeader);
	}
#endif
/* Check for presence of pres url
 */

#ifdef SDF_IM
	/*w534*/ (void)sip_getHeaderCount(pMsg, SipHdrTypeRecordRoute, &dHdrCount,\
		(SipError*)&(pErr->stkErrCode));

	for(dIndex = 0; dIndex < dHdrCount; dIndex++ )
	{
		if (sip_getHeaderAtIndex(pMsg, SipHdrTypeRecordRoute, \
			&dTempHeader, dIndex, (SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get Record Route header at index from SipMessage",\
				pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}

		if (sip_getAddrSpecFromRecordRouteHdr(&dTempHeader, &pAddrSpec, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get AddrSpec from Record Route header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
		if( pAddrSpec->dType == SipAddrReqUri)
		{
#ifdef SDF_PRES
			if( sip_isPresUrl(pAddrSpec, (SipError*)&(pErr->stkErrCode)) == \
					SipSuccess)
			{
				sip_freeSipHeader(&dTempHeader);
				sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_schemeNotAllowed, \
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
					"Record Router Contains Pres URI",pErr);
#endif
				pErr->errCode=Sdf_en_schemeNotAllowed;
				return Sdf_en_fail;
			}
#endif /* SDF_PRES */
		}
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipHeader(&dTempHeader);
	} 
	dHdrCount = 0;
	/*w534*/ (void)sip_getHeaderCount(pMsg, SipHdrTypeRoute, &dHdrCount,\
		(SipError*)&(pErr->stkErrCode));

	for(dIndex = 0; dIndex < dHdrCount; dIndex++ )
	{
		if (sip_getHeaderAtIndex(pMsg, SipHdrTypeRoute, \
			&dTempHeader, dIndex, (SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get Route header at index from SipMessage", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}

		if (sip_getAddrSpecFromRouteHdr(&dTempHeader, &pAddrSpec, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
				"Failed to get AddrSpec from Routed header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_fail;
		}
		if( pAddrSpec->dType == SipAddrReqUri)
		{
#ifdef SDF_PRES
			if( sip_isPresUrl(pAddrSpec, (SipError*)&(pErr->stkErrCode)) == \
					SipSuccess)
			{
				sip_freeSipHeader(&dTempHeader);
				sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_schemeNotAllowed, \
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateMessage(): "
					"Router Contains Pres URI",pErr);
#endif
				pErr->errCode=Sdf_en_schemeNotAllowed;
				return Sdf_en_fail;
			}
#endif /* SDF_PRES */			
		}
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipHeader(&dTempHeader);
	}
#endif
  	return Sdf_en_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaAddViaRecdParam
**
** DESCRIPTION: This functions Validates the Via header against the actual
**				address from which the message is received.  If both are
**				different then add "received" param to the Via header. This
**				Param contains the actual Remote IP address.
*****************************************************************************/
Sdf_ty_matchMessageType sdf_fn_uaAddViaRecdParam
#ifdef ANSI_PROTO
	( SipMessage  	*pMsg,
	  Sdf_st_eventContext *pEventContext,
	  Sdf_st_error *pErr)
#else
	(pMsg, pEventContext, pErr)
	SipMessage		*pMsg;
	Sdf_st_eventContext *pEventContext;
	Sdf_st_error 	*pErr;
#endif
{
	SipHeader dViaHeader;
	Sdf_ty_s8bit *pTempViaSentBy	= Sdf_co_null;
	Sdf_ty_s8bit *pTempViaSentProto = Sdf_co_null;
    Sdf_ty_s8bit dTempArray[Sdf_co_characterBufferSize] = {'\0',};
	Sdf_ty_s8bit *pViaSentBy		= Sdf_co_null;
	Sdf_ty_s8bit *pTempPort			= Sdf_co_null;
	Sdf_ty_s8bit *pActualReceivedIp = Sdf_co_null;

	/*
	 * Proceed to add the received param in the Via header if the 
	 * 'sentby' is different from where the request actually arrived.
	 */
	if(sip_getHeaderAtIndex(pMsg, SipHdrTypeVia, \
		&dViaHeader, (Sdf_ty_u32bit)0, (SipError*)&(pErr->stkErrCode)) \
		== SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaAddViaRecdParam(): "
			"Failed to get topmost Via header from SIP message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}

	if (sip_getSentByFromViaHdr(&dViaHeader, &pTempViaSentBy, \
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaAddViaRecdParam(): "
			"Failed to get sentby from Via header",pErr);
#endif			
		pErr->errCode=Sdf_en_headerManipulationError;
		sip_freeSipHeader(&dViaHeader);
		return Sdf_en_fail;
	}
	if((Sdf_mc_strlen(pTempViaSentBy) > sizeof(dTempArray)) ||
        (Sdf_mc_strlen(pTempViaSentBy) == 0) )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invRange,\
			(Sdf_ty_s8bit *)"sdf_fn_uaAddViaRecdParam(): "
			"Length of the Sent By filed is more than the specified length"
			" Sdf_co_characterBufferSizer",pErr);
#endif			
		pErr->errCode=Sdf_en_headerManipulationError;
		sip_freeSipHeader(&dViaHeader);
		return Sdf_en_fail;
	}
	Sdf_mc_strncpy(dTempArray,pTempViaSentBy,Sdf_co_characterBufferSize);

	/* 
	 * Take care of '[',']' while extracting the host from IPV6 reference. 
	 */
	if(Sdf_mc_strstr(dTempArray,"[") !=Sdf_co_null)
	{
		/* Strip the '[',']' before resolving the address. */
		pTempPort = Sdf_mc_strstr(dTempArray, "]:");
		pViaSentBy = dTempArray+1;
		if(pTempPort !=Sdf_co_null)
		{
			pTempPort=pTempPort+2;
			pViaSentBy[Sdf_mc_strlen(dTempArray)-Sdf_mc_strlen(\
				pTempPort)-3] = '\0';	
		}
		else
		{
			pViaSentBy[Sdf_mc_strlen(dTempArray)-2] = '\0';	
		}
	}
	else
	{
		pViaSentBy = Sdf_mc_strtokr(dTempArray, ":",&pTempPort);
	}

	pActualReceivedIp = pEventContext->pTranspAddr->pHost;
	if (Sdf_mc_strcmp(pViaSentBy, pActualReceivedIp) != 0)
	{
		Sdf_ty_s8bit	*pTmpValue = Sdf_co_null;
		/*
		 * address returned by recvfrom is not same as address put
		 * in the Via header. Add the received param here.
		 */
		SipParam *pReceivedParam = Sdf_co_null;
		if (sip_initSipParam(&pReceivedParam, (SipError *)\
			&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddViaRecdParam(): "
				"Failed to init a SipParam structure.",pErr);
#endif			
			sip_freeSipHeader(&dViaHeader);
			return Sdf_en_fail;
		}

		pTmpValue = Sdf_mc_strdupCallHandler("received");

		if (sip_setNameInSipParam(pReceivedParam, pTmpValue, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddViaRecdParam(): "
				"Failed to set name in a SipParam structure.",pErr);
#endif			
			sip_freeSipHeader(&dViaHeader);
			sip_freeSipParam(pReceivedParam);
			return Sdf_en_fail;
		}

		pTmpValue = Sdf_mc_strdupCallHandler(pActualReceivedIp);
		
		if (sip_insertValueAtIndexInSipParam(pReceivedParam, pTmpValue, \
			(Sdf_ty_u32bit)0, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddViaRecdParam(): "
				"Failed to insert value in a SipParam structure.",pErr);
#endif			
			sip_freeSipHeader(&dViaHeader);
			sip_freeSipParam(pReceivedParam);
			return Sdf_en_fail;
		}
		if (sip_insertViaParamAtIndexInViaHdr(&dViaHeader, pReceivedParam,\
			(Sdf_ty_u32bit)0, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddViaRecdParam(): "
				"Failed to insert value in a SipParam structure.", \
				pErr);
#endif			
			sip_freeSipHeader(&dViaHeader);
			sip_freeSipParam(pReceivedParam);
			return Sdf_en_fail;
		}
		sip_freeSipParam(pReceivedParam);
	}

	/*
	 * Check if there is a mismatch between the protocol in the topmost
	 * Via and the actual protocol on which it was received.
	 *
	 * If the topmost Via header has scheme SIP/2.0/TCP, but actually
	 * came on UDP, (or vice versa) flag off error. Application should 
	 * respond to this with 400 Bad Request 
	 * _after_changing_the_topmost_Via_ appropriately.
	 */
	if (sip_getSentProtocolFromViaHdr(&dViaHeader, &pTempViaSentProto, 
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaAddViaRecdParam(): "
			"Failed to get sent protocol from Via header",pErr);
#endif			
		pErr->errCode=Sdf_en_headerManipulationError;
		sip_freeSipHeader(&dViaHeader);
		return Sdf_en_fail;
	}
	
    if ( ((pEventContext->dProtocol == Sdf_en_protoTcp) && \
		  (Sdf_mc_strcasecmp(pTempViaSentProto, "SIP/2.0/TCP") != 0)) ||
#ifdef SDF_TLS
        ((pEventContext->dProtocol == Sdf_en_protoTls) && \
		 (Sdf_mc_strcasecmp(pTempViaSentProto, "SIP/2.0/TLS") != 0)) || 
#endif /* SDF_TLS */
		((pEventContext->dProtocol == Sdf_en_protoUdp) && \
		 (Sdf_mc_strcasecmp(pTempViaSentProto, "SIP/2.0/UDP") != 0)) )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_protocolMismatchInVia,\
			(Sdf_ty_s8bit *)"sdf_fn_uaAddViaRecdParam(): "
			"Incorrect transport protocol in topmost Via header",pErr);
#endif			
		pErr->errCode=Sdf_en_protocolMismatchInVia;
		sip_freeSipHeader(&dViaHeader);
		return Sdf_en_fail;
	}
	/* Free the local reference */
	sip_freeSipHeader(&dViaHeader);

	return Sdf_en_success;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaGetMessageInfo
**
** DESCRIPTION: This function extracts the basic information required from
**				sip message.  This returns the structure containing the
**				information.  It includes the Method type, its enum, req or 
**				resp type and response code incase of responses
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetMessageInfo
#ifdef ANSI_PROTO
	( SipMessage  		*pMsg,
	  Sdf_st_msgInfo	*pMessageInfo,
	  Sdf_st_error 		*pErr)
#else
	(pMsg, pMessageInfo, pErr)
	SipMessage			*pMsg;
	Sdf_st_msgInfo		*pMessageInfo;
	Sdf_st_error 		*pErr;
#endif
{
	if (sip_getMessageType(pMsg, &(pMessageInfo->dReqRespType), (SipError*)\
			&(pErr->stkErrCode)) ==  SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGetMessageInfo(): "
			"Failed to get type of SIP Message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	if(sdf_ivk_uaGetMethodFromSipMessage(pMsg, &(pMessageInfo->pMethod), \
		pErr) == Sdf_co_fail)
	{		
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGetMessageInfo(): "
			"Error in getting Method from Sip Message",pErr);
#endif
		return Sdf_co_fail;
	}

	/*w534*/ (void)sdf_fn_uaGetMethodNameAsEnum(pMessageInfo->pMethod, \
				&(pMessageInfo->dMsgType), pErr);

	if ( pMessageInfo->dReqRespType == SipMessageResponse)
	{
		if (sdf_ivk_uaGetRespCodeFromSipMessage(pMsg,\
			&(pMessageInfo->dResponse), pErr) == Sdf_co_fail)
		{		
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaGetMessageInfo(): "
				"Error in getting response code from SIP Message",pErr);
#endif
			return Sdf_co_fail;
		}	
	}
	else
		pMessageInfo->dResponse = 0;

	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaValidateRequireAndSupported
**
** DESCRIPTION: This function is used to invoke the function to validate
**			the Require header fields and validate the Method received
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateRequireAndSupported
#ifdef ANSI_PROTO
	( Sdf_st_callObject	*pCallObj,
	  Sdf_ty_transactionType	dTxnType,
	  Sdf_st_eventContext *pEventContext,
	  Sdf_st_overlapTransInfo *pOverlapTransInfo,
	  Sdf_st_error 		*pErr)
#else
	(pCallObj, dTxnType, pEventContext, pOverlapTransInfo, pErr)
	Sdf_st_callObject	*pCallObj;
	Sdf_ty_transactionType	dTxnType;
	Sdf_st_eventContext *pEventContext;
	Sdf_st_overlapTransInfo	*pOverlapTransInfo;
	Sdf_st_error 		*pErr;
#endif
{
    Sdf_st_transaction 		*pTransaction = Sdf_co_null;
	Sdf_st_validateInArg	dValidateInArg;
	Sdf_st_validateOutArg 	dValidateOutArg;
	Sdf_ty_messageValidity dValidity;

	if (sdf_ivk_uaGetTransactionFromCallObject(pCallObj, \
		dTxnType, &pTransaction,pErr) == Sdf_co_fail)
	{
		pErr->errCode=Sdf_en_callObjectAccessError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
		(Sdf_ty_s8bit *)"sdf_fn_uaValidateRequireAndSupported(): "
		"Getting Transaction Failed",pErr);
#endif
		return Sdf_co_fail;
	}

	dValidateInArg.pCallObj = pCallObj;
	dValidateInArg.pTransaction = pTransaction;
	dValidateInArg.pOverlapTransaction = pOverlapTransInfo;
	dValidateInArg.dTransactionType = dTxnType;
	dValidateInArg.pInitData = pCallObj->pInitData;

	if((pEventContext->dSdfDisabledChecks & Sdf_mc_disableRequireCheck)!=\
			Sdf_mc_disableRequireCheck)
	{
		dValidity = sdf_ivk_uaHandleRequireFromMsg(&dValidateInArg, \
				&dValidateOutArg,pErr);

		if((dValidity==Sdf_en_invalid) || (dValidity==Sdf_en_failed))	
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaValidateRequireAndSupported(): "
			"HandleRequire Failed",pErr);
#endif
			(void)sdf_ivk_uaFreeTransaction(pTransaction);
			return Sdf_co_fail;
		}
	}

	if((pEventContext->dSdfDisabledChecks & Sdf_mc_disableMethodCheck)!=\
			Sdf_mc_disableMethodCheck)
	{
		dValidity = sdf_ivk_uaIsSupportedMethodFromMsg(&dValidateInArg, \
				&dValidateOutArg, pErr);

		if((dValidity==Sdf_en_invalid) || (dValidity==Sdf_en_failed))	
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateRequireAndSupported(): "
				"IsSupported Failed",pErr);
#endif
			(void)sdf_ivk_uaFreeTransaction(pTransaction);
			return Sdf_co_fail;
		}
	}
	(void)sdf_ivk_uaFreeTransaction(pTransaction);
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaValidateCseqValue
**
** DESCRIPTION: 
**	  Validate whether the incoming request has a Cseq that is >
**	  (2**31 - 1). If so, report error Sdf_en_cseqValueExceeded.
**	  This is done only for non-ACK requests.
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateCseqValue
#ifdef ANSI_PROTO
	( SipMessage	*pSipMsg,
	  Sdf_st_error 	*pErr)
#else
	(pSipMsg, pErr)
	SipMessage		*pSipMsg;
	Sdf_st_error 	*pErr;
#endif
{
	SipHeader       header;
	Sdf_ty_u32bit   dCseqNum = 0;

	if (sip_getHeader( pSipMsg, SipHdrTypeCseq, &header, \
				(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaValidateCseqValue(): "
			"Failed to get CSeq header from the Message",pErr);
#endif
		pErr->errCode = Sdf_en_headerManipulationError;			
		return Sdf_co_fail;
	}

	if(sip_getSeqNumFromCseqHdr(&header,&dCseqNum,(SipError*)\
				&(pErr->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaValidateCseqValue(): "
			"Failed to get CSeq Value from the Message",pErr);
#endif
		sip_freeSipHeader(&header);
		pErr->errCode = Sdf_en_headerManipulationError;		
		return Sdf_co_fail;
	}
	if(dCseqNum>Sdf_mc_maxCseqValue)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_cseqValueExceeded,\
			(Sdf_ty_s8bit *)"sdf_fn_uaValidateCseqValue(): "
			"Maximum Value of CSeq exceeded",pErr);
#endif
		sip_freeSipHeader(&header);
		pErr->errCode = Sdf_en_cseqValueExceeded;		
		return Sdf_co_fail;
	}
	sip_freeSipHeader(&header);
	return Sdf_co_success;
}

#ifdef SDF_SESSION_TIMER
/*****************************************************************************
** FUNCTION: sdf_fn_uaUpdateSessionInfoFromMsg
**
** DESCRIPTION: This function is used to extract the information required to
**				session timer and update the call object structures.
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateSessionInfoFromMsg
#ifdef ANSI_PROTO
	( Sdf_st_callObject		*pCallObj,
	  Sdf_st_overlapTransInfo	*pOverlapTransInfo,
	  SipMessage			*pSipMsg,
	  Sdf_st_msgInfo		dMessageInfo,
	  Sdf_ty_state 			dCallState,
	  Sdf_st_eventContext   *pEventContext,
	  Sdf_st_error 			*pErr)
#else
	(pCallObj, pOverlapTransInfo, pSipMsg, dMessageInfo, dCallState, 
	 pEventContext, pErr)
	 Sdf_st_callObject		*pCallObj;
	 Sdf_st_overlapTransInfo	*pOverlapTransInfo;
	 SipMessage				*pSipMsg;
	 Sdf_st_msgInfo			dMessageInfo;
	 Sdf_ty_state 			dCallState;
	 Sdf_st_eventContext    *pEventContext,
	 Sdf_st_error 			*pErr;
#endif
{
	SipHeader 			dTempHeader;
	Sdf_ty_messageType	dMethodType = dMessageInfo.dMsgType;
	en_SipMessageType	dMsgType = dMessageInfo.dReqRespType;
	Sdf_ty_u16bit		dStatusCode=dMessageInfo.dResponse;

	(void)pEventContext;
	/* Currently the session paramaters are effected only for Invite and
	 * Bye requests. Later Update can also be considered. */
	if( (dMethodType != Sdf_en_invite) && (dMethodType != Sdf_en_bye) && \
				(dMethodType != Sdf_en_update))
	   return Sdf_co_success;
/* CSR_1-6042790 Change Start */
#if 0
/* CSR_1-6042790 Change End */
	
	/*
	 * Any session timer info that comes in 
	 * an early UPDATE should be ignored
	 */
	if((dMethodType == Sdf_en_update) && (sdf_fn_uaCallState(pCallObj)!=\
				Sdf_en_confirmedDialogState))
	   return Sdf_co_success;	
/* CSR_1-6042790 Change Start */
#endif
/* CSR_1-6042790 Change End */

/* CSR_5155164 Change Start */
   if(  (dMsgType == SipMessageRequest) && \
        (dMethodType == Sdf_en_bye) && \
        (pCallObj->pSessionTimer->pTimerId != Sdf_co_null ))
{
#ifdef SDF_TRACE
        sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,\
            (Sdf_ty_s8bit*)"sdf_fn_uaUpdateSessionInfoFromMsg():"
            " Issuing callback sdf_cbk_uaStopTimer"
            " for stopping Session-Timer ", 0, pErr);
#endif
        if(sdf_fn_uaStopSessionTimer((Sdf_ty_pvoid *)\
            &(pCallObj->pSessionTimer->pTimerId) ,pErr)==Sdf_co_fail)
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
                (Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionInfoFromMsg(): "
                " Failed to Stop the timer",pErr);
#endif
            return Sdf_co_fail;
        }
    }
/* CSR_5155164 Change End */


#if 0
	if( (dCallState == Sdf_en_callEstablished) && \
		(dMsgType == SipMessageRequest) && \
		(pCallObj->pSessionTimer->pTimerId != Sdf_co_null )) 
	{
#ifdef SDF_TRACE
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,\
			(Sdf_ty_s8bit*)"sdf_fn_uaUpdateSessionInfoFromMsg():"
			" Issuing callback sdf_cbk_uaStopTimer"
			" for stopping Session-Timer ", 0, pErr);
#endif
		if(sdf_fn_uaStopSessionTimer((Sdf_ty_pvoid *)\
			&(pCallObj->pSessionTimer->pTimerId) ,pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionInfoFromMsg(): "
				" Failed to Stop the timer",pErr);
#endif
			return Sdf_co_fail;
		}
	}
#endif
	/*
	 * The rest of the code is valid only for INVITE/UPDATE
	 * and responses to it. Bye processing stops here
	 */
	if (dMethodType == Sdf_en_bye)
	   return Sdf_co_success;
	/*
	 * If the incoming request has Session-Expires header, then set the
	 * 'dRefreshRequest' flag of SessionTimerInfo.
	 * 
	 * This flag is reset in sdf_ivk_uaSendCallToPeer()
	 * when the response if finally sent to peer.
	 */
	if (dMsgType==SipMessageRequest)
	{
		if ((sip_getHeader(pSipMsg, SipHdrTypeSessionExpires, &dTempHeader, \
				(SipError*)&(pErr->stkErrCode)) != SipFail))
		{
			/*
			 * In this state (Call established), we have received a 
			 * (re)fresh request, set the session timer flag 
			 * 'dRefreshRequest' to indicate that either a 
			 * (re)INVITE/UPDATE has come
			 */
			if (dMethodType==Sdf_en_invite)
				pCallObj->pSessionTimer->dRefreshRequest = \
								Sdf_en_refresherInvite;
			else if (dMethodType==Sdf_en_update)
				pCallObj->pSessionTimer->dRefreshRequest = \
								Sdf_en_refresherUpdate;
			
			sip_freeSipHeader(&dTempHeader);
		}
		else
		{
			/* If "Session-Expires" header didnt come in Request:
			 * Check if the Session-Timer was in use(we come to know whether
			 * or not Session-Timer was in use through the value of
			 * Session-Interval & MinSE)
			 * If yes, reset the value of these two parameters, so that
			 * further requests which originate from this UA goes without
			 * Session-Expires header
			 */
                 /* CSR_1_7108373 Merged SPR 19672 Start */
                 /* code has been removed from here to fix the problem and now session 
                    refresh headers are going in 200 OK and call is not getting cleared
                    after 35 seconds and IPTK waits for session refresh from peer which
                    it gets after 45seconds.
                 */
                 /* CSR_1_7108373 Merged SPR 19672 End */
		}
	}

	/*
	 * Update the SessionTimerInfo structure w.r.t incoming message
	 */
	/* CSR 1-6360207 changes start */
	/* CSR_1-6360207 As part of this CSR the dStatusCode is checked for all
	 * 2xx responses rather than the check for only 200*/
	if ((dMsgType==SipMessageRequest) || ((dMsgType==SipMessageResponse) \
                && (((dStatusCode >= 200) && (dStatusCode < 300)) || (dStatusCode==422))))
	{
		/*- Once a 422 comes, Session-Timer should be turned off -*/
		/*if (dStatusCode == 422)
			pCallObj->pSessionTimer->dSessionInterval = 0;*/
        /* CSR 1-6360207 changes End */
		if (sdf_fn_uaUpdateSessionTimerInfo(pCallObj, pSipMsg, \
							pErr) == Sdf_co_fail)
		{	
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionInfoFromMsg(): "
				" Updation of SessionTimerInfo details failed",pErr);
#endif
			return Sdf_co_fail;
		}
	}

    /* CSR 1-6447091 changes start */ 
       if (dMsgType == SipMessageResponse) 
             pCallObj->pSessionTimer->dRefreshRequest = Sdf_en_refresherInvalid; 
    /* CSR 1-6447091 changes end */ 

	/*
	 * If the response being sent is 200OK, then start the session timer
	 * after modyfying the SessionTimerInfo
	 */
	if( (dMsgType == SipMessageResponse) && (dStatusCode >= 200) && \
		(dStatusCode < 300))
	{
		Sdf_ty_u32bit 	dDuration;

		if(SipSuccess == sip_getHeader(pSipMsg,SipHdrTypeSessionExpires, \
				&dTempHeader,(SipError*)&(pErr->stkErrCode)))
		{
			sip_freeSipHeader(&dTempHeader);

			dDuration = pCallObj->pSessionTimer->dSessionInterval;
		
            /* CSR 1-6447091 changes start */
			/* As the dRefreshRequest is set to Sdf_en_refresherInvalid above
			 *  hence this is commneted. */
			/*pCallObj->pSessionTimer->dRefreshRequest = Sdf_en_refresherInvalid;*/
			/* CSR 1-6447091 changes start */
	
			if(Sdf_co_fail == sdf_fn_uaModifySessionInterval(&dDuration,\
						pCallObj->pSessionTimer->dRefresher,pErr))
			{	
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionInfoFromMsg(): "
					"Failed to Modify the Session Interval",pErr);
#endif
				return Sdf_co_fail;
			}
			else if ( Sdf_en_invalidRefesher == pErr->errCode)
			{
                /* If invalid refresher is the ecode then do nothing */
                /* pCallObj->pSessionTimer->dRefresher = Sdf_en_invalidRefesher;*/
			}
			else
			{			
#ifdef SDF_TRACE
			sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,\
				(Sdf_ty_s8bit*)"sdf_fn_uaUpdateSessionInfoFromMsg():"
				" Issuing callback sdf_cbk_uaStartTimer"
				" for starting Session-Timer ", 0, pErr);
#endif
            /* To start the session timer, we need to do
             * a restart mechanism.
             * SPR-5461
             */
            if(pCallObj->pSessionTimer->pTimerId != Sdf_co_null)
            {    
                if(sdf_fn_uaStopSessionTimer((Sdf_ty_pvoid *)\
                    &(pCallObj->pSessionTimer->pTimerId) ,pErr)
                        ==Sdf_co_fail)
                {
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
                     (Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionInfoFromMsg():"
                            "Failed to Stop the timer",pErr);
#endif
                    return Sdf_co_fail;
                }
            }
			if(sdf_fn_uaStartSessionTimer(pCallObj,\
				(Sdf_ty_pvoid *)&(pCallObj->pSessionTimer->pTimerId), \
				dDuration,pEventContext,pErr) == Sdf_co_fail)
			{	
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionInfoFromMsg(): "
					"Failed to Start Session Timer",pErr);
#endif
				return Sdf_co_fail;
		    }	
			}
		}
	}
    if( (dMsgType == SipMessageResponse) && 
         ((dStatusCode == 408) ||(dStatusCode == 481)))
    {
            /*For 408 and 481, we need to stop the session.
             */ 
          if(pCallObj->pSessionTimer->pTimerId != Sdf_co_null)
          {    
            if(sdf_fn_uaStopSessionTimer((Sdf_ty_pvoid *)\
                   &(pCallObj->pSessionTimer->pTimerId) ,pErr)
                        ==Sdf_co_fail)
              {
#ifdef SDF_ERROR
                  sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
                    (Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionInfoFromMsg():"
                          "Failed to Stop the timer",pErr);
#endif
                  return Sdf_co_fail;
              }
          }  
    }
	/*
	 * Request handling: Negotiate refresh interval & refresher
	 */
	if (dMsgType == SipMessageRequest) 
	{
#ifdef SDF_LINT
        Sdf_ty_s8bit dReqestMethod[10] = {'\0',};
#else        
        Sdf_ty_s8bit dReqestMethod[10];
#endif
        
		if (dMethodType == Sdf_en_invite)
			Sdf_mc_strcpy(dReqestMethod, "INVITE");
		else if (dMethodType == Sdf_en_update)
			Sdf_mc_strcpy(dReqestMethod, "UPDATE");
		
		if (sdf_fn_uaNegotiateRefresher(pCallObj,pErr) == Sdf_co_fail)
		{	
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionInfoFromMsg(): "
				"Negotiation of Refresher parameter of SessionTimerInfo" 
				" failed",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}

		if((pCallObj->pSessionTimer->dSessionInterval != 0) && \
		   ((sdf_fn_uaGetGlbMinSe() > pCallObj->pSessionTimer-> \
					dSessionInterval) || \
			(pCallObj->pSessionTimer->dMinSe > pCallObj->pSessionTimer-> \
			 		dSessionInterval)))
		{
			if(pCallObj->pSessionTimer->dIsSupportedTimer == Sdf_co_true)
			{
				if( dCallState == Sdf_en_idle )
				{
					if (sdf_ivk_uaChangeCallState(pCallObj, Sdf_co_null, \
						pSipMsg, Sdf_en_msgModeRecd, pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
						(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionInfoFromMsg(): "
							"Failed to Change the Call State",pErr);
#endif
						return Sdf_co_fail;
					}
				}

				/*- Once a 422 is sent, Session-Timer should be turned off -*/
				pCallObj->pSessionTimer->dSessionInterval = 0;
				if (sdf_ivk_uaFormResponse(422,dReqestMethod, pCallObj,\
					pOverlapTransInfo, Sdf_co_false, pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
						(Sdf_ty_s8bit *)"sdf_fn_uaUpdateSessionInfoFromMsg(): "
						"Forming 422 response failed",pErr);
#endif
					return Sdf_co_fail;
				}

				/* Get appropriate SipMsg to send -*/
				if (dMethodType == Sdf_en_invite)
					pSipMsg = pCallObj->pUasTransaction->pSipMsg;
				else if (dMethodType == Sdf_en_update)
						if(pOverlapTransInfo != Sdf_co_null)						
								pSipMsg = pOverlapTransInfo->pSipMsg;

				pErr->errCode = Sdf_en_lowSessionTimerIntervalError;
				sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting "
					"sdf_fn_uaUpdateSessionInfoFromMsg");
				return Sdf_co_fail;
			}
			else
			{
				pCallObj->pSessionTimer->dSessionInterval = \
									sdf_fn_uaGetGlbMinSe();

				if(pCallObj->pSessionTimer->dMinSe > sdf_fn_uaGetGlbMinSe())
					pCallObj->pSessionTimer->dMinSe = sdf_fn_uaGetGlbMinSe();
			}
		}
	}
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting from "
			"sdf_fn_uaUpdateSessionInfoFromMsg");
	return Sdf_co_success;
}
#endif

/*****************************************************************************
** FUNCTION: sdf_fn_uaUpdateToTag
**
** DESCRIPTION: If to Tag is not present in incoming message, it generates
**			the To tag and appends to it.
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateToTag
#ifdef ANSI_PROTO
	( Sdf_st_callObject *pCallObj,
	  SipMessage		*pSipMsg,
	  Sdf_ty_bool		*pIsToTagPresent,
	  Sdf_st_error 		*pErr)
#else
	( pCallObj, pSipMsg, pIsToTagPresent, pErr)
	Sdf_st_callObject	*pCallObj;
	SipMessage			*pSipMsg;
	SipMessage			*pIsToTagPresent;
	Sdf_st_error 		*pErr;
#endif
{
	SipHeader 	dHeader;
	Sdf_ty_u32bit dTagCount = 0;

	(void)pCallObj;

	/* Check if the To Tag present in message or not. If present then
	 * sent the boolean flag "*pIsToTagPresent" true. 
	 * else generate tag and insert into To tag field.*/
	if (sip_getHeader( pSipMsg, SipHdrTypeTo, &dHeader, \
				(SipError *)&(pErr->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *) "sdf_fn_uaUpdateToTag( ):"
			"Failed to get the To header from incoming SipMessage",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	 /*w534*/ (void)sip_getTagCountFromToHdr(&dHeader, &dTagCount, \
						(SipError *)&(pErr->stkErrCode));

	if (dTagCount != 0)
		*pIsToTagPresent = Sdf_co_true;

	sip_freeSipHeader(&dHeader);
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaFormAndSendResponse
**
** DESCRIPTION: Function to form the response and send it over network. 
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormAndSendResponse
#ifdef ANSI_PROTO
	( Sdf_st_callObject 		*pCallObj,
	  SipMessage				**ppMessage,
	  Sdf_ty_u16bit				dRespCode,
	  Sdf_st_eventContext 		*pEventContext,
	  Sdf_st_error 				*pErr)
#else
	( pCallObj, ppMessage, dRespCode, pEventContext, pErr)
	Sdf_st_callObject		*pCallObj;
	SipMessage				**ppMessage;
	Sdf_ty_u16bit			dRespCode;
	Sdf_st_eventContext 	*pEventContext;
	Sdf_st_error 			*pErr;
#endif
{
	
	SipMessage *pOutMsg;
  (void) pEventContext; 
	if (sdf_ivk_uaFormResponseMessage( pCallObj->pInitData, *ppMessage, \
			&pOutMsg, dRespCode, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormAndSendResponse(): "
			"Forming 481 response failed",pErr);
#endif
		return Sdf_co_fail;
	}
	sip_freeSipMessage( *ppMessage );

	*ppMessage = pOutMsg;

  return Sdf_co_success; 
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaHandleInvite
**
** DESCRIPTION: Code to handle the received new Invite
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleInvite
#ifdef ANSI_PROTO
	( Sdf_st_callObject **ppCallObj,
	  SipMessage		*pMessage,
	  Sdf_st_eventContext	*pEventContext,
	  Sdf_ty_state 		dCallState,
	  Sdf_st_error 		*pErr)
#else
	( ppCallObj, pMessage, pEventContext, dCallState, pErr)
	Sdf_st_callObject	**ppCallObj;
	SipMessage			*pMessage;
	Sdf_st_eventContext	*pEventContext;
	Sdf_ty_state 		dCallState;
	Sdf_st_error 		*pErr;
#endif
{
#ifdef SDF_TRACE
	Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
#endif
	(void)pMessage;

	if( dCallState == Sdf_en_idle )
	{
#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,"Call : %s"
			"- New call received; "
			"Issuing sdf_cbk_uaNewCallReceived to application", \
			(*ppCallObj)->pCommonInfo->pKey->pCallId);
		sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_cbksTraces, Trace, 0, pErr);
#endif

		if(Sdf_co_fail == sdf_cbk_uaNewCallReceived(ppCallObj, \
				pEventContext, pErr))
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleInvite(): "
				"Callback sdf_cbk_uaNewCallReceived failed",pErr);
#endif
			return Sdf_co_fail;
		}
	}
	else
	{
#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize, "Call : %s -"
			" Re-INVITE received;"
			"Issuing sdf_cbk_uaReInviteReceived to application",\
			(*ppCallObj)->pCommonInfo->pKey->pCallId);

		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces, Trace,0,pErr);
#endif
		/*
		* Issue the sdf_cbk_uaReInviteReceived callback to the application. 
		*/
		if(Sdf_co_fail == sdf_cbk_uaReInviteReceived(ppCallObj, \
						pEventContext, pErr))
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleInvite(): "
				"Callback sdf_cbk_uaReInviteReceived failed",pErr);
#endif
			return Sdf_co_fail;
		}
	}
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaGetOverlapTransInfo
**
** DESCRIPTION: This function is invoked from Update Call Details.  Based on
**			the transaction type and message type, if required returns either
**			the new overlap transaction or existing overlap transaction.
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetOverlapTransInfo
#ifdef ANSI_PROTO
	( Sdf_st_callObject			*pCallObj,
	  SipMessage				*pMessage,
	  Sdf_st_overlapTransInfo 	**ppOverlapTxnInfo,
	  Sdf_ty_u16bit				dStatusCode,
	  Sdf_ty_transactionType	dTransactionType,
	  en_SipMessageType 		dType,
	  Sdf_ty_messageType 		dMethodType,
	  Sdf_st_error 				*pErr)
#else
	( pCallObj, pMessage, ppOverlapTxnInfo, dStatusCode, dTransactionType, \
	  dType, dMethodType, pErr)
	  Sdf_st_callObject			*pCallObj;
	  SipMessage				*pMessage;
	  Sdf_st_overlapTransInfo 	**ppOverlapTxnInfo;
	  Sdf_ty_u16bit				dStatusCode;
	  Sdf_ty_transactionType	dTransactionType;
	  en_SipMessageType 		dType;
	  Sdf_ty_messageType 		dMethodType;
	  Sdf_st_error 				*pErr;
#endif
{
	Sdf_ty_bool		dIsOverTxnExists = Sdf_co_false;
	Sdf_ty_bool		dIsOverTxnRequred = Sdf_co_false;
#ifdef SDF_TRACE
	Sdf_ty_s8bit Trace[Sdf_co_smallBufferSize];
#endif

	if( dTransactionType == Sdf_en_overlapTransaction )
	{
		dIsOverTxnRequred = Sdf_co_true;

#ifdef SIP_RPR
		if( ((dType == SipMessageResponse) && (dStatusCode >= 200)) || \
		  ((dType == SipMessageRequest) && (dMethodType == Sdf_en_prack)))
#else
		if( (dType == SipMessageResponse) && (dStatusCode >= 200) )

#endif
			dIsOverTxnExists = Sdf_co_true;

		else if ( ((dType == SipMessageResponse) && (dStatusCode < 200)) || \
		  ((dType == SipMessageRequest) && (dMethodType == Sdf_en_comet)))
			dIsOverTxnExists = Sdf_co_false;
	}
	else if( (dTransactionType == Sdf_en_RegularUacTransaction) || \
		     (dTransactionType == Sdf_en_RegularUasTransaction))
	{
		dIsOverTxnRequred = Sdf_co_true;

		if( dType == SipMessageResponse )
			dIsOverTxnExists = Sdf_co_true;
		else
			dIsOverTxnExists = Sdf_co_false;
	}
	else
		dIsOverTxnRequred = Sdf_co_false;

	if( dIsOverTxnRequred == Sdf_co_true )
	{
		if ( dIsOverTxnExists == Sdf_co_true )
		{
			if (sdf_ivk_uaRetrieveMatchingOverlapTransInfo(pMessage, pCallObj, \
				ppOverlapTxnInfo, Sdf_en_msgModeRecd, pErr) == Sdf_co_fail)
			{
#ifdef SDF_TRACE
				(void)sdf_fn_uaSprintf(Trace,\
					"No Corresponding Overlap Transaction element Exist");
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_uaAllTraces,Trace, \
					0,pErr);
#endif
				pErr->errCode = Sdf_en_noUaError;
				return Sdf_co_fail;
			}
			/* Reduce the Ref dCount which got incremented by doing the
			 * Retrieve OverlapTransInfo */
			(void)sdf_ivk_uaFreeOverlapTransInfo(*ppOverlapTxnInfo);
		}
		/*
		 * If the incoming message is a COMET request, or an RPR, then an
		 * OverlapTransInfo should be created.
		 */
		else
		{
			if (sdf_ivk_uaInitOverlapTransInfo( ppOverlapTxnInfo, pErr) \
							== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaGetOverlapTransInfo(): "
					"Failed to initialize an OverlapTransInfo element",pErr);
#endif
				return Sdf_co_fail;
			}

			/* Append the new overlapTransInfo structure to the callObject's 
			 * list */

			if (sdf_listAppend(&(pCallObj->slOverlapTransInfo), \
				*ppOverlapTxnInfo, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaGetOverlapTransInfo(): "
					"Failed to add OverlapTransInfo element into original "
					"call object",pErr);
#endif
				pErr->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
		}
		(*ppOverlapTxnInfo)->dMsgType = dMethodType;

	}
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaUpdateTempCallObject
**
** DESCRIPTION: On sending any response over the Temp Call Object, this 
**				function is called.  This updates the Temp call object with
**				the information received.
*****************************************************************************/
Sdf_ty_messageValidity sdf_fn_uaUpdateTempCallObject
#ifdef ANSI_PROTO
	( Sdf_st_callObject			*pTempCallObj,
	  Sdf_st_callObject			*pOrigCallObj,
	  SipMessage				*pMessage,
	  Sdf_st_msgInfo			dMessageInfo,
	  Sdf_st_error 				*pErr)
#else
	( pTempCallObj, pOrigCallObj, pMessage, dMessageInfo, pErr)
	  Sdf_st_callObject			*pTempCallObj;
	  Sdf_st_callObject			*pOrigCallObj;
	  SipMessage				*pMessage;
	  Sdf_st_msgInfo			dMessageInfo;
	  Sdf_st_error 				*pErr;
#endif
{
	Sdf_st_initData *pInitData = Sdf_co_null;
	Sdf_ty_s8bit 			*pTag = Sdf_co_null;
	Sdf_st_commonInfo 		*pOrigCommonInfo;
	Sdf_st_commonInfo 		*pTempCommonInfo;
	Sdf_st_nameInfo 		*pOrgTo=Sdf_co_null;

	/* 
	 * Update the call object with the details of the incoming SIP
	 * message.
	 */
	if (sdf_fn_uaUpdateCallObject(pTempCallObj, Sdf_co_null, pMessage, \
		dMessageInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateTempCallObject(): "
			"Updating Call Object failed",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_en_failed;
	}
	
	if (sdf_fn_uaUpdateCallConstants(pTempCallObj, pMessage, dMessageInfo, \
		Sdf_co_true, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateTempCallObject(): "
			"Updating Call constants in common info failed",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_en_failed;
	}
	
	/* 
	 * Store all the message bodies present in the message into the
	 * slMsgBodyList of the call object
	 */
	if (sdf_fn_uaStoreAllMsgBodies(pMessage, \
		&(pTempCallObj->pUasTransaction->slMsgBodyList), pErr) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateTempCallObject(): "
			"Storing all message bodies in call object failed",pErr);
#endif
		return Sdf_en_failed;
	}
	
	/*
	 * Set the user profile of the original call object as the user
	 * profile of the temporary call object. This is to ensure that
	 * when a final response to the CANCEL is prepared within the
	 * callback, the correct tag is picked up from the user profile.
	 */
	if (sdf_ivk_uaGetInitDataFromCallObject(pOrigCallObj, \
		&pInitData, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateTempCallObject(): "
			"Unable to get user profile from original call object",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_en_failed;
	}

	if (sdf_ivk_uaSetInitDataInCallObject(pTempCallObj, pInitData, pErr)\
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateTempCallObject(): "
			"Failed to set user profile in temporary call object",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		(void)sdf_ivk_uaFreeInitData(pInitData);
		return Sdf_en_failed;
	}
	(void)sdf_ivk_uaFreeInitData(pInitData);

	/* Issue sdf_cbk_uaCancelReceived for INVITE and
	 * sdf_cbk_uaCancelTransaction for non-INVITE
	 */

	/* Copy the To Tag from Original Call Object to Temp Call Object.
	 * This will make sure that the 200 OK to CANCEL will have the
	 * same tag as that of 1xxs sent on the main call object.  */

	/* Before copying the to tag, check whether the incoming message
	 * has To-tag. If yes, the tag needn't be copied from original call
	 * object to temporary call object. */

	/*w534*/ (void)sip_getTagAtIndexFromToHdr(pTempCallObj->pCommonInfo->pTo, &pTag, \
			0, (SipError*)&(pErr->stkErrCode));

	if (pTag == Sdf_co_null)
	{
		Sdf_ty_s8bit	*pTmpTag = Sdf_co_null;

		/*w534*/ (void)sdf_ivk_uaGetCommonInfoFromCallObject(pOrigCallObj, \
				&pOrigCommonInfo, pErr); 

		/*w534*/ (void)sdf_ivk_uaGetToInfoFromCommonInfo(pOrigCommonInfo, &pOrgTo, pErr); 
	
		/*w534*/ (void)sdf_ivk_uaGetCommonInfoFromCallObject(pTempCallObj, \
				&pTempCommonInfo, pErr);

		if ((Sdf_co_null != pOrgTo) && (pOrgTo->pTag != Sdf_co_null))
		{
			pTmpTag = pOrgTo->pTag;

			if(sip_insertTagAtIndexInToHdr(pTempCommonInfo->pTo, pTmpTag, \
				0, (SipError *)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError,
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateTempCallObject(): "
					"Failed to set user profile in temporary call object",pErr);
#endif
				(void)sdf_ivk_uaFreeNameInfo(pOrgTo);
				(void)sdf_ivk_uaFreeCommonInfo(pTempCommonInfo);
				(void)sdf_ivk_uaFreeCommonInfo(pOrigCommonInfo);
				return Sdf_en_failed;
			}
		}
		(void)sdf_ivk_uaFreeNameInfo(pOrgTo);
		(void)sdf_ivk_uaFreeCommonInfo(pTempCommonInfo);
		(void)sdf_ivk_uaFreeCommonInfo(pOrigCommonInfo);
	}
	return Sdf_en_valid;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaHandleAck
**
** DESCRIPTION: Code to handle the ACK received
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleAck
#ifdef ANSI_PROTO
	( Sdf_st_callObject **ppCallObj,
	  SipMessage		*pMessage,
	  Sdf_st_eventContext	*pEventContext,
	  Sdf_ty_state 		dCallState,
	  Sdf_st_error 		*pErr)
#else
	( ppCallObj, pMessage, pEventContext, dCallState, pErr)
	Sdf_st_callObject	**ppCallObj;
	SipMessage			*pMessage;
	Sdf_st_eventContext	*pEventContext;
	Sdf_ty_state 		dCallState;
	Sdf_st_error 		*pErr;
#endif
{
#ifdef SDF_TRACE	
	Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
#endif	
	(void)pMessage;

	if (dCallState == Sdf_en_reInviteFinalResponseSent)
	{
		/* ACK for the final response to a re-INVITE */
#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
			"Call : %s -  Acknowledgement to Re-INVITE received;"
			"Issuing sdf_cbk_uaReInviteAcknowledged to application",\
			(*ppCallObj)->pCommonInfo->pKey->pCallId);
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces, \
				Trace,0,pErr);
#endif
		/*- Reset the Rseq, so that the New RSeq sent 'll wrap up -*/
		(*ppCallObj)->pCommonInfo->dLocalRseq = 0;

        /* Fix for CSR 1-5210208 */
		sip_freeSipAddrSpec( (*ppCallObj)->pCommonInfo->pNewRequestUri );
		(*ppCallObj)->pCommonInfo->pNewRequestUri =Sdf_co_null;
		/* Fix for CSR 1-5210208 */

		/* Issue the sdf_cbk_uaReInviteAcknowledged callback to the 
		 * application.  */
		if( Sdf_co_fail == sdf_cbk_uaReInviteAcknowledged(ppCallObj, \
			pEventContext, pErr))
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleAck(): "
				"Callback sdf_cbk_uaReInviteAcknowledged failed",pErr);
#endif
			return Sdf_co_fail;
		}
	}
	else if (dCallState == Sdf_en_finalSuccessSent)
	{
		/* ACK for the final response to a INVITE */

#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
			"Call : %s -  Call Acknowledgement received;"
			"Issuing sdf_cbk_uaCallAcknowledged to application",\
			(*ppCallObj)->pCommonInfo->pKey->pCallId);
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace,0,pErr);
#endif

		/*- Reset the Rseq, so that the New RSeq sent 'll wrap up -*/
		(*ppCallObj)->pCommonInfo->dLocalRseq = 0;

		/* 
		 * Issue the sdf_cbk_uaCallAcknowledged callback to the 
		 * application. 
		 */
		if( Sdf_co_fail == sdf_cbk_uaCallAcknowledged( ppCallObj, \
			pEventContext, pErr))
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleAck(): "
				"Callback sdf_cbk_uaCallAcknowledged failed",pErr);
#endif
			return Sdf_co_fail;
		}
	}
	else if (dCallState == Sdf_en_finalFailureSent)
	{
#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(Trace, Sdf_co_traceBufferSize,\
			"Call: %s - Call Failure Acknowledged received;"
			"Issuing sdf_cbk_uaFailedCallAcknowledged to application",\
			(*ppCallObj)->pCommonInfo->pKey->pCallId);

		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace,0,pErr);
#endif
		/* 
		 * Issue the sdf_cbk_uaFailedCallAcknowledged callback to the 
		 * application. 
		 */
		if(Sdf_co_fail == sdf_cbk_uaFailedCallAcknowledged( ppCallObj, \
			pEventContext, pErr))
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleAck(): "
				"Callback sdf_cbk_uaFailedCallAcknowledged failed", pErr);
#endif
			return Sdf_co_fail;
		}
	}
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaHandleProvResp
**
** DESCRIPTION: This function handles the provisional responses received
**				for Invite request.
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleProvResp
#ifdef ANSI_PROTO
	( Sdf_st_callObject **ppCallObj,
	  SipMessage		*pMessage,
	  Sdf_st_overlapTransInfo	*pOverlapTxnInfo,
	  Sdf_st_eventContext	*pEventContext,
	  Sdf_st_error 		*pErr)
#else
	( ppCallObj, pMessage, pOverlapTxnInfo, pEventContext, pErr)
	Sdf_st_callObject	**ppCallObj;
	SipMessage			*pMessage;
	Sdf_st_overlapTransInfo	*pOverlapTxnInfo;
	Sdf_st_eventContext	*pEventContext;
	Sdf_st_error 		*pErr;
#endif
{
	Sdf_ty_retVal	retVal = Sdf_co_success;
#ifdef SDF_TRACE	
	Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
#endif	
#ifdef SIP_RPR

	
	/*
	 * If it is a reliable provisional response, issue the 
	 * sdf_cbk_uaRPRReceived; else issue the sdf_cbk_uaInProgress
	 * callback.
	 */
	if (sdf_fn_uaIsMessageRPR(pMessage, pErr) == 0)
	{
#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
			"Call : %s - RPR received;"
			"Issuing sdf_cbk_uaRPRReceived to application",\
			(*ppCallObj)->pCommonInfo->pKey->pCallId);
		sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_cbksTraces, Trace,\
			0,pErr);
#endif
		/* 
		 * Issue the sdf_cbk_uaRPRReceived callback to the 
		 * application. 
		 */
		retVal = sdf_cbk_uaRPRReceived(ppCallObj, pOverlapTxnInfo, \
						pEventContext, pErr);
	}
	else
#endif	
	{
#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
			"Call : %s - 1xx response received;"
			"Issuing sdf_cbk_uaInProgress to application",\
			(*ppCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_cbksTraces, \
			Trace, 0,pErr);
#endif
		/* 
		 * Issue the sdf_cbk_uaInProgress callback to the 
		 * application. 
		 */
		retVal = sdf_cbk_uaInProgress (ppCallObj, pOverlapTxnInfo, \
				pEventContext, pErr);
	}
	
	if (retVal == Sdf_co_fail)
	{
		pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaHandleProvResp(): "
			"Callback sdf_cbk_uaInProgress/sdf_cbk_uaRPRReceived "
			"failed",pErr);
#endif
		return Sdf_co_fail;
	}

	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaHandleInvFinalResp
**
** DESCRIPTION: Function handles the final response to Invite.
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleInvFinalResp
#ifdef ANSI_PROTO
	( Sdf_st_callObject **ppCallObj,
	  SipMessage		*pMessage,
	  Sdf_ty_u16bit		dStatusCode,
	  Sdf_ty_state		dCallState,
	  Sdf_st_eventContext	*pEventContext,
	  Sdf_st_error 		*pErr)
#else
	( ppCallObj, pMessage, dStatusCode, dCallState, pEventContext, pErr)
	Sdf_st_callObject	**ppCallObj;
	SipMessage			*pMessage;
	Sdf_ty_u16bit		dStatusCode;
	Sdf_ty_state		dCallState;
	Sdf_st_eventContext	*pEventContext;
	Sdf_st_error 		*pErr;
#endif
{
	Sdf_ty_retVal		retVal = Sdf_co_success;	
#ifdef SDF_TRACE	
	Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
#endif	
	(void)pMessage;
	
	if( (dStatusCode >= 200) && (dStatusCode <= 299) )
	{
        /* CSR_1-6392221 change*/
		if ((dCallState == Sdf_en_reInviteSent) || \
			(dCallState == Sdf_en_reInviteprovRespReceived) || \
            (dCallState == Sdf_en_byeSent))
		{
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
				"Call : %s - Call accepted; Issuing "
				"sdf_cbk_uaReInviteAccepted to application",\
				(*ppCallObj)->pCommonInfo->pKey->pCallId);

			sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,\
				Trace,0,pErr);
#endif
			/* 
			 * Issue the sdf_cbk_uaReInviteAccepted callback to the 
			 * application. 
			 */
			retVal =  sdf_cbk_uaReInviteAccepted(ppCallObj,\
						pEventContext, pErr);
		}
		else
		{
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
				"Call : %s - Call accepted; "
				"Issuing sdf_cbk_uaCallAccepted to application",\
				(*ppCallObj)->pCommonInfo->pKey->pCallId);

			sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,\
				Trace,0,pErr);
#endif

			/* 
			 * Issue the sdf_cbk_uaCallAccepted callback to the 
			 * application. 
			 */
			retVal =  sdf_cbk_uaCallAccepted (ppCallObj, pEventContext, pErr);
		}

		if (retVal == Sdf_co_fail)
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			if (dCallState == Sdf_en_reInviteSent)
			{
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaHandleInvFinalResp(): "
					"Callback sdf_cbk_uaReInviteAccepted failed",pErr);
			}
			else
			{
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaHandleInvFinalResp(): "
					"Callback sdf_cbk_uaCallAccepted failed",pErr);
			}
#endif
			return Sdf_co_fail;
		}
	}
	else if( (dStatusCode >= 300) && (dStatusCode <= 399) )
	{
#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
			"Call : %s - Call Re-directed; "
			"Issuing  sdf_cbk_uaCallRedirected to application",\
			(*ppCallObj)->pCommonInfo->pKey->pCallId);

		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace,0,pErr);
#endif

		/* 
		 * Issue the sdf_cbk_uaCallRedirected callback to the 
		 * application. 
		 */
		retVal = sdf_cbk_uaCallRedirected (ppCallObj, pEventContext, pErr);


		if (retVal == Sdf_co_fail)
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleInvFinalResp(): "
				"Callback sdf_cbk_uaCallRedirected failed", pErr);
#endif
			return Sdf_co_fail;
		}
	}
	else if(dStatusCode >= 400)
	{
		if ( (dCallState == Sdf_en_reInviteSent) ||
			 (dCallState == Sdf_en_reInviteprovRespReceived) )
		{
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
				"Call : %s - Re-INVITE failed; "
				"Issuing sdf_cbk_uaReInviteFailed  to application",\
				(*ppCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_cbksTraces, \
				Trace,	0,pErr);
#endif
			/* 
			 * Issue the sdf_cbk_uaReInviteFailed callback to the 
			 * application. 
			 */
			retVal = sdf_cbk_uaReInviteFailed(ppCallObj, \
				pEventContext, pErr);
		}
		else
		{
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
				"Call : %s - Call failed; "
				"Issuing  sdf_cbk_uaCallFailed to application",\
				(*ppCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces, \
				Trace,0,pErr);
#endif
			/* 
			 * Issue the sdf_cbk_uaCallFailed callback to the 
			 * application. 
			 */
			retVal = sdf_cbk_uaCallFailed (ppCallObj, pEventContext, pErr);
		}

		if (retVal == Sdf_co_fail)
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			if (dCallState == Sdf_en_reInviteSent)
			{
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaHandleInvFinalResp(): "
					"Callback sdf_cbk_uaReInviteFailed failed",pErr);
			}
			else
			{
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaHandleInvFinalResp(): "
					"Callback sdf_cbk_uaCallFailed failed",pErr);
			}
#endif
			return Sdf_co_fail;
		}
	}
	return Sdf_co_success;
}/*****************************************************************************
** FUNCTION: sdf_fn_uaCheckGlareCondition
**
** DESCRIPTION: This function is used to check the Invites crossing the wire.
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaCheckGlareCondition
#ifdef ANSI_PROTO
	( Sdf_st_callObject *pCallObj,
	  Sdf_st_error 		*pErr)
#else
	( pCallObj, pErr)
	  Sdf_st_callObject *pCallObj;
	  Sdf_st_error 		*pErr;
#endif
{
	pErr = pErr;
	
	if ( ( (pCallObj->pCallInfo->dState != Sdf_en_idle) && \
	   (pCallObj->pCallInfo->dState != Sdf_en_callEstablished) ) || \
	   (pCallObj->pUacTransaction->dTransactionState == Sdf_en_transactionSent))
	{
		return Sdf_co_fail;
	}
	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaEstablishChannel
**
** DESCRIPTION: This function is used to establish the TCP channel towards
**				the remote endpoint.  If connection is already established
**				it simply returns without try to establish again
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaEstablishChannel
#ifdef ANSI_PROTO
	( Sdf_st_callObject	*pCallObj,	 
	  Sdf_st_overlapTransInfo	*pOverlapTransInfo,
	  en_SipMessageType	 dReqRespType,
	  Sdf_ty_messageType dMethodType,
	  Sdf_st_error 		 *pError)
#else
	( pCallObj, pOverlapTransInfo, dReqRespType, dMethodType, pError)
	  Sdf_st_callObject *pCallObj;
	  Sdf_st_overlapTransInfo	*pOverlapTransInfo;
	  en_SipMessageType	 dReqRespType;
	  Sdf_ty_messageType dMethodType;
	  Sdf_st_error 		*pError;
#endif
{
	Sdf_st_transaction 	*pTransaction;
	Sdf_st_transportInfo *pDestInfo;

	if (dReqRespType == SipMessageRequest)
		pTransaction = pCallObj->pUacTransaction;
	else
		pTransaction = pCallObj->pUasTransaction;

	if(sdf_ivk_uaGetDestInfoFromTransaction(pTransaction,\
		pOverlapTransInfo,&pDestInfo, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaEstablishChannel( ): "
			"Getting DestInfo from call object failed",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}
	
	/* 
	 * If no scheme is set, UDP is set as the default
	 */

    if((pDestInfo->dScheme == Sdf_en_protoNone) && \
       (pCallObj->pCommonInfo->dScheme == Sdf_en_protoNone))
            pDestInfo->dScheme = Sdf_en_protoUdp;

    else if((pDestInfo->dScheme == Sdf_en_protoNone) && \
            (pCallObj->pCommonInfo->dScheme != Sdf_en_protoNone))
            pDestInfo->dScheme = pCallObj->pCommonInfo->dScheme;

	
	/* At the time of Sending INVITE fill-up the CommonInfo pScheme
	 * so that latter while forimg PRACK we can take it from CommonInfo
	 * pScheme. bcoz At that time pOverlapTransInfo->pScheme will be
	 * null
	 */
	if((dReqRespType==SipMessageRequest) && ( dMethodType == Sdf_en_invite))
	{

        pCallObj->pCommonInfo->dScheme=pDestInfo->dScheme;
	}
#ifdef SDF_TLS
	if ((pDestInfo->dScheme == Sdf_en_protoTcp) || \
			(pDestInfo->dScheme == Sdf_en_protoTls))
#else
	if (pDestInfo->dScheme == Sdf_en_protoTcp)
#endif
	{
		if (sdf_fn_uaInitiateConnection(pTransaction, pOverlapTransInfo,\
			pCallObj->pInitData, pError) == Sdf_co_fail)
		{	
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_tcpConnectError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaEstablishChannel( ): "
				"Could not initiate TCP connection with remote host\n",pError);
#endif
			pError->errCode=Sdf_en_tcpConnectError;
			(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
			return Sdf_co_fail;
		}	
	}	
	(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
	return Sdf_co_success;
}



/*****************************************************************************
** FUNCTION: sdf_fn_uaSendSipMsgOverNetwork
**
** DESCRIPTION: This function is used to send the message over network.  
**				This function is invoked from Send Call To Peer. This
**				Initialises the context required and invokes the sip function
**				to send the message over network.
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaSendSipMsgOverNetwork
#ifdef ANSI_PROTO
	( Sdf_st_callObject 	*pCallObj,
	  Sdf_st_transaction	*pTransaction,
	  Sdf_st_overlapTransInfo	*pOverlapTransInfo,
	  SipMessage			*pMessage,
	  en_SipMessageType		dReqRespType,
	  Sdf_st_eventContext 	*pEventContext,
	  SipOptions 			*pOptions,
	  SipTranspAddr		 	*pTranspaddr,
	  Sdf_ty_s8bit 			transptype,
	  Sdf_st_error 		 	*pError)
#else
	( pCallObj, pTransaction, pOverlapTransInfo, pMessage, dReqRespType, \
	  pEventContext, pOptions, pTranspaddr, transptype, pError)
	  Sdf_st_callObject 	*pCallObj;
	  Sdf_st_transaction	*pTransaction;
	  Sdf_st_overlapTransInfo	*pOverlapTransInfo;
	  SipMessage			*pMessage;
	  en_SipMessageType		dReqRespType;
	  Sdf_st_eventContext	*pEventContext;
	  SipOptions 			*pOptions;
	  SipTranspAddr			*pTranspaddr;
	  Sdf_ty_s8bit			transptype;
	  Sdf_st_error 		 	*pError;

#endif
{
#ifdef SDF_TXN
	Sdf_st_error	dLocalErr;
	SipTxnContext			*pSipTxnContext		= Sdf_co_null;
	SipTxnKey				*pSipTxnKey			= Sdf_co_null;
	Sdf_st_txnContextData	*pTxnContextData	= Sdf_co_null;
	Sdf_st_txnContextData	*pData = Sdf_co_null;
#endif
	
#ifdef SDF_TXN
	/*
	 * Txn layer handling: Common code (for both TCP and UDP).
	 * Initialize a context structure first and set the 
	 * options, event context etc.
	 */
	if (sip_txn_initSipTxnContext(&pSipTxnContext, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSendSipMsgOverNetwork( ): "
			"Failed to initialize a txn context struct.",pError);
#endif
		pError->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail ;
	}
	
	pSipTxnContext->pEventContext		= (SipEventContext *)pEventContext;
	pSipTxnContext->dTxnType			= SipUATypeTxn;
	pSipTxnContext->txnOption.dOption	= *pOptions;
	pSipTxnContext->timeoutValues		= pEventContext->dTimeoutValues;
	pSipTxnContext->txnOption.dTimerOption = pEventContext->dTimerOption;
	pSipTxnContext->txnOption.dTimeoutCbkOption = pEventContext->dTimeoutCbkOption;
	
	/*
	 * Also set the transaction context data.
	 */
	if (sdf_ivk_uaInitTxnContextData(&pTxnContextData,\
		pError) == Sdf_co_fail)
	{
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&pSipTxnContext, pError);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSendSipMsgOverNetwork( ): "
			"Failed to initialize data for txn context struct.",pError);
#endif
		pError->errCode = Sdf_en_noMemoryError;

		return Sdf_co_fail ;
	}

	/*
	 * Set the call object in the data. Also encapsulate the user 
	 * defined data with the txncontextdata structure.
	 */
	HSS_LOCKEDINCREF(pCallObj->dRefCount);
	pTxnContextData->pRefCallObject = pCallObj;
	pTxnContextData->pApplicationData = pEventContext->pData;
	HSS_LOCKEDINCREF(pEventContext->dRefCount);
	pTxnContextData->pEventContext = pEventContext;

#ifdef SDF_SUBSCRIPTION_LAYER
  /* Copy the SubsObj into TxnContextData */
	/* COMPILATION ERROR: src/sdf_internal.c:15679: dereferencing pointer to 
	incomplete type*/
  if (pEventContext->pSubsObj != Sdf_co_null)
  {
    pTxnContextData->pSubsObj = 
        (Sdf_st_uaSLSubscriptionObject *)
                                pEventContext->pSubsObj;

    /* NOTE: Set the pSubsObj in EventContext to NULL, else pSubsObj will be
     * free 2 times when sip_freeEventContext (sdf_txn.c) is called (once in
     * sdf_ivk_uaFreeTxnContextData and again in sdf_ivk_uaFreeEventContext).
     */
    pEventContext->pSubsObj = Sdf_co_null;

    /* NOTE: Since the EventContext is already being cloned in 
		 * sdf_ivk_uaSendCallToPeer, we needn't increment the refCount here.
    HSS_LOCKEDINCREF(pTxnContextData->pSubsObj->dRefCount);
		*/
  }
#endif
	pEventContext->pData = pTxnContextData;

	/*
	 * Set the transaction type and the actual transaction structure 
	 * in the context information.
	 */
	if (pOverlapTransInfo != Sdf_co_null)
	{
#ifdef SIP_RPR	
		if (sdf_fn_uaIsMessageRPR(pMessage, pError) == 0)
		{
			pTxnContextData->dType = Sdf_en_overlapTransaction;
		}
		else
#endif		
		{
			if (dReqRespType == SipMessageRequest)
				pTxnContextData->dType = Sdf_en_RegularUacTransaction;
			else
				pTxnContextData->dType = Sdf_en_RegularUasTransaction;
		}
		
		/*
		 * Store the overlapping transaction in the 
		 * Sdf_st_txnContextData structure here so 
		 * that it is available in the timeout cases
		 * as well.
		 */
		pTxnContextData->pTransaction = (Sdf_ty_pvoid) pOverlapTransInfo;
		HSS_LOCKEDINCREF(pOverlapTransInfo->dRefCount);
	}
	else
	{
		if (dReqRespType == SipMessageRequest)
		{
			pTxnContextData->dType = Sdf_en_uacTransaction;
			pTxnContextData->pTransaction = \
				(Sdf_ty_pvoid) pCallObj->pUacTransaction;
			HSS_LOCKEDINCREF(pCallObj->pUacTransaction->dRefCount);
		}
		else
		{
			pTxnContextData->dType = Sdf_en_uasTransaction;
			pTxnContextData->pTransaction = \
				(Sdf_ty_pvoid) pCallObj->pUasTransaction;
			HSS_LOCKEDINCREF(pCallObj->pUasTransaction->dRefCount);
		}
	}
#endif /* End - ifdef SDF_TXN */
	
#ifndef SDF_TXN
	pCallObj = pCallObj;
	pTransaction = pTransaction;
	pOverlapTransInfo = pOverlapTransInfo;
	dReqRespType = dReqRespType;

	/* 
	 * NON-TXN LAYER HANDLING
	 * ----------------------
	 */
	if ((sip_sendMessage(pMessage, pOptions, pTranspaddr,\
		 transptype, (SipEventContext *)pEventContext,\
		 (SipError*)&(pError->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_sendNetworkError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSendSipMsgOverNetwork( ): "
			"Could not send SIP message using UDP",pError);
#endif
		pError->errCode=Sdf_en_sendNetworkError;
		return Sdf_co_fail ;
	}
#else
	/* 
	 * TXN LAYER HANDLING
	 * ------------------
	 */
#if 0
	/* Enable this for CERT testing (Corresponding to this, there must be a free
	 * in file sdf_network.c(sdf_ivk_uaSendCallToPeer)*/
	if ((sip_txn_sendMessage(pMessage, pTranspaddr, \
		transptype, pSipTxnContext,	SipTxnByPass, &pSipTxnKey, \
		(SipError*)&(pError->stkErrCode))) == SipFail)
#endif
	if ((sip_txn_sendMessage(pMessage, pTranspaddr, \
		transptype, pSipTxnContext,	SipTxnNoByPass, &pSipTxnKey, \
		(SipError*)&(pError->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_sendNetworkError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSendSipMsgOverNetwork( ): "
			"Could not send SIP message using UDP",pError);
#endif
		pError->errCode=Sdf_en_sendNetworkError;
		pData = (Sdf_st_txnContextData *) pEventContext->pData;
		pEventContext->pData = pData->pApplicationData;
		pData->pApplicationData = Sdf_co_null;
		(void)sdf_ivk_uaFreeTxnContextData(pData);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&pSipTxnContext, &dLocalErr);
		return Sdf_co_fail ;
	}
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pSipTxnContext,pError);
		
	/*
	 * Txn layer handling: Store the transaction key here.
	 * No need to increment reference dCount as stack has 
	 * done it already.
	 */
	if (pSipTxnKey != Sdf_co_null)
	{
		if (pOverlapTransInfo != Sdf_co_null)
		{
			Sdf_ty_bool	dMsgIsRpr	= Sdf_co_false;
#ifdef SIP_RPR				
			if (sdf_fn_uaIsMessageRPR(pMessage, pError) == 0)
			{
				sip_freeSipTxnKey(pOverlapTransInfo->pRprTxnKey);
				pOverlapTransInfo->pRprTxnKey = pSipTxnKey;
				dMsgIsRpr = Sdf_co_true;
			}
#endif			
			if (dMsgIsRpr == Sdf_co_false)
			{
				sip_freeSipTxnKey(pOverlapTransInfo->pOLTxnKey);
				pOverlapTransInfo->pOLTxnKey = pSipTxnKey;
			}
		}
		else
		{
			sip_freeSipTxnKey(pTransaction->pTxnKey);
			pTransaction->pTxnKey = pSipTxnKey;
			/* If the method in message is INVITE,then store the TxnKey
			 * in pInvTxnKey also.This will be used when the
			 * application calls sdf_ivk_uaAbortCall
			 */
			if(pTransaction->pTxnKey!=pTransaction->pInvTxnKey)
			{
				if(Sdf_mc_strcmp(pTransaction->pTxnKey->pMethod,\
					"INVITE")==0)
				{
					sip_freeSipTxnKey(pTransaction->pInvTxnKey);
					pTransaction->pInvTxnKey=pSipTxnKey;
					HSS_LOCKEDINCREF(pSipTxnKey->dRefCount);
				}
			}

		}
	}
#endif
	return Sdf_co_success;
}

#ifndef SDF_TXN
/*****************************************************************************
** FUNCTION: sdf_fn_uaPopulateRREntry
**
** DESCRIPTION: This function is called from Send Call To peer.  After sending
**			 the responses, the remote retransmission entries are updated with
**			 it.
*****************************************************************************/

Sdf_ty_retVal sdf_fn_uaPopulateRREntry
#ifdef ANSI_PROTO
	( Sdf_st_callObject 	*pCallObj,
	  SipMessage			*pMessage,
	  Sdf_st_transportInfo 	*pDestInfo,
	  Sdf_ty_socketId 		dSockfd,
	  SipOptions 			dOptions,
	  Sdf_st_eventContext	*pEventContext,
	  Sdf_st_msgInfo		dMessageInfo,
	  Sdf_st_error 			*pErr)
#else
	( pCallObj, pMessage, pDestInfo, dSockfd, dOptions, pEVentContext,
	  dMessageInfo, pErr)
	  Sdf_st_callObject 	*pCallObj;
	  SipMessage			*pMessage;
	  Sdf_st_transportInfo 	*pDestInfo;
	  Sdf_ty_socketId 		dSockfd;
	  SipOptions 			dOptions;
	  Sdf_st_eventContext	*pEventContext;
	  Sdf_st_msgInfo		dMessageInfo;
	  Sdf_st_error 			*pErr;
#endif
{
	 en_SipMessageType		dReqRespType = dMessageInfo.dReqRespType;
	 Sdf_ty_messageType		dMethodType = dMessageInfo.dMsgType;
	 Sdf_ty_u16bit			dRespCode = dMessageInfo.dResponse;

	if (pDestInfo->dScheme == Sdf_en_protoUdp)
	{
		if( (dReqRespType == SipMessageRequest) && (dMethodType == Sdf_en_ack))
		{
			if(sdf_fn_uaInsertIntoRemoteRetransList(pCallObj, pDestInfo, \
				dSockfd, pMessage,dOptions, pEventContext,\
				pErr)== Sdf_co_fail)
			{		
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_retransListError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaPopulateRREntry( ): "
					"Error in inserting ACK (UDP) into remote "
					"retransmision list",pErr);
#endif
				pErr->errCode=Sdf_en_retransListError;
				return Sdf_co_fail;
			}	
		}
		else if(dReqRespType == SipMessageResponse)
		{
			/* Responses sent on UDP */
			if((dRespCode>=100) && (dRespCode<300))
			{
				/* 
				 * For 1xx and 2xx responses the previous entries from the
				 * remote retransmission hash table should be removed. this is
				 * because,if the request is retransmitted, the latest
				 * response needs to be sent back
				 */
				Sdf_st_remoteRetransKey *pKey = Sdf_co_null; 
				Sdf_st_hash				*pHash = Sdf_co_null;

				/* Get the remote retransmission hash to operate upon */
#ifdef SDF_APP_CONTROLLED_REMOTE_RETX
				if (sdf_cbk_uaGetRemoteRetransHashTable(&pHash, pEventContext,\
					pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callBackError, \
						(Sdf_ty_s8bit *)"sdf_fn_uaPopulateRREntry(): "
						"Callback sdf_cbk_uaGetRemoteRetransHashTable failed", \
						pErr);
#endif
					pErr->errCode=Sdf_en_callBackError;
					return Sdf_co_fail;
				}
#else
				pHash = &(pGlbToolkitData->dRemoteRetransHash);
#endif
				
				/*
				 * If the application did not return any hash, then don't
				 * try to remove a hash entry etc. Simply carry on.
				 */
				if (pHash != Sdf_co_null)
				{
					/* Initialize the Retrans Key */
					if(sdf_ivk_uaInitRemoteRetransKey(&pKey, \
									pErr)==Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_retransListError,\
							(Sdf_ty_s8bit *)"sdf_fn_uaPopulateRREntry( ): "
							"Error in Initializinig RemoteRetrans Key",pErr);
#endif
						pErr->errCode=Sdf_en_retransListError;
						return Sdf_co_fail;
					}

					/*
					 * Generate a key to be used to index into RemoteRetrans
					 * hash. 
					 */

					if(sdf_fn_uaGenerateRetransKey(pMessage, pKey, \
							pErr)	== Sdf_co_fail)
					{
						(void)sdf_fn_uaFreeRemoteRetransKey(pKey);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_retransListError,\
							(Sdf_ty_s8bit *)"sdf_fn_uaPopulateRREntry( ): "
							"Error in Generating RemoteRetrans Key",pErr);
#endif
						pErr->errCode=Sdf_en_retransListError;
						return Sdf_co_fail;
					}

					/*
					 * Change the Message type.This is because 
					 * CompareRemoteRetransKey() checks the message type.
					 * It returns a failure if the message types are same.
					 */
					pKey->dMessageType=SipMessageAny;

					/*Should Remove any previous entries in the hash*/
					/*w534*/ (void)sdf_ivk_uaHashRemove(pHash, (Sdf_ty_pvoid)pKey);

					/*free the Key*/
					(void)sdf_fn_uaFreeRemoteRetransKey(pKey);

				} /* End of "if hash is not NULL */				
			} /* End of response code check */

			if(sdf_fn_uaInsertIntoRemoteRetransList(pCallObj, pDestInfo, \
				dSockfd, pMessage, dOptions, pEventContext,\
				pErr)== Sdf_co_fail)
			{		
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_retransListError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaPopulateRREntry( ): "
					"Error in inserting SIP response (UDP) into remote "
					"retransmision list",pErr);
#endif
				pErr->errCode=Sdf_en_retransListError;
				return Sdf_co_fail;
			}	
		} /* End of else block handling Responses sent on UDP */
	}
#ifdef SDF_TLS
	else if((pDestInfo->dScheme ==  Sdf_en_protoTcp) || \
			(pDestInfo->dScheme == Sdf_en_protoTls))
#else
	else if(pDestInfo->dScheme == Sdf_en_protoTcp)
#endif
	{
		/* 
		 * In case of TCP keep ACK'S in the remoteRetransList since final 
		 * responses to INVITE are retransmitted in TCP
		 */
		if((dReqRespType == SipMessageRequest) && (dMethodType == Sdf_en_ack))
		{
			if(sdf_fn_uaInsertIntoRemoteRetransList(pCallObj, \
				pDestInfo, dSockfd, pMessage, dOptions, pEventContext,\
				pErr)== Sdf_co_fail)
			{		
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_retransListError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaPopulateRREntry( ):" \
					"error in inserting into remote retransmision list",pErr);
#endif
				pErr->errCode=Sdf_en_retransListError;
				return Sdf_co_fail;
			}	
		} /* End of if SipMessageRequest */
	} /* End of else block handling insertion of TCP messages into RRlist */
	return Sdf_co_success;
}
#endif


/*****************************************************************************
** FUNCTION: sdf_fn_uaValidateViaAddr
**
** DESCRIPTION: This function checks whether the received Via header 
**				corresponds to the address specified in its profile or not.
*****************************************************************************/

Sdf_ty_messageValidity sdf_fn_uaValidateViaAddr
#ifdef ANSI_PROTO
	( Sdf_st_callObject 	*pCallObj,
	  SipMessage			*pMessage,
	  Sdf_st_error 			*pErr)
#else
	( pCallObj, pMessage, pErr)
	  Sdf_st_callObject 	*pCallObj;
	  SipMessage			*pMessage;
	  Sdf_st_error 			*pErr;
#endif
{
	/* 
	 * If the IP/port in the topmost Via header does not matches 
	 * with that in the user profile's transport information,
	 * the message should be discarded 
	 */

	SipHeader 		dTempHeader;
	Sdf_ty_s8bit 	*pSentBy = Sdf_co_null;
	Sdf_ty_s8bit 	*pHostAddress = Sdf_co_null, *pHostPort = Sdf_co_null;
	Sdf_ty_u16bit 	dPort;
#ifdef SDF_LINT
    Sdf_ty_s8bit 	dTempSentby[Sdf_co_characterBufferSize] = {'\0',};
#else    
    Sdf_ty_s8bit 	dTempSentby[Sdf_co_characterBufferSize];
#endif    
	Sdf_st_initData *pInitData = Sdf_co_null;
	Sdf_st_transportInfo *pTransport = Sdf_co_null;
/*Change from UATK 2.1 Start 4.0 fix*/
	Sdf_ty_s8bit	*pTemp = Sdf_co_null;
/*Change from UATK 2.1 End*/	

	if (sip_getHeaderAtIndex(pMessage, SipHdrTypeVia,\
		&dTempHeader, 0, (SipError*)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaValidateViaAddr(): "
			"Error Accessing Via Header From Sip Message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_failed;
	}

	if (sip_getSentByFromViaHdr(&dTempHeader, &pSentBy, (SipError*)\
		&(pErr->stkErrCode)) == SipFail)
	{		
		sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerManipulationError, \
		(Sdf_ty_s8bit *)"sdf_fn_uaValidateViaAddr(): "
		"Error accessing SentBy from Via Header in Sip Message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_failed;
	}	

	Sdf_mc_strcpy(dTempSentby, pSentBy);	

	/* 
	 * Take care of '[',']' while extracting the host from IPV6 reference. 
	 */
	if(Sdf_mc_strstr(dTempSentby,"[") !=Sdf_co_null)
	{
		/* Strip the '[',']' before resolving the address. */
		pHostPort = Sdf_mc_strstr(dTempSentby, "]:");
		pHostAddress = dTempSentby+1;
		if(pHostPort !=Sdf_co_null)
		{
			pHostPort = pHostPort +2;
			pHostAddress[Sdf_mc_strlen(dTempSentby)-\
				Sdf_mc_strlen(pHostPort)-3] = '\0';
		}
		else
		{
			pHostAddress[Sdf_mc_strlen(dTempSentby)-2] = '\0';
		}
	}
	else
	{
		pHostAddress = Sdf_mc_strtokr(dTempSentby, ":",&pHostPort);
	}

	if (pHostPort != Sdf_co_null)
		dPort = Sdf_mc_atoi(pHostPort);
	else
	{
#ifdef SDF_TLS
		Sdf_ty_s8bit *pTempViaSentProto = Sdf_co_null;

		if (sip_getSentProtocolFromViaHdr(&dTempHeader,
			&pTempViaSentProto, (SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaValidateViaAddr(): "
				"Failed to get sent protocol from Via header",pErr);
#endif			
			pErr->errCode=Sdf_en_headerManipulationError;
			sip_freeSipHeader(&dTempHeader);
			return Sdf_en_failed;
		}

		if (Sdf_mc_strstr(pTempViaSentProto, "TLS") != Sdf_co_null)
			dPort = Sdf_mc_defaultSipSPort;
		else
#endif /* SDF_TLS */
		    dPort = Sdf_mc_defaultSipPort;
	}

	if(sdf_ivk_uaGetInitDataFromCallObject(pCallObj, &pInitData,\
		pErr)==Sdf_co_fail)
	{
		sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaValidateViaAddr(): "
			"Getting InitData From Callobject failed",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_en_failed;
	}	

	if (sdf_ivk_uaGetTransportInfoFromInitData(pInitData, \
		&pTransport, pErr) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeInitData(pInitData);
		sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaValidateViaAddr(): "
			"Getting TransportInfo from InitData failed",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_en_failed;
	}

	/* 
	 * If the IP/port in the topmost Via header does not matches 
	 * with that in the user profile's transport information,
	 * the message should be discarded 
	 */
/*Change from UATK 2.1 Start 4.0 fix*/
	pTemp = Sdf_mc_strstr(pTransport->pIp,"[");
	if((pTemp != Sdf_co_null) && (Sdf_co_null != pHostAddress))
	{
		sdf_fn_uaSprintf(pTemp,"[%s]",pHostAddress);
		Sdf_mc_strcpy(pHostAddress,pTemp);
	}
/*Change from UATK 2.1 End*/	

	if( (Sdf_mc_strcmp(pHostAddress, pTransport->pIp) != 0) || \
		(dPort != pTransport->dPort) )
	{
		(void)sdf_ivk_uaFreeInitData(pInitData);
		(void)sdf_ivk_uaFreeTransportInfo(pTransport);
		sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_addressMismatchInVia, \
		(Sdf_ty_s8bit *)"sdf_fn_uaValidateViaAddr(): "
		"Topmost Via header in incoming SIP Message has IP/port "
		"other than local IP/port. Spurious message. Discarded",pErr);
#endif
		pErr->errCode = Sdf_en_addressMismatchInVia;
		return Sdf_en_discarded;
	}
	(void)sdf_ivk_uaFreeInitData(pInitData);
	(void)sdf_ivk_uaFreeTransportInfo(pTransport);
	sip_freeSipHeader(&dTempHeader);

	return Sdf_en_valid;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaValidateResponseState
**
** DESCRIPTION:  This function is called from Check Message Validity. 
**				This does validations for incoming response messages
*****************************************************************************/

Sdf_ty_messageValidity sdf_fn_uaValidateResponseState
#ifdef ANSI_PROTO
	( Sdf_st_callObject 	*pCallObj,
	  SipMessage			*pMessage,
	  Sdf_st_msgInfo		dMessageInfo,
	  Sdf_st_error 			*pErr)
#else
	( pCallObj, pMessage, dMessageInfo, pErr)
	  Sdf_st_callObject 	*pCallObj;
	  SipMessage			*pMessage;
	  Sdf_st_msgInfo		dMessageInfo;
	  Sdf_st_error 			*pErr;
#endif
{
	Sdf_ty_u16bit		dRespCode = dMessageInfo.dResponse;
	Sdf_ty_messageType	dMsgType  = dMessageInfo.dMsgType;
	Sdf_ty_messageType 	dRespType;
	Sdf_st_overlapTransInfo	*pOverlapTransInfo = Sdf_co_null;
	
	if (dRespCode >= 200)
	{
		if ((dMsgType == Sdf_en_invite) || (dMsgType == Sdf_en_ack))
		{
			if (dRespCode >= 300)
				dRespType = Sdf_en_finalFailure;
			else
				dRespType = Sdf_en_finalSuccess;

		
			if (dCsmNxtStateOnMsgRecd[dRespType]\
				[pCallObj->pCallInfo->dState].dNextState == \
				Sdf_en_invalidStateTransition)
			{
				pErr->errCode = Sdf_en_callStateError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callStateError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateResponseState(): "
					"Failed in State Validation", pErr);
#endif
				return Sdf_en_invalid;
			}	
		}
		/* Final response to CANCEL will always be valid as there is 
		 * no state transition when CANCEL was sent out. The result of 
		 * the cancellation will be decided based on the response to 
		 * the original INVITE 
		 */
		else if ( (dMsgType == Sdf_en_cancel) || (dMsgType == Sdf_en_bye))
		{
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_en_valid;
		}
#ifdef SIP_RPR
		else if ((dMsgType == Sdf_en_prack) || (dMsgType == Sdf_en_comet))
#else
		else if ((dMsgType == Sdf_en_comet))
#endif

		{
			/*
			 *	Final response for a PRACK or COMET (overlapping
			 *	transactions) to be handled here.
			 */

			/*
			 * Try to retrieve an overlapTransInfo for the 200 OK
			 * recd. If it succeeds, then we have finished the 
			 * following checks:
			 * 		a. 	From, To, call-id header checks performed 
			 * 			outside	(from where the function is invoked)
			 *		b. 	Check if the Cseq of the 200 OK is same as
			 *			a PRACK sent out. (this is done in the API
			 *			sdf_ivk_uaRetrieveMatchingOverlapTransInfo() )
			 * 
			 *		Only need to check state for the RPR 
			 *		transaction here.
			 */
			 if (sdf_ivk_uaRetrieveMatchingOverlapTransInfo(\
				pMessage, pCallObj, &pOverlapTransInfo, Sdf_en_msgModeRecd, \
				pErr) == Sdf_co_fail)
			{
				if (pErr->errCode != Sdf_en_noExistError)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
						(Sdf_ty_s8bit*)"sdf_fn_uaValidateResponseState()"
						": Failed to get matching overlapTransInfo from"
						"CallObject", pErr);
#endif
					return Sdf_en_failed;
				}
				else
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_transpInfoAccessError,\
						(Sdf_ty_s8bit*)"sdf_fn_uaValidateResponseState()"
						": Invalid response received. No matching "
						"transaction for the final response received.",\
						pErr);
#endif
					pErr->errCode=Sdf_en_transpInfoAccessError;
					return Sdf_en_invalid;
				}
			}
			/*
			 * If a overlapping transaction matching the 
			 * response was not recd; it is a invalid response
			 */
			if (pOverlapTransInfo != Sdf_co_null)
			{
				if (pOverlapTransInfo->dState !=  Sdf_en_overlapTransactionSent)
				{
					(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
					pErr->errCode = Sdf_en_noUaError;
					return Sdf_en_invalid;
				}
			}
			else
			{
				(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
				pErr->errCode = Sdf_en_noUaError;
				return Sdf_en_invalid;
			}
			(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
		}
		else
		{
			/*
			 * All other transactions (INFO/OPTIONS/REGISTER/SUBSCRIBE/
			 * NOTIFY) map in here.
			 */
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_en_valid;
		}
	} /* End of if block for respcode >= 200 */
	else
	{
#ifdef SIP_RPR	
		/* 
		 * It is a provisional response.
		 * If the response is a RPR, we need to check if the Rseq of the
		 * RPR is just one greater than the previously received RPR.
		 * If not drop this RPR.
		 */
		Sdf_ty_u32bit dRespNum;

		SipHeader dHeader;
		if (sdf_fn_uaIsMessageRPR(pMessage, pErr) == 0)
		{
			/* -------
			 * 	NOTE
			 * -------
			 * If the message is a RPR and an overlapping
			 * transaction corresponding to this RPR exists, then
			 * we need to return remoteRetransmission here. This so
			 * because this RPR is a local retrasmission from the
			 * other end and can be ignored.
			 */
			if (sdf_ivk_uaRetrieveMatchingOverlapTransInfo(pMessage, pCallObj, \
				&pOverlapTransInfo, Sdf_en_msgModeRecd, pErr) == Sdf_co_fail)
			{
				if (pErr->errCode != Sdf_en_noExistError)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
						(Sdf_ty_s8bit*)"sdf_fn_uaValidateResponseState()"
						": Failed to get matching overlapTransInfo from"
						"CallObject", pErr);
#endif
					return Sdf_en_failed;
				}
			}
			else
			{
				/*
				 * There exists an Overlapping transaction
				 * structure for this RPR. Implies that it is
				 * already being processed. Reject this RPR recd as
				 * a retransmission.
				 */
				(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
				pErr->errCode = Sdf_en_noUaError;
				return Sdf_en_discarded;
			}

			if (sip_getHeader(pMessage, SipHdrTypeRSeq, &dHeader, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateResponseState(): "
					"Failed to get rseq header from SIP message",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}
			if (sip_rpr_getRespNumFromRSeqHdr(&dHeader,\
				&dRespNum, (SipError *)&(pErr->stkErrCode)) == SipFail)
			{
				sip_freeSipHeader(&dHeader);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaValidateResponseState(): "
					"Failed to get rseq num from the Rseq header",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}
			sip_freeSipHeader(&dHeader);

			/* 
			 * If the Rseq has increased by more than 1 discard. 
			 * However if this is the first RPR, don't do a strict 
			 * check for respnum.
			 * If dRemoteRseq.dRseq is 0, this is the first RPR.
			 */
			if (pCallObj->pCommonInfo->dRemoteRseq.dRseq != 0)
			{
				if ((pCallObj->pCommonInfo->dRemoteRseq.dRseq+1) != dRespNum)
				{
					pErr->errCode = Sdf_en_noUaError;
					return Sdf_en_invalid;
				}
			}
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_en_valid;
		}
		else
#endif /* SIP_RPR */		
		{
			if (((pCallObj->pCallInfo->dState == Sdf_en_callTerminated) ||
    			 (pCallObj->pCallInfo->dState == Sdf_en_idle))&& \
				 ((dMsgType == Sdf_en_invite) || (dMsgType == Sdf_en_bye)))
			{
				pErr->errCode = Sdf_en_noUaError;
				return Sdf_en_discarded;
			}
		}
	}
	return Sdf_en_valid;
}

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetMatchingOverlapTransaction
 **
 ** DESCRIPTION: This API returns the overlap transaction structure with 
 ** 			 the method Type passed.
 **
 ******************************************************************************/
Sdf_ty_retVal	sdf_fn_uaGetMatchingOverlapTransaction
#ifdef ANSI_PROTO
	(Sdf_st_callObject 		*pCallObj,
	Sdf_st_overlapTransInfo **ppTransInfo,
	Sdf_ty_messageType 		dMsgType, 
	Sdf_st_error 			*pError)
#else	
	(pCallObj, ppTransInfo, dMsgType, pError)
	Sdf_st_callObject 		*pCallObj;
	Sdf_st_overlapTransInfo **ppTransInfo;
	Sdf_ty_messageType 		dMsgType;
	Sdf_st_error 			*pError;
#endif
{
	Sdf_st_listIterator dListIterator;

	/*
	 * Now iterate through call objects's OverlapTransInfo elements
     * and get the relevant one
     */
	(void)sdf_listInitIterator(&(pCallObj->slOverlapTransInfo), \
		&dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		*ppTransInfo = (Sdf_st_overlapTransInfo*)\
							(dListIterator.pCurrentElement->pData);

		if ((*ppTransInfo)->dMsgType == dMsgType)
		{
			HSS_LOCKEDINCREF((*ppTransInfo)->dRefCount);
			pError->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}
		(void)sdf_listNext(&dListIterator, pError);
	}

	*ppTransInfo = Sdf_co_null;
	pError->errCode = Sdf_en_noExistError;
	return Sdf_co_fail;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaCompareFullURI
**
** DESCRIPTION: This function is called from Compare CallObject Keys function.
**				This compares the two headers with all header param 
**				comparisions.
*****************************************************************************/

Sdf_ty_retVal sdf_fn_uaCompareFullURI
#ifdef ANSI_PROTO
	( SipHeader		 		*pNewHeader,
	  SipHeader				*pExistHeader,
	  Sdf_ty_bool			dTagMatched,
	  Sdf_ty_callLegMatch 	*pCallLegMatch,
	  Sdf_st_error 			*pError)
#else
	( pNewHeader, pExistHeader, dTagMatched,  pCallLegMatch, pError)
	  SipHeader		 		*pNewHeader;
	  SipHeader				*pExistHeader;
	  Sdf_ty_bool			dIsTagMatched;
	  Sdf_ty_callLegMatch 	*pCallLegMatch;
	  Sdf_st_error 			*pError;
#endif
{
	SipAddrSpec		*pAddrSpec1 = Sdf_co_null;
	SipAddrSpec		*pAddrSpec2 = Sdf_co_null;
	Sdf_ty_s8bit 	dTemp=0;
        dTagMatched=dTagMatched;

	/* Retrieve the AddrSpec from the headers */
	if (pNewHeader->dType == SipHdrTypeFrom)
	{
		if (sip_getAddrSpecFromFromHdr(pNewHeader, &pAddrSpec1, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareFullURI(): "
				"Failed to get AddrSpec from FromHdr",pError);
#endif			
			return Sdf_co_fail;
		}
	}
	else
	{
		if (sip_getAddrSpecFromToHdr(pNewHeader, &pAddrSpec1, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareFullURI(): "
				"Failed to get AddrSpec from ToHdr",pError);
#endif			
			return Sdf_co_fail;
		}
	}

	if (pExistHeader->dType == SipHdrTypeFrom)
	{
		if (sip_getAddrSpecFromFromHdr(pExistHeader, &pAddrSpec2, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareFullURI(): "
				"Failed to get AddrSpec from FromHdr",pError);
#endif			
			return Sdf_co_fail;
		}
	}
	else
	{
		if (sip_getAddrSpecFromToHdr(pExistHeader, &pAddrSpec2, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCompareFullURI(): "
				"Failed to get AddrSpec from ToHdr",pError);
#endif			
			return Sdf_co_fail;
		}
	}

	dTemp = sdf_fn_uaCompareSipAddrSpec(pAddrSpec1, pAddrSpec2, Sdf_co_true, \
		Sdf_co_false, pError);

	/* Free local variables */
	sip_freeSipAddrSpec(pAddrSpec1);
	sip_freeSipAddrSpec(pAddrSpec2);

	
	if ( (dTemp == 0) || (dTemp == 2) )
	{
		/* 
		 * AddrSpec's match. May be overlap dialing.
		 * Check if header param's match. Also check if the 
		 * tag's match 
		 */
		Sdf_ty_u32bit dHeaderParamListSize1 = 0, dHeaderParamListSize2 = 0;
		Sdf_ty_u32bit dIndex = 0, dJindex = 0;

		/*
		 * Start comparing header parameters. 
		 *
		 * NOTE :
		 * -> Only header parameters that are present in both headers
		 * 		are taken for the comparision.
		 * -> Paramnames are case insensitive
		 * -> Paramvalues are case insensitive except quoted string
		 * 		paramvalues, which are case sensitive
		 * -> Characters other than those in the "reserved" and 
		 * 		"unsafe" sets are equivalent to their "% HEX HEX" encoding.
		 */
		if (pNewHeader->dType == SipHdrTypeFrom)
			/*w534*/ (void)sip_getExtensionParamCountFromFromHdr(pNewHeader, \
				&dHeaderParamListSize1, (SipError *)&(pError->stkErrCode));
		else	
			/*w534*/ (void)sip_getExtensionParamCountFromToHdr(pNewHeader, \
				&dHeaderParamListSize1, (SipError *)&(pError->stkErrCode));

		if (pExistHeader->dType == SipHdrTypeFrom)	
			/*w534*/ (void)sip_getExtensionParamCountFromFromHdr(pExistHeader, \
				&dHeaderParamListSize2, (SipError *)&(pError->stkErrCode));
		else	
			/*w534*/ (void)sip_getExtensionParamCountFromToHdr(pExistHeader, \
				&dHeaderParamListSize2, (SipError *)&(pError->stkErrCode));

		/*
		 * Iterate through each param of pUrl1
		 */
		for (dIndex = 0; dIndex < dHeaderParamListSize1; dIndex++)
		{
			SipParam *pParam1 = Sdf_co_null;
			Sdf_ty_s8bit *pName1 = Sdf_co_null, *pValue1 = Sdf_co_null;
			Sdf_ty_s8bit dHexConvertedName1[Sdf_co_smallBufferSize];
			Sdf_ty_s8bit dHexConvertedValue1[Sdf_co_smallBufferSize];

			if (pNewHeader->dType == SipHdrTypeFrom)
			{
				/*w534*/ (void)sip_getExtensionParamAtIndexFromFromHdr(pNewHeader, &pParam1, \
					dIndex, (SipError *)&(pError->stkErrCode));
			}
			else
			{
				/*w534*/ (void)sip_getExtensionParamAtIndexFromToHdr(pNewHeader, &pParam1, \
					dIndex, (SipError *)&(pError->stkErrCode));
			}

			/*w534*/ (void)sip_getNameFromSipParam(pParam1, &pName1, \
				(SipError *)&(pError->stkErrCode));
			/* Since MicroSip also returns tag as extension param, ignore 
			 * it as it need not be compared */
			if(Sdf_mc_strcasecmp(pName1,"tag") == 0)
			{
				sip_freeSipParam(pParam1);
				continue;
			}

			/*w534*/ (void)sip_getValueAtIndexFromSipParam(pParam1, &pValue1, \
				0, (SipError *)&(pError->stkErrCode));

             /*CSR 1-7574892 change start*/
            if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pName1, dHexConvertedName1,\
                        Sdf_co_smallBufferSize))
            {
                sip_freeSipParam(pParam1);

                return Sdf_co_fail;
            }
            if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pValue1, dHexConvertedValue1,\
                        Sdf_co_smallBufferSize))
            {
                sip_freeSipParam(pParam1);
                return Sdf_co_fail;

            }
            /*CSR 1-7574892 change end*/

			/*
			 * Now iterate through each param of pUrl2 searching
			 * for a param with the same name
			 */
			for (dJindex = 0; dJindex < dHeaderParamListSize2; dJindex++)
			{
				SipParam *pParam2 = Sdf_co_null;
				Sdf_ty_s8bit *pName2 = Sdf_co_null, *pValue2 = Sdf_co_null;
				Sdf_ty_s8bit dHexConvertedName2[Sdf_co_smallBufferSize];
				Sdf_ty_s8bit dHexConvertedValue2[Sdf_co_smallBufferSize];
				
				if (pExistHeader->dType == SipHdrTypeFrom)
				{
					/*w534*/ (void)sip_getExtensionParamAtIndexFromFromHdr(pExistHeader, \
						&pParam2, dJindex, (SipError *)&(pError->stkErrCode));
				}
				else
				{
					/*w534*/ (void)sip_getExtensionParamAtIndexFromToHdr(pExistHeader, \
						&pParam2, dJindex, (SipError *)&(pError->stkErrCode));
				}

				/*w534*/ (void)sip_getNameFromSipParam(pParam2, &pName2, \
					(SipError *)&(pError->stkErrCode));

                 /*CSR 1-7574892 change start*/
                if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pName2, dHexConvertedName2,\
                            Sdf_co_smallBufferSize))
                {
                    sip_freeSipParam(pParam1);
                    sip_freeSipParam(pParam2);

                    return Sdf_co_fail;
                }
                /*CSR 1-7574892 change end*/

				if (Sdf_mc_strcasecmp(dHexConvertedName1, \
					dHexConvertedName2) == 0)
				{
					/*
					 * Matching names found
					 */
					/*w534*/ (void)sip_getValueAtIndexFromSipParam(pParam2, &pValue2, \
						0, (SipError *)&(pError->stkErrCode));

                     /*CSR 1-7574892 change start*/
                    if (Sdf_co_fail == sdf_fn_uaConvertHexEncodedString(pValue2, \
                                dHexConvertedValue2,Sdf_co_smallBufferSize))
                    {

                        sip_freeSipParam(pParam1);
                        sip_freeSipParam(pParam2);

                        return Sdf_co_fail;
                    }
                    /*CSR 1-7574892 change end*/

					if ( (dHexConvertedValue1[0] != '\0') && \
						(dHexConvertedValue2[0] != '\0') )
					{
						/*
						 * If the values are quoted strings a case sensitive
						 * match is done, else a case insensitive match is
						 * done
						 */
						if (dHexConvertedValue1[0] == '"')
						{
							if (Sdf_mc_strcmp(dHexConvertedValue1, \
								dHexConvertedValue2) == 0)
							{
								/*
								 * Param name and param values match. Break
								 * out and pick up the next param
								 */
								sip_freeSipParam(pParam2);
								break;
							}
							else
							{
								/*
								 * A url-parameter name matched, but the values
								 * did not match. Return Sdf_en_notMatching
								 */
								sip_freeSipParam(pParam1);
								sip_freeSipParam(pParam2);

								*pCallLegMatch = Sdf_en_notMatching;
								
								return Sdf_co_success;
							}
						}
						else
						{
							if (Sdf_mc_strcasecmp(dHexConvertedValue1, \
								dHexConvertedValue2) == 0)
							{
								/*
								 * Param name and param values match. Break
								 * out and pick up the next param
								 */
								sip_freeSipParam(pParam2);
								break;
							}
							else
							{
								/*
								 * A url-parameter name matched, but the values
								 * did not match. Return Sdf_en_notMatching
								 */
								sip_freeSipParam(pParam1);
								sip_freeSipParam(pParam2);

								*pCallLegMatch = Sdf_en_notMatching;
								
								return Sdf_co_success;
							}
						}
					}
					else if ( (dHexConvertedValue1[0] == '\0') && \
						(dHexConvertedValue2[0] == '\0') )
					{
						/*
						 * Both param values are NULL. Break out
						 * and pick up the next param
						 */
						sip_freeSipParam(pParam2);
						break;
					}
					else
					{
						/*
						 * A url-parameter name matched, but the values
						 * did not match. Return Sdf_en_notMatching
						 */
						sip_freeSipParam(pParam1);
						sip_freeSipParam(pParam2);

						*pCallLegMatch = Sdf_en_notMatching;
						return Sdf_co_success;
					}
				}
				/* Free the local reference */
				sip_freeSipParam(pParam2);
			}
			/* Free the local reference */
			sip_freeSipParam(pParam1);
		}

		if (dTemp == 2)
			*pCallLegMatch = Sdf_en_overlapDialing;
		/*else
		{	
			if( dTagMatched == Sdf_co_true )
				*pCallLegMatch = Sdf_en_sameCallLeg;
			else
				*pCallLegMatch = Sdf_en_differentCallLeg;
		}*/
	}
	else
	{
		/* AddrSpec itself does not match */
		*pCallLegMatch = Sdf_en_notMatching;
		return Sdf_co_success;
	}
	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaStoreRequireHdr
**
** DESCRIPTION: This function stores the Require and Proxy Headers into
**				the Call Objects Persistant header List with proper 
**				correlation string.  Before adding into persistent list it
**				also deletes the previous entry
*****************************************************************************/

Sdf_ty_retVal sdf_fn_uaStoreRequireHdr
#ifdef ANSI_PROTO
	( Sdf_st_callObject		*pCallObj,
	  SipMessage			*pMessage,
	  Sdf_st_error 			*pError)
#else
	( pCallObj, pMessage, pError)
	  Sdf_st_callObject		*pCallObj;
	  SipMessage			*pMessage;
	  Sdf_st_error 			*pError;
#endif
{
	Sdf_ty_u32bit		dReqCount = 0;
	Sdf_ty_u32bit		dPxyReqCount = 0;
	Sdf_st_headerList 	*pRequireList = Sdf_co_null;
	Sdf_st_headerList 	*pPxyRequireList = Sdf_co_null;
	Sdf_ty_u32bit		dIndex=0;
	Sdf_st_error		dLocalErr;
	(void)pRequireList;
	(void)dPxyReqCount;
	(void)pPxyRequireList;
	/*w534*/ (void)sip_getHeaderCount(pMessage, SipHdrTypeRequire, &dReqCount, \
			(SipError *)&(pError->stkErrCode));

	if( dReqCount != 0 )
	{
		if( Sdf_co_fail == sdf_ivk_uaInitHeaderList(&pRequireList, 
			SipHdrTypeRequire, (Sdf_ty_s8bit *)"uat-Require", pError))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaStoreRequireHdr():  "
				"Failed to Initialize Require HeaderList",pError);
#endif
			pError->errCode = Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}
		
		for( dIndex=0; dIndex<dReqCount; dIndex++)
		{
			SipHeader		*pSrcHeader;

			if(sip_initSipHeader(&pSrcHeader, SipHdrTypeAny, \
				(SipError *)&(pError->stkErrCode)) == SipFail )
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaStoreRequireHdr():  "
					"Failed to Initialize Require HeaderList",pError);
#endif
				(void)sdf_ivk_uaFreeHeaderList(pRequireList);
				pError->errCode = Sdf_en_noMemoryError;
				return Sdf_co_fail;
			}

			if( SipFail == sip_getHeaderAtIndex(pMessage, \
				SipHdrTypeRequire, pSrcHeader, dIndex,\
				(SipError *)&(pError->stkErrCode)))
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaStoreRequireHdr():  "
					"Failed to Get Require Header at Index",pError);
#endif
				(void)sdf_ivk_uaFreeHeaderList(pRequireList);
				sip_freeSipHeader (pSrcHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				   (Sdf_ty_pvoid *)&pSrcHeader, &dLocalErr);
				return Sdf_co_fail;
			}
			if( Sdf_co_fail == sdf_listAppend(&(pRequireList->slHeaders),\
						pSrcHeader, pError))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaStoreRequireHdr():  "
					"Failed to Insert Require Header to List",pError);
#endif
				(void)sdf_ivk_uaFreeHeaderList(pRequireList);
				sip_freeSipHeader (pSrcHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				   (Sdf_ty_pvoid *)&pSrcHeader, &dLocalErr);
				return Sdf_co_fail;
			}
		}

		/* Delete the previous entry and add new list */
		/*w534*/ (void)sdf_ivk_uaDeleteHeaderListByCorrelation( \
			&(pCallObj->slPersistentHeaders), "uat-Require",pError);

		if(Sdf_co_fail == sdf_listAppend(&(pCallObj->slPersistentHeaders), \
			(Sdf_ty_pvoid)pRequireList, pError))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaStoreRequireHdr():  "
				"Failed to Insert Require HeaderList to CallObject",pError);
#endif
			(void)sdf_ivk_uaFreeHeaderList(pRequireList);
			return Sdf_co_fail;
		}
	}

	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaGetAllowHeaderList
**
** DESCRIPTION: This function returns the ALLOW header list.
**				The list is extracted from profile(InitData), if present,
**				Otherwise UATK sets all the methods it supports,
**				by default, into the header list.
** PARAMETERS:
**	pCallObj(IN)			: CallObject to operate upon.
**	pInitData(IN)			: Profile that [may] contain ALLOW header
**	ppAllowHdrList(OUT)		: The extracted/created ALLOW header
**	pErr(OUT)				: Error structure in case the function fails
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetAllowHeaderList
	(Sdf_st_callObject	*pCallObj, 
	 Sdf_st_initData 	*pInitData, 
	 Sdf_st_headerList 	**ppAllowHdrList,
	 Sdf_st_error 		*pErr)
{
	Sdf_ty_bool 	dIsAllowHdrPresent = Sdf_co_false;
#ifdef SDF_UAB2B
	Sdf_ty_bool     dIsPRACKPresent = Sdf_co_false;
	Sdf_ty_bool     dIsRequirePresent = Sdf_co_false;
	Sdf_ty_bool     dIsSupportedPresent = Sdf_co_false;
	Sdf_ty_bool     dFoundSupportedTags = Sdf_co_false;
	Sdf_ty_bool     dFound100rel = Sdf_co_false;
	Sdf_ty_u32bit   dSupportedHdrListSize =0;
	Sdf_ty_u32bit 	dRequireHdrListSize =0;
    Sdf_ty_s8bit    *pHdrTempStr = Sdf_co_null;
#endif
	Sdf_ty_u32bit 	dAllowHdrListSize =0;
	Sdf_ty_u32bit	dIndex=0;
	Sdf_ty_s8bit 	*pTempStr = Sdf_co_null;
	Sdf_ty_s8bit	*pHdrStr  = Sdf_co_null;
	Sdf_st_configExtraHeaders *pTemp = Sdf_co_null;
	SipHeader 		*pHdr = Sdf_co_null;
	Sdf_st_listIterator dListIterator;
	/* 
	 * Array for methods INVITE, ACK, CANCEL, BYE and OPTIONS respectively. 
	 * True means that the method is missing in the Allow list of the user
	 * profile and hence the UATK should insert these automatically.
	 */
	Sdf_ty_bool dMethodsMissingArray[5] = {Sdf_co_true, Sdf_co_true, \
	   									   Sdf_co_true, Sdf_co_true, \
										   Sdf_co_true};
	/* Allow header absent in profile, so use the default */
	Sdf_ty_s8bit *dSupported[Sdf_co_noOfMethodsSupported] = {
			(Sdf_ty_s8bit *)"INVITE",
			(Sdf_ty_s8bit *) "BYE",
			(Sdf_ty_s8bit *) "CANCEL",
			(Sdf_ty_s8bit *) "ACK",
			(Sdf_ty_s8bit *)"INFO", 
#ifdef SIP_RPR		
			(Sdf_ty_s8bit *)"PRACK",
#endif			
			(Sdf_ty_s8bit *)"COMET",
			(Sdf_ty_s8bit *)"OPTIONS",
#ifdef SIP_IMPP
			(Sdf_ty_s8bit *)"SUBSCRIBE",
			(Sdf_ty_s8bit *)"NOTIFY",
#endif
#ifdef SDF_IM
			(Sdf_ty_s8bit *)"MESSAGE",
#endif						
#ifdef SDF_REFER
			(Sdf_ty_s8bit *)"REFER",
#endif
			(Sdf_ty_s8bit *)"REGISTER",
			(Sdf_ty_s8bit *)"UPDATE"
		};
	
	*ppAllowHdrList = Sdf_co_null;
	(void)pCallObj;
	
	if (pInitData != Sdf_co_null)
	{
		/* If a User profile is provided,  look for ALLOW header */
		
		(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList),\
				&dListIterator, pErr);
		/* Iterate through profile & check if 
		 * ALLOW header is present or not */
		while(dListIterator.pCurrentElement != Sdf_co_null)
		{
			pTemp = (Sdf_st_configExtraHeaders *) \
							(dListIterator.pCurrentElement->pData);

			if(Sdf_mc_strcasecmp((Sdf_ty_s8bit *)"ALLOW", \
									pTemp->pParam->pName)==0)
			{
				/*
				 * Check if "addalways" xml tag is enabled or not.
				 * If it is disabled, add the default set supported by UATK
				 */
				if(Sdf_mc_strcasecmp((Sdf_ty_s8bit *)"yes", \
						(Sdf_ty_s8bit *)pTemp->pData) == 0)
					dIsAllowHdrPresent = Sdf_co_true;
				break;
			}
			(void)sdf_listNext(&dListIterator, pErr);
		}
	}
	
	/*
	 * Initialize a HeaderList structure to hold the 
	 * headers.
	 */
	if (sdf_ivk_uaInitHeaderList(ppAllowHdrList, SipHdrTypeAllow, \
				Sdf_co_null, pErr) ==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGetAllowHeaderList():  "
			"Failed to Initialize Allow HeaderList",pErr);
#endif
		return Sdf_co_fail;
	}

	/* 
	 * If Allow header is present in profile,
	 * extract the methods and add to header list
	 */
#ifdef SDF_LINT
	if((dIsAllowHdrPresent == Sdf_co_true) &&
		 (pTemp != Sdf_co_null))
#else	
	if(dIsAllowHdrPresent == Sdf_co_true)
#endif			
	{
        (void)sip_listSizeOf(&(pTemp->pParam->slValue), &dAllowHdrListSize, \
				(SipError*)&(pErr->stkErrCode));
	}
	else
	{
		dAllowHdrListSize = Sdf_co_noOfMethodsSupported;
	}
	
	for(dIndex=0; dIndex<dAllowHdrListSize; dIndex++)
	{
		if (sip_initSipHeader(&pHdr, SipHdrTypeAllow, \
					(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode=Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_fn_uaGetAllowHeaderList():"
				"Failed to Initialize SipHeader",pErr);
#endif
			(void)sdf_ivk_uaFreeHeaderList(*ppAllowHdrList);
			return Sdf_co_fail;
		}

#ifdef SDF_LINT
	if((dIsAllowHdrPresent == Sdf_co_true) &&
		 (pTemp != Sdf_co_null))
#else	
	if(dIsAllowHdrPresent == Sdf_co_true)
#endif
		{
            
            if (sip_listGetAt(&(pTemp->pParam->slValue), 
					dIndex,(Sdf_ty_pvoid *)&pTempStr,
					(SipError*)&(pErr->stkErrCode)) == SipFail)
			{
				sip_freeSipHeader(pHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHdr,pErr);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,
					Sdf_en_extraHeaderAccessError,\
					(Sdf_ty_s8bit*)"sdf_fn_uaGetAllowHeaderList():"
					"Failed to get value at index from header list",pErr);
#endif
				pErr->errCode = Sdf_en_extraHeaderAccessError;
				(void)sdf_ivk_uaFreeHeaderList(*ppAllowHdrList);
				return Sdf_co_fail;
			}

			Sdf_mc_strdup(pHdrStr,pTempStr);
		}
		else
			Sdf_mc_strdup(pHdrStr,dSupported[dIndex]);
		
		/*- Check if any of the basic methods are missing -*/
		if (Sdf_mc_strcmp(pHdrStr, "INVITE") == 0)
			dMethodsMissingArray[0] = Sdf_co_false;
		else if (Sdf_mc_strcmp(pHdrStr, "ACK") == 0)
			dMethodsMissingArray[1] = Sdf_co_false;
		else if (Sdf_mc_strcmp(pHdrStr, "OPTIONS") == 0)
			dMethodsMissingArray[2] = Sdf_co_false;
		else if (Sdf_mc_strcmp(pHdrStr, "CANCEL") == 0)
			dMethodsMissingArray[3] = Sdf_co_false;
		else if (Sdf_mc_strcmp(pHdrStr, "BYE") == 0)
			dMethodsMissingArray[4] = Sdf_co_false;
#ifdef SDF_UAB2B
		else if (Sdf_mc_strcmp(pHdrStr, "PRACK") == 0)
			dIsPRACKPresent = Sdf_co_true;
#endif

		if (sip_setMethodInAllowHdr(pHdr, pHdrStr,\
				(SipError *)&(pErr->stkErrCode))==SipFail)
		{
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHdr,pErr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHdrStr,pErr);
			(void)sdf_ivk_uaFreeHeaderList(*ppAllowHdrList);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaGetAllowHeaderList():"
				"Failed to set method in Allow Header", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		/*
		 * Add this header to the HeaderList
		 */
		if(sdf_listAppend(&((*ppAllowHdrList)->slHeaders), \
			(Sdf_ty_pvoid)pHdr,pErr) == Sdf_co_fail)
		{
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHdr,pErr);
			(void)sdf_ivk_uaFreeHeaderList(*ppAllowHdrList);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerListAccessError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaGetAllowHeaderList(): "
				"Failed to append Supported header to HeaderList",pErr);
#endif
			pErr->errCode=Sdf_en_headerListAccessError;
			return Sdf_co_fail;
		}
	}
	/*
	 * If INVITE, OPTIONS CANCEL ACK or BYE are 
	 * missing from the profile, UATK inserts them here.
	 */
	if ( (dMethodsMissingArray[0] == Sdf_co_true) || \
		 (dMethodsMissingArray[1] == Sdf_co_true) || \
		 (dMethodsMissingArray[2] == Sdf_co_true) || \
		 (dMethodsMissingArray[3] == Sdf_co_true) || \
		 (dMethodsMissingArray[4] == Sdf_co_true) )
	{
			
		for (dIndex = 0; dIndex < 5; dIndex++)
		{
			if (dMethodsMissingArray[dIndex] == Sdf_co_false)
				continue;

			if (sip_initSipHeader(&pHdr, SipHdrTypeAllow, \
				(SipError*)&(pErr->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaGetAllowHeaderList(): "
					"Failed to Initialize Allow SipHeader",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				(void)sdf_ivk_uaFreeHeaderList(*ppAllowHdrList);
				return Sdf_co_fail;
			}
			switch (dIndex)
			{
				case 0:
					Sdf_mc_strdup(pHdrStr, "INVITE");
					break;
				case 1:
					Sdf_mc_strdup(pHdrStr, "ACK");
					break;
				case 2:
					Sdf_mc_strdup(pHdrStr, "OPTIONS");
					break;
				case 3:
					Sdf_mc_strdup(pHdrStr, "CANCEL");
					break;
				case 4:
					Sdf_mc_strdup(pHdrStr, "BYE");
					break;
			}

			if(sip_setMethodInAllowHdr(pHdr, pHdrStr, \
				(SipError *)&(pErr->stkErrCode))==SipFail)
			{
				sip_freeSipHeader(pHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHdr,pErr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pHdrStr,pErr);
				(void)sdf_ivk_uaFreeHeaderList(*ppAllowHdrList);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaGetAllowHeaderList(): "
					"Failed to set method in Allow Header",\
					pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			/*
			 * Add this header to the HeaderList
			 */
			if(sdf_listAppend(&((*ppAllowHdrList)->slHeaders), \
				(Sdf_ty_pvoid)pHdr,pErr) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeHeaderList(*ppAllowHdrList);
				sip_freeSipHeader(pHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHdr,pErr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
								(Sdf_ty_pvoid*)&pHdrStr,pErr);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,
					Sdf_en_headerListAccessError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaGetAllowHeaderList(): "
					"Failed to append Allow header to "
					"HeaderList structure",pErr);
#endif
				pErr->errCode=Sdf_en_headerListAccessError;
				return Sdf_co_fail;
			}
		}	/*- end of 'for' iterating thru missing method array -*/
	}	/*- end of 'if' for Missing Array check -*/
#ifdef SDF_UAB2B

    if(dIsPRACKPresent == Sdf_co_false)
	{
		(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList),\
				&dListIterator, pErr);
		/* Iterate through profile & check if 
		 * ALLOW header is present or not */
		while(dListIterator.pCurrentElement != Sdf_co_null)
		{
			pTemp = (Sdf_st_configExtraHeaders *) \
							(dListIterator.pCurrentElement->pData);

			if(Sdf_mc_strcasecmp((Sdf_ty_s8bit *)"REQUIRE", \
									pTemp->pParam->pName)==0)
			{
				/*
				 * Check if "addalways" xml tag is enabled or not.
				 * If it is disabled, add the default set supported by UATK
				 */
				if(Sdf_mc_strcasecmp((Sdf_ty_s8bit *)"yes", \
						(Sdf_ty_s8bit *)pTemp->pData) == 0)
				{	
					dIsRequirePresent = Sdf_co_true;
					break;
				}
			}
			else if(Sdf_mc_strcasecmp((Sdf_ty_s8bit	*)"SUPPORTED", \
									pTemp->pParam->pName)==0)
			{
				if(Sdf_mc_strcasecmp((Sdf_ty_s8bit *)"yes", \
						(Sdf_ty_s8bit *)pTemp->pData) == 0)
				{	
					dIsSupportedPresent = Sdf_co_true;

					(void)sip_listSizeOf(&(pTemp->pParam->slValue), \
							 &dSupportedHdrListSize,(SipError*)&(pErr->stkErrCode));

					for(dIndex=0; dIndex<dSupportedHdrListSize; dIndex++)
					{
						if (sip_listGetAt(&(pTemp->pParam->slValue), 
							dIndex,(Sdf_ty_pvoid *)&pTempStr,
							(SipError*)&(pErr->stkErrCode)) == SipFail)
						{
							pErr->errCode = Sdf_en_extraHeaderAccessError;
							return Sdf_co_fail;
						}
						Sdf_mc_strdup(pHdrStr,pTempStr);

						if ((Sdf_mc_strcmp(pHdrStr, "100rel") == 0)|| \
							(Sdf_mc_strcmp(pHdrStr, "*") == 0))
							dFoundSupportedTags = Sdf_co_true;
					}
				}
			}

			(void)sdf_listNext(&dListIterator, pErr);
		}

		(void)sip_listSizeOf(&(pTemp->pParam->slValue), \
							 &dRequireHdrListSize, \
				(SipError*)&(pErr->stkErrCode));

		for(dIndex=0; dIndex<dRequireHdrListSize; dIndex++)
		{
			if(dIsRequirePresent == Sdf_co_true)
			{
            	if (sip_listGetAt(&(pTemp->pParam->slValue), 
					dIndex,(Sdf_ty_pvoid *)&pTempStr,
					(SipError*)&(pErr->stkErrCode)) == SipFail)
				{
					pErr->errCode = Sdf_en_extraHeaderAccessError;
					return Sdf_co_fail;
				}

				Sdf_mc_strdup(pHdrStr,pTempStr);
			}
		
		/*- Check if any of the basic methods are missing -*/
			if (Sdf_mc_strcmp(pHdrStr, "100rel") == 0)
			{
				dFound100rel = Sdf_co_true;
				break;
			}
		}

		if(((dIsRequirePresent == Sdf_co_true)&&(dFound100rel == Sdf_co_true))|| \
				((dIsSupportedPresent == Sdf_co_true)&&(dFoundSupportedTags == Sdf_co_true)))
		{	
			if (sip_initSipHeader(&pHdr, SipHdrTypeAllow, \
				(SipError*)&(pErr->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaGetAllowHeaderList(): "
					"Failed to Initialize Allow SipHeader",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}

			Sdf_mc_strdup(pHdrStr,"PRACK");

			if(sip_setMethodInAllowHdr(pHdr, pHdrStr, \
				(SipError *)&(pErr->stkErrCode))==SipFail)
			{
				sip_freeSipHeader(pHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHdr,pErr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
						(Sdf_ty_pvoid*)&pHdrStr,pErr);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaGetAllowHeaderList(): "
					"Failed to set method in Allow Header",\
					pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}

			if(sdf_listAppend(&((*ppAllowHdrList)->slHeaders), \
				(Sdf_ty_pvoid)pHdr,pErr) == Sdf_co_fail)
			{
				sip_freeSipHeader(pHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pHdr,pErr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pHdrStr,pErr);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,
					Sdf_en_headerListAccessError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaGetAllowHeaderList(): "
					"Failed to append Allow header to "
					"HeaderList structure",pErr);
#endif
				pErr->errCode=Sdf_en_headerListAccessError;
				return Sdf_co_fail;
			}

			(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList),\
				&dListIterator, pErr);
			Sdf_mc_strdup(pHdrTempStr, "PRACK");
		/* Iterate through profile & check if 
		 * ALLOW header is present or not */
			while(dListIterator.pCurrentElement != Sdf_co_null)
			{
				pTemp = (Sdf_st_configExtraHeaders *) \
							(dListIterator.pCurrentElement->pData);

				if(Sdf_mc_strcasecmp((Sdf_ty_s8bit *)"ALLOW", \
									pTemp->pParam->pName)==0)
				{
					if(sip_listAppend(&(pTemp->pParam->slValue), \
				(Sdf_ty_pvoid)pHdrTempStr,(SipError*)&(pErr->stkErrCode))
							== SipFail)
					{
						(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pHdrTempStr,pErr);
						return Sdf_co_fail;
					}
					
				}
				(void)sdf_listNext(&dListIterator, pErr);
			}
		}
	}
#endif
	
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaCallState
**
** DESCRIPTION: This function checks the current call state and returns
**				Sdf_en_earlyDialogState if the call establishment in progress,
**				Sdf_en_confirmedDialogState if the call is in established state &
**				Sdf_en_invalidDialogState if the call is in other states.
** PARAMETERS:
**	pCallObj(IN)			: Call Object whos call state has to be checked
******************************************************************************/
Sdf_ty_callState sdf_fn_uaCallState(Sdf_st_callObject *pCallObj)
{

	Sdf_ty_callState   dDialogState = Sdf_en_invalidDialogState;

#ifdef SDF_PARAMVALIDATION
	if ((pCallObj == Sdf_co_null) ||\
			(pCallObj->pCallInfo == Sdf_co_null))
	{
		return dDialogState;
	}
#endif
	
	switch(pCallObj->pCallInfo->dState)
	{
		case Sdf_en_inviteReceived:
		case Sdf_en_inviteSent:
#ifdef SIP_RPR	
		case Sdf_en_rprReceived:
		case Sdf_en_rprSent:
#endif		
		case Sdf_en_overlapTransactionReceived:
		case Sdf_en_overlapTransactionSent:
		case Sdf_en_provisionalRespSent:
		case Sdf_en_provisionalRespReceived:
			{
				dDialogState = Sdf_en_earlyDialogState;
				break;
			}
		case Sdf_en_reInviteReceived:
		case Sdf_en_reInviteSent:
		case Sdf_en_reInviteFinalResponseReceived:
		case Sdf_en_reInviteFinalResponseSent:
		case Sdf_en_finalSuccessReceived:
		case Sdf_en_finalSuccessSent:
		case Sdf_en_callEstablished:
		case Sdf_en_reInviteCancelled:
		case Sdf_en_transactionReceived:
		case Sdf_en_transactionSent:
		case Sdf_en_reInviteprovRespReceived:
		case Sdf_en_reInviteprovRespSent:
			{
				dDialogState = Sdf_en_confirmedDialogState;
				break;
			}
		case Sdf_en_invalidStateTransition:
		case Sdf_en_idle:
		case Sdf_en_finalFailureReceived:
		case Sdf_en_finalFailureSent:
		case Sdf_en_byeReceived:
		case Sdf_en_byeSent:
		case Sdf_en_inviteCancelled:
		case Sdf_en_callTerminated:
		case Sdf_en_transactionCancel:	
			{
				dDialogState = Sdf_en_invalidDialogState;
				break;
			}
	}
	return dDialogState;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaConverHexToChar
 **
 ** DESCRIPTION: This function converts the Hex chracter into the ascii and
 ** 			removes it from the string. Hex character is identified by %
 **				eg asd%23
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaConverHexToChar
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *pHexString,
	Sdf_st_error *pErr)
#else
	(pHexString, pErr)
	Sdf_ty_s8bit *pHexString;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dIndex;
	(void)pErr;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaConverHexToChar");

	for(dIndex=0; dIndex<Sdf_mc_strlen(pHexString);dIndex++)
	{
		if(pHexString[dIndex]=='%')
		{
			Sdf_ty_s8bit dNumber[3],dAsci;
			Sdf_ty_u32bit dHexValue=0x0,dJindex;

			dNumber[0]=pHexString[dIndex+1];
			dNumber[1]=pHexString[dIndex+2];
			dNumber[2]='\0';

			/* conver the string to hex value */
			dHexValue=strtol(dNumber, (Sdf_ty_s8bit **)Sdf_co_null, 16);

			/*Convert Hexvalue to Char */
			dAsci=(Sdf_ty_s8bit)dHexValue;

			/* Remove the % and the hex number from the original string
			 * and insert the new char
			 */
			pHexString[dIndex]=dAsci;
			for( dJindex = dIndex+1; dJindex < (Sdf_mc_strlen(pHexString)-2);\
				dJindex++)
			{
				pHexString[dJindex]=pHexString[dJindex+2];
			}
			pHexString[dJindex]='\0';
			break;
		}
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Exiting sdf_fn_uaConverHexToChar");
	return Sdf_co_success;

}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaInsertHeaderFromProfile
 **
 ** DESCRIPTION: This function inserts the ACCEPT, ACCEPT-ENCODING and
 **				ACCEPT-LANGUAGE header from the profile to the response message
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaInsertHeaderFromProfile
#ifdef ANSI_PROTO
	(Sdf_st_configExtraHeaders *pConfigParam,
	 SipMessage *pMesg,
	Sdf_st_error *pErr)
#else
	(pConfigParam,pMesg,pErr)
	Sdf_st_configExtraHeaders *pConfigParam;
	SipMessage *pMesg; 
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit size=0,dIndex=0;
	Sdf_st_error	dLocalErr;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaInsertHeaderFromProfile");

	(void)sip_listSizeOf(&(pConfigParam->pParam->slValue), &size, \
   		(SipError*)&(pErr->stkErrCode));

	for(dIndex=0;dIndex<size;dIndex++)
	{
		Sdf_ty_s8bit *pTempVal = Sdf_co_null;
		Sdf_ty_s8bit *pTmpHdr = Sdf_co_null;
		SipHeader *pHdr=Sdf_co_null;
		en_HeaderType dHdrType;
		Sdf_ty_s8bit *pHdrStr,pActualHdrString[Sdf_co_characterBufferSize];
        Sdf_ty_s8bit  pTempString[Sdf_co_characterBufferSize] = {'\0',};

		if(Sdf_mc_strcasecmp("ACCEPT",pConfigParam->pParam->pName)==0)
			Sdf_mc_strcpy(pTempString,"ACCEPT:");
		else if(Sdf_mc_strcasecmp("ACCEPT-ENCODING", \
								pConfigParam->pParam->pName)==0)
			Sdf_mc_strcpy(pTempString,"ACCEPT-ENCODING:");
		else if(Sdf_mc_strcasecmp("ACCEPT-LANGUAGE", \
					pConfigParam->pParam->pName)==0)
			Sdf_mc_strcpy(pTempString,"ACCEPT-LANGUAGE:");

		
		if( SipFail == sip_listGetAt(&(pConfigParam->pParam->slValue), dIndex, \
			(Sdf_ty_pvoid *)&pTempVal,  (SipError*)&(pErr->stkErrCode)))
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR							
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaInsertHeaderFromProfile(): "
				"Could not get structure from list", pErr);
#endif							
			return Sdf_co_fail;
		}

		while( pTempVal != Sdf_co_null )
		{
			pTempVal = Sdf_mc_strtokr(pTempVal,",", &pTmpHdr);

			Sdf_mc_strdup(pHdrStr, pTempVal);

			Sdf_mc_strcpy(pActualHdrString, pTempString);
			Sdf_mc_strcat(pActualHdrString, pHdrStr);
			
			if(sip_getTypeFromString(pActualHdrString, &dHdrType,\
					(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR							
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*) "sdf_fn_uaInsertHeaderFromProfile(): "
					"Could Not get Type of the header", pErr);
#endif							
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
								(Sdf_ty_pvoid *)&pHdrStr, &dLocalErr);
				return Sdf_co_fail;
			}
			if(sip_initSipHeader(&pHdr, SipHdrTypeAny, \
				(SipError*)&(pErr->stkErrCode))==SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR							
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_fn_uaInsertHeaderFromProfile(): "
					"Could Not Initialize  SipHeader Structure", pErr);
#endif							
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
								(Sdf_ty_pvoid *)&pHdrStr, &dLocalErr);
				return Sdf_co_fail;
			}

			if (sip_parseSingleHeader(pActualHdrString, dHdrType, pHdr, \
				(SipError *)&(pErr->stkErrCode))==SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR							
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_fn_uaInsertHeaderFromProfile(): "
					"Failed to form Accept header",pErr);
#endif							
#if 0
				sip_freeSipHeader(pHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pHdr, \
						&dLocalErr);
#endif
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
								(Sdf_ty_pvoid *)&pHdrStr, &dLocalErr);
				return Sdf_co_fail;
			}
			if(sip_insertHeaderAtIndex(pMesg, pHdr, dIndex, \
				(SipError *)&(pErr->stkErrCode))==SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR							
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_fn_uaInsertHeaderFromProfile(): "
					"Could Not insert Header into SipMessage",pErr);
#endif							
				sip_freeSipHeader(pHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pHdr, \
						&dLocalErr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
								(Sdf_ty_pvoid *)&pHdrStr, &dLocalErr);
				return Sdf_co_fail;
			}
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pHdr, pErr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
								(Sdf_ty_pvoid *)&pHdrStr, &dLocalErr);
			pTempVal = pTmpHdr;
		}
	}		
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Exiting sdf_fn_uaConverHexToChar");
	return Sdf_co_success;

}

#ifdef SDF_AUTHENTICATE
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUpdateAuthorization
 **
 ** DESCRIPTION: This function updates the authorization header present in the
 **				 message by the Next nonce count value stored in the Auth-Info 
 **				 header . Also Updates the authorization header list stored in
 **				 the Persistent header list of the call object. 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateAuthorization
#ifdef ANSI_PROTO
	(Sdf_st_callObject 	*pCallObj,
	 SipMessage 		*pSipMsg,
	 const Sdf_ty_s8bit *pMethod,
	 Sdf_st_error 		*pErr)
#else
	(pCallObj, pSipMsg, pMethod, pErr)
	Sdf_st_callObject 	*pCallObj;
	SipMessage 			*pSipMsg;
	const Sdf_ty_s8bit 	*pMethod;
	Sdf_st_error 		*pErr;
#endif
{
	Sdf_st_uacAuthInfo 			*pUacAuthInfo=Sdf_co_null;
  Sdf_st_uacAuthInfo 			*pProxyAuthInfo=Sdf_co_null;
	Sdf_st_authenticationParams *pAuthParams=Sdf_co_null;
  Sdf_st_authenticationParams *pProxyauthParams=Sdf_co_null;
	SipGenericCredential 		*pAuthCredential=Sdf_co_null;
  SipGenericCredential 		*pProxyauthCredential=Sdf_co_null;
	SipGenericChallenge			*pAuthChallenge=Sdf_co_null;
  SipGenericChallenge			*pProxyauthChallenge=Sdf_co_null;
	Sdf_ty_slist				slAuthInfoHeaderList;
	Sdf_ty_u32bit 				dIndex=0;
	Sdf_ty_u32bit				dAuthCount=0;
  Sdf_ty_u32bit				dProxyauthCount=0;
	Sdf_ty_u32bit				dAuthCountinMsg = 0;
  Sdf_ty_u32bit       dProxyauthCountinMsg = 0;
	Sdf_st_headerList 			*pAuthorizationHeaderList = Sdf_co_null;
  Sdf_st_headerList 			*pProxyauthorizationHeaderList = Sdf_co_null;
	SipHeader 					*pAuthHeader=Sdf_co_null;
  SipHeader           *pProxyauthHeader=Sdf_co_null;
	en_CredentialType			dCredType = SipCredBasic;
  en_CredentialType			dProxyCredType = SipCredBasic;
	Sdf_ty_s8bit 				*pScheme=Sdf_co_null;
  Sdf_ty_s8bit 				*pProxyScheme=Sdf_co_null;
	Sdf_ty_s8bit 				*pNextNonce=Sdf_co_null;
	Sdf_st_error				dLocalErr;
	SipBool dFlag = SipSuccess;
	SipBool dRetVal1 = SipSuccess;
  SipBool dRetVal2 = SipSuccess;
	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaUpdateAuthorization(): pError param passed is invalid");
#endif
		return Sdf_co_fail;
	}

	if( (pCallObj == Sdf_co_null) || (pSipMsg == Sdf_co_null) || \
		(pMethod == Sdf_co_null))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization( ): "
			"Invalid Param",pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */


	/*  Below portion checks whether the Updation is Required or Not */
	sip_getHeaderCount(pSipMsg, SipHdrTypeAuthorization, &dAuthCountinMsg, \
			(SipError *)&(pErr->stkErrCode));
  sip_getHeaderCount(pSipMsg, SipHdrTypeProxyauthorization, &dProxyauthCountinMsg, \
			(SipError *)&(pErr->stkErrCode));
	/* There is no authenticate header so no updation is required */
	if(dAuthCountinMsg == 0 && dProxyauthCountinMsg == 0) 
	{
		sdf_fn_debug((Sdf_ty_s8bit*) \
			"SDF_DEBUG--Exiting sdf_fn_uaUpdateAuthorization");
		return Sdf_co_success;
	}
	/* Get the Auth-Info Header List from the slPersistentHeaders header 
	 * list of the call Object */

	if (sdf_ivk_uaGetHeaderListByCorrelation(&(pCallObj->slPersistentHeaders), \
		(Sdf_ty_s8bit *)"uat-authInfo", &slAuthInfoHeaderList,\
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
			"Failed to get Auth-Info header list by correlation "
			" uat-authInfo",pErr);
#endif				
		return Sdf_co_fail;
	}
	
	sdf_listSizeOf(&slAuthInfoHeaderList, &dAuthCount, pErr);
	
	/* If the Size is not Zero then update the list */

	if(dAuthCount ==0)
	{
		sdf_listDeleteAll(&slAuthInfoHeaderList,pErr);
		dFlag = SipFail;
	}
	sip_initSipHeader(&pAuthHeader, SipHdrTypeAny, \
					(SipError*)&(pErr->stkErrCode));
  sip_initSipHeader(&pProxyauthHeader, SipHdrTypeAny, \
					(SipError*)&(pErr->stkErrCode));
  
	if(dAuthCountinMsg > 0)
  {
    dRetVal1 = sip_getHeader(pSipMsg, SipHdrTypeAuthorization, pAuthHeader, \
			(SipError*)&(pErr->stkErrCode));
  }
  if(dProxyauthCountinMsg > 0)
  {
    dRetVal2 = sip_getHeader(pSipMsg, SipHdrTypeProxyauthorization, pProxyauthHeader, \
			(SipError*)&(pErr->stkErrCode));
  }
  if(dRetVal1 == SipFail || dRetVal2 == SipFail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
			"Failed to get Authorization header list by correlation "
			" uat-authenticate",pErr);
#endif				
		sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);
		sip_freeSipHeader(pAuthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
		sip_freeSipHeader(pProxyauthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
		sdf_fn_debug((Sdf_ty_s8bit*) \
			"SDF_DEBUG--Exiting sdf_fn_uaUpdateAuthorization");
		return Sdf_co_success;
	}

	if(dAuthCountinMsg > 0)
	{
		sip_getCredentialsFromAuthorizationHdr(pAuthHeader, &pAuthCredential, \
				(SipError*) &(pErr->stkErrCode));
		sip_getCredentialTypeFromCredential(pAuthCredential, &dCredType,\
		(SipError*)&(pErr->stkErrCode));
	}
	if(dProxyauthCountinMsg > 0)
	{
		sip_getCredentialsFromProxyAuthorizationHdr(pProxyauthHeader, &pProxyauthCredential, \
				(SipError*) &(pErr->stkErrCode));
		sip_getCredentialTypeFromCredential(pProxyauthCredential, &dProxyCredType,\
		(SipError*)&(pErr->stkErrCode));
	}	
	/* If the scheme  basic then return success */
	if(SipCredBasic == dCredType && SipCredBasic == dProxyCredType)
	{
		sip_freeSipGenericCredential(pAuthCredential);
    sip_freeSipGenericCredential(pProxyauthCredential);
		sip_freeSipHeader(pAuthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
    sip_freeSipHeader(pProxyauthHeader);
		sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
		sdf_fn_debug((Sdf_ty_s8bit*) \
			"SDF_DEBUG--Exiting sdf_fn_uaUpdateAuthorization");
		return Sdf_co_success;
	}
	if(dAuthCountinMsg > 0)
	{
		sip_getChallengeFromCredential(pAuthCredential, &pAuthChallenge,\
			(SipError*)&(pErr->stkErrCode));
		sip_getSchemeFromChallenge(pAuthChallenge, &pScheme,\
		(SipError*)&(pErr->stkErrCode));
		sip_freeSipGenericCredential(pAuthCredential);
	}
	if(dProxyauthCountinMsg > 0)
	{
  	sip_getChallengeFromCredential(pProxyauthCredential, &pProxyauthChallenge,\
			(SipError*)&(pErr->stkErrCode));	
  	sip_getSchemeFromChallenge(pProxyauthChallenge, &pProxyScheme,\
		(SipError*)&(pErr->stkErrCode));
  	sip_freeSipGenericCredential(pProxyauthCredential);
	}

	/* If scheme is other than Digest return success. UATK supports only
	 * Digest scheme of Authentication */
	if(((pScheme != Sdf_co_null) && (Sdf_mc_strcasecmp(pScheme,"Digest") != 0))\
			||((pProxyScheme != Sdf_co_null) &&
				(Sdf_mc_strcasecmp(pProxyScheme,"Digest") != 0)))
	{
		sip_freeSipGenericChallenge(pAuthChallenge);
    sip_freeSipGenericChallenge(pProxyauthChallenge);
		sip_freeSipHeader(pAuthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
    sip_freeSipHeader(pProxyauthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
		sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);
		sdf_fn_debug((Sdf_ty_s8bit*) \
			"SDF_DEBUG--Exiting sdf_fn_uaUpdateAuthorization");
		return Sdf_co_success;
	}


	/* 
	 * Fill up the Sdf_st_uacAuthInfo and Sdf_st_authenticationParams  
	 * structures From the Authorization header
	 */

	sdf_ivk_uaInitUacAuthInfo(&pUacAuthInfo, pErr);
  sdf_ivk_uaInitUacAuthInfo(&pProxyAuthInfo, pErr);
	sdf_ivk_uaInitAuthenticationParams(&pAuthParams,pErr);
  sdf_ivk_uaInitAuthenticationParams(&pProxyauthParams,pErr);

	pAuthParams->dHdrType = SipHdrTypeWwwAuthenticate;
	pAuthParams->pScheme = Sdf_mc_strdupAuth(pScheme);
  pProxyauthParams->dHdrType = SipHdrTypeProxyAuthenticate;
	pProxyauthParams->pScheme = Sdf_mc_strdupAuth(pProxyScheme);

	if(dAuthCountinMsg > 0)
		sip_getAuthorizationParamCountFromChallenge(pAuthChallenge, &dAuthCount,\
		(SipError*)&(pErr->stkErrCode));
	if(dProxyauthCountinMsg > 0)
  	sip_getAuthorizationParamCountFromChallenge(pProxyauthChallenge, &dProxyauthCount,\
		(SipError*)&(pErr->stkErrCode));
	/*
	* Iterate through the parameters in the  Authorization header and
	*  extract the relevant parameters into pUacAuthInfo and pAuthParams		
	*/

	for (dIndex=0; dIndex < dAuthCount; dIndex++)
	{
		SipParam *pParam;
		Sdf_ty_s8bit *pName, *pValue;
		Sdf_ty_s8bit pTempValue[Sdf_co_smallBufferSize];

		pParam=Sdf_co_null;
		
		if (sip_getAuthorizationParamAtIndexFromChallenge(pAuthChallenge, \
			&pParam, dIndex,(SipError *) &(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipGenericChallenge(pAuthChallenge);
      sip_freeSipGenericChallenge(pProxyauthChallenge);
			sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
      sdf_ivk_uaFreeAuthenticationParams(pProxyauthParams);
			sdf_ivk_uaFreeUacAuthInfo(pUacAuthInfo);
      sdf_ivk_uaFreeUacAuthInfo(pProxyAuthInfo);
			sip_freeSipHeader(pAuthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
      sip_freeSipHeader(pProxyauthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
				"Error Getting the param from Authenticate Challenge", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if (sip_getNameFromSipParam(pParam, &pName, \
			 (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
      sdf_ivk_uaFreeAuthenticationParams(pProxyauthParams);
			sdf_ivk_uaFreeUacAuthInfo(pUacAuthInfo);
      sdf_ivk_uaFreeUacAuthInfo(pProxyAuthInfo);
			sip_freeSipGenericChallenge(pAuthChallenge);
      sip_freeSipGenericChallenge(pProxyauthChallenge);
			sip_freeSipParam(pParam);
			sip_freeSipHeader(pAuthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
      sip_freeSipHeader(pProxyauthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);		
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
				"Error getting param name from Authenticate Challenge", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if (sip_getValueAtIndexFromSipParam(pParam, &pValue, 0,\
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
      sdf_ivk_uaFreeAuthenticationParams(pProxyauthParams);
			sdf_ivk_uaFreeUacAuthInfo(pUacAuthInfo);
      sdf_ivk_uaFreeUacAuthInfo(pProxyAuthInfo);
			sip_freeSipGenericChallenge(pAuthChallenge);
      sip_freeSipGenericChallenge(pProxyauthChallenge);
			sip_freeSipParam(pParam);
			sip_freeSipHeader(pAuthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
      sip_freeSipHeader(pProxyauthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
				"Error Getting param from Authenticate Challenge", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		/* Free the local reference */
		sip_freeSipParam(pParam);
		
		if(pValue == Sdf_co_null)
		{
			sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
      sdf_ivk_uaFreeAuthenticationParams(pProxyauthParams);
			sdf_ivk_uaFreeUacAuthInfo(pUacAuthInfo);
      sdf_ivk_uaFreeUacAuthInfo(pProxyAuthInfo);
			sip_freeSipGenericChallenge(pAuthChallenge);
      sip_freeSipGenericChallenge(pProxyauthChallenge);
			sip_freeSipHeader(pAuthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
      sip_freeSipHeader(pProxyauthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);		
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
				"Authenticate param has no value", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if (Sdf_mc_strcasecmp(pName, "realm") == 0)
			pAuthParams->pRealm = Sdf_mc_strdupAuth(pValue);
		else if (Sdf_mc_strcasecmp(pName, "domain") == 0)
			pAuthParams->pDomain = Sdf_mc_strdupAuth(pValue);
		else if (Sdf_mc_strcasecmp(pName, "opaque") == 0)
			pAuthParams->pOpaque = Sdf_mc_strdupAuth(pValue);
		else if (Sdf_mc_strcasecmp(pName, "stale") == 0)
			pAuthParams->pStale = Sdf_mc_strdupAuth(pValue);
		else if (Sdf_mc_strcasecmp(pName, "algorithm") == 0)
			pAuthParams->pAlgorithm = Sdf_mc_strdupAuth(pValue);
		else if ((Sdf_mc_strcasecmp(pName, "nonce") == 0) && dFlag == SipFail)
			pAuthParams->pNonce = Sdf_mc_strdupAuth(pValue);
		else if (Sdf_mc_strcasecmp(pName, "username") == 0)
		{
			sdf_fn_uaUnq(pValue, pTempValue);
			pUacAuthInfo->pUserName = Sdf_mc_strdupAuth(pTempValue);
		}
		else if (Sdf_mc_strcasecmp(pName, "qop") == 0)
		{
			pAuthParams->pQop = Sdf_mc_strdupAuth(pValue);
			pUacAuthInfo->pQop=Sdf_mc_strdupAuth(pValue);
		}
		else if (Sdf_mc_strcasecmp(pName, "Cnonce") == 0)
			pUacAuthInfo->pCnonce=Sdf_mc_strdupAuth(pValue);
		else if (Sdf_mc_strcasecmp(pName, "nc") == 0)
		{
			sdf_fn_uaUnq(pValue, pTempValue);
			pUacAuthInfo->pNonceCount = Sdf_mc_strdupAuth(pTempValue);
		}
	}
  for(dIndex =0; dIndex < dProxyauthCount; dIndex++)
  {
    SipParam *pProxyParam;
		Sdf_ty_s8bit *pProxyName, *pProxyValue;
		Sdf_ty_s8bit pProxyTempValue[Sdf_co_smallBufferSize];

		pProxyParam=Sdf_co_null;
		
		if (sip_getAuthorizationParamAtIndexFromChallenge(pProxyauthChallenge, \
			&pProxyParam, dIndex,(SipError *) &(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipGenericChallenge(pAuthChallenge);
      sip_freeSipGenericChallenge(pProxyauthChallenge);
			sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
      sdf_ivk_uaFreeAuthenticationParams(pProxyauthParams);
			sdf_ivk_uaFreeUacAuthInfo(pUacAuthInfo);
      sdf_ivk_uaFreeUacAuthInfo(pProxyAuthInfo);
			sip_freeSipHeader(pAuthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
      sip_freeSipHeader(pProxyauthHeader);
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
				"Error Getting the param from Authenticate Challenge", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if (sip_getNameFromSipParam(pProxyParam, &pProxyName, \
			 (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
      sdf_ivk_uaFreeAuthenticationParams(pProxyauthParams);
			sdf_ivk_uaFreeUacAuthInfo(pUacAuthInfo);
      sdf_ivk_uaFreeUacAuthInfo(pProxyAuthInfo);
			sip_freeSipGenericChallenge(pAuthChallenge);
      sip_freeSipGenericChallenge(pProxyauthChallenge);
			sip_freeSipParam(pProxyParam);
			sip_freeSipHeader(pAuthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
      sip_freeSipHeader(pProxyauthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
				"Error getting param name from Authenticate Challenge", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if (sip_getValueAtIndexFromSipParam(pProxyParam, &pProxyValue, 0,\
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
      sdf_ivk_uaFreeAuthenticationParams(pProxyauthParams);
			sdf_ivk_uaFreeUacAuthInfo(pUacAuthInfo);
      sdf_ivk_uaFreeUacAuthInfo(pProxyAuthInfo);
			sip_freeSipGenericChallenge(pAuthChallenge);
      sip_freeSipGenericChallenge(pProxyauthChallenge);
			sip_freeSipParam(pProxyParam);
			sip_freeSipHeader(pAuthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
      sip_freeSipHeader(pProxyauthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);			
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
				"Error Getting param from Authenticate Challenge", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		/* Free the local reference */
		sip_freeSipParam(pProxyParam);
		
		if(pProxyValue == Sdf_co_null)
		{
			sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
      sdf_ivk_uaFreeAuthenticationParams(pProxyauthParams);
			sdf_ivk_uaFreeUacAuthInfo(pUacAuthInfo);
      sdf_ivk_uaFreeUacAuthInfo(pProxyAuthInfo);
			sip_freeSipGenericChallenge(pAuthChallenge);
      sip_freeSipGenericChallenge(pProxyauthChallenge);
			sip_freeSipHeader(pAuthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
      sip_freeSipHeader(pProxyauthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
				"Authenticate param has no value", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if (Sdf_mc_strcasecmp(pProxyName, "realm") == 0)
			pProxyauthParams->pRealm = Sdf_mc_strdupAuth(pProxyValue);
		else if (Sdf_mc_strcasecmp(pProxyName, "domain") == 0)
			pProxyauthParams->pDomain = Sdf_mc_strdupAuth(pProxyValue);
		else if (Sdf_mc_strcasecmp(pProxyName, "opaque") == 0)
			pProxyauthParams->pOpaque = Sdf_mc_strdupAuth(pProxyValue);
		else if (Sdf_mc_strcasecmp(pProxyName, "stale") == 0)
			pProxyauthParams->pStale = Sdf_mc_strdupAuth(pProxyValue);
		else if (Sdf_mc_strcasecmp(pProxyName, "algorithm") == 0)
			pProxyauthParams->pAlgorithm = Sdf_mc_strdupAuth(pProxyValue);
		else if ((Sdf_mc_strcasecmp(pProxyName, "nonce") == 0) && dFlag == SipFail)
			pProxyauthParams->pNonce = Sdf_mc_strdupAuth(pProxyValue);
		else if (Sdf_mc_strcasecmp(pProxyName, "username") == 0)
		{
			sdf_fn_uaUnq(pProxyValue, pProxyTempValue);
			pProxyAuthInfo->pUserName = Sdf_mc_strdupAuth(pProxyTempValue);
		}
		else if (Sdf_mc_strcasecmp(pProxyName, "qop") == 0)
		{
			pProxyauthParams->pQop = Sdf_mc_strdupAuth(pProxyValue);
			pProxyAuthInfo->pQop=Sdf_mc_strdupAuth(pProxyValue);
		}
		else if (Sdf_mc_strcasecmp(pProxyName, "Cnonce") == 0)
			pProxyAuthInfo->pCnonce=Sdf_mc_strdupAuth(pProxyValue);
		else if (Sdf_mc_strcasecmp(pProxyName, "nc") == 0)
		{
			sdf_fn_uaUnq(pProxyValue, pProxyTempValue);
			pProxyAuthInfo->pNonceCount = Sdf_mc_strdupAuth(pProxyTempValue);
		}
  }
	sip_freeSipGenericChallenge(pAuthChallenge);
  sip_freeSipGenericChallenge(pProxyauthChallenge);
#if 0
	/* If the QOP is auth-int  then chech whether msg body is present of not
	 * if not then don't update the  Authorization header. 
	 * Since at this pt of time no msgbody is present in
	 * the SipMessage. This function has been called from the
	 * sdf_ivk_uaFormRequest API
	 */
	if((Sdf_mc_strcasecmp(pAuthParams->pQop,"auth-int")==0) || (Sdf_mc_strcasecmp(pAuthParams->pQop,"auth-int")==0))
	{
		Sdf_ty_u32bit dMsgBodyCount=0;

		sip_getMsgBodyCount(pSipMsg, &dMsgBodyCount, \
				(SipError *)&(pErr->stkErrCode));
		
		/* If no msg body found return success */
		if(dMsgBodyCount==0)
		{
			sip_freeSipHeader(pAuthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
      sip_freeSipHeader(pProxyauthHeader);
      sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pNextNonce, \
				&dLocalErr);
			sdf_fn_debug((Sdf_ty_s8bit*) \
				"SDF_DEBUG--Exiting sdf_fn_uaUpdateAuthorization");
			return Sdf_co_success;
		}
	}

#endif

	/* Get  the passwd for the user pUacAuthInfo->pUserName from the CommonInfo
	 */
	pUacAuthInfo->pPassword = \
		Sdf_mc_strdupAuth(pCallObj->pCommonInfo->pChallangePasswd);
  pProxyAuthInfo->pPassword = \
		Sdf_mc_strdupAuth(pCallObj->pCommonInfo->pChallangePasswd);

	if(dAuthCountinMsg > 0)
  {
    dRetVal1 = sdf_fn_uaFormAuthorizationCredential(pSipMsg,(Sdf_ty_s8bit *)pMethod,\
		&pAuthCredential,pAuthParams, pUacAuthInfo, pErr);
  }
  if(dProxyauthCountinMsg > 0)
  {
    dRetVal2 = sdf_fn_uaFormAuthorizationCredential(pSipMsg,(Sdf_ty_s8bit *)pMethod,\
		&pProxyauthCredential,pProxyauthParams, pProxyAuthInfo, pErr);
  }
  if( dRetVal1 == Sdf_co_fail || dRetVal2 == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_authError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
			"Failed to form the Authorization Credential",pErr);
#endif
		sip_freeSipHeader(pAuthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
    sip_freeSipHeader(pProxyauthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
		sdf_ivk_uaFreeUacAuthInfo(pUacAuthInfo);
    sdf_ivk_uaFreeUacAuthInfo(pProxyAuthInfo);
		sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
    sdf_ivk_uaFreeAuthenticationParams(pProxyauthParams);
		pErr->errCode=Sdf_en_authError;
		return Sdf_co_fail;
	}
	sdf_ivk_uaFreeUacAuthInfo(pUacAuthInfo);
  sdf_ivk_uaFreeUacAuthInfo(pProxyAuthInfo);
	sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
  sdf_ivk_uaFreeAuthenticationParams(pProxyauthParams);
	sip_freeSipHeader(pAuthHeader);
  sip_freeSipHeader(pProxyauthHeader);
	sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
  sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);

	/*
	 * Authorization header is to be formed and set the Credential 
	 */

	if(dAuthCountinMsg > 0)
  {
    dRetVal1 = sip_initSipHeader(&pAuthHeader, SipHdrTypeAuthorization,\
			(SipError *)&(pErr->stkErrCode));
  }
  if(dProxyauthCountinMsg > 0)
  {
    dRetVal2 = sip_initSipHeader(&pProxyauthHeader, SipHdrTypeProxyauthorization,\
			(SipError *)&(pErr->stkErrCode));
  }
  if(dRetVal1 == SipFail || dRetVal2 == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
			"Failed to initialize the Authorization Header",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		sip_freeSipGenericCredential(pAuthCredential);
    sip_freeSipGenericCredential(pProxyauthCredential);
		return Sdf_co_fail;
	}
	
	if(dAuthCountinMsg > 0)
  {
    dRetVal1 = sip_setCredentialsInAuthorizationHdr(pAuthHeader, pAuthCredential,\
		(SipError *) &(pErr->stkErrCode));
  }
  if(dProxyauthCountinMsg > 0)
  {
    dRetVal2 = sip_setCredentialsInProxyAuthorizationHdr(pProxyauthHeader, pProxyauthCredential,\
		(SipError *) &(pErr->stkErrCode));
  }
  if(dRetVal1 == SipFail || dRetVal2 == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
			"Failed to set credentials in Authorization Header",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		sip_freeSipHeader(pAuthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
    sip_freeSipHeader(pProxyauthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
		sip_freeSipGenericCredential(pAuthCredential);
    sip_freeSipGenericCredential(pProxyauthCredential);
		return Sdf_co_fail;
	}
	sip_freeSipGenericCredential(pAuthCredential);
  sip_freeSipGenericCredential(pProxyauthCredential);

	/* Remove the  Authorization header from the outgoing sipmsg */
	for(dIndex=0; dIndex < dAuthCountinMsg; dIndex++)
	{
		sip_deleteHeaderAtIndex(pSipMsg, SipHdrTypeAuthorization, \
				dIndex,(SipError *)&(pErr->stkErrCode));
	}
  for(dIndex=0; dIndex < dProxyauthCountinMsg; dIndex++)
	{
		sip_deleteHeaderAtIndex(pSipMsg, SipHdrTypeProxyauthorization, \
				dIndex,(SipError *)&(pErr->stkErrCode));
	}

	if(dAuthCountinMsg > 0)
  {
    dRetVal1 = sip_insertHeaderAtIndex(pSipMsg, pAuthHeader, 0, \
			(SipError *) &(pErr->stkErrCode));
  }
  if(dProxyauthCountinMsg > 0)
  {
    dRetVal1 = sip_insertHeaderAtIndex(pSipMsg, pProxyauthHeader, 0, \
			(SipError *) &(pErr->stkErrCode));
  }
  if(dRetVal1 == SipFail || dRetVal2 == SipFail)    
  {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
			"Failed to set Authorization header to the SIP Message",pErr);
#endif
		sip_freeSipHeader(pAuthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
    sip_freeSipHeader(pProxyauthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
		pErr->errCode = Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	

		/* Initialise a headerlist of type "uat-authenticate"
	 and Add the header into the 	pAuthorizationHeaderList list */

	if(dAuthCountinMsg > 0)
  {
    sdf_ivk_uaInitHeaderList(&pAuthorizationHeaderList, \
		SipHdrTypeAuthorization, (Sdf_ty_s8bit *)"uat-authenticate", pErr);
		dRetVal1 = sdf_listAppend(&(pAuthorizationHeaderList->slHeaders),\
		(Sdf_ty_pvoid)pAuthHeader,pErr);
  }
  if(dProxyauthCountinMsg > 0)
  {
    sdf_ivk_uaInitHeaderList(&pProxyauthorizationHeaderList, \
		SipHdrTypeProxyauthorization, (Sdf_ty_s8bit *)"uat-proxyauth", pErr);
		dRetVal2 = sdf_listAppend(&(pProxyauthorizationHeaderList->slHeaders),\
		(Sdf_ty_pvoid)pProxyauthHeader,pErr);
  }
  if(dRetVal1 == Sdf_co_fail || dRetVal2 == Sdf_co_fail)  
	{
		sip_freeSipHeader(pAuthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pAuthHeader), pErr);
    sip_freeSipHeader(pProxyauthHeader);
    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pProxyauthHeader), pErr);
		sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
    sdf_ivk_uaFreeHeaderList(pProxyauthorizationHeaderList);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
			"Failed to append the Authorization header to header list", pErr);
#endif
		return Sdf_co_fail;
	}

	sdf_ivk_uaDeleteHeaderListByCorrelation(&(pCallObj->slPersistentHeaders),\
		   	"uat-authenticate",pErr);
  sdf_ivk_uaDeleteHeaderListByCorrelation(&(pCallObj->slPersistentHeaders),\
		   	"uat-proxyauth",pErr);
	 /* 
	 * Append the pAuthorizationHeaderList to the slPersistentHeaders 
	  * header list structures in the call object
	 */

	if(dAuthCountinMsg > 0)
  {
    dRetVal1 = sdf_listAppend(&(pCallObj->slPersistentHeaders), \
			(Sdf_ty_pvoid)pAuthorizationHeaderList, pErr);
  }
  if(dProxyauthCountinMsg > 0)
  {
    dRetVal2 = sdf_listAppend(&(pCallObj->slPersistentHeaders), \
			(Sdf_ty_pvoid)pProxyauthorizationHeaderList, pErr);
  }
  if(dRetVal1 == Sdf_co_fail || dRetVal2 == Sdf_co_fail)
	{
		sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
    sdf_ivk_uaFreeHeaderList(pProxyauthorizationHeaderList);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaUpdateAuthorization(): "
			"Failed to add the header list of Authorization headers to "
			"the PersistentHeaders list of callObject", pErr);
#endif
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG--Exiting sdf_fn_uaUpdateAuthorization");
	
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}
#endif
/* IPTK Auth Change End */

/******************************************************************************
 ** FUNCTION:  		sdf_fn_uaInsertHeaderFromList
 **
 ** DESCRIPTION:	This function  inserts header from the header list to the
 **					Outgoing  message and specially takes care of inserting
 **					Route header( in case of strict router)
 **
 ** PARAMETERS: ICF_SUPPRESS_INTERMEDIATE_ERROR_IND
 **		pMessage(IN/OUT)  : The SipMessage structure where the hedaer will 
 **							be insert
 **						 
 **		pHeaderList(IN)   : Header list where from header will be insert
 **
 **		pErr(OUT)	  :	Error structure in case the function fails
 **
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaInsertHeaderFromList
#ifdef ANSI_PROTO
	(SipMessage *pMessage,
	Sdf_st_headerList *pHeaderList,
	Sdf_st_error *pErr)
#else
	(pMessage, pHeaderList,pErr)
	SipMessage *pMessage;
	Sdf_st_headerList *pHeaderList;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dJindex = 0;
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaInsertHeaderFromList");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if((pErr == Sdf_co_null) ||(pMessage == Sdf_co_null) ||\
		(pHeaderList == Sdf_co_null))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertHeaderFromList( ): "
			"Invalid Param passed to this function",pErr);
#endif		
        if( pErr != Sdf_co_null)
           pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	(void)sdf_listInitIterator(&(pHeaderList->slHeaders), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SipHeader *pElement;
		
		pElement = (SipHeader*)(dListIterator.pCurrentElement->pData);

		/* if header type is Route and the top most element is a strict Router
		 * then don't insert the topmost element in the Route header of the
		 * message 
		 */

		if ((pHeaderList->dType == SipHdrTypeRoute) && (dJindex==0))
		{
			/* Check whether lr parameter in the tompost route is present or not
			 */
			/* Get the AddrSpec of the Route Header */

			SipAddrSpec *pAddrspec=Sdf_co_null;
			
			if(SipFail == sip_getAddrSpecFromRouteHdr(pElement, &pAddrspec,\
					(SipError *)&(pErr->stkErrCode)))
			{
				(void)sdf_listNext(&dListIterator, pErr);
				continue;
			}

			/* If the Addr Type is SipAddrSipUri then only check for lr
			 * parameter
			 */
#ifdef SDF_TLS
	        if((pAddrspec->dType == SipAddrSipUri ) ||
			        (pAddrspec->dType == SipAddrSipSUri ))
#else
	        if(pAddrspec->dType == SipAddrSipUri )
#endif
			{
				Sdf_ty_bool dIsPresentLr=Sdf_co_false;
				Sdf_ty_u32bit dCount;
				SipUrl	*pUrl=Sdf_co_null;

				/* Get the SipUrl From the AddrSpec 
				 */
				if(Sdf_co_fail == sdf_ivk_uaGetUrlFromAddrSpec(pAddrspec,\
							&pUrl,pErr))
				{
					sip_freeSipAddrSpec(pAddrspec);
					(void)sdf_listNext(&dListIterator, pErr);
					continue;
				}

				/* Get the param count */
				/*w534*/ (void)sip_getUrlParamCountFromUrl(pUrl,&dCount,\
						(SipError *)&(pErr->stkErrCode));
				if(dCount !=0)
				{
					SipParam *pParam;
					Sdf_ty_u32bit dIndex;
					/* Iterate through the Param list to find the lr parameter
					 * name
					 */
					for(dIndex=0; dIndex < dCount; dIndex++)
					{
						/*w534*/ (void)sip_getUrlParamAtIndexFromUrl(pUrl,&pParam,dIndex,\
								(SipError *)&(pErr->stkErrCode));
						if(Sdf_mc_strcasecmp(pParam->pName,"lr")==0)
						{
							dIsPresentLr=Sdf_co_true;
							sip_freeSipParam(pParam);
							break;
						}
						sip_freeSipParam(pParam);
					}
				}

				sip_freeSipUrl(pUrl);
				if(dIsPresentLr == Sdf_co_false)
				{
					sip_freeSipAddrSpec(pAddrspec);
					(void)sdf_listNext(&dListIterator, pErr);
					continue;
				}
			}
			sip_freeSipAddrSpec(pAddrspec);
		}


		/* Insert the header into the Sip Message */

		if (sip_insertHeaderAtIndex(pMessage, \
			pElement, dJindex, (SipError *)&(pErr->stkErrCode))\
			== SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR							
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaInsertHeaderFromList( ): "
				"Failed to insert header into SipMessage", \
				pErr);
#endif							
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
		dJindex++;
	}/* End of headerlist iteration */

	return Sdf_co_success;
	
}


/******************************************************************************
 ** FUNCTION:  		sdf_fn_uaCheckLrParameter
 **
 ** DESCRIPTION:	This function  checks whether lr parameter is present or not
 ** 				in the Addrspec
 **
 ** PARAMETERS:
 **		pAddrspec(IN)  : The SipAddrSpec structure which will be verify for
 **						 presence of lr parameterer
 **						 
 **		pIsPresentLr(OUT) : Wiil set to true if lr parameter is present in the 
 **							Addrspec
 **
 **		pErr(OUT)	  :	Error structure in case the function fails
 **
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCheckLrParameter
#ifdef ANSI_PROTO
	(SipAddrSpec *pAddrspec,
	Sdf_ty_bool  *pIsPresentLr,
	Sdf_st_error *pErr)
#else
	(pAddrspec, pIsPresentLr,pErr)
	SipAddrSpec *pAddrspec;
	Sdf_ty_bool  *pIsPresentLr;
	Sdf_st_error *pErr;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaCheckLrParameter");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if((pErr == Sdf_co_null) || (pAddrspec == Sdf_co_null))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCheckLrParameter( ): "
			"Invalid Param passed to this function",pErr);
#endif		
        if( pErr != Sdf_co_null)
            pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */


	/* If the Addr Type is SipAddrSipUri then only check for lr
	 * parameter
	 */
#ifdef SDF_TLS
	if((pAddrspec->dType == SipAddrSipUri ) ||
	        (pAddrspec->dType == SipAddrSipSUri ))
#else
	if(pAddrspec->dType == SipAddrSipUri )
#endif
	{
		Sdf_ty_u32bit dCount;
		SipUrl	*pUrl=Sdf_co_null;

		/* Get the SipUrl From the AddrSpec 
		 */
		/*w534*/ (void)sdf_ivk_uaGetUrlFromAddrSpec(pAddrspec,&pUrl,pErr);

		/* Get the param count */
		/*w534*/ (void)sip_getUrlParamCountFromUrl(pUrl,&dCount,\
				(SipError *)&(pErr->stkErrCode));

		if(dCount !=0)
		{
			SipParam *pParam;
			Sdf_ty_u32bit dIndex;

			/* Iterate through the Param list to find the lr parameter
			 * name
			 */

			for(dIndex=0; dIndex < dCount; dIndex++)
			{
				/*w534*/ (void)sip_getUrlParamAtIndexFromUrl(pUrl,&pParam,dIndex,\
						(SipError *)&(pErr->stkErrCode));
				if(Sdf_mc_strcasecmp(pParam->pName,"lr")==0)
				{
					*pIsPresentLr=Sdf_co_true;
					sip_freeSipParam(pParam);
					break;
				}
				sip_freeSipParam(pParam);
			}
		}
		sip_freeSipUrl(pUrl);
	}
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_fn_uaCheckLrParameter");
	return Sdf_co_success;
	
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaDoSendMessageProcs
 **
 ** DESCRIPTION: This function does the procedures that needs to be done before
 **              sending a message on the n/w.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaDoSendMessageProcs
#ifdef ANSI_PROTO
	(Sdf_st_callObject 	*pObject,
	SipMessage   		*pMessage, 
	Sdf_st_overlapTransInfo **ppOverlapTransInfo,
	Sdf_st_msgInfo 		dMessageInfo,
	Sdf_st_error 		*pError)
#else
	(pObject, pMessage, ppOverlapTransInfo, dMessageInfo, pError)
	Sdf_st_callObject *pObject;
	SipMessage   *pMessage; 
	Sdf_st_overlapTransInfo **ppOverlapTransInfo;
	Sdf_st_msgInfo 		dMessageInfo;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
	en_SipMessageType	  dReqRespType;
	Sdf_ty_messageType	  dMethodType	= Sdf_en_unknownRequest;

    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaDoSendMessageProcs");

	dMethodType   = dMessageInfo.dMsgType;		
	dReqRespType  = dMessageInfo.dReqRespType;
 
	dTransactionType = sdf_fn_getTransactionType(pMessage, dMessageInfo, \
						Sdf_co_false, pError);
	
#ifdef SDF_THREAD_SAFE
	/*
	 * Lock the dCallStateMutex. This remains locked till we check that
	 * the message being sent out results in a valid state transition, 
	 * update the destination info structure, send the message and change
	 * the call state
	 */
	sdf_fn_uaLockMutex(0, &(pObject->dCallStateMutex), 0);
#endif

	/* Stop the Session-Timer for messages of type BYE and re-INVITE
	 */
#ifdef SDF_SESSION_TIMER
#if 0
	if (((dMethodType == Sdf_en_invite) || (dMethodType == Sdf_en_bye) || \
		 (dMethodType == Sdf_en_update)) && \
		(dReqRespType == SipMessageRequest) && \
		(pObject->pCallInfo->dState == Sdf_en_callEstablished) && \
		(pObject->pSessionTimer->pTimerId !=Sdf_co_null))
	{
#endif
        if ((dMethodType == Sdf_en_bye) && \
		(dReqRespType == SipMessageRequest) && \
		(pObject->pSessionTimer->pTimerId !=Sdf_co_null))
	{
#ifdef SDF_TRACE
			sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,\
			(Sdf_ty_s8bit*)"Issuing callback sdf_cbk_uaStopTimer"
			"for stopping Sessiom-Timer ", 0, pError);
#endif

		if(sdf_fn_uaStopSessionTimer((Sdf_ty_pvoid *)&(pObject->pSessionTimer->\
			pTimerId),pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaDoSendMessageProcs( ): "
				"Failed to Stop the timer",pError);
#endif
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0, &(pObject->dCallStateMutex));
#endif
			return Sdf_co_fail;
		}
	}
#endif	

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &(pObject->dCallStateMutex));
#endif
	/* Store the Require header passed in Invite and reuse the same 
	 * in the Ack to 2xx */
	if((dMethodType  == Sdf_en_invite) && (dReqRespType == SipMessageRequest))
		/*w534*/ (void)sdf_fn_uaStoreRequireHdr(pObject, pMessage, pError);
	if ((dTransactionType == Sdf_en_overlapTransaction) ||
		(dTransactionType == Sdf_en_RegularUacTransaction) ||
		(dTransactionType == Sdf_en_RegularUasTransaction))
	{
		if (sdf_ivk_uaRetrieveMatchingOverlapTransInfo(pMessage, \
			pObject, ppOverlapTransInfo, Sdf_en_msgModeSend, pError) \
			== Sdf_co_fail)
		{		
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaDoSendMessageProcs( ): "
				"Unable to retrieve matching overlap transaction",pError);
#endif
			return Sdf_co_fail;
		}
	}
	/* As now we are storing the Request Message sent on network only. */
	if(dReqRespType == SipMessageRequest)
	{
        
		    /*
         *  pLastSipMsg will be cloned and not referenced.
         * Also, since sip_cloneSipMessage fn will delete the destination
         * if its not NULL, so there is no need to maintain pTempMessage.
         */
		
		if(dTransactionType == Sdf_en_uacTransaction)
		{

            if (pObject->pUacTransaction->pLastSipMsg != Sdf_co_null)
            {
                sip_freeSipMessage(pObject->pUacTransaction->pLastSipMsg);
            }
            if(sip_initSipMessage(&pObject->pUacTransaction->pLastSipMsg, \
		 	pMessage->dType,(SipError *)&(pError->stkErrCode)) ==SipFail)
		    {
#ifdef SDF_ERROR
			    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				    (Sdf_ty_s8bit *)"sdf_fn_uaDoSendMessageProcs(): "
				    "Initializing SipMessage failed ",pError);
#endif
			/* pErr->errCode=Sdf_en_headerManipulationError; */
			    return Sdf_co_fail;
		    }
            if (sip_cloneSipMessage(pObject->pUacTransaction->pLastSipMsg, \
			            pMessage, (SipError *)&(pError->stkErrCode)) == SipFail)
		    {
#ifdef SDF_ERROR
		     	sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			    	(Sdf_ty_s8bit *)"sdf_fn_uaDoSendMessageProcs(): "
				    "Cloning SIP message from source to destination failed ",\
			    	pError);
#endif
			    return Sdf_co_fail;
		    }
		}
		else if(dTransactionType == Sdf_en_uasTransaction)
		{

            if (pObject->pUasTransaction->pLastSipMsg != Sdf_co_null)
            {
                sip_freeSipMessage(pObject->pUasTransaction->pLastSipMsg);
            }
            if(sip_initSipMessage(&pObject->pUasTransaction->pLastSipMsg, \
		 	pMessage->dType, (SipError *)&(pError->stkErrCode)) ==SipFail)
		    {
#ifdef SDF_ERROR
			    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				    (Sdf_ty_s8bit *)"sdf_fn_uaDoSendMessageProcs(): "
				    "Initializing SipMessage failed ",pError);
#endif
			/* pErr->errCode=Sdf_en_headerManipulationError; */
			    return Sdf_co_fail;
		    }
            if (sip_cloneSipMessage( \
                        pObject->pUasTransaction->pLastSipMsg, \
			            pMessage, (SipError *)&(pError->stkErrCode)) == SipFail)
		    {
#ifdef SDF_ERROR
		     	sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			    	(Sdf_ty_s8bit *)"sdf_fn_uaDoSendMessageProcs(): "
				    "Cloning SIP message from source to destination failed ",\
			    	pError);
#endif
			    return Sdf_co_fail;
		    }
		}
		else
		{
            if ((*ppOverlapTransInfo)->pLastSipMsg != Sdf_co_null)
            {
                sip_freeSipMessage((*ppOverlapTransInfo)->pLastSipMsg);
            }
            if(sip_initSipMessage(&(*ppOverlapTransInfo)->pLastSipMsg, \
		 	pMessage->dType, (SipError *)&(pError->stkErrCode)) ==SipFail)
		    {
#ifdef SDF_ERROR
			    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				    (Sdf_ty_s8bit *)"sdf_fn_uaDoSendMessageProcs(): "
				    "Initializing SipMessage failed ",pError);
#endif
			/* pErr->errCode=Sdf_en_headerManipulationError; */
			    return Sdf_co_fail;
		    }
            if (sip_cloneSipMessage( \
                        (*ppOverlapTransInfo)->pLastSipMsg, \
			            pMessage, \
				    (SipError *)&(pError->stkErrCode)) == SipFail)
		    {
#ifdef SDF_ERROR
		     	sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			    	(Sdf_ty_s8bit *)"sdf_fn_uaDoSendMessageProcs(): "
				    "Cloning SIP message from source to destination failed ",\
			    	pError);
#endif
			    return Sdf_co_fail;
		    }
		}
	}

#ifndef SDF_UASSF
	/* 
	 * Set the variable dSdpInInvite to true if an INVITE
	 * is being sent out with SDP.
	 */
	if(dReqRespType==SipMessageRequest)
	{
		if (sdf_fn_uaCheckSdpInInvite(pObject, dMessageInfo, pMessage, \
								pError) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeOverlapTransInfo(*ppOverlapTransInfo);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_msgBodyError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaDoSendMessageProcs(): "
				"Failed to check for presence of SDP in the message",pError);
#endif
			pError->errCode=Sdf_en_msgBodyError;
			return Sdf_co_fail;
		}
	}
#endif /* SDF_UASSF */

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "\
			"sdf_fn_uaDoSendMessageProcs");
	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaInitMediaStateValidationTable
**
** DESCRIPTION: This API will initialize the Media State Validation Funciton
**				Pointer table.
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitMediaStateValidationTable
#ifdef ANSI_PROTO
(void)
#else
(void)
#endif
{
	dGlbMediaStateValidationTable[Sdf_en_idleMedia] = \
											sdf_fn_uaValidateIdleState;
	dGlbMediaStateValidationTable[Sdf_en_offerSent] = \
											sdf_fn_uaValidateOfferSentState;
	dGlbMediaStateValidationTable[Sdf_en_offerRecv] = \
											sdf_fn_uaValidateOfferRecvState;
	dGlbMediaStateValidationTable[Sdf_en_establishedMedia] =\
											sdf_fn_uaValidateEstablishedState;
	dGlbMediaStateValidationTable[Sdf_en_offerChangeSent] = \
									sdf_fn_uaValidateOfferSentState;
	dGlbMediaStateValidationTable[Sdf_en_offerChangeRecv] = \
									sdf_fn_uaValidateOfferRecvState;
	
	return Sdf_co_success;
}
/*****************************************************************************
** FUNCTION: sdf_fn_uaCheckMediaStateValidity
**
** DESCRIPTION: This API will check the Media State
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaCheckMediaStateValidity
#ifdef ANSI_PROTO
 (Sdf_st_callObject *pOrigCallObj , 
 Sdf_st_callObject *pNewCallObj,
 Sdf_ty_messageType	dMessageType,
 Sdf_st_error *pErr)
#else
 (pOrigCallObj,pNewCallObj,dMessageType,pErr)
  Sdf_st_callObject *pOrigCallObj;
  Sdf_st_callObject *pNewCallObj;
  Sdf_ty_messageType dMessageType;
  Sdf_st_error		*pErr;
#endif
{
	Sdf_ty_mediaState dCurrentMediaState = Sdf_en_invalidMediaState;
	SipMessage	*pMessage;
	Sdf_ty_fptrMediaStateValidationFunc pFuncPtr;

	(void)pErr;
#ifdef SIP_RPR
	if((dMessageType != Sdf_en_invite) && (dMessageType != Sdf_en_update) &&
		(dMessageType != Sdf_en_prack) && (dMessageType != Sdf_en_comet) &&
		(dMessageType != Sdf_en_ack))
#else
	if((dMessageType != Sdf_en_invite) && (dMessageType != Sdf_en_update) &&
		(dMessageType != Sdf_en_comet) && (dMessageType != Sdf_en_ack))

#endif	
	{
		/*No State Change Needed*/
		return Sdf_co_success;
	}

	/*Extract Current Media State*/
	dCurrentMediaState = pOrigCallObj->pMediaInfo->dMediaState;
	
	
 	/* Get the function ptr to call*/ 
	pFuncPtr = dGlbMediaStateValidationTable[dCurrentMediaState];

	/*Get Message from NewCallObject*/
	if(pNewCallObj->pUacTransaction->pSipMsg!=Sdf_co_null)
		pMessage = pNewCallObj->pUacTransaction->pSipMsg;
	else
		pMessage = pNewCallObj->pUasTransaction->pSipMsg;
	
	if ( pFuncPtr(pMessage,pOrigCallObj,pErr) == Sdf_co_success )
		return Sdf_co_success;
	else
		return Sdf_co_fail;
}
/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaMessageSentNtfn 
 **
 ** DESCRIPTION:	Notification function invoked by the application after
 **                 sending the SIP message on the network.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaMessageSentNtfn
#ifdef ANSI_PROTO
	(Sdf_st_callObject       *pObject,
	 Sdf_st_overlapTransInfo *pOverlapTransaction,
	 SipMessage              *pSipMsg, 
	 Sdf_st_msgInfo		  	 dMessageInfo,
	 Sdf_st_eventContext	 *pEventContext,
	 Sdf_st_error            *pError)
#else
	(pObject,pOverlapTransaction,pSipMsg, dMessageInfo, pEventContext, pError)
	 Sdf_st_callObject       *pObject;
	 Sdf_st_overlapTransInfo *pOverlapTransaction;
	 SipMessage              *pSipMsg; 
	 Sdf_st_msgInfo		  	 dMessageInfo;
	 Sdf_st_eventContext	 *pEventContext;
	 Sdf_st_error            *pError;
#endif
{
#ifdef SDF_LINT
#ifdef SDF_TRACE
    Sdf_ty_s8bit		  *pMethod		= Sdf_co_null;
#endif
#else
    Sdf_ty_s8bit		  *pMethod		= Sdf_co_null;
#endif    
	en_SipMessageType	  dReqRespType;
	Sdf_ty_u16bit		  dRespCode		= 0;
	Sdf_ty_messageType	  dMethodType	= Sdf_en_unknownRequest;

#ifdef SDF_TRACE	
	Sdf_ty_s8bit trace[Sdf_co_traceBufferSize];
#endif	

#ifndef SDF_SESSION_TIMER
		(void)*pEventContext;
#endif	
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Entering sdf_fn_uaMessageSentNtfn");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_fn_uaMessageSentNtfn( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}	
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaMessageSentNtfn( ): "
			" Call object parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (pSipMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaMessageSentNtfn( ): "
			" SipMessage parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif  /* End of Param Validation. */

	/* 
	 * Message has been sent. Now display the call flow
	 */
	sdf_fn_uaUpdateCallFlow(pSipMsg, Sdf_co_false);

	dMethodType   = dMessageInfo.dMsgType;		
	dReqRespType  = dMessageInfo.dReqRespType;
	dRespCode     = dMessageInfo.dResponse;
#ifdef SDF_LINT
#ifdef SDF_TRACE    
    pMethod	      = dMessageInfo.pMethod;
#endif
#else
    pMethod	      = dMessageInfo.pMethod;
#endif    

#ifdef SDF_THREAD_SAFE
	/*
	 * Lock the dCallStateMutex.
	 */
	sdf_fn_uaLockMutex(0, &(pObject->dCallStateMutex), 0);
#endif

	if (sdf_ivk_uaChangeCallState(pObject, pOverlapTransaction, pSipMsg, \
		Sdf_en_msgModeSend, pError) == Sdf_co_fail)
	{		
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callStateError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaMessageSentNtfn( ): "
			"Could not change the call state", pError);
#endif
		pError->errCode=Sdf_en_callStateError;

		if(pOverlapTransaction != Sdf_co_null)
		    (void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransaction);
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(pObject->dCallStateMutex));
#endif

		return Sdf_co_fail;
	}
	/* If sending success response to Bye then set the overlap transactions
	 * to Transaction Cancelled state  */
	if((dMethodType == Sdf_en_bye) && (dRespCode >= 200) &&(dRespCode < 300))
		(void)sdf_fn_uaSetOverlapInfoCancelled(pObject, pError);
	/* If method Send it Invite then revert the dInviteInProg to false */
	if( (dMethodType == Sdf_en_invite) && (dReqRespType == SipMessageRequest) )
	   pObject->pCallInfo->dInviteInProg = Sdf_co_false;	
	
#ifndef SDF_UASSF
	/*Change Media State*/
	if(sdf_fn_uaChangeMediaState(pObject, \
			&(pObject->pMediaInfo->dMediaStateChange), dMessageInfo, pSipMsg, \
			Sdf_en_msgModeSend,	pError)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_mediaStateTransitionError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaMessageSentNtfn( ): "
			"Could not change the Media state", pError);
#endif
		pError->errCode=Sdf_en_mediaStateTransitionError;
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(pObject->dCallStateMutex));
#endif
		return Sdf_co_fail;
	}
#endif /* SDF_UASSF */

	/*Now revert the flag*/
	pObject->pMediaInfo->dOfferProgress = Sdf_co_false;

	
	/*
	 * All call state and Media State related processing complete. 
	 * Unlock the dCallStateMutex
	 */
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &(pObject->dCallStateMutex));
#endif

	
	/*
	 * If a CANCEL was sent out, set the dCallerCancelStatus flag to 
	 * Sdf_en_cancelSent. Else if a request other than CANCEL or ACK
	 * was sent out, reset the dCallerCancelStatus flag
	 */
	if(dReqRespType == SipMessageRequest)
	{
		if (dMethodType == Sdf_en_cancel)
		{
			/* Set the dCallerCancelStatus to Sdf_en_cancelSent */
			pObject->pCommonInfo->dCallerCancelStatus = Sdf_en_cancelSent;
		}
		else if (dMethodType != Sdf_en_ack)
		{
			/*  Reset the cancel status here */
			pObject->pCommonInfo->dCallerCancelStatus = Sdf_en_reset;
		}
	}


#ifdef SDF_TRACE
	/*
	 * Print a trace describing message that was just sent out 
	 */
	if (pObject->pCommonInfo->pKey->pCallId != Sdf_co_null)
	{
		if(dReqRespType == SipMessageResponse)
		{
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
				"Call: %s - %d to %s sent.\n", \
				(pObject->pCommonInfo->pKey)->pCallId, dRespCode, pMethod);
	
			sdf_fn_trace(Sdf_en_allTraceLevels, \
				Sdf_en_mesgTraces, (Sdf_ty_s8bit *)trace, 0, pError);
		}
		else
		{
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
				"Call: %s - %s sent.\n", \
				(pObject->pCommonInfo->pKey)->pCallId, pMethod);
	
			sdf_fn_trace(Sdf_en_allTraceLevels, \
				Sdf_en_mesgTraces, (Sdf_ty_s8bit *)trace, 0, pError);
		}
	}
#endif

	
#ifdef SDF_SESSION_TIMER
	if(dReqRespType == SipMessageResponse)
	{
		if ((dRespCode >= 200) && (dRespCode <=299) && \
			((dMethodType == Sdf_en_invite) || \
			 ((dMethodType == Sdf_en_update) && \
			  (sdf_fn_uaCallState(pObject)==Sdf_en_confirmedDialogState))))
		{
			/* Get The Modified Session-Interval */
			SipHeader dTempHeader;
			
			/* Negotiation is complete by now. Hence reset the
			 * dRefreshRequest parameter of session timer -*/
			pObject->pSessionTimer->dRefreshRequest = Sdf_en_refresherInvalid;

			if(sip_getHeader(pSipMsg,SipHdrTypeSessionExpires,\
				&dTempHeader,(SipError*)&(pError->stkErrCode)) != SipFail)
			{
				Sdf_ty_u32bit duration;

				duration = pObject->pSessionTimer->dSessionInterval;

				sip_freeSipHeader( &dTempHeader);

				if (sdf_fn_uaModifySessionInterval(&duration,\
					pObject->pSessionTimer->dRefresher,pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_invalidSessionInterval, \
						(Sdf_ty_s8bit *)"sdf_fn_uaMessageSentNtfn( ): "
						"Could not get SessionTimer Time \n",pError);
#endif
					pError->errCode=Sdf_en_invalidSessionInterval;
					return Sdf_co_fail;
				}
				/* 
				 * Start the Session Timer by issuing the callback 
				 * sdf_cbk_uaStartTimer 
				 */
#ifdef SDF_TRACE
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,\
					(Sdf_ty_s8bit*)"Issuing callback sdf_cbk_uaStartTimer"
					"for starting Sessiom-Timer ", 0, pError);
#endif
            /*Here we need to stop the previously running timer
                 * before starting the new one.
                 */
               /* To start the session timer, we need to do
                * a restart mechanism.
                * SPR-5461
                */
                if(pObject->pSessionTimer->pTimerId != Sdf_co_null)
                {    
                    if(sdf_fn_uaStopSessionTimer((Sdf_ty_pvoid *)\
                                &(pObject->pSessionTimer->pTimerId) ,pError)
                            ==Sdf_co_fail)
                    {
#ifdef SDF_ERROR
                        sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
                                (Sdf_ty_s8bit *)"sdf_fn_uaMessageSentNtfn():"
                                "Failed to Stop the timer",pError);
#endif
                        return Sdf_co_fail;
                    }
                }
				if (sdf_fn_uaStartSessionTimer (pObject, \
					(Sdf_ty_pvoid *)&(pObject->pSessionTimer->pTimerId), \
					duration, pEventContext, pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						pError->errCode, \
						(Sdf_ty_s8bit *)"sdf_fn_uaMessageSentNtfn( ): "
						"Could not Start the SessionTimer  \n",pError);
#endif
					return Sdf_co_fail;
				}
			}
		}
		/*Negotiation is complete by now. Hence reset the
		 * dRefreshRequest parameter of session timer -*/
       	else if ((dRespCode >= 300) && (dRespCode <=699)) 
        {
            /* We need to set the dRefreshRequest to Sdf_en_refresherInvalid
             * since the the refresh request is being responded.
             * An end point which responds negatively to refresh request
             * should have dRefreshRequest as Sdf_en_refresherInvalid.
             * Earlier it was happening for 422, now we do it for
             * all negative responses.
             */
            pObject->pSessionTimer->dRefreshRequest = Sdf_en_refresherInvalid;
            /*For 408 and 481, we need to stop the session.
             */ 
            if((dRespCode == 408) || (dRespCode == 481))
            {
                if(pObject->pSessionTimer->pTimerId != Sdf_co_null)
                {    
                    if(sdf_fn_uaStopSessionTimer((Sdf_ty_pvoid *)\
                                &(pObject->pSessionTimer->pTimerId) ,pError)
                            ==Sdf_co_fail)
                    {
#ifdef SDF_ERROR
                        sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
                                (Sdf_ty_s8bit *)"sdf_fn_uaMessageSentNtfn():"
                                "Failed to Stop the timer",pError);
#endif
                        return Sdf_co_fail;
                    }
                }/*end of if pObject->pSessionTimer->pTimerId !=
                   Sdf_co_null */
            }/*End of if dRespCode == 408*/

           /* CSR_1-7033347  START*/
            pObject->pSessionTimer->dSessionInterval = 
                     pObject->pSessionTimer->dOldSessionInterval;
		    pObject->pSessionTimer->dRefresher = 
                     pObject->pSessionTimer->dOldRefresher;
            pObject->pSessionTimer->dMinSe = 
                     pObject->pSessionTimer->dOldMinSe;
     	   /* CSR_1-7033347 END */
        }
	} /* End of "if type == SipMessageResponse" */
#endif
	
	if( (dReqRespType == SipMessageResponse) && (dRespCode >= 200) )
	{
		if( pOverlapTransaction != Sdf_co_null )
		{
			/*w534*/ (void)sdf_ivk_uaRemoveOverlapInfo(pObject, pOverlapTransaction);
		}
	}

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		               "sdf_fn_uaMessageSentNtfn");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaMuteMediaStream 
 **
 ** DESCRIPTION:	Function used to Mute a given MediaStream based on the 
 **                 type of Mute required.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaMuteMediaStream
#ifdef ANSI_PROTO
	(Sdf_st_mediaStream  *pMediaStream,
	 Sdf_ty_muteType     dType,
	 Sdf_st_error        *pError)
#else
	(pMediaStream,dType,pError)
	 Sdf_st_mediaStream  *pMediaStream;
	 Sdf_ty_muteType     dType;
	 Sdf_st_error        *pError;
#endif
{
	Sdf_ty_s8bit *pTmpValue = Sdf_co_null;
	Sdf_ty_u32bit dSize = 0, dIndex;
	Sdf_st_error	dLocalErr;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "\
			           "sdf_fn_uaMuteMediaStream");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_fn_uaMuteMediaStream( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}	
	if (pMediaStream == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaStream( ): "
			" pMediaStream parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif  /* End of Param Validation. */

	if (dType == Sdf_en_connectionMute)
    {
		(void)sdf_listSizeOf(&(pMediaStream->slConnection), &dSize, pError);

		if (dSize == 0)
        {
			SdpConnection *pConnection;

			if (sip_initSdpConnection(&pConnection, \
					(SipError *) &(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaStream():"
					" Error initializing SdpConnection", pError);
#endif
				return Sdf_co_fail;
			}
			pTmpValue = Sdf_co_null;

			pTmpValue = Sdf_mc_strdupCallHandler("IN");

			if ((sdp_setNetTypeInConnection(pConnection, pTmpValue, \
				(SipError*)&(pError->stkErrCode))) ==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaStream():"
					" Error setting net type in Connection",pError);
#endif
				(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid*)&pTmpValue, \
						&dLocalErr);
				sip_freeSdpConnection(pConnection);
				return Sdf_co_fail;
			}	
			pTmpValue = Sdf_co_null;

			pTmpValue = Sdf_mc_strdupCallHandler("IP4");
			if ((sdp_setAddrTypeInConnection(pConnection, pTmpValue,\
						   (SipError*) &(pError->stkErrCode))) ==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaStream():"
					" Error setting Addr type in Connection",pError);
#endif
				(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid*)&pTmpValue, \
							&dLocalErr);
				sip_freeSdpConnection(pConnection);
				return Sdf_co_fail;
			}	
			
			pTmpValue = Sdf_co_null;

			pTmpValue = Sdf_mc_strdupCallHandler("0.0.0.0");

			if (sdp_setAddrInConnection(pConnection, pTmpValue, (SipError*)\
				&(pError->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaStream():"
					" Error setting Addr to 0 in Connection", pError);
#endif
				(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid*)&pTmpValue, \
							&dLocalErr);
				sip_freeSdpConnection(pConnection);
				return Sdf_co_fail;
			}

			if (sdf_listAppend(&(pMediaStream->slConnection), pConnection,\
						pError) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaStream():"
					" Error appending pConnection in slConnection", pError);
#endif
				sip_freeSdpConnection(pConnection);
				return Sdf_co_fail;
            }
        }
		else
        {
			SdpConnection *pOldConnection = Sdf_co_null;

			if (sdf_fn_uaCloneSdpConnectionList(&(pMediaStream->\
						slPreviousConnection), &(pMediaStream->slConnection),\
						pError) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				 (Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaStream():"
				 " Unable to clone connection lists.", pError);
#endif
				return Sdf_co_fail;
            }

			for (dIndex = 0; dIndex < dSize; dIndex++)
            {
				if (sdf_listGetAt(&(pMediaStream->slConnection), dIndex,\
					(Sdf_ty_pvoid *)&pOldConnection, pError) == Sdf_co_fail)
                {
#ifdef SDF_ERROR
				    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					    (Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaStream():"
					    " Error getting connection at index from slConnection",\
					   	pError);
#endif
				    return Sdf_co_fail;
                }
			    (void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid*)&(pOldConnection->\
							pAddr), pError);

			    pTmpValue = Sdf_mc_strdupCallHandler("0.0.0.0");

			    if (sdp_setAddrInConnection(pOldConnection, pTmpValue,\
						   (SipError*)&(pError->stkErrCode))==SipFail)
			    {
#ifdef SDF_ERROR
				    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				    	(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaStream():"
				    	" Error setting Addr to 0 in Connection", pError);
#endif
					(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid*)&\
							(pOldConnection->pAddr), &dLocalErr);
				    return Sdf_co_fail;
			    }
            }
        }
    }
	else if (dType == Sdf_en_holdSelfMedia)
    {
      pMediaStream->dPrevDirectionAttrib = pMediaStream->dDirectionAttrib;
      if((pMediaStream->dDirectionAttrib == Sdf_en_recvonly) || 
         (pMediaStream->dDirectionAttrib == Sdf_en_sendrecv))
      {
        pMediaStream->dDirectionAttrib = Sdf_en_recvonly;
      }
      else 
      {
        pMediaStream->dDirectionAttrib = Sdf_en_inactive;
      }
    }
	else if (dType == Sdf_en_completeMute)
    {
		pMediaStream->dPrevDirectionAttrib = pMediaStream->dDirectionAttrib;
		pMediaStream->dDirectionAttrib = Sdf_en_inactive;
    }
	else if (dType == Sdf_en_holdPeerMedia)
    {
      pMediaStream->dPrevDirectionAttrib = pMediaStream->dDirectionAttrib;
      if((pMediaStream->dDirectionAttrib == Sdf_en_recvonly) || 
         (pMediaStream->dDirectionAttrib == Sdf_en_sendrecv))
      {
        pMediaStream->dDirectionAttrib = Sdf_en_recvonly;
      }
      else 
      {
        pMediaStream->dDirectionAttrib = Sdf_en_inactive;
      }
    }

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		               "sdf_fn_uaMuteMediaStream");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaUnMuteMediaStream 
 **
 ** DESCRIPTION:	Function used to UnMute a given MediaStream.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaUnMuteMediaStream
#ifdef ANSI_PROTO
	(Sdf_st_mediaStream  *pMediaStream,
	 Sdf_ty_muteType     dType,
	 Sdf_st_error        *pError)
#else
	(pMediaStream,dType,pError)
	 Sdf_st_mediaStream  *pMediaStream;
	 Sdf_ty_muteType     dType;
	 Sdf_st_error        *pError;
#endif
{
	Sdf_ty_u32bit dSize = 0;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "\
			           "sdf_fn_uaUnMuteMediaStream");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_fn_uaUnMuteMediaStream( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}	
	if (pMediaStream == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUnMuteMediaStream( ): "
			" pMediaStream parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif  /* End of Param Validation. */

	/*
     * First check if the MediaStream was muted by changing the direction
	 * attribute. If yes, then just restore the previous direction.
	 */
	if (dType != Sdf_en_connectionMute)
    {
		pMediaStream->dDirectionAttrib = pMediaStream->dPrevDirectionAttrib;
		pMediaStream->dPrevDirectionAttrib = Sdf_en_dirAttribNone;
    }
	/*
     * The MediaStream was muted by setting the IP address in the c-line to
	 * 0.0.0.0
	 */
	else
    {
		/*
         * First check whether any c-lines existed in this MediaStream prior
		 * to muting.
		 */
	    (void)sdf_listSizeOf(&(pMediaStream->slPreviousConnection), &dSize, pError);

	    if (dSize == 0)
        {
			/*
             * This means a new c-line was added with address set to 0.0.0.0
			 * to mute the MediaStream. So delete the only c-line from this
			 * MediaStream.
			 */
			if(sdf_listDeleteAt(&(pMediaStream->slConnection), 0,\
					   pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
			    sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
		            (Sdf_ty_s8bit *)"sdf_fn_uaUnMuteMediaStream(): "
			        "Failed to delete element at index from list",pError);
#endif
			    return Sdf_co_fail;
		    }
        }
		else
        {
			/*
             * Delete the existing set of c-lines from this MediaStream
			 * and restore the previous c-lines.
			 */
		    if (sdf_listDeleteAll(&(pMediaStream->slConnection), pError)\
				    == Sdf_co_fail)
            {
#ifdef SDF_ERROR
			    sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
		            (Sdf_ty_s8bit *)"sdf_fn_uaUnMuteMediaStream(): "
			        "Failed to delete all elements from list",pError);
#endif
			    return Sdf_co_fail;
		    }

			if (sdf_fn_uaCloneSdpConnectionList(&(pMediaStream->\
						slConnection), &(pMediaStream->slPreviousConnection),\
						pError) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				 (Sdf_ty_s8bit *)"sdf_fn_uaUnMuteMediaStream():"
				 " Unable to clone connection lists.", pError);
#endif
				return Sdf_co_fail;
            }

			/* Empty the slPreviousConnection list. */
			if (sdf_listDeleteAll(&(pMediaStream->slPreviousConnection), \
						pError) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
			    sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
		            (Sdf_ty_s8bit *)"sdf_fn_uaUnMuteMediaStream(): "
			        "Failed to delete all elements from list",pError);
#endif
			    return Sdf_co_fail;
		    }
        }
	}

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		               "sdf_fn_uaUnMuteMediaStream");
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaFormRequestLine 
 **
 ** DESCRIPTION:	Function is used to form the Request Line.
 **
 *****************************************************************************/
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
Sdf_ty_retVal sdf_fn_uaFormRequestLine
#ifdef ANSI_PROTO
	(Sdf_st_callObject  	*pCallObj,
	 Sdf_st_overlapTransInfo	*pOverlapTransInfo,
	 const Sdf_ty_s8bit		*pMethod,
	 SipMessage				*pSipMessage,
	 Sdf_st_msgInfo			dMessageInfo,
	 Sdf_st_error       	*pError)
#else
	(pCallObj, pOverlapTransInfo, pMethod, pSipMessage, dMessageInfo, pError)
	Sdf_st_callObject	*pCallObj;
	Sdf_st_overlapTransInfo	*pOverlapTransInfo;
	const Sdf_ty_s8bit		*pMethod;
	SipMessage				*pSipMessage;
	 Sdf_st_msgInfo			dMessageInfo;
	Sdf_st_error       	 	*pError;
#endif
{
	SipReqLine 		*pReqLine = Sdf_co_null;
	Sdf_ty_s8bit	*pTmpMethod = Sdf_co_null;
	SipAddrSpec		*pAddrSpec = Sdf_co_null;
	Sdf_ty_bool  	dIsPresentLr = Sdf_co_false;
	Sdf_ty_messageType dMethodType;	
	Sdf_ty_bool		dIsRequestURISet = Sdf_co_false;
	SipAddrSpec		*pReqlineAddrSpec = Sdf_co_null;
	Sdf_st_error	dLocalErr;
	Sdf_ty_u16bit	dRespCode = 0;

	dMethodType = dMessageInfo.dMsgType;
	dRespCode	= dMessageInfo.dResponse;

	if (sip_initSipReqLine(&pReqLine, (SipError *)&(pError->stkErrCode)) == \
		SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine(): "
			"Failed to init Sip Request Line", pError);
#endif		
		pError->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}	
	pTmpMethod = Sdf_mc_strdupCallHandler(pMethod);
	
	if (sip_setMethodInReqLine(pReqLine, pTmpMethod, \
		(SipError *) &(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formRequestError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine(): "
			"Failed to set method in Request Line", pError);
#endif		
		sip_freeSipReqLine(pReqLine);	
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pTmpMethod,
							 &dLocalErr);
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}	
	pTmpMethod = Sdf_co_null;
	pTmpMethod = Sdf_mc_strdupCallHandler("SIP/2.0");

	if (sip_setVersionInReqLine(pReqLine, pTmpMethod, \
		(SipError *) &(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formRequestError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
			"Failed to set version in Request Line", pError);
#endif		
		sip_freeSipReqLine(pReqLine);	
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pTmpMethod,
							 &dLocalErr);
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}

	/* If any Pre-loaded route set form the Route entry in below 
	 * function */
	if(sdf_fn_uaFormPreloadedRouteSet(pCallObj, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formRequestError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
			"Failed to Form Route Set", pError);
#endif		
		sip_freeSipReqLine(pReqLine);	
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}

	
	/* For Initial Requests or Cancel or Ack to Non-2xx Request the 
	 * Request URI should be picked
	 * from the Request URI present in CommonInfo */

	if( pCallObj->pCommonInfo->pRequestUri != Sdf_co_null )
	{
		if ((dMethodType == Sdf_en_cancel)   ||
			((dMethodType == Sdf_en_ack) && (dRespCode > 299)))
		{
			pAddrSpec = pCallObj->pCommonInfo->pRequestUri; 
			HSS_LOCKEDINCREF(pAddrSpec->dRefCount);
			dIsRequestURISet = Sdf_co_true;
		}
	}

	/* Rule for forming Request URI :
		=> 	For a REGISTER request, the address stored in the init data 
			of the call object is used.
		=> 	If a Route is present and Top route is not a loose router, 
			make the topmost route header the request URI of the message
		=> 	If a Route is present and Top route is a loose router, make
			the last Route entry (which is actual target contact or to addr)
			as Request URI of the message.			
		=> 	If there are no contacts present, the To Address should be used.	
	*/	

	if( dMethodType == Sdf_en_register )
	{
		Sdf_st_transportInfo *pInfo;
		Sdf_ty_s8bit	dScheme[5] = "sip";

		if (sdf_ivk_uaGetRegistrarInfoFromInitData(pCallObj->pInitData,\
			&pInfo,pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
				"Failed to get Registrar Name Info from "
				"Init Data of the call object.", pError);
#endif			
			sip_freeSipReqLine(pReqLine);	
			pError->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}	
#if SDF_TLS
		/* SPR 19927: check for NULL before access */
		if ((Sdf_co_null != pOverlapTransInfo) && 
				(Sdf_en_protoTls == pOverlapTransInfo->pDestInfo->dScheme))
		{
			Sdf_mc_strcpy(dScheme,"sips");
		}
#endif		

		if (sdf_ivk_uaMakeAddrSpecFromConstituents(&pAddrSpec, pInfo->pIp, \
			pInfo->dPort, (Sdf_ty_s8bit *)dScheme, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): Failed"
				" to make an addrspec using the registrar info.", pError);
#endif			
			pError->errCode=Sdf_en_initDataAccessError;
			(void)sdf_ivk_uaFreeTransportInfo(pInfo);
			sip_freeSipReqLine(pReqLine);	
			return Sdf_co_fail;
		}
		(void)sdf_ivk_uaFreeTransportInfo(pInfo);
	}
	else if(dIsRequestURISet == Sdf_co_false)
	{
		/* Check If any Route Set is established. If present then take it */
		(void)sdf_fn_uaRetrieveAddrSpecFromTopmostRoute(pCallObj, \
				pOverlapTransInfo, &pAddrSpec, pError);
		if(pAddrSpec !=Sdf_co_null)
		{
			Sdf_ty_slist 		slMatchedHeaderList;
			Sdf_st_headerList 	*pHeaderList = Sdf_co_null;
			SipHeader 			*pElement = Sdf_co_null;
			Sdf_ty_u32bit		size=0;

			/* If Address Spec is present, then two cases can arise.
			 * If Top Route entry is Loose Router:  
			 * 					Last Route becomes Request URI.
			 * If Top Route entry is Strict Router: The Same Address Spec
			 * 	returned will be Top Route Entry which becomes Request URI.
			 * */

			(void)sdf_fn_uaCheckLrParameter(pAddrSpec, &dIsPresentLr, pError);
			if(dIsPresentLr == Sdf_co_true)
			{
				sip_freeSipAddrSpec(pAddrSpec);
				pAddrSpec = Sdf_co_null;

				if ( Sdf_co_fail == sdf_ivk_uaGetHeaderListByCorrelation( \
					&(pCallObj->slPersistentHeaders),(Sdf_ty_s8bit *)\
					"uat-routeset", &slMatchedHeaderList, pError))
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_minorErrors,  pError->errCode,\
						(Sdf_ty_s8bit *)"sdf_fn_uaFormRequestLine(): "
						"Failed to get Route header list by"
						"correlation  uat-routeset", pError);
#endif			
					sip_freeSipReqLine(pReqLine);	
					return Sdf_co_fail;
				}

				if (sdf_listGetAt(&(slMatchedHeaderList), 0, \
					(Sdf_ty_pvoid *)&pHeaderList,pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
						(Sdf_ty_s8bit*) "sdf_fn_uaFormRequestLine(): Failed"
						"to get Route HeaderList structure", pError);
#endif					
					(void)sdf_listDeleteAll(&slMatchedHeaderList, &dLocalErr);
					sip_freeSipReqLine(pReqLine);	
					return Sdf_co_fail;
				}
				(void)sdf_listSizeOf(&(pHeaderList->slHeaders), &size, pError);

				if (sdf_listGetAt(&(pHeaderList->slHeaders), size - 1, \
					(Sdf_ty_pvoid *)&pElement,	pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
						(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine(): "
						"Failed to get Route header list", pError);
#endif				
					(void)sdf_listDeleteAll(&slMatchedHeaderList, &dLocalErr);
					sip_freeSipReqLine(pReqLine);	
					return Sdf_co_fail;
				}

				if (sip_getAddrSpecFromRouteHdr(pElement, &pAddrSpec, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_formRequestError,\
						(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine(): "
						"Failed to get AddrSpec form Route header", pError);
#endif				
					pError->errCode=Sdf_en_formRequestError;
					(void)sdf_listDeleteAll(&slMatchedHeaderList, &dLocalErr);
					sip_freeSipReqLine(pReqLine);	
					return Sdf_co_fail;
				}
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pError);
			}
		}
		else
		{
			SipHeader 		dActualToHeader;
			Sdf_ty_retVal	dCallInitiatedByUs;
			
			dCallInitiatedByUs = sdf_fn_uaVerifyCaller(pCallObj, pError);

			dActualToHeader.dType = SipHdrTypeTo;

			if (dCallInitiatedByUs == Sdf_co_success)
				dActualToHeader.pHeader = pCallObj->pCommonInfo->pTo->pHeader;
			else
				dActualToHeader.pHeader = pCallObj->pCommonInfo->pFrom->pHeader;

			(void)sip_getAddrSpecFromToHdr(&dActualToHeader, &pAddrSpec,(SipError*)
				&(pError->stkErrCode));
		}
	}
#ifdef SDF_LINT
    if(pAddrSpec == Sdf_co_null)
    {
      sip_freeSipReqLine(pReqLine);
      /* Also free the pre-loaded Route-set if it was formed */
      return Sdf_co_fail;
    }
#endif
#ifdef SDF_TLS
	if(pAddrSpec !=Sdf_co_null)
    {
	if (pAddrSpec->dType == SipAddrSipSUri)
    {
		SipParam *pParam = Sdf_co_null;
		SipUrl	*pSipUrl = Sdf_co_null;
		Sdf_ty_s8bit *pName = Sdf_co_null, *pValue = Sdf_co_null;
		Sdf_ty_u32bit dIndex = 0, dUrlParamListSize = 0;
			
		if (sip_getSUrlFromAddrSpec(pAddrSpec, &pSipUrl, \
					(SipError*)&(pError->stkErrCode)) ==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaFormRequestLine( ):"
				"Failed to get URL from Address spec",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipReqLine(pReqLine);
			return Sdf_co_fail;
		}

		(void)sip_getUrlParamCountFromUrl(pSipUrl, &dUrlParamListSize, \
			(SipError *)&(pError->stkErrCode));

		for (dIndex = 0; dIndex < dUrlParamListSize; dIndex++)
		{
			(void)sip_getUrlParamAtIndexFromUrl(pSipUrl, &pParam, \
				dIndex, (SipError *)&(pError->stkErrCode));

			(void)sip_getNameFromSipParam(pParam, &pName, \
				(SipError *)&(pError->stkErrCode));

			if (Sdf_mc_strcmp(pName, "transport") == 0)
			{
				(void)sip_getValueAtIndexFromSipParam(pParam, &pValue, \
					0, (SipError *)&(pError->stkErrCode));

				if (Sdf_mc_strcasecmp(pValue, "UDP") == 0)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_invalidSipSUrlParamError, (Sdf_ty_s8bit *)\
						"sdf_fn_uaFormRequestLine( ): "
						"Invalid transport param in the Request Uri.",pError);
#endif
					sip_freeSipParam(pParam);
					sip_freeSipUrl(pSipUrl);
					sip_freeSipReqLine(pReqLine);
					pError->errCode=Sdf_en_invalidSipSUrlParamError;
					return Sdf_co_fail;
				}
				else
				{
					sip_freeSipParam(pParam);
					break;
				}
			}
			/* Free the local reference */
			sip_freeSipParam(pParam);
		}
		/* Free the local reference */
		sip_freeSipUrl(pSipUrl);
	}
	}
#endif /* SDF_TLS */

	/*
	 * Clone the address spec formed for insertion into the request line.
	 * This is needed so that any modification to the requestline addr spec
	 * (such as stripping off the maddr) does not influence the header to
	 * which the reqline addr spec is referencing.
	 */

#ifdef SDF_IM
	/* If IM URI is present in Request URI, then through a callback to
	 * application to convert it to other schemes. No need to check the
	 * return value of this callback. If it fails, then returned address
	 * spec will be null */
	if(dIsRequestURISet == Sdf_co_false)
	{
		if(sip_isImUrl(pAddrSpec,(SipError *)&(pError->stkErrCode)) == \
				SipSuccess)
		{
			if(sdf_fn_uaResolveImUrl(&pReqlineAddrSpec, pAddrSpec, pError) \
				!=	Sdf_co_success)
			{
#ifdef SDF_TRACE
				Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
				if(pAddrSpec !=Sdf_co_null) /*Fixed LINT warning */
				{
				(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
					"Failed to Resolve the IM URL" \
					"in  Request Uri :%s", pAddrSpec->u.pUri);
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
						0, pError);
				} /* pAddrSpec !=Sdf_co_null */
#endif
				pReqlineAddrSpec = Sdf_co_null;
			}
		}
	}
/*In subba's changes PERS support is not present in uatk2.1. From UATK3.0 PRES related things are added for ssf support*/
#ifdef SDF_PRES
	/* If PRES URI is present in Request URI, then through a callback to
	 * application to convert it to other schemes. No need to check the
	 * return value of this callback. If it fails, then returned address
	 * spec will be null */
	if(dIsRequestURISet == Sdf_co_false)
	{
		if(sip_isPresUrl(pAddrSpec,(SipError *)&(pError->stkErrCode)) == \
				SipSuccess)
		{
			if(sdf_fn_uaResolvePresUrl(&pReqlineAddrSpec, pAddrSpec, pError) \
				!=	Sdf_co_success)
			{
#ifdef SDF_TRACE
				Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

				sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
					"Failed to Resolve the PRES URL" \
					"in  Request Uri :%s", pAddrSpec->u.pUri);
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
						0, pError);
#endif
				pReqlineAddrSpec = Sdf_co_null;
			}
		}
	}

#endif

/*end-PRES*/
#endif  /*End of IM*/
#if defined (SDF_IM) || defined (SDF_PRES) 
	if( pReqlineAddrSpec == Sdf_co_null )
#endif
	{
		SipUrl			*pReqSipUrl = Sdf_co_null;
		SipParam 		*pReqParam = Sdf_co_null;
		Sdf_ty_s8bit 	*pReqName = Sdf_co_null;
		Sdf_ty_u32bit 	dReqIndex = 0, dReqUrlParamListSize = 0;

		if(pAddrSpec !=Sdf_co_null)
		if (sip_initSipAddrSpec(&pReqlineAddrSpec, pAddrSpec->dType, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
				"Failed to initialize an AddrSpec structure.", pError);
#endif		
			sip_freeSipAddrSpec(pAddrSpec);
			sip_freeSipReqLine(pReqLine);	
			pError->errCode=Sdf_en_formRequestError;
			return Sdf_co_fail;
		}
		if (sip_cloneSipAddrSpec(pReqlineAddrSpec, pAddrSpec, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
				"Failed to clone the addrspec structure for the reqline.",\
			   	pError);
#endif		
			pError->errCode=Sdf_en_formRequestError;
			sip_freeSipAddrSpec(pAddrSpec);
			sip_freeSipAddrSpec(pReqlineAddrSpec);
			sip_freeSipReqLine(pReqLine);	
			return Sdf_co_fail;
		}
						
if(pAddrSpec !=Sdf_co_null) /*Fixed LINT warning */
	if((pAddrSpec->dType == SipAddrSipSUri) || (pAddrSpec->dType == SipAddrSipUri))
	{
		/* get url parmas if method, strip off the method param- as per
		 * RFC, the Request Uri should not contain method param.*/
		if(sdf_ivk_uaGetUrlFromAddrSpec(pReqlineAddrSpec, &pReqSipUrl, \
				pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaFormRequestLine( ):"
				"Failed to get URL from Address spec",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		(void)sip_getUrlParamCountFromUrl(pReqSipUrl, &dReqUrlParamListSize, \
			(SipError *)&(pError->stkErrCode));
		
		for (dReqIndex = 0; dReqIndex < dReqUrlParamListSize; dReqIndex++)
		{
			(void)sip_getUrlParamAtIndexFromUrl(pReqSipUrl, &pReqParam, \
				dReqIndex, (SipError *)&(pError->stkErrCode));

			(void)sip_getNameFromSipParam(pReqParam, &pReqName, \
				(SipError *)&(pError->stkErrCode));

			if (Sdf_mc_strcmp(pReqName, "method") == 0)
			{
				/* delete this param*/
				(void)sip_deleteUrlParamAtIndexInUrl(pReqSipUrl, dReqIndex, \
					(SipError *)&(pError->stkErrCode));

				sip_freeSipParam(pReqParam);
				break;
			}
			sip_freeSipParam(pReqParam);
		}

		/*
		 * If any header params are present in the URL, remove that
		 * as well (acc. to RFC, method and header params should
		 * not be present in Request-URI.
		 */
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
				(pReqSipUrl->pHeader),pError);

		sip_freeSipUrl(pReqSipUrl);
	} /*End: if check to sip or sips (for this only stripoff method param)(for this only stripoff method param)*/
} /*End of IM or PRES check*/		
	

	if (sip_setAddrSpecInReqLine(pReqLine,pReqlineAddrSpec,(SipError *) &(\
		pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
			"Failed to set AddrSpec in Request Line", pError);
#endif		
		pError->errCode=Sdf_en_formRequestError;
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipAddrSpec(pReqlineAddrSpec);
		sip_freeSipReqLine(pReqLine);	
		return Sdf_co_fail;
	}	

	if (sip_setReqLine (pSipMessage, pReqLine, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
			"Failed to set Request Line in Sip Message", pError);
#endif		
		pError->errCode=Sdf_en_formRequestError;
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipAddrSpec(pReqlineAddrSpec);
		sip_freeSipReqLine(pReqLine);	
		return Sdf_co_fail;
	}	

	/* Store the Request-Uri in the	pCallObj->pCommonInfo->pRequestUri
	 * for CACNELing the request that should go with the same request-uri
	 * of the INVITE
	 */
	if ( dMethodType == Sdf_en_invite )
	{
		if( pCallObj->pCommonInfo->pRequestUri != Sdf_co_null )
			sip_freeSipAddrSpec(pCallObj->pCommonInfo->pRequestUri);
			
		pCallObj->pCommonInfo->pRequestUri = pAddrSpec;
		if(pAddrSpec !=Sdf_co_null) /*Fixed LINT warning */
			HSS_LOCKEDINCREF(pAddrSpec->dRefCount);
	}
	
	/* Free the Local References */
	sip_freeSipAddrSpec(pAddrSpec);
	sip_freeSipAddrSpec(pReqlineAddrSpec);
	sip_freeSipReqLine(pReqLine);
	return Sdf_co_success;
}

#else

Sdf_ty_retVal sdf_fn_uaFormRequestLine
#ifdef ANSI_PROTO
	(Sdf_st_callObject  	*pCallObj,
	 Sdf_st_overlapTransInfo	*pOverlapTransInfo,
	 const Sdf_ty_s8bit		*pMethod,
	 SipMessage				*pSipMessage,
	 Sdf_st_msgInfo			dMessageInfo,
	 Sdf_st_error       	*pError)
#else
	(pCallObj, pOverlapTransInfo, pMethod, pSipMessage, dMessageInfo, pError)
	Sdf_st_callObject	*pCallObj;
	Sdf_st_overlapTransInfo	*pOverlapTransInfo;
	const Sdf_ty_s8bit		*pMethod;
	SipMessage				*pSipMessage;
	 Sdf_st_msgInfo			dMessageInfo;
	Sdf_st_error       	 	*pError;
#endif
{
	SipReqLine 		*pReqLine = Sdf_co_null;
	Sdf_ty_s8bit	*pTmpMethod = Sdf_co_null;
	SipAddrSpec		*pAddrSpec = Sdf_co_null;
	Sdf_ty_bool  	dIsPresentLr = Sdf_co_false;
	Sdf_ty_messageType dMethodType;	
	Sdf_ty_bool		dIsRequestURISet = Sdf_co_false;
	SipAddrSpec		*pReqlineAddrSpec = Sdf_co_null;
	Sdf_st_error	dLocalErr;
	Sdf_ty_u16bit	dRespCode = 0;

	dMethodType = dMessageInfo.dMsgType;
	dRespCode	= dMessageInfo.dResponse;

	if (sip_initSipReqLine(&pReqLine, (SipError *)&(pError->stkErrCode)) == \
		SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine(): "
			"Failed to init Sip Request Line", pError);
#endif		
		pError->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}	
	pTmpMethod = Sdf_mc_strdupCallHandler(pMethod);
	
	if (sip_setMethodInReqLine(pReqLine, pTmpMethod, \
		(SipError *) &(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formRequestError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine(): "
			"Failed to set method in Request Line", pError);
#endif		
		sip_freeSipReqLine(pReqLine);	
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pTmpMethod,
							 &dLocalErr);
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}	
	pTmpMethod = Sdf_co_null;
	pTmpMethod = Sdf_mc_strdupCallHandler("SIP/2.0");

	if (sip_setVersionInReqLine(pReqLine, pTmpMethod, \
		(SipError *) &(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formRequestError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
			"Failed to set version in Request Line", pError);
#endif		
		sip_freeSipReqLine(pReqLine);	
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pTmpMethod,
							 &dLocalErr);
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}

	/* If any Pre-loaded route set form the Route entry in below 
	 * function */
	if(sdf_fn_uaFormPreloadedRouteSet(pCallObj, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formRequestError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
			"Failed to Form Route Set", pError);
#endif		
		sip_freeSipReqLine(pReqLine);	
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}

	
	/* For Initial Requests or Cancel or Ack to Non-2xx Request the 
	 * Request URI should be picked
	 * from the Request URI present in CommonInfo */

	if( pCallObj->pCommonInfo->pRequestUri != Sdf_co_null )
	{
		if ((dMethodType == Sdf_en_cancel)   ||
			((dMethodType == Sdf_en_ack) && (dRespCode > 299)))
		{
			pAddrSpec = pCallObj->pCommonInfo->pRequestUri; 
			HSS_LOCKEDINCREF(pAddrSpec->dRefCount);
			dIsRequestURISet = Sdf_co_true;
            /* CSR_1-7588994 Fix Start */
            if(((pAddrSpec->dType == SipAddrSipUri) || 
                        (pAddrSpec->dType == SipAddrSipSUri)) && 
                (Sdf_co_null == pAddrSpec->u.pSipUrl->dPort) && 
                (0 != pCallObj->pCommonInfo->dReqUriPort))
			{            
               pAddrSpec->u.pSipUrl->dPort = sdf_memget(0, sizeof(SIP_U16bit), pError);
               if(Sdf_co_null != pAddrSpec->u.pSipUrl->dPort)
               {
                   *(pAddrSpec->u.pSipUrl->dPort) = pCallObj->pCommonInfo->dReqUriPort;
               }
            }
            if((dMethodType == Sdf_en_ack) && ((300 >= dRespCode) &&
                        (399 <= dRespCode)))
                pCallObj->pCommonInfo->dReqUriPort = 0;
            /* CSR_1-7588994 Fix End */            
		}
	}

	/* Rule for forming Request URI :
		=> 	For a REGISTER request, the address stored in the init data 
			of the call object is used.
		=> 	If a Route is present and Top route is not a loose router, 
			make the topmost route header the request URI of the message
		=> 	If a Route is present and Top route is a loose router, make
			the last Route entry (which is actual target contact or to addr)
			as Request URI of the message.			
		=> 	If there are no contacts present, the To Address should be used.	
	*/	

	if( dMethodType == Sdf_en_register )
	{
		Sdf_st_transportInfo *pInfo;
		Sdf_ty_s8bit	dScheme[5] = "sip";

		if (sdf_ivk_uaGetRegistrarInfoFromInitData(pCallObj->pInitData,\
			&pInfo,pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
				"Failed to get Registrar Name Info from "
				"Init Data of the call object.", pError);
#endif			
			sip_freeSipReqLine(pReqLine);	
			pError->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}	
#if SDF_TLS
		if ((Sdf_co_null !=pOverlapTransInfo ) &&
			   	(Sdf_en_protoTls == pOverlapTransInfo->pDestInfo->dScheme))
		{
			Sdf_mc_strcpy(dScheme,"sips");
		}
#endif		

		if (sdf_ivk_uaMakeAddrSpecFromConstituents(&pAddrSpec, pInfo->pIp, \
                                        pInfo->dPort, (Sdf_ty_s8bit *)dScheme, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): Failed"
				" to make an addrspec using the registrar info.", pError);
#endif			
			pError->errCode=Sdf_en_initDataAccessError;
			(void)sdf_ivk_uaFreeTransportInfo(pInfo);
			sip_freeSipReqLine(pReqLine);	
			return Sdf_co_fail;
		}
		(void)sdf_ivk_uaFreeTransportInfo(pInfo);
	}
	else if(dIsRequestURISet == Sdf_co_false)
	{
		/* Check If any Route Set is established. If present then take it */
		/*w534*/ (void)sdf_fn_uaRetrieveAddrSpecFromTopmostRoute(pCallObj, \
				pOverlapTransInfo, &pAddrSpec, pError);
		if(pAddrSpec !=Sdf_co_null)
		{
			Sdf_ty_slist 		slMatchedHeaderList;
			Sdf_st_headerList 	*pHeaderList = Sdf_co_null;
			SipHeader 			*pElement = Sdf_co_null;
			Sdf_ty_u32bit		size=0;

			/* If Address Spec is present, then two cases can arise.
			 * If Top Route entry is Loose Router:  
			 * 					Last Route becomes Request URI.
			 * If Top Route entry is Strict Router: The Same Address Spec
			 * 	returned will be Top Route Entry which becomes Request URI.
			 * */

			/*w534*/ (void)sdf_fn_uaCheckLrParameter(pAddrSpec, &dIsPresentLr, pError);
			if(dIsPresentLr == Sdf_co_true)
			{
				sip_freeSipAddrSpec(pAddrSpec);
				pAddrSpec = Sdf_co_null;

				if ( Sdf_co_fail == sdf_ivk_uaGetHeaderListByCorrelation( \
					&(pCallObj->slPersistentHeaders),(Sdf_ty_s8bit *)\
					"uat-routeset", &slMatchedHeaderList, pError))
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_minorErrors,  pError->errCode,\
						(Sdf_ty_s8bit *)"sdf_fn_uaFormRequestLine(): "
						"Failed to get Route header list by"
						"correlation  uat-routeset", pError);
#endif			
					sip_freeSipReqLine(pReqLine);	
					return Sdf_co_fail;
				}

				if (sdf_listGetAt(&(slMatchedHeaderList), 0, \
					(Sdf_ty_pvoid *)&pHeaderList,pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
						(Sdf_ty_s8bit*) "sdf_fn_uaFormRequestLine(): Failed"
						"to get Route HeaderList structure", pError);
#endif					
					(void)sdf_listDeleteAll(&slMatchedHeaderList, &dLocalErr);
					sip_freeSipReqLine(pReqLine);	
					return Sdf_co_fail;
				}
				(void)sdf_listSizeOf(&(pHeaderList->slHeaders), &size, pError);

				if (sdf_listGetAt(&(pHeaderList->slHeaders), size - 1, \
					(Sdf_ty_pvoid *)&pElement,	pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
						(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine(): "
						"Failed to get Route header list", pError);
#endif				
					(void)sdf_listDeleteAll(&slMatchedHeaderList, &dLocalErr);
					sip_freeSipReqLine(pReqLine);	
					return Sdf_co_fail;
				}

				if (sip_getAddrSpecFromRouteHdr(pElement, &pAddrSpec, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_formRequestError,\
						(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine(): "
						"Failed to get AddrSpec form Route header", pError);
#endif				
					pError->errCode=Sdf_en_formRequestError;
					(void)sdf_listDeleteAll(&slMatchedHeaderList, &dLocalErr);
					sip_freeSipReqLine(pReqLine);	
					return Sdf_co_fail;
				}
                /* CSR_1-7588994 Fix Start */
                if(((pAddrSpec->dType == SipAddrSipUri) || (pAddrSpec->dType == SipAddrSipSUri)) &&
                        (Sdf_co_null == pAddrSpec->u.pSipUrl->dPort) &&
                        (0 != pCallObj->pCommonInfo->dReqUriPort))
                {                
                   pAddrSpec->u.pSipUrl->dPort = sdf_memget(0, sizeof(SIP_U16bit), pError);
                   if(Sdf_co_null != pAddrSpec->u.pSipUrl->dPort)
                   {
                       *(pAddrSpec->u.pSipUrl->dPort) = pCallObj->pCommonInfo->dReqUriPort;
                   }
                }
                /* CSR_1-7588994 Fix End */                
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pError);
			}
            /* CSR_1-7588994 Fix Start */
            else if(((pAddrSpec->dType == SipAddrSipUri) || 
                        (pAddrSpec->dType == SipAddrSipSUri)) && 
                    (Sdf_co_null == pAddrSpec->u.pSipUrl->dPort) &&
                    (0 != pCallObj->pCommonInfo->dReqUriPort))
            {            
                pAddrSpec->u.pSipUrl->dPort = sdf_memget(0, sizeof(SIP_U16bit), pError);
                if(Sdf_co_null != pAddrSpec->u.pSipUrl->dPort)
                {
                    *(pAddrSpec->u.pSipUrl->dPort) = pCallObj->pCommonInfo->dReqUriPort;
                }
            }
            /* CSR_1-7588994 Fix End */            
		}
		else
		{
			SipHeader 		dActualToHeader;
			Sdf_ty_retVal	dCallInitiatedByUs;
			
			dCallInitiatedByUs = sdf_fn_uaVerifyCaller(pCallObj, pError);

			dActualToHeader.dType = SipHdrTypeTo;

			if (dCallInitiatedByUs == Sdf_co_success)
				dActualToHeader.pHeader = pCallObj->pCommonInfo->pTo->pHeader;
			else
				dActualToHeader.pHeader = pCallObj->pCommonInfo->pFrom->pHeader;

			/*w534*/ (void)sip_getAddrSpecFromToHdr(&dActualToHeader, &pAddrSpec,(SipError*)
				&(pError->stkErrCode));
            /* CSR_1-7588994 Fix Start */
            if(((pAddrSpec->dType == SipAddrSipUri) || 
                        (pAddrSpec->dType == SipAddrSipSUri)) && 
                    (Sdf_co_null == pAddrSpec->u.pSipUrl->dPort) && 
                    (0 != pCallObj->pCommonInfo->dReqUriPort))
            {
                pAddrSpec->u.pSipUrl->dPort = sdf_memget(0, sizeof(SIP_U16bit), pError);
                if(Sdf_co_null != pAddrSpec->u.pSipUrl->dPort)
                {
                    *(pAddrSpec->u.pSipUrl->dPort) = pCallObj->pCommonInfo->dReqUriPort;
                }
            }
            /* CSR_1-7588994 Fix End */            
		}
	}
#ifdef SDF_LINT
    if(pAddrSpec == Sdf_co_null)
    {
      sip_freeSipReqLine(pReqLine);
      /* Also free the pre-loaded Route-set if it was formed */
      return Sdf_co_fail;
    }
#endif
    
#ifdef SDF_TLS
if(pAddrSpec !=Sdf_co_null)
    {    

	if (pAddrSpec->dType == SipAddrSipSUri)
    {
		SipParam *pParam = Sdf_co_null;
		SipUrl	*pSipUrl = Sdf_co_null;
		Sdf_ty_s8bit *pName = Sdf_co_null, *pValue = Sdf_co_null;
		Sdf_ty_u32bit dIndex = 0, dUrlParamListSize = 0;
			
		if (sip_getSUrlFromAddrSpec(pAddrSpec, &pSipUrl, \
					(SipError*)&(pError->stkErrCode)) ==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaFormRequestLine( ):"
				"Failed to get URL from Address spec",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipReqLine(pReqLine);
			return Sdf_co_fail;
		}

		/*w534*/ (void)sip_getUrlParamCountFromUrl(pSipUrl, &dUrlParamListSize, \
			(SipError *)&(pError->stkErrCode));

		for (dIndex = 0; dIndex < dUrlParamListSize; dIndex++)
		{
			/*w534*/ (void)sip_getUrlParamAtIndexFromUrl(pSipUrl, &pParam, \
				dIndex, (SipError *)&(pError->stkErrCode));

			/*w534*/ (void)sip_getNameFromSipParam(pParam, &pName, \
				(SipError *)&(pError->stkErrCode));

			if (Sdf_mc_strcmp(pName, "transport") == 0)
			{
				/*w534*/ (void)sip_getValueAtIndexFromSipParam(pParam, &pValue, \
					0, (SipError *)&(pError->stkErrCode));

				if (Sdf_mc_strcasecmp(pValue, "UDP") == 0)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_invalidSipSUrlParamError, (Sdf_ty_s8bit *)\
						"sdf_fn_uaFormRequestLine( ): "
						"Invalid transport param in the Request Uri.",pError);
#endif
					sip_freeSipParam(pParam);
					sip_freeSipUrl(pSipUrl);
					sip_freeSipReqLine(pReqLine);
					pError->errCode=Sdf_en_invalidSipSUrlParamError;
					return Sdf_co_fail;
				}
				else
				{
					sip_freeSipParam(pParam);
					break;
				}
			}
			/* Free the local reference */
			sip_freeSipParam(pParam);
		}
		/* Free the local reference */
		sip_freeSipUrl(pSipUrl);
	}
}
#endif /* SDF_TLS */

	/*
	 * Clone the address spec formed for insertion into the request line.
	 * This is needed so that any modification to the requestline addr spec
	 * (such as stripping off the maddr) does not influence the header to
	 * which the reqline addr spec is referencing.
	 */

#ifdef SDF_IM
	/* If IM URI is present in Request URI, then through a callback to
	 * application to convert it to other schemes. No need to check the
	 * return value of this callback. If it fails, then returned address
	 * spec will be null */
	if(dIsRequestURISet == Sdf_co_false)
	{
		if(sip_isImUrl(pAddrSpec,(SipError *)&(pError->stkErrCode)) == \
				SipSuccess)
		{
			
            if(sdf_fn_uaResolveImUrl(&pReqlineAddrSpec, pAddrSpec, pError) \
				!=	Sdf_co_success)
			{
#ifdef SDF_TRACE
				Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

				(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
					"Failed to Resolve the IM URL" \
					"in  Request Uri :%s", pAddrSpec->u.pUri);
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
						0, pError);
#endif
				pReqlineAddrSpec = Sdf_co_null;
			}
		}
	}
#ifdef SDF_PRES
	/* If PRES URI is present in Request URI, then through a callback to
	 * application to convert it to other schemes. No need to check the
	 * return value of this callback. If it fails, then returned address
	 * spec will be null */
	if(dIsRequestURISet == Sdf_co_false)
	{
		if(sip_isPresUrl(pAddrSpec,(SipError *)&(pError->stkErrCode)) == \
				SipSuccess)
		{
			if(sdf_fn_uaResolvePresUrl(&pReqlineAddrSpec, pAddrSpec, pError) \
				!=	Sdf_co_success)
			{
#ifdef SDF_TRACE
				Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

				sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
					"Failed to Resolve the PRES URL" \
					"in  Request Uri :%s", pAddrSpec->u.pUri);
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
						0, pError);
#endif
				pReqlineAddrSpec = Sdf_co_null;
			}
		}
	}

#endif
#endif
#if defined (SDF_IM) || defined (SDF_PRES)   
	if( pReqlineAddrSpec == Sdf_co_null )
#endif
	{
		if(Sdf_co_null != pAddrSpec)
		{
		if (sip_initSipAddrSpec(&pReqlineAddrSpec, pAddrSpec->dType, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
				"Failed to initialize an AddrSpec structure.", pError);
#endif		
			sip_freeSipAddrSpec(pAddrSpec);
			sip_freeSipReqLine(pReqLine);	
			pError->errCode=Sdf_en_formRequestError;
			return Sdf_co_fail;
		}
		if (sip_cloneSipAddrSpec(pReqlineAddrSpec, pAddrSpec, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
				"Failed to clone the addrspec structure for the reqline.",\
			   	pError);
#endif		
			pError->errCode=Sdf_en_formRequestError;
			sip_freeSipAddrSpec(pAddrSpec);
			sip_freeSipAddrSpec(pReqlineAddrSpec);
			sip_freeSipReqLine(pReqLine);	
			return Sdf_co_fail;
		}
		}
	}

	if (sip_setAddrSpecInReqLine(pReqLine,pReqlineAddrSpec,(SipError *) &(\
		pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
			"Failed to set AddrSpec in Request Line", pError);
#endif		
		pError->errCode=Sdf_en_formRequestError;
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipAddrSpec(pReqlineAddrSpec);
		sip_freeSipReqLine(pReqLine);	
		return Sdf_co_fail;
	}	

	if (sip_setReqLine (pSipMessage, pReqLine, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
			"Failed to set Request Line in Sip Message", pError);
#endif		
		pError->errCode=Sdf_en_formRequestError;
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipAddrSpec(pReqlineAddrSpec);
		sip_freeSipReqLine(pReqLine);	
		return Sdf_co_fail;
	}	

	/* Store the Request-Uri in the	pCallObj->pCommonInfo->pRequestUri
	 * for CACNELing the request that should go with the same request-uri
	 * of the INVITE
	 */
	if ( dMethodType == Sdf_en_invite )
	{
		if( pCallObj->pCommonInfo->pRequestUri != Sdf_co_null )
			sip_freeSipAddrSpec(pCallObj->pCommonInfo->pRequestUri);
			
        /* CSR 1-6110044 change start */
		/*pCallObj->pCommonInfo->pRequestUri = pAddrSpec;
		HSS_LOCKEDINCREF(pAddrSpec->dRefCount);*/

		/* SPR 19927: check for NULL before access */
		if(Sdf_co_null != pReqlineAddrSpec)
		{
			pCallObj->pCommonInfo->pRequestUri = pReqlineAddrSpec;
			HSS_LOCKEDINCREF(pReqlineAddrSpec->dRefCount);
		}
        /* CSR 1-6110044 change End */
	}
	
	/* Free the Local References */
	sip_freeSipAddrSpec(pAddrSpec);
	sip_freeSipAddrSpec(pReqlineAddrSpec);
	sip_freeSipReqLine(pReqLine);
	return Sdf_co_success;
}
#endif


/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaFormPreloadedRouteSet 
 **
 ** DESCRIPTION:	Function Forms the Route Set from the Init Data.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormPreloadedRouteSet
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pCallObj,
	 Sdf_st_error       *pError)
#else
	(pCallObj, pError)
	Sdf_st_callObject	*pCallObj;
	Sdf_st_error        *pError;
#endif
{
	Sdf_ty_slist 	slRouteHeaderList;
	Sdf_ty_u32bit dRouteSize=0;
	Sdf_st_initData *pInitData = Sdf_co_null;
	Sdf_st_listIterator dListIterator;
	SipAddrSpec		*pAddrSpec = Sdf_co_null;
	Sdf_st_error	dLocalErr;


	/* Check if route set is already formed or not.  If not formed then
	 * Form the Route set and store it in persistant header list */
	/*w534*/ (void)sdf_ivk_uaGetHeaderListByCorrelation(&(pCallObj->slPersistentHeaders), \
		(Sdf_ty_s8bit *)"uat-routeset",	&slRouteHeaderList, pError);

	(void)sdf_listSizeOf(&slRouteHeaderList, &dRouteSize, pError);

	(void)sdf_listDeleteAll(&slRouteHeaderList, pError);

	if( dRouteSize == 0 )
	{
		Sdf_st_headerList 	*pRouteList=Sdf_co_null;
		SipHeader			*pDestHeader = Sdf_co_null;

		/*
		 * Initialize a HeaderList structure to hold the 
		 * headers.
		 */
		if (sdf_ivk_uaInitHeaderList(&pRouteList, SipHdrTypeRoute, \
			(Sdf_ty_s8bit *)"uat-routeset", pError) ==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormPreloadedRouteSet( ):  "
				"Failed to Initialize HeaderList ",pError);
#endif
			return Sdf_co_fail;
		}

		/*w534*/ (void)sdf_ivk_uaGetInitDataFromCallObject(pCallObj, &pInitData, pError);

		dRouteSize = 0;
		(void)sdf_listSizeOf(&(pInitData->slPreLoadedRoute), &dRouteSize, pError);

		if(dRouteSize !=0)
		{
			(void)sdf_listInitIterator(&(pInitData->slPreLoadedRoute), \
				&dListIterator, pError);

			while (dListIterator.pCurrentElement != Sdf_co_null)
			{
				SipHeader *pSrcHeader;

				pSrcHeader = (SipHeader *)\
					(dListIterator.pCurrentElement->pData);

				if (sip_initSipHeader(&pDestHeader, SipHdrTypeRoute, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
					pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
						(Sdf_ty_s8bit *)"sdf_fn_uaFormPreloadedRouteSet( ): "
						"Failed to init dest header",pError);
#endif			
					(void)sdf_ivk_uaFreeInitData(pInitData);
					(void)sdf_ivk_uaFreeHeaderList(pRouteList);
					return Sdf_co_fail;
				}
				if (sip_cloneSipHeader(pDestHeader, pSrcHeader, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
					pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
						(Sdf_ty_s8bit *)"sdf_fn_uaFormPreloadedRouteSet( ): "
						"Failed to clone a header",pError);
#endif			
					(void)sdf_ivk_uaFreeInitData(pInitData);
					(void)sdf_ivk_uaFreeHeaderList(pRouteList);
					sip_freeSipHeader(pDestHeader);
					(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
						(Sdf_ty_pvoid *)&pDestHeader, &dLocalErr);
					return Sdf_co_fail;
				}


				if(sdf_listAppend(&(pRouteList->slHeaders), \
					(Sdf_ty_pvoid)pDestHeader,pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_headerListAccessError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaFormPreloadedRouteSet(): "
						"Failed to append Route header to "
						"HeaderList structure",pError);
#endif
					pError->errCode=Sdf_en_headerListAccessError;
					(void)sdf_ivk_uaFreeInitData(pInitData);
					(void)sdf_ivk_uaFreeHeaderList(pRouteList);
					sip_freeSipHeader(pDestHeader);
					(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
						(Sdf_ty_pvoid *)&pDestHeader, &dLocalErr);
					return Sdf_co_fail;
				}
				(void)sdf_listNext(&dListIterator, pError);
			}

		}
		(void)sdf_ivk_uaFreeInitData(pInitData);
		/* Last entry should be either To Header or the Application
			 * set Req URI */
		/*w534*/ (void)sip_initSipHeader(&pDestHeader, SipHdrTypeRoute, \
				(SipError *)&(pError->stkErrCode));

		if(pCallObj->pCommonInfo->pRequestUri != Sdf_co_null)
		{
			pAddrSpec = pCallObj->pCommonInfo->pRequestUri; 
			HSS_LOCKEDINCREF(pAddrSpec->dRefCount);
		}
		else
		{
			SipHeader 		dActualToHeader;
			Sdf_ty_retVal	dCallInitiatedByUs;
			
			dCallInitiatedByUs = sdf_fn_uaVerifyCaller(pCallObj, pError);

			dActualToHeader.dType = SipHdrTypeTo;

			if (dCallInitiatedByUs == Sdf_co_success)
				dActualToHeader.pHeader = pCallObj->pCommonInfo->pTo->pHeader;
			else
				dActualToHeader.pHeader = pCallObj->pCommonInfo->pFrom->pHeader;

			/*w534*/ (void)sip_getAddrSpecFromToHdr(&dActualToHeader, &pAddrSpec,(SipError*)
				&(pError->stkErrCode));
		}

		if(SipFail == sip_setAddrSpecInRouteHdr(pDestHeader, pAddrSpec, \
						(SipError*)&(pError->stkErrCode)))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormPreloadedRouteSet(): "
				"Failed to set the address spec in Route" ,pError);
#endif
			pError->errCode=Sdf_en_callObjectAccessError;
			(void)sdf_ivk_uaFreeHeaderList(pRouteList);
			sip_freeSipAddrSpec(pAddrSpec);
			sip_freeSipHeader(pDestHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pDestHeader,\
				&dLocalErr);
			return Sdf_co_fail;
		}
		sip_freeSipAddrSpec(pAddrSpec);

		if(Sdf_co_fail == sdf_listAppend(&(pRouteList->slHeaders), \
						(Sdf_ty_pvoid)pDestHeader,pError))
		{
			(void)sdf_ivk_uaFreeHeaderList(pRouteList);
			sip_freeSipHeader(pDestHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pDestHeader,\
				pError);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormPreloadedRouteSet(): "
				"Failed to append the address spec in Route" ,pError);
#endif
			pError->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}

		if ((sdf_listAppend(&(pCallObj->slPersistentHeaders), \
			(Sdf_ty_pvoid)pRouteList, pError)) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormPreloadedRouteSet(): "
				"Failed to append Supported HeaderList to "
				"Persistant header List ",pError);
#endif
			pError->errCode=Sdf_en_callObjectAccessError;
			(void)sdf_ivk_uaFreeHeaderList(pRouteList);
			return Sdf_co_fail;
		}

		/* Set a flag indicating that it is a preloaded Set only.
		 * It need to be updated on getting first RPR or 2xx for Invite */
		pCallObj->pCommonInfo->dPreloadedSet = Sdf_co_true;
	}
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaFormViaHeader
 **
 ** DESCRIPTION:	Function is used to form Via Header.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormViaHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pCallObj,
	 Sdf_st_overlapTransInfo	*pOverlapTransInfo,
	 Sdf_ty_messageType dMethodType,
	 SipMessage			*pSipMessage,
	 Sdf_st_error       *pError)
#else
	(pCallObj, pOverlapTransInfo, dMethodType, pSipMessage, pError)
	Sdf_st_callObject	*pCallObj;
	Sdf_st_overlapTransInfo	*pOverlapTransInfo;
	Sdf_ty_messageType	dMethodType;	
	SipMessage			*pSipMessage,
	Sdf_st_error        *pError;
#endif
{
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
	SipHeader 				*pHeader = Sdf_co_null;
	Sdf_ty_s8bit			*pTemp = Sdf_co_null;
    Sdf_ty_protocol         dTempScheme = Sdf_en_protoNone;
	Sdf_st_transportInfo *pDestInfo = Sdf_co_null;
	Sdf_st_msgInfo	dMessageInfo;
	Sdf_ty_s8bit 	dTemp[Sdf_co_smallBufferSize]="";
	Sdf_ty_s8bit 	*pVia = Sdf_co_null;
	Sdf_ty_u16bit	dRespCode = 0;
	Sdf_ty_s8bit	*pViaBranch = Sdf_co_null;
	Sdf_st_error	dLocalErr;
	Sdf_ty_bool     dTransportSet = Sdf_co_false;
#ifdef SDF_TLS
	SipReqLine      *pReqLine = Sdf_co_null;
	SipAddrSpec     *pReqLineAddrSpec = Sdf_co_null;
	Sdf_ty_bool     dIsPresentLr = Sdf_co_false;
#endif


	dMessageInfo.pMethod = Sdf_co_null;
	dMessageInfo.dReqRespType = SipMessageRequest;
	dMessageInfo.dMsgType = dMethodType;
	dMessageInfo.dResponse = 0;

	dTransactionType = sdf_fn_getTransactionType(pSipMessage, dMessageInfo, \
		   					Sdf_co_false, pError);

	/* * Setting the Via Header * */
	if (sip_initSipHeader(&pHeader, SipHdrTypeVia,(SipError *)&(\
		pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormViaHeader( ): "
			"Failed to get Via header from Sip message", pError);
#endif		
		pError->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/* To fill the sentby in via header */

	pTemp = (Sdf_ty_s8bit *) sdf_memget(Sdf_mc_callHandlerMemId, \
				Sdf_co_ViaSentByLength, pError); 

	if(Sdf_co_null == pTemp)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormViaHeader( ): "
			"pTemp - memeory allocation failure", pError);
#endif
		pError->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}
	Sdf_mc_strcpy(pTemp, "SIP/2.0/");

#ifdef SDF_TLS
	/* 
	 * Check if the AddrSpec that is to be set in the Request Uri contains SIPS
	 * URI.
	 */

	/* 
	 * First check If any Route Set is present and the topmost route is a loose
	 * router.
	 */
	/*w534*/ (void)sdf_fn_uaRetrieveAddrSpecFromTopmostRoute(pCallObj, \
				pOverlapTransInfo, &pReqLineAddrSpec, pError);

	if(pReqLineAddrSpec !=Sdf_co_null)
	{
        /*w534*/ (void)sdf_fn_uaCheckLrParameter(pReqLineAddrSpec, &dIsPresentLr, pError);
		if ((dIsPresentLr == Sdf_co_true) && \
			(pReqLineAddrSpec->dType == SipAddrSipSUri))
		{
			Sdf_mc_strcat(pTemp, "TLS");
			dTransportSet = Sdf_co_true;
		}
		/* Free the local reference. */
		sip_freeSipAddrSpec(pReqLineAddrSpec);
	}

	if (dTransportSet != Sdf_co_true)
	{
		if(sip_getReqLineFromSipReqMsg(pSipMessage, &pReqLine, \
					(SipError *)&(pError->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaFormViaHeader( ):"
				"Failed to get Request Line from Sip Message",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipHeader(pHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
				&pHeader, &dLocalErr);
			return Sdf_co_fail;
		}

		if (sip_getAddrSpecFromReqLine(pReqLine, &pReqLineAddrSpec, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaFormViaHeader( ):"
				"Failed to get Address Spec from Request Line",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipReqLine(pReqLine);
			sip_freeSipHeader(pHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
				&pHeader, &dLocalErr);	
			return Sdf_co_fail;
		}

		if (pReqLineAddrSpec->dType == SipAddrSipSUri)
		{
			Sdf_mc_strcat(pTemp, "TLS");
			dTransportSet = Sdf_co_true;
		}

		sip_freeSipAddrSpec(pReqLineAddrSpec);
		sip_freeSipReqLine(pReqLine);
	}
#endif /* SDF_TLS */
	if (dTransportSet != Sdf_co_true)
	{
		if (sdf_ivk_uaGetDestInfoFromTransaction(pCallObj->pUacTransaction,\
				pOverlapTransInfo, &pDestInfo, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaFormViaHeader( ): "
				"Failed to get Destination Info from Call Object", pError);
#endif		
			sip_freeSipHeader(pHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
				&pHeader, &dLocalErr);
			pError->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}

        if(pDestInfo->dScheme == Sdf_en_protoNone)
            dTempScheme = pCallObj->pCommonInfo->dScheme;
        else
            dTempScheme = pDestInfo->dScheme;

        if(dTempScheme == Sdf_en_protoTcp)
			Sdf_mc_strcat(pTemp, "TCP");
#ifdef SDF_TLS
        else if(dTempScheme == Sdf_en_protoTls)
			Sdf_mc_strcat(pTemp, "TLS");
#endif
        else if(dTempScheme == Sdf_en_protoUdp)
			Sdf_mc_strcat(pTemp, "UDP");
        else
			Sdf_mc_strcat(pTemp, "UDP");
	}

	(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);

	if (sip_setSentProtocolInViaHdr(pHeader, pTemp,(SipError *)\
		&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormViaHeader(): "
			"Failed to set Sent Protocol in Via header", pError);
#endif		
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
			&pHeader, &dLocalErr);
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}
		
	if (sdf_ivk_uaGetTransportInfoFromInitData(pCallObj->pInitData, \
		&pDestInfo, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormViaHeader(): "
			"Failed to get Transport Info from Init Data", pError);
#endif		
		pError->errCode=Sdf_en_initDataAccessError;
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
			&pHeader, &dLocalErr);
		return Sdf_co_fail;
	}
	
	
	if(pDestInfo->pIp == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormViaHeader( ): "
			"Empty IPAddress in DestTransport Info of call profile ",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
			&pHeader, &dLocalErr);
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
		return Sdf_co_fail;
	}

#ifdef SDF_IPV6
/*Change from UATK 2.1 Start 4.0 fix*/
	/* Radha for IPv6
	 check for IPv6 Address*/
	if (Sdf_mc_strstr(pDestInfo->pIp,":") !=Sdf_co_null)
	{
		/* for IPV6 with "["*/
		if (Sdf_mc_strstr(pDestInfo->pIp, "[") != Sdf_co_null )
		{
			sdf_fn_uaSprintf(dTemp, "%s:%d", pDestInfo->pIp, pDestInfo->dPort);
		}
		else
			sdf_fn_uaSprintf(dTemp, "[%s]:%d", pDestInfo->pIp, pDestInfo->dPort);
	} /* for Ipv4 address*/
	else
			sdf_fn_uaSprintf(dTemp, "%s:%d", pDestInfo->pIp, pDestInfo->dPort);
#else
		/* IPv4 address string will never have ":"*/
		sdf_fn_uaSprintf(dTemp, "%s:%d", pDestInfo->pIp, pDestInfo->dPort);
#endif                 
/*Change from UATK 2.1 End*/

	pVia = (Sdf_ty_s8bit *) sdf_memget (Sdf_mc_callHandlerMemId, \
		Sdf_mc_strlen(dTemp)+ 1, pError);

	if(Sdf_co_null == pVia)
	{
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
			&pHeader, &dLocalErr);
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
		pError->errCode = Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	Sdf_mc_strcpy(pVia, dTemp);

	if(sip_setSentByInViaHdr(pHeader, pVia, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
	{	
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormViaHeader( ): "
			"Failed to set SentBy in Via header", pError);	
#endif		
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
			&pHeader, &dLocalErr);
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}	

	/* Added Branch paramater to the Via header. For New Request create
	 * New Via branch paramater except for ACK.  For ACK insert same
	 * Via branch as that send in INVITE.
	 * Store the Via Branch into the UAC branch key*/

	if( dMethodType == Sdf_en_ack )
		/*w534*/ (void)sdf_ivk_uaGetRespCodeFromSipMessage( 
			pCallObj->pUacTransaction->pSipMsg, &dRespCode, pError);
	if(dMethodType == Sdf_en_cancel)
	{
		pViaBranch = Sdf_mc_strdupAccessor( \
						pCallObj->pCommonInfo->pKey->pLocalViaBranch);
	}
	else if((dMethodType == Sdf_en_ack) && (dRespCode > 299))
	{
		SipHeader 		dViaHeader;

		#ifndef SDF_LINT 
		SipBool			dRetVal = SipSuccess;
		#endif
		

		

		Sdf_ty_s8bit	*pIncViaBranch = Sdf_co_null;

		/* Get the Via Branch from the incoming response message.
		 * The same Via Branch should be used for ACK also */

#ifdef SDF_LINT

		if( sip_getHeader( pCallObj->pUacTransaction->pSipMsg, \
				SipHdrTypeVia, &dViaHeader, (SipError *) &(pError->stkErrCode))!= SipFail )
		{
				
				/*w534*/ (void)sdf_fn_GetBranchFromViaHdr(&dViaHeader,\
				&pIncViaBranch, pError);
		
		if( pIncViaBranch != Sdf_co_null )
			pViaBranch = Sdf_mc_strdupAccessor(pIncViaBranch);
		else
			pViaBranch = Sdf_mc_strdupAccessor( \
						pCallObj->pCommonInfo->pKey->pLocalViaBranch);

		sip_freeSipHeader(&dViaHeader);
		}

#else
		dRetVal = sip_getHeader( pCallObj->pUacTransaction->pSipMsg, \
				SipHdrTypeVia, &dViaHeader, (SipError *) &(pError->stkErrCode));
		
		if( dRetVal != SipFail )
			/*w534*/ (void)sdf_fn_GetBranchFromViaHdr(&dViaHeader,\
				&pIncViaBranch, pError);
		
		if( pIncViaBranch != Sdf_co_null )
			pViaBranch = Sdf_mc_strdupAccessor(pIncViaBranch);
		else
			pViaBranch = Sdf_mc_strdupAccessor( \
						pCallObj->pCommonInfo->pKey->pLocalViaBranch);
		
		if(dRetVal != SipFail )
			sip_freeSipHeader(&dViaHeader);
	
#endif	/* End of SDF_LINT*/
}
	else
	{
		Sdf_ty_s8bit	*pLocalBranch = Sdf_co_null;

		(void)sdf_fn_uaGenerateViaBranch( pDestInfo, &pLocalBranch);

		/* Add Mazic Cookie to the Via branch */
		pViaBranch = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_callHandlerMemId, \
				Sdf_mc_strlen(pLocalBranch) + 1 + \
				Sdf_mc_strlen(Sdf_co_viaMagicCookie), \
				(Sdf_st_error *)Sdf_co_null);
	
		if(Sdf_co_null == pViaBranch)
		{
			pError->errCode = Sdf_en_memoryError;
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,  \
							  (Sdf_ty_pvoid *)&(pLocalBranch), Sdf_co_null);

			sip_freeSipHeader(pHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
							  &pHeader, &dLocalErr);
			return Sdf_co_fail;
		}

		(void)sdf_fn_uaSprintf(pViaBranch, "%s%s", Sdf_co_viaMagicCookie,	\
						pLocalBranch);

		(void)sdf_memfree(Sdf_mc_callHandlerMemId,  \
				(Sdf_ty_pvoid *)&(pLocalBranch), Sdf_co_null);

		if(dTransactionType == Sdf_en_uacTransaction)  
		{
			if( pCallObj->pCommonInfo->pKey->pLocalViaBranch != Sdf_co_null)

				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *) \
					&(pCallObj->pCommonInfo->pKey->pLocalViaBranch), \
					Sdf_co_null);

			pCallObj->pCommonInfo->pKey->pLocalViaBranch = \
							Sdf_mc_strdupAccessor(pViaBranch);
		}
		else if ( pOverlapTransInfo != Sdf_co_null )
		{
			if( pOverlapTransInfo->pOverlapViaBranch != Sdf_co_null )
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *) \
					&(pOverlapTransInfo->pOverlapViaBranch), Sdf_co_null);

			pOverlapTransInfo->pOverlapViaBranch = \
					Sdf_mc_strdupAccessor(pViaBranch);
		}
		else
		{
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,  \
					(Sdf_ty_pvoid *)&(pViaBranch), Sdf_co_null);
			pViaBranch = Sdf_co_null;
		}
	}
	(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);

	if( sdf_fn_InsertBranchFromViaHdr(pHeader, pViaBranch, pError) == \
					Sdf_co_fail )
	{
		pError->errCode = pError->stkErrCode ;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormViaHeader( ): "
			"Failed to insert Via Branch into Via header", pError);
#endif		
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
			&pHeader, &dLocalErr);
		return Sdf_co_fail;
	}

	if (sip_insertHeaderAtIndex(pSipMessage, pHeader, (Sdf_ty_u32bit)0,(\
		SipError *) &(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormViaHeader( ): "
			"Failed to insert Via header in Sip Message", pError);
#endif		
		sip_freeSipHeader(pHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
			&pHeader, &dLocalErr);
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}	

	sip_freeSipHeader(pHeader);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
		&pHeader, Sdf_co_null);

	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaCopyRouteToMessage
 **
 ** DESCRIPTION:	Store the Route Header into the Processed Hdr.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaCopyRouteToMessage
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pCallObj,
	 SipMessage			*pSipMessage,
	 Sdf_st_error       *pError)
#else
	(pCallObj, pSipMessage, pError)
	Sdf_st_callObject	*pCallObj;
	SipMessage			*pSipMessage;
	Sdf_st_error        *pError;
#endif
{
	Sdf_ty_u32bit	dRouteSize = 0;
	Sdf_ty_u32bit	dIndex = 0;

	(void)sdf_listSizeOf(&(pCallObj->pCommonInfo->slInvRoute),&dRouteSize, pError);

	if(dRouteSize !=0)
	{
		Sdf_st_listIterator dListIterator;

		(void)sdf_listInitIterator(&(pCallObj->pCommonInfo->slInvRoute), \
			&dListIterator, pError);

		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			SipHeader *pSrcHeader;

			pSrcHeader = (SipHeader *)\
				(dListIterator.pCurrentElement->pData);

			if(sip_insertHeaderAtIndex(pSipMessage, pSrcHeader, \
				dIndex, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaCopyRouteToMessage( ): "
					"Failed to insert Sip Route header",pError);
#endif			
				return Sdf_co_fail;
			}
			dIndex++;
			(void)sdf_listNext(&dListIterator, pError);
		}
	}
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaCorrectRouteHeader
 **
 ** DESCRIPTION:	Correct Route Header based on first route entry.
 **					If the 1st Route entry is an loose router, then
 **					delete the last route entry which is actual target
 **					URI.
 **					If the 1st route entry is a strict router then remove
 **					that entry because that is moved to Request URI.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaCorrectRouteHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pCallObj,
	 SipMessage			*pSipMessage,
	 Sdf_st_error       *pError)
#else
	(pCallObj, pSipMessage, pError)
	Sdf_st_callObject	*pCallObj;
	SipMessage			*pSipMessage;
	Sdf_st_error        *pError;
#endif
{
	Sdf_ty_u32bit	dCount = 0;
	Sdf_ty_u32bit	dDeleteIndex = 0;
	SipAddrSpec		*pAddrSpec = Sdf_co_null;
	Sdf_ty_bool		dIsPresentLr = Sdf_co_false;
	SipHeader		dRouteHdr;

	pCallObj = pCallObj;
	
	/*w534*/ (void)sip_getHeaderCount(pSipMessage,SipHdrTypeRoute, &dCount, \
			(SipError *)&(pError->stkErrCode));

	if( dCount != 0 )
	{
		/* Get the 1st route entry */
		/*w534*/ (void)sip_getHeaderAtIndex(pSipMessage, SipHdrTypeRoute,
				&dRouteHdr, 0, (SipError *)&(pError->stkErrCode));

		/*w534*/ (void)sip_getAddrSpecFromRouteHdr(&dRouteHdr, \
			&pAddrSpec, (SipError *)&(pError->stkErrCode));

		/*w534*/ (void)sdf_fn_uaCheckLrParameter(pAddrSpec, &dIsPresentLr, pError);
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipHeader(&dRouteHdr);

		if( dIsPresentLr == Sdf_co_true )
			dDeleteIndex = dCount - 1;
		else
			dDeleteIndex = 0;

		if (sip_deleteHeaderAtIndex(pSipMessage, SipHdrTypeRoute, \
				dDeleteIndex,(SipError *)&(pError->stkErrCode)) == SipFail) 
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formRequestError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaCorrectRouteHeader( ): "
				"Failed to Remove Last Route", pError);
#endif			
			pError->errCode=Sdf_en_formRequestError;
			return Sdf_co_fail;
		}
	}
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaStoreRouteHeader
 **
 ** DESCRIPTION:	This function is used to store the Route header
 **					Sent in Invite.  This can be reused while sending
 **					Cancel to the invite.  Before adding new header list
 **					Delete previous list.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaStoreRouteHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pCallObj,
	 SipMessage			*pSipMessage,
	 Sdf_st_error       *pError)
#else
	(pCallObj, pSipMessage, pError)
	Sdf_st_callObject	*pCallObj;
	SipMessage			*pSipMessage;
	Sdf_st_error        *pError;
#endif
{
	Sdf_ty_u32bit dCount=0;
	Sdf_ty_u32bit dIndex=0;

	/*w534*/ (void)sip_getHeaderCount(pSipMessage,SipHdrTypeRoute ,\
		&dCount,(SipError *)&(pError->stkErrCode));

	(void)sdf_listDeleteAll(&(pCallObj->pCommonInfo->slInvRoute),pError);
	if(dCount !=0)
	{
		for( dIndex=0; dIndex<dCount; dIndex++)
		{
			SipHeader *pHeader=Sdf_co_null;

			if (sip_initSipHeader(&pHeader, SipHdrTypeAny, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formResponseError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaStoreRouteHeader( ): "
					"Failed to init Route header from message", pError);
#endif			
				pError->errCode=Sdf_en_formResponseError;
				return Sdf_co_fail;
			}
			pHeader->dType=SipHdrTypeRoute;

			if (sip_getHeaderAtIndex(pSipMessage, SipHdrTypeRoute,pHeader, \
				(Sdf_ty_u32bit)dIndex, (SipError *)&(pError->stkErrCode)) \
				== SipFail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formRequestError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaStoreRouteHeader( ): "
					"Failed to get Route header from message", pError);
#endif			
				pError->errCode=Sdf_en_formRequestError;
   				/* klockwork fix */
				sip_freeSipHeader(pHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid *)&pHeader, pError);

				return Sdf_co_fail;
			}
			(void)sdf_listInsertAt(&(pCallObj->pCommonInfo->slInvRoute),
                    dIndex, (Sdf_ty_pvoid)pHeader, pError);
		}
	}
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaFormCseqHeader
 **
 ** DESCRIPTION:	This function is used to Form the Cseq Header
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormCseqHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pCallObj,
	 Sdf_st_overlapTransInfo	*pOverlapTransInfo,
	 SipMessage			*pSipMessage,
	 const Sdf_ty_s8bit		*pMethod,
	 Sdf_st_error       *pError)
#else
	(pCallObj, pOverlapTransInfo, pSipMessage, pMethod, pError)
	 Sdf_st_callObject  *pCallObj;
	 Sdf_st_overlapTransInfo	*pOverlapTransInfo;
	 SipMessage			*pSipMessage;
	 const Sdf_ty_s8bit		*pMethod;
	 Sdf_st_error       *pError;
#endif
{
	SipHeader		*pHeader = Sdf_co_null;
	Sdf_ty_s8bit	*pTmpMethod = Sdf_co_null;
	Sdf_ty_messageType dMethodType;
	Sdf_ty_transactionType	dTxnType;
	Sdf_st_msgInfo		dMessageInfo;

	/*w534*/ (void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMethodType, pError);

	dMessageInfo.pMethod = Sdf_co_null;
	dMessageInfo.dReqRespType = SipMessageRequest;
	dMessageInfo.dMsgType = dMethodType;
	dMessageInfo.dResponse = 0;

	dTxnType = sdf_fn_getTransactionType(pSipMessage, dMessageInfo, \
		   					Sdf_co_false, pError);

	if ( SipFail == sip_initSipHeader(&pHeader, SipHdrTypeCseq, \
			(SipError *) &(pError->stkErrCode)))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormCseqHeader( ): "
			"Failed to Init Cseq Header", pError);
#endif		
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}	
	
	/*
	 * For PRACK/COMET, pick up Cseq from the pOverlapTransInfo structure
	 */
#ifdef SIP_RPR
	if ( (dMethodType == Sdf_en_prack) || (dMethodType == Sdf_en_comet) ||
		 (dTxnType == Sdf_en_RegularUacTransaction))
#else
	if (  (dMethodType == Sdf_en_comet) ||\
		(dTxnType == Sdf_en_RegularUacTransaction))

#endif
	{
        /*Klocwork Warning Fix*/
        if (Sdf_co_null != pOverlapTransInfo)
        {
		    if (sip_setSeqNumInCseqHdr(pHeader, pOverlapTransInfo->dLocalCseq, \
			    (SipError *)&(pError->stkErrCode)) == SipFail)
            {
#ifdef SDF_ERROR			
                sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
                        (Sdf_ty_s8bit*)"sdf_fn_uaFormCseqHeader(): "
                        "Failed to set sequence number in CSeq header", pError);
#endif			
                /* klock tool error fix */
                sip_freeSipHeader(pHeader);
                (void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pHeader,pError);

                pError->errCode=Sdf_en_formRequestError;
                return Sdf_co_fail;
            }	
        }
	}
	/*
	 * For all other requests, use the dLocalCseq in the callObject
	 */
	else
	{
		if( dMethodType != Sdf_en_ack) 
		{
			if (SipFail == sip_setSeqNumInCseqHdr(pHeader, \
					pCallObj->pCommonInfo->dLocalCseq, \
					(SipError *)&(pError->stkErrCode)))
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
					(Sdf_ty_s8bit*)"sdf_fn_uaFormCseqHeader(): "
					"Failed to set sequence number in CSeq header", pError);
#endif			
                /* klock tool error fix */
				sip_freeSipHeader(pHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pHeader,pError);

				pError->errCode=Sdf_en_formRequestError;
				return Sdf_co_fail;
			}	
		}
		/* While forming the ACK Take the Cseq from the response message */
		else
		{
			SipHeader 			pMsgHeader;
			Sdf_ty_u32bit 		dSeqNum=0;

			if (sip_getHeader( pCallObj->pUacTransaction->pSipMsg, \
					SipHdrTypeCseq, &pMsgHeader, \
					(SipError *) &(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode ;
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaFormCseqHeader( ): "
					"Failed to get  CSeq header from Sip Message",pError);	
#endif		
/* klock tool error fix */
				sip_freeSipHeader(pHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pHeader,pError);

				return Sdf_co_fail;
			}
			if (sip_getSeqNumFromCseqHdr(&pMsgHeader, &dSeqNum,(SipError *) \
				&(pError->stkErrCode)) == SipFail)
			{
				pError->errCode = pError->stkErrCode ;
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaFormCseqHeader( ): "
					"Failed to get Sequence number from CSeq header from" \
				   	"Sip Message",pError);	
#endif		
                /* klock tool error fix */
	sip_freeSipHeader(pHeader);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pHeader,pError);

				return Sdf_co_fail;
			}
			sip_freeSipHeader(&pMsgHeader);

			if (sip_setSeqNumInCseqHdr(pHeader, dSeqNum, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
					(Sdf_ty_s8bit*)"sdf_fn_uaFormCseqHeader(): "
					"Failed to set sequence number in CSeq header", pError);
#endif			
				pError->errCode=Sdf_en_formRequestError;
                /* klock tool error fix */
				sip_freeSipHeader(pHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pHeader,pError);

				return Sdf_co_fail;
			}
		}
	}

	pTmpMethod = Sdf_mc_strdupCallHandler(pMethod);

	if (sip_setMethodInCseqHdr(pHeader, pTmpMethod, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormCseqHeader(): "
			"Failed to set method in Cseq header", pError);
#endif		
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}	
	if ( SipFail == sip_setHeader(pSipMessage, pHeader, \
			(SipError *) &(pError->stkErrCode)))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormCseqHeader(): "
			"Failed to set CSeq header in SIP Message", pError);
#endif		
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}	
	/* Freeing the Local Reference */
	sip_freeSipHeader(pHeader);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pHeader,pError);
	return Sdf_co_success;
}

#ifdef SIP_RPR
/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaFormRackHeader
 **
 ** DESCRIPTION:	This function is used to Form the Rack Header
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormRackHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pCallObj,
	 Sdf_st_overlapTransInfo	*pOverlapTransInfo,
	 SipMessage			*pSipMessage,
	 Sdf_st_error       *pError)
#else
	(pCallObj, pOverlapTransInfo, pSipMessage, pError)
	 Sdf_st_callObject  *pCallObj;
	 Sdf_st_overlapTransInfo	*pOverlapTransInfo;
	 SipMessage			*pSipMessage;
	 Sdf_st_error       *pError;
#endif
{
	SipHeader *pRAckHeader;
	Sdf_ty_s8bit	*pTmpMethod = Sdf_co_null;

    /*Klocwork warning removal*/
	if(Sdf_co_null == pOverlapTransInfo)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormRackHeader( ): "
			"Invalid Param passed to this function",pError);
#endif	 
        pError->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }

	if (sip_initSipHeader(&pRAckHeader, SipHdrTypeRAck, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRackHeader( ): "
			"Failed to initialize Rack header", pError);
#endif			
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}

	if (sip_rpr_setRespNumInRAckHdr(pRAckHeader, pOverlapTransInfo->dRseq,\
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipHeader(pRAckHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&pRAckHeader, \
				pError);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
		(Sdf_ty_s8bit*)"sdf_fn_uaFormRackHeader( ): \
		Failed to set response number in Rack header", pError);
#endif			
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}


	if (sip_rpr_setCseqNumInRAckHdr(pRAckHeader, \
			pCallObj->pCommonInfo->dLocalCseq, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipHeader(pRAckHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
			&pRAckHeader,pError);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRackHeader( ): "
			"Failed to set Cseq number in Rack header", pError);
#endif			
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}

	pTmpMethod = Sdf_mc_strdupCallHandler(pCallObj->pCommonInfo->pRPRMethod);

	if (sip_rpr_setMethodInRAckHdr(pRAckHeader, pTmpMethod, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipHeader(pRAckHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
			&pRAckHeader,pError);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRackHeader( ): "
			"Failed to set Method in Rack header", pError);
#endif			
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}

	if (sip_setHeader(pSipMessage, pRAckHeader, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipHeader(pRAckHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
			&pRAckHeader,pError);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRackHeader( ): "
			"Failed to set Rack header in SipMessage", pError);
#endif			
		pError->errCode=Sdf_en_formRequestError;
		return Sdf_co_fail;
	}

	/* Freeing the local reference */
	sip_freeSipHeader(pRAckHeader);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pRAckHeader,pError);

	return Sdf_co_success;
}
#endif

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaFormRequireHdr
 **
 ** DESCRIPTION:	This function is used to Form the Require Header
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormRequireHdr
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pCallObj,
	 SipMessage			*pSipMessage,
	 Sdf_st_error       *pError)
#else
	(pCallObj, pSipMessage, pError)
	 Sdf_st_callObject  *pCallObj;
	 SipMessage			*pSipMessage;
	 Sdf_st_error       *pError;
#endif
{
	Sdf_ty_slist		slReqHeaders;
	
	Sdf_ty_u32bit		dSize = 0;
	Sdf_ty_u32bit		dIndex = 0;
	Sdf_st_headerList 	*pReqHeaderList;
	Sdf_ty_u32bit		dReqSize = 0;

	if(Sdf_co_fail == sdf_ivk_uaGetHeaderListByCorrelation( \
		&(pCallObj->slPersistentHeaders), (Sdf_ty_s8bit *)"uat-Require",
		&slReqHeaders, pError))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
			(Sdf_ty_s8bit*)"sdf_fn_uaFormRequireHdr(): "
			"Failed to get Require Header List with correlation", pError);
#endif		
		return Sdf_co_fail;
	}
	dSize = 0;

	(void)sdf_listSizeOf(&slReqHeaders, &dSize, pError);

	if( dSize != 0)
	{
		if(Sdf_co_fail == sdf_listGetAt(&slReqHeaders, 0, \
			(Sdf_ty_pvoid *)&pReqHeaderList, pError))
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors, pError->errCode,\
				(Sdf_ty_s8bit*)"sdf_fn_uaFormRequireHdr(): "
				"Failed to get Require Header List", pError);
#endif		
			(void)sdf_listDeleteAll(&slReqHeaders, pError);
			return Sdf_co_fail;
		}

		(void)sdf_listSizeOf(&(pReqHeaderList->slHeaders), &dReqSize, pError);
		
		for( dIndex=0; dIndex < dReqSize; dIndex++)
		{
			SipHeader *pReqHeader = Sdf_co_null;

			(void)sdf_listGetAt(&(pReqHeaderList->slHeaders), dIndex, \
				(Sdf_ty_pvoid *)&pReqHeader, pError);
			
			if(SipFail == sip_insertHeaderAtIndex(pSipMessage, pReqHeader, \
					dIndex, (SipError *)&(pError->stkErrCode)))
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors, pError->stkErrCode,\
					(Sdf_ty_s8bit*)"sdf_fn_uaFormRequireHdr(): "
					"Failed to Insert the Require Header", pError);
#endif		
				(void)sdf_listDeleteAll(&slReqHeaders, pError);
				return Sdf_co_fail;
			}
		}
	}
	(void)sdf_listDeleteAll(&slReqHeaders, pError);
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaUpdateContactInRouteSet
 **
 ** DESCRIPTION:	This function is used to Update the Contact Address
 **					present in the Route Set. Note: Last entry of the 
 **					Route Set contains the Contact Address
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateContactInRouteSet
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pCallObj,
	 SipMessage			*pSipMessage,
	 Sdf_st_error       *pErr)
#else
	(pCallObj, pSipMessage, pErr)
	 Sdf_st_callObject  *pCallObj;
	 SipMessage			*pSipMessage;
	 Sdf_st_error       *pErr;
#endif
{
	Sdf_st_headerList *pHeaderList = Sdf_co_null;
	Sdf_ty_slist	  slMatchedHeaders;
	Sdf_ty_u32bit	  dSize = 0;
	Sdf_ty_u32bit	  dCount = 0;

	/*w534*/ (void)sip_getHeaderCount(pSipMessage, SipHdrTypeContactAny, &dCount, \
		(SipError *)&(pErr->stkErrCode));

	if (dCount > 0)
	{
		SipHeader	*pRouteHeader;
		SipAddrSpec *pAddrSpec;
		Sdf_ty_s8bit *pDispName;
		SipHeader 		  dTempHeader;

		if (sdf_ivk_uaGetHeaderListByCorrelation( \
			&(pCallObj->slPersistentHeaders),(Sdf_ty_s8bit *)"uat-routeset", \
			&slMatchedHeaders, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactInRouteSet( ): "
				"Failed to get Routeset from the call object.",pErr);
#endif		
			return Sdf_co_fail;
		}
		if (sdf_listGetAt(&slMatchedHeaders, (Sdf_ty_u32bit)0, \
				(Sdf_ty_pvoid *)&pHeaderList, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactInRouteSet( ): "
				"Routeset does not exist in the call object.",pErr);
#endif		
			(void)sdf_listDeleteAll(&slMatchedHeaders, pErr);
			return Sdf_co_fail;
		}

		if (sip_getHeaderAtIndex(pSipMessage, SipHdrTypeContactAny, \
			&dTempHeader, 0,(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactInRouteSet( ): "
				"Error getting Contact Hdr",pErr);
#endif			
			(void)sdf_listDeleteAll(&slMatchedHeaders, pErr);
			return Sdf_co_fail;
		}

		(void)sdf_listSizeOf(&(pHeaderList->slHeaders), &dSize, pErr);

		if (sip_getAddrSpecFromContactHdr(&dTempHeader, &pAddrSpec, \
			(SipError *) &(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactInRouteSet(): "
				"Error getting AddrSpec from Contact Hdr",pErr);
#endif					
			(void)sdf_listDeleteAll(&slMatchedHeaders, pErr);
			return Sdf_co_fail;
		}
		if (sip_initSipHeader(&pRouteHeader, SipHdrTypeRoute, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactInRouteSet(): "
				"Error initing Route Hdr",pErr);
#endif					
			(void)sdf_listDeleteAll(&slMatchedHeaders, pErr);
			return Sdf_co_fail;
		}
		if (sip_setAddrSpecInRouteHdr(pRouteHeader,pAddrSpec, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateContactInRouteSet(): "
				"Error setting AddrSpec in Route Hdr",pErr);
#endif		
/* klock tool error fix */
	sip_freeSipHeader(pRouteHeader);
	(void)sdf_memfree(0,(Sdf_ty_pvoid *)&pRouteHeader,pErr);

			(void)sdf_listDeleteAll(&slMatchedHeaders, pErr);
			return Sdf_co_fail;
		}
		sip_freeSipAddrSpec(pAddrSpec);
		
		/*w534*/ (void)sip_getDispNameFromContactHdr(&dTempHeader, 
			&pDispName,(SipError *)&(pErr->stkErrCode));
		 
		/* 
		 * Replace the last Route header with the Contact header from 
		 * SipMessage
		 */
		if (sdf_listSetAt(&(pHeaderList->slHeaders), dSize-1, \
				(Sdf_ty_pvoid)pRouteHeader, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR						
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaUpdateRoutePathInCallObj( ): "
				"Error setting Route Header in Header List",pErr);
#endif						
			(void)sdf_listDeleteAll(&slMatchedHeaders, pErr);
			return Sdf_co_fail;
		}
		sip_freeSipHeader(&dTempHeader);
		(void)sdf_listDeleteAll(&slMatchedHeaders, pErr);
	} /* End of "if dCount (of Contact headers) greater than zero" */

	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaHandleContactParams
 **
 ** DESCRIPTION:	This function interprets the Contact Params received
 **					in redirect Response and sets the headers properly into
 **					the sip message.
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleContactParams
#ifdef ANSI_PROTO
	(SipMessage			*pSipMessage,
	 SipHeader 			*pContact,
	 Sdf_st_error       *pError)
#else
	(pSipMessage, pContact, pError)
	 SipMessage			*pSipMessage;
	 SipHeader 			*pContact;
	 Sdf_st_error       *pError;
#endif
{
	SipAddrSpec *pContactAddrSpec = Sdf_co_null;
	SipUrl		*pSipUrl=Sdf_co_null;

	
	if (sip_getAddrSpecFromContactHdr(pContact, &pContactAddrSpec, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaHandleContactParams( ): "
			"Failed to get addrspec from contact hdr", pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	/*w534*/ (void)sdf_ivk_uaGetUrlFromAddrSpec(pContactAddrSpec,&pSipUrl,pError);

	if( pSipUrl != Sdf_co_null )
	{
		Sdf_ty_s8bit	*pContHeaders = Sdf_co_null;
		Sdf_ty_s8bit	*pContHdr = Sdf_co_null;
		Sdf_ty_s8bit 	*pTempContactHdr = Sdf_co_null;

		/*w534*/ (void)sip_getHeaderFromUrl(pSipUrl, &pContHeaders, \
			(SipError *)&(pError->stkErrCode));

		if(pContHeaders != Sdf_co_null)
		{
			do{
				Sdf_ty_s8bit 	pActualContact[Sdf_co_mediumBufferSize]= {'\0',};
				en_HeaderType 	dHdrType;
				Sdf_ty_u32bit 	dSize = 0;
				Sdf_ty_u32bit 	dIndex = 0;
                Sdf_ty_s8bit 	pTempConact[Sdf_co_mediumBufferSize] = {'\0',};
				Sdf_ty_s8bit 	*pTempHolder=Sdf_co_null;
				Sdf_ty_s8bit	*pHeaderName=Sdf_co_null;

				pContHdr = Sdf_mc_strtokr(pContHeaders,"&", \
					   				&pTempContactHdr);
				if( pContHdr == Sdf_co_null )
					break;
				Sdf_mc_strcpy(pTempConact, pContHdr);

				if(Sdf_mc_strstr(pTempConact,"=")!=Sdf_co_null)
				{
					pHeaderName = \
							Sdf_mc_strtokr(pTempConact,"=",&pTempHolder);
					Sdf_mc_strncpy(pActualContact, pHeaderName, Sdf_co_mediumBufferSize);

					if(Sdf_co_mediumBufferSize > 
							(Sdf_mc_strlen(pActualContact) + 1))
					     Sdf_mc_strcat(pActualContact, ":");

					if(Sdf_co_mediumBufferSize > 
							(Sdf_mc_strlen(pActualContact) + Sdf_mc_strlen(pTempHolder)))
					     Sdf_mc_strcat(pActualContact, pTempHolder);
				}
				else
				{
					pContHeaders = pTempContactHdr;
					continue;
				}
				
				/* Remove escape char */
				for(dIndex=0; dIndex<Sdf_mc_strlen(pActualContact);dIndex++)
				{
					if(pActualContact[dIndex]=='%')
					{
						(void)sdf_fn_uaConverHexToChar(pActualContact,pError);
					}
				}
				if(sip_getTypeFromString(pActualContact,&dHdrType,\
						(SipError *)&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError, \
						(Sdf_ty_s8bit*)"sdf_fn_uaHandleContactParams():"
						"Failed to get the Type of the Header", pError);
#endif			
					pError->errCode=Sdf_en_headerManipulationError;
					sip_freeSipUrl(pSipUrl);
					sip_freeSipAddrSpec(pContactAddrSpec);
					return Sdf_co_fail;
				}
				if((dHdrType==SipHdrTypeFrom) || (dHdrType==SipHdrTypeTo) || \
					(dHdrType==SipHdrTypeVia) || (dHdrType==SipHdrTypeCallId))
				{
					pContHeaders = pTempContactHdr;
					continue;
				}
				/*w534*/ (void)sip_getHeaderCount(pSipMessage, dHdrType, \
					&dSize, (SipError*)&(pError->stkErrCode));
				
				if(dSize == 0)
				{
					if (sip_setHeaderFromStringAtIndex( pSipMessage, \
						dHdrType, pActualContact, 0, \
						(SipError *)&(pError->stkErrCode)) == SipFail)
					{
						if(pError->stkErrCode != Sdf_en_invalidIndexError)
						{
#ifdef SDF_ERROR			
							sdf_fn_setError(Sdf_en_majorErrors,\
								Sdf_en_headerManipulationError, \
								(Sdf_ty_s8bit*)"sdf_fn_uaHandleContactParams():"
								"Failed to set the content type header ",\
								pError);
#endif			
							pError->errCode=Sdf_en_headerManipulationError;
							sip_freeSipUrl(pSipUrl);
							sip_freeSipAddrSpec(pContactAddrSpec);
							return Sdf_co_fail;
						}

						if (sip_insertHeaderFromStringAtIndex( \
							pSipMessage, dHdrType, pActualContact, 0, \
							(SipError *)&(pError->stkErrCode)) == SipFail)
						{
#ifdef SDF_ERROR			
							sdf_fn_setError(Sdf_en_majorErrors, \
								Sdf_en_headerManipulationError, \
								(Sdf_ty_s8bit*)"sdf_fn_uaHandleContactParams:"
								"Failed to set the content type header",\
								pError);
#endif			
							pError->errCode=Sdf_en_headerManipulationError;
							sip_freeSipUrl(pSipUrl);
							sip_freeSipAddrSpec(pContactAddrSpec);
							return Sdf_co_fail;
						}
					}
				}
				else if (dSize == 1)
				{
					if (sip_insertHeaderFromStringAtIndex(pSipMessage,  \
						dHdrType, pActualContact, dSize, \
						(SipError *)&(pError->stkErrCode)) == SipFail)
					{
						if(pError->stkErrCode != Sdf_en_invalidIndexError)
						{
#ifdef SDF_ERROR			
							sdf_fn_setError(Sdf_en_majorErrors, \
								 Sdf_en_headerManipulationError, \
								(Sdf_ty_s8bit*)"sdf_fn_uaHandleContactParams():"
								"Failed to set the content type header ",\
								pError);
#endif			
							pError->errCode=Sdf_en_headerManipulationError;
							sip_freeSipUrl(pSipUrl);
							sip_freeSipAddrSpec(pContactAddrSpec);
							return Sdf_co_fail;
						}
						if (sip_setHeaderFromStringAtIndex(pSipMessage,  \
							dHdrType, pActualContact, 0, \
							(SipError *)&(pError->stkErrCode)) == SipFail)
						{
#ifdef SDF_ERROR			
							sdf_fn_setError(Sdf_en_majorErrors, \
								Sdf_en_headerManipulationError, \
								(Sdf_ty_s8bit*)"sdf_fn_uaHandleContactParams:"
							"Failed to set the content type header ",\
							pError);
#endif			
							pError->errCode=Sdf_en_headerManipulationError;
							sip_freeSipUrl(pSipUrl);
							sip_freeSipAddrSpec(pContactAddrSpec);
							return Sdf_co_fail;
						}
					}
				}
				else
				{
					if (sip_insertHeaderFromStringAtIndex( pSipMessage, \
						dHdrType, pActualContact, dSize, \
						(SipError *)&(pError->stkErrCode)) == SipFail)
					{
#ifdef SDF_ERROR			
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_headerManipulationError, \
							(Sdf_ty_s8bit*)"sdf_fn_uaHandleContactParams():"
							"Failed to set the content type header in SipMsg",\
							pError);
#endif			
						sip_freeSipUrl(pSipUrl);
						sip_freeSipAddrSpec(pContactAddrSpec);
						pError->errCode=Sdf_en_headerManipulationError;
						return Sdf_co_fail;
					}
				}
				pContHeaders = pTempContactHdr;

			}while(pContHdr != Sdf_co_null);
		}
		sip_freeSipUrl(pSipUrl);
	}
	sip_freeSipAddrSpec(pContactAddrSpec);

	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaInsertRequireHdr
 **
 ** DESCRIPTION:	This function Extracts the Require header and inserts
 **					it to the Sipmessage.
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaInsertRequireHdr
#ifdef ANSI_PROTO
	(SipMessage			*pSipMessage,
	 Sdf_st_initData	*pInitData,
	 Sdf_st_error       *pError)
#else
	(pSipMessage, pInitData, pError)
	 SipMessage			*pSipMessage;
	 Sdf_st_initData	*pInitData;
	 Sdf_st_error       *pError;
#endif
{
	Sdf_st_listIterator dListIterator;
	Sdf_st_configExtraHeaders	*pTemp = Sdf_co_null;
	Sdf_ty_bool			dIsRequirePresent = Sdf_co_false;
	Sdf_ty_u32bit		dRequireHdrSize = 0;
	SipHeader			*pReqHeader = Sdf_co_null;
	Sdf_ty_s8bit		*pOptions = Sdf_co_null;
	Sdf_ty_u32bit		dIndex =0;
	Sdf_st_error		dLocalErr;

	if (pInitData == Sdf_co_null)
	{
		return Sdf_co_success;
	}

	(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList),\
				&dListIterator, pError);
	/* Iterate through profile & check if Require header is present or not */
	while(dListIterator.pCurrentElement != Sdf_co_null)
	{
		pTemp = (Sdf_st_configExtraHeaders *) \
				(dListIterator.pCurrentElement->pData);

		if(Sdf_mc_strcasecmp((Sdf_ty_s8bit *)"REQUIRE",pTemp->pParam->pName)==0)
		{
			/*
			 * Check if "addalways" xml tag is enabled or not.
			 * If it is disabled, add the default set supported by UATK
			 */
			if(Sdf_mc_strcasecmp((Sdf_ty_s8bit *)"no", \
						(Sdf_ty_s8bit *)pTemp->pData) == 0)
			{
				return Sdf_co_success;
			}
			dIsRequirePresent = Sdf_co_true;
			break;
		}
		(void)sdf_listNext(&dListIterator, pError);
	}

	if( dIsRequirePresent == Sdf_co_false )
	{
		return Sdf_co_success;
	}

	(void)sip_listSizeOf(&(pTemp->pParam->slValue), &dRequireHdrSize, \
				(SipError*)&(pError->stkErrCode));

	for(dIndex=0; dIndex<dRequireHdrSize; dIndex++)
	{
		if(SipFail ==sip_listGetAt(&(pTemp->pParam->slValue), dIndex, \
				(Sdf_ty_pvoid *)&pOptions, (SipError*)&(pError->stkErrCode)))
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaInsertRequireHdr:"
				"Failed to get the Options from header ", pError);
#endif			
			return Sdf_co_fail;
		}

		if(SipFail == sip_initSipHeader(&pReqHeader, SipHdrTypeRequire, \
					(SipError*)&(pError->stkErrCode)))
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaInsertRequireHdr:"
				"Failed to init Sip Header", pError);
#endif			
			pError->errCode = Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}

		if( SipFail == sip_setTokenInRequireHdr(pReqHeader, \
			Sdf_mc_strdupCallHandler(pOptions), \
			(SipError *)&(pError->stkErrCode)))
		{
			sip_freeSipHeader(pReqHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pReqHeader, \
					&dLocalErr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaInsertRequireHdr:"
				"Failed to Set the Token into Sip Require Header", pError);
#endif			
			pError->errCode = Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}
		
		if(SipFail == sip_insertHeaderAtIndex(pSipMessage, pReqHeader, dIndex, \
			(SipError *)&(pError->stkErrCode)))
		{
			sip_freeSipHeader(pReqHeader);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pReqHeader, \
					&dLocalErr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
				(Sdf_ty_s8bit*)"sdf_fn_uaInsertRequireHdr:"
				"Failed to insert header into Sip Message", pError);
#endif			
			pError->errCode = Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}

		sip_freeSipHeader(pReqHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pReqHeader, pError);
	}
	return Sdf_co_success;
}

Sdf_ty_retVal sdf_fn_uaAddSupportedHdrToSipMsg
#ifdef ANSI_PROTO
	(SipMessage			*pMesg,
	 Sdf_st_initData	*pInitData,
	 Sdf_st_error       *pErr)
#else
	(pMesg, pInitData, pErr)
	 SipMessage			*pMesg;
	 Sdf_st_initData	*pInitData;
	 Sdf_st_error       *pErr;
#endif

{
	Sdf_st_listIterator dListIterator;
	Sdf_ty_u32bit size,j;
	
#ifdef SDF_PARAMVALIDATION
	if ((pMesg == Sdf_co_null) || (pInitData == Sdf_co_null))
		return Sdf_co_fail;
#endif
	(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList), \
		&dListIterator, pErr);
	
	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_configExtraHeaders *pTemp;
		en_HeaderType pType;
		Sdf_ty_s8bit dTempType[Sdf_co_characterBufferSize]="";

		pTemp = (Sdf_st_configExtraHeaders*)\
			(dListIterator.pCurrentElement->pData);

		Sdf_mc_strncpy(dTempType,pTemp->pParam->pName,Sdf_co_characterBufferSize);

		if(Sdf_co_characterBufferSize > (strlen(dTempType) + 1))
		       Sdf_mc_strcat(dTempType,":");

		/*
		 * Invoke stack API to convert the Header name into the stacks
		 * en_HeaderType enumeration
		 */
		if(sip_getTypeFromString(dTempType,&pType, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddSupportedHdrToSipMsg():  "
				"Failed to get header type from string ",pErr);
#endif
			return Sdf_co_fail;
		}
		if (pType != SipHdrTypeSupported)
		{
			(void)sdf_listNext(&dListIterator, pErr);
			continue;
		}
		if(Sdf_mc_strcasecmp("yes",(Sdf_ty_s8bit *)pTemp->pData) \
				!= 0)
		{
			(void)sdf_listNext(&dListIterator, pErr);
			continue;
		}

		(void)sip_listSizeOf(&(pTemp->pParam->slValue), &size, \
 			  (SipError*)&(pErr->stkErrCode));

		/*
		 * Iterate through the list of headers in the
		 * ExtraHeaders and set it into the HeaderList structure
		 */
		for (j = 0; j < size; j++)
		{
			Sdf_ty_s8bit *pTempVal, *pOptions;
			SipHeader *pHdr;

			if (sip_listGetAt(&(pTemp->pParam->slValue),  \
				(Sdf_ty_u32bit) j,(Sdf_ty_pvoid *)&pTempVal,  \
				(SipError*)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,
					Sdf_en_extraHeaderAccessError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaAddSupportedHdrToSipMsg(): "
					"Failed to get value at index from header list",pErr);
#endif
				pErr->errCode=Sdf_en_extraHeaderAccessError;
				return Sdf_co_fail;
			}
			if (sip_initSipHeader(&pHdr, pType, \
				(SipError*)&(pErr->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaAddSupportedHdrToSipMsg(): "
					"Failed to Initialize SipHeader",pErr);
#endif
				pErr->errCode=Sdf_en_noMemoryError;
				return Sdf_co_fail;
			}
			
			/*
			 * Set the options in supported header
			 */
			Sdf_mc_strdup(pOptions,pTempVal);
			if(sip_setOptionInSupportedHdr(pHdr,pOptions, \
						(SipError *)&(pErr->stkErrCode))==SipFail)
			{
				sip_freeSipHeader(pHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,\
							(Sdf_ty_pvoid*)&pHdr,pErr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,\
							(Sdf_ty_pvoid*)&pOptions,pErr);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaAddSupportedHdrToSipMsg(): "
					"Failed to set options in Supported Header",\
					pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}

			if(sip_insertHeaderAtIndex(pMesg, pHdr, j, \
						(SipError *)&(pErr->stkErrCode))==SipFail)
			{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
					(Sdf_ty_s8bit*) "sdf_fn_uaAddSupportedHdrToSipMsg(): "
					"Could Not insert Header into SipMessage",pErr);
#endif
				sip_freeSipHeader(pHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,\
							(Sdf_ty_pvoid*)&pHdr,pErr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,\
							(Sdf_ty_pvoid*)&pOptions,pErr);
				return Sdf_co_fail;
			}
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,\
					(Sdf_ty_pvoid*)&pHdr,pErr);
		} /* 
		   * End of for loop making a header list structure. Use the
		   * header list formed in the loop now
		   */
		(void)sdf_listNext(&dListIterator, pErr);
	}
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaGetPointerTo
 **
 ** DESCRIPTION:	This function returns a pointer to the SIP header which 
 **					starts with the string present in 'pSrchStr'.
 **					If "*" is passed as 'pSrchStr', then it returns the 
 **					next line.
 **					This function identifies next header by the presence of
 **					"\r\n" at the end of every line. If the SIP message
 **					is just separated by "\n", it is not considered as next header.
 *****************************************************************************/
Sdf_ty_s8bit *sdf_fn_uaGetPointerTo(Sdf_ty_s8bit *pOrgStr, Sdf_ty_s8bit \
		*pSrchStr, Sdf_st_error *pError)
{
	Sdf_ty_s8bit *pStrDup, *pStr, *pNew;
	Sdf_ty_u32bit len;

#ifdef SDF_TRACE
	Sdf_ty_s8bit trace[Sdf_co_traceBufferSize];
	
	sdf_fn_trace(Sdf_en_detailedTraces, \
			Sdf_en_mesgTraces, (Sdf_ty_s8bit *)"Entered "
			"sdf_fn_uaGetPointerTo", 0, pError);
	
	(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
		"Header to be searched is:%s", pSrchStr);
	sdf_fn_trace(Sdf_en_detailedTraces, \
			Sdf_en_mesgTraces, (Sdf_ty_s8bit *)trace , 0, pError);
#endif	
	if (pOrgStr == Sdf_co_null)
		return Sdf_co_null;
	
	/*
	 * Make a copy of the original string, so that it is not altered
	 * because of strstr
	 */
	Sdf_mc_strdup(pStrDup, pOrgStr);
	if (pStrDup == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGetPointerTo(): "
			"Failed to allocate memory", pError);
#endif			
		pError->errCode = Sdf_en_memoryError;
		return Sdf_co_null;
	}
	
	len = Sdf_mc_strlen(pSrchStr);
	pStr = pStrDup;
	/*
	 * Start searching for `pSrchStr` string in `pStr`.
	 * Break from this loop, either
	 * 		If the search string was found
	 * 				OR
	 * 		if `pStr` is exhausted
	 */
	while (pStr != Sdf_co_null)
	{
		/* Break, if search string is found */
		if (Sdf_mc_strncasecmp(pStr, pSrchStr, len) == 0)
			break;
		
		pNew = Sdf_mc_strstr(pStr, "\r\n");
		/* Break if source string is exhausted */
		if (pNew == Sdf_co_null)
			pStr = Sdf_co_null;
		else /* Else: skip the `\r\n` at the end of each line */
			pStr = pNew+2;
		
		/*
		 * If "*" is passed as the search-string, return the next line.
		 * Now, pStr points to next line.
		 */
		if (Sdf_mc_strcmp(pSrchStr, "*") == 0)
			break;
	}
	if (pStr == Sdf_co_null)
	{	
#ifdef SDF_TRACE
		sdf_fn_trace(Sdf_en_detailedTraces, \
			Sdf_en_mesgTraces, (Sdf_ty_s8bit *)"String not found!!!", 0, pError);
#endif
	}	
	else
	{
		/*
		 * As of now, `pStr` is pointing to the DUPED string `pStrDup`.
		 * Make this point to `pOrgStr`, as `pStrDup` is going to be free'd down
		 * the line
		 */
		pStr = &pOrgStr[pStr-pStrDup];
#ifdef SDF_TRACE
		sdf_fn_trace(Sdf_en_detailedTraces, \
			Sdf_en_mesgTraces, (Sdf_ty_s8bit *)"String found!!!", 0, pError);
#endif		
	}
	(void)sdf_memfree (Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pStrDup, pError);
	
#ifdef SDF_TRACE
	sdf_fn_trace(Sdf_en_detailedTraces, \
			Sdf_en_mesgTraces, (Sdf_ty_s8bit *)"Exiting "
			"sdf_fn_uaGetPointerTo", 0, pError);
#endif	
	return pStr;
}

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaGetCurrentLineLength
 **
 ** DESCRIPTION:	This function splits the given string into "Name" "Value"
 **					pair (Separator used is ":" (colon-for SIP headers) or 
 **					"=" (equal to-for SDP headers). This function returns
 **					the length of string contained in "Value"
 *****************************************************************************/
Sdf_ty_u32bit sdf_fn_uaGetCurrentLineLength(Sdf_ty_s8bit *pStr, \
		Sdf_st_error *pError)
{
	Sdf_ty_u32bit dLength=0, dLenCurLine=0, dpStrLen;
	Sdf_ty_s8bit *pStrDup=Sdf_co_null, *pTempStr;
#ifdef SDF_TRACE
	Sdf_ty_s8bit trace[Sdf_co_traceBufferSize];
	
	sdf_fn_trace(Sdf_en_detailedTraces, \
			Sdf_en_mesgTraces, (Sdf_ty_s8bit *)"Entered "
			"sdf_fn_uaGetCurrentLineLength", 0, pError);
#endif	
	if (pStr == Sdf_co_null)
		return 0;
	
	dpStrLen = Sdf_mc_strlen(pStr);
	if (dpStrLen == 0)
		dLenCurLine = 0;
	else
	{
		/*- Get the length of Current Line -*/
		/* CERT: UMR fixed in test case 625 */
		for (dLenCurLine=1;((pStr[dLenCurLine-1]!='\r') && 
					(pStr[dLenCurLine]!='\n'));dLenCurLine++)
		{
			/* 
			 * Break, if the "length of current line" increases above the "length
			 * of the whole string". This can happen, if the line is not
			 * terminated with "\r\n"
			 */
			if (dLenCurLine >= dpStrLen)
				break;
		}
		if (dLenCurLine < dpStrLen)
 			dLenCurLine++;
	}
	/*
	 * Make a copy of the original string, so that it is not altered
	 * because of strstr
	 */
	pStrDup = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_callHandlerMemId, \
				dLenCurLine+1, pError);
	if (pStrDup == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGetCurrentLineLength(): "
			"Failed to allocate memory", pError);
#endif			
		pError->errCode = Sdf_en_memoryError;
		return 0;
	}
	Sdf_mc_strncpy(pStrDup, pStr, dLenCurLine);
	pStrDup[dLenCurLine] = '\0';

#ifdef SDF_TRACE
	sdf_fn_trace(Sdf_en_detailedTraces, \
			Sdf_en_mesgTraces, "Header under inspection:", 0, pError);
	sdf_fn_trace(Sdf_en_detailedTraces, \
			Sdf_en_mesgTraces, pStrDup , 0, pError);
#endif	
	/*
	 * We are interested in the "Value" of "Name: Value" pair. Extract it.
	 * SIP header is a "Name: Value" pair (Note: Colon)
	 * SDP header is a "Name= Value" pair (Note: Equal)
	 */
	pTempStr = Sdf_mc_strpbrk(pStrDup, ":=");
	if (pTempStr == Sdf_co_null)
	{
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pStrDup, pError);
		return 0;
	}
	dLength = Sdf_mc_strlen(pTempStr) - 1;
	/* -1 to skip the separator (: or =) that separates "Name" "Value" */
	
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pStrDup, pError);
	
#ifdef SDF_TRACE
	(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
		"Length of Current header:%d", dLength);
	sdf_fn_trace(Sdf_en_detailedTraces, \
			Sdf_en_mesgTraces, (Sdf_ty_s8bit *)trace , 0, pError);
	sdf_fn_trace(Sdf_en_detailedTraces, \
			Sdf_en_mesgTraces, (Sdf_ty_s8bit *)"Exiting "
			"sdf_fn_uaGetCurrentLineLength", 0, pError);
#endif	
	return dLength;
}



/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUnMuteMediaSession
 **
 ** DESCRIPTION: This function UnMutes the SessionParam by
 **              either restoring the c= line to prev IP addr or by restoring 
 **              the direction attribute, depending on dType.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUnMuteMediaSession
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pSessionParams,
	 Sdf_ty_muteType     dType,
	 Sdf_st_error        *pErr)
#else
	( pSessionParams, dType, pErr )
	 Sdf_st_sessionParam *pSessionParams;
	 Sdf_ty_muteType     dType;
	 Sdf_st_error        *pErr;
#endif
{
	Sdf_st_mediaStream *pMediaStream = Sdf_co_null;
	Sdf_ty_u8bit        dMediaStreamIndex = 0;
	Sdf_ty_u8bit        dNumMediaStreams = 0;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "\
			"sdf_fn_uaUnMuteMediaSession");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_fn_uaUnMuteMediaSession( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if(pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaUnMuteMediaSession( ):  "
			"Invalid Session Param parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif  /* End of Param Validation. */
	
	/* 
	 * First lets do a session level unmute
	 */
	if (dType == Sdf_en_connectionMute)
	{
		if (pSessionParams->pPreviousConnection == Sdf_co_null)
		{
			/*
			 * This means a new c-line was added with address set to 0.0.0.0
			 * to mute the MediaStream. So delete the c-line from 
			 * SessionParam.
			 */
			sip_freeSdpConnection(pSessionParams->pConnection);
			pSessionParams->pConnection = Sdf_co_null;
		}
		else
		{
			sip_freeSdpConnection(pSessionParams->pConnection);
			pSessionParams->pConnection = \
						  pSessionParams->pPreviousConnection;
			pSessionParams->pPreviousConnection = Sdf_co_null;
		}
	}
	else
	{
		pSessionParams->dDirectionAttrib = \
							 pSessionParams->dPrevDirectionAttrib;
	}

	/*
	 * Now unmute the media level
	 */
	dNumMediaStreams = pSessionParams->dNumMediaStreams;

	for (dMediaStreamIndex = 0; dMediaStreamIndex < dNumMediaStreams;\
		   dMediaStreamIndex++)
	{
		if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
				&pMediaStream, dMediaStreamIndex, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_sessionParamAccessError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUnMuteMediaSession( ):  "
				"Failed to get MediaStream at index from SessionParam",\
				pErr);
#endif
			pErr->errCode=Sdf_en_sessionParamAccessError;
			return Sdf_co_fail;
		}

		if (sdf_fn_uaUnMuteMediaStream(pMediaStream, dType,\
						pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaUnMuteMediaSession( ):  "
				"Error Muting MediaStream in SDP",pErr);
#endif
			(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
			return Sdf_co_fail;
		}

		/* Free the local reference */
		(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
   }

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "\
			"sdf_fn_uaUnMuteMediaSession");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaMuteMediaSession
 **
 ** DESCRIPTION: This function mutes the MediaStreams present in the given 
 **              Session Param. 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaMuteMediaSession
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pSessionParams,
	 Sdf_ty_muteType     dType,
	 Sdf_st_error        *pErr)
#else
	( pSessionParams, dType, pErr )
	 Sdf_st_sessionParam *pSessionParams;
	 Sdf_ty_muteType     dType,
	 Sdf_st_error        *pErr;
#endif
{
	Sdf_st_error		dLocalErr;
	Sdf_st_mediaStream *pMediaStream = Sdf_co_null;
	Sdf_ty_u8bit       dIndex = 0;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Entering sdf_fn_uaMuteMediaSession");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_fn_uaMuteMediaSession( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if(pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaSession( ):  "
			"Invalid Session Param parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* Param Validation ends here. */

	/* First do a Session Level mute */
	if (dType == Sdf_en_connectionMute)
	{
		Sdf_ty_s8bit	*pTmpValue = Sdf_co_null;
		SdpConnection   *pCurrConnection = Sdf_co_null;

		/*
		 * First store this Connection Information so that UnMute call can 
		 * be handled properly.
		 */
		if (sdf_ivk_uaGetSessionConnection(pSessionParams,
					&pCurrConnection, pErr) == Sdf_co_success)
		{
			/*
			 * First store this Connection Information so that UnMute 
			 * call can be handled properly.
			 */
			if (sip_initSdpConnection(\
						&(pSessionParams->pPreviousConnection),\
						(SipError *)&(pErr->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaMuteMediaSession( ): Failed "
					"to allocate memory for the SDP Connection", pErr);
#endif		
				pErr->errCode=Sdf_en_memoryError;
				sip_freeSdpConnection(pCurrConnection);
				return Sdf_co_fail;
			}

			if (sdp_cloneSdpConnection(pSessionParams->pPreviousConnection,\
				pCurrConnection, (SipError *)&(pErr->stkErrCode)) \
					== SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaSession( ):  "
					"Error cloning SdpConnection into PrevSdpConnection "
					"in SDP",pErr);
#endif
				sip_freeSdpConnection(pSessionParams->pPreviousConnection);
				sip_freeSdpConnection(pCurrConnection);
				return Sdf_co_fail;
			}

			/* 
			 * Replace the current IP address in the connection 
			 * information with 0.0.0.0
			 */
			if (Sdf_co_null == (pTmpValue =\
					   Sdf_mc_strdupCallHandler("0.0.0.0")))
			{
#ifdef SDF_ERROR
				pErr->errCode = Sdf_en_noMemoryError;
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaSession( ): Error "\
					"in allocating memory for new c-line address",pErr);
#endif
				sip_freeSdpConnection(pSessionParams->pPreviousConnection);
				sip_freeSdpConnection(pCurrConnection);
				return Sdf_co_fail;
			}
			if (sdp_setAddrInConnection(pCurrConnection,\
					pTmpValue, (SipError *)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaSession( ):  "
					"Error setting Address to 0.0.0.0 in connection "
					"in SDP",pErr);
#endif
				sip_freeSdpConnection(pSessionParams->pPreviousConnection);
				sip_freeSdpConnection(pCurrConnection);
				(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pTmpValue),\
							&dLocalErr);
				return Sdf_co_fail;
			}

			/* Free the local reference */
			sip_freeSdpConnection(pCurrConnection);
		}
		/* 
		 * No session level c-line is present. Media level will be handled
		 * in the media level below
		 */
	}
	else /* direction - change it only if it is available*/
	{
		if(pSessionParams->dDirectionAttrib != Sdf_en_dirAttribNone)
		{
			if (dType == Sdf_en_holdSelfMedia)
			{
				pSessionParams->dPrevDirectionAttrib = \
									pSessionParams->dDirectionAttrib;
				if(pSessionParams->dDirectionAttrib == Sdf_en_recvonly)
				{
					pSessionParams->dDirectionAttrib = Sdf_en_inactive;
				}
				else
				{
					pSessionParams->dDirectionAttrib = Sdf_en_recvonly;
				}
			}
			else if (dType == Sdf_en_completeMute)
			{
				pSessionParams->dPrevDirectionAttrib = \
									pSessionParams->dDirectionAttrib;
				pSessionParams->dDirectionAttrib = 	Sdf_en_inactive;
			}
			else if (dType == Sdf_en_holdPeerMedia)
			{
				pSessionParams->dPrevDirectionAttrib = \
									pSessionParams->dDirectionAttrib;
				if(pSessionParams->dDirectionAttrib == 	Sdf_en_sendonly)
				{
					pSessionParams->dDirectionAttrib = 	Sdf_en_recvonly;
				}
				else
				{
					pSessionParams->dDirectionAttrib = 	Sdf_en_sendonly;
				}
			}
		}
	}

    /* Now do a Media level mute */
	for (dIndex = 0; dIndex < (pSessionParams->dNumMediaStreams); dIndex++)
    {

		if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
						&pMediaStream, dIndex, pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
		          	Sdf_en_sessionParamAccessError,\
   			       	(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaSession( ):  "
      			    "Failed to get MediaStream at index from SessionParam",\
				   	pErr);
#endif
			pErr->errCode=Sdf_en_sessionParamAccessError;
            return Sdf_co_fail;
        }
		if (sdf_fn_uaMuteMediaStream(pMediaStream, dType,\
							pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
	        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
    				(Sdf_ty_s8bit *)"sdf_fn_uaMuteMediaSession( ):  "
	   				"Error Muting MediaStream in SDP",pErr);
#endif
			(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
	       	return Sdf_co_fail;
         }
		/* Free the local reference */
		(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
    }

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "\
			"sdf_fn_uaMuteMediaSession");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


#ifdef SDF_REFER
/****************************************************************************
** FUNCTION: sdf_fn_uaConvertHexToChar
**
** DESCRIPTION: This function is used to convert a hex number (presented in
**		first 2 bytes of input argument) into decimal digit.
**
** PARAMETERS:
**		pHexInput(IN)	: Input hex value.
**
****************************************************************************/
Sdf_ty_s8bit sdf_fn_uaConvertHexToChar
#ifdef ANSI_PROTO 
(Sdf_ty_s8bit *pHexInput)
#else
(pHexInput)
Sdf_ty_s8bit *pHexInput;
#endif
{
  	Sdf_ty_s8bit dDigit;

    dDigit = (pHexInput[0] >= 'A' ? ((pHexInput[0] & 0xdf) - 'A')+10 \
        : (pHexInput[0] - '0'));
  	dDigit *= 16;
  	dDigit += (pHexInput[1] >= 'A' ? ((pHexInput[1] & 0xdf) - 'A')+10 \
        : (pHexInput[1] - '0'));
  	return(dDigit);
}
#endif

/****************************************************************************
** FUNCTION: sdf_fn_uaEscapeCharacters
**
** DESCRIPTION: This function is used to escape a certain set of characters
**		Some ASCII characters are converted to their HEX equivalents.
**
** PARAMETERS:
**		dInputStr(IN)	: Input string, whose special characters 
**							should be escaped.
**		pErr(OUT)		: Error structure in case the function fails
**
****************************************************************************/
Sdf_ty_s8bit* sdf_fn_uaEscapeCharacters
#ifdef ANSI_PROTO
(Sdf_ty_s8bit* dInputStr, Sdf_st_error* pErr)
#else
(dInputStr,pErr)
Sdf_ty_s8bit* dInputStr;
SipError* pErr;
#endif
{
	Sdf_ty_s8bit escapedCharSet[]={'/','?',':','@','&','=','+','$',',', \
		'<','>' ,'#','%','"','{','}','|','\\','^','[',']','`',' ',';', '~'};
	Sdf_ty_u32bit i=0,k=0,x=0;
	Sdf_ty_u32bit dStrLen;
	Sdf_ty_s8bit *pOutputStr = Sdf_co_null;

#ifdef SDF_LINT
    if(dInputStr == Sdf_co_null)
    {
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_null;
    }
#endif    
	/* Allocate more memory to the return variable, as the length is expected
	 * to increase once the characters are escaped */
	dStrLen = Sdf_mc_strlen(dInputStr);
	pOutputStr = (Sdf_ty_s8bit*)sdf_memget(0, ((dStrLen*3)+1), pErr);
	if(pOutputStr == Sdf_co_null)
		return Sdf_co_null;
	strcpy(pOutputStr,"");

	for(i=0,x=0; i<dStrLen; i++,x++)
	{
    	pOutputStr[x] = dInputStr[i];
		/* Check each character with the Escape characters */
		for(k=0; k<sizeof(escapedCharSet); k++)
		{
			if(dInputStr[i] == escapedCharSet[k])
			{
				(void)sdf_fn_uaSprintf(&pOutputStr[x], "%%%x", dInputStr[i]);
 				x=x+2;
				break;
			}
		}
	}
	pOutputStr[x] = '\0';
	return (pOutputStr);
}


#ifdef SDF_REFER
/****************************************************************************
** FUNCTION: sdf_fn_uaUnescapeCharacters
**
** DESCRIPTION: This function is used to un=escape a certain set of characters
**		i.e. the HEX numbers are converted to their ASCII equivalents.
**
** PARAMETERS:
**		pUrl(IN/OUT)	: String which has escaped characters. After 
**							conversion, this string is overwritten to
**							contain the correct (un-escaped) string.
**		pErr(OUT)		: Error structure in case the function fails
**
****************************************************************************/
Sdf_ty_s8bit* sdf_fn_uaUnescapeCharacters
#ifdef ANSI_PROTO 
(Sdf_ty_s8bit* pUrl, Sdf_st_error *pErr)
#else
(pUrl, pErr)
Sdf_ty_s8bit* pUrl;
Sdf_st_error *pErr;
#endif
{
  	Sdf_ty_u32bit x,y;
	(void)pErr;

   	for (x=0,y=0; pUrl[y]; ++x,++y) 
  	{
	  /* Check if the character is an Escape'd one, then convert it */
      if((pUrl[x] = pUrl[y]) == '%') 
      {
        pUrl[x] = sdf_fn_uaConvertHexToChar(&pUrl[y+1]);
        y+=2;
      }
  	}
  	pUrl[x] = '\0';
  	return(pUrl);
}

/*****************************************************************************
** FUNCTION: sdf_fn_uaValidateRefer
**
** DESCRIPTION: This function checks if "Refer-To" header is present in 
**				SipMessage or not.
**				If the header is present, dValidity is set to Sdf_en_valid
**				otherwise, dValidity is set to Sdf_en_invalid
**
** PARAMETERS:
**		pSipMsg(IN)			: SipMsg in which Refer-To header is to be checked
**		pValidity(OUT)		: Indicates whether REFER message is valid
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateRefer(SipMessage *pSipMsg, \
		Sdf_ty_messageValidity *pValidity, Sdf_st_error *pErr)
{
	SipHeader dSipHdr;
	
#ifdef SDF_PARAMVALIDATION
	if (Sdf_co_null == pErr)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaValidateRefer(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if ((Sdf_co_null == pSipMsg) || (Sdf_co_null == pValidity))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaValidateRefer(): "
			"pSipMsg/pValidity param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
	
	/* Initialize pValidity to Invalid. */
	*pValidity = Sdf_en_invalid;
	
	if (SipFail != sip_getHeader(pSipMsg, SipHdrTypeReferTo, &dSipHdr, \
				(SipError*)&pErr->stkErrCode))
	{
		sip_freeSipHeader(&dSipHdr);
		*pValidity = Sdf_en_valid;
		return Sdf_co_success;
	}
	return Sdf_co_success;
}

#endif /* SDF_REFER */

/******************************************************************************
** FUNCTION: sdf_fn_uaInitCallObjectIdGenerator
**
** DESCRIPTION: Initialize the call object id generator.
**
*******************************************************************************/
Sdf_ty_retVal  sdf_fn_uaInitCallObjectIdGenerator
#ifdef ANSI_PROTO	
	(Sdf_st_error *pError)
#else
	(pError)
	Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Entering sdf_fn_uaInitCallObjectIdGenerator");

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaInitMutex(&dGlbSdfCallObjectIdGeneratorMutex, Sdf_co_false);
#endif
	dGlbSdfNextCallObjectId = 1;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_fn_uaInitCallObjectIdGenerator");

	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/******************************************************************************
** FUNCTION: sdf_fn_uaGetUniqueCallObjectId
**
** DESCRIPTION: This is an internal function used by 
** 				sdf_ivk_uaInitCall to generate a unique call object	Id.
**
*******************************************************************************/
Sdf_ty_u32bit  sdf_fn_uaGetUniqueCallObjectId
#ifdef ANSI_PROTO	
	(void)
#else
	()
#endif
{
	Sdf_ty_u32bit	dAssignedCallObjectId;
#ifdef SDF_TRACE
	Sdf_ty_s8bit	dTraceStr[Sdf_co_traceBufferSize];
	Sdf_st_error	dError;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaGetUniqueCallObjectId");

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0, &dGlbSdfCallObjectIdGeneratorMutex, 0);
#endif

	dAssignedCallObjectId = dGlbSdfNextCallObjectId;

	dGlbSdfNextCallObjectId++;
	if (dGlbSdfNextCallObjectId == 0)
		dGlbSdfNextCallObjectId = 1;

#ifdef SDF_TRACE
	(void)sdf_fn_uaSnprintf(dTraceStr,Sdf_co_traceBufferSize,\
			"Allocated call object Id = 0x%0x", dAssignedCallObjectId);
	sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_haTraces, dTraceStr, 0, &dError);
#endif
	
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &dGlbSdfCallObjectIdGeneratorMutex);
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_fn_uaGetUniqueCallObjectId");

	return dAssignedCallObjectId;
}

/*#ifdef SDF_TLS*/

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetContactAddrType
 **
 ** DESCRIPTION: This function is used to determine the Address Type for
 **              constructing Contact headers. In case of forming request, 
 **              SipMessage will be the message containing the Request. In 
 **              case of response, SipMessage (taken from UAS txn or Overlap
 **              txn.) will be the incoming request for which the response 
 **              is being formed.
 **
 ******************************************************************************/
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/

Sdf_ty_retVal sdf_fn_uaGetContactAddrType
#ifdef ANSI_PROTO
	(SipMessage         *pSipMessage,
	 Sdf_st_callObject  *pCallObj,
	 Sdf_ty_addrType    *pContactAddrType,
	 en_SipMessageType  dReqRespType,
	 Sdf_st_error       *pErr)
#else
	( pSipMessage, pCallObj, pContactAddrType, dReqRespType, pErr )
	 SipMessage         *pSipMessage;
	 Sdf_st_callObject  *pCallObj;
	 Sdf_ty_addrType    *pContactAddrType;
	 en_SipMessageType  dReqRespType;
	 Sdf_st_error       *pErr;
#endif
{
	Sdf_ty_bool		dGotTlsTransport = Sdf_co_false;

#ifdef SDF_TLS
	Sdf_ty_u32bit	size = 0;
	SipHeader		dSipHeader;
	SipAddrSpec 	*pAddrSpec = Sdf_co_null;
	SipReqLine     *pRequest = Sdf_co_null;
#endif

	(void)pCallObj;
	(void)pSipMessage;
	(void)dReqRespType;
	(void)pErr;

#ifdef SDF_TLS
	if (dReqRespType == SipMessageRequest)
    {
		Sdf_ty_messageType 	dMsgType = Sdf_en_unknownRequest;
		Sdf_ty_s8bit        *pMethod;

		(void)sdf_ivk_uaGetMethodFromSipMessage(pSipMessage, &pMethod, pErr);
		
		(void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, pErr);

		/*
		 * If it is a REGISTER request, then just need to check the To
		 * header in the SIP message.
		 */
		if (dMsgType == Sdf_en_register)
		{
			(void)sip_getHeader(pSipMessage, SipHdrTypeTo, &dSipHeader,\
					(SipError*)	&(pErr->stkErrCode));

			(void)sip_getAddrSpecFromToHdr(&dSipHeader, &pAddrSpec, \
					(SipError*)&(pErr->stkErrCode));
			
			pContactAddrType->dProtocolScheme = pAddrSpec->dType;

			sip_freeSipAddrSpec(pAddrSpec);
			sip_freeSipHeader(&dSipHeader);

#ifdef SDF_TLS
			if (pContactAddrType->dProtocolScheme == SipAddrSipSUri)
				pContactAddrType->dTransportScheme = Sdf_en_protoTls;
			else
#endif
				pContactAddrType->dTransportScheme = Sdf_en_protoUdp;

			return Sdf_co_success;
		}

		/* 
		 * Not a REGISTER request, so first check if the Request Line 
		 * contains SIPS uri. 
		 */
		if(sip_getReqLineFromSipReqMsg(pSipMessage, \
			&pRequest, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaGetContactAddrType( ):"
				"Failed to get Request Line from Sip Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if (pRequest->pRequestUri->dType == SipAddrSipSUri)
		{
			pContactAddrType->dProtocolScheme = SipAddrSipSUri;
			pContactAddrType->dTransportScheme = Sdf_en_protoTls;
			dGotTlsTransport = Sdf_co_true;
		}
		else if(pRequest->pRequestUri->dType == SipAddrSipUri)
		{
			pContactAddrType->dProtocolScheme = SipAddrSipUri;

			(void)sdf_fn_uaGetTransportParam( pRequest->pRequestUri, \
					&(pContactAddrType->dTransportScheme), pErr);
		
			if( pContactAddrType->dTransportScheme == Sdf_en_protoTls )
				dGotTlsTransport = Sdf_co_true;
		}
		sip_freeSipReqLine(pRequest);

		if( dGotTlsTransport == Sdf_co_true )
			return Sdf_co_success;
		else
		{
			size = 0;
			/*
			 * Request Uri does not contain SIPS uti. Now check if the
			 * topmost Route header contains SIPS uri.
			 */
			(void)sip_getHeaderCount(pSipMessage,SipHdrTypeRoute, &size, \
					(SipError *)&(pErr->stkErrCode));

			if (size != 0)
			{
				(void)sip_getHeaderAtIndex(pSipMessage, SipHdrTypeRoute,
					&dSipHeader, 0, (SipError *)&(pErr->stkErrCode));

				(void)sip_getAddrSpecFromRouteHdr(&dSipHeader, \
					&pAddrSpec, (SipError *)&(pErr->stkErrCode));

				if (pAddrSpec->dType == SipAddrSipSUri)
				{
					pContactAddrType->dProtocolScheme = SipAddrSipSUri;
					pContactAddrType->dTransportScheme = Sdf_en_protoTls;
					dGotTlsTransport = Sdf_co_true;
				}
				else if(pAddrSpec->dType == SipAddrSipUri)
				{
					pContactAddrType->dProtocolScheme = SipAddrSipUri;
		
					(void)sdf_fn_uaGetTransportParam( pAddrSpec, \
							&(pContactAddrType->dTransportScheme), pErr);
		
					if( pContactAddrType->dTransportScheme == Sdf_en_protoTls )
						dGotTlsTransport = Sdf_co_true;
				}
				sip_freeSipHeader(&dSipHeader);
				sip_freeSipAddrSpec(pAddrSpec);
			}
		}
	}
	else
	{
		/* 
		 * First check if the Request Line of the incoming request 
		 * contains SIPS uri. 
		 */
		(void)sip_getReqLineFromSipReqMsg(pSipMessage, \
			&pRequest, (SipError *)&(pErr->stkErrCode));

		if (pRequest->pRequestUri->dType == SipAddrSipSUri)
		{
			pContactAddrType->dProtocolScheme = SipAddrSipSUri;
			pContactAddrType->dTransportScheme = Sdf_en_protoTls;
			dGotTlsTransport = Sdf_co_true;
		}
		else if(pRequest->pRequestUri->dType == SipAddrSipUri)
		{
			pContactAddrType->dProtocolScheme = SipAddrSipUri;

			(void)sdf_fn_uaGetTransportParam( pRequest->pRequestUri, \
					&(pContactAddrType->dTransportScheme), pErr);
		
			if( pContactAddrType->dTransportScheme == Sdf_en_protoTls )
				dGotTlsTransport = Sdf_co_true;
		}

		sip_freeSipReqLine(pRequest);

		if( dGotTlsTransport == Sdf_co_true )
			return Sdf_co_success;
		else
		{
		    size = 0;
			(void)sip_getHeaderCount(pSipMessage,SipHdrTypeRecordRoute, &size, \
					(SipError *)&(pErr->stkErrCode));

			if (size != 0)
			{
				(void)sip_getHeaderAtIndex(pSipMessage, SipHdrTypeRecordRoute,
					&dSipHeader, 0, (SipError *)&(pErr->stkErrCode));

				(void)sip_getAddrSpecFromRecordRouteHdr(&dSipHeader, \
					&pAddrSpec, (SipError *)&(pErr->stkErrCode));

				if (pAddrSpec->dType == SipAddrSipSUri)
				{
					pContactAddrType->dProtocolScheme = SipAddrSipSUri;
					pContactAddrType->dTransportScheme = Sdf_en_protoTls;
					dGotTlsTransport = Sdf_co_true;
				}
				else if(pAddrSpec->dType == SipAddrSipUri)
				{
					pContactAddrType->dProtocolScheme = SipAddrSipUri;
		
					(void)sdf_fn_uaGetTransportParam( pAddrSpec, \
							&(pContactAddrType->dTransportScheme), pErr);
		
					if( pContactAddrType->dTransportScheme == Sdf_en_protoTls )
						dGotTlsTransport = Sdf_co_true;
				}
				sip_freeSipHeader(&dSipHeader);
				sip_freeSipAddrSpec(pAddrSpec);

				if( dGotTlsTransport == Sdf_co_true)
					return Sdf_co_success;
			}
		}

		if( dGotTlsTransport != Sdf_co_true )
		{
			size = 0;
			(void)sip_getHeaderCount(pSipMessage,SipHdrTypeContactAny, &size, \
					(SipError *)&(pErr->stkErrCode));

			if (size != 0)
			{
				(void)sip_getHeaderAtIndex(pSipMessage, SipHdrTypeContactAny,
					&dSipHeader, 0, (SipError *)&(pErr->stkErrCode));

				(void)sip_getAddrSpecFromContactHdr(&dSipHeader, &pAddrSpec, \
						(SipError *)&(pErr->stkErrCode));

				if (pAddrSpec->dType == SipAddrSipSUri)
				{
					pContactAddrType->dProtocolScheme = SipAddrSipSUri;
					pContactAddrType->dTransportScheme = Sdf_en_protoTls;
					dGotTlsTransport = Sdf_co_true;
				}
				else if(pAddrSpec->dType == SipAddrSipUri)
				{
					pContactAddrType->dProtocolScheme = SipAddrSipUri;

					(void)sdf_fn_uaGetTransportParam( pAddrSpec, \
						&(pContactAddrType->dTransportScheme), pErr);
	
					if( pContactAddrType->dTransportScheme == \
							Sdf_en_protoTls )
						dGotTlsTransport = Sdf_co_true;
				}

				sip_freeSipAddrSpec(pAddrSpec);
				sip_freeSipHeader(&dSipHeader);
			}
		}
	}
#endif
	if( dGotTlsTransport != Sdf_co_true )
	{
		pContactAddrType->dProtocolScheme = SipAddrSipUri;
		pContactAddrType->dTransportScheme = Sdf_en_protoUdp;
	}
	return Sdf_co_success;
}


#else

#ifdef SDF_TLS

Sdf_ty_retVal sdf_fn_uaGetContactAddrType
#ifdef ANSI_PROTO
	(SipMessage         *pSipMessage,
	 Sdf_st_callObject  *pCallObj,
	 en_AddrType        *pContactAddrType,
	 en_SipMessageType  dReqRespType,
	 Sdf_st_error       *pErr)
#else
	( pSipMessage, pCallObj, pContactAddrType, dReqRespType, pErr )
	 SipMessage         *pSipMessage;
	 Sdf_st_callObject  *pCallObj;
	 en_AddrType        *pContactASipReqLine *pRequest = Sdf_co_null;ddrType;
	 en_SipMessageType  dReqRespType;
	 Sdf_st_error       *pErr;
#endif
{
	SipAddrSpec *pRequestAddrSpec=Sdf_co_null;
	*pContactAddrType = SipAddrSipUri;

	if (dReqRespType == SipMessageRequest)
    {
		SipReqLine          *pRequest = Sdf_co_null;
		Sdf_ty_messageType 	dMsgType = Sdf_en_unknownRequest;
		Sdf_ty_s8bit        *pMethod;

		if(sdf_ivk_uaGetMethodFromSipMessage(pSipMessage, &pMethod, \
			pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaGetContactAddrType( ):"
				"Failed to get method from Sip Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		/*w534*/ (void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, pErr);

		/*
		 * If it is a REGISTER request, then just need to check the To
		 * header in the SIP message.
		 */
		if (dMsgType == Sdf_en_register)
		{
			if (pSipMessage->pGeneralHdr->pToHdr->pAddrSpec->dType == \
					SipAddrSipSUri)
			{
				*pContactAddrType = SipAddrSipSUri;
				return Sdf_co_success;
			}
		}

		/* 
		 * Not a REGISTER request, so first check if the Request Line 
		 * contains SIPS uri. 
		 */
		if(sip_getReqLineFromSipReqMsg(pSipMessage, \
			&pRequest, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaGetContactAddrType( ):"
				"Failed to get Request Line from Sip Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
                if ( sip_getAddrSpecFromReqLine(pRequest,&pRequestAddrSpec,\
							(SipError *)&(pErr->stkErrCode) ) == SipFail ) 
                { 
                        #ifdef SDF_ERROR 
                        sdf_fn_setError(Sdf_en_majorErrors, \
                                Sdf_en_headerManipulationError,\
                                (Sdf_ty_s8bit *) "sdf_fn_uaGetContactAddrType( ):"\
                                "Failed to getAddrSpec from  Request Line",pErr); 
                        #endif 
                        pErr->errCode=Sdf_en_headerManipulationError; 
	                /* Before returning,free the Reqline allocated above */ 
                        sip_freeSipReqLine(pRequest) ; 
                        return Sdf_co_fail; 

                } 
		if (pRequestAddrSpec->dType == SipAddrSipSUri)
		{
			*pContactAddrType = SipAddrSipSUri;
		}
		else
		{
			SipRouteHeader *pRouteHeader = Sdf_co_null;
			Sdf_ty_u32bit  size = 0;

			/*
			 * Request Uri does not contain SIPS uti. Now check if the
			 * topmost Route header contains SIPS uri.
			 */
			(void)sip_listSizeOf(&(pSipMessage->u.pRequest->pRequestHdr->slRouteHdr),\
							&size, (SipError *)&(pErr->stkErrCode));

			if (size != 0)
			{
				if (sip_listGetAt(\
					&(pSipMessage->u.pRequest->pRequestHdr->slRouteHdr), 0,
					(Sdf_ty_pvoid *)&pRouteHeader, \
					(SipError*) &(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, \
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaGetContactAddrType(): "
						"Failed to get Route header from SipMessage", pErr);
#endif			
					pErr->errCode=Sdf_en_callObjectAccessError;
					/* Free already accessed ReqLine,Addrspec */ 
	        	                sip_freeSipAddrSpec(pRequestAddrSpec); 
        	        	        sip_freeSipReqLine(pRequest) ; 
                
					return Sdf_co_fail;
				}

				if (pRouteHeader->pAddrSpec->dType == SipAddrSipSUri)
				{
					*pContactAddrType = SipAddrSipSUri;
				}
			}
		}
		sip_freeSipAddrSpec(pRequestAddrSpec);
		/*
		 * The following check is required for setting the proper Contact
		 * headers in target refresh requests.
		 */
		if (*pContactAddrType == SipAddrSipUri)
		{
			if ((dMsgType == Sdf_en_invite) && \
				(pCallObj->pCallInfo->dSecureCall == Sdf_co_true))
			{
				*pContactAddrType = SipAddrSipSUri;
			}
		}

		/* Free the local reference here. */
		sip_freeSipReqLine(pRequest);
	}
	else
	{
		SipReqLine     *pRequest = Sdf_co_null;

		/* 
		 * First check if the Request Line of the incoming request 
		 * contains SIPS uri. 
		 */
		if(sip_getReqLineFromSipReqMsg(pSipMessage, \
			&pRequest, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_fn_uaGetContactAddrType( ):"
				"Failed to get Request Line from Sip Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

               if ( sip_getAddrSpecFromReqLine(pRequest,&pRequestAddrSpec,\
						   (SipError *)&(pErr->stkErrCode) ) == SipFail ) 
                { 
                        #ifdef SDF_ERROR 
                        sdf_fn_setError(Sdf_en_majorErrors, \
                                Sdf_en_headerManipulationError,\
                                (Sdf_ty_s8bit *) "sdf_fn_uaGetContactAddrType( ):"\
                                "Failed to getAddrSpec from  Request Line",pErr); 
                        #endif 
                        pErr->errCode=Sdf_en_headerManipulationError; 
                	/* Before returning,free the Reqline allocated above */ 
                        sip_freeSipReqLine(pRequest) ; 
                        return Sdf_co_fail; 
                
                } 
		if (pRequestAddrSpec->dType == SipAddrSipSUri)
		{
			*pContactAddrType = SipAddrSipSUri;
			/* Free already accessed ReqLine,Addrspec */ 
                        sip_freeSipAddrSpec(pRequestAddrSpec); 
			sip_freeSipReqLine(pRequest);
			return Sdf_co_success;
		}
		else
		{
			SipRecordRouteHeader  *pRecordRouteHeader = Sdf_co_null;
			SipContactHeader      *pContactHeader = Sdf_co_null;
            Sdf_ty_u32bit         size = 0;
            Sdf_ty_u32bit  dContactType = 0;

			SipHeader pContactHdr;

			(void)sip_listSizeOf(&(pSipMessage->pGeneralHdr->slRecordRouteHdr),\
							&size, (SipError *)&(pErr->stkErrCode));

			if (size != 0)
			{
				if (sip_listGetAt(\
					&(pSipMessage->pGeneralHdr->slRecordRouteHdr), 0,
					(Sdf_ty_pvoid *)&pRecordRouteHeader, \
					(SipError*) &(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, \
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaGetContactAddrType(): "
						"Failed to get Record-Route header from SipMsg", pErr);
#endif			
					pErr->errCode=Sdf_en_callObjectAccessError;
                    /*Codenomicon fix merge: CSR_1-6714693*/
                    sip_freeSipAddrSpec(pRequestAddrSpec);
					sip_freeSipReqLine(pRequest);
					return Sdf_co_fail;
				}

				if (pRecordRouteHeader->pAddrSpec->dType == SipAddrSipSUri)
				{
					*pContactAddrType = SipAddrSipSUri;
                    /*Codenomicon fix merge: CSR_1-6714693*/
                    sip_freeSipAddrSpec(pRequestAddrSpec);
					sip_freeSipReqLine(pRequest);
					return Sdf_co_success;
				}
			}

			(void)sip_listSizeOf(&(pSipMessage->pGeneralHdr->slContactHdr),\
							&size, (SipError *)&(pErr->stkErrCode));

			if (size != 0)
			{
				if (sip_listGetAt(\
					&(pSipMessage->pGeneralHdr->slContactHdr), 0,
					(Sdf_ty_pvoid *)&pContactHeader, \
					(SipError*) &(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, \
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaGetContactAddrType(): "
						"Failed to get Contact header from SipMsg", pErr);
#endif			
					pErr->errCode=Sdf_en_callObjectAccessError;
                    /*Codenomicon fix merge: CSR_1-6714693*/
                    sip_freeSipAddrSpec(pRequestAddrSpec);
					sip_freeSipReqLine(pRequest);
					return Sdf_co_fail;
				}
				pContactHdr.dType=SipHdrTypeContactNormal;
				pContactHdr.pHeader=pContactHeader;
   				if ( sip_getTypeFromContactHdr(&pContactHdr, &dContactType,
							(SipError *)&(pErr->stkErrCode)) == SipFail)
				{
					#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, \
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_fn_uaGetContactAddrType(): "
						"Failed to get Type from Contact header", pErr);
					#endif			
					pErr->errCode=Sdf_en_callObjectAccessError;
                    /*Codenomicon fix merge: CSR_1-6714693*/
                    sip_freeSipAddrSpec(pRequestAddrSpec);
					sip_freeSipReqLine(pRequest);
					return Sdf_co_fail;
				}

				if ((SipContactWildCard != dContactType) && \
						pContactHeader->pAddrSpec->dType == SipAddrSipSUri)
				{
					*pContactAddrType = SipAddrSipSUri;
					sip_freeSipReqLine(pRequest);
					sip_freeSipAddrSpec(pRequestAddrSpec);
					return Sdf_co_success;
				}
			}
		}
		/* Free the local reference here. */
		sip_freeSipAddrSpec(pRequestAddrSpec); 
		sip_freeSipReqLine(pRequest);
	}
	return Sdf_co_success;
}
#endif /* SDF_TLS */
#endif 
/*END: SDF_UASSF If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/

#ifdef SDF_TLS


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaMapCipherName
 **
 ** DESCRIPTION: This function maps a TLS sipher name to corresponding SSL
 **              cipher name. This is required because OpenSSL library
 **              understands only the SSL cipher names.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaMapCipherName
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit    *pTlsCipherStr,
	 Sdf_ty_s8bit    *pSslCipherStr,
	 Sdf_ty_u32bit	  dLen)
#else
	( pTlsCipherStr, pSslCipherStr,dLen )
	 Sdf_ty_s8bit    *pTlsCipherStr;
	 Sdf_ty_s8bit    *pSslCipherStr;
	 Sdf_ty_u32bit	  dLen;
#endif
{
	Sdf_ty_u32bit  i = 0;

	while (i < Sdf_co_maxNumOfCiphers)
	{
		if (Sdf_mc_strcmp(\
				(Sdf_ty_s8bit *)(sdf_gl_dCipherMapping[i].pTlsCipherName), \
				pTlsCipherStr) == 0)
		{
			Sdf_mc_strncpy(pSslCipherStr,\
					sdf_gl_dCipherMapping[i].pSslCipherName,dLen);
			return Sdf_co_success;
		}
		i++;
	}

	/* 
	 * There was no SSL cipher name found corresponding to the TLS cipher
	 * specified by pTlsCipherStr.
	 */
	return Sdf_co_fail;
}

#endif /* SDF_TLS */

#ifdef SDF_IM
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCorrectImUrlInRoute
 **
 ** DESCRIPTION: This function is used to Validate the presence of IM uri. 
 **	Scan through the Route and Record Route headers for IM URI.
 ** If IM URI contains in the Route and Record Route headers, try to
 ** convert them to either sip or sips. If conversion is not possible
 ** then through error. 
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCorrectImUrlInRoute
#ifdef ANSI_PROTO
	(SipMessage         *pSipMessage,
	 Sdf_st_error       *pError)
#else
	(pSipMessage, pError)
	 SipMessage         *pSipMessage;
	 Sdf_st_error       *pError;
#endif
{
	Sdf_ty_u32bit	dCount = 0;
	SipAddrSpec		*pAddrSpec = Sdf_co_null;
	SipHeader		*pRouteHdr = Sdf_co_null;
	Sdf_ty_u32bit	dIndex = 0;
	SipAddrSpec		*pResAddrSpec = Sdf_co_null;

	/*w534*/ (void)sip_getHeaderCount(pSipMessage,SipHdrTypeRoute, &dCount, \
			(SipError *)&(pError->stkErrCode));

	for(dIndex = 0 ; dIndex < dCount; dIndex++)
	{
		if(sip_initSipHeader(&pRouteHdr, SipHdrTypeAny, \
			(SipError *)&(pError->stkErrCode)) != SipSuccess)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to Init Sip Header", pError);
#endif			
			pError->errCode=Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}

		if(sip_getHeaderAtIndex(pSipMessage, SipHdrTypeRoute, pRouteHdr, \
			dIndex, (SipError *)&(pError->stkErrCode)) != SipSuccess)
		{
			sip_freeSipHeader(pRouteHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to get the Route Header", pError);
#endif			
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if(sip_getAddrSpecFromRouteHdr(pRouteHdr, \
			&pAddrSpec, (SipError *)&(pError->stkErrCode)) != SipSuccess )
		{
			sip_freeSipHeader(pRouteHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to get Address Spec from the Route Header", pError);
#endif			
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if(SipSuccess == sip_isImUrl(pAddrSpec, \
							(SipError *)&(pError->stkErrCode)))
		{
			/*  No need to check the return value. If it fails
			 *  then the resolved address spec will be null */
			if(sdf_fn_uaResolveImUrl(&pResAddrSpec, pAddrSpec, pError) \
					!= Sdf_co_success)
			{
#ifdef SDF_TRACE
				Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

				(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
					"Failed to Resolve the IM URL"
					"In Route Header:%s",pAddrSpec->u.pUri);
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
						0, pError);
#endif
				pResAddrSpec = Sdf_co_null;
			}

			sip_freeSipAddrSpec(pAddrSpec);
			if(pResAddrSpec == Sdf_co_null)
			{
				/* As the resolution is not possible, the presence of im url
				 * in Route header is not allowed. */
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pRouteHdr, pError);
				pError->errCode = Sdf_en_schemeNotAllowed;
				return Sdf_co_fail;
			}
			
			if(sip_setAddrSpecInRouteHdr(pRouteHdr, pResAddrSpec, \
					(SipError *)&(pError->stkErrCode)) != SipSuccess)
			{
				sip_freeSipAddrSpec(pResAddrSpec);
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
					"Failed to set the Resolved Address spec into route", \
					pError);
#endif			
				pError->errCode = Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}

			sip_freeSipAddrSpec(pResAddrSpec);

			if(sip_setHeaderAtIndex(pSipMessage, pRouteHdr, \
				dIndex, (SipError *)&(pError->stkErrCode)) != SipSuccess)
			{
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
					"Failed to set the Header at index", pError);
#endif			
				pError->errCode = Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
		}
		else
			sip_freeSipAddrSpec(pAddrSpec);

		sip_freeSipHeader(pRouteHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pRouteHdr, pError);
	}

	dCount = 0;
	
	/*w534*/ (void)sip_getHeaderCount(pSipMessage,SipHdrTypeRecordRoute, &dCount, \
		(SipError *)&(pError->stkErrCode));

	for(dIndex = 0 ; dIndex < dCount; dIndex++)
	{
		if(sip_initSipHeader(&pRouteHdr, SipHdrTypeAny, \
			(SipError *)&(pError->stkErrCode)) != SipSuccess)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to Init Sip Header", pError);
#endif			
			pError->errCode=Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}

		if(sip_getHeaderAtIndex(pSipMessage, SipHdrTypeRecordRoute, pRouteHdr, \
			dIndex, (SipError *)&(pError->stkErrCode)) != SipSuccess)
		{
			sip_freeSipHeader(pRouteHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to get the Record-Route Header", pError);
#endif			
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}	

		if(sip_getAddrSpecFromRecordRouteHdr(pRouteHdr, \
			&pAddrSpec, (SipError *)&(pError->stkErrCode)) != SipSuccess )
		{
			sip_freeSipHeader(pRouteHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to get Address Spec from the Record-Route Header",\
			   	pError);
#endif			
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if(SipSuccess == sip_isImUrl(pAddrSpec, \
							(SipError *)&(pError->stkErrCode)))
		{
			if(sdf_fn_uaResolveImUrl(&pResAddrSpec, pAddrSpec, pError) \
					!= Sdf_co_success)
			{
#ifdef SDF_TRACE
				Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

				(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
					"The resolution of IM URL in"
					"Record Route Header failed : %s", pAddrSpec->u.pUri);
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
						0, pError);
#endif
				pResAddrSpec = Sdf_co_null;
			}
			sip_freeSipAddrSpec(pAddrSpec);

			if(pResAddrSpec == Sdf_co_null)
			{
				/* As the resolution is not possible, the presence of im url
				 * in Route header is not allowed. */
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&pRouteHdr, pError);
				pError->errCode = Sdf_en_schemeNotAllowed;
				return Sdf_co_fail;
			}
			
			
			if(sip_setAddrSpecInRecordRouteHdr(pRouteHdr, pResAddrSpec, \
					(SipError *)&(pError->stkErrCode)) != SipSuccess)
			{
				sip_freeSipAddrSpec(pResAddrSpec);
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
					"Failed to set the Resolved Address spec into route", \
					pError);
#endif			
				pError->errCode = Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			sip_freeSipAddrSpec(pResAddrSpec);

			if(sip_setHeaderAtIndex(pSipMessage, pRouteHdr, \
				dIndex, (SipError *)&(pError->stkErrCode)) != SipSuccess)
			{
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
					"Failed to set the Header at index", pError);
#endif			
				pError->errCode = Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
		}
		else
			sip_freeSipAddrSpec(pAddrSpec);

		sip_freeSipHeader(pRouteHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid*)&pRouteHdr, pError);
	}
	return Sdf_co_success;
}
#endif

#ifdef SDF_PRES
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCorrectPresUrlInRoute
 **
 ** DESCRIPTION: This function is used to Validate the presence of IM uri. 
 **	Scan through the Route and Record Route headers for IM URI.
 ** If IM URI contains in the Route and Record Route headers, try to
 ** convert them to either sip or sips. If conversion is not possible
 ** then through error. 
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCorrectPresUrlInRoute
#ifdef ANSI_PROTO
	(SipMessage         *pSipMessage,
	 Sdf_st_error       *pError)
#else
	(pSipMessage, pError)
	 SipMessage         *pSipMessage;
	 Sdf_st_error       *pError;
#endif
{
	Sdf_ty_u32bit	dCount = 0;
	SipAddrSpec		*pAddrSpec = Sdf_co_null;
	SipHeader		*pRouteHdr = Sdf_co_null;
	Sdf_ty_u32bit	dIndex = 0;
	SipAddrSpec		*pResAddrSpec = Sdf_co_null;

	/*w534*/ (void)sip_getHeaderCount(pSipMessage,SipHdrTypeRoute, &dCount, \
			(SipError *)&(pError->stkErrCode));

	for(dIndex = 0 ; dIndex < dCount; dIndex++)
	{
		if(sip_initSipHeader(&pRouteHdr, SipHdrTypeAny, \
			(SipError *)&(pError->stkErrCode)) != SipSuccess)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to Init Sip Header", pError);
#endif			
			pError->errCode=Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}

		if(sip_getHeaderAtIndex(pSipMessage, SipHdrTypeRoute, pRouteHdr, \
			dIndex, (SipError *)&(pError->stkErrCode)) != SipSuccess)
		{
			sip_freeSipHeader(pRouteHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to get the Route Header", pError);
#endif			
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if(sip_getAddrSpecFromRouteHdr(pRouteHdr, \
			&pAddrSpec, (SipError *)&(pError->stkErrCode)) != SipSuccess )
		{
			sip_freeSipHeader(pRouteHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to get Address Spec from the Route Header", pError);
#endif			
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if(SipSuccess == sip_isPresUrl(pAddrSpec, \
							(SipError *)&(pError->stkErrCode)))
		{
			/*  No need to check the return value. If it fails
			 *  then the resolved address spec will be null */
			if(sdf_fn_uaResolvePresUrl(&pResAddrSpec, pAddrSpec, pError) \
					!= Sdf_co_success)
			{
#ifdef SDF_TRACE
				Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

				(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
					"Failed to Resolve the PRES URL"
					"In Route Header:%s",pAddrSpec->u.pUri);
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
						0, pError);
#endif
				pResAddrSpec = Sdf_co_null;
			}

			sip_freeSipAddrSpec(pAddrSpec);
			if(pResAddrSpec == Sdf_co_null)
			{
				/* As the resolution is not possible, the presence of im url
				 * in Route header is not allowed. */
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pRouteHdr, pError);
				pError->errCode = Sdf_en_schemeNotAllowed;
				return Sdf_co_fail;
			}
			
			if(sip_setAddrSpecInRouteHdr(pRouteHdr, pResAddrSpec, \
					(SipError *)&(pError->stkErrCode)) != SipSuccess)
			{
				sip_freeSipAddrSpec(pResAddrSpec);
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCorrectPresUrlInRoute(): "
					"Failed to set the Resolved Address spec into route", \
					pError);
#endif			
				pError->errCode = Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}

			sip_freeSipAddrSpec(pResAddrSpec);

			if(sip_setHeaderAtIndex(pSipMessage, pRouteHdr, \
				dIndex, (SipError *)&(pError->stkErrCode)) != SipSuccess)
			{
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
					"Failed to set the Header at index", pError);
#endif			
				pError->errCode = Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
		}
		else
			sip_freeSipAddrSpec(pAddrSpec);

		sip_freeSipHeader(pRouteHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pRouteHdr, pError);
	}

	dCount = 0;
	
	/*w534*/ (void)sip_getHeaderCount(pSipMessage,SipHdrTypeRecordRoute, &dCount, \
		(SipError *)&(pError->stkErrCode));

	for(dIndex = 0 ; dIndex < dCount; dIndex++)
	{
		if(sip_initSipHeader(&pRouteHdr, SipHdrTypeAny, \
			(SipError *)&(pError->stkErrCode)) != SipSuccess)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to Init Sip Header", pError);
#endif			
			pError->errCode=Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}

		if(sip_getHeaderAtIndex(pSipMessage, SipHdrTypeRecordRoute, pRouteHdr, \
			dIndex, (SipError *)&(pError->stkErrCode)) != SipSuccess)
		{
			sip_freeSipHeader(pRouteHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to get the Record-Route Header", pError);
#endif			
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}	

		if(sip_getAddrSpecFromRecordRouteHdr(pRouteHdr, \
			&pAddrSpec, (SipError *)&(pError->stkErrCode)) != SipSuccess )
		{
			sip_freeSipHeader(pRouteHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
				"Failed to get Address Spec from the Record-Route Header",\
			   	pError);
#endif			
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if(SipSuccess == sip_isPresUrl(pAddrSpec, \
							(SipError *)&(pError->stkErrCode)))
		{
			if(sdf_fn_uaResolvePresUrl(&pResAddrSpec, pAddrSpec, pError) \
					!= Sdf_co_success)
			{
#ifdef SDF_TRACE
				Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

				(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
					"The resolution of PRES URL in"
					"Record Route Header failed : %s", pAddrSpec->u.pUri);
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
						0, pError);
#endif
				pResAddrSpec = Sdf_co_null;
			}
			sip_freeSipAddrSpec(pAddrSpec);

			if(pResAddrSpec == Sdf_co_null)
			{
				/* As the resolution is not possible, the presence of im url
				 * in Route header is not allowed. */
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&pRouteHdr, pError);
				pError->errCode = Sdf_en_schemeNotAllowed;
				return Sdf_co_fail;
			}
			
			
			if(sip_setAddrSpecInRecordRouteHdr(pRouteHdr, pResAddrSpec, \
					(SipError *)&(pError->stkErrCode)) != SipSuccess)
			{
				sip_freeSipAddrSpec(pResAddrSpec);
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
					"Failed to set the Resolved Address spec into route", \
					pError);
#endif			
				pError->errCode = Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			sip_freeSipAddrSpec(pResAddrSpec);

			if(sip_setHeaderAtIndex(pSipMessage, pRouteHdr, \
				dIndex, (SipError *)&(pError->stkErrCode)) != SipSuccess)
			{
				sip_freeSipHeader(pRouteHdr);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
					(Sdf_ty_pvoid*)&pRouteHdr, pError);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaCorrectPresUrlInRoute(): "
					"Failed to set the Header at index", pError);
#endif			
				pError->errCode = Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
		}
		else
			sip_freeSipAddrSpec(pAddrSpec);

		sip_freeSipHeader(pRouteHdr);
		sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid*)&pRouteHdr, pError);
	}
	return Sdf_co_success;
}
#endif

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaIsTransactionMatched
 **
 ** DESCRIPTION: This function compares the Sip Message against the Cseq and
 **              Via branch passed.  If the via branch and Cseq present in
 **				message matches with that of inputs then return true.
 **
 ******************************************************************************/

Sdf_ty_bool sdf_fn_uaIsTransactionMatched
#ifdef ANSI_PROTO
	(SipMessage			*pSipMsg,
	Sdf_ty_s8bit		*pIncViaBr,
	Sdf_ty_s8bit		*pCseqMethod,
	Sdf_ty_u32bit		dCseqNo,
	Sdf_st_error        *pErr)
#else
	( pSipMsg, pIncViaBr, pCseqMethod, dCseqNo, pErr )
	SipMessage			*pSipMsg;
	Sdf_ty_s8bit		*pIncViaBr;
	Sdf_ty_s8bit		*pCseqMethod;
	Sdf_ty_u32bit		dCseqNo;
	Sdf_st_error        *pErr;
#endif
{
#ifdef SDF_LINT
	SipHeader		dLocalViaHdr={SipHdrTypeAny,0};
	SipHeader		dLocalCseqHdr={SipHdrTypeAny,0};
#else
    SipHeader		dLocalViaHdr={0,0};
	SipHeader		dLocalCseqHdr={0,0};
#endif
	Sdf_ty_s8bit	*pLocalViaBranch = Sdf_co_null;
	Sdf_ty_s8bit	*pLocalCseqMethod = Sdf_co_null;
	Sdf_ty_u32bit	dLocalCseqNo	= 0;
	Sdf_ty_bool		dMatched = Sdf_co_false;

#ifdef SDF_UAB2B
    Sdf_ty_s8bit *pMethod = Sdf_co_null;
	en_SipMessageType dReqRespType;
#endif

	if (pSipMsg == Sdf_co_null)
		return dMatched;

#ifdef SDF_UAB2B  
    if(sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, &pMethod, \
			pErr) == Sdf_co_fail)
			return dMatched;

	if (sip_getMessageType(pSipMsg, &dReqRespType, \
			(SipError*)&(pErr->stkErrCode)) ==  SipFail)
	{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaInsertIntoRemoteRetransList(): "
				"failed to get Message type",pErr);
#endif			
			return dMatched;
	}
#endif	
	if(sip_getHeader(pSipMsg, SipHdrTypeVia, &dLocalViaHdr,\
			(SipError*)&(pErr->stkErrCode)) == SipSuccess)
	{
		/*w534*/ (void)sdf_fn_GetBranchFromViaHdr(&dLocalViaHdr, &pLocalViaBranch, pErr);
	}
	if( sip_getHeader(pSipMsg, SipHdrTypeCseq, &dLocalCseqHdr,\
				(SipError*)&(pErr->stkErrCode)) == SipSuccess)
	{
		/*w534*/ (void)sip_getMethodFromCseqHdr(&dLocalCseqHdr, &pLocalCseqMethod, \
			   (SipError*)&(pErr->stkErrCode));
		/*w534*/ (void)sip_getSeqNumFromCseqHdr(&dLocalCseqHdr, &dLocalCseqNo, \
				(SipError*)&(pErr->stkErrCode));
	}

	if((pIncViaBr == Sdf_co_null) && (pLocalViaBranch == Sdf_co_null))
	{
		/* Means via branch is not used for comparision. Cseq method and 
		 * number are used */
		if( (Sdf_mc_strcmp(pCseqMethod, pLocalCseqMethod) == 0) && \
				(dLocalCseqNo == dCseqNo))
			dMatched = Sdf_co_true;
		else
			dMatched = Sdf_co_false;
	}
	else
	{
		if( Sdf_mc_strcmp(pLocalViaBranch, pIncViaBr) == 0)
        /*Fix for CSR 1-5292726*/
        {
                 /*Fix for CSR 1-5292726*/
      	         /*If the message is request and method is notify we
		 * extend the transaction matching to Cseq also.If 
		 * via branches match Cseq is checked.If they are
		 * same returns true else returns false.For methods
		 * other tahn NOTIFY this is nt require*/
#ifdef SDF_UAB2B
        {
            if((dReqRespType == SipMessageRequest)&&(Sdf_mc_strcmp(pMethod, "NOTIFY") == 0))
            {
                if(dLocalCseqNo == dCseqNo)
#endif
                    dMatched = Sdf_co_true;
#ifdef SDF_UAB2B
                else
                    dMatched = Sdf_co_false;
            }
            else
                dMatched = Sdf_co_true;
        }
#endif

/*Fix for CSR 1-5292726*/
        if( (Sdf_mc_strcmp(pCseqMethod, pLocalCseqMethod) == 0) && \
                          (dLocalCseqNo == dCseqNo))
        {   
           dMatched = Sdf_co_true;
        }   
       else
        {   
           dMatched = Sdf_co_false;
        }    

      }
/*Fix for CSR 1-5292726*/
		else
			dMatched = Sdf_co_false;
	}
	sip_freeSipHeader(&dLocalViaHdr);
	sip_freeSipHeader(&dLocalCseqHdr);

	return dMatched;
}

#ifdef SDF_IM
/****************************************************************************
 ** FUNCTION: sdf_fn_uaResolveImUrl
 **
 ** DESCRIPTION: This function resolves a IM URL into the Other schemes.
 **				  In through callback to application to resolve
 **	Paramaters:
 **		pImAddrSpec (IN):The Address spec containing the IM URL to be resolved
 **		ppAddrSpec (IN/OUT): The Address Spec containing the resolved URL
 **						The Memory to this address spec need to be allocated
 **						within this function itself.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_fn_uaResolveImUrl
#ifdef ANSI_PROTO
	(SipAddrSpec	**ppAddrSpec,
	SipAddrSpec		*pImAddrSpec,
	Sdf_st_error 	*pError)
#else
	(ppAddrSpec, pImAddrSpec, pError)
	SipAddrSpec		**ppAddrSpec;
	SipAddrSpec		*pImAddrSpec;
	Sdf_st_error	*pError;
#endif
{
	Sdf_ty_s8bit	pInput[Sdf_co_smallBufferSize] = "";
	Sdf_ty_s8bit	*pResolvedUrl = Sdf_co_null;
	Sdf_ty_s8bit	*pTempInput  = Sdf_co_null;
	Sdf_ty_s8bit  	*pTempValue = Sdf_co_null;
	Sdf_ty_s8bit	*pPort = Sdf_co_null;
	Sdf_ty_u32bit	dPort = 0;
	Sdf_ty_s8bit	*pScheme = Sdf_co_null;
	Sdf_ty_s8bit	*pAddr = Sdf_co_null;
	Sdf_st_error	dLocalErr;

	if(pImAddrSpec !=Sdf_co_null)
	if(sdf_cbk_uaResolveImUrl(pImAddrSpec->u.pUri, &pResolvedUrl, pError) == \
			Sdf_co_fail)
	{
#ifdef SDF_TRACE
		Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
			"Application Not Able to resolve"
			"IM URL : %s through sdf_cbk_uaResolveImUrl",
					pImAddrSpec->u.pUri);
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
				0, &dLocalErr);
#endif
		pError->errCode = Sdf_en_callBackError;
		*ppAddrSpec = Sdf_co_null;
		return Sdf_co_fail;
	}
	
	if(pResolvedUrl == Sdf_co_null)
	{
#ifdef SDF_TRACE
		Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
	if(pImAddrSpec !=Sdf_co_null) /*Fixed LINT warning */
	{
		(void)sdf_fn_uaSnprintf(Trace, Sdf_co_traceBufferSize,\
			"Application Not Able to resolve"
			"IM URL : %s through sdf_cbk_uaResolveImUrl",
					pImAddrSpec->u.pUri);
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
				0, &dLocalErr);
	} /* pImAddrSpec !=Sdf_co_null */
#endif
		pError->errCode = Sdf_en_callBackError;
		*ppAddrSpec = Sdf_co_null;
		return Sdf_co_fail;
	}

	Sdf_mc_strcpy(pInput, pResolvedUrl);

	pTempValue = Sdf_mc_strtokr(pInput,":",&pTempInput);
    Sdf_mc_strdup(pScheme, pTempValue);

	if(Sdf_mc_strcmp(pScheme, "im") == 0)
	{
#ifdef SDF_TRACE
		Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
	if(pImAddrSpec !=Sdf_co_null) /*Fixed LINT warning */
	{
		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
			"Application cannot resolve to"
			"IM URL :%s to IM URL %s again, through sdf_cbk_uaResolveImUrl",
					pImAddrSpec->u.pUri, pResolvedUrl);
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
				0, &dLocalErr);
	} /* pImAddrSpec !=Sdf_co_null */
#endif
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pScheme, \
				&dLocalErr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pResolvedUrl, \
				&dLocalErr);
		pError->errCode = Sdf_en_callBackError;
		*ppAddrSpec = Sdf_co_null;
		return Sdf_co_fail;
	}

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pResolvedUrl, \
				&dLocalErr);
	pTempValue=Sdf_mc_strtokr(pTempInput,":",&pPort);

	if( pPort != Sdf_co_null )
		dPort = Sdf_mc_atoi(pPort);

	Sdf_mc_strdup(pAddr, pTempValue);

	if(sdf_ivk_uaMakeAddrSpecFromConstituents(ppAddrSpec, pAddr, dPort, \
			pScheme, pError) != Sdf_co_success )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCorrectImUrlInRoute(): "
			"Failed to Make Address Spec from Inputs provided", pError);
#endif			
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pScheme, \
				&dLocalErr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pAddr, &dLocalErr);
		*ppAddrSpec = Sdf_co_null;
		return Sdf_co_fail;
	}
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pScheme,pError);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pAddr,pError);

	return Sdf_co_success;
}
#endif


#ifdef SDF_PRES
/****************************************************************************
 ** FUNCTION: sdf_fn_uaResolvePresUrl
 **
 ** DESCRIPTION: This function resolves a PRES URL into the Other schemes.
 **				  In through callback to application to resolve
 **	Paramaters:
 **		pPresAddrSpec (IN):The Address spec containing the IM URL to be resolved
 **		ppAddrSpec (IN/OUT): The Address Spec containing the resolved URL
 **						The Memory to this address spec need to be allocated
 **						within this function itself.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_fn_uaResolvePresUrl
#ifdef ANSI_PROTO
	(SipAddrSpec	**ppAddrSpec,
	SipAddrSpec		*pPresAddrSpec,
	Sdf_st_error 	*pError)
#else
	(ppAddrSpec, pPresAddrSpec, pError)
	SipAddrSpec		**ppAddrSpec;
	SipAddrSpec		*pPresAddrSpec;
	Sdf_st_error	*pError;
#endif
{
	Sdf_ty_s8bit	pInput[Sdf_co_smallBufferSize] = "";
	Sdf_ty_s8bit	*pResolvedUrl = Sdf_co_null;
	Sdf_ty_s8bit	*pTempInput  = Sdf_co_null;
	Sdf_ty_s8bit  	*pTempValue = Sdf_co_null;
	Sdf_ty_s8bit	*pPort = Sdf_co_null;
	Sdf_ty_u32bit	dPort = 0;
	Sdf_ty_s8bit	*pScheme = Sdf_co_null;
	Sdf_ty_s8bit	*pAddr = Sdf_co_null;
	Sdf_st_error	dLocalErr;

  if(sdf_cbk_uaResolvePresUrl(pPresAddrSpec->u.pUri, &pResolvedUrl, pError) == \
			Sdf_co_fail)
	{
#ifdef SDF_TRACE
		Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
			"Application Not Able to resolve"
			"PRES URL : %s through sdf_cbk_uaResolveImUrl",
					pPresAddrSpec->u.pUri);
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
				0, &dLocalErr);
#endif
		pError->errCode = Sdf_en_callBackError;
		*ppAddrSpec = Sdf_co_null;
		return Sdf_co_fail;
	}
	
	if(pResolvedUrl == Sdf_co_null)
	{
#ifdef SDF_TRACE
		Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

		(void)sdf_fn_uaSnprintf(Trace, Sdf_co_traceBufferSize,\
			"Application Not Able to resolve"
			"PRES URL : %s through sdf_cbk_uaResolvePresUrl",
					pPresAddrSpec->u.pUri);
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
				0, &dLocalErr);
#endif
		pError->errCode = Sdf_en_callBackError;
		*ppAddrSpec = Sdf_co_null;
		return Sdf_co_fail;
	}

	Sdf_mc_strcpy(pInput, pResolvedUrl);

	pTempValue = Sdf_mc_strtokr(pInput,":",&pTempInput);
    Sdf_mc_strdup(pScheme, pTempValue);

	if(Sdf_mc_strcmp(pScheme, "pres") == 0)
	{
#ifdef SDF_TRACE
		Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];

		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
		"Application cannot resolve to"
		"PRES URL :%s to PRES URL %s again, through sdf_cbk_uaResolvePresUrl",
					pPresAddrSpec->u.pUri, pResolvedUrl);
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, \
				0, &dLocalErr);
#endif
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pScheme, \
				&dLocalErr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pResolvedUrl, \
				&dLocalErr);
		pError->errCode = Sdf_en_callBackError;
		*ppAddrSpec = Sdf_co_null;
		return Sdf_co_fail;
	}

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pResolvedUrl, \
				&dLocalErr);
	pTempValue=Sdf_mc_strtokr(pTempInput,":",&pPort);

	if( pPort != Sdf_co_null )
		dPort = Sdf_mc_atoi(pPort);

	Sdf_mc_strdup(pAddr, pTempValue);

	if(sdf_ivk_uaMakeAddrSpecFromConstituents(ppAddrSpec, pAddr, dPort, \
			pScheme, pError) != Sdf_co_success )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCorrectPresUrlInRoute(): "
			"Failed to Make Address Spec from Inputs provided", pError);
#endif			
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pScheme, \
				&dLocalErr);
		sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pAddr, &dLocalErr);
		*ppAddrSpec = Sdf_co_null;
		return Sdf_co_fail;
	}
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pScheme,pError);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&pAddr,pError);

	return Sdf_co_success;
}
#endif


/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaIsHeaderPresent 
 **
 ** DESCRIPTION:	This function checks for the presence of given header
 **					inside a SipMessage structure.
 **
 *****************************************************************************/
Sdf_ty_bool sdf_fn_uaIsHeaderPresent
	(SipMessage		*pSipMsg,
	 en_HeaderType	dHeader,
	 Sdf_st_error	*pErr)
{
	SipHeader dSipHdr;
	
	if (sip_getHeader(pSipMsg,dHeader,&dSipHdr,(SipError*)&pErr->stkErrCode)\
			== SipFail)
	{
		return Sdf_co_false;
	}
	sip_freeSipHeader(&dSipHdr);

	return Sdf_co_true;
}
#ifdef SIP_BADMESSAGE_PARSING
/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaGetSpecificNonMandatoryBadHeaderError
 **
 ** DESCRIPTION:	This function iterates thru the bad header list and 
 **					returns specific error code w.r.t the headers.
 **
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaGetSpecificNonMandatoryBadHeaderError
(Sdf_ty_slist	*pslNonMandatoryBadHeaders,Sdf_st_error *pErr)
{
	Sdf_ty_u32bit	dCount=0,dIndex=0;

	pErr->errCode = Sdf_en_nonMandatoryHeaderError;

	/*Get the size of List*/
	if(sdf_listSizeOf(pslNonMandatoryBadHeaders,&dCount, \
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,\
			Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGetSpecificNonMandatoryBadHeaderError(): "
			"Failed to get the count of the Bad header",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	for(dIndex=0;dIndex<dCount;dIndex++)
	{
		SipBadHeader	*pBadHeader = Sdf_co_null;
		SIP_S8bit 		*pStr = Sdf_co_null;

		if(sdf_listGetAt(pslNonMandatoryBadHeaders,0,\
			(Sdf_ty_pvoid*)&pBadHeader,pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)\
					"sdf_fn_uaGetSpecificNonMandatoryBadHeaderError(): "
					"Failed to get the Bad header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if(sip_getNameFromBadHdr(pBadHeader, &pStr,\
			(SipError*)&(pErr->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)\
					"sdf_fn_uaGetSpecificNonMandatoryBadHeaderError(): "
					"Failed to Name from Bad header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
#ifdef SDF_REFER
		if(Sdf_mc_strcasecmp(pStr,"Replaces")==0) 
		{
			pErr->errCode = Sdf_en_badReplacesHeaderError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				pErr->errCode, \
				(Sdf_ty_s8bit *)\
					"sdf_fn_uaGetSpecificNonMandatoryBadHeaderError(): "
					"Bad Replaces Header",pErr);
#endif
			return Sdf_co_success;
		}
		else
#endif
		{
			continue;
		}
	}
	return Sdf_co_success;
	
}
#endif

/******************************************************************************
 ** FUNCTION	: sdf_fn_uaHandleForkedResponse
 **
 ** DESCRIPTION	: API is used to update the temp callobject with the
 **				  forked response message  details. 
 **				  Application can store this permanent in its
 **				  call object aggregation.
 **				  It issues the callback to application about the receipt
 **				  of forked responses.
 **
 ** PARAMETERS:
 **		pForkCallObj(IN): The Call Object to be Updated 
 **		pOrgCallObj(IN) : The Call Object Corresponding to main dialog
 ** 	pEventContext(IN) : The Event Context corresponding to the forked leg.
 **		pError(OUT)		: Error structure in case the function fails
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaHandleForkedResponse 
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pForkCallObj,
	 Sdf_st_callObject	*pOrgCallObj,
	 Sdf_st_eventContext	*pEventContext,
	 Sdf_st_error 		*pError)
#else
	(pForkCallObj, pOrgCallObj, pEventContext, pError)
	 Sdf_st_callObject	*pForkCallObj;
	 Sdf_st_callObject	*pOrgCallObj;
	 Sdf_st_eventContext	*pEventContext;
	 Sdf_st_error 		*pError;
#endif
{
	Sdf_st_msgInfo		dMessageInfo;
	SipMessage			*pSipMsg = Sdf_co_null;
	Sdf_st_socket		*pForkSocket = Sdf_co_null;
	Sdf_st_socket		*pOrgSocket = Sdf_co_null;
	Sdf_st_overlapTransInfo *pOverlapTransInfo = Sdf_co_null;
	en_SipMessageType 		dType;
	Sdf_ty_messageType 		dMethodType = Sdf_en_unknownRequest;
	Sdf_ty_u16bit 			dStatusCode;
#ifndef SDF_LINT
    Sdf_ty_s8bit 			*pMethod = Sdf_co_null;
#endif    
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;

#ifdef SDF_TRACE
	Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize]="";
#endif
	/* Changes for CSR 1-6197646 Starts here */
#ifdef SDF_AUTHENTICATE
       Sdf_st_listIterator dListIterator;
#endif
	/* Changes for CSR 1-6197646 Ends here */
#ifdef SDF_THREAD_SAFE
	Sdf_ty_threadId dCallingThread = 0;
	dCallingThread = sdf_fn_uaGetCallingThreadId(Sdf_co_null);
#endif

	/* We expect this function need to be invoked only on receiving
	 * the forked responses. */
	pSipMsg = pForkCallObj->pUacTransaction->pSipMsg;

	if(pSipMsg == Sdf_co_null)
		return Sdf_co_success;	
	
	if(sdf_fn_uaGetMessageInfo(pSipMsg,&dMessageInfo,pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaHandleForkedResponse(): "
			"Failed to get message info from sip message", pError);
#endif		
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(dMessageInfo.dReqRespType != SipMessageResponse)
		return Sdf_co_success;
		
  /* whenever ForkedCallObj changed to Permanent CallObj,
	 * change the callobj type to permanent callobj */
	
	pForkCallObj->dCallObjectType = Sdf_en_PermanentCallObject;

	/*
	 * Trying to populate the OverlapTransInfo structure here to handle
	 * forked RPRs - SPR 2563.
	 */
	dMethodType = dMessageInfo.dMsgType;		
	dType 		= dMessageInfo.dReqRespType;
	dStatusCode = dMessageInfo.dResponse;
#ifndef SDF_LINT
    pMethod	    = dMessageInfo.pMethod;
#endif    

	dTransactionType = sdf_fn_getTransactionType( pSipMsg, \
								dMessageInfo, Sdf_co_true, pError);

	/*w534*/ (void)sdf_fn_uaGetOverlapTransInfo( pForkCallObj,  \
		pSipMsg, &pOverlapTransInfo, dStatusCode, dTransactionType, \
		dType, dMethodType, pError);

	if( (dTransactionType == Sdf_en_overlapTransaction) ||
	 	(dTransactionType == Sdf_en_RegularUasTransaction) ||
	 	(dTransactionType == Sdf_en_RegularUacTransaction) )
	{
		if (Sdf_co_null != pOverlapTransInfo)
			pOverlapTransInfo->pSipMsg = pSipMsg;
		HSS_LOCKEDINCREF(pSipMsg->dRefCount);
	}

	if (pOverlapTransInfo != Sdf_co_null)
		pOverlapTransInfo->dLocalCseq = pOrgCallObj->pCommonInfo->dLocalRegCseq;

	if( sdf_ivk_uaInitInitData( &(pForkCallObj->pInitData), \
				pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaHandleForkedResponse(): "
			"Failed to Initialise the Init Data", pError);
#endif		
		pError->errCode = Sdf_en_memoryError;
		return Sdf_co_fail;
	}
	if(sdf_ivk_uaSetInitDataInCallObject(pForkCallObj, pOrgCallObj->pInitData,\
			pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaHandleForkedResponse(): "
			"Failed to Set the Init Data", pError);
#endif		
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
  /* Final failure Response like 4xx,3xx Ack is sending  to
	 * UAS directly instread of Proxy, it should send it through
	 * proxy */
	
	if(pOrgCallObj->pCommonInfo->pRequestUri!= Sdf_co_null)
		{
			if (sip_initSipAddrSpec(&pForkCallObj->pCommonInfo->pRequestUri,\
						pOrgCallObj->pCommonInfo->pRequestUri->dType, \
						(SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
						(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
						"Failed to initialize an AddrSpec structure.", pError);
#endif		
				pError->errCode=Sdf_en_formRequestError;
				return Sdf_co_fail;
			}


			if (sip_cloneSipAddrSpec(pForkCallObj->pCommonInfo->pRequestUri, \
						pOrgCallObj->pCommonInfo->pRequestUri, \
						(SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError,\
						(Sdf_ty_s8bit*)"sdf_fn_uaFormRequestLine( ): "
						"Failed to clone the addrspec structure for the reqline.",\
						pError);
#endif		
				pError->errCode=Sdf_en_formRequestError;
				return Sdf_co_fail;
			}

	}
#ifdef SDF_LOOKUP
    pForkCallObj->pUacTransaction->dTxnDnsState =
          pOrgCallObj->pUacTransaction->dTxnDnsState;

    if(pForkCallObj->pUacTransaction->pDestInfo 
             == Sdf_co_null)
	{
		if(sdf_ivk_uaInitTransportInfo(&(pForkCallObj->pUacTransaction->\
			pDestInfo),pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
				"Initializing transport info in destination failed ",pError);
#endif
			pError->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
	}
	if(sdf_ivk_uaCloneTransportInfo(pForkCallObj->pUacTransaction->pDestInfo, \
		pOrgCallObj->pUacTransaction->pDestInfo, pError)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
			"Cloning transport info failed ",pError);
#endif
		pError->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}
	/*
	 * Clone the dSockfd
	 */
	pForkCallObj->pUacTransaction->dSocket.dSockfd=
        pOrgCallObj->pUacTransaction->dSocket.dSockfd;
#endif
	if(sdf_fn_uaUpdateCallConstants(pForkCallObj, pSipMsg,\
			dMessageInfo, Sdf_co_true, pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaHandleForkedResponse(): "
			"Failed to Update the Call Constants in Call Object", pError);
#endif		
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
    #ifdef SDF_SESSION_TIMER
	if(pOrgCallObj->pSessionTimer != Sdf_co_null)
	{
		if (sdf_ivk_uaCloneSessionTimerInfo(pForkCallObj->pSessionTimer, \
					pOrgCallObj->pSessionTimer, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaHandleForkedResponse():  "
					"Cloning SessionTimerInfo from source to destination failed ",pError);
#endif
			pError->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}

	/* Forking call object and Org call object has same Timer Id so set 
	 * Forking Timer Id to Sdf_co_null.
	 */
	
	 pForkCallObj->pSessionTimer->pTimerId =Sdf_co_null;
	}

    /* SPR 20156: CSR 1-7927993
     * In case the Forked response is received after call
     * establishment then it shall be ignored therefore the Session 
     * Information need not be updated for such responses.
     */
    if(Sdf_en_callEstablished != pOrgCallObj->pCallInfo->dState )
    {
	   /* CSR 1-6455964: Changes start */ 
	   if( Sdf_co_success != sdf_fn_uaUpdateSessionInfoFromMsg( \
							pForkCallObj, pOverlapTransInfo, pSipMsg, dMessageInfo, \
							pOrgCallObj->pCallInfo->dState, pEventContext, pError)) 
	   { 
#ifdef SDF_ERROR 
		    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleForkedResp( ): " 
							"function UpdateSessionInfoFromMsg() Failed",pError); 
#endif 
            return Sdf_co_fail;
       } 
	   /* Code Changes End */  
    }   
#endif	



	/* Fix for SPR 2563 - passing OverlapTransInfo to the following API. */
	if(sdf_fn_uaUpdateCallObject(pForkCallObj, pOverlapTransInfo, \
			pSipMsg, dMessageInfo, pError) != Sdf_co_success)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaHandleForkedResponse(): "
			"Failed to Update the Call Object with Message Info", pError);
#endif		
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
/* Changes for CSR 1-6197646 Starts here */

#ifdef SDF_AUTHENTICATE
  
  /* The Auth header is updated in to the original call object's
   * persistant header list after receiving the 401/407 
   * and this should be cloned in to forked call-Object */

  /* delete previous entries of authorization and 
	 * proxy-authorization headers from the persistent list.
	 */
  (void)sdf_listDeleteAll(&(pForkCallObj->slPersistentHeaders), pError);

	/* get the list from original call object persistant heard list */
  (void)sdf_listInitIterator(&(pOrgCallObj->slPersistentHeaders), \
		&dListIterator, pError);

    while (dListIterator.pCurrentElement != Sdf_co_null)
    {
	Sdf_st_headerList *pHeaderList;
	Sdf_st_headerList *pClonedHdrList;

	pHeaderList =(Sdf_st_headerList*)\
		(dListIterator.pCurrentElement->pData);
    
       /* clone and append to forked call object with authorization or proxy 
        * authorization header */
       if( Sdf_mc_strcmp(pHeaderList->pCorrelation,"uat-authenticate") == 0 )
       {
    
	  if(sdf_ivk_uaInitHeaderList(&pClonedHdrList, pHeaderList->dType,\
			    pHeaderList->pCorrelation, pError)==Sdf_co_fail)
          {
#ifdef SDF_ERROR
              sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
				    (Sdf_ty_s8bit *)"sdf_fn_uaHandleForkedResponse(): " 
				    "Initializing HeaderList for Persistent Headers failed ",pError);
#endif
             return Sdf_co_fail;
          }
          if(sdf_ivk_uaCloneHeaderList(pClonedHdrList, pHeaderList,\
			  pError)==Sdf_co_fail)
          {
#ifdef SDF_ERROR
              sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaHandleForkedResponse(): " 
				"Cloning Persistent Headers List failed ",pError);
#endif
              return Sdf_co_fail;
          }
          if(sdf_listAppend(&(pForkCallObj->slPersistentHeaders), (Sdf_ty_pvoid)\
                       pClonedHdrList, pError) == Sdf_co_fail)
          {
#ifdef SDF_ERROR
              sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				  (Sdf_ty_s8bit *)"sdf_fn_uaHandleForkedResponse(): " 
				  "Appending PersistentHeader to slPersistentHeaders of "
				  "destination failed ",pError);
#endif
              pError->errCode=Sdf_en_callObjectAccessError;
			  return Sdf_co_fail;
          }
          break;
      }
      (void)sdf_listNext(&dListIterator, pError);
    }
#endif

/* Changes for CSR 1-6197646 Ends here */
	pForkCallObj->pCommonInfo->pKey->dCaller = Sdf_en_caller;
    /* 1-7697931  Start */
	pForkCallObj->pCommonInfo->dScheme = \
			pForkCallObj->pUacTransaction->pDestInfo->dScheme;
	/* 1-7697931  End */

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0, &(pForkCallObj->dCallStateMutex), 0);
#endif
	pForkCallObj->pCallInfo->dState = pOrgCallObj->pCallInfo->dState;
	
	/* Fix for SPR 2563 - passing OverlapTransInfo to the following API. */
	if (sdf_ivk_uaChangeCallState(pForkCallObj, pOverlapTransInfo, \
		pSipMsg, Sdf_en_msgModeRecd, pError) == Sdf_co_fail)
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(pForkCallObj->dCallStateMutex));
#endif
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callStateError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaHandleForkedResponse(): "
			"Changing call state failed", pError);
#endif
		pError->errCode=Sdf_en_callStateError;
		return Sdf_co_fail;
	}
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0,	&(pForkCallObj->dCallStateMutex));
#endif

	/*Clone the Media Info from the Orig call object to the Forked Call
	 * Object  */

	if(sdf_ivk_uaCloneMediaHandling(pForkCallObj->pMediaInfo, \
		pOrgCallObj->pMediaInfo, pError)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaHandleForkedResponse(): "
			"Cloning MediaHandling from source to destination failed ",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}
	
	pForkCallObj->pCommonInfo->dSdpInInvite = \
			pOrgCallObj->pCommonInfo->dSdpInInvite;

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0, &(pForkCallObj->dCallStateMutex), 0);
#endif

/*Merged for SPR-19294*/
/*CSR_1-5232801 starts*/

/*   This is for handling Media state information properly.
     Suppose the initial invite contains an SDP and first 2xx response contain an answer, 
     then the media state will be in established state. So in case of reception of a forked 
     2xx response, after cloning the media information, the forked object's media state information 
     will be also in media established state, which is wrong for forked leg.
     For forked call object initial media state should be in 'offerSent' state if the initial invite
     contains an SDP message body, otherwise it should be in 'idlemedia' state. 
*/
        if(Sdf_co_true == (pForkCallObj->pCommonInfo->dSdpInInvite) )
        {
            pForkCallObj->pMediaInfo->dMediaState = Sdf_en_offerSent;		
	}
        else
        {
            pForkCallObj->pMediaInfo->dMediaState = Sdf_en_idleMedia;
        }

/*CSR_1-5232801 ends*/
/*Merge end for SPR-19294*/

#ifndef SDF_UASSF
	if(sdf_fn_uaChangeMediaState(pForkCallObj, \
			&(pForkCallObj->pMediaInfo->dMediaStateChange),\
			dMessageInfo, pSipMsg,Sdf_en_msgModeRecd, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_mediaStateTransitionError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaHandleForkedResponse(): "
			"Changing Media state failed",pError);
#endif
		pError->errCode=Sdf_en_mediaStateTransitionError;
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,	&(pForkCallObj->dCallStateMutex));
#endif
		return Sdf_co_fail;
	}
#endif /* SDF_UASSF */

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0,	&(pForkCallObj->dCallStateMutex));
#endif
	

#ifdef SDF_THREAD_SAFE
	/*w534*/ (void)sdf_ivk_uaLockCallObject(pForkCallObj, pError);
#endif

#ifndef SDF_UASSF
	/* Start the No Answer timer on receiving the provisional responses*/
	if(dMessageInfo.dResponse < 200)
	{
		/*w534*/ (void)sdf_fn_uaStartNoAnswerTimer(pForkCallObj, pEventContext, \
			Sdf_co_false, pError);
	}
#endif /* SDF_UASSF */

#ifdef SDF_TRACE
	(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
		"Call : %s - Forked Response Received; "
		"Issuing sdf_cbk_uaForkedRespReceived to application",\
		pForkCallObj->pCommonInfo->pKey->pCallId);
	sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace, 0,pError);
#endif
	/* If the incoming forked response is received over the Same TCP 
	 * connection as that of the main dialog, then reset the socket fd
	 * on forked transaction. So that while deleting the forked
	 * transaction, it will not delete the main dialogs socket */

	pForkSocket = &(pForkCallObj->pUacTransaction->dSocket);
	pOrgSocket = &(pOrgCallObj->pUacTransaction->dSocket);

	if( pForkSocket->dProtocol != Sdf_en_protoUdp )
	{
		if( pForkSocket->dSockfd == pOrgSocket->dSockfd )
			pForkSocket->dSockfd = Sdf_co_InvalidSocketFd;
	}

/*CSR_1-4953913*/

	/* Update the Cseq of the Forked CallObject to be the same as that of
	 * OriginalCall Obj*/

	if (sdf_ivk_uaSetCseqInCallObject(pForkCallObj,\
		(pOrgCallObj->pUacTransaction->pSipMsg->pGeneralHdr->pCseqHdr)->dIntVar1,\
		pError)== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_mediaStateTransitionError, \
		(Sdf_ty_s8bit *)"sdf_fn_uaHandleForkedResponse(): "
		"Changing Media state failed",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,	&(pForkCallObj->dCallStateMutex));
#endif
		return Sdf_co_fail;

	}
			
    /*CSR_1-4953913*/
	
	/* It means the received response is an forked response.
	 * Issue an callback to application to indicate this.*/
	/* Fix for SPR 2563 - passing OverlapTransInfo to the following cbk. */
	/*w534*/ (void)sdf_cbk_uaForkedRespReceived(pForkCallObj, pOrgCallObj, pOverlapTransInfo,
			pEventContext, pError);

#ifdef SDF_THREAD_SAFE
	if (sdf_fn_uaTryLock(&(pForkCallObj->dCallObjectMutex), 
			dCallingThread) == Sdf_co_success)
		/*w534*/ (void)sdf_ivk_uaUnlockCallObject(pForkCallObj, pError);
#endif

	return Sdf_co_success;
}




/******************************************************************************
 ** FUNCTION	: sdf_fn_uaIsContactHdrMandatory
 **
 ** DESCRIPTION	: This function is used to check whetehr contact header is 
 **				  Mandatory for the request/response or not.
 **
 ** PARAMETERS:
 **		dReqRespType(IN)	: The request/response type 
 **		dMsgType(IN) 		: The request/response method type
 ** 	dResponseCode(IN) 	: If response then response-code else 0.
 **
 ** RETURN VALURE:
 **		Sdf_co_true if Contact hdr is mandatory
 **		Sdf_co_false if Contact hdr is not mandatory.
 **
 *****************************************************************************/

Sdf_ty_bool sdf_fn_uaIsContactHdrMandatory
#ifdef ANSI_PROTO
	(en_SipMessageType	dReqRespType, \
	 Sdf_ty_messageType dMsgType, \
	 Sdf_ty_u16bit		dResponseCode)
#else
	(dReqRespType, dMsgType, dResponseCode)
   	 en_SipMessageType	dReqRespType;
	 Sdf_ty_messageType dMsgType;
	 Sdf_ty_u16bit		dResponseCode;
#endif
{
	if(SipMessageRequest ==	dReqRespType)
	{
		if((Sdf_en_invite == dMsgType) /*RFC 3261*/
#ifdef SIP_IMPP
				|| (Sdf_en_subscribe == dMsgType) || (Sdf_en_notify == dMsgType)	/*RFC 3265 */
#endif
#ifdef SDF_REFER				
				|| (Sdf_en_refer == dMsgType) /*RFC 3515 */
#endif
				|| (Sdf_en_update == dMsgType))	/*RFC 3311 */
		{
			return Sdf_co_true;
		}
	}
	else if(SipMessageResponse == dReqRespType)
	{
		if((200 <= dResponseCode) && (300 > dResponseCode))
		{
			if((Sdf_en_invite == dMsgType) /*RFC 3261 */
#ifdef SIP_IMPP
					|| (Sdf_en_subscribe == dMsgType) /*RFC 3265 */
#endif
#ifdef SDF_REFER
					|| (Sdf_en_refer == dMsgType) /*RFC 3515 */
#endif
					|| (Sdf_en_update == dMsgType))	/*RFC 3311 */
				
			{
				return Sdf_co_true;
			}
		}
#ifdef SIP_IMPP
		else if((300 <= dResponseCode) && (400 > dResponseCode))
		{
			if((Sdf_en_subscribe == dMsgType) || (Sdf_en_notify == dMsgType)) /*RFC 3265 */
			{
				return Sdf_co_true;
			}
		}
#endif
	}
	return Sdf_co_false; 
}


#ifdef SDF_RPORT





/*****************************************************************************
** FUNCTION: sdf_fn_uaPopulateViaRportParam
**
** DESCRIPTION: 
*****************************************************************************/



Sdf_ty_matchMessageType sdf_fn_uaPopulateViaRportParam
	( SipMessage  	*pMsg,
	  Sdf_st_eventContext *pEventContext,
	  Sdf_ty_u32bit *pIsrportpresent,	
	  Sdf_st_error *pErr)
{
	SipHeader dViaHeader;
	Sdf_ty_s8bit *pActualReceivedIp = Sdf_co_null, *pPort_str, *pTmpValue;
	Sdf_ty_u32bit dActualReceivedPort,dViaParamCount;
	SipParam *pViaParam, *pReceivedParam;
	Sdf_ty_u32bit dIndex;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_fn_uaPopulateViaRportParam");



#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pEventContext == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam( ): "
			"Invalid Param: pEventContext",pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}

	if(pMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam( ): "
			"Invalid Param: pMsg",pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}

	if(pIsrportpresent == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam( ): "
			"Invalid Param: pIsrpotpresent",pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}


	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam( ): "
			"Invalid Param: pErr",pErr);
#endif			
		return Sdf_en_fail;
	}
#endif	/* End of param validation ifdef */





	if(sip_getHeaderAtIndex(pMsg, SipHdrTypeVia, \
		&dViaHeader, (Sdf_ty_u32bit)0, (SipError*)&(pErr->stkErrCode)) \
		== SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam(): "
			"Failed to get topmost Via header from SIP message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}


	
	*pIsrportpresent=0;

	dActualReceivedPort=pEventContext->pTranspAddr->dPort;
	if(sip_getViaParamCountFromViaHdr(&dViaHeader, &dViaParamCount, \
			(SipError *)&(pErr->stkErrCode)) == SipFail )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam(): "
			"Failed to get via param count from topmost Via header of SIP message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		sip_freeSipHeader(&dViaHeader);
		return Sdf_en_fail;


	}
	for(dIndex=0; dIndex<dViaParamCount; dIndex++)
	{
		if(sip_getViaParamAtIndexFromViaHdr(&dViaHeader, &pViaParam, \
			dIndex, (SipError *)&(pErr->stkErrCode)) == SipFail )
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam(): "
				"Failed to get via param from topmost Via header of SIP message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;

			sip_freeSipHeader(&dViaHeader);
			return Sdf_en_fail;

		}


		if (sip_getNameFromSipParam(pViaParam, &pTmpValue, \
			(SipError *)&(pErr->stkErrCode)) == SipFail )
		{

#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam(): "
					"Failed to get name from via param in topmost Via header of SIP message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;

			sip_freeSipParam(pViaParam);
			sip_freeSipHeader(&dViaHeader);
			return Sdf_en_fail;

		}
		if(strcmp(pTmpValue, "rport")==0)
		{
			*pIsrportpresent=1;
			break;
		}
		sip_freeSipParam(pViaParam);

	}
	if(*pIsrportpresent==1)
	{
		pPort_str= (Sdf_ty_s8bit *)sdf_memget(0, 7, pErr); 
		(void)sdf_fn_uaSnprintf(pPort_str, 7, "%d", dActualReceivedPort);


		if(sip_insertValueAtIndexInSipParam(pViaParam, pPort_str, \
			(Sdf_ty_u32bit)0, (SipError *)&(pErr->stkErrCode)) ==SipFail)
		{

#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam(): "
				"Failed to insert value in SipParam",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;

			(void)sdf_memfree(0,(void **) &pPort_str, pErr);
			sip_freeSipParam(pViaParam);
			sip_freeSipHeader(&dViaHeader);
			return Sdf_en_fail;

		}

		sip_freeSipParam(pViaParam);

		


	
		pActualReceivedIp = pEventContext->pTranspAddr->pHost;
			
			/*
			 * Add the received param here.
			 */
		if (sip_initSipParam(&pReceivedParam, (SipError *)\
				&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam(): "
			"Failed to init a SipParam structure.",pErr);
#endif			
			sip_freeSipHeader(&dViaHeader);
			return Sdf_en_fail;
		}

		pTmpValue = Sdf_mc_strdupCallHandler("received");

		if (sip_setNameInSipParam(pReceivedParam, pTmpValue, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam(): "
			"Failed to set name in a SipParam structure.",pErr);
#endif			
			sip_freeSipParam(pReceivedParam);
			sip_freeSipHeader(&dViaHeader);
			return Sdf_en_fail;
		}

		pTmpValue = Sdf_mc_strdupCallHandler(pActualReceivedIp);
		
		if (sip_insertValueAtIndexInSipParam(pReceivedParam, pTmpValue, \
			(Sdf_ty_u32bit)0, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam(): "
				"Failed to insert value in a SipParam structure.",pErr);
#endif		
			sip_freeSipParam(pReceivedParam);
			sip_freeSipHeader(&dViaHeader);

			return Sdf_en_fail;
		}
		if (sip_insertViaParamAtIndexInViaHdr(&dViaHeader, pReceivedParam,\
			(Sdf_ty_u32bit)0, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaPopulateViaRportParam(): "
				"Failed to insert value in a SipParam structure.", \
				pErr);
#endif		
			sip_freeSipParam(pReceivedParam);
			sip_freeSipHeader(&dViaHeader);

			return Sdf_en_fail;
		}
		sip_freeSipParam(pReceivedParam);
		
	}


	/* Free the local reference */
	sip_freeSipHeader(&dViaHeader);
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_fn_uaPopulateViaRportParam");
	return Sdf_en_success;
	
}
#endif /* SDF_RPORT */
/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaAddHeaderInPersistentHdrList
 **
 ** DESCRIPTION:	This function stores the given Header into
 **					Persistent hdrList
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAddHeaderInPersistentHdrList
(Sdf_st_callObject	*pCallObject,
 SipHeader	*pHeader,
 en_HeaderType	dType,
 Sdf_ty_s8bit	*pCorrelation,
 Sdf_st_error	*pErr)
{
  Sdf_st_headerList 	*pHeaderList = Sdf_co_null;
  Sdf_ty_slist 	          slHeaderList ; 
  Sdf_ty_u32bit	          dSize=0;
  Sdf_ty_u32bit	          dListSize=0;

#ifdef SDF_PARAMVALIDATION
  if(pCallObject == Sdf_co_null)
  {
#ifdef SDF_ERROR		
    sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
        (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList( ): "
        "Invalid Param:pCallObject",pErr);
#endif			
    pErr->errCode=Sdf_en_invalidParamError;
    return Sdf_co_fail;
  }

  if(pHeader == Sdf_co_null)
  {
#ifdef SDF_ERROR		
    sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
        (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList( ): "
        "Invalid Param:pHeader",pErr);
#endif			
    pErr->errCode=Sdf_en_invalidParamError;
    return Sdf_co_fail;
  }
  if(pErr == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR -"\
        "sdf_fn_uaAddHeaderInPersistentHdrList( ): Invalid pErr "
        "parameter passed to the function");
#endif
    return Sdf_co_fail;
  }

/*  if(dType == Sdf_co_null)
  {
#ifdef SDF_ERROR		
    sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
        (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList( ): "
        "Invalid Param:dType",pErr);
#endif			
    pErr->errCode=Sdf_en_invalidParamError;
    return Sdf_co_fail;
  }
*/
  if(pCorrelation == Sdf_co_null)
  {
#ifdef SDF_ERROR		
    sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
        (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList( ): "
        "Invalid Param:pCorrelation",pErr);
#endif			
    pErr->errCode=Sdf_en_invalidParamError;
    return Sdf_co_fail;
  }
#endif	/* End of param validation ifdef */

  
#ifdef SDF_PRIVACY 
  /* If the Header is P-Preffered-Id, then check the list size,
   * if list size is 1 then no need to do init function for the
   * header.   
   */
  if (dType == SipHdrTypePPreferredId)
  {
      if (sdf_ivk_uaGetHeaderListByCorrelation(&(pCallObject->\
                      slPersistentHeaders),(Sdf_ty_s8bit *) "uat-Preffered", \
                  &slHeaderList, pErr) == Sdf_co_fail)
      {
#ifdef SDF_ERROR			
          sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
                  (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList(): "
                  "Failed to get header list by correlation "
                  "uat-Preffered", pErr);
#endif				
          return Sdf_co_fail;
      }
  }
  if (dType == SipHdrTypePrivacy)
  {
      if (sdf_ivk_uaGetHeaderListByCorrelation(&(pCallObject->\
                      slPersistentHeaders),(Sdf_ty_s8bit *) "uat-Privacy", \
                  &slHeaderList, pErr) == Sdf_co_fail)
      {
#ifdef SDF_ERROR			
          sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
                  (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList(): "
                  "Failed to get header list by correlation "
                  "uat-Privacy", pErr);
#endif				
          return Sdf_co_fail;
      }
  }
  
#endif
#ifdef SDF_CONGEST 
  /* If the Header is Resource-Priority, then check the list size,
   * if list size is 1 then no need to do init function for the
   * header.
   */
  if ( dType == SipHdrTypeRsrcPriority )
  {
      if (sdf_ivk_uaGetHeaderListByCorrelation(&(pCallObject->slPersistentHeaders),
                  (Sdf_ty_s8bit *) "uatk-Rsrcpriority",&slHeaderList, pErr) == Sdf_co_fail)
      {
#ifdef SDF_ERROR			
          sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
                  (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList(): "
                  "Failed to get header list by correlation "
                  "uatk-Rsrcpriority", pErr);
#endif				
          return Sdf_co_fail;
      }
  }
#endif
#ifdef SDF_3GPP 
  /* If the Header is History-Info, then check the list size,
   * if list size is 1 then no need to do init function for the
   * header.
   */
  if ( dType == SipHdrTypeHistoryInfo )
  {
      if (sdf_ivk_uaGetHeaderListByCorrelation(&(pCallObject->slPersistentHeaders),
                  (Sdf_ty_s8bit *) "uatk-Historyinfo",&slHeaderList, pErr) == Sdf_co_fail)
      {
#ifdef SDF_ERROR			
          sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
                  (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList(): "
                  "Failed to get header list by correlation "
                  "uatk-Historyinfo", pErr);
#endif				
          return Sdf_co_fail;
      }
  }
#endif
#ifdef SDF_PEMEDIA 
  /* If the Header is P-Early-Media, then check the list size,
   * if list size is 1 then no need to do init function for the
   * header.
   */
  if ( dType == SipHdrTypePEarlyMedia )
  {
      if (sdf_ivk_uaGetHeaderListByCorrelation(&(pCallObject->slPersistentHeaders),
                  (Sdf_ty_s8bit *) "uatk-Pearlymedia",&slHeaderList, pErr) == Sdf_co_fail)
      {
#ifdef SDF_ERROR			
          sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
                  (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList(): "
                  "Failed to get header list by correlation "
                  "uatk-Historyinfo", pErr);
#endif				
          return Sdf_co_fail;
      }
  }
#endif
  
  if ( Sdf_co_fail == sdf_listSizeOf(&slHeaderList, &dSize, pErr) )
  {
#ifdef SDF_ERROR				
      sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
          (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList( ): "
          "Failed to get the list size of Header List from" \
          "Persistant Headers",pErr);
#endif				
      return Sdf_co_fail;
      
      
  }

  if( dSize > 0 )
  {
    if (sdf_listGetAt(&slHeaderList, 0, \
          (Sdf_ty_pvoid *)&pHeaderList, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR				
      sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
          (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList( ): "
          "Failed to get Header List from" \
          "Persistant Headers",pErr);
#endif				
      (void)sdf_listDeleteAll(&slHeaderList,pErr);
      return Sdf_co_fail;
    }
    (void)sdf_listSizeOf(&(pHeaderList->slHeaders), &dListSize, \
                         pErr);
  }
  if ((dListSize > 0) && (pHeaderList != Sdf_co_null))
  {
    if(sdf_listAppend(&(pHeaderList->slHeaders),\
          (Sdf_ty_pvoid)pHeader,pErr) == Sdf_co_fail )
    {
      (void)sdf_ivk_uaFreeHeaderList(pHeaderList);
#ifdef SDF_ERROR
      sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
          (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList(): "
          "Failed to append the header to header list", pErr);
#endif
      return Sdf_co_fail;
    }

/*    if ((sdf_listAppend(&(pCallObject->slPersistentHeaders), \
            (Sdf_ty_pvoid)pHeaderList, pErr)) == Sdf_co_fail)
    {
      (void)sdf_ivk_uaFreeHeaderList(pHeaderList);
#ifdef SDF_ERROR
      sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
          (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList(): "
          "Failed to add the header list of headers to "
          "the PersistentHeaders list of callObject", pErr);
#endif
      return Sdf_co_fail;
    }*/
  }

  else
  {

    /* Initialise a headerlist of */
    if(sdf_ivk_uaInitHeaderList(&pHeaderList, \
          dType,pCorrelation, pErr) == Sdf_co_fail)
      return Sdf_co_fail;

    /* Add the header into the 	list */

    if(sdf_listAppend(&(pHeaderList->slHeaders),\
          (Sdf_ty_pvoid)pHeader,pErr) == Sdf_co_fail )
    {
      (void)sdf_ivk_uaFreeHeaderList(pHeaderList);
#ifdef SDF_ERROR
      sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
          (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList(): "
          "Failed to append the header to header list", pErr);
#endif
      return Sdf_co_fail;
    }

    /* 
     * Append the pHeaderList to the slPersistentHeaders 
     * header list structures in the call object
     */

    if ((sdf_listAppend(&(pCallObject->slPersistentHeaders), \
            (Sdf_ty_pvoid)pHeaderList, pErr)) == Sdf_co_fail)
    {
      (void)sdf_ivk_uaFreeHeaderList(pHeaderList);
#ifdef SDF_ERROR
      sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
          (Sdf_ty_s8bit *)"sdf_fn_uaAddHeaderInPersistentHdrList(): "
          "Failed to add the header list of headers to "
          "the PersistentHeaders list of callObject", pErr);
#endif
      return Sdf_co_fail;
    }
  }
	/* Fix for  Memleak in MicroUA*/
	  (void)sdf_listDeleteAll(&slHeaderList,pErr);
  return	Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaDeleteParamsFromUrl
 **
 ** DESCRIPTION:	This function deletes the params in the url by
 **                 finding the param names and comparing them with
 **                 the list of names passed
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaDeleteParamsFromUrl
(SipUrl         *pSipUrl,
 Sdf_ty_s8bit	**ppParamNameList,
 Sdf_ty_u32bit  dListSize,
 Sdf_st_error	*pError)
{
    SipParam        *pSipParam=SIP_NULL;
    Sdf_ty_u32bit    dCount=0;
    Sdf_ty_u32bit    dIndex=0;
    Sdf_ty_u32bit    dListIndex=0;

    if (sip_listSizeOf(&(pSipUrl->slParam), &dCount ,(SipError *)&(pError->stkErrCode)) == SipFail )
    {
        return Sdf_co_fail;
    }
    /*      Iterate through the list of params*/        
    for(dIndex=0;dIndex<dCount;dIndex++)
    {
        if (sip_getUrlParamAtIndexFromUrl(pSipUrl,&pSipParam,dIndex, \
                    (SipError*)&(pError->stkErrCode)) == SipFail)
        {
            return Sdf_co_fail;
        }
        dListIndex = 0 ;
        /* If method or header param is present,then delete it*/
        while ( dListIndex < dListSize )
        {
            if( Sdf_mc_strcmp(pSipParam->pName,*(ppParamNameList+dListIndex)) == 0 )
            {
                if (sip_deleteUrlParamAtIndexInUrl(pSipUrl,dIndex, \
                            (SipError *)&(pError->stkErrCode)) == SipFail)
                {
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
                            (Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
                            "Failed to delete method or header param from contact URL ",pError);
#endif
                    pError->errCode=Sdf_en_headerManipulationError;
                    sip_freeSipParam(pSipParam);
                    return Sdf_co_fail;
                }
                dIndex--;
                dCount--;
                break;
            }
            else
            {
                dListIndex ++;
            }
        }
        sip_freeSipParam(pSipParam);
    }
    return Sdf_co_success;
}
#ifdef SDF_3GPP
/*******************************************************************
** FUNCTION: sdf_fn_uaAddHistoryInfoHeader
**
** DESCRIPTION: 
**
** PARAMETERS:
**		pCallObj   (IN)   :Hist-Info header will be added in the persistant
**                          hdr list of this CallObject 
**		pSipMsg   (IN)    :sipMessage to which the Header to added
**		pAddrSpec  (IN)   : AddressSpec to be used to form History-Info hdr
**      pHistoryIndex (IN): Index to be used to form the History-Info hdr
**      dListIndex (IN)   : Index at which the History-Info hdr has to be set
**                          in the sipmsg.
**		pErr(OUT)		  : Error structure in case the function fail
**
**
*******************************************************************/

Sdf_ty_retVal sdf_fn_uaAddHistoryInfoHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	 SipMessage  *pSipMsg,
	 SipAddrSpec *pAddrSpec,
	 Sdf_ty_s8bit *pHistoryIndex,
     Sdf_ty_u32bit dListIndex,
	 Sdf_st_error *pErr)
#else
	(pCallObj,pSipMsg,pAddrSpec,pIndex,pErr )
     Sdf_st_callObject *pCallObj,
	 SipMessage  *pSipMsg,
	 SipAddrSpec *pAddrSpec,
	 Sdf_ty_s8bit *pHistoryIndex,
	 Sdf_ty_u32bit dListIndex,
	 Sdf_st_error *pErr;
#endif
{
	SipHeader       *pHistoryInfoHdr;
    Sdf_st_error	dLocalErr;
	SipParam 		*pParam;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
            "sdf_fn_uaAddHistoryInfoHeader");

#ifdef SDF_PARAMVALIDATION

    if(pErr == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR -"\
                "sdf_fn_uaAddHistoryInfoHeader( ): Invalid pError "
                "parameter passed to the function");
#endif
        return Sdf_co_fail;
    }

    if(pSipMsg == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR -"\
                "sdf_fn_uaAddHistoryInfoHeader( ): Invalid pSipMsg"
                "parameter passed to the function");
#endif
        return Sdf_co_fail;
    }

    if(pAddrSpec == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR -" \
                "sdf_fn_uaAddHistoryInfoHeader( ):"
                "Invalid pAddeSpec parameter  passed to the function");
#endif
        return Sdf_co_fail;
    }
    if(pHistoryIndex == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR -" \
                "sdf_fn_uaAddHistoryInfoHeader( ):"
                "Invalid History-Info Index parameter  passed to the function");
#endif
        return Sdf_co_fail;
    }
#endif 	/* end-of-param-validation */
	if (SipFail == sip_initSipHeader(&pHistoryInfoHdr,SipHdrTypeHistoryInfo,\
                (SipError *)&(pErr->stkErrCode)))
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_fn_uaAddHistoryInfoHeader:"
                " initsipheader failed", pErr);
#endif
        return Sdf_co_fail;
    }
	if(SipFail == sip_setAddrSpecInHistoryInfoHdr(pHistoryInfoHdr,pAddrSpec,\
                (SipError *)&(pErr->stkErrCode)))
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_fn_uaAddHistoryInfoHeader:"
                "Failed to set Addrspec in HistoryInfo Header", pErr);
#endif
        if(pHistoryInfoHdr != Sdf_co_null)
        {
            sip_freeSipHeader(pHistoryInfoHdr);		
            (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHistoryInfoHdr,&dLocalErr);
        }
        return Sdf_co_fail;
    }

		(void)sip_initSipParam(&pParam,(SipError *)&(pErr->stkErrCode));
	    
	    if(Sdf_co_null != pParam)
		{	
			pParam->pName = Sdf_mc_strdupConfig("index");
			(void)sip_listAppend(&(pParam->slValue),pHistoryIndex,(SipError *)&(pErr->stkErrCode));
		}

		if(sip_insertParamAtIndexInHistoryInfoHdr(pHistoryInfoHdr,pParam,0,(SipError *)&(pErr->stkErrCode)) == SipFail)
        {
            pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
            sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                    (Sdf_ty_s8bit*)"sdf_fn_uaAddHistoryInfoHeader:"
                    "Failed to insert index in HistoryInfo Header", pErr);
#endif
            if(pHistoryInfoHdr != Sdf_co_null)
            {
                sip_freeSipHeader(pHistoryInfoHdr);		
                (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHistoryInfoHdr,&dLocalErr);
            }
            sip_freeSipParam(pParam);
            return Sdf_co_fail;
        }
        sip_freeSipParam(pParam);   
    /* insert the formed History-Info header into the message passed */
    if(sip_insertHeaderAtIndex(pSipMsg,pHistoryInfoHdr,dListIndex, \
                (SipError*)&(pErr->stkErrCode))==SipFail)
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_fn_uaAddHistoryInfoHeader:"
                "Failed to insert History-Info Header", pErr);
#endif
        if(pHistoryInfoHdr != Sdf_co_null)
        {
            sip_freeSipHeader(pHistoryInfoHdr);		
            (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHistoryInfoHdr,&dLocalErr);
        }
        return Sdf_co_fail;
    }
	if ( Sdf_co_fail == sdf_fn_uaAddHeaderInPersistentHdrList(pCallObj, \
                pHistoryInfoHdr,SipHdrTypeHistoryInfo,"uatk-Historyinfo",pErr))
	{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaAddHistoryInfoHeader(): "
				"sdf_fn_uaAddHeaderInPersistentHdrList ",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
            sip_freeSipHeader(pHistoryInfoHdr);		
            (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHistoryInfoHdr,&dLocalErr);
			return Sdf_co_fail;        
        
    }
    
    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
            "sdf_fn_uaAddHistoryInfoHeader:");
    return Sdf_co_success;
}
#endif
