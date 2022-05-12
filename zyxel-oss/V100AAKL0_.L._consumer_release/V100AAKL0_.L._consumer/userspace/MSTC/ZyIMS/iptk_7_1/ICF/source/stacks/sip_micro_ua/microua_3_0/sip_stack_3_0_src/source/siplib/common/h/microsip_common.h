
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 ** FUNCTION:
 **		This file has common definitions used by all modules
 **
 *******************************************************************************
 **
 ** FILENAME:
 ** microsip_common.h
 **
 ** DESCRIPTION: All common wrappers are defined here.
 **	It is expected that all programs use datatypes defined here.
 **
 ** DATE			NAME			REFERENCE		REASON
 ** ----			----			--------		------
 ** 15-02-2002		Aparna Kuppachi		-			Initial
 **													Creation
 **
 ** COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#ifndef __MICROSIPCOMMON_H__
#define __MICROSIPCOMMON_H__

#include <stdio.h>
#include <ctype.h>
#include <string.h>


/* This part ID is automatically inserted by the packaging script */
#define SIP_PART_ID "1-000-5-0219-0111-14-0400-000"
#define SIP_USE_CPP ANSI_PROTO

#ifdef ANSI_PROTO
#define _ARGS_(x)	x
#define _CONST_	const
#else
#define _ARGS_(x)	()
#define _CONST_
#endif

/* 
 * This is the size of the buffer allocated by the stack when converting
 * a SIP message structure to text. This will be the limit on the size
 * of the SIP messages that can be sent using the stack. 
 * TODO
 */
#ifndef SIP_IPADDR_SIZE
#define  SIP_IPADDR_SIZE  (16)
#endif

#ifndef SIP_FIRST_20_BYTES
#define SIP_FIRST_20_BYTES 21
#endif

#ifndef SIP_TRACE_BUF_SIZE 
#define SIP_TRACE_BUF_SIZE 200
#endif

#ifndef SIP_MAX_MSG_SIZE
#define  SIP_MAX_MSG_SIZE (3000)
#endif

/* This is the size of the buffer allocated by the stack when converting
 * a SIP message structure to text. This will be the limit on the size
 * of the SIP messages that can be sent using the stack.
 */
#ifndef SIP_MAXU16BIT
#define  SIP_MAXU16BIT (65535)
#endif

/* MICROUA begin */
/* This is the limit on the size of the buffer allocated by functions that
   return SIP headers or SDP lines as strings. This is not a limit on the
   size of the headers that can be formed by the stack. */
#ifndef SIP_MAX_HDR_SIZE
#define SIP_MAX_HDR_SIZE (1000)
#endif
/* MICROUA end */


#ifdef STATIC_SYMS
#define _STATIC_	static
#else
#define _STATIC_
#endif

#define SIP_NULL NULL

#define TIMER_CALLLEG		0
#define TIMER_TRANSACTION	1
#define TIMER_CALLLEG_GE300	2 /* to stop ret for resps with code >=300 */
#define TIMER_RPR			3
#define TIMER_NONE			4 /* Initial value */

/*
 ** Basic Data Types 
 */

typedef 	unsigned char		SIP_U8bit;
typedef 	unsigned short		SIP_U16bit;
typedef 	unsigned int		SIP_U32bit;

typedef 	char				SIP_S8bit;
typedef 	short				SIP_S16bit;
typedef 	int					SIP_S32bit;

typedef 	void *				SIP_Pvoid;


#define SipErrorLevel	SIP_S32bit
#define SIP_UndefError	(-1)

/*
 ** Enumerations
 */

typedef enum
{
	SipSuccess		=  1,
	SipFail 		=  0
} SipBool;

typedef enum
{
	SipTrue		=  1,
	SipFalse 	=  0
} en_SipBoolean;


typedef enum 
{
#define 	SipErrorBase		2000
	E_INV_HEADER			=  (SipErrorBase +0),
	E_INV_INDEX				=  (SipErrorBase +1),	       
	E_NO_MEM				=  (SipErrorBase +2),  
	E_INV_TYPE				=  (SipErrorBase +3),	   
	E_INV_TRACELEVEL		=  (SipErrorBase +4),
	E_INV_TRACETYPE 		=  (SipErrorBase +5),
	E_INV_ERRORLEVEL		=  (SipErrorBase +6),
	E_INV_STATSTYPE 		=  (SipErrorBase +7),
	E_TRACE_DISABLED		=  (SipErrorBase +8), 
	E_ERROR_DISABLED		=  (SipErrorBase +9),
	E_STATS_DISABLED		=  (SipErrorBase +10),
	E_DUP_ENTRY				=  (SipErrorBase +11),
	E_NO_EXIST				=  (SipErrorBase +12),
	E_SYSTEM_ERROR			=  (SipErrorBase +13),
	E_MEM_ERROR				=  (SipErrorBase +14),
	E_INV_PARAM				=  (SipErrorBase +15),
	E_NO_ERROR				=  (SipErrorBase +16),
	E_PARSER_ERROR			=  (SipErrorBase +17),
	/* Timer Errors */
	E_TIMER_DUPLICATE		=  (SipErrorBase+18),
	E_TIMER_FULL			=  (SipErrorBase+19),
	E_TIMER_NO_EXIST		=  (SipErrorBase+20),

	/* Added errors for possible incomplete messages*/
	E_MAYBE_INCOMPLETE		=  (SipErrorBase+21),
	E_INCOMPLETE			=  (SipErrorBase+22),

        /* Enhancement Change Start */
        /* Added Error Code for messages with possibly incomplete SIP header */
        E_SIPINCOMPLETE = (SipErrorBase+23),
        /* Enhancement Change End */

        /* Errors applicable for Txn layer */
        E_TXN_INV_MSG           = (SipErrorBase+24),
        E_TXN_NO_EXIST          = (SipErrorBase+25),
        E_TXN_INV_STATE         = (SipErrorBase+26),
        E_TXN_EXISTS            = (SipErrorBase+27),

        /* Error defined for data overflow */
        E_INV_RANGE             = (SipErrorBase+28),
        E_TXN_NETWORK_SEND_ERR          = (SipErrorBase+29),

        /*
         * This last error code only serves as an indicator for the last
         * error code. No other purpose is served.
        */
        E_LAST_ERROR                    = (SipErrorBase+30)
 
} SipError;

typedef enum 
{
	SIP_UndefTrace  = -1, 
	SIP_None		= 0,
	SIP_Brief		= 1,
	SIP_Detailed	= 2,
	SIP_TraceLevel	= 2
} SipTraceLevel;

typedef enum 
{
	SIP_Init	      = 0x01,
	SIP_Incoming      = 0x02,
	SIP_Outgoing      = 0x04,
	SIP_SysError      = 0x08,
	SIP_All 	      = 0x01 | 0x02 | 0x04 | 0x08			
} SipTraceType;

#define SIP_Major		(0x10)
#define SIP_Minor		(0x20)
#define SIP_Critical	(0x40)

#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

