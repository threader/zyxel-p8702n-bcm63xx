/****************************************************************************
 ** FUNCTION: 		Internal APIs used for message encoding
 **
 ****************************************************************************
 **
 ** FILENAME: 		microsip_encodeinternal.c
 **
 ** DESCRIPTION:	This file contains APIs used internally to encode a 
 **					SIP message.
 **
 **
 ** 	DATE	 	NAME			REFERENCE	 		REASON
 ** 	----	 	----			---------	 		--------
 ** 27-Feb-2002		Siddharth		sipformmesage.c		Initial
 **					Toshniwal		of SIP Stack
 **
 ***************************************************************************
 **				COPYRIGHT , Aricent, 2006
 ***************************************************************************/

#include "microsip_struct.h"
#include "microsip_encodeinternal.h"


/****************************************************************************
** FUNCTION: STRCAT_AND_SPACE
** DESCRIPTION: This function is used to append a string to another string, 
**				which will then be followed by a space.
** 
** PARAMETERS:
**				a(IN/OUT)	- The string to which we will append 
**				b(IN)		- The string to be appended 
**
** NOTE: 
****************************************************************************/

void STRCAT_AND_SPACE(SIP_S8bit *a, SIP_S8bit *b)
{
	if (b != SIP_NULL)
	{
			SIP_U32bit      srcLength;
			SIP_S8bit       *pTemp;

			if (a) 
				a+=sip_strlen(a);

			srcLength = sip_strlen(b);
			pTemp = a + srcLength;
#ifdef SIP_LINT
            if( a != SIP_NULL)
#endif
            {
                pTemp = a + srcLength;
                memcpy(a, b, srcLength);
                a = pTemp;
                memcpy(a, (SIP_S8bit *)" ", 1);
                a = a+1;
                *a = 0;
            }
	}
}

/****************************************************************************
** FUNCTION: STRCAT
** DESCRIPTION: This function is used to append a non-null string to another 
**				string.
** 
** PARAMETERS:
**				a(IN/OUT)	- The string to which we will append 
**				b(IN)		- The string to be appended 
**
** NOTE: 
****************************************************************************/

void STR_CAT (SIP_S8bit *a, SIP_S8bit *b)
{ 

        if (b != SIP_NULL)
        {
                SIP_U32bit      srcLength;
                SIP_S8bit       *pTemp;

                if (a) 
                        a+=sip_strlen(a);

                srcLength = sip_strlen(b);
                pTemp = a + srcLength;
#ifdef SIP_LINT
                if( a != SIP_NULL)
#endif
				{
						pTemp = a + srcLength;
						memcpy(a, b, srcLength);
						a = pTemp;
						*a = 0;
				}
        }

}


/****************************************************************************
** FUNCTION: STRCPY
** DESCRIPTION: This function is used to copy a non-null string to another 
**				string.
** 
** PARAMETERS:
**				a(OUT)	- The destination string 
**				b(IN)	- The source string 
**
** NOTE: 
****************************************************************************/

void STRCPY (SIP_S8bit *a, SIP_S8bit *b) 
{
	if (b != SIP_NULL) 
		(void)sip_strcpy ((SIP_S8bit *)a, (SIP_S8bit *)b);
}

/****************************************************************************
** FUNCTION: VALIDATE_HEADER
** DESCRIPTION: This function is used during header encoding to perform some 
**				validations for the header.
** 
** PARAMETERS:
**				pHeader(IN)		- The header to be validated 
**				dIndex(IN)		- The index of the header
**				pErr(OUT)		- Error value if any 
**
** NOTE: This function is currently used to validate headers of which 
**		 there is supposed to be only 1 per SIP Message.
**
****************************************************************************/

SipBool VALIDATE_HEADER (SIP_Pvoid pHeader, SIP_U32bit dIndex, SipError *pErr)
{
	if (pHeader == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
		
	if (dIndex != 0)
	{
		*pErr = E_INV_INDEX;
		return SipFail;
	}
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_formHeaderName
** DESCRIPTION: This is an internal function to set the Header name in long or short
**				form as desired by the top-level encoder. It also handles
**				putting a comma instead of the header name if mode is not
**				'SipModeNew'.
** 
** PARAMETERS:
**				dMode(IN)			- The mode in which the header is to be encoded 
**				dFullOrShortForm(IN)- The desired header form 
**				pOut(OUT)			- The output text string
**				pFullName(IN)		- The string to be used for hdr name when
**									  encoding in full form
**				pShortname(IN)		- The string to be used for hdr name when
**									  encoding in short form
**
** NOTE: 
****************************************************************************/

void sip_formHeaderName
#ifdef ANSI_PROTO
	(en_AdditionMode dMode, en_HeaderForm dFullOrShortForm, \
	 SIP_S8bit *pOut, SIP_S8bit *pFullName, SIP_S8bit *pShortname)
#else
	(dMode, dFullOrShortForm, pOut, pFullName, pShortname)
	en_AdditionMode dMode;
	en_HeaderForm dFullOrShortForm;
	SIP_S8bit *pOut;
	SIP_S8bit *pFullName;
	SIP_S8bit *pShortname;
#endif
{
	if (dMode == SipModeNew)
	{
		if (dFullOrShortForm == SipFormShort)
		{
			(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit *)pShortname);
		}
		else
			(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit *)pFullName);
	}
	else
		(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit *)",");

	return;
}


/****************************************************************************
** FUNCTION: __sip_setHeaderCountInHeaderLine
** DESCRIPTION: This is an internal function to set the header count in a 
**				  particular Header line of a SIP message.
** 
** PARAMETERS:
**				msg(IN)			- The SipMessage containing the header line
**				line(IN/OUT)	- The hdr line for which the count is to be set
**				count(IN)		- 
**				err(OUT)		- Error if any
**
** NOTE: 
****************************************************************************/

SipBool __sip_setHeaderCountInHeaderLine
#ifdef ANSI_PROTO	
	( SipMessage		*msg,	
	  SIP_U32bit		line,	
	  SIP_U32bit		count,
	  SipError			*err )	
#else	
	( msg, line, count, err )
	  SipMessage		*msg;	
	  SIP_U32bit		line;	
	  SIP_U32bit		count;
	  SipError		*err;
#endif	
{	
	SIP_Pvoid	temp;

	SIPDEBUGFN("Entering function sip_setHeaderCountAtHeaderLine");
	if (count > 0)
	{
		if (sip_listGetAt (&(msg->slOrderInfo), line, &temp, err) == SipFail)
			 return SipFail;
		((SipHeaderOrderInfo *)temp)->dNum = count;
		*err = E_NO_ERROR;
		SIPDEBUGFN("Exitting function sip_setHeaderCountAtHeaderLine");
		return SipSuccess;
	}
	else
	{
		*err = E_INV_PARAM; 
		return SipFail;
	}
}


/*****************************************************************************
 ** FUNCTION	: __checkHeaderTypeHop
 **
 ** DESCRIPTION	: Internal function to check if Header is of 
 **				  Type Hop-by-Hop
 **
 *****************************************************************************/
/****************************************************************************
** FUNCTION: __checkHeaderTypeHop
** DESCRIPTION: This is an internal function to check if a Header is of 
**				  type Hop-by-Hop
** 
** PARAMETERS:
**				dType(IN)		- The header type being checked
**
** RETURN VALUE:
**				SipBool (indicating whether header is Hop-by-Hop or not)
** NOTE: 
****************************************************************************/

SipBool __checkHeaderTypeHop
#ifdef ANSI_PROTO
	(en_HeaderType dType)
#else
	(dType)
	en_HeaderType dType;
#endif
{
	/*
	 * Hop-by-hop headers are Route, Record-Route, Proxy-Authenticate
	 * and Proxy-Authorization.
	 */

	switch (dType)
	{
		case	SipHdrTypeRecordRoute			:
		case	SipHdrTypeRoute					: 
#ifdef SIP_AUTHENTICATE		
		case	SipHdrTypeProxyAuthenticate 	:
		case	SipHdrTypeProxyauthorization	:
#endif		
												return SipSuccess;

		default									: 
												return SipFail;
	}
}


/****************************************************************************
** FUNCTION: glbSipParserMemStrStr
** DESCRIPTION: This is an internal function for comparing a memory buffer with a string
** The buffer may contain '\0'. Required to locate content boundaries in
** multipart messages which may contain binary encoded parts that contain
** '\0' which prevents usage of strstr.  
** 
** PARAMETERS:
**
** NOTE: 
****************************************************************************/

SipBool glbSipParserMemStrStr
#ifdef ANSI_PROTO
(SIP_S8bit *pBuffer,SIP_U32bit buflen, SIP_S8bit *separator, SIP_U32bit *offset)
#else
(pBuffer, buflen, separator, offset)
        SIP_S8bit *pBuffer;
        SIP_U32bit buflen;
        SIP_S8bit *separator;
        SIP_U32bit *offset;
#endif
{
	SIP_U32bit srcindex=0, cmpindex=0, cmplen;

	cmplen = sip_strlen(separator);
	while (srcindex < buflen)
	{
		if (pBuffer[srcindex] == separator[cmpindex])
		{
			srcindex++;
			cmpindex++;
		}
		else
		{
			srcindex += 1-cmpindex;
			cmpindex = 0;
		}
		if (cmpindex==cmplen) 
			break;
	}
	if (cmpindex != cmplen)
		return SipFail;

	*offset = srcindex - cmpindex;

	return SipSuccess;
}


