/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************
 ** FUNCTION:
 **		This file is the header for Trace functions

 ******************************************************************************************
 **
 ** FILENAME:
 ** microsip_trace.h
 **
 ** DESCRIPTION:
 ** Trace/Error modules implemented here
 **
 ** DATE	 		NAME		      REFERENCE    REASON
 ** ----		 	----		      --------	   ------
 **23-03-2002		mahesh				Core Stack
 **
 ** COPYRIGHT , Aricent, 2006
 *******************************************************************************************/


#ifndef __SIP_TRACE_H_
#define __SIP_TRACE_H_
#include "microsip_struct.h"
#include "microsip_common.h"
#include "microsip_portlayer.h"

#if defined(PARSER_TEST) || defined(ACCTEST)
#define PrintFunction SocketWrite
#else
#ifndef PrintFunction
#define PrintFunction sip_printf
#endif
#endif


extern SipTraceLevel curSipLevel;
extern SipTraceType  curSipType;
extern SipErrorLevel	curSipErrLevel;

void  SocketWrite( SIP_S8bit *fmt, ...);
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
extern SipBool	sip_setTraceLevel _ARGS_((SipTraceLevel  lev, SipError *err));
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
extern SipBool	sip_getTraceLevel _ARGS_((SipTraceLevel  *lev, SipError *err));

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
extern SipBool	sip_setTraceType _ARGS_((SipTraceType  type, SipError *err));

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
extern SipBool	sip_getTraceType _ARGS_((SipTraceType  *type, SipError *err));
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
extern SipBool	sip_trace _ARGS_((SipTraceLevel lev, SipTraceType type, SIP_S8bit *txt));

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
extern SipBool	sip_setErrorLevel _ARGS_((SipErrorLevel  lev, SipError *err));
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
extern SipBool	sip_getErrorLevel _ARGS_((SipErrorLevel  *lev, SipError *err));

#define sip_trace_printf printf
#ifdef SIP_ERROR
#define sip_error(lev, str) \
do \
{ \
	if (curSipErrLevel & lev ) \
	{ \
		PrintFunction("SIP STACK ERROR:%s\n", str); \
	} \
} while (0)
#else
#define sip_error(lev, str)
#endif


#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif

