/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** FUNCTION:
**		All the porting layer functions of the stack.
********************************************************************************      
**
** FILENAME:
** microsip_portlayer.c
**
** DESCRIPTION:
** 		This file contains all the functions which need to be ported across
**		different O/S.  
**
** DATE				NAME			REFERENCE	  REASON
** ----				----			---------	  --------
** 06/03/2002	 Mahesh Govind 			-		portlayer.c of core stk. 
** 18/03/2002	 Aparna Kuppachi		-		Added wrappers for strncmp,
**												strlen and memset.
** 26/03/2002	 Aparna Kuppachi		-		Added wrappers for strncpy
**												strstr and sprintf.
** 29-Jan-2009   Alok Tiwari      ICF Rel-8.1   Changes done for 
**                                flag ICF_STACK_PORT.This flag filters
**                                the ICF related code/changes from this file.
** 07-Feb-09     Alok Tiwari      ICF Rel-8.1   Code review comments disposed.
** 12-Jun-09     Anuradha Gupta   SPR 19737      sip_strncat defined
** 18-Jul-2009   Rajiv Kumar      IPTK Rel 8.2   Vxwork 5.4 porting
** 12-Aug-2009   Anuradha Gupta   SPR 19748      Modified fast_memget
**
** COPYRIGHT , Aricent, 2009
*******************************************************************************/
	
/*Changes done for Vxwork 5.4 Porting */	
#ifdef 	ICF_STACK_PORT

#if defined (ICF_PORT_WINDOWS) && !defined (ICF_PORT_WINCE)
#include <sys/timeb.h>
#endif

#if defined(SIP_WINDOWS) || defined(SIP_SOLARIS)
#include <time.h>
#endif

#ifdef ICF_PORT_LINUX
#include <sys/time.h>
#endif

#if defined(SIP_WINDOWS) && defined(SIP_THREAD_SAFE)
#include "Windows.h"
#endif
	
#if defined (ICF_PORT_SYMBIAN)
#include <sys\time.h>
#endif

#ifndef ICF_PORT_WINCE
#ifndef SIP_VXWORKS
#include <errno.h>
#endif
#endif

#else  /*ifndef ICF_STACK_PORT*/

#include <sys/time.h>
#include <errno.h>

#endif  /*ICF_STACK_PORT*/

#ifndef SIP_VXWORKS
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#else
#include "stdlib.h"
#include "stdarg.h"
#include "limits.h"
#include "errno.h"
#endif

#include "microsip_common.h"
#include "microsip_portlayer.h"
#include "microsip_trace.h"


#define MAX_LENGTH 4294967295u

#ifdef 	ICF_STACK_PORT
/*ICF inclusion*/
#include "icf_common_types.h"
#include "icf_macro.h"
#include "icf_common_prototypes.h"

/*Defined in Eshell*/
extern icf_glb_pdb_st        *p_persistent_glb_pdb;
#endif /*ICF_STACK_PORT*/

SIP_U32bit	SIP_T1; 			/* T1 retrans value */
SIP_U32bit	SIP_T2; 			/* Cap off value */
SIP_U32bit	SIP_MAXRETRANS; 	   /*
									* No. of times non INVITE messages 
									* will be retransmitted 
									*/
SIP_U32bit  SIP_MAXINVRETRANS;	/* 
								 * No. of times INVITE messages 
								 * will be retransmitted 
								 */

/* 
 * In VxWorks the stdout descriptor is not accessed in a thread safe manner
 * Hence if multiplethreads try to write using this variable, VxWorks
 * itself does not provide any safety, hence the application itself needs
 * to do writes in a thread safe mechanism. For this reason the stack uses
 * a mutex to ensure all prints/writes are done in a thread safe manner
 */

#ifdef SIP_THREAD_SAFE
#endif

/*****************************************************************
** FUNCTION:sip_releaseStackPortSpecific
**
**
** DESCRIPTION: This functions handles all the port specific handling 
**              while the sip_releaseStack API is called.
*******************************************************************/
void sip_releaseStackPortSpecific(void)
{
int dummy;
	(void)dummy;
#ifdef SIP_THREAD_SAFE
#endif	
}

/*****************************************************************
** FUNCTION:sip_initStackPortSpecific
**
**
** DESCRIPTION: This functions handles all the port specific handling 
**              while the sip_initStack API is called.
*******************************************************************/
void sip_initStackPortSpecific(void)
{
int dummy;
	(void)dummy;				
#ifdef SIP_THREAD_SAFE
#endif	
}
#ifdef SIP_THREAD_SAFE
#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
/*****************************************************************
** FUNCTION:fast_lock_mutex
**
**
** DESCRIPTION:
*******************************************************************/
void fast_lock_mutex
#ifdef ANSI_PROTO
	(thread_id_t tid, mutex_id_t *mutex, SIP_U32bit flags)
#else
	(tid, mutex, flags)
	thread_id_t tid;
	mutex_id_t *mutex;
	SIP_U32bit flags;
#endif
{
	thread_id_t dummy_tid;
	SIP_U32bit dummy_flags;
	dummy_tid = tid;
	dummy_flags =flags;
    (void)dummy_flags;
	(void)dummy_tid;

	(void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Grabbing Mutex Lock");
	(void)WaitForSingleObject(*mutex, INFINITE);
	(void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Successfully Grabbed Mutex Lock");
}
/*****************************************************************
** FUNCTION:fast_trylock_mutex
**
**
** DESCRIPTION:
*******************************************************************/
SipBool fast_trylock_mutex
#ifdef ANSI_PROTO
	(thread_id_t tid, mutex_id_t *mutex, SIP_U32bit flags)
#else
	(tid, mutex, flags)
	thread_id_t tid;
	mutex_id_t *mutex;
	SIP_U32bit flags;
#endif
{
	thread_id_t dummy_tid;
	SIP_U32bit dummy_flags;
	dummy_tid = tid;
	dummy_flags =flags;
    (void)dummy_flags;
	(void)dummy_tid;

/* Port according to system and library selection */
	(void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Trying to Grab Mutex Lock");
	if ( WaitForSingleObject(*mutex,SIP_RETURN_IMMEDIATELY) == WAIT_TIMEOUT )
		return SipFail;
	(void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Successfully Grabbed Mutex Lock");
	return SipSuccess;
}

/*****************************************************************
** FUNCTION:fast_unlock_mutex
**
**
** DESCRIPTION:
*******************************************************************/
void fast_unlock_mutex
#ifdef ANSI_PROTO
	(thread_id_t tid, mutex_id_t *mutex)
#else
	(tid, mutex)
	thread_id_t tid;
	mutex_id_t *mutex;
#endif
{
	thread_id_t dummy_tid;
	dummy_tid = tid;
    (void)dummy_tid;
	(void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Released Mutex Lock");
	(void)ReleaseMutex(*mutex);
}

/*****************************************************************
** FUNCTION:fast_init_mutex
**
**
** DESCRIPTION:
*******************************************************************/
void fast_init_mutex
#ifdef ANSI_PROTO
	(mutex_id_t *mutex)
#else
	( mutex)
	mutex_id_t *mutex;
#endif
{
	/* removed PTHREAD_PROCESS_PRIVATE from the next statement & made null*/
	*mutex = CreateMutex(NULL, FALSE, NULL);
	(void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Initialised Mutex Lock");
}
/*****************************************************************
** FUNCTION:fast_free_mutex
**
**
** DESCRIPTION:
*******************************************************************/
void fast_free_mutex
#ifdef ANSI_PROTO
	(mutex_id_t *mutex)
#else
	( mutex)
	mutex_id_t *mutex;
#endif
{
	/* removed PTHREAD_PROCESS_PRIVATE from the next statement & made null*/
	(void)ReleaseMutex(*mutex);
	(void)CloseHandle(*mutex);
	(void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Destroy Mutex Lock");
}
#endif
/****************************************************************************
 ** FUNCTION :		fast_lock_synch
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
void fast_lock_synch
#ifdef ANSI_PROTO
	(thread_id_t tid, synch_id_t *mutex, SIP_U32bit flags)
#else
	(tid, mutex, flags)
	thread_id_t tid;
	synch_id_t *mutex;
	SIP_U32bit flags;
#endif
{
	thread_id_t dummy_tid;
	SIP_U32bit dummy_flags;
	dummy_tid = tid;
	dummy_flags =flags;

#ifdef SIP_SOLARIS
	pthread_mutex_lock(mutex);
#endif
#ifdef SIP_LINUX
	pthread_mutex_lock(mutex);
#endif
#ifdef SIP_VXWORKS
	semTake (*mutex,WAIT_FOREVER);
#endif

#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
    /* Port according to system and library selection */
#ifdef SIP_CRITICAL_SECTION
EnterCriticalSection(mutex);
#else
WaitForSingleObject(*mutex, INFINITE);
#endif
#endif /*SIP_WINDOWS*/

}


/*****************************************************************
** FUNCTION:fast_trylock_synch
**
**
** DESCRIPTION:
*******************************************************************/
SipBool fast_trylock_synch
#ifdef ANSI_PROTO
	(thread_id_t tid, synch_id_t *mutex, SIP_U32bit flags)
#else
	(tid, mutex, flags)
	thread_id_t tid;
	synch_id_t *mutex;
	SIP_U32bit flags;
#endif
{
#ifdef ICF_STACK_PORT
	thread_id_t dummy_tid;
	SIP_U32bit dummy_flags;
	dummy_tid = tid;
	dummy_flags =flags;

/* Port according to system and library selection */
#ifndef SIP_VXWORKS  
	sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Trying to Grab Mutex Lock");
#endif
#ifdef SIP_SOLARIS
	if ( pthread_mutex_trylock(mutex) != 0 )
		return SipFail;
#endif
#ifdef SIP_LINUX
	if ( pthread_mutex_trylock(mutex) != 0 )
		return SipFail;
#endif
#ifdef SIP_VXWORKS
	if ( semTake (*mutex,NO_WAIT) != OK )
		return SipFail;
#endif


#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#ifndef SIP_CRITICAL_SECTION
	if ( WaitForSingleObject(*mutex,SIP_RETURN_IMMEDIATELY) == WAIT_TIMEOUT )
		return SipFail;
#endif
#endif/*SIP_WINDOWS*/

#ifndef SIP_VXWORKS  
sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Successfully Grabbed Mutex Lock");
#endif

#else /****** ifndef ICF_STACK_PORT *******/

    (void)tid;
    (void)flags;
    /* Port according to system and library selection */
	(void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Trying to Grab Mutex Lock");

	if ( pthread_mutex_trylock(mutex) != 0 )
		return SipFail;


#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#ifndef SIP_CRITICAL_SECTION
	if ( WaitForSingleObject(*mutex,SIP_RETURN_IMMEDIATELY) == WAIT_TIMEOUT )
		return SipFail;
#endif
#endif

    (void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Successfully Grabbed Mutex Lock");

#endif /******ICF_STACK_PORT********************/

return SipSuccess;
}

 
/****************************************************************************
 ** FUNCTION :		fast_unlock_synch
 **
 ** DESCRIPTION:	This function locks\protects a critical section\global.
 ** PARAMETERS:
 **	 		thread_id_t(IN)			: The thread id.
 **			mutex 		(IN)		: mutex/semaphore id
 **			Return Value:
 **			void
 **
 ****************************************************************************/
void fast_unlock_synch
#ifdef ANSI_PROTO
	(thread_id_t tid, synch_id_t *mutex)
#else
	(tid, mutex)
	thread_id_t tid;
	synch_id_t *mutex;
#endif
{
#ifdef ICF_STACK_PORT
	thread_id_t dummy_tid;
	dummy_tid = tid;
/* Port according to system and library selection */
#ifdef SIP_SOLARIS
	pthread_mutex_unlock(mutex);
#endif
#ifdef SIP_LINUX
	pthread_mutex_unlock(mutex);
#endif
#ifdef SIP_VXWORKS
	semGive(*mutex);
#endif

#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#ifdef SIP_CRITICAL_SECTION
	(void)LeaveCriticalSection(mutex);
#else
	(void)ReleaseMutex(*mutex);
#endif
#endif/*SIP_WINDOWS*/

#else /******* ifndef ICF_STACK_PORT ********/
    (void)tid;
    /* Port according to system and library selection */
	(void)pthread_mutex_unlock(mutex);

#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#ifdef SIP_CRITICAL_SECTION
	LeaveCriticalSection(mutex);
#else
	(void)ReleaseMutex(*mutex);
#endif
#endif
#endif/***ICF_STACK_PORT****/
}

/****************************************************************************
 ** FUNCTION :		fast_init_synch
 **
 ** DESCRIPTION:	This function initializes mutex/semaphore.
 ** PARAMETERS:
 **			mutex (IN/OUT)		: mutex to be initialized
 **			Return Value:
 **			void
 **
 ****************************************************************************/
void fast_init_synch
#ifdef ANSI_PROTO
	(synch_id_t *mutex)
#else
	( mutex)
	synch_id_t *mutex;
#endif
{
#ifdef ICF_STACK_PORT
/* Port according to system and library selection */
	/* removed PTHREAD_PROCESS_PRIVATE from the next statement & made null*/
#ifdef SIP_SOLARIS
	pthread_mutex_unlock(mutex);
#endif
#ifdef SIP_LINUX
	pthread_mutex_unlock(mutex);
#endif
#ifdef SIP_VXWORKS
	semGive(*mutex);
#endif

#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#ifdef SIP_CRITICAL_SECTION
	InitializeCriticalSection(mutex);
#else
	*mutex = CreateMutex(NULL, FALSE, NULL);
#endif
#endif/*SIP_WINDOWS*/
	(void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Initialised Mutex Lock");

#else /*****ifndef ICF_STACK_PORT *********/

/* Port according to system and library selection */
	/* removed PTHREAD_PROCESS_PRIVATE from the next statement & made null*/
	/*(void)pthread_mutex_init(mutex,NULL);*/
    pthread_mutexattr_t attr;
	(void)pthread_mutexattr_init(&attr);
	(void)pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
    (void)pthread_mutex_init(mutex,&attr);
	(void)pthread_mutexattr_destroy(&attr);

#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#ifdef SIP_CRITICAL_SECTION
	InitializeCriticalSection(mutex);
#else
	*mutex = CreateMutex(NULL, FALSE, NULL);
#endif
#endif
	(void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Initialised Mutex Lock");

#endif /********ICF_STACK_PORT*******/
}
 
/*endif of #ifdef SIP_THREAD_SAFE*/
#endif

/*****************************************************************
* FUNCTION NAME :fast_memrealloc
*
*
* DESCRIPTION:
*******************************************************************/

SIP_Pvoid fast_memrealloc
#ifdef ANSI_PROTO
(SIP_U32bit module_id, SIP_Pvoid pBuffer, SIP_U32bit noctets, SipError *err)
#else
	(moduleId, pBuffer, noctets, err)
	SIP_U32bit module_id;
    SIP_Pvoid pBuffer;
	SIP_U32bit noctets;
	SipError *err;
#endif
{
    SIP_Pvoid pvoid;
    (void)module_id;

#ifdef DYNAMIC_MEMORY_CALC_BUGGY
    /* This calculation might be wrong we need to test and 
      fix this.
      However this is for internal purposes and should not
      affect the working of the stack.
     */
#endif

    pvoid = (SIP_Pvoid)realloc(pBuffer, noctets);

    if(pvoid == SIP_NULL)
    {
        if (err!=SIP_NULL) 
        {
            *err = E_NO_MEM;
        }
        return SIP_NULL;
    }

    if (err!=SIP_NULL) 
    {
        *err = E_NO_ERROR;
    }

#ifdef DYNAMIC_MEMORY_CAL_BUGGY
  {
      int indx=0 ;
      int flag=0 ;

      /*
       * If the allocation is done for parser modules
       * then add the memory allocated to the count
       */

      if ( module_id == DECODE_MEM_ID )
      {
          glb_memory_alloc_parser[0] -= initSize ;
          glb_memory_alloc_parser[0] += noctets ;
      }
      else if ( module_id == BISON_MEM_ID )
      {
          glb_memory_alloc_parser[1] -= initSize ;
          glb_memory_alloc_parser[1] += noctets ;
      }
      else if ( module_id == FLEX_MEM_ID )
      {
          glb_memory_alloc_parser[1] -= initSize ;
          glb_memory_alloc_parser[2] += noctets ;
      }
      else
      {
          flag = 1 ;
      }

      /*
       * If allocation is done for parser module
       * then count the memory allocation
       * pattern
       */
      if ( flag == 0 )
      {
          if ( (0 < noctets) && (noctets  <= 10) )
              indx = 1 ;
          else  if ( (10 < noctets  ) && ( noctets <= 100 ) )
              indx = 2 ;
          else  if ( (100 < noctets ) && (noctets <= 1000 ) )
              indx = 3 ;
          else
              indx = 4 ;
          glb_memory_alloc_dist[indx]++ ;
      }
  }
#endif
  return pvoid;
}

/****************************************************************************
 ** FUNCTION :		fast_memget
 **
 ** DESCRIPTION:	This function allocates memory.This function can
 **			be ported for  having appropriate memory management scheme.
 ** PARAMETERS:
 **	 		module_id(IN):  The Module for which memory is allocated.
 **			noctets(IN):   	The number of bytes of memory to be allocated
 **			SipError(IN):	The SipError value
 **			Return Value:
 **			Pointer to allocated buffer or Null
 **
 ****************************************************************************/

SIP_Pvoid fast_memget
#ifdef ANSI_PROTO
	( SIP_U32bit module_id, SIP_U32bit noctets, SipError *err)
#else
	(module_id, noctets, err)
	SIP_U32bit module_id;
	SIP_U32bit noctets;
	SipError *err;
#endif
{
#ifdef ICF_STACK_PORT
    icf_error_t      ecode;
    icf_void_t       *p_buff = ICF_NULL;

    module_id = module_id;
	err = err;

    p_buff = icf_cmn_memget(p_persistent_glb_pdb, \
            (icf_uint32_t)noctets, \
            ICF_MEM_COMMON, &ecode);

    if (ICF_NULL == p_buff)
    {
        /* SPR 19748: In case of memory allocation failure , set the
         * error code as E_NO_MEM
         */ 
        if (err!=SIP_NULL) *err = E_NO_MEM;
        ICF_SET_ECODE(p_persistent_glb_pdb, ICF_NULL,\
                ICF_ERROR_MEM_ALLOC_FAILURE, &ecode)
            return SIP_NULL;
    }
    else
    {
        return (SIP_Pvoid)p_buff ;
    }
#else /*ifndef ICF_STACK_PORT*/

    SIP_Pvoid pvoid;
    (void)module_id;
	pvoid = (SIP_Pvoid)malloc(noctets);
	if(pvoid == SIP_NULL)
	{
		if (err!=SIP_NULL) *err = E_NO_MEM;
		return SIP_NULL;
	}

	if (err!=SIP_NULL) *err = E_NO_ERROR;
	return pvoid;

#endif /*end of ICF_STACK_PORT*/
}
/****************************************************************************
 ** FUNCTION :		fast_memfree
 **
 ** DESCRIPTION:	This function allocates memory.This function can
 **			be ported for  having appropriate memory management scheme.
 ** PARAMETERS:
 **	 		module_id(IN):  The Module for which memory is allocated.
 **			noctets(IN):   	The number of bytes of memory to be allocated
 **			SipError(IN):	The SipError value
 ** Return Value:
 **				SipBool
 ****************************************************************************/
SipBool fast_memfree
#ifdef ANSI_PROTO
	( SIP_U32bit module_id, SIP_Pvoid p_buf , SipError *err)
#else
	(module_id, p_buf, err)
	SIP_U32bit module_id;
	SIP_Pvoid p_buf;
	SipError *err;
#endif
{
#ifdef ICF_STACK_PORT
	icf_error_t      ecode;
    icf_return_t     ret_val = ICF_SUCCESS;

    ret_val = icf_cmn_memfree(p_persistent_glb_pdb,
            (icf_void_t*)(p_buf),
            ICF_MEM_COMMON, &ecode);
    if (SIP_NULL != err)
    {
        *err = E_NO_ERROR;
    }
    p_buf=SIP_NULL;
    module_id = 0;

    return SipSuccess;
#else /*ifndef ICF_STACK_PORT*/

	(void)module_id;
    if(p_buf!=SIP_NULL)
	free(p_buf);

	if (err != SIP_NULL) 
		*err = E_NO_ERROR;
	return SipSuccess;

#endif /*ICF_STACK_PORT*/
}

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

SIP_Pvoid sip_memget
#ifdef ANSI_PROTO
	( SIP_U32bit module_id, SIP_U32bit noctets, SipError *err)
#else
	(module_id, noctets, err)
	SIP_U32bit module_id;
	SIP_U32bit noctets;
	SipError *err;
#endif
{
#ifdef ICF_STACK_PORT
    icf_error_t      ecode;
    icf_void_t       *p_buff = ICF_NULL;

    module_id = module_id;
err = err;

    p_buff = icf_cmn_memget(p_persistent_glb_pdb, \
            (icf_uint32_t)noctets, \
            ICF_MEM_COMMON, &ecode);

    if (ICF_NULL == p_buff)
    {
        ICF_SET_ECODE(p_persistent_glb_pdb, ICF_NULL,\
                ICF_ERROR_MEM_ALLOC_FAILURE, &ecode)
            return SIP_NULL;
    }
    else
    {
        return (SIP_Pvoid)p_buff ;
    }
#else /*ifndef ICF_STACK_PORT*/

	return ( fast_memget(module_id, noctets, err));

#endif /*ICF_STACK_PORT*/
}

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

SipBool sip_memfree
#ifdef ANSI_PROTO
	( SIP_U32bit module_id, SIP_Pvoid* p_buf , SipError *err)
#else
	(module_id, p_buf, err)
	SIP_U32bit module_id;
	SIP_Pvoid* p_buf;
	SipError *err;
#endif
{
#ifdef ICF_STACK_PORT
    icf_error_t      ecode;
    icf_return_t     ret_val = ICF_SUCCESS;

    module_id = module_id;

    ret_val = icf_cmn_memfree(p_persistent_glb_pdb,
            (icf_void_t*)(*p_buf),
            ICF_MEM_COMMON, &ecode);
    if (SIP_NULL != err)
    {
        *err = E_NO_ERROR;
    }
    *p_buf=SIP_NULL;
    return SipSuccess;

#else /*ifndef ICF_STACK_PORT*/

	SipBool x;

	x = fast_memfree(module_id, *p_buf, err);
	*p_buf=SIP_NULL;
	return x;

#endif /*ICF_STACK_PORT*/
}
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

#ifdef SIP_OVERRIDE_SNPRINTF
SIP_U32bit  sip_snprintf(SIP_S8bit *s , size_t n, const SIP_S8bit *fmt, ...)
{

	va_list  ap;
	(void)n;

	va_start(ap,fmt);
	vsprintf(s,fmt,ap);
	va_end(ap);
	return 1; /* Should realy be returning the number of bytes formatted */
}
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

SIP_U32bit STRTOLCAP
#ifdef ANSI_PROTO
	( SIP_S8bit *str )
#else
	(str)
	SIP_S8bit *str;
#endif
{
	return strtoul(str,NULL,10);
	/* Capping functionality missing */
}

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

SIP_Pvoid sip_memset
#ifdef ANSI_PROTO
	(SIP_Pvoid pStr, SIP_U32bit dVal, SIP_U32bit dSize)
#else
	(pStr, dVal, dSize)
	SIP_Pvoid *pStr;
	SIP_U32bit dVal;
	SIP_U32bit dSize;
#endif
{
	return memset(pStr, dVal, (size_t) dSize);
}
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
SIP_U32bit sip_strncmp
#ifdef ANSI_PROTO
	(SIP_S8bit *pStr1, SIP_S8bit *pStr2, SIP_U32bit dSize)
#else
	(pStr1, pStr2, dSize)
	SIP_S8bit *pStr1;
	SIP_S8bit *pStr2;
	SIP_U32bit dSize;
#endif
{
	return strncmp(pStr1, pStr2, (size_t) dSize);
}
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

SIP_U32bit sip_strcmp
#ifdef ANSI_PROTO
	(SIP_S8bit *pStr1, SIP_S8bit *pStr2)
#else
	(pStr1, pStr2)
	SIP_S8bit *pStr1;
	SIP_S8bit *pStr2;
#endif
{
	return strcmp(pStr1, pStr2);
}

/*******************************************************************************
** FUNCTION:sip_strdup
** DESCRIPTION: wrapper function around C strdup function.
** 
** PARAMETERS:
**			pStr (OUT) :the string to be , duplicated.
** Return Value:
**			SIP_S8bit *
** 
*******************************************************************************/
char*  sip_strdup
#ifdef ANSI_PROTO
        (const char *pOriginalString, SIP_U32bit dMemId)
#else
        (pOriginalString, dMemId)
        const char *pOriginalString;
		SIP_U32bit dMemId;
#endif
{
	char *pRetVal=SIP_NULL;
	SIP_U32bit dSize;

	if (pOriginalString == SIP_NULL)
		return SIP_NULL;

	dSize = strlen(pOriginalString);
	pRetVal =(char *) fast_memget(dMemId, (dSize+1), SIP_NULL);

	if (pRetVal == SIP_NULL)
		return SIP_NULL;
	strcpy(pRetVal, pOriginalString);
	return pRetVal;
}

#ifdef ICF_STACK_PORT

#ifdef SIP_VXWORKS
/* new functions for VxWorks porting of ICF */
SIP_U32bit sip_strcasecmp
	(const SIP_S8bit *pStr1, const SIP_S8bit *pStr2)
{
    SIP_U32bit							i = 0;
    SIP_U8bit							temp1 = 0;
    SIP_U8bit							temp2 = 0;

	while(pStr1[i])
	{ 
		/* lower case alphabets... ascii value ... 97 .. 122 */
		if ((pStr1[i] >= 97) && (pStr1[i] <= 122))
		{
			/* lower case - upper case = 32 */
			temp1 = pStr1[i] - 32;
		}
		else
		{
			temp1 = pStr1[i];
		}
		/* lower case alphabets... ascii value ... 97 .. 122 */
		if ((pStr2[i] >= 97) && (pStr2[i] <= 122))
		{
			/* lower case - upper case = 32 */
			temp2 = pStr2[i] - 32;
		}
		else
		{
			temp2 = pStr2[i];
		}

		if (temp1 != temp2)
			return(temp1 - temp2);
		i++;
	}

	if(pStr2[i])
		return(0 - pStr2[i]);

	return 0;
    
}/* function sip_strcasecmp() ends here */

SIP_U32bit sip_strncasecmp
	(SIP_S8bit *pStr1, SIP_S8bit *pStr2,SIP_U32bit n)
{
SIP_U32bit  							i = 0;
 SIP_U32bit								temp1 = 0;
	SIP_U32bit							temp2 = 0;

SIP_U32bit local_count = n;
    
	if (!local_count)
		return(0);
    
	while(pStr1[i])
	{ 
      		/* lower case alphabets... ascii value ... 97 .. 122 */
		if ((pStr1[i] >= 97) && (pStr1[i] <= 122))
		{
			/* lower case - upper case = 32 */
			temp1 = pStr1[i] - 32;
		}
		else
		{
			temp1 = pStr1[i];
		}
		/* lower case alphabets... ascii value ... 97 .. 122 */
		if ((pStr2[i] >= 97) && (pStr2[i] <= 122))
		{
			/* lower case - upper case = 32 */
			temp2 = pStr2[i] - 32;
		}
		else
		{
			temp2 = pStr2[i];
		}

		if (temp1 != temp2)
        {
			return(temp1 - temp2);
        }
		i++;
         --local_count;
        if (!local_count)
            break;


	}

    if (0 == local_count)
    {
        return 0;
    }
	if(pStr2[i])
    {
		return(0 - pStr2[i]);
    }

	return 0;

    
}/* sip_strncasecmp() ends here */

#endif/* SIP_VXWORKS defined */
#endif /*ICF_STACK_PORT*/

#ifndef SIP_VXWORKS
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

SIP_U32bit sip_strcasecmp
#ifdef ANSI_PROTO
	(const SIP_S8bit *pStr1, const SIP_S8bit *pStr2)
#else
	(pStr1, pStr2)
	SIP_S8bit *pStr1;
	SIP_S8bit *pStr2;
#endif
{
#if defined(SIP_WINDOWS)
	return (stricmp(pStr1, pStr2));
#elif defined (SIP_WINCE)
	return (_stricmp(pStr1, pStr2));
#else
	return strcasecmp(pStr1, pStr2);
#endif
}
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
SIP_U32bit sip_strncasecmp
#ifdef ANSI_PROTO
	(SIP_S8bit *pStr1, SIP_S8bit *pStr2,SIP_U32bit n)
#else
	(pStr1, pStr2)
	SIP_S8bit *pStr1;
	SIP_S8bit *pStr2;
#endif
{
#if defined(SIP_WINDOWS)
	return (strnicmp(pStr1, pStr2,n));
#elif defined SIP_WINCE
	return (_strnicmp(pStr1, pStr2,n));
#else
	return strncasecmp(pStr1, pStr2,n);
#endif
}
#endif /*SIP_VXWORKS*/

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

SIP_S8bit *sip_strncpy
#ifdef ANSI_PROTO
	(SIP_S8bit *pStr1, SIP_S8bit *pStr2, SIP_U32bit dSize)
#else
	(pStr1, pStr2, dSize)
	SIP_S8bit *pStr1;
	SIP_S8bit *pStr2;
	SIP_U32bit dSize;
#endif
{
	return strncpy(pStr1, pStr2, (size_t) dSize);
}
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

SIP_U32bit sip_strlen
#ifdef ANSI_PROTO
	(SIP_S8bit *pStr)
#else
	(pStr)
	SIP_S8bit *pStr;
#endif
{
	return ((SIP_U32bit) strlen(pStr));
}
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

SIP_S8bit *sip_strstr
#ifdef ANSI_PROTO
	(SIP_S8bit *pStr1, SIP_S8bit *pStr2)
#else
	(pStr1, pStr2)
	SIP_S8bit *pStr1;
	SIP_S8bit *pStr2;
#endif
{
	return strstr(pStr1, pStr2);
}
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

SIP_U32bit sip_sprintf
#ifdef ANSI_PROTO
	(SIP_S8bit *pStr, SIP_S8bit *pFmt, ...)
#else
	(pStr, pFmt, ...)
	SIP_S8bit *pStr;
	SIP_S8bit *pFmt;
#endif
{
	va_list  ap;
	va_start(ap,pFmt);
	vsprintf(pStr,pFmt,ap);
	va_end(ap);

	return 1;
}



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

SIP_S8bit* sip_strcat
#ifdef ANSI_PROTO
	(SIP_S8bit *pDest,SIP_S8bit *pSrc)
#else
	(pDest,pSrc)
	SIP_S8bit *pDest;
	SIP_S8bit *pSrc;
#endif
{
	return strcat(pDest,pSrc);
}

/*******************************************************************************
** FUNCTION:sip_strncat
** DESCRIPTION: wrapper function around C strncat function.
** 
** PARAMETERS:
**			pDest (OUT) :the string to which another string is to be added
**			pSrc  (IN) :the string , which must be appended to pDest
**			dSize (IN)	 : maximum number of bytes to be appended
** Return Value:
**			SIP_S8bit *
** 
*******************************************************************************/

SIP_S8bit* sip_strncat
#ifdef ANSI_PROTO
	(SIP_S8bit *pDest,SIP_S8bit *pSrc,SIP_U32bit dSize)
#else
	(pDest,pSrc)
	SIP_S8bit *pDest;
	SIP_S8bit *pSrc;
    SIP_U32bit dSize;
#endif
{
	return strncat(pDest,pSrc,(size_t)dSize);
}

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
SIP_S8bit* sip_strcpy
#ifdef ANSI_PROTO
	(SIP_S8bit *pDest,SIP_S8bit *pSrc)
#else
	(pDest,pSrc)
	SIP_S8bit *pDest;
	SIP_S8bit *pSrc;
#endif
{
	return strcpy(pDest,pSrc);
}

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
#ifdef ICF_STACK_PORT

void sip_getTimeString
#ifdef ANSI_PROTO
	(SIP_S8bit **ppString)
#else
	(pString)
	SIP_S8bit **ppString;
#endif
{
/*
 * Use the time function according to 
 * the OS
 */
#ifdef ICF_PORT_WINCE

/*	unsigned long ti;

	char *timeline;

	time(&ti);

	timeline = ctime(&ti);

	strncpy(*pString,timeline,SIP_MAX_CTIME_R_BUF_SIZE-1);

	(*pString)[SIP_MAX_CTIME_R_BUF_SIZE-1] = '\0';*/

	SIP_U16bit *pDateStr,*pTimeStr,*pTempStr;
	SYSTEMTIME dSysTime;
	SIP_S8bit *pCharTime;
	SIP_S32bit dLen;

	pDateStr = NULL;
	pTimeStr = NULL;
	pTempStr = NULL;
	pCharTime = NULL;


	GetLocalTime(&dSysTime);
			
	dLen = GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &dSysTime,_T("ddd MMM dd yyyy"),pDateStr,0 );
	pDateStr = (TCHAR *)malloc(dLen * sizeof(TCHAR));
	memset(pDateStr, 0, dLen * sizeof(TCHAR));
	GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &dSysTime,_T("ddd MMM dd yyyy"),pDateStr,dLen );
		
	dLen = 0;
	dLen = GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER, &dSysTime, NULL, pTimeStr, 0 );
	pTimeStr = (TCHAR *)malloc(dLen * sizeof(TCHAR));
	memset(pTimeStr, 0, dLen * sizeof(TCHAR));
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER, &dSysTime, NULL, pTimeStr, dLen );
		

	dLen = 0;
	dLen = wcslen(pDateStr) + wcslen(pTimeStr) + wcslen(_T(" ")) + wcslen(_T("\0")); 
	pTempStr = (TCHAR *)malloc(dLen * sizeof(TCHAR));
	memset(pTempStr, 0, dLen * sizeof(TCHAR));
	wcscpy(pTempStr,pDateStr);
	wcscat(pTempStr,_T(" "));
	wcscat(pTempStr,pTimeStr);
	
	pCharTime = sip_wideCharToMultiByte(pTempStr);

	sip_strcpy(*ppString,pCharTime);
	free(pDateStr);
	free(pTimeStr);
	free(pTempStr);
	free(pCharTime);

#else  /* else for win-ce port */

#ifdef ICF_PORT_WINDOWS

/* Get the time from the system -- vidyut */

	struct _timeb timebuffer;

	char *timeline;

	_ftime( &timebuffer );

    timeline = ctime( & ( timebuffer.time ) );

	strncpy(*ppString,timeline,SIP_MAX_CTIME_R_BUF_SIZE-1);

	(*ppString)[SIP_MAX_CTIME_R_BUF_SIZE-1] = '\0';

#else		/* else for windows port */

		char *pTime;
	char buffer[26];


#ifdef SIP_VXWORKS
	size_t buffer_len = 26;

    struct timespec             ts;
    clock_gettime(CLOCK_REALTIME, &ts);
	
	pTime = ctime_r(&ts.tv_sec,buffer,&buffer_len);
#else
struct timeval tv;

gettimeofday(&tv,SIP_NULL);

	pTime = ctime_r(&tv.tv_sec,buffer);
#endif
	strncpy(*ppString,pTime,26);
#endif /* end of WINDOWS port */
#endif /* end of WIN_CE PORT */

}

#else /*ifndef ICF_STACK_PORT*/

void sip_getTimeString
#ifdef ANSI_PROTO
	(SIP_S8bit **pString)
#else
	(pString)
	SIP_S8bit **pString;
#endif
{

/*
 * Use the time function according to 
 * the OS
 */

	struct timeval tv;
	char *pTime;
	char buffer[26];
	(void)gettimeofday(&tv,SIP_NULL);
	pTime = ctime_r(&tv.tv_sec,buffer);
	strncpy(*pString,pTime,26);
}

#endif /*ICF_STACK_PORT*/


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

SIP_U32bit STRTOU32CAP
#ifdef ANSI_PROTO
	( SIP_S8bit *str, SipError *pErr )
#else
	(str, pErr)
	SIP_S8bit *str;
	SipError *pErr;
#endif
{
#ifdef ICF_STACK_PORT

#if defined(ICF_PORT_WINCE) 
	unsigned long result;
	/*
	 * converting string to number
	 */
	result= strtoul(str,NULL,10);

	if(result == LONG_MAX || result == LONG_MIN)
	{
		if(pErr != SIP_NULL)
			*pErr = E_INV_RANGE;
		return MAX_LENGTH;
	}
	else
	{
		if(pErr != SIP_NULL)
			*pErr = E_NO_ERROR;
		return result;
	}
#else  /* else of ICF_PORT_WINCE */

#if defined(SIP_SOLARIS) || defined(SIP_LINUX) || defined(SIP_VXWORKS) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_SYMBIAN) 

	unsigned long result;
	errno=0;
	/*
	 * converting string to number
	 */
	result= strtoul(str,NULL,10);
	if(errno == ERANGE)
	{
		if(pErr != SIP_NULL)
			*pErr = E_INV_RANGE;
		return MAX_LENGTH;
	}
	else
	{
		if(pErr != SIP_NULL)
			*pErr = E_NO_ERROR;
		return result;
	}
#endif
#endif /* end for WINCE flag*/

#else /*ifndef ICF_STACK_PORT*/

#if defined(SIP_SOLARIS) || defined(SIP_LINUX) || defined(SIP_VXWORKS) || defined(SIP_WINDOWS) 
	unsigned long result;
	errno=0;
	/*
	 * converting string to number
	 */
	result= strtoul(str,NULL,10);
	if(errno == ERANGE)
	{
		if(pErr != SIP_NULL)
			*pErr = E_INV_RANGE;
		return MAX_LENGTH;
	}
	else
	{
		if(pErr != SIP_NULL)
			*pErr = E_NO_ERROR;
		return result;
	}
#endif

#endif /*ICF_STACK_PORT*/
}


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
#ifdef SIP_THREAD_SAFE
void fast_free_synch (synch_id_t *mutex)
{
	/* Port according to system and library selection */
#ifdef SIP_SOLARIS
    pthread_mutex_destroy(mutex);
#endif
#ifdef SIP_LINUX
    pthread_mutex_destroy(mutex);
#endif
#ifdef SIP_VXWORKS
	semDelete(*mutex);
#endif

#if defined (SIP_WINDOWS) || defined (SIP_WINCE)
#ifdef SIP_CRITICAL_SECTION
 DeleteCriticalSection(mutex); 
#else
 (void)ReleaseMutex(*mutex);
 (void)CloseHandle(*mutex);
#endif
#endif/*SIP_WINDOWS*/
 (void)sip_trace(SIP_Detailed,SIP_Init,
					(SIP_S8bit *)"STACK TRACE: Destroy Mutex Lock");

}
#endif

#ifdef ICF_STACK_PORT

/* These are required by Vxworks Port */
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
	(SIP_U16bit *pWbuf)
#else
	(pWbuf)
	SIP_U16bit *pWbuf;
#endif
{

	SIP_S8bit *pBuf;
	SIP_S32bit dLen;

	pBuf = NULL;
	dLen = WideCharToMultiByte(CP_ACP, 0, pWbuf, -1, pBuf, 0,NULL,NULL);
	
	pBuf = (char *)malloc(dLen * sizeof(char));
	memset(pBuf, 0, dLen * sizeof(char));
	dLen = WideCharToMultiByte(CP_ACP, 0, pWbuf, -1, pBuf, dLen ,NULL,NULL);
	return(pBuf);
}

#endif /* SIP_WINCE */
#endif /*ICF_STACK_PORT*/


#ifdef __cplusplus
}
#endif 

