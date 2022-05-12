/*****************************************************************************
 ** FUNCTION:		This file contains all API's related to creating a 
 **					user profile
 **
 *****************************************************************************
 **
 ** FILENAME		: sdf_configs.c
 **
 ** DESCRIPTION		: This file contains all API's related to creating a 
 **					  user profile
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 16/11/00	Ashok						Creation
 ** 10/12/02	Srikanth					SdpRevamp
 ** 22/04/03    Anil Kumar     ---          Changes for TLS feature.
 ** 12/05/03     Subbarao		UATK 2.1 Design      RFC 3428 feature added.
 **
 *****************************************************************************
 **				COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#include "ALheader.h"
#include "sdf_portlayer.h"
#include "sdf_configs.h"
#include "sdf_sdpinternal.h"
#include "sdf_internal.h"
#include "sdf_accessor.h"
#include "sdf_debug.h"
#include "sdf.h"
#include "sdf_reqresp.h"
#include "sdf_sdp.h"
#include "sdf_callbacks.h"
#include "sdf_init.h"
#include "sdf_network.h"
#include "sdf_trace.h"
#ifndef SDF_LINT
#include "sdf_sdpstruct.h"
#include "sdf_free.h"
#include "sdf_list.h"
#include "sdf_common.h"
#include "sdf_struct.h"
#endif

#ifndef ICF_PORT_SYMBIAN
Sdf_ty_u32bit IndicateContact=0;
Sdf_ty_u32bit IndicatePreLoadedRoute=0;
Sdf_ty_u32bit IndicateFrom=0;
Sdf_ty_u32bit IndicateRegistrar=0;
Sdf_ty_u32bit glbIndicateRegistrar=0;
Sdf_ty_u32bit IndicateMedia=0;
Sdf_ty_u32bit IndicateSdp=0;
Sdf_ty_u32bit IndicateAny=0;
Sdf_ty_u32bit IndicateAnyFlag=0;
Sdf_ty_u32bit IndicateMediaFlag=0;
Sdf_ty_u32bit IndicateTLSFlag=0;
#ifdef SDF_IMS
Sdf_ty_u32bit IndicateResponse=0;
#endif
#ifdef SDF_TLS
Sdf_ty_u32bit glbIndicateTLSFlag=0;
#endif

Sdf_st_localMedia *pGlbMedia = Sdf_co_null;
SdpMedia *pGlbSdpMedia = Sdf_co_null;
SipMsgBody *pMsgBody = Sdf_co_null;
SdpMessage *pGlbSdpMessage = Sdf_co_null;
Sdf_st_configExtraHeaders *pExtraHdrs = Sdf_co_null;
#ifdef SDF_IMS
Sdf_st_configExtraHeaders *pRespHdrs = Sdf_co_null;
#endif
SipParam *GC = Sdf_co_null;


/***********************************************************************
** Function:  sdf_fn_xmlParseAttlist
** Description: Parses the attributes inside a tag
**
***********************************************************************/
Sdf_ty_retVal sdf_fn_xmlParseAttlist 
#ifdef ANSI_PROTO
(Sdf_ty_s8bit* pInput, \
Sdf_ty_s32bit* pPosition,\
Sdf_ty_s8bit **ppAttrib,\
Sdf_st_error* pErr)
#else
(pInput,pPosition,ppAttrib,pErr)
Sdf_ty_s8bit* pInput;
Sdf_ty_s32bit* pPosition;
Sdf_ty_s8bit **ppAttrib;
Sdf_st_error* pErr;
#endif
{
	Sdf_ty_s8bit attribute[Sdf_mc_maxConfigFileLen];
	Sdf_ty_s8bit value[Sdf_mc_maxConfigFileLen];
	Sdf_ty_s8bit equalTo = 0;
	Sdf_ty_u32bit BreakVar=0;
	*ppAttrib=Sdf_co_null;

	sdf_fn_skipSpaces(pInput,pPosition);
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_fn_xmlParseAttlist\n");

	while(pInput[(*pPosition)]!='>')
	{
    	/* get some x="y" into att */
		Sdf_mc_sscanf(&pInput[*pPosition],"%[a-zA-Z]",attribute);
		*pPosition = *pPosition +Sdf_mc_strlen(attribute);


		sdf_fn_skipSpaces(pInput,pPosition);

		equalTo=pInput[*pPosition];
		*pPosition = *pPosition +1;

		if(equalTo!='=')
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_configDataError,\
				(Sdf_ty_s8bit *)"sdf_fn_xmlParseAttlist(): "
				"XML invalid syntax - invalid attribute list\n",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			BreakVar=1;
			break;	
		}

		sdf_fn_skipSpaces(pInput,pPosition);
		if(pInput[(*pPosition)++]!='"')
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_configDataError,\
				(Sdf_ty_s8bit *)"sdf_fn_xmlParseAttlist(): "
				"XML invalid syntax -- \" expected\n",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			BreakVar=1;
			break;
		}

		Sdf_mc_sscanf(&pInput[*pPosition],"%[a-zA-Z]",value);
		*pPosition = *pPosition +Sdf_mc_strlen(value);

		sdf_fn_skipSpaces(pInput,pPosition);
		if(pInput[(*pPosition)++]!='"')
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_configDataError,\
				(Sdf_ty_s8bit *)"sdf_fn_xmlParseAttlist():  "
				"XML invalid syntax -- \" expected\n",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			BreakVar=1;
			break;
		}

		if ( (Sdf_mc_strcasecmp(attribute,"name") == 0) \
			|| (Sdf_mc_strcasecmp(attribute,"addalways") == 0) )
		{

#ifndef SDF_LINT            
			if(Sdf_co_null!=value)
#endif                
				*ppAttrib=Sdf_mc_strdupConfig(value);

			if(sip_listAppend(&(GC->slValue),(Sdf_ty_pvoid)*ppAttrib, \
				(SipError *)&(pErr->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_configDataError, \
					(Sdf_ty_s8bit *)"sdf_fn_xmlParseAttlist(): "
					"Error Appending to Garbabge List",pErr);
#endif
				pErr->errCode=Sdf_en_configDataError;
				return Sdf_co_fail;
			}
		}
		sdf_fn_skipSpaces(pInput,pPosition);
	}
    if(BreakVar==1)
    {
		pErr->errCode=Sdf_en_configDataError;
		return Sdf_co_fail;	
    }
	else
	{
		sdf_fn_skipSpaces(pInput,pPosition);
		pErr->errCode=Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_fn_xmlParseAttlist \n");
		return Sdf_co_success;
	}
}


/**************************************************************** 
** FUNCTION: sdf_fn_uaInitLocalMedia
**
** DESCRIPTION: Initializes the LocalMedia structure. The parser uses 
**				this function to fill in the media params
**
***************************************************************/ 

Sdf_ty_retVal sdf_fn_uaInitLocalMedia
#ifdef ANSI_PROTO
	(Sdf_st_error* pErr)
#else
	(pErr)
	Sdf_st_error* pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_fn_uaInitLocalMedia");

	pGlbMedia=(Sdf_st_localMedia *)sdf_memget(Sdf_mc_configMemId, \
		sizeof(Sdf_st_localMedia), pErr);

	if(pGlbMedia==Sdf_co_null)
	{
		pErr->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	(pGlbMedia)->pMediaType=Sdf_co_null;
	(pGlbMedia)->pPort     =Sdf_co_null;
	(pGlbMedia)->pPortNum  =Sdf_co_null;
	(pGlbMedia)->pTransport=Sdf_co_null;
	(pGlbMedia)->pCodecs   =Sdf_co_null;
	(pGlbMedia)->pTransType=Sdf_co_null;
	(pGlbMedia)->pBandwidth=Sdf_co_null;
	(pGlbMedia)->pKey=Sdf_co_null;
	(pGlbMedia)->pInfo=Sdf_co_null;


    if(	sdf_listInit(&((pGlbMedia)->slAttribute), \
		__sdf_fn_uaFreeString,Sdf_co_false, pErr)==Sdf_co_fail)
		return Sdf_co_fail;

    if(	sdf_listInit(&((pGlbMedia)->slConnection), \
		__sdf_fn_uaFreeString,Sdf_co_false,pErr)==Sdf_co_fail)
		return Sdf_co_fail;

	pErr->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaInitLocalMedia");
	return Sdf_co_success;
}

/**************************************************************** 
** FUNCTION:  sdf_fn_skipCharsForward
**
** DESCRIPTION: Skips caharcters till a matching pattern
**
***************************************************************/ 

Sdf_ty_bool sdf_fn_skipCharsForward
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit* pInput,
	Sdf_ty_s32bit* pPosition,
	Sdf_ty_s8bit* pPattern)
#else
	(pInput, pPosition, pPattern)
	Sdf_ty_s8bit* pInput;
	Sdf_ty_s32bit* pPosition;
	Sdf_ty_s8bit* pPattern;
#endif
{
	Sdf_ty_s32bit matched=0,len=0;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG--Entering "
		"sdf_fn_skipCharsForward");
	len=Sdf_mc_strlen(pPattern);

	/* 
	 * While not end of buffer input, skip data 
	 * till the pattern is found
	 */
	while((pInput[(*pPosition)++]!='\0')&&(!matched))
	{
		if(!Sdf_mc_strncmp(&pInput[*pPosition],pPattern,len))
		{
			*pPosition=*pPosition+Sdf_mc_strlen(pPattern)-1;

			matched=1;
		}
	}
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG--Exiting  sdf_fn_skipCharsForward");
	if(!matched)			
		return Sdf_co_false;

	return Sdf_co_true;
}

/***************************************************************
** FUNCTION:  sdf_fn_skipSpaces
**
** DESCRIPTION: This function skips spaces in given buffer and at a 
**				given position. It advances the pointer to the next 
**				non-space character.
***************************************************************/ 
void sdf_fn_skipSpaces
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit* pInput,
	 Sdf_ty_s32bit* pPosition)
#else
	(pInput, pPosition)
	Sdf_ty_s8bit* pInput;
	Sdf_ty_s32bit* pPosition;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG--Entering  sdf_fn_skipSpaces\n");

	while ((pInput[*pPosition]==' ')||(pInput[*pPosition]=='\n')||\
		(pInput[*pPosition]=='\b')||(pInput[*pPosition]=='\t')||\
		(pInput[*pPosition]=='\r'))
	{
		*pPosition=*pPosition+1;
	}

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG--Exiting sdf_fn_skipSpaces \n");
}


/***************************************************************
** FUNCTION:  sdf_fn_xmlParseTag
**
** DESCRIPTION: This function parses the tag at the point pointed 
**				by position. It will parse recursively till end of 
**				the same tag if found.
***************************************************************/ 
Sdf_ty_retVal sdf_fn_xmlParseTag
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *pInput,
	Sdf_ty_s32bit* pPosition,
	Sdf_st_initData* pInitData,
	Sdf_st_error* pErr)
#else
	(pInput,pPosition, pInitData, pErr)
	Sdf_ty_s8bit *pInput;
	Sdf_ty_s32bit* pPosition;
	Sdf_st_initData* pInitData;
	Sdf_st_error* pErr;
#endif
{
	SipHeader	         *pContHdr = Sdf_co_null;
	SipHeader	         *pRouteHdr = Sdf_co_null;
	SipAddrSpec          *pFromAddrSpec = Sdf_co_null;
	Sdf_ty_u16bit	     contPort = 0;
	Sdf_ty_s8bit	     *pTmpVal = Sdf_co_null;
	SipAddrSpec 	     *pTempSpec = Sdf_co_null;
	Sdf_st_transportInfo *pTempTransport = Sdf_co_null;

	/* Flag to exit the Parsing */
	Sdf_ty_u32bit ExitVar=0;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG--Entering  sdf_fn_xmlParseTag\n");
	sdf_fn_skipSpaces(pInput,pPosition);

   	/* 
	 * Special tag (like the <?xml ...?> tag at the beginning of a document)
	 */
    if(!Sdf_mc_strncmp(&pInput[*pPosition],"<?",2))
	{
	   	if(!sdf_fn_skipCharsForward(pInput,pPosition,(Sdf_ty_s8bit *)"?>"))
		{
#ifdef SDF_ERROR
	    	sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_configDataError,
				(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): ?> not found\n",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			return Sdf_co_fail;
		}
	}

   /* DTD for the document */
  if(!Sdf_mc_strcasecmp(&pInput[*pPosition],"<!DOCTYPE"))
	{
		if(!sdf_fn_skipCharsForward(pInput,pPosition,(Sdf_ty_s8bit *)"]>"))
		{
#ifdef SDF_ERROR
    		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_configDataError,\
				(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
				"XML invalid syntax -- no end for <!DOCTYPE> tag\n",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			return Sdf_co_fail;
		}
	}

   /* Skip comments */
    if(!Sdf_mc_strncmp(&pInput[*pPosition],"<!--",4))
	{
		if(!sdf_fn_skipCharsForward(pInput,pPosition,(Sdf_ty_s8bit *)"-->"))
		{
#ifdef SDF_ERROR
    		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_configDataError,\
				(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): \"-->\" not found\n",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			return Sdf_co_fail;
		}
	}

    if(!Sdf_mc_strncmp(&pInput[*pPosition], "</",2))
	{
#ifdef SDF_ERROR
    	sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_configDataError, \
			(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
			"XML invalid syntax--Improper Close tag\n\n",pErr);
#endif
		pErr->errCode=Sdf_en_configDataError;
		pErr->errCode=Sdf_en_configDataError;
		return Sdf_co_fail;
	}

    /* Opening tag */
    if(!Sdf_mc_strncmp(&pInput[*pPosition],"<",1))
    {
		Sdf_ty_s8bit nodeName[Sdf_mc_maxConfigFileLen],endTagExpected[Sdf_mc_maxConfigFileLen]; 
		Sdf_ty_s8bit *attribute=Sdf_co_null;

		*pPosition=*pPosition+1;

		/* Retrieve the node name and store it in the Nodes tagName files */
		sdf_fn_skipSpaces(pInput,pPosition);

		Sdf_mc_sscanf(&pInput[*pPosition],"%[a-z]",nodeName);
		*pPosition =*pPosition +Sdf_mc_strlen(nodeName);

		/* The end tag expected for this particular node */
		sdf_fn_skipSpaces(pInput,pPosition);


		/* Check for the attributes */		
		if(pInput[*pPosition]!='>')
		{
			if(sdf_fn_xmlParseAttlist(pInput,pPosition,&attribute,pErr)== \
			  Sdf_co_fail)
				return Sdf_co_fail;
		}
		Sdf_mc_strcpy(endTagExpected,"</");
		Sdf_mc_strcat(endTagExpected,nodeName);
		Sdf_mc_strcat(endTagExpected,">");

		sdf_fn_skipSpaces(pInput,pPosition);

		if(pInput[*pPosition]!='>')
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_configDataError, \
				(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): '>' expected\n",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			return Sdf_co_fail;
		}

		*pPosition=*pPosition+1;
		sdf_fn_skipSpaces(pInput,pPosition);

    	while (Sdf_mc_strncmp(&pInput[*pPosition], endTagExpected,\
			Sdf_mc_strlen(endTagExpected)))
        {
			Sdf_ty_s32bit len;
			Sdf_ty_s8bit tagData[Sdf_mc_maxConfigFileLen];

			if(!Sdf_mc_strncmp(&pInput[*pPosition],"</",2))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_configDataError,\
						(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
						"XML invalid syntax--invalid end tag expecting node-name\n"\
						,pErr);
#endif
				pErr->errCode=Sdf_en_configDataError;
				ExitVar=1;
				break;
			}
			if(pInput[*pPosition]== '<')
			{
				if(sdf_fn_xmlParseTag(pInput,pPosition,pInitData, \
							pErr) ==Sdf_co_fail)
				{
					pErr->errCode=Sdf_en_configDataError;
					ExitVar=1;
					break;	
				}
				sdf_fn_skipSpaces(pInput,pPosition);
				continue;
			}

			/* Get the total string */
			Sdf_mc_sscanf(&pInput[*pPosition],"%[^<]",tagData);
			len=Sdf_mc_strlen(tagData);
			if(len!=0)
			{
				while(tagData[len-1]==' ')
					len--;
			}
			tagData[len]='\0';

			/* Copy the tag Data in the Node tagData field */
			*pPosition=*pPosition+Sdf_mc_strlen(tagData);
			sdf_fn_skipSpaces(pInput,pPosition);

			/* Compare and fill in the user profile structure */
			if(Sdf_mc_strcasecmp("name",nodeName)==0)
		  {
				IndicateContact=0;
				IndicateFrom=0;
				IndicateMedia=0;
				IndicateRegistrar=0;
				IndicateSdp=0;
				IndicateAny=0;
				IndicatePreLoadedRoute=0;
				IndicateTLSFlag=0;

				if((Sdf_mc_strcasecmp("FROM",tagData)==0)||\
					(Sdf_mc_strcasecmp("f",tagData)==0))
				{
					IndicateFrom=1;
				}
				else if((Sdf_mc_strcasecmp("CONTACT",tagData)==0)||
					(Sdf_mc_strcasecmp("m",tagData)==0))
				{
					IndicateContact=1;
				}
				else if(Sdf_mc_strcasecmp("REGISTRAR",tagData)==0)
				{
					glbIndicateRegistrar=1;
					IndicateRegistrar=1;
				}
				else if(Sdf_mc_strcasecmp("PRELOADEDROUTE",tagData)==0)
				{
					IndicatePreLoadedRoute=1;
				}

				else if(Sdf_mc_strcasecmp("SDP",tagData)==0)
				{
					IndicateSdp=1;
				}
				else if(Sdf_mc_strcasecmp("MEDIA",tagData)==0)
				{
					IndicateMedia=1;
					if(sdf_fn_uaInitLocalMedia(pErr)==Sdf_co_fail)
					{
						pErr->errCode=Sdf_en_configDataError;
						ExitVar=1;
						break;
					}
				}
				else if (Sdf_mc_strcasecmp("TLS",tagData) == 0)
				{
					IndicateTLSFlag = 1;
#ifdef SDF_TLS
					glbIndicateTLSFlag = 1;
#endif
				}
#ifdef SDF_RPORT
				else if (Sdf_mc_strcasecmp("RPORT",tagData) == 0)
				{
					pInitData->dEnableRport = Sdf_co_true;
				}
#endif

				else if (Sdf_mc_strcasecmp("CUAFW",tagData)!=0)
				{
                    IndicateAny=1;
#ifdef SDF_IMS
					if(IndicateResponse==1)
					{
						if(sdf_ivk_uaInitConfigExtraHeaders(&pRespHdrs, \
					   	pErr)==Sdf_co_fail)
						{
							pErr->errCode=Sdf_en_configDataError;
							ExitVar=1;
							break;
						}
						pRespHdrs->pParam->pName=Sdf_mc_strdupConfig(tagData);
						if(attribute!=Sdf_co_null)
						{
							pRespHdrs->pData=(void*)Sdf_mc_strdupConfig(attribute);
						}
					}
					else
					{
#endif
						if(sdf_ivk_uaInitConfigExtraHeaders(&pExtraHdrs, \
					   	pErr)==Sdf_co_fail)
						{
							pErr->errCode=Sdf_en_configDataError;
							ExitVar=1;
							break;
						}
						pExtraHdrs->pParam->pName=Sdf_mc_strdupConfig(tagData);
						if(attribute!=Sdf_co_null)
						{
							pExtraHdrs->pData=(void*)Sdf_mc_strdupConfig(attribute);
						}
#ifdef SDF_IMS
					}
#endif
				}
		  }
#ifdef SDF_IMS
      else if(Sdf_mc_strcasecmp("type",nodeName)==0)
		  {
			  if (Sdf_mc_strcasecmp("RESPONSE",tagData) == 0)
				  IndicateResponse=1;
		  }
#endif

			if(attribute!=Sdf_co_null)
			{
				if(Sdf_mc_strcasecmp("address",attribute)==0)
				{
					Sdf_ty_s8bit *pHost, *pIp = Sdf_co_null,*pName=Sdf_co_null;
					Sdf_ty_s8bit *pParam=Sdf_co_null;
					Sdf_ty_bool	dAddrAny=Sdf_co_false;
					Sdf_ty_s8bit finalAddr[Sdf_mc_maxConfigFileLen]="";
					Sdf_ty_bool		dTelUrlPresent = Sdf_co_false;
					Sdf_ty_bool		dImUrlPresent  = Sdf_co_false;
					Sdf_ty_bool		dPresUrlPresent  = Sdf_co_false;
					Sdf_ty_s8bit dTempHost[Sdf_co_smallBufferSize];
#if defined SDF_TEL || defined SDF_IM || defined SDF_PRES
					Sdf_ty_s8bit dTempStr[Sdf_mc_maxConfigFileLen];
                    Sdf_ty_s8bit *pScheme=Sdf_co_null;
					Sdf_mc_strcpy(dTempStr,tagData);
					pScheme = Sdf_mc_strtok(dTempStr,":");                    
#endif

#ifdef SDF_TEL
					if(Sdf_mc_strcasecmp(pScheme,"tel") == 0)
						dTelUrlPresent = Sdf_co_true;
#endif
#ifdef SDF_IM
					if( dTelUrlPresent != Sdf_co_true )
					{
						if(Sdf_mc_strcasecmp(pScheme,"im") == 0)
							dImUrlPresent = Sdf_co_true;
					}
#endif
#ifdef SDF_PRES
					if((dTelUrlPresent != Sdf_co_true)&&
                        (dImUrlPresent != Sdf_co_true))
					{
						if(Sdf_mc_strcasecmp(pScheme,"pres") == 0)
							dPresUrlPresent = Sdf_co_true;
					}
#endif                    
					if((dTelUrlPresent == Sdf_co_true) || \
						(dImUrlPresent == Sdf_co_true) || \
                        (dPresUrlPresent == Sdf_co_true))	
					{
						(void)sdf_fn_uaSprintf(finalAddr, "%s",tagData);
					}
					else
					{
						Sdf_ty_s8bit *pTempParam=Sdf_co_null;
						if(Sdf_mc_strstr(tagData,"@")!=Sdf_co_null)
						{
							pName=Sdf_mc_strtokr(tagData,"@",&pTempParam);
							
							if(Sdf_mc_strstr(pTempParam,";") != Sdf_co_null)
							 pHost=Sdf_mc_strtokr(pTempParam,";",&pParam);
							else
								pHost=pTempParam;
						}	
						else
						{
							if(Sdf_mc_strstr(tagData,";") != Sdf_co_null)
								pHost=Sdf_mc_strtokr(tagData,";",&pParam);
							else
								pHost=tagData;
						}
						if(IndicatePreLoadedRoute != 1)
						{
							if(Sdf_mc_strcmp(pHost,"*")==0)
							{
								if(sdf_fn_uaGetHostName(&pHost,pErr)==\
										Sdf_co_fail)
								{
#ifdef SDF_ERROR	
									sdf_fn_setError(Sdf_en_minorErrors,\
									Sdf_en_configDataError,\
										(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
										"Unable to resolve host name",pErr);
#endif
									pErr->errCode=Sdf_en_configDataError;
									ExitVar=1;
									break;
								}
								dAddrAny=Sdf_co_true;
							}

							if (Sdf_mc_strstr(pHost, "[") != Sdf_co_null)
							{
							    /*
							     * Strip-off the "[" & "]" before calling
							     * sdf_fn_uaGetHostIp.
							     */
							    Sdf_mc_strncpy(dTempHost, (pHost+1),
							    		Sdf_mc_strlen(pHost)-2);
							    dTempHost[Sdf_mc_strlen(pHost)-2] = '\0';
							}
							else
							{
								Sdf_mc_strcpy(dTempHost, pHost);
							}

							if(sdf_fn_uaGetHostIp(dTempHost,&pIp,pErr)==\
									Sdf_co_fail)
							{
#ifdef SDF_ERROR	
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError,\
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Unable to resolve host name",pErr);
#endif
                                (void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&pIp, pErr);
								pErr->errCode=Sdf_en_configDataError;
								ExitVar=1;
								break;
							}
						}

						if (pName != Sdf_co_null)
						{
							if(Sdf_mc_strstr(pName,":") != Sdf_co_null)
								(void)sdf_fn_uaSprintf(finalAddr, "%s@%s", pName,\
									pHost);
							else
								(void)sdf_fn_uaSprintf(finalAddr, "sip:%s@%s", pName,\
									pHost);
						}
						else 
							(void)sdf_fn_uaSprintf(finalAddr, "%s", pHost);

						if(pParam !=Sdf_co_null)
						{
							Sdf_mc_strcat(finalAddr,";");
							Sdf_mc_strcat(finalAddr,pParam);
						}
					}
					
					if(IndicateFrom==1)
					{
						if(sip_setAddrSpecFromString(&pFromAddrSpec,\
							 finalAddr, (SipError *)&(pErr->stkErrCode)) \
								== SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError, \
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Invalid From Address."
							   	"Unable to convert String to Header",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							ExitVar=1;
							break;
						}

#ifndef SDF_TLS
						/*
						 * Check if TLS is disabled and SIPS uri is present in
						 * the From header.
						 */
						if (pFromAddrSpec->dType == SipAddrSipSUri)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError, \
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Invalid Uri scheme in From Address." ,pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&pIp, pErr);
							sip_freeSipAddrSpec(pFromAddrSpec);
							ExitVar=1;
							break;
						}
#endif /* SDF_TLS */

#ifdef SDF_TEL
						if(dTelUrlPresent == Sdf_co_true)
						{
							TelUrl	*pTelUrl = Sdf_co_null;

							/*Check if the Tel Url is a valid one*/
							if(sip_getTelUrlFromAddrSpec(pFromAddrSpec, \
								&pTelUrl, (SipError*)&(pErr->stkErrCode)) \
								==SipFail)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Invalid Tel Url in From",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								sip_freeSipAddrSpec(pFromAddrSpec);
								ExitVar=1;
								break;

							}
							/*At this point the Tel Url has been parsed and the
							 * respective data strtucture are filled up.
							 *  Now the application can proceed accessing 
							 *  this structure.
							 */
							sip_freeTelUrl(pTelUrl);
						}
#endif						
#ifdef SDF_IM
						if(dImUrlPresent == Sdf_co_true)
						{
							ImUrl	*pImUrl = Sdf_co_null;

							/*Check if the Tel Url is a valid one*/
							if(sip_getImUrlFromAddrSpec(pFromAddrSpec, \
								&pImUrl, (SipError*)&(pErr->stkErrCode)) \
								==SipFail)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Invalid Im Url in From",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								sip_freeSipAddrSpec(pFromAddrSpec);
								ExitVar=1;
								break;

							}
							sip_freeImUrl(pImUrl);
						}
#endif	
#ifdef SDF_PRES
						if(dPresUrlPresent == Sdf_co_true)
						{
							PresUrl	*pPresUrl = Sdf_co_null;

							/*Check if the Tel Url is a valid one*/
							if(sip_getPresUrlFromAddrSpec(pFromAddrSpec, \
								&pPresUrl, (SipError*)&(pErr->stkErrCode)) \
								==SipFail)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Invalid Im Url in From",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								sip_freeSipAddrSpec(pFromAddrSpec);
								ExitVar=1;
								break;

							}
							sip_freePresUrl(pPresUrl);
						}
#endif
						if(sip_setAddrSpecInFromHdr(pInitData->pFrom, \
							pFromAddrSpec, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
							sip_freeSipAddrSpec(pFromAddrSpec);
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError, \
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Unable to set address spec in From",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							ExitVar=1;
							break;
						}
						sip_freeSipAddrSpec(pFromAddrSpec);
						pFromAddrSpec = Sdf_co_null;

#if defined (SDF_TEL) || defined (SDF_IM) || defined (SDF_PRES)
						if( (dTelUrlPresent == Sdf_co_true) || \
							(dImUrlPresent == Sdf_co_true)  || \
                            (dPresUrlPresent == Sdf_co_true))
						{
/*w534*/ 			(void)Sdf_mc_uaGetHostIp(pHost,pIp,dAddrAny,pErr);
	
							pInitData->pTransport->pIp = \
											Sdf_mc_strdupConfig(pIp);
							(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&pIp,\
								 pErr);
						}
						else
#endif
							pInitData->pTransport->pIp=Sdf_mc_strdupConfig(pIp);
					}
					else if(IndicateContact==1)
					{
						Sdf_ty_s8bit pTemp[Sdf_co_largeBufferSize];

						(void)sdf_fn_uaSprintf(pTemp, "Contact: <%s>", finalAddr);
						if (sip_initSipHeader(&pContHdr, SipHdrTypeAny, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError, \
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to initialize SipHeader to hold "
								"Contact",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							break;
						}

						if (sip_parseSingleHeader(pTemp, \
							SipHdrTypeContactNormal, pContHdr, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError, \
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to parse Contact string into a Contact"
								"SipHeader",pErr);
#endif
							sip_freeSipHeader(pContHdr);
                            (void)sdf_memfree(Sdf_mc_freeMemId,
                                    (Sdf_ty_pvoid*)&pContHdr,pErr);
                            (void)sdf_memfree(Sdf_mc_freeMemId,
                                    (Sdf_ty_pvoid*)&pIp,pErr);

							pErr->errCode=Sdf_en_configDataError;
							break;
						}
#ifndef SDF_TLS
						/*
						 * Check if TLS is disabled and SIPS uri is present in
						 * the Contact header.
						 */
						else
						{
						    SipAddrSpec *pContactSpec;

						    if(sip_getAddrSpecFromContactHdr(pContHdr,\
									&pContactSpec,\
									(SipError*)&pErr->stkErrCode)==SipFail)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Failed to Get AddrSpec From Contact"
									"Header.",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								break;
							}
							if (pContactSpec->dType == SipAddrSipSUri)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Invalid Uri scheme in Contact "
									"Address." ,pErr);
#endif
								sip_freeSipHeader(pContHdr);
								sip_freeSipAddrSpec(pContactSpec);
								pErr->errCode=Sdf_en_configDataError;
								ExitVar=1;
								break;
							}
							sip_freeSipAddrSpec(pContactSpec);
						}
#endif /* SDF_TLS */

#ifdef SDF_TEL						
						/*Check if the Tel Url is a valid one*/
						if(dTelUrlPresent == Sdf_co_true)
						{
							SipAddrSpec *pContactSpec;
							TelUrl	*pTelUrl;
							if(sip_getAddrSpecFromContactHdr(pContHdr,\
										&pContactSpec,\
										(SipError*)&pErr->stkErrCode)==SipFail)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Failed to Get AddrSpec From Contact"
									"Header to check" 
									"for Tel Url Validity",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								break;
							}
		
							if(sip_getTelUrlFromAddrSpec(pContactSpec,\
										&pTelUrl,\
									(SipError*)&(pErr->stkErrCode))==SipFail)
							{
								sip_freeSipAddrSpec(pContactSpec);
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Invalid Tel Url Contact",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								ExitVar=1;
								break;

							}
							/*At this point the Tel Url has been parsed and the
							 * respective data strtucture are filled up.
							 *  Now the application can proceed accessing 
							 *  this structure.
							 */

							sip_freeSipAddrSpec(pContactSpec);
							sip_freeTelUrl(pTelUrl);
						}
#endif
#ifdef SDF_IM						
						/*Check if the IM Url is a valid one*/
						if(dImUrlPresent == Sdf_co_true)
						{
							SipAddrSpec *pContactSpec;
							ImUrl		*pImUrl;

							if(sip_getAddrSpecFromContactHdr(pContHdr,\
									&pContactSpec,\
									(SipError*)&pErr->stkErrCode)==SipFail)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Failed to Get AddrSpec From Contact"
									"Header to check" 
									"for IM Url Validity",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								break;
							}
		
							if(sip_getImUrlFromAddrSpec(pContactSpec, &pImUrl,\
									(SipError*)&(pErr->stkErrCode))==SipFail)
							{
								sip_freeSipAddrSpec(pContactSpec);
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Invalid Im Url Contact",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								ExitVar=1;
								break;
							}
							/*At this point the IM Url has been parsed and the
							 * respective data strtucture are filled up.
							 *  Now the application can proceed accessing 
							 *  this structure.
							 */

							sip_freeSipAddrSpec(pContactSpec);
							sip_freeImUrl(pImUrl);
						}
#endif

#ifdef SDF_PRES						
						/*Check if the PRES Url is a valid one*/
						if(dPresUrlPresent == Sdf_co_true)
						{
							SipAddrSpec *pContactSpec;
							PresUrl		*pPresUrl;

							if(sip_getAddrSpecFromContactHdr(pContHdr,\
									&pContactSpec,\
									(SipError*)&pErr->stkErrCode)==SipFail)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Failed to Get AddrSpec From Contact"
									"Header to check" 
									"for Pres Url Validity",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								break;
							}
		
						if(sip_getPresUrlFromAddrSpec(pContactSpec, &pPresUrl,\
									(SipError*)&(pErr->stkErrCode))==SipFail)
							{
								sip_freeSipAddrSpec(pContactSpec);
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError, \
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Invalid Pres Url Contact",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								ExitVar=1;
								break;
							}
							/*At this point the Pres Url has been parsed and the
							 * respective data strtucture are filled up.
							 *  Now the application can proceed accessing 
							 *  this structure.
							 */

							sip_freeSipAddrSpec(pContactSpec);
							sip_freePresUrl(pPresUrl);
						}
#endif

						/*
						 * Initialize and fill the pTempTransport structure 
						 * so that the Contact transport Info can be added to
						 * slContactTransport.
						 */
                        if (sdf_ivk_uaInitTransportInfo(&pTempTransport,
									pErr) == Sdf_co_fail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors, \
								Sdf_en_configDataError, \
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Unable to init transport info.",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							ExitVar=1;
							break;
						}

#if defined (SDF_TEL) || defined (SDF_IM) || defined (SDF_PRES)
						if( (dTelUrlPresent == Sdf_co_true) || \
							(dImUrlPresent == Sdf_co_true)  || \
                            (dPresUrlPresent == Sdf_co_true))
						{
              (void)Sdf_mc_uaGetHostIp(pHost,pIp,dAddrAny,pErr);
							pTempTransport->pIp = Sdf_mc_strdupConfig(pIp);
							(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&pIp,\
								 pErr);
						}
						else
#endif /* TEL | IM */
							pTempTransport->pIp=Sdf_mc_strdupConfig(pIp);
						
						if(contPort != 0)
						{
							SipError err;

							if(sip_getAddrSpecFromContactHdr(pContHdr, \
								&pTempSpec,	&err) != SipFail)
							{
								if((pTempSpec->dType == SipAddrSipUri) || \
									(pTempSpec->dType == SipAddrSipSUri))
								{
									SipUrl *pUrl;

									if(sdf_ivk_uaGetUrlFromAddrSpec(pTempSpec, \
											&pUrl, pErr)!= Sdf_co_fail)
									{
/*534*/								(void)sip_setPortInUrl(pUrl, (Sdf_ty_u16bit)\
											Sdf_mc_atoi(tagData), &err);
										contPort = 0;
										sip_freeSipUrl(pUrl);
									}
								}
								sip_freeSipAddrSpec(pTempSpec);
							}

							/* Set the port in the Contact Transport Info. */
							pTempTransport->dPort = Sdf_mc_atoi(tagData);
						}
						else
						{
							SipError err;

							/* 
							 * Set the default port here to hanldle the case
							 * when no port is mentioned in the profile.
							 */
							if(sip_getAddrSpecFromContactHdr(pContHdr, \
								&pTempSpec,	&err) != SipFail)
							{
#ifdef SDF_TLS
								if(pTempSpec->dType == SipAddrSipSUri)
								{
							        pTempTransport->dPort = \
										Sdf_mc_defaultSipSPort;
								}
							    else
#endif /* SDF_TLS */
						            pTempTransport->dPort = \
										Sdf_mc_defaultSipPort;
								sip_freeSipAddrSpec(pTempSpec);
							}
						}
						/* 	
						 * At the end this again needs to be done in case if 
						 * there is only one contact header.
						 */
						if(pContHdr != Sdf_co_null)
						{
							if((sdf_listAppend(&(pInitData->slContact),\
								pContHdr, pErr))==Sdf_co_fail)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors,\
									Sdf_en_configDataError,\
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Unable to set address spec in From",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								break;
							}

							/* 
							 * Also append the Contact Transport Info into
							 * the slContactTransport.
							 */
							if((sdf_listAppend(&(pInitData->slContactTransport),
								pTempTransport, pErr))==Sdf_co_fail)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors,\
									Sdf_en_configDataError,\
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Unable to set address spec in From",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								break;
							}
						}
					}
					else if(IndicatePreLoadedRoute==1)
					{
						Sdf_ty_s8bit pTemp[Sdf_co_largeBufferSize];

						(void)sdf_fn_uaSprintf(pTemp, "Route: <%s>", finalAddr);
						if (sip_initSipHeader(&pRouteHdr, SipHdrTypeAny, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError, \
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to initialize SipHeader to "
								"Route",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							break;
						}

						if (sip_parseSingleHeader(pTemp, \
							SipHdrTypeRoute, pRouteHdr, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError, \
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to parse Route string into a Route"
								"SipHeader",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							break;
						}
						if(contPort != 0)
						{
							SipError err;
							if(sip_getAddrSpecFromContactHdr(pRouteHdr, \
								&pTempSpec,	&err) != SipFail)
							{
								if((pTempSpec->dType == SipAddrSipUri) || \
									(pTempSpec->dType == SipAddrSipSUri))
								{
									SipUrl *pUrl;

									if(sdf_ivk_uaGetUrlFromAddrSpec(pTempSpec, \
												&pUrl, pErr)!= Sdf_co_fail)
									{
/*w534*/							(void)sip_setPortInUrl(pUrl, (Sdf_ty_u16bit) \
											Sdf_mc_atoi(tagData), &err);
										contPort = 0;
									sip_freeSipUrl(pUrl);
									}
								}
								sip_freeSipAddrSpec(pTempSpec);
							}
						}
						/* 	
						 * At the end this again needs to be done in case if 
						 * there is only one Route header.
						 */
						if(pRouteHdr != Sdf_co_null)
						{
							if((sdf_listAppend(&(pInitData->slPreLoadedRoute),\
								pRouteHdr, pErr))==Sdf_co_fail)
							{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors,\
									Sdf_en_configDataError,\
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Unable to Append Route to pInitData",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								break;
							}
						}
					}
					else if(IndicateRegistrar==1)
					{
						pInitData->pRegistrar->pIp = \
						   		Sdf_mc_strdupConfig(finalAddr);
					}
					
					if((dTelUrlPresent == Sdf_co_false) && \
						(dImUrlPresent == Sdf_co_false) && \
                        (dPresUrlPresent == Sdf_co_false)&& \
						(IndicatePreLoadedRoute != 1))
						(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&pIp, pErr);

					if(dAddrAny)
						(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&pHost,\
									pErr);
				}
				else if(Sdf_mc_strcasecmp("displayname",attribute)==0)
				{
					if(IndicateFrom==1)
					{
						pTmpVal = Sdf_mc_strdupConfig(tagData);
/*w534*/		(void)sip_setDispNameInFromHdr(pInitData->pFrom, pTmpVal, \
							(SipError *)&(pErr->stkErrCode));
					}
			    else if(IndicateContact==1)
                {
                    Sdf_ty_u32bit dCount;

                    /* Get the last header from the contact header list */
                    (void)sdf_listSizeOf(&(pInitData->slContact), &dCount, \
                            pErr);
                    /*w534*/		(void)sdf_listGetAt(&(pInitData->slContact), \
                            (dCount-1), (Sdf_ty_pvoid *)&pContHdr, \
                            pErr);

                    pTmpVal = Sdf_co_null;
                    pTmpVal = Sdf_mc_strdupConfig(tagData);
 /*w534*/		    if(sip_setDispNameInContactHdr(pContHdr, pTmpVal, \
                                (SipError*)&(pErr->stkErrCode)) == SipFail)
                    {
                        (void)sdf_memfree(Sdf_mc_configMemId, 
                                (Sdf_ty_pvoid *)&pTmpVal, pErr);
                    }
                }	
					else if(IndicatePreLoadedRoute==1)
					{
						Sdf_ty_u32bit dCount;

						/* Get the last header from the Route header list */
						(void)sdf_listSizeOf(&(pInitData->slPreLoadedRoute), &dCount,\
							pErr);
/*w534*/		(void)sdf_listGetAt(&(pInitData->slPreLoadedRoute), \
							(dCount-1), (Sdf_ty_pvoid *)&pRouteHdr, \
							pErr);

						pTmpVal = Sdf_co_null;
						pTmpVal = Sdf_mc_strdupConfig(tagData);
/*w534*/		(void)sip_setDispNameInRouteHdr(pRouteHdr, pTmpVal, \
							(SipError*)&(pErr->stkErrCode));
					}
				}
				else if(Sdf_mc_strcasecmp("port",attribute)==0)
				{
					if(Sdf_mc_atoi(tagData)>65535)
					{
						pErr->errCode=Sdf_en_configDataError;
						ExitVar=1;
						break;
					}
					if(IndicateFrom == 1)
					{
						SipError err;
						
						if( sip_getAddrSpecFromFromHdr(pInitData->pFrom, \
							&pTempSpec, &err) == SipSuccess)
						{
							if((pTempSpec->dType == SipAddrSipUri) || \
								(pTempSpec->dType == SipAddrSipSUri))
							{
								SipUrl *pUrl;

								if(sdf_ivk_uaGetUrlFromAddrSpec(pTempSpec, \
											&pUrl, pErr)!= Sdf_co_fail)
								{
/*w534*/				(void)sip_setPortInUrl(pUrl, (Sdf_ty_u16bit)\
										Sdf_mc_atoi(tagData), &err);
									sip_freeSipUrl(pUrl);
								}
							}
							sip_freeSipAddrSpec(pTempSpec);
						}
						pInitData->pTransport->dPort=Sdf_mc_atoi(tagData);
					}
					else if(IndicateContact==1)
					{
						SipError err;
						Sdf_ty_u32bit dCount;

						contPort = Sdf_mc_atoi(tagData);
						
						/*Get the last header from the contact header list*/
						(void)sdf_listSizeOf(&(pInitData->slContact), &dCount, \
							pErr);
/*w534*/		(void)sdf_listGetAt(&(pInitData->slContact), \
							(dCount-1), (Sdf_ty_pvoid *)&pContHdr, pErr);

						if(sip_getAddrSpecFromContactHdr(pContHdr, \
							&pTempSpec,	&err) != SipFail)
						{
							if((pTempSpec->dType == SipAddrSipUri) || \
								(pTempSpec->dType == SipAddrSipSUri))
							{
								SipUrl *pUrl;

								if(sdf_ivk_uaGetUrlFromAddrSpec(pTempSpec, \
									&pUrl, pErr)!= Sdf_co_fail)
								{
/*w534*/						(void)sip_setPortInUrl(pUrl,(Sdf_ty_u16bit)\
										Sdf_mc_atoi(tagData),&err);
									sip_freeSipUrl(pUrl);
								}
								else
								{
									sip_freeSipAddrSpec(pTempSpec);
									return Sdf_co_fail;
								}
							}
							sip_freeSipAddrSpec(pTempSpec);
						}

						/*Get the last entry from the contact transport list*/
						(void)sdf_listSizeOf(&(pInitData->slContactTransport), \
								&dCount, pErr);
/*w534*/		(void)sdf_listGetAt(&(pInitData->slContactTransport), \
							(dCount-1), (Sdf_ty_pvoid *)&pTempTransport, pErr);

						/* Set the port number. */
					
						if(pTempTransport != Sdf_co_null)
						pTempTransport->dPort = Sdf_mc_atoi(tagData);
					}
					else if(IndicatePreLoadedRoute==1)
					{
						SipError err;
						Sdf_ty_u32bit dCount;

						contPort = Sdf_mc_atoi(tagData);
						
						/* Get the last header from the Route header list */
						(void)sdf_listSizeOf(&(pInitData->slPreLoadedRoute), \
							&dCount, pErr);
/*w534*/		(void)sdf_listGetAt(&(pInitData->slPreLoadedRoute), \
							(dCount-1), (Sdf_ty_pvoid *)&pRouteHdr, pErr);

						if(sip_getAddrSpecFromRouteHdr(pRouteHdr, \
							&pTempSpec,	&err) != SipFail)
						{
							if((pTempSpec->dType == SipAddrSipUri) || \
								(pTempSpec->dType == SipAddrSipSUri))
							{
								SipUrl *pUrl;

								if(sdf_ivk_uaGetUrlFromAddrSpec(pTempSpec, \
									&pUrl, pErr)!= Sdf_co_fail)
								{
/*w534*/					(void)sip_setPortInUrl(pUrl,(Sdf_ty_u16bit)\
										Sdf_mc_atoi(tagData),&err);
									sip_freeSipUrl(pUrl);
								}
							}
							sip_freeSipAddrSpec(pTempSpec);
						}
					}
					else if(IndicateRegistrar==1)
					{
						pInitData->pRegistrar->dPort=Sdf_mc_atoi(tagData);
					}
					else if(IndicateMedia==1)
					{
						pGlbMedia->pPort=(Sdf_ty_u16bit *)sdf_memget \
							(Sdf_mc_configMemId,sizeof(Sdf_ty_u16bit), pErr);
						if(pGlbMedia->pPort==Sdf_co_null)
						{
							pErr->errCode=Sdf_en_noMemoryError;
							ExitVar=1;
							break;
						}
						*(pGlbMedia->pPort)=Sdf_mc_atoi(tagData);
					}
				}
				else if(Sdf_mc_strcasecmp("protocol",attribute)==0)
				{
					if(IndicateRegistrar==1)
					{
                        if(Sdf_mc_strcasecmp(tagData,"UDP") == 0)
                        {
                           pInitData->pRegistrar->dScheme = Sdf_en_protoUdp;
                        }
                        else if(Sdf_mc_strcasecmp(tagData,"TCP") == 0)
                        {
                           pInitData->pRegistrar->dScheme = Sdf_en_protoTcp;
                        }
#ifdef SDF_TLS
                        else if(Sdf_mc_strcasecmp(tagData,"TLS") == 0)
                        {
                           pInitData->pRegistrar->dScheme = Sdf_en_protoTls;
                        }
                        else
                           pInitData->pRegistrar->dScheme = Sdf_en_protoNone;
                            
#else
                        else
                           pInitData->pRegistrar->dScheme = Sdf_en_protoNone;
#endif

					}
				}
				else if(Sdf_mc_strcasecmp("uri",attribute)==0)
				{
					if(IndicateSdp==1)
					{
						Sdf_ty_s8bit *pStr;

						pStr=Sdf_mc_strdupConfig(&tagData[2]);
						if(sdp_setUri(pGlbSdpMessage, \
						pStr,(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError, \
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to set Uri in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}
					}
				}
				else if(Sdf_mc_strcasecmp("information",attribute)==0)
				{
					if(IndicateSdp==1)
					{
						pTmpVal = Sdf_co_null;
						pTmpVal = Sdf_mc_strdupConfig(&tagData[2]);
						if(sdp_setInfo(pGlbSdpMessage, pTmpVal, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to set Info in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}
					}
				}
				else if(Sdf_mc_strcasecmp("bandwidth",attribute)==0)
				{
					if(IndicateSdp==1)
					{
						Sdf_ty_u32bit size=0;
						
						if(sdp_getBandwidthCount(pGlbSdpMessage,&size, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to get Bandwidth count in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}				
						pTmpVal = Sdf_co_null;
						pTmpVal = Sdf_mc_strdupConfig(&tagData[2]);
						if(sdp_insertBandwidthAtIndex(pGlbSdpMessage,pTmpVal, \
							size, (SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to set Bandwidth in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}
					}
				}
				else if(Sdf_mc_strcasecmp("attribute",attribute)==0)
				{
					if(IndicateSdp==1)
					{
						Sdf_ty_u32bit size=0;
						Sdf_ty_s8bit *pTemp;
						
						if(sdp_getAttrCount(pGlbSdpMessage,&size, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to get Bandwidth count in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}
						pTemp = Sdf_mc_strdupConfig(tagData);
						if(sdp_insertSdpAttrAtIndexFromString(pGlbSdpMessage, \
							pTemp,(Sdf_ty_s8bit)size,\
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to set Bandwidth in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							(void)sdf_memfree(Sdf_mc_configMemId, (Sdf_ty_pvoid *)&pTemp, pErr);
							return Sdf_co_fail;
						}
						(void)sdf_memfree(Sdf_mc_configMemId, (Sdf_ty_pvoid *)&pTemp, pErr);
					}
				}
				else if(Sdf_mc_strcasecmp("key",attribute)==0)
				{
					if(IndicateSdp==1)
					{
						pTmpVal = Sdf_co_null;
						pTmpVal = Sdf_mc_strdupConfig(&tagData[2]);
						if(sdp_setKey(pGlbSdpMessage, pTmpVal, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to set key in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}
					}
				}
				else if(Sdf_mc_strcasecmp("session",attribute)==0)
				{
					if(IndicateSdp==1)
					{
						pTmpVal = Sdf_co_null;
						pTmpVal = Sdf_mc_strdupConfig(&tagData[2]);
						if(sdp_setSession(pGlbSdpMessage, pTmpVal, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to set session in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}
					}
				}
				else if(Sdf_mc_strcasecmp("origin",attribute)==0)
				{
					if(IndicateSdp==1)
					{
						if(sdp_setSdpOriginFromString(pGlbSdpMessage, \
							tagData,(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to set origin in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}
					}
				}
				else if(Sdf_mc_strcasecmp("email",attribute)==0)
				{
					if(IndicateSdp==1)
					{
						Sdf_ty_u32bit size=0;
						
						if(sdp_getEmailCount(pGlbSdpMessage,&size, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to get email count in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}				
						pTmpVal = Sdf_co_null;
						pTmpVal = Sdf_mc_strdupConfig(&tagData[2]);
						if(sdp_insertEmailAtIndex(pGlbSdpMessage, pTmpVal,\
							size, (SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to set email in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}
					}
				}
				else if(Sdf_mc_strcasecmp("phone",attribute)==0)
				{
					if(IndicateSdp==1)
					{
						Sdf_ty_u32bit size=0;
						
						if(sdp_getPhoneCount(pGlbSdpMessage,&size, \
							(SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to get phone count in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}				
						pTmpVal = Sdf_co_null;
						pTmpVal = Sdf_mc_strdupConfig(&tagData[2]);
						if(sdp_insertPhoneAtIndex(pGlbSdpMessage, pTmpVal, \
							size, (SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_minorErrors, \
								Sdf_en_configDataError,\
								(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
								"Failed to set phone in Sdp\n",pErr);
#endif
							pErr->errCode=Sdf_en_configDataError;
							return Sdf_co_fail;
						}
					}
				}
				else if(Sdf_mc_strcasecmp("connection",attribute)==0)
				{
					if(IndicateSdp==1)
					{
						Sdf_ty_s8bit *pTemp;
						pTemp = Sdf_mc_strdupConfig(tagData);
						if(sdp_setSdpConnectionFromString(pGlbSdpMessage, \
							pTemp, (SipError *)&(pErr->stkErrCode))==SipFail)
						{
#ifdef SDF_ERROR
								sdf_fn_setError(Sdf_en_minorErrors, \
									Sdf_en_configDataError,\
									(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag(): "
									"Failed to set connection in Sdp\n",pErr);
#endif
								pErr->errCode=Sdf_en_configDataError;
								(void)sdf_memfree(Sdf_mc_configMemId, (Sdf_ty_pvoid *)&pTemp, pErr);
								return Sdf_co_fail;
						}
						(void)sdf_memfree(Sdf_mc_configMemId, (Sdf_ty_pvoid *)&pTemp, pErr);
					}
				}
				else if(Sdf_mc_strcasecmp("mediatype",attribute)==0)
				{
					if(IndicateMedia==1)
					{
						pGlbMedia->pMediaType=Sdf_mc_strdupConfig(tagData);
					}
				}
				else if(Sdf_mc_strcasecmp("numports",attribute)==0)
				{
					if(IndicateMedia==1)
					{
						pGlbMedia->pPortNum=(Sdf_ty_u32bit *) \
						sdf_memget(Sdf_mc_configMemId,sizeof(Sdf_ty_u32bit),\
							pErr);
						if(pGlbMedia->pPortNum==Sdf_co_null)
						{
							pErr->errCode=Sdf_en_noMemoryError;
							ExitVar=1;
							break;
						}
						*(pGlbMedia->pPortNum)=Sdf_mc_atoi(tagData);
					}
				}
				else if(Sdf_mc_strcasecmp("transport",attribute)==0)
				{
					if(IndicateMedia==1)
					{
						pGlbMedia->pTransport=Sdf_mc_strdupConfig(tagData);
					}
				}
				else if(Sdf_mc_strcasecmp("codecs",attribute)==0)
				{
					if(IndicateMedia==1)
					{
						pGlbMedia->pCodecs=Sdf_mc_strdupConfig(tagData);
						IndicateMediaFlag=1;
					}
				}
				else if(Sdf_mc_strcasecmp("typeoftransport",attribute)==0)
				{
					if(IndicateMedia==1)
					{
						pGlbMedia->pTransType=Sdf_mc_strdupConfig(tagData);

					}
				}
				else if(Sdf_mc_strcasecmp("mediaattribute",attribute)==0)
				{
					if(IndicateMedia==1)
					{
						Sdf_ty_s8bit* temp;

						temp=(Sdf_ty_s8bit*)sdf_memget \
							(Sdf_mc_configMemId,Sdf_mc_strlen(tagData)+1, pErr);
						
						if(temp==Sdf_co_null)
						{
							pErr->errCode=Sdf_en_noMemoryError;
							ExitVar=1;
							break;
						}

						Sdf_mc_strcpy(temp,tagData);

						/* Check for the validity of attribute line */
						if((temp[0]!='a')||(temp[1]!='='))
						{
							(void)sdf_memfree(Sdf_mc_configMemId, \
								(Sdf_ty_pvoid*)&temp, pErr);
							pErr->errCode=Sdf_en_configDataError;
							ExitVar=1;
							break;
						}

						if(sdf_listAppend(&(pGlbMedia->slAttribute), temp, \
							pErr)==Sdf_co_fail)
						{
							pErr->errCode=Sdf_en_configDataError;
							(void)sdf_memfree(Sdf_mc_configMemId, \
								(Sdf_ty_pvoid*)&temp, pErr);
							return Sdf_co_fail;
						}
					}
				}
				else if(Sdf_mc_strcasecmp("mediaconnection",attribute)==0)
				{
					if(IndicateMedia==1)
					{
						Sdf_ty_s8bit* temp;

						temp=(Sdf_ty_s8bit*)sdf_memget \
							(Sdf_mc_configMemId,Sdf_mc_strlen(tagData)+1, pErr);
						
						if(temp==Sdf_co_null)
						{
							pErr->errCode=Sdf_en_noMemoryError;
							ExitVar=1;
							break;
						}

						Sdf_mc_strcpy(temp,tagData);

						/* Check for the validity of attribute line */
						if((temp[0]!='c')||(temp[1]!='='))
						{
							(void)sdf_memfree(Sdf_mc_configMemId, \
								(Sdf_ty_pvoid*)&temp, pErr);
							pErr->errCode=Sdf_en_configDataError;
							ExitVar=1;
							break;
						}

						if(sdf_listAppend(&(pGlbMedia->slConnection), temp, \
						pErr)==Sdf_co_fail)
						{
							pErr->errCode=Sdf_en_configDataError;
							(void)sdf_memfree(Sdf_mc_configMemId, \
								(Sdf_ty_pvoid*)&temp, pErr);
							return Sdf_co_fail;
						}
					}
				}
				else if(Sdf_mc_strcasecmp("mediakey",attribute)==0)
				{	
					if(IndicateMedia==1)
					{
						pGlbMedia->pKey=Sdf_mc_strdupConfig(&tagData[2]);
					}		
				}
				else if(Sdf_mc_strcasecmp("mediainformation",attribute)==0)
				{	
					if(IndicateMedia==1)
					{
						pGlbMedia->pInfo=Sdf_mc_strdupConfig(&tagData[2]);
					}		
				}
				else if(Sdf_mc_strcasecmp("mediabandwidth",attribute)==0)
				{	
					if(IndicateMedia==1)
					{
						pGlbMedia->pBandwidth=Sdf_mc_strdupConfig(&tagData[2]);
					}		
				}
				else if(Sdf_mc_strcasecmp("any",attribute)==0)
                {
                    if(IndicateAny==1)
                    {
                        Sdf_ty_s8bit *temp;
#ifdef SDF_IMS
                        if(IndicateResponse==1)
                        {	
                            if(pRespHdrs==Sdf_co_null)
                            {
                                pErr->errCode=Sdf_en_configDataError;
                                ExitVar=1;
                                break;
                            }
                        }
                        else
                        {	
#endif
                            if(pExtraHdrs==Sdf_co_null)
                            {
                                pErr->errCode=Sdf_en_configDataError;
                                ExitVar=1;
                                break;
                            }
#ifdef SDF_IMS
                        }
#endif

                        temp=Sdf_mc_strdupConfig(tagData);	
#ifdef SDF_IMS
                        if(IndicateResponse==1)
                        {	
                            if(sip_listAppend(&(pRespHdrs->pParam->slValue), \
                                        (Sdf_ty_pvoid)temp,(SipError*)&(pErr->stkErrCode))==SipFail)
                            {
                                pErr->errCode = Sdf_en_configDataError;
                                ExitVar=1;
                                (void)sdf_memfree(Sdf_mc_configMemId, \
                                        (Sdf_ty_pvoid*)&temp, pErr);
                                break;
                            }
                        }
                        else
                        {
#endif
                            if(sip_listAppend(&(pExtraHdrs->pParam->slValue), \
                                        (Sdf_ty_pvoid)temp,(SipError*)&(pErr->stkErrCode))==SipFail)
                            {
                                pErr->errCode = Sdf_en_configDataError;
                                ExitVar=1;
                                (void)sdf_memfree(Sdf_mc_configMemId, \
                                        (Sdf_ty_pvoid*)&temp, pErr);
                                break;
                            }
#ifdef SDF_IMS
                        }
#endif	
                        IndicateAnyFlag=1;
                    }
                }
#ifdef SDF_TLS
				else if(IndicateTLSFlag == 1)
				{
					if(Sdf_mc_strcasecmp("certfile",attribute)==0)
					{
						pInitData->pSslData->pSslParams->pCertFile = \
							Sdf_mc_strdupConfig(tagData);
					}
					else if(Sdf_mc_strcasecmp("certformat",attribute)==0)
					{
						if (Sdf_mc_strcasecmp("PEM",tagData)==0)
						{
							pInitData->pSslData->pSslParams->dCertFormat = \
								Sdf_en_fileFormatPEM;
						}
						else
						{
							pInitData->pSslData->pSslParams->dCertFormat = \
								Sdf_en_fileFormatDER;
						}
					}
					else if(Sdf_mc_strcasecmp("privatekeyfile",attribute)==0)
					{
						pInitData->pSslData->pSslParams->pPrivateKeyFile = \
										 Sdf_mc_strdupConfig(tagData);
					}
					else if(Sdf_mc_strcasecmp("privatekeypassword", \
								attribute)==0)
					{
						pInitData->pSslData->pSslParams->pPrivateKeyPasswd = \
										 Sdf_mc_strdupConfig(tagData);
					}
					else if(Sdf_mc_strcasecmp("cacertdirpath",attribute)==0)
					{
						pInitData->pSslData->pSslParams->pCACertDirPath = \
										 Sdf_mc_strdupConfig(tagData);
					}
                                        /* CSR_1-5036521*/
					else if(Sdf_mc_strcasecmp("cacertfile",attribute)==0)
					{
						pInitData->pSslData->pSslParams->pCACertFile = \
										 Sdf_mc_strdupConfig(tagData);
					}					
					/* CSR_1-5036521*/
					else if(Sdf_mc_strcasecmp("ciphersuites",attribute)==0)
					{
						pInitData->pSslData->pSslParams->pCipherSuites = \
										 Sdf_mc_strdupConfig(tagData);
					}
					else if(Sdf_mc_strcasecmp("dhparamfile",attribute)==0)
					{
						pInitData->pSslData->pSslParams->pDHParamFile = \
							             Sdf_mc_strdupConfig(tagData);
					}
					else if(Sdf_mc_strcasecmp("dhparamfileformat",attribute)==0)
					{
						if (Sdf_mc_strcasecmp("PEM",tagData)==0)
						{
							pInitData->pSslData->pSslParams->\
								dDHParamFileFormat = Sdf_en_fileFormatPEM;
						}
						else
						{
							pInitData->pSslData->pSslParams->\
								dDHParamFileFormat = Sdf_en_fileFormatDER;
						}
					}
					else if(Sdf_mc_strcasecmp("certverifymode",attribute)==0)
					{
						pInitData->pSslData->pSslParams->dCertVerifyMode = \
							Sdf_mc_atoi(tagData);
					}
				}
#endif /* SDF_TLS */
			}
		} /* End of while loop */

		if(ExitVar==1)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag():"
				"Error in configuration input\n",pErr);
#endif
			return Sdf_co_fail;
		}
		*pPosition=*pPosition+Sdf_mc_strlen(endTagExpected);


		if((Sdf_mc_strcasecmp("</property>",endTagExpected)==0))
		{
			if(IndicateMediaFlag==1)
			{
				Sdf_ty_u16bit port;
				Sdf_ty_u32bit portnum;
				Sdf_ty_u32bit size=0;

				if(pGlbMedia->pPort!=Sdf_co_null)
				{
					port=*(pGlbMedia->pPort);
				}
				else
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_configDataError,\
						(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag():  "
						"Media Port Missing in configuration input\n",pErr);
#endif
					pErr->errCode=Sdf_en_configDataError;
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia,\
					pErr);
					return Sdf_co_fail;
				}
				if(pGlbMedia->pPortNum!=Sdf_co_null)
				{
					portnum=*(pGlbMedia->pPortNum);
				}
				else
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_configDataError,\
						(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag():  "
						"No of ports in Media missing. Setting it to "
						"default 0\n",pErr);
#endif
					pErr->errCode=Sdf_en_configDataError;
					portnum=0;
				}
				if((pGlbMedia->pTransport == Sdf_co_null)|| \
				  (pGlbMedia->pCodecs	  == Sdf_co_null))
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_configDataError,\
						(Sdf_ty_s8bit *)"sdf_fn_xmlParseTag():  "
						"Error in Media of configuration input\n",pErr);
#endif
					pErr->errCode=Sdf_en_configDataError;

					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pMediaType,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pTransport,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pTransType,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pPort,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pPortNum,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pCodecs,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pBandwidth,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pInfo,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pKey,pErr);

					
					if(sdf_listDeleteAll(&(pGlbMedia->slAttribute), \
					  pErr)==Sdf_co_fail)	
						return Sdf_co_fail;
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia,\
					pErr);
					return Sdf_co_fail;
				}


				/* Form the SDP Media Structure */
/*w534*/	(void)sdf_ivk_uaFormSdpMedia(pGlbMedia->pMediaType,port,portnum, \
					pGlbMedia->pTransport,pGlbMedia->pCodecs, \
					pGlbMedia->pTransType,&(pGlbMedia->slAttribute), \
					&(pGlbMedia->slConnection),pGlbMedia->pBandwidth, \
					pGlbMedia->pInfo,pGlbMedia->pKey, &pGlbSdpMedia, pErr );

				/* Append it to the current Sdp message Body */
				if(SipFail==sdp_getMediaCount(pGlbSdpMessage, \
					&size,(SipError*)&(pErr->stkErrCode)))
					return Sdf_co_fail;

				if(SipFail==sdp_insertMediaAtIndex(pGlbSdpMessage \
					,pGlbSdpMedia,size ,(SipError*)&(pErr->stkErrCode)))
					return Sdf_co_fail;

				sip_freeSdpMedia(pGlbSdpMedia);
				if(sdf_listDeleteAll(&(pGlbMedia->slAttribute), \
				  pErr)==Sdf_co_fail)	
				  return Sdf_co_fail;
				if(sdf_listDeleteAll(&(pGlbMedia->slConnection), \
				  pErr)==Sdf_co_fail)	
				  return Sdf_co_fail;

					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pMediaType,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pTransport,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pTransType,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pPort,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pPortNum,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pCodecs,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pBandwidth,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pInfo,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia->\
					pKey,pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid*)&pGlbMedia,\
					pErr);
				IndicateMediaFlag=0;
			}
			else if(IndicateAnyFlag==1)
            {
#ifdef SDF_IMS
                if(IndicateResponse==1)
                {
                    if(sdf_listAppend(&(pInitData->slRespHeadersList), \
                                (Sdf_ty_pvoid)pRespHdrs,pErr)==Sdf_co_fail)
                    {
                        (void)sdf_ivk_uaFreeConfigExtraHeaders(pRespHdrs);
                        return Sdf_co_fail;
                    }
                    IndicateAnyFlag=0;
                    IndicateResponse=0;
                    pRespHdrs=Sdf_co_null;
                }
                else
                {	
#endif
                    if(sdf_listAppend(&(pInitData->slExtraHeadersList), \
                                (Sdf_ty_pvoid)pExtraHdrs,pErr)==Sdf_co_fail)
                    {
                        (void)sdf_ivk_uaFreeConfigExtraHeaders(pExtraHdrs);
                        return Sdf_co_fail;
                    }
                    IndicateAnyFlag=0;
                    pExtraHdrs=Sdf_co_null;
#ifdef SDF_IMS
                }
#endif
            }    
		}
	}

	if(ExitVar==1)
	{
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG--Exiting "
			"sdf_fn_xmlParseTag\n");
		return Sdf_co_fail;
	}
	else
	{
		pErr->errCode=Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG--Exiting "
			"sdf_fn_xmlParseTag\n");
		return 	Sdf_co_success;
	}
}

/*********************************************************
** FUNCTION: sdf_ivk_uaParseConfigData
**
** DESCRIPTION: This function parses the input using a XML
**	parser, and sets it into the passed user profile structure
**
**********************************************************/
Sdf_ty_retVal  sdf_ivk_uaParseConfigData
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *pInput,
	Sdf_ty_s32bit  count,
	Sdf_st_initData *pInitData,
	Sdf_st_error *pErr)	
#else
	(pInput, count,pInitData, pErr)
	Sdf_ty_s8bit *pInput;
	Sdf_ty_s32bit  count;
	Sdf_st_initData *pInitData;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_s32bit position=0,BreakVar=0;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaParseConfigData \n");

	/*
	 * Initialize all Indicate variables to 0
	 */
	IndicateContact=0;
	IndicateFrom=0;
	IndicateRegistrar=0;
	glbIndicateRegistrar=0;
	IndicateMedia=0;
	IndicateSdp=0;
	IndicateAny=0;
	IndicateAnyFlag=0;
	IndicateMediaFlag=0;
	IndicatePreLoadedRoute=0;
    IndicateTLSFlag=0;
#ifdef SDF_TLS
    glbIndicateTLSFlag=0;
#endif
#ifdef SDF_IMS
  IndicateResponse=0;
#endif


#ifdef SDF_PARAMVALIDATION
	if(pErr==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaParseConfigData(): Invalid error parameter");	
#endif
		return Sdf_co_fail;
	}
	if(pInitData==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
			"pInitData parameter invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* Param validation ends. */


	/* Init the Garbage Collecteor GC */
	if(sip_initSipParam(&GC,(SipError *)&(pErr->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_configDataError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
			"Error in Configuration",pErr);	
#endif
		pErr->errCode=Sdf_en_configDataError;
		return Sdf_co_fail;
	}
	GC->pName=Sdf_mc_strdupConfig("GARBAGE");

	/* Init the message body structure */
	if(sip_initSipMsgBody(&pMsgBody,SipSdpBody, \
		(SipError *)&(pErr->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_configDataError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
			"Could not initialize Message Body Structure",pErr);
#endif
		sip_freeSipParam(GC);
		pErr->errCode=Sdf_en_configDataError;
		return Sdf_co_fail;
	}
	if(sip_initSdpMessage(&pGlbSdpMessage,(SipError *)&(pErr->stkErrCode)) \
		==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_configDataError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
			"Could not initialize Sdp Message Structure",pErr);
#endif
		sip_freeSipMsgBody(pMsgBody);
		pErr->errCode=Sdf_en_configDataError;
		sip_freeSipParam(GC);
		return Sdf_co_fail;
	}

	if(pInitData->pFrom == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_configDataError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
			"From header not initialized in the pInitData structure",pErr);
#endif
		sip_freeSipMsgBody(pMsgBody);
		sip_freeSdpMessage(pGlbSdpMessage);
		sip_freeSipParam(GC);
		pErr->errCode=Sdf_en_configDataError;
		return Sdf_co_fail;
	}
	
	while(position<count)
	{
		sdf_fn_skipSpaces(pInput,&position);
		if(pInput[position]=='<' )
		{
      if(!Sdf_mc_strncmp(&pInput[position],"<subslayer>",11))
	    {
		    if(!sdf_fn_skipCharsForward(pInput,&position,
                                   (Sdf_ty_s8bit *)"</subslayer>"))
		    {
			    return Sdf_co_fail;
		    }
        continue;
	    }  
      if(sdf_fn_xmlParseTag(pInput,&position,pInitData,pErr) == 
                                               Sdf_co_fail )
			{
			  BreakVar=1;
			  break;
			}
		}
	}

	/* Parsing Done. Whether Success or Failure, GC has to be freed */
	sip_freeSipParam(GC);

	if(BreakVar==1)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_criticalErrors,Sdf_en_configDataError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
			"Error in Configuration input",pErr);	
#endif
		pErr->errCode=Sdf_en_configDataError;
		sip_freeSipMsgBody(pMsgBody);
		sip_freeSdpMessage(pGlbSdpMessage);
		return Sdf_co_fail;
	}
	else
	{
		Sdf_ty_u32bit contNum;
		SipFromHeader *pFromHeader;
		
		pFromHeader=(SipFromHeader *)(pInitData->pFrom->pHeader);
		if(pFromHeader->pAddrSpec == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_criticalErrors,Sdf_en_configDataError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
				"From header Information missing in configuration input",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			sip_freeSipMsgBody(pMsgBody);
			sip_freeSdpMessage(pGlbSdpMessage);
			return Sdf_co_fail;
		}

		/*
		 * Set the port value of the TransportInfo and From info of the
		 * user profile to 5060 if it was zero
		 */
		if(pInitData->pTransport->dPort==0)
		{
#ifdef SDF_TLS
			if(pFromHeader->pAddrSpec->dType==SipAddrSipSUri)
			{
				pInitData->pTransport->dPort = Sdf_mc_defaultSipSPort;
				if((((SipFromHeader *)pInitData->pFrom->pHeader)->pAddrSpec\
					->u.pSipUrl->dPort)==Sdf_co_null)
					((SipFromHeader *)pInitData->pFrom->pHeader)->pAddrSpec\
						->u.pSipUrl->dPort = (Sdf_ty_u16bit *)sdf_memget\
						(Sdf_mc_configMemId,sizeof(Sdf_ty_u16bit),pErr);
				*(((SipFromHeader *)pInitData->pFrom->pHeader)->pAddrSpec\
					->u.pSipUrl->dPort) = Sdf_mc_defaultSipSPort;
			}
			else
#endif /* SDF_TLS */
			if(pFromHeader->pAddrSpec->dType==SipAddrSipUri)
			{
	
				pInitData->pTransport->dPort = Sdf_mc_defaultSipPort;
				if((((SipFromHeader *)pInitData->pFrom->pHeader)->pAddrSpec\
					->u.pSipUrl->dPort)==Sdf_co_null)
					((SipFromHeader *)pInitData->pFrom->pHeader)->pAddrSpec\
						->u.pSipUrl->dPort = (Sdf_ty_u16bit *)sdf_memget\
						(Sdf_mc_configMemId,sizeof(Sdf_ty_u16bit),pErr);
				*(((SipFromHeader *)pInitData->pFrom->pHeader)->pAddrSpec\
					->u.pSipUrl->dPort) = Sdf_mc_defaultSipPort;
			}
			else
				pInitData->pTransport->dPort = Sdf_mc_defaultSipPort;
		}

#ifdef SDF_TLS
		/* Do the Validation of TLS Information */
		if (glbIndicateTLSFlag == 1)
		{
			Sdf_ty_retVal   dRetVal = Sdf_co_success;

			if (sdf_fn_uaFormSslData(\
						(Sdf_ty_pvoid*)pInitData->pSslData->pSslParams, \
						pInitData->pSslData, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_criticalErrors,Sdf_en_configDataError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
					"Forming the SslData failed.",pErr);
#endif
				pErr->errCode=Sdf_en_configDataError;
				sip_freeSipMsgBody(pMsgBody);
				sip_freeSdpMessage(pGlbSdpMessage);
				dRetVal = Sdf_co_fail;
			}

			if (dRetVal == Sdf_co_fail)
				return Sdf_co_fail;
		}
#endif /* SDF_TLS */

		/* Do the Validation of Register Information */
		
		/* If Register tag is present and the Register Information is missing
		 * then throw an error
		 */
		if(glbIndicateRegistrar==1)
		{
#ifndef SDF_LOOKUP
			if((pInitData->pRegistrar->dPort==0)|| \
				(pInitData->pRegistrar->pIp==Sdf_co_null))
#else
			/* For the DNS SRV query */	
			if(pInitData->pRegistrar->pIp==Sdf_co_null)
#endif
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_criticalErrors,Sdf_en_configDataError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
					"Registrar Info missing in configuration input",pErr);
#endif
				pErr->errCode=Sdf_en_configDataError;
				sip_freeSipMsgBody(pMsgBody);
				sip_freeSdpMessage(pGlbSdpMessage);
				return Sdf_co_fail;
			}
		}
		/* else if Register tag is absent then fill-up the Register information
		 * from the From field
		 */
#ifndef SDF_UASSF
		else
		{
			/* Check the type of the uri and fill-up the Registar
			 *  information from the Addrspec.
			 *  If  AddrType is other than SipAddrSipUri then return failuere
			 */
			SipAddrSpec *pTempSpec=Sdf_co_null;

			if(sip_getAddrSpecFromFromHdr(pInitData->pFrom,	&pTempSpec,\
					   (SipError *)&(pErr->stkErrCode)) != SipFail)
			{
				SipUrl *pUrl;

				if(sdf_ivk_uaGetUrlFromAddrSpec(pTempSpec, &pUrl, \
						pErr) != Sdf_co_fail)
				{
					/* 
					 * Get the Host and Port from the SipUrl and set it to
					 *  the  pInitData->pRegistrar->pIp, 
					 *  pInitData->pRegistrar->dPort	 */

					pInitData->pRegistrar->pIp = \
							Sdf_mc_strdupConfig(pUrl->pHost);
					pInitData->pRegistrar->dPort = \
							Sdf_mc_defaultSipPort;
					sip_freeSipUrl(pUrl);
				}
				else
				{
					/* Return failure for other type of uri */
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_criticalErrors, \
						Sdf_en_configDataError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
						"AddrSpec Type is not SipAddrSipUri ",pErr);
#endif
					pErr->errCode=Sdf_en_configDataError;
					sip_freeSipMsgBody(pMsgBody);
					sip_freeSdpMessage(pGlbSdpMessage);
					sip_freeSipAddrSpec(pTempSpec);
					return Sdf_co_fail;
				}
				sip_freeSipAddrSpec(pTempSpec);
			}
			else
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_criticalErrors, \
					Sdf_en_configDataError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
					"Faile to get AddrSpec from the From field",pErr);
#endif
				pErr->errCode=Sdf_en_configDataError;
				sip_freeSipMsgBody(pMsgBody);
				sip_freeSdpMessage(pGlbSdpMessage);
				return Sdf_co_fail;
			}
		}
#endif
		(void)sdf_listSizeOf(&(pInitData->slContact), &contNum, pErr);

		if(contNum <= 0)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_criticalErrors,Sdf_en_configDataError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
				"Contact Information Missing in configuration input",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			sip_freeSipMsgBody(pMsgBody);
			sip_freeSdpMessage(pGlbSdpMessage);
			return Sdf_co_fail;
		}

		
		/*Before Appending the message body Add the mandatory line
		 * v,o,s,c,t needed in a SDP message.
		 */
		if ((sdf_fn_uaCheckAndAddMandatoryLines(pInitData, \
			pGlbSdpMessage, pErr)) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_criticalErrors,Sdf_en_configDataError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
				"Failed to add Mandatory Lines",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			sip_freeSipMsgBody(pMsgBody);
			sip_freeSdpMessage(pGlbSdpMessage);
			return Sdf_co_fail;
		}
		/* Convert the SDP Message Body into session param structure*/
		if(sdf_ivk_uaFormSessionParam(pGlbSdpMessage, \
			pInitData->pSessionParam,Sdf_en_convertLocal,pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_configDataError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
				"Could not Convert SdpMessage to Session Param",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			sip_freeSipMsgBody(pMsgBody);
			sip_freeSdpMessage(pGlbSdpMessage);
			return Sdf_co_fail;
		}
		if(sip_setSdpInMsgBody(pMsgBody,pGlbSdpMessage, \
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_configDataError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
				"Could not initialize Sdp Message Structure",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			sip_freeSipMsgBody(pMsgBody);
			sip_freeSdpMessage(pGlbSdpMessage);
			return Sdf_co_fail;
		}
		
		sip_freeSdpMessage(pGlbSdpMessage);
		/*
		 * Append the msg body structure to the list inside the 
		 * user profile.
		 */

		if (sdf_listAppend(&(pInitData->slMsgBodyList),(Sdf_ty_pvoid)pMsgBody,\
				pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_configDataError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
				"Could not append the message body to the init data's "
				"message body list",pErr);
#endif
			pErr->errCode=Sdf_en_configDataError;
			sip_freeSipMsgBody(pMsgBody);
			return Sdf_co_fail;
		}


		/* 
		 * Parsing is complete and is successful. 
		 * Tracing Code is present in this block
		 */
#ifdef SDF_TRACE
		{
			Sdf_ty_s8bit *pBuf=Sdf_co_null;
			Sdf_ty_s8bit pExtraHdrMesg[]= \
				"The following Headers will be sent out in ALL Requests\n";
			Sdf_ty_uaTraceLevel enLevel;
			Sdf_ty_u32bit dListSize=0,i;

			(void)sdf_ivk_uaGetTraceLevel(&enLevel,pErr);

			/* Form the Registrar Header Info */
			sdf_fn_uaFormRegistrarBuffer(pInitData,&pBuf,pErr);
			sdf_fn_trace(enLevel,Sdf_en_configTraces,pBuf, \
			(Sdf_mc_strlen(pBuf)),pErr);
			(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid *)&pBuf,pErr);

		

			/* Form the Media Header(s) */

			/*
			 * Form the Unknown headers from the Configuaration data. 
			 * Only headers that have the tag attribute 'addalways=yes' 
			 * will be shown in detailed trace
			 */
			(void)sdf_listSizeOf(&(pInitData->slExtraHeadersList), &dListSize,  \
				pErr);

			pBuf=Sdf_mc_strdupConfig(pExtraHdrMesg);
			sdf_fn_trace(enLevel,Sdf_en_configTraces,pBuf, \
			(Sdf_mc_strlen(pBuf)),pErr);
			(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid *)&pBuf,pErr);

			for(i=0;i<dListSize;i++)
			{
				Sdf_st_configExtraHeaders *pConf;
				if(sdf_listGetAt(&(pInitData->slExtraHeadersList),i, \
					(Sdf_ty_pvoid*)&pConf,pErr)	==Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_criticalErrors, \
						Sdf_en_configDataError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaParseConfigData():  "
						"Failed to Get Unknown Header from List", pErr);
#endif
					pErr->errCode=Sdf_en_configDataError;
					return Sdf_co_fail;
				}
				/* Form the Unknown Header Buffer only if addalways=yes */
				if(Sdf_mc_strcasecmp((Sdf_ty_s8bit*)pConf->pData,"yes")==0)
				{
					sdf_fn_uaFormExtraHeaderBuffer(pConf,&pBuf,pErr);
					sdf_fn_trace(enLevel,Sdf_en_configTraces,pBuf, \
					(Sdf_mc_strlen(pBuf)),pErr);
					(void)sdf_memfree(Sdf_mc_configMemId,(Sdf_ty_pvoid *)&pBuf,pErr);
				}
			}

		}
#endif
		sdf_fn_debug((Sdf_ty_s8bit *)"\nSDF_DEBUG - "
			"Exiting sdf_ivk_uaParseConfigData \n");
		pErr->errCode=Sdf_en_noUaError;
		return Sdf_co_success;
	}
}


#ifdef SDF_TRACE
/***********************************************************************
** FUNCTION: sdf_fn_uaFormRegistrarBuffer
**
** DESCRIPTION: Forms the Extra header buffer from the pData
**
************************************************************************/
void sdf_fn_uaFormRegistrarBuffer
(Sdf_st_initData *pData, 
Sdf_ty_s8bit **ppBuf,
Sdf_st_error *pError)
{

	Sdf_ty_s8bit temp[Sdf_mc_maxConfigFileLen];
	Sdf_ty_u32bit len = 0,dname=0;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_fn_uaFormRegistrarBuffer");
	
	/* Registrar: + name + ip + sip: + <> + : + '\0' */
	len = dname + Sdf_mc_strlen(pData->pRegistrar->pIp)+ \
		sizeof(Sdf_ty_u16bit)+101;
	
	*ppBuf = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_configMemId,len,pError);
	if (*ppBuf == Sdf_co_null)
	{
		pError->errCode = Sdf_en_noMemoryError;
		return;
	}

	Sdf_mc_strcpy(*ppBuf,"Setting Registrar in user profile structure:\n");
	Sdf_mc_strcat(*ppBuf,"Registrar:");
	Sdf_mc_strcat(*ppBuf,"<");
	Sdf_mc_strcat(*ppBuf,"sip:");
	Sdf_mc_strcat(*ppBuf,pData->pRegistrar->pIp);
	Sdf_mc_strcat(*ppBuf,":");
	(void)sdf_fn_uaSprintf(temp,"%u",pData->pRegistrar->dPort);
	Sdf_mc_strcat(*ppBuf,temp);
	Sdf_mc_strcat(*ppBuf,">");
	
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaFormRegistrarBuffer");
}

/***********************************************************************
** FUNCTION: sdf_fn_uaFormMediaBuffer
**
** DESCRIPTION: Forms the Media header buffer from the SdpMedia structure
**
************************************************************************/
void sdf_fn_uaFormMediaBuffer
(SdpMedia *pData, 
Sdf_ty_s8bit **ppBuf,
Sdf_st_error *pError)
{
	Sdf_ty_s8bit dPort[Sdf_co_characterBufferSize];
	Sdf_ty_u32bit tsize=0, tindex=0;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_fn_uaFormMediaBuffer");
	*ppBuf = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_configMemId,1000,pError);
	if (*ppBuf == Sdf_co_null)
	{
		pError->errCode = Sdf_en_noMemoryError;
		return;
	}

	Sdf_mc_strcpy(*ppBuf,"m=");
	Sdf_mc_strcat(*ppBuf,pData->pMediaValue);
	Sdf_mc_strcat(*ppBuf," ");
	(void)sdf_fn_uaSprintf (dPort,"%u",pData->dPort);	
	Sdf_mc_strcat(*ppBuf,dPort);
	if (pData->pPortNum != Sdf_co_null)
			{
				Sdf_mc_strcat(*ppBuf,"/");
				(void)sdf_fn_uaSprintf(dPort,"%u",*(pData->pPortNum));
				Sdf_mc_strcat(*ppBuf, dPort);
			}
	Sdf_mc_strcat(*ppBuf," ");
	Sdf_mc_strcat(*ppBuf,pData->pProtocol);
	Sdf_mc_strcat(*ppBuf," ");
	Sdf_mc_strcat(*ppBuf,pData->pFormat);
	Sdf_mc_strcat(*ppBuf,"\r\n");

	(void)sip_listSizeOf( &(pData->slAttr), &tsize,(SipError *)&(pError->stkErrCode));
	tindex = 0;
	while (tindex < tsize)
	{
		SdpAttr *a;
/*w534*/		(void)sip_listGetAt(&(pData->slAttr), tindex, (Sdf_ty_pvoid *)&a, \
			(SipError *)&(pError->stkErrCode));
		Sdf_mc_strcat(*ppBuf, "a=");
		Sdf_mc_strcat(*ppBuf,a->pName);
		if (a->pValue != Sdf_co_null)
		/* Since attribute can be <attribute:value> or <attribute> itself */
		{
			Sdf_mc_strcat(*ppBuf,":");
			Sdf_mc_strcat(*ppBuf,a->pValue);
		}	
		Sdf_mc_strcat(*ppBuf,"\r\n");
		tindex++;
	}
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaFormMediaBuffer");

	return;
}


/***********************************************************************
** FUNCTION: sdf_fn_uaFormExtraHeaderBuffer
**
** DESCRIPTION: Forms the Extra header buffer from the pData
**
************************************************************************/
void sdf_fn_uaFormExtraHeaderBuffer
(Sdf_st_configExtraHeaders *pData, 
Sdf_ty_s8bit **ppBuf, 
Sdf_st_error *pError)
{
	Sdf_ty_u32bit len=0,dSize=0, j=0, memsize=0;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_fn_uaFormExtraHeaderBuffer");

	/* Find out the size of the header values */
	(void)sip_listSizeOf( &(pData->pParam->slValue), &dSize,(SipError *)&(pError->stkErrCode));
	for(j=0;j<dSize;j++)
	{
		Sdf_ty_s8bit *a;
/*w534*/		(void)sip_listGetAt(&(pData->pParam->slValue),j, (Sdf_ty_pvoid *)&a, \
			(SipError *)&(pError->stkErrCode));
		memsize += Sdf_mc_strlen(a);	
	}
	len = memsize + Sdf_mc_strlen(pData->pParam->pName) + dSize + 5;
	*ppBuf=(Sdf_ty_s8bit *)sdf_memget(Sdf_mc_configMemId,len,pError);	
	if (*ppBuf == Sdf_co_null)
	{
		pError->errCode = Sdf_en_noMemoryError;
		return;
	}

	if(pData->pParam->pName!=Sdf_co_null)
		Sdf_mc_strcpy(*ppBuf,pData->pParam->pName);
	else
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_configDataError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormExtraHeaderBuffer(): "
			"Header Name Missing",pError);
#endif
		pError->errCode=Sdf_en_configDataError;
		return;
	}
	Sdf_mc_strcat(*ppBuf,":");
	for(j=0;j<dSize;j++)
	{
		Sdf_ty_s8bit *a;
/*w534*/		(void)sip_listGetAt(&(pData->pParam->slValue),j, (Sdf_ty_pvoid *)&a, \
			(SipError *)&(pError->stkErrCode));
		Sdf_mc_strcat(*ppBuf,a);
		if(j!=(dSize-1))
			Sdf_mc_strcat(*ppBuf,",");
	}
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaFormExtraHeaderBuffer");
}
#endif
#endif/*if ndef ICF_PORT_SYMBIAN*/


/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetDefaultFrom
**
** DESCRIPTION: Sets the "From" parameters into the profile data structure
**				(Sdf_st_initData structure)
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetDefaultFrom 
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData,
	 Sdf_ty_s8bit *pName,
	 Sdf_ty_s8bit *pAddr,
	 Sdf_ty_u16bit dPort,
	 Sdf_ty_s8bit *pScheme,
	 Sdf_st_error *pError)
#else
	( pInitData,pName, pAddr,dPort, pScheme, pError )
	 Sdf_st_initData *pInitData;
	 Sdf_ty_s8bit *pName;
	 Sdf_ty_s8bit *pAddr;
	 Sdf_ty_u16bit dPort;
	 Sdf_ty_s8bit *pScheme;
	 Sdf_st_error * pError;
#endif
{
	SipAddrSpec *pAddrSpec = Sdf_co_null;
	Sdf_ty_s8bit	*pTmpValue = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetDefaultFrom");
	
#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetDefaultFrom(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultFrom(): "
			"pInitData param passed is Invalid",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pAddr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultFrom(): "
			"pAddr param passed is Invalid",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return  Sdf_co_fail;
	}
#endif

	/* Free the existing header if present */
	if (pInitData->pFrom != Sdf_co_null)
	{
		sip_freeSipHeader(pInitData->pFrom);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&(pInitData->pFrom), \
			pError);
	}

	/*
	 * Make a SipAddrSpec structure from the values passed to this API.
	 */
	if (sdf_ivk_uaMakeAddrSpecFromConstituents(&pAddrSpec, pAddr, \
		dPort, pScheme, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultFrom():  Failed to make "
			 "an addr spec from the parameters passed.", pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	/*The User Could have set a tel Url, In that case, we will have to parse
	 * and check its correctness. If Some other URI scheme is specfied,we will
	 * have to return Failure.
	 */
	if(pAddrSpec->dType==SipAddrReqUri)
	{
#ifdef SDF_TEL
		TelUrl *pTelUrl = Sdf_co_null;
#endif
#ifdef SDF_IM
		ImUrl  *pImUrl = Sdf_co_null;
#endif
		Sdf_ty_bool	dIsTelUrl	= Sdf_co_false;
		Sdf_ty_bool	dIsImUrl	= Sdf_co_false;
		SipBool		dRetVal = SipSuccess;
		
#ifdef SDF_TEL
		if(sip_isTelUrl(pAddrSpec,(SipError*)&(pError->stkErrCode))==SipSuccess)
		{
			dIsTelUrl = Sdf_co_true;
			dRetVal = sip_getTelUrlFromAddrSpec(pAddrSpec,&pTelUrl,\
							(SipError*)&(pError->stkErrCode));
			if(dRetVal == SipSuccess)
				sip_freeTelUrl(pTelUrl);
		}
#endif
#ifdef SDF_IM
		if(dIsTelUrl != Sdf_co_true)
		{
			if(sip_isImUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
					== SipSuccess)
			{
				dIsImUrl = Sdf_co_true;
				dRetVal = sip_getImUrlFromAddrSpec(pAddrSpec,&pImUrl,\
							(SipError*)&(pError->stkErrCode));
				if(dRetVal == SipSuccess)
					sip_freeImUrl(pImUrl);
			}
		}
#endif
		if((dIsTelUrl == Sdf_co_false) && (dIsImUrl == Sdf_co_false))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_notImplemented,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultFrom():"
		  		"Invalid Scheme in From Header", pError);
#endif 
			pError->errCode = Sdf_en_notImplemented;
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}

		if( dRetVal == SipFail )
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_notImplemented,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultFrom():"
		  		"Wrong Syntax in From Header", pError);
#endif 
			pError->errCode = Sdf_en_notImplemented;
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}
	}
				
	/* 
	 * Initialize a From header and set the AddrSpec in it.
	 */
	if (sip_initSipHeader(&pInitData->pFrom, SipHdrTypeFrom, \
		(SipError*)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultFrom( ): "
			"Failed to initialize From SipHeader",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	if (sip_setAddrSpecInFromHdr(pInitData->pFrom, pAddrSpec, \
		(SipError*)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipHeader(pInitData->pFrom);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultFrom( ): "
			"Failed to set addrspec in From header",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	sip_freeSipAddrSpec(pAddrSpec);

	/*  Set the display name */
	if (pName != Sdf_co_null)
	{
		pTmpValue = Sdf_mc_strdupCallHandler(pName);
		
		if (sip_setDispNameInFromHdr(pInitData->pFrom, pTmpValue, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultFrom( ): "
				"Failed to set DisplayName in From header", pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
	}
			
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaSetDefaultFrom");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/*******************************************************************
** FUNCTION: sdf_ivk_uaAddDefaultContact
**
** DESCRIPTION: Adds a Contact address to the call profile data structure
**				(Sdf_st_initData structure)
**
********************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddDefaultContact 
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData,
	 Sdf_ty_s8bit *pName,
	 Sdf_ty_s8bit *pAddr,
	 Sdf_ty_u16bit dPort,
	 Sdf_ty_s8bit *pScheme,
	 Sdf_st_error *pError)
#else
	( pInitData,pName, pAddr,dPort, pScheme, pError )
	 Sdf_st_initData *pInitData;
	 Sdf_ty_s8bit *pName;
	 Sdf_ty_s8bit *pAddr;
	 Sdf_ty_u16bit dPort;
	 Sdf_ty_s8bit *pScheme;
	 Sdf_st_error *pError;
#endif
{
	SipAddrSpec *pAddrSpec = Sdf_co_null;
	SipHeader *pContact = Sdf_co_null;
	Sdf_ty_s8bit  *pTmpValue = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaAddDefaultContact");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaAddDefaultContact(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultContact(): "
			"pInit param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pAddr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultContact(): "
			"pAddr param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return  Sdf_co_fail;
	}
#endif

	/*
	 * Make a SipAddrSpec structure from the values passed to this API.
	 */
	if (sdf_ivk_uaMakeAddrSpecFromConstituents(&pAddrSpec, pAddr, \
		dPort, pScheme, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultContact():  Failed to make "
			 "an addr spec from the parameters passed.", pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	/*The User Could have set a tel Url, In that case, we will have to parse
	 * and check its correctness. If Some other URI scheme is specfied,we will
	 * have to return Failure.
	 */
	if(pAddrSpec->dType==SipAddrReqUri)
	{
#ifdef SDF_TEL
		TelUrl *pTelUrl = Sdf_co_null;
#endif
#ifdef SDF_IM
		ImUrl  *pImUrl = Sdf_co_null;
#endif
		Sdf_ty_bool	dIsTelUrl	= Sdf_co_false;
		Sdf_ty_bool	dIsImUrl	= Sdf_co_false;
		SipBool		dRetVal = SipSuccess;
		
#ifdef SDF_TEL
		if(sip_isTelUrl(pAddrSpec,(SipError*)&(pError->stkErrCode))==SipSuccess)
		{
			dIsTelUrl = Sdf_co_true;
			dRetVal = sip_getTelUrlFromAddrSpec(pAddrSpec,&pTelUrl,\
							(SipError*)&(pError->stkErrCode));
			if( dRetVal == SipSuccess)
				sip_freeTelUrl(pTelUrl);
		}
#endif
#ifdef SDF_IM
		if(dIsTelUrl != Sdf_co_true)
		{
			if(sip_isImUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
					== SipSuccess)
			{
				dIsImUrl = Sdf_co_true;
				dRetVal = sip_getImUrlFromAddrSpec(pAddrSpec,&pImUrl,\
							(SipError*)&(pError->stkErrCode));
				if(dRetVal == SipSuccess)
					sip_freeImUrl(pImUrl);
			}
		}
#endif
		if((dIsTelUrl == Sdf_co_false) && (dIsImUrl == Sdf_co_false))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_notImplemented,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultContact():"
		  		"Invalid Scheme in Contact Header", pError);
#endif 
			pError->errCode = Sdf_en_notImplemented;
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}

		if( dRetVal == SipFail )
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_notImplemented,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultContact():"
		  		"Wrong Syntax in Contact Header", pError);
#endif 
			pError->errCode = Sdf_en_notImplemented;
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}
	}

	/* 
	 * Initialize a Contact header and set the AddrSpec in it.
	 */
	if (sip_initSipHeader(&pContact, SipHdrTypeContactNormal, \
		(SipError*)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultContact(): "
			"Failed to initialize SipHeader Contact",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	if (sip_setAddrSpecInContactHdr(pContact, pAddrSpec, \
		(SipError*)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipHeader(pContact);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultContact(): "
			"Failed to set addrspec in Contact header",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	/* Free the local reference */
	sip_freeSipAddrSpec(pAddrSpec);

	/* 
	 * Set the display name in the Contact header
	 */
	if (pName != Sdf_co_null)
	{
		pTmpValue = Sdf_mc_strdupCallHandler(pName);
		
		if (sip_setDispNameInContactHdr(pContact, pTmpValue, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultContact(): "
				"Failed to set DisplayName in Contact header",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
        /* klock tool error fix */
		sip_freeSipHeader(pContact);
		(void)sdf_memfree(0,(Sdf_ty_pvoid *)&pContact, pError);

			return Sdf_co_fail;
		}
	}

	/* 
	 * Append the new Contact to the existing list of Contacts
	 */
	if (sdf_listAppend(&(pInitData->slContact), pContact, \
		pError) == Sdf_co_fail)
	{
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipHeader(pContact);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultContact(): "
			"Failed to add Contact to list of contacts in InitData",pError);
#endif
		pError->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}


	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaAddDefaultContact");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetDefaultRegistrar
**
** DESCRIPTION: Sets the Registrar information in the call profile
**				(Sdf_st_initData structure)
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetDefaultRegistrar 
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData,
	 Sdf_ty_s8bit *pRegistrar,
	 Sdf_ty_u32bit dPort,
	 Sdf_st_error *pErr)
#else
	( pInitData, pRegistrar, dPort, pErr )
	 Sdf_st_initData *pInitData;
	 Sdf_ty_s8bit *pRegistrar;
	 Sdf_ty_u32bit dPort;
	 Sdf_st_error *pErr;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaSetDefaultRegistrar");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetDefaultRegistrar():  pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultRegistrar(): "
			"pInitData param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;

		return Sdf_co_fail;
	}	
#endif

	/*
	 * Free the existing Registrar IP if any, and set the new IP/port
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
		(Sdf_ty_pvoid*)&(pInitData->pRegistrar->pIp),pErr);

	pInitData->pRegistrar->pIp =Sdf_mc_strdupCallHandler(pRegistrar);

	pInitData->pRegistrar->dPort = dPort;
	
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetDefaultRegistrar");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;

}



/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetDefaultTransport
**
** DESCRIPTION: Sets the IP and Port on which the user agent will bind
**  and listen for incoming messages. This will also be used to insert
**	the Via header for outgoing requests from the user agent.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetDefaultTransport 
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData,
	 Sdf_ty_s8bit *pIp,
	 Sdf_ty_u32bit dPort,
	 Sdf_st_error *pErr)
#else
	( pInitData,pIp, dPort, pErr )
	 Sdf_st_initData *pInitData;
	 Sdf_ty_s8bit *pIp;
	 Sdf_ty_u32bit dPort;
	 Sdf_st_error * pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetDefaultTransport");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetDefaultTransport(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultTransport(): "
			"pInitData param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pIp == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultTransport(): "
			"pIp param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif

	/*
	 * Free the existing IP if any, and set the new IP/port
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)\
		&(pInitData->pTransport->pIp),pErr);

	pInitData->pTransport->pIp =Sdf_mc_strdupCallHandler(pIp);

	pInitData->pTransport->dPort = dPort;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetDefaultTransport");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaAddDefaultMsgBody
**
** DESCRIPTION: Adds a Message body in the message body list of 
**				the call profile
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddDefaultMsgBody
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData,
	 SipMsgBody *pSipMsgBody,
	 Sdf_st_error *pErr)
#else
	( pInitData, pSipMsgBody, pErr )
	 Sdf_st_initData *pInitData;
	 SipMsgBody *pSipMsgBody;
	 Sdf_st_error *pErr;
#endif
{
	en_SipMsgBodyType dType;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaAddDefaultMsgBody");

	/* Error checks in the input parameters */
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaAddDefaultMsgBody(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultMsgBody(): "
			"pInitData param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pSipMsgBody == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultMsgBody(): "
			"pSipMsgBody param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif
	
	HSS_LOCKEDINCREF(pSipMsgBody->dRefCount);

/*w534*/	(void)sip_getTypeFromMsgBody(pSipMsgBody, &dType, (SipError*)&(pErr->stkErrCode));
	if (SipSdpBody == dType)
	{
		SdpMessage *pSdpMsg;
		if (sip_getSdpFromMsgBody(pSipMsgBody, &pSdpMsg, \
				(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultMsgBody(): "
				"Getting pSdpMessage Failed",pErr);
#endif
			return Sdf_co_fail;
		}
		if (sdf_ivk_uaFormSessionParam(pSdpMsg, \
			pInitData->pSessionParam,Sdf_en_convertLocal,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultMsgBody(): "
				"pSipMsgBody param passed is Invalid",pErr);
#endif
			return Sdf_co_fail;
		}
		sip_freeSdpMessage(pSdpMsg);
	}
	if(sdf_listAppend(&(pInitData->slMsgBodyList),(Sdf_ty_pvoid)pSipMsgBody,\
		pErr) ==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultMsgBody(): "
			"could not append the message body in the defualt message"
			"body list",pErr);
#endif
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaAddDefaultMsgBody");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaAddDefaultHeader
**
** DESCRIPTION: Adds Header information to the call profile.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddDefaultHeader
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData,
	Sdf_ty_s8bit *pName,
	Sdf_ty_s8bit *pValue,
	Sdf_ty_bool dAddAlways,
	 Sdf_st_error *pErr)
#else
	( pInitData, pName, pValue, dAddAlways, pErr )
	 Sdf_st_initData *pInitData;
	Sdf_ty_s8bit *pName;
	Sdf_ty_s8bit *pValue;
	Sdf_ty_bool dAddAlways;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_s8bit *current_name = Sdf_co_null;
	Sdf_ty_s8bit *pTmpValue = Sdf_co_null;
	Sdf_ty_u32bit valueCount,match_found=0;
	Sdf_st_configExtraHeaders *pConfHdr = Sdf_co_null;
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaAddDefaultHeader");

	/* Error checks in the input parameters */
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaAddDefaultHeader(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
			"pInitData param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pName== Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
			"pName param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	
	if(pValue== Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
			"pValue param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif

	(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		pConfHdr =(Sdf_st_configExtraHeaders *) \
					(dListIterator.pCurrentElement->pData);


		if(pConfHdr->pParam == Sdf_co_null)
		{	
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
				"pParam in the extra headers list is invalid",pErr);
#endif
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}

		if(sip_getNameFromSipParam(pConfHdr->pParam,&current_name,\
				(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
				"couldnot get the name from the pParam of extra"
				"header list",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if(Sdf_mc_strcasecmp(current_name,pName) ==0)
		{
			if( sip_getValueCountFromSipParam(pConfHdr->pParam,&valueCount,\
				(SipError*)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
					"couldnot get the value count from the pParam"
					"of the extra headers list",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			pTmpValue = Sdf_mc_strdupCallHandler(pValue);

			if(sip_insertValueAtIndexInSipParam(pConfHdr->pParam, pTmpValue, \
				valueCount, (SipError*)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
					"couldnot insert the value into the pParam"
					"of the extra headers list",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			match_found =1;
	    }
		(void)sdf_listNext(&dListIterator, pErr);
	}

	if(!match_found)
	{
		if(sdf_ivk_uaInitConfigExtraHeaders(&pConfHdr,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
				"couldnot init extra header",pErr);
#endif
			pErr->errCode=Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}

		pTmpValue = Sdf_co_null;

		pTmpValue = Sdf_mc_strdupCallHandler(pName);

		if (sip_setNameInSipParam(pConfHdr->pParam, pTmpValue, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
				"couldnot set the name in the pparam of the"
				"extra header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		pTmpValue = Sdf_co_null;

		pTmpValue = Sdf_mc_strdupCallHandler(pValue);


		if(sip_insertValueAtIndexInSipParam(pConfHdr->pParam, pTmpValue, 0, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
				"couldnot insert the value into the pparam"
				"of the extra header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if (dAddAlways == Sdf_co_true)
		{
			pConfHdr->pData = Sdf_mc_strdupConfig("yes");
		}

		if(sdf_listAppend(&(pInitData->slExtraHeadersList),\
			(Sdf_ty_pvoid)pConfHdr,pErr) ==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultHeader(): "
				"couldnot append the extra header into the call profile",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}	
	}
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaAddDefaultHeader");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetDefaultTag
**
** DESCRIPTION: Sets the tag value in the call profile
**		(Sdf_st_initData structure).
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetDefaultTag
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData,
	 Sdf_ty_s8bit *pTag,
	 Sdf_st_error *pErr)
#else
	( pInitData, pTag, pErr )
	 Sdf_st_initData *pInitData;
	 Sdf_ty_s8bit *pTag;
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetDefaultTag");

	/* Error checks in the input parameters */
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetDefaultTag(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultTag(): "
			"pInitData param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pTag == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultTag(): "
			"pTag param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif

	/*
	 * Free the existing Tag, if any, and set the new Tag
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)\
		&(pInitData->pTag),pErr);

	pInitData->pTag =Sdf_mc_strdupCallHandler(pTag);

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetDefaultTag");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaUpdateDefaults
**
** DESCRIPTION: Updates the call profile structure with the details of the
**	input structure. Note that the new profile structure completely replaces
**	all the fields of the old call profile structure.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaUpdateDefaults
#ifdef ANSI_PROTO 
	(Sdf_st_initData *pOrigInitData,
	Sdf_st_initData *pNewInitData,
	 Sdf_st_error *pErr)
#else
	( pOrigInitData, pNewInitData, pErr )
	 Sdf_st_initData *pOrigInitData;
	 Sdf_st_initData *pNewInitData;
	 Sdf_st_error * pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaUpdateDefaults");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaUpdateDefaults(): pErr passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pOrigInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults(): "
			"pOrigInitData passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pNewInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults(): "
			"pNewInitData passed is Invalid", pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;	
	}
#endif

	/*
	 * Copy the From information from source to destination
	 */
	if (sip_cloneSipHeader(pOrigInitData->pFrom, pNewInitData->pFrom, \
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults(): "
			"Error cloning SipHeader From",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, (Sdf_ty_s8bit *)"\"From\" details of UA initialized into user profile.\n", 0, pErr);
#endif	

	/*
	 * Copy the tag from source to destination
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
		(Sdf_ty_pvoid*)&(pOrigInitData->pTag),pErr);
	if(pNewInitData->pTag == Sdf_co_null)
		pOrigInitData->pTag = Sdf_co_null;
	else
	{
		pOrigInitData->pTag = Sdf_mc_strdupCallHandler(pNewInitData->pTag);
	}

	/*
	 * Copy the Contact list from source to destination
	 */
	(void)sdf_listDeleteAll(&(pOrigInitData->slContact),pErr);

	if (sdf_fn_uaCloneSipHeadersList(&(pOrigInitData->slContact), \
		&(pNewInitData->slContact), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults():  "
			"Failed to clone list of Contact headers from source to"
			"destination", pErr);	
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}


#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, (Sdf_ty_s8bit *)"\"Contact\" details of UA initialized into user profile.\n", 0, pErr);
#endif	

	/*
	 * Copy the Registrar information from source to destination
	 */
	 if(sdf_ivk_uaCloneTransportInfo(pOrigInitData->pRegistrar,\
		pNewInitData->pRegistrar,pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults(): "
			"Error cloning Registrar structure",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}

#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, \
		(Sdf_ty_s8bit *)"\"Registrar\" details of UA initialized into "
		"user profile.\n", 0, pErr);
#endif	

	/*
	 * Copy the Default Transport information from source to destination
	 */
	if(sdf_ivk_uaCloneTransportInfo(pOrigInitData->pTransport,\
		pNewInitData->pTransport,pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults(): "
			"Error cloning TransportInfo structure",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}

#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, \
		(Sdf_ty_s8bit *)"Transport details of UA initialized into user "
		"profile.\n", 0, pErr);
#endif	

	/*
	 * Copy the Message Body list from source to destination
	 */
	if (sdf_listDeleteAll(&(pOrigInitData->slMsgBodyList), \
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults(): "
			"Error deleting SipMsgBody structures from original user profile", \
			pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}

	if (sdf_fn_uaCloneSipMsgBodyList(&(pOrigInitData->slMsgBodyList),\
		&(pNewInitData->slMsgBodyList), pErr) == Sdf_co_fail)	
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults(): "
			"Error cloning list of SipMsgBody structures",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}
	
#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces,\
		(Sdf_ty_s8bit *)"Media details of UA initialized into user profile.\n",\
		0, pErr);
#endif	

	/*
	 * Copy the Extra Headers list from source to destination
	 */
	if (sdf_listDeleteAll(&(pOrigInitData->slExtraHeadersList), \
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults(): "
			"Error deleting Extraheaders from original user profile",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}

	if(sdf_fn_uaCloneSlExtraHeadersList(&(pOrigInitData-> \
		slExtraHeadersList),&(pNewInitData->slExtraHeadersList),pErr) \
	    == Sdf_co_fail)	
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults(): "
			"Error cloning list of Extraheaders",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}

#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, \
		(Sdf_ty_s8bit *)"Extra Headers (if any) initialized into user "
		"profile.\n", 0, pErr);
#endif	

	(void)sdf_listDeleteAll(&(pNewInitData->slPreLoadedRoute), pErr);

	if (sdf_fn_uaCloneSipHeadersList(&(pNewInitData->slPreLoadedRoute), \
		&(pOrigInitData->slPreLoadedRoute), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateDefaults():  "
			"Failed to clone list of Route  headers from source to"
			"destination", pErr);	
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
	}
	/*
	 * Copy the Call-ID generator function pointer from source to destination
	 */
	pOrigInitData->sdf_fp_uaCallIdGen = pNewInitData->sdf_fp_uaCallIdGen;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaUpdateDefaults");
	
	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetDefaultCallIdGenerator
**
** DESCRIPTION: Sets the Call-Id generator function for the user profile.
**	If no function pointer is passed, the default Call-Id generator provided
**	by the toolkit will be used.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetDefaultCallIdGenerator 
#ifdef ANSI_PROTO
(Sdf_st_initData *pInitData,
 sdf_fp_uaCallIdGen fptrCallIdGen,
 Sdf_st_error *pErr)
#else
 (pInitData, fptrCallIdGen,pErr)
 Sdf_st_initData *pInitData;
 sdf_fp_uaCallIdGen fptrCallIdGen;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_ivk_uaSetDefaultCallIdGenerator");

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaSetDefaultCallIdGenerator(): pErr passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if (pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultCallIdGenerator(): "
			"InitData param passed is Invalid", pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
	 * If a function pointer has been passed to this API, it is set as
	 * the Call-Id generator. Else, the default implementation provided
	 * by the toolkit is used.
	 */
	if (fptrCallIdGen == Sdf_co_null)
		pInitData->sdf_fp_uaCallIdGen = sdf_ivk_uaCallIdGenerator;
	else
		pInitData->sdf_fp_uaCallIdGen = fptrCallIdGen;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetDefaultCallIdGenerator");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/*******************************************************************
** FUNCTION: sdf_ivk_uaAddDefaultPreLoadedRoute
**
** DESCRIPTION: Adds a PreLoaded Route address to the call profile data 
**				structure (Sdf_st_initData structure)
**
********************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddDefaultPreLoadedRoute 
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData,
	 Sdf_ty_s8bit *pName,
	 Sdf_ty_s8bit *pAddr,
	 Sdf_ty_u16bit dPort,
	 Sdf_ty_s8bit *pParam,
	 Sdf_ty_s8bit *pScheme,
	 Sdf_st_error *pError)
#else
	( pInitData,pName, pAddr,dPort,pParam,pScheme,pError )
	 Sdf_st_initData *pInitData;
	 Sdf_ty_s8bit *pName;
	 Sdf_ty_s8bit *pAddr;
	 Sdf_ty_u16bit dPort;
	 Sdf_ty_s8bit *pParam;
	 Sdf_ty_s8bit *pScheme;
	 Sdf_st_error *pError;
#endif
{
	Sdf_ty_s8bit finalAddr[Sdf_mc_maxConfigFileLen]="";
	Sdf_ty_s8bit pTemp[Sdf_mc_maxConfigFileLen]="";
	Sdf_ty_s8bit dTempPort[Sdf_co_characterBufferSize];
	SipHeader	*pRouteHdr = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaAddDefaultPreLoadedRoute");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
		"sdf_ivk_uaAddDefaultPreLoadedRoute(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultPreLoadedRoute(): "
			"pInit param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pAddr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultPreLoadedRoute(): "
			"pAddr param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return  Sdf_co_fail;
	}
#endif

	if(dPort !=0)
		(void)sdf_fn_uaSprintf(dTempPort,"%d",dPort); 
	else
	{
#ifdef SDF_LOOKUP
		(void)sdf_fn_uaSprintf(dTempPort,"%d",0);
#else		
#ifdef SDF_TLS
		if (Sdf_mc_strcmp(pScheme, "sips") == 0)
		    (void)sdf_fn_uaSprintf(dTempPort,"%d",Sdf_mc_defaultSipSPort); 
		else
#endif
#endif
		    (void)sdf_fn_uaSprintf(dTempPort,"%d",Sdf_mc_defaultSipPort); 
	}


	if ((Sdf_mc_strcmp(pScheme, "sip") == 0) || \
		(Sdf_mc_strcmp(pScheme, "sips") == 0))
	{
		if (pName != Sdf_co_null)
			(void)sdf_fn_uaSprintf(finalAddr, "%s:%s@%s", pScheme, pName, pAddr);
		else 
			(void)sdf_fn_uaSprintf(finalAddr, "%s:%s", pScheme, pAddr);
	
#ifdef SDF_LOOKUP
		if (dPort != 0)
		{
#endif
			Sdf_mc_strcat(finalAddr,":");
			Sdf_mc_strcat(finalAddr,dTempPort);
#ifdef SDF_LOOKUP
		}
#endif
	}
	else
	{
		if( pScheme == Sdf_co_null )
			(void)sdf_fn_uaSprintf(finalAddr, "sip:%s", pAddr);
		else	
			(void)sdf_fn_uaSprintf(finalAddr, "%s:%s", pScheme, pAddr);
	}

	if((pParam !=Sdf_co_null) || (Sdf_mc_strcmp(pParam,"")==0))
	{
		Sdf_mc_strcat(finalAddr,";");
		Sdf_mc_strcat(finalAddr,pParam);
	}

	if (sip_initSipHeader(&pRouteHdr, SipHdrTypeAny, \
			(SipError *)&(pError->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultPreLoadedRoute(): "
			"Failed to initialize Route hdr",pError);
#endif
		pError->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}
	(void)sdf_fn_uaSprintf(pTemp, "Route: <%s>", finalAddr);
	if (sip_parseSingleHeader(pTemp, SipHdrTypeRoute, pRouteHdr, \
			(SipError *)&(pError->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultPreLoadedRoute(): "
			"Failed to parse Route string ",pError);
#endif
		sip_freeSipHeader(pRouteHdr);
		pError->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}

	/* 
	 * Append the new Route to the existing list of  Preloaded Route
	 */
	if (sdf_listAppend(&(pInitData->slPreLoadedRoute), pRouteHdr, \
		pError) == Sdf_co_fail)
	{
		sip_freeSipHeader(pRouteHdr);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddDefaultPreLoadedRoute(): "
			"Failed to add Route to list of Preloaded Route in InitData",pError);
#endif
		pError->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}


	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaAddDefaultPreLoadedRoute");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

#ifdef SDF_TLS

/*******************************************************************
** FUNCTION: sdf_ivk_uaSetDefaultTlsInfo
**
** DESCRIPTION: Form the SSL data out of the pSslParams passed and sets 
**              it into the InitData.
**
********************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetDefaultTlsInfo 
	(Sdf_st_initData  *pInitData,
	 Sdf_st_sslParams *pSslParams,
	 Sdf_st_error     *pError)
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaSetDefaultTlsInfo");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
		"sdf_ivk_uaSetDefaultTlsInfo(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultTlsInfo(): "
			"pInit param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if(pSslParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultTlsInfo(): "
			"SSL param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* SDF_PARAMVALIDATION */

	/* 
	 * Form the SSL data out of the SSL params and store it in the 
	 * InitData. 
	 */
	if (sdf_fn_uaFormSslData((Sdf_ty_pvoid*)pSslParams, \
						pInitData->pSslData, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_criticalErrors,Sdf_en_configDataError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetDefaultTlsInfo():  "
			"Forming the SslData failed.",pError);
#endif
		pError->errCode=Sdf_en_configDataError;
		return Sdf_co_fail;
	}

	/* Set the SSL Params in the InitData. */
	if (pInitData->pSslData->pSslParams != Sdf_co_null)
	{
		(void)sdf_ivk_uaFreeSslParams(pInitData->pSslData->pSslParams);
	}
	pInitData->pSslData->pSslParams = pSslParams;
	HSS_LOCKEDINCREF(pSslParams->dRefCount);

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetDefaultTlsInfo");

	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

#endif /* SDF_TLS */

#ifdef SDF_RPORT
/*******************************************************************
** FUNCTION: sdf_ivk_uaEnableRport
**
** DESCRIPTION: This will set the Rport boolean variable in Sdf_st_initData 
**              This will cause rport to be present in via header of each client request.
**
********************************************************************/

Sdf_ty_retVal sdf_ivk_uaEnableRport 
	(Sdf_st_initData  *pInitData,
	 Sdf_st_error     *pError)
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaEnableRport");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{

#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
		"sdf_ivk_uaEnableRport(): pError param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaEnableRport(): "
			"pInitData param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif /* SDF_PARAMVALIDATION */

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting sdf_ivk_uaEnableRport");
		

	
	pInitData->dEnableRport = Sdf_co_true;
	
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}




/*******************************************************************
** FUNCTION: sdf_ivk_uaDisableRport
**
** DESCRIPTION: This will reset the Rport boolean variable in Sdf_st_initData 
**              With this rport viaparam will not be present in via header of 
**		any client request.
**
********************************************************************/

Sdf_ty_retVal sdf_ivk_uaDisableRport 
	(Sdf_st_initData  *pInitData,
	 Sdf_st_error     *pError)
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaDisableRport");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{

#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
		"sdf_ivk_uaDisableRport(): pError param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDisableRport(): "
			"pInitData param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif /* SDF_PARAMVALIDATION */



	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaDisableRport");
		

	
	pInitData->dEnableRport = Sdf_co_false;
	
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


#endif
