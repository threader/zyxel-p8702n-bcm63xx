/*****************************************************************************************
 ** FUNCTION:
 **		This file implements Trace/Error routines
 **
 ******************************************************************************************
 **
 ** FILENAME:
 ** microsip_trace.c
 **
 ** DESCRIPTION:
 ** Trace implementation is here.
 **
 ** DATE       NAME		    REFERENCE	 REASON
 ** ----       ----		    --------	 ------
 **23-03-2002	mahesh		Core Stack
 **
 **	COPYRIGHT , Aricent, 2006
 *******************************************************************************************/

#include <stdio.h>
#include "microsip_common.h"
#include "microsip_trace.h"

#if defined(PARSER_TEST) || defined(ACCTEST)
int newsockfd;
#endif

SipTraceLevel curSipLevel = SIP_UndefTrace;
SipTraceType  curSipType;
SipErrorLevel curSipErrLevel = SIP_UndefError;

#if defined(PARSER_TEST) || defined(ACCTEST)
/*****************************************************************
** FUNCTION: SocketWrite
** DESCRIPTION:
*****************************************************************/

void  SocketWrite( SIP_S8bit *first, ...)
{
	SIP_S8bit* str;
	va_list  ips;
	SipError err;
	
	str = (SIP_S8bit *)fast_memget(0, 5000, &err);
	if(str==SIP_NULL)
	{
		send(newsockfd, "Alloc for write buffer failed.",30,0);
		return;
	}
	va_start(ips,first);
	vsprintf(str,first,ips);
	va_end(ips);
	send(newsockfd, str,sip_strlen(str),0);
	fast_memfree(0, str, SIP_NULL);
	
}

#endif




/*******************************************************************************
** FUNCTION: sip_getTraceLevel
** DESCRIPTION: gets default Trace Level
** 
** PARAMETERS:
**      	      lev(OUT)				:The trace Level
**				  err(IN)				:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_getTraceLevel
#ifdef ANSI_PROTO
	(SipTraceLevel	*lev,
	 SipError	*err)
#else
	(lev, err)
	SipTraceLevel	*lev;
	SipError	*err;
#endif
{
#ifndef SIP_TRACE
    (void)lev;
	*err = E_TRACE_DISABLED;
	return SipFail;
#else
	*lev = curSipLevel;
	*err = E_NO_ERROR;
	return SipSuccess;
#endif
}


/*******************************************************************************
** FUNCTION: sip_getTraceType
** DESCRIPTION: gets default Trace Type
** 
** PARAMETERS:
**      	      dType(OUT)			:The trace type
**				  err(IN)				:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_getTraceType
#ifdef ANSI_PROTO
	(SipTraceType  *dType,
	 SipError	*err)
#else
	(dType, err)
	SipTraceType	*dType;
	SipError	*err;
#endif
{
#ifndef SIP_TRACE
	(void)dType;
    *err = E_TRACE_DISABLED;
	return SipFail;
#else

	*dType = curSipType;
	*err = E_NO_ERROR;
	return SipSuccess;
#endif
}

/*******************************************************************************
** FUNCTION: sip_setErrorLevel
** DESCRIPTION: Sets default Error Level
** 
** PARAMETERS:
**      	      lev(IN)			:The error Level
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_setErrorLevel
#ifdef ANSI_PROTO
	(SipErrorLevel	lev,
	 SipError	*err)
#else
	(lev, err)
	SipErrorLevel	lev;
	SipError	*err;
#endif
{
#ifndef SIP_ERROR
	*err = E_ERROR_DISABLED;
	lev=lev;		/* just to remove warning */
	return SipFail;
#else
	if (lev != SIP_None )
	{
			if ( (!(lev & SIP_Major )) &&
				 (!(lev & SIP_Minor )) &&
				 (!(lev & SIP_Critical)) )
			{
				*err = E_INV_ERRORLEVEL;
				return SipFail;
			}
	}
	
	curSipErrLevel = lev;
	*err = E_NO_ERROR;
	return SipSuccess;
#endif
}
/*******************************************************************************
** FUNCTION: sip_getErrorLevel
** DESCRIPTION: gets default Error Level
** 
** PARAMETERS:
**      	      lev(OUT)			:The error Level
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_getErrorLevel
#ifdef ANSI_PROTO
	(SipErrorLevel*  lev,
	 SipError	*err)
#else
	(lev, err)
	SipErrorLevel	*lev;
	SipError	*err;
#endif
{
#ifndef SIP_ERROR
	*err = E_ERROR_DISABLED;
	lev=lev;		/* just to remove warning */
	return SipFail;
#else
	*lev = curSipErrLevel;
	*err = E_NO_ERROR;
	return SipSuccess;
#endif
}



/*******************************************************************************
** FUNCTION: sip_setTraceLevel
** DESCRIPTION: Sets default Trace Level
** 
** PARAMETERS:
**      	      levr(IN)				:The trace Level
**				  err(IN)				:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_setTraceLevel
#ifdef ANSI_PROTO
	(SipTraceLevel	lev,
	 SipError	*err)
#else
	(lev, err)
	SipTraceLevel	lev;
	SipError	*err;
#endif
{
#ifndef SIP_TRACE
	(void)lev;
    *err = E_TRACE_DISABLED;
	return SipFail;
#else

	SIPDEBUGFN("Entering function: sip_setTraceLevel ");
	if (lev > SIP_TraceLevel) 
	{
		*err = E_INV_TRACELEVEL;
		SIPDEBUGFN("Leaving function: sip_setTraceLevel ");
		return SipFail;
	}
	curSipLevel = lev;
	SIPDEBUGFN("Leaving function: sip_setTraceLevel ");
	return SipSuccess;
#endif
}

/*******************************************************************************
** FUNCTION: sip_setTraceType
** DESCRIPTION: sets default Trace Type
** 
** PARAMETERS:
**      	      dType(IN)				:The trace type
**				  err(IN)				:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_setTraceType
#ifdef ANSI_PROTO
	(SipTraceType	dType,
	 SipError	*err)
#else
	(dType, err)
	SipTraceType	dType;
	SipError	*err;
#endif
{
#ifndef SIP_TRACE
	(void)dType;
    *err = E_TRACE_DISABLED;
	return SipFail;
#else

	SIPDEBUGFN("Entering function: sip_setTraceType ");
	if ( dType & ~(SIP_All))
	{
		*err = E_INV_TRACETYPE;
		SIPDEBUGFN("Leaving function: sip_setTraceType ");
		return SipFail;
	}
	curSipType = dType;
	SIPDEBUGFN("Leaving function: sip_setTraceType ");
	return SipSuccess;
#endif
}


/*******************************************************************************
** FUNCTION: sip_trace
** DESCRIPTION: Basically, flags are valid only if dLevel is SIP_Brief
** In case dLevel is SIP_None, then we dont log
** In case dLevel is SIP_All, we log all and dont check for flags
** 
** PARAMETERS:
**      	      lev(IN)			:The trace Level
**      	      dType(IN)			:The trace type
**      	      fmt(IN)			:The fromat string telling details 
**									regarding dispaly of trace string
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_trace
#ifdef ANSI_PROTO
	(SipTraceLevel		lev,
	 SipTraceType		dType,
	 SIP_S8bit		*fmt
	 )
#else
	(lev, dType, fmt)
	SipTraceLevel		lev;
	SipTraceType		dType;
	SIP_S8bit		*fmt;
#endif
{
	SIP_S8bit *pTimeString;
	SipError err;
	SIPDEBUGFN("Entering function: sip_trace ");
	/* Do not record this message if curr lev is less than pRequest lev */
	if (curSipLevel == SIP_UndefTrace)
		return SipFail;

	if (curSipLevel == SIP_None) 
	{
		SIPDEBUGFN("Leaving function: sip_trace ");
		return SipSuccess;   /* If None, Flags dont matter */
	}
	if (curSipLevel <  lev ) 
	{
		SIPDEBUGFN("Leaving function: sip_trace ");
		return SipSuccess;
	}

	/* For Detailed, all messages, irrespective of Flags are set */
   
	if (curSipLevel != SIP_Detailed )
		if ( !(curSipType & dType)) 
		{
			SIPDEBUGFN("Leaving function: sip_trace ");
			return SipSuccess;
		}

	if(dType & SIP_Init)
		sip_trace_printf("SIP STACK SIP_TRACE Type: INIT	   ");
	else if(dType & SIP_Incoming)
		sip_trace_printf("SIP STACK SIP_TRACE Type: INCOMING ");
	else if(dType & SIP_Outgoing)
		sip_trace_printf("SIP STACK SIP_TRACE Type: OUTGOING ");
	else if(dType & SIP_SysError)
		sip_trace_printf("SIP STACK SIP_TRACE Type: SIP_ERROR    ");
	pTimeString = (SIP_S8bit *)fast_memget(0,30,&err);
	if(pTimeString==SIP_NULL) 
	 {
		return SipFail;
	}

	sip_getTimeString((SIP_S8bit **)&pTimeString);

	sip_trace_printf("Generated at: %s",pTimeString);
	sip_trace_printf("%s\n",fmt);
	sip_trace_printf("-------------------------------------------------------\n");
	(void)fast_memfree (0, pTimeString, NULL);
	return SipSuccess;
}


