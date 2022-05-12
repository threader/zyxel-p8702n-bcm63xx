/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
** FUNCTION:
**		Port Layer code - has all memory related and other
**		common utils
************************************************************************
**
** FILENAME:
** microsip_portlayer.h
**
** DESCRIPTION:
** 	This file contains macros, constants, func decls, etc. for
**		porting layer. 
**
** DATE			NAME			REFERENCE		REASON
** ----			----			---------	    --------
** 15/2/2002   Mahesh Govind		-		   Initial Creation
**
** COPYRIGHT , Aricent, 2006
***********************************************************************/


#ifndef __PORTLAYER_H_
#define __PORTLAYER_H_

#include "microsip_common.h"
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#ifdef WIN32_LEAN_AND_MEAN
#include <Winsock2.h>
#include <ws2tcpip.h>
#endif
#endif
/*#include "Windows.h"*/

#ifdef SIP_LINUX
#ifdef SIP_THREAD_SAFE
#include "pthread.h"
#endif
#endif

#ifdef SIP_WINDOWS
#include <windows.h>
#endif


#include <string.h>
#define sip_printf printf
/* These are memory IDS that will be used for fast_memget */
#define DECODE_MEM_ID		0
#define ENCODE_MEM_ID		1
#define TIMER_MEM_ID		2
#define	NETWORK_MEM_ID		3
#define ACCESSOR_MEM_ID		4
#define SIP_RETURN_IMMEDIATELY	0
#define SIP_MAX_CTIME_R_BUF_SIZE 26
/* INIT and FREE IDs are used by sipinit.c & sipfree.c
  since there is no way of guessing in which context
  these free/inits were called, they have a different
  memory id */

#define	INIT_MEM_ID			5
#define FREE_MEM_ID			6
/* user apps may want to use this or any other value > 7 */
#define APP_MEM_ID			7 		

#define NON_SPECIFIC_MEM_ID		8 		

#ifdef	SIP_DEBUG
#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#define SIPDEBUG(a)  printf ("SIP_DEBUG:%s\n", a)
#endif



#else
#define SIPDEBUG(a)
#endif /* SIP_DEBUG */



#ifdef	SIP_FNDEBUG
#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#define SIPDEBUGFN(a) printf ("FUNCTION SIP_DEBUG:%s\n", a)
#endif 
#else
#define SIPDEBUGFN(a)
#endif /* SIP_FNDEBUG */

#ifdef SIP_TRACE
#define sip_trace_printf printf
#endif

#if defined(ICF_PORT_WINDOWS) || defined(SDF_WINDOWS)
/* For windows we use SIP_CRITICAL_SECTION instead of SIP_THREAD_SAFE.*/
/* We use critical section instead of mutex. */

#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#ifdef SIP_CRITICAL_SECTION
typedef CRITICAL_SECTION synch_id_t; 
#else
typedef HANDLE synch_id_t; 
#endif
typedef HANDLE mutex_id_t;
#endif
#endif

#ifdef SIP_THREAD_SAFE
typedef HANDLE thread_id_t;
typedef HANDLE thread_attr_t;
typedef HANDLE thread_cond_t;
#endif

#ifdef SIP_LINUX
#ifdef SIP_THREAD_SAFE
typedef pthread_t thread_id_t;
typedef pthread_mutex_t mutex_id_t; /* port this to mutex library variable */
typedef pthread_mutex_t synch_id_t;
typedef pthread_cond_t	thread_cond_t;
typedef pthread_attr_t	thread_attr_t;
typedef void * (*fpThreadStartFunc)(void *args);
#endif
#endif


#ifdef SIP_THREAD_SAFE
#endif


#define FAST_MXLOCK_SHARED (0)
#define FAST_MXLOCK_EXCLUSIVE (1)

#define STRDUP(x) sip_strdup(x, 0) /* TODO */
#define STRDUPACCESSOR(x) sip_strdup(x, ACCESSOR_MEM_ID)
#define STRDUPNETWORK(x) sip_strdup(x, NETWORK_MEM_ID)
#define STRDUPTIMER(x) sip_strdup(x, TIMER_MEM_ID)

/*
 * Locked ref count  macros are  here
 */
#ifdef SIP_LOCKEDREFCOUNT
#define HSS_INITREF(x) do { fast_init_synch(&(x.lock)); x.ref=1;} while (0)
#define HSS_LOCKREF(x) do { fast_lock_synch(0,&(x.lock),0);} while (0)
#define HSS_UNLOCKREF(x) do { fast_unlock_synch(0,&(x.lock));} while (0)
#define HSS_INCREF(x) do { x.ref++;} while (0)
#define HSS_DECREF(x) do { x.ref--;} while (0)
#define HSS_CHECKREF(x) (x.ref==0)
#define HSS_LOCKEDINCREF(x) do { \
	fast_lock_synch(0,&(x.lock),0); \
	x.ref++;\
	fast_unlock_synch(0,&(x.lock)); } while (0)
#define HSS_LOCKEDDECREF(x) do { \
	fast_lock_synch(0,&(x.lock),0); \
	x.ref--;\
	fast_unlock_synch(0,&(x.lock)); } while (0)
#define HSS_DELETEREF(x) do { fast_free_synch(&(x.lock));} while (0)
/* 
 * End of Locked refcount declarations
 */
#else
#define HSS_INITREF(x) x=1
#define HSS_LOCKREF(x) 
#define HSS_UNLOCKREF(x)  
#define HSS_INCREF(x) x++
#define HSS_DECREF(x) x--
#define HSS_CHECKREF(x) x==0
#define HSS_LOCKEDINCREF(x) x++
#define HSS_LOCKEDDECREF(x) x--
#define HSS_DELETEREF(x) 
/*
 * If ordinary refcount  ends here
 */ 
#endif


#define HSS_SPRINTF sip_sprintf


/* SIP retransmission values variables */
extern SIP_U32bit SIP_T1;
extern SIP_U32bit SIP_T2;
extern SIP_U32bit SIP_MAXRETRANS;
extern SIP_U32bit SIP_MAXINVRETRANS;

/* 
 * In VxWorks the stdout descriptor is not accessed in a thread safe manner
 * Hence if multiplethreads try to write using this variable, VxWorks
 * itself does not provide any safety, hence the application itself needs
 * to do writes in a thread safe mechanism. For this reason the stack uses
 * a mutex to ensure all prints/writes are done in a thread safe manner
 */


/*
 * The 2 below prototypes are functions that are called from within the
 * sip_initStack and sip_releaseStack APIs. These functions handle all port
  specific handling involved in the release and initing of the Stack
 */
extern void sip_initStackPortSpecific(void);
extern void sip_releaseStackPortSpecific(void);

#if defined(SIP_VXWORKS)
#define NUM_BUCKET  100
#else
#define NUM_BUCKET	1000
#endif


/****************************************************************************
 ** FUNCTION :		fast_memget
 **
 ** DESCRIPTION:	This function allocates memory.This function can
 **			be ported for  having appropriate memory management scheme.
 ** PARAMETERS:
 **	 		module_id(IN):  The Module for which memory is allocated.
 **			noctets(IN):   	The number of bytes of memory to be allocated
 **			SipError(IN):	The SipError value
 **
 **			Return Value:
 **			Pointer to allocated buffer or Null
 **
 ****************************************************************************/
extern SIP_Pvoid fast_memget _ARGS_((SIP_U32bit module_id, SIP_U32bit noctets, \
			SipError *err));
extern SIP_Pvoid fast_memrealloc _ARGS_((SIP_U32bit module_id, SIP_Pvoid pBuffer, SIP_U32bit noctets, SipError *err));
/****************************************************************************
 ** FUNCTION :		sip_memget
 **
 ** DESCRIPTION:	This function allocates memory.This function can
 **			be ported for  having appropriate memory management scheme.
 ** PARAMETERS:
 **	 		module_id(IN):  The Module for which memory is allocated.
 **			noctets(IN):   	The number of bytes of memory to be allocated
 **			SipError(IN):	The SipError value
 **
 **			Return Value:
 **			Pointer to allocated buffer or Null
 **
 ****************************************************************************/

extern SIP_Pvoid sip_memget _ARGS_((SIP_U32bit module_id, SIP_U32bit noctets, \
		SipError *err));
/****************************************************************************
 ** FUNCTION :		fast_memfree
 **
 ** DESCRIPTION:	This function frees allocated memory.This function can
 **			be ported for  having appropriate memory management scheme.
 ** PARAMETERS:
 **	 		module_id(IN):  The Module for which memory is allocated.
 **			p_buf(IN)	: 	The chunk of memory to be freed
 **			SipError(IN):	The SipError value
 ** Return Value:
 **				SipBool
 ****************************************************************************/

extern SipBool fast_memfree _ARGS_((SIP_U32bit module_id, SIP_Pvoid p_buf , \
		SipError *err));
/****************************************************************************
 ** FUNCTION :		sip_memfree
 **
 ** DESCRIPTION:	 frees memory and SIP_NULLifies pointer. Accepts pointer
** to pointer to area 
** Allows pUser to pass SIP_NULL in SipError* param
 ** PARAMETERS:
 **	 		module_id(IN)	: The Module for which memory is allocated.
 **			p_buf(IN)		: The chunk of memory to be freed
 **			err(IN)			: The SipError value
 ** Return Value:
 **				SipBool
 ****************************************************************************/

extern SipBool sip_memfree _ARGS_((SIP_U32bit module_id, SIP_Pvoid* p_buf , \
		SipError *err));

#ifdef SIP_THREAD_SAFE
/****************************************************************************
 ** FUNCTION :		fast_lock_mutex
 **
 ** DESCRIPTION:	This function locks\protects a critical section\global.
 ** PARAMETERS:
 **	 		thread_id_t(IN)			: The thread id.
 **			mutex 		(IN)		: mutex/semaphore id
 **			flags		(IN)		: The flags to be used if any
 **			Return Value:
 **			void
 **
 ****************************************************************************/
#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
extern void fast_lock_mutex _ARGS_((thread_id_t tid,\
       mutex_id_t *mutex, SIP_U32bit flags));
#endif
extern void fast_lock_synch _ARGS_((thread_id_t tid, synch_id_t *mutex, \
	SIP_U32bit flags) );
/****************************************************************************
 ** FUNCTION :		fast_unlock_mutex
 **
 ** DESCRIPTION:	This function locks\protects a critical section\global.
 ** PARAMETERS:
 **	 		thread_id_t(IN)			: The thread id.
 **			mutex 		(IN)		: mutex/semaphore id
 **			Return Value:
 **			void
 **
 ****************************************************************************/
#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
extern void fast_unlock_mutex _ARGS_((thread_id_t tid,\
       mutex_id_t *mutex));
#endif
extern void fast_unlock_synch _ARGS_ ((thread_id_t tid, synch_id_t *mutex));

/*****************************************************************
** FUNCTION:fast_trylock_synch
**
**
** DESCRIPTION:
*******************************************************************/
#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
extern SipBool fast_trylock_mutex _ARGS_((thread_id_t tid,\
       mutex_id_t *mutex, SIP_U32bit flags));
#endif
extern SipBool fast_trylock_synch _ARGS_ ((thread_id_t tid, synch_id_t *mutex, \
	SIP_U32bit flags));


/****************************************************************************
 ** FUNCTION :		fast_init_mutex
 **
 ** DESCRIPTION:	This function initializes mutex/semaphore.
 ** PARAMETERS:
 **			mutex (IN/OUT)		: mutex to be initialized
 **			Return Value:
 **			void
 **
 ****************************************************************************/
#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
extern void fast_init_mutex _ARGS_((mutex_id_t *mutex));
#endif
extern void fast_init_synch _ARGS_ ((synch_id_t *mutex));


/*****************************************************************
** FUNCTION:fast_free_synch
** DESCRIPTION: This function frees the mutex.
**
** PARAMETERS:
**			mutex(IN/OUT): mutex id to be freed
**
**
** RETURN VALUE: None
*******************************************************************/
#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
extern void fast_free_mutex _ARGS_((mutex_id_t *mutex));
#endif
extern void fast_free_synch _ARGS_ ((synch_id_t *mutex));


#endif

/****************************************************************************
** FUNCTION:STRTOLCAP
** DESCRIPTION: converts string to unsigned long.
** this function wont caps off pValue to 2^32-1 if larger pValue is
** given as input. 
**
** PARAMETERS:
**	 		str(IN)		:  The Module for which memory is allocated.
**			Return Value
**			SIP_U32bit
**			
**
 ****************************************************************************/

extern SIP_U32bit STRTOLCAP _ARGS_(( SIP_S8bit *str ));
/****************************************************************************
** FUNCTION:sip_getTimeString
** DESCRIPTION: Gets the current time as a string
**
** PARAMETERS:
**	 		ppString(OUT)	:  Returned time string.
**			Return Value
**			void
**			
**
 ****************************************************************************/
void sip_getTimeString _ARGS_((SIP_S8bit **pString));

/*******************************************************************************
** FUNCTION:sip_strcmp
** DESCRIPTION: wrapper function around C strncmp function.
** 
** PARAMETERS:
**			pStr1 (IN)	 : String 1 to be compared
**			pStr2 (IN)	 : String 2 to be compared
** Return Value:
**			SIP_U32bit
** 
*******************************************************************************/
extern SIP_U32bit sip_strcmp _ARGS_((SIP_S8bit* dest, \
	SIP_S8bit* source));
/*******************************************************************************
** FUNCTION:sip_strncmp
** DESCRIPTION: wrapper function around C strcmp function.
** 
** PARAMETERS:
**			pStr1 (IN)	 : String 1 to be compared
**			pStr2 (IN)	 : String 2 to be compared
**			dSize (IN)	 : n number of bytes , to be compared
** Return Value:
**			SIP_U32bit
** 
*******************************************************************************/
	
extern SIP_U32bit sip_strncmp _ARGS_((SIP_S8bit* dest, \
	SIP_S8bit* source, SIP_U32bit n));

/*******************************************************************************
** FUNCTION:sip_strncpy
** DESCRIPTION: wrapper function around C strncpy function.
** 
** PARAMETERS:
**			pStr1 (OUT)	 : destination
**			pStr2 (IN)	 : source
**			 n	   (n)	 : n bytes to be copied
** Return Value:
**			SIP_S8bit *
** 
*******************************************************************************/
extern SIP_S8bit *sip_strncpy _ARGS_((SIP_S8bit* dest, \
	SIP_S8bit* source, SIP_U32bit n));

/*******************************************************************************
** FUNCTION:sip_strlen
** DESCRIPTION: wrapper function around C strlen function.
** 
** PARAMETERS:
**			pStr1 (OUT)	 : the string whose length to be found.
** Return Value:
**			SIP_U32bit
** 
*******************************************************************************/
extern SIP_U32bit sip_strlen _ARGS_((SIP_S8bit *pStr));

/*******************************************************************************
** FUNCTION:sip_strstr
** DESCRIPTION: wrapper function around C strstr function.
** 
** PARAMETERS:
**			pStr1 (IN)	 : the pattern string .
**			pStr2 (OUT)	 : the string in which the pattern has to be found.
** Return Value:
**			SIP_Sbit *
** 
*******************************************************************************/
extern SIP_S8bit *sip_strstr _ARGS_((SIP_S8bit* dest, \
	SIP_S8bit* source));

/*******************************************************************************
** FUNCTION:sip_memset
** DESCRIPTION: wrapper function around C memset function.
** 
** PARAMETERS:
**			pStr (IN/OUT): starting point of input buffer
**			dVal (IN)	 : The value to be set
**			n    (IN)	 : n number of bytes , which has to be set with dVal
** 
*******************************************************************************/
extern SIP_Pvoid sip_memset _ARGS_((SIP_Pvoid pStr, SIP_U32bit dVal, \
	SIP_U32bit dSize));

/*******************************************************************************
** FUNCTION:sip_sprintf
** DESCRIPTION: wrapper function around C sprintf function.
** 
** PARAMETERS:
**			pStr (OUT) :the buffer to which the formatted string to be printed.
**			pFmt (OUT) : the format string.
** Return Value:
**			SIP_U32bit
** 
*******************************************************************************/
extern SIP_U32bit sip_sprintf _ARGS_((SIP_S8bit *pStr, SIP_S8bit *pFmt, ...));

/*******************************************************************************
** FUNCTION:sip_strcat
** DESCRIPTION: wrapper function around C strcat function.
** 
** PARAMETERS:
**			pDest (OUT) :the string to which another string is to be added
**			pSrc  (OUT) :the string , which must be appended to pDest
** Return Value:
**			SIP_S8bit *
** 
*******************************************************************************/

extern SIP_S8bit* sip_strcat(SIP_S8bit* dest,SIP_S8bit* src);


/*******************************************************************************
** FUNCTION:sip_strcpy
** DESCRIPTION: wrapper function around C strcy function.
** 
** PARAMETERS:
**			pDest (OUT) :the string to which another string is to be copied
**			pSrc  (OUT) :the string , which must be copied to pDest
** Return Value:
**			SIP_S8bit *
** 
*******************************************************************************/

extern SIP_S8bit* sip_strcpy(SIP_S8bit* dest,SIP_S8bit *src);
/****************************************************************************
** FUNCTION:STRTOU32LCAP
** DESCRIPTION: converts string to unsigned long.
** caps off pValue to 2^32-1 if larger pValue is given as input. \
**
** PARAMETERS:
**	 		str(IN)		:  The Module for which memory is allocated.
**			pErr(OUT)	:  The number of bytes of memory to be allocated
**			Return Value
**			SIP_U32bit
**			
**
 ****************************************************************************/

extern SIP_U32bit STRTOU32CAP _ARGS_(( SIP_S8bit *str , SipError* pErr));

/*******************************************************************************
** FUNCTION:sip_strdup
** DESCRIPTION: wrapper function around C strdup function.
** 
** PARAMETERS:
**			pStr (OUT) :the string to be , duplicated.
**			dMemId(IN) : the module id of the requesting entity
** Return Value:
**			SIP_S8bit *
** 
*******************************************************************************/

extern char*  sip_strdup
        (const char *pOriginalString, SIP_U32bit dMemId);

/*******************************************************************************
** FUNCTION:sip_strncasecmp
** DESCRIPTION: wrapper function around C strncasecmp function.
** 
** PARAMETERS:
**			pStr1 (IN)	 : String 1 to be compared
**			pStr2 (IN)	 : String 2 to be compared
**			 n	   (n)	 : n bytes to be compared
** Return Value:
**			SIP_U32bit
** 
*******************************************************************************/

extern SIP_U32bit sip_strncasecmp _ARGS_ ((SIP_S8bit *pStr1, SIP_S8bit *pStr2,\
		SIP_U32bit n));
/*******************************************************************************
** FUNCTION:sip_strcasecmp
** DESCRIPTION: wrapper function around C strcasecmp function.
** 
** PARAMETERS:
**			pStr1 (IN)	 : String 1 to be compared
**			pStr2 (IN)	 : String 2 to be compared
** Return Value:
**			SIP_U32bit
** 
*******************************************************************************/

extern SIP_U32bit sip_strcasecmp _ARGS_ ((const SIP_S8bit *pStr1, const SIP_S8bit *pStr2));

/*******************************************************************************
** FUNCTION:sip_snprintf
** DESCRIPTION: wrapper function around C snprintf function.
** 
** PARAMETERS:
**			pStr (OUT) :the buffer to which the formatted string to be printed.
**			pFmt (IN) : the format string.
**			n    (IN) :	N bytes to be printed
**
** Return Value:
**			SIP_U32bit
** 
*******************************************************************************/

extern   SIP_U32bit sip_snprintf _ARGS_ ((SIP_S8bit *s , size_t n, \
	const SIP_S8bit * fmt, ...));
#ifndef SIP_OVERRIDE_SNPRINTF
#if defined SIP_WINDOWS || defined SIP_WINCE
#define sip_snprintf _snprintf
#else
#define sip_snprintf snprintf
#endif
#endif

/* Solaris/Linux strings.h seems to have a problem with prototype */
#if !defined (SIP_WINDOWS) && !defined (SIP_WINCE)
extern char *strdup _ARGS_((const char *)); 
extern int	fileno _ARGS_((FILE *));
#endif

extern int strcasecmp _ARGS_((const char *, const char *));

/* Ensure Names are not mangled by C++ compilers */

#endif

#ifdef ICF_PORT_WINCE
/*******************************************************************************
** FUNCTION:sip_wideCharToMultiByte
** DESCRIPTION: Function to convert wide character string(16 bit) to
**				multibyte string(8 bit).(Required for Windows CE)
**				
** PARAMETERS:
**			pWbuf	(IN): Wide character string.
**			
**
** Return Value:
**			SIP_S8bit*
** 
*******************************************************************************/
SIP_S8bit* sip_wideCharToMultiByte
#ifdef ANSI_PROTO
	(SIP_U16bit *pWbuf);
#else
	(pWbuf);
	SIP_U16bit *pWbuf;
#endif
#endif

#ifdef __cplusplus
}
#endif 

