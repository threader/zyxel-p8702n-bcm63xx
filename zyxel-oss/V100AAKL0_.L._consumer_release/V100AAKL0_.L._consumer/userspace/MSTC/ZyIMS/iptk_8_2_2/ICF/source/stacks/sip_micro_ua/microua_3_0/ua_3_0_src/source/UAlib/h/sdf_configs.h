
/***********************************************************************
 ** FUNCTION:
 **			This file contains all API's related to creating a user
 **			profile
 **
 *********************************************************************
 **
 ** FILENAME		: sdf_configs.h
 **
 ** DESCRIPTION		: This file contains all API's related to creating a 
 **						user profile
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 16/11/00	Ashok						Creation
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/
#ifndef __SDF_CONFIG_H_
#define __SDF_CONFIG_H_

/* Maximum length of string allowed in the XML file*/
#define Sdf_mc_maxConfigFileLen		1000

/*#define MAX_CONFIG_SIZE					10000*/

/*Structure local to sdf_configs.c*/
typedef struct {
	Sdf_ty_s8bit* pMediaType;
	Sdf_ty_u16bit* pPort;
	Sdf_ty_u32bit* pPortNum;
	Sdf_ty_s8bit* pTransport;
	Sdf_ty_s8bit* pCodecs;
	Sdf_ty_s8bit* pTransType;
	Sdf_ty_slist slAttribute;
	Sdf_ty_slist slConnection;
	Sdf_ty_s8bit* pBandwidth;
	Sdf_ty_s8bit* pKey;
	Sdf_ty_s8bit* pInfo;
}Sdf_st_localMedia;

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/***********************************************************************
** FUNCTION:  sdf_fn_skipCharsForward
**
** DESCRIPTION: Skips caharcters till a matching pattern
**
** PARAMETERS:
**		pInput(IN/OUT): The input buffer which has to be parsed
**		pPosition(IN) : The position from which the chars have to be skipped
**		pPattern(IN)  : The pattern to be matched
**
************************************************************************/
 Sdf_ty_bool sdf_fn_skipCharsForward _ARGS_((Sdf_ty_s8bit *pInput, \
		Sdf_ty_s32bit *pPosition, Sdf_ty_s8bit *pPattern)); 


/***********************************************************************
** FUNCTION:  sdf_fn_skipSpaces
**
** DESCRIPTION: This function skips spaces in given buffer and at a 
**				given position. It advances the pointer to the next 
**				non-space character.
**
** PARAMETERS:
**		pInput(IN/OUT): The input buffer which has to be parsed
**		pPosition(IN) : The position from which the chars have to be skipped
**
************************************************************************/
 void sdf_fn_skipSpaces _ARGS_((Sdf_ty_s8bit *pInput, \
			 Sdf_ty_s32bit *pPosition));

/***********************************************************************
** FUNCTION:  sdf_fn_xmlParseTag
**
** DESCRIPTION: This function parses the tag at the point pointed 
**				by position. It will parse recursively till end of 
**				the same tag if found.
**
** PARAMETERS:
**		pInput(IN/OUT)		: The input buffer which has to be parsed
**		pPosition(IN) 		: The position from which parsing should begin
**		pInitData(IN/OUT)	: The user profile to be created using the input
**		pErr(OUT)	  		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_fn_xmlParseTag _ARGS_((Sdf_ty_s8bit *pInput, \
	 Sdf_ty_s32bit *pPosition, Sdf_st_initData *pInitData,  \
	 Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaParseConfigData
**
** DESCRIPTION: This function parses the input using a XML
**				parser, and sets it into the passed user profile structure
**
** PARAMETERS: 
**		pInput(IN)			: The input buffer to be parsed
**		count(IN)			: The no of bytes passed (size of pInput)
**		pInitData(IN/OUT) 	: The user profile to be created using the input
**  	pErr(OUT)     		: Error Structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal  sdf_ivk_uaParseConfigData _ARGS_((Sdf_ty_s8bit *pInput, \
	Sdf_ty_s32bit count,Sdf_st_initData *pInitData, Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_fn_xmlParseAttlist
**
** DESCRIPTION: Parses the attributes inside a tag
**
** PARAMETERS:
**		pInput (IN/OUT)	: The buffer that has to be parsed.
**		pPosition(IN)  	: The index from which parsing has to begin
**		ppAttrib (OUT) 	: The attribute value
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_fn_xmlParseAttlist _ARGS_((Sdf_ty_s8bit* pInput, \
	    Sdf_ty_s32bit* pPosition,Sdf_ty_s8bit  **ppAttrib, Sdf_st_error* pErr));

/***********************************************************************
** FUNCTION: sdf_fn_uaInitLocalMedia
**
** DESCRIPTION: Initializes the LocalMedia structure. The parser uses 
**				this function to fill in the media params
**
** PARAMETERS:
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_fn_uaInitLocalMedia _ARGS_ ((Sdf_st_error* pErr));

#ifdef SDF_TRACE
/***********************************************************************
** FUNCTION: sdf_fn_uaFormMediaBuffer
**
** DESCRIPTION: Forms the Media header buffer from the SdpMedia structure
**
** PARAMETERS:
**		pData(IN)	: The SdpMedia structure
**		ppBuf(OUT)	: The buffer formed from pData
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 void sdf_fn_uaFormMediaBuffer _ARGS_ ((SdpMedia *pData, \
	Sdf_ty_s8bit **ppBuf,Sdf_st_error *pError));

/***********************************************************************
** FUNCTION: sdf_fn_uaFormExtraHeaderBuffer
**
** DESCRIPTION: Forms the Extra header buffer from the pData
**
** PARAMETERS:
**		pData(IN)	: The Sdf_st_configExtraHeaders structure
**		ppBuf(OUT)	: The buffer formed from pData
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 void sdf_fn_uaFormExtraHeaderBuffer _ARGS_ ((Sdf_st_configExtraHeaders \
	*pData, Sdf_ty_s8bit **ppBuf, Sdf_st_error *pError));

/***********************************************************************
** FUNCTION: sdf_fn_uaFormRegistrarBuffer
**
** DESCRIPTION: Forms the Extra header buffer from the pData
**
** PARAMETERS:
**		pData(IN)	: The Sdf_st_initData structure
**		ppBuf(OUT)	: The registrar buffer formed from pData
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 void sdf_fn_uaFormRegistrarBuffer _ARGS_ ((Sdf_st_initData *pData, \
	Sdf_ty_s8bit **ppBuf, Sdf_st_error *pError));

#endif


/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetDefaultFrom
**
** DESCRIPTION: Sets the "From" parameters into the profile data structure
**				(Sdf_st_initData structure)
**
** PARAMETERS:
**		pInitData(IN/OUT)	: The call profile structure
**		pName(IN)			: Display Name 
**		pAddr(IN)			: Sip Address
**		dPort(IN)			: Receiving Port
**		pScheme(IN)			: Scheme used for the address (Eg: sip, tel)
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetDefaultFrom _ARGS_((Sdf_st_initData \
		*pInitData, Sdf_ty_s8bit *pName, Sdf_ty_s8bit *pAddr,\
		Sdf_ty_u16bit dPort, Sdf_ty_s8bit *pScheme, Sdf_st_error *pErr)); 


/***********************************************************************
** FUNCTION:  sdf_ivk_uaAddDefaultContact
**
** DESCRIPTION: Adds a Contact address to the call profile data structure
**				(Sdf_st_initData structure)
**
** PARAMETERS:
**		pInitData(IN/OUT) 	: The call profile structure
**		pName(IN)			: Display Name 
**		pAddr(IN)			: Address
**		dPort(IN)			: Receiving Port
**		pScheme(IN)			: Scheme used for the address (Eg: sip, tel)
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaAddDefaultContact _ARGS_((Sdf_st_initData \
		*pInitData, Sdf_ty_s8bit *pName, Sdf_ty_s8bit *pAddr,\
		Sdf_ty_u16bit dPort, Sdf_ty_s8bit *pScheme, Sdf_st_error *pErr)); 

/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetDefaultRegistrar
**
** DESCRIPTION: Sets the Registrar information in the call profile
**				(Sdf_st_initData structure)
**
** PARAMETERS:
**		pInitData(IN/OUT)	: The call profile structure
**		pRegistrar(IN)		: Registrar host name/IP
**		dPort(IN)			: Registrar Port
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetDefaultRegistrar _ARGS_ ((Sdf_st_initData \
		*pInitData, Sdf_ty_s8bit *pRegistrar, Sdf_ty_u32bit dPort,\
		Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetDefaultTransport
**
** DESCRIPTION: Sets the IP and Port on which the user agent will bind
**	 			and listen for incoming messages. This will also be used 
**				to insert the Via header for outgoing requests from the 
**				user agent.
**
** PARAMETERS:
**		pInitData(IN/OUT)	: The call profile structure
**		pIp(IN)				: IP address the user agent will bind to
**		dPort(IN)			: Port at which the UA will bind
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetDefaultTransport _ARGS_((Sdf_st_initData \
		*pInitData, Sdf_ty_s8bit *pIp, Sdf_ty_u32bit dPort,\
		Sdf_st_error *pErr)); 


/***********************************************************************
** FUNCTION:  sdf_ivk_uaAddDefaultMsgBody
**
** DESCRIPTION: Adds a Message body in the message body list of 
**				the call profile
**
** PARAMETERS:
**		pInitData(IN/OUT)	: The call profile structure
**		pMsgBody(IN)		: Message body to be added
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaAddDefaultMsgBody _ARGS_ (( \
	Sdf_st_initData  *pInitData,SipMsgBody *pMsgBody, Sdf_st_error *pErr)); 


/***********************************************************************
** FUNCTION:  sdf_ivk_uaAddDefaultHeader
**
** DESCRIPTION: Adds Header information to the call profile
**
** PARAMETERS:
**		pInitData(IN/OUT)	: The call profile structure
**		pName(IN)			: The name of the Header
**		pValue(IN)			: The value of the header
**		dAddAlways(IN)		: Boolean indicating whether the header
**								should be added in all requests/responses.
**								If set to true, the UAT will add it to the
**								ProcessedHeadersList and to the SipMessage. 
**								Else it adds it to the UnProcessedHeadersList.
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaAddDefaultHeader _ARGS_ ((\
	Sdf_st_initData  *pInitData,Sdf_ty_s8bit *pName,Sdf_ty_s8bit *pValue,\
	Sdf_ty_bool dAddAlways, Sdf_st_error *pErr)); 


/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetDefaultTag
**
** DESCRIPTION: Sets the tag value in the user profile
**
** PARAMETERS:
**		pInitData(IN/OUT)	: The call profile structure
**		pTag(IN)			: Tag value to be set
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetDefaultTag _ARGS_ (( \
	Sdf_st_initData  *pInitData, Sdf_ty_s8bit *pTag,Sdf_st_error *pErr)); 


/***********************************************************************
** FUNCTION:  sdf_ivk_uaUpdateDefaults
**
** DESCRIPTION: Updates the call profile structure with the details of the
**				input structure. Note that the new profile structure 
**				completely replaces all the fields of the old call 
**				profile structure.
**
** PARAMETERS:
**		pOrigInitData(IN/OUT)	: The call profile structure to be updated
**		pNewInitData(IN)		: The input profile structure whose contents
**									will be updated into pOrigInitData
**		pErr(OUT)				: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaUpdateDefaults _ARGS_((Sdf_st_initData\
		*pOrigInitData, Sdf_st_initData *pNewInitData, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION: sdf_ivk_uaSetDefaultCallIdGenerator 
**
** DESCRIPTION: Sets the Call-Id generator function for the user profile.
**				If no function pointer is passed, the default Call-Id 
**				generator provided by the toolkit will be used.
**
** PARAMETERS:
**	pInitData(IN) 			: The user profile for which the call-id generator
**								is to be set.
**	sdf_fp_uaCallIdGen(IN)	: Function pointer to the function generating
**								the callId 
**	pErr(OUT)				: Error structure in case the function fails
**
************************************************************************/
typedef Sdf_ty_s8bit* (*sdf_fp_uaCallIdGen)(Sdf_st_appData *pAppData);
 Sdf_ty_retVal sdf_ivk_uaSetDefaultCallIdGenerator _ARGS_\
	((Sdf_st_initData *pInitData, sdf_fp_uaCallIdGen fptrCallIdGen, \
	 Sdf_st_error *pErr ));



/***********************************************************************
** FUNCTION:  sdf_ivk_uaAddDefaultPreLoadedRoute 
**
** DESCRIPTION: Adds a PreLoaded Route address to the call profile data structure
**				(Sdf_st_initData structure)
**
** PARAMETERS:
**		pInitData(IN/OUT) 	: The call profile structure
**		pName(IN)			: Display Name 
**		pAddr(IN)			: Address
**		dPort(IN)			: Receiving Port
**		pParam(IN)			: Uri Parameter
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaAddDefaultPreLoadedRoute _ARGS_((Sdf_st_initData \
		*pInitData, Sdf_ty_s8bit *pName, Sdf_ty_s8bit *pAddr,\
		Sdf_ty_u16bit dPort,Sdf_ty_s8bit *pParam, Sdf_ty_s8bit *pScheme,
		Sdf_st_error *pErr)); 

#ifdef SDF_TLS

/*******************************************************************
** FUNCTION: sdf_ivk_uaSetDefaultTlsInfo
**
** DESCRIPTION: Form the SSL data out of the pSslParams passed and sets 
**              it into the InitData.
**
********************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetDefaultTlsInfo _ARGS_((Sdf_st_initData  
		*pInitData, Sdf_st_sslParams *pSslParams,
		Sdf_st_error *pError));

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
	 Sdf_st_error     *pError);



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
	 Sdf_st_error     *pError);


#endif


/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
#endif
