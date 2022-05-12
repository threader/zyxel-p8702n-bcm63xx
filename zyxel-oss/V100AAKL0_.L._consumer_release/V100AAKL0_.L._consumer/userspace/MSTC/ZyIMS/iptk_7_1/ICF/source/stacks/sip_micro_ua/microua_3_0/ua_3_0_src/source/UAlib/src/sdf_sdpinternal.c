/******************************************************************************
 ** FUNCTION:		Has all the internal functions of the SDP handling 
 ** 				within the UA Toolkit.
 **
 ******************************************************************************
 **
 ** FILENAME: 		sdf_sdpinternal.c
 **
 ** DESCRIPTION: 	Has all the internal functions of the SDP handling within 
 ** 				the UA Toolkit.
 **
 ** DATE     	 NAME          	REFERENCE     	REASON
 ** ----   	   	 ----           ---------      	------
 ** 25-Feb-01	C. Rajasri,						Initial
 **				Siddharth
 **
 ** 05-Jul-01  	Siddharth						Modified for changes
 **												needed for overlapping
 **												transactions handling
 ** 21 Nov  02		Elias/Srikanth				  SDP Revamp
 ** 
 ******************************************************************************
 ** 			COPYRIGHT , Aricent, 2006
 ******************************************************************************/


#include <ctype.h>

#include "ALheader.h"
#include "sdf_common.h"
#include "sdf_struct.h"
#include "sdf_internal.h"
#include "sdf_reqresp.h"
#include "sdf_debug.h"
#include "sdf_sdp.h"
#include "sdf_init.h"
#include "sdf_sdpinit.h"
#include "sdf_accessor.h"
#include "sdf_sdpaccessor.h"
#include "sdf_network.h"
#include "sdf_callbacks.h"
#include "sdf_sdpinternal.h"
#include "sdf_trace.h"
#include "sdf.h"
#ifndef SDF_LINT
#include "sdf_sdpstruct.h"
#include "sdf_free.h"
#include "sdf_portlayer.h"
#include "sdf_list.h"
#include "sdf_sdpfree.h"
#endif
#if 0
#include "microsip_msgbody.h"
#endif

/*
 * Table mapping payload types to their registered IANA names
 */
/*
 * Table mapping codec Numbers to the corresponding names
 * Its size is mentioned through Sdf_co_codecTableSize.
 * If any updations in this table should get reflected in Sdf_co_codecTableSize
 * value
 */
Sdf_st_avtProfileMap sdf_gl_dCodecProfile[] = {
	{"PCMU/8000/1",		Sdf_en_PCMU			},
	{"1016/8000/1",		Sdf_en_1016			},
	{"G726-32/8000/1",	Sdf_en_G726_32		},
	{"GSM/8000/1",		Sdf_en_GSM			},
	{"G723/8000/1",		Sdf_en_G723			},
	{"DVI4/8000/1",		Sdf_en_DVI4			},
	{"DVI4/16000/1",	Sdf_en_DVI4_16		},
	{"LPC/8000/1",		Sdf_en_LPC			},
	{"PCMA/8000/1",		Sdf_en_PCMA			},
	{"G722/8000/1",		Sdf_en_G722			},
	{"L16/44100/2",		Sdf_en_L16_2		},
	{"L16/44100/1",		Sdf_en_L16			},
	{"QCELP/8000/1",	Sdf_en_QCELP		},
	{"CN/8000/1",		Sdf_en_CN			},
	{"MPA/90000/1",		Sdf_en_MPA			},
	{"G728/8000/1",		Sdf_en_G728			},
	{"DVI4/11025/1",	Sdf_en_DVI4_11		},
	{"DVI4/22050/1",	Sdf_en_DVI4_22		},
#ifdef SDF_UAB2B
    {"G729/8000/1",		Sdf_en_G729			},    
    /*TODO - SPR 3851*/
	{"unassigned",	    Sdf_en_unassigned	},
	{"unassigned",		Sdf_en_unassigned	},
	{"unassigned",		Sdf_en_unassigned	},
	{"unassigned",		Sdf_en_unassigned	},
	{"unassigned",		Sdf_en_unassigned	},
	{"unassigned",		Sdf_en_unassigned	},
	{"CelB/90000",		Sdf_en_CelB			},
	{"JPEG/90000",		Sdf_en_JPEG			},
	{"unassigned",		Sdf_en_unassigned	},
	{"unassigned",		Sdf_en_unassigned	},
	{"unassigned",		Sdf_en_unassigned	},
	{"unassigned",		Sdf_en_unassigned	},
	{"H261/90000",		Sdf_en_H261			},
	{"MPV/90000",		Sdf_en_MPV			},
	{"MP2T/90000",		Sdf_en_MP2T			},
	{"H263/90000",		Sdf_en_H263			}
#else
   	{"G729/8000/1",		Sdf_en_G729			}
#endif
};
	
/*Csm Table for Media State*/
Sdf_ty_mediaState dMediaCsmNxtStateOnMsgSent[Sdf_mc_noOfMessageTypes]\
											[Sdf_mc_noOfMediaStates];
Sdf_ty_mediaState dMediaCsmNxtStateOnMsgRecv[Sdf_mc_noOfMessageTypes]\
											[Sdf_mc_noOfMediaStates];
/*Media State Validation Funciton Pointer table*/
Sdf_ty_fptrMediaStateValidationFunc	dGlbMediaStateValidationTable\
												[Sdf_mc_noOfMediaStates];
/*Array Of Strings Specifying the Groups to be Supported by UATK.
 * By Default FID and LS will be supported.If application wants
 * to support anything else,it needs to add the Group name here
 * and increment the Macro Sdf_mc_noOfExtraGroups
 */
/*
Sdf_ty_s8bit*	dOtherSupportedGroups[]={};
*/

/***************************************************************************
** FUNCTION: sdp_insertBandwidthAtIndex
**
** DESCRIPTION: This function inserts a pBandwidth field at a specified
**		index in an SDP message structure
**
****************************************************************************/
SipBool sdp_insertBandwidthAtIndex_level1
#ifdef ANSI_PROTO
	( SdpMessage *msg, SIP_S8bit *pBandwidth, SIP_U32bit cnt, SipError *err )
#else
	( msg,pBandwidth,cnt,err)
	  SdpMessage *msg;
	  SIP_S8bit *pBandwidth;
	  SIP_U32bit cnt;
	  SipError *err;
#endif
{
	return sdp_insertBandwidthAtIndex(msg,pBandwidth,cnt,err);
}	

SipBool sdp_insertPhoneAtIndex_level1
#ifdef ANSI_PROTO
	( SdpMessage *msg, SIP_S8bit *slPhone, SIP_U32bit cnt, SipError *err )
#else
	( msg,slPhone,cnt,err)
	  SdpMessage *msg;
	  SIP_S8bit *slPhone;
	  SIP_U32bit cnt;
	  SipError *err;
#endif
{
	return sdp_insertPhoneAtIndex(msg,slPhone,cnt,err);
}	

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaMapPayloadFromCodec
 **
 ** DESCRIPTION: 	This API takes in the codec in string form (Eg. GSM/8000) 
 **					and returns the payload associated with the codec(Eg. 3).
 **
 *****************************************************************************/
Sdf_ty_s8bit *sdf_fn_uaMapPayloadFromCodec
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *pCodec,
	Sdf_st_error *pError)
#else
	(pCodec, pError)
	Sdf_ty_s8bit *pCodec;
	Sdf_st_error *pError;
#endif	
{
	Sdf_ty_u32bit dIndex=0, dLength = 0;
	Sdf_ty_s8bit  dTemp[Sdf_co_characterBufferSize];
	Sdf_ty_s8bit  *pTempCodec= Sdf_co_null;
	/*
	 * To remove compiler warning
	 */
	(void)pError;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_fn_uaMapPayloadFromCodec");
	
	/* 
	 * Parameter checks
	 */
#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError( (Sdf_ty_s8bit*) "SDF_ERROR-\
		sdf_fn_uaMapPayloadFromCodec(): \
		Invalid pError Param Passed");
#endif		
		return Sdf_co_null;
	}
	if(pCodec==Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaMapPayloadFromCodec(): "
			"Invalid pCodec Param Passed",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_null;
	}
#endif
	/*
	 * Try to find the format (payload) in the table mapping codecs 
	 * to formats.
	 */
	for (dIndex = 0; dIndex < Sdf_co_codecTableSize; dIndex++)
	{
		if (Sdf_mc_strstr(sdf_gl_dCodecProfile[dIndex].dCodecString, \
						pCodec) != Sdf_co_null)
		{
			(void)sdf_fn_uaSprintf(dTemp, "%d", dIndex);
			/*
			 * Match found. Return here.
			 */
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
				"Exiting sdf_fn_uaMapPayloadFromCodec");
			return Sdf_mc_strdupSdp(dTemp);
		}
	}
	/* If codec match failed, then repeat the same by converting
	 * the codec name to upper letters */
	dLength = Sdf_mc_strlen(pCodec);

	pTempCodec = (Sdf_ty_s8bit  *)sdf_memget(0, dLength + 1, pError);
	if(pTempCodec == Sdf_co_null)
	{
		pError->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaMapPayloadFromCodec(): "
			"Failed to allocate memory",pError);
#endif		
		return Sdf_co_null;
	}
	
	for (dIndex = 0; dIndex < dLength ; dIndex++)
			pTempCodec[dIndex] = Sdf_mc_toupper(pCodec[dIndex]);
    pTempCodec[dIndex]='\0'; 
	for (dIndex = 0; dIndex < Sdf_co_codecTableSize; dIndex++)
	{
		if (Sdf_mc_strstr(sdf_gl_dCodecProfile[dIndex].dCodecString, \
					pTempCodec) != Sdf_co_null)
		{
			(void)sdf_fn_uaSprintf(dTemp, "%d", dIndex);
			(void)sdf_memfree(0,(Sdf_ty_pvoid *)&pTempCodec,pError);
			/*
			 * Match found. Return here.
			 */
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
				"Exiting sdf_fn_uaMapPayloadFromCodec");
			return Sdf_mc_strdupSdp(dTemp);
		}
	}
	(void)sdf_memfree(0,(Sdf_ty_pvoid *)&pTempCodec,pError);
	/*
	 * No match found. Return empty string.
	 */
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exiting sdf_fn_uaMapPayloadFromCodec");

	return Sdf_co_null;
}


/*****************************************************************************
 ** FUNCTION: sdf_fn_uaMapCodecFromPayload
 **
 ** DESCRIPTION: This API takes in the payload in string form (Eg. 0)
 **				 and returns the codec associated with the payload 
 **				 (Eg. PCMU/8000).
 **
 *****************************************************************************/
Sdf_ty_s8bit *sdf_fn_uaMapCodecFromPayload
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *pPayload,
	Sdf_st_error *pError)
#else
	(pPayload,pError)
	Sdf_ty_s8bit *pPayload;
	Sdf_st_error *pError;
#endif	
{
	Sdf_ty_u32bit	dIndex=0;
	Sdf_ty_s8bit	*pCodecString = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_fn_uaMapCodecFromPayload");

	/* 
	 * Parameter checks
	 */
#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError( (Sdf_ty_s8bit*) "SDF_ERROR-\
		sdf_fn_uaMapCodecFromPayload(): \
		Invalid pError Param Passed");
#endif		
		return Sdf_co_null;
	}
	if(Sdf_co_null==pPayload)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaMapCodecFromPayload(): "
			"Invalid pPayload Param Passed",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_null;
	}
#endif

	dIndex = Sdf_mc_atoi(pPayload);
	
	if( dIndex >= Sdf_co_codecTableSize )
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaMapCodecFromPayload(): "
			"Invalid pPayload Param Passed",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_null;
	}

	pCodecString=Sdf_mc_strdupSdp(sdf_gl_dCodecProfile[dIndex].dCodecString);
#ifdef SDF_UAB2B
    /*TODO SPR 3851*/
    if ( 0 == Sdf_mc_strcmp(pCodecString,"unassigned"))
    {
#ifdef SDF_ERROR		
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit*)"sdf_fn_uaMapCodecFromPayload(): "
                "Invalid pPayload Param Passed, Codec is not assigned",pError);
#endif		
        pError->errCode=Sdf_en_invalidParamError;
        return Sdf_co_null;
    }
#endif
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exiting sdf_fn_uaMapCodecFromPayload");

	return pCodecString;
}


/******************************************************************************
 ** FUNCTION: 		sdf_fn_uaCloneSipMsgBodyList
 **
 ** DESCRIPTION:  	This function clones a list of Message bodies
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCloneSipMsgBodyList
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
		"Entering sdf_fn_uaCloneSipMsgBodyList");

	/* 
	 * Parameter checks
	 */
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError( (Sdf_ty_s8bit*) "SDF_ERROR - "
			"sdf_fn_uaCloneSipMsgBodyList(): Invalid pError Param Passed");
#endif
		return Sdf_co_fail;
	}
	if(Sdf_co_null==pDest)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaCloneSipMsgBodyList(): "
			"Invalid pDest Param Passed",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(Sdf_co_null==pSrc)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaCloneSipMsgBodyList(): "
			"Invalid pSrc Param Passed",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
	 * Delete everything from the destination list before proceeding.
	 */
	if (sdf_listDeleteAll(pDest ,pErr) == Sdf_co_fail )
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaCloneSipMsgBodyList( ):  "
			"Failed to delete destination message bodies before cloning", \
			pErr);
#endif		
		return Sdf_co_fail;
	}

	/*
	 * Get each element of the source list, clone it and insert into the
	 * destination list.
	 */
	(void)sdf_listInitIterator(pSrc, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SipMsgBody *pTemp,*pClone;

		pTemp = (SipMsgBody*)\
			(dListIterator.pCurrentElement->pData);

		if (sip_initSipMsgBody(&pClone, SipBodyAny, (SipError*)&(\
			pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaCloneSipMsgBodyList( ): "
			 	"Failed to initialize a SdpMedia structure",pErr);
#endif			 
			return Sdf_co_fail;
		}
		if (sip_cloneSipMsgBody(pClone, pTemp, (SipError*)&(\
			pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaCloneSipMsgBodyList( ): "
				"Failed to clone the SdpMedia structure",pErr);
#endif			
			sip_freeSipMsgBody(pClone);
			return Sdf_co_fail;
		}
		if(sdf_listAppend(pDest,pClone,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit*) "sdf_fn_uaCloneSipMsgBodyList( ): "
		 		"Failed to add the cloned structure to the destination "
				"list of medias",pErr);
#endif			
			sip_freeSipMsgBody(pClone);
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - " 
		"Exiting sdf_fn_uaCloneSipMsgBodyList");

	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaDeleteSdpFromMsgBodyList
 **
 ** DESCRIPTION: 	Finds and deletes the SDP msgbody from the msgbodylist
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaDeleteSdpFromMsgBodyList
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pMsgBodyList,
	Sdf_st_error *pError)
#else	
	(pMsgBodyList, pError)
	Sdf_ty_slist *pMsgBodyList;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_u32bit i=0, dCount=0;
	en_SipMsgBodyType dType = SipBodyAny;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - " 
		"Entering sdf_fn_uaDeleteSdpFromMsgBodyList");

	/* 
	 * Parameter checks
	 */
#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError( (Sdf_ty_s8bit*) "SDF_ERROR-\
		sdf_fn_uaDeleteSdpFromMsgBodyList():  \
		Invalid pError Param Passed");
#endif		
		return Sdf_co_fail;
	}
	if (pMsgBodyList== Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaDeleteSdpFromMsgBodyList():  "
			"Invalid MsgBody List to be operated on.",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif


	/*
	 * Start performing tasks on the operating list.
	 */
	(void)sdf_listSizeOf(pMsgBodyList, &dCount, pError);

	for (i = 0; i < dCount; i++)
	{
		SipMsgBody *pMsgBody= Sdf_co_null;

		if (sdf_listGetAt(pMsgBodyList, i, (SIP_Pvoid*)&pMsgBody, \
			pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaDeleteSdpFromMsgBodyList():  "
				"Failed to get MsgBody from MsgBody List",pError);
#endif			
			return Sdf_co_fail;
		}
		if (sip_getMsgBodyType(pMsgBody, &dType, (SipError*)\
			&(pError->stkErrCode))  == SipFail)
		{
			pError->errCode = pError->stkErrCode ;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaDeleteSdpFromMsgBodyList():  "
				"Failed to get message body type ",pError);
#endif		
			return Sdf_co_fail;
		}
		if (dType == SipSdpBody)
		{
			/*
			 * Found a SDP body. Delete it now and return (At most one 
			 * SDP can be present in the list)
			 */
			if (sdf_listDeleteAt(pMsgBodyList, i, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit*)"sdf_fn_uaDeleteSdpFromMsgBodyList():  "
					"Failed to delete message body list ",pError);
#endif				
				return Sdf_co_fail;
			}
			break;
		}
	}
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - " 
		"Exiting sdf_fn_uaDeleteSdpFromMsgBodyList");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaModifyConnectionInSdpMessage
 **
 ** DESCRIPTION: 	This function adds the "c=" line to the	SdpMessage 
 **					that is taken as input. If a c= line is already present
 **					in the SdpMessage taken, it is left untouched.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaModifyConnectionInSdpMessage
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData, 
	SdpMessage *pNegotiatedSdpMessage,
	Sdf_st_error *pError)
#else
	(pInitData, pNegotiatedSdpMessage pError)
	Sdf_st_initData *pInitData;
	SdpMessage *pNegotiatedSdpMessage;
	Sdf_st_error *pError;
#endif
{
	SdpConnection *pConnection = Sdf_co_null;
	Sdf_ty_s8bit	*pTmpVal = Sdf_co_null;
	Sdf_ty_retVal insertConnectionLine = Sdf_co_fail;	
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_fn_uaModifyConnectionInSdpMessage ");

	/* 
	 * Parameter checks
	 */
#ifdef SDF_PARAMVALIDATION
	if(pError==Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError((Sdf_ty_s8bit*) "SDF_ERROR - "
			"sdf_fn_uaModifyConnectionInSdpMessage(): "
			"Invalid pError Param Passed");
#endif					
		return Sdf_co_fail;
	}
#endif

	/* 
	 * Add a new SdpConnection only if one does not exist already
	 */
	if ((sdp_getConnection(pNegotiatedSdpMessage, &pConnection, \
		(SipError*)&(pError->stkErrCode)))==SipFail)
	{
		Sdf_st_transportInfo *pTransport;

		if (pError->stkErrCode == Sdf_en_noExistError)
		{
			Sdf_ty_u32bit i,cnt = 0,connectionLines;
			SdpMedia *pTmpMedia = Sdf_co_null;
		
			(void)sdp_getMediaCount(pNegotiatedSdpMessage,&cnt,(SipError*)&(pError->stkErrCode));

			if(cnt == 0)
				insertConnectionLine = Sdf_co_success;
		
			for(i=0;i<cnt;i++)
			{
				(void)sdp_getMediaAtIndex(pNegotiatedSdpMessage,&pTmpMedia, \
						i,(SipError*)&(pError->stkErrCode));
				if(sdp_getConnectionCountFromMedia(pTmpMedia,&connectionLines,(SipError*)&(pError->stkErrCode))
							== SipFail)
				{
					insertConnectionLine = Sdf_co_success;
					sip_freeSdpMedia(pTmpMedia);	
					break;
				}
				else if(connectionLines==0)
				{
					insertConnectionLine = Sdf_co_success;
					sip_freeSdpMedia(pTmpMedia);	
					break;
				}
				
				sip_freeSdpMedia(pTmpMedia);	
			}		
			
			if (insertConnectionLine == Sdf_co_success)
			{

			/*
			 * A c= line is missing. Add one now.
			 */
			if(sip_initSdpConnection(&pConnection, \
					(SipError*)&(pError->stkErrCode)) == SipFail)
					return Sdf_co_fail;
			/*
			 * set net type
			 */
			pTmpVal = Sdf_mc_strdupSdp(Sdf_mc_networkType);
			if ((sdp_setNetTypeInConnection(pConnection, pTmpVal, \
				(SipError*)&(pError->stkErrCode)))==SipFail)
			{
				pError->errCode = pError->stkErrCode ;
				sip_freeSdpConnection(pConnection);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit*)"sdf_fn_uaModifyConnectionInSdpMessage(): "
					"Failed to set net type in connection structure "
					"from SDP", pError);
#endif					
				return Sdf_co_fail;
			}
			
			if (sdf_ivk_uaGetTransportInfoFromInitData(pInitData, \
				&pTransport, pError) == Sdf_co_fail)
			{
				sip_freeSdpConnection(pConnection);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
					(Sdf_ty_s8bit*)"sdf_fn_uaModifyConnectionInSdpMessage (): "
					"Failed to set get transportInfo from the initData", \
					pError);
#endif				
				return Sdf_co_fail;
			}

			/*
			 * set address type
			 */
			pTmpVal = Sdf_co_null;
			if(Sdf_mc_strstr(pTransport->pIp,":") !=Sdf_co_null)
				pTmpVal = Sdf_mc_strdupSdp("IP6");
			else
				pTmpVal = Sdf_mc_strdupSdp("IP4");

			if ((sdp_setAddrTypeInConnection(pConnection, pTmpVal, \
				(SipError*)&(pError->stkErrCode)))==SipFail)
			{
				(void)sdf_ivk_uaFreeTransportInfo(pTransport);
				pError->errCode = pError->stkErrCode ;
				sip_freeSdpConnection(pConnection);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit*)"sdf_fn_uaModifyConnectionInSdpMessage(): "
					"Failed to set addr type in connection structure from "
					"SDP", pError);
#endif					
				return Sdf_co_fail;
			}

			/*
			 * set IP address
			 */
			
			pTmpVal = Sdf_co_null;
			pTmpVal = Sdf_mc_strdupSdp(pTransport->pIp);

			if ((sdp_setAddrInConnection(pConnection, pTmpVal, \
				(SipError*)&(pError->stkErrCode))) ==SipFail)
			{
				pError->errCode = pError->stkErrCode ;
				sip_freeSdpConnection(pConnection);
				(void)sdf_ivk_uaFreeTransportInfo(pTransport);
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					(Sdf_ty_s8bit*)"sdf_fn_uaModifyConnectionInSdpMessage(): "
					"Failed to set addr in connection structure from SDP", \
					pError);
#endif				
				return Sdf_co_fail;
			}
			(void)sdf_ivk_uaFreeTransportInfo(pTransport);

			/*
			 * set SDP connection in the SDP message now.
			 */
			if(sdp_setConnection(pNegotiatedSdpMessage, \
				pConnection,(SipError*)&(pError->stkErrCode)) == SipFail)
            {
               sip_freeSdpConnection(pConnection);
			   return Sdf_co_fail;
            }

			/* 
			 * Free local variable 
			 */
			sip_freeSdpConnection(pConnection);
		}
		}
		else
		{
			/*
			 * sdp_getConnection failed for reaason other than c= line not
			 * existing. Return failure in this case.
			 */
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyConnectionInSdpMessage(): "
				"Failed to get SdpConnection from the SdpMessage",pError);
#endif			
			return Sdf_co_fail;
		}
	}
	else
	{
		/* 
		 * c= line already exists. Let it remain 
		 */
		sip_freeSdpConnection(pConnection);
	}

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exiting sdf_fn_uaModifyConnectionInSdpMessage ");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaModifyOriginInSdpMessage
 **
 ** DESCRIPTION: 	This function adds an "o=" line to the SdpMessage 
 **					that was passed from own details in the initdata. If 
 **					an o= line was present already, it is left untouched.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaModifyOriginInSdpMessage
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData, 
	SdpMessage *pNegotiatedSdpMessage,
	Sdf_st_error *pError)
#else
	(pInitData, pNegotiatedSdpMessage, pTransport, pError)
	Sdf_st_initData *pInitData;
	SdpMessage *pNegotiatedSdpMessage;
	Sdf_st_error *pError;
#endif	
{
	SdpOrigin *pOrigin = Sdf_co_null;
	Sdf_ty_s8bit *pTempDisplayName = Sdf_co_null, dTemp[Sdf_co_payloadSize];
	Sdf_ty_s8bit *dTimeStamp = Sdf_co_null, *pDisplayName = Sdf_co_null,\
		*pStr = Sdf_co_null;
	Sdf_ty_u32bit dTime=0;
	Sdf_ty_s8bit	*pTmpVal = Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_fn_uaModifyOriginInSdpMessage ");

	/* 
	 * Parameter checks
	 */
#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError( (Sdf_ty_s8bit*) "SDF_ERROR - "
			"sdf_fn_uaModifyOriginInSdpMessage (): "
			"Invalid pError Param Passed");
#endif		
		return Sdf_co_fail;
	}
	if (pNegotiatedSdpMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage():  "
			"Invalid SdpMessage param passed", pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
	
	/* 
	 * Add a new SDP Origin only if one does not exist already 
	 */
	if (sdp_getOrigin(pNegotiatedSdpMessage, &pOrigin, (SipError*)\
			&(pError->stkErrCode)) == SipFail)
	{
		Sdf_st_transportInfo *pTransport;
		SipHeader *pFrom = pInitData->pFrom;

		pError->errCode = pError->stkErrCode ;
		if (pError->errCode != Sdf_en_noExistError)
		{
			/*
			 * sdp_getOrigin API failed for reason other than non-existence
			 * of a SdpOrigin. Return failure.
			 */
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
				"Failed to get SDP Origin from SDP Message",pError);
#endif			
			return Sdf_co_fail;
		}

		/*
		 * SdpOrigin does not exist. Add it here.
		 */
		if(sip_initSdpOrigin(&pOrigin,	(SipError*)&(pError->stkErrCode)) ==
						SipFail)
				return Sdf_co_fail;

		/*
		 * Fill user name from the display name part of the From header
		 */
		if(sip_getDispNameFromFromHdr(pFrom, &pTempDisplayName, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{
				sip_freeSdpOrigin(pOrigin);
				return Sdf_co_fail;
		}
		 

		if (pTempDisplayName != Sdf_co_null)
			pDisplayName = Sdf_mc_strtokr(pTempDisplayName, " ",&pStr);
		else
		{
			pDisplayName = (Sdf_ty_s8bit *)"anonymous";
		}
		pTmpVal = Sdf_mc_strdupSdp(pDisplayName);
		if (sdp_setUserInOrigin(pOrigin, pTmpVal, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode ;
			sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
				"Failed to set User in Origin Structure",pError);
#endif			
			return Sdf_co_fail;
		}

		/*
		 * Fill session in the o= line using current time.
		 */
		/*w534*/(void)sdf_fn_uaGetCurrentTimeInMicroSecs(&dTime);
		(void)sdf_fn_uaSprintf(dTemp, "%lu",&dTime);
		dTimeStamp = (Sdf_ty_s8bit *) \
		sdf_memget (Sdf_mc_callHandlerMemId, Sdf_mc_strlen(dTemp)+1, pError);
		if(Sdf_co_null==dTimeStamp)
		{
			sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
				"Failed to allocate memory",pError);
#endif			
			return Sdf_co_fail;
		}
		Sdf_mc_strcpy(dTimeStamp, dTemp);
		
		if (sdp_setSessionIdInOrigin(pOrigin, dTimeStamp ,\
			(SipError*) &(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode ;
			sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
				"Failed to set session id in Origin Structure",pError);
#endif			
			return Sdf_co_fail;
		}

		/*
		 * Set version in the o= line similarly
		 */
		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupSdp(dTimeStamp);

		if (sdp_setVersionInOrigin(pOrigin, pTmpVal, \
			(SipError*) &(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode ;
			sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
				"Failed to set version in Origin Structure",pError);
#endif			
			return Sdf_co_fail;
		}	

		/*
		 * set net type
		 */
		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupSdp(Sdf_mc_networkType);

		if (sdp_setNetTypeInOrigin(pOrigin, pTmpVal, \
			(SipError*) &(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode ;
			sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
				"Failed to set net type in Origin Structure",pError);
#endif			
			return Sdf_co_fail;
		}

		if (sdf_ivk_uaGetTransportInfoFromInitData(pInitData, \
			&pTransport, pError) == Sdf_co_fail)
		{
			sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
				"Failed to set get transportInfo from the initData",pError);
#endif			
			return Sdf_co_fail;
		}

		/*
		 * set addr type
		 */
		pTmpVal = Sdf_co_null;
		if(Sdf_mc_strstr(pTransport->pIp,":") !=Sdf_co_null)
			pTmpVal = Sdf_mc_strdupSdp("IP6");
		else
			pTmpVal = Sdf_mc_strdupSdp("IP4");


		if (sdp_setAddrTypeInOrigin(pOrigin, pTmpVal,\
			(SipError*) &(pError->stkErrCode)) == SipFail)
		{
			(void)sdf_ivk_uaFreeTransportInfo(pTransport);
			pError->errCode = pError->stkErrCode ;
			sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
				"Failed to set Addr type in Origin Structure",pError);
#endif			
			return Sdf_co_fail;
		}

		/*
		 * set IP address in the o= line.
		 */
		
		pTmpVal = Sdf_co_null;
		pTmpVal = Sdf_mc_strdupSdp(pTransport->pIp);

		if (sdp_setAddrInOrigin(pOrigin, pTmpVal, \
			(SipError*)&(pError->stkErrCode))== SipFail)
		{
			pError->errCode = pError->stkErrCode ;
			sip_freeSdpOrigin(pOrigin);
			(void)sdf_ivk_uaFreeTransportInfo(pTransport);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
				"Failed to set Addr in Origin Structure",pError);
#endif			
			return Sdf_co_fail;
		}
		
		/*
		 * set SdpOrigin in the SdpMessage.
		 */
		if (sdp_setOrigin(pNegotiatedSdpMessage, pOrigin, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = pError->stkErrCode ;
			sip_freeSdpOrigin(pOrigin);
			(void)sdf_ivk_uaFreeTransportInfo(pTransport);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
				"Failed to set SdpOrigin in SdpMessage",pError);
#endif			
			return Sdf_co_fail;
		}
		
		(void)sdf_ivk_uaFreeTransportInfo(pTransport);
	}
	/* 
	 * Free the local reference.
	 */
	sip_freeSdpOrigin(pOrigin);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exiting sdf_fn_uaModifyOriginInSdpMessage ");
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}



/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetSdpMessageFromDefaults
 **
 ** DESCRIPTION: 	This function gets the SDP message stored in the 
 **					initdata that is passed. The SDP message returned 
 **					has its ref count incremented & so it would have to 
 **					be freed after use.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetSdpMessageFromDefaults
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData, SdpMessage **ppSdpMessage, \
	Sdf_st_error *pErr)
#else
	(pInitData, ppSdpMessage, pErr)
	Sdf_st_initData *pInitData;
	SdpMessage **ppSdpMessage;
	Sdf_st_error *pErr;
#endif
{
	SipMsgBody *pMsgBody = Sdf_co_null;
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_fn_uaGetSdpMessageFromDefaults ");

	/* 
	 * Parameter checks
	 */
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError((Sdf_ty_s8bit*) "SDF_ERROR - "
			"sdf_fn_uaGetSdpMessageFromDefaults (): "
			"Invalid pError Param Passed");
#endif		
		return Sdf_co_fail;
	}
#endif

	*ppSdpMessage=Sdf_co_null;

	(void)sdf_listInitIterator(&(pInitData->slMsgBodyList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		pMsgBody = (SipMsgBody*)\
			(dListIterator.pCurrentElement->pData);

		/*
		 * Continue to iterate if the message body does not have a SDP Message
		 */
		if (pMsgBody->dType != SipSdpBody)
		{
			(void)sdf_listNext(&dListIterator, pErr);
			continue;
		}

		/* 
		 * Ref count for SdpMessage will increase here. Free it later 
		 * after use (in the code from where this API is invoked)
		 */
		if (sip_getSdpFromMsgBody(pMsgBody, ppSdpMessage, \
			(SipError*) &(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaGetSdpMessageFromDefaults(): "
				"Failed to get SDP from Message Body ",pErr);
#endif			
			return Sdf_co_fail;
		}
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
			"Exitting sdf_fn_uaGetSdpMessageFromDefaults ");
		return Sdf_co_success;
	}

	/*
	 * Did ot find any SDP message at all. Return failure.
	 */
	pErr->errCode = Sdf_en_noExistError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exitting sdf_fn_uaGetSdpMessageFromDefaults ");
	return Sdf_co_fail;
}




#ifdef SDF_TRACE
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaTraceSdpMessageInSessionParam
 **
 ** DESCRIPTION: 	This function converts the Session Param Structure to
 **					SdpMessage structure & calls sdf_fn_uaTraceMediaLines
 **					which prints the contents in string format.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaTraceSdpMessageInSessionParam
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, Sdf_st_sessionParam *pNgtdSession, \
	 Sdf_ty_s8bit *pStr, Sdf_st_error *pErr)
#else
	(pObject, pSdpMsg, pStr)
	Sdf_st_callObject *pObject;
	Sdf_st_sessionParam *pNgtdSession;
	Sdf_ty_s8bit *pStr;
#endif
{
	SdpMessage *pSdpMsg=Sdf_co_null;
	if (sip_initSdpMessage(&pSdpMsg, (SipError *)&(pErr->stkErrCode)) == \
			SipFail)
	{
		return Sdf_co_fail;
	}
	if (sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMsg, pNgtdSession,
				Sdf_en_convertLocal, pErr) == Sdf_co_fail)
	{
		sip_freeSdpMessage(pSdpMsg);
		return Sdf_co_fail;
	}
	(void)sdf_fn_uaTraceMediaLines(pObject, pSdpMsg, pStr);
	sip_freeSdpMessage(pSdpMsg);
	
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaTraceMediaLines
 **
 ** DESCRIPTION: 	This function displays all the m= lines that are present 
 **					in the SdpMessage being passed. The parameter pStr should 
 **					be one of "incoming" or "negotiated".
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaTraceMediaLines
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, SdpMessage *pSdpMsg,\
	Sdf_ty_s8bit *pStr)
#else
	(pObject, pSdpMsg, pStr)
	Sdf_st_callObject *pObject;
	SdpMessage *pSdpMsg;
	Sdf_ty_s8bit *pStr;
#endif
{
	Sdf_ty_s8bit temp[Sdf_co_smallBufferSize],
				 trace[Sdf_co_traceBufferSize];
	Sdf_ty_u32bit countOfMediaLines=0, i=0, countOfAttrs=0;
	Sdf_st_error dError;

	(void)sip_listSizeOf(&(pSdpMsg->slMedia), &countOfMediaLines, \
		(SipError *)&(dError.stkErrCode));

	(void)pObject;

	if (countOfMediaLines == 0)
		return Sdf_co_success;
	
	/*sdf_fn_uaSprintf(temp, "Call: %s - %s SDP Media line at index ", \
		pObject->pCommonInfo->pKey->pCallId, pStr);*/
	
	(void)sdf_fn_uaSprintf(temp, "%s SDP Media line at index ", pStr);
	
	for (i=0; i<countOfMediaLines; i++)
	{
		SdpMedia *pMedia = Sdf_co_null;
		SdpAttr *pAttr;
		Sdf_ty_s8bit *pMvalue = Sdf_co_null , *pProto, *pFormat;
		Sdf_ty_u16bit dPort;
		Sdf_ty_u32bit dNumport, loop;

		if ((sdp_getMediaAtIndex(pSdpMsg, &pMedia, i, \
			(SipError *)&(dError.stkErrCode))) == SipFail)
			return Sdf_co_fail;

		if ((sdp_getMvalueFromMedia(pMedia, &pMvalue, \
			(SipError *)&(dError.stkErrCode))) == SipFail)
			return Sdf_co_fail;
		if (sdp_getPortFromMedia(pMedia, &dPort, \
			(SipError*)&(dError.stkErrCode)) == SipFail)
			return Sdf_co_fail;
		if (sdp_getNumportFromMedia(pMedia, &dNumport, \
			(SipError*)&(dError.stkErrCode)) == SipFail)
			dNumport = 0;
		if ((sdp_getProtoFromMedia(pMedia, &pProto, \
			(SipError *)&(dError.stkErrCode))) == SipFail)
			return Sdf_co_fail;
		if (sdp_getFormatFromMedia(pMedia, &pFormat, \
			(SipError*)&(dError.stkErrCode)) == SipFail)
			return Sdf_co_fail;

		if (dNumport != 0)
		{
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
				"%s %d:\n\tm= %s %d/%d %s %s\n", temp, i,\
				pMvalue, dPort, dNumport, pProto, pFormat);
		}
		else
		{
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
				"%s %d:\n\tm= %s %d %s %s\n",  temp, i,\
				pMvalue, dPort, pProto, pFormat);
		}

		sdf_fn_trace(Sdf_en_allTraceLevels, \
			Sdf_en_mediaTraces, trace, 0, &dError);

		if(sdp_getAttrCountFromMedia(pMedia, &countOfAttrs, \
				(SipError*)&(dError.stkErrCode))==SipFail)
				return Sdf_co_fail;
		
		if (countOfAttrs != 0)
		{
			sdf_fn_trace(Sdf_en_allTraceLevels, \
				Sdf_en_mediaTraces, (Sdf_ty_s8bit*)"\tAssociated attribute "
				"lines: \n", 0, &dError);
		}

		sip_freeSdpMedia(pMedia);	

		for (loop = 0; loop < countOfAttrs; loop++)
		{
			Sdf_ty_s8bit *pAttrValue, *pAttrName = Sdf_co_null;

			if (sdp_getAttrAtIndexFromMedia(pMedia, &pAttr,\
				loop, (SipError*)&(dError.stkErrCode)) == SipFail)
				return Sdf_co_fail;

			if ((sdp_getNameFromAttr(pAttr, &pAttrName, \
				(SipError*)&(dError.stkErrCode))) == SipFail)
				return Sdf_co_fail;
			Sdf_mc_strcpy(trace, "\ta=");
			Sdf_mc_strcat(trace, pAttrName);
			
			if (sdp_getValueFromAttr(pAttr, &pAttrValue, \
				(SipError*)&(dError.stkErrCode)) == SipFail)
			{
				dError.errCode = dError.stkErrCode ;
				if (dError.errCode != Sdf_en_noExistError)
				{
					sip_freeSdpAttr(pAttr);
					return Sdf_co_fail;
				}
				Sdf_mc_strcat(trace, "\n");
				sip_freeSdpAttr(pAttr);
			}
			else
			{
				Sdf_mc_strcat(trace, ": ");
				Sdf_mc_strcat(trace, pAttrValue);
				Sdf_mc_strcat(trace, "\n");
				sip_freeSdpAttr(pAttr);
			}
			sdf_fn_trace(Sdf_en_allTraceLevels, \
				Sdf_en_mediaTraces, trace, 0, &dError);
		}
	}
	return Sdf_co_success;
}

#endif  /* End of trace ifdef */


/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaCheckAndAddMandatoryLines
 **
 ** DESCRIPTION: 	This function checks whether the SDP message taken as 
 **					input contains the mandatory lines, viz  v,o,c,t,s
 **					If the lines are missing, the same are filled here 
 **					using the initData.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaCheckAndAddMandatoryLines
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData, SdpMessage *pSdpMsg, \
	Sdf_st_error *pError)
#else
	(pInitData, pSdpMsg, pError)
	Sdf_st_initData *pInitData;
	SdpMessage *pSdpMsg;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_u32bit dCount=0;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_fn_uaCheckAndAddMandatoryLines");

	/* Error checks */
#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
		return Sdf_co_fail;
	if (pSdpMsg == Sdf_co_null)
	{
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pInitData == Sdf_co_null)
	{
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
	
	/* 	Set the version always. If it was present,
		the stack API frees that memory. Else its
		set now after this call */
	if (sdp_setVersion(pSdpMsg, Sdf_mc_strdupSdp(Sdf_mc_versionSdp), \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode ;
		return Sdf_co_fail;
	}
	/* Check for the SdpOrigin now. */
	if ((sdf_fn_uaModifyOriginInSdpMessage(pInitData, pSdpMsg, \
		pError)) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}
	/* Check for the SdpConnection now */ 
	if ((sdf_fn_uaModifyConnectionInSdpMessage(pInitData, pSdpMsg, \
		pError)) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}
	/* Check for the SdpTime now */
	(void)sip_listSizeOf(&(pSdpMsg->slTime), &dCount, \
		(SipError *)&(pError->stkErrCode));
	if (dCount == 0)
	{
		SdpTime *pSdpTime;

		if (sip_initSdpTime(&pSdpTime, (SipError *)&(\
			pError->stkErrCode)) == SipFail)
		{
			return Sdf_co_fail;
		}
		if (sdp_setStartInTime(pSdpTime, Sdf_mc_strdupSdp("0"),\
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			return Sdf_co_fail;
		}
		if (sdp_setStopInTime(pSdpTime, Sdf_mc_strdupSdp("0"),\
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			return Sdf_co_fail;
		}
		if (sdp_insertTimeAtIndex(pSdpMsg, pSdpTime, (Sdf_ty_u32bit)0,
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			return Sdf_co_fail;
		}
		sip_freeSdpTime(pSdpTime);
	}
	
	/* check for the SdpSession now */
	if (pSdpMsg->pSession == Sdf_co_null)
	{
		pSdpMsg->pSession = Sdf_mc_strdupSdp("-");
	}
	
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exitting sdf_fn_uaCheckAndAddMandatoryLines");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetContentTypeString
 **
 ** DESCRIPTION: 	This function returns a string for use in the content-type
 **					header. The string is returned depending on the SipMsgBody
 **					dType.
 **
 ** RETURN VALUES:	"application/sdp"		 - if dType is SipSdpBody
 **					"application/isup"		 - if dType is SipIsupBody
 **					<application set string> - if dType is unknown but a content
 **											   type header was present in the 
 **											   SipMsgBody structure.
 **					"text/plain"			 - if dType is SipUnknownBody and
 **											   no Content-Type header in the 
 **											   message body to tell what it is
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetContentTypeString
#ifdef ANSI_PROTO
	(SipMsgBody *pMsgBody, Sdf_ty_s8bit **ppCtypeString, Sdf_st_error *pError)
#else
	(pMsgBody, ppCtypeString, pError)
	SipMsgBody *pMsgBody;
	Sdf_ty_s8bit **ppCtypeString;
	Sdf_st_error *pError;
#endif
{
	en_SipMsgBodyType dType  = SipBodyAny;
#ifdef SDF_LINT
    Sdf_ty_s8bit dTemp[Sdf_co_characterBufferSize] = {'\0',};
#else    
    Sdf_ty_s8bit dTemp[Sdf_co_characterBufferSize];
#endif    
		
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_fn_uaGetContentTypeString");

	*ppCtypeString=Sdf_co_null;	

	if (sip_getMsgBodyType(pMsgBody, &dType, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode ;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetContentTypeString( ): "
			"Failed to get the type of message body at index 0", pError);
#endif			
		return Sdf_co_fail;
	}

	switch (dType)
	{
		case SipSdpBody:
		{
			Sdf_mc_strcpy(dTemp, "application/sdp");
			break;
		}	
		case SipAppSipBody:
		{
			Sdf_mc_strcpy(dTemp, "application/sip");
			break;
		}	
		case SipIsupBody:
		{
			Sdf_mc_strcpy(dTemp, "application/isup");
			break;
		}	
		case SipUnknownBody:
		{
#ifdef SIP_MIME_PARSING
			SipMimeHeader *pMime = Sdf_co_null;
			SipHeader *pContentType = Sdf_co_null;
			Sdf_ty_s8bit *pMediaType = Sdf_co_null;
#endif
			Sdf_ty_retVal contentTypeFound = Sdf_co_fail;
#ifdef SIP_MIME_PARSING
			if (sip_bcpt_getMimeHeaderFromMsgBody(pMsgBody, &pMime, \
				(SipError *)&(pError->stkErrCode)) != SipFail)
			{
				if (sip_initSipHeader(&pContentType, SipHdrTypeAny, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
					pError->errCode = pError->stkErrCode ;
#ifdef SDF_ERROR							
					sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
						(Sdf_ty_s8bit*)"sdf_fn_uaGetContentTypeString( ): "
						"Failed to initialize ContentType header", pError);
#endif							
					return Sdf_co_fail;
				}

				if (sip_bcpt_getContentTypeFromMimeHdr(pMime, \
					&pContentType, (SipError *)&(pError->stkErrCode)) \
					!= SipFail)
				{	
					/* Get the Media Type from the content Type header */
					if (sip_getMediaTypeFromContentTypeHdr(pContentType, \
						&pMediaType, (SipError *)&(pError->stkErrCode)) \
						== SipFail)
					{
						pError->errCode = pError->stkErrCode ;
						sip_bcpt_freeSipMimeHeader(pMime);
						sip_freeSipHeader(pContentType);
						(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid *)&pContentType, Sdf_co_null);
#ifdef SDF_ERROR							
						sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
							(Sdf_ty_s8bit*)"sdf_fn_uaGetContentTypeString( ): "
							"Failed to get media type from content type "
							"header", pError);
#endif							
						return Sdf_co_fail;
					}	
					Sdf_mc_strcpy(dTemp, pMediaType);
					contentTypeFound = Sdf_co_success;
					sip_freeSipHeader(pContentType);
					(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
						(Sdf_ty_pvoid *)&pContentType, Sdf_co_null);
					sip_bcpt_freeSipMimeHeader(pMime);
				}	
				else
				{
					sip_bcpt_freeSipMimeHeader(pMime);
				}
			}
#endif			
			/* If a content type was not present in the msgbody, set it 
				to the default viz. text/plain */
			if (contentTypeFound == Sdf_co_fail)
				Sdf_mc_strcpy(dTemp, "text/plain");
			break;
		}
		default: 
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaGetContentTypeString( ): "
				"Failed to get message body type", pError);
#endif
			return Sdf_co_fail;
		}	
	}
	/*
	 * Fill variable to be returned
	 */
	*ppCtypeString = Sdf_mc_strdupSdp(dTemp);
	
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaGetContentTypeString");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaGetSessionParamForNgtn 	    	
 **
 ** DESCRIPTION: This API returns the session param for the negotiation based 
 **              on the media state
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGetSessionParamForNgtn 
#ifdef ANSI_PROTO
		(Sdf_st_mediaHandling *pMediaInfo,Sdf_st_sessionParam **ppSessionParam, 
		Sdf_st_error *pError)
#else 
		(pMediaInfo,ppSessionParam, pError)
		Sdf_st_mediaHandling *pMediaInfo;
		Sdf_st_sessionParam **ppSessionParam;
		Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null)||(pMediaInfo == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetSessionParamForNgtnFromCallObject( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	if(pMediaInfo->dMediaState == Sdf_en_establishedMedia)
	{
		if(pMediaInfo->pLastOfferMade == Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}
		*ppSessionParam = pMediaInfo->pLastOfferMade;
		HSS_LOCKEDINCREF(pMediaInfo->pLastOfferMade->dRefCount);
	}
	else
	{
		if(pMediaInfo->pCapabilities == Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}
		*ppSessionParam = pMediaInfo->pCapabilities;
		HSS_LOCKEDINCREF(pMediaInfo->pCapabilities->dRefCount);

	}
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaGetSdpMessageFromSipMessage
 **
 ** DESCRIPTION: This API returns the SDP messagefrom the 
 **              the sip message
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGetSdpMessageFromSipMessage 
#ifdef ANSI_PROTO
	(SipMessage *pSipMsg, SdpMessage **ppSdpMsg, 
	Sdf_st_error *pError)
#else
	(pSipMsg, ppSdpMsg, pError)
	SipMessage *pSipMsg;
	SdpMessage **ppSdpMsg; 
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_u32bit i=0;
	SipMsgBody *pMsgBody = Sdf_co_null;
	Sdf_ty_retVal sdpFound = Sdf_co_fail;
	Sdf_ty_u32bit dCount=0; 

#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null)||(pSipMsg == Sdf_co_null))
		
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetSdpMessageFromSipMessage( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

		(void)sip_getMsgBodyCount(pSipMsg, &dCount, \
		(SipError*) &(pError->stkErrCode));

	if (dCount == 0)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetSdpMessageFromSipMessage(): "
			"No SDP in received SIP message.", pError);
#endif		
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}

	/* 	======================================
		  Locate SDP in the message bodies
		====================================== */
	for (i = 0; i < dCount; i++)
	{
		en_SipMsgBodyType dType;
		if (sip_getMsgBodyTypeAtIndex(pSipMsg, \
			&dType, i, (SipError*)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaGetSdpMessageFromSipMessage( ): "
				"Failed to get the type of message body", pError);
#endif			
			pError->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		if (dType == SipSdpBody)
		{
			if (sip_getMsgBodyAtIndex(pSipMsg, \
				&pMsgBody, i, (SipError*)&(pError->stkErrCode)) \
				== SipFail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaGetSdpMessageFromSipMessage( ): "
					"Failed to get the message body from the SipMessage", \
					pError);
#endif				
				pError->errCode=Sdf_en_sdpError;
				return Sdf_co_fail;
			}
			sdpFound = Sdf_co_success;	
			break;	
		}
	}
	if (sdpFound == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetSdpMessageFromSipMessage( ): "
			"No SDP in received SIP message.", pError);
#endif		
		/* free local variables */
	    sip_freeSipMsgBody(pMsgBody);

		pError->errCode=Sdf_en_noSdpError;
		return Sdf_co_fail;
	}

	/* 	=========================================
		  get the "incoming" SdpMessage struct
		========================================= */
	if (sip_getSdpFromMsgBody(pMsgBody, ppSdpMsg, \
		(SipError*)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetSdpMessageFromSipMessage( ): "
			"Failed to get the Sdp Message that was cloned.", pError);
#endif		
	    /* free local variables */
	    sip_freeSipMsgBody(pMsgBody);

		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}
	/* free local variables */
	sip_freeSipMsgBody(pMsgBody);
	return Sdf_co_success;
}
	


/*****************************************************************************
 ** FUNCTION: 	   sdf_fn_uaCompareMediaStreams	
 **
 ** DESCRIPTION:   This API compares two media streams in the session param 
 **                provided and provides the negotiated Session parameter 
 **                and the list of media streams that were acceptable.  	
 **
 *****************************************************************************/
Sdf_ty_matchMediaResult sdf_fn_uaCompareMediaStreams
#ifdef ANSI_PROTO
		(Sdf_st_sessionParam *pNegotiatedSessionParam,\
		Sdf_st_sessionParam *pSessionParamOld, \
		Sdf_st_mediaHandling *pMediaInfo, \
		Sdf_ty_slist *slAcceptedMedia,
#ifdef SDF_SESSIONCHANGE
		Sdf_st_sessionChange *pSessionChange, Sdf_ty_options dOptions,
#endif
		Sdf_st_error *pError)
#else
		(pNegotiatedSessionParam, pSessionParamOld,\
		 pMediaInfo, slAcceptedMedia,
#ifdef SDF_SESSIONCHANGE
		 pSessionChange, dOptions,
#endif
		 pError)
		Sdf_st_sessionParam *pNegotiatedSessionParam;\
		Sdf_st_sessionParam *pSessionParamOld;\
		Sdf_st_mediaHandling *pMediaInfo;\
		Sdf_ty_slist *slAcceptedMedia;
#ifdef SDF_SESSIONCHANGE
		Sdf_st_sessionChange *pSessionChange;
		Sdf_ty_options dOptions;
#endif
		Sdf_st_error *pError;
#endif
{
	Sdf_ty_bool  matchFoundForThisLine = Sdf_co_false;
	Sdf_ty_bool  atLeastOneMatch = Sdf_co_false;
	Sdf_ty_bool  atLeastOneCodecMatch = Sdf_co_false;
	Sdf_ty_u32bit i, j = 0;
	Sdf_st_mediaStream *pMediaStreamNgtd = Sdf_co_null, 
					   *pMediaStreamOld = Sdf_co_null;
	Sdf_st_listIterator dIterator1, dIterator2;
	Sdf_st_codec *pOldCodec = Sdf_co_null,
				 *pNgtdCodec = Sdf_co_null;
	Sdf_ty_retVal dRetVal = Sdf_co_fail;
	Sdf_ty_matchMediaResult dMatchResult = Sdf_en_matchingFailed;
	Sdf_ty_slist dNgtdCodecList;
	Sdf_ty_bool dFreeCodecList = Sdf_co_false;
#ifdef SDF_SESSIONCHANGE
	Sdf_ty_bool dCheckForAddition = Sdf_co_true;
	Sdf_ty_bool dSessionChanged = Sdf_co_false;
	Sdf_st_sessionChange *pLocalSessionChange = Sdf_co_null;
	Sdf_st_mediaChange  *pLocalMediaChange = Sdf_co_null;
	Sdf_ty_u32bit dNgtdCodecIndex = 0, dOldCodecIndex = 0;
#endif

	(void)pMediaInfo;
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) || (pMediaInfo == Sdf_co_null) || \
		(pSessionParamOld == Sdf_co_null) || \
		(pNegotiatedSessionParam == Sdf_co_null) || \
		(slAcceptedMedia == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaCompareMediaStreams( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return dMatchResult; 
	}
#endif	/* End of param validation ifdef */

/*Fix for the SPR-3778 After this fix if invite comes with zero or more m
 * line then answer will go with zero or m lines */
if(pNegotiatedSessionParam->dNumMediaStreams == 0)
	{
				if( pMediaInfo->dMediaState == Sdf_en_offerRecv)
				{
						dMatchResult = Sdf_en_matchingSuccessful;
						return dMatchResult;
				}		
				else if( (pMediaInfo->dMediaState == Sdf_en_offerChangeRecv) && \
								( pMediaInfo->pCurrentSession->dNumMediaStreams == 0) )
				{
							dMatchResult = Sdf_en_matchingSuccessful;
							return dMatchResult;
				}
	}
/* End Fix  SPR-3778 */

	for(i=0; i< pNegotiatedSessionParam->dNumMediaStreams; i++)
	{
		pMediaStreamNgtd = pNegotiatedSessionParam->pMediaStreams[i];
		
		for(j=0; j<pSessionParamOld->dNumMediaStreams; j++)
		{
			pMediaStreamOld = pSessionParamOld->pMediaStreams[j];
			
#ifdef SDF_SESSIONCHANGE
			if(dOptions & SDF_OPT_COMPARE_SESSION)
			{
				/*
				 * We want to do one to one match only - so check 
				 * for session change only if the # of media stream
				 * matches i.e. i&j
				 */
				if(i == j)
				{
					pLocalSessionChange = pSessionChange;
					if(Sdf_fn_uaCheckMediaStreamForChange(pMediaStreamNgtd,\
					 pMediaStreamOld, &(pSessionChange->pMediaChange[i]), \
					 &dSessionChanged, pError) == Sdf_co_fail)
					{
						return dMatchResult;
					}
					if(dSessionChanged == Sdf_co_true)
					{
						pSessionChange->dSessionChange = Sdf_co_true;
					}
				}
				else
				{
					pLocalSessionChange = Sdf_co_null;
				}
				/* 
				 * Check for the addition of the media stream
				 * i.e. added streams at the end of the media
				 * stream list in the session param
				 */
				if(dCheckForAddition == Sdf_co_true)
				{
					Sdf_ty_u32bit dId = 0;
					if(pNegotiatedSessionParam->dNumMediaStreams >
						pSessionParamOld->dNumMediaStreams)
					{
						dId = pSessionParamOld->dNumMediaStreams;
						pSessionChange->dSessionChange = Sdf_co_true;
						while(dId<pNegotiatedSessionParam->dNumMediaStreams)
						{
							pSessionChange->pMediaChange[dId].\
								dMediaChangeType = Sdf_en_addition;
							dId++;
						}
					}
					dCheckForAddition = Sdf_co_false;
				}
			}

			if (pLocalSessionChange)
			{
			    pLocalMediaChange =
					&(pLocalSessionChange->pMediaChange[i]);
			}
#endif
			if(pMediaStreamNgtd->dPort != 0)
			{
				/* Check the protocol and the transport */
				if(sdf_fn_uaIsMediaTypeAndTransportSame(pMediaStreamNgtd, 
						pMediaStreamOld, pError) == Sdf_co_fail)
				{
					continue;
				}
				(void)sdf_listInitIterator(&(pMediaStreamNgtd->slCodecs), 
								&dIterator1, pError);

				if(sdf_listInit(&dNgtdCodecList, __sdf_fn_uaFreeCodec, \
							Sdf_co_false, pError) == Sdf_co_fail)
				{
					return dMatchResult;
				}
				dFreeCodecList = Sdf_co_false;

				while(dIterator1.pCurrentElement !=Sdf_co_null)
				{
					Sdf_ty_bool dMatch = Sdf_co_true;
					pNgtdCodec = (Sdf_st_codec*)dIterator1.pCurrentElement\
											->pData;

					(void)sdf_listInitIterator(&(pMediaStreamOld->slCodecs), 
								&dIterator2, pError);

					while(dIterator2.pCurrentElement !=Sdf_co_null)
					{
						pOldCodec = (Sdf_st_codec*)dIterator2.pCurrentElement\
									->pData;
#ifdef SDF_SESSIONCHANGE
						if(dOptions & SDF_OPT_COMPARE_SESSION)
						{
							/*
							 * Only if the codex indices match go and compare
							 * the session change. This is not applicable 
							 * for the normal negotiation where we need to 
							 * compare all the available codecs. So dMatch
							 * is set to true by default and if we enter here
							 * then based on the condition the flag will be reset
							 */
							if(dNgtdCodecIndex != dOldCodecIndex)
							{
								dMatch = Sdf_co_false;
							}
						}
#endif
						if(dMatch == Sdf_co_true)
						{
							dRetVal = sdf_fn_uaMatchCodec(pNgtdCodec, pOldCodec, 
#ifdef SDF_SESSIONCHANGE
								pLocalSessionChange,pLocalMediaChange,dOptions,
#endif
								pError);
						}
						if(dRetVal != Sdf_co_success)
						{
							(void)sdf_listNext(&dIterator2, pError);
#ifdef SDF_SESSIONCHANGE
							dOldCodecIndex++;
#endif
							continue;
						}

						atLeastOneCodecMatch = Sdf_co_true;
						/* 
						 * Append the matched codec into the ngtd codec list
						 * which will be updated in the media stream later
						 */
						if(sdf_listAppend(&dNgtdCodecList, pNgtdCodec, pError)
							== Sdf_co_fail)
						{
							(void)sdf_listDeleteAll(&dNgtdCodecList, pError);
							return dMatchResult; 
						}
						HSS_LOCKEDINCREF(pNgtdCodec->dRefCount);
						dFreeCodecList = Sdf_co_true;
						break;
					}
					
					(void)sdf_listNext(&dIterator1, pError);
#ifdef SDF_SESSIONCHANGE
					dNgtdCodecIndex++;
#endif
				}
			}
			else
			{
				/*
				 * It is a ngtd stream and which has been rejected
				 * by the peer so set atLeastOneCodecMatch = Sdf_co_true
				 * so that the stream will be added to ngtd session as 
				 * it is. 
				 */
				atLeastOneCodecMatch = Sdf_co_true;
			}

			if(atLeastOneCodecMatch == Sdf_co_true)
			{
				atLeastOneCodecMatch = Sdf_co_false;
				
				if(sdf_fn_uaCompareDirectionAttrib(\
				   	pMediaStreamNgtd->dDirectionAttrib,
					pMediaStreamOld->dDirectionAttrib, 
#ifdef SDF_SESSIONCHANGE
					pLocalSessionChange, pLocalMediaChange,
					dOptions,
#endif
					pError)==Sdf_co_success)
				{
					if(pMediaStreamOld->dMatchedAlready != Sdf_co_true)
					{
						Sdf_st_mediaStream *pAcceptedMediaStream = Sdf_co_null;
						if(pMediaStreamNgtd->dPort != 0)
						{
							pMediaStreamOld->dMatchedAlready = Sdf_co_true;
						}
						matchFoundForThisLine = Sdf_co_true;
						atLeastOneMatch = Sdf_co_true;

						/* 
						 * Special Case:- If the peer is provided
						 * with sendrecv and our direction is send
						 * only/recvonly then change the direction
						 * accordingly i.e. recvonly/sendonly. So that
						 * while changing the direction for the ngtd 
						 * session the peers view and the ngtd will be
						 * in sync.
						 * e.g 
						 * Recvd:- sendrecv; Local:-recvonly
						 * here it will be updated as sendonly and after
						 * the ngtn the ngtd session will be having
						 * recvonly and the peers view will be having
						 * sendonly
						 *
						 * In the case of inactive in either side (from the
						 * peer or local the resultant ngtd as well as the 
						 * peers view shall be inactive.
						 */
						if(pMediaStreamNgtd->dDirectionAttrib \
							== Sdf_en_sendrecv)
						{
							if(pMediaStreamOld->dDirectionAttrib \
								== Sdf_en_sendonly)
							{
								pMediaStreamNgtd->dDirectionAttrib
													= Sdf_en_recvonly;
							}
							else if(pMediaStreamOld->dDirectionAttrib \
								== Sdf_en_recvonly)
							{
								pMediaStreamNgtd->dDirectionAttrib
													= Sdf_en_sendonly;
							}
						}
						else if(pMediaStreamOld->dDirectionAttrib \
												== Sdf_en_inactive)
						{
							pMediaStreamNgtd->dDirectionAttrib
												= Sdf_en_inactive;
						}
						if(pMediaStreamNgtd->dPort != 0)
						{
							/* 
							 * Now remove the existing codec list from the 
							 * NgtdMediaStream and update it with the ngtd 
							 * codec list constructed
							 */
							if(sdf_fn_uaCloneCodecList( \
								&(pMediaStreamNgtd->slCodecs),
								&dNgtdCodecList, pError) == Sdf_co_fail)
							{
								(void)sdf_listDeleteAll(&dNgtdCodecList, pError);
								return dMatchResult;
							}
							/* Free the local reference */
							(void)sdf_listDeleteAll(&dNgtdCodecList, pError);
							dFreeCodecList = Sdf_co_false;
						}

						/* 
						 * In the accepted media streams list we shall be
						 * giving the peer's view of the stream (i.e. peers
						 * port, IP and etc) so before updating with the local 
						 * info in the media stream clone the media stream
						 * and append the cloned stream into the accepted list
						 */
						if(sdf_ivk_uaInitMediaStream(&pAcceptedMediaStream, \
									pError) == Sdf_co_fail)
						{
							return dMatchResult;
						}
						if(sdf_ivk_uaCloneMediaStream(pAcceptedMediaStream, \
								pMediaStreamNgtd, pError) == Sdf_co_fail)
						{
							return dMatchResult; 
						}
						if(sdf_listAppend(slAcceptedMedia,pAcceptedMediaStream,\
										pError)	== Sdf_co_fail)
						{
							return dMatchResult;
						}

						/* 
						 * Now update the local properties in the negotiated
						 * media stream
						 */
						if(Sdf_fn_uaUpdateLocalPropertiesInMediaStream(\
									pMediaStreamNgtd, pMediaStreamOld,\
									pError) == Sdf_co_fail)
						{
							return dMatchResult;
						}
					    
						/* 
						 * Update the media direction in the ngtd media
						 * stream
						 */
						if(sdf_fn_uaChangeDirectionAttrib(\
							&pMediaStreamNgtd->dDirectionAttrib,
							pError) == Sdf_co_fail)
						{
							return dMatchResult;
						}
						break;
					}
				}
			}
			if(dFreeCodecList == Sdf_co_true)
			{
				(void)sdf_listDeleteAll(&dNgtdCodecList, pError);
				dFreeCodecList = Sdf_co_false;
			}
		}
		if(dFreeCodecList == Sdf_co_true)
		{
			(void)sdf_listDeleteAll(&dNgtdCodecList, pError);
		}
		if(matchFoundForThisLine == Sdf_co_false)
		{
			/* 
			 * Put the port # as 0 
			 */
			pMediaStreamNgtd->dPort = 0;
			pMediaStreamNgtd->dNumPort = 0;
#ifdef SDF_SESSIONCHANGE
			/*
			 * Add this rejected stream also into the accepted 
			 * media streams. Once we are done with the updation
			 * of the peer session we'll remove it from the 
			 * peers session
			 */
			if(dOptions & SDF_OPT_COMPUTE_PEER_SESSION)
			{
				Sdf_st_mediaStream *pRejectedMediaStream = Sdf_co_null;
				if(sdf_ivk_uaInitMediaStream(&pRejectedMediaStream, \
							pError) == Sdf_co_fail)
				{
					return dMatchResult;
				}
				if(sdf_ivk_uaCloneMediaStream(pRejectedMediaStream, \
						pMediaStreamNgtd, pError) == Sdf_co_fail)
				{
					return dMatchResult; 
				}
				if(sdf_listAppend(slAcceptedMedia,pRejectedMediaStream,\
								pError)	== Sdf_co_fail)
				{
					return dMatchResult;
				}

			}
#endif
#ifdef SDF_MEDIAGROUP
			(void)sdf_fn_uaRemoveMediaStreamFromGroup(pNegotiatedSessionParam,i,\
								pError);
#endif
		}
		else
		{
#ifdef SDF_QOS
			if((pMediaStreamNgtd->pCurrentQos != Sdf_co_null) || 
			   (pMediaStreamNgtd->pDesiredQos != Sdf_co_null) ||
			   (pMediaStreamNgtd->pConfirmedQos != Sdf_co_null))
			{
				dMatchResult = Sdf_en_qosPreconditionsPresent;
			}
#endif
			matchFoundForThisLine = Sdf_co_false;
		}
#ifdef SDF_SESSIONCHANGE
		/*
		 * If the media change type is addition then
		 * reset the codec and the direction change set while 
		 * doing the media change processing
		 */ 
		if(pLocalMediaChange != Sdf_co_null)
		{
			if((pLocalMediaChange->dMediaChangeType == Sdf_en_addition) ||\
			 (pLocalMediaChange->dMediaChangeType == Sdf_en_rejection))
			{
				pLocalMediaChange->dDirectionChange = Sdf_co_false; 
				pLocalMediaChange->dCodecChange = Sdf_co_false;
			}
		}
#endif
	}
	if(atLeastOneMatch == Sdf_co_true)
	{
		Sdf_ty_u32bit dIndex;
		/* 
		 * Reset the media streams dMatchedAlready flag as we are done with
		 * the negotiation
		 */
		for(dIndex = 0;
			dIndex < pSessionParamOld->dNumMediaStreams;
			dIndex++)
		{
			pSessionParamOld->pMediaStreams[dIndex]->dMatchedAlready 
										= Sdf_co_false;
		}

#ifdef SDF_QOS
		if(dMatchResult != Sdf_en_qosPreconditionsPresent)
#endif
		{
			dMatchResult = Sdf_en_matchingSuccessful;
		}
	}
	else
	{
		dMatchResult = Sdf_en_noMatchFound; 
	}
	return dMatchResult;
}

#ifdef SDF_MEDIAGROUP
/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaHandleGroupInSessionParam 	    	
 **
 ** DESCRIPTION: This API compares the groups from the session parameters 
 **              for the validity based on the fid draft. If the grouping 
 **              is not valid then it ignores the corresponding group. 	
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleGroupInSessionParam
#ifdef ANSI_PROTO
		(Sdf_st_sessionParam	*pSessionParamNew,
		 Sdf_st_sessionParam *pSessionParamOld,
		Sdf_st_sessionParam *pSessionParamNgtd,
		Sdf_st_mediaHandling *pMediaInfo,
		Sdf_st_error *pError)
#else
		(pSessionParamNew,pSessionParamOld,pSessionParamNgtd, pMediaInfo, \
		 pError)
		Sdf_st_sessionParam *pSessionParamNew;
		Sdf_st_sessionParam *pSessionParamOld;
		Sdf_st_sessionParam *pSessionParamNgtd;
		Sdf_st_mediaHandling *pMediaInfo;
		Sdf_st_error *pError;
#endif
{
	Sdf_st_mediaStreamGroup *pNewGroup = Sdf_co_null,
                            *pOldGroup = Sdf_co_null;
	Sdf_st_mediaStream      *pNewStream = Sdf_co_null,
							*pOldStream = Sdf_co_null;
	Sdf_ty_u32bit			i = 0, j = 0;
	Sdf_st_listIterator     dListIterator1, dListIterator2;
	Sdf_ty_slist			slGroupList;
	Sdf_ty_u32bit			dCount=0;
	
#ifdef SDF_PARAMVALIDATION
	if ((pError == Sdf_co_null) || (pSessionParamNew == Sdf_co_null) || 
		(pSessionParamOld == Sdf_co_null) || (pSessionParamNgtd == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaCompareSessionParams( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail; 
	}
#endif	/* End of param validation ifdef */

	/*If Both the New and Old SessionParam Has dNumGroups as zero,then we
	 * can return success
	 */
	if((pSessionParamOld->dNumGroups ==0)&&(pSessionParamNew->dNumGroups==0))
	{
	   pError->errCode = Sdf_en_noUaError;
	   return Sdf_co_success;
	}

	/* 
	 * ??: If the m line is not aligned with the mids then ignore the group
     */
	if((pMediaInfo->dMediaState == Sdf_en_establishedMedia) || 
			(pMediaInfo->dMediaState == Sdf_en_idleMedia))
	{
		/* If the peer doesnt understand Group,then in the Answer he may 
		 * remove the group attribute and all the mid's.We will return
		 * Success from here. We are lenient here.
		 */
		if(pSessionParamNew->dNumGroups ==0)
		{
	   		pError->errCode = Sdf_en_noUaError;
		    return Sdf_co_success;
		}

	    if((pSessionParamNew->dNumGroups) > (pSessionParamOld->dNumGroups))
	    {
		    pError->errCode = Sdf_en_mediaGroupError;
		    return Sdf_co_fail;
	    }
		/*Check if the Mid values are intact.i.e, The Mid Values should be
		 * same as the one for all streams sent in INVITE
		 */
		for(i=0;i<pSessionParamNew->dNumMediaStreams;i++)
		{
			/*Compare the Media Stream Id of each Media Stream*/
			if(Sdf_mc_strcasecmp(pSessionParamNew->pMediaStreams[i]->\
					pMediaStreamId,pSessionParamOld->pMediaStreams[i]->\
					pMediaStreamId)!=0)
			{
				pError->errCode = Sdf_en_mediaGroupError;
		 		return Sdf_co_fail;
			}
		}
	}

	(void)sdf_listInit(&slGroupList,__sdf_fn_uaFreeMediaStreamGroup,Sdf_co_false,\
			pError);
	
	for(i=0; i< pSessionParamNew->dNumGroups; i++)
	{
		Sdf_ty_bool	dGroupMatch = Sdf_co_true;
		
		pNewGroup = pSessionParamNew->pMediaStreamGroups[i];

		if(sdf_fn_uaIsGroupSupported(pNewGroup,pError)==Sdf_co_false)
			continue;

		/*If the Group type is supported then Check if this was a response
		 * and do response specific validations
		 */

		if((pMediaInfo->dMediaState == Sdf_en_establishedMedia) || 
			(pMediaInfo->dMediaState == Sdf_en_idleMedia))
		{
			/* Validate the group:- If no group present in the 
			 * Old session param then the new one also 
			 * shouldn't contain the group. The number of groups in 
			 * the answer will be the same or less. If any grp is 
			 * not understood that'll be ignored 
			 */ 
			/*Find the Group in LocalSession which matches this Group*/
			for(j=0;j<pSessionParamOld->dNumGroups;j++)
			{
				Sdf_ty_bool		dMidMatch = Sdf_co_false;
				/*Check the dMatchedAlready*/
				pOldGroup = pSessionParamOld->pMediaStreamGroups[j];

				if(pOldGroup->dGroupType != pNewGroup->dGroupType)
					continue;
			
				/*Initialize the Iterators and Compare the Mid's in them*/
				(void)sdf_listInitIterator(&(pNewGroup->slMediaStreamIndex), \
						&dListIterator1, pError);
				(void)sdf_listInitIterator(&(pOldGroup->slMediaStreamIndex), \
						&dListIterator2, pError);
				
				while (dListIterator1.pCurrentElement != Sdf_co_null)
				{
					Sdf_ty_u32bit	*pIndex1=Sdf_co_null;
			
					pIndex1 = (Sdf_ty_u32bit*)\
								dListIterator1.pCurrentElement->pData;
					pNewStream = pSessionParamNew->pMediaStreams[*pIndex1];

					while (dListIterator2.pCurrentElement != Sdf_co_null)
					{
						Sdf_ty_u32bit	*pIndex2=Sdf_co_null;
						pIndex2 = (Sdf_ty_u32bit*)\
							dListIterator2.pCurrentElement->pData;

						pOldStream = pSessionParamOld->\
									pMediaStreams[*pIndex2];

						if(Sdf_mc_strcmp(pNewStream->pMediaStreamId,\
							pOldStream->pMediaStreamId ) == 0)
						{
							dMidMatch=Sdf_co_true;
							break;
						}
						(void)sdf_listNext(&dListIterator2, pError);
					}
					if(dMidMatch == Sdf_co_false)
					{
						dGroupMatch = Sdf_co_false;
						break;
					}
					(void)sdf_listNext(&dListIterator1, pError);
				}
				if(dMidMatch == Sdf_co_true)
				{
					dGroupMatch = Sdf_co_true;
					break;
				}
			}
			if(dGroupMatch == Sdf_co_false)
			{
				(void)sdf_listDeleteAll(&slGroupList, pError);	
				pError->errCode = Sdf_en_mediaGroupError;
				return Sdf_co_fail;
			}
		}
		if(dGroupMatch == Sdf_co_true)
		{
			Sdf_st_mediaStreamGroup	*pMediaStreamGroup=Sdf_co_null;
			/*Initialize a mediaStreamGroup Attribute*/
			/*w534*/if(sdf_ivk_uaInitMediaStreamGroup(&pMediaStreamGroup,pError) ==
							Sdf_co_fail)
             {
                /*The list needs to be freed ,if the failure happens
                 *for iteration greater than 1.
                 */
                (void)sdf_listDeleteAll(&slGroupList, pError);	
				return Sdf_co_fail;
             } 
			/*Clone Group From New to Negotiated*/
			/*w534*/if(sdf_ivk_uaCloneMediaStreamGroup(pMediaStreamGroup, pNewGroup,\
			pError) == Sdf_co_fail)
            {
               /*The list needs to be freed ,if the failure happens
                *for iteration greater than 1.
                */
               (void)sdf_listDeleteAll(&slGroupList, pError);
               (void)sdf_ivk_uaFreeMediaStreamGroup(pMediaStreamGroup);
               return Sdf_co_fail;
            }     
			/*Append it to the List*/
			/*w534*/if(sdf_listAppend(&slGroupList,(Sdf_ty_pvoid)pMediaStreamGroup, \
			pError) == Sdf_co_fail)
            {
               /*The list needs to be freed ,if the failure happens
                *for iteration greater than 1.
                */
               (void)sdf_listDeleteAll(&slGroupList, pError);
               (void)sdf_ivk_uaFreeMediaStreamGroup(pMediaStreamGroup);
               return Sdf_co_fail;
            }
			/*Set Flag*/
		}
		else
		{
			(void)sdf_listDeleteAll(&slGroupList, pError);	
			pError->errCode = Sdf_en_mediaGroupError;
			return Sdf_co_fail;
		}
	}
	/*Now Add the MediaStreamGroups from the List into the
	 * NegotiatedSesionParam
	 */
	(void)sdf_listSizeOf(&slGroupList,&dCount,pError);
	if(dCount>0)
	{
		/*Allocate Memory for the Grouping in SessionParam*/
		pSessionParamNgtd->pMediaStreamGroups\
			 = (Sdf_st_mediaStreamGroup**)sdf_memget(Sdf_mc_sdpMemId,\
							sizeof(Sdf_st_mediaStreamGroup*)*\
								dCount,pError);
		i=0;
		(void)sdf_listInitIterator(&slGroupList,&dListIterator1,pError);
		while(dListIterator1.pCurrentElement != Sdf_co_null)
		{
			Sdf_st_mediaStreamGroup	*pMediaStreamGroup=Sdf_co_null;
			pMediaStreamGroup =
				(Sdf_st_mediaStreamGroup*)dListIterator1.pCurrentElement->pData;
			HSS_LOCKEDINCREF(pMediaStreamGroup->dRefCount);
			pSessionParamNgtd->pMediaStreamGroups[i]=pMediaStreamGroup;
			i++;
			(void)sdf_listNext(&dListIterator1,pError);
		}
	}
	pSessionParamNgtd->dNumGroups = dCount;
	/*Delete the Local List*/
	(void)sdf_listDeleteAll(&slGroupList,pError);	
			
	return Sdf_co_success;
}
#endif


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaIsMediaTypeAndTransportSame	    	
 **
 ** DESCRIPTION: This API compares the media type and the transport protocol
 **              and returns success if they match else it returns failure.  	
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaIsMediaTypeAndTransportSame
#ifdef ANSI_PROTO
		(Sdf_st_mediaStream *pStream1,
		Sdf_st_mediaStream *pStream2, Sdf_st_error *pError)
#else
		(pStream1, PStream2, pError)
   		Sdf_st_mediaStream *pStream1;
		Sdf_st_mediaStream *pStream2;
		Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) || (pStream2 == Sdf_co_null) ||
		(pStream1 == Sdf_co_null)) 
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaIsMediaTypeAndTransportSame ( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail; 
	}
#endif	/* End of param validation ifdef */

	if(pStream1->dMediaType != pStream2->dMediaType)
	{
		pError->errCode = Sdf_en_mediaTypeMisMatch ;
		return Sdf_co_fail;
	}
	if(pStream1->dTransport != pStream2->dTransport)
	{
		pError->errCode = Sdf_en_transportProtocolMisMatch;
		return Sdf_co_fail;
	}
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaCompareMediaDirectionAttrib	    	
 **
 ** DESCRIPTION: This API compares the direction attribute based on the
 **              followingrule and return success/failure
 **  			 sendonly-sendonly 	OR 	recvonly-recvonly ---> failure.
 ** 			 sendonly-recvonly 	OR 	recvonly-sendonly ---> success.
 ** 			 sendonly-sendrecv 	OR 	recvonly-sendrecv ---> success.
 **       		 If dRecdDirection is "inactive" or "sendrecv"---> success.
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaCompareDirectionAttrib
#ifdef ANSI_PROTO
			(Sdf_ty_dirAttrib dRecdDirection,
			Sdf_ty_dirAttrib dDirection,
#ifdef SDF_SESSIONCHANGE
			Sdf_st_sessionChange *pSessionChange,
			Sdf_st_mediaChange *pMediaChange,
			Sdf_ty_options dOptions,
#endif
			Sdf_st_error *pError)
#else
			(dRecdDirection, dDirection,
#ifdef SDF_SESSIONCHANGE
			 pSessionChange, pMediaChange, dOptions,
#endif
			 pError)
			Sdf_ty_dirAttrib *dRecdDirection;
			Sdf_ty_dirAttrib dDirection;
#ifdef SDF_SESSIONCHANGE
			Sdf_st_sessionChange *pSessionChange;
			Sdf_st_mediaChange *pMediaChange;
			Sdf_ty_options dOptions;
#endif
			Sdf_st_error *pError;
#endif
{
	if(dRecdDirection == Sdf_en_recvonly)
	{
		if(dDirection == Sdf_en_recvonly)
		{
#ifdef SDF_SESSIONCHANGE
			if(dOptions & SDF_OPT_COMPARE_SESSION)
				return Sdf_co_success;
			else
#endif
			    return Sdf_co_fail;
		}
#ifdef SDF_SESSIONCHANGE
		else
		{
			if((dOptions & SDF_OPT_COMPARE_SESSION) &&\
				(pSessionChange != Sdf_co_null))
			{
				pSessionChange->dSessionChange = Sdf_co_true;
				if(pMediaChange != Sdf_co_null)
				{
					pMediaChange->dDirectionChange = Sdf_co_true;
					if(dDirection != Sdf_en_inactive)
						pMediaChange->dMediaChangeType = Sdf_en_mute;
					else
						pMediaChange->dMediaChangeType = Sdf_en_otherChange;
				}
			}
		}
#endif
	}
	else if(dRecdDirection == Sdf_en_sendonly)
	{
		if(dDirection== Sdf_en_sendonly) 
		{
#ifdef SDF_SESSIONCHANGE
			if(dOptions & SDF_OPT_COMPARE_SESSION)
				return Sdf_co_success;
			else
#endif
			    return Sdf_co_fail;
		}
#ifdef SDF_SESSIONCHANGE
		else
		{
			if((dOptions & SDF_OPT_COMPARE_SESSION) &&\
				(pSessionChange != Sdf_co_null))
			{
				pSessionChange->dSessionChange = Sdf_co_true;
			    if(pMediaChange != Sdf_co_null)
				{
					pMediaChange->dDirectionChange = Sdf_co_true;
					if(dDirection != Sdf_en_inactive)
						pMediaChange->dMediaChangeType = Sdf_en_mute;
					else
						pMediaChange->dMediaChangeType = Sdf_en_otherChange;
				}
			}
		}
#endif
	}
#ifdef SDF_SESSIONCHANGE
	else if(dRecdDirection == Sdf_en_inactive)
	{
		if((dOptions & SDF_OPT_COMPARE_SESSION) &&\
			(pSessionChange != Sdf_co_null))
		{
			if(dDirection != Sdf_en_inactive)
			{
				pSessionChange->dSessionChange = Sdf_co_true;
				if(pMediaChange != Sdf_co_null)
				{
					pMediaChange->dDirectionChange = Sdf_co_true;
					pMediaChange->dMediaChangeType = Sdf_en_otherChange;
				}
			}
		}
	}
	else if((dRecdDirection == Sdf_en_sendrecv)||\
				(dRecdDirection == Sdf_en_dirAttribNone))
	{
		if((dOptions & SDF_OPT_COMPARE_SESSION) &&\
			(pSessionChange != Sdf_co_null))
		{
			if((dDirection != Sdf_en_sendrecv)&&\
					(dDirection != Sdf_en_dirAttribNone))
			{
				pSessionChange->dSessionChange = Sdf_co_true;
				if(pMediaChange != Sdf_co_null)
				{
					pMediaChange->dDirectionChange = Sdf_co_true;
					pMediaChange->dMediaChangeType = Sdf_en_otherChange;
				}
			}
		}
	}
#endif
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: sdf_fn_uaChangeDirectionAttrib	    	
 **
 ** DESCRIPTION: This API changes the direction attribute based on the
 **              followingrule 
 **  				sendonly to recvonly.
 ** 				recvonly to	sendonly
 ** 				sendrecv/inactive to the local direction.
 **
 ** PARAMETERS:	
 **     pRecdDirection[IN/OUT] :Direction attribute to be changed
 **		pError[OUT]	    :	Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaChangeDirectionAttrib
 #ifdef ANSI_PROTO
	(Sdf_ty_dirAttrib *pRecdDirection,
	 Sdf_st_error *pError)
#else 
	(Sdf_ty_dirAttrib *pRecdDirection,
	 Sdf_st_error *pError)
	Sdf_ty_dirAttrib *pRecdDirection;
	Sdf_st_error *pError;
#endif
{
	if(*pRecdDirection == Sdf_en_sendonly)
	{
		*pRecdDirection = Sdf_en_recvonly;
	}
	else if(*pRecdDirection == Sdf_en_recvonly)
	{
		*pRecdDirection = Sdf_en_sendonly;
	}
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaIsSameVersion	    	
 **
 ** DESCRIPTION: This API compares the version and returns 
 **              success if they are same else it returns failure.  	
 **
 ** PARAMETERS:	
 **		 PsessionParam1[IN]:	The version of SessionParam to be compared.
 **		 PsessionParam2[IN]:	The version of SessionParam to be compared with.
 **		 pError[OUT]	 :	Error variable returned in case of error.
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaIsSameVersion 
#ifdef ANSI_PROTO
		(Sdf_st_sessionParam *pSessionParamNew,
		Sdf_st_sessionParam *pSessionParamOld, 
		Sdf_st_error *pError)
#else
		(pSessionParamNew,pSessionParamOld, 
		pError)
		Sdf_st_sessionParam *pSessionParamNew;
		Sdf_st_sessionParam *pSessionParamOld; 
		Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) || (pSessionParamOld ==  Sdf_co_null) ||
		(pSessionParamNew ==  Sdf_co_null))
		
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"Sdf_fn_uaIsSameVersion( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail; 
	}
#endif	/* End of param validation ifdef */

	if(pSessionParamNew->dRemoteSessionVersion 
		== pSessionParamOld->dRemoteSessionVersion)
	{
		pError->errCode = Sdf_en_noUaError;
		return Sdf_co_success;
	}
	return Sdf_co_fail;
}


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaMatchCodec 	    	
 **
 ** DESCRIPTION: This API matches the codec information and returns 
 **              success or failure based on the result.  	
 ** PARAMETERS:	
 **		PCodec1 :	Codec to be matched
 **		PCodec2	:   Codec to be matched with
 **		pObject :   The call object
 **     dOptions:   The options
 **		pError[OUT]	 :	Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaMatchCodec 
#ifdef ANSI_PROTO
		(Sdf_st_codec *pCodec1, Sdf_st_codec *pCodec2,
#ifdef SDF_SESSIONCHANGE
		Sdf_st_sessionChange *pSessionChange,
		Sdf_st_mediaChange	*pMediaChange,
	    Sdf_ty_options dOptions,
#endif
		Sdf_st_error *pError)
#else
		(pCodec1, pCodec2,
#ifdef SDF_SESSIONCHANGE
		 pSessionChange.pMediaChange,dOptions,
#endif
		 pError)
        Sdf_st_codec *pCodec1;
		Sdf_st_codec *pCodec2;
#ifdef SDF_SESSIONCHANGE
		Sdf_st_sessionChange *pSessionChange;
		Sdf_st_mediaChange	*pMediaChange;
	    Sdf_ty_options dOptions;
#endif
		Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) || (pCodec2 ==  Sdf_co_null) || 
				(pCodec1 == Sdf_co_null))
		
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaMatchCodec( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail; 
	}
#ifdef SDF_LINT
#ifdef SDF_SESSIONCHANGE
    if( pMediaChange == Sdf_co_null)
    {
        if(pError != Sdf_co_null)
        {
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaMatchCodec( ): "
			"Invalid pMediaChange Param passed to function", pError);
#endif	            
        }
        return Sdf_co_fail; 
    }
#endif
#endif
    
#endif	/* End of param validation ifdef */

	/* Match the codec name */
	if(pCodec1->dCodecName == Sdf_en_otherCodec) 
	{
		if(pCodec2->pOtherCodecName == Sdf_co_null)
		{
			pError->errCode = Sdf_en_codecMismatch;
#ifdef SDF_SESSIONCHANGE
			if((dOptions & SDF_OPT_COMPARE_SESSION) &&\
			   (pSessionChange != Sdf_co_null))
			{
				pSessionChange->dSessionChange = Sdf_co_true;
				pMediaChange->dCodecChange = Sdf_co_true;
				pMediaChange->dMediaChangeType = Sdf_en_otherChange;
			}
#endif			
			return Sdf_co_fail;
		}
		if ((Sdf_mc_strcasecmp(pCodec1->pOtherCodecName, \
						pCodec2->pOtherCodecName)) != 0)
		{
			pError->errCode = Sdf_en_codecMismatch;
#ifdef SDF_SESSIONCHANGE
			if((dOptions & SDF_OPT_COMPARE_SESSION) &&\
				(pSessionChange != Sdf_co_null))
			{
				pSessionChange->dSessionChange = Sdf_co_true;
				pMediaChange->dCodecChange = Sdf_co_true;
				pMediaChange->dMediaChangeType = Sdf_en_otherChange;
			}
#endif			
			return Sdf_co_fail;

		}
	}
	else
	{
		if(pCodec1->dCodecName !=  pCodec2->dCodecName)
		{
			pError->errCode = Sdf_en_codecMismatch;
#ifdef SDF_SESSIONCHANGE
			if((dOptions & SDF_OPT_COMPARE_SESSION) &&\
				(pSessionChange != Sdf_co_null))
			{
				pSessionChange->dSessionChange = Sdf_co_true;
				pMediaChange->dCodecChange = Sdf_co_true;
				pMediaChange->dMediaChangeType = Sdf_en_otherChange;
			}
#endif			
			return Sdf_co_fail;

		}
	}

	/* Now match the encoding rate */
	if(pCodec1->dEncodingRate !=  pCodec2->dEncodingRate)
	{
		pError->errCode = Sdf_en_codecMismatch;
#ifdef SDF_SESSIONCHANGE
	    if((dOptions & SDF_OPT_COMPARE_SESSION) &&\
			(pSessionChange != Sdf_co_null))
		{	
			pSessionChange->dSessionChange = Sdf_co_true;
			pMediaChange->dCodecChange = Sdf_co_true;
			pMediaChange->dMediaChangeType = Sdf_en_otherChange;
		}
#endif		
		return Sdf_co_fail;
	}
	return Sdf_co_success;
}

#ifdef SDF_SESSIONCHANGE
/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaCheckSessionForChange 
 **
 ** DESCRIPTION: This API returns the session change happened in the session
 **              level
 *****************************************************************************/
 Sdf_ty_retVal Sdf_fn_uaCheckSessionForChange
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pSessionParam1,
	 Sdf_st_sessionParam *pSessionParam2,
	 Sdf_st_sessionChange *pSessionChange, 
	 Sdf_st_error *pError)
#else
	(pSessionParam1, pSessionParam2, pSessionChange, pError)
	Sdf_st_sessionParam *pSessionParam1;
	Sdf_st_sessionParam *pSessionParam2;
	Sdf_st_sessionChange *pSessionChange;
	Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) || (pSessionChange ==  Sdf_co_null) ||
		(pSessionParam1 ==  Sdf_co_null) || (pSessionParam2 == Sdf_co_null))
		
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"Sdf_fn_uaCheckSessionForChange( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail; 
	}
#endif	/* End of param validation ifdef */
    
	/* Check the connection line */
	if(pSessionParam1->pConnection != Sdf_co_null)
	{
		if(pSessionParam2->pConnection != Sdf_co_null)
		{
			if(Sdf_fn_uaCheckConnectionForChange(pSessionParam1->pConnection,
				pSessionParam2->pConnection, pError) == Sdf_co_success)
			{
				pSessionChange->dSessionChange = Sdf_co_true;
				pSessionChange->dSessionConnectionChange = Sdf_co_true;
			}
			else
			{
				if(pError->errCode != Sdf_en_noUaError)
				return Sdf_co_fail;
			}
		}
		else
		{
			pSessionChange->dSessionChange = Sdf_co_true;
			pSessionChange->dSessionConnectionChange = Sdf_co_true;
		}
	}

	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaCheckConnectionForChange 
 **
 ** DESCRIPTION: This API returns the change happened in the connection line
 *****************************************************************************/
 Sdf_ty_retVal Sdf_fn_uaCheckConnectionForChange
#ifdef ANSI_PROTO
	(SdpConnection *pConnection1,
	 SdpConnection *pConnection2,
	 Sdf_st_error *pError)
#else
	(pConnection1, pConnection2, pError)
	Sdf_st_sessionParam *pConnection1;
	Sdf_st_sessionParam *pConnection2;
	Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) || (pConnection1 ==  Sdf_co_null) ||
		(pConnection2 == Sdf_co_null))
		
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"Sdf_fn_uaCheckConnectionForChange( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail; 
	}
#endif	/* End of param validation ifdef */
    
	/* Check the connection line */
	if ((Sdf_mc_strcmp(pConnection2->pNetType, 
		 pConnection1->pNetType) != 0) || 
		(Sdf_mc_strcmp(pConnection2->pAddrType, 
		 pConnection1->pAddrType) != 0) || 
		(Sdf_mc_strcmp(pConnection2->pAddr, 
		 pConnection1->pAddr) != 0))
	{
		return Sdf_co_success;
	}
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_fail;
}


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaCheckMediaStreamForChange 
 **
 ** DESCRIPTION: This API returns the media parameter changes happened in 
 **              the session
 **              level
 ** PARAMETERS:	
 **		pMediaStreamNew[IN] 	: The received SessionParam
 **     pMediaStreamOld[IN] 	: The old Session Param 
 **     pMediaChange[OUT]       : The Session change information
 **     pChanged[OUT]           : boolean specifies whether something changed
 **		pError[OUT]	 		    : Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal Sdf_fn_uaCheckMediaStreamForChange
#ifdef ANSI_PROTO
		(Sdf_st_mediaStream *pMediaStreamNew,
		 Sdf_st_mediaStream *pMediaStreamOld,
		 Sdf_st_mediaChange *pMediaChange,
		 Sdf_ty_bool        *pChanged,
		 Sdf_st_error 		*pError)
#else
		(pMediaStreamNew, pMediaStreamOld, pMediaChange, pChanged, pError)
		 Sdf_st_mediaStream *pMediaStreamNew;
		 Sdf_st_mediaStream *pMediaStreamOld; 
		 Sdf_st_mediaChange *pMediaChange;
		 Sdf_ty_bool        *pChanged;
		 Sdf_st_error *pError;
#endif
{
	Sdf_st_listIterator     dListIterator1;
    Sdf_st_listIterator     dListIterator2;
	SdpConnection           *pConnectionNew = Sdf_co_null,
							*pConnectionOld = Sdf_co_null;
	Sdf_ty_bool             dConnectionChanged = Sdf_co_false;
	Sdf_st_listIterator     dListIteratorBW1;
	Sdf_st_listIterator     dListIteratorBW2;
	Sdf_ty_s8bit            *pBwNew = Sdf_co_null, *pBwOld = Sdf_co_null;
	Sdf_ty_bool             dBWChanged = Sdf_co_false;
	Sdf_ty_u32bit           size1 = 0;
	Sdf_ty_u32bit           size2 = 0;


#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) || (pMediaChange ==  Sdf_co_null) ||
		(pMediaStreamOld ==  Sdf_co_null) || (pMediaStreamNew == Sdf_co_null)\
		|| (pChanged == Sdf_co_null))
		
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaMatchCodec( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail; 
	}
#endif	/* End of param validation ifdef */

	/* Check the port */
	if(pMediaStreamNew->dPort != pMediaStreamOld->dPort)
	{
		*pChanged = Sdf_co_true;
		if((pMediaStreamNew->dPort == 0) && (pMediaStreamOld->dPort != 0))
		{
			pMediaChange->dMediaChangeType = Sdf_en_rejection;
		}
		else if((pMediaStreamNew->dPort != 0) && (pMediaStreamOld->dPort == 0))
		{
			pMediaChange->dMediaChangeType = Sdf_en_addition;
		}
		else
		{
			pMediaChange->dPortChange = Sdf_co_true;
			pMediaChange->dMediaChangeType = Sdf_en_otherChange;
		}
	}
    if((pMediaChange->dMediaChangeType != Sdf_en_addition)&&\
		(pMediaChange->dMediaChangeType != Sdf_en_rejection))
	{
		/* Check the number of ports */
		if(pMediaStreamNew->dNumPort != pMediaStreamOld->dNumPort)
		{
			*pChanged = Sdf_co_true;
			pMediaChange->dPortChange = Sdf_co_true;
			pMediaChange->dMediaChangeType = Sdf_en_otherChange;
		}

		/* Check the connection */
		(void)sdf_listSizeOf(&(pMediaStreamNew->slConnection), &size1, pError);
		(void)sdf_listSizeOf(&(pMediaStreamOld->slConnection), &size2, pError);

		if(size1 != 0)
		{
		   if(size2 != 0)
		   {
				(void)sdf_listInitIterator(&(pMediaStreamNew->slConnection), 
									&dListIterator1, pError);
				(void)sdf_listInitIterator(&(pMediaStreamOld->slConnection), 
									&dListIterator2, pError);
		
				while (dListIterator1.pCurrentElement != Sdf_co_null)
				{
					pConnectionNew = (SdpConnection*)dListIterator1.\
									pCurrentElement->pData;

					while (dListIterator2.pCurrentElement != Sdf_co_null)
					{
						pConnectionOld =  \
							(SdpConnection*)dListIterator2.pCurrentElement->pData;

						if(Sdf_fn_uaCheckConnectionForChange(pConnectionNew,
							pConnectionOld, pError) != Sdf_co_fail)
						{
							*pChanged = Sdf_co_true;
							dConnectionChanged = Sdf_co_true;
							pMediaChange->dMediaConnectionChange = Sdf_co_true;
						if((Sdf_mc_strcmp(pConnectionOld->pAddr, "0.0.0.0")\
								!= 0) &&
							(Sdf_mc_strcmp(pConnectionNew->pAddr, "0.0.0.0") == 0))
							{
								pMediaChange->dMediaChangeType = Sdf_en_mute;
							}
							else
							{
								pMediaChange->dMediaChangeType = \
									Sdf_en_otherChange;
							}

							/*
							 * break the inner "while" as we know something got 
							 * changed in the media line connection
							 */
							break;
						}
						else if(pError->errCode != Sdf_en_noUaError)
						{
							return Sdf_co_fail;
						}
						(void)sdf_listNext(&dListIterator2, pError);
					}
					/*
					 * If we know that something got changed in the media line
					 * connection then break the outer "while" also
					 */
					if(dConnectionChanged == Sdf_co_true)
					{
						break;
					}
					(void)sdf_listNext(&dListIterator1, pError);
				}
		   }
		   else
		   {
			   pMediaChange->dOtherAttributeChange = Sdf_co_true;
			   pMediaChange->dMediaChangeType = Sdf_en_otherChange;
		   }
		   /* Reset the variables */
		   size1 = 0;
		   size2 = 0;
		}


		/* Check to see any other attribute got changed in the media stream*/
		if((pMediaStreamNew->dMediaType != pMediaStreamOld->dMediaType) ||
		   (pMediaStreamNew->dTransport != pMediaStreamOld->dTransport) ||
		   (pMediaStreamNew->dPacketTime != pMediaStreamOld->dPacketTime))
		{
			*pChanged = Sdf_co_true;
			pMediaChange->dOtherAttributeChange = Sdf_co_true;
			pMediaChange->dMediaChangeType = Sdf_en_otherChange;
		}
		else
		{
		   if((pMediaStreamNew->pKey != Sdf_co_null) && \
				   (pMediaStreamOld->pKey !=  Sdf_co_null))
		   {
			   if(Sdf_mc_strcmp(pMediaStreamNew->pKey, pMediaStreamOld->pKey) != 0)
				{
					*pChanged = Sdf_co_true;
					pMediaChange->dOtherAttributeChange = Sdf_co_true;
					pMediaChange->dMediaChangeType = Sdf_en_otherChange;
				}
		   }

		   if((pMediaStreamNew->pTitle != Sdf_co_null) && \
				   (pMediaStreamOld->pTitle !=  Sdf_co_null))
		   {
			   if(Sdf_mc_strcmp(pMediaStreamNew->pTitle, \
									 pMediaStreamOld->pTitle) != 0)
				{
					*pChanged = Sdf_co_true;
					pMediaChange->dOtherAttributeChange = Sdf_co_true;
					pMediaChange->dMediaChangeType = Sdf_en_otherChange;
				}
		   }
		}

		/* Now check for the bandwidth */
		(void)sdf_listSizeOf(&(pMediaStreamNew->slBandwidth), &size1, pError);
		(void)sdf_listSizeOf(&(pMediaStreamOld->slBandwidth), &size2, pError);
		if(size1 != 0)
		{
		   if(size2 != 0)
		   {
				(void)sdf_listInitIterator(&(pMediaStreamNew->slBandwidth), 
											&dListIteratorBW1, pError);
				(void)sdf_listInitIterator(&(pMediaStreamOld->slBandwidth), 
											&dListIteratorBW2, pError);
				while (dListIteratorBW1.pCurrentElement != Sdf_co_null)
				{
					pBwNew = (Sdf_ty_s8bit*)dListIteratorBW1.pCurrentElement->pData;

					while (dListIteratorBW2.pCurrentElement != Sdf_co_null)
					{
						pBwOld = (Sdf_ty_s8bit*)dListIteratorBW2.pCurrentElement->\
									pData;

						if((pBwNew != Sdf_co_null) || (pBwOld != Sdf_co_null))
						{
							if(Sdf_mc_strcmp(pBwNew, pBwOld) != 0)
							{
								*pChanged = Sdf_co_true;
								dBWChanged = Sdf_co_true;
								pMediaChange->dOtherAttributeChange = Sdf_co_true;
								pMediaChange->dMediaChangeType = \
										Sdf_en_otherChange;
								/*
								 * break the inner "while" as we know something got
								 * changed in the bw
								 */
								break;
							}
						}
						(void)sdf_listNext(&dListIteratorBW2, pError);
					}
					/*
					 * If we know that something got changed in the media line
					 * BW then break the outer "while" also
					 */
					if(dBWChanged == Sdf_co_true)
					{
						break;
					}
					(void)sdf_listNext(&dListIteratorBW1, pError);
				}
		   }
		   else
		   {
			   pMediaChange->dOtherAttributeChange = Sdf_co_true;
			   pMediaChange->dMediaChangeType = Sdf_en_otherChange;
		   }
		}
	}
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}
#endif


/*****************************************************************************
 ** FUNCTION: sdf_fn_uaFormMediaListFromSessionParam
 **
 ** DESCRIPTION: This API forms a SdpMedia list out of the given
 **				 session param
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormMediaListFromSessionParam
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pSessionParam,
	 Sdf_ty_slist *pslAcceptedMedia,
	 Sdf_st_error *pError)
#else
	 (pSessionParam, pslAcceptedMedia, pError)
	 Sdf_st_sessionParam *pSessionParam; 
	 Sdf_ty_slist *pslAcceptedMedia;
	 Sdf_st_error *pError;
#endif
{
	Sdf_ty_u8bit	dIndex = 0;
	Sdf_st_mediaStream *pMediaStream = Sdf_co_null;

	for( dIndex = 0; dIndex < pSessionParam->dNumMediaStreams; dIndex++)
	{
		if(Sdf_co_fail == sdf_ivk_uaGetMediaStreamAtIndex(pSessionParam, \
				&pMediaStream, dIndex, pError))
		{
			return Sdf_co_fail;
		}
		if( sdf_listAppend(pslAcceptedMedia, (Sdf_ty_pvoid)pMediaStream, \
				pError) == Sdf_co_fail)
		{
			return Sdf_co_fail;
		}
	}
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaExtractSessionLevelAttribute
 **
 ** DESCRIPTION: 	The API will extract the session level attribute lines and
 **					except group attribute.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaExtractSessionLevelAttribute
#ifdef ANSI_PROTO
	(SdpMessage *pSdpMessage,Sdf_st_sessionParam *pSessionParam,\
		Sdf_ty_slist *pslGroupList,Sdf_st_error *pErr)
#else
	(pSdpMessage,pSessionParam,pslGroupList,pErr)
	SdpMessage *pSdpMessage;
	Sdf_st_sessionParam *pSessionParam;
	Sdf_ty_slist		*pslGroupList;
	Sdf_st_error *pErr;
#endif
{
	/*No param validation requiered*/
	Sdf_ty_u32bit	dCount = 0;
	Sdf_ty_u32bit	i;
	
#ifndef SDF_MEDIAGROUP
	(void)pslGroupList;
#endif
	if(sdp_getAttrCount(pSdpMessage,&dCount,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaExtractSessoinLevelAttribute( ): "
			"Failed to get Attribute Count from SDP message", pErr);
#endif
		return Sdf_co_fail;
	}
	for(i=0;i<dCount;i++)	
	{
		SdpAttr			*pTempAttr=Sdf_co_null;
		Sdf_ty_s8bit	*pName=Sdf_co_null;

		/*Get the Attribute*/
		if(sdp_getAttrAtIndex(pSdpMessage,&pTempAttr,i,\
							(SipError*)&pErr->stkErrCode)==SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaExtractSessoinLevelAttribute: "
				"Failed to get Attribute ",pErr);
#endif					
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		if(sdp_getNameFromAttr(pTempAttr,&pName,\
			(SipError*)&pErr->stkErrCode)==SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaExtractSessoinLevelAttribute: "
				"Failed to get Attribute Name from Attribute",pErr);
#endif		
			sip_freeSdpAttr(pTempAttr);
			pErr->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
#ifdef SDF_MEDIAGROUP
		if(Sdf_mc_strcmp(pName,"group")==0)
		{
			if(sdf_listAppend(pslGroupList,(Sdf_ty_pvoid)pTempAttr,\
				pErr)==Sdf_co_fail)
			{
				
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaExtractSessoinLevelAttribute: "
					"Failed to get Attribute Name from Attribute",pErr);
#endif					
				sip_freeSdpAttr(pTempAttr);
				pErr->errCode=Sdf_en_sdpError;
				return Sdf_co_fail;
			}
			HSS_LOCKEDINCREF(pTempAttr->dRefCount);
		}
		else 
#endif
		if(Sdf_mc_strcmp(pName,"sendonly")==0)
		{
			pSessionParam->dDirectionAttrib = Sdf_en_sendonly;
		}else if(Sdf_mc_strcmp(pName,"recvonly")==0)
		{
			pSessionParam->dDirectionAttrib = Sdf_en_recvonly;
		}else if(Sdf_mc_strcmp(pName,"sendrecv")==0)
		{
			pSessionParam->dDirectionAttrib = Sdf_en_sendrecv;
		}else if(Sdf_mc_strcmp(pName,"inactive")==0)
		{
			pSessionParam->dDirectionAttrib = Sdf_en_inactive;
		}
		/* Now this is niether a group attribute nor a direction attirubute
		 * So,add it into slOtherSessionAttributes
		 */
		else
		{
			if(sdf_listAppend(&(pSessionParam->slOtherSessionAttributes),\
							(Sdf_ty_pvoid)pTempAttr,pErr)==Sdf_co_fail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaExtractSessoinLevelAttribute: "
					"Failed to get Attribute Name from Attribute",pErr);
#endif					
				sip_freeSdpAttr(pTempAttr);
				pErr->errCode=Sdf_en_sdpError;
				return Sdf_co_fail;
			}
			HSS_LOCKEDINCREF(pTempAttr->dRefCount);
		}
		/*Free Local Reference*/
		sip_freeSdpAttr(pTempAttr);
	}
	return Sdf_co_success;
}
 /*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaExtractMediaStreams
 **
 ** DESCRIPTION: 	The API will extract all the media streams 
 ** 				from the SDP message.It extracts the m lines,
 ** 				corresponding a lines,c lines,i lines, b lines,
 **					and k lines
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaExtractMediaStreams
#ifdef ANSI_PROTO
	(SdpMessage *pSdpMessage,
	Sdf_st_sessionParam *pSessionParam,
	Sdf_st_error *pErr)
#else
	(pSdpMessage, pSessionParam, pErr)
	SdpMessage *pSdpMessage;
	Sdf_st_sessionParam	*pSessionParam;
	Sdf_st_error	*pErr;
#endif
{
	Sdf_ty_u32bit	i=0;
	Sdf_ty_u32bit	dMediaCount=0;
	/*No Param Validation*/
	
	/* Get Media Count */
	if(sdp_getMediaCount(pSdpMessage,&dMediaCount,\
			 (SipError *)&(pErr->stkErrCode))==SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaExtractMediaStreams: "
			"Failed to get Media Count",pErr);
#endif	
		return Sdf_co_fail;
	}
	pSessionParam->dNumMediaStreams = dMediaCount;

	if(dMediaCount==0)
	{
		pErr->stkErrCode = Sdf_en_noUaError;
		/*No media Stream is present in the SdpMessage
		 * so return from here
		 */
		return Sdf_co_success;
	}
	/*Allocate array of MediaStreams Pointers*/
	
	pSessionParam->pMediaStreams = (Sdf_st_mediaStream**)sdf_memget(\
			Sdf_mc_sdpMemId, sizeof(Sdf_st_mediaStream*)*dMediaCount, pErr);

	/* Allocate Memory for Individual Pointers*/
	for(i=0;i<dMediaCount;i++)
	{
		/*Init MediaStreams*/
		if(sdf_ivk_uaInitMediaStream(&(pSessionParam->pMediaStreams[i]),\
				pErr)==Sdf_co_fail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaExtractMediaStreams: "
				"Failed to Init MediaStream",pErr);
#endif	
			return Sdf_co_fail;
		}
	}

	/* Iterate through all media lines */
	for(i=0;i<dMediaCount;i++)
	{
		SdpMedia	*pSdpMedia=Sdf_co_null;
		Sdf_st_mediaStream *pMediaStream;
		
		/*Get the MediaStream to work with*/
		pMediaStream = pSessionParam->pMediaStreams[i] ;
		
		/* Get Media Line */
		if(sdp_getMediaAtIndex(pSdpMessage,&pSdpMedia,i,\
			(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaExtractMediaStreams: "
			"Failed to get Media from SdpMessage",pErr);
#endif	
		return Sdf_co_fail;
		}
		/*Fill the MediaStream Structure with the SdpMedia Info*/
		if(sdf_ivk_uaFormMediaStreamFromSdpMedia(pMediaStream,pSdpMedia,\
					pErr)==Sdf_co_fail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaExtractMediaStreams: "
			"Failed to Form MediaStream Structure",pErr);
#endif	
		return Sdf_co_fail;

		}
		/*Free Local Reference*/	
		sip_freeSdpMedia(pSdpMedia);
		
	}
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaExtractMediaTypeFromMeida
 **
 ** DESCRIPTION: 	The API will extract the media type from the media line  
 **					and store its corresponding ENUM in the structure
 *****************************************************************************/
		
Sdf_ty_retVal sdf_fn_uaExtractMediaTypeFromMeida
#ifdef ANSI_PROTO
	(SdpMedia 			*pSdpMedia,
	 Sdf_st_mediaStream	*pMediaStream,
	 Sdf_st_error		*pErr)
#else
	(pSdpMedia,pMediaStream,pErr)
	SdpMedia			*pSdpMedia;
	Sdf_st_mediaStream	*pMediaStream;
	Sdf_st_error		*pErr;
#endif
{
	Sdf_ty_s8bit	*pMediaType=Sdf_co_null;
	/*Get M value from Media*/
	
	if(sdp_getMvalueFromMedia(pSdpMedia,&pMediaType,\
		(SipError*)&pErr->stkErrCode)==SipFail)
	{
#ifdef SDF_ERROR			
	sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
		(Sdf_ty_s8bit*)"sdf_fn_uaExtractMediaTypeFromMeida: "
		"Failed to get MediaType from Media",pErr);
#endif	
	return Sdf_co_fail;
	}
	if(Sdf_mc_strcasecmp(pMediaType,"audio")==0)
		pMediaStream->dMediaType = Sdf_en_audio;
	else if(Sdf_mc_strcasecmp(pMediaType,"video")==0)
		pMediaStream->dMediaType = Sdf_en_video;
	else if(Sdf_mc_strcasecmp(pMediaType,"whiteboard")==0)
		pMediaStream->dMediaType = Sdf_en_whiteboard;
	else if(Sdf_mc_strcasecmp(pMediaType,"text")==0)
		pMediaStream->dMediaType = Sdf_en_text;
	else 
		{
			pMediaStream->dMediaType = Sdf_en_mediaTypeother;
			Sdf_mc_strdup(pMediaStream->pOtherMediaType, pMediaType);
		}
	return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaExtractTransportTypeFromMedia
 **
 ** DESCRIPTION: 	The API will extract the transport protocol from 
 ** 				the media line and store its corresponding ENUM 
 **					in the structure
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaExtractTransportTypeFromMedia
#ifdef ANSI_PROTO
	(SdpMedia 			*pSdpMedia,
	 Sdf_st_mediaStream	*pMediaStream,
	 Sdf_st_error		*pErr)
#else
	(pSdpMedia,pMediaStream,pErr)
	SdpMedia			*pSdpMedia;
	Sdf_st_mediaStream	*pMediaStream;
	Sdf_st_error		*pErr;
#endif
{
	Sdf_ty_s8bit	*pProtoType = Sdf_co_null;
	/*Get Protocolfrom Media*/
	
	if(sdp_getProtoFromMedia(pSdpMedia,&pProtoType,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
#ifdef SDF_ERROR			
	sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
		(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
		"Failed to get Protocol from Media",pErr);
#endif	
	return Sdf_co_fail;
	}

	if(Sdf_mc_strcasecmp(pProtoType,"RTP/AVP")==0)
		pMediaStream->dTransport = Sdf_en_rtpAvp;
	else if(Sdf_mc_strcasecmp(pProtoType,"udp")==0)
		pMediaStream->dTransport = Sdf_en_udp;
	else if(Sdf_mc_strcasecmp(pProtoType,"tcp")==0)
		pMediaStream->dTransport = Sdf_en_tcp;
	else
		{
			pMediaStream->dTransport = Sdf_en_other;
			Sdf_mc_strdup(pMediaStream->pOtherTransportType,pProtoType);
		}

		return Sdf_co_success;

}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaProcessCodecs
 **
 ** DESCRIPTION: 	The API will Extract the Format String and process the
 **					individual codecs and also associate the "rtmap" and
 **					"fmtp" attributes with them.
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaProcessCodecs
#ifdef ANSI_PROTO
	(Sdf_st_mediaStream	*pMediaStream,
	 SdpMedia			*pSdpMedia,
	 Sdf_st_error		*pErr)
#else
	(pMediaStream,pSdpMedia,pErr)
	Sdf_st_mediaStream	*pMediaStream;
	 SdpMedia			*pSdpMedia;
	 Sdf_st_error		*pErr;
#endif
{
	Sdf_ty_s8bit	*pFormatString = Sdf_co_null;
	Sdf_ty_s8bit	dTempFormatString[Sdf_co_characterBufferSize];
	Sdf_ty_s8bit	*pSplitFormatString=Sdf_co_null;
	Sdf_ty_s8bit	*pCodecString=Sdf_co_null;
	
	/* Extract format string*/
	if(sdp_getFormatFromMedia(pSdpMedia,&pFormatString,
		(SipError*)&pErr->stkErrCode)==SipFail)
	{
#ifdef SDF_ERROR			
	sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
		(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
		"Failed to get Protocol from Media",pErr);
#endif	
	return Sdf_co_fail;
	}
	/* Split them into codecs*/
	Sdf_mc_strcpy(dTempFormatString, pFormatString);
	pCodecString = Sdf_mc_strtokr(dTempFormatString, " ",&pSplitFormatString);

	while(pCodecString != Sdf_co_null)
	{
		Sdf_st_codec *pCodec;
		Sdf_ty_u32bit	dAttrCount=0,i=0;
		Sdf_ty_bool		dFoundRtpMap=Sdf_co_false;
		
		/*Init a Codec Structure*/
		if(sdf_ivk_uaInitCodec(&pCodec,pErr) == Sdf_co_fail) return Sdf_co_fail;
		
		if(sdf_fn_uaIsNumber(pCodecString))
		{
			pCodec->dCodecNum = Sdf_mc_atoi(pCodecString);
			pCodec->dCodecName = sdf_fn_uaMapCodecNumberToName( \
										pCodec->dCodecNum, pErr);
#ifdef SDF_UAB2B
            /*TODO SPR 3851*/
            if (Sdf_en_unassigned == pCodec->dCodecName)
            {
                pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
                sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
                        (Sdf_ty_s8bit*)"sdf_fn_uaProcessCodecs: "
                        "Codec not yet assigned",pErr);
#endif		
                (void)sdf_ivk_uaFreeCodec(pCodec);
                return Sdf_co_fail;

            }
#endif
        }
		else
		{
			if(sdf_fn_uaMapCodecStringToEnum(pCodecString, \
					&pCodec->dCodecName) == Sdf_co_fail)
			{
				pCodec->dCodecName = Sdf_en_otherCodec;
			}
		}
		if(pCodec->dCodecName == Sdf_en_otherCodec)
			Sdf_mc_strdup(pCodec->pOtherCodecName, pCodecString);	
		
		/*Get the Codec ENUM for a given codec number*/
			
		if(sdp_getAttrCountFromMedia(pSdpMedia,&dAttrCount,
			(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
				"Failed to get Protocol from Media",pErr);
#endif		
			(void)sdf_ivk_uaFreeCodec(pCodec);
			return Sdf_co_fail;
		}
		for(i=0;i<dAttrCount;i++)
		{
			Sdf_ty_s8bit	dTempAttrValue[Sdf_co_characterBufferSize];
			Sdf_ty_s8bit	*pTempAttrSplit=Sdf_co_null;
			SdpAttr			*pMediaAttr=Sdf_co_null;
			Sdf_ty_s8bit	*pAttrName=Sdf_co_null,*pAttrValue=Sdf_co_null;
			Sdf_ty_s8bit	*pRtpStringCodec=Sdf_co_null;

			if(sdp_getAttrAtIndexFromMedia(pSdpMedia,&pMediaAttr,i,\
					(SipError*)&pErr->stkErrCode)==SipFail)
			{
				pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
					"Failed to get Protocol from Media",pErr);
#endif	
				(void)sdf_ivk_uaFreeCodec(pCodec);
				return Sdf_co_fail;
			}
			if(sdp_getNameFromAttr(pMediaAttr,&pAttrName,\
				(SipError*)&pErr->stkErrCode)==SipFail)
			{
				pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
					"Failed to get Protocol from Media",pErr);
#endif			
				(void)sdf_ivk_uaFreeCodec(pCodec);
				sip_freeSdpAttr(pMediaAttr);
				return Sdf_co_fail;
			}
			if(Sdf_mc_strcasecmp(pAttrName,"rtpmap")==0)
			{
				if(sdp_getValueFromAttr(pMediaAttr,&pAttrValue,\
					(SipError*)&pErr->stkErrCode)==SipFail)
				{
					pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
						(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia:"
						"Failed to get Protocol from Media",pErr);
#endif	
					(void)sdf_ivk_uaFreeCodec(pCodec);
					sip_freeSdpAttr(pMediaAttr);
					return Sdf_co_fail;	
				}
				/*Split the Attribute Value*/
				Sdf_mc_strcpy(dTempAttrValue, pAttrValue);
				/*??: Make this to take any white space*/
				pRtpStringCodec = Sdf_mc_strtokr(dTempAttrValue," ",\
										&pTempAttrSplit);

				if(pRtpStringCodec == Sdf_co_null)
				{
					pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
						sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
							(Sdf_ty_s8bit*)"sdf_fn_uaProcessCodecs: "
							"Failed Parse rtpmap Attribute",pErr);
#endif	
					(void)sdf_ivk_uaFreeCodec(pCodec);
					sip_freeSdpAttr(pMediaAttr);
					return Sdf_co_fail;	
				}
				if(pCodec->dCodecNum == Sdf_mc_atoi(pRtpStringCodec))
				{
					Sdf_ty_s8bit	*pRtpEncodingName = Sdf_co_null;
					Sdf_ty_s8bit	*pRtpEncodingRate = Sdf_co_null;
					Sdf_ty_s8bit 	*pRtpChannels = Sdf_co_null;
					Sdf_ty_s8bit	dRtpMapping[Sdf_co_characterBufferSize];
					
					dFoundRtpMap = Sdf_co_true;	

					if( pTempAttrSplit != Sdf_co_null )
						pRtpEncodingName = Sdf_mc_strtokr(pTempAttrSplit, \
												"/", &pTempAttrSplit);
					if( pTempAttrSplit != Sdf_co_null )
						pRtpEncodingRate = Sdf_mc_strtokr(pTempAttrSplit, \
											"/", &pTempAttrSplit);
					if( pTempAttrSplit != Sdf_co_null )
						pRtpChannels	= Sdf_mc_strtokr(pTempAttrSplit," ",
											&pTempAttrSplit);
					if(pRtpEncodingName == Sdf_co_null)
					{
						pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
						sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_sdpError,\
							(Sdf_ty_s8bit*)"sdf_fn_uaProcessCodecs: "
							"Failed Parse rtpmap Attribute",pErr);
#endif	
						(void)sdf_ivk_uaFreeCodec(pCodec);
						sip_freeSdpAttr(pMediaAttr);
						return Sdf_co_fail;	
					}						
					Sdf_mc_strcpy(dRtpMapping, pRtpEncodingName);
				
					if(sdf_fn_uaMapCodecStringToEnum(dRtpMapping,\
									&pCodec->dCodecName)==Sdf_co_fail)
					{
						pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
						sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_sdpError,\
							(Sdf_ty_s8bit*)"sdf_fn_uaProcessCodecs: "
							"Failed Parse rtpmap Attribute",pErr);
#endif	
						(void)sdf_ivk_uaFreeCodec(pCodec);
						sip_freeSdpAttr(pMediaAttr);
						return Sdf_co_fail;	
					}

					if(pCodec->dCodecName != Sdf_en_otherCodec)
					{
						if( pRtpEncodingRate != Sdf_co_null )
							pCodec->dEncodingRate = \
										Sdf_mc_atoi(pRtpEncodingRate);
						if(pRtpChannels != Sdf_co_null)
							pCodec->dChannels = Sdf_mc_atoi(pRtpChannels);
					}
					else
					{
						if( pRtpEncodingRate != Sdf_co_null )
						{

							pCodec->dEncodingRate = \
										Sdf_mc_atoi(pRtpEncodingRate);
						}
						if(pRtpChannels != Sdf_co_null )
						{
							
							pCodec->dChannels = Sdf_mc_atoi(pRtpChannels);
						}
						if( pCodec->pOtherCodecName != Sdf_co_null )
							(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
								(Sdf_ty_pvoid *)&(pCodec->pOtherCodecName), \
								pErr);

						Sdf_mc_strdup(pCodec->pOtherCodecName,dRtpMapping);	
					}
				}
			}
			else if(Sdf_mc_strcasecmp(pAttrName,"fmtp")==0)
			{
				if(sdp_getValueFromAttr(pMediaAttr,&pAttrValue,\
					(SipError*)&pErr->stkErrCode)==SipFail)
				{
					pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
					"Failed to get Attribute Value from Attribute",pErr);
#endif	
				(void)sdf_ivk_uaFreeCodec(pCodec);
				sip_freeSdpAttr(pMediaAttr);
				return Sdf_co_fail;	
			
				}
				/*Split the Attribute Value*/
				Sdf_mc_strcpy(dTempAttrValue, pAttrValue);
				pRtpStringCodec = Sdf_mc_strtokr(dTempAttrValue, " ",\
										&pTempAttrSplit);
				if(Sdf_mc_strcasecmp(pRtpStringCodec,pCodecString)==0)
				{
					/*Copy the entire String after a=fmtp:<codec>*/
					Sdf_mc_strdup(pCodec->pFmtpString,pAttrValue);
				}
			}
			/*Free the Attribute*/
			sip_freeSdpAttr(pMediaAttr);
		}/* End of Attribute iteration*/

		/*If ther was no rtpmap Attribute found and if it is a
		 * Standard Codec we add one here
		 */
		 if((!dFoundRtpMap) && (pCodec->dCodecName != Sdf_en_otherCodec))
		 {
			 
			 Sdf_ty_s8bit	dTempRtp[Sdf_co_characterBufferSize];
			 Sdf_ty_s8bit	*pStr1=Sdf_co_null;
			 Sdf_ty_s8bit	*pStr2=Sdf_co_null;
			 
			 if(sdf_fn_uaMapCodecNumToRtpmapString(pCodec->dCodecNum, \
								dTempRtp, pErr) == Sdf_co_fail)
			 {
				pErr->errCode = Sdf_en_sdpError;
				(void)sdf_ivk_uaFreeCodec(pCodec);
				return Sdf_co_fail;	
			 }
			 pStr1 = Sdf_mc_strtokr(dTempRtp,"/",&pStr2);
		 
			 pStr1 = Sdf_mc_strtokr(pStr2,"/",&pStr2);

			 pCodec->dEncodingRate = Sdf_mc_atoi(pStr1);

			 pStr1 = Sdf_mc_strtokr(pStr2," ",&pStr2);

			 pCodec->dChannels = Sdf_mc_atoi(pStr1);
		}
			
			 
		/*Append Codec to slCodecs*/
		if(sdf_listAppend(&(pMediaStream->slCodecs),(Sdf_ty_pvoid)pCodec,\
							pErr)==Sdf_co_fail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
				"Failed to get Attribute Value from Attribute",pErr);
#endif	
			(void)sdf_ivk_uaFreeCodec(pCodec);
			return Sdf_co_fail;	
		}
		/*Get the next Codec*/
		pCodecString = Sdf_mc_strtokr(pSplitFormatString, " ",\
								&pSplitFormatString);

	}/*End of Codec Iteration*/
	return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaProcessMediaAttributes
 **
 ** DESCRIPTION: 	The API will process media level attributes such
 **					as "qos","direction" specially and other
 **					attributes will be pushed in slOtherAttribs field  
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaProcessMediaAttributes
#ifdef ANSI_PROTO
	(Sdf_st_mediaStream	*pMediaStream,
	 SdpMedia			*pSdpMedia,
	 Sdf_st_error		*pErr)
#else
	(pMediaStream,pSdpMedia,pErr)
	Sdf_st_mediaStream	*pMediaStream;
	 SdpMedia			*pSdpMedia;
	 Sdf_st_error		*pErr;
#endif
{
	Sdf_ty_u32bit	dAttrCount=0,i=0;
	/* Get Attr Count from Media */
	if(sdp_getAttrCountFromMedia(pSdpMedia,&dAttrCount,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
				"Failed to get Attribute Value from Attribute",pErr);
#endif	
			return Sdf_co_fail;	
	}
	for(i=0;i<dAttrCount;i++)
	{
		SdpAttr				*pAttr;
#ifdef SDF_QOS
		Sdf_ty_reservTypeInfo	dQos = Sdf_en_noneQos;
		Sdf_ty_s8bit	*pSplitString1=Sdf_co_null;
		Sdf_ty_s8bit	*pSplitString2=Sdf_co_null;
		Sdf_ty_s8bit	dTempAttrValue[Sdf_co_smallBufferSize];
#endif
		Sdf_ty_s8bit	*pAttrName = Sdf_co_null,*pAttrValue;
		Sdf_ty_bool		dMalFormed = Sdf_co_false;


		if(sdp_getAttrAtIndexFromMedia(pSdpMedia,&pAttr,i,\
				(SipError*)&pErr->stkErrCode)==SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
				"Failed to get Attribute Value from Attribute",pErr);
#endif	
			return Sdf_co_fail;
		}
		if(sdp_getNameFromAttr(pAttr,&pAttrName,\
				(SipError*)&pErr->stkErrCode)==SipFail)
		{
			sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
				"Failed to get Attribute Value from Attribute",pErr);
#endif	
			return Sdf_co_fail;
		}
		/*
		 * Process qos and direction 
		 * specially. If any of the attribute is not 
		 * formed properly (malformed) then it will go
		 * into the slOtherAttributes member, else into
		 * their respective place holder
		 */
#ifdef SDF_QOS
		if(Sdf_mc_strcasecmp(pAttrName,"curr")==0)
		{
			if(sdp_getValueFromAttr(pAttr,&pAttrValue,\
			(SipError*)&pErr->stkErrCode)==SipFail)
			{
				sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
					"Failed to get Attribute Value from Attribute",pErr);
#endif	
				return Sdf_co_fail;
			}
			
			Sdf_mc_strcpy(dTempAttrValue,pAttrValue);

			pSplitString1 = Sdf_mc_strtokr(dTempAttrValue," ",\
									&pSplitString2);
			if(Sdf_mc_strcasecmp(pSplitString1,"qos")==0)
				dQos = Sdf_en_currQos;

		}else if(Sdf_mc_strcasecmp(pAttrName,"des")==0)
		{
			if(sdp_getValueFromAttr(pAttr,&pAttrValue,\
			(SipError*)&pErr->stkErrCode)==SipFail)
			{
				sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
					"Failed to get Attribute Value from Attribute",pErr);
#endif	
				return Sdf_co_fail;
			}
			Sdf_mc_strcpy(dTempAttrValue,pAttrValue);
			pSplitString1 = Sdf_mc_strtokr(dTempAttrValue," ",\
									&pSplitString2);
			
			if(Sdf_mc_strcasecmp(pSplitString1,"qos")==0)
				dQos = Sdf_en_desQos;

		}else if(Sdf_mc_strcasecmp(pAttrName,"conf")==0)
		{
			if(sdp_getValueFromAttr(pAttr,&pAttrValue,\
			(SipError*)&pErr->stkErrCode)==SipFail)
			{
				sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
					"Failed to get Attribute Value from Attribute",pErr);
#endif	
				return Sdf_co_fail;
			}
			
			Sdf_mc_strcpy(dTempAttrValue,pAttrValue);

			pSplitString1 = Sdf_mc_strtokr(dTempAttrValue," ",\
									&pSplitString2);
			
			if(Sdf_mc_strcasecmp(pSplitString1,"qos")==0)
				dQos = Sdf_en_confQos;
		}
		if(dQos != Sdf_en_noneQos)
		{
			switch(dQos)
			{
				case Sdf_en_currQos:
				{
					Sdf_st_qosAttrib	*pCurrQos=Sdf_co_null;

					/*Handle curr Qos*/
					pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
										&pSplitString2);

					if(Sdf_mc_strcasecmp(pSplitString1,"e2e")==0)
					{
						Sdf_st_e2eQosDetails	*pE2eQos=Sdf_co_null;

						if(sdf_ivk_uaInitQosAttrib(\
							&(pMediaStream->pCurrentQos),\
								Sdf_en_e2e,pErr) == Sdf_co_fail)
						{
							sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
							sdf_fn_setError(Sdf_en_majorErrors,\
							Sdf_en_sdpError, \
							(Sdf_ty_s8bit*)
							"sdf_fn_uaExtractTransportTypeFromMedia: "
							"Failed to Init QosAttrib",\
							pErr);
#endif	
							return Sdf_co_fail;
						}

							
						pCurrQos = pMediaStream->pCurrentQos;

						if(sdf_ivk_uaInitE2eQosDetails(\
							&(pCurrQos->u.pE2eQos), pErr) == Sdf_co_fail)
						{
							sip_freeSdpAttr(pAttr);
							(void)sdf_ivk_uaFreeQosAttrib(pCurrQos);
#ifdef SDF_ERROR			
							sdf_fn_setError(Sdf_en_majorErrors,\
							Sdf_en_sdpError, \
							(Sdf_ty_s8bit*)
							"sdf_fn_uaExtractTransportTypeFromMedia: "
							"Failed to Init E2eQosDetails",\
							pErr);
#endif	
							return Sdf_co_fail;
						}
						pE2eQos = pCurrQos->u.pE2eQos; 

						pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
											&pSplitString2);
						
						/*Direction Type*/
						if(Sdf_mc_strcasecmp(pSplitString1,"send")==0)
						{
							Sdf_mc_strdup(pE2eQos->dE2eSend.\
										pPrecondition,"qos");
							
						}else if(Sdf_mc_strcasecmp(pSplitString1,"recv")==0)
						{

							Sdf_mc_strdup(pE2eQos->dE2eRecv.\
										pPrecondition,"qos");
						}else if(Sdf_mc_strcasecmp(pSplitString1,"sendrecv")==0)
						{
							Sdf_mc_strdup(pE2eQos->dE2eSend.\
										pPrecondition,"qos");
							Sdf_mc_strdup(pE2eQos->dE2eRecv.\
										pPrecondition,"qos");

						}
						else if(Sdf_mc_strcasecmp(pSplitString1,"none")!=0)
						{
							/* 
							 * In this case don't set anything 
							 * the top level pointer will be available 
							 * but the precondition will be null for 
							 * all the direction then the application 
							 * shall assume that it is "none" 
							 */
						}
						
					}
					else /* Segmented Qos */
					{
						Sdf_st_segmentedQosDetails	*pSegmentQos=Sdf_co_null;

						if(pMediaStream->pCurrentQos == Sdf_co_null)
						{
							if(sdf_ivk_uaInitQosAttrib(\
								&(pMediaStream->pCurrentQos),\
								Sdf_en_segmented,pErr) == Sdf_co_fail)
							{
								sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
								sdf_fn_setError(Sdf_en_majorErrors,\
									Sdf_en_sdpError, \
									(Sdf_ty_s8bit*)
									"sdf_fn_uaExtractTransportTypeFromMedia: "
									"Failed to Init QosAttrib",\
									pErr);
#endif	
								return Sdf_co_fail;
							}

						}
						pCurrQos = pMediaStream->pCurrentQos;

						if(pCurrQos->u.pSegmentedQos == Sdf_co_null)
						{
							if(sdf_ivk_uaInitSegmentedQosDetails(\
								&(pCurrQos->u.pSegmentedQos),\
							   	pErr) == Sdf_co_fail)
							{
								sip_freeSdpAttr(pAttr);
								(void)sdf_ivk_uaFreeQosAttrib(pCurrQos);
#ifdef SDF_ERROR			
								sdf_fn_setError(Sdf_en_majorErrors,\
								Sdf_en_sdpError, \
								(Sdf_ty_s8bit*)
								"sdf_fn_uaExtractTransportTypeFromMedia: "
								"Failed to Init SegQosDetails",\
								pErr);
#endif	
								return Sdf_co_fail;
							}
						}
						pSegmentQos = pCurrQos->u.pSegmentedQos;

						if(Sdf_mc_strcasecmp(pSplitString1,"local")==0)
						{
							pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
												&pSplitString2);
							
							/*Direction Type*/
							
							if(Sdf_mc_strcasecmp(pSplitString1,"send")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dLocalSend.\
											pPrecondition,"qos");

							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"recv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dLocalRecv.\
											pPrecondition,"qos");

							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"sendrecv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dLocalSend.\
											pPrecondition,"qos");
								Sdf_mc_strdup(pSegmentQos->dLocalRecv.\
											pPrecondition,"qos");
							}
							else if((Sdf_mc_strcasecmp(pSplitString1,"none"))!=0)
							{
								/* 
							 	 * In this case don't set anything 
							 	 * the top level pointer will be available 
							 	 * but the precondition will be null for 
							 	 * all the direction then the application 
							 	 * shall assume that it is "none" 
							 	 */
							}

						}
						else if (Sdf_mc_strcasecmp(pSplitString1,"remote")==0)
						{

							pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
												&pSplitString2);
							/*Direction Type*/
							if(Sdf_mc_strcasecmp(pSplitString1,"send")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dRemoteSend.\
											pPrecondition,"qos");

							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"recv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dRemoteRecv.\
											pPrecondition,"qos");

							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"sendrecv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dRemoteSend.\
											pPrecondition,"qos");
								Sdf_mc_strdup(pSegmentQos->dRemoteRecv.\
											pPrecondition,"qos");
							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"none")!=0)
							{
								/* 
							 	 * In this case don't set anything 
							 	 * the top level pointer will be available 
							 	 * but the precondition will be null for 
							 	 * all the direction then the application 
							 	 * shall assume that it is none direction
							 	 */
							}
						}
					}
					break;
											
				}
				case Sdf_en_desQos:
				{
					Sdf_ty_qosStrength	dQosStrength = Sdf_en_qosStrengthNone;
					Sdf_st_qosAttrib	*pDesQos=Sdf_co_null;

					/*Handle des Qos*/
					pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
											&pSplitString2);
					/*Strength*/
					if(Sdf_mc_strcasecmp(pSplitString1,"optional")==0)
					{
						dQosStrength = Sdf_en_optional;
					}
					else if(Sdf_mc_strcasecmp(pSplitString1,"mandatory")==0)
					{
						dQosStrength = Sdf_en_mandatory;
					}
					else if(Sdf_mc_strcasecmp(pSplitString1,"failure")==0)
					{
						dQosStrength = Sdf_en_failure;
					}
					else if(Sdf_mc_strcasecmp(pSplitString1,"none")==0)
					{
						dQosStrength = Sdf_en_none;
					}
					else 
					{
						dQosStrength = Sdf_en_unknown;
					}

					/*Status Type*/
					pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
											&pSplitString2);
					if(Sdf_mc_strcasecmp(pSplitString1,"e2e")==0)
					{
						Sdf_st_e2eQosDetails	*pE2eQos=Sdf_co_null;
						if(sdf_ivk_uaInitQosAttrib(\
							&(pMediaStream->pDesiredQos),\
							Sdf_en_e2e,pErr) == Sdf_co_fail)
						{
							sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
							sdf_fn_setError(Sdf_en_majorErrors,\
							Sdf_en_sdpError, \
							(Sdf_ty_s8bit*)
							"sdf_fn_uaExtractTransportTypeFromMedia: "
							"Failed to Init QosAttrib",\
							pErr);
#endif	
							return Sdf_co_fail;

						}
						pDesQos = pMediaStream->pDesiredQos;

						if(sdf_ivk_uaInitE2eQosDetails(\
							&(pDesQos->u.pE2eQos), pErr) == Sdf_co_fail)
						{
							sip_freeSdpAttr(pAttr);
							(void)sdf_ivk_uaFreeQosAttrib(pDesQos);
#ifdef SDF_ERROR			
							sdf_fn_setError(Sdf_en_majorErrors,\
							Sdf_en_sdpError, \
							(Sdf_ty_s8bit*)
							"sdf_fn_uaExtractTransportTypeFromMedia: "
							"Failed to Init E2eQosDetails",\
							pErr);
#endif	
							return Sdf_co_fail;
						}
						pE2eQos = pDesQos->u.pE2eQos; 

						/*Direction Type*/
						pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
											&pSplitString2);

						if(Sdf_mc_strcasecmp(pSplitString1,"send")==0)
						{
							Sdf_mc_strdup(pE2eQos->dE2eSend.\
										pPrecondition,"qos");
							
							pE2eQos->dE2eSend.dQosStrength = dQosStrength;

						}
						else if(Sdf_mc_strcasecmp(pSplitString1,"recv")==0)
						{
							Sdf_mc_strdup(pE2eQos->dE2eRecv.\
										pPrecondition,"qos");
							
							pE2eQos->dE2eRecv.dQosStrength = dQosStrength;

						}
						else if(Sdf_mc_strcasecmp(pSplitString1,"sendrecv")==0)
						{
							Sdf_mc_strdup(pE2eQos->dE2eSend.\
										pPrecondition,"qos");
							
							pE2eQos->dE2eSend.dQosStrength = dQosStrength;

							Sdf_mc_strdup(pE2eQos->dE2eRecv.\
										pPrecondition,"qos");
							
							pE2eQos->dE2eRecv.dQosStrength = dQosStrength;

						}
						else if(Sdf_mc_strcasecmp(pSplitString1,"none")==0)
						{
							pErr->errCode = Sdf_en_malformedQosAttribute;
							return Sdf_co_fail;
						}
					}
					else /* Segmented */
					{
						Sdf_st_segmentedQosDetails	*pSegmentQos=Sdf_co_null;

						if(pMediaStream->pDesiredQos == Sdf_co_null)
						{
							if(sdf_ivk_uaInitQosAttrib(\
								&(pMediaStream->pDesiredQos),\
								Sdf_en_segmented,pErr) == Sdf_co_fail)
							{
								sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
								sdf_fn_setError(Sdf_en_majorErrors,\
									Sdf_en_sdpError, \
									(Sdf_ty_s8bit*)
									"sdf_fn_uaExtractTransportTypeFromMedia: "
									"Failed to Init QosAttrib",\
									pErr);
#endif	
								return Sdf_co_fail;
							}
						}
						pDesQos = pMediaStream->pDesiredQos;

						if(pDesQos->u.pSegmentedQos == Sdf_co_null)
						{
							if(sdf_ivk_uaInitSegmentedQosDetails(\
								&(pDesQos->u.pSegmentedQos), \
								pErr) == Sdf_co_fail)
							{
								sip_freeSdpAttr(pAttr);
								(void)sdf_ivk_uaFreeQosAttrib(pDesQos);
#ifdef SDF_ERROR			
								sdf_fn_setError(Sdf_en_majorErrors,\
									Sdf_en_sdpError, \
									(Sdf_ty_s8bit*)
									"sdf_fn_uaExtractTransportTypeFromMedia: "
									"Failed to Init SegQosDetails",\
									pErr);
#endif	
								return Sdf_co_fail;
							}
						}
						pSegmentQos = pDesQos->u.pSegmentedQos;

						if(Sdf_mc_strcasecmp(pSplitString1,"local")==0)
						{

							/*Direction Type*/
							pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
												&pSplitString2);

							if(Sdf_mc_strcasecmp(pSplitString1,"send")==0)
							{
								
								Sdf_mc_strdup(pSegmentQos->dLocalSend.\
										pPrecondition,"qos");
								pSegmentQos->dLocalSend.dQosStrength = \
											dQosStrength;
							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"recv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dLocalRecv.\
										pPrecondition,"qos");
								pSegmentQos->dLocalRecv.dQosStrength = \
									dQosStrength;

							}
							else if(Sdf_mc_strcasecmp(pSplitString1, \
										"sendrecv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dLocalSend.\
										pPrecondition,"qos");
								pSegmentQos->dLocalSend.dQosStrength = \
									dQosStrength;

								Sdf_mc_strdup( \
		 						pSegmentQos->dLocalRecv.pPrecondition,"qos");
								pSegmentQos->dLocalRecv.dQosStrength = \
										dQosStrength;

							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"none")!=0)
							{
								sip_freeSdpAttr(pAttr);
								pErr->errCode = Sdf_en_malformedQosAttribute;
								return Sdf_co_fail;
							}
						}
						else if(Sdf_mc_strcasecmp(pSplitString1,"remote")==0)
						{
							/*Direction Type*/
							pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
												&pSplitString2);

							if(Sdf_mc_strcasecmp(pSplitString1,"send")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dRemoteSend.\
										pPrecondition,"qos");
								pSegmentQos->dRemoteSend.dQosStrength =\
													dQosStrength;
							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"recv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dRemoteRecv.\
										pPrecondition,"qos");
								pSegmentQos->dRemoteRecv.dQosStrength =\
											dQosStrength;
							}
							else if(Sdf_mc_strcasecmp(pSplitString1, \
										"sendrecv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dRemoteSend.\
										pPrecondition,"qos");
								pSegmentQos->dRemoteSend.dQosStrength \
										= dQosStrength;
								Sdf_mc_strdup(pSegmentQos->dRemoteRecv.\
										pPrecondition,"qos");
								pSegmentQos->dRemoteRecv.dQosStrength =\
											dQosStrength;
							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"none")!=0)
							{
								sip_freeSdpAttr(pAttr);
								pErr->errCode = Sdf_en_malformedQosAttribute;
								return Sdf_co_fail;
								
							}
						}
					}
					break;
				}
				case Sdf_en_confQos:
				{
					/*Handle conf Qos*/
					Sdf_st_qosAttrib	*pConfirmedQos=Sdf_co_null;

					pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
										&pSplitString2);

					if(Sdf_mc_strcasecmp(pSplitString1,"e2e")==0)
					{
						Sdf_st_e2eQosDetails	*pE2eQos=Sdf_co_null;

						if(sdf_ivk_uaInitQosAttrib(\
							&(pMediaStream->pConfirmedQos),\
									Sdf_en_e2e,pErr) == Sdf_co_fail)
						{
							sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
							sdf_fn_setError(Sdf_en_majorErrors,\
							Sdf_en_sdpError, \
							(Sdf_ty_s8bit*)
							"sdf_fn_uaExtractTransportTypeFromMedia: "
							"Failed to Init QosAttrib",\
							pErr);
#endif	
							return Sdf_co_fail;
						}
						pConfirmedQos = pMediaStream->pConfirmedQos;

						if(sdf_ivk_uaInitE2eQosDetails(\
							&(pConfirmedQos->u.pE2eQos), \
							pErr) == Sdf_co_fail)
						{
							sip_freeSdpAttr(pAttr);
							(void)sdf_ivk_uaFreeQosAttrib(pConfirmedQos);
#ifdef SDF_ERROR			
							sdf_fn_setError(Sdf_en_majorErrors,\
							Sdf_en_sdpError, \
							(Sdf_ty_s8bit*)
							"sdf_fn_uaExtractTransportTypeFromMedia: "
							"Failed to Init E2eQosDetails",\
							pErr);
#endif	
							return Sdf_co_fail;
						}
						pE2eQos = pConfirmedQos->u.pE2eQos;
						
						pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
											&pSplitString2);
						
						/*Direction Type*/
						if(Sdf_mc_strcasecmp(pSplitString1,"send")==0)
						{
							Sdf_mc_strdup(pE2eQos->dE2eSend.\
								pPrecondition,"qos");
							
						}else if(Sdf_mc_strcasecmp(pSplitString1,"recv")==0)
						{
							Sdf_mc_strdup(pE2eQos->dE2eRecv.\
								pPrecondition,"qos");
						}else if(Sdf_mc_strcasecmp(pSplitString1,"sendrecv")==0)
						{
							Sdf_mc_strdup(pE2eQos->dE2eSend.\
								pPrecondition,"qos");
							Sdf_mc_strdup(pE2eQos->dE2eRecv.\
								pPrecondition,"qos");
						}
						else if(Sdf_mc_strcasecmp(pSplitString1,"none")!=0)
						{
							sip_freeSdpAttr(pAttr);
							pErr->errCode = Sdf_en_malformedQosAttribute;
							return Sdf_co_fail;
							
						}
					}
					else
					{
						Sdf_st_segmentedQosDetails	*pSegmentQos = Sdf_co_null;	
						if(pMediaStream->pConfirmedQos == Sdf_co_null)
						{
							if(sdf_ivk_uaInitQosAttrib(\
								&(pMediaStream->pConfirmedQos),\
										Sdf_en_segmented,\
										pErr) == Sdf_co_fail)
							{
								sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
								sdf_fn_setError(Sdf_en_majorErrors,\
									Sdf_en_sdpError, \
									(Sdf_ty_s8bit*)
									"sdf_fn_uaExtractTransportTypeFromMedia: "
									"Failed to Init QosAttrib",\
									pErr);
#endif	
								return Sdf_co_fail;
							}
						}
						pConfirmedQos = pMediaStream->pConfirmedQos;

						if(pConfirmedQos->u.pSegmentedQos == Sdf_co_null)
						{
							if(sdf_ivk_uaInitSegmentedQosDetails(\
								&(pConfirmedQos->u.pSegmentedQos),\
								pErr) == Sdf_co_fail)
							{
								sip_freeSdpAttr(pAttr);
								(void)sdf_ivk_uaFreeQosAttrib(pConfirmedQos);
#ifdef SDF_ERROR			
								sdf_fn_setError(Sdf_en_majorErrors,\
									Sdf_en_sdpError, \
									(Sdf_ty_s8bit*)
									"sdf_fn_uaExtractTransportTypeFromMedia: "
									"Failed to Init SegQosDetails",\
									pErr);
#endif	
								return Sdf_co_fail;
							}
						}
						pSegmentQos = pConfirmedQos->u.pSegmentedQos; 

						if(Sdf_mc_strcasecmp(pSplitString1,"local")==0)
						{

							pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
												&pSplitString2);

							/*Direction Type*/
							if(Sdf_mc_strcasecmp(pSplitString1,"send")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dLocalSend.\
									pPrecondition,"qos");

							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"recv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dLocalRecv.\
									pPrecondition,"qos");

							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"sendrecv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dLocalSend.\
									pPrecondition,"qos");
								Sdf_mc_strdup(pSegmentQos->dLocalRecv.\
									pPrecondition,"qos");
							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"none")!=0)
							{
								sip_freeSdpAttr(pAttr);
								pErr->errCode = Sdf_en_malformedQosAttribute;
								return Sdf_co_fail;
							}

						}
						else if (Sdf_mc_strcasecmp(pSplitString1,"remote")==0)
						{
							pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",\
												&pSplitString2);
							/*Direction Type*/
							if(Sdf_mc_strcasecmp(pSplitString1,"send")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dRemoteSend.\
									pPrecondition,"qos");

							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"recv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dRemoteRecv.\
									pPrecondition,"qos");
							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"sendrecv")==0)
							{
								Sdf_mc_strdup(pSegmentQos->dRemoteSend.\
									pPrecondition,"qos");
								Sdf_mc_strdup(pSegmentQos->dRemoteRecv.\
									pPrecondition,"qos");
							}
							else if(Sdf_mc_strcasecmp(pSplitString1,"none")!=0)
							{
								sip_freeSdpAttr(pAttr);
								pErr->errCode = Sdf_en_malformedQosAttribute;
								return Sdf_co_fail;
							}
						}
					}
					break;							
				}
				default:
				{
					dMalFormed = Sdf_co_true;
					break;
				}
			}
		}
		else if(Sdf_mc_strcasecmp(pAttrName,"sendonly")==0)
#else
		if(Sdf_mc_strcasecmp(pAttrName,"sendonly")==0)
#endif
		{
			pMediaStream->dDirectionAttrib = Sdf_en_sendonly;
		}
		else if(Sdf_mc_strcasecmp(pAttrName,"recvonly")==0)
		{
			pMediaStream->dDirectionAttrib = Sdf_en_recvonly;
		}
		else if(Sdf_mc_strcasecmp(pAttrName,"sendrecv")==0)
		{
			pMediaStream->dDirectionAttrib = Sdf_en_sendrecv;
		}
		else if(Sdf_mc_strcasecmp(pAttrName,"inactive")==0)
		{
			pMediaStream->dDirectionAttrib = Sdf_en_inactive;
		}
		else if(Sdf_mc_strcasecmp(pAttrName,"mid")==0)
		{
			pMediaStream->dMidPresence = Sdf_co_true;
			
			/*Extract mid*/
			if(sdp_getValueFromAttr(pAttr,&pAttrValue,\
					(SipError*)&pErr->stkErrCode)==SipFail)
			{
				sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaExtractTransportTypeFromMedia: "
					"Failed to get Attribute Value from Attribute",pErr);
#endif	
				return Sdf_co_fail;

			}
			(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pMediaStream->pMediaStreamId,pErr);
			pMediaStream->pMediaStreamId = Sdf_mc_strdupSdp(pAttrValue);
		}
		else if((Sdf_mc_strcasecmp(pAttrName,"rtpmap")!=0) &&\
					(Sdf_mc_strcasecmp(pAttrName,"fmtp")!=0))
		{
			/* Both rtpmap and fmtp are already handled.
			 * Add other attributes to slOtherAttribs */
			if(sdf_listAppend(&(pMediaStream->slOtherAttribs),\
						(Sdf_ty_pvoid)pAttr,pErr)==Sdf_co_fail)
			{
				sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaProcessMediaAttributes: "
					"Failed to Append Attribute",pErr);
#endif	
				return Sdf_co_fail;
			}
			/*Increment the RefCount of the attribute*/
			HSS_LOCKEDINCREF(pAttr->dRefCount);
		}
		
		/*If the Attribute Was Malformed Add it to slOtherAttribs*/
		if(dMalFormed)
		{
			if(sdf_listAppend(&(pMediaStream->slOtherAttribs),\
						(Sdf_ty_pvoid)pAttr,pErr)==Sdf_co_fail)
			{
				sip_freeSdpAttr(pAttr);
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaProcessMediaAttributes: "
					"Failed to Append Attribute",pErr);
#endif	
				return Sdf_co_fail;
			}
			/*Increment the RefCount of the attribute*/
			HSS_LOCKEDINCREF(pAttr->dRefCount);
		}
		/*Free Local Reference*/
		sip_freeSdpAttr(pAttr);

	}
	return Sdf_co_success;
}

	
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetSessionList
 **
 ** DESCRIPTION: 	The API will extract the session level List Attributes
 **					such as phone,email and bandwidth.It takes a function
 **					pointer and the list into which the attributes should
 **					be stored
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetSessionList
#ifdef ANSI_PROTO
	(SdpMessage	*pSdpMessage,
	Sdf_ty_slist *pslList,
	Sdf_ty_sdpListGetFuncPtr	pListGetFuncPtr,
	Sdf_ty_sdpCountFuncPtr 		pCountFunctPtr,
	Sdf_st_error	*pErr)
#else
	(pSdpMessage,pslList,pListGetFuncPtr,pCountFunctPtr,pErr)
	SdpMessage	*pSdpMessage;
	Sdf_ty_slist *pslList;
	Sdf_ty_sdpListGetFuncPtr	pListGetFuncPtr;
	Sdf_ty_sdpCountFuncPtr	pCountFunctPtr;
	Sdf_st_error	*pErr;
#endif
{
	Sdf_ty_s8bit	*pStr1=Sdf_co_null;
	Sdf_ty_s8bit	*pStr2=Sdf_co_null;
	Sdf_ty_u32bit	i=0,dCount=0;
	
	if(pCountFunctPtr(pSdpMessage,&dCount,\
		(SipError*)&pErr->stkErrCode)==SipFail)
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormSessionParams( ): "
			"Failed to get Count from SDP message", pErr);
#endif
		return Sdf_co_fail;
	}

	for(i=0;i< dCount;i++)
	{
		if(pListGetFuncPtr(pSdpMessage,&pStr1,i,\
			(SipError*)&pErr->stkErrCode)==SipFail)
		{
			if(pErr->stkErrCode != Sdf_en_noExistError)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_fn_uaGetSessionList( ): "
					"Failed to get String from SDP message", pErr);
#endif
				return Sdf_co_fail;
			}
		}
		
		Sdf_mc_strdup(pStr2,pStr1);

		/*Append it to the List of Email*/
		if(sdf_listAppend(pslList,(Sdf_ty_pvoid)pStr2,\
							pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaGetSessionList( ): "
				"Failed to append to List", pErr);
#endif
			return Sdf_co_fail;
		}
	}
	return Sdf_co_success;
}

#ifdef SDF_MEDIAGROUP
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaProcessGroupings
 **
 ** DESCRIPTION: 	The API will process the session level Group lines
 **					and also does some validations on the use of FID and
 **					LS groupings
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaProcessGroupings
#ifdef ANSI_PROTO
	(Sdf_ty_slist			*pslGroupList,
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_ty_u32bit			dSize,
	Sdf_st_error			*pErr)
#else
	(pslGroupList,pSessionParam,pErr)
	Sdf_ty_slist			*pslGroupList;
	Sdf_st_sessionParam		*pSessionParam;
	Sdf_ty_u32bit			dSize;
	Sdf_st_error			*pErr;
#endif
{
	Sdf_ty_u32bit	i=0,j=0;	
	/* 
 	 * Iterate through 
	 * all MediaStreams and check for the presence 
	 * of mid's in all of them
	 */
	/* 
	 * Absence of mid in any of the MediaStream will
	 * result in a failure and the Session Shall 
	 * be rejected
	 */
	for(i=0;i<pSessionParam->dNumMediaStreams;i++)
	{
		Sdf_st_mediaStream	*pMediaStream;
		
		pMediaStream	= pSessionParam->pMediaStreams[i];

		if(pMediaStream->dMidPresence == Sdf_co_false )
		{
			pErr->errCode = Sdf_en_mediaGroupError;
			return Sdf_co_fail;
		}
	}
	/*
 	* If grouping is by FID or LS,then iterate through 
	* all the media streams to check if all of 
	* them have a different IP or port, return failure
	* if two streams  have same IP and Port and 
	* is grouped by FID or LS. If the grouping is 
	* other than FID or LS then the validation are
	* skipped for it.
	*/
	pSessionParam->dNumGroups = dSize;

	/*Allocate Memory for the Grouping in SessionParam*/
	pSessionParam->pMediaStreamGroups\
			 = (Sdf_st_mediaStreamGroup**)sdf_memget(Sdf_mc_sdpMemId,\
							sizeof(Sdf_st_mediaStreamGroup*)*\
								dSize,pErr);
	/*Initialize all MediaStreamGroup*/
	for(i=0;i<dSize;i++)
	{
		/*w534*/if(sdf_ivk_uaInitMediaStreamGroup(&(pSessionParam->\
			pMediaStreamGroups[i]),pErr) == Sdf_co_fail) 
            { 
              Sdf_ty_u32bit k=0;

              for(k=0;k<i;k++)  
                (void)sdf_ivk_uaFreeMediaStreamGroup(pSessionParam->pMediaStreamGroups[k]);

              (void)sdf_memfree(Sdf_mc_freeMemId,\
			                    (Sdf_ty_pvoid*)&(pSessionParam->pMediaStreamGroups), \
			                    (Sdf_st_error*)Sdf_co_null);
		       return Sdf_co_fail;
            }
	}
	
	for(i=0;i<dSize;i++)
	{
		SdpAttr	*pAttr;
		Sdf_ty_s8bit	*pAttrValue = Sdf_co_null;
		Sdf_ty_s8bit	dTempAttrValue[Sdf_co_characterBufferSize];
		Sdf_ty_s8bit	*pSplitString1,*pSplitString2 = Sdf_co_null;
		
		Sdf_st_mediaStreamGroup	*pMediaStreamGroup;
		
		pMediaStreamGroup = pSessionParam->pMediaStreamGroups[i];
	
		if(sdf_listGetAt(pslGroupList,i,(Sdf_ty_pvoid*)&pAttr,\
							pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaProcessGroupings( ): "
				"Failed to Group Attribute from List", pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdp_getValueFromAttr(pAttr,&pAttrValue,\
						(SipError*)&pErr->stkErrCode)==SipFail)
		{
			if(pErr->stkErrCode == Sdf_en_noExistError)
			{
				/*This is a Empty Group Attribute
				 * Continue with Others
				 */
				 continue;
			}
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaProcessGroupings( ): "
				"Failed to Attribute Value", pErr);
#endif
			return Sdf_co_fail;
		}
		
		/*Parse the Attr Value*/
		Sdf_mc_strcpy(dTempAttrValue,pAttrValue);
		
		/*Extract Group*/

		pSplitString1 = Sdf_mc_strtokr(dTempAttrValue," ",&pSplitString2);

		
		if(Sdf_mc_strcasecmp(pSplitString1,"FID")==0)
		{
			pMediaStreamGroup->dGroupType = Sdf_en_groupFID;
		}
		else if(Sdf_mc_strcasecmp(pSplitString1,"LS")==0)
		{
			pMediaStreamGroup->dGroupType = Sdf_en_groupLS;
		}
		else if(Sdf_mc_strcasecmp(pSplitString1,"SRF")==0)
		{
			pMediaStreamGroup->dGroupType = Sdf_en_groupSRF;
		}
		else
		{
			pMediaStreamGroup->dGroupType = Sdf_en_groupOther;
			pMediaStreamGroup->pOtherGroupType =
				Sdf_mc_strdupSdp(pSplitString1);
		}
			
		/*Extract mid's*/
		pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",&pSplitString2);
		
		while (pSplitString1!=Sdf_co_null)
		{
			Sdf_ty_bool		dRejectSdp = Sdf_co_true;

			/*Iterarte through all media streams to match the mid*/
			for(j=0;j<pSessionParam->dNumMediaStreams;j++)
			{
				Sdf_st_mediaStream *pMediaStream;
	
				pMediaStream = pSessionParam->pMediaStreams[j];
				if(Sdf_mc_strcmp(pSplitString1,\
					pMediaStream->pMediaStreamId)==0)
				{
					Sdf_ty_u32bit	*pIndex = (Sdf_ty_u32bit*)sdf_memget(\
							Sdf_mc_sdpMemId, sizeof(Sdf_ty_u32bit), pErr);
					if(pIndex == Sdf_co_null)
					{
						pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR			
						sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
							(Sdf_ty_s8bit*)"sdf_fn_uaProcessGroupings( ): "
							"Malloc Failed", pErr);
#endif
						return Sdf_co_fail;
					}
					*pIndex = j;		
					/*Add this index into slMediaStreamIndex*/
					if(sdf_listAppend(&(pMediaStreamGroup->slMediaStreamIndex),\
										(Sdf_ty_pvoid)pIndex,pErr)==Sdf_co_fail)
					{
#ifdef SDF_ERROR				
						sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
							(Sdf_ty_s8bit*)"sdf_fn_uaProcessGroupings( ): "
							"Failed to Append index", pErr);
#endif
						return Sdf_co_fail;
					}
					dRejectSdp = Sdf_co_false;
				}
			}
			if(dRejectSdp)
			{
				pErr->errCode = Sdf_en_noMatchingMid;
#ifdef SDF_ERROR				
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
						(Sdf_ty_s8bit*)"sdf_fn_uaProcessGroupings( ): "
						"No Matching Mid", pErr);
#endif
					return Sdf_co_fail;
			}

			/*Extract mid's*/
			pSplitString1 = Sdf_mc_strtokr(pSplitString2," ",&pSplitString2);
		}
	}
	if(sdf_fn_uaValidateGroupings(pSessionParam,pSessionParam->pMediaStreamGroups,\
					pSessionParam->dNumGroups,pErr)==Sdf_co_false)
	{
		return Sdf_co_fail;
	}
	return Sdf_co_success;
}
#endif

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateConnectionLines
 **
 ** DESCRIPTION: 	The API validates the presence of Connection Line either
 **					at Session Level or at each Media Level
 *****************************************************************************/
Sdf_ty_bool sdf_fn_uaValidateConnectionLines
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam	*pSessionParams, 
	 Sdf_st_error			*pErr)
#else
	(pSessionParams,pErr)
	Sdf_st_sessionParam	*pSessionParams;
	Sdf_st_error		*pErr;
#endif	
{
	Sdf_ty_u32bit	i=0;
	if(pSessionParams->pConnection==Sdf_co_null)
	{
		/*now every media line should have a connection line*/
		for(i=0;i<pSessionParams->dNumMediaStreams;i++)
		{
			Sdf_st_mediaStream	*pMediaStream;
			Sdf_ty_u32bit		dCount=0;
			
			pMediaStream = pSessionParams->pMediaStreams[i];
            /* Code is added for SPR-3804 : c line is not mandatory for
             * rejected media streams */
            if ( 0 == pMediaStream->dPort) 
                continue;
            /* End of fix SPR- 3804 */
			
			if(sdf_listSizeOf(&(pMediaStream->slConnection),&dCount,\
						pErr)==Sdf_co_fail)
			{
			}
			if(dCount==0)
			{
				return Sdf_co_false;
			}
		}
	}
	return Sdf_co_true;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateIdleState
 **
 ** DESCRIPTION: 	The API does media state related validation in Idle state 
 **					when an incoming message is received
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateIdleState
#ifdef ANSI_PROTO
 ( SipMessage	*pMessage ,Sdf_st_callObject *pCallObject,Sdf_st_error *pErr)
#else
 (pMessage,pCallObject,pErr)
 SipMessage	*pMessage;
 Sdf_st_callObject	*pCallObject;
 Sdf_st_error	*pErr;
#endif
{
	(void)pMessage;
	(void)pCallObject;
	(void)pErr;
	return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateOfferSent
 **
 ** DESCRIPTION: 	The API does media state related validation in OfferSent
 **					state when an incoming message is received
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateOfferSentState
#ifdef ANSI_PROTO
 ( SipMessage	*pMessage ,Sdf_st_callObject *pCallObject,Sdf_st_error *pErr)
#else
 (pMessage,pCallObject,pErr)
 SipMessage	*pMessage;
 Sdf_st_callObject	*pCallObject;
 Sdf_st_error	*pErr;
#endif
{
	(void)pMessage;
	(void)pCallObject;
	(void)pErr;
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateOfferRecvState
 **
 ** DESCRIPTION: 	The API does media state related validation in OfferSent
 **					state when an incoming message is received
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateOfferRecvState
#ifdef ANSI_PROTO
 ( SipMessage	*pMessage ,Sdf_st_callObject *pCallObject,Sdf_st_error *pErr)
#else
 (pMessage,pCallObject,pErr)
 SipMessage	*pMessage;
 Sdf_st_callObject	*pCallObject;
 Sdf_st_error	*pErr;
#endif
{
	(void)pCallObject;
	if(sdf_ivk_uaIsSdpBodyPresent(pMessage,pErr)==Sdf_co_success)
		return Sdf_co_fail;
	else
		return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateEstablishedState
 **
 ** DESCRIPTION: 	The API does media state related validation in OfferSent
 **					state when an incoming message is received
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateEstablishedState
#ifdef ANSI_PROTO
 ( SipMessage	*pMessage ,Sdf_st_callObject *pCallObject,Sdf_st_error *pErr)
#else
 (pMessage,pCallObject,pErr)
 SipMessage	*pMessage;
 Sdf_st_callObject	*pCallObject;
 Sdf_st_error	*pErr;
#endif
{
	(void)pMessage;
	(void)pCallObject;
	(void)pErr;
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormMandatoryLines
 **
 ** DESCRIPTION: 	The API will form the mandatory lines of the SDP message.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormMandatoryLines
#ifdef ANSI_PROTO
	(SdpMessage *pSdpMessage,
	Sdf_st_sessionParam	*pSessionParam,
	Sdf_ty_conversionType dType,
	Sdf_st_error	*pErr)
#else
	(pSdpMessage,pSessionParam,dType,pErr)
	SdpMessage *pSdpMessage;
	Sdf_st_sessionParam	*pSessionParam;
	Sdf_ty_conversionType dType;
	Sdf_st_error	*pErr;
#endif
{
	SdpTime *pSdpTime=Sdf_co_null;
	SdpConnection *pConnection=Sdf_co_null;
	Sdf_ty_s8bit	*pStr=Sdf_co_null,dTemp[Sdf_co_smallBufferSize];
	Sdf_ty_bool   dConnAtMedia=Sdf_co_false;
	Sdf_ty_u32bit dConnListSize=0;
#ifdef SDF_UAB2B
  /*spr-4218 r-line*/
  Sdf_ty_u32bit dRepeatListSize=0;
#endif
	Sdf_ty_s32bit i;
	
	/* 	Set the version always. If it was present,
		the stack API frees that memory. Else its
		set now after this call */
	if (sdp_setVersion(pSdpMessage, Sdf_mc_strdupSdp("0"), \
		(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, \
			pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormMandatoryLines(): "
			"Failed Set Version\n",pErr);
#endif
		pErr->errCode = Sdf_en_sdpError;
		return Sdf_co_fail;
	}
	/*Set the Origin Line*/
	if(sdf_fn_uaSetOriginInfoInSdp(pSdpMessage,pSessionParam,dType,\
				pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, \
			pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormMandatoryLines(): "
			"Failed Set Origin Info\n",pErr);
#endif
		pErr->errCode = Sdf_en_sdpError;
		return Sdf_co_fail;

	}
	/*Set Connection*/
	if(sdf_ivk_uaGetSessionConnection(pSessionParam,&pConnection,\
						pErr)==Sdf_co_fail)
	{
		/* Check if at least on c= line is present in the media streams */

		for(i=0; i < (Sdf_ty_s32bit)(pSessionParam->dNumMediaStreams); i++)
		{
  
        /* Code is added for SPR-3804 : c line is not mandatory for
             * rejected media streams */
            if ( 0 == pSessionParam->pMediaStreams[i]->dPort) 
              continue;
            /* End of fix SPR- 3804 */
 
        (void)sdf_listSizeOf(&(pSessionParam->pMediaStreams[i]->slConnection),\
							&dConnListSize, pErr);

			if(0 >= dConnListSize)
			{
				pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit*)"sdf_fn_uaFormMandatoryLines( ): "
					"Failed to Session Conncetion", pErr);
#endif			
				return Sdf_co_fail;
			}	
		}
		if(0 == pSessionParam->dNumMediaStreams)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaFormMandatoryLines( ): "
				"Failed to Session Conncetion", pErr);
#endif			
			return Sdf_co_fail;
		}	
		dConnAtMedia=Sdf_co_true;
	}
	/* Set the session level connection only now. Media level will be
		put later.*/
	if(Sdf_co_false == dConnAtMedia && \
			sdp_setConnection(pSdpMessage,pConnection,\
			(SipError*)&pErr->stkErrCode)==SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaProcessGroupings( ): "
			"Failed to set Connection", pErr);
#endif			
		return Sdf_co_fail;
	}
	/*Free connection only is it exists at session level*/
	if(Sdf_co_false == dConnAtMedia)
		sip_freeSdpConnection(pConnection);
	
	/*Set Session Name*/
	pStr = Sdf_mc_strdupSdp(pSessionParam->pSessionName);

	if(sdp_setSession(pSdpMessage,pStr,\
				(SipError*)&pErr->stkErrCode)==SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormMandatoryLines( ): "
			"Failed to Set Session String", pErr);
#endif			
		return Sdf_co_fail;
	}
	/*Set Time*/
	if (sip_initSdpTime(&pSdpTime,\
		(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormMandatoryLines( ): "
			"Failed to Init SdpTime", pErr);
#endif			
		return Sdf_co_fail;
	}
	
	(void)sdf_fn_uaSprintf(dTemp,"%lu",pSessionParam->dSessionStart);

	pStr = Sdf_co_null;
	pStr = Sdf_mc_strdupSdp(dTemp);
	if (sdp_setStartInTime(pSdpTime,pStr,\
		(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormMandatoryLines( ): "
			"Failed set Start Time", pErr);
#endif			
		return Sdf_co_fail;
	}
	(void)sdf_fn_uaSprintf(dTemp,"%lu",pSessionParam->dSessionEnd);

	pStr = Sdf_co_null;
	pStr = Sdf_mc_strdupSdp(dTemp);

	if (sdp_setStopInTime(pSdpTime,pStr,\
		(SipError *)&(pErr->stkErrCode))==SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormMandatoryLines( ): "
			"Failed set Stop Time", pErr);
#endif			
		return Sdf_co_fail;
	}
	/*Add z line info into the time structure if exists*/
	if(pSessionParam->pTimeZoneInfo !=Sdf_co_null)
	{
		pStr = Sdf_co_null;
		pStr = Sdf_mc_strdupSdp(pSessionParam->pTimeZoneInfo);
		if(sdp_setZoneInTime(pSdpTime,pStr,\
								(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_fn_uaFormMandatoryLines( ): "
				"Failed set Zone info", pErr);
#endif			
			return Sdf_co_fail;

		}
	}
#ifdef SDF_UAB2B 
  	/*Add r lines info into the time structure if exists . SPR-4218*/
  {
    
    /*sip_listSizeOf(&(pSessionParam->slRepeatTime),&dRepeatListSize,(SipError*)&pErr->stkErrCode);*/
    sdf_listSizeOf(&(pSessionParam->slRepeatTime),&dRepeatListSize,pErr);
    if(dRepeatListSize > 0)
    {

    /*  if(sdf_fn_uaCloneStringList(&(pSdpTime->slRepeat),
            &(pSessionParam->slRepeatTime),
            pErr) == Sdf_co_fail)*/
      if(__sip_cloneSipStringList(&(pSdpTime->slRepeat),
            (SipList *)(&(pSessionParam->slRepeatTime)),
            (SipError*)&pErr->stkErrCode) == SipFail)
      {
        pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
        sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
            (Sdf_ty_s8bit*)"sdf_fn_uaFormMandatoryLines( ): "
            "Failed set slRepeatTime info", pErr);
#endif			
        return Sdf_co_fail;

      }
    }
  }
#endif
    if (sdp_insertTimeAtIndex(pSdpMessage, pSdpTime, (Sdf_ty_u32bit)0,\
		(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormMandatoryLines( ): "
			"Failed set Time", pErr);
#endif			
		return Sdf_co_fail;
	}
	sip_freeSdpTime(pSdpTime);
	
	return Sdf_co_success;
	
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaSetOriginInfoInSdp
 **
 ** DESCRIPTION: 	The API Forms the Origin Line and sets it into the 
 **					SdpMessage
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaSetOriginInfoInSdp
#ifdef ANSI_PROTO
	(SdpMessage	*pSdpMessage,
	 Sdf_st_sessionParam *pSessionParam,
	 Sdf_ty_conversionType	dType,
	 Sdf_st_error	*pErr)
#else
	(pSdpMessage,pSessionParam,dType,pErr)
	SdpMessage	*pSdpMessage;
	Sdf_st_sessionParam *pSessionParam;
	Sdf_ty_conversionType	dType;
	Sdf_st_error	*pErr;
#endif
{
	SdpOrigin	*pOrigin=Sdf_co_null;
	Sdf_ty_s8bit	*pDisplayName=Sdf_co_null;
	Sdf_ty_s8bit	*pStr=Sdf_co_null;
	Sdf_ty_s8bit	dTemp[Sdf_co_payloadSize];

	if(sip_initSdpOrigin(&pOrigin,	(SipError*)&(pErr->stkErrCode)) ==
					SipFail)
			return Sdf_co_fail;

	/*
	 * Set user name 
	 */
	if (pSessionParam->pSessionUser != Sdf_co_null)
		pDisplayName = Sdf_mc_strdupSdp(pSessionParam->pSessionUser);
	else
	{
		pDisplayName = Sdf_mc_strdupSdp((Sdf_ty_s8bit *)"anonymous");
	}

	if (sdp_setUserInOrigin(pOrigin,pDisplayName, \
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode ;
		sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
			(Sdf_ty_s8bit*)"sdf_fn_uaSetOriginInfoInSdp(): "
			"Failed to set User in Origin Structure",pErr);
#endif			
		return Sdf_co_fail;
	}
	/*
	 * Set session id 
	 */
	if(dType==Sdf_en_convertLocal)
		(void)sdf_fn_uaSprintf(dTemp,"%lu",pSessionParam->dLocalSessionId);
	else
		(void)sdf_fn_uaSprintf(dTemp,"%lu",pSessionParam->dRemoteSessionId);

		pStr = Sdf_mc_strdupSdp(dTemp);
	
	
	if (sdp_setSessionIdInOrigin(pOrigin, pStr,\
		(SipError*) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode ;
		sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
			(Sdf_ty_s8bit*)"sdf_fn_uaSetOriginInfoInSdp(): "
			"Failed to set session id in Origin Structure",pErr);
#endif			
		return Sdf_co_fail;
	}

	/*
	 * Set version 
	 */
	if(dType==Sdf_en_convertLocal)
		(void)sdf_fn_uaSprintf(dTemp,"%lu",pSessionParam->dLocalSessionVersion);
	else
		(void)sdf_fn_uaSprintf(dTemp,"%lu",pSessionParam->dRemoteSessionVersion);

		pStr = Sdf_mc_strdupSdp(dTemp);
	
	
	if (sdp_setVersionInOrigin(pOrigin, pStr,\
		(SipError*) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode ;
		sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
			(Sdf_ty_s8bit*)"sdf_fn_uaSetOriginInfoInSdp(): "
			"Failed to set Version in Origin Structure",pErr);
#endif			
		return Sdf_co_fail;
	}
	/*
	 * set net type
	 */
	pStr = Sdf_co_null;
	pStr = Sdf_mc_strdupSdp(Sdf_mc_networkType);

	if (sdp_setNetTypeInOrigin(pOrigin, pStr, \
		(SipError*) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode ;
		sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
			(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
			"Failed to set net type in Origin Structure",pErr);
#endif			
		return Sdf_co_fail;
	}

	/*
	 * set addr type
	 */
	pStr = Sdf_co_null;
	pStr = Sdf_mc_strdupSdp(pSessionParam->pOriginAddrType);

	if (sdp_setAddrTypeInOrigin(pOrigin, pStr,\
		(SipError*) &(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode ;
		sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
			(Sdf_ty_s8bit*)"sdf_fn_uaModifyOriginInSdpMessage (): "
			"Failed to set Addr type in Origin Structure",pErr);
#endif			
		return Sdf_co_fail;
	}

	/*
	 * set IP address in the o= line.
	 */
	pStr = Sdf_co_null;
	pStr = Sdf_mc_strdupSdp(pSessionParam->pOriginIp);

	if (sdp_setAddrInOrigin(pOrigin, pStr, \
		(SipError*)&(pErr->stkErrCode))== SipFail)
	{
		pErr->errCode = pErr->stkErrCode ;
		sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
			(Sdf_ty_s8bit*)"sdf_fn_uaSetOriginInfoInSdp(): "
			"Failed to set Addr in Origin Structure",pErr);
#endif			
		return Sdf_co_fail;
	}
	
	/*
	 * set SdpOrigin in the SdpMessage.
	 */
	if (sdp_setOrigin(pSdpMessage, pOrigin, \
		(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode ;
		sip_freeSdpOrigin(pOrigin);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
			(Sdf_ty_s8bit*)"sdf_fn_uaSetOriginInfoInSdp(): "
			"Failed to set SdpOrigin in SdpMessage",pErr);
#endif			
		return Sdf_co_fail;
	}
	/*free Origin*/
	sip_freeSdpOrigin(pOrigin);
	return	Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormOtherSessionAttributes
 **
 ** DESCRIPTION: 	The API Forms Sets the Attributes present
 **					slOtherSessionAttributes into SdpMessage.
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormOtherSessionAttributes
#ifdef ANSI_PROTO
	(SdpMessage 			*pSdpMessage,
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_st_error			*pErr)
#else
	(pSdpMessage,pSessionParam,pErr)
	SdpMessage 			*pSdpMessage;
	Sdf_st_sessionParam	*pSessionParam;
	Sdf_st_error			*pErr;
#endif
{
	Sdf_ty_u32bit	dIndex=0;
	Sdf_st_listIterator	dListIterator;

	(void)sdf_listInitIterator(&(pSessionParam->slOtherSessionAttributes),\
									&dListIterator,pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SdpAttr		*pSdpAttr=Sdf_co_null;
		pSdpAttr = (SdpAttr*)\
			(dListIterator.pCurrentElement->pData);
		if(sdp_insertAttrAtIndex(pSdpMessage,pSdpAttr,dIndex,\
					(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaFormOtherSessionAttributes(): "
				"Failed to Insert Attr",pErr);
#endif			
			return Sdf_co_fail;
		}
		dIndex++;
		(void)sdf_listNext(&dListIterator, pErr);

	}
	return Sdf_co_success;
}

#ifdef SDF_MEDIAGROUP
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormGroupAttribute
 **
 ** DESCRIPTION: 	The API Forms Group Attributes and sets them
 **					in SdpMessage
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormGroupAttribute
#ifdef ANSI_PROTO
	(SdpMessage *pSdpMessage,
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_st_error	*pErr)
#else
	(pSdpMessage,pSessionParam,pErr)
	SdpMessage *pSdpMessage;
	 Sdf_st_sessionParam	*pSessionParam;
	 Sdf_st_error	*pErr;
#endif
{
	Sdf_ty_u32bit	i=0;

	
	/*Form the Group Attrbutes*/
	for(i=0; i<pSessionParam->dNumGroups;i++)
	{
		Sdf_st_mediaStreamGroup	*pMediaStreamGroup;
		SdpAttr		*pSdpAttr;
		Sdf_st_listIterator	dListIterator;
		Sdf_ty_s8bit	dAttrValue[Sdf_co_smallBufferSize];
		Sdf_ty_s8bit	*pAttrValue = Sdf_co_null;
		
		pMediaStreamGroup = pSessionParam->pMediaStreamGroups[i];

		/*Initialize Sdp Attribute*/
		if(sip_initSdpAttr(&pSdpAttr,(SipError*)&pErr->stkErrCode) == SipFail)
				return Sdf_co_fail;
		
		/*Set the attribute name as "Group"*/
		/*w534*/(void)sdp_setNameInAttr(pSdpAttr,Sdf_mc_strdupSdp("group"),\
							(SipError*)&pErr->stkErrCode);
		
		if(pMediaStreamGroup->dGroupType==Sdf_en_groupFID)
			Sdf_mc_strcpy(dAttrValue,"FID");
		else if(pMediaStreamGroup->dGroupType==Sdf_en_groupLS)
		{
			Sdf_mc_strcpy(dAttrValue,"LS");
		}
		else if(pMediaStreamGroup->dGroupType==Sdf_en_groupSRF)
		{
			Sdf_mc_strcpy(dAttrValue,"SRF");
		}
		else
		{
			Sdf_mc_strcpy(dAttrValue,pMediaStreamGroup->pOtherGroupType);
		}
	
		/*Iterate through the integer list and form the attribute*/

		(void)sdf_listInitIterator(&(pMediaStreamGroup->slMediaStreamIndex),\
									&dListIterator,pErr);

		while(dListIterator.pCurrentElement!=Sdf_co_null)
		{
			Sdf_st_mediaStream	*pMediaStream=Sdf_co_null;

			Sdf_ty_u32bit		dMediaStreamIndex=0;

			/*Get the index of Media Stream*/
		
			dMediaStreamIndex = *((Sdf_ty_u32bit*)\
						dListIterator.pCurrentElement->pData);

			pMediaStream = pSessionParam->pMediaStreams[dMediaStreamIndex];

			Sdf_mc_strcat(dAttrValue," ");
			Sdf_mc_strcat(dAttrValue,pMediaStream->pMediaStreamId);
			(void)sdf_listNext(&dListIterator,pErr);
		}
		/*Now that the Attribute value is formed,set it into Attribute*/

		pAttrValue = Sdf_mc_strdupSdp(dAttrValue);

		if(sdp_setValueInAttr(pSdpAttr,pAttrValue,\
			(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaFormGroupAttributes(): "
				"Failed to Attr Value",pErr);
#endif	
			sip_freeSdpAttr(pSdpAttr);		
			return Sdf_co_fail;
		}
		/*Set the attribute into Sdp Message*/
		if(sdp_insertAttrAtIndex(pSdpMessage,pSdpAttr,i,\
								(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaFormGroupAttributes(): "
				"Failed to Attr Value",pErr);
#endif			
			sip_freeSdpAttr(pSdpAttr);		
			return Sdf_co_fail;
		}
		/*Free SdpAttr*/
		sip_freeSdpAttr(pSdpAttr);
	}/*End of for loop*/
	return Sdf_co_success;
}
#endif

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaUpdateDirectionAttributes
 **
 ** DESCRIPTION: 	The API Forms Direction Attributes and sets them
 **					in SdpMessage
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaUpdateDirectionAttribute
#ifdef ANSI_PROTO
	(SdpMessage *pSdpMessage,
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_st_error	*pErr)
#else
	(pSdpMessage,pSessionParam,pErr)
	SdpMessage *pSdpMessage;
	 Sdf_st_sessionParam	*pSessionParam;
	 Sdf_st_error	*pErr;
#endif
{
	Sdf_ty_u32bit	i=0;
	/*Form the direction Attribute*/
	if(pSessionParam->dDirectionAttrib!=Sdf_en_dirAttribNone)
	{
		SdpAttr	*pSdpAttr=Sdf_co_null;

		/*Form the Direction Attribute*/
		if(sdf_fn_uaFormDirectionAttribute(pSessionParam->dDirectionAttrib,\
										&pSdpAttr,pErr)==Sdf_co_fail)
		{
			pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaUpdateDirectionAttributes(): "
				"Failed to Form Direction Attribute",pErr);
#endif			
			return Sdf_co_fail;
		}
		/*Set the Direction attribute in Sdp Message*/
		if(sdp_insertAttrAtIndex(pSdpMessage,pSdpAttr,i,\
								(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = pErr->stkErrCode ;
			sip_freeSdpAttr(pSdpAttr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
				(Sdf_ty_s8bit*)"sdf_fn_uaUpdateDirectionAttributes(): "
				"Failed to insert Direction Attr",pErr);
#endif			
			return Sdf_co_fail;
		}
		/*Free SdpAttr*/
		sip_freeSdpAttr(pSdpAttr);
	}
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormMediaStreams
 **
 ** DESCRIPTION: 	The API Forms m lines from the MediaStreams Structre
 **					and sets them into the SdpMessage.
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormMediaStreams
#ifdef ANSI_PROTO
	(SdpMessage *pSdpMessage,
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_st_error			*pErr)
#else
	(pSdpMessage,pSessionParam,pErr)
	 SdpMessage *pSdpMessage;
	 Sdf_st_sessionParam	*pSessionParam;
	 Sdf_st_error			*pErr;
#endif
{
	Sdf_ty_u8bit	dIndex = 0;
#ifdef SDF_MEDIAGROUP
	Sdf_ty_bool		dGroupAttr=Sdf_co_false;
	
	/*Check if there is atleast one group attribute*/
	if(pSessionParam->dNumGroups !=0)
		dGroupAttr=Sdf_co_true;
#endif

	for(dIndex=0; dIndex < pSessionParam->dNumMediaStreams; dIndex++)
	{
		SdpMedia			*pSdpMedia=Sdf_co_null;
		Sdf_st_mediaStream	*pMediaStream=Sdf_co_null;
		
		if(sdf_ivk_uaGetMediaStreamAtIndex(pSessionParam,&pMediaStream, \
				dIndex,	pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream(): "
				"Failed to get MediaStream",pErr);
#endif			
			(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
			return Sdf_co_fail;
		}
		
		/*Form the SdpMedia*/
		if(sdf_ivk_uaFormSdpMediaFromMediaStream(pMediaStream,&pSdpMedia,\
				pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream(): "
				"Failed to form SdpMedia",pErr);
#endif			
			(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
			return Sdf_co_fail;
		}
#ifdef SDF_MEDIAGROUP
		if((dGroupAttr) && (pMediaStream->dMidPresence== Sdf_co_false))
		{
			Sdf_ty_u32bit	dCount=0;
			Sdf_ty_s8bit	dTemp[Sdf_co_smallBufferSize];
			(void)sdf_fn_uaSprintf(dTemp,"%s%s","a=mid:", \
								pMediaStream->pMediaStreamId);
			if(sdp_getAttrCountFromMedia(pSdpMedia,&dCount,\
							(SipError*)&(pErr->stkErrCode))==SipFail)
			{
				pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream(): "
					"Failed to get Attr Count",pErr);
#endif			
				(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
				return Sdf_co_fail;
			}
			if(sdp_insertAttrFromStringAtIndexInMedia(pSdpMedia,dTemp,dCount,\
								(SipError*)&(pErr->stkErrCode))==SipFail)
			{
				pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
					(Sdf_ty_s8bit*)"sdf_ivk_uaFormSdpMediaFromMediaStream(): "
					"Failed to Insert Attr",pErr);
#endif			
				(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
				return Sdf_co_fail;
			}
		}
#endif
		/*The complete SdpMedia is formed Now.
		 * Add it to the SdpMessage
		 */
		if(sdp_insertMediaAtIndex(pSdpMessage,pSdpMedia, (Sdf_ty_u32bit)dIndex,\
					(SipError*)&pErr->stkErrCode)==SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_dumpError( (Sdf_ty_s8bit*) "SDF_ERROR-\
				sdf_fn_uaFormMediaStreams(): \
				Failed to insert Media");
#endif		
			return Sdf_co_fail;
		}
		(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
		sip_freeSdpMedia(pSdpMedia);
	}
	return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormDirectionAttribute
 **
 ** DESCRIPTION: 	The API Forms the Sdp Attribute from Direction
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormDirectionAttribute
#ifdef ANSI_PROTO
	(Sdf_ty_dirAttrib	dDirection,
	 SdpAttr			**ppSdpAttr,
	 Sdf_st_error		*pErr)
#else
	(dDirection,ppSdpAttr,pErr)
	 Sdf_ty_dirAttrib	dDirection;
	 SdpAttr			**ppSdpAttr;
	 Sdf_st_error		*pErr;
#endif
{
	Sdf_ty_s8bit	dTemp[Sdf_co_characterBufferSize];
	Sdf_ty_s8bit	*pAttrName=Sdf_co_null;

	switch(dDirection)
	{
		case Sdf_en_sendonly:
		{
			Sdf_mc_strcpy(dTemp,"sendonly");
			break;
		}
		case Sdf_en_recvonly:
		{
			Sdf_mc_strcpy(dTemp,"recvonly");
			break;
		}
		case Sdf_en_sendrecv:
		{
			Sdf_mc_strcpy(dTemp,"sendrecv");
			break;
		}
		case Sdf_en_inactive:
		{
			Sdf_mc_strcpy(dTemp,"inactive");
			break;
		}
		default:
		{
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_fail;

		}
	}

	pAttrName = Sdf_mc_strdupSdp(dTemp);

	/*Initialize Attribute*/
	if(sip_initSdpAttr(ppSdpAttr,(SipError*)&pErr->stkErrCode) == SipFail)
    {
      (void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid *)&pAttrName,pErr);
	  return Sdf_co_fail;
    } 

	/*Set the direction type*/
	if(sdp_setNameInAttr(*ppSdpAttr,pAttrName,\
					(SipError*)&pErr->stkErrCode)==SipFail)
	{
    (void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid *)&pAttrName,pErr);
		sip_freeSdpAttr(*ppSdpAttr);
		pErr->errCode = Sdf_en_sdpAccessorError;
		return Sdf_co_fail;
	}
	pErr->errCode = Sdf_en_noUaError;
	return	Sdf_co_success;
}
#ifdef SDF_QOS
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormQosAttributesFromMediaStream
 **
 ** DESCRIPTION: 	The API Forms the qos attribute for the coressponfing
 **					Media Stream
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormQosAttributesFromMediaStream
#ifdef ANSI_PROTO
	(Sdf_st_mediaStream	*pMediaStream,
	 SdpMedia			*pSdpMedia,
	 Sdf_st_error		*pErr)
#else
	(pMediaStream,pSdpMedia,pErr)
	Sdf_st_mediaStream	*pMediaStream;
	SdpMedia			*pSdpMedia;
	Sdf_st_error		*pErr;
#endif
{
	/*Process Current Qos if present*/
	if(pMediaStream->pCurrentQos!=Sdf_co_null)
		/*w534*/if(sdf_fn_uaFormQosAttributes(pMediaStream->pCurrentQos,Sdf_en_currQos,\
							pSdpMedia,pErr) == Sdf_co_fail)
                   return Sdf_co_fail;
	/*Process Desired Qos if present*/
	if(pMediaStream->pDesiredQos!=Sdf_co_null)
		/*w534*/if(sdf_fn_uaFormQosAttributes(pMediaStream->pDesiredQos,Sdf_en_desQos,\
							pSdpMedia,pErr) == Sdf_co_fail)
                   return Sdf_co_fail;
	
	/*Process Confirmed Qos if present*/
	if(pMediaStream->pConfirmedQos!=Sdf_co_null)
		/*w534*/if(sdf_fn_uaFormQosAttributes(pMediaStream->pConfirmedQos,Sdf_en_confQos,\
								pSdpMedia,pErr) == Sdf_co_fail)
                   return Sdf_co_fail;

	return	Sdf_co_success;
										
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormQosAttributes
 **
 ** DESCRIPTION: 	Subfunction Invoked to form the qos attribute
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormQosAttributes
#ifdef	ANSI_PROTO
	(Sdf_st_qosAttrib	*pQosAttrib,
	 Sdf_ty_reservTypeInfo	dType,
	 SdpMedia				*pSdpMedia,
	 Sdf_st_error			*pErr)
#else
	(pQosAttrib,dType,pErr)
	Sdf_st_qosAttrib	*pQosAttrib;
	Sdf_ty_reservTypeInfo	dType;
	SdpMedia				*pSdpMedia;
	Sdf_st_error			*pErr;
#endif
{
	Sdf_ty_s8bit	dCurrQos[]="a=curr:";
	Sdf_ty_s8bit	dConfQos[]="a=conf:";
	Sdf_ty_s8bit	dDesQos[]="a=des:";
	Sdf_ty_s8bit	*pQosString=Sdf_co_null;

	Sdf_ty_s8bit	dTemp[Sdf_co_smallBufferSize];
	Sdf_ty_s8bit	dStrength[Sdf_co_smallBufferSize];

	Sdf_ty_qosType	dQosType;

	if(dType==Sdf_en_currQos)
		pQosString = dCurrQos;
	else if(dType == Sdf_en_confQos)
		pQosString = dConfQos;
	else
		pQosString = dDesQos;
		

	(void)sdf_ivk_uaGetQosType(pQosAttrib,&dQosType,pErr);

	switch (dQosType)	
	{
		case Sdf_en_e2e:
		{
			Sdf_st_e2eQosDetails *pQosDetails = Sdf_co_null;

			if(sdf_ivk_uaGetE2eQosDetails(pQosAttrib,&pQosDetails,pErr)\
				==Sdf_co_fail)
			{
				return Sdf_co_fail;
			}

			if((pQosDetails->dE2eSend.pPrecondition == Sdf_co_null)
				&&(pQosDetails->dE2eRecv.pPrecondition == Sdf_co_null))
			{
				if(dType == Sdf_en_desQos)
				{
					(void)sdf_fn_uaSprintf(dTemp,"%s %s ",pQosString,\
						"qos none e2e sendrecv");
				}
				else
				{
					(void)sdf_fn_uaSprintf(dTemp,"%s %s ",pQosString,\
						"qos e2e none");
				}
				if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
					pErr) == Sdf_co_fail)
				{
					(void)sdf_ivk_uaFreeE2eQosDetails(pQosDetails);
					return Sdf_co_fail;
				}
			}
			else if((dType == Sdf_en_desQos) && \
				((pQosDetails->dE2eSend.pPrecondition!=Sdf_co_null) &&\
					(pQosDetails->dE2eRecv.pPrecondition!=Sdf_co_null)) &&\
					(pQosDetails->dE2eSend.dQosStrength == \
					pQosDetails->dE2eRecv.dQosStrength))
			{
				(void)sdf_fn_uaGetStringForStrength(pQosDetails->\
							dE2eSend.dQosStrength,dStrength);

				(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",pQosString,\
							pQosDetails->dE2eSend.pPrecondition,dStrength,\
							"e2e sendrecv");
				if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
					pErr) == Sdf_co_fail)
				{
					(void)sdf_ivk_uaFreeE2eQosDetails(pQosDetails);
					return Sdf_co_fail;
				}
			}
			else if((dType != Sdf_en_desQos) &&\
				((pQosDetails->dE2eSend.pPrecondition!=Sdf_co_null) &&\
					(pQosDetails->dE2eRecv.pPrecondition!=Sdf_co_null)))
			{
				(void)sdf_fn_uaSprintf(dTemp,"%s %s %s",pQosString,\
							pQosDetails->dE2eSend.pPrecondition,"e2e sendrecv");
				if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
					pErr) == Sdf_co_fail)
				{
					(void)sdf_ivk_uaFreeE2eQosDetails(pQosDetails);
					return Sdf_co_fail;
				}
			}
			else
			{
				if(pQosDetails->dE2eSend.pPrecondition!=Sdf_co_null)
				{
					if(dType == Sdf_en_desQos)
					{
						(void)sdf_fn_uaGetStringForStrength(pQosDetails->\
							dE2eSend.dQosStrength,dStrength);

						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",pQosString,\
							pQosDetails->dE2eSend.pPrecondition,dStrength,\
							"e2e send");
					}
					else
					{
						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s",pQosString,\
							pQosDetails->dE2eSend.pPrecondition,"e2e send");
					}
					if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
						pErr) == Sdf_co_fail)
					{
						(void)sdf_ivk_uaFreeE2eQosDetails(pQosDetails);
						return Sdf_co_fail;
					}
							
				}
				if(pQosDetails->dE2eRecv.pPrecondition!=Sdf_co_null)
				{
					if(dType == Sdf_en_desQos)
					{
						(void)sdf_fn_uaGetStringForStrength(pQosDetails->\
							dE2eRecv.dQosStrength,dStrength);

						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",pQosString\
							,pQosDetails->dE2eRecv.pPrecondition,dStrength,\
							"e2e recv");
					}
					else
					{
						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s",pQosString,\
							pQosDetails->dE2eRecv.pPrecondition,"e2e recv");
					}
					if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
						pErr) == Sdf_co_fail)
					{
						(void)sdf_ivk_uaFreeE2eQosDetails(pQosDetails);
						return Sdf_co_fail;
					}

				}
			}
			/*Free Local Reference*/
			(void)sdf_ivk_uaFreeE2eQosDetails(pQosDetails);
			break;
		}
		case Sdf_en_segmented:
		{
			Sdf_st_segmentedQosDetails	*pQosDetails=Sdf_co_null;
			
			if(sdf_ivk_uaGetSegmentQosDetails(pQosAttrib,&pQosDetails,\
				pErr) == Sdf_co_fail)
			{
				return Sdf_co_fail;
			}
			
			/* Local */
			if((pQosDetails->dLocalSend.pPrecondition == Sdf_co_null)
				&&(pQosDetails->dLocalRecv.pPrecondition == Sdf_co_null))
			{
				if(dType == Sdf_en_desQos)
				{
					(void)sdf_fn_uaSprintf(dTemp,"%s %s",pQosString,\
					"qos none local sendrecv");
				}
				else
				{
					(void)sdf_fn_uaSprintf(dTemp,"%s %s",pQosString,"qos"
						" local none");
				}
				if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
					pErr) == Sdf_co_fail)
				{
					(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
					return Sdf_co_fail;
				}
			}
			else if((dType == Sdf_en_desQos) && \
				((pQosDetails->dLocalSend.pPrecondition!=Sdf_co_null) &&\
					(pQosDetails->dLocalRecv.pPrecondition!=Sdf_co_null)) &&\
					(pQosDetails->dLocalSend.dQosStrength == \
					pQosDetails->dLocalRecv.dQosStrength))
			{
				(void)sdf_fn_uaGetStringForStrength(pQosDetails->\
							dLocalSend.dQosStrength,dStrength);

				(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",pQosString,\
							pQosDetails->dLocalSend.pPrecondition,dStrength,\
							"local sendrecv");
				if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
					pErr) == Sdf_co_fail)
				{
					(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
					return Sdf_co_fail;
				}
			}
			else if((dType != Sdf_en_desQos) &&\
				((pQosDetails->dLocalSend.pPrecondition!=Sdf_co_null) &&\
					(pQosDetails->dLocalRecv.pPrecondition!=Sdf_co_null)))
			{
				(void)sdf_fn_uaSprintf(dTemp,"%s %s %s",pQosString,\
							pQosDetails->dLocalSend.pPrecondition,"local sendrecv");
				if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
					pErr) == Sdf_co_fail)
				{
					(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
					return Sdf_co_fail;
				}
			}
			else
			{
				if(pQosDetails->dLocalSend.pPrecondition!=Sdf_co_null)
				{
					if(dType == Sdf_en_desQos)
					{
						(void)sdf_fn_uaGetStringForStrength(pQosDetails->\
							dLocalSend.dQosStrength,dStrength);

						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",\
							pQosString,pQosDetails->dLocalSend.pPrecondition,\
							dStrength,"local send");
					}
					else
					{
						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",pQosString,\
							pQosDetails->dLocalSend.pPrecondition,\
							"local send");
					}
					if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
						pErr) == Sdf_co_fail)
					{
						(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
						return Sdf_co_fail;
					}
				}
				if(pQosDetails->dLocalRecv.pPrecondition!=Sdf_co_null)
				{
					if(dType == Sdf_en_desQos)
					{
						(void)sdf_fn_uaGetStringForStrength(pQosDetails->\
							dLocalRecv.dQosStrength,dStrength);

						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",\
							pQosString,pQosDetails->dLocalRecv.pPrecondition,\
							dStrength,"local recv");
						
					}
					else
					{
						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",pQosString,\
							pQosDetails->dLocalSend.pPrecondition,\
							"local recv");
					}
					if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
						pErr) == Sdf_co_fail)
					{
						(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
						return Sdf_co_fail;
					}
				}
			}

			/* Remote */
			if((pQosDetails->dRemoteSend.pPrecondition == Sdf_co_null)
				&&(pQosDetails->dRemoteRecv.pPrecondition == Sdf_co_null))
			{
				if(dType == Sdf_en_desQos)
				{
					(void)sdf_fn_uaSprintf(dTemp,"%s %s",pQosString,\
							"qos none remote sendrecv");
				}
				else
				{
					(void)sdf_fn_uaSprintf(dTemp,"%s %s",pQosString,"qos"
						" remote none");
				}
				if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
					pErr) == Sdf_co_fail)
				{
					(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
					return Sdf_co_fail;
				}
			}
			else if((dType == Sdf_en_desQos) && \
				((pQosDetails->dRemoteSend.pPrecondition!=Sdf_co_null) &&\
					(pQosDetails->dRemoteRecv.pPrecondition!=Sdf_co_null)) &&\
					(pQosDetails->dRemoteSend.dQosStrength == \
					pQosDetails->dRemoteRecv.dQosStrength))
			{
				(void)sdf_fn_uaGetStringForStrength(pQosDetails->\
							dRemoteSend.dQosStrength,dStrength);

				(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",pQosString,\
							pQosDetails->dRemoteSend.pPrecondition,dStrength,\
							"remote sendrecv");
				if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
					pErr) == Sdf_co_fail)
				{
					(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
					return Sdf_co_fail;
				}
			}
			else if((dType != Sdf_en_desQos) &&\
				((pQosDetails->dRemoteSend.pPrecondition!=Sdf_co_null) &&\
					(pQosDetails->dRemoteRecv.pPrecondition!=Sdf_co_null)))
			{
				(void)sdf_fn_uaSprintf(dTemp,"%s %s %s",pQosString,\
							pQosDetails->dRemoteSend.pPrecondition,"remote sendrecv");
				if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
					pErr) == Sdf_co_fail)
				{
					(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
					return Sdf_co_fail;
				}
			}
			else
			{
				if(pQosDetails->dRemoteSend.pPrecondition!=Sdf_co_null)
				{
					if(dType == Sdf_en_desQos)
					{
						(void)sdf_fn_uaGetStringForStrength(pQosDetails->\
							dRemoteSend.dQosStrength,dStrength);

						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",\
							pQosString,pQosDetails->dRemoteSend.pPrecondition,\
							dStrength,"remote send");
					}
					else
					{
						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s",pQosString,\
							pQosDetails->dRemoteSend.pPrecondition,\
							"remote send");
					}
					if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
						pErr) == Sdf_co_fail)
					{
						(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
						return Sdf_co_fail;
					}
				}
				if(pQosDetails->dRemoteRecv.pPrecondition!=Sdf_co_null)
				{
					if(dType == Sdf_en_desQos)
					{
						(void)sdf_fn_uaGetStringForStrength(pQosDetails->\
							dRemoteRecv.dQosStrength,dStrength);

						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s %s",\
							pQosString,pQosDetails->dRemoteRecv.pPrecondition,\
							dStrength,"remote recv");
					}
					else
					{
						(void)sdf_fn_uaSprintf(dTemp,"%s %s %s",pQosString,\
							pQosDetails->dRemoteSend.pPrecondition,\
							"remote recv");
					}

					if(sdf_fn_uaFormAttributeFromString(dTemp,pSdpMedia,\
						pErr) == Sdf_co_fail)
					{
						(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
						return Sdf_co_fail;
					}
				}
			}
			/*Free Local Reference*/
			(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosDetails);
			break;
		}
		default:
		{
			/* Do nothing - to remove the warning */
		}
	}
	return Sdf_co_success;
}
#endif

/*****************************************************************************
** FUNCTION: sdf_fn_uaInitMediaStateTable
**
** DESCRIPTION: This API will initialize the Media State Transition table
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitMediaStateTable
#ifdef ANSI_PROTO
(void)
#else
(void)
#endif
{
	/*Initialize dMediaCsmNxtStateOnMsgSent Table*/
	
	/*State: Sdf_en_idleMedia*/	
	dMediaCsmNxtStateOnMsgSent[Sdf_en_invite][Sdf_en_idleMedia]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_cancel][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_ack][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_idleMedia]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_options][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_register][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_info][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgSent[Sdf_en_notify][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_subscribe][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgSent[Sdf_en_message][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
#endif
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgSent[Sdf_en_refer][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
#endif
	dMediaCsmNxtStateOnMsgSent[Sdf_en_unknownRequest][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_rpr][Sdf_en_idleMedia]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_prack][Sdf_en_idleMedia]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_comet][Sdf_en_idleMedia]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_update][Sdf_en_idleMedia]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_provisionalResp][Sdf_en_idleMedia]=\
										Sdf_en_offerSent;

	/*State: Sdf_en_offerSent*/	
	dMediaCsmNxtStateOnMsgSent[Sdf_en_invite][Sdf_en_offerSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_cancel][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_ack][Sdf_en_offerSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_options][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_register][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_info][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgSent[Sdf_en_notify][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_subscribe][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgSent[Sdf_en_message][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
#endif
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgSent[Sdf_en_refer][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
#endif
	dMediaCsmNxtStateOnMsgSent[Sdf_en_unknownRequest][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_rpr][Sdf_en_offerSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_prack][Sdf_en_offerSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_comet][Sdf_en_offerSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_update][Sdf_en_offerSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_provisionalResp][Sdf_en_offerSent]=\
										Sdf_en_invalidMediaState;

	/*State: Sdf_en_offerChangeSent*/	
	dMediaCsmNxtStateOnMsgSent[Sdf_en_invite][Sdf_en_offerChangeSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_cancel][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_ack][Sdf_en_offerChangeSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_offerChangeSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_options][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_register][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_info][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgSent[Sdf_en_notify][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_subscribe][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgSent[Sdf_en_message][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
#endif
	dMediaCsmNxtStateOnMsgSent[Sdf_en_unknownRequest][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
#ifdef SIP_RPR										
	dMediaCsmNxtStateOnMsgSent[Sdf_en_rpr][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_prack][Sdf_en_offerChangeSent]=\
										Sdf_en_invalidMediaState;
#endif										
	dMediaCsmNxtStateOnMsgSent[Sdf_en_comet][Sdf_en_offerChangeSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_update][Sdf_en_offerChangeSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_provisionalResp][Sdf_en_offerChangeSent]=\
										Sdf_en_invalidMediaState;
									

	/*State: Sdf_en_offerRecv*/	
	dMediaCsmNxtStateOnMsgSent[Sdf_en_invite][Sdf_en_offerRecv]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_cancel][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_ack][Sdf_en_offerRecv]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_offerRecv]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_offerRecv]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_options][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_register][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_info][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgSent[Sdf_en_notify][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_subscribe][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgSent[Sdf_en_message][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#endif
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgSent[Sdf_en_refer][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#endif
	dMediaCsmNxtStateOnMsgSent[Sdf_en_unknownRequest][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#ifdef SIP_RPR										
	dMediaCsmNxtStateOnMsgSent[Sdf_en_rpr][Sdf_en_offerRecv]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_prack][Sdf_en_offerRecv]=\
										Sdf_en_establishedMedia;
#endif										
	dMediaCsmNxtStateOnMsgSent[Sdf_en_comet][Sdf_en_offerRecv]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_update][Sdf_en_offerRecv]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_provisionalResp][Sdf_en_offerRecv]=\
										Sdf_en_establishedMedia;

	/*State: Sdf_en_offerChangeRecv*/	
	dMediaCsmNxtStateOnMsgSent[Sdf_en_invite][Sdf_en_offerChangeRecv]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_cancel][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_ack][Sdf_en_offerChangeRecv]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_offerChangeRecv]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_offerChangeRecv]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_options][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_register][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_info][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgSent[Sdf_en_notify][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_subscribe][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgSent[Sdf_en_message][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#endif
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgSent[Sdf_en_refer][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#endif
	dMediaCsmNxtStateOnMsgSent[Sdf_en_unknownRequest][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#ifdef SIP_RPR										
	dMediaCsmNxtStateOnMsgSent[Sdf_en_rpr][Sdf_en_offerChangeRecv]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_prack][Sdf_en_offerChangeRecv]=\
										Sdf_en_establishedMedia;
#endif										
	dMediaCsmNxtStateOnMsgSent[Sdf_en_comet][Sdf_en_offerChangeRecv]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_update][Sdf_en_offerChangeRecv]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_provisionalResp][Sdf_en_offerChangeRecv]=\
										Sdf_en_establishedMedia;
	


	/*State: Sdf_en_establishedMedia*/	
	dMediaCsmNxtStateOnMsgSent[Sdf_en_invite][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_cancel][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_ack][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_bye][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalSuccess][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_finalFailure][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_options][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_register][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_info][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgSent[Sdf_en_notify][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_subscribe][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgSent[Sdf_en_message][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
#endif
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgSent[Sdf_en_refer][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
#endif
	dMediaCsmNxtStateOnMsgSent[Sdf_en_unknownRequest][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
#ifdef SIP_RPR										
	dMediaCsmNxtStateOnMsgSent[Sdf_en_rpr][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_prack][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeSent;
#endif										
	dMediaCsmNxtStateOnMsgSent[Sdf_en_comet][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_update][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgSent[Sdf_en_provisionalResp][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeSent;

	/*Initialize dMediaCsmNxtStateOnMsgRecv */
	
	/*State: Sdf_en_idleMedia*/	
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_invite][Sdf_en_idleMedia]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_cancel][Sdf_en_idleMedia]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_ack][Sdf_en_idleMedia]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_bye][Sdf_en_idleMedia]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalSuccess][Sdf_en_idleMedia]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalFailure][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_options][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_register][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_info][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_notify][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_subscribe][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_message][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
#endif
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_refer][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
#endif
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_unknownRequest][Sdf_en_idleMedia]=\
										Sdf_en_idleMedia;
#ifdef SIP_RPR										
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_rpr][Sdf_en_idleMedia]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_prack][Sdf_en_idleMedia]=\
										Sdf_en_offerRecv;
#endif										
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_comet][Sdf_en_idleMedia]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_update][Sdf_en_idleMedia]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_provisionalResp][Sdf_en_idleMedia]=\
										Sdf_en_offerRecv;

	/*State: Sdf_en_offerSent*/	
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_invite][Sdf_en_offerSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_cancel][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_ack][Sdf_en_offerSent]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_bye][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalSuccess][Sdf_en_offerSent]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalFailure][Sdf_en_offerSent]=\
										Sdf_en_idleMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_options][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_register][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_info][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_notify][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_subscribe][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_message][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
#endif
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_refer][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
#endif
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_unknownRequest][Sdf_en_offerSent]=\
										Sdf_en_offerSent;
#ifdef SIP_RPR										
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_rpr][Sdf_en_offerSent]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_prack][Sdf_en_offerSent]=\
										Sdf_en_establishedMedia;
#endif										
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_comet][Sdf_en_offerSent]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_update][Sdf_en_offerSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_provisionalResp][Sdf_en_offerSent]=\
										Sdf_en_establishedMedia;

	/*State: Sdf_en_offerChangeSent*/	
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_invite][Sdf_en_offerChangeSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_cancel][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_ack][Sdf_en_offerChangeSent]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_bye][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalSuccess][Sdf_en_offerChangeSent]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalFailure][Sdf_en_offerChangeSent]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_options][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_register][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_info][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_notify][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_subscribe][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_message][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
#endif
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_refer][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
#endif
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_unknownRequest][Sdf_en_offerChangeSent]=\
										Sdf_en_offerChangeSent;
#ifdef SIP_RPR										
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_rpr][Sdf_en_offerChangeSent]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_prack][Sdf_en_offerChangeSent]=\
										Sdf_en_establishedMedia;
#endif										
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_comet][Sdf_en_offerChangeSent]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_update][Sdf_en_offerChangeSent]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_provisionalResp][Sdf_en_offerChangeSent]=\
										Sdf_en_establishedMedia;
									

	/*State: Sdf_en_offerRecv*/	
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_invite][Sdf_en_offerRecv]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_cancel][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_ack][Sdf_en_offerRecv]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_bye][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalSuccess][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalFailure][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_options][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_register][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_info][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_notify][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_subscribe][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_message][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#endif
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_refer][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#endif
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_unknownRequest][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#ifdef SIP_RPR									
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_rpr][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_prack][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
#endif										
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_comet][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_update][Sdf_en_offerRecv]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_provisionalResp][Sdf_en_offerRecv]=\
										Sdf_en_offerRecv;

	/*State: Sdf_en_offerChangeRecv*/	
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_invite][Sdf_en_offerChangeRecv]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_cancel][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_ack][Sdf_en_offerChangeRecv]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_bye][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalSuccess][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalFailure][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_options][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_register][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_info][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_refer][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#endif
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_unknownRequest][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_subscribe][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_notify][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_message][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#endif
#ifdef SIP_RPR
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_rpr][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_prack][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
#endif										
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_comet][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_update][Sdf_en_offerChangeRecv]=\
										Sdf_en_invalidMediaState;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_provisionalResp][Sdf_en_offerChangeRecv]=\
										Sdf_en_offerChangeRecv;
	


	/*State: Sdf_en_establishedMedia*/	
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_invite][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_cancel][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_ack][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_bye][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalSuccess][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_finalFailure][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_options][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_register][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_info][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_unknownRequest][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
#ifdef SIP_IMPP
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_subscribe][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_notify][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
#endif
#ifdef SDF_IM
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_message][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
#endif
#ifdef SDF_REFER
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_refer][Sdf_en_establishedMedia]=\
										Sdf_en_establishedMedia;
#endif
#ifdef SIP_RPR
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_rpr][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_prack][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeRecv;
#endif										
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_comet][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_update][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeRecv;
	dMediaCsmNxtStateOnMsgRecv[Sdf_en_provisionalResp][Sdf_en_establishedMedia]=\
										Sdf_en_offerChangeRecv;
	
	return	Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaMapCodecNumberToName
 **
 ** DESCRIPTION: 	This API takes in the codec in u8bit form and returns the 
 **                 corresponding enum value of type Sdf_ty_codecName.
 **                 e.g. Sdf_en_PCMA for the input 8.
 *****************************************************************************/
Sdf_ty_codecName sdf_fn_uaMapCodecNumberToName
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit dCodec,
	Sdf_st_error   *pErr)
#else
	(dCodec, pErr)
	Sdf_ty_u32bit dCodec;
	Sdf_st_error  *pErr;
#endif
{
	Sdf_ty_codecName  dCodecName;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Entering sdf_fn_uaMapCodecNumberToName");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError( (Sdf_ty_s8bit*) "SDF_ERROR-\
		sdf_fn_uaMapPayloadFromCodec(): \
		Invalid pError Param Passed");
#endif		
		return Sdf_en_otherCodec;
	}
#endif
#ifdef SDF_UAB2B
/*TODO - SPP 3851*/
    if (dCodec < Sdf_co_codecTableSize)
    {

        dCodecName = sdf_gl_dCodecProfile[dCodec].dCodecName;
        if (Sdf_en_unassigned == dCodecName)
        {
            dCodecName = Sdf_en_unassigned;
        }
    }
    else
      dCodecName = Sdf_en_otherCodec;
#else
	if (dCodec < Sdf_co_codecTableSize)
		dCodecName = sdf_gl_dCodecProfile[dCodec].dCodecName;
	else
		dCodecName = Sdf_en_otherCodec;
#endif
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
		"Exiting sdf_fn_uaMapCodecNumberToName");
	pErr->errCode = Sdf_en_noUaError;
	return dCodecName;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaMapCodecEnumToString
 **
 ** DESCRIPTION: 	This API takes in the codec enumeration and returns the 
 **                 corresponding String
 **                 e.g. "PCMA" for Sdf_en_PCMA.
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaMapCodecEnumToString
#ifdef ANSI_PROTO
	(Sdf_ty_codecName	dCodec,
	 Sdf_ty_s8bit		*pCodecString,
	 Sdf_st_error		*pErr)
#else
	(dCodec,pCodecString,pErr)
	Sdf_ty_codecName	dCodec;
	Sdf_ty_s8bit		*pCodecString;
	Sdf_st_error		*pErr;
#endif
{
	Sdf_ty_s8bit	dTemp[Sdf_co_characterBufferSize];
	Sdf_ty_s8bit	*pString1=Sdf_co_null,*pString2=Sdf_co_null;

	if (dCodec < Sdf_co_codecTableSize)
    {
#ifdef SDF_UAB2B
        /*TODO SPR 3851*/
        Sdf_ty_s8bit *pTempCodecName = \
            sdf_gl_dCodecProfile[dCodec].dCodecString;
        if (0 != Sdf_mc_strcmp("unassigned",pTempCodecName))
        {
            Sdf_mc_strcpy(dTemp, sdf_gl_dCodecProfile[dCodec].dCodecString);
            pString1 = Sdf_mc_strtokr(dTemp,"/", &pString2);
            Sdf_mc_strcpy(pCodecString, pString1);

            return Sdf_co_success;
        }
#else
        Sdf_mc_strcpy(dTemp, sdf_gl_dCodecProfile[dCodec].dCodecString);
		pString1 = Sdf_mc_strtokr(dTemp,"/", &pString2);
		Sdf_mc_strcpy(pCodecString, pString1);
#endif        
    }
	else
    {
		pErr->errCode = Sdf_en_sdpError;
		return Sdf_co_fail;
    }
	return	Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaMapCodecNumToRtpmapString
 **
 ** DESCRIPTION: 	
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaMapCodecNumToRtpmapString
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit	dCodecNum,
	 Sdf_ty_s8bit		*pRtpString,
	 Sdf_st_error		*pErr)
#else
	(dCodecNum,ppRtpString,pErr)
	Sdf_ty_codecName	dCodec;
	Sdf_ty_s8bit		**ppRtpString;
	Sdf_st_error		*pErr;
#endif
{
	if (dCodecNum < Sdf_co_codecTableSize)
    {
#ifdef SDF_UAB2B
        /*TODO SPR 3851*/
        Sdf_ty_codecName dCodecName = sdf_gl_dCodecProfile[dCodecNum].dCodecName;
        if (Sdf_en_unassigned != dCodecName)
        {
            Sdf_mc_strcpy(pRtpString, sdf_gl_dCodecProfile[dCodecNum].dCodecString);
            return	Sdf_co_success;
        }
#else
        Sdf_mc_strcpy(pRtpString, sdf_gl_dCodecProfile[dCodecNum].dCodecString);
		return	Sdf_co_success;
#endif        
    }
	pErr->errCode = Sdf_en_sdpError;
	return Sdf_co_fail;
}

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaChangeMediaState
 **
 ** DESCRIPTION: 	The API changes the media state based on the current state 
 **					and the SDP message in outgoing message. 
 **	NOTE:			We should handle PRACK and ACK Specially.This is because
 ** 				these are the only request which can contain answers.
 *****************************************************************************/
 Sdf_ty_retVal	sdf_fn_uaChangeMediaState
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObject,
	 Sdf_ty_mediaStateChange	*pMediaChange,
	 Sdf_st_msgInfo		dMessageInfo,
	 SipMessage			*pSipMsg,
	 Sdf_ty_msgMode		dMode,
	 Sdf_st_error		*pErr)
#else
	(pCallObject, pMediaChange, dMessageInfo, pSipMsg, dMode, pErr)
	Sdf_st_callObject	*pCallObject;
	Sdf_ty_mediaStateChange	*pMediaChange,
	Sdf_st_msgInfo		dMessageInfo;
	SipMessage			*pSipMsg;
	Sdf_ty_msgMode		dMode;
	Sdf_st_error		*pErr;
#endif
{
	Sdf_ty_mediaState	dState;
	Sdf_ty_retVal		dSdpPresence = Sdf_co_fail;
	en_SipMessageType	dType;
	Sdf_ty_messageType	dMessageType;
	
	/*Get the current Media State*/
	dState = pCallObject->pMediaInfo->dMediaState;

	*pMediaChange = Sdf_en_noMediaChange;

	dMessageType =  dMessageInfo.dMsgType;
	
	if((dMessageType != Sdf_en_invite) && (dMessageType != Sdf_en_update) &&
		(dMessageType != Sdf_en_prack) && (dMessageType != Sdf_en_comet) &&
		(dMessageType != Sdf_en_ack))
	{
		/*No State Change Needed*/
		return Sdf_co_success;
	}
	/*Check if Sdp is Present*/
	dSdpPresence = sdf_ivk_uaIsSdpBodyPresent(pSipMsg,pErr);
	
	dType = dMessageInfo.dReqRespType;

	/*If the Message is a Response Override the dMessageType*/	
	if(dType == SipMessageResponse)
	{
		Sdf_ty_u16bit	dRespCode=0;

		dRespCode = dMessageInfo.dResponse;

		if( dRespCode==100)
		{
			/*No State Change Needed*/
			return Sdf_co_success;
		}
		else if( dRespCode>100 && dRespCode <200)
		{
			/* We check if the Media State is established,If so,we also
			 * check if an answer was already received before.
			 * Normally the answers are echoed in the prov responses.This
			 * should be ignored.
			 */
			if(dState == Sdf_en_establishedMedia)
			{
				if(pCallObject->pMediaInfo->dProvRespWithSdp == Sdf_co_true)
				{
					/*We dont have to revert the Flag here. This is because
					 * there could be still more RPR's coming and all those
					 * should also be ignored
					 */
					return Sdf_co_success;
				}
			}

			/*Check if the Incoming Message is a RPR*/
#ifdef SIP_RPR			
			if(sdf_fn_uaIsMessageRPR(pSipMsg,pErr)==0)
			{
				dMessageType = Sdf_en_rpr;
			}
			else
#endif			
			{
				dMessageType = Sdf_en_provisionalResp;
			}
		}
		else if(dRespCode>=200 && dRespCode <300)
		{
			/* Handle Special Case:
			 *	1. When a 200 to INVITE Goes after Prov Response and ProvResp had an 
			 *		SDP then MediaState Transition should be IGNORED
			 *	2. When a 200 to INVITE is received after ProvResponse and 
			 *	ProvResponse had a SDP then Media State Transition should be IGNORED
			 */
			if(dState == Sdf_en_establishedMedia)
			{
				if( (pCallObject->pMediaInfo->dProvRespWithSdp == Sdf_co_true) && 
						(dMessageType == Sdf_en_invite))
				{
					/*We Revert back the flag now. This is because the
					 * transaction is complete now
					 */
					pCallObject->pMediaInfo->dProvRespWithSdp = Sdf_co_false;
					return Sdf_co_success;
				}
			}
			dMessageType= Sdf_en_finalSuccess;
		}
		else
		{
			dMessageType= Sdf_en_finalFailure;
		}
	}
		
	if(dMessageType == Sdf_en_provisionalResp)
	{
		/* If an Unreliable 1xx response contains the SDP message, UATK
		 * need not change its states.  either OFFER or ANSWER received
		 * through Unreliable 1xx response need to be received again in
		 * final response. */	
		return Sdf_co_success;
	}

	if((dSdpPresence != Sdf_co_success) && \
			(Sdf_en_finalFailure != dMessageType))
	{
		/* No SDP to process. So return Success */	
		return Sdf_co_success;
	}

	/*Now Invoke State Transition Table*/
	if( dMode==Sdf_en_msgModeSend)
	{
		Sdf_ty_mediaState dTempMediaState;
		/*Change State*/
		 if((dTempMediaState =\
			dMediaCsmNxtStateOnMsgSent[dMessageType][dState]) == \
								Sdf_en_invalidMediaState)
		{
			pErr->errCode = Sdf_en_mediaStateTransitionError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaChangeMediaState(): "
			"Media State Transition Resulting in Invalid State ",pErr);
#endif
			return Sdf_co_fail;
		}
		pCallObject->pMediaInfo->dMediaState = dTempMediaState;
				
		if(dState != pCallObject->pMediaInfo->dMediaState)
		{
			*pMediaChange = Sdf_en_changeValid;
		}
	}
	else
	{
		Sdf_ty_mediaState dTempMediaState;
		/*Change State*/
		 if((dTempMediaState =\
			 dMediaCsmNxtStateOnMsgRecv[dMessageType][dState])\
			 ==	Sdf_en_invalidMediaState)
		{
			pErr->errCode = Sdf_en_mediaStateTransitionError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaChangeMediaState(): "
				"Media State Transition Resulting in Invalid State ",pErr);
#endif
			return Sdf_co_fail;
		}
		pCallObject->pMediaInfo->dMediaState = dTempMediaState;
						
		if((dState != pCallObject->pMediaInfo->dMediaState) && \
		   (dSdpPresence == Sdf_co_success))
		{
			*pMediaChange = Sdf_en_changeValid;
		}
	}
	/*State is Changed.
	 *Now Handle Special Case
	 */
#ifdef SIP_RPR	
	if(((dMessageType == Sdf_en_rpr) || (dMessageType ==\
					Sdf_en_provisionalResp))&& \
			(*pMediaChange == Sdf_en_changeValid))
#else
	if((dMessageType ==Sdf_en_provisionalResp)&& \
			(*pMediaChange == Sdf_en_changeValid))
#endif
	{
		/* Set the flag inside Media Info. This will be checked against 
		 * when a 200 OK is sent or received
		 */
		 pCallObject->pMediaInfo->dProvRespWithSdp = Sdf_co_true;
	}
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaIsSameIp
 **
 ** DESCRIPTION: 	The API checks if two media streams have the same IP 	
 *****************************************************************************/
Sdf_ty_bool	sdf_fn_uaIsSameIp
#ifdef ANSI_PROTO
	(Sdf_st_mediaStream	*pMediaStream1,
	 Sdf_st_mediaStream *pMediaStream2,
	 SdpConnection		*pSessionConnection,
	 Sdf_st_error		*pErr)
#else
	(pMediaStream1,pMediaStream2,pSessionConnection,pErr)
	Sdf_st_mediaStream	*pMediaStream1;
	Sdf_st_mediaStream *pMediaStream2;
	SdpConnection		*pSessionConnection;
	Sdf_st_error		*pErr;
#endif
{
     Sdf_st_listIterator	dListIterator1 = {0};
	 Sdf_st_listIterator	dListIterator2 = {0};

	 Sdf_ty_u32bit 			dSize1=0;
	 Sdf_ty_u32bit 			dSize2=0;
#ifdef SDF_LINT
     Sdf_st_listIterator	dListIterator1={0};
#endif     

	(void)sdf_listSizeOf(&(pMediaStream1->slConnection),&dSize1,pErr);
	(void)sdf_listSizeOf(&(pMediaStream2->slConnection),&dSize2,pErr);
	 
	if((dSize1 ==0) && (dSize2 ==0))
	{
		/*Media Level Connection is not Present,Hence both has same IP*/
		return Sdf_co_true;
	}
	if((dSize1 !=0) && (dSize2!=0))
	{
		(void)sdf_listInitIterator(&(pMediaStream1->slConnection),&dListIterator1,pErr);
	 
		while(dListIterator1.pCurrentElement != Sdf_co_null)
		{
			SdpConnection *pConnection1=Sdf_co_null;

			pConnection1 =(SdpConnection*)dListIterator1.pCurrentElement->pData;
			
			(void)sdf_listInitIterator(&(pMediaStream2->slConnection),&dListIterator2,pErr);
			
			while(dListIterator2.pCurrentElement != Sdf_co_null)
			{
				SdpConnection *pConnection2=Sdf_co_null;

				pConnection2 =(SdpConnection*)dListIterator2.pCurrentElement->pData;
				if(Sdf_mc_strcasecmp(pConnection1->pAddr,pConnection2->pAddr)==0)
				{
					/*IP Matching*/
					return Sdf_co_true;
				}
				
				(void)sdf_listNext(&dListIterator2,pErr);
			}
			(void)sdf_listNext(&dListIterator1,pErr);
		 }
		return Sdf_co_false;
	}
	if((dSize1 !=0) && (dSize2==0))
	{
		(void)sdf_listInitIterator(&(pMediaStream1->slConnection),&dListIterator1,pErr);
	}
	if((dSize2 !=0) && (dSize1 ==0))
	{
		(void)sdf_listInitIterator(&(pMediaStream2->slConnection),&dListIterator1,pErr);
	}
		
	while(dListIterator1.pCurrentElement != Sdf_co_null)
	{
		SdpConnection *pConnection=Sdf_co_null;

		pConnection =(SdpConnection*)dListIterator1.pCurrentElement->pData;
		if(Sdf_mc_strcasecmp(pConnection->pAddr,pSessionConnection->pAddr)==0)
		{
			/*IP Matching*/
			return Sdf_co_true;
		}
		(void)sdf_listNext(&dListIterator1,pErr);
	}
	return Sdf_co_false;

}
#ifdef SDF_QOS
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetStringForStrength
 **
 ** DESCRIPTION: 	The API converts the strength ENUM into String
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaGetStringForStrength
#ifdef ANSI_PROTO
	(Sdf_ty_qosStrength	dStrength,
	 Sdf_ty_s8bit		*pString)
#else
	(dStrength,pString)
	Sdf_ty_qosStrength	dStrength;
	Sdf_ty_s8bit		*pString;
#endif
{
	const Sdf_ty_s8bit*	dStrengthStringMapping[]=
	{
		" ",
		"mandatory",
		"optional",
		"none",
 		"failure",
		"unknown"
	};
	
	Sdf_mc_strcpy(pString,dStrengthStringMapping[dStrength]);

	return Sdf_co_success;
}
#endif
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaMapCodecStringToEnum
 **
 ** DESCRIPTION: 	The API maps a String to corresponding codec
 ** 					eg: "PCMU" -> Sdf_en_PCMU
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaMapCodecStringToEnum
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit	*pCodecString,
	 Sdf_ty_codecName	*pCodecName)
#else
	(pCodecString,pCodecName)
	Sdf_ty_s8bit	*pCodecString;
	Sdf_ty_codecName	*pCodecEnum;
#endif
{
	Sdf_ty_u32bit	dIndex, dLength = 0;
	Sdf_ty_s8bit 	*pTempCodec= Sdf_co_null;
	Sdf_st_error 	dError;
	if(pCodecString == Sdf_co_null)
		return	Sdf_co_fail;
	
	for(dIndex=0;dIndex<Sdf_co_codecTableSize;dIndex++)
	{
		if(Sdf_mc_strstr(sdf_gl_dCodecProfile[dIndex].dCodecString, \
				pCodecString) != Sdf_co_null)
		{
			*pCodecName = sdf_gl_dCodecProfile[dIndex].dCodecName;
			break;
		}
	}
	if(dIndex == Sdf_co_codecTableSize)
	{
		dLength = Sdf_mc_strlen(pCodecString);
		pTempCodec = (Sdf_ty_s8bit *)sdf_memget(0, dLength+1, &dError);

		if(pTempCodec == Sdf_co_null)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaMapPayloadFromCodec(): "
			"Failed to allocate memory", &dError);
#endif		
			return Sdf_co_fail;
		}

		for (dIndex = 0; dIndex < dLength ; dIndex++)
			pTempCodec[dIndex] = Sdf_mc_toupper(pCodecString[dIndex]);

		pTempCodec[dLength] = '\0';

		for(dIndex=0;dIndex<Sdf_co_codecTableSize;dIndex++)
		{
			if(Sdf_mc_strstr(sdf_gl_dCodecProfile[dIndex].dCodecString, \
					pTempCodec) != Sdf_co_null)
			{
				*pCodecName = sdf_gl_dCodecProfile[dIndex].dCodecName;
				break;
			}
		}
		(void)sdf_memfree(0,(Sdf_ty_pvoid *)&pTempCodec,&dError);
		if(dIndex == Sdf_co_codecTableSize)
		{
			*pCodecName = Sdf_en_otherCodec;
		}
	}

	return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: sdf_fn_uaGenerateMid
 **
 ** DESCRIPTION: This API clones the session param without the media group
 **              member of the session param
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGenerateMid
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit **ppMid, 
	 Sdf_st_error *pErr)
#else 
	(ppMid, pErr)
	 Sdf_ty_s8bit **ppMid;
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaGenerateMid");

	/*w534*/if(sdf_fn_uaGenerateViaBranch(Sdf_co_null, ppMid)
            			== Sdf_co_fail)
            {
              pErr->errCode = Sdf_en_noMemoryError;
              return Sdf_co_fail;
            } 

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_fn_uaGenerateMid");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaCloneSdpConnectionList
 **
 ** DESCRIPTION: This function clones the list of SdpConnection lines
 **              represented by slSrcList to pslDestList.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCloneSdpConnectionList
#ifdef ANSI_PROTO 
(Sdf_ty_slist *pslDestList,
Sdf_ty_slist  *pslSrcList,
Sdf_st_error  *pErr)
#else
( pslDestList, pslSrcList, pErr )
Sdf_ty_slist *pslDestList;
Sdf_ty_slist *pslSrcList;
Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_fn_uaCloneSdpConnectionList");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaCloneSdpConnectionList(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if((pslDestList == Sdf_co_null) || (pslSrcList == Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCloneSdpConnectionList():  "
			"pslDestList param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* Delete Everything in the Destination List*/
	(void)sdf_listDeleteAll(pslDestList,pErr);
	
    (void)sdf_listInitIterator(pslSrcList, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SdpConnection *pConnection = Sdf_co_null;
		SdpConnection *pNewConnection = Sdf_co_null;

		pConnection = (SdpConnection*)(dListIterator.pCurrentElement->pData);

		if (sip_initSdpConnection(&pNewConnection, (SipError *)\
					&(pErr->stkErrCode)) == SipFail)
        {
#ifdef SDF_ERROR
		    sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			    (Sdf_ty_s8bit *)"sdf_fn_uaCloneSdpConnectionList():  "
			    "Unable to Init SdpConnection",pErr);
#endif
			(void)sdf_listDeleteAll(pslDestList, pErr);
		    return Sdf_co_fail;
        }

		if (__sip_cloneSdpConnection(pNewConnection, pConnection,\
					(SipError *)&(pErr->stkErrCode)) == SipFail)
        {
#ifdef SDF_ERROR
		    sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			    (Sdf_ty_s8bit *)"sdf_fn_uaCloneSdpConnectionList():  "
			    "Unable to clone SdpConnection",pErr);
#endif
			(void)sdf_listDeleteAll(pslDestList, pErr);
			sip_freeSdpConnection(pNewConnection);
		    return Sdf_co_fail;
        }

		if (sdf_listAppend(pslDestList, pNewConnection, pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
		    sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			    (Sdf_ty_s8bit *)"sdf_fn_uaCloneSdpConnectionList():  "
			    "Unable to Append SdpConnection to pslDestList",pErr);
#endif
			(void)sdf_listDeleteAll(pslDestList, pErr);
			sip_freeSdpConnection(pNewConnection);
		    return Sdf_co_fail;
        }

		(void)sdf_listNext(&dListIterator, pErr);
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting "
			"sdf_fn_uaCloneSdpConnectionList");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaCloneCodecList
 **
 ** DESCRIPTION: This function clones the list of codecs 
 **              represented by slSrcList to pslDestList.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCloneCodecList
#ifdef ANSI_PROTO 
(Sdf_ty_slist *pslDestList,
Sdf_ty_slist  *pslSrcList,
Sdf_st_error  *pErr)
#else
( pslDestList, pslSrcList, pErr )
Sdf_ty_slist *pslDestList;
Sdf_ty_slist *pslSrcList;
Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_fn_uaCloneCodecList");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaCloneCodecList(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if((pslDestList == Sdf_co_null) || (pslSrcList == Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCloneCodecList():  "
			"pslDestList param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* Delete Everything in the Destination List*/
	(void)sdf_listDeleteAll(pslDestList,pErr);
	
    (void)sdf_listInitIterator(pslSrcList, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_codec *pCodec = Sdf_co_null;
		Sdf_st_codec *pCodecSrc = Sdf_co_null;

		pCodecSrc = (Sdf_st_codec*)(dListIterator.pCurrentElement->pData);

		if (sdf_ivk_uaInitCodec(&pCodec,pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
		    sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			    (Sdf_ty_s8bit *)"sdf_fn_uaCloneCodecList():  "
			    "Unable to Init Codec",pErr);
#endif
			(void)sdf_listDeleteAll(pslDestList, pErr);
		    return Sdf_co_fail;
        }

		if (sdf_ivk_uaCloneCodec(pCodec, pCodecSrc,\
					pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
		    sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			    (Sdf_ty_s8bit *)"sdf_fn_uaCloneCodecList():  "
			    "Unable to clone Codec",pErr);
#endif
			(void)sdf_listDeleteAll(pslDestList, pErr);
			(void)sdf_ivk_uaFreeCodec(pCodec);
		    return Sdf_co_fail;
        }

		if (sdf_listAppend(pslDestList, pCodec, pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
		    sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			    (Sdf_ty_s8bit *)"sdf_fn_uaCloneCodecList():  "
			    "Unable to Append Codec to pslDestList",pErr);
#endif
			(void)sdf_listDeleteAll(pslDestList, pErr);
			(void)sdf_ivk_uaFreeCodec(pCodec);
		    return Sdf_co_fail;
        }

		(void)sdf_listNext(&dListIterator, pErr);
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting "
			"sdf_fn_uaCloneCodecList");
	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaClonestringList
 **
 ** DESCRIPTION: This function clones the list of Strings
 **              represented by slSrcList to pslDestList.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCloneStringList
#ifdef ANSI_PROTO 
	(Sdf_ty_slist *pslDestList,
	Sdf_ty_slist  *pslSrcList,
	Sdf_st_error  *pErr)
#else
	( pslDestList, pslSrcList, pErr )
	Sdf_ty_slist *pslDestList;
	Sdf_ty_slist *pslSrcList;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_fn_uaCloneBandwidthList");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaCloneBandwidthList(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if((pslDestList == Sdf_co_null) || (pslSrcList == Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCloneBandwidthList():  "
			"pslDestList param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* Delete Everything in the Destination List*/
	(void)sdf_listDeleteAll(pslDestList,pErr);
	
    (void)sdf_listInitIterator(pslSrcList, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_ty_s8bit *pString = Sdf_co_null;
		Sdf_ty_s8bit *pStringSrc = Sdf_co_null;

		pStringSrc = (Sdf_ty_s8bit*)(dListIterator.pCurrentElement->pData);
		Sdf_mc_strdup(pString, pStringSrc);

		if (sdf_listAppend(pslDestList, pString, pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
		    sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			    (Sdf_ty_s8bit *)"sdf_fn_uaCloneBandwidthList():  "
			    "Unable to Append Bandwidth to pslDestList",pErr);
#endif
			(void)sdf_listDeleteAll(pslDestList, pErr);
			sip_freeString(pString);
		    return Sdf_co_fail;
        }

		(void)sdf_listNext(&dListIterator, pErr);
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting "
			"sdf_fn_uaCloneBandwidthList");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: sdf_fn_uaCloneSessionParamWithoutMediaGroup
 **
 ** DESCRIPTION: This API clones the session param without the media group
 **              member of the session param
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaCloneSessionParamWithoutMediaGroup
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pClonedSessionParam, 
	 Sdf_st_sessionParam *pSessionParam, 
	 Sdf_st_error *pErr)
#else 
	(pClonedSessionParam, pSessionParam, pErr)
	 Sdf_st_sessionParam *pClonedSessionParam; 
	 Sdf_st_sessionParam *pSessionParam; 
	 Sdf_st_error *pErr;
#endif
{
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaCloneSessionParamWithoutMediaGroup(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pClonedSessionParam == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCloneSessionParamWithoutMediaGroup():  "
			"pClonedSessionParam param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSessionParam == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCloneSessionParamWithoutMediaGroup():  "
			"pSessionParam param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	if (sdf_ivk_uaCloneSessionParam(pClonedSessionParam, pSessionParam,pErr)\
			== Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCloneSessionParamWithoutMediaGroup():  "
			"Unable to clone SessionParams str.",pErr);
#endif
		return Sdf_co_fail;
    }
#ifdef SDF_MEDIAGROUP
	/*Free the MediaStream Groups*/
	if(pClonedSessionParam->pMediaStreamGroups != Sdf_co_null)
	{
		Sdf_ty_u8bit  i = 0;

		for(i=0;i<pClonedSessionParam->dNumGroups;i++)
		{
			(void)sdf_ivk_uaFreeMediaStreamGroup((pClonedSessionParam->\
				pMediaStreamGroups[i]));
		}
	}
	pClonedSessionParam->dNumGroups=0;
	
	(void)sdf_memfree(Sdf_mc_freeMemId,\
			(Sdf_ty_pvoid*)&(pClonedSessionParam->pMediaStreamGroups), \
					(Sdf_st_error*)Sdf_co_null);
#endif
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting "
			"sdf_fn_uaCloneSessionParamWithoutMediaGroup");
	return Sdf_co_success;
}

Sdf_ty_retVal	sdf_fn_uaFormAttributeFromString
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit	*pString,
	 SdpMedia		*pSdpMedia,
	 Sdf_st_error	*pErr)
#else
	(pString,pSdpMedia,pErr)
	Sdf_ty_s8bit	*pString;
	SdpMedia		*pSdpMedia;
	Sdf_st_error	*pErr;
#endif
{
	Sdf_ty_u32bit	dIndex=0;

	/*Fix merged from UATK CSR-1-5674509 for IPTK CSR-1-6205472*/
	/*Should populate at the end of a-line list.*/
	if( (sip_listSizeOf(&(pSdpMedia->slAttr), &dIndex, \
									(SipError *)&(pErr->stkErrCode))) == SipFail)
	{
			pErr->errCode = Sdf_en_noExistError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
							(Sdf_ty_s8bit*)"sdf_fn_uaFormAttributeFromString(): "
							"Failed in size of list",pErr);
#endif			
			return Sdf_co_fail;
	}

	if(sdp_insertAttrFromStringAtIndexInMedia(pSdpMedia,pString,dIndex,\
						(SipError*)&(pErr->stkErrCode))==SipFail)
	{
		pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,  \
			(Sdf_ty_s8bit*)"sdf_fn_uaFormAttributeFromString(): "
			"Failed to Insert Attr",pErr);
#endif			
		return Sdf_co_fail;
	}
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormSessionAttributes
 **
 ** DESCRIPTION: 	The API Forms Sets the Attributes present
 **					slOtherSessionAttributes into SdpMessage.
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormSessionAttributes
#ifdef ANSI_PROTO
	(SdpMessage 			*pSdpMessage,
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_st_error			*pErr)
#else
	(pSdpMessage,pSessionParam,pErr)
	SdpMessage 			*pSdpMessage;
	Sdf_st_sessionParam	*pSessionParam;
	Sdf_st_error			*pErr;
#endif
{
	/*No Param Validation*/

	Sdf_ty_s8bit	*pStr=Sdf_co_null;
	/*Form the SessionInfo Line*/
	if(pSessionParam->pSessionInfo !=Sdf_co_null)
	{
		pStr = Sdf_mc_strdupSdp(pSessionParam->pSessionInfo);	

		if(sdp_setInfo(pSdpMessage,pStr, \
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, \
				pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormSessionAttributes(): "
				"Failed to set Info in Sdp\n",pErr);
#endif
			return Sdf_co_fail;
		}
	}
	/*Form u line*/
	if(pSessionParam->pUri !=Sdf_co_null)
	{
		pStr = Sdf_mc_strdupSdp(pSessionParam->pUri);	

		if(sdp_setUri(pSdpMessage,pStr, \
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, \
				pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormSessionAttributes(): "
				"Failed to set Uri in Sdp\n",pErr);
#endif
			return Sdf_co_fail;
		}
	}
	/*Form k line*/
	if(pSessionParam->pEncryptionKey !=Sdf_co_null)
	{
		pStr = Sdf_mc_strdupSdp(pSessionParam->pEncryptionKey);	

		if(sdp_setKey(pSdpMessage,pStr, \
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, \
				pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormSessionAttributes(): "
				"Failed to set Key in Sdp\n",pErr);
#endif
			return Sdf_co_fail;
		}
	}
	/*Form the e-mail list*/
	if(sdf_fn_uaFormSessionListAttributes(pSdpMessage,\
	        (Sdf_ty_sdpListInsertFuncPtr)sdp_insertEmailAtIndex_level1,&(pSessionParam->slEmail), pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, \
				pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormSessionAttributes(): "
				"Failed to Form Email List\n",pErr);
#endif
		return Sdf_co_fail;
	}
	/*Form the phonel list*/
	if(sdf_fn_uaFormSessionListAttributes(pSdpMessage,\
                sdp_insertPhoneAtIndex_level1,&(pSessionParam->slPhone), pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, \
				pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormSessionAttributes(): "
				"Failed to Form Phone List\n",pErr);
#endif
		return Sdf_co_fail;
	}
	/*Form the bandwidth list*/
	if(sdf_fn_uaFormSessionListAttributes(pSdpMessage,\
         	sdp_insertBandwidthAtIndex_level1,&(pSessionParam->slBandwidth), \
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, \
				pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormSessionAttributes(): "
				"Failed to Form Phone List\n",pErr);
#endif
		return Sdf_co_fail;
	}
	return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormSessionListAttributes
 **
 ** DESCRIPTION: 	This is a Genric API which forms any SessionLevel Attribute
 **					which is stored as List Of Strings.Takes the List and the 
 **					Funciton Pointer to Extract the element.
 **					.
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormSessionListAttributes
#ifdef ANSI_PROTO
	(SdpMessage				*pSdpMessage,
     Sdf_ty_sdpListInsertFuncPtr	pListInsertFuncptr,
	 Sdf_ty_slist			*pslList,
	 Sdf_st_error			*pErr)
#else
	 (pSdpMessage,pListInsertFuncptr,pslList,pErr)
	 SdpMessage				*pSdpMessage;
     Sdf_ty_sdpListInsertFuncPtr	pListInsertFuncptr;
	 Sdf_ty_slist			*pslList;
	 Sdf_st_error			*pErr;
#endif
{
	Sdf_st_listIterator	dListIterator;
	Sdf_ty_u32bit		dIndex=0;

	(void)sdf_listInitIterator(pslList,&dListIterator,pErr);

	while(dListIterator.pCurrentElement !=Sdf_co_null)
	{
		Sdf_ty_s8bit	*pStr=Sdf_co_null;

		pStr= Sdf_mc_strdupSdp
			((Sdf_ty_s8bit*)dListIterator.pCurrentElement->pData);
		if(pListInsertFuncptr(pSdpMessage,pStr,dIndex,\
								(SipError*)&pErr->stkErrCode)==SipFail)
		{
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, \
				pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormSessionListAttributes(): "
				"Failed Append Session Attribute into SdpMessage\n",pErr);
#endif
		return Sdf_co_fail;

		}
		dIndex++;
		(void)sdf_listNext(&dListIterator,pErr);
	}
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaUpdateLocalPropertiesInMediaStream
 **
 ** DESCRIPTION: This API updates the given local media streams properties 
 **              into the negotiated stream
 **
 *****************************************************************************/
 Sdf_ty_retVal Sdf_fn_uaUpdateLocalPropertiesInMediaStream
#ifdef ANSI_PROTO
	(Sdf_st_mediaStream *pMediaStreamNgtd,
	 Sdf_st_mediaStream *pMediaStreamLocal,
	 Sdf_st_error *pError)
#else
	(pMediaStreamNgtd,pMediaStreamLocal,
	 pError)
    Sdf_st_mediaStream *pMediaStreamNgtd;
	Sdf_st_mediaStream *pMediaStreamLocal;
    Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) ||(pMediaStreamLocal == Sdf_co_null) 
		|| (pMediaStreamNgtd == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"Sdf_fn_uaUpdateLocalPropertiesInMediaStream():"
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail; 
	}
#endif	/* End of param validation ifdef */

	/*
	 * Set the port if the peer is not rejected the stream 
	 */
	if(pMediaStreamNgtd->dPort != 0)
	{
		pMediaStreamNgtd->dPort = pMediaStreamLocal->dPort;
		pMediaStreamNgtd->dNumPort = pMediaStreamLocal->dNumPort;
	}

	/*
	 * Set Packet time
	 */
	pMediaStreamNgtd->dPacketTime = pMediaStreamLocal->dPacketTime;

	/* 
	 * Set the connection
	 */
	if(sdf_fn_uaCloneSdpConnectionList(&(pMediaStreamNgtd->slConnection),
									   &(pMediaStreamLocal->slConnection),
									   pError) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}
	/* 
	 * Set the previous connection
	 */
	if(sdf_fn_uaCloneSdpConnectionList(&(pMediaStreamNgtd->slPreviousConnection),
									   &(pMediaStreamLocal->slPreviousConnection),
									   pError) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}

	/*
	 * Set the other attributes
	 */
   	if(sdf_fn_uaCloneStringList(&(pMediaStreamNgtd->slBandwidth),
									   &(pMediaStreamLocal->slBandwidth),
									   pError) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}
	/*Update Prev Direction Attribute*/
	pMediaStreamNgtd->dPrevDirectionAttrib =
		pMediaStreamLocal->dPrevDirectionAttrib;

	if(pMediaStreamLocal->pKey != Sdf_co_null)
	{
		if(pMediaStreamNgtd->pKey != Sdf_co_null)
		{
			sip_freeString(pMediaStreamNgtd->pKey);
		}
		Sdf_mc_strdup(pMediaStreamNgtd->pKey, pMediaStreamLocal->pKey);
	}

	if(pMediaStreamLocal->pTitle != Sdf_co_null)
	{
		if(pMediaStreamNgtd->pTitle != Sdf_co_null)
		{
			sip_freeString(pMediaStreamNgtd->pTitle);
		}
		Sdf_mc_strdup(pMediaStreamNgtd->pTitle, pMediaStreamLocal->pTitle);
	}


	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaUpdateLocalPropertiesInSessionParam
 **
 ** DESCRIPTION: This API updates the given local sessionParam properties 
 **              into the negotiated one
 **
 *****************************************************************************/
Sdf_ty_retVal Sdf_fn_uaUpdateLocalPropertiesInSessionParam
#ifdef ANSI_PROTO
	(Sdf_st_mediaHandling *pMediaInfo,
	 Sdf_st_sessionParam *pSessionParamNgtd,
	 Sdf_st_sessionParam *pSessionParamLocal,
	 Sdf_st_error *pError)
#else
	(pMediaInfo, pSessionParamNgtd,pSessionParamLocal, pError)
	Sdf_st_mediaHandling *pMediaInfo;
    Sdf_st_sessionParam *pSessionParamNgtd;
	Sdf_st_sessionParam *pSessionParamLocal;
    Sdf_st_error *pError;
#endif
{
	Sdf_ty_u8bit 	dIndex;
	Sdf_ty_u8bit	dIncMediaStreams = 0;

	/*
	 * The pMediaInfo is not used anymore because of fix for SPR 2547.
	 * To prevent change of prototype and compilation warnings, the 
	 * following statement has been added.
	 */
	(void)pMediaInfo;

#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) ||(pSessionParamLocal == Sdf_co_null) 
		|| (pSessionParamNgtd == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"Sdf_fn_uaUpdateLocalPropertiesInMediaStream():"
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail; 
	}
#endif	/* End of param validation ifdef */

	pSessionParamNgtd->dVersion = pSessionParamLocal->dVersion;
	dIncMediaStreams = pSessionParamNgtd->dNumMediaStreams;

	/*
	 * Fix for SPR 2547. The condition 
	 * if(pMediaInfo->dMediaState != Sdf_en_offerRecv)
	 * was removed because this condition was leading to the MIDs
	 * of pCurrentSession and pCapabilities not getting synchronized.
	 * And because of which the deletion of media streams using MIDs
	 * at UAS was failing.
	 */
	for(dIndex = 0; dIndex < pSessionParamLocal->dNumMediaStreams; dIndex++)
	{
		if( dIndex == dIncMediaStreams)
			break;
		/*
		 * Update the mids with the local one if the msg doesn't contain 
		 * mid.
		 */
		if(pSessionParamNgtd->pMediaStreams[dIndex]->dMidPresence == \
				Sdf_co_false)
		{
			Sdf_st_mediaStream		*pMediaStream = Sdf_co_null;
			pMediaStream = (pSessionParamNgtd)->pMediaStreams[dIndex];

			if(pMediaStream->pMediaStreamId != Sdf_co_null )
				(void)sdf_memfree (0, (Sdf_ty_pvoid *)\
						&(pMediaStream->pMediaStreamId), pError);

			pMediaStream->pMediaStreamId = Sdf_mc_strdupSdp
				(pSessionParamLocal->pMediaStreams[dIndex]->pMediaStreamId);
		}
	}

	/* 
	 * Clone the session level connection if any i.e. update with the 
	 * local connection information in the Ngtd Session Param
	 */
	if(pSessionParamLocal->pConnection != Sdf_co_null)
	{
		if(pSessionParamNgtd->pConnection != Sdf_co_null)
		{
			sip_freeSdpConnection(pSessionParamNgtd->pConnection);
		}
		if (sip_initSdpConnection(&(pSessionParamNgtd->pConnection),\
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"Sdf_fn_uaUpdateLocalPropertiesInSessionParam():"
				"Unable to Init SdpConnection",pError);
#endif
			return Sdf_co_fail; 
		}

		if (__sip_cloneSdpConnection(pSessionParamNgtd->pConnection, 
				pSessionParamLocal->pConnection,\
					(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"Sdf_fn_uaUpdateLocalPropertiesInSessionParam(): "
				"Unable to clone SdpConnection",pError);
#endif
			return Sdf_co_fail; 
		}
	}
	/* 
	 * Clone the session level Previous connection if any 
	 */
	if(pSessionParamLocal->pPreviousConnection != Sdf_co_null)
	{
		if(pSessionParamNgtd->pPreviousConnection!= Sdf_co_null)
		{
			sip_freeSdpConnection(pSessionParamNgtd->pPreviousConnection);
		}
		if (sip_initSdpConnection(&(pSessionParamNgtd->pPreviousConnection),\
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"Sdf_fn_uaUpdateLocalPropertiesInSessionParam():"
				"Unable to Init SdpConnection",pError);
#endif
			return Sdf_co_fail; 
		}

		if (__sip_cloneSdpConnection(pSessionParamNgtd->pPreviousConnection, 
				pSessionParamLocal->pPreviousConnection,\
					(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"Sdf_fn_uaUpdateLocalPropertiesInSessionParam(): "
				"Unable to clone SdpConnection",pError);
#endif
			return Sdf_co_fail; 
		}
	}
	/*Update Previous Direction Attrib if any*/
	pSessionParamNgtd->dPrevDirectionAttrib = pSessionParamLocal->dPrevDirectionAttrib;

   	if(pSessionParamLocal->pSessionUser != Sdf_co_null)
	{
		if(pSessionParamNgtd->pSessionUser != Sdf_co_null)
		{
			sip_freeString(pSessionParamNgtd->pSessionUser);
		}
		Sdf_mc_strdup(pSessionParamNgtd->pSessionUser,\
			pSessionParamLocal->pSessionUser);
	}
	
	pSessionParamNgtd->dLocalSessionVersion = \
		pSessionParamLocal->dLocalSessionVersion;
	pSessionParamNgtd->dLocalSessionId = pSessionParamLocal->dLocalSessionId;

   	if(pSessionParamLocal->pOriginIp != Sdf_co_null)
	{
		if(pSessionParamNgtd->pOriginIp != Sdf_co_null)
		{
			sip_freeString(pSessionParamNgtd->pOriginIp);
		}
		Sdf_mc_strdup(pSessionParamNgtd->pOriginIp, \
			pSessionParamLocal->pOriginIp);
	}
	if(pSessionParamLocal->pOriginAddrType!= Sdf_co_null)
	{
		if(pSessionParamNgtd->pOriginAddrType!= Sdf_co_null)
		{
			sip_freeString(pSessionParamNgtd->pOriginAddrType);
		}
		Sdf_mc_strdup(pSessionParamNgtd->pOriginAddrType, \
			pSessionParamLocal->pOriginAddrType);
	}

   	if(pSessionParamLocal->pSessionName != Sdf_co_null)
	{
		if(pSessionParamNgtd->pSessionName != Sdf_co_null)
		{
			sip_freeString(pSessionParamNgtd->pSessionName);
		}
		Sdf_mc_strdup(pSessionParamNgtd->pSessionName,\
			pSessionParamLocal->pSessionName);
	}
   	if(pSessionParamLocal->pSessionInfo != Sdf_co_null)
	{
		if(pSessionParamNgtd->pSessionInfo != Sdf_co_null)
		{
			sip_freeString(pSessionParamNgtd->pSessionInfo);
		}
		Sdf_mc_strdup(pSessionParamNgtd->pSessionInfo, \
			pSessionParamLocal->pSessionInfo);
	}

   	if(pSessionParamLocal->pUri != Sdf_co_null)
	{
		if(pSessionParamNgtd->pUri != Sdf_co_null)
		{
			sip_freeString(pSessionParamNgtd->pUri);
		}
		Sdf_mc_strdup(pSessionParamNgtd->pUri, pSessionParamLocal->pUri);
	}

  	if(sdf_fn_uaCloneStringList(&(pSessionParamNgtd->slEmail),
									   &(pSessionParamLocal->slEmail),
									   pError) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}

  	if(sdf_fn_uaCloneStringList(&(pSessionParamNgtd->slPhone),
									   &(pSessionParamLocal->slPhone),
									   pError) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}

  	if(sdf_fn_uaCloneStringList(&(pSessionParamNgtd->slBandwidth),
									   &(pSessionParamLocal->slBandwidth),
									   pError) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}
   	if(pSessionParamLocal->pTimeZoneInfo != Sdf_co_null)
	{
		if(pSessionParamNgtd->pTimeZoneInfo != Sdf_co_null)
		{
			sip_freeString(pSessionParamNgtd->pTimeZoneInfo);
		}
		Sdf_mc_strdup(pSessionParamNgtd->pTimeZoneInfo,\
			pSessionParamLocal->pTimeZoneInfo);
	}
   	if(pSessionParamLocal->pEncryptionKey != Sdf_co_null)
	{
		if(pSessionParamNgtd->pEncryptionKey != Sdf_co_null)
		{
			sip_freeString(pSessionParamNgtd->pEncryptionKey);
		}
		Sdf_mc_strdup(pSessionParamNgtd->pEncryptionKey,\
			pSessionParamLocal->pEncryptionKey);
	}

	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaConvertMediaStreamListToSdpMediaList
 **
 ** DESCRIPTION: This API converts the given Sdf_st_mediaStream list to the 
 **              equivalent	SdpMedia list
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaConvertMediaStreamListToSdpMediaList
#ifdef ANSI_PROTO
	(Sdf_ty_slist	*pSdpMediaList,
	 Sdf_ty_slist		*pMediaStreamList,
	 Sdf_st_error		*pError)
#else
	(pSdpMediaList, pMediaStreamList, pError)
	Sdf_ty_slist	*pSdpMediaList;
	Sdf_ty_slist	*pMediaStreamList;
	Sdf_st_error	*pError;
#endif
{
	SdpMedia                *pMedia = Sdf_co_null;
	Sdf_st_mediaStream      *pMediaStream = Sdf_co_null;
	Sdf_st_listIterator     dIterator;
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null) ||(pMediaStreamList == Sdf_co_null) 
		|| (pSdpMediaList == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaConvertMediaStreamListToSdpMediaList():"
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail; 
	}
#endif	/* End of param validation ifdef */
	
	/* Convert the list of media streams to list of sdp media */
	(void)sdf_listInitIterator(pMediaStreamList, \
		&dIterator, pError);

	while (dIterator.pCurrentElement != Sdf_co_null)
	{
		pMediaStream = (Sdf_st_mediaStream*)\
			(dIterator.pCurrentElement->pData);

		if(sdf_ivk_uaFormSdpMediaFromMediaStream(pMediaStream,
			&pMedia, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR				
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaConvertMediaStreamListToSdpMediaList"
			"Could not convert the media stream to sdp media structure."\
			,pError);
#endif				
			return Sdf_co_fail; 
		}	

		/*
		 * Now append this SdpMedia into the Accepted media list output
		 * parameter
		 */
		if (sdf_listAppend(pSdpMediaList, (Sdf_ty_pvoid)pMedia, pError) == \
				Sdf_co_fail)
		{
#ifdef SDF_ERROR						
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaConvertMediaStreamListToSdpMediaList:"
				"Failed to append to list of accepted media lines ", pError);
#endif					
			pError->errCode=Sdf_en_sdpError;
			sip_freeSdpMedia(pMedia);
			return Sdf_co_fail; 
		}
		(void)sdf_listNext(&dIterator, pError);
	}
	pError->errCode=Sdf_en_noUaError;
	return Sdf_co_success;
}

#ifdef SDF_MEDIAGROUP
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaRemoveMediaStreamFromGroup
 **
 ** DESCRIPTION: This API converts the given Sdf_st_mediaStream list to the 
 **              equivalent	SdpMedia list
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaRemoveMediaStreamFromGroup
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam	*pSessionParam,
	 Sdf_ty_u32bit			dIndex,
	 Sdf_st_error		*pError)
#else
	(pSessionParam, dIndex, pError)
	Sdf_st_sessionParam	*pSessionParam;
	Sdf_ty_u32bit			dIndex;
	Sdf_st_error		*pError;

#endif
{
	Sdf_ty_u32bit		i=0;

	/*Iterate through all MediaStreamGroup and remove the Media Stream Id
	 * which matches the incoming Index
	 */
	for(i=0;i<pSessionParam->dNumGroups;i++)
	{
		Sdf_st_mediaStreamGroup	*pMediaGroup=Sdf_co_null;
		Sdf_st_listIterator	dListIterator;
		Sdf_ty_u32bit		dListIndex=0;
		pMediaGroup = pSessionParam->pMediaStreamGroups[i];

		if(sdf_listInitIterator(&(pMediaGroup->slMediaStreamIndex), \
				&dListIterator,	pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR						
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaRemoveMediaStreamFromGroup:"
				"Failed to Initialize Iterator", pError);
#endif					
			pError->errCode=Sdf_en_sdpError;
			return Sdf_co_fail; 
		}

		while(dListIterator.pCurrentElement != Sdf_co_null)
		{
			Sdf_ty_u32bit	*pMediaIndex=Sdf_co_null;

			pMediaIndex = (Sdf_ty_u32bit*)dListIterator.pCurrentElement->pData;

			if(*pMediaIndex == dIndex)
			{
				/*Remove the Index from List*/
				if(sdf_listDeleteAt(&(pMediaGroup->slMediaStreamIndex), \
						dListIndex,	pError) == Sdf_co_fail)
				{
#ifdef SDF_ERROR						
					sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaRemoveMediaStreamFromGroup:"
					"Failed to Delete", pError);
#endif					
					pError->errCode=Sdf_en_sdpError;
					return Sdf_co_fail; 

				}
				break ;
			}
			dListIndex++;
			(void)sdf_listNext(&dListIterator,pError);
		}
	}
	return Sdf_co_success;		
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaIsGroupSupported
 **
 ** DESCRIPTION: This API checks if the Group is Supported by UATK 
 *****************************************************************************/
Sdf_ty_bool sdf_fn_uaIsGroupSupported
#ifdef ANSI_PROTO
	(Sdf_st_mediaStreamGroup	*pMediaStreamGroup,
	 Sdf_st_error		*pError)
#else
	(pMediaStreamGroup,pError)
	Sdf_st_mediaStreamGroup	pMediaStreamGroup;
	Sdf_st_error		*pError;

#endif
{
/*
	Sdf_ty_u32bit	i=0;
*/
	(void)pError;
	if(pMediaStreamGroup->dGroupType == Sdf_en_groupOther)
	{
/*Code to be enabled if application needs to support Extra Groupings
		for(i=0;i<Sdf_mc_noOfExtraGroups;i++)
		{
			if(Sdf_mc_strcasecmp(pMediaStreamGroup->pOtherGroupType,\
							dOtherSupportedGroups[i])==0)
				return Sdf_co_true;
		}
*/		
		return Sdf_co_false;
	}
	else
	{
		return Sdf_co_true;
	}
}
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateGroupings
 **
 ** DESCRIPTION: This API checks Validity of group Attribute 
 *****************************************************************************/
Sdf_ty_bool sdf_fn_uaValidateGroupings
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam		*pSessionParam,
	 Sdf_st_mediaStreamGroup	**ppMediaStreamGroups,
	 Sdf_ty_u32bit				dNumGroups,
	 Sdf_st_error		*pErr)
#else
	(pSessionParam,ppMediaStreamGroups,dNumGroups,pErr)
	Sdf_st_sessionParam		*pSessionParam;
	Sdf_st_mediaStreamGroup	*ppMediaStreamGroups;
	Sdf_ty_u32bit			dNumGroups;
	Sdf_st_error		*pErr;

#endif
{
	Sdf_ty_u32bit	i=0,j=0;
	
	for(i=0;i<dNumGroups;i++)
	{
		Sdf_st_mediaStreamGroup	*pMediaStreamGroup;
		
		Sdf_st_listIterator	dListIterator1;
		/*For Each Group*/
		pMediaStreamGroup = ppMediaStreamGroups[i];
		/*Do Validations only for groups understood by UATK*/
		if((pMediaStreamGroup->dGroupType == Sdf_en_groupOther)\
					||(pMediaStreamGroup->dGroupType ==	Sdf_en_groupSRF))
		{
			continue;
		}
		else
		{
			Sdf_ty_u32bit	dCount=0;
			/*Initialize the Iterator for Mid List*/
			(void)sdf_listInitIterator(&(pMediaStreamGroup->slMediaStreamIndex),\
										&dListIterator1, pErr);
			
			while (dListIterator1.pCurrentElement != Sdf_co_null)
			{
				Sdf_ty_u32bit	*pIndex1;
				Sdf_ty_u32bit	dPointer=0;
				
				Sdf_st_listIterator	dListIterator2;

				Sdf_st_mediaStream	*pOutMediaStream;

				pIndex1 = (Sdf_ty_u32bit*)\
						(dListIterator1.pCurrentElement->pData);
				pOutMediaStream = pSessionParam->pMediaStreams[*pIndex1];
				
				dCount++;	

				/*Initialize the Iterator*/
				(void)sdf_listInitIterator(&(pMediaStreamGroup->slMediaStreamIndex),\
									&dListIterator2, pErr);
				while(dCount>dPointer)
				{
					dPointer++;
					(void)sdf_listNext(&dListIterator2,pErr);
				}
			
				while (dListIterator2.pCurrentElement != Sdf_co_null)
				{
					Sdf_ty_u32bit	*pIndex2;
					Sdf_st_mediaStream	*pInMediaStream;

							

					pIndex2 = (Sdf_ty_u32bit*)\
						(dListIterator2.pCurrentElement->pData);
					pInMediaStream = pSessionParam->pMediaStreams[*pIndex2];
					
					/*Compare Port and IP for MediaStreams which are distinct*/
					if(pOutMediaStream->dPort == pInMediaStream->dPort)
					{
						/* If the port is 0 then dont check the IPs */
						if (pOutMediaStream->dPort != 0)
						{
						    /*Compare IP*/
						    if(sdf_fn_uaIsSameIp(pOutMediaStream,\
						    		pInMediaStream,pSessionParam->pConnection,\
						    			pErr)==Sdf_co_true)
						    {
						    	pErr->errCode = Sdf_en_mediaGroupError;
#ifdef SDF_ERROR				
						    	sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
						    		(Sdf_ty_s8bit*)"sdf_fn_uaProcessGroupings( ): "
						    		"Group has same IP and Port for 2 "
									"MediaStreams",  pErr);
#endif
						    	return Sdf_co_false;
		
						    }
						}
					}
					(void)sdf_listNext(&dListIterator2, pErr);
				}
			(void)sdf_listNext(&dListIterator1, pErr);
			}
		}
	}
	/*Now Validate Presence in Mid's in more than one group of
	 * same semantics
	 */
	for(i=0;i<dNumGroups;i++)
	{
		Sdf_st_mediaStreamGroup	*pMediaStreamGroup1;
		
		/*For Each Group*/
		pMediaStreamGroup1 = ppMediaStreamGroups[i];
		
		/*Do Validations only for groups understood by UATK*/
		if((pMediaStreamGroup1->dGroupType == Sdf_en_groupOther)\
					||(pMediaStreamGroup1->dGroupType ==	Sdf_en_groupSRF))
		{
			continue;
		}
		else
		{

			for(j=i+1;j<dNumGroups;j++)
			{
				Sdf_st_mediaStreamGroup	*pMediaStreamGroup2;
				
				pMediaStreamGroup2 = ppMediaStreamGroups[j];

				if(pMediaStreamGroup1->dGroupType == pMediaStreamGroup2->dGroupType)
				{
					Sdf_st_listIterator	dListIterator1;

					/*Iterate through the mid's and find if any of them are present
					 * in both of them
					 */
					/*Initialize the Iterator*/
					(void)sdf_listInitIterator(&(pMediaStreamGroup1->slMediaStreamIndex),\
										&dListIterator1, pErr);
					while(dListIterator1.pCurrentElement != Sdf_co_null)
					{
						Sdf_st_listIterator	dListIterator2;
						Sdf_ty_u32bit	*pIndex1=Sdf_co_null;
					
						pIndex1 =
							(Sdf_ty_u32bit*)dListIterator1.pCurrentElement->pData;
						/*Initialize the Iterator*/
						(void)sdf_listInitIterator(&(pMediaStreamGroup2->slMediaStreamIndex),\
										&dListIterator2, pErr);
						while(dListIterator2.pCurrentElement != Sdf_co_null)
						{
							Sdf_ty_u32bit	*pIndex2=Sdf_co_null;
					
							pIndex2 =
							(Sdf_ty_u32bit*)dListIterator2.pCurrentElement->pData;
							if(*pIndex1 == *pIndex2)
							{
								pErr->errCode = Sdf_en_mediaGroupError;
#ifdef SDF_ERROR				
									sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
										(Sdf_ty_s8bit*)"sdf_fn_uaProcessGroupings( ): "
										"Same Mid in more than one Grouping"
										" of same semantic", pErr);
#endif
									return Sdf_co_false;
							}
							(void)sdf_listNext(&dListIterator2,pErr);
						}
						(void)sdf_listNext(&dListIterator1,pErr);
					}
				}/*End of If*/
			}/*End of For*/
		}/*End of If*/
	}/*End of For*/
	return Sdf_co_true;
}
#endif

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaConvertCodecListToMID
 **
 ** DESCRIPTION: 
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaConvertCodecListToMID
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pSessionParams,
	 Sdf_ty_slist		*slMediaList,
	 Sdf_ty_s8bit       *pMediaFormats,
	 Sdf_st_error		*pError)
#else
	(pCallObj, slMediaList, pMediaFormats, pError)
	 Sdf_st_sessionParam *pSessionParams;
	 Sdf_ty_slist		*slMediaList;
	 Sdf_ty_s8bit       *pMediaFormats;
	 Sdf_st_error		*pError;
#endif
{
	if (pMediaFormats != Sdf_co_null)
    {
		Sdf_ty_s8bit *pCodecStr = Sdf_co_null;
		Sdf_ty_s8bit *payload = Sdf_co_null;
        Sdf_ty_s8bit dTempCodecList[Sdf_co_characterBufferSize];
		Sdf_ty_s8bit *pTempCodec = Sdf_co_null;

        Sdf_mc_strcpy(dTempCodecList, pMediaFormats);

        pCodecStr = Sdf_mc_strtokr(dTempCodecList," ",&pTempCodec);

		do
        {
			Sdf_st_mediaStream *pMediaStream = Sdf_co_null;
			Sdf_ty_u8bit       dIndex = 0;
			Sdf_ty_u32bit      codecIndex = 0;

			payload = pCodecStr;

			for (dIndex = 0; dIndex < (pSessionParams->dNumMediaStreams); dIndex++)
            {
				Sdf_ty_u32bit dCodecListSize = 0;
				Sdf_st_codec *pCodec = Sdf_co_null;
				Sdf_ty_codecName dCodecName;
				Sdf_ty_bool dStreamFound = Sdf_co_false;

				if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
							&pMediaStream, dIndex, pError) == Sdf_co_fail)
                {
#ifdef SDF_ERROR
	                sdf_fn_setError(Sdf_en_majorErrors,\
			           	Sdf_en_sessionParamAccessError,\
   				       	(Sdf_ty_s8bit *)"sdf_fn_uaConvertCodecListToMID( ):  "
      				    "Failed to get MediaStream at index from SessionParam",\
					   	pError);
#endif
					(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
					pError->errCode=Sdf_en_sessionParamAccessError;
                    return Sdf_co_fail;
                }

				(void)sdf_listSizeOf(&(pMediaStream->slCodecs), &dCodecListSize,\
					           pError);

				for (codecIndex = 0; codecIndex < dCodecListSize; codecIndex++)
                {
					if (sdf_listGetAt(&(pMediaStream->slCodecs), codecIndex,\
						      (Sdf_ty_pvoid *)&pCodec, pError) == Sdf_co_fail)
                    {
#ifdef SDF_ERROR
		                sdf_fn_setError(Sdf_en_majorErrors,\
				           	Sdf_en_sdpAccessorError,\
      				       	(Sdf_ty_s8bit *)"sdf_fn_uaConvertCodecListToMID( ):  "
	      				    "Failed to get Codec at index from MediaStream",
			        		pError);
#endif
						(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
					    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
                        return Sdf_co_fail;
                    }

					/* Get the codec as name (enumeration). */
					dCodecName = \
						sdf_fn_uaMapCodecNumberToName(Sdf_mc_atoi(payload),\
								pError);

					if (dCodecName != Sdf_en_otherCodec)
                    {
                        if (dCodecName == pCodec->dCodecName)
                        {
							dStreamFound = Sdf_co_true;
							break;
                        }
                    }
#ifdef SDF_UAB2B
                    /*TODO SPR 3851*/
                    else if(dCodecName == Sdf_en_otherCodec)
                    {
						if (Sdf_mc_strcmp(payload,pCodec->pOtherCodecName) == 0)
                        {
							dStreamFound = Sdf_co_true;
							break;
                        }
                    }
                    else
                        dStreamFound = Sdf_co_false;
#else                    
					else
                    {
						if (Sdf_mc_strcmp(payload,pCodec->pOtherCodecName) == 0)
                        {
							dStreamFound = Sdf_co_true;
							break;
                        }
                    }
#endif                    
                } /* End of for loop of dCodecListSize */

				if (dStreamFound == Sdf_co_true)
                {
					Sdf_ty_s8bit *pTemp = Sdf_co_null;

					Sdf_mc_strdup(pTemp, pMediaStream->pMediaStreamId);

					if (sdf_listAppend(slMediaList, pTemp, pError) == Sdf_co_fail)
			        {
#ifdef SDF_ERROR				
				        sdf_fn_setError(Sdf_en_minorErrors,pError->errCode, \
					        (Sdf_ty_s8bit*)"sdf_fn_uaConvertCodecListToMID(): "
					        "Failed to append to  List", pError);
#endif				
				        return Sdf_co_fail;
			        }
                }

				/* Free the local reference */
				(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
            } /* End of for loop of dNumMediaStreams */

			/* 
			 * Pick up the next payload from the input list
			 */
			pCodecStr = Sdf_mc_strtokr(pTempCodec, " ",&pTempCodec);

        } while (pCodecStr != Sdf_co_null);
    }
	else
	{
#ifdef SDF_ERROR				
	    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
		    (Sdf_ty_s8bit*)"sdf_fn_uaConvertCodecListToMID(): "
	        "Invalid codec list passed to the fn.", pError);
#endif				
	    return Sdf_co_fail;
	}

	return Sdf_co_success;
}

#ifdef SDF_MEDIAGROUP
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaRemoveMediaStreamFromGrouping
 **
 ** DESCRIPTION: 	Removes the Index from all groupings
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaRemoveMediaStreamFromGrouping
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit	dIndex,
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_st_error		*pError)
#else
	(dIndex,pSessionParam,pError)
	Sdf_ty_u32bit	dIndex;
	Sdf_st_sessionParam	*pSessionParam;
	Sdf_st_error		*pError;
#endif
{
	Sdf_ty_u32bit	i=0;
	/*Iterate through the Groupings and remove this Index*/

	for(i=0;i<pSessionParam->dNumGroups;i++)
	{
		Sdf_ty_u32bit	j=0,dSize=0;

		Sdf_st_mediaStreamGroup *pMediaStreamGroup=Sdf_co_null;

		pMediaStreamGroup = pSessionParam->pMediaStreamGroups[i];

		(void)sdf_listSizeOf(&(pMediaStreamGroup->slMediaStreamIndex),&dSize,pError);

		for(j=0;j<dSize;j++)
		{
			/*Get the Index*/
			Sdf_ty_u8bit	*pMediaIndex=Sdf_co_null;

			/*w534*/(void)sdf_listGetAt(&(pMediaStreamGroup->slMediaStreamIndex),j,\
					(Sdf_ty_pvoid *)&pMediaIndex,pError);

			if(*pMediaIndex == dIndex)
			{
				/*Delete this from the List*/
				/*w534*/(void)sdf_listDeleteAt(&(pMediaStreamGroup->slMediaStreamIndex),j,\
						pError);
				break;
			}
		}
	}
	return Sdf_co_success;
}
#endif

/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaCloneSessionParamWithoutMediaStream
 **
 ** DESCRIPTION: This function copies the Sdf_st_sessionParam structure from 
 **				 src to dest without the media information
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCloneSessionParamWithoutMediaStream
#ifdef ANSI_PROTO 
(Sdf_st_sessionParam *pDestSessionParam,
Sdf_st_sessionParam *pSrcSessionParam,
Sdf_st_error *pErr)
#else
(pDestSessionParam,pSrcSessionParam, pErr )
Sdf_st_sessionParam *pDestSessionParam;
Sdf_st_sessionParam *pSrcSessionParam;
 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit i=0,count=0;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneSessionParam");

	pDestSessionParam->dVersion = pSrcSessionParam->dVersion;

	/* Free the dest connection befor clone to it */
	if(pDestSessionParam->pConnection !=Sdf_co_null)
		sip_freeSdpConnection(pDestSessionParam->pConnection);
	if(pSrcSessionParam->pConnection==Sdf_co_null)
	{
		pDestSessionParam->pConnection = Sdf_co_null;
	}
	else
	{
		if(sip_initSdpConnection(&(pDestSessionParam->pConnection),\
		(SipError *)&(pErr->stkErrCode)) == SipFail) 
             return Sdf_co_fail;

		if(__sip_cloneSdpConnection(pDestSessionParam->pConnection,\
			pSrcSessionParam->pConnection,(SipError *)&(pErr->stkErrCode)) == SipFail)
             return Sdf_co_fail;
	}
	
	/* Free the dest prev connection befor clone to it */
	if(pDestSessionParam->pPreviousConnection !=Sdf_co_null)
		sip_freeSdpConnection(pDestSessionParam->pPreviousConnection);
	if(pSrcSessionParam->pPreviousConnection==Sdf_co_null)
	{
		pDestSessionParam->pPreviousConnection = Sdf_co_null;
	}
	else
	{
		if(sip_initSdpConnection(&(pDestSessionParam->pPreviousConnection),\
			(SipError *)&(pErr->stkErrCode)) == SipFail) 
		      return Sdf_co_fail;
		if(__sip_cloneSdpConnection(pDestSessionParam->pPreviousConnection,\
			pSrcSessionParam->pPreviousConnection,\
			(SipError *)&(pErr->stkErrCode)) == SipFail)
 		      return Sdf_co_fail;
	}

	/* 
	 * Copy elements from the source to the destination. Before copying free 
	 * any existing members in the destination and overwrite with the members
	 * from the source
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pSessionUser),pErr);

	if(pSrcSessionParam->pSessionUser == Sdf_co_null)
		pDestSessionParam->pSessionUser = Sdf_co_null;
	else
		pDestSessionParam->pSessionUser = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pSessionUser);

	pDestSessionParam->dLocalSessionVersion = pSrcSessionParam->\
		dLocalSessionVersion;
	pDestSessionParam->dRemoteSessionVersion = pSrcSessionParam->\
		dRemoteSessionVersion;
	pDestSessionParam->dLocalSessionId = pSrcSessionParam->dLocalSessionId;
	pDestSessionParam->dRemoteSessionId = pSrcSessionParam->dRemoteSessionId;

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pOriginIp),pErr);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pOriginAddrType),pErr);

	if(pSrcSessionParam->pOriginIp == Sdf_co_null)
		pDestSessionParam->pOriginIp = Sdf_co_null;
	else
		pDestSessionParam->pOriginIp = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pOriginIp);

	if(pSrcSessionParam->pOriginAddrType== Sdf_co_null)
		pDestSessionParam->pOriginAddrType= Sdf_co_null;
	else
		pDestSessionParam->pOriginAddrType= Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pOriginAddrType);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pSessionName),pErr);

	if(pSrcSessionParam->pSessionName == Sdf_co_null)
		pDestSessionParam->pSessionName = Sdf_co_null;
	else
		pDestSessionParam->pSessionName = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pSessionName);


	pDestSessionParam->dDirectionAttrib = pSrcSessionParam->dDirectionAttrib;
	pDestSessionParam->dPrevDirectionAttrib = pSrcSessionParam->dPrevDirectionAttrib;
	pDestSessionParam->dSessionStart = pSrcSessionParam->dSessionStart;
	pDestSessionParam->dSessionEnd = pSrcSessionParam->dSessionEnd;

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pSessionInfo),pErr);
	if(pSrcSessionParam->pSessionInfo == Sdf_co_null)
		pDestSessionParam->pSessionInfo = Sdf_co_null;
	else
		pDestSessionParam->pSessionInfo = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pSessionInfo);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pUri),pErr);
	if(pSrcSessionParam->pUri == Sdf_co_null)
		pDestSessionParam->pUri = Sdf_co_null;
	else
		pDestSessionParam->pUri = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pUri);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pTimeZoneInfo),pErr);
	if(pSrcSessionParam->pTimeZoneInfo == Sdf_co_null)
		pDestSessionParam->pTimeZoneInfo = Sdf_co_null;
	else
		pDestSessionParam->pTimeZoneInfo = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pTimeZoneInfo);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pEncryptionKey),pErr);
	if(pSrcSessionParam->pEncryptionKey == Sdf_co_null)
		pDestSessionParam->pEncryptionKey = Sdf_co_null;
	else
		pDestSessionParam->pEncryptionKey = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pEncryptionKey);

	/* Clone the list member */

	/*
	 * Delete contents of the destination OtherAttrib list
	 * before adding the new cloned Atrrib structure to it
	 */

	(void)sdf_listDeleteAll(&(pDestSessionParam->slOtherSessionAttributes),pErr);
	(void)sdf_listSizeOf(&(pSrcSessionParam->slOtherSessionAttributes), &count, pErr);
	for ( i = 0; i < count ; i++ )
	{
		SdpAttr *attrout, *clone_attrout;

		(void)sdf_listGetAt(&(pSrcSessionParam->slOtherSessionAttributes),i,\
			(Sdf_ty_pvoid *)(&attrout), pErr); 
		if (attrout == Sdf_co_null)
			clone_attrout = Sdf_co_null;
		else
		{
			if(sip_initSdpAttr(&clone_attrout, (SipError *)&(pErr->stkErrCode)) ==
							SipFail) 
				return Sdf_co_fail; 
			if(__sip_cloneSdpAttr (clone_attrout, attrout,\
				(SipError *)&(pErr->stkErrCode)) == SipFail)
            {
                sip_freeSdpAttr(clone_attrout);
				return Sdf_co_fail; 
            }
		}
		if(sdf_listAppend (&(pDestSessionParam->slOtherSessionAttributes),\
			(Sdf_ty_pvoid)clone_attrout, pErr) == Sdf_co_fail)
        {
                sip_freeSdpAttr(clone_attrout);
				return Sdf_co_fail; 
        } 
	}

	/* Copy slEmail list */
	(void)sdf_listDeleteAll(&(pDestSessionParam->slEmail),pErr);

	/*w534*/if(__sdf_cloneSdfStringList(&(pDestSessionParam->slEmail),\
		&(pSrcSessionParam->slEmail), pErr) == Sdf_co_fail)
				return Sdf_co_fail; 

#ifdef SDF_UAB2B
/* Copy slRepeatTime list:Optional SPR-4218*/
  	(void)sdf_listDeleteAll(&(pDestSessionParam->slRepeatTime),pErr);

	/*w534*/if(__sdf_cloneSdfStringList(&(pDestSessionParam->slRepeatTime),\
		&(pSrcSessionParam->slRepeatTime), pErr) == Sdf_co_fail)
				return Sdf_co_fail; 

#endif
	/* Copy slPhone list */
	(void)sdf_listDeleteAll(&(pDestSessionParam->slPhone),pErr);
	/*w534*/if(__sdf_cloneSdfStringList(&(pDestSessionParam->slPhone),\
		&(pSrcSessionParam->slPhone), pErr) == Sdf_co_fail)
				return Sdf_co_fail; 

	/* Copy BandWidth list */
	(void)sdf_listDeleteAll(&(pDestSessionParam->slBandwidth),pErr);
	/*w534*/if(__sdf_cloneSdfStringList(&(pDestSessionParam->slBandwidth),\
		&(pSrcSessionParam->slBandwidth), pErr) == Sdf_co_fail)
				return Sdf_co_fail; 

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneSessionParam");

	pErr->errCode = Sdf_en_noUaError;
	return 	Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaUpdateMediaStreamsInSessionParam
 **
 ** DESCRIPTION: This function updates the Sdf_st_sessionParam structure with 
 **				 the media information
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateMediaStreamsInSessionParam (\
	Sdf_st_sessionParam *pDestSessionParam,\
	Sdf_ty_slist *slAcceptedMedia,Sdf_st_error *pErr)
{
	Sdf_ty_u32bit dCount, i = 0;

	/* Free the previous contains of MediaStreams */
	if(	pDestSessionParam->pMediaStreams !=Sdf_co_null)
	{
		for(i=0;i<pDestSessionParam->dNumMediaStreams;i++)
			(void)sdf_ivk_uaFreeMediaStream(\
				pDestSessionParam->pMediaStreams[i]);
	}
	/*
	 * Get each element of the source list, clone it and insert into the
	 * destination list.
	 */
	(void)sdf_listSizeOf(slAcceptedMedia,&dCount,pErr);
	
	pDestSessionParam->pMediaStreams = (Sdf_st_mediaStream **)\
		sdf_memget(Sdf_mc_accessorMemId,dCount * \
		sizeof(Sdf_st_mediaStream), pErr);

	for(i=0;i<dCount;i++)
	{
		Sdf_st_mediaStream *pMediaStream = Sdf_co_null;
        if(sdf_listGetAt(slAcceptedMedia, i, \
			(Sdf_ty_pvoid*)&pMediaStream, \
            pErr) == Sdf_co_fail)
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
                (Sdf_ty_s8bit*)"sdf_fn_uaUpdateMediaStreamsInSessionParam():  "
                "Failed to get mediastream from Media List",pErr);
#endif
			(void)sdf_memfree(Sdf_mc_accessorMemId, \
				(Sdf_ty_pvoid *)&pDestSessionParam->pMediaStreams,\
				pErr);
            return Sdf_co_fail;
        }

		if(sdf_ivk_uaInitMediaStream(&(pDestSessionParam->pMediaStreams[i]),\
				pErr) == Sdf_co_fail) 
        {
           Sdf_ty_u32bit j=0;
           for(j=0;j<i;j++)
              (void)sdf_ivk_uaFreeMediaStream(pDestSessionParam->pMediaStreams[i]);
		   (void)sdf_memfree(Sdf_mc_accessorMemId, \
		           		     (Sdf_ty_pvoid *)&pDestSessionParam->pMediaStreams,\
				              pErr);
           return Sdf_co_fail;
        }
		/*w534*/if(sdf_ivk_uaCloneMediaStream(pDestSessionParam->pMediaStreams[i], \
				pMediaStream,pErr) == Sdf_co_fail)
        {
            Sdf_ty_u32bit j=0;
            for(j=0;j<i;j++)
              (void)sdf_ivk_uaFreeMediaStream(pDestSessionParam->pMediaStreams[i]);
		    (void)sdf_memfree(Sdf_mc_accessorMemId, \
		            		  (Sdf_ty_pvoid *)&pDestSessionParam->pMediaStreams,\
			 	              pErr);
            return Sdf_co_fail;
        }

	}
	pDestSessionParam->dNumMediaStreams = dCount; 

	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


